#include <string.h>
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>
#include<bits/stdc++.h>
using namespace std;

/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData {
  GstDiscoverer *discoverer;
  GMainLoop *loop;
  GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *resample;
    GstElement *sink;
} CustomData;
gchar *adec;
gchar *vdec;
static void pad_added_handler (GstElement *src, GstPad *pad, CustomData *data);


/* Print a tag in a human-readable format (name: value) */
static void print_tag_foreach (const GstTagList *tags, const gchar *tag, gpointer user_data) {
  GValue val = { 0, };
  gchar *str;
  gint depth = GPOINTER_TO_INT (user_data);

  gst_tag_list_copy_value (&val, tags, tag);

  if (G_VALUE_HOLDS_STRING (&val))
    str = g_value_dup_string (&val);
  else
    str = gst_value_serialize (&val);

  if(strcmp(gst_tag_get_nick (tag),"audio codec")==0)
  {
	  cout<<str<<endl;
	  adec=g_strdup(str);
	  cout<<"-----------------------------------------------"<<adec<<endl;
  }
  g_print ("%*s%s: %s\n", 2 * depth, " ", gst_tag_get_nick (tag), str);
  g_free (str);

  g_value_unset (&val);
}

/* Print information regarding a stream */
//static void print_stream_info (GstDiscovererStreamInfo *info, gint depth) {
//  gchar *desc = NULL;
//  GstCaps *caps;
//  const GstTagList *tags;
//
//  caps = gst_discoverer_stream_info_get_caps (info);
//
//  if (caps) {
//    if (gst_caps_is_fixed (caps))
//      desc = gst_pb_utils_get_codec_description (caps);
//    else
//      desc = gst_caps_to_string (caps);
//    gst_caps_unref (caps);
//  }
//
//  g_print ("%*s%s: %s\n", 2 * depth, " ", gst_discoverer_stream_info_get_stream_type_nick (info), (desc ? desc : ""));
//
//  if (desc) {
//    g_free (desc);
//    desc = NULL;
//  }
//
//  tags = gst_discoverer_stream_info_get_tags (info);
//  if (tags) {
//    g_print ("%*sTags:\n", 2 * (depth + 1), " ");
//    gst_tag_list_foreach (tags, print_tag_foreach, GINT_TO_POINTER (depth + 2));
//  }
//}

/* Print information regarding a stream and its substreams, if any */
static void print_topology (GstDiscovererStreamInfo *info, gint depth) {
  GstDiscovererStreamInfo *next;

  if (!info)
    return;

 // print_stream_info (info, depth);

  next = gst_discoverer_stream_info_get_next (info);
  if (next) {
    print_topology (next, depth + 1);
    gst_discoverer_stream_info_unref (next);
  } else if (GST_IS_DISCOVERER_CONTAINER_INFO (info)) {
    GList *tmp, *streams;

    streams = gst_discoverer_container_info_get_streams (GST_DISCOVERER_CONTAINER_INFO (info));
    for (tmp = streams; tmp; tmp = tmp->next) {
      GstDiscovererStreamInfo *tmpinf = (GstDiscovererStreamInfo *) tmp->data;
      print_topology (tmpinf, depth + 1);
    }
    gst_discoverer_stream_info_list_free (streams);
  }
}

/* This function is called every time the discoverer has information regarding
 * one of the URIs we provided.*/
static void on_discovered_cb (GstDiscoverer *discoverer, GstDiscovererInfo *info, GError *err, CustomData *data) {
  GstDiscovererResult result;
  const gchar *uri;
  const GstTagList *tags;
  GstDiscovererStreamInfo *sinfo;

  uri = gst_discoverer_info_get_uri (info);
  result = gst_discoverer_info_get_result (info);
  switch (result) {
    case GST_DISCOVERER_URI_INVALID:
      g_print ("Invalid URI '%s'\n", uri);
      break;
    case GST_DISCOVERER_ERROR:
      g_print ("Discoverer error: %s\n", err->message);
      break;
    case GST_DISCOVERER_TIMEOUT:
      g_print ("Timeout\n");
      break;
    case GST_DISCOVERER_BUSY:
      g_print ("Busy\n");
      break;
    case GST_DISCOVERER_MISSING_PLUGINS:{
      const GstStructure *s;
      gchar *str;

      s = gst_discoverer_info_get_misc (info);
      str = gst_structure_to_string (s);

      g_print ("Missing plugins: %s\n", str);
      g_free (str);
      break;
    }
    case GST_DISCOVERER_OK:
      g_print ("Discovered '%s'\n", uri);
      break;
  }

  if (result != GST_DISCOVERER_OK) {
    g_printerr ("This URI cannot be played\n");
    return;
  }

  /* If we got no error, show the retrieved information */

  g_print ("\nDuration: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS (gst_discoverer_info_get_duration (info)));

  tags = gst_discoverer_info_get_tags (info);
  if (tags) {
    g_print ("Tags:\n");
    gst_tag_list_foreach (tags, print_tag_foreach, GINT_TO_POINTER (1));
  }

  g_print ("Seekable: %s\n", (gst_discoverer_info_get_seekable (info) ? "yes" : "no"));

  g_print ("\n");

  sinfo = gst_discoverer_info_get_stream_info (info);
  if (!sinfo)
    return;

 // g_print ("Stream information:\n");

  print_topology (sinfo, 1);

  gst_discoverer_stream_info_unref (sinfo);

  g_print ("\n");
}

/* This function is called when the discoverer has finished examining
 * all the URIs we provided.*/
static void on_finished_cb (GstDiscoverer *discoverer, CustomData *data) {
  g_print ("Finished discovering\n");

  g_main_loop_quit (data->loop);
}

int main (int argc, char **argv) {
  CustomData data;
  GError *err = NULL;
  gchar *uri = "file:///home/ee212934/assignment/videos/v16.mp4";

  /* if a URI was provided, use it instead of the default one */
//  if (argc > 1) {
//    uri = argv[1];
//  }

  /* Initialize custom data structure */
 // memset (&data, 0, sizeof (data));

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  g_print ("Discovering '%s'\n", uri);

  /* Instantiate the Discoverer */
  data.discoverer = gst_discoverer_new (5 * GST_SECOND, &err);
  if (!data.discoverer) {
    g_print ("Error creating discoverer instance: %s\n", err->message);
    g_clear_error (&err);
    return -1;
  }

  /* Connect to the interesting signals */
  g_signal_connect (data.discoverer, "discovered", G_CALLBACK (on_discovered_cb), &data);
  g_signal_connect (data.discoverer, "finished", G_CALLBACK (on_finished_cb), &data);

  /* Start the discoverer process (nothing to do yet) */
  gst_discoverer_start (data.discoverer);

  /* Add a request to process asynchronously the URI passed through the command line */
  if (!gst_discoverer_discover_uri_async (data.discoverer, uri)) {
    g_print ("Failed to start discovering URI '%s'\n", uri);
    g_object_unref (data.discoverer);
    return -1;
  }

  /* Create a GLib Main Loop and set it to run, so we can wait for the signals */
  data.loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (data.loop);

  /* Stop the discoverer process */
  gst_discoverer_stop (data.discoverer);

  /* Free resources */
  g_object_unref (data.discoverer);
  g_main_loop_unref (data.loop);

  //g_print(adec);
 // g_print ("%*s%s: %s\n", 2 * depth, " ", gst_tag_get_nick (tag), str);
 // g_print("%s",adec);
  cout<<adec<<"-------------------------------"<<endl;
  g_print("%s",adec);

    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    /* Create the elements */
    data.source = gst_element_factory_make ("uridecodebin", "source");
    data.convert = gst_element_factory_make ("audioconvert", "convert");
    data.resample = gst_element_factory_make ("audioresample", "resample");
    data.sink = gst_element_factory_make ("autoaudiosink", "sink");

    /* Create the empty pipeline */
    data.pipeline = gst_pipeline_new ("test-pipeline");

    if (!data.pipeline || !data.source || !data.convert || !data.resample || !data.sink) {
      g_printerr ("Not all elements could be created.\n");
      return -1;
    }

    /* Build the pipeline. Note that we are NOT linking the source at this
     * point. We will do it later. */
    gst_bin_add_many (GST_BIN (data.pipeline), data.source, data.convert, data.resample, data.sink, NULL);
    if (!gst_element_link_many (data.convert, data.resample, data.sink, NULL)) {
      g_printerr ("Elements could not be linked.\n");
      gst_object_unref (data.pipeline);
      return -1;
    }

    /* Set the URI to play */
    g_object_set (data.source, "uri", "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", NULL);

    /* Connect to the pad-added signal */
    g_signal_connect (data.source, "pad-added", G_CALLBACK (pad_added_handler), &data);

    /* Start playing */
    ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
      g_printerr ("Unable to set the pipeline to the playing state.\n");
      gst_object_unref (data.pipeline);
      return -1;
    }

    /* Listen to the bus */
    bus = gst_element_get_bus (data.pipeline);
    do {
      msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
         (GstMessageType) (GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

      /* Parse message */
      if (msg != NULL) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE (msg)) {
          case GST_MESSAGE_ERROR:
            gst_message_parse_error (msg, &err, &debug_info);
            g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
            g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
            g_clear_error (&err);
            g_free (debug_info);
            terminate = TRUE;
            break;
          case GST_MESSAGE_EOS:
            g_print ("End-Of-Stream reached.\n");
            terminate = TRUE;
            break;
          case GST_MESSAGE_STATE_CHANGED:
            /* We are only interested in state-changed messages from the pipeline */
            if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data.pipeline)) {
              GstState old_state, new_state, pending_state;
              gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
              g_print ("Pipeline state changed from %s to %s:\n",
                  gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
            }
            break;
          default:
            /* We should not reach here */
            g_printerr ("Unexpected message received.\n");
            break;
        }
        gst_message_unref (msg);
      }
    } while (!terminate);

    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);
    return 0;
}



static void pad_added_handler (GstElement *src, GstPad *new_pad, CustomData *data) {
  GstPad *sink_pad = gst_element_get_static_pad (data->convert, "sink");
  GstPadLinkReturn ret;
  GstCaps *new_pad_caps = NULL;
  GstStructure *new_pad_struct = NULL;
  const gchar *new_pad_type = NULL;

  g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

  /* If our converter is already linked, we have nothing to do here */
  if (gst_pad_is_linked (sink_pad)) {
    g_print ("We are already linked. Ignoring.\n");
    goto exit;
  }

  /* Check the new pad's type */
  new_pad_caps = gst_pad_get_current_caps (new_pad);
  new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
  new_pad_type = gst_structure_get_name (new_pad_struct);
  if (!g_str_has_prefix (new_pad_type, "audio/x-raw")) {
    g_print ("It has type '%s' which is not raw audio. Ignoring.\n", new_pad_type);
    goto exit;
  }

  /* Attempt the link */
  ret = gst_pad_link (new_pad, sink_pad);
  if (GST_PAD_LINK_FAILED (ret)) {
    g_print ("Type is '%s' but link failed.\n", new_pad_type);
  } else {
    g_print ("Link succeeded (type '%s').\n", new_pad_type);
  }

exit:
  /* Unreference the new pad's caps, if we got them */
  if (new_pad_caps != NULL)
    gst_caps_unref (new_pad_caps);

  /* Unreference the sink pad */
  gst_object_unref (sink_pad);
}






























//#include <gst/gst.h>
//
//int main(int argc, char *argv[]) {
//    GstElement *pipeline;
//    GstBus *bus;
//    GstMessage *msg;
//    GstFormat format = GST_FORMAT_TIME;
//    GstSeekFlags flags = GST_SEEK_FLAG_FLUSH;
//    GstSeekType type = GST_SEEK_TYPE_SET;
//    gint64 position =  (7000*5)* GST_SECOND;
//    gboolean ret;
//
//    /* Initialize GStreamer */
//    gst_init(&argc, &argv);
//
//    /* Create the pipeline */
//    pipeline = gst_parse_launch("playbin uri=file:///home/ee212934/Downloads/sam.mp4", NULL);
//
//    /* Set the pipeline to the playing state */
//    gst_element_set_state(pipeline, GST_STATE_PLAYING);
//
//    /* Wait for the pipeline to reach the playing state */
//    ret = gst_element_get_state(pipeline, NULL, NULL, GST_CLOCK_TIME_NONE);
//    if (ret != GST_STATE_CHANGE_SUCCESS) {
//            g_print("Failed to start pipeline\n");
//            gst_object_unref(pipeline);
//            return 1;
//        }
//
//    bus = gst_element_get_bus(pipeline);
//    while ((msg = gst_bus_pop(bus)) != NULL) {
//           gst_message_unref(msg);
//       }
//
//
//    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
//            (GstMessageType)(GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
//    if (msg != NULL)
//    {
//        gst_message_unref(msg);
//    }
//
//  //  ret = gst_element_seek_simple(pipeline, format, flags, position);
//    ret = gst_element_seek_simple(pipeline, format, flags, position);
//        if (!ret) {
//            g_print("Failed to seek pipeline\n");
//        }
//
//        msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
//                   (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
//           if (msg != NULL) {
//               gst_message_unref(msg);
//           }
//
//
//  //  gst_object_unref(bus);
//   // GstFormat format = GST_FORMAT_TIME;
//    /* Seek to the beginning of the stream with flush */
//  //  ret = gst_element_seek_simple(pipeline,flags, type, position);
//   // ret = gst_element_seek_simple(pipeline,format,flags,type,position,GST_SEEK_TYPE_NONE,0);
//  //  ret = gst_element_seek_simple(pipeline, format, flags, position);
//
////    if (!ret) {
////        g_print("Failed to flush pipeline\n");
////    }
//
//
////    ret = gst_element_seek_simple(pipeline, format, flags, type, position, GST_SEEK_TYPE_NONE, 0);
////          if (!ret) {
////              g_print("Failed to seek pipeline\n");
////          }
////
//
//    /* Wait for the pipeline to finish */
////    bus = gst_element_get_bus(pipeline);
////    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
////    		(GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
////    if (msg != NULL) {
////        gst_message_unref(msg);
////    }
//
//
//
//
//
//
//
//
//
//
//
//    /* Clean up */
//    gst_element_set_state(pipeline, GST_STATE_NULL);
//    gst_object_unref(bus);
//    gst_object_unref(pipeline);
//
//    return 0;
//}

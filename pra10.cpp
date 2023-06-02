#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<stdio.h>
#include<stdlib.h>
#include<bits/stdc++.h>
#include <sys/wait.h>
#include<sys/types.h>
#include<signal.h>
using namespace std;
void handler(int sig)
{
    if(sig==SIGINT)
    {
        cout<<"ctrl-c pressed "<<endl;
    }
    if(sig==SIGTSTP)
    {
        cout<<"ctrl+z is pressed"<<endl;
    }
    cout<<"ALARM"<<endl;
    alarm(5);
    // if(sig==SIGTERM)
    // {
    //     cout<<""<<endl;
    // }
  exit(EXIT_SUCCESS);
}
int main(void)
{
 //   cout<<"this is parent"<<getpid()<<endl;
    signal(SIGINT,handler);
    signal(SIGTSTP,handler);
  //  signal(SIGTERM,handler);
  //  signal(SIGKILL,handler);
    signal(SIGALRM,handler);
    alarm(5);
    int i=1;
    while(i>0)
    {
        i++;
        cout<<i<<endl;
        sleep(1);
    }
    return 0;
}
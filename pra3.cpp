#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
using namespace std;
int main()
{
    //O_TRUNC :- write to file from beginning, delete the old content
   // O_APPEND :- write the contentsof the file from the end of the  existing file content.
  int fd=open("f1",O_RDWR|O_APPEND,0777);
  cout<<"Enter Name:-";
  string s;
  if(fd==-1)
  exit(0);
// cin.getline(s,'');
getline(cin,s);
  //cin>>s;
  char sc[s.length()+1];
 // cout<<s;
  strcpy(sc,s.c_str());
  int wr=write(fd,sc,s.length());
  //getline(cin,s);

}
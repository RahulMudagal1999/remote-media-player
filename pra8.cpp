#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<stdio.h>
#include<stdlib.h>
#include<bits/stdc++.h>
#include <sys/wait.h>
using namespace std;//demonstrate of Waitpid() system call
int main()
{
    pid_t pid1=fork();
    //pid_t pid2=fork();
    if(pid1==0)
    {
       
        cout<<"this is child1   "<<getpid()<<endl;
         sleep(3);
        cout<<"exited1"<<endl;
        return 0;
    }
    pid_t pid2=fork();
    if(pid2==0)
    {
        cout<<"this is child2   "<<getpid()<<endl;
        sleep(10);
          cout<<"exited2"<<endl;
        return 0;
    }

        pid_t p1=waitpid(pid1,NULL,0);
      pid_t p2=waitpid(pid2,NULL,0);
        cout<<"this is child1    "<<p1<<endl;
         // cout<<"this is child2   "<<p2<<endl;
          cout<<"rhis is parent:"<<getpid()<<endl;
    
    return 0;
}
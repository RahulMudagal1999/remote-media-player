#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<stdio.h>
#include<stdlib.h>
#include<bits/stdc++.h>
#include <sys/wait.h>
using namespace std;//demonstrate of Wait() system call
int main()
{
   int s;
    int pid=fork();  
    int state;
    if(pid==0)
    {
        cout<<"this is child:"<<getpid()<<endl;
        int pid1=fork();
        if(pid1==0)
        {
            cout<<"this is grand children"<<endl;
        }
        else
        {
            cout<<"this is childs child"<<endl;
        }

       // exit(1);
    }
    else
    {
      
        cout<<"this is parent:"<<getpid()<<endl;
       s=wait(&state);
       cout<<"status"<<s<<endl;   
         cout<<"terminated"<<endl;
    }

    return 0;
}
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<stdio.h>
#include<stdlib.h>
#include<bits/stdc++.h>
#include <sys/wait.h>
#include<sys/types.h>
using namespace std;//Zombies and orphne
int main()
{
    pid_t p=fork();
    if(p==0)
    {
        cout<<"this is child"<<getpid()<<endl;
        cout<<"child end"<<endl;

    }
    else
    {
        sleep(10);
        cout<<"this is parent"<<getpid()<<endl;
        cout<<"parent end";
    }
    return 0;
}
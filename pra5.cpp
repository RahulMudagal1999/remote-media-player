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
    int fd=open("f1",O_RDWR,0777);
    if(fd==-1)
    {
        cout<<"error occurd"<<endl;
    }
    else
    {
        cout<<"open success"<<endl;
    }
    // int fd1=dup(fd);
    // cout<<fd<<endl;
    // cout<<fd1<<endl;
    close(1);
    int fd1=dup(fd);
    cout<<"HI RAHUL HOW YOU DOING";
//     int fd=open("f1",O_RDONLY,0777);
//     char s[20];
//     read(fd,s,5);
    
    
//     string str=s;
//     cout<<str;
//     cout<<endl;
//    // lseek(fd,6,SEEK_CUR);//SEEK_CUR :-used to form current position
//    //lseek(fd,10,SEEK_SET);//SEEK_SET:- starts form the beginning (skips 10 bytes from  beginning)
//     //lseek(fd,-9,SEEK_END);//SEEK_END:- start from the last 
//     lseek(fd,10,SEEK_HOLE);
//     read(fd,s,5);
//     str=s;
//     cout<<str;
}
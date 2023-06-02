#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<stdio.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace std;
int main()
{
    cout<<getpid()<<endl;
    int child=fork();
    //cout<<getpid()<<endl;
    if(child!=0)
    cout<<child<<endl;
    return 0;
}

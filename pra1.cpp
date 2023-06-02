#include<bits/stdc++.h>
#include<unistd.h>
#include<fcntl.h>
using namespace std;
// int main()
// {
// int fd;
// fd=open("f1",O_CREAT|O_RDONLY,0777);
// char s[1]={0};
// string s1;
// if(fd!=-1)
// while((read(fd,s,1)))
// {
//     s1+=s;
// }
// else{
//     cout<<"FILE DOES NOT OPEN"<<endl;
// }
// cout<<s1;
// }
int main()
{
    string s="rhaul";
    int fo;
    int x=25;
    vector<string> vect={"rhaul","asd","adsasd"};
    fo=open("f1",O_CREAT|O_RDONLY,0777);
    if(fo==-1)
    {
        //cout<<"ERROR IN OPENING FILE"<<endl;
        cout<<errno<<endl;
        perror("ERROR:-");
    }
    else
    {
        int a=10;
        for(int i=0;i<5;i++)
        {
            x++;
        }
        cout<<fo<<endl;
        char st[100]={0};
        
        char st1[10]={0};
        int fw=read(fo,st,20);//Read System call
        int fw1=read(fo,st1,10);
        if(fw>-1)
        {
            cout<<st<<endl;
            cout<<st1<<endl;
        }
    }
}

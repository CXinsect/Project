#include "model.h"
#include "FileOptions.h"
#include "Write_Read.h"
#include<iostream>
#include<fstream>
static long num = 1;

//处理文件操作
void File_Opt::Do_File (int sockfd) {
    int n = 0;
    cout<<"sockfd:: "<<sockfd<<endl;
    struct packet bufp;
    memset(&bufp,0,sizeof(bufp));
    int sn = 0;
    while((sn = readn(sockfd,&bufp,sizeof(bufp)))) {
        if(sn == -1) {
            cout<<"read 1 err"<<endl;
            close(sockfd);
            return;
        }
        cout<<"file_opt content: "<<bufp.buf<<endl;
        switch(bufp.Rtype) {
            case OPEN:
                Open_Do(sockfd,&bufp);
                break;
            case CLOSE:
            cout<<"close in"<<endl;
                Close_Do(sockfd,&bufp);
                break;
            case ALIVE:
                Alive_Do(sockfd);
                break;
            default:
                cout<<"Invaild Package"<<endl;
                break;
        }
    }
    close(sockfd);
}
//处理读操作

void File_Opt::Open_Do (int sockfd,struct packet *bufp) {
    map <string,unsigned int> file;
    int fd;
    char buf[2048] = {0};
    int n = 0;
    cout<<"content"<<bufp->buf<<endl;
    int len = strlen(bufp->Amac);
    bufp->Amac[len - 1] = '\0';
    sprintf(buf,"%s+%s",bufp->Amac,bufp->pathname);
    cout<<"buf:"<<buf<<endl;
    if(file.find(buf) == file.end()) {
        auto ret = file.insert(make_pair(buf,num));
        char name[1024] = {0};
        sprintf(name,"%x",(unsigned int)num);
        cout<<"转换成的名字："<<name<<endl;
        char cwd[1024] = {0};
        if((fd = open(name,O_CREAT | O_RDWR | O_TRUNC,0666)) < 0) 
            ERR_EXIT("open err");
        writen(fd,bufp->buf,strlen(bufp->buf));
        cout<<"file content: "<<bufp->buf<<endl;
        int rdret;
        while((rdret = readn(sockfd,bufp,sizeof(struct packet))) > 0) {
            if(rdret == -1) {
                cout<<"readn 2 err"<<endl;
                return;
            }
            if(strlen(bufp->buf) == 0)
                break;
            cout<<"bufp: readn 2 "<<bufp->buf<<endl;
            writen(fd,bufp->buf,strlen(bufp->buf));
        }
        close(fd);
        char secret[] = "It's a sercet!";
        strcpy(bufp->buf,secret);
        cout<<"bufp pathname: "<<bufp->pathname<<endl;
        int wdret;
        wdret = writen(sockfd,bufp,sizeof(struct packet));
        if(wdret == -1) {
            cout<<"write err"<<endl;
            return;
        }
        cout<<sockfd<<endl;
        int fds = open("mapdd.txt",O_CREAT | O_RDWR,0666);
        if(fds < 0) 
            ERR_EXIT("open server err");
        char maptxt[1024];
        sprintf(maptxt,"%s %ld",buf,num);
        writen(fds,maptxt,strlen(maptxt));
        close(fds);

        cout<<"文件发送完毕"<<endl;
        num++;
    }
    else {
        char secret[] = "It's a sercet!";
        strcpy(bufp->buf,secret);
        if(writen(sockfd,bufp,sizeof(struct packet)) < 0) 
            ERR_EXIT("write server");
    }
   // close(sockfd);
}

//处理关闭请求
void File_Opt::Close_Do(int sockfd,struct packet *bufp) {
    char buf[2048] = {0};
    cout<<"name_close"<<endl;
    int len = strlen(bufp->Amac);
    bufp->Amac[len - 1] = '\0';
    sprintf(buf,"%s+%s",bufp->Amac,bufp->pathname);
    map <string,unsigned int> file;
    cout<<buf<<endl;
    ifstream ins("mapdd.txt");
    while(!ins.eof()) {
        string key;
        long value;
        ins>>key>>value;
        cout<<"key"<<key<<"value"<<value<<endl;
        file.insert(make_pair(key,value));
    }
    map <string,unsigned int>::iterator iter = file.begin();
    for(;iter != file.end();iter++) {
        cout<<iter->first<<" "<<iter->second<<endl;
    }
    cout<<"close"<<endl;
    auto ret = file.find(buf);
    if(ret != file.end()) {
        char name[1024] = {0};
        sprintf(name,"%x",ret->second);
        cout<<"name close: "<<name<<endl;
        int fd = open(name,O_RDONLY);
        if(fd < 0) 
            ERR_EXIT("open");
        int n;
        while((n = readn(fd,bufp->buf,sizeof(bufp->buf))) > 0) {
            if(n != sizeof(bufp->buf))
                bufp->buf[n] = 0;
            writen(sockfd,bufp,sizeof(struct packet));
            cout<<"buf mac: "<<bufp->Amac<<endl;
            cout<<"The content That has sent"<<bufp->buf<<endl;
        }
        close(fd);
        memset(bufp->buf,0,sizeof(bufp->buf));
        int ret = writen(sockfd,bufp,sizeof(struct packet));
        if(ret < 0) {
            cout<<"writen close err"<<endl;
            return;
        }
        cout<<"size: "<<strlen(bufp->buf)<<"buf: "<<bufp->buf<<endl;
    }
   // close(sockfd);
}

//处理心跳包
void File_Opt::Alive_Do(int sockfd) {
    int n;
    char c = 'c';
    if((n = write(sockfd,&c,1)) < 0) 
        ERR_EXIT("Alive write err");
}
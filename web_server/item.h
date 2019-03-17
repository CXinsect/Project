#pragma once
#include <iostream>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;
#define EXIT_ERR(m) \
    do \
{ \
    perror("m"); \
    exit(0); \
}while(0)
#define MAXSIZE 2048
int recv_peek(int fd,void *buf,int size);
int sock_init(int port,int epfd);
void set_noblock(int confd);
void do_accept(int lfd,int epfd);
void  http_request_head(int confd,int no,const char * dsp,const char * type,long len);
void encode(char * to,int tosize,const char * from);
int Binary_Tran(char c);
void decode(char *to,char *from);
void send_dir(int confd,const char * dirname);
void send_file(int confd,const char * fname);
const char *get_file_type(const char *name);
void http_request(const char *str,int confd);
void disconnect(int confd,int epfd);
void do_read(int confd,int epfd);
void do_epoll(int port);

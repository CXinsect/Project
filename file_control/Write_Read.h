#ifndef __WRITE_HEAD_H_
#define __WRITE_HEAD_H_
#include "model.h"
//IO函数
int readn (int fd, char* buf,int size);
int writen (int fd,char *buf,int size);

#endif
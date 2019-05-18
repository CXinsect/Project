#ifndef _MODEL_H
#define _MODEL_H
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include <event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <limits.h>
#include <memory>
#include <semaphore.h>
#include <map>

//文件请求类型
#define OPEN 1
#define CLOSE 2
#define ALIVE 3
#define SHM_SIZE 1024
#define BUFF_SIZE (4 *1024)
#define ERR_EXIT(m) \
 do { \
    perror(m); \
    exit(EXIT_FAILURE); \
}while(0)
using namespace std;
#endif

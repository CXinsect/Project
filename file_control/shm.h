#ifndef _SHM_H_
#define _SHM_H_

#include "model.h"
#include <sys/ipc.h>
#include <limits.h>

//错误码
// #define ERR_BASE 100
// #define ERR_EEXIST (ERR_BASE + 1)
// #define ERR_PRAGRAM (ERR_BASE + 2)

enum error {ERR_BASE = 0,ERR_EEXIST,ERR_PRAGRAM};
//共享内存
int IPC_CreateShm(int key,int size,int *shmfd);
int IPC_MapShm(int shmfd, void **mapaddr);
int IPC_UnMapShm(void * unmapaddr);
int IPC_DelShm(int shmfd);
#endif
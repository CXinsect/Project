#include "shm.h"
#include <limits.h>
#define SHM_SIZE 1024
#define BUFF_SIZE PATH_MAX
#define MYPATH "."
#define PRO_ID 0x6666
struct shmseg {
    int cnt;
    char buf[BUFF_SIZE];
};
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                           (Linux specific) */
};
/***
 * 函数返回０代表成功-1代表出错
*/

int IPC_CreateShm(int size,int *shmfd) {
    
    key_t key = ftok(MYPATH,PRO_ID);  
    *shmfd = shmget(key,size,0666|IPC_CREAT |IPC_EXCL);
    if(*shmfd == -1) {
        return -1;
    }
    return 0;
}
int IPC_OpenShm(int size,int *shmid) {
     key_t key = ftok(MYPATH,PRO_ID);  
    *shmid = shmget(key,0,0);
     if(*shmid == -1) {
        return -1;
    }
    return 0;
}
int IPC_MapShm(int shmfd, void **mapaddr) {
    void *ptr;
    if(shmfd == -1) {
        errno = ERR_PRAGRAM;
        exit(0);
    }
    ptr = (void*)shmat(shmfd,0,SHM_RND);
    if(ptr == (void *)-1) 
        return -1;
    *mapaddr = ptr;
    return 0;
}
int IPC_UnMapShm(void * unmapaddr) {
    int ret;
    ret = shmdt((char*)unmapaddr);
    if(ret == -1) 
        return -1;
    return 0;
}
int IPC_DelShm(int shmfd) {
    int ret;
    ret = shmctl(shmfd,IPC_RMID,NULL);
    if(ret < 0)
        return -1;
    return 0;
}
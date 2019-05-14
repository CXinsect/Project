#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "shm.cpp"
/*
    hook的目标是open，所以typedef了一个OPEN函数指针
    hook的目的是要控制函数行为，从原库libc.so.6中拿到OPEN指针，保存成old_open以备调用
*/
typedef int(*OPEN)(const char *pathname, int flags, ...);

int open(const char *pathname, int flags, ...)
{
    static void *handle = NULL;
    static OPEN old_open = NULL;

    if(!handle )
    {
        handle = dlopen("libc.so.6", RTLD_LAZY);
        old_open = (OPEN)dlsym(handle, "open");
    }
    /***
     * 业务处理函数
     */
    int shmid;
    cout<<"ddddddd"<<endl;
    int ret = IPC_CreateShm(SHM_SIZE,&shmid);
    if(ret < 0) {
        if(errno == EEXIST) {
            cout<<"共享内存已经存在"<<endl;
            IPC_OpenShm(0,&shmid);
            //exit(0);
        }
        else{
            cout<<"出错了"<<endl;
            exit(0);
        }
        
    }
    struct shmseg * shmp;
    ret = IPC_MapShm(shmid,(void**)&shmp);
    char *pstr;
    //判断问价属性
    struct stat sb;
    ssize_t numBytes;
    char buf[BUFF_SIZE] = {0};
    getcwd(buf,sizeof(buf));
    cout<<"buf: "<<buf<<" "<<pathname<<endl;
    if(realpath(pathname,buf) == NULL) {
        cout<<strerror(errno)<<endl;
        ERR_EXIT("realpath");
    }
    cout<<"ddd"<<endl;
    size_t len = strlen(buf);
    cout<<len<<endl;
    memcpy(shmp->buf,buf,len);
    printf("string: %s\n",shmp->buf);
    return old_open(pathname,flags);
}

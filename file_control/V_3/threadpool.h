#include <list>
#include <cstdio>
#include <exception>
#include "locker.h"
template <typename T>

class ThreadPool
{
public:
    ThreadPool(int num_pthread = 4,int num_max_pthread = 1000);
    bool append(T * requeset,int &sockfd); //添加任务
    ~ThreadPool();
private:
    static void *worker(void * arg);
    void run();
private:
    //线程数量
    int num_pthread;
    //最大线程数量
    int num_max_pthread;
    //线程数组
    pthread_t * threads;
    //任务列表
    list <T *> w_queue;
    //锁
    Locker lw_queue;
    //信号量
    Sem stat;
    //
    bool stop;
    int sockfd;
};
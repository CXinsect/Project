#include <list>
#include <cstdio>
#include <exception>
#include "locker.h"
template <typename T>
class ThreadPool
{
public:
    ThreadPool(int num_pthread = 8,int num_max_pthread = 1000);
    bool append(T * requeset,int sockfd); //添加任务
    ~ThreadPool();
private:
    static void *worker(void * arg);
    void run();
private:
    int num_pthread;
    int num_max_pthread;
    pthread_t * threads;
    list <T *> w_queue;
    Locker lw_queue;
    Sem stat;
    bool stop;
    int sockfd;
};
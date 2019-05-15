#include "threadpool.h"

template <typename T>

ThreadPool <T> ::ThreadPool(int pthread,int max_pthread) : 
    num_pthread(pthread),num_max_pthread(max_pthread),threads(NULL),
    stop(false) {
    
        if(pthread <= 0 || max_pthread <= 0) {
            throw exception();
        }
        threads = new pthread_t [max_pthread];
        if(!threads) {
            cout<<"Memory Error"<<endl;
            throw exception();
        }
        for(int i = 0; i < num_pthread;i++) {
            if(pthread_create(threads + i,NULL,worker,this)) {
                ERR_EXIT("pthread_create err");
                delete [] threads;
                throw exception();
            }
            if(pthread_detach(threads[i])) {
                ERR_EXIT("pthread_detach err");
                delete [] threads;
                throw exception();
            }
        }
}
template <typename T> 
ThreadPool <T>::~ThreadPool() {
    delete [] threads;
    stop = true;
}
template <typename T>
bool ThreadPool<T>::append(T * request,int _sockfd) {
    lw_queue.lock();
    if(w_queue.size() > num_max_pthread) {
        lw_queue.unlock();
        return false;
    }
    w_queue.push_back(request);
    sockfd = _sockfd;
    lw_queue.unlock();
    stat.post();
    return true;
}
template <typename T>
void * ThreadPool <T>::worker(void * arg) {
    ThreadPool * pool = (ThreadPool *)arg;
    pool->run();
    return pool;
}
template <typename T>
void ThreadPool<T>::run() {
    while(!stop) {
        stat.wait();
        lw_queue.lock();
        if(w_queue.empty()) {
            lw_queue.unlock();
            continue;
        }
        T * request = w_queue.front();
        
        w_queue.pop_front();
        lw_queue.unlock();
        
        if(!request) {
            continue;
        }
        request->Do_File(sockfd);
    }
}
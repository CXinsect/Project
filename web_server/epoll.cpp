#include "item.h"
void do_epoll(int port)
{
    int epfd = epoll_create(MAXSIZE);
    //初始化并将监听套结子挂到epoll树上
    int sockfd = sock_init(port,epfd);
    struct epoll_event all[MAXSIZE];
    while(1)
    {
        int ret = epoll_wait(epfd,all,MAXSIZE,-1);
        if(ret < 0)
            EXIT_ERR("epoll_wait err");
        for(int i = 0;i<ret;i++)
        {
            struct epoll_event * ptr = (struct epoll_event*)&all[i];
            if(!(ptr->events & EPOLLIN))
            {
                continue;
            }
            if(ptr->data.fd == sockfd)
            {
                do_accept(sockfd,epfd);
            }
            else
            {
                do_read(ptr->data.fd,epfd);
            }
        }

    }
}


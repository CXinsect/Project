#include "item.h"
//初始化套结子
int sock_init(int port,int epfd)
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    //地址复用
    int opt = 1;
    int ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if(ret < 0)
        EXIT_ERR("setsockopt err");
    struct sockaddr_in serv;
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd,(struct sockaddr*)&serv,sizeof(serv));
    listen(sockfd,SOMAXCONN);
    //将监听套结子挂到epoll树上
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
    return sockfd;
}
//设置非阻塞
void set_noblock(int confd)
{
    int flags = fcntl(confd,F_GETFL);
    if(flags < 0)
        EXIT_ERR("fcntl err one");
    flags |= O_NONBLOCK;
    int ret = fcntl(confd,F_SETFL,flags);
    if(ret < 0)
        EXIT_ERR("fcntl");
}
//处理连接请求
void do_accept(int lfd,int epfd)
{
    struct sockaddr_in cli;
    memset(&cli,0,sizeof(cli));
    socklen_t len = sizeof(cli);
    int confd;
    do
    {
        confd = accept(lfd,(struct sockaddr*)&cli,&len);
    }while(errno == EINTR && confd == -1);
    if(confd == -1)
        EXIT_ERR("accept err");
    char ip[64];
    cout<<"New client"<<endl;
    cout<<"ip: "<<inet_ntop(AF_INET,&cli.sin_addr.s_addr,ip,sizeof(ip))<<endl;
    //将通信套结子添加到epoll树上并设置为边沿非阻塞模式
    set_noblock(confd);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = confd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,confd,&ev);
}

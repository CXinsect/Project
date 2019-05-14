#include "model.h"
#include "SocketEpoll.h"
#include "FileOptions.h"

int Socket::create_listenfd(int epollfd,int port, char *ip) {
    struct sockaddr_in serv;
    int listenfd;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd < 0) 
        ERR_EXIT("socket");
    int flags = 1;
    socklen_t len = sizeof(flags);
    
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&flags,len) < 0) 
        ERR_EXIT("setsockopt");
    
    memset(&serv,0,sizeof(serv));
    serv.sin_family = PF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;
    if(bind(listenfd,(struct sockaddr *)&serv,sizeof(serv)) < 0) 
       ERR_EXIT("bind");
    
    if(listen(listenfd,SOMAXCONN) < 0)
        ERR_EXIT("listen");
    add_fd(epollfd,listenfd,false);
    return listenfd;
}
int Socket::create_connfd(int epollfd,int listenfd,bool enable) {
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int connfd;
    if((connfd = accept(listenfd,(struct sockaddr *)&cli,&len)) < 0) 
        ERR_EXIT("accept");
    char buf[1024] = {0};
    cout<<inet_ntop(AF_INET,&cli.sin_addr,buf,sizeof(buf))<<endl;
    add_fd(epollfd,connfd,enable);
    return connfd;
}
void Socket::set_noblock(int fd) {
    int flags;
    int ret;
    flags = fcntl(fd,F_GETFL);
    if(flags < 0) 
        ERR_EXIT("fcntl");
    flags |= O_NONBLOCK;
    ret = fcntl(fd,F_SETFL,flags);
    if(ret < 0) 
        ERR_EXIT("fcntl");
}
void Socket::add_fd(int epollfd,int fd,bool enable) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(enable) {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    set_noblock(fd);
}
void Socket::reset_shot(int epollfd,int fd) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
void action(struct epoll_event *events,int nums,int efd,int lfd,Socket s) {
    struct fds * sfd = NULL;
    File_Opt f;
    for(int i = 0; i < nums;i ++) {
        int sockfd = events[i].data.fd;
        if(sockfd == lfd) 
            s.create_connfd(efd,lfd,true);
        else if(events[i].events & EPOLLIN) {
           // sfd = new (struct fds);
            //auto p = make_shared <struct fds> (efd,sockfd);
           // pthread_t tid;
            //struct packet bufp;
            f.Do_File(sockfd);
           /* sfd = (struct fds *)malloc(sizeof(struct fds));
            
            sfd->epollfd = efd;
            sfd->sockfd = sockfd;
            cout<<"cc"<<sockfd<<endl;*/
       // pthread_create(&tid,NULL,Open_Worker,(void*)p.get());
          //  struct packet bufp;
            //do_file(sockfd,bufp);
           // cout<<"main: "<<bufp.pathname<<endl;
            
        }
    }
    /*if(sfd != NULL) {
        free(sfd);
    }*/
}
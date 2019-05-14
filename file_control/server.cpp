#include "Write_Read.cpp"
#include "SocketEpoll.cpp"
#include "FileOptions.cpp"

#define LENGTH 1024


int main (int argc,char * argv[]) {
    int lfd,cfd;
    if(argc < 2) 
        cout<<"please enter like [./a.out] [Port]"<<endl;
    int efd = epoll_create(100);
    Socket s;
    lfd = s.create_listenfd(efd,atoi(argv[1]),argv[2]);
    struct epoll_event events[100];
    while(1) {
        int ret = epoll_wait(efd,events,100,-1);
        if(ret < 0 ) 
            ERR_EXIT("epoll_wait");
        action(events,ret,efd,lfd,s);
    } 
    close(lfd);
    return 0;
}
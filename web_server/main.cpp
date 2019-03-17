#include <iostream>
#include "epoll.cpp"
#include "respond.cpp"
#include "read.cpp"
#include "socket.cpp"
int main(int argc,char * argv[])
{
    if(argc < 3)
    {
        cout<<"[./a.out] [port] [path]"<<endl;
        exit(0);
    }
    int port = atoi(argv[1]);
   int ret =  chdir(argv[2]);
   if(ret == -1)
        EXIT_ERR("chdir");
    do_epoll(port);
    return 0;
}


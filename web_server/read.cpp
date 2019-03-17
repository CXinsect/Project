#include "item.h"
int recv_peek(int fd,void *buf,int size)
{
    int ret;
    while(1)
    {
        ret = recv(fd,buf,size,MSG_PEEK);
        if(ret == -1 && errno == EINTR)
            continue;
        return ret;
    }
}
//每次读取一个字节
int readline(int confd,void *buf,int size)
{
    int i = 0;
    int n;
    char c = '\0';
    char *bufp = (char*)buf;
    while((i < size - 1)  && (c != '\n'))
    {
        n = recv(confd,&c,1,0);
        if(n > 0)
        {
            if(c == '\r')
            {
                n = recv_peek(confd,&c,1);
                if((n > 0) && (c == '\n'))
                    recv(confd,&c,1,0);
                else
                    c = '\n';
            }
            else
            {
                bufp[i] = c;
                i++;
            }
        }
        else
            c = '\n';
    }
    bufp[i] = '\0';
    if(n == -1)
        i = -1;
    return i;
}
//接受数据
void do_read(int confd,int epfd)
{
    char line[1024] = {0};
    int len = readline(confd,line,sizeof(line));
    if(len == 0)
    {
        cout<<"client is over"<<endl;
        disconnect(confd,epfd);
    }
    if(len == -1)
        EXIT_ERR("readline read");
    else
    {
        cout<<"line: "<<line<<endl;
        cout<<"------------begin"<<endl;
        while(len)
        {
            char buf[1024] = {0};
            len = readline(confd,buf,sizeof(buf));
            cout<<"buf: "<<buf<<endl;
        }
        cout<<"------------end"<<endl;
    }
        if(strncasecmp("get",line,3) == 0)
        {
            http_request(line,confd);
            disconnect(confd,epfd);
        }
    cout<<"len"<<len<<endl;
}

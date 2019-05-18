#include "model.h"
#include "Write_Read.h"
#include  "FileOptions.h"
int readn (int fd,void * buf,size_t size) {
    char * bufp = (char *)buf;
    int nleft = size;
  //cout<<"size readn"<<size<<endl;
    int nread;
    while(nleft > 0) {
        //cout<<"nleft: "<<nleft<<endl;
        if((nread = read(fd,bufp,nleft)) < 0) {
            //cout<<strerror(errno)<<endl;            cout<<"limian : nread"<<nread<<endl;
            if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                continue;      
            return -1;
        }
        //cout<<"nread="<<nread<<endl;
        /*int len = strlen(bufpAmac);
        bufp->Amac[len] = 0;
        cout<<"amac "<<bufp->Amac;
        cout<<"pathname"<<bufp->pathname<<endl;
        cout<<"content "<<bufp->buf[0]<<endl;
        cout<<"content "<<bufp->buf[1]<<endl;
        cout<<"content "<<bufp->buf[2]<<endl;
*/
        if(nread == 0) {
            cout<<"client is over"<<endl;
            break;
        }
        nleft -= nread;
        bufp += nread;
    }
    return size - nleft;
}
int writen (int fd,void *buf,size_t size) {
    int nwrite;
    char * bufp =(char *)buf;
    int nleft = size;
    while (nleft > 0) {
        if((nwrite = write(fd,bufp,nleft)) < 0) {
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                continue;
            }
            return -1;
        }
        if(nwrite == 0)
            continue;
        nleft -= nwrite;
        bufp += nleft;
    }
    return size - nleft;
}
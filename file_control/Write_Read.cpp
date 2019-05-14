#include "model.h"
#include "Write_Read.h"
int readn (int fd,char * buf,int size) {
    char * bufp = (char *)buf;
    int nleft = size;
    int nread;
    while(nleft > 0) {
        if((nread = read(fd,bufp,nleft)) < 0) {
            if(nread == -1 && errno == EINTR)
                continue;      
            return -1;
        }
        if(nread == 0)
            break;
        nleft -= nread;
        bufp += nread;
    }
    return size - nleft;
}
int writen (int fd,char *buf,int size) {
    int nwrite;
    char * bufp = (char *)buf;
    int nleft = size;
    while (nleft > 0) {
        if((nwrite = write(fd,bufp,nleft)) < 0) {
            if (nwrite == -1 && EINTR == errno) {
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
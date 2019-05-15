#ifndef __FILE_OPTIONS_H_
#define __FILE_OPTIONS_H_
#include "model.h"
//包裹函数
struct packet {
        int Rtype; 
        char Amac[1024];
        char pathname[1024];
        char buf[BUFF_SIZE];
};

class File_Opt {
    public:
        File_Opt() {}
        File_Opt(int _sockfd,struct packet _bufp) {
            sockfd = _sockfd;
            buf = _bufp;
        }
        void Do_File (int sockfd);
        void Open_Do (int sockfd,struct packet *bufp);
        void Close_Do (int sockfd,struct packet *bufp);
        void Alive_Do (int sockfd, struct packet * bufp);
        ~File_Opt() {}
    private:
        int sockfd;
        struct packet buf;
};

#endif
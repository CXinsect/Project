#include <string>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#define MYFILE "myfile"
#include <limits.h>
#include "FileOptions.h"
#include "Write_Read.cpp"
#include "model.h"
#include "shm.cpp"
using namespace std;

#define OPEN 1

bool GetIP(const vector<string>& NetType,string& strip)
{
    for(size_t i=0;i<NetType.size();i++)
    {
        for(char c='0';c<='9';++c)
        {
            string strDevice = NetType[i] + c; //根据网卡类型，遍历设备如eth0，eth1
            int fd;
            struct ifreq ifr;
            //使用UDP协议建立无连接的服务
            fd = socket(AF_INET,SOCK_DGRAM, 0);
            strcpy(ifr.ifr_name, strDevice.c_str() );       
            //获取IP地址
            if (ioctl(fd, SIOCGIFADDR, &ifr) <  0) {
                ::close(fd);
                continue;
            }
            // 将一个IP转换成一个互联网标准点分格式的字符串
            strip = inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);
            if(!strip.empty())
            {
                ::close(fd);
                return true;
            }
        }
    }
    return false;
}
int main (int argc,char * argv[]) {
    int sockfd;
    char pbuf[1024] = {0};
    struct sockaddr_in serv;
    if (argc < 3) {
        cout<<"please enter like [./a.out] [port] [IP] [Path]"<<endl;
        exit(0);
    }
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        ERR_EXIT("socket");
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(argv[1]));
    inet_pton(AF_INET,argv[2],&serv.sin_addr);
    if(connect(sockfd,(struct sockaddr*)&serv,sizeof(serv)) < 0) {
        ERR_EXIT("connect");
    }
    cout<<"connected"<<inet_ntop(AF_INET,&serv.sin_addr,pbuf,sizeof(pbuf))<<endl;
    
    memset(pbuf,0,sizeof(pbuf));
    int fd,shmid,ret;
    struct shmseg *shmp;
    struct packet bufp;
    cout<<"dddddddddddd"<<endl;
    if((fd = open(MYFILE,O_RDWR,0666)) < 0)
        ERR_EXIT("open");
    if(IPC_OpenShm(0,&shmid)<0)
        ERR_EXIT("IPC_OpenShm");
    if((ret = IPC_MapShm(shmid,(void**)&shmp)) < 0)
        ERR_EXIT("IPC_MapShm");
    memcpy(pbuf,shmp->buf,strlen(shmp->buf)+1);
    cout<<"res: " <<pbuf<<endl;
    IPC_DelShm(shmid);
    int n = 0;
    int count = 0;
    strcpy(bufp.pathname,pbuf);
    bufp.Rtype = ALIVE;
    strcpy(bufp.Amac,"af:de:we");
    //获取mac地址
    while((n = read(fd,bufp.buf,BUFF_SIZE)) > 0) {
        cout<<"n: "<<n<<endl;
        bufp.buf[n] = 0;
        if(writen(sockfd,(char*)&bufp,sizeof(bufp)) < 0) {
            cout<<"writen err"<<endl;
            break;
        }
        /*else {
            cout<<"type: "<<bufp.Rtype<<endl;
            cout<<"Mac: "<<bufp.Amac<<endl;
            cout<<"buffer: "<<bufp.buf<<endl;
        }*/
    }
    cout<<n<<endl;
    //struct packet buffer;
    char c;
    while(read(sockfd,&c,1) > 0) {
        cout<<"字符"<<c<<endl;
        //int num = ntohs(buffer.Rtype);
       // cout<<"type: "<<num<<endl;
        //cout<<"Mac: "<<buffer.pathname<<endl;
       // cout<<"buffer: "<<buffer.buf<<endl;
       // memset(&bufp,0,sizeof(buffer));
    }
   /* n = writen(sockfd,(void*)&bufp.pathname,strlen(pbuf)+1);
    cout<<"write: "<<bufp.pathname<<endl;*/
    return 0;
}

#include "model.h"
#include "FileOptions.h"
#include "Write_Read.h"

void File_Opt::Do_File (int sockfd) {
    int n = 0;
    struct packet bufp;
    memset(&bufp,0,sizeof(bufp));
    cout<<"ddddddd"<<endl;
    while(read(sockfd,&bufp,sizeof(bufp)) > 0) {
        cout<<"type: "<<bufp.Rtype<<endl;
        switch(bufp.Rtype) {
            case OPEN:
                Open_Do(sockfd,&bufp);
                break;
            case CLOSE:
                Close_Do(sockfd,&bufp);
                break;
            case ALIVE:
            // Alive_Do(sockfd,bufp);
            break;
            default:
                cout<<"Invaild Package"<<endl;
                break;
        }
    }
}
void File_Opt::Open_Do (int sockfd,struct packet *bufp) {
    map <string,long> file;
    int fd;
    char buf[2048] = {0};
    static long num = 1;
    int n = 0;
    cout<<"ddd"<<endl;
   // readn(sockfd,(char*)(bufp + 4),sizeof(bufp)-4);
    sprintf(buf,"%s+%s",bufp->Amac,bufp->pathname);
    cout<<"buf:"<<buf<<endl;
    if(file.find(buf) == file.end()) {
        auto ret = file.insert(make_pair(buf,num));
        char name[1024] = {0};
        sprintf(name,"%x",(unsigned int)num);
        cout<<"转换成的名字："<<name<<endl;
        char cwd[1024] = {0};
        getcwd(cwd,sizeof(cwd));
        cout<<"ing: "<<cwd<<endl;
        if((fd = open(name,O_CREAT | O_RDWR,0666)) < 0) 
            ERR_EXIT("open err");
        writen(fd,bufp->buf,strlen(bufp->buf));
        cout<<"file content: "<<bufp->buf<<endl;
        while(read(sockfd,(char *)&bufp,sizeof(bufp)) > 0) {
            write(fd,bufp->buf,strlen(bufp->buf));
        }
        close(fd);
        num++;
        char secret[] = "It's a sercet!";
        if(writen(sockfd,secret,strlen(secret)) < 0) 
            ERR_EXIT("write server");
        int fds = open("mapdd.txt",O_CREAT | O_RDWR,0666);
        if(fds < 0) 
            ERR_EXIT("open server err");
        char maptxt[1024];
        sprintf(maptxt,"%s %ld",buf,num);
        writen(fds,maptxt,strlen(maptxt));
        close(fds);
    }
    else {
        char secret[] = "It's a sercet!";
        if(writen(sockfd,secret,strlen(secret)) < 0) 
            ERR_EXIT("write server");
    }
    close(sockfd);
}
void File_Opt::Close_Do(int sockfd,struct packet *bufp) {
    char buf[2048] = {0};
    sprintf(buf,"%s+%s",bufp->Amac,bufp->pathname);
    if(())
}

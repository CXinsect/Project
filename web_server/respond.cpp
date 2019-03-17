#include "item.h"
//发送文件头
void  http_request_head(int confd,int no,const char * dsp,const char * type,long len)
{
    cout<<"http_head"<<endl;
    char buf[1024] = {0};
    //状态行
    sprintf(buf,"http/1.1 %d %s\r\n",no,dsp);
    send(confd,buf,strlen(buf),0);
    //消息报头
    sprintf(buf,"Content-Type:%s\r\n",type);
    sprintf(buf+strlen(buf),"Content-Length:%ld\r\n",len);
    send(confd,buf,strlen(buf),0);
    //空行
    send(confd,"\r\n",2,0);
}
//转码
void encode(char * to,int tosize,const char * from)
{
     int tolen;

    for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from) 
    {
        if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0) 
        {
            *to = *from;
            ++to;
            ++tolen;
        } 
        else 
        {
            sprintf(to, "%%%02x", (int) *from & 0xff);
            to += 3;
            tolen += 3;
        }

    }
    *to = '\0';
}
// 16进制数转化为10进制
int Binary_Tran(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 0;
}
//解码
void decode(char *to,char *from)
{
    for ( ; *from != '\0'; ++to, ++from  )
    {
        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
        {

            *to = Binary_Tran(from[1])*16 + Binary_Tran(from[2]);

            from += 2;
        }
        else
        {
            *to = *from;

        }

    }
    *to = '\0';
}
void send_dir(int confd,const char * dirname)
{
    do{
    char buf[4096] = {0};
    sprintf(buf,"<html><head><title>目录:%s</title></head>",dirname);
    sprintf(buf+strlen(buf),"<body><h1>当前目录:%s</h1><table>",dirname);
    struct dirent ** ptr;
    int num = scandir(dirname,&ptr,NULL,alphasort);
    if(num < 0)
        break;

    char path[1024] = {0};
    char enstr[1024] = {0};
    for(int i = 0;i<num;i++)
    {
        char *name = ptr[i]->d_name;
        sprintf(path,"%s/%s",dirname,name);
        struct stat st;
        stat(path,&st);
        //将中文转码防止因为浏览器乱码请求导致server挂掉
        encode(enstr,sizeof(enstr),name);
        if(S_ISDIR(st.st_mode))
        {
            sprintf(buf+strlen(buf),"<tr><td><a href = \"%s/\">%s</a></td><td>%ld</td>",
                   enstr,name,(long)st.st_size);
        }
        if(S_ISREG(st.st_mode))
        {
            sprintf(buf+strlen(buf),"<tr><td><a href = \"%s\">%s</a></td><td>%ld</td>",
                   enstr,name,st.st_size);
        }
        send(confd,buf,strlen(buf),0);
        memset(buf,0,sizeof(buf));
    }
    sprintf(buf+strlen(buf),"</table></body></html>");
    send(confd,buf,strlen(buf),0);
    cout<<"send dir ok"<<endl;
    } while(0);
}
void send_file(int confd,const char * fname)
{
    do
    {
    int fd = open(fname,O_RDONLY);
    if(fd < 0)
    {
        perror("open err");
        send_file(confd,"/home/Insect/code/web/item/temp/404.html");
        break;
    }
    int len = 0;
    char buf[1024] = {0};
    while((len = read(fd,buf,sizeof(buf))) > 0)
    {
        send(confd,buf,len,0);
    }
    close(fd);
    }while(0);
    cout<<"send file ok"<<endl;
}
// 通过文件名获取文件的类型
const char *get_file_type(const char *name)
{
    const char* dot;

    // 自右向左查找‘.’字符, 如不存在返回NULL
    dot = strrchr(name, '.');   
    if (dot == NULL)
        return "text/plain; charset=utf-8";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp( dot, ".wav" ) == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}

//回应请求
void http_request(const char *str,int confd)
{
    char method[12],path[1024],pro[12];
    //分离请求行
    sscanf(str,"%[^ ] %[^ ] %[^ ]",method,path,pro);
    cout<<"method: "<<method<<" path: "<<path<<" pro: "<<pro<<endl;
    //解码
    decode(path,path);
    cout<<"path: "<<path<<endl;
    const char * file = path + 1;
    if(strcmp(path,"/") == 0)
        file = "./";
    struct stat st;
    int ret = stat(file,&st);
    if(ret == -1)
    {
        http_request_head(confd,404,"Not Found",".html",-1);
        send_file(confd,"404.html");
    }
    if(S_ISDIR(st.st_mode))
    {
        //回应头请求
        http_request_head(confd,200,"ok",get_file_type(".html"),-1);
        //发送目录
        send_dir(confd,file);
    }
    if(S_ISREG(st.st_mode))
    {
        http_request_head(confd,200,"ok",get_file_type(file),st.st_size);
        send_file(confd,file);
    }
}
void disconnect(int confd,int epfd)
{
    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,confd,0);
    if(ret < 0)
        EXIT_ERR("epoll_ctl err");
    close(confd);
}

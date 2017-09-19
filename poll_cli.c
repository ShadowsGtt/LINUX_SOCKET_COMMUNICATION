/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 17时18分45秒
 ************************************************************************/
#include<netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#define PORT 40009
#define MAXDATASIZE 1024 
#define SerIP "localhost"

int main()
{
    char newmyput[100] = "\0";
    char name[10] = "\0";
    char myput[MAXDATASIZE] = {'\0'};
    int sockfd,numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr;


    printf("请输出您的用户名:");
    fgets(name,sizeof(name),stdin);
    name[strlen(name)-1] = '\0';

    struct pollfd WRS[3];

    if((he = gethostbyname(SerIP))==NULL)
    {
        herror("gethostbyname");
        exit(1);
    }
    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero),8);
    if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("服务器未启动");
        exit(1);
    }
    send(sockfd,name,sizeof(name),0);
    WRS[0].fd = 0;  //输入;
    WRS[1].fd = sockfd; //输出
    WRS[0].events = POLLWRNORM;  //写事件
    WRS[1].events = POLLRDNORM; // 读事件
    printf("发送给指定人消息格式为:@[对方名字][:][消息内容]\n");
    while(1)
    {
        int nready = poll(WRS,2,0);
        
        if(WRS[1].revents & POLLRDNORM) //接受外来信息
        {
            numbytes=recv(sockfd,buf,sizeof(buf),0);
            if(numbytes == -1)
            {
                perror("recv");
                exit(1);
            }
            else if(numbytes == 0)
            {
                printf("Server has closed\n");
                exit(1);
            }
            else
                printf("%s",buf);
        }
        if(WRS[0].revents & POLLWRNORM) //获取终端输入
        {
            if(fgets(myput,sizeof(myput),stdin) == NULL)
            {
                break;
            }
            if(send(sockfd,myput,sizeof(myput),0) == -1)
            {
                perror("send");
            }
        }
            memset(myput,0,sizeof(myput));
            memset(newmyput,0,sizeof(newmyput));
            memset(buf,0,sizeof(buf));
    }
    return 0;
}

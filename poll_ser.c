/*************************************************************************
	> File Name: server
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 16时56分02秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<sys/select.h>
#include<poll.h>

#define BACKLOG 10
#define MYPORT 40009
#define MaxClients 500
#define localIP 127.0.0.1
#define netIP 192.168.1.107


struct Client
{
    char IP[20];
    char PORT[20];
    char name[20];
    int fd;
};
struct sockaddr_in my_addr;
struct sockaddr_in their_addr;
struct Client clients[50];
int Init()
{
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket");
        exit(1);
    }
    int on = 1;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)//地址重复利用

        bzero(&(my_addr.sin_zero),8);
    if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sockfd,BACKLOG)==-1)
    {
        perror("listen");
        exit(1);
    }  
    return sockfd;
}

int main()
{
    int i = 0;
    int total = 0;
    char buf[1024] = {'\0'};
    char myput[1024] = {'\0'};
    int numbytes;
    int new_fd;
    int conn = -1;
    int sockfd = Init();
    char name[20] = "\0";
    struct pollfd Poll_Cli[MaxClients];
    Poll_Cli[0].fd = sockfd;
    Poll_Cli[0].events = POLLRDNORM;
    for(i = 1; i < MaxClients ; i++)
    {
        Poll_Cli[i].fd = -1;

    }
    int maxi = 0;
    while(1)
    {
        int nready = poll(Poll_Cli,maxi+1,0);
        if (Poll_Cli[0].revents & POLLRDNORM)  /* 新的连接到来 */
        {
            int sin_size = sizeof(struct sockaddr_in);
            if(  (conn = (accept(sockfd,(struct sockaddr *)&their_addr,&sin_size)  )) == -1)
            {
                perror("accept");
                continue;
            }
            recv(conn,name,sizeof(name),0);
            send(conn,"登录成功!\n",50,0);
            printf("新客户%s已经链接！IP:%s,Port:%d\n",name,inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port));
            printf("当前在线人数:%d\n",++total);
            maxi++;
            for(int k = 1; k <= MaxClients;k++)
            {
                if(Poll_Cli[k].fd == -1)
                {
                    Poll_Cli[k].fd = conn;
                    strcpy(clients[k].name,name);
                    clients[k].fd = conn;
                    Poll_Cli[k].events = POLLRDNORM;
                    break;
                }
            }
        }
        for(int k = 1;k <= maxi ; k++)
        {
            conn = Poll_Cli[k].fd;
            if(conn == -1)
                return(0) ;
            if (Poll_Cli[k].revents & POLLRDNORM)
            {
                numbytes = recv(conn,buf,sizeof(buf),0);
                buf[numbytes] = '\0';
                strcpy(name,clients[k].name); 
                char newbuf[200] = "\0";
                char dname[20] = "\0";
                char msg[300] = "\0";
                int n = 0;
                int m = 0;
                if(buf[0] == '@')
                {
                    int i = 0;
                    for(i = 1 ;buf[i] != ':';i++)
                    {
                        dname[m++] = buf[i]; 
                    }
                    n = 0;
                    for(int p = i+1;buf[p] != '\0';p++)
                    {
                        newbuf[n++] = buf[p];   //newbuf 存客户端发送的消息
                    }
                }
                else
                {
                    n = 0;
                    for(int i = 0;buf[i] != '\0';i++)
                    {
                        newbuf[n++] = buf[i];   //newbuf 存客户端发送的消息
                    }
                }
                if(numbytes == -1)
                {
                    perror("recieve");
                    exit(0);
                }
                else if(numbytes == 0)
                {
                    printf("Port = %d已经关闭连接!\n",ntohs(their_addr.sin_port));
                    printf("当前在线人数:%d\n",--total);
                    clients[k].fd = -1;
                    Poll_Cli[k].fd = -1;
                    memset(clients[k].name,0,sizeof(clients[k].name));
                }
                else
                {
                   printf("name=%s\n",name);
                    printf("dname=%s\n",dname);
                    printf("msg=%s\n",newbuf);
                    strcat(msg,name);
                    strcat(msg,":");
                    strcat(msg,newbuf);
                    msg[strlen(msg)] = '\0';
                   for(int k = 0 ; k < 50 ;k++)
                    {
                        if(strcmp(clients[k].name,dname) == 0)
                        {
                            send(clients[k].fd,msg,sizeof(msg),0);
                        }
                    }
                }
            memset(buf,0,sizeof(buf));
            memset(name,0,sizeof(name));
            memset(dname,0,sizeof(dname));
            memset(newbuf,0,sizeof(newbuf));
            memset(msg,0,sizeof(msg));
            }
        }
    }
    close(sockfd);
    return 0;
}

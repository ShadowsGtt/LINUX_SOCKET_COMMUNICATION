#ifndef _GT_H
#define _GT_H
#include<netdb.h>
#include <poll.h>
#include<sys/sendfile.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/select.h>
#define BACKLOG 100
#define ServPort 40009
#define MaxClients 500
#define LocalIp "127.0.0.1"
#define ServIp "127.0.0.1"
#define UDPSERV 1
#define UDPCLIENT 2
#define TCPSERV 3
#define TCPCLIENT 4

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;

int Socket(int type)
{
    const int on = 1;
    int sockfd;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ServPort);
    //inet_pton(AF_INET,ServIp,&serv_addr.sin_addr);
    serv_addr.sin_addr.s_addr = inet_addr(ServIp);
    switch(type)
    {
        case UDPSERV:
            if((sockfd = socket(AF_INET,SOCK_DGRAM,0))==-1)
            {
                perror("socket");
                exit(1);
            }
            if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)
            {
                perror("setsockopt");
            }
            if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1)
            {
                perror("bind");
                exit(1);
            }
            break;    
        case UDPCLIENT:
            if((sockfd = socket(AF_INET,SOCK_DGRAM,0))==-1)
            {
                perror("socket");
                exit(1);
            }
            int res;
            if( res = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1 )
            {
                perror("connect");
                exit(-1);
            }

            break;
        case TCPSERV:
            if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(1);
            }
            if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)//地址重复利用
            {
                perror("setsockopt");
            }
            if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1)
            {
                perror("bind");
                exit(1);
            }
            if(listen(sockfd,BACKLOG)==-1)
            {
                perror("listen");
                exit(1);
            }
            break;

        case TCPCLIENT:
            if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(1);
            }

            break;
        default:
            return -1;
    }
        return sockfd;
}
#endif

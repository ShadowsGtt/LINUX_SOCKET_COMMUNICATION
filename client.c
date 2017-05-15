/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 17时18分45秒
 ************************************************************************/
#include<netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#define PORT 4000
#define MAXDATASIZE 100 

int main(int argc,char *argv[])
{
    int sockfd,numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr;

    if((he = gethostbyname(argv[1]))==NULL)
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
        perror("connet");
        exit(1);
    }
    while(1){
        numbytes=recv(sockfd,buf,MAXDATASIZE,0);
    if(numbytes == -1)
    {
        perror("recv");
        exit(1);
    }
        buf[numbytes] = '\0';
        printf("Received:%s\n",buf);
    }
    if(argc!=2)
    {
        fprintf(stderr,"usage:client hostname\n");
        exit(1);
    }
    close(sockfd);
    return 0;
}

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
#define PORT 40009
#define MAXDATASIZE 100 

int main(int argc,char *argv[])
{
    char myput[50] = "\0";
    int sockfd,numbytes;
    char buf[50];
    struct hostent *he;
    struct sockaddr_in their_addr;
    if((he = gethostbyname("192.168.1.107"))==NULL)
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
    fd_set set;
    FD_ZERO(&set);
    while(1)
    {
        FD_SET(0,&set);
        FD_SET(sockfd,&set);
        int sel = select(sockfd+1,&set,NULL,NULL,NULL);
        if(sel == -1)
        {
            perror("select");
            break;
        }
        if(sel == 0)
        {
            continue;
        }
        if(FD_ISSET(sockfd,&set))
        {
            numbytes=recv(sockfd,buf,MAXDATASIZE,0);
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
            printf("Server:%s",buf);
        }
        if(FD_ISSET(0,&set))
        {
            if(fgets(myput,sizeof(myput),stdin) == NULL)
            {
                break;
            }
            if(send(sockfd,myput,sizeof(myput)-1,0) == -1)
            {
                perror("send");
            }
        }
    }
    return 0;
}

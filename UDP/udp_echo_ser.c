#include "gt.h"
//#include <signal.h>
int main()
{
    int sockfd = Socket(UDPSERV);
    if(sockfd == -1)
    {
        printf("Socket() failed\n");
        exit(-1);
    }
    char recvbuf[300] = "\0";
    int client_len = sizeof(struct sockaddr_in);
    //int client_len;
    while(1)
    {
        bzero(recvbuf,sizeof(recvbuf));
        int  n = recvfrom(sockfd,recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&client_addr,&client_len);
        if(n == -1)
        {
            perror("recvfrom");
            exit(-1);
        }
        recvbuf[n] = 0;
        printf("client_len:%d\n",client_len);
        printf("ip:%s\tport:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        printf("recv %d byte:%s\n",n,recvbuf);
        int sd = sendto(sockfd,recvbuf,n,0,(struct sockaddr *)&client_addr,client_len);
        if(sd == -1)
        {
            perror("send");
        }
    }
    return 0;
}


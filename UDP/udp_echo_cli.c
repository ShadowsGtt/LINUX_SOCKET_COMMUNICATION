#include "gt.h"
int main()
{
    char sendbuf[300] = "\0";
    char recvbuf[300] = "\0";
    int sockfd = Socket(UDPCLIENT);
    if(sockfd == -1)
    {
        printf("Socket() failed\n");
        exit(-1);
    }
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1 )
    {
        perror("connect");
        exit(-1);
    }
    while(1)
    {
        bzero(recvbuf,sizeof(recvbuf));
        bzero(sendbuf,sizeof(sendbuf));
        if(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
        {
            send(sockfd,sendbuf,sizeof(sendbuf),0);
        }
        int n = recv(sockfd,recvbuf,sizeof(recvbuf),0);
        if(n == -1)
        {
            perror("recv");
            exit(-1);
        }
        recvbuf[n] = 0;
        printf("recv %d byte:%s\n",n,recvbuf);
    }
    
    return 0;
}


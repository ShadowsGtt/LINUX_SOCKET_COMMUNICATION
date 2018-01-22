#include "gt.h"
int main()
{
    int sockfd = Socket(UDPCLIENT);
    if(sockfd == -1)
    {
        printf("Socket() failed\n");
        exit(-1);
    }
    char sendbuf[300] = "\0";
    char recvbuf[300] = "\0";
    while(1)
    {
        bzero(recvbuf,sizeof(recvbuf));
        bzero(sendbuf,sizeof(sendbuf));
        if(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
        {
            int sd = sendto(sockfd,sendbuf,sizeof(sendbuf),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
            if(sd == -1)
            {
                perror("sendto");
                exit(-1);
            }
        }
        int n = recvfrom(sockfd,recvbuf,sizeof(recvbuf),0,NULL,NULL );
        recvbuf[n] = 0;
        printf("recv %d byte:%s\n",n,recvbuf);
    }
    
    return 0;
}


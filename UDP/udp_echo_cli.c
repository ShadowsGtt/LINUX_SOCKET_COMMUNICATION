#include "gt.h"
int main()
{
    int sockfd = Socket(UDPCLIENT);
    char sendbuf[300] = "\0";
    char recvbuf[300] = "\0";
    
    while(1)
    {
        if(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
        {
            //sendto(sockfd,sendbuf,sizeof(sendbuf),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
            if(send(sockfd,sendbuf,sizeof(sendbuf),0) == -1 )
            {
                perror("send");
                exit(-1);
            }
            int n = recv(sockfd,recvbuf,sizeof(recvbuf),0);
            if(n == -1)
            {
                perror("recv");
                exit(-1);
            }
            recvbuf[n] = 0;
            printf("recv:%s\n",recvbuf);
        }
    }
    
    return 0;
}


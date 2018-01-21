#include "gt.h"
int main()
{
    int sockfd = Socket(UDPSERV);
    char recvbuf[300] = "\0";
    int client_len;
    while(1)
    {
        int  n = recvfrom(sockfd,recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&client_addr,&client_len);
        sendto(sockfd,recvbuf,n,0,(struct sockaddr *)&client_addr,sizeof(client_addr) );
    }
    return 0;
}

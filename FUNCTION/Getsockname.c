#include "gt.h"
void Getsockname(int sockfd,struct sockaddr *sc,socklen_t *len)  //获取内核赋予的ip和端口
{
    int socknum = getsockname(sockfd,sc,len);
    if(socknum == -1)
        fprintf(stderr,"error:getsockname error\n");
    struct sockaddr_in * sci = (struct sockaddr_in *)sc;
    printf("ip:%s   port:%d\n",inet_ntoa(sci->sin_addr),ntohs(sci->sin_port)); 
    
}

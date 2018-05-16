#include "gt.h"

int main()
{
    struct sockaddr_in serv;
    int fd = socket(AF_INET,SOCK_STREAM,0);
    
    serv.sin_family = AF_INET;
    serv.sin_port = htons(40009);
    inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr); 
    int n = noblock_connect(fd,(struct sockaddr *)&serv,sizeof(serv),2);
    if(n == -2)
        printf("timeout\n");
    if(n == 0)
        printf("success\n");
    if(n == -1)
        printf("failed\n");

}

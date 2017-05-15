/*************************************************************************
	> File Name: server
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 16时56分02秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include <arpa/inet.h>
 #include <unistd.h>

/*服务器要监听的本地端口*/
#define BACKLOG 10
#define MYPORT 4000
int main()
{
    int numbytes;
    char buf[100];
    char myput[20] = {'\0'};
    int sockfd,new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket");
        exit(1);
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);
    if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sockfd,BACKLOG)==-1)
    {
        perror("listen");
        exit(1);
    }
    while(1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if((new_fd = accept(sockfd,(struct sockaddr *)&their_addr,&sin_size))==-1)
        {
            perror("accept");
            continue;
        }
        printf("server:got connetion from %s\n",inet_ntoa(their_addr.sin_addr));
        while(1){
            if(!fork()){
                scanf("%s",myput);
                if(send(new_fd,myput,14,0)==-1)
                {
                    perror("send");
                }
                numbytes = recv(new_fd,buf,100,0);
                buf[numbytes] = '\0';
                printf("Client:%s\n",buf);
        }

    }
    }
        while(waitpid(-1,NULL,WNOHANG)>0);
    return 0;
}

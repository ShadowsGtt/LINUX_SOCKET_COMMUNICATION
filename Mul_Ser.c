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
#define MYPORT 4009
int main()
{
    int numbytes;
    char buf[50];
    char myput[50] = "\0";
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
        printf("新客户已经链接！IP:%s,主机名:\n",inet_ntoa(their_addr.sin_addr));
            int pid = fork();
            if(pid == 0)
            {
                while(1)
                {
                    numbytes = recv(new_fd,buf,100,0);
                    buf[numbytes] = '\0';
                    printf("Client:%s\n",buf);
                    if(send(new_fd,buf,50,0)==-1)
                    {
                        perror("send");
                    }

                }
            }
            if(pid == -1)
            {
                perror("fork");
            }
    }
        while(waitpid(-1,NULL,WNOHANG)>0);
    return 0;
}

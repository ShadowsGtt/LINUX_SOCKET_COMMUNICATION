/*************************************************************************
	> File Name: 1.c
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
    char myput[20] = {'\0'};
    /*在sock_fd上进行监听,new_fd接受的新连接*/
    int sockfd,new_fd;
    /*自己的地址信息*/
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        /*输出错误提示并退出*/
        perror("socket");
        exit(1);
    }
    /*主机字节顺序*/
    my_addr.sin_family = AF_INET;
    /*网络字节顺序,短整形*/

    my_addr.sin_port = htons(MYPORT);
    /*将程序运行的机器的IP填充如s_addr*/
    my_addr.sin_addr.s_addr = INADDR_ANY;
    /*将此结构体的其余空间清零*/
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
        if(!fork())
        {
            while(1)
            {
                scanf("%s",myput);
                if(send(new_fd,myput,14,0)==-1)
                {
                    perror("send");
                 //   close(new_fd);
                 //exit(0);
                }
            }
            close(new_fd);
            exit(0);

        }
    }
        while(waitpid(-1,NULL,WNOHANG)>0);
    return 0;
}

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
 #include<sys/select.h>

#define BACKLOG 10
#define MYPORT 4009
int main()
{
    int i = 0;
    int total = 0;
    int numbytes;
    int cli[1024] = {-1};
    char buf[1024] = {'\0'};
    char myput[1024] = {'\0'};
    int sockfd,new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    fd_set set1;
    fd_set set2;
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
    sin_size = sizeof(struct sockaddr_in);
    int maxfd = sockfd;
    while(1)
    {
        FD_ZERO(&set1);
        FD_ZERO(&set2);
        FD_SET(sockfd,&set1);
        if(FD_ISSET(sockfd,&set1))
        {
            if(cli[i]= accept(sockfd,(struct sockaddr *)&their_addr,&sin_size) == -1)
            {
                perror("accept");
                continue;
            }
        printf("新客户已经链接！IP:%s,Port:%d\n",inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port));
            printf("当前在线人数:%d\n",++total);
            for(int j = 0;j <= i ;j++)
            {
                FD_SET(cli[j],&set2);
            }
            if(cli[i] > maxfd)
                maxfd = cli[i];
            int sel = select(maxfd+1,&set2,NULL,NULL,NULL);
            if(sel == -1)
            {
                printf("即将退出，select出错！%s\n",strerror(errno));
                break;
            }
            else if(sel == 0)
            {
                continue;
            }
            else
            {
                    numbytes = recv(cli[i],buf,sizeof(buf),0);
                    if(numbytes == 0 )
                    {
                        printf("Port = %d已经关闭连接!\n",ntohs(their_addr.sin_port));
                        printf("当前在线人数:%d\n",--total);
                        continue;
                    }
                    else
                        printf("Client:%s",buf);

            }
        }
        i++;
    }
   /* while(1)
    {
        //set2 = set1;
        sin_size = sizeof(struct sockaddr_in);
        if((new_fd= accept(sockfd,(struct sockaddr *)&their_addr,&sin_size))==-1)
        {
            perror("accept");
            continue;
        }
            FD_ZERO(&set2); //每次循环都要清空集合，否则不能检测描述符变化  
            FD_ZERO(&set1); //每次循环都要清空集合，否则不能检测描述符变化  
        fds[i] = new_fd;
            for(int j = 1 ; j <=i ; j ++)
            {
                FD_SET(fds[j],&set2); //添加描述符  
            }
        printf("新客户已经链接！IP:%s,Port:%d\n",inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port));
        printf("当前在线人数:%d\n",++total);
            int sel = select(maxfd+1,&set2,NULL,NULL,NULL);
            if(sel == -1)
            {
                printf("即将退出，select出错！%s\n",strerror(errno));
                break;
            }
            else if(sel == 0)
            {
                continue;
            }
            else
        {
            
                while(FD_ISSET(fds[i],&set2))
                {
                    numbytes = recv(fds[i],buf,sizeof(buf),0);
                    if(numbytes == 0 )
                    {
                        printf("Port = %d已经关闭连接!\n",ntohs(their_addr.sin_port));
                        printf("当前在线人数:%d\n",--total);
                        break;
                    }
                    printf("Client:%s",buf);
                    if(send(fds[i],buf,numbytes-1,0)==-1)
                    {
                        perror("send");
                    }  
                }
        }
        i++;
    }*/
        while(waitpid(-1,NULL,WNOHANG)>0);
    return 0;
}

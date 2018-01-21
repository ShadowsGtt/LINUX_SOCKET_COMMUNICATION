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
#include <arpa/inet.h>
 #include <unistd.h>
 #include<sys/select.h>

#define BACKLOG 10
#define MYPORT 40009
#define IP 127.0.0.1
#define netIP 192.168.1.107
struct Client
{
    char cliIP[20];
    char cliPort[20];
    char name[20];
    int fd;
};

int main()
{
    int i = 0;
    int total = 0;
    int conn;
    int numbytes;
    int new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    struct Client clients[50];
    for(int i = 0;i < 50;i++)
    {
        clients[i].fd = -1;
    }
    int cli[1024] = {0};
    for(int k = 0; k < 1024 ; k++)
        cli[k] = -1;
    char buf[1024] = {'\0'};
    char myput[1024] = {'\0'};
    int sin_size,sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket");
        exit(1);
    }
    int on = 1;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);

    //my_addr.sin_addr.s_addr = INADDR_ANY;
    //my_addr.sin_addr.s_addr = INADDR_ANY; /*INADDR_ANY　指定地址为 0.0.0.0 这个地址事实上表示不确定地址或“所有地址”、“任意地址”。 一般来说，在各个系统中均定义成为0值*


    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //inet_addr()是将一个点分制的IP地址(如192.168.0.1)转换为上述结构中需要的32位IP地址(0xC0A80001)
    
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)//地址重复利用
    
    bzero(&(my_addr.sin_zero),8);
    // 将my_addr结构体中的sin_zero 前八个字节用0替换（char   sin_zero[8] ）　同memset(my_addr.sin_zero,0,8)
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
    fd_set set,sett;
    int maxfd = sockfd;
    FD_ZERO(&set );
    FD_ZERO(&sett);
    while(1)
    {
        set = sett;
        FD_SET(sockfd,&set);
        int sel = select(maxfd+1,&set,NULL,NULL,NULL);
        if(sel == 0)
        {
            continue;
        }
        if(sel == -1)
        {
            perror("select");
        }
        if(FD_ISSET(sockfd,&set))
        {
            sin_size = sizeof(struct sockaddr_in);
            if((conn = accept(sockfd,(struct sockaddr *)&their_addr,&sin_size)) == -1)
            {
                perror("accept");
                continue;
            }
            printf("新客户已经链接！IP:%s,Port:%d\n",inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port));
            printf("当前在线人数:%d\n",++total);
            FD_SET(conn,&sett);
            if(conn > maxfd)
                maxfd = conn;
            for(int i = 0; i < 1024 ; i++)
            {
                if(cli[i] == -1 )
                {
                    cli[i] = conn;
                    clients[i].fd = conn;
                    //strcpy(clients[i].name,name);
                    break;
                }
            }
        }
        for(int j = 0; j < 1024 ;j++)
        {
            conn = cli[j];
            if(conn <= 0)
                continue;
            if(FD_ISSET(conn,&set)) 
            {
                strcpy(buf,"\0");
                numbytes = recv(conn,buf,sizeof(buf),0);
                buf[numbytes] = '\0';
                char name[20] = "\0";
                char newbuf[1024] = "\0";
                /*int m = 0;
                int n = 0;
                while(buf[m] != ':')
                {
                    name[m] = buf[m];
                    m++;                                //gao:abcde
                }
                for(int i = m+1;buf[i] != '\0';i++)
                {
                    newbuf[n++] = buf[i];
                }
                if(clients[j].fd == conn)
                {
                    strcpy(clients[j].name,name);
                }*/
                if(numbytes == -1)
                {
                    perror("recieve");
                    exit(0);
                }
                else if(numbytes == 0 )
                {
                    printf("Port = %d已经关闭连接!\n",ntohs(their_addr.sin_port));
                    printf("当前在线人数:%d\n",--total);
                    clients[j].fd = -1;
                    strcpy(clients[j].name,"\0");
                    FD_CLR(conn,&sett);
                }
                else
                {
                    fputs(buf,stdout);
                    for(int k = 0 ;k < 1024;k++)
                    {
                        if(cli[k] == -1)
                            break;
                        if(cli[k] != conn)
                        send(cli[k],buf,sizeof(buf),0);
                    }
			memset(buf,0,sizeof(buf));
                }
            }
        }
    }
    close(sockfd);
    return 0;
	
}

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
#define localIP 127.0.0.1
#define netIP 192.168.1.107
struct Client
{
    char IP[20];
    char PORT[20];
    char name[20];
    int fd;
};
struct sockaddr_in my_addr;
struct sockaddr_in their_addr;
struct Client clients[50];
int Init()
{
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket");
        exit(1);
    }
    int on = 1;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)//地址重复利用

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
    return sockfd;
}

int main()
{
    char buf[1024] = {'\0'};
    char myput[1024] = {'\0'};
    for(int i = 0;i < 50;i++)
    {
        clients[i].fd = -1;
    }
    int i = 0;
    int total = 0;
    int conn;
    int numbytes;
    int new_fd;
    fd_set set,sett;
    int sockfd = Init();
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
            int sin_size = sizeof(struct sockaddr_in);
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



            for(int i = 0; i < 50 ; i++)
            {
                if(clients[i].fd == -1 )
                {
                    clients[i].fd = conn;
                    break;
                }
            }
        }

        for(int j = 0; j < 50 ;j++)
        {
            conn = clients[j].fd;
            if(conn <= 0)
                continue;
            if(FD_ISSET(conn,&set)) 
            {
                numbytes = recv(conn,buf,sizeof(buf),0);
                buf[numbytes] = '\0';
                char name[20] = "\0";
                char newbuf[200] = "\0";
                char dname[20] = "\0";
                char msg[300] = "\0";
                int m = 0;
                int n = 0;
                while(buf[m] != ':')
                {
                    name[m] = buf[m];  //name 存发消息客户端的名字
                    m++;                                //gao:abcde
                }
                if(buf[m+1] == '@')
                {
                    int i = 0;
                    for(i = m+2 ;buf[i] != ':';i++)
                    {
                        dname[n++] = buf[i]; 
                    }
                    n = 0;
                    for(int p = i+1;buf[p] != '\0';p++)
                    {
                        newbuf[n++] = buf[p];   //newbuf 存客户端发送的消息
                    }
                }
                else
                {
                    n = 0;
                    for(int i = m+1;buf[i] != '\0';i++)
                    {
                        newbuf[n++] = buf[i];   //newbuf 存客户端发送的消息
                    }
                }
                if(clients[j].fd == conn)
                {
                    strcpy(clients[j].name,name);
                }
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
                    printf("name=%s\n",name);
                    printf("dname=%s\n",dname);
                    printf("msg=%s\n",newbuf);
                    strcat(msg,name);
                    strcat(msg,":");
                    strcat(msg,newbuf);
                    msg[strlen(msg)] = '\0';
                   for(int k = 0 ; k < 50 ;k++)
                    {
                        if(strcmp(clients[k].name,dname) == 0)
                        {
                            send(clients[k].fd,msg,sizeof(msg),0);
                        }
                    }
                }
#if 0
                    char online[300] = "\0";
                    for(int k = 0;k < 50;k++)
                    {
                        if(clients[k].fd != -1)
                        {
                            strcat(online,"当前在线人:");
                            strcat(online,clients[k].name);
                            strcat(online,",");
                        }
                    }
                    for(int k = 0;k < 50;k++)
                    {
                        if(clients[k].fd != -1 )
                        {
                            //printf("%s,%d\n",clients[k].name,clients[k].fd);
                            //send(clients[k].fd,online,sizeof(online),0);
                        }
                    }
                    memset(online,0,sizeof(online));
#endif
                    memset(buf,0,sizeof(buf));
                    memset(name,0,sizeof(name));
                    memset(dname,0,sizeof(dname));
                    memset(newbuf,0,sizeof(newbuf));
                    memset(msg,0,sizeof(msg));
            }
        }
    }
    close(sockfd);
    return 0;
}

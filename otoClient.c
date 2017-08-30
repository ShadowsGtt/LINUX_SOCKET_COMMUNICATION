#include<netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#define PORT 40009
#define MAXDATASIZE 1024 

int main(int argc,char *argv[])
{
    char newmyput[100] = "\0";
    char name[10] = "\0";
    char myput[MAXDATASIZE] = {'\0'};
    int sockfd,numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr;


    printf("请输出您的用户名:");
    fgets(name,sizeof(name),stdin);
    name[strlen(name)-1] = '\0';



    if((he = gethostbyname("127.0.0.1"))==NULL)
    {
        herror("gethostbyname");
        exit(1);
    }
    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero),8);
    if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("服务器未启动");
        exit(1);
    }
    fd_set set;
    FD_ZERO(&set);
    printf("发送给指定人消息格式为:@[对方名字][:][消息内容]\n");
    while(1)
    {
        FD_SET(0,&set);
        FD_SET(sockfd,&set);
        int sel = select(sockfd+1,&set,NULL,NULL,NULL);
        if(sel == -1)
        {
            perror("select");
            break;
        }
        if(sel == 0)
        {
            continue;
        }
        if(FD_ISSET(sockfd,&set))
        {
            numbytes=recv(sockfd,buf,MAXDATASIZE,0);
            if(numbytes == -1)
            {
                perror("recv");
                exit(1);
            }
            else if(numbytes == 0)
            {
                printf("Server has closed\n");
                exit(1);
            }
            else
                printf("%s",buf);
        }
        if(FD_ISSET(0,&set))
        {
            if(fgets(newmyput,sizeof(newmyput),stdin) == NULL)
            {
                break;
            }
            strcat(myput,name);
            strcat(myput,":");
            strcat(myput,newmyput);
            if(send(sockfd,myput,sizeof(myput),0) == -1)
            {
                perror("send");
            }
            memset(myput,0,sizeof(myput));
            memset(newmyput,0,sizeof(newmyput));
            memset(buf,0,sizeof(buf));
        }
    }
    return 0;
}

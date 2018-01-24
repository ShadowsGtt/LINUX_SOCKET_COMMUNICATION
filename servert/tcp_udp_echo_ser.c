#include "gt.h"
void handle_udp_client(int udpfd,int *client_len)
{
    char recvbuf[100] = "\0";
    int n = recvfrom(udpfd,recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&client_addr,client_len);
    if(n == -1)
    {
        perror("recvfrom");
    }
    recvbuf[n] = 0;
    printf("recvbuf:%s\n",recvbuf);
    n = sendto(udpfd,recvbuf,n,0,(struct sockaddr *)&client_addr,*client_len  );
    if(n == -1)
    {
        perror("sendto");
    }
}
void handle_tcp_client(int tcpfd)
{
    char recvbuf[100] = "\0";
    while(1)
    {
        int n = recv(tcpfd,recvbuf,sizeof(recvbuf),0 );
        if(n == -1)
            perror("recv");
        if(n == 0)
        {
            printf("client close\n");
            return ;
        }
        recvbuf[n] = 0;
        printf("recvbuf:%s\n",recvbuf);
        n = send(tcpfd,recvbuf,n,0 );
        if(n == -1)
            perror("send");
    }
}
int main()
{
    int udpfd = Socket(UDPSERV);
    int tcpfd = Socket(TCPSERV);

    pid_t pid;
    fd_set rset;
    
    int new_fd;
    int client_len = sizeof(struct sockaddr_in);
    int nready;
    int maxfd = tcpfd+1;

    while(1)
    {
        FD_SET(udpfd,&rset);
        FD_SET(tcpfd,&rset);
        if( (nready = select(maxfd,&rset,NULL,NULL,NULL) ) < 0 )
        {
            if(nready == EINTR)
            {
                printf("被中断的select已经重启\n");
                continue;
            }
            else
                perror("select");
        }
        if(FD_ISSET(udpfd,&rset) ) //监测udp Socket
        {
            handle_udp_client(udpfd,&client_len);
        }
        if(FD_ISSET(tcpfd,&rset) )  //监测tcp Socket
        {
            if((new_fd = accept(tcpfd,(struct sockaddr *)&client_addr,&client_len)) == -1)
            {
                if(new_fd == -1)
                {
                    perror("accept");
                    exit(1);
                }
            }
            printf("newclient->ip:%s\tport:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
            if( (pid = fork()) == -1)
            {
                printf("fork error\n");
                exit(1);
            }
            if(pid == 0)  //子进程处理客户请求
            {
                close(tcpfd);   //关闭继承而来的监听fd
                handle_tcp_client(new_fd);   //处理客户请求
                exit(0);
            }
            close(new_fd);
        }
    }
    return 0;
}

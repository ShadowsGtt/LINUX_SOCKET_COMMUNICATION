#include "gt.h"

/*　成功返回0
 * 出错返回-1
 * 超时返回-2
 * */
int noblock_connect(int sockfd,struct sockaddr *serv_addr,size_t len,int timeout)
{
    int old_option;
    int new_option;

    /* 获取文件描述符旧状态标志 */
    if( (old_option = fcntl(sockfd,F_GETFL,0)) < 0)
    {
        perror("fcntl GETFL");
    }
    /* 设置非阻塞标志 */
    new_option =  old_option | O_NONBLOCK;
    if(fcntl(sockfd,F_SETFL,new_option) < 0)
        perror("fcntl SETFL");
    if(connect(sockfd,serv_addr,len) == 0 )
    {
        if(fcntl(sockfd,F_SETFL,old_option) < 0)
            perror("fcntl SETFL");
        printf("直接连接成功\n");
        return 0;
    }
    else
    {
#if 1
        int error=0;
        socklen_t length = sizeof(errno);
        //调用getsockopt来获取并清除sockfd上的错误.
        if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &length) < 0)
        {
            printf("get socket option failed\n");
            close(sockfd);
            return -1;
        }
#endif
        fd_set wr,rd,er;
        FD_SET(sockfd,&wr);
        FD_SET(sockfd,&rd);
        FD_SET(sockfd,&er);
        struct timeval tv;
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        int nready = select(sockfd+1,&rd,&wr,&er,&tv);
        printf("select 返回\n");
        /* timeout */
        if(nready == 0)
            return -2;
        if(nready == -1)
        {
            printf("select error\n");
            exit(-1);
        }
        /* 可写不可读表示连接成功 */
        if(FD_ISSET(sockfd,&wr) && !FD_ISSET(sockfd,&rd))
        {
            printf("可写不可读\n");
            return 0;
        }
        if(FD_ISSET(sockfd,&er))
        {
            printf("er occor\n");
            return-1;
        }
        /* 可读又可写　
         * 1.出错时sockfd可读又可写
         * 2.刚连接上服务器就收到服务器数据也是可读又可写*/
        if(FD_ISSET(sockfd,&wr) && FD_ISSET(sockfd,&rd))
        {
            printf("既可读又可写\n");
            //int err = errno;
            if(connect(sockfd,serv_addr,len) < 0 && errno == EISCONN)
            {
                printf("已经连接啦～\n");
                if(fcntl(sockfd,F_SETFL,old_option) < 0)
                    perror("fcntl SETFL");
                return 0;
            }
            //errno = err;
            perror("connect error");
            close(sockfd);
            return -1;

        }
    }
    printf("exit\n");
    perror("exit error");
    return -1;
}

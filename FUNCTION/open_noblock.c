
#include "gt.h"
int open_noblock(int sockfd)    /* 设置非阻塞 */
{
    int old_option;
    int new_option;

    if( (old_option = fcntl(sockfd,F_GETFL,0)) < 0)  /* 获取文件描述符旧状态标志 */
    {
        perror("fcntl GETFL");
    }
    new_option =  old_option | O_NONBLOCK;   /* 设置非阻塞标志 */
    if(fcntl(sockfd,F_SETFL,new_option) < 0)
    {
        perror("fcntl SETFL");
    }

    return old_option;  /* 返回旧文件描述符便于恢复先前状态 */
}

#include "gt.h"
void sig_chld(int signo)        /* 处理信号SIG_CHLD的函数(子进程退出时会发送该信号给父进程) */
{
    pid_t pid;
    int stat;                                       /* 子进程的终止状态 */
                                                    /* 这样使用的原因:防止同一时刻有多个进程同时退出 */
                                                    /* 父进收到了多个进程发送的信号，而信号处理函数只处理了一个僵尸进程 */
    while( (pid = waitpid(-1,&stat,WNOHANG)) > 0)   /* -1表示等待第一个终止的子进程 */
        printf("child %d terminated\n",pid);        /* WNOHANG告诉内核 在没有终止的子进程时不要阻塞 */
    return ;
}

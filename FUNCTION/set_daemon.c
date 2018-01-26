
#include "gt.h"
int set_daemon(const char* pname ,int facility) /* 设置当前进程为守护进程,第一个参数为程序名字(argv[0]),第二个是设施,成功返回0  */
{
    pid_t pid;

    pid = fork();    /* 创建子进程 */
    if(pid < 0)
    {
        fprintf(stderr,"error:fork failed!");
        return (-1);
    }
    if(pid > 0)     /* 父进程退出,子进程继续运行 */
    {
        _exit(0);
    }
    if( setsid() < 0 )  /* 创建会话,此时子进程是该会话的头进程以及新进程组的进程组头进程,因此不在拥有控制终端 */
        return (-1);
    signal(SIGHUP,SIG_IGN);

    pid = fork();      /* 创建子进程的子进程 */
    if(pid < 0)
    {
        fprintf(stderr,"errno:fork failed!");
        return (-1);
    }
    if(pid > 0)         /* 子进程退出,子进程的子进程继续运行 */
        _exit(0);

    chdir("/");         /* 改变工作目录 */
    for(int i = 0;i < 24; i++)      /* 关闭该进程所有打开的文件描述符(这些文件描述符其实都是继承而来) */
        close(i);                   /* 我们猜测打开的文件描述符个数 <= 24  */

    /* 这时该进程没有打开的文件描述符 */
    open("/dev/null",O_RDONLY);     /* 将0号文件描述符(标准输入)重定向到/dev/null */
    open("/dev/null",O_RDWR);       /* 将1号文件描述符(标准输出)重定向到/dev/null */
    open("/dev/null",O_RDWR);       /* 将2号文件描述符(标准错误)定向到/dev/null */

    openlog(pname,LOG_PID,facility);/* pname:进程名字  LOG_PID:指定将进程ID填写到每个日志消息中 */
                                    /*第三个参数:一般填0(使用默认值LOG_USER) */
    
    syslog(LOG_INFO, "pid=%d daemon was started", getpid());
    closelog();
    return 0; 


//问题一:
    /*  为什么要再次调用fork() 然后再次退出子进程 */
    /* 首先你得知道一点:当没有控制终端的一个会话头进程打开一个终端设备时(该终端不会时当前某个其他会话的控制终端) */
    /* 该终端会自动成为这个会话头进程的控制终端 */
    /* 原因:为了确保本守护进程将来即使打开了一个终端设备,也不会获得控制终端.回想一下:我们第一次创建了一个子进程, */
    /* 然后我们调用_exit()使父进程退出,然后我们创建了会话,此时子进程成为会话的头进程,然后我们再次创建了子进程的子进程， */
    /* 此时子进程的子进程是会话的二号进程，接着又调用_exit()使第一次创建的子进程退出,只留下了第二次创建的子进程 */
    /* 因为这次创建的进程不是会话的头进程,所以不会出现获得控制终端的机会 */
//问题二:
    /* signal捕获信号的作用？ */
    /* 作用:防止因收到SIG_HUP信号被系统退出当前进程 */
    /* 当会话的头进程退出时会发送一个SIG_HUP信号,系统对收到该信号的进程的默认处理是退出该进程 */
}

#ifndef _GT_H
#define _GT_H
#include<netdb.h>
#include<wait.h>
#include<syslog.h>
#include <poll.h>
#include<signal.h>
#include <fcntl.h>
#include<sys/sendfile.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/select.h>
#define BACKLOG 100
#define ServPort 40009
#define MaxClients 500
#define LocalIp "127.0.0.1"
#define ServIp "127.0.0.1"
#define UDPSERV 1
#define UDPCLIENT 2
#define TCPSERV 3
#define TCPCLIENT 4
extern int Socket(int);         /* 根据参数创建相应套接字 */
extern void sig_chld(int);      /* 处理SIG_CHLD信号 */
extern int open_noblock(int);   /* 设置文件描述符为非阻塞,返回旧设置 */
extern int set_daemon(const char*,int); /* 设置当前进程为守护进程,第一个参数为程序名字(argv[0]),第二个是设施,成功返回0  */
extern void Getsockname(int ,struct sockaddr *,socklen_t *);  //获取内核赋予的ip和端口

struct sockaddr_in serv_addr;
struct sockaddr_in client_addr;
#endif

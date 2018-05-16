#ifndef _C_TW_TIMER_H
#define _C_TW_TIMER_H

#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<netdb.h>
#include<termios.h>
#include<wait.h>
#include<syslog.h>
#include <poll.h>
#include<signal.h>
#include <fcntl.h>
#include<sys/sendfile.h>
#include<pthread.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/epoll.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/select.h>

#define TIME_WHEEL_AUTO_RUN
#define BUFFER_SIZE 64
//客户端数据
typedef struct tw_timer tw_timer_t;
typedef struct time_wheel time_wheel_t;


typedef struct timer_data
{
    struct sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    tw_timer_t *timer;
}client_data_t;

typedef struct tw_timer
{
    
    int minutes;                //定时器的分钟数
    int seconds;                //定时器的秒钟数
    int time_slot;              //定时器属于时间轮上的哪个槽
    void (*callback_func)(client_data_t*);      //定时器的回调函数
    client_data_t *user_data;             //客户端数据
    tw_timer_t *prev;                     //指向上一个定时器
    tw_timer_t *next;                     //指向下一个定时器
}tw_timer_t;

typedef struct time_wheel
{
    int N;    //时间轮上槽的数目
    int TI;    //槽间隔时间，即每1秒时间轮转动一次
    int cur_slot;               //秒针所在位置
    void (*add_timer)(time_wheel_t *,unsigned timeout,
            void (*callback_func)(client_data_t *),
            client_data_t *);
    void (*del_timer)(time_wheel_t *,tw_timer_t *);
    void (*tick)();         //促使秒针转动的函数 
    tw_timer_t *slots[0];      //时间轮的槽,每一个槽的元素指向一个双向链表
    
}time_wheel_t;

/* 向时间轮中插入新的定时器
* @tw:时间轮
* @timeout:定时器时间
* @callback_func:定时器过期时执行的回调函数
* @client_data_t:回调函数需要的参数
* */
extern void AddTimer(time_wheel_t *tw,unsigned timeout,
                     void (*callback_func)(client_data_t *),
                     client_data_t *data );

/* 删除时间轮里的某个定时器 */
extern void DeleteTimer(time_wheel_t *tw,tw_timer_t *tw_timer);

/* 时间轮转动函数,即促使秒针走动的函数*/
extern void tick(time_wheel_t *tw);

extern time_wheel_t *CreateTimeWheel(const int N,const int TI);


extern void TimeWheelStartRunning(time_wheel_t *);

extern void *TimeWheelThread(void *);
#endif

#include "../include/wechat.h"
#include "../include/C_tw_timer.h"

/* 向时间轮中插入新的定时器
* @tw:时间轮
* @timeout:定时器时间
* @callback_func:定时器过期时执行的回调函数
* @client_data_t:回调函数需要的参数
* */
void AddTimer(time_wheel_t *tw,unsigned timeout,
                     void (*callback_func)(client_data_t *),client_data_t *data )
{
    /* 入口参数检查 */
    if( tw == NULL )
        return;
    if(timeout <= 0)
        return;
    if(callback_func == NULL)
        return;

    /* 创建定时器 */
    tw_timer_t *tw_timer = (tw_timer_t *)malloc(sizeof(tw_timer_t));
    assert(tw_timer != NULL);

    /* 初始化定时器 */
    tw_timer->callback_func = callback_func;
    tw_timer->prev = NULL;
    tw_timer->next = NULL;
    tw_timer->user_data = data;

    /* 待插入定时器所需要总ticks */
    int ticks = 0;
    ticks = timeout / (tw->TI);

    /* 计算待插入的定时器在时间轮上要转动多少圈后触发 */
    int minutes = ticks / (tw->N);   

    /* 计算定时器应该被插入的位置 */
    int ts = ((tw->cur_slot) + ticks) % (tw->N); 

    /* 初始化定时器所在槽的位置 */
    tw_timer->time_slot = ts; 
    
    /* 初始化定时器分钟数 */
    tw_timer->minutes = minutes;

    /* 初始化定时器秒数 */
    tw_timer->seconds = ticks % tw->N;

    //如果槽为空，则它新定时器插入，并设置为该槽的头节点
    if (!(tw->slots[ts])) {
        printf("新定时器:%d 分 %d 秒 ,过期时秒针位置: %d\n",
               tw_timer->minutes, tw_timer->seconds, tw_timer->time_slot);
        tw->slots[ts] = tw_timer;
    }
    else {
        printf("新定时器:%d 分 %d 秒 ,过期时秒针位置: %d\n",
               tw_timer->minutes, tw_timer->seconds, tw_timer->time_slot);
        tw_timer->next = tw->slots[ts];
        tw->slots[ts]->prev = tw_timer;
        tw->slots[ts] = tw_timer;
    }
}

/* 删除时间轮里的某个定时器 */
void DeleteTimer(time_wheel_t *tw,tw_timer_t *tw_timer)
{
    printf("删除时间为:%d 分 %d 秒的定时器\n",tw_timer->minutes,tw_timer->seconds);
    
    if (tw_timer == NULL)
        return;
    if(tw == NULL)
        return;

    /* 取得定时器所在槽下标 */
    int ts = tw_timer->time_slot;
    if (tw_timer == tw->slots[ts]) {   //如果是头结点
        tw->slots[ts] = tw->slots[ts]->next;
        if (tw->slots[ts])
            tw->slots[ts]->prev = NULL;

        free(tw_timer);
    }
    else {
        tw_timer->prev->next = tw_timer->next;
        if (tw_timer->next)
            tw_timer->next->prev = tw_timer->prev;
        free(tw_timer);
    }

}

/* 时间轮转动函数,即促使秒针走动的函数*/
void tick(time_wheel_t *tw)
{
   //取得时间轮上当前槽的头结点
   tw_timer_t *tmp = tw->slots[tw->cur_slot];
   printf("秒针位置: %d\n", tw->cur_slot);

   while (tmp) {

       //如果定时器的minutes值大于0，则未到时，不处理
       if (tmp->minutes > 0) {
           tmp->minutes--;
           tmp = tmp->next;
       }
       else {
           printf("定时器到时,即将调用回调函数\n");
           tmp->callback_func(tmp->user_data);
           tw_timer_t *tmp2 = tmp->next;
           tw->del_timer(tw,tmp);
           tmp = tmp2;
       }
   }
   //更新秒针位置，以反映时间轮的转动
   tw->cur_slot = (++tw->cur_slot) % (tw->N);
    
}

/* 创建时间轮 */
time_wheel_t *CreateTimeWheel(const int N,const int TI)
{
    time_wheel_t *tw = (time_wheel_t *)malloc
    (sizeof(time_wheel_t) + sizeof(tw_timer_t*)*N);
    assert(tw != NULL);
    tw->N = N;
    tw->TI = TI;
    tw->add_timer = AddTimer;
    tw->del_timer = DeleteTimer;
    tw->cur_slot = 0;
    tw->tick = tick;
    for(int i = 0 ;i < N;i++)
        tw->slots[i] = NULL;

#ifdef TIME_WHEEL_AUTO_RUN
    printf("has define\n");
    TimeWheelStartRunning(tw);
#endif
    return tw;
}

/* 摧毁时间轮 */
void DestoryTimeWheel(time_wheel_t *tw)
{
    tw_timer_t *tmp ;
    for(int i = 0;i < tw->N;i++)
    {
        if(!tw->slots[i])
        {
            tmp = tw->slots[i];
            while(!tmp)
                free(tmp);
        }
    }
    free(tw);
    tw = NULL;
}

void TimeWheelStartRunning(time_wheel_t *wheel)
{
    printf("time wheel start running.....\n");

    pthread_t tid;
    if( !pthread_create(&tid,NULL,TimeWheelThread,(void*)wheel) )
    {
        printf("time wheel running failed\n");
        perror("error mesg");
        return ;
    }
}
void *TimeWheelThread(void *arg)
{
    time_wheel_t *wheel = (time_wheel_t *)arg;
    struct timeval val;
    while(1)
    {
        val.tv_sec = 1;
        val.tv_usec = 0;
        int res = select(1,NULL,NULL,NULL,&val);
        if(!res)
            wheel->tick(wheel);
    }
}



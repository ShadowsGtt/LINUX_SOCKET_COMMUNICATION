#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100

struct Message
{
    pthread_t tid;
    double value;
}thr[MAX];

double Money ;
int number ;
int k = 0;
double Rands_number(double LeftMoney) //剩余红包内随机生成红包数值
{
    srand((unsigned)time(NULL));
    double value= rand()/(double)(RAND_MAX/LeftMoney);
    int temp = value*100;
    value = ((double)temp/100);
    return value;
}
void *Main_thread()  //输出结果线程
{
    printf("开始抢红包啦!\t金额=%lf\n",Money);
}
pthread_mutex_t lock;
void *thread1()
{
    //pthread_mutex_init (&g_mutex_Msg,NULL);
    pthread_mutex_lock (&lock);
    double value;
    if(number != 1)
        value = Rands_number(Money);
    else
        value = Money;
    thr[k++].value = value;
    Money -= value;
    number--;
    pthread_mutex_unlock(&lock);
}
int main()
{
    int i ;
    printf("input the number and Money:");
    scanf("%d%lf",&number,&Money);
    int total = number;
    for(i = 0;i < total;i++)
    {
        if((pthread_create(&(thr[i].tid),NULL,thread1,NULL)) )
           perror("thread failed"); 
    }
    for(int j = 0;j < total;j++)
    {
        printf("Money:%.2lf\n",thr[j].value);
    }
    pthread_mutex_destroy(&lock);
    return 0;
}

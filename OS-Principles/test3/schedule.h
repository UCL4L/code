#ifndef _SCHEDULE_H
#define _SCHEDULE_H
#include<stdio.h>
#include<string.h>
const int TIME_SLOT=5; //时间片为 5
const int MAXPCB=50; //最大进程数
//进程结构体
typedef struct node
{
    char p_name[20]; //进程名
    int p_status; //1 就绪 2 等待 3 运行
    int p_runningtime; //需要运行的时间
    int p_priority; //优先权
    int p_finished; //是否运行完成
    int p_wait_time; //已等待时间
}pcb;
pcb pcbs[MAXPCB]; //进程数组
int nums; //实际进程数
#endif
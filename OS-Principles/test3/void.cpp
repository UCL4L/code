#include <stdio.h> 
#include "schedule.h" 

//先来先服务算法
void fcfs()
{
    int i,j; int total;
    //输出 FCFS 算法执行顺序
    printf("*****************************************************\n");
    printf("FCFS 算法执行顺序:\n");
    printf("进程名\t 等待时间\n");
    for(i=0;i<nums;i++)
    {
        printf("%s\t%d\n",pcbs[i].p_name,pcbs[i].p_wait_time);
        for(j=i+1;j<nums;j++)
        pcbs[j].p_wait_time+=pcbs[i].p_runningtime;
    }
    total=0;
    for(i=0;i<nums;i++) 
    total+=pcbs[i].p_wait_time;
    printf("总等待时间: %d 平均等待时间: %.2f\n",total,total*1.0/nums);
}

//优先级高优选调度算法模拟
void p_priority()
{
    int i, j, p, total, current_priority;
    int passed_time = 0;
    int queue[MAXPCB];
    for (i = 0; i < nums; i++)
    {
        current_priority = 1000;
        for (j = 0; j < nums; j++)
        {
            if ((pcbs[j].p_finished == 0) && (pcbs[j].p_priority < current_priority))
            {
                p = j;
                current_priority = pcbs[j].p_priority;
            }
        }
        queue[i] = p;
        pcbs[p].p_finished = 1;
        pcbs[p].p_wait_time += passed_time;
        passed_time += pcbs[p].p_runningtime;
    }
    //输出高优先权调度执行顺序
    printf("*****************************************************\n");
    printf("高优先权调度执行顺序:\n");
    printf("进程名\t 等待时间\n");
    for (i = 0; i < nums; i++)
        printf("%s\t%d\n", pcbs[queue[i]].p_name, pcbs[queue[i]].p_wait_time);
    total = 0;
    for (i = 0; i < nums; i++)
        total += pcbs[i].p_wait_time;
    printf("总等待时间: %d 平均等待时间: %.2f\n", total, total * 1.0 / nums);
}
//时间片轮转调度算法模拟
void p_round_robin()
{
    int i, j, number, flag = 1, round = 0, total = 0;
    int queue[1000];
    while (flag)
    {
        flag = 0;
        number = 0;
        for (i = 0; i < nums; i++)
        {
            if (pcbs[i].p_finished == 0)
            {
                number++;
                j = i;
            }
        }
        if (number == 1)
        {
            queue[total] = j;
            total++;
            pcbs[j].p_finished = 1;
        }
        if (number > 1)
        {
            for (i = 0; i < nums; i++)
            {
                if (pcbs[i].p_finished == 0)
                {
                    flag = 1;
                    queue[total] = i;
                    total++;
                    if (pcbs[i].p_runningtime <= TIME_SLOT * (round + 1))
                        pcbs[i].p_finished = 1;
                }
            }
        }
        round++;
    }
    if (queue[total - 1] == queue[total - 2])
        total--;
    printf("*******************************************************\n");
    printf("时间片轮转调度执行顺序:\n");
    for (i = 0; i < total; i++)
    {
        printf("%s ", pcbs[queue[i]].p_name);
        printf("\n");
    }
}
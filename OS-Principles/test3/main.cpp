#include "schedule.h"
#include "void.cpp"
//初始化函数
void initial()
{
    int i;
    for (i = 0; i < MAXPCB; i++)
    {
        strcpy(pcbs[i].p_name, "");
        pcbs[i].p_status = 0;
        pcbs[i].p_runningtime = 0;
        pcbs[i].p_priority = 0;
        pcbs[i].p_finished = 0;
        pcbs[i].p_wait_time = 0;
    }
    nums = 0;
}
//从文件读进程数据
int readPData()
{
    FILE *fp;
    char fp_name[20];
    int i;
    printf("请输入进程文件名:");
    scanf("%s", fp_name);
    if ((fp = fopen(fp_name, "r")) == NULL)
        printf("错误,文件打不开,请检查文件名");
    else
    {
        while (!feof(fp))
        {
            fscanf(fp, "%s %d %d %d", pcbs[nums].p_name, &pcbs[nums].p_status,
                   &pcbs[nums].p_runningtime, &pcbs[nums].p_priority);
            nums++;
        }
        printf("从文件读入的进程数据:\n");
        printf("进程名\t 进程状态\t 运行时间\t 优先权\n");
        for (i = 0; i < nums; i++)
            printf("%s\t%d\t\t%d\t\t%d\n", pcbs[i].p_name, pcbs[i].p_status, pcbs[i].p_runningtime, pcbs[i].p_priority);
        return 1;
    }
    return 0;
}
//重置数据,以供另一个算法使用
void init()
{
    int i;
    for (i = 0; i < MAXPCB; i++)
    {
        pcbs[i].p_finished = 0;
        pcbs[i].p_wait_time = 0;
    }
}

int main()
{
    int flag;
    printf(" ********************* 进程调度 ********************\n");
    initial();
    flag = readPData();
    if (flag == 1)
    {
        fcfs();
        init();
        p_priority();
        init();
        p_round_robin();
    }
    return 0;
}
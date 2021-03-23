#include "memswap.h"
#include <string.h>
//创建页面号引用串
void CreateRS()
{
	int i;
	printf("请输入进程逻辑空间的页面个数: ");
	scanf_s("%d", &rs_size);
	if (rs_size > RS_MAX_SIZE)
	{
		printf("非法！页面个数超出进程空间的最大范围\n");
		return;
	}
	printf("请输入进程页面号引用串(空格隔开，<=%d 个页面号):\n", RS_MAX_SIZE);
	for (i = 0; i < rs_size; i++) scanf_s("%d", &rs[i]);
}
//显示进程装在物理页框中的页面号
void printPF(int n)
{
	int i;
	for (i = 0; i < n; i++) printf("%d ", pf[i]);
	printf("\n");
}
//进程物理块中是否包含逻辑页，包含不置换，否则块满，置换
int Contains(int rs_p) //rs_p: 虚存上的逻辑页号
{
	int i, result = 0;
	for (i = 0; i < P; i++)
	{
		if (rs_p == pf[i])
		{
			result = 1;
			break;
		}
	}
	return result;
}
//页面置换
void Swap(int which)
{
	int i, swap_loc = 0, count = 0;
	char s[20];
	for (i = 0; i < rs_size; i++)
	{
		if (i < P) //有空页框
		{
			pf[i] = rs[i];
			printf("当前内存页：");
			printPF(i + 1);
		}
		else //无空页框，可能需要换页
		{
			//当前物理页框中是否含有逻辑页面
			if (!Contains(rs[i])) //逻辑页在物理块中不存在，将发生置换
			{
				count++; //页面置换次数统计
				if (which == 1) //最优置换
				{
					swap_loc = OPT(i);
					printf("第[%d]次页面置换：物理块号[%d]上的页[%d]被换出到虚存，逻辑页[%d]被调入内存。\n",count,swap_loc,pf[swap_loc],rs[i]);
						pf[swap_loc] = rs[i];
				}
				else if (which == 2) //先进先出置换
				{
					printf("第[%d]次页面置换：物理块号[%d]上的页[%d]被换出到虚存，逻辑页[%d]被调入内存。\n",count,swap_loc,pf[swap_loc],rs[i]);
						pf[swap_loc] = rs[i];
					swap_loc++;
					swap_loc %= P;
				}
				else //最近最久未使用置换
				{
					//Todo: 期待你的实现...
				}
				printf("当前内存页：");
				printPF(P);
			}
			else printf("访问页面[%d]未缺页\n", rs[i]);
		}
	}
	printf("进程执行完毕!\n");
	if (which == 1) strcpy_s(s, "最佳");
	else if (which == 2) strcpy_s(s, "先进先出");
	else strcpy_s(s, "最近最久未使用");
	printf("采用%s 置换算法，总共发生了%d 次页面置换。\n\n\n", s, count);
}
int main()
{
	int choice, flag = 1;
	printf("本程序为页面置换算法模拟练习:\n");
	printf("目前你的进程分配的物理内存页框个数为：%d\n", P);
	CreateRS();
	while (flag)
	{
		printf("请选择: \n");
		printf("1.最佳（OPT）页面置换算法\n");
		printf("2.先进先出（FIFO）页面置换算法\n");
		printf("3.最近最久未使用（LRU）页面置换算法\n");
		printf("4.退出程序\n");
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("最佳（OPT）页面置换算法运行结果：\n");
			Swap(1);
			break;
		case 2:
			printf("先进先出（FIFO）页面置换算法运行结果：\n");
			Swap(2);
			break;
		case 3:
			printf("最近最久未使用（LRU）页面置换算法运行结果：\n");
			printf("不好意思，需要你自己实现算法，才能看到结果!\n\n\n");
			break;
		default:
			flag = 0;
		}
	}
	return 0;
}
//标记清理
void cleanFlag()
{
	int i;
	for (i = 0; i < P; i++) call_loc[i] = 0;
}
//最佳置换算法
//计算最佳置换淘汰物理页块号
int OPT(int begin)
{
	int i, j, swap_loc, count = 0;
	cleanFlag();
	for (j = begin + 1; j < rs_size; j++)
	{
		for (i = 0; i < P; i++)
		{
			if (rs[j] == pf[i] && call_loc[i] == 0)
			{
				call_loc[i] = -1;
				count++;
				break;
			}
		}
		if (P - count == 1) break;
	}
	for (i = 0; i < P; i++)
	{
		if (call_loc[i] != -1)
		{
			swap_loc = i;
			break;
		}
	}
	return swap_loc;
}
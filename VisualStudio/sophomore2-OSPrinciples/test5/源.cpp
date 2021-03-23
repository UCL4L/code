#include <stdio.h>
#include <conio.h>
#define MAXPROCESS 50                         /*最大进程数*/
#define MAXRESOURCE 100                       /*最大资源数*/
int AVAILABLE[MAXRESOURCE];                   /*所剩可用资源数组*/
int MAX[MAXPROCESS][MAXRESOURCE];             /*最大需求矩阵*/
int ALLOCATION[MAXPROCESS][MAXRESOURCE];      /*分配矩阵*/
int NEED[MAXPROCESS][MAXRESOURCE];            /*当前需求矩阵*/
int REQUEST[MAXPROCESS][MAXRESOURCE];         /*进程需要资源数*/
bool FINISH[MAXPROCESS];                      /*系统是否有足够的资源分配*/
int p[MAXPROCESS];                            /*记录序列*/
int m, n;                                     /*m 个进程,n 个资源*/
int WORK[MAXRESOURCE];
int temp[MAXRESOURCE];                        //记录进程安全执行的顺序
void Init();
bool Safe();
void Bank();
void main()
{
	Init();
	Safe();
	Bank();
}
void Init() { /*初始化算法*/
	int i, j;
	printf("请输入进程的数目:");
	scanf_s("%d", &m);

	printf("请输入资源的种类:");
	scanf_s("%d", &n);

	printf("请输入每个进程最多所需的各资源数,按照%d x %d 矩阵输入\n", m, n);
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) scanf_s("%d", &MAX[i][j]);

	printf("请输入每个进程已分配的各资源数,也按照%d x %d 矩阵输入\n", m, n);
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			scanf_s("%d", &ALLOCATION[i][j]);
			NEED[i][j] = MAX[i][j] - ALLOCATION[i][j];
			if (NEED[i][j] < 0)
			{
				printf("您输入的第%d 个进程所拥有的第%d 个资源数错误,请重新输入:\n", i + 1, j + 1);
				j--;
				//continue;
			}
		}
	}
	printf("请输入各资源现有数量:\n");
	for (i = 0; i < n; i++) scanf_s("%d", &AVAILABLE[i]);
}
/*银行家算法*/
void Bank()
{
	int i, cusneed;
	char flag;
	while (1)
	{
		printf("请输入申请资源的进程号(注:第 1 个进程号为 0,依次类推)\n");
		scanf_s("%d", &cusneed);

		printf("请输入进程请求各资源的数量\n");
		for (i = 0; i < n; i++) scanf_s("%d", &REQUEST[cusneed][i]);

		for (i = 0; i < n; i++)
		{
			if (REQUEST[cusneed][i] > NEED[cusneed][i])
			{
				printf("输入请求数>进程需求量!请重新输入!\n");
				continue;
			}
			if (REQUEST[cusneed][i] > AVAILABLE[i]) {
				printf("输入请求数>系统现有资源数!请重新输入!\n");
				continue;
			}
		}
		for (i = 0; i < n; i++) {
			AVAILABLE[i] -= REQUEST[cusneed][i];
			ALLOCATION[cusneed][i] += REQUEST[cusneed][i];
			NEED[cusneed][i] -= REQUEST[cusneed][i];
		}
		if (Safe())
			printf("同意分配请求!\n");
		else {
			printf("请求被拒绝!\n");
			for (i = 0; i < n; i++) {
				AVAILABLE[i] += REQUEST[cusneed][i];
				ALLOCATION[cusneed][i] -= REQUEST[cusneed][i];
				NEED[cusneed][i] += REQUEST[cusneed][i];
			}
		}
		for (i = 0; i < m; i++) FINISH[i] = false;

		printf("再次请求分配吗?是按 y 或 Y,否按其它键\n");
		flag = getch();
		if (flag == 'y' || flag == 'Y') continue;
		break;
	}
}
/*安全性算法*/
bool Safe()
{
	int i, j, m = 0, k = 0, c;
	for (i = 0; i < m; i++)
		FINISH[i] = false;
	for (j = 0; j < n; j++)
		WORK[j] = AVAILABLE[j];//把可利用资源数赋给WORK[]
	for (i = 0; i < m; i++)
	{
		c = 0;
		for (j = 0; j < n; j++)
			if (FINISH[i] == false && NEED[i][j] <= WORK[j])
				c++;
		if (c == n)  
		{
			for (m = 0; m < n; m++)
				WORK[m] = WORK[m] + ALLOCATION[i][m];
			FINISH[i] = true;
			temp[k] = i;//记录下满足条件的进程
			k++;
			i = -1;
		}
	}
	for (i = 0; i < m; i++)
		if (FINISH[i] == false)
		{
			printf("系统不安全!!!本次资源申请不成功!!!\n");
			return 1;
		}
	printf("\n");
	printf("经安全性检查，系统安全，本次分配成功。\n");
	printf("\n");
	printf("本次安全序列：");
	for (i = 0; i < m; i++)//打印安全系统的进程调用顺序
	{
		printf("进程");
		printf("%d", temp[i]);
		if (i < m - 1)
			printf("->");
	}
	printf("\n");
	return 0;
}
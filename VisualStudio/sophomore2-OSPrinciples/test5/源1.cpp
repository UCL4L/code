#include <stdio.h>
#include <conio.h>
#include <string.h>
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


	int listNumber = 0;
	bool key = false;

	int colNum = 1;
	int i = 0;
	int a = 0;
	while (i < m) {    //外层循环，循环遍历每一个进程
		int available[MAXRESOURCE];
		memcpy(available, AVAILABLE, sizeof(AVAILABLE));
		int index = 0;   //队列下标
		for (int i = 0; i < m; i++)
			FINISH[i] = false;


		for (int j = 0; j < n; j++) {
			FINISH[i] = true;
			if (available[j] < NEED[i][j]) {    //判断系统所有资源是否满足该进程资所需资源
				FINISH[i] = false;
				break;
			}
		}

		if (FINISH[i] == true) {			//如果满足将该资源作为队列的头
			p[index] = i;					//记录对头
			free(available, i);		//释放该进程的所有资源
			int recordIn = 1;				//用于判断是否构成安全序列
			while (a < n) {
				for (int x = 0; x < m; x++) {   //确定序列头后，继续确定后面的进程

					if (FINISH[x] == true)   //已经加入序列的进程跳出当前循环继续判断其它的
						continue;

					else {           //未加入则判断

						for (int q = 0; q < n; q++)
						{
							FINISH[x] = true;
							if (available[q] < NEED[x][q]) {   //判断系统所有资源是否满足该进程资所需资源
								FINISH[x] = false;
								break;
							}
						}

						if (FINISH[x] == true) {   //满足
							index++;              //序列号递增
							p[index] = x;	      //加入序列
							Release(available, x);  //释放资源
							recordIn++;
						}

					}

				}

				if (recordIn == m) {   //判断是否构成了安全序列，是则输出
					printf("第%d个安全序列为：", colNum++);
					for (int i = 0; i < m; i++) {
						printf("%d ", p[i]);
					}
					printf("\n");
					key = true;
					break;
				}
				a++;
			}
		}
		i++;
	}

	return key;

}

int *Release(int available[], int line) {

	for (int i = 0; i < n; i++) {
		available[i] += ALLOCATION[line][i];
	}

	return available;
}
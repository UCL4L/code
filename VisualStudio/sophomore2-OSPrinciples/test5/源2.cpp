/*安全性检查函数*/
intchkerr()//在假定分配资源的情况下检查系统的安全性
{
	intWORK[N], FINISH[M], temp[M];//temp[]用来记录进程安全执行的顺序
	inti, j, m, k = 0, count;
	for (i = 0; i < M; i++)
		FINISH[i] = FALSE;
	for (j = 0; j < N; j++)
		WORK[j] = AVAILABLE[j];//把可利用资源数赋给WORK[]
	for (i = 0; i < M; i++)
	{
		count = 0;
		for (j = 0; j < N; j++)
			if (FINISH[i] == FALSE && NEED[i][j] <= WORK[j])
				count++;
		if (count == N)//当进程各类资源都满足NEED<=WORK时
		{
			for (m = 0; m < N; m++)
				WORK[m] = WORK[m] + ALLOCATION[i][m];
			FINISH[i] = TRUE;
			temp[k] = i;//记录下满足条件的进程
			k++;
			i = -1;
		}
	}
	for (i = 0; i < M; i++)
		if (FINISH[i] == FALSE)
		{
			printf("系统不安全!!!本次资源申请不成功!!!\n");
			return1;
		}
	printf("\n");
	printf("经安全性检查，系统安全，本次分配成功。\n");
	printf("\n");
	printf("本次安全序列：");
	for (i = 0; i < M; i++)//打印安全系统的进程调用顺序
	{
		printf("进程");
		printf("%d", temp[i]);
		if (i < M - 1)
			printf("->");
	}
	printf("\n");
	return0;
}
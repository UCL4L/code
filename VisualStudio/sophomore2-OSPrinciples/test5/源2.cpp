/*��ȫ�Լ�麯��*/
intchkerr()//�ڼٶ�������Դ������¼��ϵͳ�İ�ȫ��
{
	intWORK[N], FINISH[M], temp[M];//temp[]������¼���̰�ȫִ�е�˳��
	inti, j, m, k = 0, count;
	for (i = 0; i < M; i++)
		FINISH[i] = FALSE;
	for (j = 0; j < N; j++)
		WORK[j] = AVAILABLE[j];//�ѿ�������Դ������WORK[]
	for (i = 0; i < M; i++)
	{
		count = 0;
		for (j = 0; j < N; j++)
			if (FINISH[i] == FALSE && NEED[i][j] <= WORK[j])
				count++;
		if (count == N)//�����̸�����Դ������NEED<=WORKʱ
		{
			for (m = 0; m < N; m++)
				WORK[m] = WORK[m] + ALLOCATION[i][m];
			FINISH[i] = TRUE;
			temp[k] = i;//��¼�����������Ľ���
			k++;
			i = -1;
		}
	}
	for (i = 0; i < M; i++)
		if (FINISH[i] == FALSE)
		{
			printf("ϵͳ����ȫ!!!������Դ���벻�ɹ�!!!\n");
			return1;
		}
	printf("\n");
	printf("����ȫ�Լ�飬ϵͳ��ȫ�����η���ɹ���\n");
	printf("\n");
	printf("���ΰ�ȫ���У�");
	for (i = 0; i < M; i++)//��ӡ��ȫϵͳ�Ľ��̵���˳��
	{
		printf("����");
		printf("%d", temp[i]);
		if (i < M - 1)
			printf("->");
	}
	printf("\n");
	return0;
}
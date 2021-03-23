#include <stdio.h>
#include <conio.h>
#define MAXPROCESS 50                         /*��������*/
#define MAXRESOURCE 100                       /*�����Դ��*/
int AVAILABLE[MAXRESOURCE];                   /*��ʣ������Դ����*/
int MAX[MAXPROCESS][MAXRESOURCE];             /*����������*/
int ALLOCATION[MAXPROCESS][MAXRESOURCE];      /*�������*/
int NEED[MAXPROCESS][MAXRESOURCE];            /*��ǰ�������*/
int REQUEST[MAXPROCESS][MAXRESOURCE];         /*������Ҫ��Դ��*/
bool FINISH[MAXPROCESS];                      /*ϵͳ�Ƿ����㹻����Դ����*/
int p[MAXPROCESS];                            /*��¼����*/
int m, n;                                     /*m ������,n ����Դ*/
int WORK[MAXRESOURCE];
int temp[MAXRESOURCE];                        //��¼���̰�ȫִ�е�˳��
void Init();
bool Safe();
void Bank();
void main()
{
	Init();
	Safe();
	Bank();
}
void Init() { /*��ʼ���㷨*/
	int i, j;
	printf("��������̵���Ŀ:");
	scanf_s("%d", &m);

	printf("��������Դ������:");
	scanf_s("%d", &n);

	printf("������ÿ�������������ĸ���Դ��,����%d x %d ��������\n", m, n);
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) scanf_s("%d", &MAX[i][j]);

	printf("������ÿ�������ѷ���ĸ���Դ��,Ҳ����%d x %d ��������\n", m, n);
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			scanf_s("%d", &ALLOCATION[i][j]);
			NEED[i][j] = MAX[i][j] - ALLOCATION[i][j];
			if (NEED[i][j] < 0)
			{
				printf("������ĵ�%d ��������ӵ�еĵ�%d ����Դ������,����������:\n", i + 1, j + 1);
				j--;
				//continue;
			}
		}
	}
	printf("���������Դ��������:\n");
	for (i = 0; i < n; i++) scanf_s("%d", &AVAILABLE[i]);
}
/*���м��㷨*/
void Bank()
{
	int i, cusneed;
	char flag;
	while (1)
	{
		printf("������������Դ�Ľ��̺�(ע:�� 1 �����̺�Ϊ 0,��������)\n");
		scanf_s("%d", &cusneed);

		printf("����������������Դ������\n");
		for (i = 0; i < n; i++) scanf_s("%d", &REQUEST[cusneed][i]);

		for (i = 0; i < n; i++)
		{
			if (REQUEST[cusneed][i] > NEED[cusneed][i])
			{
				printf("����������>����������!����������!\n");
				continue;
			}
			if (REQUEST[cusneed][i] > AVAILABLE[i]) {
				printf("����������>ϵͳ������Դ��!����������!\n");
				continue;
			}
		}
		for (i = 0; i < n; i++) {
			AVAILABLE[i] -= REQUEST[cusneed][i];
			ALLOCATION[cusneed][i] += REQUEST[cusneed][i];
			NEED[cusneed][i] -= REQUEST[cusneed][i];
		}
		if (Safe())
			printf("ͬ���������!\n");
		else {
			printf("���󱻾ܾ�!\n");
			for (i = 0; i < n; i++) {
				AVAILABLE[i] += REQUEST[cusneed][i];
				ALLOCATION[cusneed][i] -= REQUEST[cusneed][i];
				NEED[cusneed][i] += REQUEST[cusneed][i];
			}
		}
		for (i = 0; i < m; i++) FINISH[i] = false;

		printf("�ٴ����������?�ǰ� y �� Y,��������\n");
		flag = getch();
		if (flag == 'y' || flag == 'Y') continue;
		break;
	}
}
/*��ȫ���㷨*/
bool Safe()
{
	int i, j, m = 0, k = 0, c;
	for (i = 0; i < m; i++)
		FINISH[i] = false;
	for (j = 0; j < n; j++)
		WORK[j] = AVAILABLE[j];//�ѿ�������Դ������WORK[]
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
			temp[k] = i;//��¼�����������Ľ���
			k++;
			i = -1;
		}
	}
	for (i = 0; i < m; i++)
		if (FINISH[i] == false)
		{
			printf("ϵͳ����ȫ!!!������Դ���벻�ɹ�!!!\n");
			return 1;
		}
	printf("\n");
	printf("����ȫ�Լ�飬ϵͳ��ȫ�����η���ɹ���\n");
	printf("\n");
	printf("���ΰ�ȫ���У�");
	for (i = 0; i < m; i++)//��ӡ��ȫϵͳ�Ľ��̵���˳��
	{
		printf("����");
		printf("%d", temp[i]);
		if (i < m - 1)
			printf("->");
	}
	printf("\n");
	return 0;
}
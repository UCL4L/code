#include <stdio.h>
#include <conio.h>
#include <string.h>
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


	int listNumber = 0;
	bool key = false;

	int colNum = 1;
	int i = 0;
	int a = 0;
	while (i < m) {    //���ѭ����ѭ������ÿһ������
		int available[MAXRESOURCE];
		memcpy(available, AVAILABLE, sizeof(AVAILABLE));
		int index = 0;   //�����±�
		for (int i = 0; i < m; i++)
			FINISH[i] = false;


		for (int j = 0; j < n; j++) {
			FINISH[i] = true;
			if (available[j] < NEED[i][j]) {    //�ж�ϵͳ������Դ�Ƿ�����ý�����������Դ
				FINISH[i] = false;
				break;
			}
		}

		if (FINISH[i] == true) {			//������㽫����Դ��Ϊ���е�ͷ
			p[index] = i;					//��¼��ͷ
			free(available, i);		//�ͷŸý��̵�������Դ
			int recordIn = 1;				//�����ж��Ƿ񹹳ɰ�ȫ����
			while (a < n) {
				for (int x = 0; x < m; x++) {   //ȷ������ͷ�󣬼���ȷ������Ľ���

					if (FINISH[x] == true)   //�Ѿ��������еĽ���������ǰѭ�������ж�������
						continue;

					else {           //δ�������ж�

						for (int q = 0; q < n; q++)
						{
							FINISH[x] = true;
							if (available[q] < NEED[x][q]) {   //�ж�ϵͳ������Դ�Ƿ�����ý�����������Դ
								FINISH[x] = false;
								break;
							}
						}

						if (FINISH[x] == true) {   //����
							index++;              //���кŵ���
							p[index] = x;	      //��������
							Release(available, x);  //�ͷ���Դ
							recordIn++;
						}

					}

				}

				if (recordIn == m) {   //�ж��Ƿ񹹳��˰�ȫ���У��������
					printf("��%d����ȫ����Ϊ��", colNum++);
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
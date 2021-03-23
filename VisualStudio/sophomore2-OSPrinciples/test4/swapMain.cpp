#include "memswap.h"
#include <string.h>
//����ҳ������ô�
void CreateRS()
{
	int i;
	printf("����������߼��ռ��ҳ�����: ");
	scanf_s("%d", &rs_size);
	if (rs_size > RS_MAX_SIZE)
	{
		printf("�Ƿ���ҳ������������̿ռ�����Χ\n");
		return;
	}
	printf("���������ҳ������ô�(�ո������<=%d ��ҳ���):\n", RS_MAX_SIZE);
	for (i = 0; i < rs_size; i++) scanf_s("%d", &rs[i]);
}
//��ʾ����װ������ҳ���е�ҳ���
void printPF(int n)
{
	int i;
	for (i = 0; i < n; i++) printf("%d ", pf[i]);
	printf("\n");
}
//������������Ƿ�����߼�ҳ���������û�������������û�
int Contains(int rs_p) //rs_p: ����ϵ��߼�ҳ��
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
//ҳ���û�
void Swap(int which)
{
	int i, swap_loc = 0, count = 0;
	char s[20];
	for (i = 0; i < rs_size; i++)
	{
		if (i < P) //�п�ҳ��
		{
			pf[i] = rs[i];
			printf("��ǰ�ڴ�ҳ��");
			printPF(i + 1);
		}
		else //�޿�ҳ�򣬿�����Ҫ��ҳ
		{
			//��ǰ����ҳ�����Ƿ����߼�ҳ��
			if (!Contains(rs[i])) //�߼�ҳ��������в����ڣ��������û�
			{
				count++; //ҳ���û�����ͳ��
				if (which == 1) //�����û�
				{
					swap_loc = OPT(i);
					printf("��[%d]��ҳ���û���������[%d]�ϵ�ҳ[%d]����������棬�߼�ҳ[%d]�������ڴ档\n",count,swap_loc,pf[swap_loc],rs[i]);
						pf[swap_loc] = rs[i];
				}
				else if (which == 2) //�Ƚ��ȳ��û�
				{
					printf("��[%d]��ҳ���û���������[%d]�ϵ�ҳ[%d]����������棬�߼�ҳ[%d]�������ڴ档\n",count,swap_loc,pf[swap_loc],rs[i]);
						pf[swap_loc] = rs[i];
					swap_loc++;
					swap_loc %= P;
				}
				else //������δʹ���û�
				{
					//Todo: �ڴ����ʵ��...
				}
				printf("��ǰ�ڴ�ҳ��");
				printPF(P);
			}
			else printf("����ҳ��[%d]δȱҳ\n", rs[i]);
		}
	}
	printf("����ִ�����!\n");
	if (which == 1) strcpy_s(s, "���");
	else if (which == 2) strcpy_s(s, "�Ƚ��ȳ�");
	else strcpy_s(s, "������δʹ��");
	printf("����%s �û��㷨���ܹ�������%d ��ҳ���û���\n\n\n", s, count);
}
int main()
{
	int choice, flag = 1;
	printf("������Ϊҳ���û��㷨ģ����ϰ:\n");
	printf("Ŀǰ��Ľ��̷���������ڴ�ҳ�����Ϊ��%d\n", P);
	CreateRS();
	while (flag)
	{
		printf("��ѡ��: \n");
		printf("1.��ѣ�OPT��ҳ���û��㷨\n");
		printf("2.�Ƚ��ȳ���FIFO��ҳ���û��㷨\n");
		printf("3.������δʹ�ã�LRU��ҳ���û��㷨\n");
		printf("4.�˳�����\n");
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("��ѣ�OPT��ҳ���û��㷨���н����\n");
			Swap(1);
			break;
		case 2:
			printf("�Ƚ��ȳ���FIFO��ҳ���û��㷨���н����\n");
			Swap(2);
			break;
		case 3:
			printf("������δʹ�ã�LRU��ҳ���û��㷨���н����\n");
			printf("������˼����Ҫ���Լ�ʵ���㷨�����ܿ������!\n\n\n");
			break;
		default:
			flag = 0;
		}
	}
	return 0;
}
//�������
void cleanFlag()
{
	int i;
	for (i = 0; i < P; i++) call_loc[i] = 0;
}
//����û��㷨
//��������û���̭����ҳ���
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
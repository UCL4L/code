#pragma  warning(disable:4996)
#include <stdio.h>
#include <string.h>
#define N 50//������Ŀ����
#define M 26//������ĸ����

void WordSort(char word[N][M], int n);
void print(const char word[][M], int n);//const ����һ�����ı�ĳ���
void main()
{
	char word[N][M];
	int n = 0, i;
	printf("���뵥�ʸ���:");
	scanf("%d", &n);
	rewind(stdin);

	while (n < 1 || n>50)
	{
		printf("����ĵ���������");
		scanf("%d", &n);
	}

	printf("������%d������,�ո�ֿ������ʵĳ��Ȳ��ܳ���%d\n", n, M);
	rewind(stdin);

	for (i = 0; i < n; i++)
	{
		scanf("%s", word[i]);
		word[i][M - 1] = '\0';
	}

	WordSort(word, n);
	print(word, n);
	printf("�����������Ҫ�Ը�����߸����ĵ����������޸�N,M��ֵ\n");
}

//��������
void WordSort(char word[N][M], int n)
{
	char temp[N + 1];//������ֵʱ�Ĺ���
	int i, j;
	for (i = 0; i < n; ++i)
	{
		for (j = 1; j < n - i - 1; ++j)
		{
			if (strcmp(word[j], word[j + 1]) > 0)//�����뵥�ʱȽϴ�С
			{
				strcpy(temp, word[j + 1]);//�ǲ��뷨����ð������
				strcpy(word[j + 1], word[j]);
				strcpy(word[j], temp);
			}
		}
	}
}

//��ӡ����
void print(const char word[][M], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("������:%s\n", word[i]);
	}
}
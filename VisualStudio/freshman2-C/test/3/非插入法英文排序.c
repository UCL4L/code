#pragma  warning(disable:4996)
#include <stdio.h>
#include <string.h>
#define N 50//单词数目限制
#define M 26//单词字母限制

void WordSort(char word[N][M], int n);
void print(const char word[][M], int n);//const 定义一个不改变的常量
void main()
{
	char word[N][M];
	int n = 0, i;
	printf("输入单词个数:");
	scanf("%d", &n);
	rewind(stdin);

	while (n < 1 || n>50)
	{
		printf("输入的单词数错误！");
		scanf("%d", &n);
	}

	printf("请输入%d个单词,空格分开，单词的长度不能超过%d\n", n, M);
	rewind(stdin);

	for (i = 0; i < n; i++)
	{
		scanf("%s", word[i]);
		word[i][M - 1] = '\0';
	}

	WordSort(word, n);
	print(word, n);
	printf("程序结束，若要对更多或者更长的单词排序请修改N,M的值\n");
}

//插入排序
void WordSort(char word[N][M], int n)
{
	char temp[N + 1];//用作赋值时的过渡
	int i, j;
	for (i = 0; i < n; ++i)
	{
		for (j = 1; j < n - i - 1; ++j)
		{
			if (strcmp(word[j], word[j + 1]) > 0)//对输入单词比较大小
			{
				strcpy(temp, word[j + 1]);//非插入法排序，冒泡排序
				strcpy(word[j + 1], word[j]);
				strcpy(word[j], temp);
			}
		}
	}
}

//打印数组
void print(const char word[][M], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("输出结果:%s\n", word[i]);
	}
}
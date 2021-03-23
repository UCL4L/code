// input.c : input user information and save to a file "record.dat".

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <conio.h>

struct user_info
{
	char name[12];
    int age;
    int weight;
};

int main(int argc, char* argv[])
{
	char	buff[128];
	struct user_info info;
	FILE	*fp;
//    errno_t ret;
	int	count = 0;

	printf("\n%s\n\n","该程序用于把用户输入的用户信息写入record.dat文件");

    fp = fopen("record.dat", "wb+");
    if(fp == NULL)
	{
        printf("打开文件record.dat失败,程序结束\n");
		return -1;
	}

	while(1)
	{
		memset(&info, 0, sizeof(struct user_info));
        printf("请输入一个用户的信息(直接回车结束录入):\n");

		memset(buff, 0, 128);
		printf("姓名:");
		fgets(buff, 128, stdin);
		if(strlen(buff) <= 1) 
		{
            printf("结束输入用户信息\n");
			break;
		}
		memcpy(info.name, buff, strlen(buff) - 1);

		while(1)
		{
			memset(buff, 0, 128);
            printf("年龄:");
			fgets(buff, 128, stdin);
			if(strlen(buff) <= 1)
			{
				printf("%s\n", "年龄数据不能为空,请重新输入");
			}
			else
			{
				info.age = atoi(buff);
				break;
			}
		}

		while(1)
		{
			memset(buff, 0, 128);
			printf("体重:");
			fgets(buff, 128, stdin);
			if(strlen(buff) <= 1)
			{
				printf("%s\n", "体重数据不能为空,请重新输入");
			
			}
			else
			{
				info.weight = atoi(buff);
				break;
			}
		}

        if(fwrite(&info, sizeof(struct user_info), 1, fp) == 0)
		{
			printf("%s", "写文件失败!");
			break;
		}
		count ++;
	}

	printf("当前成功录入%d个用户信息\n\n", count);

	fclose(fp);

	printf("%s", "按任意键结束程序...");
	getchar();

	return 0;
}

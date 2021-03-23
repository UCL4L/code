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

	printf("\n%s\n\n","�ó������ڰ��û�������û���Ϣд��record.dat�ļ�");

    fp = fopen("record.dat", "wb+");
    if(fp == NULL)
	{
        printf("���ļ�record.datʧ��,�������\n");
		return -1;
	}

	while(1)
	{
		memset(&info, 0, sizeof(struct user_info));
        printf("������һ���û�����Ϣ(ֱ�ӻس�����¼��):\n");

		memset(buff, 0, 128);
		printf("����:");
		fgets(buff, 128, stdin);
		if(strlen(buff) <= 1) 
		{
            printf("���������û���Ϣ\n");
			break;
		}
		memcpy(info.name, buff, strlen(buff) - 1);

		while(1)
		{
			memset(buff, 0, 128);
            printf("����:");
			fgets(buff, 128, stdin);
			if(strlen(buff) <= 1)
			{
				printf("%s\n", "�������ݲ���Ϊ��,����������");
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
			printf("����:");
			fgets(buff, 128, stdin);
			if(strlen(buff) <= 1)
			{
				printf("%s\n", "�������ݲ���Ϊ��,����������");
			
			}
			else
			{
				info.weight = atoi(buff);
				break;
			}
		}

        if(fwrite(&info, sizeof(struct user_info), 1, fp) == 0)
		{
			printf("%s", "д�ļ�ʧ��!");
			break;
		}
		count ++;
	}

	printf("��ǰ�ɹ�¼��%d���û���Ϣ\n\n", count);

	fclose(fp);

	printf("%s", "���������������...");
	getchar();

	return 0;
}

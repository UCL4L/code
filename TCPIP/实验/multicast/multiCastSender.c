#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include </usr/include/errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define		BUFF_LEN	 1024
	
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in to;
	char *ipaddr="226.1.2.3";
	unsigned short port = 5006;		//端口号
	char Buffer[BUFF_LEN];	//收发缓冲区
	int retval;				//临时变量
	int addrLen;

	addrLen = sizeof(struct sockaddr_in);

	//建立UDP套接字
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock <0)
	{
		printf("socket()失败，错误号：%d\n", errno);
		
		return -1;
	}

	memset(&to, 0, sizeof(to));
	to.sin_addr.s_addr = inet_addr(ipaddr);	//多播组"226.1.2.3"
	to.sin_family = AF_INET;
	to.sin_port = htons(port);

	//循环发送组播报文,输入"quit"可退出程序
	while(1)
	{
		memset(Buffer, 0, BUFF_LEN);	
		printf("请输入欲组播的信息:");
		fgets(Buffer,sizeof(Buffer),stdin);
		if(!strlen(Buffer)) //如果没有输入信息，继续循环
			continue;

		retval = sendto(sock, Buffer, strlen(Buffer)+1, 0, (struct sockaddr *)&to, sizeof(to));
		if (retval <0)
		{
			printf("发送失败，错误号是：%d", errno);
			break;
		}
		else
		{
			printf("已发送 %d字节的信息：%s\n\n", retval, Buffer);
			//程序正常退出条件
			if(!strncasecmp(Buffer, "quit",4)) 
				break;
		}
	}
	
	close(sock);
	
	return 0;
}

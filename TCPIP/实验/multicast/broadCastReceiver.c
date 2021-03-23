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

int main(void)
{
	struct sockaddr_in local, from;	//套节字地址结构变量
	int socket_type = SOCK_DGRAM;	//UDP套接字
	unsigned short port= 5005;		//端口号
	int sock;					//套节字变量
	int fromlen;			//地址结构长度

	char Buffer[BUFF_LEN];			//收发缓冲区
	int retval;				//临时变量

	//建立套节字
	sock = socket(AF_INET, socket_type, 0); 
	if (sock <0)
	{
		printf("socket()失败，错误号：%d\n", errno);
		
		return -1;
	}

	//拷贝本机地址信息到sockaddr_in结构中
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY; 
	local.sin_port = htons(port);
	fromlen =sizeof(from);

	//邦定网络地址到套节字
	if (bind(sock,(struct sockaddr*)&local, sizeof(local) ) <0) 
	{
		printf("bind()失败，错误号：%d\n", errno);
		close(sock);
		
		return -1;
	}		

	//循环接收与发送数据
	puts("等待数据到来.............");
	while(1)
	{
		//等待接收网络信息
		retval = recvfrom(sock, Buffer, sizeof (Buffer), 0,	(struct sockaddr *)&from, &fromlen);
		//结果判断
		if (retval <0) 
		{
			printf("recv()失败，错误号：%d\n", errno);
			break;
		}
		else //正常接收
		{
			printf("从%s的端口%d收到 %d 字节的数据:%s\n", inet_ntoa(from.sin_addr), htons(from.sin_port), retval,Buffer);
			//程序的一个退出条件
			if(!strncasecmp(Buffer, "quit",4)) 
				break;
		}
	}

	 close(sock);
	
	return 0;
}

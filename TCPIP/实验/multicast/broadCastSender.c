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
	int sock;	//收发数据用的套接口
	struct sockaddr_in sendtoAdrr;
	unsigned short port = 5005;	//端口号
	int sendtoAdrrLen, fromAddrLen;	//地址结构长度

	char Buffer[BUFF_LEN];	//收发缓冲区
	int retval = 0;
	int bBroadCast = 1;	//设置可广播套接口
	int optLen;				//广播选项值长度

	//建立UDP套接字
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock <0 )
	{
		printf("socket()失败，错误号：%d\n", errno);
		
		return -1;
	}

	//设置套接口为允许广播的
	optLen = sizeof(bBroadCast);
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bBroadCast, optLen) <0)
	{
		printf("设置套接口为允许广播失败! 错误号%d.\n",errno);
		close(sock);
		
		return -1;
	}

	//检验是否已成功将套接口设置为允许广播,不是必要的
	if (getsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bBroadCast, &optLen) <0)
	{
		printf("检验套接口的函数getsockopt失败!....\n");
		close(sock);
		
		return -1;
	}

	if (!bBroadCast)
	{
		printf("经检验，设置套接口允许广播失败！ \n");
	}

	//拷贝目标主机地址信息到sendtoAdrr
	memset(&sendtoAdrr, 0, sizeof(sendtoAdrr));
	sendtoAdrr.sin_family = AF_INET;
	sendtoAdrr.sin_addr.s_addr = INADDR_BROADCAST;	//"255.255.255.255" 
	sendtoAdrr.sin_port = htons(port);

	sendtoAdrrLen = sizeof(sendtoAdrr);

	//循环发送报文
	while(1)
	{
		memset(Buffer, 0, BUFF_LEN);	
		puts("请输入欲广播的信息:");
		fgets(Buffer,sizeof(Buffer),stdin);
		if(!strlen(Buffer)) //如果没有输入信息，继续循环
			continue;
		
		//发送广播报文
		retval = sendto(sock, Buffer, strlen(Buffer)+1, 0, (struct sockaddr *)&sendtoAdrr, sendtoAdrrLen);
		if (retval <0)
		{
			printf("发送广播报文失败! 错误码是： %d\n", errno);
			break;
		}
		else	//正常发送
		{
			printf("已发送广播报文： %s.......\n", Buffer);
			//程序的一个退出条件
			if(!strncasecmp(Buffer, "quit",4)) 
				break;
		}
	}

	close(sock);
	
	return 0;
}



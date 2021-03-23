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

int main(void)
{
    unsigned short port= 5001;		//端口号
    int socket_type = SOCK_STREAM;	//缺省使用TCP连接
    char Buffer[1280];			//收发缓冲区
    int retval, fromlen, i, isconnected = 0;	//临时变量
    struct sockaddr_in local, from;	//套节字地址结构变量
    int listen_socket, msgsock;	//套节字变量
 
	//建立套节字
	listen_socket = socket(PF_INET, socket_type, 0); 

	// 拷贝 解析的信息到sockaddr_in结构中
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY; 
	local.sin_port = htons(port);
	//邦定网络地址到套节字
	bind(listen_socket,(struct sockaddr*)&local, sizeof(local));
	//对于流式套节字，让套节字处于监听状态，等待连接到来
	listen(listen_socket, 5);
	while(1)
	{
		fromlen =sizeof(from); 
		if( isconnected == 0)//对于TCP连接，如果没有连接，等待一个连接的到来
		{
			printf("wait client connect....\n");
			msgsock = accept(listen_socket,(struct sockaddr*)&from, &fromlen);
			isconnected = 1;
			printf("从%s收到连接, 端口是：%d\n", inet_ntoa(from.sin_addr), htons(from.sin_port));
		}

		//等待接收网络信息
		retval = recv(msgsock, Buffer, sizeof (Buffer), 0 ); //面向tcp，类似BSD read()
		if (retval <= 0) //结果判断
		{
			printf("recv()失败or客户端关闭连接，错误号：%d\n", strerror(errno));
			close(msgsock);
			continue;
		}
		else //正常接收
		{
			printf("收到 %d 字节的数据：%s \n", retval, Buffer);
			if(!strncasecmp(Buffer, "exit",4)) 
			{
				printf("关闭TCP 连接，等待下一次连接...\n");
				close(msgsock);
				isconnected = 0;
				continue;
			}
		}

		send(msgsock, Buffer, strlen(Buffer)+1, 0);
		//程序的一个退出条件
		if(!strncasecmp(Buffer, "quit",4))
		{
			close(msgsock);
			break;
		}
	}

	close(listen_socket);
	return 0;
}

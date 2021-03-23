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

extern int	errno;
int processmsg(char *buf, struct sockaddr_in addr, int buflen);

int main(void)
{
    unsigned short port= 5001;		//端口号
    int socket_type = SOCK_STREAM;	//缺省使用TCP连接
    char Buffer[1280];			//收发缓冲区
    int retval, fromlen, i, isconnected = 0;	//临时变量

    struct sockaddr_in local, from;	//套节字地址结构变量
    int listen_socket, msgsock;	//套节字变量

	//先输入服务器程序所需要的基本信息：端口号，套节字类型
	printf("请输入端口号( >5000 ):");
	scanf("%d", &i);
	if(i < 5000 || i > 65535)
		printf("不正确的端口号%d,用缺省端口号5001\n",i);
	else
		port = i;

 
	//建立套节字
	listen_socket = socket(PF_INET, socket_type, 0); 
	if (listen_socket < 0)
	{
		printf("socket(）失败，错误：%d\n",strerror(errno));
		return -1;
	}

	// 拷贝 解析的信息到sockaddr_in结构中
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY; 
	local.sin_port = htons(port);
	//邦定网络地址到套节字
	if (bind(listen_socket,(struct sockaddr*)&local, sizeof(local) ) < 0)
	{
		printf("bind()失败，错误：%d\n",strerror(errno));
		return -1;
	}

	//对于流式套节字，让套节字处于监听状态，等待连接到来
	if (listen(listen_socket, 5)  < 0)
	{
		printf("listen()失败，错误：%d\n",strerror(errno));
		return -1;
	}

	printf("监听端口：%d, 使用协议：%s\n", port,
		(socket_type == SOCK_STREAM)? "TCP": "UDP");

	while(1)
	{
		fromlen =sizeof(from); 

		if( isconnected == 0)//对于TCP连接，如果没有连接，等待一个连接的到来
		{
			msgsock = accept(listen_socket,(struct sockaddr*)&from, &fromlen);
			if (msgsock  < 0)
			{
				printf("accept()失败，错误：%d\n",strerror(errno));
				return -1;
			}

			isconnected = 1;
			printf("从%s收到连接, 端口是：%d\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
		}

		//等待接收网络信息
		retval = recv(msgsock, Buffer, sizeof (Buffer), 0 ); //面向tcp，类似BSD read()

		//结果判断
		if (retval < 0) 
		{
			printf("recv()失败，错误号：%d\n", strerror(errno));
			close(msgsock);
			isconnected = 0;
			continue;
		 }
		else if (retval == 0) 
		{
			printf("客户端关闭连接\n");
			close(msgsock);
			isconnected = 0;
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

		//接收信息的处理
		processmsg(Buffer, from, 1280);

		retval = send(msgsock, Buffer, strlen(Buffer)+1, 0);	//面向tcp，类似BSD write()
		if (retval < 0) 
		{
			printf("send()失败，错误号：%d\n", strerror(errno));
		}

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


//将buf的信息进行解析，然后产生回答信息放入buf。
int processmsg(char *buf, struct sockaddr_in addr, int buflen)
{
	//仅仅打印信息
	printf("从%s收到数据报:%s，端口是%d\n", inet_ntoa(addr.sin_addr), buf,htons(addr.sin_port));
	printf("将同样的信息回送给客户端...\n\n");
	return 0;
}

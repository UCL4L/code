#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include </usr/include/errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE	128
extern int	errno;
int quitag = 0;

void *Handle(void *param)
{
	int clientsock =  (int) param;
	int retval;
	char buf[BUFFER_SIZE];

	while(1)
	{
		//接收网络信息
		memset(buf, 0 , BUFFER_SIZE);
		retval = recv(clientsock, buf, BUFFER_SIZE, 0 );//面向tcp，类似BSD read()

		if (retval < 0) 
		{
			printf("接收信息失败，错误号：%d\n",strerror(errno));
			break;
		}
		else if (retval == 0) // 对于面向连接的套节字，需要判断对方是否关闭连接
		{
			printf("TCP连接已经关闭\n");
			break;
		}
		else //正确接收到网络信息
		{
			printf("(收到信息)socket = %d的信息：%s \n",clientsock, buf);
			send(clientsock,buf,retval, 0);
			if(!strncasecmp(buf, "exit",4))
				break;
			else if(!strncasecmp(buf, "quit",4))
			{
				quitag=1;
				break;
			}
		}
	}
	close(clientsock);
	printf("处理socket=%d线程结束!", clientsock);
}

void ErrorPro(char *msg)
{
	printf("%d失败，错误号：%d\n",msg, strerror(errno));
	exit(-1);
}

int main(void)
{
	unsigned short port= 5001;		//端口号
	int socket_type = SOCK_STREAM;	//缺省使用TCP连接
	int retval, fromlen;
	struct sockaddr_in local, from;	//套节字地址结构变量
	int listen_socket, msgsock;	//套节字变量
	pthread_t ntid;
 
	//建立套节字
	listen_socket = socket(AF_INET, socket_type, 0); 
	if (listen_socket < 0)
	{
		ErrorPro("socket");
	}

	// 拷贝 解析的信息到sockaddr_in结构中
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY; 
	local.sin_port = htons(port);
	//绑定网络地址到套节字
	if (bind(listen_socket,(struct sockaddr*)&local, sizeof(local) ) < 0) 
	{
		ErrorPro("bind");
	}

	//对于流式套节字，让套节字处于监听状态，等待连接到来
	if (listen(listen_socket, 5) < 0) 
	{
		ErrorPro("listen");
	}

	printf("监听端口：%d, 使用TCP\n", port);
	fromlen =sizeof(from); 

	while(1)
	{
		msgsock = accept(listen_socket,(struct sockaddr*)&from, &fromlen);
		if (msgsock < 0) 
		{
			ErrorPro("accept");
		}
		printf("从%s收到连接, 端口是：%d\n", inet_ntoa(from.sin_addr), htons(from.sin_port));
			//利用专用线程进行接收处理

		if(quitag)
			break;

		retval = pthread_create(&ntid, NULL, Handle, (void *)msgsock);
    	if (retval != 0)
		{
			printf("无法创建线程，原因是: %s\n", strerror(retval));    
			break;
		}    		

	}

	close(msgsock);
	close(listen_socket);
	
	return 0;
}

//gcc threadtcpserver.c -lpthread -o ttcps

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include </usr/include/errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern int	errno;

int main(void)
{
    unsigned short port= 5001;		//端口号
    int socket_type = SOCK_DGRAM;	//使用UDP套接字
    char Buffer[1280];				//收发缓冲区
    int retval, fromlen, i, isconnected = 0;		//临时变量

    struct sockaddr_in local, from;	//套节字地址结构变量
    int sock;	//套节字变量

	//先输入服务器程序所需要的基本信息：端口号，套节字类型
	printf("请输入端口号( >5000 ):");
	scanf("%d", &i);
	if(i < 5000 || i > 65535)
		printf("不正确的端口号%d,用缺省端口号5001\n",i);
	else
		port = i;

   
	//建立套节字
	sock = socket(AF_INET, socket_type, 0); 
	if (sock < 0)
	{
		printf("socket()失败，错误号：%d\n", strerror(errno));
		return -1;
	}

	// 拷贝 解析的信息到sockaddr_in结构中
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY; 
	local.sin_port = htons(port);
	//邦定网络地址到套节字
	if (bind(sock,(struct sockaddr*)&local, sizeof(local) ) < 0) 
	{
		printf("bind()失败，错误：%d\n", strerror(errno));
		return -1;
	}

	printf("监听端口：%d, 使用UDP协议\n", port);

	while(1)
	{
		fromlen =sizeof(from); 

		//等待接收网络信息
		retval = recvfrom(sock, Buffer, sizeof (Buffer), 0, 
				(struct sockaddr *)&from, &fromlen);			//面向UDP
		printf("从%s收到数据报，端口是：%d\n", inet_ntoa(from.sin_addr), htons(from.sin_port));

		//结果判断
		if (retval < 0) 
		{
			printf("recv()失败，错误：%d\n", strerror(errno));
			close(sock);
			continue;
		}
		else if (retval == 0) 
		{
			printf("客户端关闭连接\n");
			close(sock);
			continue;
		}
		else //正常接收
		{
			printf("收到 %d 字节的数据：%s \n", retval, Buffer);
		}

		//信息回送
		printf("将同样的信息回送给客户端...\n\n");
		retval = sendto(sock, Buffer, strlen(Buffer)+1, 0, 
			(struct sockaddr *)&from, fromlen);				//面向UDP

		if (retval < 0) 
		{
			printf("send()失败，错误：%d\n", strerror(errno));
		 }

		//程序的一个退出条件
		if(!strncasecmp(Buffer, "quit",4))
			break;
	}

	return 0;
}

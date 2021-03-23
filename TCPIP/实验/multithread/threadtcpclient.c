#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include </usr/include/errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE	1280
char Buffer[BUFFER_SIZE];	//收发缓冲区

void *Handle(void *param)
{
	int clientsock =  (int)param;
	int retval;

	while(1)
	{
		//接收网络信息
		memset(Buffer, 0 , BUFFER_SIZE);
		retval = recv(clientsock, Buffer, BUFFER_SIZE, 0 );//面向tcp，类似BSD read()

		 if (retval <0 ) 
		{
			printf("接收信息失败，错误号：%d\n",errno);
			break;
		}
		else if (retval == 0) // 对于面向连接的套节字，需要判断对方是否关闭连接
		{
            	printf("TCP连接已经关闭\n");
            	break;
        }
		else //正确接收到网络信息
			printf("(收到信息)%s \n", Buffer);
	}
	printf("读线程结束！");
}


int main(void)
{
	unsigned short port = 5001;		//端口号
	unsigned int addr;	//IP地址变量
	int retval,i;	//临时变量
	pthread_t ntid;
		
	struct sockaddr_in server; //套节字地址结构变量
	struct hostent *hp;				//主机信息结构变量指针
	int  conn_socket;			//套节字变量

	//先输入客户程序所需要的基本信息：服务器地址，端口号，套节字类型
	printf("请输入服务器地址：");
	scanf("%s", Buffer);

	printf("请输入端口号( >5000 ):");
	scanf("%d", &i);
	if(i < 5000 || i > 65535)
		printf("不正确的端口号%d,用缺省端口号5001\n", i);
	else
		port = i;

   	 //取得主机IP地址
   	 if (isalpha(Buffer[0])) //是域名或命名地址
        hp = gethostbyname(Buffer);
	else 
	{
		addr = inet_addr(Buffer);//是点分地址
		hp = gethostbyaddr((char *)&addr,4,AF_INET);
	}

	if (hp == NULL ) 
	{
		printf("不能解析地址[%s]: 错误号%d\n", Buffer, errno);
		exit(1);
	}

	// 拷贝 解析的信息到sockaddr_in结构中
	memset(&server,0,sizeof(server));
	memcpy(&(server.sin_addr),hp->h_addr,hp->h_length);
	server.sin_family = hp->h_addrtype;
	server.sin_port = htons(port);

	//建立套节字
	conn_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (conn_socket <0 ) 
	{
		printf("socket(）失败，错误号：%d\n", errno);
		 
		return -1;
	}

	//对于流式套节字，需要先建立连接
	printf("正在连接到: %s\n",hp->h_name);
	if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server)) <0 ) 
	{
		printf("connect()失败，错误号：%d\n",errno);
		close(conn_socket);
		 
		return -1;
	}

	//利用专用线程进行接收处理
	retval = pthread_create(&ntid, NULL, Handle, (void *)conn_socket);
	if (retval != 0)
	{
		printf("can't create thread: %s\n", strerror(retval));
		close(conn_socket);   
		return -1;
	}   
		
	while(1) 
	{
		//得到要发送的信息字符串
		printf("\n输入送到服务器的信息:");
		fgets(Buffer,sizeof(Buffer),stdin);
		if(!strlen(Buffer)) //如果没有输入信息，继续循环
			continue;

		//发送网络信息
		 retval = send(conn_socket, Buffer, strlen(Buffer)+1, 0);	//面向tcp，类似BSD write()
		if (retval < 0) 
		{
			printf("发送失败，错误号： %d\n", errno);
			break;
		 }
		else
			printf("发送信息=%s\n", Buffer);

		//程序的一个退出条件
		if(strncasecmp(Buffer, "quit",4)==0 || strncasecmp(Buffer, "exit",4)==0 ) 
			break;
	}
	//关闭套节字
	close(conn_socket);
	return 0;
}
//gcc threadtcpclient.c -lpthread -o ttcpc

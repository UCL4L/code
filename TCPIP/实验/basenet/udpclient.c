#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include </usr/include/errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

//#include <conio.h>
extern int	errno;

int main(void)
{
    unsigned short port = 5001;		//端口号
    int socket_type = SOCK_DGRAM;	//使用TCP连接

    char Buffer[1280];		//收发缓冲区
    unsigned int addr;		//IP地址变量
    int retval,i;		//临时变量

    struct sockaddr_in server, from; 	//套节字地址结构变量
    struct hostent *hp;		//主机信息结构变量指针
    int  conn_socket;		//套节字变量

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
		printf("不能解析地址[%s]: 错误:%d\n", Buffer, strerror(errno));
		exit(1);
	}

	// 拷贝 解析的信息到sockaddr_in结构中
	memset(&server,0,sizeof(server));
	memcpy(&(server.sin_addr),hp->h_addr,hp->h_length);
	server.sin_family = hp->h_addrtype;
	server.sin_port = htons(port);

	//建立套节字
	conn_socket = socket(AF_INET,socket_type,0);
	if (conn_socket < 0) 
	{
		printf("socket(）失败，错误：%d\n",strerror(errno));
		return -1;
	}

	while(1) 
	{
		//得到要发送的信息字符串
		puts("\n输入送到服务器的信息:");
		fgets(Buffer,sizeof(Buffer),stdin);
		if(!strlen(Buffer)) //如果没有输入信息，继续循环
			continue;

		//发送网络信息
		retval = sendto(conn_socket, Buffer, strlen(Buffer)+1, 0,	//发送字符串的时候，须将字符串的结束符\0发送
				(struct sockaddr*)&server, sizeof(server));			//面向UDP
		if (retval < 0) 
		{
			printf("发送失败，错误： %d\n", strerror(errno));
			break;
		}
		else
			printf("发送信息：%s\n", Buffer);

		//接收网络信息
		i = sizeof(from);
		retval = recvfrom(conn_socket, Buffer, sizeof(Buffer), 0,
					(struct sockaddr*)&from, &i);			//面向UDP
		if (retval < 0) 
		{
			printf("接收信息失败，错误：%d\n",strerror(errno));
			break;
		}
		else if (retval == 0) // 对于面向连接的套节字，需要判断对方是否关闭连接
		{
			printf("服务器关闭连接\n");
			break;
		 }
		else //正确接收到网络信息
			printf("收到 %d 字节信息：%s \n", retval, Buffer);

		//程序的一个退出条件
		if(!strncasecmp(Buffer, "quit", 4)) 
			break;
	}
	//关闭套节字
	close(conn_socket);
	return 0;
}

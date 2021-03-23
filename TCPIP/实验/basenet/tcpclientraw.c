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

int main(void)
{
    unsigned short port = 5001;	//端口号
    char *ipaddr="127.0.0.1";
    char Buffer[512];			//收发缓冲区
    struct sockaddr_in server; 	//套节字地址结构变量
    int  conn_socket;			//套节字变量
    int retval;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ipaddr); 
	server.sin_port = htons(port);
	
	//建立套节字
	conn_socket = socket(PF_INET,SOCK_STREAM,0);
	connect(conn_socket,(struct sockaddr*)&server,sizeof(server)); 

	printf("连接到: %s\n",ipaddr);
    while(1) 
	{
		//得到要发送的信息字符串
		puts("\n输入送到服务器的信息:");
		fgets(Buffer,sizeof(Buffer),stdin);
		if(!strlen(Buffer)) //如果没有输入信息，继续循环
			continue;

		//发送网络信息
		send(conn_socket, Buffer, strlen(Buffer)+1, 0);	//面向tcp，类似BSD write()
		//接收网络信息
		retval = recv(conn_socket, Buffer, sizeof(Buffer), 0 );//面向tcp，类似BSD read()
		printf("收到 %d 字节信息：%s \n", retval, Buffer);

		//程序的一个退出条件
		if(!strncasecmp(Buffer, "quit",4)) 
			break;
		if(!strncasecmp(Buffer, "exit",4)) 
			break;
    }

	//关闭套节字
	close(conn_socket);
	return 0;
}

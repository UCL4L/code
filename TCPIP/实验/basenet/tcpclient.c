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

extern int	errno;

int parseaddr(char *Buffer, struct sockaddr_in *haddr, unsigned short port)
{
	unsigned int addr;	//IP地址变量
	int ret = 0;
	struct hostent *hp;				//主机信息结构变量指针

	memset(haddr,0,sizeof(struct sockaddr_in));

   	 //取得主机IP地址
	if (isalpha(Buffer[0])) //是域名或命名地址
	{
		hp = gethostbyname(Buffer);
			// 拷贝 解析的信息到sockaddr_in结构中
		if (hp == NULL ) 
		{
			printf("不能解析地址[%s]: 错误号%d\n", Buffer, errno);
			ret = -1;
		}
		else
			memcpy(&(haddr->sin_addr),hp->h_addr,hp->h_length);
	 }
	else 
	{
		addr = inet_addr(Buffer);//是点分地址
		if(addr == INADDR_NONE) //地址非法
		{
			printf("输入非法地址：%s\n",Buffer);
			ret = -1;
		}
		else
		{
			haddr->sin_addr.s_addr = addr;
		}
	}

	if(ret == 0)
	{
		haddr->sin_family = AF_INET;
		haddr->sin_port = htons(port);
	}

	return ret;
}

int main(void)
{
    unsigned short port = 5001;		//端口号
    int socket_type = SOCK_STREAM;	//缺省使用TCP连接

    char Buffer[1280];	//收发缓冲区
    unsigned int addr;	//IP地址变量
    int retval,i;	//临时变量

    struct sockaddr_in server, from; //套节字地址结构变量
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


	if(parseaddr(Buffer, &server,port) < 0)
	{
		printf("无法使用的地址：%s，程序退出！\n",Buffer);
		return -1;
	}

	//建立套节字
	conn_socket = socket(PF_INET,socket_type,0);
	if (conn_socket <0 ) 
	{
		printf("socket(）失败，错误：%s\n",strerror(errno));
		return -1;
	}

	//对于流式套节字，需要先建立连接
	//其实也可以使用connect(),send(),recv()等函数在windows里面一样实现数据报套节字功能

	printf("正在连接到: %s\n",Buffer);
	if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server)) < 0) 
	{
		printf("socket(）失败，错误：%s\n",strerror(errno));
            	close(conn_socket);
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
		 retval = send(conn_socket, Buffer, strlen(Buffer)+1, 0);	//面向tcp，类似BSD write()
		if (retval < 0) 
		{
			printf("发送失败，错误号： %d\n", errno);
			break;
		 }
		else
			printf("发送信息：%s\n", Buffer);

		//接收网络信息
		retval = recv(conn_socket, Buffer, sizeof(Buffer), 0 );//面向tcp，类似BSD read()

		 if (retval < 0) 
		{
			printf("接收信息失败，错误：%s\n",strerror(errno));
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
		if(!strncasecmp(Buffer, "quit",4)) 
			break;
		if(!strncasecmp(Buffer, "exit",4)) 
			break;
    }

	//关闭套节字
	close(conn_socket);
	return 0;
}

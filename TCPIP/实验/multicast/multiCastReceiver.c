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

int mc_join(int s, struct in_addr *mcaddr, struct in_addr *local_if);  //加上多播组
int mc_leave(int s, struct in_addr *mcaddr, struct in_addr *local_if); //退出多播组

int main(int argc, char *argv[])
{
	int sock;	//收发数据用的套接口
	struct sockaddr_in local, from;
	struct in_addr mcaddr;
	int addrLen;			//地址结构长度
	unsigned short port = 5006;

	char *ipaddr="226.1.2.3";
	char Buffer[BUFF_LEN];	//收发缓冲区
	int retval;				//临时变量
	
	addrLen = sizeof(struct sockaddr_in);

	//建立UDP套接字
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		printf("socket()失败，错误号：%d\n", errno);
		
		return -1;
	}

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = INADDR_ANY;	//本地任意以太网接口IP
	

	//绑定本地主机IP与端口
	retval = bind(sock, (struct sockaddr *)&local, sizeof(local));
	if (retval <0)
	{
		printf("bind失败，错误号是：%d\n", errno);
	}		

	//加入组播组
	mcaddr.s_addr = inet_addr(ipaddr);
	retval = mc_join(sock, &mcaddr, (struct in_addr *)&local.sin_addr);
	if (retval <0)
	{
		printf("加入多播组失败,错误号是：%d\n", errno);
	}

	//循环接收组播报文
	printf("准备接收组播数据..........\n");
	while(1)
	{
		memset(Buffer, 0, BUFF_LEN);
		retval = recvfrom(sock, Buffer, BUFF_LEN, 0, (struct sockaddr *)&from, &addrLen);
		if (retval <0) 
		{
			printf("recvfrom失败，错误号是：%d\n", errno);
			break;
		}
		else
		{
			//正常接收
			printf("从 %s端口 %d收到 %d 字节的数据: %s\n", inet_ntoa(from.sin_addr), htons(from.sin_port), retval,Buffer);

			//程序正常退出条件如果是quit，则退出程序
			if(!strncasecmp(Buffer, "quit",4)) 
				break;
		}
	}

	return 0;
}


//本地接口local_if加入多播组mcaddr
int mc_join(int s, struct in_addr *mcaddr, struct in_addr *local_if)
{
	struct ip_mreq mreq;
	memcpy(&(mreq.imr_interface), local_if, sizeof(struct in_addr));	//本地接口
	memcpy(&(mreq.imr_multiaddr), mcaddr, sizeof(struct in_addr));		//多播地址

	return (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)));
}

//本地接口local_if离开多播组mcaddr
int mc_leave(int s, struct in_addr *mcaddr, struct in_addr *local_if)
{
	struct ip_mreq mreq;
	memcpy(&(mreq.imr_interface), local_if, sizeof(struct in_addr));	//本地接口
	memcpy(&(mreq.imr_multiaddr), mcaddr, sizeof(struct in_addr));		//多播地址
	
	return(setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq)));
}

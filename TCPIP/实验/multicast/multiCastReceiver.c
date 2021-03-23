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

int mc_join(int s, struct in_addr *mcaddr, struct in_addr *local_if);  //���϶ಥ��
int mc_leave(int s, struct in_addr *mcaddr, struct in_addr *local_if); //�˳��ಥ��

int main(int argc, char *argv[])
{
	int sock;	//�շ������õ��׽ӿ�
	struct sockaddr_in local, from;
	struct in_addr mcaddr;
	int addrLen;			//��ַ�ṹ����
	unsigned short port = 5006;

	char *ipaddr="226.1.2.3";
	char Buffer[BUFF_LEN];	//�շ�������
	int retval;				//��ʱ����
	
	addrLen = sizeof(struct sockaddr_in);

	//����UDP�׽���
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		printf("socket()ʧ�ܣ�����ţ�%d\n", errno);
		
		return -1;
	}

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = INADDR_ANY;	//����������̫���ӿ�IP
	

	//�󶨱�������IP��˿�
	retval = bind(sock, (struct sockaddr *)&local, sizeof(local));
	if (retval <0)
	{
		printf("bindʧ�ܣ�������ǣ�%d\n", errno);
	}		

	//�����鲥��
	mcaddr.s_addr = inet_addr(ipaddr);
	retval = mc_join(sock, &mcaddr, (struct in_addr *)&local.sin_addr);
	if (retval <0)
	{
		printf("����ಥ��ʧ��,������ǣ�%d\n", errno);
	}

	//ѭ�������鲥����
	printf("׼�������鲥����..........\n");
	while(1)
	{
		memset(Buffer, 0, BUFF_LEN);
		retval = recvfrom(sock, Buffer, BUFF_LEN, 0, (struct sockaddr *)&from, &addrLen);
		if (retval <0) 
		{
			printf("recvfromʧ�ܣ�������ǣ�%d\n", errno);
			break;
		}
		else
		{
			//��������
			printf("�� %s�˿� %d�յ� %d �ֽڵ�����: %s\n", inet_ntoa(from.sin_addr), htons(from.sin_port), retval,Buffer);

			//���������˳����������quit�����˳�����
			if(!strncasecmp(Buffer, "quit",4)) 
				break;
		}
	}

	return 0;
}


//���ؽӿ�local_if����ಥ��mcaddr
int mc_join(int s, struct in_addr *mcaddr, struct in_addr *local_if)
{
	struct ip_mreq mreq;
	memcpy(&(mreq.imr_interface), local_if, sizeof(struct in_addr));	//���ؽӿ�
	memcpy(&(mreq.imr_multiaddr), mcaddr, sizeof(struct in_addr));		//�ಥ��ַ

	return (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)));
}

//���ؽӿ�local_if�뿪�ಥ��mcaddr
int mc_leave(int s, struct in_addr *mcaddr, struct in_addr *local_if)
{
	struct ip_mreq mreq;
	memcpy(&(mreq.imr_interface), local_if, sizeof(struct in_addr));	//���ؽӿ�
	memcpy(&(mreq.imr_multiaddr), mcaddr, sizeof(struct in_addr));		//�ಥ��ַ
	
	return(setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq)));
}

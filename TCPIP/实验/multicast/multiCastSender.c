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
	
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in to;
	char *ipaddr="226.1.2.3";
	unsigned short port = 5006;		//�˿ں�
	char Buffer[BUFF_LEN];	//�շ�������
	int retval;				//��ʱ����
	int addrLen;

	addrLen = sizeof(struct sockaddr_in);

	//����UDP�׽���
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock <0)
	{
		printf("socket()ʧ�ܣ�����ţ�%d\n", errno);
		
		return -1;
	}

	memset(&to, 0, sizeof(to));
	to.sin_addr.s_addr = inet_addr(ipaddr);	//�ಥ��"226.1.2.3"
	to.sin_family = AF_INET;
	to.sin_port = htons(port);

	//ѭ�������鲥����,����"quit"���˳�����
	while(1)
	{
		memset(Buffer, 0, BUFF_LEN);	
		printf("���������鲥����Ϣ:");
		fgets(Buffer,sizeof(Buffer),stdin);
		if(!strlen(Buffer)) //���û��������Ϣ������ѭ��
			continue;

		retval = sendto(sock, Buffer, strlen(Buffer)+1, 0, (struct sockaddr *)&to, sizeof(to));
		if (retval <0)
		{
			printf("����ʧ�ܣ�������ǣ�%d", errno);
			break;
		}
		else
		{
			printf("�ѷ��� %d�ֽڵ���Ϣ��%s\n\n", retval, Buffer);
			//���������˳�����
			if(!strncasecmp(Buffer, "quit",4)) 
				break;
		}
	}
	
	close(sock);
	
	return 0;
}

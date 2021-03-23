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

int main(void)
{
	struct sockaddr_in local, from;	//�׽��ֵ�ַ�ṹ����
	int socket_type = SOCK_DGRAM;	//UDP�׽���
	unsigned short port= 5005;		//�˿ں�
	int sock;					//�׽��ֱ���
	int fromlen;			//��ַ�ṹ����

	char Buffer[BUFF_LEN];			//�շ�������
	int retval;				//��ʱ����

	//�����׽���
	sock = socket(AF_INET, socket_type, 0); 
	if (sock <0)
	{
		printf("socket()ʧ�ܣ�����ţ�%d\n", errno);
		
		return -1;
	}

	//����������ַ��Ϣ��sockaddr_in�ṹ��
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY; 
	local.sin_port = htons(port);
	fromlen =sizeof(from);

	//������ַ���׽���
	if (bind(sock,(struct sockaddr*)&local, sizeof(local) ) <0) 
	{
		printf("bind()ʧ�ܣ�����ţ�%d\n", errno);
		close(sock);
		
		return -1;
	}		

	//ѭ�������뷢������
	puts("�ȴ����ݵ���.............");
	while(1)
	{
		//�ȴ�����������Ϣ
		retval = recvfrom(sock, Buffer, sizeof (Buffer), 0,	(struct sockaddr *)&from, &fromlen);
		//����ж�
		if (retval <0) 
		{
			printf("recv()ʧ�ܣ�����ţ�%d\n", errno);
			break;
		}
		else //��������
		{
			printf("��%s�Ķ˿�%d�յ� %d �ֽڵ�����:%s\n", inet_ntoa(from.sin_addr), htons(from.sin_port), retval,Buffer);
			//�����һ���˳�����
			if(!strncasecmp(Buffer, "quit",4)) 
				break;
		}
	}

	 close(sock);
	
	return 0;
}

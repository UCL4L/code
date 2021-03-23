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
	int sock;	//�շ������õ��׽ӿ�
	struct sockaddr_in sendtoAdrr;
	unsigned short port = 5005;	//�˿ں�
	int sendtoAdrrLen, fromAddrLen;	//��ַ�ṹ����

	char Buffer[BUFF_LEN];	//�շ�������
	int retval = 0;
	int bBroadCast = 1;	//���ÿɹ㲥�׽ӿ�
	int optLen;				//�㲥ѡ��ֵ����

	//����UDP�׽���
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock <0 )
	{
		printf("socket()ʧ�ܣ�����ţ�%d\n", errno);
		
		return -1;
	}

	//�����׽ӿ�Ϊ����㲥��
	optLen = sizeof(bBroadCast);
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bBroadCast, optLen) <0)
	{
		printf("�����׽ӿ�Ϊ����㲥ʧ��! �����%d.\n",errno);
		close(sock);
		
		return -1;
	}

	//�����Ƿ��ѳɹ����׽ӿ�����Ϊ����㲥,���Ǳ�Ҫ��
	if (getsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bBroadCast, &optLen) <0)
	{
		printf("�����׽ӿڵĺ���getsockoptʧ��!....\n");
		close(sock);
		
		return -1;
	}

	if (!bBroadCast)
	{
		printf("�����飬�����׽ӿ�����㲥ʧ�ܣ� \n");
	}

	//����Ŀ��������ַ��Ϣ��sendtoAdrr
	memset(&sendtoAdrr, 0, sizeof(sendtoAdrr));
	sendtoAdrr.sin_family = AF_INET;
	sendtoAdrr.sin_addr.s_addr = INADDR_BROADCAST;	//"255.255.255.255" 
	sendtoAdrr.sin_port = htons(port);

	sendtoAdrrLen = sizeof(sendtoAdrr);

	//ѭ�����ͱ���
	while(1)
	{
		memset(Buffer, 0, BUFF_LEN);	
		puts("���������㲥����Ϣ:");
		fgets(Buffer,sizeof(Buffer),stdin);
		if(!strlen(Buffer)) //���û��������Ϣ������ѭ��
			continue;
		
		//���͹㲥����
		retval = sendto(sock, Buffer, strlen(Buffer)+1, 0, (struct sockaddr *)&sendtoAdrr, sendtoAdrrLen);
		if (retval <0)
		{
			printf("���͹㲥����ʧ��! �������ǣ� %d\n", errno);
			break;
		}
		else	//��������
		{
			printf("�ѷ��͹㲥���ģ� %s.......\n", Buffer);
			//�����һ���˳�����
			if(!strncasecmp(Buffer, "quit",4)) 
				break;
		}
	}

	close(sock);
	
	return 0;
}



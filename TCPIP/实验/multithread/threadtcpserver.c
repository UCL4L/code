#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include </usr/include/errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE	128
extern int	errno;
int quitag = 0;

void *Handle(void *param)
{
	int clientsock =  (int) param;
	int retval;
	char buf[BUFFER_SIZE];

	while(1)
	{
		//����������Ϣ
		memset(buf, 0 , BUFFER_SIZE);
		retval = recv(clientsock, buf, BUFFER_SIZE, 0 );//����tcp������BSD read()

		if (retval < 0) 
		{
			printf("������Ϣʧ�ܣ�����ţ�%d\n",strerror(errno));
			break;
		}
		else if (retval == 0) // �����������ӵ��׽��֣���Ҫ�ж϶Է��Ƿ�ر�����
		{
			printf("TCP�����Ѿ��ر�\n");
			break;
		}
		else //��ȷ���յ�������Ϣ
		{
			printf("(�յ���Ϣ)socket = %d����Ϣ��%s \n",clientsock, buf);
			send(clientsock,buf,retval, 0);
			if(!strncasecmp(buf, "exit",4))
				break;
			else if(!strncasecmp(buf, "quit",4))
			{
				quitag=1;
				break;
			}
		}
	}
	close(clientsock);
	printf("����socket=%d�߳̽���!", clientsock);
}

void ErrorPro(char *msg)
{
	printf("%dʧ�ܣ�����ţ�%d\n",msg, strerror(errno));
	exit(-1);
}

int main(void)
{
	unsigned short port= 5001;		//�˿ں�
	int socket_type = SOCK_STREAM;	//ȱʡʹ��TCP����
	int retval, fromlen;
	struct sockaddr_in local, from;	//�׽��ֵ�ַ�ṹ����
	int listen_socket, msgsock;	//�׽��ֱ���
	pthread_t ntid;
 
	//�����׽���
	listen_socket = socket(AF_INET, socket_type, 0); 
	if (listen_socket < 0)
	{
		ErrorPro("socket");
	}

	// ���� ��������Ϣ��sockaddr_in�ṹ��
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY; 
	local.sin_port = htons(port);
	//�������ַ���׽���
	if (bind(listen_socket,(struct sockaddr*)&local, sizeof(local) ) < 0) 
	{
		ErrorPro("bind");
	}

	//������ʽ�׽��֣����׽��ִ��ڼ���״̬���ȴ����ӵ���
	if (listen(listen_socket, 5) < 0) 
	{
		ErrorPro("listen");
	}

	printf("�����˿ڣ�%d, ʹ��TCP\n", port);
	fromlen =sizeof(from); 

	while(1)
	{
		msgsock = accept(listen_socket,(struct sockaddr*)&from, &fromlen);
		if (msgsock < 0) 
		{
			ErrorPro("accept");
		}
		printf("��%s�յ�����, �˿��ǣ�%d\n", inet_ntoa(from.sin_addr), htons(from.sin_port));
			//����ר���߳̽��н��մ���

		if(quitag)
			break;

		retval = pthread_create(&ntid, NULL, Handle, (void *)msgsock);
    	if (retval != 0)
		{
			printf("�޷������̣߳�ԭ����: %s\n", strerror(retval));    
			break;
		}    		

	}

	close(msgsock);
	close(listen_socket);
	
	return 0;
}

//gcc threadtcpserver.c -lpthread -o ttcps

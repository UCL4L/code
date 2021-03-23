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
#include <sys/time.h>

#define ICMP_ECHO	8 //ICMP��������
#define ICMP_ECHOREPLY 0 //ICMP����Ӧ��
#define ICMP_MIN	8 //ICMP���ݰ����Ϊ8���ֽ�
#define DEF_PACKET_SIZE    32   //Ĭ�����ݰ�����
#define DEF_PACKET_NUMBER  4   //Ĭ�Ϸ���ICMP����Ĵ���
#define MAX_PACKET	1024        //���ݰ���󳤶�

//����IPͷ�� 
typedef struct iphdr
{ 
	unsigned int h_len:4; // ͷ���� 
	unsigned int version:4; // �汾��
	unsigned char tos; // �������� 
	unsigned short total_len; // �ܳ���
	unsigned short ident; // ��ʶ 
	unsigned short frag_and_flags; //��־
	unsigned char ttl; //����ʱ��
	unsigned char proto; // �ϲ�Э�� 
	unsigned short checksum; // У���
	unsigned int sourceIP; //ԴIP
	unsigned int destIP; //Ŀ��IP
}IpHeader; 
 
// ����ICMP ͷ�� 
typedef struct icmphdr 
{ 
	unsigned char i_type; //����
	unsigned char i_code; //����
	unsigned short i_cksum; //У���
	unsigned short i_id; //��ʶ
	unsigned short i_seq; //���к�
	unsigned long timestamp; //����
}IcmpHeader; 


void fill_icmp_data(char *, int); //���icmp���ݰ�
unsigned short checksum(unsigned short *, int); //����У���
int decode_resp(char *,int ,struct sockaddr_in *); //�յ����ݺ����

unsigned long GetTickCount(void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000 + tv.tv_usec/1000;  //����
}

void Usage(char *progname)//��ʾ�û��ó���ʹ�÷���
{ 
	printf("Usage:\n"); 
	printf("%s target [number of packets] [data_size]\n",progname); 
	printf("datasize can be up to 1Kb\n"); 
} 


void main(int argc, char **argv)
{ 

	int sockRaw;  //ԭʼ�׽���
	struct sockaddr_in dest,from; //Դ��Ŀ��IP��ַ
	struct hostent * hp; //ָ��ָ���������������ַ�б����Ϣ�Ľṹ��
	int iRecv,iSend, datasize,times; 
	int fromlen = sizeof(from); 

	struct timeval timeout;  //��ʱʱ��1000ms=1s
	int statistic = 0;  // ����ͳ��  
	char *dest_ip; 
	char *icmp_data; 
	char *recvbuf; 
	unsigned int addr=0; 
	unsigned short	seq_no = 0; 
	int		i;
	
	//ʹ�÷�������ʱ��ʾ��ʾ��Ϣ
	if (argc <2 ) 
	{ 
		Usage(argv[0]); 
		return;
	} 

	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	//����ԭʼ�׽���
	sockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	// ����ԭʼ�׽��ֲ��ɹ�
	if (sockRaw < 0)
	{ 
		printf("socket(��ʧ�ܣ�����%s\n",strerror(errno)); 
		return; 
	} 

	//�趨���ܳ�ʱʱ��
	iRecv = setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)); 
	if(iRecv < 0)
	{ 
		printf("failed to set recv timeout: %s\n",strerror(errno)); 
		return; 
	} 

	//�趨�������ݳ�ʱʱ��
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	iRecv = setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)); 
	if(iRecv < 0) { 
		printf("failed to set send timeout: %s\n",strerror(errno)); 
		return; 
	} 

	
	memset(&dest,0,sizeof(dest)); 

	//�����û������Ŀ���ַ
	hp = gethostbyname(argv[1]); 
	if (!hp)
	{ 
		addr = inet_addr(argv[1]); 
	} 

	//�Ƿ�����
	if ((!hp) && (addr == INADDR_NONE))
	{ 
		printf("Unable to resolve %s\n",argv[1]); 
		return; 
	} 

	//��¼Ŀ��������Ϣ�Ľṹ��
	//��ַ
	if (hp != NULL) 
		memcpy(&(dest.sin_addr),hp->h_addr,hp->h_length); 
	else 
		dest.sin_addr.s_addr = addr; 

	//Э����
	if (hp) 
		dest.sin_family = hp->h_addrtype; 
	else 
		dest.sin_family = AF_INET; 


	//Ŀ��IP
	dest_ip = inet_ntoa(dest.sin_addr); 
		

	//����Ŀ���ַ����������Ping�Ĵ���
	if(argc>2)
	{
		times=atoi(argv[2]);
		if(times == 0)
			 times = DEF_PACKET_NUMBER;
	}
	else
		times = DEF_PACKET_NUMBER;
	
	//�����������ݴ�С
	if (argc >3) 
	{ 
		datasize = atoi(argv[3]); 

		//������0������Ĭ�����ݰ���С
		if (datasize == 0) 
			datasize = DEF_PACKET_SIZE;

		//�û����������ݰ���С̫��
		if (datasize >1024)    
		{
			printf("WARNING : data_size is too large !\n");
			datasize = DEF_PACKET_SIZE; 
		}
	} 
	else 
		datasize = DEF_PACKET_SIZE; 
	
	datasize += sizeof(IcmpHeader); 

	icmp_data = (char *)malloc(MAX_PACKET); 
	recvbuf = (char *)malloc(MAX_PACKET); 
	
	if (!icmp_data)
	{ 
		printf("HeapAlloc failed %d\n",errno); 
		return; 
	} 
	
	memset(icmp_data, 0, MAX_PACKET); 

	//���ICMP���ݰ������͡����롢��ʶ��
	fill_icmp_data(icmp_data,datasize); 

	//��ʾ����pingĿ������
	printf("\nPinging %s ....\n\n",dest_ip);

	//Ping���
	for(i=0; i<times; i++)
	{ 
		//׼��ICMP��ͷ������
		((IcmpHeader *)icmp_data)->i_cksum = 0; 
		//ȡ���Ժ���Ϊ��λ�ļ��������������ʱ����
		((IcmpHeader *)icmp_data)->timestamp = GetTickCount(); 	
		((IcmpHeader *)icmp_data)->i_seq = seq_no++; //���кŵ���
		((IcmpHeader *)icmp_data)->i_cksum = checksum((unsigned short*)icmp_data,datasize);//����У���

		//����ICMP���ݰ�
		iSend = sendto(sockRaw,icmp_data,datasize,0,(struct sockaddr*)&dest,sizeof(dest)); 
		
		//����ʧ��
		if (iSend < 0)
		{ 
			if (errno == EAGAIN||errno ==EWOULDBLOCK)  
			{ 
				printf("Request timed out.\n"); 
				continue; 
			} 
			printf("sendto failed: %s\n",strerror(errno)); 
			break; 
		} 


		if (iSend < datasize )
		{ 
			printf("Only sent  %d bytes\n",iSend); 
		} 


		//����Ӧ������
		iRecv = recvfrom(sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr*)&from, &fromlen); 
		
		//����ʧ��
		if (iRecv < 0)
		{ 
			if (errno == EAGAIN||errno ==EWOULDBLOCK) 
			{ 
				printf("recieve timed out.\n"); 
				continue; 
			} 
			printf("recvfrom failed: %s\n",strerror(errno)); 
			break; 
		} 

		//�ɹ�����
		if(!decode_resp(recvbuf,iRecv,&from))
			statistic++; //��¼�ɹ�������Ӧ���ݰ��Ĵ���
		
		sleep(2); 
	}

	//ͳ������Ping�����ͳ�ƽ��
	printf("\nPing statistics for %s \n",dest_ip);
	printf("    Packets: Sent = %d,Received = %d, Lost = %d (%2.0f%% loss)\n",times,
			statistic,(times-statistic), (float)(times-statistic)/times*100);

	free(recvbuf);
	free(icmp_data);

	close(sockRaw);

	return; 
} 


//�յ���ӦIP���ݰ��󣬶�����н���
int decode_resp(char *buf, int bytes,struct sockaddr_in *from) 
{ 
	IpHeader *iphdr; 
	IcmpHeader *icmphdr; 
	unsigned short iphdrlen; 
	
	iphdr = (IpHeader *)buf; 
	iphdrlen = (iphdr->h_len) * 4 ; //ͷ��ռ�������ֽ� 
	
	if (bytes < iphdrlen + ICMP_MIN)
	{ 
		printf("Too few bytes from %s\n",inet_ntoa(from->sin_addr)); 
	} 

	//�ҵ�ICMP���ݰ���ʼ�ĵط�
	icmphdr = (IcmpHeader*)(buf + iphdrlen); 
	if (icmphdr->i_type != ICMP_ECHOREPLY)
	{ 
		printf("non-echo type %d recvd\n",icmphdr->i_type); 
		return 1; 
	} 

	//�ǲ��Ƿ�������������ݰ�
	if (icmphdr->i_id != (unsigned short)getpid()) 
	{ 
		printf("someone else''s packet!\n"); 
		return 1; 
	} 

	printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr)); 
	printf(" icmp_seq = %d. ",icmphdr->i_seq); 
	printf(" time: %d ms ", GetTickCount()-icmphdr->timestamp); //���͵����չ��̵ľ�����ʱ��
	printf("\n");
	return 0; 
} 

//����У���
unsigned short checksum(unsigned short *buffer, int size) 
{ 
	unsigned long cksum=0; 
	
	while(size >1) { 
		cksum+=*buffer++; 
		size -=sizeof(unsigned short); 
	} 
	if(size) { 
		cksum += *(unsigned char *)buffer; 
	} 
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	return (unsigned short)(~cksum); 
} 

//���ICMP���ݰ�
void fill_icmp_data(char * icmp_data, int datasize)
{ 
	IcmpHeader *icmp_hdr; 
	char *datapart; 
	
	icmp_hdr = (IcmpHeader *)icmp_data; 
	icmp_hdr->i_type = ICMP_ECHO; 
	icmp_hdr->i_code = 0; 
	icmp_hdr->i_id = (unsigned short)getpid(); 
	icmp_hdr->i_cksum = 0; 
	icmp_hdr->i_seq = 0; 
	datapart = icmp_data + sizeof(IcmpHeader); 

	//������������
	memset(datapart,17, datasize - sizeof(IcmpHeader)); 
}

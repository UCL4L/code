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

#define ICMP_ECHO	8 //ICMP回显请求
#define ICMP_ECHOREPLY 0 //ICMP回显应答
#define ICMP_MIN	8 //ICMP数据包最短为8个字节
#define DEF_PACKET_SIZE    32   //默认数据包长度
#define DEF_PACKET_NUMBER  4   //默认发送ICMP请求的次数
#define MAX_PACKET	1024        //数据包最大长度

//定义IP头部 
typedef struct iphdr
{ 
	unsigned int h_len:4; // 头部长 
	unsigned int version:4; // 版本号
	unsigned char tos; // 服务类型 
	unsigned short total_len; // 总长度
	unsigned short ident; // 标识 
	unsigned short frag_and_flags; //标志
	unsigned char ttl; //生存时间
	unsigned char proto; // 上层协议 
	unsigned short checksum; // 校验和
	unsigned int sourceIP; //源IP
	unsigned int destIP; //目的IP
}IpHeader; 
 
// 定义ICMP 头部 
typedef struct icmphdr 
{ 
	unsigned char i_type; //类型
	unsigned char i_code; //代码
	unsigned short i_cksum; //校验和
	unsigned short i_id; //标识
	unsigned short i_seq; //序列号
	unsigned long timestamp; //数据
}IcmpHeader; 


void fill_icmp_data(char *, int); //填充icmp数据包
unsigned short checksum(unsigned short *, int); //计算校验和
int decode_resp(char *,int ,struct sockaddr_in *); //收到数据后解码

unsigned long GetTickCount(void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000 + tv.tv_usec/1000;  //毫秒
}

void Usage(char *progname)//提示用户该程序使用方法
{ 
	printf("Usage:\n"); 
	printf("%s target [number of packets] [data_size]\n",progname); 
	printf("datasize can be up to 1Kb\n"); 
} 


void main(int argc, char **argv)
{ 

	int sockRaw;  //原始套接字
	struct sockaddr_in dest,from; //源、目的IP地址
	struct hostent * hp; //指针指向包含主机名、地址列表等信息的结构体
	int iRecv,iSend, datasize,times; 
	int fromlen = sizeof(from); 

	struct timeval timeout;  //超时时间1000ms=1s
	int statistic = 0;  // 用于统计  
	char *dest_ip; 
	char *icmp_data; 
	char *recvbuf; 
	unsigned int addr=0; 
	unsigned short	seq_no = 0; 
	int		i;
	
	//使用方法不对时显示提示信息
	if (argc <2 ) 
	{ 
		Usage(argv[0]); 
		return;
	} 

	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	//创建原始套接字
	sockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	// 创建原始套接字不成功
	if (sockRaw < 0)
	{ 
		printf("socket(）失败，错误：%s\n",strerror(errno)); 
		return; 
	} 

	//设定接受超时时间
	iRecv = setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)); 
	if(iRecv < 0)
	{ 
		printf("failed to set recv timeout: %s\n",strerror(errno)); 
		return; 
	} 

	//设定发送数据超时时间
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	iRecv = setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)); 
	if(iRecv < 0) { 
		printf("failed to set send timeout: %s\n",strerror(errno)); 
		return; 
	} 

	
	memset(&dest,0,sizeof(dest)); 

	//解析用户输入的目标地址
	hp = gethostbyname(argv[1]); 
	if (!hp)
	{ 
		addr = inet_addr(argv[1]); 
	} 

	//非法输入
	if ((!hp) && (addr == INADDR_NONE))
	{ 
		printf("Unable to resolve %s\n",argv[1]); 
		return; 
	} 

	//记录目标主机信息的结构体
	//地址
	if (hp != NULL) 
		memcpy(&(dest.sin_addr),hp->h_addr,hp->h_length); 
	else 
		dest.sin_addr.s_addr = addr; 

	//协议族
	if (hp) 
		dest.sin_family = hp->h_addrtype; 
	else 
		dest.sin_family = AF_INET; 


	//目标IP
	dest_ip = inet_ntoa(dest.sin_addr); 
		

	//除了目标地址，还给出了Ping的次数
	if(argc>2)
	{
		times=atoi(argv[2]);
		if(times == 0)
			 times = DEF_PACKET_NUMBER;
	}
	else
		times = DEF_PACKET_NUMBER;
	
	//还给出了数据大小
	if (argc >3) 
	{ 
		datasize = atoi(argv[3]); 

		//给的是0，则用默认数据包大小
		if (datasize == 0) 
			datasize = DEF_PACKET_SIZE;

		//用户给出的数据包大小太大
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

	//填充ICMP数据包，类型、代码、标识等
	fill_icmp_data(icmp_data,datasize); 

	//提示正在ping目标主机
	printf("\nPinging %s ....\n\n",dest_ip);

	//Ping多次
	for(i=0; i<times; i++)
	{ 
		//准备ICMP包头部数据
		((IcmpHeader *)icmp_data)->i_cksum = 0; 
		//取得以毫秒为单位的计算机启动后经历的时间间隔
		((IcmpHeader *)icmp_data)->timestamp = GetTickCount(); 	
		((IcmpHeader *)icmp_data)->i_seq = seq_no++; //序列号递增
		((IcmpHeader *)icmp_data)->i_cksum = checksum((unsigned short*)icmp_data,datasize);//计算校验和

		//发送ICMP数据包
		iSend = sendto(sockRaw,icmp_data,datasize,0,(struct sockaddr*)&dest,sizeof(dest)); 
		
		//发送失败
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


		//接收应答数据
		iRecv = recvfrom(sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr*)&from, &fromlen); 
		
		//接收失败
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

		//成功解码
		if(!decode_resp(recvbuf,iRecv,&from))
			statistic++; //记录成功接收响应数据包的次数
		
		sleep(2); 
	}

	//统计运行Ping命令的统计结果
	printf("\nPing statistics for %s \n",dest_ip);
	printf("    Packets: Sent = %d,Received = %d, Lost = %d (%2.0f%% loss)\n",times,
			statistic,(times-statistic), (float)(times-statistic)/times*100);

	free(recvbuf);
	free(icmp_data);

	close(sockRaw);

	return; 
} 


//收到响应IP数据包后，对其进行解码
int decode_resp(char *buf, int bytes,struct sockaddr_in *from) 
{ 
	IpHeader *iphdr; 
	IcmpHeader *icmphdr; 
	unsigned short iphdrlen; 
	
	iphdr = (IpHeader *)buf; 
	iphdrlen = (iphdr->h_len) * 4 ; //头部占几个节字节 
	
	if (bytes < iphdrlen + ICMP_MIN)
	{ 
		printf("Too few bytes from %s\n",inet_ntoa(from->sin_addr)); 
	} 

	//找到ICMP数据包开始的地方
	icmphdr = (IcmpHeader*)(buf + iphdrlen); 
	if (icmphdr->i_type != ICMP_ECHOREPLY)
	{ 
		printf("non-echo type %d recvd\n",icmphdr->i_type); 
		return 1; 
	} 

	//是不是发给本程序的数据包
	if (icmphdr->i_id != (unsigned short)getpid()) 
	{ 
		printf("someone else''s packet!\n"); 
		return 1; 
	} 

	printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr)); 
	printf(" icmp_seq = %d. ",icmphdr->i_seq); 
	printf(" time: %d ms ", GetTickCount()-icmphdr->timestamp); //发送到接收过程的经历的时间
	printf("\n");
	return 0; 
} 

//计算校验和
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

//填充ICMP数据包
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

	//数据区随便填充
	memset(datapart,17, datasize - sizeof(IcmpHeader)); 
}

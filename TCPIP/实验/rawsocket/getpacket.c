#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if_arp.h>
#include <linux/ip.h>
#include <linux/sockios.h>
#include <unistd.h>

int get_nic_index(int fd, const char* nic_name)
{
    struct ifreq ifr;
    
    if (nic_name == NULL)
           return -1;
           
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, nic_name, IFNAMSIZ);
    
    if (ioctl(fd, SIOCGIFINDEX, &ifr) == -1) {
        puts("SIOCGIFINDEX ioctl error\n");
        return -1;
    	}
    return ifr.ifr_ifindex;
}

void showpkt(char *buf, int len)
{
	int i,col1=8,col2=16;
	for(i=0;i<len;i++)
	{
		printf("%02X ",(unsigned char )buf[i]);
		col1--;
		if(col1 == 0)
		{
			printf("  ");
			col1 = 8;
		}
		
		col2--;
		if(col2 == 0)
		{
			col2=16;
			printf("\n");
		}
	}
	printf("\n");
}

int set_nic_promisc(int sockfd, const char *nic_name)
{
    struct ifreq ethreq;
    strncpy(ethreq.ifr_name, nic_name, IFNAMSIZ);
    ioctl(sockfd, SIOCGIFFLAGS, &ethreq);
    ethreq.ifr_flags |= IFF_PROMISC;
    ioctl(sockfd, SIOCSIFFLAGS, &ethreq);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("usage: %s cardname pktnum\n",argv[0]);
        exit(1);
		}

    int pktfd;
    int maxpak = 0, ipak = 0;
    int fromlen;
    char buffer[BUFSIZ];
    struct iphdr *iph;
    struct sockaddr_ll sll;

    /* Low level socket */
    pktfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (pktfd == -1) {
        puts("Unable to create low level socket");
        return -1;
		}

    memset(&sll, 0, sizeof(sll));
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = get_nic_index(pktfd, argv[1]);

    if (bind(pktfd, (struct sockaddr *) &sll, sizeof(sll)) != 0) {
        printf("bind error");
        goto FAIL;
    	}
    	
    set_nic_promisc(pktfd, argv[1]);
    maxpak = atoi(argv[2]);
    
    while (!maxpak || ipak < maxpak) {
        fromlen = recv(pktfd, buffer, BUFSIZ, MSG_TRUNC);
        printf("\nNew Packet Length: %d bytes, Content:\n", fromlen);
        showpkt(buffer,fromlen);
        
        struct ethhdr *ethdr = (struct ethhdr *) buffer;
        if ( ethdr->h_proto == htons(ETH_P_IP) ) {
            iph = (struct iphdr *) (buffer + sizeof(struct ethhdr));
            printf("IP Packet from: %s\n", inet_ntoa(*(struct in_addr*)&iph->saddr));
            printf("IP Packet To: %s\n", inet_ntoa(*(struct in_addr*)&iph->daddr));
            printf("Upper Protocol: %#x\n", iph->protocol);
            ipak++;
        	}
    	}

    return 0;

FAIL:
    close(pktfd);
    return -1;
}

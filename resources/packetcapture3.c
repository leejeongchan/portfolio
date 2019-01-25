/*
 * header file define
 */
/*
 * 처음 시작 시 ip 헤더와 tcp 헤더가 출력 되는 부분은 정상입니다. 각 프로토콜 입력시 ip 와 tcp가 나오다가 해당 프로토콜 헤더가 나오게됩니다.
 * 2014154042 hwang in seung
 * 2014154031 lee jeong chan
 * 2014152016 shin je woo
 * Computer Network Term Project
 * application protocol: http/dns/email/p2p
 * raw socket programming
 * computer engineering of Korea Polytechnic University
 */
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen

#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h> //promisk set
#include<sys/time.h>
#include<sys/types.h>
#include<ctype.h>
#include<unistd.h>

//DNS header define
struct __attribute__((packed)) dnshdr {
	unsigned short id;
	unsigned short flags;
	unsigned short questions;
	unsigned short answer_RRs;
	unsigned short authority_RRs;
	unsigned short additional_RRs;
	unsigned char query;
};

//SMTP header define
/*struct __attribute__((packed)) smtphdr{
  unsigned char to;
  unsigned char from;
  unsigned char subject;
  };*/


int total=0;
int main(int argc,char *argv[])
{
	int sock;//socket descriptor
	//struct ifreq ifr;
	struct sockaddr saddr;
	char *interface="eth0";
	int saddr_size,data_length;//socket size, data size
	struct iphdr* ip;//ip header
	unsigned short iphdr_len;

	struct tcphdr * tcp; // tcp header pointer
	int tcphdr_len; // tcp header size

	struct udphdr * udp; // udp header pointer
	int udphdr_len; // udp header size


	struct sockaddr_in source; // source ip
	struct sockaddr_in destination; // destination ip 

	unsigned char *http_data; //http header pointer(string)
	int http_len;//http header length
	int out_len;//http output length

	struct dnshdr *dns;//dns header pointer
	int dnshdr_len;//dns header length

	unsigned char *smtp_data;
	int smtp_len;
	int index=0;

	unsigned char *imap_data;
	int imap_len;

	unsigned char *p2p_data;
	int p2p_len;

	unsigned char *p2p_udp_data;
	int p2p_udp_len;

	unsigned char * ssh; // ssh 헤더 포인터
   	int ssh_len; // ssh 헤더 사이즈
	int print_len;
	unsigned char *buf=(unsigned char*)malloc(65536); //buffer

	// unsigned char *buf=(unsigned char*)calloc(65536,1); //buffer
	printf("-----------선택 하신 프로토콜:%s---------------\n",argv[2]);
	printf("-----------my ip addr:%s-----------------------\n",argv[1]);
	printf("Attention:처음 시작 시 ip 헤더와 tcp 헤더가 출력 되는 부분은 정상입니다.\n 각 프로토콜 입력시 ip 와 tcp가 나오다가 해당 프로토콜 헤더가 나오게됩니다.\n");


	sock=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));  // AF_PACKET     
	if(sock==-1)
	{
		perror("sock error\n");
		return 1;
	}
	//promisc set up
	/*strncpy(ifr.ifr_name,interface,strlen(interface)+1);
	  if(ioctl(sock,SIOCGIFFLAGS,&ifr)<0){
	  perror("SIOCGIFFLAGS error : ");
	  exit(1);
	  }
	  ifr.ifr_flags |=IFF_PROMISC;

	  if(ioctl(sock,SIOCSIFFLAGS,&ifr)<0)
	  {
	  perror("could not set the promisc flag :");
	  exit(1);
	  }*/
	printf("capture start\n");
	while(1)
	{
		//packet capture start
		saddr_size=sizeof(saddr);   //data recvfrom
		data_length=recvfrom(sock,buf,65536,0,&saddr,(socklen_t*)&saddr_size);
		if(data_length<0)
		{
			printf("RecvFrom Erro, failed to get packets\n");
			return 1;
		}

		ip=(struct iphdr*)(buf+sizeof(struct ethhdr)); //ipheader
		iphdr_len=ip->ihl * 4; //ipheader length

		memset(&source,0,sizeof(source));
		source.sin_addr.s_addr=ip->saddr; //ip source get
		//printf("my ip addr:%s\n",inet_ntoa(source.sin_addr));

		memset(&destination,0,sizeof(destination)); // ip destination get
		destination.sin_addr.s_addr=ip->daddr;
		//printf("dest ip addr:%s\n",inet_ntoa(destination.sin_addr));
		//filter my address

		if(strcmp(argv[1],inet_ntoa(source.sin_addr)) && strcmp(argv[1],inet_ntoa(destination.sin_addr)))
		{
			//printf("not my ip!!\n");
			continue;
		}
		//TCP
		if((!strcmp(argv[2],"http")|| !strcmp(argv[2],"smtp")|| !strcmp(argv[2],"p2ptcp")|| !strcmp(argv[2],"ssh")|| !strcmp(argv[2],"imap")) && 6==ip->protocol)    // http smtp p2ptcp ssh
		{
			//tcp header
			tcp=(struct tcphdr*)(buf+iphdr_len+sizeof(struct ethhdr));
			//tcp header length
			tcphdr_len=sizeof(struct ethhdr)+iphdr_len+tcp->doff*4;
			//if not http, continue
			if(80!=ntohs(tcp->source) &&80!= ntohs(tcp->dest)&&22!=ntohs(tcp->source)&&22!=ntohs(tcp->dest) && 465!=ntohs(tcp->source) && 465!=ntohs(tcp->dest) && 993!=ntohs(tcp->source) &&993!=ntohs(tcp->dest)&&49152>ntohs(tcp->source)&&49152>ntohs(tcp->dest))
			{
				continue;
			}


		}
		else if((!strcmp(argv[2],"dns")|| !strcmp(argv[2],"p2ptracker")) &&17==ip->protocol)
		{
			//udp header
			udp=(struct udphdr*)(buf+iphdr_len+sizeof(struct ethhdr));
			//udp header lenght
			udphdr_len=sizeof(struct ethhdr) + iphdr_len+8;
			//if not dns, continue
			if(ntohs(udp->source)!=53 && ntohs(udp->dest)!=53&& ntohs(udp->source)!=8999 && ntohs(udp->dest)!=8999)
			{
				continue;
			}
		}else{

			//printf("else enter\n");
			continue;
		}
		//IP header print
		sleep(1);
		printf("IP Header\n");

		printf(" + IP Version          : %d\n", (unsigned int)ip->version);

		printf(" | IP Header Length    : %d Bytes\n", ((unsigned int)(ip->ihl))*4);

		printf(" | Type Of Service     : %d\n", (unsigned int)ip->tos);

		printf(" | IP Total Length     : %d  Bytes (FULL SIZE)\n", ntohs(ip->tot_len));

		printf(" | TTL                 : %d\n", (unsigned int)ip->ttl);

		printf(" | Protocol            : %d\n", (unsigned int)ip->protocol);

		printf(" | Checksum            : %d\n", ntohs(ip->check));

		printf(" | Source IP           : %s\n", inet_ntoa(source.sin_addr));

		printf(" + Destination IP      : %s\n", inet_ntoa(destination.sin_addr));
		printf("======================================================\n");


		//TCP header print
		if(6==ip->protocol)
		{
			printf("TCP Header\n");

			printf(" + Source Port          : %u\n", ntohs(tcp->source));

			printf(" | Destination Port     : %u\n", ntohs(tcp->dest));

			printf(" | Sequence Number      : %u\n", ntohl(tcp->seq));

			printf(" | Acknowledge Number   : %u\n", ntohl(tcp->ack_seq));

			printf(" | Header Length        : %d BYTES\n", (unsigned int) tcp->doff * 4);

			printf(" | URG Flag             : %d\n", (unsigned int) tcp->urg);

			printf(" | PSH Flag             : %d\n", (unsigned int) tcp->psh);

			printf(" | RST Flag             : %d\n", (unsigned int) tcp->rst);

			printf(" | SYN Flag             : %d\n", (unsigned int) tcp->syn);

			printf(" | ACK Flag             : %d\n", (unsigned int) tcp->ack);

			printf(" | Finish Flag          : %d\n", (unsigned int) tcp->fin);

			printf(" | receive Window Size  : %d\n", ntohs(tcp->window));

			printf(" + Checksum             : %d\n", ntohs(tcp->check));
			if((unsigned int)tcp->syn==1){
				printf("3hand shake start\n");
			}
			if((unsigned int)tcp->ack==1 && (unsigned int)tcp->syn==1)
				printf("server send ack and syn\n");
			if((unsigned int)tcp->ack==1){
				printf("client send ack to server\n");
				printf("connection complete \n");
			}

			printf("==============================================\n");
		}

		//UDP header print
		else if(17==ip->protocol)
		{
			printf("\nUDP Header\n");

			printf(" + Source Port      : %d\n", ntohs(udp->source));

			printf(" | Destination Port : %d\n", ntohs(udp->dest));

			printf(" | UDP Length       : %d\n", ntohs(udp->len));

			printf(" + UDP Checksum     : %d\n", ntohs(udp->check));
			printf("============================================\n");
		}
		else
			continue;

		//http header print(TCP)
		if(6==ip->protocol ){
			//http
			if(!strcmp(argv[2],"http"))
			{
				if(80==ntohs(tcp->source) || 80==ntohs(tcp->dest)){
				printf("Http enter\n");
				int cnt=0;
				http_data=buf+tcphdr_len;
				http_len=data_length-tcphdr_len;

				printf("Http Header\n");
				while(isprint(*http_data)|| isspace(*http_data)){
					printf("%c",*http_data);
					http_data++;
				}

				printf("\n==============================================\n");
				}
			}
			//smtp email send
			else if(!strcmp(argv[2],"smtp"))
			{
				if(465==ntohs(tcp->source) ||465==ntohs(tcp->dest)){
				printf("smtp enter\n");
				int cnt=0;
				int i;
				//smtp=(struct smtphdr*)(buf+udphdr_len); //smtp header pointer
				smtp_data=buf+tcphdr_len;
				smtp_len=data_length-tcphdr_len;
				printf("[1]:%d \n",smtp_len);
				printf("Smtp Header\n");
				for(i=0; i<smtp_len; i++){
					index++;
					//if(cnt==15)
					//{
					//	printf("\n");
					//	cnt=0;
					//}
					fprintf(stderr,"%c",*smtp_data);
					smtp_data++;
					cnt++;

				}
				printf("%d \n",index);
				printf("\n================================================\n");

				}

			}
			//imap email accept
			else if(!strcmp(argv[2],"imap"))
			{
				if(993==ntohs(tcp->source) || 993==ntohs(tcp->dest)){
				printf("imap enter\n");
				int cnt=0;
				int i;
				//smtp=(struct smtphdr*)(buf+udphdr_len); //smtp header pointer
				imap_data=buf+tcphdr_len;
				imap_len=data_length-tcphdr_len;
				printf("[1]:%d \n",imap_len);
				printf("Imap Header\n");
				for(i=0; i<imap_len; i++){
					index++;
					//if(cnt==15)
					//{
					//      printf("\n");
					//      cnt=0;
					//}
					fprintf(stderr,"%c",*imap_data);
					imap_data++;
					cnt++;

				}
				printf("%d \n",index);
				printf("\n================================================\n");
				}

			}
			//p2p tcp header
			else if(!strcmp(argv[2],"p2ptcp"))
			{
				if(49152<ntohs(tcp->source) || 49152<ntohs(tcp->dest)){
				int i;
				printf("p2p enter\n");
				p2p_data=buf+tcphdr_len;
				p2p_len=data_length-tcphdr_len;
				printf("p2p Header\n");
				for(i=0; i<p2p_len; i++)
				{
					fprintf(stderr,"%c",*p2p_data);
					p2p_data++;
				}
				printf("\n=========================================\n");
				}
			}
			//ssh header payload
			else if(!strcmp(argv[2],"ssh"))
			{
				if(22==ntohs(tcp->source) || 22==ntohs(tcp->dest)){
				int i;
				int cnt=0;
				ssh=buf+tcphdr_len;
				ssh_len=data_length-tcphdr_len;

				if(ssh_len>500)
					print_len=500;
				else
					print_len=ssh_len;

				printf("SSH Payload\n");
				for(i=0; i<print_len; i++)
				{
					if(cnt>15){
						printf("\n");
						cnt=0;
					}
					printf("%02x ",*ssh);
					ssh++;
					cnt++;
				}

				printf("\n\n");
				ssh=buf+tcphdr_len;
				cnt=0;
				for(i=0; i<print_len; i++)
				{
					if(cnt>15)
					{
						printf("\n");
						cnt=0;
					}
					if((*ssh)>=32 && (*ssh)<=128)
					{
						printf("%c",*ssh);
					}
					else
						printf("-");
					ssh++;
					cnt++;
				}
				printf("\n==========================================\n");

				}

			}
			else
				continue;

		}
		//dns header print(UDP)
		else if(17==ip->protocol)
		{
			if(!strcmp(argv[2],"dns")){
				if(53==ntohs(udp->source) || 53==ntohs(udp->dest)){
				
				printf("DNS enter\n");
				unsigned char * query; //query pointer 
				int i;
				int cnt=0;
				int dns_query_len;//dns header length

				dns=(struct dnshdr *)(buf+udphdr_len); //dns header
				dns_query_len=data_length-udphdr_len-12;
				printf("\nDNS Header\n");

				printf(" + Identification      : %x\n", ntohs(dns->id));

				printf(" | Flags              : 0x%X\n", ntohs(dns->flags));

				printf(" | Questions           : %d\n", ntohs(dns->questions));

				printf(" | Answer RRs          : %d\n", ntohs(dns->answer_RRs));
				printf(" | Authority RRs       : %d\n", ntohs(dns->authority_RRs));
				printf(" + Additional RRs      : %d\n", ntohs(dns->additional_RRs));
				query=&(dns->query);
				//query print
				printf("Query print: \n");

				for(i=0; i<dns_query_len; i++)
				{
					//line 15
					if(cnt==15)
					{
						printf("\n");
						cnt=0;
					}
					printf("%02x ",*query);
					query++;
					cnt++;

				}
				printf("\n");
				query=&(dns->query);
				cnt=0;
				for(i=0; i<dns_query_len; i++)
				{
					//line 15
					if(cnt==15)
					{
						printf("\n");
						cnt=0;
					}
					if((*query)>=32 && (*query)<=128)
						printf("%c ",*query);
					else
						printf("-");
					query++;
					cnt++;

				}
				printf("\n");
				printf("=====================================================\n");

				}
			}
			//p2p tracker udp
			else if(!strcmp(argv[2],"p2ptracker"))
			{
				if(8999==ntohs(udp->source) || 8999==ntohs(udp->dest)){
				int i;
				printf("p2p enter\n");
				p2p_udp_data=buf+udphdr_len;
				p2p_udp_len=data_length-udphdr_len-12;
				printf("p2p Header\n");
				for(i=0; i<p2p_udp_len; i++)
				{
					fprintf(stderr,"%c",*p2p_udp_data);
					p2p_udp_data++;
				}
				}

			}
			else
				continue;
		}
		//process rule
		//ip address get
		//ip address filter(my address is only)
		//if TCP protocol
		//argv[2]: application protocol(http)
		//in protocol number http:80
		//port number filter
		//if UDP protocol
		//argv[2]: application protocol(dns)
		//port number: 53
		//
		//ip print
		//tcp print
		//udp print
		//if tcp in http
		//http print
		//if udp in dns
		//dns print


	}
	close(sock);
	return 0;





}

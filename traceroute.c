#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
static int in_cksum(unsigned short *buf, int sz)
{
    int nleft = sz;
    int sum = 0;
    unsigned short *w = buf;
    unsigned short ans = 0;
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
    if (nleft == 1)
    {
        *(unsigned char *) (&ans) = *(unsigned char *) w;
        sum += ans;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    ans = ~sum;
    return (ans);
}

int main(int argc, char *argv[])
{
    struct iphdr *ip;
    struct icmphdr *icmp;
    struct sockaddr_in target;
    struct hostent* host;
    struct in_addr address;
    int sock, optval = 1, bytes, retval;
    socklen_t addrlen = sizeof(struct sockaddr);
    char dest_ip[20], recv_buffer[BUFSIZ], send_buffer[(sizeof(struct iphdr)+sizeof(struct icmphdr))];
    struct iphdr *re_ip = (struct iphdr*)recv_buffer;
    struct icmphdr *re_icmp = (struct icmphdr*) (recv_buffer + sizeof(struct iphdr));
    ip = (struct iphdr*)send_buffer;
    icmp = (struct icmphdr*)(send_buffer+sizeof(struct iphdr));
    void *packet = (void *)ip;
    if (getuid() != 0)
    {
        fprintf(stderr, "%s: Require super user privileges\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc < 2)
    {
        fprintf(stderr, "\nUsage: %s <ip_address>\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    host = gethostbyname(argv[1]);
    strcpy(dest_ip,inet_ntoa(*(struct in_addr *)host->h_addr));
    ip -> ihl = 5;
    ip -> version = 4;
    ip -> tos  = 0;
    ip -> tot_len = sizeof(struct icmphdr)+sizeof(struct iphdr);
    ip -> id  = htons(0);
    ip -> frag_off = 0;
    ip -> ttl  = 1;
    ip -> protocol = IPPROTO_ICMP;
    ip -> saddr = INADDR_ANY;
    ip -> daddr = inet_addr(dest_ip);
    ip -> check  = in_cksum((unsigned short *)ip, sizeof(struct iphdr));
    icmp -> type = ICMP_ECHO;
    icmp -> code = 0;
    icmp -> un.echo.id = random();
    icmp -> un.echo.sequence = 0;
    icmp -> checksum = 0;
    icmp -> checksum = in_cksum((unsigned short*)icmp,sizeof(struct icmphdr));
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(dest_ip);
    sock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
    if (sock == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    retval = setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&optval,optval);
    if (retval == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    printf("\n\t TraceRoute\n");
    printf("\n\n");
    printf("Tracing: %s (%s)\n\n",argv[1],dest_ip);
    do
    {
        bytes = sendto(sock,packet,ip -> tot_len,0,(struct sockaddr*)&target,sizeof(struct sockaddr));
        if (bytes == -1)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
        bytes = recvfrom(sock,recv_buffer,BUFSIZ,0,(struct sockaddr*)NULL,&addrlen);
        if (bytes == -1)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        address.s_addr = re_ip -> saddr;
        printf("%3d - Packet returned by: %s - ",ip -> ttl,inet_ntoa(address));
	if (re_icmp -> type == ICMP_TIMXCEED)
	    printf("TTL Expired message\n");
	else if (re_icmp -> type == ICMP_ECHOREPLY)
	    printf("Echo reply\n");
	else
            printf("Message type: %d",re_icmp -> type);
	if (strcmp(dest_ip,inet_ntoa(address)) == 0)
        	{
	    printf("\nTrace route successful\n\n");
                  break;
               }
	ip -> ttl++;
   }while (1);
   return 0;
}


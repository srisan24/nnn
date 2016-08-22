#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netdb.h>
#include<errno.h>
int main(){

char s[50],p[13];
printf("Enter domain name:\n");
scanf("%s",s);
struct hostent *h;
struct in_addr **addr;
h=gethostbyname(s);
addr=(struct in_addr**)h->h_addr_list;
printf("\n%s\n",inet_ntoa(*addr[0]));


printf("\nEnter IP address :\n");
scanf("%s",p);
struct sockaddr_in convert;
inet_pton(AF_INET,p,&convert);
struct hostent *he;
he=gethostbyaddr(&convert,sizeof(convert),AF_INET);
printf("\n%s\n",he->h_name);

return 0;
}
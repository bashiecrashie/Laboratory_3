#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<poll.h>

void print_help();
int client(char *ip, int port, char *req);

#define red "\033[1;31m"
#define blue "\033[1;34m"
#define green "\033[1;32m"
#define ret "\033[00m"

void print_help()
{
	char *help = ""
	"./lab_3_client <server ip> <server port> <requst typ:A,B>\n"
	"./lab_3_client 192.168.0.2 1234 A\n";
	puts(help);
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("%s[-]%sInvalid number of arguments !\n", red, green);
		exit(1);
	}
	client(argv[1], atoi(argv[2]), argv[3]);
	return 0;
}

int client(char *ip, int port, char *req)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	char buff[2];
	struct pollfd pfd;

	if(sock < 0)
	{
		perror("[socket] reason");
		exit(1);
	}
	
	fcntl(sock, F_SETFL, O_NONBLOCK);
	
	pfd.fd = sock;
	pfd.events = POLLIN;

	struct sockaddr_in srv;
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port);
	srv.sin_addr.s_addr = inet_addr(ip);
	socklen_t srv_size = sizeof(srv);

	int sendto_status = sendto(sock, req, sizeof(req), MSG_CONFIRM, (struct sockaddr *)&srv, srv_size);
	
	int pollcount = poll(&pfd, 1, 5000);
	if ( pollcount == 0 ) 
		printf("%s[*]%sDatagram recieve timeout (5 s).\n", blue, green);

	if(sendto_status < 0)
	{
		perror("[sendto] reason");
		exit(1);
	}

	int recvfrom_status = recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr *)&srv, &srv_size);
	if(recvfrom_status < 0)
	{
		perror("[recvfrom] reason");
		exit(1);
	}
	
	printf("%s[+] Ваш номер в очереди: %s\n", green, buff);

	close(sock);

}

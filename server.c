#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

void print_logo();
void print_help();
int server(int port);

#define red "\033[1;31m"
#define blue "\033[1;34m"
#define green "\033[1;32m"
#define ret "\033[00m"

void print_logo()
{
	char *logo = ""
         "    ██████  █████  ██████ ██    ██  █████  ██████  \n"
	 "   ██░░░░  ██░░░██░░██░░█░██   ░██ ██░░░██░░██░░█  \n"
	 "  ░░█████ ░███████ ░██ ░ ░░██ ░██ ░███████ ░██ ░   \n"
	 "   ░░░░░██░██░░░░  ░██    ░░████  ░██░░░░  ░██     \n"
	 "    ██████ ░░██████░███     ░░██   ░░██████░███    \n"
	 "    ░░░░░░   ░░░░░░ ░░░       ░░     ░░░░░░ ░░░    \n";
	puts(logo);
}

void print_help()
{
	char *help = ""
	"./lab_3_server <port>\n"
	"example: ./lab_3_server 1234\n";
	puts(help);
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("%s[-]%sInvalid number of arguments !\n", red, green);
		print_help();
		exit(1);
	}
	print_logo();
	server(atoi(argv[1]));
	return 0;
}

int server(int port)
{
	struct sockaddr_in srv, client;
	int counter = 0;
	socklen_t client_size = sizeof(client);
	char buff[2], ip_buff[20], buff_a[2], buff_b[2];
	
	printf("%s[*]%sCreating socket...\n", blue, green);
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		perror("[socket] reason");
		exit(1);
	}
	printf("%s[+] Socket created !\n", green);

	srv.sin_family = AF_INET;
	srv.sin_port = htons(port);
	srv.sin_addr.s_addr = htonl(INADDR_ANY);

	printf("%s[*]%sBinding...\n", blue, green);
	int bind_status = bind(sock, (struct sockaddr *)&srv, sizeof(srv));
	if(bind_status < 0)
	{
		perror("[bind] reason");
		exit(1);
	}
	printf("%s[+] Binded !\n%s[*]%sWaiting for data...\n", green, blue, green);
	
	while(1)
	{
		int recvfrom_status = recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr *)&client, &client_size);
		if(recvfrom_status < 0)
		{
			perror("[recvfrom] reason");
			exit(1);
		}
		bzero(ip_buff, 20);
		inet_ntop(AF_INET, (struct sockaddr *)&client, ip_buff, client_size);
		printf("%s[+] Received data:%s Source:%s:%d", green, buff, ip_buff, htons(client.sin_port));

		if((strcmp(buff, "A")) == 0 || (strcmp(buff, "a")) == 0)
		{
			buff_a[0] = 'A';
			buff_a[1] = counter + '0';
			int sendto_status = sendto(sock, buff_a, sizeof(buff_a), MSG_DONTROUTE, (struct sockaddr *)&client, client_size);
			if(sendto_status < 0)
				perror("[sendto] reason");

		}else if((strcmp(buff, "B")) == 0 || (strcmp(buff, "b") == 0))
		{
			buff_b[0] = 'B';
			buff_b[1] = counter + '0';
			int sendto_status = sendto(sock, buff_b, sizeof(buff_b), MSG_DONTROUTE, (struct sockaddr *)&client, client_size);
			if(sendto_status < 0)
				perror("[sendto] reason");
		}
		++counter;
	}
	close(sock);
}

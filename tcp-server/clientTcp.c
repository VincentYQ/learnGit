#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 8080

int main(int argc, char *argv[])
{
	int clinPort = MYPORT;
	int clinFd;
	char buf[1000];
	struct sockaddr_in clinaddr;
	memset(buf,0,sizeof(buf));
	
	if((clinFd = socket(AF_INET, SOCK_STREAM,0)) == -1)
	{
		perror("client socket() error!");
		exit(1);
	}

	clinaddr.sin_family = AF_INET;
	clinaddr.sin_port   = htons(clinPort);
	clinaddr.sin_addr.s_addr   = inet_addr("127.0.0.1");
	
	if(connect(clinFd, (struct sockaddr *)&clinaddr, sizeof(clinaddr)) < 0)
	{
		perror("client connect() error!");
		exit(1);
	}
	int len;

	len = recv(clinFd, buf,sizeof(buf),0);
	if(len <= 0)
	{
		perror("client recv() error!");
		exit(1);
	}

	printf("%s\n",buf);

	close(clinFd);

	return 0;
}

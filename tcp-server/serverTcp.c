#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MYPORT 8080

int main(int argc,char *argv[])
{
	int myfd;
	int ClinetConn;
	unsigned int myport = MYPORT;
	unsigned int sinsize;
	struct sockaddr_in myaddr;
	
	if((myfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket() error!");
		exit(1);
	}

	myaddr.sin_family      = AF_INET;
	myaddr.sin_port        = htons(myport);
	myaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	if(bind(myfd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind() error!");
		exit(1);
	}

	if(listen(myfd, 10) == -1)
	{
		perror("listen() error!");
		exit(1);
	}
	int count = 0;
	while(1)
	{
		struct sockaddr_in addrClient;
		sinsize = sizeof(struct sockaddr_in);
		if((ClinetConn = accept(myfd,(struct sockaddr *)&addrClient, &sinsize)) == -1)
		{
			perror("accept() error!");
			exit(1);
		}
		char sendMessage[100];
		memset(sendMessage,0,sizeof(sendMessage));
		sprintf(sendMessage, "%d:%s",++count, "hello word");
		
		write(ClinetConn, sendMessage,sizeof(sendMessage));
	}

	close(myfd);
	return 0;
}

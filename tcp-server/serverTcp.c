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
		perror("server socket() error!");
		exit(1);
	}

	myaddr.sin_family      = AF_INET;
	myaddr.sin_port        = htons(myport);
	myaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	if(bind(myfd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr)) == -1)
	{
		perror("server bind() error!");
		exit(1);
	}

	if(listen(myfd, 10) == -1)
	{	
		perror("server listen() error!");
		exit(1);
	}

	int count = 0;
	char sendMessage[100];
	memset(sendMessage,0,sizeof(sendMessage));

	while(1)
	{
		struct sockaddr_in addrClient;
		sinsize = sizeof(struct sockaddr_in);
		if((ClinetConn = accept(myfd,(struct sockaddr *)&addrClient, &sinsize)) == -1)
		{
			perror("server accept() error!");
			exit(1);
		}

		sprintf(sendMessage, "%d:%s",++count, "hello word!");
		
		int sizeWrite,messaSize;
		messaSize = sizeof(sendMessage);
		sizeWrite = write(ClinetConn, sendMessage,messaSize);
		if(sizeWrite != messaSize)
		{
			perror("server write() error");
			exit(1);
		}		

		close(ClinetConn);
	}

	close(myfd);
	return 0;
}

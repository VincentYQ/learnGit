#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFsize 1024
#define MYPORT 8080

int main(int argc, char *argv[])
{
	int clinPort = MYPORT;
	int clinFd;
	char * buf = NULL;
	struct sockaddr_in clinaddr;
	
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
	int len,buflen;
	len = buflen = 0;
	char tempBuff[BUFFsize];
	while((len = read(STDIN_FILENO,tempBuff,BUFFsize) > 0))
	{
		buflen += len;
		buf = realloc(buf,buflen);
		if(buf == NULL)
		{
			perror("client realloc() error");
			exit(1);
		}
		strcat(buf,tempBuff);
	}
	if(len == -1)
	{
		perror("client read() error");
		exit(1);
	}
	len = 0;
	len = write(clinFd,buf,buflen);
	if(len != buflen)
	{
		perror("client write() error");
		exit(1);
	}
	
	//printf("%s\n",buf);

	close(clinFd);

	return 0;
}

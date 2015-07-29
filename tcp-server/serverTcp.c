#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf.h"
#include "ini.h"

#define BUFFSIZE 1024

void  readFromClient(const int ,char *,int * len);
//读取客户端发来的信息保存到char *中，长度为len

void mySocket(const int,char *,int *);
//服务端和客户端建立连接

int main(int argc,char *argv[])
{
   	int myfd;
	unsigned int myport;
	int buffLen = 0;
	char * readBuff = NULL;
	struct sockaddr_in myaddr;
	
	conf_init();
	myport = global_settings.port;

//	printf("port :%d\n",myport);

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

	while(1)
	{
		mySocket(myfd,readBuff,&buffLen);
		printf("readBuff:%s\n buffLen:%d\n",readBuff,buffLen);
	}

	close(myfd);
	return 0;
}

void mySocket(int myfd, char * readBuff, int * bufLen)
{
	int ClientConn;
	unsigned int sinsize;
	struct sockaddr_in addrClient;
	sinsize = sizeof(struct sockaddr_in);
	if((ClientConn = accept(myfd,(struct sockaddr *)&addrClient, &sinsize)) == -1)
	{
		perror("server accept() error!");
		exit(1);
	}
	readFromClient(ClientConn,readBuff,bufLen);
	close(ClientConn);
}
void readFromClient(int ClientConn,char * readBuff,int *bufLen)
{
	char buf[BUFFSIZE];
	int len;
	while((len = read(ClientConn,buf,BUFFSIZE))>0)
	{
		*bufLen += len;
		readBuff = realloc(readBuff,*bufLen);
		if(readBuff == NULL)
		{
			perror("realloc() error");
			exit(1);
		}
		strcat(readBuff,buf);
	}
	if(len == -1)
	{
		perror("readFromClient read() error");
		exit(1);
	}
}

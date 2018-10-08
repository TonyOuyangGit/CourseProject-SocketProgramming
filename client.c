// Copyright©TonyOuyang 10/2018
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define BUF_SIZE 10
//My USC ID is xxxx765584
#define SERVERAWS_PORT 25584

int Client_Socket,Flag;
struct sockaddr_in Client_addr;
char DataSend_BUF[BUF_SIZE];

int main(int argc, char const *argv[])
{

	/******************************
			Address Initiation
	Initiation of Socket_addr address set up. This codes is from Beej's tutorial guide.
	******************************/
	memset(&Client_addr,0,sizeof(Client_addr));
	Client_addr.sin_family = AF_INET;
	Client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	Client_addr.sin_port = htons(SERVERAWS_PORT);

	/******************************
		Socket set up
	******************************/
	Client_Socket = socket(AF_INET,SOCK_STREAM,0);
	if (Client_Socket < 0)
	{
		perror("Socket");
	}
	printf("The client is up and running\n");

	/******************************
	to avoid any typo or wrong input to send to aws
	I checked the input of parameter in client
	******************************/

	if ((0 == strcmp("LOG",argv[1]) || (0 == strcmp("DIV",argv[1]))) )
	{
		if ((atof(argv[2]) > 0) && (atof(argv[2]) < 1))
		{
			Flag = connect(Client_Socket,(struct sockaddr *)&Client_addr,sizeof(Client_addr));

			if (Flag < 0)
			{
				perror("Connect");
			}
			//printf("Client connected to serverAWS\n");

			//combine two string into one message to send to aws
			strcpy(DataSend_BUF,argv[1]);
			strcat(DataSend_BUF,argv[2]);
			//printf("The input is %s:\n",argv[1]);
			//printf("The input is %s:\n",argv[2]);

			printf("The client sent <%s> and <%s> to AWS\n",argv[2],argv[1]);

			Flag = send(Client_Socket,DataSend_BUF,sizeof(DataSend_BUF),0);

			if (Flag < 0)
			{
				perror("send");
			}
			
			Flag = recv(Client_Socket,DataSend_BUF,sizeof(DataSend_BUF),0);
			
			if (Flag < 0)
			{
				perror("recv");
			}
			
			printf("According to AWS <%s> on <%s>: <%s>\n",argv[1],argv[2],DataSend_BUF);

			close(Client_Socket);
		}
		
		else
		{
			printf("Input Number not in range\n");
			return 0;
		}
		return 0;
	}
	else
	{
		printf("Input Funtion Error\n");
		return 0;
	}




	return 0;
}
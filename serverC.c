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

#define SERVER_C 23584
#define DATABUFFER_SIZE 10
#define TRUE 1

int main(int argc, char const *argv[])
{
	int ServerC_socket;
	struct sockaddr_in ServerC_addr;
	struct sockaddr_in Remote_addr;
	int Remote_addr_len,Receive_Flag,Send_Flag;
	char Data_Buf[DATABUFFER_SIZE];
	double Receive_Numebr;

	while(TRUE)
	{
		/******************************
		Socket and address initiate
		Initiation of Socket_addr address set up. This codes is from Beej's tutorial guide.

		******************************/	
		memset(&ServerC_addr,0,sizeof(ServerC_addr));
		memset(&Remote_addr,0,sizeof(Remote_addr));
		
		Remote_addr_len = sizeof(Remote_addr);

		ServerC_addr.sin_family = AF_INET;
		ServerC_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		ServerC_addr.sin_port = htons(SERVER_C);
/*		Remote_addr.sin_family = AF_INET;
		Remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1")
		Remote_addr.sin_port = htons(UDP_RECEIVE_PORT);*/

		ServerC_socket = socket(AF_INET, SOCK_DGRAM, 0);

		if (-1 == ServerC_socket)
		{
			perror("ServerC_socket");
		}
		printf("The Server C is up and running using UDP on port <%d>\n",ntohs(ServerC_addr.sin_port));

		// Reslove port reuse, I got this idea from stackoverflow
    	if (setsockopt(ServerC_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
    	{	
    		printf("setsockopt(SO_REUSEADDR) failed");
    	}	

		/******************************
		Bind port and start receive
		******************************/	
		if (bind(ServerC_socket,(struct sockaddr *)&ServerC_addr, sizeof(ServerC_addr)) < 0)
		{
			perror("bind");
		}

		/******************************
		receive and calculate 
		******************************/

		Receive_Flag = recvfrom(ServerC_socket, Data_Buf, sizeof(Data_Buf), 0, (struct sockaddr *)&Remote_addr, &Remote_addr_len);
		if (-1 == Receive_Flag)
		{
			perror("recvfrom");
		}
		//printf("received packet from :%s port: %d\n",inet_ntoa(Remote_addr.sin_addr),ntohs(ServerC_addr.sin_port));
		printf("The Server C received input <%s>\n", Data_Buf);
		
		Receive_Numebr = atof(Data_Buf);
		Receive_Numebr = Receive_Numebr * Receive_Numebr * Receive_Numebr * Receive_Numebr * Receive_Numebr;
		printf("The Server C calculated 5th power <%g>\n", Receive_Numebr);
		sprintf(Data_Buf,"%g",Receive_Numebr);
		
		Send_Flag = sendto(ServerC_socket,Data_Buf,sizeof(Data_Buf),0,(struct sockaddr *)&Remote_addr,sizeof(Remote_addr)); 
		if (-1 == Send_Flag)
		{
			perror("sendto");
		}
		printf("The Server C finished sending the output to AWS\n");

		close(ServerC_socket);
	}


	return 0;
}
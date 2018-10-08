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

#define SERVER_A 21584
#define DATABUFFER_SIZE 10
#define TRUE 1

int main(int argc, char const *argv[])
{
	int ServerA_socket,ServerA_Remote_socket;
	struct sockaddr_in ServA_addr;
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
		memset(&ServA_addr,0,sizeof(ServA_addr));
		memset(&Remote_addr,0,sizeof(Remote_addr));
		
		Remote_addr_len = sizeof(Remote_addr);

		ServA_addr.sin_family = AF_INET;
		ServA_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		ServA_addr.sin_port = htons(SERVER_A);

/*		Remote_addr.sin_family = AF_INET;
		Remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1")
		Remote_addr.sin_port = htons(UDP_RECEIVE_PORT);*/

		ServerA_socket = socket(AF_INET, SOCK_DGRAM, 0);

		if (-1 == ServerA_socket)
		{
			perror("ServerA_socket");
		}
		printf("The Server A is up and running using UDP on port <%d>\n",ntohs(ServA_addr.sin_port));

		// Reslove port reuse, I got this idea from stackoverflow
    	if (setsockopt(ServerA_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
    	{	
    		printf("setsockopt(SO_REUSEADDR) failed");
    	}	

		/******************************
		Bind port and start receive
		******************************/	
		if (bind(ServerA_socket,(struct sockaddr *)&ServA_addr, sizeof(ServA_addr)) < 0)
		{
			perror("bind");
		}

		/******************************
		receive and calculate 
		******************************/

		Receive_Flag = recvfrom(ServerA_socket, Data_Buf, sizeof(Data_Buf), 0, (struct sockaddr *)&Remote_addr, &Remote_addr_len);
		if (-1 == Receive_Flag)
		{
			perror("recvfrom");
		}
		//printf("received packet from :%s port: %d\n",inet_ntoa(Remote_addr.sin_addr),ntohs(ServA_addr.sin_port));
		printf("The Server A received input <%s>\n", Data_Buf);
		
		Receive_Numebr = atof(Data_Buf);
		Receive_Numebr = Receive_Numebr * Receive_Numebr;
		printf("The Server A calculated square <%g>\n", Receive_Numebr);
		sprintf(Data_Buf,"%g",Receive_Numebr);
		
		Send_Flag = sendto(ServerA_socket,Data_Buf,sizeof(Data_Buf),0,(struct sockaddr *)&Remote_addr,sizeof(Remote_addr)); 
		if (-1 == Send_Flag)
		{
			perror("sendto");
		}
		printf("The Server A finished sending the output to AWS\n");

		close(ServerA_socket);
	}


	return 0;
}
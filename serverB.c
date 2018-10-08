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

#define SERVER_B 22584
#define DATABUFFER_SIZE 10
#define TRUE 1

int main(int argc, char const *argv[])
{
	int ServerB_socket;
	struct sockaddr_in ServB_addr;
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
		memset(&ServB_addr,0,sizeof(ServB_addr));
		memset(&Remote_addr,0,sizeof(Remote_addr));
		
		Remote_addr_len = sizeof(Remote_addr);

		ServB_addr.sin_family = AF_INET;
		ServB_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		ServB_addr.sin_port = htons(SERVER_B);
/*		Remote_addr.sin_family = AF_INET;
		Remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1")
		Remote_addr.sin_port = htons(UDP_RECEIVE_PORT);*/

		ServerB_socket = socket(AF_INET, SOCK_DGRAM, 0);

		if (-1 == ServerB_socket)
		{
			perror("ServerB_socket");
		}
		printf("The Server B is up and running using UDP on port <%d>\n",ntohs(ServB_addr.sin_port));

		// Reslove port reuse, I got this idea from stackoverflow
    	if (setsockopt(ServerB_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
    	{	
    		printf("setsockopt(SO_REUSEADDR) failed");
    	}	

		/******************************
		Bind port and start receive
		******************************/	
		if (bind(ServerB_socket,(struct sockaddr *)&ServB_addr, sizeof(ServB_addr)) < 0)
		{
			perror("bind");
		}

		/******************************
		receive and calculate 
		******************************/

		Receive_Flag = recvfrom(ServerB_socket, Data_Buf, sizeof(Data_Buf), 0, (struct sockaddr *)&Remote_addr, &Remote_addr_len);
		if (-1 == Receive_Flag)
		{
			perror("recvfrom");
		}
		//printf("received packet from :%s port: %d\n",inet_ntoa(Remote_addr.sin_addr),ntohs(ServB_addr.sin_port));
		printf("The Server B received input <%s>\n", Data_Buf);
		
		Receive_Numebr = atof(Data_Buf);
		Receive_Numebr = Receive_Numebr * Receive_Numebr * Receive_Numebr;
		printf("The Server B calculated cube <%g>\n", Receive_Numebr);
		sprintf(Data_Buf,"%g",Receive_Numebr);
		
		Send_Flag = sendto(ServerB_socket,Data_Buf,sizeof(Data_Buf),0,(struct sockaddr *)&Remote_addr,sizeof(Remote_addr)); 
		if (-1 == Send_Flag)
		{
			perror("sendto");
		}
		printf("The Server B finished sending the output to AWS\n");

		close(ServerB_socket);
	}


	return 0;
}
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


#define BACKLOG 10
//My USC ID is xxxx765584
#define SERVAWS_PORT 25584
#define UDP_RECEIVE_PORT 24584
#define SERVER_A 21584
#define SERVER_B 22584
#define SERVER_C 23584
#define Data_BUF_SIZE 10
#define RESULT_BUF_SIZE 10
#define TRUE 1

int TCPSocketInit(int sk);
void UDPSocketInit(void);
void DivideString(void);
void Calculation(void);
void DIV_Calculation(void);
void LOG_Calculation(void);

int AWS_Socket, Client_Socket, UDP_Socket, SERV_A_Socket;
struct sockaddr_in ServerAWS_addr, Client_addr, UDPA_addr, UDPB_addr, UDPC_addr, UDPReceive_addr;
socklen_t clin_len,udp_len;
char Data_BUF[Data_BUF_SIZE], Result_BUF[RESULT_BUF_SIZE];
char FunctionAWS[3],ValueString[Data_BUF_SIZE];
int  UDPReceive_Flag,UDPSend_Flag;
double Receive_Num;
double Value,X,X1,X2,X3,X4,X5,X6;


int main(int argc, char const *argv[])
{

	while(TRUE)
	{
		/******************************
				Address Initiation
		Initiation of Socket_addr address set up. This codes is from Beej's tutorial guide.
		******************************/
		memset(&ServerAWS_addr,0,sizeof(ServerAWS_addr));
		memset(&Client_addr,0,sizeof(Client_addr));
		memset(&UDPA_addr,0,sizeof(UDPA_addr));
		memset(&UDPB_addr,0,sizeof(UDPB_addr));
		memset(&UDPC_addr,0,sizeof(UDPC_addr));
		memset(&UDPReceive_addr,0,sizeof(UDPA_addr));

		printf("The AWS is up and running\n");
		AWS_Socket = TCPSocketInit(AWS_Socket);
		UDPSocketInit();
		udp_len = sizeof(struct sockaddr_in);


		// Reslove port reuse, I got this idea from stackoverflow
    	if (setsockopt(AWS_Socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
    	{	
    		printf("setsockopt(SO_REUSEADDR) failed");
    	}
		
		/******************************
		Bind TCP socket and Receive Data 
		******************************/
		bind(AWS_Socket, (struct sockaddr *)&ServerAWS_addr, sizeof(ServerAWS_addr));
		listen(AWS_Socket,BACKLOG);

		clin_len = sizeof(Client_addr);
		Client_Socket = accept(AWS_Socket, (struct sockaddr *)&Client_addr,&clin_len);
		recv(Client_Socket, Data_BUF, sizeof(Data_BUF),0);

		/******************************
		Find Function and value
		******************************/
		DivideString();
		printf("The AWS received <%g> and function=<%s> from the client using TCP over port <%d>\n",atof(ValueString),FunctionAWS,ntohs(ServerAWS_addr.sin_port));

		X = atof(ValueString);  //Save the value of X after divided for further calculation.
		sprintf(ValueString,"%g",X);

		/******************************
		Send Value to ServerA to get x2
		******************************/
		UDPSend_Flag = sendto(UDP_Socket, ValueString, sizeof(ValueString), 0, (struct sockaddr *)&UDPA_addr, sizeof(UDPA_addr));
		if(UDPSend_Flag < 0)
		{
			perror("sendto");
		}
		printf("The AWS sent <%g> to Backend-Server <A>\n",atof(ValueString));

		UDPReceive_Flag = recvfrom(UDP_Socket, ValueString,sizeof(ValueString),0,(struct sockaddr *)&UDPA_addr,&udp_len);
		if (UDPReceive_Flag < 0)
		{
			perror("recvfrom");
		}
		X2 = atof(ValueString);
		printf("The AWS received <%g> Backend-Server <A> using UDP over port <%d>\n",X2,ntohs(UDPReceive_addr.sin_port));

		/******************************
		Send Value to ServerB to get x3
		******************************/	
		sprintf(ValueString,"%g",X);
		UDPSend_Flag = sendto(UDP_Socket, ValueString, sizeof(ValueString), 0, (struct sockaddr *)&UDPB_addr, sizeof(UDPB_addr));
		if(UDPSend_Flag < 0)
		{
			perror("sendto");
		}
		printf("The AWS sent <%g> to Backend-Server <B>\n",atof(ValueString));

		UDPReceive_Flag = recvfrom(UDP_Socket, ValueString,sizeof(ValueString),0,(struct sockaddr *)&UDPB_addr,&udp_len);
		if (UDPReceive_Flag < 0)
		{
			perror("recvfrom");
		}
		X3 = atof(ValueString);
		printf("The AWS received <%g> Backend-Server <B> using UDP over port <%d>\n",X3,ntohs(UDPReceive_addr.sin_port));


		/******************************
		Send Value to ServerA to get x4
		******************************/
		sprintf(ValueString,"%g",X2);
		UDPSend_Flag = sendto(UDP_Socket, ValueString, sizeof(ValueString), 0, (struct sockaddr *)&UDPA_addr, sizeof(UDPA_addr));
		if(UDPSend_Flag < 0)
		{
			perror("sendto");
		}
		printf("The AWS sent <%g> to Backend-Server <A>\n",atof(ValueString));

		UDPReceive_Flag = recvfrom(UDP_Socket, ValueString,sizeof(ValueString),0,(struct sockaddr *)&UDPA_addr,&udp_len);
		if (UDPReceive_Flag < 0)
		{
			perror("recvfrom");
		}
		X4 = atof(ValueString);
		printf("The AWS received <%g> Backend-Server <A> using UDP over port <%d>\n",X4,ntohs(UDPReceive_addr.sin_port));


		/******************************
		Send Value to ServerC to get x5
		******************************/
		sprintf(ValueString,"%g",X);
		UDPSend_Flag = sendto(UDP_Socket, ValueString, sizeof(ValueString), 0, (struct sockaddr *)&UDPC_addr, sizeof(UDPC_addr));
		if(UDPSend_Flag < 0)
		{
			perror("sendto");
		}
		printf("The AWS sent <%g> to Backend-Server <C>\n",atof(ValueString));

		UDPReceive_Flag = recvfrom(UDP_Socket, ValueString,sizeof(ValueString),0,(struct sockaddr *)&UDPC_addr,&udp_len);
		if (UDPReceive_Flag < 0)
		{
			perror("recvfrom");
		}
		X5 = atof(ValueString);
		printf("The AWS received <%g> Backend-Server <C> using UDP over port <%d>\n",X5,ntohs(UDPReceive_addr.sin_port));


		/******************************
		Send Value to ServerB to get x6
		******************************/	
		sprintf(ValueString,"%g",X2);
		UDPSend_Flag = sendto(UDP_Socket, ValueString, sizeof(ValueString), 0, (struct sockaddr *)&UDPB_addr, sizeof(UDPB_addr));
		if(UDPSend_Flag < 0)
		{
			perror("sendto");
		}
		printf("The AWS sent <%g> to Backend-Server <B>\n",atof(ValueString));

		UDPReceive_Flag = recvfrom(UDP_Socket, ValueString,sizeof(ValueString),0,(struct sockaddr *)&UDPB_addr,&udp_len);
		if (UDPReceive_Flag < 0)
		{
			perror("recvfrom");
		}
		X6 = atof(ValueString);
		printf("The AWS received <%g> Backend-Server <B> using UDP over port <%d>\n",X6,ntohs(UDPReceive_addr.sin_port));
		
		printf("Values of powers received by AWS:<%g,%g,%g,%g,%g,%g>\n",X,X2,X3,X4,X5,X6);

		Calculation();
		//sprintf(Result_BUF,"%g",Receive_Num);
		//printf("Result BUF:%s\n",Result_BUF);
		printf("AWS calculate %s on <%g>: <%s>\n", FunctionAWS,X,Result_BUF);
		printf("The AWS sent <%s> to client\n", Result_BUF);

		if (send(Client_Socket,Result_BUF,sizeof(Result_BUF),0) < 0)	
		{
			perror("send");
		}



		close(AWS_Socket);
		close(Client_Socket);
		close(UDP_Socket);

	}


	return 0;
}

/******************************
	TCP Server Initiation
******************************/
int TCPSocketInit(int sk)
{
	sk = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sk)
	{
		perror("sk");
	}

	ServerAWS_addr.sin_family = AF_INET;
	ServerAWS_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAWS_addr.sin_port = htons(SERVAWS_PORT);
	return sk;
}
	
/******************************
	UDP Socket Initiation
******************************/	
void UDPSocketInit(void)
{
	UDP_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == UDP_Socket)
	{
		perror("udpsk");
	}

	UDPA_addr.sin_family = AF_INET;
	UDPA_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	UDPA_addr.sin_port = htons(SERVER_A);

	UDPB_addr.sin_family = AF_INET;
	UDPB_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	UDPB_addr.sin_port = htons(SERVER_B);

	UDPC_addr.sin_family = AF_INET;
	UDPC_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	UDPC_addr.sin_port = htons(SERVER_C);


	UDPReceive_addr.sin_family = AF_INET;
	UDPReceive_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	UDPReceive_addr.sin_port = htons(UDP_RECEIVE_PORT);

    if (setsockopt(UDP_Socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
	{	
	  	printf("setsockopt(SO_REUSEADDR) failed");
	}

	if (bind(UDP_Socket,(struct sockaddr *)&UDPReceive_addr, sizeof(UDPReceive_addr)) < 0)
	{
		perror("bind");
	}
	   //printf("Bind UDP receive success on port:%d\n", ntohs(UDPReceive_addr.sin_port));

}

/******************************
Divide into FunctionAWS and value
******************************/
void DivideString(void)
{

	//printf("DataBUF is: %s ,Divide the string\n",Data_BUF);
	strncpy(FunctionAWS,Data_BUF,sizeof(FunctionAWS));
	FunctionAWS[3] = '\0';
	strncpy(ValueString,Data_BUF + 3,sizeof(ValueString));
	ValueString[Data_BUF_SIZE] = '\0';
	//printf("FunctionAWS is :%s\n",FunctionAWS);
	//printf("ValueString is :%s\n",ValueString);

}

void Calculation(void)
{
	if (0 == strcmp("DIV",FunctionAWS))
	{
		//printf("You are so DIV\n");
		DIV_Calculation();
	}

	else if (0 == strcmp("LOG",FunctionAWS))
	{
		//printf("You are so LOG\n");
		LOG_Calculation();
	}
	else
	{
		printf("Function Error\n");
	}
}

void DIV_Calculation(void)
{
	double DIV_Result;
	DIV_Result = 1 + X + X2 + X3 + X4 + X5 + X6;
	//printf("DIV calculate:%g\n", DIV_Result);
	sprintf(Result_BUF,"%g",DIV_Result);
	//printf("Result BUFFER: %s\n", Result_BUF);
}

void LOG_Calculation(void)
{
	double LOG_Result;
	LOG_Result = - X - (X2/2) - (X3/3) - (X4/4) - (X5/5) - (X6/6);
	//printf("LOG calculate:%g\n", LOG_Result);
	sprintf(Result_BUF,"%g",LOG_Result);
	//printf("Result BUFFER: %s\n", Result_BUF);
}
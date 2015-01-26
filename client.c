#include <stdio.h>			//Declaration of all the header files	
#include <sys/types.h>
#include <sys/socket.h>								
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

void error(char *msg)			//Function used to print ERROR
{
perror(msg);
exit(1);
}

int main(int argc, char *argv[])
{

	int sock_client_proxy, port_no, n;
	char buffer1[BUFSIZ+1];
	struct hostent *server;
	struct sockaddr_in serv_addr;
		
	if (argc<3)
	{
		error("Please give the correct parameters as follows:\n<proxy server name> <ip address of the proxy server> <port no>");
		
	}
	
	port_no= atoi(argv[2]);					//Converts the string argument to an integer
	sock_client_proxy=socket(AF_INET, SOCK_STREAM,0);

	if (sock_client_proxy<0)
	{
		error("Error in opening socket");
	}
	
	server= gethostbyname(argv[1]);
	if (server == NULL)
	{
		error("ERROR, there is no such host\n");
		
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));		//to empty everything in serv_addr
	
	serv_addr.sin_family= AF_INET;				//For using IPv4
	
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	
	serv_addr.sin_port= htons(port_no);				//to convert to Big Endian format

	if (connect(sock_client_proxy,&serv_addr, sizeof(serv_addr))<0)
	{
		error("ERROR connecting to the socket of the proxy server!");
	}
	printf("\nThe connection with the proxy server has been established!");
	printf("\n*********************************************************");
	printf("\n*********************************************************");
	printf("\n************WELCOME TO PRANJAL'S PROXY SERVER************");
	printf("\n*********************************************************");
	printf("\n*********************************************************");
	printf("\nPlease enter the site where you want to go according to the format given below\n <www.xyz.com> <portno> <file name>: ");
	bzero(buffer1,BUFSIZ+1);
	fgets(buffer1,BUFSIZ,stdin);
	n= write(sock_client_proxy,buffer1,strlen(buffer1));
	if (n<0)
	{
		error("ERROR in writing to the socket of the proxy server!");
	}

	bzero(buffer1,BUFSIZ+1);
	n= read(sock_client_proxy,buffer1,BUFSIZ);

	if (n<0)
	{
		error("ERROR in reading from the socket of the proxy server!");
	}
	printf("\nThe reply from proxy server is:\nHere's you website: %s\n",buffer1);
	bzero(buffer1,BUFSIZ+1);

	return 0;
}	


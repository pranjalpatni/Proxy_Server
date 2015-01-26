#include <stdio.h>						//Headers required for using different functions
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
static void *doit(void *);
char *proxy_web(char *addr1,char *addr2, char *addr3);		//Function for connection between proxy server and webserver
void proxy_client(int sock_proxy_listen);			//Function for connection between proxy server and client
void error(char *msg)						//function to display error
{
perror(msg);
exit(1);
}
int sock_proxy_listen, sock_proxy_client;
int main(int argc, char *argv[])
{
	
	pthread_t tid;
	int portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;
		
	if (argc<2)
	{
		error("No port provided\n");
		
	}
	
	sock_proxy_listen=socket(AF_INET, SOCK_STREAM,0);		//for listening

	if (sock_proxy_listen<0)
	{
		error("ERROR in opening socket\n");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	portno= atoi(argv[1]);				//converts string argument to integer

	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr= INADDR_ANY;
	serv_addr.sin_port= htons(portno);

	if (bind(sock_proxy_listen, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)		//binding
	{
		error("ERROR in binding the socket with the client");
	}

	listen(sock_proxy_listen,5);
	for(;;) 
	{
	clilen=sizeof(cli_addr);

	sock_proxy_client=accept(sock_proxy_listen,(struct sockaddr *) &cli_addr, &clilen);		//accept

	if (sock_proxy_client<0)
	{
		error("ERROR on accepting the socket for the client!");
	}
	else
	{
	printf("\nThe connection with the client has been established!");
	}
	pthread_create(&tid, NULL, &doit, (void *) sock_proxy_client);			//thread creation
	}
	}

	static void * doit(void *arg)
	{
    	pthread_detach(pthread_self());
	proxy_client((int) arg);
	close((int) arg);
	return (NULL);
	}
	void proxy_client(int sock_proxy_listen)
	{
	char buffer1[BUFSIZ+1];
	int n;
	char addr_1[BUFSIZ+1],addr_2[BUFSIZ+1],addr_3[BUFSIZ+1];
	bzero(buffer1,BUFSIZ+1);
	bzero(addr_1,BUFSIZ+1);
	bzero(addr_2,BUFSIZ+1);
	bzero(addr_3,BUFSIZ+1);
	n= read(sock_proxy_listen,buffer1,BUFSIZ);		//reading
	if (n<0)
	{
		error("ERROR in reading from socket");
	}
	sscanf(buffer1, "%s %s %s", addr_1, addr_2, addr_3);
	printf("\nReceived message from client:- Host: %s Port: %s File: %s\n",addr_1,addr_2,addr_3);
	bzero(buffer1,BUFSIZ+1);
	strcpy(buffer1,proxy_web(addr_1,addr_2,addr_3));
	n=write(sock_proxy_listen,buffer1,BUFSIZ+1);		//writing
	if (n<0)
	{
		error("ERROR writing to socket");
	}
	close(sock_proxy_client);			//closing the socket
	}

	char *proxy_web(char *addr1, char *addr2,char *addr3)
	{
	int i,n2,portno2,sock_proxy_web;
	struct hostent *server2;
	struct in_addr **addr_list;
	struct sockaddr_in serv_addr2;
	char buffer2[BUFSIZ+1];
							//Now the proxy server acts as client for the web server
	server2= gethostbyname(addr1);
	if (server2 == NULL)
	{
		error("No such remote site\n");
		exit(0);
	}
	printf("\nThe official name of the website is: %s\n", server2->h_name);
	
	addr_list= (struct in_addr **)server2->h_addr_list;
	char *abc=inet_ntoa(*addr_list[0]);
	printf("\nThe IP address of the website is: %s\n",abc);
	

	portno2= atoi(addr2);
	sock_proxy_web=socket(AF_INET, SOCK_STREAM,0);

	if (sock_proxy_web<0)
	{
		error("ERROR opening socket");
	}
	
	bzero((char *) &serv_addr2, sizeof(serv_addr2));
	
	serv_addr2.sin_family= AF_INET;
	
	bcopy((char *)server2->h_addr,(char *)&serv_addr2.sin_addr.s_addr,server2->h_length);
	
	serv_addr2.sin_port= htons(portno2);

	if (connect(sock_proxy_web,&serv_addr2, sizeof(serv_addr2))<0)
	{
		error("ERROR connecting");
	}
	char a[]="GET http://";
	char b[]="/";
	char c[]=" ";
	char d[]="HTTP/1.1";
	char e[]="\r\n";
	char f[]="GET";
	char g[]="Host: ";
	char h[]="www.utdallas.edu";
	
	if (strcmp(server2->h_name,h)== 0)
	{
	bzero(buffer2,BUFSIZ+1);
	strcpy(buffer2,f);
	strcat(buffer2,c);
	strcat(buffer2,addr3);
	strcat(buffer2,c);
	strcat(buffer2,d);
	strcat(buffer2,c);
	strcat(buffer2,e);
	strcat(buffer2,g);
	strcat(buffer2,addr1);
	strcat(buffer2,c);
	strcat(buffer2,e);
	strcat(buffer2,e);
	
	}

	else
	{
	bzero(buffer2,BUFSIZ+1);
	strcpy(buffer2,a);
	strcat(buffer2,addr1);
	strcat(buffer2,b);
	strcat(buffer2,addr3);
	strcat(buffer2,c);
	strcat(buffer2,d);
	strcat(buffer2,c);
	strcat(buffer2,e);
	strcat(buffer2,e);
	}
	printf("\nConnection with %s has been established!",server2->h_name);
	printf("\nThe transferred message to %s is:\n%s",server2->h_name,buffer2);
	n2= write(sock_proxy_web,buffer2,strlen(buffer2));
	if (n2<0)
	{
		error("ERROR writing to socket");
	}

	bzero(buffer2,BUFSIZ+1);
	n2= read(sock_proxy_web,buffer2,BUFSIZ);

	if (n2<0)
	{
		error("ERROR reading from socket");
	}
	printf("\nThe reply from %s is:\n%s\n",server2->h_name,buffer2);
	
	return buffer2;
	}	


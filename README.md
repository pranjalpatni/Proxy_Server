# Proxy_Server

DESCRIPTION:
The scenario is that we have a private network with a number of client computers that do not have direct connection to the Internet. The access to the Internet is provided by the web proxy server. Any client, when he/she wants to communicate with some remote web server, will contact the web proxy server to get forwarding service.

The goal of the application is to communicate between client and external web server (e.g., Google or UT Dallas web server) through the web proxy server. This web proxy server will work as Internet gateway at the application layer. If a client wants to go to www.google.com, it will pass the requested application layer job (like getting www.google.com homepage) to the web proxy server. And the web proxy server will get the requested content from the Google web server and then will pass it to the client. The client is single-threaded and uses TCP connection to communicate with the web proxy server. So the client does not connect to the external web server directly.

I have done my programming in C, so the following codes can be used for compilation:

For proxyserver.c:
gcc -o proxyserver proxyserver.c -lpthread

For client.c:
gcc -o client client.c

To run the proxyserver, you need to specify the port-number that you wan't to use. For example-
./proxyserver 9632

To run the client, you need to mention the ip address of the proxyserver and the same port-number that we have used above. For example-
Let the proxyserver is running in "net01.utdallas.edu". So we need to give the following arguments:
./client 10.176.67.64 9632

where,  10.176.67.64 is the ip address of "net01.utdallas.edu" (we can use ping to find the ip address of any website)
	and 9632 is the port-number used by proxyserver.

At the client side, we now need to mention the site where we want to go. Its format is:
<name of site> <port number> <file>

For example:
1- www.google.com 80 index.html

Note: Although we are asked to only write code for http. But still I am taking input from the user as 80. I could have hard-coded the program to "80", but it is better to take input from the user in practical situation.

2- www.utdallas.edu 80 /~ksarac/index.html

LIMITATION:
The program will work for only sending an HTTP get request. Also, the output may vary for differnet sites.

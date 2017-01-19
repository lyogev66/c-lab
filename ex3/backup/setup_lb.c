#include "common.h"
#include "setup_lb.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>



#define HTTP_PORT_FILENAME "http_port"
#define SERVER_PORT_FILENAME "server_port"
#define MAX_PORT_NUMBER 64000
#define MIN_PORT_NUMBER 1024
#define BACKLOG 10

//should be in common.h?
#define NUMBER_OF_SERVERS 3


int get_port_in_range()
{
	return ((rand()+MIN_PORT_NUMBER)%MAX_PORT_NUMBER);
}
void write_port_to_file(char *filename,int port_number)
{
	FILE *port_file;

	port_file=fopen(filename,"w");
	fprintf(port_file,"%d",port_number);
	fclose(port_file);
}
int setup_listening_socket(int type_server_or_http)
{
	struct sockaddr_in service;
	int new_socket,port_number;
	int bind_succesfull=ERROR_VALUE,enable=1,listen_succesfull;

	new_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (new_socket == ERROR_VALUE){
		printf("Error creating socket\n");
		exit(STATUS_ERROR);
	}
	setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	while (bind_succesfull == ERROR_VALUE)
	{
	port_number = get_port_in_range();
	service.sin_port = htons( port_number );
	bind_succesfull = bind( new_socket, (struct sockaddr *) &service, sizeof(service) );
	}
	listen_succesfull = listen(new_socket,BACKLOG);
	if(listen_succesfull == ERROR_VALUE){
		printf("Error listening to socket\n");
		close(new_socket);
		exit(STATUS_ERROR);
	}
	if (type_server_or_http==TYPE_HTTP){
		write_port_to_file(HTTP_PORT_FILENAME,port_number);
	}else{
		write_port_to_file(SERVER_PORT_FILENAME,port_number);
	}
	return new_socket; 
}
void accept_all_servers(int socket_server_array[NUMBER_OF_SERVERS],int bind_socket_for_server)
{
	int i;
	for(i=0;i<NUMBER_OF_SERVERS;i++)
	{
		socket_server_array[i]=accept(bind_socket_for_server,NULL,NULL);
                if(socket_server_array[i] == ERROR_VALUE) {
			printf("Error accepting socket\n");
                        exit(STATUS_ERROR);
                }
	}
}

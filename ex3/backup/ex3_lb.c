#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define STATUS_OK 0
#define BACKLOG 10
#define BUFFER_SIZE 2
#define END_OF_STR_CHAR '\0'
#define END_OF_MESSAGE_STRING "\r\n\r\n"
#define NUMBER_OF_SERVERS 3
#define TYPE_HTTP 100
#define TYPE_SERVER 101
//setsockopt(lb_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
int create_bind_listen_socket(int port_number)
{
	struct sockaddr_in service;
	int returned_socket;
	int bind_return_value,listen_return_value;
	returned_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons( port_number );
	bind_return_value = bind( returned_socket, (struct sockaddr *) &service, sizeof(service) ); 
	listen_return_value = listen(returned_socket,BACKLOG);
	//need to check for errors-- or not???
	return returned_socket; 
}

void write_port_to_file(char *filename,int port_number)
{
	FILE *file;

	file=fopen(filename,"w");
	fprintf(file,"%d",port_number);
	fclose(file);
}

int check_if_finished_recving(char *recv_buf,int recv_buffer_size,int type)
{
	int i,counter;

	if (type==TYPE_HTTP){
		counter=1;
	}else {
		counter=2;
	}
	for(i=0;i<(recv_buffer_size-3);i++)
	{
		if((recv_buf[i]=='\r')&&(recv_buf[i+1]=='\n')&&(recv_buf[i+2]=='\r')&&(recv_buf[i+3]=='\n')){
			if(counter<=1){
				return true;
			}else{
				counter--;
			}
		}
	}
	return false;
}

char * parse_the_request_or_responce(int socket_handle,int http_or_server)
{
	char *recv_string,recv_buffer[BUFFER_SIZE];
	int flag_for_recv_done=0,recv_string_size=BUFFER_SIZE;
	int bytes_recived;

	recv_string=(char*) calloc(recv_string_size,sizeof(char));
	recv_string[0]=END_OF_STR_CHAR;
	while(flag_for_recv_done == 0)
	{
		bytes_recived=recv(socket_handle,recv_buffer,sizeof(recv_buffer),0);
		recv_buffer[bytes_recived]=END_OF_STR_CHAR;
		//printf("buffer=%s ,string=%s\n",recv_buffer,recv_string);
		strcat(recv_string,recv_buffer);
		flag_for_recv_done=check_if_finished_recving(recv_string,recv_string_size,http_or_server);;
		if(flag_for_recv_done == 0)
		{
			recv_string_size=recv_string_size+BUFFER_SIZE;
			recv_string=(char*) realloc(recv_string,recv_string_size*sizeof(char));
		}
	}
	if(TYPE_HTTP)
		printf("parsed http:%s\n",recv_string);
	else
		printf("parsed server:%s\n",recv_string);
	return recv_string;
}

void accept_all_servers(int socket_server_array[NUMBER_OF_SERVERS],int socket_server_bind)
{
	int i;
	for(i=0;i<NUMBER_OF_SERVERS;i++)
	{
		socket_server_array[i]=accept(socket_server_bind,NULL,NULL);
		//printf("opened server %d\n",i);
	}
}

void send_to_socket(int socket_handle,char *parsed_string)
{
	printf("sending %s\n",parsed_string);
	send(socket_handle,parsed_string,strlen(parsed_string),0);
}

int main()
{
	time_t srand_seed;
	int port_http,port_server,server_selector=1;
	char *parsed_request_from_http,*parsed_responce_from_server;
	int socket_http_bind,socket_http;
	int socket_server_bind,socket_server_array[NUMBER_OF_SERVERS];
	int counter=0;

	srand((unsigned) time(&srand_seed));
	//move to create_bind_listen_socket???
	port_http = (rand()+1024)%64000;
	port_server = (rand()+1024)%64000;


	//not for unix
	socket_http_bind = create_bind_listen_socket(port_http);
	socket_server_bind = create_bind_listen_socket(port_server);
	write_port_to_file("http_port",port_http);
	write_port_to_file("server_port",port_server);
	accept_all_servers(socket_server_array,socket_server_bind);
	
	while(1){
		printf("start while %d LB\n",counter++);
		socket_http=accept(socket_http_bind,NULL,NULL);
		parsed_request_from_http=parse_the_request_or_responce(socket_http,TYPE_HTTP);
		printf("finished http parse-sending to server\n");
		send_to_socket(socket_server_array[server_selector],parsed_request_from_http);
		free(parsed_request_from_http);
		printf("finished sent to server- parsing server\n");
		parsed_responce_from_server=parse_the_request_or_responce(socket_server_array[server_selector],TYPE_SERVER);
		printf("finished parsed server- sending to http\n");
		send_to_socket(socket_http,parsed_responce_from_server);
		free(parsed_responce_from_server);
		server_selector=(server_selector+1)%3;
		close(socket_http);
	}
	return STATUS_OK;
}

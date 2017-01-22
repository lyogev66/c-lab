#include "messaging.h"
#include "common.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string.h>

#define END_OF_MESSAGE_STRING "\r\n\r\n"
#define BUFFER_SIZE 512
#define END_OF_STR_CHAR '\0'

int message_end_found(char *message)
{
  char *end_of_message_location;

  end_of_message_location = strstr(message, END_OF_MESSAGE_STRING);
  if (end_of_message_location != NULL) {
    return true;
  } else {
    return false;
  }
}
int check_if_finished_recving(char *message, int type_http_or_server)
{
  char *end_of_message_location;

  if (type_http_or_server == TYPE_HTTP) {
    return message_end_found(message);
  } else {
    if (message_end_found(message)) {
      end_of_message_location = strstr(message, END_OF_MESSAGE_STRING);
      return message_end_found(end_of_message_location + 1);
    }
  }
  return false;

  //	end_of_message_location=strstr(message,END_OF_MESSAGE_STRING);
  //	if (end_of_message_location != NULL){
  //		if(type_http_or_server == TYPE_HTTP){
  //			return true;
  //		}else{
  //			end_of_message_location =
  // strstr(end_of_message_location+1,END_OF_MESSAGE_STRING);
  //			if (end_of_message_location != NULL){
  //				return true;
  //			}
  //		}
  //	}
  //	return false;
}

char *parse_the_request_or_responce(int socket_handle, int http_or_server)
{
  char *recv_message, recv_buffer[BUFFER_SIZE];
  int bytes_recived, flag_for_recv_done = 0, recv_message_size = BUFFER_SIZE;

  recv_message = (char *)calloc(recv_message_size, sizeof(char));
  recv_message[0] = END_OF_STR_CHAR;
  while (flag_for_recv_done == 0) {
    bytes_recived = recv(socket_handle, recv_buffer, sizeof(recv_buffer), 0);
    if (bytes_recived == ERROR_VALUE) {
      printf("Failed reciving message\n");
      exit(STATUS_ERROR);
    }
    recv_buffer[bytes_recived] = END_OF_STR_CHAR;
    strcat(recv_message, recv_buffer);
    flag_for_recv_done = check_if_finished_recving(recv_message, http_or_server);
    if (flag_for_recv_done != true) {
      recv_message_size = recv_message_size + bytes_recived;
      recv_message = (char *)realloc(recv_message, recv_message_size * sizeof(char));
    }
  }
  return recv_message;
}

void send_to_socket(int socket_handle, char *parsed_string)
{
  int byets_sent;

  byets_sent = send(socket_handle, parsed_string, strlen(parsed_string), 0);
  if (byets_sent == ERROR_VALUE) {
    printf("Failed sending message\n");
    exit(STATUS_ERROR);
  }
}

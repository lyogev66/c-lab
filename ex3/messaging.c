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

bool is_message_end_found(char *message)
{
  char *end_of_message_location;

  end_of_message_location = strstr(message, END_OF_MESSAGE_STRING);
  if (end_of_message_location != NULL) {
    return true;
  } else {
    return false;
  }
}

bool check_if_finished_recving(char *message, int type_client_or_server)
{
  char *end_of_message_location;

  if (type_client_or_server == TYPE_CLIENT) {
    return is_message_end_found(message);
  } else {
    if (is_message_end_found(message)) {
      end_of_message_location = strstr(message, END_OF_MESSAGE_STRING);
      return is_message_end_found(end_of_message_location + 1);
    }
  }
  return false;
}

char *recv_full_request_or_responce(int socket_handle, int type_client_or_server)
{
  char *recv_message, recv_buffer[BUFFER_SIZE];
  int bytes_recived, recv_message_size = BUFFER_SIZE;
  bool flag_for_recv_done = false;

  recv_message = (char *)calloc(recv_message_size, sizeof(char));
  recv_message[0] = '\0';
  while (flag_for_recv_done == false) {
    bytes_recived = recv(socket_handle, recv_buffer, sizeof(recv_buffer), 0);
    if (bytes_recived == ERROR_VALUE) {
      printf("Failed reciving message\n");
      exit(ERROR_VALUE);
    }
    recv_buffer[bytes_recived] = '\0';
    strcat(recv_message, recv_buffer);
    flag_for_recv_done = check_if_finished_recving(recv_message, type_client_or_server);
    if (flag_for_recv_done == false) {
      recv_message_size = recv_message_size + bytes_recived;
      recv_message = (char *)realloc(recv_message, recv_message_size * sizeof(char));
    }
  }
  return recv_message;
}

void send_to_socket(int socket_handle, char *string)
{
  int byets_sent;

  byets_sent = send(socket_handle, string, strlen(string), 0);
  if (byets_sent == ERROR_VALUE) {
    printf("Failed sending message\n");
    exit(ERROR_VALUE);
  }
}

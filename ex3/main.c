#include "common.h"
#include "messaging.h"
#include "setup_sockets.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>
#include <time.h>

#define STATUS_OK 0

// should be in common.h?
#define NUMBER_OF_SERVERS 3

int main()
{
  time_t srand_seed;
  int server_selector = 1;
  char *parsed_request_from_http, *parsed_responce_from_server;
  int bind_socket_for_http, bind_socket_for_server;
  int socket_http, socket_server_array[NUMBER_OF_SERVERS];

  srand((unsigned)time(&srand_seed));
  bind_socket_for_http = setup_listening_socket(TYPE_HTTP);
  bind_socket_for_server = setup_listening_socket(TYPE_SERVER);
  accept_all_servers(socket_server_array, bind_socket_for_server);

  while (true) {
    socket_http = accept(bind_socket_for_http, NULL, NULL);
    if (socket_http == ERROR_VALUE) {
      printf("Error accepting socket\n");
      exit(STATUS_ERROR);
    }
    parsed_request_from_http = parse_the_request_or_responce(socket_http, TYPE_HTTP);
    send_to_socket(socket_server_array[server_selector], parsed_request_from_http);
    free(parsed_request_from_http);
    parsed_responce_from_server = parse_the_request_or_responce(socket_server_array[server_selector], TYPE_SERVER);
    send_to_socket(socket_http, parsed_responce_from_server);
    free(parsed_responce_from_server);
    server_selector = (server_selector + 1) % NUMBER_OF_SERVERS;
    close(socket_http);
  }
  return STATUS_OK;
}

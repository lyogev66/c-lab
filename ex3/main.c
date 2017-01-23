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

void accept_connections_and_serve_client(int socket_server_array[NUMBER_OF_SERVERS], int bind_socket_for_client)
{
  int server_selector = 0, socket_client;
  char *request_from_client, *responce_from_server;

  while (true) {
    socket_client = accept(bind_socket_for_client, NULL, NULL);
    if (socket_client == ERROR_VALUE) {
      printf("Error accepting socket\n");
      exit(ERROR_VALUE);
    }
    request_from_client = recv_full_request_or_responce(socket_client, TYPE_CLIENT);
    send_to_socket(socket_server_array[server_selector], request_from_client);
    free(request_from_client);
    responce_from_server = recv_full_request_or_responce(socket_server_array[server_selector], TYPE_SERVER);
    send_to_socket(socket_client, responce_from_server);
    free(responce_from_server);
    server_selector = (server_selector + 1) % NUMBER_OF_SERVERS;
    close(socket_client);
  }
}

int main()
{
  time_t srand_seed;

  int bind_socket_for_client, bind_socket_for_server;
  int socket_server_array[NUMBER_OF_SERVERS];

  srand((unsigned)time(&srand_seed));
  bind_socket_for_client = choose_port_and_start_listening(TYPE_CLIENT);
  bind_socket_for_server = choose_port_and_start_listening(TYPE_SERVER);
  accept_all_servers(socket_server_array, bind_socket_for_server);
  accept_connections_and_serve_client(socket_server_array, bind_socket_for_client);

  return 0;
}

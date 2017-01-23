#ifndef SETUP_SOCKETS_H
#define SETUP_SOCKETS_H

#define NUMBER_OF_SERVERS 3

int choose_port_and_start_listening(int type_client_or_server);
void accept_all_servers(int socket_server_array[NUMBER_OF_SERVERS], int bind_socket_for_server);

#endif

#ifndef SETUP_LB_H
#define SETUP_LB_H

#define NUMBER_OF_SERVERS 3

int setup_listening_socket(int type_server_or_http);
void accept_all_servers(int socket_server_array[NUMBER_OF_SERVERS], int bind_socket_for_server);

#endif

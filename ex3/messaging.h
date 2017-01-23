#ifndef MESSAGING_H
#define MESSAGING_H

void send_to_socket(int socket_handle, char *string);
char *recv_full_request_or_responce(int socket_handle, int type_client_or_server);

#endif

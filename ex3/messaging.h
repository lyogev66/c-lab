#ifndef MESSAGING_H
#define MESSAGING_H

void send_to_socket(int socket_handle, char *parsed_string);
char *parse_the_request_or_responce(int socket_handle, int http_or_server);

#endif

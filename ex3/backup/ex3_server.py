#!/usr/bin/python2.7 -tt
from socket import *
filename="server_port"
buffersize=1024
host="localhost"
response_string_for_invalid_request="HTTP/1.1 404 Not Found\r\n" \
                                    "Content-type: text/html\r\nContent-length: 135\r\n" \
                                    "<html><head><title>Not Found</title></head><body>\r\n" \
                                    "Sorry, the object you requested was not found.\r\n</body><html>"
file=open(filename,"r")
port_number=int(file.readline())
file.close()
server_socket=socket()
server_socket.connect((host,port_number))
counter=0
while (True):
    response = ""
    while (True):
        response += server_socket.recv(buffersize)
        if response.count('\r\n\r\n') == 1:
            break
    counter_word_or_not=str(response.split("\r\n")[0].split(" ")[1])
    if (counter_word_or_not=="/counter"):
        counter+=1
        response_string_for_valid_request="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-Length: %s\r\n\r\n%s\r\n\r\n" %(len(str(counter)),counter)
        server_socket.send(response_string_for_valid_request+"\r\n\r\n")
    else:
        server_socket.send(response_string_for_invalid_request)
server_socket.close()



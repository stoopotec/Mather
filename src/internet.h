#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>





struct sockaddr create_sockaddr(socklen_t* lengthr, uint16_t port);

int create_server_socket(struct sockaddr* address);


long find(const char* from, const char* what);
char* whatFileWant(char* http_header);

const char* define_content_type(const char* filename);
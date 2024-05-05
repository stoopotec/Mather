
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <signal.h>

#include "internet.h"
#include "server.h"


#define PORT 8080


int socketfd;

void close_socket(int integer) {
    close(socketfd);
    exit(EXIT_SUCCESS);
}


int main() {
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == socketfd) {
        fprintf(stderr, "ERROR: cannot create socket\n");
        exit(EXIT_FAILURE);
    }


    signal(SIGINT , (__sighandler_t)&close_socket);


    struct sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof(addr_in));
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(PORT);
    addr_in.sin_addr.s_addr = INADDR_ANY;


    if (-1 == bind(socketfd, (struct sockaddr*)&addr_in, sizeof(addr_in))) {
        fprintf(stderr, "ERROR: cannot bind\n");
        exit(EXIT_FAILURE);
    }

    if (-1 == listen(socketfd, 10)) {
        fprintf(stderr, "ERROR: cannot listen\n");
        exit(EXIT_FAILURE);
    }

    printf("INFO: server running! you can get acsex it by http://localhost:%d/index.html\n", PORT);

    while (1)
        serve_client(accept(socketfd, NULL, NULL));


    close(socketfd);



    return 0;
}


/* 
 * TODO:
 *  
 *  
 *
 *
 */




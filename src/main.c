#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "internet.h"
#include "server.h"

#define PORT 6969



int main(int argc, char** argv) {


    printf("INFO: creating socket...\n");

    socklen_t address_len;
    struct sockaddr address = create_sockaddr(&address_len, PORT);

    int server_socket = create_server_socket(&address);
    if (server_socket < 0) {
        perror("ERROR: cannot create socket\n");
        return EXIT_FAILURE;
    }

    printf("INFO: server on line! you can access in by http://localhost:6969/index.html\n");



    while (1)
        serve_client(accept(server_socket, (struct sockaddr*)&address, &address_len));


    close(server_socket);



    return 0;
}


/* 
 * TODO:
 *  
 *  
 *
 *
 */




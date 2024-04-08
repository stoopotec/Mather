

#include "internet.h"



struct sockaddr create_sockaddr(socklen_t* lengthr, uint16_t port) {

    struct sockaddr_in address;
    *lengthr = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    return *(struct sockaddr*)&address;

}


int create_server_socket(struct sockaddr* address) {
    int server_socket;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR: socket failed\n");
        return server_socket;
    }

    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("ERROR: Ошибка при установке опции SO_REUSEADDR\n");
        return -1;
    }

    printf("INFO: binding socket to INADDR_ANY\n");

    
    if (bind(server_socket, address, sizeof(*address)) < 0) {
        perror("ERROR: bind failed\n");
        return -1;
    }
    printf("INFO: done!\n");


    printf("INFO: initialization of listen...\n");

    if (listen(server_socket, 5) < 0) {
        perror("ERROR: listen failed\n");
        return -1;
    }

    printf("INFO: done!\n");

    return server_socket;
}
#include "server.h"
#include "internet.h"
#include "messages.h"

#include <stdio.h>
#include <stdlib.h>




long find(const char* from, const char* what) {
    for (size_t i = 0; from[i] != '\0'; ++i) {
        char correct = 1;
        for (size_t j = 0; what[j] != '\0'; ++j) {
            if (from[i + j] == '\0' || from[i + j] != what[j]) {
                correct = 0;
                break;
            }
        }
        if (correct) return i;
    }
    return -1l;
}

char* whatFileWant(char* http_header) {
    long from, to;
    // if ((from = find(http_header, "GET /")) == -1) return NULL;
    from = 4;

    if ((to = find(http_header + from, " ")) == -1) return NULL;
    to += from;


    char* filename = (char*)malloc((to - from + 1) * sizeof(*filename));
    filename[0] = '.';
    strncpy(filename + 1, http_header + from, to - from);
    return filename;
}




int serve_client(int socketfd) {

    if (-1 == socketfd) return -1;

    printf(INFO "new connection\n");


    size_t buffer_length;
    char* buffer = read_all_alloc(&buffer_length, socketfd);
    if (buffer == NULL) {
        fprintf(stderr, ERR "buffer is NULL\n");
        exit(EXIT_FAILURE);
    }

    printf(E_BOLD "received:" E_RESET "\n");
    printf(E_ITALIC);
    for (size_t i = 0; i < buffer_length; ++i) { putchar(buffer[i]); }
    printf(E_RESET);


    char* filename;
    if ((filename = whatFileWant(buffer)) == NULL) {
        printf(ERR "клиент прислал несуразную чушь!\n");
        close(socketfd);
        printf(INFO "session end\n\n");
        return -1;
    }
    free(buffer);

    printf(INFO "i think client want file " E_ITALIC "%s" E_RESET ", конечно мы ему дадим то, что он хочет)))\n", filename);

    send_small_file(socketfd, filename);
    free(filename);
    
    close(socketfd);
    printf(INFO "session end\n\n");

    return 0;

}








char* read_all_alloc(size_t* len, int socketfd) {
    size_t allocated = 0;
    *len = 0;
    char* data = (char*)malloc(allocated * sizeof(*data));
    do {
        allocated += 1024;
        data = (char*)realloc(data, allocated);
        ssize_t read_len = read(socketfd, data + *len, allocated - *len);
        if (-1 == read_len) return NULL;
        *len += read_len;
        if ((size_t)read_len == (allocated - *len - read_len)) break;
        if (read_len == 0) break;
        if (read_len < 0) {
            free(data);
            *len = 0;
            return NULL;
        }
    } while (0);
    return data;
}

void write_all(char* data, size_t data_len, int socketfd) {

}
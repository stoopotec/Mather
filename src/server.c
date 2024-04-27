#include "server.h"
#include "internet.h"

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
    printf("file name: \"%s\"\n", filename);
    return filename;
}




int serve_client(int socketfd) {

    if (-1 == socketfd) return -1;

    printf("INFO: new connection, will serve like web server\n");


    size_t buffer_length;
    char* buffer = read_all_alloc(&buffer_length, socketfd);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: buffer is NULL\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < buffer_length; ++i) { putchar(buffer[i]); }


    char* filename;
    if ((filename = whatFileWant(buffer)) == NULL) {
        printf("INFO: клиент прислал несуразную чушь!\n");
        close(socketfd);
        printf("INFO: session end\n\n");
        return -1;
    }

    free(buffer);



    printf("INFO: loading data...\n");

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("File \"%s\" could not open\n", filename);
        close(socketfd);
        printf("INFO: session end\n\n");
        return -1;
    }

    size_t content_index = 0;
    size_t content_len = 1024;
    char* content = (char*)malloc(content_len * sizeof(*content));


    while(!feof(file)) {
        content_index += fread(content + content_index, 1, content_len - content_index, file);
        content_len += 1024;
        content = (char*)realloc(content, content_len * sizeof(*content));
    }

    // printf("Content-Length: %lu\n", content_index);
    // printf("Content:\n---------------\n");
    // for (size_t i = 0; i < content_index; ++i) putchar(content[i]);
    // printf("\n---------------\n");

    fclose(file);

    const char* content_type;
    if (find(filename, ".html") != -1) content_type = "text/html";
    if (find(filename, ".js") != -1) content_type = "text/javascript";
    if (find(filename, ".css") != -1) content_type = "text/css";

    free(filename);


    char* response = (char*)malloc((content_index + 2048) * sizeof(*response));
    size_t response_len = sprintf(response, 
        "HTTP/1.1 200 OK\n"
        "Server: Prikol\n"
        "Content-Type: %s; charset=utf-8\n"
        "Connection: Keep-Alive\n"
        "Content-Length: %lu\n"
        "\n"
        "\n"
        "%s", content_type, content_index, content
    );
    

    send(socketfd, response, response_len, 0);


    close(socketfd);
    printf("INFO: session end\n\n");

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
        *len += read_len;
        printf("last char: %c (%d), read_len: %d\n", data[*len - 1], (int)data[*len - 1], read_len);
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
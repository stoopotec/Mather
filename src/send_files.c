#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "internet.h"
#include "messages.h"



int send_small_file(int socketfd, const char* filename) {
    if (filename == NULL) {
        printf(ERR "cannot send file. " E_ITALIC TOSTRING(filename) E_RESET" in NULL\n");
        return EXIT_FAILURE;
    }
    if (socketfd <= 0) {
        printf(ERR "cannot send file " E_ITALIC "%s" E_RESET "\n becaurse of corrupted socketfd\n", filename);
        return EXIT_FAILURE;
    }


    printf(INFO "downloading " E_ITALIC "%s" E_RESET " in memory\n", filename);


    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf(WARN "File " E_ITALIC "%s" E_RESET " could not open, sending 404 response\n", filename);
        const char* resp = 
        "HTTP/1.1 404 NOt Found\n"
        "Server: Prikol\n"
        "Connection: Close\n"
        "\n"
        "\n";
        send(socketfd, resp, sizeof(resp), 0);
        return EXIT_FAILURE;
    }

    size_t content_length = 0;
    size_t content_len = 1024;
    char* content = (char*)malloc(content_len * sizeof(*content));


    while(!feof(file)) {
        content_length += fread(content + content_length, 1, content_len - content_length, file);
        content_len += 1024;
        content = (char*)realloc(content, content_len * sizeof(*content));
    }

    // printf("Content-Length: %lu\n", content_length);
    // printf("Content:\n---------------\n");
    // for (size_t i = 0; i < content_length; ++i) putchar(content[i]);
    // printf("\n---------------\n");

    fclose(file);

    const char* content_type = define_content_type(filename);

    printf(INFO "i think " E_ITALIC TOSTRING(content_type) E_RESET " is %s\n", content_type);


    char* response = (char*)malloc((content_length + 2048) * sizeof(*response));
    size_t response_len = sprintf(response, 
        "HTTP/1.1 200 OK\n"
        "Server: Prikol\n"
        "Content-Type: %s; charset=utf-8\n"
        "Connection: Keep-Alive\n"
        "Content-Length: %lu\n"
        "\n"
        "\n"
        "%s", content_type, content_length, content
    );

    printf(INFO "response is ready\n");
    
    printf(E_RESET INFO "sending...\n");

    return send(socketfd, response, response_len, 0);

}












const char* define_content_type(const char* filename) {
    if (find(filename, ".html") != -1) return "text/html";
    if (find(filename, ".js"  ) != -1) return "text/javascript";
    if (find(filename, ".css" ) != -1) return "text/css";
    if (find(filename, ".json") != -1) return "text/json";

    return "text/txt";
}
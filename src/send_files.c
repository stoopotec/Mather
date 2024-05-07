#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "server.h"
#include "internet.h"
#include "messages.h"
#include "http.h"


const char* define_content_type(const char* filename);


int send_small_file(int socketfd, const char* filename) {
    if (filename == NULL) {
        printf(ERR "cannot send file. " E_ITALIC TOSTRING(filename) E_RESET" in NULL\n");
        return EXIT_FAILURE;
    }
    if (socketfd <= 0) {
        printf(ERR "cannot send file " E_ITALIC "%s" E_RESET "\n becaurse of corrupted socketfd\n", filename);
        return EXIT_FAILURE;
    }




    FILE* file = fopen(filename, "rb");

    struct stat file_stat;

    if (file == NULL || fstat(fileno(file), &file_stat) == -1 || S_ISDIR(file_stat.st_mode)) {
        printf(WARN "File " E_ITALIC "%s" E_RESET " could not open, sending 404 response\n", filename);
        const char* resp = 
        "HTTP/1.1 404 Not Found\n"
        "Server: Prikol\n"
        "Connection: Close\n"
        "\n"
        "\n";
        send(socketfd, resp, sizeof(resp), 0);
        return EXIT_FAILURE;
    }


    printf(INFO "downloading " E_ITALIC "%s" E_RESET " in memory\n", filename);

    char* content = (char*)malloc((file_stat.st_size + 1) * sizeof(*content));
    content[file_stat.st_size] = '\0';

    size_t actual_length_readed = fread(content, 1, file_stat.st_size, file);

    if (actual_length_readed == file_stat.st_size)
        printf(INFO "content length: %ld\n", file_stat.st_size);
    else
        printf(WARN "content length is %ld, but read only %ld\n", file_stat.st_size, actual_length_readed);

    // printf("Content-Length: %lu\n", content_length);
    // printf("Content:\n---------------\n");
    // for (size_t i = 0; i < content_length; ++i) putchar(content[i]);
    // printf("\n---------------\n");

    fclose(file);

    const char* content_type = define_content_type(filename);

    printf(INFO "i think " E_ITALIC TOSTRING(content_type) E_RESET " is %s\n", content_type);


    char* response;
    size_t response_len;

    if (0) {

        response = (char*)malloc((file_stat.st_size + 2048) * sizeof(*response));
        response_len = sprintf(response, 
            "HTTP/1.1 200 OK"
            "Vary: Origin"
            "Access-Control-Allow-Credentials: true"
            "Accept-Ranges: bytes"
            "Cache-Control: public, max-age=0"
            "Content-Type: image/svg+xml"
            "Content-Length: %ld"
            "Date: Tue, 07 May 2024 10:27:42 GMT"
            "Connection: keep-alive"
            "Keep-Alive: timeout=5"
            "\n"
            "\n"
            "%s", file_stat.st_size, content
        );
    } else {
        response = (char*)malloc((file_stat.st_size + 2048) * sizeof(*response));
        response_len = sprintf(response, 
            "HTTP/1.1 %d %s\n"
            "Server: Prikol\n"
            "Content-Type: %s\n"
            "Connection: Keep-Alive\n"
            "Content-Length: %ld\n"
            "\n"
            "\n"
            "%s", CODE_200_OK, get_code_comment(CODE_200_OK),
            content_type, file_stat.st_size, content
        );
    }

    printf(INFO "response is ready\n");
    
    if (content_type[0] == 'i' && content_type[1] == 'm' && content_type[2] == 'a' && content_type[3] == 'g') {
        printf(E_BOLD "response:\n" E_RESET E_ITALIC); 
        for (size_t i = 0; i < response_len; ++i) printf("%ld: %3d (%c); ", i, response[i], response[i]);
        printf(E_RESET "\n");
    }
    
    printf(E_RESET INFO "sending...\n");


    return send(socketfd, response, response_len, 0);

}












const char* define_content_type(const char* filename) {

    if (find(filename, ".html") != -1) return "text/html";
    if (find(filename, ".svg" ) != -1) return "image/svg+xml";
    if (find(filename, ".png" ) != -1) return "image/png";
    if (find(filename, ".jpg" ) != -1) return "image/jpeg";
    if (find(filename, ".js"  ) != -1) return "text/javascript";
    if (find(filename, ".css" ) != -1) return "text/css";
    if (find(filename, ".json") != -1) return "text/json";

    return "text/plain";
}
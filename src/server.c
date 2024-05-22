#include "server.h"
#include "internet.h"
#include "messages.h"
#include "list.h"
#include "http.h"

#include <stdio.h>
#include <stdlib.h>

#include "transformer.h"


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




int serve_client(int socketfd) {

    if (-1 == socketfd) return -1;

    printf(INFO "new connection\n");


    http_message_additional msg = get_http_message(socketfd);

    printf(E_BOLD "received:" E_RESET "\n");
    printf(E_ITALIC);
    printf("method:   %s\n", msg.message.method);
    printf("url:      %s\n", msg.message.url);
    for (size_t i = 0; i < msg.message.url_args.length; ++i) {
        printf("\t%s=%s&\n", msg.message.url_args.data[i].key, msg.message.url_args.data[i].value);
    }
    printf("http ver: %s\n", msg.message.http_version);
    printf("headers:\n");
    for (size_t i = 0; i < msg.message.headers.length; ++i) {
        printf("\t%s: %s\n", msg.message.headers.data[i].key, msg.message.headers.data[i].value);
    }
    printf("body: %s\n\n", msg.message.body);
    printf(E_RESET "\n");


    printf(INFO "i receive:\n");

    char* url = msg.message.url;

    printf("\turl: %s\n", url);

    char* filename = strcpy_a(url, "public");
    
    printf("\tpath: %s\n", filename);


    enum METHOD_E method = get_method(msg.message.method);

    printf("\tmethod: %d\n", method);


    if (method == GET) {

        printf(INFO "i think client wants my file " E_ITALIC "%s" E_RESET "\n", filename);

        send_small_file(socketfd, filename);
        

    } else if (method == CALC) {

        

        printf(INFO "client wants to calculate this silly equation " E_ITALIC "%s" E_RESET "\n", msg.message.body);

        list_transformation_t transforms = get_all_transformations_s(msg.message.body);

        char* response = calloc(100000, sizeof(char));
        char* content = calloc(100000, sizeof(char));

        size_t content_length = sprintf(content,
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<transforamtions>\n"
                "<transformation>\n"
                    "<description>%s</description>\n"
                    "<solution>%s</solution>\n"
                "</transformation>\n"
            "</transforamtions>",
            transforms.data[0].comment,
            get_string_from_symbol(transforms.data[0].equation.symbols.data[0])
        );

        size_t len = sprintf(response, 
            "HTTP/1.1 200 OK\n"
            "Content-Type: application/xml\n"
            "Content-Length: %ld\n"
            "\n"
            "%s",
            content_length,
            content
        );



        send(socketfd, response, len, 0);



        free(response);
        free(content);

    } else {

        printf(WARN "клиент прислал гадость");
        const char* resp = 
        "HTTP/1.1 501 Not Implemented\n"
        "Connection: Close\n"
        "\n"
        "\n";
        send(socketfd, resp, sizeof(resp), 0);

    }
    
    free(filename);
    free(msg.alloced_raw_content);
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
        data = (char*)realloc(data, allocated + 1);
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
    data[*len] = '\0';
    return data;
}

void write_all(char* data, size_t data_len, int socketfd) {

}
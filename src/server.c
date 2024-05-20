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

    // printf("\targs: %s\n", (args.length == 0) ? "(args.length == 0)" : "");

    // for (size_t i = 0; i < args.length; ++i) {
    //     printf("\t\t\'");

    //     for (size_t j = 0; j < args.data[i].key_len; ++j) putchar(args.data[i].key[j]);
        
    //     printf("\': \'");

    //     for (size_t j = 0; j < args.data[i].val_len; ++j) putchar(args.data[i].val[j]);
        
    //     printf("\'\n");
    // }


    enum METHOD_E method = get_method(msg.message.method);

    printf("\tmethod: %d\n", method);


    if (method == GET) {

        printf(INFO "i think client wants my file " E_ITALIC "%s" E_RESET "\n", filename);

        send_small_file(socketfd, filename);
        

    } else if (method == CALC) {

        

        printf(INFO "client wants to calculate this silly equation " E_ITALIC "%s" E_RESET "\n", msg.message.body);

        printf(WARN "отправляю заглушку\n");
        const char* resp = 
            "HTTP/1.1 200 OK\n"
            "Server: Prikol\n"
            "Connection: keep-alive\n"
            "Content-Type: application/xml\n"
            "Content-Length: 13\n"
            "\n"
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<div classname=\"transition-list\"><div classname=\"transition\"><div classname=\"description\">описание</div><div classname=\"equation\">"
            "$${\\displaystyle 1+{\\frac {1}{1\\cdot 3}}+{\\frac {1}{1\\cdot 3\\cdot 5}}+{\\frac {1}{1\\cdot 3\\cdot 5\\cdot 7}}+{\\frac {1}{1\\cdot 3\\cdot 5\\cdot 7\\cdot 9}}+\\ldots +{\\frac {1}{1+\\displaystyle {\\frac {1}{1+\\displaystyle {\\frac {2}{1+\\displaystyle {\\frac {3}{1+\\displaystyle {\\frac {4}{1+\\displaystyle {\\frac {5}{1+\\ldots }}}}}}}}}}}}={\\sqrt {\\frac {e\\cdot \\pi }{2}}}}.$$"
            "</div></div></div>";
        send(socketfd, resp, sizeof(resp), 0);

    } else {

        printf(WARN "клиент прислал гадость");
        const char* resp = 
        "HTTP/1.1 501 Not Implemented\n"
        "Server: Prikol\n"
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
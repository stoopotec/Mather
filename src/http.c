#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "internet.h"
#include "http.h"
#include "messages.h"


#define CHUNK_SIZE 1024
// #define RECEIVE_DELAY


int strcmp_start(const char* s1, const char* s2) {
    for (size_t i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i)
        if (s1[i] != s2[i]) return 0;
    return 1;
}

enum METHOD_E get_method(char* http_header) {
    if (strcmp_start(http_header, TOSTRING(GET) )) return GET;
    if (strcmp_start(http_header, TOSTRING(POST))) return POST;
    if (strcmp_start(http_header, TOSTRING(CALC))) return CALC;
    return UNDEFINED_METHOD;
}





char* strcpy_a(const char* url, const char* add_to_start) {
    LIST(char) path = {0};
    if (add_to_start != NULL) {
        for (; *add_to_start != '\0'; ++add_to_start) {
            LIST_APPEND(*add_to_start, path);
        }
    }
    for (; *url != '?' && *url != '\0'; ++url) {
        LIST_APPEND(*url, path);
    }
    LIST_APPEND('\0', path);
    return path.data;
}


list_url_arg_t get_url_args(const char* url) {
    list_url_arg_t args = {0};
    for (; *url != '?' && *url != '\0'; ++url) {}
    if (*url == '\0') return args;

    url += 1;


    url_arg_t null_arg = {.key = NULL, .key_len = 0, .val = NULL, .val_len = 0};
    LIST_APPEND(null_arg, args);

    args.data[args.length-1].key = url;


    for (size_t i = 0; url[i] != '\0'; ++i) {
        if (url[i] == '&') {
            LIST_APPEND(null_arg, args);
            args.data[args.length-2].val_len = url + i - args.data[args.length-2].val;
            args.data[args.length-1].key = url + i + 1;
            continue;
        }
        if (url[i] == '=') {
            args.data[args.length-1].key_len = url + i - args.data[args.length-1].key;
            args.data[args.length-1].val = url + i + 1;
            continue;
        }
        if (url[i+1] == '\0') {
            args.data[args.length-1].val_len = url + i+1 - args.data[args.length-1].val;
        }
    }

    return args;
}


struct http_message server_response_custom(enum HTTP_CODES_E status_code, char* body) {
    return (struct http_message) {
        .http_version = "HTTP/1.1",
        .status_code = {
            '0' + (status_code % 1000) / 100,
            '0' + (status_code % 100) / 10,
            '0' + (status_code % 10) / 1,
            '\0',
        },
        .status_description = get_code_comment(status_code),
        .method = NULL,
        .url = NULL,
        .url_args = {0},
        .headers = {0},
        .body = body,
    };
}

struct http_message server_response_default() {
    return (struct http_message) {
        .http_version = "HTTP/1.1",
        .status_code = "200",
        .status_description = get_code_comment(CODE_200_OK),
        .method = NULL,
        .url = NULL,
        .url_args = {0},
        .headers = {0},
        .body = NULL,
    };
}

struct http_message server_response_notfound() {
    return (struct http_message) {
        .http_version = "HTTP/1.1",
        .status_code = "404",
        .status_description = get_code_comment(CODE_404_NOT_FOUND),
        .method = NULL,
        .url = NULL,
        .url_args = {0},
        .headers = {0},
        .body = 
"        <!DOCTYPE html>\n"
"        <html lang=\"en\">\n"
"        <head>\n"
"            <meta charset=\"UTF-8\">\n"
"            <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
"            <title>not found :(</title>\n"
"        </head>\n"
"            <body>\n"
"                <div style=\"width: 100vw; height: 100vh; text-align: center;\">\n"
"                    <h1>requested page not found</h1>\n"
"                </div>\n"
"            </body>\n"
"        </html>\n",
    };
}

void send_http_message(int sockfd, struct http_message* message) {
    
    send(sockfd, message->http_version, 7, MSG_MORE);
    send(sockfd, " ", 1, MSG_MORE);
    send(sockfd, message->http_version, 3, MSG_MORE);
    send(sockfd, " ", 1, MSG_MORE);
    send(sockfd, message->http_version, strlen(message->http_version), MSG_MORE);
    send(sockfd, "\n", 1, MSG_MORE);

    for (size_t i = 0; i < message->headers.length; ++i) {
        send(sockfd, message->headers.data[i].key, 1, MSG_MORE);
        send(sockfd, ": ", 2, MSG_MORE);
        send(sockfd, message->headers.data[i].value, 1, MSG_MORE);
        send(sockfd, "\n", 1, MSG_MORE);
    }
    send(sockfd, "\n", 1, MSG_MORE);
    
    send(sockfd, message->body, strlen(message->body), MSG_WAITALL);
}

void next_word_and(char* text, char** word, size_t* word_len, const char* separators) {
    *word = text;
    *word_len = 0;
    for (size_t i = 0; text[i] != '\0'; ++i) {
        *word_len = i;
        for (size_t s = 0; separators[s] != '\0'; ++s) {
            if (text[i+s] != separators[s]) {
                goto ce;
            }
        }
        return;
        ce:
    }
    *word_len += 1;
    return;
}

// 's' in 'ors' stands for "skip first separators if there are any"
void next_word_ands(char* text, char** word, size_t* word_len, const char* separators) {

    for (; *text != '\0'; ++text) {
        for (size_t s = 0; separators[s] != '\0'; ++s) {
            if (separators[s] == *text) goto te;
        }
        break;
        te:
    }

    return next_word_and(text, word, word_len, separators);
}


void next_word_or(char* text, char** word, size_t* word_len, const char* separators) {
    *word = text;
    *word_len = 0;
    for (size_t i = 0; text[i] != '\0'; ++i) {
        *word_len = i;
        for (size_t s = 0; separators[s] != '\0'; ++s) {
            if (text[i] == separators[s]) {
                return;
            }
        }
    }
    *word_len += 1;
    return;
}

// 's' in 'ors' stands for "skip first separators if there are any"
void next_word_ors(char* text, char** word, size_t* word_len, const char* separators) {

    for (; *text != '\0'; ++text) {
        for (size_t s = 0; separators[s] != '\0'; ++s) {
            if (separators[s] == *text) goto te;
        }
        break;
        te:
    }

    return next_word_or(text, word, word_len, separators);
}

void sget_http_message(char* text, struct http_message* message) { // WARN: я, когда изменяю text (прибавляю к нему штуки), я не проверяю его на нулевой символ

    size_t word_size;

    next_word_ors(text, &message->method, &word_size, " ");
    message->method[word_size] = '\0';
    text = message->method + word_size + 1;

    next_word_ors(text, &message->url, &word_size, " ");
    message->url[word_size] = '\0';
    text = message->url + word_size + 1;

    do {
        char* url = message->url;
        for (; *url != '?' && *url != '\0'; ++url) {}
        if (*url == '\0') break;


        for (size_t i = 0; url[i] != '\0'; ++i) {
            if (url[i] == '&' || url[i] == '?') {
                url[i] = '\0';
                if (url[i+1] != '\0') {
                    LIST_EXPAND_BY(message->url_args, 1);
                    message->url_args.data[message->url_args.length-1].key = url + i + 1;
                    message->url_args.data[message->url_args.length-1].value = NULL;
                }
                continue;
            }
            if (url[i] == '=') {
                url[i] = '\0';
                if (url[i+1] != '\0') {
                    message->url_args.data[message->url_args.length-1].value = url + i + 1;
                }
                continue;
            }
        }

    } while (0);

    next_word_ors(text, &message->http_version, &word_size, " ");
    message->http_version[word_size] = '\0';
    text = message->http_version + word_size + 2;


    for (; *text != '\r' && *text != '\0';) {
        LIST_EXPAND_BY(message->headers, 1);
        next_word_ors(text, &message->headers.data[message->headers.length].key, &word_size, ":");
        message->headers.data[message->headers.length].key[word_size] = '\0';
        text = message->headers.data[message->headers.length].key + word_size + 2;

        next_word_ors(text, &message->headers.data[message->headers.length].value, &word_size, "\r");
        message->headers.data[message->headers.length].value[word_size] = '\0';
        text = message->headers.data[message->headers.length].value + word_size + 2;

        message->headers.length += 1;
    }

    message->body = text + 2;

    return;
}

http_message_additional get_http_message(int sockfd) {

    printf(INFO E_FOREGROUND_RED "get_http_message\n" E_RESET);
    http_message_additional r = {0};

    #if defined(RECEIVE_DELAY)
        struct timespec ts;
        ts.tv_sec = 2;
        ts.tv_nsec = 100000000;
        printf("Начало задержки\n");
        nanosleep(&ts, NULL);
        printf("Конец задержки\n");
    #endif
    
    size_t data_size = 0;
    {
        size_t buffer_size = 0;
        ssize_t bytes_received;

        do {
            buffer_size += CHUNK_SIZE;
            r.alloced_raw_content = realloc(r.alloced_raw_content, buffer_size);
            if (r.alloced_raw_content == NULL) {
                perror(ERR "buy more ram, lol\n");
                exit(EXIT_FAILURE);
            }

            bytes_received = recv(sockfd, r.alloced_raw_content + data_size, CHUNK_SIZE, 0);
            if (bytes_received < 0) {
                perror(ERR "Error receiving data from socket\n");
                exit(EXIT_FAILURE);
            }

            data_size += bytes_received;
        } while (bytes_received == CHUNK_SIZE);

        // Resize buffer to actual data size
        r.alloced_raw_content = realloc(r.alloced_raw_content, data_size + 1);
        if (r.alloced_raw_content == NULL) {
            perror(ERR "buy more ram, lol\n");
            exit(EXIT_FAILURE);
        }
        r.alloced_raw_content[data_size] = '\0'; // Null-terminate the string
    }

    printf(INFO "raw content:\n" E_ITALIC "%s" E_RESET "\n", r.alloced_raw_content);

    sget_http_message(r.alloced_raw_content, &r.message);

    return r;
}


const char* get_code_comment(enum HTTP_CODES_E code) {

    switch (code) {
        
        case 100: return "Continue";
        case 101: return "Switching Protocols";
        case 102: return "Processing";
        case 103: return "Early Hints";

        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 207: return "Multi-Status";
        case 208: return "Already Reported";
        case 226: return "IM Used";

        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 306: return "unused";
        case 307: return "Temporary Redirect";
        case 308: return "Permanent Redirect";

        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Payload Too Large";
        case 414: return "URI Too Long";
        case 415: return "Unsupported Media Type";
        case 416: return "Range Not Satisfiable";
        case 417: return "Expectation Failed";
        case 418: return "I\'m a teapot";
        case 421: return "Misdirected Request";
        case 422: return "Unprocessable Content";
        case 423: return "Locked";
        case 424: return "Failed Dependency";
        case 425: return "Too Early";
        case 426: return "Upgrade Required";
        case 428: return "Precondition Required";
        case 429: return "Too Many Requests";
        case 431: return "Request Header Fields Too Large";
        case 451: return "Unavailable For Legal Reasons";

        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";
        case 506: return "Variant Also Negotiates";
        case 507: return "Insufficient Storage";
        case 508: return "Loop Detected";
        case 510: return "Not Extended";
        case 511: return "Network Authentication Required";

    }

}
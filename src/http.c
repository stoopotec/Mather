#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "internet.h"
#include "http.h"
#include "messages.h"


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



char* get_url_alloc(char* http) {
    size_t spaces = 0;
    for (; *http != '\0'; ++http) {
        if (spaces >= 1) break;
        if (*http == ' ') ++spaces;
    }

    LIST(char) url = {0};

    for (; *http != ' ' && *http != '\0'; ++http) {
        LIST_APPEND(*http, url, char)
    }

    LIST_APPEND('\0', url, char)

    return url.data;
}

char* get_path_alloc(char* url, const char* add_to_start) {
    LIST(char) path = {0};
    if (add_to_start != NULL) {
        for (; *add_to_start != '\0'; ++add_to_start) {
            LIST_APPEND(*add_to_start, path, char)
        }
    }
    for (; *url != '?' && *url != '\0'; ++url) {
        LIST_APPEND(*url, path, char);
    }
    LIST_APPEND('\0', path, char);
    return path.data;
}


list_url_arg_t get_url_args(const char* url) {
    list_url_arg_t args = {0};
    for (; *url != '?' && *url != '\0'; ++url) {}
    if (*url == '\0') return args;

    url += 1;


    url_arg_t null_arg = {.key = NULL, .key_len = 0, .val = NULL, .val_len = 0};
    LIST_APPEND(null_arg, args, url_arg_t);

    args.data[args.length-1].key = url;


    for (size_t i = 0; url[i] != '\0'; ++i) {
        if (url[i] == '&') {
            LIST_APPEND(null_arg, args, url_arg_t);
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
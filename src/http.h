#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "list.h"




enum METHOD_E {
    UNDEFINED_METHOD = 0,
    GET  = 1,
    POST = 2,
    CALC = 3,
};

enum METHOD_E get_method(char* http_header);

const char* define_content_type(const char* filename);


typedef struct url_arg {
    const char* key;
    size_t key_len;
    const char* val;
    size_t val_len;
} url_arg_t;


typedef LIST(url_arg_t) list_url_arg_t;
char* get_url_alloc(char* http);

list_url_arg_t get_url_args(const char* url);

char* get_path_alloc(char* url, const char* add_to_start);

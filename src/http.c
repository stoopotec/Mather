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
        LIST_APPEND(*http, url);
    }

    LIST_APPEND('\0', url);

    return url.data;
}

char* get_path_alloc(char* url, const char* add_to_start) {
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

        default: return "NULL";

    }

}
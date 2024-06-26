#pragma once

#include <string.h>


#ifndef LIST_MALLOC_
    #include <stdlib.h>
    #define LIST_MALLOC_ malloc
#endif

#ifndef LIST_CALLOC_
    #include <stdlib.h>
    #define LIST_CALLOC_ calloc
#endif

#ifndef LIST_REALLOC_
    #include <stdlib.h>
    #define LIST_REALLOC_ realloc
#endif

#ifndef LIST_FREE_
    #include <stdlib.h>
    #define LIST_FREE_ free
#endif


#define APPEND_ALLOC_LENGTH ((size_t)10)


#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define SWAP(a, b, data_type) {\
    data_type x = a;\
    a = b;\
    b = x;\
}\


#define LIST_FREE(list) (LIST_FREE_((list).data))


#define LIST_EXPAND_BY(list, expand_by) {\
    list.alloc_length += expand_by;\
    list.data = LIST_REALLOC_((list.data), list.alloc_length * sizeof(*(list.data)));\
}\

#define LIST_EXPAND_TO(list, expand_to) {\
    list.alloc_length = MAX(list.alloc_length, expand_to), \
    list.data = LIST_REALLOC_((list.data), list.alloc_length * sizeof(*(list.data)));\
}\

#define LIST_INIT(list) {\
    list.alloc_length = LIST_ALLOC_LENGTH;\
    list.length = 0;\
    list.data = LIST_CALLOC_(list.alloc_length, sizeof(*list.data));\
}\

#define LIST_INIT_LEN(list, alloc_len) {\
    list.alloc_length = alloc_len;\
    list.length = 0;\
    list.data = LIST_CALLOC_(list.alloc_length, sizeof(*list.data));\
}\


#define LIST_APPEND(x, list) {\
    if (list.data == NULL) {\
        list.alloc_length = APPEND_ALLOC_LENGTH;\
        list.length = 0;\
        list.data = LIST_MALLOC_(list.alloc_length * sizeof(*(list.data)));\
    }\
    if (list.length >= list.alloc_length) {\
        LIST_EXPAND_BY(list, APPEND_ALLOC_LENGTH)\
    }\
    list.data[list.length++] = x;\
}

#define LIST_APPEND_RANGE(x, x_length, list) if (x != NULL) {\
    if (list.data == NULL) {\
        list.alloc_length = MAX(APPEND_ALLOC_LENGTH, x_length);\
        list.length = 0;\
        list.data = LIST_MALLOC_(list.alloc_length * sizeof(*(list.data)));\
    }\
    if (list.length + x_length - 1 >= list.alloc_length) {\
        LIST_EXPAND_BY(list, APPEND_ALLOC_LENGTH + x_length)\
    }\
    memcpy(list.data + list.length, x, x_length * sizeof(*list.data));\
    list.length += x_length;\
}\


#define LIST_PUSH_AT(x, list, index) {\
    if (list.data == NULL) {\
        list.alloc_length = APPEND_ALLOC_LENGTH;\
        list.length = 0;\
        list.data = LIST_MALLOC_(list.alloc_length * sizeof(*(list.data)));\
    }\
    if (index < list.length) {\
        LIST_APPEND(list.data[list.length - 1], list)\
        for (int i = list.length-2; i >= index && i < list.length; --i)\
            list.data[i+1] = list.data[i];\
        list.data[index] = x;\
    }\
    else LIST_APPEND(x, list)\
}

#define LIST_PUSH_RANGE_AT(x, x_length, list, index) if (x != NULL) {\
    if (list.data == NULL) {\
        list.alloc_length = APPEND_ALLOC_LENGTH;\
        list.length = 0;\
        list.data = LIST_MALLOC_(list.alloc_length * sizeof(*(list.data)));\
    }\
    if (index < list.length) {\
        if (index + x_length >= list.alloc_length || 1) {\
            LIST_EXPAND_BY(list, x_length)\
        }\
        for (int i = list.length - 1; i >= index; --i)\
            list.data[i+x_length] = list.data[i];\
        list.length += x_length;\
        memcpy(list.data + index, x, x_length * sizeof(*list.data));\
    }\
    else LIST_APPEND_RANGE(x, x_length, list)\
}\

#define LIST(type) struct list_##type {\
    type* data;\
    size_t length;\
    size_t alloc_length;\
}

#define LIST_ANON(type) struct {\
    type* data;\
    size_t length;\
    size_t alloc_length;\
}



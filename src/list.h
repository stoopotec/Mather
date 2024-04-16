#pragma once

#define LIST_MALLOC_ malloc
#define LIST_REALLOC_ realloc
#define LIST_FREE_ free

#define APPEND_ALLOC_LENGTH ((size_t)10)


#define LIST_FREE(list) (LIST_FREE_((list).data))

#define LIST_APPEND(x, list, data_type) {\
    if ((list).data == NULL) {\
        (list).alloc_length = APPEND_ALLOC_LENGTH;\
        (list).length = 0;\
        (list).data = (data_type*)LIST_MALLOC_((list).alloc_length * sizeof(*((list).data)));\
    }\
    if ((list).length >= (list).alloc_length) {\
        (list).alloc_length += APPEND_ALLOC_LENGTH;\
        (list).data = (data_type*)LIST_REALLOC_(((list).data), (list).alloc_length * sizeof(*((list).data)));\
    }\
    ((list)).data[((list)).length++] = (x);\
}

#define LIST(type) struct list_##type {\
    type* data;\
    size_t length;\
    size_t alloc_length;\
}



#pragma once

#include <stdlib.h>
#include "list.h"



enum symbol_type {

    NULL_SYMBOL                 = 0b000000000000000000000000000000000000000000000000,


    IMPLIES                     = 0b100000000000000000000000000000000000101100000001,
    EQUIVALENT                  = 0b100000000000000000000000000000000000100100000010,
    NOT                         = 0b100000000000000000000000000000000000000100000011,
    AND                         = 0b100000000000000000000000000000000000100000000100,
    OR                          = 0b100000000000000000000000000000000000111000000101,
    XOR                         = 0b100000000000000000000000000000000000011000000110,

    TRUE                        = 0b010000000000000000000000000000000000000100000001,
    FALSE                       = 0b010000000000000000000000000000000000000000000000,

    FORALL                      = 0b110000000000000000000000000000000000000000000001,
    EXIST                       = 0b110000000000000000000000000000000000000000000010,
    // NOEXIST                     = 0b110000000000000000000000000000000000000000000011,

    BRACE_OPEN_ROUND            = 0b001000000000000100000000000000000000000000000000,
    BRACE_CLOSE_ROUND           = 0b001000000000000100000000000000000000000000000001,

    // BRACE_OPEN_FIGURE           = 0b101000000000000000000000000000000000000000000000,
    // BRACE_CLOSE_FIGURE          = 0b101000000000000000000000000000000000000000000001,



    VARIABLE                    = 0b101000000000000000000000000000000000000000000000,

    // PLUS                        = 0b000000000000000000000000000000000000000000000000,
    // MINUS                       = 0b000000000000000000000000000000000000000000000000,

    // MULTIPLY                    = 0b000000000000000000000000000000000000000000000000,
    // DIVIDE                      = 0b000000000000000000000000000000000000000000000000,

};

struct symbol {
    enum symbol_type type;

    const char* text;
    size_t text_len;
};
typedef struct symbol symbol_t;


struct equation {
    LIST(symbol_t) symbols;
};



const char* get_string_from_symbol_type(enum symbol_type type);


struct symbol   get_next_symbol(const char* string, size_t* spaces, size_t* text_len);
struct equation get_equation_from_string(const char* string);


struct equation get_all_transformations_s(size_t* transformations, const char* string);
struct equation get_all_transformations(size_t* transformations, struct equation* equation);












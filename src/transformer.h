#pragma once

#include <stdlib.h>
#include "list.h"



enum symbol_type {

    NULL_SYMBOL                 = 0b0000000000000000000000000000000000000000000000000000000000000000,


    IMPLIES                     = 0b1000000000000000000000000000000000001011000000010000001000000001,
    EQUIVALENT                  = 0b1000000000000000000000000000000000001001000000010000001000000010,
    NOT                         = 0b1000000000000000000000000000000000000001000001000000000100000011,
    AND                         = 0b1000000000000000000000000000000000001000000000010000001000000100,
    OR                          = 0b1000000000000000000000000000000000001110000000010000001000000101,
    XOR                         = 0b1000000000000000000000000000000000000110000000010000001000000110,

    TRUE                        = 0b0100000000000000000000000000000000000000000000000000000100000001,
    FALSE                       = 0b0100000000000000000000000000000000000000000000000000000000000000,

    FORALL                      = 0b0010000000000000000000000000000000000000000001000000000100000001,
    EXIST                       = 0b0010000000000000000000000000000000000000000001000000000100000010,
    // NOEXIST                     = 0b0000001100000000000000000000000000000000000000000000000000000011,

    BRACE_OPEN_ROUND            = 0b0001000000000001000000000000000000000000000000000000000000000000,
    BRACE_CLOSE_ROUND           = 0b0001000000000001000000000000000000000000000000000000000000000001,

    // BRACE_OPEN_FIGURE           = 0b0000010100000000000000000000000000000000000000000000000000000000,
    // BRACE_CLOSE_FIGURE          = 0b0000010100000000000000000000000000000000000000000000000000000001,



    VARIABLE                    = 0b0000100000000000000000000000000000000000000000000000000000000000,
    NUMBER                      = 0b0000100000000000000000000000000000000000000000000000000000000001,

    OPERATOR                    = 0b1010010000000000000000000000000000000000000000000000000000000000,

    ALGEBRAIC_OPERATOR          = 0b0000010000000000000000000000000000000000000000000000000000000000,

    PLUS                        = 0b0000010000000000000000000000000000000000000000010000001000000000,
    MINUS                       = 0b0000010000000000000000000000000000000000000000010000001000000001,

    MULTIPLY                    = 0b0000010000000000000000000000000000000000000000100000001000000010,
    DIVIDE                      = 0b0000010000000000000000000000000000000000000000100000001000000011,

    NEGATE                      = 0b0000010000000000000000000000000000000000000000110000000100000100,
};

struct symbol {
    enum symbol_type type;

    const char* text;
    size_t text_len;
};
typedef struct symbol symbol_t;

LIST(symbol_t);

struct equation {
    struct list_symbol_t symbols;
};

typedef struct equation equation_t;




struct equation_tree {
    symbol_t symbol;
    struct list_equation_tree_t {
        struct equation_tree* data;
        size_t length;
        size_t alloc_length;
    } sub_equations;
    int filled;
};

#define NULL_TREE {\
    .symbol = NULL_SYMBOL,\
    .sub_equations = {0},\
    .filled = 0\
}

void add_symbol_to_equation_tree(symbol_t symbol, struct equation_tree* tree);
void print_equation_tree(struct equation_tree* tree, size_t zindex);


const char* get_string_from_symbol_type(enum symbol_type type);
const char* get_string_from_symbol(struct symbol symbol);


struct symbol   get_next_symbol(const char* string, size_t* spaces, size_t* text_len);
struct equation get_equation_from_string(const char* string);


struct equation  to_reverse_polish(struct equation eq);


struct equation* get_all_transformations_s(const char* string, size_t* transformations);
struct equation* get_all_transformations(struct equation* equation, size_t* transformations);












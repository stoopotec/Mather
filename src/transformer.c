
#include <stdio.h>
#include "transformer.h"


struct symbol_kv {
    const char* key;
    enum symbol_type value;
};

struct symbol_kv symbols_table[] = {

    // { "NULL"               , NULL_SYMBOL }, 



    { "⇒"                 , IMPLIES },
    { "\\rightarrow"       , IMPLIES },
    { "\\Rightarrow"       , IMPLIES },
    { "\\imp"              , IMPLIES },

    { "⇔"                 , EQUIVALENT },
    { "\\eq"               , EQUIVALENT },

    { "¬"                  , NOT },
    { "\\not"              , NOT },

    { "∧"                  , AND },
    { "\\and"              , AND },
    
    { "∨"                  , OR },
    { "\\or"               , OR },
    
    { "⊻"                  , XOR },
    { "\\xor"              , XOR },



    { "⊤"                  , TRUE },
    { "\\top"              , TRUE },
    { "\\true"             , TRUE },

    { "⊥"                  , FALSE },
    { "\\bot"              , FALSE },
    { "\\false"            , FALSE },



    { "∀"                  , FORALL },
    { "\\forall"           , FORALL },

    { "∃"                  , EXIST },
    { "\\exist"            , EXIST },



    { "("                  , BRACE_OPEN_ROUND },
    { ")"                  , BRACE_CLOSE_ROUND },

};
#define LEN(x) (sizeof(x) / sizeof(*(x)))


const char* get_string_from_symbol_type(enum symbol_type type) {
    for (size_t i = 0; i < LEN(symbols_table); ++i) {
        if (symbols_table[i].value == type) return symbols_table[i].key;
    }
    return "NULL";
}



struct symbol   get_next_symbol(const char* string, size_t* use_to) {
    size_t start = 0, text_len = 0;

    for (; string[start] != '\0'; ++start) 
        if (
            string[start] != ' ' && 
            string[start] != '\t' && 
            string[start] != '\n' 
        ) break;


    for (size_t i = 0; i < LEN(symbols_table); ++i) {
        int accept = 1;
        text_len = 0;
        for (size_t j = 0; symbols_table[i].key[j] != '\0'; ++j) {
            if (symbols_table[i].key[j] != string[start + j]) {
                accept = 0;
                break;
            }
            if (symbols_table[i].key[j + 1] == '\0') text_len = j + 1;
        }
        if (accept) {
            struct symbol symb = {
                .type = symbols_table[i].value,
                .text = string + start,
                .text_len = text_len,
            };
            *use_to = start + text_len;
            return symb;
        };
    }

    struct symbol symb = {
        .type = NULL_SYMBOL,
        .text = NULL,
        .text_len = 0,
    };
    *use_to = start;
    return symb;
}


struct equation get_equation_from_string(const char* string) {

    struct equation eq = {0};

    size_t use_to = 0;

    for (size_t i = 0; string[i] != '\0'; i += use_to) {
        struct symbol next_symbol = get_next_symbol(string + i, &use_to);

        printf("nextw symblol: %ld (%s), use_to: %ld, text_len: %ld\n", 
            next_symbol.type, 
            get_string_from_symbol_type(next_symbol.type), 
            use_to, 
            next_symbol.text_len
        );

        if (next_symbol.type) {
            LIST_APPEND(next_symbol, eq.symbols, struct symbol);
        }
        else use_to = 1;
    }


    return eq;

}


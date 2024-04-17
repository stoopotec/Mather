
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


int is_word_symbol(char c) {
    return (
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') // ||
        // (c >= '0' && c <= '1') ||
        // (c == '_')
    );
}

const char*     get_next_word(const char* string, size_t* spaces, size_t* word_len) {

    for (*spaces = 0; string[(*spaces)] != '\0'; ++(*spaces)) 
        if (
            string[(*spaces)] !=  ' ' && 
            string[(*spaces)] != '\t' && 
            string[(*spaces)] != '\n' 
        ) break;
    string = string + *spaces;



    for (*word_len = 0; is_word_symbol(string[*word_len]); ++(*word_len)) {}

    return string;

}







struct symbol   get_next_symbol(const char* string, size_t* spaces, size_t* text_len) {

    for (*spaces = 0; string[(*spaces)] != '\0'; ++(*spaces)) 
        if (
            string[(*spaces)] !=  ' ' && 
            string[(*spaces)] != '\t' && 
            string[(*spaces)] != '\n' 
        ) break;
    string = string + *spaces;

    for (size_t i = 0; i < LEN(symbols_table); ++i) {
        int accept = 1;
        *text_len = 0;
        for (size_t j = 0; symbols_table[i].key[j] != '\0'; ++j) {
            if (symbols_table[i].key[j] != string[j]) {
                accept = 0;
                break;
            }
            if (symbols_table[i].key[j + 1] == '\0') *text_len = j + 1;
        }
        if (accept) {
            struct symbol symb = {
                .type = symbols_table[i].value,
                .text = string,
                .text_len = *text_len,
            };
            return symb;
        };
    }

    struct symbol symb = {
        .type = NULL_SYMBOL,
        .text = NULL,
        .text_len = 0,
    };
    return symb;
}


struct equation get_equation_from_string(const char* string) {

    struct equation eq = {0};

    size_t spaces = 0;
    size_t word_length = 0;

    for (size_t i = 0; string[i] != '\0'; i += (spaces + word_length)) {
        struct symbol next_symbol = get_next_symbol(string + i, &spaces, &word_length);

        printf("nextw symblol: %ld (%s), spaces: %ld, word_length: %ld\n", 
            next_symbol.type, 
            get_string_from_symbol_type(next_symbol.type), 
            spaces, 
            word_length
        );

        if (next_symbol.type) {
            LIST_APPEND(next_symbol, eq.symbols, struct symbol);
        }
        else spaces = 1;
    }


    return eq;

}


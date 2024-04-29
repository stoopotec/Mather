
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



    { "+"                  , PLUS },
    { "-"                  , MINUS },
    { "*"                  , MULTIPLY },
    { "/"                  , DIVIDE },

};
#define LEN(x) (sizeof(x) / sizeof(*(x)))


const char* get_string_from_symbol_type(enum symbol_type type) {
    for (size_t i = 0; i < LEN(symbols_table); ++i) {
        if (symbols_table[i].value == type) return symbols_table[i].key;
    }
    if (type == VARIABLE) return "VARIABLE";
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


    size_t word_spaces = 0;
    const char* nw = get_next_word(string, &word_spaces, text_len);

    if (*text_len != 0) {
        symb.text = nw;
        symb.text_len = *text_len;
        symb.type = VARIABLE;
        *spaces = *spaces + word_spaces;
        return symb;
    }



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





unsigned char get_operation_proirity(enum symbol_type symb) {
    return (symb >> (8*2)) & 0b11111111;
}


struct list_symbol_t braces_to_reverse_polish(const struct list_symbol_t symbols, size_t from, size_t* processed_count) {

    struct list_symbol_t symbols_r = {0};
    size_t where_paste = symbols_r.length; // which is 0
    unsigned char previous_braces = 0;
    
    for (size_t i = from; i < symbols.length; ++i) {
        if (symbols.data[i].type & VARIABLE) { printf("RP: %3ld [[ %s ]]\n", i, get_string_from_symbol_type(symbols.data[i].type));
            LIST_PUSH_AT(symbols.data[i], symbols_r, where_paste, symbol_t);
            where_paste = symbols_r.length;
            continue;
        }
        if (symbols.data[i].type & ALGEBRAIC_OPERATOR) { printf("RP: %3ld [[ %s ]]\n", i, get_string_from_symbol_type(symbols.data[i].type));
            LIST_APPEND(symbols.data[i], symbols_r, symbol_t);
            where_paste = symbols_r.length - 1; // useless

            // printf("RP: SWAP? %s and %s\n", 
            //     (symbols_r.data[symbols_r.length-2].type & ALGEBRAIC_OPERATOR) ? "true" : "false",
            //     (get_operation_proirity(symbols_r.data[symbols_r.length-2].type) < get_operation_proirity(symbols_r.data[symbols_r.length-1].type)) ? "true" : "false"
            // );
            if ((symbols_r.data[symbols_r.length-2].type & ALGEBRAIC_OPERATOR) 
                && (get_operation_proirity(symbols_r.data[symbols_r.length-2].type) < get_operation_proirity(symbols_r.data[symbols_r.length-1].type))
                && !previous_braces
            )
            {
                printf("RP: SWAP!\n");
                SWAP(symbols_r.data[symbols_r.length-1], symbols_r.data[symbols_r.length-2], symbol_t);
                where_paste -= 1;
            }
            previous_braces = 0;
            continue;
        }
        if (symbols.data[i].type == BRACE_OPEN_ROUND) { printf("RP: %3ld [[ ( ]]\n", i);
            size_t pc = 0;
            struct list_symbol_t braces_stuff = braces_to_reverse_polish(symbols, i+1, &pc);
            i += pc;
            LIST_PUSH_RANGE_AT(braces_stuff.data, braces_stuff.length, symbols_r, where_paste, symbol_t);
            LIST_FREE(braces_stuff);
            where_paste = symbols_r.length;
            previous_braces = ~0;
            continue;
        }
        if (symbols.data[i].type == BRACE_CLOSE_ROUND) { printf("RP: %3ld [[ ) ]]\n", i);
            if (processed_count) *processed_count = i - from + 1;
            return symbols_r;
        }
    }
    if (processed_count) *processed_count = symbols.length - from;
    return symbols_r;

}

struct equation  to_reverse_polish(struct equation eq) {

    struct equation eq_r = {.symbols = braces_to_reverse_polish(eq.symbols, 0, NULL)};  
    return eq_r;
    
}





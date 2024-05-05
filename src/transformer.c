
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
    if (type == NUMBER) return "NUMBER";
    return "NULL_TYPE";
}

char* soviet_union = NULL;
size_t soviet_union_len = 0;

const char* get_string_from_symbol(struct symbol symbol) {
    for (size_t i = 0; i < LEN(symbols_table); ++i) {
        if (symbols_table[i].value == symbol.type) return symbols_table[i].key;
    }
    if (symbol.text != NULL) {
        if (soviet_union == NULL) {

            soviet_union_len = symbol.text_len+1;
            soviet_union = (char*)malloc(soviet_union_len);

        } else if (soviet_union_len-1 < symbol.text_len) {

            soviet_union_len = symbol.text_len+1;
            soviet_union = (char*)realloc(soviet_union, soviet_union_len);

        }
        memcpy(soviet_union, symbol.text, symbol.text_len + 1);
        soviet_union[symbol.text_len] = '\0';
        return soviet_union;
    }
    return "NULL_SYMBOL";
}


int is_word_symbol(char c) {
    return (
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') // ||
        // (c >= '0' && c <= '1') ||
        // (c == '_')
    );
}

const char*     get_next_word(const char* string, size_t* spaces, size_t* word_len, enum symbol_type* type) {

    for (*spaces = 0; string[(*spaces)] != '\0'; ++(*spaces)) 
        if (
            string[(*spaces)] !=  ' ' && 
            string[(*spaces)] != '\t' && 
            string[(*spaces)] != '\n' 
        ) break;
    string = string + *spaces;



    for (*word_len = 0; is_word_symbol(string[*word_len]); ++(*word_len)) {}

    if (0 == *word_len) return NULL;
    *type = VARIABLE;
    return string;

}


const char*     get_next_operator(const char* string, size_t* spaces, size_t* word_len, enum symbol_type* type) {

    for (*spaces = 0; string[(*spaces)] != '\0'; ++(*spaces)) 
        if (
            string[(*spaces)] !=  ' ' && 
            string[(*spaces)] != '\t' && 
            string[(*spaces)] != '\n' 
        ) break;
    string = string + *spaces;

    for (size_t i = 0; i < LEN(symbols_table); ++i) {
        int accept = 1;
        *word_len = 0;
        for (size_t j = 0; symbols_table[i].key[j] != '\0'; ++j) {
            if (symbols_table[i].key[j] != string[j]) {
                accept = 0;
                break;
            }
            if (symbols_table[i].key[j + 1] == '\0') *word_len = j + 1;
        }
        if (accept) {
            *type = symbols_table[i].value;
            return string;
        }
    }

    return NULL;

}



const char*     get_next_number(const char* string, size_t* spaces, size_t* word_len, enum symbol_type* type) {

    for (*spaces = 0; string[(*spaces)] != '\0'; ++(*spaces)) 
        if (
            string[(*spaces)] !=  ' ' && 
            string[(*spaces)] != '\t' && 
            string[(*spaces)] != '\n' 
        ) break;
    string = string + *spaces;


    int was_dot = 0;
    for (*word_len = 0; string[*word_len] != '\0'; ++(*word_len)) {

        if (string[(*word_len)] == '.') {
            if (was_dot) break;
            else {
                was_dot = 1;
                continue;
            }
        }

        if (string[*word_len] < '0' || string[*word_len] > '9') break; 


    }


    if (0 == *word_len) return NULL;
    *type = NUMBER;
    return string;

}




struct symbol   get_next_symbol(const char* string, size_t* spaces, size_t* text_len) {


    struct symbol symb = {
        .type = NULL_SYMBOL,
        .text = NULL,
        .text_len = 0,
    };


    const char* nw;
    enum symbol_type type;
    
    
    nw = get_next_operator(string, spaces, text_len, &type);

    if (nw != NULL) {
        symb.text = nw;
        symb.text_len = *text_len;
        symb.type = type;
        return symb;
    }



    nw = get_next_word(string, spaces, text_len, &type);

    if (nw != NULL) {
        symb.text = nw;
        symb.text_len = *text_len;
        symb.type = type;
        return symb;
    }



    nw = get_next_number(string, spaces, text_len, &type);

    if (nw != NULL) {
        symb.text = nw;
        symb.text_len = *text_len;
        symb.type = type;
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

        // printf("nextw symblol: %ld (%s), spaces: %ld, word_length: %ld\n", 
        //     next_symbol.type, 
        //     get_string_from_symbol_type(next_symbol.type), 
        //     spaces, 
        //     word_length
        // );

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

#define PRINT_SYMBOL_LIST(list) \
        for (size_t s = 0; s < list.length; ++s)\
        {\
            if (where_paste == s) printf("_ ");\
            printf("%s ", get_string_from_symbol(list.data[s]));\
        }\
        printf("\n"); 


struct list_symbol_t braces_to_reverse_polish(const struct list_symbol_t symbols, size_t from, size_t* processed_count) {

    struct list_symbol_t symbols_r = {0};
    size_t where_paste = symbols_r.length; // which is 0
    size_t previous_braces_end = 0;
    
    for (size_t i = from; i < symbols.length; ++i) {
        if (symbols.data[i].type & VARIABLE) { 

            // printf("RP: %3ld [[ %s ]] ", i, get_string_from_symbol(symbols.data[i]));
            LIST_PUSH_AT(symbols.data[i], symbols_r, where_paste, symbol_t);
            where_paste = symbols_r.length;
            // PRINT_SYMBOL_LIST(symbols_r);
       
        } else if (symbols.data[i].type & OPERATOR) { 

            // printf("RP: %3ld [[ %s ]] ", i, get_string_from_symbol(symbols.data[i]));
            LIST_APPEND(symbols.data[i], symbols_r, symbol_t);
            where_paste = symbols_r.length - 1; // useless

            // printf("RP: SWAP? %s and %s\n", 
            //     (symbols_r.data[symbols_r.length-2].type & OPERATOR) ? "true" : "false",
            //     (get_operation_proirity(symbols_r.data[symbols_r.length-2].type) < get_operation_proirity(symbols_r.data[symbols_r.length-1].type)) ? "true" : "false"
            // );
            // PRINT_SYMBOL_LIST(symbols_r);
            if ((symbols_r.data[symbols_r.length-2].type & OPERATOR) 
                && (previous_braces_end < symbols_r.length-2)
                && (get_operation_proirity(symbols_r.data[symbols_r.length-2].type) < get_operation_proirity(symbols_r.data[symbols_r.length-1].type))
            )
            {
                // printf("RP: %3ld %7s ", i, "SWAP!");
                SWAP(symbols_r.data[symbols_r.length-1], symbols_r.data[symbols_r.length-2], symbol_t);
                where_paste -= 1;
                // PRINT_SYMBOL_LIST(symbols_r);
            }
        
        } else if (symbols.data[i].type == BRACE_OPEN_ROUND) { 

            // printf("RP: %3ld [[ ( ]]\n", i);
            size_t pc = 0;
            struct list_symbol_t braces_stuff = braces_to_reverse_polish(symbols, i+1, &pc);
            i += pc;
            LIST_PUSH_RANGE_AT(braces_stuff.data, braces_stuff.length, symbols_r, where_paste, symbol_t);
            previous_braces_end = where_paste + braces_stuff.length - 1;
            LIST_FREE(braces_stuff);
            where_paste = symbols_r.length;
            // PRINT_SYMBOL_LIST(symbols_r);
       
        } else if (symbols.data[i].type == BRACE_CLOSE_ROUND) { 

            // printf("RP: %3ld [[ ) ]] ", i);
            if (processed_count) *processed_count = i - from + 1;
            return symbols_r;
        }
    }
    if (processed_count) *processed_count = symbols.length - from;
    return symbols_r;

}

struct equation  to_postfix_notation(struct equation eq) {

    struct equation eq_r = {.symbols = braces_to_reverse_polish(eq.symbols, 0, NULL)};  
    return eq_r;
    
}


struct equation  to_infix_notation(struct equation reverse_polish) {
    // a b + c - g *
    //
    // ( ( ( a + b ) - c ) * g )

    // a b c + *
    // ( b + c ) * c

    symbol_t brace_open = {
        .type = BRACE_OPEN_ROUND,
        .text = "(",
        .text_len = 1,
    };

    symbol_t brace_close = {
        .type = BRACE_CLOSE_ROUND,
        .text = ")",
        .text_len = 1,
    };


    struct equation norm_r = {0};

    LIST(symbol_t) symbol_stack = {0};

    for (size_t i = 0; i < reverse_polish.symbols.length; ++i) {
        if (reverse_polish.symbols.data[i].type & OPERAND) {
            LIST_APPEND(reverse_polish.symbols.data[i], symbol_stack, symbol_t);
        } else {
            if (norm_r.symbols.length > 0) {
                LIST_PUSH_AT(brace_open, norm_r.symbols, 0, symbol_t);
                LIST_APPEND(brace_close, norm_r.symbols, symbol_t);
            }
            
            size_t push_at = norm_r.symbols.length;

            if (symbol_stack.length > 0) {
                LIST_PUSH_AT(symbol_stack.data[symbol_stack.length-1], norm_r.symbols, push_at, symbol_t);
                symbol_stack.length -= 1;
            }

            LIST_PUSH_AT(reverse_polish.symbols.data[i], norm_r.symbols, push_at, symbol_t);

            if (symbol_stack.length > 0) {
                LIST_PUSH_AT(symbol_stack.data[symbol_stack.length-1], norm_r.symbols, push_at, symbol_t);
                symbol_stack.length -= 1;
            }


        }
    }

    return norm_r;
}




const struct equation_tree null_tree = NULL_TREE;

struct equation* get_all_transformations_s(const char* string, size_t* transformations, struct equation* permutations, size_t permutations_len) {
    struct equation eq = get_equation_from_string(string);
    struct equation* r = get_all_transformations(&eq, transformations, permutations, permutations_len);
    LIST_FREE(eq.symbols);
    return r;
}



struct equation* get_all_transformations(struct equation* equation, size_t* transformations, struct equation* permutations, size_t permutations_len) {

    LIST(equation_t) equations_r = {0};


    struct equation_tree eq_tree = null_tree;
    for (size_t i = equation->symbols.length-1; i < equation->symbols.length; --i) {
        add_symbol_to_equation_tree(equation->symbols.data[i], &eq_tree);
    }


    *transformations = equations_r.length;
    return equations_r.data;

}



unsigned char get_operator_arity(enum symbol_type oper) {
    if (oper & OPERATOR) {
        return ((oper >> 8) & 0b11111111);
    }
    return 0;
}



void add_symbol_to_equation_tree(symbol_t symbol, struct equation_tree* tree) {

    if (tree->symbol.type == NULL_SYMBOL) {
        tree->symbol = symbol;
        if (tree->symbol.type & VARIABLE) {
            tree->filled = 1;
            return;
        }
        LIST_INIT_LEN(tree->sub_equations, get_operator_arity(tree->symbol.type), struct equation_tree);
        goto check;
    }

    if (tree->symbol.type & VARIABLE) {
        return;
    }

    for (size_t i = tree->sub_equations.alloc_length-1; i < tree->sub_equations.alloc_length; --i) {
        if (!tree->sub_equations.data[i].filled) {
            add_symbol_to_equation_tree(symbol, tree->sub_equations.data + i);
            if (tree->sub_equations.data[i].filled)
                tree->sub_equations.length = tree->sub_equations.alloc_length - i;
            goto check;
        }
    }
check:
    
    if (tree->sub_equations.length >= get_operator_arity(tree->symbol.type))
        tree->filled = 1;

    
}

void print_equation_tree(struct equation_tree* tree, size_t zindex) {
    for (size_t i = 0; i < zindex; ++i) printf("   ");
    printf("[%s]", get_string_from_symbol(tree->symbol));
    if (tree->sub_equations.length != (size_t)get_operator_arity(tree->symbol.type))
        printf(" (sub length: %ld, but would be %ld)",
            tree->sub_equations.length,
            (size_t)get_operator_arity(tree->symbol.type)
        );
    putchar('\n');
    for (size_t i = 0; i < tree->sub_equations.length; ++i)
        print_equation_tree(tree->sub_equations.data + i, zindex+1);
}
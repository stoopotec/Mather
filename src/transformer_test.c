#include <stdio.h>

#include "transformer.h"


int main() {
    
    char buffer[2056];

    while (1) {

        printf("plasew entr a textuwu\n");

        fgets(buffer, sizeof(buffer), stdin);

        // size_t transformations;

        // struct equation* equations = get_all_transformations_s(&transformations, buffer);

        struct equation eq = get_equation_from_string(buffer);
        
        printf("received: ");

        for (size_t i = 0; i < eq.symbols.length; ++i) {
            if (eq.symbols.data[i].type != VARIABLE)
                printf("%s ", get_string_from_symbol_type(eq.symbols.data[i].type));
            else {
                for (size_t j = 0; j < eq.symbols.data[i].text_len; ++j) {
                    putchar(eq.symbols.data[i].text[j]);
                }
                putchar(' ');
            }
        }
        printf("\n\n");


        struct equation eq_p = to_reverse_polish(eq);
        
        printf("to reverse polish: ");

        for (size_t i = 0; i < eq_p.symbols.length; ++i) {
            if (eq_p.symbols.data[i].type != VARIABLE)
                printf("%s ", get_string_from_symbol_type(eq_p.symbols.data[i].type));
            else {
                for (size_t j = 0; j < eq_p.symbols.data[i].text_len; ++j) {
                    putchar(eq_p.symbols.data[i].text[j]);
                }
                putchar(' ');
            }
        }
        printf("\n\n");

    }

}
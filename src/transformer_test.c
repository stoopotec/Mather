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
            printf("%s ", get_string_from_symbol_type(eq.symbols.data[i].type));
        }
        printf("\n\n");

    }

}
#include <stdio.h>

#include "transformer.h"


int main() {
    
    char buffer[2056];

    while (1) {

        fgets(buffer, sizeof(buffer), stdin);

        size_t transformations;

        struct equation* equations = get_all_transformations(&transformations, buffer);

    }

}
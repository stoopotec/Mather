#include <stdio.h>

#include "transformer.h"
#include "list.h"


int main() {


    // LIST(int) list = {0};

    // LIST_APPEND(10, list, int);
    // LIST_APPEND(20, list, int);
    // LIST_APPEND(30, list, int);
    // LIST_APPEND(40, list, int);
    // LIST_APPEND(50, list, int);



    // int arr[] = {1, 2, 3, 4};
    // const size_t arr_len = sizeof(arr) / sizeof(*arr);

    // LIST_PUSH_RANGE_AT(arr, arr_len, list, arr[1], int);

    // for (size_t i = 0; i < list.length; ++i) printf("%2d; ", list.data[i]); putchar('\n');

    // return 0;



    
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




        printf("equation tree: \n");

        struct equation_tree tree = NULL_TREE;
        for (size_t i = eq_p.symbols.length - 1; i < eq_p.symbols.length; --i) {
            printf("BT: adding symbol [%s]...\n", get_string_from_symbol(eq_p.symbols.data[i]));
            add_symbol_to_equation_tree(eq_p.symbols.data[i], &tree);
        }

        print_equation_tree(&tree, 0);

        printf("\n\n");





        size_t trans_len = 0;
        struct equation* transformations = get_all_transformations(&eq, &trans_len);

        printf("transformations:\n");
        for (size_t i = 0; i < trans_len; ++i) {
            for (size_t i = 0; i < transformations[i].symbols.length; ++i) {
                if (transformations[i].symbols.data[i].type != VARIABLE)
                    printf("%s ", get_string_from_symbol_type(transformations[i].symbols.data[i].type));
                else {
                    for (size_t j = 0; j < transformations[i].symbols.data[i].text_len; ++j) {
                        putchar(transformations[i].symbols.data[i].text[j]);
                    }
                    putchar(' ');
                }
            }
            printf("\n");
        }




    }

}
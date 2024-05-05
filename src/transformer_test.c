#include <stdio.h>

#include "transformer.h"
#include "list.h"


#define ARR_LEN(x) (sizeof(x) / sizeof(*(x)))

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

    // printf("computing standart permutations... ");

    // struct equation permuts[] = {
    //     get_equation_from_string("a + b \\eq b + a"),
    //     get_equation_from_string("a + (b + c) \\eq (a + b) + c"),

    //     get_equation_from_string("a + 0 \\eq a"),
    //     get_equation_from_string("a + (-a) \\eq 0"),

    //     get_equation_from_string("a * b \\eq b * a"),
    //     get_equation_from_string("a * (b * c) \\eq (a * b) * c"),
    //     get_equation_from_string("1 * a \\eq a"),
    //     get_equation_from_string("a * (1/a) \\eq 1"),
    //     get_equation_from_string("(a + b) * c \\eq a * c + b * c"),

    //     // get_equation_from_string("a = b"),
    //     // get_equation_from_string("a \leq c"),
    //     // get_equation_from_string("a + c \leq b + c"),
    //     // get_equation_from_string("0 \leq a * b"),
    //     // get_equation_from_string("a < b"),
    //     // get_equation_from_string("b \geq a"),
    //     // get_equation_from_string("a > b"),

    //     // get_equation_from_string("\exists c \in \mathbb{R} | a \leq c \land c \leq b"),
    // };
    // printf("done.\n");


    
    char buffer[2056];

    while (1) {

        printf("your equation\n> ");

        fgets(buffer, sizeof(buffer), stdin);

        printf("\n\n");

        // size_t transformations;

        // struct equation* equations = get_all_transformations_s(&transformations, buffer);

        struct equation eq = get_equation_from_string(buffer);
        
        printf("received: ");

        for (size_t i = 0; i < eq.symbols.length; ++i) {
            printf("%s ", get_string_from_symbol(eq.symbols.data[i]));
        }
        printf("\n\n");


        struct equation eq_p = to_postfix_notation(eq);
        
        printf("to reverse polish from normal: ");

        for (size_t i = 0; i < eq_p.symbols.length; ++i) {
            printf("%s ", get_string_from_symbol(eq_p.symbols.data[i]));
        }
        printf("\n\n");


        double result;
        if (computable(&eq_p, &result))
            printf("solution: %lf\n\n", result);



        struct equation eq_np = to_infix_notation(eq_p);

        printf("to normal from reverse polish: ");

        for (size_t i = 0; i < eq_np.symbols.length; ++i) {
            printf("%s ", get_string_from_symbol(eq_np.symbols.data[i]));
        }
        printf("\n\n");




        printf("equation tree from reverse polish: \n");

        struct equation_tree tree = NULL_TREE;
        for (size_t i = eq_p.symbols.length - 1; i < eq_p.symbols.length; --i) {
            add_symbol_to_equation_tree(eq_p.symbols.data[i], &tree);
        }

        print_equation_tree(&tree, 0);

        printf("\n\n");





        // size_t trans_len = 0;
        // struct equation* transformations = get_all_transformations(&eq, &trans_len, permuts, ARR_LEN(permuts));

        // printf("transformations:\n");
        // for (size_t i = 0; i < trans_len; ++i) {
        //     for (size_t j = 0; j < transformations[i].symbols.length; ++j) {
        //         printf("%s ", get_string_from_symbol(transformations[i].symbols.data[j]));
        //     }
        //     printf("\n");
        // }




    }

}
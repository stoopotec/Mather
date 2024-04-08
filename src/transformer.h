#pragma once

#include <stdlib.h>







struct equation {

};



struct equation* get_equation_from_string(const char* string);


struct equation* get_all_transformations(size_t* transformations, struct equation* equation);
struct equation* get_all_transformations(size_t* transformations, const char* string);












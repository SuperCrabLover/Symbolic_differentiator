#include <stdbool.h>
#include <math.h>
#ifndef PARSEDEFINED
#define PARSEDEFINED

#include "parse_tree.h"

#endif
/*
 * recursive parser for grammar-like rules:
 * G -> E$;
 * E -> T{[+-]T}^*;
 * T -> P{[/ or *]P}^* | -P{[/ or *]P}^* ; 
 * P -> K{[^]K}^*;
 * K -> '('E')' | N | x | fun'('E')';
 * N -> x | 0 | ... | 9 ;
 * 
 * P.S. an unary minus is saved with "m\0" in a node
 */

typedef enum 
{
    SIN,
    COS,
    TAN,
    LOG,
    NONE
} function;

static char* s = NULL;
static size_t p = 0;

void SyntaxError(); 
void Require(char symb); 
function is_func();
struct node* get_g(char* str);
struct node* get_e(void);
struct node* get_t(void);
struct node* get_p(void);
struct node* get_k(void);
struct node* get_n(void);

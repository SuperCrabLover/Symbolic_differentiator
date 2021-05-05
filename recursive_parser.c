#include "recursive_parser.h"
/*
 * recursive parser for grammar-like rules:
 * G -> E$;
 * E -> T{[+-]T}^*;
 * T -> P{[/ or *]}^* | -P{[/ or *]}^*;
 * P -> K{[^]K}^*;
 * K -> '('E')' | N | x | fun'('E')';
 * N -> x | 0 | ... | 9 ;
 */

void SyntaxError() 
{
    printf("syntax error\n");
}

void Require(char symb) 
{
    if (s[p] == symb)
        p++;
    else
        printf("Need %c symbol\n", symb);
}

function is_func() 
{
    if (s[p] == 's' && s[p+1] == 'i' && s[p+2] == 'n') 
    {
        p += 3;
        return SIN;
    }
    else if (s[p] == 'c' && s[p+1] == 'o' && s[p+2] == 's') 
    {
        p += 3;
        return COS;
    }
    else if (s[p] == 't' && s[p+1] == 'a' && s[p+2] == 'n') 
    {
        p += 3;
        return TAN;
    }
    else if (s[p] == 'l' && s[p+1] == 'o' && s[p+2] == 'g')
    {
        p += 3;
        return LOG;
    }
    else
        return NONE;
}

struct node* get_g(char* str) 
{
    struct node* root = init_tree();
    s = str;
    p = 0;
    struct node* e_node = get_e();
    if (s[p] == '$')
        p++;
    else
        SyntaxError();
    root->right_child = e_node;
    return root;
}

struct node* get_e() 
{
    struct node* g_node = get_t();
    struct node* plsmns_node = NULL; 
    if (s[p] == '+' || s[p] == '-')
    {
        char op[2];
        op[0] = s[p];
        op[1] = '\0'; 
        p++;
        struct node* next_g_node = get_t();
        plsmns_node = create_node(op, g_node, next_g_node);
    }
    while (s[p] == '+' || s[p] == '-') 
    {
        char op[2];
        op[0] = s[p];
        op[1] = '\0'; 
        p++;
        struct node* next_g_node = get_t();
        struct node* tmp = plsmns_node;
        plsmns_node = create_node(op, tmp, next_g_node);
    }
    if (NULL == plsmns_node)
    {
        return g_node;
    }
    else
    {
        return plsmns_node;
    }
}

struct node* get_t() 
{
    struct node* m_node = NULL;
    struct node* p_node = NULL;
    if (s[p] == '-')
    {
        p++;
        m_node = create_node("m\0", NULL, NULL);
        p_node = get_p();
        m_node->right_child = p_node;
        p_node = m_node;
    }
    else
    {
        p_node = get_p();
    }

    struct node* mldvpw_node = NULL;
    if (s[p] == '*' || s[p] == '/' )
    {
        char op[2];
        op[0] = s[p];
        op[1] = '\0'; 
        p++;
        struct node* next_p_node = get_p();
        mldvpw_node = create_node(op, p_node, next_p_node);
    }
    while (s[p] == '*' || s[p] == '/') 
    {
        char op[2];
        op[0] = s[p];
        op[1] = '\0'; 
        p++;
        struct node* next_g_node = get_p();
        struct node* tmp = mldvpw_node;
        mldvpw_node = create_node(op, tmp, next_g_node);
    }
    if (NULL == mldvpw_node)
    {
        return p_node;
    }
    else
    {
        return mldvpw_node;
    }
}
struct node* get_p()
{
    struct node* k_node = get_k();
    struct node* pwr_node = NULL;

    if (s[p] == '^')
    {
        p++;
        struct node* next_k_node = NULL;
        next_k_node = get_k();
        pwr_node = create_node("^\0", k_node, next_k_node);
    }
    while (s[p] == '^')
    {
        p++;
        struct node* next_k_node = NULL;
        next_k_node = get_k();
        struct node* tmp = pwr_node;
        pwr_node = create_node("^\0", tmp, next_k_node);
    }
    if (NULL == pwr_node)
    {
        return k_node;
    }
    else
    {
        return pwr_node;
    }

}

struct node* get_k() 
{
    function func = is_func();
    if (s[p] == '(') 
    {
        p++;
        struct node* e_node = get_e();
        struct node* fun_node = NULL;
        Require(')');
        switch (func) 
        {
            case SIN:
                fun_node = create_node("sin\0", NULL, e_node);
                break;
            case COS:
                fun_node = create_node("cos\0", NULL, e_node);
                break;
            case TAN:
                fun_node = create_node("tan\0", NULL, e_node);
                break;
            case LOG:
                fun_node = create_node("log\0", NULL, e_node);
                break;
            default:
                fun_node = e_node;
                break;
        }
        return fun_node;
    }
    else
    {
        return get_n();
    }
}

struct node* get_n() 
{
    
    //printf("p: %zu\n", p);
    if (s[p] == 'x')
    {   
        p++;
        struct node* n_node = create_node("x\0", NULL, NULL);
        return n_node;
    }
    char val[1000];
    bool changed = false;
    int i = 0;
    while ('0' <= s[p] && s[p] <= '9') 
    {
        //printf("%c\n", s[p]);
        val[i] = s[p];
        changed = true;
        p++;
        i++;
    }
    val[i] = '\0';
    if (!changed)
    {
        SyntaxError();
    }
    struct node* n_node = create_node(val, NULL, NULL);
    return n_node;
}
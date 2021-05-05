#define SIZE 15
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _SIN "sin\0"
#define _COS "cos\0"
#define _TAN "tan\0"
#define _LOG "log\0"

struct node
{
	char value[SIZE];
	struct node* left_child;
	struct node* right_child;
};

struct node* init_tree();
struct node* create_node(char* in_value, struct node* in_left_child, struct node* in_right_child);
char* to_string(struct node* node);
int _is_fun(char* node_value);
int _is_op(char* node_value);
int _is_pow_op(char* node_value);
int _is_m_op(char* node_value);
int _is_highpr_op(char* node_value);
int _is_lowpr_op(char* node_value);

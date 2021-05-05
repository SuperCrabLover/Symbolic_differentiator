#ifndef PARSEDEFINED
#define PARSEDEFINED

#include "parse_tree.h"

#endif

void tree_print(struct node* root);
void _r_mm_to_plus(struct node* root);
void _l_mm_to_plus(struct node* root);
void _right_m_handler(struct node* root);
struct node* _simplify(struct node* root, struct node* child);
void tree_simplify(struct node* root);
void tree_delete(struct node* root);
struct node* tree_copy(struct node* root);
int _has_x(struct node* root);
struct node* tree_diff(struct node* root);
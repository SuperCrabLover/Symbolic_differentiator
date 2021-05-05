#include "parse_tree.h"
#define SIZE_OF_PART 2000

struct node* init_tree()
{
	struct node* root = (struct node*)calloc(1, sizeof(struct node));
	if (NULL == root)
	{
		fprintf(stderr, "init_tree(): calloc error!\n");
		return NULL;
	}

	root->value[0] = '\0';
	root->left_child = NULL;
	root->right_child = NULL;

	return root;
}

struct node* create_node(char* in_value, struct node* in_left_child, struct node* in_right_child)
{
	struct node* new_node = (struct node*)calloc(1, sizeof(struct node));
	if (NULL == new_node)
	{
		fprintf(stderr, "create_node(): calloc error!\n");
		return NULL;
	}

	strcpy(new_node->value, in_value);
	new_node->left_child = in_left_child;
	new_node->right_child = in_right_child;

	return new_node;
}

int _is_fun(char* node_value)
{
	if ((strcmp(_SIN, node_value) == 0) ||
		(strcmp(_COS, node_value) == 0) ||
		(strcmp(_TAN, node_value) == 0) ||
		(strcmp(_LOG, node_value) == 0))
		{
			return 1;
		}
		
		return 0;
}

int _is_op(char* node_value)
{
	return _is_pow_op(node_value) || _is_m_op(node_value) || _is_highpr_op(node_value) || _is_lowpr_op(node_value);
}

int _is_pow_op(char* node_value)
{
	if (node_value[0] == '^')
	{
		return 1;
	}

	return 0;
}

int _is_m_op(char* node_value)
{
	if (node_value[0] == 'm')
	{
		return 1;
	}

	return 0;
}

int _is_highpr_op(char* node_value)
{
	if (node_value[0] == '/' || node_value[0] == '*')
	{
		return 1;
	}

	return 0;
}

int _is_lowpr_op(char* node_value)
{
	if (node_value[0] == '+' || node_value[0] == '-')
	{
		return 1;
	} 

	return 0;
}

char* to_string(struct node* node)
{
	if (NULL == node)
	{
		char* empty_str = (char *)calloc(1, sizeof(char));
		if (NULL == empty_str)
		{
			fprintf(stderr, "to_string(): calloc error! Cannot create empty_str!\n");
			return NULL;
		}
		empty_str[0] = '\0';
		return empty_str;
	}
	char* left_part = to_string(node->left_child);
	char* right_part = to_string(node->right_child);

	char lpart_str[SIZE_OF_PART];
	lpart_str[0] = '\0';
	char open[2] = {'(', '\0'};
	char close[2] = {')', '\0'};
	char* str = (char*)calloc(SIZE_OF_PART, sizeof(char));
	if (NULL == str)
	{
		fprintf(stderr, "to_string(): calloc error!\n");
		return NULL;
	}
	str[0] = '\0';
	if (NULL != node->left_child)
	{
			//  {"+-m" 			   <- "*/"}  ==> brakets
			//  {"not x or number" <- "^"}   ==> brackets
			//  {"fun or * or /"   <- "/"}   ==> brackets
		if (
			(_is_highpr_op(node->value) && _is_lowpr_op(node->left_child->value)) || _is_m_op(node->left_child->value) ||
			(_is_pow_op(node->value) && _is_op(node->left_child->value)) || _is_fun(node->left_child->value)           ||
			(node->value[0] == '/' && _is_highpr_op(node->left_child->value)) || _is_fun(node->left_child->value)    
		   )
		{
			strcat(lpart_str, open);        //tmp: "("
			strcat(lpart_str, left_part);   //tmp: "(" + "left_part"
			strcat(lpart_str, close);       //tmp: "(" + "left_part" + ")"
		}
		else
		{
			strcat(lpart_str, left_part);   //tmp: "left_part"
		}

	}
 
	if (NULL != node->right_child)
	{
			// "*/" -> "+-m"             ==> brackets
			// "^"  -> "not x or number" ==> brackets
			// "/"  -> "fun or * or /"   ==> brackets 
		if ( 
			(_is_highpr_op(node->value) && _is_lowpr_op(node->right_child->value)) || _is_m_op(node->right_child->value)   ||
			(_is_pow_op(node->value) && _is_op(node->right_child->value)) || _is_fun(node->right_child->value)             ||
		    (node->value[0] == '/' && _is_highpr_op(node->right_child->value)) || _is_fun(node->right_child->value) 	   ||
			(node->value[0] == '-'  && node->right_child->value[0] == '+')   
		   ) 
		{
			strcat(str, open);		  	//str: "("
			strcat(str, right_part);  	//str: "(" + "right_part"
			strcat(str, close);		  	//str: "(" + "right_part" + ")"
		}
		else if (_is_fun(node->value) || (_is_m_op(node->value) && _is_op(node->right_child->value)))  //put function arguments or unary minus arguments into brackets
		{
			strcat(str, open);		  	//str: "("
			strcat(str, right_part);  	//str: "(" + "right_part"
			strcat(str, close);		  	//str: "(" + "right_part" + ")"
		}
		else
		{
			strcat(str, right_part);  	//str: "right_part"
		}
	}
	strcat(lpart_str, node->value);  
	strcat(lpart_str, str);
	strcpy(str, lpart_str);
	free(right_part);
	free(left_part);

	return str;
}

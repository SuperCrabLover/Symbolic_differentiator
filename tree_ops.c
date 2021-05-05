#include "tree_ops.h"

void tree_print(struct node* root)
{
	if (NULL == root)
	{
		return;
	}

	printf("The value is: %s\n", root->value);
	tree_print(root->left_child);
	tree_print(root->right_child);
}

void _r_mm_to_plus(struct node* root)
{
	struct node* r1_ch = root->right_child;
	if (NULL != r1_ch->right_child)	//root->right_child->right_child
	{
		struct node* r2_ch = r1_ch->right_child;
		if (r1_ch->value[0] == 'm' && r2_ch->value[0] == 'm') // (L <- root -> m -> m-> R) ==> (L <- root -> R)
		{
			if (NULL == r2_ch->right_child)
			{
				fprintf(stderr, "r_mm_to_plus(): error! Error in the tree detected: no value after unary minus!\n");
				return;
			}
			else
			{
				//root->right_child->right_child->right_child
				root->right_child = r2_ch->right_child;
				free(r1_ch);
				free(r2_ch);
				if (root->right_child->value[0] == 'm')
				{
					_r_mm_to_plus(root);
				}
			}
		}
	}
}

void _l_mm_to_plus(struct node* root)
{
	struct node* l1_ch = root->left_child;
	if (NULL != l1_ch->right_child)
	{
		_r_mm_to_plus(l1_ch);
	}
	if (NULL != l1_ch->right_child)
	{
		struct node* l1r_ch = l1_ch->right_child;
		if (l1_ch->value[0] == 'm' &&  l1r_ch->value[0] == 'm')
		{
			if (NULL == l1r_ch->right_child)
			{
				fprintf(stderr, "l_mm_to_plus(): error! Error in the tree detected: no value after unary minus!\n");
				return;	
			}
			else
			{
				root->left_child = l1r_ch->right_child;
				free(l1_ch);
				free(l1r_ch);
			}
		}
	}
}

void _right_m_handler(struct node* root)
{
	struct node* r1_ch = root->right_child;
	if (root->value[0] == '-' && r1_ch->value[0] == 'm') // (L<- '-' -> 'm' -> R) ==> (L <- '+' -> R)
	{
		if (NULL == r1_ch->right_child)
		{
			fprintf(stderr, "tree_simplify(): error! Error in the tree detected: no value after unary minus!\n");
			return;
		}
		else
		{
			//root->right_child->right_child
			root->value[0] = '+';
			root->right_child = r1_ch->right_child;
			free(r1_ch);
			r1_ch = root->right_child;
		}		
	}
	if (root->value[0] == '+' && r1_ch->value[0] == 'm') // (L<- '+' -> 'm' -> R) ==> (L <- '-' -> R)
	{
		if (NULL == r1_ch->right_child)
		{
			fprintf(stderr, "tree_simplify(): error! Error in the tree detected: no value after unary minus!\n");
			return;
		}
		else
		{
			//root->right_child->right_child
			root->value[0] = '-';
			root->right_child = r1_ch->right_child;
			free(r1_ch);
			r1_ch = root->right_child;
		}		
	}

}

struct node* _simplify(struct node* root, struct node* child)
{
	if (child->value[0] == '+' || child->value[0] == '-') 
	{
		if (NULL == child->right_child) // root->right_child->right_child
		{
			fprintf(stderr, "tree_simplify(): error! Error in the tree detected: no right value for binary plus!\n");
			return NULL;
		}
		if (NULL == child->left_child)
		{
			fprintf(stderr, "tree_simplify(): error! Error in the tree detected: no left value for binary plus!\n");
			return NULL;
		}
		struct node* r2_ch = child->right_child;
		struct node* l2_ch = child->left_child;
		if (r2_ch->value[0] == '0' && r2_ch->value[1] == '\0')
		{
			if (l2_ch->value[0] == '0' && l2_ch->value[1] == '\0')// L1 <- root -> (0 <- +/- -> 0) ==> L1 <- root -> 0 || (0 <- +/- -> 0) <- root ->  R1  == > 0 <- root -> R1  
			{
				struct node* new_root_child = create_node("0\0", NULL, NULL);
				tree_delete(l2_ch);
				tree_delete(r2_ch);
				free(child);
				return new_root_child;
			}
			else // L1 <- root -> (L2 <- +/- -> 0) ==> L1 <- root -> L2 || (L2 <- +/- -> 0) <- root ->  R1 ==> L2 <- root -> R1
			{
				tree_delete(r2_ch);
				free(child);
				return l2_ch;
			}
		}	
		if (l2_ch->value[0] == '0' && l2_ch->value[1] == '\0')
		{
			if (child->value[0] == '+') // L1 <- root -> (0 <- + -> R2) ==> L1 <- root -> R2 || (0 <- + -> R2) <- root -> R1  ==> R2 <- root -> R1
			{
				tree_delete(l2_ch);
				free(child);
				return r2_ch;
			}
			else // L1 <- root -> (0 <- - -> R2) ==> L1 <- root -> (NULL <- m -> R2) ||  (0 <- - -> R2) <- root -> R1 ==> (NULL <- m -> R2) <- root -> R1
			{
				tree_delete(l2_ch);
				free(child);
				return create_node("m\0", NULL, r2_ch);
			}
		}
	}
	else if (child->value[0] == '*')
	{
		if (NULL == child->right_child) // root->right_child->right_child
		{
			fprintf(stderr, "tree_simplify(): error! Error in the tree detected: no right value for multiply!\n");
			return NULL;
		}
		if (NULL == child->left_child)
		{
			fprintf(stderr, "tree_simplify(): error! Error in the tree detected: no left value for multiply!\n");
			return NULL;
		}
		struct node* r2_ch = child->right_child;
		struct node* l2_ch = child->left_child;
		if (r2_ch->value[0] == '1' && r2_ch->value[1] == '\0')// L1 <- root -> (L2 <- * -> 1) ==> L1 <- root -> L2 || (L2 <- * -> 1) <- root -> R1  ==> L2 <- root -> R1
		{
			tree_delete(r2_ch);
			free(child);
			return l2_ch;
		}
		else if (l2_ch->value[0] == '1' && l2_ch->value[1] == '\0')// L1 <- root -> (1 <- * -> R2) ==> L1 <- root -> R2 || (1 <- * -> R2) <- root -> R1  ==> R2 <- root -> R1
		{
			tree_delete(l2_ch);
			free(child);
			return r2_ch;
		}
		else if ((r2_ch->value[0] == '0' && r2_ch->value[1] == '\0')|| (l2_ch->value[0] == '0' && l2_ch->value[1] == '\0'))// L1 <- root -> (L2/0 <- * -> R2/0) ==> L1 <- root -> 0 || (L2/0 <- * -> R2/0) <- root -> R1  ==> 0 <- root -> R1
		{
			tree_delete(r2_ch);
			tree_delete(l2_ch);
			free(child);
			return create_node("0\0", NULL, NULL);
		}
	}
	else if (child->value[0] == '^')
	{
		if (NULL == child->right_child) // root->right_child->right_child
		{
			fprintf(stderr, "tree_simplify(): error! Error in the tree detected: no right value for power!\n");
			return NULL;
		}
		if (NULL == child->left_child)
		{
			fprintf(stderr, "tree_simplify(): error! Error in the tree detected: no left value for power!\n");
			return NULL;
		}
		struct node* r2_ch = child->right_child;
		struct node* l2_ch = child->left_child;
		if (r2_ch->value[0] == '1' && r2_ch->value[1] == '\0')// L1 <- root -> (L2 <- ^ -> 1) ==> L1 <- root -> L2 || (L2 <- ^ -> 1) <- root -> R1  ==> L2 <- root -> R1
		{
			tree_delete(r2_ch);
			free(child);
			return l2_ch;
		}
		else if (l2_ch->value[0] == '1' && l2_ch->value[1] == '\0')// L1 <- root -> (1 <- ^ -> R2) ==> L1 <- root -> 1 || (1 <- ^ -> R2) <- root -> R1  ==> 1 <- root -> R1
		{
			tree_delete(r2_ch);
			tree_delete(l2_ch);
			free(child);
			return create_node("1\0", NULL, NULL);
		}
		else if (r2_ch->value[0] == '0' && l2_ch->value[1] == '\0' && l2_ch->value[0] == '0' && l2_ch->value[1] == '\0')// L1 <- root -> (0 <- ^ -> 0) ==> L1 <- root -> 1 || (0 <- ^ -> 0) <- root -> R1  ==> 1 <- root -> R1
		{
			tree_delete(r2_ch);
			tree_delete(l2_ch);
			free(child);
			return create_node("1\0", NULL, NULL);
		}
		else if (r2_ch->value[0] == '0' && r2_ch->value[1] == '\0')// L1 <- root -> (L2 <- ^ -> 0) ==> L1 <- root -> 1 || (L2 <- ^ -> 0) <- root -> R1  ==> 1 <- root -> R1
		{
			tree_delete(r2_ch);
			tree_delete(l2_ch);
			free(child);
			return create_node("1\0", NULL, NULL);
		}
		else if (l2_ch->value[0] == '0' && l2_ch->value[1] == '\0')// L1 <- root -> (0 <- ^ -> R2) ==> L1 <- root -> 0 || (0 <- ^ -> R2) <- root -> R1  ==> 0 <- root -> R1
		{
			tree_delete(r2_ch);
			tree_delete(l2_ch);
			free(child);
			return create_node("0\0", NULL, NULL);
		}		
	}
	return NULL;
}

void tree_simplify(struct node* root)
{
	if (NULL == root)
	{
		return;
	}
	tree_simplify(root->left_child);
	tree_simplify(root->right_child);
	if (NULL != root->right_child)//root->right_child
	{
		_r_mm_to_plus(root); //after that only one m to the right of root
		_right_m_handler(root);
		struct node* simplified = _simplify(root, root->right_child);
		if (NULL != simplified)
		{
			root->right_child = simplified;
		}
	}
	if (NULL != root->left_child) //root->left_child
	{
		_l_mm_to_plus(root); //after that only one m to the left of root
		struct node* simplified = _simplify(root, root->left_child);
		if (NULL != simplified)
		{
			root->left_child = simplified;
		}
	}
	return;
}

void tree_delete(struct node* root)
{
	if (NULL == root)
	{
		return;
	}

	tree_delete(root->left_child);
	tree_delete(root->right_child);

	free(root);
}

struct node* tree_copy(struct node* root)
{
	if (root == NULL)
	{
		return NULL;
	}
	struct node* new_parent = create_node("\0", NULL, NULL);
	strcpy(new_parent->value, root->value);
	new_parent->left_child = tree_copy(root->left_child);
	new_parent->right_child = tree_copy(root->right_child);

	return new_parent;
}

int _has_x(struct node* root)
{
    if (NULL == root)
    {
        return 0;
    }

    if (root->value[0] == 'x' && root->value[1] == '\0')
    {
        return 1;
    }
    else
    {
        int has_left = _has_x(root->left_child);
        int has_right = _has_x(root->right_child);

        return has_left || has_right;
    }
}

struct node* tree_diff(struct node* root)
{
	if (NULL == root)
	{
		return NULL;
	}
	if (root->value[0] == '\0') // if the root is beginning of the tree
	{
		root->right_child = tree_diff(root->right_child);
		return root;
	}
	else if (root->value[0] == '+' || root->value[0] == '-')
	{
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diff(): error! Error in the tree detected: no right value for plus or minus!\n");
			return NULL;
		}
		if (NULL == root->left_child)
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no left value for plus or minus!\n");
			return NULL;
		}
		root->left_child  = tree_diff(root->left_child);
		root->right_child = tree_diff(root->right_child);

		return root;
	}
	else if (root->value[0] == '*' )
	{
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no right value for multiply!\n");
			return NULL;
		}
		if (NULL == root->left_child)
		{
			fprintf(stderr, "tree_diff(): error! Error in the tree detected: no left value for multipy!\n");
			return NULL;
		}
		struct node* ln_ch = tree_copy(root->left_child);
		struct node* rn_ch = tree_copy(root->right_child);

		struct node* ld_ch = tree_diff(root->left_child);
		struct node* rd_ch = tree_diff(root->right_child);
		free(root);

		struct node* new_root = create_node("+\0", NULL, NULL);
		new_root->left_child = create_node("*\0", tree_copy(ld_ch), tree_copy(rn_ch));
		new_root->right_child = create_node("*\0", tree_copy(ln_ch), tree_copy(rd_ch));
		
		tree_delete(ln_ch);
		tree_delete(rn_ch);
		tree_delete(ld_ch);
		tree_delete(rd_ch);

		return new_root;
	}
	else if (root->value[0] == 'm' && root->value[1] == '\0')
	{
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no right value for unary minus!\n");
			return NULL;
		}
		root->right_child = tree_diff(root->right_child);
		return root;
	}
	else if (root->value[0] == '/')
	{
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no right value for division!\n");
			return NULL;
		}
		if (NULL == root->left_child)
		{
			fprintf(stderr, "tree_diff(): error! Error in the tree detected: no left value for divison!\n");
			return NULL;
		}
		struct node* ln_ch = tree_copy(root->left_child);
		struct node* rn_ch = tree_copy(root->right_child);

		struct node* ld_ch = tree_diff(root->left_child);
		struct node* rd_ch = tree_diff(root->right_child);

		free(root);

		struct node* new_root = create_node("/\0", 
												create_node("-\0", 
																create_node("*\0", tree_copy(ld_ch), tree_copy(rn_ch)), 
																create_node("*\0", tree_copy(ln_ch), tree_copy(rd_ch)) 
															),
												create_node("*\0", tree_copy(rn_ch), tree_copy(rn_ch))
											);
		tree_delete(ln_ch);
		tree_delete(rn_ch);
		tree_delete(ld_ch);
		tree_delete(rd_ch);
		return new_root;
	}
	else if (strcmp(_SIN, root->value) == 0)
	{
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no right value for sin function!\n");
			return NULL;
		}
		struct node* rn_ch = tree_copy(root->right_child);
		struct node* rd_ch = tree_diff(root->right_child);
		free(root);
		struct node* new_root = create_node("*\0", create_node("cos\0", NULL, rn_ch), rd_ch);
		return new_root;
	}
	else if (strcmp(_COS, root->value) == 0)
	{
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no right value for cos function!\n");
			return NULL;
		}
		struct node* rn_ch = tree_copy(root->right_child);
		struct node* rd_ch = tree_diff(root->right_child);
		free(root);
		struct node* new_root = create_node(
											"*\0",
											create_node(
														"m\0", 
														NULL, 
														create_node("sin\0", NULL, rn_ch)
														), 
											rd_ch
											);
		return new_root;
	}
	else if (strcmp(_LOG, root->value) == 0)
	{
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no right value for log function!\n");
			return NULL;
		}
		struct node* rn_ch = tree_copy(root->right_child);
		struct node* rd_ch = tree_diff(root->right_child);
		free(root);
		struct node* new_root = create_node("/\0",rd_ch, rn_ch);
		return new_root;
	}
	else if (strcmp(_TAN, root->value) == 0)
	{
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no right value for tan function!\n");
			return NULL;
		}
		struct node* rn_ch = tree_copy(root->right_child);
		struct node* rd_ch = tree_diff(root->right_child);
		free(root);
		struct node* coscos = create_node("*\0", 
										 create_node("cos\0", 
										 			NULL, 
													tree_copy(rn_ch)
													),
										create_node("cos\0", 
													NULL,
													tree_copy(rn_ch)
													)
										 );
		struct node* new_root = create_node("/\0", rd_ch, coscos);
		tree_delete(rn_ch);
		return new_root;
	}
    else if (root->value[0] == '^')
    {
		if (NULL == root->right_child) 
		{
			fprintf(stderr, "tree_diif(): error! Error in the tree detected: no right value for power!\n");
			return NULL;
		}
		if (NULL == root->left_child)
		{
			fprintf(stderr, "tree_diff(): error! Error in the tree detected: no left value for power!\n");
			return NULL;
		}
        if (!_has_x(root->left_child))
        {
            if (!_has_x(root->right_child))
            {
                //0
                tree_delete(root->left_child);
                tree_delete(root->right_child);
                free(root);

                return create_node("0\0", NULL, NULL);
            }
            else
            {
		        struct node* ln_ch = tree_copy(root->left_child);
		        struct node* rn_ch = tree_copy(root->right_child);

		        struct node* rd_ch = tree_diff(root->right_child); 
                
                tree_delete(root->left_child);
                free(root);

                struct node* pow = create_node("^\0", tree_copy(ln_ch), tree_copy(rn_ch));
                struct node* mul = create_node("*\0", pow, create_node("log\0", NULL, tree_copy(ln_ch)));
                struct node* new_root = create_node("*\0", mul, tree_copy(rd_ch));

                tree_delete(ln_ch);
                tree_delete(rn_ch);
                tree_delete(rd_ch);

                return new_root;
                //L^R * Log(L)*d(R)
            }
        }
        else
        {
            if(!_has_x(root->right_child))
            {
		        struct node* ln_ch = tree_copy(root->left_child);
		        struct node* rn_ch = tree_copy(root->right_child);

                struct node* ld_ch = tree_diff(root->left_child); 
                
                tree_delete(root->right_child);
                free(root);
                
                struct node* pow = create_node("^\0", tree_copy(ln_ch), create_node("-\0", tree_copy(rn_ch), create_node("1\0", NULL, NULL)));
                struct node* mul = create_node("*\0", tree_copy(rn_ch), pow);
                struct node* new_root = create_node("*\0", mul, tree_copy(ld_ch));

                tree_delete(ln_ch);
                tree_delete(rn_ch);
                tree_delete(ld_ch);

                return new_root;

                //R*L^(R-1)*d(L)
            }
            else
            {
		        struct node* ln_ch = tree_copy(root->left_child);
		        struct node* rn_ch = tree_copy(root->right_child);

		        struct node* ld_ch = tree_diff(root->left_child);
		        struct node* rd_ch = tree_diff(root->right_child);

                free(root);
                
                struct node* frist_mul = create_node("*\0", tree_copy(rd_ch), create_node("log\0", NULL, tree_copy(ln_ch)));
                struct node* second_mul = create_node("*\0", tree_copy(rn_ch), tree_copy(ld_ch));
                struct node* div = create_node("/\0", second_mul, tree_copy(ln_ch));
                struct node* sum = create_node("+\0", frist_mul, div);
                struct node* pow = create_node("^\0", tree_copy(ln_ch), tree_copy(rn_ch));
                struct node* new_root = create_node("*\0", pow, sum);

                tree_delete(ln_ch);
		        tree_delete(rn_ch);
		        tree_delete(ld_ch);
		        tree_delete(rd_ch);
		        return new_root;
                //L^R * (d(R)*Log(L) + (R*dL)/L )
            }
        }        
    }
	else if (root->value[0] == 'x')
	{
		free(root);
		return create_node("1\0", NULL, NULL);
	}
	else if ('0' <= root->value[0] && root->value[0] <= '9')
	{
        free(root);
        root = NULL;
		return create_node("0\0", NULL, NULL);
	}
    else
    {
        fprintf(stderr, "tree_diff(): error: there is no case for diff typed!\n");
        free(root);
        return NULL;
    }
}
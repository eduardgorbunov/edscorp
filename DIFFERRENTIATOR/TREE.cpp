#ifndef TREE_CPP
#define TREE_CPP

//{=================================================================================
//! @file    TREE.h
//! @date    2014-12-05 19:26
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Library for using binary trees
//}=================================================================================

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "ERRORS_TREE.h"
#include "ITS.h"
#include <assert.h>

#define assert_tree(cond) if (!cond)\
                            {\
                            Tree_print_brackets(root);\
                            printf("---------------------------------------------\n");\
                            printf("ERROR in the line number %d\n", __LINE__);\
                            printf("---------------------------------------------\n");\
                            assert(cond);\
                            }

#define POISON 1337

struct EDTree_t;

struct EDTree_t
{
double number;
char* oper;
ITS type;
EDTree_t* left;
EDTree_t* right;
};

EDTree_t* Tree_construct (double number, char* oper, ITS type, EDTree_t* left, EDTree_t* right);

ERRORS_TREE Tree_ok(EDTree_t* root);

ERRORS_TREE Tree_print_brackets (EDTree_t* root);

ERRORS_TREE Tree_print_cpu (EDTree_t* root);

void Tree_destruct (EDTree_t* root);

int main()
{
EDTree_t* root = Tree_construct(1, "ADD", OPERATOR,Tree_construct(2,"PUSH",NUMBER,0,0),
                                Tree_construct(3, "PUSH", NUMBER, 0, 0));
Tree_print_cpu(root);
Tree_destruct(root);
return 0;
}

//{=================================================================================
//! Tree_construct - program which create new node
//!
//! @param   value   - Value of node
//! @param    left   - Pointer to the left subtree
//! @param   right   - Pointer to the right subtree
//!
//! @return     Pointer to the node
//}=================================================================================

EDTree_t* Tree_construct (double number, char* oper, ITS type, EDTree_t* left, EDTree_t* right)
{
EDTree_t* root = (EDTree_t*) calloc (1, sizeof(EDTree_t));

root->oper = (char*) calloc (strlen(oper), sizeof(char));
strcpy(root->oper,oper);
root->number = number;
root->type = type;
root->left = (EDTree_t*) calloc (1, sizeof(EDTree_t));
root->left = left;
root->right = (EDTree_t*) calloc (1, sizeof(EDTree_t));
root->right = right;

ERRORS_TREE nerror = Tree_ok(root);
assert_tree(nerror == OK);

return root;
}

//{=================================================================================
//! Tree_ok - program which check tree for errors
//!
//! @param      root   - Pointer to tree
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok(EDTree_t* root)
{
if (root == NULL) return NULL_POINTER_ON_ROOT;

return OK;
}

//{=================================================================================
//! Tree_print_brackets - program which print tree in brackets-form
//!
//! @param      root   - Pointer to tree
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_print_brackets (EDTree_t* root)
{
if (root == NULL)
  {
  return NULL_POINTER_ON_ROOT;
  }

printf("( ");
switch (root->type)
  {
  case OPERATOR: printf("%s ", root->oper); break;
  case NUMBER: printf("%lg ", root->number); break;
  default: printf("WRONG SWITCH\n"); break;
  }
Tree_print_brackets(root->left);
Tree_print_brackets(root->right);
printf(") ");

return Tree_ok(root);
}

//{=================================================================================
//! Tree_print_cpu - program which print tree in cpu-form
//!
//! @param      root   - Pointer to tree
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_print_cpu (EDTree_t* root)
{
if (root == NULL)
  {
  return NULL_POINTER_ON_ROOT;
  }

Tree_print_cpu(root->left);
Tree_print_cpu(root->right);
switch (root->type)
  {
  case OPERATOR: printf("%s\n", root->oper); break;
  case NUMBER: printf("%s ", root->oper); printf("%lg\n", root->number); break;
  default: printf("WRONG SWITCH\n"); break;
  }

return Tree_ok(root);
}

//{=================================================================================
//! Tree_destruct - program which destruct tree
//!
//! @param   root - Pointer to the root of tree
//}=================================================================================

void Tree_destruct (EDTree_t* root)
{
ERRORS_TREE nerror = Tree_ok(root);
assert_tree(nerror == OK);

if (root->left != NULL) Tree_destruct(root->left);

if (root->right != NULL) Tree_destruct(root->right);

free(root->left);
free(root->right);
free(root->oper);
root->number = POISON;
free(root);
}
#endif

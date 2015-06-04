#ifndef TREE_H
#define TREE_H
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
#define MAX_NUMBER_LENGTH 100
#define MAX_LENGTH_OF_OPER 20
#define MAX_VARIABLE_LENGTH 2
#define MAX_FUNCTION_LENGTH 10
#define MAX_OPERATOR_LENGTH 1

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

ERRORS_TREE Tree_print_cpu (EDTree_t* root, FILE* dump);

ERRORS_TREE Tree_print_function (EDTree_t* root, FILE* dump);

EDTree_t* Tree_copy (EDTree_t* root);

void Tree_destruct (EDTree_t* root);

//{=================================================================================
//! Tree_construct - program which create new node
//!
//! @param  number   - Value of  node (number)
//! @param    oper   - Value of node (operator, function, variable)
//! @param    left   - Pointer to the left subtree
//! @param   right   - Pointer to the right subtree
//!
//! @return     Pointer to the node
//}=================================================================================

EDTree_t* Tree_construct (double number, char* oper, ITS type, EDTree_t* left, EDTree_t* right)
{
EDTree_t* root = (EDTree_t*) calloc (1, sizeof(EDTree_t));
assert(root != NULL);

if (oper != NULL)
  {
  root->oper = (char*) calloc (strlen(oper), sizeof(char));
  strcpy(root->oper,oper);
  }
else
  {
  root->oper = NULL;
  }
root->number = number;
root->type = type;
root->left = left;
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

if (root->type == FUNCTION)
  {
  if (strcmp(root->oper, "SIN") != 0 && strcmp(root->oper, "COS") != 0 &&
      strcmp(root->oper, "TAN") != 0 && strcmp(root->oper, "SQRT") != 0 &&
      strcmp(root->oper, "LOG") != 0 /*strcmp(root->oper, "POW")*/) return WRONG_FUNCTION;
  }

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
  case FUNCTION: printf("%s ", root->oper); break;
  case VARIABLE: switch ((int)root->number)
                   {
                   case 1: printf("ax "); break;
                   case 2: printf("bx "); break;
                   case 3: printf("cx "); break;
                   case 4: printf("dx "); break;
                   default: printf("WRONG VARIABLE\n"); break;
                   }
  case END: break;
  default: printf("CHECK ITS.h\n"); assert(1==0); break;
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

ERRORS_TREE Tree_print_cpu (EDTree_t* root, FILE* dump)
{
assert(dump != NULL);
if (root == NULL)
  {
  return NULL_POINTER_ON_ROOT;
  }

Tree_print_cpu(root->left, dump);
Tree_print_cpu(root->right, dump);
switch (root->type)
  {
  case OPERATOR: fprintf(dump,"%s\n", root->oper);                           break;
  case NUMBER:   fprintf(dump,"PUSH "); fprintf(dump,"%lg\n", root->number); break;
  case FUNCTION: fprintf(dump,"%s\n", root->oper);                           break;
  case VARIABLE: fprintf(dump,"%s ", root->oper);                            break;
  case END:                                                                  break;
  default: printf("CHECK ITS.h\n"); assert(1==0);                            break;
  }

return Tree_ok(root);
}

//{=================================================================================
//! Tree_print_function - program which print tree in function-form
//!
//! @param      root   - Pointer to tree
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_print_function (EDTree_t* root, FILE* dump)
{
assert(dump != NULL);
if (root == NULL)
  {
  return NULL_POINTER_ON_ROOT;
  }

switch (root->type)
  {
  case NUMBER: if (root->number >= 0)
                 {
                 fprintf(dump, "%lg", root->number);
                 }
               else
                 {
                 fprintf(dump, "(");
                 fprintf(dump, "%lg", root->number);
                 fprintf(dump, ")");
                 }
               break;
  case OPERATOR: if (root->oper[0] == '*' || root->oper[0] == '/')
                   {
                   if (root->left->type == OPERATOR)
                     {
                     fprintf(dump, "(");
                     Tree_print_function (root->left, dump);
                     fprintf(dump, ")");
                     }
                   else
                     {
                     Tree_print_function (root->left, dump);
                     }
                   fprintf(dump, "%s", root->oper);
                   if (root->right->type == OPERATOR)
                     {
                     fprintf(dump, "(");
                     Tree_print_function (root->right, dump);
                     fprintf(dump, ")");
                     }
                   else
                     {
                     Tree_print_function (root->right, dump);
                     }
                   }
                 else
                   {
                   Tree_print_function (root->left, dump);
                   fprintf(dump, "%s", root->oper);
                   Tree_print_function (root->right, dump);
                   }
                 break;
  case FUNCTION: fprintf(dump, "%s", root->oper);
                 fprintf(dump, "(");
                 Tree_print_function (root->right, dump);
                 fprintf(dump, ")");
                 break;
  case VARIABLE: fprintf(dump, "%s", root->oper);
                 break;
  case END: break;
  default: printf("CHECK ITS.h\n"); assert(1==0); break;
  }

return Tree_ok(root);
}

//{=================================================================================
//! Tree_copy - program which creates a copy of tree and returns it
//!
//! @param      root   - Pointer to tree
//!
//! @return     Pointer to a copy
//}=================================================================================

EDTree_t* Tree_copy (EDTree_t* root)
{
ERRORS_TREE nerror = Tree_ok(root);
assert_tree(nerror == OK);

EDTree_t* Copy = (EDTree_t*) calloc (1, sizeof(EDTree_t));
Copy->number = root->number;
Copy->type = root->type;
switch (Copy->type)
  {
  case NUMBER: Copy->oper = NULL;
               break;
  case OPERATOR: Copy->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
                 strcpy(Copy->oper, root->oper);
                 break;
  case FUNCTION: Copy->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
                 strcpy(Copy->oper, root->oper);
                 break;
  case VARIABLE: Copy->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
                 strcpy(Copy->oper, root->oper);
                 break;
  case END: Copy->oper = NULL;
            break;
  default: printf("CHECK ITS.h\n"); assert(1==0); break;
  }
if (root->left)  Copy->left = Tree_copy(root->left);
if (root->right) Copy->right = Tree_copy(root->right);

nerror = Tree_ok(Copy);
assert_tree(nerror == OK);

return Copy;
}

//{=================================================================================
//! Tree_destruct - program which destruct tree
//!
//! @param   root - Pointer to the root of tree
//}=================================================================================

void Tree_destruct (EDTree_t* root)
{
//ERRORS_TREE nerror = Tree_ok(root);
//assert_tree(nerror == OK);

if (root != NULL)
  {
  //printf("DESTRUCT ROOT\n");
  if (root->left != NULL) Tree_destruct(root->left);

  if (root->right != NULL) Tree_destruct(root->right);

  //printf("FREE\n");
  //free(root->left);
  //free(root->right);
  if (root->oper != NULL) free(root->oper);
  root->number = POISON;
  free(root);
  }
}
#endif

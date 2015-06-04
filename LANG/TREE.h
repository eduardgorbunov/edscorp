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
#define MAX_OPERATOR_LENGTH 2
#define MAX_CONTROL_LENGTH 50

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

ERRORS_TREE Tree_ok_number (EDTree_t* root);

ERRORS_TREE Tree_ok_operator (EDTree_t* root);

ERRORS_TREE Tree_ok_operator_child (EDTree_t* root);

ERRORS_TREE Tree_ok_function (EDTree_t* root);

ERRORS_TREE Tree_ok_function_child (EDTree_t* root);

ERRORS_TREE Tree_ok_compare (EDTree_t* root);

ERRORS_TREE Tree_ok_compare_child (EDTree_t* root);

ERRORS_TREE Tree_ok_control (EDTree_t* root);

ERRORS_TREE Tree_ok_variable (EDTree_t* root);

ERRORS_TREE Tree_ok_daPrib_child (EDTree_t* root);

ERRORS_TREE Tree_print_brackets (EDTree_t* root);

ERRORS_TREE Tree_print_cpu (EDTree_t* root, FILE* dump);

ERRORS_TREE Tree_print_function (EDTree_t* root, FILE* dump);

void Tree_print_code (EDTree_t* root, FILE* dump);

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

//ERRORS_TREE nerror = Tree_ok(root);
//assert_tree(nerror == OK);

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

switch (root->type)
  {
  case NUMBER:   return Tree_ok_number(root);
                 break;
  case OPERATOR: return Tree_ok_operator(root);
                 break;
  case COMPARE:  return Tree_ok_compare(root);
                 break;
  case FUNCTION: return Tree_ok_function(root);
                 break;
  case VARIABLE: return Tree_ok_variable(root);
                 break;
  case CONTROL:  return Tree_ok_control(root);
                 break;
  case END:      return END_IN_TREE;
                 break;
  default:       return WRONG_ITS_H;
                 break;
  }

return OK;
}

//{=================================================================================
//! Tree_ok_number - program which check node-number
//!
//! @param      root   - Pointer to node-number
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_number (EDTree_t* root)
{
if (root == NULL) return NULL_POINTER_ON_ROOT;

if (root->type != NUMBER) return WRONG_TYPE;

if (root->left != NULL || root->left != NULL) return NUMBER_HAS_CHILD;

return OK;
}

//{=================================================================================
//! Tree_ok_operator - program which check node-operator
//!
//! @param      root   - Pointer to node-operator
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_operator (EDTree_t* root)
{
assert(root != NULL);
if (root->type != OPERATOR) return WRONG_TYPE;

ERRORS_TREE nerror = Tree_ok(root->left);
if (nerror != OK) return nerror;
nerror = Tree_ok(root->right);
if (nerror != OK) return nerror;

assert(root->oper != NULL);
if (root->oper[0] != '+' && root->oper[0] != '-' && root->oper[0] != '*' && root->oper[0] != '/')
   return WRONG_OPERATOR;

if (root->left != NULL)
  {
  nerror = Tree_ok_operator_child(root->left);
  if (nerror != OK) return nerror;
  }
if (root->right != NULL)
  {
  nerror = Tree_ok_operator_child(root->right);
  if (nerror != OK) return nerror;
  }

return OK;
}

//{=================================================================================
//! Tree_ok_operator_child - program which check children of node-operator (types)
//!
//! @param      root   - Pointer to children of node-operator
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_operator_child (EDTree_t* root)
{
assert(root != NULL);

if (root->type == CONTROL || root->type == COMPARE || root->type == END) return OPER_WRONG_CHILD;

ERRORS_TREE nerror = OK;
if (root->left != NULL)
  {
  nerror = Tree_ok_operator_child(root->left);
  if (nerror != OK) return nerror;
  }

if (root->right != NULL)
  {
  nerror = Tree_ok_operator_child(root->right);
  if (nerror != OK) return nerror;
  }

return OK;
}

//{=================================================================================
//! Tree_ok_compare - program which check node-compare
//!
//! @param      root   - Pointer to node-compare
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_compare (EDTree_t* root)
{
assert(root != NULL);

if (root->type != COMPARE) return WRONG_TYPE;

ERRORS_TREE nerror = Tree_ok(root->left);
if (nerror != OK) return nerror;
nerror = Tree_ok(root->right);
if (nerror != OK) return nerror;

assert(root->oper != NULL);
if (strcmp(root->oper, "<\0") != 0 && strcmp(root->oper, "==\0") != 0 &&
    strcmp(root->oper, ">\0") != 0 && strcmp(root->oper, "<=\0") != 0 &&
    strcmp(root->oper, ">=\0") != 0) return WRONG_COMPARE;

nerror = Tree_ok_compare_child(root->left);
if (nerror != OK) return nerror;
nerror = Tree_ok_compare_child(root->right);
if (nerror != OK) return nerror;

return OK;
}

//{=================================================================================
//! Tree_ok_compare_child - program which check children of node-compare (types)
//!
//! @param      root   - Pointer to children of node-compare
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_compare_child (EDTree_t* root)
{
assert(root != NULL);

if (root->type == CONTROL || root->type == COMPARE || root->type == END) return COMP_WRONG_CHILD;

ERRORS_TREE nerror = OK;
if (root->left != NULL)
  {
  nerror = Tree_ok_compare_child(root->left);
  if (nerror != OK) return nerror;
  }

if (root->right != NULL)
  {
  nerror = Tree_ok_compare_child(root->right);
  if (nerror != OK) return nerror;
  }

return OK;
}

//{=================================================================================
//! Tree_ok_function - program which check node-function
//!
//! @param      root   - Pointer to node-function
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_function (EDTree_t* root)
{
assert (root != NULL);
if (root->type != FUNCTION) return WRONG_TYPE;

if (root->left != NULL) return FUNC_WRONG_CHILD;

if (strcmp(root->oper, "SIN") != 0 && strcmp(root->oper, "COS") != 0 &&
    strcmp(root->oper, "TAN") != 0 && strcmp(root->oper, "SQRT") != 0 &&
    strcmp(root->oper, "LOG") != 0) return WRONG_FUNCTION;

ERRORS_TREE nerror = Tree_ok(root->right);
if (nerror != OK) return nerror;

nerror = Tree_ok_function_child(root->right);
if (nerror != OK) return FUNC_WRONG_CHILD;

return OK;
}

//{=================================================================================
//! Tree_ok_function_child - program which check children of node-function (types)
//!
//! @param      root   - Pointer to children of node-function
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_function_child (EDTree_t* root)
{
assert(root != NULL);

if (root->type == COMPARE || root->type == END || root->type == CONTROL) return FUNC_WRONG_CHILD;

ERRORS_TREE nerror = OK;
if (root->left != NULL)
  {
  nerror = Tree_ok_function_child(root->left);
  if (nerror != OK) return FUNC_WRONG_CHILD;
  }
if (root->right != NULL)
  {
  nerror = Tree_ok_function_child(root->right);
  if (nerror != OK) return FUNC_WRONG_CHILD;
  }

return OK;
}

//{=================================================================================
//! Tree_ok_variable - program which check node-variable
//!
//! @param      root   - Pointer to node-variable
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_variable (EDTree_t* root)
{
assert (root != NULL);

if (root->type != VARIABLE) return WRONG_TYPE;

if (root->left != NULL || root->right != NULL) return VAR_HAS_CHILD;

assert(root->oper != NULL);

if (strcmp(root->oper, "ax\0") != 0 && strcmp(root->oper, "bx\0") != 0 &&
    strcmp(root->oper, "cx\0") != 0 && strcmp(root->oper, "dx\0") != 0) return WRONG_VARIABLE;

return OK;
}

//{=================================================================================
//! Tree_ok_control - program which check node-control
//!
//! @param      root   - Pointer to node-control
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_control (EDTree_t* root)
{
int wasChecking = 0;

assert(root != NULL);
if (root->type != CONTROL) return WRONG_TYPE;
assert(root->oper != NULL);

if (strcmp(root->oper, "я_стою_выше_тебя\0") == 0)
  {
  wasChecking = 1;
  ERRORS_TREE nerror = Tree_ok (root->left);
  if (nerror != OK) return nerror;

  if (root->right == NULL) return CONT_WRONG_CHILD;

  if (root->right->type != CONTROL || root->right->oper == NULL ||
      root->right->left != NULL || root->right->right != NULL ||
      strcmp(root->right->oper, "ты_недооцениваешь_мою_мощь\0") != 0) return CONT_WRONG_CHILD;
  }

if (strcmp(root->oper, "да_прибудет_с_тобой\0") == 0)
  {
  wasChecking = 1;
  if (root->left == NULL || root->left->type != VARIABLE) return CONT_WRONG_CHILD;

  ERRORS_TREE nerror = Tree_ok(root->right);
  if (nerror != OK) return nerror;

  nerror = Tree_ok_daPrib_child(root->right);
  if (nerror != OK) return nerror;
  }

if (strcmp(root->oper, "звездолёт\0") == 0)
  {
  wasChecking = 1;
  ERRORS_TREE nerror = Tree_ok(root->left);
  if (nerror != OK) return nerror;
  nerror = Tree_ok(root->right);
  if (nerror != OK) return nerror;
  }

if (strcmp(root->oper, "сила\0") == 0)
  {
  wasChecking = 1;
  ERRORS_TREE nerror = Tree_ok(root->left);
  if (nerror != OK) return nerror;

  if (root->left->type != COMPARE) return FORCE_WRONG_CHILD;
  if (root->right == NULL) return FORCE_WRONG_CHILD;
  if (root->right->type != CONTROL || root->right->oper == NULL ||
      strcmp(root->right->oper, "светлая_сторона\0") != 0) return FORCE_WRONG_CHILD;
  nerror = Tree_ok(root->right->left);
  if (nerror != OK) return nerror;

  if (root->right->left->type != CONTROL || root->right->left->oper == NULL ||
      strcmp(root->right->left->oper, "я_стою_выше_тебя\0") != 0) return FORCE_WRONG_CHILD;

  if (root->right->right != NULL)
    {
    if (root->right->right->type != CONTROL || root->right->right->oper == NULL ||
        strcmp(root->right->right->oper, "тёмная_сторона\0") != 0) return FORCE_WRONG_CHILD;

    if (root->right->right->right != NULL) return FORCE_WRONG_CHILD;

    nerror = Tree_ok(root->right->right->left);
    if (nerror != OK) return nerror;

    if (root->right->right->left->type != CONTROL || root->right->right->left->oper == NULL ||
        strcmp(root->right->right->left->oper, "я_стою_выше_тебя\0") != 0)
      return FORCE_WRONG_CHILD;
    }
  }

if (strcmp(root->oper, "йода\0") == 0)
  {
  wasChecking = 1;
  ERRORS_TREE nerror = Tree_ok(root->left);
  if (nerror != OK) return nerror;

  if (root->left->type != COMPARE) return YODA_WRONG_CHILD;
  if (root->right == NULL) return YODA_WRONG_CHILD;

  if (root->right->type != CONTROL || root->right->oper == NULL ||
      strcmp(root->right->oper, "я_стою_выше_тебя\0") != 0) return YODA_WRONG_CHILD;
  }

if (strcmp(root->oper, "голограмма\0") == 0)
  {
  wasChecking = 1;
  ERRORS_TREE nerror = Tree_ok(root->right);
  if (nerror != OK) return nerror;

  if (root->right->type != VARIABLE) return GOLOG_WRONG_CHILD;
  }

if (strcmp(root->oper, "новый_дроид\0") == 0)
  {
  wasChecking = 1;
  ERRORS_TREE nerror = Tree_ok(root->right);
  if (nerror != OK) return nerror;

  if (root->right->type != VARIABLE) return NEWDR_WRONG_CHILD;
  }

if (wasChecking == 0)
  {
  printf ("UNKNOWN CONTROL OPERATOR\n");
  return BAD_INPUT;
  }

return OK;
}

//{=================================================================================
//! Tree_ok_daPrib_child - program which check children of node-assignment
//!
//! @param      root   - Pointer to children of node-assignment
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_TREE Tree_ok_daPrib_child (EDTree_t* root)
{
assert(root != NULL);

if (root->type == CONTROL ||root->type == END || root->type == COMPARE) return DAPRIB_WRONG_CHILD;

ERRORS_TREE nerror = OK;
if (root->left != NULL)
  {
  nerror = Tree_ok_daPrib_child(root->left);
  if (nerror != OK) return nerror;
  }

if (root->right != NULL)
  {
  nerror = Tree_ok_daPrib_child(root->right);
  if (nerror != OK) return nerror;
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
  case OPERATOR: fprintf(dump,"%s\n", root->oper); break;
  case NUMBER: fprintf(dump,"PUSH "); fprintf(dump,"%lg\n", root->number); break;
  case FUNCTION: fprintf(dump,"%s\n", root->oper); break;
  case VARIABLE: fprintf(dump,"%s ", root->oper); break;
  case END: break;
  default: printf("CHECK ITS.h\n"); assert(1==0); break;
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
//! Tree_print_code - program which print tree like a code in StarWars-language
//!
//! @param      root   - Pointer to tree
//!
//! @return     Number of an error
//}=================================================================================

void Tree_print_code (EDTree_t* root, FILE* dump)
{
assert(dump != NULL);
if (root != NULL)
  {
  switch (root->type)
    {
    case NUMBER:   fprintf(dump, "%lg ", root->number);
                   break;
    case VARIABLE: fprintf(dump, "%s ", root->oper);
                   break;
    case CONTROL:  if (root->oper != NULL && strcmp(root->oper, "я_стою_выше_тебя\0") == 0)
                     {
                     fprintf(dump, "%s\n", root->oper);
                     Tree_print_code(root->left, dump);
                     Tree_print_code(root->right, dump);
                     fprintf(dump, "\n");
                     }
                   else
                     {
                     if (root->oper != NULL && strcmp(root->oper, "звездолёт\0") == 0)
                       {
                       Tree_print_code(root->left, dump);
                       fprintf(dump, "%s\n", root->oper);
                       Tree_print_code(root->right, dump);
                       }
                     else
                       {
                       if (root->oper != NULL && strcmp(root->oper, "сила\0") == 0)
                         {
                         fprintf(dump, "%s( ", root->oper);
                         Tree_print_code(root->left, dump);
                         fprintf(dump, ")\n");
                         fprintf (dump, "светлая_сторона\n");
                         Tree_print_code(root->right->left, dump);
                         if (root->right->right != NULL)
                           {
                           fprintf(dump, "тёмная_сторона\n");
                           Tree_print_code(root->right->right->left, dump);
                           }
                         }
                       else
                         {
                         if (root->oper != NULL && strcmp(root->oper, "йода\0") == 0)
                           {
                           fprintf(dump, "%s( ", root->oper);
                           Tree_print_code(root->left, dump);
                           fprintf(dump, ")\n");
                           Tree_print_code(root->right, dump);
                           }
                         else
                           {
                           Tree_print_code(root->left, dump);
                           fprintf(dump, "%s ", root->oper);
                           Tree_print_code(root->right, dump);
                           }
                         }
                       }
                     }
                   break;
    case OPERATOR: if (root->oper != NULL && root->oper[0] == '(')
                     {
                     fprintf(dump, "%s ", root->oper);
                     Tree_print_code(root->left, dump);
                     Tree_print_code(root->right, dump);
                     }
                   else
                     {
                     Tree_print_code(root->left, dump);
                     fprintf(dump, "%s ", root->oper);
                     Tree_print_code(root->right, dump);
                     }
                   break;
    case COMPARE:  Tree_print_code(root->left, dump);
                   fprintf(dump, "%s ", root->oper);
                   Tree_print_code(root->right, dump);
                   break;
    case FUNCTION: fprintf(dump, "%s( ", root->oper);
                   Tree_print_code(root->right, dump);
                   fprintf(dump, ") ");
                   break;
    case END:      break;
    default:       printf("CHECK ITS.h\n");
                   assert(NULL);
                   break;
    }
  }
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
//ERRORS_TREE nerror = Tree_ok(root);
//assert_tree(nerror == OK);
assert(root != NULL);
EDTree_t* Copy = (EDTree_t*) calloc (1, sizeof(EDTree_t));
Copy->number = root->number;
Copy->type = root->type;
switch (Copy->type)
  {
  case NUMBER: Copy->oper = NULL;
               break;
  case OPERATOR: Copy->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
                 assert(root->oper != NULL);
                 strcpy(Copy->oper, root->oper);
                 break;
  case FUNCTION: Copy->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
                 assert(root->oper != NULL);
                 strcpy(Copy->oper, root->oper);
                 break;
  case VARIABLE: Copy->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
                 assert(root->oper != NULL);
                 strcpy(Copy->oper, root->oper);
                 break;
  case CONTROL: Copy->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
                assert(root->oper != NULL);
                strcpy(Copy->oper, root->oper);
                break;
  case COMPARE: Copy->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
                assert(root->oper != NULL);
                strcpy(Copy->oper, root->oper);
                break;
  case END: Copy->oper = NULL;
            break;
  default: printf("CHECK ITS.h\n"); assert(1==0); break;
  }
if (root->left != NULL) Copy->left = Tree_copy(root->left);
if (root->right != NULL) Copy->right = Tree_copy(root->right);

//nerror = Tree_ok(Copy);
//assert_tree(nerror == OK);

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

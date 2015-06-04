#ifndef TREE_CODE_CPP
#define TREE_CODE_CPP
//{=================================================================================
//! @file    TREE_CODE.cpp
//! @date    2014-12-13 00:14
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Program, which get code-tree from code from file
//}=================================================================================

#include <conio.h>
#include "TREE.h"
#include "LEXIC_ANALYSIS_LANG.cpp"

int NUM_OF_LABEL = 0;

EDTree_t* Tree_getCode (FILE* from);

void Tree_code (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_control (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_daPrib (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_iStand (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_force (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_yoda (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_gologram (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_newdroid (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_operator (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_function (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_compare (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_code_zvezd (EDToken_t* tokens, int* position, EDTree_t** root);

void Tree_del_brackets (EDTree_t** root);

void Tree_print_code_cpu (EDTree_t* root, FILE* dump);

void Tree_print_code_cpu_operator (EDTree_t* root, FILE* dump);

void Tree_print_code_cpu_compare (EDTree_t* root, FILE* dump);

void Tree_print_code_cpu_control (EDTree_t* root, FILE* dump);

int main(int argc, const char* argv[])
{
FILE* from = fopen ((argv[1]? argv[1] : "FACT.TXT"), "r");
assert(from != NULL);
setlocale(LC_ALL, "Russian");
EDTree_t* root = Tree_getCode(from);
fclose(from);
//setlocale(LC_ALL, "Russian");
FILE* out = fopen ("CPUCODE.TXT", "w");
Tree_print_code_cpu(root, out);
fprintf(out, "END");
fclose(out);
Tree_destruct(root);

if (argv[1])
  {
  /*printf("\n");
  printf("Write something\n");
  double forMain = 0;
  scanf("%lg", &forMain);*/
  getch();
  }
return 0;
}

//{=================================================================================
//! Tree_getCode - program which transmits code in StarWars-language in tree
//!
//! @param       from       - Pointer to a file with code
//!
//! @return      pointer to the root of tree-code
//}=================================================================================

EDTree_t* Tree_getCode (FILE* from)
{
assert(from != NULL);

EDToken_t* tokens = NULL;
Lang_lexic(from, &tokens);
printf("AFTER LEXIC\n");
int position = 0;
EDTree_t* root = NULL;
Tree_code (tokens, &position, &root);

printf("AFTER TREE_CODE\n");
FILE* out = fopen ("OUTCODE.TXT", "w");
assert(out != NULL);
Tree_print_code(root, out);
fclose(out);

Tree_del_brackets(&root);
if (tokens[position].type != END)
  {
  printf("TREE_FUNC: YOUR LEXIC ANALYSIS IS WRONG\n");
  assert(NULL);
  }

if (Tree_ok(root) != OK) printf("TREE IS FULL OF ERRORS\n");

Tokens_destruct(tokens);
return root;
}

//{=================================================================================
//! Tree_code - program which transmits code in StarWars-language in tree (recursive)
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
printf("%d\n", *position);

switch (tokens[*position].type)
  {
  case NUMBER:   assert((*root) == NULL);
                 (*root) = Tree_construct(tokens[*position].number, 0, NUMBER, 0, 0);
                 (*position)++;
                 if (tokens[*position].type != CONTROL || tokens[*position].oper == NULL ||
                     (strcmp(tokens[*position].oper, "звездолёт\0") != 0 &&
                      strcmp(tokens[*position].oper, "ты_недооцениваешь_мою_мощь\0") != 0))
                   {
                   Tree_code(tokens, position, root);
                   }
                 break;
  case VARIABLE: assert((*root) == NULL);
                 (*root) = Tree_construct(0, tokens[*position].oper, VARIABLE, 0, 0);
                 (*position)++;
                 if (tokens[*position].type != CONTROL || tokens[*position].oper == NULL ||
                     (strcmp(tokens[*position].oper, "звездолёт\0") != 0 &&
                      strcmp(tokens[*position].oper, "ты_недооцениваешь_мою_мощь\0") != 0))
                   {
                   Tree_code(tokens, position, root);
                   }
                 break;
  case CONTROL:  Tree_code_control (tokens, position, root);
                 break;
  case FUNCTION: Tree_code_function(tokens, position, root);
                 break;
  case OPERATOR: if (tokens[*position].oper[0] != ')') Tree_code_operator(tokens, position, root);
                 break;
  case COMPARE:  Tree_code_compare(tokens, position, root);
                 break;
  case END:      printf("END\n");
                 break;
  default:       printf("CHECK ITS.h\n");
                 assert(NULL);
                 break;
  }
}

//{=================================================================================
//! Tree_code_control - program which transmits control-operators in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_control (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);

if (strcmp(tokens[*position].oper, "да_прибудет_с_тобой\0") == 0)
  {
  Tree_code_daPrib(tokens, position, root);
  }

if (tokens[*position].oper != NULL &&
    strcmp(tokens[*position].oper, "я_стою_выше_тебя\0") == 0)
  {
  Tree_code_iStand(tokens, position, root);
  }

if (tokens[*position].oper != NULL &&
    strcmp(tokens[*position].oper, "звездолёт\0") == 0)
  {
  Tree_code_zvezd(tokens, position, root);
  }
if (tokens[*position].oper != NULL &&
    strcmp(tokens[*position].oper, "сила\0") == 0)
  {
  Tree_code_force(tokens, position, root);
  }

if (tokens[*position].oper != NULL &&
    strcmp(tokens[*position].oper, "йода\0") == 0)
  {
  Tree_code_yoda(tokens, position, root);
  }

if (tokens[*position].oper != NULL &&
    strcmp(tokens[*position].oper, "голограмма\0") == 0)
  {
  Tree_code_gologram(tokens, position, root);
  }

if (tokens[*position].oper != NULL &&
    strcmp(tokens[*position].oper, "новый_дроид\0") == 0)
  {
  Tree_code_newdroid(tokens, position, root);
  }

}

//{=================================================================================
//! Tree_code_daPrib - program which transmits the assignment operator in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_daPrib (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert((*root) != NULL);
assert((*root)->type == VARIABLE);
assert(tokens[*position].oper != NULL);
assert(strcmp(tokens[*position].oper, "да_прибудет_с_тобой\0") == 0);

EDTree_t* left = NULL;
left = Tree_copy((*root));
Tree_destruct((*root));
(*root) = Tree_construct(0, "да_прибудет_с_тобой\0", CONTROL, left, 0);
assert(tokens[(*position)+1].type != OPERATOR);
assert(tokens[(*position)+1].type != END);
assert(tokens[(*position)+1].type != CONTROL);

(*position)++;
Tree_code(tokens, position, &((*root)->right));
}

//{=================================================================================
//! Tree_code_iStand - program which transmits the group operator in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_iStand (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert((*root) == NULL);
assert(tokens[*position].oper != NULL);
assert(strcmp(tokens[*position].oper, "я_стою_выше_тебя\0") == 0);

EDTree_t* left = NULL;
(*position)++;
Tree_code(tokens, position, &left);
(*root) = Tree_construct(0, "я_стою_выше_тебя\0", CONTROL, left, 0);

printf("%d %s\n", (*position), tokens[*position].oper);
assert(tokens[*position].oper != NULL);
assert(tokens[*position].type == CONTROL);
assert(strcmp(tokens[*position].oper, "ты_недооцениваешь_мою_мощь\0") == 0);
(*position)++;
(*root)->right = Tree_construct(0, "ты_недооцениваешь_мою_мощь\0", CONTROL, 0, 0);
}

//{=================================================================================
//! Tree_code_zvezd - program which transmits the operator of end of string in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_zvezd (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert((*root) != NULL);
assert(tokens[*position].oper != NULL);
assert(strcmp(tokens[*position].oper, "звездолёт\0") == 0);

if (tokens[(*position)+1].type == END || (tokens[(*position)+1].type == CONTROL &&
    tokens[(*position)+1].oper != NULL &&
    strcmp(tokens[(*position)+1].oper, "ты_недооцениваешь_мою_мощь\0") == 0))
  {
  (*position)++;
  }
else
  {
  EDTree_t* left = Tree_copy((*root));
  (*root) = Tree_construct(0, "звездолёт\0", CONTROL, left, NULL);
  (*position)++;
  Tree_code(tokens, position, &((*root)->right));
  }
}

//{=================================================================================
//! Tree_code_force - program which transmits the operator of end of string in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_force (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert((*root) == NULL);
assert(tokens[*position].oper != NULL);
assert(strcmp(tokens[*position].oper, "сила\0") == 0);
(*position)++;
assert(tokens[(*position)].type == OPERATOR && tokens[(*position)].oper != NULL &&
       tokens[(*position)].oper[0] == '(');

(*position)++;
EDTree_t* left = NULL;
Tree_code(tokens, position, &left);
assert(tokens[(*position)].type == OPERATOR && tokens[(*position)].oper != NULL &&
       tokens[(*position)].oper[0] == ')');
(*position)++;
(*root) = Tree_construct(0, "сила\0", CONTROL, left, NULL);
assert(tokens[(*position)].type == CONTROL && tokens[(*position)].oper != NULL &&
       strcmp(tokens[(*position)].oper, "светлая_сторона\0") == 0);
(*position)++;
(*root)->right = Tree_construct(0, "светлая_сторона\0", CONTROL, NULL, NULL);
assert(tokens[*position].type == CONTROL && tokens[*position].oper != NULL &&
       strcmp(tokens[*position].oper, "я_стою_выше_тебя\0") == 0);

Tree_code_iStand(tokens, position, &((*root)->right->left));

if (tokens[*position].type == CONTROL && tokens[*position].oper != NULL &&
    strcmp(tokens[*position].oper, "тёмная_сторона\0") == 0)
  {
  (*root)->right->right = Tree_construct(0, "тёмная_сторона\0", CONTROL, NULL, NULL);
  (*position)++;
  assert(tokens[*position].type == CONTROL && tokens[*position].oper != NULL &&
         strcmp(tokens[*position].oper, "я_стою_выше_тебя\0") == 0);
  Tree_code_iStand(tokens, position, &((*root)->right->right->left));
  }

Tree_code(tokens, position, root);
}

//{=================================================================================
//! Tree_code_yoda - program which transmits the yoda-operator (cycle) in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_yoda (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert((*root) == NULL);
assert(tokens[*position].oper != NULL);
assert(strcmp(tokens[*position].oper, "йода\0") == 0);
(*position)++;
assert(tokens[(*position)].type == OPERATOR && tokens[(*position)].oper != NULL &&
       tokens[(*position)].oper[0] == '(');

(*position)++;
EDTree_t* left = NULL;
Tree_code(tokens, position, &left);
assert(tokens[(*position)].type == OPERATOR && tokens[(*position)].oper != NULL &&
       tokens[(*position)].oper[0] == ')');
(*position)++;
(*root) = Tree_construct(0, "йода\0", CONTROL, left, NULL);

assert(tokens[*position].type == CONTROL && tokens[*position].oper != NULL &&
       strcmp(tokens[*position].oper, "я_стою_выше_тебя\0") == 0);

Tree_code_iStand(tokens, position, &((*root)->right));
Tree_code(tokens, position, root);
}

//{=================================================================================
//! Tree_code_gologram - program which transmits the gologram-operator in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_gologram (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert((*root) == NULL);
assert(tokens[*position].oper != NULL);
assert(strcmp(tokens[*position].oper, "голограмма\0") == 0);
(*position)++;
assert(tokens[(*position)].type == OPERATOR && tokens[(*position)].oper != NULL &&
       tokens[(*position)].oper[0] == '(');
(*position)++;

assert(tokens[*position].type == VARIABLE && tokens[*position].oper != NULL);
(*root) = Tree_construct(0, tokens[(*position)-2].oper, CONTROL, 0,
          Tree_construct(0, tokens[*position].oper, VARIABLE, 0, 0));

(*position)++;
assert(tokens[(*position)].type == OPERATOR && tokens[(*position)].oper != NULL &&
       tokens[(*position)].oper[0] == ')');
(*position)++;
Tree_code(tokens, position, root);
}

//{=================================================================================
//! Tree_code_newdroid - program which transmits the newdroid-operator in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_newdroid (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert((*root) == NULL);
assert(tokens[*position].oper != NULL);
assert(strcmp(tokens[*position].oper, "новый_дроид\0") == 0);
(*position)++;
assert(tokens[(*position)].type == OPERATOR && tokens[(*position)].oper != NULL &&
       tokens[(*position)].oper[0] == '(');
(*position)++;

assert(tokens[*position].type == VARIABLE && tokens[*position].oper != NULL);
(*root) = Tree_construct(0, tokens[(*position)-2].oper, CONTROL, 0,
          Tree_construct(0, tokens[*position].oper, VARIABLE, 0, 0));

(*position)++;
assert(tokens[(*position)].type == OPERATOR && tokens[(*position)].oper != NULL &&
       tokens[(*position)].oper[0] == ')');
(*position)++;
Tree_code(tokens, position, root);
}

//{=================================================================================
//! Tree_code_operator - program which transmits operators for variables and numbers
//!                      in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_operator (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert(tokens[*position].oper != NULL);
assert(tokens[*position].type == OPERATOR);

EDTree_t* left = NULL;
if (tokens[*position].oper[0] == '(')
  {
  assert((*root) == NULL);
  (*position)++;
  Tree_code(tokens, position, &left);
  assert(left != NULL);
  assert(tokens[*position].oper != NULL);
  assert(tokens[*position].type == OPERATOR);
  assert(tokens[*position].oper[0] == ')');

  (*root) = Tree_construct(0, "(\0", OPERATOR, left,
            Tree_construct(0, ")\0", OPERATOR, 0, 0));
  (*position)++;
  Tree_code(tokens, position, root);
  }
else
  {
  assert((*root) != NULL);
  left = Tree_copy(*root);
  Tree_destruct(*root);
  (*root) = Tree_construct(0, tokens[*position].oper, OPERATOR, left, 0);
  (*position)++;
  Tree_code(tokens, position, &((*root)->right));
  }
}

//{=================================================================================
//! Tree_code_function - program which transmits math functions in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_function (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert((*root) == NULL);
assert(tokens[(*position)+1].oper != NULL);
assert(tokens[(*position)+1].type == OPERATOR);
assert(tokens[(*position)+1].oper[0] == '(');

(*root) = Tree_construct(0, tokens[(*position)].oper, FUNCTION, NULL, NULL);
(*position) = (*position) + 2;
Tree_code(tokens, position, &((*root)->right));
assert((*root)->right != NULL);
assert(tokens[*position].oper != NULL);
assert(tokens[*position].type == OPERATOR);
assert(tokens[*position].oper[0] == ')');
(*position)++;
if (tokens[*position].type != CONTROL || tokens[*position].oper == NULL ||
    (strcmp(tokens[*position].oper, "звездолёт\0") != 0 &&
    strcmp(tokens[*position].oper, "ты_недооцениваешь_мою_мощь\0") != 0))
  {
  Tree_code(tokens, position, root);
  }
}

//{=================================================================================
//! Tree_code_compare - program which transmits comparison operators
//!                      for variables and numbers in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//! @param[out]  root     - Pointer to pointer to the root of previous tree-code
//}=================================================================================

void Tree_code_compare (EDToken_t* tokens, int* position, EDTree_t** root)
{
assert(tokens != NULL && position != NULL && root != NULL);
assert(tokens[*position].oper != NULL);
assert(tokens[*position].type == COMPARE);
assert((*root) != NULL);

EDTree_t* left = Tree_copy(*root);
Tree_destruct(*root);
if (tokens[*position].oper[0] == '=')
  {
  (*position)++;
  assert(tokens[*position].type == COMPARE);
  assert(tokens[*position].oper != NULL);
  assert(tokens[*position].oper[0] == '=');

  (*root) = Tree_construct(0, "==\0", COMPARE, left, 0);
  (*position)++;
  assert(tokens[*position].type != COMPARE);
  Tree_code(tokens, position, &((*root)->right));
  }
else
  {
  (*position)++;
  if (tokens[*position].type == COMPARE)
    {
    assert(tokens[*position].oper != NULL);
    assert(tokens[*position].oper[0] == '=');

    strcat(tokens[(*position)-1].oper, "=\0");
    (*root) = Tree_construct(0, tokens[(*position)-1].oper, COMPARE, left, 0);
    (*position)++;
    assert(tokens[*position].type != COMPARE);
    Tree_code(tokens, position, &((*root)->right));
    }
  else
    {
    (*root) = Tree_construct(0, tokens[(*position)-1].oper, COMPARE, left, 0);
    Tree_code(tokens, position, &((*root)->right));
    }
  }
}

//{=================================================================================
//! Tree_del_brackets - program which deletes brackets from tree
//!
//! @param[out]  root     - Pointer to pointer to the root
//}=================================================================================

void Tree_del_brackets (EDTree_t** root)
{
assert(root != NULL);
assert((*root) != NULL);

if ((*root)->left != NULL) Tree_del_brackets(&((*root)->left));

if ((*root)->type == OPERATOR && (*root)->oper != NULL && (*root)->oper[0] == '(')
  {
  EDTree_t* temp = Tree_copy((*root)->left);
  Tree_destruct((*root));
  (*root) = Tree_copy(temp);
  Tree_destruct(temp);
  }
else
  {
  if ((*root)->right != NULL) Tree_del_brackets(&((*root)->right));
  }
}

//{=================================================================================
//! Tree_print_code_cpu - program which print tree in cpu-form
//!
//! @param      root   - Pointer to tree
//! @param      dump   - Pointer to a file for printing
//!
//! @return     Number of an error
//}=================================================================================

void Tree_print_code_cpu (EDTree_t* root, FILE* dump)
{
assert(dump != NULL);

if (root != NULL)
  {
  switch (root->type)
    {
    case NUMBER:   fprintf(dump, "PUSH %lg\n", root->number);
                   break;
    case VARIABLE: assert(root->oper != NULL);
                   fprintf(dump, "PUSHR %s\n", root->oper);
                   break;
    case OPERATOR: Tree_print_code_cpu_operator(root, dump);
                   break;
    case COMPARE:  Tree_print_code_cpu_compare(root, dump);
                   break;
    case FUNCTION: Tree_print_code_cpu(root->right, dump);
                   assert(root->oper != NULL);
                   fprintf(dump, "%s\n", root->oper);
                   break;
    case CONTROL:  Tree_print_code_cpu_control(root, dump);
                   break;
    case END:      printf("END IN TREE\n");
                   assert(NULL);
                   break;
    default:       printf("CHECK ITS.h\n");
                   assert(NULL);
                   break;
    }
  }
}

//{=================================================================================
//! Tree_print_code_cpu_operator - program which print node-operator in cpu-form
//!
//! @param      root   - Pointer to node-operator
//! @param      dump   - Pointer to a file for printing
//!
//! @return     Number of an error
//}=================================================================================

void Tree_print_code_cpu_operator (EDTree_t* root, FILE* dump)
{
assert(dump != NULL && root != NULL);
assert(root->oper != NULL);

switch (root->oper[0])
  {
  case '+': Tree_print_code_cpu(root->left, dump);
            Tree_print_code_cpu(root->right, dump);
            fprintf(dump, "ADD\n");
            break;
  case '-': Tree_print_code_cpu(root->left, dump);
            Tree_print_code_cpu(root->right, dump);
            fprintf(dump, "PUSH -1\n");
            fprintf(dump, "MUL\n");
            fprintf(dump, "ADD\n");
            break;
  case '*': Tree_print_code_cpu(root->left, dump);
            Tree_print_code_cpu(root->right, dump);
            fprintf(dump, "MUL\n");
            break;
  case '/': Tree_print_code_cpu(root->right, dump);
            Tree_print_code_cpu(root->left, dump);
            fprintf(dump, "DIV\n");
            break;
  default:  printf("WRONG OPERATOR\n");
            assert(NULL);
            break;
  }
}

//{=================================================================================
//! Tree_print_code_cpu_compare - program which print node-compare in cpu-form
//!
//! @param      root   - Pointer to node-compare
//! @param      dump   - Pointer to a file for printing
//!
//! @return     Number of an error
//}=================================================================================

void Tree_print_code_cpu_compare (EDTree_t* root, FILE* dump)
{
assert(dump != NULL && root != NULL);
assert(root->oper != NULL);

Tree_print_code_cpu(root->left, dump);
Tree_print_code_cpu(root->right, dump);
switch(root->oper[0])
  {
  case '<': if (root->oper[1] == '=')
              {
              fprintf (dump, "JBES :%d\n", NUM_OF_LABEL);
              }
            else
              {
              fprintf(dump, "JBS :%d\n", NUM_OF_LABEL);
              }
            break;
  case '=': fprintf (dump, "JES :%d\n", NUM_OF_LABEL);
            break;
  case '>': if (root->oper[1] == '=')
              {
              fprintf (dump, "JAES :%d\n", NUM_OF_LABEL);
              }
            else
              {
              fprintf(dump, "JAS :%d\n", NUM_OF_LABEL);
              }
            break;
  default:  printf("WRONG COMPARISON OPERATOR\n");
            assert(NULL);
            break;
  }
}

//{=================================================================================
//! Tree_print_code_cpu_control - program which print node-control in cpu-form
//!
//! @param      root   - Pointer to node-control
//! @param      dump   - Pointer to a file for printing
//!
//! @return     Number of an error
//}=================================================================================

void Tree_print_code_cpu_control (EDTree_t* root, FILE* dump)
{
assert(root != NULL && dump != NULL);
assert(root->oper != NULL);

if (strcmp(root->oper, "я_стою_выше_тебя\0") == 0)
  {
  Tree_print_code_cpu(root->left, dump);
  }

if (strcmp(root->oper, "да_прибудет_с_тобой\0") == 0)
  {
  Tree_print_code_cpu(root->right, dump);
  assert(root->left->type == VARIABLE);
  fprintf(dump, "POPR %s\n", root->left->oper);
  }

if (strcmp(root->oper, "звездолёт\0") == 0)
  {
  Tree_print_code_cpu(root->left, dump);
  Tree_print_code_cpu(root->right, dump);
  }

if (strcmp(root->oper, "йода\0") == 0)
  {
  fprintf(dump, ":%d\n", NUM_OF_LABEL);
  NUM_OF_LABEL++;
  Tree_print_code_cpu(root->left, dump);
  NUM_OF_LABEL++;
  fprintf(dump, "JMP :%d\n", NUM_OF_LABEL);
  fprintf(dump, ":%d\n", NUM_OF_LABEL-1);
  int pocket = NUM_OF_LABEL;
  NUM_OF_LABEL++;
  Tree_print_code_cpu(root->right, dump);
  fprintf(dump, "JMP :%d\n", pocket-2);
  fprintf(dump, ":%d\n", pocket);
  }

if (strcmp(root->oper, "сила\0") == 0)
  {
  Tree_print_code_cpu(root->left, dump);
  NUM_OF_LABEL = NUM_OF_LABEL + 2;
  int pocket = NUM_OF_LABEL - 1;
  Tree_print_code_cpu(root->right->right->left, dump);
  fprintf(dump, "JMP :%d\n", pocket);
  fprintf(dump, ":%d\n", pocket-1);
  Tree_print_code_cpu(root->right->left, dump);
  fprintf(dump, ":%d\n", pocket);
  }

if (strcmp(root->oper, "голограмма\0") == 0)
  {
  fprintf(dump, "OUTR %s\n", root->right->oper);
  }

if (strcmp(root->oper, "новый_дроид\0") == 0)
  {
  fprintf(dump, "IN %s\n", root->right->oper);
  }
}

#endif

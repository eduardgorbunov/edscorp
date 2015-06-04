#ifndef TREE_FUNC_CPP
#define TREE_FUNC_CPP
//{=================================================================================
//! @file    TREE_FUNC.cpp
//! @date    2014-12-10 00:05
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Program, which transmits mathematical function in tree by using
//! the recursive descent
//}=================================================================================

#include "TREE.h"
#include "LEXIC_ANALYSIS.cpp"

EDTree_t* Tree_getFunc (FILE* from, char** variable);

EDTree_t* Tree_GetE (EDToken_t* tokens, int* position);

EDTree_t* Tree_GetT (EDToken_t* tokens, int* position);

EDTree_t* Tree_GetP (EDToken_t* tokens, int* position);

EDTree_t* Tree_GetN (EDToken_t* tokens, int* position);

EDTree_t* Tree_GetV (EDToken_t* tokens, int* position);

EDTree_t* Tree_GetF (EDToken_t* tokens, int* position);

/*int main()
{
FILE* from = fopen ("FUNCTION.TXT", "r");
char* variable = NULL;
printf("CALL TREE_GETFUNC\n");
EDTree_t* root = Tree_getFunc(from, &variable);
printf("OUT OF TREE_GETFUNC\n");
fclose(from);
printf("After fclose(from)\n");
FILE* out = fopen ("EXPRESSION.TXT", "w");
assert(out != NULL);
fprintf(out, "%s ", variable);
Tree_print_function(root, out);
Tree_destruct(root);
fclose(out);
free(variable);
return 0;
}*/

//{=================================================================================
//! Tree_getFunc - program which transmits mathematical function in tree
//!
//! @param       from       - Pointer to a file with function
//! @param[out]  variable   - String with variable of differentiation
//!
//! @return      pointer to the root
//}=================================================================================

EDTree_t* Tree_getFunc (FILE* from, char** variable)
{
assert(from != NULL && variable != NULL && *variable == NULL);

EDToken_t* tokens = NULL;
//printf("CALL LEXIC_ANALYSIS\n");
Lexic_analysis(from, &tokens);
assert(tokens[0].type == VARIABLE);
//printf("OUT OF LEXIC_ANALYSIS\n");
//2 - it is the length of variable (ax, bx, cx, dx - are allowed variables)
//printf("CALLOC\n");
(*variable) = (char*) calloc (3, sizeof(char));
//printf("AFTER CALLOC\n");
assert(*variable != NULL);
(*variable)[0] = tokens[0].oper[0];
(*variable)[1] = tokens[0].oper[1];
printf("%s\n", *variable);
int position = 1;
EDTree_t* root = Tree_GetE (tokens, &position);
//printf("AFTER TREE_GetE\n");
if (tokens[position].type != END)
  {
  printf("TREE_FUNC: YOUR LEXIC ANALYSIS IS WRONG\n");
  assert(1 == 0);
  }
printf("%s\n", *variable);
free(tokens);
return root;
}

//{=================================================================================
//! Tree_GetE - program which get the answer of summs
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//!
//! @return      pointer to a root
//}=================================================================================

EDTree_t* Tree_GetE (EDToken_t* tokens, int* position)
{
printf("IN GetE\n");
assert(tokens != NULL && position != NULL);

EDTree_t* left = Tree_GetT(tokens, position), *root = left;
printf("After GetT in GetE\n");
while ((tokens[(*position)].oper != NULL) &&
       (tokens[(*position)].oper[0] == '+' || tokens[(*position)].oper[0] == '-'))
  {
  printf("GetE - SEARCHING OPERATORS\n");
  (*position)++;
  if (tokens[(*position)-1].oper[0] == '+')
    {
    root = Tree_construct(0, "+", OPERATOR, root, Tree_GetT(tokens, position));
    }
  else
    {
    root = Tree_construct(0, "-", OPERATOR, root, Tree_GetT(tokens, position));
    }
  }

assert(tokens != NULL && position != NULL);
return root;
}

//{=================================================================================
//! Tree_GetT - program which get multiplication
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//!
//! @return      pointer to a root
//}=================================================================================

EDTree_t* Tree_GetT (EDToken_t* tokens, int* position)
{
printf("In GetT\n");
assert(tokens != NULL && position != NULL);

EDTree_t* right = Tree_GetP(tokens, position), *root = right;
printf("After GetP in GetT\n");
printf("In GetT %s\n", tokens[(*position)].oper);
while ((tokens[(*position)].oper != NULL) &&
       (tokens[(*position)].oper[0] == '*' || tokens[(*position)].oper[0] == '/'))
  {
  printf("GetT - SEARCHING OPERATORS\n");
  (*position)++;
  if (tokens[(*position)-1].oper[0] == '*')
    {
    printf("Multiple\n");
    root = Tree_construct(0, "*", OPERATOR, root, Tree_GetP(tokens, position));
    }
  else
    {
    root = Tree_construct(0, "/", OPERATOR, root, Tree_GetP(tokens, position));
    }
  }
assert(tokens != NULL && position != NULL);
return root;
}

//{=================================================================================
//! Tree_GetP - program which get the value in brackets ( )
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//!
//! @return      pointer to a root
//}=================================================================================

EDTree_t* Tree_GetP (EDToken_t* tokens, int* position)
{
printf("In_GetP\n");
assert(tokens != NULL && position != NULL);

EDTree_t* root = NULL;

switch (tokens[(*position)].type)
  {
  case OPERATOR: if (tokens[(*position)].oper[0] == '(')
                   {
                   (*position)++;
                   root = Tree_GetE(tokens, position);
                   printf("In GetP after GetE\n");
                   if (tokens[(*position)].oper[0] == ')')
                     {
                     printf("In GetP %s\n", tokens[(*position)].oper);
                     (*position)++;
                     //printf("Inc\n");
                     }
                   else
                     {
                     printf("SYNTAX ERROR: WRONG RIGHT BRACKET\n");
                     assert(1 == 0);
                     }
                   }
                 else
                   {
                   if ((tokens[(*position)].oper[0] == '-' || tokens[(*position)].oper[0] == '+')
                                                           && tokens[(*position)+1].type == NUMBER)
                     {
                     root = Tree_GetN(tokens, position);
                     }
                   else
                     {
                     printf("SYNTAX ERROR: WRONG SIGN\n");
                     assert(1 == 0);
                     }
                   }
                 break;
  case NUMBER: root = Tree_GetN(tokens, position); break;
  case VARIABLE: root = Tree_GetV(tokens, position); break;
  case FUNCTION: root = Tree_GetF(tokens, position); break;
  case END: break;
  default: assert(tokens[(*position)].type == END); break;
  }

assert(tokens != NULL && position != NULL);
return root;
}

//{=================================================================================
//! Tree_GetN - program which get the number from token and puts it in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//!
//! @return      pointer to a root
//}=================================================================================

EDTree_t* Tree_GetN (EDToken_t* tokens, int* position)
{
printf("In_GetN\n");
printf("%lg\n", tokens[(*position)].number);
assert(tokens != NULL && position != NULL);
//printf("%lg\n", tokens[(*position)].number);
EDTree_t* root = NULL;
//printf("After root\n");
if (tokens[(*position)].type == OPERATOR)
  {
  //printf("OPERATOR\n");
  if (tokens[(*position)].oper[0] == '-' || tokens[(*position)].oper[0] == '+')
    {
    //printf("OPERATOR\n");
    if (tokens[(*position)+1].type == NUMBER)
      {
      (*position)++;
      root = Tree_construct(-tokens[(*position)].number, 0, NUMBER, NULL, NULL);
      (*position)++;
      }
    else
      {
      printf("SYNTAX ERROR: WRONG UNARY NUMBER\n");
      assert(1 == 0);
      }
    }
  else
    {
    printf("SYNTAX ERROR: WRONG UNARY SIGN\n");
    assert(1 == 0);
    }
  }
else
  {
  //printf("NUMBER\n");
  assert(tokens[(*position)].type == NUMBER);
  //printf("%lg\n");
  root = Tree_construct(tokens[(*position)].number, NULL, NUMBER, NULL, NULL);
  (*position)++;
  }

assert(tokens != NULL && position != NULL);
return root;
}

//{=================================================================================
//! Tree_GetV - program which get the variable from token and puts it in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//!
//! @return      pointer to a root
//}=================================================================================

EDTree_t* Tree_GetV (EDToken_t* tokens, int* position)
{
printf("In_GetV\n");
assert(tokens != NULL && position != NULL);
assert(tokens[(*position)].type == VARIABLE);

EDTree_t* root = Tree_construct(0, tokens[(*position)].oper, VARIABLE, NULL, NULL);
(*position)++;

assert(tokens != NULL && position != NULL);
return root;
}

//{=================================================================================
//! Tree_GetF - program which get the function from token and puts it in tree
//!
//! @param       tokens   - Array of tokens
//! @param[out]  position - Pointer to the number of cell (position)
//!
//! @return      pointer to a root
//}=================================================================================

EDTree_t* Tree_GetF (EDToken_t* tokens, int* position)
{
printf("In_GetF\n");
assert(tokens != NULL && position != NULL);
assert(tokens[(*position)].type == FUNCTION);
printf("%s\n", tokens[(*position)].oper);

if (tokens[(*position)+1].oper[0] != '(')
  {
  printf("SYNTAX ERROR: WRONG USING OF FUNCTION. NEED THE BRACKETS FOR ARGUMENT.\n");
  assert(1 == 0);
  }

(*position) = (*position) + 2;

EDTree_t* root = NULL;
if (strcmp(tokens[(*position)-2].oper, "SIN") == 0)
  root = Tree_construct(0, "SIN", FUNCTION, NULL, Tree_GetE(tokens, position));

if (strcmp(tokens[(*position)-2].oper, "COS") == 0)
  root = Tree_construct(0, "COS", FUNCTION, NULL, Tree_GetE(tokens, position));

if (strcmp(tokens[(*position)-2].oper, "TAN") == 0)
  root = Tree_construct(0, "TAN", FUNCTION, NULL, Tree_GetE(tokens, position));

if (strcmp(tokens[(*position)-2].oper, "SQRT") == 0)
  root = Tree_construct(0, "SQRT", FUNCTION, NULL, Tree_GetE(tokens, position));

if (strcmp(tokens[(*position)-2].oper, "LOG") == 0)
  root = Tree_construct(0, "LOG", FUNCTION, NULL, Tree_GetE(tokens, position));

if (tokens[(*position)].oper[0] != ')')
  {
  printf("SYNTAX ERROR: WRONG USING OF FUNCTION. NEED THE BRACKETS FOR ARGUMENT.\n");
  assert(1 == 0);
  }

(*position)++;
return root;
}
#endif

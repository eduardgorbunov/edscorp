#ifndef TREE_CALC_CPP
#define TREE_CALC_CPP
//{=================================================================================
//! @file    TreeCalc.cpp
//! @date    2014-11-17 13:40
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Program, which transmits mathematical expression in tree by using
//! the recursive descent
//}=================================================================================

#define EXPRESSION_LENGTH 100
#include "TREE.h"

EDTree_t* Tree_GetN (char** s, int* p);

EDTree_t* Tree_GetG0 (char** s);

EDTree_t* Tree_GetE (char** s, int* p);

EDTree_t* Tree_GetT (char** s, int* p);

EDTree_t* Tree_GetP (char** s, int* p);

int main()
{
printf("Write a mathematical expression: ");

char* buf = (char*) calloc(EXPRESSION_LENGTH, sizeof(char));
scanf("%s", buf);
EDTree_t* root = Tree_GetG0(&buf);
FILE* dump = fopen ("DUMP.TXT", "w");
Tree_print_cpu(root, dump);
fprintf(dump,"OUT\n");
fprintf(dump,"END");
Tree_destruct(root);
fclose(dump);
free(buf);
return 0;
}

//{=================================================================================
//! Tree_GetN - program which get the number from string and puts it in tree
//!
//! @param      s   - Pointer to a string
//! @param[out] p   - Pointer to the number of cell (position)
//!
//! @return      pointer to the node
//}=================================================================================

EDTree_t* Tree_GetN (char** s, int* p)
{
assert(s != NULL && (*s) != NULL && p != NULL);

int val = 0;
while ('0' <= (*s)[(*p)] && (*s)[(*p)] <= '9')
  {
  val = val*10 + (*s)[(*p)] - '0';
  (*p)++;
  }

return Tree_construct(val, "PUSH", NUMBER, NULL, NULL);
}

//{=================================================================================
//! Tree_GetG0 - program which get the answer of expression
//!
//! @param      s   - Pointer to a root
//}=================================================================================

EDTree_t* Tree_GetG0 (char** s)
{
assert(s != NULL && (*s) != NULL);

int p = 0;
EDTree_t* root = Tree_GetE(s,&p);

if ((*s)[p] != '\0' || p <= 0)
  {
  Tree_destruct(root);
  root = Tree_construct(0,"BAD_INPUT", OPERATOR, NULL, NULL);
  }

assert(s != NULL && (*s) != NULL);

return root;
}

//{=================================================================================
//! Tree_GetE - program which get the answer of summs
//!
//! @param      s   - Pointer to a string
//! @param[out] p   - Pointer to the number of cell (position)
//!
//! @return      pointer to a root
//}=================================================================================

EDTree_t* Tree_GetE (char** s, int* p)
{
assert(s != NULL && (*s) != NULL && p != NULL);

EDTree_t* left = Tree_GetT(s, p), *root = left;

while ((*s)[(*p)] == '+' || (*s)[(*p)] == '-')
  {
  (*p)++;
  if ((*s)[(*p)-1] == '+')
    {
    root = Tree_construct(0, "ADD", OPERATOR, root, Tree_GetT(s, p));
    }
  else
    {
    root = Tree_construct(0, "ADD", OPERATOR, root,
                          Tree_construct(0, "MUL", OPERATOR,
                          Tree_construct(-1, "PUSH", NUMBER, NULL, NULL),
                          Tree_GetT(s, p)));
    }
  }

assert(s != NULL && (*s) != NULL && p != NULL);
return root;
}

//{=================================================================================
//! Tree_GetT - program which get multiplication
//!
//! @param      s   - Pointer to a string
//! @param[out] p   - Pointer to the number of cell (position)
//!
//! @return      pointer to a root
//}=================================================================================

EDTree_t* Tree_GetT (char** s, int* p)
{
assert(s != NULL && (*s) != NULL && p != NULL);

EDTree_t* right = Tree_GetP(s, p), *root = right;

while ((*s)[(*p)] == '*' || (*s)[(*p)] == '/')
  {
  (*p)++;
  if ((*s)[(*p)-1] == '*')
    {
    root = Tree_construct(0, "MUL", OPERATOR, Tree_GetP(s, p), root);
    }
  else
    {
    root = Tree_construct(0, "DIV", OPERATOR, Tree_GetP(s, p), root);
    }
  }

assert(s != NULL && (*s) != NULL && p != NULL);
return root;
}

//{=================================================================================
//! Tree_GetP - program which get the value in brackets ( )
//!
//! @param      s   - Pointer to a string
//! @param[out] p   - Pointer to the number of cell (position)
//!
//! @return     pointer to a root
//}=================================================================================

EDTree_t* Tree_GetP (char** s, int* p)
{
assert(s != NULL && (*s) != NULL && p != NULL);

EDTree_t* root = NULL;
if ((*s)[(*p)] == '(')
  {
  (*p)++;
  root = Tree_GetE(s, p);
  if ((*s)[(*p)] == ')')
    {
    (*p)++;
    }
  }
else
  {
  root = Tree_GetN(s, p);
  }

assert(s != NULL && (*s) != NULL && p != NULL);
return root;
}
#endif

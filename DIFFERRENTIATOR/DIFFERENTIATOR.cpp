#ifndef DIFFERENTIATOR_CPP
#define DIFFERENTIATOR_CPP
//{=================================================================================
//! @file    DIFFERENTIATOR.cpp
//! @date    2014-12-09 20:03
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Program, which finds derivatives of mathematical functions
//}=================================================================================

#include <math.h>
#include <time.h>
#include "LEXIC_ANALYSIS.cpp"
#include "TREE_FUNC.cpp"
#include "TREE.H"

EDTree_t* Tree_derivative (EDTree_t* root, char* variable);

EDTree_t* operatorBEK(EDTree_t* root, char* variable);

void Tree_optimization (EDTree_t** root);

EDTree_t* Tree_optimize (EDTree_t* root, int* isOptim);

void Tree_print_tex_main (const char start_name[], const char out_name[], EDTree_t* func,
                          char* variable);

void Tree_print_tex (FILE* out, EDTree_t* func);

void Tree_print_tex_final (FILE* out, EDTree_t* func);

/*int main (int argc, const char* argv[])
{
FILE* from = fopen ((argv[1]? argv[1] : "FUNCTION.TXT"), "r");
char* variable = NULL;
EDTree_t* root = Tree_getFunc(from, &variable);
fclose(from);
Tree_optimization (&root);

EDTree_t* deriv = Tree_derivative(root, variable);
//printf("AFTER DERIVATIVE\n");
FILE* out = fopen ("EXPRESSION.TXT", "w");
assert(out != NULL);

Tree_print_function(deriv, out);
fclose(out);
//printf("AFTER PRINT DERIV\n");
FILE* func = fopen ("OURFUNC.TXT", "w");
assert(func != NULL);
Tree_print_function (root, func);
fclose(func);
//printf("AFTER PRINT ROOT\n");
Tree_destruct(root);
//printf("AFTER DESTRUCT ROOT\n");
Tree_destruct(deriv);
//printf("AFTER DESTRUCT DERIV\n");
free(variable);

if (argv[1])
  {
  printf("\n");
  printf("Write something\n");
  double forMain = 0;
  scanf("%lg", &forMain);
  }
return 0;
} */

int main (int argc, const char* argv[])
{
FILE* from = fopen ((argv[1]? argv[1] : "FUNCTION.TXT"), "r");
char* variable = NULL;
EDTree_t* root = Tree_getFunc(from, &variable);
fclose(from);

Tree_print_tex_main ("START_DOCUMENT.TXT", "OUT_DOCUMENT.TXT", root, variable);

system("latex -output-format=pdf OUT_DOCUMENT.TXT");

Tree_destruct(root);
free(variable);

if (argv[1])
  {
  printf("\n");
  printf("Write something\n");
  double forMain = 0;
  scanf("%lg", &forMain);
  }
return 0;
}

//{=================================================================================
//! Tree_derivative - program which finds derivatives of mathematical functions
//!
//! @param   root      -   pointer to the root with mathematical function
//! @param   variable  -   variable of differentiation
//!
//! @return     Pointer to the optimized root of tree with derivative
//}=================================================================================

EDTree_t* Tree_derivative (EDTree_t* root, char* variable)
{
assert(root != NULL && variable != NULL);
FILE* dump = fopen("DUMP.TXT", "w");

EDTree_t* deriv = operatorBEK(root, variable);
Tree_print_function(deriv, dump);
fclose(dump);
Tree_optimization (&deriv);

return deriv;
}

//{=================================================================================
//! operatorBEK - program which finds derivatives of mathematical functions
//!               (recursive descent)
//!
//! @param   root      -   pointer to the root with mathematical function
//! @param   variable  -   variable of differentiation
//!
//! @return     Pointer to the root of tree with derivative
//}=================================================================================

EDTree_t* operatorBEK(EDTree_t* root, char* variable)
{
assert(root != NULL && variable != NULL);
assert(Tree_ok(root) == OK);

EDTree_t* deriv = NULL;
switch (root->type)
  {
  case NUMBER: deriv = Tree_construct(0, 0, NUMBER, NULL, NULL);
               break;
  case VARIABLE: if (strcmp(root->oper, variable) == 0)
                   {
                   deriv = Tree_construct(1, 0, NUMBER, NULL, NULL);
                   }
                 else
                   {
                   deriv = Tree_construct(0, 0, NUMBER, NULL, NULL);
                   }
                 break;
  case OPERATOR: if (root->oper[0] == '+' || root->oper[0] == '-')
                   {
                   deriv = Tree_construct(0, root->oper, OPERATOR,
                                          operatorBEK(root->left, variable),
                                          operatorBEK(root->right, variable));
                   break;
                   }
                 if (root->oper[0] == '*')
                   {
                   deriv = Tree_construct(0, "+", OPERATOR,
                           Tree_construct(0, root->oper, OPERATOR,
                                          operatorBEK(root->left, variable),
                                          Tree_copy(root->right)),
                           Tree_construct(0, root->oper, OPERATOR,
                                          Tree_copy(root->left),
                                          operatorBEK(root->right, variable)));
                   }
                 if (root->oper[0] == '/')
                   {
                   deriv = Tree_construct(0, root->oper, OPERATOR,
                           Tree_construct(0, "-", OPERATOR,
                           Tree_construct(0, "*", OPERATOR,
                                          operatorBEK(root->left, variable),
                                          Tree_copy(root->right)),
                           Tree_construct(0, "*", OPERATOR,
                                          Tree_copy(root->left),
                                          operatorBEK(root->right, variable))),
                           Tree_construct(0, "*", OPERATOR,
                                          Tree_copy(root->right), Tree_copy(root->right)));
                   }
                 break;
  case FUNCTION: if (strcmp(root->oper, "SIN") == 0)
                   {
                   deriv = Tree_construct(0, "*", OPERATOR,
                           Tree_construct(0, "COS", FUNCTION, NULL, Tree_copy(root->right)),
                           operatorBEK(root->right, variable));
                   }
                 if (strcmp(root->oper, "COS") == 0)
                   {
                   deriv = Tree_construct(0, "*", OPERATOR,
                           Tree_construct(0, "*", OPERATOR,
                           Tree_construct(-1, 0, NUMBER, NULL, NULL),
                           Tree_construct(0, "SIN", FUNCTION, NULL, Tree_copy(root->right))),
                           operatorBEK(root->right, variable));
                   }
                 if (strcmp(root->oper, "TAN") == 0)
                   {
                   deriv = Tree_construct(0, "*", OPERATOR,
                           Tree_construct(0, "/", OPERATOR,
                           Tree_construct(1, 0, NUMBER, NULL, NULL),
                           Tree_construct(0, "*", OPERATOR,
                           Tree_construct(0, "COS", FUNCTION, NULL, Tree_copy(root->right)),
                           Tree_construct(0, "COS", FUNCTION, NULL, Tree_copy(root->right)))),
                           operatorBEK(root->right, variable));
                   }
                 if (strcmp(root->oper, "SQRT") == 0)
                   {
                   deriv = Tree_construct(0, "*", OPERATOR,
                           Tree_construct(0, "/", OPERATOR,
                           Tree_construct(1, 0, NUMBER, NULL, NULL),
                           Tree_construct(0, "*", OPERATOR,
                           Tree_construct(2, 0, NUMBER, NULL, NULL),
                           Tree_construct(0, "SQRT", FUNCTION, NULL, Tree_copy(root->right)))),
                           operatorBEK(root->right, variable));
                   }
                 if (strcmp(root->oper, "LOG") == 0)
                   {
                   deriv = Tree_construct(0, "*", OPERATOR,
                           Tree_construct(0, "/", OPERATOR,
                           Tree_construct(1, 0, NUMBER, NULL, NULL), Tree_copy(root->right)),
                           operatorBEK(root->right, variable));
                   }
  case END: break;
  default: printf("CHECK ITS.h\n"); break;
  }

assert(Tree_ok(deriv) == OK);
return deriv;
}

//{=================================================================================
//! Tree_optimization - program which optimize tree with mathematical expressions
//!
//! @param[out] root  -   pointer to pointer to the root with mathematical function
//}=================================================================================

void Tree_optimization (EDTree_t** root)
{
assert(root != NULL && (*root) != NULL);
int isOptim = 0;
EDTree_t* optim = NULL;
while (isOptim != 1)
  {
  optim = Tree_optimize (*root, &isOptim);
  Tree_destruct(*root);
  (*root) = Tree_copy(optim);
  Tree_destruct(optim);
  }
}

//{=================================================================================
//! Tree_optimize - program which optimize tree with mathematical expressions
//!
//! @param        root      -   pointer to the root with mathematical function
//! @param[out]   isOptim   -   it is a flag of tree change
//!
//! @return     Pointer to the root of optimized tree
//}=================================================================================

EDTree_t* Tree_optimize (EDTree_t* root, int* isOptim)
{
//printf("IN Tree_optimize\n");
(*isOptim) = 1;
EDTree_t* optim = NULL, *left = NULL, *right = NULL;
if (root->left != NULL) left = Tree_optimize (root->left, isOptim);
if ((*isOptim) == 0)
  {
  if (root->right != NULL) right = Tree_optimize (root->right, isOptim);
  (*isOptim) = 0;
  }
else
  {
  if (root->right != NULL) right = Tree_optimize (root->right, isOptim);
  }

switch (root->type)
  {
  case NUMBER: //printf("Optimize number %lg\n", root->number);
               optim = Tree_copy (root);
               break;
  case OPERATOR: switch (root->oper[0])
                   {
                   case '+': //printf("Optimize +\n");
                             if (left->type == NUMBER && right->type == NUMBER)
                               {
                               optim = Tree_construct(left->number + right->number,
                                                      0, NUMBER, NULL, NULL);
                               Tree_destruct(left);
                               Tree_destruct(right);
                               (*isOptim) = 0;
                               }
                             else
                               {
                               if (left->type == NUMBER && left->number == 0)
                                 {
                                 optim = right;
                                 Tree_destruct(left);
                                 (*isOptim) = 0;
                                 }
                               else
                                 {
                                 if (right->type == NUMBER && right->number == 0)
                                   {
                                   optim = left;
                                   Tree_destruct(right);
                                   (*isOptim) = 0;
                                   }
                                 else
                                   {
                                   optim = Tree_construct(0, root->oper, OPERATOR, left, right);
                                   }
                                 }
                               }
                             break;
                   case '-': //printf("Optimize -\n");
                             assert(left != NULL);
                             assert(right != NULL);
                             if (left->type == NUMBER && right->type == NUMBER)
                               {
                               //printf("Calc -\n");
                               optim = Tree_construct(left->number - right->number,
                                                      0, NUMBER, NULL, NULL);
                               Tree_destruct(left);
                               Tree_destruct(right);
                               (*isOptim) = 0;
                               }
                             else
                               {
                               if (left->type == NUMBER && left->number == 0)
                                 {
                                 //printf("In - left->number == 0\n");
                                 optim = Tree_construct(0, "*", OPERATOR,
                                         Tree_construct(-1, 0, NUMBER, NULL, NULL), right);
                                 Tree_destruct(left);
                                 (*isOptim) = 0;
                                 }
                               else
                                 {
                                 if (right->type == NUMBER && right->number == 0)
                                   {
                                   //printf("In - right->number == 0\n");
                                   optim = left;
                                   Tree_destruct(right);
                                   (*isOptim) = 0;
                                   }
                                 else
                                   {
                                   //printf("Not optimized -\n");
                                   optim = Tree_construct(0, root->oper, OPERATOR, left, right);
                                   }
                                 }
                               }
                             break;
                   case '*': //printf("Optimize *\n");
                             if (left->type == NUMBER && right->type == NUMBER)
                               {
                               //printf("%lg * %lg\n", left->number, right->number);
                               optim = Tree_construct(left->number * right->number,
                                                      0, NUMBER, NULL, NULL);
                               Tree_destruct(left);
                               Tree_destruct(right);
                               (*isOptim) = 0;
                               }
                             else
                               {
                               if (left->type == NUMBER)
                                 {
                                 if (left->number == 0)
                                   {
                                   //printf("left->number == 0\n");
                                   optim = Tree_construct(0, 0, NUMBER, NULL, NULL);
                                   Tree_destruct(left);
                                   Tree_destruct(right);
                                   (*isOptim) = 0;
                                   }
                                 else
                                   {
                                   if (left->number == 1)
                                     {
                                     //printf("left->number == 1\n");
                                     optim = right;
                                     Tree_destruct(left);
                                     (*isOptim) = 0;
                                     }
                                   else
                                     {
                                     //printf("Not optimizing *\n");
                                     optim = Tree_construct(0, root->oper, OPERATOR,
                                                            left, right);
                                     }

                                   }
                                 }
                               else
                                 {
                                 if (right->type == NUMBER)
                                   {
                                   if (right->number == 0)
                                     {
                                     //printf("right->number == 0\n");
                                     optim = Tree_construct(0, 0, NUMBER, NULL, NULL);
                                     Tree_destruct(left);
                                     Tree_destruct(right);
                                     (*isOptim) = 0;
                                     }
                                   else
                                     {
                                     if (right->number == 1)
                                       {
                                       //printf("right->number == 1\n");
                                       optim = left;
                                       Tree_destruct(right);
                                       (*isOptim) = 0;
                                       }
                                     else
                                       {
                                       //printf("Not optimizing *\n");
                                       optim = Tree_construct(0, root->oper, OPERATOR,
                                                              left, right);
                                       }
                                     }
                                   }
                                 else
                                   {
                                   //printf("Not optimizing *\n");
                                   optim = Tree_construct(0, root->oper, OPERATOR,
                                                              left, right);
                                   }
                                 }
                               }
                             break;
                   case '/': //printf("Optimize /\n");
                             if (left->type == NUMBER && right->type == NUMBER)
                               {
                               optim = Tree_construct(left->number / right->number,
                                                      0, NUMBER, NULL, NULL);
                               Tree_destruct(left);
                               Tree_destruct(right);
                               (*isOptim) = 0;
                               }
                             else
                               {
                               if (left->type == NUMBER && left->number == 0)
                                 {
                                 optim = Tree_construct(0, 0, NUMBER, NULL, NULL);
                                 Tree_destruct(left);
                                 Tree_destruct(right);
                                 (*isOptim) = 0;
                                 }
                               else
                                 {
                                 if (right->type == NUMBER && right->number == 1)
                                   {
                                   optim = left;
                                   Tree_destruct(right);
                                   (*isOptim) = 0;
                                   }
                                 else
                                   {
                                   optim = Tree_construct(0, root->oper, OPERATOR, left, right);
                                   }
                                 }
                               }
                             break;
                   default: break;
                   }
                 break;
  case VARIABLE: //printf("Optimize VARIABLE\n");
                 optim = Tree_copy (root);
                 break;
  case FUNCTION: //printf("Optimize FUNCTION\n");
                 if (right->type == NUMBER)
                   {
                   if (strcmp(root->oper, "SIN") == 0)
                     {
                     optim = Tree_construct(sin(right->number), 0, NUMBER, NULL, NULL);
                     Tree_destruct(left);
                     Tree_destruct(right);
                     (*isOptim) = 0;
                     }
                   if (strcmp(root->oper, "COS") == 0)
                     {
                     optim = Tree_construct(cos(right->number), 0, NUMBER, NULL, NULL);
                     Tree_destruct(left);
                     Tree_destruct(right);
                     (*isOptim) = 0;
                     }
                   if (strcmp(root->oper, "TAN") == 0)
                     {
                     optim = Tree_construct(tan(right->number), 0, NUMBER, NULL, NULL);
                     Tree_destruct(left);
                     Tree_destruct(right);
                     (*isOptim) = 0;
                     }
                   if (strcmp(root->oper, "SQRT") == 0)
                     {
                     optim = Tree_construct(sqrt(right->number), 0, NUMBER, NULL, NULL);
                     Tree_destruct(left);
                     Tree_destruct(right);
                     (*isOptim) = 0;
                     }
                   if (strcmp(root->oper, "LOG") == 0)
                     {
                     optim = Tree_construct(log(right->number), 0, NUMBER, NULL, NULL);
                     Tree_destruct(left);
                     Tree_destruct(right);
                     (*isOptim) = 0;
                     }
                   }
                else
                  {
                  optim = Tree_construct(0, root->oper, FUNCTION, NULL, right);
                  }
                break;
  case END: printf("WRONG TREE\n"); assert(1==0); break;
  default: printf("CHECK ITS.h\n"); assert(1==0); break;
  }

return optim;
}

//{=================================================================================
//! Tree_print_tex_main - program which print function in tex-form with
//! the begining of document
//!
//! @param      start_name   - Name of file with start of document
//! @param      out_name     - Name of output file
//! @param      func         - Pointer to root of tree with math function
//! @param      variable     - Variable of differentiation
//}=================================================================================

void Tree_print_tex_main (const char start_name[], const char out_name[], EDTree_t* func,
                          char* variable)
{
assert(func != NULL && variable != NULL);

FILE* from = fopen(start_name, "r");
if (from == NULL)
  {
  printf("ERROR: START FILE IS NOT FOUND\n");
  return;
  }

FILE* out = fopen(out_name, "w");
if (from == NULL)
  {
  printf("ERROR: OUTPUT FILE IS NOT FOUND\n");
  fclose(from);
  return;
  }
char* buf = NULL;
int len = 0;
ReadFileInBuffer (&buf, &len, from);
fprintf(out, "%s", buf);
free(buf);

fprintf(out, "Займёмся задачей о нахождении частной производной следующей математической функции.\n\n");
fprintf(out, "$y = ");
Tree_print_tex (out, func);
fprintf(out, "$\n\n");
fprintf(out, "Переменная дифференцирования есть %s.\n\n", variable);

int isOptim = 0, statement = 0;
EDTree_t* optim = NULL;
while (isOptim != 1)
  {
  optim = Tree_optimize (func, &isOptim);
  statement = rand() % 7;
  switch (statement)
    {
    case 0: fprintf(out, "Очевидно, что\n\n"); break;
    case 1: fprintf(out, "Нетрудно заметить, что эта математическая функция эквивалентна следующей\n\n");
            break;
    case 2: fprintf(out, "Из аксиом вещестенных чисел следует, что\n\n"); break;
    case 3: fprintf(out, "Следовательно\n\n"); break;
    case 4: fprintf(out, "Путём элементарных вычислений получаем, что\n\n"); break;
    case 5: fprintf(out, "В детском саду с математическим уклоном функцию записали бы в виде\n\n");
            break;
    case 6: fprintf(out, "Путём тривиальных преобразований получаем\n\n"); break;
    default: break;
    }
  fprintf(out, "$y = ");
  Tree_print_tex(out, optim);
  fprintf(out, "$\n\n");
  Tree_destruct(func);
  func = Tree_copy(optim);
  Tree_destruct(optim);
  }
fprintf(out, "Перейдём к основной части нашей работы.\n\n");
EDTree_t* deriv = operatorBEK(func, variable);
isOptim = 0;
srand(time(NULL));
while(isOptim != 1)
  {
  optim = Tree_optimize (deriv, &isOptim);
  statement = rand() % 7;
  switch (statement)
    {
    case 0: fprintf(out, "Очевидно, что\n\n"); break;
    case 1: fprintf(out, "Нетрудно заметить, что эта математическая функция эквивалентна следующей\n\n");
            break;
    case 2: fprintf(out, "Из аксиом вещестенных чисел следует, что\n\n"); break;
    case 3: fprintf(out, "Следовательно\n\n"); break;
    case 4: fprintf(out, "Путём элементарных вычислений получаем, что\n\n"); break;
    case 5: fprintf(out, "В детском саду с математическим уклоном функцию записали бы в виде\n\n");
            break;
    case 6: fprintf(out, "Путём тривиальных преобразований получаем\n\n"); break;
    default: break;
    }
  fprintf(out, "$y^{'} = ");
  Tree_print_tex(out, optim);
  fprintf(out, "$\n\n");
  Tree_destruct(deriv);
  deriv = Tree_copy(optim);
  Tree_destruct(optim);
  }
fprintf(out, "Итак, производная исходной функции есть\n\n");
Tree_print_tex_final (out, deriv);
Tree_destruct(deriv);
fclose(from);
fclose(out);
}

//{=================================================================================
//! Tree_print_tex - program which print function in tex-form
//!
//! @param      out          - Pointer to output-file
//! @param      func         - Pointer to root of tree with math function
//}=================================================================================

void Tree_print_tex (FILE* out, EDTree_t* func)
{
assert(out != NULL);
assert(func != NULL);

switch (func->type)
  {
  case NUMBER: if (func->number >= 0)
                 {
                 fprintf(out, "%lg", func->number);
                 }
               else
                 {
                 fprintf(out, "(");
                 fprintf(out, "%lg", func->number);
                 fprintf(out, ")");
                 }
               break;
  case OPERATOR: if (func->oper[0] == '*')
                   {
                   if (func->left->type == OPERATOR)
                     {
                     fprintf(out, "(");
                     Tree_print_tex (out, func->left);
                     fprintf(out, ")");
                     }
                   else
                     {
                     Tree_print_tex (out, func->left);
                     }
                   fprintf(out, " \\cdot ");
                   if (func->right->type == OPERATOR)
                     {
                     fprintf(out, "(");
                     Tree_print_tex (out, func->right);
                     fprintf(out, ")");
                     }
                   else
                     {
                     Tree_print_tex (out, func->right);
                     }
                   }
                 else
                   {
                   if (func->oper[0] == '/')
                     {
                     fprintf(out, " \\frac {");
                     Tree_print_tex(out, func->left);
                     fprintf(out, "}{");
                     Tree_print_tex(out, func->right);
                     fprintf(out, "} ");
                     }
                   else
                     {
                     Tree_print_tex (out, func->left);
                     fprintf(out, "%s", func->oper);
                     Tree_print_tex (out, func->right);
                     }
                    }
                 break;
  case FUNCTION: if (strcmp(func->oper, "SIN") == 0)
                   {
                   fprintf(out, "\\sin");
                   }
                 if (strcmp(func->oper, "COS") == 0)
                   {
                   fprintf(out, "\\cos");
                   }
                 if (strcmp(func->oper, "SQRT") == 0)
                   {
                   fprintf(out, "\\sqrt");
                   }
                 if (strcmp(func->oper, "TAN") == 0)
                   {
                   fprintf(out, "\\tan");
                   }
                 if (strcmp(func->oper, "LOG") == 0)
                   {
                   fprintf(out, "\\ln");
                   }
                 fprintf(out, "(");
                 Tree_print_tex (out, func->right);
                 fprintf(out, ")");
                 break;
  case VARIABLE: fprintf(out, "%s", func->oper);
                 break;
  case END: break;
  default: printf("CHECK ITS.h\n"); assert(1==0); break;
  }
}

//{=================================================================================
//! Tree_print_tex_final - program which print function in tex-form (the end of document)
//!
//! @param      out          - Pointer to output-file
//! @param      func         - Pointer to root of tree with math function
//}=================================================================================

void Tree_print_tex_final (FILE* out, EDTree_t* func)
{
assert(out != NULL);
assert(func != NULL);

fprintf(out, "$\\boxed{y^{'} = ");
switch (func->type)
  {
  case NUMBER: if (func->number >= 0)
                 {
                 fprintf(out, "%lg", func->number);
                 }
               else
                 {
                 fprintf(out, "(");
                 fprintf(out, "%lg", func->number);
                 fprintf(out, ")");
                 }
               break;
  case OPERATOR: if (func->oper[0] == '*')
                   {
                   if (func->left->type == OPERATOR)
                     {
                     fprintf(out, "(");
                     Tree_print_tex (out, func->left);
                     fprintf(out, ")");
                     }
                   else
                     {
                     Tree_print_tex (out, func->left);
                     }
                   fprintf(out, " \\cdot ");
                   if (func->right->type == OPERATOR)
                     {
                     fprintf(out, "(");
                     Tree_print_tex (out, func->right);
                     fprintf(out, ")");
                     }
                   else
                     {
                     Tree_print_tex (out, func->right);
                     }
                   }
                 else
                   {
                   if (func->oper[0] == '/')
                     {
                     fprintf(out, " \\frac {");
                     Tree_print_tex(out, func->left);
                     fprintf(out, "}{");
                     Tree_print_tex(out, func->right);
                     fprintf(out, "} ");
                     }
                   else
                     {
                     Tree_print_tex (out, func->left);
                     fprintf(out, "%c", func->oper[0]);
                     Tree_print_tex (out, func->right);
                     }
                    }
                 break;
  case FUNCTION: if (strcmp(func->oper, "SIN") == 0)
                   {
                   fprintf(out, "\\sin");
                   }
                 if (strcmp(func->oper, "COS") == 0)
                   {
                   fprintf(out, "\\cos");
                   }
                 if (strcmp(func->oper, "SQRT") == 0)
                   {
                   fprintf(out, "\\sqrt");
                   }
                 if (strcmp(func->oper, "TAN") == 0)
                   {
                   fprintf(out, "\\tan");
                   }
                 if (strcmp(func->oper, "LOG") == 0)
                   {
                   fprintf(out, "\\ln");
                   }
                 fprintf(out, "(");
                 Tree_print_tex (out, func->right);
                 fprintf(out, ")");
                 break;
  case VARIABLE: fprintf(out, "%s", func->oper);
                 break;
  case END: break;
  default: printf("CHECK ITS.h\n"); assert(1==0); break;
  }
fprintf(out, "}$\n\n");
fprintf(out, "\\end{document}");
}
#endif

#ifndef LEXIC_ANALYSIS_CPP
#define LEXIC_ANALYSIS_CPP
//{=================================================================================
//! @file    LEXIC_ANALYSIS.cpp
//! @date    2014-12-07 12:24
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Program, which analyse mathematical functions
//}=================================================================================

#include <ctype.h>
#include "TREE.h"
#include "READFILE.h"
#define MAX_NUMBER_OF_TOKENS 1000

struct EDToken_t
{
ITS type;
char* oper;
double number;
};

void Lexic_analysis (FILE* from, EDToken_t** tokens);

void Get_token (EDToken_t* token, char* str, int* position);

void SkipSpases (char* str, int* position);

void Get_token_number (EDToken_t* token, char* str, int* position);

void Get_token_variable (EDToken_t* token, char* str, int* position);

void Get_token_function (EDToken_t* token, char* str, int* position);

void Get_token_operator (EDToken_t* token, char* str, int* position);

void Tokens_print (EDToken_t* tokens, FILE* out);

void Tokens_destruct (EDToken_t* tokens);

/*int main ()
{
FILE* from = fopen ("FUNCTION.TXT", "r");
EDToken_t* tokens = NULL;

Lexic_analysis(from, &tokens);

FILE* out = fopen ("EXPRESSION.TXT", "w");

Tokens_print(tokens, out);
Tokens_destruct(tokens);
fclose(from);
fclose(out);

return 0;
}*/

//{=================================================================================
//! Lexic_analysis - program which prepares mathematical expression or function for
//! translation in tree
//!
//! @param      from   - It's a file, which has mathematical expression or function
//! @param[out] tokens - Array of tokens
//}=================================================================================

void Lexic_analysis (FILE* from, EDToken_t** tokens)
{
assert(from != NULL && tokens != NULL && *tokens == NULL);
char* str = NULL;
int len = POISON;
ReadFileInBuffer (&str, &len, from);

int position = 0;
*tokens = (EDToken_t*) calloc (MAX_NUMBER_OF_TOKENS, sizeof(EDToken_t));
int checkPosition = position, i = 0;

while (str[position] != '\0')
  {
  checkPosition = position;
  Get_token(&(*tokens)[i], str, &position);
  printf("%lg %s\n", (*tokens)[i].number, (*tokens)[i].oper);
  assert(checkPosition != position);
  i++;
  }
free(str);

(*tokens)[i].type = END;
}

//{=================================================================================
//! Get_token - program which reads new token from string
//!
//! @param[out] token    - Pointer to the token
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Get_token (EDToken_t* token, char* str, int* position)
{
assert(str != NULL && position != NULL && token != NULL);
SkipSpases (str, position);

if (isdigit(str[*position]) == 1)
  {
  Get_token_number(token, str, position);
  }
else
  {
  if (isalpha(str[*position]) != 0)
    {
    if (str[(*position) + 1] == 'x')
      {
      Get_token_variable(token, str, position);
      }
    else Get_token_function(token, str, position);
    }
  else //if (str[(*position)] != '\n' && str[(*position)] != '\0')
             Get_token_operator(token, str, position);
  }
}

//{=================================================================================
//! SkipSpases - program which skip spases in mathematical expressions and functions
//!
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void SkipSpases (char* str, int* position)
{
assert(str != NULL && position != NULL);

while (str[*position] == ' ')
  {
  (*position)++;
  }
}

//{=================================================================================
//! Get_token_number - program which reads new token-number from string
//!
//! @param[out] token    - Pointer to the token-number
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Get_token_number (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);
assert(isdigit(str[*position]) == 1);

char* number = (char*) calloc (MAX_NUMBER_LENGTH, sizeof(char));
int i = 0;
number[i] = str[*position];
(*position)++;
i++;

while (isdigit(str[*position]) == 1)
  {
  number[i] = str[*position];
  i++;
  (*position)++;
  }

if (str[*position] == '.' && isdigit(str[(*position)+1]) == 1)
  {
  for (int j = 0; j < 2; j++)
    {
    number[i] = str[*position];
    (*position)++;
    i++;
    }
  while (isdigit(str[*position]) == 1)
    {
    number[i] = str[*position];
    i++;
    (*position)++;
    }
  }
number[i] = '\0';

token->number = atof(number);
free(token->oper);
token->type = NUMBER;

free(number);
}

//{=================================================================================
//! Get_token_variable - program which reads new token-variable from string
//!
//! @param[out] token    - Pointer to the token-variable
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Get_token_variable (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);
assert(isalpha(str[*position]) != 0 && str[(*position) + 1] == 'x');

switch (str[*position])
  {
  case 'a': free(token->oper);
            token->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "ax");
            (*position) = (*position) + 2;
            break;
  case 'b': free(token->oper);
            token->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "bx");
            (*position) = (*position) + 2;
            break;
  case 'c': free(token->oper);
            token->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "cx");
            (*position) = (*position) + 2;
            break;
  case 'd': free(token->oper);
            token->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "dx");
            (*position) = (*position) + 2;
            break;
  default:  printf("GET_TOKEN_VARIABLE FALL DOWN\n");
            assert(1 == 0);
            break;
  }
token->type = VARIABLE;
}

//{=================================================================================
//! Get_token_function - program which reads new token-function from string
//!
//! @param[out] token    - Pointer to the token-function
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Get_token_function (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);
assert(isalpha(str[*position]) != 0);
char* func = (char*) calloc (MAX_FUNCTION_LENGTH, sizeof(char));
int i = 0;
func[i] = str[*position];
i++;
(*position)++;

while (isalpha(str[*position]) != 0)
  {
  func[i] = str[*position];
  i++;
  (*position)++;
  }
func[i] = '\0';
int checkFunc = 0;

if (strcmp(func, "SIN") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  strcpy(token->oper, func);
  }

if (strcmp(func, "COS") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  strcpy(token->oper, func);
  }

if (strcmp(func, "TAN") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  strcpy(token->oper, func);
  }

if (strcmp(func, "SQRT") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  strcpy(token->oper, func);
  }

if (strcmp(func, "LOG") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  strcpy(token->oper, func);
  }

if (strcmp(func, "POW") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  strcpy(token->oper, func);
  }

if (checkFunc == 0)
  {
  printf ("GET_TOKEN_FUNCTION FALL DOWN\n");
  assert(1 == 0);
  }

token->type = FUNCTION;
}

//{=================================================================================
//! Get_token_operator - program which reads new token-operator from string
//!
//! @param[out] token    - Pointer to the token-operator
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Get_token_operator (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);

switch (str[*position])
  {
  case '+': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "+\0");
            (*position)++;
            break;
  case '-': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "-\0");
            (*position)++;
            break;
  case '*': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "*\0");
            (*position)++;
            break;
  case '/': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "/\0");
            (*position)++;
            break;
  case '(': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            strcpy(token->oper, "(\0");
            (*position)++;
            break;
  case ')': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            strcpy(token->oper, ")\0");
            (*position)++;
            break;
  default:  printf ("GET_TOKEN_OPERATOR FALL DOWN\n");
            assert(1 == 0);
            break;
  }

token->type = OPERATOR;
}

//{=================================================================================
//! Tokens_print - program which prints tokens
//!
//! @param      tokens   - Array of tokens
//! @param         out   - File for printing out
//}=================================================================================

void Tokens_print (EDToken_t* tokens, FILE* out)
{
assert(tokens != NULL);
assert(out != NULL);
int i = 0;

while (tokens[i].type != END && i < MAX_NUMBER_OF_TOKENS)
  {
  switch (tokens[i].type)
    {
    case NUMBER: fprintf(out, "%lg", tokens[i].number); break;
    case OPERATOR: fprintf(out, "%s", tokens[i].oper); break;
    case FUNCTION: fprintf(out, "%s", tokens[i].oper); break;
    case VARIABLE: fprintf(out, "%s", tokens[i].oper); break;
    case END: fprintf(out, "WRONG CHECKING IN TOKENS_PRINT\n"); break;
    default: fprintf(out, "TOKENS_PRINT FALL DOWN\n"); break;
    }
  i++;
  }
}

//{=================================================================================
//! Tokens_destruct - program which destructs array of tokens
//!
//! @param      tokens   - Array of tokens
//}=================================================================================

void Tokens_destruct (EDToken_t* tokens)
{
int i = 0;

while (i < MAX_NUMBER_OF_TOKENS && tokens[i].type != END)
  {
  free(tokens[i].oper);
  i++;
  }

free(tokens);
}
#endif

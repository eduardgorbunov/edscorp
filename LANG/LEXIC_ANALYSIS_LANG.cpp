#ifndef LEXIC_ANALYSIS_LANG_CPP
#define LEXIC_ANALYSIS_LANG_CPP
//{=================================================================================
//! @file    LEXIC_ANALYSIS_LANG.cpp
//! @date    2014-12-12 02:00
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Program, which analyse programs in StarWars-language
//}=================================================================================

#include <ctype.h>
#include "TREE.h"
#include "READFILE.h"
#include <locale.h>
#define MAX_NUMBER_OF_TOKENS 5000

struct EDToken_t
{
ITS type;
char* oper;
double number;
};

void Lang_lexic (FILE* from, EDToken_t** tokens);

void SkipSpases_Enters (char* str, int* position);

void Lang_get_token (EDToken_t* token, char* str, int* position);

void Lang_get_token_number (EDToken_t* token, char* str, int* position);

void Lang_get_token_variable (EDToken_t* token, char* str, int* position);

void Lang_get_token_function (EDToken_t* token, char* str, int* position);

void Lang_get_token_operator (EDToken_t* token, char* str, int* position);

void Lang_get_token_control (EDToken_t* token, char* str, int* position);

void Tokens_print (EDToken_t* tokens, FILE* out);

void Tokens_destruct (EDToken_t* tokens);

int isbukva (char let);

/*int main()
{
FILE* from = fopen ("CODE.TXT", "r");
EDToken_t* tokens = NULL;

Lang_lexic(from, &tokens);

FILE* out = fopen ("TOKEN.TXT", "w");

Tokens_print(tokens, out);
Tokens_destruct(tokens);
fclose(from);
fclose(out);

return 0;
}*/

//{=================================================================================
//! Lang_lexic - program which prepares program in StarWars-language
//! for transformation in tree
//!
//! @param      from   - It's a file, which has mathematical expression or function
//! @param[out] tokens - Array of tokens
//}=================================================================================

void Lang_lexic (FILE* from, EDToken_t** tokens)
{
assert(from != NULL && tokens != NULL && (*tokens) == NULL);
//setlocale(LC_ALL, "Russian");
char* str = NULL;
int len = POISON;
ReadFileInBuffer (&str, &len, from);
//printf("%s\n", str);
int position = 0;
(*tokens) = (EDToken_t*) calloc (MAX_NUMBER_OF_TOKENS+1, sizeof(EDToken_t));
assert((*tokens) != NULL);
int checkPosition = position, i = 0;

while (str[position] != '\0')
  {
  checkPosition = position;
  Lang_get_token(&(*tokens)[i], str, &position);
  printf("%lg %s\n", (*tokens)[i].number, (*tokens)[i].oper);
  assert(checkPosition != position);
  i++;
  SkipSpases_Enters(str, &position);
  }
free(str);

(*tokens)[i].type = END;
}

//{=================================================================================
//! Lang_get_token - program which reads new token from string
//!
//! @param[out] token    - Pointer to the token
//! @param      str      - String with code in StarWars-language
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Lang_get_token (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);
SkipSpases_Enters (str, position);

if (isdigit(str[(*position)]) == 1)
  {
  Lang_get_token_number (token, str, position);
  }
else
  {
  if (isalpha(str[(*position)]) != 0 && isbukva(str[(*position)]) == 0)
    {
    if (str[(*position) + 1] == 'x')
      {
      Lang_get_token_variable(token, str, position);
      }
    else
      {
      Lang_get_token_function(token, str, position);
      }
    }
  else if (isbukva(str[(*position)]) == 1)
         {
         Lang_get_token_control(token, str, position);
         }
       else
         {
         Lang_get_token_operator(token, str, position);
         }
  }
}

//{=================================================================================
//! SkipSpases_Enters - program which skip spases and enters in code
//!
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void SkipSpases_Enters (char* str, int* position)
{
assert (str != NULL && position != NULL);

while (str[(*position)] == ' ' || str[(*position)] == '\n')
  {
  (*position)++;
  }
}

//{=================================================================================
//! Lang_get_token_number - program which reads new token-number from string
//!
//! @param[out] token    - Pointer to the token-number
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Lang_get_token_number (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);
assert(isdigit(str[*position]) == 1);

char* number = (char*) calloc (MAX_NUMBER_LENGTH, sizeof(char));
assert(number != NULL);
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
//! Lang_get_token_variable - program which reads new token-variable from string
//!
//! @param[out] token    - Pointer to the token-variable
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Lang_get_token_variable (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);
assert(isalpha(str[*position]) != 0 && str[(*position) + 1] == 'x');

switch (str[*position])
  {
  case 'a': free(token->oper);
            token->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "ax\0");
            (*position) = (*position) + 2;
            break;
  case 'b': free(token->oper);
            token->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "bx\0");
            (*position) = (*position) + 2;
            break;
  case 'c': free(token->oper);
            token->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "cx\0");
            (*position) = (*position) + 2;
            break;
  case 'd': free(token->oper);
            token->oper = (char*) calloc (MAX_VARIABLE_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "dx\0");
            (*position) = (*position) + 2;
            break;
  default:  printf("LANG_GET_TOKEN_VARIABLE FALL DOWN\n");
            assert(NULL);
            break;
  }
token->type = VARIABLE;
}

//{=================================================================================
//! Lang_get_token_function - program which reads new token-function from string
//!
//! @param[out] token    - Pointer to the token-function
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Lang_get_token_function (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);
assert(isalpha(str[*position]) != 0);
char* func = (char*) calloc (MAX_FUNCTION_LENGTH, sizeof(char));
assert(func != NULL);
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
  assert(token->oper != NULL);
  strcpy(token->oper, func);
  }

if (strcmp(func, "COS") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, func);
  }

if (strcmp(func, "TAN") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, func);
  }

if (strcmp(func, "SQRT") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, func);
  }

if (strcmp(func, "LOG") == 0)
  {
  checkFunc = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_FUNCTION_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, func);
  }

if (checkFunc == 0)
  {
  printf ("LANG_GET_TOKEN_FUNCTION FALL DOWN\n");
  assert(NULL);
  }

token->type = FUNCTION;
}

//{=================================================================================
//! Lang_get_token_operator - program which reads new token-operator from string
//!
//! @param[out] token    - Pointer to the token-operator
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Lang_get_token_operator (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);

switch (str[*position])
  {
  case '+': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "+\0");
            token->type = OPERATOR;
            (*position)++;
            break;
  case '-': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "-\0");
            token->type = OPERATOR;
            (*position)++;
            break;
  case '*': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "*\0");
            token->type = OPERATOR;
            (*position)++;
            break;
  case '/': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "/\0");
            token->type = OPERATOR;
            (*position)++;
            break;
  case '(': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "(\0");
            token->type = OPERATOR;
            (*position)++;
            break;
  case ')': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, ")\0");
            token->type = OPERATOR;
            (*position)++;
            break;
  case '<': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "<\0");
            token->type = COMPARE;
            (*position)++;
            break;
  case '=': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, "=\0");
            token->type = COMPARE;
            (*position)++;
            break;
  case '>': free(token->oper);
            token->oper = (char*) calloc (MAX_OPERATOR_LENGTH + 1, sizeof(char));
            assert(token->oper != NULL);
            strcpy(token->oper, ">\0");
            token->type = COMPARE;
            (*position)++;
            break;
  default:  if (str[(*position)] != '\0')
              {
              printf ("LANG_GET_TOKEN_OPERATOR FALL DOWN\n");
              printf("%c", str[(*position)]);
              assert(NULL);
              }
            break;
  }
}

//{=================================================================================
//! Lang_get_token_control - program which reads new token-control
//! (token-control - control operator) from string
//!
//! @param[out] token    - Pointer to the token-operator
//! @param      str      - String with mathematical expression or function
//! @param[out] position - Pointer to the number of cell (position)
//}=================================================================================

void Lang_get_token_control (EDToken_t* token, char* str, int* position)
{
assert(token != NULL && str != NULL && position != NULL);
assert(isbukva(str[(*position)]) == 1);
char* control = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
assert(control != NULL);
int i = 0, checkControl = 0;
control[i] = str[(*position)];
(*position)++;
i++;

while (isbukva(str[(*position)]) == 1 ||
       (str[(*position)] == '_' && isbukva(str[(*position)+1]) == 1 ))
  {
  control[i] = str[(*position)];
  i++;
  (*position)++;
  }

if (strcmp(control, "я_стою_выше_тебя\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "ты_недооцениваешь_мою_мощь\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "да_прибудет_с_тобой\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "звездолёт\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "йода\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "голограмма\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "новый_дроид\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "сила\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "светлая_сторона\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (strcmp(control, "тёмная_сторона\0") == 0)
  {
  checkControl = 1;
  free(token->oper);
  token->oper = (char*) calloc (MAX_CONTROL_LENGTH + 1, sizeof(char));
  assert(token->oper != NULL);
  strcpy(token->oper, control);
  }

if (checkControl == 0)
  {
  printf ("LANG_GET_TOKEN_CONTROL FALL DOWN\n");
  printf("%s\n",control);
  assert(NULL);
  }

token->type = CONTROL;
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
    case NUMBER: fprintf(out, "%lg\n", tokens[i].number);       break;
    case OPERATOR: fprintf(out, "%s\n", tokens[i].oper);        break;
    case FUNCTION: fprintf(out, "%s\n", tokens[i].oper);        break;
    case VARIABLE: fprintf(out, "%s\n", tokens[i].oper);        break;
    case CONTROL: fprintf(out, "%s\n", tokens[i].oper);         break;
    case COMPARE: fprintf(out, "%s\n", tokens[i].oper);         break;
    case END: fprintf(out, "WRONG CHECKING IN TOKENS_PRINT\n"); break;
    default: fprintf(out, "TOKENS_PRINT FALL DOWN\n");          break;
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

//{=================================================================================
//! isbukva - program which check, is parameter Russian letter
//!
//! @param      let   -   letter
//!
//! @return     0 - it isn't Russian letter; 1 - it is small Russian letter;
//! @return     2 - it is big Russian letter.
//}=================================================================================

int isbukva (char let)
{
int answer = 0;
if (('а' <= let && let <= 'я') || let == 'ё') answer = 1;
if (('А' <= let && let <= 'Я') || let == 'Ё') answer = 2;

return answer;
}
#endif

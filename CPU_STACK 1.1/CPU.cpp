#include <string.h>
#include <conio.h>
#include <locale.h>
#include <ctype.h>

#include "Stack_Instructions.cpp"
#define assert_cpu(cond) if (!cond) {CPU_info(cp);\
                         printf("ERROR in the line number %d\n", __LINE__); assert(cond);}

#define NUMBER_OF_COMANDS 30

struct EDCPU_t
  {
  double     ax;
  double     bx;
  double     cx;
  double     dx;
  EDStack_t  Stack;
  };

enum Commands_t
  {
  #define DEF_CMD_(name, num, code) name = num,
  #include "Comands.h"
  #undef DEF_CMD_
  CMD_MAX
  };

ERRORS CPU_popr (EDCPU_t* cp, int nreg);

ERRORS CPU_ok   (EDCPU_t* cp);

void CPU_info (EDCPU_t* cp);

ERRORS CPU_add  (EDCPU_t* cp);

ERRORS CPU_mul  (EDCPU_t* cp);

ERRORS CPU_outr (EDCPU_t* cp, int nreg);

ERRORS CPU_in (EDCPU_t* cp, int nreg);

ERRORS CPU_div (EDCPU_t* cp);

ERRORS CPU_pow (EDCPU_t* cp, double exponent);

void ReadFileInBuffer (char** buf, int* len, FILE* f);

int CountOf (char* buf, char a, int length);

void CreateArrayOfPointersToStr (char*** str, int* AmountOfStr, char* buf, int len);

ERRORS CPU_pushr(EDCPU_t* cp, int nreg);

int main (int argc, const char* argv[])
{
EDCPU_t cp = {};
FILE *f = fopen ((argv[1]? argv[1] : "FIBO.TXT"), "r");

Stack_construct(&(cp.Stack));

assert(f != NULL);

char* buf = NULL;
int len = POISON;
ReadFileInBuffer (&buf, &len, f);

char** str = NULL;
int AmountOfStr = POISON;

CreateArrayOfPointersToStr(&str, &AmountOfStr, buf, len);

#include "assembler.h"

#include "disassembler.h"

EDStack_t CallStack = {};
Stack_construct(&(CallStack));

int i = 0;
double value = POISON; //it's a pocket for Comands.h

assert(i == 0);
while (i < NumOfCellIncmds)
  {
  if (0 <= (int) cmds[i] && (int) cmds[i] <= 9)
    {
    i++;
    }
  else
    {
    #define DEF_CMD_(name, num, code) case num: code; break;
    switch ((int) cmds[i])
    {
    #include "Comands.h"
    default: printf("WRONG ASSERT IN ASSEMBLER.H");
             assert(0);
             break;
    }
    #undef DEF_CMD_
    }
  }

Stack_destruct(&(cp.Stack));
Stack_destruct(&(CallStack));
fclose(f);
free(buf);
free(str);
free(cmds);
free(labels);

if (argv[1])
  {
  printf("\n");
  printf("Write something\n");
  double forMain = 0;
  scanf("%lg", &forMain);
  }
return 0;
}

//{==================================================================================
//! CPU_popr - program which take the last element from stack and push it in register
//!
//! @param      stk   - Pointer to a structure
//! @param      nreg  - The number of register
//!
//! @return     Number of an error
//}==================================================================================

ERRORS CPU_popr (EDCPU_t* cp, int nreg)
{
ERRORS nerrorcp = CPU_ok(cp);

assert_cpu(nerrorcp == OK);

assert(0 < nreg && nreg < 5);
switch (nreg)
{
case 1:  Stack_pop(&(cp->Stack), &(cp->ax));
         break;
case 2:  Stack_pop(&(cp->Stack), &(cp->bx));
         break;
case 3:  Stack_pop(&(cp->Stack), &(cp->cx));
         break;
case 4:  Stack_pop(&(cp->Stack), &(cp->dx));
         break;
default: printf("FAILURE IN MATRIX: WRONG ASSERT");
         break;
}

nerrorcp = CPU_ok(cp);
assert_cpu(nerrorcp == OK);

return nerrorcp;
}

//{=================================================================================
//! CPU_ok - program which check cpu for errors
//!
//! @param      stk   - Pointer to a cpu
//!
//! @return     Number of an error
//}=================================================================================

ERRORS CPU_ok (EDCPU_t* cp)
{
ERRORS nerrorcp = OK;

if (cp != NULL)
  {
  nerrorcp = Stack_ok(&(cp->Stack));
  if (nerrorcp == OK)
    {
    if (&(cp->ax) == NULL) nerrorcp = NULL_POINTER_ON_REGISTER_ax;
    if (&(cp->bx) == NULL) nerrorcp = NULL_POINTER_ON_REGISTER_bx;
    if (&(cp->cx) == NULL) nerrorcp = NULL_POINTER_ON_REGISTER_cx;
    if (&(cp->dx) == NULL) nerrorcp = NULL_POINTER_ON_REGISTER_dx;
    }
  }
else nerrorcp = NULL_POINTER_ON_CPU;

return nerrorcp;
}

//{=================================================================================
//! CPU_info - program which print some useful information about cpu
//!
//! @param      stk   - Pointer to a structure
//}=================================================================================

void CPU_info (EDCPU_t* cp)
{
printf("================================================================================\n");
printf("The list of ERRORS:\n");
printf("OK        = 0:                   stack is normal\n");
printf("BAD_PUSH  = 1:                   stack has elements in zero cell\n");
printf("BAD_NTOP  = 2:                   the number of the last element in stack is more\n");
printf("                                 than the maximum allowable number or it's less\n");
printf("                                 than zero\n");
printf("STRERROR  = 3:                   NULL pointer on the body of stack\n");
printf("BAD_INPUT = 4:                   bad parameters were transmitted to function\n");
printf("NULL_POINTER_ON_STACK = 5:       null pointer on structure\n");
printf("NULL_POINTER_ON_REGISTER_ax = 6: null pointer on register ax\n");
printf("NULL_POINTER_ON_REGISTER_bx = 7: null pointer on register bx\n");
printf("NULL_POINTER_ON_REGISTER_cx = 8: null pointer on register cx\n");
printf("NULL_POINTER_ON_REGISTER_dx = 9: null pointer on register dx\n");
printf("NULL_POINTER_ON_CPU = 10:        null pointer on cpu\n");
printf("BAD_ZERO_CELL = 11:              there is not a POISON = %d in zero cell\n", POISON);
printf("--------------------------------------------------------------------------------\n");
ERRORS nerrorcp = CPU_ok(cp);
printf("EDCPU_t (%d) [%p]\n", nerrorcp, cp);
if (nerrorcp != NULL_POINTER_ON_CPU)
  {
  if (nerrorcp != NULL_POINTER_ON_REGISTER_ax) printf("Register ax = %lg\n", cp->ax);
  if (nerrorcp != NULL_POINTER_ON_REGISTER_bx) printf("Register bx = %lg\n", cp->bx);
  if (nerrorcp != NULL_POINTER_ON_REGISTER_cx) printf("Register cx = %lg\n", cp->cx);
  if (nerrorcp != NULL_POINTER_ON_REGISTER_dx) printf("Register dx = %lg\n", cp->dx);

  ERRORS nerror = Stack_ok(&(cp->Stack));
  if (nerror != NULL_POINTER_ON_STACK)
    {
    printf("  {\n");
    printf("  ntop = %d\n", (cp->Stack).ntop);
    printf("  body [%d] [%p]\n", (cp->Stack).maxntop, (cp->Stack).body);
    printf("    {\n");
    printf("    [0] = %lg  *\n", (cp->Stack).body[0]);

    int i = 0;
    for (i = 1; i <= (cp->Stack).ntop; i++)
      printf("    [%d] = %lg\n", i, (cp->Stack).body[i]);

    for (i = (cp->Stack).ntop + 1; i <= (cp->Stack).maxntop; i++)
      printf("    [%d] = %lg  *\n", i, (cp->Stack).body[i]);

    printf("    }\n");
    printf("  }\n");
    }
  }

printf("================================================================================\n");
}

//{=================================================================================
//! CPU_add - program which add two last elements in stack
//!
//! @param      cp - Pointer to a cpu
//!
//! @return     Number of an error
//}=================================================================================

ERRORS CPU_add (EDCPU_t* cp)
{
ERRORS nerrorcp = CPU_ok(cp);

assert_cpu(nerrorcp == OK);

if ((cp->Stack).ntop < 2)
  {
  nerrorcp = BAD_INPUT;
  assert_cpu(nerrorcp == OK);
  }
else
  {
  (cp->Stack).body[(cp->Stack).ntop - 1] = (cp->Stack).body[(cp->Stack).ntop - 1] + (cp->Stack).body[(cp->Stack).ntop];
  (cp->Stack).body[(cp->Stack).ntop] = POISON;
  (cp->Stack).ntop--;

  nerrorcp = CPU_ok(cp);
  assert_cpu(nerrorcp == OK);
  }

return nerrorcp;
}

//{=================================================================================
//! CPU_mul - program which multiple two last elements in stack
//!
//! @param      cp - Pointer to a cpu
//!
//! @return     Number of an error
//}=================================================================================

ERRORS CPU_mul (EDCPU_t* cp)
{
ERRORS nerrorcp = CPU_ok(cp);

assert_cpu(nerrorcp == OK);

if ((cp->Stack).ntop < 2)
  {
  nerrorcp = BAD_INPUT;
  assert_cpu(nerrorcp == OK);
  }
else
  {
  (cp->Stack).body[(cp->Stack).ntop - 1] = (cp->Stack).body[(cp->Stack).ntop - 1] * (cp->Stack).body[(cp->Stack).ntop];
  (cp->Stack).body[(cp->Stack).ntop] = POISON;
  (cp->Stack).ntop--;

  nerrorcp = CPU_ok(cp);
  assert_cpu(nerrorcp == OK);
  }

return nerrorcp;
}

//{=================================================================================
//! CPU_outr - program which print out value of register
//!
//! @param      cp - Pointer to a cpu
//!
//! @return     Number of an error
//}=================================================================================

ERRORS CPU_outr (EDCPU_t* cp, int nreg)
{
ERRORS nerrorcp = CPU_ok(cp);

assert_cpu(nerrorcp == OK);

assert(0 < nreg && nreg < 5);
switch (nreg)
{
case 1:  printf("%lg\n", cp->ax);
         break;
case 2:  printf("%lg\n", cp->bx);
         break;
case 3:  printf("%lg\n", cp->cx);
         break;
case 4:  printf("%lg\n", cp->dx);
         break;
default: printf("FAILURE IN MATRIX: WRONG ASSERT\n");
         assert(0);
         break;
}

nerrorcp = CPU_ok(cp);
assert_cpu(nerrorcp == OK);

return nerrorcp;
}

//{=================================================================================
//! CPU_in - program which scan value from keyboard and put it in register
//!
//! @param      cp - Pointer to a cpu
//! @param      nreg - The number of register
//!
//! @return     Number of an error
//}=================================================================================

ERRORS CPU_in (EDCPU_t* cp, int nreg)
{
ERRORS nerrorcp = CPU_ok(cp);

assert_cpu(nerrorcp == OK);

double value = POISON;

assert(&value != NULL);
int Input = scanf("%lg", &value);

assert(Input == 1);

assert(0 < nreg && nreg < 5);
switch (nreg)
{
case 1:  cp->ax = value;
         break;
case 2:  cp->bx = value;
         break;
case 3:  cp->cx = value;
         break;
case 4:  cp->dx = value;
         break;
default: printf("FAILURE IN MATRIX: WRONG ASSERT");
         break;
}

nerrorcp = CPU_ok(cp);
assert_cpu(nerrorcp == OK);

return nerrorcp;
}

//{=================================================================================
//! CPU_div - program which divide the last element on the last but one element
//!           from stack
//!
//! @param      cp - Pointer to a cpu
//!
//! @return     Number of an error
//}=================================================================================

ERRORS CPU_div (EDCPU_t* cp)
{
ERRORS nerrorcp = CPU_ok(cp);

assert_cpu(nerrorcp == OK);

if ((cp->Stack).ntop < 2 || (cp->Stack).body[(cp->Stack).ntop - 1] == 0)
  {
  nerrorcp = BAD_INPUT;
  assert_cpu(nerrorcp == OK);
  }
else
  {
  (cp->Stack).body[(cp->Stack).ntop - 1] = (cp->Stack).body[(cp->Stack).ntop]/(cp->Stack).body[(cp->Stack).ntop - 1];
  (cp->Stack).body[(cp->Stack).ntop] = POISON;
  (cp->Stack).ntop--;
  }

nerrorcp = CPU_ok(cp);
assert_cpu(nerrorcp == OK);

return nerrorcp;
}

//{=================================================================================
//! CPU_pow - program which involute last element from stack in degree exponent
//!
//! @param      cp - Pointer to a cpu
//! @param      exponent - exponent
//!
//! @return     Number of an error
//}=================================================================================

ERRORS CPU_pow (EDCPU_t* cp, double exponent)
{
ERRORS nerrorcp = CPU_ok(cp);

assert_cpu(nerrorcp == OK);

if (Stack_isEmpty(&(cp->Stack)) == 1)
  {
  nerrorcp = BAD_INPUT;
  assert_cpu(nerrorcp == OK);
  }
else
  {
  (cp->Stack).body[(cp->Stack).ntop] = pow((cp->Stack).body[(cp->Stack).ntop], exponent);
  }

nerrorcp = CPU_ok(cp);
assert_cpu(nerrorcp == OK);

return nerrorcp;
}

//{=================================================================================
//! ReadFileInBuffer - program which read file in buffer
//!
//! @param[out]      buf - Pointer to the buffer
//! @param[out]      len - The length of the buffer
//! @param[out]      f   - Pointer to the file
//}=================================================================================

void ReadFileInBuffer (char** buf, int* len, FILE* f)
{
assert(f != NULL);
assert(buf != NULL);
assert((*buf) == NULL);
assert(len != NULL);

fseek (f, 0, SEEK_END);
*len = ftell (f);
rewind (f);

*buf = (char*) calloc ((*len) + 1, sizeof (char));

assert((*buf) != NULL);

fread (*buf, *len, sizeof(char), f);

(*buf)[*len] = '\n';

(*len)++;
}

//{=================================================================================
//! CountOf - ñounting the number of occurrences of the symbol in the string.
//!
//! @param      buf     A pointer to the beginning of the string
//! @param      a       Symbol
//! @param      length  The length of the string
//!
//! @return     Number of occurrences of the symbol a (the second parameter)
//}=================================================================================

int CountOf (char* buf, char a, int length)
{
assert(buf != NULL);

int i = 0, counter = 0;

for (i = 0; i < length; i++)
  {
  if (buf[i] == a) counter++;
  }

return counter;
}

//{=================================================================================
//! CreateArrayOfPointersToStr - program which create array of pointer to strings
//!
//! @param[out]      str - Pointer to the char**
//! @param[out]      AmountOfStr - The amount of strings
//! @param[out]      buf - Pointer to the buffer
//! @param[out]      len - The length of the buffer
//}=================================================================================

void CreateArrayOfPointersToStr (char*** str, int* AmountOfStr, char* buf, int len)
{
assert(AmountOfStr != NULL);
assert(str != NULL);
assert(*str == NULL);

*AmountOfStr = CountOf(buf, '\n', len);

*str = (char**) calloc(*AmountOfStr, sizeof(char*));
assert((*str) != NULL);

int i = 0, startStr = 0, nStr = 0;

for (i = 0; i < len; i++)
  {
  if (buf[i] == '\n')
    {
    (*str)[nStr] = buf + startStr;
    nStr++;
    startStr = i + 1;
    }
  }
}

//{==================================================================================
//! CPU_pushr - program which put value from register in stack
//!
//! @param      stk   - Pointer to a structure
//! @param      nreg  - The number of register
//!
//! @return     Number of an error
//}==================================================================================

ERRORS CPU_pushr(EDCPU_t* cp, int nreg)
{
ERRORS nerrorcp = CPU_ok(cp);
assert_cpu(nerrorcp == OK);

assert(0 < nreg && nreg < 5);
switch (nreg)
{
case 1:  Stack_push(&(cp->Stack), cp->ax);
         break;
case 2:  Stack_push(&(cp->Stack), cp->bx);
         break;
case 3:  Stack_push(&(cp->Stack), cp->cx);
         break;
case 4:  Stack_push(&(cp->Stack), cp->dx);
         break;
default: printf("WRONG ASSERT\n");
         assert(0);
         break;
}

nerrorcp = CPU_ok(cp);
assert_cpu(nerrorcp == OK);

return nerrorcp;
}

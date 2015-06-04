//{=================================================================================
//! @file    Stack_Instructions.cpp
//! @date    2014-10-15 18:00
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Some programs which helps to use stack.
//}=================================================================================

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

#define POISON  1337
#define assert_stk(cond)  if (!cond) {Stack_info(stk);\
                          printf("ERROR in the line number %d\n", __LINE__); assert(cond);}

struct EDStack_t
  {
  int     ntop;
  double* body;
  int     maxntop;
  };

#include "ERRORS.h"

ERRORS Stack_push (EDStack_t* stk, double value);

ERRORS Stack_pop (EDStack_t* stk, double* value);

ERRORS Stack_construct (EDStack_t* stk);

ERRORS Stack_destruct (EDStack_t* stk);

ERRORS Stack_ok (const EDStack_t* stk);

void   Stack_info (const EDStack_t* stk);

ERRORS Stack_clear(EDStack_t* stk);

ERRORS Stack_erase(EDStack_t* stk, int amount);

ERRORS Stack_swap(EDStack_t* stk);

ERRORS Stack_dup (EDStack_t* stk);

int Stack_isEmpty (EDStack_t* stk);

int Stack_isFull (EDStack_t* stk);

ERRORS Stack_out(EDStack_t* stk);

//{=================================================================================
//! Stack_push - program which add new value in stack.
//!
//! @param      stk   - Pointer to a structure
//! @param      value - Value which should be placed on the "top" of stack
//!
//! @return     Number of an error
//}=================================================================================


ERRORS Stack_push (EDStack_t* stk, double value)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

if (Stack_isFull(stk) == 1)
  {
  stk->body = (double*) realloc(stk->body, stk->maxntop * 2 * sizeof(double));
  stk->maxntop = stk->maxntop * 2;
  }

//Stack_push doesn't push anything in zero's case. It helps to find mistakes.
stk->ntop++;
stk->body[stk->ntop] = value;

nerror = Stack_ok(stk);
assert_stk(nerror == OK);

return nerror;
}

//{=================================================================================
//! Stack_pop - program which take the last element from stack.
//!
//! @param      stk   - Pointer to a structure
//! @param      value - Pointer to a place where program must to put the last element from stack
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_pop (EDStack_t* stk, double* value)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

if (Stack_isEmpty(stk) != 1)
  {
  *value = stk->body[stk->ntop];

  stk->body[stk->ntop] = POISON;
  stk->ntop--;

  nerror = Stack_ok(stk);
  assert_stk(nerror == OK);
  }
else
  {
  nerror = BAD_INPUT;
  assert_stk(nerror == OK);
  }

return nerror;
}

//{=================================================================================
//! Stack_construct - program which build stack for next using
//!
//! @param      stk   - Pointer to a structure
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_construct (EDStack_t* stk)
{
ERRORS nerror = OK;

if (stk != NULL)
  {
  stk->ntop = 0;
  stk->maxntop = 100;
  stk->body = (double*) calloc(101, sizeof(double));

  int i = 0;
  for (i = 0; i <= stk->maxntop; i++)
    stk->body[i] = POISON;

  nerror = Stack_ok(stk);
  assert_stk(nerror == OK);
  }
else
  {
  nerror = NULL_POINTER_ON_STACK;
  assert_stk(nerror == OK);
  }

return nerror;
}

//{=================================================================================
//! Stack_destruct - program which destructs stack and releases memory
//!
//! @param      stk   - Pointer to a structure
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_destruct (EDStack_t* stk)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

int i = 0;
for (i = 0; i <= stk->maxntop; i++) stk->body[i] = POISON;

free(stk->body);
stk->body = NULL;

stk->ntop    = -1;
stk->maxntop = -1;

return nerror;
}

//{=================================================================================
//! Stack_ok - program which check stack for errors
//!
//! @param      stk   - Pointer to a structure
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_ok (const EDStack_t* stk)
{
ERRORS nerror = OK;

if (stk != NULL)
  {
  if (stk->ntop > stk->maxntop || stk->ntop < 0) nerror = BAD_NTOP;

  if (stk->body[0] != POISON) nerror = BAD_ZERO_CELL;

  if (stk->body == NULL) nerror = STRERROR;
  }
else nerror = NULL_POINTER_ON_STACK;

return nerror;
}

//{=================================================================================
//! Stack_info - program which print some useful information about stack
//!
//! @param      stk   - Pointer to a structure
//}=================================================================================

void Stack_info (const EDStack_t* stk)
{
ERRORS nerror = Stack_ok(stk);

printf("=================================================================================\n");
printf("The list of ERRORS:\n");
printf("OK        = 0:               stack is normal\n");
printf("BAD_PUSH  = 1:               stack has elements in zero cell\n");
printf("BAD_NTOP  = 2:               the number of the last element in stack is more than\n");
printf("                             the maximum allowable number or it's less than zero\n");
printf("STRERROR  = 3:               NULL pointer on the body of stack\n");
printf("BAD_INPUT = 4:               bad parameters were transmitted to function\n");
printf("NULL_POINTER_ON_STACK   = 5: null pointer on structure\n");
printf("BAD_ZERO_CELL = 11:          there is not a POISON = %d in zero cell\n", POISON);
printf("---------------------------------------------------------------------------------\n");
printf("EDStack_t (%d) [%p]\n", nerror, stk);
if (nerror != NULL_POINTER_ON_STACK)
  {
  printf("  {\n");
  printf("  ntop = %d\n", stk->ntop);
  printf("  body [%d] [%p]\n", stk->maxntop, stk->body);
  printf("    {\n");
  printf("    [0] = %lg  *\n", stk->body[0]);

  int i = 0;
  for (i = 1; i <= stk->ntop; i++)
    printf("    [%d] = %lg\n", i, stk->body[i]);

  for (i = stk->ntop + 1; i <= stk->maxntop; i++)
    printf("    [%d] = %lg  *\n", i, stk->body[i]);

  printf("    }\n");
  printf("  }\n");
  }
printf("=================================================================================\n");
}

//{=================================================================================
//! Stack_clear - program which fill the stack with POISON
//!
//! @param      stk   - Pointer to a structure
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_clear(EDStack_t* stk)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

int i = 0;
for (i = 1; i <= stk->maxntop; i++)
  stk->body[i] = POISON;

stk->ntop = 0;

nerror = Stack_ok(stk);
assert_stk(nerror == OK);

return nerror;
}

//{=================================================================================
//! Stack_erase - delete this amount of elements from the end of stack
//!
//! @param      stk    - Pointer to a structure
//! @param      amount - The number of elements in stack, which should be deleted
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_erase(EDStack_t* stk, int amount)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

if (amount > stk->ntop || amount <= 0)
  {
  nerror = BAD_INPUT;
  assert_stk(nerror == OK);
  }
else
  {
  int i = 0;
  for (i = 0; i < amount; i++)
    {
    stk->body[stk->ntop] = POISON;
    stk->ntop--;
    }
  }

nerror = Stack_ok(stk);
assert_stk(nerror == OK);

return nerror;
}

//{=================================================================================
//! Stack_swap - program which swap two last elements
//!
//! @param      stk     - Pointer to a structure
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_swap(EDStack_t* stk)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

if (stk->ntop < 2)
  {
  nerror = BAD_INPUT;
  assert_stk(nerror == OK);
  }

else
  {
  double temp              = stk->body[stk->ntop];
  stk->body[stk->ntop]     = stk->body[stk->ntop - 1];
  stk->body[stk->ntop - 1] = temp;
  nerror = Stack_ok(stk);
  assert_stk(nerror == OK);
  }

return nerror;
}

//{=================================================================================
//! Stack_dup - program which dub last element in stack
//!
//! @param      stk     - Pointer to a structure
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_dup (EDStack_t* stk)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

if (stk->ntop != 0)
  {
  nerror = Stack_push(stk, stk->body[stk->ntop]);
  assert_stk(nerror == OK);
  }
else
  {
  nerror = BAD_INPUT;
  assert_stk(nerror == OK);
  }

return nerror;
}

//{=================================================================================
//! Stack_isEmpty - program which answers the question: Is stack empty?
//!
//! @param      stk     - Pointer to a structure
//!
//! @return     1 - if stack is empty, 0 - otherwise
//}=================================================================================

int Stack_isEmpty (EDStack_t* stk)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

if (stk->ntop == 0) return 1;
else return 0;
}

//{=================================================================================
//! Stack_isEmpty - program which answers the question: Is stack full?
//!
//! @param      stk     - Pointer to a structure
//!
//! @return     1 - if stack is full, 0 - otherwise
//}=================================================================================

int Stack_isFull (EDStack_t* stk)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

if (stk->ntop == stk->maxntop) return 1;
else return 0;
}

//{=================================================================================
//! CPU_out - program which print out value of last element in stack
//!
//! @param      cp - Pointer to a cpu
//!
//! @return     Number of an error
//}=================================================================================

ERRORS Stack_out(EDStack_t* stk)
{
ERRORS nerror = Stack_ok(stk);

assert_stk(nerror == OK);

if (Stack_isEmpty(stk) == 0) printf("%lg\n", stk->body[stk->ntop]);
else
  {
  nerror = BAD_INPUT;
  assert_stk(nerror == OK);
  }

nerror = Stack_ok(stk);
assert_stk(stk);

return nerror;
}

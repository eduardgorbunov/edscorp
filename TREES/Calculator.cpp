#ifndef CALCULATOR_CPP
#define CALCULATOR_CPP
//{=================================================================================
//! @file    Calculator.cpp
//! @date    2014-11-17 13:40
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Program, which calculate mathematical expression by using the recursive descent
//}=================================================================================

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>

void GetG0 (char** s);

int GetE (char** s, int* p);

int GetT (char** s, int* p);

int GetP (char** s, int* p);

int GetN (char** s, int* p);

int main()
{
printf("Write a mathematical expression: ");

//we use only short strings
char* buf = (char*) calloc(100, sizeof(char));
scanf("%s", buf);
GetG0(&buf);
free(buf);
return 0;
}

//{=================================================================================
//! GetG0 - program which get the answer of expression
//!
//! @param      s   - Pointer to a string
//}=================================================================================

void GetG0 (char** s)
{
assert(s != NULL && (*s) != NULL);

int p = 0, val = GetE(s,&p);
if ((*s)[p] == '\0' && p > 0)
  {
  printf("Answer: %d", val);
  }
else
  {
  printf("You mathematical expression is incorrect");
  }

assert(s != NULL && (*s) != NULL);
}

//{=================================================================================
//! GetE - program which get the answer of summs
//!
//! @param      s   - Pointer to a string
//! @param[out] p   - Pointer to the number of cell (position)
//!
//! @return      value of the summ
//}=================================================================================

int GetE (char** s, int* p)
{
assert(s != NULL && (*s) != NULL && p != NULL);

int val = GetT(s, p);
while ((*s)[(*p)] == '+' || (*s)[(*p)] == '-')
  {
  (*p)++;
  if ((*s)[(*p)-1] == '+')
    {
    val += GetT(s, p);
    }
  else
    {
    val -= GetT(s, p);
    }
  }
assert(s != NULL && (*s) != NULL && p != NULL);
return val;
}

//{=================================================================================
//! GetT - program which get multiplication
//!
//! @param      s   - Pointer to a string
//! @param[out] p   - Pointer to the number of cell (position)
//!
//! @return      value of the multiplication
//}=================================================================================

int GetT (char** s, int* p)
{
assert(s != NULL && (*s) != NULL && p != NULL);

int val = GetP(s, p);
while ((*s)[(*p)] == '*' || (*s)[(*p)] == '/')
  {
  (*p)++;
  if ((*s)[(*p)-1] == '*')
    {
    val *= GetP(s, p);
    }
  else
    {
    val /= GetP(s, p);
    }
  }

assert(s != NULL && (*s) != NULL && p != NULL);
return val;
}

//{=================================================================================
//! GetP - program which get the value in brackets ( )
//!
//! @param      s   - Pointer to a string
//! @param[out] p   - Pointer to the number of cell (position)
//!
//! @return      value from brackets
//}=================================================================================

int GetP (char** s, int* p)
{
assert(s != NULL && (*s) != NULL && p != NULL);

if ((*s)[(*p)] == '(')
  {
  (*p)++;
  int val = GetE(s, p);
  if ((*s)[(*p)] == ')')
    {
    (*p)++;
    }

  assert(s != NULL && (*s) != NULL && p != NULL);
  return val;
  }
else
  {
  assert(s != NULL && (*s) != NULL && p != NULL);
  return GetN(s, p);
  }
}

//{=================================================================================
//! GetN - program which get the number from string
//!
//! @param      s   - Pointer to a string
//! @param[out] p   - Pointer to the number of cell (position)
//!
//! @return      number
//}=================================================================================

int GetN (char** s, int* p)
{
assert(s != NULL && (*s) != NULL && p != NULL);

int val = 0;
while ('0' <= (*s)[(*p)] && (*s)[(*p)] <= '9')
  {
  val = val*10 + (*s)[(*p)] - '0';
  (*p)++;
  }

return val;
}
#endif

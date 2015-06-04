#ifndef HASH_CPP
#define HASH_CPP
#define assert_hash(cond) if (!cond)\
                            {\
                            Hash_dump(hs);\
                            printf("---------------------------------------------\n");\
                            printf("ERROR in the line number %d\n", __LINE__);\
                            printf("---------------------------------------------\n");\
                            assert(cond);\
                            }
#define NUMBER_OF_CELLS 1000
#define WORDS_LENGTH 100
#include "LIST.CPP"

int Hash_func_const(char* s);

int Hash_func_ASCII(char* s);

int Hash_func_length(char* s);

int Hash_func_sum(char* s);

int Hash_func_best (char* s);

struct EDHash_t
  {
  EDDescription_t* descr;
  int (*func)(char* s);
  int number;
  };

ERRORS_LIST Hash_ok (EDHash_t* hs);

ERRORS_LIST Hash_construct (EDHash_t* hs, int number,int (*func)(char* s));

ERRORS_LIST Hash_destruct (EDHash_t* hs);

ERRORS_LIST Hash_dump (EDHash_t* hs);

ERRORS_LIST Hash_insert (EDHash_t* hs, char* val);

int main()
{
EDHash_t hs = {};

Hash_construct(&hs, NUMBER_OF_CELLS, Hash_func_best);

FILE* from = fopen("Text.txt", "r");

assert(from != NULL);
fseek (from, 0, SEEK_END);
int len = ftell(from), Input = 1;
rewind (from);

char* buf = (char*) calloc (WORDS_LENGTH, sizeof(char));
assert(buf != NULL);
for (int i = 0; i < len; i++)
  {
  Input = fscanf(from, "%s", buf);
  //printf("cycle %s\n", buf);
  if (Input != 1)
    {
    break;
    }
  else
    {
    Hash_insert(&hs, buf);
    }
  }
fclose(from);
free(buf);
Hash_dump(&hs);

FILE* out = fopen("Hash_func_best.csv", "w");
assert (out != NULL);
for (int i = 0; i < hs.number; i++)
  {
  fprintf(out, "%d\n", ((hs.descr)[i]).amount);
  }
Hash_destruct(&hs);
fclose(out);
return 0;
}

//{=================================================================================
//! Hash_ok - program which check hash for errors
//!
//! @param      hs   - Pointer to hash
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST Hash_ok (EDHash_t* hs)
{
if (hs == NULL) return NULL_POINTER_ON_HASH;

if (hs->descr == NULL) return NULL_POINTER_ON_MASS_OF_DESCR;

if (hs->func == NULL) return NULL_POINTER_ON_FUNC;

if (&(hs->number) == NULL) return NULL_POINTER_ON_NUMBER_IN_HASH;

ERRORS_LIST nerror = OK;
for (int i = 0; i < hs->number; i++)
  {
  nerror = List_ok(&((hs->descr)[i]));
  if (nerror != OK) return nerror;
  }

return OK;
}

//{=================================================================================
//! Hash_construct - program which construct hash
//!
//! @param      hs   - Pointer to hash
//! @param  number   - Number of cells in hash-table
//! @param    func   - Pointer to hash-function
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST Hash_construct (EDHash_t* hs, int number,int (*func)(char* s))
{
ERRORS_LIST nerror = Hash_ok(hs);

if (nerror == NULL_POINTER_ON_HASH || nerror == NULL_POINTER_ON_FUNC)
  {
  assert_hash(nerror == OK);
  }

else
  {
  if (number == 0 || func == NULL)
    {
    nerror = BAD_INPUT;
    assert_hash(nerror == OK);
    }
  else
    {
    hs->number = number;
    hs->func   = func;
    hs->descr = (EDDescription_t*) calloc (number, sizeof(EDDescription_t));
    for (int i = 0; i < number; i++)
      List_construct(&((hs->descr)[i]));

    nerror = Hash_ok(hs);
    assert_hash(nerror == OK);
    }
  }

return nerror;
}

//{=================================================================================
//! Hash_destruct - program which destruct hash
//!
//! @param      hs   - Pointer to hash
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST Hash_destruct (EDHash_t* hs)
{
ERRORS_LIST nerror = Hash_ok(hs);
assert_hash(nerror == OK);

for (int i = 0; i < hs->number; i++)
  List_destruct(&((hs->descr)[i]));

hs->number = 0;
nerror = Hash_ok(hs);
assert_hash(nerror == OK);

hs->func = NULL;
return nerror;
}

//{=================================================================================
//! Hash_dump - program which print out some useful information about hash
//!
//! @param      hs   - Pointer to hash
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST Hash_dump (EDHash_t* hs)
{
FILE* output = fopen ("DUMP.TXT", "w");
assert(output != NULL);
fprintf(output,"=================================================================================\n");
fprintf(output,"The list of ERRORS:\n");
fprintf(output,"OK                             =  0:list is normal\n");
fprintf(output,"NULL_POINTER_ON_DESCRIPTION    =  1:pointer on description of list is NULL-pointer\n");
fprintf(output,"NULL_POINTER_ON_FIRST_ELEMENT  =  2:pointer on the first element is NULL-pointer\n");
fprintf(output,"NULL_POINTER_ON_LAST_ELEMENT   =  3:pointer on the last element is NULL-pointer\n");
fprintf(output,"WRONG_AMOUNT_OF_ELEMENTS       =  4:amount of elements in list is wrong\n");
fprintf(output,"WRONG_POINTER_ON_FIRST_ELEMENT =  5:pointer on the first element is wrong\n");
fprintf(output,"WRONG_POINTER_ON_LAST_ELEMENT  =  6:pointer on the last element is wrong\n");
fprintf(output,"BAD_INPUT                      =  7:bad parameters transmitted to function\n");
fprintf(output,"NULL_POINTER_ON_AMOUNT_IN_LIST =  8:pointer on the amount of elements is NULL\n");
fprintf(output,"NULL_POINTER_ON_FUNC           =  9:pointer on hash-function is NULL\n");
fprintf(output,"NULL_POINTER_ON_NUMBER_IN_HASH = 10:pointer on number of cells in hash is NULL\n");
fprintf(output,"NULL_POINTER_ON_MASS_OF_DESCR  = 11:pointer on massive of descriptions is NULL\n");
fprintf(output,"NULL_POINTER_ON_HASH           = 12:pointer on hash table is NULL\n");
fprintf(output,"---------------------------------------------------------------------------------\n");

ERRORS_LIST nerror = Hash_ok(hs);
fprintf(output,"EDHash_t [%p]. Number of error: %d\n", hs, nerror);
fprintf(output,"Number of lists: %d\n", hs->number);
if (nerror != NULL_POINTER_ON_HASH)
  {
  for (int i = 0; i < hs->number; i++)
    {
    fprintf(output,"---------------------------------------------------------------------------------\n");
    fprintf(output,"List number: %d\n", i);
    List_dump(&((hs->descr)[i]), output);
    }
  }

fprintf(output,"=================================================================================\n");
fclose(output);
return nerror;
}

//{=================================================================================
//! Hash_insert - program which create new element before of the first
//!                element in list and put in the new element specified value.
//!
//! @param      hs   - Pointer to hash
//! @param      val   - the value, which must be inserted in new element
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST Hash_insert (EDHash_t* hs, char* val)
{
ERRORS_LIST nerror = Hash_ok(hs);
assert_hash(nerror == OK);

if (val == NULL)
  {
  nerror = BAD_INPUT;
  assert_hash(nerror == OK);
  }
else
  {
  int place = (*(hs->func))(val) % (hs->number);

  if (List_isElement(&((hs->descr)[place]), val) == 0)
    {
    if (List_isEmpty(&((hs->descr)[place])) == 1)
      {
      List_firstIns(&((hs->descr)[place]), val);
      }
    else
      {
      List_headIns(&((hs->descr)[place]), val, BEFORE);
      }
    }
  }

nerror = Hash_ok(hs);
assert(nerror == OK);

return nerror;
}

//{=================================================================================
//! Hash_func_const - the simplest hash-fuction, which always returns 1
//!
//! @param      s - it's a string
//!
//! @return     1
//}=================================================================================

int Hash_func_const(char* s)
{
assert(s != NULL);
return 1;
}

//{=================================================================================
//! Hash_func_ASCII - the hash-fuction, which returns ASCII-code of the first letter
//!
//! @param      s - it's a string
//!
//! @return     ASCII-code of the first letter
//}=================================================================================

int Hash_func_ASCII(char* s)
{
assert(s != NULL);
int a = (int) s[0];
return a;
}

//{=================================================================================
//! Hash_func_length - the hash-fuction, which returns length of the word
//!
//! @param      s - it's a string
//!
//! @return     length of the word
//}=================================================================================

int Hash_func_length(char* s)
{
assert(s != NULL);
return strlen(s);
}

//{=================================================================================
//! Hash_func_sum - the hash-fuction, which returns sum of ASCII-codes of letters
//!
//! @param      s - it's a string
//!
//! @return     sum of ASCII-codes of letters
//}=================================================================================

int Hash_func_sum (char* s)
{
assert(s != NULL);
int sum = 0, len = strlen(s);

for (int i = 0; i < len; i++)
  sum += (int) s[i];

return sum;
}

//{=================================================================================
//! Hash_func_best - the hash-fuction, which is the best
//!
//! @param      s - it's a string
//!
//! @return     answer
//}=================================================================================

int Hash_func_best (char* s)
{
assert(s != NULL);
int answer = s[0], len = strlen(s);

for (int i = 1; i < len; i++)
  {
  answer = (answer << 1) ^ s[i];
  }

answer = abs(answer);
return answer;
}
#endif

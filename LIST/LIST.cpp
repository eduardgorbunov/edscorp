#ifndef LIST_CPP
#define LIST_CPP
//{=================================================================================
//! @file    LIST.cpp
//! @date    2014-11-15 18:00
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Library for using lists.
//}=================================================================================

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "ERRORS_LIST.h"
#include <assert.h>
#include "LOGIC.h"
#include "WHERE.h"

#define assert_list(cond) if (!cond)\
                            {\
                            FILE* dump = fopen ("DUMP.TXT", "w");\
                            assert(dump != NULL);\
                            List_dump(des, dump);\
                            printf("---------------------------------------------\n");\
                            printf("ERROR in the line number %d\n", __LINE__);\
                            printf("---------------------------------------------\n");\
                            assert(cond);\
                            }

struct EDList_t;

struct EDList_t
  {
  char* value;
  EDList_t* next;
  EDList_t* prev;
  };

struct EDDescription_t
  {
  int amount;
  EDList_t* first;
  EDList_t* last;
  };

ERRORS_LIST List_ok (EDDescription_t* des);

ERRORS_LIST List_construct (EDDescription_t* des);

ERRORS_LIST List_destruct (EDDescription_t* des);

ERRORS_LIST List_dump (EDDescription_t* des, FILE* output);

int List_isEmpty (EDDescription_t* des);

ERRORS_LIST List_firstIns (EDDescription_t* des, char* val);

ERRORS_LIST List_insert(EDDescription_t* des, EDList_t* element, char* val, where place);

ERRORS_LIST List_headIns(EDDescription_t* des, char* val, where place);

ERRORS_LIST List_tailIns(EDDescription_t* des, char* val, where place);

ERRORS_LIST List_delFirst(EDDescription_t* des);

ERRORS_LIST List_delLast(EDDescription_t* des);

ERRORS_LIST List_delete(EDDescription_t* des, EDList_t* element);

EDList_t* List_getPointer(EDDescription_t* des, int number);

int List_numOfElement(EDDescription_t* des, EDList_t* element);

int List_isElement (EDDescription_t* des, char* val);

/*int main ()
{
EDDescription_t des = {};
List_construct(&des);
List_firstIns(&des, "Lol");
List_headIns(&des, "trol", AFTER);
char* buf = (char*) calloc (100, sizeof(char));
scanf("%s", buf);
List_tailIns(&des, buf, AFTER);
scanf("%s", buf);
List_tailIns(&des, buf, AFTER);
List_dump(&des);
printf("%d\n", List_isElement(&des, "Lol"));
printf("%d\n", List_isElement(&des, "trol"));
scanf("%s", buf);
printf("%d\n", List_isElement(&des, buf));
List_destruct(&des);
} */

//{=================================================================================
//! List_ok - program which check list for errors
//!
//! @param      des   - Pointer to a head-structure (description) of list
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_ok (EDDescription_t* des)
{
if (des == NULL) return NULL_POINTER_ON_DESCRIPTION;

//if list is empty, it isn't an error
//amount == 0 only in one situation (look down); otherwise it is an error
if (des->amount == 0 && des->first == NULL && des->last == NULL) return OK;

if (des->first == NULL) return NULL_POINTER_ON_FIRST_ELEMENT;

if (des->last == NULL) return NULL_POINTER_ON_LAST_ELEMENT;

if (&(des->amount) == NULL) return NULL_POINTER_ON_AMOUNT_IN_LIST;

if ((des->first)->prev != NULL) return WRONG_POINTER_ON_FIRST_ELEMENT;

if ((des->last)->next !=NULL) return WRONG_POINTER_ON_LAST_ELEMENT;

EDList_t* pointer = des->first;
int i = 0;
while (i < des->amount && pointer->next != NULL)
  {
  i++;
  pointer = pointer->next;
  }
if (pointer->next != NULL || des->amount <= 0) return WRONG_AMOUNT_OF_ELEMENTS;

i = 0;
pointer = des->last;
while (i < des->amount && pointer->prev != NULL)
  {
  i++;
  pointer = pointer->prev;
  }
if (pointer->prev != NULL || des->amount <= 0) return WRONG_AMOUNT_OF_ELEMENTS;

return OK;
}

//{=================================================================================
//! List_construct - program which construct the list
//!
//! @param      des   - Pointer to a head-structure (description) of list
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_construct (EDDescription_t* des)
{
ERRORS_LIST nerror = List_ok(des);

if (nerror == NULL_POINTER_ON_DESCRIPTION)
  {
  assert_list(nerror == OK);
  }

else
  {
  des->amount = 0;
  des->first = NULL;
  des->last  = NULL;
  }

assert_list(nerror == OK)

return nerror;
}

//{=================================================================================
//! List_destruct - program which destruct the list
//!
//! @param      des   - Pointer to a head-structure (description) of list
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_destruct (EDDescription_t* des)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

if (List_isEmpty(des) == 0)
  {
  EDList_t* pointer = des->first;
  for (int i = 0; i < des->amount; i++)
    {
    free(pointer->value);
    free(pointer->prev);
    pointer = pointer->next;
    }

  free(des->last);
  des->amount = 0;
  des->first = NULL;
  des->last = NULL;
  }

nerror = List_ok(des);
assert_list(nerror == OK);

return nerror;
}

//{=================================================================================
//! List_dump - program which print out some useful information about list
//!
//! @param      des   - Pointer to a head-structure (description) of list
//! @param   output   - Pointer to the file for dump
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_dump (EDDescription_t* des, FILE* output)
{
/*printf("=================================================================================\n");
printf("The list of ERRORS:\n");
printf("OK                             = 0:list is normal\n");
printf("NULL_POINTER_ON_DESCRIPTION    = 1:pointer on description of list is NULL-pointer\n");
printf("NULL_POINTER_ON_FIRST_ELEMENT  = 2:pointer on the first element is NULL-pointer\n");
printf("NULL_POINTER_ON_LAST_ELEMENT   = 3:pointer on the last element is NULL-pointer\n");
printf("WRONG_AMOUNT_OF_ELEMENTS       = 4:amount of elements in list is wrong\n");
printf("WRONG_POINTER_ON_FIRST_ELEMENT = 5:pointer on the first element is wrong\n");
printf("WRONG_POINTER_ON_LAST_ELEMENT  = 6:pointer on the last element is wrong\n");
printf("BAD_INPUT                      = 7:bad parameters transmitted to function\n");
printf("NULL_POINTER_ON_AMOUNT_IN_LIST = 8:pointer on the amount of elements is NULL\n");
printf("---------------------------------------------------------------------------------\n");
*/
ERRORS_LIST nerror = List_ok(des);
fprintf(output,"EDDescription [%p]. Number of error: %d\n", des, nerror);

if (nerror != NULL_POINTER_ON_DESCRIPTION && nerror != NULL_POINTER_ON_FIRST_ELEMENT
    && nerror != NULL_POINTER_ON_LAST_ELEMENT)
  {
  int i = 0;
  EDList_t* pointer = des->first;

  while (i < des->amount && pointer != NULL)
    {
    i++;
    fprintf(output,"%d: (%p) value[%s] prev(%p) next(%p)\n", i, pointer, pointer->value, pointer->prev, pointer->next);
    pointer = pointer->next;
    }
  }
//printf("=================================================================================\n");

return nerror;
}

//{=================================================================================
//! List_isEmpty - program which determine is the list empty
//!
//! @param      des   - Pointer to a head-structure (description) of list
//!
//! @return     1 - if list is EMPTY; 0 - if list is NOT EMPTY
//}=================================================================================

int List_isEmpty (EDDescription_t* des)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

int isEmpty = 0;
if (des->amount == 0) isEmpty = 1;

nerror = List_ok(des);
assert_list(nerror == OK);

return isEmpty;
}

//{=================================================================================
//! List_firstIns - program which create the first element in the EMPTY list
//!                 !!!USE ONLY FOR THE INSERTION IN THE EMPTY LIST!!!
//!
//! @param      des   - Pointer to a head-structure (description) of list
//! @param      val   - The value which must be inserted in the new element
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_firstIns (EDDescription_t* des, char* val)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

if (val == NULL)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }

if (List_isEmpty(des) == 1)
  {
  des->amount = 1;
  EDList_t* pointer = (EDList_t*) calloc (1, sizeof(EDList_t));
  pointer->value = (char*) calloc (strlen(val) + 1, sizeof(char));
  strcpy(pointer->value, val);
  pointer->prev = NULL;
  pointer->next = NULL;
  des->first = pointer;
  des->last  = pointer;
  }
else
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }

nerror = List_ok(des);
assert_list(nerror == OK);

return nerror;
}

//{=================================================================================
//! List_insert - program which create new element before or after of the specified
//!               element and put in the new element specified value.
//!
//!               !!!DON"T USE FOR THE INSERTION IN THE EMPTY LIST!!!
//!
//! @param      des   - Pointer to a head-structure (description) of list
//! @param      element - The element before or after which you want to create a new element
//! @param      val   - the value, which must be inserted in new element
//! @param      place - AFTER or BEFORE - place of the new element in list
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_insert(EDDescription_t* des, EDList_t* element, char* val, where place)
{
ERRORS_LIST nerror = OK;

if (element == NULL || val == NULL || List_isEmpty(des) == 1)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }

nerror = List_ok(des);
assert_list(nerror == OK);

int i = List_numOfElement(des, element);

if (i == 0)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }
else
  {
  int len = strlen(val);
  len++;

  EDList_t* pointer = (EDList_t*) calloc(1, sizeof(EDList_t));
  pointer->value = (char*) calloc (len, sizeof(char));
  strcpy(pointer->value, val);
  if (place == BEFORE)
    {
    pointer->next = element;
    pointer->prev = element->prev;
    if (element->prev == NULL)
      {
      des->first = pointer;
      }
    else
      {
      (element->prev)->next = pointer;
      }
    element->prev = pointer;
    }
  else
    {
    pointer->prev = element;
    pointer->next = element->next;
    if (element->next == NULL)
      {
      des->last = pointer;
      }
    else
      {
      (element->next)->prev = pointer;
      }
    element->next = pointer;
    }
    (des->amount)++;
  }

nerror = List_ok(des);
assert_list(nerror == OK);

return nerror;
}

//{=================================================================================
//! List_headIns - program which create new element before or after of the first
//!                element and put in the new element specified value.
//!
//!               !!!DON"T USE FOR THE INSERTION IN THE EMPTY LIST!!!
//!
//! @param      des   - Pointer to a head-structure (description) of list
//! @param      val   - the value, which must be inserted in new element
//! @param      place - AFTER or BEFORE - place of the new element in list
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_headIns(EDDescription_t* des, char* val, where place)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

List_insert(des, des->first, val, place);

nerror = List_ok(des);
assert_list(nerror == OK);

return nerror;
}

//{=================================================================================
//! List_tailIns - program which create new element before or after of the last
//!                element and put in the new element specified value.
//!
//!               !!!DON"T USE FOR THE INSERTION IN THE EMPTY LIST!!!
//!
//! @param      des   - Pointer to a head-structure (description) of list
//! @param      val   - the value, which must be inserted in new element
//! @param      place - AFTER or BEFORE - place of the new element in list
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_tailIns(EDDescription_t* des, char* val, where place)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

List_insert(des, des->last, val, place);

nerror = List_ok(des);
assert_list(nerror == OK);

return nerror;
}

//{=================================================================================
//! List_delFirst - program which delete the first element from the list
//!
//!               !!!DON"T USE FOR THE DELETION FROM THE EMPTY LIST!!!
//!
//! @param      des   - Pointer to a head-structure (description) of list
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_delFirst(EDDescription_t* des)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

if (List_isEmpty(des) == 1)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }

if (des->amount == 1)
  {
  free((des->first)->value);
  free(des->first);
  des->first = NULL;
  des->last = NULL;
  des->amount = 0;
  }
else
  {
  (des->amount)--;
  (des->first) = (des->first)->next;
  free(((des->first)->prev)->value);
  free((des->first)->prev);
  (des->first)->prev = NULL;
  }

nerror = List_ok(des);
assert_list(nerror == OK);

return nerror;
}

//{=================================================================================
//! List_delLast - program which delete the last element from the list
//!
//!               !!!DON"T USE FOR THE DELETION FROM THE EMPTY LIST!!!
//!
//! @param      des   - Pointer to a head-structure (description) of list
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_delLast(EDDescription_t* des)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

if (List_isEmpty(des) == 1)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }

if (des->amount == 1)
  {
  free((des->last)->value);
  free(des->last);
  des->first = NULL;
  des->last = NULL;
  des->amount = 0;
  }
else
  {
  (des->amount)--;
  (des->last) = (des->last)->prev;
  free(((des->last)->next)->value);
  free((des->last)->next);
  (des->last)->next = NULL;
  }

nerror = List_ok(des);
assert_list(nerror == OK);

return nerror;
}

//{=================================================================================
//! List_delete - program which delete the specified element from the list
//!
//!               !!!DON"T USE FOR THE DELETION FROM THE EMPTY LIST!!!
//!
//! @param      des   - Pointer to a head-structure (description) of list
//! @param      element - The element which must be deleted
//!
//! @return     Number of an error
//}=================================================================================

ERRORS_LIST List_delete(EDDescription_t* des, EDList_t* element)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

if (element == NULL || List_isEmpty(des) == 1)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }

LOGIC isInList = NO;

int i = 0;
EDList_t* pointer = des->first;
while (i < des->amount && isInList == NO)
  {
  if (pointer == element) isInList = YES;
  i++;
  }

if (isInList == NO)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }
else
  {
  if (element == des->first)
    {
    List_delFirst(des);
    }
  else
    {
    if (element == des->last)
      {
      List_delLast(des);
      }
    else
      {
      (element->prev)->next = element->next;
      (element->next)->prev = element->prev;
      free(element->value);
      free(element);
      }
    }
  }

nerror = List_ok(des);
assert_list(nerror == OK);

return nerror;
}

//{=================================================================================
//! List_getPointer - program which find element by its number
//!                 !!!DON"T USE FOR THE EMPTY LIST!!!
//!
//! @param      des    - Pointer to a head-structure (description) of list
//! @param      number - The number of the element
//!
//! @return     pointer to the element in list
//}=================================================================================

EDList_t* List_getPointer(EDDescription_t* des, int number)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(des);

if (number <= 0 || number > des->amount)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }

EDList_t* pointer = des->first;
for (int i = 1; i < number; i++) pointer = pointer->next;

nerror = List_ok(des);
assert(nerror == OK);

return pointer;
}

//{=================================================================================
//! List_numOfElement- program which find number of element in the list
//!                 !!!DON"T USE FOR THE EMPTY LIST!!!
//!
//! @param      des     - Pointer to a head-structure (description) of list
//! @param      element - Pointer to the element
//!
//! @return     number of the element if it is in the list; 0 - otherwise
//}=================================================================================

int List_numOfElement(EDDescription_t* des, EDList_t* element)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

if (element == NULL)
  {
  nerror = BAD_INPUT;
  assert(nerror == OK);
  }

LOGIC isInList = NO;

int i = 0;
EDList_t* pointer = des->first;
while (i < des->amount && isInList == NO)
  {
  if (pointer == element) isInList = YES;
  pointer = pointer->next;
  i++;
  }

if (isInList == NO)
  {
  return 0;
  }
else
  {
  return i;
  }
}

//{=================================================================================
//! List_isElement - program which answer the question, is it the element
//!
//! @param      des     - Pointer to a head-structure (description) of list
//! @param      val     - Value
//!
//! @return     1 - if it is the element; 0 - otherwise
//}=================================================================================

int List_isElement (EDDescription_t* des, char* val)
{
ERRORS_LIST nerror = List_ok(des);
assert_list(nerror == OK);

int flag = 0;
if (val == NULL)
  {
  nerror = BAD_INPUT;
  assert_list(nerror == OK);
  }
else
  {
  EDList_t* pointer = des->first;
  while (pointer != NULL && flag == 0)
    {
    if (strcmp(pointer->value,val) == 0)
      {
      nerror = List_ok(des);
      assert_list(des);
      flag = 1;
      }
    pointer = pointer->next;
    }
  }

nerror = List_ok(des);
assert_list(des);
return flag;
}
#endif

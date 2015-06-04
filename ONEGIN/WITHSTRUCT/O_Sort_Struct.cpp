//{=================================================================================
//! @file    O_Sort_Struct.cpp
//! @date    2014-11-04 13:52
//! @author  Eduard Gorbunov <ed-gorbunov@yandex.ru>
//!
//! Sorting Eugene Onegin.
//!
//! @par  Eugene Onegin is an encyclopedia of Russian life, as Belinsky said.
//!       But in the encyclopedia everything should be in order.
//!       So this program sort strings in Eugene Onegin in right order.
//}=================================================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <assert.h>

#define POISON 1337

struct String_t
  {
  char* str;
  int len;
  int alllen;
  };

int CountOf (char* buf, char a, int length);

int ComparatorED (const void* a, const void* b);

void Encyclopedia ();

void RhymingDictionary ();

void ReadFileInBuffer (char** buf, int* len, FILE* f);

void CreateArrayOfPointersToStr (String_t** StrsAndLens, int* AmountOfStr, char* buf, int len);

void ReverseString (String_t* StrsAndLens);

void PrintfInFile (String_t** StrsAndLens, int AmountOfStr, FILE* out);

int main()
{
printf("==============================================================\n");
printf("What do you want to do with Eugene Onegin?\n");
printf("1: to sort like in Encyclopedia (in alphabetical order)\n");
printf("2: to create the rhyming dictionary\n");
printf("Your choice: ");

int N = 0;
int Input = scanf("%d", &N);

assert(0 < N && N < 3 && Input == 1);

switch (N)
{
case 1:  Encyclopedia ();
         break;
case 2:  RhymingDictionary ();
         break;
default: printf("FAILURE IN MATRIX: WRONG ASSERT");
         break;
}
return 0;
}

//{=================================================================================
//! Encyclopedia - sort Eugene Onegin in alphabetical order
//}=================================================================================

void Encyclopedia ()
{
FILE *f = fopen ("ONEGIN.TXT", "r");
char* buf = NULL;
int len = POISON;

assert(f != NULL);
ReadFileInBuffer(&buf, &len, f);

String_t* StrsAndLens = NULL;
int AmountOfStr = 0;

CreateArrayOfPointersToStr(&StrsAndLens, &AmountOfStr, buf, len);

qsort (StrsAndLens, AmountOfStr, sizeof(String_t), ComparatorED);

FILE* out = fopen("OUTPUT.TXT", "w");

assert(out != NULL);
PrintfInFile (&StrsAndLens, AmountOfStr, out);

fclose(f);
fclose(out);
free(buf);
free(StrsAndLens);
}

//{=================================================================================
//! RhymingDictionary - create the rhyming dictionary by strings in Eugene Onegin
//}=================================================================================

void RhymingDictionary ()
{
FILE *f = fopen ("ONEGIN.TXT", "r");
char* buf = NULL;
int len = POISON;

assert(f != NULL);
ReadFileInBuffer(&buf, &len, f);

String_t* StrsAndLens = NULL;
int AmountOfStr = 0;

CreateArrayOfPointersToStr(&StrsAndLens, &AmountOfStr, buf, len);

int i = 0;

for (i = 0; i < AmountOfStr; i++)
  {
  ReverseString(&StrsAndLens[i]);
  }

qsort (StrsAndLens, AmountOfStr, sizeof(String_t), ComparatorED);

for (i = 0; i < AmountOfStr; i++)
  {
  ReverseString(&StrsAndLens[i]);
  }

FILE* out = fopen("OUTPUT.TXT", "w");

assert(out != NULL);
PrintfInFile (&StrsAndLens, AmountOfStr, out);

fclose(f);
fclose(out);
free(buf);
free(StrsAndLens);
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

return (counter);
}

//{=================================================================================
//! ComparatorED - comparison of the two strings
//!
//! @param      a   The pointer on the structure of the type String_t
//! @param      b   The pointer on the structure of the type String_t
//!
//! @return     A positive number, when the first string > the second string
//!                          Zero, when the first string = the second string
//!             A negative number, when the first string < the second string
//}=================================================================================

int ComparatorED (const void* a, const void* b)
{
return strcmp ( (const char*) ((String_t*) a)->str, (const char*) ((String_t*) b)->str);
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
//! CreateArrayOfPointersToStr - program which create array of pointer to strings
//!
//! @param[out]      StrsAndLens - Pointer to the String_t*
//! @param[out]      AmountOfStr - The amount of strings
//! @param[out]      buf - Pointer to the buffer
//! @param[out]      len - The length of the buffer
//}=================================================================================

void CreateArrayOfPointersToStr (String_t** StrsAndLens, int* AmountOfStr, char* buf, int len)
{
assert(AmountOfStr != NULL);
assert(StrsAndLens != NULL);
assert(*StrsAndLens == NULL);

*AmountOfStr = CountOf(buf, '\n', len);

*StrsAndLens = (String_t*) calloc (*AmountOfStr, sizeof(String_t));
assert((*StrsAndLens) != NULL);

int i = 0, startStr = 0, nStr = 0, lastLetter = 0;
char pocket = 0;

for (i = 0; i < len; i++)
  {
  pocket = buf[i];

  if (pocket == '\n')
    {
    buf[i] = '\0';

    ((*StrsAndLens)[nStr]).str = buf + startStr;

    ((*StrsAndLens)[nStr]).len = lastLetter - startStr + 1;

    ((*StrsAndLens)[nStr]).alllen = i - startStr + 1;

    nStr++;

    startStr = i + 1;
    lastLetter = i + 1;
    }
  else if (('à' <= pocket && pocket <='ÿ') || ('À' <= pocket && pocket <= 'ß')) lastLetter = i;
  }
}

//{=================================================================================
//! ReverseString - program which reverse first len'th symbols in string
//!
//! @param[out]      StrsAndLens - Pointer to structure String_t
//}=================================================================================

void ReverseString (String_t* StrsAndLens)
{
assert(StrsAndLens != NULL);

int center = (int) ((StrsAndLens->len - 1)/2), i = POISON;
char pocket = '\0';

  for (i = 0; i <= center; i++)
    {
    pocket = (StrsAndLens->str)[i];
    (StrsAndLens->str)[i] = (StrsAndLens->str)[StrsAndLens->len - i - 1];
    (StrsAndLens->str)[StrsAndLens->len - i - 1] = pocket;
    }
}

//{=================================================================================
//! PrintInFile - to print strings in file
//!
//! @param[out]      StrsAndLens - Pointer to structure String_t
//}=================================================================================

void PrintfInFile (String_t** StrsAndLens, int AmountOfStr, FILE* out)
{
assert(out != NULL && StrsAndLens != NULL && *StrsAndLens != NULL);

setlocale(LC_ALL, "Russian");

int i = 0;
for (i = 0; i < AmountOfStr; i++)
  {
  fwrite((const char*) ((*StrsAndLens)[i]).str, sizeof(char), ((*StrsAndLens)[i]).alllen, out);
  putc('\n', out);
  }
}

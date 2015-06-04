#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <assert.h>

int main()
{
FILE* f = fopen("Hobbit.TXT", "r");
char* buf = (char*) calloc (ftell(f), sizeof(char));
fscanf(f, "%s", buf);
setlocale(LC_ALL, "Russian");
printf("%s", buf);
fclose(f);
return 0;
}

char** NamesOfComands = (char**) calloc(NUMBER_OF_COMANDS, sizeof(char*));
assert(NamesOfComands != NULL);

#define DEF_CMD_(name, num, code) {NamesOfComands[num] = (char*) calloc(6, sizeof(char));\
                                   assert(NamesOfComands[num] != NULL); \
                                   NamesOfComands[num] = #name;}
#include "Comands.h"
#undef DEF_CMD_

int nSYMBOL = 0, *labels = (int*) calloc (10, sizeof(int));
assert(labels != NULL);

for (nSYMBOL = 0; nSYMBOL < 10; nSYMBOL++) labels[nSYMBOL] = -1;

int nLINE = 0, NumOfCmd = POISON, NumOfCellIncmds = 0, //nLINE - number of string
    COUNTERofCMD = 0, COUNTERofDIGITS = 0, NumberOfReg = 0, wasPoint = 0,
    nowLabel = POISON;
char  *comand = (char*) calloc(6, sizeof(char)), *NameOfReg = (char*) calloc(3, sizeof(char)),
      *number = (char*) calloc(30, sizeof(char));
double* cmds = (double*) calloc (len, sizeof(double));

assert(cmds != NULL);
assert(comand != NULL);
assert(NameOfReg != NULL);
assert(number != NULL);
assert(nLINE == 0);

while (nLINE < AmountOfStr)
  {
  nSYMBOL = 0;
  NumOfCmd = 0;

  if ((str[nLINE])[nSYMBOL] == ':' && isdigit((str[nLINE])[nSYMBOL + 1]) != 0)
    {
    number[0] = (str[nLINE])[nSYMBOL + 1];
    number[1] = '\0';

    nowLabel = atoi(number);

    for (nSYMBOL = 0; nSYMBOL < 10; nSYMBOL++)
      {
      if (nowLabel == nSYMBOL)
        {
        assert(labels[nowLabel] == -1);

        labels[nowLabel] = NumOfCellIncmds;
        cmds[NumOfCellIncmds] = nowLabel;
        NumOfCellIncmds++;
        }
      }
    }
  else
    {
    while (isalpha((str[nLINE])[nSYMBOL]) != 0)
      {
      comand[nSYMBOL] = (str[nLINE])[nSYMBOL];
      nSYMBOL++;
      }

    comand[nSYMBOL] = '\0';
    COUNTERofCMD = 0;

    assert(COUNTERofCMD == 0);
    while (COUNTERofCMD < NUMBER_OF_COMANDS)
      {
      if (strcmp(comand, NamesOfComands[COUNTERofCMD + 10]) == 0)
        {
        NumOfCmd = COUNTERofCMD + 10;
        COUNTERofCMD = NUMBER_OF_COMANDS;
        }
      else COUNTERofCMD++;
      }

    switch (NumOfCmd)
    {
    #define DEF_CMD_(name, num, code) case num: code; break;
    #include "ComandsASM.h"
    #undef DEF_CMD_
    default: printf("WRONG COMAND, LINE = %d\n", nLINE+1);
             assert(0);
             break;
    }
    }

  nLINE++;
  }

free(comand);
free(NameOfReg);
free(number);
free(NamesOfComands);

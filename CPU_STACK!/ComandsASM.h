#ifndef COMANDS_ASM_H
#define COMANDS_ASM_H
DEF_CMD_(PUSH,  10, {fprintf(assm, "%d ", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd;
         nSYMBOL++;
         assert(isdigit((str[nLINE])[nSYMBOL]) != 0 || (str[nLINE])[nSYMBOL] == '-'
         || (str[nLINE])[nSYMBOL] == '+');
         COUNTERofDIGITS = 0;
         number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
         nSYMBOL++;
         COUNTERofDIGITS++;
         wasPoint = 0;
         while (isdigit((str[nLINE])[nSYMBOL]) != 0 ||
               ((str[nLINE])[nSYMBOL] == '.' && wasPoint == 0))
           {
           number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
           if ((str[nLINE])[nSYMBOL] == '.') wasPoint = 1;
           nSYMBOL++;
           COUNTERofDIGITS++;
           }
         number[COUNTERofDIGITS] = '\0';
         fprintf(assm, "%lg\n", atof(number));
         cmds[NumOfCellIncmds + 1] = atof(number);
         NumOfCellIncmds = NumOfCellIncmds + 2;})
DEF_CMD_(ADD,   11, {fprintf(assm, "%d\n", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(MUL,   12, {fprintf(assm, "%d\n", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(POPR,  13, {fprintf(assm, "%d ", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd;
         assert(isalpha((str[nLINE])[nSYMBOL+1]) != 0 && isalpha((str[nLINE])[nSYMBOL+2]) != 0);
         NameOfReg[0] = (str[nLINE])[nSYMBOL+1];
         NameOfReg[1] = (str[nLINE])[nSYMBOL+2];
         NameOfReg[2] = '\0';
         if (strcmp(NameOfReg, "ax\0") == 0) NumberOfReg = 1;
         if (strcmp(NameOfReg, "bx\0") == 0) NumberOfReg = 2;
         if (strcmp(NameOfReg, "cx\0") == 0) NumberOfReg = 3;
         if (strcmp(NameOfReg, "dx\0") == 0) NumberOfReg = 4;
         assert(0 < NumberOfReg && NumberOfReg < 5);
         fprintf(assm, "%d\n", NumberOfReg);
         cmds[NumOfCellIncmds+1] = NumberOfReg;
         NumOfCellIncmds = NumOfCellIncmds + 2;})
DEF_CMD_(OUTR,  14, {fprintf(assm, "%d ", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd;
         assert(isalpha((str[nLINE])[nSYMBOL+1]) != 0 && isalpha((str[nLINE])[nSYMBOL+2]) != 0);
         NameOfReg[0] = (str[nLINE])[nSYMBOL+1];
         NameOfReg[1] = (str[nLINE])[nSYMBOL+2];
         NameOfReg[2] = '\0';
         if (strcmp(NameOfReg, "ax\0") == 0) NumberOfReg = 1;
         if (strcmp(NameOfReg, "bx\0") == 0) NumberOfReg = 2;
         if (strcmp(NameOfReg, "cx\0") == 0) NumberOfReg = 3;
         if (strcmp(NameOfReg, "dx\0") == 0) NumberOfReg = 4;
         assert(0 < NumberOfReg && NumberOfReg < 5);
         fprintf(assm, "%d\n", NumberOfReg);
         cmds[NumOfCellIncmds+1] = NumberOfReg;
         NumOfCellIncmds = NumOfCellIncmds + 2;})
DEF_CMD_(END,   15, {fprintf(assm, "%d\n", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(IN,    16, {fprintf(assm, "%d ", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd;
         assert(isalpha((str[nLINE])[nSYMBOL+1]) != 0 && isalpha((str[nLINE])[nSYMBOL+2]) != 0);
         NameOfReg[0] = (str[nLINE])[nSYMBOL+1];
         NameOfReg[1] = (str[nLINE])[nSYMBOL+2];
         NameOfReg[2] = '\0';
         if (strcmp(NameOfReg, "ax\0") == 0) NumberOfReg = 1;
         if (strcmp(NameOfReg, "bx\0") == 0) NumberOfReg = 2;
         if (strcmp(NameOfReg, "cx\0") == 0) NumberOfReg = 3;
         if (strcmp(NameOfReg, "dx\0") == 0) NumberOfReg = 4;
         assert(0 < NumberOfReg && NumberOfReg < 5);
         fprintf(assm, "%d\n", NumberOfReg);
         cmds[NumOfCellIncmds+1] = NumberOfReg;
         NumOfCellIncmds = NumOfCellIncmds + 2;})
DEF_CMD_(INFO,  17, {fprintf(assm, "%d\n", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(OUT,   18, {fprintf(assm, "%d\n", NumOfCmd);
                     cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(SQRT,  19, {fprintf(assm, "%d\n", NumOfCmd);
                     cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(DIV,   20, {fprintf(assm, "%d\n", NumOfCmd);
                     cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(SIN,   21, {fprintf(assm, "%d\n", NumOfCmd);
                     cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(COS,   22, {fprintf(assm, "%d\n", NumOfCmd);
                     cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(TAN,   23, {fprintf(assm, "%d\n", NumOfCmd);
                     cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(POW,   24, {fprintf(assm, "%d ", NumOfCmd);
         cmds[NumOfCellIncmds] = NumOfCmd;
         nSYMBOL++;
         assert(isdigit((str[nLINE])[nSYMBOL]) != 0 || (str[nLINE])[nSYMBOL] == '-'
         || (str[nLINE])[nSYMBOL] == '+');
         COUNTERofDIGITS = 0;
         number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
         nSYMBOL++;
         COUNTERofDIGITS++;
         wasPoint = 0;
         while (isdigit((str[nLINE])[nSYMBOL]) != 0 ||
               ((str[nLINE])[nSYMBOL] == '.' && wasPoint == 0))
           {
           number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
           if ((str[nLINE])[nSYMBOL] == '.') wasPoint = 1;
           nSYMBOL++;
           COUNTERofDIGITS++;
           }
         number[COUNTERofDIGITS] = '\0';
         fprintf(assm, "%lg\n", atof(number));
         cmds[NumOfCellIncmds + 1] = atof(number);
         NumOfCellIncmds = NumOfCellIncmds + 2;})
DEF_CMD_(SWAP,  25, {fprintf(assm, "%d\n", NumOfCmd);
                     cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(JMP,   26, {fprintf(assm, "%d ", NumOfCmd);
                     cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;
                    assert((str[nLINE])[nSYMBOL] == ' ' && (str[nLINE])[nSYMBOL+1] == ':'
                            && isdigit((str[nLINE])[nSYMBOL+2]) != 0);
                    number[0] = (str[nLINE])[nSYMBOL+2];
                    number[1] = '\0';
                    fprintf(assm, "%d\n", atoi(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;})
DEF_CMD_(JA,    27, {fprintf(assm, "%d ", NumOfCmd);
                    cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;
                    assert((str[nLINE])[nSYMBOL] == ' ' && (str[nLINE])[nSYMBOL+1] == ':'
                            && isdigit((str[nLINE])[nSYMBOL+2]) != 0);
                    number[0] = (str[nLINE])[nSYMBOL+2];
                    number[1] = '\0';
                    fprintf(assm, "%d ", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;
                    nSYMBOL = nSYMBOL + 3;
                    assert((str[nLINE])[nSYMBOL] == ' ');
                    nSYMBOL++;
                    assert(isdigit((str[nLINE])[nSYMBOL]) != 0 || (str[nLINE])[nSYMBOL] == '-'
                           || (str[nLINE])[nSYMBOL] == '+');
                    COUNTERofDIGITS = 0;
                    number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                    nSYMBOL++;
                    COUNTERofDIGITS++;
                    wasPoint = 0;
                    while (isdigit((str[nLINE])[nSYMBOL]) != 0 ||
                          ((str[nLINE])[nSYMBOL] == '.' && wasPoint == 0))
                      {
                      number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                      if ((str[nLINE])[nSYMBOL] == '.') wasPoint = 1;
                      nSYMBOL++;
                      COUNTERofDIGITS++;
                      }
                    number[COUNTERofDIGITS] = '\0';
                    fprintf(assm, "%lg\n", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;})

DEF_CMD_(JAE,   28,{fprintf(assm, "%d ", NumOfCmd);
                    cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;
                    assert((str[nLINE])[nSYMBOL] == ' ' && (str[nLINE])[nSYMBOL+1] == ':'
                            && isdigit((str[nLINE])[nSYMBOL+2]) != 0);
                    number[0] = (str[nLINE])[nSYMBOL+2];
                    number[1] = '\0';
                    fprintf(assm, "%d ", atoi(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;
                    nSYMBOL = nSYMBOL + 3;
                    assert((str[nLINE])[nSYMBOL] == ' ');
                    nSYMBOL++;
                    assert(isdigit((str[nLINE])[nSYMBOL]) != 0 || (str[nLINE])[nSYMBOL] == '-'
                           || (str[nLINE])[nSYMBOL] == '+');
                    COUNTERofDIGITS = 0;
                    number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                    nSYMBOL++;
                    COUNTERofDIGITS++;
                    wasPoint = 0;
                    while (isdigit((str[nLINE])[nSYMBOL]) != 0 ||
                          ((str[nLINE])[nSYMBOL] == '.' && wasPoint == 0))
                      {
                      number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                      if ((str[nLINE])[nSYMBOL] == '.') wasPoint = 1;
                      nSYMBOL++;
                      COUNTERofDIGITS++;
                      }
                    number[COUNTERofDIGITS] = '\0';
                    fprintf(assm, "%lg\n", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;})
DEF_CMD_(JB,    29,{fprintf(assm, "%d ", NumOfCmd);
                    cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;
                    assert((str[nLINE])[nSYMBOL] == ' ' && (str[nLINE])[nSYMBOL+1] == ':'
                            && isdigit((str[nLINE])[nSYMBOL+2]) != 0);
                    number[0] = (str[nLINE])[nSYMBOL+2];
                    number[1] = '\0';
                    fprintf(assm, "%d ", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;
                    nSYMBOL = nSYMBOL + 3;
                    assert((str[nLINE])[nSYMBOL] == ' ');
                    nSYMBOL++;
                    assert(isdigit((str[nLINE])[nSYMBOL]) != 0 || (str[nLINE])[nSYMBOL] == '-'
                           || (str[nLINE])[nSYMBOL] == '+');
                    COUNTERofDIGITS = 0;
                    number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                    nSYMBOL++;
                    COUNTERofDIGITS++;
                    wasPoint = 0;
                    while (isdigit((str[nLINE])[nSYMBOL]) != 0 ||
                          ((str[nLINE])[nSYMBOL] == '.' && wasPoint == 0))
                      {
                      number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                      if ((str[nLINE])[nSYMBOL] == '.') wasPoint = 1;
                      nSYMBOL++;
                      COUNTERofDIGITS++;
                      }
                    number[COUNTERofDIGITS] = '\0';
                    fprintf(assm, "%d\n", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;})
DEF_CMD_(JBE,   30,{fprintf(assm, "%d ", NumOfCmd);
                    cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;
                    assert((str[nLINE])[nSYMBOL] == ' ' && (str[nLINE])[nSYMBOL+1] == ':'
                            && isdigit((str[nLINE])[nSYMBOL+2]) != 0);
                    number[0] = (str[nLINE])[nSYMBOL+2];
                    number[1] = '\0';
                    fprintf(assm, "%d ", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;
                    nSYMBOL = nSYMBOL + 3;
                    assert((str[nLINE])[nSYMBOL] == ' ');
                    nSYMBOL++;
                    assert(isdigit((str[nLINE])[nSYMBOL]) != 0 || (str[nLINE])[nSYMBOL] == '-'
                           || (str[nLINE])[nSYMBOL] == '+');
                    COUNTERofDIGITS = 0;
                    number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                    nSYMBOL++;
                    COUNTERofDIGITS++;
                    wasPoint = 0;
                    while (isdigit((str[nLINE])[nSYMBOL]) != 0 ||
                          ((str[nLINE])[nSYMBOL] == '.' && wasPoint == 0))
                      {
                      number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                      if ((str[nLINE])[nSYMBOL] == '.') wasPoint = 1;
                      nSYMBOL++;
                      COUNTERofDIGITS++;
                      }
                    number[COUNTERofDIGITS] = '\0';
                    fprintf(assm, "%lg\n", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;})
DEF_CMD_(JE,    31,{fprintf(assm, "%d ", NumOfCmd);
                    cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;
                    assert((str[nLINE])[nSYMBOL] == ' ' && (str[nLINE])[nSYMBOL+1] == ':'
                            && isdigit((str[nLINE])[nSYMBOL+2]) != 0);
                    number[0] = (str[nLINE])[nSYMBOL+2];
                    number[1] = '\0';
                    fprintf(assm, "%d ", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;
                    nSYMBOL = nSYMBOL + 3;
                    assert((str[nLINE])[nSYMBOL] == ' ');
                    nSYMBOL++;
                    assert(isdigit((str[nLINE])[nSYMBOL]) != 0 || (str[nLINE])[nSYMBOL] == '-'
                           || (str[nLINE])[nSYMBOL] == '+');
                    COUNTERofDIGITS = 0;
                    number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                    nSYMBOL++;
                    COUNTERofDIGITS++;
                    wasPoint = 0;
                    while (isdigit((str[nLINE])[nSYMBOL]) != 0 ||
                          ((str[nLINE])[nSYMBOL] == '.' && wasPoint == 0))
                      {
                      number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                      if ((str[nLINE])[nSYMBOL] == '.') wasPoint = 1;
                      nSYMBOL++;
                      COUNTERofDIGITS++;
                      }
                    number[COUNTERofDIGITS] = '\0';
                    fprintf(assm, "%lg\n", atof(number));
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;})
DEF_CMD_(JNE,   32,{fprintf(assm, "%d ", NumOfCmd);
                    cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;
                    assert((str[nLINE])[nSYMBOL] == ' ' && (str[nLINE])[nSYMBOL+1] == ':'
                            && isdigit((str[nLINE])[nSYMBOL+2]) != 0);
                    number[0] = (str[nLINE])[nSYMBOL+2];
                    number[1] = '\0';
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;
                    nSYMBOL = nSYMBOL + 3;
                    assert((str[nLINE])[nSYMBOL] == ' ');
                    nSYMBOL++;
                    assert(isdigit((str[nLINE])[nSYMBOL]) != 0 || (str[nLINE])[nSYMBOL] == '-'
                           || (str[nLINE])[nSYMBOL] == '+');
                    COUNTERofDIGITS = 0;
                    number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                    nSYMBOL++;
                    COUNTERofDIGITS++;
                    wasPoint = 0;
                    while (isdigit((str[nLINE])[nSYMBOL]) != 0 ||
                          ((str[nLINE])[nSYMBOL] == '.' && wasPoint == 0))
                      {
                      number[COUNTERofDIGITS] = (str[nLINE])[nSYMBOL];
                      if ((str[nLINE])[nSYMBOL] == '.') wasPoint = 1;
                      nSYMBOL++;
                      COUNTERofDIGITS++;
                      }
                    number[COUNTERofDIGITS] = '\0';
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;})
DEF_CMD_(PUSHR, 33, {cmds[NumOfCellIncmds] = NumOfCmd;
                    assert(isalpha((str[nLINE])[nSYMBOL+1]) != 0 && isalpha((str[nLINE])[nSYMBOL+2]) != 0);
                    NameOfReg[0] = (str[nLINE])[nSYMBOL+1];
                    NameOfReg[1] = (str[nLINE])[nSYMBOL+2];
                    NameOfReg[2] = '\0';
                    if (strcmp(NameOfReg, "ax\0") == 0) NumberOfReg = 1;
                    if (strcmp(NameOfReg, "bx\0") == 0) NumberOfReg = 2;
                    if (strcmp(NameOfReg, "cx\0") == 0) NumberOfReg = 3;
                    if (strcmp(NameOfReg, "dx\0") == 0) NumberOfReg = 4;
                    assert(0 < NumberOfReg && NumberOfReg < 5);
                    cmds[NumOfCellIncmds+1] = NumberOfReg;
                    NumOfCellIncmds = NumOfCellIncmds + 2;})
DEF_CMD_(SPACE, 34, {cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(CALL,  35, {cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;
                    assert((str[nLINE])[nSYMBOL] == ' ' && (str[nLINE])[nSYMBOL+1] == ':'
                            && isdigit((str[nLINE])[nSYMBOL+2]) != 0);
                    number[0] = (str[nLINE])[nSYMBOL+2];
                    number[1] = '\0';
                    cmds[NumOfCellIncmds] = atof(number);
                    NumOfCellIncmds++;})
DEF_CMD_(RET,   36, {cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(CLEAR, 37, {cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
DEF_CMD_(DUP, 38, {cmds[NumOfCellIncmds] = NumOfCmd; NumOfCellIncmds++;})
#endif

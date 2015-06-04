int CounterForDISASM = POISON;
int j = 0;
FILE *disasm = fopen("DISASM.TXT", "w");

assert(j == 0 && disasm != NULL);
while (j < NumOfCellIncmds)
  {
  if (0 <= (int) cmds[j] && (int) cmds[j] <= 9)
    {
    for (CounterForDISASM = 0; CounterForDISASM <= 9; CounterForDISASM++)
      {
      if ((int) cmds[j] == CounterForDISASM) fprintf(disasm, ":%d\n", CounterForDISASM);
      }
    j++;
    }
  else
    {
    #define DEF_CMD_(name, num, code) case num: code; break;
    switch ((int) cmds[j])
    {
    #include "ComandsDISASM.h"
    default: printf("WRONG ASSEMBLER");
             assert(0);
             break;
    }
    #undef DEF_CMD_
    }
  }

fclose(disasm);

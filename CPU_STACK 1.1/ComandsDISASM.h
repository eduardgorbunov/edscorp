DEF_CMD_(PUSH,  10, fprintf(disasm, "PUSH %lg\n", cmds[j+1]); j = j + 2;)
DEF_CMD_(ADD,   11, fprintf(disasm, "ADD\n"); j++)
DEF_CMD_(MUL,   12, fprintf(disasm, "MUL\n"); j++)
DEF_CMD_(POPR,  13, fprintf(disasm, "POPR ");
                   switch ((int) cmds[j+1])
                   {
                   case 1:  fprintf(disasm, "ax\n");
                            break;
                   case 2:  fprintf(disasm, "bx\n");
                            break;
                   case 3:  fprintf(disasm, "cx\n");
                            break;
                   case 4:  fprintf(disasm, "dx\n");
                            break;
                   default: fprintf(disasm, "BAD ASSEMBLER");
                            break;
                   }
                   j = j + 2;)
DEF_CMD_(OUTR,  14, fprintf(disasm, "OUTR ");
                   switch ((int) cmds[j+1])
                   {
                   case 1:  fprintf(disasm, "ax\n");
                            break;
                   case 2:  fprintf(disasm, "bx\n");
                            break;
                   case 3:  fprintf(disasm, "cx\n");
                            break;
                   case 4:  fprintf(disasm, "dx\n");
                            break;
                   default: fprintf(disasm, "BAD ASSEMBLER");
                            break;
                   }
                   j = j + 2;)
DEF_CMD_(END,   15, fprintf(disasm, "END\n"); j++;)
DEF_CMD_(IN,    16, fprintf(disasm, "IN ");
                   switch ((int) cmds[j+1])
                   {
                   case 1:  fprintf(disasm, "ax\n");
                            break;
                   case 2:  fprintf(disasm, "bx\n");
                            break;
                   case 3:  fprintf(disasm, "cx\n");
                            break;
                   case 4:  fprintf(disasm, "dx\n");
                            break;
                   default: fprintf(disasm, "BAD ASSEMBLER");
                            break;
                   }
                   j = j + 2;)
DEF_CMD_(INFO,  17, fprintf(disasm, "INFO\n"); j++;)
DEF_CMD_(OUT,   18, fprintf(disasm, "OUT\n"); j++;)
DEF_CMD_(SQRT,  19, fprintf(disasm, "SQRT\n"); j++;)
DEF_CMD_(DIV,   20, fprintf(disasm, "DIV\n"); j++;)
DEF_CMD_(SIN,   21, fprintf(disasm, "SIN\n"); j++;)
DEF_CMD_(COS,   22, fprintf(disasm, "COS\n"); j++;)
DEF_CMD_(TAN,   23, fprintf(disasm, "TAN\n"); j++;)
DEF_CMD_(POW,   24, fprintf(disasm, "POW %lg\n", cmds[j+1]); j = j + 2;)
DEF_CMD_(SWAP,  25, fprintf(disasm, "SWAP\n"); j++;)
DEF_CMD_(JMP,   26, fprintf(disasm, "JMP :%lg\n", cmds[j+1]); j = j + 2;)
DEF_CMD_(JA,    27, fprintf(disasm, "JA :%lg %lg\n", cmds[j+1], cmds[j+2]); j = j + 3;)
DEF_CMD_(JAE,   28, fprintf(disasm, "JAE :%lg %lg\n", cmds[j+1], cmds[j+2]); j = j + 3;)
DEF_CMD_(JB,    29, fprintf(disasm, "JB :%lg %lg\n", cmds[j+1], cmds[j+2]); j = j + 3;)
DEF_CMD_(JBE,   30, fprintf(disasm, "JBE :%lg %lg\n", cmds[j+1], cmds[j+2]); j = j + 3;)
DEF_CMD_(JE,    31, fprintf(disasm, "JE :%lg %lg\n", cmds[j+1], cmds[j+2]); j = j + 3;)
DEF_CMD_(JNE,   32, fprintf(disasm, "JNE :%lg %lg\n", cmds[j+1], cmds[j+2]); j = j + 3;)
DEF_CMD_(PUSHR, 33, fprintf(disasm, "PUSHR ");
                    switch ((int) cmds[j+1])
                    {
                    case 1:  fprintf(disasm, "ax\n");
                             break;
                    case 2:  fprintf(disasm, "bx\n");
                             break;
                    case 3:  fprintf(disasm, "cx\n");
                             break;
                    case 4:  fprintf(disasm, "dx\n");
                             break;
                    default: fprintf(disasm, "BAD ASSEMBLER");
                             break;
                    }
                    j = j + 2;)
DEF_CMD_(SPACE, 34, fprintf(disasm, "SPACE\n"); j++;)
DEF_CMD_(CALL,  35, fprintf(disasm, "CALL :%lg\n", cmds[j+1]); j = j + 2;)
DEF_CMD_(RET,   36, fprintf(disasm, "RET\n"); j++;)
DEF_CMD_(CLEAR, 37, fprintf(disasm, "CLEAR\n"); j++;)
DEF_CMD_(DUP,   38, fprintf(disasm, "DUP\n"); j++;)
DEF_CMD_(LOG,   39, fprintf(disasm, "LOG\n"); j++;)

DEF_CMD_(PUSH,  10, Stack_push(&(cp.Stack), cmds[i+1]); i = i + 2;)
DEF_CMD_(ADD,   11, CPU_add(&cp); i++)
DEF_CMD_(MUL,   12, CPU_mul(&cp); i++)
DEF_CMD_(POPR,  13, CPU_popr(&cp, (int) cmds[i+1]); i = i + 2;)
DEF_CMD_(OUTR,  14, CPU_outr(&cp, (int) cmds[i+1]); i = i + 2;)
DEF_CMD_(END,   15, i = NumOfCellIncmds;)
DEF_CMD_(IN,    16, CPU_in(&cp, (int) cmds[i+1]); i = i + 2;)
DEF_CMD_(INFO,  17, CPU_info(&cp); i++;)
DEF_CMD_(OUT,   18, Stack_out(&(cp.Stack)); i++;)
DEF_CMD_(SQRT,  19, (cp.Stack).body[(cp.Stack).ntop] = sqrt((cp.Stack).body[(cp.Stack).ntop]); i++;)
DEF_CMD_(DIV,   20, CPU_div(&cp); i++;)
DEF_CMD_(SIN,   21, (cp.Stack).body[(cp.Stack).ntop] = sin((cp.Stack).body[(cp.Stack).ntop]); i++;)
DEF_CMD_(COS,   22, (cp.Stack).body[(cp.Stack).ntop] = cos((cp.Stack).body[(cp.Stack).ntop]); i++;)
DEF_CMD_(TAN,   23, (cp.Stack).body[(cp.Stack).ntop] = tan((cp.Stack).body[(cp.Stack).ntop]); i++;)
DEF_CMD_(POW,   24, CPU_pow(&cp, cmds[i+1]); i = i + 2;)
DEF_CMD_(SWAP,  25, Stack_swap(&(cp.Stack)); i++;)
DEF_CMD_(JMP,   26, i = labels[(int) cmds[i+1]];)
DEF_CMD_(JA,    27, Stack_pop(&(cp.Stack), &value);
                    if (value > cmds[i+2]) i = labels[(int) cmds[i+1]];
                    else i = i + 3;)
DEF_CMD_(JAE,   28, Stack_pop(&(cp.Stack), &value);
                    Stack_push(&(cp.Stack), value);
                    if (value >= cmds[i+2]) i = labels[(int) cmds[i+1]];
                    else i = i + 3;)
DEF_CMD_(JB,    29, Stack_pop(&(cp.Stack), &value);
                    Stack_push(&(cp.Stack), value);
                    if (value < cmds[i+2]) i = labels[(int) cmds[i+1]];
                    else i = i + 3;)
DEF_CMD_(JBE,   30, Stack_pop(&(cp.Stack), &value);
                    Stack_push(&(cp.Stack), value);
                    if (value <= cmds[i+2]) i = labels[(int) cmds[i+1]];
                    else i = i + 3;)
DEF_CMD_(JE,    31, Stack_pop(&(cp.Stack), &value);
                    Stack_push(&(cp.Stack), value);
                    if (value == cmds[i+2]) i = labels[(int) cmds[i+1]];
                    else i = i + 3;)
DEF_CMD_(JNE,   32, Stack_pop(&(cp.Stack), &value);
                    Stack_push(&(cp.Stack), value);
                    if (value != cmds[i+2]) i = labels[(int) cmds[i+1]];
                    else i = i + 3;)
DEF_CMD_(PUSHR, 33, CPU_pushr(&cp, (int) cmds[i+1]); i = i + 2;)
DEF_CMD_(SPACE, 34, printf("\n"); i++;)
DEF_CMD_(CALL,  35, Stack_push(&(CallStack), i + 2);
                    i = labels[(int) cmds[i+1]];)
DEF_CMD_(RET,   36, Stack_pop(&(CallStack), &value);
                    i = (int) value;)
DEF_CMD_(CLEAR, 37, Stack_clear(&(cp.Stack)); i++;)
DEF_CMD_(DUP,   38, Stack_dup(&(cp.Stack)); i++;)
DEF_CMD_(LOG,   39, (cp.Stack).body[(cp.Stack).ntop] = log((cp.Stack).body[(cp.Stack).ntop]); i++;)
DEF_CMD_(JAS,   40, Stack_pop(&(cp.Stack), &value);
                    Stack_pop(&(cp.Stack), &secValue);
                    Stack_push(&(cp.Stack), secValue);
                    Stack_push(&(cp.Stack), value);
                    if (secValue > value) i = labels[(int) cmds[i+1]];
                    else i = i + 2;)
DEF_CMD_(JAES,  41, Stack_pop(&(cp.Stack), &value);
                    Stack_pop(&(cp.Stack), &secValue);
                    Stack_push(&(cp.Stack), secValue);
                    Stack_push(&(cp.Stack), value);
                    if (secValue >= value) i = labels[(int) cmds[i+1]];
                    else i = i + 2;)
DEF_CMD_(JBS,   42, Stack_pop(&(cp.Stack), &value);
                    Stack_pop(&(cp.Stack), &secValue);
                    Stack_push(&(cp.Stack), secValue);
                    Stack_push(&(cp.Stack), value);
                    if (secValue < value) i = labels[(int) cmds[i+1]];
                    else i = i + 2;)
DEF_CMD_(JBES,  43, Stack_pop(&(cp.Stack), &value);
                    Stack_pop(&(cp.Stack), &secValue);
                    Stack_push(&(cp.Stack), secValue);
                    Stack_push(&(cp.Stack), value);
                    if (secValue <= value) i = labels[(int) cmds[i+1]];
                    else i = i + 2;)
DEF_CMD_(JES,   44, Stack_pop(&(cp.Stack), &value);
                    Stack_pop(&(cp.Stack), &secValue);
                    Stack_push(&(cp.Stack), secValue);
                    Stack_push(&(cp.Stack), value);
                    if (secValue == value) i = labels[(int) cmds[i+1]];
                    else i = i + 2;)
DEF_CMD_(JNES,  45, Stack_pop(&(cp.Stack), &value);
                    Stack_pop(&(cp.Stack), &secValue);
                    Stack_push(&(cp.Stack), secValue);
                    Stack_push(&(cp.Stack), value);
                    if (secValue != value) i = labels[(int) cmds[i+1]];
                    else i = i + 2;)

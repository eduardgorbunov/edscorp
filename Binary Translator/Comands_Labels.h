DEF_CMD_(PUSH,  64, {
                        count_comands += 2;
                        count_exec += 6;
                        status = 0;
                    })
DEF_CMD_(ADD,   65, {
                        count_exec += 5;
                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(MUL,   66, {
                        count_exec += 9;
                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(POPR,  67, {
                        count_comands += 2;
                        ++count_exec;
                        status = 0;
                    })
DEF_CMD_(OUTR,  68, {
                        count_comands += 2;
                        count_exec += 9;
                        status = 0;
                    })
DEF_CMD_(END,   69, {
                        ++count_comands;
                        ++count_exec;
                        status = 0;
                    })
DEF_CMD_(IN,    70, {
                        count_comands += 2;
                        count_exec += 9;
                        status = 0;
                    })
DEF_CMD_(OUT,   71, {
                        ++count_comands;
                        count_exec += 7;
                        status = 0;
                    })
DEF_CMD_(DIV,   72, {
                        ++count_comands;
                        count_exec += 9;
                        status = 0;
                    })
DEF_CMD_(SWAP,  73, {
                        ++count_comands;
                        count_exec += 4;
                        status = 0;
                    })
DEF_CMD_(JMP,   74, {
                        ++count_exec;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(JA,    75, {
                        count_exec += 11;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JAE,   76, {
                        count_exec += 11;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JB,    77, {
                        count_exec += 11;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JBE,   78, {
                        count_exec += 11;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JE,    79, {
                        count_exec += 11;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JNE,   80, {
                        count_exec += 11;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(PUSHR, 81, {
                        count_comands += 2;
                        ++count_exec;
                        status = 0;
                    })
DEF_CMD_(DUP,   82, {
                        ++count_comands;
                        count_exec += 4;
                        status = 0;
                    })
DEF_CMD_(JAS,   83, {
                        count_exec += 8;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(JAES,  84, {
                        count_exec += 8;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(JBS,   85, {
                        count_exec += 8;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(JBES,  86, {
                        count_exec += 8;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(JES,   87, {
                        count_exec += 8;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(JNES,  88, {
                        count_exec += 8;
                        ++count_comands;
                        int adress =  buf_labels[buf_comands[count_comands]].adr_exec;
                        adress = adress - (count_exec+4);
                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        adress >>= 8;
                        ++count_exec;

                        assert((*buf_exec)[count_exec] == 0);
                        (*buf_exec)[count_exec] = adress % 256;
                        ++count_exec;

                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(SUB,   89, {
                        count_exec += 5;
                        ++count_comands;
                        status = 0;
                    })

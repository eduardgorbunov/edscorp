DEF_CMD_(PUSH,  64, {
                        assert(count_exec < BUF_EXEC_LENGTH-5);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0xb8;                        //mov eax, number<---|
                        int number = buf_comands[count_comands+1];             //-------------------|
                        (*buf_exec)[count_exec+1] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+2] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+3] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+4] = number % 256;

                        (*buf_exec)[count_exec+5] = 0x50;                      //push eax

                        count_exec += 6;
                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(ADD,   65, {
                        assert(count_exec < BUF_EXEC_LENGTH-4);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec]   = 0x58;                      //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                      //pop ebx

                        (*buf_exec)[count_exec+2] = 0x01;                      //add eax, ebx
                        (*buf_exec)[count_exec+3] = 0xd8;

                        (*buf_exec)[count_exec+4] = 0x50;                      //push eax

                        count_exec += 5;
                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(MUL,   66, {
                        assert(count_exec < BUF_EXEC_LENGTH-8);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec]   = 0x58;                      //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                      //pop ebx

                        (*buf_exec)[count_exec+2] = 0x52;                      //push edx (save register edx)

                        (*buf_exec)[count_exec+3] = 0x31;                      //xor edx, edx
                        (*buf_exec)[count_exec+4] = 0xd2;

                        (*buf_exec)[count_exec+5] = 0xf7;                      //mul ebx
                        (*buf_exec)[count_exec+6] = 0xe3;

                        (*buf_exec)[count_exec+7] = 0x5a;                      //pop edx

                        (*buf_exec)[count_exec+8] = 0x50;                      //push eax

                        count_exec += 9;
                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(POPR,  67, {
                        assert(count_exec < BUF_EXEC_LENGTH);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        ++count_comands;
                        switch(buf_comands[count_comands])
                        {
                            case 3:  (*buf_exec)[count_exec] = 0x59;               //pop ecx
                                     break;
                            case 4:  (*buf_exec)[count_exec] = 0x5a;               //pop edx
                                     break;
                            default: printf("WRONG REGISTER IN BUF_COMANDS\n");
                                     assert(0);
                                     break;
                        }

                        ++count_comands;
                        ++count_exec;
                        status = 0;
                    })
DEF_CMD_(OUTR,  68, {
                        assert(count_exec < BUF_EXEC_LENGTH-8);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        ++count_comands;
                        switch(buf_comands[count_comands])
                        {
                            case 3:  (*buf_exec)[count_exec] = 0x51;               //push ecx

                                     (*buf_exec)[count_exec+1] = 0xbb;             //mov ebx, my_out
                                     (*buf_exec)[count_exec+2] = outInt1;
                                     (*buf_exec)[count_exec+3] = outInt2;
                                     (*buf_exec)[count_exec+4] = outInt3;
                                     (*buf_exec)[count_exec+5] = outInt4;

                                     (*buf_exec)[count_exec+6] = 0xff;             //call ebx
                                     (*buf_exec)[count_exec+7] = 0xd3;

                                     (*buf_exec)[count_exec+8] = 0x59;             //pop ecx
                                     break;
                            case 4:  (*buf_exec)[count_exec] = 0x52;               //push edx

                                     (*buf_exec)[count_exec+1] = 0xbb;             //mov ebx, my_out
                                     (*buf_exec)[count_exec+2] = outInt1;
                                     (*buf_exec)[count_exec+3] = outInt2;
                                     (*buf_exec)[count_exec+4] = outInt3;
                                     (*buf_exec)[count_exec+5] = outInt4;

                                     (*buf_exec)[count_exec+6] = 0xff;             //call ebx
                                     (*buf_exec)[count_exec+7] = 0xd3;

                                     (*buf_exec)[count_exec+8] = 0x5a;             //pop edx
                                     break;
                        }

                        ++count_comands;
                        count_exec += 9;
                        status = 0;
                    })
DEF_CMD_(END,   69, {
                        assert(count_exec < BUF_EXEC_LENGTH);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0xc3;                            //ret
                        ++count_comands;
                        ++count_exec;
                        status = 0;
                    })
DEF_CMD_(IN,    70, {
                        assert(count_exec < BUF_EXEC_LENGTH-8);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec]   = 0xbb;                          //mov ebx, my_in
                        (*buf_exec)[count_exec+1] = inpInt1;
                        (*buf_exec)[count_exec+2] = inpInt2;
                        (*buf_exec)[count_exec+3] = inpInt3;
                        (*buf_exec)[count_exec+4] = inpInt4;

                        (*buf_exec)[count_exec+5] = 0xff;                           //call ebx
                        (*buf_exec)[count_exec+6] = 0xd3;

                        ++count_comands;
                        switch(buf_comands[count_comands])
                        {
                            case 3:  (*buf_exec)[count_exec+7]  = 0x89;             //mov ecx, eax
                                     (*buf_exec)[count_exec+8]  = 0xc1;
                                     break;
                            case 4:  (*buf_exec)[count_exec+7]  = 0x89;             //mov edx, eax
                                     (*buf_exec)[count_exec+8] = 0xc2;
                                     break;
                            default: printf("WRONG REGISTER: IN\n");
                                     assert(0);
                                     break;
                        }
                        count_exec += 9;
                        ++count_comands;
                        status = 0;
                    })
DEF_CMD_(OUT,   71, {
                        assert(count_exec < BUF_EXEC_LENGTH-6);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec]   = 0xbb;                          //mov ebx, my_out
                        (*buf_exec)[count_exec+1] = outInt1;
                        (*buf_exec)[count_exec+2] = outInt2;
                        (*buf_exec)[count_exec+3] = outInt3;
                        (*buf_exec)[count_exec+4] = outInt4;

                        (*buf_exec)[count_exec+5] = 0xff;                           //call ebx
                        (*buf_exec)[count_exec+6] = 0xd3;

                        ++count_comands;
                        count_exec += 7;
                        status = 0;
                    })
DEF_CMD_(DIV,   72, {
                        assert(count_exec < BUF_COMANDS_LENGTH-8);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x58;                            //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                          //pop ebx

                        (*buf_exec)[count_exec+2] = 0x52;                          //push edx (save register edx)

                        (*buf_exec)[count_exec+3] = 0x31;                          //xor edx, edx
                        (*buf_exec)[count_exec+4] = 0xd2;

                        (*buf_exec)[count_exec+5] = 0xf7;                          //div ebx
                        (*buf_exec)[count_exec+6] = 0xf3;

                        (*buf_exec)[count_exec+7] = 0x5a;                          //pop edx

                        (*buf_exec)[count_exec+8] = 0x50;                          //push eax

                        ++count_comands;
                        count_exec += 9;
                        status = 0;
                    })
DEF_CMD_(SWAP,  73, {
                        assert(count_exec < BUF_EXEC_LENGTH-3);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x58;                            //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                          //pop ebx

                        (*buf_exec)[count_exec+2] = 0x50;                          //push eax

                        (*buf_exec)[count_exec+3] = 0x53;                          //push ebx

                        ++count_comands;
                        count_exec += 4;
                        status = 0;
                    })
DEF_CMD_(JMP,   74, {
                        assert(count_exec < BUF_EXEC_LENGTH-4);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0xe9;                                     //jmpq

                        (*buf_exec)[count_exec+1] = 0;
                        (*buf_exec)[count_exec+2] = 0;
                        (*buf_exec)[count_exec+3] = 0;
                        (*buf_exec)[count_exec+4] = 0;

                        count_exec += 5;
                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JA,    75, {
                        assert(count_exec < BUF_EXEC_LENGTH-14);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        int number = buf_comands[count_comands+2];
                        (*buf_exec)[count_exec] = 0xbb;                                     //mov ebx, number
                        (*buf_exec)[count_exec+1] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+2] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+3] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+4] = number % 256;

                        (*buf_exec)[count_exec+5] = 0x58;                                   //pop eax

                        (*buf_exec)[count_exec+6] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+7] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+8] = 0xd8;

                        (*buf_exec)[count_exec+9] = 0x0f;                                   //ja ...
                        (*buf_exec)[count_exec+10] = 0x87;

                        (*buf_exec)[count_exec+11] = 0;
                        (*buf_exec)[count_exec+12] = 0;
                        (*buf_exec)[count_exec+13] = 0;
                        (*buf_exec)[count_exec+14] = 0;

                        count_exec += 15;
                        count_comands += 3;
                        status = 0;
                    })
DEF_CMD_(JAE,   76, {
                        assert(count_exec < BUF_EXEC_LENGTH-14);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        int number = buf_comands[count_comands+2];
                        (*buf_exec)[count_exec] = 0xbb;                                     //mov ebx, number
                        (*buf_exec)[count_exec+1] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+2] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+3] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+4] = number % 256;

                        (*buf_exec)[count_exec+5] = 0x58;                                   //pop eax

                        (*buf_exec)[count_exec+6] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+7] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+8] = 0xd8;

                        (*buf_exec)[count_exec+9] = 0x0f;                                   //jae ...
                        (*buf_exec)[count_exec+10] = 0x83;

                        (*buf_exec)[count_exec+11] = 0;
                        (*buf_exec)[count_exec+12] = 0;
                        (*buf_exec)[count_exec+13] = 0;
                        (*buf_exec)[count_exec+14] = 0;

                        count_exec += 15;
                        count_comands += 3;
                        status = 0;
                    })
DEF_CMD_(JB,    77, {
                        assert(count_exec < BUF_EXEC_LENGTH-14);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        int number = buf_comands[count_comands+2];
                        (*buf_exec)[count_exec] = 0xbb;                                     //mov ebx, number
                        (*buf_exec)[count_exec+1] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+2] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+3] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+4] = number % 256;

                        (*buf_exec)[count_exec+5] = 0x58;                                   //pop eax

                        (*buf_exec)[count_exec+6] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+7] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+8] = 0xd8;

                        (*buf_exec)[count_exec+9] = 0x0f;                                   //jb ...
                        (*buf_exec)[count_exec+10] = 0x82;

                        (*buf_exec)[count_exec+11] = 0;
                        (*buf_exec)[count_exec+12] = 0;
                        (*buf_exec)[count_exec+13] = 0;
                        (*buf_exec)[count_exec+14] = 0;

                        count_exec += 15;
                        count_comands += 3;
                        status = 0;
                    })
DEF_CMD_(JBE,   78, {
                        assert(count_exec < BUF_EXEC_LENGTH-14);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        int number = buf_comands[count_comands+2];
                        (*buf_exec)[count_exec] = 0xbb;                                     //mov ebx, number
                        (*buf_exec)[count_exec+1] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+2] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+3] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+4] = number % 256;

                        (*buf_exec)[count_exec+5] = 0x58;                                   //pop eax

                        (*buf_exec)[count_exec+6] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+7] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+8] = 0xd8;

                        (*buf_exec)[count_exec+9] = 0x0f;                                   //jbe ...
                        (*buf_exec)[count_exec+10] = 0x86;

                        (*buf_exec)[count_exec+11] = 0;
                        (*buf_exec)[count_exec+12] = 0;
                        (*buf_exec)[count_exec+13] = 0;
                        (*buf_exec)[count_exec+14] = 0;

                        count_exec += 15;
                        count_comands += 3;
                        status = 0;
                    })
DEF_CMD_(JE,    79, {
                        assert(count_exec < BUF_EXEC_LENGTH-14);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        int number = buf_comands[count_comands+2];
                        (*buf_exec)[count_exec] = 0xbb;                                     //mov ebx, number
                        (*buf_exec)[count_exec+1] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+2] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+3] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+4] = number % 256;

                        (*buf_exec)[count_exec+5] = 0x58;                                   //pop eax

                        (*buf_exec)[count_exec+6] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+7] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+8] = 0xd8;

                        (*buf_exec)[count_exec+9] = 0x0f;                                   //je ...
                        (*buf_exec)[count_exec+10] = 0x84;

                        (*buf_exec)[count_exec+11] = 0;
                        (*buf_exec)[count_exec+12] = 0;
                        (*buf_exec)[count_exec+13] = 0;
                        (*buf_exec)[count_exec+14] = 0;

                        count_exec += 15;
                        count_comands += 3;
                        status = 0;
                    })
DEF_CMD_(JNE,   80, {
                        assert(count_exec < BUF_EXEC_LENGTH-14);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        int number = buf_comands[count_comands+2];
                        (*buf_exec)[count_exec] = 0xbb;                                     //mov ebx, number
                        (*buf_exec)[count_exec+1] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+2] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+3] = number % 256;
                        number >>= 8;
                        (*buf_exec)[count_exec+4] = number % 256;

                        (*buf_exec)[count_exec+5] = 0x58;                                   //pop eax

                        (*buf_exec)[count_exec+6] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+7] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+8] = 0xd8;

                        (*buf_exec)[count_exec+9] = 0x0f;                                   //jne ...
                        (*buf_exec)[count_exec+10] = 0x85;

                        (*buf_exec)[count_exec+11] = 0;
                        (*buf_exec)[count_exec+12] = 0;
                        (*buf_exec)[count_exec+13] = 0;
                        (*buf_exec)[count_exec+14] = 0;

                        count_exec += 15;
                        count_comands += 3;
                        status = 0;
                    })
DEF_CMD_(PUSHR, 81, {
                        assert(count_exec < BUF_EXEC_LENGTH);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        ++count_comands;
                        switch(buf_comands[count_comands])
                        {
                            case 3:  (*buf_exec)[count_exec] = 0x51;               //push ecx
                                     break;
                            case 4:  (*buf_exec)[count_exec] = 0x52;               //push edx
                                     break;
                            default: printf("WRONG REGISTER IN BUF_COMANDS\n");
                                     assert(0);
                                     break;
                        }

                        ++count_comands;
                        ++count_exec;
                        status = 0;
                    })
DEF_CMD_(DUP,   82, {
                        assert(count_exec < BUF_EXEC_LENGTH-4);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x8b;                            //mov eax, [esp]
                        (*buf_exec)[count_exec+1] = 4;
                        (*buf_exec)[count_exec+2] = 0x24;

                        (*buf_exec)[count_exec+3] = 0x50;                          //push eax

                        ++count_comands;
                        count_exec += 4;
                        status = 0;
                    })
DEF_CMD_(JAS,   83, {
                        assert(count_exec < BUF_EXEC_LENGTH-11);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x58;                                     //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                                   //pop ebx

                        (*buf_exec)[count_exec+2] = 0x53;                                   //push ebx

                        (*buf_exec)[count_exec+3] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+4] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+5] = 0xd8;

                        (*buf_exec)[count_exec+6] = 0x0f;                                   // ja ...
                        (*buf_exec)[count_exec+7] = 0x87;

                        (*buf_exec)[count_exec+8] = 0;
                        (*buf_exec)[count_exec+9] = 0;
                        (*buf_exec)[count_exec+10] = 0;
                        (*buf_exec)[count_exec+11] = 0;

                        count_exec += 12;
                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JAES,  84, {
                        assert(count_exec < BUF_EXEC_LENGTH-11);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x58;                                     //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                                   //pop ebx

                        (*buf_exec)[count_exec+2] = 0x53;                                   //push ebx

                        (*buf_exec)[count_exec+3] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+4] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+5] = 0xd8;

                        (*buf_exec)[count_exec+6] = 0x0f;                                   //jae ...
                        (*buf_exec)[count_exec+7] = 0x83;

                        (*buf_exec)[count_exec+8] = 0;
                        (*buf_exec)[count_exec+9] = 0;
                        (*buf_exec)[count_exec+10] = 0;
                        (*buf_exec)[count_exec+11] = 0;

                        count_exec += 12;
                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JBS,   85, {
                        assert(count_exec < BUF_EXEC_LENGTH-11);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x58;                                     //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                                   //pop ebx

                        (*buf_exec)[count_exec+2] = 0x53;                                   //push ebx

                        (*buf_exec)[count_exec+3] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+4] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+5] = 0xd8;

                        (*buf_exec)[count_exec+6] = 0x0f;                                   //jb ...
                        (*buf_exec)[count_exec+7] = 0x82;

                        (*buf_exec)[count_exec+8] = 0;
                        (*buf_exec)[count_exec+9] = 0;
                        (*buf_exec)[count_exec+10] = 0;
                        (*buf_exec)[count_exec+11] = 0;

                        count_exec += 12;
                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JBES,  86, {
                        assert(count_exec < BUF_EXEC_LENGTH-11);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x58;                                     //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                                   //pop ebx

                        (*buf_exec)[count_exec+2] = 0x53;                                   //push ebx

                        (*buf_exec)[count_exec+3] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+4] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+5] = 0xd8;

                        (*buf_exec)[count_exec+6] = 0x0f;                                   //jbe ...
                        (*buf_exec)[count_exec+7] = 0x86;

                        (*buf_exec)[count_exec+8] = 0;
                        (*buf_exec)[count_exec+9] = 0;
                        (*buf_exec)[count_exec+10] = 0;
                        (*buf_exec)[count_exec+11] = 0;

                        count_exec += 12;
                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JES,   87, {
                        assert(count_exec < BUF_EXEC_LENGTH-11);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x58;                                     //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                                   //pop ebx

                        (*buf_exec)[count_exec+2] = 0x53;                                   //push ebx

                        (*buf_exec)[count_exec+3] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+4] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+5] = 0xd8;

                        (*buf_exec)[count_exec+6] = 0x0f;                                   //je ...
                        (*buf_exec)[count_exec+7] = 0x84;

                        (*buf_exec)[count_exec+8] = 0;
                        (*buf_exec)[count_exec+9] = 0;
                        (*buf_exec)[count_exec+10] = 0;
                        (*buf_exec)[count_exec+11] = 0;

                        count_exec += 12;
                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(JNES,  88, {
                        assert(count_exec < BUF_EXEC_LENGTH-11);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec] = 0x58;                                     //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                                   //pop ebx

                        (*buf_exec)[count_exec+2] = 0x53;                                   //push ebx

                        (*buf_exec)[count_exec+3] = 0x50;                                   //push eax

                        (*buf_exec)[count_exec+4] = 0x39;                                   //cmp eax, ebx
                        (*buf_exec)[count_exec+5] = 0xd8;

                        (*buf_exec)[count_exec+6] = 0x0f;                                   //jne ...
                        (*buf_exec)[count_exec+7] = 0x85;

                        (*buf_exec)[count_exec+8] = 0;
                        (*buf_exec)[count_exec+9] = 0;
                        (*buf_exec)[count_exec+10] = 0;
                        (*buf_exec)[count_exec+11] = 0;

                        count_exec += 12;
                        count_comands += 2;
                        status = 0;
                    })
DEF_CMD_(SUB,   89, {
                        assert(count_exec < BUF_EXEC_LENGTH-4);

                        int num_of_label = get_num_of_label(count_comands, buf_labels);
                        if (num_of_label != -1)
                        {
                            buf_labels[num_of_label].adr_exec = count_exec;
                        }

                        (*buf_exec)[count_exec]   = 0x58;                      //pop eax

                        (*buf_exec)[count_exec+1] = 0x5b;                      //pop ebx

                        (*buf_exec)[count_exec+2] = 0x29;                      //sub eax, ebx
                        (*buf_exec)[count_exec+3] = 0xd8;

                        (*buf_exec)[count_exec+4] = 0x50;                      //push eax

                        count_exec += 5;
                        ++count_comands;
                        status = 0;
                    })

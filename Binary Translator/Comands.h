DEF_CMD_(PUSH,  64, {
                        (*buf_comands)[count_comands] = 64;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int k = 0;                                                         //counter for string_with_number

                        int sign_bit = 1;
                        if (mass_of_strings[nStr][num_of_symbol] == '-')
                        {
                            sign_bit = -1;
                            ++num_of_symbol;
                        }

                        while (isdigit(mass_of_strings[nStr][num_of_symbol]) != 0)
                        {
                            string_with_number[k] = mass_of_strings[nStr][num_of_symbol];
                            ++k;
                            ++num_of_symbol;
                        }
                        string_with_number[k] = 0;

                        (*buf_comands)[count_comands] = atoi(string_with_number)*sign_bit;
                        ++count_comands;
                    })
DEF_CMD_(ADD,   65, {
                        (*buf_comands)[count_comands] = 65;
                        ++count_comands;
                    })
DEF_CMD_(MUL,   66, {
                        (*buf_comands)[count_comands] = 66;
                        ++count_comands;
                    })
DEF_CMD_(POPR,  67, {
                        (*buf_comands)[count_comands] = 67;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        assert(isalpha(mass_of_strings[nStr][num_of_symbol]) != 0);
                        assert(mass_of_strings[nStr][num_of_symbol+1] == 'x');
                        assert(mass_of_strings[nStr][num_of_symbol] == 'c' || mass_of_strings[nStr][num_of_symbol] == 'd');

                        if (mass_of_strings[nStr][num_of_symbol] == 'c') (*buf_comands)[count_comands] = 3;
                        if (mass_of_strings[nStr][num_of_symbol] == 'd') (*buf_comands)[count_comands] = 4;
                        ++count_comands;
                    })
DEF_CMD_(OUTR,  68, {
                        (*buf_comands)[count_comands] = 68;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        assert(isalpha(mass_of_strings[nStr][num_of_symbol]) != 0);
                        assert(mass_of_strings[nStr][num_of_symbol+1] == 'x');
                        assert(mass_of_strings[nStr][num_of_symbol] == 'c' || mass_of_strings[nStr][num_of_symbol] == 'd');

                        if (mass_of_strings[nStr][num_of_symbol] == 'c') (*buf_comands)[count_comands] = 3;
                        if (mass_of_strings[nStr][num_of_symbol] == 'd') (*buf_comands)[count_comands] = 4;
                        ++count_comands;
                    })
DEF_CMD_(END,   69, {
                        (*buf_comands)[count_comands] = 69;
                        ++count_comands;
                    })
DEF_CMD_(IN,    70, {
                        (*buf_comands)[count_comands] = 70;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        assert(isalpha(mass_of_strings[nStr][num_of_symbol]) != 0);
                        assert(mass_of_strings[nStr][num_of_symbol+1] == 'x');
                        assert(mass_of_strings[nStr][num_of_symbol] == 'c' || mass_of_strings[nStr][num_of_symbol] == 'd');

                        if (mass_of_strings[nStr][num_of_symbol] == 'c') (*buf_comands)[count_comands] = 3;
                        if (mass_of_strings[nStr][num_of_symbol] == 'd') (*buf_comands)[count_comands] = 4;
                        ++count_comands;
                    })
DEF_CMD_(OUT,   71, {
                        (*buf_comands)[count_comands] = 71;
                        ++count_comands;
                    })
DEF_CMD_(DIV,   72, {
                        (*buf_comands)[count_comands] = 72;
                        ++count_comands;
                    })
DEF_CMD_(SWAP,  73, {
                        (*buf_comands)[count_comands] = 73;
                        ++count_comands;
                    })
DEF_CMD_(JMP,   74, {
                        (*buf_comands)[count_comands] = 74;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;
                    })
DEF_CMD_(JA,    75, {
                        (*buf_comands)[count_comands] = 75;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != ' ' && mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;

                        int k = 0;                                                         //counter for string_with_number

                        int sign_bit = 1;
                        if (mass_of_strings[nStr][num_of_symbol] == '-')
                        {
                            sign_bit = -1;
                            ++num_of_symbol;
                        }

                        while (isdigit(mass_of_strings[nStr][num_of_symbol]) != 0)
                        {
                            string_with_number[k] = mass_of_strings[nStr][num_of_symbol];
                            ++k;
                            ++num_of_symbol;
                        }
                        string_with_number[k] = 0;

                        (*buf_comands)[count_comands] = atoi(string_with_number)*sign_bit;
                        ++count_comands;
                    })
DEF_CMD_(JAE,   76, {
                        (*buf_comands)[count_comands] = 76;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != ' ' && mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;

                        int k = 0;                                                         //counter for string_with_number

                        int sign_bit = 1;
                        if (mass_of_strings[nStr][num_of_symbol] == '-')
                        {
                            sign_bit = -1;
                            ++num_of_symbol;
                        }

                        while (isdigit(mass_of_strings[nStr][num_of_symbol]) != 0)
                        {
                            string_with_number[k] = mass_of_strings[nStr][num_of_symbol];
                            ++k;
                            ++num_of_symbol;
                        }
                        string_with_number[k] = 0;

                        (*buf_comands)[count_comands] = atoi(string_with_number)*sign_bit;
                        ++count_comands;
                    })
DEF_CMD_(JB,    77, {
                        (*buf_comands)[count_comands] = 77;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != ' ' && mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;

                        int k = 0;                                                         //counter for string_with_number

                        int sign_bit = 1;
                        if (mass_of_strings[nStr][num_of_symbol] == '-')
                        {
                            sign_bit = -1;
                            ++num_of_symbol;
                        }

                        while (isdigit(mass_of_strings[nStr][num_of_symbol]) != 0)
                        {
                            string_with_number[k] = mass_of_strings[nStr][num_of_symbol];
                            ++k;
                            ++num_of_symbol;
                        }
                        string_with_number[k] = 0;

                        (*buf_comands)[count_comands] = atoi(string_with_number)*sign_bit;
                        ++count_comands;
                    })
DEF_CMD_(JBE,   78, {
                        (*buf_comands)[count_comands] = 78;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != ' ' && mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;

                        int k = 0;                                                         //counter for string_with_number

                        int sign_bit = 1;
                        if (mass_of_strings[nStr][num_of_symbol] == '-')
                        {
                            sign_bit = -1;
                            ++num_of_symbol;
                        }

                        while (isdigit(mass_of_strings[nStr][num_of_symbol]) != 0)
                        {
                            string_with_number[k] = mass_of_strings[nStr][num_of_symbol];
                            ++k;
                            ++num_of_symbol;
                        }
                        string_with_number[k] = 0;

                        (*buf_comands)[count_comands] = atoi(string_with_number)*sign_bit;
                        ++count_comands;
                    })
DEF_CMD_(JE,    79, {
                        (*buf_comands)[count_comands] = 79;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != ' ' && mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;

                        int k = 0;                                                         //counter for string_with_number

                        int sign_bit = 1;
                        if (mass_of_strings[nStr][num_of_symbol] == '-')
                        {
                            sign_bit = -1;
                            ++num_of_symbol;
                        }

                        while (isdigit(mass_of_strings[nStr][num_of_symbol]) != 0)
                        {
                            string_with_number[k] = mass_of_strings[nStr][num_of_symbol];
                            ++k;
                            ++num_of_symbol;
                        }
                        string_with_number[k] = 0;

                        (*buf_comands)[count_comands] = atoi(string_with_number)*sign_bit;
                        ++count_comands;
                    })
DEF_CMD_(JNE,   80, {
                        (*buf_comands)[count_comands] = 80;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != ' ' && mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;

                        int k = 0;                                                         //counter for string_with_number

                        int sign_bit = 1;
                        if (mass_of_strings[nStr][num_of_symbol] == '-')
                        {
                            sign_bit = -1;
                            ++num_of_symbol;
                        }

                        while (isdigit(mass_of_strings[nStr][num_of_symbol]) != 0)
                        {
                            string_with_number[k] = mass_of_strings[nStr][num_of_symbol];
                            ++k;
                            ++num_of_symbol;
                        }
                        string_with_number[k] = 0;

                        (*buf_comands)[count_comands] = atoi(string_with_number)*sign_bit;
                        ++count_comands;
                    })
DEF_CMD_(PUSHR, 81, {
                        (*buf_comands)[count_comands] = 81;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        assert(isalpha(mass_of_strings[nStr][num_of_symbol]) != 0);
                        assert(mass_of_strings[nStr][num_of_symbol+1] == 'x');
                        assert(mass_of_strings[nStr][num_of_symbol] == 'c' || mass_of_strings[nStr][num_of_symbol] == 'd');

                        if (mass_of_strings[nStr][num_of_symbol] == 'c') (*buf_comands)[count_comands] = 3;
                        if (mass_of_strings[nStr][num_of_symbol] == 'd') (*buf_comands)[count_comands] = 4;
                        ++count_comands;
                    })
DEF_CMD_(DUP,   82, {
                        (*buf_comands)[count_comands] = 82;
                        ++count_comands;
                    })
DEF_CMD_(JAS,   83, {
                        (*buf_comands)[count_comands] = 83;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;
                    })
DEF_CMD_(JAES,  84, {
                        (*buf_comands)[count_comands] = 84;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;
                    })
DEF_CMD_(JBS,   85, {
                        (*buf_comands)[count_comands] = 85;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;
                    })
DEF_CMD_(JBES,  86, {
                        (*buf_comands)[count_comands] = 86;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;
                    })
DEF_CMD_(JES,   87, {
                        (*buf_comands)[count_comands] = 87;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;
                    })
DEF_CMD_(JNES,  88, {
                        (*buf_comands)[count_comands] = 88;
                        ++count_comands;

                        assert(mass_of_strings[nStr][num_of_symbol] == ' ');
                        ++num_of_symbol;

                        int temp_counter = 0;
                        while (mass_of_strings[nStr][num_of_symbol] != '\0')
                        {
                            cur_label[temp_counter] = mass_of_strings[nStr][num_of_symbol];
                            ++temp_counter;
                            ++num_of_symbol;
                        }
                        cur_label[temp_counter] = 0;

                        temp_counter = 0;
                        int isLabel = 0;
                        while (temp_counter < number_of_labels && isLabel == 0)
                        {
                            if(strcmp(cur_label, (*buf_labels)[temp_counter].label) == 0)
                            {
                                isLabel = 1;
                            }
                            else
                            {
                                ++temp_counter;
                            }
                        }
                        assert(isLabel == 1);

                        (*buf_comands)[count_comands] = temp_counter;
                        ++count_comands;
                    })
DEF_CMD_(SUB,   89, {
                        (*buf_comands)[count_comands] = 89;
                        ++count_comands;
                    })

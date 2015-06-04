#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "File_reading.h"

#ifdef _POSIX_MAPPED_FILES

void * mmap(void *start, size_t length, int prot , int flags, int fd, off_t offset);

int munmap(void *start, size_t length);

#endif

#define BUF_EXEC_LENGTH 8192
#define BUF_COMANDS_LENGTH 4096
#define AMOUNT_OF_LABELS 64
#define AMOUNT_OF_COMANDS 26
#define COMANDS_LENGTH 6
#define NUMBER_LENGTH 32
#define LABEL_LENGTH 32

struct ed_label_t
{
    char* label;
    int adr_comands;
    int adr_exec;
};

int my_in ();

extern "C" void my_out (int answer);

void dump_buf_exec (char* buf_exec, int length);

void create_massive_of_comands (int** buf_comands, int* len, ed_label_t** buf_labels,
                                       char** mass_of_strings, int amount_of_strings);

void dump_mass_of_strings (char** mass_of_strings, int amount_of_strings);

void dump_buf_comands (int* buf_comands, int buf_comands_len);

void dump_buf_labels (ed_label_t* buf_labels);

void bin_translation (char** buf_exec, int* buf_comands, int buf_comands_len, ed_label_t* buf_labels);

int get_num_of_label (int num_of_comand, ed_label_t* buf_labels);

int main ()
{
    char* buf_exec = (char*) mmap(0, BUF_EXEC_LENGTH*sizeof(char),PROT_EXEC|PROT_READ|PROT_WRITE,    //executable bufer
                                                        MAP_SHARED|MAP_ANON, -1, 0);                      //for program

    FILE* prog_exec = fopen("fibo.txt", "r");                                         //program for translation
    assert(prog_exec != 0);

    char* buf_text = 0;
    int len_text = 0;
    ReadFileInBuffer(&buf_text, &len_text, prog_exec);
    fclose(prog_exec);

    char** mass_of_strings = 0;
    int amount_of_strings = 0;
    CreateArrayOfPointersToStr(&mass_of_strings, &amount_of_strings, buf_text, len_text);
    dump_mass_of_strings (mass_of_strings, amount_of_strings);

    int* buf_comands = 0;                                                               //bufer for comands and arguments
    int buf_comands_len = 0;
    ed_label_t* buf_labels = 0;                                                         //bufer for labels
    create_massive_of_comands(&buf_comands, &buf_comands_len, &buf_labels, mass_of_strings, amount_of_strings);
    dump_buf_comands (buf_comands, buf_comands_len);
    dump_buf_labels (buf_labels);

    bin_translation (&buf_exec, buf_comands, buf_comands_len, buf_labels);

    dump_buf_exec (buf_exec, BUF_EXEC_LENGTH);
    int temp = ((int(*)(void)) buf_exec) ();
    munmap((void*)buf_exec, BUF_EXEC_LENGTH);
    free(buf_text);
    free(mass_of_strings);
    free(buf_comands);

    for(int i = 0; i < AMOUNT_OF_LABELS && buf_labels[i].label != 0; ++i)
    {
        free(buf_labels[i].label);
    }
    free(buf_labels);

    return 0;
}

//!=============================================================================
//! my_in - program, which scanf int-number and return it
//!
//! @return scanfed number
//!=============================================================================

int my_in ()
{
    int i = 0;
    scanf("%d", &i);
    return i;
}

//!=============================================================================
//! my_out - program, which print int-number
//!
//! @param answer - number which should be printed out
//!
//! @return nothing
//!=============================================================================

void my_out (int answer)
{
    printf("%d\n", answer);
}

//!=============================================================================
//! dump_buf_exec
//!
//! @param buf_exec - executable bufer for objdump
//! @param length   - the length of buf_exec
//!
//! @return nothing
//!=============================================================================

void dump_buf_exec (char* buf_exec, int length)
{
    printf("======================================================================\n");
    printf("Buf_exec objdump:\n");
    FILE* binary = fopen("binary.out", "wb");
    fwrite(buf_exec, length, 1, binary);
    fclose(binary);

    int errorCode = system("objdump -D ./binary.out -b binary -M intel -m i386");
    assert(errorCode == 0);
    printf("======================================================================\n");
    printf("\n");
}

//!=============================================================================
//! create_massive_of_comands - program which create massive of comands and
//! arguments for comfortable using in future
//!
//! @param[out] buf_comands       - pointer to massive for comands
//! @param[out] len               - length of buf_comands
//! @param[out] buf_labels        - pointer to bufer of labels
//! @param[out] len_labels        - length of buf_labels
//! @param      mass_of_strings   - massive of strings
//! @param      amount_of_strings - amount of strings
//!
//! @return nothing
//!=============================================================================

void create_massive_of_comands (int** buf_comands, int* len, ed_label_t** buf_labels,
                                       char** mass_of_strings, int amount_of_strings)
{
    assert(mass_of_strings != 0);
    assert(buf_comands != 0);
    assert((*buf_comands) == 0);
    assert(len != 0);
    assert(buf_labels != 0);
    assert((*buf_labels) == 0);

    (*buf_comands) = (int*) calloc (BUF_COMANDS_LENGTH, sizeof(int));                       //it's the massive of comands
    assert((*buf_comands) != 0);

    (*buf_labels) = (ed_label_t*) calloc (AMOUNT_OF_LABELS, sizeof(ed_label_t));
    assert((*buf_labels) != 0);

    char** names_of_comands = (char**) calloc (AMOUNT_OF_COMANDS, sizeof(char*));           //massive for detecting
    assert(names_of_comands != 0);                                                              //comands in string

    char* string_with_number = (char*) calloc (NUMBER_LENGTH, sizeof(char));                //it's a bufer-number for
    assert(string_with_number != 0);                                                                      //Comands.h

    #define DEF_CMD_(name, num, code) {names_of_comands[num - AMOUNT_OF_LABELS] = (char*)\
                                       calloc(COMANDS_LENGTH, sizeof(char));\
                                       assert(names_of_comands[num - AMOUNT_OF_LABELS] != 0); \
                                       strcpy(names_of_comands[num - AMOUNT_OF_LABELS],#name);}
    #include "Comands.h"
    #undef DEF_CMD_

    char* cur_comand = (char*) calloc (COMANDS_LENGTH, sizeof(char));                       //string for getting comands
    int count_comands = 0;                                                                  //counter for (*buf_comands)
    char* cur_label = (char*) calloc (LABEL_LENGTH, sizeof(char));
    int count_labels = 0;

    for(int nStr = 0; nStr < amount_of_strings; ++nStr)
    {
        int num_of_symbol = 0;

        if (mass_of_strings[nStr][num_of_symbol] == ':')
        {
            ++num_of_symbol;

            while(mass_of_strings[nStr][num_of_symbol] != '\0')
            {
                assert(0 < num_of_symbol && num_of_symbol < LABEL_LENGTH);
                cur_label[num_of_symbol-1] = mass_of_strings[nStr][num_of_symbol];
                ++num_of_symbol;
            }
            cur_label[num_of_symbol-1] = 0;

            (*buf_labels)[count_labels].label = (char*) calloc(LABEL_LENGTH, sizeof(char));
            strcpy((*buf_labels)[count_labels].label, cur_label);
            ++count_labels;
        }
    }

    int number_of_labels = count_labels;
    count_labels = 0;
    for (int nStr = 0; nStr < amount_of_strings; ++nStr)
    {
        int num_of_symbol = 0;                                                              //number of current symbol
                                                                                            //in current string

        if (mass_of_strings[nStr][num_of_symbol] != ':')
        {
            while (mass_of_strings[nStr][num_of_symbol] != ' ' && mass_of_strings[nStr][num_of_symbol] != 0
                   && num_of_symbol < COMANDS_LENGTH-1 && mass_of_strings[nStr][num_of_symbol] != ';'
                   && mass_of_strings[nStr][num_of_symbol] != ' ')
            {
                char cur_symbol = mass_of_strings[nStr][num_of_symbol];
                assert(-1 < num_of_symbol && num_of_symbol < COMANDS_LENGTH-1);
                assert(isalpha(cur_symbol) != 0);

                cur_comand[num_of_symbol] = cur_symbol;
                ++num_of_symbol;
            }
            cur_comand[num_of_symbol] = 0;

            int isComand = 0, comand_num;
            for (comand_num = 0; comand_num < AMOUNT_OF_COMANDS && isComand == 0; ++comand_num)
            {
                if (strcmp(cur_comand, names_of_comands[comand_num]) == 0)
                    {
                    isComand = 1;
                    }
            }
            --comand_num;
            assert(isComand == 1);

            comand_num += AMOUNT_OF_LABELS;
            switch (comand_num)
            {
                #define DEF_CMD_(name, num, code) {case num: code break;}
                #include "Comands.h"
                #undef DEF_CMD_
                default:    printf("WRONG COMMAND, LINE = %d\n", nStr);
                            assert(0);
                            break;
            }
        }
        else
        {
            (*buf_labels)[count_labels].adr_comands = count_comands;
            ++count_labels;
        }
    }
    free(cur_comand);
    free(cur_label);
    assert((*buf_comands)[count_comands-1] == 69);
    (*len) = count_comands;

    for (int i = 0; i < AMOUNT_OF_COMANDS; ++i)
    {
        free(names_of_comands[i]);
    }
    free(names_of_comands);
    free(string_with_number);
}

//!=============================================================================
//! dump_mass_of_strings
//!
//! @param mass_of_strings   - massive of strings of program
//! @param amount_of_strings - amount of strings in the massive
//!
//! @return nothing
//!=============================================================================

void dump_mass_of_strings (char** mass_of_strings, int amount_of_strings)
{
    printf("======================================================================\n");
    printf("The program:\n");
    for (int i = 0; i < amount_of_strings; ++i) printf("%s\n", mass_of_strings[i]);
    printf("======================================================================\n");
    printf("\n");
}

//!=============================================================================
//! dump_buf_comands
//!
//! @param buf_comands     - massive with comands
//! @param buf_comands_len - amount of cells in the massive
//!
//! @return nothing
//!=============================================================================

void dump_buf_comands (int* buf_comands, int buf_comands_len)
{
    printf("======================================================================\n");
    printf("buf_comands:\n");
    printf("Amount of comands and arguments in the program: %d\n", buf_comands_len);
    for (int i = 0; i < buf_comands_len; ++i)
    {
        printf("%d ", buf_comands[i]);
    }
    printf("\n");
    printf("======================================================================\n");
    printf("\n");

}

//!=============================================================================
//! dump_buf_labels
//!
//! @param buf_labels - bufer with labels
//! @return nothing
//!=============================================================================

void dump_buf_labels (ed_label_t* buf_labels)
{
    printf("======================================================================\n");
    printf("buf_labels:\n");
    for(int i = 0; i < AMOUNT_OF_LABELS; ++i)
    {
        printf("Label %d: %s. In buf_comands: %d. In buf_exec: %d\n", i, buf_labels[i].label, buf_labels[i].adr_comands,
                                                                                              buf_labels[i].adr_exec);
    }
    printf("======================================================================\n");
    printf("\n");
}

//!=============================================================================
//! bin_translator - program which create the executable bufer by
//!                  the bufer with comands
//!
//! @param[out] buf_exec        - pointer to the executable bufer
//! @param      buf_comands     - massive of comands
//! @param      buf_comands_len - length of the massive of comands
//! @param      buf_labels      - massive of labels
//!
//! @return nothing
//!=============================================================================

void bin_translation (char** buf_exec, int* buf_comands, int buf_comands_len, ed_label_t* buf_labels)
{
    assert(buf_exec != 0);
    assert((*buf_exec) != 0);
    assert(buf_comands != 0);
    assert(buf_labels != 0);

    int (*inp)() = my_in;                                                       //for separation absolute adress of my_in
    int inpInt = *(int*)&inp;                                                           //into 4 bytes to call it in time
    char inpInt1 = inpInt % 256;
    inpInt >>= 8;
    char inpInt2 = inpInt % 256;
    inpInt >>= 8;
    char inpInt3 = inpInt % 256;
    inpInt >>= 8;
    char inpInt4 = inpInt % 256;

    void (*out)(int answer) = my_out;                                            //for separation absolute adress of my_out
    int outInt = *(int*)&out;                                                            //into 4 bytes to call it in time
    char outInt1 = outInt % 256;
    outInt >>= 8;
    char outInt2 = outInt % 256;
    outInt >>= 8;
    char outInt3 = outInt % 256;
    outInt >>= 8;
    char outInt4 = outInt % 256;

    int count_comands = 0, count_exec = 0;
    while (count_comands < buf_comands_len)
    {
        int status = buf_comands[count_comands];
        //printf("%d\n", status);
        while(status != 0)
        {
            #define DEF_CMD_(name, num, code) {case num: code break;}
            switch (status)
            {
                #include "Comands_Exec.h"
                default:    printf("WRONG BUF_COMANDS\n");
                            assert(0);
                            break;
            }
            #undef DEF_CMD_
        }
    }

    count_comands = 0;
    count_exec = 0;
    while (count_comands < buf_comands_len)
    {
        int status = buf_comands[count_comands];
        //printf("%d\n", status);
        while(status != 0)
        {
            #define DEF_CMD_(name, num, code) {case num: code break;}
            switch (status)
            {
                #include "Comands_Labels.h"
                default:    printf("WRONG BUF_COMANDS\n");
                            assert(0);
                            break;
            }
            #undef DEF_CMD_
        }
    }
}

//!=============================================================================
//! get_num_of_label - program which answers the question:
//!                    Is this comand marked and what the number of the label?
//!
//! @param num_of_comand - number of comand in the bufer with comands
//! @param buf_labels    - bufer with labels
//!
//! @return number of label - if it's marked, -1 - otherwise
//!=============================================================================

int get_num_of_label (int num_of_comand, ed_label_t* buf_labels)
{
    int num_of_label = -1;
    for (int i = 0; i < AMOUNT_OF_COMANDS && buf_labels[i].label != 0 && num_of_label == -1; ++i)
    {
        if (buf_labels[i].adr_comands == num_of_comand)
        {
            num_of_label = i;
        }
    }

    return num_of_label;
}

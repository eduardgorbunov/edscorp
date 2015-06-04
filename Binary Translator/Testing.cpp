#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <assert.h>

#ifdef _POSIX_MAPPED_FILES

void * mmap(void *start, size_t length, int prot , int flags, int fd, off_t offset);

int munmap(void *start, size_t length);

#endif

#define BUF_EXEC_LENGTH 100*sizeof(char)

int my_in ();

extern "C" void my_out (int answer);

void Dump_buf_exec (char* buf_exec, int length);

int main ()
{
    char* buf_exec = (char*) mmap(0, BUF_EXEC_LENGTH,PROT_EXEC|PROT_READ|PROT_WRITE,
                                                MAP_SHARED|MAP_ANON, -1, 0);

    int (*j)() = my_in;
    void (*i)(int answer) = my_out;
    printf("%p\n", j);

    buf_exec[0] = 0xb8;
    buf_exec[1] = 108;
    buf_exec[2] = 0;
    buf_exec[3] = 0;
    buf_exec[4] = 0;
    buf_exec[5] = 0xbb;
    buf_exec[6] = 2;
    buf_exec[7] = 0;
    buf_exec[8] = 0;
    buf_exec[9] = 0;
    buf_exec[10] = 1;
    buf_exec[11] = 0xd8;
    buf_exec[12] = 0xb9;
    buf_exec[13] = 0xa;
    buf_exec[14] = 0;
    buf_exec[15] = 0;
    buf_exec[16] = 0;
    buf_exec[17] = 0x31;
    buf_exec[18] = 0xd2;
    buf_exec[19] = 0xf7;
    buf_exec[20] = 0xf1;
    buf_exec[21] = 0x50;
    buf_exec[22] = 0x58;
    buf_exec[23] = 0x53;
    buf_exec[24] = 0x5b;
    buf_exec[25] = 0x51;
    buf_exec[26] = 0x59;
    buf_exec[27] = 0x52;
    buf_exec[28] = 0x5a;
    buf_exec[29] = 0xbb;
    int jInt = *(int*)&j;

    buf_exec[30] = jInt % 256;
    jInt >>= 8;
    buf_exec[31] = jInt % 256;
    jInt >>= 8;
    buf_exec[32] = jInt % 256;
    jInt >>= 8;
    buf_exec[33] = jInt % 256;

    buf_exec[34] = 0xff;
    buf_exec[35] = 0xd3;
    int iInt = *(int*)&i;
    buf_exec[36] = 0xbb;

    buf_exec[37] = iInt % 256;
    iInt >>= 8;
    buf_exec[38] = iInt % 256;
    iInt >>= 8;
    buf_exec[39] = iInt % 256;
    iInt >>= 8;
    buf_exec[40] = iInt % 256;
    buf_exec[41] = 0x50;
    buf_exec[42] = 0xff;
    buf_exec[43] = 0xd3;
    buf_exec[44] = 0x58;
    buf_exec[45] = 0xc3;

    Dump_buf_exec (buf_exec, BUF_EXEC_LENGTH);

    int temp = ((int(*)(void)) buf_exec) ();

    munmap((void*)buf_exec, BUF_EXEC_LENGTH);

    return 0;
}

//!=============================================================================
//! my_in - program, which scanf int-number and return it
//!
//! @return scanfed number
//!=============================================================================

int my_in ()
{
    int i = 13;
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
    printf("%d", answer);
}

//!=============================================================================
//! Dump_buf_exec
//!
//! @param buf_exec - executable bufer for objdump
//! @param length   - the length of buf_exec
//!
//! @return nothing
//!=============================================================================

void Dump_buf_exec (char* buf_exec, int length)
{
    FILE* binary = fopen("binary.out", "wb");
    fwrite(buf_exec, length, 1, binary);
    fclose(binary);

    int errorCode = system("objdump -D ./binary.out -b binary -M intel -m i386");
    assert(errorCode == 0);

}

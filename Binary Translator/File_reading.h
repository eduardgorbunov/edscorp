//!=================================================================================
//! ReadFileInBuffer - program which read file in buffer
//!
//! @param[out]      buf - Pointer to the buffer
//! @param[out]      len - The length of the buffer
//! @param[out]      f   - Pointer to the file
//!=================================================================================

void ReadFileInBuffer (char** buf, int* len, FILE* f)
{
    assert(f != 0);
    assert(buf != 0);
    assert((*buf) == 0);
    assert(len != 0);

    fseek (f, 0, SEEK_END);
    *len = ftell (f);
    rewind (f);

    *buf = (char*) calloc ((*len) + 1, sizeof (char));

    assert((*buf) != NULL);

    fread (*buf, *len, sizeof(char), f);

    (*buf)[*len] = '\n';

    (*len)++;
}

//!=================================================================================
//! CountOf - counting the number of occurrences of the symbol in the string.
//!
//! @param      buf     A pointer to the beginning of the string
//! @param      a       Symbol
//! @param      length  The length of the string
//!
//! @return     Number of occurrences of the symbol a (the second parameter)
//!=================================================================================

int CountOf (char* buf, char a, int length)
{
    assert(buf != 0);

    int i = 0, counter = 0;

    for (i = 0; i < length; i++)
    {
        if (buf[i] == a) counter++;
    }

    return counter;
}

//!=================================================================================
//! CreateArrayOfPointersToStr - program which create array of pointer to strings
//!
//! @param[out]      str - Pointer to the char**
//! @param[out]      AmountOfStr - The amount of strings
//! @param[out]      buf - Pointer to the buffer
//! @param[out]      len - The length of the buffer
//!=================================================================================

void CreateArrayOfPointersToStr (char*** str, int* AmountOfStr, char* buf, int len)
{
    assert(AmountOfStr != 0);
    assert(str != 0);
    assert(*str == 0);
    assert(buf != 0);

    *AmountOfStr = CountOf(buf, '\n', len);

    (*str) = (char**) calloc((*AmountOfStr), sizeof(char*));
    assert((*str) != 0);

    int i = 0, startStr = 0, nStr = 0;

    for (i = 0; i < len; i++)
    {
        if (buf[i] == '\n')
        {
            buf[i] = '\0';
            (*str)[nStr] = buf + startStr;
            nStr++;
            startStr = i + 1;
        }
    }
    --(*AmountOfStr);
}

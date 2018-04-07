#include "pow2.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

/* Unsigned Long Long One for 64-bit bit shifting. */
#define LONG_ONE 1ULL

static char* find_next_free(char *buffer, pow2_size_t buffer_size, pow2_size_t required_size);

pow2_byte_t round_next_pow2(pow2_size_t num);


static void zero_memory(char *dst, pow2_size_t size);

/*
** Find the next chunk of memory that is free. Looks for single byte pow2 size bytes. Attempts
** to find pockets of deallocated memory.
*/
static char* find_next_free(char *buffer, pow2_size_t buffer_size, pow2_size_t required_size)
{
    char *pBend;
    char *pResult;
    int found;
    int has_mem_mark;
    pow2_size_t i;

    pBend = buffer + buffer_size;
    pResult = buffer;
    found = 0;

    /*
    ** Loop through array as long as a sufficient space is not found. Looping will advance
    ** pResult forward to a desired memory loation. Looping will be stopped if the required_size 
    ** would not fit between pResult and the end of the buffer.
    */
    while(!found && (pResult + required_size) < pBend)
    {
        /* 
        ** If first byte non-null then it is a pow2 byte.
        */
        if(*pResult != 0)
        {
            pResult += (1 << *pResult);
        }
        /*
        ** If no pow2 byte found, then check the buffer to see if there is enough
        ** space to hold the data. Check for pow2 bytes that may not align with 
        ** prior memory allocations (which will happen when memory is freed).
        */
        else
        {
            has_mem_mark = 0;
            i = 1;

            while(i < required_size && !has_mem_mark)
            {
                /*
                ** If a non-zero byte is found, then a pow2 memory marker has been found.
                ** Otherwise, keep looping bytes.
                */
                if(*(pResult + i) != 0)
                {
                    has_mem_mark = 1;
                }
                else
                {
                    ++i;
                }
            }

            /*
            ** If a pow2 memory marker found, then advance the pResult pointer by 
            ** two raised to the pow2 number. If no pow2 memory marker found, then 
            ** the address pointer by pResult is sufficient to hold the data.
            */
            if(has_mem_mark)
            {
                pResult += (1 << *(pResult + i));
            }
            else
            {
                found = 1;
            }
        }
    }

    /*
    ** If a space was not found, then out of memory. Return NULL to indicate
    ** allocation failure.
    */
    if(found)
    {
        return pResult;
    }
    else
    {
        return NULL;
    }
}

/*
** Place data into the given storage_buff. Returns a pointer to the memory position on success.
** Returns NULL if no space available.
*/
POW2API void* pow2_alloc(char *storage_buff, pow2_size_t storage_size, pow2_size_t size)
{
    char *pMem;
    pow2_byte_t pw2_byte;
    pow2_size_t real_required_size;

    /*
    ** Guard: Make sure storage buffer not null.
    */
    if(storage_buff == NULL)
    {
        return NULL;
    }

    /*
    ** Add one byte to size to account for the pow2 byte that will be 
    ** placed before the data.
    */
    real_required_size = size + 1;

    pw2_byte = round_next_pow2(real_required_size);
    pMem = find_next_free(storage_buff, storage_size, real_required_size);

    /*
    ** Set the first byte to the pow2 byte and return pMem + 1
    */
    if(pMem != NULL)
    {
        *pMem = pw2_byte;
        pMem += 1;
    }

    return pMem;
}

/*
** Free data from the given pointer. This looks at the byte one before
** data to see how much data was allocate in the chunk. Sets bytes to Zero.
** Calling this on a NULL pointer is a no-op.
*/
POW2API void pow2_free(void *data)
{
    pow2_byte_t alloc_pw2;
    pow2_size_t size;

    if(data != NULL)
    {
        /*
        ** Get pow2 byte, then raise 2 to the result to see how much memory to reset.
        ** Start resetting at data - 1 to clear the pow2 byte.
        */
        alloc_pw2 = *((char*)data - 1);
        size = LONG_ONE << alloc_pw2;
        zero_memory(((char*)data - 1), size);
    }
}

/*
** Round a number up to the next power of two, and return a number such that two raised to the number
** will equal the next highest power of two (round_pw2(x) = 2^n).
**
** Example inputs and outputs: 
**     3 -> 2 (2^2 = 4)
**     13 => 4 (2^4 = 16)
**     65 -> 7 (2^7 = 128)
*/
pow2_byte_t round_next_pow2(pow2_size_t num)
{
    pow2_byte_t pw2;

    pw2 = 0;

    while(num > (LONG_ONE << pw2 ))
    {
        ++pw2;
    }

    return pw2;
}

/*
** Zero memory up to size (basically memset(dst, 0, size)).
*/
static void zero_memory(char *dst, pow2_size_t size)
{
    char *pDst;
    char *pEnd;

    pEnd = dst + size;

    for(pDst = dst; pDst < pEnd; ++pDst)
    {
        *pDst = 0;
    }
}
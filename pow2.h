#ifndef _POW2_H
#define _POW2_H

#ifndef NO_STL
#include <inttypes.h>
typedef unsigned char       pow2_byte_t;
typedef uint64_t            pow2_size_t;
#else
typedef unsigned char       pow2_byte_t;
typedef unsigned long long  pow2_size_t;
#endif

/*
** Used to mark public pow2 api.
*/
#define POW2API


/*
** Allocate memory of "size" in the given storage buffer.
*/
POW2API void* pow2_alloc(char *storage_buff, pow2_size_t storage_size, pow2_size_t size);

/*
** Free memory pointed by data. Behavior is undefined if data was not created from pow2_alloc.
*/
POW2API void pow2_free(void *data);


#endif /* _POW2_H */
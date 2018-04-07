# pow2_alloc

Dynamic memory allocation in a static buffer. Wut?

This uses a static array (or other memory) and dynamically allocates and frees memory into that buffer similar to malloc and free. It rounds memory size to the nearest power of 2, then stores a 1 byte "header" to keep track of memory allocations.

## API

The API consists of two simple methods:

* `void* pow2_alloc(char *buffer, uint64 buffer_size, uint64 allocate_size)` - Allocates memory to the buffer.
* `void pow2_free(void *)` - Fress memory created from pow2_alloc.

## Memory Allocation

Memory is allocated in chunks of powers of two. A one-byte exponent is stored in the first byte of the memory allocation, and a pointer to the next byte is returned. A memory allocation for 6 bytes will look like the following:


|0  |  1|  2|  3|  4|  5|  6|  7|  8|
|---|---|---|---|---|---|---|---|---|
|  4|ptr|   |   |   |   |   |   |nxt|


* **Byte 0** holds the unsigned 8-bit integer 4 (this is called the pow2 byte). When 2 is raised by this number, it gives the memory used by the allocation (8 bytes).

* **ptr** is what is returned by the function. It points to the first usable byte in the memory block.

* **nxt** is the next byte that would be allocated in furture pow2_alloc calls.

## Fragmentation and Free

Allocated bytes are zeroed out when pow2_free is called. pow2 will attempt to fit data into the first available block that can hold the required size plus the pow2 byte.

## Tests

`makefile` builds a test program for pow2.
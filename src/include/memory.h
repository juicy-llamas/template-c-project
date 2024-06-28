
#ifndef MEM_ALLOC_STUFF_DAMN_THESE_STUPID_GUARDS_FUCK_YOU_GO_FUCK_YOURSELF
#define MEM_ALLOC_STUFF_DAMN_THESE_STUPID_GUARDS_FUCK_YOU_GO_FUCK_YOURSELF

#include <stddef.h>

void*  mem_malloc       ( size_t amt );
void*  mem_realloc      ( void* oldptr, size_t amt );
void   mem_free         ( void* ptr );
size_t temp_exp_realloc ( void** orig, size_t sizevar, size_t lenvar,
                          size_t addedlen, char rate );

// c compiler complains about type of a.
// Of course, one has to be careful to provide a pointer to the pointer
// in question that is being realloced...
#define exp_realloc( a, b, c, d, e ) \
    temp_exp_realloc( (void**)(a), (b), (c), (d), (e) )

#endif

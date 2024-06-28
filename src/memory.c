
#include "root.h"
#include "memory.h"
#include "logging.h"

#include <stdlib.h>

#ifdef MDEBUG
#define log_mdebug( fmt, ... ) log_debug( (fmt), ##__VA_ARGS__ )
#else
#define log_mdebug( fmt, ... ) ((void)0)
#endif

// these fns are pretty straightforward and aim to replace the standard
// alloc/free set

void* mem_malloc ( size_t amt ) {
    log_mdebug( "mem_malloc( %li ): attempting malloc", amt );
    void* ret = malloc( amt );

    if ( ret == 0 ) {
        log_fatal( "mem_malloc: malloc failed" );
        exit( 1 );
    }

    log_mdebug( "mem_malloc: ptr == %p", ret );
    return ret;
}

void* mem_realloc ( void* oldptr, size_t amt ) {
    log_mdebug( "mem_realloc( %p, %li ): attempting realloc", oldptr, amt );
    void* ret = realloc( oldptr, amt );

    if ( ret == 0 ) {
        log_fatal( "mem_realloc: realloc failed" );
        exit( 1 );
    }

    log_mdebug( "mem_realloc: new ptr == %p", ret );
    return ret;
}

void mem_free ( void* ptr ) {
    log_mdebug( "mem_free( %p ): attempting free", ptr );
    free( ptr );
}

// this takes the size of orig, the current # of elms (lenvar), the
// added len, and the fraction of allocation:
// rate calculation (^ is exp): new_size = size * (1 + (1/2)^rate)
// returns the new size
size_t temp_exp_realloc ( void** orig, size_t sizevar,
                          size_t lenvar, size_t addedlen, char rate ) {
    log_mdebug( "exp_realloc( %p, %li, %li, %li, %i ): attempting exp "
            "realloc", *orig, sizevar, lenvar, addedlen, rate );
    size_t ret = sizevar;

    // if our size is already big enough, no need to call realloc.
    if ( sizevar >= lenvar + addedlen ) {
        log_mdebug( "exp_realloc: no new memory added" );
        return ret;
    }

    do { // we loop until we get a number bigger than lenvar + addedlen.
        size_t amt = ( ret >> rate );
        amt = amt > 0 ? amt : 1;
        ret = ret + amt;
    } while ( ret < lenvar + addedlen ); 
    
    *orig = realloc( *orig, ret );
    if ( *orig == 0 ) {
        log_fatal( "exp_realloc: realloc failed", ret );
        exit( 1 );
    }

    log_mdebug( "exp_realloc: new ptr == %p", *orig );
    return ret;
}

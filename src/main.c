
#include "root.h"
#include "logging.h"
#include "memory.h"
#include "assert_macros.h"
#include <time.h>

int main () {
    // look in logging.h
    log_info( "hola" );
    log_warn( "this is a warning" );
    log_err( "this is an error" );

    // this logs to STDOUT if MDEBUG is set
    // will also close program with an explanation if failure
    // if this is undesirable, malloc is always available
    time_t* ptr = mem_malloc( sizeof( time_t ) );
    time( ptr );

    // capacity of ptr
    size_t ptr_size = sizeof( time_t );
    // len of ptr (in bytes, aka how many bytes are occupied)
    size_t ptr_len = sizeof( time_t );

    // let's say we want to add two more times to ptr

    // this will resize the pointer to fit 4 + 8 bytes, growing
    // it exponentially. THe last parameter is the fraction of
    // growth: 0 is doubling, 1 is adding half, 2 is adding a quarter,
    // etc. It will add a whole/half/quarter until ptr_size > ptr_len +
    // added amt
    ptr_size = exp_realloc( &ptr, 
                             ptr_size, 
                             ptr_len, 
                             2 * sizeof( time_t ), 
                             0 );
    // handy assert macros. If assertion fails, it exits the program
    // and prints a nice message rather than abort.
    assert_eq( ptr_size, 4 * sizeof( time_t ) );

    time( ptr + 1 );
    time( ptr + 2 );
    ptr_len = ptr_len + 2 * sizeof( time_t );

    // another convenience function for printing out buffers.
    pbuffer( (const char*)ptr, ptr_len );
    printf( "\n" );

    return 0;
}

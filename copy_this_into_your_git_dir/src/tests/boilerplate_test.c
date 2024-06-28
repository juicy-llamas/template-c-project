
// test boilerplate

#include "root.h"
#include "memory.h"
#include "assert_macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include <time.h>

int test ();

#ifndef BENCH_TESTING
int main () {
    return test();
}
#endif

int test () {
    // begin the test
    
    // this is an example that also functions as a test of
    // the assert macros in assert_macros.h
    puts( "Write a test here!" );
    int k = time( 0 ) % 2;
    assert_eq( 1, 1 );
    assert_neq( 1, 2 );
    assert_neq( k, 3 );

    // end the test
    return 0;
}

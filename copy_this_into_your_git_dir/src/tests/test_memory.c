
// test boilerplate
#include "root.h"
#include "memory.h"
#include "assert_macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

int test ();

#ifndef BENCH_TESTING
int main () {
    return test();
}
#endif

int test () {
    // begin the test

    printf( "TODO: write some tests for memory macros" );

    // end the test
    return 0;
}

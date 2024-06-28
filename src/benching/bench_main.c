
// this gets defined in other bench files.
extern int test();

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "root.h"
#include "logging.h"

#ifndef TRIALS
#define TRIALS 1
#endif

// this gets compiled with `-DBENCH_TESTING`
// this is to prevent two mains when benching tests
int main () {
    int ret = 0;
    struct timespec t_start, t___end;

    // test the clock
    if ( clock_gettime( CLOCK_MONOTONIC, &t_start ) < 0 )
        log_err_ret( -1, "getting time failed" );

    // Programs should NOT WRITE TO STDIO AT ALL, thus we disable it
    if ( fflush( stdout ) == -1 )
        log_err_ret( -1, "fflush stdout failed" );
    if ( fflush( stderr ) == -1 )
        log_err_ret( -1, "fflush stderr failed" );

    int ORIG_STDOUT = dup( STDOUT_FILENO );
    if ( ORIG_STDOUT == -1 )
        log_err_ret( -1, "dup STDOUT_FILENO failed" );

    int ORIG_STDERR = dup( STDERR_FILENO );
    if ( ORIG_STDERR == -1 )
        log_err_ret( -1, "dup STDERR_FILENO failed" );

    int dev_null = open( "/dev/null", O_WRONLY );
    if ( dev_null == -1 )
        log_err_ret( -1, "open /dev/null failed" );

    if ( dup2( dev_null, STDOUT_FILENO ) == -1 )
        log_err_ret( -1, "dup2 STDOUT_FILENO failed" );
    if ( dup2( dev_null, STDERR_FILENO ) == -1 )
        log_err_ret( -1, "dup2 STDERR_FILENO failed" );

    // Benchmarking part
    clock_gettime( CLOCK_MONOTONIC, &t_start );
    for ( int i = 0; i < TRIALS; i++ )
        ret = test();
    clock_gettime( CLOCK_MONOTONIC, &t___end );
    
    // Restore streams
    if ( fflush( stdout ) == -1 )
        log_err_ret( -1, "fflush stdout failed" );
    if ( fflush( stderr ) == -1 )
        log_err_ret( -1, "fflush stderr failed" );
    
    if ( dup2( ORIG_STDOUT, STDOUT_FILENO ) == -1 )
        log_err_ret( -1, "dup2 (restore) STDOUT_FILENO failed" );
    if ( dup2( ORIG_STDERR, STDERR_FILENO ) == -1 )
        log_err_ret( -1, "dup2 (restore) STDERR_FILENO failed" );

    // Compute final times
    double elapsed = t___end.tv_sec - t_start.tv_sec;
    elapsed += ( (double)t___end.tv_nsec / (double)1000000000.0L ) -
               ( (double)t_start.tv_nsec / (double)1000000000.0L );

    printf( "%lf\n", elapsed );
    return ret;
}

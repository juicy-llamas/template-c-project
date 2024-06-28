
#include "root.h"
#include "logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t log_mutex;

void pbuffer ( const char* ptr, size_t ptr_len ) {
    printf( "[ " );
    for ( size_t i = 0; i < ptr_len; i++ ) {
        printf( "0x%x", ptr[ i ] );
        if ( i < ptr_len - 1 ) 
            printf( ", " );
    }
    printf( " ]" );
}

// file and line are filled in by macros
void log_at_level ( enum logging_level level, const char* fmt, 
                    const char* file, int line, ... ) {
    va_list ap;
    char* log_lvl = 0;
    char str_time[ 26 ];
    char str_prompt[ PROMPT_SIZE ];
    time_t int_time;
    char* errno_str;
    FILE* print_stream = stdout;

    if ( pthread_mutex_lock( &(log_mutex) ) != 0 )
        log_err_ret( (void)0, "mutex lock failed" );

    // no point in doing anything
    // if ( level < ctx->prompt_level && level < ctx->file_level )
    //     return;

    // get the log lvl
    switch ( level ) {
        case TRIVIAL:
            log_lvl = "TRIVIAL";
            break;
        case TDEBUG:
            log_lvl = "DEBUG";
            break;
        case INFO:
            log_lvl = "INFO";
            break;
        case WARN:
            log_lvl = "WARNING";
            break;
        case ERROR:
            log_lvl = "ERROR";
            print_stream = stderr;
            errno_str = strerror( errno );
            break;
        case FATAL:
            log_lvl = "FATAL ERROR";
            print_stream = stderr;
            errno_str = strerror( errno );
            break;
         default:
            return;
    }
    
    va_start( ap, line );

    // get the time as a string
    time( &int_time );
    if ( int_time == -1 ) {
        fprintf( stderr, "FATAL ERROR: log_at_level: couldn't get the "
                "time: %s\nExiting...\n", strerror( errno ) );
        exit( 1 );
    }
    ctime_r( &int_time, str_time );
    str_time[ 24 ] = 0;    

    // form the prompt string: "[time]: [prompt]: [msg]{: [errno]}\n"
    if ( level >= ERROR && errno_str != strerror( 0 ) ) {
        snprintf( str_prompt, PROMPT_SIZE,
                "%s: %s: %s: %s\n", str_time, log_lvl, fmt, errno_str );
    // trivial and debug should have file/line
    } else if ( level <= DEBUG ) {
        snprintf( str_prompt, PROMPT_SIZE,
                "%s: %s (%s:%i): %s\n", str_time, log_lvl, 
                file, line, fmt );
    } else {
        snprintf( str_prompt, PROMPT_SIZE,
                "%s: %s: %s\n", str_time, log_lvl, fmt );
    }

    // deliver the print messages
    // if ( level >= ctx->prompt_level )
   
    // I decided due to threading, it would be best to just use stdout
    // and let the caller redirect that stream if they want
    vfprintf( print_stream, str_prompt, ap );
    
    // if ( level >= ctx->file_level )
    //     vfprintf( ctx->log_file, str_prompt, ap );
    
    if ( pthread_mutex_unlock( &(log_mutex) ) != 0 )
        log_err_ret( (void)0, "mutex unlock failed" );

    if ( level == FATAL )
        exit( 1 );
}

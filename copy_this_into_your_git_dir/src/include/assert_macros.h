#ifndef STUYFF_STUFF_STUFF_ASSERT_MACROS_HEADER_THINGYS_DFSD_F_SD_F
#define STUYFF_STUFF_STUFF_ASSERT_MACROS_HEADER_THINGYS_DFSD_F_SD_F

/*
 * These should really only be used in tests.
 */

// exit()
#include <stdlib.h>
#include <stdio.h>

#ifdef ASSERT_MACROS

/*
 * These are internal and should not be used by the outside
 */
    // _Generic is a new thingy since C11
    #define deduce_fmt( x ) _Generic( (x),                              \
        char: "%c",                                                     \
        unsigned char: "%i",                                            \
        short: "%i",                                                    \
        unsigned short: "%u",                                           \
        int: "%i",                                                      \
        unsigned int: "%u",                                             \
        long int: "%li",                                                \
        unsigned long int: "%lu",                                       \
        long long int: "%lli",                                          \
        unsigned long long int: "%llu",                                 \
        float: "%f",                                                    \
        double: "%f",                                                   \
        /* assume something that is not listed is a ptr */              \
        default: "%p"                                                   \
    )

    #define STR( x ) #x

    #define assert_stub( x, y, op ) ({                                  \
        if ( !( op( x, y ) ) ) {                                        \
            char the_fmt_buffer[ 4096 ];                                \
            snprintf( the_fmt_buffer, 4096, "Assertion (file " __FILE__ \
                      ", line %i) failed (%s %s %s)\n", __LINE__,       \
                      deduce_fmt( x ), STR( op ), deduce_fmt( y ) );    \
            fprintf( stderr, the_fmt_buffer, x, y );                    \
            exit( 1 );                                                  \
        }                                                               \
    })

    #define operator_eq( x, y ) ( (x) == (y) )
    #define operator_lt( x, y ) ( (x) <  (y) )
    #define operator_gt( x, y ) ( (x) >  (y) )
    #define operator_neq( x, y ) ( (x) != (y) )
    #define operator_leq( x, y ) ( (x) <= (y) )
    #define operator_geq( x, y ) ( (x) >= (y) )

/*
 * These are public
 */
    #define assert_eq( x, y ) assert_stub( (x), (y), operator_eq )
    #define assert_lt( x, y ) assert_stub( (x), (y), operator_lt )
    #define assert_gt( x, y ) assert_stub( (x), (y), operator_gt )
    #define assert_neq( x, y ) assert_stub( (x), (y), operator_neq )
    #define assert_leq( x, y ) assert_stub( (x), (y), operator_leq )
    #define assert_geq( x, y ) assert_stub( (x), (y), operator_geq )

//  this is special
    #define assert_steq( x, y ) ({                                      \
         if ( strcmp( (x), (y) ) ) {                                    \
            printf( "Assertion (file %s, line %i) failed (%s !="        \
            "%s)\n", __FILE__, __LINE__, (x), (y) );                    \
            exit( 1 );                                                  \
        }                                                               \
    })

//  this is also special
    #define assert_blkeq( x, y, n ) ({                                  \
        for ( int i = 0; i < (n); i++ ) {                               \
            if ( ((const char*)(x))[ i ] != ((const char*)(y))[ i ] ) { \
                printf( "Assertion (file %s, line %i) failed (%s !="    \
                "%s)\n", __FILE__, __LINE__, (x), (y) );                \
                exit( 1 );                                              \
            }                                                           \
        }                                                               \
    })

#else //#ifdef ASSERT_MACROS
    #define assert_eq( x, y ) ((void)0)
    #define assert_lt( x, y ) ((void)0)
    #define assert_gt( x, y ) ((void)0)
    #define assert_neq( x, y ) ((void)0)
    #define assert_leq( x, y ) ((void)0)
    #define assert_geq( x, y ) ((void)0)
    #define assert_steq( x, y ) ((void)0)
    #define assert_blkeq( x, y, n ) ((void)0)
#endif //#ifdef ASSERT_MACROS


#endif //#ifndef STUYFF_STUFF_STUFF_ASSERT_MACROS_HEADER_THINGYS_DFSD_F_SD_F

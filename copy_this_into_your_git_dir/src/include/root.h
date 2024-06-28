
#ifndef I_AM_GROOT_HEAD__xXEXTRA_DISTINCTIVE_VERSIONXx
#define I_AM_GROOT_HEAD__xXEXTRA_DISTINCTIVE_VERSIONXx

/*
 * IMPORTANT Definitions.
 */

// disables asserts. Up to the builder.
// #define NDEBUG

// enables logging macros (replaced with logging.c/h)
// #define USE_LOGGING_MACROS

// Assert macros are conditional on debug
#ifdef DEBUG
#define ASSERT_MACROS
#endif

// enables memory debugging (see every single time the code calls 
// malloc and realloc, maybe free too)
#define MDEBUG

// C stuff
#define _GNU_SOURCE

/*
 * Other useful macros, such as suppressing the unused warning.
 */

#define UNUSED( a ) ( (void)( 0 + a ) )

#endif //#ifndef I_AM_GROOT_HEAD__xXEXTRA_DISTINCTIVE_VERSIONXx

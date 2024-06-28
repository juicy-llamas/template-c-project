
#ifndef OMG_THIS_IS_SUCH_A_STUPID_THINGY_LOGGING_H_AHHAHAHAHAHAAAAHAHAHA
#define OMG_THIS_IS_SUCH_A_STUPID_THINGY_LOGGING_H_AHHAHAHAHAHAAAAHAHAHA

#include <stddef.h>

#define PROMPT_SIZE 2048

enum logging_level {
    ALL,    // this is for a logging context and is a catchall
    TRIVIAL,
    TDEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NONE    // this is for a logging context and is also a catchall
};

// struct logging_context {
//     FILE* log_file;
//     enum logging_level prompt_level;
//     enum logging_level file_level;
//     pthread_mutex_t mutex;
// };

#define log_trivial( fmt, ... )\
    log_at_level( TRIVIAL, (fmt), __FILE__, __LINE__, ##__VA_ARGS__ )
#define log_debug( fmt, ... )\
    log_at_level( DEBUG, (fmt), __FILE__, __LINE__, ##__VA_ARGS__ )
#define log_info( fmt, ... )\
    log_at_level( INFO, (fmt), __FILE__, __LINE__, ##__VA_ARGS__ )
#define log_warn( fmt, ... )\
    log_at_level( WARN, (fmt), __FILE__, __LINE__, ##__VA_ARGS__ )
#define log_err( fmt, ... )\
    log_at_level( ERROR, (fmt), __FILE__, __LINE__, ##__VA_ARGS__ )
#define log_fatal( fmt, ... )\
    log_at_level( FATAL, (fmt), __FILE__, __LINE__, ##__VA_ARGS__ )

#define log_err_ret( ret, fmt, ... ) ({                             \
    log_at_level( ERROR, (fmt), __FILE__, __LINE__, ##__VA_ARGS__ );\
    return (ret);                                                   \
})
#define log_fatal_ret( ret, fmt, ... ) ({                           \
    log_at_level( FATAL, (fmt), __FILE__, __LINE__, ##__VA_ARGS__ );\
    return (ret);                                                   \
})

void log_at_level ( enum logging_level level,
                 /* struct logging_context* ctx, */
                    const char* fmt,
                    const char* file, int line, ... );

void pbuffer ( const char* ptr, size_t ptr_len );

#endif

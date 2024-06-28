
#include <time.h>

int test () {
    int the_time = (int)time( NULL );
    int the_time2 = (int)time( NULL );
    return the_time + the_time2;
}

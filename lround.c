/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <errno.h>
#include <limits.h>
#include <math.h>

long lround(double x) {
    double r = round(x);
    if(!isfinite(r) || r > (double)LONG_MAX || r < (double)LONG_MIN) {
        errno = ERANGE;
        /* Return value on error is implementation defined, often LONG_MIN/MAX */
        return (r > 0) ? LONG_MAX : LONG_MIN;
    }
    return (long)r;
}

/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <math.h>
#include <stdint.h>

double round(double x) {
    int e;
    double abs_x, t;
    union {
        double f;
        uint64_t i;
    } u;
    u.f = x;
    e = (int)((u.i >> 52) & 0x7FF) - 1023;

    /* 1. Handle NaNs, Infs, and large integers */
    if(e >= 52)
        return x;

    /* 2. Handle small numbers */
    if(e < -1) {
        /* |x| < 0.5 -> return +/-0.0 */
        u.i &= 0x8000000000000000ULL;
        return u.f;
    }

    /* 3. Handle 0.5 <= |x| < 1.0 */
    if(e == -1) {
        /* return +/- 1.0 */
        u.i &= 0x8000000000000000ULL;
        u.f += 1.0;
        return u.f;
    }

    /* 4. General case: use floor/ceil logic + offset */
    /* Algorithm: add 0.5 to magnitude, then truncate (floor for pos, ceil for neg) */
    abs_x = fabs(x);
    t = floor(abs_x + 0.5);

    return (x < 0) ? -t : t;
}

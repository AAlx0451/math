/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <math.h>
#include <stdint.h>

double floor(double x) {
    union {
        double f;
        uint64_t i;
    } u;
    u.f = x;
    int e = (int)((u.i >> 52) & 0x7FF) - 1023;

    if(e >= 52)
        return x; /* Integer or NaN/Inf */
    if(e < 0) {
        if(x == 0.0)
            return x;
        return (x > 0.0) ? 0.0 : -1.0;
    }

    uint64_t m = 0xFFFFFFFFFFFFFFFFULL >> (12 + e);
    if((u.i & m) == 0)
        return x; /* Already integer */

    if(x < 0.0)
        u.f += 1.0;
    u.i &= ~m;
    if(x < 0.0)
        u.f -= 1.0;

    return u.f;
}

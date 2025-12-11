/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

/*
 * Source:
 * IEEE 754 Bit Manipulation (Decomposition).
 */

#include <math.h>
#include <stdint.h>

double modf(double x, double *iptr) {
    union { double f; uint64_t i; } u;
    u.f = x;
    int e = (int)((u.i >> 52) & 0x7FF) - 1023;

    if (e >= 52) {
        *iptr = x;
        if (isnan(x)) return x;
        u.i &= 0x8000000000000000ULL;
        return u.f;
    }

    if (e < 0) {
        u.i &= 0x8000000000000000ULL;
        *iptr = u.f;
        return x;
    }

    uint64_t m = -1ULL >> (12 + e);
    
    u.i &= ~m;
    *iptr = u.f;
    
    return x - u.f;
}

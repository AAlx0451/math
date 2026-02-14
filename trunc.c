/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <math.h>
#include <stdint.h>

double trunc(double x) {
    union {
        double f;
        uint64_t i;
    } u;
    u.f = x;
    int e = (int)((u.i >> 52) & 0x7FF) - 1023;

    /* If exponent >= 52, it's already integer or Inf/NaN */
    if(e >= 52)
        return x;

    /* If exponent < 0, |x| < 1.0, result is +/- 0.0 */
    if(e < 0) {
        /* Keep sign bit, zero the rest */
        u.i &= 0x8000000000000000ULL;
        return u.f;
    }

    /* Mask out fractional bits based on exponent */
    /* 52 bits of mantissa. We keep 'e' bits + implied 1. */
    uint64_t m = -1ULL >> (12 + e);

    /* If masked bits are 0, it is already integer */
    if((u.i & m) == 0)
        return x;

    /* Zero out the fractional bits */
    u.i &= ~m;
    return u.f;
}

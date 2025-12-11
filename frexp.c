/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <math.h>
#include <stdint.h>

double frexp(double x, int *exp) {
    union { double f; uint64_t i; } u;
    u.f = x;
    int e = (int)((u.i >> 52) & 0x7FF);
    
    if (e == 0) { /* Subnormal */
        if (x == 0.0) { *exp = 0; return x; }
        x *= 1.8014398509481984e16; /* 2^54 */
        u.f = x;
        e = (int)((u.i >> 52) & 0x7FF) - 54;
    }
    
    if (e == 0x7FF) { *exp = 0; return x; } /* Inf/NaN */
    
    *exp = e - 1022;
    u.i = (u.i & 0x800FFFFFFFFFFFFFULL) | 0x3FE0000000000000ULL;
    return u.f;
}

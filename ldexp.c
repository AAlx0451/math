/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <errno.h>
#include <math.h>
#include <stdint.h>

double ldexp(double x, int exp) {
    union {
        double f;
        uint64_t i;
    } u;
    if(exp == 0 || x == 0.0)
        return x;
    if(!isfinite(x))
        return x;

    if(exp > 0) {
        if(exp > 1023) {
            /* Handle potential overflow in steps */
            x *= 8.9884656743115795e307; /* 2^1023 */
            exp -= 1023;
            if(exp > 1023) {
                errno = ERANGE;
                return (x > 0) ? HUGE_VAL : -HUGE_VAL;
            }
        }
        u.i = (uint64_t)(exp + 1023) << 52;
        return x * u.f;
    } else {
        if(exp < -1022) {
            /* Handle potential underflow */
            x *= 2.2250738585072014e-308; /* 2^-1022 */
            exp += 1022;
        }
        u.i = (uint64_t)(exp + 1023) << 52;
        return x * u.f;
    }
}

/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <math.h>
#include <stdint.h>

double fabs(double x) {
    union { double f; uint64_t i; } u;
    u.f = x;
    u.i &= 0x7FFFFFFFFFFFFFULL; /* Clear sign bit */
    return u.f;
}

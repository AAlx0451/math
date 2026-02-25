/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

#include <math.h>
#include <stdint.h>

double copysign(double x, double y) {
    union {
        double f;
        uint64_t i;
    } ux, uy;
    ux.f = x;
    uy.f = y;
    /* Take all value bits from x, take sign bit from y */
    ux.i = (ux.i & 0x7FFFFFFFFFFFFFULL) | (uy.i & 0x8000000000000000ULL);
    return ux.f;
}

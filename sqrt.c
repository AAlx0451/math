/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Academic Source:
 * Newton-Raphson Method (Heron's Algorithm).
 * Initial guess via bit manipulation (Magic number).
 */

#include <math.h>
#include <errno.h>
#include <stdint.h>

double sqrt(double x) {
    if (x < 0.0) {
        errno = EDOM;
        return NAN;
    }
    if (x == 0.0 || !isfinite(x)) return x;

    /* Magic Initial Guess (approximates 1/sqrt(x) logic inverted) */
    /* Often attributed to Kahan/Carmack but mathematically derived from log structure of IEEE */
    union { double f; uint64_t i; } u;
    u.f = x;
    u.i = (u.i >> 1) + 0x1FF8000000000000ULL;
    double y = u.f;

    /* Babylonian / Heron's steps */
    y = 0.5 * (y + x / y);
    y = 0.5 * (y + x / y);
    y = 0.5 * (y + x / y); /* Double precision usually converges by 3rd or 4th step */
    
    return y;
}

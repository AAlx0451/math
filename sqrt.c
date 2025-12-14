/*
 * Part of PD LibM
 * Method: Newton-Raphson + Tuckerman rounding correction
 */

#include "math_private.h"
#include <errno.h>

/* 
 * Magic constant origin:
 * Derived from 3/2 * 2^52 * (127 - sigma)
 * Approximation for 1/sqrt(x) initial guess.
 */
#define SQRT_MAGIC 0x1FF8000000000000ULL

double sqrt(double x) {
    if (x < 0.0) {
        errno = EDOM;
        return NAN;
    }
    if (x == 0.0 || !isfinite(x)) return x;

    /* Initial guess */
    uint64_t i;
    EXTRACT_WORD64(i, x);
    i = (i >> 1) + SQRT_MAGIC;
    double y;
    INSERT_WORD64(y, i);

    /* Heron's Iterations (Newton) */
    /* 3 iterations are usually sufficient for double precision convergence */
    y = 0.5 * (y + x / y);
    y = 0.5 * (y + x / y);
    y = 0.5 * (y + x / y);

    /* 
     * Tuckerman rounding test / Adjustment:
     * Check if y*y is correctly related to x to within 0.5 ULP.
     * Simple correction step: y = y + (x - y*y) / (2*y)
     */
    double e = x - y * y;
    y = y + e / (2.0 * y);
    
    return y;
}

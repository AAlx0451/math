/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

#include <math.h>

double fmax(double x, double y) {
    if (isnan(x)) return y;
    if (isnan(y)) return x;

    /* IEEE 754: Signed zero check. fmax(-0.0, +0.0) = +0.0 */
    if (x == 0.0 && y == 0.0) {
        return (signbit(x)) ? y : x;
    }

    return (x > y) ? x : y;
}

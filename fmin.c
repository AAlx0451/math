/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

#include <math.h>

double fmin(double x, double y) {
    if(isnan(x))
        return y;
    if(isnan(y))
        return x;

    /* IEEE 754: Signed zero check. -0.0 < +0.0 */
    if(x == 0.0 && y == 0.0) {
        return (signbit(x)) ? x : y;
    }

    return (x < y) ? x : y;
}

/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <math.h>
#include <errno.h>

static const double M_PI_2 = 1.57079632679489661923;

/* 
 * Calculates acos(x) using the identity:
 * acos(x) = pi/2 - asin(x)
 */
double acos(double x) {
    if (x < -1.0 || x > 1.0) {
        errno = EDOM;
        return NAN;
    }
    return M_PI_2 - asin(x);
}

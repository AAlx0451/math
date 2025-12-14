/*
 * Part of PD LibM
 */

#include "math_private.h"
#include <errno.h>

static const double M_PI_2 = 1.57079632679489661923;
static const double M_PI   = 3.14159265358979323846;

double acos(double x) {
    if (x < -1.0 || x > 1.0) {
        errno = EDOM;
        return NAN;
    }

    /* 
     * When x is near 1, acos(x) is close to 0.
     * Direct computation pi/2 - asin(x) involves cancellation (pi/2 - ~pi/2).
     * Identity: acos(x) = 2 * asin(sqrt((1-x)/2))
     */

    if (x > 0.5) {
        double z = sqrt(0.5 * (1.0 - x));
        /* asin(z) is computed accurately near 0 */
        return 2.0 * asin(z);
    }
    
    if (x < -0.5) {
        /* acos(x) = pi - acos(-x) */
        double z = sqrt(0.5 * (1.0 + x)); /* 1 - (-x) */
        return M_PI - 2.0 * asin(z);
    }

    return M_PI_2 - asin(x);
}

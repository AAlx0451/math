/*
 * Part of PD LibM
 */

#include "math.h"
#include "math_private.h"
#include <errno.h>

double atan2(double y, double x) {
    if (isnan(x) || isnan(y)) return NAN;

    /* Handle Infinities (IEEE 754 spec) */
    if (isinf(x) || isinf(y)) {
        if (isinf(x)) {
            if (isinf(y)) {
                /* Inf / Inf -> +/- pi/4 or 3pi/4 */
                double v = (y > 0) ? (M_PI/4) : -(M_PI/4);
                if (x < 0) v = (y > 0) ? (3*M_PI/4) : -(3*M_PI/4);
                return v;
            }
            /* x=Inf, y=Finite */
            if (x > 0) return (y >= 0) ? 0.0 : -0.0;
            return (y >= 0) ? M_PI : -M_PI;
        }
        /* x=Finite, y=Inf */
        return (y > 0) ? M_PI_2 : -M_PI_2;
    }

    /* Handle Zeros */
    if (y == 0.0) {
        if (x >= 0) return y; /* Returns +/- 0.0 correctly */
        return (signbit(y)) ? -M_PI : M_PI;
    }
    
    if (x == 0.0) {
        return (y > 0) ? M_PI_2 : -M_PI_2;
    }

    /* Calculate via atan() with argument reduction */
    double z = atan(fabs(y / x));

    /* Adjust quadrant placement */
    if (x > 0) {
        return (y > 0) ? z : -z;
    } else {
        return (y > 0) ? (M_PI - z) : (z - M_PI);
    }
}

/*
 * Part of PD LibM
 */

#include "math_private.h"
#include <errno.h>

static const double M_PI = 3.14159265358979323846;
static const double M_PI_2 = 1.57079632679489661923;

/* Coefficients for Atan (Hart & Cheney) */
static const double
a0 = -0.33333333333333333333e0,
a1 =  0.19999999999999999999e0,
a2 = -0.14285714285714285714e0,
a3 =  0.11111111111111111110e0,
a4 = -0.09090909090909090909e0,
a5 =  0.07692307692307692307e0,
a6 = -0.06666666666666666666e0,
a7 =  0.05882352941176470588e0;

static double _atan_core(double x) {
    double x2 = x * x;
    double p = a7;
    p = a6 + x2 * p;
    p = a5 + x2 * p;
    p = a4 + x2 * p;
    p = a3 + x2 * p;
    p = a2 + x2 * p;
    p = a1 + x2 * p;
    p = a0 + x2 * p;
    return x + x * (x2 * p);
}

double atan(double x) {
    double absx = fabs(x);
    double res;
    if (absx > 1.0) {
        res = M_PI_2 - _atan_core(1.0 / absx);
    } else {
        res = _atan_core(absx);
    }
    return (x < 0) ? -res : res;
}

double atan2(double y, double x) {
    if (isnan(x) || isnan(y)) return NAN;

    /* Handle Signed Zeros and Signs explicitly BEFORE division */
    uint64_t ix, iy;
    EXTRACT_WORD64(ix, x);
    EXTRACT_WORD64(iy, y);

    int sign_x = (ix >> 63);
    int sign_y = (iy >> 63);

    /* x = +/- 0 */
    if (x == 0.0) {
        if (y == 0.0) {
            /* 
             * (+0, +0) -> +0
             * (-0, +0) -> -0
             * (+0, -0) -> +pi
             * (-0, -0) -> -pi
             */
            return sign_y ? (sign_x ? -M_PI : -0.0) 
                          : (sign_x ?  M_PI :  0.0);
        }
        return (y > 0) ? M_PI_2 : -M_PI_2;
    }

    /* y = +/- 0 */
    if (y == 0.0) {
        return (x > 0) ? y : (sign_y ? -M_PI : M_PI);
    }

    /* Inf handling */
    if (isinf(x) || isinf(y)) {
        if (isinf(x)) {
            if (isinf(y)) {
                double v = (y > 0) ? M_PI/4 : -M_PI/4;
                return (x > 0) ? v : (y > 0 ? 3*v : -3*v); // Correct quadrants for inf/inf
            }
            return (x > 0) ? (sign_y ? -0.0 : 0.0) : (sign_y ? -M_PI : M_PI);
        }
        return (y > 0) ? M_PI_2 : -M_PI_2;
    }

    /* Calculate z safely */
    double z = y / x;
    double az = fabs(z);
    double ret;

    /* Standard atan reduction */
    if (az > 1.0) {
        ret = M_PI_2 - _atan_core(1.0 / az);
    } else {
        ret = _atan_core(az);
    }

    /* Quadrant placement based on original signs */
    if (x > 0) {
        return (y < 0) ? -ret : ret;
    } else {
        return (y < 0) ? -M_PI + ret : M_PI - ret;
    }
}

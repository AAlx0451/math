/*
 * Part of PD LibM
 */

#include "math_private.h"
#pragma clang diagnostic ignored "-Wreserved-identifier"

/*
 * Coefficients for Atan (Remez algorithm for |x| <= 0.414)
 * Much more precise than simple Taylor for the reduced range.
 */
static const double
    at1 = 0.999999999999895256e0,
    at3 = -0.333333333324640166e0,
    at5 = 0.199999999247664653e0,
    at7 = -0.142857053550882998e0,
    at9 = 0.111108257088456885e0,
    at11 = -0.090852331561668962e0;

/*
 * Internal kernel: assumes x >= 0
 * Implements argument reduction:
 * 1. If x > 2.414, atan(x) = pi/2 - atan(1/x)
 * 2. If x > 0.414, atan(x) = pi/4 + atan((x-1)/(x+1))
 */
static double _atan_kernel(double x) {
    double z, p;

    if(x > 2.414213562373095) {
        return M_PI_2 - _atan_kernel(1.0 / x);
    }

    if(x > 0.414213562373095) {
        return M_PI_4 + _atan_kernel((x - 1.0) / (x + 1.0));
    }

    /* Polynomial evaluation for small x (Horner's method) */
    z = x * x;
    p = at11;
    p = at9 + z * p;
    p = at7 + z * p;
    p = at5 + z * p;
    p = at3 + z * p;
    p = at1 + z * p;

    return x * p;
}

double atan(double x) {
    double res;
    /* Handle NaN */
    if(isnan(x))
        return x;

    res = _atan_kernel(fabs(x));
    return (x < 0) ? -res : res;
}

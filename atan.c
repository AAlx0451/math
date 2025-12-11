/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Academic Source:
 * Hart & Cheney #4941 for Atan.
 */

#include <math.h>
#include <errno.h>

static const double M_PI = 3.14159265358979323846;
static const double M_PI_2 = 1.57079632679489661923;

/* H&C #4941 Coefficients */
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
    /* Minimax polynomial for x in [0, 1] */
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
    if (x == 0.0) {
        if (y == 0.0) return 0.0; /* Implementation defined, commonly 0 */
        return (y > 0) ? M_PI_2 : -M_PI_2;
    }
    
    double z = y / x;
    double res = atan(fabs(z)); // Always positive result 0..pi/2

    if (x > 0.0) {
        return (y < 0.0) ? -res : res;
    } else {
        /* x < 0 */
        return (y < 0.0) ? -M_PI + res : M_PI - res;
    }
}

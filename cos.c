/*
 * Part of PD LibM
 */

#include <errno.h>
#include <math.h>
#pragma clang diagnostic ignored "-Wreserved-identifier"

/* Reduction constants */
static const double C1 = 1.57079632673412561417e+00;
static const double C2 = 6.07710050650619224932e-11;
static const double C3 = 1.90821492927058770002e-20;
static const double M_2_PI = 0.63661977236758134308;

/* Sin Kernel Coefficients */
static const double
    S1 = -1.66666666666666324348e-01,
    S2 = 8.33333333332248946124e-03,
    S3 = -1.98412698298579493134e-04,
    S4 = 2.75573137070700676789e-06,
    S5 = -2.50507602534068634195e-08,
    S6 = 1.58969099521155010221e-10;

/* Cos Kernel Coefficients */
static const double
    C1_c = -4.99999999999999888889e-01,
    C2_c = 4.16666666666666019037e-02,
    C3_c = -1.38888888888741095749e-03,
    C4_c = 2.48015872894767294178e-05,
    C5_c = -2.75573143513906633035e-07,
    C6_c = 2.08757232129817482790e-09;

static inline double _sin_kernel(double x) {
    double z = x * x;
    double r = S6;
    r = S5 + z * r;
    r = S4 + z * r;
    r = S3 + z * r;
    r = S2 + z * r;
    r = S1 + z * r;
    return x + x * z * r;
}

static inline double _cos_kernel(double x) {
    double z = x * x;
    double r = C6_c;
    r = C5_c + z * r;
    r = C4_c + z * r;
    r = C3_c + z * r;
    r = C2_c + z * r;
    r = C1_c + z * r;
    return 1.0 + z * r;
}

double cos(double x) {
    double y, n, r, res;
    int quad;
    if(!isfinite(x)) {
        errno = EDOM;
        return NAN;
    }

    y = fabs(x);
    if(y > 1.0e9) {
        errno = EDOM;
        return 0.0;
    }

    n = floor(y * M_2_PI + 0.5);
    r = (y - n * C1) - n * C2 - n * C3;
    quad = (int)((int64_t)n & 3);

    /*
     * cos(n * pi/2 + r)
     * n=0: cos(r)       -> cos kernel
     * n=1: cos(pi/2+r)  -> -sin(r)
     * n=2: cos(pi+r)    -> -cos(r)
     * n=3: cos(3pi/2+r) -> sin(r)
     */

    switch(quad) {
    case 0:
        res = _cos_kernel(r);
        break;
    case 1:
        res = -_sin_kernel(r);
        break;
    case 2:
        res = -_cos_kernel(r);
        break;
    default:
        res = _sin_kernel(r);
        break; /* case 3 */
    }

    /* cos(x) is even, so sign of x doesn't matter for the logic above */
    return res;
}

/*
 * Part of PD LibM
 */

#include <math.h>
#include <errno.h>
#pragma clang diagnostic ignored "-Wreserved-identifier"

/* Cody-Waite constants for Pi/2 reduction */
static const double C1 = 1.57079632673412561417e+00;
static const double C2 = 6.07710050650619224932e-11;
static const double C3 = 1.90821492927058770002e-20;
static const double M_2_PI = 0.63661977236758134308;

/* 
 * Sin Kernel Coefficients (Radians)
 * Range: [-pi/4, +pi/4]
 * sin(x) ~ x + S1*x^3 + S2*x^5 ...
 */
static const double
    S1 = -1.66666666666666324348e-01,
    S2 = 8.33333333332248946124e-03,
    S3 = -1.98412698298579493134e-04,
    S4 = 2.75573137070700676789e-06,
    S5 = -2.50507602534068634195e-08,
    S6 = 1.58969099521155010221e-10;

/* 
 * Cos Kernel Coefficients (Radians)
 * Range: [-pi/4, +pi/4]
 * cos(x) ~ 1 + C1*x^2 + C2*x^4 ...
 */
static const double
    C1_c = -4.99999999999999888889e-01, /* Name changed to avoid collision with C1 reduction const */
    C2_c = 4.16666666666666019037e-02,
    C3_c = -1.38888888888741095749e-03,
    C4_c = 2.48015872894767294178e-05,
    C5_c = -2.75573143513906633035e-07,
    C6_c = 2.08757232129817482790e-09;

/* Kernel: sin(x) approx x + x^3 * P(x^2) */
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

/* Kernel: cos(x) approx 1 + x^2 * P(x^2) */
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

double sin(double x) {
    double y, n, r, res;
    int quad;
    if(!isfinite(x)) {
        errno = EDOM;
        return NAN;
    }
    if(x == 0.0)
        return x;

    y = fabs(x);

    /* 
     * Argument Reduction.
     * For y > 1e9, precision is lost entirely without 1024-bit pi.
     * Returning 0.0 or error is safer than returning garbage.
     */
    if(y > 1.0e9) {
        errno = EDOM;
        return 0.0;
    }

    /* n = nearest integer to y / (pi/2) */
    n = floor(y * M_2_PI + 0.5);

    /* r = y - n * (pi/2) in high precision */
    r = (y - n * C1) - n * C2 - n * C3;

    /* 
     * Quadrant determination.
     * n is double, but fits in int64 for reasonable inputs (< 1e9).
     * We need n mod 4.
     */
    quad = (int)((int64_t)n & 3);

    /* Fix sign of r if x was negative? 
     * Actually, better to solve for positive y, then fix sign of result.
     * sin(-x) = -sin(x).
     */

    /*
     * sin(n * pi/2 + r)
     * n=0: sin(r)       -> sin kernel
     * n=1: sin(pi/2+r)  -> cos(r) -> cos kernel
     * n=2: sin(pi+r)    -> -sin(r) -> sin kernel (neg)
     * n=3: sin(3pi/2+r) -> -cos(r) -> cos kernel (neg)
     */

    if(quad == 0) {
        res = _sin_kernel(r);
    } else if(quad == 1) {
        res = _cos_kernel(r);
    } else if(quad == 2) {
        res = -_sin_kernel(r);
    } else { /* quad == 3 */
        res = -_cos_kernel(r);
    }

    if(x < 0)
        res = -res;

    return res;
}

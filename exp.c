/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

/*
 * Academic Source:
 * Standard Range Reduction + Taylor Series up to degree 12.
 * exp(x) = 2^k * exp(r)
 *
 * Degree 12 is required to keep error below 1e-16 for |r| < ln(2)/2.
 */

#include <errno.h>
#include <math.h>

static const double LN2_HI = 6.93147180369123816490e-01;
static const double LN2_LO = 1.90821492927058770002e-10;
static const double INV_LN2 = 1.44269504088896340736;

/* Inverse factorial coefficients for Taylor series: 1/n! */
static const double
    P3 = 1.66666666666666666667e-01,  /* 1/6 */
    P4 = 4.16666666666666666667e-02,  /* 1/24 */
    P5 = 8.33333333333333333333e-03,  /* 1/120 */
    P6 = 1.38888888888888888889e-03,  /* 1/720 */
    P7 = 1.98412698412698412698e-04,  /* 1/5040 */
    P8 = 2.48015873015873015873e-05,  /* 1/40320 */
    P9 = 2.75573192239858906526e-06,  /* 1/362880 */
    P10 = 2.75573192239858906526e-07, /* 1/3628800 */
    P11 = 2.50521083854417187751e-08, /* 1/39916800 */
    P12 = 2.08767569878680989792e-09; /* 1/479001600 */

double exp(double x) {
    int k;
    double z, r, p, r2, result;
    if(isnan(x))
        return x;
    if(isinf(x))
        return (x > 0) ? x : 0.0;

    /* Boundary check: exp(709.78) overflows double */
    if(x > 709.78) {
        errno = ERANGE;
        return HUGE_VAL;
    }

    if(x < -745.0) {
        /* Underflow to true zero */
        return 0.0;
    }

    /* Range reduction: k = round(x / ln2) */
    z = floor(x * INV_LN2 + 0.5);
    k = (int)z;

    /* r = x - k * ln2 */
    /* Calculated with extra precision using LN2_HI/LO to minimize error */
    r = (x - z * LN2_HI) - z * LN2_LO;

    /*
     * Horner's scheme for exp(r) - 1 - r - r^2/2
     * Poly = r^3/6 + ... + r^12/12!
     */
    p = P12;
    p = P11 + r * p;
    p = P10 + r * p;
    p = P9 + r * p;
    p = P8 + r * p;
    p = P7 + r * p;
    p = P6 + r * p;
    p = P5 + r * p;
    p = P4 + r * p;
    p = P3 + r * p;

    /*
     * Reconstruction:
     * exp(r) = 1 + r + r^2/2 + (r^3 * p)
     */
    r2 = r * r;
    result = 1.0 + r + (r2 * 0.5) + (r2 * r * p);

    /* Reconstruct 2^k * result using ldexp (fast bit manip) */
    return ldexp(result, k);
}

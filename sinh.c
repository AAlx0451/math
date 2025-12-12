/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Academic Source:
 * Hart & Cheney, "Computer Approximations", 1968.
 * Approximation #2922.
 *
 * IEEE 754 updates:
 * - Correct handling of signed zeros (-0.0 -> -0.0)
 * - Handling of the overflow gap between exp(x) and sinh(x)
 */

#include <math.h>
#include <errno.h>

static const double 
p0 = -0.19036136208630046e5,
p1 = -0.21838637775510662e4,
p2 = -0.54877395048261805e2,
q0 = -0.19036136208630046e5,
q1 =  0.98888060867727498e3,
q2 = -0.15852528771383321e2;

/* log(DBL_MAX_VALUE) */
static const double o_threshold = 7.09782712893383973096e+02;
/* log(DBL_MAX_VALUE) + log(2) */
static const double sh_max = 7.10475860073943977113e+02; 
/* ln(2) */
static const double ln2 = 0.693147180559945309417;

double sinh(double x) {
    /* IEEE 754: sinh(NaN) = NaN, sinh(Inf) = Inf */
    if (!isfinite(x)) {
        return x; /* Returns x (including NaN and Inf with sign) */
    }

    double ax = fabs(x);
    double h;

    /* 
     * IEEE 754: sinh(+-0) = +-0. 
     * Logic handles this naturally, but explicit check optimizes.
     * Also handles underflow range.
     */
    if (ax < 1.0) {
        if (ax < 1e-10) return x; 
        
        double x2 = x * x;
        double num = (p2 * x2 + p1) * x2 + p0;
        double den = ((x2 + q2) * x2 + q1) * x2 + q0;
        return x * (num / den);
    }

    /* Range check for overflow */
    if (ax > sh_max) {
        errno = ERANGE;
        return (x > 0) ? HUGE_VAL : -HUGE_VAL;
    }

    /* 
     * Calculation logic:
     * 1. Normal range: sinh(x) = (e^x - 1/e^x) / 2
     * 2. Large range:  sinh(x) ~= e^x / 2
     * 3. Critical range (near overflow): 
     *    exp(x) would overflow, but result is finite.
     *    Use exp(x - ln2) which equals e^x / 2 directly.
     */
    
    if (ax < o_threshold) {
        double e = exp(ax);
        h = 0.5 * (e - 1.0 / e);
    } else {
        /* 
         * x is in [709.78, 710.47]. 
         * direct exp(x) overflows, but sinh(x) is valid.
         * sinh(x) approx exp(x)/2 = exp(x - ln2)
         */
        h = exp(ax - ln2);
    }

    return (x < 0) ? -h : h;
}

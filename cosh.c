/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Academic Source:
 * Hart & Cheney, "Computer Approximations", 1968.
 *
 * IEEE 754 updates:
 * - Handling of the overflow gap between exp(x) and cosh(x)
 */

#include <errno.h>
#include <math.h>

/* log(DBL_MAX_VALUE) */
static const double o_threshold = 7.09782712893383973096e+02;
/* log(DBL_MAX_VALUE) + log(2) */
static const double ch_max = 7.10475860073943977113e+02;
/* ln(2) */
static const double ln2 = 0.693147180559945309417;

double cosh(double x) {
    double h, ax, e;

    /* IEEE 754: cosh(NaN) = NaN, cosh(+-Inf) = +Inf */
    if(!isfinite(x)) {
        if(isnan(x))
            return x;
        return HUGE_VAL;
    }

    ax = fabs(x);

    if(ax > ch_max) {
        errno = ERANGE;
        return HUGE_VAL;
    }

    /* 
     * |x| < 22.0: Use full formula (e^x + e^-x)/2
     * 22.0 <= |x| < o_threshold: e^-x is negligible, use exp(x)/2
     * |x| >= o_threshold: Use exp(x - ln2) to avoid intermediate overflow
     */

    if(ax < 22.0) {
        e = exp(ax);
        h = 0.5 * (e + 1.0 / e);
    } else if(ax < o_threshold) {
        h = 0.5 * exp(ax);
    } else {
        /* Critical range: [709.78, 710.47] */
        h = exp(ax - ln2);
    }

    return h;
}

/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Academic Source:
 * Hart & Cheney #2705.
 */

#include <errno.h>
#include <math.h>

static const double LOG2 = 0.693147180559945309417;

static const double
    lp0 = -0.240139179559210510e2,
    lp1 = 0.309572928215376501e2,
    lp2 = -0.963769093368686593e1,
    lp3 = 0.421087371217979714e0,
    lq0 = -0.120069589779605255e2,
    lq1 = 0.194809660700889731e2,
    lq2 = -0.891110902798312337e1;

double log(double x) {
    int k;
    double f, z, z2, num, den;
    if(x <= 0.0) {
        if(x == 0.0) {
            errno = ERANGE;
            return -HUGE_VAL;
        }
        errno = EDOM;
        return NAN;
    }
    if(!isfinite(x))
        return x;

    f = frexp(x, &k); /* x = f * 2^k, f in [0.5, 1) */

    /* Optimization for continuity around 1.0/sqrt(2) */
    if(f < 0.707106781186547524) {
        f *= 2.0;
        k--;
    }

    z = (f - 1.0) / (f + 1.0);
    z2 = z * z;
    num = ((lp3 * z2 + lp2) * z2 + lp1) * z2 + lp0;
    den = ((z2 + lq2) * z2 + lq1) * z2 + lq0;

    return k * LOG2 + z * (num / den);
}

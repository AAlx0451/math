/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

/*
 * Academic Source:
 * Standard Range Reduction + Minimax/Pade.
 * exp(x) = 2^k * exp(r)
 */

#include <math.h>
#include <errno.h>
#include <stdint.h>

static const double LN2_HI = 6.93147180369123816490e-01;
static const double LN2_LO = 1.90821492927058770002e-10;
static const double INV_LN2 = 1.44269504088896340736;

/* Pade coefficients for exp(r) where |r| < ln2/2 */
static const double
P1 = 1.66666666666666019037e-01,
P2 = -2.77777777770155933842e-03,
P3 = 6.61375632143793436117e-05,
P4 = -1.65339022054652515390e-06,
P5 = 4.13813679705723846039e-08;

double exp(double x) {
    if (isnan(x)) return x;
    if (x == HUGE_VAL) return x;
    if (x == -HUGE_VAL) return 0.0;

    /* Boundary check roughly log(DBL_MAX) */
    if (x > 709.78) {
        errno = ERANGE;
        return HUGE_VAL;
    }
    
    if (x < -745.0) {
        errno = ERANGE;
        return 0.0;
    }

    /* Range reduction: k = round(x / ln2) */
    int k = (int)floor(x * INV_LN2 + 0.5);
    double r = x - k * LN2_HI - k * LN2_LO;

    /* Pade approximation exp(r) ~ 1 + 2r / (2 - r + r^2 * P(r^2)) */
    double r2 = r * r;
    double p = P5;
    p = P4 + r2 * p;
    p = P3 + r2 * p;
    p = P2 + r2 * p;
    p = P1 + r2 * p;

    double c = r - r2 * p;
    double result = 1.0 + ((r * c) / (2.0 - c) * 2.0 + r);

    /* Reconstruct 2^k * result using ldexp (fast bit manip) */
    return ldexp(result, k);
}

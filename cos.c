/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Academic Source:
 * Hart & Cheney, "Computer Approximations", 1968.
 * Approximation #3370.
 */

#include <math.h>
#include <errno.h>
#include <stdint.h>

static const double
p0 =  0.1357884097877375669092680e8,
p1 = -0.4942908100902844161158627e7,
p2 =  0.4401030535375266501944918e6,
p3 = -0.1384727249982452873054457e5,
p4 =  0.1459688406665768722226959e3,
q0 =  0.8644558652922534429915149e7,
q1 =  0.4081792252343299749395779e6,
q2 =  0.9463096101538208180571257e4,
q3 =  0.1326534908786136358911494e3;

static const double two_over_pi = 0.63661977236758134308;
static const double c1 = 1.57079632673412561417e00;
static const double c2 = 6.07710050650619224932e-11;
static const double c3 = 1.90821492927058770002e-20;

double cos(double x) {
    if (!isfinite(x)) {
        errno = EDOM;
        return NAN;
    }
    
    double y = fabs(x);
    int n = (int)(y * two_over_pi + 0.5);
    double r;

    /* Cody-Waite range reduction */
    if (n < 32768) {
        r = (y - n * c1) - n * c2 - n * c3;
    } else {
        double fn = floor(y * two_over_pi + 0.5);
        r = y - fn * 1.57079632679489661923;
        n = (int)fn;
    }

    if (x < 0) r = -r;

    /* 
     * cos(x) is essentially sin(x + pi/2). 
     * We add 1 to the quadrant (n + 1).
     */
    int quad = (n + 1) & 3;
    double res;

    double x2 = r * r;
    double num = ((p4 * x2 + p3) * x2 + p2) * x2 + p1;
    num = num * x2 + p0;
    double den = ((x2 + q3) * x2 + q2) * x2 + q1;
    den = den * x2 + q0;
    res = r * (num / den);

    /* Cosine adjustment logic (inverted here relative to sin.c because we shifted quad) */
    if (quad == 1 || quad == 3) {
        double r_co = 1.57079632679489661923 - fabs(r);
        double x2_co = r_co * r_co;
        double num_co = ((p4 * x2_co + p3) * x2_co + p2) * x2_co + p1;
        num_co = num_co * x2_co + p0;
        double den_co = ((x2_co + q3) * x2_co + q2) * x2_co + q1;
        den_co = den_co * x2_co + q0;
        res = r_co * (num_co / den_co);
        if (r < 0) res = res;
    }

    if (quad > 1) res = -res;

    return res;
}

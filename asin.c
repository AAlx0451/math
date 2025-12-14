/*
 * Part of PD LibM
 */

#include "math_private.h"
#include <errno.h>

static const double M_PI_2 = 1.57079632679489661923;

static double _asin_kernel(double x) {
    double z = x * x;
    return x * (1.0 + z * (0.16666666666666666 + z * (0.075 + z * (0.0446428571428571 + z * 0.0303819444444444))));
}

double asin(double x) {
    if (x < -1.0 || x > 1.0) {
        errno = EDOM;
        return NAN;
    }

    int sign = (x < 0);
    double absx = fabs(x);
    double res;

    if (absx < 0.5) {
        res = _asin_kernel(absx);
    } else {
        /* Use identity: asin(x) = pi/2 - 2*asin(sqrt((1-x)/2)) */
        /* This handles the edge near 1.0 gracefully */
        double z = sqrt(0.5 * (1.0 - absx));
        res = M_PI_2 - 2.0 * _asin_kernel(z);
    }

    return sign ? -res : res;
}

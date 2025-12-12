/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

#include <math.h>
#include <errno.h>

double pow(double x, double y) {
    if (y == 0.0) return 1.0;
    if (x == 1.0) return 1.0;
    if (isnan(x) || isnan(y)) return NAN;

    /* IEEE 754: Handling of infinite y */
    if (isinf(y)) {
        if (x == -1.0) return 1.0;
        
        double absx = fabs(x);
        if (absx < 1.0) return (y > 0.0) ? 0.0 : HUGE_VAL;
        if (absx > 1.0) return (y > 0.0) ? HUGE_VAL : 0.0;
    }

    /* Handle integer y cases for negative x */
    double y_int;
    int is_int = (modf(y, &y_int) == 0.0);
    int odd_int = is_int && ((long long)y_int % 2 != 0);

    if (x == 0.0) {
        if (y < 0.0) {
            errno = ERANGE;
            return odd_int ? (signbit(x) ? -HUGE_VAL : HUGE_VAL) : HUGE_VAL;
        }
        /* +0 or -0 */
        return odd_int ? (signbit(x) ? -0.0 : 0.0) : 0.0;
    }

    if (x < 0.0) {
        if (!is_int) {
            errno = EDOM;
            return NAN;
        }
        double res = exp(y * log(-x));
        return odd_int ? -res : res;
    }

    return exp(y * log(x));
}

/*
 * Part of PD LibM
 */

#include "math_private.h"
#include <errno.h>

double pow(double x, double y) {
    /* Special cases */
    if (y == 0.0) return 1.0;
    if (x == 1.0) return 1.0;
    if (isnan(x) || isnan(y)) return NAN;

    if (isinf(y)) {
        if (x == -1.0) return 1.0;
        double absx = fabs(x);
        if (absx < 1.0) return (y > 0.0) ? 0.0 : HUGE_VAL;
        if (absx > 1.0) return (y > 0.0) ? HUGE_VAL : 0.0;
    }

    /* 
     * Integer check for negative bases.
     * Logic: If x < 0, we need to know if y is an odd integer to determine sign.
     */
    double y_int;
    int is_int = (modf(y, &y_int) == 0.0);
    int odd_int = 0;

    if (is_int) {
        /* 
         * Check parity. 
         * Doubles larger than 2^53 are always even integers (precision step >= 2.0).
         * INT64_MAX is ~9e18. We guard against UB in casting.
         */
        if (fabs(y) < 9007199254740992.0) { /* 2^53 */
             odd_int = ((int64_t)y_int % 2 != 0);
        } else {
             /* Larger values are always even */
             odd_int = 0;
        }
    }

    if (x == 0.0) {
        if (y < 0.0) {
            errno = ERANGE;
            return odd_int ? (signbit(x) ? -HUGE_VAL : HUGE_VAL) : HUGE_VAL;
        }
        return odd_int ? (signbit(x) ? -0.0 : 0.0) : 0.0;
    }

    if (x < 0.0) {
        if (!is_int) {
            errno = EDOM;
            return NAN;
        }
        /* pow(-x, y) = pow(x, y) * (-1)^y */
        /* We compute pow(|x|, y) and fix sign at the end */
        x = -x;
    } else {
        odd_int = 0; /* No sign flip needed for positive x */
    }

    /* 
     * Core Algorithm: exp(y * log(x))
     * Relying on system log/exp for now. 
     * Note: For strict 0.5 ULP pow, a multi-precision kernel is required.
     */
    
    double l = log(x);
    double p = y * l;

    double res = exp(p);
    
    return odd_int ? -res : res;
}

/*
 * Part of PD LibM
 */

#include <math.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#pragma clang diagnostic ignored "-Wreserved-identifier"

/* Helper for integer exponentiation by squaring (Binary Exponentiation) */
static double _pow_int(double b, int e) {
    double res = 1.0;
    int exp = (e < 0) ? -e : e;

    while(exp > 0) {
        if(exp & 1)
            res *= b;
        b *= b;
        exp >>= 1;
    }
    return (e < 0) ? (1.0 / res) : res;
}

double pow(double x, double y) {
    double y_int_part, res;
    int y_is_int, odd_int;
    /* 1. Special cases */
    if(y == 0.0)
        return 1.0;
    if(x == 1.0)
        return 1.0;
    if(isnan(x) || isnan(y))
        return (double)NAN;

    /* 2. Check integer status */
    y_is_int = (modf(y, &y_int_part) == 0.0);

    /* 3. Handle Infinity Exponents */
    if(isinf(y)) {
        double ax = fabs(x);
        if(ax == 1.0)
            return 1.0; /* (-1)^Inf -> 1 */
        if(y > 0)
            return (ax > 1.0) ? HUGE_VAL : 0.0;
        else
            return (ax > 1.0) ? 0.0 : HUGE_VAL;
    }

    /* 
     * 4. Integer Exponent Optimization 
     * If y is a safe integer, use binary exponentiation for exact results.
     * This fixes precision issues like pow(2, 3) != 8.0
     */
    if(y_is_int && y > INT_MIN && y < INT_MAX) {
        return _pow_int(x, (int)y);
    }

    /* 5. Handle x = 0 */
    if(x == 0.0) {
        if(y < 0.0) {
            errno = ERANGE;
            odd_int = y_is_int && ((int64_t)y_int_part % 2 != 0);
            return odd_int ? (copysign(HUGE_VAL, x)) : HUGE_VAL;
        }
        odd_int = y_is_int && ((int64_t)y_int_part % 2 != 0);
        return odd_int ? x : 0.0;
    }

    /* 6. Handle Negative Base */
    if(x < 0.0) {
        if(!y_is_int) {
            errno = EDOM;
            return (double)NAN;
        }
        /* x^y = (-1)^y * |x|^y */
        odd_int = ((int64_t)y_int_part % 2 != 0);
        res = exp(y * log(-x));
        return odd_int ? -res : res;
    }

    /* 7. General case: exp(y * log(x)) */
    return exp(y * log(x));
}

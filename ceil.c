/*
 * Part of PD LibM
 */

#include "math_private.h"
#include <math.h>

double ceil(double x) {
    uint64_t i, m;
    double r;
    int e;
    EXTRACT_WORD64(i, x);

    /* Extract exponent: bits 62-52 */
    e = (int)((i >> 52) & 0x7FF) - 1023;

    /* Case 1: Exponent is large (>= 52). Number is already integer. */
    if(e >= 52)
        return x;

    /* Case 2: Exponent is negative. 0 < |x| < 1. */
    if(e < 0) {
        /* If x is 0.0 or -0.0, return x */
        if((i & 0x7FFFFFFFFFFFFFFFULL) == 0)
            return x;

        /* If x > 0, ceil is 1.0 */
        if((i >> 63) == 0)
            return 1.0;

        /* If x < 0, ceil is -0.0 */
        return -0.0;
    }

    /* Case 3: Normal range with fractional part */
    m = 0xFFFFFFFFFFFFFFFFULL >> (12 + e);

    /* If fractional bits are already zero, return x */
    if((i & m) == 0)
        return x;

    /* If positive, we truncate and add 1.0.
     * If negative, we just truncate (which moves towards 0, i.e., up for negatives).
     */
    if((i >> 63) == 0) {
        /* Positive: Mask fraction, implies floor. Add 1.0 safely */
        /* It is safer to use float math for the increment to handle carry */
        i &= ~m;
        INSERT_WORD64(r, i);
        return r + 1.0;
    } else {
        /* Negative: Mask fraction (truncation) is ceil */
        i &= ~m;
        INSERT_WORD64(r, i);
        return r;
    }
}

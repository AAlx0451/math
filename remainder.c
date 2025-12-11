/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Source: 
 * Standard definition of IEEE 754 remainder.
 * r = x - n*y, where n is round_half_to_even(x/y).
 */

#include <math.h>

double remainder(double x, double y) {
    if (isnan(x) || isnan(y)) return NAN;
    if (y == 0.0) return NAN;
    if (!isfinite(x)) return NAN; /* x is Inf */
    
    /* Argument reduction using fmod to bring x near 0 */
    double rem = fmod(x, y);
    
    if (rem == 0.0) return 0.0; // Keep sign of x usually, but strictly IEEE implies 0

    double abs_rem = fabs(rem);
    double abs_y = fabs(y);

    /* If remainder > y/2, we went to the "wrong" integer in fmod (truncation) */
    if (abs_rem > 0.5 * abs_y) {
        if (rem > 0) rem -= abs_y;
        else         rem += abs_y;
    }
    /* Tie-breaking: if exactly halfway, round to even quotient */
    else if (abs_rem == 0.5 * abs_y) {
        /* Determine quotient parity */
        double quo = x / y;
        double nearest = round(quo);
        double diff = fabs(quo - nearest);
        
        /* If round() rounded to odd, but we are halfway, check parity */
        /* Actually simpler logic: quotient from fmod is (x-rem)/y */
        double k = (x - rem) / y;
        
        /* If k is odd integer (checking via modulo 2 approx) */
        /* We can check if k/2 is integer */
        double half_k = k * 0.5;
        if (floor(half_k) != half_k) {
             /* k is odd, adjust to make it even */
             if (rem > 0) rem -= abs_y;
             else         rem += abs_y;
        }
    }
    
    return rem;
}

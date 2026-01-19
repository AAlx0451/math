/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <math.h>
#include <float.h>

double remainder(double x, double y) {
    if (isnan(x) || isnan(y)) return NAN;
    if (y == 0.0) return NAN;
    if (!isfinite(x)) return NAN; /* x is Inf */
    
    /* Argument reduction using fmod to bring x near 0 */
    double rem = fmod(x, y);
    
    if (rem == 0.0) return rem; // Keep sign of x

    double abs_rem = fabs(rem);
    double abs_y = fabs(y);

    /* If remainder > y/2, we went to the "wrong" integer in fmod (truncation) */
    if (abs_rem > 0.5 * abs_y) {
        if (rem > 0) rem -= abs_y;
        else         rem += abs_y;
    }
    /* Tie-breaking: if exactly halfway, round to even quotient */
    else if (fabs(abs_rem - (0.5 * abs_y)) < DBL_EPSILON) {
        /* Determine quotient parity */	
        double k = (x - rem) / y;
        if (fmod(fabs(k), 2.0) == 1.0) rem += (rem > 0) ? -abs_y : abs_y;
    }
    
    return rem;
}

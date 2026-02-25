/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

/*
 * Source:
 * Standard definition: x - trunc(x/y) * y
 */

#include <math.h>

double fmod(double x, double y) {
    double div, n;
    if(isnan(x) || isnan(y) || isinf(x) || y == 0.0) {
        return NAN;
    }
    if(x == 0.0)
        return x;

    div = x / y;
    if(!isfinite(div))
        return x;

    n = trunc(div);

    return x - n * y;
}

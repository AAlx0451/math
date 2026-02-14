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
    if(isnan(x) || isnan(y) || isinf(x) || y == 0.0) {
        return NAN;
    }
    if(x == 0.0)
        return x;

    double div = x / y;
    if(!isfinite(div))
        return x;

    double n = trunc(div);

    return x - n * y;
}

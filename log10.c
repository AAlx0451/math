/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

#include <math.h>

static const double LN10_INV = 0.43429448190325182765;

double log10(double x) {
    return log(x) * LN10_INV;
}

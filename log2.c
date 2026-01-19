/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#include <math.h>

static const double LOG2 = 0.693147180559945309417;

double log2(double x) {
    return log(x) / LOG2;
}

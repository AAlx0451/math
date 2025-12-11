/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Academic Source:
 * Hart & Cheney, "Computer Approximations", 1968.
 * Approximation #4285 (Tan).
 */

#include <math.h>
#include <errno.h>

static const double invpi = 0.31830988618379067154; /* 1/pi */
static const double p0 = -0.1306820264754825668269611177e+5;
static const double p1 =  0.1055970901714953193602353981e+4;
static const double p2 = -0.1550685653483266376941705728e+2;
static const double p3 =  0.3422554387241003435328470489e-1;
static const double p4 =  0.3386638642677172096076369e-4;
static const double q0 = -0.1663895238947119001851464661e+5;
static const double q1 =  0.4765751362916483698926655581e+4;
static const double q2 = -0.1555033164031709966900124574e+3;

/* Cody-Waite Constants */
static const double c1 = 1.57079632673412561417e+00;
static const double c2 = 6.07710050650619224932e-11;
static const double c3 = 1.90821492927058770002e-20;
static const double M_PI_2 = 1.57079632679489661923;

double tan(double x) {
    if (!isfinite(x)) {
        errno = EDOM;
        return NAN;
    }
    if (x == 0.0) return x;

    /* Argument Reduction: x = n * pi/2 + r */
    double n_float = round(x / M_PI_2);
    int n = (int)n_float;
    double r = (x - n_float * c1) - n_float * c2 - n_float * c3;

    /* Hart & Cheney #4285 on reduced range */
    double x2 = r * r;
    double num = (((p4 * x2 + p3) * x2 + p2) * x2 + p1) * x2 + p0;
    double den = ((x2 + q2) * x2 + q1) * x2 + q0;
    double temp = r * (num / den);

    /* If odd quadrant, return -1/tan(r) (cotangent) */
    if (n & 1) {
        if (temp == 0.0) {
            errno = ERANGE;
            return (x > 0) ? HUGE_VAL : -HUGE_VAL; /* Approximation of overflow */
        }
        return -1.0 / temp;
    }

    return temp;
}

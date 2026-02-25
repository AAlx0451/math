/*
 * Part of PD LibM
 */

#include <errno.h>
#include <math.h>

#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

static const double C1 = 1.57079632673412561417e+00;
static const double C2 = 6.07710050650619224932e-11;
static const double C3 = 1.90821492927058770002e-20;
static const double M_2_PI = 0.63661977236758134308;

/*
 * Tan Kernel Coefficients (Radians)
 * Range: [-pi/4, +pi/4]
 * tan(x) ~ x + T1*x^3 + T2*x^5 ...
 */
static const double
    T1 = 0.333333333333334091986,
    T2 = 0.133333333333201242699,
    T3 = 0.0539682539762265521083,
    T4 = 0.0218694882948595424599,
    T5 = 0.00886323982359975800586,
    T6 = 0.00359207910759131235356;

static inline double _tan_kernel(double x) {
    double z = x * x;
    double r = T6;
    r = T5 + z * r;
    r = T4 + z * r;
    r = T3 + z * r;
    r = T2 + z * r;
    r = T1 + z * r;
    return x + x * z * r;
}

double tan(double x) {
    double n, r, t;
    int64_t n_int;
    if(!isfinite(x)) {
        errno = EDOM;
        return NAN;
    }
    if(x == 0.0)
        return x;

    if(fabs(x) > 1.0e9) {
        errno = EDOM;
        return 0.0;
    }

    n = floor(x * M_2_PI + 0.5);
    r = (x - n * C1) - n * C2 - n * C3;

    n_int = (int64_t)n;

    /*
     * tan(x) = tan(n*pi/2 + r)
     * If n is even: tan(r)
     * If n is odd:  tan(pi/2 + r) = -cot(r) = -1/tan(r)
     */

    t = _tan_kernel(r);

    if(n_int & 1) {
        /* Cotangent case */
        if(t == 0.0) {
            /* r was 0, so x was n*pi/2 (vertical asymptote) */
            errno = ERANGE;
            return (x > 0) ? HUGE_VAL : -HUGE_VAL; /* Sign is technically ill-defined here but Inf is required */
        }
        return -1.0 / t;
    }

    return t;
}

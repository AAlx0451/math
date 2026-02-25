/*
 * Part of PD LibM
 * Originally made for Small-LibC
 */

/*
 * Academic Source:
 * Hart & Cheney, "Computer Approximations", 1968.
 * Approximation #3522.
 */

#include <math.h>

static const double
    th_p0 = -0.56386340242137666e5,
    th_p1 = -0.59023193616603091e4,
    th_p2 = -0.14589255269781878e3,
    th_q0 = -0.56386340242137666e5,
    th_q1 = 0.12894101140939525e5,
    th_q2 = -0.76019685324021202e3;

double tanh(double x) {
    double ax, x2, num, den, e2x, res;
    /* IEEE 754: tanh(NaN) = NaN, tanh(+-Inf) = +-1 */
    if(!isfinite(x)) {
        if(isnan(x))
            return x;
        return (x > 0) ? 1.0 : -1.0;
    }

    ax = fabs(x);

    /*
     * Range reduction:
     * If |x| > 19, tanh(x) is essentially 1.0 (in double precision).
     * Also protects against overflow in exp(2*x).
     */
    if(ax > 19.0) {
        return (x > 0) ? 1.0 : -1.0;
    }

    /*
     * Small x (0 <= |x| <= 0.57):
     * Use Hart & Cheney #3522.
     * tanh(x) = x * P(x^2) / Q(x^2).
     *
     * Note on Signed Zero:
     * If x is -0.0, formula results in -0.0 * (...), which returns -0.0.
     * Compliant.
     */
    if(ax <= 0.57) {
        if(ax < 1e-10)
            return x;

        x2 = x * x;
        num = (th_p2 * x2 + th_p1) * x2 + th_p0;
        den = ((x2 + th_q2) * x2 + th_q1) * x2 + th_q0;

        return x * (num / den);
    }

    /*
     * Medium x:
     * tanh(x) = (e^2x - 1) / (e^2x + 1) = 1 - 2 / (e^2x + 1)
     */
    e2x = exp(2.0 * ax);
    res = 1.0 - 2.0 / (e2x + 1.0);

    return (x < 0) ? -res : res;
}

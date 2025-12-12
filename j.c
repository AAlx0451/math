/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

/*
 * Academic Source:
 * Hart & Cheney: #6050, #6750, #7150, #5849, etc.
 */

#include <math.h>
#include <errno.h>

static const double tpi = 0.63661977236758134308;
static const double pio4 = 0.78539816339744830962;

/* J0 Coefficients (H&C) */
static const double j0_p[] = {
    0.4933787251794133561816813446e21,
   -0.1179157629107610536038440800e21,
    0.6382059341072356562289432465e19,
   -0.1367620353088171386865416609e18,
    0.1434354939140344111664316553e16,
   -0.8085222034853793871199468171e13,
    0.2507158285536881945555156435e11,
   -0.4050412371833132706360663322e8,
    0.2685786856980014981415848441e5
};
static const double j0_q[] = {
    0.4933787251794133562113278438e21,
    0.5428918384092285160200195092e19,
    0.3024635616709462698627330784e17,
    0.1127756739679798507056031594e15,
    0.3123043114941213172572469442e12,
    0.6699987672982239671814028660e9,
    0.1114636098462985378182402543e7,
    0.1363063652328970604442810507e4,
    1.0
};

/* Asymptotic Coefficients */
static const double as_p2[] = { 0.5393485083869438325262122897e7, 0.1233238476817638145232406055e8, 0.8413041456550439208464315611e7, 0.2016135283049983642487182349e7, 0.1539826532623911470917825993e6, 0.2485271928957404011288128951e4, 0.0 };
static const double as_q2[] = { 0.5393485083869438325560444960e7, 0.1233831022786324960844856182e8, 0.8426449050629797331554404810e7, 0.2025066801570134013891035236e7, 0.1560017276940030940592769933e6, 0.2615700736920839685159081813e4, 1.0 };
static const double as_p3[] = { -0.3984617357595222463506790588e4, -0.1038141698748464093880530341e5, -0.8239066313485606568803548860e4, -0.2365956170779108192723612816e4, -0.2262630641933704113967255053e3, -0.4887199395841261531199129300e1, 0.0 };
static const double as_q3[] = { 0.2550155108860942382983170882e6, 0.6667454239319826986004038103e6, 0.5332913634216897168722255057e6, 0.1560213206679291652539287109e6, 0.1570489191515395519392882766e5, 0.4087714673983499223402830260e3, 1.0 };

/* J1 Coefficients */
static const double j1_p[] = { 0.581199354001606143928050809e21, -0.6672106568924916298020941484e20, 0.2316433580634002297931815435e19, -0.3588817569910106050743641413e17, 0.2908795263834775409737601689e15, -0.1322983480332126453125473247e13, 0.3413234182301700539091292655e10, -0.4695753530642995859767162166e7, 0.2701122710892323414856790990e4 };
static const double j1_q[] = { 0.1162398708003212287858529400e22, 0.1185770712190320999837113348e20, 0.6092061398917521746105196863e17, 0.2081661221307607351240184229e15, 0.5243710262167649715406728642e12, 0.1013863514358673989967045588e10, 0.1501793594998585505921097578e7, 0.1606931573481487801970916749e4, 1.0 };

static void _asympt(double x, double *pzero, double *qzero) {
    double zsq = 64.0 / (x * x);
    double num = 0, den = 0;
    
    /* P2/Q2 */
    num = as_p2[0]; den = as_q2[0];
    for(int i=1; i<7; i++) { num = num*zsq + as_p2[i]; den = den*zsq + as_q2[i]; }
    *pzero = num / den;

    /* P3/Q3 */
    num = as_p3[0]; den = as_q3[0];
    for(int i=1; i<7; i++) { num = num*zsq + as_p3[i]; den = den*zsq + as_q3[i]; }
    *qzero = (8.0 / x) * (num / den);
}

double j0(double x) {
    double ax = fabs(x);
    if (ax > 8.0) {
        double pz, qz;
        _asympt(ax, &pz, &qz);
        double n = ax - pio4;
        return sqrt(tpi / ax) * (pz * cos(n) - qz * sin(n));
    }
    double xsq = x * x;
    double num = j0_p[0], den = j0_q[0];
    for (int i=1; i<9; i++) {
        num = num * xsq + j0_p[i];
        den = den * xsq + j0_q[i];
    }
    return num / den;
}

double j1(double x) {
    double ax = fabs(x);
    if (ax > 8.0) {
        double pz, qz;
        _asympt(ax, &pz, &qz);
        double n = ax - 3.0 * pio4;
        double res = sqrt(tpi / ax) * (pz * cos(n) - qz * sin(n));
        return (x < 0) ? -res : res;
    }
    double xsq = x * x;
    double num = j1_p[0], den = j1_q[0];
    for (int i=1; i<9; i++) {
        num = num * xsq + j1_p[i];
        den = den * xsq + j1_q[i];
    }
    return x * (num / den);
}

double jn(int n, double x) {
    if (n < 0) {
        n = -n;
        double res = jn(n, x);
        return (n & 1) ? -res : res;
    }
    if (n == 0) return j0(x);
    if (n == 1) return j1(x);
    if (x == 0.0) return 0.0;
    
    /* Standard recurrence relation */
    if (n > x) {
        /* Backward recursion for stability when n > x */
        double xsq = x * x;
        double t = 0;
        /* Continued fraction approximation guess */
        for(int i = n + 16; i > n; i--) t = xsq / (2.0 * i - t);
        t = x / (2.0 * n - t);
        
        double a = t;
        double b = 1.0;
        double temp;
        for (int i = n - 1; i > 0; i--) {
            temp = b;
            b = (2.0 * i / x) * b - a;
            a = temp;
        }
        return t * j0(x) / b;
    } else {
        /* Forward recursion */
        double a = j0(x);
        double b = j1(x);
        double temp;
        for (int i = 1; i < n; i++) {
            temp = b;
            b = (2.0 * i / x) * b - a;
            a = temp;
        }
        return b;
    }
}

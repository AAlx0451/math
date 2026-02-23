/*
 * Part of PD LibM
 * Internal helper macros and constants.
 */

#ifndef MATH_PRIVATE_H
#define MATH_PRIVATE_H

#include <stdint.h>
#include <math.h>

typedef union {
    double value;
    uint64_t word;
} ieee_double_shape_type;

/* Get a 64-bit int from a double. */
#define EXTRACT_WORD64(i, d) do { \
    ieee_double_shape_type gh_u; \
    gh_u.value = (d); \
    (i) = gh_u.word; \
} while (0)

/* Set a double from a 64-bit int. */
#define INSERT_WORD64(d, i) do { \
    ieee_double_shape_type iw_u; \
    iw_u.word = (i); \
    (d) = iw_u.value; \
} while (0)

static const double M_PI = 3.14159265358979323846;
static const double M_PI_2 = 1.57079632679489661923;
static const double M_PI_4 = 0.78539816339744830962;

#endif

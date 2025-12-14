/*
 * Part of PD LibM
 * Internal helper macros and constants.
 */

#ifndef _MATH_PRIVATE_H
#define _MATH_PRIVATE_H

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

#endif

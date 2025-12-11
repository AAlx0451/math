/*
 * Part of PD LibM
 * Originally made for Small-LibC 
 */

#ifndef _MATH_PRIVATE_H
#define _MATH_PRIVATE_H

#include <stdint.h>
#include <math.h>

/* IEEE 754 double precision bit manipulation */
typedef union {
    double value;
    uint64_t word;
} ieee_double_shape_type;

static inline uint64_t GET_WORD(double d) {
    ieee_double_shape_type u;
    u.value = d;
    return u.word;
}

static inline double SET_WORD(uint64_t w) {
    ieee_double_shape_type u;
    u.word = w;
    return u.value;
}

#endif

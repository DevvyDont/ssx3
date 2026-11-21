#ifndef SEG_NEWLIB_FDLIBM_H
#define SEG_NEWLIB_FDLIBM_H

// The float-word union of newlib's libm (fdlibm.h), which the game's libm functions in seg/seg_2FE810 (__ieee754_logf
// = newlib e_logf.c, tanf = s_tanf.c, ...) use through GET_FLOAT_WORD / SET_FLOAT_WORD (views group D:
// ieee_float_shape_type_E550, FloatBits_D588).
// [L] refs/newlib-1.9.0-src/newlib-1.9.0/newlib/libm/common/fdlibm.h, which carries this notice:
//
//   ====================================================
//   Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
//
//   Developed at SunPro, a Sun Microsystems, Inc. business.
//   Permission to use, copy, modify, and distribute this
//   software is freely granted, provided that this notice
//   is preserved.
//   ====================================================
//
// newlib spells the word __uint32_t; on the EE (long is 64-bit) that is a 32-bit unsigned int.

#include "layout.h"

typedef union
{
  float value;          // 0x0 [L]
  unsigned int word;    // 0x0 [L] __uint32_t
} ieee_float_shape_type;

CHECK_SIZE(ieee_float_shape_type, 0x4);

/* Get a 32 bit int from a float.  [L] */
#define GET_FLOAT_WORD(i,d)                                     \
do {                                                            \
  ieee_float_shape_type gf_u;                                   \
  gf_u.value = (d);                                             \
  (i) = gf_u.word;                                              \
} while (0)

/* Set a float from a 32 bit int.  [L] */
#define SET_FLOAT_WORD(d,i)                                     \
do {                                                            \
  ieee_float_shape_type sf_u;                                   \
  sf_u.word = (i);                                              \
  (d) = sf_u.value;                                             \
} while (0)

#endif /* SEG_NEWLIB_FDLIBM_H */

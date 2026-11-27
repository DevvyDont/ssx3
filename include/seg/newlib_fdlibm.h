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

/* The double-word union (fdlibm.h, __IEEE_LITTLE_ENDIAN branch: lsw first). [L] fdlibm.h; [U] the soft-float double
   routines in seg/seg_316FB8 (__ieee754_log_2: EXTRACT_WORDS reads msw = word 1, lsw = word 0)
   (views group D: ieee_double_shape_type_0041F3A8). */
typedef union
{
  double value;         // 0x0 [L]
  struct
  {
    unsigned int lsw;   // 0x0 [L] __uint32_t
    unsigned int msw;   // 0x4 [L] __uint32_t
  } parts;
} ieee_double_shape_type;

CHECK_SIZE(ieee_double_shape_type, 0x8);

/* A double's high word: [L] libc/stdlib/mprec.h (IEEE branch). Exp_1 is the high word of 1.0, Sign_bit its sign bit,
   Ebits the exponent's width and Exp_shift the exponent's shift (also the count of fraction bits in the high word).
   mprec.c's b2d builds words with them; fdlibm (s_floor.c, e_log.c) spells the same values in hex. */
#define Exp_shift 20
#define Ebits     11
#define Exp_1     ((unsigned int)0x3ff00000)
#define Sign_bit  ((unsigned int)0x80000000)

/* The unbiased exponent of an infinity or NaN: the 11-bit exponent field all ones (0x7ff) less the bias 0x3ff
   ([L] s_floor.c tests `j0==0x400`, commented "inf or NaN"). Name ours. */
#define DOUBLE_EXP_INF_NAN 0x400

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

// struct exception: what newlib's libm wrappers (w_log.c log, wf_exp.c expf, ...; SVID error handling) fill and pass
// to matherr (views group D: E40D610 in log, exception_D758 in expf). [L] newlib-1.9.0 libc/include/math.h
// (`struct exception` in C, `struct __exception` in C++); the wrappers that use it are SunPro fdlibm code (notice
// above). On the EE double is 8 bytes and 8-aligned: arg1 at 0x8.
#ifndef __cplusplus
struct exception
#else
struct __exception
#endif
{
  int type;             // 0x00 [L] DOMAIN 1, SING 2, OVERFLOW 3, UNDERFLOW 4, TLOSS 5, PLOSS 6
  char *name;           // 0x04 [L] the function's name
  double arg1;          // 0x08 [L]
  double arg2;          // 0x10 [L]
  double retval;        // 0x18 [L]
  int err;              // 0x20 [L]
};

// CHECK_SIZE / CHECK_OFFSET need a typedef name; the checks are spelled out.
#if !defined(__cplusplus) && !defined(DECOMP_NO_LAYOUT_CHECKS)
typedef char check_exception_size[(sizeof(struct exception) == 0x28) ? 1 : -1];
typedef char check_exception_retval[(DECOMP_OFFSETOF(struct exception, retval) == 0x18) ? 1 : -1];
#endif

#endif /* SEG_NEWLIB_FDLIBM_H */

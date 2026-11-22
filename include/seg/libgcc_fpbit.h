#ifndef SEG_LIBGCC_FPBIT_H
#define SEG_LIBGCC_FPBIT_H

/* The unpacked number of libgcc's software floating point (gcc config/fp-bit.c), as the double-precision copy in
   seg/seg_312310 uses it (views group D: S412280 in dpdiv). fp-bit.c's double build names its routines __unpack_d /
   __pack_d / __fpcmp_parts_d and, with US_SOFTWARE_GOFAST, `divide` = dpdiv - the retail symbols of that unit.
   [L] refs/gcc-2.95.3/gcc/config/fp-bit.c (fp_class_type, fp_number_type; for double fractype = UDItype, halffractype
   = USItype); [R] retail dpdiv: __unpack_d(&x, &numer) fills class 0x0 / sign 0x4 / normal_exp 0x8 / fraction 0x10
   (ld / sd: the 64-bit fraction is 8-aligned, so 0xC is padding); class < 2 is isnan (CLASS_SNAN / CLASS_QNAN),
   class 4 CLASS_INFINITY, 2 CLASS_ZERO. Caveat: dpdiv's two locals sit 0x20 apart on the stack and the view S412280
   carries an extra 8-byte pad (size 0x20); whether this 0x18 struct gives the same frame needs a compile check.

   fp-bit.c carries this notice (with the libgcc linking exception):

   Copyright (C) 1994, 1995, 1996, 1997, 1998 Free Software Foundation, Inc.

   This file is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either version 2, or (at your option) any later version.

   In addition to the permissions in the GNU General Public License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file with other programs, and to distribute those programs without
   any restriction coming from the use of this file.  (The General Public License restrictions do apply in other
   respects; for example, they cover modification of the file, and distribution when not linked into another program.)

   This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.

   As a special exception, if you link this library with other files, some of which are compiled with GCC, to produce
   an executable, this library does not by itself cause the resulting executable to be covered by the GNU General
   Public License.  This exception does not however invalidate any other reasons why the executable file might be
   covered by the GNU General Public License.  */

#include "layout.h"

typedef enum
{
  CLASS_SNAN,                       /* 0 [L] */
  CLASS_QNAN,                       /* 1 [L] */
  CLASS_ZERO,                       /* 2 [L] */
  CLASS_NUMBER,                     /* 3 [L] */
  CLASS_INFINITY                    /* 4 [L] */
} fp_class_type;

/* [L] fp_number_type (not SMALL_MACHINE), double precision. fractype is UDItype (64-bit; `unsigned long` on the EE,
   spelled `unsigned long long` here so the layout is the same on any host). */
typedef struct
{
  fp_class_type class_;             /* 0x00 [L] `class` in fp-bit.c (renamed: `class` is a C++ keyword) */
  unsigned int sign;                /* 0x04 [L] */
  int normal_exp;                   /* 0x08 [L] */
  union
    {
      unsigned long long ll;        /* 0x10 [L] fractype (UDItype) */
      unsigned int l[2];            /* 0x10 [L] halffractype (USItype) */
    } fraction;
} fp_number_type;

CHECK_OFFSET(fp_number_type, sign, 0x04);
CHECK_OFFSET(fp_number_type, normal_exp, 0x08);
CHECK_OFFSET(fp_number_type, fraction, 0x10);
CHECK_SIZE(fp_number_type, 0x18);

#endif /* SEG_LIBGCC_FPBIT_H */

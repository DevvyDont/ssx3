/* GNU CC runtime support, reconstructed: libgcc2.c (64-bit integer and conversion routines) and fp-bit.c
   (software double precision, US_SOFTWARE_GOFAST names: dpadd, dpmul, ...).
   Originals: Copyright (C) 1989, 92-98, 1999 / 1994-1998 Free Software Foundation, Inc.
   It is free software under the GNU General Public License, version 2 or (at your option) any later version, with
   this special exception: "if you link this library with other files, some of which are compiled with GCC, to
   produce an executable, this library does not by itself cause the resulting executable to be covered by the
   GNU General Public License. This exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License."
   fp-bit.c adds: "the Free Software Foundation gives you unlimited permission to link the compiled version of
   this file with other programs, and to distribute those programs without any restriction coming from the use
   of this file."
   This file stays under those terms; the repository's CC0 dedication does not apply to it (README, License).
   Bodies credited to peak3project come from their fork (github.com/peak3project/ssx3). */

#include "common.h"

//100%
INCLUDE_ASM("seg/seg_312310", func_00411310);
#ifdef SKIP_ASM
extern void func_0040FB88(int);
extern void func_00416FE0(void);
extern void func_001001C8(int);
void func_00411310(int a) {
    func_0040FB88(a);
    func_00416FE0();
    func_001001C8(a);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", __fixunssfdi);
#ifdef SKIP_ASM
extern double a_411340(double) __asm__("fptodp");
extern int cmp_411340(double, double) __asm__("dpcmp");
extern double mul_411340(double, double) __asm__("dpmul");
extern unsigned dtou_411340(double) __asm__("dptoul");
extern double ltod_411340(long) __asm__("__floatdidf");
extern double add_411340(double, double) __asm__("dpadd");
extern double sub_411340(double, double) __asm__("dpsub");
/**
 * Converts a float to an unsigned 64-bit integer (libgcc2's __fixunssfdi), using the soft-double routines.
 *
 * Widens the value with fptodp, takes the high word from value / 2^32, then adds or subtracts the remainder.
 *
 * @param a  the value to convert
 * @return the unsigned 64-bit result; 0 for a negative value
 */
unsigned long __fixunssfdi(double a) {
    unsigned long hi;
    double v;
    double d;
    v = a_411340(a);
    if (cmp_411340(v, 0.0) < 0)
        return 0;
    hi = (unsigned long)dtou_411340(mul_411340(v, 2.3283064365386963e-10)) << 32;
    if ((long)hi >= 0) {
        d = ltod_411340(hi);
    } else {
        d = ltod_411340((hi & 1) | (hi >> 1));
        d = add_411340(d, d);
    }
    v = sub_411340(v, d);
    if (cmp_411340(v, 0.0) < 0)
        hi -= (unsigned long)dtou_411340(sub_411340(0.0, v));
    else
        hi += (unsigned long)dtou_411340(v);
    return hi;
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", __floatdidf);
#ifdef SKIP_ASM
extern double itod_411438(int) __asm__("litodp");
extern double dmul_411438(double, double) __asm__("dpmul");
extern double dadd_411438(double, double) __asm__("dpadd");
/**
 * Converts a signed 64-bit integer to a double (libgcc2's __floatdidf), using the soft-double routines.
 *
 * Computes high word * 65536 * 65536 plus the low word, read as unsigned.
 *
 * @param a  the value to convert
 * @return the value as a double
 */
double __floatdidf(long a) {
    double hi = itod_411438((int)(a >> 32));
    double lo;
    hi = dmul_411438(hi, 65536.0);
    hi = dmul_411438(hi, 65536.0);
    lo = itod_411438((int)(a & 0xFFFFFFFF));
    if ((int)(a & 0xFFFFFFFF) < 0)
        lo = dadd_411438(lo, 4294967296.0);
    return dadd_411438(hi, lo);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_004114D0);
#ifdef SKIP_ASM
/* libgcc2.c + longlong.h shared text for the 64-bit integer routines (GCC runtime, GPL v2+ with the libgcc linking
   exception). Resolved for this target (mips r5900, little-endian, ee-gcc 2.9-ee-991111): */

/* tm.h */
#define BITS_PER_UNIT 8

/* libgcc2.c: mode-specific type names. (The source's `#define int bogus_type` ... guards are left out: this unit
   also holds code that uses the C type names.) */
typedef unsigned int UQItype	__attribute__ ((mode (QI)));
typedef 	 int SItype	__attribute__ ((mode (SI)));
typedef unsigned int USItype	__attribute__ ((mode (SI)));
typedef		 int DItype	__attribute__ ((mode (DI)));
typedef unsigned int UDItype	__attribute__ ((mode (DI)));

typedef int word_type __attribute__ ((mode (__word__)));

#define SI_TYPE_SIZE (sizeof (SItype) * BITS_PER_UNIT)

/* DIstructs are pairs of SItype values in the order determined by
   LIBGCC2_WORDS_BIG_ENDIAN.  */

/* LIBGCC2_WORDS_BIG_ENDIAN is 0 (little-endian EE) */
  struct DIstruct {SItype low, high;};

/* We need this union to unpack/pack DImode values, since we don't have
   any arithmetic yet.  Incoming DImode parameters are stored into the
   `ll' field, and the unpacked result is read from the struct `s'.  */

typedef union
{
  struct DIstruct s;
  DItype ll;
} DIunion;

/* longlong.h (SI_TYPE_SIZE is defined above, so longlong.h's default of 32 is not needed). */
#define __BITS4 (SI_TYPE_SIZE / 4)
#define __ll_B (1L << (SI_TYPE_SIZE / 2))
#define __ll_lowpart(t) ((USItype) (t) % __ll_B)
#define __ll_highpart(t) ((USItype) (t) / __ll_B)

/* longlong.h, __mips__ */
// PORT: longlong.h umul_ppmm (MIPS multu); a port uses longlong.h's generic C version
#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("multu %2,%3"						\
	   : "=l" ((USItype) (w0)),					\
	     "=h" ((USItype) (w1))					\
	   : "d" ((USItype) (u)),					\
	     "d" ((USItype) (v)))

/* longlong.h generic C macros (the mips section defines none of these) */
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  do {									\
    USItype __x;							\
    __x = (al) - (bl);							\
    (sh) = (ah) - (bh) - (__x > (al));					\
    (sl) = __x;								\
  } while (0)

#define __umulsidi3(u, v) \
  ({DIunion __w;							\
    umul_ppmm (__w.s.high, __w.s.low, u, v);				\
    __w.ll; })

/* Define this unconditionally, so it can be used for debugging.  */
#define __udiv_qrnnd_c(q, r, n1, n0, d) \
  do {									\
    USItype __d1, __d0, __q1, __q0;					\
    USItype __r1, __r0, __m;						\
    __d1 = __ll_highpart (d);						\
    __d0 = __ll_lowpart (d);						\
									\
    __r1 = (n1) % __d1;							\
    __q1 = (n1) / __d1;							\
    __m = (USItype) __q1 * __d0;					\
    __r1 = __r1 * __ll_B | __ll_highpart (n0);				\
    if (__r1 < __m)							\
      {									\
	__q1--, __r1 += (d);						\
	if (__r1 >= (d)) /* i.e. we didn't get carry when adding to __r1 */\
	  if (__r1 < __m)						\
	    __q1--, __r1 += (d);					\
      }									\
    __r1 -= __m;							\
									\
    __r0 = __r1 % __d1;							\
    __q0 = __r1 / __d1;							\
    __m = (USItype) __q0 * __d0;					\
    __r0 = __r0 * __ll_B | __ll_lowpart (n0);				\
    if (__r0 < __m)							\
      {									\
	__q0--, __r0 += (d);						\
	if (__r0 >= (d))						\
	  if (__r0 < __m)						\
	    __q0--, __r0 += (d);					\
      }									\
    __r0 -= __m;							\
									\
    (q) = (USItype) __q1 * __ll_B | __q0;				\
    (r) = __r0;								\
  } while (0)

/* If udiv_qrnnd was not defined for this processor, use __udiv_qrnnd_c.  */
/* (mips defines neither udiv_qrnnd nor sdiv_qrnnd) */
#define UDIV_NEEDS_NORMALIZATION 1
#define udiv_qrnnd __udiv_qrnnd_c

/* (mips defines no count_leading_zeros: the __clz_tab version; SI_TYPE_SIZE <= 32) */
#define count_leading_zeros(count, x) \
  do {									\
    USItype __xr = (x);							\
    USItype __a;							\
									\
    if (SI_TYPE_SIZE <= 32)						\
      {									\
	__a = __xr < ((USItype)1<<2*__BITS4)				\
	  ? (__xr < ((USItype)1<<__BITS4) ? 0 : __BITS4)		\
	  : (__xr < ((USItype)1<<3*__BITS4) ?  2*__BITS4 : 3*__BITS4);	\
      }									\
    else								\
      {									\
	for (__a = SI_TYPE_SIZE - 8; __a > 0; __a -= 8)			\
	  if (((__xr >> __a) & 0xff) != 0)				\
	    break;							\
      }									\
									\
    (count) = SI_TYPE_SIZE - (__clz_tab[__xr >> __a] + __a);		\
  } while (0)

/* libgcc2.c __muldi3 (GCC runtime, GPL v2+ with the libgcc linking exception) */
DItype __muldi3 (DItype u, DItype v) __asm__("func_004114D0");

DItype
__muldi3 (DItype u, DItype v)
{
  DIunion w;
  DIunion uu, vv;

  uu.ll = u,
  vv.ll = v;

  w.ll = __umulsidi3 (uu.s.low, vv.s.low);
  w.s.high += ((USItype) uu.s.low * (USItype) vv.s.high
	       + (USItype) uu.s.high * (USItype) vv.s.low);

  return w.ll;
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_00411530);
#ifdef SKIP_ASM
extern void func_004103B0(void);
void func_00411530(void) {
    func_004103B0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_00411538);
#ifdef SKIP_ASM
/* libgcc2.c __udivdi3 with its static inline __udivmoddi4 (GCC runtime, GPL v2+ with the libgcc linking exception) */
/* The L_udivdi3 object's own copy of libgcc2.c's `static const UQItype __clz_tab[]` (its rodata stays in asm). */
extern const UQItype __clz_tab[] __asm__("D_004997A0");

/* (L_udivdi3: static inline; UDIV_NEEDS_NORMALIZATION is 1, so the normalizing d1 == 0 branch) */
static inline
UDItype
__udivmoddi4 (UDItype n, UDItype d, UDItype *rp)
{
  DIunion ww;
  DIunion nn, dd;
  DIunion rr;
  USItype d0, d1, n0, n1, n2;
  USItype q0, q1;
  USItype b, bm;

  nn.ll = n;
  dd.ll = d;

  d0 = dd.s.low;
  d1 = dd.s.high;
  n0 = nn.s.low;
  n1 = nn.s.high;

  if (d1 == 0)
    {
      if (d0 > n1)
	{
	  /* 0q = nn / 0D */

	  count_leading_zeros (bm, d0);

	  if (bm != 0)
	    {
	      /* Normalize, i.e. make the most significant bit of the
		 denominator set.  */

	      d0 = d0 << bm;
	      n1 = (n1 << bm) | (n0 >> (SI_TYPE_SIZE - bm));
	      n0 = n0 << bm;
	    }

	  udiv_qrnnd (q0, n0, n1, n0, d0);
	  q1 = 0;

	  /* Remainder in n0 >> bm.  */
	}
      else
	{
	  /* qq = NN / 0d */

	  if (d0 == 0)
	    d0 = 1 / d0;	/* Divide intentionally by zero.  */

	  count_leading_zeros (bm, d0);

	  if (bm == 0)
	    {
	      /* From (n1 >= d0) /\ (the most significant bit of d0 is set),
		 conclude (the most significant bit of n1 is set) /\ (the
		 leading quotient digit q1 = 1).

		 This special case is necessary, not an optimization.
		 (Shifts counts of SI_TYPE_SIZE are undefined.)  */

	      n1 -= d0;
	      q1 = 1;
	    }
	  else
	    {
	      /* Normalize.  */

	      b = SI_TYPE_SIZE - bm;

	      d0 = d0 << bm;
	      n2 = n1 >> b;
	      n1 = (n1 << bm) | (n0 >> b);
	      n0 = n0 << bm;

	      udiv_qrnnd (q1, n1, n2, n1, d0);
	    }

	  /* n1 != d0...  */

	  udiv_qrnnd (q0, n0, n1, n0, d0);

	  /* Remainder in n0 >> bm.  */
	}

      if (rp != 0)
	{
	  rr.s.low = n0 >> bm;
	  rr.s.high = 0;
	  *rp = rr.ll;
	}
    }

  else
    {
      if (d1 > n1)
	{
	  /* 00 = nn / DD */

	  q0 = 0;
	  q1 = 0;

	  /* Remainder in n1n0.  */
	  if (rp != 0)
	    {
	      rr.s.low = n0;
	      rr.s.high = n1;
	      *rp = rr.ll;
	    }
	}
      else
	{
	  /* 0q = NN / dd */

	  count_leading_zeros (bm, d1);
	  if (bm == 0)
	    {
	      /* From (n1 >= d1) /\ (the most significant bit of d1 is set),
		 conclude (the most significant bit of n1 is set) /\ (the
		 quotient digit q0 = 0 or 1).

		 This special case is necessary, not an optimization.  */

	      /* The condition on the next line takes advantage of that
		 n1 >= d1 (true due to program flow).  */
	      if (n1 > d1 || n0 >= d0)
		{
		  q0 = 1;
		  sub_ddmmss (n1, n0, n1, n0, d1, d0);
		}
	      else
		q0 = 0;

	      q1 = 0;

	      if (rp != 0)
		{
		  rr.s.low = n0;
		  rr.s.high = n1;
		  *rp = rr.ll;
		}
	    }
	  else
	    {
	      USItype m1, m0;
	      /* Normalize.  */

	      b = SI_TYPE_SIZE - bm;

	      d1 = (d1 << bm) | (d0 >> b);
	      d0 = d0 << bm;
	      n2 = n1 >> b;
	      n1 = (n1 << bm) | (n0 >> b);
	      n0 = n0 << bm;

	      udiv_qrnnd (q0, n1, n2, n1, d1);
	      umul_ppmm (m1, m0, q0, d0);

	      if (m1 > n1 || (m1 == n1 && m0 > n0))
		{
		  q0--;
		  sub_ddmmss (m1, m0, m1, m0, d1, d0);
		}

	      q1 = 0;

	      /* Remainder in (n1n0 - m1m0) >> bm.  */
	      if (rp != 0)
		{
		  sub_ddmmss (n1, n0, n1, n0, m1, m0);
		  rr.s.low = (n1 << b) | (n0 >> bm);
		  rr.s.high = n1 >> bm;
		  *rp = rr.ll;
		}
	    }
	}
    }

  ww.s.low = q0;
  ww.s.high = q1;
  return ww.ll;
}

UDItype __udivdi3 (UDItype n, UDItype d) __asm__("func_00411538");

UDItype __udivmoddi4 ();
UDItype
__udivdi3 (UDItype n, UDItype d)
{
  return __udivmoddi4 (n, d, (UDItype *) 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_00411B08);
#ifdef SKIP_ASM
/* fp-bit.c (GCC runtime, GPL v2+ with the libgcc linking exception): the header part, compiled as dp-bit.c for
   DFmode. mips/t-elf builds dp-bit.c as `#ifdef __MIPSEL__ / #define FLOAT_BIT_ORDER_MISMATCH / #endif`,
   `#define US_SOFTWARE_GOFAST`, then fp-bit.c; resolved here for little-endian MIPS: FLOAT not defined (double),
   FLOAT_BIT_ORDER_MISMATCH defined, US_SOFTWARE_GOFAST defined; SMALL_MACHINE, NO_NANS, _DEBUG_BITFLOAT,
   FLOAT_WORD_ORDER_MISMATCH, FLOAT_ONLY and EXTENDED_FLOAT_STUBS not defined. */

/* fp-bit.c also declares SItype, DItype, USItype and UDItype (`typedef int SItype __attribute__ ((mode (SI)));`
   ...): in this unit they come from gpl/seg_312310.libgcc2.shared.c (libgcc2.c's identical typedefs, landed
   earlier in the unit), since C does not allow a typedef to be repeated. */
typedef float SFtype __attribute__ ((mode (SF)));
typedef float DFtype __attribute__ ((mode (DF)));

typedef int HItype __attribute__ ((mode (HI)));

/* The type of the result of a fp compare */
#define CMPtype SItype

typedef unsigned int UHItype __attribute__ ((mode (HI)));

#define MAX_SI_INT   ((SItype) ((unsigned) (~0)>>1))
#define MAX_USI_INT  ((USItype) ~0)

/* !FLOAT: the double-precision parameters */
#	define PREFIXFPDP dp
#	define PREFIXSFDF df
#	define NGARDS 8L
#	define GARDROUND 0x7f
#	define GARDMASK  0xff
#	define GARDMSB   0x80
#	define EXPBITS 11
#	define EXPBIAS 1023
#	define FRACBITS 52
#	define EXPMAX (0x7ff)
#	define QUIET_NAN 0x8000000000000LL
#	define FRAC_NBITS 64
#	define FRACHIGH  0x8000000000000000LL
#	define FRACHIGH2 0xc000000000000000LL
#	define pack_d __pack_d
#	define unpack_d __unpack_d
#	define __fpcmp_parts __fpcmp_parts_d
	typedef UDItype fractype;
	typedef USItype halffractype;
	typedef DFtype FLO_type;
	typedef DItype intfrac;

/* US_SOFTWARE_GOFAST, !FLOAT: entry-point names (the ones already landed in this unit, dpadd ... __make_dp, are
   written under their GOFAST names directly, so only the remaining ones are mapped here). */
#		define multiply 	dpmul
#		define df_to_sf 	dptofp

#define INLINE __inline__

/* Preserve the sticky-bit when shifting fractions to the right.  */
#define LSHIFT(a) { a = (a & 1) | (a >> 1); }

/* numeric parameters */
/* F_D_BITOFF is the number of bits offset between the MSB of the mantissa
   of a float and of a double. Assumes there are only two float types.
   (double::FRAC_BITS+double::NGARDS-(float::FRAC_BITS-float::NGARDS))
 */
#define F_D_BITOFF (52+8-(23+7))


#define NORMAL_EXPMIN (-(EXPBIAS)+1)
#define IMPLICIT_1 (1LL<<(FRACBITS+NGARDS))
#define IMPLICIT_2 (1LL<<(FRACBITS+1+NGARDS))

/* common types */

typedef enum
{
  CLASS_SNAN,
  CLASS_QNAN,
  CLASS_ZERO,
  CLASS_NUMBER,
  CLASS_INFINITY
} fp_class_type;

typedef struct
{
  /* !SMALL_MACHINE */
  fp_class_type class;
  unsigned int sign;
  int normal_exp;

  union
    {
      fractype ll;
      halffractype l[2];
    } fraction;
} fp_number_type;

typedef union
{
  FLO_type value;
  fractype value_raw;

  /* !FLOAT */
  halffractype words[2];

  /* FLOAT_BIT_ORDER_MISMATCH */
  struct
    {
      fractype fraction:FRACBITS __attribute__ ((packed));
      unsigned int exp:EXPBITS __attribute__ ((packed));
      unsigned int sign:1 __attribute__ ((packed));
    }
  bits;
}
FLO_union_type;


/* end of header */

/* IEEE "special" number predicates */

/* nan()'s function-local `static fp_number_type thenan;` is retail's D_0052ADC0 (.bss): declared here at file
   scope under that symbol so every reference (here and in the landed dpdiv) names the same object. */
extern fp_number_type thenan __asm__("D_0052ADC0");

INLINE
static fp_number_type *
nan ()
{
  return &thenan;
}

INLINE
static int
isnan ( fp_number_type *  x)
{
  return x->class == CLASS_SNAN || x->class == CLASS_QNAN;
}

INLINE
static int
isinf ( fp_number_type *  x)
{
  return x->class == CLASS_INFINITY;
}

INLINE
static int
iszero ( fp_number_type *  x)
{
  return x->class == CLASS_ZERO;
}

INLINE
static void
flip_sign ( fp_number_type *  x)
{
  x->sign = !x->sign;
}

/* The fp-bit.c functions, bound to their retail symbols (the unit's landed bodies call them by these). */
extern FLO_type pack_d ( fp_number_type * ) __asm__("func_00411B08");
extern void unpack_d (FLO_union_type *, fp_number_type *) __asm__("func_00411C38");
int __fpcmp_parts (fp_number_type * a, fp_number_type *b) __asm__("func_004123E8");

/* fp-bit.c pack_d (GCC runtime, GPL v2+ with the libgcc linking exception): __pack_d, retail func_00411B08 */
FLO_type
pack_d ( fp_number_type *  src)
{
  FLO_union_type dst;
  fractype fraction = src->fraction.ll;	/* wasn't unsigned before? */
  int sign = src->sign;
  int exp = 0;

  if (isnan (src))
    {
      exp = EXPMAX;
      if (src->class == CLASS_QNAN || 1)
	{
	  fraction |= QUIET_NAN;
	}
    }
  else if (isinf (src))
    {
      exp = EXPMAX;
      fraction = 0;
    }
  else if (iszero (src))
    {
      exp = 0;
      fraction = 0;
    }
  else if (fraction == 0)
    {
      exp = 0;
    }
  else
    {
      if (src->normal_exp < NORMAL_EXPMIN)
	{
	  /* This number's exponent is too low to fit into the bits
	     available in the number, so we'll store 0 in the exponent and
	     shift the fraction to the right to make up for it.  */

	  int shift = NORMAL_EXPMIN - src->normal_exp;

	  exp = 0;

	  if (shift > FRAC_NBITS - NGARDS)
	    {
	      /* No point shifting, since it's more that 64 out.  */
	      fraction = 0;
	    }
	  else
	    {
	      /* Shift by the value */
	      fraction >>= shift;
	    }
	  fraction >>= NGARDS;
	}
      else if (src->normal_exp > EXPBIAS)
	{
	  exp = EXPMAX;
	  fraction = 0;
	}
      else
	{
	  exp = src->normal_exp + EXPBIAS;
	  /* IF the gard bits are the all zero, but the first, then we're
	     half way between two numbers, choose the one which makes the
	     lsb of the answer 0.  */
	  if ((fraction & GARDMASK) == GARDMSB)
	    {
	      if (fraction & (1 << NGARDS))
		fraction += GARDROUND + 1;
	    }
	  else
	    {
	      /* Add a one to the guards to round up */
	      fraction += GARDROUND;
	    }
	  if (fraction >= IMPLICIT_2)
	    {
	      fraction >>= 1;
	      exp += 1;
	    }
	  fraction >>= NGARDS;
	}
    }

  /* We previously used bitfields to store the number, but this doesn't
     handle little/big endian systems conveniently, so use shifts and
     masks */
  /* FLOAT_BIT_ORDER_MISMATCH (little-endian MIPS) */
  dst.bits.fraction = fraction;
  dst.bits.exp = exp;
  dst.bits.sign = sign;

  /* FLOAT_WORD_ORDER_MISMATCH is not defined: no word swap */

  return dst.value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_00411C38);
#ifdef SKIP_ASM
/* fp-bit.c unpack_d (GCC runtime, GPL v2+ with the libgcc linking exception): __unpack_d, retail func_00411C38 */
void
unpack_d (FLO_union_type * src, fp_number_type * dst)
{
  /* We previously used bitfields to store the number, but this doesn't
     handle little/big endian systems conveniently, so use shifts and
     masks */
  fractype fraction;
  int exp;
  int sign;

  /* FLOAT_WORD_ORDER_MISMATCH is not defined: no word swap */

  /* FLOAT_BIT_ORDER_MISMATCH (little-endian MIPS) */
  fraction = src->bits.fraction;
  exp = src->bits.exp;
  sign = src->bits.sign;

  dst->sign = sign;
  if (exp == 0)
    {
      /* Hmm.  Looks like 0 */
      /* Retail differs from 2.95.3 here: it has later fp-bit.c's NO_DENORMALS test,
	 `if (fraction == 0 #ifdef NO_DENORMALS || 1 #endif)`, resolved with NO_DENORMALS defined: a zero exponent
	 is always CLASS_ZERO. The dead denormal branch below is still compiled: its loop's alignment survives as the
	 retail nop before the exp == EXPMAX test (without the branch: 97.44). */
      if (fraction == 0 || 1)
	{
	  /* tastes like zero */
	  dst->class = CLASS_ZERO;
	}
      else
	{
	  /* Zero exponent with non zero fraction - it's denormalized,
	     so there isn't a leading implicit one - we'll shift it so
	     it gets one.  */
	  dst->normal_exp = exp - EXPBIAS + 1;
	  fraction <<= NGARDS;

	  dst->class = CLASS_NUMBER;
	  while (fraction < IMPLICIT_1)
	    {
	      fraction <<= 1;
	      dst->normal_exp--;
	    }
	  dst->fraction.ll = fraction;
	}
    }
  else if (exp == EXPMAX)
    {
      /* Huge exponent*/
      if (fraction == 0)
	{
	  /* Attached to a zero fraction - means infinity */
	  dst->class = CLASS_INFINITY;
	}
      else
	{
	  /* Non zero fraction, means nan */
	  if (fraction & QUIET_NAN)
	    {
	      dst->class = CLASS_QNAN;
	    }
	  else
	    {
	      dst->class = CLASS_SNAN;
	    }
	  /* Keep the fraction part as the nan number */
	  dst->fraction.ll = fraction;
	}
    }
  else
    {
      /* Nothing strange about this number */
      dst->normal_exp = exp - EXPBIAS;
      dst->class = CLASS_NUMBER;
      dst->fraction.ll = (fraction << NGARDS) | IMPLICIT_1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_00411CD8);
#ifdef SKIP_ASM
/* fp-bit.c _fpadd_parts (GCC runtime, GPL v2+ with the libgcc linking exception): retail func_00411CD8 */
static fp_number_type *_fpadd_parts (fp_number_type *, fp_number_type *, fp_number_type *) __asm__("func_00411CD8");

static fp_number_type *
_fpadd_parts (fp_number_type * a,
	      fp_number_type * b,
	      fp_number_type * tmp)
{
  intfrac tfraction;

  /* Put commonly used fields in local variables.  */
  int a_normal_exp;
  int b_normal_exp;
  fractype a_fraction;
  fractype b_fraction;

  if (isnan (a))
    {
      return a;
    }
  if (isnan (b))
    {
      return b;
    }
  if (isinf (a))
    {
      /* Adding infinities with opposite signs yields a NaN.  */
      if (isinf (b) && a->sign != b->sign)
	return nan ();
      return a;
    }
  if (isinf (b))
    {
      return b;
    }
  if (iszero (b))
    {
      if (iszero (a))
	{
	  *tmp = *a;
	  tmp->sign = a->sign & b->sign;
	  return tmp;
	}
      return a;
    }
  if (iszero (a))
    {
      return b;
    }

  /* Got two numbers. shift the smaller and increment the exponent till
     they're the same */
  {
    int diff;

    a_normal_exp = a->normal_exp;
    b_normal_exp = b->normal_exp;
    a_fraction = a->fraction.ll;
    b_fraction = b->fraction.ll;

    diff = a_normal_exp - b_normal_exp;

    if (diff < 0)
      diff = -diff;
    if (diff < FRAC_NBITS)
      {
	/* ??? This does shifts one bit at a time.  Optimize.  */
	while (a_normal_exp > b_normal_exp)
	  {
	    b_normal_exp++;
	    LSHIFT (b_fraction);
	  }
	while (b_normal_exp > a_normal_exp)
	  {
	    a_normal_exp++;
	    LSHIFT (a_fraction);
	  }
      }
    else
      {
	/* Somethings's up.. choose the biggest */
	if (a_normal_exp > b_normal_exp)
	  {
	    b_normal_exp = a_normal_exp;
	    b_fraction = 0;
	  }
	else
	  {
	    a_normal_exp = b_normal_exp;
	    a_fraction = 0;
	  }
      }
  }

  if (a->sign != b->sign)
    {
      if (a->sign)
	{
	  tfraction = -a_fraction + b_fraction;
	}
      else
	{
	  tfraction = a_fraction - b_fraction;
	}
      if (tfraction >= 0)
	{
	  tmp->sign = 0;
	  tmp->normal_exp = a_normal_exp;
	  tmp->fraction.ll = tfraction;
	}
      else
	{
	  tmp->sign = 1;
	  tmp->normal_exp = a_normal_exp;
	  tmp->fraction.ll = -tfraction;
	}
      /* and renormalize it */

      while (tmp->fraction.ll < IMPLICIT_1 && tmp->fraction.ll)
	{
	  tmp->fraction.ll <<= 1;
	  tmp->normal_exp--;
	}
    }
  else
    {
      tmp->sign = a->sign;
      tmp->normal_exp = a_normal_exp;
      tmp->fraction.ll = a_fraction + b_fraction;
    }
  tmp->class = CLASS_NUMBER;
  /* Now the fraction is added, we have to shift down to renormalize the
     number */

  if (tmp->fraction.ll >= IMPLICIT_2)
    {
      LSHIFT (tmp->fraction.ll);
      tmp->normal_exp++;
    }
  return tmp;

}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", dpadd);
#ifdef SKIP_ASM
struct S412778;
extern void func_00411C38(long *, int *);
extern struct S412778 *func_00411CD8(int *, int *, int *);
extern void func_00411B08(struct S412778 *);
/**
 * Adds two doubles in software (fp-bit.c's GOFAST dpadd).
 *
 * Unpacks both operands (func_00411C38), adds the parts (func_00411CD8) and packs the result (func_00411B08);
 * the packed sum is left in the return registers although the C declaration returns void.
 *
 * @param a  first operand, as the raw 64-bit pattern of a double
 * @param b  second operand, as the raw 64-bit pattern of a double
 */
void dpadd(long a, long b) {
    int x[8];
    int y[8];
    int z[8];
    func_00411C38(&a, x);
    func_00411C38(&b, y);
    func_00411B08(func_00411CD8(x, y, z));
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", dpsub);
#ifdef SKIP_ASM
struct S412778;
extern void func_00411C38(long *, int *);
extern struct S412778 *func_00411CD8(int *, int *, int *);
extern void func_00411B08(struct S412778 *);
/**
 * Subtracts two doubles in software (fp-bit.c's GOFAST dpsub): flips the sign of @p b and adds.
 *
 * Like dpadd(), the packed result is left in the return registers although the C declaration returns void.
 *
 * @param a  the minuend, as the raw 64-bit pattern of a double
 * @param b  the subtrahend, as the raw 64-bit pattern of a double
 */
void dpsub(long a, long b) {
    int x[8];
    int y[8];
    int z[8];
    func_00411C38(&a, x);
    func_00411C38(&b, y);
    y[1] ^= 1;
    func_00411B08(func_00411CD8(x, y, z));
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", dpmul);
#ifdef SKIP_ASM
/* fp-bit.c multiply (GCC runtime, GPL v2+ with the libgcc linking exception): dpmul (US_SOFTWARE_GOFAST), with _fpmul_parts inlined */
/* The 64-bit multiplies below are libgcc calls to __muldi3 (the R5900 has no dmult): bind gcc's fixed libcall
   name to retail's symbol for it (naming only). */
__asm__("__muldi3 = func_004114D0");

static INLINE fp_number_type *
_fpmul_parts ( fp_number_type *  a,
	       fp_number_type *  b,
	       fp_number_type * tmp)
{
  fractype low = 0;
  fractype high = 0;

  if (isnan (a))
    {
      a->sign = a->sign != b->sign;
      return a;
    }
  if (isnan (b))
    {
      b->sign = a->sign != b->sign;
      return b;
    }
  if (isinf (a))
    {
      if (iszero (b))
	return nan ();
      a->sign = a->sign != b->sign;
      return a;
    }
  if (isinf (b))
    {
      if (iszero (a))
	{
	  return nan ();
	}
      b->sign = a->sign != b->sign;
      return b;
    }
  if (iszero (a))
    {
      a->sign = a->sign != b->sign;
      return a;
    }
  if (iszero (b))
    {
      b->sign = a->sign != b->sign;
      return b;
    }

  /* Calculate the mantissa by multiplying both 64bit numbers to get a
     128 bit number */
  {
    /* !NO_DI_MODE, !FLOAT: */
    /* Doing a 64*64 to 128 */
    {
      UDItype nl = a->fraction.ll & 0xffffffff;
      UDItype nh = a->fraction.ll >> 32;
      UDItype ml = b->fraction.ll & 0xffffffff;
      UDItype mh = b->fraction.ll >>32;
      UDItype pp_ll = ml * nl;
      UDItype pp_hl = mh * nl;
      UDItype pp_lh = ml * nh;
      UDItype pp_hh = mh * nh;
      UDItype res2 = 0;
      UDItype res0 = 0;
      UDItype ps_hh__ = pp_hl + pp_lh;
      if (ps_hh__ < pp_hl)
	res2 += 0x100000000LL;
      pp_hl = (ps_hh__ << 32) & 0xffffffff00000000LL;
      res0 = pp_ll + pp_hl;
      if (res0 < pp_ll)
	res2++;
      res2 += ((ps_hh__ >> 32) & 0xffffffffL) + pp_hh;
      high = res2;
      low = res0;
    }
  }

  tmp->normal_exp = a->normal_exp + b->normal_exp;
  tmp->sign = a->sign != b->sign;
  /* !FLOAT */
  tmp->normal_exp += 4;		/* ??????????????? */
  while (high >= IMPLICIT_2)
    {
      tmp->normal_exp++;
      if (high & 1)
	{
	  low >>= 1;
	  low |= FRACHIGH;
	}
      high >>= 1;
    }
  while (high < IMPLICIT_1)
    {
      tmp->normal_exp--;

      high <<= 1;
      if (low & FRACHIGH)
	high |= 1;
      low <<= 1;
    }
  /* rounding is tricky. if we only round if it won't make us round later. */
  /* (the source's `#if 0` early-rounding block is omitted) */
  if ((high & GARDMASK) == GARDMSB)
    {
      if (high & (1 << NGARDS))
	{
	  /* half way, so round to even */
	  high += GARDROUND + 1;
	}
      else if (low)
	{
	  /* but we really weren't half way */
	  high += GARDROUND + 1;
	}
    }
  tmp->fraction.ll = high;
  tmp->class = CLASS_NUMBER;
  return tmp;
}

/**
 * Multiplies two doubles in software (fp-bit.c's multiply, exported as GOFAST dpmul).
 *
 * @param arg_a  first factor
 * @param arg_b  second factor
 * @return the product
 */
FLO_type
multiply (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type tmp;
  fp_number_type *res;

  unpack_d ((FLO_union_type *) & arg_a, &a);
  unpack_d ((FLO_union_type *) & arg_b, &b);

  res = _fpmul_parts (&a, &b, &tmp);

  return pack_d (res);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", dpdiv);
#ifdef SKIP_ASM
struct S412280 { unsigned cls; int sign; int exp; int pad; unsigned long frac; long pad2; };
extern void unp_80(long *, struct S412280 *) __asm__("func_00411C38");
extern void pk_80(struct S412280 *) __asm__("func_00411B08");
extern struct S412280 D_0052ADC0;
/**
 * Divides two doubles in software (fp-bit.c's divide, exported as GOFAST dpdiv).
 *
 * NaN operands come back unchanged; inf / inf and 0 / 0 give the NaN D_0052ADC0, x / inf gives 0 and x / 0
 * gives infinity. The quotient is left in the return register by pack_d (func_00411B08) although the C
 * declaration returns void.
 *
 * @param x  dividend, as the raw 64-bit pattern of a double
 * @param y  divisor, as the raw 64-bit pattern of a double
 */
void dpdiv(long x, long y) {
    struct S412280 a;
    struct S412280 b;
    struct S412280 *r;
    struct S412280 *p;
    unsigned long n, d, q, bit;
    unp_80(&x, &a);
    unp_80(&y, &b);
    p = &a;
    if (a.cls < 2) {
        r = &a;
    } else if (b.cls < 2) {
        r = &b;
    } else {
        a.sign ^= b.sign;
        if ((a.cls ^ 4) == 0 || (a.cls ^ 2) == 0) {
            r = &a;
            if (a.cls == b.cls)
                r = &D_0052ADC0;
        } else if ((b.cls ^ 4) == 0) {
            a.frac = 0;
            a.exp = 0;
            r = &a;
        } else if ((b.cls ^ 2) == 0) {
            a.cls = 4;
            r = &a;
        } else {
            a.exp = a.exp - b.exp;
            n = a.frac;
            d = b.frac;
            if (n < d) {
                a.exp--;
                n <<= 1;
            }
            q = 0;
            for (bit = 0x1000000000000000UL; bit != 0; bit >>= 1) {
                if (n >= d) {
                    q |= bit;
                    n -= d;
                }
                n <<= 1;
            }
            if ((q & 0xFF) == 0x80) {
                if (q & 0x100)
                    q += 0x80;
                else if (n != 0)
                    q += 0x80;
            }
            p->frac = q;
            r = p;
        }
    }
    pk_80(r);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_004123E8);
#ifdef SKIP_ASM
/* fp-bit.c __fpcmp_parts (GCC runtime, GPL v2+ with the libgcc linking exception): __fpcmp_parts_d, retail func_004123E8 */
int __fpcmp_parts (fp_number_type * a, fp_number_type *b);

/* according to the demo, fpcmp returns a comparison with 0... thus
   a<b -> -1
   a==b -> 0
   a>b -> +1
 */

int
__fpcmp_parts (fp_number_type * a, fp_number_type * b)
{
  /* (the source's `#if 0` both-nan block is omitted) */

  if (isnan (a) || isnan (b))
    {
      return 1;			/* how to indicate unordered compare? */
    }
  if (isinf (a) && isinf (b))
    {
      /* +inf > -inf, but +inf != +inf */
      /* b    \a| +inf(0)| -inf(1)
       ______\+--------+--------
       +inf(0)| a==b(0)| a<b(-1)
       -------+--------+--------
       -inf(1)| a>b(1) | a==b(0)
       -------+--------+--------
       So since unordered must be non zero, just line up the columns...
       */
      return b->sign - a->sign;
    }
  /* but not both... */
  if (isinf (a))
    {
      return a->sign ? -1 : 1;
    }
  if (isinf (b))
    {
      return b->sign ? 1 : -1;
    }
  if (iszero (a) && iszero (b))
    {
      return 0;
    }
  if (iszero (a))
    {
      return b->sign ? 1 : -1;
    }
  if (iszero (b))
    {
      return a->sign ? -1 : 1;
    }
  /* now both are "normal". */
  if (a->sign != b->sign)
    {
      /* opposite signs */
      return a->sign ? -1 : 1;
    }
  /* same sign; exponents? */
  if (a->normal_exp > b->normal_exp)
    {
      return a->sign ? -1 : 1;
    }
  if (a->normal_exp < b->normal_exp)
    {
      return a->sign ? 1 : -1;
    }
  /* same exponents; check size. */
  if (a->fraction.ll > b->fraction.ll)
    {
      return a->sign ? -1 : 1;
    }
  if (a->fraction.ll < b->fraction.ll)
    {
      return a->sign ? 1 : -1;
    }
  /* after all that, they're equal. */
  return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", dpcmp);
#ifdef SKIP_ASM
extern void func_00411C38(long *, int *);
extern void func_004123E8(int *, int *);
/**
 * Compares two doubles in software (fp-bit.c's GOFAST dpcmp) through the unpacked-parts compare func_004123E8.
 *
 * The comparison result (negative, zero or positive) is left in the return register although the C declaration
 * returns void.
 *
 * @param a  first operand, as the raw 64-bit pattern of a double
 * @param b  second operand, as the raw 64-bit pattern of a double
 */
void dpcmp(long a, long b) {
    int x[8];
    int y[8];
    func_00411C38(&a, x);
    func_00411C38(&b, y);
    func_004123E8(x, y);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", litodp);
#ifdef SKIP_ASM
struct S412550 { unsigned cls; int sign; int exp; int pad; unsigned long frac; };
extern void pk_412550(struct S412550 *) __asm__("func_00411B08");
/**
 * Converts a 32-bit signed integer to a double in software (fp-bit.c's si_to_df, exported as GOFAST litodp).
 *
 * INT_MIN is returned directly as -2147483648.0. Any other value is normalised into unpacked form and packed
 * by func_00411B08, whose result the C body leaves in the return register without a return statement.
 *
 * @param x  the integer to convert
 * @return the value as a double
 */
double litodp(int x) {
    struct S412550 s;
    s.cls = 3;
    s.sign = (unsigned)x >> 31;
    if (x == 0) {
        s.cls = 2;
    } else {
        s.exp = 60;
        if (s.sign) {
            if (x == (-2147483647 - 1))
                return -2147483648.0;
            s.frac = -x;
        } else {
            s.frac = x;
        }
        while (s.frac <= (~0UL >> 4)) {
            s.frac <<= 1;
            s.exp--;
        }
    }
    pk_412550(&s);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", dptoli);
#ifdef SKIP_ASM
extern void func_00411C38(long *, int *);
struct S412608 { unsigned cls; int sign; int exp; int pad; unsigned long frac; };
/**
 * Converts a double to a 32-bit signed integer in software, truncating toward zero (fp-bit.c's df_to_si,
 * GOFAST dptoli).
 *
 * Zero, NaN and magnitudes below 1 give 0; infinity and values with an exponent of 31 or more saturate to
 * 0x80000000 or 0x7FFFFFFF by sign.
 *
 * @param value  the double to convert, as its raw 64-bit pattern
 * @return the truncated integer
 */
int dptoli(long value) {
    struct S412608 s;
    int r;
    func_00411C38(&value, (int *)&s);
    if ((s.cls ^ 2) == 0)
        return 0;
    if (s.cls < 2)
        return 0;
    if ((s.cls ^ 4) == 0)
        goto sat;
    if (s.exp < 0)
        return 0;
    if (s.exp >= 31) {
sat:
        return s.sign ? 0x80000000 : 0x7FFFFFFF;
    }
    r = (int)(s.frac >> (60 - s.exp));
    return s.sign ? -r : r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", dptoul);
#ifdef SKIP_ASM
extern void func_00411C38(long *, int *);
struct S4126A0 { unsigned cls; int sign; int exp; int pad; unsigned long frac; };
/**
 * Converts a double to a 32-bit unsigned integer in software, truncating toward zero (fp-bit.c's df_to_usi,
 * GOFAST dptoul).
 *
 * Zero, NaN, negative values (even -infinity) and magnitudes below 1 give 0; infinity and values of 2^32 or
 * more give 0xFFFFFFFF. The left-shift branch for exponents of 61 or more can never run, as in fp-bit.c.
 *
 * @param value  the double to convert, as its raw 64-bit pattern
 * @return the truncated unsigned integer
 */
unsigned dptoul(long value) {
    struct S4126A0 s;
    func_00411C38(&value, (int *)&s);
    if ((s.cls ^ 2) == 0)
        return 0;
    if (s.cls < 2)
        return 0;
    if (s.sign != 0)
        return 0;
    if ((s.cls ^ 4) == 0)
        goto sat;
    if (s.exp < 0)
        return 0;
    if (s.exp >= 32) {
sat:
        return 0xFFFFFFFF;
    }
    if (s.exp >= 61)
        return s.frac << (s.exp - 60);
    else
        return s.frac >> (60 - s.exp);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_00412740);
#ifdef SKIP_ASM
extern void func_00411C38(long *, int *);
void func_00412740(long a) {
    int buf[8];
    func_00411C38(&a, buf);
    buf[1] = buf[1] == 0;
    func_00411B08((struct S412778 *)buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", func_00412778);
#ifdef SKIP_ASM
struct S412778 { int a, b, c; long d; };
extern void func_00411B08(struct S412778 *);
void func_00412778(int a, int b, int c, long d) {
    struct S412778 s;
    s.a = a;
    s.b = b;
    s.c = c;
    s.d = d;
    func_00411B08(&s);
}
#endif

//100%
INCLUDE_ASM("seg/seg_312310", dptofp);
#ifdef SKIP_ASM
/* fp-bit.c df_to_sf (GCC runtime, GPL v2+ with the libgcc linking exception): dptofp (US_SOFTWARE_GOFAST) */
extern SFtype __make_fp (fp_class_type, unsigned int, int, USItype) __asm__("func_00413AC8");

/**
 * Converts a double to a float in software (libgcc's df_to_sf).
 *
 * Unpacks the double, shifts its fraction down to single precision with the lowest guard bit set when any
 * discarded bit was nonzero, and lets __make_fp() round and pack the result.
 *
 * @param arg_a  the double to convert
 * @return the value as a float
 */
SFtype
df_to_sf (DFtype arg_a)
{
  fp_number_type in;
  USItype sffrac;

  unpack_d ((FLO_union_type *) & arg_a, &in);

  sffrac = in.fraction.ll >> F_D_BITOFF;

  /* We set the lowest guard bit in SFFRAC if we discarded any non
     zero bits.  */
  if ((in.fraction.ll & (((USItype) 1 << F_D_BITOFF) - 1)) != 0)
    sffrac |= 1;

  return __make_fp (in.class, in.sign, in.normal_exp, sffrac);
}
#endif

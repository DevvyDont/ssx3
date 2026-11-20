/* GNU CC runtime support, reconstructed: fp-bit.c (software single precision, GOFAST names) and frame.c
   (DWARF2 frame unwind tables). Originals: Copyright (C) 1994-1998 / 1997, 1998 Free Software Foundation, Inc.
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
INCLUDE_ASM("seg/seg_313F58", func_00412F58);
#ifdef SKIP_ASM
/* fp-bit.c (GCC runtime, GPL v2+ with the libgcc linking exception): shared header part, single-precision copy.
   libgcc built this copy with FLOAT and US_SOFTWARE_GOFAST, plus FLOAT_BIT_ORDER_MISMATCH (the mips t-* makefile
   fragments prepend it for little-endian targets: pack/unpack go through the `bits' bitfields). Conditionals
   resolved for that: no SMALL_MACHINE, NO_NANS, FLOAT_ONLY, _DEBUG_BITFLOAT or FLOAT_WORD_ORDER_MISMATCH. */

typedef float SFtype __attribute__ ((mode (SF)));
typedef float DFtype __attribute__ ((mode (DF)));

typedef int HItype __attribute__ ((mode (HI)));
typedef int SItype __attribute__ ((mode (SI)));
typedef int DItype __attribute__ ((mode (DI)));

/* The type of the result of a fp compare */
#define CMPtype SItype

typedef unsigned int UHItype __attribute__ ((mode (HI)));
typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef unsigned int UDItype __attribute__ ((mode (DI)));

#define MAX_SI_INT   ((SItype) ((unsigned) (~0)>>1))
#define MAX_USI_INT  ((USItype) ~0)

/* FLOAT */
#define NGARDS    7L
#define GARDROUND 0x3f
#define GARDMASK  0x7f
#define GARDMSB   0x40
#define EXPBITS 8
#define EXPBIAS 127
#define FRACBITS 23
#define EXPMAX (0xff)
#define QUIET_NAN 0x100000L
#define FRAC_NBITS 32
#define FRACHIGH  0x80000000L
#define FRACHIGH2 0xc0000000L
#define pack_d __pack_f
#define unpack_d __unpack_f
#define __fpcmp_parts __fpcmp_parts_f
typedef USItype fractype;
typedef UHItype halffractype;
typedef SFtype FLO_type;
typedef SItype intfrac;

/* US_SOFTWARE_GOFAST, FLOAT */
#define add 		fpadd
#define sub 		fpsub
#define multiply 	fpmul
#define divide 		fpdiv
#define compare 	fpcmp
#define si_to_float 	sitofp
#define float_to_si 	fptosi
#define float_to_usi 	fptoui
#define negate 		__negsf2
#define sf_to_df	fptodp
#define dptofp 		dptofp

#define INLINE __inline__

/* Preserve the sticky-bit when shifting fractions to the right.  */
#define LSHIFT(a) { a = (a & 1) | (a >> 1); }

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

/* IEEE "special" number predicates */

/* nan()'s `static fp_number_type thenan;' is the bss object D_0052ADD8, which is not split into this unit, so the
   object is declared here and nan () returns its address. */
extern fp_number_type thenan __asm__("D_0052ADD8");

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

/* Entry points, under the unit's retail labels (asm-label aliases). */
extern FLO_type pack_d ( fp_number_type * ) __asm__("func_00412F58");
extern void unpack_d (FLO_union_type *, fp_number_type *) __asm__("func_00413068");
int __fpcmp_parts (fp_number_type * a, fp_number_type *b) __asm__("func_00413748");

/* frame.c (GCC runtime, GPL v2+ with the libgcc linking exception): shared header part. Built with
   DWARF2_UNWIND_INFO, inhibit_libc (no libc headers: the libc calls below are declared with asm labels to the
   retail C library's addresses; memcpy stays the undeclared builtin, which retail inlines as a block move) and
   gthr-single.h (the mutex calls are no-ops). */

typedef unsigned int size_t;	/* <stddef.h> */

/* ee-gcc's r5900 FIRST_PSEUDO_REGISTER (tm.h): retail's struct frame_state is 0x2F0 bytes, saved[] at 0x29C. */
#define FIRST_PSEUDO_REGISTER 79

/* frame.h */
typedef struct frame_state
{
  void *cfa;
  void *eh_ptr;
  long cfa_offset;
  long args_size;
  long reg_or_offset[FIRST_PSEUDO_REGISTER+1];
  unsigned short cfa_reg;
  unsigned short retaddr_column;
  char saved[FIRST_PSEUDO_REGISTER+1];
} frame_state;

/* Values for 'saved' above.  */
#define REG_UNSAVED 0
#define REG_SAVED_OFFSET 1
#define REG_SAVED_REG 2

struct object {
  void *pc_begin;
  void *pc_end;
  struct dwarf_fde *fde_begin;
  struct dwarf_fde **fde_array;
  size_t count;
  struct object *next;
};

/* dwarf2.h: call frame information */
enum dwarf_call_frame_info
  {
    DW_CFA_advance_loc = 0x40,
    DW_CFA_offset = 0x80,
    DW_CFA_restore = 0xc0,
    DW_CFA_nop = 0x00,
    DW_CFA_set_loc = 0x01,
    DW_CFA_advance_loc1 = 0x02,
    DW_CFA_advance_loc2 = 0x03,
    DW_CFA_advance_loc4 = 0x04,
    DW_CFA_offset_extended = 0x05,
    DW_CFA_restore_extended = 0x06,
    DW_CFA_undefined = 0x07,
    DW_CFA_same_value = 0x08,
    DW_CFA_register = 0x09,
    DW_CFA_remember_state = 0x0a,
    DW_CFA_restore_state = 0x0b,
    DW_CFA_def_cfa = 0x0c,
    DW_CFA_def_cfa_register = 0x0d,
    DW_CFA_def_cfa_offset = 0x0e,
    /* SGI/MIPS specific */
    DW_CFA_MIPS_advance_loc8 = 0x1d,

    /* GNU extensions */
    DW_CFA_GNU_window_save = 0x2d,
    DW_CFA_GNU_args_size = 0x2e,
    DW_CFA_GNU_negative_offset_extended = 0x2f
  };

/* gthr-single.h */
typedef int __gthread_mutex_t;

static inline int
__gthread_mutex_lock (__gthread_mutex_t *mutex __attribute__ ((__unused__)))
{
  return 0;
}

static inline int
__gthread_mutex_unlock (__gthread_mutex_t *mutex __attribute__ ((__unused__)))
{
  return 0;
}

/* frame.c's `static __gthread_mutex_t object_mutex = __GTHREAD_MUTEX_INIT;': its storage is not split into this
   unit and the gthr-single calls never use it, so it is only declared. */
extern __gthread_mutex_t object_mutex;

/* Some types used by the DWARF 2 spec.  */

typedef          int  sword __attribute__ ((mode (SI)));
typedef unsigned int  uword __attribute__ ((mode (SI)));
typedef unsigned int  uaddr __attribute__ ((mode (pointer)));
typedef          int  saddr __attribute__ ((mode (pointer)));
typedef unsigned char ubyte;

struct dwarf_cie {
  uword length;
  sword CIE_id;
  ubyte version;
  char augmentation[0];
} __attribute__ ((packed, aligned (__alignof__ (void *))));

/* The first few fields of an FDE.  */

struct dwarf_fde {
  uword length;
  sword CIE_delta;
  void* pc_begin;
  uaddr pc_range;
} __attribute__ ((packed, aligned (__alignof__ (void *))));

typedef struct dwarf_fde fde;

/* Objects to be searched for frame unwind info: `static struct object *objects;', the bss object D_0052ADE8. */
extern struct object *objects __asm__("D_0052ADE8");

/* The information we care about from a CIE.  */

struct cie_info {
  char *augmentation;
  void *eh_ptr;
  int code_align;
  int data_align;
  unsigned ra_regno;
};

/* The current unwind state, plus a saved copy for DW_CFA_remember_state.  */

struct frame_state_internal
{
  struct frame_state s;
  struct frame_state_internal *saved_state;
};

/* This is undefined below if we need it to be an actual function.  */
#define init_object_mutex_once()

/* Read unaligned data from the instruction buffer.  */

union unaligned {
  void *p;
  unsigned b2 __attribute__ ((mode (HI)));
  unsigned b4 __attribute__ ((mode (SI)));
  unsigned b8 __attribute__ ((mode (DI)));
} __attribute__ ((packed));
static inline void *
read_pointer (void *p)
{ union unaligned *up = p; return up->p; }
static inline unsigned
read_1byte (void *p)
{ return *(unsigned char *)p; }
static inline unsigned
read_2byte (void *p)
{ union unaligned *up = p; return up->b2; }
static inline unsigned
read_4byte (void *p)
{ union unaligned *up = p; return up->b4; }
static inline unsigned long
read_8byte (void *p)
{ union unaligned *up = p; return up->b8; }

/* Ordering function for FDEs.  Functions can't overlap, so we just compare
   their starting addresses.  */

static inline saddr
fde_compare (fde *x, fde *y)
{
  return (saddr)x->pc_begin - (saddr)y->pc_begin;
}

/* Return the address of the FDE after P.  */

static inline fde *
next_fde (fde *p)
{
  return (fde *)(((char *)p) + p->length + sizeof (p->length));
}

typedef struct fde_vector
{
  fde **array;
  size_t count;
} fde_vector;

typedef struct fde_accumulator
{
  fde_vector linear;
  fde_vector erratic;
} fde_accumulator;

static inline struct dwarf_cie *
get_cie (fde *f)
{
  return ((void *)&f->CIE_delta) - f->CIE_delta;
}

/* C library (inhibit_libc: no headers), at the retail C library's labels. */
extern void *malloc (size_t) __asm__("func_004188C0");
extern void free (void *) __asm__("func_00418910");
extern void abort (void) __asm__("func_00418790") __attribute__ ((noreturn));
extern void *memset (void *, int, size_t) __asm__("func_00416210");

/* frame.c functions, under the unit's retail labels (asm-label aliases). The ones whose bodies are landed under
   their func_ names are declared extern here (static in frame.c; fde_split is a static inline that is too big to
   inline, so retail calls an out-of-line copy). */
extern void *decode_uleb128 (unsigned char *buf, unsigned *r) __asm__("func_00413B38");
extern void *decode_sleb128 (unsigned char *buf, int *r) __asm__("func_00413B80");
extern void fde_split (fde_vector *linear, fde_vector *erratic) __asm__("func_004149F0");
extern void fde_merge (fde_vector *v1, const fde_vector *v2) __asm__("func_00413BD0");
extern void frame_init (struct object* ob) __asm__("func_00414008");
extern void *extract_cie_info (fde *f, struct cie_info *c) __asm__("func_00414230");
static fde **end_fde_sort (fde_accumulator *accu, size_t count) __asm__("func_00413CD0");
static fde *find_fde (void *pc) __asm__("func_00414148");
static void *execute_cfa_insn (void *p, struct frame_state_internal *state,
			       struct cie_info *info, void **pc) __asm__("func_00414338");

/* fp-bit.c pack_d (__pack_f, single precision) (GCC runtime, GPL v2+ with the libgcc linking exception) */
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

  /* FLOAT_BIT_ORDER_MISMATCH: the bitfield stores */
  dst.bits.fraction = fraction;
  dst.bits.exp = exp;
  dst.bits.sign = sign;

  return dst.value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413068);
#ifdef SKIP_ASM
struct FB {
    unsigned m : 23;
    unsigned e : 8;
    unsigned s : 1;
};
/**
 * Splits an IEEE float into the unpacked fp-bit form (class, sign, exponent, guarded fraction).
 *
 * Writes four words to dst: class, sign (bit 31), normal_exp and fraction. A zero exponent gives class 2 (zero)
 * whatever the fraction, so denormals read as zero. Exponent 0xFF gives class 4 (infinity) for a zero fraction, else
 * class 1 (quiet NaN) when bit 0x100000 is set or 0 (signalling NaN), keeping the raw fraction. Anything else is class
 * 3 with normal_exp = exponent - 127 and fraction = (mantissa << 7) | 0x40000000 (the implicit one at bit 30, 7 guard
 * bits).
 *
 * @param src  the float to split (read through a pointer)
 * @param dst  the four-word unpacked result
 */
void func_00413068(float *src, int *dst) {
    struct FB *bits = (struct FB *)src;
    unsigned fraction = bits->m;
    int exponent = bits->e;
    dst[1] = bits->s;
    if (exponent == 0) {
        dst[0] = 2;
        return;
    }
    if (exponent == 0xFF) {
        if (fraction == 0) {
            dst[0] = 4;
            return;
        }
        if (fraction & 0x100000)
            dst[0] = 1;
        else
            dst[0] = 0;
        dst[3] = fraction;
        return;
    }
    dst[2] = exponent - 0x7F;
    dst[0] = 3;
    dst[3] = (fraction << 7) | 0x40000000;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004130F8);
#ifdef SKIP_ASM
/* fp-bit.c _fpadd_parts (single precision) (GCC runtime, GPL v2+ with the libgcc linking exception) */
static fp_number_type *
_fpadd_parts (fp_number_type * a,
	      fp_number_type * b,
	      fp_number_type * tmp) __asm__("func_004130F8");

/**
 * Adds two unpacked single-precision numbers and returns a pointer to the unpacked sum.
 *
 * A NaN operand is returned as is (lhs first). Infinities of opposite sign give the shared NaN D_0052ADD8; otherwise an
 * infinite operand is returned. Zero plus zero is written to tmp with the AND of both signs; a single zero operand
 * returns the other one. Otherwise the fraction with the smaller exponent is shifted right one bit at a time (keeping a
 * sticky bit) until the exponents agree, or dropped when they differ by 32 or more; the fractions are added or
 * subtracted by sign into tmp, renormalised left after a subtraction and shifted right once when the sum carries past
 * bit 31.
 *
 * @param lhs  first operand
 * @param rhs  second operand
 * @param tmp  storage for a computed result
 * @return lhs, rhs, the shared NaN or tmp
 */
static fp_number_type *
_fpadd_parts (fp_number_type * lhs,
	      fp_number_type * rhs,
	      fp_number_type * tmp)
{
  intfrac tfraction;

  /* Put commonly used fields in local variables.  */
  int a_normal_exp;
  int b_normal_exp;
  fractype a_fraction;
  fractype b_fraction;

  if (isnan (lhs))
    {
      return lhs;
    }
  if (isnan (rhs))
    {
      return rhs;
    }
  if (isinf (lhs))
    {
      /* Adding infinities with opposite signs yields a NaN.  */
      if (isinf (rhs) && lhs->sign != rhs->sign)
	return nan ();
      return lhs;
    }
  if (isinf (rhs))
    {
      return rhs;
    }
  if (iszero (rhs))
    {
      if (iszero (lhs))
	{
	  *tmp = *lhs;
	  tmp->sign = lhs->sign & rhs->sign;
	  return tmp;
	}
      return lhs;
    }
  if (iszero (lhs))
    {
      return rhs;
    }

  /* Got two numbers. shift the smaller and increment the exponent till
     they're the same */
  {
    int diff;

    a_normal_exp = lhs->normal_exp;
    b_normal_exp = rhs->normal_exp;
    a_fraction = lhs->fraction.ll;
    b_fraction = rhs->fraction.ll;

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

  if (lhs->sign != rhs->sign)
    {
      if (lhs->sign)
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
      tmp->sign = lhs->sign;
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
INCLUDE_ASM("seg/seg_313F58", func_00413330);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int *func_004130F8(int *, int *, int *);
extern int func_00412F58(int *);

/**
 * Adds two floats in software.
 *
 * Unpacks both operands (func_00413068), adds the parts (func_004130F8) and packs the result (func_00412F58).
 *
 * @param x  first operand
 * @param y  second operand
 * @return the sum, as func_00412F58 leaves it in the float return register (the C declaration says int)
 */
int func_00413330(float x, float y) {
    int unpackedX[4];
    int unpackedY[4];
    int sum[4];
    float fx = x;
    float fy = y;
    func_00413068(&fx, unpackedX);
    func_00413068(&fy, unpackedY);
    return func_00412F58(func_004130F8(unpackedX, unpackedY, sum));
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413388);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int *func_004130F8(int *, int *, int *);
extern int func_00412F58(int *);

/**
 * Subtracts one float from another in software.
 *
 * Unpacks both operands (func_00413068), flips the second one's sign, adds the parts (func_004130F8) and packs the
 * result (func_00412F58).
 *
 * @param x  minuend
 * @param y  subtrahend
 * @return x - y, as func_00412F58 leaves it in the float return register (the C declaration says int)
 */
int func_00413388(float x, float y) {
    int unpackedX[4];
    int unpackedY[4];
    int difference[4];
    float fx = x;
    float fy = y;
    func_00413068(&fx, unpackedX);
    func_00413068(&fy, unpackedY);
    unpackedY[1] ^= 1;
    return func_00412F58(func_004130F8(unpackedX, unpackedY, difference));
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004133F0);
#ifdef SKIP_ASM
/* fp-bit.c multiply (fpmul, single precision; _fpmul_parts inlined) (GCC runtime, GPL v2+ with the libgcc linking exception) */
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
    /* FLOAT (not NO_DI_MODE) */
    {
      /* Multiplying two 32 bit numbers to get a 64 bit number  on 
        a machine with DI, so we're safe */

      DItype answer = (DItype)(a->fraction.ll) * (DItype)(b->fraction.ll);
      
      high = answer >> 32;
      low = answer;
    }
  }

  tmp->normal_exp = a->normal_exp + b->normal_exp;
  tmp->sign = a->sign != b->sign;
  /* FLOAT */
  tmp->normal_exp += 2;		/* ??????????????? */
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

FLO_type
multiply (FLO_type arg_a, FLO_type arg_b) __asm__("func_004133F0");

/**
 * Multiplies two floats in software.
 *
 * Unpacks both operands and multiplies the parts inline: a NaN operand is returned with the XOR of the signs; infinity
 * times zero gives the shared NaN D_0052ADD8; an infinite or zero operand is returned with the XOR sign. Otherwise the
 * 32-bit fractions are multiplied into 64 bits, the exponent is the sum plus 2, the high word is normalised into [1 <<
 * 30, 1 << 31) (low bits carried along) and rounded to nearest even with the low word as sticky bits. The result is
 * packed with func_00412F58.
 *
 * @param arg_a  first factor
 * @param arg_b  second factor
 * @return the product
 */
FLO_type
multiply (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type lhs;
  fp_number_type rhs;
  fp_number_type product;
  fp_number_type *res;

  unpack_d ((FLO_union_type *) & arg_a, &lhs);
  unpack_d ((FLO_union_type *) & arg_b, &rhs);

  res = _fpmul_parts (&lhs, &rhs, &product);

  return pack_d (res);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004135E8);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern float func_00412F58_f(int *) __asm__("func_00412F58");
extern int D_0052ADD8[];

/**
 * Divides one float by another in software.
 *
 * Unpacks both operands (func_00413068). A NaN dividend, else a NaN divisor, is returned as is. The sign is the XOR of
 * both. Infinity / infinity and zero / zero give the shared NaN D_0052ADD8; an infinite or zero dividend is returned; a
 * finite value over infinity becomes zero (fraction and exponent cleared); a finite value over zero becomes infinity.
 * Otherwise the exponents are subtracted (one less when the dividend's fraction is the smaller, which is then doubled),
 * 31 quotient bits are produced by shift-and-subtract from bit 30 down, and the quotient is rounded to nearest even
 * (+0x40 at a guard pattern of 0x40 when bit 7 is set or a remainder is left). The result is packed with func_00412F58.
 *
 * @param x  dividend
 * @param y  divisor
 * @return x / y
 */
float func_004135E8(float x, float y) {
    int dividend[4];
    int divisor[4];
    float fx = x;
    float fy = y;
    int *result;
    unsigned n, divisorFraction, quotient, bit;
    int *quotientParts;
    func_00413068(&fx, dividend);
    func_00413068(&fy, divisor);
    quotientParts = dividend;
    if ((unsigned)dividend[0] < 2) {
        result = dividend;
    } else if ((unsigned)divisor[0] < 2) {
        result = divisor;
    } else {
        dividend[1] ^= divisor[1];
        if ((dividend[0] ^ 4) == 0 || (dividend[0] ^ 2) == 0) {
            result = dividend;
            if (dividend[0] == divisor[0])
                result = D_0052ADD8;
        } else if ((divisor[0] ^ 4) == 0) {
            dividend[3] = 0;
            dividend[2] = 0;
            result = dividend;
        } else if ((divisor[0] ^ 2) == 0) {
            dividend[0] = 4;
            result = dividend;
        } else {
            dividend[2] = dividend[2] - divisor[2];
            n = dividend[3];
            divisorFraction = divisor[3];
            if (n < divisorFraction) {
                dividend[2]--;
                n <<= 1;
            }
            bit = 0x40000000;
            quotient = 0;
            for (; bit != 0; bit >>= 1) {
                if (n >= divisorFraction) {
                    quotient |= bit;
                    n -= divisorFraction;
                }
                n <<= 1;
            }
            if ((quotient & 0x7F) == 0x40) {
                if (quotient & 0x80)
                    quotient += 0x40;
                else if (n != 0)
                    quotient += 0x40;
            }
            quotientParts[3] = quotient;
            result = quotientParts;
        }
    }
    return func_00412F58_f(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413748);
#ifdef SKIP_ASM
/* according to the demo, fpcmp returns a comparison with 0... thus
   a<b -> -1
   a==b -> 0
   a>b -> +1
 */

/**
 * Compares two unpacked single-precision numbers.
 *
 * Returns 1 when either is a NaN (unordered must be nonzero). Two infinities compare by sign (rhs sign - lhs sign); a
 * single infinity or a single zero decides by the signs; two zeros are equal. Otherwise opposite signs decide, then the
 * exponents, then the fractions, each flipped for negative numbers.
 *
 * @param lhs  first operand
 * @param rhs  second operand
 * @return -1, 0 or 1 for lhs <, == or > rhs; 1 when unordered
 */
int
__fpcmp_parts (fp_number_type * lhs, fp_number_type * rhs)
{
  if (isnan (lhs) || isnan (rhs))
    {
      return 1;			/* how to indicate unordered compare? */
    }
  if (isinf (lhs) && isinf (rhs))
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
      return rhs->sign - lhs->sign;
    }
  /* but not both... */
  if (isinf (lhs))
    {
      return lhs->sign ? -1 : 1;
    }
  if (isinf (rhs))
    {
      return rhs->sign ? 1 : -1;
    }
  if (iszero (lhs) && iszero (rhs))
    {
      return 0;
    }
  if (iszero (lhs))
    {
      return rhs->sign ? 1 : -1;
    }
  if (iszero (rhs))
    {
      return lhs->sign ? -1 : 1;
    }
  /* now both are "normal". */
  if (lhs->sign != rhs->sign)
    {
      /* opposite signs */
      return lhs->sign ? -1 : 1;
    }
  /* same sign; exponents? */
  if (lhs->normal_exp > rhs->normal_exp)
    {
      return lhs->sign ? -1 : 1;
    }
  if (lhs->normal_exp < rhs->normal_exp)
    {
      return lhs->sign ? 1 : -1;
    }
  /* same exponents; check size. */
  if (lhs->fraction.ll > rhs->fraction.ll)
    {
      return lhs->sign ? -1 : 1;
    }
  if (lhs->fraction.ll < rhs->fraction.ll)
    {
      return lhs->sign ? 1 : -1;
    }
  /* after all that, they're equal. */
  return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413860);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int func_00413748(int *, int *);

/**
 * Compares two floats in software.
 *
 * Unpacks both operands (func_00413068) and compares the parts with func_00413748.
 *
 * @param x  first operand
 * @param y  second operand
 * @return -1, 0 or 1 for x <, == or > y; 1 when either is a NaN
 */
int func_00413860(float x, float y) {
    int unpackedX[4];
    int unpackedY[4];
    float fx = x;
    float fy = y;
    func_00413068(&fx, unpackedX);
    func_00413068(&fy, unpackedY);
    return func_00413748(unpackedX, unpackedY);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004138B0);
#ifdef SKIP_ASM
extern float func_00412F58_f(int *) __asm__("func_00412F58");

/**
 * Converts an int to a float in software.
 *
 * Zero gives class zero. -2147483648 returns -2147483648.0f directly. Otherwise the magnitude becomes the fraction with
 * exponent 30, shifted left until bit 30 is set (decrementing the exponent), and is packed with func_00412F58.
 *
 * @param x  the integer
 * @return the float value
 */
float func_004138B0(int x) {
    int unpacked[4];
    unsigned fraction, shifted;
    int exponent;
    unpacked[0] = 3;
    unpacked[1] = (unsigned)x >> 31;
    if (x == 0) {
        unpacked[0] = 2;
    } else {
        unpacked[2] = 0x1E;
        if (unpacked[1]) {
            if (x == 0x80000000)
                return -2147483648.0f;
            unpacked[3] = -x;
        } else {
            unpacked[3] = x;
        }
        fraction = unpacked[3];
        if (fraction <= 0x3FFFFFFF) {
            exponent = unpacked[2];
            do {
                shifted = fraction << 1;
                exponent--;
                fraction = shifted;
            } while (shifted <= 0x3FFFFFFF);
            unpacked[2] = exponent;
            unpacked[3] = shifted;
        }
    }
    return func_00412F58_f(unpacked);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413968);
#ifdef SKIP_ASM
/**
 * Converts a float to an int in software, truncating toward zero and saturating.
 *
 * Zero and NaNs give 0; infinities and exponents of 31 or more saturate to 0x80000000 or 0x7FFFFFFF by sign; negative
 * exponents give 0. Otherwise the fraction is shifted right by 30 - exponent and negated for negative inputs.
 *
 * @param x  the float
 * @return the truncated integer
 */
int func_00413968(float x) {
    int unpacked[4];
    float value = x;
    unsigned fpClass;
    unsigned magnitude;
    func_00413068(&value, unpacked);
    fpClass = unpacked[0];
    if ((fpClass ^ 2) == 0)
        return 0;
    if (fpClass < 2)
        return 0;
    if ((fpClass ^ 4) == 0)
        goto sat;
    if (unpacked[2] < 0)
        return 0;
    if (unpacked[2] >= 0x1F) {
sat:
        return unpacked[1] ? 0x80000000 : 0x7FFFFFFF;
    }
    magnitude = (unsigned)unpacked[3] >> (0x1E - unpacked[2]);
    return unpacked[1] ? -magnitude : magnitude;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004139F8);
#ifdef SKIP_ASM
/**
 * Converts a float to an unsigned int in software, truncating and saturating.
 *
 * Zero, NaNs and negative values give 0; infinities and exponents of 32 or more give 0xFFFFFFFF; negative exponents
 * give 0. Exponent 31 shifts the fraction left by one, smaller ones shift it right by 30 - exponent.
 *
 * @param x  the float
 * @return the truncated unsigned value
 */
unsigned func_004139F8(float x) {
    int unpacked[4];
    float value = x;
    unsigned fpClass;
    func_00413068(&value, unpacked);
    fpClass = unpacked[0];
    if ((fpClass ^ 2) == 0)
        return 0;
    if (fpClass < 2)
        return 0;
    if (unpacked[1])
        return 0;
    if ((fpClass ^ 4) == 0)
        goto sat;
    if (unpacked[2] < 0)
        return 0;
    if (unpacked[2] >= 0x20) {
sat:
        return 0xFFFFFFFF;
    }
    if (unpacked[2] >= 0x1F)
        return (unsigned)unpacked[3] << (unpacked[2] - 0x1E);
    return (unsigned)unpacked[3] >> (0x1E - unpacked[2]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413A90);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int func_00412F58(int *);
/**
 * Negates a float in software.
 *
 * Unpacks it (func_00413068), sets sign = !sign and packs it again (func_00412F58).
 *
 * The C reconstruction declares its parts buffer as int obj[2] although func_00413068 writes four words; retail
 * reserves 16 bytes there, so a port must widen it.
 *
 * @param x  the value
 * @return -x, as func_00412F58 leaves it in the float return register (the C declaration says int)
 */
int func_00413A90(float x) {
    int obj[2];
    float value = x;
    func_00413068(&value, obj);
    obj[1] = obj[1] == 0;
    return func_00412F58(obj);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413AC8);
#ifdef SKIP_ASM
/**
 * Builds a float from its unpacked fields: class, sign, exponent and guarded fraction.
 *
 * Stores the four arguments into an fp_number_type on the stack and packs it with func_00412F58. Declared void, but the
 * packed float is left in the float return register for the caller; dptofp (the double-to-float conversion) calls it
 * that way.
 *
 * @param fpClass   fp-bit class (0 signalling NaN, 1 quiet NaN, 2 zero, 3 number, 4 infinity)
 * @param sign      sign bit
 * @param exponent  unbiased exponent
 * @param fraction  fraction with the implicit one at bit 30
 */
void func_00413AC8(int fpClass, int sign, int exponent, int fraction) {
    int parts[4];
    parts[0] = fpClass;
    parts[1] = sign;
    parts[2] = exponent;
    parts[3] = fraction;
    func_00412F58(parts);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", fptodp);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int func_00412778(int, int, int, unsigned long long);
/**
 * Converts a float to a double in software (fp-bit.c's GOFAST fptodp).
 *
 * Unpacks the float (func_00413068, unpack_d) and packs the parts as a double (func_00412778), shifting the
 * fraction into the double's position.
 *
 * @param x  the value to convert
 * @return the packed double, left by func_00412778() in the return registers (the C declaration says int)
 */
int fptodp(float x) {
    int unpacked[4];
    float value = x;
    func_00413068(&value, unpacked);
    return func_00412778(unpacked[0], unpacked[1], unpacked[2], ((unsigned long long)(long long)unpacked[3] << 32) >> 2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413B38);
#ifdef SKIP_ASM
/**
 * Decodes an unsigned LEB128 value and returns the address after it.
 *
 * Takes 7 bits per byte, low group first, while bit 7 is set.
 *
 * @param buf  the encoded bytes
 * @param out  receives the value
 * @return the first byte after the value
 */
unsigned char *func_00413B38(unsigned char *buf, unsigned *out) {
    unsigned curByte = *buf++;
    unsigned value = curByte & 0x7F;
    int shift = 0;
    if (curByte & 0x80) {
        do {
            curByte = *buf++;
            shift += 7;
            value |= (curByte & 0x7F) << shift;
        } while (curByte & 0x80);
    }
    *out = value;
    return buf;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413B80);
#ifdef SKIP_ASM
/**
 * Decodes a signed LEB128 value and returns the address after it.
 *
 * Takes 7 bits per byte while bit 7 is set and sign-extends from the last group when bit 6 of the last byte is set and
 * fewer than 32 bits were read.
 *
 * @param buf  the encoded bytes
 * @param out  receives the value
 * @return the first byte after the value
 */
unsigned char *func_00413B80(unsigned char *buf, int *out) {
    unsigned shift = 0;
    int value = 0;
    unsigned curByte;
    do {
        curByte = *buf++;
        value |= (curByte & 0x7F) << shift;
        shift += 7;
    } while (curByte & 0x80);
    if (shift < 32 && (curByte & 0x40))
        value |= -1 << shift;
    *out = value;
    return buf;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413BD0);
#ifdef SKIP_ASM
struct K13BD0 { int f0; int f4; int key; };
struct L13BD0 { struct K13BD0 **items; unsigned n; };
/**
 * Merges a sorted vector of FDE pointers into another sorted vector in place, from the back.
 *
 * For each source entry from the last one down, moves the destination entries whose pc_begin (+8) is larger up by the
 * number of source entries still to place, then stores the source entry; finally adds the source count to the
 * destination count. The destination array must already have room for both (func_00414008 allocates it for every FDE).
 *
 * @param dstVec  the linear (sorted) vector, extended in place
 * @param srcVec  the sorted erratic entries
 */
void func_00413BD0(struct L13BD0 *dstVec, struct L13BD0 *srcVec) {
    unsigned srcCount = srcVec->n;
    unsigned n;
    unsigned j;
    j = srcCount;
    if (srcCount != 0) {
        n = dstVec->n;
        do {
            struct K13BD0 *x;
            j--;
            x = srcVec->items[j];
            if (n != 0 && dstVec->items[n - 1]->key - x->key > 0) {
                do {
                    dstVec->items[n + j] = dstVec->items[n - 1];
                    n--;
                } while (n != 0 && dstVec->items[n - 1]->key - x->key > 0);
            }
            dstVec->items[n + j] = x;
        } while (j != 0);
        dstVec->n = dstVec->n + srcCount;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413CD0);
#ifdef SKIP_ASM
/* This is O(n log(n)).  BSD/OS defines heapsort in stdlib.h, so we must
   use a name that does not conflict.  */
static inline void
frame_heapsort (fde_vector *erratic)
{
  /* For a description of this algorithm, see:
     Samuel P. Harbison, Guy L. Steele Jr.: C, a reference manual, 2nd ed.,
     p. 60-61. */
  fde ** a = erratic->array;
  /* A portion of the array is called a "heap" if for all i>=0:
     If i and 2i+1 are valid indices, then a[i] >= a[2i+1].
     If i and 2i+2 are valid indices, then a[i] >= a[2i+2]. */
#define SWAP(x,y) do { fde * tmp = x; x = y; y = tmp; } while (0)
  size_t n = erratic->count;
  size_t m = n;
  size_t i;

  while (m > 0)
    {
      /* Invariant: a[m..n-1] is a heap. */
      m--;
      for (i = m; 2*i+1 < n; )
        {
          if (2*i+2 < n
              && fde_compare (a[2*i+2], a[2*i+1]) > 0
              && fde_compare (a[2*i+2], a[i]) > 0)
            {
              SWAP (a[i], a[2*i+2]);
              i = 2*i+2;
            }
          else if (fde_compare (a[2*i+1], a[i]) > 0)
            {
              SWAP (a[i], a[2*i+1]);
              i = 2*i+1;
            }
          else
            break;
        }
    }
  while (n > 1)
    {
      /* Invariant: a[0..n-1] is a heap. */
      n--;
      SWAP (a[0], a[n]);
      for (i = 0; 2*i+1 < n; )
        {
          if (2*i+2 < n
              && fde_compare (a[2*i+2], a[2*i+1]) > 0
              && fde_compare (a[2*i+2], a[i]) > 0)
            {
              SWAP (a[i], a[2*i+2]);
              i = 2*i+2;
            }
          else if (fde_compare (a[2*i+1], a[i]) > 0)
            {
              SWAP (a[i], a[2*i+1]);
              i = 2*i+1;
            }
          else
            break;
        }
    }
#undef SWAP
}

/**
 * Finishes sorting an FDE accumulator and returns the sorted array.
 *
 * Aborts (func_00418790) unless the linear vector holds count entries, splits the out-of-order entries into the erratic
 * vector (func_004149F0), aborts again unless both together still hold count, heapsorts the erratic entries by pc_begin
 * (inline), merges them back into the linear vector (func_00413BD0) and frees the erratic array (func_00418910).
 *
 * @param accu   the accumulator (linear and erratic vectors)
 * @param count  the number of FDEs added
 * @return the sorted linear array
 */
static fde **
end_fde_sort (fde_accumulator *accu, size_t count)
{
  if (accu->linear.count != count)
    abort ();
  fde_split (&accu->linear, &accu->erratic);
  if (accu->linear.count + accu->erratic.count != count)
    abort ();
  frame_heapsort (&accu->erratic);
  fde_merge (&accu->linear, &accu->erratic);
  free (accu->erratic.array);
  return accu->linear.array;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413F38);
#ifdef SKIP_ASM
struct N13F38 {
    int off;
    int a;
    int b;
};
/**
 * Counts the FDEs in one .eh_frame section.
 *
 * Walks length-prefixed entries until a zero length, counting those whose CIE pointer (+4) and pc_begin (+8) are both
 * nonzero (CIEs and discarded link-once FDEs are skipped).
 *
 * @param n  the first entry of the section
 * @return the number of usable FDEs
 */
int func_00413F38(struct N13F38 *n) {
    int cnt = 0;
    if (n->off != 0) {
        do {
            if (n->a != 0 && n->b != 0)
                cnt++;
            n = (struct N13F38 *)((char *)n + n->off + 4);
        } while (n->off);
    }
    return cnt;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413F80);
#ifdef SKIP_ASM
struct N13F80 {
    int off;
    int a;
    unsigned addr;
    int size;
};
struct L13F80 {
    struct N13F80 **arr;
    int cnt;
};
/**
 * Appends the FDEs of one .eh_frame section to an accumulator and widens the covered address range.
 *
 * For each usable FDE (nonzero CIE pointer and pc_begin) it appends the FDE pointer to the accumulator's linear array
 * and lowers *lo to pc_begin / raises *hi to pc_begin + pc_range when they extend the range.
 *
 * @param n            the first entry of the section
 * @param accumulator  the FDE accumulator (linear vector first)
 * @param lo           in/out: lowest pc_begin seen
 * @param hi           in/out: highest pc end seen
 */
void func_00413F80(struct N13F80 *n, struct L13F80 *accumulator, unsigned *lo, unsigned *hi) {
    unsigned mn = *lo;
    unsigned mx = *hi;
    if (n->off != 0) {
        do {
            if (n->a != 0 && n->addr != 0) {
                accumulator->arr[accumulator->cnt++] = n;
                if (n->addr < mn)
                    mn = n->addr;
                if (mx < n->addr + n->size)
                    mx = n->addr + n->size;
            }
            n = (struct N13F80 *)((char *)n + n->off + 4);
        } while (n->off);
    }
    *lo = mn;
    *hi = mx;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414008);
#ifdef SKIP_ASM
struct N14008 {
    unsigned lo;
    unsigned hi;
    struct N13F80 *root;
    struct N13F80 **roots;
    int cnt;
};
struct LL14008 {
    int *a;
    int na;
    int *b;
    int nb;
};
extern int *func_004188C0(int);
extern struct N13F80 **func_00413CD0(struct LL14008 *, int);
/**
 * Builds an unwind object's sorted FDE table and its covered address range on first use.
 *
 * Counts the FDEs (func_00413F38) of every section in the object's fde_array list (+0xC, null-terminated) when it is
 * set, else of the single section at fde_begin (+8), and stores the count (+0x10). Allocates the linear and erratic
 * arrays (func_004188C0, 4 bytes per FDE), adds every FDE (func_00413F80), sorts (func_00413CD0) and stores the sorted
 * array in fde_array and the range in pc_begin (+0) and pc_end (+4).
 *
 * @param n  the registered unwind object
 */
void func_00414008(struct N14008 *n) {
    struct N13F80 **pp;
    int cnt, bytes;
    struct LL14008 accumulator;
    unsigned lo, hi;
    if (n->roots != 0) {
        pp = n->roots;
        cnt = 0;
        while (*pp != 0) {
            cnt += func_00413F38((struct N13F38 *)*pp);
            pp++;
        }
    } else {
        cnt = func_00413F38((struct N13F38 *)n->root);
    }
    bytes = cnt * 4;
    n->cnt = cnt;
    accumulator.a = func_004188C0(bytes);
    accumulator.b = func_004188C0(bytes);
    accumulator.na = 0;
    accumulator.nb = 0;
    lo = 0xFFFFFFFF;
    hi = 0;
    if (n->roots != 0) {
        pp = n->roots;
        while (*pp != 0) {
            func_00413F80(*pp, (struct L13F80 *)&accumulator, &lo, &hi);
            pp++;
        }
    } else {
        func_00413F80(n->root, (struct L13F80 *)&accumulator, &lo, &hi);
    }
    n->roots = func_00413CD0(&accumulator, cnt);
    n->lo = lo;
    n->hi = hi;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414148);
#ifdef SKIP_ASM
/* Return a pointer to the FDE for the function containing PC.  */

/**
 * Finds the FDE describing the function that contains a code address.
 *
 * Walks the registered objects (D_0052ADE8), initialising each not yet sorted one (pc_begin 0) with func_00414008,
 * until one's [pc_begin, pc_end) contains pc, then binary-searches its sorted FDE array for the entry with pc_begin <=
 * pc < pc_begin + pc_range.
 *
 * @param pc  the code address
 * @return the FDE, or 0 when no registered object covers it
 */
static fde *
find_fde (void *pc)
{
  struct object *ob;
  size_t lo, hi;

  init_object_mutex_once ();
  __gthread_mutex_lock (&object_mutex);

  for (ob = objects; ob; ob = ob->next)
    {
      if (ob->pc_begin == 0)
	frame_init (ob);
      if (pc >= ob->pc_begin && pc < ob->pc_end)
	break;
    }

  __gthread_mutex_unlock (&object_mutex);

  if (ob == 0)
    return 0;

  /* Standard binary search algorithm.  */
  for (lo = 0, hi = ob->count; lo < hi; )
    {
      size_t i = (lo + hi) / 2;
      fde *entry = ob->fde_array[i];

      if (pc < entry->pc_begin)
	hi = i;
      else if (pc >= entry->pc_begin + entry->pc_range)
	lo = i + 1;
      else
	return entry;
    }

  return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414230);
#ifdef SKIP_ASM
extern int D_004998F0[];
extern int D_004998F8[];
extern int func_004165A8(int, int);
struct C14230 {
    char *aug;
    int f4;
    unsigned f8;
    int fC;
    int f10;
};
struct U14230 {
    int v;
} __attribute__((packed));
extern int strlen(char *);
/**
 * Reads the CIE that an FDE refers to into a cie_info and returns the address of its first instruction.
 *
 * Despite its name the first parameter is the FDE: the CIE is at the FDE's CIE-pointer field minus its value, and its
 * augmentation string at CIE + 9. Only "" (D_004998F0), "eh" (D_004998F8) and strings starting with 'z' are accepted
 * (func_004165A8 is strcmp); others return 0. "eh" is followed by a 4-byte eh_ptr (+4, else 0); then come the code
 * alignment (ULEB128, +8), the data alignment (SLEB128, +0xC) and the return-address column (one byte, +0x10); a 'z'
 * augmentation then skips its ULEB128-sized data block.
 *
 * @param cie  the FDE whose CIE is read
 * @param cieInfo    receives the CIE fields (augmentation string at +0)
 * @return the CIE's first call-frame instruction, or 0 for an unknown augmentation
 */
unsigned char *func_00414230(int *cie, struct C14230 *cieInfo) {
    char *augmentationEnd;
    unsigned char *cursor;
    unsigned augDataLength;
    cieInfo->aug = (char *)cie - cie[1] + 13;
    if (func_004165A8((int)cieInfo->aug, (int)D_004998F0) == 0 || func_004165A8((int)cieInfo->aug, (int)D_004998F8) == 0 || *cieInfo->aug == 'z') {
        augmentationEnd = cieInfo->aug + strlen(cieInfo->aug);
        cursor = (unsigned char *)augmentationEnd + 1;
        if (func_004165A8((int)cieInfo->aug, (int)D_004998F8) == 0) {
            cieInfo->f4 = ((struct U14230 *)cursor)->v;
            cursor += 4;
        } else {
            cieInfo->f4 = 0;
        }
        cursor = func_00413B38(cursor, &cieInfo->f8);
        cursor = func_00413B80(cursor, &cieInfo->fC);
        cieInfo->f10 = *cursor++;
        if (*cieInfo->aug == 'z')
            cursor = func_00413B38(cursor, &augDataLength) + augDataLength;
        return cursor;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414338);
#ifdef SKIP_ASM
/* Decode one instruction's worth of DWARF 2 call frame information.
   Used by __frame_state_for.  Takes pointers P to the instruction to
   decode, STATE to the current register unwind information, INFO to the
   current CIE information, and PC to the current PC value.  Returns a
   pointer to the next instruction.  */

/**
 * Decodes one DWARF 2 call-frame instruction, applies it to the unwind state and returns the next one.
 *
 * Handles the advance, offset and restore opcodes (low 6 bits as operand), set_loc, advance_loc1/2/4, the extended
 * offset/restore forms, register, def_cfa and its register/offset variants and GNU_args_size. remember_state saves a
 * heap copy of the state (func_004188C0) and restore_state copies it back and frees it; GNU_window_save marks registers
 * 16..31 as saved at SPARC window offsets. Unknown opcodes end the program (func_00418790).
 *
 * @param insnPtr  the instruction to decode
 * @param state    the unwind state being built
 * @param info     the CIE's alignment factors
 * @param pc       in/out: the address the instructions have reached
 * @return the address of the next instruction
 */
static void *
execute_cfa_insn (void *insnPtr, struct frame_state_internal *state,
		  struct cie_info *info, void **pc)
{
  unsigned insn = *(unsigned char *)insnPtr++;
  unsigned reg;
  int offset;

  if (insn & DW_CFA_advance_loc)
    *pc += ((insn & 0x3f) * info->code_align);
  else if (insn & DW_CFA_offset)
    {
      reg = (insn & 0x3f);
      insnPtr = decode_uleb128 (insnPtr, &offset);
      offset *= info->data_align;
      state->s.saved[reg] = REG_SAVED_OFFSET;
      state->s.reg_or_offset[reg] = offset;
    }
  else if (insn & DW_CFA_restore)
    {
      reg = (insn & 0x3f);
      state->s.saved[reg] = REG_UNSAVED;
    }
  else switch (insn)
    {
    case DW_CFA_set_loc:
      *pc = read_pointer (insnPtr);
      insnPtr += sizeof (void *);
      break;
    case DW_CFA_advance_loc1:
      *pc += read_1byte (insnPtr);
      insnPtr += 1;
      break;
    case DW_CFA_advance_loc2:
      *pc += read_2byte (insnPtr);
      insnPtr += 2;
      break;
    case DW_CFA_advance_loc4:
      *pc += read_4byte (insnPtr);
      insnPtr += 4;
      break;

    case DW_CFA_offset_extended:
      insnPtr = decode_uleb128 (insnPtr, &reg);
      insnPtr = decode_uleb128 (insnPtr, &offset);
      offset *= info->data_align;
      state->s.saved[reg] = REG_SAVED_OFFSET;
      state->s.reg_or_offset[reg] = offset;
      break;
    case DW_CFA_restore_extended:
      insnPtr = decode_uleb128 (insnPtr, &reg);
      state->s.saved[reg] = REG_UNSAVED;
      break;

    case DW_CFA_undefined:
    case DW_CFA_same_value:
    case DW_CFA_nop:
      break;

    case DW_CFA_register:
      {
	unsigned reg2;
	insnPtr = decode_uleb128 (insnPtr, &reg);
	insnPtr = decode_uleb128 (insnPtr, &reg2);
	state->s.saved[reg] = REG_SAVED_REG;
	state->s.reg_or_offset[reg] = reg2;
      }
      break;

    case DW_CFA_def_cfa:
      insnPtr = decode_uleb128 (insnPtr, &reg);
      insnPtr = decode_uleb128 (insnPtr, &offset);
      state->s.cfa_reg = reg;
      state->s.cfa_offset = offset;
      break;
    case DW_CFA_def_cfa_register:
      insnPtr = decode_uleb128 (insnPtr, &reg);
      state->s.cfa_reg = reg;
      break;
    case DW_CFA_def_cfa_offset:
      insnPtr = decode_uleb128 (insnPtr, &offset);
      state->s.cfa_offset = offset;
      break;
      
    case DW_CFA_remember_state:
      {
	struct frame_state_internal *save =
	  (struct frame_state_internal *)
	  malloc (sizeof (struct frame_state_internal));
	memcpy (save, state, sizeof (struct frame_state_internal));
	state->saved_state = save;
      }
      break;
    case DW_CFA_restore_state:
      {
	struct frame_state_internal *save = state->saved_state;
	memcpy (state, save, sizeof (struct frame_state_internal));
	free (save);
      }
      break;

      /* FIXME: Hardcoded for SPARC register window configuration.  */
    case DW_CFA_GNU_window_save:
      for (reg = 16; reg < 32; ++reg)
	{
	  state->s.saved[reg] = REG_SAVED_OFFSET;
	  state->s.reg_or_offset[reg] = (reg - 16) * sizeof (void *);
	}
      break;

    case DW_CFA_GNU_args_size:
      insnPtr = decode_uleb128 (insnPtr, &offset);
      state->s.args_size = offset;
      break;

    default:
      abort ();
    }
  return insnPtr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004146D0);
#ifdef SKIP_ASM
extern int *D_0052ADE8;
/**
 * Registers an .eh_frame section for unwinding, using an object record the caller supplies.
 *
 * Sets the record's fde_begin (+8) to the section, clears pc_begin, pc_end, fde_array and count (the FDEs are sorted on
 * first lookup) and links it at the head of the object list D_0052ADE8.
 *
 * @param begin  the first entry of the .eh_frame section
 * @param frameObject  the 0x18-byte object record to fill and link
 */
void func_004146D0(int begin, int *frameObject) {
    frameObject[2] = begin;
    frameObject[5] = (int)D_0052ADE8;
    D_0052ADE8 = frameObject;
    frameObject[0] = 0;
    frameObject[3] = 0;
    frameObject[4] = 0;
    frameObject[1] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004146F8);
#ifdef SKIP_ASM
extern void func_004146D0(int, int *);
extern int *func_004188C0(int);
/**
 * Registers an .eh_frame section for unwinding with a newly allocated object record.
 *
 * Allocates 0x18 bytes (func_004188C0) and passes them with the section to func_004146D0.
 *
 * @param begin  the first entry of the .eh_frame section
 */
void func_004146F8(int begin) {
    func_004146D0(begin, func_004188C0(0x18));
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414728);
#ifdef SKIP_ASM
extern int *D_0052ADE8;
/**
 * Registers a null-terminated table of .eh_frame sections for unwinding, using an object record the caller supplies.
 *
 * Like func_004146D0 but also stores the table in fde_array (+0xC), so func_00414008 walks every listed section. crt0
 * (func_001001D0) calls it at startup with D_0043CE00 and the object D_004A5D48.
 *
 * @param begin  the table of section pointers
 * @param frameObject  the 0x18-byte object record to fill and link
 */
void func_00414728(int begin, int *frameObject) {
    frameObject[2] = begin;
    frameObject[5] = (int)D_0052ADE8;
    D_0052ADE8 = frameObject;
    frameObject[1] = 0;
    frameObject[0] = 0;
    frameObject[4] = 0;
    frameObject[3] = begin;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414750);
#ifdef SKIP_ASM
extern void func_00414728(int, int *);
extern int *func_004188C0(int);
/**
 * Registers a table of .eh_frame sections for unwinding with a newly allocated object record.
 *
 * Allocates 0x18 bytes (func_004188C0) and passes them with the table to func_00414728.
 *
 * @param begin  the table of section pointers
 */
void func_00414750(int begin) {
    func_00414728(begin, func_004188C0(0x18));
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414780);
#ifdef SKIP_ASM
extern int *D_0052ADE8;
extern void func_00418910(int);
extern void func_00418790();
/**
 * Unregisters an .eh_frame section and returns its object record.
 *
 * Unlinks the object whose fde_begin (+8) equals key from the list D_0052ADE8 and frees its sorted FDE array
 * (func_00418910) when the object was initialised (pc_begin nonzero). Aborts (func_00418790) when no object matches.
 * The crt teardown func_00100218 calls it with D_0043CE00.
 *
 * @param key  the section (or table) the object was registered with
 * @return the unlinked object record
 */
int func_00414780(key)
    int key;
{
    int **pp = &D_0052ADE8;
    int *n;
    if (D_0052ADE8 != 0) {
        do {
            n = *pp;
            if (n[2] == key) {
                *pp = (int *)n[5];
                if (n[0] != 0)
                    func_00418910(n[3]);
                return (int)n;
            }
            pp = (int **)&n[5];
        } while (n[5] != 0);
    }
    func_00418790(key, pp);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004147F8);
#ifdef SKIP_ASM
extern int func_00414780();
extern void func_00418910(int);
/**
 * Unregisters an .eh_frame section and frees the object record func_004146F8 allocated for it.
 *
 * Declared without parameters: the caller's section pointer reaches func_00414780 in the first argument register
 * untouched; the returned record is freed with func_00418910.
 */
void func_004147F8(void) {
    func_00418910(func_00414780());
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414818);
#ifdef SKIP_ASM
/* Called from __throw to find the registers to restore for a given
   PC_TARGET.  The caller should allocate a local variable of `struct
   frame_state' (declared in frame.h) and pass its address to STATE_IN.  */

extern struct frame_state *__frame_state_for (void *, struct frame_state *) __asm__("func_00414818");

/**
 * Computes the register save state at a code address from its DWARF 2 unwind info.
 *
 * Finds the FDE (func_00414148) and its CIE (func_00414230); returns 0 when either is missing. Starts from a zeroed
 * state with the CIE's return-address column and eh_ptr, runs the CIE's instructions, skips a 'z' augmentation block,
 * then runs the FDE's instructions up to the target address (func_00414338) and copies the resulting frame_state (0x2F0
 * bytes) to state_in. func_00410F48 (the throw) and the other unwinders call it.
 *
 * @param pc_target  the code address to unwind from
 * @param state_in   receives the frame state
 * @return state_in, or 0 when the address has no unwind info
 */
struct frame_state *
__frame_state_for (void *pc_target, struct frame_state *state_in)
{
  fde *entry;
  void *insn, *end, *pc;
  struct cie_info info;
  struct frame_state_internal state;

  entry = find_fde (pc_target);
  if (entry == 0)
    return 0;

  insn = extract_cie_info (entry, &info);
  if (insn == 0)
    return 0;

  memset (&state, 0, sizeof (state));
  state.s.retaddr_column = info.ra_regno;
  state.s.eh_ptr = info.eh_ptr;

  /* First decode all the insns in the CIE.  */
  end = next_fde ((fde*) get_cie (entry));
  while (insn < end)
    insn = execute_cfa_insn (insn, &state, &info, 0);

  insn = ((fde *)entry) + 1;

  if (info.augmentation[0] == 'z')
    {
      int i;
      insn = decode_uleb128 (insn, &i);
      insn += i;
    }

  /* Then the insns in the FDE up to our target PC.  */
  end = next_fde (entry);
  pc = entry->pc_begin;
  while (insn < end && pc <= pc_target)
    insn = execute_cfa_insn (insn, &state, &info, &pc);

  memcpy (state_in, &state.s, sizeof (state.s));
  return state_in;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004149F0);
#ifdef SKIP_ASM
struct K { int f0; int f4; int key; };
struct L { struct K **items; unsigned n; };
struct M { struct K **arr; int n; };
/**
 * Splits an FDE vector into a sorted run and the out-of-order entries.
 *
 * Keeps a chain of earlier maxima in a stack array (one slot per FDE): each entry whose pc_begin (+8) is larger than
 * the current one's is moved to the erratic vector and nulled; the linear vector is then compacted and its count
 * updated. An out-of-line copy of a function the source marks inline.
 *
 * @param linear  the FDE vector to split; keeps the sorted run
 * @param out     receives the out-of-order FDEs
 */
void func_004149F0(struct L *linear, struct M *out) {
    unsigned n = linear->n;
    unsigned link[n];
    unsigned top = -1;
    unsigned i;
    unsigned pos;
    for (i = 0; i < n; i++) {
        pos = top;
        if (pos != -1 && linear->items[i]->key - linear->items[pos]->key < 0) {
            do {
                out->arr[out->n++] = linear->items[pos];
                linear->items[pos] = 0;
                pos = link[pos];
                if (pos == -1) break;
            } while (linear->items[i]->key - linear->items[pos]->key < 0);
        }
        top = i;
        link[i] = pos;
    }
    pos = 0;
    for (i = 0; i < n; i++) {
        struct K *k = linear->items[i];
        if (k) linear->items[pos++] = k;
    }
    linear->n = pos;
}
#endif

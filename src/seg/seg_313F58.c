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
void func_00413068(float *f, int *o) {
    struct FB *b = (struct FB *)f;
    unsigned m = b->m;
    int e = b->e;
    o[1] = b->s;
    if (e == 0) {
        o[0] = 2;
        return;
    }
    if (e == 0xFF) {
        if (m == 0) {
            o[0] = 4;
            return;
        }
        if (m & 0x100000)
            o[0] = 1;
        else
            o[0] = 0;
        o[3] = m;
        return;
    }
    o[2] = e - 0x7F;
    o[0] = 3;
    o[3] = (m << 7) | 0x40000000;
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
INCLUDE_ASM("seg/seg_313F58", func_00413330);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int *func_004130F8(int *, int *, int *);
extern int func_00412F58(int *);

int func_00413330(float x, float y) {
    int a[4];
    int b[4];
    int c[4];
    float fx = x;
    float fy = y;
    func_00413068(&fx, a);
    func_00413068(&fy, b);
    return func_00412F58(func_004130F8(a, b, c));
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413388);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int *func_004130F8(int *, int *, int *);
extern int func_00412F58(int *);

int func_00413388(float x, float y) {
    int a[4];
    int b[4];
    int c[4];
    float fx = x;
    float fy = y;
    func_00413068(&fx, a);
    func_00413068(&fy, b);
    b[1] ^= 1;
    return func_00412F58(func_004130F8(a, b, c));
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
INCLUDE_ASM("seg/seg_313F58", func_004135E8);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern float func_00412F58_f(int *) __asm__("func_00412F58");
extern int D_0052ADD8[];

float func_004135E8(float x, float y) {
    int a[4];
    int b[4];
    float fx = x;
    float fy = y;
    int *r;
    unsigned n, d, q, bit;
    int *p;
    func_00413068(&fx, a);
    func_00413068(&fy, b);
    p = a;
    if ((unsigned)a[0] < 2) {
        r = a;
    } else if ((unsigned)b[0] < 2) {
        r = b;
    } else {
        a[1] ^= b[1];
        if ((a[0] ^ 4) == 0 || (a[0] ^ 2) == 0) {
            r = a;
            if (a[0] == b[0])
                r = D_0052ADD8;
        } else if ((b[0] ^ 4) == 0) {
            a[3] = 0;
            a[2] = 0;
            r = a;
        } else if ((b[0] ^ 2) == 0) {
            a[0] = 4;
            r = a;
        } else {
            a[2] = a[2] - b[2];
            n = a[3];
            d = b[3];
            if (n < d) {
                a[2]--;
                n <<= 1;
            }
            bit = 0x40000000;
            q = 0;
            for (; bit != 0; bit >>= 1) {
                if (n >= d) {
                    q |= bit;
                    n -= d;
                }
                n <<= 1;
            }
            if ((q & 0x7F) == 0x40) {
                if (q & 0x80)
                    q += 0x40;
                else if (n != 0)
                    q += 0x40;
            }
            p[3] = q;
            r = p;
        }
    }
    return func_00412F58_f(r);
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

int
__fpcmp_parts (fp_number_type * a, fp_number_type * b)
{
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
INCLUDE_ASM("seg/seg_313F58", func_00413860);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int func_00413748(int *, int *);

int func_00413860(float x, float y) {
    int a[4];
    int b[4];
    float fx = x;
    float fy = y;
    func_00413068(&fx, a);
    func_00413068(&fy, b);
    return func_00413748(a, b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004138B0);
#ifdef SKIP_ASM
extern float func_00412F58_f(int *) __asm__("func_00412F58");

float func_004138B0(int x) {
    int o[4];
    unsigned m, t;
    int e;
    o[0] = 3;
    o[1] = (unsigned)x >> 31;
    if (x == 0) {
        o[0] = 2;
    } else {
        o[2] = 0x1E;
        if (o[1]) {
            if (x == 0x80000000)
                return -2147483648.0f;
            o[3] = -x;
        } else {
            o[3] = x;
        }
        m = o[3];
        if (m <= 0x3FFFFFFF) {
            e = o[2];
            do {
                t = m << 1;
                e--;
                m = t;
            } while (t <= 0x3FFFFFFF);
            o[2] = e;
            o[3] = t;
        }
    }
    return func_00412F58_f(o);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413968);
#ifdef SKIP_ASM
int func_00413968(float x) {
    int o[4];
    float f = x;
    unsigned c;
    unsigned r;
    func_00413068(&f, o);
    c = o[0];
    if ((c ^ 2) == 0)
        return 0;
    if (c < 2)
        return 0;
    if ((c ^ 4) == 0)
        goto sat;
    if (o[2] < 0)
        return 0;
    if (o[2] >= 0x1F) {
sat:
        return o[1] ? 0x80000000 : 0x7FFFFFFF;
    }
    r = (unsigned)o[3] >> (0x1E - o[2]);
    return o[1] ? -r : r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004139F8);
#ifdef SKIP_ASM
unsigned func_004139F8(float x) {
    int o[4];
    float f = x;
    unsigned c;
    func_00413068(&f, o);
    c = o[0];
    if ((c ^ 2) == 0)
        return 0;
    if (c < 2)
        return 0;
    if (o[1])
        return 0;
    if ((c ^ 4) == 0)
        goto sat;
    if (o[2] < 0)
        return 0;
    if (o[2] >= 0x20) {
sat:
        return 0xFFFFFFFF;
    }
    if (o[2] >= 0x1F)
        return (unsigned)o[3] << (o[2] - 0x1E);
    return (unsigned)o[3] >> (0x1E - o[2]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413A90);
#ifdef SKIP_ASM
extern void func_00413068(float *, int *);
extern int func_00412F58(int *);
int func_00413A90(float x) {
    int obj[2];
    float f = x;
    func_00413068(&f, obj);
    obj[1] = obj[1] == 0;
    return func_00412F58(obj);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413AC8);
#ifdef SKIP_ASM
void func_00413AC8(int a, int b, int c, int d) {
    int v[4];
    v[0] = a;
    v[1] = b;
    v[2] = c;
    v[3] = d;
    func_00412F58(v);
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
unsigned char *func_00413B38(unsigned char *p, unsigned *out) {
    unsigned b = *p++;
    unsigned r = b & 0x7F;
    int shift = 0;
    if (b & 0x80) {
        do {
            b = *p++;
            shift += 7;
            r |= (b & 0x7F) << shift;
        } while (b & 0x80);
    }
    *out = r;
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413B80);
#ifdef SKIP_ASM
unsigned char *func_00413B80(unsigned char *p, int *out) {
    unsigned shift = 0;
    int r = 0;
    unsigned b;
    do {
        b = *p++;
        r |= (b & 0x7F) << shift;
        shift += 7;
    } while (b & 0x80);
    if (shift < 32 && (b & 0x40))
        r |= -1 << shift;
    *out = r;
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00413BD0);
#ifdef SKIP_ASM
struct K13BD0 { int f0; int f4; int key; };
struct L13BD0 { struct K13BD0 **items; unsigned n; };
void func_00413BD0(struct L13BD0 *a, struct L13BD0 *b) {
    unsigned m = b->n;
    unsigned n;
    unsigned j;
    j = m;
    if (m != 0) {
        n = a->n;
        do {
            struct K13BD0 *x;
            j--;
            x = b->items[j];
            if (n != 0 && a->items[n - 1]->key - x->key > 0) {
                do {
                    a->items[n + j] = a->items[n - 1];
                    n--;
                } while (n != 0 && a->items[n - 1]->key - x->key > 0);
            }
            a->items[n + j] = x;
        } while (j != 0);
        a->n = a->n + m;
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
void func_00413F80(struct N13F80 *n, struct L13F80 *l, unsigned *lo, unsigned *hi) {
    unsigned mn = *lo;
    unsigned mx = *hi;
    if (n->off != 0) {
        do {
            if (n->a != 0 && n->addr != 0) {
                l->arr[l->cnt++] = n;
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
void func_00414008(struct N14008 *n) {
    struct N13F80 **pp;
    int cnt, bytes;
    struct LL14008 l;
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
    l.a = func_004188C0(bytes);
    l.b = func_004188C0(bytes);
    l.na = 0;
    l.nb = 0;
    lo = 0xFFFFFFFF;
    hi = 0;
    if (n->roots != 0) {
        pp = n->roots;
        while (*pp != 0) {
            func_00413F80(*pp, (struct L13F80 *)&l, &lo, &hi);
            pp++;
        }
    } else {
        func_00413F80(n->root, (struct L13F80 *)&l, &lo, &hi);
    }
    n->roots = func_00413CD0(&l, cnt);
    n->lo = lo;
    n->hi = hi;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414148);
#ifdef SKIP_ASM
/* Return a pointer to the FDE for the function containing PC.  */

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
      fde *f = ob->fde_array[i];

      if (pc < f->pc_begin)
	hi = i;
      else if (pc >= f->pc_begin + f->pc_range)
	lo = i + 1;
      else
	return f;
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
unsigned char *func_00414230(int *cie, struct C14230 *o) {
    char *s;
    unsigned char *p;
    unsigned tmp;
    o->aug = (char *)cie - cie[1] + 13;
    if (func_004165A8((int)o->aug, (int)D_004998F0) == 0 || func_004165A8((int)o->aug, (int)D_004998F8) == 0 || *o->aug == 'z') {
        s = o->aug + strlen(o->aug);
        p = (unsigned char *)s + 1;
        if (func_004165A8((int)o->aug, (int)D_004998F8) == 0) {
            o->f4 = ((struct U14230 *)p)->v;
            p += 4;
        } else {
            o->f4 = 0;
        }
        p = func_00413B38(p, &o->f8);
        p = func_00413B80(p, &o->fC);
        o->f10 = *p++;
        if (*o->aug == 'z')
            p = func_00413B38(p, &tmp) + tmp;
        return p;
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

static void *
execute_cfa_insn (void *p, struct frame_state_internal *state,
		  struct cie_info *info, void **pc)
{
  unsigned insn = *(unsigned char *)p++;
  unsigned reg;
  int offset;

  if (insn & DW_CFA_advance_loc)
    *pc += ((insn & 0x3f) * info->code_align);
  else if (insn & DW_CFA_offset)
    {
      reg = (insn & 0x3f);
      p = decode_uleb128 (p, &offset);
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
      *pc = read_pointer (p);
      p += sizeof (void *);
      break;
    case DW_CFA_advance_loc1:
      *pc += read_1byte (p);
      p += 1;
      break;
    case DW_CFA_advance_loc2:
      *pc += read_2byte (p);
      p += 2;
      break;
    case DW_CFA_advance_loc4:
      *pc += read_4byte (p);
      p += 4;
      break;

    case DW_CFA_offset_extended:
      p = decode_uleb128 (p, &reg);
      p = decode_uleb128 (p, &offset);
      offset *= info->data_align;
      state->s.saved[reg] = REG_SAVED_OFFSET;
      state->s.reg_or_offset[reg] = offset;
      break;
    case DW_CFA_restore_extended:
      p = decode_uleb128 (p, &reg);
      state->s.saved[reg] = REG_UNSAVED;
      break;

    case DW_CFA_undefined:
    case DW_CFA_same_value:
    case DW_CFA_nop:
      break;

    case DW_CFA_register:
      {
	unsigned reg2;
	p = decode_uleb128 (p, &reg);
	p = decode_uleb128 (p, &reg2);
	state->s.saved[reg] = REG_SAVED_REG;
	state->s.reg_or_offset[reg] = reg2;
      }
      break;

    case DW_CFA_def_cfa:
      p = decode_uleb128 (p, &reg);
      p = decode_uleb128 (p, &offset);
      state->s.cfa_reg = reg;
      state->s.cfa_offset = offset;
      break;
    case DW_CFA_def_cfa_register:
      p = decode_uleb128 (p, &reg);
      state->s.cfa_reg = reg;
      break;
    case DW_CFA_def_cfa_offset:
      p = decode_uleb128 (p, &offset);
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
      p = decode_uleb128 (p, &offset);
      state->s.args_size = offset;
      break;

    default:
      abort ();
    }
  return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004146D0);
#ifdef SKIP_ASM
extern int *D_0052ADE8;
void func_004146D0(int a, int *p) {
    p[2] = a;
    p[5] = (int)D_0052ADE8;
    D_0052ADE8 = p;
    p[0] = 0;
    p[3] = 0;
    p[4] = 0;
    p[1] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_004146F8);
#ifdef SKIP_ASM
extern void func_004146D0(int, int *);
extern int *func_004188C0(int);
void func_004146F8(int a) {
    func_004146D0(a, func_004188C0(0x18));
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414728);
#ifdef SKIP_ASM
extern int *D_0052ADE8;
void func_00414728(int a, int *p) {
    p[2] = a;
    p[5] = (int)D_0052ADE8;
    D_0052ADE8 = p;
    p[1] = 0;
    p[0] = 0;
    p[4] = 0;
    p[3] = a;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414750);
#ifdef SKIP_ASM
extern void func_00414728(int, int *);
extern int *func_004188C0(int);
void func_00414750(int a) {
    func_00414728(a, func_004188C0(0x18));
}
#endif

//100%
INCLUDE_ASM("seg/seg_313F58", func_00414780);
#ifdef SKIP_ASM
extern int *D_0052ADE8;
extern void func_00418910(int);
extern void func_00418790();
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

struct frame_state *
__frame_state_for (void *pc_target, struct frame_state *state_in)
{
  fde *f;
  void *insn, *end, *pc;
  struct cie_info info;
  struct frame_state_internal state;

  f = find_fde (pc_target);
  if (f == 0)
    return 0;

  insn = extract_cie_info (f, &info);
  if (insn == 0)
    return 0;

  memset (&state, 0, sizeof (state));
  state.s.retaddr_column = info.ra_regno;
  state.s.eh_ptr = info.eh_ptr;

  /* First decode all the insns in the CIE.  */
  end = next_fde ((fde*) get_cie (f));
  while (insn < end)
    insn = execute_cfa_insn (insn, &state, &info, 0);

  insn = ((fde *)f) + 1;

  if (info.augmentation[0] == 'z')
    {
      int i;
      insn = decode_uleb128 (insn, &i);
      insn += i;
    }

  /* Then the insns in the FDE up to our target PC.  */
  end = next_fde (f);
  pc = f->pc_begin;
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
void func_004149F0(struct L *a, struct M *out) {
    unsigned n = a->n;
    unsigned link[n];
    unsigned top = -1;
    unsigned i;
    unsigned p;
    for (i = 0; i < n; i++) {
        p = top;
        if (p != -1 && a->items[i]->key - a->items[p]->key < 0) {
            do {
                out->arr[out->n++] = a->items[p];
                a->items[p] = 0;
                p = link[p];
                if (p == -1) break;
            } while (a->items[i]->key - a->items[p]->key < 0);
        }
        top = i;
        link[i] = p;
    }
    p = 0;
    for (i = 0; i < n; i++) {
        struct K *k = a->items[i];
        if (k) a->items[p++] = k;
    }
    a->n = p;
}
#endif

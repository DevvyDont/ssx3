/* GNU CC C++ runtime support, reconstructed: cp/tinfo.cc and cp/tinfo2.cc (type_info, dynamic_cast).
   Original: Copyright (C) 1994, 1996, 1998 Free Software Foundation.
   It is free software under the GNU General Public License, version 2 or (at your option) any later version, with
   this special exception: "if you link this library with other files, some of which are compiled with GCC, to
   produce an executable, this library does not by itself cause the resulting executable to be covered by the
   GNU General Public License. This exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License."
   This file stays under those terms; the repository's CC0 dedication does not apply to it (README, License).
   Bodies credited to peak3project come from their fork (github.com/peak3project/ssx3). */

#include "common.h"
#include "seg/seg_315B68_tinfo.h"

extern "C" {

int func_00414B98(int *a, int *b);

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414B68);
#ifdef SKIP_ASM
extern int D_00499A48[];
extern int operator_delete__FPi(int *);
int func_00414B68(int *p, int flag) {
    p[1] = (int)D_00499A48;
    if (flag & 1) return operator_delete__FPi(p);
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414B98);
#ifdef SKIP_ASM
extern int func_004165A8(int, int);
int func_00414B98(int *a, int *b) {
    int r = 0;
    if (b == a || func_004165A8(*a, *b) == 0)
        r = 1;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414BD8);
#ifdef SKIP_ASM
extern int D_004999D0[];
void func_00414BD8(int *p, int a, int b, int c) {
    if (p != 0) {
        p[3] = c;
        p[0] = a;
        p[2] = b;
        p[1] = (int)D_004999D0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414C00);
#ifdef SKIP_ASM
extern int D_004999E8[];
void func_00414C00(int *p, int a, int b) {
    if (p != 0) {
        p[2] = b;
        p[0] = a;
        p[1] = (int)D_004999E8;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414C20);
#ifdef SKIP_ASM
extern int D_00499A00[];
void func_00414C20(int *p, int a) {
    if (p != 0) {
        p[0] = a;
        p[1] = (int)D_00499A00;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414C40);
#ifdef SKIP_ASM
int func_00414C40(int *a, int *b, int c, int d) {
    if (func_00414B98(a, b) == 0) d = 0;
    return d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414C70);
#ifdef SKIP_ASM
int func_00414C70(int *p, int a, int b, int c, int d, int e) {
    int *o;
    short *vt;
    if (func_00414B98(p, (int *)a) != 0)
        return c;
    o = (int *)p[2];
    vt = (short *)o[1];
    return ((int (*)(int *, int, int, int, int, int))((int *)vt)[5])((int *)((char *)o + vt[8]), a, b, c, d, e);
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414D08);
#ifdef SKIP_ASM
/* cp/tinfo.cc / cp/tinfo2.cc (GCC runtime, GPL v2+ with the runtime linking exception): needs the class
   declarations of include/seg/seg_315B68_tinfo.h (included at the top of the unit).
   g++ calls these under fixed names (type_info::operator==, the typeinfo functions typeid and dynamic_cast use,
   __dynamic_cast); bind each to the retail symbol (assembler assignments: the relocations then name the retail
   symbols). All of them are defined in this unit. */
__asm__("__eq__C9type_infoRC9type_info = func_00414B98\n"
        "__tf16__user_type_info = func_00415018\n"
        "__tf9type_info = func_004151F8\n"
        "__dynamic_cast = func_00415840\n"
        "__tfv = func_004158F0\n"
        "__tf16__func_type_info = func_00415DF8\n"
        "__tf16__attr_type_info = func_00415ED8\n"
        "__tf19__pointer_type_info = func_00415F48");

/* tinfo.cc __class_type_info::dcast (GCC runtime, GPL v2+ with the runtime linking exception) */
}  // extern "C"  (a member function definition cannot have C linkage)

void* __class_type_info::
dcast (const type_info& desired, int is_public, void *objptr,
       const type_info *sub, void *subptr) const
{
  if (*this == desired)
    return objptr;

  void *match_found = 0;
  for (size_t i = 0; i < n_bases; i++)
    {
      if (is_public && base_list[i].access != PUBLIC)
	continue;

      void *p = (char *)objptr + base_list[i].offset;
      if (base_list[i].is_virtual)
	p = *(void **)p;
      p = base_list[i].base->dcast (desired, is_public, p, sub, subptr);
      if (p)
	{
	  if (match_found == 0)
	    match_found = p;
	  else if (match_found != p)
	    {
	      if (sub)
		{
		  // Perhaps we're downcasting from *sub to desired; see if
		  // subptr is a subobject of exactly one of {match_found,p}.

		  const __user_type_info &d =
		    static_cast <const __user_type_info &> (desired);

		  void *os = d.dcast (*sub, 1, match_found);
		  void *ns = d.dcast (*sub, 1, p);

		  if (os == ns)
		    /* ambiguous -- subptr is a virtual base */;
		  else if (os == subptr)
		    continue;
		  else if (ns == subptr)
		    {
		      match_found = p;
		      continue;
		    }
		}

	      // base found at two different pointers,
	      // conversion is not unique
	      return 0;
	    }
	}
    }

  return match_found;
}

extern "C" {
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414EE0);
#ifdef SKIP_ASM
extern int D_004999D0[];
int *func_00414EE0(int *p, int a, int b, int c) {
    p[0] = a;
    p[1] = (int)D_004999D0;
    p[2] = b;
    p[3] = c;
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414F00);
#ifdef SKIP_ASM
extern int D_0053EA60[];
extern int D_00499A58[];
extern int D_0053EA40[];
extern int *func_00415018(void);
extern void func_00414C00(int *, int, int);
int *func_00414F00(void) {
    if (D_0053EA60[0] == 0) {
        func_00415018();
        func_00414C00(D_0053EA60, (int)D_00499A58, (int)D_0053EA40);
    }
    return D_0053EA60;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414F50);
#ifdef SKIP_ASM
void func_00414F50(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414F70);
#ifdef SKIP_ASM
extern int D_004999E8[];
int *func_00414F70(int *p, int a, int b) {
    p[0] = a;
    p[1] = (int)D_004999E8;
    p[2] = b;
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414F90);
#ifdef SKIP_ASM
extern int D_0053EA50[];
extern int D_00499A70[];
extern int D_0053EA40[];
extern int *func_00415018(void);
extern void func_00414C00(int *, int, int);
int *func_00414F90(void) {
    if (D_0053EA50[0] == 0) {
        func_00415018();
        func_00414C00(D_0053EA50, (int)D_00499A70, (int)D_0053EA40);
    }
    return D_0053EA50;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414FE0);
#ifdef SKIP_ASM
void func_00414FE0(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415000);
#ifdef SKIP_ASM
extern int D_00499A00[];
int *func_00415000(int *p, int a) {
    p[0] = a;
    p[1] = (int)D_00499A00;
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415018);
#ifdef SKIP_ASM
extern int D_0053EA40[];
extern int D_00499A88[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void func_00414C00(int *, int, int);
int *func_00415018(void) {
    if (D_0053EA40[0] == 0) {
        func_004151F8();
        func_00414C00(D_0053EA40, (int)D_00499A88, (int)D_004A5BB0);
    }
    return D_0053EA40;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415068);
#ifdef SKIP_ASM
void func_00415068(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415088);
#ifdef SKIP_ASM
extern int D_004998B8[];
extern int operator_delete__FPi(int *);
int func_00415088(int **p, int flag) {
    *p = D_004998B8;
    if (flag & 1) return operator_delete__FPi((int *)p);
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004150B8);
#ifdef SKIP_ASM
extern int D_00499A18[];
int *func_004150B8(int **p) {
    *p = D_00499A18;
    return (int*)p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004150D0);
#ifdef SKIP_ASM
extern int D_0053EA20[];
extern int D_00499AA0[];
extern int D_004A5BA8[];
extern int *func_00412F18(void);
extern void func_00414C00(int *, int, int);
int *func_004150D0(void) {
    if (D_0053EA20[0] == 0) {
        func_00412F18();
        func_00414C00(D_0053EA20, (int)D_00499AA0, (int)D_004A5BA8);
    }
    return D_0053EA20;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415120);
#ifdef SKIP_ASM
extern int D_004998B8[];
extern int operator_delete__FPi(int *);
int func_00415120(int **p, int flag) {
    *p = D_004998B8;
    if (flag & 1) return operator_delete__FPi((int *)p);
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415150);
#ifdef SKIP_ASM
extern int D_00499A30[];
int *func_00415150(int **p) {
    *p = D_00499A30;
    return (int*)p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415168);
#ifdef SKIP_ASM
extern int D_0053EA30[];
extern int D_00499AB0[];
extern int D_004A5BA8[];
extern int *func_00412F18(void);
extern void func_00414C00(int *, int, int);
int *func_00415168(void) {
    if (D_0053EA30[0] == 0) {
        func_00412F18();
        func_00414C00(D_0053EA30, (int)D_00499AB0, (int)D_004A5BA8);
    }
    return D_0053EA30;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004151B8);
#ifdef SKIP_ASM
int func_004151B8(int *a, int *b) {
    return func_00414B98(a, b) ^ 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004151D8);
#ifdef SKIP_ASM
int func_004151D8(int *p) {
    return *p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004151E0);
#ifdef SKIP_ASM
extern int D_00499A48[];
int *func_004151E0(int *p, int a) {
    p[0] = a;
    p[1] = (int)D_00499A48;
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004151F8);
#ifdef SKIP_ASM
extern int D_00499AC0[];
extern int D_004A5BB0[];
extern void func_00414C20(int *, int);
int *func_004151F8(void) {
    if (D_004A5BB0[0] == 0)
        func_00414C20(D_004A5BB0, (int)D_00499AC0);
    return D_004A5BB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415238);
#ifdef SKIP_ASM
extern int func_004165A8(int, int);
int func_00415238(int *a, int *b) {
    return (unsigned)func_004165A8(*a, *b) >> 31;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415260);
#ifdef SKIP_ASM
/* tinfo2.cc __throw_type_match_rtti (GCC runtime, GPL v2+ with the runtime linking exception) */
/* Low level match routine used by compiler to match types of catch
   variables and thrown objects.  */

extern "C" void*
__throw_type_match_rtti (const void *catch_type_r, const void *throw_type_r,
			 void *objptr) __asm__ ("func_00415260");
extern "C" void*
__throw_type_match_rtti (const void *catch_type_r, const void *throw_type_r,
			 void *objptr)
{
  const type_info &catch_type = *(const type_info *)catch_type_r;
  const type_info &throw_type = *(const type_info *)throw_type_r;
  
  if (catch_type == throw_type)
    return objptr;
  
  /* (a disabled `#if 0` printf of the two type names was here) */

  void *new_objptr = 0;

  if (const __user_type_info *p
      = dynamic_cast <const __user_type_info *> (&throw_type))
    {
      /* The 1 skips conversions to private bases. */
      new_objptr = p->dcast (catch_type, 1, objptr);
    }
  else if (const __pointer_type_info *fr =
	   dynamic_cast <const __pointer_type_info *> (&throw_type))
    {
      const __pointer_type_info *to =
	   dynamic_cast <const __pointer_type_info *> (&catch_type);

      if (! to)
	goto fail;

      const type_info *subfr = &fr->type, *subto = &to->type;
      __attr_type_info::cv cvfrom, cvto;

      if (const __attr_type_info *at
	  = dynamic_cast <const __attr_type_info *> (subfr))
	{
	  cvfrom = at->attr;
	  subfr = &at->type;
	}
      else
	cvfrom = __attr_type_info::NONE;
      
      if (const __attr_type_info *at
	  = dynamic_cast <const __attr_type_info *> (subto))
	{
	  cvto = at->attr;
	  subto = &at->type;
	}
      else
	cvto = __attr_type_info::NONE;

      if (((cvfrom & __attr_type_info::CONST)
	   > (cvto & __attr_type_info::CONST))
	  || ((cvfrom & __attr_type_info::VOLATILE)
	      > (cvto & __attr_type_info::VOLATILE)))
	goto fail;

      if (*subto == *subfr)
	new_objptr = objptr;
      else if (*subto == typeid (void)
	       && dynamic_cast <const __func_type_info *> (subfr) == 0)
	new_objptr = objptr;
      else if (const __user_type_info *p
	       = dynamic_cast <const __user_type_info *> (subfr))
	{
	  /* The 1 skips conversions to private bases. */
	  new_objptr = p->dcast (*subto, 1, objptr);
	}
      else if (const __pointer_type_info *pfr
	       = dynamic_cast <const __pointer_type_info *> (subfr))
	{
	  // Multi-level pointer conversion.

	  const __pointer_type_info *pto
	    = dynamic_cast <const __pointer_type_info *> (subto);

	  if (! pto)
	    goto fail;
	    
	  bool constp = (cvto & __attr_type_info::CONST);
	  for (subto = &pto->type, subfr = &pfr->type; ;
	       subto = &pto->type, subfr = &pfr->type)
	    {
	      if (const __attr_type_info *at
		  = dynamic_cast <const __attr_type_info *> (subfr))
		{
		  cvfrom = at->attr;
		  subfr = &at->type;
		}
	      else
		cvfrom = __attr_type_info::NONE;
      
	      if (const __attr_type_info *at
		  = dynamic_cast <const __attr_type_info *> (subto))
		{
		  cvto = at->attr;
		  subto = &at->type;
		}
	      else
		cvto = __attr_type_info::NONE;

	      if (((cvfrom & __attr_type_info::CONST)
		   > (cvto & __attr_type_info::CONST))
		  || ((cvfrom & __attr_type_info::VOLATILE)
		      > (cvto & __attr_type_info::VOLATILE)))
		goto fail;

	      if (! constp
		  && (((cvfrom & __attr_type_info::CONST)
		       < (cvto & __attr_type_info::CONST))
		      || ((cvfrom & __attr_type_info::VOLATILE)
			  < (cvto & __attr_type_info::VOLATILE))))
		goto fail;

	      if (*subto == *subfr)
		{
		  new_objptr = objptr;
		  break;
		}

	      pto = dynamic_cast <const __pointer_type_info *> (subto);
	      pfr = dynamic_cast <const __pointer_type_info *> (subfr);
	      if (! pto || ! pfr)
		goto fail;		

	      if (! (cvto & __attr_type_info::CONST))
		constp = false;
	    }
	}
    }
 fail:

  /* (a disabled `#if 0` printf of the pointer delta was here) */
  return new_objptr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415728);
#ifdef SKIP_ASM
extern int *func_004151F8(void);
extern int *func_00415F48(void);
extern int func_00415840(int, char *, int, char *, char *, void *);
int func_00415728(int *p) {
    int r = 0;
    if (p) {
        short *vt = (short *)p[1];
        r = func_00415840(((int *)vt)[1], (char *)func_00415F48, 1, (char *)p + vt[0], (char *)func_004151F8, p);
    }
    return r != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415778);
#ifdef SKIP_ASM
extern int D_00499BC8[];
void func_00415778(int *p, int a, int b) {
    if (p != 0) {
        p[2] = b;
        p[0] = a;
        p[1] = (int)D_00499BC8;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415798);
#ifdef SKIP_ASM
extern int D_00499BB8[];
void func_00415798(int *p, int a, int b, int c) {
    if (p != 0) {
        p[3] = b;
        p[0] = a;
        p[2] = c;
        p[1] = (int)D_00499BB8;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004157C0);
#ifdef SKIP_ASM
extern int D_00499B98[];
void func_004157C0(int *p, int a) {
    if (p != 0) {
        p[0] = a;
        p[1] = (int)D_00499B98;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004157E0);
#ifdef SKIP_ASM
extern int D_00499B88[];
void func_004157E0(int *p, int a) {
    if (p != 0) {
        p[0] = a;
        p[1] = (int)D_00499B88;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415800);
#ifdef SKIP_ASM
extern int D_00499B78[];
void func_00415800(int *p, int a) {
    if (p != 0) {
        p[0] = a;
        p[1] = (int)D_00499B78;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415820);
#ifdef SKIP_ASM
extern int D_00499B68[];
void func_00415820(int *p, int a) {
    if (p != 0) {
        p[0] = a;
        p[1] = (int)D_00499B68;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415840);
#ifdef SKIP_ASM
int func_00415840(int f0, char *f1, int a, char *b, char *f4, void *c) {
    char *o = ((char *(*)(void))f0)();
    int *vt = *(int **)(o + 4);
    char *th = o + *(short *)((char *)vt + 0x10);
    int r1;
    int r4;
    vt = (int *)((char *)vt + 0x10);
    r1 = ((int (*)(void))f1)();
    r4 = ((int (*)(void))f4)();
    return ((int (*)(char *, int, int, char *, int, void *))vt[1])(th, r1, a, b, r4, c);
}
#endif

}  // extern "C"

// libstdc++-v2 tinfo2.cc, BUILTIN(m): __tf<m>() lazily placement-constructs the builtin type_info __ti<m> (its name
// is the mangled type letter, its vtable is __builtin_type_info's) and returns it. The objects, name strings and
// the vtable are retail data (D_*): only their addresses are referenced here.
typedef unsigned int size_t;
inline void* operator new(size_t, void* p) throw() { return p; }

extern "C" {
extern const int D_00499BA8[];
extern unsigned char D_00454158[8];
extern unsigned char D_00454160[8];
extern unsigned char D_00454168[8];
extern unsigned char D_00454170[8];
extern unsigned char D_00454178[8];
extern unsigned char D_00454180[8];
extern unsigned char D_00454188[8];
extern unsigned char D_00454190[8];
extern unsigned char D_00454198[8];
extern unsigned char D_004541A0[8];
extern unsigned char D_004541A8[8];
extern unsigned char D_004541B0[8];
extern unsigned char D_004541B8[8];
extern unsigned char D_004541C0[8];
extern unsigned char D_004541C8[8];
extern unsigned char D_004541D0[8];
extern unsigned char D_004541D8[8];
extern const char D_00499AE0[];
extern const char D_00499AE8[];
extern const char D_00499AF0[];
extern const char D_00499AF8[];
extern const char D_00499B00[];
extern const char D_00499B08[];
extern const char D_00499B10[];
extern const char D_00499B18[];
extern const char D_00499B20[];
extern const char D_00499B28[];
extern const char D_00499B30[];
extern const char D_00499B38[];
extern const char D_00499B40[];
extern const char D_00499B48[];
extern const char D_00499B50[];
extern const char D_00499B58[];
extern const char D_00499B60[];
}

class __builtin_ti {
    const char* _name;
    const void* vtbl_;
public:
    __builtin_ti(const char* n) : _name(n), vtbl_(D_00499BA8) {}
};

// (a non-polymorphic stand-in class that stores the retail vtable's address: a real polymorphic class would emit its
// own vtable, and the retail one is data we reference rather than regenerate)
#define BUILTIN(fn, ti, name)                  \
    extern "C" const __builtin_ti& fn(void) {  \
        if (*(void**)ti == 0)                  \
            new (ti) __builtin_ti(name);       \
        return *(const __builtin_ti*)ti;       \
    }

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004158F0);
#ifdef SKIP_ASM
BUILTIN(func_004158F0, D_00454158, D_00499AE0)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415928);
#ifdef SKIP_ASM
BUILTIN(func_00415928, D_00454160, D_00499AE8)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415960);
#ifdef SKIP_ASM
BUILTIN(func_00415960, D_00454168, D_00499AF0)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415998);
#ifdef SKIP_ASM
BUILTIN(func_00415998, D_00454170, D_00499AF8)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004159D0);
#ifdef SKIP_ASM
BUILTIN(func_004159D0, D_00454178, D_00499B00)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415A08);
#ifdef SKIP_ASM
BUILTIN(func_00415A08, D_00454180, D_00499B08)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415A40);
#ifdef SKIP_ASM
BUILTIN(func_00415A40, D_00454188, D_00499B10)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415A78);
#ifdef SKIP_ASM
BUILTIN(func_00415A78, D_00454190, D_00499B18)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415AB0);
#ifdef SKIP_ASM
BUILTIN(func_00415AB0, D_00454198, D_00499B20)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415AE8);
#ifdef SKIP_ASM
BUILTIN(func_00415AE8, D_004541A0, D_00499B28)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415B20);
#ifdef SKIP_ASM
BUILTIN(func_00415B20, D_004541A8, D_00499B30)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415B58);
#ifdef SKIP_ASM
BUILTIN(func_00415B58, D_004541B0, D_00499B38)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415B90);
#ifdef SKIP_ASM
BUILTIN(func_00415B90, D_004541B8, D_00499B40)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415BC8);
#ifdef SKIP_ASM
BUILTIN(func_00415BC8, D_004541C0, D_00499B48)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415C00);
#ifdef SKIP_ASM
BUILTIN(func_00415C00, D_004541C8, D_00499B50)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415C38);
#ifdef SKIP_ASM
BUILTIN(func_00415C38, D_004541D0, D_00499B58)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415C70);
#ifdef SKIP_ASM
BUILTIN(func_00415C70, D_004541D8, D_00499B60)
#endif

extern "C" {

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415CA8);
#ifdef SKIP_ASM
extern int D_0053EAA0[];
extern int D_00499BD8[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void func_00414C00(int *, int, int);
int *func_00415CA8(void) {
    if (D_0053EAA0[0] == 0) {
        func_004151F8();
        func_00414C00(D_0053EAA0, (int)D_00499BD8, (int)D_004A5BB0);
    }
    return D_0053EAA0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415CF8);
#ifdef SKIP_ASM
void func_00415CF8(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415D18);
#ifdef SKIP_ASM
extern int D_0053EAC0[];
extern int D_00499BF0[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void func_00414C00(int *, int, int);
int *func_00415D18(void) {
    if (D_0053EAC0[0] == 0) {
        func_004151F8();
        func_00414C00(D_0053EAC0, (int)D_00499BF0, (int)D_004A5BB0);
    }
    return D_0053EAC0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415D68);
#ifdef SKIP_ASM
void func_00415D68(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415D88);
#ifdef SKIP_ASM
extern int D_0053EA90[];
extern int D_00499C08[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void func_00414C00(int *, int, int);
int *func_00415D88(void) {
    if (D_0053EA90[0] == 0) {
        func_004151F8();
        func_00414C00(D_0053EA90, (int)D_00499C08, (int)D_004A5BB0);
    }
    return D_0053EA90;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415DD8);
#ifdef SKIP_ASM
void func_00415DD8(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415DF8);
#ifdef SKIP_ASM
extern int D_0053EA70[];
extern int D_00499C20[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void func_00414C00(int *, int, int);
int *func_00415DF8(void) {
    if (D_0053EA70[0] == 0) {
        func_004151F8();
        func_00414C00(D_0053EA70, (int)D_00499C20, (int)D_004A5BB0);
    }
    return D_0053EA70;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415E48);
#ifdef SKIP_ASM
void func_00415E48(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415E68);
#ifdef SKIP_ASM
extern int D_0053EAD0[];
extern int D_00499C38[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void func_00414C00(int *, int, int);
int *func_00415E68(void) {
    if (D_0053EAD0[0] == 0) {
        func_004151F8();
        func_00414C00(D_0053EAD0, (int)D_00499C38, (int)D_004A5BB0);
    }
    return D_0053EAD0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415EB8);
#ifdef SKIP_ASM
void func_00415EB8(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415ED8);
#ifdef SKIP_ASM
extern int D_0053EA80[];
extern int D_00499C50[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void func_00414C00(int *, int, int);
int *func_00415ED8(void) {
    if (D_0053EA80[0] == 0) {
        func_004151F8();
        func_00414C00(D_0053EA80, (int)D_00499C50, (int)D_004A5BB0);
    }
    return D_0053EA80;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415F28);
#ifdef SKIP_ASM
void func_00415F28(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415F48);
#ifdef SKIP_ASM
extern int D_0053EAB0[];
extern int D_00499C68[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void func_00414C00(int *, int, int);
int *func_00415F48(void) {
    if (D_0053EAB0[0] == 0) {
        func_004151F8();
        func_00414C00(D_0053EAB0, (int)D_00499C68, (int)D_004A5BB0);
    }
    return D_0053EAB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415F98);
#ifdef SKIP_ASM
void func_00415F98(void) {
    ((void (*)(void))func_00414B68)();
}
#endif

}  // extern "C"

#include "common.h"

// PORT: PS2 kernel syscall stub: the syscall number goes in $3. Defined under a second C name with an
// asm label because this file declares these symbols with their argument lists elsewhere.
#define SYSCALL_STUB(name, n) \
    void name##_syscall(void) __asm__(#name); \
    void name##_syscall(void) { register int v __asm__("$3") = (n); __asm__ volatile("syscall" : : "r"(v)); }

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00415FB8);
#ifdef SKIP_ASM
extern int D_00454944;
int func_00415FB8(void) {
    return D_00454944;
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_00415FC8);

INCLUDE_ASM("seg/seg_316FB8", func_0041605C);

INCLUDE_ASM("seg/seg_316FB8", func_0041610C);

INCLUDE_ASM("seg/seg_316FB8", func_00416210);

INCLUDE_ASM("seg/seg_316FB8", func_004162D0);

INCLUDE_ASM("seg/seg_316FB8", strchr);

INCLUDE_ASM("seg/seg_316FB8", func_004165A8);

INCLUDE_ASM("seg/seg_316FB8", strcpy);

INCLUDE_ASM("seg/seg_316FB8", strlen);

INCLUDE_ASM("seg/seg_316FB8", func_00416958);

INCLUDE_ASM("seg/seg_316FB8", func_00416B18);

INCLUDE_ASM("seg/seg_316FB8", strncpy);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00416EB0);
#ifdef SKIP_ASM
extern int func_00420180(int, char *, int *);
extern char *func_00420010(int);
extern int func_0041FCB0(int *, int, int, int);
extern long func_00417948(), func_004179B0(), func_00417A30();
extern void func_00417A98();
extern long func_00417008(char *, int, int);
char *func_00416EB0_(int ptr, char *file, char *mode) __asm__("func_00416EB0");
char *func_00416EB0_(int ptr, char *file, char *mode) {
    char *fp;
    int f, flags, oflags;
    if ((flags = func_00420180(ptr, mode, &oflags)) == 0)
        return 0;
    if ((fp = func_00420010(ptr)) == 0)
        return 0;
    if ((f = func_0041FCB0(*(int **)(fp + 0x54), (int)file, oflags, 0x1B6)) < 0) {
        *(short *)(fp + 0xC) = 0;
        return 0;
    }
    *(short *)(fp + 0xE) = f;
    *(short *)(fp + 0xC) = flags;
    *(char **)(fp + 0x1C) = fp;
    *(void **)(fp + 0x20) = func_00417948;
    *(void **)(fp + 0x24) = func_004179B0;
    *(void **)(fp + 0x28) = func_00417A30;
    *(void **)(fp + 0x2C) = func_00417A98;
    if (flags & 0x100)
        func_00417008(fp, 0, 2);
    return fp;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00416F90);
#ifdef SKIP_ASM
extern int D_00454944;
extern void func_00416EB0(int, int, int);
void func_00416F90(int a, int b) {
    func_00416EB0(D_00454944, a, b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00416FC0);
#ifdef SKIP_ASM
extern void func_0041FE30(void);
extern int func_00417418(int, void (*)(void));
void func_00416FC0(int a) {
    func_00417418(a, func_0041FE30);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00416FE0);
#ifdef SKIP_ASM
extern int D_00454944;
extern void func_00416FC0(int);
void func_00416FE0(void) {
    func_00416FC0(D_00454944);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417008);
#ifdef SKIP_ASM
struct R17008 {
    int err;
    char pad[0x38 - 4];
    int didinit;
};
struct F17008 {
    unsigned char *p;
    int r;
    int w;
    unsigned short flags;
    short file;
    unsigned char *base;
    int size;
    int lbfsize;
    int cookie;
    int pad20[2];
    long (*seek)(int, long, int);
    int pad2C;
    unsigned char *ubbase;
    int ubsize;
    unsigned char *up;
    int ur;
    char pad40[0x4C - 0x40];
    int blksize;
    int offset;
    struct R17008 *data;
};
extern struct R17008 *g17008_impure __asm__("D_00454944");
extern void g17008_init(struct R17008 *) __asm__("func_004200F0");
extern int g17008_fflush(struct F17008 *) __asm__("func_0041FE30");
extern int g17008_fstat(struct R17008 *, int, int *) __asm__("func_0041FBF0");
extern long g17008_sseek(int, long, int) __asm__("func_00417A30");
extern void g17008_smakebuf(struct F17008 *) __asm__("func_004174B0");
extern int g17008_srefill(struct F17008 *) __asm__("func_00417648");
int func_00417008_(struct F17008 *fp, long offset, int whence) __asm__("func_00417008");
int func_00417008_(struct F17008 *fp, long offset, int whence) {
    long (*seekfn)(int, long, int);
    long target, curoff;
    unsigned n;
    int st[0x68 / 4];
    int havepos;
    struct R17008 *ptr;
    if (fp->data == 0)
        fp->data = g17008_impure;
    if (fp->data->didinit == 0)
        g17008_init(fp->data);
    ptr = fp->data;
    if ((*(int *)&fp->flags & 0x108) == 0x108)
        g17008_fflush(fp);
    if ((seekfn = fp->seek) == 0) {
        ptr->err = 0x1D;
        return -1;
    }
    switch (whence) {
    case 1:
        g17008_fflush(fp);
        if (fp->flags & 0x1000)
            curoff = fp->offset;
        else {
            curoff = (*seekfn)(fp->cookie, 0, 1);
            if (curoff == -1)
                return -1;
        }
        if (fp->flags & 4) {
            curoff -= fp->r;
            if (fp->ubbase)
                curoff -= fp->ur;
        } else if (fp->flags & 8 && fp->p != 0)
            curoff += fp->p - fp->base;
        offset += curoff;
        whence = 0;
        havepos = 1;
        break;
    case 0:
    case 2:
        havepos = 0;
        break;
    default:
        ptr->err = 0x16;
        return -1;
    }
    if (fp->base == 0)
        g17008_smakebuf(fp);
    if (fp->flags & 0x81A)
        goto dumb;
    if ((fp->flags & 0x400) == 0) {
        if (seekfn != g17008_sseek || fp->file < 0 || g17008_fstat(ptr, fp->file, st) ||
            (st[1] & 0xF000) != 0x8000) {
            fp->flags |= 0x800;
            goto dumb;
        }
        fp->blksize = 0x400;
        fp->flags |= 0x400;
    }
    if (whence == 0)
        target = offset;
    else {
        if (g17008_fstat(ptr, fp->file, st))
            goto dumb;
        target = *(long *)&st[4] + offset;
    }
    if (!havepos) {
        if (fp->flags & 0x1000)
            curoff = fp->offset;
        else {
            curoff = (*seekfn)(fp->cookie, 0, 1);
            if (curoff == -1)
                goto dumb;
        }
        curoff -= fp->r;
        if (fp->ubbase)
            curoff -= fp->ur;
    }
    if (fp->ubbase) {
        curoff += fp->r;
        n = fp->up - fp->base;
        curoff -= n;
        n += fp->ur;
    } else {
        n = fp->p - fp->base;
        curoff -= n;
        n += fp->r;
    }
    if ((fp->flags & 0x2000) == 0 && target >= curoff && target < curoff + n) {
        int o = target - curoff;
        fp->p = fp->base + o;
        fp->r = n - o;
        fp->flags &= ~0x20;
        return 0;
    }
    curoff = target & -fp->blksize;
    if ((*seekfn)(fp->cookie, curoff, 0) == -1)
        goto dumb;
    fp->r = 0;
    fp->flags &= ~0x20;
    n = target - curoff;
    if (n) {
        if (g17008_srefill(fp) || fp->r < n)
            goto dumb;
        fp->p += n;
        fp->r -= n;
    }
    return 0;
dumb:
    if (g17008_fflush(fp) || (*seekfn)(fp->cookie, offset, whence) == -1)
        return -1;
    fp->p = fp->base;
    fp->r = 0;
    fp->flags &= ~0x20;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417418);
#ifdef SKIP_ASM
struct G17418 {
    struct G17418 *next;
    int n;
    char *files;
};
int func_00417418(int r_, void (*fn_)(void)) {
    char *r = (char *)r_;
    int (*fn)(char *) = (int (*)(char *))fn_;
    struct G17418 *g;
    char *fp;
    int n;
    int ret = 0;
    for (g = (struct G17418 *)(r + 0x1D8); g; g = g->next) {
        fp = g->files;
        for (n = g->n; --n >= 0; fp += 0x58)
            if (*(short *)(fp + 0xC))
                ret |= fn(fp);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004174B0);
#ifdef SKIP_ASM
struct F174B0 {
    char *p;
    int r;
    int w;
    unsigned short flags;
    short file;
    char *base;
    int size;
    char pad1[0x28 - 0x18];
    void *seek;
    char pad2[0x43 - 0x2C];
    char nbuf[9];
    int blksize;
    int offset;
    int data;
};
extern long func_00417A30();
extern int func_0041FBF0(int *, int, int);
void func_004174B0(struct F174B0 *f) {
    int st[0x68 / 4];
    if (f->flags & 2) {
        f->p = f->nbuf;
        f->base = f->nbuf;
        f->size = 1;
        return;
    }
    if (f->file < 0 || func_0041FBF0((int *)f->data, f->file, (int)st) < 0) {
        f->flags |= 0x800;
    } else if ((st[1] & 0xF000) == 0x8000 && f->seek == (void *)func_00417A30) {
        f->blksize = 0x400;
        f->flags |= 0x400;
    } else {
        f->flags |= 0x800;
    }
    f->flags |= 2;
    f->size = 1;
    f->p = f->nbuf;
    f->base = f->nbuf;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417578);
#ifdef SKIP_ASM
#define func_00417578_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern void func_0041B728(int *, int, int, char *);
void func_00417578(int *p, int fmt, ...) {
    char *ap;
    func_00417578_va_start(ap);
    func_0041B728(p, p[2], fmt, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004175C8);
#ifdef SKIP_ASM
#define func_004175C8_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern int D_00454944;
extern int func_0041B658(int, int, char *);
void func_004175C8(int fmt, ...) {
    char *ap;
    int *g;
    func_004175C8_va_start(ap);
    g = (int *)D_00454944;
    ((int *)g[2])[0x54 / 4] = (int)g;
    func_0041B658(g[2], fmt, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417628);
#ifdef SKIP_ASM
extern void func_0041FE30(void);
void func_00417628(void) {
    func_0041FE30();
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417648);
#ifdef SKIP_ASM
struct R7648 {
    int err;
    char pad[0x38 - 4];
    int didinit;
};
struct F7648 {
    unsigned char *p;
    int r;
    int w;
    unsigned short flags;
    short file;
    unsigned char *base;
    int size;
    int lbfsize;
    int cookie;
    int (*rd)(int, unsigned char *, int);
    int pad24;
    int pad28;
    int pad2C;
    unsigned char *ubbase;
    int ubsize;
    unsigned char *up;
    int ur;
    char pad40[0x54 - 0x40];
    struct R7648 *data;
};
extern struct R7648 *g7648_impure __asm__("D_00454944");
extern void g7648_init(struct R7648 *) __asm__("func_004200F0");
extern int g7648_fflush(struct F7648 *) __asm__("func_0041FE30");
extern void g7648_smakebuf(struct F7648 *) __asm__("func_004174B0");
extern int g7648_fwalk(struct R7648 *, void *) __asm__("func_00417418");
extern void g7648_lflush(void) __asm__("func_00417628");
int func_00417648_impl(struct F7648 *fp) __asm__("func_00417648");
/* newlib's CHECK_INIT (stdio/local.h), a do { } while (0) macro: make sure stdio is set up (98.89 without the macro's
   loop) */
#define CHECK_INIT_7648(fp)                     \
  do {                                          \
      if ((fp)->data == 0)                      \
          (fp)->data = g7648_impure;            \
      if ((fp)->data->didinit == 0)             \
          g7648_init((fp)->data);               \
  } while (0)
/* __srefill */
int func_00417648_impl(struct F7648 *fp) {
    int n;
    CHECK_INIT_7648(fp);
    fp->r = 0;
    if (fp->flags & 0x20)
        return -1;
    if ((fp->flags & 4) == 0) {
        if ((fp->flags & 0x10) == 0)
            return -1;
        if (fp->flags & 8) {
            if (g7648_fflush(fp))
                return -1;
            fp->flags &= ~8;
            fp->w = 0;
            fp->lbfsize = 0;
        }
        fp->flags |= 4;
    } else if (fp->ubbase != 0) {
        if ((fp->r = fp->ur) != 0) {
            fp->p = fp->up;
            return 0;
        }
    }
    if (fp->base == 0)
        g7648_smakebuf(fp);
    if (fp->flags & 3)
        g7648_fwalk(fp->data, g7648_lflush);
    fp->p = fp->base;
    n = fp->rd(fp->cookie, fp->base, fp->size);
    fp->r = n;
    fp->flags &= ~0x2000;
    if (n <= 0) {
        if (n == 0) {
            fp->flags |= 0x20;
        } else {
            fp->r = 0;
            fp->flags |= 0x40;
        }
        return -1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004177B0);
#ifdef SKIP_ASM
#define func_004177B0_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
struct F177B0 {
    char *p;
    int r;
    int w;
    short flags, file;
    char *base;
    int size;
    int pad[0x54 / 4 - 6];
    int ent;
};
void func_004177B0(int reent, char *buf, int fmt, ...) {
    char *ap;
    struct F177B0 f;
    func_004177B0_va_start(ap);
    f.p = buf;
    f.w = 0x7FFFFFFF;
    f.flags = 0x208;
    f.base = buf;
    f.size = 0x7FFFFFFF;
    f.ent = reent;
    func_0041B658((int)&f, fmt, ap);
    *f.p = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", sprintf);
#ifdef SKIP_ASM
#define sprintf_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
struct Fsprintf {
    char *p;
    int r;
    int w;
    short flags, file;
    char *base;
    int size;
    int pad[0x54 / 4 - 6];
    int ent;
};
extern int D_00454944;
void sprintf(char *buf, int fmt, ...) {
    char *ap;
    struct Fsprintf f;
    sprintf_va_start(ap);
    f.p = buf;
    f.w = 0x7FFFFFFF;
    f.flags = 0x208;
    f.base = buf;
    f.size = 0x7FFFFFFF;
    f.ent = D_00454944;
    func_0041B658((int)&f, fmt, ap);
    *f.p = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004178A8);
#ifdef SKIP_ASM
int func_004178A8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004178B0);
#ifdef SKIP_ASM
#define func_004178B0_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
struct F178B0 {
    char *p;
    int r;
    int w;
    short flags, file;
    char *base;
    int size;
    int lbfsize;
    int cookie;
    int (*read)(void);
    int pad1[3];
    int ubbase;
    int pad2[0x44 / 4 - 0x34 / 4];
    int lbbase;
    int pad3[0x54 / 4 - 0x48 / 4];
    int ent;
};
extern int strlen(char *);
extern int D_00454944;
extern int func_00417AB8(struct F178B0 *, int, char *);
int func_004178B0(char *str, int fmt, ...) {
    char *ap;
    struct F178B0 f;
    f.flags = 4;
    f.p = str;
    f.base = str;
    f.size = f.r = strlen(str);
    f.read = func_004178A8;
    f.ubbase = 0;
    f.lbbase = 0;
    f.ent = D_00454944;
    func_004178B0_va_start(ap);
    return func_00417AB8(&f, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417948);
#ifdef SKIP_ASM
extern long func_0041FD10(int *, int, int, int);
long func_00417948(int *f, int a, int b) {
    int r = func_0041FD10((int *)f[0x54 / 4], ((short *)f)[7], a, b);
    if (r >= 0) {
        f[0x50 / 4] += r;
    } else {
        ((unsigned short *)f)[6] &= 0xEFFF;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004179B0);
#ifdef SKIP_ASM
extern long func_0041FDD0(int *, int, int, int);
int func_004179B0_(int *f, int a, int b) __asm__("func_004179B0");
int func_004179B0_(int *f, int a, int b) {
    if (((unsigned short *)f)[6] & 0x100)
        func_0041FC50((int *)f[0x54 / 4], ((short *)f)[7], 0, 2);
    ((unsigned short *)f)[6] &= 0xEFFF;
    return func_0041FDD0((int *)f[0x54 / 4], ((short *)f)[7], a, b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417A30);
#ifdef SKIP_ASM
extern long func_0041FC50(int *, int, int, int);
long func_00417A30(int *f, int a, int b) {
    long r = func_0041FC50((int *)f[0x54 / 4], ((short *)f)[7], a, b);
    unsigned short *p = (unsigned short *)f + 6;
    if (r == -1) {
        *p &= 0xEFFF;
    } else {
        f[0x50 / 4] = r;
        *p |= 0x1000;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417A98);
#ifdef SKIP_ASM
extern int func_0041FB98(int, int);
void func_00417A98(int *p) {
    func_0041FB98(p[0x54 / 4], ((short *)p)[7]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00417AB8);
#ifdef SKIP_ASM
/* newlib libc/stdio/vfscanf.c (__svfscanf). The body follows newlib 1.9.0 (MB_CAPABLE format scan) with the
 * CT_FLOAT conversion of the earlier newlib releases the retail code shows (atof, no leading-zero exponent fixup):
 *
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* EE EABI va_arg (gcc 2.95 va-mips.h, __mips64 soft/single-float branch), for types of at most 8 bytes */
#define va_arg_00417AB8(__AP, __type) (*(__type *) (void *) ((__AP += 8) - 8))
/* newlib ctype.h; D_00499C81 is _ctype_ + 1 */
#define isspace_00417AB8(c)	(D_00499C81[(unsigned)(c)]&010)
#define isupper_00417AB8(c)	(D_00499C81[(unsigned)(c)]&01)
#define MB_CUR_MAX_00417AB8 (*(int *)D_00455140)
extern char D_00499C81[];
extern char D_00455140[];
extern const short D_00499D90[17]; /* static const short basefix[17] */
extern int mbtowc_r_7AB8(int, unsigned short *, char *, int, int *) __asm__("func_00418958");
extern char *func_004185D0(char *, char *);
extern unsigned func_00420260(char *buf, unsigned size, unsigned count, int *fp);
extern int ungetc_7AB8(int, struct F7648 *) __asm__("ungetc");
extern unsigned long g7AB8_strtol() __asm__("func_0041E9E0");
extern unsigned long g7AB8_strtoul() __asm__("func_0041AA50");
extern double g7AB8_atof(char *) __asm__("func_004187A0");

#define	BUF_00417AB8	(308+39+3)	/* MAXEXP+MAXFRACT+3: 3 = sign + decimal point + NUL */

#define	LONG		0x01	/* l: long or double */
#define	LONGDBL		0x02	/* L: long double */
#define	SHORT		0x04	/* h: short */
#define	SUPPRESS	0x08	/* suppress assignment */
#define	POINTER		0x10	/* weird %p pointer (`fake hex') */
#define	NOSKIP		0x20	/* do not skip blanks */
#define	SIGNOK		0x40	/* +/- is (still) legal */
#define	NDIGITS		0x80	/* no digits detected */
#define	DPTOK		0x100	/* (float) decimal point is still legal */
#define	EXPOK		0x200	/* (float) exponent (e+3, etc) still legal */
#define	PFXOK		0x100	/* 0x prefix is (still) legal */
#define	NZDIGITS	0x200	/* no zero digits detected */
#define	CT_CHAR		0	/* %c conversion */
#define	CT_CCL		1	/* %[...] conversion */
#define	CT_STRING	2	/* %s conversion */
#define	CT_INT		3	/* integer, i.e., strtol or strtoul */
#define	CT_FLOAT	4	/* floating, i.e., strtod */
#define u_char_00417AB8 char
#define u_long_00417AB8 unsigned long

#define BufferEmpty_00417AB8 (fp->r <= 0 && func_00417648_impl(fp))

int func_00417AB8_(struct F7648 *fp, char *fmt0, char *ap) __asm__("func_00417AB8");
int
func_00417AB8_(struct F7648 *fp, char *fmt0, char *ap)
{
  register u_char_00417AB8 *fmt = (u_char_00417AB8 *) fmt0;
  register int c;		/* character from format, or conversion */
  register unsigned int width;	/* field width, or 0 */
  register char *p;		/* points into all kinds of strings */
  register int n;		/* handy integer */
  register int flags;		/* flags as defined above */
  register char *p0;		/* saves original value of p when necessary */
  int nassigned;		/* number of fields assigned */
  int nread;			/* number of characters consumed from fp */
  int base = 0;			/* base argument to strtol/strtoul */
  int nbytes = 1;               /* number of bytes read from fmt string */
  unsigned short wc;            /* wchar to use to read format string */

  u_long_00417AB8 (*ccfn) () = 0;	/* conversion function (strtol/strtoul) */
  char ccltab[256];		/* character class table for %[...] */
  char buf[BUF_00417AB8];	/* buffer for numeric conversions */
  char *lptr;                   /* literal pointer */
  int state = 0;                /* value to keep track of multibyte state */

  short *sp;
  int *ip;
  float *flp;
  double *ldp;
  double *dp;
  long *lp;

  /* `basefix' is used to avoid `if' tests in the integer scanner: the function's static table
     {10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} is retail's D_00499D90 */

  nassigned = 0;
  nread = 0;
  for (;;)
    {
      nbytes = mbtowc_r_7AB8 (D_00454944, &wc, fmt, MB_CUR_MAX_00417AB8, &state);
      fmt += nbytes;
      if (wc == 0)
	return nassigned;
      if (nbytes == 1 && isspace_00417AB8 (wc))
	{
	  for (;;)
	    {
	      if (BufferEmpty_00417AB8)
		return nassigned;
	      if (!isspace_00417AB8 (*fp->p))
		break;
	      nread++, fp->r--, fp->p++;
	    }
	  continue;
	}
      if (wc != '%')
	goto literal;
      width = 0;
      flags = 0;

      /*
       * switch on the format.  continue if done; break once format
       * type is derived.
       */

    again:
      c = *fmt++;

      switch (c)
	{
	case '%':
	literal:
          lptr = fmt - nbytes;
          for (n = 0; n < nbytes; ++n)
            {
	      if (BufferEmpty_00417AB8)
	        goto input_failure;
	      if (*fp->p != *lptr)
	        goto match_failure;
	      fp->r--, fp->p++;
	      nread++;
              ++lptr;
            }
	  continue;

	case '*':
	  flags |= SUPPRESS;
	  goto again;
	case 'l':
	  flags |= LONG;
	  goto again;
	case 'L':
	  flags |= LONGDBL;
	  goto again;
	case 'h':
	  flags |= SHORT;
	  goto again;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  width = width * 10 + c - '0';
	  goto again;

	case 'D':		/* compat */
	  flags |= LONG;
	  /* FALLTHROUGH */
	case 'd':
	  c = CT_INT;
	  ccfn = (u_long_00417AB8 (*)())g7AB8_strtol;
	  base = 10;
	  break;

	case 'i':
	  c = CT_INT;
	  ccfn = (u_long_00417AB8 (*)())g7AB8_strtol;
	  base = 0;
	  break;

	case 'O':		/* compat */
	  flags |= LONG;
	  /* FALLTHROUGH */
	case 'o':
	  c = CT_INT;
	  ccfn = g7AB8_strtoul;
	  base = 8;
	  break;

	case 'u':
	  c = CT_INT;
	  ccfn = g7AB8_strtoul;
	  base = 10;
	  break;

	case 'X':		/* compat   XXX */
	case 'x':
	  flags |= PFXOK;	/* enable 0x prefixing */
	  c = CT_INT;
	  ccfn = g7AB8_strtoul;
	  base = 16;
	  break;

	case 'E':		/* compat   XXX */
	case 'G':		/* compat   XXX */
/* ANSI says that E,G and X behave the same way as e,g,x */
	  /* FALLTHROUGH */
	case 'e':
	case 'f':
	case 'g':
	  c = CT_FLOAT;
	  break;

	case 's':
	  c = CT_STRING;
	  break;

	case '[':
	  fmt = func_004185D0 (ccltab, fmt);
	  flags |= NOSKIP;
	  c = CT_CCL;
	  break;

	case 'c':
	  flags |= NOSKIP;
	  c = CT_CHAR;
	  break;

	case 'p':		/* pointer format is like hex */
	  flags |= POINTER | PFXOK;
	  c = CT_INT;
	  ccfn = g7AB8_strtoul;
	  base = 16;
	  break;

	case 'n':
	  if (flags & SUPPRESS)	/* ??? */
	    continue;
	  if (flags & SHORT)
	    {
	      sp = va_arg_00417AB8 (ap, short *);
	      *sp = nread;
	    }
	  else if (flags & LONG)
	    {
	      lp = va_arg_00417AB8 (ap, long *);
	      *lp = nread;
	    }
	  else
	    {
	      ip = va_arg_00417AB8 (ap, int *);
	      *ip = nread;
	    }
	  continue;

	  /*
	   * Disgusting backwards compatibility hacks.	XXX
	   */
	case '\0':		/* compat */
	  return -1;

	default:		/* compat */
	  if (isupper_00417AB8 (c))
	    flags |= LONG;
	  c = CT_INT;
	  ccfn = (u_long_00417AB8 (*)())g7AB8_strtol;
	  base = 10;
	  break;
	}

      /*
       * We have a conversion that requires input.
       */
      if (BufferEmpty_00417AB8)
	goto input_failure;

      /*
       * Consume leading white space, except for formats that
       * suppress this.
       */
      if ((flags & NOSKIP) == 0)
	{
	  while (isspace_00417AB8 (*fp->p))
	    {
	      nread++;
	      if (--fp->r > 0)
		fp->p++;
	      else
	      if (func_00417648_impl (fp))
		goto input_failure;
	    }
	  /*
	   * Note that there is at least one character in the
	   * buffer, so conversions that do not set NOSKIP ca
	   * no longer result in an input failure.
	   */
	}

      /*
       * Do the conversion.
       */
      switch (c)
	{

	case CT_CHAR:
	  /* scan arbitrary characters (sets NOSKIP) */
	  if (width == 0)
	    width = 1;
	  if (flags & SUPPRESS)
	    {
	      unsigned int sum = 0;

	      for (;;)
		{
		  if ((n = fp->r) < width)
		    {
		      sum += n;
		      width -= n;
		      fp->p += n;
		      if (func_00417648_impl (fp))
			{
			  if (sum == 0)
			    goto input_failure;
			  break;
			}
		    }
		  else
		    {
		      sum += width;
		      fp->r -= width;
		      fp->p += width;
		      break;
		    }
		}
	      nread += sum;
	    }
	  else
	    {
	      unsigned int r = func_00420260 ((void *) va_arg_00417AB8 (ap, char *), 1, width, (int *) fp);

	      if (r == 0)
		goto input_failure;
	      nread += r;
	      nassigned++;
	    }
	  break;

	case CT_CCL:
	  /* scan a (nonempty) character class (sets NOSKIP) */
	  if (width == 0)
	    width = ~0;		/* `infinity' */
	  /* take only those things in the class */
	  if (flags & SUPPRESS)
	    {
	      n = 0;
	      while (ccltab[*fp->p])
		{
		  n++, fp->r--, fp->p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty_00417AB8)
		    {
		      if (n == 0)
			goto input_failure;
		      break;
		    }
		}
	      if (n == 0)
		goto match_failure;
	    }
	  else
	    {
	      p0 = p = va_arg_00417AB8 (ap, char *);
	      while (ccltab[*fp->p])
		{
		  fp->r--;
		  *p++ = *fp->p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty_00417AB8)
		    {
		      if (p == p0)
			goto input_failure;
		      break;
		    }
		}
	      n = p - p0;
	      if (n == 0)
		goto match_failure;
	      *p = 0;
	      nassigned++;
	    }
	  nread += n;
	  break;

	case CT_STRING:
	  /* like CCL, but zero-length string OK, & no NOSKIP */
	  if (width == 0)
	    width = ~0;
	  if (flags & SUPPRESS)
	    {
	      n = 0;
	      while (!isspace_00417AB8 (*fp->p))
		{
		  n++, fp->r--, fp->p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty_00417AB8)
		    break;
		}
	      nread += n;
	    }
	  else
	    {
	      p0 = p = va_arg_00417AB8 (ap, char *);
	      while (!isspace_00417AB8 (*fp->p))
		{
		  fp->r--;
		  *p++ = *fp->p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty_00417AB8)
		    break;
		}
	      *p = 0;
	      nread += p - p0;
	      nassigned++;
	    }
	  continue;

	case CT_INT:
	  /* scan an integer as if by strtol/strtoul */
	  /* size_t is unsigned, hence this optimisation */
	  if (--width > sizeof (buf) - 2)
	    width = sizeof (buf) - 2;
	  width++;
	  flags |= SIGNOK | NDIGITS | NZDIGITS;
	  for (p = buf; width; width--)
	    {
	      c = *fp->p;
	      /*
	       * Switch on the character; `goto ok' if we
	       * accept it as a part of number.
	       */
	      switch (c)
		{
		case '0':
		  if (base == 0)
		    {
		      base = 8;
		      flags |= PFXOK;
		    }
		  if (flags & NZDIGITS)
		    flags &= ~(SIGNOK | NZDIGITS | NDIGITS);
		  else
		    flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* 1 through 7 always legal */
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		  base = D_00499D90[base];
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* digits 8 and 9 ok iff decimal or hex */
		case '8':
		case '9':
		  base = D_00499D90[base];
		  if (base <= 8)
		    break;	/* not legal here */
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* letters ok iff hex */
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		  /* no need to fix base here */
		  if (base <= 10)
		    break;	/* not legal here */
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* sign ok only as first character */
		case '+':
		case '-':
		  if (flags & SIGNOK)
		    {
		      flags &= ~SIGNOK;
		      goto ok;
		    }
		  break;

		  /* x ok iff flag still set & 2nd char */
		case 'x':
		case 'X':
		  if (flags & PFXOK && p == buf + 1)
		    {
		      base = 16;/* if %i */
		      flags &= ~PFXOK;
		      goto ok;
		    }
		  break;
		}

	      /*
	       * If we got here, c is not a legal character
	       * for a number.  Stop accumulating digits.
	       */
	      break;
	    ok:
	      /*
	       * c is legal: store it and look at the next.
	       */
	      *p++ = c;
	      if (--fp->r > 0)
		fp->p++;
	      else
	      if (func_00417648_impl (fp))
		break;		/* EOF */
	    }
	  /*
	   * If we had only a sign, it is no good; push back the sign.
	   * If the number ends in `x', it was [sign] '0' 'x', so push back
	   * the x and treat it as [sign] '0'.
	   */
	  if (flags & NDIGITS)
	    {
	      if (p > buf)
		(void) ungetc_7AB8 (*(u_char_00417AB8 *)-- p, fp);
	      goto match_failure;
	    }
	  c = ((u_char_00417AB8 *) p)[-1];
	  if (c == 'x' || c == 'X')
	    {
	      --p;
	      /*(void)*/ ungetc_7AB8 (c, fp);
	    }
	  if ((flags & SUPPRESS) == 0)
	    {
	      u_long_00417AB8 res;

	      *p = 0;
	      res = (*ccfn) (buf, (char **) 0, base);
	      if (flags & POINTER)
		*(va_arg_00417AB8 (ap, void **)) = (void *) (unsigned int) res;
	      else if (flags & SHORT)
		{
		  sp = va_arg_00417AB8 (ap, short *);
		  *sp = res;
		}
	      else if (flags & LONG)
		{
		  lp = va_arg_00417AB8 (ap, long *);
		  *lp = res;
		}
	      else
		{
		  ip = va_arg_00417AB8 (ap, int *);
		  *ip = res;
		}
	      nassigned++;
	    }
	  nread += p - buf;
	  break;

	case CT_FLOAT:
	  /* scan a floating point number as if by strtod */
	  /* size_t is unsigned, hence this optimisation */
	  if (--width > sizeof (buf) - 2)
	    width = sizeof (buf) - 2;
	  width++;
	  flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
	  for (p = buf; width; width--)
	    {
	      c = *fp->p;
	      /*
	       * This code mimicks the integer conversion
	       * code, but is much simpler.
	       */
	      switch (c)
		{

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		  flags &= ~(SIGNOK | NDIGITS);
		  goto fok;

		case '+':
		case '-':
		  if (flags & SIGNOK)
		    {
		      flags &= ~SIGNOK;
		      goto fok;
		    }
		  break;
		case '.':
		  if (flags & DPTOK)
		    {
		      flags &= ~(SIGNOK | DPTOK);
		      goto fok;
		    }
		  break;
		case 'e':
		case 'E':
		  /* no exponent without some digits */
		  if ((flags & (NDIGITS | EXPOK)) == EXPOK)
		    {
		      flags =
			(flags & ~(EXPOK | DPTOK)) |
			SIGNOK | NDIGITS;
		      goto fok;
		    }
		  break;
		}
	      break;
	    fok:
	      *p++ = c;
	      if (--fp->r > 0)
		fp->p++;
	      else
	      if (func_00417648_impl (fp))
		break;		/* EOF */
	    }
	  /*
	   * If no digits, might be missing exponent digits
	   * (just give back the exponent) or might be missing
	   * regular digits, but had sign and/or decimal point.
	   */
	  if (flags & NDIGITS)
	    {
	      if (flags & EXPOK)
		{
		  /* no digits at all */
		  while (p > buf)
		    ungetc_7AB8 (*(u_char_00417AB8 *)-- p, fp);
		  goto match_failure;
		}
	      /* just a bad exponent (e and maybe sign) */
	      c = *(u_char_00417AB8 *)-- p;
	      if (c != 'e' && c != 'E')
		{
		  (void) ungetc_7AB8 (c, fp);	/* sign */
		  c = *(u_char_00417AB8 *)-- p;
		}
	      (void) ungetc_7AB8 (c, fp);
	    }
	  if ((flags & SUPPRESS) == 0)
	    {
	      double res;

	      *p = 0;
	      res = g7AB8_atof (buf);
	      if (flags & LONG)
		{
		  dp = va_arg_00417AB8 (ap, double *);
		  *dp = res;
		}
	      else if (flags & LONGDBL)
		{
		  ldp = va_arg_00417AB8 (ap, double *);
		  *ldp = res;
		}
	      else
		{
		  flp = va_arg_00417AB8 (ap, float *);
		  *flp = res;
		}
	      nassigned++;
	    }
	  nread += p - buf;
	  break;
	}
    }
input_failure:
  return nassigned ? nassigned : -1;
match_failure:
  return nassigned;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004185D0);
#ifdef SKIP_ASM
char *func_004185D0(char *tab, char *fmt) {
    int c, n, v;
    c = *fmt++;
    if (c == '^') {
        v = 1;
        c = *fmt++;
    } else
        v = 0;
    for (n = 0; n < 256; n++)
        tab[n] = v;
    if (c == 0)
        return fmt - 1;
    v = 1 - v;
    for (;;) {
        tab[c] = v;
    doswitch:
        n = *fmt++;
        switch (n) {
        case 0:
            return fmt - 1;
        case '-':
            n = *fmt;
            if (n == ']' || n < c) {
                c = '-';
                break;
            }
            fmt++;
            do {
                tab[++c] = v;
            } while (c < n);
            goto doswitch;
            break;
        case ']':
            return fmt;
        default:
            c = n;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004186C8);
#ifdef SKIP_ASM
struct Fvsprintf {
    char *p;
    int r;
    int w;
    short flags, file;
    char *base;
    int size;
    int pad[0x54 / 4 - 6];
    int ent;
};
extern int D_00454944;
extern int func_0041B658(int, int, char *);
void func_004186C8(char *buf, int fmt, char *ap) {
    struct Fvsprintf f;
    f.p = buf;
    f.w = 0x7FFFFFFF;
    f.flags = 0x208;
    f.base = buf;
    f.size = 0x7FFFFFFF;
    f.ent = D_00454944;
    func_0041B658((int)&f, fmt, ap);
    *f.p = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418720);
#ifdef SKIP_ASM
struct F18720 {
    char *p;
    int r;
    unsigned w;
    short flags, file;
    char *base;
    unsigned size;
    int pad[0x54 / 4 - 6];
    int ent;
};
extern int D_00454944;
int func_00418720(char *buf, unsigned n, int fmt, char *ap) {
    struct F18720 f;
    int r;
    f.p = buf;
    f.w = n;
    f.flags = 0x208;
    f.base = buf;
    f.size = n;
    f.ent = D_00454944;
    r = func_0041B658((int)&f, fmt, ap);
    if (n < (unsigned)r) r = -1;
    *f.p = 0;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418790);
#ifdef SKIP_ASM
void func_001001C8(int) __attribute__((noreturn));
void func_00418790(void) {
    func_001001C8(1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004187A0);
#ifdef SKIP_ASM
extern void func_0041A7E8(int, int);
void func_004187A0(int a) {
    func_0041A7E8(a, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004187C0);
#ifdef SKIP_ASM
extern long func_0041E9E0(int, int, int);
int func_004187C0(int a) {
    return func_0041E9E0(a, 0, 10);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004187E8);
#ifdef SKIP_ASM
void *func_004187E8(void *key, char *base, unsigned nmemb, unsigned size, int (*compar)(void *, void *)) {
    unsigned lo = 0, hi = nmemb, mid;
    char *p;
    int c;
    if (nmemb == 0 || size == 0)
        return 0;
    while (lo < hi) {
        mid = (lo + hi) >> 1;
        p = base + mid * size;
        c = compar(key, p);
        if (c < 0)
            hi = mid;
        else if (c > 0)
            lo = mid + 1;
        else
            return p;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004188C0);
#ifdef SKIP_ASM
extern int D_00454944;
extern void func_00418CA8(int);
extern void func_00418D08(int);
extern int func_0041DBF8(int, int);
int func_004188C0(int a) {
    int r;
    func_00418CA8(D_00454944);
    r = func_0041DBF8(D_00454944, a);
    func_00418D08(D_00454944);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418910);
#ifdef SKIP_ASM
extern int D_00454944;
extern void func_00418CA8(int);
extern void func_00418D08(int);
extern void func_0041E340(int, int);
void func_00418910(int a) {
    func_00418CA8(D_00454944);
    func_0041E340(D_00454944, a);
    func_00418D08(D_00454944);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418958);
#ifdef SKIP_ASM
/* newlib stdlib/mbtowc_r.c (_mbtowc_r) with stdlib/mbctype.h; neither file carries its own notice (newlib's
 * default: Copyright (c) 1994-2000 Red Hat, Inc. Permission to use, copy, modify and distribute this software is
 * freely granted, provided that this notice is preserved.) */
typedef unsigned short wchar_t_8958;
struct _reent_8958 {
  int _errno;
  void *_stdin, *_stdout, *_stderr;
  int  _inc;
  char _emergency[25];
  int _current_category;
  const char *_current_locale;
};

#define ESC_CHAR_8958 0x1b
#define _issjis1_8958(c)    (((c) >= 0x81 && (c) <= 0x9f) || ((c) >= 0xe0 && (c) <= 0xef))
#define _issjis2_8958(c)    (((c) >= 0x40 && (c) <= 0x7e) || ((c) >= 0x80 && (c) <= 0xfc))
#define _iseucjp_8958(c)    ((c) >= 0xa1 && (c) <= 0xfe)
#define _isjis_8958(c)      ((c) >= 0x21 && (c) <= 0x7e)

typedef enum { ESCAPE, DOLLAR, BRACKET, AT, B, J,
               NUL, JIS_CHAR, OTHER, JIS_C_NUM } JIS_CHAR_TYPE_8958;
typedef enum { ASCII, A_ESC, A_ESC_DL, JIS, JIS_1, JIS_2, J_ESC, J_ESC_BR,
               J2_ESC, J2_ESC_BR, DONE, INV, JIS_S_NUM } JIS_STATE_8958;
typedef enum { COPY_A, COPY_J, COPY_J2, MAKE_A, MAKE_J, NOOP, EMPTY, ERROR } JIS_ACTION_8958;

/* the file's static state/action tables, in .data */
extern JIS_STATE_8958 JIS_state_table_8958[JIS_S_NUM][JIS_C_NUM] __asm__("D_00454948");
extern JIS_ACTION_8958 JIS_action_table_8958[JIS_S_NUM][JIS_C_NUM] __asm__("D_00454AF8");
extern unsigned int strlen_8958(const char *) __asm__("strlen");
extern int strcmp_8958(const char *, const char *) __asm__("func_004165A8");

int
func_00418958 (struct _reent_8958 *r, wchar_t_8958 *pwc, const char *s, unsigned int n, int *state)
{
  wchar_t_8958 dummy;
  unsigned char *t = (unsigned char *)s;

  if (pwc == 0)
    pwc = &dummy;

  if (s != 0 && n == 0)
    return -1;

  if (r->_current_locale == 0 ||
      (strlen_8958 (r->_current_locale) <= 1))
    { /* fall-through */ }
  else if (!strcmp_8958 (r->_current_locale, "C-SJIS"))
    {
      int char1 = *t;
      if (s == 0)
        return 0;  /* not state-dependent */
      if (_issjis1_8958 (char1))
        {
          int char2 = t[1];
          if (n <= 1)
            return -1;
          if (_issjis2_8958 (char2))
            {
              *pwc = (((wchar_t_8958)*t) << 8) + (wchar_t_8958)(*(t+1));
              return 2;
            }
          else
            return -1;
        }
    }
  else if (!strcmp_8958 (r->_current_locale, "C-EUCJP"))
    {
      int char1 = *t;
      if (s == 0)
        return 0;  /* not state-dependent */
      if (_iseucjp_8958 (char1))
        {
          int char2 = t[1];
          if (n <= 1)
            return -1;
          if (_iseucjp_8958 (char2))
            {
              *pwc = (((wchar_t_8958)*t) << 8) + (wchar_t_8958)(*(t+1));
              return 2;
            }
          else
            return -1;
        }
    }
  else if (!strcmp_8958 (r->_current_locale, "C-JIS"))
    {
      JIS_STATE_8958 curr_state;
      JIS_ACTION_8958 action;
      JIS_CHAR_TYPE_8958 ch;
      unsigned char *ptr;
      int i, curr_ch;

      if (s == 0)
        {
          *state = 0;
          return 1;  /* state-dependent */
        }

      curr_state = (*state == 0 ? ASCII : JIS);
      ptr = t;

      for (i = 0; i < n; ++i)
        {
          curr_ch = t[i];
          switch (curr_ch)
            {
	    case ESC_CHAR_8958:
              ch = ESCAPE;
              break;
	    case '$':
              ch = DOLLAR;
              break;
            case '@':
              ch = AT;
              break;
            case '(':
	      ch = BRACKET;
              break;
            case 'B':
              ch = B;
              break;
            case 'J':
              ch = J;
              break;
            case '\0':
              ch = NUL;
              break;
            default:
              if (_isjis_8958 (curr_ch))
                ch = JIS_CHAR;
              else
                ch = OTHER;
	    }

          action = JIS_action_table_8958[curr_state][ch];
          curr_state = JIS_state_table_8958[curr_state][ch];

          switch (action)
            {
            case NOOP:
              break;
            case EMPTY:
              *state = 0;
              *pwc = (wchar_t_8958)0;
              return i;
            case COPY_A:
	      *state = 0;
              *pwc = (wchar_t_8958)*ptr;
              return (i + 1);
            case COPY_J:
              *state = 0;
              *pwc = (((wchar_t_8958)*ptr) << 8) + (wchar_t_8958)(*(ptr+1));
              return (i + 1);
            case COPY_J2:
              *state = 1;
              *pwc = (((wchar_t_8958)*ptr) << 8) + (wchar_t_8958)(*(ptr+1));
              return (ptr - t) + 2;
            case MAKE_A:
            case MAKE_J:
              ptr = (char *)(t + i + 1);
              break;
            case ERROR:
            default:
              return -1;
            }

        }

      return -1;  /* n < bytes needed */
    }

  /* otherwise this must be the "C" locale or unknown locale */
  if (s == 0)
    return 0;  /* not state-dependent */

  *pwc = (wchar_t_8958)*t;

  if (*t == '\0')
    return 0;

  return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418CA8);
#ifdef SKIP_ASM
extern int func_00423C90(void);
extern void func_00423DE0(int);
extern int D_00454CA8;
extern int D_00454CAC;
extern int D_00455248;
void func_00418CA8(int unused) {
    int id = func_00423C90();
    if (D_00454CA8 == id) {
        D_00454CAC++;
    } else {
        func_00423DE0(D_00455248);
        D_00454CA8 = id;
        D_00454CAC++;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418D08);
#ifdef SKIP_ASM
extern void func_00423DC0(int);
extern int D_00454CA8;
extern int D_00454CAC;
extern int D_00455248;
void func_00418D08(int unused) {
    if (--D_00454CAC == 0) {
        D_00454CA8 = -1;
        func_00423DC0(D_00455248);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418D48);
#ifdef SKIP_ASM
extern void func_004218D8(int, int, int, int, int);
void func_00418D48(int a, int b, int c, int d) {
    func_004218D8(a, b, c, d, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418D68);
#ifdef SKIP_ASM
extern int fptodp(void);
extern void func_00418D48(int, int, int, int);
void func_00418D68(int a, int b, int c) {
    func_00418D48(fptodp(), a, b, c);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418DB8);
#ifdef SKIP_ASM
extern int fptodp(void);
extern int func_00418E68(int, int, int);
void func_00418DB8(int a, int b) {
    func_00418E68(fptodp(), a, b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418DF8);
#ifdef SKIP_ASM
extern void func_00421A50(int, int, int, int, int);
void func_00418DF8(int a, int b, int c, int d) {
    func_00421A50(a, b, c, d, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418E18);
#ifdef SKIP_ASM
extern int fptodp(void);
extern void func_00418DF8(int, int, int, int);
void func_00418E18(int a, int b, int c) {
    func_00418DF8(fptodp(), a, b, c);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418E68);
#ifdef SKIP_ASM
extern int D_00454944;
extern int dpcmp(int, int);
extern int func_00421B98(int, int, int, char *, int, int);
int func_00418E68(int a, int prec, int buf_) {
    char *buf = (char *)buf_;
    char *p = buf;
    if (dpcmp(a, 0) < 0) {
        *p++ = '-';
        prec--;
    }
    return func_00421B98(D_00454944, a, prec, p, 'g', 0) ? (int)buf : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418EF8);
#ifdef SKIP_ASM
/* newlib 1.9 libc/stdlib/qsort.c (qsort), verbatim apart from the K&R wrappers:
 *
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
static inline char *med3_00418EF8(char *, char *, char *, int (*)());
static inline void swapfunc_00418EF8(char *, char *, int, int);

#define min_00418EF8(a, b)	(a) < (b) ? a : b

#define swapcode_00418EF8(TYPE, parmi, parmj, n) { 		\
	long i = (n) / sizeof (TYPE); 			\
	register TYPE *pi = (TYPE *) (parmi); 		\
	register TYPE *pj = (TYPE *) (parmj); 		\
	do { 						\
		register TYPE	t = *pi;		\
		*pi++ = *pj;				\
		*pj++ = t;				\
        } while (--i > 0);				\
}

#define SWAPINIT_00418EF8(a, es) swaptype = ((char *)a - (char *)0) % sizeof(long) || \
	es % sizeof(long) ? 2 : es == sizeof(long)? 0 : 1;

static inline void
swapfunc_00418EF8(char *a, char *b, int n, int swaptype)
{
	if(swaptype <= 1) 
		swapcode_00418EF8(long, a, b, n)
	else
		swapcode_00418EF8(char, a, b, n)
}

#define swap_00418EF8(a, b)					\
	if (swaptype == 0) {				\
		long t = *(long *)(a);			\
		*(long *)(a) = *(long *)(b);		\
		*(long *)(b) = t;			\
	} else						\
		swapfunc_00418EF8(a, b, es, swaptype)

#define vecswap_00418EF8(a, b, n) 	if ((n) > 0) swapfunc_00418EF8(a, b, n, swaptype)

static inline char *
med3_00418EF8(char *a, char *b, char *c, int (*cmp)())
{
	return cmp(a, b) < 0 ?
	       (cmp(b, c) < 0 ? b : (cmp(a, c) < 0 ? c : a ))
              :(cmp(b, c) > 0 ? b : (cmp(a, c) < 0 ? a : c ));
}

void
func_00418EF8(void *a, unsigned int n, unsigned int es, int (*cmp)())
{
	char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
	int d, r, swaptype, swap_cnt;

loop:	SWAPINIT_00418EF8(a, es);
	swap_cnt = 0;
	if (n < 7) {
		for (pm = (char *) a + es; pm < (char *) a + n * es; pm += es)
			for (pl = pm; pl > (char *) a && cmp(pl - es, pl) > 0;
			     pl -= es)
				swap_00418EF8(pl, pl - es);
		return;
	}
	pm = (char *) a + (n / 2) * es;
	if (n > 7) {
		pl = a;
		pn = (char *) a + (n - 1) * es;
		if (n > 40) {
			d = (n / 8) * es;
			pl = med3_00418EF8(pl, pl + d, pl + 2 * d, cmp);
			pm = med3_00418EF8(pm - d, pm, pm + d, cmp);
			pn = med3_00418EF8(pn - 2 * d, pn - d, pn, cmp);
		}
		pm = med3_00418EF8(pl, pm, pn, cmp);
	}
	swap_00418EF8(a, pm);
	pa = pb = (char *) a + es;

	pc = pd = (char *) a + (n - 1) * es;
	for (;;) {
		while (pb <= pc && (r = cmp(pb, a)) <= 0) {
			if (r == 0) {
				swap_cnt = 1;
				swap_00418EF8(pa, pb);
				pa += es;
			}
			pb += es;
		}
		while (pb <= pc && (r = cmp(pc, a)) >= 0) {
			if (r == 0) {
				swap_cnt = 1;
				swap_00418EF8(pc, pd);
				pd -= es;
			}
			pc -= es;
		}
		if (pb > pc)
			break;
		swap_00418EF8(pb, pc);
		swap_cnt = 1;
		pb += es;
		pc -= es;
	}
	if (swap_cnt == 0) {  /* Switch to insertion sort */
		for (pm = (char *) a + es; pm < (char *) a + n * es; pm += es)
			for (pl = pm; pl > (char *) a && cmp(pl - es, pl) > 0; 
			     pl -= es)
				swap_00418EF8(pl, pl - es);
		return;
	}

	pn = (char *) a + n * es;
	r = min_00418EF8(pa - (char *)a, pb - pa);
	vecswap_00418EF8(a, pb - r, r);
	r = min_00418EF8(pd - pc, pn - pd - es);
	vecswap_00418EF8(pb, pn - r, r);
	if ((r = pb - pa) > es)
		func_00418EF8(a, r / es, es, cmp);
	if ((r = pd - pc) > es) { 
		/* Iterate rather than recurse to save stack space */
		a = pn - r;
		n = r / es;
		goto loop;
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004198D8);
#ifdef SKIP_ASM
/* newlib 1.9 libc/stdlib/strtod.c (_strtod_r) with the mprec.h macros it uses, verbatim (IEEE_8087, Pack_32,
 * no Sudden_Underflow / RND_PRODQUOT / Check_FLT_ROUNDS; FLT_ROUNDS is 1):
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */
struct _Bigint_004198D8
{
  struct _Bigint_004198D8 *_next;
  int _k, _maxwds, _sign, _wds;
  unsigned int _x[1];
};
typedef struct _Bigint_004198D8 _Bigint_004198D8;
struct _reent_004198D8 { int _errno; };
union double_union_004198D8
{
  double d;
  unsigned int i[2];
};
union __dmath_004198D8
{
  unsigned int i[2];
  double d;
};
#define word0_004198D8(x) (x.i[1])
#define word1_004198D8(x) (x.i[0])
#define Exp_msk1_004198D8    ((unsigned int)0x100000L)
#define Exp_mask_004198D8  ((unsigned int)0x7ff00000L)
#define P_004198D8 53
#define Bias_004198D8 1023
#define Emin_004198D8 (-1022)
#define Frac_mask1_004198D8 ((unsigned int)0xfffffL)
#define Ten_pmax_004198D8 22
#define Bndry_mask_004198D8  ((unsigned int)0xfffffL)
#define Bndry_mask1_004198D8 ((unsigned int)0xfffffL)
#define LSB_004198D8 1
#define Log2P_004198D8 1
#define Tiny0_004198D8 0
#define Tiny1_004198D8 1
#define rounded_product_004198D8(a,b) a *= b
#define rounded_quotient_004198D8(a,b) a /= b
#define Big0_004198D8 (Frac_mask1_004198D8 | Exp_msk1_004198D8*(1024+Bias_004198D8-1))
#define Big1_004198D8 ((unsigned int)0xffffffffL)
#define n_bigtens_004198D8 5
#define DBL_DIG_004198D8 15
#define DBL_MAX_10_EXP_004198D8 308
#define DBL_MAX_EXP_004198D8 1024
#define FLT_ROUNDS_004198D8 1
#define FLT_RADIX_004198D8 2
#define ERANGE_004198D8 34
#define HUGE_VAL_004198D8 (infinity_004198D8[0].d)
#define Bcopy_004198D8(x,y) g98D8_memcpy((char *)&x->_sign, (char *)&y->_sign, y->_wds*sizeof(int) + 2*sizeof(int))
extern const union __dmath_004198D8 infinity_004198D8[] __asm__("D_0049A898");
extern const double tens_004198D8[] __asm__("D_0049A8F8");
extern const double bigtens_004198D8[] __asm__("D_0049A9C0");
extern const double tinytens_004198D8[] __asm__("D_0049A9E8");
extern void *g98D8_memcpy(void *, void *, int) __asm__("func_0041605C");
extern _Bigint_004198D8 *g98D8_Balloc(struct _reent_004198D8 *, int) __asm__("func_00420510");
extern void g98D8_Bfree(struct _reent_004198D8 *, _Bigint_004198D8 *) __asm__("func_004205B8");
extern _Bigint_004198D8 *g98D8_s2b(struct _reent_004198D8 *, const char *, int, int, unsigned int) __asm__("func_004206F8");
extern _Bigint_004198D8 *g98D8_i2b(struct _reent_004198D8 *, int) __asm__("func_00420978");
extern _Bigint_004198D8 *g98D8_mult(struct _reent_004198D8 *, _Bigint_004198D8 *, _Bigint_004198D8 *) __asm__("func_004209B0");
extern _Bigint_004198D8 *g98D8_pow5mult(struct _reent_004198D8 *, _Bigint_004198D8 *, int) __asm__("func_00420BC8");
extern _Bigint_004198D8 *g98D8_lshift(struct _reent_004198D8 *, _Bigint_004198D8 *, int) __asm__("func_00420CC8");
extern int g98D8_cmp(_Bigint_004198D8 *, _Bigint_004198D8 *) __asm__("func_00420E28");
extern _Bigint_004198D8 *g98D8_diff(struct _reent_004198D8 *, _Bigint_004198D8 *, _Bigint_004198D8 *) __asm__("func_00420E90");
extern double g98D8_ulp(double) __asm__("func_00421020");
extern _Bigint_004198D8 *g98D8_d2b(struct _reent_004198D8 *, double, int *, int *) __asm__("func_00421280");
extern double g98D8_ratio(_Bigint_004198D8 *, _Bigint_004198D8 *) __asm__("func_00421400");

/* the unit declares this as void func_004198D8(int, int, int) (strtod, func_0041A7E8) */
double func_004198D8_(struct _reent_004198D8 *, const char *, char **) __asm__("func_004198D8");
double func_004198D8_(struct _reent_004198D8 *ptr, const char *s00, char **se)
{
  int bb2, bb5, bbe, bd2, bd5, bbbits, bs2, c, dsign, e1, esign, i, j,
    k, nd, nd0, nf, nz, nz0, sign;
  long e;
  const char *s, *s0, *s1;
  double aadj, aadj1, adj;
  long L;
  unsigned long z;
  unsigned int y;
  union double_union_004198D8 rv, rv0;

  _Bigint_004198D8 *bb, *bb1, *bd, *bd0, *bs, *delta;
  sign = nz0 = nz = 0;
  rv.d = 0.;
  for (s = s00;; s++)
    switch (*s)
      {
      case '-':
	sign = 1;
	/* no break */
      case '+':
	if (*++s)
	  goto break2;
	/* no break */
      case 0:
	s = s00;
	goto ret;
      case '\t':
      case '\n':
      case '\v':
      case '\f':
      case '\r':
      case ' ':
	continue;
      default:
	goto break2;
      }
break2:
  if (*s == '0')
    {
      nz0 = 1;
      while (*++s == '0');
      if (!*s)
	goto ret;
    }
  s0 = s;
  y = z = 0;
  for (nd = nf = 0; (c = *s) >= '0' && c <= '9'; nd++, s++)
    if (nd < 9)
      y = 10 * y + c - '0';
    else if (nd < 16)
      z = 10 * z + c - '0';
  nd0 = nd;
  if (c == '.')
    {
      c = *++s;
      if (!nd)
	{
	  for (; c == '0'; c = *++s)
	    nz++;
	  if (c > '0' && c <= '9')
	    {
	      s0 = s;
	      nf += nz;
	      nz = 0;
	      goto have_dig;
	    }
	  goto dig_done;
	}
      for (; c >= '0' && c <= '9'; c = *++s)
	{
	have_dig:
	  nz++;
	  if (c -= '0')
	    {
	      nf += nz;
	      for (i = 1; i < nz; i++)
		if (nd++ < 9)
		  y *= 10;
		else if (nd <= DBL_DIG_004198D8 + 1)
		  z *= 10;
	      if (nd++ < 9)
		y = 10 * y + c;
	      else if (nd <= DBL_DIG_004198D8 + 1)
		z = 10 * z + c;
	      nz = 0;
	    }
	}
    }
dig_done:
  e = 0;
  if (c == 'e' || c == 'E')
    {
      if (!nd && !nz && !nz0)
	{
	  s = s00;
	  goto ret;
	}
      s00 = s;
      esign = 0;
      switch (c = *++s)
	{
	case '-':
	  esign = 1;
	case '+':
	  c = *++s;
	}
      if (c >= '0' && c <= '9')
	{
	  while (c == '0')
	    c = *++s;
	  if (c > '0' && c <= '9')
	    {
	      e = c - '0';
	      s1 = s;
	      while ((c = *++s) >= '0' && c <= '9')
		e = 10 * e + c - '0';
	      if (s - s1 > 8)
		/* Avoid confusion from exponents
		 * so large that e might overflow.
		 */
		e = 9999999L;
	      if (esign)
		e = -e;
	    }
	  else
	    e = 0;
	}
      else
	s = s00;
    }
  if (!nd)
    {
      if (!nz && !nz0)
	s = s00;
      goto ret;
    }
  e1 = e -= nf;

  /* Now we have nd0 digits, starting at s0, followed by a
   * decimal point, followed by nd-nd0 digits.  The number we're
   * after is the integer represented by those digits times
   * 10**e */

  if (!nd0)
    nd0 = nd;
  k = nd < DBL_DIG_004198D8 + 1 ? nd : DBL_DIG_004198D8 + 1;
  rv.d = y;
  if (k > 9)
    rv.d = tens_004198D8[k - 9] * rv.d + z;
  bd0 = 0;
  if (nd <= DBL_DIG_004198D8
      && FLT_ROUNDS_004198D8 == 1
    )
    {
      if (!e)
	goto ret;
      if (e > 0)
	{
	  if (e <= Ten_pmax_004198D8)
	    {
	      /* rv.d = */ rounded_product_004198D8 (rv.d, tens_004198D8[e]);
	      goto ret;
	    }
	  i = DBL_DIG_004198D8 - nd;
	  if (e <= Ten_pmax_004198D8 + i)
	    {
	      /* A fancier test would sometimes let us do
				 * this for larger i values.
				 */
	      e -= i;
	      rv.d *= tens_004198D8[i];
	      /* rv.d = */ rounded_product_004198D8 (rv.d, tens_004198D8[e]);
	      goto ret;
	    }
	}
      else if (e >= -Ten_pmax_004198D8)
	{
	  /* rv.d = */ rounded_quotient_004198D8 (rv.d, tens_004198D8[-e]);
	  goto ret;
	}
    }
  e1 += nd - k;

  /* Get starting approximation = rv.d * 10**e1 */

  if (e1 > 0)
    {
      if ((i = e1 & 15) != 0)
	rv.d *= tens_004198D8[i];
      if (e1 &= ~15)
	{
	  if (e1 > DBL_MAX_10_EXP_004198D8)
	    {
	    ovfl:
	      ptr->_errno = ERANGE_004198D8;
	      rv.d = HUGE_VAL_004198D8;
	      if (bd0)
		goto retfree;
	      goto ret;
	    }
	  if (e1 >>= 4)
	    {
	      for (j = 0; e1 > 1; j++, e1 >>= 1)
		if (e1 & 1)
		  rv.d *= bigtens_004198D8[j];
	      /* The last multiplication could overflow. */
	      word0_004198D8 (rv) -= P_004198D8 * Exp_msk1_004198D8;
	      rv.d *= bigtens_004198D8[j];
	      if ((z = word0_004198D8 (rv) & Exp_mask_004198D8)
		  > Exp_msk1_004198D8 * (DBL_MAX_EXP_004198D8 + Bias_004198D8 - P_004198D8))
		goto ovfl;
	      if (z > Exp_msk1_004198D8 * (DBL_MAX_EXP_004198D8 + Bias_004198D8 - 1 - P_004198D8))
		{
		  /* set to largest number */
		  /* (Can't trust DBL_MAX) */
		  word0_004198D8 (rv) = Big0_004198D8;
		  word1_004198D8 (rv) = Big1_004198D8;
		}
	      else
		word0_004198D8 (rv) += P_004198D8 * Exp_msk1_004198D8;
	    }

	}
    }
  else if (e1 < 0)
    {
      e1 = -e1;
      if ((i = e1 & 15) != 0)
	rv.d /= tens_004198D8[i];
      if (e1 &= ~15)
	{
	  e1 >>= 4;
	  if (e1 >= 1 << n_bigtens_004198D8)
            goto undfl;
	  for (j = 0; e1 > 1; j++, e1 >>= 1)
	    if (e1 & 1)
	      rv.d *= tinytens_004198D8[j];
	  /* The last multiplication could underflow. */
	  rv0.d = rv.d;
	  rv.d *= tinytens_004198D8[j];
	  if (!rv.d)
	    {
	      rv.d = 2. * rv0.d;
	      rv.d *= tinytens_004198D8[j];
	      if (!rv.d)
		{
		undfl:
		  rv.d = 0.;
		  ptr->_errno = ERANGE_004198D8;
		  if (bd0)
		    goto retfree;
		  goto ret;
		}
	      word0_004198D8 (rv) = Tiny0_004198D8;
	      word1_004198D8 (rv) = Tiny1_004198D8;
	      /* The refinement below will clean
	       * this approximation up.
	       */
	    }
	}
    }

  /* Now the hard part -- adjusting rv to the correct value.*/

  /* Put digits into bd: true value = bd * 10^e */

  bd0 = g98D8_s2b (ptr, s0, nd0, nd, y);

  for (;;)
    {
      bd = g98D8_Balloc (ptr, bd0->_k);
      Bcopy_004198D8 (bd, bd0);
      bb = g98D8_d2b (ptr, rv.d, &bbe, &bbbits);	/* rv.d = bb * 2^bbe */
      bs = g98D8_i2b (ptr, 1);

      if (e >= 0)
	{
	  bb2 = bb5 = 0;
	  bd2 = bd5 = e;
	}
      else
	{
	  bb2 = bb5 = -e;
	  bd2 = bd5 = 0;
	}
      if (bbe >= 0)
	bb2 += bbe;
      else
	bd2 -= bbe;
      bs2 = bb2;
      i = bbe + bbbits - 1;	/* logb(rv.d) */
      if (i < Emin_004198D8)		/* denormal */
	j = bbe + (P_004198D8 - Emin_004198D8);
      else
	j = P_004198D8 + 1 - bbbits;
      bb2 += j;
      bd2 += j;
      i = bb2 < bd2 ? bb2 : bd2;
      if (i > bs2)
	i = bs2;
      if (i > 0)
	{
	  bb2 -= i;
	  bd2 -= i;
	  bs2 -= i;
	}
      if (bb5 > 0)
	{
	  bs = g98D8_pow5mult (ptr, bs, bb5);
	  bb1 = g98D8_mult (ptr, bs, bb);
	  g98D8_Bfree (ptr, bb);
	  bb = bb1;
	}
      if (bb2 > 0)
	bb = g98D8_lshift (ptr, bb, bb2);
      if (bd5 > 0)
	bd = g98D8_pow5mult (ptr, bd, bd5);
      if (bd2 > 0)
	bd = g98D8_lshift (ptr, bd, bd2);
      if (bs2 > 0)
	bs = g98D8_lshift (ptr, bs, bs2);
      delta = g98D8_diff (ptr, bb, bd);
      dsign = delta->_sign;
      delta->_sign = 0;
      i = g98D8_cmp (delta, bs);
      if (i < 0)
	{
	  /* Error is less than half an ulp -- check for
	   * special case of mantissa a power of two.
	   */
	  if (dsign || word1_004198D8 (rv) || word0_004198D8 (rv) & Bndry_mask_004198D8)
	    break;
	  delta = g98D8_lshift (ptr, delta, Log2P_004198D8);
	  if (g98D8_cmp (delta, bs) > 0)
	    goto drop_down;
	  break;
	}
      if (i == 0)
	{
	  /* exactly half-way between */
	  if (dsign)
	    {
	      if ((word0_004198D8 (rv) & Bndry_mask1_004198D8) == Bndry_mask1_004198D8
		  && word1_004198D8 (rv) == 0xffffffff)
		{
		  /*boundary case -- increment exponent*/
		  word0_004198D8 (rv) = (word0_004198D8 (rv) & Exp_mask_004198D8)
		    + Exp_msk1_004198D8
		    ;
		  word1_004198D8 (rv) = 0;
		  break;
		}
	    }
	  else if (!(word0_004198D8 (rv) & Bndry_mask_004198D8) && !word1_004198D8 (rv))
	    {
	    drop_down:
	      /* boundary case -- decrement exponent */
	      L = (word0_004198D8 (rv) & Exp_mask_004198D8) - Exp_msk1_004198D8;
	      word0_004198D8 (rv) = L | Bndry_mask1_004198D8;
	      word1_004198D8 (rv) = 0xffffffff;
	      break;
	    }
	  if (!(word1_004198D8 (rv) & LSB_004198D8))
	    break;
	  if (dsign)
	    rv.d += g98D8_ulp (rv.d);
	  else
	    {
	      rv.d -= g98D8_ulp (rv.d);
	      if (!rv.d)
		goto undfl;
	    }
	  break;
	}
      if ((aadj = g98D8_ratio (delta, bs)) <= 2.)
	{
	  if (dsign)
	    aadj = aadj1 = 1.;
	  else if (word1_004198D8 (rv) || word0_004198D8 (rv) & Bndry_mask_004198D8)
	    {
	      if (word1_004198D8 (rv) == Tiny1_004198D8 && !word0_004198D8 (rv))
		goto undfl;
	      aadj = 1.;
	      aadj1 = -1.;
	    }
	  else
	    {
	      /* special case -- power of FLT_RADIX to be */
	      /* rounded down... */

	      if (aadj < 2. / FLT_RADIX_004198D8)
		aadj = 1. / FLT_RADIX_004198D8;
	      else
		aadj *= 0.5;
	      aadj1 = -aadj;
	    }
	}
      else
	{
	  aadj *= 0.5;
	  aadj1 = dsign ? aadj : -aadj;
	  if (FLT_ROUNDS_004198D8 == 0)
	    aadj1 += 0.5;
	}
      y = word0_004198D8 (rv) & Exp_mask_004198D8;

      /* Check for overflow */

      if (y == Exp_msk1_004198D8 * (DBL_MAX_EXP_004198D8 + Bias_004198D8 - 1))
	{
	  rv0.d = rv.d;
	  word0_004198D8 (rv) -= P_004198D8 * Exp_msk1_004198D8;
	  adj = aadj1 * g98D8_ulp (rv.d);
	  rv.d += adj;
	  if ((word0_004198D8 (rv) & Exp_mask_004198D8) >=
	      Exp_msk1_004198D8 * (DBL_MAX_EXP_004198D8 + Bias_004198D8 - P_004198D8))
	    {
	      if (word0_004198D8 (rv0) == Big0_004198D8 && word1_004198D8 (rv0) == Big1_004198D8)
		goto ovfl;
	      word0_004198D8 (rv) = Big0_004198D8;
	      word1_004198D8 (rv) = Big1_004198D8;
	      goto cont;
	    }
	  else
	    word0_004198D8 (rv) += P_004198D8 * Exp_msk1_004198D8;
	}
      else
	{
	  /* Compute adj so that the IEEE rounding rules will
	   * correctly round rv.d + adj in some half-way cases.
	   * If rv.d * ulp(rv.d) is denormalized (i.e.,
	   * y <= (P-1)*Exp_msk1), we must adjust aadj to avoid
	   * trouble from bits lost to denormalization;
	   * example: 1.2e-307 .
	   */
	  if (y <= (P_004198D8 - 1) * Exp_msk1_004198D8 && aadj >= 1.)
	    {
	      aadj1 = (double) (int) (aadj + 0.5);
	      if (!dsign)
		aadj1 = -aadj1;
	    }
	  adj = aadj1 * g98D8_ulp (rv.d);
	  rv.d += adj;
	}
      z = word0_004198D8 (rv) & Exp_mask_004198D8;
      if (y == z)
	{
	  /* Can we stop now? */
	  L = aadj;
	  aadj -= L;
	  /* The tolerances below are conservative. */
	  if (dsign || word1_004198D8 (rv) || word0_004198D8 (rv) & Bndry_mask_004198D8)
	    {
	      if (aadj < .4999999 || aadj > .5000001)
		break;
	    }
	  else if (aadj < .4999999 / FLT_RADIX_004198D8)
	    break;
	}
    cont:
      g98D8_Bfree (ptr, bb);
      g98D8_Bfree (ptr, bd);
      g98D8_Bfree (ptr, bs);
      g98D8_Bfree (ptr, delta);
    }
retfree:
  g98D8_Bfree (ptr, bb);
  g98D8_Bfree (ptr, bd);
  g98D8_Bfree (ptr, bs);
  g98D8_Bfree (ptr, bd0);
  g98D8_Bfree (ptr, delta);
ret:
  if (se)
    *se = (char *) s;
  return sign ? -rv.d : rv.d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041A7E8);
#ifdef SKIP_ASM
extern int D_00454944;
extern void func_004198D8(int, int, int);
void func_0041A7E8(int a, int b) {
    func_004198D8(D_00454944, a, b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041A818);
#ifdef SKIP_ASM
extern void dptofp(int);
void func_0041A818(void) {
    dptofp(((int (*)(void))func_0041A7E8)());
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041A840);
#ifdef SKIP_ASM
extern char D_00499C81[];
extern long func_00411538_div(long, long) __asm__("func_00411538");
extern long func_004114D0_mul(long, long) __asm__("func_004114D0");
extern long func_0042DAD8_mod(long, long) __asm__("func_0042DAD8");
#define XCT(c) (D_00499C81[(int)(c)])
unsigned long func_0041A840_(int *ptr, char *nptr, char **endptr, int base) __asm__("func_0041A840");
unsigned long func_0041A840_(int *ptr, char *nptr, char **endptr, int base) {
    char *s = nptr;
    unsigned long acc, cutoff;
    int c, neg = 0, any, cutlim;
    do {
        c = *s++;
    } while (XCT(c) & 8);
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+')
        c = *s++;
    if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = func_00411538_div(-1, base);
    cutlim = func_0042DAD8_mod(-1, base);
    for (acc = 0, any = 0;; c = *s++) {
        if (XCT(c) & 4)
            c -= '0';
        else if (XCT(c) & 3)
            c -= (XCT(c) & 1) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc = func_004114D0_mul(acc, base);
            acc += c;
        }
    }
    if (any < 0) {
        acc = -1;
        *ptr = 34;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = any ? s - 1 : nptr;
    return acc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AA50);
#ifdef SKIP_ASM
extern int D_00454944;
extern void func_0041A840(int, int, int, int);
void func_0041AA50(int a, int b, int c) {
    func_0041A840(D_00454944, a, b, c);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AA88);
#ifdef SKIP_ASM
int func_0041AA88(char *a, char *b) {
    while (*a && func_0041EA18(*a) == func_0041EA18(*b)) {
        a++;
        b++;
    }
    return func_0041EA18((unsigned char)*a) - func_0041EA18((unsigned char)*b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AB08);
#ifdef SKIP_ASM
int func_0041AB08(char *s1, char *s2) {
    char *s = s1;
    char *c;
    while (*s1) {
        for (c = s2; *c; c++)
            if (*s1 == *c)
                break;
        if (*c)
            break;
        s1++;
    }
    return s1 - s;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AB78);
#ifdef SKIP_ASM
extern int func_0041EA18(int);
int func_0041AB78(char *s1, char *s2, unsigned n) {
    if (n == 0)
        return 0;
    while (n-- != 0 && func_0041EA18(*s1) == func_0041EA18(*s2)) {
        if (n == 0 || *s1 == '\0' || *s2 == '\0')
            break;
        s1++;
        s2++;
    }
    return func_0041EA18(*(unsigned char *)s1) - func_0041EA18(*(unsigned char *)s2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AC40);
#ifdef SKIP_ASM
char *func_0041AC40(char *s, char *accept) {
    char *p;
    if (*s == 0)
        return 0;
    for (; *s; s++) {
        for (p = accept; *p && *s != *p; p++)
            ;
        if (*p)
            break;
    }
    return *p ? s : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041ACC0);
#ifdef SKIP_ASM
char *func_0041ACC0(char *s, char c) {
    char *r = 0;
    while (*s) {
        if (*s == c) r = s;
        s++;
    }
    return *s == c ? s : r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AD10);
#ifdef SKIP_ASM
int func_0041AD10(char *s1, char *s2) {
    char *s = s1;
    char *c;
    while (*s1) {
        for (c = s2; *c; c++)
            if (*s1 == *c)
                break;
        if (*c == 0)
            break;
        s1++;
    }
    return s1 - s;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AD80);
#ifdef SKIP_ASM
/* newlib string/strstr.c (no notice in the file; newlib's default: Copyright (c) 1994-2000 Red Hat, Inc.
 * Permission to use, copy, modify and distribute this software is freely granted, provided that this notice is
 * preserved.) */
char *
func_0041AD80 (const char *searchee, const char *lookfor)
{
  if (*searchee == 0)
    {
      if (*lookfor)
	return (char *) 0;
      return (char *) searchee;
    }

  while (*searchee)
    {
      unsigned int i;
      i = 0;

      while (1)
	{
	  if (lookfor[i] == 0)
	    {
	      return (char *) searchee;
	    }

	  if (lookfor[i] != searchee[i])
	    {
	      break;
	    }
	  i++;
	}
      searchee++;
    }

  return (char *) 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AE00);
#ifdef SKIP_ASM
/* Sony's libc float-to-string helper (not in newlib 1.9.0; called by func_0041AF88): round the n-digit decimal
   string s at its last digit, propagating the carry. Returns 0 when the carry runs out of the first digit. */
int func_0041AE00(char *s, int n)
{
    if (n > 0) {
        n--;
        if (s[n] >= '5') {
            do
                s[n] = '0';
            while (--n > 0 && s[n] == '9');
            if (s[n] == '9')
                return 0;
            s[n]++;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AE70);
#ifdef SKIP_ASM
char *func_0041AE70(char *s) {
    int i, j;
    char t;
    j = 0;
    while (s[j])
        j++;
    j--;
    for (i = 0; i < j; i++, j--) {
        t = s[i];
        s[i] = s[j];
        s[j] = t;
    }
    return s;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AEE8);
#ifdef SKIP_ASM
/* Sony's libc addition itoa (not in newlib 1.9.0): digits from the .data table, then reversed in place by
   func_0041AE70 (strrev). */
extern char D_00454CB0[];
extern char *func_0041AE70(char *s);

char *func_0041AEE8(int value, char *buf, unsigned int base)
{
    int i;
    int neg = value < 0;
    unsigned int v;

    v = value;
    if (neg && base == 10)
        v = -value;
    i = 0;
    do {
        buf[i++] = D_00454CB0[v % base];
        v /= base;
    } while (v != 0);
    if (neg)
        buf[i++] = '-';
    buf[i] = 0;
    return func_0041AE70(buf);
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_0041AF88);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041B528);
#ifdef SKIP_ASM
extern void func_0041B550(int, int, int, int);
void func_0041B528(short *p) {
    func_0041B550(p[7], 0, 0, 2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041B550);
#ifdef SKIP_ASM
extern char *D_00454CDC;
extern int D_00454CD8;
extern char D_0052AE98[];
extern int func_00424368(int, int, int);
int func_0041B550_(int fd, char *buf, unsigned n, int mode) __asm__("func_0041B550");
int func_0041B550_(int fd, char *buf, unsigned n, int mode) {
    unsigned i;
    int r;
    if (mode == 1) {
        r = func_00424368(fd, (int)D_0052AE98, D_00454CD8);
        D_00454CD8 = 0;
        D_00454CDC = D_0052AE98;
        return r;
    }
    for (i = 0; i < n; i++) {
        *D_00454CDC = buf[i];
        D_00454CDC++;
        D_00454CD8++;
        if (D_00454CD8 >= 128) {
            r = func_00424368(fd, (int)D_0052AE98, D_00454CD8);
            D_00454CDC = D_0052AE98;
            D_00454CD8 = 0;
            if (r == 0)
                return 0;
        }
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041B658);
#ifdef SKIP_ASM
extern int func_0041B728_f(int, int, char *, char *) __asm__("func_0041B728");
extern int func_0041CA70_i(int, int, char *, char *) __asm__("func_0041CA70");
int func_0041B658(int fp, int fmt, char *ap) {
    char *p = (char *)fmt;
    while (*p) {
        if (*p == '%' && p[1]) {
            p++;
            while (*p < 'A' && p[1])
                p++;
            switch (*p) {
            case 'E':
            case 'G':
            case 'L':
            case 'e':
            case 'f':
            case 'g':
                return func_0041B728_f(*(int *)(fp + 0x54), fp, (char *)fmt, ap);
            }
        }
        p++;
    }
    return func_0041CA70_i(*(int *)(fp + 0x54), fp, (char *)fmt, ap);
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_0041B728);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041C788);
#ifdef SKIP_ASM
extern void *g_c788_memcpy(void *, const void *, unsigned) __asm__("func_0041605C");
char *func_0041C788_impl(char *dest, char *start, char *end, int *outlen) __asm__("func_0041C788");
char *func_0041C788_impl(char *dest, char *start, char *end, int *outlen) {
    int len = end - start + 10;
    char *buf = __builtin_alloca(len);
    char *w = buf + len - 1;
    int cnt = 0;
    int st[2];
    char *p;
    if (dest == 0 || start == 0 || end == 0) return 0;
    st[0] = 0;
    st[1] = (int)end;
    p = start;
    if (!(end < p)) {
        do {
            switch (*p) {
            case '.':
                st[1] = (int)p;
                if (st[0] == 0) st[0] = 1;
                break;
            case 'E':
            case 'e':
                st[0] = 2;
                break;
            }
            p++;
        } while (!(end < p) && st[0] != 2);
    }
    if (st[0] == 2) return start;
    p = end;
    if (start < p) {
        do {
            *w = *p;
            w--;
            if (!((char *)st[1] < p)) {
                if (cnt % 3 == 0 && cnt != 0) {
                    *w = ',';
                    len++;
                    w--;
                }
                cnt++;
            }
            p--;
        } while (start < p);
    }
    *w = *p;
    len -= 10;
    g_c788_memcpy(dest, w, len);
    *outlen = len;
    return dest;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041C938);
#ifdef SKIP_ASM
extern char *D_00454CE4;
extern int D_00454CE0;
extern char D_0052AF18[];
int func_0041C938_(int fd, char *buf, unsigned n, int mode) __asm__("func_0041C938");
int func_0041C938_(int fd, char *buf, unsigned n, int mode) {
    unsigned i;
    int r;
    if (mode == 1) {
        r = func_00424368(fd, (int)D_0052AF18, D_00454CE0);
        D_00454CE0 = 0;
        D_00454CE4 = D_0052AF18;
        return r;
    }
    for (i = 0; i < n; i++) {
        *D_00454CE4 = buf[i];
        D_00454CE4++;
        D_00454CE0++;
        if (D_00454CE0 >= 128) {
            r = func_00424368(fd, (int)D_0052AF18, D_00454CE0);
            D_00454CE4 = D_0052AF18;
            D_00454CE0 = 0;
            if (r == 0)
                return 0;
        }
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041CA40);
#ifdef SKIP_ASM
extern void func_0041CA70(int, int *, int, int);
void func_0041CA40(int *p, int b, int c) {
    func_0041CA70(p[21], p, b, c);
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_0041CA70);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041D990);
#ifdef SKIP_ASM
struct mchunk {
    unsigned prev_size, size;
    struct mchunk *fd, *bk;
};
typedef struct mchunk *mchunkptr;
extern unsigned D_00454CE8[];
extern unsigned long D_004550F0;
extern unsigned long D_004550F8;
#define bin_at(i) ((mchunkptr)((char *)&D_00454CE8[2 * (i) + 2] - 8))
#define top (bin_at(0)->fd)
#define last_remainder (bin_at(1))
#define binblocks (bin_at(0)->size)
#define chunk_at_offset(p, s) ((mchunkptr)((char *)(p) + (s)))
#define chunksize(p) ((p)->size & ~3u)
#define inuse_bit_at_offset(p, s) (chunk_at_offset(p, s)->size & 1)
#define unlink(P, BK, FD) \
    { \
        BK = (P)->bk; \
        FD = (P)->fd; \
        FD->bk = BK; \
        BK->fd = FD; \
    }
#define set_head(p, s) ((p)->size = (s))
#define set_foot(p, s) (chunk_at_offset(p, s)->prev_size = (s))
#define idx2binblock(ix) ((unsigned long)1 << ((ix) / 4))
#define mark_binblock(ii) (binblocks |= idx2binblock(ii))
#define smallbin_index(sz) (((unsigned)(sz)) >> 3)
#define bin_index(sz) \
    ((((unsigned)(sz)) >> 9 == 0) ? (((unsigned)(sz)) >> 3) \
    : (((unsigned)(sz)) >> 9 <= 4) ? 56 + (((unsigned)(sz)) >> 6) \
    : (((unsigned)(sz)) >> 9 <= 20) ? 91 + (((unsigned)(sz)) >> 9) \
    : (((unsigned)(sz)) >> 9 <= 84) ? 110 + (((unsigned)(sz)) >> 12) \
    : (((unsigned)(sz)) >> 9 <= 340) ? 119 + (((unsigned)(sz)) >> 15) \
    : (((unsigned)(sz)) >> 9 <= 1364) ? 124 + (((unsigned)(sz)) >> 18) \
    : 126)
#define frontlink(P, S, IDX, BK, FD) \
    { \
        if (S < 512) { \
            IDX = smallbin_index(S); \
            mark_binblock(IDX); \
            BK = bin_at(IDX); \
            FD = BK->fd; \
            P->bk = BK; \
            P->fd = FD; \
            FD->bk = BK->fd = P; \
        } else { \
            IDX = bin_index(S); \
            BK = bin_at(IDX); \
            FD = BK->fd; \
            if (FD == BK) { \
                mark_binblock(IDX); \
            } else { \
                while (FD != BK && S < chunksize(FD)) \
                    FD = FD->fd; \
                BK = FD->bk; \
            } \
            P->bk = BK; \
            P->fd = FD; \
            FD->bk = BK->fd = P; \
        } \
    }
#define link_last_remainder(P) \
    { \
        lr->fd = lr->bk = P; \
        P->fd = P->bk = lr; \
    }

#define set_head_size(p, s) ((p)->size = (((p)->size & 1) | (s)))
extern char *D_00455100;
extern int D_00455118;
extern unsigned long D_00455108;
extern unsigned long D_00455110;
extern char *func_0041FD70_sbrk(int, int) __asm__("func_0041FD70");
extern void func_0041E340_free(int, void *) __asm__("func_0041E340");
void func_0041D990_(int ptr, unsigned nb) __asm__("func_0041D990");
void func_0041D990_(int ptr, unsigned nb) {
    char *brk;
    unsigned front_misalign;
    unsigned correction;
    char *new_brk;
    unsigned top_size;
    mchunkptr old_top = top;
    unsigned old_top_size = chunksize(old_top);
    char *old_end = (char *)chunk_at_offset(old_top, old_top_size);
    unsigned sbrk_size = nb + D_004550F8 + 16;
    unsigned long pagesz = 0x1000;
    if (D_00455100 != (char *)-1)
        sbrk_size = (sbrk_size + (pagesz - 1)) & ~(pagesz - 1);
    brk = func_0041FD70_sbrk(ptr, sbrk_size);
    if (brk == (char *)-1 || (brk < old_end && old_top != (mchunkptr)D_00454CE8))
        return;
    D_00455118 += sbrk_size;
    if (brk == old_end) {
        top_size = sbrk_size + old_top_size;
        set_head(top, top_size | 1);
    } else {
        if (D_00455100 == (char *)-1)
            D_00455100 = brk;
        else
            D_00455118 += brk - old_end;
        front_misalign = (unsigned long)(brk + 8) & 15;
        if (front_misalign > 0) {
            correction = 16 - front_misalign;
            brk += correction;
        } else
            correction = 0;
        correction += pagesz - ((unsigned long)(brk + sbrk_size) & (pagesz - 1));
        new_brk = func_0041FD70_sbrk(ptr, correction);
        if (new_brk == (char *)-1)
            return;
        D_00455118 += correction;
        top = (mchunkptr)brk;
        top_size = new_brk - brk + correction;
        set_head(top, top_size | 1);
        if (old_top != (mchunkptr)D_00454CE8) {
            if (old_top_size < 16) {
                set_head(top, 1);
                return;
            }
            old_top_size = (old_top_size - 12) & ~15u;
            set_head_size(old_top, old_top_size);
            chunk_at_offset(old_top, old_top_size)->size = 5;
            chunk_at_offset(old_top, old_top_size + 4)->size = 5;
            if (old_top_size >= 16)
                func_0041E340_free(ptr, (char *)old_top + 8);
        }
    }
    if ((unsigned long)D_00455118 > D_00455108)
        D_00455108 = D_00455118;
    if ((unsigned long)D_00455118 > D_00455110)
        D_00455110 = D_00455118;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041DBF8);
#ifdef SKIP_ASM
/* newlib 1.9 libc/stdlib/mallocr.c (mALLOc = _malloc_r) and its macros, verbatim:
 *
 * A version of malloc/free/realloc written by Doug Lea and released to the
 * public domain.  Send questions/comments/complaints/performance data
 * to dl@cs.oswego.edu
 * VERSION 2.6.4  Thu Nov 28 07:54:55 1996  Doug Lea  (dl at gee)
 *
 * Configuration as the retail code shows: MALLOC_ALIGNMENT 16, SIZE_T_SMALLER_THAN_LONG (EE long is 64-bit),
 * no HAVE_MMAP. av_ (__malloc_av_) is D_00454CE8, one 0x408-byte object (config/symbol_addrs.txt).
 */
typedef mchunkptr mbinptr_0041DBF8;
#define SIZE_SZ_0041DBF8                (sizeof(unsigned int))
#define MALLOC_ALIGNMENT_0041DBF8       16
#define MALLOC_ALIGN_0041DBF8           MALLOC_ALIGNMENT_0041DBF8
#define MALLOC_ALIGN_MASK_0041DBF8      (MALLOC_ALIGNMENT_0041DBF8 - 1)
#define MINSIZE_0041DBF8                (sizeof(struct mchunk))
#define chunk2mem_0041DBF8(p)   ((void*)((char*)(p) + 2*SIZE_SZ_0041DBF8))
#define request2size_0041DBF8(req) \
 (((long)((req) + (SIZE_SZ_0041DBF8 + MALLOC_ALIGN_MASK_0041DBF8)) < \
  (long)(MINSIZE_0041DBF8 + MALLOC_ALIGN_MASK_0041DBF8)) ? ((MINSIZE_0041DBF8 + MALLOC_ALIGN_MASK_0041DBF8) & ~(MALLOC_ALIGN_MASK_0041DBF8)) : \
   (((req) + (SIZE_SZ_0041DBF8 + MALLOC_ALIGN_MASK_0041DBF8)) & ~(MALLOC_ALIGN_MASK_0041DBF8)))
#define long_sub_size_t_0041DBF8(x, y) ( (x < y) ? -((long)(y - x)) : (x - y) );
#define PREV_INUSE_0041DBF8 0x1
#define SIZE_BITS_0041DBF8 (0x1|0x2)
#define set_inuse_bit_at_offset_0041DBF8(p, s)\
 (((mchunkptr)(((char*)(p)) + (s)))->size |= PREV_INUSE_0041DBF8)
#define chunksize_0041DBF8(p)          ((p)->size & ~(SIZE_BITS_0041DBF8))
#define set_head_0041DBF8(p, s)        ((p)->size = (s))
#define set_foot_0041DBF8(p, s)   (((mchunkptr)((char*)(p) + (s)))->prev_size = (s))
#define av__0041DBF8 D_00454CE8
#define bin_at_0041DBF8(i)      ((mbinptr_0041DBF8)((char*)&(av__0041DBF8[2*(i) + 2]) - 2*SIZE_SZ_0041DBF8))
#define next_bin_0041DBF8(b)    ((mbinptr_0041DBF8)((char*)(b) + 2 * sizeof(mbinptr_0041DBF8)))
#define prev_bin_0041DBF8(b)    ((mbinptr_0041DBF8)((char*)(b) - 2 * sizeof(mbinptr_0041DBF8)))
#define top_0041DBF8            (bin_at_0041DBF8(0)->fd)
#define last_remainder_0041DBF8 (bin_at_0041DBF8(1))
#define first_0041DBF8(b) ((b)->fd)
#define last_0041DBF8(b)  ((b)->bk)
#define bin_index_0041DBF8(sz)                                                          \
(((((unsigned long)(sz)) >> 9) ==    0) ?       (((unsigned long)(sz)) >>  3): \
 ((((unsigned long)(sz)) >> 9) <=    4) ?  56 + (((unsigned long)(sz)) >>  6): \
 ((((unsigned long)(sz)) >> 9) <=   20) ?  91 + (((unsigned long)(sz)) >>  9): \
 ((((unsigned long)(sz)) >> 9) <=   84) ? 110 + (((unsigned long)(sz)) >> 12): \
 ((((unsigned long)(sz)) >> 9) <=  340) ? 119 + (((unsigned long)(sz)) >> 15): \
 ((((unsigned long)(sz)) >> 9) <= 1364) ? 124 + (((unsigned long)(sz)) >> 18): \
                                          126)
#define MAX_SMALLBIN_SIZE_0041DBF8   512
#define SMALLBIN_WIDTH_0041DBF8        8
#define SMALLBIN_WIDTH_BITS_0041DBF8   3
#define MAX_SMALLBIN_0041DBF8        (MAX_SMALLBIN_SIZE_0041DBF8 / SMALLBIN_WIDTH_0041DBF8) - 1
#define smallbin_index_0041DBF8(sz)  (((unsigned long)(sz)) >> SMALLBIN_WIDTH_BITS_0041DBF8)
#define is_small_request_0041DBF8(nb) (nb < MAX_SMALLBIN_SIZE_0041DBF8 - SMALLBIN_WIDTH_0041DBF8)
#define BINBLOCKWIDTH_0041DBF8     4
#define binblocks_0041DBF8      (bin_at_0041DBF8(0)->size)
#define idx2binblock_0041DBF8(ix)    ((unsigned long)1 << (ix / BINBLOCKWIDTH_0041DBF8))
#define mark_binblock_0041DBF8(ii)   (binblocks_0041DBF8 |= idx2binblock_0041DBF8(ii))
#define frontlink_0041DBF8(P, S, IDX, BK, FD)                                 \
{                                                                             \
  if (S < MAX_SMALLBIN_SIZE_0041DBF8)                                         \
  {                                                                           \
    IDX = smallbin_index_0041DBF8(S);                                         \
    mark_binblock_0041DBF8(IDX);                                              \
    BK = bin_at_0041DBF8(IDX);                                                \
    FD = BK->fd;                                                              \
    P->bk = BK;                                                               \
    P->fd = FD;                                                               \
    FD->bk = BK->fd = P;                                                      \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    IDX = bin_index_0041DBF8(S);                                              \
    BK = bin_at_0041DBF8(IDX);                                                \
    FD = BK->fd;                                                              \
    if (FD == BK) mark_binblock_0041DBF8(IDX);                                \
    else                                                                      \
    {                                                                         \
      while (FD != BK && S < chunksize_0041DBF8(FD)) FD = FD->fd;             \
      BK = FD->bk;                                                            \
    }                                                                         \
    P->bk = BK;                                                               \
    P->fd = FD;                                                               \
    FD->bk = BK->fd = P;                                                      \
  }                                                                           \
}
#define unlink_0041DBF8(P, BK, FD)                                            \
{                                                                             \
  BK = P->bk;                                                                 \
  FD = P->fd;                                                                 \
  FD->bk = BK;                                                                \
  BK->fd = FD;                                                                \
}
#define link_last_remainder_0041DBF8(P)                                       \
{                                                                             \
  last_remainder_0041DBF8->fd = last_remainder_0041DBF8->bk =  P;             \
  P->fd = P->bk = last_remainder_0041DBF8;                                    \
}
#define clear_last_remainder_0041DBF8 \
  (last_remainder_0041DBF8->fd = last_remainder_0041DBF8->bk = last_remainder_0041DBF8)
#define MALLOC_LOCK_0041DBF8 func_00418CA8(reent_ptr)
#define MALLOC_UNLOCK_0041DBF8 func_00418D08(reent_ptr)
extern void g41DBF8_extend_top(int, unsigned int) __asm__("func_0041D990");

/* the unit declares this as int func_0041DBF8(int, int) (malloc, func_004188C0) */
void *func_0041DBF8_(int, unsigned int) __asm__("func_0041DBF8");
void *func_0041DBF8_(int reent_ptr, unsigned int bytes)
{
  mchunkptr victim;                  /* inspected/selected chunk */
  unsigned int victim_size;          /* its size */
  int       idx;                     /* index for bin traversal */
  mbinptr_0041DBF8 bin;              /* associated bin */
  mchunkptr remainder;               /* remainder from a split */
  long      remainder_size;          /* its size */
  int       remainder_index;         /* its bin index */
  unsigned long block;               /* block traverser bit */
  int       startidx;                /* first bin of a traversed block */
  mchunkptr fwd;                     /* misc temp for linking */
  mchunkptr bck;                     /* misc temp for linking */
  mbinptr_0041DBF8 q;                /* misc temp */

  unsigned int nb  = request2size_0041DBF8(bytes);  /* padded request size; */

  MALLOC_LOCK_0041DBF8;

  /* Check for exact match in a bin */

  if (is_small_request_0041DBF8(nb))  /* Faster version for small requests */
  {
    idx = smallbin_index_0041DBF8(nb); 

    /* No traversal or size check necessary for small bins.  */

    q = bin_at_0041DBF8(idx);
    victim = last_0041DBF8(q);

    if (victim != q)
    {
      victim_size = chunksize_0041DBF8(victim);
      unlink_0041DBF8(victim, bck, fwd);
      set_inuse_bit_at_offset_0041DBF8(victim, victim_size);
      MALLOC_UNLOCK_0041DBF8;
      return chunk2mem_0041DBF8(victim);
    }

    idx += 2; /* Set for bin scan below. We've already scanned 2 bins. */

  }
  else
  {
    idx = bin_index_0041DBF8(nb);
    bin = bin_at_0041DBF8(idx);

    for (victim = last_0041DBF8(bin); victim != bin; victim = victim->bk)
    {
      victim_size = chunksize_0041DBF8(victim);
      remainder_size = long_sub_size_t_0041DBF8(victim_size, nb);
      
      if (remainder_size >= (long)MINSIZE_0041DBF8) /* too big */
      {
        --idx; /* adjust to rescan below after checking last remainder */
        break;   
      }

      else if (remainder_size >= 0) /* exact fit */
      {
        unlink_0041DBF8(victim, bck, fwd);
        set_inuse_bit_at_offset_0041DBF8(victim, victim_size);
	MALLOC_UNLOCK_0041DBF8;
        return chunk2mem_0041DBF8(victim);
      }
    }

    ++idx; 

  }

  /* Try to use the last split-off remainder */

  if ( (victim = last_remainder_0041DBF8->fd) != last_remainder_0041DBF8)
  {
    victim_size = chunksize_0041DBF8(victim);
    remainder_size = long_sub_size_t_0041DBF8(victim_size, nb);

    if (remainder_size >= (long)MINSIZE_0041DBF8) /* re-split */
    {
      remainder = chunk_at_offset(victim, nb);
      set_head_0041DBF8(victim, nb | PREV_INUSE_0041DBF8);
      link_last_remainder_0041DBF8(remainder);
      set_head_0041DBF8(remainder, remainder_size | PREV_INUSE_0041DBF8);
      set_foot_0041DBF8(remainder, remainder_size);
      MALLOC_UNLOCK_0041DBF8;
      return chunk2mem_0041DBF8(victim);
    }

    clear_last_remainder_0041DBF8;

    if (remainder_size >= 0)  /* exhaust */
    {
      set_inuse_bit_at_offset_0041DBF8(victim, victim_size);
      MALLOC_UNLOCK_0041DBF8;
      return chunk2mem_0041DBF8(victim);
    }

    /* Else place in bin */

    frontlink_0041DBF8(victim, victim_size, remainder_index, bck, fwd);
  }

  /* 
     If there are any possibly nonempty big-enough blocks, 
     search for best fitting chunk by scanning bins in blockwidth units.
  */

  if ( (block = idx2binblock_0041DBF8(idx)) <= binblocks_0041DBF8)  
  {

    /* Get to the first marked block */

    if ( (block & binblocks_0041DBF8) == 0) 
    {
      /* force to an even block boundary */
      idx = (idx & ~(BINBLOCKWIDTH_0041DBF8 - 1)) + BINBLOCKWIDTH_0041DBF8;
      block <<= 1;
      while ((block & binblocks_0041DBF8) == 0)
      {
        idx += BINBLOCKWIDTH_0041DBF8;
        block <<= 1;
      }
    }
      
    /* For each possibly nonempty block ... */
    for (;;)  
    {
      startidx = idx;          /* (track incomplete blocks) */
      q = bin = bin_at_0041DBF8(idx);

      /* For each bin in this block ... */
      do
      {
        /* Find and use first big enough chunk ... */

        for (victim = last_0041DBF8(bin); victim != bin; victim = victim->bk)
        {
          victim_size = chunksize_0041DBF8(victim);
          remainder_size = long_sub_size_t_0041DBF8(victim_size, nb);

          if (remainder_size >= (long)MINSIZE_0041DBF8) /* split */
          {
            remainder = chunk_at_offset(victim, nb);
            set_head_0041DBF8(victim, nb | PREV_INUSE_0041DBF8);
            unlink_0041DBF8(victim, bck, fwd);
            link_last_remainder_0041DBF8(remainder);
            set_head_0041DBF8(remainder, remainder_size | PREV_INUSE_0041DBF8);
            set_foot_0041DBF8(remainder, remainder_size);
	    MALLOC_UNLOCK_0041DBF8;
            return chunk2mem_0041DBF8(victim);
          }

          else if (remainder_size >= 0)  /* take */
          {
            set_inuse_bit_at_offset_0041DBF8(victim, victim_size);
            unlink_0041DBF8(victim, bck, fwd);
	    MALLOC_UNLOCK_0041DBF8;
            return chunk2mem_0041DBF8(victim);
          }

        }

       bin = next_bin_0041DBF8(bin);

       if (idx < MAX_SMALLBIN_0041DBF8)
         {
           bin = next_bin_0041DBF8(bin);
           ++idx;
         }
      } while ((++idx & (BINBLOCKWIDTH_0041DBF8 - 1)) != 0);

      /* Clear out the block bit. */

      do   /* Possibly backtrack to try to clear a partial block */
      {
        if ((startidx & (BINBLOCKWIDTH_0041DBF8 - 1)) == 0)
        {
          binblocks_0041DBF8 &= ~block;
          break;
        }
        --startidx;
       q = prev_bin_0041DBF8(q);
      } while (first_0041DBF8(q) == q);

      /* Get to the next possibly nonempty block */

      if ( (block <<= 1) <= binblocks_0041DBF8 && (block != 0) ) 
      {
        while ((block & binblocks_0041DBF8) == 0)
        {
          idx += BINBLOCKWIDTH_0041DBF8;
          block <<= 1;
        }
      }
      else
        break;
    }
  }


  /* Try to use top chunk */

  /* Require that there be a remainder, ensuring top always exists  */
  remainder_size = long_sub_size_t_0041DBF8(chunksize_0041DBF8(top_0041DBF8), nb);
  if (chunksize_0041DBF8(top_0041DBF8) < nb || remainder_size < (long)MINSIZE_0041DBF8)
  {
    /* Try to extend */
    g41DBF8_extend_top(reent_ptr, nb);
    remainder_size = long_sub_size_t_0041DBF8(chunksize_0041DBF8(top_0041DBF8), nb);
    if (chunksize_0041DBF8(top_0041DBF8) < nb || remainder_size < (long)MINSIZE_0041DBF8)
    {
      MALLOC_UNLOCK_0041DBF8;
      return 0; /* propagate failure */
    }
  }

  victim = top_0041DBF8;
  set_head_0041DBF8(victim, nb | PREV_INUSE_0041DBF8);
  top_0041DBF8 = chunk_at_offset(victim, nb);
  set_head_0041DBF8(top_0041DBF8, remainder_size | PREV_INUSE_0041DBF8);
  MALLOC_UNLOCK_0041DBF8;
  return chunk2mem_0041DBF8(victim);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041E340);
#ifdef SKIP_ASM
extern void func_00418CA8_lock(int) __asm__("func_00418CA8");
extern void func_00418D08_unlock(int) __asm__("func_00418D08");
extern int func_0041E638_trim(int, unsigned) __asm__("func_0041E638");
void func_0041E340_(int ptr, void *mem) __asm__("func_0041E340");
void func_0041E340_(int ptr, void *mem) {
    mchunkptr p;
    unsigned hd, sz;
    int idx;
    mchunkptr next;
    unsigned nextsz, prevsz;
    mchunkptr bck, fwd;
    int islr;
    mchunkptr lr;
    if (mem == 0)
        return;
    func_00418CA8_lock(ptr);
    p = (mchunkptr)((char *)mem - 8);
    hd = p->size;
    sz = hd & ~1u;
    next = chunk_at_offset(p, sz);
    nextsz = chunksize(next);
    if (next == top) {
        sz += nextsz;
        if (!(hd & 1)) {
            prevsz = p->prev_size;
            p = chunk_at_offset(p, -(int)prevsz);
            sz += prevsz;
            unlink(p, bck, fwd);
        }
        set_head(p, sz | 1);
        top = p;
        if ((unsigned long)sz >= D_004550F0)
            func_0041E638_trim(ptr, D_004550F8);
        func_00418D08_unlock(ptr);
        return;
    }
    set_head(next, nextsz);
    islr = 0;
    if (!(hd & 1)) {
        prevsz = p->prev_size;
        p = chunk_at_offset(p, -(int)prevsz);
        sz += prevsz;
        if (p->fd == last_remainder)
            islr = 1;
        else
            unlink(p, bck, fwd);
    }
    if (!inuse_bit_at_offset(next, nextsz)) {
        sz += nextsz;
        if (!islr && next->fd == (lr = last_remainder)) {
            islr = 1;
            link_last_remainder(p);
        } else
            unlink(next, bck, fwd);
    }
    set_head(p, sz | 1);
    set_foot(p, sz);
    if (!islr)
        frontlink(p, sz, idx, bck, fwd);
    func_00418D08_unlock(ptr);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041E638);
#ifdef SKIP_ASM
/* newlib stdlib/mallocr.c malloc_trim (_malloc_trim_r): "A version of malloc/free/realloc written by Doug Lea and
 * released to the public domain." Uses the unit's mallocr macros (top, chunksize, set_head) and declarations from
 * func_0041D990 / func_0041E340, and its libgcc 64-bit helpers from func_0041A840. */
/* newlib's struct mallinfo; sbrked_mem is current_mallinfo.arena (D_00455118, declared int earlier in the unit) */
struct mallinfo_E638 {
  int arena;    /* total space allocated from system */
  int ordblks;  /* number of non-inuse chunks */
  int smblks;   /* unused -- always zero */
  int hblks;    /* number of mmapped regions */
  int hblkhd;   /* total space in mmapped regions */
  int usmblks;  /* unused -- always zero */
  int fsmblks;  /* unused -- always zero */
  int uordblks; /* total allocated space */
  int fordblks; /* total non-inuse space */
  int keepcost; /* top-most, releasable (via malloc_trim) space */
};
extern struct mallinfo_E638 current_mallinfo_E638 __asm__("D_00455118");
#define sbrked_mem_E638  (current_mallinfo_E638.arena)
int func_0041E638_(int reent_ptr, unsigned int pad) __asm__("func_0041E638");
int func_0041E638_(int reent_ptr, unsigned int pad)
{
  long  top_size;        /* Amount of top-most memory */
  long  extra;           /* Amount to release */
  char* current_brk;     /* address returned by pre-check sbrk call */
  char* new_brk;         /* address returned by negative sbrk call */

  unsigned long pagesz = 0x1000;

  func_00418CA8_lock(reent_ptr);

  top_size = chunksize(top);
  extra = func_004114D0_mul(func_00411538_div(top_size - pad - 16 + (pagesz-1), pagesz) - 1, pagesz);

  if (extra < (long)pagesz)  /* Not enough memory to release */
  {
    func_00418D08_unlock(reent_ptr);
    return 0;
  }

  else
  {
    /* Test to make sure no one else called sbrk */
    current_brk = (char*)(func_0041FD70_sbrk (reent_ptr, 0));
    if (current_brk != (char*)(top) + top_size)
    {
      func_00418D08_unlock(reent_ptr);
      return 0;     /* Apparently we don't own memory; must fail */
    }

    else
    {
      new_brk = (char*)(func_0041FD70_sbrk (reent_ptr, -extra));

      if (new_brk == (char*)(-1)) /* sbrk failed? */
      {
        /* Try to figure out what we have */
        current_brk = (char*)(func_0041FD70_sbrk (reent_ptr, 0));
        top_size = current_brk - (char*)top;
        if (top_size >= (long)16) /* if not, we are very very dead! */
        {
          sbrked_mem_E638 = current_brk - D_00455100;
          set_head(top, top_size | 1);
        }
        func_00418D08_unlock(reent_ptr);
        return 0;
      }

      else
      {
        /* Success. Adjust top accordingly. */
        set_head(top, (top_size - extra) | 1);
        sbrked_mem_E638 -= extra;
        func_00418D08_unlock(reent_ptr);
        return 1;
      }
    }
  }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041E7A8);
#ifdef SKIP_ASM
long func_0041E7A8_(int *ptr, char *nptr, char **endptr, int base) __asm__("func_0041E7A8");
long func_0041E7A8_(int *ptr, char *nptr, char **endptr, int base) {
    char *s = nptr;
    unsigned long acc, cutoff;
    int c, neg = 0, any, cutlim;
    do {
        c = *s++;
    } while (XCT(c) & 8);
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+')
        c = *s++;
    if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = neg ? -(unsigned long)0x8000000000000000L : 0x7FFFFFFFFFFFFFFFL;
    cutlim = func_0042DAD8_mod(cutoff, base);
    cutoff = func_00411538_div(cutoff, base);
    for (acc = 0, any = 0;; c = *s++) {
        if (XCT(c) & 4)
            c -= '0';
        else if (XCT(c) & 3)
            c -= (XCT(c) & 1) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc = func_004114D0_mul(acc, base);
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? 0x8000000000000000L : 0x7FFFFFFFFFFFFFFFL;
        *ptr = 34;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = any ? s - 1 : nptr;
    return acc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041E9E0);
#ifdef SKIP_ASM
extern int D_00454944;
extern long func_0041E7A8(int, int, int, int);
long func_0041E9E0(int s, int end, int base) {
    return func_0041E7A8(D_00454944, s, end, base);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041EA18);
#ifdef SKIP_ASM
extern char D_00499C81[];
int func_0041EA18(int a) {
    return (D_00499C81[a] & 1) ? a + 0x20 : a;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041EA38);
#ifdef SKIP_ASM
extern char D_0049A7D0b[] __asm__("D_0049A7D0");
extern char D_0049A7C0[], D_0049A7D0[], D_0049A7D8[], D_0049A7E0[], D_0049A7E8[];
extern char D_00455140[], D_00455148[], D_00455150[];
extern int func_004165A8_cmp(char *, char *) __asm__("func_004165A8");
extern char *strcpy_(char *, char *) __asm__("strcpy");
extern unsigned strlen_(char *) __asm__("strlen");
char *func_0041EA38_(int *p, int category, char *locale) __asm__("func_0041EA38");
char *func_0041EA38_(int *p, int category, char *locale) {
    if (locale) {
        if (category != 2) {
            if (func_004165A8_cmp(locale, D_0049A7D0) && func_004165A8_cmp(locale, D_0049A7C0))
                return 0;
            if (category == 0) {
                strcpy_(D_00455150, D_00455148);
                strcpy_(D_00455148, locale);
                *(int *)D_00455140 = 1;
            }
        } else {
            if (func_004165A8_cmp(locale, D_0049A7D0) && func_004165A8_cmp(locale, D_0049A7C0) && func_004165A8_cmp(locale, D_0049A7D0) &&
                func_004165A8_cmp(locale, D_0049A7D8) && func_004165A8_cmp(locale, D_0049A7E0) &&
                func_004165A8_cmp(locale, D_0049A7E8))
                return 0;
            strcpy_(D_00455150, D_00455148);
            strcpy_(D_00455148, locale);
            if (func_004165A8_cmp(locale, D_0049A7D8) == 0)
                *(int *)D_00455140 = 8;
            else if (strlen_(locale) >= 2)
                *(int *)D_00455140 = 2;
            else
                *(int *)D_00455140 = 1;
        }
        p[0xD] = (int)locale;
        p[0xC] = category;
        return category == 2 ? D_00455150 : D_0049A7D0b;
    }
    return category == 2 ? D_00455148 : D_0049A7D0b;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041EC08);
#ifdef SKIP_ASM
extern char D_0049A790[];
char *func_0041EC08(void) {
    return D_0049A790;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041EC18);
#ifdef SKIP_ASM
extern int D_00454944;
extern void func_0041EA38(int, int, int);
void func_0041EC18(int a, int b) {
    func_0041EA38(D_00454944, a, b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041EC48);
#ifdef SKIP_ASM
extern int D_00454944;
void func_0041EC48(void) {
    ((char *(*)(int))func_0041EC08)(D_00454944);
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_0041EC70);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041ED60);
#ifdef SKIP_ASM
/* newlib libm floor (fdlibm s_floor.c). The EE has no double hardware: the double travels in one 64-bit GPR and
 * the arithmetic goes through the soft-float helpers (dpadd, dpcmp).
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* newlib libm's word-access helpers (fdlibm.h), little-endian layout, verbatim: the do-while(0) is part of the
   library macro */
typedef union
{
  double value;
  struct
  {
    unsigned int lsw;
    unsigned int msw;
  } parts;
} ieee_double_shape_type_ED60;
#define EXTRACT_WORDS_ED60(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type_ED60 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define INSERT_WORDS_ED60(d,ix0,ix1)				\
do {								\
  ieee_double_shape_type_ED60 iw_u;				\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

static const double huge_ED60 = 1.0e300;

double func_0041ED60(double x)
{
	int i0,i1,j0;
	unsigned int i,j;
	EXTRACT_WORDS_ED60(i0,i1,x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;
	if(j0<20) {
	    if(j0<0) { 	/* raise inexact if x != 0 */
		if(huge_ED60+x>0.0) {/* return 0*sign(x) if |x|<1 */
		    if(i0>=0) {i0=i1=0;}
		    else if(((i0&0x7fffffff)|i1)!=0)
			{ i0=0xbff00000;i1=0;}
		}
	    } else {
		i = (0x000fffff)>>j0;
		if(((i0&i)|i1)==0) return x; /* x is integral */
		if(huge_ED60+x>0.0) {	/* raise inexact flag */
		    if(i0<0) i0 += (0x00100000)>>j0;
		    i0 &= (~i); i1=0;
		}
	    }
	} else if (j0>51) {
	    if(j0==0x400) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	} else {
	    i = ((unsigned int)(0xffffffff))>>(j0-20);
	    if((i1&i)==0) return x;	/* x is integral */
	    if(huge_ED60+x>0.0) { 		/* raise inexact flag */
		if(i0<0) {
		    if(j0==20) i0+=1;
		    else {
			j = i1+(1<<(52-j0));
			if(j<i1) i0 +=1 ; 	/* got a carry */
			i1=j;
		    }
		}
		i1 &= (~i);
	    }
	}
	INSERT_WORDS_ED60(x,i0,i1);
	return x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041EF50);
#ifdef SKIP_ASM
// FAKEMATCH: `while((unsigned int)n-- > 0)` for fdlibm's `while(n--)` (the same test for every n); the plain
// form is strength-reduced to `--n != -1`; plain form 97.11%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
/* newlib libm __ieee754_fmod (fdlibm e_fmod.c). The EE has no double hardware: the double travels in one 64-bit
 * GPR and the arithmetic goes through the soft-float helpers.
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* newlib libm's word-access helpers (fdlibm.h), little-endian layout, verbatim: the do-while(0) is part of the
   library macro */
typedef union
{
  double value;
  struct
  {
    unsigned int lsw;
    unsigned int msw;
  } parts;
} ieee_double_shape_type_EF50;
#define EXTRACT_WORDS_EF50(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type_EF50 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define INSERT_WORDS_EF50(d,ix0,ix1)				\
do {								\
  ieee_double_shape_type_EF50 iw_u;				\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

static const double one_EF50 = 1.0;
/* fdlibm's `static const double Zero[] = {0.0, -0.0,};` (e_fmod.c), indexed by the code: retail's copy is D_0049A808 */
extern const double Zero_EF50[] __asm__("D_0049A808");

double func_0041EF50(double x, double y)
{
	int n,hx,hy,hz,ix,iy,sx,i;
	unsigned int lx,ly,lz;

	EXTRACT_WORDS_EF50(hx,lx,x);
	EXTRACT_WORDS_EF50(hy,ly,y);
	sx = hx&0x80000000;		/* sign of x */
	hx ^=sx;		/* |x| */
	hy &= 0x7fffffff;	/* |y| */

    /* purge off exception values */
	if((hy|ly)==0||(hx>=0x7ff00000)||	/* y=0,or x not finite */
	  ((hy|((ly|-ly)>>31))>0x7ff00000))	/* or y is NaN */
	    return (x*y)/(x*y);
	if(hx<=hy) {
	    if((hx<hy)||(lx<ly)) return x;	/* |x|<|y| return x */
	    if(lx==ly) 
		return Zero_EF50[(unsigned int)sx>>31];	/* |x|=|y| return x*0*/
	}

    /* determine ix = ilogb(x) */
	if(hx<0x00100000) {	/* subnormal x */
	    if(hx==0) {
		for (ix = -1043, i=lx; i>0; i<<=1) ix -=1;
	    } else {
		for (ix = -1022,i=(hx<<11); i>0; i<<=1) ix -=1;
	    }
	} else ix = (hx>>20)-1023;

    /* determine iy = ilogb(y) */
	if(hy<0x00100000) {	/* subnormal y */
	    if(hy==0) {
		for (iy = -1043, i=ly; i>0; i<<=1) iy -=1;
	    } else {
		for (iy = -1022,i=(hy<<11); i>0; i<<=1) iy -=1;
	    }
	} else iy = (hy>>20)-1023;

    /* set up {hx,lx}, {hy,ly} and align y to x */
	if(ix >= -1022) 
	    hx = 0x00100000|(0x000fffff&hx);
	else {		/* subnormal x, shift x to normal */
	    n = -1022-ix;
	    if(n<=31) {
	        hx = (hx<<n)|(lx>>(32-n));
	        lx <<= n;
	    } else {
		hx = lx<<(n-32);
		lx = 0;
	    }
	}
	if(iy >= -1022) 
	    hy = 0x00100000|(0x000fffff&hy);
	else {		/* subnormal y, shift y to normal */
	    n = -1022-iy;
	    if(n<=31) {
	        hy = (hy<<n)|(ly>>(32-n));
	        ly <<= n;
	    } else {
		hy = ly<<(n-32);
		ly = 0;
	    }
	}

    /* fix point fmod */
	n = ix - iy;
	while((unsigned int)n-- > 0) {
	    hz=hx-hy;lz=lx-ly; if(lx<ly) hz -= 1;
	    if(hz<0){hx = hx+hx+(lx>>31); lx = lx+lx;}
	    else {
	    	if((hz|lz)==0) 		/* return sign(x)*0 */
		    return Zero_EF50[(unsigned int)sx>>31];
	    	hx = hz+hz+(lz>>31); lx = lz+lz;
	    }
	}
	hz=hx-hy;lz=lx-ly; if(lx<ly) hz -= 1;
	if(hz>=0) {hx=hz;lx=lz;}

    /* convert back to floating value and restore the sign */
	if((hx|lx)==0) 			/* return sign(x)*0 */
	    return Zero_EF50[(unsigned int)sx>>31];	
	while(hx<0x00100000) {		/* normalize x */
	    hx = hx+hx+(lx>>31); lx = lx+lx;
	    iy -= 1;
	}
	if(iy>= -1022) {	/* normalize output */
	    hx = ((hx-0x00100000)|((iy+1023)<<20));
	    INSERT_WORDS_EF50(x,hx|sx,lx);
	} else {		/* subnormal output */
	    n = -1022 - iy;
	    if(n<=20) {
		lx = (lx>>n)|((unsigned int)hx<<(32-n));
		hx >>= n;
	    } else if (n<=31) {
		lx = (hx<<(32-n))|(lx>>n); hx = sx;
	    } else {
		lx = hx>>(n-32); hx = sx;
	    }
	    INSERT_WORDS_EF50(x,hx|sx,lx);
	    x *= one_EF50;		/* create necessary signal */
	}
	return x;		/* exact output */
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041F3A8);
#ifdef SKIP_ASM
/* newlib 1.9 libm/math/e_log.c (__ieee754_log) and the fdlibm.h word macros, verbatim:
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */
typedef union {
    double value;
    struct {
        unsigned int lsw;
        unsigned int msw;
    } parts;
} ieee_double_shape_type_0041F3A8;
#define EXTRACT_WORDS_0041F3A8(ix0,ix1,d)			\
do {								\
  ieee_double_shape_type_0041F3A8 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define GET_HIGH_WORD_0041F3A8(i,d)				\
do {								\
  ieee_double_shape_type_0041F3A8 gh_u;				\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)
#define SET_HIGH_WORD_0041F3A8(d,v)				\
do {								\
  ieee_double_shape_type_0041F3A8 sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

static const double
ln2_hi_0041F3A8  =  6.93147180369123816490e-01,	/* 3fe62e42 fee00000 */
ln2_lo_0041F3A8  =  1.90821492927058770002e-10,	/* 3dea39ef 35793c76 */
two54_0041F3A8   =  1.80143985094819840000e+16,  /* 43500000 00000000 */
Lg1_0041F3A8 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
Lg2_0041F3A8 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
Lg3_0041F3A8 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
Lg4_0041F3A8 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
Lg5_0041F3A8 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
Lg6_0041F3A8 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
Lg7_0041F3A8 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

static const double zero_0041F3A8   =  0.0;

double func_0041F3A8(double x)
{
	double hfsq,f,s,z,R,w,t1,t2,dk;
	int k,hx,i,j;
	unsigned int lx;

	EXTRACT_WORDS_0041F3A8(hx,lx,x);

	k=0;
	if (hx < 0x00100000) {			/* x < 2**-1022  */
	    if (((hx&0x7fffffff)|lx)==0) 
		return -two54_0041F3A8/zero_0041F3A8;		/* log(+-0)=-inf */
	    if (hx<0) return (x-x)/zero_0041F3A8;	/* log(-#) = NaN */
	    k -= 54; x *= two54_0041F3A8; /* subnormal number, scale up x */
	    GET_HIGH_WORD_0041F3A8(hx,x);
	} 
	if (hx >= 0x7ff00000) return x+x;
	k += (hx>>20)-1023;
	hx &= 0x000fffff;
	i = (hx+0x95f64)&0x100000;
	SET_HIGH_WORD_0041F3A8(x,hx|(i^0x3ff00000));	/* normalize x or x/2 */
	k += (i>>20);
	f = x-1.0;
	if((0x000fffff&(2+hx))<3) {	/* |f| < 2**-20 */
          if(f==zero_0041F3A8) { if(k==0) return zero_0041F3A8;  else {dk=(double)k;
                               return dk*ln2_hi_0041F3A8+dk*ln2_lo_0041F3A8;}}
	    R = f*f*(0.5-0.33333333333333333*f);
	    if(k==0) return f-R; else {dk=(double)k;
	    	     return dk*ln2_hi_0041F3A8-((R-dk*ln2_lo_0041F3A8)-f);}
	}
 	s = f/(2.0+f); 
	dk = (double)k;
	z = s*s;
	i = hx-0x6147a;
	w = z*z;
	j = 0x6b851-hx;
	t1= w*(Lg2_0041F3A8+w*(Lg4_0041F3A8+w*Lg6_0041F3A8)); 
	t2= z*(Lg1_0041F3A8+w*(Lg3_0041F3A8+w*(Lg5_0041F3A8+w*Lg7_0041F3A8))); 
	i |= j;
	R = t2+t1;
	if(i>0) {
	    hfsq=0.5*f*f;
	    if(k==0) return f-(hfsq-s*(hfsq+R)); else
		     return dk*ln2_hi_0041F3A8-((hfsq-(s*(hfsq+R)+dk*ln2_lo_0041F3A8))-f);
	} else {
	    if(k==0) return f-s*(f-R); else
		     return dk*ln2_hi_0041F3A8-((s*(f-R)-dk*ln2_lo_0041F3A8)-f);
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041F9D8);
#ifdef SKIP_ASM
/* newlib libm __ieee754_log10 (fdlibm e_log10.c). The EE has no double hardware: the double travels in one 64-bit
 * GPR and the arithmetic goes through the soft-float helpers.
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* newlib libm's word-access helpers (fdlibm.h), little-endian layout, verbatim: the do-while(0) is part of the
   library macro */
typedef union
{
  double value;
  struct
  {
    unsigned int lsw;
    unsigned int msw;
  } parts;
} ieee_double_shape_type_F9D8;
#define EXTRACT_WORDS_F9D8(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type_F9D8 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define GET_HIGH_WORD_F9D8(i,d)					\
do {								\
  ieee_double_shape_type_F9D8 gh_u;				\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)
#define SET_HIGH_WORD_F9D8(d,v)					\
do {								\
  ieee_double_shape_type_F9D8 sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

static const double
two54_F9D8      =  1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
ivln10_F9D8     =  4.34294481903251816668e-01, /* 0x3FDBCB7B, 0x1526E50E */
log10_2hi_F9D8  =  3.01029995663611771306e-01, /* 0x3FD34413, 0x509F6000 */
log10_2lo_F9D8  =  3.69423907715893078616e-13; /* 0x3D59FEF3, 0x11F12B36 */

static const double zero_F9D8   =  0.0;

extern double __ieee754_log_F9D8(double) __asm__("func_0041F3A8");

double func_0041F9D8(double x)
{
	double y,z;
	int i,k,hx;
	unsigned int lx;

	EXTRACT_WORDS_F9D8(hx,lx,x);

        k=0;
        if (hx < 0x00100000) {                  /* x < 2**-1022  */
            if (((hx&0x7fffffff)|lx)==0)
                return -two54_F9D8/zero_F9D8;             /* log(+-0)=-inf */
            if (hx<0) return (x-x)/zero_F9D8;        /* log(-#) = NaN */
            k -= 54; x *= two54_F9D8; /* subnormal number, scale up x */
	    GET_HIGH_WORD_F9D8(hx,x);
        }
	if (hx >= 0x7ff00000) return x+x;
	k += (hx>>20)-1023;
	i  = ((unsigned int)k&0x80000000)>>31;
        hx = (hx&0x000fffff)|((0x3ff-i)<<20);
        y  = (double)(k+i);
	SET_HIGH_WORD_F9D8(x,hx);
	z  = y*log10_2lo_F9D8 + ivln10_F9D8*__ieee754_log_F9D8(x);
	return  z+y*log10_2hi_F9D8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FB98);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424488(int);
int func_0041FB98(int err, int a) {
    int r;
    D_004A5BB8 = 0;
    r = func_00424488(a);
    if (r == -1 && D_004A5BB8)
        *(int *)err = D_004A5BB8;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FBF0);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424558(int, int);
int func_0041FBF0(int *err, int a, int b) {
    int r;
    D_004A5BB8 = 0;
    r = func_00424558(a, b);
    if (r == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FC50);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424498(int, int, int);
long func_0041FC50(int *err, int a, int b, int c) {
    int r;
    D_004A5BB8 = 0;
    r = func_00424498(a, b, c);
    if (r == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FCB0);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424460(int, int, int);
int func_0041FCB0(int *err, int a, int b, int c) {
    int r;
    D_004A5BB8 = 0;
    r = func_00424460(a, b, c);
    if (r == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FD10);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_004243E8(int, int, int);
long func_0041FD10(int *err, int a, int b, int c) {
    int r;
    D_004A5BB8 = 0;
    r = func_004243E8(a, b, c);
    if (r == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FD70);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern unsigned func_004244A0(int);
unsigned func_0041FD70(int *err, int a) {
    unsigned r;
    D_004A5BB8 = 0;
    r = func_004244A0(a);
    if (r == 0xFFFFFFFF && D_004A5BB8)
        *err = D_004A5BB8;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FDD0);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424368(int, int, int);
long func_0041FDD0(int *err, int a, int b, int c) {
    int r;
    D_004A5BB8 = 0;
    r = func_00424368(a, b, c);
    if (r == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FE30);
#ifdef SKIP_ASM
/* newlib stdio/fflush.c (fflush), with CHECK_INIT from stdio/local.h and FILE from sys/reent.h:
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
struct _reent_FE30 {
  int _errno;
  struct __sFILE_FE30 *_stdin, *_stdout, *_stderr;
  int  _inc;
  char _emergency[25];
  int _current_category;
  const char *_current_locale;
  int __sdidinit;
};
struct __sbuf_FE30 {
	unsigned char *_base;
	int	_size;
};
struct __sFILE_FE30 {
  unsigned char *_p;	/* current position in (some) buffer */
  int	_r;		/* read space left for getc() */
  int	_w;		/* write space left for putc() */
  short	_flags;		/* flags, below; this FILE is free if 0 */
  short	_file;		/* fileno, if Unix descriptor, else -1 */
  struct __sbuf_FE30 _bf;	/* the buffer (at least 1 byte, if !NULL) */
  int	_lbfsize;	/* 0 or -_bf._size, for inline putc */
  void *_cookie;	/* cookie passed to io functions */
  int	(*_read)(void *_cookie, char *_buf, int _n);
  int	(*_write)(void *_cookie, const char *_buf, int _n);
  long	(*_seek)(void *_cookie, long _offset, int _whence);
  int	(*_close)(void *_cookie);
  struct __sbuf_FE30 _ub;	/* ungetc buffer */
  unsigned char *_up;	/* saved _p when _p is doing ungetc data */
  int	_ur;		/* saved _r when _r is counting ungetc data */
  unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
  unsigned char _nbuf[1];	/* guarantee a getc() buffer */
  struct __sbuf_FE30 _lb;	/* buffer for fgetline() */
  int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
  int	_offset;	/* current lseek offset */
  struct _reent_FE30 *_data;
};
#define __SLBF_FE30	0x0001
#define __SNBF_FE30	0x0002
#define __SWR_FE30	0x0008
#define __SERR_FE30	0x0040
extern struct _reent_FE30 *_impure_ptr_FE30 __asm__("D_00454944");
#define _REENT_FE30 _impure_ptr_FE30
extern void __sinit_FE30(struct _reent_FE30 *) __asm__("func_004200F0");
extern int _fwalk_FE30(struct _reent_FE30 *, int (*)(struct __sFILE_FE30 *)) __asm__("func_00417418");
/* newlib's CHECK_INIT (stdio/local.h), verbatim: the do-while(0) is part of the library macro */
#define CHECK_INIT_FE30(fp) \
  do					\
    {					\
      if ((fp)->_data == 0)		\
	(fp)->_data = _REENT_FE30;	\
      if (!(fp)->_data->__sdidinit)	\
	__sinit_FE30 ((fp)->_data);	\
    }					\
  while (0)

int fflush_FE30(struct __sFILE_FE30 *fp) __asm__("func_0041FE30");
int
fflush_FE30 (register struct __sFILE_FE30 * fp)
{
  register unsigned char *p;
  register int n, t;

  if (fp == 0)
    return _fwalk_FE30 (_REENT_FE30, fflush_FE30);

  CHECK_INIT_FE30 (fp);

  t = fp->_flags;
  if ((t & __SWR_FE30) == 0 || (p = fp->_bf._base) == 0)
    return 0;
  n = fp->_p - p;		/* write this much */

  /*
   * Set these immediately to avoid problems with longjmp
   * and to allow exchange buffering (via setvbuf) in user
   * write function.
   */
  fp->_p = p;
  fp->_w = t & (__SLBF_FE30 | __SNBF_FE30) ? 0 : fp->_bf._size;

  while (n > 0)
    {
      t = (*fp->_write) (fp->_cookie, (char *) p, n);
      if (t <= 0)
	{
	  fp->_flags |= __SERR_FE30;
	  return -1;
	}
      p += t;
      n -= t;
    }
  return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FF40);
#ifdef SKIP_ASM
struct F41FF40 {
    int p, r, w;
    short flags, file;
    int base, size_, lbf;
    int cookie;
    void *rd, *wr, *sk, *cl;
    int pad[0x54 / 4 - 12];
    int ent;
};
extern long func_00417948(), func_004179B0(), func_00417A30();
void func_0041FF40(struct F41FF40 *f, short flags, short file, int ent) {
    f->p = 0;
    f->r = 0;
    f->w = 0;
    f->flags = flags;
    f->file = file;
    f->base = 0;
    f->lbf = 0;
    f->cookie = (int)f;
    f->rd = func_00417948;
    f->wr = func_004179B0;
    f->sk = func_00417A30;
    f->cl = (void *)func_00417A98;
    f->ent = ent;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FF98);
#ifdef SKIP_ASM
extern void *func_00416210(void *, int, int);
int *func_0041FF98(int a, int cnt) {
    int sz = cnt * 0x58;
    int *p = (int *)func_0041DBF8(a, sz + 0xC);
    if (!p)
        return 0;
    p[1] = cnt;
    p[0] = 0;
    p[2] = (int)(p + 3);
    func_00416210(p + 3, 0, sz);
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420010);
#ifdef SKIP_ASM
/* newlib stdio/findfp.c (__sfp), FILE and struct _glue from sys/reent.h:
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
struct __sbuf_0010 {
	unsigned char *_base;
	int	_size;
};
struct __sFILE_0010 {
  unsigned char *_p;	/* current position in (some) buffer */
  int	_r;		/* read space left for getc() */
  int	_w;		/* write space left for putc() */
  short	_flags;		/* flags, below; this FILE is free if 0 */
  short	_file;		/* fileno, if Unix descriptor, else -1 */
  struct __sbuf_0010 _bf;	/* the buffer (at least 1 byte, if !NULL) */
  int	_lbfsize;	/* 0 or -_bf._size, for inline putc */
  void *_cookie;	/* cookie passed to io functions */
  int	(*_read)(void *_cookie, char *_buf, int _n);
  int	(*_write)(void *_cookie, const char *_buf, int _n);
  long	(*_seek)(void *_cookie, long _offset, int _whence);
  int	(*_close)(void *_cookie);
  struct __sbuf_0010 _ub;	/* ungetc buffer */
  unsigned char *_up;	/* saved _p when _p is doing ungetc data */
  int	_ur;		/* saved _r when _r is counting ungetc data */
  unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
  unsigned char _nbuf[1];	/* guarantee a getc() buffer */
  struct __sbuf_0010 _lb;	/* buffer for fgetline() */
  int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
  int	_offset;	/* current lseek offset */
  struct _reent_0010 *_data;
};
struct _glue_0010
{
  struct _glue_0010 *_next;
  int _niobs;
  struct __sFILE_0010 *_iobs;
};
/* newlib 1.9's struct _reent up to __sglue (0x1d8) */
struct _reent_0010 {
  int _errno;
  struct __sFILE_0010 *_stdin, *_stdout, *_stderr;
  int  _inc;
  char _emergency[25];
  int _current_category;
  const char *_current_locale;
  int __sdidinit;
  char _pad_0x3c[0x1d8 - 0x3c];
  struct _glue_0010 __sglue;
};
#define NDYNAMIC_0010 4
#define ENOMEM_0010 12
extern void __sinit_0010(struct _reent_0010 *) __asm__("func_004200F0");
extern struct _glue_0010 *__sfmoreglue_0010(struct _reent_0010 *, int) __asm__("func_0041FF98");

/* the unit declares func_00420010(int) for its caller func_00416EB0 */
struct __sFILE_0010 *__sfp_0010(struct _reent_0010 *d) __asm__("func_00420010");
struct __sFILE_0010 *
__sfp_0010 (struct _reent_0010 *d)
{
  struct __sFILE_0010 *fp;
  int n;
  struct _glue_0010 *g;

  if (!d->__sdidinit)
    __sinit_0010 (d);
  for (g = &d->__sglue;; g = g->_next)
    {
      for (fp = g->_iobs, n = g->_niobs; --n >= 0; fp++)
	if (fp->_flags == 0)
	  goto found;
      if (g->_next == 0 &&
	  (g->_next = __sfmoreglue_0010 (d, NDYNAMIC_0010)) == 0)
	break;
    }
  d->_errno = ENOMEM_0010;
  return 0;

found:
  fp->_flags = 1;		/* reserve this slot; caller sets real flags */
  fp->_p = 0;		/* no current pointer */
  fp->_w = 0;			/* nothing to read or write */
  fp->_r = 0;
  fp->_bf._base = 0;		/* no buffer */
  fp->_bf._size = 0;
  fp->_lbfsize = 0;		/* not line buffered */
  fp->_file = -1;		/* no file */
  /* fp->_cookie = <any>; */	/* caller sets cookie, _read/_write etc */
  fp->_ub._base = 0;		/* no ungetc buffer */
  fp->_ub._size = 0;
  fp->_lb._base = 0;		/* no line buffer */
  fp->_lb._size = 0;
  fp->_data = d;
  return fp;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004200F0);
#ifdef SKIP_ASM
void func_004200F0(char *r) {
    char *f0 = r + 0x1E4;
    *(void **)(r + 0x3C) = (void *)func_00416FC0;
    *(int *)(r + 0x38) = 1;
    func_0041FF40((void *)f0, 4, 0, (int)r);
    func_0041FF40((void *)(r + 0x23C), 9, 1, (int)r);
    func_0041FF40((void *)(r + 0x294), 0xA, 2, (int)r);
    *(char **)(r + 0x1E0) = f0;
    *(int *)(r + 0x1DC) = 3;
    *(int *)(r + 0x1D8) = 0;
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_00420180);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420228);
#ifdef SKIP_ASM
void func_00420228(char *d, char *s, unsigned n) {
    while (n--)
        *d++ = *s++;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420260);
#ifdef SKIP_ASM
extern void func_00420228(char *, char *, unsigned);
extern int func_00417648(int *);
unsigned func_00420260(char *buf, unsigned size, unsigned count, int *fp) {
    unsigned resid, total, r;
    char *p;
    if ((resid = count * size) == 0)
        return 0;
    if (fp[1] < 0)
        fp[1] = 0;
    total = resid;
    p = buf;
    while (resid > (r = fp[1])) {
        func_00420228(p, (char *)fp[0], r);
        p += r;
        fp[0] = fp[0] + r;
        resid -= r;
        if (func_00417648(fp))
            return (total - resid) / size;
    }
    func_00420228(p, (char *)fp[0], resid);
    fp[1] -= resid;
    fp[0] += resid;
    return count;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", ungetc);
#ifdef SKIP_ASM
/* newlib stdio/ungetc.c (ungetc), as in Sony's copy: a full ungetc buffer is reported on fd 1 instead of being
 * grown with __submore. CHECK_INIT from stdio/local.h, FILE from sys/reent.h:
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
struct _reent_0378 {
  int _errno;
  struct __sFILE_0378 *_stdin, *_stdout, *_stderr;
  int  _inc;
  char _emergency[25];
  int _current_category;
  const char *_current_locale;
  int __sdidinit;
};
struct __sbuf_0378 {
	unsigned char *_base;
	int	_size;
};
struct __sFILE_0378 {
  unsigned char *_p;	/* current position in (some) buffer */
  int	_r;		/* read space left for getc() */
  int	_w;		/* write space left for putc() */
  short	_flags;		/* flags, below; this FILE is free if 0 */
  short	_file;		/* fileno, if Unix descriptor, else -1 */
  struct __sbuf_0378 _bf;	/* the buffer (at least 1 byte, if !NULL) */
  int	_lbfsize;	/* 0 or -_bf._size, for inline putc */
  void *_cookie;	/* cookie passed to io functions */
  int	(*_read)(void *_cookie, char *_buf, int _n);
  int	(*_write)(void *_cookie, const char *_buf, int _n);
  long	(*_seek)(void *_cookie, long _offset, int _whence);
  int	(*_close)(void *_cookie);
  struct __sbuf_0378 _ub;	/* ungetc buffer */
  unsigned char *_up;	/* saved _p when _p is doing ungetc data */
  int	_ur;		/* saved _r when _r is counting ungetc data */
  unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
  unsigned char _nbuf[1];	/* guarantee a getc() buffer */
  struct __sbuf_0378 _lb;	/* buffer for fgetline() */
  int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
  int	_offset;	/* current lseek offset */
  struct _reent_0378 *_data;
};
#define __SRD_0378	0x0004
#define __SWR_0378	0x0008
#define __SRW_0378	0x0010
#define __SEOF_0378	0x0020
#define EOF_0378	(-1)
#define HASUB_0378(fp) ((fp)->_ub._base != 0)
extern struct _reent_0378 *_impure_ptr_0378 __asm__("D_00454944");
#define _REENT_0378 _impure_ptr_0378
extern void __sinit_0378(struct _reent_0378 *) __asm__("func_004200F0");
extern int fflush_0378(struct __sFILE_0378 *) __asm__("func_0041FE30");
extern int write_0378(int, const char *, int) __asm__("func_00424368");
/* newlib's CHECK_INIT (stdio/local.h), verbatim: the do-while(0) is part of the library macro */
#define CHECK_INIT_0378(fp) \
  do					\
    {					\
      if ((fp)->_data == 0)		\
	(fp)->_data = _REENT_0378;	\
      if (!(fp)->_data->__sdidinit)	\
	__sinit_0378 ((fp)->_data);	\
    }					\
  while (0)
#define UNGETC_OVERFLOW_0378 "\nungetc error: *static buffer overflow* - char not added to buffer\n"

int
ungetc (int c, register struct __sFILE_0378 *fp)
{
  if (c == EOF_0378)
    return (EOF_0378);

  /* Ensure stdio has been initialized.
     ??? Might be able to remove this as some other stdio routine should
     have already been called to get the char we are un-getting.  */

  CHECK_INIT_0378 (fp);

  /* After ungetc, we won't be at eof anymore */
  fp->_flags &= ~__SEOF_0378;

  if ((fp->_flags & __SRD_0378) == 0)
    {
      /*
       * Not already reading: no good unless reading-and-writing.
       * Otherwise, flush any current write stuff.
       */
      if ((fp->_flags & __SRW_0378) == 0)
	return EOF_0378;
      if (fp->_flags & __SWR_0378)
	{
	  if (fflush_0378 (fp))
	    return EOF_0378;
	  fp->_flags &= ~__SWR_0378;
	  fp->_w = 0;
	  fp->_lbfsize = 0;
	}
      fp->_flags |= __SRD_0378;
    }
  c = (unsigned char) c;

  /*
   * If we are in the middle of ungetc'ing, just continue.
   */

  if (HASUB_0378 (fp))
    {
      if (fp->_r >= fp->_ub._size)
	{
	  write_0378 (1, UNGETC_OVERFLOW_0378, sizeof (UNGETC_OVERFLOW_0378));
	  return EOF_0378;
	}
      *--fp->_p = c;
      fp->_r++;
      return c;
    }

  /*
   * If we can handle this by simply backing up, do so,
   * but never replace the original character.
   * (This makes sscanf() work when scanning `const' data.)
   */

  if (fp->_bf._base != 0 && fp->_p > fp->_bf._base && fp->_p[-1] == c)
    {
      fp->_p--;
      fp->_r++;
      return c;
    }

  /*
   * Create an ungetc buffer.
   * Initially, we will use the `reserve' buffer.
   */

  fp->_ur = fp->_r;
  fp->_up = fp->_p;
  fp->_ub._base = fp->_ubuf;
  fp->_ub._size = sizeof (fp->_ubuf);
  fp->_ubuf[sizeof (fp->_ubuf) - 1] = c;
  fp->_p = &fp->_ubuf[sizeof (fp->_ubuf) - 1];
  fp->_r = 1;
  return c;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420510);
#ifdef SKIP_ASM
extern int *func_004224A0(int *, int, int);
int *func_00420510(int ep_, int k) {
    int *ep = (int *)ep_;
    int x;
    int *rv;
    if (ep[0x4C / 4] == 0) {
        ep[0x4C / 4] = (int)func_004224A0(ep, 4, 16);
        if (ep[0x4C / 4] == 0)
            return 0;
    }
    rv = ((int **)ep[0x4C / 4])[k];
    if (rv) {
        ((int **)ep[0x4C / 4])[k] = (int *)*rv;
    } else {
        x = 1 << k;
        rv = func_004224A0(ep, 1, 0x18 + (x - 1) * 4);
        if (rv == 0)
            return 0;
        rv[1] = k;
        rv[2] = x;
    }
    rv[3] = rv[4] = 0;
    return rv;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004205B8);
#ifdef SKIP_ASM
void func_004205B8(int *self, int *p) {
    if (p) {
        int i = p[1];
        p[0] = ((int *)self[0x4C / 4])[i];
        ((int **)self[0x4C / 4])[i] = p;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004205F0);
#ifdef SKIP_ASM
struct Bigint {
    struct Bigint *next;
    int k, maxwds, sign, wds;
    unsigned x[1];
};
extern int *func_00420510(int, int);
extern void func_004205B8(int *, int *);
extern void *func_0041605C(void *, void *, int);
struct Bigint *func_004205F0(int ptr, struct Bigint *b, int m, int a) {
    int i, wds;
    unsigned *x, y, xi, z, carry;
    struct Bigint *b1;
    wds = b->wds;
    x = b->x;
    i = 0;
    carry = a;
    do {
        xi = *x;
        y = (xi & 0xffff) * m + carry;
        z = (xi >> 16) * m + (y >> 16);
        carry = z >> 16;
        *x++ = (z << 16) + (y & 0xffff);
    } while (++i < wds);
    if (carry) {
        if (wds >= b->maxwds) {
            b1 = (struct Bigint *)func_00420510(ptr, b->k + 1);
            func_0041605C(&b1->sign, &b->sign, b->wds * 4 + 8);
            func_004205B8((int *)ptr, (int *)b);
            b = b1;
        }
        b->x[wds++] = carry;
        b->wds = wds;
    }
    return b;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004206F8);
#ifdef SKIP_ASM
extern struct Bigint *func_004205F0(int, struct Bigint *, int, int);
struct Bigint *func_004206F8(int ptr, char *s, int nd0, int nd, int y9) {
    struct Bigint *b;
    int i, k;
    int x, y;
    x = (nd + 8) / 9;
    for (k = 0, y = 1; x > y; y <<= 1, k++)
        ;
    b = (struct Bigint *)func_00420510(ptr, k);
    b->x[0] = y9;
    b->wds = 1;
    i = 9;
    if (9 < nd0) {
        s += 9;
        do
            b = func_004205F0(ptr, b, 10, *s++ - '0');
        while (++i < nd0);
        s++;
    } else
        s += 10;
    for (; i < nd; i++)
        b = func_004205F0(ptr, b, 10, *s++ - '0');
    return b;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420830);
#ifdef SKIP_ASM
int func_00420830(unsigned x) {
    int k = 0;
    if (!(x & 0xffff0000)) {
        k = 16;
        x <<= 16;
    }
    if (!(x & 0xff000000)) {
        k += 8;
        x <<= 8;
    }
    if (!(x & 0xf0000000)) {
        k += 4;
        x <<= 4;
    }
    if (!(x & 0xc0000000)) {
        k += 2;
        x <<= 2;
    }
    if (!(x & 0x80000000)) {
        k++;
        if (!(x & 0x40000000))
            return 32;
    }
    return k;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004208B8);
#ifdef SKIP_ASM
int func_004208B8(unsigned *y) {
    int k;
    unsigned x = *y;
    if (x & 7) {
        if (x & 1)
            return 0;
        if (x & 2) {
            *y = x >> 1;
            return 1;
        }
        *y = x >> 2;
        return 2;
    }
    k = 0;
    if (!(x & 0xffff)) {
        k = 16;
        x >>= 16;
    }
    if (!(x & 0xff)) {
        k += 8;
        x >>= 8;
    }
    if (!(x & 0xf)) {
        k += 4;
        x >>= 4;
    }
    if (!(x & 3)) {
        k += 2;
        x >>= 2;
    }
    if (!(x & 1)) {
        k++;
        x >>= 1;
        if (!x)
            return 32;
    }
    *y = x;
    return k;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420978);
#ifdef SKIP_ASM
extern int *func_00420510(int, int);
int *func_00420978(int a, int b) {
    int *p = func_00420510(a, 1);
    p[5] = b;
    p[4] = 1;
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004209B0);
#ifdef SKIP_ASM
extern int *func_00420510(int, int);
struct Bigint *func_004209B0(int ptr, struct Bigint *a, struct Bigint *b) {
    struct Bigint *c;
    int i, k, wa, wb, wc;
    unsigned *x, *xa, *xae, *xb, *xbe, *xc, *xc0;
    unsigned y, carry, z, z2;
    if (a->wds < b->wds) {
        c = a;
        a = b;
        b = c;
    }
    k = a->k;
    wa = a->wds;
    wb = b->wds;
    wc = wa + wb;
    if (wc > a->maxwds)
        k++;
    c = (struct Bigint *)func_00420510(ptr, k);
    for (x = c->x, xa = x + wc; x < xa; x++)
        *x = 0;
    xa = a->x;
    xae = xa + wa;
    xb = b->x;
    xbe = xb + wb;
    xc0 = c->x;
    for (; xb < xbe; xb++, xc0++) {
        if ((y = *xb & 0xffff)) {
            x = xa;
            xc = xc0;
            carry = 0;
            do {
                z = (*x & 0xffff) * y + (*xc & 0xffff) + carry;
                carry = z >> 16;
                z2 = (*x++ >> 16) * y + (*xc >> 16) + carry;
                carry = z2 >> 16;
                ((unsigned short *)xc)[1] = z2;
                ((unsigned short *)xc)[0] = z;
                xc++;
            } while (x < xae);
            *xc = carry;
        }
        if ((y = *xb >> 16)) {
            x = xa;
            xc = xc0;
            carry = 0;
            z2 = *xc;
            do {
                z = (*x & 0xffff) * y + (*xc >> 16) + carry;
                carry = z >> 16;
                ((unsigned short *)xc)[1] = z;
                ((unsigned short *)xc)[0] = z2;
                xc++;
                z2 = (*x++ >> 16) * y + (*xc & 0xffff) + carry;
                carry = z2 >> 16;
            } while (x < xae);
            *xc = z2;
        }
    }
    for (xc0 = c->x, xc = xc0 + wc; wc > 0 && !*--xc; --wc)
        ;
    c->wds = wc;
    return c;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420BC8);
#ifdef SKIP_ASM
extern struct Bigint *func_004205F0(int, struct Bigint *, int, int);
extern struct Bigint *func_004209B0(int, struct Bigint *, struct Bigint *);
extern int *func_00420978(int, int);
extern void func_004205B8(int *, int *);
extern int D_0049A8E8[];
struct Bigint *func_00420BC8(int ptr, struct Bigint *b, int k) {
    struct Bigint *b1, *p5, *p51;
    int i;
    if ((i = k & 3))
        b = func_004205F0(ptr, b, D_0049A8E8[i - 1], 0);
    if (!(k >>= 2))
        return b;
    if (!(p5 = ((struct Bigint **)ptr)[0x48 / 4])) {
        p5 = ((struct Bigint **)ptr)[0x48 / 4] = (struct Bigint *)func_00420978(ptr, 625);
        p5->next = 0;
    }
    for (;;) {
        if (k & 1) {
            b1 = func_004209B0(ptr, b, p5);
            func_004205B8((int *)ptr, (int *)b);
            b = b1;
        }
        if (!(k >>= 1))
            break;
        if (!(p51 = p5->next)) {
            p51 = p5->next = func_004209B0(ptr, p5, p5);
            p51->next = 0;
        }
        p5 = p51;
    }
    return b;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420CC8);
#ifdef SKIP_ASM
extern int *func_00420510(int, int);
extern void func_004205B8(int *, int *);
struct Bigint *func_00420CC8(int ptr, struct Bigint *b, int k) {
    int i, k1, n, n1;
    struct Bigint *b1;
    unsigned *x, *x1, *xe, z;
    n = k >> 5;
    k1 = b->k;
    n1 = n + b->wds + 1;
    for (i = b->maxwds; n1 > i; i <<= 1)
        k1++;
    b1 = (struct Bigint *)func_00420510(ptr, k1);
    x1 = b1->x;
    for (i = 0; i < n; i++)
        *x1++ = 0;
    x = b->x;
    xe = x + b->wds;
    if (k &= 0x1f) {
        k1 = 32 - k;
        z = 0;
        do {
            *x1++ = *x << k | z;
            z = *x++ >> k1;
        } while (x < xe);
        if ((*x1 = z))
            ++n1;
    } else
        do
            *x1++ = *x++;
        while (x < xe);
    b1->wds = n1 - 1;
    func_004205B8((int *)ptr, (int *)b);
    return b1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420E28);
#ifdef SKIP_ASM
struct Bigint28 {
    struct Bigint28 *next;
    int k, maxwds, sign, wds;
    unsigned x[1];
};
int func_00420E28(struct Bigint28 *a, struct Bigint28 *b) {
    unsigned *xa, *xa0, *xb, *xb0;
    int i, j;
    i = a->wds;
    j = b->wds;
    if (i -= j)
        return i;
    xa0 = a->x;
    xa = xa0 + j;
    xb0 = b->x;
    xb = xb0 + j;
    for (;;) {
        if (*--xa != *--xb)
            return *xa < *xb ? -1 : 1;
        if (xa <= xa0)
            break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420E90);
#ifdef SKIP_ASM
extern int *func_00420510(int, int);
struct Bigint *func_00420E90(int ptr, struct Bigint *a, struct Bigint *b) {
    struct Bigint *c;
    int i, wa, wb;
    int borrow, y, z;
    unsigned *xa, *xae, *xb, *xbe, *xc;
    i = func_00420E28((struct Bigint28 *)a, (struct Bigint28 *)b);
    if (!i) {
        c = (struct Bigint *)func_00420510(ptr, 0);
        c->wds = 1;
        c->x[0] = 0;
        return c;
    }
    if (i < 0) {
        c = a;
        a = b;
        b = c;
        i = 1;
    } else
        i = 0;
    c = (struct Bigint *)func_00420510(ptr, a->k);
    c->sign = i;
    wa = a->wds;
    xa = a->x;
    xae = xa + wa;
    wb = b->wds;
    xb = b->x;
    xbe = xb + wb;
    xc = c->x;
    borrow = 0;
    do {
        y = (*xa & 0xffff) - (*xb & 0xffff) + borrow;
        borrow = y >> 16;
        z = (*xa++ >> 16) - (*xb++ >> 16) + borrow;
        borrow = z >> 16;
        ((unsigned short *)xc)[1] = z;
        ((unsigned short *)xc)[0] = y;
        xc++;
    } while (xb < xbe);
    while (xa < xae) {
        y = (*xa & 0xffff) + borrow;
        borrow = y >> 16;
        z = (*xa++ >> 16) + borrow;
        borrow = z >> 16;
        ((unsigned short *)xc)[1] = z;
        ((unsigned short *)xc)[0] = y;
        xc++;
    }
    while (!*--xc)
        wa--;
    c->wds = wa;
    return c;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00421020);
#ifdef SKIP_ASM
union double_union {
    double d;
    struct {
        unsigned long lo : 32;
        unsigned long hi : 32;
    } w;
    long l;
};
#define word0(x) x.w.hi
#define word1(x) x.w.lo
double func_00421020(double _x) {
    union double_union x, a;
    int L;
    x.d = _x;
    L = (word0(x) & 0x7ff00000) - 52 * 0x100000;
    if (L > 0) {
        word0(a) = L;
        word1(a) = 0;
    } else {
        L = -L >> 20;
        if (L < 20) {
            word0(a) = 0x80000 >> L;
            word1(a) = 0;
        } else {
            word0(a) = 0;
            L -= 20;
            word1(a) = L >= 31 ? 1 : 1 << (31 - L);
        }
    }
    return a.d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004210E8);
#ifdef SKIP_ASM
extern int func_00420830(unsigned);
double func_004210E8(struct Bigint *a, int *e) {
    unsigned *xa, *xa0, w, y, z;
    int k;
    union double_union d;
    xa0 = a->x;
    xa = xa0 + a->wds;
    y = *--xa;
    k = func_00420830(y);
    *e = 32 - k;
    if (k < 11) {
        d.w.hi = 0x3ff00000 | y >> (11 - k);
        w = xa > xa0 ? *--xa : 0;
        d.w.lo = y << (21 + k) | w >> (11 - k);
        goto ret_d;
    }
    z = xa > xa0 ? *--xa : 0;
    if (k -= 11) {
        d.w.hi = 0x3ff00000 | y << k | z >> (32 - k);
        y = xa > xa0 ? *--xa : 0;
        d.w.lo = z << k | y >> (32 - k);
    } else {
        d.w.hi = 0x3ff00000 | y;
        d.w.lo = z;
    }
ret_d:
    return d.d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00421280);
#ifdef SKIP_ASM
extern int *func_00420510(int, int);
extern int func_004208B8(unsigned *);
extern int func_00420830(unsigned);
struct Bigint *func_00421280(int ptr, double _d, int *e, int *bits) {
    struct Bigint *b;
    int de, i, k;
    unsigned *x, y, z;
    union double_union d;
    d.d = _d;
    b = (struct Bigint *)func_00420510(ptr, 1);
    x = b->x;
    z = d.w.hi & 0xfffff;
    d.w.hi &= 0x7fffffff;
    if ((de = (int)(d.w.hi >> 20)))
        z |= 0x100000;
    if ((y = d.w.lo)) {
        if ((k = func_004208B8(&y))) {
            x[0] = y | z << (32 - k);
            z >>= k;
        } else
            x[0] = y;
        i = b->wds = (x[1] = z) ? 2 : 1;
    } else {
        k = func_004208B8(&z);
        x[0] = z;
        i = b->wds = 1;
        k += 32;
    }
    if (de) {
        *e = de - 1023 - 52 + k;
        *bits = 53 - k;
    } else {
        *e = de - 1023 - 52 + 1 + k;
        *bits = 32 * i - func_00420830(x[i - 1]);
    }
    return b;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00421400);
#ifdef SKIP_ASM
/* newlib stdlib/mprec.c (ratio), with the unit's struct Bigint, union double_union and word0 (func_00421020..):
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */
#define Exp_msk1_1400 0x100000
/* PORT: the soft-float double divide (gcc's dpdiv libcall), called by name: retail's symbol for it is
   dpdiv (as in seg_325220's dpdiv(x, 10.0)); on PC this is just da.d / db.d */
extern double dpdiv_1400(double, double) __asm__("dpdiv");

double func_00421400(struct Bigint *a, struct Bigint *b)
{
  union double_union da, db;
  int k, ka, kb;

  da.d = func_004210E8 (a, &ka);
  db.d = func_004210E8 (b, &kb);
  k = ka - kb + 32 * (a->wds - b->wds);
  if (k > 0)
    word0 (da) += k * Exp_msk1_1400;
  else
    {
      k = -k;
      word0 (db) += k * Exp_msk1_1400;
    }
  return dpdiv_1400 (da.d, db.d);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004214C0);
#ifdef SKIP_ASM
extern double D_0049A8F8[];
extern double dpmul(double, double);
double func_004214C0(int n) {
    double r = 1.0;
    if (n < 24)
        return D_0049A8F8[n];
    if (n > 0) {
        do {
            r = dpmul(r, 10.0);
        } while (--n > 0);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00421530);
#ifdef SKIP_ASM
struct R21530;
extern char *g21530_dtoa(struct R21530 *, double, int, int, int *, int *, char **) __asm__("func_00422780");
extern char *g21530_strcpy(char *, char *) __asm__("strcpy");
void func_00421530_(struct R21530 *ptr, char *buf, double invalue, int ndigit, int type, int dot, int mode) __asm__("func_00421530");
void func_00421530_(struct R21530 *ptr, char *buf, double invalue, int ndigit, int type, int dot, int mode) {
    int decpt;
    int sign;
    char *p, *start, *end;
    start = p = g21530_dtoa(ptr, invalue, mode, ndigit, &decpt, &sign, &end);
    if (decpt == 9999) {
        g21530_strcpy(buf, p);
        return;
    }
    while (*p != '\0' && decpt > 0) {
        *buf++ = *p++;
        decpt--;
    }
    while (decpt > 0) {
        *buf++ = '0';
        decpt--;
    }
    if (dot == 0 && *p == '\0') {
        *buf = '\0';
        return;
    }
    if (p == start)
        *buf++ = '0';
    *buf++ = '.';
    while (decpt < 0 && ndigit > 0) {
        *buf++ = '0';
        decpt++;
        ndigit--;
    }
    while (*p && ndigit > 0) {
        *buf++ = *p++;
        ndigit--;
    }
    while (ndigit > 0) {
        *buf++ = '0';
        ndigit--;
    }
    *buf = '\0';
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004216E8);
#ifdef SKIP_ASM
struct R216E8;
extern char *g216E8_dtoa(struct R216E8 *, double, int, int, int *, int *, char **) __asm__("func_00422780");
extern char *g216E8_strcpy(char *, char *) __asm__("strcpy");
void func_004216E8_(struct R216E8 *ptr, char *buf, double invalue, int ndigit, char type, int dot) __asm__("func_004216E8");
void func_004216E8_(struct R216E8 *ptr, char *buf, double invalue, int ndigit, char type, int dot) {
    int decpt;
    int i = ndigit;
    int sign;
    char *p, *end;
    p = g216E8_dtoa(ptr, invalue, 2, i + 1, &decpt, &sign, &end);
    if (decpt == 9999) {
        g216E8_strcpy(buf, p);
        return;
    }
    *buf++ = *p++;
    if (dot || i != 0)
        *buf++ = '.';
    while (*p && i > 0) {
        *buf++ = *p++;
        i--;
    }
    if (type == 'g')
        type = 'e';
    else if (type == 'G')
        type = 'E';
    else {
        while (i > 0) {
            *buf++ = '0';
            i--;
        }
    }
    *buf++ = type;
    decpt--;
    if (decpt < 0) {
        *buf++ = '-';
        decpt = -decpt;
    } else
        *buf++ = '+';
    if (decpt >= 100) {
        int dig = decpt / 100;
        *buf++ = dig + '0';
        decpt -= dig * 100;
    }
    {
        int dig = decpt / 10;
        *buf++ = dig + '0';
        decpt -= dig * 10;
    }
    *buf++ = decpt + '0';
    *buf = '\0';
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004218D8);
#ifdef SKIP_ASM
struct R218D8 {
    int pad0[0x50 / 4];
    int cvtlen;
    char *cvtbuf;
};
extern struct R218D8 *g218D8_impure __asm__("D_00454944");
#define REENT218D8 g218D8_impure
extern char *g218D8_realloc(struct R218D8 *, char *, int) __asm__("func_00421F20");
extern char *g218D8_dtoa(struct R218D8 *, double, int, int, int *, int *, char **) __asm__("func_00422780");
char *func_004218D8_(double invalue, int ndigit, int *decpt, int *sign, char *buf) __asm__("func_004218D8");
char *func_004218D8_(double invalue, int ndigit, int *decpt, int *sign, char *buf) {
    char *save, *p, *end;
    int done = 0;
    int newlen;
    if (buf == 0) {
        if (ndigit >= REENT218D8->cvtlen) {
            newlen = ndigit + 1;
            buf = g218D8_realloc(REENT218D8, REENT218D8->cvtbuf, newlen);
            if (buf == 0)
                return 0;
            REENT218D8->cvtlen = newlen;
            REENT218D8->cvtbuf = buf;
        }
        buf = REENT218D8->cvtbuf;
    }
    save = buf;
    if (invalue < 1.0 && invalue > -1.0)
        p = g218D8_dtoa(REENT218D8, invalue, 2, ndigit, decpt, sign, &end);
    else
        p = g218D8_dtoa(REENT218D8, invalue, 3, ndigit, decpt, sign, &end);
    while (p < end) {
        *buf++ = *p++;
        done++;
    }
    if (done < ndigit) {
        done = ndigit - done;
        do {
            *buf++ = '0';
        } while (--done);
    }
    *buf = 0;
    return save;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00421A50);
#ifdef SKIP_ASM
struct Reent51 {
    int pad0[0x50 / 4];
    int cvtlen;
    char *cvtbuf;
};
extern int D_00454944;
#define REENT ((struct Reent51 *)D_00454944)
extern char *func_00421F20(struct Reent51 *, char *, int);
extern char *func_00422780(struct Reent51 *, double, int, int, int *, int *, char **);
char *func_00421A50_(double value, int ndigit, int *decpt, int *sign, char *buf) __asm__("func_00421A50");
char *func_00421A50_(double value, int ndigit, int *decpt, int *sign, char *buf) {
    char *save, *p, *end;
    int i = 0;
    int newlen;
    if (buf == 0) {
        if (ndigit >= REENT->cvtlen) {
            newlen = ndigit + 1;
            buf = func_00421F20(REENT, REENT->cvtbuf, newlen);
            if (buf == 0)
                return 0;
            REENT->cvtlen = newlen;
            REENT->cvtbuf = buf;
        }
        buf = REENT->cvtbuf;
    }
    save = buf;
    p = func_00422780(REENT, value, 2, ndigit, decpt, sign, &end);
    while (p < end) {
        *buf++ = *p++;
        i++;
    }
    if (i < ndigit) {
        i = ndigit - i;
        do {
            *buf++ = '0';
        } while (--i);
    }
    *buf = 0;
    return save;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00421B98);
#ifdef SKIP_ASM
/* newlib stdlib/ecvtbuf.c (_gcvt). The file carries no notice of its own (newlib's default: Copyright (c)
 * 1994-2000 Red Hat, Inc. Permission to use, copy, modify and distribute this software is freely granted, provided
 * that this notice is preserved.) */
struct _reent_1B98;
extern double _mprec_log10_1B98(int) __asm__("func_004214C0");
extern void print_e_1B98(struct _reent_1B98 *, char *, double, int, char, int) __asm__("func_004216E8");
extern char *_dtoa_r_1B98(struct _reent_1B98 *, double, int, int, int *, int *, char **) __asm__("func_00422780");
extern char *strcpy_1B98(char *, const char *) __asm__("strcpy");

char *func_00421B98_(struct _reent_1B98 *ptr, double invalue, int ndigit, char *buf, char type, int dot)
    __asm__("func_00421B98");
char *
func_00421B98_ (struct _reent_1B98 *ptr, double invalue, int ndigit, char *buf, char type, int dot)
{
  char *save = buf;

  if (invalue < 0)
    {
      invalue = -invalue;
    }

  if (invalue == 0)
    {
      *buf++ = '0';
      *buf = '\0';
    }
  else
    /* Which one to print ?
       ANSI says that anything with more that 4 zeros after the . or more
       than precision digits before is printed in e with the qualification
       that trailing zeroes are removed from the fraction portion.  */

  if (0.0001 >= invalue || invalue >= _mprec_log10_1B98 (ndigit))
    {
      /* We subtract 1 from ndigit because in the 'e' format the precision is
	 the number of digits after the . but in 'g' format it is the number
	 of significant digits.

	 We defer changing type to e/E so that print_e() can know it's us
	 calling and thus should remove trailing zeroes.  */

      print_e_1B98 (ptr, buf, invalue, ndigit - 1, type, dot);
    }
  else
    {
      int decpt;
      int sign;
      char *end;
      char *p;

      if (invalue < 1.0)
	{
	  /* what we want is ndigits after the point */
	  p = _dtoa_r_1B98 (ptr, invalue, 3, ndigit, &decpt, &sign, &end);
	}
      else
	{
	  p = _dtoa_r_1B98 (ptr, invalue, 2, ndigit, &decpt, &sign, &end);
	}

      if (decpt == 9999)
	{
	  strcpy_1B98 (buf, p);
	  return save;
	}
      while (*p && decpt > 0)
	{
	  *buf++ = *p++;
	  decpt--;
	  ndigit--;
	}
      /* Even if not in buffer */
      while (decpt > 0 && ndigit > 0)
	{
	  *buf++ = '0';
	  decpt--;
	  ndigit--;
	}

      if (dot || *p)
	{
	  if (buf == save)
	    *buf++ = '0';
	  *buf++ = '.';
	  while (decpt < 0 && ndigit > 0)
	    {
	      *buf++ = '0';
	      decpt++;
	      ndigit--;
	    }

	  /* Print rest of stuff */
	  while (*p && ndigit > 0)
	    {
	      *buf++ = *p++;
	      ndigit--;
	    }
	  /* And trailing zeros */
	  if (dot)
	    {
	      while (ndigit > 0)
		{
		  *buf++ = '0';
		  ndigit--;
		}
	    }
	}
      *buf++ = 0;
    }

  return save;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00421E60);
#ifdef SKIP_ASM
extern void func_00421530_f(int, char *, long, int, int, int, int) __asm__("func_00421530");
extern void func_00421B98_g(int, long, int, char *, int, int) __asm__("func_00421B98");
extern void func_004216E8_e(int, char *, long, int, int, int) __asm__("func_004216E8");
char *func_00421E60(int ep, char *buf, long val, int nd, int unused, char fmt, int pad) {
    switch (fmt) {
    case 'f':
    case 'F':
        func_00421530_f(ep, buf, val, nd, fmt, nd ? 1 : pad, 3);
        break;
    case 'g':
    case 'G':
        func_00421B98_g(ep, val, nd ? nd : 1, buf, fmt, pad);
        break;
    case 'e':
    case 'E':
        func_004216E8_e(ep, buf, val, nd, fmt, pad);
        break;
    }
    return buf;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00421F20);
#ifdef SKIP_ASM
extern void func_00418CA8_lock(int) __asm__("func_00418CA8");
extern void func_00418D08_unlock(int) __asm__("func_00418D08");
extern void *func_0041DBF8_malloc(int, int) __asm__("func_0041DBF8");
extern void func_0041E340_free(int, void *) __asm__("func_0041E340");
extern void *func_0041605C_memcpy(void *, void *, unsigned) __asm__("func_0041605C");
#define MALLOC_COPY(dest, src, nbytes)     do {         unsigned mcn = (nbytes);         if (mcn < 37) {             unsigned *mcsrc = (unsigned *)(src);             unsigned *mcdst = (unsigned *)(dest);             if (mcn >= 20) {                 *mcdst++ = *mcsrc++;                 *mcdst++ = *mcsrc++;                 if (mcn >= 28) {                     *mcdst++ = *mcsrc++;                     *mcdst++ = *mcsrc++;                     if (mcn >= 36) {                         *mcdst++ = *mcsrc++;                         *mcdst++ = *mcsrc++;                     }                 }             }             *mcdst++ = *mcsrc++;             *mcdst++ = *mcsrc++;             *mcdst = *mcsrc;         } else             func_0041605C_memcpy(dest, src, mcn);     } while (0)
#define inuse(p) (chunk_at_offset(p, (p)->size & ~1u)->size & 1)
void *func_00421F20_(int ptr, void *oldmem, unsigned bytes) __asm__("func_00421F20");
void *func_00421F20_(int ptr, void *oldmem, unsigned bytes) {
    unsigned nb, oldsize, newsize, nextsize, prevsize, remainder_size;
    mchunkptr oldp, newp, next, prev, remainder, bck, fwd;
    void *newmem;
    if (oldmem == 0)
        return func_0041DBF8_malloc(ptr, bytes);
    func_00418CA8_lock(ptr);
    newp = oldp = (mchunkptr)((char *)oldmem - 8);
    newsize = oldsize = chunksize(oldp);
    nb = (bytes + 19 < 31) ? 16 : ((bytes + 19) & ~15u);
    if (oldsize < nb) {
        next = chunk_at_offset(oldp, oldsize);
        if (next == top || !inuse(next)) {
            nextsize = chunksize(next);
            if (next == top) {
                if (nextsize + newsize >= nb + 16) {
                    newsize += nextsize;
                    top = chunk_at_offset(oldp, nb);
                    set_head(top, (newsize - nb) | 1);
                    set_head_size(oldp, nb);
                    func_00418D08_unlock(ptr);
                    return (char *)oldp + 8;
                }
            } else if (nextsize + newsize >= nb) {
                unlink(next, bck, fwd);
                newsize += nextsize;
                goto split;
            }
        } else {
            next = 0;
            nextsize = 0;
        }
        if (!(oldp->size & 1)) {
            prev = chunk_at_offset(oldp, -(int)oldp->prev_size);
            prevsize = chunksize(prev);
            if (next != 0) {
                if (next == top) {
                    if (nextsize + prevsize + newsize >= nb + 16) {
                        unlink(prev, bck, fwd);
                        newp = prev;
                        newsize += prevsize + nextsize;
                        newmem = (char *)newp + 8;
                        MALLOC_COPY((char *)newp + 8, oldmem, oldsize - 4);
                        top = chunk_at_offset(newp, nb);
                        set_head(top, (newsize - nb) | 1);
                        set_head_size(newp, nb);
                        func_00418D08_unlock(ptr);
                        return newmem;
                    }
                } else if (nextsize + prevsize + newsize >= nb) {
                    unlink(next, bck, fwd);
                    unlink(prev, bck, fwd);
                    newp = prev;
                    newsize += nextsize + prevsize;
                    newmem = (char *)newp + 8;
                    MALLOC_COPY((char *)newp + 8, oldmem, oldsize - 4);
                    goto split;
                }
            }
            if (prev != 0 && prevsize + newsize >= nb) {
                unlink(prev, bck, fwd);
                newp = prev;
                newsize += prevsize;
                newmem = (char *)newp + 8;
                MALLOC_COPY((char *)newp + 8, oldmem, oldsize - 4);
                goto split;
            }
        }
        newmem = func_0041DBF8_malloc(ptr, bytes);
        if (newmem == 0) {
            func_00418D08_unlock(ptr);
            return 0;
        }
        if ((newp = (mchunkptr)((char *)newmem - 8)) == chunk_at_offset(oldp, oldp->size & ~1u)) {
            newsize += chunksize(newp);
            newp = oldp;
            goto split;
        }
        MALLOC_COPY(newmem, oldmem, oldsize - 4);
        func_0041E340_free(ptr, oldmem);
        func_00418D08_unlock(ptr);
        return newmem;
    }
split:
    if (newsize - nb >= 16) {
        remainder = chunk_at_offset(newp, nb);
        remainder_size = newsize - nb;
        set_head_size(newp, nb);
        set_head(remainder, remainder_size | 1);
        chunk_at_offset(remainder, remainder_size)->size |= 1;
        func_0041E340_free(ptr, (char *)remainder + 8);
    } else {
        set_head_size(newp, newsize);
        chunk_at_offset(newp, newsize)->size |= 1;
    }
    func_00418D08_unlock(ptr);
    return (char *)newp + 8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004224A0);
#ifdef SKIP_ASM
extern void *func_0041DBF8_malloc(int, int) __asm__("func_0041DBF8");
extern void *func_00416210_memset(void *, int, unsigned) __asm__("func_00416210");
#define MALLOC_ZERO(charp, nbytes)     do {         unsigned mctmp = (nbytes);         if (mctmp < 37) {             unsigned *mzp = (unsigned *)(charp);             if (mctmp >= 20) {                 *mzp++ = 0;                 *mzp++ = 0;                 if (mctmp >= 28) {                     *mzp++ = 0;                     *mzp++ = 0;                     if (mctmp >= 36) {                         *mzp++ = 0;                         *mzp++ = 0;                     }                 }             }             *mzp++ = 0;             *mzp++ = 0;             *mzp = 0;         } else             func_00416210_memset(charp, 0, mctmp);     } while (0)
void *func_004224A0_(int ptr, int n, int elem_size) __asm__("func_004224A0");
void *func_004224A0_(int ptr, int n, int elem_size) {
    mchunkptr p;
    unsigned csz;
    unsigned sz = n * elem_size;
    void *mem;
    mem = func_0041DBF8_malloc(ptr, sz);
    if (mem == 0)
        return 0;
    p = (mchunkptr)((char *)mem - 8);
    csz = chunksize(p);
    MALLOC_ZERO(mem, csz - 4);
    return mem;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00422560);
#ifdef SKIP_ASM
extern int func_00420E28_cmp(struct Bigint *, struct Bigint *) __asm__("func_00420E28");
int func_00422560(struct Bigint *b, struct Bigint *S) {
    int n;
    int borrow, y;
    unsigned carry, q, ys;
    unsigned *bx, *bxe, *sx, *sxe;
    int z;
    unsigned si, zs;
    n = S->wds;
    if (b->wds < n)
        return 0;
    sx = S->x;
    sxe = sx + --n;
    bx = b->x;
    bxe = bx + n;
    q = *bxe / (*sxe + 1);
    if (q) {
        borrow = 0;
        carry = 0;
        do {
            si = *sx++;
            ys = (si & 0xffff) * q + carry;
            zs = (si >> 16) * q + (ys >> 16);
            carry = zs >> 16;
            y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
            borrow = y >> 16;
            z = (*bx >> 16) - (zs & 0xffff) + borrow;
            borrow = z >> 16;
            ((unsigned short *)bx)[1] = (unsigned short)z;
            ((unsigned short *)bx)[0] = (unsigned short)y;
            bx++;
        } while (sx <= sxe);
        if (!*bxe) {
            bx = b->x;
            while (--bxe > bx && !*bxe)
                --n;
            b->wds = n;
        }
    }
    if (func_00420E28_cmp(b, S) >= 0) {
        q++;
        borrow = 0;
        carry = 0;
        bx = b->x;
        sx = S->x;
        do {
            si = *sx++;
            ys = (si & 0xffff) + carry;
            zs = (si >> 16) + (ys >> 16);
            carry = zs >> 16;
            y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
            borrow = y >> 16;
            z = (*bx >> 16) - (zs & 0xffff) + borrow;
            borrow = z >> 16;
            ((unsigned short *)bx)[1] = (unsigned short)z;
            ((unsigned short *)bx)[0] = (unsigned short)y;
            bx++;
        } while (sx <= sxe);
        bx = b->x;
        bxe = bx + n;
        if (!*bxe) {
            while (--bxe > bx && !*bxe)
                --n;
            b->wds = n;
        }
    }
    return q;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00422780);
#ifdef SKIP_ASM
/* newlib 1.9 libc/stdlib/dtoa.c (_dtoa_r) with the mprec.h macros it uses, verbatim (IEEE_8087, Pack_32,
 * IEEE_Arith, no Sudden_Underflow / ROUND_BIASED / Check_FLT_ROUNDS / No_leftright):
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */
struct _reent_00422780 {
  int pad0[0x40 / 4];
  struct Bigint *_result;
  int _result_k;
};
union double_union_00422780
{
  double d;
  unsigned int i[2];
};
#define word0_00422780(x) (x.i[1])
#define word1_00422780(x) (x.i[0])
#define Exp_shift1_00422780 20
#define Exp_msk1_00422780    ((unsigned int)0x100000L)
#define Exp_mask_00422780  ((unsigned int)0x7ff00000L)
#define P_00422780 53
#define Bias_00422780 1023
#define Exp_11_00422780 ((unsigned int)0x3ff00000L)
#define Frac_mask1_00422780 ((unsigned int)0xfffffL)
#define Ten_pmax_00422780 22
#define Bletch_00422780 0x10
#define Bndry_mask_00422780  ((unsigned int)0xfffffL)
#define Sign_bit_00422780 ((unsigned int)0x80000000L)
#define Log2P_00422780 1
#define Quick_max_00422780 14
#define Int_max_00422780 14
#define n_bigtens_00422780 5
#define Bcopy_00422780(x,y) func_0041605C((char *)&x->sign, (char *)&y->sign, y->wds*sizeof(int) + 2*sizeof(int))
extern const double tens_00422780[] __asm__("D_0049A8F8");
extern const double bigtens_00422780[] __asm__("D_0049A9C0");
extern struct Bigint *g2780_Balloc(struct _reent_00422780 *, int) __asm__("func_00420510");
extern void g2780_Bfree(struct _reent_00422780 *, struct Bigint *) __asm__("func_004205B8");
extern struct Bigint *g2780_multadd(struct _reent_00422780 *, struct Bigint *, int, int) __asm__("func_004205F0");
extern int g2780_hi0bits(unsigned int) __asm__("func_00420830");
extern struct Bigint *g2780_i2b(struct _reent_00422780 *, int) __asm__("func_00420978");
extern struct Bigint *g2780_mult(struct _reent_00422780 *, struct Bigint *, struct Bigint *) __asm__("func_004209B0");
extern struct Bigint *g2780_pow5mult(struct _reent_00422780 *, struct Bigint *, int) __asm__("func_00420BC8");
extern struct Bigint *g2780_lshift(struct _reent_00422780 *, struct Bigint *, int) __asm__("func_00420CC8");
extern int g2780_cmp(struct Bigint *, struct Bigint *) __asm__("func_00420E28");
extern struct Bigint *g2780_diff(struct _reent_00422780 *, struct Bigint *, struct Bigint *) __asm__("func_00420E90");
extern struct Bigint *g2780_d2b(struct _reent_00422780 *, double, int *, int *) __asm__("func_00421280");

/* the unit declares this with struct Reent51 * (ecvtbuf, func_00421A50) */
char *func_00422780_(struct _reent_00422780 *, double, int, int, int *, int *, char **) __asm__("func_00422780");
char *
func_00422780_(struct _reent_00422780 *ptr, double _d, int mode, int ndigits, int *decpt, int *sign, char **rve)
{
  int bbits, b2, b5, be, dig, i, ieps, ilim, ilim0, ilim1, j, j1, k, k0,
    k_check, leftright, m2, m5, s2, s5, spec_case, try_quick;
  union double_union_00422780 d, d2, eps;
  int L;
  int denorm;
  unsigned long x;
  struct Bigint *b, *b1, *delta, *mlo, *mhi, *S;
  double ds;
  char *s, *s0;

  d.d = _d;

  if (ptr->_result)
    {
      ptr->_result->k = ptr->_result_k;
      ptr->_result->maxwds = 1 << ptr->_result_k;
      g2780_Bfree (ptr, ptr->_result);
      ptr->_result = 0;
    }

  if (word0_00422780 (d) & Sign_bit_00422780)
    {
      /* set sign for everything, including 0's and NaNs */
      *sign = 1;
      word0_00422780 (d) &= ~Sign_bit_00422780;	/* clear sign bit */
    }
  else
    *sign = 0;

  if ((word0_00422780 (d) & Exp_mask_00422780) == Exp_mask_00422780)
    {
      /* Infinity or NaN */
      *decpt = 9999;
      s =
	!word1_00422780 (d) && !(word0_00422780 (d) & 0xfffff) ? "Infinity" :
	"NaN";
      if (rve)
	*rve =
	  s[3] ? s + 8 :
	  s + 3;
      return s;
    }
  if (!d.d)
    {
      *decpt = 1;
      s = "0";
      if (rve)
	*rve = s + 1;
      return s;
    }

  b = g2780_d2b (ptr, d.d, &be, &bbits);
  if ((i = (int) (word0_00422780 (d) >> Exp_shift1_00422780 & (Exp_mask_00422780 >> Exp_shift1_00422780))) != 0)
    {
      d2.d = d.d;
      word0_00422780 (d2) &= Frac_mask1_00422780;
      word0_00422780 (d2) |= Exp_11_00422780;

      /* log(x)	~=~ log(1.5) + (x-1.5)/1.5
		 * log10(x)	 =  log(x) / log(10)
		 *		~=~ log(1.5)/log(10) + (x-1.5)/(1.5*log(10))
		 * log10(d) = (i-Bias)*log(2)/log(10) + log10(d2)
		 *
		 * This suggests computing an approximation k to log10(d) by
		 *
		 * k = (i - Bias)*0.301029995663981
		 *	+ ( (d2-1.5)*0.289529654602168 + 0.176091259055681 );
		 *
		 * We want k to be too large rather than too small.
		 * The error in the first-order Taylor series approximation
		 * is in our favor, so we just round up the constant enough
		 * to compensate for any error in the multiplication of
		 * (i - Bias) by 0.301029995663981; since |i - Bias| <= 1077,
		 * and 1077 * 0.30103 * 2^-52 ~=~ 7.2e-14,
		 * adding 1e-13 to the constant term more than suffices.
		 * Hence we adjust the constant term to 0.1760912590558.
		 * (We could get a more accurate k by invoking log10,
		 *  but this is probably not worthwhile.)
		 */

      i -= Bias_00422780;
      denorm = 0;
    }
  else
    {
      /* d is denormalized */

      i = bbits + be + (Bias_00422780 + (P_00422780 - 1) - 1);
      x = (i > 32) ? (word0_00422780 (d) << (64 - i)) | (word1_00422780 (d) >> (i - 32))
       : (word1_00422780 (d) << (32 - i));
      d2.d = x;
      word0_00422780 (d2) -= 31 * Exp_msk1_00422780;	/* adjust exponent */
      i -= (Bias_00422780 + (P_00422780 - 1) - 1) + 1;
      denorm = 1;
    }
  ds = (d2.d - 1.5) * 0.289529654602168 + 0.1760912590558 + i * 0.301029995663981;
  k = (int) ds;
  if (ds < 0. && ds != k)
    k--;			/* want k = floor(ds) */
  k_check = 1;
  if (k >= 0 && k <= Ten_pmax_00422780)
    {
      if (d.d < tens_00422780[k])
	k--;
      k_check = 0;
    }
  j = bbits - i - 1;
  if (j >= 0)
    {
      b2 = 0;
      s2 = j;
    }
  else
    {
      b2 = -j;
      s2 = 0;
    }
  if (k >= 0)
    {
      b5 = 0;
      s5 = k;
      s2 += k;
    }
  else
    {
      b2 -= k;
      b5 = -k;
      s5 = 0;
    }
  if (mode < 0 || mode > 9)
    mode = 0;
  try_quick = 1;
  if (mode > 5)
    {
      mode -= 4;
      try_quick = 0;
    }
  leftright = 1;
  switch (mode)
    {
    case 0:
    case 1:
      ilim = ilim1 = -1;
      i = 18;
      ndigits = 0;
      break;
    case 2:
      leftright = 0;
      /* no break */
    case 4:
      if (ndigits <= 0)
	ndigits = 1;
      ilim = ilim1 = i = ndigits;
      break;
    case 3:
      leftright = 0;
      /* no break */
    case 5:
      i = ndigits + k + 1;
      ilim = i;
      ilim1 = i - 1;
      if (i <= 0)
	i = 1;
    }
  j = sizeof (unsigned int);
  for (ptr->_result_k = 0; sizeof (struct Bigint) - sizeof (unsigned int) + j <= i;
       j <<= 1)
    ptr->_result_k++;
  ptr->_result = g2780_Balloc (ptr, ptr->_result_k);
  s = s0 = (char *) ptr->_result;

  if (ilim >= 0 && ilim <= Quick_max_00422780 && try_quick)
    {
      /* Try to get by with floating-point arithmetic. */

      i = 0;
      d2.d = d.d;
      k0 = k;
      ilim0 = ilim;
      ieps = 2;			/* conservative */
      if (k > 0)
	{
	  ds = tens_00422780[k & 0xf];
	  j = k >> 4;
	  if (j & Bletch_00422780)
	    {
	      /* prevent overflows */
	      j &= Bletch_00422780 - 1;
	      d.d /= bigtens_00422780[n_bigtens_00422780 - 1];
	      ieps++;
	    }
	  for (; j; j >>= 1, i++)
	    if (j & 1)
	      {
		ieps++;
		ds *= bigtens_00422780[i];
	      }
	  d.d /= ds;
	}
      else if ((j1 = -k) != 0)
	{
	  d.d *= tens_00422780[j1 & 0xf];
	  for (j = j1 >> 4; j; j >>= 1, i++)
	    if (j & 1)
	      {
		ieps++;
		d.d *= bigtens_00422780[i];
	      }
	}
      if (k_check && d.d < 1. && ilim > 0)
	{
	  if (ilim1 <= 0)
	    goto fast_failed;
	  ilim = ilim1;
	  k--;
	  d.d *= 10.;
	  ieps++;
	}
      eps.d = ieps * d.d + 7.;
      word0_00422780 (eps) -= (P_00422780 - 1) * Exp_msk1_00422780;
      if (ilim == 0)
	{
	  S = mhi = 0;
	  d.d -= 5.;
	  if (d.d > eps.d)
	    goto one_digit;
	  if (d.d < -eps.d)
	    goto no_digits;
	  goto fast_failed;
	}
      if (leftright)
	{
	  /* Use Steele & White method of only
	   * generating digits needed.
	   */
	  eps.d = 0.5 / tens_00422780[ilim - 1] - eps.d;
	  for (i = 0;;)
	    {
	      L = d.d;
	      d.d -= L;
	      *s++ = '0' + (int) L;
	      if (d.d < eps.d)
		goto ret1;
	      if (1. - d.d < eps.d)
		goto bump_up;
	      if (++i >= ilim)
		break;
	      eps.d *= 10.;
	      d.d *= 10.;
	    }
	}
      else
	{
	  /* Generate ilim digits, then fix them up. */
	  eps.d *= tens_00422780[ilim - 1];
	  for (i = 1;; i++, d.d *= 10.)
	    {
	      L = d.d;
	      d.d -= L;
	      *s++ = '0' + (int) L;
	      if (i == ilim)
		{
		  if (d.d > 0.5 + eps.d)
		    goto bump_up;
		  else if (d.d < 0.5 - eps.d)
		    {
		      while (*--s == '0');
		      s++;
		      goto ret1;
		    }
		  break;
		}
	    }
	}
    fast_failed:
      s = s0;
      d.d = d2.d;
      k = k0;
      ilim = ilim0;
    }

  /* Do we have a "small" integer? */

  if (be >= 0 && k <= Int_max_00422780)
    {
      /* Yes. */
      ds = tens_00422780[k];
      if (ndigits < 0 && ilim <= 0)
	{
	  S = mhi = 0;
	  if (ilim < 0 || d.d <= 5 * ds)
	    goto no_digits;
	  goto one_digit;
	}
      for (i = 1;; i++)
	{
	  L = d.d / ds;
	  d.d -= L * ds;
	  *s++ = '0' + (int) L;
	  if (i == ilim)
	    {
	      d.d += d.d;
             if ((d.d > ds) || ((d.d == ds) && (L & 1)))
		{
		bump_up:
		  while (*--s == '9')
		    if (s == s0)
		      {
			k++;
			*s = '0';
			break;
		      }
		  ++*s++;
		}
	      break;
	    }
	  if (!(d.d *= 10.))
	    break;
	}
      goto ret1;
    }

  m2 = b2;
  m5 = b5;
  mhi = mlo = 0;
  if (leftright)
    {
      if (mode < 2)
	{
	  i =
	    denorm ? be + (Bias_00422780 + (P_00422780 - 1) - 1 + 1) :
	    1 + P_00422780 - bbits;
	}
      else
	{
	  j = ilim - 1;
	  if (m5 >= j)
	    m5 -= j;
	  else
	    {
	      s5 += j -= m5;
	      b5 += j;
	      m5 = 0;
	    }
	  if ((i = ilim) < 0)
	    {
	      m2 -= i;
	      i = 0;
	    }
	}
      b2 += i;
      s2 += i;
      mhi = g2780_i2b (ptr, 1);
    }
  if (m2 > 0 && s2 > 0)
    {
      i = m2 < s2 ? m2 : s2;
      b2 -= i;
      m2 -= i;
      s2 -= i;
    }
  if (b5 > 0)
    {
      if (leftright)
	{
	  if (m5 > 0)
	    {
	      mhi = g2780_pow5mult (ptr, mhi, m5);
	      b1 = g2780_mult (ptr, mhi, b);
	      g2780_Bfree (ptr, b);
	      b = b1;
	    }
         if ((j = b5 - m5) != 0)
	    b = g2780_pow5mult (ptr, b, j);
	}
      else
	b = g2780_pow5mult (ptr, b, b5);
    }
  S = g2780_i2b (ptr, 1);
  if (s5 > 0)
    S = g2780_pow5mult (ptr, S, s5);

  /* Check for special case that d is a normalized power of 2. */

  if (mode < 2)
    {
      if (!word1_00422780 (d) && !(word0_00422780 (d) & Bndry_mask_00422780)
	  && word0_00422780 (d) & Exp_mask_00422780
	)
	{
	  /* The special case */
	  b2 += Log2P_00422780;
	  s2 += Log2P_00422780;
	  spec_case = 1;
	}
      else
	spec_case = 0;
    }

  /* Arrange for convenient computation of quotients:
   * shift left if necessary so divisor has 4 leading 0 bits.
   *
   * Perhaps we should just compute leading 28 bits of S once
   * and for all and pass them and a shift to quorem, so it
   * can do shifts and ors to compute the numerator for q.
   */

  if ((i = ((s5 ? 32 - g2780_hi0bits (S->x[S->wds - 1]) : 1) + s2) & 0x1f) != 0)
    i = 32 - i;
  if (i > 4)
    {
      i -= 4;
      b2 += i;
      m2 += i;
      s2 += i;
    }
  else if (i < 4)
    {
      i += 28;
      b2 += i;
      m2 += i;
      s2 += i;
    }
  if (b2 > 0)
    b = g2780_lshift (ptr, b, b2);
  if (s2 > 0)
    S = g2780_lshift (ptr, S, s2);
  if (k_check)
    {
      if (g2780_cmp (b, S) < 0)
	{
	  k--;
	  b = g2780_multadd (ptr, b, 10, 0);	/* we botched the k estimate */
	  if (leftright)
	    mhi = g2780_multadd (ptr, mhi, 10, 0);
	  ilim = ilim1;
	}
    }
  if (ilim <= 0 && mode > 2)
    {
      if (ilim < 0 || g2780_cmp (b, S = g2780_multadd (ptr, S, 5, 0)) <= 0)
	{
	  /* no digits, fcvt style */
	no_digits:
	  k = -1 - ndigits;
	  goto ret;
	}
    one_digit:
      *s++ = '1';
      k++;
      goto ret;
    }
  if (leftright)
    {
      if (m2 > 0)
	mhi = g2780_lshift (ptr, mhi, m2);

      /* Compute mlo -- check for special case
       * that d is a normalized power of 2.
       */

      mlo = mhi;
      if (spec_case)
	{
	  mhi = g2780_Balloc (ptr, mhi->k);
	  Bcopy_00422780 (mhi, mlo);
	  mhi = g2780_lshift (ptr, mhi, Log2P_00422780);
	}

      for (i = 1;; i++)
	{
	  dig = func_00422560 (b, S) + '0';
	  /* Do we yet have the shortest decimal string
	   * that will round to d?
	   */
	  j = g2780_cmp (b, mlo);
	  delta = g2780_diff (ptr, S, mhi);
	  j1 = delta->sign ? 1 : g2780_cmp (b, delta);
	  g2780_Bfree (ptr, delta);
	  if (j1 == 0 && !mode && !(word1_00422780 (d) & 1))
	    {
	      if (dig == '9')
		goto round_9_up;
	      if (j > 0)
		dig++;
	      *s++ = dig;
	      goto ret;
	    }
         if ((j < 0) || ((j == 0) && !mode
	      && !(word1_00422780 (d) & 1)
           ))
	    {
	      if (j1 > 0)
		{
		  b = g2780_lshift (ptr, b, 1);
		  j1 = g2780_cmp (b, S);
                 if (((j1 > 0) || ((j1 == 0) && (dig & 1)))
		      && dig++ == '9')
		    goto round_9_up;
		}
	      *s++ = dig;
	      goto ret;
	    }
	  if (j1 > 0)
	    {
	      if (dig == '9')
		{		/* possible if i == 1 */
		round_9_up:
		  *s++ = '9';
		  goto roundoff;
		}
	      *s++ = dig + 1;
	      goto ret;
	    }
	  *s++ = dig;
	  if (i == ilim)
	    break;
	  b = g2780_multadd (ptr, b, 10, 0);
	  if (mlo == mhi)
	    mlo = mhi = g2780_multadd (ptr, mhi, 10, 0);
	  else
	    {
	      mlo = g2780_multadd (ptr, mlo, 10, 0);
	      mhi = g2780_multadd (ptr, mhi, 10, 0);
	    }
	}
    }
  else
    for (i = 1;; i++)
      {
	*s++ = dig = func_00422560 (b, S) + '0';
	if (i >= ilim)
	  break;
	b = g2780_multadd (ptr, b, 10, 0);
      }

  /* Round off last digit */

  b = g2780_lshift (ptr, b, 1);
  j = g2780_cmp (b, S);
  if ((j > 0) || ((j == 0) && (dig & 1)))
    {
    roundoff:
      while (*--s == '9')
	if (s == s0)
	  {
	    k++;
	    *s++ = '1';
	    goto ret;
	  }
      ++*s++;
    }
  else
    {
      while (*--s == '0');
      s++;
    }
ret:
  g2780_Bfree (ptr, S);
  if (mhi)
    {
      if (mlo && mlo != mhi)
	g2780_Bfree (ptr, mlo);
      g2780_Bfree (ptr, mhi);
    }
ret1:
  g2780_Bfree (ptr, b);
  *s = 0;
  *decpt = k + 1;
  if (rve)
    *rve = s;
  return s0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00423980);
#ifdef SKIP_ASM
// PORT: PS2-only kernel syscall 0 stub (gcc writes the zero as move $3,$0, the stub has li $3,0); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
void func_00423980(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "addiu $3, $0, 0x0\n"
        "syscall 0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00423990);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423990, 0x1)
#endif

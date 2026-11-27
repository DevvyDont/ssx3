#ifndef SEG_NEWLIB_REENT_H
#define SEG_NEWLIB_REENT_H

// newlib 1.9.0's reentrancy structure and the stdio FILE it embeds, as the game's C library (src/seg/seg_316FB8.c:
// __sfp, __sinit, __smakebuf, _sprintf_r, _mbtowc_r, _dtoa_r's callers, ...) uses them (views group D: R17008,
// R21530, R216E8, _reent_0010, _reent_0180, _reent_1B98, _reent_8958 -> struct _reent; F17008, F174B0, F177B0,
// __sFILE_0010 -> struct __sFILE; G17418, _glue_0010 -> struct _glue; wchar_t_8958 -> wchar_t).
//
// [L] refs/newlib-1.9.0-src/newlib-1.9.0/newlib/libc/include/sys/reent.h (the structs, verbatim apart from the
// comments and the two EE notes below) and libc/include/stdio.h (the __S* flags). sys/reent.h has no notice of its
// own; newlib's COPYING.NEWLIB says each source carries its own. The stdio layout is Berkeley's; stdio.h's notice:
//
//   Copyright (c) 1990 The Regents of the University of California.
//   All rights reserved.
//
//   Redistribution and use in source and binary forms are permitted
//   provided that the above copyright notice and this paragraph are
//   duplicated in all such forms and that any documentation,
//   advertising materials, and other materials related to such
//   distribution and use acknowledge that the software was developed
//   by the University of California, Berkeley.  The name of the
//   University may not be used to endorse or promote products derived
//   from this software without specific prior written permission.
//   THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
//   IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// EE notes ([R]): `long` is 64-bit on the EE (include/gcc/stdint.h: typedef long int64_t), so newlib's __ULong is
// `unsigned int` (__LONG_MAX__ is not 2^31 - 1, __INT_MAX__ is) and _fpos_t (long) is 8 bytes (only _seek's return
// type uses it). The unit's own copy of struct _reent puts __sglue at 0x1D8 ("newlib 1.9's struct _reent up to
// __sglue (0x1d8)"), and retail's impure_data (0x00454658, _impure_ptr's target) is followed by _impure_ptr at
// 0x00454944, i.e. struct _reent is 0x2EC bytes: 4-byte aligned, so nothing in the _new union may need 8. newlib's
// `__extension__ unsigned long long _rand_next` is therefore written as a packed `unsigned long` (8 bytes on the EE,
// alignment 1 as a packed member); no retail code in this unit reads it.
// Offsets in the comments are the EE's; CHECK lines hold where pointer / long sizes do not change them.

#include "layout.h"

#define _ATEXIT_SIZE 32             // [L] must be at least 32 to guarantee ANSI conformance

struct _reent;
struct __sFILE;

// [L] struct _glue: a block of FILEs in the glue chain.
struct _glue
{
  struct _glue *_next;              // 0x0 [L]
  int _niobs;                       // 0x4 [L]
  struct __sFILE *_iobs;            // 0x8 [L]
};

// [L] struct _Bigint (mprec).
struct _Bigint
{
  struct _Bigint *_next;            // 0x00 [L]
  int _k, _maxwds, _sign, _wds;     // 0x04 [L]
  unsigned int _x[1];               // 0x14 [L] __ULong (unsigned int on the EE)
};

// [L] atexit() support.
struct _atexit {
  struct _atexit *_next;            // 0x00 [L] next in list
  int _ind;                         // 0x04 [L] next index in this table
  void (*_fns[_ATEXIT_SIZE])(void); // 0x08 [L] the table itself
};

// [L] struct __sbuf: a stdio buffer.
struct __sbuf {
  unsigned char *_base;             // 0x0 [L]
  int _size;                        // 0x4 [L]
};

typedef long _fpos_t;               // [L] (8 bytes on the EE)

// [L] struct __sFILE (0x58 on the EE): stdio state.
struct __sFILE {
  unsigned char *_p;                // 0x00 [L] current position in (some) buffer
  int _r;                           // 0x04 [L] read space left for getc()
  int _w;                           // 0x08 [L] write space left for putc()
  short _flags;                     // 0x0C [L] flags (__S* below); this FILE is free if 0
  short _file;                      // 0x0E [L] fileno, if Unix descriptor, else -1
  struct __sbuf _bf;                // 0x10 [L] the buffer (at least 1 byte, if !NULL)
  int _lbfsize;                     // 0x18 [L] 0 or -_bf._size, for inline putc
  void *_cookie;                    // 0x1C [L] cookie passed to io functions
  int (*_read)(void *_cookie, char *_buf, int _n);              // 0x20 [L]
  int (*_write)(void *_cookie, const char *_buf, int _n);       // 0x24 [L]
  _fpos_t (*_seek)(void *_cookie, _fpos_t _offset, int _whence); // 0x28 [L]
  int (*_close)(void *_cookie);     // 0x2C [L]
  struct __sbuf _ub;                // 0x30 [L] ungetc buffer
  unsigned char *_up;               // 0x38 [L] saved _p when _p is doing ungetc data
  int _ur;                          // 0x3C [L] saved _r when _r is counting ungetc data
  unsigned char _ubuf[3];           // 0x40 [L] guarantee an ungetc() buffer
  unsigned char _nbuf[1];           // 0x43 [L] guarantee a getc() buffer
  struct __sbuf _lb;                // 0x44 [L] buffer for fgetline()
  int _blksize;                     // 0x4C [L] stat.st_blksize (may be != _bf._size)
  int _offset;                      // 0x50 [L] current lseek offset
  struct _reent *_data;             // 0x54 [L]
};

// [L] stdio.h's __sFILE flags (_sprintf_r sets __SWR | __SSTR = 0x208).
#define __SLBF  0x0001              // line buffered
#define __SNBF  0x0002              // unbuffered
#define __SRD   0x0004              // OK to read
#define __SWR   0x0008              // OK to write
#define __SRW   0x0010              // open for reading & writing
#define __SEOF  0x0020              // found EOF
#define __SERR  0x0040              // found error
#define __SMBF  0x0080              // _buf is from malloc
#define __SAPP  0x0100              // fdopen()ed in append mode
#define __SSTR  0x0200              // this is an sprintf/snprintf string
#define __SOPT  0x0400              // do fseek() optimisation
#define __SNPT  0x0800              // do not do fseek() optimisation
#define __SOFF  0x1000              // set iff _offset is in fact correct
#define __SMOD  0x2000              // true => fgetline modified _p text

// [L] stdio.h: the end-of-file / error result and fseek's whence values (sys/unistd.h has the same three for lseek).
#define EOF     (-1)
#define SEEK_SET 0                  // set file offset to offset
#define SEEK_CUR 1                  // set file offset to current plus offset
#define SEEK_END 2                  // set file offset to EOF plus offset

// [L] stdio/fseek.c: a seek callback's failure result (stdio.c's __sseek spells the same value -1L).
#define POS_ERR (-(_fpos_t)1)

// The -1 a POSIX-style system call (_close, _fstat, _lseek, _open, _read, _write) returns on failure: newlib's reent
// wrappers ([L] reent/closer.c, fstatr.c, ...: `(ret = _close (fd)) == -1 && errno != 0`) test it (name ours).
#define SYSCALL_FAILED (-1)

// [L] sys/errno.h: the errno values the library stores.
#define EINVAL 22                   // Invalid argument
#define ESPIPE 29                   // Illegal seek
#define ERANGE 34                   // Math result not representable

// [L] locale.h: setlocale's categories (this library handles LC_ALL and LC_CTYPE only).
#define LC_ALL      0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5

// _dtoa_r's conversion modes: [L] stdlib/dtoa.c's comment above _dtoa_r numbers them (0 shortest, 1 shortest with
// the Steele & White stopping rule, 2 ndigits significant digits as ecvt, 3 ndigits past the point as fcvt, 4 and 5 as
// 2 and 3 with left-to-right digit generation; 6-9 as 2-5 without the floating-point estimate). Names ours.
enum dtoa_mode
{
  DTOA_SHORTEST = 0,
  DTOA_SHORTEST_STEELE_WHITE = 1,
  DTOA_SIGNIFICANT_DIGITS = 2,
  DTOA_FRACTION_DIGITS = 3,
  DTOA_SIGNIFICANT_DIGITS_LTR = 4,
  DTOA_FRACTION_DIGITS_LTR = 5
};

// The *decpt _dtoa_r gives an infinity or NaN: [L] dtoa.c "If d is +-Infinity or NaN, then *decpt is set to 9999"
// (ecvtbuf.c's print_f / print_e / _gcvt test `decpt == 9999`). Name ours.
#define DTOA_DECPT_INF_NAN 9999

// [L] newlib time.h's struct tm (inside _reent's _new union).
struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

#define _N_LISTS 30                 // [L]

// [L] struct _reent: all the library's per-thread state (0x2EC on the EE).
struct _reent
{
  int _errno;                       // 0x000 [L] local copy of errno; [U] __sflags / __sfp store it
  struct __sFILE *_stdin, *_stdout, *_stderr; // 0x004 [L] pointers into __sf
  int _inc;                         // 0x010 [L] used by tmpnam
  char _emergency[25];              // 0x014 [L]
  int _current_category;            // 0x030 [L] used by setlocale
  const char *_current_locale;      // 0x034 [L]
  int __sdidinit;                   // 0x038 [L] 1 means stdio has been init'd; [U] __sfp tests it
  void (*__cleanup)(struct _reent *); // 0x03C [L]
  struct _Bigint *_result;          // 0x040 [L] used by mprec routines
  int _result_k;                    // 0x044 [L]
  struct _Bigint *_p5s;             // 0x048 [L]
  struct _Bigint **_freelist;       // 0x04C [L]
  int _cvtlen;                      // 0x050 [L] used by some fp conversion routines
  char *_cvtbuf;                    // 0x054 [L]
  union
    {
      struct
        {
          unsigned int _unused_rand;
          char * _strtok_last;
          char _asctime_buf[26];
          struct tm _localtime_buf;
          int _gamma_signgam;
          unsigned long _rand_next __attribute__ ((packed)); // [L] unsigned long long; see the EE notes
        } _reent;
      struct
        {
          unsigned char * _nextf[_N_LISTS];
          unsigned int _nmalloc[_N_LISTS];
        } _unused;
    } _new;                         // 0x058 [L] (0xF0)
  struct _atexit *_atexit;          // 0x148 [L] points to head of LIFO stack
  struct _atexit _atexit0;          // 0x14C [L] one guaranteed table, required by ANSI
  void (**(_sig_func))(int);        // 0x1D4 [L] signal info
  struct _glue __sglue;             // 0x1D8 [L] root of glue chain; [R] the unit's copy places it here; [U] __sfp
  struct __sFILE __sf[3];           // 0x1E4 [L] first three file descriptors
};

// wchar_t: [L] _mbtowc_r(struct _reent *r, wchar_t *pwc, const char *s, size_t n, int *state) (mbtowc_r.c);
// [R] the unit's typedef is unsigned short (16-bit stores). C only: wchar_t is a keyword in C++.
#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

// The layout checks name the types with their struct keyword (newlib has no typedefs for them, and the units use
// `struct _reent`), so CHECK_OFFSET / CHECK_SIZE (which paste the name) are spelled out.
#ifndef DECOMP_NO_LAYOUT_CHECKS
typedef char check___sbuf__size[(DECOMP_OFFSETOF(struct __sbuf, _size) == 0x4) ? 1 : -1];
typedef char check___sFILE__flags[(DECOMP_OFFSETOF(struct __sFILE, _flags) == 0x0C) ? 1 : -1];
typedef char check___sFILE__file[(DECOMP_OFFSETOF(struct __sFILE, _file) == 0x0E) ? 1 : -1];
typedef char check___sFILE__bf[(DECOMP_OFFSETOF(struct __sFILE, _bf) == 0x10) ? 1 : -1];
typedef char check___sFILE__seek[(DECOMP_OFFSETOF(struct __sFILE, _seek) == 0x28) ? 1 : -1];
typedef char check___sFILE__ubuf[(DECOMP_OFFSETOF(struct __sFILE, _ubuf) == 0x40) ? 1 : -1];
typedef char check___sFILE__nbuf[(DECOMP_OFFSETOF(struct __sFILE, _nbuf) == 0x43) ? 1 : -1];
typedef char check___sFILE__blksize[(DECOMP_OFFSETOF(struct __sFILE, _blksize) == 0x4C) ? 1 : -1];
typedef char check___sFILE__offset[(DECOMP_OFFSETOF(struct __sFILE, _offset) == 0x50) ? 1 : -1];
typedef char check___sFILE__data[(DECOMP_OFFSETOF(struct __sFILE, _data) == 0x54) ? 1 : -1];
typedef char check___sFILE_size[(sizeof(struct __sFILE) == 0x58) ? 1 : -1];
typedef char check__glue__iobs[(DECOMP_OFFSETOF(struct _glue, _iobs) == 0x8) ? 1 : -1];
typedef char check__glue_size[(sizeof(struct _glue) == 0xC) ? 1 : -1];
typedef char check__atexit_size[(sizeof(struct _atexit) == 0x88) ? 1 : -1];
typedef char check__reent__inc[(DECOMP_OFFSETOF(struct _reent, _inc) == 0x010) ? 1 : -1];
typedef char check__reent__emergency[(DECOMP_OFFSETOF(struct _reent, _emergency) == 0x014) ? 1 : -1];
typedef char check__reent__current_category[(DECOMP_OFFSETOF(struct _reent, _current_category) == 0x030) ? 1 : -1];
typedef char check__reent___sdidinit[(DECOMP_OFFSETOF(struct _reent, __sdidinit) == 0x038) ? 1 : -1];
typedef char check__reent__cvtbuf[(DECOMP_OFFSETOF(struct _reent, _cvtbuf) == 0x054) ? 1 : -1];
typedef char check__reent__new[(DECOMP_OFFSETOF(struct _reent, _new) == 0x058) ? 1 : -1];
typedef char check__reent__atexit[(DECOMP_OFFSETOF(struct _reent, _atexit) == 0x148) ? 1 : -1];
typedef char check__reent__atexit0[(DECOMP_OFFSETOF(struct _reent, _atexit0) == 0x14C) ? 1 : -1];
typedef char check__reent__sig_func[(DECOMP_OFFSETOF(struct _reent, _sig_func) == 0x1D4) ? 1 : -1];
typedef char check__reent___sglue[(DECOMP_OFFSETOF(struct _reent, __sglue) == 0x1D8) ? 1 : -1];
typedef char check__reent___sf[(DECOMP_OFFSETOF(struct _reent, __sf) == 0x1E4) ? 1 : -1];
typedef char check__reent_size[(sizeof(struct _reent) == 0x2EC) ? 1 : -1];
#endif

#endif /* SEG_NEWLIB_REENT_H */

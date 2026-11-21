/* GNU CC runtime support, reconstructed: the structures of libgcc2.c's L_eh exception-handling runtime and of frame.c
   (DWARF 2 frame unwinding) that the game's libgcc units use (views group D: seg/seg_3113A0_eh S410A78, F410C18,
   S410C18 -> frame_state; E410858 -> old_exception_table; E410930 -> exception_table; C410930 -> __eh_info;
   seg/seg_313F58 N13F38, N13F80 -> fde; L13F80 -> fde_accumulator).
   [L] refs/gcc-2.95.3/gcc/frame.h (frame_state), eh-common.h (the exception tables, __eh_info), frame.c (struct
   dwarf_fde / fde, fde_vector, fde_accumulator); member names and types are the library's.
   Original: frame.h and eh-common.h Copyright (C) 1997, 1998 Free Software Foundation, Inc.; frame.c Copyright (C)
   1997, 1998, 2000 Free Software Foundation, Inc. (Contributed by Jason Merrill <jason@cygnus.com>.)
   It is free software under the GNU General Public License, version 2 or (at your option) any later version, with
   this special exception (frame.c): "if you link this library with other files, some of which are compiled with GCC,
   to produce an executable, this library does not by itself cause the resulting executable to be covered by the
   GNU General Public License. This exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License."
   This file stays under those terms; the repository's CC0 dedication does not apply to it (README, License).

   EE notes ([R]): `long` is 64-bit on the EE, so frame_state's cfa_offset / args_size / reg_or_offset[] are 8-byte
   words and reg_or_offset starts at 0x18; the EE compiler has 0x50 register slots (FIRST_PSEUDO_REGISTER + 1 = 0x50,
   stock mips.h's 76 + 1 is 77): [U] get_reg_addr / next_stack_level / copy_reg read cfa_reg at 0x298 and saved[] at
   0x29C (view S410A78: e[0x50] at 0x18, n at 0x298, kind[] at 0x29C), so the struct is 0x2F0. */

#ifndef SEG_LIBGCC_FRAME_H
#define SEG_LIBGCC_FRAME_H

#include "layout.h"

#define FRAME_STATE_NREGS 0x50      /* FIRST_PSEUDO_REGISTER + 1 on the EE ([R], see above) */

/* [L] frame.h. */
typedef struct frame_state
{
  void *cfa;                        /* 0x000 [L]; [U] the frame's canonical frame address */
  void *eh_ptr;                     /* 0x004 [L] */
  long cfa_offset;                  /* 0x008 [L] */
  long args_size;                   /* 0x010 [L] */
  long reg_or_offset[FRAME_STATE_NREGS]; /* 0x018 [L]; [U] get_reg_addr: the save slot offset or register */
  unsigned short cfa_reg;           /* 0x298 [L] */
  unsigned short retaddr_column;    /* 0x29A [L] */
  char saved[FRAME_STATE_NREGS];    /* 0x29C [L]; [U] REG_SAVED_OFFSET / REG_SAVED_REG */
} frame_state;

/* Values for 'saved' above.  [L] */
#define REG_UNSAVED 0
#define REG_SAVED_OFFSET 1
#define REG_SAVED_REG 2

/* [L] eh-common.h. */
typedef struct old_exception_table
{
  void *start_region;               /* 0x0 [L] */
  void *end_region;                 /* 0x4 [L] */
  void *exception_handler;          /* 0x8 [L]; [U] old_find_exception_handler returns it */
} old_exception_table;

typedef struct exception_table
{
  void *start_region;               /* 0x0 [L] */
  void *end_region;                 /* 0x4 [L] */
  void *exception_handler;          /* 0x8 [L] */
  void *match_info;                 /* 0xC [L] runtime type info */
} exception_table;

typedef struct exception_lang_info
{
  short language;                   /* 0x0 [L] */
  short version;                    /* 0x2 [L] */
} exception_lang_info;

/* [L] the new-model table: find_exception_handler reads table[] after the 8-byte header. */
typedef struct exception_descriptor
{
  void *runtime_id_field;           /* 0x0 [L] NEW_EH_RUNTIME (-2) */
  exception_lang_info lang;         /* 0x4 [L] */
  exception_table table[1];         /* 0x8 [L] */
} exception_descriptor;

typedef void * (*__eh_matcher) (void *, void *, void *); /* [L] */

typedef struct __eh_info
{
  __eh_matcher match_function;      /* 0x0 [L]; [U] find_exception_handler calls it (view C410930::match) */
  short language;                   /* 0x4 [L] */
  short version;                    /* 0x6 [L] */
} __eh_info;

/* [L] frame.c. */
typedef          int  sword __attribute__ ((mode (SI)));
typedef unsigned int  uword __attribute__ ((mode (SI)));
typedef unsigned int  uaddr __attribute__ ((mode (pointer)));

struct dwarf_fde {
  uword length;                     /* 0x0 [L]; [U] count_fdes / add_fdes: the next FDE is at this + length + 4 */
  sword CIE_delta;                  /* 0x4 [L]; [U] 0 marks a CIE (skipped) */
  void* pc_begin;                   /* 0x8 [L]; [U] add_fdes */
  uaddr pc_range;                   /* 0xC [L]; [U] add_fdes */
} __attribute__ ((packed, aligned (__alignof__ (void *))));

typedef struct dwarf_fde fde;

typedef struct fde_vector
{
  fde **array;                      /* 0x0 [L] */
  unsigned int count;               /* 0x4 [L] size_t */
} fde_vector;

typedef struct fde_accumulator
{
  fde_vector linear;                /* 0x0 [L]; [U] add_fdes -> fde_insert appends here (view L13F80 {arr, cnt}) */
  fde_vector erratic;               /* 0x8 [L] */
} fde_accumulator;

CHECK_OFFSET(frame_state, cfa_offset, 0x008);
CHECK_OFFSET(frame_state, args_size, 0x010);
CHECK_OFFSET(frame_state, reg_or_offset, 0x018);
CHECK_OFFSET(frame_state, cfa_reg, 0x298);
CHECK_OFFSET(frame_state, retaddr_column, 0x29A);
CHECK_OFFSET(frame_state, saved, 0x29C);
CHECK_SIZE(frame_state, 0x2F0);
CHECK_OFFSET(old_exception_table, exception_handler, 0x8);
CHECK_SIZE(old_exception_table, 0xC);
CHECK_OFFSET(exception_table, match_info, 0xC);
CHECK_SIZE(exception_table, 0x10);
CHECK_OFFSET(exception_descriptor, table, 0x8);
CHECK_OFFSET(__eh_info, language, 0x4);
CHECK_SIZE(__eh_info, 0x8);
CHECK_OFFSET(fde, pc_begin, 0x8);
CHECK_OFFSET(fde, pc_range, 0xC);
CHECK_SIZE(fde, 0x10);
CHECK_OFFSET(fde_accumulator, erratic, 0x8);
CHECK_SIZE(fde_accumulator, 0x10);

#endif /* SEG_LIBGCC_FRAME_H */

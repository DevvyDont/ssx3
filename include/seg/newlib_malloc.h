#ifndef SEG_NEWLIB_MALLOC_H
#define SEG_NEWLIB_MALLOC_H

/* The chunk header of newlib's malloc (Doug Lea's malloc 2.6.4, libc/stdlib/mallocr.c), which the game's _malloc_r /
   _free_r / malloc_extend_top in seg/seg_316FB8 are (views group D: mbinptr_0041DBF8; the unit's former copy, `struct
   mchunk` / mchunkptr, was the same struct under a shorter name).
   [L] refs/newlib-1.9.0-src/newlib-1.9.0/newlib/libc/stdlib/mallocr.c ("A version of malloc/free/realloc written by
   Doug Lea and released to the public domain."): struct malloc_chunk {INTERNAL_SIZE_T prev_size, size; struct
   malloc_chunk *fd, *bk;}, typedef struct malloc_chunk* mchunkptr / mbinptr; INTERNAL_SIZE_T is size_t (32-bit on
   the EE). [R] the unit's bin_at / top / last_remainder macros (bins at &av_[2 * i + 2] - 8, i.e. fd at 0x8) and
   unlink (fd 0x8, bk 0xC); chunksize masks size (0x4) with ~3. */

#include "layout.h"

struct malloc_chunk
{
  unsigned int prev_size;           /* 0x0 [L] size of previous chunk (if free) */
  unsigned int size;                /* 0x4 [L] size in bytes, including overhead (bit 0 PREV_INUSE) */
  struct malloc_chunk* fd;          /* 0x8 [L] double links -- used only if free */
  struct malloc_chunk* bk;          /* 0xC [L] */
};

typedef struct malloc_chunk* mchunkptr; /* [L] */
typedef struct malloc_chunk* mbinptr;   /* [L] a bin header (only fd / bk are real; see bin_at) */

#ifndef DECOMP_NO_LAYOUT_CHECKS
typedef char check_malloc_chunk_size[(sizeof(struct malloc_chunk) == 0x10) ? 1 : -1];
typedef char check_malloc_chunk_fd[(DECOMP_OFFSETOF(struct malloc_chunk, fd) == 0x8) ? 1 : -1];
typedef char check_malloc_chunk_bk[(DECOMP_OFFSETOF(struct malloc_chunk, bk) == 0xC) ? 1 : -1];
#endif

#endif /* SEG_NEWLIB_MALLOC_H */

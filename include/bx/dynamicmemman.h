#ifndef BX_DYNAMICMEMMAN_H
#define BX_DYNAMICMEMMAN_H

// cDynamicMemMan: EA's dlmalloc-based dynamic heap (the global TheDynamicMemMan). Its constructor
// cDynamicMemMan_cDynamicMemMan (bx/cubicspline) zeroes exactly these four words; the size is On Tour's (no retail
// allocation: it is a global).
// Size 0x10 (On Tour's; retail's constructor writes 0x0-0xC). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

// [Staged by group J: adds dlmalloc 2.7's malloc_chunk and malloc_state (On Tour structs of the same names) before
// cDynamicMemMan and drops the malloc_state forward declaration; cDynamicMemMan is unchanged.]
//
// malloc_chunk / malloc_state: Doug Lea's dlmalloc 2.7 (public domain), which cDynamicMemMan wraps (the
// cDynamicMemMan_dl* / sYSMALLOc / sYSTRIm / malloc_consolidate functions in bx/cubicspline take one as `av`).
// Member names, order and offsets: [O] SSX On Tour's DWARF (struct malloc_chunk 0x10, struct malloc_state 0x378),
// equal to dlmalloc 2.7's declarations; every retail access in bx/cubicspline falls on a member start.

// A chunk header (dlmalloc's struct malloc_chunk; a free chunk's fd/bk link it into a bin).
struct malloc_chunk {
    unsigned int prev_size;         // 0x000 [O]; [U] sYSMALLOc, dlfree (sMChunk_31E2D8::prev_size)
    unsigned int size;              // 0x004 [O] size | PREV_INUSE (1) | IS_MMAPPED (2); [U] every dl* function
    malloc_chunk* fd;               // 0x008 [O]; [U] bins / fastbins links; malloc_init_state points it at the bin
    malloc_chunk* bk;               // 0x00C [O]; [U] ditto (bins only)
};

CHECK_OFFSET(malloc_chunk, prev_size, 0x000);
CHECK_OFFSET(malloc_chunk, size, 0x004);
CHECK_OFFSET(malloc_chunk, fd, 0x008);
CHECK_OFFSET(malloc_chunk, bk, 0x00C);
CHECK_SIZE(malloc_chunk, 0x10);

// The heap's state (dlmalloc's struct malloc_state, `mstate av`): it sits at the start of the heap block
// (cDynamicMemMan::MallocState = the 16-aligned base). Size 0x378: [R] cDynamicMemMan_init clears it with
// MEM_fill(start, 0, 888) and starts the free area 0x380 bytes in; [O] On Tour 0x378.
struct malloc_state {
    unsigned int max_fast;          // 0x000 [O] the fastbin limit, low bits FASTCHUNKS_BIT (1) / ANYCHUNKS_BIT (2);
                                    //       [R] malloc_init_state: (max_fast & 3) | 0x50; [U] malloc_consolidate
                                    //       calls malloc_init_state while it is 0
    malloc_chunk* fastbins[11];     // 0x004 [O]; [U] dlmalloc / dlfree / malloc_consolidate
    malloc_chunk* top;              // 0x030 [O]; [R] malloc_init_state stores self + 0x38 (initial_top);
                                    //       [U] sYSMALLOc, sYSTRIm, dlmalloc
    malloc_chunk* last_remainder;   // 0x034 [O]; [U] dlmalloc
    malloc_chunk* bins[192];        // 0x038 [O] NBINS * 2 (96 fd/bk pairs); [R] malloc_init_state's loop (95
                                    //       steps of 8 from self + 0x38) points each bin's fd/bk (+0x8/+0xC) at it
    unsigned int binmap[3];         // 0x338 [O] BINMAPSIZE; [U] dlmalloc
    unsigned int trim_threshold;    // 0x344 [O]; [R] malloc_init_state 0x8000; [U] dlfree, sYSTRIm
    unsigned int top_pad;           // 0x348 [O]; [R] malloc_init_state 0; [U] sYSMALLOc
    unsigned int mmap_threshold;    // 0x34C [O]; [R] malloc_init_state 0x40000
    int n_mmaps;                    // 0x350 [O]
    int n_mmaps_max;                // 0x354 [O]; [R] malloc_init_state 0 (no mmap on PS2)
    int max_n_mmaps;                // 0x358 [O]
    unsigned int pagesize;          // 0x35C [O]; [R] malloc_init_state 0x1000; [U] sYSMALLOc, sYSTRIm, dlvalloc
    unsigned int morecore_properties; // 0x360 [O] bit 0 MORECORE_CONTIGUOUS; [R] malloc_init_state sets bit 0;
                                    //       [U] sYSMALLOc
    unsigned int mmapped_mem;       // 0x364 [O]; [U] sYSMALLOc (sMState_31E2D8)
    unsigned int sbrked_mem;        // 0x368 [O]; [U] sYSMALLOc, sYSTRIm
    unsigned int max_sbrked_mem;    // 0x36C [O]; [U] sYSMALLOc
    unsigned int max_mmapped_mem;   // 0x370 [O]; [U] sYSMALLOc
    unsigned int max_total_mem;     // 0x374 [O]; [U] sYSMALLOc
};

CHECK_OFFSET(malloc_state, max_fast, 0x000);
CHECK_OFFSET(malloc_state, fastbins, 0x004);
CHECK_OFFSET(malloc_state, top, 0x030);
CHECK_OFFSET(malloc_state, last_remainder, 0x034);
CHECK_OFFSET(malloc_state, bins, 0x038);
CHECK_OFFSET(malloc_state, binmap, 0x338);
CHECK_OFFSET(malloc_state, trim_threshold, 0x344);
CHECK_OFFSET(malloc_state, top_pad, 0x348);
CHECK_OFFSET(malloc_state, mmap_threshold, 0x34C);
CHECK_OFFSET(malloc_state, n_mmaps, 0x350);
CHECK_OFFSET(malloc_state, n_mmaps_max, 0x354);
CHECK_OFFSET(malloc_state, max_n_mmaps, 0x358);
CHECK_OFFSET(malloc_state, pagesize, 0x35C);
CHECK_OFFSET(malloc_state, morecore_properties, 0x360);
CHECK_OFFSET(malloc_state, mmapped_mem, 0x364);
CHECK_OFFSET(malloc_state, sbrked_mem, 0x368);
CHECK_OFFSET(malloc_state, max_sbrked_mem, 0x36C);
CHECK_OFFSET(malloc_state, max_mmapped_mem, 0x370);
CHECK_OFFSET(malloc_state, max_total_mem, 0x374);
CHECK_SIZE(malloc_state, 0x378);

struct cDynamicMemMan {
    malloc_state* MallocState;      // 0x000 [O]
    char* Heap;                     // 0x004 [O]
    char* CoreTop;                  // 0x008 [O]
    unsigned int HeapSize;          // 0x00C [O]
};

CHECK_OFFSET(cDynamicMemMan, MallocState, 0x000);
CHECK_OFFSET(cDynamicMemMan, Heap, 0x004);
CHECK_OFFSET(cDynamicMemMan, CoreTop, 0x008);
CHECK_OFFSET(cDynamicMemMan, HeapSize, 0x00C);
CHECK_SIZE(cDynamicMemMan, 0x10);

// struct mallinfo (0x28 [O] On Tour's dlmalloc 2.7 struct mallinfo, the same ten ints as dlmalloc's own): the result
// of cDynamicMemMan_dlmallinfo (returned by value, [R] retail's dlmallinfo fills ten words). cMemMan_totalUnused /
// cMemMan_totalUsed read it.
struct mallinfo {
    int arena;                      // 0x000 [O]; [U] dlmallinfo: non-mmapped space allocated from the system
    int ordblks;                    // 0x004 [O]; [U] number of free chunks
    int smblks;                     // 0x008 [O]; [U] number of fastbin blocks
    int hblks;                      // 0x00C [O]; [U] number of mmapped regions
    int hblkhd;                     // 0x010 [O]; [U] space in mmapped regions
    int usmblks;                    // 0x014 [O]; [U] maximum total allocated space
    int fsmblks;                    // 0x018 [O]; [U] space in freed fastbin blocks
    int uordblks;                   // 0x01C [O]; [U] total allocated space
    int fordblks;                   // 0x020 [O]; [U] total free space (cMemMan_totalUnused)
    int keepcost;                   // 0x024 [O]; [U] releasable (top) space
};

CHECK_OFFSET(mallinfo, arena, 0x000);
CHECK_OFFSET(mallinfo, ordblks, 0x004);
CHECK_OFFSET(mallinfo, smblks, 0x008);
CHECK_OFFSET(mallinfo, hblks, 0x00C);
CHECK_OFFSET(mallinfo, hblkhd, 0x010);
CHECK_OFFSET(mallinfo, usmblks, 0x014);
CHECK_OFFSET(mallinfo, fsmblks, 0x018);
CHECK_OFFSET(mallinfo, uordblks, 0x01C);
CHECK_OFFSET(mallinfo, fordblks, 0x020);
CHECK_OFFSET(mallinfo, keepcost, 0x024);
CHECK_SIZE(mallinfo, 0x28);

#endif

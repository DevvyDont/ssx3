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

struct malloc_state;

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

#endif

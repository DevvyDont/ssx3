#ifndef BX_BLOCKMAN_H
#define BX_BLOCKMAN_H

// cBlockMan: a block allocator over one memory range: the range, its groups of blocks and the alignment. Methods in
// bx/blockman and bx/bigfile.
//
// Size 0x14 (On Tour's; retail's methods reach Allignment at size - 4). Member names, order and offsets are EA's, from
// SSX On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"

struct tBlockGroup;

struct cBlockMan {
    char* Memory;                   // 0x000 [O]
    int MemorySize;                 // 0x004 [O]
    tBlockGroup* Groups;            // 0x008 [O]
    int NumGroups;                  // 0x00C [O]
    int Allignment;                 // 0x010 [O]
};

CHECK_OFFSET(cBlockMan, Memory, 0x000);
CHECK_OFFSET(cBlockMan, MemorySize, 0x004);
CHECK_OFFSET(cBlockMan, Groups, 0x008);
CHECK_OFFSET(cBlockMan, NumGroups, 0x00C);
CHECK_OFFSET(cBlockMan, Allignment, 0x010);
CHECK_SIZE(cBlockMan, 0x14);

#endif

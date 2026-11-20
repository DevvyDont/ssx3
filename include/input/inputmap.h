#ifndef INPUT_INPUTMAP_H
#define INPUT_INPUTMAP_H

// cInputMap: a compiled input map (input/inputmap): offsets of up to MaxMaps maps into an op-code buffer built by its
// cInputMapParser. Methods: init, loadMapFile, compileMap, getState, getValue, purgeMapFile, purge.
//
// Size 0x18 ([R] the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names, order
// and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods falls
// on a member start (tools/gen/self_offsets.py).

#include "layout.h"

struct cInputMapParser;
struct tInputOpCode;

struct cInputMap {
    int MaxMaps;                    // 0x000 [O]
    int* MapOffset;                 // 0x004 [O]
    cInputMapParser* Parser;        // 0x008 [O]
    int OpCodeLen;                  // 0x00C [O]
    int OpCodeTop;                  // 0x010 [O]
    tInputOpCode* OpCodeBuf;        // 0x014 [O]
};

CHECK_OFFSET(cInputMap, MaxMaps, 0x000);
CHECK_OFFSET(cInputMap, MapOffset, 0x004);
CHECK_OFFSET(cInputMap, Parser, 0x008);
CHECK_OFFSET(cInputMap, OpCodeLen, 0x00C);
CHECK_OFFSET(cInputMap, OpCodeTop, 0x010);
CHECK_OFFSET(cInputMap, OpCodeBuf, 0x014);
CHECK_SIZE(cInputMap, 0x18);

#endif

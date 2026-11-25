#ifndef FE_HEARTBEAT_H
#define FE_HEARTBEAT_H

// cHeartBeat: the debug front end's heartbeat trace (fe/debugfe): two angles and phases and a 64-entry history ring
// that draw draws. Methods: update, draw.
//
// Size 0x214 (On Tour's; retail's methods reach 0x210, the last member). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods
// falls on a member start (tools/gen/self_offsets.py). EA's t2Vector (two floats x, y; On Tour: t2Vector : t2vect, 0x8)
// is written as float[2] here because no shared header defines it and fe/ovstatehud.cpp defines its own t2Vector.

#include "layout.h"


struct cHeartBeat {
    float Angle[2];                 // 0x000 [O] t2Vector Angle; [R] cDebugFE_cDebugFE zeroes both words (sw 0x24 / 0x28 of the
                                    //       debug FE); [U] cHeartBeat_update adds the per-step offset to x / y and wraps them
                                    //       (lwc1/swc1 0x0 / 0x4)
    float Phase[2];                 // 0x008 [O] t2Vector Phase; [R] zeroed by cDebugFE_cDebugFE (0x2C / 0x30); [U] cHeartBeat_update
                                    //       steps x / y by (0.005, 0.009) and takes their sines
    float History[64][2];           // 0x010 [O] t2Vector History[64]; [R] MEM_fill(.., 0, 0x200); [U] cHeartBeat_update stores
                                    //       the next trail point, cHeartBeat_draw draws the 64 points
    int HistoryIdx;                 // 0x210 [O]

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cHeartBeat_update");
    void draw(int tex) __asm__("cHeartBeat_draw");
};

CHECK_OFFSET(cHeartBeat, Angle, 0x000);
CHECK_OFFSET(cHeartBeat, Phase, 0x008);
CHECK_OFFSET(cHeartBeat, History, 0x010);
CHECK_OFFSET(cHeartBeat, HistoryIdx, 0x210);
CHECK_SIZE(cHeartBeat, 0x214);

#endif

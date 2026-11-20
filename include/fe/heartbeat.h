#ifndef FE_HEARTBEAT_H
#define FE_HEARTBEAT_H

// cHeartBeat: the debug front end's heartbeat trace (fe/debugfe): two angles and phases and a 64-entry history ring
// that draw draws. Methods: update, draw.
//
// Size 0x214 (On Tour's; retail's methods reach 0x210, the last member). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods
// falls on a member start (tools/gen/self_offsets.py). Members of class type are byte arrays (type in the comment).

#include "layout.h"


struct cHeartBeat {
    char Angle[0x8];                // 0x000 [O] t2Vector
    char Phase[0x8];                // 0x008 [O] t2Vector
    char History[0x200];            // 0x010 [O] t2Vector[64]
    int HistoryIdx;                 // 0x210 [O]
};

CHECK_OFFSET(cHeartBeat, Angle, 0x000);
CHECK_OFFSET(cHeartBeat, Phase, 0x008);
CHECK_OFFSET(cHeartBeat, History, 0x010);
CHECK_OFFSET(cHeartBeat, HistoryIdx, 0x210);
CHECK_SIZE(cHeartBeat, 0x214);

#endif

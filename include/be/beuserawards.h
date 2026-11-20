#ifndef BE_BEUSERAWARDS_H
#define BE_BEUSERAWARDS_H

// cBEUserAwards: a player's earned awards (be/beintreward): the award data and a bit array per award kind. Methods:
// setAward, setBit / isBitSet, getAwardBits, getNumAwards / getTotalNumAwards, clear.
//
// Size 0x19C (On Tour's; retail's methods reach 0x194, inside mpAwardBits (0x174-0x19B)). A FLAT layout, like
// main/game.h: member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access
// through `this` in the methods falls on a member start (tools/gen/self_offsets.py). Members of class type are byte
// arrays (type in the comment).

#include "layout.h"


struct cBEUserAwards {
    char mAwards[0x174];            // 0x000 [O] tUserAwardsData
    unsigned char* mpAwardBits[10]; // 0x174 [O]
};

CHECK_OFFSET(cBEUserAwards, mAwards, 0x000);
CHECK_OFFSET(cBEUserAwards, mpAwardBits, 0x174);
CHECK_SIZE(cBEUserAwards, 0x19C);

#endif

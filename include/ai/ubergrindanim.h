#ifndef AI_UBERGRINDANIM_H
#define AI_UBERGRINDANIM_H

// tBEUberGrindAnim: one uber grind's animation set; kUberGrindAnim holds one per grind (cUberGrindControl::CurrentGrind).
// Size 0x24: [O] On Tour struct tBEUberGrindAnim (0x24; kUberGrindAnim is `const tBEUberGrindAnim[4]`, 144 bytes, in
// all_in_one_ai.cpp); [R] retail indexes kUberGrindAnim by CurrentGrind x 0x24 (cUberGrindControl_update, view
// sAnimSet_00136268 in cUberGrindControl's entry function, ai/ai.cpp). tAIAnimID / eTrickNameUberGrind are words.
// Tags: [O] On Tour's DWARF; [U] retail use.

#include "layout.h"

struct tBEUberGrindAnim {
    int BSIntoAnim;                 // 0x00 [O]; [U] the entry anim for RailslideMode 4 (sAnimSet_00136268 anims[0])
    int FSIntoAnim;                 // 0x04 [O]; [U] the entry anim for RailslideMode 3 (anims[1])
    int FakieIntoAnim;              // 0x08 [O]; [U] the entry anim for RailslideMode 2 (anims[2])
    int RegIntoAnim;                // 0x0C [O]; [U] the entry anim otherwise (anims[3])
    int CycleAnim;                  // 0x10 [O]; [U] cUberGrindControl_update plays it (tbl + 0x10)
    int BalanceAnim;                // 0x14 [O]; [U] cUberGrindControl_update plays it (tbl + 0x14)
    int LandAnim;                   // 0x18 [O]; [U] cUberGrindControl_update plays it (tbl + 0x18)
    int OutOfAnim;                  // 0x1C [O]; [U] cUberGrindControl_update plays it (tbl + 0x1C)
    int GrindName;                  // 0x20 [O] eTrickNameUberGrind; [U] passed to cRiderMetrix_evUberGrindStart (x20)
};

CHECK_OFFSET(tBEUberGrindAnim, BSIntoAnim, 0x00);
CHECK_OFFSET(tBEUberGrindAnim, FSIntoAnim, 0x04);
CHECK_OFFSET(tBEUberGrindAnim, FakieIntoAnim, 0x08);
CHECK_OFFSET(tBEUberGrindAnim, RegIntoAnim, 0x0C);
CHECK_OFFSET(tBEUberGrindAnim, CycleAnim, 0x10);
CHECK_OFFSET(tBEUberGrindAnim, BalanceAnim, 0x14);
CHECK_OFFSET(tBEUberGrindAnim, LandAnim, 0x18);
CHECK_OFFSET(tBEUberGrindAnim, OutOfAnim, 0x1C);
CHECK_OFFSET(tBEUberGrindAnim, GrindName, 0x20);
CHECK_SIZE(tBEUberGrindAnim, 0x24);

// [O] const tBEUberGrindAnim kUberGrindAnim[4] (declared non-const here, as ai/ai.cpp's view declares it).
extern tBEUberGrindAnim kUberGrindAnim[];

#endif /* AI_UBERGRINDANIM_H */

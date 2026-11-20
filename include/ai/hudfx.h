#ifndef AI_HUDFX_H
#define AI_HUDFX_H

// cHUDFX: one HUD effect of a rider's metrix (cRiderMetrix::HUDfx, ai/ridermetrix.h): its ID, decay, data, the number
// and string it shows, and its slot. Methods in seg/seg_14B10: set, update, copyDispStr.
//
// Size 0x9C ([R] cRiderMetrix_linkToRider allocates 44 of 0x9C; On Tour's is the same). A FLAT layout, like
// main/game.h: member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access
// through `this` in the methods falls on a member start (tools/gen/self_offsets.py). Enums and bools are words.

#include "layout.h"


struct cHUDFX {
    int ID;                         // 0x000 [O] eHUDFXID
    float MaxDecay;                 // 0x004 [O]
    float CurDecay;                 // 0x008 [O]
    int Data;                       // 0x00C [O]
    int AdditionalData;             // 0x010 [O]
    int DisplayInt;                 // 0x014 [O]
    char DisplayStr[128];           // 0x018 [O]
    int SlotID;                     // 0x098 [O]
};

CHECK_OFFSET(cHUDFX, ID, 0x000);
CHECK_OFFSET(cHUDFX, MaxDecay, 0x004);
CHECK_OFFSET(cHUDFX, CurDecay, 0x008);
CHECK_OFFSET(cHUDFX, Data, 0x00C);
CHECK_OFFSET(cHUDFX, AdditionalData, 0x010);
CHECK_OFFSET(cHUDFX, DisplayInt, 0x014);
CHECK_OFFSET(cHUDFX, DisplayStr, 0x018);
CHECK_OFFSET(cHUDFX, SlotID, 0x098);
CHECK_SIZE(cHUDFX, 0x9C);

#endif

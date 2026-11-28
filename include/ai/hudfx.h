#ifndef AI_HUDFX_H
#define AI_HUDFX_H

// cHUDFX: one HUD effect of a rider's metrix (cRiderMetrix::HUDfx, ai/ridermetrix.h): its ID, decay, data, the number
// and string it shows, and its slot. Methods in seg/seg_14B10: set, update, copyDispStr.
//
// Size 0x9C ([R] cRiderMetrix_linkToRider allocates 44 of 0x9C; On Tour's is the same). A FLAT layout, like
// main/game.h: member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access
// through `this` in the methods falls on a member start (tools/gen/self_offsets.py). Enums and bools are words.

#include "layout.h"

// A HUD effect's id (cHUDFX::ID, and the HUDfx slot it is shown in). [O] On Tour's enum eHUDFXID, names and values,
// for the ids whose retail use fits On Tour's names (the others are left out, unchecked). [U]
// cRiderMetrix_updateStaticHUD shows kHFX_PotentialPoints with the trick's points, kHFX_PotentialPower against the
// boost bar, kHFX_WipeoutRecover with the wipeout's RecoverPercent in CS_Wipeout, the style meter group
// (kHFX_StyleMeter to kHFX_StyleMultiplier) and the eight zones (kHFX_StyleZoneR to kHFX_StyleZoneDR, picked by the
// flip-spin input angle) only in CS_Prewind / CS_Spin; cRiderMetrix_evAddCash shows kHFX_CashOrPointsAdded and the
// matching kHFX_Earn*Cash / kHFX_EarnCollectible popup; hasActiveHUDfx and updateStaticHUD skip kHFX_Null.
enum eHUDFXID {
    kHFX_PotentialPoints = 1,
    kHFX_PotentialPower = 5,
    kHFX_WipeoutRecover = 11,
    kHFX_StyleMeter = 12,
    kHFX_StylePointer = 13,
    kHFX_StyleLabel = 14,
    kHFX_StyleMultiplier = 15,
    kHFX_StyleZoneR = 16,
    kHFX_StyleZoneUR = 17,
    kHFX_StyleZoneU = 18,
    kHFX_StyleZoneUL = 19,
    kHFX_StyleZoneL = 20,
    kHFX_StyleZoneDL = 21,
    kHFX_StyleZoneD = 22,
    kHFX_StyleZoneDR = 23,
    kHFX_CashOrPointsAdded = 24,
    kHFX_EarnBonusCash = 46,
    kHFX_EarnTrickPointCash = 47,
    kHFX_EarnComboCash = 48,
    kHFX_EarnCollectible = 49,
    kHFX_Null = 52
};


struct cHUDFX {
    int ID;                         // 0x000 [O] eHUDFXID
    float MaxDecay;                 // 0x004 [O]
    float CurDecay;                 // 0x008 [O]
    int Data;                       // 0x00C [O]
    int AdditionalData;             // 0x010 [O]
    int DisplayInt;                 // 0x014 [O]
    char DisplayStr[128];           // 0x018 [O]
    int SlotID;                     // 0x098 [O]

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cHUDFX_update");
    void copyDispStr(signed char *str) __asm__("cHUDFX_copyDispStr");
    void set(int id, signed char *displayStr, int decayTime, float fparg0) __asm__("cHUDFX_set");
    void set1(int id, int displayInt, float fparg0, int data) __asm__("cHUDFX_set1");
    void setAsync(int fxId, int displayInt, float fparg0, int fxData) __asm__("cHUDFX_setAsync");
    void setAsync1(int fxId, signed char *displayStr, int data, float fparg0) __asm__("cHUDFX_setAsync1");
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

#ifndef OBJECT_TEXFLIPMODIFIER_H
#define OBJECT_TEXFLIPMODIFIER_H

// cTexFlipModifier: flips a placed instance's textures through an animation sequence (tModifierBlock::TexFlip).
// No vtable. Size 0xE8: retail cMemMan_alloc(0xE8, "TexFlipModifier") in cMoveNode's addTexFlip and the replay loader;
// On Tour 0xA8 (its tTexFlipMaterialBlockRecord is smaller; the members before it keep On Tour's offsets).
// EA's source: d:/ssx/code/source/object/texflipmodifier.cpp (preview assert).
// Methods (src/object/railmodifier.cpp): ctor cTexFlipModifier_cTexFlipModifier (tAddTexFlipData, instance), replay ctor cTexFlipModifier_cTexFlipModifier1,
// setup cTexFlipModifier_setupTexFlip (counts materials, fills MaterialBlock), update cTexFlipModifier_update, signal cTexFlipModifier_signal,
// writeToReplayFrame cTexFlipModifier_writeToReplayFrame.

#include "layout.h"

struct tInstanceRecord;

// World-script data that adds a texture-flip modifier (On Tour tAddTexFlipData, 0x18; same layout). [O] On Tour's
// cTexFlipModifier(const tAddTexFlipData& tdata, tInstanceRecord* inst); [U] cTexFlipModifier_cTexFlipModifier
// (src/object/railmodifier.cpp) stores Type / Dir into the members of the same names, Speed / TimerHz as the rate,
// and starts the flip at StartId (TexIndex).
struct tAddTexFlipData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read)
    int Type;                       // 0x04 [O]; [U] -> Type
    int StartId;                    // 0x08 [O]; [U] -> TexIndex (below 0: a random start)
    int Dir;                        // 0x0C [O]; [U] -> Dir
    float Speed;                    // 0x10 [O]; [U] Speed / TimerHz -> Speed and BaseSpeed
    char unk_014[0x4];              // 0x14 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tAddTexFlipData, unk_000, 0x00);
CHECK_OFFSET(tAddTexFlipData, Type, 0x04);
CHECK_OFFSET(tAddTexFlipData, StartId, 0x08);
CHECK_OFFSET(tAddTexFlipData, Dir, 0x0C);
CHECK_OFFSET(tAddTexFlipData, Speed, 0x10);
CHECK_OFFSET(tAddTexFlipData, unk_014, 0x14);
CHECK_SIZE(tAddTexFlipData, 0x18);

struct cTexFlipModifier {
    // --- cTexFlipModifierState (0x18; the replay block)
    int Type;                       // 0x00 [O]; [R] ctor; [U] update switches on it
    float Time;                     // 0x04 [O]; [R] ctor; [U] update
    float Speed;                    // 0x08 [O]; [R] ctor; [U] update, signal
    float BaseSpeed;                // 0x0C [O]; [R] ctor; [U] update
    int Dir;                        // 0x10 [O]; [R] ctor; [U] update
    int TexIndex;                   // 0x14 [O]; [R] ctor (BXrand); [U] setup, update
    // --- cTexFlipModifier
    tInstanceRecord* Instance;      // 0x18 [O]; [R] ctor stores the instance arg, the replay ctor reads it; [U] setup
                                    //      (model +0x94), writeToReplayFrame
    int NumMaterials;               // 0x1C [O]; [R] ctor zeroes it; [U] setup
    int NumTexFlipMaterials;        // 0x20 [O]; [R] ctor zeroes it; [U] setup, update
    char MaterialBlock[0xc4];       // 0x24 [O] tTexFlipMaterialBlockRecord (On Tour 0x84; retail 0xC4, not laid out);
                                    //      [U] setup MEM_fills it and passes +0x24 to cTexFlipModifier_setTexIndex
};

CHECK_OFFSET(cTexFlipModifier, Type, 0x00);
CHECK_OFFSET(cTexFlipModifier, Time, 0x04);
CHECK_OFFSET(cTexFlipModifier, Speed, 0x08);
CHECK_OFFSET(cTexFlipModifier, BaseSpeed, 0x0C);
CHECK_OFFSET(cTexFlipModifier, Dir, 0x10);
CHECK_OFFSET(cTexFlipModifier, TexIndex, 0x14);
CHECK_OFFSET(cTexFlipModifier, Instance, 0x18);
CHECK_OFFSET(cTexFlipModifier, NumMaterials, 0x1C);
CHECK_OFFSET(cTexFlipModifier, NumTexFlipMaterials, 0x20);
CHECK_OFFSET(cTexFlipModifier, MaterialBlock, 0x24);
CHECK_SIZE(cTexFlipModifier, 0xE8);

#endif /* OBJECT_TEXFLIPMODIFIER_H */

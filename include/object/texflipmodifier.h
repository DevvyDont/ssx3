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
#include "wscript/wscriptnull.h"
#include "world/worldrecords.h" /* tMaterialRecord */

struct tInstanceRecord;

// World-script data that adds a texture-flip modifier (On Tour tAddTexFlipData, 0x18; same layout). [O] On Tour's
// cTexFlipModifier(const tAddTexFlipData& tdata, tInstanceRecord* inst); [U] cTexFlipModifier_cTexFlipModifier
// (src/object/railmodifier.cpp) stores Type / Dir into the members of the same names, Speed / TimerHz as the rate,
// and starts the flip at StartId (TexIndex).
struct tAddTexFlipData {
    unsigned int InstId;            // 0x00 [O] tResourceID; [U] AddTexFlipFunc's instance lookup (-1: the script's
                                    //      current instance)
    int Type;                       // 0x04 [O]; [U] -> Type
    int StartId;                    // 0x08 [O]; [U] -> TexIndex (below 0: a random start)
    int Dir;                        // 0x0C [O]; [U] -> Dir
    float Speed;                    // 0x10 [O]; [U] Speed / TimerHz -> Speed and BaseSpeed
    WScriptNullType null;           // 0x14 [O]
    char unk_015[0x3];              // 0x15 padding
};

CHECK_OFFSET(tAddTexFlipData, InstId, 0x00);
CHECK_OFFSET(tAddTexFlipData, Type, 0x04);
CHECK_OFFSET(tAddTexFlipData, StartId, 0x08);
CHECK_OFFSET(tAddTexFlipData, Dir, 0x0C);
CHECK_OFFSET(tAddTexFlipData, Speed, 0x10);
CHECK_OFFSET(tAddTexFlipData, null, 0x14);
CHECK_SIZE(tAddTexFlipData, 0x18);

// The modifier's private material block (On Tour struct tTexFlipMaterialBlockRecord : tMaterialBlockRecord, 0x84
// there; retail 0xC4 because its tMaterialRecord is 0x14). The base's count written out inline. [O]; [U]
// cTexFlipModifier_setupTexFlip (view sRailSelF218) MEM_fills it, copies the model's material records into
// MaterialRecords, points Materials[i] at them and stores the count; cMoveNode_getMaterialBlock returns it (+0x24) in
// place of the model's tMaterialBlockRecord, which it extends.
struct tTexFlipMaterialBlockRecord {
    int m_iNumMaterials;            // 0x00 [O] tMaterialBlockRecord::m_iNumMaterials; [U] setupTexFlip
    tMaterialRecord* Materials[8];  // 0x04 [O]; [U] setupTexFlip: &MaterialRecords[i] (the list
                                    //      tMaterialBlockRecord::GetMaterialRecordList() reads)
    tMaterialRecord MaterialRecords[8]; // 0x24 [O]; [U] setupTexFlip copies the model's records here (view sRailOutF218)
};

CHECK_OFFSET(tTexFlipMaterialBlockRecord, m_iNumMaterials, 0x00);
CHECK_OFFSET(tTexFlipMaterialBlockRecord, Materials, 0x04);
CHECK_OFFSET(tTexFlipMaterialBlockRecord, MaterialRecords, 0x24);
CHECK_SIZE(tTexFlipMaterialBlockRecord, 0xC4);

struct cBxReplayFrame;

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
    tTexFlipMaterialBlockRecord MaterialBlock; // 0x24 [O] (On Tour 0x84; retail 0xC4); [U] setup MEM_fills and
                                    //      fills it; passed to cTexFlipModifier_setTexIndex

    // Type: how the flip advances (names ours). [U] cTexFlipModifier_update steps a kTexFlip_Sequence one texture per
    // Speed tick (backward with Dir set); a random flicker alternates kTexFlip_RandomSlow (a random 0.25..1.0 of
    // BaseSpeed) and kTexFlip_RandomFast (10 steps a second), switching on every step.
    enum eTexFlipType { kTexFlip_Sequence = 0, kTexFlip_RandomSlow = 1, kTexFlip_RandomFast = 2 };
    // signal's ids (names ours). [U] cTexFlipModifier_signal: 300 sets Speed, 301 picks a texture, 302 steps to the
    // next one.
    enum eTexFlipSignal {
        kTexFlipSignal_Speed = 300, kTexFlipSignal_SetTexture = 301, kTexFlipSignal_NextTexture = 302
    };

    // Methods bound to their retail symbols by asm labels.
    void* cTexFlipModifier1(cBxReplayFrame* obj) __asm__("cTexFlipModifier_cTexFlipModifier1");
    void setupTexFlip() __asm__("cTexFlipModifier_setupTexFlip");
    void setTexIndex(int i) __asm__("cTexFlipModifier_setTexIndex");
    void setupMaterialRecords() __asm__("cTexFlipModifier_setupMaterialRecords");
    void signal(int id, float value) __asm__("cTexFlipModifier_signal");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cTexFlipModifier_writeToReplayFrame");
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

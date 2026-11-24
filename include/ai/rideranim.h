#ifndef AI_RIDERANIM_H
#define AI_RIDERANIM_H

// cRiderAnim: the rider's animation player with the game rules attached. It derives from cRiderAnimBase (the six
// sequencers) and implements its virtual update / expire rules (cRiderAnim_updateRule dispatches on the animation's
// update rule to onUpdateFWD .. onUpdateASYNC_BP_PIVOT, cRiderAnim_expireRule on the expire rule to onExpireNOP ..
// onExpireBLENDTO_GEAR_CYC), maps AI animation ids to game animation data (mapAIDToGAD) and fires the animation sound
// events. It is linked either to an in-game rider (linkToRider) or to a front-end rider (linkToFERider). EA's source:
// d:/ssx/code/source/ai/rideranim.cpp (preview assert path); retail keeps its methods in seg/seg_1218
// (0x001036A0 updateRule ... 0x00104DE0 readFromReplayFrame) and the ctor / dtor in ai/control/handplantcontrol.
//
// Size 0x70: [R] cRider_loadRider allocates it with cMemMan_alloc(0x70, "RiderAnim") (D_004579B0), then
// cRiderAnimBase_cRiderAnimBase, the cRiderAnim vtable at 0x58 and zeroes 0x60 / 0x64 (the same steps as
// cRiderAnim_cRiderAnim); cFERider_compileModel allocates 0x70 too (new cRiderAnim). [O] On Tour's cRiderAnim is 0x80
// (its cRiderAnimBase is 0x10 bigger: 0x70), with Rider / FERider right after the base, at 0x70 / 0x74.
// Base: cRiderAnimBase (animation/rideranimbase.h, 0x60), written out inline below with the same member names; the
// class adds no virtuals of its own beyond the base's slots, so the base's vptr at 0x58 is the only one
// (_vt$10cRiderAnim D_00458458: slot 1 cRiderAnim__cRiderAnim__FPv, 2 updateRule, 3 expireRule, 4 mapAIDToGAD).
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [P] the July 2003 preview's inline accessors; [R] retail's construction (above) and cRiderAnimBase_reset;
// [U] how retail code uses the field; [O] SSX On Tour's DWARF.

#include "layout.h"
#include "bx/math3d.h"
#include "animation/rideranimbase.h" /* tPose */

struct cAnimSequencer;
struct cAnimModel;
struct cRider;
struct cFERider;

struct cRiderAnim {
    // --- cRiderAnimBase (animation/rideranimbase.h; ctor cRiderAnimBase_cRiderAnimBase, methods in
    //       animation/rideranimbase)
    int CurrentAnimID[6];           // 0x000 [O]; [R] cRiderAnimBase_reset stores 0x1B6 (no anim) in each; [U]
                                    //       cRiderAnim_expireRule stores the expire handler's result in [sequencer
                                    //       index] (view sEventOwner3918.results); cRiderAnimBase_play stores the
                                    //       started id
    int IsFlipped;                  // 0x018 [P] isFlipped/setIsFlipped (lw/sw: a 4-byte bool); [O]; [R] reset: 0
    float FrameRateFactor;          // 0x01C [P] get/setFrameRateFactor (lwc1/swc1); [O]; [R] reset: 1.0f
    unsigned long GraftMask;        // 0x020 [P] get/setGraftingMask (ld/sd); [O]; [R] reset: -1. PORT: 64-bit long on
                                    //       the EE
    char unk_028[0x8];              // 0x028 alignment gap before the 16-byte-aligned tPose (as in cRiderAnimBase)
    tPose OrientationOffset;        // 0x030 [P] getOrientationOffset (this + 0x30); [O]; [R] reset copies the identity
                                    //       pose
    cAnimSequencer* Sequencer;      // 0x050 [O]; [R] cRiderAnimBase ctor: 6 x 8-byte sequencers ("Sequencers"); [U]
                                    //       cRiderAnim_writeToReplayFrame / readFromReplayFrame walk the 6 (Sequencer +
                                    //       i * 8) after the first 0x50 bytes; expireRule's sequencer index is (pSeqr -
                                    //       Sequencer)
    cAnimModel* AnimModel;          // 0x054 [O]; [R] the base ctor stores 0; [U] cRiderAnim_linkToRider copies
                                    //       Rider->mAnimModel (+0x780), linkToFERider (cRiderAnim_linkToFERider) FERider's +0x8
                                    //       (cFERider::mpAnimModel)
    void** mVtbl;                   // 0x058 [R] cRiderAnim_cRiderAnim / cRider_loadRider / cFERider_compileModel store
                                    //       cRiderAnim_vtbl (D_00458458) over the base's D_0048D7D8
    char unk_05C[0x4];              // 0x05C cRiderAnimBase's tail padding (the base is 16-byte aligned by tPose)
    // --- cRiderAnim
    cRider* Rider;                  // 0x060 [O] (0x70 in On Tour); [R] the ctor stores 0; [U] cRiderAnim_linkToRider
                                    //       sets it, linkToFERider clears it; the onUpdateASYNC_* rules read the
                                    //       rider's state through it (+0x238, +0x244, +0x268, +0x274, +0x280, +0x330),
                                    //       mapAIDToGAD reads +0x364 when FERider is 0
    cFERider* FERider;              // 0x064 [O] (0x74 in On Tour); [R] the ctor stores 0; [U] linkToFERider
                                    //       (cRiderAnim_linkToFERider) sets it, linkToRider clears it; mapAIDToGAD reads +0xCD8
                                    //       through it; onExpireBLENDTO_AIR tests it
    char unk_068[0x8];              // 0x068 tail padding to the 16-byte-aligned size 0x70 (never accessed)
};

CHECK_OFFSET(cRiderAnim, CurrentAnimID, 0x000);
CHECK_OFFSET(cRiderAnim, IsFlipped, 0x018);
CHECK_OFFSET(cRiderAnim, FrameRateFactor, 0x01C);
CHECK_OFFSET(cRiderAnim, GraftMask, 0x020);
CHECK_OFFSET(cRiderAnim, unk_028, 0x028);
CHECK_OFFSET(cRiderAnim, OrientationOffset, 0x030);
CHECK_OFFSET(cRiderAnim, Sequencer, 0x050);
CHECK_OFFSET(cRiderAnim, AnimModel, 0x054);
CHECK_OFFSET(cRiderAnim, mVtbl, 0x058);
CHECK_OFFSET(cRiderAnim, unk_05C, 0x05C);
CHECK_OFFSET(cRiderAnim, Rider, 0x060);
CHECK_OFFSET(cRiderAnim, FERider, 0x064);
CHECK_OFFSET(cRiderAnim, unk_068, 0x068);
CHECK_SIZE(cRiderAnim, 0x070);

#endif /* AI_RIDERANIM_H */

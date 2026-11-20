#ifndef ANIMATION_RIDERANIMBASE_H
#define ANIMATION_RIDERANIMBASE_H

// cRiderAnimBase: a rider's animation player. It owns 6 animation sequencers (one per AI-anim sequencer slot), the
// current AI-anim id of each, and the state copied into every sequence it starts (flip, frame-rate factor, grafting
// mask, orientation offset). Its methods live in animation/rideranimbase.cpp (EA's cRiderAnimBase::play, reset,
// changeHeadingOffset, changeOrientationOffset, getAIAnimFrame ...).
//
// Derived class: cRiderAnim (`cMemMan_alloc(0x70, "RiderAnim")` in cFERider_compileModel; its ctor cRiderAnim_cRiderAnim calls
// cRiderAnimBase_cRiderAnimBase, then stores its own vtable cRiderAnim_vtbl at 0x58 and its fields at 0x60 and 0x64).
// cRider holds one at mRiderAnim (0x784).
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [P] the July 2003 preview's inline accessors (isFlipped lw 0x18, getFrameRateFactor lwc1 0x1C,
// getGraftingMask ld 0x20, getOrientationOffset addiu 0x30; preview and retail agree on every offset of this class;
// tools/layout_xlate.py's "0x18 -> 0x4" pair is a misalignment); [R] retail's constructor
// (cRiderAnimBase_cRiderAnimBase) and cRiderAnimBase_reset; [U] how matched retail code uses the field;
// [O] SSX On Tour's DWARF. On Tour's 0x70-byte cRiderAnimBase has the per-sequencer state in its first 0x60 bytes
// (the same members as its class cRiderAnimState, plus a BlendTimeScale at 0x50 that retail lacks), then Sequencer
// at 0x60 and AnimModel at 0x64; retail has them at 0x50 and 0x54 (On Tour's cRiderAnim is 0x10 bigger too: 0x80).

#include "layout.h"
#include "bx/math3d.h"

struct cAnimSequencer;
struct cAnimModel;

struct cRiderAnimBase {
    int CurrentAnimID[6];           // 0x000 [O] (cRiderAnimState::CurrentAnimID[6]); [R] reset stores 0x1B6 (no anim)
                                    //       in each; [U] cRiderAnimBase_play stores the started id at [slot],
                                    //       cRiderAnimBase_getCurrentAnimCategory / cRiderAnimBase_getCurrentAnimID read [i]
    int IsFlipped;                  // 0x018 [P] isFlipped/setIsFlipped (lw/sw: a 4-byte bool); [O]; [R] reset: 0;
                                    //       [U] cRiderAnimBase_onInput copies it to the new sequence (+0x80), as
                                    //       cRiderAnimBase_changeIsFlipped does for every sequence
    float FrameRateFactor;          // 0x01C [P] get/setFrameRateFactor (lwc1/swc1); [O]; [R] reset: 1.0f;
                                    //       [U] cRiderAnimBase_onInput copies it to the sequence (+0x90)
    unsigned long GraftMask;        // 0x020 [P] get/setGraftingMask (ld/sd, `Ul`), clearGraftingMask stores -1; [O];
                                    //       [R] reset: -1; [U] cRiderAnimBase_onInput copies it to the sequence (+0x88); cRider
                                    //       copies mUpperBodyBoneMask here before play. PORT: 64-bit long on the EE
    char unk_028[0x8];              // 0x028 alignment gap before the 16-byte-aligned tPose (On Tour has it too)
    tPose OrientationOffset;        // 0x030 [P] getOrientationOffset (this + 0x30), setOrientationOffset (sq 0x30,
                                    //       0x40); [O]; [R] reset copies tPose_Identity (the identity pose) into it;
                                    //       [U] cRiderAnimBase_onInput copies it to the sequence (+0x60)
    cAnimSequencer* Sequencer;      // 0x050 [O] Sequencer; [R] the ctor allocates 0x30 bytes tagged "Sequencers"
                                    //       (6 x 8-byte cAnimSequencer, each cAnimSequencer_init'ed); [P] the preview
                                    //       destructor reads 0x50 too; [U] every method indexes it by slot
    cAnimModel* AnimModel;          // 0x054 [O] AnimModel (follows Sequencer in On Tour); [R] the ctor stores 0;
                                    //       [U] cRiderAnimBase_setupAnimation passes it to func_0030F2B0 (animation/animmodel.cpp)
                                    //       with the states gathered from the 6 sequencers
    void** mVtbl;                   // 0x058 [R] the ctor and destructor (cRiderAnimBase__cRiderAnimBase) store cRiderAnimBase_vtbl (slot 1 =
                                    //       cRiderAnimBase__cRiderAnimBase; slot 2 = func_00411530, a stub in 5 vtables, likely
                                    //       __pure_virtual); cRiderAnim's ctor overwrites it with
                                    //       cRiderAnim_vtbl; [P] preview vptr at 0x58; [U] slots 2 and 4 called
                                    //       (cRiderAnimBase_updateAnimation, cRiderAnimBase_play, cRiderAnimBase_getAIAnimFrame)
    char unk_05C[0x4];              // 0x05C tail padding (the class is 16-byte aligned by tPose)
};

CHECK_OFFSET(cRiderAnimBase, CurrentAnimID, 0x00);
CHECK_OFFSET(cRiderAnimBase, IsFlipped, 0x18);
CHECK_OFFSET(cRiderAnimBase, FrameRateFactor, 0x1C);
CHECK_OFFSET(cRiderAnimBase, GraftMask, 0x20);
CHECK_OFFSET(cRiderAnimBase, unk_028, 0x28);
CHECK_OFFSET(cRiderAnimBase, OrientationOffset, 0x30);
CHECK_OFFSET(cRiderAnimBase, Sequencer, 0x50);
CHECK_OFFSET(cRiderAnimBase, AnimModel, 0x54);
CHECK_OFFSET(cRiderAnimBase, mVtbl, 0x58);
CHECK_OFFSET(cRiderAnimBase, unk_05C, 0x5C);
CHECK_SIZE(cRiderAnimBase, 0x60);

#endif /* ANIMATION_RIDERANIMBASE_H */

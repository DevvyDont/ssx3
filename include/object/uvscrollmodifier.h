#ifndef OBJECT_UVSCROLLMODIFIER_H
#define OBJECT_UVSCROLLMODIFIER_H

// cUVScrollModifier: scrolls / rotates a placed instance's texture coordinates (tModifierBlock::UVScroll; waterfalls,
// signs). No vtable (not virtual). Size 0x50: retail cMemMan_alloc(0x50, "UVScrollModifier") in cMoveNode's addUVScroll
// (src/object/movenode.cpp) and the replay loader (src/object/modifierblock.cpp); On Tour 0x50 (all of it is
// cUVScrollModifierState, which the replay frame stores whole).
// Methods (src/object/railmodifier.cpp): ctor cUVScrollModifier_cUVScrollModifier (tAddUVScrollData), replay ctor cUVScrollModifier_cUVScrollModifier1, update
// cUVScrollModifier_update (tModifierBlock preUpdate), signal cUVScrollModifier_signal, getTextureMat cUVScrollModifier_getTextureMat (cMoveNode_render/
// renderModel), writeToReplayFrame cUVScrollModifier_writeToReplayFrame. Every On Tour offset equals retail (On: 4-byte bool).

#include "layout.h"
#include "bx/math3d.h"

struct cBxReplayFrame;
struct t4Matrix;

struct cUVScrollModifier {
    // --- cUVScrollModifierState (the whole object)
    int Type;                       // 0x00 [O]; [R] ctor (desc +0x4); [U] update switches on it
    float Time;                     // 0x04 [O]; [R] ctor zeroes it; [U] update
    float TimeOn;                   // 0x08 [O]; [R] ctor; [U] update, signal
    float TimeOff;                  // 0x0C [O]; [R] ctor; [U] update, signal
    float Angle;                    // 0x10 [O]; [R] ctor zeroes it; [U] update, getTextureMat
    float AngVel;                   // 0x14 [O]; [R] ctor (rate / frame rate); [U] update
    char unk_18[0x8];               // 0x18 padding (Axis is 16-byte aligned)
    t4Vector Axis;                  // 0x20 [O]; [R] ctor (sq 0x20); [U] getTextureMat
    t2Vector UV;                    // 0x30 [O] t2Vector; [R] ctor; [U] update, signal, getTextureMat
    t2Vector UVScroll;              // 0x38 [O] t2Vector; [R] ctor; [U] update (unaligned sdl/sdr copy), signal
    int On;                         // 0x40 [O] bool; [R] ctor stores 1; [U] update, signal
    char unk_44[0xc];               // 0x44 padding to the 0x50 allocation

    // The Type values update treats specially (names ours). [U] cUVScrollModifier_update reverses UVScroll every
    // TimeOn for types 2 and 6, and type 6 also eases the offset over the on phase; other types scroll one way.
    enum { kUVScroll_PingPong = 2, kUVScroll_EasedPingPong = 6 };
    // signal's ids (names ours). [U] cUVScrollModifier_signal: 200 / 201 set UVScroll[1] / [0], 202 / 203 TimeOn /
    // TimeOff, 204 / 205 UV[0] / [1].
    enum eUVScrollSignal {
        kUVScrollSignal_VScroll = 200, kUVScrollSignal_UScroll = 201, kUVScrollSignal_TimeOn = 202,
        kUVScrollSignal_TimeOff = 203, kUVScrollSignal_U = 204, kUVScrollSignal_V = 205
    };

    // Methods bound to their retail symbols by asm labels.
    void* cUVScrollModifier1(cBxReplayFrame* stream) __asm__("cUVScrollModifier_cUVScrollModifier1");
    void update() __asm__("cUVScrollModifier_update");
    void signal(int id, float value) __asm__("cUVScrollModifier_signal");
    t4Matrix* getTextureMat(t4Matrix* mat) __asm__("cUVScrollModifier_getTextureMat");
    void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cUVScrollModifier_writeToReplayFrame");
};

CHECK_OFFSET(cUVScrollModifier, Type, 0x00);
CHECK_OFFSET(cUVScrollModifier, Time, 0x04);
CHECK_OFFSET(cUVScrollModifier, TimeOn, 0x08);
CHECK_OFFSET(cUVScrollModifier, TimeOff, 0x0C);
CHECK_OFFSET(cUVScrollModifier, Angle, 0x10);
CHECK_OFFSET(cUVScrollModifier, AngVel, 0x14);
CHECK_OFFSET(cUVScrollModifier, unk_18, 0x18);
CHECK_OFFSET(cUVScrollModifier, Axis, 0x20);
CHECK_OFFSET(cUVScrollModifier, UV, 0x30);
CHECK_OFFSET(cUVScrollModifier, UVScroll, 0x38);
CHECK_OFFSET(cUVScrollModifier, On, 0x40);
CHECK_OFFSET(cUVScrollModifier, unk_44, 0x44);
CHECK_SIZE(cUVScrollModifier, 0x50);

#endif /* OBJECT_UVSCROLLMODIFIER_H */

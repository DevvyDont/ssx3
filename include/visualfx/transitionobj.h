#ifndef VISUALFX_TRANSITIONOBJ_H
#define VISUALFX_TRANSITIONOBJ_H

// The screen-transition objects cTransitionMan draws (visualfx/transitionman.h: mTransitionIn / mTransitionOut): each
// is a cTransitionObj whose Draw(amount) covers the screen in a colour, amount 0..1 being how far the transition has
// gone. cSSXScriptEngine_MakeTransitionObject makes them for scripts (types 1/7 fade, 2-5 wipe, 6 circle);
// cGFSessionTeleportState_gainFocus (main/gamestate) and cResetControl_gainFocus (ai/ai) build fades inline.
//   cFade_TObj    a full-screen rectangle with the colour's alpha scaled by amount;
//   cWipe_TObj    a full-screen rectangle slid in from a side (mHDir / mVDir: -1, 0 or 1);
//   cCircle_TObj  an iris: everything outside a circle of radius (1 - amount) * 800 around (mOriginX, mOriginY); the
//                 one-colour ctor picks a random origin on the 640x480 screen, cCircle_TObj_cCircle_TObj1 takes it explicitly.
// Base cTransitionObj (On Tour 0x4, vptr only; vtable cTransitionObj_vtbl 0x004880E0: 1 dtor, 2 Draw = pure virtual):
// its operator new / delete hand out one of four static 0x64-byte buffers (cTransitionObj_mBuffer, in-use flags cTransitionObj_mbBufferUsed;
// cTransitionObj_operator_new ignores the size its callers pass), so the objects are never heap-allocated.
// EA's source file is unknown (the preview names none of these classes in asserts); cTransitionObj's ctor and
// buffer functions sit right after cTransitionMan's methods in splat unit visualfx/boardwakefx (0x002E4CB0 ..
// 0x002E4D70), the Draw functions and the wipe / circle ctors in visualfx/boardtrailfx (0x002EBB10 cFade_TObj_Draw ..
// 0x002EC060 cCircle_TObj_Draw), the fade ctor and the dtors in visualfx/worldlightman (0x002F8000 .. 0x002F8108).
//
// Sizes: [R] the callers pass the object size to cTransitionObj_operator_new: 20 (0x14) for cFade_TObj
// (cSSXScriptEngine_MakeTransitionObject, cGFSessionTeleportState_gainFocus, cResetControl_gainFocus) and 0x1C for
// cWipe_TObj and cCircle_TObj (MakeTransitionObject); the ctors write up to 0x18. On Tour: 0x20 / 0x30 / 0x30 (its
// tARGB is 16-byte aligned, so mColour is at 0x10 there; retail's tARGB is 4-aligned and sits at 0x4).
// Vtables (2 slots: 1 dtor, 2 Draw): cFade_TObj_vtbl 0x004880C0, cWipe_TObj_vtbl 0x004880A0, cCircle_TObj_vtbl
// 0x00488080.
//
// A FLAT layout, like main/game.h: every field at its retail offset, the base's vptr written out inline. Evidence tags:
//   [R] retail ctors (cFade_TObj_cFade_TObj, cWipe_TObj_cWipe_TObj, cCircle_TObj_cCircle_TObj, cCircle_TObj_cCircle_TObj1), dtors and
//       the pool allocation sizes;
//   [P] the preview has the same ctor __10cFade_TObjG5tARGB (colour by value); tools/layout_xlate.py pairs the dtors
//       and Draws: 0x0, 0x4-0x13 (the colour), 0x14 and 0x18, preview == retail;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cFade_TObj::mColour, cWipe_TObj::mColour/mHDir/mVDir, cCircle_TObj::mColour/mOriginX/
//       mOriginY; cTransitionObj).

#include "layout.h"
#include "ui/uithing.h" // tARGB (4-byte aligned on the PS2)

struct cFade_TObj {
    // --- cTransitionObj (0x4)
    void** mVtbl;                   // 0x00 [R] cTransitionObj_vtbl (cTransitionObj_cTransitionObj__FPv), then
                                    //      cFade_TObj_vtbl (ctor); the dtor stores cTransitionObj_vtbl
    // --- cFade_TObj
    tARGB mColour;                  // 0x04 [O]; [R] ctor stores the colour (A, R, G, B); [U] Draw draws it with
                                    //      A * amount (lwc1 0x4)

    // Methods bound to their retail symbols by asm labels.
    void Draw(float amount) __asm__("cFade_TObj_Draw");
};

CHECK_OFFSET(cFade_TObj, mVtbl, 0x00);
CHECK_OFFSET(cFade_TObj, mColour, 0x04);
CHECK_SIZE(cFade_TObj, 0x14);

struct cWipe_TObj {
    // --- cTransitionObj (0x4)
    void** mVtbl;                   // 0x00 [R] cTransitionObj_vtbl, then cWipe_TObj_vtbl (ctor); the dtor stores
                                    //      cTransitionObj_vtbl
    // --- cWipe_TObj
    tARGB mColour;                  // 0x04 [O]; [R] ctor; [U] Draw passes its address as the rectangle colour
    int mHDir;                      // 0x14 [O] eTransitionHorizontal; [R] ctor (MakeTransitionObject passes -1 / 1 / 0);
                                    //      [U] Draw: x offset = mHDir * (1 - amount) * 640 (lwc1 + cvt.s.w)
    int mVDir;                      // 0x18 [O] eTransitionVertical; [R] ctor; [U] Draw: y offset = mVDir * (1 - amount)
                                    //      * -480 (lwc1 + cvt.s.w)

    // Methods bound to their retail symbols by asm labels.
    void Draw(float amount) __asm__("cWipe_TObj_Draw");
};

CHECK_OFFSET(cWipe_TObj, mVtbl, 0x00);
CHECK_OFFSET(cWipe_TObj, mColour, 0x04);
CHECK_OFFSET(cWipe_TObj, mHDir, 0x14);
CHECK_OFFSET(cWipe_TObj, mVDir, 0x18);
CHECK_SIZE(cWipe_TObj, 0x1C);

struct cCircle_TObj {
    // --- cTransitionObj (0x4)
    void** mVtbl;                   // 0x00 [R] cTransitionObj_vtbl, then cCircle_TObj_vtbl (both ctors); the dtor
                                    //      stores cTransitionObj_vtbl
    // --- cCircle_TObj
    tARGB mScreenColour;            // 0x04 [O]; [R] both ctors; [U] Draw passes its address to the rectangle and
                                    //      vertex helpers (func_002EB938, InitVertex)
    float mOriginX;                 // 0x14 [O]; [R] ctor (rand % 1000) * 0.64, cCircle_TObj_cCircle_TObj1 originX; [U] Draw (lwc1)
    float mOriginY;                 // 0x18 [O]; [R] ctor (rand % 1000) * 0.48, cCircle_TObj_cCircle_TObj1 originY; [U] Draw (lwc1)

    // Methods bound to their retail symbols by asm labels.
    void Draw(float amount) __asm__("cCircle_TObj_Draw");
};

CHECK_OFFSET(cCircle_TObj, mVtbl, 0x00);
CHECK_OFFSET(cCircle_TObj, mScreenColour, 0x04);
CHECK_OFFSET(cCircle_TObj, mOriginX, 0x14);
CHECK_OFFSET(cCircle_TObj, mOriginY, 0x18);
CHECK_SIZE(cCircle_TObj, 0x1C);

#endif /* VISUALFX_TRANSITIONOBJ_H */

#ifndef VISUALFX_BOARDWAKEFX_H
#define VISUALFX_BOARDWAKEFX_H

// cWakeParams: a board wake's smoothed riding parameters (the ground frame, speed and alpha the wake ribbon is built
// with), embedded in cBoardWakeFX at +0x60 (its ribbon colour follows at +0xA0). cBoardWakeFX_update blends it towards
// the current ground normal / side / forward vectors and the surface's wake strength and grip each frame;
// cBoardWakeFX_fillCurrentSeg builds the newest ribbon points from it; cBoardWakeFX_reset clears it (cWakeParams_Clear,
// the class's only out-of-line method).
// EA's source: d:/ssx/code/source/visualfx/boardwakefx.cpp (July 2003 preview assert strings; On Tour compiles
// cWakeParams::Clear in visualfx/all_in_one_visualfx.cpp). Retail: unit visualfx/boardwakefx (0x002DE368 Clear).
// (cBoardWakeFX itself, a cRider-derived class with a virtual base, is laid out in ai/riderhandler.h.)
//
// Size 0x40: [U] cBoardWakeFX's views place it at +0x60 and the next member (the wake colour) at +0xA0; Clear writes
// 0x0-0x34; t4Vector alignment rounds the three floats up to 0x40. On Tour: 0x40, same offsets.
// No base class, no vtable.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] none (it is a member, never allocated; Clear is its only method);
//   [P] none;
//   [U] how retail code uses the field (cWakeParams_Clear; cBoardWakeFX_update and cBoardWakeFX_fillCurrentSeg at
//       cBoardWakeFX + 0x60 + offset);
//   [O] SSX On Tour's DWARF (class cWakeParams 0x40: mUp, mSide, mForward, mSpeed, mAlpha, mVeryTightTurn).

#include "layout.h"
#include "bx/math3d.h"

struct cWakeParams {
    t4Vector mUp;                   // 0x00 [O]; [U] Clear = t4vect_Zero; update blends it 0.8 / 0.2 towards the ground
                                    //      normal (wake +0x60); fillCurrentSeg scales the ribbon rows by it
    t4Vector mSide;                 // 0x10 [O]; [U] Clear = 0; update blends towards the side vector (wake +0x70);
                                    //      fillCurrentSeg bends the rows with it
    t4Vector mForward;              // 0x20 [O]; [U] Clear = 0; update blends towards the forward vector (wake +0x80);
                                    //      fillCurrentSeg: segment direction = 0.85 * (velocity - mForward * |velocity|)
    float mSpeed;                   // 0x30 [O]; [U] Clear 0 (sw $0); update 0.8 / 0.2 blend of the surface's wake
                                    //      strength (wake +0x90); fillCurrentSeg scales the row width by it
    float mAlpha;                   // 0x34 [O]; [U] Clear 0 (sw $0); update 0.7 / 0.3 blend of the surface grip (wake
                                    //      +0x94), passed as the segment width; fillCurrentSeg alpha = mAlpha * 128
    float mVeryTightTurn;           // 0x38 [O]; no retail access found (Clear leaves it; the views pad it)
    char unk_3C[0x4];               // 0x3C padding (t4Vector alignment)
};

CHECK_OFFSET(cWakeParams, mUp, 0x00);
CHECK_OFFSET(cWakeParams, mSide, 0x10);
CHECK_OFFSET(cWakeParams, mForward, 0x20);
CHECK_OFFSET(cWakeParams, mSpeed, 0x30);
CHECK_OFFSET(cWakeParams, mAlpha, 0x34);
CHECK_OFFSET(cWakeParams, mVeryTightTurn, 0x38);
CHECK_OFFSET(cWakeParams, unk_3C, 0x3C);
CHECK_SIZE(cWakeParams, 0x40);

#endif /* VISUALFX_BOARDWAKEFX_H */

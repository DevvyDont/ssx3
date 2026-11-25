#ifndef RENDER_PLAYERLIGHTS_H
#define RENDER_PLAYERLIGHTS_H

// cPlayerLights: the extra lights applied to one rider (an ambient term plus up to 5 directional lights, and two
// additive-blend flags). Embedded in cRiderHandler at 0xD30 (ai/riderhandler.h); the trick-boost FX adds its
// directional light to it (cTrickBoostFX update, cTrickBoostFX_update) and the rider clears it (ai/rider.cpp, cPlayerLights_Clear).
// EA's source: d:/ssx/code/source/render/playerlights.cpp (preview assert path). Its methods sit in the render/font
// unit (On Tour: render/all_in_one_render.cpp), in On Tour's declaration order:
//   cPlayerLights_Clear Clear, cPlayerLights_AddAmbient AddAmbient (out of line in retail), cPlayerLights_AddDirectional AddDirectional(dir, colour),
//   cPlayerLights_PlayerIsAdditive PlayerIsAdditive(value), cPlayerLights_BoardIsAdditive__FPvi BoardIsAdditive(value).
// render/font.cpp views it as `struct sFontState` (same layout; sFontVec3 = tRGB, sFontQuad = t4Vector).
//
// Size 0xB0: [O] class cPlayerLights (size 0xB0); retail's two bools are words (0xA0, 0xA4), so the same size after
// the 16-byte alignment; cRiderHandler's next member (MotionState) is at 0xD30 + 0xB0 = 0xDE0. No base, no vptr.
// Tags: [O] On Tour's DWARF member names/order; [U] the retail methods above.

#include "layout.h"
#include "bx/math3d.h"

// EA's RGB colour (On Tour struct tRGB {float R, G, B;}, 0xC bytes, 4-aligned; retail copies it with ldl/ldr + lw).
// PORT/TODO: belongs in a render/bx header once a second class needs it.
struct tRGB {
    float R;                                    // 0x0 [O]; [U] AddAmbient adds component-wise
    float G;                                    // 0x4 [O]
    float B;                                    // 0x8 [O]
};

CHECK_OFFSET(tRGB, R, 0x0);
CHECK_OFFSET(tRGB, G, 0x4);
CHECK_OFFSET(tRGB, B, 0x8);
CHECK_SIZE(tRGB, 0xC);

struct t4Vector;

struct cPlayerLights {
    tRGB mExtraAmbientLight;                    // 0x00 [O]; [U] Clear zeroes it (12-byte copy), AddAmbient adds to it
    int mNumDirectionalLights;                  // 0x0C [O]; [U] Clear = 0; AddDirectional indexes with it, then ++
    tRGB mDirectionalLightColours[5];           // 0x10 [O]; [U] AddDirectional stores colour at [n] (12-byte copy)
    char pad_4C[0x4];                           // 0x4C t4Vector alignment
    t4Vector mDirectionalLightDirections[5];    // 0x50 [O]; [U] AddDirectional stores direction at [n] (sq)
    int mPlayerIsAdditive;                      // 0xA0 [O] bool (a word in retail); [U] Clear = 0, PlayerIsAdditive
    int mBoardIsAdditive;                       // 0xA4 [O] bool (word); [U] Clear = 0, BoardIsAdditive
    char pad_A8[0x8];                           // 0xA8 the class is 16-byte aligned

    // Methods bound to their retail symbols by asm labels.
    void Clear() __asm__("cPlayerLights_Clear");
    void AddAmbient(tRGB* colour) __asm__("cPlayerLights_AddAmbient");
    void AddDirectional(t4Vector* direction, tRGB* colour) __asm__("cPlayerLights_AddDirectional");
    void PlayerIsAdditive(int value) __asm__("cPlayerLights_PlayerIsAdditive");
    void BoardIsAdditive(int val) __asm__("cPlayerLights_BoardIsAdditive__FPvi");
};

CHECK_OFFSET(cPlayerLights, mExtraAmbientLight, 0x00);
CHECK_OFFSET(cPlayerLights, mNumDirectionalLights, 0x0C);
CHECK_OFFSET(cPlayerLights, mDirectionalLightColours, 0x10);
CHECK_OFFSET(cPlayerLights, pad_4C, 0x4C);
CHECK_OFFSET(cPlayerLights, mDirectionalLightDirections, 0x50);
CHECK_OFFSET(cPlayerLights, mPlayerIsAdditive, 0xA0);
CHECK_OFFSET(cPlayerLights, mBoardIsAdditive, 0xA4);
CHECK_OFFSET(cPlayerLights, pad_A8, 0xA8);
CHECK_SIZE(cPlayerLights, 0xB0);

#endif /* RENDER_PLAYERLIGHTS_H */

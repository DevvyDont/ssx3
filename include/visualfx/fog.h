#ifndef VISUALFX_FOG_H
#define VISUALFX_FOG_H

// cFogParticleMan / cPS2FogParticleMan: the camera-facing fog sprites drawn for placed fog-sphere instances (EA source
// d:/ssx/code/source/visualfx/fog.cpp: the preview's asserts pair the sort comparator func_002DC168 with it; retail
// unit seg/seg_1DCD10, plus the two destructors in src/visualfx/worldlightman.cpp). One instance, cGame::FogMan
// (main/game.h +0x18): cMemMan_alloc(0x3010, "cPS2FogParticleMan") + cFogParticleMan ctor func_002DBF80 + the
// cPS2FogParticleMan vtable D_004882E0 stored inline + 256 sprites constructed at +0x10 (cGame_loadTrack).
//   cFogParticleMan     0x8 (On Tour 0x8): miNumSphere + vptr. Vtable cFogParticleMan_vtbl: 1 dtor cFogParticleMan__cFogParticleMan, 2
//                       addFogInstance and 3 platformrender pure (func_00411530). render (cFogParticleMan_render) is
//                       non-virtual. Static mSortKey = D_00537AC0 (0x800 bytes: 256 tFogSortKey; On Tour has 128).
//   cPS2FogParticleMan  0x3010: + 256 tFogSprite. Vtable D_004882E0: 1 dtor cPS2FogParticleMan__cPS2FogParticleMan, 2 addFogInstance
//                       func_002DC190, 3 platformrender func_002DC7B0.
//
// A FLAT layout, like main/game.h. Evidence tags: [R] ctor/dtors/allocation; [U] matched retail use; [O] SSX On Tour's
// DWARF (cFogParticleMan, tFogSortKey; its cPSPFogParticleMan is a different platform class).

#include "layout.h"
#include "bx/math3d.h"

// One sort key per queued sprite (On Tour tFogSortKey, 8 bytes).
struct tFogSortKey {
    int mIdx;                       // 0x000 [O]; [U] addFogInstance stores the sprite index
    int miZ;                        // 0x004 [O]; [U] addFogInstance stores the view depth; _FogSortFuc (func_002DC168)
                                    //       sorts far-to-near on it
};

CHECK_OFFSET(tFogSortKey, mIdx, 0x00);
CHECK_OFFSET(tFogSortKey, miZ, 0x04);
CHECK_SIZE(tFogSortKey, 0x8);

// One queued screen-space fog sprite (0x30; addFogInstance/platformrender; no On Tour twin).
struct tFogSprite {
    t4Vector mTopLeft;              // 0x000 [U] addFogInstance: screen position - half size; platformrender
    t4Vector mBottomRight;          // 0x010 [U] addFogInstance: screen position + half size
    float mColour[4];               // 0x020 [U] addFogInstance: {alpha, r, g, b} * 128 (GS colour scale);
                                    //       platformrender packs them
};

CHECK_OFFSET(tFogSprite, mTopLeft, 0x00);
CHECK_OFFSET(tFogSprite, mBottomRight, 0x10);
CHECK_OFFSET(tFogSprite, mColour, 0x20);
CHECK_SIZE(tFogSprite, 0x30);

struct cFogParticleMan {
    int miNumSphere;                // 0x000 [O]; [R] ctor func_002DBF80 zeroes it; [U] render sorts mSortKey[0..n),
                                    //       draws (slot 3) and clears it; addFogInstance appends (lw/sw, and lhu for
                                    //       its unsigned-short index)
    void** mVtbl;                   // 0x004 [R] ctor stores cFogParticleMan_vtbl (cFogParticleMan: dtor cFogParticleMan__cFogParticleMan + 2 pure
                                    //       virtuals); render calls slot 3
};

CHECK_OFFSET(cFogParticleMan, miNumSphere, 0x00);
CHECK_OFFSET(cFogParticleMan, mVtbl, 0x04);
CHECK_SIZE(cFogParticleMan, 0x8);

struct cPS2FogParticleMan {
    // --- cFogParticleMan
    int miNumSphere;                // 0x000 [O]; [R] ctor func_002DBF80 zeroes it; [U] render sorts mSortKey[0..n),
                                    //       draws (slot 3) and clears it; addFogInstance appends (lw/sw, and lhu for
                                    //       its unsigned-short index)
    void** mVtbl;                   // 0x004 [R] ctor stores cFogParticleMan_vtbl (cFogParticleMan: dtor cFogParticleMan__cFogParticleMan + 2 pure
                                    //       virtuals); render calls slot 3
    // --- cPS2FogParticleMan
    char unk_008[0x8];              // 0x008 padding (On Tour's cPSPFogParticleMan keeps m_drawState here; retail has
                                    //       nothing: the sprite array is 16-aligned)
    tFogSprite mSprites[256];       // 0x010 [R] cGame_loadTrack constructs 256 x 0x30 at +0x10 (alloc 0x3010); [U]
                                    //       addFogInstance writes [miNumSphere++] (no bound check), platformrender
                                    //       reads [mSortKey[i].mIdx]
};

CHECK_OFFSET(cPS2FogParticleMan, miNumSphere, 0x00);
CHECK_OFFSET(cPS2FogParticleMan, mVtbl, 0x04);
CHECK_OFFSET(cPS2FogParticleMan, unk_008, 0x08);
CHECK_OFFSET(cPS2FogParticleMan, mSprites, 0x10);
CHECK_SIZE(cPS2FogParticleMan, 0x3010);

#endif /* VISUALFX_FOG_H */

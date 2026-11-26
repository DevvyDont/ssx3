#ifndef VISUALFX_LENSFX_H
#define VISUALFX_LENSFX_H

// The lens-flare effects: cLensFxMan (the manager), its per-view light halos (cLightHalo : cHalo) and per-view sun
// (cSunHalo : cHalo), and the view description they are processed against (tViewInfo). EA's source:
// d:/ssx/code/source/visualfx/lensfx.cpp (preview assert strings); the PS2 subclass cPS2LensFxMan is in
// visualfx/ps2lensfx.h (d:/ssx/code/source/visualfx/ps2lensfx.cpp). Retail code: cLensFxMan_getHaloRadius ...
// cLensFxMan_endDrawFrame in the visualfx/boardwakefx splat unit (0x2E27E8 ... 0x2E3680), cSunHalo_* in seg/seg_1F1548
// (0x2F4380 ... 0x2F5148), the Lock/SetNumViews accessors in visualfx/worldlightman, cLensFxMan_construct in
// visualfx/lensfx.
//
// Retail diverges from On Tour (a later game, single view, larger cHalo): retail keeps one halo list and one sun per
// view, so mNumHalo, mLightHalo and mSunHalo each gain a leading [2] (view) dimension, cHalo is 0x30 (no mVisTestID /
// mNumPixels), and tSunLens is 0x1C (its tARGB is only 4-aligned). Every field below is placed by retail code; On Tour
// supplies the names where the retail role is the same.
//
// Sizes:
//   cHalo / cLightHalo 0x30: [R] cLensFxMan_cLensFxMan constructs mLightHalo with cHalo_cHalo at a 0x30 stride
//       (0x3000 per view); cLensFxMan_AddHalo indexes count * 0x30 + view * 0x3000.
//   cSunHalo 0x210: [R] cLensFxMan_cLensFxMan constructs two with cSunHalo_cSunHalo at a 0x210 stride from +0x6070;
//       cLensFxMan_ProcessLens / DrawLens / SampleLens index view * 0x210. 16-aligned (t4Vector mScreenPosition).
//   cLensFxMan 0x64A0: [R] its vptr is the word at 0x649C (both ctors and the dtor store it) and cPS2LensFxMan's own
//       data starts at 0x64A0 (cPS2LensFxMan_sampleZBuffer's first packet); 16-aligned, so the size is 0x64A0.
// Bases: none for cHalo and cLensFxMan; cLightHalo and cSunHalo derive from cHalo (On Tour; [R] cSunHalo_cSunHalo
// runs cHalo_cHalo on +0x0). cLensFxMan is the first class with virtuals in its chain, so g++ 2.95 puts the vptr
// after its data: cLensFxMan_vtbl (0x004886B0, 4 slots: 1 dtor cLensFxMan__cLensFxMan, 2 beginDrawFrame,
// 3 endDrawFrame, 4 sampleZBuffer = __pure_virtual); cPS2LensFxMan_vtbl (0x00487F00) fills slot 4.
// The static bool cLensFxMan::mbDrawSun is the global cLensFxMan_mbDrawSun (not a member).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags:
//   [R] retail constructors / destructor and allocation size (cMemMan_alloc(0x6820, "LensFX") in
//       cLensFxMan_construct, for the PS2 subclass);
//   [P] the July 2003 preview's inline accessors cLensFxMan::SetNumViews(int) / Lock(bool) (sw 0x6490 / 0x6494, the
//       retail offsets; tools/layout_xlate.py: vptr 0x649C -> 0x649C);
//   [U] how retail code uses the field (C bodies in src/visualfx/boardwakefx.cpp, src/seg/seg_1F1548.cpp,
//       src/visualfx/worldlightman.cpp, src/main/game.cpp and the asm);
//   [O] SSX On Tour's DWARF (cLensFxMan, cHalo, cLightHalo, cSunHalo, tSunLens, tViewInfo): names, types and order.

#include "layout.h"
#include "bx/math3d.h"
#include "render/2drender.h"

// A world light halo (On Tour struct tHaloRecord : tHaloRecord_PSP : _tPSPHalo, 0x68; cHalo::mpHalo points at one).
// Retail's record is more compact than the PSP one (no intensity / direction pads) and keeps one occlusion value per
// view. Placed by [U] cChunkResolve_resolveHalo ([O] cChunkResolve::resolveHalo(tHaloRecord* pLightHalo): the box
// is the position -/+ the radius of the halo type flags & 0x70; it clears both occlusion words; its view
// cChunkResolve_resolveHalo_sObj in world/worldview.cpp) and cLightHalo_Render (the
// colour is the normalised vector at 0x10, the sprite is drawn at 0x1C, alpha = the occlusion of the view index,
// flags & 0x70 pick texture and size). The type / flags swap of tLightRecord (visualfx/worldlightman.h) shows here
// too: retail flags at 0xC. Size: 0x48 is the end of the last member retail touches; no array stride is known.
struct tHaloRecord {
    // Halo types, m_iFlags & 0x70 (names ours, by size). [U] cLensFxMan_getHaloRadius gives them radius 180, 100 and
    // 350; cLightHalo_Render draws them with effect textures 53, 54 and 53 at the same sizes.
    enum eHaloType { kHaloType_Medium = 0x10, kHaloType_Small = 0x20, kHaloType_Large = 0x40 };
    char unk_000[0xC];              // 0x00 (On Tour: the Nash node header m_iIntersect[3]); no retail access seen
    unsigned int m_iFlags;          // 0x0C [O] m_iFlags; [U] resolveHalo / cLightHalo_Render: flags & 0x70 = the
                                    //      halo type (0x10, 0x20, 0x40)
    float m_fvColor[3];             // 0x10 [O] tf3Vector (the first vector, as in On Tour); [U] cLightHalo_Render:
                                    //      normalised, it gives the sprite's r, g, b
    float m_fvWorldPos[3];          // 0x1C [O] tf3Vector; [U] resolveHalo centres the box on it; cLightHalo_Render
                                    //      draws the sprite there
    float m_fvMin[3];               // 0x28 [O] tf3Vector; [U] resolveHalo = m_fvWorldPos - radius
    float m_fvMax[3];               // 0x34 [O] tf3Vector; [U] resolveHalo = m_fvWorldPos + radius
    float m_fOcclude[2];            // 0x40 [O] m_fOcclude (one float on the single-view PSP); [U] resolveHalo clears
                                    //      both; cLightHalo_Render reads [view] as the alpha (cLensFxMan's 2 views)
};

CHECK_OFFSET(tHaloRecord, unk_000, 0x00);
CHECK_OFFSET(tHaloRecord, m_iFlags, 0x0C);
CHECK_OFFSET(tHaloRecord, m_fvColor, 0x10);
CHECK_OFFSET(tHaloRecord, m_fvWorldPos, 0x1C);
CHECK_OFFSET(tHaloRecord, m_fvMin, 0x28);
CHECK_OFFSET(tHaloRecord, m_fvMax, 0x34);
CHECK_OFFSET(tHaloRecord, m_fOcclude, 0x40);
CHECK_SIZE(tHaloRecord, 0x48);

// The view the halos are processed against (On Tour tViewInfo, 0x60). cLensFxMan_ProcessLens fills it from the
// graphics manager each view; cSunHalo_Process / cLightHalo_Process read it.
struct tViewInfo {
    tRect mViewRect;                // 0x000 [O]; [U] ProcessLens stores GrMan slot 28 (GetViewportWindow); the halos
                                    //       clamp their sample rectangles inside it
    t4Matrix mViewMat;              // 0x010 [O]; [U] ProcessLens copies GrMan slot 35's matrix (lqc2/sqc2); the
                                    //       Process functions read its rows
    float mFOV;                     // 0x050 [O]; [U] ProcessLens passes &mFOV, &mZNear, &mZFar to GrMan slot 30
                                    //       (GetViewportPerspective)
    float mZNear;                   // 0x054 [O]; [U] ditto
    float mZFar;                    // 0x058 [O]; [U] ditto; cSunHalo_Process puts the sun at mZFar - 500
    char unk_05C[0x4];              // 0x05C padding (t4Matrix alignment)
};

CHECK_OFFSET(tViewInfo, mViewRect, 0x000);
CHECK_OFFSET(tViewInfo, mViewMat, 0x010);
CHECK_OFFSET(tViewInfo, mFOV, 0x050);
CHECK_OFFSET(tViewInfo, mZNear, 0x054);
CHECK_OFFSET(tViewInfo, mZFar, 0x058);
CHECK_OFFSET(tViewInfo, unk_05C, 0x05C);
CHECK_SIZE(tViewInfo, 0x60);

// A screen-space halo: the rectangle whose depth samples decide its visibility (On Tour cHalo, 0x38; retail 0x30).
struct cHalo {
    tRect mZRect;                   // 0x000 [O]; [U] cLightHalo_Process: the depth-scaled sample area (up to 16 x 8)
                                    //       centred on the halo; cPS2LensFxMan_sampleZBuffer counts samples inside it
                                    //       and compares the count with W * H
    tRect mZSRect;                  // 0x010 [O]; [U] cLightHalo_Process: the 16 x 8 (sun: 16 x 16) area clamped
                                    //       inside the view; sampleZBuffer reads it back from the GS (TRXPOS/TRXREG)
    tHaloRecord* mpHalo;            // 0x020 [O]; [U] cLensFxMan_AddHalo stores the record; cLightHalo_Render/Process
                                    //       read its type (+0xC & 0x70), colour (+0x10), position (+0x1C) and
                                    //       per-view visibility (+0x40); SampleLens writes that visibility
    float mZDist;                   // 0x024 [O]; [U] Process stores the projected depth; sampleZBuffer's threshold
                                    //       (fptoui)
    float mAngle;                   // 0x028 [O]; [U] cLightHalo_Process stores (screen x in -1..1) * pi/2;
                                    //       cLightHalo_Render draws with it as the rotation; AddHalo zeroes it
    unsigned short mbOnScreen;      // 0x02C [O]; [U] Process sets/clears it; Render and DrawLens skip a halo without
                                    //       it; [R] cSunHalo_cSunHalo zeroes it
    unsigned short mbRequestZSample; // 0x02E [O]; [U] Process sets it when the halo is far enough to sample;
                                    //       SampleLens samples only these; Render picks the sampled alpha by it
};

CHECK_OFFSET(cHalo, mZRect, 0x000);
CHECK_OFFSET(cHalo, mZSRect, 0x010);
CHECK_OFFSET(cHalo, mpHalo, 0x020);
CHECK_OFFSET(cHalo, mZDist, 0x024);
CHECK_OFFSET(cHalo, mAngle, 0x028);
CHECK_OFFSET(cHalo, mbOnScreen, 0x02C);
CHECK_OFFSET(cHalo, mbRequestZSample, 0x02E);
CHECK_SIZE(cHalo, 0x30);

// A world light's halo (On Tour cLightHalo : cHalo, no data of its own; methods Render / Process). Same layout as
// cHalo, written out flat.
struct cLightHalo {
    // --- cHalo
    tRect mZRect;                   // 0x000 [O] (see cHalo)
    tRect mZSRect;                  // 0x010 [O]
    tHaloRecord* mpHalo;            // 0x020 [O]
    float mZDist;                   // 0x024 [O]
    float mAngle;                   // 0x028 [O]
    unsigned short mbOnScreen;      // 0x02C [O]
    unsigned short mbRequestZSample; // 0x02E [O]
    // --- cLightHalo (no data)

    // Methods bound to their retail symbols by asm labels.
    void Render(int idx) __asm__("cLightHalo_Render");
    void Process(tViewInfo* viewInfo) __asm__("cLightHalo_Process");
};

CHECK_OFFSET(cLightHalo, mZRect, 0x000);
CHECK_OFFSET(cLightHalo, mZSRect, 0x010);
CHECK_OFFSET(cLightHalo, mpHalo, 0x020);
CHECK_OFFSET(cLightHalo, mZDist, 0x024);
CHECK_OFFSET(cLightHalo, mAngle, 0x028);
CHECK_OFFSET(cLightHalo, mbOnScreen, 0x02C);
CHECK_OFFSET(cLightHalo, mbRequestZSample, 0x02E);
CHECK_SIZE(cLightHalo, 0x30);

// One element of the sun's lens flare (On Tour tSunLens, 0x20 with a 16-aligned tARGB; retail 0x1C).
struct tSunLens {
    int mType;                      // 0x000 [O]; [U] cSunHalo_InitLens stores 0..3; cSunHalo_RenderLens picks the
                                    //       texture quarter by it
    float mPosition;                // 0x004 [O]; [U] RenderLens: the element sits at centre + (sun - centre) * this
    float mScale;                   // 0x008 [O]; [U] RenderLens draws a mScale * 300 square
    float mColor[4];                // 0x00C [O] tARGB {A, R, G, B}; [U] InitLens copies D_004FAB80.. (sdl/sdr pairs, so
                                    //       4-aligned); RenderLens scales the first float (alpha) by the occlusion
};

CHECK_OFFSET(tSunLens, mType, 0x000);
CHECK_OFFSET(tSunLens, mPosition, 0x004);
CHECK_OFFSET(tSunLens, mScale, 0x008);
CHECK_OFFSET(tSunLens, mColor, 0x00C);
CHECK_SIZE(tSunLens, 0x1C);

// The sun's halo and lens flare for one view (On Tour cSunHalo : cHalo, 0x260; retail 0x210).
struct cSunHalo {
    // --- cHalo (0x30)
    tRect mZRect;                   // 0x000 [O]; [U] cSunHalo_Process: the 16 x 16 square around the sun
    tRect mZSRect;                  // 0x010 [O]; [U] Process: a copy clamped inside the view
    tHaloRecord* mpHalo;            // 0x020 [O]; not used by the sun's code
    float mZDist;                   // 0x024 [O]; [U] Process stores the projected depth
    float mAngle;                   // 0x028 [O]; not used by the sun's code
    unsigned short mbOnScreen;      // 0x02C [O]; [R] ctor zeroes; [U] Process sets/clears; Render skips without it
    unsigned short mbRequestZSample; // 0x02E [O]; [R] ctor zeroes; [U] Process sets/clears; SampleLens samples by it
    // --- cSunHalo
    tSunLens mSunLens[16];          // 0x030 [O]; [U] InitLens fills entries 0..8; RenderLens draws mNumLens of them
    t4Vector mScreenPosition;       // 0x1F0 [O]; [U] Process stores GrMan slot 92's projection (sq); Render and
                                    //       RenderLens read x, y
    int mNumLens;                   // 0x200 [O]; [U] InitLens zeroes and counts it up; RenderLens loops over it
    float mfOcclude;                // 0x204 [O]; [R] ctor zeroes; [U] SampleLens stores the visible fraction; Process
                                    //       zeroes it when culled; Render / RenderLens scale the alpha by it
    char unk_208[0x8];              // 0x208 padding (16-aligned class)

    // Methods bound to their retail symbols by asm labels.
    void RenderLens() __asm__("cSunHalo_RenderLens");
    void Render(tViewInfo* viewInfo, int viewId) __asm__("cSunHalo_Render");
    void InitLens() __asm__("cSunHalo_InitLens");
};

CHECK_OFFSET(cSunHalo, mZRect, 0x000);
CHECK_OFFSET(cSunHalo, mZSRect, 0x010);
CHECK_OFFSET(cSunHalo, mpHalo, 0x020);
CHECK_OFFSET(cSunHalo, mZDist, 0x024);
CHECK_OFFSET(cSunHalo, mAngle, 0x028);
CHECK_OFFSET(cSunHalo, mbOnScreen, 0x02C);
CHECK_OFFSET(cSunHalo, mbRequestZSample, 0x02E);
CHECK_OFFSET(cSunHalo, mSunLens, 0x030);
CHECK_OFFSET(cSunHalo, mScreenPosition, 0x1F0);
CHECK_OFFSET(cSunHalo, mNumLens, 0x200);
CHECK_OFFSET(cSunHalo, mfOcclude, 0x204);
CHECK_OFFSET(cSunHalo, unk_208, 0x208);
CHECK_SIZE(cSunHalo, 0x210);

// The lens-flare manager (On Tour cLensFxMan, 0x3AE0, one view; retail 0x64A0, two views). The game's instance is
// cGame::LensFxMan (main/game.h), a cPS2LensFxMan.
struct cLensFxMan {
    tViewInfo mViewInfo;            // 0x000 [O]; [U] cLensFxMan_ProcessLens fills it; cSunHalo_Process /
                                    //       cLightHalo_Process get &mViewInfo
    int mNumHalo[2];                // 0x060 [O] (one per view in retail); [R] ctor zeroes both; [U] Reset(view) zeroes,
                                    //       AddHalo counts up, ProcessLens / DrawLens / SampleLens loop over it
    cLightHalo mLightHalo[2][256];  // 0x068 [O] (per view in retail); [R] ctor runs cHalo_cHalo on each (0x30 stride,
                                    //       0x3000 per view); [U] AddHalo fills [view][mNumHalo[view]]
    char unk_6068[0x8];             // 0x6068 padding (cSunHalo is 16-aligned)
    cSunHalo mSunHalo[2];           // 0x6070 [O] (per view in retail); [R] ctor runs cSunHalo_cSunHalo on both;
                                    //       [U] ProcessLens / DrawLens / SampleLens use [view]
    int mNumViews;                  // 0x6490 [O]; [P] SetNumViews (sw); [U] SampleLens's view loop bound
    int mbLocked;                   // 0x6494 [O] (bool, a word in retail); [P] Lock(bool) (sw); [R] ctor sets 1;
                                    //       [U] SampleLens does nothing while set; cGame_renderLightHalos sets it
                                    //       around its halo update
    int unk_6498;                   // 0x6498 placed by the vptr at 0x649C; no retail access (On Tour's next member
                                    //       after mbLocked is int mTexID)
    void** mVtbl;                   // 0x649C [R] cLensFxMan_vtbl (ctor, dtor), cPS2LensFxMan_vtbl (PS2 ctor, dtor);
                                    //       [U] DrawLens calls slots 2 / 3, SampleLens slot 4; [P] same offset

    // Methods bound to their retail symbols by asm labels.
    void Reset(int i) __asm__("cLensFxMan_Reset__FPvi");
    void AddLightHalos(int* vals, int count, int ignored, int i) __asm__("cLensFxMan_AddLightHalos");
    void render(int i) __asm__("cLensFxMan_render");
    void ProcessLens(int idx) __asm__("cLensFxMan_ProcessLens");
    void SetNumViews(int numViews) __asm__("cLensFxMan_SetNumViews");
    void Lock(int locked) __asm__("cLensFxMan_Lock");
};

CHECK_OFFSET(cLensFxMan, mViewInfo, 0x000);
CHECK_OFFSET(cLensFxMan, mNumHalo, 0x060);
CHECK_OFFSET(cLensFxMan, mLightHalo, 0x068);
CHECK_OFFSET(cLensFxMan, unk_6068, 0x6068);
CHECK_OFFSET(cLensFxMan, mSunHalo, 0x6070);
CHECK_OFFSET(cLensFxMan, mNumViews, 0x6490);
CHECK_OFFSET(cLensFxMan, mbLocked, 0x6494);
CHECK_OFFSET(cLensFxMan, unk_6498, 0x6498);
CHECK_OFFSET(cLensFxMan, mVtbl, 0x649C);
CHECK_SIZE(cLensFxMan, 0x64A0);

#endif /* VISUALFX_LENSFX_H */

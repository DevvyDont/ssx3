#ifndef RENDER_BEZIERMAN_H
#define RENDER_BEZIERMAN_H

// cBezierMan: the abstract Bezier patch manager - the base of cPS2BezierMan (render/ps2bezierman.h), which does all the
// work (patch queues, LODs, VIF caches, drawing). The base keeps the patch distance and the vptr.
// cBezierMan::TheBezierMan (retail cBezierMan_TheBezierMan, gp+0x1158) is the singleton: the ctor sets it, the dtor
// clears it, cBezierMan_getBezierMan returns it; cGame_gainFocus, cGame_renderPatches, patchLodContext_begin and
// cPS2DeferManager_writeVu1ProgramSetup read it. cBezierMan_construct__Fv (On Tour cBezierMan::construct,
// render/bezierman) creates the PS2 manager: cMemMan_alloc(0x9C90, "PS2BezierMan") + cPS2BezierMan_cPS2BezierMan__FPv.
// EA's source path is not in the preview's strings; the header name follows the unit, render/bezierman.cpp.
//
// Size 0x8: [O] On Tour's cBezierMan is 0x8 (m_currPatchDist at 0x0, then the vptr); [R] cBezierMan_cBezierMan /
// cBezierMan__cBezierMan store cBezierMan_vtbl at 0x4 (g++ 2.95: the vptr follows the own data of the class that
// declares the first virtual); [P] the preview's GetPatchDist__10cBezierMan / SetPatchDist__10cBezierManf read and
// write the float at 0x0, its inline AddPatch loads the vptr from 0x4.
// No base class. A FLAT layout, like main/game.h. cPS2BezierMan's header writes these two fields out inline at the
// same offsets (m_currPatchDist 0x0, mVtbl 0x4) - consistent; including this header there changes no offset.
//
// Vtable cBezierMan_vtbl (0x00493820; 8-byte {delta, fn} entries, entry 0 = RTTI), names and order [O] On Tour's
// cBezierMan, [P] the preview's _vt$10cBezierMan (0x461500, the same 22 slots):
//   1 ~cBezierMan (cBezierMan__cBezierMan)  2 Init  3 Purge  4 Reset (cBezierMan_Reset__FPv, empty)
//   5 UpdatePatchesCache (cBezierMan_UpdatePatchesCache__FPv, empty)  6 SetNearLOD  7 SetFarLOD  8 GetNearLOD
//   9 GetFarLOD  10 SetLmapStart  11 EnableWireMode  12 EnableLightMaps  13 EnableShadows  14 IsWireModeEnabled
//   15 IsLightMapsEnabled  16 IsShadowsEnabled  17 ClearPatches  18 DrawPatches
//   19 SetPatchDist (cBezierMan_SetPatchDist__FPvf)  20 GetPatchDist (cBezierMan_GetPatchDist__FPv)
//   21 AddPatch (pure; cPS2BezierMan_AddPatch)
//   22 AddPatch(tPatchRecord*, const unsigned char*, const int*, bool, tLightMats*) ([P] the preview's mangled name):
//      cBezierMan_AddPatch, which calls slot 22 again through the vptr (+0xB0) - cPS2BezierMan overrides slot 22
//      with cPS2BezierMan_queuePatch (func_0038B190), so it forwards there. On Tour declares a single AddPatch.
// Slots 2, 3, 6-18 and 21 are __pure_virtual in cBezierMan_vtbl.

#include "layout.h"

// The vtable slots above, as indexes into the {delta, fn} entries (include/vtable.h sVEntry<F>). AddPatch2 (name
// ours) is the second AddPatch overload, slot 22.
enum {
    kBezierManVt_dtor = 1, kBezierManVt_Init = 2, kBezierManVt_Purge = 3, kBezierManVt_Reset = 4,
    kBezierManVt_UpdatePatchesCache = 5, kBezierManVt_SetNearLOD = 6, kBezierManVt_SetFarLOD = 7,
    kBezierManVt_GetNearLOD = 8, kBezierManVt_GetFarLOD = 9, kBezierManVt_SetLmapStart = 10,
    kBezierManVt_EnableWireMode = 11, kBezierManVt_EnableLightMaps = 12, kBezierManVt_EnableShadows = 13,
    kBezierManVt_IsWireModeEnabled = 14, kBezierManVt_IsLightMapsEnabled = 15, kBezierManVt_IsShadowsEnabled = 16,
    kBezierManVt_ClearPatches = 17, kBezierManVt_DrawPatches = 18, kBezierManVt_SetPatchDist = 19,
    kBezierManVt_GetPatchDist = 20, kBezierManVt_AddPatch = 21, kBezierManVt_AddPatch2 = 22
};

struct cBezierMan {
    float m_currPatchDist;           // 0x0 [O] cBezierMan::m_currPatchDist; [P] GetPatchDist (lwc1 0) / SetPatchDist
                                     //     (swc1 0); [U] cBezierMan_GetPatchDist__FPv / cBezierMan_SetPatchDist__FPvf
                                     //     (slots 20 / 19); no ctor initialises it
    void** mVtbl;                    // 0x4 [R] cBezierMan_cBezierMan / cBezierMan__cBezierMan store cBezierMan_vtbl,
                                     //     cPS2BezierMan's ctor / dtor its own (0x00493760); [U] cBezierMan_AddPatch,
                                     //     cPS2BezierMan_AddPatch, cPS2BezierMan_invokeClearPatches, cGame_gainFocus,
                                     //     cGame_renderPatches call through it

    // Methods bound to their retail symbols by asm labels.
    void Reset() __asm__("cBezierMan_Reset__FPv");
    void UpdatePatchesCache() __asm__("cBezierMan_UpdatePatchesCache__FPv");
    void SetPatchDist(float val) __asm__("cBezierMan_SetPatchDist__FPvf");
    float GetPatchDist() __asm__("cBezierMan_GetPatchDist__FPv");
    void AddPatch(int patchRecord, int edgeLods, int textureIds, int clip, int lightMats)
        __asm__("cBezierMan_AddPatch");
};

CHECK_OFFSET(cBezierMan, m_currPatchDist, 0x0);
CHECK_OFFSET(cBezierMan, mVtbl, 0x4);
CHECK_SIZE(cBezierMan, 0x8);

// The Bezier manager singleton ([O] static cBezierMan* TheBezierMan).
extern cBezierMan* cBezierMan_TheBezierMan;

#endif /* RENDER_BEZIERMAN_H */

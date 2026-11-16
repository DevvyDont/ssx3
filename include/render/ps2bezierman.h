#ifndef RENDER_PS2BEZIERMAN_H
#define RENDER_PS2BEZIERMAN_H

// cPS2BezierMan: the Bezier patch manager (cBezierMan::TheBezierMan): queues the visible terrain patches per LOD
// (4x4, 6x6, 8x8 tessellation, or mixed edges) and draws them through three VIF caches. Retail keeps On Tour's PSP
// class name (cPS2BezierMan_cPS2BezierMan__FPv, vtables.tsv); the allocation tag says "PS2BezierMan".
// Size 0x9C90: [R] cBezierMan_construct__Fv (render/bezierman) does cMemMan_alloc(0x9C90, "PS2BezierMan") + the ctor
// cPS2BezierMan_cPS2BezierMan__FPv (render/ps2bezierman): cBezierMan ctor func_00374C90 (vtable D_00493820, stores
// TheBezierMan), vtable D_00493760, func_00372B30 on this + 0x10, then Init (cPS2BezierMan_Init). Dtor cPS2BezierMan__cPS2BezierMan (Purge,
// then cBezierMan dtor func_00374CA8, which clears TheBezierMan).
// Bases: cBezierMan (On Tour 0x8: m_currPatchDist 0x0, then its vptr at 0x4) <- cPS2BezierMan.
// Methods (D_00493760; the same order as On Tour's cBezierMan declarations): 0 ~ cPS2BezierMan__cPS2BezierMan, 1 Init cPS2BezierMan_Init,
// 2 Purge cPS2BezierMan_Purge, 3 Reset cPS2BezierMan_Reset__FPv, 4 UpdatePatchesCache func_00394B80__FPv (empty), 5/6 SetNearLOD/
// SetFarLOD cPS2BezierMan_SetNearLOD/C8__FPvf, 7/8 GetNearLOD/GetFarLOD cPS2BezierMan_GetNearLOD/D8__FPv, 9 SetLmapStart cPS2BezierMan_SetLmapStart__FPvi,
// 10-12 EnableWireMode/EnableLightMaps/EnableShadows cPS2BezierMan_EnableWireMode/F0/F8__FPvi, 13-15 IsWireModeEnabled/
// IsLightMapsEnabled/IsShadowsEnabled cPS2BezierMan_IsWireModeEnabled/08/10__FPv, 16 ClearPatches func_0038B178__FPv, 17 DrawPatches
// cPS2BezierMan_DrawPatches, 18/19 SetPatchDist/GetPatchDist func_00394B88__FPvf / func_00394B90__FPv (cBezierMan's), 20 AddPatch
// cPS2BezierMan_AddPatch, 21 func_0038B190 (queues one patch by its edge LODs). cBezierMan's own slots (D_00493820):
// func_00394B78__FPv (3), func_00394B80__FPv (4), func_00394B98 (21: calls slot 20). Non-virtual accessors:
// func_00394C18 (cache pointer + slot), func_00394C30 (lw 0x1A0), func_00394C38 (lw 0x320).
// Instance global: cBezierMan_TheBezierMan (gp+0x1158) = cBezierMan::TheBezierMan (func_00374C90 stores it, func_00374CA8 clears
// it); func_00394B70 returns it (getBezierMan).
//
// FLAT layout. Members: [O] On Tour's cBezierMan / cPSPBezierMan (mNearLODcm, mFarLODcm and the four enable flags, which
// are bools there and words here), [R] Init (cPS2BezierMan_Init), [U] the accessors and the patch-queue functions. The
// patch cache at 0x10 and the tail from 0x5364 (used by DrawPatches) are not laid out yet.

#include "layout.h"

// One queued patch (0x10). [U] func_0038B190 stores mpPatch (a1), unk_04 (t0), mLod (0/1/2 for the 4x4/6x6/8x8 lists),
// and a halfword read from the patch record (+0x1A6/+0x1A8/+0x1AA) at 0xC.
struct tBezierPatchEntry {
    void* mpPatch;                   // 0x0 [U]
    void* unk_04;                    // 0x4 [U] the caller's second pointer
    int mLod;                        // 0x8 [U] 0, 1 or 2
    unsigned short unk_0C;           // 0xC [U] sh
    char unk_0E[0x2];                // 0xE padding
};

CHECK_OFFSET(tBezierPatchEntry, mpPatch, 0x0);
CHECK_OFFSET(tBezierPatchEntry, unk_04, 0x4);
CHECK_OFFSET(tBezierPatchEntry, mLod, 0x8);
CHECK_OFFSET(tBezierPatchEntry, unk_0C, 0xC);
CHECK_SIZE(tBezierPatchEntry, 0x10);

// One queued patch with mixed edge LODs (0x14): a tBezierPatchEntry plus its four edge LOD bytes (copied in a loop).
struct tBezierMixedPatchEntry {
    void* mpPatch;                   // 0x00 [U]
    void* unk_04;                    // 0x04 [U]
    int mLod;                        // 0x08 [U] 2 if any edge is 8, else 0
    unsigned short unk_0C;           // 0x0C [U] sh
    char unk_0E[0x2];                // 0x0E padding
    unsigned char mEdgeLods[4];      // 0x10 [U] sb loop over the four edge LODs
};

CHECK_OFFSET(tBezierMixedPatchEntry, mpPatch, 0x00);
CHECK_OFFSET(tBezierMixedPatchEntry, mEdgeLods, 0x10);
CHECK_SIZE(tBezierMixedPatchEntry, 0x14);

struct cPS2BezierMan {
    // --- cBezierMan
    float m_currPatchDist;           // 0x0000 [O]; [U] func_00394B88 SetPatchDist (swc1), func_00394B90 GetPatchDist (lwc1)
    void** mVtbl;                    // 0x0004 [R] D_00493820 (cBezierMan ctor/dtor), D_00493760 (ctor, dtor cPS2BezierMan__cPS2BezierMan)
    // --- cPS2BezierMan
    char unk_0008[0x8];              // 0x0008 (no retail access found)
    char mPatchCache[0x32C];         // 0x0010 [R] func_00372B30 / func_00372B78(this + 0x10, 900, 220, 140, ...) in the
                                     //        ctor and Init, func_003739D0 in Reset; not laid out (func_00394C30 reads
                                     //        this + 0x1A0, func_00394C38 this + 0x320)
    float mNearLODcm;                // 0x033C [O]; [R] Init = 3000.0; [U] slots 5 / 7
    float mFarLODcm;                 // 0x0340 [O]; [R] Init = 15000.0; [U] slots 6 / 8
    int mWireModeEnabled;            // 0x0344 [O] bool (a word); [R] Init = 1; [U] slots 10 / 13
    int mBasePassEnabled;            // 0x0348 [O] bool (a word; On Tour order); [R] Init = 1
    int mLightMapsEnabled;           // 0x034C [O] bool (a word); [R] Init = 1; [U] slots 11 / 14
    int mShadowsEnabled;             // 0x0350 [O] bool (a word); [R] Init = 1; [U] slots 12 / 15
    int mLmapStart;                  // 0x0354 [U] SetLmapStart (slot 9, cPS2BezierMan_SetLmapStart); [R] Init = 0
    float unk_0358;                  // 0x0358 [R] Init = 8.0
    char unk_035C[0xE4];             // 0x035C (no retail access found in these methods)
    void* mVifCache[3];              // 0x0440 [R] Init: new(0x38400, "4x4VifCache"), new(0xDC00, "6x6VifCache"),
                                     //        new(0x8C00, "8x8VifCache"); [U] Purge frees them
    void* mVifCachePos[3];           // 0x044C [R] Init copies mVifCache; [U] func_00394C18 returns mVifCachePos[i] + (n << 8)
    int mNum4x4Patches;              // 0x0458 [U] ClearPatches = 0; func_0038B190 appends (limit 900)
    tBezierPatchEntry m4x4Patches[900];   // 0x045C [U] func_0038B190 (lod 4)
    int mNum6x6Patches;              // 0x3C9C [U] ClearPatches = 0; func_0038B190 appends (limit 220)
    tBezierPatchEntry m6x6Patches[220];   // 0x3CA0 [U] func_0038B190 (lod 6)
    int mNum8x8Patches;              // 0x4A60 [U] ClearPatches = 0; func_0038B190 appends (limit 50)
    tBezierPatchEntry m8x8Patches[50];    // 0x4A64 [U] func_0038B190 (lod 8)
    int mNumMixedPatches;            // 0x4D84 [U] ClearPatches = 0; func_0038B190 appends (limit 75)
    tBezierMixedPatchEntry mMixedPatches[75]; // 0x4D88 [U] func_0038B190 (edges differ)
    char unk_5364[0x492C];           // 0x5364 [U] DrawPatches cPS2BezierMan_DrawPatches (sw 0x5364, ...); not laid out
};

CHECK_OFFSET(cPS2BezierMan, m_currPatchDist, 0x0000);
CHECK_OFFSET(cPS2BezierMan, mVtbl, 0x0004);
CHECK_OFFSET(cPS2BezierMan, unk_0008, 0x0008);
CHECK_OFFSET(cPS2BezierMan, mPatchCache, 0x0010);
CHECK_OFFSET(cPS2BezierMan, mNearLODcm, 0x033C);
CHECK_OFFSET(cPS2BezierMan, mFarLODcm, 0x0340);
CHECK_OFFSET(cPS2BezierMan, mWireModeEnabled, 0x0344);
CHECK_OFFSET(cPS2BezierMan, mBasePassEnabled, 0x0348);
CHECK_OFFSET(cPS2BezierMan, mLightMapsEnabled, 0x034C);
CHECK_OFFSET(cPS2BezierMan, mShadowsEnabled, 0x0350);
CHECK_OFFSET(cPS2BezierMan, mLmapStart, 0x0354);
CHECK_OFFSET(cPS2BezierMan, unk_0358, 0x0358);
CHECK_OFFSET(cPS2BezierMan, unk_035C, 0x035C);
CHECK_OFFSET(cPS2BezierMan, mVifCache, 0x0440);
CHECK_OFFSET(cPS2BezierMan, mVifCachePos, 0x044C);
CHECK_OFFSET(cPS2BezierMan, mNum4x4Patches, 0x0458);
CHECK_OFFSET(cPS2BezierMan, m4x4Patches, 0x045C);
CHECK_OFFSET(cPS2BezierMan, mNum6x6Patches, 0x3C9C);
CHECK_OFFSET(cPS2BezierMan, m6x6Patches, 0x3CA0);
CHECK_OFFSET(cPS2BezierMan, mNum8x8Patches, 0x4A60);
CHECK_OFFSET(cPS2BezierMan, m8x8Patches, 0x4A64);
CHECK_OFFSET(cPS2BezierMan, mNumMixedPatches, 0x4D84);
CHECK_OFFSET(cPS2BezierMan, mMixedPatches, 0x4D88);
CHECK_OFFSET(cPS2BezierMan, unk_5364, 0x5364);
CHECK_SIZE(cPS2BezierMan, 0x9C90);

#endif /* RENDER_PS2BEZIERMAN_H */

#ifndef WORLD_PATCHRECORD_H
#define WORLD_PATCHRECORD_H

// tPatchRecord: one Bezier terrain patch of a world section (the records cGameRender::patchesinside / patchescrossing,
// cOverlapList::patches, tBezierPatchEntry::mpPatch and the rider's surface Patch point at). The PS2 renderer copies a
// record to the scratchpad and draws it from there (render/bezierman), and caches its tessellation in the Bezier patch
// cache (render/particle func_003747A0).
// Size 0x1B0: [U] func_0038CA70 / func_0038CE20 / func_0038D968 copy 432 bytes of it to the scratchpad
// (func_0038F708(obj, 432, 2)); [O] the PSP record is likewise padded to a 16-byte multiple after its last field
// (tPatchRecord_PSP::m_PadBytes).
// Name and member names: [O] SSX On Tour's DWARF (struct tPatchRecord : tPatchRecord_PSP : _tPSPPatch, 0x290). Retail
// agrees with _tPSPPatch up to 0x140 (the PSP record adds m_fmControl[4][4][4] there); after it only the members that
// retail use places are named. Its vectors are the float-array tf* types ([O]; world/instancerecord.h).
// EA's header: unknown (On Tour's DWARF has no file). Local definitions it replaces: the patch views of
// render/bezierman (patchRec / objData / patchCopy byte offsets) and render/particle (the mat / src parameters).
//
// Evidence tags: [O] On Tour's DWARF; [P] the July 2003 preview's asserts; [U] how retail code uses the field.

#include "layout.h"
#include "world/instancerecord.h" /* tf3Vector */

struct tPatchRecord {
    nash_Node m_iIntersect;         // 0x000 [O] int m_iIntersect[2] (typed as tInstanceRecord's); [U] the nash link:
                                    //       FrustumIntersector_AddAllPatchesInside walks a cell's patch list
                                    //       (nodesinside[1]) through next
    short m_sSurfaceType;           // 0x008 [O]; [U] terrainIntersect (src/intersect/worldsphtree.cpp, view
                                    //       sTriObj_333EF8 id) copies it into the hit's surface type
    unsigned short m_sSurfaceFlags; // 0x00A [O] short; [U] terrainIntersect tests & 0x41 (lhu)
    // The m_sSurfaceFlags bits a patch needs for collision (name ours): [U] terrainIntersect and
    // TerrainCallback_intersectpatch skip a patch unless both are set; cWorldCache::activateSectionPatches sets 0x40
    // on each patch of a section that comes in (no retail write of 0x01 seen).
    enum { kCollideFlags = 0x41 };
    // A texture layer's type, 3 bits per layer in m_iFlags (names ours). [U] cRenderStateMan_calculatePatchColour
    // samples a kPatchLayer_Lightmap layer in the m_fMapScale rectangle (the lightmap UVs, func_00374440) and a
    // kPatchLayer_Base layer at the m_fUV0 corner UVs, giving its base and lightmap colours.
    enum ePatchLayerType { kPatchLayer_Base = 1, kPatchLayer_Lightmap = 5 };
    int m_iFlags;                   // 0x00C [O]; [U] bezierman: 0x800000 snow sparkle (func_0038D690), 0x600000 shine
                                    //       (func_0038CA70 / CE20, Render_B370), 0x400000 (func_0038D168), 0x180000
                                    //       pass selection (func_0038CA70)
    float m_fMapScale[4];           // 0x010 [O]; [U] func_00374440: the lightmap-UV rectangle (origin +0x10, size +0x18)
    float m_fUV0[2][4];             // 0x020 [O]; [U] func_00374298: the four corner UVs (+0x20 / +0x28 first row,
                                    //       +0x30 / +0x38 last row)
    float m_fBm0[4][4];             // 0x040 [O]; [U] func_00374180 / func_00374518 load the 16 coefficient rows from
                                    //       +0x40 (four 4x4 power-basis blocks); func_0038D690 copies them
    float m_fBm1[4][4];             // 0x080 [O]; [U] as m_fBm0
    float m_fBm2[4][4];             // 0x0C0 [O]; [U] as m_fBm0
    float m_fBm3[4][4];             // 0x100 [O]; [U] as m_fBm0
    tf4Vector m_fvBSphere;          // 0x140 [O] name (On Tour's sits elsewhere in its larger record); [U] the bounding
                                    //       sphere: FrustumIntersector_crossingpatch loads it into vf22 for the VU0
                                    //       box test (as crossinginstance does tInstanceRecord::m_fvBSphere);
                                    //       AddAllPatchesInside sphere-tests it (microprogram 0xEF0)
    unsigned int m_RID;             // 0x150 [O] tResourceID m_RID (On Tour's name; placed by retail use); [U] the
                                    //       patch's id: cAirMotion_resolve / cGroundMotion_setupGroundVectors copy it
                                    //       to the rider's mGroundPolyId, cAirPredictor_simStep to GuessLandPatchRID,
                                    //       the ground hit sort (isBetter_13D1B8) compares it unsigned
    tStreamMap m_SMap;              // 0x154 [O] (in On Tour after the box); [U] FrustumIntersector_crossingpatch /
                                    //       AddAllPatchesInside: section +0x155 (lbu), hull page +0x156 (lh), tested
                                    //       as crossinginstance tests tInstanceRecord::m_SMap
    tf3Vector m_fvMin;              // 0x158 [O] (after the bound sphere in On Tour); [U] func_0038CA08 passes it as Min
                                    //       of cPS2GraphicsMan_CheckProjectiveTexturesHits ([O] (const t4Vector& Min,
                                    //       const t4Vector& Max)); func_0038D690: the box diagonal (sparkle count)
    tf3Vector m_fvMax;              // 0x164 [O]; [U] func_0038CA08: Max; func_0038D690
    char unk_170[0x30];             // 0x170 no retail access checked (On Tour: m_iPatchIndex; its m_RID and m_SMap
                                    //       are at 0x150 / 0x154 here)
    short m_sTextures[3];           // 0x1A0 [O]; [U] func_0038CA70: +0x1A0 / +0x1A2 go into the draw state's two texture
                                    //       ids (and the deferred entry's tex / tex2); func_0038D168: +0x1A4 = the shine
                                    //       texture (mSpecTexID when negative)
    short m_sCacheRecord[3];        // 0x1A6 [P] assert "Patch->m_sCacheRecord[Size] < 0"; [U] func_003747A0 stores the
                                    //       cache record index at +0x1A6[lod]; func_0038B190 reads +0x1A6 / +0x1A8 /
                                    //       +0x1AA into tBezierPatchEntry::unk_0C
    char unk_1AC[0x4];              // 0x1AC padding to the 0x1B0 size
};

CHECK_OFFSET(tPatchRecord, m_iIntersect, 0x000);
CHECK_OFFSET(tPatchRecord, m_sSurfaceType, 0x008);
CHECK_OFFSET(tPatchRecord, m_sSurfaceFlags, 0x00A);
CHECK_OFFSET(tPatchRecord, m_iFlags, 0x00C);
CHECK_OFFSET(tPatchRecord, m_fMapScale, 0x010);
CHECK_OFFSET(tPatchRecord, m_fUV0, 0x020);
CHECK_OFFSET(tPatchRecord, m_fBm0, 0x040);
CHECK_OFFSET(tPatchRecord, m_fBm1, 0x080);
CHECK_OFFSET(tPatchRecord, m_fBm2, 0x0C0);
CHECK_OFFSET(tPatchRecord, m_fBm3, 0x100);
CHECK_OFFSET(tPatchRecord, m_fvBSphere, 0x140);
CHECK_OFFSET(tPatchRecord, m_RID, 0x150);
CHECK_OFFSET(tPatchRecord, m_SMap, 0x154);
CHECK_OFFSET(tPatchRecord, m_fvMin, 0x158);
CHECK_OFFSET(tPatchRecord, m_fvMax, 0x164);
CHECK_OFFSET(tPatchRecord, unk_170, 0x170);
CHECK_OFFSET(tPatchRecord, m_sTextures, 0x1A0);
CHECK_OFFSET(tPatchRecord, m_sCacheRecord, 0x1A6);
CHECK_OFFSET(tPatchRecord, unk_1AC, 0x1AC);
CHECK_SIZE(tPatchRecord, 0x1B0);

#endif /* WORLD_PATCHRECORD_H */

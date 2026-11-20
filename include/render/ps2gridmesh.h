#ifndef RENDER_PS2GRIDMESH_H
#define RENDER_PS2GRIDMESH_H

// cPS2GridMesh: the PS2 grid mesh - a width x height grid of vertices drawn as height - 1 triangle strips from
// prebuilt VIF/GIF packets, double-buffered (Update* fills the buffer not on screen, Render swaps). Flags (cFlagSet)
// and flexible bridges (cFlexBridgeNode) create theirs with cPS2GraphicsMan::CreateGridMesh(width, height, memtype)
// and free them with cGraphicsMan::DeleteGridMesh. It implements the abstract cGridMesh (On Tour: size 0x8, a
// cGarbageMan; IsOk, Update x4, Render x2 pure, SetScale(float) empty).
// EA's source file is not known (the preview names no grid-mesh path; On Tour keeps its PSP twin cPSPGridMesh in
// render/all_in_one_render.cpp); retail: splat unit render/particle (ctor 0x00385138 .. Render1 0x00385C10 and the
// packet helpers to 0x00386128) and two inline copies in render/font (IsOk 0x00396108, SetScale 0x00396110, and
// cGridMesh::SetScale 0x00394B68). ps2gridmesh.h is our choice (render/particle.h is the emitters' header).
//
// Size 0x1CC: [R] cPS2GraphicsMan_CreateGridMesh: cMemMan_alloc(0x1CC, "cPS2GridMesh", memtype) +
// cPS2GridMesh_cPS2GridMesh(p, width, height, this, memtype); the last field (+0x1C8, IsOk) ends at 0x1CC.
// Bases: cGridMesh (0x8) <- cGarbageMan (0x8: mNext at 0x0, the vptr at 0x4: cGarbageMan declares the virtual dtor;
// On Tour class cGarbageMan { cGarbageMan* mNext; }), written out inline. Vtables: cPS2GridMesh_vtbl (D_00493208, 9
// slots: 1 dtor, 2 IsOk, 3 Update(t3Vector*), 4 Update(t2Vector*), 5 Update(t3Vector*, tARGB*), 6 Update(vert, rgb, uv),
// 7 Render(BMat, material, TexMat), 8 Render(BMat, texid, TexMat), 9 SetScale); cGridMesh's D_004938E0 (dtor, 7 x
// __pure_virtual, SetScale) and cGarbageMan's D_00493938 (dtor) are restored by the base destructor.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Tags:
//   [R] retail's constructor cPS2GridMesh_cPS2GridMesh, destructors cPS2GridMesh__cPS2GridMesh / cGridMesh__cGridMesh;
//   [P] the preview's out-of-line inlines IsOk__12cPS2GridMesh (lw 0x1C8) and SetScale__12cPS2GridMeshf (0x194, 0x198);
//       tools/layout_xlate.py: preview == retail at 0x4, 0x194, 0x198, 0x19C, 0x1C8;
//   [U] how retail code uses the field (Update / Update1..3, Render1, the packet helpers func_00385EB0 / func_00386128 /
//       func_00385A38);
//   [O] On Tour's DWARF: cGarbageMan::mNext; cPSPGridMesh (the PSP twin of this class: m_pGfxMan, m_pMem, m_iWidth,
//       m_iHeight, m_iNumStrip, m_iStripLen, m_bUpdate, mbOK) - names used where the retail role is the same; its
//       offsets do not apply.

#include "layout.h"

struct cPS2GraphicsMan;

// The abstract base (On Tour class cGridMesh, size 0x8; no data of its own).
struct cGridMesh {
    // --- cGarbageMan
    cGridMesh* mNext;               // 0x000 [O] cGarbageMan::mNext (no retail access seen; no ctor writes it)
    void** mVtbl;                   // 0x004 [R] cGridMesh__cGridMesh stores cGarbageMan's vtable D_00493938 before delete
};

CHECK_OFFSET(cGridMesh, mNext, 0x000);
CHECK_OFFSET(cGridMesh, mVtbl, 0x004);
CHECK_SIZE(cGridMesh, 0x008);

struct cPS2GridMesh {
    // --- cGridMesh / cGarbageMan (0x8)
    cGridMesh* mNext;               // 0x000 [O] cGarbageMan::mNext (no retail access seen)
    void** mVtbl;                   // 0x004 [R] ctor / dtor store cPS2GridMesh_vtbl; [P] 0x4; [U] CreateGridMesh calls slot 2
                                    //       (IsOk), Render calls slot 8 (Render1)
    // --- cPS2GridMesh
    int unk_008;                    // 0x008 never accessed by the class's code
    int unk_00C[32][3];             // 0x00C the packet table: 32 records of 3 words {type (0x1D / 2), count, ...}; [U]
                                    //       func_00385EB0 builds it, func_00386128 reads it
    int unk_18C;                    // 0x18C the packet-record count; [U] func_00385EB0 (zeroes, counts), func_00386128
    unsigned short m_iWidth;        // 0x190 [O] cPSPGridMesh::m_iWidth (an int there); [R] the ctor's width (sh); [U] the
                                    //       Update loops (lhu)
    unsigned short m_iHeight;       // 0x192 [O] cPSPGridMesh::m_iHeight; [R] the ctor's height (sh)
    float unk_194;                  // 0x194 the scale: [R] 20000.0; [P][U] SetScale(scale) stores it; Render1 reads it
    float unk_198;                  // 0x198 the position-to-short factor: [R] 32767 / 20000; [P][U] SetScale stores
                                    //       32767 / scale; func_00385A38 multiplies positions by it
    char* m_pMem;                   // 0x19C [O] cPSPGridMesh::m_pMem; [R] the one "GridMeshMem" allocation (2 packet
                                    //       buffers + 2 strip tables); [P] 0x19C; [U] the dtor frees it
    int unk_1A0;                    // 0x1A0 the index of the buffer on screen: [R] -1; [U] Update* fill the other one
                                    //       (index == 0); Render1 swaps it when unk_1A4 is set
    int m_bUpdate;                  // 0x1A4 [O] cPSPGridMesh::m_bUpdate (bool; a word here); [R] 0; [U] Update* set it,
                                    //       Render1 clears it
    int unk_1A8;                    // 0x1A8 one buffer's packet size: [U] func_00385EB0 stores it, func_00386128 reads it
    char* unk_1AC[2];               // 0x1AC the two packet buffers: [R] m_pMem and m_pMem + size
    void* unk_1B4[2];               // 0x1B4 the two strip tables (16 bytes per strip: {?, positions, uvs, colours});
                                    //       [R] after the buffers; [U] Update* write through [index]
    int m_iNumStrip;                // 0x1BC [O] cPSPGridMesh::m_iNumStrip; [R] height - 1; [U] the Update loops
    int m_iStripLen;                // 0x1C0 [O] cPSPGridMesh::m_iStripLen; [R] 2 * width
    cPS2GraphicsMan* m_pGfxMan;     // 0x1C4 [O] cPSPGridMesh::m_pGfxMan; [R] the ctor's graphics manager; [U] Render1
    int mbOK;                       // 0x1C8 [O] cPSPGridMesh::mbOK (bool; a word here); [R] 1 when the allocation
                                    //       succeeded, else 0; [P] IsOk__12cPS2GridMesh (lw 0x1C8)
};

CHECK_OFFSET(cPS2GridMesh, mNext, 0x000);
CHECK_OFFSET(cPS2GridMesh, mVtbl, 0x004);
CHECK_OFFSET(cPS2GridMesh, unk_008, 0x008);
CHECK_OFFSET(cPS2GridMesh, unk_00C, 0x00C);
CHECK_OFFSET(cPS2GridMesh, unk_18C, 0x18C);
CHECK_OFFSET(cPS2GridMesh, m_iWidth, 0x190);
CHECK_OFFSET(cPS2GridMesh, m_iHeight, 0x192);
CHECK_OFFSET(cPS2GridMesh, unk_194, 0x194);
CHECK_OFFSET(cPS2GridMesh, unk_198, 0x198);
CHECK_OFFSET(cPS2GridMesh, m_pMem, 0x19C);
CHECK_OFFSET(cPS2GridMesh, unk_1A0, 0x1A0);
CHECK_OFFSET(cPS2GridMesh, m_bUpdate, 0x1A4);
CHECK_OFFSET(cPS2GridMesh, unk_1A8, 0x1A8);
CHECK_OFFSET(cPS2GridMesh, unk_1AC, 0x1AC);
CHECK_OFFSET(cPS2GridMesh, unk_1B4, 0x1B4);
CHECK_OFFSET(cPS2GridMesh, m_iNumStrip, 0x1BC);
CHECK_OFFSET(cPS2GridMesh, m_iStripLen, 0x1C0);
CHECK_OFFSET(cPS2GridMesh, m_pGfxMan, 0x1C4);
CHECK_OFFSET(cPS2GridMesh, mbOK, 0x1C8);
CHECK_SIZE(cPS2GridMesh, 0x1CC);

#endif /* RENDER_PS2GRIDMESH_H */

#ifndef RENDER_SIMPLETUBE_H
#define RENDER_SIMPLETUBE_H

// cSimpleTube: a segmented tube (a ribbon strip with sides) drawn through a chain of points - the flex rails' and flex
// bridges' "Tube" (cFlexRailNode::Tube, object/flexrailnode.h +0x58; cFlexBridgeNode's ctors build one with
// NumNodes * 8 - 9 segments). The 262 vertices it draws are static and shared by every tube (D_00501440, filled once
// by the first ctor; updateColours refreshes their colour from the tube colour statics D_004A41E0..E8, On Tour
// cTube::m_R/G/B); render rebuilds them from the points and draws two strips through graphics slots 71 and 73.
// EA's source file is unknown (the preview's assert strings do not name it); On Tour compiles it in
// render/all_in_one_render.cpp and retail keeps it in splat unit render/particle (0x00372520 ctor .. 0x003726C8
// render), plus setPosition (func_003968D0) in render/font, hence render/.
//
// Size 0xC: [R] cFlexBridgeNode's two ctors do cMemMan_alloc(0xC, "Tube") + cSimpleTube_cSimpleTube(p, segments)
// (src/object/flexbridgenode.cpp); the ctor writes 0x0-0x8. On Tour: 0xC, same offsets.
// Bases: cVisualEffect (vptr only; the dtor ends by storing its vtable D_00488680) and, judging by slot 2
// (cTube_update__FPv, shared with cTube_vtbl), cTube (vptr only, statics only; its dtor stores no vtable of its own).
// g++ 2.95 puts the vptr at 0x0 (the first class with virtuals has no data). Vtable cSimpleTube_vtbl (0x00493068,
// 5 slots): 1 dtor cSimpleTube__cSimpleTube, 2 update cTube_update__FPv (calls updateColours), 3 render
// cSimpleTube_render, 4 deferredRender cVisualEffect_deferredRender__FPv, 5 setPosition func_003968D0.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cSimpleTube_cSimpleTube, dtor cSimpleTube__cSimpleTube and the 0xC allocation;
//   [P] none (no paired methods, no inline accessors);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cSimpleTube 0xC: m_numSegments 0x4, m_positions 0x8; static m_vertices[262],
//       m_initialized; setPosition(int id, t4Vector& position)).

#include "layout.h"
#include "bx/math3d.h"

struct cSimpleTube {
    // --- cVisualEffect / cTube (vptr only)
    void** mVtbl;                   // 0x0 [R] cSimpleTube_vtbl (ctor; dtor, which then stores cVisualEffect's
                                    //     D_00488680)
    // --- cSimpleTube
    int m_numSegments;              // 0x4 [O]; [R] the ctor stores its count argument; [U] render: the strips use
                                    //     m_numSegments + 1 points (m_positions[0 .. m_numSegments])
    t4Vector* m_positions;          // 0x8 [O]; [R] ctor 0; [U] the dtor frees it (cMemMan_free) when set, render reads
                                    //     the points (lq), setPosition (func_003968D0) stores m_positions[id]
};

CHECK_OFFSET(cSimpleTube, mVtbl, 0x0);
CHECK_OFFSET(cSimpleTube, m_numSegments, 0x4);
CHECK_OFFSET(cSimpleTube, m_positions, 0x8);
CHECK_SIZE(cSimpleTube, 0xC);

#endif /* RENDER_SIMPLETUBE_H */

#ifndef VISUALFX_AIVISUALEFFECTSMAN_H
#define VISUALFX_AIVISUALEFFECTSMAN_H

// cAIVisualEffectsMan: the manager of the AI-driven visual effects, i.e. the avalanche (cAvalanche,
// visualfx/avalanche.h). An abstract base (update and render are pure virtuals, func_00411530) whose ctor makes the
// object the current manager (D_004A3B80, returned by aivisualEffectsMan()); cAIVisualEffectsMan_construct builds the
// platform class cPS2AIVisualEffectsMan, which allocates the cAvalanche and forwards update / render to it.
// cGame_load wraps the result in a cAIVisualEffectsObjNode (main/gameobjnodes.h). On Tour also declares
// commonUpdate(eRenderStateQueryId); retail's empty func_002F0388 sits where cVisualEffectsMan's commonUpdate sits
// (right after the ctor), so it is probably that, unreferenced.
// EA's source: d:/ssx/code/source/visualfx/aivisualeffectsman.cpp (July 2003 preview assert strings). Retail
// methods: unit visualfx/renderstateman (0x002F0360 aivisualEffectsMan .. 0x002F04E0 cPS2AIVisualEffectsMan_render)
// and visualfx/worldlightman (0x002F8340, the base dtor).
//
// Sizes: cAIVisualEffectsMan 0x8 and cPS2AIVisualEffectsMan 0x8 (it adds no members): [R] cAIVisualEffectsMan_construct
// does cMemMan_alloc(8, "PS2AIVisualEffectsMan") + cPS2AIVisualEffectsMan_cPS2AIVisualEffectsMan; the ctors write 0x0
// and 0x4. On Tour: cAIVisualEffectsMan 0x8 (m_Avalanche 0x0), cPSPAIVisualEffectsMan 0x8 (no members).
// No base class; g++ 2.95 puts the vptr after the class's data: 0x4. Vtables (3 slots: 1 dtor, 2 update, 3 render):
// cAIVisualEffectsMan_vtbl 0x00487D78 (slots 2 / 3 pure), cPS2AIVisualEffectsMan_vtbl 0x00487D50
// (cPS2AIVisualEffectsMan__cPS2AIVisualEffectsMan, _update, _render).
//
// A FLAT layout, like main/game.h; cPS2AIVisualEffectsMan repeats the base's two members. Evidence tags:
//   [R] retail ctors cAIVisualEffectsMan_cAIVisualEffectsMan / cPS2AIVisualEffectsMan_cPS2AIVisualEffectsMan, dtors and
//       the 8-byte allocation;
//   [P] tools/layout_xlate.py pairs three cPS2AIVisualEffectsMan methods: 0x0 and 0x4, preview == retail; no inline
//       accessors;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cAIVisualEffectsMan 0x8: m_Avalanche; cPSPAIVisualEffectsMan).

#include "layout.h"

struct cAvalanche;

struct cAIVisualEffectsMan {
    cAvalanche* m_Avalanche;            // 0x0 [O]; [R] base ctor 0; the PS2 ctor stores a new 4-byte "Avalanche"
                                        //     (cAvalanche_vtbl + cAvalanche_init)
    void** mVtbl;                       // 0x4 [R] cAIVisualEffectsMan_vtbl (ctor and dtor); [U] cAIVisualEffectsObjNode
                                        //     calls slots 1-3 through it
};

CHECK_OFFSET(cAIVisualEffectsMan, m_Avalanche, 0x0);
CHECK_OFFSET(cAIVisualEffectsMan, mVtbl, 0x4);
CHECK_SIZE(cAIVisualEffectsMan, 0x8);

// cPS2AIVisualEffectsMan: the PS2 platform class (On Tour's PSP twin is cPSPAIVisualEffectsMan); adds no members.
struct cPS2AIVisualEffectsMan {
    // --- cAIVisualEffectsMan (0x8)
    cAvalanche* m_Avalanche;            // 0x0 [O] cAIVisualEffectsMan::m_Avalanche; [R] the ctor stores the new
                                        //     "Avalanche"; [U] update / render call its slots 2 / 3, the dtor its slot 1
                                        //     (flags 3) when non-null
    void** mVtbl;                       // 0x4 [R] cPS2AIVisualEffectsMan_vtbl (ctor; dtor, which then stores
                                        //     cAIVisualEffectsMan_vtbl)
};

CHECK_OFFSET(cPS2AIVisualEffectsMan, m_Avalanche, 0x0);
CHECK_OFFSET(cPS2AIVisualEffectsMan, mVtbl, 0x4);
CHECK_SIZE(cPS2AIVisualEffectsMan, 0x8);

#endif /* VISUALFX_AIVISUALEFFECTSMAN_H */

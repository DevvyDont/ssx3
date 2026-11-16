#ifndef WORLDPAINTER_WORLDPAINTERMAN_H
#define WORLDPAINTER_WORLDPAINTERMAN_H

// cWorldPainterMan: the world painter's manager, one per world (cWorld::m_pWorldPainterMan, world/world.h; reached
// in retail as SSXApp->Game->World->m_pWorldPainterMan or cWorld::m_pTheWorld->m_pWorldPainterMan). It owns one
// cWorldPainterSection per world section (m_pCache, sized by cWorld_load through init); each section holds that
// section's streamed world-painter data (tWPHeader, worldpainter/worldpainterlayer.h). Queries
// (cWorldPainterQuery, worldpainter/worldpainterquery.h) read the section in cWorldPainterMan::m_iCurrSection.
//
// Sizes [R]: cWorldPainterMan 0x8 (cWorld_cWorld: cMemMan_alloc(8, "cWorldPainterMan")); cWorldPainterSection 0xC
// (init's new[]: n * 12 + 0x10 bytes, tag "cWorldPainterMan", 12-byte stride everywhere). On Tour: 0x8 / 0xC, same
// members and offsets. No vtables, no bases. EA's source: d:/ssx/code/source/worldpainter/worldpainterman.h and
// worldpainterman.cpp (preview asserts "m_pWPHeader", "m_pCache", "( uiSectionID >= 0 ) && ( uiSectionID <
// m_uiNumSections )", "m_iStreamed == false", "!m_pWPHeader", "m_iState == kWPSection_Unloaded",
// "( m_iState == kWPSection_Resolved ) || ( m_iState == kWPSection_Loaded )", "m_iState == kWPSection_Loaded",
// "getNumWPLayers() == kWPLT_NumLayers", "SSXApp->getGame()->getWorld()->getWorldPainterMan()").
// Methods (both classes, worldpainterman.cpp = retail 0x002BB100-0x002BC500 in bx/seg_1BA100, plus reset and getQuery
// in the worldpainter splits): see methods.txt.
//
// Statics (not declared here: the .cpp files declare these D_ symbols with differing types):
//   cWorldPainterMan::m_iCurrSection = D_004A3860 (unsigned int; [O] static member; [U] read by queryPosition
//   cWorldPainterQuery_queryPosition and render func_002BB6E0; func_002ED490 reads it too).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail's constructors cWorldPainterMan_cWorldPainterMan and func_002BB370 (cWorldPainterSection), init
//       cWorldPainterMan_init, the allocation sizes above;
//   [P] the July 2003 preview's assert strings (worldpainterman.h / worldpainterman.cpp);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cWorldPainterMan, cWorldPainterSection): same members, same offsets.

#include "layout.h"
#include "worldpainter/worldpainterlayer.h"

// cWorldPainterSection::m_iState values ([P] names; 1 is set while the async file read runs, name unknown).
//   0 kWPSection_Unloaded   ctor, unload
//   1 (loading)             load(szFileName) with a valid async handle
//   2 kWPSection_Loaded     update when the read completes, load(pData), load(szFileName) on a failed open
//   3 kWPSection_Resolved   resolve

struct cWorldPainterSection {
    short m_iState;                 // 0x0 [O]; [P] "m_iState == kWPSection_Unloaded" etc.; [R] ctor 0; [U] update,
                                    //       load, unload, resolve; queryPosition requires 3 (kWPSection_Resolved)
    short m_iStreamed;              // 0x2 [O]; [P] "m_iStreamed == false"; [R] ctor 0; [U] load(szFileName) 0,
                                    //       load(pData) 1; unload/dtor free m_pWPHeader only when 0
    int m_iAsyncHandle;             // 0x4 [O]; [R] ctor -1; [U] load(szFileName) stores the async file handle,
                                    //       update polls and releases it
    tWPHeader* m_pWPHeader;         // 0x8 [O]; [P] "m_pWPHeader", "!m_pWPHeader"; [R] ctor 0; [U] update allocates it,
                                    //       load(pData) stores pData, resolve relocates it, queryPosition/render read it
};

CHECK_OFFSET(cWorldPainterSection, m_iState, 0x0);
CHECK_OFFSET(cWorldPainterSection, m_iStreamed, 0x2);
CHECK_OFFSET(cWorldPainterSection, m_iAsyncHandle, 0x4);
CHECK_OFFSET(cWorldPainterSection, m_pWPHeader, 0x8);
CHECK_SIZE(cWorldPainterSection, 0xC);

struct cWorldPainterMan {
    unsigned int m_uiNumSections;   // 0x0 [O]; [P] "( uiSectionID >= 0 ) && ( uiSectionID < m_uiNumSections )";
                                    //       [R] init stores iNumSections; [U] update's loop bound (unsigned compare)
    cWorldPainterSection* m_pCache; // 0x4 [O]; [P] "m_pCache"; [R] ctor 0, init new[] (count cookie at m_pCache[-0x10 bytes]);
                                    //       [U] load/unload index it by section id; queryPosition/render use m_pCache[m_iCurrSection]
};

CHECK_OFFSET(cWorldPainterMan, m_uiNumSections, 0x0);
CHECK_OFFSET(cWorldPainterMan, m_pCache, 0x4);
CHECK_SIZE(cWorldPainterMan, 0x8);

#endif /* WORLDPAINTER_WORLDPAINTERMAN_H */

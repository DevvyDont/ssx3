#ifndef WORLD_WORLDVIEW_H
#define WORLD_WORLDVIEW_H

// cWorldView: the streaming "view" of the open mountain. It owns the section table of the track's .sdb (the
// tStreamDataHeader that cWorldCache_init loads), a load state per section, and two stream foci (one per local
// rider: position + load radius) that pick which sections' hull pages to request. One instance, embedded by value at
// cWorldCache+0x10 (cWorldCache_cWorldCache constructs it in place; reached as World->m_pWorldCache->m_WorldView).
// No vtable, no base class. EA's source: d:/ssx/code/source/world/worldview.cpp and ../source/world/worldview.h
// (preview assert strings). Retail splits it over two units:
//   world/worldcache (0x003A8F80-0x003A9800): cWorldView_cWorldView, cWorldView__cWorldView, cWorldView_init,
//     cWorldView_purge, cWorldView_reset, cWorldView_update, cWorldView_recursiveFlushPages,
//     cWorldView_recursiveIsEmptyPages, cWorldView_setFocus, cWorldView_getFocus,
//     cWorldView_getTestFocus;
//   world/worldview (0x003A9800-0x003AA2D8): cWorldView_getNumSections, cWorldView_getSectionName,
//     cWorldView_isSectionLoaded, cWorldView_isSectionUnLoaded, cWorldView_loadSection,
//     cWorldView_unloadSection, cWorldView_activateSection, cWorldView_deactivateSection,
//     cWorldView_getWorldBoundBox, cWorldView_getHull, cWorldView_getNumHull,
//     cWorldView_isBoxValid, cWorldView_recursiveIsValid, cWorldView_refreshStreamFocus,
//     cWorldView_getDistanceToBox, cWorldView_recursiveTreeFocus.
// (Names from On Tour's cWorldView methods, matched by body; the rest of world/worldview is cChunkResolve,
// cPS2ChunkResolve and cWScriptCache.)
//
// Size 0x2F0: cWorldCache's next member, m_StreamMan, is at 0x300 = 0x10 + 0x2F0 (cWorldCache_cWorldCache), and the
// last field is 0x2E4 (rounded up to the 16-byte alignment that the t4Vector members give the class). On Tour: 0x2F0
// with the same member order; retail stores tStreamFocusInfo's bools as words (0x240/0x244), see below.
//
// A FLAT layout, like main/game.h. Member names are EA's, from SSX On Tour's PSP DWARF, placed by:
//   [P] the July 2003 preview's assert strings in worldview.cpp/worldview.h ("mp_Hdr && (id <
//       mp_Hdr->m_uiNumSections)", "mp_WorldCache->isPageEmpty(mp_Section[i].m_BSP.m_iGlobalHull)",
//       "mp_Hdr && (uiIdx<mp_Hdr->m_uiNumBSPHulls)");
//   [R] retail's init (cWorldView_init) and reset (cWorldView_reset);
//   [U] how retail code uses the field;
//   [O] On Tour's member order, where the fields on both sides are placed by [P], [R] or [U].
// Section states (On Tour eSectionState): 0 Empty, 1 LoadQueue, 2 Loading, 3 UnLoadQueue, 4 UnLoading, 5 Loaded,
// 6 Active. Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"
#include "bx/math3d.h"
#include "world/instancerecord.h"   // tf4Vector (tBxBSPNode)

struct cWorldCache;
struct tBxBSPNode;

// The .sdb section database header (On Tour: tStreamDataHeader_PSP; the PS2 tStreamDataHeader has the same 0x50 size).
struct tStreamDataHeader {
    // m_cCookie[0..1] read as one halfword in a .sdb this build loads (name ours): [U] isValidMap rejects any other.
    enum { kCookie = 0x3800 };
    char m_cCookie[4];              // 0x00 [P] assert mp_DataHeader->m_cCookie[kCookieVer]; [U] isValidMap reads
                                    //      [0..1] as one halfword (== kCookie)
    unsigned int m_uiCheckSum;      // 0x04 [O]
    unsigned int m_uiNumSections;   // 0x08 [P] assert mp_Hdr->m_uiNumSections; [U] cWorldView_getNumSections
    unsigned int m_uiNumBSPNodes;   // 0x0C [O]; [R] cWorldView_init: count of 0x60-byte nodes
    unsigned int m_uiNumBSPHulls;   // 0x10 [P] assert mp_Hdr->m_uiNumBSPHulls; [U] cWorldView_getNumHull
    struct tBxSection* m_pSection;  // 0x14 [O]; [R] cWorldCache_init fixes it up to this + 0x50
    short m_iTable[28];             // 0x18 [O]; [R] cWorldCache_init: per-type counts of the shared cWorldCacheTable
};

CHECK_OFFSET(tStreamDataHeader, m_cCookie, 0x00);
CHECK_OFFSET(tStreamDataHeader, m_uiCheckSum, 0x04);
CHECK_OFFSET(tStreamDataHeader, m_uiNumSections, 0x08);
CHECK_OFFSET(tStreamDataHeader, m_uiNumBSPNodes, 0x0C);
CHECK_OFFSET(tStreamDataHeader, m_uiNumBSPHulls, 0x10);
CHECK_OFFSET(tStreamDataHeader, m_pSection, 0x14);
CHECK_OFFSET(tStreamDataHeader, m_iTable, 0x18);
CHECK_SIZE(tStreamDataHeader, 0x50);

// A section's BSP header (On Tour tBxBSPHeader).
struct tBxBSPHeader {
    int m_iNumNodes;                // 0x0 [O]
    int m_iNumHulls;                // 0x4 [O]
    int m_iGlobalHull;              // 0x8 [P] assert mp_Section[i].m_BSP.m_iGlobalHull; [U] the section's own hull page
                                    //     (cWorldView_update touchPage/isPageLoaded; cWorldView_isBoxValid)
    tBxBSPNode* m_pRoot;            // 0xC [O] an anonymous union {tBxBSPNode* m_pRoot; unsigned int m_uiRoot} (names
                                    //     from the prx DIEs); [R] cWorldView_init fixes the root node index up to a
                                    //     pointer; [U] the recursive tree walks start here
};

CHECK_OFFSET(tBxBSPHeader, m_iNumNodes, 0x0);
CHECK_OFFSET(tBxBSPHeader, m_iNumHulls, 0x4);
CHECK_OFFSET(tBxBSPHeader, m_iGlobalHull, 0x8);
CHECK_OFFSET(tBxBSPHeader, m_pRoot, 0xC);
CHECK_SIZE(tBxBSPHeader, 0x10);

// One world section's record in the .sdb (On Tour tBxSection). 0x58 bytes: cWorldView_init steps 0x58.
struct tBxSection {
    char m_sSectionName[16];        // 0x00 [O]; [U] cWorldView_update compares the first 4 chars with "ASKY".."ESKY";
                                    //      cSectionMan's init matches BE section names against it (cWorldView_getSectionName)
    tBxBSPHeader m_BSP;             // 0x10 [P] assert mp_Section[i].m_BSP
    short m_iTable[28];             // 0x20 [O]; [R] cWorldCache_activateSectionMem: per-type counts of the section's
                                    //      cWorldCacheTable; [U] cWorld_resetMap reads [3] (instances)
};

CHECK_OFFSET(tBxSection, m_sSectionName, 0x00);
CHECK_OFFSET(tBxSection, m_BSP, 0x10);
CHECK_OFFSET(tBxSection, m_iTable, 0x20);
CHECK_SIZE(tBxSection, 0x58);

// One streamable hull page (On Tour tBxBSPHull). 0x44 bytes: getHull steps 0x44.
struct tBxBSPHull {
    unsigned short m_uiNumChunk;    // 0x00 [O]; [U] cWorldCache_updatePages passes it to addStreamRequest; cHullPage's
                                    //      ctor sizes its entry table by it; cWorldView_isBoxValid tests it
    unsigned short m_uiHullIndex;   // 0x02 [O]; [U] cWorldView_recursiveFlushPages/cWorldView_recursiveIsEmptyPages/cWorldView_recursiveIsValid read it (lhu 2) as the
                                    //      hull page id
    int m_iStreamOffset;            // 0x04 [O]; [U] cWorldCache_updatePages passes it to addStreamRequest
    unsigned int m_uiTotalDataSize; // 0x08 [O]
    unsigned short m_usNumData[28]; // 0x0C [O]
};

CHECK_OFFSET(tBxBSPHull, m_uiNumChunk, 0x00);
CHECK_OFFSET(tBxBSPHull, m_uiHullIndex, 0x02);
CHECK_OFFSET(tBxBSPHull, m_iStreamOffset, 0x04);
CHECK_OFFSET(tBxBSPHull, m_uiTotalDataSize, 0x08);
CHECK_OFFSET(tBxBSPHull, m_usNumData, 0x0C);
CHECK_SIZE(tBxBSPHull, 0x44);

// One BSP tree node (On Tour tBxBSPNode, 0x60; same layout). The three words at 0x50 are node / hull indices in the
// file that cWorldView_init fixes up to pointers (cWorldView::mp_Nodes / mp_Hulls). Union member names from the prx
// DIEs. [U] the tree walks (cWorldView_recursiveIsValid / recursiveTreeFocus in world/worldview,
// cWorldView_recursiveFlushPages / recursiveIsEmptyPages in world/worldcache; views sSection_9BB8, sSection_A028,
// cWorldView_recursiveFlushPages_sNode) skip a node whose box misses theirs, touch / test its hull page, and visit
// the in child when the in plane's dot with the box minimum is negative, the out child likewise with the maximum.
struct tBxBSPNode {
    tf4Vector m_v4BoxMin;           // 0x00 [O]; [U] the overlap test's minimum
    tf4Vector m_v4BoxMax;           // 0x10 [O]; [U] the overlap test's maximum; recursiveTreeFocus's distance box
    tf4Vector m_v4SplitPlaneIn;     // 0x20 [O]; [U] dotted with the box minimum to visit the in child
    tf4Vector m_v4SplitPlaneOut;    // 0x30 [O]; [U] dotted with the box maximum to visit the out child
    tf4Vector unk_040;              // 0x40 [O] m_v4SplitPlane (no retail read seen)
    union {
        tBxBSPNode* m_pInChild;     // 0x50 [O]; [U] the first child the walks visit
        int m_iInChild;             // 0x50 [O]; [R] the index before cWorldView_init's fix-up
    };
    union {
        tBxBSPNode* m_pOutChild;    // 0x54 [O]; [U] the second child
        int m_iOutChild;            // 0x54 [O]; [R] ditto
    };
    union {
        tBxBSPHull* m_pHull;        // 0x58 [O]; [U] the node's hull page (m_uiHullIndex is the page id), 0 = none
        int m_iHull;                // 0x58 [O]; [R] ditto
    };
    unsigned int unk_05C;           // 0x5C [O] pad
};

CHECK_OFFSET(tBxBSPNode, m_v4BoxMin, 0x00);
CHECK_OFFSET(tBxBSPNode, m_v4BoxMax, 0x10);
CHECK_OFFSET(tBxBSPNode, m_v4SplitPlaneIn, 0x20);
CHECK_OFFSET(tBxBSPNode, m_v4SplitPlaneOut, 0x30);
CHECK_OFFSET(tBxBSPNode, unk_040, 0x40);
CHECK_OFFSET(tBxBSPNode, m_pInChild, 0x50);
CHECK_OFFSET(tBxBSPNode, m_pOutChild, 0x54);
CHECK_OFFSET(tBxBSPNode, m_pHull, 0x58);
CHECK_OFFSET(tBxBSPNode, unk_05C, 0x5C);
CHECK_SIZE(tBxBSPNode, 0x60);

// tSectionState::state. [O] On Tour's cWorldView::eSectionState, names and values; [U] loadSection queues an empty
// section (kSection_LoadQueue) and takes kSection_UnLoadQueue back to kSection_Loaded, unloadSection queues loaded and
// active sections, activateSection makes a loaded section kSection_Active.
enum eSectionState {
    kSection_Empty = 0, kSection_LoadQueue = 1, kSection_Loading = 2, kSection_UnLoadQueue = 3, kSection_UnLoading = 4,
    kSection_Loaded = 5, kSection_Active = 6
};

// Per-section load state (On Tour tSectionState).
struct tSectionState {
    int state;                      // 0x0 [O] eSectionState; [U] the cWorldView state machine (values above)
    float priority;                 // 0x4 [O]; [R] loadSection stores it (swc1); [U] update passes it to touchPage
};

CHECK_OFFSET(tSectionState, state, 0x0);
CHECK_OFFSET(tSectionState, priority, 0x4);
CHECK_SIZE(tSectionState, 0x8);

// An axis-aligned box (On Tour tBoundBox). Move to a bx header when a second user appears.
struct tBoundBox {
    t4Vector MinPoint;              // 0x00 [O]; [R] setFocus: pos - radius
    t4Vector MaxPoint;              // 0x10 [O]; [R] setFocus: pos + radius
};

CHECK_OFFSET(tBoundBox, MinPoint, 0x00);
CHECK_OFFSET(tBoundBox, MaxPoint, 0x10);
CHECK_SIZE(tBoundBox, 0x20);

// One stream focus (On Tour tStreamFocusInfo). On Tour packs m_bActive/m_bFocusUpdate into bytes 0/1 and puts
// m_iFocusTick at 4; retail keeps each bool a word: reset (cWorldView_reset) clears 0x0, setFocus stores 1 to 0x4 and
// 0 to 0x8, refreshStreamFocus stores the frame to 0x8 and 0 to 0x4.
struct tStreamFocusInfo {
    int m_bActive;                  // 0x00 [O] (a bool); [R] reset = 0; [U] getFocus/getTestFocus/refreshStreamFocus
                                    //      test it; setActive is inlined (sw cache+0x250 + i*0x50 in game-state code);
                                    //      cSectionMan_update reads both foci (cache+0x250, +0x2A0)
    int m_bFocusUpdate;             // 0x04 [O] (a bool); [R] setFocus = 1; [U] refreshStreamFocus = 0
    int m_iFocusTick;               // 0x08 [O]; [R] setFocus = 0; [U] refreshStreamFocus = the app frame counter
    int unk_0C;                     // 0x0C padding (t4Vector alignment)
    t4Vector m_v4LoadPos;           // 0x10 [O]; [R] setFocus = pos; [U] getFocus returns it; recursiveTreeFocus
    tBoundBox m_FocusBox;           // 0x20 [O]; [R] setFocus: pos -/+ radius; [U] recursiveTreeFocus overlap tests
    float m_fLoadRadius;            // 0x40 [O]; [R] setFocus = max(radius, 0); [U] recursiveTreeFocus, refreshStreamFocus
    float m_fSafeRadius;            // 0x44 [O]; [R] setFocus = 0; [U] recursiveTreeFocus/refreshStreamFocus;
                                    //      getFocus returns it - 0.5
    float m_fSafeTestRadius;        // 0x48 [O]; [R] setFocus = 0; [U] getTestFocus returns it - 0.5
    int unk_4C;                     // 0x4C padding (16-byte alignment)
};

CHECK_OFFSET(tStreamFocusInfo, m_bActive, 0x00);
CHECK_OFFSET(tStreamFocusInfo, m_bFocusUpdate, 0x04);
CHECK_OFFSET(tStreamFocusInfo, m_iFocusTick, 0x08);
CHECK_OFFSET(tStreamFocusInfo, m_v4LoadPos, 0x10);
CHECK_OFFSET(tStreamFocusInfo, m_FocusBox, 0x20);
CHECK_OFFSET(tStreamFocusInfo, m_fLoadRadius, 0x40);
CHECK_OFFSET(tStreamFocusInfo, m_fSafeRadius, 0x44);
CHECK_OFFSET(tStreamFocusInfo, m_fSafeTestRadius, 0x48);
CHECK_SIZE(tStreamFocusInfo, 0x50);

struct t4Vector;

struct cWorldView {
    cWorldCache* mp_WorldCache;     // 0x000 [P] assert mp_WorldCache->isPageEmpty; [R] init's third argument (the
                                    //       cache; the init view calls it `id`), reset = 0; [U] every page call
    tStreamDataHeader* mp_Hdr;      // 0x004 [P] assert mp_Hdr->m_uiNumSections; [R] init; [U] all the section getters
    tBxSection* mp_Section;         // 0x008 [P] assert mp_Section[i]; [R] init = hdr + 0x50
    tBxBSPNode* mp_Nodes;           // 0x00C [O]; [R] init: 16-aligned after the sections, indices fixed up to pointers
    tBxBSPHull* mp_Hulls;           // 0x010 [O]; [R] init: after the nodes; [U] cWorldView_getHull steps 0x44
    tSectionState mp_SectionState[64]; // 0x014 [O]; [R] reset zeroes 64; [U] isSectionLoaded = state 5 or 6,
                                    //       load/unload/activate/deactivate, update's state machine
    int unk_214[3];                 // 0x214 padding (tBoundBox alignment)
    tBoundBox m_WorldBoundBox;      // 0x220 [O]; [R] init copies section 0's root node box, reset = zero vectors;
                                    //       [U] cWorldView_getWorldBoundBox returns its address
    tStreamFocusInfo m_StreamFocus[2]; // 0x240 [O] (kStreamFocus_Num = 2, one per local rider); [R] reset clears both
                                    //       and calls setFocus(i, zero, 0)
    unsigned int m_uiNumSectionsLoaded; // 0x2E0 [O]; [R] reset = 0; [U] update counts sections in state 5 or 6
    unsigned int m_uiNumSectionsActive; // 0x2E4 [O]; [R] reset = 0; [U] update counts sections in state 6

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cWorldView_update");
    int getFocus(int i, t4Vector* outPos, float* outT) __asm__("cWorldView_getFocus");
    int getTestFocus(int i, t4Vector* outPos, float* outT) __asm__("cWorldView_getTestFocus");
    int getNumSections() __asm__("cWorldView_getNumSections__FP10cWorldView");
    void* getSectionName(unsigned int i) __asm__("cWorldView_getSectionName");
    bool isSectionLoaded(int section) __asm__("cWorldView_isSectionLoaded__FP10cWorldViewi");
    bool isSectionUnLoaded(unsigned int i) __asm__("cWorldView_isSectionUnLoaded");
    bool loadSection(unsigned int idx, float value) __asm__("cWorldView_loadSection");
    bool unloadSection(unsigned int idx) __asm__("cWorldView_unloadSection");
    bool activateSection(unsigned int i) __asm__("cWorldView_activateSection");
    bool deactivateSection(unsigned int idx) __asm__("cWorldView_deactivateSection");
    void* getHull(int uiIdx) __asm__("cWorldView_getHull");
    int getNumHull() __asm__("cWorldView_getNumHull__FPv");
    int isBoxValid(int box) __asm__("cWorldView_isBoxValid");
    void recursiveIsValid(tBxBSPNode* sec, t4Vector* box, int* ok) __asm__("cWorldView_recursiveIsValid");
    void refreshStreamFocus(int cam) __asm__("cWorldView_refreshStreamFocus");
    void recursiveTreeFocus(tBxBSPNode* sec_, int cam, int loaded) __asm__("cWorldView_recursiveTreeFocus");
    void purge() __asm__("cWorldView_purge__FPv");
};

CHECK_OFFSET(cWorldView, mp_WorldCache, 0x000);
CHECK_OFFSET(cWorldView, mp_Hdr, 0x004);
CHECK_OFFSET(cWorldView, mp_Section, 0x008);
CHECK_OFFSET(cWorldView, mp_Nodes, 0x00C);
CHECK_OFFSET(cWorldView, mp_Hulls, 0x010);
CHECK_OFFSET(cWorldView, mp_SectionState, 0x014);
CHECK_OFFSET(cWorldView, unk_214, 0x214);
CHECK_OFFSET(cWorldView, m_WorldBoundBox, 0x220);
CHECK_OFFSET(cWorldView, m_StreamFocus, 0x240);
CHECK_OFFSET(cWorldView, m_uiNumSectionsLoaded, 0x2E0);
CHECK_OFFSET(cWorldView, m_uiNumSectionsActive, 0x2E4);
CHECK_SIZE(cWorldView, 0x2F0);

#endif /* WORLD_WORLDVIEW_H */

#ifndef RENDER_OCCLUSIONMAN_H
#define RENDER_OCCLUSIONMAN_H

// cOcclusionMan: the vis-curtain occlusion manager (cGame::OcclusionMan, allocated as "OcclusionMan"). It keeps the
// world's occluder planes (tVisCurtainRecord) and, per view, the up to 32 curtains in view sorted by
// cOcclusionMan__VisSortFunc. Methods in movie/movieplayer: addVisCurtain, removeVisCurtain, updateView.
// Size 0x114 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

// An occluder plane from the world data (On Tour: 0xD0, members not in its DWARF). Retail keeps the planes in
// cOcclusionMan's doubly linked list: [U] addVisCurtain / removeVisCurtain link them through +0xBC and +0xC0. The
// link names are ours; the rest of the record is not laid out yet.
struct tVisCurtainRecord {
    char unk_000[0xBC];
    tVisCurtainRecord* next;        // 0x0BC [U] (our name)
    tVisCurtainRecord* prev;        // 0x0C0 [U] (our name)
};

// One curtain in view and its distance (On Tour tVisInView, 8 bytes); [U] updateView stores both per entry.
struct tVisInView {
    tVisCurtainRecord* pVis;        // 0x0 [O]
    float fDist;                    // 0x4 [O]
};

struct cOcclusionMan {
    tVisCurtainRecord* m_pPlanes;   // 0x000 [O]
    int m_iNumPlanes;               // 0x004 [O]
    tVisInView m_InView[32];        // 0x008 [O]
    int m_iNumInView;               // 0x108 [O]
    int m_iNumActive;               // 0x10C [O]
    unsigned int m_uiVisMask;       // 0x110 [O]
};

CHECK_OFFSET(cOcclusionMan, m_pPlanes, 0x000);
CHECK_OFFSET(cOcclusionMan, m_iNumPlanes, 0x004);
CHECK_OFFSET(cOcclusionMan, m_InView, 0x008);
CHECK_OFFSET(cOcclusionMan, m_iNumInView, 0x108);
CHECK_OFFSET(cOcclusionMan, m_iNumActive, 0x10C);
CHECK_OFFSET(cOcclusionMan, m_uiVisMask, 0x110);
CHECK_OFFSET(tVisCurtainRecord, next, 0x0BC);
CHECK_SIZE(tVisInView, 0x8);
CHECK_SIZE(cOcclusionMan, 0x114);

#endif

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
#include "bx/math3d.h"                // t4Vector
#include "world/instancerecord.h"     // tf4Vector

// An occluder plane from the world data (On Tour: tVisCurtainRecord : tVisCurtainRecord_PSP : _tPSPVisCurtainRecord,
// 0xD0). Retail keeps the planes in cOcclusionMan's doubly linked list: [U] addVisCurtain / removeVisCurtain link them
// through +0xBC and +0xC0. [O] _tPSPVisCurtainRecord (0xC4): m_fvBSphere, m_fvCorners[4], m_fvPlaneCoeffs,
// m_fvVisFrustum[4], m_fvMin, m_fvMax, m_iFlags, m_pNext, m_pPrev, then tVisCurtainRecord_PSP's m_PadBytes[12] (0xD0).
// [U] cOcclusionMan_updateVisPlane (movie/movieplayer, view sFrustum_29B0) reads the four corners (0x10..0x40, lqc2)
// and writes the four view-frustum planes (0x60..0x90) from the eye. Members above 0x10 and below 0x60 and the
// bounds / flags are On Tour's (not yet seen used in retail). The link names next / prev are ours (On Tour m_pNext /
// m_pPrev); kept because landed code uses them.
struct tVisCurtainRecord {
    t4Vector m_fvBSphere;           // 0x000 [O] the bounding sphere (center, radius)
    t4Vector m_fvCorners[4];        // 0x010 [O]; [U] updateVisPlane reads corner i and i + 1
    t4Vector m_fvPlaneCoeffs;       // 0x050 [O] the curtain's own plane
    tf4Vector m_fvVisFrustum[4];    // 0x060 [O]; [U] updateVisPlane stores the plane through the eye and two corners
                                    //       (x, y, z, d) as four plain floats: its stores are scheduled as not
                                    //       aliasing the t4Vector temporaries, which a t4Vector member would
    float m_fvMin[3];               // 0x0A0 [O] tf3Vector
    float m_fvMax[3];               // 0x0AC [O] tf3Vector
    int m_iFlags;                   // 0x0B8 [O]
    tVisCurtainRecord* next;        // 0x0BC [U] (our name; On Tour m_pNext)
    tVisCurtainRecord* prev;        // 0x0C0 [U] (our name; On Tour m_pPrev)
    char m_PadBytes[12];            // 0x0C4 [O] tVisCurtainRecord_PSP::m_PadBytes (the t4Vector alignment rounds to 0xD0)
};

// One curtain in view and its distance (On Tour tVisInView, 8 bytes); [U] updateView stores both per entry.
struct tVisInView {
    tVisCurtainRecord* pVis;        // 0x0 [O]
    float fDist;                    // 0x4 [O]
};

struct t4Vector;
struct tBoundBox;

struct cOcclusionMan {
    tVisCurtainRecord* m_pPlanes;   // 0x000 [O]
    int m_iNumPlanes;               // 0x004 [O]
    tVisInView m_InView[32];        // 0x008 [O]
    int m_iNumInView;               // 0x108 [O]
    int m_iNumActive;               // 0x10C [O]
    unsigned int m_uiVisMask;       // 0x110 [O]

    // Methods bound to their retail symbols by asm labels.
    void addVisCurtain(tVisCurtainRecord* curtain) __asm__("cOcclusionMan_addVisCurtain");
    void removeVisCurtain(tVisCurtainRecord* curtain) __asm__("cOcclusionMan_removeVisCurtain");
    void updateView(tBoundBox* bb) __asm__("cOcclusionMan_updateView");
    void updateVisPlane(t4Vector* eyep, tVisCurtainRecord* frustumPtr) __asm__("cOcclusionMan_updateVisPlane");
};

CHECK_OFFSET(cOcclusionMan, m_pPlanes, 0x000);
CHECK_OFFSET(cOcclusionMan, m_iNumPlanes, 0x004);
CHECK_OFFSET(cOcclusionMan, m_InView, 0x008);
CHECK_OFFSET(cOcclusionMan, m_iNumInView, 0x108);
CHECK_OFFSET(cOcclusionMan, m_iNumActive, 0x10C);
CHECK_OFFSET(cOcclusionMan, m_uiVisMask, 0x110);
CHECK_OFFSET(tVisCurtainRecord, m_fvBSphere, 0x000);
CHECK_OFFSET(tVisCurtainRecord, m_fvCorners, 0x010);
CHECK_OFFSET(tVisCurtainRecord, m_fvPlaneCoeffs, 0x050);
CHECK_OFFSET(tVisCurtainRecord, m_fvVisFrustum, 0x060);
CHECK_OFFSET(tVisCurtainRecord, m_fvMin, 0x0A0);
CHECK_OFFSET(tVisCurtainRecord, m_fvMax, 0x0AC);
CHECK_OFFSET(tVisCurtainRecord, m_iFlags, 0x0B8);
CHECK_OFFSET(tVisCurtainRecord, next, 0x0BC);
CHECK_OFFSET(tVisCurtainRecord, prev, 0x0C0);
CHECK_OFFSET(tVisCurtainRecord, m_PadBytes, 0x0C4);
CHECK_SIZE(tVisCurtainRecord, 0xD0);
CHECK_SIZE(tVisInView, 0x8);
CHECK_SIZE(cOcclusionMan, 0x114);

#endif

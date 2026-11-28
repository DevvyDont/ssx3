#ifndef MAIN_CROWDMAN2D_H
#define MAIN_CROWDMAN2D_H

// cCrowdMan2d: the 2D crowd manager. Crowds at the race courses are flat quads in the world models; scripts
// (Crowd2dFunc) attach up to 64 of them by world resource id, and the manager animates them with one shared 2D crowd
// animation (a cCrowdRender2D sprite sheet + cCrowdAnim2D, visualfx/crowdanim2d.h) whose current frame goes into the
// shared record D_00536690 that the crowd models' bones point at (cCrowdMan2d_attachBoneToCrowdPose). It also spawns short-lived markers
// (camera flashes) over the quads at a rate that follows the crowd's cheer level from the audio (cCrowdMan2d_spawnMarkers),
// draws them as sprites (cCrowdMan2d_renderMarkers), drops the quads of an unloading world section (cCrowdMan2d_releaseSectionQuads) and saves /
// restores the quad ids in replays (cCrowdMan2d_writeToReplayFrame / cCrowdMan2d_readFromReplayFrame).
// EA's source: d:/ssx/code/source/main/crowdman2d.cpp (the July 2003 preview's assert strings, with "!TheCrowdMan2d":
// a static singleton, retail TheCrowdMan2d, set by the ctor and cleared by the dtor). The class name follows the
// allocation tag "CrowdMan2d", the preview's cGame accessor getCrowdMan2d__C5cGame and the file name; main/game.h
// already declares cGame::CrowdMan2d as cCrowdMan2d* (no mangled symbol spells the class itself). Retail code:
// main/ssxapp 0x00229180..0x00229E58. No On Tour class (On Tour keeps only cCrowdAnim2D / cCrowdRender2D).
//
// Size 0x2720: [R] cGame_load (main/gameload): cMemMan_alloc(10016, "CrowdMan2d") + the constructor cCrowdMan2d_cCrowdMan2d,
// which builds the arrays up to 0x2220 + 40 * 0x20 = 0x2720. No vtable, no base class (the destructor cCrowdMan2d__cCrowdMan2d is
// called directly by cGame_purge with flags 3).
// Quad slots and markers hold 16-byte vectors (lqc2 / sqc2), so they are 16-byte aligned; the 8-byte gaps at 0x208
// and 0x218 are that alignment (no access).
//
// A FLAT layout, like main/game.h. Evidence tags: [R] the constructor cCrowdMan2d_cCrowdMan2d / destructor cCrowdMan2d__cCrowdMan2d / reset
// cCrowdMan2d_resetSlots; [U] how retail code uses the field (src/main/ssxapp.cpp views sCrowd_229180, sCrowd_2292E0,
// sCrowd_229530 and the asm). Names are ours.

#include "layout.h"
#include "bx/math3d.h"                  // t4Vector

struct cCrowdRender2D;
struct cCrowdAnim2D;

// One crowd quad slot (0x40 bytes).
struct tCrowdQuad2d {
    t4Vector mCentre;               // 0x00 [U] cCrowdMan2d_placeQuad: corner 0 + (corner 3 - corner 0) / 2, raised 150 in z
    t4Vector mHalfX;                // 0x10 [U] cCrowdMan2d_placeQuad: 0.45 * (corner 2 - corner 0); cCrowdMan2d_spawnMarkers scales it
    t4Vector mHalfY;                // 0x20 [U] cCrowdMan2d_placeQuad: 0.45 * (corner 1 - corner 0)
    int mDelay;                     // 0x30 [R] reset: 300 + BXrand() % 300; [U] cCrowdMan2d_spawnMarkers counts it down by
                                    //      10 * level + 4 and spawns a marker at 0
    char unk_34[0xC];               // 0x34 no access (alignment)
};

CHECK_OFFSET(tCrowdQuad2d, mCentre, 0x00);
CHECK_OFFSET(tCrowdQuad2d, mHalfX, 0x10);
CHECK_OFFSET(tCrowdQuad2d, mHalfY, 0x20);
CHECK_OFFSET(tCrowdQuad2d, mDelay, 0x30);
CHECK_OFFSET(tCrowdQuad2d, unk_34, 0x34);
CHECK_SIZE(tCrowdQuad2d, 0x40);

// One marker of the ring (0x20 bytes).
struct tCrowdMarker2d {
    t4Vector mPos;                  // 0x00 [U] cCrowdMan2d_addMarker stores the spawn position; cCrowdMan2d_renderMarkers draws there
    int mLife;                      // 0x10 [U] cCrowdMan2d_addMarker stores it (3); cCrowdMan2d_update counts it down; cCrowdMan2d_renderMarkers
                                    //      draws the live ones
    char unk_14[0xC];               // 0x14 no access (alignment)
};

CHECK_OFFSET(tCrowdMarker2d, mPos, 0x00);
CHECK_OFFSET(tCrowdMarker2d, mLife, 0x10);
CHECK_OFFSET(tCrowdMarker2d, unk_14, 0x14);
CHECK_SIZE(tCrowdMarker2d, 0x20);

struct cBxReplayFrame;

struct cCrowdMan2d {
    cCrowdRender2D* mpRender;       // 0x0000 [R] ctor: cCrowdRender2D_cCrowdRender2D(cMemMan_alloc(0x10,
                                    //        "cCrowdRender2D")); dtor destroys it (flags 3)
    cCrowdAnim2D* mpAnim;           // 0x0004 [R] ctor: cCrowdRender2D_constructCrowdAnim2D(mpRender); dtor frees it;
                                    //        [U] cCrowdMan2d_update updates it, cCrowdMan2d_spawnMarkers sets its target state (+0x0)
    unsigned int mQuadId[128];      // 0x0008 [R] ctor / reset 0xFFFFFFFF (free); [U] the world resource id of each used
                                    //        slot (low byte: the world section); cCrowdMan2d_addQuad / cCrowdMan2d_removeQuad use the
                                    //        first 64; cCrowdMan2d_writeToReplayFrame / cCrowdMan2d_readFromReplayFrame save / load all 0x200 bytes
    // mQuadId of a free slot (name ours): [R] the ctor and reset fill it; [U] cCrowdMan2d_releaseAllQuads, cCrowdMan2d_releaseSectionQuads,
    // cCrowdMan2d_spawnMarkers and cCrowdMan2d_readFromReplayFrame skip such slots, cCrowdMan2d_addQuad takes the first one.
    enum { kFreeQuad = 0xFFFFFFFF };
    char unk_0208[0x8];             // 0x0208 no access (alignment of mQuad)
    tCrowdQuad2d mQuad[128];        // 0x0210 [R] ctor builds 128, reset MEM_fill's 0x2000 bytes; [U] cCrowdMan2d_placeQuad
                                    //        places one, cCrowdMan2d_spawnMarkers walks the used ones
    int mCheerLevel;                // 0x2210 [R] reset 0; [U] cCrowdMan2d_spawnMarkers: |cCrowdSys_GetCurrentCheer(gSSXAudio, 0)|
    int mMarkerIndex;               // 0x2214 [R] reset 0; [U] cCrowdMan2d_addMarker: the next ring entry, modulo 40
    char unk_2218[0x8];             // 0x2218 no access (alignment of mMarker)
    tCrowdMarker2d mMarker[40];     // 0x2220 [R] ctor builds 40, reset MEM_fill's 1280 bytes; [U] cCrowdMan2d_addMarker adds,
                                    //        cCrowdMan2d_update ages, cCrowdMan2d_renderMarkers draws

    // Methods bound to their retail symbols by asm labels.
    void resetSlots() __asm__("cCrowdMan2d_resetSlots");
    void releaseAllQuads() __asm__("cCrowdMan2d_releaseAllQuads");
    void releaseSectionQuads(int id) __asm__("cCrowdMan2d_releaseSectionQuads");
    void restart() __asm__("cCrowdMan2d_restart");
    void update() __asm__("cCrowdMan2d_update");
    void spawnMarkers() __asm__("cCrowdMan2d_spawnMarkers");
    void addQuad(int resourceId) __asm__("cCrowdMan2d_addQuad");
    void removeQuad(int val) __asm__("cCrowdMan2d_removeQuad");
    void attachBoneToCrowdPose(unsigned int handle, int partIndex, int boneIndex)
        __asm__("cCrowdMan2d_attachBoneToCrowdPose");
    void freeQuad(int slotIndex) __asm__("cCrowdMan2d_freeQuad");
    void renderMarkers() __asm__("cCrowdMan2d_renderMarkers");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cCrowdMan2d_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cCrowdMan2d_readFromReplayFrame");
};

CHECK_OFFSET(cCrowdMan2d, mpRender, 0x0000);
CHECK_OFFSET(cCrowdMan2d, mpAnim, 0x0004);
CHECK_OFFSET(cCrowdMan2d, mQuadId, 0x0008);
CHECK_OFFSET(cCrowdMan2d, unk_0208, 0x0208);
CHECK_OFFSET(cCrowdMan2d, mQuad, 0x0210);
CHECK_OFFSET(cCrowdMan2d, mCheerLevel, 0x2210);
CHECK_OFFSET(cCrowdMan2d, mMarkerIndex, 0x2214);
CHECK_OFFSET(cCrowdMan2d, unk_2218, 0x2218);
CHECK_OFFSET(cCrowdMan2d, mMarker, 0x2220);
CHECK_SIZE(cCrowdMan2d, 0x2720);

#endif /* MAIN_CROWDMAN2D_H */

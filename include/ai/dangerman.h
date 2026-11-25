#ifndef AI_DANGERMAN_H
#define AI_DANGERMAN_H

// cDangerMan: the AI's danger manager. It tracks "danger elements" (world instances that hurt riders: up to 64, added
// and removed by the DangerStartFunc / DangerEndFunc world scripts) and "danger zones" (trigger volumes with a danger
// level), keeps one danger record per rider and works out the danger level that applies to each rider
// (cDangerMan_CalcAppliedDangerLevel; the AI's handplant / avoidance code asks cDangerMan_GetRiderDangerLevel).
// One instance per cAI: cAI_cAI allocates it with cMemMan_alloc(0xB60, "DangerMan") + cDangerMan_cDangerMan(p, this) and
// stores it in cAI::DangerMan (ai/ai.h, +0xA8; the preview's getDangerMan). No vtable.
// EA's source file is not known (no preview assert names it; On Tour has it in ai/all_in_one_ai.cpp); the header path
// follows ai/ai.h. Retail's methods are in seg/seg_1218.cpp (0x101310-0x101A10: ctor, Update, Reset, AddDangerElem,
// RemoveDangerElem, AddDangerVolume, RemoveDangerVolume, GetRiderDangerLevel) and ai/control/handplantcontrol.cpp
// (0x1441A0 DeleteTrackedDangerElemRecord, 0x1441B8 CalcAppliedDangerLevel).
//
// Size 0xB60 [R] (the allocation). On Tour's class is 0xB48: retail has 6 rider records (the race's 6 riders; On Tour
// 4), so every member from mDangerZoneTrackHeap on sits 0x18 further on (On Tour 0x73C -> retail 0x754, ...).
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] retail ctor cDangerMan_cDangerMan (member ctors set every element's and zone's rID to -1) and cDangerMan_Reset;
//   [U] how retail code uses the field (its methods; visualfx/avalanche.cpp counts mNumActiveAvalanche);
//   [O] SSX On Tour's DWARF (cDangerMan 0xB48, tDangerElemInfo 0x1C, tRiderDangerTrack 0xC, tDangerZoneTrack 0x10):
//       member names, types and order. No preview evidence: tools/layout_xlate.py pairs no cDangerMan method.

#include "layout.h"

struct cAI;

// One tracked danger element (On Tour tDangerElemInfo, 0x1C).
struct tDangerElemInfo {
    float radius;            // 0x00 [O]; [U] cDangerMan_AddDangerElem stores its radius argument; CalcAppliedDangerLevel
                             //      tests the rider against a sphere of this radius
    float endDelaySecsLeft;  // 0x04 [O]; [U] AddDangerElem stores onEndDelaySecs; Update counts it down by 1/60 in state 1
    int state;               // 0x08 [O] eDangerElemState (0 active, 1 ending, 2 free); [R] Reset sets 2; [U] AddDangerElem 0,
                             //      Update 1 when the instance is gone or dead; CalcAppliedDangerLevel skips state 2
    float pos[3];            // 0x0C [O] t3vect; [U] AddDangerElem / Update copy the instance's position
    unsigned int rID;        // 0x18 [O] tResourceID; [R] the member ctor sets -1; [U] AddDangerElem stores the instance id,
                             //      RemoveDangerElem / Update look the instance up by it
};

CHECK_OFFSET(tDangerElemInfo, radius, 0x00);
CHECK_OFFSET(tDangerElemInfo, endDelaySecsLeft, 0x04);
CHECK_OFFSET(tDangerElemInfo, state, 0x08);
CHECK_OFFSET(tDangerElemInfo, pos, 0x0C);
CHECK_OFFSET(tDangerElemInfo, rID, 0x18);
CHECK_SIZE(tDangerElemInfo, 0x1C);

// A rider's danger record (On Tour tRiderDangerTrack, 0xC).
struct tRiderDangerTrack {
    int baseLevel;           // 0x0 [O]; [R] Reset 0; [U] Update stores the highest zone level; CalcAppliedDangerLevel reads it
    int appliedLevel;        // 0x4 [O]; [R] Reset 0; [U] Update stores CalcAppliedDangerLevel's result; GetRiderDangerLevel
    int expiryTicksLeft;     // 0x8 [O]; [U] Update: 60 when a zone is found, then counts down and clears the record at 0
};

CHECK_OFFSET(tRiderDangerTrack, baseLevel, 0x0);
CHECK_OFFSET(tRiderDangerTrack, appliedLevel, 0x4);
CHECK_OFFSET(tRiderDangerTrack, expiryTicksLeft, 0x8);
CHECK_SIZE(tRiderDangerTrack, 0xC);

// A danger zone (On Tour tDangerZoneTrack, 0x10): a node of the inactive (free) or active list.
struct tDangerZoneTrack {
    unsigned int rID;        // 0x0 [O] tResourceID; [R] the member ctor sets -1; [U] AddDangerVolume / RemoveDangerVolume
                             //      match the volume instance by it; Update looks the instance up
    int level;               // 0x4 [O]; [U] AddDangerVolume stores the level; Update takes the highest
    int refCount;            // 0x8 [O]; [U] AddDangerVolume / RemoveDangerVolume count references to the same volume
    tDangerZoneTrack* pNext; // 0xC [O]; [R] Reset chains the 64 nodes into the inactive list
};

CHECK_OFFSET(tDangerZoneTrack, rID, 0x0);
CHECK_OFFSET(tDangerZoneTrack, level, 0x4);
CHECK_OFFSET(tDangerZoneTrack, refCount, 0x8);
CHECK_OFFSET(tDangerZoneTrack, pNext, 0xC);
CHECK_SIZE(tDangerZoneTrack, 0x10);

struct cDangerMan {
    cAI* mpAI;                                   // 0x000 [O]; [R] the ctor's argument (cAI_cAI passes itself); [U] Update and
                                                 //       CalcAppliedDangerLevel read its riders (cAI +0x28) and count
    tDangerElemInfo mTrackedDangerElems[64];     // 0x004 [O]; [R] ctor (rID = -1), Reset (state = 2)
    int mTrackedDangerElemNum;                   // 0x704 [O]; [R] Reset 0; [U] AddDangerElem +1,
                                                 //       DeleteTrackedDangerElemRecord -1; Update / RemoveDangerElem / CalcAppliedDangerLevel test it
    int mNumActiveAvalanche;                     // 0x708 [O]; [R] Reset 0; [U] visualfx/avalanche.cpp counts it up when an
                                                 //       avalanche starts and down when it ends; CalcAppliedDangerLevel uses the
                                                 //       height bands while it is nonzero
    tRiderDangerTrack mRiderDangerTrack[6];      // 0x70C [O] ([4] in On Tour); [R] Reset clears 6; [U] Update /
                                                 //       RemoveDangerVolume / CalcAppliedDangerLevel / GetRiderDangerLevel index it by AI index
    tDangerZoneTrack mDangerZoneTrackHeap[64];   // 0x754 [O]; [R] ctor (rID = -1), Reset chains them (pNext of [63] = 0 at 0xB50)
    tDangerZoneTrack* mpInactiveList;            // 0xB54 [O]; [R] Reset = &mDangerZoneTrackHeap[0]; [U] AddDangerVolume pops,
                                                 //       RemoveDangerVolume pushes
    tDangerZoneTrack* mpActiveList;              // 0xB58 [O]; [R] Reset 0; [U] AddDangerVolume pushes, RemoveDangerVolume unlinks,
                                                 //       Update walks it
    int mRiderIndexDangerZoneCheck;              // 0xB5C [O]; [R] Reset 0; [U] Update tests one rider per tick (wraps to 0)

    // Methods bound to their retail symbols by asm labels.
    int DeleteTrackedDangerElemRecord(void* entry) __asm__("cDangerMan_DeleteTrackedDangerElemRecord__FPvT0");
    int CalcAppliedDangerLevel(int pl) __asm__("cDangerMan_CalcAppliedDangerLevel");
    void Reset() __asm__("cDangerMan_Reset");
    void AddDangerElem(unsigned int* idp, float x, float y) __asm__("cDangerMan_AddDangerElem");
    void RemoveDangerElem(unsigned int* idp) __asm__("cDangerMan_RemoveDangerElem");
    void AddDangerVolume(int *instId, int level) __asm__("cDangerMan_AddDangerVolume");
    void RemoveDangerVolume(int* key) __asm__("cDangerMan_RemoveDangerVolume");
    int GetRiderDangerLevel(int aiIndex) __asm__("cDangerMan_GetRiderDangerLevel");
};

CHECK_OFFSET(cDangerMan, mpAI, 0x000);
CHECK_OFFSET(cDangerMan, mTrackedDangerElems, 0x004);
CHECK_OFFSET(cDangerMan, mTrackedDangerElemNum, 0x704);
CHECK_OFFSET(cDangerMan, mNumActiveAvalanche, 0x708);
CHECK_OFFSET(cDangerMan, mRiderDangerTrack, 0x70C);
CHECK_OFFSET(cDangerMan, mDangerZoneTrackHeap, 0x754);
CHECK_OFFSET(cDangerMan, mpInactiveList, 0xB54);
CHECK_OFFSET(cDangerMan, mpActiveList, 0xB58);
CHECK_OFFSET(cDangerMan, mRiderIndexDangerZoneCheck, 0xB5C);
CHECK_SIZE(cDangerMan, 0xB60);

#endif /* AI_DANGERMAN_H */

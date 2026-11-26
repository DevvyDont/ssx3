#ifndef BE_BEINTMISSION_H
#define BE_BEINTMISSION_H

// cBEMissionInterface: the back end's mission interface - collectibles (totals and found counts per section, peak and
// mountain; the found bits), the missions (names, descriptions, location, points, difficulty, locked / ready /
// available / completed / show-icon flags, counts per track and peak), the session (teleport) points and the
// streamed mission data. Like every BE interface it is a cBEInterface (three word bools + vptr); it adds one word, the
// session point the next session teleport goes to. Its other methods ignore `this` and work on the mission and
// collectible tables and the profile library. One instance, created on demand by cBEMissionInterface_getThis
// (`cMemMan_alloc(0x14, "BEMissionInt", 0, 0)`, bIsActive = 0, vtable cBEMissionInterface_vtbl) into
// cBEMissionInterface::instance (retail cBEMissionInterface_instance, D_004A1238); the rest of the game reaches it as
// cBE_getInterface(cBE_getBE(), 10). EA's source: d:/ssx/code/source/be/beintmission.cpp (preview assert string).
//
// Size 0x14 [R] (the allocation; the July 2003 preview's getThis allocates 0x14 too).
// Base cBEInterface (0x00-0x0F), written out inline (the class itself: be/beinterface.h, whose virtuals give the slots):
// vtable cBEMissionInterface_vtbl (0x45ADF8), slot 1 submit = cBEMissionInterface_submit (empty), slot 2 discard =
// cBEMissionInterface_discard (empty), slots 3/4 its own cBEMissionInterface_writeToReplayFrame /
// cBEMissionInterface_readFromReplayFrame (they save / restore the current character's collect and mission records and
// the 4 bytes at &bIsActive).
// [O] On Tour's cBEMissionInterface (size 0x14 with its 8-byte base) has m_bMissionSetFromFE, m_missionSet and
// nextSessionID; retail has room for one word, and every retail use of it is a session point, so it is nextSessionID.
// The retail getter at 0x15BCF8 is the preview's getNextSessionID (both `lw v0, 0x10(a0)`; it was named
// isMissionSetFromFE until 2026-11-20), and the setter after it is setNextSessionID; they follow each other in
// be/bedbreward in the same order as On Tour's declarations getNextSessionID / setNextSessionID.
//
// Evidence tags:
//   [R] retail's getThis (allocation, bIsActive, vptr) and constructor cBEMissionInterface_cBEMissionInterface (vptr);
//   [P] the July 2003 preview's inline methods __19cBEMissionInterface (stores the vtable at 0xC), setMissionActive
//       (instance->+0x8), getNextSessionID__19cBEMissionInterface (lw 0x10) and
//       setNextSessionID__19cBEMissionInterfaceUi (sw a1, 0x10: an unsigned int);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface / cBEMissionInterface; On Tour's bools are 1 byte, retail's are words).

#include "layout.h"
#include "be/beinterface.h"

struct cBEMissionInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [P] setMissionActive; [U] cBEMissionInterface_setMissionActive (static:
    //       instance->bIsActive = arg); cBERaceState_onEnter/onExit set 0/1; write/readFromReplayFrame save and
    //       restore the 4 bytes at its address
    //   the vptr (0x0C): [R] getThis and cBEMissionInterface_cBEMissionInterface store cBEMissionInterface_vtbl; [P]
    //       __19cBEMissionInterface; [U] cBELibrary_readFromData and cBESaveInterface_generateSaveProfile call slot 2
    //       (discard) through it
    // cBEMissionInterface
    unsigned int nextSessionID;      // 0x010 [O] (On Tour 0x010, after m_bMissionSetFromFE and m_missionSet, which
                                     //       retail lacks); [P] get/setNextSessionID (lw / sw 0x10, unsigned int); [R]
                                     //       cBEMissionInterface_getNextSessionID returns it,
                                     //       cBEMissionInterface_setNextSessionID stores it; [U]
                                     //       cOVState_MAP_onWidgetEvent stores the map's selected session point
                                     //       (mSelectedSessionPoint) before cGame_setState(0xF),
                                     //       cGFSessionTeleportState_gainFocus passes it to cRider_putAtStartpoint;
                                     //       cGFUserSelectState_update and cOVTemplate_Dialog_onWidgetEvent store it
                                     //       too

    // Methods bound to their retail symbols by asm labels.
    int getNextSessionID() __asm__("cBEMissionInterface_getNextSessionID");
    void setNextSessionID(int sessionID) __asm__("cBEMissionInterface_setNextSessionID");
    int getTotalCollectForMnt(int start) __asm__("cBEMissionInterface_getTotalCollectForMnt");
    int getTotalCollectByType(int start) __asm__("cBEMissionInterface_getTotalCollectByType");
    int getTotalCollectForSection(int start) __asm__("cBEMissionInterface_getTotalCollectForSection");
    int getTotalCollectForPeak(int owner) __asm__("cBEMissionInterface_getTotalCollectForPeak");
    int getCurrentCollectForMnt(int rider, int idx) __asm__("cBEMissionInterface_getCurrentCollectForMnt");
    int getCurrentCollectByType(int rider, int idx) __asm__("cBEMissionInterface_getCurrentCollectByType");
    int getCurrentCollectForSection(int rider, int idx) __asm__("cBEMissionInterface_getCurrentCollectForSection");
    int getCurrentCollectForPeak(int rider, int owner) __asm__("cBEMissionInterface_getCurrentCollectForPeak");
    int getCurrentCollectForPeak1(int profile, int character, int owner)
        __asm__("cBEMissionInterface_getCurrentCollectForPeak1");
    int getCurrentCollectForPeakByChar(int profile, int character, int owner)
        __asm__("cBEMissionInterface_getCurrentCollectForPeakByChar");
    char* getCollectBits(int rider, int idx) __asm__("cBEMissionInterface_getCollectBits");
    bool isBitSet(int rider, int bit, int idx) __asm__("cBEMissionInterface_isBitSet");
    void setBit(int rider, int bit, int idx) __asm__("cBEMissionInterface_setBit");
    unsigned short* getMissionName(int id) __asm__("cBEMissionInterface_getMissionName");
    unsigned short* getMissionShortDesc(int id) __asm__("cBEMissionInterface_getMissionShortDesc");
    unsigned short* getMissionLongDesc(int id) __asm__("cBEMissionInterface_getMissionLongDesc");
    int isMissionCompleted(int id) __asm__("cBEMissionInterface_isMissionCompleted");
    int isMissionReady(int id) __asm__("cBEMissionInterface_isMissionReady");
    int isMissionLocked(int id) __asm__("cBEMissionInterface_isMissionLocked");
    int isMissionAvailable(int id) __asm__("cBEMissionInterface_isMissionAvailable");
    int isMissionShowIcon(int id) __asm__("cBEMissionInterface_isMissionShowIcon");
    int getLocationID(int id) __asm__("cBEMissionInterface_getLocationID");
    int getPoints(int id) __asm__("cBEMissionInterface_getPoints");
    short getDiffLevel(int id) __asm__("cBEMissionInterface_getDiffLevel");
    int getNextMissionID(int id) __asm__("cBEMissionInterface_getNextMissionID");
    void setIsLocked(int id, int value) __asm__("cBEMissionInterface_setIsLocked");
    void setIsReady(int id, int value) __asm__("cBEMissionInterface_setIsReady");
    void setIsAvailable(int id, int value) __asm__("cBEMissionInterface_setIsAvailable");
    void setIsCompleted(int id, int value) __asm__("cBEMissionInterface_setIsCompleted");
    void setIsShowIcon(int id, int value) __asm__("cBEMissionInterface_setIsShowIcon");
    int getMissionIdx(int id) __asm__("cBEMissionInterface_getMissionIdx");
    int getNumberMissions() __asm__("cBEMissionInterface_getNumberMissions");
    int getNumberMissionsOnTrack(int group) __asm__("cBEMissionInterface_getNumberMissionsOnTrack");
    int getNumberCompletedMissionsOnTrack(int group) __asm__("cBEMissionInterface_getNumberCompletedMissionsOnTrack");
    int getNumberMissionsOnPeak(int owner) __asm__("cBEMissionInterface_getNumberMissionsOnPeak");
    int getNumberCompletedMissionsOnPeak(int owner) __asm__("cBEMissionInterface_getNumberCompletedMissionsOnPeak");
    int getNumberCompletedMissionsOnPeakByChar(int profile, int character, int owner)
        __asm__("cBEMissionInterface_getNumberCompletedMissionsOnPeakByChar");
    int getMissionID(int iIdx) __asm__("cBEMissionInterface_getMissionID");
    int getMissionIDOnTrack(int index, int group) __asm__("cBEMissionInterface_getMissionIDOnTrack");
    int getNumberSessionsOnTrack(int id) __asm__("cBEMissionInterface_getNumberSessionsOnTrack");
    bool isSessionPoint(int value, int id) __asm__("cBEMissionInterface_isSessionPoint");
};

extern cBEMissionInterface* cBEMissionInterface_instance;

CHECK_OFFSET(cBEMissionInterface, nextSessionID, 0x10);
CHECK_SIZE(cBEMissionInterface, 0x14);

// The one mission interface (On Tour: cBEMissionInterface::instance), created by cBEMissionInterface_getThis.

// tBEPictureDB: one location's map picture record (the 22-entry static table cBELibrary_pictureDB). [O] On Tour
// struct tBEPictureDB (0x2C), the record of the static cBEMissionInterface::pictureDB[22] that
// cBEMissionInterface::getPictureDBRecord(eBELocationID) returns by value; [R] retail's cBEMissionInterface_getPictureDBRecord
// returns record i of a 22-entry, 0x2C-stride table the same way (record 0 when no id matches). [U] the id is matched by
// the session-count lookups (be/beintmission.cpp's view sMissionEntry {id 0x0, value 0x18} is this record),
// getPictureFileName returns +0x4, and cOVState_MAP_setupLocalSessionList maps session points into the picture
// with the four floats (fe/ovstatemap.cpp's view sBounds_96A8 minX / minY / maxX / maxY).
struct tBEPictureDB {
    int iLocationID;                // 0x00 [O] eBELocationID; [U] getPictureDBRecord / the session-count lookups
    char strFileName[20];           // 0x04 [O]; [U] cBEMissionInterface_getPictureFileName returns it
    int numSessionPoints;           // 0x18 [O]; [U] the session count read by the lookups after the id
    float world_topleft_x;          // 0x1C [O]; [U] setupLocalSessionList (the picture's world bounds)
    float world_topleft_y;          // 0x20 [O]
    float world_bottomright_x;      // 0x24 [O]
    float world_bottomright_y;      // 0x28 [O]
};

CHECK_OFFSET(tBEPictureDB, iLocationID, 0x00);
CHECK_OFFSET(tBEPictureDB, strFileName, 0x04);
CHECK_OFFSET(tBEPictureDB, numSessionPoints, 0x18);
CHECK_OFFSET(tBEPictureDB, world_topleft_x, 0x1C);
CHECK_OFFSET(tBEPictureDB, world_topleft_y, 0x20);
CHECK_OFFSET(tBEPictureDB, world_bottomright_x, 0x24);
CHECK_OFFSET(tBEPictureDB, world_bottomright_y, 0x28);
CHECK_SIZE(tBEPictureDB, 0x2C);

// tBEMissionInfo: one mission's static record, the 88-entry table cBELibrary_missionInfo (retail D_0043EE10, 0xC60
// bytes: 88 x 0x24). Type and member names ours; [U] be/beintmission.cpp's getters read one field each (index from
// cBEMissionInterface_getMissionIdx), [R] cBECharProfileDB_init (be/beintecon.cpp) seeds the mission flags from it.
struct tBEMissionInfo {
    int id;                         // 0x00 [U] getMissionIdx / getAudioType match it against the mission id
    int nameHash;                   // 0x04 [U] getMissionName: FELocale->getStringByHash(it), the mission's name
    int shortDescHash;              // 0x08 [U] getMissionShortDesc: FELocale->getStringByHash(it)
    int longDescHash;               // 0x0C [U] getMissionLongDesc: FELocale->getStringByHash(it)
    int locationID;                 // 0x10 [U] getLocationID returns it; the per-track counts compare it with the
                                    //       location (eBELocationID)
    int points;                     // 0x14 [U] getPoints returns it
    short diffLevel;                // 0x18 [U] getDiffLevel returns it (1-3)
    signed char bStartLocked;       // 0x1A [R] cBECharProfileDB_init copies it to mMissions[i].bIsLocked
    signed char bStartShowIcon;     // 0x1B [R] cBECharProfileDB_init copies it to mMissions[i].bIsShowIcon
    int nextMissionID;              // 0x1C [U] getNextMissionID returns it
    char pad_0x20[0x2];
    short audioType;                // 0x22 [U] getAudioType returns it (the sound system's mission audio kind)
};

CHECK_OFFSET(tBEMissionInfo, id, 0x00);
CHECK_OFFSET(tBEMissionInfo, nameHash, 0x04);
CHECK_OFFSET(tBEMissionInfo, shortDescHash, 0x08);
CHECK_OFFSET(tBEMissionInfo, longDescHash, 0x0C);
CHECK_OFFSET(tBEMissionInfo, locationID, 0x10);
CHECK_OFFSET(tBEMissionInfo, points, 0x14);
CHECK_OFFSET(tBEMissionInfo, diffLevel, 0x18);
CHECK_OFFSET(tBEMissionInfo, bStartLocked, 0x1A);
CHECK_OFFSET(tBEMissionInfo, bStartShowIcon, 0x1B);
CHECK_OFFSET(tBEMissionInfo, nextMissionID, 0x1C);
CHECK_OFFSET(tBEMissionInfo, audioType, 0x22);
CHECK_SIZE(tBEMissionInfo, 0x24);

#endif /* BE_BEINTMISSION_H */

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

#endif /* BE_BEINTMISSION_H */

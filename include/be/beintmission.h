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
// Base cBEInterface (0x00-0x0F), written out inline as in be/beintreward.h, whose kBEInterfaceVt_* slot enum applies:
// vtable cBEMissionInterface_vtbl (0x45ADF8), slot 1 submit = cBEMissionInterface_submit (empty), slot 2 discard =
// cBEMissionInterface_discard (empty), slots 3/4 its own cBEMissionInterface_writeToReplayFrame /
// cBEMissionInterface_readFromReplayFrame (they save / restore the current character's collect and mission records and
// the 4 bytes at &bIsActive).
// [O] On Tour's cBEMissionInterface (size 0x14 with its 8-byte base) has m_bMissionSetFromFE, m_missionSet and
// nextSessionID; retail has room for one word, and every retail use of it is a session point, so it is nextSessionID.
// NOTE: the retail getter at 0x15BCF8 is named cBEMissionInterface_isMissionSetFromFE in symbol_addrs, but it is the
// preview's getNextSessionID (both `lw v0, 0x10(a0)`), and func_0015BD00 after it is setNextSessionID; they follow
// each other in be/bedbreward in the same order as On Tour's declarations getNextSessionID / setNextSessionID.
//
// Evidence tags:
//   [R] retail's getThis (allocation, bIsActive, vptr) and constructor cBEMissionInterface_cBEMissionInterface (vptr);
//   [P] the July 2003 preview's inline methods __19cBEMissionInterface (stores the vtable at 0xC), setMissionActive
//       (instance->+0x8), getNextSessionID__19cBEMissionInterface (lw 0x10) and
//       setNextSessionID__19cBEMissionInterfaceUi (sw a1, 0x10: an unsigned int);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface / cBEMissionInterface; On Tour's bools are 1 byte, retail's are words).

#include "layout.h"

struct cBEMissionInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] getThis sets 0; [P] setMissionActive; [U]
                                     //       cBEMissionInterface_setMissionActive (static: instance->bIsActive = arg);
                                     //       cBERaceState_onEnter/onExit set 0/1; write/readFromReplayFrame save and
                                     //       restore the 4 bytes at its address
    void** mVtbl;                    // 0x00C [R] getThis and cBEMissionInterface_cBEMissionInterface store
                                     //       cBEMissionInterface_vtbl; [P] __19cBEMissionInterface; [U]
                                     //       cBELibrary_readFromData and cBESaveInterface_generateSaveProfile call slot
                                     //       2 (discard) through it
    // cBEMissionInterface
    unsigned int nextSessionID;      // 0x010 [O] (On Tour 0x010, after m_bMissionSetFromFE and m_missionSet, which
                                     //       retail lacks); [P] get/setNextSessionID (lw / sw 0x10, unsigned int); [R]
                                     //       0x15BCF8 (misnamed cBEMissionInterface_isMissionSetFromFE) returns it,
                                     //       func_0015BD00 stores it; [U] cOVState_MAP_onWidgetEvent stores the map's
                                     //       selected session point (mSelectedSessionPoint) before cGame_setState(0xF),
                                     //       cGFSessionTeleportState_gainFocus passes it to cRider_putAtStartpoint;
                                     //       cGFUserSelectState_update and cOVTemplate_Dialog_onWidgetEvent store it
                                     //       too
};

extern cBEMissionInterface* cBEMissionInterface_instance;

CHECK_OFFSET(cBEMissionInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBEMissionInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBEMissionInterface, bIsActive, 0x08);
CHECK_OFFSET(cBEMissionInterface, mVtbl, 0x0C);
CHECK_OFFSET(cBEMissionInterface, nextSessionID, 0x10);
CHECK_SIZE(cBEMissionInterface, 0x14);

// The one mission interface (On Tour: cBEMissionInterface::instance), created by cBEMissionInterface_getThis.

#endif /* BE_BEINTMISSION_H */

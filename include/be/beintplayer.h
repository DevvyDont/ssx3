#ifndef BE_BEINTPLAYER_H
#define BE_BEINTPLAYER_H

// cBENewPlayerInterface: the back end's player interface - who the riders of the next race are (character, cheat
// character, controller, name, board/skis/skin, the difficulty level) and their career flags (first time on a track,
// peak / rival / event locks, last hub, the mission-man rider). The object holds no player data: like every BE
// interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore `this` and work on the
// statics cBENewPlayerInterface::tempPlayers (retail cBENewPlayerInterface_tempPlayers, D_00534FE0: six 0x1C-byte
// cBEPlayerDB records, the working copy), cBELibrary::playerDB (cBELibrary_playerDB, D_00535B20, which submit copies
// them to) and the profile library. One instance, created on demand by cBENewPlayerInterface_getThis
// (`cMemMan_alloc(0x10, "BENewPlayerInt", 0, 0)` + the constructor cBENewPlayerInterface_cBENewPlayerInterface) into
// cBENewPlayerInterface::instance (retail cBENewPlayerInterface_instance, D_004A11C0); the rest of the game reaches it
// as cBE_getInterface(cBE_getBE(), 1). EA's source: d:/ssx/code/source/be/beintplayer.cpp (preview assert string).
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's
// cBENewPlayerInterface has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
// cBENewPlayerInterface_vtbl (0x45AE28), slot 1 submit = cBENewPlayerInterface_submit, slot 2 discard =
// cBENewPlayerInterface_discard, slots 3/4 cBEInterface_writeToReplayFrame / cBEInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's constructor cBENewPlayerInterface_cBENewPlayerInterface (bIsActive = 0, vptr) and the allocation;
//   [P] the July 2003 preview's inline methods __21cBENewPlayerInterface (stores the vtable at 0xC) and
//       setNewPlayerActive (instance->+0x8 = arg);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).
// The local view cBENewPlayerInterfaceCtor (be/beintplayer.cpp: pad[8], field_0x8, vtable) has the same layout.

#include "layout.h"
#include "be/beinterface.h"

struct cBENewPlayerInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] the constructor sets 0; [P] setNewPlayerActive; [U] cBENewPlayerInterface_setNewPlayerActive
    //       (static: instance->bIsActive = arg); cBENewPlayerInterface_submit copies the whole tempPlayers table to
    //       playerDB only while it is set (otherwise ingameSubmit: names and flag bit 1);
    //       cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit set 0/1, cBERaceState_onExit clears it
    //   the vptr (0x0C): [R] the constructor stores cBENewPlayerInterface_vtbl; [P] __21cBENewPlayerInterface; [U]
    //       slot 1 (submit) is called through it by ~20 functions (cAI_initComputerRiders, cGameComm_syncGame,
    //       cFEStateCharSelect_onWidgetEvent, cGFTrackStartState_gainFocus, ...), slot 2 (discard) by ~14
    //       (cBELibrary_newProfile, cBELibrary_readFromData, cBESaveInterface_loadSaveGame,
    //       cBENewPlayerInterface_defaultCtrl, ...)

    // Methods bound to their retail symbols by asm labels.
    void defaultCtrl() __asm__("cBENewPlayerInterface_defaultCtrl");
    int isFirstTimeWC(int index) __asm__("cBENewPlayerInterface_isFirstTimeWC");
    void setIsFirstTimeWC(int index, int value) __asm__("cBENewPlayerInterface_setIsFirstTimeWC");
    bool isFirstTimeOnTrack(int index, int bit) __asm__("cBENewPlayerInterface_isFirstTimeOnTrack");
    void setFirstTimeOnTrack(bool flag, int index, int bit) __asm__("cBENewPlayerInterface_setFirstTimeOnTrack");
    int hasVisitedPeakWC(int index, int kind) __asm__("cBENewPlayerInterface_hasVisitedPeakWC");
    bool hasPlayedEvent(int index, int locationId, int event) __asm__("cBENewPlayerInterface_hasPlayedEvent");
    int isPeakLocked(int iProfileIndex, int iCharacterID, int peak) __asm__("cBENewPlayerInterface_isPeakLocked");
    int isPeakLocked1(int rider, int peak) __asm__("cBENewPlayerInterface_isPeakLocked1");
    void setPeakLocked(int index, int kind, int value) __asm__("cBENewPlayerInterface_setPeakLocked");
    int isRivalChallengePlayed(int index, int kind, int flag) __asm__("cBENewPlayerInterface_isRivalChallengePlayed");
    void setRivalChallengePlayed(int index, int kind, int flag, int value)
        __asm__("cBENewPlayerInterface_setRivalChallengePlayed");
    int isRivalEventLocked(int index, int character, int kind, int flag)
        __asm__("cBENewPlayerInterface_isRivalEventLocked");
    int isRivalEventLocked1(int index) __asm__("cBENewPlayerInterface_isRivalEventLocked1");
    void setRivalEventLocked(int index, int character, int kind, int flag, int value)
        __asm__("cBENewPlayerInterface_setRivalEventLocked");
    int isEventLocked(int index, int character, int kind) __asm__("cBENewPlayerInterface_isEventLocked");
    void setEventLocked(int rider, int character, int event, int locked)
        __asm__("cBENewPlayerInterface_setEventLocked");
    int getLastHub(int index) __asm__("cBENewPlayerInterface_getLastHub");
    void setLastHub(int index, int value) __asm__("cBENewPlayerInterface_setLastHub");
    int getHumanRider(int second) __asm__("cBENewPlayerInterface_getHumanRider");
    void setPlayerName(int playerIndex, const char* name) __asm__("cBENewPlayerInterface_setPlayerName");
    char* getPlayerName(int idx) __asm__("cBENewPlayerInterface_getPlayerName");
    void setRiderCtrlID(int index, int ctrl) __asm__("cBENewPlayerInterface_setRiderCtrlID");
    int getRiderCtrlID(int index) __asm__("cBENewPlayerInterface_getRiderCtrlID");
    void setPlayerCtrlID(int iPlayer, int ctrlID) __asm__("cBENewPlayerInterface_setPlayerCtrlID");
    int getPlayerCtrlID(int iPlayer) __asm__("cBENewPlayerInterface_getPlayerCtrlID");
    void setRiderCharID(int index, int charID) __asm__("cBENewPlayerInterface_setRiderCharID");
    void setRiderCheatCharID(int index, int value) __asm__("cBENewPlayerInterface_setRiderCheatCharID");
    signed char getRiderCheatCharID(int index) __asm__("cBENewPlayerInterface_getRiderCheatCharID");
    void setPlayerCharID(int iPlayer, int charID) __asm__("cBENewPlayerInterface_setPlayerCharID");
    void setPlayerCheatCharID(int playerId, int value) __asm__("cBENewPlayerInterface_setPlayerCheatCharID");
    signed char getPlayerCheatCharID(int playerId) __asm__("cBENewPlayerInterface_getPlayerCheatCharID");
    int getUseDualShock(int index) __asm__("cBENewPlayerInterface_getUseDualShock");
    void setUseDualShock(int index, int value) __asm__("cBENewPlayerInterface_setUseDualShock");
    int getHelpDone(int index) __asm__("cBENewPlayerInterface_getHelpDone");
    void setHelpDone(int index, int value) __asm__("cBENewPlayerInterface_setHelpDone");
    signed char getControllerMapping(int index) __asm__("cBENewPlayerInterface_getControllerMapping");
    void setControllerMapping(int index, int value) __asm__("cBENewPlayerInterface_setControllerMapping");
    int getCamera(int index) __asm__("cBENewPlayerInterface_getCamera");
    void setCamera(int index, int value) __asm__("cBENewPlayerInterface_setCamera");
    int getRiderHumanIndex(int index) __asm__("cBENewPlayerInterface_getRiderHumanIndex");
    void resetFromMissionMan(int index) __asm__("cBENewPlayerInterface_resetFromMissionMan");
    void setDifficultyLevel(int value, int flag) __asm__("cBENewPlayerInterface_setDifficultyLevel");
    short getDifficultyLevel(int which) __asm__("cBENewPlayerInterface_getDifficultyLevel");
    short getDifficultyDifference(int flag) __asm__("cBENewPlayerInterface_getDifficultyDifference");
    signed char getRiderCharID(int riderIndex) __asm__("cBENewPlayerInterface_getRiderCharID__FPvi");
    signed char getPlayerCharID(int index) __asm__("cBENewPlayerInterface_getPlayerCharID__FPvi");
    // submit stays a C function (cBENewPlayerInterface_submit): declared first, it is the vtable's key function, which
    // keeps the vtable out of the units defining the other overrides
    virtual void submit() __asm__("cBENewPlayerInterface_submit");
    virtual void discard() __asm__("cBENewPlayerInterface_discard");
};

extern cBENewPlayerInterface* cBENewPlayerInterface_instance;

CHECK_SIZE(cBENewPlayerInterface, 0x10);

// The one player interface (On Tour: cBENewPlayerInterface::instance), created by cBENewPlayerInterface_getThis.

// ---- cBEPlayerDB, one player's record (views group D) ----
//
// cBEPlayerDB (retail 0x1C): the six records of cBENewPlayerInterface::tempPlayers (retail
// cBENewPlayerInterface_tempPlayers, D_00534FE0) and of cBELibrary::playerDB (cBELibrary_playerDB, D_00535B20); submit /
// discard copy all six (6 * 0x1C). [O] On Tour class cBEPlayerDB (0x3C) has the same members in the same order with a
// longer name, a full-word iHumanID and the board / skin fields between cCharacterID and cCheatCharacterID; retail keeps
// strUserName[12], packs iHumanID into the flag word and has no board fields. Tags: [O] On Tour's member order and names;
// [R] the retail accessors of cBENewPlayerInterface (offsets in the comments); [U] how they use the field.
struct cBEPlayerDB {
    char strUserName[12];           // 0x00 [O] ([24] in On Tour); [R] setPlayerName strncpy's 0xC bytes, getPlayerName
                                    //      returns the record's address
    int iControllerID;              // 0x0C [O]; [R] setRiderCtrlID / setPlayerCtrlID store the pad, getRiderCtrlID /
                                    //      getPlayerCtrlID return it

    // iControllerID of a rider no pad drives, a computer rider (name ours): [U] cBELibrary_getProfileIndex gives it
    // the computer profile (2), cBENewPlayerInterface_getRiderHumanIndex returns -1 for it, getHumanRider counts only
    // the riders with a controller of 0 or more.
    enum { kNoController = -1 };

    unsigned int iHumanID : 1;      // 0x10 bit 0 [O] (a full word in On Tour); [R] setPlayerCtrlID / setRiderCtrlID store
                                    //      the low bit of the player index
    unsigned int iUseDualShock : 1; // 0x10 bit 1 [O]; [R] get/setUseDualShock; reset sets it
    unsigned int iIsMissionMan : 1; // 0x10 bit 2 [O]; [R] isMissionMan / setToMissionMan; setPlayerCharID /
                                    //      setRiderCharID copy player 0's character into a mission-man record;
                                    //      setPlayerCtrlID clears it
    unsigned int iIsHelpDone : 1;   // 0x10 bit 3 [O]; [R] getHelpDone / setHelpDone
    unsigned int : 4;               // 0x10 bits 4-7 unused
    signed char cCharacterID;       // 0x11 [O]; [R] setPlayerCharID / setRiderCharID store it (sb), getPlayerCharID reads it
    signed char cCheatCharacterID;  // 0x12 [O]; [R] set/getPlayerCheatCharID, set/getRiderCheatCharID
    signed char cControlMap;        // 0x13 [O] (On Tour's member after cCheatCharacterID); [R] get/setControllerMapping;
                                    //      the reset clears it
    int iCamera;                    // 0x14 [R] cBENewPlayerInterface_getCamera / cBENewPlayerInterface_setCamera get / set it per player; [O] On Tour
                                    //      cBEPlayerDB::iCamera (0x38 there, after cControlMap as here)
    int unk_018;                    // 0x18 [R] func_00147718 / func_00147758 get / set it per player
};

CHECK_OFFSET(cBEPlayerDB, strUserName, 0x00);
CHECK_OFFSET(cBEPlayerDB, iControllerID, 0x0C);
CHECK_OFFSET(cBEPlayerDB, cCharacterID, 0x11);
CHECK_OFFSET(cBEPlayerDB, cCheatCharacterID, 0x12);
CHECK_OFFSET(cBEPlayerDB, cControlMap, 0x13);
CHECK_OFFSET(cBEPlayerDB, iCamera, 0x14);
CHECK_OFFSET(cBEPlayerDB, unk_018, 0x18);
CHECK_SIZE(cBEPlayerDB, 0x1C);

extern cBEPlayerDB cBENewPlayerInterface_tempPlayers[6];  // [O] cBENewPlayerInterface::tempPlayers; [R] D_00534FE0
extern cBEPlayerDB cBELibrary_playerDB[6];                // [O] cBELibrary::playerDB; [R] D_00535B20


#endif /* BE_BEINTPLAYER_H */

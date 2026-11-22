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
// Base cBEInterface, written out inline as in be/beintreward.h, whose kBEInterfaceVt_* slot enum applies: vtable
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

struct cBENewPlayerInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] the constructor sets 0; [P]
                                     //       setNewPlayerActive; [U] cBENewPlayerInterface_setNewPlayerActive (static:
                                     //       instance->bIsActive = arg); cBENewPlayerInterface_submit copies the whole
                                     //       tempPlayers table to playerDB only while it is set (otherwise
                                     //       ingameSubmit: names and flag bit 1); cBEPreRaceState_onEnter/onExit,
                                     //       cBEPostRaceState_onEnter/onExit set 0/1, cBERaceState_onExit clears it
    void** mVtbl;                    // 0x00C [R] the constructor stores cBENewPlayerInterface_vtbl; [P]
                                     //       __21cBENewPlayerInterface; [U] slot 1 (submit) is called through it by ~20
                                     //       functions (cAI_initComputerRiders, cGameComm_syncGame,
                                     //       cFEStateCharSelect_onWidgetEvent, cGFTrackStartState_gainFocus, ...), slot
                                     //       2 (discard) by ~14 (cBELibrary_newProfile, cBELibrary_readFromData,
                                     //       cBESaveInterface_loadSaveGame, cBENewPlayerInterface_defaultCtrl, ...)
};

extern cBENewPlayerInterface* cBENewPlayerInterface_instance;

CHECK_OFFSET(cBENewPlayerInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBENewPlayerInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBENewPlayerInterface, bIsActive, 0x08);
CHECK_OFFSET(cBENewPlayerInterface, mVtbl, 0x0C);
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
    int unk_014;                    // 0x14 [R] cBENewPlayerInterface_getCamera / cBENewPlayerInterface_setCamera get / set it per player (no On Tour
                                    //      counterpart at this position)
    int unk_018;                    // 0x18 [R] func_00147718 / func_00147758 get / set it per player
};

CHECK_OFFSET(cBEPlayerDB, strUserName, 0x00);
CHECK_OFFSET(cBEPlayerDB, iControllerID, 0x0C);
CHECK_OFFSET(cBEPlayerDB, cCharacterID, 0x11);
CHECK_OFFSET(cBEPlayerDB, cCheatCharacterID, 0x12);
CHECK_OFFSET(cBEPlayerDB, cControlMap, 0x13);
CHECK_OFFSET(cBEPlayerDB, unk_014, 0x14);
CHECK_OFFSET(cBEPlayerDB, unk_018, 0x18);
CHECK_SIZE(cBEPlayerDB, 0x1C);

extern cBEPlayerDB cBENewPlayerInterface_tempPlayers[6];  // [O] cBENewPlayerInterface::tempPlayers; [R] D_00534FE0
extern cBEPlayerDB cBELibrary_playerDB[6];                // [O] cBELibrary::playerDB; [R] D_00535B20


#endif /* BE_BEINTPLAYER_H */

#ifndef BE_BEINTNETWORK_H
#define BE_BEINTNETWORK_H

// cBENetworkInterface: the back end's network-game interface. Its object carries no network data of its own: like
// every BE interface it is a cBEInterface (three bools + vptr, 0x10 bytes), and its accessors read and write the
// static tBENetworkDB networkOpt below (network game / ranked / host flags, port, latency, names, the two riders'
// character IDs and stats). One instance, created on demand by cBENetworkInterface_getThis
// (`cMemMan_alloc(0x10, "BENetworkInt", 0, 0)`, bIsActive = 0, vtable D_0045AD98) into cBENetworkInterface::instance
// (retail cBENetworkInterface_instance); the rest of the game reaches it as cBE_getInterface(SSXApp->BE, 7). EA's source:
// d:/ssx/code/source/be/beintnetwork.cpp (the preview build's assert string).
//
// Size 0x10 [R] (the allocation above; the preview's getThis allocates 0x10 too). Base cBEInterface, written out
// inline exactly as in be/beintreward.h (whose kBEInterfaceVt_* slot enum applies here: slot 1 submit = func_0014E130
// copies networkOpt into cBELibrary::networkDB, slot 2 discard = func_0014E1F8 copies it back, slots 3/4 are
// cBEInterface's replay-frame hooks).
//
// Statics (retail addresses; names from On Tour's symbol table):
//   cBENetworkInterface::instance       cBENetworkInterface_instance
//   cBENetworkInterface::networkOpt     cBENetworkInterface_networkOpt  tBENetworkDB (0x4B0; read by ~20 units; splat also names the
//                                                   interior symbols D_00534B34 ... D_00534FD8 used below)
//   cBELibrary::networkDB               D_00535088  tBENetworkDB (submit/discard's other side; On Tour has both)
//   cBENetworkInterface::BackupPlayerDB D_00534A88  6 player records of 0x1C (backupCharProfile cBENetworkInterface_backupCharProfile /
//                                                   restoreCharProfile cBENetworkInterface_restoreCharProfile; On Tour: cBEPlayerDB[4])
//
// Evidence tags:
//   [R] retail's own code: getThis (allocation, vptr, bIsActive), the constructor func_0015C660 (vptr), and the
//       out-of-line accessors func_0015C250..func_0015C618 (be/bedbreward), each of which reads or writes one
//       tBENetworkDB field;
//   [P] the July 2003 preview's inline accessors (getIsNetworkGame ... setOpponentStats), which address networkOpt
//       (preview base 0x5B5358) at the same offsets as retail up to OpponentStats (preview 0x3D8, retail 0x3DC:
//       tRiderStats grew from 0xA8 to 0xAC bytes);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface / cBENetworkInterface, struct tBENetworkDB, size 0x330): member
//       names and order; On Tour's bools are 1 byte, retail's are words, and On Tour drops the character IDs and
//       stats (it adds bDisabledPlayers[4] and nPlayerID for 4 players).
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

// tBENetworkDB: the network-game options (cBENetworkInterface::networkOpt / cBELibrary::networkDB), 0x4B0 bytes
// ([R] submit/discard copy 0x4B0 bytes).
struct tBENetworkDB {
    int bIsNetworkGame;              // 0x000 [O]; [P] get/setIsNetworkGame; [R] func_0015C278 / func_0015C288;
                                     //       [U] tested by ~20 units ("is this an online race")
    int bIsRankedGame;               // 0x004 [O]; [P] get/setIsRankedGame; [R] func_0015C298 / func_0015C2A8
    int bIsHost;                     // 0x008 [O]; [P] get/setIsHost; [R] func_0015C2B8 / func_0015C2C8 (D_00534B38);
                                     //       [U] cGameComm_syncGame / syncWait / gameExit, setSyncData (cBENetworkInterface_setSyncData),
                                     //       preInitBE (cBENetworkInterface_preInitBE) and the results report pick the host side by it
    int iPort;                       // 0x00C [O]; [P] get/setPort; [R] func_0015C2D8 / func_0015C2E8 (D_00534B3C);
                                     //       [U] debugfe (func_0017D0A8, func_0017DE70) and tagsunk func_002639E0 read
                                     //       it as an unsigned short (lhu) for openDSockChannel
    float fLatency;                  // 0x010 [O]; [P] get/setLatency (float); [R] func_0015C2F8 / func_0015C308
                                     //       (D_00534B40); [U] cGameComm_syncGame stores cGameComm::mLatency, cGame_load
                                     //       sizes cInputSync from it
    char strLocalAddr[256];          // 0x014 [O] (On Tour 0x00C, same size); the 0x200 bytes between fLatency and
    char strRemoteAddr[256];         // 0x114 [O]  iClientID (both builds); no retail access except whole-struct copies
    int iClientID;                   // 0x214 [O]; [P] get/setClientID (preview +0x214); [R] func_0015C318 / func_0015C328
                                     //       (D_00534D44)
    char strHostName[120];           // 0x218 [O] (size 120); [P] get/setHostName; [R] func_0015C338 (strcpy) /
                                     //       func_0015C358 (D_00534D48)
    char strGameName[120];           // 0x290 [O]; [P] get/setGameName; [R] func_0015C368 / func_0015C388 (D_00534DC0)
    char strPassword[30];            // 0x308 [O] (size 30); [P] get/setPassword; [R] func_0015C398 / func_0015C3B8
                                     //       (D_00534E38)
    char unk_326[0x2];               // 0x326 padding
    int MyCharacterID;               // 0x328 [P] get/setMyCharacterID (field name from the accessor; not in On Tour);
                                     //       [R] func_0015C3D8 / func_0015C3E8 (D_00534E58); "CHARACTER0" in the report
    int OpponentCharacterID;         // 0x32C [P] get/setOpponentCharacterID; [R] func_0015C3F8 / func_0015C408 (D_00534E5C)
    char MyStats[0xAC];              // 0x330 [P] get/setMyStats(tRiderStats const&); [R] setter func_0015C418 copies 0xAC
                                     //       bytes, getter func_0015C4D0 (D_00534E60); a tRiderStats (not modelled here)
    char OpponentStats[0xAC];        // 0x3DC [P] get/setOpponentStats; [R] func_0015C4E0 / func_0015C598 (D_00534F0C)
    char unk_488[0x10];              // 0x488 [R] func_0014EBD8 (strcpy) / func_0015C5B8 (D_00534FB8); [U] this player's
                                     //       name (from the lobby session +0x80, func_00259098); prefixes our own chat
                                     //       lines (ovstatetrophy func_00210BD8)
    char unk_498[0x10];              // 0x498 [R] func_0014EBB8 / func_0015C5A8 (D_00534FC8); [U] the opponent's name
                                     //       (lobby session +0x98); prefixes received chat, shown by func_00210940
    unsigned char oppCharacterAttributes[7]; // 0x4A8 [O] (On Tour 0x326); [R] setOpponentAttributes func_0015C5C8
                                     //       copies 7 bytes, getOpponentAttributes func_0015C600, getOverallOpponentLevel
                                     //       func_0015C618 sums them / 35 (D_00534FD8)
    char unk_4AF[0x1];               // 0x4AF padding
};

struct cBENetworkInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] getThis sets 0; [U] setNetworkActive
                                     //       (func_0015C3C8, static: instance->bIsActive = arg); set 0/1 with every BE
                                     //       interface (cBEPostRaceState_onExit/cBEPostRaceState_onEnter/cBEPreRaceState_onExit/cBEPreRaceState_onEnter)
    void** mVtbl;                    // 0x00C [R] D_0045AD98 (getThis, ctor func_0015C660); [P] the preview ctor
                                     //       __19cBENetworkInterface stores _vt$19cBENetworkInterface at 0xC;
                                     //       [U] func_00259098 calls slot 1 (submit) through it
};

CHECK_OFFSET(tBENetworkDB, bIsNetworkGame, 0x000);
CHECK_OFFSET(tBENetworkDB, bIsRankedGame, 0x004);
CHECK_OFFSET(tBENetworkDB, bIsHost, 0x008);
CHECK_OFFSET(tBENetworkDB, iPort, 0x00C);
CHECK_OFFSET(tBENetworkDB, fLatency, 0x010);
CHECK_OFFSET(tBENetworkDB, strLocalAddr, 0x014);
CHECK_OFFSET(tBENetworkDB, strRemoteAddr, 0x114);
CHECK_OFFSET(tBENetworkDB, iClientID, 0x214);
CHECK_OFFSET(tBENetworkDB, strHostName, 0x218);
CHECK_OFFSET(tBENetworkDB, strGameName, 0x290);
CHECK_OFFSET(tBENetworkDB, strPassword, 0x308);
CHECK_OFFSET(tBENetworkDB, unk_326, 0x326);
CHECK_OFFSET(tBENetworkDB, MyCharacterID, 0x328);
CHECK_OFFSET(tBENetworkDB, OpponentCharacterID, 0x32C);
CHECK_OFFSET(tBENetworkDB, MyStats, 0x330);
CHECK_OFFSET(tBENetworkDB, OpponentStats, 0x3DC);
CHECK_OFFSET(tBENetworkDB, unk_488, 0x488);
CHECK_OFFSET(tBENetworkDB, unk_498, 0x498);
CHECK_OFFSET(tBENetworkDB, oppCharacterAttributes, 0x4A8);
CHECK_OFFSET(tBENetworkDB, unk_4AF, 0x4AF);
CHECK_SIZE(tBENetworkDB, 0x4B0);

CHECK_OFFSET(cBENetworkInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBENetworkInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBENetworkInterface, bIsActive, 0x08);
CHECK_OFFSET(cBENetworkInterface, mVtbl, 0x0C);
CHECK_SIZE(cBENetworkInterface, 0x10);

// The one network interface (On Tour: cBENetworkInterface::instance), created by cBENetworkInterface_getThis,
// and its static network options (cBENetworkInterface::networkOpt).
extern cBENetworkInterface* cBENetworkInterface_instance;
extern tBENetworkDB cBENetworkInterface_networkOpt;

#endif /* BE_BEINTNETWORK_H */

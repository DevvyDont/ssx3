#ifndef BE_BEINTNETWORK_H
#define BE_BEINTNETWORK_H

// cBENetworkInterface: the back end's network-game interface. Its object carries no network data of its own: like
// every BE interface it is a cBEInterface (three bools + vptr, 0x10 bytes), and its accessors read and write the
// static tBENetworkDB networkOpt below (network game / ranked / host flags, port, latency, names, the two riders'
// character IDs and stats). One instance, created on demand by cBENetworkInterface_getThis
// (`cMemMan_alloc(0x10, "BENetworkInt", 0, 0)`, bIsActive = 0, vtable cBENetworkInterface_vtbl) into cBENetworkInterface::instance
// (retail cBENetworkInterface_instance); the rest of the game reaches it as cBE_getInterface(SSXApp->BE, 7). EA's source:
// d:/ssx/code/source/be/beintnetwork.cpp (the preview build's assert string).
//
// Size 0x10 [R] (the allocation above; the preview's getThis allocates 0x10 too). Base cBEInterface, written out
// inline exactly as in be/beintreward.h (whose virtual slots (be/beinterface.h) applies here: slot 1 submit = cBENetworkInterface_submit
// copies networkOpt into cBELibrary::networkDB, slot 2 discard = cBENetworkInterface_discard copies it back, slots 3/4 are
// cBEInterface's replay-frame hooks).
//
// Statics (retail addresses; names from On Tour's symbol table):
//   cBENetworkInterface::instance       cBENetworkInterface_instance
//   cBENetworkInterface::networkOpt     cBENetworkInterface_networkOpt  tBENetworkDB (0x4B0; read by ~20 units; size:0x4B0 in
//                                                   config/symbol_addrs.txt, so splat writes its fields as
//                                                   networkOpt + off, no longer as D_00534B34 ... D_00534FD8)
//   cBELibrary::networkDB               cBELibrary_networkDB  tBENetworkDB (submit/discard's other side; On Tour has both)
//   cBENetworkInterface::BackupPlayerDB cBENetworkInterface_BackupPlayerDB  6 player records of 0x1C (backupCharProfile cBENetworkInterface_backupCharProfile /
//                                                   restoreCharProfile cBENetworkInterface_restoreCharProfile; On Tour: cBEPlayerDB[4])
//
// Evidence tags:
//   [R] retail's own code: getThis (allocation, vptr, bIsActive), the constructor cBENetworkInterface_cBENetworkInterface (vptr), and the
//       out-of-line accessors cBENetworkInterface_purgeInterface..cBENetworkInterface_getOverallOpponentLevel (be/bedbreward), each of which reads or writes one
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
#include "be/beinterface.h"
#include "ai/ridermetrix.h"             // tRiderStats (0xAC)

// tBENetworkDB: the network-game options (cBENetworkInterface::networkOpt / cBELibrary::networkDB), 0x4B0 bytes
// ([R] submit/discard copy 0x4B0 bytes).
struct tBENetworkDB {
    int bIsNetworkGame;              // 0x000 [O]; [P] get/setIsNetworkGame; [R] cBENetworkInterface_setIsNetworkGame / cBENetworkInterface_getIsNetworkGame;
                                     //       [U] tested by ~20 units ("is this an online race")
    int bIsRankedGame;               // 0x004 [O]; [P] get/setIsRankedGame; [R] cBENetworkInterface_setIsRankedGame / cBENetworkInterface_getIsRankedGame
    int bIsHost;                     // 0x008 [O]; [P] get/setIsHost; [R] cBENetworkInterface_setIsHost / cBENetworkInterface_getIsHost (D_00534B38);
                                     //       [U] cGameComm_syncGame / syncWait / gameExit, setSyncData (cBENetworkInterface_setSyncData),
                                     //       preInitBE (cBENetworkInterface_preInitBE) and the results report pick the host side by it
    int iPort;                       // 0x00C [O]; [P] get/setPort; [R] cBENetworkInterface_setPort / cBENetworkInterface_getPort (D_00534B3C);
                                     //       [U] debugfe (connectDebugDirectLink, cDebugLobbyScreen_startOnlineRace) and tagsunk cGameConnMan_connect read
                                     //       it as an unsigned short (lhu) for openDSockChannel
    float fLatency;                  // 0x010 [O]; [P] get/setLatency (float); [R] cBENetworkInterface_setLatency / cBENetworkInterface_getLatency
                                     //       (D_00534B40); [U] cGameComm_syncGame stores cGameComm::mLatency, cGame_load
                                     //       sizes cInputSync from it
    char strLocalAddr[256];          // 0x014 [O] (On Tour 0x00C, same size); the 0x200 bytes between fLatency and
    char strRemoteAddr[256];         // 0x114 [O]  iClientID (both builds); no retail access except whole-struct copies
    int iClientID;                   // 0x214 [O]; [P] get/setClientID (preview +0x214); [R] cBENetworkInterface_setClientID / cBENetworkInterface_getClientID
                                     //       (D_00534D44)
    char strHostName[120];           // 0x218 [O] (size 120); [P] get/setHostName; [R] cBENetworkInterface_setHostName (strcpy) /
                                     //       cBENetworkInterface_getHostName (D_00534D48)
    char strGameName[120];           // 0x290 [O]; [P] get/setGameName; [R] cBENetworkInterface_setGameName / cBENetworkInterface_getGameName (D_00534DC0)
    char strPassword[30];            // 0x308 [O] (size 30); [P] get/setPassword; [R] cBENetworkInterface_setPassword / cBENetworkInterface_getPassword
                                     //       (D_00534E38)
    char unk_326[0x2];               // 0x326 padding
    int MyCharacterID;               // 0x328 [P] get/setMyCharacterID (field name from the accessor; not in On Tour);
                                     //       [R] cBENetworkInterface_setMyCharacterID / cBENetworkInterface_getMyCharacterID (D_00534E58); "CHARACTER0" in the report
    int OpponentCharacterID;         // 0x32C [P] get/setOpponentCharacterID; [R] cBENetworkInterface_setOpponentCharacterID / cBENetworkInterface_getOpponentCharacterID (D_00534E5C)
    tRiderStats MyStats;             // 0x330 [P] get/setMyStats(tRiderStats const&); [R] setter cBENetworkInterface_setMyStats copies 0xAC
                                     //       bytes, getter cBENetworkInterface_getMyStats (D_00534E60); tRiderStats is 0xAC
                                     //       (ai/ridermetrix.h; views group D: sBEBlk_C418 is this record)
    tRiderStats OpponentStats;       // 0x3DC [P] get/setOpponentStats; [R] cBENetworkInterface_setOpponentStats / cBENetworkInterface_getOpponentStats (D_00534F0C)
    char strMyName[0x10];            // 0x488 (name ours, after MyStats) [R] cBENetworkInterface_setMyName (strcpy) / cBENetworkInterface_getMyName
                                     //       (D_00534FB8); [U] this player's name (from the lobby session +0x80,
                                     //       func_00259098); prefixes our own chat lines (ovstatetrophy cOVState_PRIVATEMESSAGE_addChatLine)
    char strOpponentName[0x10];      // 0x498 (name ours, after OpponentStats) [R] cBENetworkInterface_setOpponentName / cBENetworkInterface_getOpponentName
                                     //       (D_00534FC8); [U] the opponent's name (lobby session +0x98); prefixes
                                     //       received chat (cOVState_onUpdate_HandleIncomingChat), shown by
                                     //       cOVState_PRIVATEMESSAGE_onCreateScreen
    unsigned char oppCharacterAttributes[7]; // 0x4A8 [O] (On Tour 0x326); [R] setOpponentAttributes cBENetworkInterface_setOpponentAttributes
                                     //       copies 7 bytes, getOpponentAttributes cBENetworkInterface_getOpponentAttributes, getOverallOpponentLevel
                                     //       cBENetworkInterface_getOverallOpponentLevel sums them / 35 (D_00534FD8)
    char unk_4AF[0x1];               // 0x4AF padding
};

struct tRiderStats;

struct cBENetworkInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [U] setNetworkActive (cBENetworkInterface_setNetworkActive, static:
    //       instance->bIsActive = arg); set 0/1 with every BE interface
    //       (cBEPostRaceState_onExit/cBEPostRaceState_onEnter/cBEPreRaceState_onExit/cBEPreRaceState_onEnter)
    //   the vptr (0x0C): [R] cBENetworkInterface_vtbl (getThis, ctor cBENetworkInterface_cBENetworkInterface); [P] the
    //       preview ctor __19cBENetworkInterface stores _vt$19cBENetworkInterface at 0xC; [U] func_00259098 calls slot
    //       1 (submit) through it

    // Methods bound to their retail symbols by asm labels.
    void setIsNetworkGame(int isNetworkGame) __asm__("cBENetworkInterface_setIsNetworkGame");
    void setIsRankedGame(int isRanked) __asm__("cBENetworkInterface_setIsRankedGame");
    void setIsHost(int isHost) __asm__("cBENetworkInterface_setIsHost");
    void setPort(int port) __asm__("cBENetworkInterface_setPort");
    void setClientID(int clientID) __asm__("cBENetworkInterface_setClientID");
    void setHostName(const char* name) __asm__("cBENetworkInterface_setHostName");
    void setGameName(const char* name) __asm__("cBENetworkInterface_setGameName");
    void setPassword(const char* name) __asm__("cBENetworkInterface_setPassword");
    void setMyCharacterID(int characterID) __asm__("cBENetworkInterface_setMyCharacterID");
    void setOpponentCharacterID(int characterID) __asm__("cBENetworkInterface_setOpponentCharacterID");
    void setMyStats(tRiderStats* src) __asm__("cBENetworkInterface_setMyStats");
    void setOpponentStats(tRiderStats* src) __asm__("cBENetworkInterface_setOpponentStats");
    void copyProfile(int srcProfile, int destProfile) __asm__("cBENetworkInterface_copyProfile");
    void preInitBE() __asm__("cBENetworkInterface_preInitBE");
    void postInitBE() __asm__("cBENetworkInterface_postInitBE");
    void backupCharProfile(int idx) __asm__("cBENetworkInterface_backupCharProfile");
    void restoreCharProfile(int idx) __asm__("cBENetworkInterface_restoreCharProfile");
    int getSyncDataSize() __asm__("cBENetworkInterface_getSyncDataSize__FPv");
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
CHECK_OFFSET(tBENetworkDB, strMyName, 0x488);
CHECK_OFFSET(tBENetworkDB, strOpponentName, 0x498);
CHECK_OFFSET(tBENetworkDB, oppCharacterAttributes, 0x4A8);
CHECK_OFFSET(tBENetworkDB, unk_4AF, 0x4AF);
CHECK_SIZE(tBENetworkDB, 0x4B0);

CHECK_SIZE(cBENetworkInterface, 0x10);

// The one network interface (On Tour: cBENetworkInterface::instance), created by cBENetworkInterface_getThis,
// and its static network options (cBENetworkInterface::networkOpt).
extern cBENetworkInterface* cBENetworkInterface_instance;
extern tBENetworkDB cBENetworkInterface_networkOpt;

#endif /* BE_BEINTNETWORK_H */

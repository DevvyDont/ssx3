#ifndef WSCRIPT_WSCRIPTMAN_H
#define WSCRIPT_WSCRIPTMAN_H

// cWScriptMan: the world script manager (one instance). cGame_load allocates it as
// `cWScriptMan_cWScriptMan(operator new(1176, "WScriptMan"))` and stores it in cGame::WScriptMan; the constructor also
// stores itself in the class static cWScriptMan::TheWScriptMan (cWScriptMan_TheWScriptMan). It runs the world's Luno
// scripts: global scripts per section, mission and process scripts (cWScriptProcess / cWScriptTask), the per-instance
// collide / focus / expire scripts, and the mission HUD state (points, count, clock, height, trick).
//
// Size 0x498 [R] (the allocation; On Tour 0x548). A FLAT layout, like main/game.h. Member names are EA's, from SSX On
// Tour's PSP DWARF: its cWScriptMan derives from cWScriptManState (0x000-0x28B here; 0x310 bytes in On Tour) and adds
// the members from World on. Placed by:
//   [R] retail's constructor cWScriptMan_cWScriptMan (wscript/wscriptprocess) and cWScriptMan_init;
//   [U] how retail code uses the field;
//   [O] On Tour's member order, where the fields on both sides are placed by [R] or [U].
// Retail differs from On Tour in three places: its trick definition is 8 bytes (On Tour's tTrickNameDef is 0xC), its
// events are 12 bytes (tWScriptEventList 0x184, On Tour 0x204), and it has no LastMedalWon (On Tour 0x328), so every
// member from CurrentProcess on sits 0x88 before its On Tour offset (World..LocalMissionId: 0x84). The vtable pointer
// is g++ 2.95's, after the members.

#include "layout.h"
#include "wscript/missionmenuqueue.h"

struct cWorld;
struct tInstanceRecord;
struct tCollideInfo;
struct tIntersectInfo;
struct tTrickNameDef;
struct cWScriptProcess;
struct cWScriptTask;
struct cLunoTable;

// A process list: the head of a singly linked cWScriptProcess list (On Tour cProcessList, 4 bytes).
struct cProcessList {
    cWScriptProcess* List;  // 0x0 [O]
};

struct cWScriptMan {
    // cWScriptManState
    int Being;                         // 0x000 [O]; [R] init = 0
    int MissionPointsEnable;           // 0x004 [O] (bool there); [R] init = 0; [U] isPointsEnabled
    int MissionPoints;                 // 0x008 [O]; [U] getNumPointsCurr, updateMissionPoints
    int MissionPointsGoal;             // 0x00C [O]; [U] getNumPointsGoal
    int MissionCountEnable;            // 0x010 [O] (bool there); [R] init = 0; [U] isCountEnabled
    int MissionCount;                  // 0x014 [O]; [U] getCountCurr
    int MissionCountMax;               // 0x018 [O]; [U] getCountMax
    int MissionTicksEnable;            // 0x01C [O] (bool there); [U] isClockEnabled
    int MissionTicks;                  // 0x020 [O]; [U] getClockTicksCurr
    int MissionTicksDir;               // 0x024 [O]; [U] getClockDir
    int MissionHeightEnable;           // 0x028 [O] (bool there); [U] getHeightCurr returns 0 while clear
    unsigned int MissionHeightInstId;  // 0x02C [O] (tResourceID); [R] ctor = -1; [U] getHeightCurr's instance
    int MissionHeightBeing;            // 0x030 [O]; [U] getHeightCurr's rider index
    int MissionTrickEnable;            // 0x034 [O] (bool there); [U] isTrickEnabled, updateMissionTrick
    unsigned int MissionTrick[2];      // 0x038 [O] tTrickNameDef (8 bytes in retail); [U] updateMissionTrick
    char Events[0x184];                // 0x040 [O] tWScriptEventList: NumEvents + 32 events of 12 bytes; [U] eventJump,
                                       //       eventLand and eventCollide push onto it
    tMissionMenuQueue MissionMenuQueue; // 0x1C4 [O] tMissionMenuQueue (0xC8 on both); [R] tMissionMenuQueue_tMissionMenuQueue
    // cWScriptMan
    cWorld* World;                     // 0x28C [O]; [R] ctor = cWorld_m_pTheWorld
    tInstanceRecord* Inst;             // 0x290 [O]; [U] the doOn*Script instance
    tCollideInfo* CollideInfo;         // 0x294 [O]; [U] doOnCollideScript, updateWorld
    tIntersectInfo* IntersectInfo;     // 0x298 [O]; [U] doOnCollideScript, updateWorld
    tTrickNameDef* TrickDef;           // 0x29C [O]; [U] getCurrentTrick
    unsigned int LocalMissionId;       // 0x2A0 [O]; [U] cancelLocalMission, failActiveLocalMission
    cWScriptProcess* CurrentProcess;   // 0x2A4 [O]; [U] advanceToTask, doProcessCmd, the doOn*Script calls
    cWScriptTask* CurrentTask;         // 0x2A8 [O]; [U] runScriptCode1, readFromReplayFrame
    int LocalMissionCmd;               // 0x2AC [O]; [U] doMissionCmd, cancelLocalMission
    unsigned int LocalMissionCmdId;    // 0x2B0 [O]; [U] doMissionCmd, cancelLocalMission
    cProcessList IdleList;             // 0x2B4 [O]; [R] cProcessList_cProcessList; [U] addProcess takes from it
    cProcessList ActiveList;           // 0x2B8 [O]; [R] cProcessList_cProcessList
    cProcessList AddList;              // 0x2BC [O]; [R] cProcessList_cProcessList; [U] addProcess appends to it
    char CollectPools[1][0x10C];       // 0x2C0 [O] cCollectPool[1]; [R] the ctor's one-pass loop, cCollectPool_init
    cLunoTable* GlobalTables[50];      // 0x3CC [O] (60 in On Tour); [R] ctor MEM_fill(+0x3CC, 0, 200); [U] isFunc,
                                       //       purgeGlobalTables
    void* mVtbl;                       // 0x494 [R] ctor = cWScriptMan_vtbl
};

CHECK_OFFSET(cWScriptMan, MissionHeightInstId, 0x02C);
CHECK_OFFSET(cWScriptMan, MissionTrick, 0x038);
CHECK_OFFSET(cWScriptMan, Events, 0x040);
CHECK_OFFSET(cWScriptMan, MissionMenuQueue, 0x1C4);
CHECK_OFFSET(cWScriptMan, World, 0x28C);
CHECK_OFFSET(cWScriptMan, LocalMissionId, 0x2A0);
CHECK_OFFSET(cWScriptMan, CurrentProcess, 0x2A4);
CHECK_OFFSET(cWScriptMan, IdleList, 0x2B4);
CHECK_OFFSET(cWScriptMan, CollectPools, 0x2C0);
CHECK_OFFSET(cWScriptMan, GlobalTables, 0x3CC);
CHECK_OFFSET(cWScriptMan, mVtbl, 0x494);
CHECK_SIZE(cWScriptMan, 0x498);

#endif

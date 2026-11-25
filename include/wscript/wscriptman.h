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

// A collect pool: the instances a mission collects (On Tour class cCollectPool : cCollectPoolState, 0x10C; no vptr).
// FLAT: cCollectPoolState's members. [R] cWScriptMan_cWScriptMan sets the 64 ids to -1 per 0x10C pool and runs
// cCollectPool_init (PoolId -1, Size / NumInstIds 0, ids -1). The unit's views cWScriptIdTable,
// cWScriptMan_collectPool_sSlot, sWSSlot_B9A0, cCollectPool_collect_sTable, sEntry_C4A8 (src/wscript/wscriptman.cpp)
// are this type.
struct cCollectPool {
    unsigned int PoolId;            // 0x000 [O]; [U] cWScriptMan_collectPool stores the id (-1 = unused);
                                    //       cCollectPool_collect / collectCount pass it to the BE mission interface
    int Size;                       // 0x004 [O]; [U] cWScriptMan_collectPool stores its size argument
    int NumInstIds;                 // 0x008 [O]; [U] cCollectPool_collect's loop bound; collectInst appends at it
    unsigned int InstIds[64];       // 0x00C [O] tResourceID[64]; [U] collect compares, collectInst appends

    // Methods bound to their retail symbols by asm labels.
    void init() __asm__("cCollectPool_init");
    void collect(int id) __asm__("cCollectPool_collect");
};

CHECK_OFFSET(cCollectPool, PoolId, 0x000);
CHECK_OFFSET(cCollectPool, Size, 0x004);
CHECK_OFFSET(cCollectPool, NumInstIds, 0x008);
CHECK_OFFSET(cCollectPool, InstIds, 0x00C);
CHECK_SIZE(cCollectPool, 0x10C);

// A process list: the head of a singly linked cWScriptProcess list (On Tour cProcessList, 4 bytes).
struct cProcessList {
    cWScriptProcess* List;  // 0x0 [O]

    // Methods bound to their retail symbols by asm labels.
    void deleteAll() __asm__("cProcessList_deleteAll");
    void* removeHead() __asm__("cProcessList_removeHead");
    int getCount() __asm__("cProcessList_getCount");
    void* findProcess1(int id) __asm__("cProcessList_findProcess1");
    void findMission1(int name) __asm__("cProcessList_findMission1");
};

// One queued world-script event (On Tour struct tWScriptEvent : tWScriptEventBase {EventId}, then an anonymous
// union; retail 0xC, On Tour 0x10) and the 32-entry event list (On Tour tWScriptEventList; retail 4 + 32 x 0xC =
// 0x184, cWScriptMan::Events). [U] cWScriptMan_update copies the list and walks it (views sCmdList_309270,
// sCmd_309270: flags 0x0, the instance id 0x4 it looks up, a word 0x8). The union's member names are not in the
// DWARF dump, so its words stay unk_.
struct tWScriptEvent {
    unsigned int EventId;           // 0x0 [O] tWScriptEventBase::EventId; [U] masked and tested by update
    unsigned int unk_04;            // 0x4 [O] union; [U] the instance resource id update looks up
    int unk_08;                     // 0x8 [O] union
};

CHECK_OFFSET(tWScriptEvent, EventId, 0x0);
CHECK_OFFSET(tWScriptEvent, unk_04, 0x4);
CHECK_OFFSET(tWScriptEvent, unk_08, 0x8);
CHECK_SIZE(tWScriptEvent, 0xC);

struct tWScriptEventList {
    int NumEvents;                  // 0x000 [O]; [U] update's loop bound
    tWScriptEvent Events[32];       // 0x004 [O]
};

CHECK_OFFSET(tWScriptEventList, NumEvents, 0x000);
CHECK_OFFSET(tWScriptEventList, Events, 0x004);
CHECK_SIZE(tWScriptEventList, 0x184);

struct cBeing;
struct cBxReplayFrame;
struct nash_Node;

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
    cCollectPool CollectPools[1];      // 0x2C0 [O]; [R] the ctor's one-pass loop, cCollectPool_init
    cLunoTable* GlobalTables[50];      // 0x3CC [O] (60 in On Tour); [R] ctor MEM_fill(+0x3CC, 0, 200); [U] isFunc,
                                       //       purgeGlobalTables
    void* mVtbl;                       // 0x494 [R] ctor = cWScriptMan_vtbl

    // Methods bound to their retail symbols by asm labels.
    void* addProcess(int sectionId, int data, int front) __asm__("cWScriptMan_addProcess");
    void* addProcess1(int sectionId, int data, int key, int front) __asm__("cWScriptMan_addProcess1");
    void remProcess(int id) __asm__("cWScriptMan_remProcess");
    void removeProcess(cWScriptProcess* node) __asm__("cWScriptMan_removeProcess");
    void startProcess(cWScriptProcess* obj) __asm__("cWScriptMan_startProcess");
    void stopProcess(cWScriptProcess* obj) __asm__("cWScriptMan_stopProcess");
    void eventCollide(int value) __asm__("cWScriptMan_eventCollide");
    void eventJump() __asm__("cWScriptMan_eventJump");
    void eventLand() __asm__("cWScriptMan_eventLand");
    void eventWipeout() __asm__("cWScriptMan_eventWipeout");
    void eventReset() __asm__("cWScriptMan_eventReset");
    int getScriptCode(int id) __asm__("cWScriptMan_getScriptCode");
    float getHeightCurr() __asm__("cWScriptMan_getHeightCurr");
    void runScript(int* handle) __asm__("cWScriptMan_runScript");
    void runScript1(int* handle, int id) __asm__("cWScriptMan_runScript1");
    int isFunc(int idx, int value) __asm__("cWScriptMan_isFunc");
    void runFunc(int idx, int value) __asm__("cWScriptMan_runFunc");
    void runGlobalScripts() __asm__("cWScriptMan_runGlobalScripts");
    void doOnCollideScript(tCollideInfo* collideInfo, tIntersectInfo* intersectInfo, cBeing* obj)
        __asm__("cWScriptMan_doOnCollideScript");
    int hasOnFocusScript(int id) __asm__("cWScriptMan_hasOnFocusScript");
    void doOnFocusScript(int id) __asm__("cWScriptMan_doOnFocusScript");
    int hasOnLoseFocusScript(int id) __asm__("cWScriptMan_hasOnLoseFocusScript");
    int isFocusInstance(nash_Node* obj) __asm__("cWScriptMan_isFocusInstance");
    void upFocusScript(tInstanceRecord* obj) __asm__("cWScriptMan_upFocusScript");
    void downFocusScript(tInstanceRecord* obj) __asm__("cWScriptMan_downFocusScript");
    void doOnLoseFocusScript(int id) __asm__("cWScriptMan_doOnLoseFocusScript");
    int hasOnExpireScript(int id) __asm__("cWScriptMan_hasOnExpireScript");
    void doOnExpireScript(int id) __asm__("cWScriptMan_doOnExpireScript");
    void doOnInitScript(int id) __asm__("cWScriptMan_doOnInitScript");
    int hasOnFrameUpdateScript(int scriptId) __asm__("cWScriptMan_hasOnFrameUpdateScript");
    void doOnFrameUpdateScript(int id) __asm__("cWScriptMan_doOnFrameUpdateScript");
    void gotoTask() __asm__("cWScriptMan_gotoTask");
    void advanceToTask() __asm__("cWScriptMan_advanceToTask");
    void doProcessCmd(unsigned int id, int kind) __asm__("cWScriptMan_doProcessCmd");
    int doMissionCmd(unsigned int id, unsigned int cmd, int ctx) __asm__("cWScriptMan_doMissionCmd");
    void runScriptCode(void* obj, int data) __asm__("cWScriptMan_runScriptCode");
    void runScriptCode1(cWScriptTask* obj, int scriptCode) __asm__("cWScriptMan_runScriptCode1");
    void* getCurrentMission() __asm__("cWScriptMan_getCurrentMission");
    int getCurrentTrick(int beingIndex, int which) __asm__("cWScriptMan_getCurrentTrick");
    void findMission(void* node) __asm__("cWScriptMan_findMission");
    void updateMissionPoints(int points, int goal) __asm__("cWScriptMan_updateMissionPoints__FPvii");
    void updateMissionCount(int count, int maxCount) __asm__("cWScriptMan_updateMissionCount__FPvii");
    void updateMissionTicks(int ticks, int dir) __asm__("cWScriptMan_updateMissionTicks__FPvii");
    void updateMissionHeight(int being, void* src) __asm__("cWScriptMan_updateMissionHeight__FPviT0");
    void startLocalMission() __asm__("cWScriptMan_startLocalMission");
    int previewLocalMission() __asm__("cWScriptMan_previewLocalMission");
    void cancelPreviewLocalMission() __asm__("cWScriptMan_cancelPreviewLocalMission");
    void cancelLocalMission() __asm__("cWScriptMan_cancelLocalMission");
    void restartLocalMission() __asm__("cWScriptMan_restartLocalMission");
    void quitLocalMission() __asm__("cWScriptMan_quitLocalMission");
    void failActiveLocalMission() __asm__("cWScriptMan_failActiveLocalMission");
    int isProcessActive(void* process) __asm__("cWScriptMan_isProcessActive__FPvT0");
    int isLocalMissionActive() __asm__("cWScriptMan_isLocalMissionActive");
    void collectPool(unsigned int id, int size) __asm__("cWScriptMan_collectPool");
    void collect(int rider, int id, int instId) __asm__("cWScriptMan_collect");
    int collectCount(int id) __asm__("cWScriptMan_collectCount");
    void collectInst(int id, int instIds, int size) __asm__("cWScriptMan_collectInst");
    void writeReplayProcessList(void** list, cBxReplayFrame* stream) __asm__("cWScriptMan_writeReplayProcessList");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cWScriptMan_readFromReplayFrame");
    void readReplayProcessList(cProcessList* list, cBxReplayFrame* stream) __asm__("cWScriptMan_readReplayProcessList");
    void setTrickDef(tTrickNameDef* trickDef) __asm__("cWScriptMan_setTrickDef");
    int isPointsEnabled() __asm__("cWScriptMan_isPointsEnabled");
    int getNumPointsCurr() __asm__("cWScriptMan_getNumPointsCurr");
    int getNumPointsGoal() __asm__("cWScriptMan_getNumPointsGoal");
    int isCountEnabled() __asm__("cWScriptMan_isCountEnabled");
    int getCountCurr() __asm__("cWScriptMan_getCountCurr");
    int getCountMax() __asm__("cWScriptMan_getCountMax");
    int isClockEnabled() __asm__("cWScriptMan_isClockEnabled");
    int getClockTicksCurr() __asm__("cWScriptMan_getClockTicksCurr");
    int getClockDir() __asm__("cWScriptMan_getClockDir");
    int isHeightEnabled() __asm__("cWScriptMan_isHeightEnabled");
    int isTrickEnabled() __asm__("cWScriptMan_isTrickEnabled");
    void setLocalMissionId(int missionId) __asm__("cWScriptMan_setLocalMissionId");
    int getLocalMissionId() __asm__("cWScriptMan_getLocalMissionId");
    int getInst() __asm__("cWScriptMan_getInst");
    int getCollideInfo() __asm__("cWScriptMan_getCollideInfo");
    int getIntersectInfo() __asm__("cWScriptMan_getIntersectInfo");
    void setInst(tInstanceRecord* inst) __asm__("cWScriptMan_setInst");
    cLunoTable* getGlobalTable(int sectionId) __asm__("cWScriptMan_getGlobalTable");
    int getCurrentTask() __asm__("cWScriptMan_getCurrentTask");
    void setCurrentTask(cWScriptTask* task) __asm__("cWScriptMan_setCurrentTask");
    int getCurrentProcess() __asm__("cWScriptMan_getCurrentProcess");
    int isMenuQueueReady() __asm__("cWScriptMan_isMenuQueueReady");
    void popMenuQueueEntry() __asm__("cWScriptMan_popMenuQueueEntry");
    void getMenuQueueEntry() __asm__("cWScriptMan_getMenuQueueEntry");
    void init() __asm__("cWScriptMan_init");
    void purgeGlobalTables() __asm__("cWScriptMan_purgeGlobalTables");
    void rebuildGlobalTables() __asm__("cWScriptMan_rebuildGlobalTables");
    void activeSection(int section) __asm__("cWScriptMan_activeSection");
    void setupMissions() __asm__("cWScriptMan_setupMissions");
    void updateWorld() __asm__("cWScriptMan_updateWorld");
    void purgeMissions() __asm__("cWScriptMan_purgeMissions");
    void purgeSection(int section) __asm__("cWScriptMan_purgeSection");
    void unloadSection(int i) __asm__("cWScriptMan_unloadSection");
    void initMissions() __asm__("cWScriptMan_initMissions");
    void processLocalMissionCmd() __asm__("cWScriptMan_processLocalMissionCmd");
    void update() __asm__("cWScriptMan_update");
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

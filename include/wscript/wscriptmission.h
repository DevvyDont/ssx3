#ifndef WSCRIPT_WSCRIPTMISSION_H
#define WSCRIPT_WSCRIPTMISSION_H

// cWScriptMission: a world-script process that is a mission (challenge): a cWScriptProcess plus the mission's scope,
// sort key and four mission scripts. cWScriptMan_addProcess1 creates one per mission definition of a world section;
// processStart flags the mission start on the rider, missionPass pays the reward and unlocks the next mission (BE
// mission interface), missionFail makes it available again, missionRestart ends and restarts it.
// EA's source: d:/ssx/code/source/wscript/wscriptmission.cpp (builds/x/v2 file_hints: preview assert lines of
// func_003071F0 / func_00307308). Retail methods: the unnamed splat unit seg/seg_1FBE38 (0x00307128 ctor ..
// 0x00307568 missionUpdateWorld) and three one-line accessors in wscript/wscriptman (0x0030D478 getSortKey,
// 0x0030D480 isMission, 0x0030D488 isScopeLocal).
//
// Size 0x78: [R] cWScriptMan_addProcess1 allocates cMemMan_alloc(0x78, "WScriptMission"); the ctor writes up to
// MissionScriptCode[3] (0x74). On Tour: 0x7C (it adds eBEMedalID MedalLevel at 0x78, which retail does not have:
// no retail method reads past 0x74 and the allocation ends there).
// Base cWScriptProcess (wscript/wscriptprocess.h, 0x60; vptr at 0x5C) at 0x00, written out inline under that header's
// names. The ctor runs cWScriptProcess_cWScriptProcess(self, manager, sectionId, data), then stores cWScriptMission_vtbl
// (0x00489A30, 20 slots: 1 dtor, 2 processComplete, 3 isMission, 4 processStart, 5/6 cWScriptProcess_processUpdate /
// processEnd, 7-16 the cWScriptProcess_eventOn* handlers, 17 missionRestart, 18 missionPass, 19 missionFail,
// 20 missionUpdateWorld).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cWScriptMission_cWScriptMission and dtor cWScriptMission__cWScriptMission (plus the base's,
//       documented in wscript/wscriptprocess.h);
//   [P] tools/layout_xlate.py: 6 paired methods show 0x0, 0x10, 0x20, 0x5C, 0x70, 0x74 unchanged; the preview's
//       inline getSortKey reads 0x64 (bracketed 0x5C..0x70);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cWScriptMission: Scope 0x60, SortKey 0x64, MissionScriptCode[4] 0x68, MedalLevel 0x78;
//       cWScriptProcess / cWScriptProcessState).

#include "layout.h"

struct cWScriptMan;
struct cWScriptTask;
struct cWScriptProcess;
struct cLunoTable;

struct cWScriptMission {
    // --- cWScriptProcess (wscript/wscriptprocess.h, 0x60), whose first part is cWScriptProcessState (0x00-0x0F)
    int State;                         // 0x00 [O]; [R] base ctor 0; [U] missionRestart / missionFail test 2 (running)
    int NumTasks;                      // 0x04 [O]; [R] base ctor
    int RunningTask;                   // 0x08 [O]; [R] base ctor
    unsigned int StateSectionId;       // 0x0C [O] cWScriptProcessState::SectionId (renamed as in wscriptprocess.h)
    cWScriptMan* Manager;              // 0x10 [O]; [R] base ctor; [U] the ctor resolves the scripts through it,
                                       //      processStart / missionPass index the game's riders by its first word
                                       //      (Being), missionPass / missionFail / missionUpdateWorld pass it to
                                       //      cWScriptMan_runScriptCode, processComplete to cWScriptMan_isProcessActive
    cWScriptTask* Tasks;               // 0x14 [O]; [R] base ctor
    cWScriptProcess* Next;             // 0x18 [O]; [R] base ctor 0
    cLunoTable* Local;                 // 0x1C [O]; [R] base ctor 0
    unsigned int ProcessId;            // 0x20 [O]; [R] base ctor; [U] missionPass / missionFail use it as the BE
                                       //      mission ID (cBEMissionInterface_*)
    unsigned int SectionId;            // 0x24 [O]; [R] base ctor
    char* ProcessScriptCode[3];        // 0x28 [O] (const char* there); [R] base ctor
    char* EventScriptCode[10];         // 0x34 [O] (const char* there); [R] base ctor
    void** mVtbl;                      // 0x5C [R] cWScriptMission_vtbl (ctor and dtor); [U] processComplete calls slots
                                       //      6 and 18, missionRestart 6 and 4, missionFail 20 through it
    // --- cWScriptMission
    int Scope;                         // 0x60 [O]; [R] ctor = the mission data's word +0x50; [U] isScopeLocal (== 2)
    int SortKey;                       // 0x64 [O]; [P] getSortKey (lw 0x64); [R] ctor = key; [U]
                                       //      cWScriptMission_getSortKey
    char* MissionScriptCode[4];        // 0x68 [O] (const char* there); [R] ctor: cWScriptMan_getScriptCode(Manager,
                                       //      data's 4 script IDs at +0x40); [U] [1] (0x6C) missionPass, [2] (0x70)
                                       //      missionFail, [3] (0x74) missionUpdateWorld; [0] is never read in retail
};

CHECK_OFFSET(cWScriptMission, State, 0x00);
CHECK_OFFSET(cWScriptMission, NumTasks, 0x04);
CHECK_OFFSET(cWScriptMission, RunningTask, 0x08);
CHECK_OFFSET(cWScriptMission, StateSectionId, 0x0C);
CHECK_OFFSET(cWScriptMission, Manager, 0x10);
CHECK_OFFSET(cWScriptMission, Tasks, 0x14);
CHECK_OFFSET(cWScriptMission, Next, 0x18);
CHECK_OFFSET(cWScriptMission, Local, 0x1C);
CHECK_OFFSET(cWScriptMission, ProcessId, 0x20);
CHECK_OFFSET(cWScriptMission, SectionId, 0x24);
CHECK_OFFSET(cWScriptMission, ProcessScriptCode, 0x28);
CHECK_OFFSET(cWScriptMission, EventScriptCode, 0x34);
CHECK_OFFSET(cWScriptMission, mVtbl, 0x5C);
CHECK_OFFSET(cWScriptMission, Scope, 0x60);
CHECK_OFFSET(cWScriptMission, SortKey, 0x64);
CHECK_OFFSET(cWScriptMission, MissionScriptCode, 0x68);
CHECK_SIZE(cWScriptMission, 0x78);

#endif /* WSCRIPT_WSCRIPTMISSION_H */

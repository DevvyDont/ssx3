#ifndef WSCRIPT_WSCRIPTPROCESS_H
#define WSCRIPT_WSCRIPTPROCESS_H

// cWScriptProcess / cWScriptTask: a world-script process and its tasks. cWScriptMan_addProcess allocates a process
// (cMemMan_alloc(0x60, "WScriptProcess")) from a world section's process definition; the constructor resolves its
// 3 process scripts (start, update, end) and 10 event scripts and builds its "WScriptTasks" array (0x44 each), whose
// tasks have the same script slots. The manager runs one task of a process at a time (RunningTask) and dispatches
// the rider events (collide, jump, land, trick, wipeout, reset, collect, three custom events) to the process and to
// its running task. Methods in wscript/wscriptprocess and wscript/wscriptman.
//
// FLAT layouts, like main/game.h. Member names are EA's, from SSX On Tour's PSP DWARF: cWScriptProcess derives from
// cWScriptProcessState (0x00-0x0F) and cWScriptTask from cWScriptTaskState (0x00). Placed by:
//   [R] the retail constructors cWScriptProcess_cWScriptProcess / cWScriptTask_cWScriptTask and initTask;
//   [U] how retail code uses the field (every access through `this` in the methods falls on a member start);
//   [O] On Tour's member order; sizes 0x60 and 0x44 on both sides.
// The vtable pointers are g++ 2.95's, after the members.

#include "layout.h"

struct cWScriptMan;
struct cWScriptProcess;
struct cLunoTable;

// A task's or a process's run state (cWScriptTask::State, cWScriptProcess::State): [O] enum tTaskState, names and
// values (cWScriptTaskState::State; On Tour's cWScriptProcessState::State is a plain int). [U] retail uses them the
// same way for both: taskStart / taskEnd set 1 / 3 once, cWScriptTask_update and cWScriptProcess_update turn 1 into 2
// and run the update script in 2, dispatchTaskEvent only passes events to a task in 2, and cWScriptProcess_endTask,
// missionRestart and missionFail test 3 and 2.
enum tTaskState {
    kTaskState_Init = 0,
    kTaskState_Start = 1,
    kTaskState_Update = 2,
    kTaskState_End = 3
};

struct cWScriptTask {
    // cWScriptTaskState
    int State;                         // 0x00 [O] tTaskState; [U] taskEnd, abortTask, dispatchTaskEvent
    // cWScriptTask
    unsigned int TaskId;               // 0x04 [O]; [R] initTask; [U] getTaskId
    char* TaskScriptCode[3];           // 0x08 [O] (const char* there); [U] taskStart, taskUpdate, taskEnd
    char* EventScriptCode[10];         // 0x14 [O] (const char* there); [U] eventOnCollide (0x14) ... eventOnEvent3
                                       //      (0x38)
    cWScriptProcess* Process;          // 0x3C [O]; [R] initTask; [U] the event handlers
    // 0x40: the vptr (g++ 2.95 puts it after the data). [R] cWScriptTask_vtbl's 16 slots in this order, all
    // cWScriptTask_* (On Tour's methods of the same names). Defined as extern "C" functions; nothing here is defined.
    virtual ~cWScriptTask();                                       // 1
    virtual void update();                                         // 2
    virtual void dispatchTaskEvent(const struct tWScriptEvent& event); // 3
    virtual void taskStart() __asm__("cWScriptTask_taskStart");                                      // 4
    virtual void taskUpdate() __asm__("cWScriptTask_taskUpdate");                                     // 5
    virtual void taskEnd() __asm__("cWScriptTask_taskEnd");                                        // 6
    virtual void eventOnCollide() __asm__("cWScriptTask_eventOnCollide");                                 // 7
    virtual void eventOnJump() __asm__("cWScriptTask_eventOnJump");                                    // 8
    virtual void eventOnLand() __asm__("cWScriptTask_eventOnLand");                                    // 9
    virtual void eventOnTrick() __asm__("cWScriptTask_eventOnTrick");                                   // 10
    virtual void eventOnWipeout() __asm__("cWScriptTask_eventOnWipeout");                                 // 11
    virtual void eventOnReset() __asm__("cWScriptTask_eventOnReset");                                   // 12
    virtual void eventOnCollect() __asm__("cWScriptTask_eventOnCollect");                                 // 13
    virtual void eventOnEvent1() __asm__("cWScriptTask_eventOnEvent1");                                  // 14
    virtual void eventOnEvent2() __asm__("cWScriptTask_eventOnEvent2");                                  // 15
    virtual void eventOnEvent3() __asm__("cWScriptTask_eventOnEvent3");                                  // 16

    // Methods bound to their retail symbols by asm labels.
    cLunoTable* getLocalTable() __asm__("cWScriptTask_getLocalTable");
    int getTaskId() __asm__("cWScriptTask_getTaskId");
    void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cWScriptTask_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* obj) __asm__("cWScriptTask_readFromReplayFrame");
};

CHECK_OFFSET(cWScriptTask, TaskId, 0x04);
CHECK_OFFSET(cWScriptTask, EventScriptCode, 0x14);
CHECK_OFFSET(cWScriptTask, Process, 0x3C);
CHECK_SIZE(cWScriptTask, 0x44);

struct cBxReplayFrame;
struct tTrickNameDef;
struct tWScriptEvent;

struct cWScriptProcess {
    // cWScriptProcessState
    int State;                         // 0x00 [O]; [R] ctor = 0; [U] clearState, end, endTask
    int NumTasks;                      // 0x04 [O]; [R] ctor = the definition's task count
    int RunningTask;                   // 0x08 [O]; [R] ctor = NumTasks (no task running); [U] advanceToTask
    unsigned int StateSectionId;       // 0x0C [O] cWScriptProcessState::SectionId (renamed here: the flat layout
                                       //      also has cWScriptProcess::SectionId); no retail method touches it
    // cWScriptProcess
    cWScriptMan* Manager;              // 0x10 [O]; [R] ctor
    cWScriptTask* Tasks;               // 0x14 [O]; [R] ctor's "WScriptTasks" array; [U] the destructor frees it
    cWScriptProcess* Next;             // 0x18 [O]; [R] ctor = 0; [U] getNext, setNext (cProcessList links)
    cLunoTable* Local;                 // 0x1C [O]; [R] ctor = 0; [U] getLocal, the destructor
    unsigned int ProcessId;            // 0x20 [O]; [R] ctor = the definition's id; [U] getProcessId
    unsigned int SectionId;            // 0x24 [O]; [R] ctor; [U] getSectionId, readFromReplayFrame
    char* ProcessScriptCode[3];        // 0x28 [O] (const char* there); [U] processStart, processUpdate, processEnd
    char* EventScriptCode[10];         // 0x34 [O] (const char* there); [U] eventOnCollide (0x34) ... eventOnEvent3
                                       //      (0x58)
    void** mVtbl;                      // 0x5C [R] ctor = cWScriptProcess_vtbl

    // Methods bound to their retail symbols by asm labels.
    cWScriptProcess* getNext() __asm__("cWScriptProcess_getNext");
    void setNext(cWScriptProcess* nextProcess) __asm__("cWScriptProcess_setNext");
    int getSectionId() __asm__("cWScriptProcess_getSectionId");
    void setTrickDef(tTrickNameDef* trickDef) __asm__("cWScriptProcess_setTrickDef");
    cLunoTable* getLocalTable() __asm__("cWScriptProcess_getLocalTable");
    int getProcessId() __asm__("cWScriptProcess_getProcessId");
    void runScriptCode() __asm__("cWScriptProcess_runScriptCode");
    void runScriptCode1() __asm__("cWScriptProcess_runScriptCode1");
    void dispatchProcessEvent(tWScriptEvent* vmsg) __asm__("cWScriptProcess_dispatchProcessEvent");
    unsigned int getRunningTaskId() __asm__("cWScriptProcess_getRunningTaskId");
    void dispatchTaskEvent(int event) __asm__("cWScriptProcess_dispatchTaskEvent");
    void eventOnCollide() __asm__("cWScriptProcess_eventOnCollide");
    void eventOnJump() __asm__("cWScriptProcess_eventOnJump");
    void eventOnLand() __asm__("cWScriptProcess_eventOnLand");
    void eventOnTrick() __asm__("cWScriptProcess_eventOnTrick");
    void eventOnWipeout() __asm__("cWScriptProcess_eventOnWipeout");
    void eventOnReset() __asm__("cWScriptProcess_eventOnReset");
    void eventOnCollect() __asm__("cWScriptProcess_eventOnCollect");
    void eventOnEvent1() __asm__("cWScriptProcess_eventOnEvent1");
    void eventOnEvent2() __asm__("cWScriptProcess_eventOnEvent2");
    void eventOnEvent3() __asm__("cWScriptProcess_eventOnEvent3");
    void processStart() __asm__("cWScriptProcess_processStart");
    void processUpdate() __asm__("cWScriptProcess_processUpdate");
    void processComplete() __asm__("cWScriptProcess_processComplete");
    void gotoTask(int id) __asm__("cWScriptProcess_gotoTask");
    void advanceToTask(int id) __asm__("cWScriptProcess_advanceToTask");
    void* getScriptCode() __asm__("cWScriptProcess_getScriptCode__FPv");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cWScriptProcess_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cWScriptProcess_readFromReplayFrame");
    bool isProcessEvent(unsigned int eventId) __asm__("cWScriptProcess_isProcessEvent");
    int isMission() __asm__("cWScriptProcess_isMission__FPv");
    void processEnd() __asm__("cWScriptProcess_processEnd");
    void end() __asm__("cWScriptProcess_end");
};

CHECK_OFFSET(cWScriptProcess, StateSectionId, 0x0C);
CHECK_OFFSET(cWScriptProcess, Manager, 0x10);
CHECK_OFFSET(cWScriptProcess, SectionId, 0x24);
CHECK_OFFSET(cWScriptProcess, ProcessScriptCode, 0x28);
CHECK_OFFSET(cWScriptProcess, EventScriptCode, 0x34);
CHECK_OFFSET(cWScriptProcess, mVtbl, 0x5C);
CHECK_SIZE(cWScriptProcess, 0x60);

#endif

#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "wscript/wscriptcache.h"
#include "sound/ssxaudio.h"
#include "be/beintnewrace.h"
#include "wscript/wscriptprocess.h"
#include "wscript/wscriptman.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "world/worldcache.h"
#include "replay/bxreplayframe.h"
//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_cWScriptProcess);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);
// PORT: cWScriptMan_getScriptCode takes (self, id) (wscriptman); the unit declares one int param.
extern "C" int cWScriptMan_getScriptCode_2(void* self, int id) __asm__("cWScriptMan_getScriptCode");
struct cWScriptTask_initTask_sObj;
struct cWScriptTask_initTask_sDef;
extern "C" void cWScriptTask_initTask(cWScriptTask* self, cWScriptProcess* owner, cWScriptTask_initTask_sDef* def);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char cWScriptProcess_vtbl[];
extern char str_WScriptTasks[];


/**
 * Constructs a world-script process for a world section from its process definition: resolves its scripts and
 * creates its tasks.
 *
 * Stores the vtable (+0x5C), manager (+0x10), section (+0x24) and the definition's id (+0x20), resolves the
 * definition's 3 script IDs into +0x28 (start script at +0x28, completion script at +0x30) and 10 more into
 * +0x34 with cWScriptMan_getScriptCode(), then allocates its "WScriptTasks" (0x44 bytes each) at +0x14 and
 * initialises each from its 0x38-byte task record with cWScriptTask_initTask().
 *
 * The state (+0x0), the Next process link (+0x18, On Tour Next) and the local table (+0x1C) start at 0; the task
 * count (+0x4) and the current task index (+0x8; equal to the count while no task runs) get the task count.
 *
 * @param man  the owning cWScriptMan (On Tour manager)
 * @param sectionId  the world section the process belongs to
 * @param def  the 0x40-byte process definition from the world (On Tour tWScriptProcessData)
 * @return @p self
 */
extern "C" void* cWScriptProcess_cWScriptProcess(cWScriptProcess* self, cWScriptMan* man, int sectionId, tWScriptProcessData* def)
{
    self->mVtbl = (void**)cWScriptProcess_vtbl;
    MEM_fill(self->ProcessScriptCode, 0, 0xC);
    self->SectionId = (unsigned int)sectionId;
    self->Manager = man;
    self->ProcessId = (unsigned int)((int)def->ProcessId);
    self->Local = 0;
    for (int i = 0; i < 3; i++)
        self->ProcessScriptCode[i] = (char*)cWScriptMan_getScriptCode_2(self->Manager, def->ProcessScriptId[i]);
    for (int j = 0; j < 10; j++)
        self->EventScriptCode[j] = (char*)cWScriptMan_getScriptCode_2(self->Manager, def->EventScriptId[j]);
    int n = def->NumTasks;
    self->NumTasks = n;
    cWScriptTask** tasks = &self->Tasks;
    *tasks = new (str_WScriptTasks, 0x20000000, 0) cWScriptTask[n];
    char* ed = ((char*)def->unk_008);
    for (int k = 0; k < self->NumTasks; k++, ed += 0x38)
        cWScriptTask_initTask(&self->Tasks[k], self, (cWScriptTask_initTask_sDef*)ed);
    self->State = 0;
    self->RunningTask = self->NumTasks;
    self->Next = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess__cWScriptProcess);
#ifdef SKIP_ASM
class cWScriptProcess__cWScriptProcess_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01(int flags);
};

void cMemMan_free(void* p);
void operator_delete(int* p);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern char cWScriptProcess_vtbl[];

/**
 * Destroys a world-script process with its tasks and local Luno table, then frees it when asked to.
 *
 * Restores the vtable (+0x5C), runs each task's virtual destructor (vptr at task +0x40, flags 0) from last to
 * first, frees the task array through its array cookie (count at -0x10) and destroys the luno table at +0x1C
 * with flags 3.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// PORT: array cookie arithmetic through int (pointer in int).
extern "C" void cWScriptProcess__cWScriptProcess(cWScriptProcess* self, int flags)
{
    self->mVtbl = (void**)cWScriptProcess_vtbl;
    char* arr = (char*)self->Tasks;
    if (arr != 0) {
        char* task = (char*)(*(int*)(arr - 0x10) * 0x44 + (int)arr);
        while ((char*)self->Tasks != task) {
            task -= 0x44;
            ((cWScriptProcess__cWScriptProcess_cElem*)task)->v01(0);
        }
        cMemMan_free((char*)self->Tasks - 0x10);
    }
    void* obj = self->Local;
    if (obj != 0) {
        luno_cLunoTable__cLunoTable(obj, 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_update);
#ifdef SKIP_ASM
class cWScriptProcess_update_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01();
    virtual void update();
};

class cWScriptProcess_update_cProc {
public:
    int state;
    int count;
    int cur;
    char pad0C[0x8];
    cWScriptProcess_update_cElem* elems;
    char pad18[0x44];
    // vptr at 0x5C
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void processUpdate();
};

/**
 * Updates a running script process for one frame: its current task first, then the process itself.
 *
 * State (+0x0) kTaskState_Start becomes kTaskState_Update and falls through; in kTaskState_Update the current task
 * (index +0x8, when below the task count +0x4) gets its virtual update (slot 2, cWScriptTask_update), and if the state
 * is still kTaskState_Update afterwards the
 * process's virtual slot 5 (cWScriptProcess_processUpdate) runs. Other states do nothing. cWScriptMan_update calls it
 * for every active process.
 */
extern "C" void cWScriptProcess_update(cWScriptProcess_update_cProc* self)
{
    switch (self->state) {
    case kTaskState_Init:
        break;
    case kTaskState_Start:
        self->state = kTaskState_Update;
    case kTaskState_Update: {
        int st = self->state;
        if (st == kTaskState_Update) {
            if (self->cur < self->count) {
                self->elems[self->cur].update();
            }
            if (self->state == st) {
                self->processUpdate();
            }
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_dispatchProcessEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVE_003078C0 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline void VCall_003078C0(char* self, int slot)
{
    sVE_003078C0* vt = *(sVE_003078C0**)(self + 0x5C);
    vt[slot].fn(vthis(self, vt[slot]));
}

/**
 * Dispatches a process event to the matching virtual handler of the process.
 *
 * The event's first halfword (an eWScriptEventType) picks the handler: kWSEvent_Collide .. kWSEvent_Event3 call vtable
 * slots 7-16 (cWScriptProcess_eventOnCollide, OnJump, OnLand, OnTrick, OnWipeout, OnReset, OnCollect, OnEvent1,
 * OnEvent2, OnEvent3). For a trick (kWSEvent_Trick) the manager's (+0x10)
 * TrickDef (+0x29C) points at the event's data (+0x4) during the call and is cleared after. Other ids are ignored.
 * cWScriptMan_update calls it for events flagged 0x20000.
 *
 * @param vself  the process
 * @param vmsg  the event (id halfword at +0x0, data from +0x4)
 */
// Dispatches a script event to the matching virtual handler (slots 7-16).
void cWScriptProcess::dispatchProcessEvent(tWScriptEvent* vmsg)
{
    unsigned short* msg = (unsigned short*)vmsg;
    switch (*msg) {
    case kWSEvent_Collide:
        VCall_003078C0(((char*)this), 7);
        break;
    case kWSEvent_Jump:
        VCall_003078C0(((char*)this), 8);
        break;
    case kWSEvent_Land:
        VCall_003078C0(((char*)this), 9);
        break;
    case kWSEvent_Trick:
        Manager->TrickDef = (tTrickNameDef*)(msg + 2);
        VCall_003078C0(((char*)this), 10);
        Manager->TrickDef = 0;
        break;
    case kWSEvent_Wipeout:
        VCall_003078C0(((char*)this), 11);
        break;
    case kWSEvent_Reset:
        VCall_003078C0(((char*)this), 12);
        break;
    case kWSEvent_Collect:
        VCall_003078C0(((char*)this), 13);
        break;
    case kWSEvent_Event1:
        VCall_003078C0(((char*)this), 14);
        break;
    case kWSEvent_Event2:
        VCall_003078C0(((char*)this), 15);
        break;
    case kWSEvent_Event3:
        VCall_003078C0(((char*)this), 16);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_getRunningTaskId);
#ifdef SKIP_ASM


/**
 * Returns the id of the task the world-script process is running.
 *
 * Reads the id word (+0x4) of the current task when the current index (+0x8) is below the task count (+0x4); the tasks
 * are 0x44-byte cWScriptTask records at +0x14.
 *
 * @return the task id, or 0xFFFFFFFF when no task is running
 */
unsigned int cWScriptProcess::getRunningTaskId()
{
    int i = RunningTask;
    if (i < NumTasks) {
        return Tasks[i].TaskId;
    }
    return 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_dispatchTaskEvent);
#ifdef SKIP_ASM
/**
 * Passes a task event to the process's current task (RunningTask) through its virtual slot 3
 * (cWScriptTask_dispatchTaskEvent), if the index is below the task count (NumTasks).
 *
 * cWScriptMan_update calls it for events flagged 0x10000.
 *
 * @param event  the event (its address)
 */
void cWScriptProcess::dispatchTaskEvent(const tWScriptEvent* event)
{
    int i = RunningTask;
    if (i < NumTasks) {
        Tasks[i].dispatchTaskEvent(*event);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnCollide);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-collide script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x34, unless that is
 * null.
 */
void cWScriptProcess::eventOnCollide()
{
    void* script = EventScriptCode[0];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnJump);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-jump script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x38, unless that is
 * null.
 */
void cWScriptProcess::eventOnJump()
{
    void* script = EventScriptCode[1];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnLand);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-land script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x3C, unless that is
 * null.
 */
void cWScriptProcess::eventOnLand()
{
    void* script = EventScriptCode[2];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnTrick);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-trick script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x40, unless that is
 * null.
 */
void cWScriptProcess::eventOnTrick()
{
    void* script = EventScriptCode[3];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnWipeout);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-wipeout script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x44, unless that is
 * null.
 */
void cWScriptProcess::eventOnWipeout()
{
    void* script = EventScriptCode[4];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnReset);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-reset script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x48, unless that is
 * null.
 */
void cWScriptProcess::eventOnReset()
{
    void* script = EventScriptCode[5];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnCollect);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-collect script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x4C, unless that is
 * null.
 */
void cWScriptProcess::eventOnCollect()
{
    void* script = EventScriptCode[6];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnEvent1);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-event 1 script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x50, unless that is
 * null.
 */
void cWScriptProcess::eventOnEvent1()
{
    void* script = EventScriptCode[7];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnEvent2);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-event 2 script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x54, unless that is
 * null.
 */
void cWScriptProcess::eventOnEvent2()
{
    void* script = EventScriptCode[8];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_eventOnEvent3);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's on-event 3 script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x58, unless that is
 * null.
 */
void cWScriptProcess::eventOnEvent3()
{
    void* script = EventScriptCode[9];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_processStart);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);
extern "C" void cWScriptMan_processStart(void* man, void* proc);
extern char str_ProcessLocalTable[];

/**
 * Starts a world-script process: gives it a fresh local table, runs its start script and starts its first task.
 *
 * Destroys any old luno table at +0x1C and creates a new 8-entry one ("ProcessLocalTable"), sets the state to 1
 * and the current task to 0, runs the script at +0x28 if there is one (cWScriptMan_runScriptCode), and moves the
 * process onto the manager's running list (cWScriptMan_processStart). If there are tasks, it calls the first
 * task's virtual slot 4, which marks a task active and runs its script at +0x8.
 */
void cWScriptProcess::processStart()
{
    if (((void*)Local) != 0) {
        luno_cLunoTable__cLunoTable(((void*)Local), 3);
    }
    Local = (cLunoTable*)luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_ProcessLocalTable, 0x20000000, 0), 8);
    State = 1;
    RunningTask = 0;
    if (((void*)ProcessScriptCode[0]) != 0) {
        cWScriptMan_runScriptCode(((void*)Manager), this, ((void*)ProcessScriptCode[0]));
    }
    cWScriptMan_processStart(((void*)Manager), this);
    if (RunningTask < NumTasks) {
        Tasks[RunningTask].taskStart();
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_processUpdate);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);

/**
 * Runs the world script process's per-frame update script.
 *
 * Calls cWScriptMan_runScriptCode with the manager (+0x10), this process and the script code at +0x2C, unless that is
 * null.
 */
void cWScriptProcess::processUpdate()
{
    void* script = ProcessScriptCode[1];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_processEnd);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);
extern "C" void cWScriptProcess_end(cWScriptProcess* self);

/**
 * Runs the world script process's end script and ends the process.
 *
 * Sets the state (+0x0) to 3, runs the script code at +0x30 through cWScriptMan_runScriptCode when there is one and
 * calls cWScriptProcess_end.
 */
void cWScriptProcess::processEnd()
{
    *(int*)this = 3;
    void* script = ProcessScriptCode[2];
    if (script != 0) {
        cWScriptMan_runScriptCode(Manager, this, script);
    }
    end();
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_end);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_processEnd(void* man, void* proc);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);

/**
 * Ends a world-script process: marks it ended, takes it off the running list and deletes its local table.
 *
 * Sets the state to 3 and calls cWScriptMan_processEnd(), which moves the process from the manager's running
 * list (+0x2B8) back to the list at +0x2B4 if it is there. Then destroys the luno table at +0x1C and clears the
 * pointer.
 */
void cWScriptProcess::end()
{
    *(int*)this = 3;
    cWScriptMan_processEnd(Manager, this);
    void* obj = Local;
    if (obj != 0) {
        luno_cLunoTable__cLunoTable(obj, 3);
    }
    Local = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_processComplete);
#ifdef SKIP_ASM
bool cWScriptMan_isProcessActive(void* man, void* proc);


/**
 * Completes a world-script process if its manager still has it on the running list.
 *
 * cWScriptMan_isProcessActive() looks for a process with the same id (+0x20) on the running list; when there is one,
 * this calls its own virtual slot 6, which for cWScriptProcess (cWScriptProcess_processEnd) sets the state to 3, runs
 * the script at +0x30 and calls cWScriptProcess_end(). The task stepper cWScriptProcess_endTask calls it after the last
 * task.
 */
void cWScriptProcess::processComplete()
{
    if (cWScriptMan_isProcessActive(Manager, this) != 0) {
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)mVtbl;
        vt[6].fn(vthis(this, vt[6]));
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_endTask);
#ifdef SKIP_ASM
class cWScriptProcess_endTask_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void taskStart();
    virtual void v05();
    virtual void taskEnd();
};

class cWScriptProcess_endTask_cProc {
public:
    int state;
    int count;
    int cur;
    char pad0C[0x8];
    cWScriptProcess_endTask_cElem* elems;
    char pad18[0x44];
    // vptr at 0x5C
    virtual void v01();
    virtual void processComplete();
};

/**
 * Ends the process's current task and moves on to the next.
 *
 * Does nothing once the process has ended (kTaskState_End). Otherwise calls virtual slot 6 of the current task (index
 * +0x8 into the 0x44-byte tasks at +0x14); if another task follows (count at +0x4) it becomes current and gets its
 * virtual slot 4, else the process's own virtual slot 2 runs.
 */
extern "C" void cWScriptProcess_endTask(cWScriptProcess_endTask_cProc* self)
{
    if (self->state == kTaskState_End) {
        return;
    }
    self->elems[self->cur].taskEnd();
    int next = self->cur + 1;
    if (next < self->count) {
        self->cur = next;
        cWScriptProcess_endTask_cElem* nextTask = &self->elems[next];
        nextTask->taskStart();
    } else {
        self->processComplete();
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_gotoTask);
#ifdef SKIP_ASM
/**
 * Jumps a script process to the task with id @p id: aborts the current task and starts that one.
 *
 * Finds the first of the task count (+0x4) tasks whose id (+0x4) is @p id; marks the current task (index +0x8, when
 * valid) aborted (state 3), makes the found task current and calls its virtual slot 4 (cWScriptTask_taskStart). An
 * unknown id does nothing (On Tour asserts instead). cWScriptMan_gotoTask calls it on the manager's current process.
 *
 * @param id  the task id to go to
 */
void cWScriptProcess::gotoTask(int id)
{
    int i;
    for (i = 0; i < NumTasks; i++) {
        if (Tasks[i].TaskId == id) {
            if (RunningTask < NumTasks) {
                Tasks[RunningTask].State = 3;
            }
            RunningTask = i;
            Tasks[i].taskStart();
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_advanceToTask);
#ifdef SKIP_ASM
/**
 * Advances a script process to the task with id @p id: ends the current task and starts that one.
 *
 * Like cWScriptProcess_gotoTask, but the current task (index +0x8, when valid) is ended normally through its virtual
 * slot 6 (cWScriptTask_taskEnd) instead of being aborted; the found task becomes current and gets slot 4
 * (cWScriptTask_taskStart). An unknown id does nothing. cWScriptMan_advanceToTask calls it on the manager's current
 * process.
 *
 * @param id  the task id to advance to
 */
void cWScriptProcess::advanceToTask(int id)
{
    int i;
    for (i = 0; i < NumTasks; i++) {
        if (Tasks[i].TaskId == id) {
            if (RunningTask < NumTasks) {
                Tasks[RunningTask].taskEnd();
            }
            RunningTask = i;
            Tasks[i].taskStart();
            return;
        }
    }
}
#endif

extern "C" void* cWScriptMan_getScriptCode(cWScriptMan* man);

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_getScriptCode__FPv);
#ifdef SKIP_ASM
/**
 * Returns the script code for a resource id.
 *
 * Forwards to cWScriptMan_getScriptCode with the process's manager (+0x10); the id in the second argument is passed
 * through untouched. The declaration here takes only the process; callers use the cWScriptProcess_getScriptCode__FPv
 * alias.
 *
 * @return the code, or 0 when the id is invalid
 */
void* cWScriptProcess::getScriptCode()
{
    return cWScriptMan_getScriptCode(Manager);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_writeToReplayFrame);
#ifdef SKIP_ASM
class cWScriptTask_writeToReplayFrame_cObj;
extern "C" void cWScriptTask_writeToReplayFrame(cWScriptTask* a, cBxReplayFrame* obj);
extern "C" int cWScriptCache_getScriptBase(void* map, int id);
extern "C" void luno_cLunoTable_writeToReplayFrame(void* obj, void* stream, int a2);

/**
 * Writes a script process's state to a replay frame.
 *
 * Writes the first 0x10 bytes (state, task count, current task index, +0xC) with the frame's first virtual, gets the
 * section's script base with cWScriptCache_getScriptBase (cWorld_m_pTheWorld's cache, section +0x24), writes whether
 * the process has a local table (+0x1C) and, if so, the table with luno_cLunoTable_writeToReplayFrame against that
 * base, then each task with cWScriptTask_writeToReplayFrame. cWScriptMan_writeReplayProcessList calls it for each
 * process in a list.
 *
 * @param stream  the replay frame
 */
void cWScriptProcess::writeToReplayFrame(cBxReplayFrame* stream)
{
    stream->write((const char*)this, 0x10);
    int x = cWScriptCache_getScriptBase(cWorld_m_pTheWorld->m_pWScriptCache, (int)SectionId);
    int has = Local != 0;
    stream->write((const char*)&has, 4);
    if (has) {
        luno_cLunoTable_writeToReplayFrame(Local, stream, x);
    }
    int i;
    for (i = 0; i < NumTasks; i++) {
        Tasks[i].writeToReplayFrame(stream);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_readFromReplayFrame);
#ifdef SKIP_ASM
extern char str_ProcessLocalTable[];
extern "C" int cWScriptCache_getScriptBase(void* map, int id);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern "C" void luno_cLunoTable_readFromReplayFrame(void* obj, cBxReplayFrame* stream, int a2);
class cWScriptTask_readFromReplayFrame_cObj;
extern "C" void cWScriptTask_readFromReplayFrame(cWScriptTask* a, cBxReplayFrame* obj);

/**
 * Reads a script process's state back from a replay frame (the reverse of cWScriptProcess_writeToReplayFrame).
 *
 * Reads the first 0x10 bytes with the frame's second virtual, gets the section's script base, destroys any local
 * table (+0x1C, flags 3) and clears it, reads the has-table flag and, if set, allocates a new 4-byte
 * "ProcessLocalTable" cLunoTable (constructed with 8) and fills it with luno_cLunoTable_readFromReplayFrame, then
 * reads each task with cWScriptTask_readFromReplayFrame. cWScriptMan_readReplayProcessList calls it for each process in
 * a list.
 *
 * @param stream  the replay frame
 */
void cWScriptProcess::readFromReplayFrame(cBxReplayFrame* stream)
{
    stream->read((char*)this, 0x10);
    int x = cWScriptCache_getScriptBase(cWorld_m_pTheWorld->m_pWScriptCache, (int)SectionId);
    if (Local != 0) {
        luno_cLunoTable__cLunoTable(Local, 3);
    }
    Local = 0;
    int has;
    stream->read((char*)&has, 4);
    if (has) {
        void* localTable = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_ProcessLocalTable, 0x20000000, 0), 8);
        Local = (cLunoTable*)localTable;
        luno_cLunoTable_readFromReplayFrame(localTable, stream, x);
    }
    int i;
    for (i = 0; i < NumTasks; i++) {
        Tasks[i].readFromReplayFrame(stream);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_cWScriptTask);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

/**
 * Constructs an empty script task: g++ stores cWScriptTask_vtbl (vptr at +0x40); zeroes its state, id and scripts.
 *
 * Clears the three task scripts (+0x8..+0x13), the process (+0x3C), the id (+0x4) and the state (+0x0).
 * cWScriptProcess_cWScriptProcess builds its task array with it before cWScriptTask_initTask fills each task.
 */
cWScriptTask::cWScriptTask()
{
    MEM_fill(TaskScriptCode, 0, 0xC);
    Process = 0;
    TaskId = 0;
    State = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_initTask);
#ifdef SKIP_ASM
// PORT: cWScriptProcess_getScriptCode is declared (void*) but takes (self, id) and forwards id; bound by asm label
void* cWScriptProcess_getScriptCode_2(void* self, int id) __asm__("cWScriptProcess_getScriptCode__FPv");

struct cWScriptTask_initTask_sDef {
    int f0;
    int a[3];
    int b[10];
};

struct cWScriptTask_initTask_sObj {
    int f0;
    int f4;
    void* a[3];
    void* b[10];
    void* owner;
};

/**
 * Initialises a world-script task from its task data, resolving the data's script ids to script code.
 *
 * Stores @p owner, copies the task id from the data's first word to +0x4 (cWScriptProcess_gotoTask finds tasks by it),
 * and turns the data's 3 + 10 script resource ids into code pointers with cWScriptMan_getScriptCode() through the
 * owner's manager (cWScriptProcess_getScriptCode).
 *
 * @param owner  the cWScriptProcess the task belongs to
 * @param def    the task's data record (On Tour: tWScriptTaskData)
 */
extern "C" void cWScriptTask_initTask(cWScriptTask* self, cWScriptProcess* owner, cWScriptTask_initTask_sDef* def)
{
    int i;
    int j;
    self->Process = (cWScriptProcess*)owner;
    self->TaskId = (unsigned int)def->f0;
    for (i = 0; i < 3; i++) {
        self->TaskScriptCode[i] = (char*)cWScriptProcess_getScriptCode_2(self->Process, def->a[i]);
    }
    for (j = 0; j < 10; j++) {
        self->EventScriptCode[j] = (char*)cWScriptProcess_getScriptCode_2(self->Process, def->b[j]);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_update);
#ifdef SKIP_ASM
class cWScriptTask_update_cProcess {
public:
    int state;                   // 0x0
    char pad_0x04[0x40 - 0x4];
    // vptr at 0x40; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void taskUpdate();
};

/**
 * Runs one frame of a world-script task: a started task (kTaskState_Start) becomes running (kTaskState_Update), and a
 * running task runs its update script (slot 5, taskUpdate).
 */
extern "C" void cWScriptTask_update(cWScriptTask_update_cProcess* self)
{
    switch (self->state) {
    case kTaskState_Init:
        break;
    case kTaskState_Start:
        self->state = kTaskState_Update;
        self->taskUpdate();
        break;
    case kTaskState_Update:
        self->taskUpdate();
        break;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_dispatchTaskEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVE_003083A0 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline void VCall_003083A0(char* self, int slot)
{
    sVE_003083A0* vt = *(sVE_003083A0**)(self + 0x40);
    vt[slot].fn(vthis(self, vt[slot]));
}

/**
 * Passes a world-script event to a running task (kTaskState_Update) as the matching event virtual.
 *
 * Event types (eWScriptEventType) kWSEvent_Collide .. kWSEvent_Event3 call slots 7-16: on collide, jump, land, trick,
 * wipeout, reset, collect and event 1-3. For a trick the event's data (+0x4) is put in the script manager's word +0x29C
 * while the handler runs.
 *
 * @param vmsg  the event (On Tour const tWScriptEvent& event): type in the first halfword
 */
// Same event dispatch as cWScriptProcess_dispatchProcessEvent for a running (kTaskState_Update) sub-process.
void cWScriptTask::dispatchTaskEvent(const struct tWScriptEvent& vmsg)
{
    unsigned short* msg = (unsigned short*)&vmsg;
    if (*(int*)this != kTaskState_Update) {
        return;
    }
    switch (*msg) {
    case kWSEvent_Collide:
        VCall_003083A0(((char*)this), 7);
        break;
    case kWSEvent_Jump:
        VCall_003083A0(((char*)this), 8);
        break;
    case kWSEvent_Land:
        VCall_003083A0(((char*)this), 9);
        break;
    case kWSEvent_Trick:
        Process->Manager->TrickDef = (tTrickNameDef*)(msg + 2);
        VCall_003083A0(((char*)this), 10);
        Process->Manager->TrickDef = 0;
        break;
    case kWSEvent_Wipeout:
        VCall_003083A0(((char*)this), 11);
        break;
    case kWSEvent_Reset:
        VCall_003083A0(((char*)this), 12);
        break;
    case kWSEvent_Collect:
        VCall_003083A0(((char*)this), 13);
        break;
    case kWSEvent_Event1:
        VCall_003083A0(((char*)this), 14);
        break;
    case kWSEvent_Event2:
        VCall_003083A0(((char*)this), 15);
        break;
    case kWSEvent_Event3:
        VCall_003083A0(((char*)this), 16);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnCollide);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's on-collide script (EventScriptCode[0], +0x14), when it has one, through the process's script
 * manager (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnCollide()
{
    void* script = EventScriptCode[0];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnJump);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's on-jump script (EventScriptCode[1], +0x18), when it has one, through the process's script manager
 * (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnJump()
{
    void* script = EventScriptCode[1];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnLand);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's on-land script (EventScriptCode[2], +0x1C), when it has one, through the process's script manager
 * (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnLand()
{
    void* script = EventScriptCode[2];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnTrick);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's on-trick script (EventScriptCode[3], +0x20), when it has one, through the process's script
 * manager (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnTrick()
{
    void* script = EventScriptCode[3];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnWipeout);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's on-wipeout script (EventScriptCode[4], +0x24), when it has one, through the process's script
 * manager (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnWipeout()
{
    void* script = EventScriptCode[4];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnReset);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's on-reset script (EventScriptCode[5], +0x28), when it has one, through the process's script
 * manager (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnReset()
{
    void* script = EventScriptCode[5];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnCollect);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's on-collect script (EventScriptCode[6], +0x2C), when it has one, through the process's script
 * manager (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnCollect()
{
    void* script = EventScriptCode[6];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnEvent1);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's event-1 script (EventScriptCode[7], +0x30), when it has one, through the process's script manager
 * (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnEvent1()
{
    void* script = EventScriptCode[7];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnEvent2);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's event-2 script (EventScriptCode[8], +0x34), when it has one, through the process's script manager
 * (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnEvent2()
{
    void* script = EventScriptCode[8];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnEvent3);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's event-3 script (EventScriptCode[9], +0x38), when it has one, through the process's script manager
 * (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::eventOnEvent3()
{
    void* script = EventScriptCode[9];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_taskStart);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Starts the task once: sets its state to kTaskState_Start and runs its start script (TaskScriptCode[0], +0x8) when it
 * has one.
 */
void cWScriptTask::taskStart()
{
    if (*(int*)this != kTaskState_Start) {
        *(int*)this = kTaskState_Start;
        void* script = TaskScriptCode[0];
        if (script != 0) {
            cWScriptMan_runScriptCode1(Process->Manager, this, script);
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_taskUpdate);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Runs the task's update script (TaskScriptCode[1], +0xC), when it has one, through the process's script manager
 * (cWScriptMan_runScriptCode1, On Tour cWScriptMan::runScriptCode).
 */
void cWScriptTask::taskUpdate()
{
    void* script = TaskScriptCode[1];
    if (script != 0) {
        cWScriptMan_runScriptCode1(Process->Manager, this, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_taskEnd);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Ends the task once: sets its state to kTaskState_End and runs its end script (TaskScriptCode[2], +0x10) when it has
 * one.
 */
void cWScriptTask::taskEnd()
{
    if (*(int*)this != kTaskState_End) {
        *(int*)this = kTaskState_End;
        void* script = TaskScriptCode[2];
        if (script != 0) {
            cWScriptMan_runScriptCode1(Process->Manager, this, script);
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_writeToReplayFrame);
#ifdef SKIP_ASM
class cWScriptTask_writeToReplayFrame_cObj {
public:
    // slot N at vtable offset N*8
    virtual int write(void* a, int b);
};

/**
 * Writes a script task's state word (+0x0, 4 bytes) to a replay frame through the frame's first virtual.
 *
 * cWScriptProcess_writeToReplayFrame calls it for each of a process's tasks.
 *
 * @param obj  the replay frame
 * @return the frame call's result
 */
void cWScriptTask::writeToReplayFrame(cBxReplayFrame* obj)
{
    obj->write((const char*)this, 4);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_readFromReplayFrame);
#ifdef SKIP_ASM
class cWScriptTask_readFromReplayFrame_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
    virtual int read(void* a, int b);
};

/**
 * Reads a script task's state word (+0x0, 4 bytes) back from a replay frame through the frame's second virtual.
 *
 * cWScriptProcess_readFromReplayFrame calls it for each of a process's tasks.
 *
 * @param obj  the replay frame
 * @return the frame call's result
 */
void cWScriptTask::readFromReplayFrame(cBxReplayFrame* obj)
{
    obj->read((char*)this, 4);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_cWScriptMan);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void __17tMissionMenuQueue(void* p);
void cProcessList_cProcessList(void* p);
extern "C" void cCollectPool_init(void* p);
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void cWScriptMan_init(cWScriptMan* self);
extern char cWScriptMan_vtbl[];
// cWScriptMan_TheWScriptMan is declared `int` later in the unit; it holds the current process.
extern void* cWScriptMan_cWScriptMan_current __asm__("cWScriptMan_TheWScriptMan");

/**
 * Constructs the world script manager.
 *
 * Sets +0x2C to -1, constructs the mission menu queue at +0x1C4, sets the vtable cWScriptMan_vtbl at +0x494, constructs
 * the idle (+0x2B4), active (+0x2B8) and pending (+0x2BC) process lists and the collect pool at +0x2C0, stores this in
 * cWScriptMan_TheWScriptMan, takes cWorld_m_pTheWorld as its world (+0x28C), zeroes the 50 section table pointers at
 * +0x3CC and runs cWScriptMan_init.
 *
 * @return this
 */
extern "C" void* cWScriptMan_cWScriptMan(cWScriptMan* self)
{
    self->MissionHeightInstId = 0xFFFFFFFF;
    __17tMissionMenuQueue(&self->MissionMenuQueue);
    self->mVtbl = cWScriptMan_vtbl;
    cProcessList_cProcessList((char*)&self->IdleList);
    cProcessList_cProcessList((char*)&self->ActiveList);
    cProcessList_cProcessList((char*)&self->AddList);
    cCollectPool* collectPool = self->CollectPools;
    int i;
    for (i = 0; i != -1; i--) {
        cCollectPool* next = collectPool + 1;
        unsigned int* poolSlot = collectPool->InstIds;
        int j;
        for (j = 63; j != -1; j--) {
            *poolSlot++ = 0xFFFFFFFF;
        }
        cCollectPool_init(collectPool);
        collectPool = next;
    }
    cWScriptMan_cWScriptMan_current = ((void*)self);
    self->World = (cWorld*)cWorld_m_pTheWorld;
    MEM_fill((char*)self->GlobalTables, 0, 200);
    self->init();
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan__cWScriptMan);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_purgeGlobalTables(cWScriptMan* self);
extern "C" void cProcessList__cProcessList(void* self, int flags);
void operator_delete(int* p);
extern char cWScriptMan_vtbl[];
extern int cWScriptMan_TheWScriptMan;

/**
 * Destroys the world script manager.
 *
 * Restores the vtable cWScriptMan_vtbl, runs cWScriptMan_purgeGlobalTables, clears cWScriptMan_TheWScriptMan, destroys
 * the pending, active and idle process lists (cProcessList__cProcessList, flag 2) and calls operator delete when bit 0
 * of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
extern "C" void cWScriptMan__cWScriptMan(cWScriptMan* self, int flags)
{
    self->mVtbl = cWScriptMan_vtbl;
    self->purgeGlobalTables();
    cWScriptMan_TheWScriptMan = 0;
    if (self->CollectPools != 0) {
        cCollectPool* collectPool = self->CollectPools + 1;
        while (self->CollectPools != collectPool) {
            collectPool--;
        }
    }
    cProcessList__cProcessList((char*)&self->AddList, 2);
    cProcessList__cProcessList((char*)&self->ActiveList, 2);
    cProcessList__cProcessList((char*)&self->IdleList, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_init);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void tMissionMenuQueue_init(void* p);
extern "C" void cWScriptMan_rebuildGlobalTables(cWScriptMan* self);


/**
 * Resets the world script manager to its starting state.
 *
 * Clears the current being, instance, infos, process and task and the mission state, sets the mission ids (+0x2A0,
 * +0x2B0) and several counters to -1, empties the menu queue (tMissionMenuQueue_init) and the event list (Events), and
 * runs cWScriptMan_rebuildGlobalTables.
 */
void cWScriptMan::init()
{
    Inst = 0;
    Being = 0;
    CollideInfo = 0;
    IntersectInfo = 0;
    CurrentProcess = 0;
    CurrentTask = 0;
    TrickDef = 0;
    LocalMissionCmd = 0;
    MissionPointsEnable = 0;
    MissionPoints = 0;
    MissionCountEnable = 0;
    MissionCount = 0;
    MissionCountMax = 0;
    MissionTicksEnable = 0;
    MissionTicks = 0;
    MissionHeightEnable = 0;
    MissionHeightBeing = 0;
    MissionTrickEnable = 0;
    LocalMissionId = 0xFFFFFFFF;
    LocalMissionCmdId = 0xFFFFFFFF;
    MissionPointsGoal = -1;
    MissionTicksDir = -1;
    MissionHeightInstId = 0xFFFFFFFF;
    MEM_fill(&MissionTrick, 0, sizeof(MissionTrick));
    tMissionMenuQueue_init(&MissionMenuQueue);
    // Events is the tWScriptEventList (the header types it as its 0x184 bytes)
    tWScriptEventList* events = (tWScriptEventList*)Events;
    events->NumEvents = 0;
    MEM_fill(events->Events, 0, 0x180);
    rebuildGlobalTables();
}

#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_purgeGlobalTables);
#ifdef SKIP_ASM
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);

/**
 * Destroys every world section's global script table.
 *
 * Destroys and clears each non-null table pointer among the 50 at +0x3CC.
 */
void cWScriptMan::purgeGlobalTables()
{
    void** objs = (void**)GlobalTables;
    int i;
    for (i = 0; i < 50; i++) {
        if (objs[i] != 0) {
            luno_cLunoTable__cLunoTable(objs[i], 3);
            objs[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_rebuildGlobalTables);
#ifdef SKIP_ASM
struct cWorldView;
int cWorldView_getNumSections(cWorldView* view);
extern "C" void cWScriptMan_purgeGlobalTables(cWScriptMan* self);
// PORT: the unit declares cWScriptMan_allocateGlobalTable as (void* self); the body also takes the slot index
void* cWScriptMan_allocateGlobalTable_impl(void* self, int i) __asm__("cWScriptMan_allocateGlobalTable");

/**
 * Recreates the global script tables for the loaded world sections.
 *
 * Runs cWScriptMan_purgeGlobalTables, then for every section of the world view whose script cache entry has reached
 * state 2 and has data, calls cWScriptMan_allocateGlobalTable with the section index.
 */
void cWScriptMan::rebuildGlobalTables()
{
    int i = 0;
    purgeGlobalTables();
    int n = cWorldView_getNumSections(&World->m_pWorldCache->m_WorldView);
    for (; i < n; i++) {
        cWScriptCacheEntry* tbl = World->m_pWScriptCache->mpCache;
        int ok = tbl[i].mState >= 2 && tbl[i].mpScriptHeader != 0;
        if (ok) {
            this->allocateGlobalTable(i);
        }
    }
}
#endif

extern "C" void* cWScriptMan_allocateGlobalTable(void* self);

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_loadSection__FPv);
#ifdef SKIP_ASM
/**
 * Prepares a loaded world section's scripts: allocates the section's global Luno table with
 * cWScriptMan_allocateGlobalTable.
 *
 * The section index arrives in the second argument register and is passed through untouched (the C here declares
 * only @p self). gameload calls it on cWScriptMan_TheWScriptMan once a section's scripts are loaded.
 *
 * @return cWScriptMan_allocateGlobalTable's result (the new table)
 */
void* cWScriptMan::loadSection()
{
    return cWScriptMan_allocateGlobalTable(this);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_allocateGlobalTable);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern const char str_cLunoTable_b[];

// PORT: the unit declares cWScriptMan_allocateGlobalTable as (void* self); the body also takes the slot index
void* cWScriptMan_allocateGlobalTable_impl(void* self, int i) __asm__("cWScriptMan_allocateGlobalTable");
/**
 * (Re)allocates the global Luno table of world section @p i: destroys the old one (flags 3) and stores a new one.
 *
 * The table array starts at +0x3CC (cWScriptMan_getGlobalTable reads it); the new table is a 4-byte "cLunoTable"
 * allocation (flags 0x20000000) constructed with 0x40. No bounds check (On Tour asserts sectionId < kMaxGlobalTables).
 * cWScriptMan_rebuildGlobalTables calls it for every section whose script data is loaded, cWScriptMan_loadSection for
 * one.
 *
 * @param i  the world section index
 * @return the new table
 */
void* cWScriptMan::allocateGlobalTable(int i)
{
    cLunoTable** tables = GlobalTables;
    cLunoTable** slot = &tables[i];
    if (*slot != 0) {
        luno_cLunoTable__cLunoTable(*slot, 3);
    }
    return *slot = (cLunoTable*)luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_cLunoTable_b, 0x20000000, 0), 0x40);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_activeSection);
#ifdef SKIP_ASM
extern "C" int cWorld_getNumProcessScripts(void* self, int i);
extern "C" void* cWorld_getProcessScripts(void* self, int i);
extern "C" void cWScriptMan_addProcess(void* self, int section, void* def, int flags);

struct cWScriptMan_activeSection_sDef {
    char pad[0x40];
};

/**
 * Adds the world-script processes of a world section that has become active.
 *
 * Calls cWScriptMan_addProcess(self, @p section, record, 0) for each of the section's 0x40-byte process script
 * records, which come from the world (+0x28C) through cWorld_getNumProcessScripts() and
 * cWorld_getProcessScripts().
 *
 * @param section  world section id
 */
void cWScriptMan::activeSection(int section)
{
    int n = cWorld_getNumProcessScripts(World, section);
    cWScriptMan_activeSection_sDef* defs = (cWScriptMan_activeSection_sDef*)cWorld_getProcessScripts(World, section);
    int i;
    for (i = 0; i < n; i++) {
        cWScriptMan_addProcess(this, section, &defs[i], 0);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_setupMissions);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cPlaybackMan_purgeLocation(void* pb);
extern "C" int cWorld_getNumMissionScripts(void* world, int i);
extern "C" void* cWorld_getMissionScripts(void* world, int i);
extern "C" void cWScriptMan_addProcess1(void* self, int section, void* def, int id, int flags);
extern "C" tBELocationDB* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cPlaybackMan_initLocation(void* pb, void* loc);
extern "C" void cWScriptMan_updateWorld(cWScriptMan* self);

static inline void* wsPlayback8C60()
{
    return SSXApp->Game->PlaybackMan;
}

/**
 * Sets up the world's missions: in game mode kGAMEMODE_FREERIDE with one player, creates a mission process for every
 * mission script of every world section.
 *
 * First releases the playback manager's loaded data (cPlaybackMan_purgeLocation). When cGameMode (newRace.cGameMode) is
 * kGAMEMODE_FREERIDE and the AI's NumberOfPlayersAllocated is 1, it adds a cWScriptMission (cWScriptMan_addProcess1)
 * per 0x54-byte mission script (tWScriptMissionData), keyed (section << 16) + section, and calls
 * cPlaybackMan_initLocation() for the race location's data (+0x34). cWScriptMan_updateWorld() always runs last: it runs
 * each mission script's code at +0x4C and clears the manager's mission state.
 */
void cWScriptMan::setupMissions()
{
    void* iface;
    cPlaybackMan_purgeLocation(wsPlayback8C60());
    iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_FREERIDE
        && SSXApp->Game->AI->NumberOfPlayersAllocated == 1) {
        int i = 0;
        int n = cWorldView_getNumSections(&World->m_pWorldCache->m_WorldView);
        for (; i < n; i++) {
            int cnt = cWorld_getNumMissionScripts(World, i);
            tWScriptMissionData* defs = (tWScriptMissionData*)cWorld_getMissionScripts(World, i);
            int j;
            for (j = 0; j < cnt; j++) {
                cWScriptMan_addProcess1(this, i, &defs[j], (i << 16) + i, 0);
            }
        }
        cPlaybackMan_initLocation(wsPlayback8C60(), cBENewRaceInterface_getLocation(iface)->fileName);
    }
    updateWorld();
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_updateWorld);
#ifdef SKIP_ASM
// cLunoValue: { word0, word1, type }
struct sWSPValue8DB8 {
    int word0;
    int word1;
    int type;
};

struct sWSPCall8DB8 {
    int pad[4];
};

struct sWSPDef8DB8 {
    int f0;                     // 0x00
    char pad4[0x48];
    int f4C;                    // 0x4C
    char pad50[0x4];
};

struct cWScriptMan_isFunc_sValue;
extern const char str_SetupLocalTable[];
extern "C" int cWorld_getNumMissionScripts(void* world, int i);
extern "C" void* cWorld_getMissionScripts(void* world, int i);
// PORT: the unit declares cWScriptMan_getScriptCode as (int); its body takes (self, id) and returns an int.
extern "C" int cWScriptMan_getScriptCode_2(void* self, int id) __asm__("cWScriptMan_getScriptCode");
void luno_cLunoFunc_init(void* self, int a1, int a2);
extern "C" void* luno_cLunoVM_cLunoVM(void* self);
extern "C" void luno_cLunoVM_run(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);

/**
 * Applies every mission's world-update script to the world.
 *
 * For each world section and each of its mission scripts whose id differs from the local mission (+0x2A0), creates an
 * 8-slot local table, runs the code for the mission's +0x4C script id in it if there is any, and destroys the table.
 * Then clears the current instance, infos, process and task.
 */
void cWScriptMan::updateWorld()
{
    int i = 0;
    int n = cWorldView_getNumSections(&World->m_pWorldCache->m_WorldView);
    for (; i < n; i++) {
        int cnt = cWorld_getNumMissionScripts(World, i);
        sWSPDef8DB8* defs = (sWSPDef8DB8*)cWorld_getMissionScripts(World, i);
        int j;
        for (j = 0; j < cnt; j++) {
            if (defs[j].f0 != (int)LocalMissionId) {
                void* table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
                int scriptCode = cWScriptMan_getScriptCode_2(this, defs[j].f4C);
                if (scriptCode != 0) {
                    sWSPValue8DB8 scriptFunc;
                    sWSPValue8DB8 ret;
                    sWSPCall8DB8 call;
                    luno_cLunoFunc_init(&scriptFunc, scriptCode, 0);
                    ret.type = 0;
                    ret.word0 = 0;
                    luno_cLunoVM_cLunoVM(&call);
                    luno_cLunoVM_run(&call, &scriptFunc, table, &ret);
                    luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
                }
                if (table != 0) {
                    luno_cLunoTable__cLunoTable(table, 3);
                }
            }
        }
    }
    Inst = 0;
    CollideInfo = 0;
    IntersectInfo = 0;
    CurrentProcess = 0;
    CurrentTask = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_purgeMissions);
#ifdef SKIP_ASM
extern "C" void cProcessList_deleteAll(void*);

/**
 * Deletes every world script process.
 *
 * Runs cProcessList_deleteAll on the active (+0x2B8), pending (+0x2BC) and idle (+0x2B4) process lists.
 */
void cWScriptMan::purgeMissions()
{
    cProcessList_deleteAll((char*)&ActiveList);
    cProcessList_deleteAll((char*)&AddList);
    cProcessList_deleteAll((char*)&IdleList);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_purgeSection);
#ifdef SKIP_ASM
extern "C" void cProcessList_unloadSection(void* list, int section);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);

/**
 * Removes a world section's processes from the manager's three process lists and deletes its local table.
 *
 * Calls cProcessList_unloadSection() on the lists at +0x2B4, +0x2B8 and +0x2BC, then destroys the section's luno
 * table (pointer array at +0x3CC, indexed by section) and clears the pointer.
 *
 * @param section  world section id
 */
void cWScriptMan::purgeSection(int section)
{
    cProcessList_unloadSection((char*)&IdleList, section);
    cProcessList_unloadSection((char*)&ActiveList, section);
    cProcessList_unloadSection((char*)&AddList, section);
    int off = section << 2;
    char* base = (char*)GlobalTables;
    void** slot = (void**)(base + off);
    if (*slot != 0) {
        luno_cLunoTable__cLunoTable(*slot, 3);
        *slot = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_unloadSection);
#ifdef SKIP_ASM
struct cWScriptMan_unloadSection_sEntry {
    char pad[0xC];
};

extern "C" void cWScriptMan_purgeSection(cWScriptMan* self, int i);
extern "C" void cWScriptCacheEntry_unload(void* entry);

/**
 * Unloads a world section's scripts: purges its processes, then releases its world script cache entry.
 *
 * After cWScriptMan_purgeSection() it calls cWScriptCacheEntry_unload on entry @p i of the world's script cache
 * (m_pWScriptCache of the world at +0x28C), which frees the entry's buffer unless its +0x2 flag is set and
 * clears the entry.
 *
 * @param i  world section id
 */
void cWScriptMan::unloadSection(int i)
{
    purgeSection(i);
    cWScriptMan_unloadSection_sEntry* items =
        (cWScriptMan_unloadSection_sEntry*)World->m_pWScriptCache->mpCache;
    cWScriptCacheEntry_unload(&items[i]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_initMissions);
#ifdef SKIP_ASM
struct cWorldView;
int cWorldView_getNumSections(cWorldView* view);
extern "C" void cProcessList_deleteAll(void*);
extern "C" void cWScriptMan_init(cWScriptMan* self);
extern "C" void cWScriptMan_activeSection(cWScriptMan* self, int section);
extern "C" void cWScriptMan_purgeSection(cWScriptMan* self, int section);

/**
 * Rebuilds the world-script processes: deletes every process, resets the manager and re-adds the processes of
 * each ready world section.
 *
 * Deletes the processes on the three lists (+0x2B8, +0x2BC, +0x2B4; cProcessList_deleteAll), resets the manager's
 * state (cWScriptMan_init), then for each world section calls cWScriptMan_activeSection() when the section's
 * entry in the world's cWScriptCache has a state of 2 or more and a data word (+0x8), otherwise
 * cWScriptMan_purgeSection().
 */
void cWScriptMan::initMissions()
{
    int i = 0;
    cProcessList_deleteAll((char*)&ActiveList);
    cProcessList_deleteAll((char*)&AddList);
    cProcessList_deleteAll((char*)&IdleList);
    init();
    int n = cWorldView_getNumSections(&World->m_pWorldCache->m_WorldView);
    for (; i < n; i++) {
        cWScriptCacheEntry* tbl = World->m_pWScriptCache->mpCache;
        int ok = tbl[i].mState >= 2 && tbl[i].mpScriptHeader != 0;
        if (ok) {
            activeSection(i);
        } else {
            purgeSection(i);
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_processLocalMissionCmd);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cWScriptMan_updateWorld(cWScriptMan* self);
extern "C" void cWScriptMan_doMissionCmd(void* self, int id, int a2, int a3);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_BigChallengeBegin(void* snd, int a1);
extern "C" void cSSXAudio_BigChallengeFailed(void* snd, int a1);
extern "C" void cReplay_startRace(void* p);
extern "C" void cPlaybackMan_startPreview(void* p);

/**
 * Carries out the local mission request queued by the menus, then clears it.
 *
 * Switches on the request at +0x2AC (eWScriptLocalMissionCmd) for the mission saved in +0x2B0: kLocalMissionCmd_Start
 * (cWScriptMan_startLocalMission) updates the world, sends kMissionCmd_Start, plays the big-challenge start audio
 * (cSSXAudio_BigChallengeBegin) and calls cReplay_startRace; kLocalMissionCmd_Restart (cWScriptMan_restartLocalMission)
 * sends kMissionCmd_Restart; kLocalMissionCmd_Cancel (cWScriptMan_cancelLocalMission) and kLocalMissionCmd_Quit
 * (cWScriptMan_quitLocalMission) send kMissionCmd_End and kMissionCmd_Fail, update the world and play the failure audio
 * (cSSXAudio_BigChallengeFailed); kLocalMissionCmd_Preview (cWScriptMan_previewLocalMission) starts the playback
 * preview (cPlaybackMan_startPreview), sends kMissionCmd_Start for rider 1 and updates the world;
 * kLocalMissionCmd_CancelPreview (cWScriptMan_cancelPreviewLocalMission) sends kMissionCmd_End and updates the world.
 * The request is then cleared (kLocalMissionCmd_None, mission kWScriptId_None). Mission commands go through
 * cWScriptMan_doMissionCmd.
 *
 * @param vself  the world script manager (this)
 */
// Runs the pending gate action (replay, sounds, camera) and clears it.
void cWScriptMan::processLocalMissionCmd()
{
    switch (LocalMissionCmd) {
    case kLocalMissionCmd_Start:
        updateWorld();
        cWScriptMan_doMissionCmd(((char*)this), (int)LocalMissionCmdId, kMissionCmd_Start, 0);
        cSSXAudio_BigChallengeBegin(cAudioInterface_bxAudio(), (int)LocalMissionId);
        cReplay_startRace(SSXApp->Game->Replay);
        break;
    case kLocalMissionCmd_Restart:
        cWScriptMan_doMissionCmd(((char*)this), (int)LocalMissionCmdId, kMissionCmd_Restart, 0);
        break;
    case kLocalMissionCmd_Cancel:
        cWScriptMan_doMissionCmd(((char*)this), (int)LocalMissionCmdId, kMissionCmd_End, 0);
        updateWorld();
        cSSXAudio_BigChallengeFailed(cAudioInterface_bxAudio(), 1);
        break;
    case kLocalMissionCmd_Quit:
        cWScriptMan_doMissionCmd(((char*)this), (int)LocalMissionCmdId, kMissionCmd_Fail, 0);
        updateWorld();
        cSSXAudio_BigChallengeFailed(cAudioInterface_bxAudio(), 1);
        break;
    case kLocalMissionCmd_Preview:
        cPlaybackMan_startPreview(SSXApp->Game->PlaybackMan);
        cWScriptMan_doMissionCmd(((char*)this), (int)LocalMissionCmdId, kMissionCmd_Start, 1);
        updateWorld();
        break;
    case kLocalMissionCmd_CancelPreview:
        cWScriptMan_doMissionCmd(((char*)this), (int)LocalMissionCmdId, kMissionCmd_End, 0);
        updateWorld();
        break;
    }
    LocalMissionCmd = kLocalMissionCmd_None;
    LocalMissionCmdId = kWScriptId_None;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_update);
#ifdef SKIP_ASM
class cWScriptProcess_update_cProc;
struct cWScriptProcList;
extern "C" void cWScriptProcess_update(cWScriptProcess_update_cProc* self);
extern "C" void cWScriptProcess_dispatchProcessEvent(cWScriptProcess* proc, tWScriptEvent* cmd);
extern "C" void cWScriptProcess_dispatchTaskEvent(cWScriptProcess* self, int a1);
extern "C" void cWScriptMan_processLocalMissionCmd(cWScriptMan* self);
extern "C" void cWScriptMan_startProcess(void* self, void* p);
extern "C" void* cProcessList_removeHead(void* self);
int cProcessList_addProcess(void* self, void* a1);

struct sRefSet_309270 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};
struct sRefWorld_309270 {
    char pad_0x0[0x8];
    sRefSet_309270** sets;  // 0x8
};


static inline void* refToPtr_309270(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* toObj_309270(unsigned int p)
{
    if (p == 0)
        return 0;
    return refToPtr_309270(p);
}

static inline void* lookup_309270(sRefWorld_309270* w, unsigned int id)
{
    sRefSet_309270* set = w->sets[id & 0xFF];
    if (set == 0) {
        return 0;
    }
    return toObj_309270(set->refs[id >> 8] >> 8);
}

/**
 * Runs the world script manager for one frame.
 *
 * Runs cWScriptMan_processLocalMissionCmd and clears the per-frame counters. Each process in the pending list (+0x2BC)
 * is moved to the idle list and started. The event list at +0x40 is taken and emptied, and each event (with the
 * instance for its id looked up when its low 16 flag bits are 0) is passed to every active process:
 * cWScriptProcess_dispatchProcessEvent for process events (0x20000) and cWScriptProcess_dispatchTaskEvent for task
 * events (0x10000). Finally every active process is updated with cWScriptProcess_update, each made current in turn.
 */
void cWScriptMan::update()
{
    processLocalMissionCmd();
    MissionPointsEnable = 0;
    MissionCountEnable = 0;
    MissionTicksEnable = 0;
    MissionHeightEnable = 0;
    MissionTrickEnable = 0;
    void* process;
    while ((process = cProcessList_removeHead((char*)&AddList)) != 0) {
        cProcessList_addProcess((char*)&IdleList, process);
        cWScriptMan_startProcess(this, process);
    }
    tWScriptEventList list = *(tWScriptEventList*)(Events);
    *(int*)(Events) = 0;
    for (int i = 0; i < list.NumEvents; i++) {
        tWScriptEvent* event = &list.Events[i];
        if ((unsigned short)event->EventId == 0)
            Inst = (tInstanceRecord*)lookup_309270(*(sRefWorld_309270**)World, event->unk_04);
        char* next = *(char**)&ActiveList;
        while (next != 0) {
            cWScriptProcess* proc = (cWScriptProcess*)next;
            next = (char*)proc->Next;
            CurrentProcess = (cWScriptProcess*)proc;
            if (event->EventId & 0x20000)
                proc->dispatchProcessEvent(event);
            if (event->EventId & 0x10000)
                proc->dispatchTaskEvent(event);
        }
        Inst = 0;
        CurrentProcess = 0;
    }
    char* next = *(char**)&ActiveList;
    while (next != 0) {
        cWScriptProcess* proc = (cWScriptProcess*)next;
        next = (char*)proc->Next;
        CurrentProcess = (cWScriptProcess*)proc;
        cWScriptProcess_update((cWScriptProcess_update_cProc*)proc);
    }
    CurrentProcess = 0;
}
#endif


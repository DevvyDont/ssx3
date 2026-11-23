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

struct sElem_307598 {
    char pad[0x44];
    // PORT: the element constructor is cWScriptTask_cWScriptTask (defined below as an extern "C" function).
    sElem_307598() __asm__("cWScriptTask_cWScriptTask");
    void operator delete[](void* p, unsigned int size);
};


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
    self->Manager = (cWScriptMan*)man;
    self->ProcessId = (unsigned int)((int)def->ProcessId);
    self->Local = 0;
    for (int i = 0; i < 3; i++)
        self->ProcessScriptCode[i] = (char*)cWScriptMan_getScriptCode_2(self->Manager, ((int*)&def->ProcessScriptId[0])[i]);
    for (int j = 0; j < 10; j++)
        self->EventScriptCode[j] = (char*)cWScriptMan_getScriptCode_2(self->Manager, ((int*)&def->EventScriptId[0])[j]);
    int n = def->NumTasks;
    self->NumTasks = n;
    sElem_307598** pv = (sElem_307598**)&self->Tasks;
    *pv = new (str_WScriptTasks, 0x20000000, 0) sElem_307598[n];
    char* ed = ((char*)def->unk_008);
    for (int k = 0; k < self->NumTasks; k++, ed += 0x38)
        cWScriptTask_initTask((cWScriptTask*)&((sElem_307598*)self->Tasks)[k], self, (cWScriptTask_initTask_sDef*)ed);
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
 * State (+0x0) 1 becomes 2 and falls through; in state 2 the current task (index +0x8, when below the task
 * count +0x4) gets its virtual update (slot 2, cWScriptTask_update), and if the state is still 2 afterwards the
 * process's virtual slot 5 (cWScriptProcess_processUpdate) runs. Other states do nothing. cWScriptMan_update calls it
 * for every active process.
 */
extern "C" void cWScriptProcess_update(cWScriptProcess_update_cProc* self)
{
    switch (self->state) {
    case 0:
        break;
    case 1:
        self->state = 2;
    case 2: {
        int st = self->state;
        if (st == 2) {
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
 * The event's first halfword picks the handler: 0-9 call vtable slots 7-16 (cWScriptProcess_eventOnCollide, OnJump,
 * OnLand, OnTrick, OnWipeout, OnReset, OnCollect, OnEvent1, OnEvent2, OnEvent3). For a trick (3) the manager's (+0x10)
 * TrickDef (+0x29C) points at the event's data (+0x4) during the call and is cleared after. Other ids are ignored.
 * cWScriptMan_update calls it for events flagged 0x20000.
 *
 * @param vself  the process
 * @param vmsg  the event (id halfword at +0x0, data from +0x4)
 */
// Dispatches a script event to the matching virtual handler (slots 7-16).
extern "C" void cWScriptProcess_dispatchProcessEvent(cWScriptProcess* vself, tWScriptEvent* vmsg)
{
    unsigned short* msg = (unsigned short*)vmsg;
    switch (*msg) {
    case 0:
        VCall_003078C0(((char*)vself), 7);
        break;
    case 1:
        VCall_003078C0(((char*)vself), 8);
        break;
    case 2:
        VCall_003078C0(((char*)vself), 9);
        break;
    case 3:
        vself->Manager->TrickDef = (tTrickNameDef*)(msg + 2);
        VCall_003078C0(((char*)vself), 10);
        vself->Manager->TrickDef = 0;
        break;
    case 4:
        VCall_003078C0(((char*)vself), 11);
        break;
    case 5:
        VCall_003078C0(((char*)vself), 12);
        break;
    case 6:
        VCall_003078C0(((char*)vself), 13);
        break;
    case 7:
        VCall_003078C0(((char*)vself), 14);
        break;
    case 8:
        VCall_003078C0(((char*)vself), 15);
        break;
    case 9:
        VCall_003078C0(((char*)vself), 16);
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
extern "C" unsigned int cWScriptProcess_getRunningTaskId(cWScriptProcess* self)
{
    int i = self->RunningTask;
    if (i < self->NumTasks) {
        return (self->Tasks)[i].TaskId;
    }
    return 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_dispatchTaskEvent);
#ifdef SKIP_ASM
class cWScriptProcess_dispatchTaskEvent_cObj {
public:
    char pad_0x00[0x40];
    // vptr at 0x40; sizeof == 0x44 (same layout as cWScriptProcEntry)
    virtual void v01();
    virtual void v02();
    virtual void dispatchTaskEvent(int a1); // 0x18
};

/**
 * Passes a task event to the process's current task (index +0x8) through its virtual slot 3
 * (cWScriptTask_dispatchTaskEvent), if the index is below the task count (+0x4).
 *
 * cWScriptMan_update calls it for events flagged 0x10000.
 *
 * @param event  the event
 */
extern "C" void cWScriptProcess_dispatchTaskEvent(cWScriptProcess* self, int event)
{
    int i = self->RunningTask;
    if (i < self->NumTasks) {
        ((cWScriptProcess_dispatchTaskEvent_cObj*)self->Tasks)[i].dispatchTaskEvent(event);
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
extern "C" void cWScriptProcess_eventOnCollide(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[0];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnJump(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[1];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnLand(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[2];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnTrick(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[3];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnWipeout(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[4];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnReset(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[5];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnCollect(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[6];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnEvent1(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[7];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnEvent2(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[8];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_eventOnEvent3(cWScriptProcess* self)
{
    void* script = self->EventScriptCode[9];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_processStart(cWScriptProcess* self)
{
    if (((void*)self->Local) != 0) {
        luno_cLunoTable__cLunoTable(((void*)self->Local), 3);
    }
    (*(void**)&self->Local) = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_ProcessLocalTable, 0x20000000, 0), 8);
    self->State = 1;
    self->RunningTask = 0;
    if (((void*)self->ProcessScriptCode[0]) != 0) {
        cWScriptMan_runScriptCode(((void*)self->Manager), self, ((void*)self->ProcessScriptCode[0]));
    }
    cWScriptMan_processStart(((void*)self->Manager), self);
    if (self->RunningTask < self->NumTasks) {
        self->Tasks[self->RunningTask].taskStart();
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
extern "C" void cWScriptProcess_processUpdate(cWScriptProcess* self)
{
    void* script = self->ProcessScriptCode[1];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
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
extern "C" void cWScriptProcess_processEnd(cWScriptProcess* self)
{
    *(int*)self = 3;
    void* script = self->ProcessScriptCode[2];
    if (script != 0) {
        cWScriptMan_runScriptCode(self->Manager, self, script);
    }
    cWScriptProcess_end(self);
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
extern "C" void cWScriptProcess_end(cWScriptProcess* self)
{
    *(int*)self = 3;
    cWScriptMan_processEnd(self->Manager, self);
    void* obj = self->Local;
    if (obj != 0) {
        luno_cLunoTable__cLunoTable(obj, 3);
    }
    self->Local = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_processComplete);
#ifdef SKIP_ASM
int cWScriptMan_isProcessActive(void* man, void* proc);


/**
 * Completes a world-script process if its manager still has it on the running list.
 *
 * cWScriptMan_isProcessActive() looks for a process with the same id (+0x20) on the running list; when there is one, this
 * calls its own virtual slot 6, which for cWScriptProcess (cWScriptProcess_processEnd) sets the state to 3, runs the script
 * at +0x30 and calls cWScriptProcess_end(). The task stepper cWScriptProcess_endTask calls it after the last task.
 */
extern "C" void cWScriptProcess_processComplete(cWScriptProcess* self)
{
    if (cWScriptMan_isProcessActive(self->Manager, self) != 0) {
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)self->mVtbl;
        vt[6].fn(vthis(self, vt[6]));
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
 * Does nothing once the process has ended (state 3). Otherwise calls virtual slot 6 of the current task (index +0x8
 * into the 0x44-byte tasks at +0x14); if another task follows (count at +0x4) it becomes current and gets its virtual
 * slot 4, else the process's own virtual slot 2 runs.
 */
extern "C" void cWScriptProcess_endTask(cWScriptProcess_endTask_cProc* self)
{
    if (self->state == 3) {
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
extern "C" void cWScriptProcess_gotoTask(cWScriptProcess* self, int id)
{
    int i;
    for (i = 0; i < self->NumTasks; i++) {
        if (self->Tasks[i].TaskId == id) {
            if (self->RunningTask < self->NumTasks) {
                self->Tasks[self->RunningTask].State = 3;
            }
            self->RunningTask = i;
            self->Tasks[i].taskStart();
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
 * Like cWScriptProcess_gotoTask, but the current task (index +0x8, when valid) is ended normally through its virtual slot 6
 * (cWScriptTask_taskEnd) instead of being aborted; the found task becomes current and gets slot 4
 * (cWScriptTask_taskStart). An unknown id does nothing. cWScriptMan_advanceToTask calls it on the manager's current process.
 *
 * @param id  the task id to advance to
 */
extern "C" void cWScriptProcess_advanceToTask(cWScriptProcess* self, int id)
{
    int i;
    for (i = 0; i < self->NumTasks; i++) {
        if (self->Tasks[i].TaskId == id) {
            if (self->RunningTask < self->NumTasks) {
                self->Tasks[self->RunningTask].taskEnd();
            }
            self->RunningTask = i;
            self->Tasks[i].taskStart();
            return;
        }
    }
}
#endif

extern "C" void* cWScriptMan_getScriptCode(int);

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_getScriptCode__FPv);
#ifdef SKIP_ASM
/**
 * Returns the script code for a resource id.
 *
 * Forwards to cWScriptMan_getScriptCode with the process's manager (+0x10); the id in the second argument is passed
 * through untouched. The declaration here takes only the process; callers use the cWScriptProcess_getScriptCode__FPv alias.
 *
 * @return the code, or 0 when the id is invalid
 */
void* cWScriptProcess_getScriptCode(void* vself)
{
    cWScriptProcess* self = (cWScriptProcess*)vself;
    return cWScriptMan_getScriptCode((int)self->Manager);
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
 * the process has a local table (+0x1C) and, if so, the table with luno_cLunoTable_writeToReplayFrame against that base, then each task
 * with cWScriptTask_writeToReplayFrame. cWScriptMan_writeReplayProcessList calls it for each process in a list.
 *
 * @param stream  the replay frame
 */
extern "C" void cWScriptProcess_writeToReplayFrame(cWScriptProcess* self, cBxReplayFrame* stream)
{
    stream->write((const char*)self, 0x10);
    int x = cWScriptCache_getScriptBase(cWorld_m_pTheWorld->m_pWScriptCache, (int)self->SectionId);
    int has = self->Local != 0;
    stream->write((const char*)&has, 4);
    if (has) {
        luno_cLunoTable_writeToReplayFrame(self->Local, stream, x);
    }
    int i;
    for (i = 0; i < self->NumTasks; i++) {
        cWScriptTask_writeToReplayFrame(&self->Tasks[i], stream);
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
 * reads each task with cWScriptTask_readFromReplayFrame. cWScriptMan_readReplayProcessList calls it for each process in a list.
 *
 * @param stream  the replay frame
 */
extern "C" void cWScriptProcess_readFromReplayFrame(cWScriptProcess* self, cBxReplayFrame* stream)
{
    stream->read((char*)self, 0x10);
    int x = cWScriptCache_getScriptBase(cWorld_m_pTheWorld->m_pWScriptCache, (int)self->SectionId);
    if (self->Local != 0) {
        luno_cLunoTable__cLunoTable(self->Local, 3);
    }
    self->Local = 0;
    int has;
    stream->read((char*)&has, 4);
    if (has) {
        void* localTable = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_ProcessLocalTable, 0x20000000, 0), 8);
        self->Local = (cLunoTable*)localTable;
        luno_cLunoTable_readFromReplayFrame(localTable, stream, x);
    }
    int i;
    for (i = 0; i < self->NumTasks; i++) {
        cWScriptTask_readFromReplayFrame(&self->Tasks[i], stream);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_cWScriptTask);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);
extern void* cWScriptTask_vtbl[];

/**
 * Constructs an empty script task: stores cWScriptTask_vtbl (vptr at +0x40) and zeroes its state, id and scripts.
 *
 * Clears the three task scripts (+0x8..+0x13), the process (+0x3C), the id (+0x4) and the state (+0x0).
 * cWScriptProcess_cWScriptProcess builds its task array with it before cWScriptTask_initTask fills each task.
 *
 * @return @p self
 */
extern "C" void* cWScriptTask_cWScriptTask(cWScriptTask* self)
{
    VPTR_OF(cWScriptTask, self) = cWScriptTask_vtbl;
    MEM_fill((char*)&self->TaskScriptCode, 0, 0xC);
    self->Process = 0;
    self->TaskId = (unsigned int)0;
    *(int*)self = 0;
    return ((void*)self);
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
 * Stores @p owner, copies the task id from the data's first word to +0x4 (cWScriptProcess_gotoTask finds tasks by it), and
 * turns the data's 3 + 10 script resource ids into code pointers with cWScriptMan_getScriptCode() through the
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
 * Runs one frame of a world-script task: a started task (state 1) becomes running (2), and a running task runs its
 * update script (slot 5, taskUpdate).
 */
extern "C" void cWScriptTask_update(cWScriptTask_update_cProcess* self)
{
    switch (self->state) {
    case 0:
        break;
    case 1:
        self->state = 2;
        self->taskUpdate();
        break;
    case 2:
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
 * Passes a world-script event to a running task (state 2) as the matching event virtual.
 *
 * Event types 0-9 call slots 7-16: on collide, jump, land, trick, wipeout, reset, collect and event 1-3. For a
 * trick the event's data (+0x4) is put in the script manager's word +0x29C while the handler runs.
 *
 * @param vself  the task (On Tour this)
 * @param vmsg  the event (On Tour const tWScriptEvent& event): type in the first halfword
 */
// Same event dispatch as cWScriptProcess_dispatchProcessEvent for a running (state 2) sub-process.
extern "C" void cWScriptTask_dispatchTaskEvent(cWScriptTask* vself, void* vmsg)
{
    unsigned short* msg = (unsigned short*)vmsg;
    if (*(int*)vself != 2) {
        return;
    }
    switch (*msg) {
    case 0:
        VCall_003083A0(((char*)vself), 7);
        break;
    case 1:
        VCall_003083A0(((char*)vself), 8);
        break;
    case 2:
        VCall_003083A0(((char*)vself), 9);
        break;
    case 3:
        vself->Process->Manager->TrickDef = (tTrickNameDef*)(msg + 2);
        VCall_003083A0(((char*)vself), 10);
        vself->Process->Manager->TrickDef = 0;
        break;
    case 4:
        VCall_003083A0(((char*)vself), 11);
        break;
    case 5:
        VCall_003083A0(((char*)vself), 12);
        break;
    case 6:
        VCall_003083A0(((char*)vself), 13);
        break;
    case 7:
        VCall_003083A0(((char*)vself), 14);
        break;
    case 8:
        VCall_003083A0(((char*)vself), 15);
        break;
    case 9:
        VCall_003083A0(((char*)vself), 16);
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
extern "C" void cWScriptTask_eventOnCollide(cWScriptTask* self)
{
    void* script = self->EventScriptCode[0];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnJump(cWScriptTask* self)
{
    void* script = self->EventScriptCode[1];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnLand(cWScriptTask* self)
{
    void* script = self->EventScriptCode[2];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnTrick(cWScriptTask* self)
{
    void* script = self->EventScriptCode[3];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnWipeout(cWScriptTask* self)
{
    void* script = self->EventScriptCode[4];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnReset(cWScriptTask* self)
{
    void* script = self->EventScriptCode[5];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnCollect(cWScriptTask* self)
{
    void* script = self->EventScriptCode[6];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnEvent1(cWScriptTask* self)
{
    void* script = self->EventScriptCode[7];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnEvent2(cWScriptTask* self)
{
    void* script = self->EventScriptCode[8];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_eventOnEvent3(cWScriptTask* self)
{
    void* script = self->EventScriptCode[9];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_taskStart);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Starts the task once: sets its state to 1 (started) and runs its start script (TaskScriptCode[0], +0x8) when it
 * has one.
 */
extern "C" void cWScriptTask_taskStart(cWScriptTask* self)
{
    if (*(int*)self != 1) {
        *(int*)self = 1;
        void* script = self->TaskScriptCode[0];
        if (script != 0) {
            cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
extern "C" void cWScriptTask_taskUpdate(cWScriptTask* self)
{
    void* script = self->TaskScriptCode[1];
    if (script != 0) {
        cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_taskEnd);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode1(void* man, void* proc, void* script);

/**
 * Ends the task once: sets its state to 3 (ended) and runs its end script (TaskScriptCode[2], +0x10) when it has
 * one.
 */
extern "C" void cWScriptTask_taskEnd(cWScriptTask* self)
{
    if (*(int*)self != 3) {
        *(int*)self = 3;
        void* script = self->TaskScriptCode[2];
        if (script != 0) {
            cWScriptMan_runScriptCode1(self->Process->Manager, self, script);
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
 * @param task  the task
 * @param obj  the replay frame
 * @return the frame call's result
 */
extern "C" void cWScriptTask_writeToReplayFrame(cWScriptTask* task, cBxReplayFrame* obj)
{
    obj->write((const char*)task, 4);
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
 * @param task  the task
 * @param obj  the replay frame
 * @return the frame call's result
 */
extern "C" void cWScriptTask_readFromReplayFrame(cWScriptTask* task, cBxReplayFrame* obj)
{
    obj->read((char*)task, 4);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_cWScriptMan);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void tMissionMenuQueue_tMissionMenuQueue(void* p);
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
 * Sets +0x2C to -1, constructs the mission menu queue at +0x1C4, sets the vtable cWScriptMan_vtbl at +0x494, constructs the
 * idle (+0x2B4), active (+0x2B8) and pending (+0x2BC) process lists and the collect pool at +0x2C0, stores this in
 * cWScriptMan_TheWScriptMan, takes cWorld_m_pTheWorld as its world (+0x28C), zeroes the 50 section table pointers at
 * +0x3CC and runs cWScriptMan_init.
 *
 * @return this
 */
extern "C" void* cWScriptMan_cWScriptMan(cWScriptMan* self)
{
    self->MissionHeightInstId = 0xFFFFFFFF;
    tMissionMenuQueue_tMissionMenuQueue(&self->MissionMenuQueue);
    self->mVtbl = cWScriptMan_vtbl;
    cProcessList_cProcessList((char*)&self->IdleList);
    cProcessList_cProcessList((char*)&self->ActiveList);
    cProcessList_cProcessList((char*)&self->AddList);
    char* collectPool = (char*)self->CollectPools;
    int i;
    for (i = 0; i != -1; i--) {
        char* next = collectPool + 0x10C;
        unsigned int* poolSlot = (unsigned int*)(collectPool + 0xC);
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
    cWScriptMan_init(self);
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
 * Restores the vtable cWScriptMan_vtbl, runs cWScriptMan_purgeGlobalTables, clears cWScriptMan_TheWScriptMan, destroys the pending, active
 * and idle process lists (cProcessList__cProcessList, flag 2) and calls operator delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
extern "C" void cWScriptMan__cWScriptMan(cWScriptMan* self, int flags)
{
    self->mVtbl = cWScriptMan_vtbl;
    cWScriptMan_purgeGlobalTables(self);
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
 * +0x2B0) and several counters to -1, empties the menu queue (tMissionMenuQueue_init) and the event list at +0x44, and runs
 * cWScriptMan_rebuildGlobalTables.
 */
extern "C" void cWScriptMan_init(cWScriptMan* self)
{
    self->Inst = 0;
    self->Being = 0;
    self->CollideInfo = 0;
    self->IntersectInfo = 0;
    self->CurrentProcess = 0;
    self->CurrentTask = 0;
    self->TrickDef = 0;
    self->LocalMissionCmd = 0;
    self->MissionPointsEnable = 0;
    self->MissionPoints = 0;
    self->MissionCountEnable = 0;
    self->MissionCount = 0;
    self->MissionCountMax = 0;
    self->MissionTicksEnable = 0;
    self->MissionTicks = 0;
    self->MissionHeightEnable = 0;
    self->MissionHeightBeing = 0;
    self->MissionTrickEnable = 0;
    self->LocalMissionId = 0xFFFFFFFF;
    self->LocalMissionCmdId = 0xFFFFFFFF;
    self->MissionPointsGoal = -1;
    self->MissionTicksDir = -1;
    self->MissionHeightInstId = 0xFFFFFFFF;
    MEM_fill((char*)self->MissionTrick, 0, 8);
    tMissionMenuQueue_init(&self->MissionMenuQueue);
    (*(int*)((char*)self + 0x40)) = 0;
    MEM_fill(&self->Events[4], 0, 0x180);
    cWScriptMan_rebuildGlobalTables(self);
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
extern "C" void cWScriptMan_purgeGlobalTables(cWScriptMan* self)
{
    void** objs = (void**)self->GlobalTables;
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

struct cWScriptMan_rebuildGlobalTables_sSection {
    short type;
    short pad2;
    int f4;
    int f8;
};

/**
 * Recreates the global script tables for the loaded world sections.
 *
 * Runs cWScriptMan_purgeGlobalTables, then for every section of the world view whose script cache entry has reached state 2 and has
 * data, calls cWScriptMan_allocateGlobalTable with the section index.
 */
extern "C" void cWScriptMan_rebuildGlobalTables(cWScriptMan* self)
{
    int i = 0;
    cWScriptMan_purgeGlobalTables(self);
    int n = cWorldView_getNumSections(&self->World->m_pWorldCache->m_WorldView);
    for (; i < n; i++) {
        cWScriptMan_rebuildGlobalTables_sSection* tbl = (cWScriptMan_rebuildGlobalTables_sSection*)self->World->m_pWScriptCache->mpCache;
        int ok = tbl[i].type >= 2 && tbl[i].f8 != 0;
        if (ok) {
            cWScriptMan_allocateGlobalTable_impl(self, i);
        }
    }
}
#endif

extern "C" void* cWScriptMan_allocateGlobalTable(void* self);

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_loadSection__FPv);
#ifdef SKIP_ASM
/**
 * Prepares a loaded world section's scripts: allocates the section's global Luno table with cWScriptMan_allocateGlobalTable.
 *
 * The section index arrives in the second argument register and is passed through untouched (the C here declares
 * only @p self). gameload calls it on cWScriptMan_TheWScriptMan once a section's scripts are loaded.
 *
 * @return cWScriptMan_allocateGlobalTable's result (the new table)
 */
void* cWScriptMan_loadSection(void* self)
{
    return cWScriptMan_allocateGlobalTable(self);
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
 * The table array starts at +0x3CC (cWScriptMan_getGlobalTable reads it); the new table is a 4-byte "cLunoTable" allocation
 * (flags 0x20000000) constructed with 0x40. No bounds check (On Tour asserts sectionId < kMaxGlobalTables).
 * cWScriptMan_rebuildGlobalTables calls it for every section whose script data is loaded, cWScriptMan_loadSection for one.
 *
 * @param i  the world section index
 * @return the new table
 */
void* cWScriptMan_allocateGlobalTable_impl(void* vself, int i)
{
    cWScriptMan* self = (cWScriptMan*)vself;
    char* base = (char*)&self->GlobalTables[0];
    void** slot = (void**)(base + (i << 2));
    if (*slot != 0) {
        luno_cLunoTable__cLunoTable(*slot, 3);
    }
    return *slot = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_cLunoTable_b, 0x20000000, 0), 0x40);
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
extern "C" void cWScriptMan_activeSection(cWScriptMan* self, int section)
{
    int n = cWorld_getNumProcessScripts(self->World, section);
    cWScriptMan_activeSection_sDef* defs = (cWScriptMan_activeSection_sDef*)cWorld_getProcessScripts(self->World, section);
    int i;
    for (i = 0; i < n; i++) {
        cWScriptMan_addProcess(self, section, &defs[i], 0);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_setupMissions);
#ifdef SKIP_ASM
struct sWSPDef8C60 {
    char pad[0x54];
};

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
 * Sets up the world's missions: in game mode 4 with one player, creates a mission process for every mission
 * script of every world section.
 *
 * First releases the playback manager's loaded data (cPlaybackMan_purgeLocation). When cGameMode (newRace.cGameMode) is 4 and the
 * AI's NumberOfPlayersAllocated is 1, it adds a cWScriptMission (cWScriptMan_addProcess1) per 0x54-byte
 * mission script, keyed (section << 16) + section, and calls cPlaybackMan_initLocation() for the race
 * location's data (+0x34). cWScriptMan_updateWorld() always runs last: it runs each mission script's code at +0x4C and
 * clears the manager's mission state.
 */
extern "C" void cWScriptMan_setupMissions(cWScriptMan* self)
{
    void* iface;
    cPlaybackMan_purgeLocation(wsPlayback8C60());
    iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameMode == 4 && SSXApp->Game->AI->NumberOfPlayersAllocated == 1) {
        int i = 0;
        int n = cWorldView_getNumSections(&self->World->m_pWorldCache->m_WorldView);
        for (; i < n; i++) {
            int cnt = cWorld_getNumMissionScripts(self->World, i);
            sWSPDef8C60* defs = (sWSPDef8C60*)cWorld_getMissionScripts(self->World, i);
            int j;
            for (j = 0; j < cnt; j++) {
                cWScriptMan_addProcess1(self, i, &defs[j], (i << 16) + i, 0);
            }
        }
        cPlaybackMan_initLocation(wsPlayback8C60(), cBENewRaceInterface_getLocation(iface)->fileName);
    }
    cWScriptMan_updateWorld(self);
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
extern "C" void cWScriptMan_updateWorld(cWScriptMan* self)
{
    int i = 0;
    int n = cWorldView_getNumSections(&self->World->m_pWorldCache->m_WorldView);
    for (; i < n; i++) {
        int cnt = cWorld_getNumMissionScripts(self->World, i);
        sWSPDef8DB8* defs = (sWSPDef8DB8*)cWorld_getMissionScripts(self->World, i);
        int j;
        for (j = 0; j < cnt; j++) {
            if (defs[j].f0 != (int)self->LocalMissionId) {
                void* table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
                int scriptCode = cWScriptMan_getScriptCode_2(self, defs[j].f4C);
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
    self->Inst = 0;
    self->CollideInfo = 0;
    self->IntersectInfo = 0;
    self->CurrentProcess = 0;
    self->CurrentTask = 0;
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
extern "C" void cWScriptMan_purgeMissions(cWScriptMan* self)
{
    cProcessList_deleteAll((char*)&self->ActiveList);
    cProcessList_deleteAll((char*)&self->AddList);
    cProcessList_deleteAll((char*)&self->IdleList);
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
extern "C" void cWScriptMan_purgeSection(cWScriptMan* self, int section)
{
    cProcessList_unloadSection((char*)&self->IdleList, section);
    cProcessList_unloadSection((char*)&self->ActiveList, section);
    cProcessList_unloadSection((char*)&self->AddList, section);
    int off = section << 2;
    char* base = (char*)self->GlobalTables;
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
extern "C" void cWScriptMan_unloadSection(cWScriptMan* self, int i)
{
    cWScriptMan_purgeSection(self, i);
    cWScriptMan_unloadSection_sEntry* items =
        (cWScriptMan_unloadSection_sEntry*)self->World->m_pWScriptCache->mpCache;
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

struct cWScriptMan_initMissions_sSection {
    short type;
    short pad2;
    int f4;
    int f8;
};

/**
 * Rebuilds the world-script processes: deletes every process, resets the manager and re-adds the processes of
 * each ready world section.
 *
 * Deletes the processes on the three lists (+0x2B8, +0x2BC, +0x2B4; cProcessList_deleteAll), resets the manager's
 * state (cWScriptMan_init), then for each world section calls cWScriptMan_activeSection() when the section's
 * entry in the world's cWScriptCache has a state of 2 or more and a data word (+0x8), otherwise
 * cWScriptMan_purgeSection().
 */
extern "C" void cWScriptMan_initMissions(cWScriptMan* self)
{
    int i = 0;
    cProcessList_deleteAll((char*)&self->ActiveList);
    cProcessList_deleteAll((char*)&self->AddList);
    cProcessList_deleteAll((char*)&self->IdleList);
    cWScriptMan_init(self);
    int n = cWorldView_getNumSections(&self->World->m_pWorldCache->m_WorldView);
    for (; i < n; i++) {
        cWScriptMan_initMissions_sSection* tbl = (cWScriptMan_initMissions_sSection*)self->World->m_pWScriptCache->mpCache;
        int ok = tbl[i].type >= 2 && tbl[i].f8 != 0;
        if (ok) {
            cWScriptMan_activeSection(self, i);
        } else {
            cWScriptMan_purgeSection(self, i);
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
 * Switches on the request at +0x2AC for the mission saved in +0x2B0: 1 (cWScriptMan_startLocalMission) updates the world, sends mission
 * command 1, plays the big-challenge start audio (cSSXAudio_BigChallengeBegin) and calls cReplay_startRace; 2
 * (cWScriptMan_restartLocalMission) sends command 3; 3 (cWScriptMan_cancelLocalMission) and 4
 * (cWScriptMan_quitLocalMission) send commands 18 and 5, update the world and play the failure audio
 * (cSSXAudio_BigChallengeFailed); 5 (cWScriptMan_previewLocalMission) starts the playback preview (cPlaybackMan_startPreview), sends
 * command 1 with argument 1 and updates the world; 6 (cWScriptMan_cancelPreviewLocalMission) sends 18 and updates the world. The request is
 * then cleared (0, mission -1). Mission commands go through cWScriptMan_doMissionCmd.
 *
 * @param vself  the world script manager (this)
 */
// Runs the pending gate action (replay, sounds, camera) and clears it.
extern "C" void cWScriptMan_processLocalMissionCmd(cWScriptMan* vself)
{
    switch (vself->LocalMissionCmd) {
    case 1:
        cWScriptMan_updateWorld(vself);
        cWScriptMan_doMissionCmd(((char*)vself), (int)vself->LocalMissionCmdId, 1, 0);
        cSSXAudio_BigChallengeBegin(cAudioInterface_bxAudio(), (int)vself->LocalMissionId);
        cReplay_startRace(SSXApp->Game->Replay);
        break;
    case 2:
        cWScriptMan_doMissionCmd(((char*)vself), (int)vself->LocalMissionCmdId, 3, 0);
        break;
    case 3:
        cWScriptMan_doMissionCmd(((char*)vself), (int)vself->LocalMissionCmdId, 18, 0);
        cWScriptMan_updateWorld(vself);
        cSSXAudio_BigChallengeFailed(cAudioInterface_bxAudio(), 1);
        break;
    case 4:
        cWScriptMan_doMissionCmd(((char*)vself), (int)vself->LocalMissionCmdId, 5, 0);
        cWScriptMan_updateWorld(vself);
        cSSXAudio_BigChallengeFailed(cAudioInterface_bxAudio(), 1);
        break;
    case 5:
        cPlaybackMan_startPreview(SSXApp->Game->PlaybackMan);
        cWScriptMan_doMissionCmd(((char*)vself), (int)vself->LocalMissionCmdId, 1, 1);
        cWScriptMan_updateWorld(vself);
        break;
    case 6:
        cWScriptMan_doMissionCmd(((char*)vself), (int)vself->LocalMissionCmdId, 18, 0);
        cWScriptMan_updateWorld(vself);
        break;
    }
    vself->LocalMissionCmd = 0;
    vself->LocalMissionCmdId = 0xFFFFFFFF;
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
 * Runs cWScriptMan_processLocalMissionCmd and clears the per-frame counters. Each process in the pending list (+0x2BC) is moved to the idle
 * list and started. The event list at +0x40 is taken and emptied, and each event (with the instance for its id looked
 * up when its low 16 flag bits are 0) is passed to every active process: cWScriptProcess_dispatchProcessEvent for process events (0x20000) and
 * cWScriptProcess_dispatchTaskEvent for task events (0x10000). Finally every active process is updated with cWScriptProcess_update, each made
 * current in turn.
 */
extern "C" void cWScriptMan_update(cWScriptMan* self)
{
    cWScriptMan_processLocalMissionCmd(self);
    self->MissionPointsEnable = 0;
    self->MissionCountEnable = 0;
    self->MissionTicksEnable = 0;
    self->MissionHeightEnable = 0;
    self->MissionTrickEnable = 0;
    void* process;
    while ((process = cProcessList_removeHead((char*)&self->AddList)) != 0) {
        cProcessList_addProcess((char*)&self->IdleList, process);
        cWScriptMan_startProcess(self, process);
    }
    tWScriptEventList list = *(tWScriptEventList*)(self->Events);
    *(int*)(self->Events) = 0;
    for (int i = 0; i < list.NumEvents; i++) {
        tWScriptEvent* event = &list.Events[i];
        if ((unsigned short)event->EventId == 0)
            self->Inst = (tInstanceRecord*)lookup_309270(*(sRefWorld_309270**)self->World, event->unk_04);
        char* next = *(char**)&self->ActiveList;
        while (next != 0) {
            cWScriptProcess* proc = (cWScriptProcess*)next;
            next = (char*)proc->Next;
            self->CurrentProcess = (cWScriptProcess*)proc;
            if (event->EventId & 0x20000)
                cWScriptProcess_dispatchProcessEvent((cWScriptProcess*)proc, event);
            if (event->EventId & 0x10000)
                cWScriptProcess_dispatchTaskEvent((cWScriptProcess*)(proc), (int)event);
        }
        self->Inst = 0;
        self->CurrentProcess = 0;
    }
    char* next = *(char**)&self->ActiveList;
    while (next != 0) {
        cWScriptProcess* proc = (cWScriptProcess*)next;
        next = (char*)proc->Next;
        self->CurrentProcess = (cWScriptProcess*)proc;
        cWScriptProcess_update((cWScriptProcess_update_cProc*)proc);
    }
    self->CurrentProcess = 0;
}
#endif


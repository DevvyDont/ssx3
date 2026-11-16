#include "common.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_cWScriptProcess);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);
// PORT: cWScriptMan_getScriptCode takes (self, id) (wscriptman); the unit declares one int param.
extern "C" int cWScriptMan_getScriptCode_2(void* self, int id) __asm__("cWScriptMan_getScriptCode");
struct cWScriptTask_initTask_sObj;
struct cWScriptTask_initTask_sDef;
extern "C" void cWScriptTask_initTask(cWScriptTask_initTask_sObj* self, void* owner, cWScriptTask_initTask_sDef* def);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char cWScriptProcess_vtbl[];
extern char str_WScriptTasks[];

struct sElem_307598 {
    char pad[0x44];
    // PORT: the element constructor is func_00308228 (defined below as an extern "C" function).
    sElem_307598() __asm__("func_00308228");
    void operator delete[](void* p, unsigned int size);
};

struct sDef_307598 {
    int f0;
    int count;          // 0x4
    char* elemDefs;     // 0x8
    int a[3];           // 0xC
    int b[10];          // 0x18
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
 * The state (+0x0), current task (+0x18) and local table (+0x1C) start at 0; +0x4 and +0x8 get the task
 * count.
 *
 * @param man  the owning cWScriptMan (On Tour manager)
 * @param sectionId  the world section the process belongs to
 * @param def  the 0x40-byte process definition from the world (On Tour tWScriptProcessData)
 * @return @p self
 */
extern "C" void* cWScriptProcess_cWScriptProcess(void* self, void* man, int sectionId, sDef_307598* def)
{
    *(void**)((char*)self + 0x5C) = cWScriptProcess_vtbl;
    MEM_fill((char*)self + 0x28, 0, 0xC);
    *(int*)((char*)self + 0x24) = sectionId;
    *(void**)((char*)self + 0x10) = man;
    *(int*)((char*)self + 0x20) = def->f0;
    *(int*)((char*)self + 0x1C) = 0;
    for (int i = 0; i < 3; i++)
        ((int*)((char*)self + 0x28))[i] = cWScriptMan_getScriptCode_2(*(void**)((char*)self + 0x10), def->a[i]);
    for (int j = 0; j < 10; j++)
        ((int*)((char*)self + 0x34))[j] = cWScriptMan_getScriptCode_2(*(void**)((char*)self + 0x10), def->b[j]);
    int n = def->count;
    *(int*)((char*)self + 0x4) = n;
    sElem_307598** pv = (sElem_307598**)((char*)self + 0x14);
    *pv = new (str_WScriptTasks, 0x20000000, 0) sElem_307598[n];
    char* ed = def->elemDefs;
    for (int k = 0; k < *(int*)((char*)self + 0x4); k++, ed += 0x38)
        cWScriptTask_initTask((cWScriptTask_initTask_sObj*)&(*(sElem_307598**)((char*)self + 0x14))[k], self, (cWScriptTask_initTask_sDef*)ed);
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x8) = *(int*)((char*)self + 0x4);
    *(int*)((char*)self + 0x18) = 0;
    return self;
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
extern "C" void cWScriptProcess__cWScriptProcess(void* self, int flags)
{
    *(void**)((char*)self + 0x5C) = cWScriptProcess_vtbl;
    char* arr = *(char**)((char*)self + 0x14);
    if (arr != 0) {
        char* task = (char*)(*(int*)(arr - 0x10) * 0x44 + (int)arr);
        while (*(char**)((char*)self + 0x14) != task) {
            task -= 0x44;
            ((cWScriptProcess__cWScriptProcess_cElem*)task)->v01(0);
        }
        cMemMan_free(*(char**)((char*)self + 0x14) - 0x10);
    }
    void* obj = *(void**)((char*)self + 0x1C);
    if (obj != 0) {
        luno_cLunoTable__cLunoTable(obj, 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003077F8);
#ifdef SKIP_ASM
class func_003077F8_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01();
    virtual void v02();
};

class func_003077F8_cProc {
public:
    int state;
    int count;
    int cur;
    char pad0C[0x8];
    func_003077F8_cElem* elems;
    char pad18[0x44];
    // vptr at 0x5C
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

extern "C" void func_003077F8(func_003077F8_cProc* self)
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
                self->elems[self->cur].v02();
            }
            if (self->state == st) {
                self->v05();
            }
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003078C0);
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
    vt[slot].fn(self + vt[slot].delta);
}

// Dispatches a script event to the matching virtual handler (slots 7-16).
extern "C" void func_003078C0(void* vself, void* vmsg)
{
    char* self = (char*)vself;
    unsigned short* msg = (unsigned short*)vmsg;
    switch (*msg) {
    case 0:
        VCall_003078C0(self, 7);
        break;
    case 1:
        VCall_003078C0(self, 8);
        break;
    case 2:
        VCall_003078C0(self, 9);
        break;
    case 3:
        *(unsigned short**)(*(char**)(self + 0x10) + 0x29C) = msg + 2;
        VCall_003078C0(self, 10);
        *(unsigned short**)(*(char**)(self + 0x10) + 0x29C) = 0;
        break;
    case 4:
        VCall_003078C0(self, 11);
        break;
    case 5:
        VCall_003078C0(self, 12);
        break;
    case 6:
        VCall_003078C0(self, 13);
        break;
    case 7:
        VCall_003078C0(self, 14);
        break;
    case 8:
        VCall_003078C0(self, 15);
        break;
    case 9:
        VCall_003078C0(self, 16);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_getRunningTaskId);
#ifdef SKIP_ASM
struct cWScriptProcEntry {
    int a;
    unsigned int id;
    char pad08[0x3C];
};

struct cWScriptProcList {
    int pad00;
    int count;
    int cur;
    int pad0C;
    int pad10;
    cWScriptProcEntry* entries;
};

/**
 * Returns the id of the task the world-script process is running.
 *
 * Reads the id word (+0x4) of the current task when the current index (+0x8) is below the task count (+0x4); the tasks
 * are 0x44-byte cWScriptTask records at +0x14.
 *
 * @return the task id, or 0xFFFFFFFF when no task is running
 */
extern "C" unsigned int cWScriptProcess_getRunningTaskId(cWScriptProcList* self)
{
    int i = self->cur;
    if (i < self->count) {
        return self->entries[i].id;
    }
    return 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307A10);
#ifdef SKIP_ASM
class func_00307A10_cObj {
public:
    char pad_0x00[0x40];
    // vptr at 0x40; sizeof == 0x44 (same layout as cWScriptProcEntry)
    virtual void v01();
    virtual void v02();
    virtual void v03(int a1); // 0x18
};

extern "C" void func_00307A10(cWScriptProcList* self, int a1)
{
    int i = self->cur;
    if (i < self->count) {
        ((func_00307A10_cObj*)self->entries)[i].v03(a1);
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
extern "C" void cWScriptProcess_eventOnCollide(void* self)
{
    void* script = *(void**)((char*)self + 0x34);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnJump(void* self)
{
    void* script = *(void**)((char*)self + 0x38);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnLand(void* self)
{
    void* script = *(void**)((char*)self + 0x3c);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnTrick(void* self)
{
    void* script = *(void**)((char*)self + 0x40);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnWipeout(void* self)
{
    void* script = *(void**)((char*)self + 0x44);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnReset(void* self)
{
    void* script = *(void**)((char*)self + 0x48);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnCollect(void* self)
{
    void* script = *(void**)((char*)self + 0x4c);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnEvent1(void* self)
{
    void* script = *(void**)((char*)self + 0x50);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnEvent2(void* self)
{
    void* script = *(void**)((char*)self + 0x54);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_eventOnEvent3(void* self)
{
    void* script = *(void**)((char*)self + 0x58);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_processStart);
#ifdef SKIP_ASM
class cWScriptProcess_processStart_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

struct cWScriptProcess_processStart_sProc {
    int state;
    int count;
    int cur;
    int pad0C;
    void* man;
    cWScriptProcess_processStart_cElem* elems;
    int pad18;
    void* luno;
    int pad20;
    int pad24;
    void* script;
};

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
extern "C" void cWScriptProcess_processStart(cWScriptProcess_processStart_sProc* self)
{
    if (self->luno != 0) {
        luno_cLunoTable__cLunoTable(self->luno, 3);
    }
    self->luno = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_ProcessLocalTable, 0x20000000, 0), 8);
    self->state = 1;
    self->cur = 0;
    if (self->script != 0) {
        cWScriptMan_runScriptCode(self->man, self, self->script);
    }
    cWScriptMan_processStart(self->man, self);
    if (self->cur < self->count) {
        self->elems[self->cur].v04();
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
extern "C" void cWScriptProcess_processUpdate(void* self)
{
    void* script = *(void**)((char*)self + 0x2c);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_processEnd);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(void* man, void* proc, void* script);
extern "C" void cWScriptProcess_end(void* self);

/**
 * Runs the world script process's end script and ends the process.
 *
 * Sets the state (+0x0) to 3, runs the script code at +0x30 through cWScriptMan_runScriptCode when there is one and
 * calls cWScriptProcess_end.
 */
extern "C" void cWScriptProcess_processEnd(void* self)
{
    *(int*)self = 3;
    void* script = *(void**)((char*)self + 0x30);
    if (script != 0) {
        cWScriptMan_runScriptCode(*(void**)((char*)self + 0x10), self, script);
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
extern "C" void cWScriptProcess_end(void* self)
{
    *(int*)self = 3;
    cWScriptMan_processEnd(*(void**)((char*)self + 0x10), self);
    void* obj = *(void**)((char*)self + 0x1C);
    if (obj != 0) {
        luno_cLunoTable__cLunoTable(obj, 3);
    }
    *(void**)((char*)self + 0x1C) = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_processComplete);
#ifdef SKIP_ASM
int cWScriptMan_isProcessActive(void* man, void* proc);

struct sVEntry00307DC8 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Completes a world-script process if its manager still has it on the running list.
 *
 * cWScriptMan_isProcessActive() looks for a process with the same id (+0x20) on the running list; when there is one, this
 * calls its own virtual slot 6, which for cWScriptProcess (cWScriptProcess_processEnd) sets the state to 3, runs the script
 * at +0x30 and calls cWScriptProcess_end(). The task stepper cWScriptProcess_endTask calls it after the last task.
 */
extern "C" void cWScriptProcess_processComplete(void* self)
{
    if (cWScriptMan_isProcessActive(*(void**)((char*)self + 0x10), self) != 0) {
        sVEntry00307DC8* vt = *(sVEntry00307DC8**)((char*)self + 0x5C);
        vt[6].fn((char*)self + vt[6].delta);
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
    virtual void v04();
    virtual void v05();
    virtual void v06();
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
    virtual void v02();
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
    self->elems[self->cur].v06();
    int next = self->cur + 1;
    if (next < self->count) {
        self->cur = next;
        cWScriptProcess_endTask_cElem* e = &self->elems[next];
        e->v04();
    } else {
        self->v02();
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307EC0);
#ifdef SKIP_ASM
class func_00307EC0_cState {
public:
    int state;          // 0x0
    int id;             // 0x4
    char pad8[0x40 - 0x8];
    // vptr at 0x40; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

struct func_00307EC0_sProc {
    int pad0;
    int count;                          // 0x4
    int cur;                            // 0x8
    char padC[0x14 - 0xC];
    func_00307EC0_cState* states;       // 0x14
};

extern "C" void func_00307EC0(func_00307EC0_sProc* self, int id)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->states[i].id == id) {
            if (self->cur < self->count) {
                self->states[self->cur].state = 3;
            }
            self->cur = i;
            self->states[i].v04();
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307F58);
#ifdef SKIP_ASM
class func_00307F58_cElem {
public:
    int f0;
    int id;
    char pad08[0x38];
    // vptr at 0x40
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
};

struct func_00307F58_sProc {
    int state;
    int count;
    int cur;
    int pad0C;
    void* man;
    func_00307F58_cElem* elems;
};

extern "C" void func_00307F58(func_00307F58_sProc* self, int id)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->elems[i].id == id) {
            if (self->cur < self->count) {
                self->elems[self->cur].v06();
            }
            self->cur = i;
            self->elems[i].v04();
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
void* cWScriptProcess_getScriptCode(void* self)
{
    return cWScriptMan_getScriptCode(*(int*)((char*)self + 0x10));
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308038);
#ifdef SKIP_ASM
class func_00308038_cStream {
public:
    // slot N at vtable offset N*8
    virtual void v01(void* data, int size);
};

class func_00308760_cObj;
extern "C" int func_00308760(void* a, func_00308760_cObj* obj);
extern "C" int func_003ACA38(void* map, int id);
extern "C" void func_002257E0(void* obj, void* stream, int a2);

extern "C" void func_00308038(void* self, func_00308038_cStream* stream)
{
    stream->v01(self, 0x10);
    int x = func_003ACA38(cWorld_m_pTheWorld->m_pWScriptCache, *(int*)((char*)self + 0x24));
    int has = *(void**)((char*)self + 0x1C) != 0;
    stream->v01(&has, 4);
    if (has) {
        func_002257E0(*(void**)((char*)self + 0x1C), stream, x);
    }
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x4); i++) {
        func_00308760(*(char**)((char*)self + 0x14) + i * 68, (func_00308760_cObj*)stream);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308118);
#ifdef SKIP_ASM
class func_00308118_cStream {
public:
    // slot N at vtable offset N*8
    virtual void v01(void* data, int size);
    virtual void v02(void* data, int size);
};

extern char str_ProcessLocalTable[];
extern "C" int func_003ACA38(void* map, int id);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern "C" void luno_cLunoTable_readFromReplayFrame(void* obj, func_00308118_cStream* stream, int a2);
class func_00308798_cObj;
extern "C" int func_00308798(void* a, func_00308798_cObj* obj);

extern "C" void func_00308118(void* self, func_00308118_cStream* stream)
{
    stream->v02(self, 0x10);
    int x = func_003ACA38(cWorld_m_pTheWorld->m_pWScriptCache, *(int*)((char*)self + 0x24));
    if (*(void**)((char*)self + 0x1C) != 0) {
        luno_cLunoTable__cLunoTable(*(void**)((char*)self + 0x1C), 3);
    }
    *(void**)((char*)self + 0x1C) = 0;
    int has;
    stream->v02(&has, 4);
    if (has) {
        void* t = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_ProcessLocalTable, 0x20000000, 0), 8);
        *(void**)((char*)self + 0x1C) = t;
        luno_cLunoTable_readFromReplayFrame(t, stream, x);
    }
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x4); i++) {
        func_00308798(*(char**)((char*)self + 0x14) + i * 68, (func_00308798_cObj*)stream);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308228);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);
extern void* cWScriptTask_vtbl[];

extern "C" void* func_00308228(void* self)
{
    *(void***)((char*)self + 0x40) = cWScriptTask_vtbl;
    MEM_fill((char*)self + 8, 0, 0xC);
    *(int*)((char*)self + 0x3C) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)self = 0;
    return self;
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
 * Stores @p owner, copies the task id from the data's first word to +0x4 (func_00307EC0 finds tasks by it), and
 * turns the data's 3 + 10 script resource ids into code pointers with cWScriptMan_getScriptCode() through the
 * owner's manager (cWScriptProcess_getScriptCode).
 *
 * @param owner  the cWScriptProcess the task belongs to
 * @param def    the task's data record (On Tour: tWScriptTaskData)
 */
extern "C" void cWScriptTask_initTask(cWScriptTask_initTask_sObj* self, void* owner, cWScriptTask_initTask_sDef* def)
{
    int i;
    int j;
    self->owner = owner;
    self->f4 = def->f0;
    for (i = 0; i < 3; i++) {
        self->a[i] = cWScriptProcess_getScriptCode_2(self->owner, def->a[i]);
    }
    for (j = 0; j < 10; j++) {
        self->b[j] = cWScriptProcess_getScriptCode_2(self->owner, def->b[j]);
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
    virtual void v05();
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
        self->v05();
        break;
    case 2:
        self->v05();
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
    vt[slot].fn(self + vt[slot].delta);
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
// Same event dispatch as func_003078C0 for a running (state 2) sub-process.
extern "C" void cWScriptTask_dispatchTaskEvent(void* vself, void* vmsg)
{
    char* self = (char*)vself;
    unsigned short* msg = (unsigned short*)vmsg;
    if (*(int*)self != 2) {
        return;
    }
    switch (*msg) {
    case 0:
        VCall_003083A0(self, 7);
        break;
    case 1:
        VCall_003083A0(self, 8);
        break;
    case 2:
        VCall_003083A0(self, 9);
        break;
    case 3:
        *(unsigned short**)(*(char**)(*(char**)(self + 0x3C) + 0x10) + 0x29C) = msg + 2;
        VCall_003083A0(self, 10);
        *(unsigned short**)(*(char**)(*(char**)(self + 0x3C) + 0x10) + 0x29C) = 0;
        break;
    case 4:
        VCall_003083A0(self, 11);
        break;
    case 5:
        VCall_003083A0(self, 12);
        break;
    case 6:
        VCall_003083A0(self, 13);
        break;
    case 7:
        VCall_003083A0(self, 14);
        break;
    case 8:
        VCall_003083A0(self, 15);
        break;
    case 9:
        VCall_003083A0(self, 16);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnCollide);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's on-collide script (EventScriptCode[0], +0x14), when it has one, through the process's script
 * manager (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnCollide(void* self)
{
    void* script = *(void**)((char*)self + 0x14);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnJump);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's on-jump script (EventScriptCode[1], +0x18), when it has one, through the process's script manager
 * (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnJump(void* self)
{
    void* script = *(void**)((char*)self + 0x18);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnLand);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's on-land script (EventScriptCode[2], +0x1C), when it has one, through the process's script manager
 * (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnLand(void* self)
{
    void* script = *(void**)((char*)self + 0x1c);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnTrick);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's on-trick script (EventScriptCode[3], +0x20), when it has one, through the process's script
 * manager (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnTrick(void* self)
{
    void* script = *(void**)((char*)self + 0x20);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnWipeout);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's on-wipeout script (EventScriptCode[4], +0x24), when it has one, through the process's script
 * manager (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnWipeout(void* self)
{
    void* script = *(void**)((char*)self + 0x24);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnReset);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's on-reset script (EventScriptCode[5], +0x28), when it has one, through the process's script
 * manager (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnReset(void* self)
{
    void* script = *(void**)((char*)self + 0x28);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnCollect);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's on-collect script (EventScriptCode[6], +0x2C), when it has one, through the process's script
 * manager (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnCollect(void* self)
{
    void* script = *(void**)((char*)self + 0x2c);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnEvent1);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's event-1 script (EventScriptCode[7], +0x30), when it has one, through the process's script manager
 * (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnEvent1(void* self)
{
    void* script = *(void**)((char*)self + 0x30);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnEvent2);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's event-2 script (EventScriptCode[8], +0x34), when it has one, through the process's script manager
 * (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnEvent2(void* self)
{
    void* script = *(void**)((char*)self + 0x34);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_eventOnEvent3);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's event-3 script (EventScriptCode[9], +0x38), when it has one, through the process's script manager
 * (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_eventOnEvent3(void* self)
{
    void* script = *(void**)((char*)self + 0x38);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_taskStart);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Starts the task once: sets its state to 1 (started) and runs its start script (TaskScriptCode[0], +0x8) when it
 * has one.
 */
extern "C" void cWScriptTask_taskStart(void* self)
{
    if (*(int*)self != 1) {
        *(int*)self = 1;
        void* script = *(void**)((char*)self + 0x8);
        if (script != 0) {
            func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3C) + 0x10), self, script);
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_taskUpdate);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Runs the task's update script (TaskScriptCode[1], +0xC), when it has one, through the process's script manager
 * (func_0030ADA8, On Tour cWScriptMan::runScriptCode).
 */
extern "C" void cWScriptTask_taskUpdate(void* self)
{
    void* script = *(void**)((char*)self + 0xc);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptTask_taskEnd);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

/**
 * Ends the task once: sets its state to 3 (ended) and runs its end script (TaskScriptCode[2], +0x10) when it has
 * one.
 */
extern "C" void cWScriptTask_taskEnd(void* self)
{
    if (*(int*)self != 3) {
        *(int*)self = 3;
        void* script = *(void**)((char*)self + 0x10);
        if (script != 0) {
            func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3C) + 0x10), self, script);
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308760);
#ifdef SKIP_ASM
class func_00308760_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
};

extern "C" int func_00308760(void* a, func_00308760_cObj* obj)
{
    return obj->v01(a, 4);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308798);
#ifdef SKIP_ASM
class func_00308798_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
    virtual int v02(void* a, int b);
};

extern "C" int func_00308798(void* a, func_00308798_cObj* obj)
{
    return obj->v02(a, 4);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_cWScriptMan);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void tMissionMenuQueue_tMissionMenuQueue(void* p);
void func_0030B058(void* p);
extern "C" void cCollectPool_init(void* p);
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void cWScriptMan_init(void* self);
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
extern "C" void* cWScriptMan_cWScriptMan(void* self)
{
    *(unsigned int*)((char*)self + 0x2C) = 0xFFFFFFFF;
    tMissionMenuQueue_tMissionMenuQueue((char*)self + 0x1C4);
    *(void**)((char*)self + 0x494) = cWScriptMan_vtbl;
    func_0030B058((char*)self + 0x2B4);
    func_0030B058((char*)self + 0x2B8);
    func_0030B058((char*)self + 0x2BC);
    char* p = (char*)self + 0x2C0;
    int i;
    for (i = 0; i != -1; i--) {
        char* next = p + 0x10C;
        unsigned int* q = (unsigned int*)(p + 0xC);
        int j;
        for (j = 63; j != -1; j--) {
            *q++ = 0xFFFFFFFF;
        }
        cCollectPool_init(p);
        p = next;
    }
    cWScriptMan_cWScriptMan_current = self;
    *(char**)((char*)self + 0x28C) = (char*)cWorld_m_pTheWorld;
    MEM_fill((char*)self + 0x3CC, 0, 200);
    cWScriptMan_init(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan__cWScriptMan);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_purgeGlobalTables(void* self);
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
extern "C" void cWScriptMan__cWScriptMan(void* self, int flags)
{
    *(void**)((char*)self + 0x494) = cWScriptMan_vtbl;
    cWScriptMan_purgeGlobalTables(self);
    cWScriptMan_TheWScriptMan = 0;
    if ((char*)self + 0x2C0 != 0) {
        char* p = (char*)self + 0x2C0 + 0x10C;
        while ((char*)self + 0x2C0 != p) {
            p -= 0x10C;
        }
    }
    cProcessList__cProcessList((char*)self + 0x2BC, 2);
    cProcessList__cProcessList((char*)self + 0x2B8, 2);
    cProcessList__cProcessList((char*)self + 0x2B4, 2);
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
extern "C" void cWScriptMan_rebuildGlobalTables(void* self);

#define WSP308988_I(off) (*(int*)((char*)self + (off)))
#define WSP308988_U(off) (*(unsigned int*)((char*)self + (off)))

/**
 * Resets the world script manager to its starting state.
 *
 * Clears the current being, instance, infos, process and task and the mission state, sets the mission ids (+0x2A0,
 * +0x2B0) and several counters to -1, empties the menu queue (tMissionMenuQueue_init) and the event list at +0x44, and runs
 * cWScriptMan_rebuildGlobalTables.
 */
extern "C" void cWScriptMan_init(void* self)
{
    WSP308988_I(656) = 0;
    WSP308988_I(0x0) = 0;
    WSP308988_I(660) = 0;
    WSP308988_I(664) = 0;
    WSP308988_I(676) = 0;
    WSP308988_I(680) = 0;
    WSP308988_I(668) = 0;
    WSP308988_I(684) = 0;
    WSP308988_I(0x4) = 0;
    WSP308988_I(0x8) = 0;
    WSP308988_I(0x10) = 0;
    WSP308988_I(20) = 0;
    WSP308988_I(0x18) = 0;
    WSP308988_I(0x1C) = 0;
    WSP308988_I(0x20) = 0;
    WSP308988_I(40) = 0;
    WSP308988_I(0x30) = 0;
    WSP308988_I(52) = 0;
    WSP308988_U(672) = 0xFFFFFFFF;
    WSP308988_U(688) = 0xFFFFFFFF;
    WSP308988_I(0xC) = -1;
    WSP308988_I(36) = -1;
    WSP308988_U(44) = 0xFFFFFFFF;
    MEM_fill((char*)self + 0x38, 0, 8);
    tMissionMenuQueue_init((char*)self + 0x1C4);
    WSP308988_I(0x40) = 0;
    MEM_fill((char*)self + 0x44, 0, 0x180);
    cWScriptMan_rebuildGlobalTables(self);
}

#undef WSP308988_I
#undef WSP308988_U
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
extern "C" void cWScriptMan_purgeGlobalTables(void* self)
{
    void** objs = (void**)((char*)self + 0x3CC);
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
extern "C" void cWScriptMan_purgeGlobalTables(void* self);
// PORT: the unit declares func_00308B78 as (void* self); the body also takes the slot index
void* func_00308B78_impl(void* self, int i) __asm__("func_00308B78");

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
 * data, calls func_00308B78 with the section index.
 */
extern "C" void cWScriptMan_rebuildGlobalTables(void* self)
{
    int i = 0;
    cWScriptMan_purgeGlobalTables(self);
    int n = cWorldView_getNumSections((cWorldView*)(**(char***)((char*)self + 0x28C) + 0x10));
    for (; i < n; i++) {
        cWScriptMan_rebuildGlobalTables_sSection* tbl = *(cWScriptMan_rebuildGlobalTables_sSection**)(*(char**)(*(char**)((char*)self + 0x28C) + 0x4) + 0x4);
        int ok = tbl[i].type >= 2 && tbl[i].f8 != 0;
        if (ok) {
            func_00308B78_impl(self, i);
        }
    }
}
#endif

extern "C" void* func_00308B78(void* self);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308B58__FPv);
#ifdef SKIP_ASM
void* func_00308B58(void* self)
{
    return func_00308B78(self);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308B78);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern const char str_cLunoTable_b[];

// PORT: the unit declares func_00308B78 as (void* self); the body also takes the slot index
void* func_00308B78_impl(void* self, int i) __asm__("func_00308B78");
void* func_00308B78_impl(void* self, int i)
{
    char* base = (char*)self + 0x3CC;
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
extern "C" void cWScriptMan_activeSection(void* self, int section)
{
    int n = cWorld_getNumProcessScripts(*(void**)((char*)self + 0x28C), section);
    cWScriptMan_activeSection_sDef* defs = (cWScriptMan_activeSection_sDef*)cWorld_getProcessScripts(*(void**)((char*)self + 0x28C), section);
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

extern signed char D_00535C10[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cPlaybackMan_purgeLocation(void* pb);
extern "C" int cWorld_getNumMissionScripts(void* world, int i);
extern "C" void* cWorld_getMissionScripts(void* world, int i);
extern "C" void cWScriptMan_addProcess1(void* self, int section, void* def, int id, int flags);
extern "C" char* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cPlaybackMan_initLocation(void* pb, void* loc);
extern "C" void cWScriptMan_updateWorld(void* self);

static inline void* wsPlayback8C60()
{
    return SSXApp->Game->PlaybackMan;
}

/**
 * Sets up the world's missions: in game mode 4 with one player, creates a mission process for every mission
 * script of every world section.
 *
 * First releases the playback manager's loaded data (cPlaybackMan_purgeLocation). When cGameMode (D_00535C10) is 4 and the
 * AI's NumberOfPlayersAllocated is 1, it adds a cWScriptMission (cWScriptMan_addProcess1) per 0x54-byte
 * mission script, keyed (section << 16) + section, and calls cPlaybackMan_initLocation() for the race
 * location's data (+0x34). cWScriptMan_updateWorld() always runs last: it runs each mission script's code at +0x4C and
 * clears the manager's mission state.
 */
extern "C" void cWScriptMan_setupMissions(void* self)
{
    void* iface;
    cPlaybackMan_purgeLocation(wsPlayback8C60());
    iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C10[0] == 4 && *(int*)((char*)SSXApp->Game->AI + 0x7C) == 1) {
        int i = 0;
        int n = cWorldView_getNumSections((cWorldView*)(**(char***)((char*)self + 0x28C) + 0x10));
        for (; i < n; i++) {
            int cnt = cWorld_getNumMissionScripts(*(void**)((char*)self + 0x28C), i);
            sWSPDef8C60* defs = (sWSPDef8C60*)cWorld_getMissionScripts(*(void**)((char*)self + 0x28C), i);
            int j;
            for (j = 0; j < cnt; j++) {
                cWScriptMan_addProcess1(self, i, &defs[j], (i << 16) + i, 0);
            }
        }
        cPlaybackMan_initLocation(wsPlayback8C60(), cBENewRaceInterface_getLocation(iface) + 0x34);
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
void func_00226600(void* self, int a1, int a2);
extern "C" void* func_002224A8(void* self);
extern "C" void func_002224B8(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);

/**
 * Applies every mission's world-update script to the world.
 *
 * For each world section and each of its mission scripts whose id differs from the local mission (+0x2A0), creates an
 * 8-slot local table, runs the code for the mission's +0x4C script id in it if there is any, and destroys the table.
 * Then clears the current instance, infos, process and task.
 */
extern "C" void cWScriptMan_updateWorld(void* self)
{
    int i = 0;
    int n = cWorldView_getNumSections((cWorldView*)(**(char***)((char*)self + 0x28C) + 0x10));
    for (; i < n; i++) {
        int cnt = cWorld_getNumMissionScripts(*(void**)((char*)self + 0x28C), i);
        sWSPDef8DB8* defs = (sWSPDef8DB8*)cWorld_getMissionScripts(*(void**)((char*)self + 0x28C), i);
        int j;
        for (j = 0; j < cnt; j++) {
            if (defs[j].f0 != *(int*)((char*)self + 0x2A0)) {
                void* table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
                int h = cWScriptMan_getScriptCode_2(self, defs[j].f4C);
                if (h != 0) {
                    sWSPValue8DB8 arg;
                    sWSPValue8DB8 ret;
                    sWSPCall8DB8 call;
                    func_00226600(&arg, h, 0);
                    ret.type = 0;
                    ret.word0 = 0;
                    func_002224A8(&call);
                    func_002224B8(&call, &arg, table, &ret);
                    luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
                }
                if (table != 0) {
                    luno_cLunoTable__cLunoTable(table, 3);
                }
            }
        }
    }
    *(int*)((char*)self + 0x290) = 0;
    *(int*)((char*)self + 0x294) = 0;
    *(int*)((char*)self + 0x298) = 0;
    *(int*)((char*)self + 0x2A4) = 0;
    *(int*)((char*)self + 0x2A8) = 0;
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
extern "C" void cWScriptMan_purgeMissions(void* self)
{
    cProcessList_deleteAll((char*)self + 0x2B8);
    cProcessList_deleteAll((char*)self + 0x2BC);
    cProcessList_deleteAll((char*)self + 0x2B4);
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
extern "C" void cWScriptMan_purgeSection(void* self, int section)
{
    cProcessList_unloadSection((char*)self + 0x2B4, section);
    cProcessList_unloadSection((char*)self + 0x2B8, section);
    cProcessList_unloadSection((char*)self + 0x2BC, section);
    int off = section << 2;
    char* base = (char*)self + 0x3CC;
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

extern "C" void cWScriptMan_purgeSection(void* self, int i);
extern "C" void func_003ACC90(void* entry);

/**
 * Unloads a world section's scripts: purges its processes, then releases its world script cache entry.
 *
 * After cWScriptMan_purgeSection() it calls func_003ACC90 on entry @p i of the world's script cache
 * (m_pWScriptCache of the world at +0x28C), which frees the entry's buffer unless its +0x2 flag is set and
 * clears the entry.
 *
 * @param i  world section id
 */
extern "C" void cWScriptMan_unloadSection(void* self, int i)
{
    cWScriptMan_purgeSection(self, i);
    cWScriptMan_unloadSection_sEntry* items =
        *(cWScriptMan_unloadSection_sEntry**)(*(char**)(*(char**)((char*)self + 0x28C) + 4) + 4);
    func_003ACC90(&items[i]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_initMissions);
#ifdef SKIP_ASM
struct cWorldView;
int cWorldView_getNumSections(cWorldView* view);
extern "C" void cProcessList_deleteAll(void*);
extern "C" void cWScriptMan_init(void* self);
extern "C" void cWScriptMan_activeSection(void* self, int section);
extern "C" void cWScriptMan_purgeSection(void* self, int section);

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
extern "C" void cWScriptMan_initMissions(void* self)
{
    int i = 0;
    cProcessList_deleteAll((char*)self + 0x2B8);
    cProcessList_deleteAll((char*)self + 0x2BC);
    cProcessList_deleteAll((char*)self + 0x2B4);
    cWScriptMan_init(self);
    int n = cWorldView_getNumSections((cWorldView*)(**(char***)((char*)self + 0x28C) + 0x10));
    for (; i < n; i++) {
        cWScriptMan_initMissions_sSection* tbl = *(cWScriptMan_initMissions_sSection**)(*(char**)(*(char**)((char*)self + 0x28C) + 0x4) + 0x4);
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
extern "C" void cWScriptMan_updateWorld(void* self);
extern "C" void cWScriptMan_doMissionCmd(void* self, int id, int a2, int a3);
extern "C" void* func_0028B180();
extern "C" void func_0029D6E0(void* snd, int a1);
extern "C" void cSSXAudio_BigChallengeFailed(void* snd, int a1);
extern "C" void cReplay_startRace(void* p);
extern "C" void cPlaybackMan_startPreview(void* p);

/**
 * Carries out the local mission request queued by the menus.
 *
 * Switches on the request at +0x2AC (2, 3 and 4 are set by cWScriptMan_restartLocalMission,
 * cWScriptMan_cancelLocalMission and cWScriptMan_quitLocalMission) for the mission saved in +0x2B0: 1 updates the
 * world, sends mission command 1, plays the big-challenge start audio (func_0029D6E0) and restarts the race; 2 sends
 * command 3; 3 and 4 send commands 18 and 5, update the world and play the failure audio (cSSXAudio_BigChallengeFailed); 5 starts the
 * playback preview (cPlaybackMan_startPreview), sends command 1 for rider 1 and updates the world; 6 sends 18 and updates the
 * world. The request is then cleared (0, id -1).
 */
// Runs the pending gate action (replay, sounds, camera) and clears it.
extern "C" void cWScriptMan_processLocalMissionCmd(void* vself)
{
    char* self = (char*)vself;
    switch (*(int*)(self + 0x2AC)) {
    case 1:
        cWScriptMan_updateWorld(self);
        cWScriptMan_doMissionCmd(self, *(int*)(self + 0x2B0), 1, 0);
        func_0029D6E0(func_0028B180(), *(int*)(self + 0x2A0));
        cReplay_startRace(SSXApp->Game->Replay);
        break;
    case 2:
        cWScriptMan_doMissionCmd(self, *(int*)(self + 0x2B0), 3, 0);
        break;
    case 3:
        cWScriptMan_doMissionCmd(self, *(int*)(self + 0x2B0), 18, 0);
        cWScriptMan_updateWorld(self);
        cSSXAudio_BigChallengeFailed(func_0028B180(), 1);
        break;
    case 4:
        cWScriptMan_doMissionCmd(self, *(int*)(self + 0x2B0), 5, 0);
        cWScriptMan_updateWorld(self);
        cSSXAudio_BigChallengeFailed(func_0028B180(), 1);
        break;
    case 5:
        cPlaybackMan_startPreview(SSXApp->Game->PlaybackMan);
        cWScriptMan_doMissionCmd(self, *(int*)(self + 0x2B0), 1, 1);
        cWScriptMan_updateWorld(self);
        break;
    case 6:
        cWScriptMan_doMissionCmd(self, *(int*)(self + 0x2B0), 18, 0);
        cWScriptMan_updateWorld(self);
        break;
    }
    *(int*)(self + 0x2AC) = 0;
    *(unsigned int*)(self + 0x2B0) = 0xFFFFFFFF;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", cWScriptMan_update);
#ifdef SKIP_ASM
class func_003077F8_cProc;
struct cWScriptProcList;
extern "C" void func_003077F8(func_003077F8_cProc* self);
extern "C" void func_003078C0(void* proc, void* cmd);
extern "C" void func_00307A10(cWScriptProcList* self, int a1);
extern "C" void cWScriptMan_processLocalMissionCmd(void* self);
extern "C" void cWScriptMan_startProcess(void* self, void* p);
extern "C" void* cProcessList_removeHead(void* self);
int func_0030B260(void* self, void* a1);

struct sRefSet_309270 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};
struct sRefWorld_309270 {
    char pad_0x0[0x8];
    sRefSet_309270** sets;  // 0x8
};

struct sCmd_309270 {
    unsigned int flags;
    unsigned int id;
    int f8;
};
struct sCmdList_309270 {
    int count;
    sCmd_309270 e[32];
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
 * up when its low 16 flag bits are 0) is passed to every active process: func_003078C0 for process events (0x20000) and
 * func_00307A10 for task events (0x10000). Finally every active process is updated with func_003077F8, each made
 * current in turn.
 */
extern "C" void cWScriptMan_update(void* self)
{
    cWScriptMan_processLocalMissionCmd(self);
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x34) = 0;
    void* p;
    while ((p = cProcessList_removeHead((char*)self + 0x2BC)) != 0) {
        func_0030B260((char*)self + 0x2B4, p);
        cWScriptMan_startProcess(self, p);
    }
    sCmdList_309270 list = *(sCmdList_309270*)((char*)self + 0x40);
    *(int*)((char*)self + 0x40) = 0;
    for (int i = 0; i < list.count; i++) {
        sCmd_309270* c = &list.e[i];
        if ((unsigned short)c->flags == 0)
            *(void**)((char*)self + 0x290) = lookup_309270(**(sRefWorld_309270***)((char*)self + 0x28C), c->id);
        char* next = *(char**)((char*)self + 0x2B8);
        while (next != 0) {
            char* proc = next;
            next = *(char**)(proc + 0x18);
            *(char**)((char*)self + 0x2A4) = proc;
            if (c->flags & 0x20000)
                func_003078C0(proc, c);
            if (c->flags & 0x10000)
                func_00307A10((cWScriptProcList*)proc, (int)c);
        }
        *(int*)((char*)self + 0x290) = 0;
        *(int*)((char*)self + 0x2A4) = 0;
    }
    char* next = *(char**)((char*)self + 0x2B8);
    while (next != 0) {
        char* proc = next;
        next = *(char**)(proc + 0x18);
        *(char**)((char*)self + 0x2A4) = proc;
        func_003077F8((func_003077F8_cProc*)proc);
    }
    *(int*)((char*)self + 0x2A4) = 0;
}
#endif


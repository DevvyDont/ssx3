#include "common.h"
#include "sound/ssxaudio.h"
#include "wscript/missionmenuqueue.h"
#include "wscript/wscriptmission.h"
#include "animation/animmodel.h"
#include "replay/frameptr.h"
#include "wscript/wscriptprocess.h"
#include "wscript/wscriptman.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_addProcess);
#ifdef SKIP_ASM
extern char str_WScriptProcess[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWScriptProcess_cWScriptProcess(void* self, void* man, int a1, int a2);
int cProcessList_addProcess(void* self, void* a1);

/**
 * Creates a world-script process for a section's process data and adds it to one of the manager's lists.
 *
 * The process ("WScriptProcess", 0x60 bytes) goes to the list at +0x2BC when @p front is set, otherwise to
 * the one at +0x2B4.
 *
 * @param sectionId     section id (On Tour sectionId)
 * @param data     the process data (On Tour const tWScriptProcessData*)
 * @param front  nonzero for the list at +0x2BC (On Tour start)
 * @return the new process
 */
extern "C" void* cWScriptMan_addProcess(cWScriptMan* self, int sectionId, int data, int front)
{
    void* process = cWScriptProcess_cWScriptProcess(cMemMan_alloc(0x60, str_WScriptProcess, 0x20000000, 0), self, sectionId, data);
    if (front != 0) {
        cProcessList_addProcess((char*)&self->AddList, process);
    } else {
        cProcessList_addProcess((char*)&self->IdleList, process);
    }
    return process;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_addProcess1);
#ifdef SKIP_ASM
extern char str_WScriptMission[];
extern "C" void* cWScriptMission_cWScriptMission(void* self, void* man, int a1, int a2, int a3);
int cProcessList_addProcess1(void* self, void* a1);

/**
 * Creates a world-script mission for a section's mission data and adds it to one of the manager's lists.
 *
 * The mission ("WScriptMission", 0x78 bytes) goes to the list at +0x2BC when @p front is set, otherwise to
 * the one at +0x2B4.
 *
 * @param sectionId     section id (On Tour sectionId)
 * @param data     the mission data (On Tour const tWScriptMissionData*)
 * @param key     the mission's key (On Tour key)
 * @param front  nonzero for the list at +0x2BC (On Tour start)
 * @return the new mission
 */
extern "C" void* cWScriptMan_addProcess1(cWScriptMan* self, int sectionId, int data, int key, int front)
{
    void* mission = cWScriptMission_cWScriptMission(cMemMan_alloc(0x78, str_WScriptMission, 0x20000000, 0), self, sectionId, data, key);
    if (front != 0) {
        cProcessList_addProcess1((char*)&self->AddList, mission);
    } else {
        cProcessList_addProcess1((char*)&self->IdleList, mission);
    }
    return mission;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_remProcess);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cWSObj_309618 {
public:
    char pad_0x00[0x5C];
    virtual void v00(int);
    virtual void v01();
};

extern "C" void* cProcessList_findProcess1_a(void* self, int id) __asm__("cProcessList_findProcess1");
extern "C" int cProcessList_removeProcess_a(void* self, void* node) __asm__("cProcessList_removeProcess");

/**
 * Removes the world script process with ID @p id and deletes it.
 *
 * Looks it up (cProcessList_findProcess1) in the active list (+0x2B8), then the start list (+0x2BC), then the idle list (+0x2B4);
 * the first hit is unlinked (cProcessList_removeProcess) and destroyed through its virtual destructor (flag 3).
 *
 * @param id  the process ID
 */
extern "C" void cWScriptMan_remProcess(cWScriptMan* self, int id) {
    cWSObj_309618* obj = (cWSObj_309618*)cProcessList_findProcess1_a(&self->ActiveList, id);
    if (obj != 0) {
        cProcessList_removeProcess_a(&self->ActiveList, obj);
        obj->v00(3);
        return;
    }
    obj = (cWSObj_309618*)cProcessList_findProcess1_a(&self->AddList, id);
    if (obj != 0) {
        cProcessList_removeProcess_a(&self->AddList, obj);
        obj->v00(3);
        return;
    }
    obj = (cWSObj_309618*)cProcessList_findProcess1_a(&self->IdleList, id);
    if (obj != 0) {
        cProcessList_removeProcess_a(&self->IdleList, obj);
        obj->v00(3);
    }
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_removeProcess);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cProcessList_removeProcess_vv(void* self, void* node) __asm__("cProcessList_removeProcess");
/**
 * Unlinks a world script process from whichever list holds it: the active list (+0x2B8), else the idle list (+0x2B4),
 * else the start list (+0x2BC) (cProcessList_removeProcess). The process is not deleted.
 *
 * @param node  the process
 */
extern "C" void cWScriptMan_removeProcess(cWScriptMan* self, cWScriptProcess* node) {
    if (cProcessList_removeProcess_vv(&self->ActiveList, node) == 0 &&
        cProcessList_removeProcess_vv(&self->IdleList, node) == 0) {
        cProcessList_removeProcess_vv(&self->AddList, node);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_startProcess);
#ifdef SKIP_ASM
struct sVEntry00309750 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Starts a world script process.
 *
 * Saves the current process (+0x2A4), makes @p obj current, calls virtual slot 4 of its vtable at +0x5C and restores
 * the saved process. (On Tour also checks the process is idle, not active.)
 *
 * @param obj  the process to start (On Tour process)
 */
extern "C" void cWScriptMan_startProcess(cWScriptMan* self, cWScriptProcess* obj)
{
    void* saved = self->CurrentProcess;
    self->CurrentProcess = (cWScriptProcess*)obj;
    sVEntry00309750* vt = (sVEntry00309750*)obj->mVtbl;
    vt[4].fn((char*)obj + vt[4].delta);
    self->CurrentProcess = (cWScriptProcess*)saved;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_stopProcess);
#ifdef SKIP_ASM
struct sVEntry00309798 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Stops a world script process.
 *
 * Saves the current process (+0x2A4), makes @p obj current, calls virtual slot 6 of its vtable at +0x5C and restores
 * the saved process. (On Tour also checks the process is active, not idle.)
 *
 * @param obj  the process to stop (On Tour process)
 */
extern "C" void cWScriptMan_stopProcess(cWScriptMan* self, cWScriptProcess* obj)
{
    void* saved = self->CurrentProcess;
    self->CurrentProcess = (cWScriptProcess*)obj;
    sVEntry00309798* vt = (sVEntry00309798*)obj->mVtbl;
    vt[6].fn((char*)obj + vt[6].delta);
    self->CurrentProcess = (cWScriptProcess*)saved;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_eventCollide);
#ifdef SKIP_ASM
struct sWSOpData {
    int a;
    int b;
};

struct sWSOp {
    int op;
    sWSOpData data;
};

struct sWSOpList {
    int count;
    sWSOp ops[1];
};

static inline void sWSOpList_push(sWSOpList* self, sWSOp o)
{
    self->ops[self->count++] = o;
}

/**
 * Tells the world scripts that the rider collided with an instance: pushes event 0x70000 with @p value onto the event
 * list (+0x40).
 *
 * @param value  the instance's resource ID (+0x78 of its record)
 */
extern "C" void cWScriptMan_eventCollide(cWScriptMan* self, int value)
{
    sWSOp cmd;
    cmd.op = 0x70000;
    cmd.data.a = value;
    sWSOpList_push((sWSOpList*)(self->Events), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_eventJump);
#ifdef SKIP_ASM
/**
 * Tells the world scripts that the rider jumped: pushes event 0x70001 (no data) onto the event list (+0x40).
 */
extern "C" void cWScriptMan_eventJump(cWScriptMan* self)
{
    sWSOp cmd;
    cmd.op = 0x70001;
    sWSOpList_push((sWSOpList*)(self->Events), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_eventLand);
#ifdef SKIP_ASM
/**
 * Tells the world scripts that the rider landed: pushes event 0x70002 (no data) onto the event list (+0x40).
 */
extern "C" void cWScriptMan_eventLand(cWScriptMan* self)
{
    sWSOp cmd;
    cmd.op = 0x70002;
    sWSOpList_push((sWSOpList*)(self->Events), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_eventTrick);
#ifdef SKIP_ASM
/**
 * Tells the world scripts that a rider landed a trick.
 *
 * Pushes an event with code 0x70003 and a copy of @p trickDef onto the event list at +0x40.
 *
 * @param trickDef  the trick that was landed (On Tour td)
 */
extern "C" void cWScriptMan_eventTrick(cWScriptMan* self, sWSOpData* trickDef)
{
    sWSOp event;
    event.op = 0x70003;
    event.data = *trickDef;
    sWSOpList_push((sWSOpList*)(self->Events), event);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_eventWipeout);
#ifdef SKIP_ASM
/**
 * Tells the world scripts that the rider wiped out: pushes event 0x70004 (no data) onto the event list (+0x40).
 */
extern "C" void cWScriptMan_eventWipeout(cWScriptMan* self)
{
    sWSOp cmd;
    cmd.op = 0x70004;
    sWSOpList_push((sWSOpList*)(self->Events), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_eventReset);
#ifdef SKIP_ASM
/**
 * Tells the world scripts that the rider was reset: pushes event 0x70005 (no data) onto the event list (+0x40).
 */
extern "C" void cWScriptMan_eventReset(cWScriptMan* self)
{
    sWSOp cmd;
    cmd.op = 0x70005;
    sWSOpList_push((sWSOpList*)(self->Events), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_processStart);
#ifdef SKIP_ASM
struct cWScriptListHead;
struct cWScriptListNode;
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node);
int cProcessList_addProcess(void* self, void* a1);

/**
 * Moves a world script process from the idle list (+0x2B4) to the active list (+0x2B8).
 *
 * @param node  the process (On Tour cWScriptProcess*)
 */
extern "C" void cWScriptMan_processStart(cWScriptMan* self, cWScriptListNode* node)
{
    cProcessList_removeProcess((cWScriptListHead*)&self->IdleList, node);
    cProcessList_addProcess((char*)&self->ActiveList, node);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_processEnd);
#ifdef SKIP_ASM
struct cWScriptListHead;
struct cWScriptListNode;
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node);
extern "C" int cProcessList_findProcess(cWScriptListHead* self, cWScriptListNode* node);
int cProcessList_addProcess(void* self, void* a1);

/**
 * Moves a world script process back from the active list (+0x2B8) to the idle list (+0x2B4).
 *
 * Does nothing when the process is not on the active list.
 *
 * @param node  the process (On Tour cWScriptProcess*)
 */
extern "C" void cWScriptMan_processEnd(cWScriptMan* self, cWScriptListNode* node)
{
    cWScriptListHead* list = (cWScriptListHead*)&self->ActiveList;
    if (cProcessList_findProcess(list, node) != 0) {
        cProcessList_removeProcess(list, node);
        cProcessList_addProcess((char*)&self->IdleList, node);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_processPass);
#ifdef SKIP_ASM
int cProcessList_addProcess(void* self, void* a1);
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node);
extern "C" int cProcessList_findProcess(cWScriptListHead* self, cWScriptListNode* node);

/**
 * Retires a world script process whose mission was passed.
 *
 * If @p node is in the active list at +0x2B8 it is removed and added to the idle list at +0x2B4 (cProcessList_addProcess). +0x2A0
 * is set to -1 either way.
 *
 * @param node  the process (On Tour process)
 */
extern "C" void cWScriptMan_processPass(cWScriptMan* self, cWScriptListNode* node)
{
    cWScriptListHead* list = (cWScriptListHead*)&self->ActiveList;
    if (cProcessList_findProcess(list, node) != 0) {
        cProcessList_removeProcess(list, node);
        cProcessList_addProcess((char*)&self->IdleList, node);
    }
    self->LocalMissionId = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getScriptCode);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptScript(void*, int*);

static inline bool cWScriptMan_getScriptCode_valid(int id) { return id != -1; }

/**
 * Looks up the code of a world script by its resource ID.
 *
 * Asks the world (the pointer at +0x28C) through cWorld_getWScriptScript (On Tour cWorld::getWScriptScript).
 *
 * @param id  the script's resource ID; -1 means none
 * @return the script code, or 0 for ID -1
 */
extern "C" int cWScriptMan_getScriptCode(cWScriptMan* self, int id)
{
    if (cWScriptMan_getScriptCode_valid(id)) {
        return cWorld_getWScriptScript(self->World, &id);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getHeightCurr);
#ifdef SKIP_ASM
struct cWScriptMan_getHeightCurr_sVec {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16)));

struct cWScriptMan_getHeightCurr_sPos {
    char pad[8];
    float z;
};

struct cWScriptMan_getHeightCurr_sVEntry {
    short delta;
    short index;
    cWScriptMan_getHeightCurr_sPos* (*fn)(void*);
};


static inline char* cWScriptMan_getHeightCurr_toPtr(unsigned int i)
{
    return (char*)(i << 2);
}

static inline char* cWScriptMan_getHeightCurr_get(char* e, unsigned int h)
{
    unsigned int i = ((unsigned int*)*(char**)(e + 0x1C))[h >> 8] >> 8;
    if (i == 0) {
        return 0;
    }
    return cWScriptMan_getHeightCurr_toPtr(i);
}

static inline char* cWScriptMan_getHeightCurr_resolve(char* tbl, unsigned int h)
{
    char* e = ((char**)*(char**)(tbl + 0x8))[h & 0xFF];
    if (e == 0) {
        return 0;
    }
    return cWScriptMan_getHeightCurr_get(e, h);
}

/**
 * Returns the current height of the mission's rider above its height instance, or 0 when the height display is off
 * (+0x28).
 *
 * The instance (resource ID +0x2C, resolved through the world's instance tables, +0x28C) gives the base height (z of
 * its position, +0x40); the rider (index +0x30 into the game's AI rider list) gives its position through virtual slot 5
 * of its +0x6C0 part.
 *
 * @return the height above the instance, never negative
 */
extern "C" float cWScriptMan_getHeightCurr(cWScriptMan* self)
{
    if (self->MissionHeightEnable == 0) {
        return 0.0f;
    }
    char* node = cWScriptMan_getHeightCurr_resolve(*(char**)self->World, self->MissionHeightInstId);
    char* list = (char*)SSXApp->Game->AI;
    char* obj = *(char**)(list + (self->MissionHeightBeing << 2) + 0x28);
    char* sub = obj + 0x6C0;
    cWScriptMan_getHeightCurr_sVEntry* vt = *(cWScriptMan_getHeightCurr_sVEntry**)sub;
    cWScriptMan_getHeightCurr_sPos* riderPos = vt[5].fn(sub + vt[5].delta);
    float z = riderPos->z;
    cWScriptMan_getHeightCurr_sVec pos = *(cWScriptMan_getHeightCurr_sVec*)(node + 0x40);
    float height = z - pos.z;
    if (height >= 0.0f) {
        return height;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_runScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptScript(void*, int*);
struct cWScriptMan_isFunc_sValue;
struct sLunoValue_9C88 {
    int word0;
    int word1;
    int type;
};
struct sLunoCall_9C88 {
    int pad[4];
};
void luno_cLunoFunc_init(void* self, int a1, int a2);
extern "C" void* luno_cLunoVM_cLunoVM(void* self);
extern "C" void luno_cLunoVM_run(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);
/**
 * Runs a world script by resource ID, discarding its result.
 *
 * Builds a Luno value from the script's code (cWorld_getWScriptScript on the world, +0x28C; luno_cLunoFunc_init) and runs it
 * in a fresh Luno VM (luno_cLunoVM_run) against the global table of the script's section (+0x3CC, indexed by the ID's
 * low byte).
 *
 * @param handle  the script's resource ID (On Tour tResourceID)
 */
extern "C" void cWScriptMan_runScript(cWScriptMan* self, int* handle)
{
    sLunoValue_9C88 scriptVar;
    sLunoValue_9C88 ret;
    sLunoCall_9C88 call;
    luno_cLunoFunc_init(&scriptVar, cWorld_getWScriptScript(self->World, handle), 0);
    ret.type = 0;
    ret.word0 = 0;
    void* table = (void*)self->GlobalTables[*(unsigned char*)handle];
    luno_cLunoVM_cLunoVM(&call);
    luno_cLunoVM_run(&call, &scriptVar, table, &ret);
    luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_runScript1);
#ifdef SKIP_ASM
struct cWScriptMan_isFunc_sValue;
// cLunoValue: { word0, word1, type } (same layout as cWScriptMan_isFunc_sValue)
struct cWScriptMan_runScript1_sValue {
    int word0;
    int word1;
    int type;
};
struct cWScriptMan_runScript1_sCall {
    int pad[4];
};

void luno_cLunoFunc_init(void* self, int a1, int a2);
extern "C" void* luno_cLunoVM_cLunoVM(void* self);
extern "C" void luno_cLunoVM_run(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);

/**
 * Runs a world script by resource ID with @p id as the current instance (+0x290) during the run; the previous current
 * instance is restored afterwards. Otherwise as cWScriptMan_runScript.
 *
 * @param handle  the script's resource ID
 * @param id  the instance record to make current
 */
extern "C" void cWScriptMan_runScript1(cWScriptMan* self, int* handle, int id)
{
    cWScriptMan_runScript1_sValue scriptVar;
    cWScriptMan_runScript1_sValue ret;
    cWScriptMan_runScript1_sCall call;
    int saved = (int)self->Inst;
    self->Inst = (tInstanceRecord*)id;
    luno_cLunoFunc_init(&scriptVar, cWorld_getWScriptScript(self->World, handle), 0);
    ret.type = 0;
    ret.word0 = 0;
    void* table = (void*)self->GlobalTables[*(unsigned char*)handle];
    luno_cLunoVM_cLunoVM(&call);
    luno_cLunoVM_run(&call, &scriptVar, table, &ret);
    self->Inst = (tInstanceRecord*)saved;
    luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isFunc);
#ifdef SKIP_ASM
// cLunoValue (luno/lunovm): { word0, word1, type }
struct cWScriptMan_isFunc_sValue {
    int word0;
    int word1;
    int type;
};

void* luno_cLunoVar_castIndex(void* self);
extern "C" cWScriptMan_isFunc_sValue* luno_cLunoTable_get1(void* table, cWScriptMan_isFunc_sValue* key);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);

/**
 * Tells whether a world section's script table defines a given function.
 *
 * Returns 0 when section @p idx has no table at +0x3CC. Otherwise makes an integer key from @p value, looks it up with
 * luno_cLunoTable_get1 and checks the result's type is 5 (function).
 *
 * @param idx  the world section (On Tour sectionId)
 * @param value  the function id (On Tour funcId)
 * @return 1 when the entry is a function, else 0
 */
extern "C" int cWScriptMan_isFunc(cWScriptMan* self, int idx, int value)
{
    int off = idx << 2;
    char* base = (char*)self->GlobalTables;
    void** slot = (void**)(base + off);
    if (*slot != 0) {
        cWScriptMan_isFunc_sValue key;
        key.type = 3;
        *(int*)luno_cLunoVar_castIndex(&key) = value;
        int isFunction = luno_cLunoTable_get1(*slot, &key)->type == 5;
        luno_cLunoVar__cLunoVar(&key, 2);
        return isFunction;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_runFunc);
#ifdef SKIP_ASM
struct cWScriptMan_runFunc_sArg {
    int word0;
    int word1;
};
struct cWScriptMan_runFunc_sCall {
    int pad[4];
};

extern "C" void* luno_cLunoVM_cLunoVM(void* self);
extern "C" void luno_cLunoVM_run(void* self, void* args, void* table, void* ret);

/**
 * Runs one function of a world section's script table.
 *
 * Makes an integer key from @p value, gets the entry from the table of section @p idx (+0x3CC), builds a VM
 * (luno_cLunoVM_cLunoVM) and runs the function with that table (luno_cLunoVM_run), then destroys the result and the key.
 *
 * @param idx  the world section (On Tour sectionId)
 * @param value  the function id (On Tour funcId)
 */
extern "C" void cWScriptMan_runFunc(cWScriptMan* self, int idx, int value)
{
    cWScriptMan_isFunc_sValue key;
    key.type = 3;
    *(int*)luno_cLunoVar_castIndex(&key) = value;
    int off = idx << 2;
    char* base = (char*)self->GlobalTables;
    void** slot = (void**)(base + off);
    cWScriptMan_runFunc_sArg scriptFunc = *(cWScriptMan_runFunc_sArg*)luno_cLunoTable_get1(*slot, &key);
    cWScriptMan_isFunc_sValue ret;
    ret.type = 0;
    ret.word0 = 0;
    cWScriptMan_runFunc_sCall call;
    luno_cLunoVM_cLunoVM(&call);
    luno_cLunoVM_run(&call, &scriptFunc, *slot, &ret);
    luno_cLunoVar__cLunoVar(&ret, 2);
    luno_cLunoVar__cLunoVar(&key, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_runGlobalScripts);
#ifdef SKIP_ASM
struct cWorldView;
int cWorldView_getNumSections(cWorldView* view);

struct sWSSection9F18 {
    short type;                 // 0x0
    short pad2;
    int f4;
    int f8;
};

struct sWSWorld9F18 {
    char* view;                 // 0x0 (cWorldView at +0x10)
    struct {
        int f0;
        sWSSection9F18* sections;   // 0x4
    }* info;                    // 0x4
};


struct cWScriptIdTable;
extern "C" void cCollectPool_init(cWScriptIdTable* self);
extern "C" void cWScriptMan_runScript(cWScriptMan* self, int* idx);
extern "C" void cAvalanche_reset(void);
extern "C" int cWorld_getNumGlobalScripts(void* world, int i);
extern "C" int* cWorld_getGlobalScripts(void* world, int i);

static inline bool wsIsType3_9F18(sWSSection9F18* s)
{
    return s->type == 3;
}

/**
 * Runs the global world scripts of every type-3 section once.
 *
 * Resets the avalanches and the script id table first, and runs the scripts with no current rider and the
 * context fields cleared; the current rider (word 0) is restored afterwards.
 */
extern "C" void cWScriptMan_runGlobalScripts(cWScriptMan* self)
{
    int i;
    int saved;
    int n;
    cAvalanche_reset();
    {
        int k;
        for (k = 0; k < 1; k++) {
            cCollectPool_init((cWScriptIdTable*)self->CollectPools[k]);
        }
    }
    saved = self->Being;
    self->Being = -1;
    *(int*)&self->Inst = 0;
    *(int*)&self->CollideInfo = 0;
    *(int*)&self->IntersectInfo = 0;
    *(int*)&self->CurrentProcess = 0;
    *(int*)&self->CurrentTask = 0;
    n = cWorldView_getNumSections((cWorldView*)(((sWSWorld9F18*)self->World)->view + 0x10));
    for (i = 0; i < n; i++) {
        if (wsIsType3_9F18(&((sWSWorld9F18*)self->World)->info->sections[i])) {
            int* globalScript = cWorld_getGlobalScripts(((sWSWorld9F18*)self->World), i);
            if (globalScript != 0) {
                int cnt = cWorld_getNumGlobalScripts(((sWSWorld9F18*)self->World), i);
                int j;
                for (j = 0; j < cnt; j++, globalScript++) {
                    cWScriptMan_runScript((cWScriptMan*)self, globalScript);
                }
            }
        }
    }
    *(int*)&self->Inst = 0;
    *(int*)&self->CollideInfo = 0;
    *(int*)&self->IntersectInfo = 0;
    *(int*)&self->CurrentProcess = 0;
    *(int*)&self->CurrentTask = 0;
    self->Being = saved;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doOnCollideScript);
#ifdef SKIP_ASM
struct cWScriptMan_isFunc_sValue;
struct sValue_30A060 {
    int word0;
    int word1;
    int type;
};
struct sCall_30A060 {
    int pad;
    ~sCall_30A060() {}
};
struct sVE_30A060 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVEF_30A060 {
    short delta;
    short index;
    void (*fn)(void*, float);
};

void luno_cLunoFunc_init(void* self, int a1, int a2);
extern "C" void* luno_cLunoVM_cLunoVM(void* self);
extern "C" void luno_cLunoVM_run(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cWScriptMan_runScript(cWScriptMan* self, int* idx);
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" int cWorld_getWScriptScript(void*, int*);
extern "C" void cWScriptMan_eventCollide(cWScriptMan* self, int v);
extern char str_SetupLocalTable[];

static inline bool valid_30A060(int* id) { return *id != -1; }

/**
 * Runs the world script attached to an object a being collided with.
 *
 * Saves the current being and makes @p obj's (its virtual slot 7) current, remembering the instance (from the intersect
 * info's +0x50) and both infos. If the being's virtual slot 8 says so, a collide event for the instance is raised
 * (cWScriptMan_eventCollide), unless a non-replay rider has its +0x480 flag set, which ends the call. The instance's collide
 * script id (header entry 2) is then run directly (cWScriptMan_runScript) when no process or task is current, or otherwise in
 * the current task's or process's local table (a new 8-slot table named "SetupLocalTable" when there is none). Finally
 * the instance's +0xC object gets virtual slot 41 with 0.5, and the being and saved fields are restored.
 *
 * @param collideInfo  collision info (On Tour tc)
 * @param intersectInfo  intersection info (On Tour ti)
 * @param obj  the being that collided (On Tour pBeing)
 */
extern "C" void cWScriptMan_doOnCollideScript(cWScriptMan* self, void* collideInfo, void* intersectInfo, void* obj)
{
    int saved = self->Being;
    {
        sVE_30A060* vt = *(sVE_30A060**)obj;
        self->Being = vt[7].fn((char*)obj + vt[7].delta);
    }
    self->Inst = (tInstanceRecord*)(*(int*)((char*)intersectInfo + 0x50));
    self->CollideInfo = (tCollideInfo*)collideInfo;
    self->IntersectInfo = (tIntersectInfo*)intersectInfo;
    int raiseEvent;
    {
        sVE_30A060* vt = *(sVE_30A060**)obj;
        raiseEvent = vt[8].fn((char*)obj + vt[8].delta);
    }
    if (raiseEvent) {
        if (SSXApp->Game->mCurrentStateID != 4) {
            char* rider = obj ? (char*)obj - 0x6C0 : 0;
            if (*(int*)(rider + 0x480) != 0) {
                self->Being = saved;
                goto done;
            }
        }
        cWScriptMan_eventCollide(self, *(int*)((char*)self->Inst + 0x78));
    }
    {
        sValue_30A060 scriptFunc;
        sValue_30A060 ret;
        sCall_30A060 call;
        int id = cWorld_getWScriptHeaderId(self->World, (int)self->Inst, 2);
        if (valid_30A060(&id)) {
            if ((int)self->CurrentProcess == 0 && (int)self->CurrentTask == 0) {
                cWScriptMan_runScript(self, &id);
            } else {
                luno_cLunoFunc_init(&scriptFunc, cWorld_getWScriptScript(self->World, &id), 0);
                ret.type = 0;
                ret.word0 = 0;
                luno_cLunoVM_cLunoVM(&call);
                char* process = (char*)self->CurrentTask;
                if (process != 0)
                    process = *(char**)(process + 0x3C);
                else
                    process = (char*)self->CurrentProcess;
                void* table = *(void**)(process + 0x1C);
                if (table != 0) {
                    luno_cLunoVM_run(&call, &scriptFunc, table, &ret);
                } else {
                    table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
                    luno_cLunoVM_run(&call, &scriptFunc, table, &ret);
                    if (table != 0)
                        luno_cLunoTable__cLunoTable(table, 3);
                }
                luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
            }
        }
    }
    {
        char* instObj = *(char**)((char*)self->Inst + 0xC);
        if (instObj != 0) {
            sVEF_30A060* vt = *(sVEF_30A060**)(instObj + 0xC);
            vt[41].fn(instObj + vt[41].delta, 0.5f);
        }
    }
    self->Being = saved;
done:
    self->CollideInfo = 0;
    self->IntersectInfo = 0;
    self->Inst = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_hasOnFocusScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);

/**
 * Returns whether a world instance has an on-focus script: its world script header entry 1 (cWorld_getWScriptHeaderId
 * on the world, +0x28C) is not -1.
 *
 * @param id  the instance record (passed as an int)
 * @return 1 when it has one, else 0
 */
extern "C" int cWScriptMan_hasOnFocusScript(cWScriptMan* self, int id)
{
    return cWorld_getWScriptHeaderId(self->World, id, 1) != -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doOnFocusScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void cWScriptMan_runScript(cWScriptMan* self, int* idx);

/**
 * Runs a world instance's on-focus script: makes it the current instance (+0x290), runs header entry 1
 * (cWorld_getWScriptHeaderId, cWScriptMan_runScript) when it is not -1, then clears the current instance.
 *
 * @param id  the instance record (passed as an int)
 */
extern "C" void cWScriptMan_doOnFocusScript(cWScriptMan* self, int id)
{
    int idx;
    self->Inst = (tInstanceRecord*)id;
    idx = cWorld_getWScriptHeaderId(self->World, id, 1);
    int ok = idx != -1;
    if (ok) {
        cWScriptMan_runScript(self, &idx);
    }
    self->Inst = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_hasOnLoseFocusScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);

/**
 * Returns whether a world instance has an on-lose-focus script: its world script header entry 3 is not -1.
 *
 * @param id  the instance record (passed as an int)
 * @return 1 when it has one, else 0
 */
extern "C" int cWScriptMan_hasOnLoseFocusScript(cWScriptMan* self, int id)
{
    return cWorld_getWScriptHeaderId(self->World, id, 3) != -1;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isFocusInstance);
#ifdef SKIP_ASM
struct sEntry_A310 {
    short type;
    char pad[10];
};
/**
 * Returns whether a world instance can take the player's focus: always when it has a script controller (+0xC);
 * otherwise only a script instance (its type in the world's instance type table, indexed by the record's byte +0x78, is
 * 3) with an on-focus or on-lose-focus script (cWScriptMan_hasOnFocusScript, cWScriptMan_hasOnLoseFocusScript).
 *
 * @param obj  the instance record
 * @return 1 when it can, else 0
 */
// PORT: the helpers take the object as int in this unit.
extern "C" int cWScriptMan_isFocusInstance(cWScriptMan* self, void* obj)
{
    char* inst = (char*)obj;
    if (*(int*)(inst + 0xC) != 0) {
        return 1;
    }
    sEntry_A310* tab = *(sEntry_A310**)((char*)self->World->m_pWScriptCache + 0x4);
    bool isType3 = tab[*(unsigned char*)(inst + 0x78)].type == 3;
    if (isType3) {
        if (cWScriptMan_hasOnFocusScript(self, (int)obj) || cWScriptMan_hasOnLoseFocusScript(self, (int)obj)) {
            return 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_upFocusScript);
#ifdef SKIP_ASM
// script object controller: 0xC bytes of data, then the vptr
class cWScriptCtl {
public:
    int f0;
    int f4;
    int f8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual int hasFocusCount();
    virtual int incFocusCount(void* obj);
    virtual int decFocusCount(void* obj);
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void loseFocus();
};

struct cWScriptMan_upFocusScript_sType {
    short type;
    short pad;
    int f4;
    int f8;
};

/**
 * Handles a world instance gaining the player's focus.
 *
 * With a script controller (+0xC): when the controller's virtual slot 20 returns non-zero its slot 21 handles the
 * instance. Without one, a script instance (type 3 in the world's instance type table) runs its on-focus script when it
 * has one (cWScriptMan_hasOnFocusScript, cWScriptMan_doOnFocusScript).
 *
 * @param obj  the instance gaining focus
 */
// PORT: cWScriptMan_hasOnFocusScript/cWScriptMan_doOnFocusScript take the object pointer as an int id
extern "C" void cWScriptMan_upFocusScript(cWScriptMan* self, void* obj)
{
    cWScriptCtl* ctl = *(cWScriptCtl**)((char*)obj + 0xC);
    if (ctl != 0) {
        if (ctl->hasFocusCount() != 0) {
            (*(cWScriptCtl**)((char*)obj + 0xC))->incFocusCount(obj);
        }
    } else {
        cWScriptMan_upFocusScript_sType* instTypes = *(cWScriptMan_upFocusScript_sType**)((char*)self->World->m_pWScriptCache + 0x4);
        bool isScript = instTypes[*(unsigned char*)((char*)obj + 0x78)].type == 3;
        if (isScript) {
            if (cWScriptMan_hasOnFocusScript(self, (int)obj) != 0) {
                cWScriptMan_doOnFocusScript(self, (int)obj);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_downFocusScript);
#ifdef SKIP_ASM
// script object controller: 0xC bytes of data, then the vptr
extern "C" void cWScriptMan_doOnLoseFocusScript(cWScriptMan* self, int id);

/**
 * Handles a world instance losing the player's focus.
 *
 * If the instance has a script controller (+0xC): when the controller's virtual slot 20 says so, its slot 22 handles
 * the instance and nothing else happens; a type-6 controller does nothing; otherwise the controller's own instance
 * (+0x18) runs its lose-focus script when cWScriptMan_hasOnLoseFocusScript finds one, or the controller's slot 35 is called. An instance
 * without a controller runs its lose-focus script when it has one.
 *
 * @param obj  the instance losing focus (On Tour inst)
 */
// PORT: cWScriptMan_hasOnLoseFocusScript/cWScriptMan_doOnLoseFocusScript take the object pointer as an int id
extern "C" void cWScriptMan_downFocusScript(cWScriptMan* self, void* obj)
{
    cWScriptCtl* ctl = *(cWScriptCtl**)((char*)obj + 0xC);
    if (ctl != 0) {
        if (ctl->hasFocusCount() != 0) {
            (*(cWScriptCtl**)((char*)obj + 0xC))->decFocusCount(obj);
            return;
        }
        ctl = *(cWScriptCtl**)((char*)obj + 0xC);
        if (*(short*)((char*)ctl + 0x10) == 6) {
            return;
        }
        obj = *(void**)((char*)ctl + 0x18);
        if (cWScriptMan_hasOnLoseFocusScript(self, (int)obj) != 0) {
            cWScriptMan_doOnLoseFocusScript(self, (int)obj);
            return;
        }
        (*(cWScriptCtl**)((char*)obj + 0xC))->loseFocus();
        return;
    }
    if (cWScriptMan_hasOnLoseFocusScript(self, (int)obj) != 0) {
        cWScriptMan_doOnLoseFocusScript(self, (int)obj);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doOnLoseFocusScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void cWScriptMan_runScript(cWScriptMan* self, int* idx);

/**
 * Runs a world instance's lose-focus script.
 *
 * Stores the instance as current (+0x290), gets its script id for entry 3 from the world with
 * cWorld_getWScriptHeaderId, runs it with cWScriptMan_runScript when it is not -1 and clears the current instance.
 *
 * @param id  the instance record, passed as an int (On Tour inst)
 */
extern "C" void cWScriptMan_doOnLoseFocusScript(cWScriptMan* self, int id)
{
    int idx;
    self->Inst = (tInstanceRecord*)id;
    idx = cWorld_getWScriptHeaderId(self->World, id, 3);
    int ok = idx != -1;
    if (ok) {
        cWScriptMan_runScript(self, &idx);
    }
    self->Inst = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_hasOnExpireScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);

/**
 * Returns whether a world instance has an on-expire script: its world script header entry 4 is not -1.
 *
 * @param id  the instance record (passed as an int)
 * @return 1 when it has one, else 0
 */
extern "C" int cWScriptMan_hasOnExpireScript(cWScriptMan* self, int id)
{
    return cWorld_getWScriptHeaderId(self->World, id, 4) != -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doOnExpireScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void cWScriptMan_runScript(cWScriptMan* self, int* idx);

/**
 * Runs a world instance's on-expire script: makes it the current instance (+0x290), runs header entry 4
 * (cWorld_getWScriptHeaderId, cWScriptMan_runScript) when it is not -1, then clears the current instance.
 *
 * @param id  the instance record (passed as an int)
 */
extern "C" void cWScriptMan_doOnExpireScript(cWScriptMan* self, int id)
{
    int idx;
    self->Inst = (tInstanceRecord*)id;
    idx = cWorld_getWScriptHeaderId(self->World, id, 4);
    int ok = idx != -1;
    if (ok) {
        cWScriptMan_runScript(self, &idx);
    }
    self->Inst = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doOnInitScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void cWScriptMan_runScript(cWScriptMan* self, int* idx);

/**
 * Runs a world instance's init script: makes it the current instance (+0x290), runs header entry 0
 * (cWorld_getWScriptHeaderId, cWScriptMan_runScript) when it is not -1, then clears the current instance.
 *
 * @param id  the instance record (passed as an int)
 */
extern "C" void cWScriptMan_doOnInitScript(cWScriptMan* self, int id)
{
    int idx;
    self->Inst = (tInstanceRecord*)id;
    idx = cWorld_getWScriptHeaderId(self->World, id, 0);
    int ok = idx != -1;
    if (ok) {
        cWScriptMan_runScript(self, &idx);
    }
    self->Inst = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_hasOnFrameUpdateScript);
#ifdef SKIP_ASM
/**
 * Returns whether a world instance has a frame-update script: its world script header entry 5 is not -1.
 *
 * @param scriptId  the instance record (passed as an int)
 * @return 1 when it has one, else 0
 */
extern "C" int cWScriptMan_hasOnFrameUpdateScript(cWScriptMan* self, int scriptId)
{
    return cWorld_getWScriptHeaderId(self->World, scriptId, 5) != -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doOnFrameUpdateScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void cWScriptMan_runScript(cWScriptMan* self, int* idx);

/**
 * Runs a world instance's frame-update script: makes it the current instance (+0x290), runs header entry 5
 * (cWorld_getWScriptHeaderId, cWScriptMan_runScript) when it is not -1, then clears the current instance.
 *
 * @param id  the instance record (passed as an int)
 */
extern "C" void cWScriptMan_doOnFrameUpdateScript(cWScriptMan* self, int id)
{
    int idx;
    self->Inst = (tInstanceRecord*)id;
    idx = cWorld_getWScriptHeaderId(self->World, id, 5);
    int ok = idx != -1;
    if (ok) {
        cWScriptMan_runScript(self, &idx);
    }
    self->Inst = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_gotoTask);
#ifdef SKIP_ASM
extern "C" void cWScriptProcess_gotoTask(void* p);

/**
 * Jumps the current process (+0x2A4) to a task: cWScriptProcess_gotoTask, with the task ID passing through in the second argument
 * register (the C declares only the manager). Nothing happens without a current process.
 */
extern "C" void cWScriptMan_gotoTask(cWScriptMan* self)
{
    void* process = self->CurrentProcess;
    if (process != 0) {
        cWScriptProcess_gotoTask(process);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_advanceToTask);
#ifdef SKIP_ASM
extern "C" void cWScriptProcess_advanceToTask(void* p);

/**
 * Advances the current process (+0x2A4) to a task: cWScriptProcess_advanceToTask, with the task ID passing through in the second
 * argument register (the C declares only the manager). Nothing happens without a current process.
 */
extern "C" void cWScriptMan_advanceToTask(cWScriptMan* self)
{
    void* process = self->CurrentProcess;
    if (process != 0) {
        cWScriptProcess_advanceToTask(process);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doProcessCmd);
#ifdef SKIP_ASM
extern "C" void* cProcessList_findProcess1(void* self, int id);
extern "C" void cWScriptProcess_end(void* obj);
extern "C" void cWScriptProcess_endTask(void* obj);

/**
 * Carries out a script command on a world script process.
 *
 * With @p id -1 the current process (+0x2A4) is used, otherwise the process is looked up with cProcessList_findProcess1. Command 1
 * looks in the idle list (+0x2B4) and starts it (cWScriptMan_startProcess); 0 stops an active one (cWScriptMan_stopProcess); 2 ends its
 * current task (cWScriptProcess_endTask); 18 ends the process (cWScriptProcess_end). Other commands are ignored.
 *
 * @param id  the process id, or -1 for the current process (On Tour processId)
 * @param kind  the command (On Tour cmd)
 */
extern "C" void cWScriptMan_doProcessCmd(cWScriptMan* self, unsigned int id, int kind)
{
    void* obj = self->CurrentProcess;
    switch (kind) {
    case 1:
        if (id != 0xFFFFFFFF) {
            obj = cProcessList_findProcess1((char*)&self->IdleList, id);
        }
        if (obj != 0) {
            cWScriptMan_startProcess(self, (cWScriptProcess*)obj);
        }
        break;
    case 0:
        if (id != 0xFFFFFFFF) {
            obj = cProcessList_findProcess1((char*)&self->ActiveList, id);
        }
        if (obj != 0) {
            cWScriptMan_stopProcess(self, (cWScriptProcess*)obj);
        }
        break;
    case 2:
        if (id != 0xFFFFFFFF) {
            obj = cProcessList_findProcess1((char*)&self->ActiveList, id);
        }
        if (obj != 0) {
            cWScriptProcess_endTask(obj);
        }
        break;
    case 18:
        if (id != 0xFFFFFFFF) {
            obj = cProcessList_findProcess1((char*)&self->ActiveList, id);
        }
        if (obj != 0) {
            cWScriptProcess_end(obj);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doMissionCmd);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cWScriptMan_doProcessCmd(cWScriptMan* self, unsigned int id, int kind);
// PORT: the unit defines tMissionMenuQueue_addEntry later as (cWSQueue*, int, int, int)
extern "C" void tMissionMenuQueue_addEntry_a(void* queue, unsigned int id, int kind, int ctx) __asm__("tMissionMenuQueue_addEntry");
extern "C" void* cWScriptMan_getCurrentMission(cWScriptMan* self);
extern "C" int cWScriptMan_isLocalMissionActive(cWScriptMan* self);
extern "C" int cBEMissionInterface_isMissionCompleted(void* iface, unsigned int id);
extern "C" int cBEMissionInterface_isMissionReady(void* iface, unsigned int id);
extern "C" int cBEMissionInterface_isMissionLocked(void* iface, unsigned int id);
extern "C" int cBEMissionInterface_isMissionAvailable(void* iface, unsigned int id);
extern "C" int cBEMissionInterface_isMissionShowIcon(void* iface, unsigned int id);
extern "C" int cBEMissionInterface_getNextMissionID(void* iface, unsigned int id);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_BigChallengeBegin(void* p, unsigned int id);
extern "C" void cSSXAudio_BigChallengeFailed(void* p, int a1);

class cWSObj_30A868 {
public:
    char pad_0x00[0x20];
    unsigned int id;        // 0x20
    char pad_0x24[0x5C - 0x24];
    // vptr at 0x5C
    virtual void v01();
    virtual void processComplete();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void missionRestart();
    virtual void v18();
    virtual void missionFail();
    virtual void missionUpdateWorld();
};

// PORT: the unit defines cProcessList_findMission later with its own class type
extern "C" cWSObj_30A868* cProcessList_findMission_a(void* self, unsigned int id) __asm__("cProcessList_findMission");

/**
 * Carries out a mission command from a world script, or answers a question about a mission.
 *
 * Except for commands 15 and 19, nothing happens unless newRace.cGameMode is 4 and the AI's +0x7C is 1. The rider is
 * temporarily set to @p ctx. Commands 0-2, 18 and 8/9/11 are forwarded to helpers; 3, 4, 5 and 17 call
 * virtuals of the running (or waiting) mission. Queries use the mission interface: 14 available, 15
 * completed, 19 last in its chain, 12, 13 and 16 other states. An id of -1 means the current mission.
 *
 * @param id   mission id, or -1 for the current one (On Tour missionId)
 * @param cmd  the command
 * @param ctx  rider index to run it for, or -1 for the current one (On Tour being)
 * @return the query's answer (1 or 0); 0 for plain commands
 */
extern "C" int cWScriptMan_doMissionCmd(cWScriptMan* self, unsigned int id, unsigned int cmd, int ctx)
{
    int force = 0;
    if (cmd == 15 || cmd == 19) {
        force = 1;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (force == 0) {
        if (cBENewRaceInterface_newRace.cGameMode != 4) {
            return 0;
        }
        if (SSXApp->Game->AI->NumberOfPlayersAllocated != 1) {
            return 0;
        }
    }
    int saved = self->Being;
    if (ctx != -1) {
        self->Being = ctx;
    }
    switch (cmd) {
    case 0:
    case 1:
    case 2:
    case 18:
        cWScriptMan_doProcessCmd(self, id, cmd);
        break;
    case 8:
    case 9:
    case 11: {
        int replayState = *(int*)SSXApp->Game->Replay;
        int busy = replayState != 0 && replayState < 10;
        if (!busy) {
            tMissionMenuQueue_addEntry_a(&self->MissionMenuQueue, id, cmd, self->Being);
        }
        break;
    }
    }
    int result = 0;
    cWSObj_30A868* obj = (cWSObj_30A868*)cWScriptMan_getCurrentMission(self);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 10);
    switch (cmd) {
    case 0:
        break;
    case 3:
        if (id != 0xFFFFFFFF) {
            obj = cProcessList_findMission_a((char*)&self->ActiveList, id);
            if (obj == 0) {
                obj = cProcessList_findMission_a((char*)&self->IdleList, id);
            }
        }
        if (obj != 0) {
            obj->missionRestart();
            cSSXAudio_BigChallengeBegin(cAudioInterface_bxAudio(), id);
        }
        break;
    case 4:
        if (id != 0xFFFFFFFF) {
            obj = cProcessList_findMission_a((char*)&self->ActiveList, id);
        }
        if (obj != 0) {
            obj->processComplete();
        }
        break;
    case 5:
        if (id != 0xFFFFFFFF) {
            obj = cProcessList_findMission_a((char*)&self->ActiveList, id);
        }
        if (obj != 0) {
            obj->missionFail();
            cSSXAudio_BigChallengeFailed(cAudioInterface_bxAudio(), 0);
        }
        break;
    case 17:
        if (id != 0xFFFFFFFF) {
            obj = cProcessList_findMission_a((char*)&self->ActiveList, id);
            if (obj == 0) {
                obj = cProcessList_findMission_a((char*)&self->IdleList, id);
            }
        }
        if (obj != 0) {
            obj->missionUpdateWorld();
        }
        break;
    case 13:
        if (id == 0xFFFFFFFF) {
            if (obj != 0) {
                id = obj->id;
            }
            if (id == 0xFFFFFFFF) {
                break;
            }
        }
        result = cBEMissionInterface_isMissionLocked(iface, id) != 0;
        break;
    case 12:
        if (id == 0xFFFFFFFF) {
            if (obj != 0) {
                id = obj->id;
            }
            if (id == 0xFFFFFFFF) {
                break;
            }
        }
        result = cBEMissionInterface_isMissionReady(iface, id) != 0;
        break;
    case 16:
        if (id == 0xFFFFFFFF) {
            if (obj != 0) {
                id = obj->id;
            }
            if (id == 0xFFFFFFFF) {
                break;
            }
        }
        if (self->LocalMissionCmd == 1) {
            break;
        }
        if (cWScriptMan_isLocalMissionActive(self) != 0) {
            break;
        }
        if (cBEMissionInterface_isMissionShowIcon(iface, id) != 0) {
            result = 1;
        } else {
            result = 0;
        }
        break;
    case 19:
        if (id == 0xFFFFFFFF) {
            if (obj != 0) {
                id = obj->id;
            }
            if (id == 0xFFFFFFFF) {
                break;
            }
        }
        result = cBEMissionInterface_getNextMissionID(iface, id) == -1;
        break;
    case 14:
        if (id == 0xFFFFFFFF) {
            if (obj != 0) {
                id = obj->id;
            }
            if (id == 0xFFFFFFFF) {
                break;
            }
        }
        result = cBEMissionInterface_isMissionAvailable(iface, id) != 0;
        break;
    case 15:
        if (id == 0xFFFFFFFF) {
            if (obj != 0) {
                id = obj->id;
            }
            if (id == 0xFFFFFFFF) {
                break;
            }
        }
        result = cBEMissionInterface_isMissionCompleted(iface, id) != 0;
        break;
    }
    self->Being = saved;
    return result;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_runScriptCode);
#ifdef SKIP_ASM
struct cWScriptMan_isFunc_sValue;
void luno_cLunoFunc_init(void* self, int a1, int a2);
extern "C" void* luno_cLunoVM_cLunoVM(void* self);
extern "C" void luno_cLunoVM_run(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern char str_SetupLocalTable[];

// cLunoValue: { word0, word1, type }
struct cWScriptMan_runScriptCode_sValue {
    int word0;
    int word1;
    int type;
};
struct cWScriptMan_runScriptCode_sCall {
    int pad[4];
};

/**
 * Runs a piece of world-script (Luno) code for a process.
 *
 * Makes @p obj the current process, then runs the code with the process's local table (+0x1C) or, if it has
 * none, a temporary 8-slot table ("SetupLocalTable"). The current process and +0x290 are restored afterwards.
 *
 * @param obj   the process (On Tour cWScriptProcess*)
 * @param data  the compiled code (On Tour const char*)
 */
extern "C" void cWScriptMan_runScriptCode(cWScriptMan* self, void* obj, int data)
{
    cWScriptMan_runScriptCode_sValue scriptFunc;
    cWScriptMan_runScriptCode_sValue ret;
    cWScriptMan_runScriptCode_sCall call;
    int savedId;
    void* savedObj = self->CurrentProcess;
    self->CurrentProcess = (cWScriptProcess*)obj;
    savedId = (int)self->Inst;
    luno_cLunoFunc_init(&scriptFunc, data, 0);
    ret.type = 0;
    ret.word0 = 0;
    luno_cLunoVM_cLunoVM(&call);
    void* table = self->CurrentProcess->Local;
    if (table != 0) {
        luno_cLunoVM_run(&call, &scriptFunc, table, &ret);
    } else {
        table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
        luno_cLunoVM_run(&call, &scriptFunc, table, &ret);
        if (table != 0) {
            luno_cLunoTable__cLunoTable(table, 3);
        }
    }
    self->CurrentProcess = (cWScriptProcess*)savedObj;
    self->Inst = (tInstanceRecord*)savedId;
    luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_runScriptCode1);
#ifdef SKIP_ASM
struct cWScriptMan_isFunc_sValue;
void luno_cLunoFunc_init(void* self, int a1, int a2);
extern "C" void* luno_cLunoVM_cLunoVM(void* self);
extern "C" void luno_cLunoVM_run(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern char str_SetupLocalTable[];

// cLunoValue: { word0, word1, type }
struct cWScriptMan_runScriptCode1_sValue {
    int word0;
    int word1;
    int type;
};
struct cWScriptMan_runScriptCode1_sCall {
    int pad[4];
};

/**
 * Runs a piece of world script code for a script task, discarding its result.
 *
 * Makes @p obj the current task (+0x2A8) and runs a Luno value built from @p scriptCode (luno_cLunoFunc_init) in a fresh Luno VM
 * against the local table (+0x1C) of the task's process (+0x3C). Without a local table a temporary one is allocated
 * ("SetupLocalTable"), used and destroyed. The current task and the current instance (+0x290) are restored afterwards.
 *
 * @param obj  the script task
 * @param scriptCode  the script code
 */
extern "C" void cWScriptMan_runScriptCode1(cWScriptMan* self, void* obj, int scriptCode)
{
    cWScriptMan_runScriptCode1_sValue codeVar;
    cWScriptMan_runScriptCode1_sValue ret;
    cWScriptMan_runScriptCode1_sCall call;
    int savedId;
    void* savedObj = self->CurrentTask;
    self->CurrentTask = (cWScriptTask*)obj;
    savedId = (int)self->Inst;
    luno_cLunoFunc_init(&codeVar, scriptCode, 0);
    ret.type = 0;
    ret.word0 = 0;
    luno_cLunoVM_cLunoVM(&call);
    void* table = self->CurrentTask->Process->Local;
    if (table != 0) {
        luno_cLunoVM_run(&call, &codeVar, table, &ret);
    } else {
        table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
        luno_cLunoVM_run(&call, &codeVar, table, &ret);
        if (table != 0) {
            luno_cLunoTable__cLunoTable(table, 3);
        }
    }
    self->CurrentTask = (cWScriptTask*)savedObj;
    self->Inst = (tInstanceRecord*)savedId;
    luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getCurrentMission);
#ifdef SKIP_ASM
struct sVEntry0030AEB8 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Returns the current world script process if it is a mission.
 *
 * @return the process at +0x2A4 when it exists and its virtual slot 3 returns non-zero, else 0
 */
extern "C" void* cWScriptMan_getCurrentMission(cWScriptMan* self)
{
    cWScriptProcess* obj = self->CurrentProcess;
    if (obj != 0) {
        sVEntry0030AEB8* vt = (sVEntry0030AEB8*)obj->mVtbl;
        if (vt[3].fn((char*)obj + vt[3].delta) != 0) {
            return self->CurrentProcess;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getCurrentTrick);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sScriptFlags_AF08 {
    unsigned int f0 : 3;
    unsigned int f1 : 4;
    unsigned int f2 : 3;
    unsigned int f3 : 2;
    unsigned int f4 : 4;
    unsigned short f5 : 3;
    unsigned int f6 : 3;
    unsigned int f7 : 6;
    unsigned int f8 : 4;
    unsigned int f9 : 3;
    unsigned int f10 : 7;
    unsigned int f11 : 1;
    unsigned int pad : 7;
    unsigned int f13 : 2;
    unsigned int f15 : 4;
    unsigned char f14 : 3;
};

/**
 * Returns field @p which of the current trick definition (+0x29C, packed bit fields): fields 0-11 and 13-15; field 12,
 * any other index and a missing definition give 0.
 *
 * @param beingIndex  the being (unused)
 * @param which  the field index
 * @return the field value
 */
// Reads one packed script setting; 0 when no settings block is loaded.
extern "C" int cWScriptMan_getCurrentTrick(cWScriptMan* self, int beingIndex, int which)
{
    sScriptFlags_AF08* fl = (sScriptFlags_AF08*)self->TrickDef;
    if (fl != 0) {
        switch (which) {
        case 0: return ((sScriptFlags_AF08*)self->TrickDef)->f0;
        case 1: return ((sScriptFlags_AF08*)self->TrickDef)->f1;
        case 2: return ((sScriptFlags_AF08*)self->TrickDef)->f2;
        case 3: return ((sScriptFlags_AF08*)self->TrickDef)->f3;
        case 4: return ((sScriptFlags_AF08*)self->TrickDef)->f4;
        case 5: return ((sScriptFlags_AF08*)self->TrickDef)->f5;
        case 6: return ((sScriptFlags_AF08*)self->TrickDef)->f6;
        case 7: return ((sScriptFlags_AF08*)self->TrickDef)->f7;
        case 8: return ((sScriptFlags_AF08*)self->TrickDef)->f8;
        case 9: return ((sScriptFlags_AF08*)self->TrickDef)->f9;
        case 10: return ((sScriptFlags_AF08*)self->TrickDef)->f10;
        case 11: return ((sScriptFlags_AF08*)self->TrickDef)->f11;
        case 13: return ((sScriptFlags_AF08*)self->TrickDef)->f13;
        case 14: return ((sScriptFlags_AF08*)self->TrickDef)->f14;
        case 15: return ((sScriptFlags_AF08*)self->TrickDef)->f15;
        }
    }
    return 0;
}
#endif

struct cWScriptListNode {
    char pad_0x00[0x18];
    void* next;
};

struct cWScriptListHead {
    void* head;
};

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_cProcessList__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty world script process list: its head (+0x0) is 0.
 *
 * @return @p self
 */
void* cProcessList_cProcessList(void* self)
{
    ((cWScriptListHead*)self)->head = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList__cProcessList);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

class cProcessList__cProcessList_cNode {
public:
    char pad_0x00[0x18];
    cProcessList__cProcessList_cNode* next;   // 0x18
    char pad_0x1C[0x5C - 0x1C];
    // vptr at 0x5C; slot N at vtable offset N*8
    virtual void v01(int);
};

/**
 * Destroys a script process list and every process in it.
 *
 * Walks the list from the head (+0x0) by each process's next link (+0x18), deleting each through its virtual destructor
 * (vtable at +0x5C), then calls operator delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
extern "C" void cProcessList__cProcessList(cProcessList__cProcessList_cNode** self, int flags)
{
    cProcessList__cProcessList_cNode* n = *self;
    while (n != 0) {
        cProcessList__cProcessList_cNode* cur = n;
        n = n->next;
        if (cur != 0) {
            cur->v01(3);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_unloadSection);
#ifdef SKIP_ASM
struct sVE_B0E8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sNode_B0E8 {
    char pad0[0x18];
    sNode_B0E8* next;   // 0x18
    char pad1C[0x24 - 0x1C];
    int key;            // 0x24
    char pad28[0x5C - 0x28];
    sVE_B0E8* vt;       // 0x5C
};

static inline void destroy_B0E8(sNode_B0E8* n)
{
    if (n != 0) {
        n->vt[1].fn((char*)n + n->vt[1].delta, 3);
    }
}

/**
 * Destroys the processes with a given section key at the front of the list.
 *
 * First destroys the leading run of nodes with @p key, advancing @p head past each. Then destroys the run of
 * matching nodes right after the new head, stopping at the first node with another key. That second run is
 * destroyed without updating the head's next link.
 *
 * @param head  pointer to the list head; updated as leading nodes are destroyed
 * @param key   section key of the processes to unload
 */
// Deletes the leading run of nodes with the given key, then the run after the new head.
extern "C" void cProcessList_unloadSection(sNode_B0E8** head, int key)
{
    if (*head == 0) {
        return;
    }
    sNode_B0E8* next = (*head)->next;
    if ((*head)->key == key) {
        for (;;) {
            destroy_B0E8(*head);
            *head = next;
            sNode_B0E8* cur = next;
            if (cur == 0) {
                return;
            }
            next = cur->next;
            if (cur->key != key) {
                break;
            }
        }
    }
    if (*head == 0) {
        return;
    }
    sNode_B0E8* n = (*head)->next;
    while (n != 0) {
        sNode_B0E8* cur = n;
        n = cur->next;
        if (cur->key != key) {
            break;
        }
        destroy_B0E8(cur);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_deleteAll);
#ifdef SKIP_ASM
class cProcessList_deleteAll_cObj {
public:
    char pad_0x00[0x5C];
    virtual ~cProcessList_deleteAll_cObj(); // vptr at 0x5C
};

extern "C" void* cProcessList_removeHead(void* self);

/**
 * Deletes every process in a script process list.
 *
 * Repeatedly takes the head with cProcessList_removeHead and deletes it through its virtual destructor until none is left.
 */
extern "C" void cProcessList_deleteAll(void* self)
{
    cProcessList_deleteAll_cObj* process;
    while ((process = (cProcessList_deleteAll_cObj*)cProcessList_removeHead(self)) != 0) {
        delete process;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_removeHead);
#ifdef SKIP_ASM
/**
 * Takes the first process off a script process list.
 *
 * @return the old head, or 0 when the list is empty; the head becomes its next process
 */
extern "C" void* cProcessList_removeHead(void* self)
{
    cWScriptListNode* process = (cWScriptListNode*)((cWScriptListHead*)self)->head;
    if (process != 0) {
        ((cWScriptListHead*)self)->head = process->next;
    }
    return process;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_getCount);
#ifdef SKIP_ASM
/**
 * Counts the processes in a world-script process list.
 *
 * Walks the singly linked list from head to the end.
 *
 * @return the number of processes
 */
extern "C" int cProcessList_getCount(cWScriptListHead* self)
{
    int n = 0;
    cWScriptListNode* node = (cWScriptListNode*)self->head;
    while (node != 0) {
        node = (cWScriptListNode*)node->next;
        n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_addProcess__FPvT0);
#ifdef SKIP_ASM
/**
 * Pushes a world script process onto the front of a process list: its next link (+0x18) takes the old head.
 *
 * @param node  the process
 * @return the previous head
 */
int cProcessList_addProcess(void* self, void* node)
{
    int oldHead = (int)((cWScriptListHead*)self)->head;
    ((cWScriptListNode*)node)->next = (void*)oldHead;
    ((cWScriptListHead*)self)->head = node;
    return oldHead;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_appendProcess);
#ifdef SKIP_ASM
/**
 * Appends a world-script process at the end of a process list.
 *
 * Clears the process's next link, then links it after the current last node, or makes it the head of an empty list.
 *
 * @param node  the process to add (On Tour cWScriptProcess* process)
 */
extern "C" void cProcessList_appendProcess(cWScriptListHead* self, cWScriptListNode* node)
{
    node->next = 0;
    cWScriptListNode* tail = (cWScriptListNode*)self->head;
    if (tail != 0) {
        while (tail->next != 0) {
            tail = (cWScriptListNode*)tail->next;
        }
        tail->next = node;
    } else {
        self->head = node;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_addProcess1__FPvT0);
#ifdef SKIP_ASM
/**
 * Pushes a world script mission onto the front of a process list: its next link (+0x18) takes the old head.
 *
 * @param node  the mission
 * @return the previous head
 */
int cProcessList_addProcess1(void* self, void* node)
{
    int oldHead = (int)((cWScriptListHead*)self)->head;
    ((cWScriptListNode*)node)->next = (void*)oldHead;
    ((cWScriptListHead*)self)->head = node;
    return oldHead;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_removeProcess);
#ifdef SKIP_ASM
/**
 * Unlinks a process from a singly linked process list.
 *
 * @param node  the process to remove
 * @return 1 when the process was found and removed, otherwise 0
 */
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node)
{
    if (node == self->head) {
        self->head = node->next;
        return 1;
    }
    cWScriptListNode* current = (cWScriptListNode*)self->head;
    if (current != 0) {
        cWScriptListNode* prev = current;
        do {
            current = (cWScriptListNode*)current->next;
            if (current == node) {
                prev->next = current->next;
                return 1;
            }
            prev = current;
        } while (current != 0);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_findProcess1);
#ifdef SKIP_ASM
/**
 * Returns the process with ID @p id (+0x20) in a process list, following the next links (+0x18).
 *
 * @param id  the process ID
 * @return the process, or 0
 */
extern "C" void* cProcessList_findProcess1(void* self, int id)
{
    void* process = *(void**)self;
    while (process != 0) {
        if (*(int*)((char*)process + 0x20) == id) {
            return process;
        }
        process = *(void**)((char*)process + 0x18);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_findProcess2);
#ifdef SKIP_ASM
extern "C" int GetHashValue32__FPc(int);

/**
 * Returns the process whose ID is the hash of @p name (GetHashValue32__FPc) in a process list (cProcessList_findProcess1). The C
 * declares no return value; the lookup's result is left in the return register.
 *
 * @param list  the process list
 * @param name  the process name
 */
extern "C" void cProcessList_findProcess2(void *list, int name) {
    cProcessList_findProcess1(list, GetHashValue32__FPc(name));
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_findMission);
#ifdef SKIP_ASM
class cProcessList_findMission_cObj {
public:
    char pad_0x00[0x5C];
    // vptr at 0x5C
    virtual void v01();
    virtual void v02();
    virtual int isMission(); // 0x18
};

extern "C" void* cProcessList_findProcess1(void*, int);

/**
 * Returns the mission with ID @p id in a process list: the process found by cProcessList_findProcess1 when its virtual slot 3 (the
 * mission test) returns non-zero.
 *
 * @param id  the mission ID, or -1
 * @return the mission, or 0 (also for -1)
 */
extern "C" cProcessList_findMission_cObj* cProcessList_findMission(void* self, unsigned int id)
{
    cProcessList_findMission_cObj* result = 0;
    if (id != 0xFFFFFFFF) {
        cProcessList_findMission_cObj* process = (cProcessList_findMission_cObj*)cProcessList_findProcess1(self, id);
        if (process != 0) {
            if (process->isMission()) {
                result = process;
            }
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_findMission1);
#ifdef SKIP_ASM
/**
 * Returns the mission whose ID is the hash of @p name (GetHashValue32__FPc) in a process list (cProcessList_findMission). The C
 * declares no return value; the lookup's result is left in the return register.
 *
 * @param name  the mission name
 */
extern "C" void cProcessList_findMission1(void* self, int name)
{
    cProcessList_findMission(self, GetHashValue32__FPc(name));
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cProcessList_findProcess);
#ifdef SKIP_ASM
/**
 * Tells whether a process is on a process list.
 *
 * @param node  the process to look for
 * @return 1 when the process is on the list, otherwise 0
 */
extern "C" int cProcessList_findProcess(cWScriptListHead* self, cWScriptListNode* node)
{
    cWScriptListNode* process;
    for (process = (cWScriptListNode*)self->head; process != 0; process = (cWScriptListNode*)process->next) {
        if (process == node) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_findMission);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cProcessList_findMission_vv(void* self, void* node) __asm__("cProcessList_findMission");
/**
 * Returns the mission with ID @p node from the active list (+0x2B8), else the idle list (+0x2B4), else the start list
 * (+0x2BC) (cProcessList_findMission). The C declares no return value; the result is left in the return register.
 *
 * @param node  the mission ID
 */
extern "C" void cWScriptMan_findMission(cWScriptMan* self, void* node) {
    if (cProcessList_findMission_vv(&self->ActiveList, node) == 0 &&
        cProcessList_findMission_vv(&self->IdleList, node) == 0) {
        cProcessList_findMission_vv(&self->AddList, node);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_render__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the world script manager's render hook).
 */
void cWScriptMan_render(void* self)
{
}
#endif

// self+0x4, +0x10, +0x1c, +0x28 are 0xc-byte slots of shape
// { int enabled; int valueB; int valueC; } (see cWScriptSlot below);
// kept as raw offsets here since struct-pointer codegen regresses
// these below their current objdiff match on this compiler.
struct cWScriptSlot {
    int enabled;
    int valueB;
    int valueC;
};

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_updateMissionPoints__FPvii);
#ifdef SKIP_ASM
/**
 * Turns on the mission points display (+0x4) with @p points (+0x8) of @p goal (+0xC).
 *
 * @param points  the current points
 * @param goal  the points goal
 */
void cWScriptMan_updateMissionPoints(void* vself, int points, int goal)
{
    cWScriptMan* self = (cWScriptMan*)vself;
    self->MissionPointsEnable = 1;
    self->MissionPoints = points;
    self->MissionPointsGoal = goal;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_updateMissionCount__FPvii);
#ifdef SKIP_ASM
/**
 * Turns on the mission count display (+0x10) with @p count (+0x14) of @p maxCount (+0x18).
 *
 * @param count  the current count
 * @param maxCount  the maximum count
 */
void cWScriptMan_updateMissionCount(void* vself, int count, int maxCount)
{
    cWScriptMan* self = (cWScriptMan*)vself;
    self->MissionCountEnable = 1;
    self->MissionCount = count;
    self->MissionCountMax = maxCount;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_updateMissionTicks__FPvii);
#ifdef SKIP_ASM
/**
 * Turns on the mission clock display (+0x1C) with @p ticks (+0x20) counting in direction @p dir (+0x24).
 *
 * @param ticks  the clock ticks
 * @param dir  the clock direction
 */
void cWScriptMan_updateMissionTicks(void* vself, int ticks, int dir)
{
    cWScriptMan* self = (cWScriptMan*)vself;
    self->MissionTicksEnable = 1;
    self->MissionTicks = ticks;
    self->MissionTicksDir = dir;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_updateMissionHeight__FPviT0);
#ifdef SKIP_ASM
/**
 * Turns on the mission height display (+0x28) for rider @p being (+0x30) above instance @p src (its resource ID, +0x78,
 * goes to +0x2C).
 *
 * @param being  the rider index
 * @param src  the instance record
 */
void cWScriptMan_updateMissionHeight(void* vself, int being, void* src)
{
    cWScriptMan* self = (cWScriptMan*)vself;
    self->MissionHeightEnable = 1;
    self->MissionHeightBeing = being;
    self->MissionHeightInstId = *(int*)((char*)src + 0x78);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_updateMissionTrick);
#ifdef SKIP_ASM
struct cWScriptPair {
    int valueB;
    int valueC;
};

/**
 * Turns on the mission trick display (+0x34) and copies the 8-byte trick definition @p src to +0x38.
 *
 * @param src  the trick definition
 */
extern "C" void cWScriptMan_updateMissionTrick(cWScriptMan* self, cWScriptPair* src)
{
    self->MissionTrickEnable = 1;
    *(cWScriptPair*)&self->MissionTrick[0] = *src;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_startLocalMission);
#ifdef SKIP_ASM
extern "C" void cPlaybackMan_stop(void* p);
extern "C" void cReplay_reset(void* p);

/**
 * Requests the start of the local mission: stops the playback manager (cPlaybackMan_stop), resets the replay
 * (cReplay_reset) and queues mission command 1 (+0x2AC) for the local mission ID (+0x2A0, copied to +0x2B0).
 */
extern "C" void cWScriptMan_startLocalMission(cWScriptMan* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    self->LocalMissionCmd = 1;
    self->LocalMissionCmdId = (unsigned int)((int)self->LocalMissionId);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_previewLocalMission);
#ifdef SKIP_ASM
extern "C" int cPlaybackMan_setupPreview(void*, int);
/**
 * Requests a preview of the local mission: stops the playback manager and resets the replay, then loads the mission's
 * playback (cPlaybackMan_setupPreview with the local mission ID, +0x2A0); on success queues mission command 5 (+0x2AC) for that
 * mission (+0x2B0).
 *
 * @return 1 when the preview was queued, else 0
 */
extern "C" int cWScriptMan_previewLocalMission(cWScriptMan* self) {
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    if (cPlaybackMan_setupPreview(SSXApp->Game->PlaybackMan, (int)self->LocalMissionId) != 0) {
        self->LocalMissionCmd = 5;
        self->LocalMissionCmdId = (unsigned int)((int)self->LocalMissionId);
        return 1;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_cancelPreviewLocalMission);
#ifdef SKIP_ASM
void cPlaybackMan_reset(void* self);

/**
 * Cancels the local mission preview: stops and resets the playback manager (cPlaybackMan_stop, cPlaybackMan_reset) and
 * queues mission command 6 (+0x2AC) for the local mission (+0x2A0, copied to +0x2B0).
 */
extern "C" void cWScriptMan_cancelPreviewLocalMission(cWScriptMan* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cPlaybackMan_reset(SSXApp->Game->PlaybackMan);
    self->LocalMissionCmd = 6;
    self->LocalMissionCmdId = (unsigned int)((int)self->LocalMissionId);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_cancelLocalMission);
#ifdef SKIP_ASM
extern "C" void cPlaybackMan_stop(void* p);
void cPlaybackMan_reset(void* self);
extern "C" void cReplay_reset(void* p);
extern "C" void cReplay_startRace(int* self);
extern "C" void cRider_resetForMission(void* self);

/**
 * Cancels the local (big challenge) mission and resets the rider.
 *
 * Stops and resets the playback manager (cPlaybackMan_stop, cPlaybackMan_reset__FPv), resets the replay and restarts its race, resets
 * the rider whose index is at +0x0 for the mission, then sets the state at +0x2AC to 3, saves the mission id (+0x2A0)
 * in +0x2B0 and sets the id to -1.
 */
extern "C" void cWScriptMan_cancelLocalMission(cWScriptMan* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cPlaybackMan_reset(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    cReplay_startRace((int*)SSXApp->Game->Replay);
    char* riders = (char*)SSXApp->Game->AI;
    cRider_resetForMission(*(void**)(riders + (self->Being << 2) + 0x28));
    self->LocalMissionCmd = 3;
    self->LocalMissionCmdId = (unsigned int)((int)self->LocalMissionId);
    self->LocalMissionId = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_restartLocalMission);
#ifdef SKIP_ASM
extern "C" void cPlaybackMan_stop(void* p);
extern "C" void cReplay_reset(void* p);
extern "C" void cReplay_startRace(int* self);

/**
 * Restarts the local (big challenge) mission.
 *
 * Stops the playback manager (cPlaybackMan_stop), resets the replay and restarts its race, sets the state at +0x2AC to 2
 * and copies the mission id (+0x2A0) to +0x2B0.
 */
extern "C" void cWScriptMan_restartLocalMission(cWScriptMan* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    cReplay_startRace((int*)SSXApp->Game->Replay);
    self->LocalMissionCmd = 2;
    self->LocalMissionCmdId = (unsigned int)((int)self->LocalMissionId);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_quitLocalMission);
#ifdef SKIP_ASM
extern "C" void cPlaybackMan_stop(void* p);
void cPlaybackMan_reset(void* self);
extern "C" void cReplay_reset(void* p);
extern "C" void cReplay_startRace(int* self);
extern "C" void cRider_resetForMission(void* self);

/**
 * Quits the local (big challenge) mission and resets the rider.
 *
 * Stops and resets the playback manager, resets the replay and restarts its race, resets the rider whose index is at
 * +0x0 for the mission, then sets the state at +0x2AC to 4, saves the mission id (+0x2A0) in +0x2B0 and sets the id to
 * -1.
 */
extern "C" void cWScriptMan_quitLocalMission(cWScriptMan* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cPlaybackMan_reset(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    cReplay_startRace((int*)SSXApp->Game->Replay);
    char* riders = (char*)SSXApp->Game->AI;
    cRider_resetForMission(*(void**)(riders + (self->Being << 2) + 0x28));
    self->LocalMissionCmd = 4;
    self->LocalMissionCmdId = (unsigned int)((int)self->LocalMissionId);
    self->LocalMissionId = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_failActiveLocalMission);
#ifdef SKIP_ASM
extern "C" void cWScriptProcess_end(void* obj);
extern "C" void tMissionMenuQueue_empty(tMissionMenuQueue* self);

/**
 * Fails the local mission in progress and stops every active script process.
 *
 * When a mission id is set (+0x2A0), ends that mission if it is active (cWScriptProcess_end) and clears the state
 * (+0x2AC = 0, +0x2B0 and +0x2A0 = -1). Every process left in the active list (+0x2B8) is then cleared and moved to the
 * idle list (+0x2B4), and the queue at +0x1C4 is emptied with tMissionMenuQueue_empty.
 */
extern "C" void cWScriptMan_failActiveLocalMission(cWScriptMan* self)
{
    unsigned int id = self->LocalMissionId;
    if (id != 0xFFFFFFFF) {
        cProcessList_findMission_cObj* obj = cProcessList_findMission((char*)&self->ActiveList, id);
        if (obj != 0) {
            cWScriptProcess_end(obj);
        }
        self->LocalMissionCmd = 0;
        self->LocalMissionCmdId = 0xFFFFFFFF;
        self->LocalMissionId = 0xFFFFFFFF;
    }
    void* process;
    while ((process = cProcessList_removeHead((char*)&self->ActiveList)) != 0) {
        *(int*)process = 0;
        cProcessList_addProcess((char*)&self->IdleList, process);
    }
    tMissionMenuQueue_empty(&self->MissionMenuQueue);
}
#endif

extern "C" void* cProcessList_findProcess1(void*, int);

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isProcessActive__FPvT0);
#ifdef SKIP_ASM
/**
 * Tells whether a world script process is in the active list.
 *
 * @param process  the process (On Tour process)
 * @return 1 when a process with @p process's id (+0x20) is in the active list at +0x2B8, else 0
 */
int cWScriptMan_isProcessActive(void* vself, void* process)
{
    cWScriptMan* self = (cWScriptMan*)vself;
    return (cProcessList_findProcess1(&self->ActiveList, *(int*)((char*)process + 0x20)) != 0);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isLocalMissionActive);
#ifdef SKIP_ASM
class cWScriptMan_isLocalMissionActive_cNode {
public:
    char pad_0x00[0x18];
    cWScriptMan_isLocalMissionActive_cNode* next;   // 0x18
    char pad_0x1C[0x5C - 0x1C];
    // vptr at 0x5C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int isMission();
};

/**
 * Tells whether a local mission is running.
 *
 * @return 1 when some process in the active list (+0x2B8) is a mission (its virtual slot 3) in state 2 (+0x60), else 0
 */
extern "C" int cWScriptMan_isLocalMissionActive(cWScriptMan* self)
{
    int n = 0;
    cWScriptMan_isLocalMissionActive_cNode* node;
    for (node = *(cWScriptMan_isLocalMissionActive_cNode**)&self->ActiveList; node != 0; node = node->next) {
        if (node->isMission() != 0 && *(int*)((char*)node + 0x60) == 2) {
            n++;
        }
    }
    return n != 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_collectPool);
#ifdef SKIP_ASM
struct cWScriptIdTable;
extern "C" void cCollectPool_init(cWScriptIdTable* self);

// same layout as cWScriptIdTable (defined later in the unit)
struct cWScriptMan_collectPool_sSlot {
    unsigned int id;     // 0x0
    int arg;             // 0x4
    int b;               // 0x8
    unsigned int entries[64];
};

/**
 * Sets up collect pool @p id: when the manager's only pool (+0x2C0) is unused (ID -1) it is initialised
 * (cCollectPool_init) and given the ID and @p size; a pool that already has the ID, or another one, is left alone.
 *
 * @param id  the pool ID
 * @param size  the pool size
 */
extern "C" void cWScriptMan_collectPool(cWScriptMan* self, unsigned int id, int size)
{
    cWScriptMan_collectPool_sSlot* pool = (cWScriptMan_collectPool_sSlot*)(self->CollectPools[0]);
    int i;
    for (i = 0; i < 1; i++, pool++) {
        if (pool->id == id) {
            return;
        }
        if (pool->id == 0xFFFFFFFF) {
            cCollectPool_init((cWScriptIdTable*)pool);
            pool->id = id;
            pool->arg = size;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_collect);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cCollectPool_collect_sTable;
extern "C" void cCollectPool_collect(cCollectPool_collect_sTable* self, int id);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);
extern "C" int cBENewRaceInterface_getLocationPeak1(void* iface);
extern "C" int cBEEconInterface_getCollectableCash(void* self, int level);
extern "C" void cRider_flagEventAddCollectible(void* rider, int level, int a2);

struct sWSSlot_B9A0 {
    int id;
    char pad[0x10C - 4];
};

/**
 * Records that a rider picked up a collectible of the manager's pool.
 *
 * When the pool id matches, the instance is marked collected, the pickup sound plays and the rider gets an
 * add-collectible event with the current peak (cBENewRaceInterface_getLocationPeak1) and its value (cBEEconInterface_getCollectableCash).
 *
 * @param rider   rider index (On Tour being)
 * @param id      collectible pool id (On Tour poolId)
 * @param instId  the collected instance's resource id
 */
extern "C" void cWScriptMan_collect(cWScriptMan* self, int rider, int id, int instId)
{
    sWSSlot_B9A0* slots = (sWSSlot_B9A0*)(self->CollectPools[0]);
    int i;
    for (i = 0; i < 1; i++) {
        if (slots[i].id == id) {
            cCollectPool_collect((cCollectPool_collect_sTable*)&slots[i], instId);
            cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 5, 0, 1.0f);
            void* newRace = cBE_getInterface_Fv(cBE_getBE(), 0);
            void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
            int lvl = cBENewRaceInterface_getLocationPeak1(newRace);
            int cash = cBEEconInterface_getCollectableCash(econ, lvl);
            cRider_flagEventAddCollectible(SSXApp->Game->AI->Riders[rider], lvl, cash);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_collectCount);
#ifdef SKIP_ASM
struct cWScriptMan_collectCount_sEntry {
    int id;
    char pad04[0x10C - 4];
};

struct cWScriptMan_collectCount_sMan {
    char pad000[0x2C0];
    cWScriptMan_collectCount_sEntry entries[1];
};

extern "C" int cCollectPool_collectCount(void* entry);

/**
 * Returns the collected count of collect pool @p id (cCollectPool_collectCount), or 0 when the manager's pool (+0x2C0) has another
 * ID.
 *
 * @param id  the pool ID
 * @return the collected count
 */
extern "C" int cWScriptMan_collectCount(cWScriptMan* self, int id)
{
    cWScriptMan_collectCount_sEntry* pool = ((cWScriptMan_collectCount_sEntry*)&self->CollectPools[0]);
    int i;
    for (i = 0; i < 1; i++, pool++) {
        if (pool->id == id) {
            return cCollectPool_collectCount(pool);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_collectInst);
#ifdef SKIP_ASM
struct cWScriptMan_collectInst_sEntry {
    int id;
    char pad04[0x10C - 4];
};

struct cWScriptMan_collectInst_sMan {
    char pad000[0x2C0];
    cWScriptMan_collectInst_sEntry entries[1];
};

extern "C" void cCollectPool_collectInst(void* e, int a1, int a2);

/**
 * Adds instances to collect pool @p id (cCollectPool_collectInst) when the manager's pool (+0x2C0) has that ID.
 *
 * @param id  the pool ID
 * @param instIds  the instance resource IDs (unsigned int array)
 * @param size  the number of IDs
 */
extern "C" void cWScriptMan_collectInst(cWScriptMan* self, int id, int instIds, int size)
{
    cWScriptMan_collectInst_sEntry* pool = ((cWScriptMan_collectInst_sEntry*)&self->CollectPools[0]);
    int i;
    for (i = 0; i < 1; i++, pool++) {
        if (pool->id == id) {
            cCollectPool_collectInst(pool, instIds, size);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_writeToReplayFrame);
#ifdef SKIP_ASM
struct sWSVtVBB10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sWSManBB10 {
    char pad0[0x28C];
    char* world;                // 0x28C
    char pad290[0x28];
    void* list2B8;              // 0x2B8
    void* list2BC;              // 0x2BC
    char tables[1][0x10C];      // 0x2C0
    void* objs[50];             // 0x3CC
};

class cCollectPool_writeToReplayFrame_cObj;
extern "C" int cCollectPool_writeToReplayFrame(void* a, cCollectPool_writeToReplayFrame_cObj* obj);
extern "C" void cWScriptMan_writeReplayProcessList(cWScriptMan* self, void** list, cReplayFramePtr* stream);
extern "C" int cWScriptCache_getScriptBase(void* a, int id);
extern "C" void luno_cLunoTable_writeToReplayFrame(void* obj, void* stream, int a2);

/**
 * Saves the world script manager into a replay frame.
 *
 * Writes the first 0x28C bytes of the manager (the stream's virtual slot 1), the collect pool (cCollectPool_writeToReplayFrame), the
 * active and start process lists (cWScriptMan_writeReplayProcessList), the number of section global tables in use (+0x3CC, 50 slots) and for
 * each its index and its contents (luno_cLunoTable_writeToReplayFrame, relative to the section's script base from
 * cWScriptCache_getScriptBase).
 *
 * @param stream  the replay frame stream
 */
extern "C" void cWScriptMan_writeToReplayFrame(sWSManBB10* self, cReplayFramePtr* stream)
{
    int count;
    int j;
    {
        sWSVtVBB10* vtEntry = &(*(sWSVtVBB10**)stream)[1];
        vtEntry->fn((char*)stream + vtEntry->delta, self, 652);
    }
    {
        int k;
        for (k = 0; k < 1; k++) {
            cCollectPool_writeToReplayFrame(self->tables[k], (cCollectPool_writeToReplayFrame_cObj*)stream);
        }
    }
    cWScriptMan_writeReplayProcessList((cWScriptMan*)self, &self->list2B8, stream);
    cWScriptMan_writeReplayProcessList((cWScriptMan*)self, &self->list2BC, stream);
    count = 0;
    {
        int i;
        for (i = 0; i < 50; i++) {
            if (self->objs[i] != 0) {
                count++;
            }
        }
    }
    {
        sWSVtVBB10* vtEntry = &(*(sWSVtVBB10**)stream)[1];
        vtEntry->fn((char*)stream + vtEntry->delta, &count, 4);
    }
    for (j = 0; j < 50; j++) {
        if (self->objs[j] != 0) {
            sWSVtVBB10* vtEntry = &(*(sWSVtVBB10**)stream)[1];
            int scriptBase;
            vtEntry->fn((char*)stream + vtEntry->delta, &j, 4);
            scriptBase = cWScriptCache_getScriptBase(*(void**)(self->world + 4), j);
            luno_cLunoTable_writeToReplayFrame(self->objs[j], stream, scriptBase);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_writeReplayProcessList);
#ifdef SKIP_ASM
extern "C" int cProcessList_getCount(cWScriptListHead* self);
extern "C" void cWScriptProcess_writeToReplayFrame(void* node, void* stream);

struct sVEntry30BC80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Saves a process list into a replay frame: its count (cProcessList_getCount), then per process its ID (+0x20) and its
 * state (cWScriptProcess_writeToReplayFrame).
 *
 * @param list  the process list
 * @param stream  the replay frame stream
 */
extern "C" void cWScriptMan_writeReplayProcessList(cWScriptMan* self, void** list, cReplayFramePtr* stream)
{
    int n = cProcessList_getCount((cWScriptListHead*)list);
    sVEntry30BC80* vtEntry = &(*(sVEntry30BC80**)stream)[1];
    vtEntry->fn((char*)stream + vtEntry->delta, &n, 4);
    char* node = (char*)*list;
    while (node != 0) {
        int id = *(int*)(node + 0x20);
        vtEntry = &(*(sVEntry30BC80**)stream)[1];
        vtEntry->fn((char*)stream + vtEntry->delta, &id, 4);
        cWScriptProcess_writeToReplayFrame(node, stream);
        node = *(char**)(node + 0x18);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_readFromReplayFrame);
#ifdef SKIP_ASM
class cStream_30BD20 {
public:
    virtual void v01();
    virtual void Read(void* dst, int size);
};

struct sWSMan_30BD20 {
    char pad0[0x28C];
    char* world;                // 0x28C
    char pad290[0x28];
    void* list2B8;              // 0x2B8
    void* list2BC;              // 0x2BC
    char tables[1][0x10C];      // 0x2C0
    void* objs[50];             // 0x3CC
};

struct cWScriptListHead;
extern "C" void* cProcessList_removeHead(void* self);
int cProcessList_addProcess(void* self, void* a1);
extern "C" int cWScriptCache_getScriptBase(void* a, int id);
class cCollectPool_readFromReplayFrame_cObj;
extern "C" int cCollectPool_readFromReplayFrame(void* a, cCollectPool_readFromReplayFrame_cObj* obj);
class cWScriptMan_readReplayProcessList_cStream;
extern "C" void cWScriptMan_readReplayProcessList(cWScriptMan* self, cWScriptListHead* list, cWScriptMan_readReplayProcessList_cStream* stream);
extern "C" void luno_cLunoTable_emptyTable(void* obj);
extern "C" void luno_cLunoTable_readFromReplayFrame(void* obj, cStream_30BD20* stream, int a2);

/**
 * Restores the world script manager's state from a replay frame.
 *
 * Reads the first 0x28C bytes of the manager, then the collect pool (cCollectPool_readFromReplayFrame). Every process in the active
 * (+0x2B8) and pending (+0x2BC) lists goes back to the idle list, and both lists are then read back from @p stream
 * (cWScriptMan_readReplayProcessList). The 50 section tables at +0x3CC are emptied (luno_cLunoTable_emptyTable), and a count of saved tables follows,
 * each read back against its section's script base (cWScriptCache_getScriptBase). Finally the current instance, infos, process and
 * task are cleared.
 *
 * @param stream  the replay frame to read from (On Tour data)
 */
extern "C" void cWScriptMan_readFromReplayFrame(sWSMan_30BD20* self, cStream_30BD20* stream)
{
    stream->Read(self, 652);
    {
        int k;
        for (k = 0; k < 1; k++) {
            cCollectPool_readFromReplayFrame(self->tables[k], (cCollectPool_readFromReplayFrame_cObj*)stream);
        }
    }
    void* process;
    while ((process = cProcessList_removeHead(&self->list2B8)) != 0) {
        *(int*)process = 0;
        cProcessList_addProcess((char*)self + 0x2B4, process);
    }
    while ((process = cProcessList_removeHead(&self->list2BC)) != 0) {
        *(int*)process = 0;
        cProcessList_addProcess((char*)self + 0x2B4, process);
    }
    cWScriptMan_readReplayProcessList((cWScriptMan*)self, (cWScriptListHead*)&self->list2B8, (cWScriptMan_readReplayProcessList_cStream*)stream);
    cWScriptMan_readReplayProcessList((cWScriptMan*)self, (cWScriptListHead*)&self->list2BC, (cWScriptMan_readReplayProcessList_cStream*)stream);
    {
        int i;
        for (i = 0; i < 50; i++) {
            if (self->objs[i] != 0) {
                luno_cLunoTable_emptyTable(self->objs[i]);
            }
        }
    }
    int count;
    stream->Read(&count, 4);
    for (int j = 0; j < count; j++) {
        int id;
        stream->Read(&id, 4);
        int scriptBase = cWScriptCache_getScriptBase(*(void**)(self->world + 4), id);
        luno_cLunoTable_readFromReplayFrame(self->objs[id], stream, scriptBase);
    }
    *(int*)((char*)self + 0x290) = 0;
    *(int*)((char*)self + 0x294) = 0;
    *(int*)((char*)self + 0x298) = 0;
    *(int*)((char*)self + 0x2A4) = 0;
    *(int*)((char*)self + 0x2A8) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_readReplayProcessList);
#ifdef SKIP_ASM
class cWScriptMan_readReplayProcessList_cStream {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void Read(void* dst, int size);
};

extern "C" void cWScriptProcess_readFromReplayFrame(void* proc, cWScriptMan_readReplayProcessList_cStream* stream);
extern "C" void cProcessList_appendProcess(cWScriptListHead* self, cWScriptListNode* node);
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node);

/**
 * Restores a process list from a replay frame: reads a count, then per entry a process ID; that process is taken out of
 * the idle list (+0x2B4; cProcessList_findProcess1, cProcessList_removeProcess), its state read (cWScriptProcess_readFromReplayFrame) and appended to
 * @p list.
 *
 * Likely bug: an ID missing from the idle list gives a null process, which is used unchecked.
 *
 * @param list  the process list to fill
 * @param stream  the replay frame stream
 */
extern "C" void cWScriptMan_readReplayProcessList(cWScriptMan* self, cWScriptListHead* list, cWScriptMan_readReplayProcessList_cStream* stream)
{
    int n;
    int id;
    int i = 0;
    stream->Read(&n, 4);
    for (; i < n; i++) {
        stream->Read(&id, 4);
        cWScriptListNode* node = (cWScriptListNode*)cProcessList_findProcess1((char*)&self->IdleList, id);
        cProcessList_removeProcess((cWScriptListHead*)&self->IdleList, node);
        cWScriptProcess_readFromReplayFrame(node, stream);
        cProcessList_appendProcess(list, node);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_checkGate);
#ifdef SKIP_ASM

struct sRefSet_BFC0 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};
struct sRefWorld_BFC0 {
    char pad_0x0[0x8];
    sRefSet_BFC0** sets;    // 0x8
};

static inline void* refToPtr_BFC0(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* toObj_BFC0(unsigned int p)
{
    if (p == 0)
        return 0;
    return refToPtr_BFC0(p);
}

static inline void* lookup_BFC0(unsigned int id)
{
    sRefSet_BFC0* set = ((sRefWorld_BFC0*)cWorld_m_pTheWorld->m_pWorldCache)->sets[id & 0xFF];
    if (set == 0) {
        return 0;
    }
    return toObj_BFC0(set->refs[id >> 8] >> 8);
}

struct sVec4_BFC0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Length_BFC0(const sVec4_BFC0& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_BFC0 vu0Sub_BFC0(const sVec4_BFC0& a, const sVec4_BFC0& b)
{
    sVec4_BFC0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

static inline sVec4_BFC0 pos_BFC0(void* p)
{
    return *(sVec4_BFC0*)((char*)p + 0x40);
}

struct sAi_BFC0 {
    char pad0[0x28];
    char* riders[20];
};

struct sVE_BFC0 {
    short delta;
    short index;
    int (*fn)(void*, void*);
};

struct sVEp_BFC0 {
    short delta;
    short index;
    sVec4_BFC0* (*fn)(void*);
};

/**
 * Checks the current rider against a gate: returns 1 when it went through, 0 while it is near, -1 when it missed.
 *
 * Resolves the three instance IDs (-1 for none) through the world cache (cWorld_m_pTheWorld). The rider is the AI rider
 * at index @p who[0]; its +0x6C0 part's virtual slot 3 says whether it passed the gate instance @p id1 (1). Otherwise
 * the rider's position (slot 5) is compared with the instance positions (+0x40): its distance to the previous gate must
 * not exceed 1.1 times the previous-to-gate distance (without one, its distance to the gate 1.5 times the gate-to-next
 * distance), and its distance to the gate must not exceed 1.1 times the gate-to-next distance (without a next, 1.5
 * times the previous-to-gate distance). Within either limit gives 0, else -1.
 *
 * @param who  the world script manager (its first word is the current being)
 * @param id0  the previous gate's instance ID, or -1
 * @param id1  the gate's instance ID
 * @param id2  the next gate's instance ID, or -1
 * @return 1 passed, 0 still near, -1 missed
 */
extern "C" int cWScriptMan_checkGate(int* who, unsigned int id0, unsigned int id1, unsigned int id2)
{
    void* prevInst = 0;
    void* inst = 0;
    void* nextInst = 0;
    if (~id0 != 0) {
        void* found = lookup_BFC0(id0);
        prevInst = found;
    }
    if (~id1 != 0) {
        void* found = lookup_BFC0(id1);
        inst = found;
    }
    if (~id2 != 0) {
        void* found = lookup_BFC0(id2);
        nextInst = found;
    }
    char* rider = ((sAi_BFC0*)(char*)SSXApp->Game->AI)->riders[*who];
    char* obj = rider + 0x6C0;
    sVE_BFC0* vt = *(sVE_BFC0**)obj;
    if (vt[3].fn(obj + vt[3].delta, inst) != 0) {
        return 1;
    }
    sVEp_BFC0* vtp = *(sVEp_BFC0**)obj;
    sVec4_BFC0* pos = vtp[5].fn(obj + vtp[5].delta);
    float dist;
    float seg;
    if (prevInst != 0) {
        dist = vu0Length_BFC0(vu0Sub_BFC0(*pos, pos_BFC0(prevInst)));
        seg = vu0Length_BFC0(vu0Sub_BFC0(pos_BFC0(inst), pos_BFC0(prevInst))) * 1.1f;
    } else {
        dist = vu0Length_BFC0(vu0Sub_BFC0(*pos, pos_BFC0(inst)));
        seg = vu0Length_BFC0(vu0Sub_BFC0(pos_BFC0(nextInst), pos_BFC0(inst))) * 1.5f;
    }
    if (dist <= seg) {
        return 0;
    }
    if (nextInst != 0) {
        dist = vu0Length_BFC0(vu0Sub_BFC0(*pos, pos_BFC0(inst)));
        seg = vu0Length_BFC0(vu0Sub_BFC0(pos_BFC0(nextInst), pos_BFC0(inst))) * 1.1f;
    } else {
        dist = vu0Length_BFC0(vu0Sub_BFC0(*pos, pos_BFC0(inst)));
        seg = vu0Length_BFC0(vu0Sub_BFC0(pos_BFC0(inst), pos_BFC0(prevInst))) * 1.5f;
    }
    if (dist <= seg) {
        return 0;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cCollectPool_init);
#ifdef SKIP_ASM
struct cWScriptIdTable {
    unsigned int id;
    int a;
    int b;
    unsigned int entries[64];
};

/**
 * Empties a collect pool.
 *
 * Sets the pool id (+0x0) to -1, the words at +0x4 and +0x8 to 0 and all 64 instance ids from +0xC to -1.
 */
extern "C" void cCollectPool_init(cWScriptIdTable* self)
{
    int i;
    self->id = 0xFFFFFFFF;
    self->a = 0;
    self->b = 0;
    for (i = 0; i < 64; i++) {
        self->entries[i] = 0xFFFFFFFF;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cCollectPool_collect);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBEMissionInterface_setBit(void* iface, int a1, int index, int value);

struct cCollectPool_collect_sTable {
    int value;      // 0x0
    int pad4;       // 0x4
    int count;      // 0x8
    int ids[1];     // 0xC
};

/**
 * Marks an instance of a collect pool as collected: for the pool entry i (count +0x8, IDs from +0xC) equal to @p id,
 * sets mission bit i of the pool's ID (+0x0) in the BE mission interface (cBEMissionInterface_setBit on
 * cBE_getInterface(10)).
 *
 * @param id  the instance's resource ID
 */
extern "C" void cCollectPool_collect(cCollectPool_collect_sTable* self, int id)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->ids[i] == id) {
            cBEMissionInterface_setBit(cBE_getInterface_Fv(cBE_getBE(), 10), 0, i, self->value);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cCollectPool_collectCount);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cBEMissionInterface_getCurrentCollectByType(void* iface, int a1, int id);

/**
 * Returns how many instances of a collect pool are collected: cBEMissionInterface_getCurrentCollectByType for the
 * pool's ID (+0x0) on cBE_getInterface(10).
 *
 * @param entry  the collect pool
 * @return the collected count
 */
extern "C" int cCollectPool_collectCount(void* entry)
{
    return cBEMissionInterface_getCurrentCollectByType(cBE_getInterface_Fv(cBE_getBE(), 10), 0, *(int*)entry);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cCollectPool_collectInst);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_C4A8(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" int cBEMissionInterface_isBitSet(void* iface, int a, int b, int c);
extern "C" void* cDeadNode_cDeadNode(void* mem, void* obj);
extern char str_DeadNode[];

struct sRefSet_C4A8 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};
struct sRefWorld_C4A8 {
    char pad_0x0[0x8];
    sRefSet_C4A8** sets;    // 0x8
};


struct sVE_C4A8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sEntry_C4A8 {
    int id;
    int f4;
    int count;
    unsigned int ids[1];
};

static inline void* refToPtr_C4A8(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* toObj_C4A8(unsigned int p)
{
    if (p == 0)
        return 0;
    return refToPtr_C4A8(p);
}

static inline void* lookup_C4A8(unsigned int id)
{
    sRefSet_C4A8* set = ((sRefWorld_C4A8*)cWorld_m_pTheWorld->m_pWorldCache)->sets[id & 0xFF];
    if (set == 0) {
        return 0;
    }
    return toObj_C4A8(set->refs[id >> 8] >> 8);
}

static inline void respawn_C4A8(unsigned int id)
{
    char* obj = (char*)lookup_C4A8(id);
    char* o = *(char**)(obj + 0xC);
    if (o != 0) {
        sVE_C4A8* vt = *(sVE_C4A8**)(o + 0xC);
        vt[1].fn(o + vt[1].delta, 3);
    }
    cDeadNode_cDeadNode(cMemMan_alloc(0x1C, str_DeadNode, 0x20000000, 0), obj);
}

/**
 * Adds instances to a collect pool and takes the already collected ones out of the world.
 *
 * Each of the @p size IDs (-1 skipped) is stored in the pool's next entry (+0xC + 4 * count, count +0x8). When the race
 * has a game path (cGamePath of cBENewRaceInterface_newRace) or the mission bit for that entry and the pool's ID (+0x0)
 * is set (cBEMissionInterface_isBitSet), the instance is removed: its script controller (+0xC), if any, is destroyed
 * (virtual slot 1, flag 3) and a cDeadNode ("DeadNode", 0x1C bytes) is created for it.
 *
 * Likely bug: the count is not checked against the pool's 64 entries.
 *
 * @param ep  the collect pool
 * @param instIds  the instance resource IDs (unsigned int array)
 * @param size  the number of IDs
 */
// PORT: a1 is really an unsigned int* id list and a2 its count (the unit declares both int).
extern "C" void cCollectPool_collectInst(void* ep, int instIds, int size)
{
    sEntry_C4A8* pool = (sEntry_C4A8*)ep;
    unsigned int* ids = (unsigned int*)instIds;
    void* iface = cBE_getInterface_C4A8(cBE_getBE(), 10);
    if (size <= 0)
        return;
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    for (int n = size; n != 0; n--, ids++) {
        unsigned int id = *ids;
        if (~id == 0)
            continue;
        ((unsigned int*)((char*)pool + 0xC))[pool->count] = id;
        cBE_getInterface_C4A8(cBE_getBE(), 0);
        if (beRaceDB->cGamePath == 0) {
            if (cBEMissionInterface_isBitSet(iface, 0, pool->count, pool->id))
                respawn_C4A8(*ids);
        } else {
            respawn_C4A8(*ids);
        }
        pool->count++;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cCollectPool_writeToReplayFrame);
#ifdef SKIP_ASM
class cCollectPool_writeToReplayFrame_cObj {
public:
    // slot N at vtable offset N*8
    virtual int write(void* a, int b);
};

/**
 * Writes collect pool @p collectPool into a replay stream: its 0x10C bytes through the stream's virtual slot 1.
 *
 * @param collectPool  the collect pool
 * @param obj  the replay stream
 * @return the stream call's result
 */
extern "C" int cCollectPool_writeToReplayFrame(void* collectPool, cCollectPool_writeToReplayFrame_cObj* obj)
{
    return obj->write(collectPool, 268);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cCollectPool_readFromReplayFrame);
#ifdef SKIP_ASM
class cCollectPool_readFromReplayFrame_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
    virtual int read(void* a, int b);
};

/**
 * Reads collect pool @p collectPool from a replay stream: its 0x10C bytes through the stream's virtual slot 2.
 *
 * @param collectPool  the collect pool
 * @param obj  the replay stream
 * @return the stream call's result
 */
extern "C" int cCollectPool_readFromReplayFrame(void* collectPool, cCollectPool_readFromReplayFrame_cObj* obj)
{
    return obj->read(collectPool, 268);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", tMissionMenuQueue_tMissionMenuQueue);
#ifdef SKIP_ASM
extern "C" void tMissionMenuQueue_init(tMissionMenuQueue* self);

/**
 * Constructs an empty mission menu queue.
 *
 * Runs tMissionMenuQueue_init.
 *
 * @return this
 */
extern "C" void* tMissionMenuQueue_tMissionMenuQueue(tMissionMenuQueue* self)
{
    tMissionMenuQueue_init(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", tMissionMenuQueue_init);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

/**
 * Empties a mission menu queue.
 *
 * Sets the read (+0x0) and write (+0x4) positions to 0 and fills the 16 twelve-byte entries from +0x8 with zero.
 */
extern "C" void tMissionMenuQueue_init(tMissionMenuQueue* self)
{
    *(int*)self = 0;
    self->Next = 0;
    MEM_fill(&self->Queue[0], 0, 0xC0);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", tMissionMenuQueue_empty);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

/**
 * Empties a mission menu queue: the read (+0x0) and write (+0x4) positions become 0 and the 16 entries (+0x8, 0xC0
 * bytes) are zeroed. Same body as tMissionMenuQueue_init.
 */
extern "C" void tMissionMenuQueue_empty(tMissionMenuQueue* self)
{
    *(int*)self = 0;
    self->Next = 0;
    MEM_fill(&self->Queue[0], 0, 0xC0);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", tMissionMenuQueue_addEntry);
#ifdef SKIP_ASM
// Same layout as cWScriptQueue / cWScriptQueueEntry, which the unit defines
// further down (above tMissionMenuQueue_getNextEntry). Renamed here only to avoid a redefinition.
struct cWSQEntry {
    int a;
    int b;
    int c;
};

struct cWSQueue {
    int head;
    int tail;
    cWSQEntry entries[16];
};

/**
 * Queues a mission command for the menus.
 *
 * Stores the three values in the entry at the write position (+0x4) and moves the write position on, wrapping at 16.
 * There is no full-queue check in this build.
 *
 * @param missionId  the mission (On Tour missionId)
 * @param cmd  the mission command (On Tour cmd)
 * @param being  the rider it applies to (On Tour being)
 */
extern "C" void tMissionMenuQueue_addEntry(tMissionMenuQueue* self, int missionId, int cmd, int being)
{
    ((cWSQEntry*)&self->Queue[0])[self->Next].a = missionId;
    ((cWSQEntry*)&self->Queue[0])[self->Next].b = cmd;
    ((cWSQEntry*)&self->Queue[0])[self->Next].c = being;
    self->Next = (self->Next + 1) % 16;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", tMissionMenuQueue_getNextEntry);
#ifdef SKIP_ASM
struct cWScriptQueueEntry {
    int a;
    int b;
    int c;
};

struct cWScriptQueue {
    int head;
    int tail;
    cWScriptQueueEntry entries[16];
};

/**
 * Takes the next mission command off the menu queue.
 *
 * Returns 0 when the read position (+0x0, On Tour Current) has caught up with the write position (+0x4, On Tour Next);
 * otherwise returns the entry at the read position and advances it, wrapping at 16.
 *
 * @return the entry (mission id, command, rider; On Tour tMissionMenuQueueEntry), or 0 when the queue is empty
 */
extern "C" cWScriptQueueEntry* tMissionMenuQueue_getNextEntry(tMissionMenuQueue* self)
{
    int head = self->Current;
    if (head == self->Next) {
        return 0;
    }
    self->Current = (head + 1) % 16;
    return &((cWScriptQueueEntry*)&self->Queue[0])[head];
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", tMissionMenuQueue_getEntry);
#ifdef SKIP_ASM
/**
 * Returns the oldest queued mission menu entry without removing it.
 *
 * @return the entry at the read position, or 0 when the queue is empty
 */
extern "C" cWScriptQueueEntry* tMissionMenuQueue_getEntry(tMissionMenuQueue* self)
{
    if (self->Current == self->Next) {
        return 0;
    }
    return &((cWScriptQueueEntry*)&self->Queue[0])[self->Current];
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C898);
#ifdef SKIP_ASM
/**
 * Returns its first argument and does nothing else; the variable arguments are only spilled to the stack.
 *
 * It has no callers in the build (a variadic stub, probably a compiled-out print).
 *
 * @return the first argument
 */
extern "C" void* func_0030C898(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C8D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColour_C8D8 {
    float a, b, c, d;
};

struct sKey_C8D8 {
    unsigned int idx : 8;
    unsigned int id : 24;
    int pad;
};

struct sPair_C8D8 {
    int a;
    int b;
};

extern sColour_C8D8 D_004FC080;
extern sColour_C8D8 D_004FC090;
extern sColour_C8D8 D_004FC0A0;
extern sColour_C8D8 D_004FC0B0;
extern sColour_C8D8 D_004FC0C0;
extern sColour_C8D8 D_004FC0D0;
extern sColour_C8D8 D_004FC0E0;
extern sColour_C8D8 D_004FC0F0;
extern sColour_C8D8 D_004FC100;
extern sColour_C8D8 D_004FC110;
extern sColour_C8D8 D_004FC120;
extern sColour_C8D8 D_004FC130;
extern sColour_C8D8 D_004FC140;
extern sColour_C8D8 D_004FC150;
extern sColour_C8D8 D_004FC160;
extern sColour_C8D8 D_004FC170;
extern sColour_C8D8 D_004FC180;
extern sColour_C8D8 D_004FC190;
extern sColour_C8D8 D_004FC1A0;
extern sColour_C8D8 D_004FC1B0;
extern sColour_C8D8 D_004FC1C0;
extern sColour_C8D8 D_004FC1D0;
extern sColour_C8D8 D_004FC1E0;
extern sColour_C8D8 D_004FC1F0;
extern sColour_C8D8 D_004FC200;
extern sColour_C8D8 D_004FC210;
extern sColour_C8D8 D_004FC220;
extern sColour_C8D8 D_004FC230;
extern sColour_C8D8 D_004FC240;
extern sColour_C8D8 D_004FC250;
extern sColour_C8D8 D_004FC260;
extern sColour_C8D8 D_004FC270;
extern sColour_C8D8 D_004FC280;
extern sColour_C8D8 D_004FC290;
extern sColour_C8D8 D_004FC2A0;
extern sColour_C8D8 D_004FC2B0;
extern sColour_C8D8 D_004FC2C0;
extern sColour_C8D8 D_004FC2D0;
extern sColour_C8D8 D_004FC2E0;
extern sColour_C8D8 D_004FC2F0;
extern sColour_C8D8 D_004FC300;
extern sColour_C8D8 D_004FC310;
extern sColour_C8D8 D_004FC320;
extern sColour_C8D8 D_004FC330;
extern sColour_C8D8 D_004FC340;
extern sColour_C8D8 D_004FC350;
extern sColour_C8D8 D_004FC360;
extern sColour_C8D8 D_004FC370;
extern sColour_C8D8 D_004FC380;
extern sColour_C8D8 D_004FC390;
extern sColour_C8D8 D_004FC3A0;
extern sColour_C8D8 D_004FC3B0;
extern sColour_C8D8 D_004FC3C0;
extern sColour_C8D8 D_004FC3D0;
extern sColour_C8D8 D_004FC3E0;
extern sColour_C8D8 D_004FC3F0;
extern sColour_C8D8 D_004FC400;
extern sColour_C8D8 D_004FC410;
extern sPair_C8D8 D_004A58D0;
extern sKey_C8D8 D_004A58C8;
extern sKey_C8D8 D_004A58D8;
extern sKey_C8D8 D_004A58E0;
extern sKey_C8D8 D_004A58E8;
extern sKey_C8D8 D_004A58F0;
extern sKey_C8D8 D_004A58F8;
extern sKey_C8D8 D_004A5900;
extern sKey_C8D8 D_004A5908;
extern sKey_C8D8 D_004A5910;
extern sKey_C8D8 D_004A5918;
extern sKey_C8D8 D_004A5920;
extern sKey_C8D8 D_004A5928;
extern sKey_C8D8 D_004A5930;
extern sKey_C8D8 D_004A5938;
extern sKey_C8D8 D_004A5940;

static inline void Key_C8D8(sKey_C8D8* p, int i)
{
    p->idx = i;
    p->id = i;
}

static inline void Pair_C8D8(sPair_C8D8* p)
{
    p->b = 0;
    p->a = 0;
}

extern "C" void func_0030C8D8_impl(int init, int prio) __asm__("func_0030C8D8");

/**
 * Builds wscriptman.cpp's static constants (this unit's g++ static initialisation routine).
 *
 * Acts only when @p init is nonzero and @p prio is 0xFFFF; there is no destruction branch. Fills the 56 four-float
 * constants D_004FC080..D_004FC3F0 (the same values every unit's static initialiser starts with, as in path/pathsys),
 * gives the key D_004A58C8 -1 in both its 8-bit index and 24-bit id fields, zeroes the two-word D_004A58D0, numbers
 * the 14 keys D_004A58D8..D_004A5940 from 0 to 13 (index and id alike), then sets the unit's own two constants
 * D_004FC400 = (1, 0.2, 0.2, 0.7) and D_004FC410 = (0.5, 0.9, 0.9, 0.9). func_0030D498 runs it with (1, 0xFFFF).
 *
 * @param init  nonzero to build the constants
 * @param prio  initialisation priority; nothing happens unless it is 0xFFFF (the default)
 */
// Static initialisation of the colour tables and colour keys.
extern "C" void func_0030C8D8_impl(int init, int prio)
{
    if (prio == 0xFFFF && init) {
        D_004FC080.a = 1.0f;
        D_004FC080.b = 1.0f;
        D_004FC080.c = 1.0f;
        D_004FC080.d = 0.5f;
        D_004FC090.a = 0.4998680055141449f;
        D_004FC090.b = 1.0f;
        D_004FC090.c = 1.0f;
        D_004FC090.d = 0.0f;
        D_004FC0A0.a = 0.12951700389385223f;
        D_004FC0A0.b = 1.0f;
        D_004FC0A0.c = 1.0f;
        D_004FC0A0.d = 0.0f;
        D_004FC0B0.a = 0.22426900267601013f;
        D_004FC0B0.b = 0.9402310252189636f;
        D_004FC0B0.c = 0.5425059795379639f;
        D_004FC0B0.d = 0.0004030000127386302f;
        D_004FC0C0.a = 0.4652079939842224f;
        D_004FC0C0.b = 0.32170501351356506f;
        D_004FC0C0.c = 0.9943439960479736f;
        D_004FC0C0.d = 0.0f;
        D_004FC0D0.a = 0.17792600393295288f;
        D_004FC0D0.b = 1.0f;
        D_004FC0D0.c = 1.0f;
        D_004FC0D0.d = 0.0f;
        D_004FC0E0.a = 0.4411740005016327f;
        D_004FC0E0.b = 0.41388601064682007f;
        D_004FC0E0.c = 0.3211260139942169f;
        D_004FC0E0.d = 0.4398239850997925f;
        D_004FC0F0.a = 0.500997006893158f;
        D_004FC0F0.b = 0.24849599599838257f;
        D_004FC0F0.c = 1.0f;
        D_004FC0F0.d = 1.0f;
        D_004FC100.a = 0.27687299251556396f;
        D_004FC100.b = 0.6395620107650757f;
        D_004FC100.c = 0.13423900306224823f;
        D_004FC100.d = 0.0f;
        D_004FC110.a = 0.8936439752578735f;
        D_004FC110.b = 0.5263469815254211f;
        D_004FC110.c = 0.5051450133323669f;
        D_004FC110.d = 0.8687300086021423f;
        D_004FC120.a = 1.0f;
        D_004FC120.b = 1.0f;
        D_004FC120.c = 1.0f;
        D_004FC120.d = 1.0f;
        D_004FC130.a = 1.0f;
        D_004FC130.b = 0.0f;
        D_004FC130.c = 0.0f;
        D_004FC130.d = 0.0f;
        D_004FC140.a = 1.0f;
        D_004FC140.b = 0.550000011920929f;
        D_004FC140.c = 0.5350000262260437f;
        D_004FC140.d = 0.550000011920929f;
        D_004FC150.a = 1.0f;
        D_004FC150.b = 1.0f;
        D_004FC150.c = 1.0f;
        D_004FC150.d = 1.0f;
        D_004FC160.a = 1.0f;
        D_004FC160.b = 1.0f;
        D_004FC160.c = 1.0f;
        D_004FC160.d = 1.0f;
        D_004FC170.a = 1.0f;
        D_004FC170.b = 0.0f;
        D_004FC170.c = 0.0f;
        D_004FC170.d = 0.0f;
        D_004FC180.a = 1.0f;
        D_004FC180.b = 0.0f;
        D_004FC180.c = 0.0f;
        D_004FC180.d = 0.0f;
        D_004FC190.a = 1.0f;
        D_004FC190.b = 1.0f;
        D_004FC190.c = 1.0f;
        D_004FC190.d = 1.0f;
        D_004FC1A0.a = 1.0f;
        D_004FC1A0.b = 0.5f;
        D_004FC1A0.c = 1.0f;
        D_004FC1A0.d = 0.5f;
        D_004FC1B0.a = 1.0f;
        D_004FC1B0.b = 0.5f;
        D_004FC1B0.c = 1.0f;
        D_004FC1B0.d = 0.5f;
        D_004FC1C0.a = 1.0f;
        D_004FC1C0.b = 0.5f;
        D_004FC1C0.c = 1.0f;
        D_004FC1C0.d = 0.5f;
        D_004FC1D0.a = 1.0f;
        D_004FC1D0.b = 0.7828119993209839f;
        D_004FC1D0.c = 0.06171900033950806f;
        D_004FC1D0.d = 0.03984399884939194f;
        D_004FC1E0.a = 1.0f;
        D_004FC1E0.b = 1.0f;
        D_004FC1E0.c = 0.7988259792327881f;
        D_004FC1E0.d = 0.0f;
        D_004FC1F0.a = 0.9886710047721863f;
        D_004FC1F0.b = 0.9230499863624573f;
        D_004FC1F0.c = 0.34311699867248535f;
        D_004FC1F0.d = 0.0f;
        D_004FC200.a = 1.0f;
        D_004FC200.b = 1.0f;
        D_004FC200.c = 1.0f;
        D_004FC200.d = 1.0f;
        D_004FC210.a = 1.0f;
        D_004FC210.b = 1.0f;
        D_004FC210.c = 0.7976430058479309f;
        D_004FC210.d = 0.0f;
        D_004FC220.a = 1.0f;
        D_004FC220.b = 0.9222670197486877f;
        D_004FC220.c = 0.34179699420928955f;
        D_004FC220.d = 0.0f;
        D_004FC230.a = 1.0f;
        D_004FC230.b = 0.7847639918327332f;
        D_004FC230.c = 0.06054700165987015f;
        D_004FC230.d = 0.04062499850988388f;
        D_004FC240.a = 1.0f;
        D_004FC240.b = 0.800000011920929f;
        D_004FC240.c = 0.44999998807907104f;
        D_004FC240.d = 1.0f;
        D_004FC250.a = 1.0f;
        D_004FC250.b = 0.3281260132789612f;
        D_004FC250.c = 1.0f;
        D_004FC250.d = 0.3539060056209564f;
        D_004FC260.a = 1.0f;
        D_004FC260.b = 1.0f;
        D_004FC260.c = 1.0f;
        D_004FC260.d = 1.0f;
        D_004FC270.a = 1.0f;
        D_004FC270.b = 1.0f;
        D_004FC270.c = 1.0f;
        D_004FC270.d = 1.0f;
        D_004FC280.a = 1.0f;
        D_004FC280.b = 0.8613290190696716f;
        D_004FC280.c = 0.38124799728393555f;
        D_004FC280.d = 0.0f;
        D_004FC290.a = 1.0f;
        D_004FC290.b = 1.0f;
        D_004FC290.c = 1.0f;
        D_004FC290.d = 1.0f;
        D_004FC2A0.a = 1.0f;
        D_004FC2A0.b = 0.3244380056858063f;
        D_004FC2A0.c = 1.0f;
        D_004FC2A0.d = 0.0f;
        D_004FC2B0.a = 1.0f;
        D_004FC2B0.b = 1.0f;
        D_004FC2B0.c = 1.0f;
        D_004FC2B0.d = 1.0f;
        D_004FC2C0.a = 1.0f;
        D_004FC2C0.b = 1.0f;
        D_004FC2C0.c = 0.0f;
        D_004FC2C0.d = 0.0f;
        D_004FC2D0.a = 0.7007060050964355f;
        D_004FC2D0.b = 0.0f;
        D_004FC2D0.c = 0.0f;
        D_004FC2D0.d = 0.0f;
        D_004FC2E0.a = 1.0f;
        D_004FC2E0.b = 0.7828530073165894f;
        D_004FC2E0.c = 0.06140600144863129f;
        D_004FC2E0.d = 0.04146299883723259f;
        D_004FC2F0.a = 1.0f;
        D_004FC2F0.b = 0.32487601041793823f;
        D_004FC2F0.c = 1.0f;
        D_004FC2F0.d = 0.35280099511146545f;
        D_004FC300.a = 1.0f;
        D_004FC300.b = 1.0f;
        D_004FC300.c = 0.7977359890937805f;
        D_004FC300.d = 0.0f;
        D_004FC310.a = 1.0f;
        D_004FC310.b = 0.4937489926815033f;
        D_004FC310.c = 1.0f;
        D_004FC310.d = 0.49179700016975403f;
        D_004FC320.a = 1.0f;
        D_004FC320.b = 1.0f;
        D_004FC320.c = 1.0f;
        D_004FC320.d = 1.0f;
        D_004FC330.a = 1.0f;
        D_004FC330.b = 0.800000011920929f;
        D_004FC330.c = 1.0f;
        D_004FC330.d = 0.800000011920929f;
        D_004FC340.a = 1.0f;
        D_004FC340.b = 1.0f;
        D_004FC340.c = 0.0f;
        D_004FC340.d = 0.0f;
        D_004FC350.a = 1.0f;
        D_004FC350.b = 1.0f;
        D_004FC350.c = 0.0f;
        D_004FC350.d = 0.0f;
        D_004FC360.a = 1.0f;
        D_004FC360.b = 1.0f;
        D_004FC360.c = 0.0f;
        D_004FC360.d = 0.0f;
        D_004FC370.a = 1.0f;
        D_004FC370.b = 0.3273420035839081f;
        D_004FC370.c = 1.0f;
        D_004FC370.d = 0.35390499234199524f;
        D_004FC380.a = 1.0f;
        D_004FC380.b = 1.0f;
        D_004FC380.c = 0.061000000685453415f;
        D_004FC380.d = 0.039000000804662704f;
        D_004FC390.a = 1.0f;
        D_004FC390.b = 1.0f;
        D_004FC390.c = 0.7999979853630066f;
        D_004FC390.d = 0.0f;
        D_004FC3A0.a = 1.0f;
        D_004FC3A0.b = 0.9589840173721313f;
        D_004FC3A0.c = 0.6143789887428284f;
        D_004FC3A0.d = 0.0f;
        D_004FC3B0.a = 1.0f;
        D_004FC3B0.b = 0.9285150170326233f;
        D_004FC3B0.c = 0.3399839997291565f;
        D_004FC3B0.d = 0.0f;
        D_004FC3C0.a = 1.0f;
        D_004FC3C0.b = 0.8738250136375427f;
        D_004FC3C0.c = 0.24931700527668f;
        D_004FC3C0.d = 0.017578000202775f;
        D_004FC3D0.a = 1.0f;
        D_004FC3D0.b = 0.7843739986419678f;
        D_004FC3D0.c = 0.06132800132036209f;
        D_004FC3D0.d = 0.03945299983024597f;
        D_004FC3E0.a = 1.0f;
        D_004FC3E0.b = 0.0f;
        D_004FC3E0.c = 1.0f;
        D_004FC3E0.d = 0.0f;
        D_004FC3F0.a = 1.0f;
        D_004FC3F0.b = 1.0f;
        D_004FC3F0.c = 0.0f;
        D_004FC3F0.d = 0.0f;
        Key_C8D8(&D_004A58C8, -1);
        Pair_C8D8(&D_004A58D0);
        Key_C8D8(&D_004A58D8, 0);
        Key_C8D8(&D_004A58E0, 1);
        Key_C8D8(&D_004A58E8, 2);
        Key_C8D8(&D_004A58F0, 3);
        Key_C8D8(&D_004A58F8, 4);
        Key_C8D8(&D_004A5900, 5);
        Key_C8D8(&D_004A5908, 6);
        Key_C8D8(&D_004A5910, 7);
        Key_C8D8(&D_004A5918, 8);
        Key_C8D8(&D_004A5920, 9);
        Key_C8D8(&D_004A5928, 10);
        Key_C8D8(&D_004A5930, 11);
        Key_C8D8(&D_004A5938, 12);
        Key_C8D8(&D_004A5940, 13);
        D_004FC400.a = 1.0f;
        D_004FC400.b = 0.20000000298023224f;
        D_004FC400.c = 0.20000000298023224f;
        D_004FC400.d = 0.699999988079071f;
        D_004FC410.a = 0.5f;
        D_004FC410.b = 0.8999999761581421f;
        D_004FC410.c = 0.8999999761581421f;
        D_004FC410.d = 0.8999999761581421f;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D238);
#ifdef SKIP_ASM
/**
 * Stores @p firstWord in the first word of @p dest and @p secondWord in the second.
 *
 * An out-of-line inline method emitted after this unit's static initialiser; nothing in the build calls it.
 *
 * @param dest  the two-word object to fill
 * @param firstWord  value for the word at +0x0
 * @param secondWord  value for the word at +0x4
 */
extern "C" void func_0030D238(void *dest, int firstWord, int secondWord) {
    (*(int *)((char*)(dest) + (0))) = firstWord;
    (*(int *)((char*)(dest) + (4))) = secondWord;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D248);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged.
 *
 * One of three identical out-of-line copies here (func_0030D248, func_0030D250, func_0030D258), the shape of an
 * empty inline constructor that returns this; nothing in the build calls it.
 *
 * @param value  the value to return
 * @return @p value
 */
extern "C" int func_0030D248(int value) {
    return value;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D250);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged.
 *
 * Identical to func_0030D248 and func_0030D258 (the shape of an empty inline constructor that returns this); nothing in
 * the build calls it.
 *
 * @param value  the value to return
 * @return @p value
 */
extern "C" int func_0030D250(int value) {
    return value;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D258);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged.
 *
 * Identical to func_0030D248 and func_0030D250 (the shape of an empty inline constructor that returns this); nothing in
 * the build calls it.
 *
 * @param value  the value to return
 * @return @p value
 */
extern "C" int func_0030D258(int value) {
    return value;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_setTrickDef);
#ifdef SKIP_ASM
/**
 * Sets the script manager's current trick definition (On Tour TrickDef, +0x29C) to @p trickDef.
 *
 * Out-of-line copy of an inline accessor; cWScriptProcess_dispatchProcessEvent writes the same field directly around a trick event, and
 * cWScriptMan_getCurrentTrick reads the trick's fields through it.
 *
 * @param wscriptMan  the script manager
 * @param trickDef  the trick definition (a pointer passed as an int), or 0 to clear it
 */
extern "C" void cWScriptMan_setTrickDef(cWScriptMan* wscriptMan, tTrickNameDef* trickDef) {
    wscriptMan->TrickDef = trickDef;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isPointsEnabled);
#ifdef SKIP_ASM
/**
 * Returns the script manager's mission-points enable flag (On Tour MissionPointsEnable, +0x4).
 *
 * cWScriptMan_update clears the flag every frame.
 *
 * @param wscriptMan  the script manager
 * @return the flag word, nonzero when the points display is enabled
 */
extern "C" int cWScriptMan_isPointsEnabled(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionPointsEnable);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getNumPointsCurr);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current mission points (On Tour MissionPoints, +0x8).
 *
 * @param wscriptMan  the script manager
 * @return the points
 */
extern "C" int cWScriptMan_getNumPointsCurr(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionPoints);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getNumPointsGoal);
#ifdef SKIP_ASM
/**
 * Returns the script manager's mission points goal (On Tour MissionPointsGoal, +0xC).
 *
 * @param wscriptMan  the script manager
 * @return the points goal
 */
extern "C" int cWScriptMan_getNumPointsGoal(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionPointsGoal);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isCountEnabled);
#ifdef SKIP_ASM
/**
 * Returns the script manager's mission-count enable flag (On Tour MissionCountEnable, +0x10).
 *
 * cWScriptMan_update clears the flag every frame.
 *
 * @param wscriptMan  the script manager
 * @return the flag word, nonzero when the count display is enabled
 */
extern "C" int cWScriptMan_isCountEnabled(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionCountEnable);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getCountCurr);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current mission count (On Tour MissionCount, +0x14).
 *
 * @param wscriptMan  the script manager
 * @return the count
 */
extern "C" int cWScriptMan_getCountCurr(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionCount);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getCountMax);
#ifdef SKIP_ASM
/**
 * Returns the script manager's mission count maximum (On Tour MissionCountMax, +0x18).
 *
 * @param wscriptMan  the script manager
 * @return the maximum count
 */
extern "C" int cWScriptMan_getCountMax(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionCountMax);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isClockEnabled);
#ifdef SKIP_ASM
/**
 * Returns the script manager's mission-clock enable flag (On Tour MissionTicksEnable, +0x1C).
 *
 * cWScriptMan_update clears the flag every frame.
 *
 * @param wscriptMan  the script manager
 * @return the flag word, nonzero when the clock display is enabled
 */
extern "C" int cWScriptMan_isClockEnabled(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionTicksEnable);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getClockTicksCurr);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current mission clock in ticks (On Tour MissionTicks, +0x20).
 *
 * @param wscriptMan  the script manager
 * @return the tick count
 */
extern "C" int cWScriptMan_getClockTicksCurr(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionTicks);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getClockDir);
#ifdef SKIP_ASM
/**
 * Returns the direction of the script manager's mission clock (On Tour MissionTicksDir, +0x24).
 *
 * @param wscriptMan  the script manager
 * @return the clock direction word
 */
extern "C" int cWScriptMan_getClockDir(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionTicksDir);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isHeightEnabled);
#ifdef SKIP_ASM
/**
 * Returns the script manager's mission-height enable flag (On Tour MissionHeightEnable, +0x28).
 *
 * cWScriptMan_update clears the flag every frame.
 *
 * @param wscriptMan  the script manager
 * @return the flag word, nonzero when the height display is enabled
 */
extern "C" int cWScriptMan_isHeightEnabled(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionHeightEnable);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isTrickEnabled);
#ifdef SKIP_ASM
/**
 * Returns the script manager's mission-trick enable flag (On Tour MissionTrickEnable, +0x34).
 *
 * cWScriptMan_update clears the flag every frame.
 *
 * @param wscriptMan  the script manager
 * @return the flag word, nonzero when the trick display is enabled
 */
extern "C" int cWScriptMan_isTrickEnabled(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionTrickEnable);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getTrickCurr);
#ifdef SKIP_ASM
/**
 * Returns the address of the script manager's mission trick record (On Tour MissionTrick, +0x38).
 *
 * @param wscriptMan  the script manager
 * @return @p wscriptMan + 0x38
 */
extern "C" int cWScriptMan_getTrickCurr(int wscriptMan) {
    return wscriptMan + 0x38;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_setLocalMissionId);
#ifdef SKIP_ASM
/**
 * Sets the script manager's local mission id (On Tour LocalMissionId, +0x2A0) to @p missionId.
 *
 * @param wscriptMan  the script manager
 * @param missionId  the mission id to store
 */
extern "C" void cWScriptMan_setLocalMissionId(cWScriptMan* wscriptMan, int missionId) {
    ((int)wscriptMan->LocalMissionId) = missionId;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getLocalMissionId);
#ifdef SKIP_ASM
/**
 * Returns the script manager's local mission id (On Tour LocalMissionId, +0x2A0).
 *
 * @param wscriptMan  the script manager
 * @return the mission id
 */
extern "C" int cWScriptMan_getLocalMissionId(cWScriptMan* wscriptMan) {
    return ((int)wscriptMan->LocalMissionId);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getTheWScriptMan);
#ifdef SKIP_ASM
extern int cWScriptMan_TheWScriptMan;

/**
 * Returns the world script manager singleton, cWScriptMan_TheWScriptMan.
 *
 * Out-of-line copy of an inline accessor that other units inline (seg/seg_1FBE38).
 *
 * @return the manager
 */
extern "C" int cWScriptMan_getTheWScriptMan(void) {
    return cWScriptMan_TheWScriptMan;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getInst);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current instance record (On Tour Inst, +0x290).
 *
 * cWScriptMan_update sets it to the instance an event refers to while the event is dispatched.
 *
 * @param wscriptMan  the script manager
 * @return the instance record, or 0
 */
extern "C" int cWScriptMan_getInst(cWScriptMan* wscriptMan) {
    return ((int)wscriptMan->Inst);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getBeing);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current being (On Tour Being, +0x0), the index of the rider scripts act on.
 *
 * @param wscriptMan  the script manager
 * @return the being index
 */
extern "C" int cWScriptMan_getBeing(int *wscriptMan) {
    return *wscriptMan;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getCollideInfo);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current collision info (On Tour CollideInfo, +0x294).
 *
 * @param wscriptMan  the script manager
 * @return the collision info pointer
 */
extern "C" int cWScriptMan_getCollideInfo(cWScriptMan* wscriptMan) {
    return ((int)wscriptMan->CollideInfo);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getIntersectInfo);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current intersection info (On Tour IntersectInfo, +0x298).
 *
 * @param wscriptMan  the script manager
 * @return the intersection info pointer
 */
extern "C" int cWScriptMan_getIntersectInfo(cWScriptMan* wscriptMan) {
    return ((int)wscriptMan->IntersectInfo);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_setInst);
#ifdef SKIP_ASM
/**
 * Sets the script manager's current instance record (On Tour Inst, +0x290) to @p inst.
 *
 * @param wscriptMan  the script manager
 * @param inst  the instance record (passed as an int), or 0
 */
extern "C" void cWScriptMan_setInst(cWScriptMan* wscriptMan, tInstanceRecord* inst) {
    wscriptMan->Inst = inst;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getGlobalTable);
#ifdef SKIP_ASM
/**
 * Returns the global Luno table of world section @p sectionId (the array at +0x3CC, On Tour GlobalTables).
 *
 * cWScriptMan_allocateGlobalTable allocates these tables; no bounds check.
 *
 * @param wscriptMan  the script manager
 * @param sectionId  the world section index
 * @return the section's cLunoTable, or 0 when none was allocated
 */
extern "C" int cWScriptMan_getGlobalTable(int wscriptMan, int sectionId) {
    return (*(int *)(((char*)(wscriptMan) + (sectionId << 2)) + (0x3CC)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getCurrentTask);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current task (On Tour CurrentTask, +0x2A8).
 *
 * @param wscriptMan  the script manager
 * @return the task, or 0
 */
extern "C" int cWScriptMan_getCurrentTask(cWScriptMan* wscriptMan) {
    return ((int)wscriptMan->CurrentTask);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_setCurrentTask);
#ifdef SKIP_ASM
/**
 * Sets the script manager's current task (On Tour CurrentTask, +0x2A8) to @p task.
 *
 * @param wscriptMan  the script manager
 * @param task  the task (passed as an int), or 0
 */
extern "C" void cWScriptMan_setCurrentTask(cWScriptMan* wscriptMan, cWScriptTask* task) {
    wscriptMan->CurrentTask = task;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getCurrentProcess);
#ifdef SKIP_ASM
/**
 * Returns the script manager's current process (On Tour CurrentProcess, +0x2A4).
 *
 * cWScriptMan_update sets it to each active process while that process handles an event or updates.
 *
 * @param wscriptMan  the script manager
 * @return the process, or 0
 */
extern "C" int cWScriptMan_getCurrentProcess(cWScriptMan* wscriptMan) {
    return ((int)wscriptMan->CurrentProcess);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isMenuQueueReady);
#ifdef SKIP_ASM
/**
 * Returns whether the script manager's mission menu queue (+0x1C4, On Tour MissionMenuQueue) holds an entry.
 *
 * Compares the queue's two indices (+0x1C4 and +0x1C8; On Tour Current and Next), as tMissionMenuQueue's isReady
 * does. ssxappdtor inlines the same test.
 *
 * @param wscriptMan  the script manager
 * @return 1 when they differ, else 0
 */
extern "C" int cWScriptMan_isMenuQueueReady(cWScriptMan* wscriptMan) {
    return (wscriptMan->MissionMenuQueue.Current) != (wscriptMan->MissionMenuQueue.Next);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_popMenuQueueEntry);
#ifdef SKIP_ASM
/**
 * Pops the next entry from the script manager's mission menu queue (+0x1C4) with tMissionMenuQueue_getNextEntry.
 *
 * The callee's result (the entry) is left as this function's result, though the C here declares none.
 */
extern "C" void cWScriptMan_popMenuQueueEntry(cWScriptMan* self)
{
    tMissionMenuQueue_getNextEntry(&self->MissionMenuQueue);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_getMenuQueueEntry);
#ifdef SKIP_ASM
/**
 * Returns the current entry of the script manager's mission menu queue (+0x1C4) via tMissionMenuQueue_getEntry.
 *
 * The callee's result (the entry) is left as this function's result, though the C here declares none.
 */
extern "C" void cWScriptMan_getMenuQueueEntry(cWScriptMan* self)
{
    tMissionMenuQueue_getEntry(&self->MissionMenuQueue);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_setBeing);
#ifdef SKIP_ASM
/**
 * Sets the script manager's current being (On Tour Being, +0x0) to @p being.
 *
 * @param wscriptMan  the script manager
 * @param being  the being (rider) index
 */
extern "C" void cWScriptMan_setBeing(int *wscriptMan, int being) {
    *wscriptMan = being;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D390);
#ifdef SKIP_ASM
/**
 * Returns whether @p eventId is one of the 10 process event ids (0-9, unsigned compare); @p self is not used.
 *
 * cWScriptProcess_dispatchProcessEvent dispatches exactly these ten ids to a process's event handlers.
 *
 * @param eventId  the event id
 * @return 1 when @p eventId < 10, else 0
 */
extern "C" int func_0030D390(void* self, unsigned int eventId) {
    return eventId < 10;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_clearState);
#ifdef SKIP_ASM
/**
 * Clears a script process's state word (+0x0) to 0.
 *
 * @param process  the process
 */
extern "C" void cWScriptProcess_clearState(int *process) {
    *process = 0;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_getNext);
#ifdef SKIP_ASM
/**
 * Returns the next process in its process list (On Tour Next, +0x18).
 *
 * cWScriptMan_update walks the active list through this link.
 *
 * @param process  the process
 * @return the next process, or 0 at the end of the list
 */
extern "C" int cWScriptProcess_getNext(cWScriptProcess* process) {
    return ((int)process->Next);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_setNext);
#ifdef SKIP_ASM
/**
 * Sets the next process in a process's list link (On Tour Next, +0x18) to @p nextProcess.
 *
 * @param process  the process
 * @param nextProcess  the process to link after it (passed as an int), or 0
 */
extern "C" void cWScriptProcess_setNext(void *process, int nextProcess) {
    (*(int *)((char*)(process) + (0x18))) = nextProcess;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_getSectionId);
#ifdef SKIP_ASM
/**
 * Returns the world section a script process belongs to (On Tour SectionId, +0x24).
 *
 * cWScriptProcess_cWScriptProcess stores it there.
 *
 * @param process  the process
 * @return the section id
 */
extern "C" int cWScriptProcess_getSectionId(cWScriptProcess* process) {
    return ((int)process->SectionId);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_setTrickDef);
#ifdef SKIP_ASM
/**
 * Sets the trick definition of a script process's manager (+0x10, its TrickDef at +0x29C) to @p trickDef.
 *
 * The same store as cWScriptMan_setTrickDef, made through the process's manager pointer.
 *
 * @param process  the process
 * @param trickDef  the trick definition (passed as an int), or 0
 */
extern "C" void cWScriptProcess_setTrickDef(cWScriptProcess* process, int trickDef) {
    (*(int *)((char*)((process->Manager)) + (0x29C))) = trickDef;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_getLocalTable);
#ifdef SKIP_ASM
/**
 * Returns a script process's local Luno table (On Tour Local, +0x1C).
 *
 * @param process  the process
 * @return the table, or 0 when the process has none
 */
extern "C" int cWScriptProcess_getLocalTable(cWScriptProcess* process) {
    return ((int)process->Local);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_getProcessId);
#ifdef SKIP_ASM
/**
 * Returns a script process's id (On Tour ProcessId, +0x20), copied from its definition by the constructor.
 *
 * @param process  the process
 * @return the process id
 */
extern "C" int cWScriptProcess_getProcessId(cWScriptProcess* process) {
    return ((int)process->ProcessId);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_isMission__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a world-script process is not a mission.
 *
 * @return always 0 (cWScriptMission returns 1)
 */
int cWScriptProcess_isMission(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_runScriptCode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cWScriptMan_runScriptCode_a1(void* self) __asm__("cWScriptMan_runScriptCode");

/**
 * Runs script code for a process through the process's manager: cWScriptMan_runScriptCode(+0x10, a1, a2).
 *
 * Only the manager argument is computed; the second and third arguments (the process and the script code) are passed
 * through untouched.
 */
extern "C" void cWScriptProcess_runScriptCode(cWScriptProcess* self)
{
    cWScriptMan_runScriptCode_a1(self->Manager);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptProcess_runScriptCode1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cWScriptMan_runScriptCode1_a1(void* self) __asm__("cWScriptMan_runScriptCode1");

/**
 * Runs script code for a task through the process's manager: cWScriptMan_runScriptCode1(+0x10, a1, a2).
 *
 * Only the manager argument is computed; the second and third arguments (the task and the script code) are passed
 * through untouched. cWScriptMan_runScriptCode1 is the manager's task variant of cWScriptMan_runScriptCode.
 */
extern "C" void cWScriptProcess_runScriptCode1(cWScriptProcess* self)
{
    cWScriptMan_runScriptCode1_a1(self->Manager);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptTask__cWScriptTask);
#ifdef SKIP_ASM
extern void* cWScriptTask_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys a world script task: restores cWScriptTask's vtable at +0x40, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cWScriptTask__cWScriptTask(cWScriptTask* self, int flags)
{
    self->mVtbl = cWScriptTask_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptTask_getLocalTable);
#ifdef SKIP_ASM
/**
 * Returns a script task's local Luno table: that of its process (+0x3C, then the process's +0x1C).
 *
 * @param task  the task
 * @return the process's local table, or 0
 */
extern "C" int cWScriptTask_getLocalTable(cWScriptTask* task) {
    return ((int)task->Process->Local);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptTask_getTaskId);
#ifdef SKIP_ASM
/**
 * Returns a script task's id (On Tour TaskId, +0x4).
 *
 * cWScriptProcess_gotoTask and cWScriptProcess_advanceToTask look tasks up by it.
 *
 * @param task  the task
 * @return the task id
 */
extern "C" int cWScriptTask_getTaskId(cWScriptTask* task) {
    return ((int)task->TaskId);
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptTask_abortTask);
#ifdef SKIP_ASM
/**
 * Marks a script task aborted: sets its state word (+0x0) to 3.
 *
 * cWScriptProcess_gotoTask does the same to the task it leaves.
 *
 * @param task  the task
 */
extern "C" void cWScriptTask_abortTask(int *task) {
    *task = 3;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMission_getSortKey);
#ifdef SKIP_ASM
/**
 * Returns a mission's sort key (On Tour SortKey, +0x64).
 *
 * @param mission  the mission
 * @return the sort key
 */
extern "C" int cWScriptMission_getSortKey(cWScriptMission* mission) {
    return (mission->SortKey);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMission_isMission__FPv);
#ifdef SKIP_ASM
/**
 * Reports that this world-script process is a mission.
 *
 * @return always 1
 */
int cWScriptMission_isMission(void* self)
{
    return 0x1;
}
#endif

extern "C" void* func_0030C8D8(int, int);

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMission_isScopeLocal);
#ifdef SKIP_ASM
/**
 * Returns whether a mission's scope (On Tour Scope, +0x60) is 2, i.e. the mission is local.
 *
 * @param mission  the mission
 * @return 1 when the scope is 2, else 0
 */
extern "C" int cWScriptMission_isScopeLocal(cWScriptMission* mission) {
    return (mission->Scope) == 2;
}
#endif

//99.38%
INCLUDE_ASM("wscript/wscriptman", func_0030D498__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static constructors: calls func_0030C8D8 with (1, 0xFFFF).
 *
 * Listed in the global constructor table (word at 0x0043CEAC).
 *
 * @return func_0030C8D8's result
 */
void* func_0030D498(void* self)
{
    return func_0030C8D8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cAnimModel_cAnimModel);
#ifdef SKIP_ASM
struct sWSVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sWSSlot8 {
    int a;
    int b;
};

/**
 * Constructs an empty animated model (cAnimModel): no parts, bones or matrices yet, and a scale of (1, 1, 1, 1).
 *
 * Nulls the bone matrices unk_030, unk_034 and unk_038, bones, parts, partIndex, boneChannelIndex, localPos and
 * localRot, zeroes numParts, unk_03C, the words at +0x12C and +0x130 and the second word of each lodInfo entry, and
 * sets unk_140 to (1, 1, 1, 1). feridermanager allocates 352 bytes tagged "AnimModel" for it, then calls
 * cAnimModel_init with (100, 15).
 *
 * @return @p self
 */
extern "C" void* cAnimModel_cAnimModel(cAnimModel* self)
{
    sWSVec4 ones;
    self->unk_038 = 0;
    self->unk_030 = 0;
    self->unk_034 = 0;
    ones.x = 1.0f;
    ones.y = 1.0f;
    ones.z = 1.0f;
    ones.w = 1.0f;
    *(sWSVec4*)&self->unk_140 = ones;
    self->bones = 0;
    self->parts = 0;
    self->partIndex = 0;
    self->unk_03C = 0;
    self->numParts = 0;
    self->NumTextureRefs = 0;
    self->TextureRefs = 0;
    self->localPos = 0;
    self->localRot = 0;
    self->boneChannelIndex = 0;
    for (int i = 3; i >= 0; i--) {
        ((sWSSlot8*)(&self->lodInfo[0].unk_04))[i].a = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cAnimModel__cAnimModel);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void cMemMan_free(void* p);
void operator_delete(int* ptr);

struct sWSTex_D540 {
    void* f0;
    int f4;
    int f8;
    void* fC;
};

struct sWSSlot2_D540 {
    sWSTex_D540* arr;
    int cnt;
};

struct sWSGrp_D540 {
    char pad[0x1C];
    sWSSlot2_D540 s[4];         // 0x1C
    char pad3C[0x58 - 0x3C];
};

struct sWSSlot_D540 {
    void* a;
    int b;
};

struct sWSVE_D540 {
    short delta;
    short index;
    void (*fn)(void*, int);
};


struct sWSRec_D540 {
    int id;
    int f4;
    int f8;
};

struct sWSMan_D540 {
    int f0;
    int f4;                     // 0x4
    int f8;                     // 0x8
    sWSGrp_D540* grps;          // 0xC
    char pad10[0xC];
    void* f1C;                  // 0x1C
    void* f20;                  // 0x20
    void* f24;                  // 0x24
    void* f28;                  // 0x28
    void* f2C;                  // 0x2C
    void* f30;                  // 0x30
    void* f34;                  // 0x34
    void* f38;                  // 0x38
    void* f3C;                  // 0x3C
    int f40;
    sWSSlot_D540 slots[4];      // 0x44
    char pad64[0x12C - 0x64];
    int f12C;                   // 0x12C
    sWSRec_D540* f130;          // 0x130
};


// render context singleton (cGraphicsMan_TheGraphicsMan) accessor
static inline cPS2GraphicsMan* GetGfx_D540()
{
    return cGraphicsMan_TheGraphicsMan;
}

/**
 * Destroys an animated model: frees every per-slot and per-group array and releases its graphics resources.
 *
 * Each animation element's handle goes to graphics manager slot 112 and each valid texture id (+0x130
 * entries) to slot 50 before the arrays are freed.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAnimModel__cAnimModel(sWSMan_D540* self, int flags)
{
    cPS2GraphicsMan* gfx = GetGfx_D540();
    int i;
    int j;
    int variant;
    int textureId;
    if (self->f38 != 0) {
        cMemMan_free(self->f38);
    }
    if (self->grps != 0) {
        for (int k = 0; k < 4; k++) {
            if (self->slots[k].a != 0) {
                cMemMan_free(self->slots[k].a);
            }
            for (j = 0; j < self->f8; j++) {
                for (variant = 0; variant < self->f4; variant++) {
                    if (self->grps[j].s[k].arr[variant].fC != 0) {
                        cMemMan_free(self->grps[j].s[k].arr[variant].fC);
                    }
                    ((sWSVE_D540*)gfx->mVtbl)[0x380 / 8].fn((char*)gfx + ((sWSVE_D540*)gfx->mVtbl)[0x380 / 8].delta, (int)self->grps[j].s[k].arr[variant].f0);
                    if (self->grps[j].s[k].arr[variant].f0 != 0) {
                        cMemMan_free(self->grps[j].s[k].arr[variant].f0);
                    }
                }
                if (self->grps[j].s[k].arr != 0) {
                    cMemMan_free(self->grps[j].s[k].arr);
                }
            }
        }
    }
    if (self->f130 != 0) {
        for (i = 0; i < self->f12C; i++) {
            textureId = self->f130[i].id;
            if (textureId >= 0) {
                ((sWSVE_D540*)gfx->mVtbl)[0x190 / 8].fn((char*)gfx + ((sWSVE_D540*)gfx->mVtbl)[0x190 / 8].delta, textureId);
            }
        }
    }
    if (self->f30 != 0) {
        cMemMan_free(self->f30);
    }
    if (self->f34 != 0) {
        cMemMan_free(self->f34);
    }
    if (self->f2C != 0) {
        cMemMan_free(self->f2C);
    }
    if (self->grps != 0) {
        cMemMan_free(self->grps);
    }
    if (self->f1C != 0) {
        cMemMan_free(self->f1C);
    }
    if (self->f3C != 0) {
        cMemMan_free(self->f3C);
    }
    if (self->f130 != 0) {
        cMemMan_free(self->f130);
    }
    if (self->f24 != 0) {
        cMemMan_free(self->f24);
    }
    if (self->f28 != 0) {
        cMemMan_free(self->f28);
    }
    if (self->f20 != 0) {
        cMemMan_free(self->f20);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cAnimModel_init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void MEM_fill(void* dst, int value, int size);
extern char str_ModelRefs[];

struct cAnimModel_init_sPool {
    int count;      // 0x0
    int field_0x4;  // 0x4
    int field_0x8;  // 0x8
    void* items;    // 0xC
    char pad_0x10[0x60 - 0x10];
    int field_0x60; // 0x60
};

/**
 * Sets up an animated model's part storage: allocates @p count zeroed 0x58-byte sAnimModelPart records.
 *
 * Stores @p count as the part capacity (+0x0) and @p maxVariants as numLodRefs (+0x4), allocates parts (+0xC) with operator
 * new tagged "ModelRefs", clears them with MEM_fill, and zeroes numParts (+0x8) and the word at +0x60.
 * feridermanager calls it with (100, 15) right after cAnimModel_cAnimModel.
 *
 * @param count  how many parts the model can hold
 * @param maxVariants  refs per LOD slot (numLodRefs), how many cAnimModel_addModelPartLOD allocates
 */
extern "C" void cAnimModel_init(cAnimModel_init_sPool* self, int count, int maxVariants)
{
    self->field_0x4 = maxVariants;
    self->count = count;
    self->items = operator_new_tag(count * 88, str_ModelRefs, 0, 0);
    MEM_fill(self->items, 0, self->count * 88);
    self->field_0x8 = 0;
    self->field_0x60 = 0;
}
#endif


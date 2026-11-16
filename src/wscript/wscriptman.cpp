#include "common.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_addProcess);
#ifdef SKIP_ASM
extern char str_WScriptProcess[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWScriptProcess_cWScriptProcess(void* self, void* man, int a1, int a2);
int func_0030B260(void* self, void* a1);

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
extern "C" void* cWScriptMan_addProcess(void* self, int sectionId, int data, int front)
{
    void* process = cWScriptProcess_cWScriptProcess(cMemMan_alloc(0x60, str_WScriptProcess, 0x20000000, 0), self, sectionId, data);
    if (front != 0) {
        func_0030B260((char*)self + 0x2BC, process);
    } else {
        func_0030B260((char*)self + 0x2B4, process);
    }
    return process;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_addProcess1);
#ifdef SKIP_ASM
extern char str_WScriptMission[];
extern "C" void* cWScriptMission_cWScriptMission(void* self, void* man, int a1, int a2, int a3);
int func_0030B2C0(void* self, void* a1);

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
extern "C" void* cWScriptMan_addProcess1(void* self, int sectionId, int data, int key, int front)
{
    void* mission = cWScriptMission_cWScriptMission(cMemMan_alloc(0x78, str_WScriptMission, 0x20000000, 0), self, sectionId, data, key);
    if (front != 0) {
        func_0030B2C0((char*)self + 0x2BC, mission);
    } else {
        func_0030B2C0((char*)self + 0x2B4, mission);
    }
    return mission;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309618);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cWSObj_309618 {
public:
    char pad_0x00[0x5C];
    virtual void v00(int);
    virtual void v01();
};

extern "C" void* func_0030B320_a(void* self, int id) __asm__("func_0030B320");
extern "C" int cProcessList_removeProcess_a(void* self, void* node) __asm__("cProcessList_removeProcess");

extern "C" void func_00309618(char* self, int id) {
    cWSObj_309618* obj = (cWSObj_309618*)func_0030B320_a(self + 0x2B8, id);
    if (obj != 0) {
        cProcessList_removeProcess_a(self + 0x2B8, obj);
        obj->v00(3);
        return;
    }
    obj = (cWSObj_309618*)func_0030B320_a(self + 0x2BC, id);
    if (obj != 0) {
        cProcessList_removeProcess_a(self + 0x2BC, obj);
        obj->v00(3);
        return;
    }
    obj = (cWSObj_309618*)func_0030B320_a(self + 0x2B4, id);
    if (obj != 0) {
        cProcessList_removeProcess_a(self + 0x2B4, obj);
        obj->v00(3);
    }
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_003096F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cProcessList_removeProcess_vv(void* self, void* node) __asm__("cProcessList_removeProcess");
extern "C" void func_003096F0(char* self, void* node) {
    if (cProcessList_removeProcess_vv(self + 0x2B8, node) == 0 &&
        cProcessList_removeProcess_vv(self + 0x2B4, node) == 0) {
        cProcessList_removeProcess_vv(self + 0x2BC, node);
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
extern "C" void cWScriptMan_startProcess(void* self, void* obj)
{
    void* saved = *(void**)((char*)self + 0x2A4);
    *(void**)((char*)self + 0x2A4) = obj;
    sVEntry00309750* vt = *(sVEntry00309750**)((char*)obj + 0x5C);
    vt[4].fn((char*)obj + vt[4].delta);
    *(void**)((char*)self + 0x2A4) = saved;
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
extern "C" void cWScriptMan_stopProcess(void* self, void* obj)
{
    void* saved = *(void**)((char*)self + 0x2A4);
    *(void**)((char*)self + 0x2A4) = obj;
    sVEntry00309798* vt = *(sVEntry00309798**)((char*)obj + 0x5C);
    vt[6].fn((char*)obj + vt[6].delta);
    *(void**)((char*)self + 0x2A4) = saved;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_003097E0);
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

extern "C" void func_003097E0(void* self, int value)
{
    sWSOp cmd;
    cmd.op = 0x70000;
    cmd.data.a = value;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309848);
#ifdef SKIP_ASM
extern "C" void func_00309848(void* self)
{
    sWSOp cmd;
    cmd.op = 0x70001;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_003098B0);
#ifdef SKIP_ASM
extern "C" void func_003098B0(void* self)
{
    sWSOp cmd;
    cmd.op = 0x70002;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), cmd);
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
extern "C" void cWScriptMan_eventTrick(void* self, sWSOpData* trickDef)
{
    sWSOp o;
    o.op = 0x70003;
    o.data = *trickDef;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), o);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309990);
#ifdef SKIP_ASM
extern "C" void func_00309990(void* self)
{
    sWSOp cmd;
    cmd.op = 0x70004;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_003099F8);
#ifdef SKIP_ASM
extern "C" void func_003099F8(void* self)
{
    sWSOp cmd;
    cmd.op = 0x70005;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), cmd);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_processStart);
#ifdef SKIP_ASM
struct cWScriptListHead;
struct cWScriptListNode;
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node);
int func_0030B260(void* self, void* a1);

/**
 * Moves a world script process from the idle list (+0x2B4) to the active list (+0x2B8).
 *
 * @param node  the process (On Tour cWScriptProcess*)
 */
extern "C" void cWScriptMan_processStart(void* self, cWScriptListNode* node)
{
    cProcessList_removeProcess((cWScriptListHead*)((char*)self + 0x2B4), node);
    func_0030B260((char*)self + 0x2B8, node);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_processEnd);
#ifdef SKIP_ASM
struct cWScriptListHead;
struct cWScriptListNode;
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node);
extern "C" int cProcessList_findProcess(cWScriptListHead* self, cWScriptListNode* node);
int func_0030B260(void* self, void* a1);

/**
 * Moves a world script process back from the active list (+0x2B8) to the idle list (+0x2B4).
 *
 * Does nothing when the process is not on the active list.
 *
 * @param node  the process (On Tour cWScriptProcess*)
 */
extern "C" void cWScriptMan_processEnd(void* self, cWScriptListNode* node)
{
    cWScriptListHead* list = (cWScriptListHead*)((char*)self + 0x2B8);
    if (cProcessList_findProcess(list, node) != 0) {
        cProcessList_removeProcess(list, node);
        func_0030B260((char*)self + 0x2B4, node);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_processPass);
#ifdef SKIP_ASM
int func_0030B260(void* self, void* a1);
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node);
extern "C" int cProcessList_findProcess(cWScriptListHead* self, cWScriptListNode* node);

/**
 * Retires a world script process whose mission was passed.
 *
 * If @p node is in the active list at +0x2B8 it is removed and added to the idle list at +0x2B4 (func_0030B260). +0x2A0
 * is set to -1 either way.
 *
 * @param node  the process (On Tour process)
 */
extern "C" void cWScriptMan_processPass(void* self, cWScriptListNode* node)
{
    cWScriptListHead* list = (cWScriptListHead*)((char*)self + 0x2B8);
    if (cProcessList_findProcess(list, node) != 0) {
        cProcessList_removeProcess(list, node);
        func_0030B260((char*)self + 0x2B4, node);
    }
    *(unsigned int*)((char*)self + 0x2A0) = 0xFFFFFFFF;
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
extern "C" int cWScriptMan_getScriptCode(void* self, int id)
{
    if (cWScriptMan_getScriptCode_valid(id)) {
        return cWorld_getWScriptScript(*(void**)((char*)self + 0x28C), &id);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309BA8);
#ifdef SKIP_ASM
struct func_00309BA8_sVec {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16)));

struct func_00309BA8_sPos {
    char pad[8];
    float z;
};

struct func_00309BA8_sVEntry {
    short delta;
    short index;
    func_00309BA8_sPos* (*fn)(void*);
};


static inline char* func_00309BA8_toPtr(unsigned int i)
{
    return (char*)(i << 2);
}

static inline char* func_00309BA8_get(char* e, unsigned int h)
{
    unsigned int i = ((unsigned int*)*(char**)(e + 0x1C))[h >> 8] >> 8;
    if (i == 0) {
        return 0;
    }
    return func_00309BA8_toPtr(i);
}

static inline char* func_00309BA8_resolve(char* tbl, unsigned int h)
{
    char* e = ((char**)*(char**)(tbl + 0x8))[h & 0xFF];
    if (e == 0) {
        return 0;
    }
    return func_00309BA8_get(e, h);
}

extern "C" float func_00309BA8(void* self)
{
    if (*(int*)((char*)self + 0x28) == 0) {
        return 0.0f;
    }
    char* node = func_00309BA8_resolve(**(char***)((char*)self + 0x28C), *(unsigned int*)((char*)self + 0x2C));
    char* list = (char*)SSXApp->Game->AI;
    char* obj = *(char**)(list + (*(int*)((char*)self + 0x30) << 2) + 0x28);
    char* sub = obj + 0x6C0;
    func_00309BA8_sVEntry* vt = *(func_00309BA8_sVEntry**)sub;
    func_00309BA8_sPos* riderPos = vt[5].fn(sub + vt[5].delta);
    float z = riderPos->z;
    func_00309BA8_sVec pos = *(func_00309BA8_sVec*)(node + 0x40);
    float height = z - pos.z;
    if (height >= 0.0f) {
        return height;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309C88);
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
void func_00226600(void* self, int a1, int a2);
extern "C" void* func_002224A8(void* self);
extern "C" void func_002224B8(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);
extern "C" void func_00309C88(void* self, int* handle)
{
    sLunoValue_9C88 scriptVar;
    sLunoValue_9C88 ret;
    sLunoCall_9C88 call;
    func_00226600(&scriptVar, cWorld_getWScriptScript(*(void**)((char*)self + 0x28C), handle), 0);
    ret.type = 0;
    ret.word0 = 0;
    void* table = *(void**)((char*)self + (*(unsigned char*)handle << 2) + 0x3CC);
    func_002224A8(&call);
    func_002224B8(&call, &scriptVar, table, &ret);
    luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309D20);
#ifdef SKIP_ASM
struct cWScriptMan_isFunc_sValue;
// cLunoValue: { word0, word1, type } (same layout as cWScriptMan_isFunc_sValue)
struct func_00309D20_sValue {
    int word0;
    int word1;
    int type;
};
struct func_00309D20_sCall {
    int pad[4];
};

void func_00226600(void* self, int a1, int a2);
extern "C" void* func_002224A8(void* self);
extern "C" void func_002224B8(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);

extern "C" void func_00309D20(void* self, int* handle, int id)
{
    func_00309D20_sValue scriptVar;
    func_00309D20_sValue ret;
    func_00309D20_sCall call;
    int saved = *(int*)((char*)self + 0x290);
    *(int*)((char*)self + 0x290) = id;
    func_00226600(&scriptVar, cWorld_getWScriptScript(*(void**)((char*)self + 0x28C), handle), 0);
    ret.type = 0;
    ret.word0 = 0;
    void* table = *(void**)((char*)self + (*(unsigned char*)handle << 2) + 0x3CC);
    func_002224A8(&call);
    func_002224B8(&call, &scriptVar, table, &ret);
    *(int*)((char*)self + 0x290) = saved;
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

void* func_00226620(void* self);
extern "C" cWScriptMan_isFunc_sValue* func_00225248(void* table, cWScriptMan_isFunc_sValue* key);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);

/**
 * Tells whether a world section's script table defines a given function.
 *
 * Returns 0 when section @p idx has no table at +0x3CC. Otherwise makes an integer key from @p value, looks it up with
 * func_00225248 and checks the result's type is 5 (function).
 *
 * @param idx  the world section (On Tour sectionId)
 * @param value  the function id (On Tour funcId)
 * @return 1 when the entry is a function, else 0
 */
extern "C" int cWScriptMan_isFunc(void* self, int idx, int value)
{
    int off = idx << 2;
    char* base = (char*)self + 0x3CC;
    void** slot = (void**)(base + off);
    if (*slot != 0) {
        cWScriptMan_isFunc_sValue key;
        key.type = 3;
        *(int*)func_00226620(&key) = value;
        int r = func_00225248(*slot, &key)->type == 5;
        luno_cLunoVar__cLunoVar(&key, 2);
        return r;
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

extern "C" void* func_002224A8(void* self);
extern "C" void func_002224B8(void* self, void* args, void* table, void* ret);

/**
 * Runs one function of a world section's script table.
 *
 * Makes an integer key from @p value, gets the entry from the table of section @p idx (+0x3CC), builds a VM
 * (func_002224A8) and runs the function with that table (func_002224B8), then destroys the result and the key.
 *
 * @param idx  the world section (On Tour sectionId)
 * @param value  the function id (On Tour funcId)
 */
extern "C" void cWScriptMan_runFunc(void* self, int idx, int value)
{
    cWScriptMan_isFunc_sValue key;
    key.type = 3;
    *(int*)func_00226620(&key) = value;
    int off = idx << 2;
    char* base = (char*)self + 0x3CC;
    void** slot = (void**)(base + off);
    cWScriptMan_runFunc_sArg arg = *(cWScriptMan_runFunc_sArg*)func_00225248(*slot, &key);
    cWScriptMan_isFunc_sValue ret;
    ret.type = 0;
    ret.word0 = 0;
    cWScriptMan_runFunc_sCall call;
    func_002224A8(&call);
    func_002224B8(&call, &arg, *slot, &ret);
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

struct sWSMan9F18 {
    int f0;                     // 0x000
    char pad4[0x288];
    sWSWorld9F18* world;        // 0x28C
    int f290;                   // 0x290
    int f294;                   // 0x294
    int f298;                   // 0x298
    char pad29C[0x8];
    int f2A4;                   // 0x2A4
    int f2A8;                   // 0x2A8
    char pad2AC[0x14];
    char tables[1][0x10C];      // 0x2C0
};

struct cWScriptIdTable;
extern "C" void cCollectPool_init(cWScriptIdTable* self);
extern "C" void func_00309C88(void* self, int* idx);
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
extern "C" void cWScriptMan_runGlobalScripts(sWSMan9F18* self)
{
    int i;
    int saved;
    int n;
    cAvalanche_reset();
    {
        int k;
        for (k = 0; k < 1; k++) {
            cCollectPool_init((cWScriptIdTable*)self->tables[k]);
        }
    }
    saved = self->f0;
    self->f0 = -1;
    self->f290 = 0;
    self->f294 = 0;
    self->f298 = 0;
    self->f2A4 = 0;
    self->f2A8 = 0;
    n = cWorldView_getNumSections((cWorldView*)(self->world->view + 0x10));
    for (i = 0; i < n; i++) {
        if (wsIsType3_9F18(&self->world->info->sections[i])) {
            int* globalScript = cWorld_getGlobalScripts(self->world, i);
            if (globalScript != 0) {
                int cnt = cWorld_getNumGlobalScripts(self->world, i);
                int j;
                for (j = 0; j < cnt; j++, globalScript++) {
                    func_00309C88(self, globalScript);
                }
            }
        }
    }
    self->f290 = 0;
    self->f294 = 0;
    self->f298 = 0;
    self->f2A4 = 0;
    self->f2A8 = 0;
    self->f0 = saved;
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

void func_00226600(void* self, int a1, int a2);
extern "C" void* func_002224A8(void* self);
extern "C" void func_002224B8(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_00309C88(void* self, int* idx);
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" int cWorld_getWScriptScript(void*, int*);
extern "C" void func_003097E0(void* self, int v);
extern char str_SetupLocalTable[];

static inline bool valid_30A060(int* id) { return *id != -1; }

/**
 * Runs the world script attached to an object a being collided with.
 *
 * Saves the current being and makes @p obj's (its virtual slot 7) current, remembering the instance (from the intersect
 * info's +0x50) and both infos. If the being's virtual slot 8 says so, a collide event for the instance is raised
 * (func_003097E0), unless a non-replay rider has its +0x480 flag set, which ends the call. The instance's collide
 * script id (header entry 2) is then run directly (func_00309C88) when no process or task is current, or otherwise in
 * the current task's or process's local table (a new 8-slot table named "SetupLocalTable" when there is none). Finally
 * the instance's +0xC object gets virtual slot 41 with 0.5, and the being and saved fields are restored.
 *
 * @param collideInfo  collision info (On Tour tc)
 * @param intersectInfo  intersection info (On Tour ti)
 * @param obj  the being that collided (On Tour pBeing)
 */
extern "C" void cWScriptMan_doOnCollideScript(void* self, void* collideInfo, void* intersectInfo, void* obj)
{
    int saved = *(int*)self;
    {
        sVE_30A060* vt = *(sVE_30A060**)obj;
        *(int*)self = vt[7].fn((char*)obj + vt[7].delta);
    }
    *(int*)((char*)self + 0x290) = *(int*)((char*)intersectInfo + 0x50);
    *(void**)((char*)self + 0x294) = collideInfo;
    *(void**)((char*)self + 0x298) = intersectInfo;
    int r;
    {
        sVE_30A060* vt = *(sVE_30A060**)obj;
        r = vt[8].fn((char*)obj + vt[8].delta);
    }
    if (r) {
        if (SSXApp->Game->mCurrentStateID != 4) {
            char* rider = obj ? (char*)obj - 0x6C0 : 0;
            if (*(int*)(rider + 0x480) != 0) {
                *(int*)self = saved;
                goto done;
            }
        }
        func_003097E0(self, *(int*)(*(char**)((char*)self + 0x290) + 0x78));
    }
    {
        sValue_30A060 arg;
        sValue_30A060 ret;
        sCall_30A060 call;
        int id = cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), *(int*)((char*)self + 0x290), 2);
        if (valid_30A060(&id)) {
            if (*(int*)((char*)self + 0x2A4) == 0 && *(int*)((char*)self + 0x2A8) == 0) {
                func_00309C88(self, &id);
            } else {
                func_00226600(&arg, cWorld_getWScriptScript(*(void**)((char*)self + 0x28C), &id), 0);
                ret.type = 0;
                ret.word0 = 0;
                func_002224A8(&call);
                char* o = *(char**)((char*)self + 0x2A8);
                if (o != 0)
                    o = *(char**)(o + 0x3C);
                else
                    o = *(char**)((char*)self + 0x2A4);
                void* table = *(void**)(o + 0x1C);
                if (table != 0) {
                    func_002224B8(&call, &arg, table, &ret);
                } else {
                    table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
                    func_002224B8(&call, &arg, table, &ret);
                    if (table != 0)
                        luno_cLunoTable__cLunoTable(table, 3);
                }
                luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
            }
        }
    }
    {
        char* v = *(char**)(*(char**)((char*)self + 0x290) + 0xC);
        if (v != 0) {
            sVEF_30A060* vt = *(sVEF_30A060**)(v + 0xC);
            vt[41].fn(v + vt[41].delta, 0.5f);
        }
    }
    *(int*)self = saved;
done:
    *(int*)((char*)self + 0x294) = 0;
    *(int*)((char*)self + 0x298) = 0;
    *(int*)((char*)self + 0x290) = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A270);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);

extern "C" int func_0030A270(void* self, int id)
{
    return cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), id, 1) != -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", func_0030A298);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void func_00309C88(void* self, int* idx);

extern "C" void func_0030A298(void* self, int id)
{
    int idx;
    *(int*)((char*)self + 0x290) = id;
    idx = cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), id, 1);
    int ok = idx != -1;
    if (ok) {
        func_00309C88(self, &idx);
    }
    *(int*)((char*)self + 0x290) = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A2E8);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);

extern "C" int func_0030A2E8(void* self, int id)
{
    return cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), id, 3) != -1;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A310);
#ifdef SKIP_ASM
struct sEntry_A310 {
    short type;
    char pad[10];
};
// PORT: the helpers take the object as int in this unit.
extern "C" int func_0030A310(void* self, void* obj)
{
    char* inst = (char*)obj;
    if (*(int*)(inst + 0xC) != 0) {
        return 1;
    }
    sEntry_A310* tab = *(sEntry_A310**)(*(char**)(*(char**)((char*)self + 0x28C) + 0x4) + 0x4);
    bool isType3 = tab[*(unsigned char*)(inst + 0x78)].type == 3;
    if (isType3) {
        if (func_0030A270(self, (int)obj) || func_0030A2E8(self, (int)obj)) {
            return 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", func_0030A3A0);
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
    virtual int v20();
    virtual int v21(void* obj);
    virtual int v22(void* obj);
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
    virtual void v35();
};

struct func_0030A3A0_sType {
    short type;
    short pad;
    int f4;
    int f8;
};

// PORT: func_0030A270/func_0030A298 take the object pointer as an int id
extern "C" void func_0030A3A0(void* self, void* obj)
{
    cWScriptCtl* ctl = *(cWScriptCtl**)((char*)obj + 0xC);
    if (ctl != 0) {
        if (ctl->v20() != 0) {
            (*(cWScriptCtl**)((char*)obj + 0xC))->v21(obj);
        }
    } else {
        func_0030A3A0_sType* t = *(func_0030A3A0_sType**)(*(char**)(*(char**)((char*)self + 0x28C) + 0x4) + 0x4);
        bool isScript = t[*(unsigned char*)((char*)obj + 0x78)].type == 3;
        if (isScript) {
            if (func_0030A270(self, (int)obj) != 0) {
                func_0030A298(self, (int)obj);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_downFocusScript);
#ifdef SKIP_ASM
// script object controller: 0xC bytes of data, then the vptr
extern "C" void cWScriptMan_doOnLoseFocusScript(void* self, int id);

/**
 * Handles a world instance losing the player's focus.
 *
 * If the instance has a script controller (+0xC): when the controller's virtual slot 20 says so, its slot 22 handles
 * the instance and nothing else happens; a type-6 controller does nothing; otherwise the controller's own instance
 * (+0x18) runs its lose-focus script when func_0030A2E8 finds one, or the controller's slot 35 is called. An instance
 * without a controller runs its lose-focus script when it has one.
 *
 * @param obj  the instance losing focus (On Tour inst)
 */
// PORT: func_0030A2E8/cWScriptMan_doOnLoseFocusScript take the object pointer as an int id
extern "C" void cWScriptMan_downFocusScript(void* self, void* obj)
{
    cWScriptCtl* ctl = *(cWScriptCtl**)((char*)obj + 0xC);
    if (ctl != 0) {
        if (ctl->v20() != 0) {
            (*(cWScriptCtl**)((char*)obj + 0xC))->v22(obj);
            return;
        }
        ctl = *(cWScriptCtl**)((char*)obj + 0xC);
        if (*(short*)((char*)ctl + 0x10) == 6) {
            return;
        }
        obj = *(void**)((char*)ctl + 0x18);
        if (func_0030A2E8(self, (int)obj) != 0) {
            cWScriptMan_doOnLoseFocusScript(self, (int)obj);
            return;
        }
        (*(cWScriptCtl**)((char*)obj + 0xC))->v35();
        return;
    }
    if (func_0030A2E8(self, (int)obj) != 0) {
        cWScriptMan_doOnLoseFocusScript(self, (int)obj);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doOnLoseFocusScript);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void func_00309C88(void* self, int* idx);

/**
 * Runs a world instance's lose-focus script.
 *
 * Stores the instance as current (+0x290), gets its script id for entry 3 from the world with
 * cWorld_getWScriptHeaderId, runs it with func_00309C88 when it is not -1 and clears the current instance.
 *
 * @param id  the instance record, passed as an int (On Tour inst)
 */
extern "C" void cWScriptMan_doOnLoseFocusScript(void* self, int id)
{
    int idx;
    *(int*)((char*)self + 0x290) = id;
    idx = cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), id, 3);
    int ok = idx != -1;
    if (ok) {
        func_00309C88(self, &idx);
    }
    *(int*)((char*)self + 0x290) = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A598);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);

extern "C" int func_0030A598(void* self, int id)
{
    return cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), id, 4) != -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", func_0030A5C0);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void func_00309C88(void* self, int* idx);

extern "C" void func_0030A5C0(void* self, int id)
{
    int idx;
    *(int*)((char*)self + 0x290) = id;
    idx = cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), id, 4);
    int ok = idx != -1;
    if (ok) {
        func_00309C88(self, &idx);
    }
    *(int*)((char*)self + 0x290) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", func_0030A610);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void func_00309C88(void* self, int* idx);

extern "C" void func_0030A610(void* self, int id)
{
    int idx;
    *(int*)((char*)self + 0x290) = id;
    idx = cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), id, 0);
    int ok = idx != -1;
    if (ok) {
        func_00309C88(self, &idx);
    }
    *(int*)((char*)self + 0x290) = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A660);
#ifdef SKIP_ASM
extern "C" int func_0030A660(char* self, int scriptId)
{
    return cWorld_getWScriptHeaderId(*(void**)(self + 0x28C), scriptId, 5) != -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", func_0030A688);
#ifdef SKIP_ASM
extern "C" int cWorld_getWScriptHeaderId(void* world, int id, int flag);
extern "C" void func_00309C88(void* self, int* idx);

extern "C" void func_0030A688(void* self, int id)
{
    int idx;
    *(int*)((char*)self + 0x290) = id;
    idx = cWorld_getWScriptHeaderId(*(void**)((char*)self + 0x28C), id, 5);
    int ok = idx != -1;
    if (ok) {
        func_00309C88(self, &idx);
    }
    *(int*)((char*)self + 0x290) = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A6D8);
#ifdef SKIP_ASM
extern "C" void func_00307EC0(void* p);

extern "C" void func_0030A6D8(void* self)
{
    void* process = *(void**)((char*)self + 0x2A4);
    if (process != 0) {
        func_00307EC0(process);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A700);
#ifdef SKIP_ASM
extern "C" void func_00307F58(void* p);

extern "C" void func_0030A700(void* self)
{
    void* process = *(void**)((char*)self + 0x2A4);
    if (process != 0) {
        func_00307F58(process);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_doProcessCmd);
#ifdef SKIP_ASM
extern "C" void* func_0030B320(void* self, int id);
extern "C" void cWScriptProcess_end(void* obj);
extern "C" void cWScriptProcess_endTask(void* obj);

/**
 * Carries out a script command on a world script process.
 *
 * With @p id -1 the current process (+0x2A4) is used, otherwise the process is looked up with func_0030B320. Command 1
 * looks in the idle list (+0x2B4) and starts it (cWScriptMan_startProcess); 0 stops an active one (cWScriptMan_stopProcess); 2 ends its
 * current task (cWScriptProcess_endTask); 18 ends the process (cWScriptProcess_end). Other commands are ignored.
 *
 * @param id  the process id, or -1 for the current process (On Tour processId)
 * @param kind  the command (On Tour cmd)
 */
extern "C" void cWScriptMan_doProcessCmd(void* self, unsigned int id, int kind)
{
    void* obj = *(void**)((char*)self + 0x2A4);
    switch (kind) {
    case 1:
        if (id != 0xFFFFFFFF) {
            obj = func_0030B320((char*)self + 0x2B4, id);
        }
        if (obj != 0) {
            cWScriptMan_startProcess(self, obj);
        }
        break;
    case 0:
        if (id != 0xFFFFFFFF) {
            obj = func_0030B320((char*)self + 0x2B8, id);
        }
        if (obj != 0) {
            cWScriptMan_stopProcess(self, obj);
        }
        break;
    case 2:
        if (id != 0xFFFFFFFF) {
            obj = func_0030B320((char*)self + 0x2B8, id);
        }
        if (obj != 0) {
            cWScriptProcess_endTask(obj);
        }
        break;
    case 18:
        if (id != 0xFFFFFFFF) {
            obj = func_0030B320((char*)self + 0x2B8, id);
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
extern "C" void cWScriptMan_doProcessCmd(void* self, unsigned int id, int kind);
// PORT: the unit defines tMissionMenuQueue_addEntry later as (cWSQueue*, int, int, int)
extern "C" void tMissionMenuQueue_addEntry_a(void* queue, unsigned int id, int kind, int ctx) __asm__("tMissionMenuQueue_addEntry");
extern "C" void* cWScriptMan_getCurrentMission(void* self);
extern "C" int cWScriptMan_isLocalMissionActive(void* self);
extern "C" int cBEMissionInterface_isMissionCompleted(void* iface, unsigned int id);
extern "C" int func_00153DD0(void* iface, unsigned int id);
extern "C" int cBEMissionInterface_isMissionLocked(void* iface, unsigned int id);
extern "C" int cBEMissionInterface_isMissionAvailable(void* iface, unsigned int id);
extern "C" int func_00153ED8(void* iface, unsigned int id);
extern "C" int cBEMissionInterface_getNextMissionID(void* iface, unsigned int id);
extern "C" void* func_0028B180();
extern "C" void func_0029D6E0(void* p, unsigned int id);
extern "C" void cSSXAudio_BigChallengeFailed(void* p, int a1);
extern signed char D_00535C10[];

class cWSObj_30A868 {
public:
    char pad_0x00[0x20];
    unsigned int id;        // 0x20
    char pad_0x24[0x5C - 0x24];
    // vptr at 0x5C
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
    virtual void v20();
};

// PORT: the unit defines func_0030B388 later with its own class type
extern "C" cWSObj_30A868* func_0030B388_a(void* self, unsigned int id) __asm__("func_0030B388");

/**
 * Carries out a mission command from a world script, or answers a question about a mission.
 *
 * Except for commands 15 and 19, nothing happens unless D_00535C10 is 4 and the AI's +0x7C is 1. The rider is
 * temporarily set to @p ctx. Commands 0-2, 18 and 8/9/11 are forwarded to helpers; 3, 4, 5 and 17 call
 * virtuals of the running (or waiting) mission. Queries use the mission interface: 14 available, 15
 * completed, 19 last in its chain, 12, 13 and 16 other states. An id of -1 means the current mission.
 *
 * @param id   mission id, or -1 for the current one (On Tour missionId)
 * @param cmd  the command
 * @param ctx  rider index to run it for, or -1 for the current one (On Tour being)
 * @return the query's answer (1 or 0); 0 for plain commands
 */
extern "C" int cWScriptMan_doMissionCmd(char* self, unsigned int id, unsigned int cmd, int ctx)
{
    int force = 0;
    if (cmd == 15 || cmd == 19) {
        force = 1;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (force == 0) {
        if (D_00535C10[0] != 4) {
            return 0;
        }
        if (*(int*)((char*)SSXApp->Game->AI + 0x7C) != 1) {
            return 0;
        }
    }
    int saved = *(int*)self;
    if (ctx != -1) {
        *(int*)self = ctx;
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
            tMissionMenuQueue_addEntry_a(self + 0x1C4, id, cmd, *(int*)self);
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
            obj = func_0030B388_a(self + 0x2B8, id);
            if (obj == 0) {
                obj = func_0030B388_a(self + 0x2B4, id);
            }
        }
        if (obj != 0) {
            obj->v17();
            func_0029D6E0(func_0028B180(), id);
        }
        break;
    case 4:
        if (id != 0xFFFFFFFF) {
            obj = func_0030B388_a(self + 0x2B8, id);
        }
        if (obj != 0) {
            obj->v02();
        }
        break;
    case 5:
        if (id != 0xFFFFFFFF) {
            obj = func_0030B388_a(self + 0x2B8, id);
        }
        if (obj != 0) {
            obj->v19();
            cSSXAudio_BigChallengeFailed(func_0028B180(), 0);
        }
        break;
    case 17:
        if (id != 0xFFFFFFFF) {
            obj = func_0030B388_a(self + 0x2B8, id);
            if (obj == 0) {
                obj = func_0030B388_a(self + 0x2B4, id);
            }
        }
        if (obj != 0) {
            obj->v20();
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
        result = func_00153DD0(iface, id) != 0;
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
        if (*(int*)(self + 0x2AC) == 1) {
            break;
        }
        if (cWScriptMan_isLocalMissionActive(self) != 0) {
            break;
        }
        if (func_00153ED8(iface, id) != 0) {
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
    *(int*)self = saved;
    return result;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_runScriptCode);
#ifdef SKIP_ASM
struct cWScriptMan_isFunc_sValue;
void func_00226600(void* self, int a1, int a2);
extern "C" void* func_002224A8(void* self);
extern "C" void func_002224B8(void* self, void* args, void* table, void* ret);
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
extern "C" void cWScriptMan_runScriptCode(void* self, void* obj, int data)
{
    cWScriptMan_runScriptCode_sValue scriptFunc;
    cWScriptMan_runScriptCode_sValue ret;
    cWScriptMan_runScriptCode_sCall call;
    int savedId;
    void* savedObj = *(void**)((char*)self + 0x2A4);
    *(void**)((char*)self + 0x2A4) = obj;
    savedId = *(int*)((char*)self + 0x290);
    func_00226600(&scriptFunc, data, 0);
    ret.type = 0;
    ret.word0 = 0;
    func_002224A8(&call);
    void* table = *(void**)(*(char**)((char*)self + 0x2A4) + 0x1C);
    if (table != 0) {
        func_002224B8(&call, &scriptFunc, table, &ret);
    } else {
        table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
        func_002224B8(&call, &scriptFunc, table, &ret);
        if (table != 0) {
            luno_cLunoTable__cLunoTable(table, 3);
        }
    }
    *(void**)((char*)self + 0x2A4) = savedObj;
    *(int*)((char*)self + 0x290) = savedId;
    luno_cLunoVar__cLunoVar((cWScriptMan_isFunc_sValue*)&ret, 2);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030ADA8);
#ifdef SKIP_ASM
struct cWScriptMan_isFunc_sValue;
void func_00226600(void* self, int a1, int a2);
extern "C" void* func_002224A8(void* self);
extern "C" void func_002224B8(void* self, void* args, void* table, void* ret);
extern "C" void luno_cLunoVar__cLunoVar(cWScriptMan_isFunc_sValue* self, int flags);
extern "C" void* luno_cLunoTable_cLunoTable(void* mem, int a1);
extern "C" void luno_cLunoTable__cLunoTable(void* obj, int flags);
extern char str_SetupLocalTable[];

// cLunoValue: { word0, word1, type }
struct func_0030ADA8_sValue {
    int word0;
    int word1;
    int type;
};
struct func_0030ADA8_sCall {
    int pad[4];
};

extern "C" void func_0030ADA8(void* self, void* obj, int a2)
{
    func_0030ADA8_sValue arg;
    func_0030ADA8_sValue ret;
    func_0030ADA8_sCall call;
    int savedId;
    void* savedObj = *(void**)((char*)self + 0x2A8);
    *(void**)((char*)self + 0x2A8) = obj;
    savedId = *(int*)((char*)self + 0x290);
    func_00226600(&arg, a2, 0);
    ret.type = 0;
    ret.word0 = 0;
    func_002224A8(&call);
    void* table = *(void**)(*(char**)(*(char**)((char*)self + 0x2A8) + 0x3C) + 0x1C);
    if (table != 0) {
        func_002224B8(&call, &arg, table, &ret);
    } else {
        table = luno_cLunoTable_cLunoTable(cMemMan_alloc(4, str_SetupLocalTable, 0x20000000, 0), 8);
        func_002224B8(&call, &arg, table, &ret);
        if (table != 0) {
            luno_cLunoTable__cLunoTable(table, 3);
        }
    }
    *(void**)((char*)self + 0x2A8) = savedObj;
    *(int*)((char*)self + 0x290) = savedId;
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
extern "C" void* cWScriptMan_getCurrentMission(void* self)
{
    void* obj = *(void**)((char*)self + 0x2A4);
    if (obj != 0) {
        sVEntry0030AEB8* vt = *(sVEntry0030AEB8**)((char*)obj + 0x5C);
        if (vt[3].fn((char*)obj + vt[3].delta) != 0) {
            return *(void**)((char*)self + 0x2A4);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030AF08);
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

// Reads one packed script setting; 0 when no settings block is loaded.
extern "C" int func_0030AF08(void* self, int a1, int which)
{
    sScriptFlags_AF08* fl = *(sScriptFlags_AF08**)((char*)self + 0x29C);
    if (fl != 0) {
        switch (which) {
        case 0: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f0;
        case 1: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f1;
        case 2: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f2;
        case 3: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f3;
        case 4: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f4;
        case 5: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f5;
        case 6: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f6;
        case 7: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f7;
        case 8: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f8;
        case 9: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f9;
        case 10: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f10;
        case 11: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f11;
        case 13: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f13;
        case 14: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f14;
        case 15: return (*(sScriptFlags_AF08**)((char*)self + 0x29C))->f15;
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
INCLUDE_ASM("wscript/wscriptman", func_0030B058__FPv);
#ifdef SKIP_ASM
void* func_0030B058(void* self)
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
    cProcessList_deleteAll_cObj* p;
    while ((p = (cProcessList_deleteAll_cObj*)cProcessList_removeHead(self)) != 0) {
        delete p;
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
    cWScriptListNode* p = (cWScriptListNode*)((cWScriptListHead*)self)->head;
    if (p != 0) {
        ((cWScriptListHead*)self)->head = p->next;
    }
    return p;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B228);
#ifdef SKIP_ASM
extern "C" int func_0030B228(cWScriptListHead* self)
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
INCLUDE_ASM("wscript/wscriptman", func_0030B260__FPvT0);
#ifdef SKIP_ASM
int func_0030B260(void* self, void* node)
{
    int oldHead = (int)((cWScriptListHead*)self)->head;
    ((cWScriptListNode*)node)->next = (void*)oldHead;
    ((cWScriptListHead*)self)->head = node;
    return oldHead;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B270);
#ifdef SKIP_ASM
extern "C" void func_0030B270(cWScriptListHead* self, cWScriptListNode* node)
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
INCLUDE_ASM("wscript/wscriptman", func_0030B2C0__FPvT0);
#ifdef SKIP_ASM
int func_0030B2C0(void* self, void* node)
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
INCLUDE_ASM("wscript/wscriptman", func_0030B320);
#ifdef SKIP_ASM
extern "C" void* func_0030B320(void* self, int id)
{
    void* p = *(void**)self;
    while (p != 0) {
        if (*(int*)((char*)p + 0x20) == id) {
            return p;
        }
        p = *(void**)((char*)p + 0x18);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B350);
#ifdef SKIP_ASM
extern "C" int GetHashValue32__FPc(int);

extern "C" void func_0030B350(void *list, int name) {
    func_0030B320(list, GetHashValue32__FPc(name));
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B388);
#ifdef SKIP_ASM
class func_0030B388_cObj {
public:
    char pad_0x00[0x5C];
    // vptr at 0x5C
    virtual void v01();
    virtual void v02();
    virtual int v03(); // 0x18
};

extern "C" void* func_0030B320(void*, int);

extern "C" func_0030B388_cObj* func_0030B388(void* self, unsigned int id)
{
    func_0030B388_cObj* result = 0;
    if (id != 0xFFFFFFFF) {
        func_0030B388_cObj* p = (func_0030B388_cObj*)func_0030B320(self, id);
        if (p != 0) {
            if (p->v03()) {
                result = p;
            }
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B3F0);
#ifdef SKIP_ASM
extern "C" void func_0030B3F0(void* self, int name)
{
    func_0030B388(self, GetHashValue32__FPc(name));
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
INCLUDE_ASM("wscript/wscriptman", func_0030B458);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0030B388_vv(void* self, void* node) __asm__("func_0030B388");
extern "C" void func_0030B458(char* self, void* node) {
    if (func_0030B388_vv(self + 0x2B8, node) == 0 &&
        func_0030B388_vv(self + 0x2B4, node) == 0) {
        func_0030B388_vv(self + 0x2BC, node);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B4B8__FPv);
#ifdef SKIP_ASM
void func_0030B4B8(void* self)
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
INCLUDE_ASM("wscript/wscriptman", func_0030B4C0__FPvii);
#ifdef SKIP_ASM
void func_0030B4C0(void* self, int points, int goal)
{
    *(int*)((char*)self + 0x4) = 1;
    *(int*)((char*)self + 0x8) = points;
    *(int*)((char*)self + 0xc) = goal;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B4D8__FPvii);
#ifdef SKIP_ASM
void func_0030B4D8(void* self, int count, int maxCount)
{
    *(int*)((char*)self + 0x10) = 1;
    *(int*)((char*)self + 0x14) = count;
    *(int*)((char*)self + 0x18) = maxCount;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B4F0__FPvii);
#ifdef SKIP_ASM
void func_0030B4F0(void* self, int ticks, int dir)
{
    *(int*)((char*)self + 0x1c) = 1;
    *(int*)((char*)self + 0x20) = ticks;
    *(int*)((char*)self + 0x24) = dir;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B508__FPviT0);
#ifdef SKIP_ASM
void func_0030B508(void* self, int being, void* src)
{
    *(int*)((char*)self + 0x28) = 1;
    *(int*)((char*)self + 0x30) = being;
    *(int*)((char*)self + 0x2c) = *(int*)((char*)src + 0x78);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B520);
#ifdef SKIP_ASM
struct cWScriptPair {
    int valueB;
    int valueC;
};

extern "C" void func_0030B520(void* self, cWScriptPair* src)
{
    *(int*)((char*)self + 0x34) = 1;
    *(cWScriptPair*)((char*)self + 0x38) = *src;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B540);
#ifdef SKIP_ASM
extern "C" void cPlaybackMan_stop(void* p);
extern "C" void cReplay_reset(void* p);

extern "C" void func_0030B540(void* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    *(int*)((char*)self + 0x2AC) = 1;
    *(int*)((char*)self + 0x2B0) = *(int*)((char*)self + 0x2A0);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B590);
#ifdef SKIP_ASM
extern "C" int func_0026CAF8(void*, int);
extern "C" int func_0030B590(char* self) {
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    if (func_0026CAF8(SSXApp->Game->PlaybackMan, *(int*)(self + 0x2A0)) != 0) {
        *(int*)(self + 0x2AC) = 5;
        *(int*)(self + 0x2B0) = *(int*)(self + 0x2A0);
        return 1;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B608);
#ifdef SKIP_ASM
void cPlaybackMan_reset(void* self);

extern "C" void func_0030B608(char* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cPlaybackMan_reset(SSXApp->Game->PlaybackMan);
    *(int*)(self + 0x2AC) = 6;
    *(int*)(self + 0x2B0) = *(int*)(self + 0x2A0);
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
extern "C" void cWScriptMan_cancelLocalMission(void* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cPlaybackMan_reset(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    cReplay_startRace((int*)SSXApp->Game->Replay);
    char* riders = (char*)SSXApp->Game->AI;
    cRider_resetForMission(*(void**)(riders + (*(int*)self << 2) + 0x28));
    *(int*)((char*)self + 0x2AC) = 3;
    *(int*)((char*)self + 0x2B0) = *(int*)((char*)self + 0x2A0);
    *(unsigned int*)((char*)self + 0x2A0) = 0xFFFFFFFF;
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
extern "C" void cWScriptMan_restartLocalMission(void* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    cReplay_startRace((int*)SSXApp->Game->Replay);
    *(int*)((char*)self + 0x2AC) = 2;
    *(int*)((char*)self + 0x2B0) = *(int*)((char*)self + 0x2A0);
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
extern "C" void cWScriptMan_quitLocalMission(void* self)
{
    cPlaybackMan_stop(SSXApp->Game->PlaybackMan);
    cPlaybackMan_reset(SSXApp->Game->PlaybackMan);
    cReplay_reset(SSXApp->Game->Replay);
    cReplay_startRace((int*)SSXApp->Game->Replay);
    char* riders = (char*)SSXApp->Game->AI;
    cRider_resetForMission(*(void**)(riders + (*(int*)self << 2) + 0x28));
    *(int*)((char*)self + 0x2AC) = 4;
    *(int*)((char*)self + 0x2B0) = *(int*)((char*)self + 0x2A0);
    *(unsigned int*)((char*)self + 0x2A0) = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_failActiveLocalMission);
#ifdef SKIP_ASM
extern "C" void cWScriptProcess_end(void* obj);
extern "C" void func_0030C790(void* self);

/**
 * Fails the local mission in progress and stops every active script process.
 *
 * When a mission id is set (+0x2A0), ends that mission if it is active (cWScriptProcess_end) and clears the state
 * (+0x2AC = 0, +0x2B0 and +0x2A0 = -1). Every process left in the active list (+0x2B8) is then cleared and moved to the
 * idle list (+0x2B4), and the queue at +0x1C4 is emptied with func_0030C790.
 */
extern "C" void cWScriptMan_failActiveLocalMission(void* self)
{
    unsigned int id = *(unsigned int*)((char*)self + 0x2A0);
    if (id != 0xFFFFFFFF) {
        func_0030B388_cObj* obj = func_0030B388((char*)self + 0x2B8, id);
        if (obj != 0) {
            cWScriptProcess_end(obj);
        }
        *(int*)((char*)self + 0x2AC) = 0;
        *(unsigned int*)((char*)self + 0x2B0) = 0xFFFFFFFF;
        *(unsigned int*)((char*)self + 0x2A0) = 0xFFFFFFFF;
    }
    void* p;
    while ((p = cProcessList_removeHead((char*)self + 0x2B8)) != 0) {
        *(int*)p = 0;
        func_0030B260((char*)self + 0x2B4, p);
    }
    func_0030C790((char*)self + 0x1C4);
}
#endif

extern "C" void* func_0030B320(void*, int);

//100%
INCLUDE_ASM("wscript/wscriptman", cWScriptMan_isProcessActive__FPvT0);
#ifdef SKIP_ASM
/**
 * Tells whether a world script process is in the active list.
 *
 * @param process  the process (On Tour process)
 * @return 1 when a process with @p process's id (+0x20) is in the active list at +0x2B8, else 0
 */
int cWScriptMan_isProcessActive(void* self, void* process)
{
    return (func_0030B320((char*)self + 0x2b8, *(int*)((char*)process + 0x20)) != 0);
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
    virtual int v03();
};

/**
 * Tells whether a local mission is running.
 *
 * @return 1 when some process in the active list (+0x2B8) is a mission (its virtual slot 3) in state 2 (+0x60), else 0
 */
extern "C" int cWScriptMan_isLocalMissionActive(void* self)
{
    int n = 0;
    cWScriptMan_isLocalMissionActive_cNode* node;
    for (node = *(cWScriptMan_isLocalMissionActive_cNode**)((char*)self + 0x2B8); node != 0; node = node->next) {
        if (node->v03() != 0 && *(int*)((char*)node + 0x60) == 2) {
            n++;
        }
    }
    return n != 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B928);
#ifdef SKIP_ASM
struct cWScriptIdTable;
extern "C" void cCollectPool_init(cWScriptIdTable* self);

// same layout as cWScriptIdTable (defined later in the unit)
struct func_0030B928_sSlot {
    unsigned int id;     // 0x0
    int arg;             // 0x4
    int b;               // 0x8
    unsigned int entries[64];
};

extern "C" void func_0030B928(void* self, unsigned int id, int size)
{
    func_0030B928_sSlot* pool = (func_0030B928_sSlot*)((char*)self + 0x2C0);
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
struct func_0030C3E0_sTable;
extern "C" void func_0030C3E0(func_0030C3E0_sTable* self, int id);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);
extern "C" int func_00144C98(void* iface);
extern "C" int func_00151178(void* self, int level);
extern "C" void cRider_flagEventAddCollectible(void* rider, int level, int a2);

struct sWSSlot_B9A0 {
    int id;
    char pad[0x10C - 4];
};

/**
 * Records that a rider picked up a collectible of the manager's pool.
 *
 * When the pool id matches, the instance is marked collected, the pickup sound plays and the rider gets an
 * add-collectible event with the current peak (func_00144C98) and its value (func_00151178).
 *
 * @param rider   rider index (On Tour being)
 * @param id      collectible pool id (On Tour poolId)
 * @param instId  the collected instance's resource id
 */
extern "C" void cWScriptMan_collect(void* self, int rider, int id, int instId)
{
    sWSSlot_B9A0* slots = (sWSSlot_B9A0*)((char*)self + 0x2C0);
    int i;
    for (i = 0; i < 1; i++) {
        if (slots[i].id == id) {
            func_0030C3E0((func_0030C3E0_sTable*)&slots[i], instId);
            cSSXAudio_PlayIconSound(func_0028B180(), 5, 0, 1.0f);
            void* newRace = cBE_getInterface_Fv(cBE_getBE(), 0);
            void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
            int lvl = func_00144C98(newRace);
            int cash = func_00151178(econ, lvl);
            cRider_flagEventAddCollectible(*(void**)((char*)SSXApp->Game->AI + (rider << 2) + 0x28), lvl, cash);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030BA80);
#ifdef SKIP_ASM
struct func_0030BA80_sEntry {
    int id;
    char pad04[0x10C - 4];
};

struct func_0030BA80_sMan {
    char pad000[0x2C0];
    func_0030BA80_sEntry entries[1];
};

extern "C" int func_0030C468(void* entry);

extern "C" int func_0030BA80(func_0030BA80_sMan* self, int id)
{
    func_0030BA80_sEntry* pool = self->entries;
    int i;
    for (i = 0; i < 1; i++, pool++) {
        if (pool->id == id) {
            return func_0030C468(pool);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030BAC8);
#ifdef SKIP_ASM
struct func_0030BAC8_sEntry {
    int id;
    char pad04[0x10C - 4];
};

struct func_0030BAC8_sMan {
    char pad000[0x2C0];
    func_0030BAC8_sEntry entries[1];
};

extern "C" void func_0030C4A8(void* e, int a1, int a2);

extern "C" void func_0030BAC8(func_0030BAC8_sMan* self, int id, int instIds, int size)
{
    func_0030BAC8_sEntry* pool = self->entries;
    int i;
    for (i = 0; i < 1; i++, pool++) {
        if (pool->id == id) {
            func_0030C4A8(pool, instIds, size);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030BB10);
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

class func_0030C6C8_cObj;
extern "C" int func_0030C6C8(void* a, func_0030C6C8_cObj* obj);
extern "C" void func_0030BC80(void* self, void** list, void* stream);
extern "C" int func_003ACA38(void* a, int id);
extern "C" void func_002257E0(void* obj, void* stream, int a2);

extern "C" void func_0030BB10(sWSManBB10* self, void* stream)
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
            func_0030C6C8(self->tables[k], (func_0030C6C8_cObj*)stream);
        }
    }
    func_0030BC80(self, &self->list2B8, stream);
    func_0030BC80(self, &self->list2BC, stream);
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
            int r;
            vtEntry->fn((char*)stream + vtEntry->delta, &j, 4);
            r = func_003ACA38(*(void**)(self->world + 4), j);
            func_002257E0(self->objs[j], stream, r);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", func_0030BC80);
#ifdef SKIP_ASM
extern "C" int func_0030B228(cWScriptListHead* self);
extern "C" void func_00308038(void* node, void* stream);

struct sVEntry30BC80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0030BC80(void* self, void** list, void* stream)
{
    int n = func_0030B228((cWScriptListHead*)list);
    sVEntry30BC80* vtEntry = &(*(sVEntry30BC80**)stream)[1];
    vtEntry->fn((char*)stream + vtEntry->delta, &n, 4);
    char* node = (char*)*list;
    while (node != 0) {
        int id = *(int*)(node + 0x20);
        vtEntry = &(*(sVEntry30BC80**)stream)[1];
        vtEntry->fn((char*)stream + vtEntry->delta, &id, 4);
        func_00308038(node, stream);
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
int func_0030B260(void* self, void* a1);
extern "C" int func_003ACA38(void* a, int id);
class func_0030C700_cObj;
extern "C" int func_0030C700(void* a, func_0030C700_cObj* obj);
class func_0030BEE8_cStream;
extern "C" void func_0030BEE8(void* self, cWScriptListHead* list, func_0030BEE8_cStream* stream);
extern "C" void func_00224F68(void* obj);
extern "C" void luno_cLunoTable_readFromReplayFrame(void* obj, cStream_30BD20* stream, int a2);

/**
 * Restores the world script manager's state from a replay frame.
 *
 * Reads the first 0x28C bytes of the manager, then the collect pool (func_0030C700). Every process in the active
 * (+0x2B8) and pending (+0x2BC) lists goes back to the idle list, and both lists are then read back from @p stream
 * (func_0030BEE8). The 50 section tables at +0x3CC are emptied (func_00224F68), and a count of saved tables follows,
 * each read back against its section's script base (func_003ACA38). Finally the current instance, infos, process and
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
            func_0030C700(self->tables[k], (func_0030C700_cObj*)stream);
        }
    }
    void* p;
    while ((p = cProcessList_removeHead(&self->list2B8)) != 0) {
        *(int*)p = 0;
        func_0030B260((char*)self + 0x2B4, p);
    }
    while ((p = cProcessList_removeHead(&self->list2BC)) != 0) {
        *(int*)p = 0;
        func_0030B260((char*)self + 0x2B4, p);
    }
    func_0030BEE8(self, (cWScriptListHead*)&self->list2B8, (func_0030BEE8_cStream*)stream);
    func_0030BEE8(self, (cWScriptListHead*)&self->list2BC, (func_0030BEE8_cStream*)stream);
    {
        int i;
        for (i = 0; i < 50; i++) {
            if (self->objs[i] != 0) {
                func_00224F68(self->objs[i]);
            }
        }
    }
    int count;
    stream->Read(&count, 4);
    for (int j = 0; j < count; j++) {
        int id;
        stream->Read(&id, 4);
        int r = func_003ACA38(*(void**)(self->world + 4), id);
        luno_cLunoTable_readFromReplayFrame(self->objs[id], stream, r);
    }
    *(int*)((char*)self + 0x290) = 0;
    *(int*)((char*)self + 0x294) = 0;
    *(int*)((char*)self + 0x298) = 0;
    *(int*)((char*)self + 0x2A4) = 0;
    *(int*)((char*)self + 0x2A8) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptman", func_0030BEE8);
#ifdef SKIP_ASM
class func_0030BEE8_cStream {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void Read(void* dst, int size);
};

extern "C" void func_00308118(void* proc, func_0030BEE8_cStream* stream);
extern "C" void func_0030B270(cWScriptListHead* self, cWScriptListNode* node);
extern "C" int cProcessList_removeProcess(cWScriptListHead* self, cWScriptListNode* node);

extern "C" void func_0030BEE8(void* self, cWScriptListHead* list, func_0030BEE8_cStream* stream)
{
    int n;
    int id;
    int i = 0;
    stream->Read(&n, 4);
    for (; i < n; i++) {
        stream->Read(&id, 4);
        cWScriptListNode* node = (cWScriptListNode*)func_0030B320((char*)self + 0x2B4, id);
        cProcessList_removeProcess((cWScriptListHead*)((char*)self + 0x2B4), node);
        func_00308118(node, stream);
        func_0030B270(list, node);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030BFC0);
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

extern "C" int func_0030BFC0(int* who, unsigned int id0, unsigned int id1, unsigned int id2)
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
INCLUDE_ASM("wscript/wscriptman", func_0030C3E0);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void func_00153B00(void* iface, int a1, int index, int value);

struct func_0030C3E0_sTable {
    int value;      // 0x0
    int pad4;       // 0x4
    int count;      // 0x8
    int ids[1];     // 0xC
};

extern "C" void func_0030C3E0(func_0030C3E0_sTable* self, int id)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->ids[i] == id) {
            func_00153B00(cBE_getInterface_Fv(cBE_getBE(), 10), 0, i, self->value);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C468);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int func_00153498(void* iface, int a1, int id);

extern "C" int func_0030C468(void* entry)
{
    return func_00153498(cBE_getInterface_Fv(cBE_getBE(), 10), 0, *(int*)entry);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C4A8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_C4A8(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" int func_001538E8(void* iface, int a, int b, int c);
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

// PORT: a1 is really an unsigned int* id list and a2 its count (the unit declares both int).
extern "C" void func_0030C4A8(void* ep, int instIds, int size)
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
            if (func_001538E8(iface, 0, pool->count, pool->id))
                respawn_C4A8(*ids);
        } else {
            respawn_C4A8(*ids);
        }
        pool->count++;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C6C8);
#ifdef SKIP_ASM
class func_0030C6C8_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
};

extern "C" int func_0030C6C8(void* a, func_0030C6C8_cObj* obj)
{
    return obj->v01(a, 268);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C700);
#ifdef SKIP_ASM
class func_0030C700_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
    virtual int v02(void* a, int b);
};

extern "C" int func_0030C700(void* a, func_0030C700_cObj* obj)
{
    return obj->v02(a, 268);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", tMissionMenuQueue_tMissionMenuQueue);
#ifdef SKIP_ASM
extern "C" void tMissionMenuQueue_init(void* self);

/**
 * Constructs an empty mission menu queue.
 *
 * Runs tMissionMenuQueue_init.
 *
 * @return this
 */
extern "C" void* tMissionMenuQueue_tMissionMenuQueue(void* self)
{
    tMissionMenuQueue_init(self);
    return self;
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
extern "C" void tMissionMenuQueue_init(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 4) = 0;
    MEM_fill((char*)self + 8, 0, 0xC0);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C790);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

extern "C" void func_0030C790(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 4) = 0;
    MEM_fill((char*)self + 8, 0, 0xC0);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", tMissionMenuQueue_addEntry);
#ifdef SKIP_ASM
// Same layout as cWScriptQueue / cWScriptQueueEntry, which the unit defines
// further down (above func_0030C820). Renamed here only to avoid a redefinition.
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
extern "C" void tMissionMenuQueue_addEntry(cWSQueue* self, int missionId, int cmd, int being)
{
    self->entries[self->tail].a = missionId;
    self->entries[self->tail].b = cmd;
    self->entries[self->tail].c = being;
    self->tail = (self->tail + 1) % 16;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C820);
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

extern "C" cWScriptQueueEntry* func_0030C820(cWScriptQueue* self)
{
    int head = self->head;
    if (head == self->tail) {
        return 0;
    }
    self->head = (head + 1) % 16;
    return &self->entries[head];
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
extern "C" cWScriptQueueEntry* tMissionMenuQueue_getEntry(cWScriptQueue* self)
{
    if (self->head == self->tail) {
        return 0;
    }
    return &self->entries[self->head];
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C898);
#ifdef SKIP_ASM
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
extern "C" void func_0030D238(void *arg0, int arg1, int arg2) {
    (*(int *)((char*)(arg0) + (0))) = arg1;
    (*(int *)((char*)(arg0) + (4))) = arg2;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D248);
#ifdef SKIP_ASM
extern "C" int func_0030D248(int arg0) {
    return arg0;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D250);
#ifdef SKIP_ASM
extern "C" int func_0030D250(int arg0) {
    return arg0;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D258);
#ifdef SKIP_ASM
extern "C" int func_0030D258(int arg0) {
    return arg0;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D260);
#ifdef SKIP_ASM
extern "C" void func_0030D260(void *wscriptMan, int trickDef) {
    (*(int *)((char*)(wscriptMan) + (0x29C))) = trickDef;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D268);
#ifdef SKIP_ASM
extern "C" int func_0030D268(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (4)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D270);
#ifdef SKIP_ASM
extern "C" int func_0030D270(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (8)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D278);
#ifdef SKIP_ASM
extern "C" int func_0030D278(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0xC)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D280);
#ifdef SKIP_ASM
extern "C" int func_0030D280(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D288);
#ifdef SKIP_ASM
extern "C" int func_0030D288(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x14)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D290);
#ifdef SKIP_ASM
extern "C" int func_0030D290(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x18)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D298);
#ifdef SKIP_ASM
extern "C" int func_0030D298(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x1C)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2A0);
#ifdef SKIP_ASM
extern "C" int func_0030D2A0(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x20)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2A8);
#ifdef SKIP_ASM
extern "C" int func_0030D2A8(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x24)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2B0);
#ifdef SKIP_ASM
extern "C" int func_0030D2B0(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x28)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2B8);
#ifdef SKIP_ASM
extern "C" int func_0030D2B8(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x34)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2C0);
#ifdef SKIP_ASM
extern "C" int func_0030D2C0(int wscriptMan) {
    return wscriptMan + 0x38;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2C8);
#ifdef SKIP_ASM
extern "C" void func_0030D2C8(void *wscriptMan, int missionId) {
    (*(int *)((char*)(wscriptMan) + (0x2A0))) = missionId;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2D0);
#ifdef SKIP_ASM
extern "C" int func_0030D2D0(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x2A0)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2D8);
#ifdef SKIP_ASM
extern int cWScriptMan_TheWScriptMan;

extern "C" int func_0030D2D8(void) {
    return cWScriptMan_TheWScriptMan;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2E0);
#ifdef SKIP_ASM
extern "C" int func_0030D2E0(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x290)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2E8);
#ifdef SKIP_ASM
extern "C" int func_0030D2E8(int *arg0) {
    return *arg0;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2F0);
#ifdef SKIP_ASM
extern "C" int func_0030D2F0(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x294)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D2F8);
#ifdef SKIP_ASM
extern "C" int func_0030D2F8(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x298)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D300);
#ifdef SKIP_ASM
extern "C" void func_0030D300(void *wscriptMan, int inst) {
    (*(int *)((char*)(wscriptMan) + (0x290))) = inst;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D308);
#ifdef SKIP_ASM
extern "C" int func_0030D308(int wscriptMan, int sectionId) {
    return (*(int *)(((char*)(wscriptMan) + (sectionId << 2)) + (0x3CC)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D318);
#ifdef SKIP_ASM
extern "C" int func_0030D318(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x2A8)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D320);
#ifdef SKIP_ASM
extern "C" void func_0030D320(void *wscriptMan, int task) {
    (*(int *)((char*)(wscriptMan) + (0x2A8))) = task;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D328);
#ifdef SKIP_ASM
extern "C" int func_0030D328(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x2A4)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D330);
#ifdef SKIP_ASM
extern "C" int func_0030D330(void *wscriptMan) {
    return (*(int *)((char*)(wscriptMan) + (0x1C4))) != (*(int *)((char*)(wscriptMan) + (0x1C8)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D348);
#ifdef SKIP_ASM
extern "C" void func_0030D348(char* self)
{
    func_0030C820((cWScriptQueue*)(self + 0x1C4));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D368);
#ifdef SKIP_ASM
extern "C" void func_0030D368(char* self)
{
    tMissionMenuQueue_getEntry((cWScriptQueue*)(self + 0x1C4));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D388);
#ifdef SKIP_ASM
extern "C" void func_0030D388(int *arg0, int arg1) {
    *arg0 = arg1;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D390);
#ifdef SKIP_ASM
extern "C" int func_0030D390(void* self, unsigned int a1) {
    return a1 < 10;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D398);
#ifdef SKIP_ASM
extern "C" void func_0030D398(int *arg0) {
    *arg0 = 0;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D3A0);
#ifdef SKIP_ASM
extern "C" int func_0030D3A0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x18)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D3A8);
#ifdef SKIP_ASM
extern "C" void func_0030D3A8(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x18))) = arg1;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D3B0);
#ifdef SKIP_ASM
extern "C" int func_0030D3B0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x24)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D3B8);
#ifdef SKIP_ASM
extern "C" void func_0030D3B8(void *arg0, int trickDef) {
    (*(int *)((char*)((*(void **)((char*)(arg0) + (0x10)))) + (0x29C))) = trickDef;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D3C8);
#ifdef SKIP_ASM
extern "C" int func_0030D3C8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x1C)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D3D0);
#ifdef SKIP_ASM
extern "C" int func_0030D3D0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20)));
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
INCLUDE_ASM("wscript/wscriptman", func_0030D3E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cWScriptMan_runScriptCode_a1(void* self) __asm__("cWScriptMan_runScriptCode");

extern "C" void func_0030D3E0(char* self)
{
    cWScriptMan_runScriptCode_a1(*(void**)(self + 0x10));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D400);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0030ADA8_a1(void* self) __asm__("func_0030ADA8");

extern "C" void func_0030D400(char* self)
{
    func_0030ADA8_a1(*(void**)(self + 0x10));
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
extern "C" void cWScriptTask__cWScriptTask(void* self, int flags)
{
    *(void***)((char*)self + 0x40) = cWScriptTask_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D450);
#ifdef SKIP_ASM
extern "C" int func_0030D450(void *arg0) {
    return (*(int *)((char*)((*(void **)((char*)(arg0) + (0x3C)))) + (0x1C)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D460);
#ifdef SKIP_ASM
extern "C" int func_0030D460(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D468);
#ifdef SKIP_ASM
extern "C" void func_0030D468(int *arg0) {
    *arg0 = 3;
}
#endif
//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D478);
#ifdef SKIP_ASM
extern "C" int func_0030D478(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x64)));
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
INCLUDE_ASM("wscript/wscriptman", func_0030D488);
#ifdef SKIP_ASM
extern "C" int func_0030D488(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x60))) == 2;
}
#endif

//99.38%
INCLUDE_ASM("wscript/wscriptman", func_0030D498__FPv);
#ifdef SKIP_ASM
void* func_0030D498(void* self)
{
    return func_0030C8D8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D4B8);
#ifdef SKIP_ASM
struct sWSVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sWSSlot8 {
    int a;
    int b;
};

extern "C" void* func_0030D4B8(void* self)
{
    sWSVec4 ones;
    *(int*)((char*)self + 0x38) = 0;
    *(int*)((char*)self + 0x30) = 0;
    *(int*)((char*)self + 0x34) = 0;
    ones.x = 1.0f;
    ones.y = 1.0f;
    ones.z = 1.0f;
    ones.w = 1.0f;
    *(sWSVec4*)((char*)self + 0x140) = ones;
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x3C) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x12C) = 0;
    *(int*)((char*)self + 0x130) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x20) = 0;
    for (int i = 3; i >= 0; i--) {
        ((sWSSlot8*)((char*)self + 0x44))[i].a = 0;
    }
    return self;
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
INCLUDE_ASM("wscript/wscriptman", func_0030D840);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void MEM_fill(void* dst, int value, int size);
extern char str_ModelRefs[];

struct func_0030D840_sPool {
    int count;      // 0x0
    int field_0x4;  // 0x4
    int field_0x8;  // 0x8
    void* items;    // 0xC
    char pad_0x10[0x60 - 0x10];
    int field_0x60; // 0x60
};

extern "C" void func_0030D840(func_0030D840_sPool* self, int count, int a2)
{
    self->field_0x4 = a2;
    self->count = count;
    self->items = operator_new_tag(count * 88, str_ModelRefs, 0, 0);
    MEM_fill(self->items, 0, self->count * 88);
    self->field_0x8 = 0;
    self->field_0x60 = 0;
}
#endif


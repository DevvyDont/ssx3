#include "common.h"
#include "scripter/ssxscriptengine.h"

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetupBXEngine);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cInSeq_cInSeq(void* obj, void* buf, int size, int a3, void* out);
extern char str_InSeqHeap[];
extern char str_BXScriptDatabase[];
extern char str_InSeq[];

struct sBXSlot {
    int a;
    short b;
};

/**
 * Sets up the BX script engine: allocates the InSeq sequencer with its heap and an empty script database.
 *
 * Allocates @p size bytes of "InSeqHeap" (mpInSeqHeap), creates the 0x20-byte "InSeq" sequencer (mpInSeq)
 * over that heap with this engine's cInSeqComm interface (+0x2AC), and allocates and zeroes @p count 8-byte
 * "BXScriptDatabase" entries (mpScriptDatabase, count in mScriptDatabaseSize).
 *
 * @param count  number of script database entries (On Tour scriptDatabaseSize)
 * @param maxUniqueAnimPlayers  passed to cInSeq_cInSeq() as its script limit (cInSeq's maxScripts)
 * @param size  InSeq heap size in bytes (On Tour heapSize)
 */
extern "C" void cBXScriptEngine_SetupBXEngine(void* self, int count, int maxUniqueAnimPlayers, int size)
{
    *(void**)((char*)self + 0x2BC) = operator_new_tag(size, str_InSeqHeap, 0, 0);
    *(void**)((char*)self + 0x2B8) = cInSeq_cInSeq(cMemMan_alloc(0x20, str_InSeq, 0, 0),
        *(void**)((char*)self + 0x2BC), size, maxUniqueAnimPlayers, (char*)self + 0x2AC);
    *(int*)((char*)self + 0x2B4) = count;
    *(sBXSlot**)((char*)self + 0x2B0) = (sBXSlot*)operator_new_tag(count * 8, str_BXScriptDatabase, 0, 0);
    for (int i = 0; i < *(int*)((char*)self + 0x2B4); i++) {
        (*(sBXSlot**)((char*)self + 0x2B0))[i].a = 0;
        (*(sBXSlot**)((char*)self + 0x2B0))[i].b = 0;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PurgeBXEngine);
#ifdef SKIP_ASM
struct sScriptEngine282390;
struct sScriptEngine2823F0;
struct sScriptEngine2B8;
extern "C" void cBXScriptEngine_StopScriptAll(sScriptEngine282390* self);
extern "C" void cBXScriptEngine_ReleaseRequestAll(sScriptEngine2823F0* self);
extern "C" void cBXScriptEngine_ResetInstVars(sScriptEngine2B8* self);
extern "C" void cScriptDatabaseObject_RemoveObjectAll(void* self);
extern "C" void cInSeq__cInSeq(void* obj, int flags);
void cMemMan_free(void*);

/**
 * Tears down the BX script engine's script database, InSeq sequencer and InSeq heap.
 *
 * Does nothing unless mpScriptDatabase was allocated (cBXScriptEngine_SetupBXEngine). Otherwise it stops every
 * script, releases every script request, removes every database object, frees the database and zeroes its size,
 * destroys mpInSeq (destructor flags 3), frees mpInSeqHeap, and finally calls cBXScriptEngine_ResetInstVars().
 */
extern "C" void cBXScriptEngine_PurgeBXEngine(void* self)
{
    if (*(void**)((char*)self + 0x2B0) != 0) {
        cBXScriptEngine_StopScriptAll((sScriptEngine282390*)self);
        cBXScriptEngine_ReleaseRequestAll((sScriptEngine2823F0*)self);
        cScriptDatabaseObject_RemoveObjectAll(self);
        if (*(void**)((char*)self + 0x2B0) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x2B0));
        }
        *(void**)((char*)self + 0x2B0) = 0;
        *(int*)((char*)self + 0x2B4) = 0;
        if (*(void**)((char*)self + 0x2B8) != 0) {
            cInSeq__cInSeq(*(void**)((char*)self + 0x2B8), 3);
        }
        *(void**)((char*)self + 0x2B8) = 0;
        if (*(void**)((char*)self + 0x2BC) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x2BC));
        }
        *(void**)((char*)self + 0x2BC) = 0;
        cBXScriptEngine_ResetInstVars((sScriptEngine2B8*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_Update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cInSeq_UpdateFrame(void* obj);
extern "C" void func_00282908(void* self, void* script);

struct sScriptSlot_2820B0 {
    int f0;
    int type;           // 0x04
    char pad8[0x18];
    int state;          // 0x20
};

struct sScriptEngine_2820B0 {
    char pad0[0x2B8];
    void* obj2B8;                       // 0x2B8
    char pad2BC[4];
    sScriptSlot_2820B0 slots[16];       // 0x2C0
};

// Retail builds compile the per-state debug hooks to nothing.
static inline void slotNop_2820B0(void* self, void* slot) {}
/**
 * Per-frame update of the script engine.
 *
 * Calls cInSeq_UpdateFrame() on the sequencer at +0x2B8, then for each of the 16 requests in state 1 runs its
 * pending-state handler by request type: type 1 goes to func_00282908, the others to slotNop_2820B0 (On Tour does this
 * in UpdatePendingRequestState).
 */
extern "C" void cBXScriptEngine_Update(sScriptEngine_2820B0* self)
{
    int i;
    cInSeq_UpdateFrame(self->obj2B8);
    for (i = 0; i < 16; i++) {
        sScriptSlot_2820B0* request = &self->slots[i];
        if (request->state == 0) {
            continue;
        }
        if (request->state == 1) {
            switch (request->type) {
            case 0: slotNop_2820B0(self, request); break;
            case 1: func_00282908(self, request); break;
            case 2: slotNop_2820B0(self, request); break;
            case 3: slotNop_2820B0(self, request); break;
            case 4: slotNop_2820B0(self, request); break;
            case 5: slotNop_2820B0(self, request); break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282150);
#ifdef SKIP_ASM
struct sSlot282150 { int value; int pad; };
extern "C" int func_00282150(void* self, int value, int index)
{
    sSlot282150* slots = *(sSlot282150**)((char*)self + 0x2b0);
    if (slots[index].value != 0) {
        return 0;
    }
    slots[index].value = value;
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282178);
#ifdef SKIP_ASM
extern "C" int func_00282178(void* self, int index)
{
    sSlot282150* slots = *(sSlot282150**)((char*)self + 0x2b0);
    int old = slots[index].value;
    if (old == 0) {
        return 0;
    }
    slots[index].value = 0;
    return old;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002821A0);
#ifdef SKIP_ASM
struct sBxsThreadHdr;
extern "C" sBxsThreadHdr* func_00282C38(void* self, int a1);
extern "C" int cInSeq_ReadyPlayer(void* self, void* src);
// PORT: the unit declares cInSeq_GetPlayer(int), but it really takes (self, id).
void* cInSeq_GetPlayer_2(void* self, int id) __asm__("cInSeq_GetPlayer");
// PORT: the setter's mangled signature is (void*, int); a pointer is passed as int.
int func_00274C10(void* self, int a1);
void func_00282C88(void* self, void* v);

struct sBxsVEntry21A0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sBXSlot21A0 {
    void* data;
    unsigned short uses;
};

struct sBXThread21A0 {
    void* owner;    // 0x0
    int state;      // 0x4
    int slot;       // 0x8
    int arg;        // 0xC
    short pad_10;   // 0x10
    short f12;      // 0x12
    int pending;    // 0x14
    float time;     // 0x18
    int result;     // 0x1C
};

extern "C" int func_002821A0(void* self, int idx, int arg)
{
    sBXSlot21A0* s = &(*(sBXSlot21A0**)((char*)self + 0x2B0))[idx];
    sBXThread21A0* t = (sBXThread21A0*)func_00282C38(self, 1);
    if (s->data == 0) {
        int one = 1;
        t->pending = idx;
        t->state = one;
        t->f12 = one;
        sBxsVEntry21A0* e = &(*(sBxsVEntry21A0**)((char*)self + 0x2A8))[7];
        if (e->fn((char*)self + e->delta, idx) == 0) {
            t->state = 7;
            func_00282C88(self, t);
            return -1;
        }
    } else {
        t->slot = idx;
        t->state = 2;
        int id = cInSeq_ReadyPlayer(*(void**)((char*)self + 0x2B8), s->data);
        void* obj = cInSeq_GetPlayer_2(*(void**)((char*)self + 0x2B8), id);
        t->owner = obj;
        func_00274C10(obj, (int)t);
        s->uses++;
    }
    t->arg = arg;
    return t->result;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002822A0);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);
extern "C" void cInSeq_Play(void* cache, void* data, int a2);

extern "C" int func_002822A0(void* self, int id, int value, int loop)
{
    char* request = (char*)func_00282CB0(self, id);
    if (request == 0) {
        return 0;
    }
    int state = *(int*)(request + 0x4);
    if (state != 2) {
        return state;
    }
    *(short*)(request + 0x10) = value;
    if (*(int*)(request + 0x20) == 1) {
        *(int*)(request + 0x4) = 3;
        cInSeq_Play(*(void**)((char*)self + 0x2B8), **(void***)request, loop);
    }
    return *(int*)(request + 0x4);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282338);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);
extern "C" void cInSeq_Stop(void* cache, void* data);

extern "C" void func_00282338(void* self, int requestHandle)
{
    char* request = (char*)func_00282CB0(self, requestHandle);
    char* obj = *(char**)request;
    if (obj != 0 && *(int*)(obj + 0x20) != 0 && *(int*)(request + 0x20) == 1) {
        cInSeq_Stop(*(void**)((char*)self + 0x2B8), *(void**)obj);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_StopScriptAll);
#ifdef SKIP_ASM
extern "C" void func_00282338(void* self, int id);

struct sScriptSlot282390 {
    int active;
    char pad_0x04[0x18];
    int id;
    int pad_0x20;
};

struct sScriptEngine282390 {
    char pad_0x000[0x2C0];
    sScriptSlot282390 slots[16];
};

/**
 * Stops the script of every script request that has a script object.
 *
 * Walks the 16 mScriptRequests (0x24 bytes each) and, for each whose first word (its script object) is set,
 * calls func_00282338() with the request's id (+0x1C), which stops that script in the InSeq sequencer when it
 * is running.
 */
extern "C" void cBXScriptEngine_StopScriptAll(sScriptEngine282390* self)
{
    for (int i = 0; i < 16; i++) {
        sScriptSlot282390* slot = &self->slots[i];
        if (slot->active != 0) {
            func_00282338(self, slot->id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ReleaseRequestAll);
#ifdef SKIP_ASM
extern "C" void func_00282540(void* self, int id);

struct sScriptSlot2823F0 {
    int active;
    char pad_0x04[0x18];
    int id;
    int state;
};

struct sScriptEngine2823F0 {
    char pad_0x000[0x2C0];
    sScriptSlot2823F0 slots[16];
};

/**
 * Releases every script request that is still in use.
 *
 * Walks the 16 mScriptRequests and calls func_00282540() with the id (+0x1C) of each whose word at +0x20 is
 * set; that runs the request type's cleanup, releases its script object and database reference and returns
 * the slot to the free list.
 */
extern "C" void cBXScriptEngine_ReleaseRequestAll(sScriptEngine2823F0* self)
{
    for (int i = 0; i < 16; i++) {
        sScriptSlot2823F0* slot = &self->slots[i];
        if (slot->state != 0) {
            func_00282540(self, slot->id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ForcePlayUpdate);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);
extern "C" void cScriptPlayerMain_UpdateFrame(void* p);

/**
 * Forces an immediate frame update of a playing script request.
 *
 * Finds the request record with func_00282CB0 (On Tour GetRequest). When it exists and its state (+0x4) is 3 or 4 (play
 * pending or playing; cBXScriptEngine_OnInSeqPlay moves 3 to 4), runs cScriptPlayerMain_UpdateFrame (On Tour cScriptPlayerMain::UpdateFrame) on the
 * request's player (+0x0). cSSXScriptEngine_ForcePlayUpdate calls it before updating its camera managers.
 *
 * @param requestHandle  the request handle
 * @return 1 if the player was updated, 0 for an unknown or idle request
 */
extern "C" int cBXScriptEngine_ForcePlayUpdate(void* self, int requestHandle)
{
    void* t = func_00282CB0(self, requestHandle);
    if (t == 0) {
        return 0;
    }
    int type = *(int*)((char*)t + 0x4);
    if (type == 3 || type == 4) {
        cScriptPlayerMain_UpdateFrame(*(void**)t);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282498);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00282CB0_v(void) __asm__("func_00282CB0");
void func_00274808(void* self, float t);

extern "C" int func_00282498(float playRate)
{
    if (playRate < 0.01f) return 0;
    char* request = (char*)func_00282CB0_v();
    if (request == 0) return 0;
    if (*(int*)(request + 4) == 1) {
        *(float*)(request + 0x18) = playRate;
        return 1;
    }
    func_00274808(*(void**)request, playRate);
    return 1;
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282500);
#ifdef SKIP_ASM
extern "C" int func_00282500(void* self, char* obj, int val)
{
    char* request = *(char**)(obj + 0xC);
    int old = *(int*)(request + 0xC);
    *(int*)(request + 0xC) = val;
    return old;
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282510);
#ifdef SKIP_ASM
extern "C" int func_00282510(void* self, int requestHandle, int val)
{
    char* request = (char*)func_00282CB0(self, requestHandle);
    int old = *(int*)(request + 0xC);
    *(int*)(request + 0xC) = val;
    return old;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282540);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00282338(void* self, int id);
int func_00274C10(void* obj, int a);
extern "C" void cInSeq_RecyclePlayer(void* mgr, int handle);
void func_00282C88(void* self, void* slot);

struct sVEnt_282540 {
    short delta;
    short index;
    void* fn;
};
typedef void (*tVFn1_282540)(void*, int);
typedef void (*tVFn2_282540)(void*, int, int);

struct sTblEnt_282540 {
    int f0;                 // 0x0
    unsigned short refs;    // 0x4
    short pad6;
};

struct sScriptSlot_282540 {
    int* obj;               // 0x00
    int type;               // 0x04
    int tblIdx;             // 0x08
    int fC;                 // 0x0C
    short f10;              // 0x10
    short pad12;
    int f14;                // 0x14
    char pad18[8];
    int state;              // 0x20
};

struct sScriptEngine_282540 {
    char pad0[0x2A8];
    sVEnt_282540* vt;       // 0x2A8
    char pad2AC[4];
    sTblEnt_282540* tbl;    // 0x2B0
    char pad2B4[4];
    void* mgr;              // 0x2B8
};

sScriptSlot_282540* func_00282CB0_282540(sScriptEngine_282540* self, int id) __asm__("func_00282CB0");

#define VCALL1(SELF, SLOT, A) ((tVFn1_282540)(SELF)->vt[SLOT].fn)((char*)(SELF) + (SELF)->vt[SLOT].delta, A)
#define VCALL2(SELF, SLOT, A, B) ((tVFn2_282540)(SELF)->vt[SLOT].fn)((char*)(SELF) + (SELF)->vt[SLOT].delta, A, B)

// Finishes a script slot: per-type cleanup, release of its object and
// table reference, then returns the slot to the free list.
// PORT: the unit declares this as void(void*, int); defined under an asm label with its real type
extern "C" int func_00282540_impl(sScriptEngine_282540* self, int id) __asm__("func_00282540");
extern "C" int func_00282540_impl(sScriptEngine_282540* self, int id)
{
    sScriptSlot_282540* s = func_00282CB0_282540(self, id);
    if (s == 0 || s->state == 0) {
        return 6;
    }
    s->f10 = 0;
    switch (s->type) {
    case 1:
        {
            sTblEnt_282540* t = self->tbl + s->f14;
            if (t->f0 == 0) {
                VCALL1(self, 8, s->f14);
            }
        }
        break;
    case 3:
    case 4:
    case 5:
        func_00282338(self, id);
        break;
    case 0:
    case 2:
    case 6:
        break;
    }
    VCALL2(self, 12, id, s->fC);
    if (s->obj != 0) {
        func_00274C10(s->obj, 0);
        cInSeq_RecyclePlayer(self->mgr, *s->obj);
    }
    if (s->tblIdx >= 0) {
        sTblEnt_282540* e = self->tbl + s->tblIdx;
        if (--e->refs == 0) {
            VCALL1(self, 9, s->tblIdx);
        }
    }
    s->type = 7;
    func_00282C88(self, s);
    return 6;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282698);
#ifdef SKIP_ASM
extern "C" void cInSeq_PauseAll(int);

extern "C" void func_00282698(void *engine) {
    cInSeq_PauseAll((*(int *)((char*)(engine) + (0x2B8))));
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002826B8);
#ifdef SKIP_ASM
extern "C" void cInSeq_ResumeAll(int);

extern "C" void func_002826B8(void *engine) {
    cInSeq_ResumeAll((*(int *)((char*)(engine) + (0x2B8))));
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002826D8);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Pause(void* self);

extern "C" void func_002826D8(void* self, int requestHandle)
{
    char* request = (char*)self + (requestHandle * 0x24 + 0x2c0);
    cScriptPlayerMain_Pause(*(void**)request);
    if (*(int*)(*(char**)request + 0x2C) > 0) {
        *(int*)(request + 0x4) = 5;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282720);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Resume(void* self);

extern "C" void func_00282720(void* self, int requestHandle)
{
    char* request = (char*)self + (requestHandle * 0x24 + 0x2c0);
    cScriptPlayerMain_Resume(*(void**)request);
    if (*(int*)(request + 0x4) == 5) {
        char* player = *(char**)request;
        if (*(int*)(player + 0x2C) <= 0) {
            int playState = *(int*)(player + 0x20);
            if (playState == 3 || playState == 4) {
                *(int*)(request + 0x4) = 4;
            } else {
                *(int*)(request + 0x4) = 3;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282790);
#ifdef SKIP_ASM
extern int D_004A34E8;

extern "C" int func_00282790(void) {
    return D_004A34E8;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282798);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);
extern "C" void func_00282908(void* self, void* script);

extern "C" int func_00282798(void* self, int requestHandle)
{
    char* request = (char*)func_00282CB0(self, requestHandle);
    if (request == 0) {
        return 0;
    }
    if (*(int*)(request + 0x4) == 1) {
        func_00282908(self, request);
    }
    return *(int*)(request + 0x4);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002827F8);
#ifdef SKIP_ASM
extern "C" int func_002827F8(void* self, int requestHandle)
{
    char* request = (char*)func_00282CB0(self, requestHandle);
    if (request == 0) return -1;
    if (*(int*)(request + 4) == 1) return *(int*)(request + 0x14);
    return *(int*)(request + 8);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282838);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);
extern "C" void func_00282908(void* self, void* script);

extern "C" int func_00282838(void* self, int requestHandle)
{
    char* request = (char*)func_00282CB0(self, requestHandle);
    if (request == 0) {
        return 0;
    }
    if (*(int*)(request + 0x4) == 1) {
        func_00282908(self, request);
    }
    if (*(int*)(request + 0x4) != 4 && *(int*)(request + 0x4) != 5) {
        return 0;
    }
    return *(int*)(*(char**)request + 0x20) == 4;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ResetInstVars);
#ifdef SKIP_ASM
struct sScriptThread2C0 {
    int a;          // 0x00
    int b;          // 0x04
    int c;          // 0x08
    int d;          // 0x0C
    short e;        // 0x10
    short f;        // 0x12
    int g;          // 0x14
    int h;          // 0x18
    int id;         // 0x1C
    int k;          // 0x20
};
struct sScriptEngine2B8 {
    char pad[0x2b8];
    int count;                      // 0x2B8
    int unk2BC;                     // 0x2BC
    sScriptThread2C0 threads[16];   // 0x2C0
};
/**
 * Resets the BX script engine's instance variables: the InSeq pointers and the 16 script request records.
 *
 * Zeroes mpInSeq and mpInSeqHeap without freeing them (cBXScriptEngine_PurgeBXEngine frees them first). Each
 * 0x24-byte request record gets its index at +0x1C, -1 at +0x14, and 0 in the words at +0x0, +0x4, +0x18 and
 * +0x20 and the halfwords at +0x10 and +0x12; +0x8 and +0xC are left as they were. +0x0 and +0x20 are the flags
 * cBXScriptEngine_StopScriptAll() and cBXScriptEngine_ReleaseRequestAll() test.
 */
extern "C" void cBXScriptEngine_ResetInstVars(sScriptEngine2B8* self)
{
    int i;
    self->count = 0;
    self->unk2BC = 0;
    for (i = 0; i < 16; i++) {
        self->threads[i].id = i;
        self->threads[i].k = 0;
        self->threads[i].a = 0;
        self->threads[i].b = 0;
        self->threads[i].e = 0;
        self->threads[i].f = 0;
        self->threads[i].h = 0;
        self->threads[i].g = -1;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282908);
#ifdef SKIP_ASM
extern "C" int cInSeq_ReadyPlayer(void* self, void* src);
// PORT: the unit declares cInSeq_GetPlayer(int), but it really takes (self, id).
void* cInSeq_GetPlayer_2(void* self, int id) __asm__("cInSeq_GetPlayer");
// PORT: the setter's mangled signature is (void*, int); a pointer is passed as int.
int func_00274C10(void* self, int a1);
void func_00274808(void* self, float t);

struct sBXSlot282908 {
    void* data;
    unsigned short uses;
};

struct sBXThread282908 {
    void* owner;    // 0x0
    int state;      // 0x4
    int slot;       // 0x8
    int pad_c;      // 0xC
    short pad_10;   // 0x10
    short f12;      // 0x12
    int pending;    // 0x14
    float time;     // 0x18
};

extern "C" void func_00282908(void* self, void* script)
{
    sBXThread282908* t = (sBXThread282908*)script;
    int idx = t->pending;
    sBXSlot282908* s = &(*(sBXSlot282908**)((char*)self + 0x2B0))[idx];
    if (s->data == 0) return;
    t->slot = idx;
    t->pending = -1;
    t->state = 2;
    t->f12 = 0;
    int id = cInSeq_ReadyPlayer(*(void**)((char*)self + 0x2B8), s->data);
    void* obj = cInSeq_GetPlayer_2(*(void**)((char*)self + 0x2B8), id);
    t->owner = obj;
    func_00274C10(obj, (int)t);
    if (t->time > 0.0f) {
        func_00274808(t->owner, t->time);
        t->time = 0.0f;
    }
    s->uses++;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqPlay);
#ifdef SKIP_ASM
struct sBxsVEntry10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int, int);
};

/**
 * Sequencer callback for a script player that starts: notifies the owner and marks its request playing.
 *
 * The player's request record is at player+0xC. Calls virtual slot 10 of the engine's vtable at +0x2A8 with the player
 * and the request's +0x8 and +0xC words, then sets the request state (+0x4) to 4 (playing). On Tour asserts the
 * sequencer exists and the request was play-pending (3); retail does not. cSSXScriptEngine_OnInSeqPlay forwards here.
 *
 * @param pPlayer  the cScriptPlayerMain that started
 */
extern "C" void cBXScriptEngine_OnInSeqPlay(void* self, void* pPlayer)
{
    char* t = *(char**)((char*)pPlayer + 0xC);
    sBxsVEntry10* vt = *(sBxsVEntry10**)((char*)self + 0x2A8);
    vt[10].fn((char*)self + vt[10].delta, pPlayer, *(int*)(t + 0x8), *(int*)(t + 0xC));
    *(int*)(t + 0x4) = 4;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqStop);
#ifdef SKIP_ASM
struct sVEntry282A18 {
    short delta;
    short index;
    void (*fn)(void*, void*, int, int);
};

extern "C" void func_00282540(void* self, int id);

/**
 * Sequencer callback for a script player that stops: notifies the owner, marks the request stopped and recycles it if
 * asked.
 *
 * Calls virtual slot 11 of the engine's vtable at +0x2A8 with the player and its request's +0x8 / +0xC words, sets the
 * request state (+0x4) to 2 and, when the request's short at +0x10 is set (probably PlayScript's autoRecycle), releases
 * the request by its handle (+0x1C) with func_00282540 (On Tour ReleaseRequest). cSSXScriptEngine_OnInSeqStop forwards
 * here.
 *
 * @param msg  the cScriptPlayerMain that stopped
 */
extern "C" void cBXScriptEngine_OnInSeqStop(void* self, void* msg)
{
    char* s = *(char**)((char*)msg + 0xC);
    sVEntry282A18* vt = *(sVEntry282A18**)((char*)self + 0x2A8);
    vt[11].fn((char*)self + vt[11].delta, msg, *(int*)(s + 0x8), *(int*)(s + 0xC));
    *(int*)(s + 0x4) = 2;
    if (*(short*)(s + 0x10) != 0) {
        func_00282540(self, *(int*)(s + 0x1C));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282A80);
#ifdef SKIP_ASM
extern "C" int func_0027D578(void* self, int key, void** out, int max);

class cBxsHandler2A80 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05(void* ctx, int arg);
};

extern "C" cBxsHandler2A80* func_00282A80(void* self, void* ctx, int arg)
{
    cBxsHandler2A80* buf[30];
    cBxsHandler2A80* found = 0;
    int n = func_0027D578(self, **(int**)((char*)ctx + 0x8), (void**)buf, 30);
    for (int i = 0; i < n; i++) {
        cBxsHandler2A80* h = buf[i];
        if (h->v05(ctx, arg)) {
            found = h;
            break;
        }
    }
    return found;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282B40);
#ifdef SKIP_ASM
struct sBxsThreadHdr {
    void* owner;    // 0x0
    int state;      // 0x4
    int handle;     // 0x8
};

extern "C" sBxsThreadHdr* func_00282C38(void* self, int a1);
// PORT: the setter's mangled signature is (void*, int); a pointer is passed as int.
int func_00274C10(void* self, int a1);

extern "C" void func_00282B40(void* self, void* obj)
{
    sBxsThreadHdr* request = func_00282C38(self, 1);
    request->state = 2;
    request->handle = -1;
    request->owner = obj;
    func_00274C10(obj, (int)request);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282B88);
#ifdef SKIP_ASM
void func_00282C88(void* self, void* v);

extern "C" void func_00282B88(void* self, void* ctx)
{
    void* request = *(void**)((char*)ctx + 0xC);
    *(int*)((char*)request + 0x4) = 7;
    func_00282C88(self, request);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BB0__FPvT0);
#ifdef SKIP_ASM
int func_00282BB0(void* self, void* player)
{
    return *(int*)((char*)*(void**)((char*)player + 0xc) + 0xc);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BC0);
#ifdef SKIP_ASM
extern "C" int func_00282BC0(void* self, int requestHandle)
{
    return *(int*)((char*)func_00282CB0(self, requestHandle) + 0xC);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BE0__FPvT0);
#ifdef SKIP_ASM
int func_00282BE0(void* self, void* player)
{
    return *(int*)((char*)*(void**)((char*)player + 0xc) + 0x1c);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestPlayerByReqHandle);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);

/**
 * Returns the script player of a request handle.
 *
 * func_00282CB0 (On Tour GetRequest) finds the request record; its first word is the cScriptPlayerMain. The tNISList
 * code and cSSXScriptEngine_ForcePlayUpdate use it to reach a request's script header.
 *
 * @param requestHandle  the request handle
 * @return the request's player (an int in this view), or 0 for an unknown handle
 */
extern "C" int cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int requestHandle)
{
    int* p = (int*)func_00282CB0(self, requestHandle);
    if (p == 0) {
        return 0;
    }
    return *p;
}
#endif

extern "C" void* cInSeq_GetPlayer(int);

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestPlayerByPlayerID__FPv);
#ifdef SKIP_ASM
/**
 * Returns the sequencer's script player for a player ID.
 *
 * Calls cInSeq_GetPlayer on the engine's sequencer (+0x2B8, On Tour mpInSeq). The C declares only the engine: the
 * player ID arrives in $a1 and passes through to cInSeq_GetPlayer untouched (cScriptObjCameraManager_OnStartObjectUse binds a two-argument alias
 * to call it).
 *
 * @return the cScriptPlayerMain cInSeq_GetPlayer returns
 */
void* cBXScriptEngine_GetRequestPlayerByPlayerID(void* self)
{
    return cInSeq_GetPlayer(*(int*)((char*)self + 0x2b8));
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282C38);
#ifdef SKIP_ASM
struct sBxsThreadSlot2C0 {
    sBxsThreadHdr hdr; // 0x00
    int unk0C;         // 0x0C
    short unk10;       // 0x10
    short unk12;       // 0x12
    int unk14;         // 0x14
    int unk18;         // 0x18
    int id;            // 0x1C
    int type;          // 0x20
};

struct sBxsEngineThreads {
    char pad[0x2C0];
    sBxsThreadSlot2C0 threads[16]; // 0x2C0
};

extern "C" sBxsThreadHdr* func_00282C38(void* self, int requestType)
{
    sBxsEngineThreads* engine = (sBxsEngineThreads*)self;
    int i = 0;
    do {
        if (engine->threads[i].type == 0) {
            engine->threads[i].type = requestType;
            switch (requestType) {
            case 1:
                engine->threads[i].hdr.handle = -1;
                break;
            }
            return &engine->threads[i].hdr;
        }
        i++;
    } while (i < 16);
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282C88__FPvT0);
#ifdef SKIP_ASM
void func_00282C88(void* self, void* request)
{
    *(int*)((char*)request + 0x0) = 0;
    *(int*)((char*)request + 0x4) = 0;
    *(int*)((char*)request + 0x20) = 0;
    *(short*)((char*)request + 0x10) = 0;
    *(short*)((char*)request + 0x12) = 0;
    *(int*)((char*)request + 0x14) = -1;
    *(int*)((char*)request + 0x18) = 0;
    *(int*)((char*)request + 0xc) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282CB0);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int requestHandle)
{
    char* request = (char*)self + (requestHandle * 0x24 + 0x2c0);
    return *(int*)(request + 0x20) != 0 ? request : 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282CD0);
#ifdef SKIP_ASM
void* cScriptObject_cScriptObject(void* self, int type);
extern void* cScriptObjTransition_vtbl[];

extern "C" void* func_00282CD0(void* self)
{
    cScriptObject_cScriptObject(self, 2);
    *(void***)((char*)self + 0xC) = cScriptObjTransition_vtbl;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0x14) = -1;
    *(int*)((char*)self + 0x18) = 9;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x28) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282D30);
#ifdef SKIP_ASM
// PORT: the callee also forwards four float args ($f12-$f15); bound to its (void*, int, int) symbol.
void* func_00282DB0_f4(void* self, int a1, int a2, float f0, float f1, float f2, float f3) __asm__("func_00282DB0__FPvii");

extern "C" void func_00282D30(void* self, float dt)
{
    int id = *(int*)((char*)self + 0x14);
    if (id >= 0) {
        float currTime = *(float*)((char*)self + 0x2C) + dt * 0.01666666753590107f;
        *(float*)((char*)self + 0x2C) = currTime;
        if (*(float*)((char*)self + 0x1C) + *(float*)((char*)self + 0x20) + *(float*)((char*)self + 0x24) - 0.009999999776482582f <= currTime) {
            func_00282DB0_f4(self, id, 9, 0.0f, 0.0f, 0.0f, 0.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282DA8__FPvi);
#ifdef SKIP_ASM
void func_00282DA8(void* self, int val)
{
    *(int*)((char*)self + 0x10) = val;
}
#endif

extern "C" void* func_00282DD0(void*, int, int, int);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282DB0__FPvii);
#ifdef SKIP_ASM
void* func_00282DB0(void* self, int playerID, int transition)
{
    return func_00282DD0(self, playerID, transition, *(int*)((char*)self + 0x28));
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282DD0);
#ifdef SKIP_ASM
struct sBxsVEntry2DD0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sBxsFade2DD0 {
    char pad_0x0[0xC];
    sBxsVEntry2DD0* vtable;     // 0xC
    char pad_0x10[0x4];
    int id;                     // 0x14
    int kind;                   // 0x18
    float rateA;                // 0x1C
    float rateB;                // 0x20
    float rateC;                // 0x24
    int arg;                    // 0x28
    int time;                   // 0x2C
};

// PORT: the unit declares func_00282DD0(void*, int, int, int) returning void*; the body also takes four floats and returns nothing.
extern "C" void func_00282DD0_impl(sBxsFade2DD0* self, int id, int kind, int arg, float t, float c, float a, float b) __asm__("func_00282DD0");
extern "C" void func_00282DD0_impl(sBxsFade2DD0* self, int id, int kind, int transitionColour, float currFrame, float outFrames, float inFrames, float holdFrames)
{
    if (self->kind != 9) {
        sBxsVEntry2DD0* vt = self->vtable;
        vt[5].fn((char*)self + vt[5].delta, self->id);
    }
    if (kind == 9 || inFrames + holdFrames + outFrames <= currFrame) {
        self->kind = 9;
        self->rateA = 0.0f;
        self->rateB = 0.0f;
        self->id = -1;
        self->rateC = 0.0f;
        self->time = 0;
    } else {
        self->kind = kind;
        self->arg = transitionColour;
        self->time = 0;
        self->id = id;
        self->rateA = inFrames * 0.01666666753590107f;
        self->rateB = holdFrames * 0.01666666753590107f;
        self->rateC = outFrames * 0.01666666753590107f;
        sBxsVEntry2DD0* vt = self->vtable;
        vt[4].fn((char*)self + vt[4].delta, self->id);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282EF0);
#ifdef SKIP_ASM
extern "C" int func_00282EF0(void* self, int playerID)
{
    return *(int*)((char*)self + 0x14) == playerID;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_PauseObjScript);
#ifdef SKIP_ASM
class cBxScriptVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    virtual void v01();
    virtual void v02();
    virtual void v03();
};

/**
 * Pauses the transition object's script: calls the second virtual of the object at +0x10 (On Tour mpCameraManager,
 * a cScriptEntity; presumably PauseEntityScript).
 */
extern "C" void cScriptObjTransition_PauseObjScript(void* self)
{
    (*(cBxScriptVirt**)((char*)self + 0x10))->v02();
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_ResumeObjScript);
#ifdef SKIP_ASM
/**
 * Resumes the transition object's script: calls the third virtual of the object at +0x10 (On Tour mpCameraManager,
 * a cScriptEntity; presumably ResumeEntityScript).
 */
extern "C" void cScriptObjTransition_ResumeObjScript(void* self)
{
    (*(cBxScriptVirt**)((char*)self + 0x10))->v03();
}
#endif

extern void* cScriptEntity_vtbl[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_cScriptEntity__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a script entity: installs cScriptEntity's vtable and initialises its two fields.
 *
 * The word at +0x4 is set to -1 and the word at +0x8 to 0.
 *
 * @return @p self
 */
void* cScriptEntity_cScriptEntity(void* self)
{
    *(void***)self = cScriptEntity_vtbl;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F80__FPv);
#ifdef SKIP_ASM
int func_00282F80(void* self)
{
    return (*(int*)((char*)self + 0x8) != 0);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F90);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_SetEntityComm(void* p, int a1);

extern "C" void func_00282F90(void* self)
{
    void* entity = *(void**)((char*)self + 0x8);
    if (entity != 0) {
        cScriptPlayerEntity_SetEntityComm(entity, 0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282FB8);
#ifdef SKIP_ASM
extern "C" void* func_0027D2E8(void);
extern "C" void func_0027D400(void* mgr, void* self, int a2);

extern "C" void func_00282FB8(void* self, void* entity)
{
    *(void**)((char*)self + 0x8) = entity;
    func_0027D400(func_0027D2E8(), self, **(int**)((char*)entity + 0x8));
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283000);
#ifdef SKIP_ASM
extern "C" void* func_0027D2E8(void);
extern "C" void func_0027D4A0(void* mgr, void* obj);

extern "C" void func_00283000(void* self)
{
    func_0027D4A0(func_0027D2E8(), self);
    *(int*)((char*)self + 0x8) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283038);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Pause(void* self);

extern "C" void func_00283038(void* self)
{
    void* entity = *(void**)((char*)self + 0x8);
    if (entity != 0) {
        cScriptPlayerMain_Pause(*(void**)((char*)entity + 0x8));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283060);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Resume(void* self);

extern "C" void func_00283060(void* self)
{
    void* entity = *(void**)((char*)self + 0x8);
    if (entity != 0) {
        cScriptPlayerMain_Resume(*(void**)((char*)entity + 0x8));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", func_00283088);
#ifdef SKIP_ASM
extern void* D_004823E0[];
void* cScriptEntity_cScriptEntity(void* self);

extern "C" void* func_00283088(void* self)
{
    cScriptEntity_cScriptEntity(self);
    *(void***)self = D_004823E0;
    *(int*)((char*)self + 0xC) = 9;
    *(int*)((char*)self + 0x10) = 0;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", func_002830C8);
#ifdef SKIP_ASM
extern "C" void func_00282FB8(void* self, void* a1);
int func_002743C8(void* self);
char* func_0027C098(void* self);
char* func_0027C2A8(void* self);
char* func_0027C4B8(void* self);
char* func_0027C6A0(void* self);

extern "C" void func_002830C8(void* self, void* obj)
{
    func_00282FB8(self, obj);
    switch (func_002743C8(obj)) {
    case 0:
        *(int*)((char*)self + 0xC) = *(signed char*)(func_0027C098(obj) + 0x21);
        break;
    case 1:
        *(int*)((char*)self + 0xC) = *(signed char*)(func_0027C2A8(obj) + 0x21);
        break;
    case 2:
        *(int*)((char*)self + 0xC) = *(signed char*)(func_0027C4B8(obj) + 0x21);
        break;
    case 3:
        *(int*)((char*)self + 0xC) = *(signed char*)(func_0027C6A0(obj) + 0x1C);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", func_00283180);
#ifdef SKIP_ASM
extern "C" void func_00283000(void* self);

extern "C" void func_00283180(void* self)
{
    func_00283000(self);
    *(int*)((char*)self + 0xC) = 9;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002831B0);
#ifdef SKIP_ASM
// PORT: func_0027A668 is declared (void*) but takes (self, idx).
void* func_0027A668_2(void* self, int idx) __asm__("func_0027A668__FPv");

extern "C" int func_002831B0(void* self, void* entity, int n)
{
    if (n >= 4) {
        return 0;
    }
    return *(int*)((char*)func_0027A668_2(ScriptEngine, *(int*)((char*)entity + 0x8)) + 0xC) ==
           *(int*)(*(char**)((char*)self + 0x10) + 0xF8);
}
#endif

extern void* cScriptObject_vtbl[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_cScriptObject__FPvi);
#ifdef SKIP_ASM
/**
 * Constructs a script object of a given type that is not yet registered in the script database.
 *
 * Stores cScriptObject's vtable at +0xC, @p objType at +0x0, -1 (no database handle) at +0x4 and 0 at +0x8.
 *
 * @param objType  the object's script type
 * @return @p self
 */
void* cScriptObject_cScriptObject(void* self, int objType)
{
    *(void***)((char*)self + 0xc) = cScriptObject_vtbl;
    *(int*)self = objType;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject__cScriptObject);
#ifdef SKIP_ASM
extern "C" void cScriptObject_DeInitScriptObject(void* self);
void operator_delete(int* ptr);

/**
 * Destroys a script object, first removing it from the script database if it is still registered.
 *
 * Restores cScriptObject's vtable (at +0xC). When the database handle at +0x4 is neither -1 nor 0 it calls
 * cScriptObject_DeInitScriptObject() and sets the handle to -1.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptObject__cScriptObject(void* self, int flags)
{
    *(void***)((char*)self + 0xC) = cScriptObject_vtbl;
    if (*(int*)((char*)self + 0x4) != -1 && *(int*)((char*)self + 0x4) != 0) {
        cScriptObject_DeInitScriptObject(self);
        *(int*)((char*)self + 0x4) = -1;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_InitScriptObject);
#ifdef SKIP_ASM
struct sBxsVEntryA {
    short delta;
    short index;
    int (*fn)(void*, void*);
};

/**
 * Registers a script object with the script database and stores the handle it gets.
 *
 * Calls virtual slot 2 of the database object returned by func_0027D2E8 (On Tour
 * cScriptDatabaseObject::GetDatabaseObject) through its vtable at +0x2A8, passing the object, and stores the result in
 * the object's database handle (+0x4, On Tour mObjID); cScriptObject_DeInitScriptObject undoes it with slot 3. On Tour
 * asserts the handle is >= 0 afterwards; retail does not. cSSXScriptEngine_Load registers its script objects with it,
 * and cRider_cRider the rider's.
 */
extern "C" void cScriptObject_InitScriptObject(void* self)
{
    char* mgr = (char*)func_0027D2E8();
    sBxsVEntryA* vt = *(sBxsVEntryA**)(mgr + 0x2A8);
    *(int*)((char*)self + 0x4) = vt[2].fn(mgr + vt[2].delta, self);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_DeInitScriptObject);
#ifdef SKIP_ASM
struct sBxsVEntryB {
    short delta;
    short index;
    int (*fn)(void*, int);
};

/**
 * Removes the script object from the script engine's object database and marks it unregistered.
 *
 * Calls virtual slot 3 of the global script engine (func_0027D2E8()) through its cScriptDatabaseObject vtable at
 * +0x2A8, passing the object's database handle (+0x4); cScriptDatabaseObject_RemoveObjectAll() uses the same
 * slot to remove each entry. The handle is then set to -1.
 */
extern "C" void cScriptObject_DeInitScriptObject(void* self)
{
    char* mgr = (char*)func_0027D2E8();
    sBxsVEntryB* vt = *(sBxsVEntryB**)(mgr + 0x2A8);
    vt[3].fn(mgr + vt[3].delta, *(int*)((char*)self + 0x4));
    *(int*)((char*)self + 0x4) = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_PauseObjScript);
#ifdef SKIP_ASM
class cScriptOwner3320 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int v06();
    virtual void* v07(int i);
};

/**
 * Pauses the scripts of every script entity the object owns.
 *
 * Loops over the object's virtual GetNumScriptEntities() (slot 6) and calls func_00283038 (On Tour
 * cScriptEntity::PauseEntityScript) on each virtual GetScriptEntity(i) (slot 7). It is the base implementation that
 * classes without their own PauseObjScript inherit (cScriptObjRider's vtable D_00482018). On Tour asserts each entity
 * is non-null; retail does not.
 */
extern "C" void cScriptObject_PauseObjScript(cScriptOwner3320* self)
{
    int i = 0;
    int n = self->v06();
    for (; i < n; i++) {
        func_00283038(self->v07(i));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_ResumeObjScript);
#ifdef SKIP_ASM
class cScriptOwner33A8 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int v06();
    virtual void* v07(int i);
};

/**
 * Resumes the scripts of every script entity the object owns.
 *
 * Loops over the object's virtual GetNumScriptEntities() (slot 6) and calls func_00283060 (On Tour
 * cScriptEntity::ResumeEntityScript) on each virtual GetScriptEntity(i) (slot 7); the base implementation that
 * cScriptObjRider (vtable D_00482018) inherits. The counterpart of cScriptObject_PauseObjScript.
 */
extern "C" void cScriptObject_ResumeObjScript(cScriptOwner33A8* self)
{
    int i = 0;
    int n = self->v06();
    for (; i < n; i++) {
        func_00283060(self->v07(i));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_OnStartObjectUse__FPv);
#ifdef SKIP_ASM
/**
 * Marks a script object as taken by one more user: increments its use count.
 *
 * Adds 1 to the in-use count at +0x8 (On Tour mInUse). The player ID argument is ignored, so the C declares only the
 * object. The subclass overrides (cScriptObjCamera_OnStartObjectUse, cScriptObjRider_OnStartObjectUse, cScriptObjEvent_OnStartObjectUse, cScriptObjCameraManager_OnStartObjectUse) call
 * it on their cScriptObject part.
 *
 * @return the new use count (left in $v0 by the code; On Tour's method is void)
 */
void* cScriptObject_OnStartObjectUse(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x8) + 0x1;
    *(int*)((char*)self + 0x8) = (int)t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283440__FPv);
#ifdef SKIP_ASM
void* func_00283440(void* self)
{
    void* useCount = (char*)*(void**)((char*)self + 0x8) - 0x1;
    *(int*)((char*)self + 0x8) = (int)useCount;
    return useCount;
}
#endif


#include "common.h"
#include "ui/uithing.h"

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_pushExplicit);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void cList_addToEnd(cList*, cListNode*);

class cUIStateStack_pushExplicit_cState {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

/**
 * Adds an explicit state: enables it, notifies it and appends it to the stack's second list.
 *
 * Sets mEnabled and mExplicit (0x60) in the state's mAttrState, calls its virtual slot 4 (empty in cUIState;
 * the slot before onDestroyScreen) and adds it to the end of the cList at +0x1C, not the main stack at +0x0.
 *
 * @param state  the state to add (On Tour pPushState)
 */
extern "C" void cUIStateStack_pushExplicit(void* self, cUIStateStack_pushExplicit_cState* state)
{
    *(int*)((char*)state + 0x1C) |= 0x60;
    state->v04();
    cList_addToEnd((cList*)((char*)self + 0x1C), (cListNode*)state);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_pushPopup);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cList;
void* cList_first(cList*);
struct cListNode;
void cList_addToFront(cList*, cListNode*);

struct cUIStateStack_pushPopup_sState {
    char pad0[0x1C];
    unsigned int b0 : 1;
    unsigned int level : 2;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int rest : 26;
    cUIStateStack_pushPopup_sState* parent;   // 0x20
};

// A state gets covered by a newly pushed one: it is no longer on top and sits one level deeper.
static inline void cUIStateStack_pushPopup_cover(cUIStateStack_pushPopup_sState* st)
{
    st->b5 = 0;
    st->level = st->level + 1;
}

/**
 * Pushes a popup state onto the front of the state stack, covering the state beneath it.
 *
 * The covered state is the popup's parent when it has one, else the current front of the stack: it loses its
 * enabled bit and its popup level goes up by one. The popup gets its is-popup and enabled bits set.
 *
 * @param st  the popup state to push (On Tour pPushState)
 */
// Pushes a state, covering its parent if it has one, else the current top of the stack.
extern "C" void cUIStateStack_pushPopup(cList* self, cUIStateStack_pushPopup_sState* st)
{
    cUIStateStack_pushPopup_sState* top = st->parent;
    if (top != 0) {
        cUIStateStack_pushPopup_cover(top);
    } else {
        top = (cUIStateStack_pushPopup_sState*)cList_first(self);
        if (top != 0) {
            cUIStateStack_pushPopup_cover(top);
        }
    }
    st->b0 = 1;
    st->b5 = 1;
    cList_addToFront(self, (cListNode*)st);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_pushSpecial);
#ifdef SKIP_ASM
struct cListNode;
void cList_addToFront(cList*, cListNode*);

struct cUIStateStack_sState {
    char pad0[0x1C];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

static inline int cUIStateStack_stateId(cUIStateStack_sState* s) { return s->id; }

extern "C" void* cUIStateStack_getCurrentState(void* self);

/**
 * Pushes a state onto the front of the stack, choosing whether it starts enabled and whether the current one
 * dies.
 *
 * If the current (first enabled) state is in mode 5 it moves to mode 7 with mPendingDelete = @p
 * bDestroyCurrent; other modes are left alone. With no current state the new one is forced enabled and set to
 * mode 2. The pushed state's mEnabled becomes @p bEnable.
 *
 * @param st               the state to push (On Tour pPushState)
 * @param bEnable          whether the pushed state starts enabled
 * @param bDestroyCurrent  stored as the current state's mPendingDelete when it is in mode 5
 */
extern "C" void cUIStateStack_pushSpecial(cList* self, cUIStateStack_sState* st, int bEnable, int bDestroyCurrent)
{
    cUIStateStack_sState* cur = (cUIStateStack_sState*)cUIStateStack_getCurrentState(self);
    if (cur != 0) {
        if (((*(int*)((char*)cur + 0x1C) >> 5) & 1) && cUIStateStack_stateId(cur) == 5) {
            cur->id = 7;
            cur->b7 = bDestroyCurrent;
        }
    } else {
        bEnable = 1;
        st->id = 2;
    }
    cList_addToFront(self, (cListNode*)st);
    st->b5 = bEnable;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_push);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);
struct cListNode;
void cList_addToFront(cList*, cListNode*);

struct cUIStateStack_push_sState {
    char pad0[0x1C];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

static inline int cUIStateStack_push_stateId(cUIStateStack_push_sState* s) { return s->id; }

/**
 * Pushes a state onto the front of the stack, retiring the state on top.
 *
 * When the top state is enabled, not explicit and in a mode below 6, it is set to mode 6 with mPendingDelete,
 * and the new state starts disabled in mode 1, probably until the old one has left; otherwise the new state
 * starts enabled in mode 2.
 *
 * @param st  the state to push (On Tour pPushState)
 */
extern "C" void cUIStateStack_push(cList* self, cUIStateStack_push_sState* st)
{
    int covered = 0;
    cUIStateStack_push_sState* top = (cUIStateStack_push_sState*)cList_first(self);
    if (top != 0) {
        int attrBits = *(int*)((char*)top + 0x1C);
        if (((attrBits >> 5) & 1) && ((attrBits >> 6) & 1) == 0 && cUIStateStack_push_stateId(top) < 6) {
            covered = 1;
            top->id = 6;
            top->b7 = 1;
        }
    }
    if (covered) {
        st->id = 1;
        st->b5 = 0;
    } else {
        st->id = 2;
        st->b5 = 1;
    }
    cList_addToFront(self, (cListNode*)st);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_pushCheck);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);
struct cListNode;
void cList_addToFront(cList*, cListNode*);

struct cUIStateStack_pushCheck_sState {
    char pad0[0x4];
    cUIStateStack_pushCheck_sState* next;     // 0x4
    char pad8[0x1C - 0x8];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
    unsigned int b14 : 1;
};

static inline void setB7_F4C0(cUIStateStack_pushCheck_sState* s, int v) { s->b7 = v; }
static inline void setB14_F4C0(cUIStateStack_pushCheck_sState* s, int v) { s->b14 = v; }
static inline int cUIStateStack_pushCheck_stateId(cUIStateStack_pushCheck_sState* s) { return s->id; }

// Release-build assert: expands to an empty do { } while (0); the leftover loop notes shape register
// allocation of the pinned-state loop the way the target has it.
#define ASSERT_F4C0(cond) do { } while (0)

// Pushes a state, first marking every pinned state on top as covered (id 7) and suspended.
extern "C" void cUIStateStack_pushCheck(cList* self, cUIStateStack_pushCheck_sState* st)
{
    int covered = 0;
    int skipped = 0;
    cUIStateStack_pushCheck_sState* top = (cUIStateStack_pushCheck_sState*)cList_first(self);
    if (top != 0) {
        while (top->b0) {
            ASSERT_F4C0(top != 0);
            if (top->id < 6) {
                top->id = 7;
                ASSERT_F4C0(top->id == 7);
                setB7_F4C0(top, 1);
            }
            setB14_F4C0(top, 1);
            top = top->next;
            skipped = 1;
        }
        int f = *(int*)((char*)top + 0x1C);
        if (skipped || ((f >> 5) & 1)) {
            if (((f >> 6) & 1) == 0 && cUIStateStack_pushCheck_stateId(top) < 6) {
                covered = 1;
                top->id = 6;
                top->b7 = 1;
            }
        }
    }
    if (covered) {
        st->id = 1;
        st->b5 = 0;
    } else {
        st->id = 2;
        st->b5 = 1;
    }
    cList_addToFront(self, (cListNode*)st);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039F600);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);

struct func_0039F600_node {
    char pad0[0x4];
    func_0039F600_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int b0 : 1;
    unsigned int b1 : 5;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
    unsigned int b14 : 1;
    unsigned int b15 : 1;
};

static inline void func_0039F600_setId(func_0039F600_node* n, int v) { n->id = v; }
static inline void func_0039F600_setB7(func_0039F600_node* n, int v) { n->b7 = v; }

extern "C" func_0039F600_node* func_0039F600(cList* list)
{
    func_0039F600_node* n = (func_0039F600_node*)cList_first(list);
    if (n != 0) {
        while (n->b0) {
            if ((*(int*)((char*)n + 0x1C) >> 15) & 1) {
                break;
            }
            if (n->id < 6) {
                func_0039F600_setId(n, 7);
                func_0039F600_setB7(n, 1);
            }
            n = n->next;
        }
        return n;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039F698);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);

struct func_0039F698_node {
    char pad0[0x4];
    func_0039F698_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int b0 : 1;
    unsigned int b1 : 5;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

static inline void func_0039F698_setId(func_0039F698_node* n, int v) { n->id = v; }
static inline void func_0039F698_setB7(func_0039F698_node* n, int v) { n->b7 = v; }

extern "C" func_0039F698_node* func_0039F698(cList* list)
{
    func_0039F698_node* n = (func_0039F698_node*)cList_first(list);
    if (n != 0) {
        if (n->b0) {
            do {
                if (n->id < 6) {
                    func_0039F698_setId(n, 7);
                    func_0039F698_setB7(n, 1);
                }
                n = n->next;
            } while (n->b0);
        }
        return n;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039F718);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);

struct func_0039F718_node {
    char pad0[0x4];
    func_0039F718_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int b0 : 1;
    unsigned int b1 : 5;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

static inline void func_0039F718_setId(func_0039F718_node* n, int v) { n->id = v; }
static inline void func_0039F718_setB7(func_0039F718_node* n, int v) { n->b7 = v; }

extern "C" func_0039F718_node* func_0039F718(cList* list)
{
    func_0039F718_node* n = (func_0039F718_node*)cList_first(list);
    if (n != 0) {
        if (n->b0) {
            do {
                if (n->id < 6) {
                    func_0039F718_setId(n, 7);
                    func_0039F718_setB7(n, 1);
                }
                n = n->next;
            } while (n->b0);
        }
        if (n->id < 6) {
            n->id = 7;
            n->b7 = 1;
        }
        return n->next;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039F7B8);
#ifdef SKIP_ASM
struct sUIState39F7B8 {
    char pad0[0x4];
    sUIState39F7B8* next;       // 0x04
    char pad8[0x14];
    unsigned int b0 : 3;        // 0x1C
    unsigned int active : 1;
    unsigned int b4 : 4;
    unsigned int type : 6;
    unsigned int b14 : 18;
    int child;                  // 0x20
};

static inline void Set39F7B8(sUIState39F7B8* s, int on)
{
    if (s->type < 6) {
        s->active = on;
    }
}

// Sets the active flag on every state in a chain (and the last one if requested), skipping types >= 6.
extern "C" void func_0039F7B8(void* self, sUIState39F7B8* s, int on, int last)
{
    if (s == 0) {
        return;
    }
    sUIState39F7B8* cur = s;
    while (cur->child != 0) {
        Set39F7B8(cur, on);
        cur = cur->next;
    }
    s = cur;
    if (last != 0) {
        Set39F7B8(s, on);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_purgeNormalStates);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct cUIStateStack_purgeNormalStates_node {
    char pad0[0x4];
    cUIStateStack_purgeNormalStates_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int b0 : 1;
    unsigned int b1 : 4;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

static inline void cUIStateStack_purgeNormalStates_setId(cUIStateStack_purgeNormalStates_node* n, int v) { n->id = v; }
static inline void cUIStateStack_purgeNormalStates_setB7(cUIStateStack_purgeNormalStates_node* n, int v) { n->b7 = v; }
static inline void cUIStateStack_purgeNormalStates_setB5(cUIStateStack_purgeNormalStates_node* n, int v) { n->b5 = v; }

extern "C" void cUIStateStack_purgeNormalStates(cList* list)
{
    cUIStateStack_purgeNormalStates_node* n = (cUIStateStack_purgeNormalStates_node*)cList_first(list);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            cUIStateStack_purgeNormalStates_node* next = n->next;
            cUIStateStack_purgeNormalStates_setId(n, 7);
            cUIStateStack_purgeNormalStates_setB7(n, 1);
            cUIStateStack_purgeNormalStates_setB5(n, 1);
            n = next;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039F8C8);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct sUIStVtF8C8 { short delta; short index; void (*fn)(void*, int); };
struct sUIStVtF8C8a { short delta; short index; void (*fn)(void*); };

static inline void func_0039F8C8_purge(cList* list)
{
    char* n = (char*)cList_first(list);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            char* next = *(char**)(n + 4);
            if (((*(int*)(n + 0x1C) >> 6) & 1) == 0) {
                sUIStVtF8C8a* vt = *(sUIStVtF8C8a**)(n + 8);
                vt[5].fn(n + vt[5].delta);
                if (n != 0) {
                    sUIStVtF8C8* vt2 = *(sUIStVtF8C8**)(n + 8);
                    vt2[1].fn(n + vt2[1].delta, 3);
                }
            }
            n = next;
        }
    }
}

extern "C" void func_0039F8C8(cList* self)
{
    func_0039F8C8_purge(self);
    func_0039F8C8_purge((cList*)((char*)self + 0x1C));
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_findState);
#ifdef SKIP_ASM
struct cListNode;
int cListNode_isSentinel(cListNode*);

/**
 * Finds a state by its hash name, searching the state stack and then the explicit-state list (+0x1C).
 *
 * @param id  the state's hash name (mHashName, On Tour hashName)
 * @return the first state with that hash name, or 0 when there is none
 */
extern "C" void* cUIStateStack_findState(void* self, int id)
{
    char* n = (char*)cList_first((cList*)self);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            if (*(int*)(n + 0xC) == id) {
                return n;
            }
            n = *(char**)(n + 4);
        }
    }
    n = (char*)cList_first((cList*)((char*)self + 0x1C));
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            if (*(int*)(n + 0xC) == id) {
                return n;
            }
            n = *(char**)(n + 4);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_getCurrentState);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);
struct cListNode;
int cListNode_isSentinel(cListNode*);

/**
 * Returns the current state: the first state on the stack whose enabled bit (bit 5 of mAttrState) is set.
 *
 * @return that state, or 0 when no state on the stack is enabled
 */
extern "C" void* cUIStateStack_getCurrentState(void* self)
{
    void* n = cList_first((cList*)self);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            if ((*(int*)((char*)n + 0x1C) >> 5) & 1) {
                return n;
            }
            n = *(void**)((char*)n + 4);
        }
    }
    return 0;
}
#endif

struct cList;
void* cList_first(cList*);

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FAE8);
#ifdef SKIP_ASM
extern "C" void* func_0039FAE8(void* self)
{
    return cList_first((cList*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FB08);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_0039FB08(void *arg0, void *arg1) {
    (*(long long *)((char*)(arg0) + (0))) = (long long) (*(long long *)((char*)(arg1) + (0)));
    (*(long long *)((char*)(arg0) + (0x10))) = (long long) (*(long long *)((char*)(arg1) + (0x10)));
    (*(long long *)((char*)(arg0) + (0x20))) = (long long) (*(long long *)((char*)(arg1) + (0x20)));
    (*(long long *)((char*)(arg0) + (0x30))) = (long long) (*(long long *)((char*)(arg1) + (0x30)));
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_cUIThing);
#ifdef SKIP_ASM
struct cUIThing_cUIThing_sVec3 {
    float x;
    float y;
    float z;
};

extern cUIThing_cUIThing_sVec3 t3vect_Zero;
extern char cUIThing_vtbl[];

struct cUIThing_cUIThing_sFlags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int b8 : 5;
    unsigned int align : 6;
};

struct cUIThing_cUIThing_sUIObj {
    void* prev;
    void* next;
    void* vtbl;
    int unkC;
    char unk10;
    cUIThing_cUIThing_sFlags flags;
    int unk18;
    float color[4];
    cUIThing_cUIThing_sVec3 pos;
    int unk38;
    int unk3C;
    int unk40;
    cUIThing_cUIThing_sVec3 rot;
    cUIThing_cUIThing_sVec3 scale;
    int unk5C;
    cUIThing_cUIThing_sVec3 offs;
    int unk6C;
    int unk70;
};

extern "C" cUIThing_cUIThing_sUIObj* cUIThing_cUIThing(cUIThing_cUIThing_sUIObj* self, int a1, int a2)
{
    self->vtbl = cUIThing_vtbl;
    self->flags.b0 = 1;
    self->flags.b7 = 0;
    self->flags.b8 = 0;
    self->flags.b5 = 0;
    self->flags.b4 = 0;
    self->flags.b2 = 0;
    self->flags.b1 = 1;
    self->flags.b3 = 1;
    self->flags.b6 = 1;
    self->flags.align = 9;
    self->next = self;
    self->prev = self;
    self->unkC = 0;
    self->unk10 = 0;
    self->unk18 = 0;
    self->color[0] = 1.0f;
    self->color[1] = 1.0f;
    self->color[2] = 1.0f;
    self->color[3] = 1.0f;
    self->pos = t3vect_Zero;
    self->unk40 = a2;
    self->unk38 = 0;
    self->unk3C = 0;
    self->rot = t3vect_Zero;
    self->scale.x = 1.0f;
    self->scale.y = 1.0f;
    self->scale.z = 1.0f;
    self->unk5C = a1;
    self->offs = t3vect_Zero;
    self->unk6C = 4;
    self->unk70 = 6;
    return self;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing__cUIThing);
#ifdef SKIP_ASM
extern "C" void cListNode_destroyListNode(void* self);
void operator_delete(int* ptr);
extern void* cListNode_vtbl[];

class cUIThing__cUIThing_cVirt {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01(int);
};

/**
 * Destroys a UI thing: deletes its animation, runs the list-node base destructor, then frees it when asked to.
 *
 * Stores cUIThing's vtable, deletes mpAnim through its virtual destructor when there is one, then stores the
 * base vtable (cListNode_vtbl) and calls cListNode_destroyListNode() on the object.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIThing__cUIThing(cUIThing* self, int flags)
{
    self->mVtbl = (void**)cUIThing_vtbl;
    cUIThing__cUIThing_cVirt* anim = (cUIThing__cUIThing_cVirt*)self->mpAnim;
    if (anim != 0) {
        anim->v01(3);
    }
    self->mVtbl = cListNode_vtbl;
    cListNode_destroyListNode(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_playAnimation);
#ifdef SKIP_ASM
extern "C" void cUIThing_deleteAnimation(cUIThing* self);
extern "C" void cUIAnimation_start(void* self, int mode, unsigned short v);

/**
 * Starts an animation on the UI thing, deleting the previous animation when it is a different one.
 *
 * Stores the playback mode in mAnimPlaybackStatus and the animation in mpAnim, then calls cUIAnimation_start().
 * On Tour's version also takes an expire flag, which this one does not.
 *
 * @param anim  the animation to play (On Tour cUIAnimation* pAnim)
 * @param mode  playback mode (On Tour playback)
 * @param frameNum     frame to start from (On Tour FrameNum)
 */
extern "C" void cUIThing_playAnimation(cUIThing* self, void* anim, unsigned char mode, unsigned short frameNum)
{
    if ((void*)self->mpAnim != anim) {
        cUIThing_deleteAnimation(self);
    }
    self->mAnimPlaybackStatus = mode;
    self->mpAnim = (cUIAnimation*)anim;
    cUIAnimation_start(anim, self->mAnimPlaybackStatus, frameNum);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_deleteAnimation);
#ifdef SKIP_ASM
struct sVEntry39FD38 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Deletes the UI thing's animation through its virtual destructor (flags 3) and clears mpAnim.
 *
 * Does nothing when there is no animation.
 */
extern "C" void cUIThing_deleteAnimation(cUIThing* self)
{
    void* obj = self->mpAnim;
    if (obj != 0) {
        sVEntry39FD38* vt = *(sVEntry39FD38**)((char*)obj + 0x10);
        vt[1].fn((char*)obj + vt[1].delta, 3);
        self->mpAnim = (cUIAnimation*)0;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FD80);
#ifdef SKIP_ASM
extern "C" int func_00397458(void* obj);

extern "C" void func_0039FD80(void* self, float f)
{
    void* obj = *(void**)((char*)self + 0xC);
    if (obj != 0) {
        float v = (float)func_00397458(obj) * f;
        *(short*)((char*)*(void**)((char*)self + 0xC) + 0xA) = (short)v;
    }
}
#endif
//100%
INCLUDE_ASM("ui/uistatestack", func_0039FDD8);
#ifdef SKIP_ASM
extern "C" int func_00397440(int);

extern "C" int func_0039FDD8(void *arg0) {
    int temp_4;
    int var_2;

    temp_4 = (*(int *)((char*)(arg0) + (0xC)));
    var_2 = 0;
    if (temp_4 != 0) {
        var_2 = func_00397440(temp_4);
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_setUIData);
#ifdef SKIP_ASM
struct sStateDef39FE00 {
    int f0;
    int id;                 // 0x04
    unsigned char layer : 5; // 0x08
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
    unsigned char pad9[3];
};

struct sVE39FE00 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sState39FE00 {
    char pad0[0x8];
    sVE39FE00* vt;          // 0x08
    char padC[0x8];
    unsigned int b0 : 1;    // 0x14
    unsigned int b1 : 6;
    unsigned int b7 : 1;
    unsigned int layer : 5;
    unsigned int rest : 19;
    char pad18[0x20];
    int id;                 // 0x38
    void* data;             // 0x3C
};

/**
 * Initialises a UI object from its screen-data record: hash name, layer, flags, visibility and event table.
 *
 * Copies the record's hash (+0x4) to mHashName and its 5-bit layer (+0x8) to mLayer; mOrientation and mWrap
 * come from bits 6 and 7 of the record's flag word and the visibility from bit 9, through the virtual
 * setVisible (slot 9). mpEvents points just past the 12-byte record.
 *
 * @param def  the object's record in the screen data (On Tour pObj, a tUIBaseObject)
 */
// Initialises a UI state from its definition record (id, layer, flags) and points it at the following data.
extern "C" void cUIThing_setUIData(sState39FE00* self, sStateDef39FE00* def)
{
    self->id = def->id;
    self->layer = def->layer;
    self->b0 = (*(int*)((char*)def + 8) >> 6) & 1;
    self->vt[9].fn((char*)self + self->vt[9].delta, (*(int*)((char*)def + 8) >> 9) & 1);
    self->b7 = def->b7;
    self->data = def + 1;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_applyPivot);
#ifdef SKIP_ASM
struct sUIAlignFlags14 {
    unsigned int lo : 13;
    unsigned int align : 6;
};

struct sUIVec2 {
    float x;
    float y;
};

static inline unsigned int getUIAlign(void* self)
{
    return ((sUIAlignFlags14*)((char*)self + 0x14))->align;
}

extern "C" void cUIThing_applyPivot(void* self, sUIVec2* pos, sUIVec2* size)
{
    unsigned int align = getUIAlign(self);
    if ((align & 1) == 0) {
        if (align & 4) {
            float d = size->y;
            pos->y -= d;
        } else {
            pos->y -= size->y * 0.5f;
        }
    }
    if ((align & 8) == 0) {
        if (align & 0x20) {
            pos->x -= size->x;
        } else {
            pos->x -= size->x * 0.5f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_getEngine__FPv);
#ifdef SKIP_ASM
int cUIThing_getEngine(void* self)
{
    return *(int*)((char*)*(void**)((char*)*(void**)((char*)self + 0x5c) + 0xd0) + 0x10);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_getObjectByHashName);
#ifdef SKIP_ASM
/**
 * Returns this UI thing when its hash name matches; the base version does not search any children.
 *
 * @param hashname  the hash name to look for
 * @return this object when its mHashName equals @p hashname, otherwise 0
 */
extern "C" void* cUIThing_getObjectByHashName(cUIThing* self, int hashname)
{
    return self->mHashName == hashname ? ((void*)self) : 0;
}
#endif


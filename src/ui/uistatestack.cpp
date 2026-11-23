#include "vtable.h"
#include "common.h"
#include "ui/uistate.h"
#include "ui/uianimation.h"
#include "ui/uistatestack.h"
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
    virtual void onCreateScreen();
};

/**
 * Adds an explicit state: enables it, notifies it and appends it to the stack's second list.
 *
 * Sets mEnabled and mExplicit (0x60) in the state's mAttrState, calls its virtual slot 4 (empty in cUIState;
 * the slot before onDestroyScreen) and adds it to the end of the cList at +0x1C, not the main stack at +0x0.
 *
 * @param state  the state to add (On Tour pPushState)
 */
extern "C" void cUIStateStack_pushExplicit(cUIStateStack* self, cUIStateStack_pushExplicit_cState* state)
{
    *(int*)((char*)state + 0x1C) |= 0x60;
    state->onCreateScreen();
    cList_addToEnd((cList*)&self->mExplicitStack, (cListNode*)state);
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
    unsigned int mIsPopup : 1;
    unsigned int mHasPopup : 2;
    unsigned int mVisible : 1;
    unsigned int mFirstTime : 1;
    unsigned int mEnabled : 1;
    unsigned int rest : 26;
    cUIStateStack_pushPopup_sState* parent;   // 0x20
};

// A state gets covered by a newly pushed one: it is no longer on top and sits one level deeper.
static inline void cUIStateStack_pushPopup_cover(cUIStateStack_pushPopup_sState* st)
{
    st->mEnabled = 0;
    st->mHasPopup = st->mHasPopup + 1;
}

/**
 * Pushes a popup state onto the front of the state stack, covering the state beneath it.
 *
 * The covered state is the popup's parent when it has one, else the current front of the stack: it loses its
 * enabled bit and its popup level goes up by one. The popup gets its is-popup and enabled bits set.
 *
 * @param st  the popup state to push (On Tour pPushState)
 */
extern "C" void cUIStateStack_pushPopup(cUIStateStack* self, cUIStateStack_pushPopup_sState* st)
{
    cUIStateStack_pushPopup_sState* top = st->parent;
    if (top != 0) {
        cUIStateStack_pushPopup_cover(top);
    } else {
        top = (cUIStateStack_pushPopup_sState*)cList_first(((cList*)self));
        if (top != 0) {
            cUIStateStack_pushPopup_cover(top);
        }
    }
    st->mIsPopup = 1;
    st->mEnabled = 1;
    cList_addToFront(((cList*)self), (cListNode*)st);
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

extern "C" void* cUIStateStack_getCurrentState(cUIStateStack* self);

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
extern "C" void cUIStateStack_pushSpecial(cUIStateStack* self, cUIStateStack_sState* st, int bEnable, int bDestroyCurrent)
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
    cList_addToFront(((cList*)self), (cListNode*)st);
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
extern "C" void cUIStateStack_push(cUIStateStack* self, cUIStateStack_push_sState* st)
{
    int covered = 0;
    cUIStateStack_push_sState* top = (cUIStateStack_push_sState*)cList_first(((cList*)self));
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
    cList_addToFront(((cList*)self), (cListNode*)st);
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
    unsigned int mIsPopup : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int mVisible : 1;
    unsigned int mFirstTime : 1;
    unsigned int mEnabled : 1;
    unsigned int mExplicit : 1;
    unsigned int mPendingDelete : 1;
    unsigned int id : 6;
    unsigned int mPushCheck : 1;
};

static inline void setB7_F4C0(cUIStateStack_pushCheck_sState* s, int v) { s->mPendingDelete = v; }
static inline void setB14_F4C0(cUIStateStack_pushCheck_sState* s, int v) { s->mPushCheck = v; }
static inline int cUIStateStack_pushCheck_stateId(cUIStateStack_pushCheck_sState* s) { return s->id; }

// Release-build assert: expands to an empty do { } while (0); the leftover loop notes shape register
// allocation of the pinned-state loop the way the target has it.
#define ASSERT_F4C0(cond) do { } while (0)

/**
 * Pushes a state onto the UI state stack, deciding from the states below whether it covers them.
 *
 * Popup states at the top (bit 0 set) are walked first: each whose mode is below 6 becomes 7 (pending removal) with bit
 * 7 set, and bit 14 is set on each. If any were skipped, or the first normal state has bit 5 set, and that state is not
 * explicit (bit 6) and still active (mode below 6), it is marked mode 6 with bit 7 and the new state @p st gets mode 1
 * with bit 5 clear; otherwise @p st gets mode 2 with bit 5 set. @p st is then added at the list front.
 *
 * @param st  the state to push (On Tour pPushState)
 */
extern "C" void cUIStateStack_pushCheck(cUIStateStack* self, cUIStateStack_pushCheck_sState* st)
{
    int covered = 0;
    int skipped = 0;
    cUIStateStack_pushCheck_sState* top = (cUIStateStack_pushCheck_sState*)cList_first(((cList*)self));
    if (top != 0) {
        while (top->mIsPopup) {
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
        int stateFlags = *(int*)((char*)top + 0x1C);
        if (skipped || ((stateFlags >> 5) & 1)) {
            if (((stateFlags >> 6) & 1) == 0 && cUIStateStack_pushCheck_stateId(top) < 6) {
                covered = 1;
                top->id = 6;
                top->mPendingDelete = 1;
            }
        }
    }
    if (covered) {
        st->id = 1;
        st->mEnabled = 0;
    } else {
        st->id = 2;
        st->mEnabled = 1;
    }
    cList_addToFront(((cList*)self), (cListNode*)st);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_popToFloor);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);

struct cUIStateStack_popToFloor_node {
    char pad0[0x4];
    cUIStateStack_popToFloor_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int mIsPopup : 1;
    unsigned int b1 : 5;
    unsigned int mExplicit : 1;
    unsigned int mPendingDelete : 1;
    unsigned int id : 6;
    unsigned int mPushCheck : 1;
    unsigned int mPopFloor : 1;
};

static inline void cUIStateStack_popToFloor_setId(cUIStateStack_popToFloor_node* n, int v) { n->id = v; }
static inline void cUIStateStack_popToFloor_setB7(cUIStateStack_popToFloor_node* n, int v) { n->mPendingDelete = v; }

/**
 * Closes the popups on top of a state stack down to a pop floor; returns the state where it stopped.
 *
 * Walks from the top while the state is a popup (bit 0) without the pop-floor bit (15); each one below mode 6 is set to
 * mode 7 (closed) and pending delete. The challenge popup code calls it.
 *
 * @param list  the state stack (its main list)
 * @return the first non-popup or pop-floor state, or null
 */
extern "C" cUIStateStack_popToFloor_node* cUIStateStack_popToFloor(cUIStateStack* list)
{
    cUIStateStack_popToFloor_node* n = (cUIStateStack_popToFloor_node*)cList_first(((cList*)list));
    if (n != 0) {
        while (n->mIsPopup) {
            if ((*(int*)((char*)n + 0x1C) >> 15) & 1) {
                break;
            }
            if (n->id < 6) {
                cUIStateStack_popToFloor_setId(n, 7);
                cUIStateStack_popToFloor_setB7(n, 1);
            }
            n = n->next;
        }
        return n;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_popToState);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);

struct cUIStateStack_popToState_node {
    char pad0[0x4];
    cUIStateStack_popToState_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int mIsPopup : 1;
    unsigned int b1 : 5;
    unsigned int mExplicit : 1;
    unsigned int mPendingDelete : 1;
    unsigned int id : 6;
};

static inline void cUIStateStack_popToState_setId(cUIStateStack_popToState_node* n, int v) { n->id = v; }
static inline void cUIStateStack_popToState_setB7(cUIStateStack_popToState_node* n, int v) { n->mPendingDelete = v; }

/**
 * Closes all popups on top of a state stack; returns the first non-popup state.
 *
 * Each popup below mode 6 is set to mode 7 (closed) and pending delete. The trophy and file screens call it.
 *
 * @param list  the state stack (its main list)
 * @return the first non-popup state, or null
 */
extern "C" cUIStateStack_popToState_node* cUIStateStack_popToState(cUIStateStack* list)
{
    cUIStateStack_popToState_node* n = (cUIStateStack_popToState_node*)cList_first(((cList*)list));
    if (n != 0) {
        if (n->mIsPopup) {
            do {
                if (n->id < 6) {
                    cUIStateStack_popToState_setId(n, 7);
                    cUIStateStack_popToState_setB7(n, 1);
                }
                n = n->next;
            } while (n->mIsPopup);
        }
        return n;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_popToStateInclusive);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);

struct cUIStateStack_popToStateInclusive_node {
    char pad0[0x4];
    cUIStateStack_popToStateInclusive_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int mIsPopup : 1;
    unsigned int b1 : 5;
    unsigned int mExplicit : 1;
    unsigned int mPendingDelete : 1;
    unsigned int id : 6;
};

static inline void cUIStateStack_popToStateInclusive_setId(cUIStateStack_popToStateInclusive_node* n, int v) { n->id = v; }
static inline void cUIStateStack_popToStateInclusive_setB7(cUIStateStack_popToStateInclusive_node* n, int v) { n->mPendingDelete = v; }

/**
 * Closes all popups on top of a state stack and the state under them; returns the state below that.
 *
 * Each closed state below mode 6 is set to mode 7 and pending delete. The pause and profile save screens call it.
 *
 * @param list  the state stack (its main list)
 * @return the next state down, or null
 */
extern "C" cUIStateStack_popToStateInclusive_node* cUIStateStack_popToStateInclusive(cUIStateStack* list)
{
    cUIStateStack_popToStateInclusive_node* n = (cUIStateStack_popToStateInclusive_node*)cList_first(((cList*)list));
    if (n != 0) {
        if (n->mIsPopup) {
            do {
                if (n->id < 6) {
                    cUIStateStack_popToStateInclusive_setId(n, 7);
                    cUIStateStack_popToStateInclusive_setB7(n, 1);
                }
                n = n->next;
            } while (n->mIsPopup);
        }
        if (n->id < 6) {
            n->id = 7;
            n->mPendingDelete = 1;
        }
        return n->next;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_setVisibleToChain);
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

/**
 * Sets the visible bit of a chain of states, from @p state down through the states that have a parent.
 *
 * Walks next links while the state's parent (+0x20) is set, then also the last one when @p last is nonzero; states in
 * mode 6 or above are left alone. cUIStateStack_updateLoop uses it on a popup's parent chain.
 *
 * @param state  the first state of the chain
 * @param on  1 to show, 0 to hide
 * @param last  nonzero to include the state without a parent
 */
extern "C" void cUIStateStack_setVisibleToChain(cUIStateStack* self, sUIState39F7B8* state, int on, int last)
{
    if (state == 0) {
        return;
    }
    sUIState39F7B8* cur = state;
    while (cur->child != 0) {
        Set39F7B8(cur, on);
        cur = cur->next;
    }
    state = cur;
    if (last != 0) {
        Set39F7B8(state, on);
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

/**
 * Marks every state on a UI state list for removal.
 *
 * Walks the list from cList_first to the sentinel and, in each state's mAttrState (+0x1C), sets mMode to 7 and
 * mPendingDelete and mEnabled to 1. Callers pass the UI engine's state stack (+0x18).
 *
 * @param list  the state list
 */
extern "C" void cUIStateStack_purgeNormalStates(cUIStateStack* list)
{
    cUIStateStack_purgeNormalStates_node* n = (cUIStateStack_purgeNormalStates_node*)cList_first(((cList*)list));
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
INCLUDE_ASM("ui/uistatestack", cUIStateStack_destroyStates);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct sUIStVtF8C8 { short delta; short index; void (*fn)(void*, int); };
struct sUIStVtF8C8a { short delta; short index; void (*fn)(void*); };

static inline void cUIStateStack_destroyStates_purge(cList* list)
{
    cUIState* n = (cUIState*)cList_first(list);
    if (((char*)n) != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            char* next = (char*)n->mpNext;
            if (((n->mAttrState >> 6) & 1) == 0) {
                sUIStVtF8C8a* vt = (sUIStVtF8C8a*)n->mVtbl;
                vt[5].fn(vthis(n, vt[5]));
                if (((char*)n) != 0) {
                    sUIStVtF8C8* vt2 = (sUIStVtF8C8*)n->mVtbl;
                    vt2[1].fn(vthis(n, vt2[1]), 3);
                }
            }
            n = (cUIState*)next;
        }
    }
}

/**
 * Destroys the non-explicit states of a state stack's main and explicit lists.
 *
 * Each state without the explicit bit (6) gets virtual slot 5, then is deleted through slot 1 with 3. cUIStateStack__cUIStateStack
 * calls it first.
 */
extern "C" void cUIStateStack_destroyStates(cUIStateStack* self)
{
    cUIStateStack_destroyStates_purge(((cList*)self));
    cUIStateStack_destroyStates_purge((cList*)&self->mExplicitStack);
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
extern "C" void* cUIStateStack_findState(cUIStateStack* self, int id)
{
    cUIState* n = (cUIState*)cList_first((cList*)self);
    if (((char*)n) != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            if (n->mHashName == id) {
                return ((char*)n);
            }
            n = (cUIState*)n->mpNext;
        }
    }
    n = (cUIState*)cList_first((cList*)&self->mExplicitStack);
    if (((char*)n) != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            if (n->mHashName == id) {
                return ((char*)n);
            }
            n = (cUIState*)n->mpNext;
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
extern "C" void* cUIStateStack_getCurrentState(cUIStateStack* self)
{
    cUIState* n = (cUIState*)cList_first((cList*)self);
    if (((void*)n) != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            if ((n->mAttrState >> 5) & 1) {
                return ((void*)n);
            }
            n = (cUIState*)((void*)n->mpNext);
        }
    }
    return 0;
}
#endif

struct cList;
void* cList_first(cList*);

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_getTopState);
#ifdef SKIP_ASM
/**
 * Returns the top state of a state stack (the first node of its main list).
 *
 * func_00259230 and a file-screen function use it on an engine's +0x18.
 *
 * @return the top state, or null
 */
extern "C" void* cUIStateStack_getTopState(cUIStateStack* self)
{
    return cList_first((cList*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FB08);
#ifdef SKIP_ASM
/**
 * Copies the first 8 bytes of each of four 16-byte blocks (offsets 0x0, 0x10, 0x20, 0x30) from @p src to @p dst.
 *
 * Nothing in the retail code references it.
 *
 * @param dst  the destination
 * @param src  the source
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_0039FB08(void *dst, void *src) {
    (*(long long *)((char*)(dst) + (0))) = (long long) (*(long long *)((char*)(src) + (0)));
    (*(long long *)((char*)(dst) + (0x10))) = (long long) (*(long long *)((char*)(src) + (0x10)));
    (*(long long *)((char*)(dst) + (0x20))) = (long long) (*(long long *)((char*)(src) + (0x20)));
    (*(long long *)((char*)(dst) + (0x30))) = (long long) (*(long long *)((char*)(src) + (0x30)));
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


/**
 * Constructs the base UI widget object.
 *
 * Installs cUIThing_vtbl, sets the enabled/visible flags (bits 0, 1, 3, 6) and clears the rest, sets the alignment to
 * 9, makes the list node point at itself, clears +0xC, +0x10, +0x18, +0x38 and +0x3C, sets the colour to (1, 1, 1, 1),
 * the position, rotation and offset to zero and the scale to 1, and stores 4 and 6 at +0x6C and +0x70.
 *
 * @param pScreen  the screen that owns the widget (On Tour pScreen)
 * @param pParent  the parent widget, or 0 (On Tour pParent)
 * @return this
 */
extern "C" cUIThing* cUIThing_cUIThing(cUIThing* self, int pScreen, int pParent)
{
    VPTR_OF(cListNode, self) = (void**)cUIThing_vtbl;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b0 = 1;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b7 = 0;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b8 = 0;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b5 = 0;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b4 = 0;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b2 = 0;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b1 = 1;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b3 = 1;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).b6 = 1;
    (*(cUIThing_cUIThing_sFlags*)&self->mAttr).align = 9;
    self->mpNext = (cListNode*)(self);
    self->mpPrev = (cListNode*)(self);
    self->mpAnim = 0;
    *(char*)&self->mAnimPlaybackStatus = 0;
    self->mMiscData = 0;
    self->mColour.A = 1.0f;
    self->mColour.R = 1.0f;
    self->mColour.G = 1.0f;
    self->mColour.B = 1.0f;
    (*(cUIThing_cUIThing_sVec3*)&self->mAngle) = t3vect_Zero;
    self->mpParent = (cUIThing*)pParent;
    self->mHashName = 0;
    self->mpEvents = 0;
    (*(cUIThing_cUIThing_sVec3*)&self->mPosition) = t3vect_Zero;
    self->mScale.x = 1.0f;
    self->mScale.y = 1.0f;
    self->mScale.z = 1.0f;
    self->mpScreen = (cUIScreen*)pScreen;
    (*(cUIThing_cUIThing_sVec3*)&self->mSize) = t3vect_Zero;
    self->mSound[0] = 4;
    self->mSound[1] = 6;
    return (self);
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
    VPTR_OF(cListNode, self) = (void**)cUIThing_vtbl;
    cUIThing__cUIThing_cVirt* anim = (cUIThing__cUIThing_cVirt*)self->mpAnim;
    if (anim != 0) {
        anim->v01(3);
    }
    VPTR_OF(cListNode, self) = cListNode_vtbl;
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

/**
 * Deletes the UI thing's animation through its virtual destructor (flags 3) and clears mpAnim.
 *
 * Does nothing when there is no animation.
 */
extern "C" void cUIThing_deleteAnimation(cUIThing* self)
{
    cUIAnimation* obj = self->mpAnim;
    if (obj != 0) {
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)obj->mVtbl;
        vt[1].fn(vthis(obj, vt[1]), 3);
        self->mpAnim = (cUIAnimation*)0;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_setFrameByPercent);
#ifdef SKIP_ASM
extern "C" int cUIAnimation_getNumFrames(void* obj);

/**
 * Sets a UI thing's animation frame (+0xA of mpAnim) to its frame count times @p fraction, truncated.
 *
 * Does nothing without an animation. The count comes from cUIAnimation_getNumFrames.
 *
 * @param fraction  the position in the animation, 0..1
 */
extern "C" void cUIThing_setFrameByPercent(cUIThing* self, float fraction)
{
    void* obj = self->mpAnim;
    if (obj != 0) {
        float frame = (float)cUIAnimation_getNumFrames(obj) * fraction;
        self->mpAnim->mFrameNum = (short)frame;
    }
}
#endif
//100%
INCLUDE_ASM("ui/uistatestack", cUIThing_isLastAnimFrame);
#ifdef SKIP_ASM
extern "C" int cUIAnimation_isLastFrame(int);

/**
 * Returns whether a UI thing's animation (mpAnim) is on its last frame; false without an animation.
 *
 * Uses cUIAnimation_isLastFrame.
 *
 * @param uiThing  the UI thing
 * @return 1 at the last frame, else 0
 */
extern "C" int cUIThing_isLastAnimFrame(cUIThing* uiThing) {
    int temp_4;
    int var_2;

    temp_4 = ((int)uiThing->mpAnim);
    var_2 = 0;
    if (temp_4 != 0) {
        var_2 = cUIAnimation_isLastFrame(temp_4);
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
extern "C" void cUIThing_setUIData(sState39FE00* self, sStateDef39FE00* def)
{
    self->id = def->id;
    self->layer = def->layer;
    self->b0 = (*(int*)((char*)def + 8) >> 6) & 1;
    self->vt[9].fn(vthis(self, self->vt[9]), (*(int*)((char*)def + 8) >> 9) & 1);
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

/**
 * Moves a widget's draw position so that its alignment pivot lands on the given position.
 *
 * Unless alignment bit 0 (top) is set, y moves up by the full height (bit 2, bottom) or half of it (centre). Unless bit
 * 3 (left) is set, x moves left by the full width (bit 5, right) or half of it.
 *
 * @param pos  the position to adjust in place (On Tour pNewPosition)
 * @param size  the widget's size
 */
extern "C" void cUIThing_applyPivot(cUIThing* self, sUIVec2* pos, sUIVec2* size)
{
    unsigned int align = getUIAlign(self);
    if ((align & 1) == 0) {
        if (align & 4) {
            float height = size->y;
            pos->y -= height;
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
/**
 * Returns the UI engine a widget belongs to.
 *
 * Follows the widget's mpScreen (+0x5C) to the screen's mpState (+0xD0) and returns that state's mpEngine (+0x10).
 *
 * @return the UI engine
 */
int cUIThing_getEngine(void* vself)
{
    cUIThing* self = (cUIThing*)vself;
    return *(int*)((char*)*(void**)((char*)self->mpScreen + 0xd0) + 0x10);
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


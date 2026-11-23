#include "vtable.h"
#include "common.h"
#include "ui/uithing.h"
#include "ui/uistatestack.h"
#include "ui/uistate.h"

//100%
INCLUDE_ASM("ui/uistate", cUIState_hideObjSafe);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
cListNode* cList_first(cList*);
int cListNode_isSentinel(cListNode*);
int GetHashValue32(char*);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);


/**
 * Hides a named object on the UI state's screens, doing nothing when no screen has it.
 *
 * Searches the screens in order for an object with the name's hash and calls its virtual setVisible(0) (slot 9)
 * on the first one found.
 *
 * @param name  the object's name
 */
extern "C" void cUIState_hideObjSafe(cUIState* self, char* name)
{
    cListNode* n = cList_first((cList*)&self->mScreens);
    if (n != 0) {
        do {
            cUIThing* obj = (cUIThing*)cUIScreen_getObjectByHashName(n, GetHashValue32(name));
            if (((void*)obj) != 0) {
                obj->setVisible(0);
                return;
            }
            n = n->mpNext;
        } while (!cListNode_isSentinel(n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIState_showObjSafe);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
cListNode* cList_first(cList*);
int cListNode_isSentinel(cListNode*);
int GetHashValue32(char*);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);


/**
 * Shows a named object on the UI state's screens, doing nothing when no screen has it.
 *
 * Searches the screens in order for an object with the name's hash and calls its virtual setVisible(1) (slot 9)
 * on the first one found.
 *
 * @param name  the object's name
 */
extern "C" void cUIState_showObjSafe(cUIState* self, char* name)
{
    cListNode* n = cList_first((cList*)&self->mScreens);
    if (n != 0) {
        do {
            cUIThing* obj = (cUIThing*)cUIScreen_getObjectByHashName(n, GetHashValue32(name));
            if (((void*)obj) != 0) {
                obj->setVisible(1);
                return;
            }
            n = n->mpNext;
        } while (!cListNode_isSentinel(n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIStateStack__cUIStateStack);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_destroyStates(void* self);
extern "C" void cList_destroyList(void*);
extern "C" void cListNode_destroyListNode(void* self);
void operator_delete(int*);
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];

/**
 * Destroys a UI state stack: destroys its states (cUIStateStack_destroyStates), then its two lists.
 *
 * The lists are the main stack (+0x0) and the explicit stack (+0x1C); bit 0 of @p flags frees the object. The UI engine
 * destructor cUIEngine__cUIEngine runs it on its +0x18.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cUIStateStack__cUIStateStack(cUIStateStack* self, int flags)
{
    cUIStateStack_destroyStates(self);
    VPTR_OF(cList, &self->mExplicitStack) = cList_vtbl;
    cList_destroyList((char*)&self->mExplicitStack);
    VPTR_OF(cListNode, &self->mExplicitStack.mEnd) = cListNode_vtbl;
    cListNode_destroyListNode(&self->mExplicitStack.mEnd);
    VPTR_OF(cListNode, &self->mExplicitStack.mStart) = cListNode_vtbl;
    cListNode_destroyListNode((char*)&self->mExplicitStack);
    VPTR_OF(cList, &self->mStack) = cList_vtbl;
    cList_destroyList(self);
    VPTR_OF(cListNode, &self->mStack.mEnd) = cListNode_vtbl;
    cListNode_destroyListNode(&self->mStack.mEnd);
    VPTR_OF(cListNode, &self->mStack.mStart) = cListNode_vtbl;
    cListNode_destroyListNode(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIStateStack_enableState);
#ifdef SKIP_ASM
struct cUIStateStack_enableState_sFlags {
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

struct cUIStateStack_enableState_node {
    char pad0[0x4];
    cUIStateStack_enableState_node* next; // 0x4
    char pad8[0x1C - 0x8];
    union {
        int flags; // 0x1C
        cUIStateStack_enableState_sFlags bits;
    };
};

static inline void cUIStateStack_enableState_setB3(cUIStateStack_enableState_node* n, int v) { n->bits.b3 = v; }
static inline void cUIStateStack_enableState_setB5(cUIStateStack_enableState_node* n, int v) { n->bits.b5 = v; }
static inline void cUIStateStack_enableState_setId(cUIStateStack_enableState_node* n, int v) { n->bits.id = v; }
static inline void cUIStateStack_enableState_setB4(cUIStateStack_enableState_node* n, int v) { n->bits.b4 = v; }

/**
 * Enables the first non-explicit state of mode @p id on a state stack: sets it visible, enabled and first-time, mode 2
 * (starting).
 *
 * Modes are bits 8..13 of the state's attribute word (+0x1C); explicit states (bit 6) are skipped. cUIStateStack_updateLoop uses
 * it to bring up a mode-1 or mode-8 state when nothing is current.
 *
 * @param list  the state stack (its main list)
 * @param id  the mode to look for
 * @return 1 when a state was enabled, else 0
 */
extern "C" int cUIStateStack_enableState(cUIStateStack* list, signed char id)
{
    cUIStateStack_enableState_node* n = (cUIStateStack_enableState_node*)cList_first(((cList*)list));
    if (n != 0) {
        do {
            if ((((unsigned int)n->flags >> 8) & 0x3F) == id && ((n->flags >> 6) & 1) == 0) {
                cUIStateStack_enableState_setB3(n, 1);
                cUIStateStack_enableState_setB5(n, 1);
                cUIStateStack_enableState_setId(n, 2);
                cUIStateStack_enableState_setB4(n, 1);
                return 1;
            }
            n = n->next;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIStateStack_checkForState);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
cListNode* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct cUIStateStack_checkForState_node {
    char pad0[0x4];
    cUIStateStack_checkForState_node* next; // 0x4
    char pad8[0x1C - 0x8];
    int flags; // 0x1C
};

/**
 * Returns whether a state stack holds a non-explicit state of mode @p id.
 *
 * cUIStateStack_updateLoop uses it with mode 1 when a popup closes.
 *
 * @param list  the state stack (its main list)
 * @param id  the mode to look for
 * @return 1 when one exists, else 0
 */
extern "C" int cUIStateStack_checkForState(cUIStateStack* list, signed char id)
{
    cUIStateStack_checkForState_node* n = (cUIStateStack_checkForState_node*)cList_first(((cList*)list));
    if (n != 0) {
        do {
            if ((((unsigned int)n->flags >> 8) & 0x3F) == id && ((n->flags >> 6) & 1) == 0) {
                return 1;
            }
            n = n->next;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIStateStack_render2D);
#ifdef SKIP_ASM
struct cUIStateStack_render2D_sVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Draws a state stack: calls virtual slot 15 (2D render) on every visible state of the main and explicit lists.
 *
 * Visible is bit 3 of a state's attribute word (+0x1C). cUIEngine_render2D calls it on the engine's state stack.
 */
extern "C" void cUIStateStack_render2D(cUIStateStack* self)
{
    cUIState* n = (cUIState*)cList_first((cList*)self);
    if (((char*)n) != 0) {
        do {
            if ((n->mAttrState >> 3) & 1) {
                cUIStateStack_render2D_sVEntry* vt = (cUIStateStack_render2D_sVEntry*)n->mVtbl;
                vt[15].fn(vthis(n, vt[15]));
            }
            n = (cUIState*)n->mpNext;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    n = (cUIState*)cList_first((cList*)&self->mExplicitStack);
    if (((char*)n) != 0) {
        do {
            if ((n->mAttrState >> 3) & 1) {
                cUIStateStack_render2D_sVEntry* vt = (cUIStateStack_render2D_sVEntry*)n->mVtbl;
                vt[15].fn(vthis(n, vt[15]));
            }
            n = (cUIState*)n->mpNext;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIStateStack_render3D__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the state stack's 3D render.
 *
 * cUIEngine_render3D calls it on the engine's state stack.
 */
void cUIStateStack_render3D(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIStateStack_updateLoop);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cUIState_updateScreens(void* self);
extern "C" void cUIStateStack_setVisibleToChain(void* self, void* st, int a2, int a3);
extern "C" void cListNode_removeFromList(void* self);
extern "C" void* cUIStateStack_getCurrentState(void* self);

struct cUIStateStack_updateLoop_sFlags {
    unsigned int b0 : 1;
    unsigned int cnt : 2;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

struct cUIStateStack_updateLoop_sVEntry {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

struct cUIStateStack_updateLoop_node {
    cUIStateStack_updateLoop_sVEntry* pad0;
    cUIStateStack_updateLoop_node* next; // 0x4
    cUIStateStack_updateLoop_sVEntry* vt; // 0x8
    char padC[0x1C - 0xC];
    union {
        int flags; // 0x1C
        cUIStateStack_updateLoop_sFlags bits;
    };
    cUIStateStack_updateLoop_node* owner; // 0x20
};

struct cUIStateStack_updateLoop_sVEntryV {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

// g++ 2.95 virtual call through a {delta, index, fn} vtable entry; the V form discards the result.
#define VCALL_ECB0(n, i, args...) (n)->vt[i].fn((char*)(n) + (n)->vt[i].delta , ## args)
#define VCALLV_ECB0(n, i, args...) ((cUIStateStack_updateLoop_sVEntryV*)(n)->vt)[i].fn((char*)(n) + (n)->vt[i].delta , ## args)

/**
 * Runs one pass of the UI state life cycle over the enabled states of a state stack's main list.
 *
 * By mode (attribute bits 8..13): 2 calls virtual slot 4 and becomes 4 (a popup also shows its parent chain with
 * cUIStateStack_setVisibleToChain and sends the parent event 0x15 through slot 17); 4 waits for slot 7 (passed the first-time bit) to
 * return nonzero, then 3; 3 calls slot 6 and becomes 5; 5 calls slot 12 each pass; 6 waits for slot 10, then 7; 7 calls
 * slots 11 and 5, becomes 8 and is disabled. A closing popup is marked pending delete, lowers its parent's popup count
 * (re-enabling the parent at 0) and sends it event 0x16; but when a mode-1 state exists (checked unless the push-check
 * bit 14 is set) it only hides its parent chain. A pending-delete state is unlinked and deleted (slot 1 with 3), others
 * are hidden. When nothing is current afterwards, a mode-1 state (or else a mode-8 one) is enabled with cUIStateStack_enableState.
 * Changed states run cUIState_updateScreens.
 *
 * @return nonzero when a state finished closing and none started in this pass (cUIStateStack_update then runs another pass)
 */
extern "C" int cUIStateStack_updateLoop(cUIStateStack* self)
{
    int popped = 0;
    int any = 0;
    cUIStateStack_updateLoop_node* n = (cUIStateStack_updateLoop_node*)cList_first((cList*)self);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            cUIStateStack_updateLoop_node* next = n->next;
            if ((n->flags >> 5) & 1) {
                switch ((signed char)n->bits.id) {
                case 2:
                    any = 1;
                    VCALLV_ECB0(n, 4);
                    n->bits.id = 4;
                    cUIState_updateScreens(n);
                    if (n->flags & 1) {
                        cUIStateStack_updateLoop_node* o = n->owner;
                        if (o != 0) {
                            cUIStateStack_setVisibleToChain(self, o, 1, 1);
                            VCALLV_ECB0(o, 17, n, 0x15);
                        }
                    }
                case 4:
                    if (VCALL_ECB0(n, 7, (n->flags >> 4) & 1)) {
                        n->bits.id = 3;
                    }
                    n->bits.b4 = 0;
                    cUIState_updateScreens(n);
                    break;
                case 3:
                    if ((n->flags >> 4) & 1) {
                        VCALL_ECB0(n, 7, 1);
                        n->bits.b4 = 0;
                    }
                    VCALLV_ECB0(n, 6, 0);
                    n->bits.id = 5;
                    cUIState_updateScreens(n);
                    break;
                case 5:
                    VCALLV_ECB0(n, 12);
                    cUIState_updateScreens(n);
                    break;
                case 6:
                    if (VCALL_ECB0(n, 10)) {
                        n->bits.id = 7;
                    }
                    n->bits.b4 = 1;
                    cUIState_updateScreens(n);
                    break;
                case 7: {
                    int top;
                    int found = 0;
                    popped = 1;
                    VCALLV_ECB0(n, 11, 0);
                    VCALLV_ECB0(n, 5);
                    n->bits.id = 8;
                    n->bits.b5 = 0;
                    if ((n->flags >> 7) & 1) {
                        cListNode_removeFromList(n);
                    }
                    top = n->flags & 1;
                    if (top) {
                        cUIStateStack_updateLoop_node* o = n->owner;
                        popped = 0;
                        if (((n->flags >> 14) & 1) == 0) {
                            found = cUIStateStack_checkForState(self, 1);
                        } else {
                            found = 0;
                        }
                        if (found == 0) {
                            n->bits.b7 = 1;
                            if (o != 0) {
                                if (o->bits.cnt != 0) {
                                    o->bits.cnt = o->bits.cnt - 1;
                                }
                                o->bits.b5 = o->bits.cnt == 0;
                                VCALLV_ECB0(o, 17, n, 0x16);
                            }
                        } else if (o != 0) {
                            cUIStateStack_setVisibleToChain(self, o, 0, 1);
                        }
                    }
                    if (n != 0) {
                        if ((n->flags >> 7) & 1) {
                            cListNode_removeFromList(n);
                            VCALLV_ECB0(n, 1, 3);
                        } else {
                            n->bits.b3 = 0;
                        }
                    }
                    if (top == 0 || found != 0) {
                        if (cUIStateStack_getCurrentState(self) == 0 && cUIStateStack_enableState(self, 1) == 0) {
                            cUIStateStack_enableState(self, 8);
                        }
                    }
                    break;
                }
                }
            }
            n = next;
        }
    }
    if (popped) {
        return any ^ 1;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uistate", cUIStateStack_update);
#ifdef SKIP_ASM
extern "C" int cUIStateStack_updateLoop(cUIStateStack* self);
extern "C" void cUIState_updateScreens(void* self);


/**
 * Updates a state stack: runs cUIStateStack_updateLoop (a second pass when the first returns nonzero), then updates the explicit
 * states.
 *
 * Every state of the explicit list (+0x1C) gets virtual slot 12 when enabled and cUIState_updateScreens. The UI engine
 * update cUIEngine_update calls it.
 */
extern "C" void cUIStateStack_update(cUIStateStack* self)
{
    if (cUIStateStack_updateLoop(self) != 0) {
        cUIStateStack_updateLoop(self);
    }
    cListNode* n = cList_first((cList*)&self->mExplicitStack);
    if (n != 0) {
        do {
            if ((*(int*)((char*)n + 0x1C) >> 5) & 1) {
                sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)VPTR_OF(cListNode, n);
                vt[12].fn(vthis(n, vt[12]));
            }
            cUIState_updateScreens(n);
            n = n->mpNext;
        } while (!cListNode_isSentinel(n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIStateStack_pop);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
cListNode* cList_first(cList*);

struct cUIStateStack_pop_node {
    char pad0[0x4];
    cUIStateStack_pop_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int b0 : 6;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

static inline void cUIStateStack_pop_setId(cUIStateStack_pop_node* n, int v) { n->id = v; }
static inline void cUIStateStack_pop_setB7(cUIStateStack_pop_node* n, int v) { n->b7 = v; }

/**
 * Closes the top @p count states of a state stack: sets each to mode 6 (losing) and pending delete.
 *
 * Popups and front-end states call it with 1 to close themselves; the states then leave through cUIStateStack_updateLoop.
 *
 * @param list  the state stack (its main list)
 * @param count  how many states to close
 * @return the state below the closed ones, or null
 */
extern "C" cUIStateStack_pop_node* cUIStateStack_pop(cUIStateStack* list, int count)
{
    cUIStateStack_pop_node* n = (cUIStateStack_pop_node*)cList_first(((cList*)list));
    int i;
    for (i = 0; i < count; i++) {
        if (n == 0) {
            goto done;
        }
        cUIStateStack_pop_setId(n, 6);
        cUIStateStack_pop_setB7(n, 1);
        n = n->next;
    }
done:
    return n;
}
#endif


//100%
INCLUDE_ASM("ui/uistate", cUIStateStack_pop1);
#ifdef SKIP_ASM
/**
 * Closes one active state: when @p node is enabled and in mode 5 it becomes mode 6 (losing) and pending delete.
 *
 * Nothing in the retail code references it.
 *
 * @param node  the state to close
 */
extern "C" void cUIStateStack_pop1(cUIStateStack* self, cUIState* node)
{
    int flags = node->mAttrState;
    if (((flags >> 5) & 1) && (((unsigned int)flags >> 8) & 0x3F) == 5) {
        node->mAttrState = (flags & ~0x3F00) | 0x680;
    }
}
#endif

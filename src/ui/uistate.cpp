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
void cUIState::hideObjSafe(char* name)
{
    cListNode* n = cList_first((cList*)&mScreens);
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
void cUIState::showObjSafe(char* name)
{
    cListNode* n = cList_first((cList*)&mScreens);
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
 * Enables the first non-explicit state of mode @p id on a state stack: sets it visible, enabled and first-time, mode
 * kPENDINGCREATE.
 *
 * Modes are bits 8..13 of the state's attribute word (+0x1C); explicit states (bit 6) are skipped.
 * cUIStateStack_updateLoop uses it to bring up a kPENDINGNEXT or kNOFOCUS state when nothing is current.
 *
 * @param id  the mode to look for
 * @return true when a state was enabled, else false
 */
bool cUIStateStack::enableState(signed char id)
{
    cUIStateStack_enableState_node* n = (cUIStateStack_enableState_node*)cList_first(((cList*)this));
    if (n != 0) {
        do {
            if ((((unsigned int)n->flags >> 8) & 0x3F) == id && ((n->flags >> 6) & 1) == 0) {
                cUIStateStack_enableState_setB3(n, 1);
                cUIStateStack_enableState_setB5(n, 1);
                cUIStateStack_enableState_setId(n, cUIState::kPENDINGCREATE);
                cUIStateStack_enableState_setB4(n, 1);
                return true;
            }
            n = n->next;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    return false;
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
 * cUIStateStack_updateLoop uses it with kPENDINGNEXT when a popup closes.
 *
 * @param id  the mode to look for
 * @return true when one exists, else false
 */
bool cUIStateStack::checkForState(signed char id)
{
    cUIStateStack_checkForState_node* n = (cUIStateStack_checkForState_node*)cList_first(((cList*)this));
    if (n != 0) {
        do {
            if ((((unsigned int)n->flags >> 8) & 0x3F) == id && ((n->flags >> 6) & 1) == 0) {
                return true;
            }
            n = n->next;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIStateStack_render2D);
#ifdef SKIP_ASM
/**
 * Draws a state stack: calls virtual slot 15 (2D render) on every visible state of the main and explicit lists.
 *
 * Visible is bit 3 of a state's attribute word (+0x1C). cUIEngine_render2D calls it on the engine's state stack.
 */
void cUIStateStack::render2D()
{
    cUIState* n = (cUIState*)cList_first((cList*)this);
    if (((char*)n) != 0) {
        do {
            if ((n->mAttrState >> 3) & 1) {
                n->onRender2D();
            }
            n = (cUIState*)n->mpNext;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    n = (cUIState*)cList_first((cList*)&mExplicitStack);
    if (((char*)n) != 0) {
        do {
            if ((n->mAttrState >> 3) & 1) {
                n->onRender2D();
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
void cUIStateStack::render3D()
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
 * By mode (cUIState::tUIStateMode, attribute bits 8..13): kPENDINGCREATE calls virtual slot 4 and becomes
 * kGAINTRANSITION (a popup also shows its parent chain with cUIStateStack_setVisibleToChain and sends the parent
 * kONPOPUPCREATE through slot 17); kGAINTRANSITION waits for slot 7 (passed the first-time bit) to return nonzero, then
 * kGAINFOCUS; kGAINFOCUS calls slot 6 and becomes kUPDATE; kUPDATE calls slot 12 each pass; kLOSETRANSITION waits for
 * slot 10, then kLOSEFOCUS; kLOSEFOCUS calls slots 11 and 5, becomes kNOFOCUS and is disabled. A closing popup is
 * marked pending delete, lowers its parent's popup count (re-enabling the parent at 0) and sends it kONPOPUPDESTROY;
 * but when a kPENDINGNEXT state exists (checked unless the push-check bit 14 is set) it only hides its parent chain. A
 * pending-delete state is unlinked and deleted (slot 1 with 3), others are hidden. When nothing is current afterwards,
 * a kPENDINGNEXT state (or else a kNOFOCUS one) is enabled with cUIStateStack_enableState. Changed states run
 * cUIState_updateScreens.
 *
 * @return nonzero when a state finished closing and none started in this pass (cUIStateStack_update then runs another pass)
 */
int cUIStateStack::updateLoop()
{
    int popped = 0;
    int any = 0;
    cUIStateStack_updateLoop_node* n = (cUIStateStack_updateLoop_node*)cList_first((cList*)this);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            cUIStateStack_updateLoop_node* next = n->next;
            if ((n->flags >> 5) & 1) {
                switch ((signed char)n->bits.id) {
                case cUIState::kPENDINGCREATE:
                    any = 1;
                    VCALLV_ECB0(n, 4);
                    n->bits.id = cUIState::kGAINTRANSITION;
                    cUIState_updateScreens(n);
                    if (n->flags & 1) {
                        cUIStateStack_updateLoop_node* o = n->owner;
                        if (o != 0) {
                            cUIStateStack_setVisibleToChain(this, o, 1, 1);
                            VCALLV_ECB0(o, 17, n, cUIState::kONPOPUPCREATE);
                        }
                    }
                case cUIState::kGAINTRANSITION:
                    if (VCALL_ECB0(n, 7, (n->flags >> 4) & 1)) {
                        n->bits.id = cUIState::kGAINFOCUS;
                    }
                    n->bits.b4 = 0;
                    cUIState_updateScreens(n);
                    break;
                case cUIState::kGAINFOCUS:
                    if ((n->flags >> 4) & 1) {
                        VCALL_ECB0(n, 7, 1);
                        n->bits.b4 = 0;
                    }
                    VCALLV_ECB0(n, 6, 0);
                    n->bits.id = cUIState::kUPDATE;
                    cUIState_updateScreens(n);
                    break;
                case cUIState::kUPDATE:
                    VCALLV_ECB0(n, 12);
                    cUIState_updateScreens(n);
                    break;
                case cUIState::kLOSETRANSITION:
                    if (VCALL_ECB0(n, 10)) {
                        n->bits.id = cUIState::kLOSEFOCUS;
                    }
                    n->bits.b4 = 1;
                    cUIState_updateScreens(n);
                    break;
                case cUIState::kLOSEFOCUS: {
                    int top;
                    int found = 0;
                    popped = 1;
                    VCALLV_ECB0(n, 11, 0);
                    VCALLV_ECB0(n, 5);
                    n->bits.id = cUIState::kNOFOCUS;
                    n->bits.b5 = 0;
                    if ((n->flags >> 7) & 1) {
                        cListNode_removeFromList(n);
                    }
                    top = n->flags & 1;
                    if (top) {
                        cUIStateStack_updateLoop_node* o = n->owner;
                        popped = 0;
                        if (((n->flags >> 14) & 1) == 0) {
                            found = checkForState(cUIState::kPENDINGNEXT);
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
                                VCALLV_ECB0(o, 17, n, cUIState::kONPOPUPDESTROY);
                            }
                        } else if (o != 0) {
                            cUIStateStack_setVisibleToChain(this, o, 0, 1);
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
                        if (cUIStateStack_getCurrentState(this) == 0 && !enableState(cUIState::kPENDINGNEXT)) {
                            enableState(cUIState::kNOFOCUS);
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
void cUIStateStack::update()
{
    if (updateLoop() != 0) {
        updateLoop();
    }
    cListNode* n = cList_first((cList*)&mExplicitStack);
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
 * Closes one active state: when @p node is enabled and in kUPDATE it becomes kLOSETRANSITION and pending delete.
 *
 * Nothing in the retail code references it.
 *
 * @param node  the state to close
 */
void cUIStateStack::pop1(cUIState* node)
{
    int flags = node->mAttrState;
    if (((flags >> 5) & 1) && (((unsigned int)flags >> 8) & 0x3F) == cUIState::kUPDATE) {
        node->mAttrState = (flags & ~0x3F00) | 0x680;
    }
}
#endif

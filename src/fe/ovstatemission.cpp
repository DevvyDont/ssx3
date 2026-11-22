#include "vtable.h"
#include "common.h"
#include "ui/uithing.h"
#include "fe/feasyncmanager.h"
#include "ui/uiengine.h"
#include "fe/festatempcircuit.h"

//100%
INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
// PORT: cFEStateMPCircuit_initCircuitList__FPv is called with (self, obj) here; bind the 2-arg form to that symbol.
void cFEStateMPCircuit_initCircuitList_2(void* self, void* obj) __asm__("cFEStateMPCircuit_initCircuitList__FPv");
extern char str_108cct_select[];
extern char str_ListBox[];

/**
 * Creates the multiplayer circuit select screen ("108cct_select") and sets up its "ListBox".
 *
 * Sets the list box's flag 0x80, clears flag 1, and fills it with cFEStateMPCircuit_initCircuitList().
 */
extern "C" void cFEStateMPCircuit_onCreateScreen(cFEStateMPCircuit* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_108cct_select), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIThing* obj = (cUIThing*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ListBox));
    if (((void*)obj) != 0) {
        obj->mAttr = (obj->mAttr & ~1) | 0x80;
        cFEStateMPCircuit_initCircuitList_2(self, obj);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_onWidgetCreate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the multiplayer-circuit state's onWidgetCreate has an empty body.
 */
void cFEStateMPCircuit_onWidgetCreate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_onGainFocus);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" char* cUIListBox_getCurrentAsciiText(void* self);
extern "C" void cUIState_onGainFocus(void* self, int a1);
void cFEStateMPCircuit_updateWidget(void* self);
extern char str_ListBox[];

/**
 * Plays the screen frame named after the list box's current entry, then gives the state focus.
 *
 * cFEStateMPCircuit_updateWidget() is empty in this build.
 *
 * @param pPreSt  the state that had focus before (On Tour pPreSt), passed on to cUIState_onGainFocus()
 */
extern "C" void cFEStateMPCircuit_onGainFocus(cFEStateMPCircuit* self, int pPreSt)
{
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ListBox));
    if (obj != 0) {
        int frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(cUIListBox_getCurrentAsciiText(obj)));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(self->mpScreen, frame, 1);
        }
    }
    cFEStateMPCircuit_updateWidget(self);
    cUIState_onGainFocus(self, pPreSt);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_onWidgetEvent);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" char* cUIListBox_getCurrentAsciiText(void* self);
extern "C" void cUIStateStack_push(void* list, void* item);
void cFEStateMPCircuit_updateWidget(void* self);


/**
 * Handles the circuit select's widget events: select or event 6 push the factory's state, a change plays the
 * entry's frame.
 *
 * @param item  the widget (its index at +0x18), or 0
 * @param msg  widget event number (On Tour Event)
 */
extern "C" void cFEStateMPCircuit_onWidgetEvent(cFEStateMPCircuit* self, cUIThing* item, unsigned int msg)
{
    if (((void*)item) == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        cUICustomInterface* obj = (cUICustomInterface*)(*(void**)self->mpEngine);
        sVEntry<void*(void*, void*, int)>* vt = (sVEntry<void*(void*, void*, int)>*)obj->mVtbl;
        void* nextState = vt[4].fn(vthis(obj, vt[4]), self, item->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        }
        break;
    }
    case 6: {
        cUICustomInterface* obj = (cUICustomInterface*)(*(void**)self->mpEngine);
        sVEntry<void*(void*, void*, int)>* vt = (sVEntry<void*(void*, void*, int)>*)obj->mVtbl;
        void* nextState = vt[5].fn(vthis(obj, vt[5]), self, item->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        }
        break;
    }
    case 9: {
        int frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(cUIListBox_getCurrentAsciiText(item)));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(self->mpScreen, frame, 1);
        }
        cFEStateMPCircuit_updateWidget(self);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_initCircuitList__FPv);
#ifdef SKIP_ASM
/**
 * Fills the multiplayer circuit select's list box; empty in this build.
 *
 * cFEStateMPCircuit_onCreateScreen calls it with the screen's "ListBox" widget as a second argument, which this
 * definition does not declare.
 */
void cFEStateMPCircuit_initCircuitList(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_updateWidget__FPv);
#ifdef SKIP_ASM
/**
 * Refreshes the multiplayer circuit select after its selection changes; empty in this build.
 *
 * Called by cFEStateMPCircuit_onGainFocus and by cFEStateMPCircuit_onWidgetEvent for a list change (event 9), each
 * after playing the frame of the current entry.
 */
void cFEStateMPCircuit_updateWidget(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_getCircuitEvent);
#ifdef SKIP_ASM
/**
 * Stub that always returns -1; nothing in the program calls it.
 *
 * @return -1
 */
extern "C" int cFEStateMPCircuit_getCircuitEvent(void) {
    return -1;
}
#endif
//100%
INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_getTopUnlockedPeakNum);
#ifdef SKIP_ASM
extern "C" int cBENewPlayerInterface_isPeakLocked1(void*, int, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void*, int);

/**
 * Returns the highest count, over the two human riders, of peaks 1 and 2 that carry the peak flag
 * cBENewPlayerInterface_isPeakLocked1 reports.
 *
 * For the first and second human (cBENewPlayerInterface_getHumanRider on back-end interface 1) it adds
 * cBENewPlayerInterface_isPeakLocked1 for peaks 1 and 2 and keeps the largest sum (0-2). That interface's doc calls the
 * flag "locked"; for a function shaped like this the count reads more naturally as the number of unlocked peaks, so one
 * of the two may be named the wrong way round. Nothing in the program calls it.
 *
 * @return the largest per-rider count, 0-2
 */
extern "C" unsigned int cFEStateMPCircuit_getTopUnlockedPeakNum(void)
{
    signed char i;
    unsigned char best = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    for (i = 0; i < 2; i++) {
        int rider = cBENewPlayerInterface_getHumanRider(iface, i != 0);
        unsigned char cnt = 0;
        signed char j;
        for (j = 1; j < 3; j++) {
            cnt += cBENewPlayerInterface_isPeakLocked1(iface, rider, j);
        }
        if (best < cnt) {
            best = cnt;
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEAsyncManager_cFEAsyncManager);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(char* name);
extern "C" void FILESYS_addbigsync(char* name, int a1, int a2, void* out);
extern char str_data_ui_mapgfx_big[];

/**
 * Constructs the front end's async manager, opening data/ui/mapgfx.big when it exists (handle at +0x58C).
 *
 * @return @p self
 */
extern "C" void* cFEAsyncManager_cFEAsyncManager(cFEAsyncManager* self)
{
    if (BXFILE_exists(str_data_ui_mapgfx_big) != 0) {
        FILESYS_addbigsync(str_data_ui_mapgfx_big, 0, 100, (char*)&self->mMapGfxBigFile);
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEAsyncManager__cFEAsyncManager);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(char* name);
extern "C" void FILESYS_delbigsync(void* handle, int arg);
void operator_delete(int* p);
extern char str_data_ui_mapgfx_big[];

/**
 * Destroys the async manager: closes data/ui/mapgfx.big if it exists, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEAsyncManager__cFEAsyncManager(cFEAsyncManager* self, int flags)
{
    if (BXFILE_exists(str_data_ui_mapgfx_big) != 0) {
        FILESYS_delbigsync((void*)self->mMapGfxBigFile, 100);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", cFEAsyncManager_Init);
#ifdef SKIP_ASM
struct sMissionSlot {
    char name[0x100];
    int f100;
    int f104;
    int f108;
    int f10C;
    int f110;
    int f114;
    int f118;
};

extern unsigned char D_004A1408[];

/**
 * Resets the front-end async file manager's five file slots to empty.
 *
 * Each 0x11C-byte slot gets an empty name, a null data pointer, handle -1, 1 at 0x108, size 0, id -1, status 0
 * (idle) and -1 at 0x118.
 */
extern "C" void cFEAsyncManager_Init(cFEAsyncManager* self)
{
    int i;
    for (i = 0; i < 5; i++) {
        unsigned char nulChar = D_004A1408[0];
        ((sMissionSlot*)self)[i].f100 = 0;
        ((sMissionSlot*)self)[i].name[0] = nulChar;
        ((sMissionSlot*)self)[i].f104 = -1;
        ((sMissionSlot*)self)[i].f108 = 1;
        ((sMissionSlot*)self)[i].f10C = 0;
        ((sMissionSlot*)self)[i].f110 = -1;
        ((sMissionSlot*)self)[i].f114 = 0;
        ((sMissionSlot*)self)[i].f118 = -1;
    }
}
#endif


//100%
INCLUDE_ASM("fe/ovstatemission", cFEAsyncManager_UnloadAll);
#ifdef SKIP_ASM
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(void* mgr, int slot);

/**
 * Unloads all five front-end async file slots (cFEAsyncManager_UnloadFEAsyncFile on 0-4).
 *
 * Nothing in the program calls it.
 *
 * @param mgr  the front-end async manager (this)
 */
extern "C" void cFEAsyncManager_UnloadAll(cFEAsyncManager* mgr)
{
    for (int i = 0; i < 5; i++) {
        cFEAsyncManager_UnloadFEAsyncFile(mgr, i);
    }
}
#endif

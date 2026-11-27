#include "vtable.h"
#include "common.h"
#include "ui/uiengine.h"
#include "fe/festateselectmp.h"
#include "ui/uithing.h"
#include "fe/festatempcircuit.h"

//100%
INCLUDE_ASM("fe/festateselectmp", cFEStateSelectMultiplayerMode_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char str_86sel_multi[];

/**
 * Builds the select-multiplayer-mode screen: adds "86sel_multi" to the state and plays its frame 0.
 *
 * The new screen is kept in mpScreen (+0x40); the frame is played only when the screen was created.
 */
void cFEStateSelectMultiplayerMode::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_86sel_multi), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateselectmp", cFEStateSelectMultiplayerMode_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A14F8[];
extern char D_004A1500[];

/**
 * Tags the multiplayer-mode choices as they are created: the widget named "1" gets misc data 0, "2" gets 1.
 *
 * The value (mMiscData, +0x18) is what onWidgetEvent() hands to the engine's custom interface.
 *
 * @param item  the widget just created (On Tour pWidget)
 */
void cFEStateSelectMultiplayerMode::onWidgetCreate(cUIThing* item)
{
    int id = item->mHashName;
    if (id == GetHashValue32(D_004A14F8)) {
        item->mMiscData = 0;
    } else {
        int id2 = item->mHashName;
        if (id2 == GetHashValue32(D_004A1500)) {
            item->mMiscData = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateselectmp", cFEStateSelectMultiplayerMode_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_push(void* list, void* item);

/**
 * Handles the select-multiplayer-mode screen's widget events by moving to the next or previous state.
 *
 * kONNEXT calls the engine's custom interface getNextState (vtable slot 4, cFECustom_getNextState), kONPREV
 * its slot 5 (probably getPrevState), each with this state and the widget's misc data; a state returned is
 * pushed on the engine's state stack (+0x18). A null widget is ignored.
 *
 * @param item  the widget (On Tour pWidget)
 * @param msg  the event (On Tour Event)
 */
void cFEStateSelectMultiplayerMode::onWidgetEvent(cUIThing* item, unsigned int msg)
{
    if (((void*)item) == 0) {
        return;
    }
    switch (msg) {
    case cUIState::kONNEXT: {
        cUICustomInterface* obj = mpEngine->mpCustom;
        void* nextState = obj->getNextState((cUIState*)this, item->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&mpEngine->mStateStack, nextState);
        }
        break;
    }
    case cUIState::kONPREV: {
        cUICustomInterface* obj = mpEngine->mpCustom;
        void* nextState = obj->getPrevState((cUIState*)this, item->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&mpEngine->mStateStack, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateselectmp", cFEStateMPCircuit_cFEStateMPCircuit);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateMPCircuit_vtbl[];

/**
 * Constructs the multiplayer circuit state (allocated as "cFEStateMPCircuit", 72 bytes).
 *
 * Runs cUIState's constructor (cUIState_cUIState; the engine argument reaches it untouched in the second argument
 * register), then stores cFEStateMPCircuit_vtbl and mHashName 23.
 *
 * @return @p self
 */
extern "C" void* cFEStateMPCircuit_cFEStateMPCircuit(cFEStateMPCircuit* self)
{
    cUIState_cUIState(self);
    VPTR_OF(cListNode, self) = cFEStateMPCircuit_vtbl;
    self->mHashName = 23;
    return ((void*)self);
}
#endif


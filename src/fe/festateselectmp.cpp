#include "common.h"

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
extern "C" void cFEStateSelectMultiplayerMode_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_86sel_multi), 0);
    *(void**)((char*)self + 0x40) = screen;
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
extern "C" void cFEStateSelectMultiplayerMode_onWidgetCreate(void* self, void* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(D_004A14F8)) {
        *(int*)((char*)item + 0x18) = 0;
    } else {
        int id2 = *(int*)((char*)item + 0x38);
        if (id2 == GetHashValue32(D_004A1500)) {
            *(int*)((char*)item + 0x18) = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateselectmp", cFEStateSelectMultiplayerMode_onWidgetEvent);
#ifdef SKIP_ASM
struct sVEntry001A3160 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void cUIStateStack_push(void* list, void* item);

/**
 * Handles the select-multiplayer-mode screen's widget events by moving to the next or previous state.
 *
 * Event 5 calls the engine's custom interface getNextState (vtable slot 4, cFECustom_getNextState), event 6
 * its slot 5 (probably getPrevState), each with this state and the widget's misc data; a state returned is
 * pushed on the engine's state stack (+0x18). A null widget is ignored.
 *
 * @param item  the widget (On Tour pWidget)
 * @param msg  the event (On Tour Event)
 */
extern "C" void cFEStateSelectMultiplayerMode_onWidgetEvent(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001A3160* vt = *(sVEntry001A3160**)((char*)obj + 4);
        void* nextState = vt[4].fn((char*)obj + vt[4].delta, self, *(int*)((char*)item + 0x18));
        if (nextState != 0) {
            cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        }
        break;
    }
    case 6: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001A3160* vt = *(sVEntry001A3160**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (nextState != 0) {
            cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateselectmp", func_001A3218);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* cFEStateMPCircuit_vtbl[];

extern "C" void* func_001A3218(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = cFEStateMPCircuit_vtbl;
    *(int*)((char*)self + 0xC) = 23;
    return self;
}
#endif


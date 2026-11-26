#ifndef UI_UISTATESTACK_H
#define UI_UISTATESTACK_H

// cUIStateStack: the UI engine's stack of cUIState screens (cUIEngine::mStateStack, ui/uiengine.h): the normal stack
// and the explicit (popup / special) stack. Methods in ui/uistatestack and ui/uistate: push, pushPopup, pushSpecial,
// pushExplicit, pop, popToState, findState, getTopState, render2D / render3D.
//
// Size 0x38 (On Tour's; it fills cUIEngine 0x18-0x4F, whose layout is verified). A FLAT layout, like main/game.h:
// member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this`
// in the methods falls on a member start (tools/gen/self_offsets.py).

#include "layout.h"
#include "util/list.h"


struct cUIState;

struct cUIStateStack {
    cList mStack;                   // 0x000 [O]
    cList mExplicitStack;           // 0x01C [O]

    // Methods bound to their retail symbols by asm labels.
    bool enableState(signed char id) __asm__("cUIStateStack_enableState");
    bool checkForState(signed char id) __asm__("cUIStateStack_checkForState");
    void render2D() __asm__("cUIStateStack_render2D");
    int updateLoop() __asm__("cUIStateStack_updateLoop");
    void update() __asm__("cUIStateStack_update");
    void pop1(cUIState* node) __asm__("cUIStateStack_pop1");
    void pushExplicit(cUIState* state) __asm__("cUIStateStack_pushExplicit");
    void setVisibleToChain(cUIState* state, int on, int last) __asm__("cUIStateStack_setVisibleToChain");
    void purgeNormalStates() __asm__("cUIStateStack_purgeNormalStates");
    void destroyStates() __asm__("cUIStateStack_destroyStates");
    void* findState(int id) __asm__("cUIStateStack_findState");
    void* getCurrentState() __asm__("cUIStateStack_getCurrentState");
    void* getTopState() __asm__("cUIStateStack_getTopState");
    void render3D() __asm__("cUIStateStack_render3D__FPv");
};

CHECK_OFFSET(cUIStateStack, mStack, 0x000);
CHECK_OFFSET(cUIStateStack, mExplicitStack, 0x01C);
CHECK_SIZE(cUIStateStack, 0x38);

#endif

#ifndef FE_FESTATEOPTIONSGAME_H
#define FE_FESTATEOPTIONSGAME_H
#pragma interface

// cFEStateOptionsGame: a front-end state (a cFEState), 6 methods; allocated with cMemMan_alloc(76, "cFEStateOptionsGame").
// Size 0x4C [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateOptionsGame : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateOptionsGame
    char mLanguageSlot;             // 0x048 [O]
    char mSpeedSlot;                // 0x049 [O]
    char mScreenSlot;               // 0x04A [O]
    char unk_04B[0x1];              // 0x04B padding

    // Methods bound to their retail symbols by asm labels.
    void updateWidget() __asm__("cFEStateOptionsGame_updateWidget");
    virtual ~cFEStateOptionsGame();   // g++'s own symbol _$_19cFEStateOptionsGame (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateOptionsGame_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* widget, unsigned int event) __asm__("cFEStateOptionsGame_onWidgetEvent");
    virtual void onPopupEvent(cUIState* popup, unsigned int event) __asm__("cFEStateOptionsGame_onPopupEvent");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
        __asm__("cFEStateOptionsGame_onWidgetMenu");
    virtual int onWidgetPair(cUIPair* pair, unsigned int event) __asm__("cFEStateOptionsGame_onWidgetPair");
    virtual void onGainFocus(cUIState*) __asm__("cFEStateOptionsGame_onGainFocus__FPv");
};

CHECK_OFFSET(cFEStateOptionsGame, mLanguageSlot, 0x048);
CHECK_OFFSET(cFEStateOptionsGame, mSpeedSlot, 0x049);
CHECK_OFFSET(cFEStateOptionsGame, mScreenSlot, 0x04A);
CHECK_SIZE(cFEStateOptionsGame, 0x4C);
__asm__("_vt$19cFEStateOptionsGame = cFEStateOptionsGame_vtbl");   // g++'s name for the retail vtable

#endif

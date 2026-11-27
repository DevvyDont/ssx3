#ifndef FE_FESTATEOPTIONSGAME_H
#define FE_FESTATEOPTIONSGAME_H

// cFEStateOptionsGame: a front-end state (a cUIState), 6 methods; allocated with cMemMan_alloc(76, "cFEStateOptionsGame").
// Size 0x4C [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateOptionsGame : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateOptionsGame
    char mLanguageSlot;             // 0x048 [O]
    char mSpeedSlot;                // 0x049 [O]
    char mScreenSlot;               // 0x04A [O]
    char unk_04B[0x1];              // 0x04B padding

    // Methods bound to their retail symbols by asm labels.
    void updateWidget() __asm__("cFEStateOptionsGame_updateWidget");
    // retail's destructor cFEStateOptionsGame__cFEStateOptionsGame stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateOptionsGame();
    virtual void onCreateScreen() __asm__("cFEStateOptionsGame_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* widget, unsigned int event) __asm__("cFEStateOptionsGame_onWidgetEvent");
    virtual void onPopupEvent(cUIState* popup, unsigned int event) __asm__("cFEStateOptionsGame_onPopupEvent");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
        __asm__("cFEStateOptionsGame_onWidgetMenu");
    virtual int onWidgetPair(cUIPair* pair, unsigned int event) __asm__("cFEStateOptionsGame_onWidgetPair");
};

CHECK_OFFSET(cFEStateOptionsGame, mLanguageSlot, 0x048);
CHECK_OFFSET(cFEStateOptionsGame, mSpeedSlot, 0x049);
CHECK_OFFSET(cFEStateOptionsGame, mScreenSlot, 0x04A);
CHECK_SIZE(cFEStateOptionsGame, 0x4C);

#endif

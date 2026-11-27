#ifndef FE_OVSTATEPAUSE_H
#define FE_OVSTATEPAUSE_H

// The in-game pause menus (EA's d:/ssx/code/source/fe/ovstatepause.h: the preview's assert strings name it next to
// "cOVTemplate_PauseMenu"; retail code in the decomp's units fe/ovtemplatepausemenu.cpp and fe/ovstatepause.cpp,
// constructors/destructors in fe/all_in_one_fe.cpp). Three layouts:
//
// cOVTemplate_PauseMenu: the pause-menu template (screen "31paus_freeride": up to 8 option rows, the PDA icons,
// the new-mail pulse). Retail vtable cOVTemplate_PauseMenu_vtbl (0x473170); constructors
// cOVTemplate_PauseMenu_cOVTemplate_PauseMenu(engine, id) and func_002214F8 (no engine), destructor
// cOVTemplate_PauseMenu__cOVTemplate_PauseMenu. Size 0x114: every subclass is allocated with
// cMemMan_alloc(0x114, "cOVStateOverlay") (overlay ids 1-6) and nothing touches a word past 0x110.
// Its six subclasses add no members (On Tour: all 0xD8 = cOVTemplate_PauseMenu's size; retail: the same 0x114
// allocation, constructors that only swap the vptr) and override only onCreateScreen, which fills
// mPauseMenuOptions / mPauseMenuNumOptions / mPauseMenuType and calls cOVTemplate_PauseMenu_onCreateScreen:
//   cOVState_PAUSE_COMPETE   id 1  vtbl 0x473098      cOVState_PAUSE_BIGCHAL   id 2  vtbl 0x472FC0
//   cOVState_PAUSE_FREERIDE  id 3  vtbl 0x472EE8      cOVState_PAUSE_SPOKE     id 4  vtbl 0x472E10
//   cOVState_PAUSE_QUICKPLAY id 5  vtbl 0x472D38      cOVState_PAUSE_ONLINE    id 6  vtbl 0x472C60
// so their methods take a cOVTemplate_PauseMenu* (no separate structs).
//
// cOVState_PAUSE_OPTIONS: the in-game options screen (overlay id 7). No members of its own: allocated 0x9C,
// On Tour size 0x90 (= cOVState). Vtable cOVState_PAUSE_OPTIONS_vtbl (0x472B88).
//
// cOVState_PAUSE_ONLINE_ERROR: the online "connection problem" pause (overlay id 39, screen with Continue /
// Cancel and a ping status). Allocated 0xA4; vtable cOVState_PAUSE_ONLINE_ERROR_vtbl (0x472AB0); constructors
// cOVState_PAUSE_ONLINE_ERROR_cOVState_PAUSE_ONLINE_ERROR (stores 0xA0 = 1, 0x9C = 0) and func_00221C80.
//
// All three derive from cOVState (fe/ovstate.h, 0x9C).
//
// Derives from cOVState (fe/ovstate.h); its own members at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovtemplatepausemenu.cpp / ovstatepause.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIThread;

struct cOVTemplate_PauseMenu : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVTemplate_PauseMenu_vtbl (or a subclass's), stored after cOVState_vtbl by the constructors
    // cOVTemplate_PauseMenu's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName; [R] zeroed by the cUIState ctor; [U] onCreateScreen stores
    //       GetHashValue32("cOVTemplate_PauseMenu")
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] unused by the template (it keeps its own mpPauseScreen at 0xC0)
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVTemplate_PauseMenu (On Tour: size 0xD8)
    int mPauseMenuOptions[8];       // 0x09C [O]; [U] the subclasses' onCreateScreen copy the option ids here;
                                    //       onCreateScreen / onGainTransition / onGainFocus index row i
    int mPauseMenuNumOptions;       // 0x0BC [O]; [U] set to 5..7 (2/3 online) by the subclasses' onCreateScreen;
                                    //       rows i >= it are hidden
    cUIScreen* mpPauseScreen;       // 0x0C0 [O] cOVTemplate_PauseMenu::mpScreen (it hides cUIState::mpScreen at
                                    //       0x40; renamed here); [U] onCreateScreen stores
                                    //       cUIEngine_addScreenByHashName("31paus_freeride"); every lookup goes
                                    //       through it
    cUIThread* mpThread;            // 0x0C4 [O] (On Tour onDestroyScreen clears it: sw zero,0xB8); [R] constructors
                                    //       zero it; [U] setPDAIconColor keeps cUIScreen_playFrame's result here;
                                    //       onDestroyScreen clears it
    float mNewMailAlpha;            // 0x0C8 [O]; [U] onUpdate pulses it between 0 and 1 and sets the "NewMail"
                                    //       icon's alpha; onCreateScreen zeroes it (sw $zero)
    float mNewMailAlphaDiff;        // 0x0CC [O]; [U] onCreateScreen stores 0.05f; onUpdate negates it at the ends
    int mScreenExiting;             // 0x0D0 [O] bool; [U] set by onWidgetEvent (option chosen) and onUpdate (online
                                    //       timeout) when "trans_out" plays; onWidgetEvent ignores input while it is
                                    //       set
    int mMoveIcons;                 // 0x0D4 [O] bool; [U] onCreateScreen sets it, onGainTransition lays the PDA icon
                                    //       groups out once and clears it
    int mPauseOptionUnlocked[14];   // 0x0D8 [O] (On Tour bool[13]); [U] onCreateScreen sets 14 words (option ids
                                    //       0-13) to 1, then clears the ones not allowed; onGainFocus /
                                    //       onWidgetEvent index it by option id
    int mPauseMenuType;             // 0x110 [O] tOVStateEnum; [U] the subclasses' onCreateScreen store their overlay
                                    //       id (1-6); onUpdate pulses the mail icon for 1, 2 and 4

    // Methods bound to their retail symbols by asm labels.
    void setPDAIconColor(int idx, int on) __asm__("cOVTemplate_PauseMenu_setPDAIconColor");
    // retail's destructor cOVTemplate_PauseMenu__cOVTemplate_PauseMenu stays a C function (g++ 2.95 takes no asm label
    // on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVTemplate_PauseMenu();
    virtual bool onGainTransition(int) __asm__("cOVTemplate_PauseMenu_onGainTransition");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
        __asm__("cOVTemplate_PauseMenu_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int event) __asm__("cOVTemplate_PauseMenu_onWidgetEvent");
    virtual void onLoseFocus(cUIState*) __asm__("cOVTemplate_PauseMenu_onLoseFocus__FPv");
    virtual bool onInputBegin(cUIInput*) __asm__("cOVTemplate_PauseMenu_onInputBegin__FPv");
};

CHECK_OFFSET(cOVTemplate_PauseMenu, mPauseMenuOptions, 0x9C);
CHECK_OFFSET(cOVTemplate_PauseMenu, mPauseMenuNumOptions, 0xBC);
CHECK_OFFSET(cOVTemplate_PauseMenu, mpPauseScreen, 0xC0);
CHECK_OFFSET(cOVTemplate_PauseMenu, mpThread, 0xC4);
CHECK_OFFSET(cOVTemplate_PauseMenu, mNewMailAlpha, 0xC8);
CHECK_OFFSET(cOVTemplate_PauseMenu, mNewMailAlphaDiff, 0xCC);
CHECK_OFFSET(cOVTemplate_PauseMenu, mScreenExiting, 0xD0);
CHECK_OFFSET(cOVTemplate_PauseMenu, mMoveIcons, 0xD4);
CHECK_OFFSET(cOVTemplate_PauseMenu, mPauseOptionUnlocked, 0xD8);
CHECK_OFFSET(cOVTemplate_PauseMenu, mPauseMenuType, 0x110);
CHECK_SIZE(cOVTemplate_PauseMenu, 0x114);

// cOVState_PAUSE_OPTIONS: cOVState with no members of its own.
struct cOVState_PAUSE_OPTIONS : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_PAUSE_OPTIONS_vtbl, stored after cOVState_vtbl by cOVState_PAUSE_OPTIONS_cOVState_PAUSE_OPTIONS
    // cOVState_PAUSE_OPTIONS's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the options screen; every widget lookup goes through it
    // cOVState (fe/ovstate.h, 0x9C)

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_PAUSE_OPTIONS__cOVState_PAUSE_OPTIONS stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_PAUSE_OPTIONS();
    virtual void onPreRender2D() __asm__("cOVState_PAUSE_OPTIONS_onPreRender2D");
    virtual void onWidgetEvent(cUIThing* obj, unsigned int msg) __asm__("cOVState_PAUSE_OPTIONS_onWidgetEvent");
    virtual void onUpdate() __asm__("cOVState_PAUSE_OPTIONS_onUpdate");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int index)
        __asm__("cOVState_PAUSE_OPTIONS_onWidgetMenu");
    virtual int onWidgetListBox(cUIListBox* pListBox, unsigned int Event)
        __asm__("cOVState_PAUSE_OPTIONS_onWidgetListBox");
    virtual int onWidgetPair(cUIPair* pPair, unsigned int Event) __asm__("cOVState_PAUSE_OPTIONS_onWidgetPair");
};

CHECK_SIZE(cOVState_PAUSE_OPTIONS, 0x9C);

struct cUIThing;

struct cOVState_PAUSE_ONLINE_ERROR : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_PAUSE_ONLINE_ERROR_vtbl, stored after cOVState_vtbl by the constructors
    // cOVState_PAUSE_ONLINE_ERROR's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the screen; every widget lookup goes through it
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_PAUSE_ONLINE_ERROR (On Tour: size 0x98)
    unsigned int miStartPingTick;   // 0x09C [O]; [R] the constructor zeroes it; [U] onCreateScreen stores the app
                                    //       tick (cAppMan_TheAppMan + 0x18) when it pings the server; onUpdate tests
                                    //       it and clears it when the ping is answered or times out
    int mbShouldDisplayAuxInfo;     // 0x0A0 [O] bool; [R] the constructor stores 1; [U] onCreateScreen clears it for
                                    //       every error but the local cheat/delay ones; displayPingTimedOut /
                                    //       displayPingReceived test it

    // EA's eOption: the option widgets' mMiscData. [O] On Tour enum cOVState_PAUSE_ONLINE_ERROR::eOption, names and
    // values; [U] onCreateScreen gives "Text_CancelGame" 0, setContinueOptionVisible gives the continue option 4, and
    // doContinueCancel continues on 4 and quits otherwise.
    enum eOption { eOptionCancel = 0, eOptionContinue = 4 };

    // Methods bound to their retail symbols by asm labels.
    void enableCancelButton(bool on) __asm__("cOVState_PAUSE_ONLINE_ERROR_enableCancelButton");
    void displayPingTimedOut() __asm__("cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut");
    void displayPingReceived() __asm__("cOVState_PAUSE_ONLINE_ERROR_displayPingReceived");
    void doContinueCancel(cUIThing* item, int msg) __asm__("cOVState_PAUSE_ONLINE_ERROR_doContinueCancel");
    void setContinueOptionVisible(int visible) __asm__("cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible");
    void setContinueHighLight(int on) __asm__("cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight");
    void setCancelHighLight(int on) __asm__("cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight");
    // retail's destructor cOVState_PAUSE_ONLINE_ERROR__cOVState_PAUSE_ONLINE_ERROR stays a C function (g++ 2.95 takes
    // no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_PAUSE_ONLINE_ERROR();
    virtual void onUpdate() __asm__("cOVState_PAUSE_ONLINE_ERROR_onUpdate");
    virtual void onWidgetEvent(cUIThing* item, unsigned int msg) __asm__("cOVState_PAUSE_ONLINE_ERROR_onWidgetEvent");
};

CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, miStartPingTick, 0x9C);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mbShouldDisplayAuxInfo, 0xA0);
CHECK_SIZE(cOVState_PAUSE_ONLINE_ERROR, 0xA4);

#endif /* FE_OVSTATEPAUSE_H */

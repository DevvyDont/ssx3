#ifndef FE_FESTATEPROFILE_H
#define FE_FESTATEPROFILE_H

// cFEStateProfileLoad: the front end's profile load / save / create screen ("93profile_load": six profile slot rows,
// the card panel, the name keyboard, save / delete), a cFEMemCard with mType 0. What it is opened for is the class
// static cFEStateProfileLoad::mScreenAction (cFEStateProfileLoad_mScreenAction, set by the ctor from iType), not a
// member.
// EA's source is fe/festateprofile.cpp (the July 2003 preview's assert strings; builds/x/v2 file_hints for retail
// cFEStateProfileLoad_onRender2D__FPv). Retail code: fe/festateprofile (0x0018ECA0 ... 0x00190768) and
// main/all_in_one_main (the destructor cFEStateProfileLoad__cFEStateProfileLoad, the engine-less ctor copy
// func_001DE240 and the setter cFEStateProfileLoad_setQuitPath).
//
// Size 0x280:
//   [R] cMemMan_alloc(640, "cFEStateProfileLoad") + cFEStateProfileLoad_cFEStateProfileLoad(mem, engine, iType,
//       ctrlMask) in cFECustom_getNextState (fe/fereal) and cFEStateCharSelect's handler (fe/festatecharselect); the
//       ctor writes 0x22C, 0x230, 0x234 and 0x27C (cFEStateProfileLoad_setQuitPath's word, "the last word of the object").
//   [O] On Tour 0x1B0 = cFEMemCard 0x164 + int miCntrlID, bool mIsOn, bool mIsHidden, unsigned short mTitle[32], int
//       mTypeOfQuit; retail: 0x22C + three words + 33 chars (0x42, kMCTitleStringSize as in cOVState_PROFILE,
//       fe/ovstateprofile.h) + 2 bytes of padding + a word = 0x280.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
// The ctor runs cFEMemCard_cFEMemCard2(this, pEngine, 0) and stores cFEStateProfileLoad_vtbl (D_0046BFD0, 47 slots: 1
// dtor, 4 onCreateScreen, 5 onDestroyScreen__FPv, 15 onRender2D__FPv, 19 onWidgetEvent, 20 onWidgetMenu, 26
// displayStatus, 33 updateDone, 37 hideActionButtons, 38 hideDeleteButton, 39 hideMainButton; the rest cFEMemCard's).
//
// Derives from cFEMemCard (fe/fememcard.h); its own members at their retail offsets. fe/fememcard.h has the inherited
// fields' evidence; only this class's own uses are noted here.
// Evidence tags:
//   [R] retail's constructors and allocation size;
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cFEStateProfileLoad / cFEMemCard): member names, types and order. On Tour's bools
//       are 1 byte (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).
// (No preview evidence: layout_xlate finds no paired cFEStateProfileLoad methods.)

#include "layout.h"
#include "ui/uistate.h"
#include "fe/fememcard.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cFEStateProfileLoad : public cFEMemCard {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateProfileLoad_vtbl, stored over cFEMemCard_vtbl by both ctors; the dtor stores it first
    // cFEStateProfileLoad's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "93profile_load"; onWidgetEvent builds the cKeyboardPopup for it
    //   mpScreen: [U] every widget lookup (cUIScreen_getObjectByHashName)
    // cOVState (fe/ovstate.h, 0x9C)
    // cFEStateProfileLoad's notes on cOVState's fields:
    //   mOVID: [O] tOVStateEnum; [R] 0 (cFEMemCard_cFEMemCard2)
    // cFEMemCard (fe/fememcard.h, 0x22C)
    // cFEStateProfileLoad's notes on cFEMemCard's fields:
    //   mPopupType: 0x19C [O]; [U] cFEStateProfileLoad_startSave sets 22 before the status popup; onWidgetEvent
    //   mProcessingKeyboard: [U] onWidgetEvent sets it when it pushes the name keyboard
    //   mpKeyboard: [U] onWidgetEvent creates the cKeyboardPopup once and reads its text; onDestroyScreen clears it
    //   mIsProcessing: 0x1A8 [O] (bool); [U] cFEStateProfileLoad_startSave sets it with the save; onWidgetEvent tests
    //       it
    //   mIsDone: [U] updateDone tests and sets it
    //   mType: [U] updateDone reads it
    //   mState: 0x1C0 [O]; [U] cFEStateProfileLoad_startSave sets 3 (saving); onWidgetEvent tests it
    //   mSlot: [U] onWidgetEvent stores the picked row
    //   mPlayerIdx: 0x1CC [O]; [R] the ctor clears it, then stores the player (0/1) using the opening controller and
    //       copies its low byte (lb) to mUserNum; [U] cFEStateProfileLoad_startSave passes it to
    //       cMCOverlayManager_writeSetFileType
    //   mIsSavePressed: 0x1DC [O] (bool); [U] cFEStateProfileLoad_startSave sets it with the save
    //   mIsCancelPressed: [U] onWidgetEvent sets it (back)
    //   mAutoLoadOn: 0x204 [O] (bool); [U] cFEStateProfileLoad_startSave skips the status popup when set
    //   mIsStatusPopupOn: 0x210 [O] (bool); [U] cFEStateProfileLoad_startSave tests and sets it
    //   mScreenStatus: [U] displayStatus / displayOn / onWidgetEvent test it
    //   mLastSelectedDevice: [U] displayOn stores the device it shows
    //   mDeleteButtonActive: [U] hideDeleteButton and the init*Screen functions store it
    //   mMainButtonActive: 0x224 [O] (bool); [U] hideMainButton and the init*Screen functions store it;
    //       cFEStateProfileLoad_startSave saves only while it is set
    // --- cFEStateProfileLoad
    int miCntrlID;                  // 0x22C [O]; [R] the index of the lowest set bit of the ctor's ctrlMask (of the
                                    //       first three); compared with cBENewPlayerInterface_getPlayerCtrlID
    int mIsOn;                      // 0x230 [O] (bool); [R] the ctor clears it (no other retail access)
    int mIsHidden;                  // 0x234 [O] (bool); [R] the ctor clears it; [U] hideActionButtons compares and
                                    //       stores the new hidden state
    unsigned short mTitle[33];      // 0x238 [O] ([32] in On Tour; 33 chars as cOVState_PROFILE::mTitle); [U] displayOn
                                    //       has the card (manager +0x434, slot 14) write each slot's title here and
                                    //       shows it (cUIText_setUnicodeString)
    char unk_27A[0x2];              // 0x27A padding
    int mTypeOfQuit;                // 0x27C [O]; [R] the ctor clears it; [U] the setter cFEStateProfileLoad_setQuitPath (0, 1 or 2);
                                    //       updateDone: 0 pushes the parent factory's next state, 1 starts a pre-game
                                    //       load, 2 returns to the title screen

    // What the screen is opened for (mScreenAction; names ours): [U] onCreateScreen runs initCreateScreen,
    // initLoadScreen or initSaveScreen for these (On Tour has the same three init methods and initDeleteScreen).
    enum { kScreenActionCreate = 1, kScreenActionLoad = 2, kScreenActionSave = 3 };
    // Where the screen goes when it closes (mTypeOfQuit; names ours): [U] updateDone pushes the previous state,
    // starts free ride in the last hub (cPreGameLoadScreen) or returns to the title screen.
    enum { kQuitToPrevious = 0, kQuitToHub = 1, kQuitToTitle = 2 };

    // Methods bound to their retail symbols by asm labels.
    void displayOn() __asm__("cFEStateProfileLoad_displayOn");
    void displayOff() __asm__("cFEStateProfileLoad_displayOff");
    void startSave() __asm__("cFEStateProfileLoad_startSave");
    void initGeneral() __asm__("cFEStateProfileLoad_initGeneral");
    void initCreateScreen() __asm__("cFEStateProfileLoad_initCreateScreen");
    void initLoadScreen() __asm__("cFEStateProfileLoad_initLoadScreen");
    void initSaveScreen() __asm__("cFEStateProfileLoad_initSaveScreen");
    void setQuitPath(int action) __asm__("cFEStateProfileLoad_setQuitPath");
    // retail's destructor cFEStateProfileLoad__cFEStateProfileLoad stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateProfileLoad();
    virtual void onCreateScreen() __asm__("cFEStateProfileLoad_onCreateScreen");
    virtual void hideActionButtons(int on) __asm__("cFEStateProfileLoad_hideActionButtons");
    virtual void displayStatus() __asm__("cFEStateProfileLoad_displayStatus");
    virtual void updateDone() __asm__("cFEStateProfileLoad_updateDone");
    virtual void onDestroyScreen() __asm__("cFEStateProfileLoad_onDestroyScreen__FPv");
    virtual int onWidgetMenu(cUIMenu* widget, unsigned int msg, unsigned int)
        __asm__("cFEStateProfileLoad_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* item, unsigned int event) __asm__("cFEStateProfileLoad_onWidgetEvent");
};

CHECK_OFFSET(cFEStateProfileLoad, miCntrlID, 0x22C);
CHECK_OFFSET(cFEStateProfileLoad, mIsOn, 0x230);
CHECK_OFFSET(cFEStateProfileLoad, mIsHidden, 0x234);
CHECK_OFFSET(cFEStateProfileLoad, mTitle, 0x238);
CHECK_OFFSET(cFEStateProfileLoad, unk_27A, 0x27A);
CHECK_OFFSET(cFEStateProfileLoad, mTypeOfQuit, 0x27C);
CHECK_SIZE(cFEStateProfileLoad, 0x280);


// ---------------------------------------------------------------------------------------------------------------------
// cFEStateProfileSelect: the front end's profile menu ("05sel_prof", state id 8): each entry's misc data goes to the
// custom interface (cFECustom, the engine's first word) for the next / previous state (slots 4 / 5); the entry under
// the cursor turns yellow (events 1 / 2 / 5 recolour it). Retail never constructs it (no caller of either ctor).
// EA's source: fe/festateprofile.cpp (see above). Retail code: the engine ctor
// cFEStateProfileSelect_cFEStateProfileSelect1 (splat puts it at the end of fe/festateoptions, 0x0018EA98),
// onCreateScreen / onWidgetEvent in fe/festateprofile (0x0018EAD8, 0x0018EB40), and the engine-less ctor and the dtor
// in main/all_in_one_main (0x001DE1B8, 0x001DE1F0).
//
// Size 0x48: [O] On Tour's class is 0x48 (no members); [R] the ctors write nothing past the cUIState part.
// Base: cFEState (fe/festate.h, no data) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The engine ctor
// runs cUIState_cUIState(this, pEngine) (its C definition declares only this), stores cFEStateProfileSelect_vtbl
// (0x0046C158, 24 slots: 1 dtor, 4 onCreateScreen, 6 cFEState_onGainFocus, 11 cFEState_onLoseFocus, 19 onWidgetEvent)
// and mHashName 8; the dtor stores cFEState_vtbl and runs cUIState__cUIState.
// Derives from cUIState (ui/uistate.h). Evidence tags: [R] retail's ctors / dtor; [U] how retail code uses the field;
// [O] SSX On Tour's DWARF (class cFEStateProfileSelect, cUIState): names, types and order.

struct cFEStateProfileSelect : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateProfileSelect_vtbl (ctors), cFEState_vtbl (dtor)
    // cFEStateProfileSelect's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "05sel_prof" through it; onWidgetEvent asks its custom interface (+0x0) for
    //       the next / previous state and pushes it on its stack (+0x18)
    //   mpScreen: [U] onCreateScreen stores the screen

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateProfileSelect1() __asm__("cFEStateProfileSelect_cFEStateProfileSelect1");
    // retail's destructor cFEStateProfileSelect__cFEStateProfileSelect stays a C function (g++ 2.95 takes no asm label
    // on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateProfileSelect();
    virtual void onCreateScreen() __asm__("cFEStateProfileSelect_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* menu, unsigned int msg) __asm__("cFEStateProfileSelect_onWidgetEvent");
};

CHECK_SIZE(cFEStateProfileSelect, 0x48);

#endif /* FE_FESTATEPROFILE_H */

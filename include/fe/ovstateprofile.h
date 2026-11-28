#ifndef FE_OVSTATEPROFILE_H
#define FE_OVSTATEPROFILE_H

// cOVState_PROFILE: the in-game profile save overlay (screen "121Profilesave_pda" on the PDA template; six "player%d"
// slots, the name keyboard, save / delete), a cFEMemCard with mType 0. Retail code: the ctor
// cOVState_PROFILE_cOVState_PROFILE in fe/ovstatetrophy (0x00211088), cOVState_PROFILE_* in fe/ovstateprofile
// (0x002111A0 ... 0x00212080), the destructor cOVState_PROFILE__cOVState_PROFILE and the engine-less ctor copy
// cOVState_PROFILE_cOVState_PROFILE1 in fe/all_in_one_fe. On Tour defines it in fe/all_in_one_fe.cpp.
//
// Size 0x27C:
//   [R] operator new(636, "cOVStateOverlay") for overlay 36 (cOVStateManager_triggerOverlay / triggerPopup) and
//       cMemMan_alloc(636, "Save Profile") in cOVState_PAUSE_OPTIONS_onWidgetEvent / cOVTemplate_Dialog_onWidgetEvent;
//       the ctor writes 0x22C, 0x230 and 0x278 (its last word).
//   [O] On Tour 0x1AC = cFEMemCard 0x164 + bool mIsOn, bool mIsHidden, unsigned short mTitle[32] and tOVStateEnum
//       mParentOV; retail: 0x22C + two words + 33 chars (0x42) + 2 bytes of padding + a word = 0x27C.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
// The ctor cOVState_PROFILE(cUIEngine*, unsigned char ucActiveControl, tOVStateEnum ov) runs
// cFEMemCard_cFEMemCard(this, pEngine, 0).
//
// Derives from cFEMemCard (fe/fememcard.h); its own members at their retail offsets. fe/fememcard.h has the inherited
// fields' evidence; only this class's own uses are noted here.
// Evidence tags:
//   [R] retail's constructors and allocation sizes;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cOVState_PROFILE): member names, types and order. On Tour's bools are 1 byte
//       (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).

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

struct cOVState_PROFILE : public cFEMemCard {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_PROFILE_vtbl (0x00472928, 47 slots), stored by cOVState_PROFILE_cOVState_PROFILE over
    //       cFEMemCard_vtbl
    // cOVState_PROFILE's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "121Profilesave_pda"; onWidgetEvent builds the cKeyboardPopup for it
    //   mCtrlBits: [U] onWidgetEvent passes it to the cKeyboardPopup ctor
    //   mAttrState: tAttrState (EA: 2 bytes; lw/sw the word); [U] cOVState_PAUSE_OPTIONS_onWidgetEvent sets mode 1
    //       (bits 8-13) after constructing it
    //   mpScreen: [U] onCreateScreen keeps the "121Profilesave_pda" screen here; every widget lookup
    // cOVState (fe/ovstate.h, 0x9C)
    // cOVState_PROFILE's notes on cOVState's fields:
    //   mOVID: [O] tOVStateEnum; [R] 0 (cFEMemCard_cFEMemCard)
    // cFEMemCard (fe/fememcard.h, 0x22C)
    // cOVState_PROFILE's notes on cFEMemCard's fields:
    //   mPopupType: [U] onWidgetEvent clears it when it opens the name keyboard
    //   mProcessingKeyboard: [U] onWidgetEvent sets it when it pushes the name keyboard
    //   mpKeyboard: [U] onWidgetEvent creates the "Keyboard" cKeyboardPopup (1092 bytes) once and reads its text
    //       (+0x74); onDestroyScreen clears it
    //   mIsProcessing: [U] onWidgetEvent tests / sets it
    //   mIsDone: [U] updateDone tests and sets it
    //   mState: [U] onWidgetEvent / displayStatus test it
    //   mSlot: [U] onWidgetEvent stores the picked item's misc data
    //   mPlayerIdx: [U] onWidgetEvent names that profile
    //   mIsSavePressed: [U] onWidgetEvent sets it with the save
    //   mIsCancelPressed: [U] onWidgetEvent sets it (back)
    //   mScreenStatus: [U] displayStatus / displayOn test 3; onWidgetEvent tests it
    //   mLastSelectedDevice: [U] displayOn stores the device it shows
    //   mDeleteButtonActive: [U] hideDeleteButton stores !hide
    //   mMainButtonActive: [U] hideMainButton stores !hide
    // --- cOVState_PROFILE
    int mIsOn;                      // 0x22C [O] (bool); [R] the ctor clears it; [U] displayStatus sets it once the
                                    //       card state is nonzero; hideActionButtons keeps the buttons hidden while it
                                    //       is clear
    int mIsHidden;                  // 0x230 [O] (bool); [R] the ctor clears it; [U] hideActionButtons compares and
                                    //       stores the new hidden state
    unsigned short mTitle[33];      // 0x234 [O] ([32] in On Tour; retail titles are kMCTitleStringSize = 33 chars);
                                    //       [U] displayOn has the card (mpMemCard slot 14) write each slot's title
                                    //       here and shows it
    char unk_276[0x2];              // 0x276 padding
    int mParentOV;                  // 0x278 [O] tOVStateEnum; [R] the ctor's last argument (42 from
                                    //       cOVStateManager_triggerOverlay and cOVState_PAUSE_OPTIONS_onWidgetEvent,
                                    //       0x18 from cOVTemplate_Dialog_onWidgetEvent); [U] updateDone tests 0x18

    // Methods bound to their retail symbols by asm labels.
    void displayOff() __asm__("cOVState_PROFILE_displayOff");
    void displayOn() __asm__("cOVState_PROFILE_displayOn");
    void initGeneral() __asm__("cOVState_PROFILE_initGeneral");
    // retail's destructor cOVState_PROFILE__cOVState_PROFILE stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_PROFILE();
    virtual void displayStatus() __asm__("cOVState_PROFILE_displayStatus");
    virtual void updateDone() __asm__("cOVState_PROFILE_updateDone");
    virtual void onWidgetEvent(cUIThing* item, unsigned int event) __asm__("cOVState_PROFILE_onWidgetEvent");
    virtual void hideActionButtons(bool show) __asm__("cOVState_PROFILE_hideActionButtons");
    virtual void hideDeleteButton(bool on) __asm__("cOVState_PROFILE_hideDeleteButton");
    virtual void hideMainButton(bool loadBlocked, bool on) __asm__("cOVState_PROFILE_hideMainButton");
    virtual void onCreateScreen() __asm__("cOVState_PROFILE_onCreateScreen");
    virtual void onDestroyScreen() __asm__("cOVState_PROFILE_onDestroyScreen__FPv");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int) __asm__("cOVState_PROFILE_onWidgetMenu");
};

CHECK_OFFSET(cOVState_PROFILE, mIsOn, 0x22C);
CHECK_OFFSET(cOVState_PROFILE, mIsHidden, 0x230);
CHECK_OFFSET(cOVState_PROFILE, mTitle, 0x234);
CHECK_OFFSET(cOVState_PROFILE, unk_276, 0x276);
CHECK_OFFSET(cOVState_PROFILE, mParentOV, 0x278);
CHECK_SIZE(cOVState_PROFILE, 0x27C);

#endif /* FE_OVSTATEPROFILE_H */

#ifndef FE_OVTEMPLATEDIALOG_H
#define FE_OVTEMPLATEDIALOG_H

// cOVTemplate_Dialog: the in-game yes/no dialog template (quit, restart, give up ...), and cOVState_QUIT, the quit
// dialog built on it. Retail code in the decomp's unit fe/ovtemplatedialog.cpp, constructors/destructors in
// fe/all_in_one_fe.cpp; EA's source file is not known (the preview's linkonce sections name _$_13cOVState_QUIT,
// _$_22cOVState_QUITCHALLENGE, _$_24cOVState_QUITCOMPETITION).
//
// cOVTemplate_Dialog: vtable cOVTemplate_Dialog_vtbl; constructor cOVTemplate_Dialog_cOVTemplate_Dialog (out of
// line, no callers), destructor cOVTemplate_Dialog__cOVTemplate_Dialog. On Tour size 0x94 = 0x90 + mDialogType. Its
// methods are cOVTemplate_Dialog_onCreateScreen / onWidgetCreate / onWidgetEvent / onWidgetMenu / onUpdate.
//
// The dialog overlays add no members: each is cMemMan_alloc(0xA0, "cOVStateOverlay") with its own vtable (all
// slots but the destructor are the template's, except cOVState_QUIT's onUpdate) and mDialogType set by the
// factory (cOVStateManager_triggerOverlay / triggerPopup):
//   id 23 kind 0 cOVState_RESTART_vtbl            "kT_OVRCMNRestartEvent"
//   id 24 kind 1 cOVState_QUIT_vtbl               "kT_OVRCMNQuitGame" (constructor cOVState_QUIT_cOVState_QUIT, own
//                                                 method cOVState_QUIT_onUpdate)
//   id 25 kind 2 cOVState_RESTARTCHALLENGE_vtbl   "kT_OVRCMNRestartChal"
//   id 26 kind 3 cOVState_QUITCHALLENGE_vtbl      "kT_OVRCMNQuitChal"
//   id 27 kind 4 cOVState_QUITCOMPETITION_vtbl    "kT_CMNQuitEventQues"
//   id 28 kind 5 cOVState_GIVEUP_vtbl             "kT_CMNGiveUp"
//   id 38 kind 6 cOVState_SAVEPROMPT_vtbl         "kT_CMNSaveBeforeQuit"
// The class names: each vtable's slot 1 is the destructor of that name (cOVState_RESTART__cOVState_RESTART ...; the
// preview's linkonce sections name _$_13cOVState_QUIT, _$_22cOVState_QUITCHALLENGE, _$_24cOVState_QUITCOMPETITION),
// in On Tour's tOVStateEnum order (RESTART, QUIT, RESTARTCHALLENGE, QUITCHALLENGE, QUITCOMPETITION, GIVEUP; SAVEPROMPT
// after AUTOSAVE, retail id 37), and each kind's title text above (onCreateScreen) fits its name. On Tour's
// QUITCHALLENGE / QUITCOMPETITION / RESTART / RESTARTCHALLENGE / GIVEUP / SAVEPROMPT are all 0x94 = the
// template's size. So cOVState_QUIT_onUpdate and the template's methods take a cOVTemplate_Dialog*.
//
// Derives from cOVState (fe/ovstate.h, 0x9C); its own members at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovtemplatedialog.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVTemplate_Dialog : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVTemplate_Dialog_vtbl or a dialog overlay's vtable, stored after cOVState_vtbl by the constructors
    // cOVTemplate_Dialog's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the dialog screen; widget lookups go through it
    // cOVState (fe/ovstate.h, 0x9C)
    // cOVTemplate_Dialog's notes on cOVState's fields:
    //   mRemovePDA: [U] onWidgetEvent sets it
    // --- cOVTemplate_Dialog (On Tour: size 0x94)
    int mDialogType;                // 0x09C [O]; [R] the factory and cOVState_QUIT_cOVState_QUIT store the dialog kind (0-6, see
                                    //       above); [U] onCreateScreen / onWidgetCreate / onWidgetEvent switch on it

    // The dialog kinds (mDialogType; names ours, after the overlay that stores each): [R]
    // cOVStateManager_triggerOverlay gives kOVSTATE_RESTART 0, kOVSTATE_QUIT 1, kOVSTATE_RESTARTCHALLENGE 2,
    // kOVSTATE_QUITCHALLENGE 3, kOVSTATE_QUITCOMPETITION 4, kOVSTATE_GIVEUP 5 and kOVSTATE_SAVEPROMPT 6; [U]
    // onCreateScreen titles them kT_OVRCMNRestartEvent, kT_OVRCMNQuitGame, kT_OVRCMNRestartChal, kT_OVRCMNQuitChal,
    // kT_CMNQuitEventQues, kT_CMNGiveUp and kT_CMNSaveBeforeQuit.
    enum {
        kDialogRestart = 0, kDialogQuit = 1, kDialogRestartChallenge = 2, kDialogQuitChallenge = 3,
        kDialogQuitCompetition = 4, kDialogGiveUp = 5, kDialogSavePrompt = 6
    };
};

CHECK_OFFSET(cOVTemplate_Dialog, mDialogType, 0x9C);
CHECK_SIZE(cOVTemplate_Dialog, 0xA0);


// ---------------------------------------------------------------------------------------------------------------------
// cPDATemplate: the in-game PDA frame (screens "PDATemplate" and "bganim1", with the "temperature" text), kept by the
// overlay manager in cOVStateManager_mpPDATemplate (cOVStateManager_addPDATemplate pushes it as an explicit state,
// cOVStateManager_removePDATemplate takes it down). A plain cUIState, not a cOVState.
// EA's source file is not known. Retail code: onCreateScreen in fe/ovtemplatedialog (0x0020A778); the dtor
// cPDATemplate__cPDATemplate and the ctor cPDATemplate_cPDATemplate in fe/all_in_one_fe (0x0021CA88, 0x0021CAA8).
//
// Size 0x48: [R] cMemMan_alloc(0x48, "PDATemplate") + cUIState_cUIState(mem, pEngine) + vtable in
// cOVStateManager_addPDATemplate; [O] On Tour's class is 0x48 (no members).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). Vtable cPDATemplate_vtbl (0x004743F0, 24 slots:
// 1 dtor, 4 onCreateScreen; the rest cUIState's). The dtor only runs cUIState__cUIState (its C definition declares no
// parameters: this stays in a0).
// Derives from cUIState (ui/uistate.h). Evidence tags: [R] retail's ctor / dtor / allocation; [U] how retail code uses
// the field; [O] SSX On Tour's DWARF (class cPDATemplate, cUIState): names, types and order.

struct cPDATemplate : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cPDATemplate_vtbl (ctor, cOVStateManager_addPDATemplate)
    // cPDATemplate's notes on cUIState's fields:
    //   mHashName: [U] onCreateScreen stores 41
    //   mpEngine: [U] onCreateScreen adds "PDATemplate" and "bganim1" through it
    //   mpScreen: [U] onCreateScreen stores the "PDATemplate" screen and looks "temperature" up on it
};

CHECK_SIZE(cPDATemplate, 0x48);


// ---------------------------------------------------------------------------------------------------------------------
// cOVTemplate_Simple: the simple one-option overlay template: cOVState's screen plus an "overlayMenu" with one visible
// option ("overlayOpt0", the localised name of D_00441D10[D_004A2860[0]]); picking it (event 5) sets
// cOVStateManager_mStatus to 1. Its onCreateScreen / onWidgetMenu / onWidgetEvent fill the slots of the overlay vtables
// built on it (D_004726A0, D_00472778, cOVState_HEAT_PREEVENT_vtbl cOVState_HEAT_PREEVENT, ...; those classes have their own headers).
// (There is no class "cOVTemplate": the cOVTemplate_* prefix covers cOVTemplate_Simple, cOVTemplate_Dialog (above) and
// cOVTemplate_PauseMenu (fe/ovstatepause.h).)
// EA's source file is not known. Retail code: onCreateScreen / onWidgetMenu / onWidgetEvent in fe/ovtemplatedialog
// (0x0020D1D8 ... 0x0020D318); the dtor and the (pEngine, id) ctor in fe/all_in_one_fe (0x0021C708, 0x0021C730).
//
// Size 0x9C: [O] On Tour's class is 0x90 = On Tour's cOVState (no members of its own); retail's cOVState is 0x9C
// (fe/ovstate.h) and the ctor writes nothing past mOVID (0x98). Retail never calls the out-of-line ctor.
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8). The ctor runs cUIState_cUIState, the
// inlined cOVState ctor (mOVID, the three bools, cOVState_vtbl, mszChatBuff = " "), then stores cOVTemplate_Simple_vtbl
// (cOVTemplate_Simple_vtbl, 25 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent, 20 onWidgetMenu, 25 cOVState_allowPauseMenu); the
// dtor stores cOVState_vtbl and runs cUIState__cUIState.
// Derives from cOVState (fe/ovstate.h); its own members at their retail offsets. fe/ovstate.h has the inherited fields'
// full evidence. Evidence tags: [R] retail's ctors / dtor; [P] the July 2003 preview (fe/ovstate.h); [U] how retail
// code uses the field; [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

struct cOVTemplate_Simple : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVTemplate_Simple_vtbl cOVTemplate_Simple_vtbl (ctor), cOVState_vtbl (dtor)
    // cOVTemplate_Simple's notes on cUIState's fields:
    //   mpScreen: [U] onCreateScreen looks "overlayMenu" and "overlayOpt0" ... "overlayOpt4" up on it
    // cOVState (fe/ovstate.h, 0x9C)
};

CHECK_SIZE(cOVTemplate_Simple, 0x9C);

#endif /* FE_OVTEMPLATEDIALOG_H */

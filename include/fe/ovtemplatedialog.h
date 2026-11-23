#ifndef FE_OVTEMPLATEDIALOG_H
#define FE_OVTEMPLATEDIALOG_H

// cOVTemplate_Dialog: the in-game yes/no dialog template (quit, restart, give up ...), and cOVState_QUIT, the quit
// dialog built on it. Retail code in the decomp's unit fe/ovtemplatedialog.cpp, constructors/destructors in
// fe/all_in_one_fe.cpp; EA's source file is not known (the preview's linkonce sections name _$_13cOVState_QUIT,
// _$_22cOVState_QUITCHALLENGE, _$_24cOVState_QUITCOMPETITION).
//
// cOVTemplate_Dialog: vtable D_00474C58; constructor cOVTemplate_Dialog_cOVTemplate_Dialog (out of line, no
// callers), destructor cOVTemplate_Dialog__cOVTemplate_Dialog. On Tour size 0x94 = 0x90 + mDialogType. Its methods are
// cOVTemplate_Dialog_onCreateScreen / onWidgetCreate / onWidgetEvent / onWidgetMenu / onUpdate.
//
// The dialog overlays add no members: each is cMemMan_alloc(0xA0, "cOVStateOverlay") with its own vtable (all
// slots but the destructor are the template's, except cOVState_QUIT's onUpdate) and mDialogType set by the
// factory: id 23 D_004749D0 kind 0, id 24 D_004748F8 kind 1 (cOVState_QUIT: constructor cOVState_QUIT_cOVState_QUIT,
// destructor cOVState_QUIT__cOVState_QUIT, own method cOVState_QUIT_onUpdate), id 25 D_00474820 kind 2, id 26
// D_00474748 kind 3, id 27 D_00474670 kind 4, id 28 D_00474598 kind 5, id 38 D_004744C0 kind 6 (On Tour's
// QUITCHALLENGE / QUITCOMPETITION / RESTART / RESTARTCHALLENGE / GIVEUP / SAVEPROMPT are all 0x94 = the
// template's size). So cOVState_QUIT_onUpdate and the template's methods take a cOVTemplate_Dialog*.
// Base cOVState (fe/ovstate.h, 0x9C), written out flat below with the same member names.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovtemplatedialog.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVTemplate_Dialog : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] D_00474C58 or a dialog overlay's vtable, stored after cOVState_vtbl by the constructors
    // cOVTemplate_Dialog's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the dialog screen; widget lookups go through it
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed; [U] onWidgetEvent sets it
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (str_sp_b, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (23-28, 38)
    // --- cOVTemplate_Dialog (On Tour: size 0x94)
    int mDialogType;                // 0x09C [O]; [R] the factory and cOVState_QUIT_cOVState_QUIT store the dialog kind (0-6, see
                                    //       above); [U] onCreateScreen / onWidgetCreate / onWidgetEvent switch on it
};

CHECK_OFFSET(cOVTemplate_Dialog, mDoneTransition, 0x48);
CHECK_OFFSET(cOVTemplate_Dialog, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVTemplate_Dialog, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVTemplate_Dialog, mRemovePDA, 0x54);
CHECK_OFFSET(cOVTemplate_Dialog, mszChatBuff, 0x58);
CHECK_OFFSET(cOVTemplate_Dialog, mOVID, 0x98);
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
// (D_00474D30, 25 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent, 20 onWidgetMenu, 25 cOVState_allowPauseMenu); the
// dtor stores cOVState_vtbl and runs cUIState__cUIState.
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. The cOVState part is written out with
// fe/ovstate.h's names (see there for the inherited fields' full evidence).
// Evidence tags: [R] retail's ctors / dtor; [P] the July 2003 preview (fe/ovstate.h); [U] how retail code uses the
// field; [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

struct cOVTemplate_Simple : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVTemplate_Simple_vtbl D_00474D30 (ctor), cOVState_vtbl (dtor)
    // cOVTemplate_Simple's notes on cUIState's fields:
    //   mpScreen: [U] onCreateScreen looks "overlayMenu" and "overlayOpt0" ... "overlayOpt4" up on it
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (On Tour: bool at 0x48); [R] the ctors zero it
    int mAllowPauseMenu;            // 0x04C [O][P] (fe/ovstate.h); [R] the ctors zero it
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] the ctors zero it
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " into it (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's id
};

CHECK_OFFSET(cOVTemplate_Simple, mDoneTransition, 0x048);
CHECK_OFFSET(cOVTemplate_Simple, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cOVTemplate_Simple, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cOVTemplate_Simple, mRemovePDA, 0x054);
CHECK_OFFSET(cOVTemplate_Simple, mszChatBuff, 0x058);
CHECK_OFFSET(cOVTemplate_Simple, mOVID, 0x098);
CHECK_SIZE(cOVTemplate_Simple, 0x9C);

#endif /* FE_OVTEMPLATEDIALOG_H */

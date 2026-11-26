#ifndef FE_OVSTATEHEAT_H
#define FE_OVSTATEHEAT_H

// cOVState_HEAT_PREEVENT: the pre-event overlay of a heat event (its strings: "title", "track_event",
// "rider%d", the Multi Play / Quick Play state text). Retail code in the decomp's unit fe/ovstatepause.cpp
// (onCreateScreen still asm), destructor and constructor in fe/all_in_one_fe.cpp. No members of its own:
// cMemMan_alloc(0x9C, "cOVStateOverlay") for overlay id 8; On Tour size 0x90. Its vtable cOVState_HEAT_PREEVENT_vtbl reuses
// cOVTemplate_Simple_onWidgetEvent / onWidgetMenu (slots 18/19), so it derives from cOVTemplate_Simple (vtable
// 0x474D30), which adds no members either (On Tour 0x90; its constructor func_0021C730 stores nothing past 0x98).
// Constructor cOVState_HEAT_PREEVENT_cOVState_HEAT_PREEVENT, destructor cOVState_HEAT_PREEVENT__cOVState_HEAT_PREEVENT. On Tour's other heat
// overlay, cOVState_HEAT_STANDINGS, has no retail counterpart identified (the overlays next to this one in the
// unit, ids 9 / 21 / 20: func_001FBD20 "41freestyle_pre", func_001FC878 "50multifs_pre", func_001FD268
// "68rival_pre", are other pre-event screens without names). Derives from cOVState (fe/ovstate.h, 0x9C).
//
// Its own members are at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstatepause.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_HEAT_PREEVENT : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_HEAT_PREEVENT_vtbl, stored after cOVState_vtbl by the constructor
    // cOVState_HEAT_PREEVENT's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the screen; widget lookups go through it
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVTemplate_Simple (vtable 0x474D30; On Tour 0x90): no members

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_HEAT_PREEVENT__cOVState_HEAT_PREEVENT stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_HEAT_PREEVENT();
    virtual void onUpdate() __asm__("cOVState_HEAT_PREEVENT_onUpdate");
};

CHECK_SIZE(cOVState_HEAT_PREEVENT, 0x9C);

#endif /* FE_OVSTATEHEAT_H */

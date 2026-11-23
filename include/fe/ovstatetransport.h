#ifndef FE_OVSTATETRANSPORT_H
#define FE_OVSTATETRANSPORT_H

// cOVState_TRANSPORT: the in-game transport (mountain map / travel) overlay, overlay id 33 (screen "Map"). It is a thin
// cOVState around an embedded cUITemplate_MAP (the same map template the FE's event select and credits use at +0x48):
// every method forwards to the template's same-named method with &mMap.
// Retail code: cOVState_TRANSPORT_* in fe/uitemplatemap (0x00208498 ... 0x00208678) and fe/all_in_one_fe (destructor
// cOVState_TRANSPORT__cOVState_TRANSPORT, the out-of-line ctor copies cOVState_TRANSPORT_cOVState_TRANSPORT_2 (engine, id) and cOVState_TRANSPORT_cOVState_TRANSPORT
// (engine-less)). On Tour defines it in fe/all_in_one_fe.cpp.
//
// Size 0x390:
//   [R] cOVStateManager_triggerOverlay builds overlay 33 inline in operator new(912, "cOVStateOverlay") and runs
//       cUITemplate_MAP_init(this + 0x9C);
//   [O] On Tour 0x1B4 = cOVState 0x90 + cUITemplate_MAP mMap (0x124), nothing after it; so retail's cUITemplate_MAP is
//       0x390 - 0x9C = 0x2F4, which matches its own methods' highest access (a word at 0x2F0, On Tour's last member
//       mScreenExiting).
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8), written out flat.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets.
// Evidence tags:
//   [R] retail's constructors and allocation size;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cOVState_TRANSPORT / cUITemplate_MAP).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/uitemplatemap.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_TRANSPORT : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_TRANSPORT_vtbl (no symbol yet; vtables.tsv: cOVState_TRANSPORT, slot 0
    //       cOVState_TRANSPORT__cOVState_TRANSPORT), stored after cOVState_vtbl by
    //       cOVState_TRANSPORT_cOVState_TRANSPORT_2 / cOVState_TRANSPORT_cOVState_TRANSPORT / the inlined ctor in
    //       cOVStateManager_triggerOverlay
    // cOVState_TRANSPORT's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "Map" to it; onWidgetEvent pops its state stack (+0x18)
    //   mAttrState: [U] onWidgetEvent sets mode 7 + the pending-delete bit (lw/sw the word)
    //   mpScreen: [U] onCreateScreen keeps the "Map" screen here and hands it to cUITemplate_MAP_onCreateScreen
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the (inlined) cOVState ctor zeroes it
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " (str_sp_b) into it
                                    //       (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's id (33 from
                                    //       cOVStateManager_triggerOverlay)
    // --- cOVState_TRANSPORT
    cUITemplate_MAP mMap;           // 0x09C [O] (On Tour 0x124; retail 0x2F4: its bools, including the two
                                    //       bool[3][3][8] tables, are words); [R] the ctors run cUITemplate_MAP_init
                                    //       on it; [U] every method passes &mMap to the same-named cUITemplate_MAP_*
                                    //       method
};

CHECK_OFFSET(cOVState_TRANSPORT, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_TRANSPORT, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_TRANSPORT, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_TRANSPORT, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_TRANSPORT, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_TRANSPORT, mOVID, 0x98);
CHECK_OFFSET(cOVState_TRANSPORT, mMap, 0x9C);
CHECK_SIZE(cOVState_TRANSPORT, 0x390);

#endif /* FE_OVSTATETRANSPORT_H */

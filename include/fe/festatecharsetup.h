#ifndef FE_FESTATECHARSETUP_H
#define FE_FESTATECHARSETUP_H

// cFEStateCharSetup: the front end's character setup screen ("09set_char"): continue / equip / rider / radio options
// around the user's 3D front-end rider (cFERiderManager at RealFE + 0xB0, fe/ferider.h). cFEStateRiderDetail (0x54,
// vtable 0x0046CD90) derives from it and shares its onDestroyScreen and onWidgetMenu.
// EA's source: d:/ssx/code/source/fe/festatecharsetup.cpp (preview strings); retail code: the ctor in
// fe/festatecharselect (0x001828C0), the methods in fe/festatecharsetup (0x00182928 ... 0x001831D0) and the
// destructor in main/all_in_one_main (0x001DD8A0).
//
// Size 0x50:
//   [R] cMemMan_alloc(0x50, "cFEStateCharSetup") + cFEStateCharSetup_cFEStateCharSetup(mem, engine, userNum) in
//       cFECustom_getNextState (twice) and cFECustom_getPrevState (three times); the methods reach the word at 0x4C.
//   [O] On Tour's class is 0x4C: cFEState 0x48 + two 1-byte bools; retail's bools are words (0x48, 0x4C). The derived
//       cFEStateRiderDetail agrees: On Tour puts its mbCheatCharEnable at 0x4C, retail allocates it 0x54.
// Base: cFEState (no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor
// runs cUIState_cUIState(this, pEngine), stores mHashName 0xC, cFEStateCharSetup_vtbl (0x0046CE60, 24 slots: 1 dtor, 4
// onCreateScreen, 5 onDestroyScreen, 12 onUpdate, 19 onWidgetEvent, 20 onWidgetMenu), mUserNum, mCtrlBits and
// mbDraw3D 0; the destructor stores cFEState_vtbl and runs cUIState__cUIState.
//
// Derives from cFEState (fe/festate.h); its own members at their retail offsets. bools are ints (ee-gcc 2.95's bool is
// 4 bytes).
// Evidence tags:
//   [R] retail's constructor / destructor and allocation size;
//   [U] how retail code uses the field (the C bodies in src/fe/festatecharsetup.cpp, fe/festatecharselect.cpp, asm);
//   [O] SSX On Tour's DWARF (class cFEStateCharSetup, cUIState, cListNode): names, types and order.
// (No preview evidence: layout_xlate finds no paired cFEStateCharSetup methods.)

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateCharSetup : public cFEState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateCharSetup_vtbl (ctor), cFEState_vtbl (dtor)
    // cFEStateCharSetup's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "09set_char" through it; the ctor reads the engine's cFECustom (*mpEngine)
    //   mCtrlBits: [R] cFECustom_getPlayerCtrlMask(.., userNum)
    //   mpScreen: [U] onCreateScreen stores the screen; the option widgets are found on it
    //   mUserNum: [U] picks the user's front-end rider and character
    // cFEState (fe/festate.h, 0x48)
    // --- cFEStateCharSetup
    int mbIsFirst;                  // 0x048 [O] (bool); [U] updateWidget / show3DCharacter set it,
                                    //       updateCharacter3DPos plays the intro animation once and clears it
    int mbDraw3D;                   // 0x04C [O] (bool); [R] ctor 0; [U] show3DCharacter stores its argument,
                                    //       updateCharacter3DPos poses the rider only while it is set
};

CHECK_OFFSET(cFEStateCharSetup, mbIsFirst, 0x048);
CHECK_OFFSET(cFEStateCharSetup, mbDraw3D, 0x04C);
CHECK_SIZE(cFEStateCharSetup, 0x50);

#endif /* FE_FESTATECHARSETUP_H */

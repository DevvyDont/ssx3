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
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet. bools are
// ints (ee-gcc 2.95's bool is 4 bytes).
// Evidence tags:
//   [R] retail's constructor / destructor and allocation size;
//   [U] how retail code uses the field (the C bodies in src/fe/festatecharsetup.cpp, fe/festatecharselect.cpp, asm);
//   [O] SSX On Tour's DWARF (class cFEStateCharSetup, cUIState, cListNode): names, types and order.
// (No preview evidence: layout_xlate finds no paired cFEStateCharSetup methods.)

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateCharSetup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState_cUIState links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateCharSetup_vtbl (ctor), cFEState_vtbl (dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 0xC
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the engine argument; [U] onCreateScreen adds "09set_char" through
                                    //       it; the ctor reads the engine's cFECustom (*mpEngine)
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cFECustom_getPlayerCtrlMask(.., userNum)
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (word, see ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; the option widgets are found on
                                    //       it
    signed char mUserNum;           // 0x044 [O]; [R] the ctor's userNum; [U] picks the user's front-end rider and
                                    //       character
    char unk_045[0x3];              // 0x045 padding
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateCharSetup
    int mbIsFirst;                  // 0x048 [O] (bool); [U] updateWidget / show3DCharacter set it,
                                    //       updateCharacter3DPos plays the intro animation once and clears it
    int mbDraw3D;                   // 0x04C [O] (bool); [R] ctor 0; [U] show3DCharacter stores its argument,
                                    //       updateCharacter3DPos poses the rider only while it is set
};

CHECK_OFFSET(cFEStateCharSetup, mpPrev, 0x000);
CHECK_OFFSET(cFEStateCharSetup, mpNext, 0x004);
CHECK_OFFSET(cFEStateCharSetup, mVtbl, 0x008);
CHECK_OFFSET(cFEStateCharSetup, mHashName, 0x00C);
CHECK_OFFSET(cFEStateCharSetup, mpEngine, 0x010);
CHECK_OFFSET(cFEStateCharSetup, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateCharSetup, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateCharSetup, unk_016, 0x016);
CHECK_OFFSET(cFEStateCharSetup, miMiscData, 0x018);
CHECK_OFFSET(cFEStateCharSetup, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateCharSetup, mpParentState, 0x020);
CHECK_OFFSET(cFEStateCharSetup, mScreens, 0x024);
CHECK_OFFSET(cFEStateCharSetup, mpScreen, 0x040);
CHECK_OFFSET(cFEStateCharSetup, mUserNum, 0x044);
CHECK_OFFSET(cFEStateCharSetup, unk_045, 0x045);
CHECK_OFFSET(cFEStateCharSetup, mbIsFirst, 0x048);
CHECK_OFFSET(cFEStateCharSetup, mbDraw3D, 0x04C);
CHECK_SIZE(cFEStateCharSetup, 0x50);

#endif /* FE_FESTATECHARSETUP_H */

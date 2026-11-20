#ifndef FE_FESTATERIDERDETAIL_H
#define FE_FESTATERIDERDETAIL_H

// cFEStateRiderDetail: the front end's rider details screen ("154rider_details"): the user's 3D front-end rider with
// five options (Option_1 .. Option_5: edit the name in an 8-character keyboard popup, pick a cheat character in
// "cFEStateCheatCharSelect", the other screens through cFECustom). It derives from cFEStateCharSetup
// (fe/festatecharsetup.h) and shares its onDestroyScreen, onWidgetMenu and its 3D-rider helpers (updateWidget,
// show3DCharacter, updateCharacter3DPos).
// EA's source file is not known (no assert of its own). Retail code: the ctor in fe/festatecharsetup (0x00183238,
// right after cFEStateCharSetup's methods), the methods in fe/festateriderbio (0x001832A8 ... 0x00183710; the code
// is contiguous with festatecharsetup.cpp's, so EA may declare the class in fe/festatecharsetup.h), the destructor in
// main/all_in_one_main (0x001DD8C8).
//
// Size 0x54:
//   [R] cMemMan_alloc(0x54, "cFEStateRiderDetail") + cFEStateRiderDetail_cFEStateRiderDetail(mem, engine, userNum) in
//       cFECustom_getNextState (once) and cFECustom_getPrevState (twice); the methods reach the word at 0x50.
//   [O] On Tour's class is 0x50: cFEStateCharSetup 0x4C + the 1-byte bool mbCheatCharEnable at 0x4C; retail's
//       cFEStateCharSetup is 0x50 (its two bools are words), so mbCheatCharEnable is the word at 0x50.
// Base: cFEStateCharSetup (0x50) : cFEState (no data) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8),
// written out inline below with the names of fe/festatecharsetup.h. The ctor runs
// cFEStateCharSetup_cFEStateCharSetup(this, pEngine, userNum), then stores mHashName 13, cFEStateRiderDetail_vtbl
// (0x0046CD90, 24 slots: 1 dtor, 4 onCreateScreen, 5 cFEStateCharSetup_onDestroyScreen, 6 onGainFocus, 12 onUpdate,
// 14 onPreRender2D, 15 onRender2D__FPv, 17 onPopupEvent, 19 onWidgetEvent, 20 cFEStateCharSetup_onWidgetMenu; the
// rest cUIState_* / cListNode_* / cFEState_onLoseFocus), mUserNum, mCtrlBits and mbDraw3D 0 again; the destructor
// stores cFEState_vtbl and runs cUIState__cUIState.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet. bools are
// ints (ee-gcc 2.95's bool is 4 bytes).
// Evidence tags:
//   [R] retail's constructor / destructor and allocation size;
//   [U] how retail code uses the field (the C bodies in src/fe/festateriderbio.cpp, fe/festatecharsetup.cpp, asm);
//   [O] SSX On Tour's DWARF (class cFEStateRiderDetail, cFEStateCharSetup, cUIState, cListNode): names, types and
//       order.
// (No preview evidence: layout_xlate finds no paired cFEStateRiderDetail methods.)

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateRiderDetail {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState_cUIState (through the base ctor)
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateRiderDetail_vtbl (ctor), cFEState_vtbl (dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 13 (over cFEStateCharSetup's 0xC)
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor reads its cFECustom (*mpEngine); [U] onCreateScreen adds
                                    //       "154rider_details" through it; onWidgetEvent pushes popups / states on its
                                    //       stack (+0x18) and asks its cFECustom for the next / previous state
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cFECustom_getPlayerCtrlMask(.., userNum)
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (word, see ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; onGainFocus / onPreRender2D find
                                    //       the Option_N objects on it
    signed char mUserNum;           // 0x044 [O]; [R] the ctor's userNum; [U] the player-interface calls (character,
                                    //       name, cheat character) and the user's front-end rider
    char unk_045[0x3];              // 0x045 padding
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateCharSetup (fe/festatecharsetup.h, 0x50)
    int mbIsFirst;                  // 0x048 [O] (bool); [U] cFEStateCharSetup_updateWidget / show3DCharacter set it,
                                    //       updateCharacter3DPos clears it (both called on this class)
    int mbDraw3D;                   // 0x04C [O] (bool); [R] ctor 0; [U] cFEStateCharSetup_show3DCharacter (from
                                    //       onPopupEvent / onWidgetEvent) stores its argument
    // --- cFEStateRiderDetail
    int mbCheatCharEnable;          // 0x050 [O] (bool; On Tour 0x4C); [U] onCreateScreen sets it when the user's
                                    //       character owns cheat characters (cBERewardInterface_getNumOwnedCheatChars),
                                    //       onPreRender2D greys out "Option_2" (the cheat character option) while 0;
                                    //       not set by the ctor
};

CHECK_OFFSET(cFEStateRiderDetail, mpPrev, 0x000);
CHECK_OFFSET(cFEStateRiderDetail, mpNext, 0x004);
CHECK_OFFSET(cFEStateRiderDetail, mVtbl, 0x008);
CHECK_OFFSET(cFEStateRiderDetail, mHashName, 0x00C);
CHECK_OFFSET(cFEStateRiderDetail, mpEngine, 0x010);
CHECK_OFFSET(cFEStateRiderDetail, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateRiderDetail, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateRiderDetail, unk_016, 0x016);
CHECK_OFFSET(cFEStateRiderDetail, miMiscData, 0x018);
CHECK_OFFSET(cFEStateRiderDetail, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateRiderDetail, mpParentState, 0x020);
CHECK_OFFSET(cFEStateRiderDetail, mScreens, 0x024);
CHECK_OFFSET(cFEStateRiderDetail, mpScreen, 0x040);
CHECK_OFFSET(cFEStateRiderDetail, mUserNum, 0x044);
CHECK_OFFSET(cFEStateRiderDetail, unk_045, 0x045);
CHECK_OFFSET(cFEStateRiderDetail, mbIsFirst, 0x048);
CHECK_OFFSET(cFEStateRiderDetail, mbDraw3D, 0x04C);
CHECK_OFFSET(cFEStateRiderDetail, mbCheatCharEnable, 0x050);
CHECK_SIZE(cFEStateRiderDetail, 0x54);

#endif /* FE_FESTATERIDERDETAIL_H */

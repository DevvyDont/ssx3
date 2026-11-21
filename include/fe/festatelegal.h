#ifndef FE_FESTATELEGAL_H
#define FE_FESTATELEGAL_H

// cFEStateLegal: the front end's legal screen: shows the screen hashed from "122bAutosave" for 240 frames, then plays
// its "TransitionOut" and pops itself. Retail never constructs it (no caller of either ctor).
// EA's source file is not known (On Tour: fe/all_in_one_fe). Retail code: the (pEngine, pParent) ctor func_00187C18
// (splat puts it at the end of fe/fememcard), onCreateScreen / onUpdate in fe/festatelegal (0x00187C50, 0x00187CB8),
// and the engine-less ctor, the dtor and onGainTransition in main/all_in_one_main (0x001DE990 ... 0x001DE9F0).
//
// Size 0x4C:
//   [O] On Tour's class is 0x4C: int mDisplayTicks at 0x48;
//   [R] func_00187C18 clears the word at 0x48 (the last one).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8); not a cFEState (vtable slot 6 is
// cUIState_onGainFocus, and the dtor runs cUIState__cUIState right after its own vtable store). func_00187C18 runs
// cUIState_cUIState1(this, pEngine, pParent) (the C definition declares only this) and stores cFEStateLegal_vtbl
// (0x0046B4A0, 24 slots: 1 dtor, 4 onCreateScreen, 7 onGainTransition, 12 onUpdate).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags: [R] retail's constructors / destructor / allocation; [U] how retail code uses the field (the C bodies
// named above and the asm); [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateLegal {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateLegal_vtbl (ctors, dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] onCreateScreen adds the screen
                                    //       through it; onUpdate pops its state stack (+0x18)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]; [R] func_00187C18's pParent (cUIState_cUIState1)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; onUpdate plays its
                                    //       "TransitionOut" frame
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateLegal
    int mDisplayTicks;              // 0x048 [O]; [R] func_00187C18 stores 0; [U] onCreateScreen 0; onUpdate counts it
                                    //       up and leaves at 240 (0xF0)
};

CHECK_OFFSET(cFEStateLegal, mpPrev, 0x000);
CHECK_OFFSET(cFEStateLegal, mpNext, 0x004);
CHECK_OFFSET(cFEStateLegal, mVtbl, 0x008);
CHECK_OFFSET(cFEStateLegal, mHashName, 0x00C);
CHECK_OFFSET(cFEStateLegal, mpEngine, 0x010);
CHECK_OFFSET(cFEStateLegal, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateLegal, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateLegal, unk_016, 0x016);
CHECK_OFFSET(cFEStateLegal, miMiscData, 0x018);
CHECK_OFFSET(cFEStateLegal, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateLegal, mpParentState, 0x020);
CHECK_OFFSET(cFEStateLegal, mScreens, 0x024);
CHECK_OFFSET(cFEStateLegal, mpScreen, 0x040);
CHECK_OFFSET(cFEStateLegal, mUserNum, 0x044);
CHECK_OFFSET(cFEStateLegal, unk_045, 0x045);
CHECK_OFFSET(cFEStateLegal, mDisplayTicks, 0x048);
CHECK_SIZE(cFEStateLegal, 0x4C);

#endif /* FE_FESTATELEGAL_H */

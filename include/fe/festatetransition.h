#ifndef FE_FESTATETRANSITION_H
#define FE_FESTATETRANSITION_H

// cFEStateTransition: a front-end state (a cUIState), 3 methods; allocated with cMemMan_alloc(80, "cFEStateTransition").
// Size 0x50 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateTransition {
    // --- cListNode
    cListNode* mpPrev;              // 0x000 [O]
    cListNode* mpNext;              // 0x004 [O]
    void** mVtbl;                   // 0x008 the vtable
    // --- cUIState
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O]
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateTransition
    cUIScreen* mpSnow;              // 0x048 [O]
    cUIScreen* mpWhite;             // 0x04C [O]
};

CHECK_OFFSET(cFEStateTransition, mpSnow, 0x048);
CHECK_OFFSET(cFEStateTransition, mpWhite, 0x04C);
CHECK_SIZE(cFEStateTransition, 0x50);


// ---------------------------------------------------------------------------------------------------------------------
// cFEStateBackground: the front end's animated background state ("snowanim"), the first state cRealFE_loadStartState
// pushes. It plays the screen and keeps nothing.
// EA's source file is not known (On Tour: fe/all_in_one_fe). Retail code: onCreateScreen in fe/festatetransition
// (0x00194528); the ctors func_001DE558 (engine 0) and cFEStateBackground_cFEStateBackground (pEngine; the C definition
// declares only this) and the dtor in main/all_in_one_main (0x001DE558 ... 0x001DE5C8).
//
// Size 0x48: [R] cMemMan_alloc(0x48, "cFEStateBackground") in cRealFE_loadStartState (the ctor inlined:
// cUIState_cUIState + vtable); [O] On Tour's class is 0x48 (no members).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8); not a cFEState (the dtor stores
// cFEStateBackground_vtbl and runs cUIState__cUIState). Vtable cFEStateBackground_vtbl (0x0046BC58, 24 slots: 1 dtor, 4
// onCreateScreen; the rest cUIState's).
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's ctors / dtor / allocation; [U] how retail code uses the
// field; [O] SSX On Tour's DWARF (class cFEStateBackground, cUIState): names, types and order.

struct cFEStateBackground {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateBackground_vtbl (ctors, cRealFE_loadStartState, dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] onCreateScreen adds "snowanim"
                                    //       through it and plays it (mpScreen is not set)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
};

CHECK_OFFSET(cFEStateBackground, mpPrev, 0x000);
CHECK_OFFSET(cFEStateBackground, mpNext, 0x004);
CHECK_OFFSET(cFEStateBackground, mVtbl, 0x008);
CHECK_OFFSET(cFEStateBackground, mHashName, 0x00C);
CHECK_OFFSET(cFEStateBackground, mpEngine, 0x010);
CHECK_OFFSET(cFEStateBackground, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateBackground, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateBackground, unk_016, 0x016);
CHECK_OFFSET(cFEStateBackground, miMiscData, 0x018);
CHECK_OFFSET(cFEStateBackground, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateBackground, mpParentState, 0x020);
CHECK_OFFSET(cFEStateBackground, mScreens, 0x024);
CHECK_OFFSET(cFEStateBackground, mpScreen, 0x040);
CHECK_OFFSET(cFEStateBackground, mUserNum, 0x044);
CHECK_OFFSET(cFEStateBackground, unk_045, 0x045);
CHECK_SIZE(cFEStateBackground, 0x48);

#endif

#ifndef FE_FESTATESELECTMP_H
#define FE_FESTATESELECTMP_H

// cFEStateSelectMultiplayerMode: the front end's "select multiplayer mode" menu ("86sel_multi", state id 22): two
// entries (misc data 0 / 1) whose pick asks the custom interface (cFECustom, the engine's first word) for the next /
// previous state (slots 4 / 5). Retail never constructs it (no caller of either ctor; the online FE uses
// cFEPopupSelectMultiplayerMode, fe/fepopupselectmultiplayermode.h).
// EA's source file is not known. Retail code: the engine ctor
// cFEStateSelectMultiplayerMode_cFEStateSelectMultiplayerMode in fe/fereal (0x001A3050); onCreateScreen /
// onWidgetCreate / onWidgetEvent in fe/festateselectmp (0x001A3090 ... 0x001A3160); the engine-less ctor
// ..._cFEStateSelectMultiplayerMode1 and the dtor in fe/fepopupmisc (0x001E07C0, 0x001E07F8).
//
// Size 0x48: [O] On Tour's class is 0x48 (no members); [R] the ctors write nothing past the cUIState part.
// Base: cFEState (fe/festate.h, no data) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor runs
// cUIState_cUIState(this, pEngine), stores mHashName 22 and cFEStateSelectMultiplayerMode_vtbl (0x00469248, 24 slots:
// 1 dtor, 4 onCreateScreen, 6 cFEState_onGainFocus, 11 cFEState_onLoseFocus, 18 onWidgetCreate, 19 onWidgetEvent); the
// dtor stores cFEState_vtbl and runs cUIState__cUIState.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags: [R] retail's constructors / destructor / allocation; [U] how retail code uses the field (the C bodies
// named above and the asm); [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateSelectMultiplayerMode {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateSelectMultiplayerMode_vtbl (ctors), cFEState_vtbl (dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] the engine ctor stores 22
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] onCreateScreen adds "86sel_multi"
                                    //       through it; onWidgetEvent asks its custom interface (+0x0) for the next
                                    //       state and pushes it on its stack (+0x18)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
};

CHECK_OFFSET(cFEStateSelectMultiplayerMode, mpPrev, 0x000);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mpNext, 0x004);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mVtbl, 0x008);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mHashName, 0x00C);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mpEngine, 0x010);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, unk_016, 0x016);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, miMiscData, 0x018);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mpParentState, 0x020);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mScreens, 0x024);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mpScreen, 0x040);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, mUserNum, 0x044);
CHECK_OFFSET(cFEStateSelectMultiplayerMode, unk_045, 0x045);
CHECK_SIZE(cFEStateSelectMultiplayerMode, 0x48);

#endif /* FE_FESTATESELECTMP_H */

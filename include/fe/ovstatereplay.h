#ifndef FE_OVSTATEREPLAY_H
#define FE_OVSTATEREPLAY_H
#pragma interface

// cOVState_REPLAY: the replay viewer overlay (screen "64replay": the timeline panel sliding at the bottom, the
// camera name, the progress ticker and the Start menu Save / Exit / Continue). EA's source
// d:/ssx/code/source/fe/ovstatereplay.cpp (preview assert strings); retail code in the decomp's units
// fe/ovstatereplay.cpp and fe/ovtemplatedialog.cpp (onCreateScreen), constructors in fe/all_in_one_fe.cpp.
// Size 0xB8: cMemMan_alloc(0xB8, "cOVStateOverlay") for overlay id 17; On Tour 0xA8 = 0x90 + its 7 members with
// 1-byte bools, i.e. 7 words here. Vtable cOVState_REPLAY_vtbl (0x474AA8); constructor
// cOVState_REPLAY_cOVState_REPLAY (out of line, no callers; triggerOverlay inlines it), destructor
// cOVState_REPLAY__cOVState_REPLAY. Derives from cOVState (fe/ovstate.h, 0x9C). On Tour's onCreateScreen stores the
// same constants (mBottom_y = mMinBottom_y = 245, mMaxBottom_y = 480, mBottom_ydiff = 0) in the same member order.
//
// Its own members are at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstatereplay.cpp / ovtemplatedialog.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_REPLAY : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_REPLAY_vtbl, stored after cOVState_vtbl by the constructor
    // cOVState_REPLAY's notes on cUIState's fields:
    //   mHashName: [U] onCreateScreen stores GetHashValue32("64replay")
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the "64replay" screen; widget lookups go through it
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_REPLAY (On Tour: size 0xA8)
    int mShowPopupMenu;             // 0x09C [O] bool; [U] onCreateScreen clears it; onInputBegin sets it when Start
                                    //       opens the replay menu and clears it on close; onUpdate / onWidgetMenu
                                    //       test it
    int mTimeLineHidden;            // 0x0A0 [O] bool; [U] onCreateScreen clears it and shows the matching hints;
                                    //       onInputBegin toggles it (ReplayTimeLine)
    int mBottom_y;                  // 0x0A4 [O]; [U] the panel position: onCreateScreen 245; onUpdate adds
                                    //       mBottom_ydiff and clamps it (read once as lwc1 + cvt.s.w for the
                                    //       object's y)
    int mBottom_ydiff;              // 0x0A8 [O]; [U] the panel speed: onCreateScreen 0; onInputBegin -4 / +4;
                                    //       onUpdate zeroes it at the ends
    int mMinBottom_y;               // 0x0AC [O]; [U] onCreateScreen 245; onUpdate / onInputBegin compare with it
    int mMaxBottom_y;               // 0x0B0 [O]; [U] onCreateScreen 480; onUpdate / onInputBegin compare with it
    int mLoaded;                    // 0x0B4 [O] bool; [U] onCreateScreen copies the replay's loaded flag
                                    //       (Game->Replay + 0x608) and, when set, hides "option_savereplay";
                                    //       onInputBegin's Save / Exit test it

    // Methods bound to their retail symbols by asm labels.
    void setupCameraName() __asm__("cOVState_REPLAY_setupCameraName");
    void setupTicker() __asm__("cOVState_REPLAY_setupTicker");
    virtual ~cOVState_REPLAY();   // g++'s own symbol _$_15cOVState_REPLAY (the retail label renamed)
    virtual void onUpdate() __asm__("cOVState_REPLAY_onUpdate");
    virtual void onWidgetCreate(cUIThing* w) __asm__("cOVState_REPLAY_onWidgetCreate");
    virtual void onCreateScreen() __asm__("cOVState_REPLAY_onCreateScreen");
    virtual bool onInputBegin(cUIInput* basePInput) __asm__("cOVState_REPLAY_onInputBegin");
};

CHECK_OFFSET(cOVState_REPLAY, mShowPopupMenu, 0x9C);
CHECK_OFFSET(cOVState_REPLAY, mTimeLineHidden, 0xA0);
CHECK_OFFSET(cOVState_REPLAY, mBottom_y, 0xA4);
CHECK_OFFSET(cOVState_REPLAY, mBottom_ydiff, 0xA8);
CHECK_OFFSET(cOVState_REPLAY, mMinBottom_y, 0xAC);
CHECK_OFFSET(cOVState_REPLAY, mMaxBottom_y, 0xB0);
CHECK_OFFSET(cOVState_REPLAY, mLoaded, 0xB4);
CHECK_SIZE(cOVState_REPLAY, 0xB8);
__asm__("_vt$15cOVState_REPLAY = cOVState_REPLAY_vtbl");   // g++'s name for the retail vtable

#endif /* FE_OVSTATEREPLAY_H */

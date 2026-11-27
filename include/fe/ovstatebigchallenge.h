#ifndef FE_OVSTATEBIGCHALLENGE_H
#define FE_OVSTATEBIGCHALLENGE_H

// The Big Challenge overlays (retail code in the decomp's unit fe/ovstateenterlodge.cpp; constructors/destructors in
// fe/all_in_one_fe.cpp; EA's source file is not known: the preview's linkonce sections name the classes
// (__27cOVState_BIGCHALLENGE_STARTP9cUIEngine12tOVStateEnum, _$_26cOVState_BIGCHALLENGE_FAIL) but no path).
// Both derive from cOVState (fe/ovstate.h, 0x9C).
//
// cOVState_BIGCHALLENGE_START: the challenge intro (overlay id 29). Allocated 0xA0 by cOVStateManager_triggerOverlay /
// triggerPopup, which then call cOVState_BIGCHALLENGE_START_getChallengeData. Vtable
// cOVState_BIGCHALLENGE_START_vtbl (0x473688); constructors cOVState_BIGCHALLENGE_START_cOVState_BIGCHALLENGE_START
// (engine, id; calls getChallengeData) and func_00220F48 (no engine); destructor
// cOVState_BIGCHALLENGE_START__cOVState_BIGCHALLENGE_START. On Tour: size 0x94, one member.
//
// cOVState_BIGCHALLENGE_FAIL: the challenge-failed screen (overlay id 30). Allocated 0xA4 (+ getChallengeData).
// Vtable cOVState_BIGCHALLENGE_FAIL_vtbl; constructors cOVState_BIGCHALLENGE_FAIL_cOVState_BIGCHALLENGE_FAIL_2 (engine, id; calls getChallengeData) and cOVState_BIGCHALLENGE_FAIL_cOVState_BIGCHALLENGE_FAIL (no engine);
// destructor cOVState_BIGCHALLENGE_FAIL__cOVState_BIGCHALLENGE_FAIL. On Tour: size 0x98, mMissionId + mRider.
//
// cOVState_BIGCHALLENGE_PASS is On Tour only: retail has no vtable, constructor or overlay id for it (the preview
// has BIGCHALLENGE_PREVIEW / CONFIRM / LIST instead, which retail dropped too).
//
// Derives from cOVState (fe/ovstate.h); its own members at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstateenterlodge.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_BIGCHALLENGE_START : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_BIGCHALLENGE_START_vtbl, stored after cOVState_vtbl by the constructors
    // cOVState_BIGCHALLENGE_START's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the screen; widget lookups go through it
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_BIGCHALLENGE_START (On Tour: size 0x94)
    int mMissionId;                 // 0x09C [O] (On Tour unsigned int); [U] getChallengeData stores the next
                                    //       mission-queue entry's id (or cWScriptMan's current mission, +0x2A0);
                                    //       onGainTransition / onWidgetEvent read it

    // Methods bound to their retail symbols by asm labels.
    void getChallengeData() __asm__("cOVState_BIGCHALLENGE_START_getChallengeData");
    // retail's destructor cOVState_BIGCHALLENGE_START__cOVState_BIGCHALLENGE_START stays a C function (g++ 2.95 takes
    // no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_BIGCHALLENGE_START();
    virtual void onCreateScreen() __asm__("cOVState_BIGCHALLENGE_START_onCreateScreen");
    virtual bool onGainTransition(int gained) __asm__("cOVState_BIGCHALLENGE_START_onGainTransition");
    virtual void onWidgetCreate(cUIThing* item) __asm__("cOVState_BIGCHALLENGE_START_onWidgetCreate");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int)
        __asm__("cOVState_BIGCHALLENGE_START_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* item, unsigned int msg) __asm__("cOVState_BIGCHALLENGE_START_onWidgetEvent");
};

CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mMissionId, 0x9C);
CHECK_SIZE(cOVState_BIGCHALLENGE_START, 0xA0);

struct cOVState_BIGCHALLENGE_FAIL : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_BIGCHALLENGE_FAIL_vtbl, stored after cOVState_vtbl by the constructors
    // cOVState_BIGCHALLENGE_FAIL's notes on cUIState's fields:
    //   mHashName: [U] onCreateScreen stores the screen's hash
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the screen; widget lookups go through it
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_BIGCHALLENGE_FAIL (On Tour: size 0x98)
    int mMissionId;                 // 0x09C [O]; [U] getChallengeData stores the next mission-queue entry's id;
                                    //       onWidgetEvent reads it
    int mRider;                     // 0x0A0 [O]; [R] placed by the allocation size (0xA4) and On Tour's order only:
                                    //       no retail code reads or writes it

    // Methods bound to their retail symbols by asm labels.
    void * cOVState_BIGCHALLENGE_FAIL_2(int engine, int overlayId)
        __asm__("cOVState_BIGCHALLENGE_FAIL_cOVState_BIGCHALLENGE_FAIL_2");

    // Methods bound to their retail symbols by asm labels.
    void getChallengeData() __asm__("cOVState_BIGCHALLENGE_FAIL_getChallengeData");
    // retail's destructor cOVState_BIGCHALLENGE_FAIL__cOVState_BIGCHALLENGE_FAIL stays a C function (g++ 2.95 takes no
    // asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_BIGCHALLENGE_FAIL();
    virtual void onCreateScreen() __asm__("cOVState_BIGCHALLENGE_FAIL_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* item) __asm__("cOVState_BIGCHALLENGE_FAIL_onWidgetCreate");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
        __asm__("cOVState_BIGCHALLENGE_FAIL_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* item, unsigned int msg) __asm__("cOVState_BIGCHALLENGE_FAIL_onWidgetEvent");
};

CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mMissionId, 0x9C);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mRider, 0xA0);
CHECK_SIZE(cOVState_BIGCHALLENGE_FAIL, 0xA4);

#endif /* FE_OVSTATEBIGCHALLENGE_H */

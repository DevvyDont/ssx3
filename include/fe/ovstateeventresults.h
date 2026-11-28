#ifndef FE_OVSTATEEVENTRESULTS_H
#define FE_OVSTATEEVENTRESULTS_H

// The event-result overlays of EA's fe/ovstateeventresults.cpp (path from the July 2003 preview's assert strings in
// cOVState_RACE_FINISHOV / cOVState_TOURNAMENT_STANDINGS code): cOVState subclasses created by
// cOVStateManager_triggerOverlay(type) (fe/ovtemplatedialog.cpp) with operator new(size, "cOVStateOverlay") and an
// inlined constructor (cUIState_cUIState, the cOVState part, then the class's vtable at +0x8). Retail code: the
// slot functions in fe/fepopupmisc, the out-of-line constructors / destructors in fe/all_in_one_fe.
//
//   type  class                          size  vtable      screen                      ctor          dtor (slot 1)
//   10    cOVState_HEAT_STANDINGS        0x9C  cOVState_HEAT_STANDINGS_vtbl  "42freestyle_standings"     cOVState_HEAT_STANDINGS_cOVState_HEAT_STANDINGS cOVState_HEAT_STANDINGS__cOVState_HEAT_STANDINGS
//   11    cOVState_FINAL_STANDINGS       0xA0  cOVState_FINAL_STANDINGS_vtbl  "43final_standings"         cOVState_FINAL_STANDINGS_cOVState_FINAL_STANDINGS cOVState_FINAL_STANDINGS__cOVState_FINAL_STANDINGS
//   12    cOVState_RACE_FINISHOV         0xA4  cOVState_RACE_FINISHOV_vtbl  "finishov" (mHashName)      cOVState_RACE_FINISHOV_cOVState_RACE_FINISHOV cOVState_RACE_FINISHOV__cOVState_RACE_FINISHOV
//   13    cOVState_BASSSP_QUAL_FINISHOV  0xA4  cOVState_BASSSP_QUAL_FINISHOV_vtbl  "finishov" (mHashName)      cOVState_BASSSP_QUAL_FINISHOV_cOVState_BASSSP_QUAL_FINISHOV cOVState_BASSSP_QUAL_FINISHOV__cOVState_BASSSP_QUAL_FINISHOV
//   35    cOVState_TOURNAMENT_STANDINGS  0x9C  cOVState_TOURNAMENT_STANDINGS_vtbl  "109MultiplayerStandings"   cOVState_TOURNAMENT_STANDINGS_cOVState_TOURNAMENT_STANDINGS cOVState_TOURNAMENT_STANDINGS__cOVState_TOURNAMENT_STANDINGS
//
// Names (two signals each): HEAT_STANDINGS and FINAL_STANDINGS - On Tour's DWARF has both classes with exactly the
// retail override sets (6 and 9 slots) and sizes base+0 / base+4 (doInit), and the retail screens / texts say
// "standings" (kT_OVRCMNQFHeat1Stand / Heat2Stand; "43final_standings"). RACE_FINISHOV, BASSSP_QUAL_FINISHOV and
// TOURNAMENT_STANDINGS - the preview's _vt$ sections with matching override sets, and their onCreateScreen strings
// (finishov / firstrun / place%d; "Invalid heat for this screen"; "109MultiplayerStandings") and constructor stores
// match retail. (builds/x/v2/name_map_v2.tsv pairs 0x00473FB0 with TOURNAMENT_STANDINGS by position only; the screen
// and override set put TOURNAMENT_STANDINGS at 0x00473C50.)
//
// Derives from cOVState (fe/ovstate.h); its cOVState part (0x9C) is written out with fe/ovstate.h's member names.
// Evidence tags: [R] retail constructors / destructors and the "cOVStateOverlay" allocation sizes; [P] the preview
// build; [U] retail usage (C in src/fe/fepopupmisc.cpp and the asm); [O] SSX On Tour's DWARF (its cOVState is
// 0x90 and its bools are 1 byte, so only member names and order are used).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

// cOVState_HEAT_STANDINGS (0x9C, no own members; On Tour likewise has none: size 0x90 = its cOVState). Slots: 4
// onCreateScreen cOVState_HEAT_STANDINGS_onCreateScreen, 6 onGainFocus cOVState_HEAT_STANDINGS_onGainFocus, 8 onInputBegin cOVState_HEAT_STANDINGS_onInputBegin, 12 onUpdate cOVState_HEAT_STANDINGS_onUpdate,
// 19 onWidgetEvent cOVState_HEAT_STANDINGS_onWidgetEvent, 20 onWidgetMenu cOVState_HEAT_STANDINGS_onWidgetMenu.

struct cOVState_HEAT_STANDINGS : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_HEAT_STANDINGS_cOVState_HEAT_STANDINGS stores cOVState_HEAT_STANDINGS_vtbl after cOVState_vtbl
    // cOVState_HEAT_STANDINGS's notes on cUIState's fields:
    //   mpScreen: [U] onCreateScreen stores the new screen
    // cOVState (fe/ovstate.h, 0x9C)

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_HEAT_STANDINGS__cOVState_HEAT_STANDINGS stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_HEAT_STANDINGS();
    virtual void onCreateScreen() __asm__("cOVState_HEAT_STANDINGS_onCreateScreen");
    virtual bool onInputBegin(cUIInput* input) __asm__("cOVState_HEAT_STANDINGS_onInputBegin");
    virtual void onGainFocus(cUIState*) __asm__("cOVState_HEAT_STANDINGS_onGainFocus");
    virtual int onWidgetMenu(cUIMenu* widget, unsigned int event, unsigned int)
        __asm__("cOVState_HEAT_STANDINGS_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* popup, unsigned int event) __asm__("cOVState_HEAT_STANDINGS_onWidgetEvent");
    virtual void onUpdate() __asm__("cOVState_HEAT_STANDINGS_onUpdate__FPv");
};

CHECK_SIZE(cOVState_HEAT_STANDINGS, 0x9C);

// cOVState_FINAL_STANDINGS (0xA0: cMemMan size for type 11). Slots: 4 onCreateScreen cOVState_FINAL_STANDINGS_onCreateScreen, 6 onGainFocus
// cOVState_FINAL_STANDINGS_onGainFocus, 7 onGainTransition cOVState_FINAL_STANDINGS_onGainTransition, 8 onInputBegin cOVState_FINAL_STANDINGS_onInputBegin, 11 onLoseFocus cOVState_FINAL_STANDINGS_onLoseFocus, 12
// onUpdate
// cOVState_FINAL_STANDINGS_onUpdate, 18 onWidgetCreate cOVState_FINAL_STANDINGS_onWidgetCreate, 19 onWidgetEvent cOVState_FINAL_STANDINGS_onWidgetEvent, 20 onWidgetMenu cOVState_FINAL_STANDINGS_onWidgetMenu.

struct cOVState_FINAL_STANDINGS : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_FINAL_STANDINGS_cOVState_FINAL_STANDINGS stores cOVState_FINAL_STANDINGS_vtbl after cOVState_vtbl
    // cOVState_FINAL_STANDINGS's notes on cUIState's fields:
    //   mpScreen: [U] onCreateScreen stores the new screen
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_FINAL_STANDINGS
    int doInit;                      // 0x09C [O] bool doInit (On Tour 0x90, its only member); [U] onCreateScreen
                                     //       cOVState_FINAL_STANDINGS_onCreateScreen sets it, onGainTransition cOVState_FINAL_STANDINGS_onGainTransition tests and clears it
                                     //       (lw/sw: a word, as retail bools are)

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_FINAL_STANDINGS__cOVState_FINAL_STANDINGS stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_FINAL_STANDINGS();
    virtual void onCreateScreen() __asm__("cOVState_FINAL_STANDINGS_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* menu) __asm__("cOVState_FINAL_STANDINGS_onWidgetCreate");
    virtual void onUpdate() __asm__("cOVState_FINAL_STANDINGS_onUpdate");
    virtual bool onInputBegin(cUIInput* input) __asm__("cOVState_FINAL_STANDINGS_onInputBegin");
    virtual bool onGainTransition(int) __asm__("cOVState_FINAL_STANDINGS_onGainTransition");
    virtual void onGainFocus(cUIState*) __asm__("cOVState_FINAL_STANDINGS_onGainFocus");
    virtual void onLoseFocus(cUIState*) __asm__("cOVState_FINAL_STANDINGS_onLoseFocus__FPv");
    virtual int onWidgetMenu(cUIMenu* widget, unsigned int event, unsigned int)
        __asm__("cOVState_FINAL_STANDINGS_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* popup, unsigned int event) __asm__("cOVState_FINAL_STANDINGS_onWidgetEvent");
};

CHECK_OFFSET(cOVState_FINAL_STANDINGS, doInit, 0x09C);
CHECK_SIZE(cOVState_FINAL_STANDINGS, 0xA0);

// cOVState_RACE_FINISHOV (0xA4: cMemMan size for type 12). The ctor also sets mHashName = GetHashValue32("finishov").
// Slots: 4 onCreateScreen cOVState_RACE_FINISHOV_onCreateScreen, 12 onUpdate cOVState_RACE_FINISHOV_onUpdate, 19 onWidgetEvent cOVState_RACE_FINISHOV_onWidgetEvent, 20 onWidgetMenu
// cOVState_RACE_FINISHOV_onWidgetMenu, 24 handleEvent cOVState_RACE_FINISHOV_handleEvent. Preview: _vt$22cOVState_RACE_FINISHOV (overrides 4 / 13 / 19 / 22 in
// its 23-slot layout = retail 4 / 12 / 19 / 24).

struct cOVState_RACE_FINISHOV : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_RACE_FINISHOV_cOVState_RACE_FINISHOV stores cOVState_RACE_FINISHOV_vtbl after cOVState_vtbl
    // cOVState_RACE_FINISHOV's notes on cUIState's fields:
    //   mpScreen: [U] onCreateScreen stores the new screen
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_RACE_FINISHOV
    float mElapsedTime;              // 0x09C seconds shown (name ours): [U] onUpdate (slot 12) advances it by 1/60 per
                                     //       frame (lwc1/swc1) and moves on at 3 s (1 s); onCreateScreen zeroes it
    int mCanContinue;                // 0x0A0 may the overlay move on (name ours): [R] the ctor stores 1; [U] handleEvent
                                     //       (slot 24) clears it on event 0 and sets it on 1; onUpdate, onWidgetEvent /
                                     //       onWidgetMenu (slots 19 / 20) move on only while it is set. Preview: the
                                     //       same store of 1 in its ctor and the same handleEvent

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_RACE_FINISHOV__cOVState_RACE_FINISHOV stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_RACE_FINISHOV();
    virtual void onCreateScreen() __asm__("cOVState_RACE_FINISHOV_onCreateScreen");
    virtual void onUpdate() __asm__("cOVState_RACE_FINISHOV_onUpdate");
    virtual void onWidgetEvent(cUIThing* popup, unsigned int event) __asm__("cOVState_RACE_FINISHOV_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* widget, unsigned int key, unsigned int)
        __asm__("cOVState_RACE_FINISHOV_onWidgetMenu");
    virtual int handleEvent(int message, void*) __asm__("cOVState_RACE_FINISHOV_handleEvent");
};

CHECK_OFFSET(cOVState_RACE_FINISHOV, mElapsedTime, 0x09C);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mCanContinue, 0x0A0);
CHECK_SIZE(cOVState_RACE_FINISHOV, 0xA4);

// cOVState_BASSSP_QUAL_FINISHOV (0xA4: cMemMan size for type 13; same members as RACE_FINISHOV, both written by the
// same ctor pattern). Slots: 4 onCreateScreen cOVState_BASSSP_QUAL_FINISHOV_onCreateScreen, 12 onUpdate cOVState_BASSSP_QUAL_FINISHOV_onUpdate, 19 onWidgetEvent cOVState_BASSSP_QUAL_FINISHOV_onWidgetEvent,
// 20 onWidgetMenu cOVState_BASSSP_QUAL_FINISHOV_onWidgetMenu, 24 handleEvent cOVState_BASSSP_QUAL_FINISHOV_handleEvent. Preview: _vt$29cOVState_BASSSP_QUAL_FINISHOV (same
// override set as RACE_FINISHOV; its onCreateScreen labels the runs by heat).

struct cOVState_BASSSP_QUAL_FINISHOV : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_BASSSP_QUAL_FINISHOV_cOVState_BASSSP_QUAL_FINISHOV stores cOVState_BASSSP_QUAL_FINISHOV_vtbl
    //       after cOVState_vtbl
    // cOVState_BASSSP_QUAL_FINISHOV's notes on cUIState's fields:
    //   mpScreen: [U] onCreateScreen stores the new screen
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_BASSSP_QUAL_FINISHOV
    float mElapsedTime;              // 0x09C seconds shown (name ours): [U] onUpdate (slot 12) advances it by 1/60 per
                                     //       frame (lwc1/swc1) and moves on at 3 s (1 s); onCreateScreen zeroes it
    int mCanContinue;                // 0x0A0 may the overlay move on (name ours): [R] the ctor stores 1; [U] handleEvent
                                     //       (slot 24) clears it on event 0 and sets it on 1; onUpdate, onWidgetEvent /
                                     //       onWidgetMenu (slots 19 / 20) move on only while it is set. Preview: the
                                     //       same store of 1 in its ctor and the same handleEvent

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_BASSSP_QUAL_FINISHOV__cOVState_BASSSP_QUAL_FINISHOV stays a C function (g++ 2.95
    // takes no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_BASSSP_QUAL_FINISHOV();
    virtual void onCreateScreen() __asm__("cOVState_BASSSP_QUAL_FINISHOV_onCreateScreen");
    virtual void onUpdate() __asm__("cOVState_BASSSP_QUAL_FINISHOV_onUpdate");
    virtual void onWidgetEvent(cUIThing* popup, unsigned int event)
        __asm__("cOVState_BASSSP_QUAL_FINISHOV_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* widget, unsigned int key, unsigned int)
        __asm__("cOVState_BASSSP_QUAL_FINISHOV_onWidgetMenu");
    virtual int handleEvent(int message, void*) __asm__("cOVState_BASSSP_QUAL_FINISHOV_handleEvent");
};

CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mElapsedTime, 0x09C);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mCanContinue, 0x0A0);
CHECK_SIZE(cOVState_BASSSP_QUAL_FINISHOV, 0xA4);

// cOVState_TOURNAMENT_STANDINGS (0x9C, no own members). Slots: 4 onCreateScreen cOVState_TOURNAMENT_STANDINGS_onCreateScreen, 19 onWidgetEvent
// cOVState_TOURNAMENT_STANDINGS_onWidgetEvent. Preview: _vt$29cOVState_TOURNAMENT_STANDINGS overrides the same two slots, screen
// "109MultiplayerStandings".

struct cOVState_TOURNAMENT_STANDINGS : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_TOURNAMENT_STANDINGS_cOVState_TOURNAMENT_STANDINGS stores cOVState_TOURNAMENT_STANDINGS_vtbl after cOVState_vtbl
    // cOVState_TOURNAMENT_STANDINGS's notes on cUIState's fields:
    //   mpScreen: [U] onCreateScreen stores the new screen
    // cOVState (fe/ovstate.h, 0x9C)

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_TOURNAMENT_STANDINGS__cOVState_TOURNAMENT_STANDINGS stays a C function (g++ 2.95
    // takes no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_TOURNAMENT_STANDINGS();
    virtual void onCreateScreen() __asm__("cOVState_TOURNAMENT_STANDINGS_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* popup, unsigned int event)
        __asm__("cOVState_TOURNAMENT_STANDINGS_onWidgetEvent");
};

CHECK_SIZE(cOVState_TOURNAMENT_STANDINGS, 0x9C);

#endif /* FE_OVSTATEEVENTRESULTS_H */

#ifndef FE_FESTATENETHELPPOPUP_H
#define FE_FESTATENETHELPPOPUP_H

// cFEStateNetHelpPopup: the help popup ("help_popup" screen) the online states and the on-screen keyboard open on the
// help button: a title, up to three description strings joined into one body text, a bottom note and up to seven
// option rows, each with one of nine button icons. It pops itself once the help button is released.
// EA's source file is not known (no assert of its own); retail code: the engine ctor in fe/fememcardcallbacks
// (0x001D9D48), the methods in fe/festatenethelppopup (0x001D9DB8 ... 0x001DA530) and the default-ctor copy, the
// destructor, onLoseTransition and four inline setters/getters in fe/fepopupmisc (0x001DF900 ... 0x001DF9A8).
//
// Size 0x140:
//   [R] cMemMan_alloc(0x140, "cFEStateNetHelpPopup") + cFEStateNetHelpPopup_cFEStateNetHelpPopup(mem, engine, parent)
//       in cKeyboardPopup_onInputBegin (fe/fepopup) and cFEStateNet_onWidgetEvent (fe/feasyncfile).
//   [O] On Tour's class is 0x148 with every member below at the same offset; its last member t3Vector
//       mtObjectInitialPos[7] (0x0F4, 0x54 bytes) does not fit in retail's 0x140, and nothing in retail reads or
//       writes 0x0F4..0x13F, so that tail is a raw region here (both ctors run an empty 7-element constructor loop,
//       which places nothing).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8), no cFEState: the ctor runs
// cUIState_cUIState1(this, pEngine, pParentState) (the default copy cUIState_cUIState(this, 0)) and stores
// cFEStateNetHelpPopup_vtbl (0x0046A618, 24 slots: 1 dtor, 4 onCreateScreen, 7 onGainTransition, 8 onInputBegin__FPv,
// 10 onLoseTransition__FPv, 12 onUpdate, 18 onWidgetCreate, 19 onWidgetEvent, 24 handleEvent); the destructor stores it
// again and runs cUIState__cUIState.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. bools and enums are ints (ee-gcc
// 2.95's bool is 4 bytes).
// Evidence tags:
//   [R] retail's constructors / destructor and allocation size;
//   [U] how retail code uses the field (the C bodies in src/fe/festatenethelppopup.cpp, fe/fepopupmisc.cpp and the
// asm);
//   [O] SSX On Tour's DWARF (class cFEStateNetHelpPopup, cUIState, cListNode): names, types and order.
// (No preview evidence: layout_xlate finds no paired cFEStateNetHelpPopup methods.)

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIText;
struct cUIIcon;

struct cFEStateNetHelpPopup : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateNetHelpPopup_vtbl (both ctors, the dtor)
    // cFEStateNetHelpPopup's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "help_popup" through it, onUpdate / onWidgetEvent pop the state stack at
    //       mpEngine + 0x18
    //   mpParentState: [U] handleEvent forwards to its slot 24
    //   mpScreen: [U] onCreateScreen stores the "help_popup" screen
    // --- cFEStateNetHelpPopup
    int miNumOptions;               // 0x048 [O]; [U] setNumHelpOptions stores it, clear zeroes it, onGainTransition
                                    //       shows rows i < miNumOptions
    int miReleasedCount;            // 0x04C [O]; [R] the ctor zeroes it; no other access
    int mbReferEventsToParent;      // 0x050 [O] (bool); [R] ctor 0; [U] setReferEventsToParent (cFEStateNetHelpPopup_setReferEventsToParent) /
                                    //       getReferEventsToParent (cFEStateNetHelpPopup_getReferEventsToParent); handleEvent tests it
    unsigned int muiScreenName;     // 0x054 [O]; [U] setScreenName (cFEStateNetHelpPopup_setScreenName), clear 0; onGainTransition: the
                                    //       title's text id
    unsigned int muiDescription[3]; // 0x058 [O]; [U] setDescription(i, id), clear zeroes 3;
                                    //       createAndSetDescriptionString joins the non-zero ids
    unsigned int muiBottomText;     // 0x064 [O]; [U] setBottomText (cFEStateNetHelpPopup_setBottomText), clear 0; onGainTransition shows it
                                    //       in
                                    //       mpBottom
    unsigned int muiOptions[7];     // 0x068 [O]; [U] setOption(i, text, ...) stores [i]; clear zeroes 7;
                                    //       onGainTransition sets row i's text from it
    int meObject[9];                // 0x084 [O] eHelpObjectType; [U] setOption stores the row's icon index; clear sets
                                    //       [0..6] to -1; onGainTransition shows mpObject[meObject[i]]
    cUIText* mpScreenName;          // 0x0A8 [O]; [U] onWidgetCreate binds "OnlineAccSetup"; onGainTransition
    cUIText* mpDescription;         // 0x0AC [O]; [U] widget "TopText"; createAndSetDescriptionString sets its text
    cUIText* mpBottom;              // 0x0B0 [O]; [U] widget "BottomNote"; onGainTransition
    cUIText* mpOptions[7];          // 0x0B4 [O]; [U] widgets "TextOption%d" (onWidgetCreate); onGainTransition shows
                                    //       or hides each row
    cUIIcon* mpObject[9];           // 0x0D0 [O]; [U] onWidgetCreate binds xselect, ps2triangle, ps2circle,
                                    //       updownarrows, leftrightarrows, ps2r1 (0xE4), ps2l1 (0xE8), ps2sel (0xEC),
                                    //       actrl (0xF0); onGainTransition hides all 9, shows and moves the used ones
    char unk_0F4[0x4C];             // 0x0F4 not accessed in retail (On Tour: t3Vector mtObjectInitialPos[7], 0x54,
                                    //       which does not fit; see the header comment)

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateNetHelpPopup1() __asm__("cFEStateNetHelpPopup_cFEStateNetHelpPopup1");
    void setScreenName(int value) __asm__("cFEStateNetHelpPopup_setScreenName");
    void setBottomText(int value) __asm__("cFEStateNetHelpPopup_setBottomText");
    void setReferEventsToParent(int value) __asm__("cFEStateNetHelpPopup_setReferEventsToParent");
    int getReferEventsToParent() __asm__("cFEStateNetHelpPopup_getReferEventsToParent");
    void createAndSetDescriptionString() __asm__("cFEStateNetHelpPopup_createAndSetDescriptionString");
    void clear() __asm__("cFEStateNetHelpPopup_clear");
    void setNumHelpOptions(int numOptions) __asm__("cFEStateNetHelpPopup_setNumHelpOptions__FPvi");
    void setDescription(int descriptionNum, int description) __asm__("cFEStateNetHelpPopup_setDescription__FPvii");
    // retail's destructor cFEStateNetHelpPopup__cFEStateNetHelpPopup stays a C function (g++ 2.95 takes no asm label on
    // a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateNetHelpPopup();
    virtual void onCreateScreen() __asm__("cFEStateNetHelpPopup_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStateNetHelpPopup_onUpdate");
    virtual void onWidgetCreate(cUIThing* w) __asm__("cFEStateNetHelpPopup_onWidgetCreate");
    void setOption(int optionNum, int textId, int iconIndex) __asm__("cFEStateNetHelpPopup_setOption__FPviii");
    virtual int handleEvent(int msg, void* msgParam) __asm__("cFEStateNetHelpPopup_handleEvent");
};

CHECK_OFFSET(cFEStateNetHelpPopup, miNumOptions, 0x048);
CHECK_OFFSET(cFEStateNetHelpPopup, miReleasedCount, 0x04C);
CHECK_OFFSET(cFEStateNetHelpPopup, mbReferEventsToParent, 0x050);
CHECK_OFFSET(cFEStateNetHelpPopup, muiScreenName, 0x054);
CHECK_OFFSET(cFEStateNetHelpPopup, muiDescription, 0x058);
CHECK_OFFSET(cFEStateNetHelpPopup, muiBottomText, 0x064);
CHECK_OFFSET(cFEStateNetHelpPopup, muiOptions, 0x068);
CHECK_OFFSET(cFEStateNetHelpPopup, meObject, 0x084);
CHECK_OFFSET(cFEStateNetHelpPopup, mpScreenName, 0x0A8);
CHECK_OFFSET(cFEStateNetHelpPopup, mpDescription, 0x0AC);
CHECK_OFFSET(cFEStateNetHelpPopup, mpBottom, 0x0B0);
CHECK_OFFSET(cFEStateNetHelpPopup, mpOptions, 0x0B4);
CHECK_OFFSET(cFEStateNetHelpPopup, mpObject, 0x0D0);
CHECK_OFFSET(cFEStateNetHelpPopup, unk_0F4, 0x0F4);
CHECK_SIZE(cFEStateNetHelpPopup, 0x140);

#endif /* FE_FESTATENETHELPPOPUP_H */

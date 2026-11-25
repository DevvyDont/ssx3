#ifndef FE_FESTATECHARSELECT_H
#define FE_FESTATECHARSELECT_H

// This header: cFEStateCharSelect and cFEStateCheatCharSelect (below).
// cFEStateCharSelect: the character select screen (a cUIState); cFEReal creates it with cMemMan_alloc(144, "cFEStateCharSelect").
// Size 0x90 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;
struct cUIThing;

struct cFEStateCharSelect : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateCharSelect
    cUIThing* mpSelectedCharWidget; // 0x048 [O]
    int mCheatCharID;               // 0x04C [O]
    int mSaveCurrentCtrl;           // 0x050 [O]
    int mbIsFirst;                  // 0x054 [O] bool
    float mZoom;                    // 0x058 [O]
    float mPan;                     // 0x05C [O]
    float mPlayerRotation;          // 0x060 [O]
    char unk_064[0xC];              // 0x064 padding
    char mCharPos[0x10];            // 0x070 [O] t4Vector
    int mbDraw3D;                   // 0x080 [O] bool
    char unk_084[0xC];              // 0x084 padding

    // Methods bound to their retail symbols by asm labels.
    void updateCharacter3DPos() __asm__("cFEStateCharSelect_updateCharacter3DPos");
};

CHECK_OFFSET(cFEStateCharSelect, mpSelectedCharWidget, 0x048);
CHECK_OFFSET(cFEStateCharSelect, mCheatCharID, 0x04C);
CHECK_OFFSET(cFEStateCharSelect, mSaveCurrentCtrl, 0x050);
CHECK_OFFSET(cFEStateCharSelect, mbIsFirst, 0x054);
CHECK_OFFSET(cFEStateCharSelect, mZoom, 0x058);
CHECK_OFFSET(cFEStateCharSelect, mPan, 0x05C);
CHECK_OFFSET(cFEStateCharSelect, mPlayerRotation, 0x060);
CHECK_OFFSET(cFEStateCharSelect, mCharPos, 0x070);
CHECK_OFFSET(cFEStateCharSelect, mbDraw3D, 0x080);
CHECK_SIZE(cFEStateCharSelect, 0x90);

// cFEStateCheatCharSelect: the cheat-character picker popup (a cFEState): six picture slots over a scrollable "6pics"
// menu of the base character and the cheat characters the user owns for it, with the highlighted entry's name.
// cFEStateRiderBio's onWidgetEvent opens it (option 6). EA's source: d:/ssx/code/source/fe/festatecharselect.cpp (the
// retail code sits in the fe/festatecharselect unit, 0x00182220 ... 0x00182870; the dtor and the isDone /
// getCheatCharID inlines in main/all_in_one_main, 0x001DD868 ... 0x001DD898).
//
// Size 0x1E4: [R] cMemMan_alloc(0x1E4, "cFEStateCheatCharSelect") + cFEStateCheatCharSelect_cFEStateCheatCharSelect
// (src/fe/festateriderbio.cpp); the ctor fills the three 30-entry tables up to mpCharNames[29] at 0x1E0.
// [O] On Tour's class is 0x454: the same members in the same order with 82-entry tables (retail 30).
// Base: cFEState (no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor
// runs cUIState_cUIState1(this, pEngine, pParentState), stores mHashName 10 and cFEStateCheatCharSelect_vtbl
// (0x0046CF30, 24 slots: 1 dtor, 4 onCreateScreen, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu, the rest
// cUIState's / cFEState's); the dtor stores cFEState_vtbl and runs cUIState__cUIState.
// Evidence tags: [R] retail ctor / dtor and allocation; [P] the preview's inline getCheatCharID / isDone (lw 0x6C /
// 0x70 there; layout_xlate: +0x4 from 0x44 on); [U] how retail code uses the field (src/fe/festatecharselect.cpp,
// src/main/all_in_one_main.cpp, the asm; widget fields name the UI object onWidgetCreate binds); [O] On Tour's DWARF
// (class cFEStateCheatCharSelect, cUIState, cListNode).

struct cUIText;
struct cUIIcon;
struct cUIMenu;
struct cTextureElement;

struct cFEStateCheatCharSelect : public cFEState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateCheatCharSelect_vtbl (ctor), cFEState_vtbl (dtor)
    // cFEStateCheatCharSelect's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "131cheat_char" through it; onWidgetEvent pops the engine's state stack
    //       (+0x18)
    //   mCtrlBits: [R] the ctor stores cFECustom_getPlayerCtrlMask(user)
    //   mpParentState: [U] the ctor looks the pictures up in its texture bank (+0x58)
    //   mpScreen: [U] onCreateScreen stores the screen
    //   mUserNum: [U] onWidgetEvent sets that player's cheat character
    // cFEState (fe/festate.h, 0x48)
    // --- cFEStateCheatCharSelect
    cUIThing* mpArrowPrev;          // 0x048 [O][P] (preview 0x44); [U] widget "arrowleft" (shown from 7 entries)
    cUIThing* mpArrowNext;          // 0x04C [O][P]; [U] widget "arrowright"
    cUIText* mpNameWidget;          // 0x050 [O][P]; [U] widget "nametext"; onWidgetEvent shows the entry's name
    cUIIcon* mpCharWidget[6];       // 0x054 [O][P]; [R] ctor zeroes; [U] widgets "pic1".. (D_00440FA0);
                                    //       updateCharWidgets gives each its entry's texture element
    cUIMenu* mpMenu;                // 0x06C [O][P]; [U] widget "6pics" (scrollable from 6 entries); its scroll offset
                                    //       (+0x98) is the first shown entry
    int mCheatCharID;               // 0x070 [O]; [P] getCheatCharID (lw); [R] ctor 0; [U] onWidgetEvent stores the
                                    //       picked entry's id (0 for the base character)
    int mDone;                      // 0x074 [O] (bool, a word); [P] isDone (lw); [R] ctor 0; [U] onWidgetEvent sets 1
    int mNumCharacters;             // 0x078 [O][P]; [R] ctor: 1 + the owned cheat characters; [U] onWidgetMenu,
                                    //       onWidgetCreate
    int mCharIDs[30];               // 0x07C [O] (On Tour [82]); [R] ctor: 0, then each owned character's id
    cTextureElement* mpCharElem[30]; // 0x0F4 [O] (On Tour [82]); [R] ctor: cUITextureBank_getElementByHashname of
                                    //       each picture; [U] onWidgetCreate / updateCharWidgets copy it to the slots
    const char* mpCharNames[30];    // 0x16C [O] (On Tour [82]); [R] ctor: the base character's short name, then each
                                    //       reward entry's name; [U] onWidgetEvent shows it

    // Methods bound to their retail symbols by asm labels.
    void updateCharWidgets() __asm__("cFEStateCheatCharSelect_updateCharWidgets");
    int isDone() __asm__("cFEStateCheatCharSelect_isDone");
    int getCheatCharID() __asm__("cFEStateCheatCharSelect_getCheatCharID");
};

CHECK_OFFSET(cFEStateCheatCharSelect, mpArrowPrev, 0x048);
CHECK_OFFSET(cFEStateCheatCharSelect, mpArrowNext, 0x04C);
CHECK_OFFSET(cFEStateCheatCharSelect, mpNameWidget, 0x050);
CHECK_OFFSET(cFEStateCheatCharSelect, mpCharWidget, 0x054);
CHECK_OFFSET(cFEStateCheatCharSelect, mpMenu, 0x06C);
CHECK_OFFSET(cFEStateCheatCharSelect, mCheatCharID, 0x070);
CHECK_OFFSET(cFEStateCheatCharSelect, mDone, 0x074);
CHECK_OFFSET(cFEStateCheatCharSelect, mNumCharacters, 0x078);
CHECK_OFFSET(cFEStateCheatCharSelect, mCharIDs, 0x07C);
CHECK_OFFSET(cFEStateCheatCharSelect, mpCharElem, 0x0F4);
CHECK_OFFSET(cFEStateCheatCharSelect, mpCharNames, 0x16C);
CHECK_SIZE(cFEStateCheatCharSelect, 0x1E4);

#endif

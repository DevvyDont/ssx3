#ifndef FE_FESTATEPEAKROOM_H
#define FE_FESTATEPEAKROOM_H
#pragma interface

// cFEStatePeakRoom: a front-end state (a cFEState), 8 methods; allocated with cMemMan_alloc(228, "cFEStatePeakRoom").
// Size 0xE4 [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cTextureElement;
struct cUIEngine;
struct cUIIcon;
struct cUIScreen;
struct cUIState;
struct cUIText;
struct cUIThing;

struct cFEStatePeakRoom : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStatePeakRoom
    cUIIcon* mpTrophyIcons[4];           // 0x048 [O]
    cUIIcon* mpMedals[5];                // 0x058 [O]
    cUIIcon* mpMedalsBackground[5];      // 0x06C [O]
    cUIText* mpMedalsText[5];            // 0x080 [O]
    cUIThing* mpMedalsTextBackground[5]; // 0x094 [O]
    cUIIcon* mpMedalsCheckMark[5];       // 0x0A8 [O]
    cUIIcon* mpMedalsCrossMark[5];       // 0x0BC [O]
    cUIText* mpHelpText;                 // 0x0D0 [O]
    cTextureElement* mpCheckElem;        // 0x0D4 [O]
    cTextureElement* mpNoCheckElem;      // 0x0D8 [O]
    int mCharID;                         // 0x0DC [O]
    int mPeakNum;                        // 0x0E0 [O]

    // Methods bound to their retail symbols by asm labels.
    void* cFEStatePeakRoom1() __asm__("cFEStatePeakRoom_cFEStatePeakRoom1");
    void updateHelpText(int index) __asm__("cFEStatePeakRoom_updateHelpText");
    void updateMedalsList(int peak) __asm__("cFEStatePeakRoom_updateMedalsList");
    virtual ~cFEStatePeakRoom();   // g++'s own symbol _$_16cFEStatePeakRoom (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStatePeakRoom_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* item, unsigned int key) __asm__("cFEStatePeakRoom_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* item, unsigned int key, unsigned int id) __asm__("cFEStatePeakRoom_onWidgetMenu");
    virtual void onGainFocus(cUIState*) __asm__("cFEStatePeakRoom_onGainFocus__FPv");
};

CHECK_OFFSET(cFEStatePeakRoom, mpTrophyIcons, 0x048);
CHECK_OFFSET(cFEStatePeakRoom, mpMedals, 0x058);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsBackground, 0x06C);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsText, 0x080);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsTextBackground, 0x094);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsCheckMark, 0x0A8);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsCrossMark, 0x0BC);
CHECK_OFFSET(cFEStatePeakRoom, mpHelpText, 0x0D0);
CHECK_OFFSET(cFEStatePeakRoom, mpCheckElem, 0x0D4);
CHECK_OFFSET(cFEStatePeakRoom, mpNoCheckElem, 0x0D8);
CHECK_OFFSET(cFEStatePeakRoom, mCharID, 0x0DC);
CHECK_OFFSET(cFEStatePeakRoom, mPeakNum, 0x0E0);
CHECK_SIZE(cFEStatePeakRoom, 0xE4);
__asm__("_vt$16cFEStatePeakRoom = cFEStatePeakRoom_vtbl");   // g++'s name for the retail vtable

#endif

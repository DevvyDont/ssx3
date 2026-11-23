#ifndef FE_FESTATEPEAKROOM_H
#define FE_FESTATEPEAKROOM_H

// cFEStatePeakRoom: a front-end state (a cUIState), 8 methods; allocated with cMemMan_alloc(228, "cFEStatePeakRoom").
// Size 0xE4 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cTextureElement;
struct cUIEngine;
struct cUIIcon;
struct cUIScreen;
struct cUIState;
struct cUIText;
struct cUIThing;

struct cFEStatePeakRoom : public cUIState {
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

#endif

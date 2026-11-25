#ifndef FE_FESTATECREDITS_H
#define FE_FESTATECREDITS_H

// cFEStateCredits: the credits screen (a cUIState): a scrolling text widget (mpTextScroll) with the Dolby icon, moved
// by mPixelDelta per frame. Methods in fe/festatecredits: constructors, onCreateScreen, onGainFocus, onUpdate,
// onRender2D, onInputBegin.
// Size 0x68 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIIcon;
struct cUIScreen;
struct cUIState;
struct cUITextScroll;
struct cUIThing;

struct cFEStateCredits : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateCredits
    cUIThing* mpWidget;             // 0x048 [O]
    float mYpos;                    // 0x04C [O]
    float mDeltaY;                  // 0x050 [O]
    int mMaxLines;                  // 0x054 [O]
    cUIIcon* mpDolbyIcon;           // 0x058 [O]
    cUITextScroll* mpTextScroll;    // 0x05C [O]
    int mPixelOffset;               // 0x060 [O]
    int mPixelDelta;                // 0x064 [O]

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateCredits1() __asm__("cFEStateCredits_cFEStateCredits1");
};

CHECK_OFFSET(cFEStateCredits, mpWidget, 0x048);
CHECK_OFFSET(cFEStateCredits, mYpos, 0x04C);
CHECK_OFFSET(cFEStateCredits, mDeltaY, 0x050);
CHECK_OFFSET(cFEStateCredits, mMaxLines, 0x054);
CHECK_OFFSET(cFEStateCredits, mpDolbyIcon, 0x058);
CHECK_OFFSET(cFEStateCredits, mpTextScroll, 0x05C);
CHECK_OFFSET(cFEStateCredits, mPixelOffset, 0x060);
CHECK_OFFSET(cFEStateCredits, mPixelDelta, 0x064);
CHECK_SIZE(cFEStateCredits, 0x68);

#endif

#ifndef UI_UIANIMATION_H
#define UI_UIANIMATION_H

// cUIAnimation: one UI animation playing on a screen (EA's UI library): the animation header, its channels, and the
// current / last evaluated frame, start frame and direction. Methods in ui/uianimation (constructors, start,
// setFrameNumber, updateFrame, updateThing, isLastFrame, getNumFrames, getHashName); the UI groups, texts and menus
// hold them (ui/uigroup.h, ui/uitext.h, ui/uimenu.h).
//
// Size 0x14 [R] (the retail allocation; On Tour's is the same; retail's vptr is at 0x10, after the members). A FLAT
// layout, like main/game.h: member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every
// retail access in the methods falls on a member start.

#include "layout.h"

struct cUIAnimationChannel;
struct tUIAnimation;

struct cUIAnimation {
    tUIAnimation* mpAnimHeader;          // 0x000 [O]
    cUIAnimationChannel* mpAnimChannels; // 0x004 [O]
    short mLastFrameEval;                // 0x008 [O]
    short mFrameNum;                     // 0x00A [O]
    unsigned short mScreenStartFrame;    // 0x00C [O]
    char mFrameDirection;                // 0x00E [O]
    char unk_00F[0x1];                   // 0x00F padding
    void** mVtbl;                        // 0x010 [R] ctor = cUIAnimation_vtbl (g++ 2.95 puts the vptr after the
                                         //       members)
};

CHECK_OFFSET(cUIAnimation, mpAnimHeader, 0x000);
CHECK_OFFSET(cUIAnimation, mpAnimChannels, 0x004);
CHECK_OFFSET(cUIAnimation, mLastFrameEval, 0x008);
CHECK_OFFSET(cUIAnimation, mFrameNum, 0x00A);
CHECK_OFFSET(cUIAnimation, mScreenStartFrame, 0x00C);
CHECK_OFFSET(cUIAnimation, mFrameDirection, 0x00E);
CHECK_OFFSET(cUIAnimation, mVtbl, 0x010);
CHECK_SIZE(cUIAnimation, 0x14);

#endif

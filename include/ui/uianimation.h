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

// cUIAnimationChannel: one animated channel of a cUIAnimation (cUIAnimation::mpAnimChannels points at an array of
// them, one per channel of the animation record): a wrapper of the channel's data record that evaluates it at a frame.
// Methods in ui/uianimation: cUIAnimationChannel_setAnimChanData, cUIAnimationChannel_evalAtFrameFloat,
// cUIAnimationChannel_getAnimChanType. No vtable.
// Size 0x4: [O] On Tour's class is 0x4 (mpAnimChan; methods evalAtFrameFloat, getAnimChanType, setAnimChanData);
// [R] cUIAnimation_cUIAnimation allocates the array with operator new(n * 4 + 0x10, "UIAnimChan") and calls
// cUIAnimationChannel_setAnimChanData(mpAnimChannels + (i << 2), record) for each channel.
struct tUIAnimChannel;

struct cUIAnimationChannel {
    tUIAnimChannel* mpAnimChan;      // 0x000 [O]; [U] setAnimChanData stores its argument (sw 0x0); evalAtFrameFloat
                                     //       and getAnimChanType read it (lw 0x0) and walk the record

    // Methods bound to their retail symbols by asm labels.
    unsigned short getAnimChanType() __asm__("cUIAnimationChannel_getAnimChanType");
    void setAnimChanData(int val) __asm__("cUIAnimationChannel_setAnimChanData__FPvi");
};

CHECK_OFFSET(cUIAnimationChannel, mpAnimChan, 0x000);
CHECK_SIZE(cUIAnimationChannel, 0x4);

struct cUIThing;

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

    // Methods bound to their retail symbols by asm labels.
    int isLastFrame() __asm__("cUIAnimation_isLastFrame");
    short getNumFrames() __asm__("cUIAnimation_getNumFrames");
    void start(int mode, unsigned short FrameNum) __asm__("cUIAnimation_start");
    void updateFrame(unsigned char flags) __asm__("cUIAnimation_updateFrame");
    void updateThing(cUIThing* obj, int mode, unsigned short FrameNum) __asm__("cUIAnimation_updateThing");
    void* cUIAnimation1() __asm__("cUIAnimation_cUIAnimation1");
    int getHashName() __asm__("cUIAnimation_getHashName");
    void setFrameNumber(short frame) __asm__("cUIAnimation_setFrameNumber");
};

CHECK_OFFSET(cUIAnimation, mpAnimHeader, 0x000);
CHECK_OFFSET(cUIAnimation, mpAnimChannels, 0x004);
CHECK_OFFSET(cUIAnimation, mLastFrameEval, 0x008);
CHECK_OFFSET(cUIAnimation, mFrameNum, 0x00A);
CHECK_OFFSET(cUIAnimation, mScreenStartFrame, 0x00C);
CHECK_OFFSET(cUIAnimation, mFrameDirection, 0x00E);
CHECK_OFFSET(cUIAnimation, mVtbl, 0x010);
CHECK_SIZE(cUIAnimation, 0x14);


// ---------------------------------------------------------------------------------------------------------------------
// cUIAnimationBank: the UI engine's animation bank (cUIEngine::mAnimBank, embedded at 0x50; ui/uiengine.h): the loaded
// UI file's animation block (a count, then variable-size entries with a hash) and the engine. getAnimationByHashName
// builds a cUIAnimation (above) for the entry with the given hash. EA's source: d:/ssx/code/source/ui/uianimation.cpp
// (preview assert strings). Retail code: setData (splat puts it at the end of render/font, 0x00397138) and
// getAnimationByHashName in ui/uianimation (0x00397140); the ctor and setEngine are inlined.
//
// Size 0x8: [O] On Tour's class is 0x8 (mpAnimationBlock, mpEngine); [R] cUIEngine_cUIEngine stores the engine at
// engine + 0x54 (= mAnimBank + 0x4), and the engine's next member (mTextureBank) is at 0x58. No base, no vtable.
// A FLAT layout. Evidence tags: [R] the engine's ctor; [U] how retail code uses the field; [O] SSX On Tour's DWARF
// (class cUIAnimationBank): names, types and order.

struct tUIAnimationBlock;
struct cUIEngine;

struct cUIAnimationBank {
    tUIAnimationBlock* mpAnimationBlock; // 0x000 [O]; [U] setData stores the block; getAnimationByHashName reads its
                                    //       count (first word) and walks the entries after it
    cUIEngine* mpEngine;            // 0x004 [O]; [R] cUIEngine_cUIEngine stores the engine (sw this, 0x54)

    // Methods bound to their retail symbols by asm labels.
    void setData(int val) __asm__("cUIAnimationBank_setData__FPvi");
};

CHECK_OFFSET(cUIAnimationBank, mpAnimationBlock, 0x000);
CHECK_OFFSET(cUIAnimationBank, mpEngine, 0x004);
CHECK_SIZE(cUIAnimationBank, 0x8);

#endif

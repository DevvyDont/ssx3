#ifndef FE_BXLOADINDICATOR_H
#define FE_BXLOADINDICATOR_H

// cBXLoadIndicator: the in-game loading indicator (cGame::LoadIndicator, main/game.h): its text, the active indicator
// bits, the animation frame and the font it draws with. Methods in fe/ovstatehud.
//
// Size 0x10 (On Tour's; retail's methods reach mpFont at size - 4). Member names, order and offsets are EA's, from SSX
// On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"

struct cFont;

struct cBXLoadIndicator {
    unsigned short* mpLoadText;     // 0x000 [O]
    unsigned int mLoadIndicators;   // 0x004 [O]
    float mAnimFrame;               // 0x008 [O]
    cFont* mpFont;                  // 0x00C [O]

    // Methods bound to their retail symbols by asm labels.
    void Init() __asm__("cBXLoadIndicator_Init");
    void Update() __asm__("cBXLoadIndicator_Update");
    void Render() __asm__("cBXLoadIndicator_Render");
    void Show(int bit) __asm__("cBXLoadIndicator_Show");
    void Hide(int bit) __asm__("cBXLoadIndicator_Hide");
    void DeInit() __asm__("cBXLoadIndicator_DeInit__FPv");
};

CHECK_OFFSET(cBXLoadIndicator, mpLoadText, 0x000);
CHECK_OFFSET(cBXLoadIndicator, mLoadIndicators, 0x004);
CHECK_OFFSET(cBXLoadIndicator, mAnimFrame, 0x008);
CHECK_OFFSET(cBXLoadIndicator, mpFont, 0x00C);
CHECK_SIZE(cBXLoadIndicator, 0x10);

#endif

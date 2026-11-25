#ifndef VISUALFX_CROWDANIM2D_H
#define VISUALFX_CROWDANIM2D_H

// cCrowdAnim2D: one animated 2D crowd (visualfx/crowdrender2d): target state, current animation and play type, the
// current frame and its texture, and its cCrowdRender2D. Methods: the constructor, update, advanceFrame.
//
// Size 0x20 ([R] the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names, order
// and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods falls
// on a member start (tools/gen/self_offsets.py). Enums and bools are words.

#include "layout.h"

struct cCrowdRender2D;

struct cCrowdAnim2D {
    int m_TargetState;              // 0x000 [O] eCrowdState
    int m_CurrentAnimation;         // 0x004 [O] eCrowdAnimation
    int m_CurrentPlayType;          // 0x008 [O] eAnimPlayType
    int m_iCurrentFrame;            // 0x00C [O]
    int m_iCurrentFrameTexID;       // 0x010 [O]
    int m_iUpdateTick;              // 0x014 [O]
    int m_iLastTick;                // 0x018 [O]
    cCrowdRender2D* m_pCR2D;        // 0x01C [O]

    // Methods bound to their retail symbols by asm labels.
    int advanceFrame(int frame, int n, int* mode) __asm__("cCrowdAnim2D_advanceFrame");
    void update() __asm__("cCrowdAnim2D_update");
};

CHECK_OFFSET(cCrowdAnim2D, m_TargetState, 0x000);
CHECK_OFFSET(cCrowdAnim2D, m_CurrentAnimation, 0x004);
CHECK_OFFSET(cCrowdAnim2D, m_CurrentPlayType, 0x008);
CHECK_OFFSET(cCrowdAnim2D, m_iCurrentFrame, 0x00C);
CHECK_OFFSET(cCrowdAnim2D, m_iCurrentFrameTexID, 0x010);
CHECK_OFFSET(cCrowdAnim2D, m_iUpdateTick, 0x014);
CHECK_OFFSET(cCrowdAnim2D, m_iLastTick, 0x018);
CHECK_OFFSET(cCrowdAnim2D, m_pCR2D, 0x01C);
CHECK_SIZE(cCrowdAnim2D, 0x20);

#endif

#ifndef RENDER_PLAYERSHADOW_H
#define RENDER_PLAYERSHADOW_H

// cPlayerShadow: a rider's projected shadow (render/particle): the projection plane and axes, the projection and
// texture-projection matrices, the shadow texture size and the six bones the shadow is fitted to. Methods:
// the constructor, the destructor and InitBones.
// Size 0x1A0 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "bx/math3d.h"


struct cPlayerShadow {
    float mObjUp[3];                // 0x000 [O] t3Vector
    float mProjPos[3];              // 0x00C [O] t3Vector
    float mProjNorm[3];             // 0x018 [O] t3Vector
    float mProjD;                   // 0x024 [O]
    float mProjUp[3];               // 0x028 [O] t3Vector
    float mProjRight[3];            // 0x034 [O] t3Vector
    float mDXL;                     // 0x040 [O]
    float mDXR;                     // 0x044 [O]
    float mDYU;                     // 0x048 [O]
    float mDYD;                     // 0x04C [O]
    t4Matrix mProjMat;              // 0x050 [O]
    t4Matrix mCProjMat;             // 0x090 [O]
    t4Matrix mUVProjMat;            // 0x0D0 [O]
    t4Matrix mTUVProjMat;           // 0x110 [O]
    t4Vector mTUVScale;             // 0x150 [O]
    t4Vector mTUVTrans;             // 0x160 [O]
    int mLockTexture;               // 0x170 [O]
    int mShadowTexWidth;            // 0x174 [O]
    int mShadowTexHeight;           // 0x178 [O]
    int ShadowBone[6];              // 0x17C [O]
    char unk_194[0xC];              // 0x194 padding

    // Methods bound to their retail symbols by asm labels.
    void InitBones(int head, int lHand, int rHand, int lFoot, int rFoot, int hips) __asm__("cPlayerShadow_InitBones");
    void AddShadow() __asm__("cPlayerShadow_AddShadow");
    void LockShadowTexture() __asm__("cPlayerShadow_LockShadowTexture");
};

CHECK_OFFSET(cPlayerShadow, mObjUp, 0x000);
CHECK_OFFSET(cPlayerShadow, mProjPos, 0x00C);
CHECK_OFFSET(cPlayerShadow, mProjNorm, 0x018);
CHECK_OFFSET(cPlayerShadow, mProjD, 0x024);
CHECK_OFFSET(cPlayerShadow, mProjUp, 0x028);
CHECK_OFFSET(cPlayerShadow, mProjRight, 0x034);
CHECK_OFFSET(cPlayerShadow, mDXL, 0x040);
CHECK_OFFSET(cPlayerShadow, mDXR, 0x044);
CHECK_OFFSET(cPlayerShadow, mDYU, 0x048);
CHECK_OFFSET(cPlayerShadow, mDYD, 0x04C);
CHECK_OFFSET(cPlayerShadow, mProjMat, 0x050);
CHECK_OFFSET(cPlayerShadow, mCProjMat, 0x090);
CHECK_OFFSET(cPlayerShadow, mUVProjMat, 0x0D0);
CHECK_OFFSET(cPlayerShadow, mTUVProjMat, 0x110);
CHECK_OFFSET(cPlayerShadow, mTUVScale, 0x150);
CHECK_OFFSET(cPlayerShadow, mTUVTrans, 0x160);
CHECK_OFFSET(cPlayerShadow, mLockTexture, 0x170);
CHECK_OFFSET(cPlayerShadow, mShadowTexWidth, 0x174);
CHECK_OFFSET(cPlayerShadow, mShadowTexHeight, 0x178);
CHECK_OFFSET(cPlayerShadow, ShadowBone, 0x17C);
CHECK_SIZE(cPlayerShadow, 0x1A0);

#endif

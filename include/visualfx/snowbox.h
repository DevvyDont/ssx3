#ifndef VISUALFX_SNOWBOX_H
#define VISUALFX_SNOWBOX_H

// cSnowBox: one box of falling snow flakes that follows the camera (allocated as "SnowBox", 0xC0 each, five per
// snowfall: visualfx/boardwakefx). Methods in visualfx/boardwakefx: the constructor (view, type, flake count, box
// scale, wind scale, flake size, gravity), update, updateForCameraPosition, render, resetWind.
// Size 0xC0 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "bx/math3d.h"


struct cSnowBox {
    int mViewNum;                   // 0x000 [O]
    int mSnowBoxType;               // 0x004 [O] eSnowBoxType
    int mNumFlakes;                 // 0x008 [O]
    float mBoxScale;                // 0x00C [O]
    float mWindScale;               // 0x010 [O]
    float mFlakeSize;               // 0x014 [O]
    float mOpacity;                 // 0x018 [O]
    float mGravity;                 // 0x01C [O]
    t4Vector mOffset;               // 0x020 [O]
    t4Vector mWindNext;             // 0x030 [O]
    t4Vector mWindLast;             // 0x040 [O]
    t4Matrix mBoxMatrix;            // 0x050 [O]
    float mCurrentTimeToWindChange; // 0x090 [O]
    float mTotalTimeToWindChange;   // 0x094 [O]
    int mRandSeed;                  // 0x098 [O]
    int mRandSeed2;                 // 0x09C [O]
    int mRandSeed3;                 // 0x0A0 [O]
    float mColourR;                 // 0x0A4 [O]
    float mColourG;                 // 0x0A8 [O]
    float mColourB;                 // 0x0AC [O]
    int mResetWind;                 // 0x0B0 [O] bool
    char unk_0B4[0xC];              // 0x0B4 padding
};

CHECK_OFFSET(cSnowBox, mViewNum, 0x000);
CHECK_OFFSET(cSnowBox, mSnowBoxType, 0x004);
CHECK_OFFSET(cSnowBox, mNumFlakes, 0x008);
CHECK_OFFSET(cSnowBox, mBoxScale, 0x00C);
CHECK_OFFSET(cSnowBox, mWindScale, 0x010);
CHECK_OFFSET(cSnowBox, mFlakeSize, 0x014);
CHECK_OFFSET(cSnowBox, mOpacity, 0x018);
CHECK_OFFSET(cSnowBox, mGravity, 0x01C);
CHECK_OFFSET(cSnowBox, mOffset, 0x020);
CHECK_OFFSET(cSnowBox, mWindNext, 0x030);
CHECK_OFFSET(cSnowBox, mWindLast, 0x040);
CHECK_OFFSET(cSnowBox, mBoxMatrix, 0x050);
CHECK_OFFSET(cSnowBox, mCurrentTimeToWindChange, 0x090);
CHECK_OFFSET(cSnowBox, mTotalTimeToWindChange, 0x094);
CHECK_OFFSET(cSnowBox, mRandSeed, 0x098);
CHECK_OFFSET(cSnowBox, mRandSeed2, 0x09C);
CHECK_OFFSET(cSnowBox, mRandSeed3, 0x0A0);
CHECK_OFFSET(cSnowBox, mColourR, 0x0A4);
CHECK_OFFSET(cSnowBox, mColourG, 0x0A8);
CHECK_OFFSET(cSnowBox, mColourB, 0x0AC);
CHECK_OFFSET(cSnowBox, mResetWind, 0x0B0);
CHECK_SIZE(cSnowBox, 0xC0);

#endif

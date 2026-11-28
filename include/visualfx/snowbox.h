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
#include "ui/uithing.h"           /* tARGB */

// cSnowBox::mSnowBoxType. [O] On Tour's enum eSnowBoxType, names and values. [U] cSnowBox_update takes the flake
// colours and sizes for 0 and the fluff ones for 1; cSnowBox_render draws RenderSnowBoxFlakes / RenderSnowBoxFluffs.
enum eSnowBoxType { eSnowBoxFlake = 0, eSnowBoxFluff = 1 };

struct t4Matrix;
struct t4Vector;
struct tWorldPainterSnowfallParameters;

void operator_delete(int* p);

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

    // Methods bound to their retail symbols by asm labels.
    void resetWind() __asm__("cSnowBox_resetWind__FPv");
    void update(const tWorldPainterSnowfallParameters* params) __asm__("cSnowBox_update");
    void updateForCameraPosition(const t4Vector* delta) __asm__("cSnowBox_updateForCameraPosition");
    void render(const t4Matrix* snowMatrix, float x, float y, float z) __asm__("cSnowBox_render");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cSnowBox(int viewNum, int snowBoxType, float boxScale, float flakeSize, float opacity, float gravity);
    ~cSnowBox();
    static void operator delete(void* p) { operator_delete((int*)p); }
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

// The per-box parameters the renderer draws a snow box's flakes and fluffs from (On Tour struct tSnowBoxData, 0x90;
// On Tour also has a ctor). [O] cGraphicsMan::RenderSnowBoxFlakes(const tSnowBoxData&) /
// RenderSnowBoxFluffs(const tSnowBoxData&); [R] cSnowBox_render fills one on the stack and
// cPS2GraphicsMan_RenderSnowBoxFlakes / RenderSnowBoxFluffs read it (views sPtObj_1310 / sPtObj_16F0: five quadwords
// and the matrix at 0x50) at the On Tour offsets.
struct tSnowBoxData {
    t4Vector mParticleOffset;       // 0x00 [O]; [R] cSnowBox_render: the box offset
    tARGB mColour;                  // 0x10 [O]; [R] cSnowBox_render: the colour (A, R, G, B)
    int mNumFlakes;                 // 0x20 [O]; [R] cSnowBox_render copies cSnowBox::mNumFlakes
    int mRandSeed;                  // 0x24 [O]; [R] cSnowBox::mRandSeed
    int mRandSeed2;                 // 0x28 [O]; [R] cSnowBox::mRandSeed2
    int mRandSeed3;                 // 0x2C [O]; [R] cSnowBox::mRandSeed3
    float mMinX;                    // 0x30 [O]; [R] the box minimum (written as one quadword)
    float mMinY;                    // 0x34 [O]
    float mMinZ;                    // 0x38 [O]
    float mFlakeSize;               // 0x3C [O]; [R] the quadword's w
    float mMaxX;                    // 0x40 [O]; [R] the box maximum (written as one quadword)
    float mMaxY;                    // 0x44 [O]
    float mMaxZ;                    // 0x48 [O]
    float mBoxScale;                // 0x4C [O]; [R] the quadword's w
    t4Matrix mSnowBoxToWorldMatrix; // 0x50 [O]; [R] cSnowBox_render: from cSnowBox::mBoxMatrix
};

CHECK_OFFSET(tSnowBoxData, mParticleOffset, 0x00);
CHECK_OFFSET(tSnowBoxData, mColour, 0x10);
CHECK_OFFSET(tSnowBoxData, mNumFlakes, 0x20);
CHECK_OFFSET(tSnowBoxData, mRandSeed, 0x24);
CHECK_OFFSET(tSnowBoxData, mRandSeed2, 0x28);
CHECK_OFFSET(tSnowBoxData, mRandSeed3, 0x2C);
CHECK_OFFSET(tSnowBoxData, mMinX, 0x30);
CHECK_OFFSET(tSnowBoxData, mFlakeSize, 0x3C);
CHECK_OFFSET(tSnowBoxData, mMaxX, 0x40);
CHECK_OFFSET(tSnowBoxData, mBoxScale, 0x4C);
CHECK_OFFSET(tSnowBoxData, mSnowBoxToWorldMatrix, 0x50);
CHECK_SIZE(tSnowBoxData, 0x90);

// One view's snowfall settings (On Tour struct tWorldPainterSnowfallParameters, 0x40; names and offsets [O]).
// [U] cSnowFall_update fills one per view on the stack, from the debug toggles (cSnowfallToggles_m_*) or the render
// state manager (cRenderStateMan_SnowIntensity ...), and passes the box's view entry to cSnowBox_update, which reads
// it.
struct tWorldPainterSnowfallParameters {
    float mFlakeRatio;              // 0x00 [O]; [U] cRenderStateMan_SnowIntensity; cSnowBox_update: flakes = it * 500
    float mFlakeWindRatio;          // 0x04 [O]; [U] cRenderStateMan_WindIntensity; flake wind = it * 500
    float mFluffRatio;              // 0x08 [O]; [U] cRenderStateMan_SnowFluffItensity; fluffs = it * 6
    float mFluffWindRatio;          // 0x0C [O]; [U] cRenderStateMan_SnowFluffWindIntensity; fluff wind = it * 250
    float mFlakeSize;               // 0x10 [O]; [U] cRenderStateMan_FlakeSize; copied to cSnowBox::mFlakeSize
    float mWindRotation;            // 0x14 [O]; [U] cRenderStateMan_WindRotation; the wind angle in degrees
    float mSnowFlurries;            // 0x18 [O]; [U] cRenderStateMan_SnowFlurries; the random share of the wind
    float mGravityMultiplier;       // 0x1C [O]; [U] cRenderStateMan_SnowGravityMultiplier; scales cSnowBox::mGravity
    float mFlakeColourA;            // 0x20 [O]; [U] the flake box's opacity
    float mFlakeColourR;            // 0x24 [O]
    float mFlakeColourG;            // 0x28 [O]
    float mFlakeColourB;            // 0x2C [O]
    float mFluffColourA;            // 0x30 [O]; [U] the fluff box's opacity
    float mFluffColourR;            // 0x34 [O]
    float mFluffColourG;            // 0x38 [O]
    float mFluffColourB;            // 0x3C [O]
};

CHECK_OFFSET(tWorldPainterSnowfallParameters, mFlakeRatio, 0x00);
CHECK_OFFSET(tWorldPainterSnowfallParameters, mGravityMultiplier, 0x1C);
CHECK_OFFSET(tWorldPainterSnowfallParameters, mFlakeColourA, 0x20);
CHECK_OFFSET(tWorldPainterSnowfallParameters, mFluffColourA, 0x30);
CHECK_OFFSET(tWorldPainterSnowfallParameters, mFluffColourB, 0x3C);
CHECK_SIZE(tWorldPainterSnowfallParameters, 0x40);

#endif

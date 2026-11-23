#ifndef RENDER_PS2GRAPHICSMAN_H
#define RENDER_PS2GRAPHICSMAN_H

// cPS2GraphicsMan: the PS2 renderer (one instance). It owns the matrix/viewport stacks, the projective textures,
// the deferred draw list (cPS2DeferManager), the texture manager, the VIF/GIF double buffers and the DMA kick thread
// that flips the frame. Size 0x75E0: cMemMan_alloc(0x75E0, "PS2GraphicsMan") + cPS2GraphicsMan ctor cPS2GraphicsMan_cPS2GraphicsMan in
// its create function cGraphicsMan_construct. Base class cGraphicsMan (ctor cGraphicsMan_cGraphicsMan, dtor cGraphicsMan__cGraphicsMan, vtable
// cGraphicsMan_vtbl): its data is 0x0-0x10D8 and it declares the virtuals, so the vptr is at +0x10D8 (cPS2GraphicsMan's
// vtable cPS2GraphicsMan_vtbl, 124 slots, stored by cPS2GraphicsMan_cPS2GraphicsMan). EA paths (preview asserts): source/render/graphicsman.cpp,
// source/render/graphicsman.h, source/render/ps2graphicsman.cpp/.h.
// Instance: cGraphicsMan::TheGraphicsMan = cGraphicsMan_TheGraphicsMan (gp+0x2A90), stored by the cGraphicsMan ctor (cGraphicsMan_cGraphicsMan)
// and cleared by its dtor (cGraphicsMan__cGraphicsMan); every holder points at this cPS2GraphicsMan.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail's constructors (cGraphicsMan_cGraphicsMan, cPS2GraphicsMan_cPS2GraphicsMan), Init (vtable slot 2, cPS2GraphicsMan_Init) and the allocation;
//   [P] the July 2003 preview's out-of-line accessors (GetView, GetDeferMan, GetTextureMan, GetNumProjTextures,
//       GetGuardBandMatrix, GetNetMatrix, GetTVDisplayFormat, GetIrradiance, Get/SetSpecID, SkipNextPageFlip,
//       ActiveDrawState, CurrentDrawState, GetCurrentFBFormat ...), matched to retail's own accessors in render/font
//       (0x394020-0x3946D8, 0x395318-0x3956E0) by body shape and order; the preview->retail shift is +0xB54 for the
//       cGraphicsMan block, +0xBD0 up to 0x5930, +0xC2C/+0xC30 for the frame-buffer block, +0xC40 from 0x6B90;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF: cGraphicsMan (same member order as retail's base, minus m_senseOfSpeed; retail's
//       tARGBs are 4-aligned, its sDrawState carries 2 texture ids) and cPSPGraphicsMan (a different platform class:
//       only DisplayModes/NumDisplayModes/CurrentDisplayMode carry over);
//   [X] refs/ssx3-rust docs/investigation/structs/render.md (names are guesses; used where retail code agrees).

#include "layout.h"
#include "bx/math3d.h"
#include "render/graphicsman.h"

struct cPS2DeferManager;
struct cTextureMan;

// One display mode of cPS2GraphicsMan::DisplayModes (EA's name: [O] tDisplayMode, the type of cPSPGraphicsMan's
// DisplayModes; On Tour's PSP record is 0x54 and only its leading name lines up). Size 0xA4: [P] GetScreenWidth /
// GetScreenHeight index DisplayModes[CurrentDisplayMode] by 0xA4; [U] cPS2GraphicsMan_GetDisplayModeName returns
// DisplayModes + mode * 0xA4, cPS2GraphicsMan_GetDisplayMode walks NumDisplayModes records of 0xA4.
struct tDisplayMode {
    char m_name[64];                // 0x00 [O] char m_name[64]; [U] GetDisplayModeName returns the record's start ([O]
                                    //      const char* GetDisplayModeName(int mode))
    char unk_040[0x40];             // 0x40 no retail access seen
    int unk_080;                    // 0x80 [U] SetDisplayMode1 passes it to func_0037BD98 as the display width
    int unk_084;                    // 0x84 [U] SetDisplayMode1: the display height
    int unk_088;                    // 0x88 [P] GetScreenWidth; [U] GetDisplayMode matches the width; func_0037BD98's
                                    //      frame-buffer width
    int unk_08C;                    // 0x8C [P] GetScreenHeight; [U] GetDisplayMode matches the height
    int unk_090;                    // 0x90 [U] the display pixel format (GetDisplayMode: 32 -> 0, 24 -> 1, 16 -> 2)
    int unk_094;                    // 0x94 [U] the frame-buffer pixel format (GetDisplayMode, SetDisplayMode1)
    int unk_098;                    // 0x98 [U] the z-buffer format (GetDisplayMode: 49 = PSMZ24)
    int unk_09C;                    // 0x9C [U] SetDisplayMode1: a flag passed with its inverse
    int unk_0A0;                    // 0xA0 [U] the mode id GetDisplayMode matches; SetDisplayMode1's last argument
};

CHECK_OFFSET(tDisplayMode, m_name, 0x00);
CHECK_OFFSET(tDisplayMode, unk_040, 0x40);
CHECK_OFFSET(tDisplayMode, unk_080, 0x80);
CHECK_OFFSET(tDisplayMode, unk_084, 0x84);
CHECK_OFFSET(tDisplayMode, unk_088, 0x88);
CHECK_OFFSET(tDisplayMode, unk_08C, 0x8C);
CHECK_OFFSET(tDisplayMode, unk_090, 0x90);
CHECK_OFFSET(tDisplayMode, unk_094, 0x94);
CHECK_OFFSET(tDisplayMode, unk_098, 0x98);
CHECK_OFFSET(tDisplayMode, unk_09C, 0x9C);
CHECK_OFFSET(tDisplayMode, unk_0A0, 0xA0);
CHECK_SIZE(tDisplayMode, 0xA4);

// [O] tTVDisplayFormat (0x14 in On Tour too). [P] GetTVDisplayFormat returns &self->mTVDisplayFormat.
struct tTVDisplayFormat {
    int mDisplayFormat;             // 0x00 [O] GMDisplayFormat
    float mfTVYOffset;              // 0x04 [O]
    float mfTVYScale;               // 0x08 [O]
    float mfXAspectAdjust;          // 0x0C [O]
    float mfYAspectAdjust;          // 0x10 [O]
};

// One projective texture (0xA0). [P] GetProjTexInfo(i) = self + 0x13F0 + i * 0xA0; the members follow
// cGraphicsMan::AddProjectiveTexture(const t4Vector&, const t4Matrix&, const t4Matrix&, float, int), which
// cPS2GraphicsMan_AddProjectiveTexture (cPS2GraphicsMan's override) copies in order. EA's type name is not known.
struct tProjTexInfo {
    t4Vector mShdwProjPos;                // 0x00 [U] cPS2GraphicsMan_AddProjectiveTexture (first argument)
    t4Matrix mShdwClipMat;                // 0x10 [U] cPS2GraphicsMan_AddProjectiveTexture (second argument); passed to slot 93 by cPS2GraphicsMan_CheckProjectiveTexturesHits
    t4Matrix mShdwProjMat;                // 0x50 [U] cPS2GraphicsMan_AddProjectiveTexture (third argument)
    float unk_90;                   // 0x90 [U] cPS2GraphicsMan_AddProjectiveTexture (float argument)
    int mShdwTexID;                     // 0x94 [U] cPS2GraphicsMan_AddProjectiveTexture (int argument)
    char unk_98[0x8];               // 0x98 padding to 16
};

// [P] cPS2GraphicsMan::sViewportStack (the preview's __as__Q215cPS2GraphicsMan14sViewportStack = cPS2GraphicsMan_sViewportStack_operatorE):
// one pushed viewport/projection state, 0x230 bytes. PushViewport (cPS2GraphicsMan_PushViewport) saves the live members listed
// below into it, PopViewport (cPS2GraphicsMan_PopViewport) restores them.
struct sViewportStack {
    float unk_000[4];               // 0x000 [U] cPS2GraphicsMan_SetViewportWindow; [X] viewport x, y, w, h
    float unk_010[3];               // 0x010 [U] cPS2GraphicsMan_SetViewportPerspective / cPS2GraphicsMan_SetViewportOrthographic store, cPS2GraphicsMan_GetViewportPerspective reads; [X] near?, 1.0, fov
    float unk_01C;                  // 0x01C [U] cPS2GraphicsMan_SetViewportPerspective / cPS2GraphicsMan_SetViewportZBias read; [R] Init zeroes entry 0
    int unk_020;                    // 0x020 [U] cPS2GraphicsMan_SetViewportPerspective / cPS2GraphicsMan_SetViewportOrthographic store; [X] projection type
    float unk_024;                  // 0x024 [U] copy of unk_590C
    float unk_028;                  // 0x028 [U] copy of unk_5908
    float unk_02C;                  // 0x02C [U] copy of mFar? (0x5910)
    t4Vector unk_030;               // 0x030 [U] copy of unk_6B70
    t4Vector unk_040;               // 0x040 [U] copy of unk_6B80
    t4Vector unk_050;               // 0x050 [U] copy of unk_5920
    t4Matrix mScreenMatrix;         // 0x060 [U] copy of mScreenMatrix
    t4Matrix mWorld;                // 0x0A0 [U] copy of mWorld
    t4Matrix mView;                 // 0x0E0 [U] copy of mView
    t4Matrix mGuardBandMatrix;      // 0x120 [U] copy of mGuardBandMatrix
    t4Vector mGuardScale;           // 0x160 [U] copy of mGuardScale
    t4Vector mGuardOffset;          // 0x170 [U] copy of mGuardOffset
    t4Matrix mViewProj;             // 0x180 [U] copy of mViewProj
    t4Matrix mNetMatrix;            // 0x1C0 [U] copy of mNetMatrix
    t4Vector mScreenScale;          // 0x200 [U] copy of mScreenScale
    t4Vector mScreenOffset;         // 0x210 [U] copy of mScreenOffset
    unsigned short mScissor[4];     // 0x220 [U] copy of mScissor
    int mDeferScissorIndex;         // 0x228 [U] the DeferMan's scissor index (+0x69CC4); -1 = re-register
    char unk_22C[0x4];              // 0x22C padding to 16
};

class cPS2GraphicsMan : public cGraphicsMan {
public:
    // 0x0000-0x10DB: cGraphicsMan (render/graphicsman.h), the vptr at 0x10D8
    int mViewportDepth;             // 0x10DC [U] index into mViewportStack (cPS2GraphicsMan_PushViewport/cPS2GraphicsMan_PopViewport); [X] viewDepth
    t4Matrix mViewStack[12];        // 0x10E0 [R] ctor's first array loop (12); [U] PushView/PopView step 0x40
    int mViewStackIdx;                 // 0x13E0 [P] asserts "mViewStackIdx < kViewMatrixStackSize", "mViewStackIdx > 0"; [U] PushView (cPS2GraphicsMan_PushView) +1, PopView -1; [X] matrixDepth
    t4Matrix* mCurrentView;         // 0x13E4 [P] GetView; [R] Init points it at mViewStack
    void* mpScratchpad;             // 0x13E8 [R] Init stores 0x70000000; [X] spad
    int mNumShdw;           // 0x13EC [P] assert "Index < mNumShdw"; GetNumProjTextures / ClearProjTextures
    tProjTexInfo mProjTextures[8];  // 0x13F0 [P] GetProjTexInfo; [R] ctor's second array loop (8)
    cPS2DeferManager* mpDeferMan;   // 0x18F0 [P] GetDeferMan (cPS2GraphicsMan_GetDeferMan); [X] "DEFER MAN" (0x6B100)
    cTextureMan* mTexMan;      // 0x18F4 [P] asserts "mTexMan->GetTextureWidth(iTexId) == ..."; GetTextureMan (cPS2GraphicsMan_GetTextureMan); [X] "TEX MAN" (0x6740)
    int unk_18F8[2000];             // 0x18F8 [U] per texture id: cPS2GraphicsMan_PlatformPurgeTextures frees each non-zero one, cPS2GraphicsMan_RemoveTexture clears it
    int unk_3838[2000];             // 0x3838 [U] per texture id: cPS2GraphicsMan_AddTexture1 sets 1 after creating it
    char unk_5778[0x8];             // 0x5778 padding to 16
    t4Matrix mWorld;                // 0x5780 [X] world (copy of *mCurrentView); [U] cPS2GraphicsMan_PopViewport restores it
    t4Matrix mView;                 // 0x57C0 [X] view; [U] cPS2GraphicsMan_SetViewportPerspective / cPS2GraphicsMan_SetViewportOrthographic build it
    t4Matrix mGuardBandMatrix;      // 0x5800 [P] GetGuardBandMatrix returns its address (cPS2GraphicsMan_GetGuardBandMatrix)
    t4Vector mGuardScale;           // 0x5840 [X] guardScale (uploaded to VU1)
    t4Vector mGuardOffset;          // 0x5850 [X] guardOffset
    t4Matrix mViewProj;             // 0x5860 [X] viewProj
    t4Matrix mNetMatrix;            // 0x58A0 [P] GetNetMatrix copies it after updateNetMat (cPS2GraphicsMan_GetNetMatrix)
    t4Vector mScreenScale;          // 0x58E0 [X] screenScale
    t4Vector mScreenOffset;         // 0x58F0 [X] screenOffset
    unsigned short mScissor[4];     // 0x5900 [X] scissor x, y, w, h; [U] func_00376938, func_0036B158
    float unk_5908;                 // 0x5908 [U] saved by cPS2GraphicsMan_PushViewport; [X] fovOrAspect?
    float unk_590C;                 // 0x590C [U] saved by cPS2GraphicsMan_PushViewport
    float unk_5910;                 // 0x5910 [U] saved by cPS2GraphicsMan_PushViewport; [X] far
    char unk_5914[0xC];             // 0x5914 padding to 16
    t4Vector unk_5920;              // 0x5920 [U] saved by cPS2GraphicsMan_PushViewport; cPS2GraphicsMan_CalcSortZ
    t4Matrix unk_5930[2];           // 0x5930 [R] ctor's third array loop (2); [U] per view, func_0036A428; [X] viewMatrices
    unsigned short unk_59B0[2][4];  // 0x59B0 [R] ctor's fourth array loop (2); [U] per-view scissor, func_0036B158/func_0036C790
    int unk_59C0;                   // 0x59C0 [U] accessor func_003956D8; func_00363490 loop count; [X] numViews (split screen)
    int NumDisplayModes;            // 0x59C4 [O] cPSPGraphicsMan; [P] GetNumDisplayModes (cPS2GraphicsMan_GetNumDisplayModes)
    int CurrentDisplayMode;         // 0x59C8 [O]; [P] GetDisplayMode (cPS2GraphicsMan_GetDisplayMode1); [R] Init stores -1
    tDisplayMode* DisplayModes;     // 0x59CC [O]; [P] GetScreenWidth/Height read [CurrentDisplayMode] (0xA4 each)
    int unk_59D0;                   // 0x59D0 [R] Init; [U] func_0037BD98; [X] dither (DTHE)
    char unk_59D4[0xC];             // 0x59D4
    unsigned long unk_59E0;         // 0x59E0 (64-bit: EE long) [U] func_0037C198 ld/sd; [X] dimx (DIMX)
    int mFilterEnabled;             // 0x59E8 [P] EnableFilter/DisableFilter (cPS2GraphicsMan_EnableFilter/cPS2GraphicsMan_DisableFilter); [X] blitToDisplay
    int unk_59EC[3];                // 0x59EC [U] func_0037C570; [X] brightness r, g, b
    int unk_59F8;                   // 0x59F8 [U] func_0037C720 / func_0037C770; [X] alpha
    float mScreenBrightness;        // 0x59FC [P] Get/SetScreenBrightness (cPS2GraphicsMan_GetScreenBrightness/cPS2GraphicsMan_SetScreenBrightness)
    unsigned int mVifCursor;        // 0x5A00 [X] vifCursor; [U] draw slots append packets here
    unsigned int mVifHalfStart;     // 0x5A04 [X]; [R] Init
    void* mpVifBuffRaw;             // 0x5A08 [X]; [R] Init: operator new(0x199940, "VifBuff")
    unsigned int mVifBuffBase;      // 0x5A0C [X]; [R] Init: raw rounded up to 128
    int mBuildIndex;                // 0x5A10 [X]; [U] func_003826E0
    unsigned int mGifCursor;        // 0x5A14 [X]
    unsigned int mGifHalfStart;     // 0x5A18 [X]
    void* mpGifBuffRaw;             // 0x5A1C [X]; [R] Init: operator new(0x4CC40, "GifBuff")
    unsigned int mGifBuffBase;      // 0x5A20 [X]
    int mGifBuildIndex;             // 0x5A24 [X]
    unsigned int unk_5A28;          // 0x5A28 [U] func_0037A260 / func_0037A430
    unsigned long* unk_5A2C;        // 0x5A2C [U] func_0037A260 / func_0037A430
    int mFBWidth;                   // 0x5A30 [X] fbWidth; [U] func_0037BD98 (SetDisplayMode)
    int mFBHeight;                  // 0x5A34 [X] fbHeight
    int unk_5A38;                   // 0x5A38 [U] func_0037BD98; [X] displayPsm
    int mCurrentFBFormat;           // 0x5A3C [P] GetCurrentFBFormat / GetPSMFormat (cPS2GraphicsMan_GetCurrentFBFormat, cPS2GraphicsMan_GetPSMFormat)
    int mCurrentZBFormat;           // 0x5A40 [P] GetCurrentZBFormat (cPS2GraphicsMan_GetCurrentZBFormat; layout_xlate "seen")
    short unk_5A44;                 // 0x5A44 [U] func_0037BD98 / func_00382AF0; [X] interlace
    short unk_5A46;                 // 0x5A46 [X] ffmd
    short unk_5A48;                 // 0x5A48 [X] omode
    char unk_5A4A[0x2];             // 0x5A4A padding
    int unk_5A4C;                   // 0x5A4C [U] accessor cPS2GraphicsMan_GetDTV; [X] progressive
    int mFrameBufferWidth;          // 0x5A50 [P] GetFrameBufferWidth (cPS2GraphicsMan_GetFrameBufferWidth, by position; shift +0xC30); [X] fbw
    int unk_5A54;                   // 0x5A54 [X] dispWidth
    int unk_5A58;                   // 0x5A58 [X] dispHeight
    int unk_5A5C;                   // 0x5A5C [U] accessor func_00395550; [X] dispFbw
    int unk_5A60;                   // 0x5A60 [X] magv
    int unk_5A64;                   // 0x5A64 [X] magh
    int unk_5A68;                   // 0x5A68 [X] displayPages
    int unk_5A6C;                   // 0x5A6C [X] drawPages
    int unk_5A70;                   // 0x5A70 [X] zPages
    int mCurrentFrameCount;         // 0x5A74 [P] getCurrentFrameCount (cPS2GraphicsMan_getCurrentFrameCount); [X] vsyncFlipCount
    int unk_5A78;                   // 0x5A78 [X] fbp0
    int unk_5A7C;                   // 0x5A7C [X] fbp1
    int mCurrentZBufferPage;        // 0x5A80 [P] GetCurrentZBufferPage (cPS2GraphicsMan_GetCurrentZBufferPage)
    int mCurrentFrameBufferPage;    // 0x5A84 [P] GetCurrentFrameBufferPage (cPS2GraphicsMan_GetCurrentFrameBufferPage)
    int mDisplayBufferAddress;      // 0x5A88 [P] GetDisplayBufferAddress (cPS2GraphicsMan_GetDisplayBufferAddress)
    volatile int mKickState;        // 0x5A8C [X] kickState; [U] the DMA kick thread and IRQ handlers
    volatile int mBufState[2];      // 0x5A90 [X] bufState
    volatile int mInFlightIndex;    // 0x5A98 [X]
    int mInFlightVif2;              // 0x5A9C [X]
    int mInFlightGif2;              // 0x5AA0 [X]
    volatile int mQueuedIndex;      // 0x5AA4 [X]
    int mQueuedVif1;                // 0x5AA8 [X]
    int mQueuedVif2;                // 0x5AAC [X]
    int mQueuedGif1;                // 0x5AB0 [X]
    int mQueuedGif2;                // 0x5AB4 [X]
    int mMinFlipWait;               // 0x5AB8 [P] SetMinFlipWait (cPS2GraphicsMan_SetMinFlipWait; layout_xlate "seen"); [X] vblanksPerFlip
    int mVblankCounter;             // 0x5ABC [X]; [U] func_003825F8
    int mIntcHandlerId;             // 0x5AC0 [X]; [U] cPS2GraphicsMan_Purge (vtable slot 3) removes it
    int mDmacHandlerId;             // 0x5AC4 [X]; [U] ditto
    int mKickSema;                  // 0x5AC8 [X]; [U] ditto
    int mQueueMutex;                // 0x5ACC [X]; [U] ditto
    int mKickThreadId;              // 0x5AD0 [X]; [U] ditto
    char unk_5AD4[0xC];             // 0x5AD4 padding to 16
    char mKickThreadStack[0x1000];  // 0x5AE0 [X]; [R] Init passes self + 0x5AE0
    unsigned int mClearColour;      // 0x6AE0 [U] cPS2GraphicsMan_SetScreenClearColour (slot 15, [X] SetClearColor) stores it
    char unk_6AE4[0xC];             // 0x6AE4 padding to 16
    t4Matrix mScreenMatrix;         // 0x6AF0 [X] screenMatrix (clip -> GS coordinates)
    t4Matrix mScreenWorld;          // 0x6B30 [X] screenWorld; [U] the draw slots load it
    t4Vector unk_6B70;              // 0x6B70 [U] saved by cPS2GraphicsMan_PushViewport; cPS2GraphicsMan_projOutCode; [X] screenOffset4
    t4Vector unk_6B80;              // 0x6B80 [U] saved by cPS2GraphicsMan_PushViewport
    int mMatricesValid;             // 0x6B90 [P] updateNetMat recomputes when 0; [U] PopView clears it; [X] matricesValid
    tTVDisplayFormat mTVDisplayFormat; // 0x6B94 [P] GetTVDisplayFormat returns its address (cPS2GraphicsMan_GetTVDisplayFormat)
    char unk_6BA8[0x8];             // 0x6BA8 padding to 16
    char mIrradianceInfo[0xA0];     // 0x6BB0 [P] Get/SetIrradiance (tIrradianceInfo, copied as 10 qwords)
    int unk_6C50;                   // 0x6C50 [R] Init zeroes it
    int unk_6C54[4];                // 0x6C54 [R] Init zeroes them
    char unk_6C64[0x40];            // 0x6C64 [U] per-view depth-fog entries, 0x20 each (func_0036ABA0, func_0036A428)
    float unk_6CA4[2][6];           // 0x6CA4 [U] per-view screen-tint entries (colour r, g, b, fill r, g, b): func_00390458
                                    //        writes cScreenTint_m_ColourR..m_FillB at +0x6CA4 + view * 0x18; func_003904A0
                                    //        reads the six floats (skips the neutral 1, 1, 1 / 0, 0, 0)
    float unk_6CD4[2][7];           // 0x6CD4 [U] per-view bloom parameters (func_0036C740, func_0036C790)
    void (*mPhase2Callback)(int);   // 0x6D0C [X] phase2Callback; [U] cPS2GraphicsMan_SetLensFXCallBack sets, func_00382760 calls
    int mPhase2CallbackArg;         // 0x6D10 [X]
    int mShinyBoarder;              // 0x6D14 [P] SetShinyBoarder (cPS2GraphicsMan_SetShinyBoarder) bool
    char unk_6D18[0x8];             // 0x6D18 padding to 16
    sViewportStack mViewportStack[4]; // 0x6D20 [P] sViewportStack; [R] ctor's fifth array loop (4); [U] mViewportDepth
    // [R] cPS2GraphicsMan_vtbl's slots past cGraphicsMan_vtbl's 121
    virtual void* SetDisplayMode();                                                                      // slot 122: [R] cPS2GraphicsMan_SetDisplayMode
    virtual int GetDisplayMode(int width, int height, unsigned int bpp, unsigned int clutBpp, int fmt, int id); // slot 123: [R] cPS2GraphicsMan_GetDisplayMode
    virtual void InitCharMatrices();                                                                     // slot 124: [R] cPS2GraphicsMan_InitCharMatrices
};

// --- layout checks

CHECK_OFFSET(tTVDisplayFormat, mDisplayFormat, 0x00);
CHECK_OFFSET(tTVDisplayFormat, mfTVYOffset, 0x04);
CHECK_OFFSET(tTVDisplayFormat, mfTVYScale, 0x08);
CHECK_OFFSET(tTVDisplayFormat, mfXAspectAdjust, 0x0C);
CHECK_OFFSET(tTVDisplayFormat, mfYAspectAdjust, 0x10);
CHECK_SIZE(tTVDisplayFormat, 0x14);

CHECK_OFFSET(tProjTexInfo, mShdwProjPos, 0x00);
CHECK_OFFSET(tProjTexInfo, mShdwClipMat, 0x10);
CHECK_OFFSET(tProjTexInfo, mShdwProjMat, 0x50);
CHECK_OFFSET(tProjTexInfo, unk_90, 0x90);
CHECK_OFFSET(tProjTexInfo, mShdwTexID, 0x94);
CHECK_OFFSET(tProjTexInfo, unk_98, 0x98);
CHECK_SIZE(tProjTexInfo, 0xA0);

CHECK_OFFSET(sViewportStack, unk_000, 0x000);
CHECK_OFFSET(sViewportStack, unk_010, 0x010);
CHECK_OFFSET(sViewportStack, unk_01C, 0x01C);
CHECK_OFFSET(sViewportStack, unk_020, 0x020);
CHECK_OFFSET(sViewportStack, unk_024, 0x024);
CHECK_OFFSET(sViewportStack, unk_028, 0x028);
CHECK_OFFSET(sViewportStack, unk_02C, 0x02C);
CHECK_OFFSET(sViewportStack, unk_030, 0x030);
CHECK_OFFSET(sViewportStack, unk_040, 0x040);
CHECK_OFFSET(sViewportStack, unk_050, 0x050);
CHECK_OFFSET(sViewportStack, mScreenMatrix, 0x060);
CHECK_OFFSET(sViewportStack, mWorld, 0x0A0);
CHECK_OFFSET(sViewportStack, mView, 0x0E0);
CHECK_OFFSET(sViewportStack, mGuardBandMatrix, 0x120);
CHECK_OFFSET(sViewportStack, mGuardScale, 0x160);
CHECK_OFFSET(sViewportStack, mGuardOffset, 0x170);
CHECK_OFFSET(sViewportStack, mViewProj, 0x180);
CHECK_OFFSET(sViewportStack, mNetMatrix, 0x1C0);
CHECK_OFFSET(sViewportStack, mScreenScale, 0x200);
CHECK_OFFSET(sViewportStack, mScreenOffset, 0x210);
CHECK_OFFSET(sViewportStack, mScissor, 0x220);
CHECK_OFFSET(sViewportStack, mDeferScissorIndex, 0x228);
CHECK_OFFSET(sViewportStack, unk_22C, 0x22C);
CHECK_SIZE(sViewportStack, 0x230);

CHECK_OFFSET(cPS2GraphicsMan, m_screenOffset, 0x000);
CHECK_OFFSET(cPS2GraphicsMan, m_AIUpdatesPerRender, 0x008);
CHECK_OFFSET(cPS2GraphicsMan, m_PatchSpecularTextureId, 0x02C);
CHECK_OFFSET(cPS2GraphicsMan, m_SnowSparkleTextureId, 0x04C);
CHECK_OFFSET(cPS2GraphicsMan, m_RimTextureId, 0x06C);
CHECK_OFFSET(cPS2GraphicsMan, m_isInNIS, 0x08C);
CHECK_OFFSET(cPS2GraphicsMan, m_DrawDistance, 0x0CC);
CHECK_OFFSET(cPS2GraphicsMan, m_FogDensity, 0x0EC);
CHECK_OFFSET(cPS2GraphicsMan, m_FogColourR, 0x10C);
CHECK_OFFSET(cPS2GraphicsMan, m_FogColourG, 0x12C);
CHECK_OFFSET(cPS2GraphicsMan, m_FogColourB, 0x14C);
CHECK_OFFSET(cPS2GraphicsMan, m_FogNear, 0x16C);
CHECK_OFFSET(cPS2GraphicsMan, m_FogFar, 0x18C);
CHECK_OFFSET(cPS2GraphicsMan, m_ScreenTintScaleR, 0x1AC);
CHECK_OFFSET(cPS2GraphicsMan, m_ScreenTintScaleG, 0x1CC);
CHECK_OFFSET(cPS2GraphicsMan, m_ScreenTintScaleB, 0x1EC);
CHECK_OFFSET(cPS2GraphicsMan, m_ScreenTintOffsetR, 0x20C);
CHECK_OFFSET(cPS2GraphicsMan, m_ScreenTintOffsetG, 0x22C);
CHECK_OFFSET(cPS2GraphicsMan, m_ScreenTintOffsetB, 0x24C);
CHECK_OFFSET(cPS2GraphicsMan, mFarPlane, 0x26C);
CHECK_OFFSET(cPS2GraphicsMan, mViewIndex, 0x274);
CHECK_OFFSET(cPS2GraphicsMan, mNumTextureFiles, 0x278);
CHECK_OFFSET(cPS2GraphicsMan, mTextureFiles, 0x27C);
CHECK_OFFSET(cPS2GraphicsMan, mNumTexRefs, 0xE80);
CHECK_OFFSET(cPS2GraphicsMan, m_pCurrentDrawState, 0xE84);
CHECK_OFFSET(cPS2GraphicsMan, m_activeDrawState, 0xED8);
CHECK_OFFSET(cPS2GraphicsMan, m_pCurrentTextureState, 0xEEC);
CHECK_OFFSET(cPS2GraphicsMan, m_textureStateStack, 0xEF0);
CHECK_OFFSET(cPS2GraphicsMan, m_activeTextureState, 0xF00);
CHECK_OFFSET(cPS2GraphicsMan, m_constantColours, 0xF04);
CHECK_OFFSET(cPS2GraphicsMan, m_lastConstantColours, 0xF24);
CHECK_OFFSET(cPS2GraphicsMan, mSkipNextPageFlip, 0xF44);
CHECK_OFFSET(cPS2GraphicsMan, m_blendedMatrixHistory, 0xF48);
CHECK_OFFSET(cPS2GraphicsMan, m_blendedMatrixHistorySize, 0xF4C);
CHECK_OFFSET(cPS2GraphicsMan, mSpecTexID, 0x10A8);
CHECK_OFFSET(cPS2GraphicsMan, mPatchSpecTexID, 0x10AC);
CHECK_OFFSET(cPS2GraphicsMan, mViewportDepth, 0x10DC);
CHECK_OFFSET(cPS2GraphicsMan, mViewStack, 0x10E0);
CHECK_OFFSET(cPS2GraphicsMan, mViewStackIdx, 0x13E0);
CHECK_OFFSET(cPS2GraphicsMan, mCurrentView, 0x13E4);
CHECK_OFFSET(cPS2GraphicsMan, mpScratchpad, 0x13E8);
CHECK_OFFSET(cPS2GraphicsMan, mNumShdw, 0x13EC);
CHECK_OFFSET(cPS2GraphicsMan, mProjTextures, 0x13F0);
CHECK_OFFSET(cPS2GraphicsMan, mpDeferMan, 0x18F0);
CHECK_OFFSET(cPS2GraphicsMan, mTexMan, 0x18F4);
CHECK_OFFSET(cPS2GraphicsMan, unk_18F8, 0x18F8);
CHECK_OFFSET(cPS2GraphicsMan, unk_3838, 0x3838);
CHECK_OFFSET(cPS2GraphicsMan, unk_5778, 0x5778);
CHECK_OFFSET(cPS2GraphicsMan, mWorld, 0x5780);
CHECK_OFFSET(cPS2GraphicsMan, mView, 0x57C0);
CHECK_OFFSET(cPS2GraphicsMan, mGuardBandMatrix, 0x5800);
CHECK_OFFSET(cPS2GraphicsMan, mGuardScale, 0x5840);
CHECK_OFFSET(cPS2GraphicsMan, mGuardOffset, 0x5850);
CHECK_OFFSET(cPS2GraphicsMan, mViewProj, 0x5860);
CHECK_OFFSET(cPS2GraphicsMan, mNetMatrix, 0x58A0);
CHECK_OFFSET(cPS2GraphicsMan, mScreenScale, 0x58E0);
CHECK_OFFSET(cPS2GraphicsMan, mScreenOffset, 0x58F0);
CHECK_OFFSET(cPS2GraphicsMan, mScissor, 0x5900);
CHECK_OFFSET(cPS2GraphicsMan, unk_5908, 0x5908);
CHECK_OFFSET(cPS2GraphicsMan, unk_590C, 0x590C);
CHECK_OFFSET(cPS2GraphicsMan, unk_5910, 0x5910);
CHECK_OFFSET(cPS2GraphicsMan, unk_5914, 0x5914);
CHECK_OFFSET(cPS2GraphicsMan, unk_5920, 0x5920);
CHECK_OFFSET(cPS2GraphicsMan, unk_5930, 0x5930);
CHECK_OFFSET(cPS2GraphicsMan, unk_59B0, 0x59B0);
CHECK_OFFSET(cPS2GraphicsMan, unk_59C0, 0x59C0);
CHECK_OFFSET(cPS2GraphicsMan, NumDisplayModes, 0x59C4);
CHECK_OFFSET(cPS2GraphicsMan, CurrentDisplayMode, 0x59C8);
CHECK_OFFSET(cPS2GraphicsMan, DisplayModes, 0x59CC);
CHECK_OFFSET(cPS2GraphicsMan, unk_59D0, 0x59D0);
CHECK_OFFSET(cPS2GraphicsMan, unk_59D4, 0x59D4);
CHECK_OFFSET(cPS2GraphicsMan, unk_59E0, 0x59E0);
CHECK_OFFSET(cPS2GraphicsMan, mFilterEnabled, 0x59E8);
CHECK_OFFSET(cPS2GraphicsMan, unk_59EC, 0x59EC);
CHECK_OFFSET(cPS2GraphicsMan, unk_59F8, 0x59F8);
CHECK_OFFSET(cPS2GraphicsMan, mScreenBrightness, 0x59FC);
CHECK_OFFSET(cPS2GraphicsMan, mVifCursor, 0x5A00);
CHECK_OFFSET(cPS2GraphicsMan, mVifHalfStart, 0x5A04);
CHECK_OFFSET(cPS2GraphicsMan, mpVifBuffRaw, 0x5A08);
CHECK_OFFSET(cPS2GraphicsMan, mVifBuffBase, 0x5A0C);
CHECK_OFFSET(cPS2GraphicsMan, mBuildIndex, 0x5A10);
CHECK_OFFSET(cPS2GraphicsMan, mGifCursor, 0x5A14);
CHECK_OFFSET(cPS2GraphicsMan, mGifHalfStart, 0x5A18);
CHECK_OFFSET(cPS2GraphicsMan, mpGifBuffRaw, 0x5A1C);
CHECK_OFFSET(cPS2GraphicsMan, mGifBuffBase, 0x5A20);
CHECK_OFFSET(cPS2GraphicsMan, mGifBuildIndex, 0x5A24);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A28, 0x5A28);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A2C, 0x5A2C);
CHECK_OFFSET(cPS2GraphicsMan, mFBWidth, 0x5A30);
CHECK_OFFSET(cPS2GraphicsMan, mFBHeight, 0x5A34);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A38, 0x5A38);
CHECK_OFFSET(cPS2GraphicsMan, mCurrentFBFormat, 0x5A3C);
CHECK_OFFSET(cPS2GraphicsMan, mCurrentZBFormat, 0x5A40);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A44, 0x5A44);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A46, 0x5A46);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A48, 0x5A48);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A4A, 0x5A4A);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A4C, 0x5A4C);
CHECK_OFFSET(cPS2GraphicsMan, mFrameBufferWidth, 0x5A50);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A54, 0x5A54);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A58, 0x5A58);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A5C, 0x5A5C);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A60, 0x5A60);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A64, 0x5A64);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A68, 0x5A68);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A6C, 0x5A6C);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A70, 0x5A70);
CHECK_OFFSET(cPS2GraphicsMan, mCurrentFrameCount, 0x5A74);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A78, 0x5A78);
CHECK_OFFSET(cPS2GraphicsMan, unk_5A7C, 0x5A7C);
CHECK_OFFSET(cPS2GraphicsMan, mCurrentZBufferPage, 0x5A80);
CHECK_OFFSET(cPS2GraphicsMan, mCurrentFrameBufferPage, 0x5A84);
CHECK_OFFSET(cPS2GraphicsMan, mDisplayBufferAddress, 0x5A88);
CHECK_OFFSET(cPS2GraphicsMan, mKickState, 0x5A8C);
CHECK_OFFSET(cPS2GraphicsMan, mBufState, 0x5A90);
CHECK_OFFSET(cPS2GraphicsMan, mInFlightIndex, 0x5A98);
CHECK_OFFSET(cPS2GraphicsMan, mInFlightVif2, 0x5A9C);
CHECK_OFFSET(cPS2GraphicsMan, mInFlightGif2, 0x5AA0);
CHECK_OFFSET(cPS2GraphicsMan, mQueuedIndex, 0x5AA4);
CHECK_OFFSET(cPS2GraphicsMan, mQueuedVif1, 0x5AA8);
CHECK_OFFSET(cPS2GraphicsMan, mQueuedVif2, 0x5AAC);
CHECK_OFFSET(cPS2GraphicsMan, mQueuedGif1, 0x5AB0);
CHECK_OFFSET(cPS2GraphicsMan, mQueuedGif2, 0x5AB4);
CHECK_OFFSET(cPS2GraphicsMan, mMinFlipWait, 0x5AB8);
CHECK_OFFSET(cPS2GraphicsMan, mVblankCounter, 0x5ABC);
CHECK_OFFSET(cPS2GraphicsMan, mIntcHandlerId, 0x5AC0);
CHECK_OFFSET(cPS2GraphicsMan, mDmacHandlerId, 0x5AC4);
CHECK_OFFSET(cPS2GraphicsMan, mKickSema, 0x5AC8);
CHECK_OFFSET(cPS2GraphicsMan, mQueueMutex, 0x5ACC);
CHECK_OFFSET(cPS2GraphicsMan, mKickThreadId, 0x5AD0);
CHECK_OFFSET(cPS2GraphicsMan, unk_5AD4, 0x5AD4);
CHECK_OFFSET(cPS2GraphicsMan, mKickThreadStack, 0x5AE0);
CHECK_OFFSET(cPS2GraphicsMan, mClearColour, 0x6AE0);
CHECK_OFFSET(cPS2GraphicsMan, unk_6AE4, 0x6AE4);
CHECK_OFFSET(cPS2GraphicsMan, mScreenMatrix, 0x6AF0);
CHECK_OFFSET(cPS2GraphicsMan, mScreenWorld, 0x6B30);
CHECK_OFFSET(cPS2GraphicsMan, unk_6B70, 0x6B70);
CHECK_OFFSET(cPS2GraphicsMan, unk_6B80, 0x6B80);
CHECK_OFFSET(cPS2GraphicsMan, mMatricesValid, 0x6B90);
CHECK_OFFSET(cPS2GraphicsMan, mTVDisplayFormat, 0x6B94);
CHECK_OFFSET(cPS2GraphicsMan, unk_6BA8, 0x6BA8);
CHECK_OFFSET(cPS2GraphicsMan, mIrradianceInfo, 0x6BB0);
CHECK_OFFSET(cPS2GraphicsMan, unk_6C50, 0x6C50);
CHECK_OFFSET(cPS2GraphicsMan, unk_6C54, 0x6C54);
CHECK_OFFSET(cPS2GraphicsMan, unk_6C64, 0x6C64);
CHECK_OFFSET(cPS2GraphicsMan, unk_6CA4, 0x6CA4);
CHECK_OFFSET(cPS2GraphicsMan, unk_6CD4, 0x6CD4);
CHECK_OFFSET(cPS2GraphicsMan, mPhase2Callback, 0x6D0C);
CHECK_OFFSET(cPS2GraphicsMan, mPhase2CallbackArg, 0x6D10);
CHECK_OFFSET(cPS2GraphicsMan, mShinyBoarder, 0x6D14);
CHECK_OFFSET(cPS2GraphicsMan, unk_6D18, 0x6D18);
CHECK_OFFSET(cPS2GraphicsMan, mViewportStack, 0x6D20);
CHECK_SIZE(cPS2GraphicsMan, 0x75E0);

// The graphics manager: GrMan holds what cSSXApp_init got from cGraphicsMan::construct() (On Tour: GrMan), and
// the cGraphicsMan constructor stores itself in the class static (cGraphicsMan::TheGraphicsMan). Both point to the
// one cPS2GraphicsMan.
extern cPS2GraphicsMan* GrMan;
extern cPS2GraphicsMan* cGraphicsMan_TheGraphicsMan;

#endif /* RENDER_PS2GRAPHICSMAN_H */

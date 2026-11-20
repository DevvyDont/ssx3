#ifndef RENDER_GRAPHICSMAN_H
#define RENDER_GRAPHICSMAN_H

// cGraphicsMan: the renderer's platform-independent base class (EA source/render/graphicsman.cpp / .h, preview
// asserts): per-view settings (specular / sparkle / rim textures, NIS flag, snow sparkle density, draw distance, fog,
// screen tint), the view count and index, the texture reference table, the draw-state and texture-state stacks, the
// constant colours, the blended-matrix history and the effect / specular texture maps. The one instance is the
// cPS2GraphicsMan (render/ps2graphicsman.h), so cGraphicsMan::TheGraphicsMan (cGraphicsMan_TheGraphicsMan) points at
// it; this struct is that object's first 0x10DC bytes. Methods: the accessors in render/font (0x394020-0x3946D8),
// render/graphicsman and render/ps2graphicsman.
//
// A FLAT copy of cPS2GraphicsMan's cGraphicsMan block (same members, offsets and evidence); keep the two in step.
// On Tour's cGraphicsMan has the same member order (size 0x10D0 there; retail's vptr ends it at 0x10DC).

#include "layout.h"
#include "render/ps2graphicsman.h"

struct cGraphicsMan {
    float m_screenOffset[2];        // 0x000 [O] t2Vector; [P] Get/SetScreenOffset (cGraphicsMan_SetScreenOffset/FD8/FF0)
    int m_AIUpdatesPerRender;       // 0x008 [O]; [R] ctor zeroes it
    int m_SpecularTextureId[8];     // 0x00C [O]; [P] get/setSpecularTextureId (preview: one int); [U] cVisualEffectsMan ctor
    int m_PatchSpecularTextureId[8]; // 0x02C [O]; [U] cVisualEffectsMan ctor stores [6]
    int m_SnowSparkleTextureId[8];  // 0x04C [O]; [U] ditto
    int m_RimTextureId[8];          // 0x06C [O]; [U] ditto
    int m_isInNIS[8];               // 0x08C [O]; [U] cVisualEffectsMan_commonUpdate stores [i]
    float m_SnowSparkleDensity[8];  // 0x0AC [O]; [U] cVisualEffectsMan_commonUpdate stores [i]; func_0038D690 reads [6]
                                    //       (+0xC4) as the patch subdivision scale
    float m_DrawDistance[8];        // 0x0CC [O]; [U] ditto
    float m_FogDensity[8];          // 0x0EC [O]; [U] ditto
    float m_FogColourR[8];          // 0x10C [O]; [U] ditto
    float m_FogColourG[8];          // 0x12C [O]; [U] ditto
    float m_FogColourB[8];          // 0x14C [O]; [U] ditto
    float m_FogNear[8];             // 0x16C [O]; [U] ditto
    float m_FogFar[8];              // 0x18C [O]; [U] ditto
    float m_ScreenTintScaleR[8];    // 0x1AC [O]; [U] ditto
    float m_ScreenTintScaleG[8];    // 0x1CC [O]; [U] ditto
    float m_ScreenTintScaleB[8];    // 0x1EC [O]; [U] ditto
    float m_ScreenTintOffsetR[8];   // 0x20C [O]; [U] ditto
    float m_ScreenTintOffsetG[8];   // 0x22C [O]; [U] ditto
    float m_ScreenTintOffsetB[8];   // 0x24C [O]; [U] ditto
    float mFarPlane;                // 0x26C [O]; [R] ctor stores 0
    int mNumViews;                  // 0x270 [O]; [R] ctor stores 1; accessors cGraphicsMan_SetNumViews/cGraphicsMan_GetNumViews
    int mViewIndex;                 // 0x274 [O]; [R] ctor stores 0; accessors cGraphicsMan_SetViewIndex/cGraphicsMan_GetViewIndex
    int mNumTextureFiles;           // 0x278 [O]; [U] cGraphicsMan_InitTextureFiles / cGraphicsMan_GetTextureID
    void** mTextureFiles;           // 0x27C [O] const char**; [U] cGraphicsMan_GetTextureID passes each to SHAPE_locatez
    tTextureRef mTexRefs[256];      // 0x280 [O]; [U] cGraphicsMan_GetTextureID
    int mNumTexRefs;                // 0xE80 [O]; [P] GetNumTexRefs (cGraphicsMan_GetNumTexRefs)
    sDrawState* m_pCurrentDrawState; // 0xE84 [O]; [P] CurrentDrawState; [R] ctor points it at the stack
    sDrawState m_drawStateStack[4]; // 0xE88 [O]; [R] ctor fills the default state (sDrawState_s_defaultDrawState)
    sDrawState m_activeDrawState;   // 0xED8 [O]; [P] ActiveDrawState returns its address; [U] cGraphicsMan_ForceActiveDrawState
    sTextureState* m_pCurrentTextureState; // 0xEEC [O]
    sTextureState m_textureStateStack[4]; // 0xEF0 [O]; [R] ctor fills -1
    sTextureState m_activeTextureState; // 0xF00 [O]
    float m_constantColours[2][4];  // 0xF04 [O] tARGB[2] (4-aligned in retail)
    float m_lastConstantColours[2][4]; // 0xF24 [O] tARGB[2]
    int mSkipNextPageFlip;          // 0xF44 [O] bool; [P] SkipNextPageFlip stores 1 (cGraphicsMan_SkipNextPageFlip); [X] forceDisplayFlip
    tMdfBlendedMatrix* m_blendedMatrixHistory; // 0xF48 [O]; [R] ctor zeroes; [U] cGraphicsMan_AddBlendedMatrix
    int m_blendedMatrixHistorySize; // 0xF4C [O]; [U] cGraphicsMan_AddBlendedMatrix
    int _EffectTexMap[86];          // 0xF50 [O]; [R] ctor fills -1 (MEM_fill 0x158); [P] getEffectTexMap(i)
    int mSpecTexID;                 // 0x10A8 [O]; [P] Get/SetSpecID (cGraphicsMan_GetSpecID/cGraphicsMan_SetSpecID)
    int mPatchSpecTexID;            // 0x10AC [O]; [P] Get/SetPatchSpecID (cGraphicsMan_GetPatchSpecID/cGraphicsMan_SetPatchSpecID)
    int mCharSpecTexID[10];         // 0x10B0 [O]; [U] indexed accessors func_003946A8/func_003946B8
    void** mVtbl;                   // 0x10D8 [R] cGraphicsMan cGraphicsMan_vtbl, cPS2GraphicsMan cPS2GraphicsMan_vtbl
};

CHECK_OFFSET(cGraphicsMan, m_SpecularTextureId, 0x00C);
CHECK_OFFSET(cGraphicsMan, m_SnowSparkleDensity, 0x0AC);
CHECK_OFFSET(cGraphicsMan, mNumViews, 0x270);
CHECK_OFFSET(cGraphicsMan, mTexRefs, 0x280);
CHECK_OFFSET(cGraphicsMan, m_drawStateStack, 0xE88);
CHECK_OFFSET(cGraphicsMan, _EffectTexMap, 0xF50);
CHECK_OFFSET(cGraphicsMan, mCharSpecTexID, 0x10B0);
CHECK_OFFSET(cGraphicsMan, mVtbl, 0x10D8);
CHECK_SIZE(cGraphicsMan, 0x10DC);

#endif

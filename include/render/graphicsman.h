#ifndef RENDER_GRAPHICSMAN_H
#define RENDER_GRAPHICSMAN_H
#pragma interface

// cGraphicsMan: the renderer's platform-independent base class (EA source/render/graphicsman.cpp / .h, preview
// asserts): per-view settings (specular / sparkle / rim textures, NIS flag, snow sparkle density, draw distance, fog,
// screen tint), the view count and index, the texture reference table, the draw-state and texture-state stacks, the
// constant colours, the blended-matrix history and the effect / specular texture maps. The one instance is the
// cPS2GraphicsMan (render/ps2graphicsman.h), so cGraphicsMan::TheGraphicsMan (cGraphicsMan_TheGraphicsMan) points at
// it; this struct is that object's first 0x10DC bytes. Methods: the accessors in render/font (0x394020-0x3946D8),
// render/graphicsman and render/ps2graphicsman.
//
// cPS2GraphicsMan (render/ps2graphicsman.h) derives from it. The member types (sDrawState, sTextureState, tTextureRef,
// tMdfBlendedMatrix) live here with it.
// On Tour's cGraphicsMan has the same member order (size 0x10D0 there; retail's vptr ends it at 0x10DC).

#include "layout.h"
#include "bx/memman.h"

// One weighted matrix of a blended (skinning) matrix (On Tour struct tMdfBlendedMatrixComponent, 0x4).
// [U] cPS2GraphicsMan_ResolveCharModel rebases m_iMatrixId by the part's bone offset (w[j*4+2] += tbl[w[j*4+3]]);
// cPS2GraphicsMan_GenerateBlendedMatrices reads the weight (lh +0) and the matrix id (lbu +2) (its view sBlendLink_6BD0);
// areBlendedMatricesEqual compares the weights (lh) and the two id bytes together (lhu +2, its helper componentKey).
struct tMdfBlendedMatrixComponent {
    short m_iWeight;                // 0x0 [O]; [U] GenerateBlendedMatrices, areBlendedMatricesEqual (tolerances)
    unsigned char m_iMatrixId;      // 0x2 [O]; [U] ResolveCharModel rebases it; GenerateBlendedMatrices indexes the
                                    //     matrix list with it
    unsigned char m_iPartId;        // 0x3 [O]; [U] ResolveCharModel: index into the part bone offsets
};

// A blended (skinning) matrix: up to three weighted matrices (On Tour struct tMdfBlendedMatrix, 0xC).
// [R] cGraphicsMan_AddBlendedMatrix allocates the 512-entry history (0x3000 bytes "BlendedMatrixHistory": 512 x 0xC
// records, then 512 x 3 components) and points each m_matrixList at its 3 components; [U] areBlendedMatricesEqual
// and AddBlendedMatrix (through this type), ResolveCharModel (sBlend_66F0), GenerateBlendedMatrices
// (sBlendEnt_6BD0).
struct tMdfBlendedMatrix {
    int m_iNumMatrices;             // 0x0 [O]; [U] the count compared / copied (1..3)
    tMdfBlendedMatrixComponent* m_matrixList; // 0x4 [O]; [R] AddBlendedMatrix; [U] every user walks it
    int m_iMatrixId;                // 0x8 [O]; [U] ResolveCharModel stores the record's index in the blended matrix
                                    //     list (sBlend_66F0::idx)
};

// [O] sTextureState: one texture id per GS context (On Tour's PSP build has 1); -1 = none.
struct sTextureState {
    // No texture (name ours): [R] the ctor fills m_texid with it; [U] InitVertex scales colours for an untextured draw.
    enum { kNoTexture = -1 };
    short m_texid[2];               // 0x0 [O]; [R] ctor fills -1; [X] texture ids for context 1/2
};

// [O][P] sDrawState, 0x14 bytes (the preview's SetDrawState copies 16 + 4 bytes). The first 16 bytes are On Tour's
// anonymous union of packed GS/VU1 state bits (decoded in [X] "RenderState"); the cGraphicsMan_Set* accessors
// mask them in place.
struct sDrawState {
    unsigned int m_bitGroups[4];    // 0x00 [O] anonymous union { int m_bitGroups[3]; m_NamedBitFields } (DWARF union
                                    //      @1ed6 in ssxd_dvd.prx: PSP has 3 words, retail 4: [P] SetDrawState copies
                                    //      16 + 4 bytes); [P] SetFixAlpha/SetVifCodeState/...; [U] the cGraphicsMan_Set*
                                    //      accessors and the particle / font / avalanche views mask words 0..2 in place.
                                    //      The PSP bit names (mCLAMP, mCULL, ...) are GE state and do not apply here.
                                    //      [Renamed from unk_00 by lstruct group L2.]
    sTextureState m_textureState;   // 0x10 [O]
};

// [O] tTextureRef: a texture loaded by its 4-character name (cGraphicsMan_GetTextureID looks up / caches them).
struct tTextureRef {
    int TextureID;                  // 0x0 [O]; [U] cGraphicsMan_GetTextureID returns it
    char Name[8];                   // 0x4 [O]; [U] strcpy target in cGraphicsMan_GetTextureID
};

CHECK_OFFSET(tMdfBlendedMatrixComponent, m_iWeight, 0x0);
CHECK_OFFSET(tMdfBlendedMatrixComponent, m_iMatrixId, 0x2);
CHECK_OFFSET(tMdfBlendedMatrixComponent, m_iPartId, 0x3);
CHECK_SIZE(tMdfBlendedMatrixComponent, 0x4);
CHECK_OFFSET(tMdfBlendedMatrix, m_iNumMatrices, 0x0);
CHECK_OFFSET(tMdfBlendedMatrix, m_matrixList, 0x4);
CHECK_OFFSET(tMdfBlendedMatrix, m_iMatrixId, 0x8);
CHECK_SIZE(tMdfBlendedMatrix, 0xC);
CHECK_OFFSET(sTextureState, m_texid, 0x0);
CHECK_SIZE(sTextureState, 0x4);
CHECK_OFFSET(sDrawState, m_bitGroups, 0x00);
CHECK_OFFSET(sDrawState, m_textureState, 0x10);
CHECK_SIZE(sDrawState, 0x14);
CHECK_OFFSET(tTextureRef, TextureID, 0x0);
CHECK_OFFSET(tTextureRef, Name, 0x4);
CHECK_SIZE(tTextureRef, 0xC);

struct SHAPE;
struct cDynamicColourEmitter;
struct cDynamicEmitter;
struct cFont;
struct tIrradianceInfo;
struct cGridMesh;
struct t2Vector;
struct t3Vector;
struct t4Matrix;
struct t4Vector;
struct tARGB;
struct tBoardWakeVertex;
struct tInstanceRecord;
struct tMaterialBlockRecord;
struct tMdfModelHeader;
struct tParticleEmitter;
struct tRGB;
struct tRect;
struct tResourceID;
struct tScreenGrabHandle;
struct tSnowBoxData;
struct tTVDisplayFormat;
struct BoardTrailInfoT;
struct tBxVertex;

// ClipBoundBox's result. [O] On Tour's enum GMClipCodes, names and values; [U] cPS2GraphicsMan_ClipBoundBox returns 1
// when the VU0 clip program's second flag is set, 2 for its first, else 0; cPS2GraphicsMan_CheckProjectiveTexturesHits
// marks every projective texture whose clip volume a box is not outside of.
enum GMClipCodes { GM_CLIPCODE_INSIDE = 0, GM_CLIPCODE_OUTSIDE = 1, GM_CLIPCODE_CROSSING = 2 };

// A texture's texel format (tTexInfo::mFormat). [O] On Tour's enum eGMTexelFormat, names and values; [U] retail fits:
// cTextureMan_AddTexture gives GM_TEXEL_RGB888 the GS PSMCT24 format, GM_TEXEL_ARGB1555 PSMCT16, the INDEX8 formats
// PSMT8, the INDEX4 formats PSMT4 and the rest PSMCT32, and keeps GM_TEXEL_INDEX8 textures in the second VRAM heap.
enum eGMTexelFormat {
    GM_TEXEL_ARGB8888 = 0, GM_TEXEL_ARGB0565 = 1, GM_TEXEL_ARGB1555 = 2, GM_TEXEL_ARGB4444 = 3, GM_TEXEL_ARGB8888S = 4,
    GM_TEXEL_ARGB0565S = 5, GM_TEXEL_ARGB1555S = 6, GM_TEXEL_ARGB4444S = 7, GM_TEXEL_RGB888 = 8, GM_TEXEL_INDEX8 = 9,
    GM_TEXEL_INDEX8S = 10, GM_TEXEL_INDEX4 = 11, GM_TEXEL_INDEX4S = 12, GM_TEXEL_RGB5A3 = 13, GM_TEXEL_R8 = 14,
    GM_TEXEL_INTENSITY8 = 15, GM_TEXEL_DXT1 = 16, GM_TEXEL_DXT3 = 17, GM_TEXEL_DXT5 = 18, GM_TEXEL_YUY2 = 19,
    GM_TEXEL_Z24 = 20, GM_NUM_TEXEL_FORMATS = 21
};

// How a texture holds its VRAM (the lockType of cTextureMan_AddTexture / cPS2GraphicsMan_AddTexture). [O] On Tour's
// enum eGMTexLockType (AddTexture(..., eGMTexLockType lockType, ...)), names and values; [U] retail fits:
// cTextureMan_AddTexture leaves a GM_TEXTYPE_DYNAMIC texture's VRAM to the bind (cTextureMan_ensureContextTextures), allocates it at once
// for GM_TEXTYPE_LOCKED and for GM_TEXTYPE_WINDOW (0x2000 bytes more, locked, 32-block aligned), which
// cTextureMan_AddLockedFrameTexture passes for its render-target texture.
enum eGMTexLockType {
    GM_TEXTYPE_DYNAMIC = 0, GM_TEXTYPE_LOCKED = 1, GM_TEXTYPE_LOCKED8H = 2, GM_TEXTYPE_WINDOW = 3, GM_TEXTYPE_VRAM = 4
};

class cGraphicsMan {
public:
    float m_screenOffset[2];        // 0x000 [O] t2Vector; [P] Get/SetScreenOffset (cGraphicsMan_SetScreenOffset/FD8/FF0)
    int m_AIUpdatesPerRender;       // 0x008 [O]; [R] ctor zeroes it
    int m_SpecularTextureId[8];     // 0x00C [O]; [P] get/setSpecularTextureId (preview: one int); [U] cVisualEffectsMan ctor
    int m_PatchSpecularTextureId[8]; // 0x02C [O]; [U] cVisualEffectsMan ctor stores [6]
    int m_SnowSparkleTextureId[8];  // 0x04C [O]; [U] ditto
    int m_RimTextureId[8];          // 0x06C [O]; [U] ditto
    int m_isInNIS[8];               // 0x08C [O]; [U] cVisualEffectsMan_commonUpdate stores [i]
    float m_SnowSparkleDensity[8];  // 0x0AC [O]; [U] cVisualEffectsMan_commonUpdate stores [i]; cPS2BezierMan_addSnowSparklePass reads [6]
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
    int mCharSpecTexID[10];         // 0x10B0 [O]; [U] indexed accessors cGraphicsMan_SetCharSpecID/cGraphicsMan_GetCharSpecID
    // [P] getEffectTexMap__12cGraphicsMani, inline (retail's out-of-line copy: cGraphicsMan_getEffectTexMap in
    // render/font);
    // [U] cStaticEmitter_render1 reads the effect texture through it (lw, then sh into the draw state)
    int getEffectTexMap(int i) { return _EffectTexMap[i]; }
    // 0x10D8: the vptr (ee-gcc 2.95.3 puts it after the own data of the class that declares the first virtual).
    // [R] cGraphicsMan_vtbl (pure slots: __pure_virtual) / cPS2GraphicsMan_vtbl (the PS2 overrides) give the slots and
    // the names; parameters: the matched implementations [R], or On Tour's cGraphicsMan / cPSPGraphicsMan [O] where its
    // ints / pointers and its floats line up with retail's (they travel in separate registers, so On Tour's order is
    // kept); `bool` parameters are spelled int (an int argument would be converted at the call). R? marks a parameter
    // whose retail type is a unit-local view (void* / int here).
    // slot 1: [R] cPS2GraphicsMan__cPS2GraphicsMan calls it; g++'s own symbol _$_12cGraphicsMan (the retail label
    // renamed)
    virtual ~cGraphicsMan();
    virtual void Init() = 0;                                                                             // slot 2: [R] cPS2GraphicsMan_Init
    virtual void Purge() = 0;                                                                            // slot 3: [R] cPS2GraphicsMan_Purge
    virtual bool SetDisplayMode1(int idx) = 0;                                                           // slot 4: [R] cPS2GraphicsMan_SetDisplayMode1
    virtual int GetDisplayMode1() = 0;                                                                   // slot 5: [R] cPS2GraphicsMan_GetDisplayMode1__FPv; parameters [O]
    virtual const char* GetDisplayModeName(int mode) = 0;                                                // slot 6: [R] cPS2GraphicsMan_GetDisplayModeName
    virtual int GetNumDisplayModes() = 0;                                                                // slot 7: [R] cPS2GraphicsMan_GetNumDisplayModes__FPv; parameters [O]
    virtual int SetDTV(int Enable);                                                                      // slot 8: [R] cPS2GraphicsMan_SetDTV
    // slot 9: [R] cPS2GraphicsMan_GetDTV__FPv; parameters [O]
    virtual bool GetDTV() __asm__("cGraphicsMan_GetDTV__FPv");
    virtual int GetScreenWidth() = 0;                                                                    // slot 10: [R] cPS2GraphicsMan_GetScreenWidth
    virtual int GetScreenHeight() = 0;                                                                   // slot 11: [R] cPS2GraphicsMan_GetScreenHeight
    // slot 12: [R] cGraphicsMan_SetScreenOffset
    virtual void SetScreenOffset(int xoffset, int yoffset) __asm__("cGraphicsMan_SetScreenOffset");
    // slot 13: [R] cGraphicsMan_GetScreenXOffset
    virtual int GetScreenXOffset() __asm__("cGraphicsMan_GetScreenXOffset");
    // slot 14: [R] cGraphicsMan_GetScreenYOffset
    virtual int GetScreenYOffset() __asm__("cGraphicsMan_GetScreenYOffset");
    virtual void SetScreenClearColour(const tRGB& col) = 0;                                              // slot 15: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_SetScreenClearColour
    // slot 16: [R] cGraphicsMan_SetAIUpdatesPerRender__FPv; parameters [O]
    virtual void SetAIUpdatesPerRender(int aiupdatesperrender) __asm__("cGraphicsMan_SetAIUpdatesPerRender__FPv");
    // slot 17: [R] cPS2GraphicsMan_IsRenderReady__FPv; parameters [O]
    virtual bool IsRenderReady() __asm__("cGraphicsMan_IsRenderReady__FPv");
    // slot 18: [R] cPS2GraphicsMan_IsRenderIdle__FPv; parameters [O]
    virtual bool IsRenderIdle() __asm__("cGraphicsMan_IsRenderIdle__FPv");
    virtual void FlushRenderer() __asm__("cGraphicsMan_FlushRenderer"); // slot 19: [R] cGraphicsMan_FlushRenderer
    virtual void DrawFrame() = 0;                                                                        // slot 20: [R] cPS2GraphicsMan_DrawFrame
    virtual void PushViewport() = 0;                                                                     // slot 21: [R] cPS2GraphicsMan_PushViewport
    virtual void PopViewport() = 0;                                                                      // slot 22: [R] cPS2GraphicsMan_PopViewport
    virtual void SetViewportWindow(float x, float y, float w, float h, int applyScreenOffset) = 0;       // slot 23: [R] cPS2GraphicsMan_SetViewportWindow; its caller cPS2GraphicsMan_SetViewportWindow1 passes (applyScreenOffset, rect's four floats); [O] SetViewportWindow(float x, float y, float w, float h, bool applyScreenOffset)
    virtual void SetViewportWindow1(const tRect& rect, int applyScreenOffset) = 0;                       // slot 24: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_SetViewportWindow1
    virtual void SetViewportPerspective(float fov, float minz, float maxz) = 0;                          // slot 25: [R] cPS2GraphicsMan_SetViewportPerspective
    virtual void SetViewportOrthographic(float x, float y, float w, float h, float minz, float maxz, int adjustforsafezone, int scaleForFE) = 0; // slot 26: [R] cPS2GraphicsMan_SetViewportOrthographic (reads the six floats only); every retail caller passes six floats and two ints ((0, 0, 640, 480, 0, 1) for the full screen); [O] On Tour's signature (the two bools)
    virtual void SetViewportZBias(float worldBias, const t4Vector& wpos) = 0;                            // slot 27: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_SetViewportZBias
    virtual tRect GetViewportWindow() = 0;                                                               // slot 28: [R] cPS2GraphicsMan_GetViewportWindow; returns tRect by value (retail passes a result pointer first)
    virtual float GetViewportFOV() = 0;                                                                  // slot 29: [R] cPS2GraphicsMan_GetViewportFOV
    virtual void GetViewportPerspective(float& fov, float& minz, float& maxz) = 0;                       // slot 30: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_GetViewportPerspective
    virtual void PushView() = 0;                                                                         // slot 31: [R] cPS2GraphicsMan_PushView
    virtual void PopView() = 0;                                                                          // slot 32: [R] cPS2GraphicsMan_PopView__FPv; parameters [O]
    virtual void SetView(float pitch, float heading, float roll, const t4Vector& pos) = 0;               // slot 33: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_SetView
    virtual void SetView1(const t4Matrix& m) = 0;                                                        // slot 34: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_SetView1
    virtual const t4Matrix* GetView() = 0;                                                               // slot 35: [R] cPS2GraphicsMan_GetView__FPv; parameters [O]
    virtual void AppendView(const t4Matrix& m) = 0;                                                      // slot 36: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_AppendView
    virtual void IdentityView() = 0;                                                                     // slot 37: [R] cPS2GraphicsMan_IdentityView
    virtual void TranslateView(const t4Vector& pos) = 0;                                                 // slot 38: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_TranslateView
    virtual void RotateView(float deg, const t4Vector& axis) = 0;                                        // slot 39: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_RotateView
    virtual void SetTVDisplayFormat(int df) = 0;                                                         // slot 40: [R] cPS2GraphicsMan_SetTVDisplayFormat
    virtual void SetTVDisplayFormat1(const tTVDisplayFormat& tvdf) = 0;                                  // slot 41: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_SetTVDisplayFormat1
    virtual const tTVDisplayFormat* GetTVDisplayFormat() = 0;                                            // slot 42: [R] cPS2GraphicsMan_GetTVDisplayFormat__FPv; parameters [O]
    virtual t4Matrix GetNetMatrix() = 0;                                                                 // slot 43: [R] cPS2GraphicsMan_GetNetMatrix; returns t4Matrix by value (retail passes a result pointer first)
    virtual void InitTextures(int memtype) = 0;                                                          // slot 44: [R] cPS2GraphicsMan_InitTextures
    virtual void SetTextureMemType() = 0;                                                                // slot 45: [R] cPS2GraphicsMan_SetTextureMemType
    virtual int AddTexture(const SHAPE* pShp, const char* sName, int lockType, int bCopyMem, int iBindId) = 0; // slot 46: [R] cPS2GraphicsMan_AddTexture
    virtual int AddTexture1(const char* pData, const char* sName, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiMipmap, int texelType, int clutType, const char* pClut, int lockType, int bCopyMem, int iBindId) = 0; // slot 47: [R] cPS2GraphicsMan_AddTexture1
    virtual void SetNewTextureData(int iTexId, const SHAPE* pShp, int bCopyMem) = 0;                     // slot 48: [R] cPS2GraphicsMan_SetNewTextureData
    virtual void SetNewTextureData1(int iTexId, const char* pData, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiMipmap, int texelType, int clutType, const char* pClut, int bCopyMem) = 0; // slot 49: [R] cPS2GraphicsMan_SetNewTextureData1
    virtual void RemoveTexture(int texid) = 0;                                                           // slot 50: [R] cPS2GraphicsMan_RemoveTexture
    virtual bool IsValidTexture(int texid) = 0;                                                          // slot 51: [R] cPS2GraphicsMan_IsValidTexture
    virtual int AddLockedFrameTexture(int width, int height, int texeltype) = 0;                         // slot 52: [R] cPS2GraphicsMan_AddLockedFrameTexture__FPv; parameters [O]
    virtual bool LockFrameToTexture(int texid, int bSafeZone) = 0;                                       // slot 53: [R] cPS2GraphicsMan_LockFrameToTexture (reads texid only); the retail caller passes (texture, 1); [O] On Tour's signature
    virtual bool RestoreFrameLock() = 0;                                                                 // slot 54: [R] cPS2GraphicsMan_RestoreFrameLock
    virtual void ShadowClearLockTexture(int texid, const tARGB& clearCol) = 0;                           // slot 55: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_ShadowClearLockTexture
    virtual int GetTextureWidth(int texid) = 0;                                                          // slot 56: [R] cPS2GraphicsMan_GetTextureWidth
    virtual int GetTextureHeight(int texid) = 0;                                                         // slot 57: [R] cPS2GraphicsMan_GetTextureHeight
    virtual int GetTexel(unsigned int textureId, float u, float v, int filter);                          // slot 58: [R] cPS2GraphicsMan_GetTexel; O/O/O/O?
    virtual void CreateScreenGrabTexture(tScreenGrabHandle& screenGrabHandle, unsigned int width, unsigned int height)
        __asm__("cGraphicsMan_CreateScreenGrabTexture__FPvT0"); // slot 59: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_CreateScreenGrabTexture
    // slot 60: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_RemoveScreenGrabTexture
    virtual void RemoveScreenGrabTexture(tScreenGrabHandle& screenGrabHandle)
        __asm__("cGraphicsMan_RemoveScreenGrabTexture__FPv");
    // slot 61: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_TakeScreenGrab
    virtual void TakeScreenGrab(const tScreenGrabHandle& screenGrabHandle) __asm__("cGraphicsMan_TakeScreenGrab__FPv");
    // slot 62: [O] reference parameters as On Tour; [R] cGraphicsMan_SetActiveDrawState__FPv; parameters [O]
    virtual void SetActiveDrawState(const sDrawState& drawstate) __asm__("cGraphicsMan_SetActiveDrawState__FPv");
    virtual void ForceActiveDrawState(const sDrawState& drawstate)
        __asm__("cGraphicsMan_ForceActiveDrawState");                                      // slot 63: [O] reference parameters as On Tour; [R] cGraphicsMan_ForceActiveDrawState
    // slot 64: [R] cGraphicsMan_ActiveDrawState__FPv; parameters [O]
    virtual const sDrawState* ActiveDrawState() __asm__("cGraphicsMan_ActiveDrawState__FPv");
    // slot 65: [R] cGraphicsMan_SetActiveTexture__FPv; parameters [O]
    virtual void SetActiveTexture(int texid, unsigned int contextID) __asm__("cGraphicsMan_SetActiveTexture__FPv");
    // slot 66: [R] cPS2GraphicsMan_ClearProjTextures__FPv; parameters [O]
    virtual void ClearProjTextures() __asm__("cGraphicsMan_ClearProjTextures__FPv");
    virtual void AddProjectiveTexture(const t4Vector& pos, const t4Matrix& clipMat, const t4Matrix& projMat,
            // slot 67: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_AddProjectiveTexture
            float intensity, int texid) __asm__("cGraphicsMan_AddProjectiveTexture__FPv");
    virtual void SetLightMats(void* lmats) = 0;                                                          // slot 68: [R] cPS2GraphicsMan_SetLightMats__FPv; parameters [O]
    virtual void SetIrradiance(const tIrradianceInfo& irradiance, int ignored) = 0;                     // slot 69: [R] cPS2GraphicsMan_SetIrradiance (copies the 0xA0 block); [P] SetIrradiance__15cPS2GraphicsManRC15tIrradianceInfob; [O] the same, parameters named irradiance, ignored; retail callers pass (block, 1)
    virtual void* GetIrradiance() = 0;                                                                   // slot 70: [R] cPS2GraphicsMan_GetIrradiance__FPv; parameters [O]
    virtual void RenderTriStrip(int numVerts, tBxVertex* src, int isstatic) = 0;                         // slot 71: [R] cPS2GraphicsMan_RenderTriStrip; isstatic: [R] every caller passes it ($a3), [O] the PSP twin's bool isstatic
    virtual void RenderIndexedTriList(int NumTris, int NumVerts, const unsigned char* Indices, const t3Vector* P,
            // slot 72: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_RenderIndexedTriList
            const t2Vector* UV, const tARGB* Color, const t3Vector& Offset)
        __asm__("cGraphicsMan_RenderIndexedTriList");
    virtual void RenderLineStrip(int numverts, const tBxVertex* verts, int isstatic) = 0;                // slot 73: [R] cPS2GraphicsMan_RenderLineStrip; every caller passes (n, tBxVertex*, 0); [O] the PSP twin's RenderLineStrip(int, const tBxVertex*, bool isstatic), and On Tour lists this pure one first
    // slot 74: [R] the base's empty body (cPS2GraphicsMan keeps it); [O] On Tour's cGraphicsMan defines
    // RenderLineStrip(int numverts, char* packedoffsets, float scale) itself
    virtual void RenderLineStrip(int numverts, char* packedoffsets, float scale)
        __asm__("cGraphicsMan_RenderLineStrip__FPv");
    virtual void RenderSpline(const t4Matrix& BezMat, const tARGB& col, int NumPoints)
        __asm__("cGraphicsMan_RenderSpline");                  // slot 75: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_RenderSpline
    virtual cGridMesh* CreateGridMesh(int width, int height, int memtype) = 0;                           // slot 76: [R] cPS2GraphicsMan_CreateGridMesh
    virtual void DeleteGridMesh(cGridMesh* pData)
        __asm__("cGraphicsMan_DeleteGridMesh__FPvi");                                                       // slot 77: [R] cGraphicsMan_DeleteGridMesh__FPvi; parameters [O]
    virtual void DrawSprite(const t4Vector& pos, const t2Vector& halfSize, const t2Vector& uv0, const t2Vector& uv1, const tARGB& colour) = 0; // slot 78: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_DrawSprite
    virtual void DrawRotatedSprite(const t4Vector& pos, const t2Vector& halfSize, const t2Vector& uv0, const t2Vector& uv1, const tARGB& colour, float rotationRads, int numPass) = 0; // slot 79: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_DrawRotatedSprite
    virtual void DrawString3D(cFont& font, float x, float y, const char* str, float xscale, float yscale, float iwidth, float iheight, const tARGB& colour, float depth) = 0; // slot 80: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_DrawString3D
    virtual void DrawString3D1(cFont& font, float x, float y, const unsigned short* str, float xscale, float yscale, float iwidth, float iheight, const tARGB& colour, float depth) = 0; // slot 81: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_DrawString3D1
    virtual void RenderParticleEmitter(const tParticleEmitter& emitter) = 0;                             // slot 82: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_RenderParticleEmitter
    virtual void RenderDynamicParticleEmitter(const cDynamicEmitter* emitter) = 0;                       // slot 83: [R] cPS2GraphicsMan_RenderDynamicParticleEmitter
    virtual void RenderDynamicParticleEmitter1(const cDynamicColourEmitter* emitter) = 0;                // slot 84: [R] cPS2GraphicsMan_RenderDynamicParticleEmitter1
    virtual void RenderSnowBoxFlakes(const tSnowBoxData& snowBoxData) = 0;                               // slot 85: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_RenderSnowBoxFlakes
    virtual void RenderSnowBoxFluffs(const tSnowBoxData& snowBoxData) = 0;                               // slot 86: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_RenderSnowBoxFluffs
    virtual void DrawSphere(t4Vector* pos, float* col, float radius, int segs) = 0;                      // slot 87: [R] cPS2GraphicsMan_DrawSphere
    virtual void RenderBoardTrail(const t3Vector& L2WOffset, void* verts, unsigned short* topindices,
            int numtopindices, unsigned short* troughindices, int numtroughindices, unsigned short* sliceindices,
            // slot 88: [O] reference parameters as On Tour; [R] cGraphicsMan_RenderBoardTrail__FPv; parameters [O]
            int numsliceindices) __asm__("cGraphicsMan_RenderBoardTrail__FPv");
    virtual void RenderMultipleBoardTrails(BoardTrailInfoT* pTrails, int nTrails);                       // slot 89: [R] cPS2GraphicsMan_RenderMultipleBoardTrails
    virtual void RenderBoardWake(tBoardWakeVertex** verts, int numSegmentsWide, int startIndex, int numIndices,
            int stripLength, float alphaMultiplier, float alphaDelta)
        __asm__("cGraphicsMan_RenderBoardWake__FPv"); // slot 90: [R] cPS2GraphicsMan_RenderBoardWake
    virtual int GetOutCode(const t4Vector& point) = 0;                                                   // slot 91: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_GetOutCode
    virtual t4Vector GetScreenCoord(const t4Vector& point) = 0;                                          // slot 92: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_GetScreenCoord; returns t4Vector by value
    virtual int ClipBoundBox(const t4Vector& iMinp, const t4Vector& iMaxp, const t4Matrix& iMat) = 0;    // slot 93: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_ClipBoundBox
    // slot 94: [O] reference parameters as On Tour; [R] cGraphicsMan_DeformMeshMatrix
    virtual void DeformMeshMatrix(t4Matrix& mat) __asm__("cGraphicsMan_DeformMeshMatrix");
    virtual void RenderModel(const tInstanceRecord* pInstance, const tMaterialBlockRecord* material, const t4Matrix& BMat, float opacity, const t4Matrix* animmats, const t4Matrix* TexMat, int flags) = 0; // slot 95: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_RenderModel
    virtual void RenderModel(tInstanceRecord* obj, int animMats, int texMat, int renderFlags)
        __asm__("cGraphicsMan_RenderModel");                                                             // slot 96
    virtual void GetMeshQuad(const tInstanceRecord* pInstance, int mesh, t3Vector* vert, t2Vector* uv, tARGB* rgb) = 0; // slot 97: [R] cPS2GraphicsMan_GetMeshQuad
    // slot 98: [R] cPS2GraphicsMan_SetShinyBoarder__FPvi; parameters [O]
    virtual void SetShinyBoarder(int State) __asm__("cGraphicsMan_SetShinyBoarder__FPv");
    virtual void DrawCharModel(const tMdfModelHeader* mdfheader, const char* data, float scale, int additive, int cheatcharid) = 0; // slot 99: [R] cPS2GraphicsMan_DrawCharModel (header, data, the additive flag); cAnimModel_render passes (part, data, additiveBoard / additivePlayer, 0, scale) after GenerateBlendedMatrices (slot 111); [O] On Tour's signature, which also takes the matrices
    // slot 100: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_DrawCharShadowModel
    virtual void DrawCharShadowModel(const tMdfModelHeader* model, const char* data, const tARGB& shdwcol)
        __asm__("cGraphicsMan_DrawCharShadowModel__FPv");
    virtual void SetShadowCharMatrices(const t4Matrix& sproj, const t4Vector& sscale, const t4Vector& strans) = 0; // slot 101: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_SetShadowCharMatrices
    // slot 102: [R] cGraphicsMan_SectionLoaded__FPv; parameters [O]
    virtual void SectionLoaded(int sectionId) __asm__("cGraphicsMan_SectionLoaded__FPv");
    // slot 103: [R] cGraphicsMan_InitGlobalBuffer__FPv; parameters [O]
    virtual void InitGlobalBuffer(void* pGlobalBuffer, const tResourceID* rid)
        __asm__("cGraphicsMan_InitGlobalBuffer__FPv");
    // slot 104: [R] cGraphicsMan_PurgeGlobalBuffer__FPv; parameters [O]
    virtual void PurgeGlobalBuffer(const tResourceID* rid) __asm__("cGraphicsMan_PurgeGlobalBuffer__FPv");
    // slot 105: [R] cGraphicsMan_InitGlobalRGBBuffer__FPv; parameters [O]
    virtual void InitGlobalRGBBuffer(void* pGlobalBuffer, const tResourceID* rid)
        __asm__("cGraphicsMan_InitGlobalRGBBuffer__FPv");
    // slot 106: [R] cGraphicsMan_PurgeGlobalRGBBuffer__FPv; parameters [O]
    virtual void PurgeGlobalRGBBuffer(const tResourceID* rid) __asm__("cGraphicsMan_PurgeGlobalRGBBuffer__FPv");
    // slot 107: [R] cGraphicsMan_PurgeMeshData__FPv; parameters [O]
    virtual void PurgeMeshData() __asm__("cGraphicsMan_PurgeMeshData__FPv");
    // slot 108: [R] cGraphicsMan_RemapModelMaterialId__FPv; parameters [O]
    virtual void RemapModelMaterialId(tMdfModelHeader* model, char* modeldata, char* name, int id)
        __asm__("cGraphicsMan_RemapModelMaterialId__FPv");
    virtual void ResolveCharModel(tMdfModelHeader* inModel, char* modeldata, int* partBoneOffset, int& numBlendedMats,
            // slot 109: [O] reference parameters as On Tour; [R] cPS2GraphicsMan_ResolveCharModel
            tMdfBlendedMatrix* blendedMats, float* morphWeights) __asm__("cGraphicsMan_ResolveCharModel__FPv");
    virtual void InitCharModelPart(tMdfModelHeader* mdfheader, int nummats, tMdfBlendedMatrix* blendmats,
            // slot 110: [R] cPS2GraphicsMan_InitCharModelPart__FPv; parameters [O]
            const t4Matrix* mats) __asm__("cGraphicsMan_InitCharModelPart__FPv");
    virtual void GenerateBlendedMatrices(const t4Matrix* matlist, int* partBoneOffsets,
            const tMdfBlendedMatrix* blendedMatrixTranslationTable, int numMatrices, t4Matrix* blendedmatlist)
        __asm__("cGraphicsMan_GenerateBlendedMatrices__FPv"); // slot 111: [R] cPS2GraphicsMan_GenerateBlendedMatrices
    // slot 112: [R] cGraphicsMan_PurgeCharModel__FPv; parameters [O]
    virtual void PurgeCharModel(tMdfModelHeader* model) __asm__("cGraphicsMan_PurgeCharModel__FPv");
    // slot 113: [R] cGraphicsMan_PurgeCharModels__FPv; parameters [O]
    virtual void PurgeCharModels() __asm__("cGraphicsMan_PurgeCharModels__FPv");
    // slot 114: [R] cPS2GraphicsMan_getCurrentFrameCount__FPv; parameters [O]
    virtual int getCurrentFrameCount() __asm__("cGraphicsMan_getCurrentFrameCount__FPv");
    // slot 115: [R] cPS2GraphicsMan_UpdateFullScreenEffects
    virtual void UpdateFullScreenEffects(int View) __asm__("cGraphicsMan_UpdateFullScreenEffects__FPv");
    // slot 116: [R] cPS2GraphicsMan_RenderFullScreenEffects
    virtual void RenderFullScreenEffects(int View) __asm__("cGraphicsMan_RenderFullScreenEffects__FPv");
    virtual void SetLensFXCallBack(void* callback, void* param) = 0;                                     // slot 117: [R] cPS2GraphicsMan_SetLensFXCallBack__FPvii; parameters [O]
    virtual void update() = 0;                                                                           // slot 118: [R] cPS2GraphicsMan_update
    // slot 119: [R] cPS2GraphicsMan_loadexecpurge
    virtual void loadexecpurge() __asm__("cGraphicsMan_loadexecpurge__FPv");
    virtual void PlatformPurgeTextures() = 0;                                                            // slot 120: [R] cPS2GraphicsMan_PlatformPurgeTextures
    virtual void PlatformUploadTextures() = 0;                                                           // slot 121: [R] cPS2GraphicsMan_PlatformUploadTextures__FPv; parameters [O]

    // Methods bound to their retail symbols by asm labels.
    void SetNumViews(int numViews) __asm__("cGraphicsMan_SetNumViews");
    int GetNumViews() __asm__("cGraphicsMan_GetNumViews");
    void SetViewIndex(int viewIndex) __asm__("cGraphicsMan_SetViewIndex");
    int GetViewIndex() __asm__("cGraphicsMan_GetViewIndex");
    void setSpecularTextureId(int value, int idx) __asm__("cGraphicsMan_setSpecularTextureId");
    int getSpecularTextureId(int idx) __asm__("cGraphicsMan_getSpecularTextureId");
    void setPatchSpecularTextureId(int value, int idx) __asm__("cGraphicsMan_setPatchSpecularTextureId");
    int getPatchSpecularTextureId(int idx) __asm__("cGraphicsMan_getPatchSpecularTextureId");
    void setSnowSparkleTextureId(int value, int idx) __asm__("cGraphicsMan_setSnowSparkleTextureId");
    int getSnowSparkleTextureId(int idx) __asm__("cGraphicsMan_getSnowSparkleTextureId");
    void setRimTextureId(int value, int idx) __asm__("cGraphicsMan_setRimTextureId");
    int getRimTextureId(int idx) __asm__("cGraphicsMan_getRimTextureId");
    void setisInNIS(int value, int idx) __asm__("cGraphicsMan_setisInNIS");
    int getisInNIS(int idx) __asm__("cGraphicsMan_getisInNIS");
    void setSnowSparkleDensity(int idx, float value) __asm__("cGraphicsMan_setSnowSparkleDensity");
    float getSnowSparkleDensity(int idx) __asm__("cGraphicsMan_getSnowSparkleDensity");
    void setDrawDistance(int idx, float value) __asm__("cGraphicsMan_setDrawDistance");
    float getDrawDistance(int idx) __asm__("cGraphicsMan_getDrawDistance");
    void setFogDensity(int idx, float value) __asm__("cGraphicsMan_setFogDensity");
    float getFogDensity(int idx) __asm__("cGraphicsMan_getFogDensity");
    void setFogColourR(int idx, float value) __asm__("cGraphicsMan_setFogColourR");
    float getFogColourR(int idx) __asm__("cGraphicsMan_getFogColourR");
    void setFogColourG(int idx, float value) __asm__("cGraphicsMan_setFogColourG");
    float getFogColourG(int idx) __asm__("cGraphicsMan_getFogColourG");
    void setFogColourB(int idx, float value) __asm__("cGraphicsMan_setFogColourB");
    float getFogColourB(int idx) __asm__("cGraphicsMan_getFogColourB");
    void setFogNear(int idx, float value) __asm__("cGraphicsMan_setFogNear");
    float getFogNear(int idx) __asm__("cGraphicsMan_getFogNear");
    void setFogFar(int idx, float value) __asm__("cGraphicsMan_setFogFar");
    float getFogFar(int idx) __asm__("cGraphicsMan_getFogFar");
    void setScreenTintScaleR(int idx, float value) __asm__("cGraphicsMan_setScreenTintScaleR");
    void setScreenTintScaleG(int idx, float value) __asm__("cGraphicsMan_setScreenTintScaleG");
    void setScreenTintScaleB(int idx, float value) __asm__("cGraphicsMan_setScreenTintScaleB");
    void setScreenTintOffsetR(int idx, float value) __asm__("cGraphicsMan_setScreenTintOffsetR");
    void setScreenTintOffsetG(int idx, float value) __asm__("cGraphicsMan_setScreenTintOffsetG");
    void setScreenTintOffsetB(int idx, float value) __asm__("cGraphicsMan_setScreenTintOffsetB");
    void SetDefaultDrawState() __asm__("cGraphicsMan_SetDefaultDrawState");
    void SetDrawState(sDrawState* src) __asm__("cGraphicsMan_SetDrawState");
    sDrawState* CurrentDrawState() __asm__("cGraphicsMan_CurrentDrawState");
    sDrawState* CurrentDrawState1() __asm__("cGraphicsMan_CurrentDrawState1");
    void SetDepthMask(int enable) __asm__("cGraphicsMan_SetDepthMask");
    void SetDrawLayer(int value) __asm__("cGraphicsMan_SetDrawLayer");
    void SetBlendMode(int mode, int context) __asm__("cGraphicsMan_SetBlendMode");
    void SetAlphaSort(int alphaSort, int sortKey) __asm__("cGraphicsMan_SetAlphaSort");
    void SetTextureClamp(int mode, int context) __asm__("cGraphicsMan_SetTextureClamp");
    void SetDmaDataPtr(int value) __asm__("cGraphicsMan_SetDmaDataPtr");
    unsigned int GetDrawLayer() __asm__("cGraphicsMan_GetDrawLayer");
    unsigned int GetBlendMode() __asm__("cGraphicsMan_GetBlendMode");
    int GetAlphaSort() __asm__("cGraphicsMan_GetAlphaSort");
    unsigned int GetAlphaSortKey() __asm__("cGraphicsMan_GetAlphaSortKey");
    int GetNumTexRefs() __asm__("cGraphicsMan_GetNumTexRefs");
    char* GetTexRef(int idx) __asm__("cGraphicsMan_GetTexRef");
    void SetSpecID(int texId) __asm__("cGraphicsMan_SetSpecID");
    int GetSpecID() __asm__("cGraphicsMan_GetSpecID");
    void SetPatchSpecID(int texId) __asm__("cGraphicsMan_SetPatchSpecID");
    int GetPatchSpecID() __asm__("cGraphicsMan_GetPatchSpecID");
    void SkipNextPageFlip() __asm__("cGraphicsMan_SkipNextPageFlip");
    void SetPixelTest(int depthtestmode, int atestmode, int aref) __asm__("cGraphicsMan_SetPixelTest");
    void SetTexture(short value, int idx) __asm__("cGraphicsMan_SetTexture");
    void PushDrawState() __asm__("cGraphicsMan_PushDrawState");
    void PopDrawState() __asm__("cGraphicsMan_PopDrawState");
    bool DrawStateChanged() __asm__("cGraphicsMan_DrawStateChanged");
    void setEffectTexMap(int idx, int value) __asm__("cGraphicsMan_setEffectTexMap");
    void AddBlendedMatrix(tMdfBlendedMatrix* blendedMatrix) __asm__("cGraphicsMan_AddBlendedMatrix");
    void UploadTextures() __asm__("cGraphicsMan_UploadTextures");
    void PurgeTextures() __asm__("cGraphicsMan_PurgeTextures");
    void SetTextureRefs(int count, tTextureRef* src) __asm__("cGraphicsMan_SetTextureRefs");
    int GetTextureID(const char* name) __asm__("cGraphicsMan_GetTextureID");
    void InitTextureFiles(int numFiles, int fileNames) __asm__("cGraphicsMan_InitTextureFiles__FPvii");
    void ResetBlendedMatrixHistory() __asm__("cGraphicsMan_ResetBlendedMatrixHistory");
    void RenderBoardTrail1() __asm__("cGraphicsMan_RenderBoardTrail1__FPv");
};

CHECK_OFFSET(cGraphicsMan, m_SpecularTextureId, 0x00C);
CHECK_OFFSET(cGraphicsMan, m_SnowSparkleDensity, 0x0AC);
CHECK_OFFSET(cGraphicsMan, mNumViews, 0x270);
CHECK_OFFSET(cGraphicsMan, mTexRefs, 0x280);
CHECK_OFFSET(cGraphicsMan, m_drawStateStack, 0xE88);
CHECK_OFFSET(cGraphicsMan, _EffectTexMap, 0xF50);
CHECK_OFFSET(cGraphicsMan, mCharSpecTexID, 0x10B0);
CHECK_SIZE(cGraphicsMan, 0x10DC);
__asm__("_vt$12cGraphicsMan = cGraphicsMan_vtbl");   // g++'s name for the retail vtable

// One board trail handed to cGraphicsMan::RenderMultipleBoardTrails (On Tour struct BoardTrailInfoT, 0x14;
// [O] RenderMultipleBoardTrails(BoardTrailInfoT* pTrails, int nTrails), retail vtable slot 89,
// cPS2GraphicsMan_RenderMultipleBoardTrails). [R] cPS2GraphicsMan_RenderMultipleBoardTrails reads the array at a 0x14
// stride (its view sPartStrip_6D10) and cBoardTrailFX's renderer (renderBoardTrails, view sTrailStripA538) fills one per
// trail at these offsets.
struct BoardTrailInfoT {
    tBxVertex** verts;              // 0x00 [O]; [R] the trail's vertex strips (cBoardTrailFX::BufVerts)
    int startIndex;                 // 0x04 [O]; [R] the first slice
    int stripLength;                // 0x08 [O]; [R] the slice count
    float alphaMultiplier;          // 0x0C [O]
    float alphaDelta;               // 0x10 [O]
};

CHECK_OFFSET(BoardTrailInfoT, verts, 0x00);
CHECK_OFFSET(BoardTrailInfoT, startIndex, 0x04);
CHECK_OFFSET(BoardTrailInfoT, stripLength, 0x08);
CHECK_OFFSET(BoardTrailInfoT, alphaMultiplier, 0x0C);
CHECK_OFFSET(BoardTrailInfoT, alphaDelta, 0x10);
CHECK_SIZE(BoardTrailInfoT, 0x14);

#endif

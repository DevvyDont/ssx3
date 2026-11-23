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
// cPS2GraphicsMan (render/ps2graphicsman.h) derives from it. The member types (sDrawState, sTextureState, tTextureRef,
// tMdfBlendedMatrix) live here with it.
// On Tour's cGraphicsMan has the same member order (size 0x10D0 there; retail's vptr ends it at 0x10DC).

#include "layout.h"

// One weighted matrix of a blended (skinning) matrix (On Tour struct tMdfBlendedMatrixComponent, 0x4).
// [U] cPS2GraphicsMan_ResolveCharModel rebases m_iMatrixId by the part's bone offset (w[j*4+2] += tbl[w[j*4+3]]);
// cPS2GraphicsMan_GenerateBlendedMatrices reads the weight (lh +0) and the matrix id (lbu +2) (its view sBlendLink_6BD0);
// areBlendedMatricesEqual compares the weights (lh) and the two id bytes together (lhu +2, its view sKey_9A78).
struct tMdfBlendedMatrixComponent {
    short m_iWeight;                // 0x0 [O]; [U] GenerateBlendedMatrices, areBlendedMatricesEqual (tolerances)
    unsigned char m_iMatrixId;      // 0x2 [O]; [U] ResolveCharModel rebases it; GenerateBlendedMatrices indexes the
                                    //     matrix list with it
    unsigned char m_iPartId;        // 0x3 [O]; [U] ResolveCharModel: index into the part bone offsets
};

// A blended (skinning) matrix: up to three weighted matrices (On Tour struct tMdfBlendedMatrix, 0xC).
// [R] cGraphicsMan_AddBlendedMatrix allocates the 512-entry history (0x3000 bytes "BlendedMatrixHistory": 512 x 0xC
// records, then 512 x 3 components) and points each m_matrixList at its 3 components; [U] areBlendedMatricesEqual
// (its view sEnt_9A78), AddBlendedMatrix (sBlendMatrix), ResolveCharModel (sBlend_66F0), GenerateBlendedMatrices
// (sBlendEnt_6BD0).
struct tMdfBlendedMatrix {
    int m_iNumMatrices;             // 0x0 [O]; [U] the count compared / copied (1..3)
    tMdfBlendedMatrixComponent* m_matrixList; // 0x4 [O]; [R] AddBlendedMatrix; [U] every user walks it
    int m_iMatrixId;                // 0x8 [O]; [U] ResolveCharModel stores the record's index in the blended matrix
                                    //     list (sBlend_66F0::idx)
};

// [O] sTextureState: one texture id per GS context (On Tour's PSP build has 1); -1 = none.
struct sTextureState {
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

class cGraphicsMan {
public:
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
    int mCharSpecTexID[10];         // 0x10B0 [O]; [U] indexed accessors cGraphicsMan_SetCharSpecID/cGraphicsMan_GetCharSpecID
    // 0x10D8: the vptr (ee-gcc 2.95.3 puts it after the own data of the class that declares the first virtual).
    // [R] cGraphicsMan_vtbl (pure slots: __pure_virtual) / cPS2GraphicsMan_vtbl (the PS2 overrides) give the slots and
    // the names; parameters: the matched implementations [R], or On Tour's cGraphicsMan / cPSPGraphicsMan [O] where its
    // ints / pointers and its floats line up with retail's (they travel in separate registers, so On Tour's order is
    // kept); `bool` parameters are spelled int (an int argument would be converted at the call). R? marks a parameter
    // whose retail type is a unit-local view (void* / int here).
    virtual ~cGraphicsMan();                                                                             // slot 1: [R] cGraphicsMan__cGraphicsMan / cPS2GraphicsMan__cPS2GraphicsMan
    virtual void Init() = 0;                                                                             // slot 2: [R] cPS2GraphicsMan_Init
    virtual void Purge() = 0;                                                                            // slot 3: [R] cPS2GraphicsMan_Purge
    virtual bool SetDisplayMode1(int idx) = 0;                                                           // slot 4: [R] cPS2GraphicsMan_SetDisplayMode1
    virtual int GetDisplayMode1() = 0;                                                                   // slot 5: [R] cPS2GraphicsMan_GetDisplayMode1__FPv; parameters [O]
    virtual const char* GetDisplayModeName(int mode) = 0;                                                // slot 6: [R] cPS2GraphicsMan_GetDisplayModeName
    virtual int GetNumDisplayModes() = 0;                                                                // slot 7: [R] cPS2GraphicsMan_GetNumDisplayModes__FPv; parameters [O]
    virtual int SetDTV(int Enable);                                                                      // slot 8: [R] cPS2GraphicsMan_SetDTV
    virtual bool GetDTV();                                                                               // slot 9: [R] cPS2GraphicsMan_GetDTV__FPv; parameters [O]
    virtual int GetScreenWidth() = 0;                                                                    // slot 10: [R] cPS2GraphicsMan_GetScreenWidth
    virtual int GetScreenHeight() = 0;                                                                   // slot 11: [R] cPS2GraphicsMan_GetScreenHeight
    virtual void SetScreenOffset(int xoffset, int yoffset);                                              // slot 12: [R] cGraphicsMan_SetScreenOffset
    virtual int GetScreenXOffset();                                                                      // slot 13: [R] cGraphicsMan_GetScreenXOffset
    virtual int GetScreenYOffset();                                                                      // slot 14: [R] cGraphicsMan_GetScreenYOffset
    virtual void SetScreenClearColour(const tRGB* col) = 0;                                              // slot 15: [R] cPS2GraphicsMan_SetScreenClearColour
    virtual void SetAIUpdatesPerRender(int aiupdatesperrender);                                          // slot 16: [R] cGraphicsMan_SetAIUpdatesPerRender__FPv; parameters [O]
    virtual bool IsRenderReady();                                                                        // slot 17: [R] cPS2GraphicsMan_IsRenderReady__FPv; parameters [O]
    virtual bool IsRenderIdle();                                                                         // slot 18: [R] cPS2GraphicsMan_IsRenderIdle__FPv; parameters [O]
    virtual void FlushRenderer();                                                                        // slot 19: [R] cGraphicsMan_FlushRenderer
    virtual void DrawFrame() = 0;                                                                        // slot 20: [R] cPS2GraphicsMan_DrawFrame
    virtual void PushViewport() = 0;                                                                     // slot 21: [R] cPS2GraphicsMan_PushViewport
    virtual void PopViewport() = 0;                                                                      // slot 22: [R] cPS2GraphicsMan_PopViewport
    virtual void SetViewportWindow(float x, float y, float w, float rectHeight) = 0;                     // slot 23: [R] cPS2GraphicsMan_SetViewportWindow
    virtual void SetViewportWindow1(const tRect* rect, int applyScreenOffset) = 0;                       // slot 24: [R] cPS2GraphicsMan_SetViewportWindow1
    virtual void SetViewportPerspective(float fov, float minz, float maxz) = 0;                          // slot 25: [R] cPS2GraphicsMan_SetViewportPerspective
    virtual void SetViewportOrthographic(float cx, float cy, float fx, float fy, float zNear, float zFar) = 0; // slot 26: [R] cPS2GraphicsMan_SetViewportOrthographic
    virtual void SetViewportZBias(float worldBias, const t4Vector* wpos) = 0;                            // slot 27: [R] cPS2GraphicsMan_SetViewportZBias
    virtual tRect GetViewportWindow() = 0;                                                               // slot 28: [R] cPS2GraphicsMan_GetViewportWindow; returns tRect by value (retail passes a result pointer first)
    virtual float GetViewportFOV() = 0;                                                                  // slot 29: [R] cPS2GraphicsMan_GetViewportFOV
    virtual void GetViewportPerspective(float* fov, float* minz, float* maxz) = 0;                       // slot 30: [R] cPS2GraphicsMan_GetViewportPerspective
    virtual void PushView() = 0;                                                                         // slot 31: [R] cPS2GraphicsMan_PushView
    virtual void PopView() = 0;                                                                          // slot 32: [R] cPS2GraphicsMan_PopView__FPv; parameters [O]
    virtual void SetView(float pitch, float heading, float roll, const t4Vector* pos) = 0;               // slot 33: [R] cPS2GraphicsMan_SetView
    virtual void SetView1(const t4Matrix* m) = 0;                                                        // slot 34: [R] cPS2GraphicsMan_SetView1
    virtual const t4Matrix* GetView() = 0;                                                               // slot 35: [R] cPS2GraphicsMan_GetView__FPv; parameters [O]
    virtual void AppendView(const t4Matrix* m) = 0;                                                      // slot 36: [R] cPS2GraphicsMan_AppendView
    virtual void IdentityView() = 0;                                                                     // slot 37: [R] cPS2GraphicsMan_IdentityView
    virtual void TranslateView(const t4Vector* pos) = 0;                                                 // slot 38: [R] cPS2GraphicsMan_TranslateView
    virtual void RotateView(float deg, const t4Vector* axis) = 0;                                        // slot 39: [R] cPS2GraphicsMan_RotateView
    virtual void SetTVDisplayFormat(int df) = 0;                                                         // slot 40: [R] cPS2GraphicsMan_SetTVDisplayFormat
    virtual void SetTVDisplayFormat1(const tTVDisplayFormat* tvdf) = 0;                                  // slot 41: [R] cPS2GraphicsMan_SetTVDisplayFormat1
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
    virtual bool LockFrameToTexture(int idx) = 0;                                                        // slot 53: [R] cPS2GraphicsMan_LockFrameToTexture
    virtual bool RestoreFrameLock() = 0;                                                                 // slot 54: [R] cPS2GraphicsMan_RestoreFrameLock
    virtual void ShadowClearLockTexture(int texid, const tARGB* clearCol) = 0;                           // slot 55: [R] cPS2GraphicsMan_ShadowClearLockTexture
    virtual int GetTextureWidth(int texid) = 0;                                                          // slot 56: [R] cPS2GraphicsMan_GetTextureWidth
    virtual int GetTextureHeight(int texid) = 0;                                                         // slot 57: [R] cPS2GraphicsMan_GetTextureHeight
    virtual int GetTexel(unsigned int textureId, float u, float v, int filter);                          // slot 58: [R] cPS2GraphicsMan_GetTexel; O/O/O/O?
    virtual void CreateScreenGrabTexture(tScreenGrabHandle* screenGrabHandle, unsigned int width, unsigned int height); // slot 59: [R] cPS2GraphicsMan_CreateScreenGrabTexture
    virtual void RemoveScreenGrabTexture(tScreenGrabHandle* screenGrabHandle);                           // slot 60: [R] cPS2GraphicsMan_RemoveScreenGrabTexture
    virtual void TakeScreenGrab(const tScreenGrabHandle* screenGrabHandle);                              // slot 61: [R] cPS2GraphicsMan_TakeScreenGrab
    virtual void SetActiveDrawState(const sDrawState* drawstate);                                        // slot 62: [R] cGraphicsMan_SetActiveDrawState__FPv; parameters [O]
    virtual void ForceActiveDrawState(const sDrawState* drawstate);                                      // slot 63: [R] cGraphicsMan_ForceActiveDrawState
    virtual const sDrawState* ActiveDrawState();                                                         // slot 64: [R] cGraphicsMan_ActiveDrawState__FPv; parameters [O]
    virtual void SetActiveTexture(int texid, unsigned int contextID);                                    // slot 65: [R] cGraphicsMan_SetActiveTexture__FPv; parameters [O]
    virtual void ClearProjTextures();                                                                    // slot 66: [R] cPS2GraphicsMan_ClearProjTextures__FPv; parameters [O]
    virtual void AddProjectiveTexture(const t4Vector* pos, const t4Matrix* clipMat, const t4Matrix* projMat, float intensity, int texid); // slot 67: [R] cPS2GraphicsMan_AddProjectiveTexture
    virtual void SetLightMats(void* lmats) = 0;                                                          // slot 68: [R] cPS2GraphicsMan_SetLightMats__FPv; parameters [O]
    virtual void SetIrradiance(void* src) = 0;                                                           // slot 69: [R] cPS2GraphicsMan_SetIrradiance; R?
    virtual void* GetIrradiance() = 0;                                                                   // slot 70: [R] cPS2GraphicsMan_GetIrradiance__FPv; parameters [O]
    virtual void RenderTriStrip(int numVerts, tBxVertex* src) = 0;                                       // slot 71: [R] cPS2GraphicsMan_RenderTriStrip
    virtual void RenderIndexedTriList(int NumTris, int NumVerts, const unsigned char* Indices, const t3Vector* P, const t2Vector* UV, const tARGB* Color, const t3Vector* Offset); // slot 72: [R] cPS2GraphicsMan_RenderIndexedTriList
    virtual void RenderLineStrip(int n, void* vtx) = 0;                                                  // slot 73: [R] cPS2GraphicsMan_RenderLineStrip; R/R?
    virtual void RenderLineStrip(int numverts, const tBxVertex* verts, int isstatic);                    // slot 74: [R] cGraphicsMan_RenderLineStrip__FPv; parameters [O]
    virtual void RenderSpline(const t4Matrix* BezMat, const tARGB* col, int NumPoints);                  // slot 75: [R] cPS2GraphicsMan_RenderSpline
    virtual cGridMesh* CreateGridMesh(int width, int height, int memtype) = 0;                           // slot 76: [R] cPS2GraphicsMan_CreateGridMesh
    virtual void DeleteGridMesh(cGridMesh* pData);                                                       // slot 77: [R] cGraphicsMan_DeleteGridMesh__FPvi; parameters [O]
    virtual void DrawSprite(const t4Vector* pos, const t2Vector* halfSize, const t2Vector* uv0, const t2Vector* uv1, const tARGB* colour) = 0; // slot 78: [R] cPS2GraphicsMan_DrawSprite
    virtual void DrawRotatedSprite(const t4Vector* pos, const t2Vector* halfSize, const t2Vector* uv0, const t2Vector* uv1, const tARGB* colour, float rotationRads, int numPass) = 0; // slot 79: [R] cPS2GraphicsMan_DrawRotatedSprite
    virtual void DrawString3D(cFont* font, float x, float y, const char* str, float xscale, float yscale, float iwidth, float iheight, const tARGB* colour, float depth) = 0; // slot 80: [R] cPS2GraphicsMan_DrawString3D
    virtual void DrawString3D1(cFont* font, float x, float y, const unsigned short* str, float xscale, float yscale, float iwidth, float iheight, const tARGB* colour, float depth) = 0; // slot 81: [R] cPS2GraphicsMan_DrawString3D1
    virtual void RenderParticleEmitter(const tParticleEmitter* emitter) = 0;                             // slot 82: [R] cPS2GraphicsMan_RenderParticleEmitter
    virtual void RenderDynamicParticleEmitter(const cDynamicEmitter* emitter) = 0;                       // slot 83: [R] cPS2GraphicsMan_RenderDynamicParticleEmitter
    virtual void RenderDynamicParticleEmitter1(const cDynamicColourEmitter* emitter) = 0;                // slot 84: [R] cPS2GraphicsMan_RenderDynamicParticleEmitter1
    virtual void RenderSnowBoxFlakes(const tSnowBoxData* snowBoxData) = 0;                               // slot 85: [R] cPS2GraphicsMan_RenderSnowBoxFlakes
    virtual void RenderSnowBoxFluffs(const tSnowBoxData* snowBoxData) = 0;                               // slot 86: [R] cPS2GraphicsMan_RenderSnowBoxFluffs
    virtual void DrawSphere(t4Vector* pos, float* col, float radius, int segs) = 0;                      // slot 87: [R] cPS2GraphicsMan_DrawSphere
    virtual void RenderBoardTrail(const t3Vector* L2WOffset, void* verts, unsigned short* topindices, int numtopindices, unsigned short* troughindices, int numtroughindices, unsigned short* sliceindices, int numsliceindices); // slot 88: [R] cGraphicsMan_RenderBoardTrail__FPv; parameters [O]
    virtual void RenderMultipleBoardTrails(BoardTrailInfoT* pTrails, int nTrails);                       // slot 89: [R] cPS2GraphicsMan_RenderMultipleBoardTrails
    virtual void RenderBoardWake(tBoardWakeVertex** verts, int numSegmentsWide, int startIndex, int numIndices, int stripLength, float alphaMultiplier, float alphaDelta); // slot 90: [R] cPS2GraphicsMan_RenderBoardWake
    virtual int GetOutCode(const t4Vector* point) = 0;                                                   // slot 91: [R] cPS2GraphicsMan_GetOutCode
    virtual t4Vector GetScreenCoord(const t4Vector* point) = 0;                                          // slot 92: [R] cPS2GraphicsMan_GetScreenCoord; returns t4Vector by value
    virtual int ClipBoundBox(const t4Vector* iMinp, const t4Vector* iMaxp, const t4Matrix* iMat) = 0;    // slot 93: [R] cPS2GraphicsMan_ClipBoundBox
    virtual void DeformMeshMatrix(t4Matrix* mat);                                                        // slot 94: [R] cGraphicsMan_DeformMeshMatrix
    virtual void RenderModel(const tInstanceRecord* pInstance, const tMaterialBlockRecord* material, const t4Matrix* BMat, float opacity, const t4Matrix* animmats, const t4Matrix* TexMat, int flags) = 0; // slot 95: [R] cPS2GraphicsMan_RenderModel
    virtual void RenderModel(tInstanceRecord* obj, int animMats, int texMat, int renderFlags);           // slot 96: [R] cGraphicsMan_RenderModel
    virtual void GetMeshQuad(const tInstanceRecord* pInstance, int mesh, t3Vector* vert, t2Vector* uv, tARGB* rgb) = 0; // slot 97: [R] cPS2GraphicsMan_GetMeshQuad
    virtual void SetShinyBoarder(int State);                                                             // slot 98: [R] cPS2GraphicsMan_SetShinyBoarder__FPvi; parameters [O]
    virtual void DrawCharModel(void* mdl, char* base, int shadow) = 0;                                   // slot 99: [R] cPS2GraphicsMan_DrawCharModel; R?/R/R
    virtual void DrawCharShadowModel(const tMdfModelHeader* model, const char* data, const tARGB* shdwcol); // slot 100: [R] cPS2GraphicsMan_DrawCharShadowModel
    virtual void SetShadowCharMatrices(const t4Matrix* sproj, const t4Vector* sscale, const t4Vector* strans) = 0; // slot 101: [R] cPS2GraphicsMan_SetShadowCharMatrices
    virtual void SectionLoaded(int sectionId);                                                           // slot 102: [R] cGraphicsMan_SectionLoaded__FPv; parameters [O]
    virtual void InitGlobalBuffer(void* pGlobalBuffer, const tResourceID* rid);                          // slot 103: [R] cGraphicsMan_InitGlobalBuffer__FPv; parameters [O]
    virtual void PurgeGlobalBuffer(const tResourceID* rid);                                              // slot 104: [R] cGraphicsMan_PurgeGlobalBuffer__FPv; parameters [O]
    virtual void InitGlobalRGBBuffer(void* pGlobalBuffer, const tResourceID* rid);                       // slot 105: [R] cGraphicsMan_InitGlobalRGBBuffer__FPv; parameters [O]
    virtual void PurgeGlobalRGBBuffer(const tResourceID* rid);                                           // slot 106: [R] cGraphicsMan_PurgeGlobalRGBBuffer__FPv; parameters [O]
    virtual void PurgeMeshData();                                                                        // slot 107: [R] cGraphicsMan_PurgeMeshData__FPv; parameters [O]
    virtual void RemapModelMaterialId(tMdfModelHeader* model, char* modeldata, char* name, int id);      // slot 108: [R] cGraphicsMan_RemapModelMaterialId__FPv; parameters [O]
    virtual void ResolveCharModel(tMdfModelHeader* inModel, char* modeldata, int* partBoneOffset, int* numBlendedMats, tMdfBlendedMatrix* blendedMats, float* morphWeights); // slot 109: [R] cPS2GraphicsMan_ResolveCharModel
    virtual void InitCharModelPart(tMdfModelHeader* mdfheader, int nummats, tMdfBlendedMatrix* blendmats, const t4Matrix* mats); // slot 110: [R] cPS2GraphicsMan_InitCharModelPart__FPv; parameters [O]
    virtual void GenerateBlendedMatrices(const t4Matrix* matlist, int* partBoneOffsets, const tMdfBlendedMatrix* blendedMatrixTranslationTable, int numMatrices, t4Matrix* blendedmatlist); // slot 111: [R] cPS2GraphicsMan_GenerateBlendedMatrices
    virtual void PurgeCharModel(tMdfModelHeader* model);                                                 // slot 112: [R] cGraphicsMan_PurgeCharModel__FPv; parameters [O]
    virtual void PurgeCharModels();                                                                      // slot 113: [R] cGraphicsMan_PurgeCharModels__FPv; parameters [O]
    virtual int getCurrentFrameCount();                                                                  // slot 114: [R] cPS2GraphicsMan_getCurrentFrameCount__FPv; parameters [O]
    virtual void UpdateFullScreenEffects(int View);                                                      // slot 115: [R] cPS2GraphicsMan_UpdateFullScreenEffects
    virtual void RenderFullScreenEffects(int View);                                                      // slot 116: [R] cPS2GraphicsMan_RenderFullScreenEffects
    virtual void SetLensFXCallBack(void* callback, void* param) = 0;                                     // slot 117: [R] cPS2GraphicsMan_SetLensFXCallBack__FPvii; parameters [O]
    virtual void update() = 0;                                                                           // slot 118: [R] cPS2GraphicsMan_update
    virtual void loadexecpurge();                                                                        // slot 119: [R] cPS2GraphicsMan_loadexecpurge
    virtual void PlatformPurgeTextures() = 0;                                                            // slot 120: [R] cPS2GraphicsMan_PlatformPurgeTextures
    virtual void PlatformUploadTextures() = 0;                                                           // slot 121: [R] cPS2GraphicsMan_PlatformUploadTextures__FPv; parameters [O]
};

CHECK_OFFSET(cGraphicsMan, m_SpecularTextureId, 0x00C);
CHECK_OFFSET(cGraphicsMan, m_SnowSparkleDensity, 0x0AC);
CHECK_OFFSET(cGraphicsMan, mNumViews, 0x270);
CHECK_OFFSET(cGraphicsMan, mTexRefs, 0x280);
CHECK_OFFSET(cGraphicsMan, m_drawStateStack, 0xE88);
CHECK_OFFSET(cGraphicsMan, _EffectTexMap, 0xF50);
CHECK_OFFSET(cGraphicsMan, mCharSpecTexID, 0x10B0);
CHECK_SIZE(cGraphicsMan, 0x10DC);

// One board trail handed to cGraphicsMan::RenderMultipleBoardTrails (On Tour struct BoardTrailInfoT, 0x14;
// [O] RenderMultipleBoardTrails(BoardTrailInfoT* pTrails, int nTrails), retail vtable slot 89,
// cPS2GraphicsMan_RenderMultipleBoardTrails). [R] cPS2GraphicsMan_RenderMultipleBoardTrails reads the array at a 0x14
// stride (its view sPartStrip_6D10) and cBoardTrailFX's renderer (func_002EA538, view sTrailStripA538) fills one per
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

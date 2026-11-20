#ifndef SCRIPTER_VIDENGINE_H
#define SCRIPTER_VIDENGINE_H

// cVidEngine: the script engine's full-motion-video player. It owns one cMoviePlayer while a NIS video is readied or
// playing, pauses the game (pause state 10), the sound effects and the cut-scene letterbox for it, and undoes all of
// that in ResetVideoEngine. EA's source: d:/ssx/code/source/scripter/videngine.cpp (preview assert path); its
// constructor, destructor and ResetVideoEngine sit in scripter/scsndmgr, IsVideoEngineBusy / IsVideoEnginePlaying in
// scripter/all_in_one_scripter (preview linkonce IsVideoEngineBusy__10cVidEngine, IsVideoEnginePlaying__10cVidEngine).
//
// Size 0x1C: [R] the only constructor call is cSSXScriptEngine_cSSXScriptEngine's cVidEngine_cVidEngine(self + 0x500),
// which zeroes 0x0-0x14 and stores the vtable at 0x18; the next base (cDataManager) starts at 0x51C. No standalone
// allocation exists. [O] On Tour's cVidEngine is 0x10 (two 1-byte bools, no saved-state words; its vptr is not in
// the dump). No bases; the class declares virtuals (a virtual destructor only: _vt$10cVidEngine D_00482228 has one
// slot, cVidEngine__cVidEngine), so the vptr follows its data at 0x18.
// The same fields are written out inline in scripter/ssxscriptengine.h (cSSXScriptEngine + 0x500).
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [R] cVidEngine_cVidEngine / cVidEngine_ReadyVideo; [P] the July 2003 preview (IsVideoEngineBusy lw 0x0,
// IsVideoEnginePlaying lw 0x4; its destructor stores the vptr at 0x10: the preview class has no 0x10 / 0x14 words,
// retail
// added them, tools/layout_xlate.py "0x10 -> 0x18"); [U] how retail code uses the field; [O] On Tour's DWARF.

#include "layout.h"

struct cMoviePlayer;

struct cVidEngine {
    cMoviePlayer* mpPlayer;     // 0x00 [O]; [P] IsVideoEngineBusy (lw 0x0); [R] ctor 0; cVidEngine_ReadyVideo
                                //      allocates the 0x44-byte player (cMemMan_alloc(0x44, "VidEnginePlayer"));
                                //      [U] every method tests it, IsVideoEngineBusy returns != 0, ResetVideoEngine
                                //      destroys it (flags 3)
    int mIsPlaying;             // 0x04 [O] (a 1-byte bool in On Tour; retail lw/sw); [P] IsVideoEnginePlaying (lw 0x4);
                                //      [U] cVidEngine_PlayVideo sets 1, IsVideoEnginePlaying returns it,
                                //      ResetVideoEngine clears it
    int mFrameUpdated;          // 0x08 [O] (a 1-byte bool at 0x5 in On Tour); [U] cVidEngine_UpdateVideo sets 1,
                                //      cVidEngine_RenderVideo passes !it to cMoviePlayer_asyncRender and clears it
    int mNumPause;              // 0x0C [O]; [U] cVidEngine_PauseVideo counts it up / down; UpdateVideo runs only at 0
    int mSavedDisplayFormat;    // 0x10 [U] cVidEngine_PlayVideo saves GrMan's TV display format here (vtable slot 42,
                                //       cPS2GraphicsMan_GetTVDisplayFormat, first word mDisplayFormat) before setting
                                //       format 0 (slot 40, SetTVDisplayFormat); ResetVideoEngine passes it back to slot
                                //       40
    int mRestoreCutSceneMode;   // 0x14 [U] cVidEngine_PlayVideo sets 1 when the cut-scene transition manager's state
                                //      (+0x10) is neither 0 nor 2, then forces regular mode; ResetVideoEngine clears it
                                //      and calls cCutSceneTransitionMan_ForceToCutSceneMode
    void** mVtbl;               // 0x18 [R] cVidEngine_cVidEngine and cVidEngine__cVidEngine store cVidEngine_vtbl
                                //      (D_00482228); cSSXScriptEngine overwrites it with D_004821A0; [P] preview 0x10
};

CHECK_OFFSET(cVidEngine, mpPlayer, 0x00);
CHECK_OFFSET(cVidEngine, mIsPlaying, 0x04);
CHECK_OFFSET(cVidEngine, mFrameUpdated, 0x08);
CHECK_OFFSET(cVidEngine, mNumPause, 0x0C);
CHECK_OFFSET(cVidEngine, mSavedDisplayFormat, 0x10);
CHECK_OFFSET(cVidEngine, mRestoreCutSceneMode, 0x14);
CHECK_OFFSET(cVidEngine, mVtbl, 0x18);
CHECK_SIZE(cVidEngine, 0x1C);

#endif /* SCRIPTER_VIDENGINE_H */

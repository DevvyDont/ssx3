#ifndef MAIN_GAME_H
#define MAIN_GAME_H

// cGame: the in-game module (one instance, `new (0x22C, "GameModule") cGame` in ssxapp; reached as SSXApp->Game).
// It owns the subsystems of a running event: AI, world, replay, UI, cameras, streaming, and the game-flow states.
//
// A FLAT layout, like ai/rider.h and main/ssxapp.h: every field at its retail offset, no C++ inheritance yet.
// Member names are EA's, from SSX On Tour's PSP dev-build DWARF (class cGame), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors (getAI = lw 0x18 ... getMenuSys = lw 0xA4), which retail moved by a
//       constant per block (-0xC up to CrowdMan3d, -0x8 from SurfPropCache to SnowFall, -0xC from SectionMan,
//       +0x40 in the tail);
//   [R] retail's own code: cGame_cGame (vtable, state objects) and cGame_load, which allocates each subsystem with
//       an EA allocation tag ("AI", "World", "Nash", "DebugFont", "Replay", "UIEngine", "GameViewMan", ...);
//   [U] how retail code uses the field (which class's functions receive it);
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
// Retail differs from On Tour around UI (it has CrowdMan2d/3d and no m_pInGameUI), keeps the two stream flags as
// words, and has 0x10 more bytes of state objects. Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

struct cAI;
struct cWorld;
struct cSkyBox;
struct cFogParticleMan;
struct cLensFxMan;
struct cBXNash;
struct cFont;
struct cReplay;
struct cFXLoader;
struct cWScriptMan;
struct cPlaybackMan;
struct cCrowdMan2d;
struct cCrowdMan3d;
struct cMultiParticleMan;
struct cSurfPropCache;
struct cUIEngine;
struct cWorldSound;
struct cAIVisualEffectsObjNode;
struct cVisualEffectsObjNode;
struct cSSXStatsObjNode;
struct cTransitionMan;
struct cCutSceneTransitionMan;
struct cSplashManager;
struct cFlagManager;
struct cSnowFall;
struct cSectionMan;
struct cInputSync;
struct cGameViewMan;
struct cInstanceMan;
struct cOcclusionMan;
struct cWorldLightMan;
struct cBXLoadIndicator;
struct cStreamPause;
struct cMenuSys;
struct cBigFile;
struct cGameState;

struct cGame {
    void** mVtbl;                                // 0x000 [R] _vt$5cGame, stored after the base ctor func_00231CD0
    int unk_04;                                  // 0x004 base class data
    int unk_08;                                  // 0x008 base class data
    cAI* AI;                                     // 0x00C [P] getAI; [R] "AI"
    cWorld* World;                               // 0x010 [P] getWorld; [R] "World"
    cSkyBox* SkyBox;                             // 0x014 [P] getSkyBox
    cFogParticleMan* FogMan;                     // 0x018 [O] (On Tour: same offset)
    cLensFxMan* LensFxMan;                       // 0x01C [O] (On Tour: same offset)
    cBXNash* BXNash;                             // 0x020 [P] getNash; [R] "Nash"; [U] ray queries
    cFont* DebugFont;                            // 0x024 [P] getDebugFont; [R] "DebugFont"
    cReplay* Replay;                             // 0x028 [P] getReplay; [R] "Replay"; [U] cReplay_* functions
    cFXLoader* FXLoader;                         // 0x02C [P] getFXLoader; [R] "FXLoader"
    cWScriptMan* WScriptMan;                     // 0x030 [R] "WScriptMan"
    cPlaybackMan* PlaybackMan;                   // 0x034 [P] getPlaybackMan; [R] "PlaybackMan"
    cCrowdMan2d* CrowdMan2d;                     // 0x038 [P] getCrowdMan2d; [R] "CrowdMan2d"
    cCrowdMan3d* CrowdMan3d;                     // 0x03C [P] getCrowdMan3d; [R] "CrowdMan3d"
    cMultiParticleMan* MultiParticleMan;         // 0x040 [R] "MultiParticleMan"
    cSurfPropCache* SurfPropCache;               // 0x044 [P] getSurfPropCache; [U] 0xB0-byte records by surface type
    cUIEngine* UI;                               // 0x048 [P] getUI; [R] "UIEngine"; [U] cUIStateStack_findState
    cWorldSound* WorldSound;                     // 0x04C [P] getWorldSound
    cAIVisualEffectsObjNode* AIVisualEffectsMan; // 0x050 [P] getAIVisualEffectsObjNode; [R] "AIVisualEffectsMan"
    cVisualEffectsObjNode* VisualEffectsMan;     // 0x054 [P] getVisualEffectsObjNode; [R] "VisualEffectsMan"
    cSSXStatsObjNode* SSXStatsMan;               // 0x058 [R] "SSXStatsMan"
    cTransitionMan* TransitionMan[2];            // 0x05C [R] "cTransitionMan", one per player
    cCutSceneTransitionMan* CutSceneTransitionMan; // 0x064 [P] getCutSceneTransitionman; [R] "cCutSceneTransitionMan"
    cSplashManager* SplashManager[2];            // 0x068 [R] "cSplashManager", one per player
    cFlagManager* FlagManager;                   // 0x070 [P] getFlagManager; [R] "cFlagManager"
    cSnowFall* SnowFall;                         // 0x074 [P] getSnowFall; [R] "SnowFall"
    cSectionMan* SectionMan;                     // 0x078 [P] getSectionMan
    cInputSync* InputSync;                       // 0x07C [P] getInputSync; [R] "InputSync"
    int PauseState;                              // 0x080 [O] On Tour: cPauseState (4 bytes)
    cGameViewMan* ViewMan;                       // 0x084 [P] getViewMan; [R] "GameViewMan"; [U] cGameViewMan_updateAll
    cInstanceMan* InstanceMan;                   // 0x088 [P] getInstanceMan; [R] "InstancesMan"
    cOcclusionMan* OcclusionMan;                 // 0x08C [P] getOcclusionMan; [R] "OcclusionMan"
    cWorldLightMan* WorldLightMan;               // 0x090 [P] getWorldLightMan; [R] "WorldLightMan"
    cBXLoadIndicator* LoadIndicator;             // 0x094 [R] "LoadIndicator"; [U] cBXLoadIndicator_Hide
    cStreamPause* StreamPause;                   // 0x098 [R] cStreamPause_construct()
    int IsStreamPaused;                          // 0x09C [O] (On Tour: a bool); zeroed by cGame_load
    int m_bFlushStreamer;                        // 0x0A0 [O] (On Tour: a bool); zeroed by cGame_load
    cMenuSys* MenuSys;                           // 0x0A4 [P] getMenuSys; [R] "MenuSys"
    cBigFile* mpTrackBigFile;                    // 0x0A8 [R] "BigFile"
    int mUpdateTick;                             // 0x0AC [O]; zeroed by cGame_load
    char mStates[0x14C];                         // 0x0B0 [R] the cGF*State game-flow objects (constructed in place by
                                                 //       cGame_cGame; On Tour: mEventStartState ... mReplayStartState)
    cGameState* mPrevState;                      // 0x1FC [O]
    cGameState* mCurrentState;                   // 0x200 [P] getCurrentStateInstance
    cGameState* mPreloadingState;                // 0x204 [O]
    int mPendingPreloadStateID;                  // 0x208 [O]
    int mPreloadStateID;                         // 0x20C [O]
    int mPendingStateID;                         // 0x210 [O]
    int mCurrentStateID;                         // 0x214 [P] getCurrentState; [U] compared with game-state numbers
    int mPrevStateID;                            // 0x218 [P] getPreviousState
    int mPendingStateStartupFlag;                // 0x21C [O]
    int mBoxTextureID;                           // 0x220 [O]; [R] cGame_load loads a texture from data/fonts/menu.ssh
    int ControllerUnplugged;                     // 0x224 [P] getControllerUnplugged
    int PauseOwner;                              // 0x228 [O]; [R] the constructor sets it to -1
};

CHECK_OFFSET(cGame, AI, 0x0C);
CHECK_OFFSET(cGame, Replay, 0x28);
CHECK_OFFSET(cGame, UI, 0x48);
CHECK_OFFSET(cGame, TransitionMan, 0x5C);
CHECK_OFFSET(cGame, SplashManager, 0x68);
CHECK_OFFSET(cGame, ViewMan, 0x84);
CHECK_OFFSET(cGame, MenuSys, 0xA4);
CHECK_OFFSET(cGame, mStates, 0xB0);
CHECK_OFFSET(cGame, mPrevState, 0x1FC);
CHECK_OFFSET(cGame, mCurrentStateID, 0x214);
CHECK_OFFSET(cGame, PauseOwner, 0x228);
CHECK_SIZE(cGame, 0x22C);

#endif /* MAIN_GAME_H */

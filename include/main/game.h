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
#include "main/gamestate.h"

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

struct FrustumIntersector;

struct cBxReplayFrame;

struct cGame {
    void** mVtbl;                                // 0x000 [R] _vt$5cGame, stored after the base ctor cFadeScreen_cFadeScreen
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
    cGFEventStartState mEventStartState;         // 0x0B0 [O] game-flow states in On Tour's order; [R] retail offsets
                                                 //       from cGame_cGame, which builds each in place
    cGFPreEventState mPreEventState;             // 0x0D0 [O]
    cGFGateState mGateState;                     // 0x0E0 [O]
    cGFRidingState mRidingState;                 // 0x0F0 [O]
    cGFFinishlineState mFinishlineState;         // 0x100 [O]
    cGFEndState mEndState;                       // 0x118 [O]
    cGFPostEventState mPostEventState;           // 0x128 [O]
    cGFEndWaitState mEndWaitState;               // 0x138 [O]
    cGFTransportWaitState mTransportWaitState;   // 0x148 [O]
    cGFTrackStartState mTrackStartState;         // 0x158 [O]
    cGFSectionLoadState mSectionLoadState;       // 0x178 [O]
    cGFPodiumState mPodiumState;                 // 0x18C [O]
    cGFNextHeatState mNextHeatState;             // 0x19C [O]
    cGFUserSelectState mUserSelectState;         // 0x1B8 [O]
    cGFSessionTeleportState mSessionTeleportState;// 0x1DC [O]
    cGFReplayStartState mReplayStartState;       // 0x1EC [O]
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

    // Methods bound to their retail symbols by asm labels.
    void renderModels(FrustumIntersector* world) __asm__("cGame_renderModels");
    void renderPatches(FrustumIntersector* world) __asm__("cGame_renderPatches");
    void renderFogVolumes(FrustumIntersector* world) __asm__("cGame_renderFogVolumes");
    void renderLightHalos(FrustumIntersector* world, int ViewID) __asm__("cGame_renderLightHalos");
    void load() __asm__("cGame_load");
    void loadTrack() __asm__("cGame_loadTrack");
    void purge() __asm__("cGame_purge");
    void gainFocus() __asm__("cGame_gainFocus");
    void resetWorld() __asm__("cGame_resetWorld");
    void partialRestart() __asm__("cGame_partialRestart");
    void restart() __asm__("cGame_restart");
    void sectionLoaded() __asm__("cGame_sectionLoaded");
    void sectionUnloading(int id) __asm__("cGame_sectionUnloading");
    void loadWScriptManSection(int sectionId) __asm__("cGame_loadWScriptManSection");
    void exit(int toFrontEnd) __asm__("cGame_exit");
    void startReplay() __asm__("cGame_startReplay");
    void stopReplay() __asm__("cGame_stopReplay");
    int getTransitionMan(int i) __asm__("cGame_getTransitionMan__FPvi");
    int getSplashManager(int i) __asm__("cGame_getSplashManager__FPvi");
    void update() __asm__("cGame_update");
    int loseFocus() __asm__("cGame_loseFocus");
    void updateState() __asm__("cGame_updateState");
    void setState(int iNextState, int startupFlag, int refresh) __asm__("cGame_setState");
    void setPreloadState(int val) __asm__("cGame_setPreloadState__FPvi");
    void cancelPreloadingState() __asm__("cGame_cancelPreloadingState");
    int isPreloadedStateReady() __asm__("cGame_isPreloadedStateReady");
    void doNextState() __asm__("cGame_doNextState");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cGame_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cGame_readFromReplayFrame");
    int activatePauseMenu() __asm__("cGame_activatePauseMenu");
    int canUserActivatePauseMenu() __asm__("cGame_canUserActivatePauseMenu");
    int render() __asm__("cGame_render");
    int getCurrentState() __asm__("cGame_getCurrentState");
    int getPreviousState() __asm__("cGame_getPreviousState");
    int getCurrentStateInstance() __asm__("cGame_getCurrentStateInstance");
    int getNash() __asm__("cGame_getNash");
    int getWorld() __asm__("cGame_getWorld");
    int getDebugFont() __asm__("cGame_getDebugFont");
    int getMenuSys() __asm__("cGame_getMenuSys");
    int getAI() __asm__("cGame_getAI");
    int getUI() __asm__("cGame_getUI");
    int getReplay() __asm__("cGame_getReplay");
    int getFXLoader() __asm__("cGame_getFXLoader");
    int getSurfPropCache() __asm__("cGame_getSurfPropCache");
    int getWorldSound() __asm__("cGame_getWorldSound");
    int getAIVisualEffectsObjNode() __asm__("cGame_getAIVisualEffectsObjNode");
    int getVisualEffectsObjNode() __asm__("cGame_getVisualEffectsObjNode");
    int getCutSceneTransitionman() __asm__("cGame_getCutSceneTransitionman");
    int getFlagManager() __asm__("cGame_getFlagManager");
    int getSnowFall() __asm__("cGame_getSnowFall");
    int getPlaybackMan() __asm__("cGame_getPlaybackMan");
    int getSectionMan() __asm__("cGame_getSectionMan");
    int getInputSync() __asm__("cGame_getInputSync");
    int getSkyBox() __asm__("cGame_getSkyBox");
    int getCrowdMan2d() __asm__("cGame_getCrowdMan2d");
    int getCrowdMan3d() __asm__("cGame_getCrowdMan3d");
    int getMultiParticleMan() __asm__("cGame_getMultiParticleMan");
    int getInstanceMan() __asm__("cGame_getInstanceMan");
    int getOcclusionMan() __asm__("cGame_getOcclusionMan");
    int getWorldLightMan() __asm__("cGame_getWorldLightMan");
    int getLoadIndicator() __asm__("cGame_getLoadIndicator");
    int getControllerUnplugged() __asm__("cGame_getControllerUnplugged");
    int getViewMan() __asm__("cGame_getViewMan");
    int getGameView(int index) __asm__("cGame_getGameView");
    int getNumGameView() __asm__("cGame_getNumGameView");
    int getCurrentRenderView() __asm__("cGame_getCurrentRenderView");
    int getCurrentRenderViewNum() __asm__("cGame_getCurrentRenderViewNum");
    int updateStreamFocus() __asm__("cGame_updateStreamFocus");
    int updateStreamLoading() __asm__("cGame_updateStreamLoading");
    bool updateUI() __asm__("cGame_updateUI");
    void updateAI() __asm__("cGame_updateAI");
    int updateScreenFade() __asm__("cGame_updateScreenFade");
    void updateVibration() __asm__("cGame_updateVibration");
};

CHECK_OFFSET(cGame, AI, 0x0C);
CHECK_OFFSET(cGame, Replay, 0x28);
CHECK_OFFSET(cGame, UI, 0x48);
CHECK_OFFSET(cGame, TransitionMan, 0x5C);
CHECK_OFFSET(cGame, SplashManager, 0x68);
CHECK_OFFSET(cGame, ViewMan, 0x84);
CHECK_OFFSET(cGame, MenuSys, 0xA4);
CHECK_OFFSET(cGame, mEventStartState, 0xB0);
CHECK_OFFSET(cGame, mPreEventState, 0xD0);
CHECK_OFFSET(cGame, mGateState, 0xE0);
CHECK_OFFSET(cGame, mRidingState, 0xF0);
CHECK_OFFSET(cGame, mFinishlineState, 0x100);
CHECK_OFFSET(cGame, mEndState, 0x118);
CHECK_OFFSET(cGame, mPostEventState, 0x128);
CHECK_OFFSET(cGame, mEndWaitState, 0x138);
CHECK_OFFSET(cGame, mTransportWaitState, 0x148);
CHECK_OFFSET(cGame, mTrackStartState, 0x158);
CHECK_OFFSET(cGame, mSectionLoadState, 0x178);
CHECK_OFFSET(cGame, mPodiumState, 0x18C);
CHECK_OFFSET(cGame, mNextHeatState, 0x19C);
CHECK_OFFSET(cGame, mUserSelectState, 0x1B8);
CHECK_OFFSET(cGame, mSessionTeleportState, 0x1DC);
CHECK_OFFSET(cGame, mReplayStartState, 0x1EC);
CHECK_OFFSET(cGame, mPrevState, 0x1FC);
CHECK_OFFSET(cGame, mCurrentStateID, 0x214);
CHECK_OFFSET(cGame, PauseOwner, 0x228);
CHECK_SIZE(cGame, 0x22C);

#endif /* MAIN_GAME_H */

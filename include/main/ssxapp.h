#ifndef MAIN_SSXAPP_H
#define MAIN_SSXAPP_H

// cSSXApp: the application object. There is one instance (constructed by cSSXApp_cSSXApp); the global SSXApp points
// at it, and most subsystems are reached through it (SSXApp->Game, SSXApp->FELocale, ...).
//
// This is a FLAT layout, like ai/rider.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// cSSXApp derives from cAppMan (the constructor calls cAppMan_cAppMan first); that base is still raw bytes here.
//
// Member names are EA's, from SSX On Tour's PSP dev-build DWARF (class cSSXApp), placed at their retail offsets by:
//   [P] the July 2003 preview's inline accessors (getBE = lw 0x54 ... getFELocale = lw 0x68; Inputs 0x80,
//       InputMaps 0x88, getGameManager 0x90, getUserName 0x94, getNetMan 0xA0, GetFEAsyncManager 0xC4), which
//       retail moved by a constant per block (+0x24 up to FELocale, +0x30 from InputPads to NetMan);
//   [R] retail's own code: cSSXApp_cSSXApp and cSSXApp_init store each subsystem with its allocation tag
//       ("InputPad", "loadicon", "LoadIndicator", "FEAsyncManager"; cConsoleConfig_construct, cGameModeMan_getGM);
//   [U] how retail code uses the field (which class's functions receive it).
// On Tour's layout differs before Game (it adds three Apt UI members) and keeps one input per player
// (getNumInputs is 2 in SSX 3). Unnamed fields are unk_XXX; rename only with evidence, and say which kind.

#include "layout.h"
#include "bx/loadtimer.h"

struct cAppModule;
struct cExecutionMan;

struct cBE;
struct cFE;
struct cDebugFE;
struct cGame;
struct cUIFontInterface;
struct cFELocale;
struct cAnimDataBase;
struct cGameAnimMap;
struct cConfMan;
struct cConsoleConfig;
struct cLoadIndicator;
struct cInputPad;
struct cInput;
struct cInputMap;
struct cGameModeMan;
struct cNetMan;
struct cFEAsyncManager;

struct cSSXApp {
    // --- cAppMan (bx/appman.h; On Tour's names and offsets): modules, frame timer, load state ---
    cAppModule* ActiveModule;            // 0x000 [O] cAppMan::ActiveModule
    cAppModule* LoadingModule;           // 0x004 [O] cAppMan::LoadingModule; [U] non-zero while a module loads
    cExecutionMan* ExecutionMan;         // 0x008 [O] cAppMan::ExecutionMan
    int m_AIUpdatesPerRender;            // 0x00C [O] cAppMan::m_AIUpdatesPerRender
    int TimerHz;                         // 0x010 [O] cAppMan::TimerHz; [U] ticks per second (statemachine timers)
    float TimerPeriod;                   // 0x014 [O] cAppMan::TimerPeriod; [U] the frame time the timeouts count down by
    int TimerTick;                       // 0x018 [O] cAppMan::TimerTick; [U] the current timer tick
    int Tick;                            // 0x01C [O] cAppMan::Tick
    int MaxUpdateSteps;                  // 0x020 [O] cAppMan::MaxUpdateSteps
    float GameRate;                      // 0x024 [O] cAppMan::GameRate
    float FracTick;                      // 0x028 [O] cAppMan::FracTick
    float FrameRate;                     // 0x02C [O] cAppMan::FrameRate
    float FrameRateDamp;                 // 0x030 [O] cAppMan::FrameRateDamp
    int mLock30Fps;                      // 0x034 [O] cAppMan::mLock30Fps (bool)
    cLoadTimer LoadTimer;                // 0x038 [O] cAppMan::LoadTimer
    int LoadState;                       // 0x058 [O] cAppMan::LoadState (volatile eLoadState)
    void** mVtbl;                        // 0x05C cAppMan's vtable pointer (_vt$7cSSXApp after construction)
    int UseRealFE;                          // 0x060 [R] set to 1 by the constructor
    int ShowHUD;                          // 0x064 [R] set to 1 by the constructor
    int unk_68;                          // 0x068 [R] zeroed by the constructor
    int unk_6C;                          // 0x06C [R] zeroed by the constructor
    int unk_70;                          // 0x070 [R] zeroed by the constructor
    int AutoLobby;                          // 0x074 [R] zeroed by the constructor
    cBE* BE;                             // 0x078 [P] getBE; [U] passed to cBE_getInterface
    cFE* RealFE;                         // 0x07C [P] getRealFE; [U] +0xB0 is cFE::mRiderManager (as in On Tour)
    cDebugFE* DebugFE;                   // 0x080 [P] getDebugFE
    cGame* Game;                         // 0x084 [P] getGame; [U] passed to cGame_exit, cGame_partialRestart
    cUIFontInterface* Fonts;             // 0x088 [U] stored into cUIEngine::mpFontInterface (+0x8)
    cFELocale* FELocale;                 // 0x08C [P] getFELocale; [U] passed to util/locale's functions
    cAnimDataBase* AnimDataBase;         // 0x090 On Tour member order (between FELocale and ConsoleConfig)
    cGameAnimMap* GameAnimMap;           // 0x094 On Tour member order
    cConfMan* ConfMan;                   // 0x098 On Tour member order
    cConsoleConfig* ConsoleConfig;       // 0x09C [R] init stores cConsoleConfig_construct()
    int LoadIconTextureID;               // 0x0A0 [R] init loads the "loadicon" texture into it
    cLoadIndicator* LoadIndicator;       // 0x0A4 [R] init: new ("LoadIndicator")
    cInputPad* InputPads[2];             // 0x0A8 [R] init: one 0x2A4-byte "InputPad" per player
    cInput* Inputs[2];                   // 0x0B0 [P] getInput(i)
    cInputMap* InputMaps[2];             // 0x0B8 [P] getInputMap(i)
    cGameModeMan* GameModeMan;           // 0x0C0 [P] getGameManager; [R] init stores cGameModeMan_getGM()
    char* UserName;                      // 0x0C4 [P] getUserName: a cBXString (4 bytes: its text pointer)
    char* Password;                      // 0x0C8 [P] getPassword: a cBXString
    char* CommandLineTrack;              // 0x0CC On Tour member order: a cBXString (the ctor sets all three)
    cNetMan* NetMan;                     // 0x0D0 [P] getNetMan
    // Big-file handles (FILESYS_addbigsync's out handle; purge closes each with FILESYS_delbigsync). [R] zeroed by the
    // constructor; [U] cSSXApp_initload opens the named file into each; [O] On Tour's members of the same names, in the
    // same order (int handles, 4 bytes lower there like NetMan).
    int mAudioBigFile;                   // 0x0D4 [O]; [U] "data/audio/audio.big" (when present)
    int mMusicBigFile;                   // 0x0D8 [O]; [U] "data/audio/music.big" (when present)
    int mMusic2BigFile;                  // 0x0DC [O]; [U] "data/audio/music2.big" (when present)
    int mSpeechBigFile;                  // 0x0E0 [O]; [U] "data/audio/speech.big" (when present)
    int mScriptDataBigFile;              // 0x0E4 [O]; [U] "data/scripts/scdat.big"
    int mModelBigFile;                   // 0x0E8 [O]; [U] "data/char/mdlps2.big"
    int mTextureBigFile[11];             // 0x0EC [U] the D_00442100 list: "data/char/mobytxp.big" .. "viggotxp.big" (ten
                                         //       riders), then "othertxp.big" = [O] On Tour's mTextureBigFileOthers
    int mRewardsBigFile;                 // 0x118 [O]; [U] "data/char/rwrdps2.big"
    cFEAsyncManager* mFEAsyncManager;    // 0x11C [P] GetFEAsyncManager; [R] init: new ("FEAsyncManager")

    // Methods bound to their retail symbols by asm labels.
    void init(int argc, char** argv) __asm__("cSSXApp_init");
    int loadInputMap(const char* path) __asm__("cSSXApp_loadInputMap");
    void parseCommandLine(int argc, char** argv) __asm__("cSSXApp_parseCommandLine");
    bool preUpdate() __asm__("cSSXApp_preUpdate");
    void purge() __asm__("cSSXApp_purge");
    void startGameLoad() __asm__("cSSXApp_startGameLoad");
    void startFELoad() __asm__("cSSXApp_startFELoad");
    void initLocale() __asm__("cSSXApp_initLocale");
    int getBE() __asm__("cSSXApp_getBE");
    int getRealFE() __asm__("cSSXApp_getRealFE");
    int getDebugFE() __asm__("cSSXApp_getDebugFE");
    int getGame() __asm__("cSSXApp_getGame");
    int getFonts() __asm__("cSSXApp_getFonts");
    int getFELocale() __asm__("cSSXApp_getFELocale");
    int getInput(int index) __asm__("cSSXApp_getInput");
    int getInputMap(int index) __asm__("cSSXApp_getInputMap");
    int getGameManager() __asm__("cSSXApp_getGameManager");
    int getNetMan() __asm__("cSSXApp_getNetMan");
    int getConsoleConfig() __asm__("cSSXApp_getConsoleConfig");
    int getLoadIndicator() __asm__("cSSXApp_getLoadIndicator");
    int getLoadIconTextureID() __asm__("cSSXApp_getLoadIconTextureID");
    void setLoadIconTextureID(int textureId) __asm__("cSSXApp_setLoadIconTextureID");
    void endFE() __asm__("cSSXApp_endFE");
    void endGame() __asm__("cSSXApp_endGame");
    bool InGame() __asm__("cSSXApp_InGame");
    int InFE() __asm__("cSSXApp_InFE");
    int GetFEAsyncManager() __asm__("cSSXApp_GetFEAsyncManager");
    void loadexecpurge() __asm__("cSSXApp_loadexecpurge__FPv");
};

CHECK_OFFSET(cSSXApp, LoadingModule, 0x04);
CHECK_OFFSET(cSSXApp, TimerHz, 0x10);
CHECK_OFFSET(cSSXApp, TimerPeriod, 0x14);
CHECK_OFFSET(cSSXApp, TimerTick, 0x18);
CHECK_OFFSET(cSSXApp, Tick, 0x1C);
CHECK_OFFSET(cSSXApp, LoadState, 0x58);
CHECK_OFFSET(cSSXApp, mVtbl, 0x5C);
CHECK_OFFSET(cSSXApp, BE, 0x78);
CHECK_OFFSET(cSSXApp, RealFE, 0x7C);
CHECK_OFFSET(cSSXApp, Game, 0x84);
CHECK_OFFSET(cSSXApp, FELocale, 0x8C);
CHECK_OFFSET(cSSXApp, ConsoleConfig, 0x9C);
CHECK_OFFSET(cSSXApp, InputPads, 0xA8);
CHECK_OFFSET(cSSXApp, Inputs, 0xB0);
CHECK_OFFSET(cSSXApp, GameModeMan, 0xC0);
CHECK_OFFSET(cSSXApp, NetMan, 0xD0);
CHECK_OFFSET(cSSXApp, mAudioBigFile, 0xD4);
CHECK_OFFSET(cSSXApp, mModelBigFile, 0xE8);
CHECK_OFFSET(cSSXApp, mTextureBigFile, 0xEC);
CHECK_OFFSET(cSSXApp, mRewardsBigFile, 0x118);
CHECK_OFFSET(cSSXApp, mFEAsyncManager, 0x11C);

// The application object (On Tour: SSXApp, a cSSXApp * in its linker map; retail D_004A28A8).
extern cSSXApp* SSXApp;

#endif /* MAIN_SSXAPP_H */

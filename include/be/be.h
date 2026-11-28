#ifndef BE_BE_H
#define BE_BE_H

// cBE: the back end (career, profiles, race setup) - a singleton facade over the BE library (cBELibrary, the static
// databases) and the three race states (cBEPreRaceState / cBERaceState / cBEPostRaceState, be/bestate.h) that hand
// out the BE interfaces. cBE_getBE (On Tour cBE::getBE) creates it on first use: `cMemMan_alloc(4, "BE", 0, 0)` into
// cBE::instance (retail cBE_instance), `cMemMan_alloc(1, "BELibrary", 0, 0)` + cBELibrary_cBELibrary into
// cBE::library (cBE_library), the three states into cBE::aBEState[] (cBE_aBEState), currentState = 0 (pre-race) and
// its onEnter, cBE::BEInitialized (cBE_BEInitialized) = 1 and cBEState_pBE = the instance. cBE_purgeBE tears it down.
// Every BE interface is reached as cBE_getInterface__Fv(cBE_getBE(), kind) (eBE_Interface, be/beinterface.h), which
// asks the current state's getInterface (vtable slot 1).
// EA's source: d:/ssx/code/source/be/be.cpp (July 2003 preview assert path; its asserts read
// "(instance->currentState < kBESTATE_NUM) && (instance->aBEState[currentState])" and
// "instance && instance->library && "Library has not yet been created"" - the statics below).
//
// Size 0x4: [R] the allocation in cBE_getBE (no constructor call, nothing stored into the object there). One word of
// data. No cBE_* method dereferences `this` (asm: all 12 checked); cBE_instance / cBEState_pBE are only touched by
// cBE_getBE / cBE_purgeBE. Across the 488 asm files that call cBE_getBE, and every read of cSSXApp::BE (SSXApp + 0x78,
// main/ssxapp.h), the object itself is dereferenced only by the debug menu (cDebugMenuScreen ctor / update, main/
// loadscreens): it stores and reloads its CurrentBoard there.
// [O] On Tour's cBE (0xC): BoardType (tBoardTypes) 0x0, HomoType 0x4, BoardTexture 0x8, with setHomoType /
// getHomoType; retail keeps only the first word (no Homo accessors exist).
// No vtable, no base class. A FLAT layout, like main/game.h.

#include "layout.h"

struct cBEState;                     // be/bestate.h
struct cBELibrary;                   // (no header yet; [O] On Tour's class cBELibrary is all statics, size 1)

struct cBEInterface;
struct cBxReplayFrame;

struct cBE {
    int BoardType;                   // 0x0 [O] cBE::BoardType, the first member (tBoardTypes: kBoard_Board 0,
                                     //     kBoard_Skis 1; an enum, a word); [U] cDebugMenuScreen_cDebugMenuScreen
                                     //     stores its CurrentBoard ([O] tBoardTypes, fe/debugfe.h) here
                                     //     (`*(int*)cBE_getBE() =`), cDebugMenuScreen_update stores it after cycling
                                     //     (sw 0x0 via SSXApp->BE) and reloads it after a game (lw 0x0); no other
                                     //     retail access

    // Methods bound to their retail symbols by asm labels.
    cBEInterface* getInterface(int kind) __asm__("cBE_getInterface__Fv");
    void newProfile(char playerIndex) __asm__("cBE_newProfile");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cBE_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* data) __asm__("cBE_readFromReplayFrame");
    void writeToData(char* buf) __asm__("cBE_writeToData");
    void* readFromData(int data) __asm__("cBE_readFromData__FPvi");
};

CHECK_OFFSET(cBE, BoardType, 0x0);
CHECK_SIZE(cBE, 0x4);

// cBE's statics ([O] On Tour: static cBEState* aBEState[3]; static int currentState; static cBE* instance;
// static bool BEInitialized; static cBELibrary* library). [P] the preview's be.cpp asserts name instance,
// currentState, aBEState and library.
extern cBEState* cBE_aBEState[3];    // 0x005308A8 [R] cBE_getBE stores the pre-race, race and post-race states
extern int cBE_currentState;         // [R] cBE_getBE = 0; [U] cBE_setState, cBEState_gotoNextState; the replay frame
                                     //     saves / restores it (cBE_writeToReplayFrame / cBE_readFromReplayFrame)
extern cBE* cBE_instance;            // [R] cBE_getBE; [U] cBE_purgeBE frees and clears it
extern int cBE_BEInitialized;        // bool (a word) [R] cBE_getBE = 1; [U] cBE_IsBEInitialized, cBE_purgeBE = 0
extern cBELibrary* cBE_library;      // [R] cBE_getBE; [U] cBE_purgeBE (dtor, flags 3), cBE_newProfile, cBE_reloadData

#endif /* BE_BE_H */

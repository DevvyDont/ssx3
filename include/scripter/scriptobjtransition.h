#ifndef SCRIPTER_SCRIPTOBJTRANSITION_H
#define SCRIPTER_SCRIPTOBJTRANSITION_H

// cScriptObjTransition: the script object "transition" (object type 2): a cut-scene script's request for a screen
// transition (fade / wipe) of one player's view, with the camera manager that owns it. A script starts one with
// SetTransition (the in, hold and out lengths arrive in frames and are kept in seconds, / 60), Update advances its
// clock and ends it once in + hold + out have passed. Starting / ending call the object's own OnStartObjectUse /
// OnEndObjectUse (vtable slots 4 / 5), which cTransitionMan overrides to run the real screen transition.
// cTransitionMan (visualfx/transitionman.h) derives from it at +0x10 and writes these members inline under the same
// names; no other retail instance exists (cScriptObjCameraManager::mpTransitionObject points at a cTransitionMan's).
// EA's source: d:/ssx/code/source/scripter/scriptobjtransition.cpp (July 2003 preview assert strings). Retail methods:
// scripter/bxscriptengine (0x00282CD0 ctor .. 0x00282F30 ResumeObjScript) and scripter/all_in_one_scripter
// (0x00284AA8 dtor .. 0x00284AE0 GetScriptEntity).
//
// Size 0x30: [R] the ctor writes up to 0x2C (mInfo.currTime), and cTransitionMan's own members start at 0x10 + 0x30
// (mNumPause at 0x40, cTransitionMan_cTransitionMan); [O] On Tour: 0x30 with the same offsets.
// Base cScriptObject (scripter/scriptobj.h, 0x10) at 0x00, written out inline: the ctor runs
// cScriptObject_cScriptObject__FPvi(self, 2), then stores cScriptObjTransition_vtbl (0x00482060, 9 slots: 1 dtor,
// 2 PauseObjScript, 3 ResumeObjScript, 4/5 cScriptObject_OnStart/OnEndObjectUse__FPv, 6 GetNumScriptEntities,
// 7 GetScriptEntity, 8 IsTransitionPending and 9 GetViewIndex = __pure_virtual) at 0xC.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cScriptObjTransition_cScriptObjTransition (and cScriptObject_cScriptObject__FPvi);
//   [P] the July 2003 preview's inline accessor GetCameraManager (lw 0x10; tools/layout_xlate.py has no paired
//       method touching `this`, and the retail out-of-line copy reads the same offset);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptObjTransition 0x30: mpCameraManager 0x10, mPlayerID 0x14, mInfo 0x18;
//       tScriptObjTransitionInfo 0x18). tScriptObjTransitionInfo is defined in visualfx/transitionman.h (included here);
//       it could move into this header, with transitionman.h including it.

#include "layout.h"
#include "visualfx/transitionman.h"

struct cScriptObjCameraManager;

struct cScriptObjTransition {
    // --- cScriptObject (scripter/scriptobj.h, 0x10; ctor cScriptObject_cScriptObject__FPvi(self, 2))
    int mObjType;                             // 0x00 [O]; [R] cScriptObject ctor stores 2 (transition)
    int mObjID;                               // 0x04 [O]; [R] cScriptObject ctor -1
    int mInUse;                               // 0x08 [O]; [R] cScriptObject ctor 0; [U] cScriptObject_OnStart/
                                              //      OnEndObjectUse__FPv (slots 4/5) count it
    void** mVtbl;                             // 0x0C [R] cScriptObjTransition_vtbl (ctor); [U] SetTransition1 calls
                                              //      slots 5 (end) and 4 (start) through it
    // --- cScriptObjTransition
    cScriptObjCameraManager* mpCameraManager; // 0x10 [O]; [P] GetCameraManager (lw); [R] ctor 0; [U]
                                              //      SetCameraManager__FPvi stores it (declared int), GetCameraManager;
                                              //      Pause/ResumeObjScript call its cScriptObject slots 2/3 (vptr +0xC)
    int mPlayerID;                            // 0x14 [O]; [R] ctor -1; [U] SetTransition1 stores the owning player
                                              //      (-1 when ended), Update runs only while >= 0, IsInUseByPlayer
    tScriptObjTransitionInfo mInfo;           // 0x18 [O]; [R] ctor: transition 9 (none), the rest 0 (sw $0 into the
                                              //      float members); [U] SetTransition1 stores transition, inTime /
                                              //      holdTime / outTime (frames / 60, swc1), transitionColour, currTime 0;
                                              //      SetTransition__FPvii reads transitionColour (0x28); Update reads the
                                              //      three times (lwc1) and advances currTime (0x2C); GetTransitionInfo
                                              //      returns &mInfo

    // Methods ([O] On Tour's names and parameters), each bound to its retail symbol by an asm label. The virtual ones
    // are declared as plain members, since the layout above is flat.
    cScriptObjTransition() __asm__("cScriptObjTransition_cScriptObjTransition");
    void Update(float frameDelta) __asm__("cScriptObjTransition_Update");
    void SetCameraManager(cScriptObjCameraManager* pManager) __asm__("cScriptObjTransition_SetCameraManager__FPvi");
    void SetTransition(int playerID, eScriptTransition transition, float currFrame, float outFrames, float inFrames,
                       float holdFrames) __asm__("cScriptObjTransition_SetTransition__FPvii");
    void SetTransition(int playerID, eScriptTransition transition, int transitionColour, float currFrame,
                       float outFrames, float inFrames, float holdFrames)
        __asm__("cScriptObjTransition_SetTransition1");
    bool IsInUseByPlayer(int playerID) __asm__("cScriptObjTransition_IsInUseByPlayer");
    void PauseObjScript() __asm__("cScriptObjTransition_PauseObjScript");
    void ResumeObjScript() __asm__("cScriptObjTransition_ResumeObjScript");
    cScriptObjCameraManager* GetCameraManager() __asm__("cScriptObjTransition_GetCameraManager");
};

CHECK_OFFSET(cScriptObjTransition, mObjType, 0x00);
CHECK_OFFSET(cScriptObjTransition, mObjID, 0x04);
CHECK_OFFSET(cScriptObjTransition, mInUse, 0x08);
CHECK_OFFSET(cScriptObjTransition, mVtbl, 0x0C);
CHECK_OFFSET(cScriptObjTransition, mpCameraManager, 0x10);
CHECK_OFFSET(cScriptObjTransition, mPlayerID, 0x14);
CHECK_OFFSET(cScriptObjTransition, mInfo, 0x18);
CHECK_SIZE(cScriptObjTransition, 0x30);

#endif /* SCRIPTER_SCRIPTOBJTRANSITION_H */

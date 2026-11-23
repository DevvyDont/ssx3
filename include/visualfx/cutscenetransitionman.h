#ifndef VISUALFX_CUTSCENETRANSITIONMAN_H
#define VISUALFX_CUTSCENETRANSITIONMAN_H

// cCutSceneTransitionMan: blends the TV display format between regular play and cut-scene (letterbox) mode. A scene
// node (cObjNode) whose update slot advances the blend; cGame_load creates the only instance
// (cGame::CutSceneTransitionMan, main/game.h +0x64) in bucket 3. StartCutScene / EndCutScene start a timed blend in or
// out (state 3 / 2, reversing a blend already running), ForceToCutSceneMode / ForceToRegularMode jump to the end state
// (1 / 0) and hand the format to the graphics manager (its vtable slot 41); FindTVParameters saves the current format
// (slot 42) and derives the cut-scene one from it; Pause counts NIS pauses (tNISList_DoPause).
// EA's source: d:/ssx/code/source/visualfx/cutscenetransitionman.cpp (July 2003 preview assert strings). Retail
// methods: unit visualfx/boardtrailfx (0x002EA670 ctor .. 0x002EAA28 FindTVParameters) and visualfx/worldlightman
// (0x002F81B8 dtor, 0x002F81D8 GetCurrentState).
//
// Size 0x48: [R] cGame_load allocates operator new(72, "cCutSceneTransitionMan") (src/main/gameload.cpp);
// FindTVParameters / update reach 0x44 (mCutSceneTVFormat.mfYAspectAdjust). On Tour: 0x44 (its cObjNode is 0xC, 4
// bytes smaller; every member here is On Tour + 4).
// Base cObjNode (object/objnode.h; retail 0x10: unk_00, Next, Prev, vptr);
// the ctor runs cObjNode_cObjNode1(self, bucket) and stores cCutSceneTransitionMan_vtbl
// (0x00487F30, 13 cObjNode slots: 1 dtor cCutSceneTransitionMan__cCutSceneTransitionMan__FPv, 2 update; 3-13 cObjNode's).
//
// Derives from cObjNode; its own members FLAT, like main/game.h. Evidence tags:
//   [R] retail ctor cCutSceneTransitionMan_cCutSceneTransitionMan and the 0x48 allocation;
//   [P] the July 2003 preview's inline GetCurrentState (lw 0x10: tools/layout_xlate.py has no paired offset to
//       measure a shift, and the retail out-of-line copy reads the same offset);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cCutSceneTransitionMan 0x44: mCurrentState 0xC, mfCurrentTime 0x10, mfLengthOfTime 0x14,
//       mNumPause 0x18, mRegularTVFormat 0x1C, mCutSceneTVFormat 0x30; tTVDisplayFormat 0x14, defined in
//       render/ps2graphicsman.h).

#include "layout.h"
#include "object/objnode.h"
#include "render/ps2graphicsman.h"

struct cObjNode;

struct cCutSceneTransitionMan : public cObjNode {
    // cObjNode (object/objnode.h, 0x10): the vptr at 0x0C:
    //   [R] cCutSceneTransitionMan_vtbl (ctor)
    // --- cCutSceneTransitionMan
    int mCurrentState;                  // 0x10 [O] eCurrentState: 0 regular, 1 cut scene, 2 blending out, 3 blending in;
                                        //      [P] GetCurrentState (lw 0x10); [R] ctor 0; [U] StartCutScene 3,
                                        //      EndCutScene 2, ForceToRegularMode 0, ForceToCutSceneMode 1; update and
                                        //      Pause test 2 / 3; cCutSceneTransitionMan_GetCurrentState
    float mfCurrentTime;                // 0x14 [O]; [U] Start/EndCutScene reset or rescale it (lwc1/swc1), update adds
                                        //      1.0 / (float) the app manager's word +0x10
    float mfLengthOfTime;               // 0x18 [O]; [U] Start/EndCutScene store the blend time, update compares and
                                        //      divides by it
    int mNumPause;                      // 0x1C [O]; [R] ctor 0; [U] Pause counts it up / down (cleared while blending);
                                        //      update does nothing while it is non-zero
    tTVDisplayFormat mRegularTVFormat;  // 0x20 [O]; [U] FindTVParameters copies the graphics manager's current format
                                        //      (slot 42) into it; ForceToRegularMode passes its address to slot 41;
                                        //      update lerps from its four floats (and passes its mDisplayFormat)
    tTVDisplayFormat mCutSceneTVFormat; // 0x34 [O]; [U] FindTVParameters fills it (2, 0, 1.0, 0.75, 0.63 for display
                                        //      format 2, else 1, 0.125, 0.75, 0.75, 0.63); ForceToCutSceneMode passes
                                        //      its address to slot 41; update lerps to its four floats
};

CHECK_OFFSET(cCutSceneTransitionMan, mCurrentState, 0x10);
CHECK_OFFSET(cCutSceneTransitionMan, mfCurrentTime, 0x14);
CHECK_OFFSET(cCutSceneTransitionMan, mfLengthOfTime, 0x18);
CHECK_OFFSET(cCutSceneTransitionMan, mNumPause, 0x1C);
CHECK_OFFSET(cCutSceneTransitionMan, mRegularTVFormat, 0x20);
CHECK_OFFSET(cCutSceneTransitionMan, mCutSceneTVFormat, 0x34);
CHECK_SIZE(cCutSceneTransitionMan, 0x48);

#endif /* VISUALFX_CUTSCENETRANSITIONMAN_H */

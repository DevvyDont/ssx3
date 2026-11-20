#ifndef VISUALFX_TRANSITIONMAN_H
#define VISUALFX_TRANSITIONMAN_H

// cTransitionMan: one view's screen-transition manager (fade/wipe in, hold, fade out, optionally over a screen grab),
// both a scene node (cObjNode: update/render are its slots 2 and 4) and the script object "transition"
// (cScriptObjTransition: scripts start it through OnStartObjectUse). cGame_load allocates one per view; script,
// rider and AI code start/end transitions on it. EA's source: d:/ssx/code/source/visualfx/transitionman.cpp (preview
// asserts); retail methods are in the visualfx/boardwakefx splat unit (0x2E4228..0x2E4CAC) plus five one-line
// getters in visualfx/worldlightman (0x2F7FD8..0x2F7FF8).
// Size 0x84: retail cMemMan_alloc(0x84, "cTransitionMan") in cGame_load (src/main/gameload.cpp); On Tour 0x7C (its
// cObjNode is 4 bytes smaller and its bools are bytes).
// Bases: cObjNode (0x00-0x10, see object/instancenode.h) and cScriptObjTransition at +0x10 (0x30 bytes: cScriptObject
// 0x10 = mObjType/mObjID/mInUse + vptr, then cScriptObjTransition's own members); both written out inline.
// Vtables: cTransitionMan_vtbl (0x00488158, 13 cObjNode slots; slot 1 dtor, 2 update, 4 render) at 0x0C and
// D_00488100 (the cScriptObjTransition-in-cTransitionMan vtable; slots dtor, OnStartObjectUse, OnEndObjectUse,
// IsTransitionPending, GetViewIndex carry delta -0x10, so those methods get the whole object) at 0x1C.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail constructor cTransitionMan_cTransitionMan (with cObjNode_cObjNode1 and
//       cScriptObjTransition_cScriptObjTransition / cScriptObject_cScriptObject__FPvi on +0x10), the destructor
//       cTransitionMan__cTransitionMan and the 0x84 allocation;
//   [P] the July 2003 preview's inline accessors via tools/layout_xlate.py (preview == retail for every offset seen):
//       IsTransitionActive lw 0x44, IsUsingTexture lw 0x60, IsWaitingForScreenGrab lw 0x64, GetViewIndex lw 0x74
//       ("seen"), GetTransitionInitiator lw 0x78 (bracketed); retail keeps them out of line (visualfx/worldlightman);
//   [U] how retail code uses the field (src/visualfx/boardwakefx.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cObjNode, cScriptObject, cScriptObjTransition, tScriptObjTransitionInfo,
//       tScreenGrabHandle, cTransitionMan): cTransitionMan's own members in On Tour's order, each On Tour offset + 4
//       up to mfOutTime, + 4/+ 8 after the two byte bools.

#include "layout.h"

struct cObjNode;
struct cScriptObjCameraManager;
struct cTransitionObj;

// The script's transition request (On Tour tScriptObjTransitionInfo, 0x18). [R] cScriptObjTransition's ctor stores
// transition = 9 and zeroes the rest; [U] cTransitionMan_OnStartObjectUse reads all six (it passes outTime (+0xC) as
// StartTransition's inTime and inTime (+0x4) as its outTime).
struct tScriptObjTransitionInfo {
    int transition;                 // 0x00 [O] eScriptTransition; [U] 7 = screen-grab transition
    float inTime;                   // 0x04 [O]
    float holdTime;                 // 0x08 [O]
    float outTime;                  // 0x0C [O]
    int transitionColour;           // 0x10 [O]; [U] passed to cSSXScriptEngine_MakeTransitionObject
    float currTime;                 // 0x14 [O]; [U] OnStartObjectUse/update copy it to mfCurrentTime
};

CHECK_OFFSET(tScriptObjTransitionInfo, transition, 0x00);
CHECK_OFFSET(tScriptObjTransitionInfo, inTime, 0x04);
CHECK_OFFSET(tScriptObjTransitionInfo, holdTime, 0x08);
CHECK_OFFSET(tScriptObjTransitionInfo, outTime, 0x0C);
CHECK_OFFSET(tScriptObjTransitionInfo, transitionColour, 0x10);
CHECK_OFFSET(tScriptObjTransitionInfo, currTime, 0x14);
CHECK_SIZE(tScriptObjTransitionInfo, 0x18);

// A grabbed screen texture (On Tour tScreenGrabHandle, 0x8; ctor: mMemory 0, mTextureId -1). The graphics manager's
// slots 59 (create), 60 (release) and 61 take its address.
struct tScreenGrabHandle {
    char* mMemory;                  // 0x0 [O]; [R] cTransitionMan's ctor zeroes it
    int mTextureId;                 // 0x4 [O]; [R] ctor -1; [U] -1 = no grab (StartTransition, EndTransition test ~id)
};

CHECK_OFFSET(tScreenGrabHandle, mMemory, 0x0);
CHECK_OFFSET(tScreenGrabHandle, mTextureId, 0x4);
CHECK_SIZE(tScreenGrabHandle, 0x8);

struct cTransitionMan {
    // --- cObjNode (retail 0x10; object/instancenode.h)
    int unk_00;                     // 0x00 never written by any node ctor (see object/instancenode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [R] cObjNode_cObjNode1 links it into the bucket
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev
    void** mVtbl;                   // 0x0C [R] cTransitionMan_vtbl (ctor and dtor)
    // --- cScriptObjTransition at +0x10 (On Tour 0x30): cScriptObject (0x10) first
    int mObjType;                   // 0x10 [O] cScriptObject::mObjType; [R] cScriptObject_cScriptObject__FPvi stores
                                    //       the
                                    //       type (cScriptObjTransition passes 2)
    int mObjID;                     // 0x14 [O] cScriptObject::mObjID; [R] cScriptObject ctor -1
    int mInUse;                     // 0x18 [O] cScriptObject::mInUse; [R] cScriptObject ctor 0; [U] update: > 0 -> the
                                    //       script drives the time (lw 0x18)
    void** mScriptVtbl;             // 0x1C [R] cScriptObject's vptr: cTransitionMan's ctor/dtor store D_00488100
                                    //       (cScriptObjTransition_vtbl from the base ctor)
    cScriptObjCameraManager* mpCameraManager; // 0x20 [O] cScriptObjTransition; [R] base ctor 0
    int mPlayerID;                  // 0x24 [O] cScriptObjTransition; [R] base ctor -1
    tScriptObjTransitionInfo mInfo; // 0x28 [O] cScriptObjTransition; [R] base ctor stores 9, 0 x 5; [U]
                                    //       OnStartObjectUse
                                    //       reads 0x28-0x38 (lw), update reads currTime (lwc1 0x3C)
    // --- cTransitionMan (On Tour 0x3C)
    int mNumPause;                  // 0x40 [O]; [R] ctor 0; [U] PauseTransition counts it up/down, update skips while
                                    //       nonzero, EndTransition clears it
    int mbPlayingTransition;        // 0x44 [O] bool (word); [P] IsTransitionActive (lw 0x44); [R] ctor 0; [U]
                                    //       StartTransition
                                    //       1, EndTransition 0, update/render/GetTransitionTimeLeft test it
    float mfCurrentTime;            // 0x48 [O]; [U] StartTransition 0 (sw $0), update advances it (lwc1/swc1),
                                    //       OnStartObjectUse copies mInfo.currTime, render/GetTransitionTimeLeft read
                                    //       it
    cTransitionObj* mTransitionIn;  // 0x4C [O]; [R] ctor 0; [U] StartTransition stores it, EndTransition deletes it
                                    //       (virtual slot 1, flags 3), render draws it
    cTransitionObj* mTransitionOut; // 0x50 [O]; [R] ctor 0; [U] as mTransitionIn; StartTransitionOut adds one to a
                                    //       running
                                    //       transition
    float mfInTime;                 // 0x54 [O]; [U] StartTransition; render/update/GetTransitionTimeLeft sum the three
                                    //       times
    float mfHoldTime;               // 0x58 [O]; [U] StartTransition; render
    float mfOutTime;                // 0x5C [O]; [U] StartTransition, StartTransitionOut; render
    int mUsingScreenGrab;           // 0x60 [O] bool (word; On Tour 0x5C); [P] IsUsingTexture (lw 0x60); [R] ctor 0;
                                    //       [U] StartTransition = useScreenGrab, EndTransition 0, IsTransitionPending
    int mRequestingScreenGrab;      // 0x64 [O] bool (word; On Tour 0x5D); [P] IsWaitingForScreenGrab (lw 0x64); [R]
                                    //       ctor 0;
                                    //       [U] StartTransition 1 with a grab, render clears it after grabbing (slot
                                    //       61)
    tScreenGrabHandle mScreenGrabHandle; // 0x68 [O]; [R] ctor 0 / -1; [U] &handle passed to graphics slots 59/60/61
                                    //       (StartTransition, dtor, update, render); EndTransition tests mTextureId
    float* mExternalTimePercentage; // 0x70 [O]; [R] ctor 0; [U] StartTransition stores it, EndTransition clears it,
                                    //       update
                                    //       derefs it (time = *p * total)
    int mViewIndex;                 // 0x74 [O]; [P] GetViewIndex (lw 0x74); [R] ctor = viewIndex; [U] render draws only
                                    //       for the current view
    int mCurrentInitiator;          // 0x78 [O] eTransitionInitiator; [P] GetTransitionInitiator (lw 0x78, bracketed);
                                    //       [R] ctor 0; [U] StartTransition = initiator, EndTransition 0
    int mRequestingDelete;          // 0x7C [O] bool (word; On Tour 0x74); [R] ctor 0; [U] EndTransition sets it while a
                                    //       grab is
                                    //       held; update/dtor release the grab (slot 60) and clear it
    int mRequestDeleteFrame;        // 0x80 [O]; [R] ctor 0; [U] EndTransition stores graphics slot 114 (frame count);
                                    //       update releases the grab 3 frames later
};

CHECK_OFFSET(cTransitionMan, unk_00, 0x00);
CHECK_OFFSET(cTransitionMan, Next, 0x04);
CHECK_OFFSET(cTransitionMan, Prev, 0x08);
CHECK_OFFSET(cTransitionMan, mVtbl, 0x0C);
CHECK_OFFSET(cTransitionMan, mObjType, 0x10);
CHECK_OFFSET(cTransitionMan, mObjID, 0x14);
CHECK_OFFSET(cTransitionMan, mInUse, 0x18);
CHECK_OFFSET(cTransitionMan, mScriptVtbl, 0x1C);
CHECK_OFFSET(cTransitionMan, mpCameraManager, 0x20);
CHECK_OFFSET(cTransitionMan, mPlayerID, 0x24);
CHECK_OFFSET(cTransitionMan, mInfo, 0x28);
CHECK_OFFSET(cTransitionMan, mNumPause, 0x40);
CHECK_OFFSET(cTransitionMan, mbPlayingTransition, 0x44);
CHECK_OFFSET(cTransitionMan, mfCurrentTime, 0x48);
CHECK_OFFSET(cTransitionMan, mTransitionIn, 0x4C);
CHECK_OFFSET(cTransitionMan, mTransitionOut, 0x50);
CHECK_OFFSET(cTransitionMan, mfInTime, 0x54);
CHECK_OFFSET(cTransitionMan, mfHoldTime, 0x58);
CHECK_OFFSET(cTransitionMan, mfOutTime, 0x5C);
CHECK_OFFSET(cTransitionMan, mUsingScreenGrab, 0x60);
CHECK_OFFSET(cTransitionMan, mRequestingScreenGrab, 0x64);
CHECK_OFFSET(cTransitionMan, mScreenGrabHandle, 0x68);
CHECK_OFFSET(cTransitionMan, mExternalTimePercentage, 0x70);
CHECK_OFFSET(cTransitionMan, mViewIndex, 0x74);
CHECK_OFFSET(cTransitionMan, mCurrentInitiator, 0x78);
CHECK_OFFSET(cTransitionMan, mRequestingDelete, 0x7C);
CHECK_OFFSET(cTransitionMan, mRequestDeleteFrame, 0x80);
CHECK_SIZE(cTransitionMan, 0x84);

#endif /* VISUALFX_TRANSITIONMAN_H */

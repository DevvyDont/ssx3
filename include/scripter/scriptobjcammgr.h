#ifndef SCRIPTER_SCRIPTOBJCAMMGR_H
#define SCRIPTER_SCRIPTOBJCAMMGR_H

// cScriptObjCameraManager: the NIS script object that runs the cut-scene cameras: nine camera script entities (one
// per camera track / sub-type), the two cScriptObjCamera objects it blends, the transition object, beat-synced cuts
// and a focus box. cSSXScriptEngine embeds two (mObjectCameraManager[2] at 0x6D0, built by the engine ctor with
// cScriptObjCameraManager_cScriptObjCameraManager; On Tour has four).
//
// Size 0x120: [R] the engine's ctor builds 2 x 0x120; the last member reached is mUseFocusBox (0x110), so 0x114-0x11F
// is tail padding to the 16-byte alignment of mFocusBox (On Tour: also 0x120).
// Bases (On Tour DWARF lists only cScriptEntity; the retail ctor and vtables show both):
//   cScriptObject  at 0x00: mObjType, mObjID, mInUse, vptr at 0x0C (primary: cScriptObjCameraManager_vtbl, 7 slots:
//                  1 dtor, 2 cScriptObject_PauseObjScript, 3 cScriptObject_ResumeObjScript, 4 OnStartObjectUse,
//                  5 OnEndObjectUse, 6 GetNumScriptEntities, 7 GetScriptEntity);
//   cScriptEntity  at 0x10: vptr at 0x10 (cScriptObjCameraManager_cScriptEntity_vtbl = _vt$23cScriptObjCameraManager$13cScriptEntity; its overrides
//                  OnInSeqKeyEvent / OnInSeqUpdate / IsMatchingEntity carry delta -0x10, so they get the full object),
//                  mDBID, mpEntity.
// g++ 2.95 puts each base's vptr after that base's own data. EA's source:
// d:/ssx/code/source/scripter/scriptobjcammgr.cpp
// (preview strings); retail methods: unit scripter/ssxscriptengine 0x00281018-0x00281C68, plus the dtor and the
// out-of-line GetViewIndex in unit scripter/all_in_one_scripter.
//
// A FLAT layout, like main/game.h. Member names, order and types are EA's, from SSX On Tour's PSP DWARF ([O] class
// cScriptObjCameraManager, cScriptObject, cScriptEntity). Retail = On Tour up to mMainCameraIndex (0xE4); On Tour's
// three
// bools packed at 0xE8-0xEA are words in retail, so mLastBeatTimeLeft and mViewIndex are On Tour + 0x8 (checked against
// On Tour's release ResetScriptManagementVars: mBeatOccurred = 1 at 0xEA, 1e9 at 0xEC; retail 1 at 0xF0, 1e9 at 0xF4).
// [P] the July 2003 preview's GetViewIndex__23cScriptObjCameraManager reads mViewIndex at 0xF8 (the retail offset).
// Enums and bools are words.

#include "layout.h"
#include "bx/math3d.h"

struct cScriptPlayerEntity;
struct cScriptObjCamera;
struct cScriptObjTransition;

struct cScriptObjCameraManager {
    // --- cScriptObject (ctor cScriptObject_cScriptObject__FPvi(self, 0))
    int mObjType;                               // 0x000 [O]; [R] cScriptObject_cScriptObject stores its objType (0
                                                //       here)
    int mObjID;                                 // 0x004 [O]; [R] cScriptObject ctor -1; [U] OnStartObjectUse /
                                                //       OnEndObjectUse read it
    int mInUse;                                 // 0x008 [O]; [R] cScriptObject ctor 0; [U] Update tests it
    void** mVtbl;                               // 0x00C [R] the ctor / dtor store cScriptObjCameraManager_vtbl
    // --- cScriptEntity (ctor cScriptEntity_cScriptEntity__FPv(self+0x10))
    void** mEntityVtbl;                         // 0x010 [R] the ctor stores cScriptObjCameraManager_cScriptEntity_vtbl (the cScriptEntity vtable)
    int mDBID;                                  // 0x014 [O]; [R] cScriptEntity ctor -1
    cScriptPlayerEntity* mpEntity;              // 0x018 [O]; [R] cScriptEntity ctor 0; [U] Update reads it
    // --- cScriptObjCameraManager
    char mEntityCameras[9][0x14];               // 0x01C [O] cScriptEntityCamera[9]; [R] the ctor runs
                                                //       cScriptEntityCamera_cScriptEntityCamera on 9 x 0x14 and sets
                                                //       each
                                                //       one's mpCamManager (+0x10) to self. Each is {vptr 0x0, mDBID
                                                //       0x4,
                                                //       mpEntity 0x8, mSubType 0xC (ctor 9), mpCamManager 0x10}
    cScriptObjCamera* mpCameraObjects[2];       // 0x0D0 [O]; [R] the ctor zeroes both; [U] Add/RemoveCameraObject,
                                                //       SetupCameraObject, OnEndObjectUse
    cScriptObjTransition* mpTransitionObject;   // 0x0D8 [O]; [R] ctor 0; [U] SetTransitionObject stores it;
                                                //       DoCameraTransition / OnInSeqKeyEvent / OnInSeqUpdate read it
    int mAttachedCameraObject[2];               // 0x0DC [O]; [U] ResetScriptManagementVars sets both to -1;
                                                //       RemoveCameraObject
    int mMainCameraIndex;                       // 0x0E4 [O]; [U] ResetScriptManagementVars 0 (On Tour: 0 at 0xE4)
    int mInTransition;                          // 0x0E8 [O] bool; [U] ResetScriptManagementVars 0; set by
                                                //       SetTransitionObject / DoCameraTransition, read by OnInSeqUpdate
    int mCutOnBeat;                             // 0x0EC [O] bool; [U] ResetScriptManagementVars 0; OnInSeqKeyEvent /
                                                //       OnInSeqUpdate
    int mBeatOccurred;                          // 0x0F0 [O] bool; [U] ResetScriptManagementVars 1 (On Tour: sb 1 at
                                                //       0xEA); OnInSeqKeyEvent reads, OnInSeqUpdate stores
    float mLastBeatTimeLeft;                    // 0x0F4 [O]; [U] ResetScriptManagementVars 1e9 (On Tour 0xEC);
                                                //       OnInSeqUpdate (lwc1/swc1)
    int mViewIndex;                             // 0x0F8 [O]; [P] GetViewIndex lw 0xF8; [R] ctor -1; [U]
                                                //       SetViewIndex__FPvi stores it; Update / IsMatchingEntity read it
    char unk_0FC[0x4];                          // 0x0FC untouched: alignment of mFocusBox (On Tour has the same gap)
    t4Vector mFocusBox;                         // 0x100 [O] cFocusBox (its only member: t4Vector Position);
                                                //       [U] ResetScriptManagementVars stores (0, 0, 0, 1) with sq,
                                                //       Update stores it (sq), OnStart/OnEndObjectUse pass its address
    int mUseFocusBox;                           // 0x110 [O] bool; [U] ResetScriptManagementVars 0;
                                                //       OnStartObjectUse / OnEndObjectUse
    char unk_114[0xC];                          // 0x114 untouched: tail padding to 16 bytes
};

CHECK_OFFSET(cScriptObjCameraManager, mObjType, 0x000);
CHECK_OFFSET(cScriptObjCameraManager, mObjID, 0x004);
CHECK_OFFSET(cScriptObjCameraManager, mInUse, 0x008);
CHECK_OFFSET(cScriptObjCameraManager, mVtbl, 0x00C);
CHECK_OFFSET(cScriptObjCameraManager, mEntityVtbl, 0x010);
CHECK_OFFSET(cScriptObjCameraManager, mDBID, 0x014);
CHECK_OFFSET(cScriptObjCameraManager, mpEntity, 0x018);
CHECK_OFFSET(cScriptObjCameraManager, mEntityCameras, 0x01C);
CHECK_OFFSET(cScriptObjCameraManager, mpCameraObjects, 0x0D0);
CHECK_OFFSET(cScriptObjCameraManager, mpTransitionObject, 0x0D8);
CHECK_OFFSET(cScriptObjCameraManager, mAttachedCameraObject, 0x0DC);
CHECK_OFFSET(cScriptObjCameraManager, mMainCameraIndex, 0x0E4);
CHECK_OFFSET(cScriptObjCameraManager, mInTransition, 0x0E8);
CHECK_OFFSET(cScriptObjCameraManager, mCutOnBeat, 0x0EC);
CHECK_OFFSET(cScriptObjCameraManager, mBeatOccurred, 0x0F0);
CHECK_OFFSET(cScriptObjCameraManager, mLastBeatTimeLeft, 0x0F4);
CHECK_OFFSET(cScriptObjCameraManager, mViewIndex, 0x0F8);
CHECK_OFFSET(cScriptObjCameraManager, unk_0FC, 0x0FC);
CHECK_OFFSET(cScriptObjCameraManager, mFocusBox, 0x100);
CHECK_OFFSET(cScriptObjCameraManager, mUseFocusBox, 0x110);
CHECK_OFFSET(cScriptObjCameraManager, unk_114, 0x114);
CHECK_SIZE(cScriptObjCameraManager, 0x120);

#endif /* SCRIPTER_SCRIPTOBJCAMMGR_H */

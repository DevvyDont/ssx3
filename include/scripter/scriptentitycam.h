#ifndef SCRIPTER_SCRIPTENTITYCAM_H
#define SCRIPTER_SCRIPTENTITYCAM_H

// cScriptEntityCamera: a camera script entity, one camera track of a cScriptObjCameraManager. When the InSeq player
// locks a camera entity of a script onto it (OnInSeqLock), it reads the camera's sub-type from the entity's camera
// header (manual / target / subject: byte +0x21; algorithmic: byte +0x1C); OnInSeqRelease resets it to 9 (none).
// IsMatchingEntity takes entity types 0-3 whose script plays for the manager's view (mViewIndex).
// cScriptObjCameraManager holds nine (mEntityCameras[9] at +0x1C, scripter/scriptobjcammgr.h): its ctor builds each
// with cScriptEntityCamera_cScriptEntityCamera and points mpCamManager at itself.
// EA's source: d:/ssx/code/source/scripter/scriptentitycam.cpp (July 2003 preview assert strings). Retail methods:
// scripter/bxscriptengine (0x00283088 ctor .. 0x002831B0 IsMatchingEntity) and scripter/all_in_one_scripter
// (0x00284890 SetCameraManager, 0x00284898 GetSubType).
//
// Size 0x14: [R] cScriptObjCameraManager_cScriptObjCameraManager builds 9 x 0x14 (mEntityCameras 0x1C .. 0xD0, its
// mpCameraObjects at 0xD0); the ctor writes 0x0-0x10. [O] On Tour: 0x14 with the same offsets.
// Base cScriptEntity (scripter/scriptentity.h, 0xC; vptr at 0x0) at 0x00, written out inline. Retail vtable
// cScriptEntityCamera_vtbl (0x004823E0, 5 slots: 1 OnInSeqLock, 2 OnInSeqRelease, 3/4 cScriptEntity_OnInSeqKeyEvent /
// OnInSeqUpdate, 5 IsMatchingEntity; no destructor).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cScriptEntityCamera_cScriptEntityCamera (after cScriptEntity_cScriptEntity__FPv);
//   [P] the July 2003 preview's inline accessors via tools/layout_xlate.py: GetSubType lw 0xC ("seen": the paired
//       OnInSeqLock / OnInSeqRelease store 0xC in both builds), SetCameraManager sw 0x10;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptEntityCamera 0x14: mSubType 0xC, mpCamManager 0x10; cScriptEntity).

#include "layout.h"

struct cScriptPlayerEntity;
struct cScriptObjCameraManager;

struct cScriptEntityCamera {
    // --- cScriptEntity (scripter/scriptentity.h, 0xC; ctor cScriptEntity_cScriptEntity__FPv(self))
    void** mVtbl;                          // 0x00 [R] cScriptEntityCamera_vtbl (ctor, over cScriptEntity_vtbl)
    int mDBID;                             // 0x04 [O]; [R] cScriptEntity ctor -1
    cScriptPlayerEntity* mpEntity;         // 0x08 [O]; [R] cScriptEntity ctor 0; [U] cScriptEntity_OnInSeqLock /
                                           //      OnInSeqRelease (called by this class's overrides) set and clear it
    // --- cScriptEntityCamera
    int mSubType;                          // 0x0C [O]; [P] GetSubType (lw); [R] ctor 9 (none); [U] OnInSeqLock stores
                                           //      the camera header's signed sub-type byte, OnInSeqRelease 9,
                                           //      cScriptEntityCamera_GetSubType
    cScriptObjCameraManager* mpCamManager; // 0x10 [O]; [P] SetCameraManager (sw); [R] ctor 0; [U]
                                           //      cScriptEntityCamera_SetCameraManager (declared int), the camera
                                           //      manager's ctor sets it to itself; IsMatchingEntity reads its
                                           //      mViewIndex (+0xF8)

    // Methods ([O] On Tour's names), each bound to its retail symbol by an asm label. The virtual ones are declared as
    // plain members, since the layout above is flat; retail's C declares only the parameters they use.
    cScriptEntityCamera() __asm__("cScriptEntityCamera_cScriptEntityCamera");
    void OnInSeqLock(cScriptPlayerEntity* pEntity) __asm__("cScriptEntityCamera_OnInSeqLock");
    void OnInSeqRelease() __asm__("cScriptEntityCamera_OnInSeqRelease");
    bool IsMatchingEntity(cScriptPlayerEntity* pEntity, int type) __asm__("cScriptEntityCamera_IsMatchingEntity");
};

CHECK_OFFSET(cScriptEntityCamera, mVtbl, 0x00);
CHECK_OFFSET(cScriptEntityCamera, mDBID, 0x04);
CHECK_OFFSET(cScriptEntityCamera, mpEntity, 0x08);
CHECK_OFFSET(cScriptEntityCamera, mSubType, 0x0C);
CHECK_OFFSET(cScriptEntityCamera, mpCamManager, 0x10);
CHECK_SIZE(cScriptEntityCamera, 0x14);

#endif /* SCRIPTER_SCRIPTENTITYCAM_H */

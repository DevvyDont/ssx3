#ifndef SCRIPTER_SCRIPTENTITY_H
#define SCRIPTER_SCRIPTENTITY_H

// cScriptEntity: the "actor" side of a NIS script binding. A script object (cScriptObjRider, cScriptObjCameraManager,
// cScriptObjEvent, cScriptObjGS, ...) has one or more of these; the InSeq player locks a cScriptPlayerEntity onto one
// (OnInSeqLock / OnInSeqRelease) and calls its key-event / update hooks while the script runs.
// EA's header is d:/ssx/code/source/scripter/scriptentity.h (July 2003 preview assert strings; scriptentity.cpp too).
// Retail methods: scripter/bxscriptengine.cpp (0x282F60 ctor .. 0x283060 ResumeEntityScript) and
// scripter/all_in_one_scripter.cpp (0x284850 GetPlayer .. 0x284888 IsMatchingEntity).
// Retail vtable cScriptEntity_vtbl (D_00482418, 5 slots, no destructor: 1 OnInSeqLock, 2 OnInSeqRelease, 3 OnInSeqKeyEvent,
// 4 OnInSeqUpdate, 5 IsMatchingEntity); subclasses cScriptEntityCamera (0x14: + mSubType 0xC, mpCamManager 0x10) and the
// script objects that inherit it (cScriptObjRider at +0x0, cScriptObjCameraManager at +0x10).
//
// Size 0xC: never allocated on its own; [R] cScriptEntity_cScriptEntity__FPv writes 0x0-0x8, and every derived class
// places its next member at +0xC (cScriptEntityCamera's mSubType, cScriptObjRider's cScriptObject part, the camera
// manager's mEntityCameras at 0x10 + 0xC). On Tour: 0xC with the same member offsets.
// The vptr is at 0x0 here, not after the data: the ctor stores cScriptEntity_vtbl at +0x0, and the derived objects' tables
// (cScriptObjRider_vtbl at cScriptObjRider+0x0, cScriptObjCameraManager_cScriptEntity_vtbl at cScriptObjCameraManager+0x10) agree.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cScriptEntity_cScriptEntity__FPv;
//   [P] the preview's inline accessors GetDatabaseID (lw 0x4) and GetPlayer (lw 0x8) (tools/layout_xlate.py: no paired
//       method to measure a shift, and the retail out-of-line copies read the same offsets);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptEntity 0xC: mDBID 0x4, mpEntity 0x8).

#include "layout.h"

struct cScriptPlayerEntity;

struct cScriptEntity {
    void** mVtbl;                  // 0x0 [R] cScriptEntity_vtbl (ctor); subclasses store their own
    int mDBID;                     // 0x4 [O]; [P] GetDatabaseID (lw); [R] ctor -1; [U] cScriptEntity_GetDatabaseID (lw),
                                   //      cScriptEntity_SetDatabaseID (sw), cScriptEntity_ClearDatabaseID (sw -1)
    cScriptPlayerEntity* mpEntity; // 0x8 [O]; [P] GetPlayer (lw); [R] ctor 0; [U] OnInSeqLock stores the entity,
                                   //      OnInSeqRelease clears it; IsPlaying tests it; DetachEntity / PauseEntityScript /
                                   //      ResumeEntityScript / cScriptEntity_GetPlayer read it

    // Methods ([O] On Tour's names), each bound to its retail symbol by an asm label. The virtual ones (OnInSeqLock,
    // OnInSeqRelease) are declared as plain members, since the layout above is flat; retail's C declares only the
    // parameters they use.
    cScriptEntity() __asm__("cScriptEntity_cScriptEntity__FPv");
    bool IsPlaying() __asm__("cScriptEntity_IsPlaying__FPv");
    void DetachEntity() __asm__("cScriptEntity_DetachEntity");
    void OnInSeqLock(cScriptPlayerEntity* pEntity) __asm__("cScriptEntity_OnInSeqLock");
    void OnInSeqRelease() __asm__("cScriptEntity_OnInSeqRelease");
    void PauseEntityScript() __asm__("cScriptEntity_PauseEntityScript");
    void ResumeEntityScript() __asm__("cScriptEntity_ResumeEntityScript");
    int GetPlayer() __asm__("cScriptEntity_GetPlayer");
    int GetDatabaseID() __asm__("cScriptEntity_GetDatabaseID");
    void SetDatabaseID(int databaseId) __asm__("cScriptEntity_SetDatabaseID");
    void ClearDatabaseID() __asm__("cScriptEntity_ClearDatabaseID");
};

CHECK_OFFSET(cScriptEntity, mVtbl, 0x0);
CHECK_OFFSET(cScriptEntity, mDBID, 0x4);
CHECK_OFFSET(cScriptEntity, mpEntity, 0x8);
CHECK_SIZE(cScriptEntity, 0xC);

#endif /* SCRIPTER_SCRIPTENTITY_H */

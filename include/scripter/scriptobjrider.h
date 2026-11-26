#ifndef SCRIPTER_SCRIPTOBJRIDER_H
#define SCRIPTER_SCRIPTOBJRIDER_H

// cScriptObjRider: a rider's NIS script object. Every cRider is one (cRider_cRider builds it at cRider+0x6D0 with
// cScriptObjRider_cScriptObjRider; ai/rider.h's mScriptObjRiderVtbl / mScriptObjectVtbl / mScriptMatrix are its fields).
// When a cut-scene script takes the rider (OnStartObjectUse) it builds the base transform the script animates from
// (actor header offset/rotations + the script's anchor) and binds the script's animation and sound banks.
// EA's source is d:/ssx/code/source/scripter/scriptobjrider.cpp (July 2003 preview assert strings). Retail methods:
// scripter/ssxscriptengine.cpp (0x27F968 ctor .. 0x280560 IsSuitableRole) and scripter/all_in_one_scripter.cpp
// (0x284AE8 dtor .. 0x284B78 GetBaseRotZ).
// Bases, in retail order (the ctor: cScriptEntity_cScriptEntity(self), cScriptObject_cScriptObject(self + 0xC, 3)):
//   cScriptEntity (scripter/scriptentity.h, 0xC) at 0x00: vptr 0x00 = cScriptObjRider_vtbl (D_00481FD8: 1-4 cScriptEntity's
//                 OnInSeq* hooks, 5 cScriptObjRider_IsMatchingEntity, 6 GetAttachedRider = __pure_virtual, cRider overrides it);
//   cScriptObject (0x10) at 0x0C: mObjType, mObjID, mInUse, vptr 0x18 = D_00482018 (_vt$15cScriptObjRider$13cScriptObject:
//                 1 dtor, 2/3 cScriptObject_Pause/ResumeObjScript, 4 OnStartObjectUse, 5 OnEndObjectUse,
//                 6 GetNumScriptEntities, 7 GetScriptEntity; slots 1 and 4-7 carry delta -0xC, so they get the whole object).
// (cScriptObjCameraManager, scripter/scriptobjcammgr.h, has the two bases the other way round.)
//
// Size 0x80: never allocated on its own; [R] cRider's next field after it (mAudioBraking, ai/rider.h 0x750) is at
// 0x6D0 + 0x80, and the last member reached is mBaseRotZ (0x78) in a 16-byte-aligned class (t4Matrix). On Tour: 0x80.
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline. Evidence tags:
//   [R] retail ctor cScriptObjRider_cScriptObjRider and dtor cScriptObjRider__cScriptObjRider;
//   [P] the preview's paired methods via tools/layout_xlate.py (dtor and OnEndObjectUse: 0x0/0x18/0x1C/0x20/0x70-0x78
//       unchanged) and its inline accessors GetBaseRotX/Y/Z (lwc1 0x70/0x74/0x78);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptObjRider 0x80: mpAnimBankInfo 0x1C, mpSoundBankInfo 0x20, mBaseTransform 0x30,
//       mBaseRot* 0x70-0x78; cScriptEntity; cScriptObject).

#include "layout.h"
#include "bx/math3d.h"

struct cScriptPlayerEntity;
struct cScriptPlayerMain;
struct tScriptAnimBankInfo;
struct tScriptSoundBankInfo;

struct cRider;

struct cScriptObjRider {
    // --- cScriptEntity (scripter/scriptentity.h, 0xC)
    void** mVtbl;                         // 0x00 [R] cScriptObjRider_vtbl (ctor and dtor); [U] IsMatchingEntity calls slot 6
                                          //      (GetAttachedRider)
    int mDBID;                            // 0x04 [O]; [R] cScriptEntity ctor -1
    cScriptPlayerEntity* mpEntity;        // 0x08 [O]; [R] cScriptEntity ctor 0; [U] OnStartObjectUse reads the bound entity's
                                          //      actor / script headers through it
    // --- cScriptObject (0x10; ctor cScriptObject_cScriptObject(self + 0xC, 3))
    int mObjType;                         // 0x0C [O]; [R] cScriptObject ctor stores 3 (rider)
    int mObjID;                           // 0x10 [O]; [R] cScriptObject ctor -1
    int mInUse;                           // 0x14 [O]; [R] cScriptObject ctor 0; [U] cScriptObject_OnStart/OnEndObjectUse
                                          //      (called on self + 0xC)
    void** mScriptObjectVtbl;             // 0x18 [R] D_00482018 (ctor and dtor); the dtor runs cScriptObject__cScriptObject on +0xC
    // --- cScriptObjRider
    tScriptAnimBankInfo* mpAnimBankInfo;  // 0x1C [O]; [R] ctor 0; [U] OnStartObjectUse binds the script's anim bank
                                          //      (cSSXScriptEngine_GetAnimationBankInfo), OnEndObjectUse clears it;
                                          //      ScriptAnimIDToGameAnimInfo reads its bank (+0x4) and ID table (+0x10)
    tScriptSoundBankInfo* mpSoundBankInfo; // 0x20 [O]; [R] ctor 0; [U] OnStartObjectUse binds it, OnEndObjectUse clears it;
                                          //      ScriptSoundIDToGameSoundInfo reads its bank (+0x4)
    char unk_24[0xC];                     // 0x24 alignment gap before the 16-byte-aligned matrix
    t4Matrix mBaseTransform;              // 0x30 [O]; [R] ctor copies t4mat_Identity (lqc2/sqc2); [U] OnStartObjectUse builds it,
                                          //      OnEndObjectUse resets it; cScriptObjRider_GetBaseTransform returns &mBaseTransform
    float mBaseRotX;                      // 0x70 [O]; [P] GetBaseRotX (lwc1); [R] ctor 0 (sw $0); [U] OnStartObjectUse (lwc1/swc1)
    float mBaseRotY;                      // 0x74 [O]; [P] GetBaseRotY (lwc1); [R] ctor 0; [U] OnStartObjectUse
    float mBaseRotZ;                      // 0x78 [O]; [P] GetBaseRotZ (lwc1); [R] ctor 0; [U] OnStartObjectUse
    char unk_7C[0x4];                     // 0x7C padding to the 16-byte class alignment

    // Methods bound to their retail symbols by asm labels.
    void OnStartObjectUse() __asm__("cScriptObjRider_OnStartObjectUse");
    void OnEndObjectUse() __asm__("cScriptObjRider_OnEndObjectUse");
    int IsMatchingEntity(cScriptPlayerEntity* msg, int type) __asm__("cScriptObjRider_IsMatchingEntity");
    void ScriptAnimIDToGameAnimInfo(int scriptAnimId, void* gameAnimIndex, void* gameAnimBankId)
        __asm__("cScriptObjRider_ScriptAnimIDToGameAnimInfo__FPviT0T0");
    void ScriptSoundIDToGameSoundInfo(int scriptSoundId, void* gameSoundIndex, void* gameSoundBankId)
        __asm__("cScriptObjRider_ScriptSoundIDToGameSoundInfo__FPviT0T0");
    bool IsSuitableRole(cRider* rider, int id, cScriptPlayerMain* playerMain) __asm__("cScriptObjRider_IsSuitableRole");
    float GetBaseRotX() __asm__("cScriptObjRider_GetBaseRotX");
    float GetBaseRotY() __asm__("cScriptObjRider_GetBaseRotY");
    float GetBaseRotZ() __asm__("cScriptObjRider_GetBaseRotZ");
};

CHECK_OFFSET(cScriptObjRider, mVtbl, 0x00);
CHECK_OFFSET(cScriptObjRider, mDBID, 0x04);
CHECK_OFFSET(cScriptObjRider, mpEntity, 0x08);
CHECK_OFFSET(cScriptObjRider, mObjType, 0x0C);
CHECK_OFFSET(cScriptObjRider, mObjID, 0x10);
CHECK_OFFSET(cScriptObjRider, mInUse, 0x14);
CHECK_OFFSET(cScriptObjRider, mScriptObjectVtbl, 0x18);
CHECK_OFFSET(cScriptObjRider, mpAnimBankInfo, 0x1C);
CHECK_OFFSET(cScriptObjRider, mpSoundBankInfo, 0x20);
CHECK_OFFSET(cScriptObjRider, unk_24, 0x24);
CHECK_OFFSET(cScriptObjRider, mBaseTransform, 0x30);
CHECK_OFFSET(cScriptObjRider, mBaseRotX, 0x70);
CHECK_OFFSET(cScriptObjRider, mBaseRotY, 0x74);
CHECK_OFFSET(cScriptObjRider, mBaseRotZ, 0x78);
CHECK_OFFSET(cScriptObjRider, unk_7C, 0x7C);
CHECK_SIZE(cScriptObjRider, 0x80);

#endif /* SCRIPTER_SCRIPTOBJRIDER_H */

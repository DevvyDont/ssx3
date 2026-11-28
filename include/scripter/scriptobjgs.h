#ifndef SCRIPTER_SCRIPTOBJGS_H
#define SCRIPTER_SCRIPTOBJGS_H

// cScriptObjGS: the NIS script object (object type 4) for the game-state manager track of a cut-scene: it is its own
// single script entity (GetNumScriptEntities 1, GetScriptEntity returns the object), takes InSeq entities of type 6
// (IsMatchingEntity) and on a key event fetches the entity's game-state frame (GetSSXGameStateManagerFrame) and
// ignores it (a release build: On Tour only asserts on it). One instance: cSSXScriptEngine::mObjectGameState (+0x910,
// scripter/ssxscriptengine.h), built by the engine ctor with cScriptObjGS_cScriptObjGS; the engine dtor inlines its
// dtor (stores both vtables, runs cScriptObject__cScriptObject on +0x91C).
// EA's header is d:/ssx/code/source/scripter/scriptobjgs.h (July 2003 preview assert strings; scriptobjgs.cpp too).
// Retail methods: scripter/ssxscriptengine (0x0027BFD0 ctor .. 0x0027C050 OnInSeqKeyEvent) and
// scripter/all_in_one_scripter (0x002848D8 dtor, 0x00284940 GetNumScriptEntities, 0x00284948 GetScriptEntity).
//
// Size 0x1C: [R] cSSXScriptEngine's ctor builds it at +0x910 and its next member (mObjectEvent) is at +0x92C
// = 0x910 + 0x1C; the ctor writes 0x0-0x18. On Tour: 0x1C. No members of its own.
// Bases, in retail order (the ctor: cScriptEntity_cScriptEntity(self), cScriptObject_cScriptObject(self + 0xC, 4)), as in
// cScriptObjEvent (scripter/scriptobjevent.h):
//   cScriptEntity (scripter/scriptentity.h, 0xC) at 0x00: vptr 0x00 = cScriptObjGS_vtbl (0x004822E0, 5 slots:
//                 1/2 cScriptEntity_OnInSeqLock / OnInSeqRelease, 3 cScriptObjGS_OnInSeqKeyEvent,
//                 4 cScriptEntity_OnInSeqUpdate, 5 cScriptObjGS_IsMatchingEntity);
//   cScriptObject (scripter/scriptobj.h, 0x10) at 0x0C: vptr 0x18 = cScriptObjGS_cScriptObject_vtbl (_vt$12cScriptObjGS$13cScriptObject:
//                 1 dtor cScriptObjGS__cScriptObjGS, 2/3 cScriptObject_Pause/ResumeObjScript, 4 cScriptObject_
//                 OnStartObjectUse__FPv, 5 cScriptObjGS_OnEndObjectUse, 6 GetNumScriptEntities, 7 GetScriptEntity;
//                 slots 1 and 5-7 carry delta -0xC, so they get the whole object).
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline. Evidence tags:
//   [R] retail ctor cScriptObjGS_cScriptObjGS and dtor cScriptObjGS__cScriptObjGS;
//   [P] tools/layout_xlate.py pairs the preview's dtor with retail's (0x0 and 0x18 unchanged); the preview keeps
//       GetScriptEntity / GetNumScriptEntities out of line, with no field access;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptObjGS : cScriptObject, 0x1C; cScriptEntity; cScriptObject).

#include "layout.h"

struct cScriptPlayerEntity;

struct cScriptObjGS {
    // --- cScriptEntity (scripter/scriptentity.h, 0xC; ctor cScriptEntity_cScriptEntity(self))
    void** mVtbl;                       // 0x00 [R] cScriptObjGS_vtbl (ctor and dtor)
    int mDBID;                          // 0x04 [O] cScriptEntity::mDBID; [R] cScriptEntity ctor -1
    cScriptPlayerEntity* mpEntity;      // 0x08 [O] cScriptEntity::mpEntity; [R] cScriptEntity ctor 0
    // --- cScriptObject (scripter/scriptobj.h, 0x10; ctor cScriptObject_cScriptObject(self + 0xC, 4))
    int mObjType;                       // 0x0C [O]; [R] cScriptObject ctor stores 4 (game state)
    int mObjID;                         // 0x10 [O]; [R] cScriptObject ctor -1
    int mInUse;                         // 0x14 [O]; [R] cScriptObject ctor 0; [U] OnEndObjectUse calls
                                        //      cScriptObject_OnEndObjectUse on self + 0xC
    void** mScriptObjectVtbl;           // 0x18 [R] cScriptObjGS_cScriptObject_vtbl (ctor and dtor); [P] the paired dtor stores it at 0x18 in
                                        //      both builds; the dtor runs cScriptObject__cScriptObject on +0xC

    // Methods bound to their retail symbols by asm labels.
    void OnEndObjectUse() __asm__("cScriptObjGS_OnEndObjectUse");
    bool IsMatchingEntity(cScriptPlayerEntity* entity, int type) __asm__("cScriptObjGS_IsMatchingEntity");
    void OnInSeqKeyEvent(void* entity) __asm__("cScriptObjGS_OnInSeqKeyEvent");
};

CHECK_OFFSET(cScriptObjGS, mVtbl, 0x00);
CHECK_OFFSET(cScriptObjGS, mDBID, 0x04);
CHECK_OFFSET(cScriptObjGS, mpEntity, 0x08);
CHECK_OFFSET(cScriptObjGS, mObjType, 0x0C);
CHECK_OFFSET(cScriptObjGS, mObjID, 0x10);
CHECK_OFFSET(cScriptObjGS, mInUse, 0x14);
CHECK_OFFSET(cScriptObjGS, mScriptObjectVtbl, 0x18);
CHECK_SIZE(cScriptObjGS, 0x1C);

#endif /* SCRIPTER_SCRIPTOBJGS_H */

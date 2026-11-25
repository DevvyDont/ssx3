#ifndef SCRIPTER_SCRIPTOBJ_H
#define SCRIPTER_SCRIPTOBJ_H

// cScriptObject: the base of every NIS script object, i.e. anything a cut-scene script can take over. The object type
// (mObjType) says which: 0 cScriptObjCameraManager, 1 cScriptObjCamera, 2 cScriptObjTransition (and cTransitionMan),
// 3 cScriptObjRider (every cRider), 4 cScriptObjGS, 5 cScriptObjEvent (the values the derived ctors pass).
// InitScriptObject registers the object with the script database (cScriptDatabaseObject, AddObject = its vtable slot 2)
// and keeps the ID it gets; DeInitScriptObject removes it (slot 3). While scripts use the object, mInUse counts them
// (OnStartObjectUse / OnEndObjectUse); PauseObjScript / ResumeObjScript pause or resume the script of every entity the
// object reports (GetNumScriptEntities / GetScriptEntity, pure virtuals here).
// EA's header is d:/ssx/code/source/scripter/scriptobj.h (July 2003 preview assert strings; scriptobj.cpp too).
// Retail methods: scripter/bxscriptengine (0x00283200 ctor .. 0x00283440 OnEndObjectUse) and three getters in
// scripter/all_in_one_scripter (0x002847C8 GetObjectType, 0x002847D0 GetObjectID, 0x002847D8 IsInUse).
//
// Size 0x10: never allocated on its own; [R] cScriptObject_cScriptObject__FPvi writes 0x0-0xC, and every derived class
// places its next member at +0x10 of this part (cScriptObjTransition_cScriptObjTransition: mpCameraManager at 0x10;
// cScriptObjRider / cScriptObjEvent / cScriptObjGS, which hold it at +0xC: their own members from 0x1C;
// cScriptObjCameraManager, which holds it at +0x0: its cScriptEntity part at 0x10). [O] On Tour: 0x10, same offsets.
// No base class. g++ 2.95 puts the vptr after the data of the class that declares the virtuals: 0xC.
// Retail vtable cScriptObject_vtbl (0x00482558, 7 slots): 1 dtor cScriptObject__cScriptObject, 2 PauseObjScript,
// 3 ResumeObjScript, 4 OnStartObjectUse__FPv, 5 OnEndObjectUse__FPv, 6 GetNumScriptEntities and 7 GetScriptEntity
// (both __pure_virtual, func_00411530).
// The derived headers write these four members inline under the same names: scripter/scriptobjcammgr.h (+0x0),
// scripter/scriptobjrider.h (+0xC), scripter/scriptobjevent.h (+0xC), scripter/scriptobjtransition.h (+0x0),
// visualfx/transitionman.h (+0x10; mScriptVtbl there), scripter/scriptobjcamera.h (+0x0).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cScriptObject_cScriptObject__FPvi and dtor cScriptObject__cScriptObject;
//   [P] the July 2003 preview's inline accessors (tools/layout_xlate.py): GetObjectID lw 0x4 ("seen": the paired
//       dtor shows 0x4 and 0xC unchanged), GetObjectType lw 0x0;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cScriptObject 0x10: mObjType 0x0, mObjID 0x4, mInUse 0x8).

#include "layout.h"

// mObjID of an object the script database has not registered (name ours). [R] the ctor and DeInitScriptObject store
// it; the dtor deinitialises an object whose mObjID is neither this nor 0.
enum { kScriptObjectID_None = -1 };

// cScriptObject::mObjType: which kind of script object this is (names ours; On Tour keeps a plain int). [R] the
// values the derived ctors pass to cScriptObject's ctor (see the comment at the top).
enum eScriptObjectType {
    keScriptObjectType_CameraManager = 0,
    keScriptObjectType_Camera = 1,
    keScriptObjectType_Transition = 2,
    keScriptObjectType_Rider = 3,
    keScriptObjectType_GameState = 4,
    keScriptObjectType_Event = 5
};

struct cScriptObject {
    int mObjType;    // 0x0 [O] (int there); [P] GetObjectType (lw 0x0); [R] the ctor stores its objType argument;
                     //     [U] cScriptObject_GetObjectType
    int mObjID;      // 0x4 [O]; [P] GetObjectID (lw 0x4); [R] ctor -1 (not registered); [U] InitScriptObject stores the
                     //     database's AddObject result, DeInitScriptObject passes it to RemoveObject and sets -1, the dtor
                     //     deinitialises when it is neither -1 nor 0; cScriptObject_GetObjectID
    int mInUse;      // 0x8 [O] (int there: a use count); [R] ctor 0; [U] OnStartObjectUse +1, OnEndObjectUse -1,
                     //     cScriptObject_IsInUse (> 0)
    void** mVtbl;    // 0xC [R] cScriptObject_vtbl (ctor and dtor); [P] the paired dtor stores it at 0xC in both builds;
                     //     [U] PauseObjScript / ResumeObjScript call slots 6 and 7 through it

    // Methods ([O] On Tour's names), each bound to its retail symbol by an asm label. The virtual ones are declared as
    // plain members, since the layout above is flat. OnStartObjectUse / OnEndObjectUse return the new use count (On
    // Tour: void, with an unused playerID).
    cScriptObject(int objType) __asm__("cScriptObject_cScriptObject__FPvi");
    void InitScriptObject() __asm__("cScriptObject_InitScriptObject");
    void DeInitScriptObject() __asm__("cScriptObject_DeInitScriptObject");
    void PauseObjScript() __asm__("cScriptObject_PauseObjScript");
    void ResumeObjScript() __asm__("cScriptObject_ResumeObjScript");
    int OnStartObjectUse() __asm__("cScriptObject_OnStartObjectUse__FPv");
    int OnEndObjectUse() __asm__("cScriptObject_OnEndObjectUse__FPv");
    int GetObjectID() __asm__("cScriptObject_GetObjectID");
    bool IsInUse() __asm__("cScriptObject_IsInUse");
};

CHECK_OFFSET(cScriptObject, mObjType, 0x0);
CHECK_OFFSET(cScriptObject, mObjID, 0x4);
CHECK_OFFSET(cScriptObject, mInUse, 0x8);
CHECK_OFFSET(cScriptObject, mVtbl, 0xC);
CHECK_SIZE(cScriptObject, 0x10);

#endif /* SCRIPTER_SCRIPTOBJ_H */

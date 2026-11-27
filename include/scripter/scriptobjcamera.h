#ifndef SCRIPTER_SCRIPTOBJCAMERA_H
#define SCRIPTER_SCRIPTOBJCAMERA_H

// cScriptObjCamera: a script-driven camera object: a camera entity (mpCameraEntity), its weight in the camera
// manager's blend (mWeight), the algorithm that moves it (mpCameraAlgorithm), the player it serves and its base
// transform / rotation. The camera manager points at two (On Tour cScriptObjCameraManager::mpCameraObjects[2]).
// Methods in scripter/ssxscriptengine and scripter/all_in_one_scripter.
//
// Size 0x80 (On Tour's; retail's methods reach mViewIndex at size - 4). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "bx/math3d.h"

struct cScriptAlgorithm;
struct cScriptEntityCamera;
struct cScriptObjCameraManager;

struct cScriptObjCamera {
    // --- cScriptObject (scripter/scriptobj.h, 0xC of data; [R] the ctor runs cScriptObject_cScriptObject(self, 1))
    int mObjType;                             // 0x000 [O] cScriptObject; [R] cScriptObject ctor stores 1 (camera)
    int mObjID;                               // 0x004 [O] cScriptObject; [R] cScriptObject ctor -1; [U]
                                              //       cScriptObjCameraManager_RemoveCameraObject matches cameras by it
    int mInUse;                               // 0x008 [O] cScriptObject; [R] cScriptObject ctor 0
    void** mVtbl;                             // 0x00C [R] ctor = cScriptObjCamera_vtbl; [U] SetCameraEntity's virtual call
    cScriptObjCameraManager* mpCameraManager; // 0x010 [O]
    cScriptEntityCamera* mpCameraEntity;      // 0x014 [O]
    float mWeight;                            // 0x018 [O]
    cScriptAlgorithm* mpCameraAlgorithm;      // 0x01C [O]
    int mPlayerID;                            // 0x020 [O]
    char unk_024[0xC];                        // 0x024 padding
    t4Matrix mBaseTransform;                  // 0x030 [O]
    float mBaseRotX;                          // 0x070 [O]
    float mBaseRotY;                          // 0x074 [O]
    float mBaseRotZ;                          // 0x078 [O]
    int mViewIndex;                           // 0x07C [O]

    // Methods bound to their retail symbols by asm labels.
    void SetCameraManager(cScriptObjCameraManager* val) __asm__("cScriptObjCamera_SetCameraManager__FPvi");
    void SetCameraEntity(void* anim) __asm__("cScriptObjCamera_SetCameraEntity");
    void SetCameraWeight(float val) __asm__("cScriptObjCamera_SetCameraWeight__FPvf");
    int IsInUseByPlayer(int val) __asm__("cScriptObjCamera_IsInUseByPlayer");
    void PauseObjScript() __asm__("cScriptObjCamera_PauseObjScript");
    void ResumeObjScript() __asm__("cScriptObjCamera_ResumeObjScript");
    void OnStartObjectUse() __asm__("cScriptObjCamera_OnStartObjectUse");
    void OnEndObjectUse() __asm__("cScriptObjCamera_OnEndObjectUse");
    cScriptEntityCamera* GetCameraEntity() __asm__("cScriptObjCamera_GetCameraEntity");
    float GetCameraWeight() __asm__("cScriptObjCamera_GetCameraWeight");
    float GetBaseRotX() __asm__("cScriptObjCamera_GetBaseRotX");
    float GetBaseRotY() __asm__("cScriptObjCamera_GetBaseRotY");
    float GetBaseRotZ() __asm__("cScriptObjCamera_GetBaseRotZ");
    int GetViewIndex() __asm__("cScriptObjCamera_GetViewIndex");
    cScriptObjCameraManager* GetCameraManager() __asm__("cScriptObjCamera_GetCameraManager");
    void SetCameraAlgorithm(cScriptAlgorithm* algorithm) __asm__("cScriptObjCamera_SetCameraAlgorithm");
    cScriptAlgorithm* GetCameraAlgorithm() __asm__("cScriptObjCamera_GetCameraAlgorithm");
};

CHECK_OFFSET(cScriptObjCamera, mObjType, 0x000);
CHECK_OFFSET(cScriptObjCamera, mObjID, 0x004);
CHECK_OFFSET(cScriptObjCamera, mInUse, 0x008);
CHECK_OFFSET(cScriptObjCamera, mVtbl, 0x00C);
CHECK_OFFSET(cScriptObjCamera, mpCameraManager, 0x010);
CHECK_OFFSET(cScriptObjCamera, mpCameraEntity, 0x014);
CHECK_OFFSET(cScriptObjCamera, mWeight, 0x018);
CHECK_OFFSET(cScriptObjCamera, mpCameraAlgorithm, 0x01C);
CHECK_OFFSET(cScriptObjCamera, mPlayerID, 0x020);
CHECK_OFFSET(cScriptObjCamera, mBaseTransform, 0x030);
CHECK_OFFSET(cScriptObjCamera, mBaseRotX, 0x070);
CHECK_OFFSET(cScriptObjCamera, mBaseRotY, 0x074);
CHECK_OFFSET(cScriptObjCamera, mBaseRotZ, 0x078);
CHECK_OFFSET(cScriptObjCamera, mViewIndex, 0x07C);
CHECK_SIZE(cScriptObjCamera, 0x80);

#endif

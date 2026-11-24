#include "common.h"
#include "vtable.h"
#include "camera/cameracontroller.h"
#include "camera/race/racecontroller.h"
#include "camera/cameraalgolist.h"
#include "camera/cameraalgorithms.h"
//100%
INCLUDE_ASM("camera/cameracontroller", cCameraController_cCameraController);
#ifdef SKIP_ASM
extern void* cCameraController_vtbl[];
extern char str_Algorithm_List[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cCameraAlgoList_cCameraAlgoList(void* self);

/**
 * Constructs the camera controller base for a camera, with an empty algorithm list and no input.
 *
 * Stores cCameraController's vtable at +0x14, allocates the 8-byte "Algorithm List" (AlgorithmList, +0x8),
 * stores @p camera at +0x10 and zeroes Input (+0xC), Type (+0x0) and BlendType (+0x4).
 *
 * @param camera  the owning cCamera
 * @return @p self
 */
extern "C" void* cCameraController_cCameraController(cCameraController* self, int camera)
{
    self->mVtbl = cCameraController_vtbl;
    self->AlgorithmList = (cCameraAlgoList*)cCameraAlgoList_cCameraAlgoList(cMemMan_alloc(8, str_Algorithm_List, 0x20000000, 0));
    self->Camera = (cCamera*)camera;
    self->Input = 0;
    self->Type = 0;
    self->BlendType = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cCameraController__cCameraController);
#ifdef SKIP_ASM
extern void* cCameraController_vtbl[];
extern "C" void cCameraAlgoList__cCameraAlgoList(void* p, int mode);
void operator_delete(int* ptr);

/**
 * Destroys a camera controller: deletes its algorithm list, then frees the controller when asked to.
 *
 * Restores cCameraController's vtable at +0x14 and destroys AlgorithmList (+0x8) with flags 3 when it is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cCameraController__cCameraController(cCameraController* self, int flags)
{
    self->mVtbl = cCameraController_vtbl;
    void* algoList = self->AlgorithmList;
    if (algoList != 0) {
        cCameraAlgoList__cCameraAlgoList(algoList, 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cCameraController_snap);
#ifdef SKIP_ASM
void* cCamera_snapResultAlgorithm(void* self);
extern "C" void cSplashManager_CameraSnapped(void* p);

// PORT: the unit declares cCameraController_snap(void* self), but the body takes a
// second argument (callers pass it in $5); the real body is bound by asm label.
void cCameraController_snap_impl(void* self, int a) __asm__("cCameraController_snap");

/**
 * Snaps every camera algorithm in the controller's list to a subject, then snaps the camera's result algorithm.
 *
 * Calls each algorithm's snap virtual (slot 3) with @p subjectID, walking AlgorithmList, then
 * cCamera_snapResultAlgorithm__FPv (On Tour cCamera::snapResultAlgorithm) and cSplashManager_CameraSnapped (On Tour
 * cSplashManager::CameraSnapped) on Camera (+0x10).
 *
 * @param subjectID  the subject (rider index) to snap to
 */
void cCameraController_snap_impl(void* vself, int subjectID)
{
    cCameraController* self = (cCameraController*)vself;
    for (cCameraAlgoListNode* n = self->AlgorithmList->Head; n != 0; n = n->Next)
    {
        n->Algorithm->snap(subjectID);
    }
    cCamera_snapResultAlgorithm(self->Camera);
    cSplashManager_CameraSnapped(self->Camera);
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cCameraController_snapCurrent);
#ifdef SKIP_ASM
struct sCamCtrlNodeB {
    void* obj;           // 0x0
    char pad_0x04[0x10];
    sCamCtrlNodeB* next; // 0x14
};

struct sCamCtrlVE0 {
    short delta;
    short index;
    void (*fn)(void*);
};

void* cCamera_snapResultAlgorithm(void* self);
extern "C" void cSplashManager_CameraSnapped(void* p);

/**
 * Snaps every camera algorithm in the controller's list to its current subject, then snaps the result algorithm.
 *
 * Calls each algorithm's snapCurrent virtual (slot 4), walking AlgorithmList (+0x8), then cCamera_snapResultAlgorithm__FPv
 * (On Tour cCamera::snapResultAlgorithm) and cSplashManager_CameraSnapped (On Tour cSplashManager::CameraSnapped) on Camera (+0x10).
 */
extern "C" void cCameraController_snapCurrent(cCameraController* self)
{
    for (sCamCtrlNodeB* n = *(sCamCtrlNodeB**)self->AlgorithmList; n != 0; n = n->next)
    {
        cCameraAlgorithm* obj = (cCameraAlgorithm*)n->obj;
        obj->snapCurrent();
    }
    cCamera_snapResultAlgorithm(self->Camera);
    cSplashManager_CameraSnapped(self->Camera);
}
#endif

extern "C" void* cCameraAlgoList_removeAll(int);

//100%
INCLUDE_ASM("camera/cameracontroller", cCameraController_clearAlgorithms__FPv);
#ifdef SKIP_ASM
/**
 * Removes every algorithm from the controller's algorithm list, deleting the ones the list owns.
 *
 * Calls cCameraAlgoList_removeAll on the list that AlgorithmList (+0x8) points to.
 *
 * @return no meaningful value (cCameraAlgoList_removeAll returns nothing)
 */
void* cCameraController_clearAlgorithms(void* vself)
{
    cCameraController* self = (cCameraController*)vself;
    return cCameraAlgoList_removeAll((int)self->AlgorithmList);
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_cManualCameraController);
#ifdef SKIP_ASM
extern void* cManualCameraController_vtbl[];
extern char str_Manual_Cam_Algo[];
extern char str_Editor_Cam_Algo[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cCameraController_cCameraController(cCameraController* self, int a1);
extern "C" void* cManualCameraAlgorithm_cManualCameraAlgorithm(void* self);
extern "C" void* cEditorCameraAlgorithm_cEditorCameraAlgorithm(void* self);
extern "C" void cManualCameraController_setCurrentAlgorithmType(cManualCameraController* self, int msg);

/**
 * Constructs a manual camera controller with its own manual and editor camera algorithms, starting in the
 * manual one.
 *
 * Runs the cCameraController constructor, installs cManualCameraController's vtable (+0x14), sets Type to 1,
 * allocates the 0x40-byte "Manual Cam Algo" (+0x18) and "Editor Cam Algo" (+0x1C) and selects algorithm type
 * 0x51 with cManualCameraController_setCurrentAlgorithmType().
 *
 * @param camera  the owning cCamera
 * @return @p self
 */
extern "C" void* cManualCameraController_cManualCameraController(cManualCameraController* self, int camera)
{
    cCameraController_cCameraController((cCameraController*)self, camera);
    self->mVtbl = cManualCameraController_vtbl;
    self->Type = 1;
    self->ManualAlgo = (cManualCameraAlgorithm*)cManualCameraAlgorithm_cManualCameraAlgorithm(cMemMan_alloc(0x40, str_Manual_Cam_Algo, 0x20000000, 0));
    self->EditorAlgo = (cEditorCameraAlgorithm*)cEditorCameraAlgorithm_cEditorCameraAlgorithm(cMemMan_alloc(0x40, str_Editor_Cam_Algo, 0x20000000, 0));
    cManualCameraController_setCurrentAlgorithmType(self, 81);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cManualCameraController__cManualCameraController);
#ifdef SKIP_ASM
extern void* cManualCameraController_vtbl[];
extern "C" void cCameraController__cCameraController(cCameraController* self, int flags);

/**
 * Destroys the manual camera controller: deletes its manual and editor algorithms, then runs the base destructor.
 *
 * Restores cManualCameraController_vtbl, deletes ManualAlgo and EditorAlgo (their virtual destructor, slot 1, flags 3)
 * when set, and passes @p flags on to cCameraController__cCameraController.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cManualCameraController__cManualCameraController(cManualCameraController* self, int flags)
{
    self->mVtbl = cManualCameraController_vtbl;
    cCameraAlgorithm* manualAlgo = self->ManualAlgo;
    if (manualAlgo != 0) {
        delete manualAlgo;
    }
    cCameraAlgorithm* editorAlgo = self->EditorAlgo;
    if (editorAlgo != 0) {
        delete editorAlgo;
    }
    cCameraController__cCameraController((cCameraController*)self, flags);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_snap);
#ifdef SKIP_ASM
extern "C" void cCameraController_snap(void* self);

/**
 * Snaps the camera to a subject: the base controller's snap, then the manual algorithm's.
 *
 * Calls cCameraController_snap (On Tour cCameraController::snap) and the snap virtual (slot 3) of ManualAlgo.
 *
 * @param subjectID  the subject to snap to (On Tour subjectID), passed only to the manual algorithm
 */
extern "C" void cManualCameraController_snap(cManualCameraController* self, int subjectID)
{
    cCameraController_snap(self);
    self->ManualAlgo->snap(subjectID);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_snapCurrent);
#ifdef SKIP_ASM
class cCamCtrlVObj {
public:
    char pad[0x10];
    // vptr at 0x10 (g++ 2.95 places it after the class's own data)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void snapCurrent();
    virtual void update();
};

extern "C" void cCameraController_snapCurrent(cCameraController* self);

/**
 * Snaps the camera to its current subject: the base controller's snapCurrent, then the manual algorithm's.
 *
 * Calls cCameraController_snapCurrent (On Tour cCameraController::snapCurrent) and the snapCurrent virtual (slot 4) of ManualAlgo
 * (+0x18).
 */
extern "C" void cManualCameraController_snapCurrent(cManualCameraController* self)
{
    cCameraController_snapCurrent((cCameraController*)self);
    ((cCamCtrlVObj*)self->ManualAlgo)->snapCurrent();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_addAlgorithm);
#ifdef SKIP_ASM
extern "C" void cCameraAlgoList_insert(void* list, int a, int b, float w);

/**
 * Makes an algorithm the manual controller's only one: clears the list, then inserts it with a full blend.
 *
 * Calls cCameraController_clearAlgorithms__FPv to empty AlgorithmList, then cCameraAlgoList_insert(AlgorithmList (+0x8), @p algo, @p deleteAlgorithmOnDestruction, 1.0f).
 *
 * @param algo  the camera algorithm to use
 * @param deleteAlgorithmOnDestruction  nonzero when the list should delete the algorithm when it removes it
 */
extern "C" void cManualCameraController_addAlgorithm(cManualCameraController* self, int algo, int deleteAlgorithmOnDestruction)
{
    cCameraController_clearAlgorithms(self);
    cCameraAlgoList_insert(self->AlgorithmList, algo, deleteAlgorithmOnDestruction, 1.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_setCurrentAlgorithmType);
#ifdef SKIP_ASM
extern "C" void cManualCameraController_addAlgorithm(cManualCameraController* self, int a, int b);

/**
 * Switches the manual camera controller to its editor or its manual algorithm.
 *
 * Type 0x50 selects the editor algorithm (+0x1C) and 0x51 the manual one (+0x18); other types are ignored.
 * cManualCameraController_addAlgorithm empties the algorithm list and inserts the chosen algorithm at weight 1.0.
 *
 * @param msg  the eCamAlgoType to select (On Tour type): 0x50 editor, 0x51 manual
 */
extern "C" void cManualCameraController_setCurrentAlgorithmType(cManualCameraController* self, int msg)
{
    switch (msg) {
    case 80:
        cManualCameraController_addAlgorithm(self, (int)self->EditorAlgo, 0);
        break;
    case 81:
        cManualCameraController_addAlgorithm(self, (int)self->ManualAlgo, 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_getCurrentAlgorithmType);
#ifdef SKIP_ASM
/**
 * Returns the AlgorithmType of the first algorithm in the controller's AlgorithmList.
 *
 * @param manualController  the controller (a cManualCameraController)
 * @return the algorithm type (an eCamAlgoType)
 */
extern "C" int cManualCameraController_getCurrentAlgorithmType(cManualCameraController* manualController) {
    return manualController->AlgorithmList->Head->Algorithm->AlgorithmType;
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_update);
#ifdef SKIP_ASM
/**
 * Updates the manual camera: runs the update virtual (slot 5) of the first algorithm in its AlgorithmList (+0x8).
 */
extern "C" void cManualCameraController_update(cManualCameraController* self)
{
    void** algoList = (void**)self->AlgorithmList;
    cCamCtrlVObj* obj = *(cCamCtrlVObj**)*algoList;
    obj->update();
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cChaseCameraController_shouldDoExtraGroundTest);
#ifdef SKIP_ASM
/**
 * Returns whether the chase camera is in a replay camera other than types 93 (0x5D) and 60 (0x3C).
 *
 * True when CamSwitchRecord.IsInReplay is set and CamSwitchRecord.LatestReplayType is neither 93 nor 60.
 *
 * @return 1 or 0
 */
extern "C" int cChaseCameraController_shouldDoExtraGroundTest(cChaseCameraController* self)
{
    int replayCamActive = 0;
    if (self->CamSwitchRecord.IsInReplay) {
        int replayType = self->CamSwitchRecord.LatestReplayType;
        if (replayType != 93) {
            replayCamActive = replayType != 0x3C;
        }
    }
    return replayCamActive;
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cChaseCameraController_createChaseAlgorithmCut);
#ifdef SKIP_ASM
extern "C" void* cChaseCameraController_createChaseAlgorithmBlend(void*, int, int, float);

/**
 * Switches the chase camera to an algorithm type with an immediate cut (no blend).
 *
 * Calls cChaseCameraController_createChaseAlgorithmBlend(self, @p type, @p action, 1.0f): a blend rate of 1 switches at
 * once.
 *
 * @param type    the camera algorithm type (On Tour eCamAlgoType)
 * @param action  the camera-trigger action that asked for the switch, if any (On Tour cCTAction*)
 * @return no meaningful value (cChaseCameraController_createChaseAlgorithmBlend returns nothing)
 */
extern "C" void* cChaseCameraController_createChaseAlgorithmCut(cChaseCameraController* self, int type, int action)
{
    return cChaseCameraController_createChaseAlgorithmBlend(self, type, action, 1.0f);
}
#endif


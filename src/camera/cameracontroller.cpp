#include "common.h"

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
extern "C" void* cCameraController_cCameraController(void* self, int camera)
{
    *(void***)((char*)self + 0x14) = cCameraController_vtbl;
    *(void**)((char*)self + 0x8) = cCameraAlgoList_cCameraAlgoList(cMemMan_alloc(8, str_Algorithm_List, 0x20000000, 0));
    *(int*)((char*)self + 0x10) = camera;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    return self;
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
extern "C" void cCameraController__cCameraController(void* self, int flags)
{
    *(void***)((char*)self + 0x14) = cCameraController_vtbl;
    void* algoList = *(void**)((char*)self + 0x8);
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
struct sCamCtrlNode {
    void* obj;          // 0x0
    char pad_0x04[0x10];
    sCamCtrlNode* next; // 0x14
};

struct sCamCtrlVE {
    short delta;
    short index;
    void (*fn)(void*, int);
};

void* cCamera_snapResultAlgorithm(void* self);
extern "C" void cSplashManager_CameraSnapped(void* p);

// PORT: the unit declares cCameraController_snap(void* self), but the body takes a
// second argument (callers pass it in $5); the real body is bound by asm label.
void cCameraController_snap_impl(void* self, int a) __asm__("cCameraController_snap");

void cCameraController_snap_impl(void* self, int a)
{
    for (sCamCtrlNode* n = **(sCamCtrlNode***)((char*)self + 0x8); n != 0; n = n->next)
    {
        void* obj = n->obj;
        sCamCtrlVE* vt = *(sCamCtrlVE**)((char*)obj + 0x10);
        vt[3].fn((char*)obj + vt[3].delta, a);
    }
    cCamera_snapResultAlgorithm(*(void**)((char*)self + 0x10));
    cSplashManager_CameraSnapped(*(void**)((char*)self + 0x10));
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

extern "C" void cCameraController_snapCurrent(void* self)
{
    for (sCamCtrlNodeB* n = **(sCamCtrlNodeB***)((char*)self + 0x8); n != 0; n = n->next)
    {
        void* obj = n->obj;
        sCamCtrlVE0* vt = *(sCamCtrlVE0**)((char*)obj + 0x10);
        vt[4].fn((char*)obj + vt[4].delta);
    }
    cCamera_snapResultAlgorithm(*(void**)((char*)self + 0x10));
    cSplashManager_CameraSnapped(*(void**)((char*)self + 0x10));
}
#endif

extern "C" void* cCameraAlgoList_removeAll(int);

//100%
INCLUDE_ASM("camera/cameracontroller", cCameraController_clearAlgorithms__FPv);
#ifdef SKIP_ASM
void* cCameraController_clearAlgorithms(void* self)
{
    return cCameraAlgoList_removeAll(*(int*)((char*)self + 0x8));
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_cManualCameraController);
#ifdef SKIP_ASM
extern void* cManualCameraController_vtbl[];
extern char str_Manual_Cam_Algo[];
extern char str_Editor_Cam_Algo[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cCameraController_cCameraController(void* self, int a1);
extern "C" void* cManualCameraAlgorithm_cManualCameraAlgorithm(void* self);
extern "C" void* cEditorCameraAlgorithm_cEditorCameraAlgorithm(void* self);
extern "C" void cManualCameraController_setCurrentAlgorithmType(void* self, int msg);

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
extern "C" void* cManualCameraController_cManualCameraController(void* self, int camera)
{
    cCameraController_cCameraController(self, camera);
    *(void***)((char*)self + 0x14) = cManualCameraController_vtbl;
    *(int*)((char*)self + 0x0) = 1;
    *(void**)((char*)self + 0x18) = cManualCameraAlgorithm_cManualCameraAlgorithm(cMemMan_alloc(0x40, str_Manual_Cam_Algo, 0x20000000, 0));
    *(void**)((char*)self + 0x1C) = cEditorCameraAlgorithm_cEditorCameraAlgorithm(cMemMan_alloc(0x40, str_Editor_Cam_Algo, 0x20000000, 0));
    cManualCameraController_setCurrentAlgorithmType(self, 0x51);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cManualCameraController__cManualCameraController);
#ifdef SKIP_ASM
extern void* cManualCameraController_vtbl[];
extern "C" void cCameraController__cCameraController(void* self, int flags);

struct sCamCtrlVEDtor {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Destroys the manual camera controller: deletes its manual and editor algorithms, then runs the base destructor.
 *
 * Restores cManualCameraController_vtbl (+0x14), deletes ManualAlgo (+0x18) and EditorAlgo (+0x1C) through their
 * destructor virtual (slot 1, flags 3) when set, and passes @p flags on to cCameraController__cCameraController.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cManualCameraController__cManualCameraController(void* self, int flags)
{
    *(void***)((char*)self + 0x14) = cManualCameraController_vtbl;
    char* manualAlgo = *(char**)((char*)self + 0x18);
    if (manualAlgo != 0) {
        sCamCtrlVEDtor* vt = *(sCamCtrlVEDtor**)(manualAlgo + 0x10);
        vt[1].fn(manualAlgo + vt[1].delta, 3);
    }
    char* editorAlgo = *(char**)((char*)self + 0x1C);
    if (editorAlgo != 0) {
        sCamCtrlVEDtor* vt = *(sCamCtrlVEDtor**)(editorAlgo + 0x10);
        vt[1].fn(editorAlgo + vt[1].delta, 3);
    }
    cCameraController__cCameraController(self, flags);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_snap);
#ifdef SKIP_ASM
class cCamCtrlVObj_0015CE98 {
public:
    char pad[0x10];
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03(int a);
};

extern "C" void cCameraController_snap(void* self);

/**
 * Snaps the camera to a subject: the base controller's snap, then the manual algorithm's.
 *
 * Calls cCameraController_snap (On Tour cCameraController::snap) and the snap virtual (slot 3) of ManualAlgo (+0x18).
 *
 * @param subjectID  the subject to snap to (On Tour subjectID), passed only to the manual algorithm
 */
extern "C" void cManualCameraController_snap(void* self, int subjectID)
{
    cCameraController_snap(self);
    (*(cCamCtrlVObj_0015CE98**)((char*)self + 0x18))->v03(subjectID);
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
    virtual void v04();
    virtual void v05();
};

extern "C" void cCameraController_snapCurrent(void* self);

/**
 * Snaps the camera to its current subject: the base controller's snapCurrent, then the manual algorithm's.
 *
 * Calls cCameraController_snapCurrent (On Tour cCameraController::snapCurrent) and the snapCurrent virtual (slot 4) of ManualAlgo
 * (+0x18).
 */
extern "C" void cManualCameraController_snapCurrent(void* self)
{
    cCameraController_snapCurrent(self);
    (*(cCamCtrlVObj**)((char*)self + 0x18))->v04();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_addAlgorithm);
#ifdef SKIP_ASM
extern "C" void cCameraAlgoList_insert(void* list, int a, int b, float w);

extern "C" void cManualCameraController_addAlgorithm(void* self, int a, int b)
{
    cCameraController_clearAlgorithms(self);
    cCameraAlgoList_insert(*(void**)((char*)self + 0x8), a, b, 1.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_setCurrentAlgorithmType);
#ifdef SKIP_ASM
extern "C" void cManualCameraController_addAlgorithm(void* self, int a, int b);

/**
 * Switches the manual camera controller to its editor or its manual algorithm.
 *
 * Type 0x50 selects the editor algorithm (+0x1C) and 0x51 the manual one (+0x18); other types are ignored.
 * cManualCameraController_addAlgorithm empties the algorithm list and inserts the chosen algorithm at weight 1.0.
 *
 * @param msg  the eCamAlgoType to select (On Tour type): 0x50 editor, 0x51 manual
 */
extern "C" void cManualCameraController_setCurrentAlgorithmType(void* self, int msg)
{
    switch (msg) {
    case 0x50:
        cManualCameraController_addAlgorithm(self, *(int*)((char*)self + 0x1C), 0);
        break;
    case 0x51:
        cManualCameraController_addAlgorithm(self, *(int*)((char*)self + 0x18), 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", func_0015CFD0);
#ifdef SKIP_ASM
extern "C" int func_0015CFD0(void *manualController) {
    return (*(int *)((char*)(**(*(void ****)((char*)(manualController) + (8)))) + (0xC)));
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", cManualCameraController_update);
#ifdef SKIP_ASM
/**
 * Updates the manual camera: runs the update virtual (slot 5) of the first algorithm in its AlgorithmList (+0x8).
 */
extern "C" void cManualCameraController_update(void* self)
{
    void** algoList = *(void***)((char*)self + 0x8);
    cCamCtrlVObj* obj = *(cCamCtrlVObj**)*algoList;
    obj->v05();
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", func_0015D020);
#ifdef SKIP_ASM
extern "C" int func_0015D020(void* self)
{
    int replayCamActive = 0;
    if (*(int*)((char*)self + 0x30) & 2) {
        int replayType = *(int*)((char*)self + 0x2C);
        if (replayType != 0x5D) {
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

extern "C" void* cChaseCameraController_createChaseAlgorithmCut(void* self, int a1, int a2)
{
    return cChaseCameraController_createChaseAlgorithmBlend(self, a1, a2, 1.0f);
}
#endif


#include "common.h"
#include "vtable.h"
#include "scripter/scriptentitycam.h"
#include "bx/math3d.h"
#include "camera/script/scriptalgorithms.h"
#include "camera/script/scriptcontroller.h"
#include "camera/camera.h"
#include "scripter/scriptobjcamera.h"
#include "camera/trigger/cameratriggerlist.h"
#include "scripter/ssxscriptengine.h"
#include "camera/cameraalgolist.h"
#include "main/gameviewman.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/rider.h"
#include "animation/animmodel.h"
#include "inseq/scriptplayerentity.h"

// t4Vector with the constructors the original's vector class has: an empty one and one from four floats (the script
// algorithms build their positions and look-at points with it).
struct t4VectorCtor : public t4Vector {
    t4VectorCtor() {}
    t4VectorCtor(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_cScriptCameraController);
#ifdef SKIP_ASM
extern "C" void* cCameraController_cCameraController(void* self, void* arg);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cScriptObjCamera_cScriptObjCamera(void* mem, void* script);
extern "C" void cScriptObject_InitScriptObject(void* p);
// PORT: operator_new__FUi takes (size, tag, flags, align) here.
void* operator_new_K2(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern void* cScriptCameraController_vtbl[];
extern char str_ScriptObjCamArray[];
extern char str_ScriptObjCam[];

/**
 * Constructs the script camera controller with its two ScriptObjCam objects.
 *
 * The second argument is really the view index (cCamera_cCamera passes it; On Tour: viewIndex): it goes to
 * each ScriptObjCam (cScriptObjCamera_cScriptObjCamera, 0x80 bytes), which is then set up by cScriptObject_InitScriptObject. Type 3, not active
 * (+0x18 = 0).
 *
 * @param script  the view index (misnamed)
 * @param camera  the owning cCamera
 * @return self
 */
extern "C" void* cScriptCameraController_cScriptCameraController(cScriptCameraController* self, void* script, cCamera* camera)
{
    int i;
    cCameraController_cCameraController(self, camera);
    self->mVtbl = cScriptCameraController_vtbl;
    self->Type = 3;
    self->ScriptObj = (cScriptObjCamera**)((void**)operator_new_K2(8, str_ScriptObjCamArray, 0, 0));
    for (i = 0; i < 2; i++) {
        void** slot = (void**)((i << 2) + (int)self->ScriptObj);
        *slot = cScriptObjCamera_cScriptObjCamera(cMemMan_alloc(0x80, str_ScriptObjCam, 0, 0), script);
        cScriptObject_InitScriptObject(self->ScriptObj[i]);
    }
    self->IsActive = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController__cScriptCameraController);
#ifdef SKIP_ASM
extern "C" void cScriptObject_DeInitScriptObject(void* p);
extern "C" void cCameraController__cCameraController(void* self, int flags);
void cMemMan_free(void* p);
extern void* cScriptCameraController_vtbl[];

class cScrCamScriptK2 {
public:
    char pad_0x00[0xC];
    // vptr at 0xC
    virtual ~cScrCamScriptK2();
};

/**
 * Destroys a script camera controller: releases its two script objects, then runs the cCameraController
 * destructor.
 *
 * Stores the vtable at +0x14; each of the two script objects in the array at +0x1C (On Tour ScriptObj) is
 * de-initialised with cScriptObject_DeInitScriptObject() and deleted, then the array itself is freed.
 *
 * @param flags  g++ destructor flags, passed to cCameraController__cCameraController()
 */
extern "C" void cScriptCameraController__cScriptCameraController(cScriptCameraController* self, int flags)
{
    int i;
    self->mVtbl = cScriptCameraController_vtbl;
    for (i = 0; i < 2; i++) {
        cScriptObject_DeInitScriptObject(((cScrCamScriptK2*)self->ScriptObj[i]));
        delete ((cScrCamScriptK2*)self->ScriptObj[i]);
    }
    if (self->ScriptObj != 0)
        cMemMan_free(self->ScriptObj);
    cCameraController__cCameraController(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_addCamera);
#ifdef SKIP_ASM
int cScriptPlayerEntity_GetType(void* self);
void cScriptAlgorithm_setScriptObj(void* self, int val);
extern "C" void* cScriptManualAlgorithm_cScriptManualAlgorithm(cScriptManualAlgorithm* self);
extern "C" void* cScriptPointAlgorithm_cScriptPointAlgorithm(cScriptPointAlgorithm* self);
extern "C" void* cScriptTargetAlgorithm_cScriptTargetAlgorithm(cScriptTargetAlgorithm* self);
extern "C" void* cScriptRelativeAlgorithm_cScriptRelativeAlgorithm(cScriptRelativeAlgorithm* self);
// PORT: cScriptCameraController_addAlgorithm is defined with (self, algo); this caller passes a third int argument.
extern "C" void cScriptCameraController_addAlgorithm_3(void* self, void* algo, int a) __asm__("cScriptCameraController_addAlgorithm");
extern char str_Manual_Script_Camera_Algo[];
extern char str_Target_Script_Camera_Algo[];
extern char str_Subject_Script_Camera_Algo[];
extern char str_Algorithmic_Script_Camera_Algo[];

/**
 * Creates the camera algorithm for a script camera object and adds it to the controller.
 *
 * The kind (eScriptCameraKind, cScriptPlayerEntity_GetType on the object's data) picks a manual, target, subject or
 * algorithmic script camera algorithm; other kinds are ignored. The algorithm is bound to the object, stored at its
 * +0x1C, added as owned (cScriptCameraController_addAlgorithm, probably addAlgorithm) and the controller becomes active (+0x18 = 1).
 *
 * @param cam  the script camera object (cScriptObjCamera)
 */
void cScriptCameraController::addCamera(cScriptObjCamera* cam)
{
    void* ctl;
    switch (cScriptPlayerEntity_GetType(cam->mpCameraEntity->mpEntity))
    {
    case kScriptCam_Manual:
        ctl = cScriptManualAlgorithm_cScriptManualAlgorithm((cScriptManualAlgorithm*)cMemMan_alloc(176, str_Manual_Script_Camera_Algo, 0x20000000, 0));
        break;
    case kScriptCam_Target:
        ctl = cScriptPointAlgorithm_cScriptPointAlgorithm((cScriptPointAlgorithm*)cMemMan_alloc(176, str_Target_Script_Camera_Algo, 0x20000000, 0));
        break;
    case kScriptCam_Subject:
        ctl = cScriptTargetAlgorithm_cScriptTargetAlgorithm((cScriptTargetAlgorithm*)cMemMan_alloc(160, str_Subject_Script_Camera_Algo, 0x20000000, 0));
        break;
    case kScriptCam_Algorithmic:
        ctl = cScriptRelativeAlgorithm_cScriptRelativeAlgorithm((cScriptRelativeAlgorithm*)cMemMan_alloc(160, str_Algorithmic_Script_Camera_Algo, 0x20000000, 0));
        break;
    default:
        return;
    }
    // PORT: the camera pointer is passed through an int parameter.
    cScriptAlgorithm_setScriptObj(ctl, (int)cam);
    cam->mpCameraAlgorithm = (cScriptAlgorithm*)ctl;
    cScriptCameraController_addAlgorithm_3(((char*)this), ctl, 1);
    IsActive = 1;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_removeCamera);
#ifdef SKIP_ASM
extern "C" void cCameraAlgoList_remove(cCameraAlgoList* list, cCameraAlgoListNode* node);

/**
 * Removes a script camera's algorithm from the controller.
 *
 * Only when the flag at +0x18 is set (cScriptCameraController_addCamera sets it): finds the node of AlgorithmList whose
 * algorithm is the one stored at +0x1C of @p algo, removes it with cCameraAlgoList_remove, and clears the flag when the list
 * is then empty.
 *
 * @param algo  the script camera object (On Tour's cScriptObjCamera)
 */
void cScriptCameraController::removeCamera(cScriptObjCamera* algo)
{
    if (IsActive != 0) {
        cCameraAlgoList* list = AlgorithmList;
        for (cCameraAlgoListNode* n = list->Head; n != 0; n = n->Next) {
            cCameraAlgorithm* target = algo->mpCameraAlgorithm;
            if (n->Algorithm == target) {
                cCameraAlgoList_remove(list, n);
                break;
            }
        }
        if (list->Length <= 0)
            IsActive = 0;
    }
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_addAlgorithm);
#ifdef SKIP_ASM
extern "C" void cCameraAlgoList_insert(void*, void*, float);

/**
 * Adds a camera algorithm to the script controller's list with a full (1.0) blend.
 *
 * Calls cCameraAlgoList_insert on AlgorithmList (+0x8). The caller's third argument (On Tour's
 * deleteAlgorithmOnDestruction flag) is not declared here: it reaches the insert untouched in $6.
 *
 * @param algo  the camera algorithm to add
 */
void cScriptCameraController::addAlgorithm(void* algo)
{
    cCameraAlgoList_insert(AlgorithmList, algo, 1.0f);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The list nodes hold the algorithm as a cScrCamAlgoK2; reach cScriptAlgorithm_getScriptObj through this alias.
cScriptObjCamera* cScriptAlgorithm_getScriptObj_K2(void* self) __asm__("cScriptAlgorithm_getScriptObj__FPv");

class cScrCamAlgoK2 {
public:
    char pad_0x00[0x10];
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void update();
};

struct sScrAlgoNodeK2 {
    cScrCamAlgoK2* algo;    // 0x0
    float t;                // 0x4
    float pad_0x08;
    float weight;           // 0xC
    int pad_0x10;
    sScrAlgoNodeK2* next;   // 0x14
};

/**
 * Updates the blend weight of each script camera algorithm and runs it.
 *
 * For each node the weight is smoothstep(t) = 3t^2 - 2t^3 (capped at 1) of the time at +0x18 of the object
 * cScriptAlgorithm_getScriptObj returns for the algorithm; then virtual slot 5 runs the algorithm.
 */
void cScriptCameraController::update()
{
    sScrAlgoNodeK2* n = *(sScrAlgoNodeK2**)AlgorithmList;
    while (n != 0) {
        float progress = cScriptAlgorithm_getScriptObj_K2(n->algo)->mWeight;
        float progressSq = progress * progress;
        float smoothed = progressSq * 3.0f - progressSq * 2.0f * progress;
        n->t = progress;
        if (smoothed > 1.0f)
            smoothed = 1.0f;
        n->weight = smoothed;
        n->algo->update();
        n = n->next;
    }
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptAlgorithm_cScriptAlgorithm);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* cCameraAlgorithm_vtbl[];
extern void* cScriptAlgorithm_vtbl[];
extern t4Vector t4vect_Origin;
extern "C" float cCameraAlgorithm_getDefaultView(void* self);

/**
 * Constructs the generic script camera algorithm base: default view, script controller of the first view.
 *
 * Installs the cCameraAlgorithm vtable (cCameraAlgorithm_vtbl) to read cCameraAlgorithm_getDefaultView, then the
 * cScriptAlgorithm vtable (cScriptAlgorithm_vtbl) at +0x10; sets the type (+0xC) to 0x55 (On Tour SCRIPT_GENERIC), view,
 * near 10 and far 30000, clears the script object (+0x14), takes the controller (+0x18) from the first view
 * camera's script controller (+0xAC) and sets the target (+0x20) to t4vect_Origin.
 *
 * @param object  the object being constructed (this)
 * @return the object
 */
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(cScriptAlgorithm* object)
{
    VPTR_OF(cCameraAlgorithm, object) = cCameraAlgorithm_vtbl;
    float fov = cCameraAlgorithm_getDefaultView(object);
    object->AlgorithmType = 85;
    VPTR_OF(cCameraAlgorithm, object) = cScriptAlgorithm_vtbl;
    object->View = fov;
    object->Near = 10.0f;
    object->Far = 30000.0f;
    object->ScriptObj = 0;
    cGame* game = SSXApp->Game;
    cGameViewMan* viewMan = game->ViewMan;
    cCamera* camera = viewMan->PlayerCamera[0];
    object->Controller = camera->ScriptController;
    object->Target = t4vect_Origin;
    return object;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptAlgorithm_setScriptObj__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the script camera object an algorithm follows.
 *
 * Stores @p val at +0x14 (On Tour cScriptAlgorithm::ScriptObj).
 *
 * @param val  the script camera object (On Tour cScriptObjCamera*)
 */
void cScriptAlgorithm::setScriptObj(int val)
{
    ScriptObj = (cScriptObjCamera*)val;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptAlgorithm_getScriptObj__FPv);
#ifdef SKIP_ASM
/**
 * Returns a script camera algorithm's script object (+0x14, set by cScriptAlgorithm_setScriptObj__FPvi).
 *
 * @return the script object (On Tour's cScriptObjCamera*); cScriptCameraController_update reads its time at +0x18
 */
cScriptObjCamera* cScriptAlgorithm::getScriptObj()
{
    return ScriptObj;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", cScriptManualAlgorithm_cScriptManualAlgorithm);
#ifdef SKIP_ASM

extern void* cScriptManualAlgorithm_vtbl[];
extern t4Vector t4vect_Origin;
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(cScriptAlgorithm* self);
extern "C" void* cCameraJitterer_cCameraJitterer(void* self);

/**
 * Constructs a manual script camera algorithm (camera at a scripted position and orientation).
 *
 * Runs cScriptAlgorithm_cScriptAlgorithm, installs cScriptManualAlgorithm_vtbl at +0x10, constructs the jitterer at +0x74 with
 * cCameraJitterer_cCameraJitterer, sets the position (+0x30) to t4vect_Origin and the type (+0xC) to 0x56
 * (On Tour SCRIPT_MANUAL), and zeroes roll, heading and pitch (+0x40..+0x48).
 *
 * @return @p self
 */
extern "C" void* cScriptManualAlgorithm_cScriptManualAlgorithm(cScriptManualAlgorithm* self)
{
    cScriptAlgorithm_cScriptAlgorithm((cScriptAlgorithm*)self);
    VPTR_OF(cCameraAlgorithm, self) = cScriptManualAlgorithm_vtbl;
    cCameraJitterer_cCameraJitterer(&self->Jitterer);
    self->Position = t4vect_Origin;
    self->AlgorithmType = 86;
    self->Heading = 0.0f;
    self->Pitch = 0.0f;
    self->Roll = 0.0f;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptManualAlgorithm_snap);
#ifdef SKIP_ASM
class cScriptCtlVirt {
public:
    char pad[0x10];
    // vptr lands at 0x10 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void update();
};

/**
 * Snaps the camera into place by running a full update at once.
 *
 * Calls the object's virtual update() (vtable slot 5, cScriptManualAlgorithm_update).
 */
void cScriptManualAlgorithm::snap(int)
{
    ((cScriptCtlVirt*)this)->update();
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptManualAlgorithm_snapCurrent);
#ifdef SKIP_ASM
class cScriptCtlVirt2 {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void snap(int);
};

/**
 * Snaps the camera onto its current subject: calls the virtual snap() with subject 0.
 *
 * Calls vtable slot 3 (cScriptManualAlgorithm_snap) with 0 as the subject id (cCameraAlgorithm::snap's argument in On
 * Tour), which the script algorithm's snap ignores.
 */
void cScriptManualAlgorithm::snapCurrent()
{
    ((cScriptCtlVirt2*)this)->snap(0);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptManualAlgorithm_update);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; needs a C fallback off-PS2.
float* GetSSXCameraManualHeader(void* cam);
extern "C" void GetSSXCameraManualFrame(void* cam, void* out);
extern "C" float func_0031C228(float x);
extern "C" void cCameraJitterer_updatePositionalJitter(void* self, float v);
extern "C" void cCameraJitterer_updateRollJitter(void* self, float v);

static inline t4VectorCtor mtxMulVec_169570(char* m, const t4Vector& v)
{
    t4VectorCtor r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

static inline t4VectorCtor Sub_169570(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_169570(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

static inline float Atan2_169570(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}


static inline float Length_169570(const t2Vector& v)
{
    return Sqrt_169570(v.x * v.x + v.y * v.y);
}



/**
 * Updates the manual script camera: position and look-at point straight from the script, transformed to the world.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (GetSSXCameraManualHeader, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and GetSSXCameraManualFrame reads this frame's channel
 * values into FrameInfo (+0x4C, On Tour tSSXFrameCameraManual). FrameInfo angles are in degrees (converted with *
 * 0.0174533).
 *
 * View (+0x0) becomes fov (+0x68); Position (+0x30) is mBaseTransform (ScriptObj +0x30) times (posX, posY, posZ, 1)
 * (+0x4C); Roll (+0x40) is roll (+0x64) plus mBaseRotX (ScriptObj +0x70); Target (+0x20) is mBaseTransform times
 * (targetX, targetY, targetZ, 1) (+0x58). Heading (+0x44) and Pitch (+0x48) are the yaw and elevation of Target -
 * Position. Finally the jitterer (+0x74) is updated with jitterFocus (+0x6C) and jitterRoll (+0x70).
 *
 */
void cScriptManualAlgorithm::update()
{
    cScriptManualAlgorithm* self = this;
    char* cam = (char*)ScriptObj->mpCameraEntity->mpEntity;
    float* header = GetSSXCameraManualHeader(cam);
    char* baseTransform = (char*)&ScriptObj->mBaseTransform;
    self->Near = header[0];
    self->Far = header[1];
    GetSSXCameraManualFrame(cam, &self->FrameInfo);
    self->View = self->FrameInfo.fov * 0.01745329424738884f;
    self->Position = t4VectorCtor(self->FrameInfo.posX, self->FrameInfo.posY, self->FrameInfo.posZ, 1.0f);
    self->Position = mtxMulVec_169570(baseTransform, self->Position);
    self->Roll = self->FrameInfo.roll * 0.01745329424738884f + ScriptObj->mBaseRotX;
    self->Target = mtxMulVec_169570(baseTransform, t4VectorCtor(self->FrameInfo.targetX, self->FrameInfo.targetY,
                                                                 self->FrameInfo.targetZ, 1.0f));
    t4VectorCtor delta = Sub_169570(self->Target, self->Position);
    self->Heading = Atan2_169570(delta.y, delta.x);
    float horizontalLen = Length_169570(t2Vector(delta.x, delta.y));
    self->Pitch = Atan2_169570(delta.z, horizontalLen);
    cCameraJitterer_updatePositionalJitter(&Jitterer, self->FrameInfo.jitterFocus);
    cCameraJitterer_updateRollJitter(&Jitterer, self->FrameInfo.jitterRoll);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptManualAlgorithm_getPose);
#ifdef SKIP_ASM



extern t4Matrix t4mat_Identity_9828 __asm__("t4mat_Identity");
extern t4Vector t4vect_XAxis_9828 __asm__("t4vect_XAxis");
extern t4Vector t4vect_YAxis_9828 __asm__("t4vect_YAxis");
extern t4Vector t4vect_ZAxis_9828 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" tPose tPose_tPose_9828(t4Matrix* m) __asm__("tPose_tPose");
extern "C" t4Vector cCameraJitterer_getPositionalJitter_9828(char* p, float a, float b) __asm__("cCameraJitterer_getPositionalJitter");
extern "C" float cCameraJitterer_getRollJitter(void* self);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_9828(t4Matrix* dst, const t4Matrix* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void vu0MulMtx_9828(t4Matrix* dst, const t4Matrix* a, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_9828(t4Matrix* m, const t4Vector& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_9828(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline void SinCos_9828(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void RotAxis_9828(t4Matrix* m, const t4Vector& axis, float angle)
{
    t4Matrix r;
    float s;
    float c;
    float one = 1.0f;
    SinCos_9828(&s, &c, angle);
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    ((float (*)[4])&r.row[0])[0][0] = tx * x + c;
    ((float (*)[4])&r.row[0])[1][0] = tx * y + sz;
    ((float (*)[4])&r.row[0])[2][0] = tx * z - sy;
    ((float (*)[4])&r.row[0])[0][1] = ty * x - sz;
    ((float (*)[4])&r.row[0])[1][1] = ty * y + c;
    ((float (*)[4])&r.row[0])[2][1] = ty * z + sx;
    ((float (*)[4])&r.row[0])[0][2] = tz * x + sy;
    ((float (*)[4])&r.row[0])[1][2] = tz * y - sx;
    ((float (*)[4])&r.row[0])[2][2] = tz * z + c;
    ((float (*)[4])&r.row[0])[3][0] = 0.0f;
    ((float (*)[4])&r.row[0])[3][1] = 0.0f;
    ((float (*)[4])&r.row[0])[3][2] = 0.0f;
    ((float (*)[4])&r.row[0])[0][3] = 0.0f;
    ((float (*)[4])&r.row[0])[1][3] = 0.0f;
    ((float (*)[4])&r.row[0])[2][3] = 0.0f;
    ((float (*)[4])&r.row[0])[3][3] = one;
    vu0MulMtx_9828(m, m, &r);
}

/**
 * Returns the camera pose: Position plus jitter, turned by Roll, Pitch and Heading.
 *
 * Starts from t4mat_Identity translated to Position (+0x30) plus the jitterer's (+0x74) positional jitter for Heading
 * (+0x44) and Pitch (+0x48) (cCameraJitterer_getPositionalJitter, On Tour cCameraJitterer::getPositionalJitter), then rotates it about
 * t4vect_XAxis by Roll (+0x40) plus cCameraJitterer_getRollJitter, about t4vect_YAxis by -Pitch and about t4vect_ZAxis
 * by Heading, and splits the matrix into a position and a rotation quaternion with tPose_tPose. The four script
 * algorithms share this body; only the jitterer offset differs.
 *
 * @param algorithm  the cScriptManualAlgorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" tPose cScriptManualAlgorithm_getPose(cScriptManualAlgorithm* algorithm)
{
    t4Matrix mat;
    vu0CopyMtx_9828(&mat, &t4mat_Identity_9828);
    vu0TransMtx_9828(&mat, Add_9828(*(&algorithm->Position), cCameraJitterer_getPositionalJitter_9828((char*)&algorithm->Jitterer, algorithm->Heading, algorithm->Pitch)));
    RotAxis_9828(&mat, t4vect_XAxis_9828, algorithm->Roll + cCameraJitterer_getRollJitter(&algorithm->Jitterer));
    RotAxis_9828(&mat, t4vect_YAxis_9828, -algorithm->Pitch);
    RotAxis_9828(&mat, t4vect_ZAxis_9828, algorithm->Heading);
    return tPose_tPose_9828(&mat);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", cScriptPointAlgorithm_cScriptPointAlgorithm);
#ifdef SKIP_ASM
extern void* cScriptPointAlgorithm_vtbl[];
extern t4Vector t4vect_Origin;
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(cScriptAlgorithm* self);
extern "C" void* cCameraJitterer_cCameraJitterer(void* self);

/**
 * Constructs a point script camera algorithm (camera at a scripted point).
 *
 * Runs cScriptAlgorithm_cScriptAlgorithm, installs cScriptPointAlgorithm_vtbl at +0x10, constructs the jitterer at +0x74, sets the position (+0x30)
 * to t4vect_Origin and the type (+0xC) to 0x57 (On Tour SCRIPT_POINT), and zeroes roll, heading and pitch.
 *
 * @return @p self
 */
extern "C" void* cScriptPointAlgorithm_cScriptPointAlgorithm(cScriptPointAlgorithm* self)
{
    cScriptAlgorithm_cScriptAlgorithm((cScriptAlgorithm*)self);
    VPTR_OF(cCameraAlgorithm, self) = cScriptPointAlgorithm_vtbl;
    cCameraJitterer_cCameraJitterer(&self->Jitterer);
    self->Position = t4vect_Origin;
    self->AlgorithmType = 87;
    self->Roll = 0.0f;
    self->Heading = 0.0f;
    self->Pitch = 0.0f;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptPointAlgorithm_snap);
#ifdef SKIP_ASM
/**
 * Snaps the camera into place by running a full update at once.
 *
 * Calls the object's virtual update() (vtable slot 5, cScriptPointAlgorithm_update).
 */
void cScriptPointAlgorithm::snap(int)
{
    ((cScriptCtlVirt*)this)->update();
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptPointAlgorithm_snapCurrent);
#ifdef SKIP_ASM
/**
 * Snaps the camera onto its current subject: calls the virtual snap() with subject 0.
 *
 * Calls vtable slot 3 (cScriptPointAlgorithm_snap) with 0 as the subject id (cCameraAlgorithm::snap's argument in On
 * Tour), which the script algorithm's snap ignores.
 */
void cScriptPointAlgorithm::snapCurrent()
{
    ((cScriptCtlVirt2*)this)->snap(0);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptPointAlgorithm_update);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; needs a C fallback off-PS2.
float* GetSSXCameraTargetHeader(void* cam);
extern "C" void GetSSXCameraTargetFrame(void* cam, void* out);
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cCameraJitterer_updatePositionalJitter(void* self, float v);
extern "C" void cCameraJitterer_updateRollJitter(void* self, float v);

static inline t4VectorCtor mtxMulVec_169DB0(char* m, const t4Vector& v)
{
    t4VectorCtor r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

static inline t4VectorCtor Scale_169DB0(const t4Vector& v, float s)
{
    t4VectorCtor r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline t4VectorCtor Sub_169DB0(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}



/**
 * Updates the point script camera: orbits a scripted point at a distance, heading and pitch.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (GetSSXCameraTargetHeader, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and GetSSXCameraTargetFrame reads this frame's channel
 * values into FrameInfo (+0x4C, On Tour tSSXFrameCameraTarget). FrameInfo angles are in degrees (converted with *
 * 0.0174533).
 *
 * Near and Far are stored a second time after the fill. View (+0x0) becomes fov (+0x68); Heading (+0x44) is heading
 * (+0x60) plus mBaseRotZ (ScriptObj +0x78), Pitch (+0x48) is pitch (+0x5C) plus mBaseRotY (+0x74) and Roll (+0x40) is
 * roll (+0x64) plus mBaseRotX (+0x70). Target (+0x20) is mBaseTransform (ScriptObj +0x30) times (targetX, targetY,
 * targetZ, 1) (+0x4C), and Position (+0x30) is Target minus the direction (cos Pitch cos Heading, cos Pitch sin
 * Heading, sin Pitch) times targetDistance (+0x58). Finally the jitterer (+0x74) is updated with jitterFocus (+0x6C)
 * and jitterRoll (+0x70).
 *
 */
void cScriptPointAlgorithm::update()
{
    cScriptPointAlgorithm* self = this;
    char* cam = (char*)ScriptObj->mpCameraEntity->mpEntity;
    float* header = GetSSXCameraTargetHeader(cam);
    char* baseTransform = (char*)&ScriptObj->mBaseTransform;
    self->Near = header[0];
    self->Far = header[1];
    GetSSXCameraTargetFrame(cam, &self->FrameInfo);
    self->Near = header[0];
    self->Far = header[1];
    self->View = self->FrameInfo.fov * 0.01745329424738884f;
    cScriptObjCamera* scriptObj = ScriptObj;
    self->Heading = self->FrameInfo.heading * 0.01745329424738884f + scriptObj->mBaseRotZ;
    self->Pitch = self->FrameInfo.pitch * 0.01745329424738884f + scriptObj->mBaseRotY;
    self->Roll = self->FrameInfo.roll * 0.01745329424738884f + scriptObj->mBaseRotX;
    self->Target = mtxMulVec_169DB0(baseTransform, t4VectorCtor(self->FrameInfo.targetX, self->FrameInfo.targetY,
                                                                 self->FrameInfo.targetZ, 1.0f));
    float sinHead, cosHead, sinPitch, cosPitch;
    fSinCos(&sinHead, &cosHead, self->Heading);
    fSinCos(&sinPitch, &cosPitch, self->Pitch);
    t4VectorCtor look(cosPitch * cosHead, cosPitch * sinHead, sinPitch, 0.0f);
    self->Position = Sub_169DB0(self->Target, Scale_169DB0(look, self->FrameInfo.targetDistance));
    cCameraJitterer_updatePositionalJitter(&Jitterer, self->FrameInfo.jitterFocus);
    cCameraJitterer_updateRollJitter(&Jitterer, self->FrameInfo.jitterRoll);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptPointAlgorithm_getPose);
#ifdef SKIP_ASM



extern t4Matrix t4mat_Identity_9F88 __asm__("t4mat_Identity");
extern t4Vector t4vect_XAxis_9F88 __asm__("t4vect_XAxis");
extern t4Vector t4vect_YAxis_9F88 __asm__("t4vect_YAxis");
extern t4Vector t4vect_ZAxis_9F88 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" tPose tPose_tPose_9F88(t4Matrix* m) __asm__("tPose_tPose");
extern "C" t4Vector cCameraJitterer_getPositionalJitter_9F88(char* p, float a, float b) __asm__("cCameraJitterer_getPositionalJitter");
extern "C" float cCameraJitterer_getRollJitter(void* self);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_9F88(t4Matrix* dst, const t4Matrix* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void vu0MulMtx_9F88(t4Matrix* dst, const t4Matrix* a, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_9F88(t4Matrix* m, const t4Vector& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_9F88(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline void SinCos_9F88(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void RotAxis_9F88(t4Matrix* m, const t4Vector& axis, float angle)
{
    t4Matrix r;
    float s;
    float c;
    float one = 1.0f;
    SinCos_9F88(&s, &c, angle);
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    ((float (*)[4])&r.row[0])[0][0] = tx * x + c;
    ((float (*)[4])&r.row[0])[1][0] = tx * y + sz;
    ((float (*)[4])&r.row[0])[2][0] = tx * z - sy;
    ((float (*)[4])&r.row[0])[0][1] = ty * x - sz;
    ((float (*)[4])&r.row[0])[1][1] = ty * y + c;
    ((float (*)[4])&r.row[0])[2][1] = ty * z + sx;
    ((float (*)[4])&r.row[0])[0][2] = tz * x + sy;
    ((float (*)[4])&r.row[0])[1][2] = tz * y - sx;
    ((float (*)[4])&r.row[0])[2][2] = tz * z + c;
    ((float (*)[4])&r.row[0])[3][0] = 0.0f;
    ((float (*)[4])&r.row[0])[3][1] = 0.0f;
    ((float (*)[4])&r.row[0])[3][2] = 0.0f;
    ((float (*)[4])&r.row[0])[0][3] = 0.0f;
    ((float (*)[4])&r.row[0])[1][3] = 0.0f;
    ((float (*)[4])&r.row[0])[2][3] = 0.0f;
    ((float (*)[4])&r.row[0])[3][3] = one;
    vu0MulMtx_9F88(m, m, &r);
}

/**
 * Returns the camera pose: Position plus jitter, turned by Roll, Pitch and Heading.
 *
 * Starts from t4mat_Identity translated to Position (+0x30) plus the jitterer's (+0x74) positional jitter for Heading
 * (+0x44) and Pitch (+0x48) (cCameraJitterer_getPositionalJitter, On Tour cCameraJitterer::getPositionalJitter), then rotates it about
 * t4vect_XAxis by Roll (+0x40) plus cCameraJitterer_getRollJitter, about t4vect_YAxis by -Pitch and about t4vect_ZAxis
 * by Heading, and splits the matrix into a position and a rotation quaternion with tPose_tPose. The four script
 * algorithms share this body; only the jitterer offset differs.
 *
 * @param algorithm  the cScriptPointAlgorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" tPose cScriptPointAlgorithm_getPose(cScriptPointAlgorithm* algorithm)
{
    t4Matrix mat;
    vu0CopyMtx_9F88(&mat, &t4mat_Identity_9F88);
    vu0TransMtx_9F88(&mat, Add_9F88(*(&algorithm->Position), cCameraJitterer_getPositionalJitter_9F88((char*)&algorithm->Jitterer, algorithm->Heading, algorithm->Pitch)));
    RotAxis_9F88(&mat, t4vect_XAxis_9F88, algorithm->Roll + cCameraJitterer_getRollJitter(&algorithm->Jitterer));
    RotAxis_9F88(&mat, t4vect_YAxis_9F88, -algorithm->Pitch);
    RotAxis_9F88(&mat, t4vect_ZAxis_9F88, algorithm->Heading);
    return tPose_tPose_9F88(&mat);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", cScriptTargetAlgorithm_cScriptTargetAlgorithm);
#ifdef SKIP_ASM
extern void* cScriptTargetAlgorithm_vtbl[];
extern t4Vector t4vect_Origin;
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(cScriptAlgorithm* self);
extern "C" void* cCameraJitterer_cCameraJitterer(void* self);

/**
 * Constructs a target script camera algorithm (camera looking at a scripted subject).
 *
 * Runs cScriptAlgorithm_cScriptAlgorithm, installs cScriptTargetAlgorithm_vtbl at +0x10, constructs the jitterer at +0x70, sets the position (+0x30)
 * to t4vect_Origin and the type (+0xC) to 0x58 (On Tour SCRIPT_TARGET), and zeroes roll, heading and pitch.
 *
 * @return @p self
 */
extern "C" void* cScriptTargetAlgorithm_cScriptTargetAlgorithm(cScriptTargetAlgorithm* self)
{
    cScriptAlgorithm_cScriptAlgorithm((cScriptAlgorithm*)self);
    VPTR_OF(cCameraAlgorithm, self) = cScriptTargetAlgorithm_vtbl;
    cCameraJitterer_cCameraJitterer(&self->Jitterer);
    self->Position = t4vect_Origin;
    self->AlgorithmType = 88;
    self->Roll = 0.0f;
    self->Heading = 0.0f;
    self->Pitch = 0.0f;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptTargetAlgorithm_snap);
#ifdef SKIP_ASM
/**
 * Snaps the camera into place by running a full update at once.
 *
 * Calls the object's virtual update() (vtable slot 5, cScriptTargetAlgorithm_update).
 */
void cScriptTargetAlgorithm::snap(int)
{
    ((cScriptCtlVirt*)this)->update();
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptTargetAlgorithm_snapCurrent);
#ifdef SKIP_ASM
/**
 * Snaps the camera onto its current subject: calls the virtual snap() with subject 0.
 *
 * Calls vtable slot 3 (cScriptTargetAlgorithm_snap) with 0 as the subject id (cCameraAlgorithm::snap's argument in On
 * Tour), which the script algorithm's snap ignores.
 */
void cScriptTargetAlgorithm::snapCurrent()
{
    ((cScriptCtlVirt2*)this)->snap(0);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptTargetAlgorithm_update);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; needs a C fallback off-PS2.
float* GetSSXCameraSubjectHeader(void* cam);
extern "C" cRider* cSSXScriptEngine_GetSubjectRider(void* self, int subject, cScriptPlayerMain* player);
extern "C" void GetSSXCameraSubjectFrame(void* cam, void* out);
extern "C" float func_0031C228(float x);
extern "C" void cCameraJitterer_updatePositionalJitter(void* self, float v);
extern "C" void cCameraJitterer_updateRollJitter(void* self, float v);

struct sVec4_16A510 {
    float x, y, z, w;
    sVec4_16A510() {}
    sVec4_16A510(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

static inline sVec4_16A510 mtxMulVec_16A510(char* m, const sVec4_16A510& v)
{
    sVec4_16A510 r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

static inline sVec4_16A510 Scale_16A510(const sVec4_16A510& v, float s)
{
    sVec4_16A510 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline sVec4_16A510 Add_16A510(const sVec4_16A510& a, const sVec4_16A510& b)
{
    sVec4_16A510 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline sVec4_16A510 Sub_16A510(const sVec4_16A510& a, const sVec4_16A510& b)
{
    sVec4_16A510 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_16A510(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

static inline float Atan2_16A510(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

static inline sVec4_16A510 vec_16A510(char* p)
{
    return *(sVec4_16A510*)p;
}

struct sVec2_16A510 {
    float x, y;
    sVec2_16A510(float ax, float ay) : x(ax), y(ay) {}
};

static inline float Length_16A510(const sVec2_16A510& v)
{
    return Sqrt_16A510(v.x * v.x + v.y * v.y);
}



struct sVEp_16A510 {
    short delta;
    short index;
    sVec4_16A510* (*fn)(void*);
};

/**
 * Updates the target script camera: a scripted position that looks at the subject rider.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (GetSSXCameraSubjectHeader, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and GetSSXCameraSubjectFrame reads
 * this frame's channel values into FrameInfo (+0x4C, On Tour tSSXFrameCameraSubject). The subject rider is
 * cSSXScriptEngine_GetSubjectRider(ScriptEngine, the header's subject byte (+0x22, byte 34), the entity's script player
 * at +0x8), or the first rider (SSXApp->Game->AI->Riders[0]) when there is none. FrameInfo angles are in degrees
 * (converted with * 0.0174533).
 *
 * View (+0x0) becomes fov (+0x64) and Roll (+0x40) roll (+0x60) plus mBaseRotX (ScriptObj +0x70). Position (+0x30) is
 * mBaseTransform (ScriptObj +0x30) times (posX, posY, posZ) (+0x4C) with Position's previous w. Target (+0x20) is the
 * rider's position (virtual getPosition) plus its front row (mMatrix +0x1B0) times targetLeadDistance (+0x58) and its
 * up row (+0x1C0) times targetLeadHeight (+0x5C), the two scaled by the rider model's scale vector (mAnimModel +0x140:
 * .y for the distance, .x for the height). Heading (+0x44) and Pitch (+0x48) are the yaw and elevation of Target -
 * Position. Finally the jitterer (+0x70) is updated with jitterFocus (+0x68) and jitterRoll (+0x6C).
 *
 */
void cScriptTargetAlgorithm::update()
{
    cScriptTargetAlgorithm* self = this;
    char* cam = (char*)ScriptObj->mpCameraEntity->mpEntity;
    float* header = GetSSXCameraSubjectHeader(cam);
    char* baseTransform = (char*)&ScriptObj->mBaseTransform;
    char* rider = (char*)cSSXScriptEngine_GetSubjectRider(ScriptEngine, ((signed char*)header)[34],
            *(cScriptPlayerMain**)(cam + 8));
    if (rider == 0) {
        rider = (char*)SSXApp->Game->AI->Riders[0];
    }
    self->Near = header[0];
    self->Far = header[1];
    GetSSXCameraSubjectFrame(cam, &self->FrameInfo);
    char* obj = rider + 0x6C0;
    sVec4_16A510 rot = *(sVec4_16A510*)(*(char**)(rider + 0x780) + 0x140);
    self->View = self->FrameInfo.fov * 0.01745329424738884f;
    self->Roll = self->FrameInfo.roll * 0.01745329424738884f + ScriptObj->mBaseRotX;
    (*(sVec4_16A510*)&self->Position) = sVec4_16A510(self->FrameInfo.posX, self->FrameInfo.posY, self->FrameInfo.posZ,
                                                     (*(sVec4_16A510*)&self->Position).w);
    (*(sVec4_16A510*)&self->Position) = mtxMulVec_16A510(baseTransform, (*(sVec4_16A510*)&self->Position));
    sVEp_16A510* vt = *(sVEp_16A510**)obj;
    sVec4_16A510* rp = vt[5].fn(vthis(obj, vt[5]));
    (*(sVec4_16A510*)&self->Target) = Add_16A510(Add_16A510(*rp, Scale_16A510(vec_16A510(rider + 0x1B0),
                                                              self->FrameInfo.targetLeadDistance * rot.y)),
                               Scale_16A510(vec_16A510(rider + 0x1C0), self->FrameInfo.targetLeadHeight * rot.x));
    sVec4_16A510 look = Sub_16A510((*(sVec4_16A510*)&self->Target), (*(sVec4_16A510*)&self->Position));
    self->Heading = Atan2_16A510(look.y, look.x);
    float horizontalLen = Length_16A510(sVec2_16A510(look.x, look.y));
    self->Pitch = Atan2_16A510(look.z, horizontalLen);
    cCameraJitterer_updatePositionalJitter(&Jitterer, self->FrameInfo.jitterFocus);
    cCameraJitterer_updateRollJitter(&Jitterer, self->FrameInfo.jitterRoll);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptTargetAlgorithm_getPose);
#ifdef SKIP_ASM



extern t4Matrix t4mat_Identity_A868 __asm__("t4mat_Identity");
extern t4Vector t4vect_XAxis_A868 __asm__("t4vect_XAxis");
extern t4Vector t4vect_YAxis_A868 __asm__("t4vect_YAxis");
extern t4Vector t4vect_ZAxis_A868 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" tPose tPose_tPose_A868(t4Matrix* m) __asm__("tPose_tPose");
extern "C" t4Vector cCameraJitterer_getPositionalJitter_A868(char* p, float a, float b) __asm__("cCameraJitterer_getPositionalJitter");
extern "C" float cCameraJitterer_getRollJitter(void* self);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_A868(t4Matrix* dst, const t4Matrix* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void vu0MulMtx_A868(t4Matrix* dst, const t4Matrix* a, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_A868(t4Matrix* m, const t4Vector& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_A868(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline void SinCos_A868(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void RotAxis_A868(t4Matrix* m, const t4Vector& axis, float angle)
{
    t4Matrix r;
    float s;
    float c;
    float one = 1.0f;
    SinCos_A868(&s, &c, angle);
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    ((float (*)[4])&r.row[0])[0][0] = tx * x + c;
    ((float (*)[4])&r.row[0])[1][0] = tx * y + sz;
    ((float (*)[4])&r.row[0])[2][0] = tx * z - sy;
    ((float (*)[4])&r.row[0])[0][1] = ty * x - sz;
    ((float (*)[4])&r.row[0])[1][1] = ty * y + c;
    ((float (*)[4])&r.row[0])[2][1] = ty * z + sx;
    ((float (*)[4])&r.row[0])[0][2] = tz * x + sy;
    ((float (*)[4])&r.row[0])[1][2] = tz * y - sx;
    ((float (*)[4])&r.row[0])[2][2] = tz * z + c;
    ((float (*)[4])&r.row[0])[3][0] = 0.0f;
    ((float (*)[4])&r.row[0])[3][1] = 0.0f;
    ((float (*)[4])&r.row[0])[3][2] = 0.0f;
    ((float (*)[4])&r.row[0])[0][3] = 0.0f;
    ((float (*)[4])&r.row[0])[1][3] = 0.0f;
    ((float (*)[4])&r.row[0])[2][3] = 0.0f;
    ((float (*)[4])&r.row[0])[3][3] = one;
    vu0MulMtx_A868(m, m, &r);
}

/**
 * Returns the camera pose: Position plus jitter, turned by Roll, Pitch and Heading.
 *
 * Starts from t4mat_Identity translated to Position (+0x30) plus the jitterer's (+0x70) positional jitter for Heading
 * (+0x44) and Pitch (+0x48) (cCameraJitterer_getPositionalJitter, On Tour cCameraJitterer::getPositionalJitter), then rotates it about
 * t4vect_XAxis by Roll (+0x40) plus cCameraJitterer_getRollJitter, about t4vect_YAxis by -Pitch and about t4vect_ZAxis
 * by Heading, and splits the matrix into a position and a rotation quaternion with tPose_tPose. The four script
 * algorithms share this body; only the jitterer offset differs.
 *
 * @param algorithm  the cScriptTargetAlgorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" tPose cScriptTargetAlgorithm_getPose(cScriptTargetAlgorithm* algorithm)
{
    t4Matrix mat;
    vu0CopyMtx_A868(&mat, &t4mat_Identity_A868);
    vu0TransMtx_A868(&mat, Add_A868(*(&algorithm->Position), cCameraJitterer_getPositionalJitter_A868((char*)&algorithm->Jitterer, algorithm->Heading, algorithm->Pitch)));
    RotAxis_A868(&mat, t4vect_XAxis_A868, algorithm->Roll + cCameraJitterer_getRollJitter(&algorithm->Jitterer));
    RotAxis_A868(&mat, t4vect_YAxis_A868, -algorithm->Pitch);
    RotAxis_A868(&mat, t4vect_ZAxis_A868, algorithm->Heading);
    return tPose_tPose_A868(&mat);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_cScriptRelativeAlgorithm);
#ifdef SKIP_ASM
extern void* cScriptRelativeAlgorithm_vtbl[];
extern t4Vector t4vect_Origin;
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(cScriptAlgorithm* self);
extern "C" void* cCameraJitterer_cCameraJitterer(void* self);

/**
 * Constructs a relative script camera algorithm (camera placed relative to an algorithmic subject).
 *
 * Runs cScriptAlgorithm_cScriptAlgorithm, installs cScriptRelativeAlgorithm_vtbl at +0x10, constructs the jitterer at +0x70, sets the position (+0x30)
 * to t4vect_Origin and the type (+0xC) to 0x59 (On Tour SCRIPT_RELATIVE), and zeroes roll, heading and pitch.
 *
 * @return @p self
 */
extern "C" void* cScriptRelativeAlgorithm_cScriptRelativeAlgorithm(cScriptRelativeAlgorithm* self)
{
    cScriptAlgorithm_cScriptAlgorithm((cScriptAlgorithm*)self);
    VPTR_OF(cCameraAlgorithm, self) = cScriptRelativeAlgorithm_vtbl;
    cCameraJitterer_cCameraJitterer(&self->Jitterer);
    self->Position = t4vect_Origin;
    self->AlgorithmType = 89;
    self->Roll = 0.0f;
    self->Heading = 0.0f;
    self->Pitch = 0.0f;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_snap);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; needs a C fallback off-PS2.
tSSXHeaderCameraAlgorithmic* GetSSXCameraAlgorithmicHeader(void* cam);
extern "C" cRider* cSSXScriptEngine_GetSubjectRider(void* self, int subject, cScriptPlayerMain* player);
extern "C" void GetSSXCameraAlgorithmicFrame(void* cam, void* out);
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" float func_0031C228(float x);
extern "C" void cCameraJitterer_updatePositionalJitter(void* self, float v);
extern "C" void cCameraJitterer_updateRollJitter(void* self, float v);

static inline t4VectorCtor mtxMulVec_16AD98(char* m, const t4Vector& v)
{
    t4VectorCtor r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

static inline t4VectorCtor Scale_16AD98(const t4Vector& v, float s)
{
    t4VectorCtor r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline t4VectorCtor Add_16AD98(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline t4VectorCtor Sub_16AD98(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_16AD98(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

static inline float Atan2_16AD98(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

static inline t4Vector vec_16AD98(const t4Vector& p)
{
    return p;
}

static inline float Length_16AD98(const t2Vector& v)
{
    return Sqrt_16AD98(v.x * v.x + v.y * v.y);
}



/**
 * Snaps the relative script camera straight to its ideal place behind (or around) the subject rider.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (GetSSXCameraAlgorithmicHeader, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and GetSSXCameraAlgorithmicFrame reads this frame's channel
 * values into FrameInfo (+0x4C, On Tour tSSXFrameCameraAlgorithmic). The subject rider is
 * cSSXScriptEngine_GetSubjectRider(ScriptEngine, the header's subject byte (+0x1D), the entity's script player at
 * +0x8), or the first rider (SSXApp->Game->AI->Riders[0]) when there is none. FrameInfo angles are in degrees
 * (converted with * 0.0174533).
 *
 * View (+0x0) becomes fov (+0x64) and Roll (+0x40) roll (+0x60) plus mBaseRotX (ScriptObj +0x70). Target (+0x20) is the
 * rider's position (virtual getPosition) plus its front row (mMatrix +0x1B0) times targetLeadDistance (+0x4C) and its
 * up row (+0x1C0) times targetLeadHeight (+0x50), the two scaled by the rider model's scale vector (mAnimModel +0x140:
 * .y for the distance, .x for the height). When the header's areAnglesWorldRelative byte (+0x1E) is 0 the angles follow
 * the rider's front row: heading = optimalHeading (+0x5C) + its yaw, pitch = its elevation - optimalPitch (+0x58);
 * otherwise they are the two values as they are. mBaseRotZ (ScriptObj +0x78) is added to the heading and mBaseRotY
 * (+0x74) to the pitch. Position (+0x30) is Target minus the direction (cos pitch cos heading, cos pitch sin heading,
 * -sin pitch) times optimalTargetDistance (+0x54). Finally the jitterer (+0x70) is updated with jitterFocus (+0x68) and
 * jitterRoll (+0x6C).
 *
 * Unlike cScriptRelativeAlgorithm_update it does not recompute Heading (+0x44) and Pitch (+0x48), so a getPose straight
 * after a snap still uses the old angles.
 *
 */
void cScriptRelativeAlgorithm::snap(int)
{
    cScriptRelativeAlgorithm* self = this;
    cScriptPlayerEntity* cam = ScriptObj->mpCameraEntity->mpEntity;
    tSSXHeaderCameraAlgorithmic* header = GetSSXCameraAlgorithmicHeader(cam);
    GetSSXCameraAlgorithmicFrame(cam, &self->FrameInfo);
    self->Near = header->nearPlane;
    self->Far = header->farPlane;
    self->View = self->FrameInfo.fov * 0.01745329424738884f;
    self->Roll = self->FrameInfo.roll * 0.01745329424738884f + ScriptObj->mBaseRotX;
    cRider* rider = cSSXScriptEngine_GetSubjectRider(ScriptEngine, header->subject, cam->mpOwner);
    if (rider == 0) {
        rider = SSXApp->Game->AI->Riders[0];
    }
    cBeing& being = *rider;
    t4Vector rot = rider->mAnimModel->ScaleVect;
    t4Vector* rp = being.getPosition();
    self->Target = Add_16AD98(Add_16AD98(*rp, Scale_16AD98(vec_16AD98(rider->mMatrix.row[1]),
                                                           self->FrameInfo.targetLeadDistance * rot.y)),
                           Scale_16AD98(vec_16AD98(rider->mMatrix.row[2]), self->FrameInfo.targetLeadHeight * rot.x));
    float pitchAngle;
    float headingAngle;
    float k;
    if (header->areAnglesWorldRelative == 0) {
        t4Vector front = vec_16AD98(rider->mMatrix.row[1]);
        t4Vector* pv = &front;
        float optPitch = self->FrameInfo.optimalPitch * 0.01745329424738884f;
        pitchAngle = Atan2_16AD98(front.z, Length_16AD98(t2Vector(front.x, front.y))) - optPitch;
        float optHead = self->FrameInfo.optimalHeading * 0.01745329424738884f;
        headingAngle = optHead + Atan2_16AD98(pv->y, pv->x);
        k = self->FrameInfo.optimalTargetDistance;
    } else {
        pitchAngle = self->FrameInfo.optimalPitch * 0.01745329424738884f;
        headingAngle = self->FrameInfo.optimalHeading * 0.01745329424738884f;
        k = self->FrameInfo.optimalTargetDistance;
    }
    float sinHead, cosHead, sinPitch, cosPitch;
    cScriptObjCamera* scriptObj = ScriptObj;
    headingAngle += scriptObj->mBaseRotZ;
    pitchAngle += scriptObj->mBaseRotY;
    fSinCos(&sinHead, &cosHead, headingAngle);
    fSinCos(&sinPitch, &cosPitch, pitchAngle);
    t4VectorCtor look(cosPitch * cosHead, cosPitch * sinHead, -sinPitch, 0.0f);
    self->Position = Sub_16AD98(self->Target, Scale_16AD98(look, k));
    cCameraJitterer_updatePositionalJitter(&Jitterer, self->FrameInfo.jitterFocus);
    cCameraJitterer_updateRollJitter(&Jitterer, self->FrameInfo.jitterRoll);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_snapCurrent);
#ifdef SKIP_ASM
/**
 * Snaps the camera onto its current subject: calls the virtual snap() with subject 0.
 *
 * Calls vtable slot 3 (cScriptRelativeAlgorithm_snap) with 0 as the subject id (cCameraAlgorithm::snap's argument in On
 * Tour), which the script algorithm's snap ignores.
 */
void cScriptRelativeAlgorithm::snapCurrent()
{
    ((cScriptCtlVirt2*)this)->snap(0);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_update);
#ifdef SKIP_ASM
static inline t4VectorCtor mtxMulVec_B180(char* m, const t4Vector& v)
{
    t4VectorCtor r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

static inline t4VectorCtor Scale_B180(const t4Vector& v, float s)
{
    t4VectorCtor r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline t4VectorCtor Add_B180(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline t4VectorCtor Sub_B180(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_B180(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

static inline float Atan2_B180(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

static inline t4Vector vec_B180(const t4Vector& p)
{
    return p;
}

static inline float Length_B180(const t2Vector& v)
{
    return Sqrt_B180(v.x * v.x + v.y * v.y);
}



// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4VectorCtor Normalize_B180(const t4Vector& v)
{
    t4VectorCtor r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length4_B180(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

/**
 * Updates the relative script camera: follows the subject rider, easing towards its ideal place at a capped speed.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (GetSSXCameraAlgorithmicHeader, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and GetSSXCameraAlgorithmicFrame reads this frame's channel
 * values into FrameInfo (+0x4C, On Tour tSSXFrameCameraAlgorithmic). The subject rider is
 * cSSXScriptEngine_GetSubjectRider(ScriptEngine, the header's subject byte (+0x1D), the entity's script player at
 * +0x8), or the first rider (SSXApp->Game->AI->Riders[0]) when there is none. FrameInfo angles are in degrees
 * (converted with * 0.0174533).
 *
 * View (+0x0) becomes fov (+0x64) and Roll (+0x40) roll (+0x60) plus mBaseRotX (ScriptObj +0x70). Target (+0x20) is the
 * rider's position (virtual getPosition) plus its front row (mMatrix +0x1B0) times targetLeadDistance (+0x4C) and its
 * up row (+0x1C0) times targetLeadHeight (+0x50), the two scaled by the rider model's scale vector (mAnimModel +0x140:
 * .y for the distance, .x for the height). When the header's areAnglesWorldRelative byte (+0x1E) is 0 the angles follow
 * the rider's front row: heading = optimalHeading (+0x5C) + its yaw, pitch = its elevation - optimalPitch (+0x58);
 * otherwise they are the two values as they are. mBaseRotZ (ScriptObj +0x78) is added to the heading and mBaseRotY
 * (+0x74) to the pitch. The ideal position is Target minus the direction (cos pitch cos heading, cos pitch sin heading,
 * -sin pitch) times optimalTargetDistance (+0x54).
 *
 * Position (+0x30) moves to ideal * stickiness + Position * (1 - stickiness) (header +0x14), with w 1, but by at most
 * maxSpeed / 60 per call (header +0x18; the step is compared at 60 per second). Heading (+0x44) and Pitch (+0x48) are
 * then the yaw and elevation of Target - Position, and the jitterer (+0x70) is updated with jitterFocus (+0x68) and
 * jitterRoll (+0x6C).
 *
 */
void cScriptRelativeAlgorithm::update()
{
    cScriptRelativeAlgorithm* self = this;
    cScriptPlayerEntity* cam = ScriptObj->mpCameraEntity->mpEntity;
    tSSXHeaderCameraAlgorithmic* header = GetSSXCameraAlgorithmicHeader(cam);
    cRider* rider = cSSXScriptEngine_GetSubjectRider(ScriptEngine, header->subject, cam->mpOwner);
    if (rider == 0) {
        rider = SSXApp->Game->AI->Riders[0];
    }
    self->Near = header->nearPlane;
    self->Far = header->farPlane;
    GetSSXCameraAlgorithmicFrame(cam, &self->FrameInfo);
    self->View = self->FrameInfo.fov * 0.01745329424738884f;
    self->Roll = self->FrameInfo.roll * 0.01745329424738884f + ScriptObj->mBaseRotX;
    float pitchAngle;
    float headingAngle;
    float k;
    if (header->areAnglesWorldRelative == 0) {
        t4Vector front = vec_B180(rider->mMatrix.row[1]);
        t4Vector* pv = &front;
        float optPitch = self->FrameInfo.optimalPitch * 0.01745329424738884f;
        pitchAngle = Atan2_B180(front.z, Length_B180(t2Vector(front.x, front.y))) - optPitch;
        float optHead = self->FrameInfo.optimalHeading * 0.01745329424738884f;
        headingAngle = optHead + Atan2_B180(pv->y, pv->x);
        k = self->FrameInfo.optimalTargetDistance;
    } else {
        pitchAngle = self->FrameInfo.optimalPitch * 0.01745329424738884f;
        headingAngle = self->FrameInfo.optimalHeading * 0.01745329424738884f;
        k = self->FrameInfo.optimalTargetDistance;
    }
    cBeing& being = *rider;
    cScriptObjCamera* scriptObj = ScriptObj;
    pitchAngle += scriptObj->mBaseRotY;
    headingAngle += scriptObj->mBaseRotZ;
    t4Vector rot = rider->mAnimModel->ScaleVect;
    t4Vector* rp = being.getPosition();
    self->Target = Add_B180(Add_B180(*rp, Scale_B180(vec_B180(rider->mMatrix.row[1]),
                                                     self->FrameInfo.targetLeadDistance * rot.y)),
                         Scale_B180(vec_B180(rider->mMatrix.row[2]), self->FrameInfo.targetLeadHeight * rot.x));
    float sinHead, cosHead, sinPitch, cosPitch;
    fSinCos(&sinHead, &cosHead, headingAngle);
    fSinCos(&sinPitch, &cosPitch, pitchAngle);
    t4VectorCtor look(cosPitch * cosHead, cosPitch * sinHead, -sinPitch, 0.0f);
    t4VectorCtor idealPos = Sub_B180(self->Target, Scale_B180(look, k));
    t4VectorCtor nt = Add_B180(Scale_B180(idealPos, header->stickiness),
                               Scale_B180(self->Position, 1.0f - header->stickiness));
    nt.w = 1.0f;
    t4VectorCtor dv = Sub_B180(nt, self->Position);
    float maxd = header->maxSpeed;
    if (maxd < Length4_B180(dv) * 59.999996185302734f) {
        self->Position = Add_B180(self->Position, Scale_B180(Normalize_B180(dv), maxd * 0.01666666753590107f));
        self->Position.w = 1.0f;
    } else {
        self->Position = nt;
    }
    t4VectorCtor toTarget = Sub_B180(self->Target, self->Position);
    self->Heading = Atan2_B180(toTarget.y, toTarget.x);
    float horizontalLen = Length_B180(t2Vector(toTarget.x, toTarget.y));
    self->Pitch = Atan2_B180(toTarget.z, horizontalLen);
    cCameraJitterer_updatePositionalJitter(&Jitterer, self->FrameInfo.jitterFocus);
    cCameraJitterer_updateRollJitter(&Jitterer, self->FrameInfo.jitterRoll);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_getPose);
#ifdef SKIP_ASM

struct sM_B7B8 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPair_B7B8 {
    t4Vector a;
    t4Vector b;
};

extern sM_B7B8 t4mat_Identity_B7B8 __asm__("t4mat_Identity");
extern t4Vector t4vect_XAxis_B7B8 __asm__("t4vect_XAxis");
extern t4Vector t4vect_YAxis_B7B8 __asm__("t4vect_YAxis");
extern t4Vector t4vect_ZAxis_B7B8 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" sPair_B7B8 tPose_tPose_B7B8(sM_B7B8* m) __asm__("tPose_tPose");
extern "C" t4Vector cCameraJitterer_getPositionalJitter_B7B8(char* p, float a, float b) __asm__("cCameraJitterer_getPositionalJitter");
extern "C" float cCameraJitterer_getRollJitter(void* self);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_B7B8(sM_B7B8* dst, const sM_B7B8* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void vu0MulMtx_B7B8(sM_B7B8* dst, const sM_B7B8* a, const sM_B7B8* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_B7B8(sM_B7B8* m, const t4Vector& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_B7B8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline void SinCos_B7B8(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void RotAxis_B7B8(sM_B7B8* m, const t4Vector& axis, float angle)
{
    sM_B7B8 r;
    float s;
    float c;
    float one = 1.0f;
    SinCos_B7B8(&s, &c, angle);
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    vu0MulMtx_B7B8(m, m, &r);
}

/**
 * Returns the camera pose: Position plus jitter, turned by Roll, Pitch and Heading.
 *
 * Starts from t4mat_Identity translated to Position (+0x30) plus the jitterer's (+0x70) positional jitter for Heading
 * (+0x44) and Pitch (+0x48) (cCameraJitterer_getPositionalJitter, On Tour cCameraJitterer::getPositionalJitter), then rotates it about
 * t4vect_XAxis by Roll (+0x40) plus cCameraJitterer_getRollJitter, about t4vect_YAxis by -Pitch and about t4vect_ZAxis
 * by Heading, and splits the matrix into a position and a rotation quaternion with tPose_tPose. The four script
 * algorithms share this body; only the jitterer offset differs.
 *
 * @param algorithm  the cScriptRelativeAlgorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" sPair_B7B8 cScriptRelativeAlgorithm_getPose(cScriptRelativeAlgorithm* algorithm)
{
    sM_B7B8 mat;
    vu0CopyMtx_B7B8(&mat, &t4mat_Identity_B7B8);
    vu0TransMtx_B7B8(&mat, Add_B7B8(algorithm->Position,
            cCameraJitterer_getPositionalJitter_B7B8((char*)&algorithm->Jitterer, algorithm->Heading,
            algorithm->Pitch)));
    RotAxis_B7B8(&mat, t4vect_XAxis_B7B8, algorithm->Roll + cCameraJitterer_getRollJitter(&algorithm->Jitterer));
    RotAxis_B7B8(&mat, t4vect_YAxis_B7B8, -algorithm->Pitch);
    RotAxis_B7B8(&mat, t4vect_ZAxis_B7B8, algorithm->Heading);
    return tPose_tPose_B7B8(&mat);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BC88);
#ifdef SKIP_ASM
// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp_0016BC88(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

/**
 * Clamps a field of view to [5, 80] (degrees) in place.
 *
 * @param fov  the value to clamp
 */
extern "C" void func_0016BC88(float* fov)
{
    *fov = Clamp_0016BC88(*fov, 5.0f, 80.0f);
}
#endif
//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BCB8);
#ifdef SKIP_ASM
// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp_0016BCB8(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

/**
 * Clamps a lead value to [-3000, 3000] in place.
 *
 * @param lead  the value to clamp
 */
extern "C" void func_0016BCB8(float* lead)
{
    *lead = Clamp_0016BCB8(*lead, -3000.0f, 3000.0f);
}
#endif
//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BCE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_BCE0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

/**
 * Wraps an angle in radians into [-pi, pi) in place (subtracts the nearest multiple of 2 pi, rounding halves up).
 *
 * @param angle  the angle to wrap
 */
extern "C" void func_0016BCE0(float* angle)
{
    float radians = *angle;
    float n = ffloor_BCE0(radians * 0.15915493667125702f + 0.5f);
    *angle = radians - n * 6.2831854820251465f;
}
#endif
//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BD30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_BD30(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp_BD30(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

static inline float Wrap_BD30(float a)
{
    float n = ffloor_BD30(a * 0.15915493667125702f + 0.5f);
    return a - n * 6.2831854820251465f;
}

/**
 * Wraps an angle in radians into [-pi, pi) and then clamps it to [-pi / 2, pi / 2], in place.
 *
 * @param angle  the angle to limit
 */
extern "C" void func_0016BD30(float* angle)
{
    float w = Wrap_BD30(*angle);
    *angle = w;
    *angle = Clamp_BD30(w, -1.5707963705062866f, 1.5707963705062866f);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BDA0);
#ifdef SKIP_ASM
extern "C" void func_0016BCE0(float* p);

/**
 * Wraps an angle in degrees into [-180, 180) in place (converts to radians, func_0016BCE0, converts back).
 *
 * @param angle  the angle to wrap
 */
extern "C" void func_0016BDA0(float* angle)
{
    float radians = *angle * 0.01745329424738884f;
    func_0016BCE0(&radians);
    *angle = radians * 57.2957763671875f;
}
#endif
//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BDE8);
#ifdef SKIP_ASM
extern "C" void func_0016BD30(float* p);

/**
 * Wraps an angle in degrees into [-180, 180) and clamps it to [-90, 90], in place (through radians and
 * func_0016BD30).
 *
 * @param angle  the angle to limit
 */
extern "C" void func_0016BDE8(float* angle)
{
    float radians = *angle * 0.01745329424738884f;
    func_0016BD30(&radians);
    *angle = radians * 57.2957763671875f;
}
#endif
//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BE30);
#ifdef SKIP_ASM
// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp_0016BE30(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

/**
 * Clamps an angle in degrees to [-180, 180] in place.
 *
 * @param angle  the value to clamp
 */
extern "C" void func_0016BE30(float* angle)
{
    *angle = Clamp_0016BE30(*angle, -180.0f, 180.0f);
}
#endif
//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BE60);
#ifdef SKIP_ASM
// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp_0016BE60(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

/**
 * Clamps a jitter value to [0, 10] in place.
 *
 * @param jitter  the value to clamp
 */
extern "C" void func_0016BE60(float* jitter)
{
    *jitter = Clamp_0016BE60(*jitter, 0.0f, 10.0f);
}
#endif
//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BE90);
#ifdef SKIP_ASM
// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp_0016BE90(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

/**
 * Clamps a jitter value to [0, 30] in place.
 *
 * @param jitter  the value to clamp
 */
extern "C" void func_0016BE90(float* jitter)
{
    *jitter = Clamp_0016BE90(*jitter, 0.0f, 30.0f);
}
#endif
//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BEC0);
#ifdef SKIP_ASM
// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp_0016BEC0(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

/**
 * Clamps a distance to [-10000, 10000] in place.
 *
 * @param dist  the value to clamp
 */
extern "C" void func_0016BEC0(float* dist)
{
    *dist = Clamp_0016BEC0(*dist, -10000.0f, 10000.0f);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cCameraTriggerList_cCameraTriggerList__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a camera trigger list: clears its first word (the trigger array pointer).
 *
 * @return @p self
 */
void* cCameraTriggerList_cCameraTriggerList(void* vself)
{
    cCameraTriggerList* self = (cCameraTriggerList*)vself;
    *(int*)self = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cCameraTriggerList__cCameraTriggerList);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a camera trigger list: frees its memory with operator_delete when @p flags bit 0 is set; nothing else.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cCameraTriggerList__cCameraTriggerList(cCameraTriggerList* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif


#include "common.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"

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
extern "C" void* cScriptCameraController_cScriptCameraController(void* self, void* script, void* camera)
{
    int i;
    cCameraController_cCameraController(self, camera);
    *(void***)((char*)self + 0x14) = cScriptCameraController_vtbl;
    *(int*)self = 3;
    *(void***)((char*)self + 0x1C) = (void**)operator_new_K2(8, str_ScriptObjCamArray, 0, 0);
    for (i = 0; i < 2; i++) {
        void** slot = (void**)((i << 2) + *(int*)((char*)self + 0x1C));
        *slot = cScriptObjCamera_cScriptObjCamera(cMemMan_alloc(0x80, str_ScriptObjCam, 0, 0), script);
        cScriptObject_InitScriptObject((*(void***)((char*)self + 0x1C))[i]);
    }
    *(int*)((char*)self + 0x18) = 0;
    return self;
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
extern "C" void cScriptCameraController__cScriptCameraController(void* self, int flags)
{
    int i;
    *(void***)((char*)self + 0x14) = cScriptCameraController_vtbl;
    for (i = 0; i < 2; i++) {
        cScriptObject_DeInitScriptObject((*(cScrCamScriptK2***)((char*)self + 0x1C))[i]);
        delete (*(cScrCamScriptK2***)((char*)self + 0x1C))[i];
    }
    if (*(void**)((char*)self + 0x1C) != 0)
        cMemMan_free(*(void**)((char*)self + 0x1C));
    cCameraController__cCameraController(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_addCamera);
#ifdef SKIP_ASM
int func_002743C8(void* self);
void cScriptAlgorithm_setScriptObj(void* self, int val);
extern "C" void* cScriptManualAlgorithm_cScriptManualAlgorithm(void* self);
extern "C" void* cScriptPointAlgorithm_cScriptPointAlgorithm(void* self);
extern "C" void* cScriptTargetAlgorithm_cScriptTargetAlgorithm(void* self);
extern "C" void* cScriptRelativeAlgorithm_cScriptRelativeAlgorithm(void* self);
// PORT: cScriptCameraController_addAlgorithm is defined with (self, algo); this caller passes a third int argument.
extern "C" void cScriptCameraController_addAlgorithm_3(void* self, void* algo, int a) __asm__("cScriptCameraController_addAlgorithm");
extern char str_Manual_Script_Camera_Algo[];
extern char str_Target_Script_Camera_Algo[];
extern char str_Subject_Script_Camera_Algo[];
extern char str_Algorithmic_Script_Camera_Algo[];

/**
 * Creates the camera algorithm for a script camera object and adds it to the controller.
 *
 * The kind (func_002743C8 on the object's data) picks a manual (0), target (1), subject (2) or algorithmic
 * (3) script camera algorithm; other kinds are ignored. The algorithm is bound to the object, stored at its
 * +0x1C, added as owned (cScriptCameraController_addAlgorithm, probably addAlgorithm) and the controller becomes active (+0x18 = 1).
 *
 * @param cam  the script camera object (cScriptObjCamera)
 */
extern "C" void cScriptCameraController_addCamera(char* self, char* cam)
{
    void* ctl;
    switch (func_002743C8(*(void**)(*(char**)(cam + 0x14) + 8)))
    {
    case 0:
        ctl = cScriptManualAlgorithm_cScriptManualAlgorithm(cMemMan_alloc(176, str_Manual_Script_Camera_Algo, 0x20000000, 0));
        break;
    case 1:
        ctl = cScriptPointAlgorithm_cScriptPointAlgorithm(cMemMan_alloc(176, str_Target_Script_Camera_Algo, 0x20000000, 0));
        break;
    case 2:
        ctl = cScriptTargetAlgorithm_cScriptTargetAlgorithm(cMemMan_alloc(160, str_Subject_Script_Camera_Algo, 0x20000000, 0));
        break;
    case 3:
        ctl = cScriptRelativeAlgorithm_cScriptRelativeAlgorithm(cMemMan_alloc(160, str_Algorithmic_Script_Camera_Algo, 0x20000000, 0));
        break;
    default:
        return;
    }
    // PORT: the camera pointer is passed through an int parameter.
    cScriptAlgorithm_setScriptObj(ctl, (int)cam);
    *(void**)(cam + 0x1C) = ctl;
    cScriptCameraController_addAlgorithm_3(self, ctl, 1);
    *(int*)(self + 0x18) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_001692D8);
#ifdef SKIP_ASM
struct sScrAlgoNode {
    int key;              // 0x0
    char pad_0x04[0x10];
    sScrAlgoNode* next;   // 0x14
};

struct sScrAlgoList {
    sScrAlgoNode* head;   // 0x0
    int count;            // 0x4
};

extern "C" void func_0015CA50(sScrAlgoList* list, sScrAlgoNode* node);

extern "C" void func_001692D8(void* self, void* algo)
{
    if (*(int*)((char*)self + 0x18) != 0) {
        sScrAlgoList* list = *(sScrAlgoList**)((char*)self + 0x8);
        for (sScrAlgoNode* n = list->head; n != 0; n = n->next) {
            int k = *(int*)((char*)algo + 0x1C);
            if (n->key == k) {
                func_0015CA50(list, n);
                break;
            }
        }
        if (list->count <= 0)
            *(int*)((char*)self + 0x18) = 0;
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
extern "C" void cScriptCameraController_addAlgorithm(void* self, void* algo)
{
    cCameraAlgoList_insert(*(void**)((char*)self + 0x8), algo, 1.0f);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001694B0 returns a pointer (the unit declares it int); bind a pointer-returning alias.
char* func_001694B0_K2(void* self) __asm__("func_001694B0__FPv");

class cScrCamAlgoK2 {
public:
    char pad_0x00[0x10];
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
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
 * func_001694B0 returns for the algorithm; then virtual slot 5 runs the algorithm.
 */
extern "C" void cScriptCameraController_update(void* self)
{
    sScrAlgoNodeK2* n = **(sScrAlgoNodeK2***)((char*)self + 0x8);
    while (n != 0) {
        float progress = *(float*)(func_001694B0_K2(n->algo) + 0x18);
        float progressSq = progress * progress;
        float smoothed = progressSq * 3.0f - progressSq * 2.0f * progress;
        n->t = progress;
        if (smoothed > 1.0f)
            smoothed = 1.0f;
        n->weight = smoothed;
        n->algo->v05();
        n = n->next;
    }
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptAlgorithm_cScriptAlgorithm);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_0045BDE0[];
extern void* D_0045C8B8[];
struct sVec4K9418 { float x, y, z, w; } __attribute__((aligned(16)));
// Same object as the unit's later `extern cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4 t4vect_Origin;` (that type is defined below this point).
extern sVec4K9418 D_004FF130_K9418 __asm__("t4vect_Origin");
extern "C" float cCameraAlgorithm_getDefaultView(void* self);

struct sCamCtlK9418 {
    float fov;      // 0x0
    float nearZ;    // 0x4
    float farZ;     // 0x8
    int type;       // 0xC
    void** vtbl;    // 0x10
    int f14;        // 0x14
    int f18;        // 0x18
    int pad_1C;
    sVec4K9418 v20; // 0x20
};

/**
 * Constructs the generic script camera algorithm base: default view, script controller of the first view.
 *
 * Installs the cCameraAlgorithm vtable (D_0045BDE0) to read cCameraAlgorithm_getDefaultView, then the
 * cScriptAlgorithm vtable (D_0045C8B8) at +0x10; sets the type (+0xC) to 0x55 (On Tour SCRIPT_GENERIC), view,
 * near 10 and far 30000, clears the script object (+0x14), takes the controller (+0x18) from the first view
 * camera's script controller (+0xAC) and sets the target (+0x20) to t4vect_Origin.
 *
 * @param object  the object being constructed (this)
 * @return the object
 */
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(void* object)
{
    sCamCtlK9418* self = (sCamCtlK9418*)object;
    self->vtbl = D_0045BDE0;
    float fov = cCameraAlgorithm_getDefaultView(self);
    self->type = 85;
    self->vtbl = D_0045C8B8;
    self->fov = fov;
    self->nearZ = 10.0f;
    self->farZ = 30000.0f;
    self->f14 = 0;
    void* cam = SSXApp->Game;
    void* chase = *(void**)((char*)cam + 0x84);
    void* ctrl = *(void**)((char*)chase + 0x4);
    self->f18 = *(int*)((char*)ctrl + 0xAC);
    self->v20 = D_004FF130_K9418;
    return self;
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
void cScriptAlgorithm_setScriptObj(void* self, int val)
{
    *(int*)((char*)self + 0x14) = val;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_001694B0__FPv);
#ifdef SKIP_ASM
int func_001694B0(void* self)
{
    return *(int*)((char*)self + 0x14);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", cScriptManualAlgorithm_cScriptManualAlgorithm);
#ifdef SKIP_ASM
struct cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4 { float x, y, z, w; } __attribute__((aligned(16)));

extern void* cScriptManualAlgorithm_vtbl[];
extern cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4 t4vect_Origin;
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(void* self);
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
extern "C" void* cScriptManualAlgorithm_cScriptManualAlgorithm(void* self)
{
    cScriptAlgorithm_cScriptAlgorithm(self);
    *(void***)((char*)self + 0x10) = cScriptManualAlgorithm_vtbl;
    cCameraJitterer_cCameraJitterer((char*)self + 0x74);
    *(cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4*)((char*)self + 0x30) = t4vect_Origin;
    *(int*)((char*)self + 0xC) = 86;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x40) = 0;
    return self;
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
    virtual void v05();
};

/**
 * Snaps the camera into place by running a full update at once.
 *
 * Calls the object's virtual update() (vtable slot 5, cScriptManualAlgorithm_update).
 */
extern "C" void cScriptManualAlgorithm_snap(cScriptCtlVirt* self)
{
    self->v05();
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
    virtual void v03(int);
};

/**
 * Snaps the camera onto its current subject: calls the virtual snap() with subject 0.
 *
 * Calls vtable slot 3 (cScriptManualAlgorithm_snap) with 0 as the subject id (cCameraAlgorithm::snap's argument in On
 * Tour), which the script algorithm's snap ignores.
 */
extern "C" void cScriptManualAlgorithm_snapCurrent(cScriptCtlVirt2* self)
{
    self->v03(0);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptManualAlgorithm_update);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; needs a C fallback off-PS2.
float* func_0027C098(void* cam);
extern "C" void func_0027C0F0(void* cam, void* out);
extern "C" float func_0031C228(float x);
extern "C" void cCameraJitterer_updatePositionalJitter(void* self, float v);
extern "C" void cCameraJitterer_updateRollJitter(void* self, float v);

struct sVec4_169570 {
    float x, y, z, w;
    sVec4_169570() {}
    sVec4_169570(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

static inline sVec4_169570 mtxMulVec_169570(char* m, const sVec4_169570& v)
{
    sVec4_169570 r;
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

static inline sVec4_169570 Sub_169570(const sVec4_169570& a, const sVec4_169570& b)
{
    sVec4_169570 r;
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

struct sVec2_169570 {
    float x, y;
    sVec2_169570(float ax, float ay) : x(ax), y(ay) {}
};

static inline float Length_169570(const sVec2_169570& v)
{
    return Sqrt_169570(v.x * v.x + v.y * v.y);
}

struct sScrCam_169570 {
    float View;             // 0x0
    float Near;               // 0x4
    float Far;               // 0x8
    char padC[0x14];
    sVec4_169570 Target;       // 0x20
    sVec4_169570 Position;    // 0x30
    float Roll;            // 0x40
    float Heading;              // 0x44
    float Pitch;              // 0x48
    float ox, oy, oz;       // 0x4C
    float dist;             // 0x58
    float r5C;              // 0x5C
    float r60;              // 0x60
    float r64;              // 0x64
    float r68;              // 0x68
    float r6C;              // 0x6C
    float r70;              // 0x70
};

/**
 * Updates the manual script camera: position and look-at point straight from the script, transformed to the world.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (func_0027C098, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and func_0027C0F0 reads this frame's channel
 * values into FrameInfo (+0x4C, On Tour tSSXFrameCameraManual). FrameInfo angles are in degrees (converted with *
 * 0.0174533).
 *
 * View (+0x0) becomes fov (+0x68); Position (+0x30) is mBaseTransform (ScriptObj +0x30) times (posX, posY, posZ, 1)
 * (+0x4C); Roll (+0x40) is roll (+0x64) plus mBaseRotX (ScriptObj +0x70); Target (+0x20) is mBaseTransform times
 * (targetX, targetY, targetZ, 1) (+0x58). Heading (+0x44) and Pitch (+0x48) are the yaw and elevation of Target -
 * Position. Finally the jitterer (+0x74) is updated with jitterFocus (+0x6C) and jitterRoll (+0x70).
 *
 * @param p  the cScriptManualAlgorithm (this)
 */
extern "C" void cScriptManualAlgorithm_update(char* p)
{
    sScrCam_169570* self = (sScrCam_169570*)p;
    char* cam = *(char**)(*(char**)(*(char**)(p + 0x14) + 0x14) + 0x8);
    float* f = func_0027C098(cam);
    char* m = *(char**)(p + 0x14) + 0x30;
    self->Near = f[0];
    self->Far = f[1];
    func_0027C0F0(cam, &self->ox);
    self->View = self->r68 * 0.01745329424738884f;
    self->Position = sVec4_169570(self->ox, self->oy, self->oz, 1.0f);
    self->Position = mtxMulVec_169570(m, self->Position);
    self->Roll = self->r64 * 0.01745329424738884f + *(float*)(*(char**)(p + 0x14) + 0x70);
    self->Target = mtxMulVec_169570(m, sVec4_169570(self->dist, self->r5C, self->r60, 1.0f));
    sVec4_169570 d = Sub_169570(self->Target, self->Position);
    self->Heading = Atan2_169570(d.y, d.x);
    float h = Length_169570(sVec2_169570(d.x, d.y));
    self->Pitch = Atan2_169570(d.z, h);
    cCameraJitterer_updatePositionalJitter(p + 0x74, self->r6C);
    cCameraJitterer_updateRollJitter(p + 0x74, self->r70);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptManualAlgorithm_getPose);
#ifdef SKIP_ASM
struct sV_9828 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_9828 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPair_9828 {
    sV_9828 a;
    sV_9828 b;
};

extern sM_9828 D_004FF1A0_9828 __asm__("t4mat_Identity");
extern sV_9828 D_004FF140_9828 __asm__("t4vect_XAxis");
extern sV_9828 D_004FF150_9828 __asm__("t4vect_YAxis");
extern sV_9828 D_004FF160_9828 __asm__("t4vect_ZAxis");
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" sPair_9828 func_0031B748_9828(sM_9828* m) __asm__("func_0031B748");
extern "C" sV_9828 func_00160228_9828(char* p, float a, float b) __asm__("func_00160228");
extern "C" float cCameraJitterer_getRollJitter(void* self);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_9828(sM_9828* dst, const sM_9828* src)
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
static inline void vu0MulMtx_9828(sM_9828* dst, const sM_9828* a, const sM_9828* b)
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
static inline void vu0TransMtx_9828(sM_9828* m, const sV_9828& v)
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
static inline sV_9828 Add_9828(const sV_9828& a, const sV_9828& b)
{
    sV_9828 r;
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
    func_0031BE50(s, c, angle);
}

static inline void RotAxis_9828(sM_9828* m, const sV_9828& axis, float angle)
{
    sM_9828 r;
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
    vu0MulMtx_9828(m, m, &r);
}

/**
 * Returns the camera pose: Position plus jitter, turned by Roll, Pitch and Heading.
 *
 * Starts from t4mat_Identity translated to Position (+0x30) plus the jitterer's (+0x74) positional jitter for Heading
 * (+0x44) and Pitch (+0x48) (func_00160228, On Tour cCameraJitterer::getPositionalJitter), then rotates it about
 * t4vect_XAxis by Roll (+0x40) plus cCameraJitterer_getRollJitter, about t4vect_YAxis by -Pitch and about t4vect_ZAxis
 * by Heading, and splits the matrix into a position and a rotation quaternion with func_0031B748. The four script
 * algorithms share this body; only the jitterer offset differs.
 *
 * @param p  the cScriptManualAlgorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" sPair_9828 cScriptManualAlgorithm_getPose(char* p)
{
    sM_9828 m;
    vu0CopyMtx_9828(&m, &D_004FF1A0_9828);
    vu0TransMtx_9828(&m, Add_9828(*(sV_9828*)(p + 0x30), func_00160228_9828(p + 0x74, *(float*)(p + 0x44), *(float*)(p + 0x48))));
    RotAxis_9828(&m, D_004FF140_9828, *(float*)(p + 0x40) + cCameraJitterer_getRollJitter(p + 0x74));
    RotAxis_9828(&m, D_004FF150_9828, -*(float*)(p + 0x48));
    RotAxis_9828(&m, D_004FF160_9828, *(float*)(p + 0x44));
    return func_0031B748_9828(&m);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", cScriptPointAlgorithm_cScriptPointAlgorithm);
#ifdef SKIP_ASM
extern void* cScriptPointAlgorithm_vtbl[];
extern cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4 t4vect_Origin;
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(void* self);
extern "C" void* cCameraJitterer_cCameraJitterer(void* self);

/**
 * Constructs a point script camera algorithm (camera at a scripted point).
 *
 * Runs cScriptAlgorithm_cScriptAlgorithm, installs cScriptPointAlgorithm_vtbl at +0x10, constructs the jitterer at +0x74, sets the position (+0x30)
 * to t4vect_Origin and the type (+0xC) to 0x57 (On Tour SCRIPT_POINT), and zeroes roll, heading and pitch.
 *
 * @return @p self
 */
extern "C" void* cScriptPointAlgorithm_cScriptPointAlgorithm(void* self)
{
    cScriptAlgorithm_cScriptAlgorithm(self);
    *(void***)((char*)self + 0x10) = cScriptPointAlgorithm_vtbl;
    cCameraJitterer_cCameraJitterer((char*)self + 0x74);
    *(cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4*)((char*)self + 0x30) = t4vect_Origin;
    *(int*)((char*)self + 0xC) = 87;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
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
extern "C" void cScriptPointAlgorithm_snap(cScriptCtlVirt* self)
{
    self->v05();
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
extern "C" void cScriptPointAlgorithm_snapCurrent(cScriptCtlVirt2* self)
{
    self->v03(0);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptPointAlgorithm_update);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; needs a C fallback off-PS2.
float* func_0027C2A8(void* cam);
extern "C" void func_0027C300(void* cam, void* out);
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void cCameraJitterer_updatePositionalJitter(void* self, float v);
extern "C" void cCameraJitterer_updateRollJitter(void* self, float v);

struct sVec4_169DB0 {
    float x, y, z, w;
    sVec4_169DB0() {}
    sVec4_169DB0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

static inline sVec4_169DB0 mtxMulVec_169DB0(char* m, const sVec4_169DB0& v)
{
    sVec4_169DB0 r;
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

static inline sVec4_169DB0 Scale_169DB0(const sVec4_169DB0& v, float s)
{
    sVec4_169DB0 r;
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

static inline sVec4_169DB0 Sub_169DB0(const sVec4_169DB0& a, const sVec4_169DB0& b)
{
    sVec4_169DB0 r;
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

struct sScrCam_169DB0 {
    float View;             // 0x0
    float Near;               // 0x4
    float Far;               // 0x8
    char padC[0x14];
    sVec4_169DB0 Target;       // 0x20
    sVec4_169DB0 Position;    // 0x30
    float Roll;            // 0x40
    float Heading;              // 0x44
    float Pitch;              // 0x48
    float ox, oy, oz;       // 0x4C
    float dist;             // 0x58
    float r5C;              // 0x5C
    float r60;              // 0x60
    float r64;              // 0x64
    float r68;              // 0x68
    float r6C;              // 0x6C
    float r70;              // 0x70
};

/**
 * Updates the point script camera: orbits a scripted point at a distance, heading and pitch.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (func_0027C2A8, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and func_0027C300 reads this frame's channel
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
 * @param p  the cScriptPointAlgorithm (this)
 */
extern "C" void cScriptPointAlgorithm_update(char* p)
{
    sScrCam_169DB0* self = (sScrCam_169DB0*)p;
    char* cam = *(char**)(*(char**)(*(char**)(p + 0x14) + 0x14) + 0x8);
    float* f = func_0027C2A8(cam);
    char* m = *(char**)(p + 0x14) + 0x30;
    self->Near = f[0];
    self->Far = f[1];
    func_0027C300(cam, &self->ox);
    self->Near = f[0];
    self->Far = f[1];
    self->View = self->r68 * 0.01745329424738884f;
    char* g = *(char**)(p + 0x14);
    self->Heading = self->r60 * 0.01745329424738884f + *(float*)(g + 0x78);
    self->Pitch = self->r5C * 0.01745329424738884f + *(float*)(g + 0x74);
    self->Roll = self->r64 * 0.01745329424738884f + *(float*)(g + 0x70);
    self->Target = mtxMulVec_169DB0(m, sVec4_169DB0(self->ox, self->oy, self->oz, 1.0f));
    float s0, c0, s1, c1;
    func_0031BE50(&s0, &c0, self->Heading);
    func_0031BE50(&s1, &c1, self->Pitch);
    sVec4_169DB0 d(c1 * c0, c1 * s0, s1, 0.0f);
    self->Position = Sub_169DB0(self->Target, Scale_169DB0(d, self->dist));
    cCameraJitterer_updatePositionalJitter(p + 0x74, self->r6C);
    cCameraJitterer_updateRollJitter(p + 0x74, self->r70);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptPointAlgorithm_getPose);
#ifdef SKIP_ASM
struct sV_9F88 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_9F88 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPair_9F88 {
    sV_9F88 a;
    sV_9F88 b;
};

extern sM_9F88 D_004FF1A0_9F88 __asm__("t4mat_Identity");
extern sV_9F88 D_004FF140_9F88 __asm__("t4vect_XAxis");
extern sV_9F88 D_004FF150_9F88 __asm__("t4vect_YAxis");
extern sV_9F88 D_004FF160_9F88 __asm__("t4vect_ZAxis");
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" sPair_9F88 func_0031B748_9F88(sM_9F88* m) __asm__("func_0031B748");
extern "C" sV_9F88 func_00160228_9F88(char* p, float a, float b) __asm__("func_00160228");
extern "C" float cCameraJitterer_getRollJitter(void* self);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_9F88(sM_9F88* dst, const sM_9F88* src)
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
static inline void vu0MulMtx_9F88(sM_9F88* dst, const sM_9F88* a, const sM_9F88* b)
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
static inline void vu0TransMtx_9F88(sM_9F88* m, const sV_9F88& v)
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
static inline sV_9F88 Add_9F88(const sV_9F88& a, const sV_9F88& b)
{
    sV_9F88 r;
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
    func_0031BE50(s, c, angle);
}

static inline void RotAxis_9F88(sM_9F88* m, const sV_9F88& axis, float angle)
{
    sM_9F88 r;
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
    vu0MulMtx_9F88(m, m, &r);
}

/**
 * Returns the camera pose: Position plus jitter, turned by Roll, Pitch and Heading.
 *
 * Starts from t4mat_Identity translated to Position (+0x30) plus the jitterer's (+0x74) positional jitter for Heading
 * (+0x44) and Pitch (+0x48) (func_00160228, On Tour cCameraJitterer::getPositionalJitter), then rotates it about
 * t4vect_XAxis by Roll (+0x40) plus cCameraJitterer_getRollJitter, about t4vect_YAxis by -Pitch and about t4vect_ZAxis
 * by Heading, and splits the matrix into a position and a rotation quaternion with func_0031B748. The four script
 * algorithms share this body; only the jitterer offset differs.
 *
 * @param p  the cScriptPointAlgorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" sPair_9F88 cScriptPointAlgorithm_getPose(char* p)
{
    sM_9F88 m;
    vu0CopyMtx_9F88(&m, &D_004FF1A0_9F88);
    vu0TransMtx_9F88(&m, Add_9F88(*(sV_9F88*)(p + 0x30), func_00160228_9F88(p + 0x74, *(float*)(p + 0x44), *(float*)(p + 0x48))));
    RotAxis_9F88(&m, D_004FF140_9F88, *(float*)(p + 0x40) + cCameraJitterer_getRollJitter(p + 0x74));
    RotAxis_9F88(&m, D_004FF150_9F88, -*(float*)(p + 0x48));
    RotAxis_9F88(&m, D_004FF160_9F88, *(float*)(p + 0x44));
    return func_0031B748_9F88(&m);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", cScriptTargetAlgorithm_cScriptTargetAlgorithm);
#ifdef SKIP_ASM
extern void* cScriptTargetAlgorithm_vtbl[];
extern cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4 t4vect_Origin;
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(void* self);
extern "C" void* cCameraJitterer_cCameraJitterer(void* self);

/**
 * Constructs a target script camera algorithm (camera looking at a scripted subject).
 *
 * Runs cScriptAlgorithm_cScriptAlgorithm, installs cScriptTargetAlgorithm_vtbl at +0x10, constructs the jitterer at +0x70, sets the position (+0x30)
 * to t4vect_Origin and the type (+0xC) to 0x58 (On Tour SCRIPT_TARGET), and zeroes roll, heading and pitch.
 *
 * @return @p self
 */
extern "C" void* cScriptTargetAlgorithm_cScriptTargetAlgorithm(void* self)
{
    cScriptAlgorithm_cScriptAlgorithm(self);
    *(void***)((char*)self + 0x10) = cScriptTargetAlgorithm_vtbl;
    cCameraJitterer_cCameraJitterer((char*)self + 0x70);
    *(cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4*)((char*)self + 0x30) = t4vect_Origin;
    *(int*)((char*)self + 0xC) = 88;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
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
extern "C" void cScriptTargetAlgorithm_snap(cScriptCtlVirt* self)
{
    self->v05();
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
extern "C" void cScriptTargetAlgorithm_snapCurrent(cScriptCtlVirt2* self)
{
    self->v03(0);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptTargetAlgorithm_update);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; needs a C fallback off-PS2.
float* func_0027C4B8(void* cam);
extern "C" char* cSSXScriptEngine_GetSubjectRider(void* self, int a1, int a2);
extern "C" void func_0027C510(void* cam, void* out);
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

struct sScrCam_16A510 {
    float View;             // 0x0
    float Near;               // 0x4
    float Far;               // 0x8
    char padC[0x14];
    sVec4_16A510 Target;       // 0x20
    sVec4_16A510 Position;    // 0x30
    float Roll;            // 0x40
    float Heading;              // 0x44
    float Pitch;              // 0x48
    float ox, oy, oz;       // 0x4C
    float dist;             // 0x58
    float r5C;              // 0x5C
    float r60;              // 0x60
    float r64;              // 0x64
    float r68;              // 0x68
    float r6C;              // 0x6C
};

struct sVEp_16A510 {
    short delta;
    short index;
    sVec4_16A510* (*fn)(void*);
};

/**
 * Updates the target script camera: a scripted position that looks at the subject rider.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (func_0027C4B8, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and func_0027C510 reads this frame's channel
 * values into FrameInfo (+0x4C, On Tour tSSXFrameCameraSubject). The subject rider is
 * cSSXScriptEngine_GetSubjectRider(ScriptEngine, the header's subject byte (+0x22), the entity's script player at
 * +0x8), or the first rider (SSXApp->Game->AI->Riders[0]) when there is none. FrameInfo angles are in degrees
 * (converted with * 0.0174533).
 *
 * View (+0x0) becomes fov (+0x64) and Roll (+0x40) roll (+0x60) plus mBaseRotX (ScriptObj +0x70). Position (+0x30) is
 * mBaseTransform (ScriptObj +0x30) times (posX, posY, posZ) (+0x4C) with Position's previous w. Target (+0x20) is the
 * rider's position (virtual getPosition) plus its front row (mMatrix +0x1B0) times targetLeadDistance (+0x58) and its
 * up row (+0x1C0) times targetLeadHeight (+0x5C), the two scaled by the rider model's scale vector (mAnimModel +0x140:
 * .y for the distance, .x for the height). Heading (+0x44) and Pitch (+0x48) are the yaw and elevation of Target -
 * Position. Finally the jitterer (+0x70) is updated with jitterFocus (+0x68) and jitterRoll (+0x6C).
 *
 * @param p  the cScriptTargetAlgorithm (this)
 */
extern "C" void cScriptTargetAlgorithm_update(char* p)
{
    sScrCam_16A510* self = (sScrCam_16A510*)p;
    char* cam = *(char**)(*(char**)(*(char**)(p + 0x14) + 0x14) + 0x8);
    float* f = func_0027C4B8(cam);
    char* m = *(char**)(p + 0x14) + 0x30;
    char* rider = cSSXScriptEngine_GetSubjectRider(ScriptEngine, ((signed char*)f)[0x22], *(int*)(cam + 8));
    if (rider == 0) {
        rider = *(char**)((char*)SSXApp->Game->AI + 0x28);
    }
    self->Near = f[0];
    self->Far = f[1];
    func_0027C510(cam, &self->ox);
    char* obj = rider + 0x6C0;
    sVec4_16A510 rot = *(sVec4_16A510*)(*(char**)(rider + 0x780) + 0x140);
    self->View = self->r64 * 0.01745329424738884f;
    self->Roll = self->r60 * 0.01745329424738884f + *(float*)(*(char**)(p + 0x14) + 0x70);
    self->Position = sVec4_16A510(self->ox, self->oy, self->oz, self->Position.w);
    self->Position = mtxMulVec_16A510(m, self->Position);
    sVEp_16A510* vt = *(sVEp_16A510**)obj;
    sVec4_16A510* rp = vt[5].fn(obj + vt[5].delta);
    self->Target = Add_16A510(Add_16A510(*rp, Scale_16A510(vec_16A510(rider + 0x1B0), self->dist * rot.y)),
                               Scale_16A510(vec_16A510(rider + 0x1C0), self->r5C * rot.x));
    sVec4_16A510 t = Sub_16A510(self->Target, self->Position);
    self->Heading = Atan2_16A510(t.y, t.x);
    float h = Length_16A510(sVec2_16A510(t.x, t.y));
    self->Pitch = Atan2_16A510(t.z, h);
    cCameraJitterer_updatePositionalJitter(p + 0x70, self->r68);
    cCameraJitterer_updateRollJitter(p + 0x70, self->r6C);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptTargetAlgorithm_getPose);
#ifdef SKIP_ASM
struct sV_A868 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_A868 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPair_A868 {
    sV_A868 a;
    sV_A868 b;
};

extern sM_A868 D_004FF1A0_A868 __asm__("t4mat_Identity");
extern sV_A868 D_004FF140_A868 __asm__("t4vect_XAxis");
extern sV_A868 D_004FF150_A868 __asm__("t4vect_YAxis");
extern sV_A868 D_004FF160_A868 __asm__("t4vect_ZAxis");
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" sPair_A868 func_0031B748_A868(sM_A868* m) __asm__("func_0031B748");
extern "C" sV_A868 func_00160228_A868(char* p, float a, float b) __asm__("func_00160228");
extern "C" float cCameraJitterer_getRollJitter(void* self);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_A868(sM_A868* dst, const sM_A868* src)
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
static inline void vu0MulMtx_A868(sM_A868* dst, const sM_A868* a, const sM_A868* b)
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
static inline void vu0TransMtx_A868(sM_A868* m, const sV_A868& v)
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
static inline sV_A868 Add_A868(const sV_A868& a, const sV_A868& b)
{
    sV_A868 r;
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
    func_0031BE50(s, c, angle);
}

static inline void RotAxis_A868(sM_A868* m, const sV_A868& axis, float angle)
{
    sM_A868 r;
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
    vu0MulMtx_A868(m, m, &r);
}

/**
 * Returns the camera pose: Position plus jitter, turned by Roll, Pitch and Heading.
 *
 * Starts from t4mat_Identity translated to Position (+0x30) plus the jitterer's (+0x70) positional jitter for Heading
 * (+0x44) and Pitch (+0x48) (func_00160228, On Tour cCameraJitterer::getPositionalJitter), then rotates it about
 * t4vect_XAxis by Roll (+0x40) plus cCameraJitterer_getRollJitter, about t4vect_YAxis by -Pitch and about t4vect_ZAxis
 * by Heading, and splits the matrix into a position and a rotation quaternion with func_0031B748. The four script
 * algorithms share this body; only the jitterer offset differs.
 *
 * @param p  the cScriptTargetAlgorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" sPair_A868 cScriptTargetAlgorithm_getPose(char* p)
{
    sM_A868 m;
    vu0CopyMtx_A868(&m, &D_004FF1A0_A868);
    vu0TransMtx_A868(&m, Add_A868(*(sV_A868*)(p + 0x30), func_00160228_A868(p + 0x70, *(float*)(p + 0x44), *(float*)(p + 0x48))));
    RotAxis_A868(&m, D_004FF140_A868, *(float*)(p + 0x40) + cCameraJitterer_getRollJitter(p + 0x70));
    RotAxis_A868(&m, D_004FF150_A868, -*(float*)(p + 0x48));
    RotAxis_A868(&m, D_004FF160_A868, *(float*)(p + 0x44));
    return func_0031B748_A868(&m);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_cScriptRelativeAlgorithm);
#ifdef SKIP_ASM
extern void* cScriptRelativeAlgorithm_vtbl[];
extern cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4 t4vect_Origin;
extern "C" void* cScriptAlgorithm_cScriptAlgorithm(void* self);
extern "C" void* cCameraJitterer_cCameraJitterer(void* self);

/**
 * Constructs a relative script camera algorithm (camera placed relative to an algorithmic subject).
 *
 * Runs cScriptAlgorithm_cScriptAlgorithm, installs cScriptRelativeAlgorithm_vtbl at +0x10, constructs the jitterer at +0x70, sets the position (+0x30)
 * to t4vect_Origin and the type (+0xC) to 0x59 (On Tour SCRIPT_RELATIVE), and zeroes roll, heading and pitch.
 *
 * @return @p self
 */
extern "C" void* cScriptRelativeAlgorithm_cScriptRelativeAlgorithm(void* self)
{
    cScriptAlgorithm_cScriptAlgorithm(self);
    *(void***)((char*)self + 0x10) = cScriptRelativeAlgorithm_vtbl;
    cCameraJitterer_cCameraJitterer((char*)self + 0x70);
    *(cScriptManualAlgorithm_cScriptManualAlgorithm_sVec4*)((char*)self + 0x30) = t4vect_Origin;
    *(int*)((char*)self + 0xC) = 89;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_snap);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; needs a C fallback off-PS2.
float* func_0027C6A0(void* cam);
extern "C" char* cSSXScriptEngine_GetSubjectRider(void* self, int a1, int a2);
extern "C" void func_0027C6F8(void* cam, void* out);
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" float func_0031C228(float x);
extern "C" void cCameraJitterer_updatePositionalJitter(void* self, float v);
extern "C" void cCameraJitterer_updateRollJitter(void* self, float v);

struct sVec4_16AD98 {
    float x, y, z, w;
    sVec4_16AD98() {}
    sVec4_16AD98(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

static inline sVec4_16AD98 mtxMulVec_16AD98(char* m, const sVec4_16AD98& v)
{
    sVec4_16AD98 r;
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

static inline sVec4_16AD98 Scale_16AD98(const sVec4_16AD98& v, float s)
{
    sVec4_16AD98 r;
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

static inline sVec4_16AD98 Add_16AD98(const sVec4_16AD98& a, const sVec4_16AD98& b)
{
    sVec4_16AD98 r;
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

static inline sVec4_16AD98 Sub_16AD98(const sVec4_16AD98& a, const sVec4_16AD98& b)
{
    sVec4_16AD98 r;
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

static inline sVec4_16AD98 vec_16AD98(char* p)
{
    return *(sVec4_16AD98*)p;
}

struct sVec2_16AD98 {
    float x, y;
    sVec2_16AD98(float ax, float ay) : x(ax), y(ay) {}
};

static inline float Length_16AD98(const sVec2_16AD98& v)
{
    return Sqrt_16AD98(v.x * v.x + v.y * v.y);
}

struct sScrCam_16AD98 {
    float View;             // 0x0
    float Near;               // 0x4
    float Far;               // 0x8
    char padC[0x14];
    sVec4_16AD98 Target;       // 0x20
    sVec4_16AD98 Position;    // 0x30
    float Roll;            // 0x40
    float Heading;              // 0x44
    float Pitch;              // 0x48
    float ox, oy, oz;       // 0x4C
    float dist;             // 0x58
    float r5C;              // 0x5C
    float r60;              // 0x60
    float r64;              // 0x64
    float r68;              // 0x68
    float r6C;              // 0x6C
};

struct sVEp_16AD98 {
    short delta;
    short index;
    sVec4_16AD98* (*fn)(void*);
};

/**
 * Snaps the relative script camera straight to its ideal place behind (or around) the subject rider.
 *
 * The playing camera entity is ScriptObj (+0x14) -> mpCameraEntity (+0x14) -> mpEntity (+0x8); its header
 * (func_0027C6A0, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and func_0027C6F8 reads this frame's channel
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
 * @param p  the cScriptRelativeAlgorithm (this)
 */
extern "C" void cScriptRelativeAlgorithm_snap(char* p)
{
    sScrCam_16AD98* self = (sScrCam_16AD98*)p;
    char* cam = *(char**)(*(char**)(*(char**)(p + 0x14) + 0x14) + 0x8);
    float* f = func_0027C6A0(cam);
    func_0027C6F8(cam, &self->ox);
    self->Near = f[0];
    self->Far = f[1];
    self->View = self->r64 * 0.01745329424738884f;
    self->Roll = self->r60 * 0.01745329424738884f + *(float*)(*(char**)(p + 0x14) + 0x70);
    char* rider = cSSXScriptEngine_GetSubjectRider(ScriptEngine, ((signed char*)f)[0x1D], *(int*)(cam + 8));
    if (rider == 0) {
        rider = *(char**)((char*)SSXApp->Game->AI + 0x28);
    }
    char* obj = rider + 0x6C0;
    sVec4_16AD98 rot = *(sVec4_16AD98*)(*(char**)(rider + 0x780) + 0x140);
    sVEp_16AD98* vt = *(sVEp_16AD98**)obj;
    sVec4_16AD98* rp = vt[5].fn(obj + vt[5].delta);
    self->Target = Add_16AD98(Add_16AD98(*rp, Scale_16AD98(vec_16AD98(rider + 0x1B0), self->ox * rot.y)),
                           Scale_16AD98(vec_16AD98(rider + 0x1C0), self->oy * rot.x));
    float pitchAngle;
    float headingAngle;
    float k;
    if (((signed char*)f)[0x1E] == 0) {
        sVec4_16AD98 v = vec_16AD98(rider + 0x1B0);
        sVec4_16AD98* pv = &v;
        float a = self->dist * 0.01745329424738884f;
        pitchAngle = Atan2_16AD98(v.z, Length_16AD98(sVec2_16AD98(v.x, v.y))) - a;
        float b = self->r5C * 0.01745329424738884f;
        headingAngle = b + Atan2_16AD98(pv->y, pv->x);
        k = self->oz;
    } else {
        pitchAngle = self->dist * 0.01745329424738884f;
        headingAngle = self->r5C * 0.01745329424738884f;
        k = self->oz;
    }
    float s0, c0, s1, c1;
    char* g = *(char**)(p + 0x14);
    headingAngle += *(float*)(g + 0x78);
    pitchAngle += *(float*)(g + 0x74);
    func_0031BE50(&s0, &c0, headingAngle);
    func_0031BE50(&s1, &c1, pitchAngle);
    sVec4_16AD98 d(c1 * c0, c1 * s0, -s1, 0.0f);
    self->Position = Sub_16AD98(self->Target, Scale_16AD98(d, k));
    cCameraJitterer_updatePositionalJitter(p + 0x70, self->r68);
    cCameraJitterer_updateRollJitter(p + 0x70, self->r6C);
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
extern "C" void cScriptRelativeAlgorithm_snapCurrent(cScriptCtlVirt2* self)
{
    self->v03(0);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_update);
#ifdef SKIP_ASM
struct sVec4_B180 {
    float x, y, z, w;
    sVec4_B180() {}
    sVec4_B180(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

static inline sVec4_B180 mtxMulVec_B180(char* m, const sVec4_B180& v)
{
    sVec4_B180 r;
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

static inline sVec4_B180 Scale_B180(const sVec4_B180& v, float s)
{
    sVec4_B180 r;
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

static inline sVec4_B180 Add_B180(const sVec4_B180& a, const sVec4_B180& b)
{
    sVec4_B180 r;
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

static inline sVec4_B180 Sub_B180(const sVec4_B180& a, const sVec4_B180& b)
{
    sVec4_B180 r;
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

static inline sVec4_B180 vec_B180(char* p)
{
    return *(sVec4_B180*)p;
}

struct sVec2_B180 {
    float x, y;
    sVec2_B180(float ax, float ay) : x(ax), y(ay) {}
};

static inline float Length_B180(const sVec2_B180& v)
{
    return Sqrt_B180(v.x * v.x + v.y * v.y);
}

struct sScrCam_B180 {
    float View;             // 0x0
    float Near;               // 0x4
    float Far;               // 0x8
    char padC[0x14];
    sVec4_B180 Target;       // 0x20
    sVec4_B180 Position;    // 0x30
    float Roll;            // 0x40
    float Heading;              // 0x44
    float Pitch;              // 0x48
    float ox, oy, oz;       // 0x4C
    float dist;             // 0x58
    float r5C;              // 0x5C
    float r60;              // 0x60
    float r64;              // 0x64
    float r68;              // 0x68
    float r6C;              // 0x6C
};

struct sVEp_B180 {
    short delta;
    short index;
    sVec4_B180* (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4_B180 Normalize_B180(const sVec4_B180& v)
{
    sVec4_B180 r;
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
static inline float Length4_B180(const sVec4_B180& v)
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
 * (func_0027C6A0, the entity's +0x20) gives Near (+0x4) and Far (+0x8), and func_0027C6F8 reads this frame's channel
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
 * @param p  the cScriptRelativeAlgorithm (this)
 */
extern "C" void cScriptRelativeAlgorithm_update(char* p)
{
    sScrCam_B180* self = (sScrCam_B180*)p;
    char* cam = *(char**)(*(char**)(*(char**)(p + 0x14) + 0x14) + 0x8);
    float* f = func_0027C6A0(cam);
    char* rider = cSSXScriptEngine_GetSubjectRider(ScriptEngine, ((signed char*)f)[0x1D], *(int*)(cam + 8));
    if (rider == 0) {
        rider = *(char**)((char*)SSXApp->Game->AI + 0x28);
    }
    self->Near = f[0];
    self->Far = f[1];
    func_0027C6F8(cam, &self->ox);
    self->View = self->r64 * 0.01745329424738884f;
    self->Roll = self->r60 * 0.01745329424738884f + *(float*)(*(char**)(p + 0x14) + 0x70);
    float pitchAngle;
    float headingAngle;
    float k;
    if (((signed char*)f)[0x1E] == 0) {
        sVec4_B180 v = vec_B180(rider + 0x1B0);
        sVec4_B180* pv = &v;
        float a = self->dist * 0.01745329424738884f;
        pitchAngle = Atan2_B180(v.z, Length_B180(sVec2_B180(v.x, v.y))) - a;
        float b = self->r5C * 0.01745329424738884f;
        headingAngle = b + Atan2_B180(pv->y, pv->x);
        k = self->oz;
    } else {
        pitchAngle = self->dist * 0.01745329424738884f;
        headingAngle = self->r5C * 0.01745329424738884f;
        k = self->oz;
    }
    char* obj = rider + 0x6C0;
    char* g = *(char**)(p + 0x14);
    pitchAngle += *(float*)(g + 0x74);
    headingAngle += *(float*)(g + 0x78);
    sVec4_B180 rot = *(sVec4_B180*)(*(char**)(rider + 0x780) + 0x140);
    sVEp_B180* vt = *(sVEp_B180**)obj;
    sVec4_B180* rp = vt[5].fn(obj + vt[5].delta);
    self->Target = Add_B180(Add_B180(*rp, Scale_B180(vec_B180(rider + 0x1B0), self->ox * rot.y)),
                         Scale_B180(vec_B180(rider + 0x1C0), self->oy * rot.x));
    float s0, c0, s1, c1;
    func_0031BE50(&s0, &c0, headingAngle);
    func_0031BE50(&s1, &c1, pitchAngle);
    sVec4_B180 d(c1 * c0, c1 * s0, -s1, 0.0f);
    sVec4_B180 t = Sub_B180(self->Target, Scale_B180(d, k));
    sVec4_B180 nt = Add_B180(Scale_B180(t, f[5]), Scale_B180(self->Position, 1.0f - f[5]));
    nt.w = 1.0f;
    sVec4_B180 dv = Sub_B180(nt, self->Position);
    float maxd = f[6];
    if (maxd < Length4_B180(dv) * 59.999996185302734f) {
        self->Position = Add_B180(self->Position, Scale_B180(Normalize_B180(dv), maxd * 0.01666666753590107f));
        self->Position.w = 1.0f;
    } else {
        self->Position = nt;
    }
    sVec4_B180 t2 = Sub_B180(self->Target, self->Position);
    self->Heading = Atan2_B180(t2.y, t2.x);
    float h = Length_B180(sVec2_B180(t2.x, t2.y));
    self->Pitch = Atan2_B180(t2.z, h);
    cCameraJitterer_updatePositionalJitter(p + 0x70, self->r68);
    cCameraJitterer_updateRollJitter(p + 0x70, self->r6C);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptRelativeAlgorithm_getPose);
#ifdef SKIP_ASM
struct sV_B7B8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_B7B8 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPair_B7B8 {
    sV_B7B8 a;
    sV_B7B8 b;
};

extern sM_B7B8 D_004FF1A0_B7B8 __asm__("t4mat_Identity");
extern sV_B7B8 D_004FF140_B7B8 __asm__("t4vect_XAxis");
extern sV_B7B8 D_004FF150_B7B8 __asm__("t4vect_YAxis");
extern sV_B7B8 D_004FF160_B7B8 __asm__("t4vect_ZAxis");
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" sPair_B7B8 func_0031B748_B7B8(sM_B7B8* m) __asm__("func_0031B748");
extern "C" sV_B7B8 func_00160228_B7B8(char* p, float a, float b) __asm__("func_00160228");
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
static inline void vu0TransMtx_B7B8(sM_B7B8* m, const sV_B7B8& v)
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
static inline sV_B7B8 Add_B7B8(const sV_B7B8& a, const sV_B7B8& b)
{
    sV_B7B8 r;
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
    func_0031BE50(s, c, angle);
}

static inline void RotAxis_B7B8(sM_B7B8* m, const sV_B7B8& axis, float angle)
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
 * (+0x44) and Pitch (+0x48) (func_00160228, On Tour cCameraJitterer::getPositionalJitter), then rotates it about
 * t4vect_XAxis by Roll (+0x40) plus cCameraJitterer_getRollJitter, about t4vect_YAxis by -Pitch and about t4vect_ZAxis
 * by Heading, and splits the matrix into a position and a rotation quaternion with func_0031B748. The four script
 * algorithms share this body; only the jitterer offset differs.
 *
 * @param p  the cScriptRelativeAlgorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" sPair_B7B8 cScriptRelativeAlgorithm_getPose(char* p)
{
    sM_B7B8 m;
    vu0CopyMtx_B7B8(&m, &D_004FF1A0_B7B8);
    vu0TransMtx_B7B8(&m, Add_B7B8(*(sV_B7B8*)(p + 0x30), func_00160228_B7B8(p + 0x70, *(float*)(p + 0x44), *(float*)(p + 0x48))));
    RotAxis_B7B8(&m, D_004FF140_B7B8, *(float*)(p + 0x40) + cCameraJitterer_getRollJitter(p + 0x70));
    RotAxis_B7B8(&m, D_004FF150_B7B8, -*(float*)(p + 0x48));
    RotAxis_B7B8(&m, D_004FF160_B7B8, *(float*)(p + 0x44));
    return func_0031B748_B7B8(&m);
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

extern "C" void func_0016BEC0(float* dist)
{
    *dist = Clamp_0016BEC0(*dist, -10000.0f, 10000.0f);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BEE8__FPv);
#ifdef SKIP_ASM
void* func_0016BEE8(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BEF8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_0016BEF8(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif


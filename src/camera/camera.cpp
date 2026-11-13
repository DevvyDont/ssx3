#include "common.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("camera/camera", cCamera_resetChaseControllerSwitches);
#ifdef SKIP_ASM
struct sCam162138;
extern "C" void func_00162138(sCam162138* self);
extern void* D_004A28A8;

extern "C" void cCamera_resetChaseControllerSwitches(void)
{
    void* cam = *(void**)((char*)D_004A28A8 + 0x84);
    int* chase = *(int**)((char*)cam + 0x84);
    if (chase != 0) {
        func_00162138(*(sCam162138**)(*(char**)((char*)chase + 4) + 0xA8));
        if (*chase >= 2) {
            func_00162138(*(sCam162138**)(*(char**)((char*)chase + 8) + 0xA8));
        }
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_cCamera);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0015D698(void* self);
extern "C" void* func_00176D68(void* self);
extern "C" void* cManualCameraController_cManualCameraController(void* self, void* cam);
extern "C" void* cChaseCameraController_cChaseCameraController(void* self, int idx, int a);
extern "C" void* cScriptCameraController_cScriptCameraController(void* self, int idx, void* cam);
extern void* D_0045B888[];
extern const char D_0045B748[];
extern const char D_0045B760[];
extern const char D_0045B778[];
// PORT: cQuad128 (the unit's 128-bit TImode typedef) is a 16-byte register copy.
extern cQuad128 D_004FF160;
extern cQuad128 D_004FF130;

extern "C" void* cCamera_cCamera(void* self, int idx)
{
    func_0015D698(self);
    *(void***)((char*)self + 0x90) = D_0045B888;
    func_00176D68((char*)self + 0xC0);
    *(void**)((char*)self + 0xA4) = cManualCameraController_cManualCameraController(cMemMan_alloc(0x20, D_0045B748, 0, 0), self);
    *(void**)((char*)self + 0xA8) = cChaseCameraController_cChaseCameraController(cMemMan_alloc(0x44, D_0045B760, 0, 0), idx, (int)self);
    *(void**)((char*)self + 0xAC) = cScriptCameraController_cScriptCameraController(cMemMan_alloc(0x20, D_0045B778, 0, 0), idx, self);
    *(int*)((char*)self + 0xB0) = -1;
    *(int*)((char*)self + 0xB4) = 0;
    *(int*)((char*)self + 0x454) = 0;
    *(int*)((char*)self + 0x458) = 0;
    *(int*)((char*)self + 0x45C) = 0;
    *(int*)((char*)self + 0x450) = 0;
    *(int*)((char*)self + 0x460) = 0;
    *(cQuad128*)((char*)self + 0x470) = D_004FF160;
    *(cQuad128*)((char*)self + 0x490) = D_004FF130;
    *(int*)((char*)self + 0x4A4) = 1;
    *(int*)((char*)self + 0x4A0) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015DD88);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0045B888[];
extern void* D_0045B8B0[];
void func_0015CD60(void* p);
void func_00176DA8_2(void* self, int flags) __asm__("func_00176DA8__FPv");

class cCamPartK2 {
public:
    char pad[0x14];
    // vptr at 0x14
    virtual ~cCamPartK2();
};

// PORT: func_00176DA8__FPv is a destructor taking (self, flags); bind the 2-arg form.
extern "C" void func_0015DD88(void* self, int flags)
{
    *(void***)((char*)self + 0x90) = D_0045B888;
    func_0015CD60(*(cCamPartK2**)((char*)self + 0xA4));
    func_0015CD60(*(cCamPartK2**)((char*)self + 0xA8));
    func_0015CD60(*(cCamPartK2**)((char*)self + 0xAC));
    delete *(cCamPartK2**)((char*)self + 0xA4);
    delete *(cCamPartK2**)((char*)self + 0xA8);
    delete *(cCamPartK2**)((char*)self + 0xAC);
    func_00176DA8_2((char*)self + 0xC0, 2);
    *(void***)((char*)self + 0x90) = D_0045B8B0;
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_init);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cManualCameraController_setCurrentAlgorithmType(void* self, int msg);
extern "C" void func_00161A60(void* self, int* p);
struct sCam162060;
extern "C" void func_00162060(sCam162060* self, int mode, int arg, float blend);
void func_00162310(void* self, int val);
void* func_0015E030(void* self);
extern void* D_004A28A8;
extern const char D_0045B790[];
extern void* D_0045B7A8[];

struct sCamCtrl_15DE60
{
    char pad_0x00[0xC];
    int viewport;   // 0xC
};

struct sCamera_15DE60
{
    char pad_0x00[0xC];
    float f0C;      // 0xC
    float f10;      // 0x10
    float f14;      // 0x14
    int index;      // 0x18
    char pad_0x1C[0xA4 - 0x1C];
    sCamCtrl_15DE60* manual;   // 0xA4
    sCamCtrl_15DE60* chase;    // 0xA8
    sCamCtrl_15DE60* script;   // 0xAC
    int fB0;        // 0xB0
    int fB4;        // 0xB4
    char pad_0xB8[0x454 - 0xB8];
    int f454;       // 0x454
    int f458;       // 0x458
    int f45C;       // 0x45C
    int f460;       // 0x460
};

struct sCamWorld_15DE60
{
    char pad_0x00[0xC];
    char* riders;   // 0xC
};

struct sApp_15DE60
{
    char pad_0x00[0x84];
    sCamWorld_15DE60* world;   // 0x84
    char pad_0x88[0xB0 - 0x88];
    int viewports[4];          // 0xB0
};

struct sRiderRef_15DE60
{
    void** vt;
    int rider;
};

extern "C" void cCamera_init(sCamera_15DE60* self, int idx, float a, float b, float c)
{
    self->f0C = a;
    self->f10 = b;
    self->f14 = c;
    self->index = idx;
    self->f460 = 0;
    self->manual->viewport = ((sApp_15DE60*)D_004A28A8)->viewports[0];
    cManualCameraController_setCurrentAlgorithmType(self->manual, 0x51);
    self->chase->viewport = ((sApp_15DE60*)D_004A28A8)->viewports[idx];
    sRiderRef_15DE60* p = (sRiderRef_15DE60*)operator new(8, D_0045B790, 0, 0);
    p->vt = D_0045B7A8;
    p->rider = *(int*)(((sApp_15DE60*)D_004A28A8)->world->riders + (idx << 2) + 0x28);
    func_00161A60(self->chase, (int*)p);
    func_00162060((sCam162060*)self->chase, 0x4C, 0, 1.0f);
    func_00162310((char*)self + 0xC0, (int)p);
    func_0015E030(self);
    self->f454 = 0;
    self->f458 = 0;
    self->f45C = 0;
    switch (idx) {
    case 0:
        self->fB0 = 0;
        break;
    case 1:
        self->fB0 = 1;
        break;
    }
    self->fB4 = 0;
    self->script->viewport = ((sApp_15DE60*)D_004A28A8)->viewports[0];
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015DF98);
#ifdef SKIP_ASM
class cCamVObj_0015DF98 {
public:
    char pad[0x14];
    // vptr at 0x14
    virtual void v01();
    virtual void v02();
};

extern "C" void* func_0015E668(void* self);

extern "C" void func_0015DF98(void* self)
{
    (*(cCamVObj_0015DF98**)((char*)self + 0xA0))->v02();
    func_0015E668(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015DFD8);
#ifdef SKIP_ASM
extern "C" void func_0015DFD8(void* self, int mode)
{
    switch (mode) {
    case 1:
        *(int*)((char*)self + 0xA0) = *(int*)((char*)self + 0xA4);
        break;
    case 2:
        *(int*)((char*)self + 0xA0) = *(int*)((char*)self + 0xA8);
        break;
    case 3:
        *(int*)((char*)self + 0xA0) = *(int*)((char*)self + 0xAC);
        break;
    }
}
#endif

extern "C" void* func_00166F28(void*);

//100%
INCLUDE_ASM("camera/camera", func_0015E030__FPv);
#ifdef SKIP_ASM
void* func_0015E030(void* self)
{
    return func_00166F28((char*)self + 0xc0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015E050);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// A transform as position + rotation quaternion, returned by value (func_0031B748 builds one from a matrix).
struct sXf {
    t4Vector pos;
    t4Vector rot;
};

extern t4Matrix D_004C53A0_E050 __asm__("D_004C53A0");

struct sObj_E050 {
    char pad[0x390];
} __attribute__((aligned(16)));

extern "C" void* func_00176D68(void* self);
extern "C" void func_00166640(void* self, int a1);
extern "C" void func_00166F90(void* self);
extern "C" sXf func_001673A0_E050(void* a1) __asm__("func_001673A0");
void func_00176DA8_E050(void* self, int flags) __asm__("func_00176DA8__FPv");

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_E050(t4Matrix* dst, const t4Matrix* src)
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

// PORT: PS2-only VU0 inline asm (unit quaternion -> 3x3 rotation, row 3 = (0,0,0,1)).
static inline void vu0QuatToMtx_E050(t4Matrix* m, const t4Vector* q)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf0, 0x30(%0)\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "sqc2       $vf11, 0x10(%0)\n"
        "sqc2       $vf12, 0x20(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "r"(q)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, m = m * b).
static inline void vu0MulEqMtx_E050(t4Matrix* m, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_E050(t4Matrix* m, const t4Vector& v)
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

extern "C" void func_0015E050_impl(char* self, char* a1, char* a2) __asm__("func_0015E050");
extern "C" void func_0015E050_impl(char* self, char* a1, char* a2)
{
    sObj_E050 o;
    func_00176D68(&o);
    *(cQuad128*)((char*)&o + 0x20) = *(cQuad128*)a2;
    *(cQuad128*)((char*)&o + 0x40) = *(cQuad128*)a1;
    *(float*)((char*)&o + 0x4C) = 1.0f;
    *(float*)((char*)&o + 0x2C) = 1.0f;
    func_00166640(&o, 1);
    func_00166F90(&o);
    sXf out = func_001673A0_E050(&o);
    t4Vector* q = (t4Vector*)(self + 0x30);
    t4Matrix* m = (t4Matrix*)(self + 0x40);
    *(sXf*)(self + 0x20) = out;
    vu0CopyMtx_E050(m, &D_004C53A0_E050);
    t4Matrix rot;
    vu0QuatToMtx_E050(&rot, q);
    vu0MulEqMtx_E050(m, &rot);
    t4Vector p;
    p.x = *(float*)(self + 0x20);
    p.y = *(float*)(self + 0x24);
    p.z = *(float*)(self + 0x28);
    out.rot.x = -p.x;
    out.rot.y = -p.y;
    out.rot.z = -p.z;
    out.pos.x = -p.x;
    out.pos.y = -p.y;
    out.pos.z = -p.z;
    out.pos.w = 1.0f;
    vu0TransMtx_E050(m, out.pos);
    out.pos.x = *(float*)(self + 0x48);
    out.pos.y = *(float*)(self + 0x58);
    out.pos.z = *(float*)(self + 0x68);
    out.pos.w = 0.0f;
    *(t4Vector*)(self + 0x80) = out.pos;
    func_00176DA8_E050(&o, 2);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015E2A8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sCamLag_15E2A8 {
    float f0, f4, f8, fC;
    struct {
        float a, b, c, d;
    } group[3];             // 0x10
    float f40, f44, f48, f4C;
};
extern sCamLag_15E2A8 D_004C53E0;
extern int D_004C5430[5];

// Resets the chase camera's tuning constants.
extern "C" void func_0015E2A8(void)
{
    D_004C53E0.f0 = 0.05f;
    D_004C53E0.f4 = 0.01f;
    D_004C53E0.f8 = 0.4f;
    D_004C53E0.fC = 20000.0f;
    D_004C5430[0] = 0;
    D_004C53E0.group[0].a = 0.02f;
    D_004C53E0.group[0].b = 0.01366f;
    D_004C53E0.group[0].c = 0.07f;
    D_004C53E0.group[0].d = -1.0f;
    D_004C5430[1] = 1;
    D_004C53E0.group[1].a = 0.02f;
    D_004C53E0.group[1].b = 0.01366f;
    D_004C53E0.group[1].c = 0.08f;
    D_004C53E0.group[1].d = -1.0f;
    D_004C5430[2] = 1;
    D_004C53E0.group[2].a = 0.02f;
    D_004C53E0.group[2].b = 0.01366f;
    D_004C53E0.group[2].c = 0.09f;
    D_004C53E0.group[2].d = -1.0f;
    D_004C5430[3] = 1;
    D_004C53E0.f40 = 0.1f;
    D_004C53E0.f44 = 0.03f;
    D_004C53E0.f48 = 1.4f;
    D_004C53E0.f4C = -1.0f;
    D_004C5430[4] = 1;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015E360);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004A28A8;
struct sCamShake_E360 {
    float f0;
    float f4;
    float weight;   // 0x8
    float range;    // 0xC
};
// View of the tuning block initialised by func_0015E2A8 as one entry per shake type.
extern sCamShake_E360 D_004C53E0_E360[] __asm__("D_004C53E0");

#define MIN_E360(a, b) ((a) <= (b) ? (a) : (b))
#define MAX_E360(a, b) ((b) <= (a) ? (a) : (b))

// Requests a camera shake of 'type' with strength 'amount' at 'dist' from the camera; keeps the strongest request.
// Replays ignore types 1-4.
extern "C" void func_0015E360(char* self, int type, float amount, float dist)
{
    int state = **(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
    int replay = state != 0 && state < 10;
    if (replay && (unsigned int)(type - 1) < 4) {
        return;
    }
    float scale;
    float range = D_004C53E0_E360[type].range;
    if (0.0f < dist && 0.0f < range) {
        float t = 2.0f - (dist + dist) / range;
        scale = MAX_E360(MIN_E360(t, 1.0f), 0.0f);
    } else {
        scale = 1.0f;
    }
    float v = amount * scale * D_004C53E0_E360[type].weight;
    if (*(float*)(self + 0x454) < v) {
        *(int*)(self + 0x450) = type;
        *(float*)(self + 0x454) = v;
    }
    *(int*)(self + 0x458) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015E460);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.
template <class F> struct sVEntry {
    short delta;
    short index;
    F* fn;
};

// The camera base matrix (set up by func_001673F8), followed by the camera-shake tuning
// table: one entry per shake type, reset by func_0015E2A8, plus each type's selector in D_004C5430.
struct sCamShake {
    float base;     // passed to func_001656B0_E460 / func_00165938
    float spread;
    float weight;   // scales the requested amount
    float range;    // distance at which the shake fades out
};

extern sCamShake D_004C53E0_E460[5] __asm__("D_004C53E0");

extern int D_004C5430[5];

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_15E460(const t4Vector& v)
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

extern int D_004A2A50;
extern "C" void func_00165938(char* self, int flag, int sel, float base, float spread, float r);
extern "C" void func_001656B0_E460(char* self, float base, float spread, float r) __asm__("func_001656B0");
extern "C" void func_0015E360_E460(char* self, int type, float amount, float dist) __asm__("func_0015E360");

extern "C" void func_0015E460(char* self)
{
    char* tgt = *(char**)(*(char**)(self + 0xA8) + 0x20);
    sVEntry<float(void*)>* vf = *(sVEntry<float(void*)>**)tgt;
    float spd = vf[0x11].fn(tgt + vf[0x11].delta);
    if (((D_004A2A50 & 1) ^ 1) != 0 && spd > 0.10000000149011612f) {
        sVEntry<t4Vector(void*)>* vv = *(sVEntry<t4Vector(void*)>**)tgt;
        t4Vector v = vv[3].fn(tgt + vv[3].delta);
        float a = Length_15E460(v) * 0.035999998450279236f;
        float k;
        if (60.0f <= a) {
            if (a <= 120.0f)
                k = (a - 60.0f) / 60.0f;
            else
                k = 1.0f;
        } else {
            k = 0.0f;
        }
        if (spd > 0.699999988079071f)
            func_0015E360_E460(self, 3, k, 0.0f);
        else if (spd > 0.4000000059604645f)
            func_0015E360_E460(self, 2, k, 0.0f);
        else
            func_0015E360_E460(self, 1, k, 0.0f);
    }
    int idx = *(int*)(self + 0x450);
    float f21 = D_004C53E0_E460[idx].base;
    float f22 = D_004C53E0_E460[idx].spread;
    int sel = D_004C5430[idx];
    char* p = self + 0xC0;
    float t = *(float*)(self + 0x454);
    if (*(int*)(self + 0x458) != 0 && *(int*)(self + 0x45C) == 0)
        func_001656B0_E460(p, f21, f22, t);
    func_00165938(p, *(int*)(self + 0x458), sel, f21, f22, t);
    *(int*)(self + 0x45C) = *(int*)(self + 0x458);
    *(int*)(self + 0x458) = 0;
    *(int*)(self + 0x454) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015E668);
#ifdef SKIP_ASM
// cCamera::updateCamMat(void) (SSX On Tour PSP symbols: all_in_one_camera.obj / camera.cpp).
#include "bx/math3d.h"

extern "C" void func_0015EE00(void*);                                   // cCamera::avoidGround
extern void func_0015E460_E668(void*) __asm__("func_0015E460");         // cCamera::applyCameraShake
extern "C" void func_00166640(void* self, int a1);                      // cLookAtChaseAlgorithm::calcPitchHeading(bool)
extern "C" void func_00166F90(void* self);                              // cLookAtChaseAlgorithm::calcPose
extern "C" void func_002ED490(int, float, float, float);                // cRenderStateMan::update
extern "C" float func_002EE3B8(int);                                    // cRenderStateMan::DrawDistance
extern t4Vector D_004FF120_E668 __asm__("D_004FF120");                  // t4vect::Zero
extern t4Vector D_004FF130_E668 __asm__("D_004FF130");                  // t4vect::Origin
extern t4Matrix D_004C53A0_E668 __asm__("D_004C53A0");                  // CamAxisFlip
extern float D_004A3B68;                                                // cRenderStateMan::m_farPlane

// EA's quaternion: the ctor takes w first.
struct tQuaternion_E668 {
    float x, y, z, w;
    tQuaternion_E668() {}
    tQuaternion_E668(const float& aw, const float& ax, const float& ay, const float& az) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

struct tPose_E668 {
    t4Vector pos;
    tQuaternion_E668 orient;
    void setIdentity() { orient = tQuaternion_E668(1.0f, 0.0f, 0.0f, 0.0f); pos = D_004FF130_E668; }
};

struct t3Vector_E668 {
    float x, y, z;
    t3Vector_E668() {}
    t3Vector_E668(float ax, float ay, float az) { x = ax; y = ay; z = az; }
    t3Vector_E668(const t4Vector& v) { x = v.x; y = v.y; z = v.z; }
    t3Vector_E668 operator-() const { return t3Vector_E668(-x, -y, -z); }
};

// t4Vector with EA's constructors (bx/math3d.h's t4Vector is a POD view).
struct t4Vec_E668 : t4Vector {
    t4Vec_E668(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
    t4Vec_E668(const t3Vector_E668& v, const float& aw) { x = v.x; y = v.y; z = v.z; w = aw; }
};

extern "C" tPose_E668 func_001673A0_E668(void* sub) __asm__("func_001673A0");   // cLookAtChaseAlgorithm::getPose

struct cCameraAlgorithm_E668 {
    float fov;      // 0x0
    float nearZ;    // 0x4
    float farZ;     // 0x8
    int type;       // 0xC
    virtual void v0();
    virtual tPose_E668 getPose();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual t4Vector getTarget();
};

struct cCameraAlgoListNode_E668 {
    cCameraAlgorithm_E668* algo;    // 0x0
    float lensWeight;               // 0x4
    int pad8;
    float weight;                   // 0xC
    int pad10;
    cCameraAlgoListNode_E668* next; // 0x14
};

struct cCameraAlgoList_E668 {
    cCameraAlgoListNode_E668* head;
    int length;
};

struct cCameraController_E668 {
    int pad[2];
    cCameraAlgoList_E668* list;
};

struct cCamera_E668 {
    float fov;                          // 0x0
    float nearZ;                        // 0x4
    float farZ;                         // 0x8
    float maxFov;                       // 0xC
    float minClip;                      // 0x10
    float maxClip;                      // 0x14
    int id;                             // 0x18
    int pad1C;
    tPose_E668 pose;                    // 0x20
    t4Matrix viewMat;                   // 0x40
    t4Vector viewDir;                   // 0x80
    int pad90[4];
    cCameraController_E668* controller; // 0xA0
    int padA4;
    cCameraController_E668* prevController; // 0xA8
    int padAC[5];
    char lookAtChase[0x20];             // 0xC0 cLookAtChaseAlgorithm
    t4Vector blendTarget;               // 0xE0
    char padF0[0x10];
    t4Vector blendPos;                  // 0x100
};

// PORT: PS2-only VU0 inline asm (scalar * vector).
static inline t4Vector operator*(float s, const t4Vector& v)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector operator+(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void CopyMtx_E668(t4Matrix* dst, const t4Matrix* src)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, m = m * b).
static inline void MulEqMtx_E668(t4Matrix* m, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (unit quaternion -> rotation matrix, row 3 = identity).
static inline void QuatToMtx_E668(const tQuaternion_E668* q, t4Matrix* m)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf0, 0x30(%0)\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "sqc2       $vf11, 0x10(%0)\n"
        "sqc2       $vf12, 0x20(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "r"(q)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void Translate_E668(t4Matrix* m, const t4Vector& v)
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

static inline float bracket_E668(float lo, float v, float hi)
{
    return (lo <= v) ? (v <? hi) : lo; // PORT: g++ <? operator
}

extern "C" void func_0015E668_E668(cCamera_E668* self) __asm__("func_0015E668");
extern "C" void func_0015E668_E668(cCamera_E668* self)
{
    cCameraAlgoListNode_E668* algoNode;
    tPose_E668 tempPose;
    float totalWeight = 0.0f;

    self->pose.setIdentity();
    algoNode = self->controller->list->head;
    cCameraAlgorithm_E668* algo = algoNode->algo;
    bool listLengthOne = self->controller->list->length == 1;
    if (listLengthOne || algo->type == 0x4A) {
        self->pose = algo->getPose();
        if (self->controller == self->prevController) {
            self->blendPos = self->pose.pos;
            self->blendTarget = algo->getTarget();
        }
    } else {
        self->blendTarget = D_004FF120_E668;
        self->blendPos = D_004FF120_E668;
        for (algoNode = self->controller->list->head; algoNode; algoNode = algoNode->next) {
            if (algoNode->weight > 0.0001f) {
                totalWeight += algoNode->weight;
                float weightFactor = algoNode->weight / totalWeight;
                tempPose = algoNode->algo->getPose();
                t4Vector tempPosePos = tempPose.pos;
                t4Vector tempAlgoPos = self->blendPos;
                self->blendPos = weightFactor * tempPosePos + (1.0f - weightFactor) * tempAlgoPos;
                t4Vector algoTargetPos = algoNode->algo->getTarget();
                t4Vector tempAlgoTargetPos = self->blendTarget;
                self->blendTarget = weightFactor * algoTargetPos + (1.0f - weightFactor) * tempAlgoTargetPos;
            }
        }
        self->blendPos.w = 1.0f;
        self->blendTarget.w = 1.0f;
        func_00166640(self->lookAtChase, 1);
        func_00166F90(self->lookAtChase);
        self->pose = func_001673A0_E668(self->lookAtChase);
    }
    if (self->controller == self->prevController) {
        func_0015EE00(self);
        func_0015E460_E668(self);
        func_00166640(self->lookAtChase, 0);
        func_00166F90(self->lookAtChase);
        self->pose = func_001673A0_E668(self->lookAtChase);
    }
    CopyMtx_E668(&self->viewMat, &D_004C53A0_E668);
    t4Matrix quaternionMat;
    QuatToMtx_E668(&self->pose.orient, &quaternionMat);
    MulEqMtx_E668(&self->viewMat, &quaternionMat);
    Translate_E668(&self->viewMat, t4Vec_E668(-t3Vector_E668(self->pose.pos.x, self->pose.pos.y, self->pose.pos.z), 1.0f));
    self->viewDir = t4Vec_E668(self->viewMat.row[0].z, self->viewMat.row[1].z, self->viewMat.row[2].z, 0.0f);
    self->fov = 0.0f;
    self->nearZ = 0.0f;
    self->farZ = 0.0f;
    totalWeight = 0.0f;
    for (algoNode = self->controller->list->head; algoNode; algoNode = algoNode->next) {
        if (algoNode->lensWeight != 0.0f) {
            totalWeight += algoNode->lensWeight;
            float weightFactor = algoNode->lensWeight / totalWeight;
            self->fov = weightFactor * algoNode->algo->fov + (1.0f - weightFactor) * self->fov;
            self->nearZ = weightFactor * algoNode->algo->nearZ + (1.0f - weightFactor) * self->nearZ;
            self->farZ = weightFactor * algoNode->algo->farZ + (1.0f - weightFactor) * self->farZ;
        }
    }
    func_002ED490(self->id + 6, self->pose.pos.x, self->pose.pos.y, -99999.0f);
    self->farZ = bracket_E668(self->minClip, self->farZ, func_002EE3B8(self->id + 6));
    D_004A3B68 = self->farZ;
    self->fov = bracket_E668(0.0f, self->fov, self->maxFov);
    self->nearZ = bracket_E668(self->minClip, self->nearZ, self->maxClip);
    self->farZ = bracket_E668(self->minClip, self->farZ, self->maxClip);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015EC98);
#ifdef SKIP_ASM
struct sVec4_15EC98
{
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void func_003A9658(void* self, unsigned int i, sVec4_15EC98* v, float f);
extern "C" int func_003A96E0(void* self, int i, sVec4_15EC98* outPos, float* outT);
extern void* D_004A28A8;

struct sCacheSlot_15EC98
{
    int active;
    char pad[0x4C];
};

struct sWorld_15EC98
{
    char pad[0x250];
    sCacheSlot_15EC98 slots[2];
};
extern char** D_004A47B8;

// PORT: g++ min operator (<?), removed in GCC 4.3.
static inline float clamp_15EC98(float v, float lo, float hi)
{
    float r;
    if (v >= lo)
        r = v <? hi;
    else
        r = lo;
    return r;
}

extern "C" void func_0015EC98(char* self)
{
    float r;
    if (*(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + 0x10) == 1)
        r = *(float*)(self + 0x8);
    else
        r = clamp_15EC98(*(float*)(self + 0x8), 0.0f, 15000.0f);
    float r15 = r * 1.5f;
    float far = clamp_15EC98(r * 0.6666666865348816f, 15000.0f, 30000.0f);
    char** sys = D_004A47B8;
    ((sWorld_15EC98*)*sys)->slots[*(int*)(self + 0xB0)].active = 1;
    func_003A9658(*sys + 0x10, *(int*)(self + 0xB0), (sVec4_15EC98*)(self + 0x20), r15);
    *(int*)(self + 0xB4) = 0;
    sVec4_15EC98 pos;
    float t;
    if (func_003A96E0(*sys + 0x10, *(int*)(self + 0xB0), &pos, &t)) {
        if (far <= t || r <= t)
            *(int*)(self + 0xB4) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015EDC8);
#ifdef SKIP_ASM
class cCamVObj_0015EDC8 {
public:
    char pad[0x14];
    // vptr at 0x14 (g++ 2.95 places it after the class's own data)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

extern "C" void func_0015EDC8(void* self, int value)
{
    char* obj = *(char**)((char*)self + 0xA8);
    *(int*)(*(char**)(obj + 0x20) + 0x4) = value;
    (*(cCamVObj_0015EDC8**)((char*)self + 0xA8))->v04();
}
#endif

INCLUDE_ASM("camera/camera", func_0015EE00);

//100%
INCLUDE_ASM("camera/camera", func_0015F568);
#ifdef SKIP_ASM
extern "C" void* cRider_getHipPosition(void* dst, void* self);

extern "C" void* func_0015F568(void* self, void* a1)
{
    cRider_getHipPosition(self, *(void**)((char*)a1 + 0x4));
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F598);
#ifdef SKIP_ASM
extern "C" void* func_0015F598(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x120);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F5B0);
#ifdef SKIP_ASM
struct sQuad_0015F5B0 {
    float v[4];
} __attribute__((aligned(16)));

class cCamVObj_0015F5B0 {
public:
    // vptr at 0x0
    virtual void v01();
    virtual sQuad_0015F5B0* v02();
};

extern "C" void* func_0015F5B0(void* self, void* a1)
{
    char* p = *(char**)((char*)a1 + 0x4);
    *(sQuad_0015F5B0*)self = *((cCamVObj_0015F5B0*)(p + 0x6C0))->v02();
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F5F8);
#ifdef SKIP_ASM
extern "C" void* func_0015F5F8(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x1b0);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F610);
#ifdef SKIP_ASM
extern "C" void* func_0015F610(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x1a0);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F628);
#ifdef SKIP_ASM
extern "C" void* func_0015F628(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x1c0);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F640__FPv);
#ifdef SKIP_ASM
float func_0015F640(void* self)
{
    return *(float*)((char*)*(void**)((char*)self + 0x4) + 0x1f0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F650);
#ifdef SKIP_ASM
extern "C" void* func_0015F650(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x370);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F668);
#ifdef SKIP_ASM
extern "C" void* func_0015F668(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x380);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F680__FPv);
#ifdef SKIP_ASM
void* func_0015F680(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x77c) + 0x130;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F690__FPv);
#ifdef SKIP_ASM
void* func_0015F690(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x77c) + 0x140;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6A0__FPv);
#ifdef SKIP_ASM
float func_0015F6A0(void* self)
{
    return *(float*)((char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x788) + 0x98);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6B0);
#ifdef SKIP_ASM
extern "C" float func_0015F6B0(void* self)
{
    void* p = *(void**)((char*)self + 0x4);
    void* p2 = *(void**)((char*)p + 0x788);
    return *(float*)((char*)p2 + 0x98) - *(float*)((char*)p2 + 0xa0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6C8);
#ifdef SKIP_ASM
extern "C" float func_0015F6C8(void* self)
{
    void* p = *(void**)((char*)self + 0x4);
    void* p2 = *(void**)((char*)p + 0x788);
    return *(float*)((char*)p2 + 0x9c) - *(float*)((char*)p2 + 0xa0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6E0__FPv);
#ifdef SKIP_ASM
void* func_0015F6E0(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x788) + 0x40;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6F0__FPv);
#ifdef SKIP_ASM
void* func_0015F6F0(void* self)
{
    return (char*)*(void**)((char*)self + 0x4) + 0x3c0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F700__FPv);
#ifdef SKIP_ASM
float func_0015F700(void* self)
{
    return *(float*)((char*)*(void**)((char*)self + 0x4) + 0x2fc);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F710__FPv);
#ifdef SKIP_ASM
float func_0015F710(void* self)
{
    return *(float*)((char*)*(void**)((char*)self + 0x4) + 0x220);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F720);
#ifdef SKIP_ASM
extern "C" int func_0015F720(void* self)
{
    void* a = *(void**)((char*)self + 0x4);
    void* b = *(void**)((char*)a + 0x788);
    int s = *(int*)((char*)b + 0xAC);
    int r = 0;
    if (s == 1 || s == 3) {
        r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F750__FPv);
#ifdef SKIP_ASM
int func_0015F750(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x4) + 0x438);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F760__FPv);
#ifdef SKIP_ASM
float func_0015F760(void* self)
{
    return *(float*)((char*)*(void**)((char*)self + 0x4) + 0x5a4);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F770__FPv);
#ifdef SKIP_ASM
int func_0015F770(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x4) + 0x5a8);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F780);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern "C" float func_0031C128(float x);

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_15F780(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_15F780(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_15F780(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" float func_0015F780(void* self)
{
    char* o = *(char**)((char*)*(void**)((char*)self + 4) + 0x788);
    t4Vector a = *(t4Vector*)(o + 0x20);
    t4Vector b = *(t4Vector*)(o + 0x10);
    int ok = 0;
    if (Dot_15F780(a, a) > 0.0f) {
        if (Dot_15F780(b, b) > 0.0f)
            ok = 1;
    }
    if (ok) {
        t4Vector n = Scale_15F780(b, -1.0f);
        float l = Sqrt_15F780(Dot_15F780(a, a) * Dot_15F780(n, n));
        if (l < 0.0010000000474974513f)
            return 0.0f;
        float q = Dot_15F780(a, n) / l;
        float c;
        if (q >= -1.0f)
            c = q <? 1.0f;
        else
            c = -1.0f;
        return 1.5707963705062866f - func_0031C128(c);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F908__FPv);
#ifdef SKIP_ASM
int func_0015F908(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x4) + 0x5ac);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F918__FPv);
#ifdef SKIP_ASM
void* func_0015F918(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x788) + 0x20;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F928__FPv);
#ifdef SKIP_ASM
void* func_0015F928(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x788) + 0x10;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_cBasicCameraAlgorithm);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
extern void* D_0045BD78[];
// PORT: cQuad128 (the unit's 128-bit TImode typedef) is a 16-byte register copy.
extern cQuad128 D_004FF130;
extern "C" float func_00167E30(void* self);

extern "C" void* cBasicCameraAlgorithm_cBasicCameraAlgorithm(void* self)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    *(float*)self = func_00167E30(self);
    *(int*)((char*)self + 0xC) = 0x4E;
    *(float*)((char*)self + 0x4) = 10.0f;
    *(float*)((char*)self + 0x8) = 30000.0f;
    *(void***)((char*)self + 0x10) = D_0045BD78;
    *(int*)((char*)self + 0x30) = 0;
    *(int*)((char*)self + 0x34) = 0;
    cQuad128 v = D_004FF130;
    *(int*)((char*)self + 0xC) = 0x4F;
    *(cQuad128*)((char*)self + 0x20) = v;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F9B8);
#ifdef SKIP_ASM
struct sV_15F9B8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_15F9B8 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPair_15F9B8 {
    sV_15F9B8 a;
    sV_15F9B8 b;
};

extern sM_15F9B8 D_004FF1A0_15F9B8 __asm__("D_004FF1A0");
extern sV_15F9B8 D_004FF150_15F9B8 __asm__("D_004FF150");
extern sV_15F9B8 D_004FF160_v15F9B8 __asm__("D_004FF160");
extern "C" void func_0031BE50(float* sout, float* cout, float x);
extern "C" sPair_15F9B8 func_0031B748_15F9B8(sM_15F9B8* m) __asm__("func_0031B748");

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_15F9B8(sM_15F9B8* dst, const sM_15F9B8* src)
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
static inline void vu0MulMtx_15F9B8(sM_15F9B8* dst, const sM_15F9B8* a, const sM_15F9B8* b)
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
static inline void vu0TransMtx_15F9B8(sM_15F9B8* m, const sV_15F9B8& v)
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

static inline void SinCos_15F9B8(float* s, float* c, float angle)
{
    func_0031BE50(s, c, angle);
}

static inline void RotAxis_15F9B8(sM_15F9B8* m, const sV_15F9B8& axis, float angle)
{
    sM_15F9B8 r;
    float s;
    float c;
    float one = 1.0f;
    SinCos_15F9B8(&s, &c, angle);
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
    vu0MulMtx_15F9B8(m, m, &r);
}

extern "C" sPair_15F9B8 func_0015F9B8(char* src)
{
    sM_15F9B8 m;
    vu0CopyMtx_15F9B8(&m, &D_004FF1A0_15F9B8);
    vu0TransMtx_15F9B8(&m, *(sV_15F9B8*)(src + 0x20));
    RotAxis_15F9B8(&m, D_004FF150_15F9B8, -*(float*)(src + 0x30));
    RotAxis_15F9B8(&m, D_004FF160_v15F9B8, *(float*)(src + 0x34));
    return func_0031B748_15F9B8(&m);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cManualCameraAlgorithm_cManualCameraAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cBasicCameraAlgorithm_cBasicCameraAlgorithm(void* self);
extern void* D_0045BD10[];

extern "C" void* cManualCameraAlgorithm_cManualCameraAlgorithm(void* self)
{
    cBasicCameraAlgorithm_cBasicCameraAlgorithm(self);
    *(int*)((char*)self + 0xC) = 0x51;
    *(void***)((char*)self + 0x10) = D_0045BD10;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015FD48__FPv);
#ifdef SKIP_ASM
void func_0015FD48(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015FD50);
#ifdef SKIP_ASM
extern void* D_004A28A8;
// PORT: cInput_getValue returns a float (input axis value, $f0).
extern "C" float cInput_getValue(int pad, int axis);
extern "C" float func_0031BF60(float x);
extern "C" float func_0031C040(float x);

struct sVec_0015FD50 {
    float x, y, z, w;
    sVec_0015FD50() {}
    sVec_0015FD50(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    sVec_0015FD50& operator+=(const sVec_0015FD50& b);
} __attribute__((aligned(16)));

struct sFreeCam_0015FD50 {
    char pad0[0x20];
    sVec_0015FD50 pos;  // 0x20
    float pitch;        // 0x30
    float yaw;          // 0x34
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_0015FD50 fcScale_0015FD50(const sVec_0015FD50& v, float s)
{
    sVec_0015FD50 r;
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

// PORT: PS2-only VU0 inline asm (vector add, in place).
inline sVec_0015FD50& sVec_0015FD50::operator+=(const sVec_0015FD50& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(*this)
        : "m"(*this), "m"(b)
        : "memory");
    return *this;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec_0015FD50 fcAdd_0015FD50(const sVec_0015FD50& a, const sVec_0015FD50& b)
{
    sVec_0015FD50 r;
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

static inline float fcClamp_0015FD50(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_0015FD50(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

// PORT: g++ `>?` / `<?` (max/min) operators.
extern "C" void func_0015FD50(sFreeCam_0015FD50* self)
{
    int pad = *(int*)(*(char**)(*(char**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + 0x4) + 0xA4) + 0xC);
    float speed = cInput_getValue(pad, 0x34);
    float ry = cInput_getValue(pad, 0x31);
    float rx = cInput_getValue(pad, 0x30);
    float mf = cInput_getValue(pad, 0x32);
    float ms = cInput_getValue(pad, 0x33);
    float rs = speed >? 2.0f;
    float pitch = fcClamp_0015FD50(self->pitch + rs * (ry * 0.6283185482025146f) * 0.01666666753590107f,
                                   -1.5707963705062866f, 1.5707963705062866f);
    float yaw = self->yaw + rs * (rx * 0.6283185482025146f) * 0.01666666753590107f;
    yaw -= ffloor_0015FD50(yaw * 0.15915493667125702f) * 6.2831854820251465f;
    sVec_0015FD50 fwd(func_0031C040(pitch) * func_0031C040(yaw), func_0031C040(pitch) * func_0031BF60(yaw),
                      func_0031BF60(pitch), 0.0f);
    sVec_0015FD50 right(func_0031BF60(yaw), -func_0031C040(yaw), 0.0f, 0.0f);
    sVec_0015FD50 pos = fcAdd_0015FD50(sVec_0015FD50(self->pos), fcScale_0015FD50(fwd, speed * (mf * 25.0f)));
    pos += fcScale_0015FD50(right, speed * (ms * -25.0f));
    self->pitch = pitch;
    self->yaw = yaw;
    self->pos = pos;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cEditorCameraAlgorithm_cEditorCameraAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cBasicCameraAlgorithm_cBasicCameraAlgorithm(void* self);
extern void* D_0045BCA8[];

extern "C" void* cEditorCameraAlgorithm_cEditorCameraAlgorithm(void* self)
{
    cBasicCameraAlgorithm_cBasicCameraAlgorithm(self);
    *(int*)((char*)self + 0xC) = 0x50;
    *(void***)((char*)self + 0x10) = D_0045BCA8;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015FFF0);
#ifdef SKIP_ASM
extern "C" void* func_0015FFF0(void* self)
{
    *(float*)((char*)self + 0x28) = 0.05000000074505806f;
    *(float*)((char*)self + 0x2C) = 0.05000000074505806f;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(float*)((char*)self + 0x20) = 1.0499999523162842f;
    *(float*)((char*)self + 0x24) = 1.0499999523162842f;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160028);
#ifdef SKIP_ASM
int BXrand();

struct sShake_160028
{
    float x;        // 0x0
    float y;        // 0x4
    float pad8;
    float nx;       // 0xC
    float ny;       // 0x10
    float pad14;
    float amount;   // 0x18
    float pad1C;
    float timer;    // 0x20
    float pad24;
    float period;   // 0x28
};

// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_160028()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

extern "C" void func_00160028(sShake_160028* self, float amount)
{
    self->amount = amount;
    if (amount != 0.0f) {
        if (self->timer > self->period) {
            self->x = self->nx;
            self->y = self->ny;
            self->nx = (randf_160028() - 0.5f) * 2.0f;
            self->ny = (randf_160028() - 0.5f) * 2.0f;
            self->timer = 0.0f;
        } else {
            self->timer = self->timer + 0.01666666753590107f;
        }
    } else {
        self->x = 0.0f;
        self->y = 0.0f;
        self->nx = 0.0f;
        self->ny = 0.0f;
        self->timer = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160130);
#ifdef SKIP_ASM
int BXrand();

struct sRollShake_160130
{
    float pad0[2];
    float cur;      // 0x8
    float pad0C[2];
    float next;     // 0x14
    float pad18;
    float amount;   // 0x1C
    float pad20;
    float timer;    // 0x24
    float pad28;
    float period;   // 0x2C
};

// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_160130()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

// PORT: abs.s via inline asm (as an SDK math-header fabsf would).
static inline float fabs_160130(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" void func_00160130(sRollShake_160130* self, float amount)
{
    self->amount = amount;
    if (amount != 0.0f) {
        if (self->timer > self->period) {
            self->cur = self->next;
            while (fabs_160130(self->cur - self->next) < 0.5f)
                self->next = (randf_160130() - 0.5f) * 2.0f;
            self->timer = 0.0f;
        } else {
            self->timer = self->timer + 0.01666666753590107f;
        }
    } else {
        self->timer = 0.0f;
        self->next = 0.0f;
        self->cur = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160228);
#ifdef SKIP_ASM
extern "C" float func_0031BF60(float x);
extern "C" float func_0031C040(float x);

struct sVec_00160228 {
    float x, y, z, w;
    sVec_00160228() {}
    sVec_00160228(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern sVec_00160228 D_004FF120_v160228 __asm__("D_004FF120");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_00160228 Scale_00160228(const sVec_00160228& v, float s)
{
    sVec_00160228 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec_00160228 Add_00160228(const sVec_00160228& a, const sVec_00160228& b)
{
    sVec_00160228 r;
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec_00160228 Cross_00160228(const sVec_00160228& a, const sVec_00160228& b)
{
    sVec_00160228 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

extern "C" sVec_00160228 func_00160228(char* p, float a, float b)
{
    float zero = 0.0f;
    if (*(float*)(p + 0x18) == zero) return D_004FF120_v160228;
    sVec_00160228 fwd(func_0031C040(b) * func_0031C040(a), func_0031C040(b) * func_0031BF60(a), -func_0031BF60(b), zero);
    sVec_00160228 right(func_0031BF60(a), -func_0031C040(a), zero, zero);
    sVec_00160228 up = Cross_00160228(fwd, right);
    float k = *(float*)(p + 0x20) / *(float*)(p + 0x28);
    float sx = k * (*(float*)(p + 0xC) - *(float*)(p + 0x0));
    float sy = k * (*(float*)(p + 0x10) - *(float*)(p + 0x4));
    float sz = *(float*)(p + 0x18) * 10.0f;
    sVec_00160228 res = Scale_00160228(Add_00160228(Scale_00160228(right, sx), Scale_00160228(up, sy)), sz);
    return res;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001603F0);
#ifdef SKIP_ASM
extern "C" float func_001603F0(void* self)
{
    float s = *(float*)((char*)self + 0x1C);
    if (s == 0.0f)
        return 0.0f;
    return s * (*(float*)((char*)self + 0x24) / *(float*)((char*)self + 0x2C) * 0.007853982038795948f)
             * (*(float*)((char*)self + 0x14) - *(float*)((char*)self + 0x8));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160438);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045BAD0[];

extern "C" void* func_00160438(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0xB;
    *(void***)((char*)self + 0x10) = D_0045BAD0;
    return self;
}
#endif

extern void* D_0045BAD0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00160478__FPv);
#ifdef SKIP_ASM
void* func_00160478(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045BAD0;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001604A0__FPv);
#ifdef SKIP_ASM
float func_001604A0(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001604B0__FPvT0);
#ifdef SKIP_ASM
void func_001604B0(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001604B8);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" float cInput_getValue(int pad, int axis);
extern "C" void func_00162568(void* self, struct sCamInfo_001607E0* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);
extern "C" void func_001668B8(void* self, void* out);
extern float D_004A127C;
extern float D_004A1280;
extern float D_004A1284;

struct sVec_1604B8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sCamInfo_1604B8 {
    char pad0[0x10];
    sVec_1604B8 pos;    // 0x10
    char pad20[0x50];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_1604B8 Scale_1604B8(const sVec_1604B8& v, float s)
{
    sVec_1604B8 r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec_1604B8 Sub_1604B8(const sVec_1604B8& a, const sVec_1604B8& b)
{
    sVec_1604B8 r;
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

// PORT: PS2-only VU0 inline asm (in-place v /= s).
static inline void DivEq_1604B8(sVec_1604B8& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1604B8(const sVec_1604B8& v)
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

// PORT: PS2-only inline asm (float absolute value).
static inline float Abs_1604B8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` (min) operator.
static inline float Clamp_1604B8(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

struct sCamYP_1604B8 {
    char pad0[0x50];
    float yaw;          // 0x50
    float pitch;        // 0x54
};

struct sGame_1604B8 {
    char pad0[0x84];
    char* f84;          // 0x84
    char pad88[0xB0 - 0x88];
    int pads[4];        // 0xB0
};

extern "C" void func_001604B8(char* self)
{
    sCamInfo_1604B8 info;
    func_00162568(self, (sCamInfo_001607E0*)&info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    sGame_1604B8* g = (sGame_1604B8*)D_004A28A8;
    int pad = g->pads[*(int*)(*(char**)(g->f84 + 0x28) + 0x28)];
    float ax = cInput_getValue(pad, 0xBE);
    float ay = cInput_getValue(pad, 0xBF);
    float az = -cInput_getValue(pad, 0xC0);
    ax *= Abs_1604B8(ax);
    ay *= Abs_1604B8(ay);
    az *= Abs_1604B8(az);
    ax *= 0.05000000074505806f;
    D_004A127C += ax;
    if (D_004A127C > 6.2831854820251465f) {
        D_004A127C = D_004A127C - 6.2831854820251465f;
    } else if (D_004A127C < 0.0f) {
        D_004A127C = D_004A127C + 6.2831854820251465f;
    }
    float k = 0.05000000074505806f;
    D_004A1280 += ay * k;
    D_004A1280 = Clamp_1604B8(D_004A1280, 0.0f, 1.5079644918441772f);
    ((sCamYP_1604B8*)self)->pitch = D_004A1280;
    ((sCamYP_1604B8*)self)->yaw = D_004A127C;
    sVec_1604B8 dir;
    func_001668B8(self, &dir);
    DivEq_1604B8(dir, Length_1604B8(dir));
    D_004A1284 = Clamp_1604B8(D_004A1284 * (az * k + 1.0f), 150.0f, 850.0f);
    *(sVec_1604B8*)(self + 0x20) = info.pos;
    *(sVec_1604B8*)(self + 0x40) = Sub_1604B8(*(sVec_1604B8*)(self + 0x20), Scale_1604B8(dir, D_004A1284));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001606D8);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);

struct sCamVEntryIntA {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sCamVEntryVoidA {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_001606D8(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        func_00166550(self, 100.0f, 100.0f);
        sCamVEntryVoidA* vt2 = *(sCamVEntryVoidA**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160760);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045BA58[];

extern "C" void* func_00160760(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x29;
    *(void***)((char*)self + 0x10) = D_0045BA58;
    *(int*)((char*)self + 0x390) = 0;
    return self;
}
#endif

extern void* D_0045BA58[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_001607A0__FPv);
#ifdef SKIP_ASM
void* func_001607A0(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045BA58;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001607C8__FPv);
#ifdef SKIP_ASM
float func_001607C8(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001607D8__FPvT0);
#ifdef SKIP_ASM
void func_001607D8(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001607E0);
#ifdef SKIP_ASM
extern "C" float func_0031BF60(float x);
extern "C" float func_0031C040(float x);

struct sVec_001607E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sCamInfo_001607E0 {
    char pad0[0x20];
    sVec_001607E0 dir;  // 0x20
    char pad30[0x38];
} __attribute__((aligned(16)));

// asm-label views: camera.cpp declares these globals as cQuad128
extern sVec_001607E0 D_004FF140_v1607E0 __asm__("D_004FF140");
extern sVec_001607E0 D_004FF160_v1607E0 __asm__("D_004FF160");
extern "C" void func_00162568(void* self, sCamInfo_001607E0* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);

struct sVEv_001607E0 {
    short delta;
    short index;
    sVec_001607E0 (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_001607E0 cmScale_001607E0(const sVec_001607E0& v, float s)
{
    sVec_001607E0 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec_001607E0 cmAdd_001607E0(const sVec_001607E0& a, const sVec_001607E0& b)
{
    sVec_001607E0 r;
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec_001607E0 cmCross_001607E0(const sVec_001607E0& a, const sVec_001607E0& b)
{
    sVec_001607E0 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float cmLength_001607E0(const sVec_001607E0& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void cmDivEq_001607E0(sVec_001607E0& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

static inline void cmNormalize_001607E0(sVec_001607E0& v)
{
    cmDivEq_001607E0(v, cmLength_001607E0(v));
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_001607E0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline sVec_001607E0 cmGet_001607E0(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEv_001607E0* e = &(*(sVEv_001607E0**)o)[slot];
    return e->fn(o + e->delta);
}

extern "C" void func_001607E0(char* self)
{
    sCamInfo_001607E0 info;
    func_00162568(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    float eps = 0.0010000000474974513f;
    sVec_001607E0 dir;
    if (__builtin_fabsf(info.dir.x) > eps || __builtin_fabsf(info.dir.y) > eps) {
        dir = info.dir;
    } else {
        dir = cmGet_001607E0(self, 4);
        if (__builtin_fabsf(dir.x) < eps && __builtin_fabsf(dir.y) < eps) {
            dir = D_004FF140_v1607E0;
        }
    }
    dir.z = 0.0f;
    sVec_001607E0 side = cmCross_001607E0(D_004FF160_v1607E0, dir);
    cmNormalize_001607E0(dir);
    cmNormalize_001607E0(side);
    float* ang = (float*)(self + 0x390);
    *ang += 0.061352409422397614f;
    *ang -= ffloor_001607E0(*ang * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
    float r = 216.23182678222656f;
    *(sVec_001607E0*)(self + 0x20) = cmAdd_001607E0(cmGet_001607E0(self, 1), cmScale_001607E0(D_004FF160_v1607E0, 0.0f));
    *(sVec_001607E0*)(self + 0x40) = cmAdd_001607E0(
        cmAdd_001607E0(cmAdd_001607E0(*(sVec_001607E0*)(self + 0x20), cmScale_001607E0(dir, func_0031C040(*ang) * r)),
                       cmScale_001607E0(side, func_0031BF60(*ang) * r)),
        cmScale_001607E0(D_004FF160_v1607E0, -0.5597707033157349f));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160AE8);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);

extern "C" void func_00160AE8(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        func_00166550(self, 100.0f, 100.0f);
        sCamVEntryVoidA* vt2 = *(sCamVEntryVoidA**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160B70);
#ifdef SKIP_ASM
extern void* D_0045B9E0[];
extern "C" void* func_00162318(void* self);

extern "C" void* func_00160B70(void* self, float f)
{
    func_00162318(self);
    *(float*)((char*)self + 0x390) = f;
    *(void***)((char*)self + 0x10) = D_0045B9E0;
    *(int*)((char*)self + 0x394) = 0;
    return self;
}
#endif

extern void* D_0045B9E0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00160BB8__FPv);
#ifdef SKIP_ASM
void* func_00160BB8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045B9E0;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160BE0__FPv);
#ifdef SKIP_ASM
float func_00160BE0(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160BF0__FPvT0);
#ifdef SKIP_ASM
void func_00160BF0(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160BF8);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec_001607E0 cmSub_00160BF8(const sVec_001607E0& a, const sVec_001607E0& b)
{
    sVec_001607E0 r;
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

extern "C" void func_00160BF8(char* self)
{
    sCamInfo_001607E0 info;
    func_00162568(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    float eps = 0.0010000000474974513f;
    sVec_001607E0 dir;
    if (__builtin_fabsf(info.dir.x) > eps || __builtin_fabsf(info.dir.y) > eps) {
        dir = info.dir;
    } else {
        dir = cmGet_001607E0(self, 4);
        if (__builtin_fabsf(dir.x) < eps && __builtin_fabsf(dir.y) < eps) {
            dir = D_004FF140_v1607E0;
        }
    }
    dir.z = 0.0f;
    sVec_001607E0 side = cmCross_001607E0(D_004FF160_v1607E0, dir);
    cmNormalize_001607E0(dir);
    cmNormalize_001607E0(side);
    *(sVec_001607E0*)(self + 0x20) = cmAdd_001607E0(cmGet_001607E0(self, 1), cmScale_001607E0(D_004FF160_v1607E0, 0.0f));
    if (*(int*)(self + 0x394)) {
        *(sVec_001607E0*)(self + 0x20) = cmAdd_001607E0(cmScale_001607E0(*(sVec_001607E0*)(self + 0x3B0), 0.6399999856948853f),
                                                    cmScale_001607E0(*(sVec_001607E0*)(self + 0x20), 0.36000001430511475f));
    }
    *(sVec_001607E0*)(self + 0x3B0) = *(sVec_001607E0*)(self + 0x20);
    float r = 385.91839599609375f;
    *(sVec_001607E0*)(self + 0x40) = cmAdd_001607E0(
        cmAdd_001607E0(cmAdd_001607E0(*(sVec_001607E0*)(self + 0x20), cmScale_001607E0(dir, func_0031C040(*(float*)(self + 0x390)) * r)),
                       cmScale_001607E0(side, func_0031BF60(*(float*)(self + 0x390)) * r)),
        cmScale_001607E0(D_004FF160_v1607E0, 53.241573333740234f));
    if (*(int*)(self + 0x394)) {
        sVec_001607E0 d = cmSub_00160BF8(*(sVec_001607E0*)(self + 0x20), *(sVec_001607E0*)(self + 0x40));
        sVec_001607E0 e = cmAdd_001607E0(cmScale_001607E0(*(sVec_001607E0*)(self + 0x3A0), 0.9700000286102295f),
                                         cmScale_001607E0(d, 0.029999971389770508f));
        *(sVec_001607E0*)(self + 0x40) = cmSub_00160BF8(*(sVec_001607E0*)(self + 0x20), e);
    }
    *(sVec_001607E0*)(self + 0x3A0) = cmSub_00160BF8(*(sVec_001607E0*)(self + 0x20), *(sVec_001607E0*)(self + 0x40));
    *(int*)(self + 0x394) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160FD0);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);

extern "C" void func_00160FD0(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        func_00166550(self, 100.0f, 100.0f);
        *(int*)((char*)self + 0x394) = 0;
        sCamVEntryVoidA* vt2 = *(sCamVEntryVoidA**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161060);
#ifdef SKIP_ASM
extern void* D_0045B968[];
extern "C" void func_0031D5E8(void* spline);
// PORT: func_0031D700 is declared (void*) but forwards a second argument (the key count) in $a1.
void func_0031D700_2(void* spline, int n) __asm__("func_0031D700__FPv");
extern "C" void func_0031D7E0(void* spline, float x, float y);
extern "C" float func_0031DEE0(void* spline, float x);

struct sVec_161060 {
    float x, y, z, w;
    sVec_161060() {}
    sVec_161060(float a, float b, float c, const float& d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec_161060 Sub_161060(const sVec_161060& a, const sVec_161060& b)
{
    sVec_161060 r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_161060(const sVec_161060& v)
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

extern "C" void* func_00161060(char* self, float* p0, float* p1, float* p2, float* p3, float dur, float a, float b,
                               float c)
{
    func_00162318(self);
    *(void***)(self + 0x10) = D_0045B968;
    char* sx = self + 0x390;
    char* sy = self + 0x39C;
    char* sz = self + 0x3A8;
    func_0031D5E8(sx);
    func_0031D5E8(sy);
    func_0031D5E8(sz);
    *(float*)(self + 0x3C0) = a;
    *(float*)(self + 0x3C4) = b;
    *(float*)(self + 0x0) = c;
    *(int*)(self + 0xC) = 0x4B;
    func_0031D700_2(sx, 4);
    func_0031D700_2(sy, 4);
    func_0031D700_2(sz, 4);
    float t1 = dur * 0.33333298563957214f;
    float t2 = dur * 0.6666669845581055f;
    func_0031D7E0(sx, 0.0f, p0[0]);
    func_0031D7E0(sx, t1, p1[0]);
    func_0031D7E0(sx, t2, p2[0]);
    func_0031D7E0(sx, dur, p3[0]);
    func_0031D7E0(sy, 0.0f, p0[1]);
    func_0031D7E0(sy, t1, p1[1]);
    func_0031D7E0(sy, t2, p2[1]);
    func_0031D7E0(sy, dur, p3[1]);
    func_0031D7E0(sz, 0.0f, p0[2]);
    func_0031D7E0(sz, t1, p1[2]);
    func_0031D7E0(sz, t2, p2[2]);
    func_0031D7E0(sz, dur, p3[2]);
    *(float*)(self + 0x3B4) = dur;
    *(float*)(self + 0x3B8) = 0.0f;
    float total = 0.0f;
    sVec_161060 prev(func_0031DEE0(sx, total), func_0031DEE0(sy, total), func_0031DEE0(sz, total), 1.0f);
    for (int i = 1; i < 101; i++) {
        float t = (dur * (float)i * 0.009999999776482582f) <? dur;
        sVec_161060 cur(func_0031DEE0(self + 0x390, t), func_0031DEE0(self + 0x39C, t), func_0031DEE0(self + 0x3A8, t), 1.0f);
        sVec_161060 d = Sub_161060(cur, prev);
        total += Length_161060(d);
        prev = cur;
    }
    *(float*)(self + 0x3BC) = total * 0.8999999761581421f / dur;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161370);
#ifdef SKIP_ASM
extern void* D_0045B968[];
extern "C" void func_0031D618(void* p, int flags);
// PORT: func_00162458 is the base deleting dtor (self, flags); the unit declares it with one arg.
void func_00162458_dtor(void* self, int flags) __asm__("func_00162458");

extern "C" void func_00161370(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045B968;
    func_0031D618((char*)self + 0x3A8, 2);
    func_0031D618((char*)self + 0x39C, 2);
    func_0031D618((char*)self + 0x390, 2);
    func_00162458_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001613D8__FPv);
#ifdef SKIP_ASM
float func_001613D8(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001613E8__FPvT0);
#ifdef SKIP_ASM
void func_001613E8(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001613F0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// The same vector with a constructor taking w by const reference (its callers compile differently from sVec4).
struct sVec4R : t4Vector
{
    sVec4R() {}
    sVec4R(float ax, float ay, float az, const float& aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern t4Vector D_004FF160_13F0 __asm__("D_004FF160");

extern "C" float func_0031DEE0(void* spline, float x);


// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4R Sub_13F0(const sVec4R& a, const sVec4R& b)
{
    sVec4R r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_13F0(const sVec4R& v)
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline sVec4R ScaleP_13F0(float s, const sVec4R* v)
{
    sVec4R r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4R Scale_13F0(float s, const sVec4R& v)
{
    sVec4R r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4R Add_13F0(const sVec4R& a, const sVec4R& b)
{
    sVec4R r;
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

static inline sVec4R Get_13F0(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<sVec4R(void*)>* e = &(*(sVEntry<sVec4R(void*)>**)o)[slot];
    return e->fn(o + e->delta);
}

extern "C" void func_001613F0(char* self)
{
    char* sx = self + 0x390;
    char* sy = self + 0x39C;
    char* sz = self + 0x3A8;
    float t = (*(float*)(self + 0x3B8) + 0.01666666753590107f) <? *(float*)(self + 0x3B4);
    float speed = *(float*)(self + 0x3BC) * 0.01666666753590107f;
    {
        sVec4R prev = *(sVec4R*)(self + 0x40);
        sVec4R cur(func_0031DEE0(sx, t), func_0031DEE0(sy, t), func_0031DEE0(sz, t), 1.0f);
        sVec4R d = Sub_13F0(cur, prev);
        float len = Length_13F0(d);
        if (0.0f < len) {
            float t0 = *(float*)(self + 0x3B8);
            float nt = (t0 + (t - t0) / (len / speed)) <? *(float*)(self + 0x3B4);
            *(float*)(self + 0x3B8) = nt;
            *(sVec4R*)(self + 0x40) = sVec4R(func_0031DEE0(sx, nt), func_0031DEE0(sy, nt), func_0031DEE0(sz, nt), 1.0f);
        }
    }
    *(sVec4R*)(self + 0x20) = Add_13F0(Add_13F0(Get_13F0(self, 1), ScaleP_13F0(*(float*)(self + 0x3C4), (const sVec4R*)&D_004FF160_13F0)),
                                          Scale_13F0(*(float*)(self + 0x3C0), Get_13F0(self, 4)));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161630);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);

struct sCamVEntryInt {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sCamVEntryVoid {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00161630(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryInt* vt = *(sCamVEntryInt**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        sCamVEntryVoid* vt2 = *(sCamVEntryVoid**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001616A8);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045BB48[];

extern "C" void* func_001616A8(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x7;
    *(void***)((char*)self + 0x10) = D_0045BB48;
    return self;
}
#endif

extern void* D_0045BB48[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_001616E8__FPv);
#ifdef SKIP_ASM
void* func_001616E8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045BB48;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161710__FPv);
#ifdef SKIP_ASM
float func_00161710(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161720__FPvT0);
#ifdef SKIP_ASM
void func_00161720(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161728);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF160_1728 __asm__("D_004FF160");

// The per-frame camera input record (0x70 bytes) that func_00162568_1728 fills from the target for the chase algorithms.
struct sCamInfo_1728 {
    t4Vector f00;               // 0x00
    t4Vector pos;               // 0x10
    t4Vector dir;               // 0x20
    t4Vector out;               // 0x30: written back by func_00162C78
    int mode;                   // 0x40
    float speed;                // 0x44: length of dir
    char pad48[4];
    float f4C;                  // 0x4C
    float f50;                  // 0x50
    float f54;                  // 0x54
    int f58;                    // 0x58
    int f5C;                    // 0x5C
    int f60;                    // 0x60
    int f64;                    // 0x64
};

extern "C" void func_00162568_1728(void* self, sCamInfo_1728* out, float a, float b, float c, float d,
                              float e, float f, float g, float h) __asm__("func_00162568");

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector cmAdd_001607E0_1728(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float cmLength_001607E0_1728(const t4Vector& v)
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

static inline t4Vector cmGet_001607E0_1728(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* e = &(*(sVEntry<t4Vector(void*)>**)o)[slot];
    return e->fn(o + e->delta);
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_00160BF8_1728(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_1728(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

extern "C" void func_00161728_v(char* self) __asm__("func_00161728");

extern "C" void func_00161728_v(char* self)
{
    sCamInfo_1728 info;
    func_00162568_1728(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    t4Vector* p20 = (t4Vector*)(self + 0x20);
    t4Vector* p40 = (t4Vector*)(self + 0x40);
    *p20 = cmAdd_001607E0_1728(cmGet_001607E0_1728(self, 1), cmScaleP_1728(&D_004FF160_1728, 13.495580673217773f));
    t4Vector d = cmSub_00160BF8_1728(*p20, *p40);
    float len = cmLength_001607E0_1728(d);
    if (0.0f < len) {
        *(float*)self = (531.6604614257812f / len <? 1.0f) * 0.7853981852531433f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161848);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF140;

extern t4Vector D_004FF160_1848 __asm__("D_004FF160");

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector cmAdd_001607E0_1848(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector cmGet_001607E0_1848(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* e = &(*(sVEntry<t4Vector(void*)>**)o)[slot];
    return e->fn(o + e->delta);
}

extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_1848b(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector plus vector at pointer).
static inline t4Vector cmAddP_1848(const t4Vector& a, const t4Vector* b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "o"(*b)
        : "memory");
    return r;
}

extern "C" void func_00161848_v(char* self, int id) __asm__("func_00161848");

extern "C" void func_00161848_v(char* self, int id)
{
    char* obj = *(char**)(*(char**)(self + 0x30) + 0x4) + 0x6C0;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        *(t4Vector*)(self + 0x40) = cmAddP_1848(cmAdd_001607E0_1848(cmGet_001607E0_1848(self, 1), cmScaleP_1848b(&D_004FF160_1848, 100.05000305175781f)), &D_004FF140);
        func_00166C60(self, id);
        func_00166550(self, 559.7440185546875f, 300.0f);
        sVEntry<void(void*)>* vt2 = *(sVEntry<void(void*)>**)(self + 0x10);
        vt2[5].fn(self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_HardReset);
#ifdef SKIP_ASM
struct cChaseCameraController_switchCamera_HardReset_flags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
};

extern int D_004A4DD0[2];

extern "C" void cChaseCameraController_switchCamera_HardReset(void* self, int i)
{
    *(int*)((char*)self + 0x1C) = 0x3D;
    *(int*)((char*)self + 0x38) = 0;
    ((cChaseCameraController_switchCamera_HardReset_flags*)((char*)self + 0x30))->b1 = 0; ((cChaseCameraController_switchCamera_HardReset_flags*)((char*)self + 0x30))->b0 = 0; ((cChaseCameraController_switchCamera_HardReset_flags*)((char*)self + 0x30))->b2 = 0;
    *(int*)((char*)self + 0x3C) = 0;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x2C) = 0x3D;
    *(int*)((char*)self + 0x24) = D_004A4DD0[i];
    *(int*)((char*)self + 0x28) = 0x3D;
    *(int*)((char*)self + 0x34) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cChaseCameraController_cChaseCameraController);
#ifdef SKIP_ASM
extern void* D_0045B908[];
extern "C" void* cCameraController_cCameraController(void* self, int a);
extern "C" void cChaseCameraController_switchCamera_HardReset(void* self, int idx);

extern "C" void* cChaseCameraController_cChaseCameraController(void* self, int idx, int a)
{
    cCameraController_cCameraController(self, a);
    *(void***)((char*)self + 0x14) = D_0045B908;
    *(int*)((char*)self + 0x0) = 2;
    *(int*)((char*)self + 0x4) = 1;
    *(int*)((char*)self + 0x20) = 0;
    cChaseCameraController_switchCamera_HardReset(self, idx);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161A10);
#ifdef SKIP_ASM
extern void* D_0045B908[];
void operator_delete(int* ptr);
extern "C" void cCameraController__cCameraController(void* self, int flags);

extern "C" void func_00161A10(void* self, int flags)
{
    *(void***)((char*)self + 0x14) = D_0045B908;
    operator_delete(*(int**)((char*)self + 0x20));
    cCameraController__cCameraController(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161A60);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00161A60(void* self, int* p)
{
    int* old = *(int**)((char*)self + 0x20);
    if (old != 0) {
        operator_delete(old);
    }
    *(int**)((char*)self + 0x20) = p;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161AA0);
#ifdef SKIP_ASM
extern "C" int func_00161AA0(void *arg0) {
    return (*(int *)((char*)((*(void **)((char*)(arg0) + (0x20)))) + (4)));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161AB0);
#ifdef SKIP_ASM
struct sCam162138;
extern "C" void func_00161EF0(void* self, int v);
extern "C" void func_001621A8(sCam162138* self);
extern "C" void func_00162218(sCam162138* self);
int func_0011FE98(void*);

struct sCamVec4_161AB0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

class cCamRider_161AB0
{
public:
    virtual void v01();
    virtual void v02();
    virtual sCamVec4_161AB0 getVelocity();
};

// PORT: VU0 macro-mode vector length; the PC port needs plain C.
static inline float vlength_161AB0(const sCamVec4_161AB0& v)
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

extern "C" void func_00161AB0(char* self)
{
    int mode = 0x3D;
    if (*(int*)(self + 0x1C) != mode && *(int*)(self + 0x18) != 0x4A)
        func_00161EF0(self, mode);
    *(int*)(self + 0x1C) = mode;
    float speed = vlength_161AB0((*(cCamRider_161AB0**)(self + 0x20))->getVelocity()) * 0.035999998450279236f;
    int boarding = func_0011FE98(*(void**)(*(char**)(self + 0x20) + 4)) == 5;
    if (*(int*)(self + 0x3C) == 0) {
        if (boarding) {
            *(int*)(self + 0x3C) = 1;
            func_001621A8((sCam162138*)self);
        }
    } else if (speed > 35.0f) {
        *(int*)(self + 0x3C) = 0;
        func_00162218((sCam162138*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_update);
#ifdef SKIP_ASM
struct sCam162060;
extern "C" void func_00162060(sCam162060* self, int mode, int arg, float blend);
extern int D_004A2A50;

class cCamAlgo_161BB8 {
public:
    char pad_0x00[0xC];
    int type;  // 0xC
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

struct sAlgoNode_161BB8 {
    cCamAlgo_161BB8* algo;    // 0x0
    float weight;             // 0x4
    float rate;               // 0x8
    float blend;              // 0xC
    sAlgoNode_161BB8* prev;   // 0x10
    sAlgoNode_161BB8* next;   // 0x14
};

struct sAlgoList_161BB8 {
    sAlgoNode_161BB8* head;
    int count;
};

extern "C" sAlgoNode_161BB8* func_0015CA50(sAlgoList_161BB8* list, sAlgoNode_161BB8* n);

static inline float Approach_161BB8(float cur, float target, float rate)
{
    if (target + rate < cur)
        return cur - rate;
    if (cur < target - rate)
        return cur + rate;
    return target;
}

static inline float ApproachZero_161BB8(float cur, float rate)
{
    if (rate < cur)
        return cur - rate;
    if (cur < -rate)
        return cur + rate;
    return 0.0f;
}

static inline void Smooth_161BB8(sAlgoNode_161BB8* n, float u)
{
    float u2 = u * u;
    float s = u2 * 3.0f - (u2 + u2) * u;
    n->weight = u;
    if (s > 1.0f)
        s = 1.0f;
    n->blend = s;
}

static inline int InRace_161BB8(int* st)
{
    return *st && *st < 10;
}

extern "C" void cChaseCameraController_update(char* self)
{
    if ((D_004A2A50 & 1) == 0) {
        func_00161AB0(self);
        sAlgoNode_161BB8* n = (*(sAlgoList_161BB8**)(self + 0x8))->head;
        n->algo->v09(0);
        float rate = n->rate;
        Smooth_161BB8(n, Approach_161BB8(n->weight, 1.0f, rate));
        n = n->next;
        while (n) {
            Smooth_161BB8(n, ApproachZero_161BB8(n->weight, rate));
            if (n->algo->type == 0x4A)
                n->weight = 0.0f;
            if (n->weight <= 0.0f || n->blend <= 0.0f) {
                n = func_0015CA50(*(sAlgoList_161BB8**)(self + 0x8), n);
            } else {
                n->algo->v09(1);
                n = n->next;
            }
        }
    }
    int* st = *(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
    int idle = InRace_161BB8(st) && st[0x484 / 4] == 0;
    if (idle)
        *(float*)(self + 0x38) = 0.0f;
    if (InRace_161BB8(st) && *(int*)(self + 0x2C) == 0x5D && *(int*)(self + 0x18) != *(int*)(self + 0x24)) {
        *(float*)(self + 0x38) += 0.01666666753590107f;
        if (*(float*)(self + 0x38) > 10.0f) {
            *(float*)(self + 0x38) = 0.0f;
            func_00162060((sCam162060*)self, *(int*)(self + 0x24), 0, 1.0f);
        }
    } else {
        *(float*)(self + 0x38) = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161E58);
#ifdef SKIP_ASM
extern "C" void cCameraAlgoList_insert(void* list, void* algo, int b, float w);
extern "C" void func_0015CB08(void* list);

extern "C" void func_00161E58(void* self, void* algo, float w)
{
    *(int*)((char*)self + 0x18) = *(int*)((char*)algo + 0xC);
    void* list = *(void**)((char*)self + 0x8);
    if (*(int*)((char*)list + 0x4) == 0) {
        cCameraAlgoList_insert(list, algo, 1, 1.0f);
        return;
    }
    if (w == 1.0f) {
        func_0015CB08(*(void**)((char*)self + 0x8));
    }
    cCameraAlgoList_insert(*(void**)((char*)self + 0x8), algo, 1, w);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161EF0);
#ifdef SKIP_ASM
extern int D_004A4DD0[2];
extern "C" void* func_0015D050(void*, int, int);

extern "C" void func_00161EF0(void* self, int v)
{
    *(int*)((char*)self + 0x24) = v;
    D_004A4DD0[*(int*)(*(char**)(*(char**)((char*)self + 0x20) + 0x4) + 0x870)] = v;
    if ((*(int*)((char*)self + 0x30) & 7) == 0) {
        func_0015D050(self, v, 0);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161F50);
#ifdef SKIP_ASM
struct sCam161F50 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int pad_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void* func_0015D050(void*, int, int);

extern "C" void func_00161F50(sCam161F50* self)
{
    int mode;
    self->b1 = 0;
    self->b0 = 0;
    mode = 0x42;
    if (!self->b2) {
        mode = self->field_0x24;
    }
    func_0015D050(self, mode, self->field_0x34);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161FA0);
#ifdef SKIP_ASM
extern "C" void func_00161FA0(void* self, int mode)
{
    int flags = *(int*)((char*)self + 0x30) | 2;
    *(int*)((char*)self + 0x30) = flags;
    if (mode == 0x4C) {
        mode = *(int*)((char*)self + 0x24);
        func_0015D050(self, mode, 0);
    } else if (mode == 0x5D) {
        if (flags & 1)
            func_0015D050(self, *(int*)((char*)self + 0x28), *(int*)((char*)self + 0x34));
        else if (flags & 4)
            func_0015D050(self, 0x42, 0);
        else
            func_0015D050(self, *(int*)((char*)self + 0x24), 0);
    } else {
        func_0015D050(self, mode, 0);
    }
    *(int*)((char*)self + 0x2C) = mode;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162060);
#ifdef SKIP_ASM
struct sCam162060 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int field_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void* cChaseCameraController_createChaseAlgorithmBlend(void*, int, int, float);

extern "C" void func_00162060(sCam162060* self, int mode, int arg, float blend)
{
    if (mode == 0x4C) {
        self->b0 = 0;
        mode = self->field_0x24;
    } else {
        self->field_0x28 = mode;
        self->b0 = 1;
        self->field_0x34 = arg;
    }
    int ok = !self->b1 ? 1 : self->field_0x2C == 0x5D;
    if (ok) {
        cChaseCameraController_createChaseAlgorithmBlend(self, mode, arg, blend);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001620D0);
#ifdef SKIP_ASM
struct sCam1620D0 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int field_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void func_0015DAC0();
extern "C" void* cChaseCameraController_createChaseAlgorithmBlend(void*, int, int, float);

extern "C" void func_001620D0(sCam1620D0* self)
{
    int mode = self->field_0x2C;
    self->b0 = 0;
    if (mode == 0x5D)
        mode = self->field_0x24;
    func_0015DAC0();
    cChaseCameraController_createChaseAlgorithmBlend(self, mode, 0, 1.0f);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162138);
#ifdef SKIP_ASM
struct sCam162138 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int pad_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
    int pad_0x38;
    int field_0x3C;
    int field_0x40;
};


extern "C" void func_00162138(sCam162138* self)
{
    self->b1 = 0;
    self->b0 = 0;
    self->b2 = 0;
    self->field_0x28 = self->field_0x24;
    self->field_0x3C = 0;
    self->field_0x40 = 0;
    self->field_0x34 = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162170);
#ifdef SKIP_ASM
extern "C" void* func_0015D050(void*, int, int);

extern "C" void* func_00162170(sCam162138* self)
{
    func_00162138(self);
    return func_0015D050(self, self->field_0x24, 0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001621A8);
#ifdef SKIP_ASM
int func_0011FE98(void*);

extern "C" void func_001621A8(sCam162138* self)
{
    self->b2 = 1;
    if (!self->b0 && !self->b1) {
        float t = func_0011FE98(*(void**)(*(char**)((char*)self + 0x20) + 4)) == 5 ? 1.2000000476837158f : 0.6000000238418579f;
        cChaseCameraController_createChaseAlgorithmBlend(self, 0x42, 0, t * 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162218);
#ifdef SKIP_ASM
extern "C" void func_00162218(sCam162138* self)
{
    self->b2 = 0;
    if (!self->b0 && !self->b1) {
        cChaseCameraController_createChaseAlgorithmBlend(self, self->field_0x24, 0, 0.0066666672937572f);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162258);
#ifdef SKIP_ASM
extern "C" void func_00162258(sCam162138* self)
{
    if (!self->b0) {
        cChaseCameraController_createChaseAlgorithmBlend(self, 0x44, 0, 0.016793444752693176f);
    }
}
#endif

extern "C" void* func_0015D050(void*, int, int);

//100%
INCLUDE_ASM("camera/camera", func_00162290__FPv);
#ifdef SKIP_ASM
void* func_00162290(void* self)
{
    return func_0015D050(self, *(int*)((char*)self + 0x24), 0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001622B0);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
extern void* D_0045BC38[];
extern "C" float func_00167E30(void* self);

extern "C" void* func_001622B0(void* self)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    *(float*)self = func_00167E30(self);
    *(int*)((char*)self + 0xC) = 0x4E;
    *(float*)((char*)self + 0x4) = 10.0f;
    *(float*)((char*)self + 0x8) = 30000.0f;
    *(void***)((char*)self + 0x10) = D_0045BC38;
    *(int*)((char*)self + 0xC) = 0x53;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162310__FPvi);
#ifdef SKIP_ASM
void func_00162310(void* self, int val)
{
    *(int*)((char*)self + 0x30) = val;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162318);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void* D_0045BBC0[];
extern cQuad128 D_004FF120_q162318 __asm__("D_004FF120");
extern "C" float func_00167D88(void);
// PORT: these take an unused self; they are called here with none.
float func_00167D98_0() __asm__("func_00167D98");
float func_00167DA8_0() __asm__("func_00167DA8");
struct sCamFilter;
extern "C" void func_00167DB8(sCamFilter* self, float value);

// Constructor: base ctor, five splines, the filter and default tuning.
extern "C" void* func_00162318(void* self)
{
    func_001622B0(self);
    char* p = (char*)self;
    sCamFilter* filter = (sCamFilter*)(p + 0x340);
    *(void***)(p + 0x10) = D_0045BBC0;
    func_0031D5E8(p + 0x304);
    func_0031D5E8(p + 0x310);
    func_0031D5E8(p + 0x31C);
    func_0031D5E8(p + 0x328);
    func_0031D5E8(p + 0x334);
    *(float*)(p + 0x380) = func_00167D88();
    *(float*)(p + 0x384) = func_00167D98_0();
    *(float*)(p + 0x388) = func_00167DA8_0();
    func_00167DB8(filter, 0.0f);
    *(cQuad128*)(p + 0x40) = D_004FF130;
    *(int*)(p + 0x54) = 0;
    *(int*)(p + 0xC) = 0x54;
    *(int*)(p + 0x50) = 0;
    *(int*)(p + 0x58) = 0;
    *(cQuad128*)(p + 0x90) = D_004FF120_q162318;
    *(float*)(p + 0x1D0) = 200.0f;
    *(int*)(p + 0x1D4) = 0;
    *(float*)(p + 0x1D8) = *(float*)(p + 0x54);
    *(float*)(p + 0x1DC) = 1.0f;
    *(float*)(p + 0x1E0) = 10.0f;
    *(float*)(p + 0x1E4) = *(float*)(p + 0x54);
    *(int*)(p + 0x2C0) = 5000;
    *(int*)(p + 0x2D0) = 0;
    *(int*)(p + 0x2D8) = 0;
    *(cQuad128*)(p + 0xC0) = D_004FF120_q162318;
    *(cQuad128*)(p + 0xD0) = D_004FF120_q162318;
    *(cQuad128*)(p + 0xE0) = D_004FF120_q162318;
    *(float*)p = 0.7853981852531433f;
    *(float*)(p + 0x388) = 0.0012785084545612335f;
    *(float*)(p + 0x384) = *(float*)(p + 0x54);
    *(float*)(p + 0x380) = 0.01139452401548624f;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162458);
#ifdef SKIP_ASM
extern void* D_0045BBC0[];
extern void* D_0045BDE0[];

// PORT: func_00162458 is the base deleting dtor (self, flags); the unit declares it with one arg
// (func_00162458_dtor is declared earlier in the unit with this asm label).
void func_00162458_dtor(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BBC0;
    func_0031D618((char*)self + 0x334, 2);
    func_0031D618((char*)self + 0x328, 2);
    func_0031D618((char*)self + 0x31C, 2);
    func_0031D618((char*)self + 0x310, 2);
    func_0031D618((char*)self + 0x304, 2);
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001624E8);
#ifdef SKIP_ASM
struct sCamVE1624E8 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sCamVE1624E8i {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001624E8(void* self, int a1)
{
    if (*(int*)((char*)self + 0x2F0) != 0) {
        sCamVE1624E8* vt = *(sCamVE1624E8**)((char*)self + 0x10);
        vt[4].fn((char*)self + vt[4].delta);
    }
    sCamVE1624E8* vt = *(sCamVE1624E8**)((char*)self + 0x10);
    vt[5].fn((char*)self + vt[5].delta);
    sCamVE1624E8i* vt2 = *(sCamVE1624E8i**)((char*)self + 0x10);
    vt2[8].fn((char*)self + vt2[8].delta, a1);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162560);
#ifdef SKIP_ASM
extern "C" void func_00162560(void) {

}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162568);
#ifdef SKIP_ASM
int func_0011FE98(void*);
extern sVec_001607E0 D_004FF120_v162568 __asm__("D_004FF120");

struct sVEi_162568 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sVEf_162568 {
    short delta;
    short index;
    float (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec_001607E0 cmSub_162568(const sVec_001607E0& a, const sVec_001607E0& b)
{
    sVec_001607E0 r;
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

// PORT: PS2-only VU0 inline asm (dot product).
static inline float cmDot_162568(const sVec_001607E0& a, const sVec_001607E0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (v / |v| via vrsqrt).
static inline sVec_001607E0 cmNormalized_162568(const sVec_001607E0& v)
{
    sVec_001607E0 r;
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

static inline int cmGetI_162568(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEi_162568* e = &(*(sVEi_162568**)o)[slot];
    return e->fn(o + e->delta);
}

static inline float cmGetF_162568(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEf_162568* e = &(*(sVEf_162568**)o)[slot];
    return e->fn(o + e->delta);
}

static inline sVec_001607E0 cmLerp_162568(const sVec_001607E0& a, const sVec_001607E0& b, float t)
{
    sVec_001607E0 r = cmAdd_001607E0(cmScale_001607E0(a, t), cmScale_001607E0(b, 1.0f - t));
    return r;
}

static inline sVec_001607E0 cmGetR_162568(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEv_001607E0* e = &(*(sVEv_001607E0**)o)[slot];
    sVec_001607E0 r = e->fn(o + e->delta);
    return r;
}

static inline void SetFrame_162568(char* out, const sVec_001607E0& pos, sVec_001607E0 dir, sVec_001607E0 vel)
{
    *(sVec_001607E0*)(out + 0x0) = pos;
    *(sVec_001607E0*)(out + 0x10) = vel;
    *(sVec_001607E0*)(out + 0x20) = dir;
}

extern "C" void func_00162568_impl(char* self, char* out, float a, float b, float c, float d, float e, float f,
                                   float g, float h) __asm__("func_00162568");
extern "C" void func_00162568_impl(char* self, char* out, float a, float b, float c, float d, float e, float f,
                                   float g, float h)
{
    sVec_001607E0 v = cmGet_001607E0(self, 3);
    int mode = func_0011FE98(*(void**)(*(char**)(self + 0x30) + 4));
    if (cmLength_001607E0(v) < 27.77777862548828f) {
        v = *(sVec_001607E0*)(self + 0x1B0);
    }
    *(sVec_001607E0*)(self + 0x1B0) = v;
    sVec_001607E0 w = v;
    w.z = 0.0f;
    sVec_001607E0 dz = cmSub_162568(v, w);
    sVec_001607E0 n = cmDot_162568(v, v) > 0.0f ? cmNormalized_162568(v) : D_004FF120_v162568;
    int k = cmGetI_162568(self, 20);
    float lo, hi, t0, t1;
    if ((unsigned)(k - 2) < 2 || k == 13) {
        lo = e;
        hi = f;
        t0 = g;
        t1 = h;
    } else {
        lo = a;
        hi = b;
        t0 = c;
        t1 = d;
    }
    float x = __builtin_fabsf(n.z);
    float frac;
    if (x >= t0) {
        if (x <= t1) {
            frac = (x - t0) / (t1 - t0);
        } else {
            frac = 1.0f;
        }
    } else {
        frac = 0.0f;
    }
    float r = lo + (hi - lo) * frac;
    if (mode == 1) {
        if (cmLength_001607E0(w) < 27.77777862548828f) {
            w = *(sVec_001607E0*)(self + 0x100);
        }
    }
    sVec_001607E0 nf = cmLerp_162568(*(sVec_001607E0*)(self + 0x100), w, r);
    *(sVec_001607E0*)(self + 0x100) = nf;
    sVec_001607E0 p = cmAdd_001607E0(nf, dz);
    *(int*)(out + 0x64) = mode == 4;
    *(int*)(out + 0x58) = mode == 2;
    *(int*)(out + 0x5C) = mode == 5;
    *(int*)(out + 0x60) = mode == 1;
    *(int*)(out + 0x40) = mode;
    *(float*)(out + 0x4C) = cmGetF_162568(self, 17);
    *(float*)(out + 0x54) = cmGetF_162568(self, 18);
    SetFrame_162568(out, *(sVec_001607E0*)(self + 0x40), p, cmGetR_162568(self, 1));
    *(float*)(out + 0x44) = cmLength_001607E0(*(sVec_001607E0*)(out + 0x20));
    float s;
    if (*(float*)(out + 0x4C) > 0.0f) {
        s = (*(float*)(out + 0x4C) + 1.0f) * 0.5f;
    } else {
        s = 0.0f;
    }
    *(float*)(out + 0x50) = s;
    *(float*)(self + 0x2F4) = *(float*)(out + 0x4C);
    *(float*)(self + 0x2F8) = *(float*)(out + 0x50);
    *(int*)(self + 0x2FC) = *(int*)(out + 0x5C);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162998);
#ifdef SKIP_ASM
extern "C" void func_00162998(void* self, void* obj)
{
    float v;
    if (*(int*)((char*)obj + 0x5C) == 0) {
        if (*(int*)((char*)self + 0x2C0) < 0x334)
            *(int*)((char*)self + 0x2C0) = *(int*)((char*)self + 0x2C0) + 1;
        int t = *(int*)((char*)self + 0x2C0);
        v = 1.0f;
        if (t >= 0x78)
            v = 1.0f - ((float)t - 120.0f) * 0.0014285714132711291f;
        if (v < 0.0f)
            v = 0.0f;
    } else {
        *(int*)((char*)self + 0x2C0) = 0;
        v = 1.0f;
    }
    *(float*)((char*)obj + 0x48) = v;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162A20);
#ifdef SKIP_ASM
extern "C" int func_001298C8();

struct sVec4_162A20
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEntry_162A20 { short delta; short index; sVec4_162A20 (*fn)(void*); };

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_162A20(const sVec4_162A20& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void DivEq_162A20(sVec4_162A20& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

static inline void Normalize_162A20(sVec4_162A20& v)
{
    DivEq_162A20(v, Length_162A20(v));
}

extern "C" void func_00162A20(char* self, char* trig)
{
    if (*(int*)(trig + 0x60))
    {
        if (*(int*)(self + 0x2D4) == 0)
        {
            sVec4_162A20 dir = *(sVec4_162A20*)(trig + 0x20);
            if (*(float*)(trig + 0x44) > 0.0f)
            {
                Normalize_162A20(dir);
                if (dir.z > 0.8500000238418579f)
                {
                    void* obj = *(void**)(self + 0x30);
                    sVEntry_162A20* vt = *(sVEntry_162A20**)obj;
                    sVec4_162A20 v = vt[9].fn((char*)obj + vt[9].delta);
                    if (v.z < 0.10000000149011612f)
                    {
                        *(float*)(self + 0x22C) = 0.0f;
                        *(int*)(self + 0x2D0) = 1;
                    }
                }
            }
            *(int*)(self + 0x2D4) = 1;
            int late = func_001298C8() - *(int*)(self + 0x2CC) >= 31;
            if (*(int*)(self + 0x2E0) == 0 && late)
                *(int*)(self + 0x2C4) = 15;
        }
        else
        {
            *(int*)(self + 0x2CC) = func_001298C8();
            return;
        }
    }
    else if (*(int*)(self + 0x2D4))
    {
        *(int*)(self + 0x2D4) = 0;
        *(int*)(self + 0x2D0) = 0;
        *(int*)(self + 0x2D8) = 0;
    }
    if (*(int*)(trig + 0x60))
        *(int*)(self + 0x2CC) = func_001298C8();
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162B80);
#ifdef SKIP_ASM
extern "C" cQuad128 func_00162B80(void* self, void* a1)
{
    cQuad128 v = *(cQuad128*)((char*)a1 + 0x10);
    *(cQuad128*)((char*)self + 0x20) = v;
    return v;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162B90);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector D_004FF160_2B90 __asm__("D_004FF160");

// The per-frame camera input record (0x70 bytes) that func_00162568 fills from the target for the chase algorithms.
struct sCamInfo {
    t4Vector f00;               // 0x00
    t4Vector pos;               // 0x10
    t4Vector dir;               // 0x20
    t4Vector out;               // 0x30: written back by func_00162C78
    int mode;                   // 0x40
    float speed;                // 0x44: length of dir
    char pad48[4];
    float f4C;                  // 0x4C
    float f50;                  // 0x50
    float f54;                  // 0x54
    int f58;                    // 0x58
    int f5C;                    // 0x5C
    int f60;                    // 0x60
    int f64;                    // 0x64
};

extern "C" void func_00168150_v(void* self) __asm__("func_00168150");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_162B90(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddTo_162B90(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

static inline float Min_162B90(float a, float b)
{
    return a <? b;
}

static inline float ClampLo_162B90(float v, float lo)
{
    if (v >= lo) {
        return v;
    }
    return lo;
}


struct sCam_162B90 {
    char pad00[0x20];
    t4Vector v20;
    char pad30[0x1B4];
    float f1E4;
    char pad1E8[0x20];
    float f208;
};

extern "C" void func_00162B90_i(float a, float b, float c, float d, float e, sCam_162B90* self, sCamInfo* src) __asm__("func_00162B90");
extern "C" void func_00162B90_i(float a, float b, float c, float d, float e, sCam_162B90* self, sCamInfo* src)
{
    float dist = src->speed - 1388.888916015625f;
    float t = Min_162B90(ClampLo_162B90(dist, 0.0f) * 0.0005142856971360743f, 1.0f);
    float x;
    if (src->f60 != 0) x = self->f208;
    else x = src->f54;
    self->f208 = x;
    float r = e * self->f1E4 + (1.0f - e) * (a + t * b + src->f4C * d + c * x);
    self->f1E4 = r;
    AddTo_162B90(self->v20, Scale_162B90(D_004FF160_2B90, r));
    func_00168150_v(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162C78);
#ifdef SKIP_ASM
struct sV_162C78 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVE_162C78 {
    short delta;
    short index;
    sV_162C78 (*fn)(void*);
};

struct sInfo_162C78 {
    char pad0[0x20];
    sV_162C78 dir;      // 0x20
    sV_162C78 out;      // 0x30
    char pad40[0x4];
    float speed;        // 0x44
};

struct sHist_162C78 {
    float a[5];         // 0x00
    float b[5];         // 0x14
    float c[5];         // 0x28
    int idx;            // 0x3C
    float k0;           // 0x40
    float k1;           // 0x44
    float k2;           // 0x48
};

extern "C" float func_0031C128(float x);

// PORT: PS2-only VU0 inline asm (dot product).
static inline float Dot_162C78(const sV_162C78& a, const sV_162C78& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV_162C78 Scale_162C78(const sV_162C78& v, float s)
{
    sV_162C78 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV_162C78 Add_162C78(const sV_162C78& a, const sV_162C78& b)
{
    sV_162C78 r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_162C78(const sV_162C78& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void DivEq_162C78(sV_162C78& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

static inline void Normalize_162C78(sV_162C78& v)
{
    DivEq_162C78(v, Length_162C78(v));
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_162C78(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` / `>?` operators.
static inline float Clamp_162C78(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

static inline sV_162C78 Get_162C78(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_162C78* e = &(*(sVE_162C78**)o)[slot];
    return e->fn(o + e->delta);
}

static inline sV_162C78 Lerp_162C78(const sV_162C78& p, const sV_162C78& q, float t)
{
    return Add_162C78(Scale_162C78(p, t), Scale_162C78(q, 1.0f - t));
}

static inline void LerpTo_162C78(sV_162C78& out, const sV_162C78& p, const sV_162C78& q, float t)
{
    out = Add_162C78(Scale_162C78(p, t), Scale_162C78(q, 1.0f - t));
}

extern "C" void func_00162C78_impl(char* self, sInfo_162C78* info) __asm__("func_00162C78");
extern "C" void func_00162C78_impl(char* self, sInfo_162C78* info)
{
    sV_162C78 n;
    if (info->speed > 27.77777862548828f) {
        sV_162C78 v = Get_162C78(self, 3);
        if (Dot_162C78(v, v) < 1.0f) {
            v = *(sV_162C78*)(self + 0x1C0);
        }
        *(sV_162C78*)(self + 0x1C0) = v;
        float ang;
        float len = Sqrt_162C78(Dot_162C78(*(sV_162C78*)(self + 0x80), *(sV_162C78*)(self + 0x80)) *
                                Dot_162C78(*(sV_162C78*)(self + 0x1C0), *(sV_162C78*)(self + 0x1C0)));
        if (len < 0.0010000000474974513f) {
            ang = 0.0f;
        } else {
            ang = 1.5707963705062866f -
                  func_0031C128(Clamp_162C78(
                      Dot_162C78(*(sV_162C78*)(self + 0x80), *(sV_162C78*)(self + 0x1C0)) / len, -1.0f, 1.0f));
        }
        ang = (ang - 0.19082477688789368f) >? 0.0f;
        float prev = ((sHist_162C78*)(self + 0x340))->b[(((sHist_162C78*)(self + 0x340))->idx + 4) % 5];
        float d = ang - ((sHist_162C78*)(self + 0x340))->b[(((sHist_162C78*)(self + 0x340))->idx + 4) % 5];
        sHist_162C78* h = (sHist_162C78*)(self + 0x340);
        float sum = 0.0f;
        for (int i = 0; i < 5; i++) {
            sum += h->c[i];
        }
        int p2 = (h->idx + 4) % 5;
        float val = prev + d * h->k0 + sum * h->k1 + (h->a[p2] - ang) * h->k2;
        h->a[h->idx] = ang;
        h->b[h->idx] = val;
        h->c[h->idx] = ang - val;
        float s = (1.0f - ((val * 2.5214130878448486f) <? 1.0f >? 0.0f)) * 0.03946405649185181f +
                  0.95552659034729f;
        h->idx = (h->idx + 1) % 5;
        if ((unsigned)(*(int*)(*(char**)(*(char**)(self + 0x30) + 4) + 0x434) - 0xB) < 3) {
            s = Sqrt_162C78(Sqrt_162C78(s));
        }
        LerpTo_162C78(n, *(sV_162C78*)(self + 0x80), Scale_162C78(info->dir, 1.0f / info->speed), s);
        *(sV_162C78*)(self + 0x80) = n;
    } else {
        n = *(sV_162C78*)(self + 0x80);
    }
    Normalize_162C78(n);
    info->out = n;
}
#endif

INCLUDE_ASM("camera/camera", func_00163010);

//100%
INCLUDE_ASM("camera/camera", func_00163158);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector D_004FF160_3158 __asm__("D_004FF160");

extern "C" void* func_00168150(void* self);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_163158(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__ volatile(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add into destination).
static inline void AddTo_163158(t4Vector& r, const t4Vector& a, const t4Vector& b)
{
    __asm__ volatile(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_163158(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__ volatile(
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void ScaleEq_163158(t4Vector& v, float s)
{
    int t;
    __asm__ volatile(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_163158(const t4Vector* a, const t4Vector& b)
{
    t4Vector r;
    __asm__ volatile(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "o"(*a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_163158(const t4Vector& v)
{
    float r;
    int t;
    __asm__ volatile(
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

extern "C" void func_00163158(void* self_, void* info, float a, float b)
{
    char* self = (char*)self_;
    float zero = 0.0f;
    t4Vector d = Sub_163158(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40));
    t4Vector c = Cross_163158(&D_004FF160_3158, d);
    float len = Length_163158(c);
    if (len > zero) {
        ScaleEq_163158(c, a * Length_163158(d) / len);
        AddTo_163158(*(t4Vector*)(self + 0x40), *(t4Vector*)(self + 0x40), c);
        t4Vector t = Scale_163158(c, b);
        AddTo_163158(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x20), t);
    }
    func_00168150(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00163270);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector D_004FF160_3270 __asm__("D_004FF160");

// The per-frame camera input record (0x70 bytes) that func_00162568 fills from the target for the chase algorithms.
extern "C" void func_00168150_v(void* self) __asm__("func_00168150");

// PORT: PS2-only VU0 inline asm (vector times scalar).
// PORT: PS2-only VU0 inline asm (dst -= b).
static inline void SubTo_163270(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_163270(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}


struct sCam_163270 {
    char pad00[0x40];
    t4Vector v40;
    char pad50[0x184];
    float f1D4;
    char pad1D8[0x2C];
    float f204;
};

extern "C" void func_00163270_i(float a, float b, float c, float d, float e, sCam_163270* self, sCamInfo* src) __asm__("func_00163270");
extern "C" void func_00163270_i(float a, float b, float c, float d, float e, sCam_163270* self, sCamInfo* src)
{
    float dist = ClampLo_162B90(src->speed - 1388.888916015625f, 0.0f);
    float t;
    if (src->speed >= 833.3333740234375f) {
        if (src->speed <= 1666.666748046875f)
            t = (src->speed - 833.3333740234375f) / 833.3333740234375f;
        else
            t = 1.0f;
    } else {
        t = 0.0f;
    }
    float tt = t * t;
    float x;
    if (src->f60 != 0) x = self->f204;
    else x = tt * d * (src->f54 - 0.5f);
    self->f204 = x;
    float r = e * self->f1D4 + (1.0f - e) * (a * (dist * 9.999999747378752e-05f) * Sqrt_163270(dist) - b + (x >? (c * Sqrt_163270(src->f4C))));
    self->f1D4 = r;
    SubTo_163270(self->v40, Scale_162B90(D_004FF160_3270, r));
    func_00168150_v(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001633B0);
#ifdef SKIP_ASM
extern "C" void* func_00168150(void* self);
extern "C" void func_001633B0(void* self, void* obj, float a, float b, float c, float d)
{
    float k = *(float*)((char*)obj + 0x44);
    float x = a * (k * 1.0000000116860974e-07f) * k + 1.0f + (c - 1.0f) * *(float*)((char*)obj + 0x4C);
    int mode = *(int*)((char*)obj + 0x40);
    if (mode == 1)
        d = 1.0f;
    else if (mode != 0)
        d = 0.9990000128746033f;
    *(float*)((char*)self + 0x1DC) = d * *(float*)((char*)self + 0x1DC) + (1.0f - d) * x;
    *(float*)self = b * 0.7853981852531433f * *(float*)((char*)self + 0x1DC);
    func_00168150(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00163450);
#ifdef SKIP_ASM
extern "C" void* func_00168150(void* self);

struct sVec4_163450
{
    float v[4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4_163450 Sub_163450(const sVec4_163450& a, const sVec4_163450& b)
{
    sVec4_163450 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec4_163450 Add_163450(const sVec4_163450& a, const sVec4_163450& b)
{
    sVec4_163450 r;
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec4_163450 Scale_163450(const sVec4_163450& v, float s)
{
    sVec4_163450 r;
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

extern "C" void func_00163450(char* self, char* trig, float a, float b, float c, float d, float e, float f, float g)
{
    if (*(int*)(trig + 0x5C))
    {
        sVec4_163450 t = Sub_163450(*(sVec4_163450*)(self + 0x20), *(sVec4_163450*)(self + 0x40));
        *(float*)(self + 0x1E0) = 0.0f;
        *(sVec4_163450*)(self + 0xB0) = t;
    }
    else if (*(float*)(self + 0x1E0) < 5.0f)
    {
        *(float*)(self + 0x1E0) += 0.01666666753590107f;
        if (*(float*)(self + 0x1E0) < d)
        {
            float u = *(float*)(self + 0x1E0) / d;
            float u2 = u * u;
            float s = u2 * 3.0f - (u2 + u2) * u;
            if (s > 1.0f)
                s = 1.0f;
            sVec4_163450 t = Add_163450(Scale_163450(Sub_163450(*(sVec4_163450*)(self + 0x20), *(sVec4_163450*)(self + 0xB0)), 1.0f - s),
                                        Scale_163450(*(sVec4_163450*)(self + 0x40), s));
            *(sVec4_163450*)(self + 0x40) = t;
        }
    }
    *(sVec4_163450*)(self + 0xA0) = *(sVec4_163450*)(trig + 0x10);
    float z;
    if (*(int*)(trig + 0x5C))
        z = f * *(float*)(self + 0x2B8) + (1.0f - f) * e;
    else
        z = g * *(float*)(self + 0x2B8);
    *(float*)(self + 0x2B8) = z;
    *(float*)(self + 0x48) += z;
    func_00168150(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001635C0);
#ifdef SKIP_ASM
extern "C" void func_001635C0(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635C8);
#ifdef SKIP_ASM
extern "C" float func_001635C8(void) {
    return 0.0f;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635D8);
#ifdef SKIP_ASM
extern "C" int func_001635D8(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635E0);
#ifdef SKIP_ASM
extern "C" int func_001635E0(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635E8);
#ifdef SKIP_ASM
extern int D_004A4DD8;

extern "C" int func_001635E8(void) {
    return D_004A4DD8;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635F0);
#ifdef SKIP_ASM
extern int D_004A4DDC;

extern "C" int func_001635F0(void) {
    return D_004A4DDC;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001635F8);
#ifdef SKIP_ASM
struct sVec_1635F8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVE_1635F8 {
    short delta;
    short index;
    void* fn;
};

extern sVec_1635F8 D_004FF160_v1635F8 __asm__("D_004FF160");
extern int D_004A1288;

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec_1635F8 Sub_1635F8(const sVec_1635F8& a, const sVec_1635F8& b)
{
    sVec_1635F8 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec_1635F8 Add_1635F8(const sVec_1635F8& a, const sVec_1635F8& b)
{
    sVec_1635F8 r;
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec_1635F8 Scale_1635F8(const sVec_1635F8& v, float s)
{
    sVec_1635F8 r;
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_1635F8(sVec_1635F8& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1635F8(const sVec_1635F8& v)
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

// 0 below lo, 1 above hi, linear in between.
static inline float Ramp_1635F8(const float& x, float lo, float hi)
{
    if (lo <= x) {
        if (x <= hi) {
            return (x - lo) / (hi - lo);
        }
        return 1.0f;
    }
    return 0.0f;
}

// Walks x toward limit by one 60 Hz frame.
static inline void Approach_1635F8(float& x, float limit)
{
    if (limit - 0.01666666753590107f <= x) {
        x = limit;
    } else {
        x = (x + 0.01666666753590107f) <? limit;
    }
}

static inline float Lerp_1635F8(float a, float b, float t)
{
    return t * b + (1.0f - t) * a;
}

// PORT: PS2-only VU0 inline asm (in-place v += b).
static inline void AddEq_1635F8(sVec_1635F8& v, const sVec_1635F8& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "+m"(v)
        : "m"(b)
        : "memory");
}

static inline int CallI_1635F8(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_1635F8* e = &(*(sVE_1635F8**)o)[slot];
    return ((int (*)(void*))e->fn)(o + e->delta);
}

static inline float CallF_1635F8(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_1635F8* e = &(*(sVE_1635F8**)o)[slot];
    return ((float (*)(void*))e->fn)(o + e->delta);
}

#define S_1635F8 ((char*)self)
#define I_1635F8 ((char*)info)
// Builds and evaluates the chase camera's jump/landing curves (splines at +0x304..+0x328).
extern "C" void func_001635F8(void* self, void* info, float a, float b, float c, float d)
{
    float dist = Length_1635F8(Sub_1635F8(*(sVec_1635F8*)(S_1635F8 + 0x20), *(sVec_1635F8*)(S_1635F8 + 0x40)));
    if (*(int*)(I_1635F8 + 0x60) != 0 && *(int*)(S_1635F8 + 0x2D0) == 0) {
        float start = 0.0f;
        if (--*(int*)(S_1635F8 + 0x2C4) > 0 && CallI_1635F8(S_1635F8, 19)) {
            start = (float)(16 - *(int*)(S_1635F8 + 0x2C4)) * 0.01666666753590107f;
            *(int*)(S_1635F8 + 0x2C4) = 0;
        }
        if (*(int*)(S_1635F8 + 0x2C4) == 14) {
            *(float*)(S_1635F8 + 0x218) = 0.01666666753590107f;
            float k = Ramp_1635F8(CallF_1635F8(S_1635F8, 21), 300.0f, 500.0f);
            float h = c * a * k;
            *(float*)(S_1635F8 + 0x228) = k;
            if (0.0f < h) {
                void* sa = S_1635F8 + 0x31C;
                void* sb = S_1635F8 + 0x328;
                func_0031D700_2(sa, 13);
                func_0031D7E0(sa, 0.0f, 0.0f);
                func_0031D7E0(sa, 0.33000001311302185f, h * -121.1346664428711f);
                func_0031D7E0(sa, 0.6600000262260437f, h * -217.7401123046875f);
                func_0031D7E0(sa, 0.998885452747345f, h * -254.97467041015625f);
                func_0031D7E0(sa, 1.3300000429153442f, h * -250.5922088623047f);
                func_0031D7E0(sa, 1.659999966621399f, h * -193.38156127929688f);
                func_0031D7E0(sa, 2.0f, h * -63.02951431274414f);
                func_0031D7E0(sa, 2.3299999237060547f, h * 110.80419158935547f);
                func_0031D7E0(sa, 2.6600000858306885f, h * 307.46771240234375f);
                func_0031D7E0(sa, 3.0f, h * 496.44952392578125f);
                func_0031D7E0(sa, 3.3299999237060547f, h * 689.6500854492188f);
                func_0031D7E0(sa, 3.6600000858306885f, h * 820.5333862304688f);
                func_0031D7E0(sa, 4.0f, h * 849.3922119140625f);
                func_0031D700_2(sb, 13);
                func_0031D7E0(sb, 0.0f, 0.0f);
                func_0031D7E0(sb, 0.33000001311302185f, h * -22.06365966796875f);
                func_0031D7E0(sb, 0.6600000262260437f, h * -34.630374908447266f);
                func_0031D7E0(sb, 1.0f, h * 13.823330879211426f);
                func_0031D7E0(sb, 1.3290001153945923f, h * 92.12850952148438f);
                func_0031D7E0(sb, 1.659500002861023f, h * 213.1912384033203f);
                func_0031D7E0(sb, 2.0000672340393066f, h * 383.46197509765625f);
                func_0031D7E0(sb, 2.333711624145508f, h * 624.2830200195312f);
                func_0031D7E0(sb, 2.6603856086730957f, h * 813.0820922851562f);
                func_0031D7E0(sb, 3.0f, h * 977.65869140625f);
                func_0031D7E0(sb, 3.3299999237060547f, h * 1070.9466552734375f);
                func_0031D7E0(sb, 3.659675359725952f, h * 1104.7698974609375f);
                func_0031D7E0(sb, 4.0f, h * 1131.4307861328125f);
                *(float*)(S_1635F8 + 0x200) = 4.0f;
                *(int*)(S_1635F8 + 0x2E8) = 1;
                *(float*)(S_1635F8 + 0x1F8) = 1.0f;
                *(float*)(S_1635F8 + 0x1FC) = 1.0f;
                *(int*)(S_1635F8 + 0x2E4) = 0;
            }
        }
        if (*(int*)(S_1635F8 + 0x2C4) == 0) {
            *(float*)(S_1635F8 + 0x1F4) = start;
            float len = CallF_1635F8(S_1635F8, 12);
            float sx = len * 0.25f;
            float k;
            if (1.7f <= len) {
                if (len <= 2.8f) {
                    float t = (len - 1.7f) / (2.8f - 1.7f);
                    float t2 = t * t;
                    k = t2 * 3.0f - (t + t) * t2;
                    if (k > 1.0f) {
                        k = 1.0f;
                    }
                } else {
                    k = 1.0f;
                }
            } else {
                k = 0.0f;
            }
            float h = k * c * *(float*)(S_1635F8 + 0x228);
            if (0.0f < h) {
                void* sc = S_1635F8 + 0x304;
                void* sd = S_1635F8 + 0x310;
                func_0031D700_2(sc, 13);
                func_0031D7E0(sc, 0.0f, 0.0f);
                func_0031D7E0(sc, sx * 0.33000001311302185f, h * -121.1346664428711f);
                func_0031D7E0(sc, sx * 0.6600000262260437f, h * -217.7401123046875f);
                func_0031D7E0(sc, sx * 0.998885452747345f, h * -254.97467041015625f);
                func_0031D7E0(sc, sx * 1.3300000429153442f, h * -250.5922088623047f);
                func_0031D7E0(sc, sx * 1.659999966621399f, h * -193.38156127929688f);
                func_0031D7E0(sc, sx * 2.0f, h * -63.02951431274414f);
                func_0031D7E0(sc, sx * 2.3299999237060547f, h * 110.80419158935547f);
                func_0031D7E0(sc, sx * 2.6600000858306885f, h * 307.46771240234375f);
                func_0031D7E0(sc, sx * 3.0f, h * 496.44952392578125f);
                func_0031D7E0(sc, sx * 3.3299999237060547f, h * 689.6500854492188f);
                func_0031D7E0(sc, sx * 3.6600000858306885f, h * 820.5333862304688f);
                func_0031D7E0(sc, sx * 4.0f, h * 849.3922119140625f);
                func_0031D700_2(sd, 13);
                func_0031D7E0(sd, 0.0f, 0.0f);
                func_0031D7E0(sd, sx * 0.33000001311302185f, h * -22.06365966796875f);
                func_0031D7E0(sd, sx * 0.6600000262260437f, h * -34.630374908447266f);
                func_0031D7E0(sd, sx * 1.0f, h * 13.823330879211426f);
                func_0031D7E0(sd, sx * 1.3290001153945923f, h * 92.12850952148438f);
                func_0031D7E0(sd, sx * 1.659500002861023f, h * 213.1912384033203f);
                func_0031D7E0(sd, sx * 2.0000672340393066f, h * 383.46197509765625f);
                func_0031D7E0(sd, sx * 2.333711624145508f, h * 624.2830200195312f);
                func_0031D7E0(sd, sx * 2.6603856086730957f, h * 813.0820922851562f);
                func_0031D7E0(sd, sx * 3.0f, h * 977.65869140625f);
                func_0031D7E0(sd, sx * 3.3299999237060547f, h * 1070.9466552734375f);
                func_0031D7E0(sd, sx * 3.659675359725952f, h * 1104.7698974609375f);
                func_0031D7E0(sd, sx * 4.0f, h * 1131.4307861328125f);
                *(int*)(S_1635F8 + 0x2E0) = 1;
                *(float*)(S_1635F8 + 0x1F8) = 1.0f;
                *(float*)(S_1635F8 + 0x1FC) = 1.0f;
                *(float*)(S_1635F8 + 0x200) = len;
                *(int*)(S_1635F8 + 0x2E4) = 0;
            }
        }
    }
    if (*(int*)(S_1635F8 + 0x2E8) != 0) {
        if (4.0f - 0.01666666753590107f <= *(float*)(S_1635F8 + 0x218)) {
            *(float*)(S_1635F8 + 0x218) = 4.0f;
        } else {
            *(float*)(S_1635F8 + 0x218) = (*(float*)(S_1635F8 + 0x218) + 0.01666666753590107f) <? 4.0f;
        }
        float yb = *(float*)(S_1635F8 + 0x1F8) * func_0031DEE0(S_1635F8 + 0x328, *(float*)(S_1635F8 + 0x1FC) * *(float*)(S_1635F8 + 0x218));
        float ya = *(float*)(S_1635F8 + 0x1F8) * func_0031DEE0(S_1635F8 + 0x31C, *(float*)(S_1635F8 + 0x1FC) * *(float*)(S_1635F8 + 0x218));
        *(float*)(S_1635F8 + 0x220) = yb;
        *(float*)(S_1635F8 + 0x21C) = ya;
    }
    if (*(int*)(S_1635F8 + 0x2E0) != 0) {
        Approach_1635F8(*(float*)(S_1635F8 + 0x1F4), *(float*)(S_1635F8 + 0x200));
        float yd = *(float*)(S_1635F8 + 0x1F8) * func_0031DEE0(S_1635F8 + 0x310, *(float*)(S_1635F8 + 0x1FC) * *(float*)(S_1635F8 + 0x1F4));
        float yc = *(float*)(S_1635F8 + 0x1F8) * func_0031DEE0(S_1635F8 + 0x304, *(float*)(S_1635F8 + 0x1FC) * *(float*)(S_1635F8 + 0x1F4));
        *(float*)(S_1635F8 + 0x210) = yd;
        *(float*)(S_1635F8 + 0x20C) = yc;
        D_004A1288 = 1;
        // PORT: spline pointers kept in int globals.
        D_004A4DD8 = (int)(S_1635F8 + 0x310);
        D_004A4DDC = (int)(S_1635F8 + 0x304);
    } else {
        D_004A1288 = 0;
        D_004A4DD8 = 0;
        D_004A4DDC = 0;
    }
    if (*(int*)(I_1635F8 + 0x60) == 0) {
        if (*(int*)(S_1635F8 + 0x2E4) == 0) {
            *(float*)(S_1635F8 + 0x224) = Lerp_1635F8(0.93f, d, Ramp_1635F8(*(float*)(S_1635F8 + 0x218), 1.2f, 2.2f));
            float v = CallF_1635F8(S_1635F8, 23);
            float cur = *(float*)(S_1635F8 + 0x224);
            float up = cur + (1.0f - cur) * 0.5f;
            float r = Ramp_1635F8(v, 1.05f, 1.4f);
            *(float*)(S_1635F8 + 0x224) = Lerp_1635F8(*(float*)(S_1635F8 + 0x224), up, r);
        }
        *(int*)(S_1635F8 + 0x2E0) = 0;
        *(int*)(S_1635F8 + 0x2E4) = 1;
        *(int*)(S_1635F8 + 0x2E8) = 0;
    }
    if (*(int*)(S_1635F8 + 0x2E4) != 0) {
        float m = *(float*)(S_1635F8 + 0x224);
        *(float*)(S_1635F8 + 0x20C) *= m;
        *(float*)(S_1635F8 + 0x210) *= m;
        *(float*)(S_1635F8 + 0x21C) *= m;
        *(float*)(S_1635F8 + 0x220) *= m;
    }
    float w;
    if (*(int*)(S_1635F8 + 0x2E0) != 0) {
        w = (*(float*)(S_1635F8 + 0x1F4) / b) <? 1.0f;
    } else {
        w = 0.0f;
    }
    float scale = Length_1635F8(Sub_1635F8(*(sVec_1635F8*)(S_1635F8 + 0x20), *(sVec_1635F8*)(S_1635F8 + 0x40))) *
                  0.0033333334140479565f;
    float ue = Lerp_1635F8(scale * *(float*)(S_1635F8 + 0x21C), scale * *(float*)(S_1635F8 + 0x20C), w);
    float ul = Lerp_1635F8(scale * *(float*)(S_1635F8 + 0x220), scale * *(float*)(S_1635F8 + 0x210), w);
    AddEq_1635F8(*(sVec_1635F8*)(S_1635F8 + 0x20), Scale_1635F8(D_004FF160_v1635F8, ue));
    AddEq_1635F8(*(sVec_1635F8*)(S_1635F8 + 0x40), Scale_1635F8(D_004FF160_v1635F8, ul));
    func_00168150(self);
    sVec_1635F8 off = Sub_1635F8(*(sVec_1635F8*)(S_1635F8 + 0x20), *(sVec_1635F8*)(S_1635F8 + 0x40));
    ScaleEq_1635F8(off, (dist >? 50.0f) / Length_1635F8(off));
    *(sVec_1635F8*)(S_1635F8 + 0x40) = Sub_1635F8(*(sVec_1635F8*)(S_1635F8 + 0x20), off);
}
#undef S_1635F8
#undef I_1635F8
#endif

//100%
INCLUDE_ASM("camera/camera", func_001641C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF160_41C0 __asm__("D_004FF160");

extern "C" void* func_00168150(void* self);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_41C0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddEq_41C0(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "+m"(dst)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_41C0(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void ScaleEq_41C0(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_41C0(const t4Vector* a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "o"(*a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_41C0(const t4Vector& v)
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
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_41C0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Normalize_41C0(const t4Vector& v)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_41C0(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

extern "C" void func_001641C0_v(char* self, float a, float b, float c, float ease) __asm__("func_001641C0");

extern "C" void func_001641C0_v(char* self, float a, float b, float c, float ease)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)o;
    int r = vt[24].fn(o + vt[24].delta);
    t4Vector d = Sub_41C0(*(t4Vector*)(self + 0x40), *(t4Vector*)(self + 0x20));
    float t = 0.0f;
    if (r) {
        t = 1.0f;
    }
    float f = ease * *(float*)(self + 0x214) + (1.0f - ease) * t;
    *(float*)(self + 0x214) = f;
    t4Vector v = Scale_41C0(Normalize_41C0(d), f * b);
    v.z += f * c;
    t4Vector cr = Cross_41C0(&D_004FF160_41C0, v);
    if (0.0f < Dot_41C0(cr, cr)) {
        ScaleEq_41C0(cr, f * a / Length_41C0(cr));
    }
    t4Vector sum = Add_41C0(v, cr);
    AddEq_41C0(*(t4Vector*)(self + 0x40), sum);
    func_00168150(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001643A8);
#ifdef SKIP_ASM
struct sVec_1643A8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec_1643A8 Sub_1643A8(const sVec_1643A8& a, const sVec_1643A8& b)
{
    sVec_1643A8 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec_1643A8 Add_1643A8(const sVec_1643A8& a, const sVec_1643A8& b)
{
    sVec_1643A8 r;
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec_1643A8 Scale_1643A8(const sVec_1643A8& v, float s)
{
    sVec_1643A8 r;
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_1643A8(sVec_1643A8& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1643A8(const sVec_1643A8& v)
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

#define S_1643A8 ((char*)self)
#define I_1643A8 ((char*)info)
extern "C" void func_001643A8(void* self, void* info, float a, float b, float c)
{
    int trig = *(float*)(I_1643A8 + 0x44) * 0.035999998450279236f < a && *(int*)(I_1643A8 + 0x60) == 0 &&
               *(int*)(I_1643A8 + 0x5C) == 0;
    if (*(int*)(S_1643A8 + 0x300) == 3 && trig) {
        *(int*)(S_1643A8 + 0x300) = 0;
        *(float*)(S_1643A8 + 0x248) = 0.0f;
        *(sVec_1643A8*)(S_1643A8 + 0x1A0) = Sub_1643A8(*(sVec_1643A8*)(S_1643A8 + 0x20), *(sVec_1643A8*)(S_1643A8 + 0x40));
    } else if (*(int*)(S_1643A8 + 0x300) == 2 && !trig) {
        *(int*)(S_1643A8 + 0x300) = 1;
        *(float*)(S_1643A8 + 0x248) = 1.0f;
    } else if (*(int*)(S_1643A8 + 0x300) == 0 && !trig) {
        *(int*)(S_1643A8 + 0x300) = 1;
    } else if (*(int*)(S_1643A8 + 0x300) == 1 && trig) {
        *(int*)(S_1643A8 + 0x300) = 0;
    }
    if (*(int*)(S_1643A8 + 0x300) == 0) {
        *(float*)(S_1643A8 + 0x248) = (*(float*)(S_1643A8 + 0x248) + 0.01666666753590107f / b) <? 1.0f;
        if (*(float*)(S_1643A8 + 0x248) == 1.0f)
            *(int*)(S_1643A8 + 0x300) = 2;
    } else if (*(int*)(S_1643A8 + 0x300) == 1) {
        *(float*)(S_1643A8 + 0x248) = (*(float*)(S_1643A8 + 0x248) - 0.01666666753590107f / c) >? 0.0f;
        if (*(float*)(S_1643A8 + 0x248) == 0.0f)
            *(int*)(S_1643A8 + 0x300) = 3;
    }
    if (*(int*)(S_1643A8 + 0x300) != 3) {
        float t = *(float*)(S_1643A8 + 0x248);
        float t2 = t * t;
        float s = t2 * 3.0f - (t2 + t2) * t;
        if (s > 1.0f)
            s = 1.0f;
        sVec_1643A8 d = Sub_1643A8(*(sVec_1643A8*)(S_1643A8 + 0x20), *(sVec_1643A8*)(S_1643A8 + 0x40));
        sVec_1643A8 e = Add_1643A8(Scale_1643A8(d, 1.0f - s), Scale_1643A8(*(sVec_1643A8*)(S_1643A8 + 0x1A0), s));
        float l1 = Length_1643A8(d);
        float l2 = Length_1643A8(e);
        ScaleEq_1643A8(e, (l1 * 0.5f + l2 * 0.5f) / l2);
        *(sVec_1643A8*)(S_1643A8 + 0x20) =
            Add_1643A8(Scale_1643A8(*(sVec_1643A8*)(S_1643A8 + 0x20), 1.0f - s), Scale_1643A8(*(sVec_1643A8*)(I_1643A8 + 0x10), s));
        *(sVec_1643A8*)(S_1643A8 + 0x40) = Sub_1643A8(*(sVec_1643A8*)(S_1643A8 + 0x20), e);
    }
    func_00168150(self);
}
#undef S_1643A8
#undef I_1643A8
#endif

//100%
INCLUDE_ASM("camera/camera", func_001646A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// Global vectors and a matrix in .bss that the camera code reads (set up elsewhere at startup).
extern t4Vector D_004FF120_46A0 __asm__("D_004FF120");

extern t4Vector D_004FF160_46A0 __asm__("D_004FF160");

extern "C" void func_001646A0_def(char* self, char* src, float a, float b) __asm__("func_001646A0");
extern "C" void* func_00168150(void* self);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_1646A0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector cmAddL_1646A0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScaleL_1646A0(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_1646A0(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v *= s).
static inline void cmScaleEq_1646A0(t4Vector& v, float s)
{
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void cmSubEq_1646A0(t4Vector& a, t4Vector b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length, vector passed by value).
static inline float cmLengthV_1646A0(t4Vector v)
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

static inline t4Vector cmLerp_1646A0(const t4Vector& a, const t4Vector& b, float t)
{
    t4Vector r = cmAddL_1646A0(cmScaleL_1646A0(a, t), cmScaleL_1646A0(b, 1.0f - t));
    return r;
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void cmAddEq_1646A0(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "+m"(dst)
        : "m"(b)
        : "memory");
}

// Sets the velocity and moves the position by it.
static inline void cmStep_1646A0(t4Vector& pos, t4Vector& vel, t4Vector v)
{
    vel = v;
    cmAddEq_1646A0(pos, v);
}


extern "C" void func_001646A0_def(char* self, char* src, float a, float b)
{
    t4Vector v;
    if (0.0f < *(float*)(src + 0x44)) {
        float l1 = cmLengthV_1646A0(cmSub_1646A0(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40)));
        float l2 = cmLengthV_1646A0(*(t4Vector*)(src + 0x20));
        float s = l1 / l2;
        t4Vector sv = *(t4Vector*)(src + 0x20);
        cmScaleEq_1646A0(sv, s);
        cmSubEq_1646A0(sv, cmScaleP_1646A0(&D_004FF160_46A0, a));
        v = cmSub_1646A0(*(t4Vector*)(self + 0x20), sv);
    } else {
        v = *(t4Vector*)(self + 0x40);
    }
    t4Vector d = cmSub_1646A0(v, *(t4Vector*)(self + 0x40));
    const t4Vector* pe = *(int*)(src + 0x64) ? &d : &D_004FF120_46A0;
    t4Vector e = *pe;
    cmStep_1646A0(*(t4Vector*)(self + 0x40), *(t4Vector*)(self + 0xC0), cmLerp_1646A0(*(t4Vector*)(self + 0xC0), e, b));
    func_00168150(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00164868);
#ifdef SKIP_ASM
extern "C" int func_00164868(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00164870);
#ifdef SKIP_ASM
extern int D_004A4DE0;

extern "C" int func_00164870(void) {
    return D_004A4DE0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00164878);
#ifdef SKIP_ASM
struct sVec_164878 {
    float x, y, z, w;
    sVec_164878() {}
    sVec_164878(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sVE_164878 {
    short delta;
    short index;
    void* fn;
};

extern sVec_164878 D_004FF160_v164878 __asm__("D_004FF160");
extern int D_004A128C;
// PORT: func_0031D738 takes (spline, knot count, x, y); its mangled name says (void*, float, float).
void func_0031D738_ivff(void* spline, int n, float x, float y) __asm__("func_0031D738__FPvff");

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec_164878 Sub_164878(const sVec_164878& a, const sVec_164878& b)
{
    sVec_164878 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec_164878 Add_164878(const sVec_164878& a, const sVec_164878& b)
{
    sVec_164878 r;
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

// PORT: PS2-only VU0 inline asm (in-place v += b).
static inline void AddEq_164878(sVec_164878& v, const sVec_164878& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place v -= b).
static inline void SubEq_164878(sVec_164878& v, const sVec_164878& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec_164878 Scale_164878(const sVec_164878& v, float s)
{
    sVec_164878 r;
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_164878(sVec_164878& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place v /= s).
static inline void DivEq_164878(sVec_164878& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (dot product).
static inline float Dot_164878(const sVec_164878& a, const sVec_164878& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_164878(const sVec_164878& v)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec_164878 Cross_164878(const sVec_164878& a, const sVec_164878& b)
{
    sVec_164878 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (rotate v by quaternion q).
static inline sVec_164878 Rot_164878(const sVec_164878& q, const sVec_164878& v)
{
    sVec_164878 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_164878(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline void Normalize_164878(sVec_164878& v)
{
    DivEq_164878(v, Length_164878(v));
}

// The part of v along n.
static inline sVec_164878 Project_164878(const sVec_164878& v, const sVec_164878& n)
{
    return Scale_164878(n, Dot_164878(v, n) / Dot_164878(n, n));
}

struct sQuat_164878 {
    float x, y, z, w;
    sQuat_164878() {}
    sQuat_164878(float ax, float ay, float az, float aw)
    {
        w = aw;
        x = ax;
        y = ay;
        z = az;
    }
} __attribute__((aligned(16)));

static inline sQuat_164878 Quat_164878(float x, float y, float z, float w)
{
    sQuat_164878 r(x, y, z, w);
    return r;
}

// PORT: PS2-only VU0 inline asm (rotate v by quaternion q).
static inline sVec_164878 RotQ_164878(const sQuat_164878& q, const sVec_164878& v)
{
    sVec_164878 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: g++ `<?` / `>?` operators.
static inline float Clamp_164878(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

// 3t^2 - 2t^3, capped at 1.
static inline float Smooth_164878(float t)
{
    float t2 = t * t;
    float s = t2 * 3.0f - (t2 + t2) * t;
    if (s > 1.0f) {
        s = 1.0f;
    }
    return s;
}

#define MAX_164878(a, b) ((b) <= (a) ? (a) : (b))

static inline int CallI_164878(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_164878* e = &(*(sVE_164878**)o)[slot];
    return ((int (*)(void*))e->fn)(o + e->delta);
}

static inline float CallF_164878(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_164878* e = &(*(sVE_164878**)o)[slot];
    return ((float (*)(void*))e->fn)(o + e->delta);
}

static inline sVec_164878* CallV_164878(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_164878* e = &(*(sVE_164878**)o)[slot];
    return ((sVec_164878* (*)(void*))e->fn)(o + e->delta);
}

#define S_164878 ((char*)self)
#define I_164878 ((char*)info)
#define V_164878(off) (*(sVec_164878*)(S_164878 + (off)))
#define F_164878(off) (*(float*)(S_164878 + (off)))
// Air swing for drops: swings the eye around the rider while airborne (spline +0x334), then blends back.
extern "C" void func_00164878(void* self, void* info, float blendOut)
{
    D_004A128C = 0;
    sVec_164878 off0 = Sub_164878(V_164878(0x20), V_164878(0x40));
    sVec_164878 eye0 = V_164878(0x40);
    if (*(int*)(S_164878 + 0x2D0) != 0) {
        if (*(int*)(S_164878 + 0x2D8) == 0) {
            F_164878(0x22C) += 0.01666666753590107f;
            if (CallI_164878(S_164878, 19)) {
                V_164878(0x110) = Sub_164878(V_164878(0x20), V_164878(0x40));
                float dist = Length_164878(V_164878(0x110));
                float* dur = &F_164878(0x230);
                *dur = MAX_164878(CallF_164878(S_164878, 12) - F_164878(0x22C), 2.299999952316284f);
                F_164878(0x23C) = blendOut;
                F_164878(0x234) = 0.0f;
                sVec_164878* up = CallV_164878(S_164878, 16);
                sVec_164878 side = Cross_164878(D_004FF160_v164878, *up);
                Normalize_164878(side);
                if (Dot_164878(side, *(sVec_164878*)(I_164878 + 0x20)) < 0.0f) {
                    ScaleEq_164878(side, -1.0f);
                }
                ScaleEq_164878(side, dist / Length_164878(side));
                V_164878(0x120) = Sub_164878(V_164878(0x110), Scale_164878(Project_164878(V_164878(0x110), *up), 2.0f));
                V_164878(0x120).z = -V_164878(0x120).z;
                if (Dot_164878(V_164878(0x120), V_164878(0x120)) <= 0.0f) {
                    V_164878(0x120) = side;
                }
                ScaleEq_164878(V_164878(0x120), 0.800000011920929f);
                AddEq_164878(V_164878(0x120), Scale_164878(side, 0.20000000298023224f));
                ScaleEq_164878(V_164878(0x120), dist / Length_164878(V_164878(0x120)));
                V_164878(0x150) = Project_164878(V_164878(0x120), *up);
                sVec_164878 lat = Sub_164878(V_164878(0x120), V_164878(0x150));
                if (Dot_164878(lat, lat) < 40000.0f) {
                    lat.z = 0.0f;
                    float l = Length_164878(lat);
                    if (1.0f < l) {
                        ScaleEq_164878(lat, 200.0f / l);
                        AddEq_164878(V_164878(0x120), lat);
                        SubEq_164878(V_164878(0x120), Scale_164878(D_004FF160_v164878, 100.0f));
                    }
                }
                V_164878(0x130) = Cross_164878(V_164878(0x110), V_164878(0x120));
                Normalize_164878(V_164878(0x130));
                float ang;
                float len = Dot_164878(V_164878(0x110), V_164878(0x110)) * Dot_164878(V_164878(0x120), V_164878(0x120));
                len = Sqrt_164878(len);
                if (len < 0.0010000000474974513f) {
                    ang = 0.0f;
                } else {
                    float c = Dot_164878(V_164878(0x110), V_164878(0x120)) / len;
                    if (-1.0f <= c) {
                        c = c <? 1.0f;
                    } else {
                        c = -1.0f;
                    }
                    ang = 1.5707963705062866f - func_0031C128(c);
                }
                F_164878(0x238) = ang;
                void* spl = S_164878 + 0x334;
                func_0031D738_ivff(spl, 9, 0.0f, 0.0f);
                func_0031D7E0(spl, 0.0f, 0.0f);
                func_0031D7E0(spl, 0.125f, 0.0639725923538208f);
                func_0031D7E0(spl, 0.25f, 0.30785250663757324f);
                func_0031D7E0(spl, 0.375f, 0.5528385043144226f);
                func_0031D7E0(spl, 0.5f, 0.6577425003051758f);
                func_0031D7E0(spl, 0.625f, 0.728873610496521f);
                func_0031D7E0(spl, 0.75f, 0.827086865901947f);
                func_0031D7E0(spl, 0.875f, 0.9414384365081787f);
                func_0031D7E0(spl, 1.0f, 1.0f);
                *(int*)(S_164878 + 0x2D8) = 1;
            }
        }
        if (*(int*)(S_164878 + 0x2D8) != 0) {
            F_164878(0x234) += 0.01666666753590107f;
            if (F_164878(0x234) < F_164878(0x230)) {
                float t = F_164878(0x234) / F_164878(0x230);
                D_004A128C = 1;
                // PORT: spline pointer kept in an int global.
                D_004A4DE0 = (int)(S_164878 + 0x334);
                float s = func_0031DEE0(S_164878 + 0x334, t);
                float sn, cs;
                func_0031BE50(&sn, &cs, s * F_164878(0x238) * 0.5f);
                sQuat_164878 q = Quat_164878(sn * V_164878(0x130).x, sn * V_164878(0x130).y, sn * V_164878(0x130).z, cs);
                V_164878(0x140) = RotQ_164878(q, V_164878(0x110));
                V_164878(0x160) = Scale_164878(V_164878(0x150), s);
            }
            float k = (F_164878(0x234) / (F_164878(0x230) * 0.30000001192092896f)) <? 1.0f >? 0.0f;
            k = Smooth_164878(k);
            V_164878(0x20) = Add_164878(Scale_164878(V_164878(0x20), 1.0f - k), Scale_164878(*(sVec_164878*)(I_164878 + 0x10), k));
            V_164878(0x40) = Add_164878(Sub_164878(V_164878(0x20), V_164878(0x140)), V_164878(0x160));
        }
    } else if (0.0f < F_164878(0x23C)) {
        float bt = (F_164878(0x23C) - 0.01666666753590107f) >? 0.0f;
        float s = bt / blendOut;
        F_164878(0x23C) = bt;
        s = Smooth_164878(s);
        sVec_164878 eye = V_164878(0x20);
        V_164878(0x20) = Add_164878(Scale_164878(V_164878(0x20), 1.0f - s), Scale_164878(*(sVec_164878*)(I_164878 + 0x10), s));
        V_164878(0x40) = Add_164878(Scale_164878(V_164878(0x40), 1.0f - s),
                                    Scale_164878(Add_164878(Sub_164878(*(sVec_164878*)(I_164878 + 0x10), V_164878(0x140)), V_164878(0x160)), s));
        V_164878(0x170) = Sub_164878(V_164878(0x20), V_164878(0x40));
        V_164878(0x180) = Sub_164878(V_164878(0x20), eye);
        F_164878(0x244) = F_164878(0x240) = F_164878(0x23C);
    }
    if (*(int*)(S_164878 + 0x2D0) != 0 && 0.0f < F_164878(0x240) && 0.0f < F_164878(0x244)) {
        float bt = (F_164878(0x240) - 0.01666666753590107f) >? 0.0f;
        float s = bt / F_164878(0x244);
        F_164878(0x240) = bt;
        s = Smooth_164878(s);
        sVec_164878 d = Sub_164878(V_164878(0x20), V_164878(0x40));
        sVec_164878 dd = Add_164878(Scale_164878(V_164878(0x170), s), Scale_164878(d, 1.0f - s));
        AddEq_164878(V_164878(0x20), Scale_164878(V_164878(0x180), s));
        V_164878(0x40) = Sub_164878(V_164878(0x20), dd);
    }
    sVec_164878 cur = Sub_164878(V_164878(0x20), V_164878(0x40));
    float lmin = Length_164878(off0) * 0.6000000238418579f;
    float l = Length_164878(cur);
    if (l < lmin) {
        if (1.0f < l) {
            ScaleEq_164878(cur, lmin / l);
            V_164878(0x40) = Sub_164878(V_164878(0x20), cur);
        } else {
            V_164878(0x40) = eye0;
        }
    }
    func_00168150(self);
}
#undef S_164878
#undef I_164878
#undef V_164878
#undef F_164878
#endif

//100%
INCLUDE_ASM("camera/camera", func_00165540);
#ifdef SKIP_ASM
extern "C" void* func_00168150(void* self);
extern "C" void func_0031BE50(float* sout, float* cout, float x);

struct sVec4_165540
{
    float x, y, z, w;
    sVec4_165540() {}
    sVec4_165540(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern sVec4_165540 D_004FF140_v165540 __asm__("D_004FF140");
extern sVec4_165540 D_004FF160_v165540 __asm__("D_004FF160");

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4_165540 Sub_165540(const sVec4_165540& a, const sVec4_165540& b)
{
    sVec4_165540 r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_165540(const sVec4_165540& v)
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_165540(sVec4_165540& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (rotate v by quaternion q).
static inline sVec4_165540 Rot_165540(const sVec4_165540& q, const sVec4_165540& v)
{
    sVec4_165540 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

static inline sVec4_165540 AxisAngle_165540(const sVec4_165540& axis, float ang)
{
    float s, c;
    func_0031BE50(&s, &c, ang * 0.5f);
    sVec4_165540 r(s * axis.x, s * axis.y, s * axis.z, c);
    return r;
}

extern "C" void func_00165540(char* self, float ang)
{
    sVec4_165540 off = Sub_165540(*(sVec4_165540*)(self + 0x20), *(sVec4_165540*)(self + 0x40));
    sVec4_165540 dir = D_004FF140_v165540;
    float z = off.z;
    off.z = 0.0f;
    float len = Length_165540(off);
    if (off.z < len)
    {
        ScaleEq_165540(dir, len);
        dir.z = z;
        sVec4_165540 q = AxisAngle_165540(D_004FF160_v165540, ang);
        sVec4_165540 rd = Rot_165540(q, dir);
        sVec4_165540 t = Sub_165540(*(sVec4_165540*)(self + 0x20), rd);
        *(sVec4_165540*)(self + 0x40) = t;
    }
    func_00168150(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001656B0);
#ifdef SKIP_ASM
// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_1656B0()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

static inline float RandRange_1656B0(float lo, float hi)
{
    return lo + (hi - lo) * randf_1656B0();
}

extern "C" void func_001656B0(char* self, float base, float spread, float r)
{
    *(float*)(self + 0x254) = base + spread * randf_1656B0();
    *(float*)(self + 0x258) = base + spread * randf_1656B0();
    *(float*)(self + 0x25C) = base + spread * randf_1656B0();
    float rz = r * 1.600000023841858f;
    *(float*)(self + 0x260) = RandRange_1656B0(-r, r);
    *(float*)(self + 0x264) = RandRange_1656B0(-r, r);
    *(float*)(self + 0x268) = RandRange_1656B0(-rz, rz);
    *(float*)(self + 0x26C) = 0.0f;
    *(float*)(self + 0x270) = 0.0f;
    *(float*)(self + 0x274) = 0.0f;
    *(float*)(self + 0x278) = 0.0f;
    *(float*)(self + 0x27C) = 0.0f;
    *(float*)(self + 0x280) = 0.0f;
    base *= 0.5f;
    spread *= 0.5f;
    r *= 0.5f;
    rz *= 0.5f;
    *(float*)(self + 0x284) = base + spread * randf_1656B0();
    *(float*)(self + 0x288) = base + spread * randf_1656B0();
    *(float*)(self + 0x28C) = base + spread * randf_1656B0();
    *(float*)(self + 0x290) = RandRange_1656B0(-r, r);
    *(float*)(self + 0x294) = RandRange_1656B0(-r, r);
    *(float*)(self + 0x298) = RandRange_1656B0(-rz, rz);
    *(float*)(self + 0x29C) = 0.0f;
    *(float*)(self + 0x2A0) = 0.0f;
    *(float*)(self + 0x2A4) = 0.0f;
    *(float*)(self + 0x2A8) = 0.0f;
    *(float*)(self + 0x2AC) = 0.0f;
    *(float*)(self + 0x2B0) = 0.0f;
    *(int*)(self + 0x2EC) = 0;
    *(float*)(self + 0x2B4) = 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00165938);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// 16-byte vector with inline constructors (a non-POD sibling of t4Vector, as in ai/ai); stores x, y, z, w.
struct sVec4 : t4Vector
{
    sVec4() {}
    sVec4(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
    sVec4& operator+=(const sVec4& b);
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector add, in place).
inline sVec4& sVec4::operator+=(const sVec4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(*this)
        : "m"(*this), "m"(b)
        : "memory");
    return *this;
}

int BXrand();

// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_165938()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

static inline float RandRange_165938(float lo, float hi)
{
    return lo + (hi - lo) * randf_165938();
}

// Smooth (3t^2 - 2t^3) ramp: 0 below lo, 1 above hi, in between eased on t = x / hi
// (the callers always start the ramp at lo = 0).
static inline float SmoothStep_165938(float lo, float hi, float x)
{
    if (x >= lo) {
        if (x <= hi) {
            float t = x / hi;
            float sq = t * t;
            float e = 3.0f * sq - 2.0f * t * sq;
            if (e > 1.0f) {
                e = 1.0f;
            }
            return e;
        }
        return 1.0f;
    }
    return 0.0f;
}

static inline float Lerp_165938(const float& a, const float& b, float k)
{
    return k * b + (1.0f - k) * a;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4 Sub_165938(const sVec4& a, const sVec4& b)
{
    sVec4 r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_165938(const sVec4& v)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4 Scale_165938(const sVec4& v, float s)
{
    sVec4 r;
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

// Camera shake update: re-rolls each axis's target when its timer runs out, eases between the
// previous and new targets and adds the result (scaled by the fade-out and distance) to the position.
extern "C" void func_00165938(char* self, int flag, int sel, float base, float spread, float r)
{
    if (flag) {
        *(float*)(self + 0x2B4) = 0.0f;
    } else if (*(int*)(self + 0x2EC)) {
        *(float*)(self + 0x2B4) = 2.0f;
    }
    *(int*)(self + 0x2EC) = flag;
    if (*(float*)(self + 0x2B4) > 0.0f) {
        // PORT: g++ `>?` (max) operator.
        *(float*)(self + 0x2B4) = (*(float*)(self + 0x2B4) - 0.01666666753590107f) >? 0.0f;
    }
    float scale;
    if (*(float*)(self + 0x2B4) > 0.0f) {
        scale = *(float*)(self + 0x2B4) * 0.5f;
    } else {
        scale = 1.0f;
    }
    if (!flag && !(*(float*)(self + 0x2B4) > 0.0f)) {
        return;
    }
    *(float*)(self + 0x278) += 0.01666666753590107f;
    *(float*)(self + 0x27C) += 0.01666666753590107f;
    *(float*)(self + 0x280) += 0.01666666753590107f;
    if (*(float*)(self + 0x278) > *(float*)(self + 0x254)) {
        *(float*)(self + 0x254) = base + spread * randf_165938();
        *(float*)(self + 0x26C) = *(float*)(self + 0x260);
        *(float*)(self + 0x260) = RandRange_165938(-r, r);
        *(float*)(self + 0x278) -= *(float*)(self + 0x254);
    }
    if (*(float*)(self + 0x27C) > *(float*)(self + 0x258)) {
        *(float*)(self + 0x258) = base + spread * randf_165938();
        *(float*)(self + 0x270) = *(float*)(self + 0x264);
        *(float*)(self + 0x264) = RandRange_165938(-r, r);
        *(float*)(self + 0x27C) -= *(float*)(self + 0x258);
    }
    float rz = r * 1.600000023841858f;
    if (*(float*)(self + 0x280) > *(float*)(self + 0x25C)) {
        *(float*)(self + 0x25C) = base + spread * randf_165938();
        *(float*)(self + 0x274) = *(float*)(self + 0x268);
        *(float*)(self + 0x268) = RandRange_165938(-rz, rz);
        *(float*)(self + 0x280) -= *(float*)(self + 0x25C);
    }
    rz *= 0.5f;
    float base2 = base * 0.5f;
    float spread2 = spread * 0.5f;
    float r2 = r * 0.5f;
    if (*(float*)(self + 0x2A8) > *(float*)(self + 0x284)) {
        *(float*)(self + 0x284) = base2 + spread2 * randf_165938();
        *(float*)(self + 0x29C) = *(float*)(self + 0x290);
        *(float*)(self + 0x290) = RandRange_165938(-r2, r2);
        *(float*)(self + 0x2A8) -= *(float*)(self + 0x284);
    }
    if (*(float*)(self + 0x2AC) > *(float*)(self + 0x288)) {
        *(float*)(self + 0x288) = base2 + spread2 * randf_165938();
        *(float*)(self + 0x2A0) = *(float*)(self + 0x294);
        *(float*)(self + 0x294) = RandRange_165938(-r2, r2);
        *(float*)(self + 0x2AC) -= *(float*)(self + 0x288);
    }
    if (*(float*)(self + 0x2B0) > *(float*)(self + 0x28C)) {
        *(float*)(self + 0x28C) = base2 + spread2 * randf_165938();
        *(float*)(self + 0x2A4) = *(float*)(self + 0x298);
        *(float*)(self + 0x298) = RandRange_165938(-rz, rz);
        *(float*)(self + 0x2B0) -= *(float*)(self + 0x28C);
    }
    float k0 = SmoothStep_165938(0.0f, *(float*)(self + 0x254), *(float*)(self + 0x278));
    float k1 = SmoothStep_165938(0.0f, *(float*)(self + 0x258), *(float*)(self + 0x27C));
    float k2 = SmoothStep_165938(0.0f, *(float*)(self + 0x25C), *(float*)(self + 0x280));
    float k3 = SmoothStep_165938(0.0f, *(float*)(self + 0x284), *(float*)(self + 0x2A8));
    float k4 = SmoothStep_165938(0.0f, *(float*)(self + 0x288), *(float*)(self + 0x2AC));
    float k5 = SmoothStep_165938(0.0f, *(float*)(self + 0x28C), *(float*)(self + 0x2B0));
    float x = Lerp_165938(*(float*)(self + 0x26C), *(float*)(self + 0x260), k0);
    float y = Lerp_165938(*(float*)(self + 0x270), *(float*)(self + 0x264), k1);
    float z = Lerp_165938(*(float*)(self + 0x274), *(float*)(self + 0x268), k2);
    x += Lerp_165938(*(float*)(self + 0x29C), *(float*)(self + 0x290), k3);
    y += Lerp_165938(*(float*)(self + 0x2A0), *(float*)(self + 0x294), k4);
    z += Lerp_165938(*(float*)(self + 0x2A4), *(float*)(self + 0x298), k5);
    float amp;
    if (sel && Length_165938(Sub_165938(*(sVec4*)(self + 0x20), *(sVec4*)(self + 0x40))) <= 150.0f) {
        amp = Length_165938(Sub_165938(*(sVec4*)(self + 0x20), *(sVec4*)(self + 0x40))) * 0.1f;
    } else {
        amp = 15.0f;
    }
    *(sVec4*)(self + 0x20) += Scale_165938(sVec4(x, y, z, 0.0f), scale * amp);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00166228);
#ifdef SKIP_ASM
struct sCamTarget166228 {
    float x, y, z, w;
    int valid; // 0x10
};

struct sCamVE166228 {
    short delta;
    short index;
    void (*fn)(void*, sCamTarget166228*);
};

extern "C" void func_00166640(void* self, int a1);
extern "C" void func_001662A0(void* self, float x, float y, float z, float w);
void* func_00166530(void* self);
extern "C" void func_00166F90(void* self);

extern "C" void func_00166228(void* self)
{
    sCamTarget166228 t;
    sCamVE166228* vt = *(sCamVE166228**)((char*)self + 0x10);
    vt[7].fn((char*)self + vt[7].delta, &t);
    func_00166640(self, 0);
    if (t.valid != 0)
        func_001662A0(self, t.x, t.y, t.z, t.w);
    func_00166530(self);
    func_00166F90(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001662A0);
#ifdef SKIP_ASM
extern "C" float func_0031C128(float x);

struct sVec_1662A0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEv_1662A0 {
    short delta;
    short index;
    sVec_1662A0 (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec_1662A0 Sub_1662A0(const sVec_1662A0& a, const sVec_1662A0& b)
{
    sVec_1662A0 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec_1662A0 Add_1662A0(const sVec_1662A0& a, const sVec_1662A0& b)
{
    sVec_1662A0 r;
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec_1662A0 Scale_1662A0(const sVec_1662A0& v, float s)
{
    sVec_1662A0 r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_1662A0(const sVec_1662A0& a, const sVec_1662A0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_1662A0(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Clamp_1662A0(float v, float lo, float hi)
{
    if (v >= lo)
        return v <? hi;
    return lo;
}

static inline float Angle_1662A0(const sVec_1662A0& a, const sVec_1662A0& b)
{
    float l = Sqrt_1662A0(Dot_1662A0(a, a) * Dot_1662A0(b, b));
    if (l < 0.0010000000474974513f)
        return 0.0f;
    return 1.5707963705062866f - func_0031C128(Clamp_1662A0(Dot_1662A0(a, b) / l, -1.0f, 1.0f));
}

static inline sVec_1662A0 Get_1662A0(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEv_1662A0* e = &(*(sVEv_1662A0**)o)[slot];
    return e->fn(o + e->delta);
}

#define S_1662A0 ((char*)self)
extern "C" void func_001662A0(void* self, float x, float y, float z, float w)
{
    float k, ang;
    int up = *(float*)(S_1662A0 + 0x1D8) < *(float*)(S_1662A0 + 0x54);
    if (up) {
        k = w;
        ang = y;
    } else {
        k = z;
        ang = x;
    }
    float old = *(float*)(S_1662A0 + 0x54);
    *(float*)(S_1662A0 + 0x54) = k * *(float*)(S_1662A0 + 0x1D8) + (1.0f - k) * old;
    sVec_1662A0 d = Sub_1662A0(*(sVec_1662A0*)(S_1662A0 + 0x20), *(sVec_1662A0*)(S_1662A0 + 0x40));
    sVec_1662A0 v;
    func_001668B8(self, &v);
    float a = Angle_1662A0(d, v);
    float lim = ang * 0.01745329424738884f;
    float k2;
    if (lim < a)
        k2 = k / (a / lim);
    else
        k2 = k;
    *(float*)(S_1662A0 + 0x1D8) = *(float*)(S_1662A0 + 0x54) = k2 * *(float*)(S_1662A0 + 0x1D8) + (1.0f - k2) * old;
    sVec_1662A0 v2;
    func_001668B8(self, &v2);
    sVec_1662A0 off = Sub_1662A0(Get_1662A0(S_1662A0, 1), *(sVec_1662A0*)(S_1662A0 + 0x40));
    float t = Dot_1662A0(off, v2) / Dot_1662A0(v2, v2);
    sVec_1662A0 sc = Scale_1662A0(v2, t);
    *(sVec_1662A0*)(S_1662A0 + 0x20) = Add_1662A0(*(sVec_1662A0*)(S_1662A0 + 0x40), sc);
    func_00168150(self);
}
#undef S_1662A0
#endif

extern "C" void* func_00168150(void* self);

//100%
INCLUDE_ASM("camera/camera", func_00166530__FPv);
#ifdef SKIP_ASM
void* func_00166530(void* self)
{
    return func_00168150(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00166550);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF160_6550 __asm__("D_004FF160");


// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_166550(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_166550(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_166550(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void DivEq_166550(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

static inline t4Vector Get_166550(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* e = &(*(sVEntry<t4Vector(void*)>**)o)[slot];
    return e->fn(o + e->delta);
}

// PORT: PS2-only VU0 inline asm (vector subtract into destination).
static inline void SubTo_166550(t4Vector& r, const t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

extern "C" void func_00166550(void* self_, float a, float b)
{
    char* self = (char*)self_;
    t4Vector& v = *(t4Vector*)(self + 0x80);
    v = Scale_166550(Get_166550(self, 4), a);
    SubTo_166550(v, v, Scale_166550(D_004FF160_6550, b));
    DivEq_166550(v, Length_166550(v));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00166640);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector D_004FF140;

extern t4Vector D_004FF150;

extern "C" float func_0031C228(float x);
extern "C" float func_0031C128(float x);


// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_166640(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_166640(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_166640(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product, second vector through an address register).
static inline float DotP_166640(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "o"(b));
    return r;
}

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_166640(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Angle_166640(const t4Vector& a, const t4Vector& b)
{
    float l = Sqrt_166640(Dot_166640(a, a) * Dot_166640(b, b));
    if (l < 0.0010000000474974513f)
        return 0.0f;
    float c = Dot_166640(a, b) / l;
    c = (c >= -1.0f) ? (c <? 1.0f) : -1.0f;
    return 1.5707963705062866f - func_0031C128(c);
}

extern "C" void func_00166640_def(char* self) __asm__("func_00166640");
extern "C" void func_00166640_def(char* self)
{
    const t4Vector& off0 = Sub_166640(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40));
    t4Vector off = off0;
    float* p = (float*)(self + 0x50);
    float z = off.z;
    off.z = 0.0f;
    float len = Length_166640(off);
    *(float*)(self + 0x54) = -func_0031C228(z / len);
    if (0.0f < len) {
        float r;
        if (0.0f < DotP_166640(off, D_004FF150)) {
            r = Angle_166640(off, D_004FF140);
        } else {
            r = 6.2831854820251465f - Angle_166640(off, D_004FF140);
        }
        *p = r;
    } else {
        *p = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001668B8);
#ifdef SKIP_ASM
struct sV_1668B8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_1668B8 {
    float m[4][4];
} __attribute__((aligned(16)));


extern sM_1668B8 D_004FF1A0_1668B8 __asm__("D_004FF1A0");
extern sV_1668B8 D_004FF150_1668B8 __asm__("D_004FF150");
extern sV_1668B8 D_004FF160_v1668B8 __asm__("D_004FF160");
extern sV_1668B8 D_004FF130_v1668B8 __asm__("D_004FF130");
extern "C" void func_0031BE50(float* sout, float* cout, float x);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_1668B8(sM_1668B8* dst, const sM_1668B8* src)
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
static inline void vu0MulMtx_1668B8(sM_1668B8* dst, const sM_1668B8* a, const sM_1668B8* b)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, m = m * b).
static inline void vu0MulEqMtx_1668B8(sM_1668B8* m, const sM_1668B8* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_1668B8(sM_1668B8* m, const sV_1668B8& v)
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

static inline void SinCos_1668B8(float* s, float* c, float angle)
{
    func_0031BE50(s, c, angle);
}

static inline void Rotate_1668B8(sM_1668B8* m, const sV_1668B8& axis, float angle)
{
    sM_1668B8 tm;
    vu0CopyMtx_1668B8(&tm, m);
    sV_1668B8 pos = *(sV_1668B8*)tm.m[3];
    *(sV_1668B8*)tm.m[3] = D_004FF130_v1668B8;
    float s;
    float c;
    SinCos_1668B8(&s, &c, angle);
    float one = 1.0f;
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
    m->m[0][0] = tx * x + c;
    m->m[1][0] = tx * y + sz;
    m->m[2][0] = tx * z - sy;
    m->m[0][1] = ty * x - sz;
    m->m[1][1] = ty * y + c;
    m->m[2][1] = ty * z + sx;
    m->m[0][2] = tz * x + sy;
    m->m[1][2] = tz * y - sx;
    m->m[2][2] = tz * z + c;
    m->m[3][0] = 0.0f;
    m->m[3][1] = 0.0f;
    m->m[3][2] = 0.0f;
    m->m[3][3] = one;
    m->m[0][3] = 0.0f;
    m->m[1][3] = 0.0f;
    m->m[2][3] = 0.0f;
    *(sV_1668B8*)m->m[3] = pos;
    vu0MulEqMtx_1668B8(m, &tm);
}

static inline sV_1668B8 Row_1668B8(const sM_1668B8& m, int i)
{
    return *(const sV_1668B8*)m.m[i];
}

extern "C" void func_001668B8_impl(char* self, sV_1668B8* out) __asm__("func_001668B8");
extern "C" void func_001668B8_impl(char* self, sV_1668B8* out)
{
    sM_1668B8 m;
    vu0CopyMtx_1668B8(&m, &D_004FF1A0_1668B8);
    Rotate_1668B8(&m, D_004FF150_1668B8, -*(float*)(self + 0x54));
    Rotate_1668B8(&m, D_004FF160_v1668B8, -*(float*)(self + 0x50));
    *out = Row_1668B8(m, 0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00166C60);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// 16-byte vector with inline constructors (a non-POD sibling of t4Vector, as in ai/ai); stores x, y, z, w.

// Global vectors and a matrix in .bss that the camera code reads (set up elsewhere at startup).
extern t4Vector D_004FF120_6C60 __asm__("D_004FF120");

extern t4Vector D_004FF160_6C60 __asm__("D_004FF160");

extern "C" void f_6C60_def(void* self, long id) __asm__("func_00166C60");


typedef long (*LongFn_6C60)(void*);
typedef sVec4 (*VecFn_6C60)(void*);
typedef void (*VecFn2_6C60)(sVec4*, void*);
typedef float (*FloatFn_6C60)(void*);
typedef void (*VoidFn_6C60)(void*);
typedef void (*IntFn_6C60)(void*, int);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline void Scale_6C60(sVec4& r, const sVec4& v, float s)
{
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
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline void Cross_6C60(sVec4& r, const sVec4& a, const sVec4& b)
{
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "o"(a), "m"(b)
        : "memory");
}

static inline sVec4 getVec_6C60(char* self, int slot)
{
    char* cam = *(char**)(self + 0x30);
    sVEntry<void>* cvt = *(sVEntry<void>**)cam;
    return ((VecFn_6C60)cvt[slot].fn)(cam + cvt[slot].delta);
}

extern "C" void f_6C60_def(void* p, long id)
{
    char* self = (char*)p;
    char* mgr = *(char**)(*(char**)(self + 0x30) + 4) + 0x6C0;
    sVEntry<void>* mvt = *(sVEntry<void>**)mgr;
    if (id == ((LongFn_6C60)mvt[7].fn)(mgr + mvt[7].delta)) {
        sVec4 pos = getVec_6C60(self, 4);
        sVec4 dir = getVec_6C60(self, 3);
        sVec4 v;
        sVec4 t;
        sVec4 u;
        *(int*)(self + 0x2C0) = 5000;
        *(float*)(self + 0x1E0) = 10.0f;
        *(float*)(self + 0x1DC) = 1.0f;
        *(int*)(self + 0x1E4) = 0;
        *(int*)(self + 0x2D0) = 0;
        *(int*)(self + 0x2D8) = 0;
        *(t4Vector*)(self + 0xC0) = D_004FF120_6C60;
        *(t4Vector*)(self + 0xD0) = D_004FF120_6C60;
        *(t4Vector*)(self + 0xE0) = D_004FF120_6C60;
        *(sVec4*)(self + 0x80) = pos;
        sVEntry<void>* svt1 = *(sVEntry<void>**)(self + 0x10);
        *(float*)(self + 0x1D4) = ((FloatFn_6C60)svt1[13].fn)(self + svt1[13].delta);
        *(int*)(self + 0x1E8) = 0;
        *(int*)(self + 0x1EC) = 0;
        Cross_6C60(t, (const sVec4&)D_004FF160_6C60, pos);
        v = t;
        *(float*)(self + 0x224) = 0.92f;
        *(sVec4*)(self + 0xF0) = t;
        *(int*)(self + 0x2E4) = 1;
        *(int*)(self + 0x2C4) = -1;
        *(float*)(self + 0x228) = 1.0f;
        *(int*)(self + 0x1F0) = 0;
        *(int*)(self + 0x2C8) = 0;
        *(int*)(self + 0x2DC) = 0;
        *(int*)(self + 0x1F8) = 0;
        *(int*)(self + 0x2E0) = 0;
        *(int*)(self + 0x204) = 0;
        *(int*)(self + 0x208) = 0;
        *(int*)(self + 0x210) = 0;
        *(int*)(self + 0x20C) = 0;
        *(int*)(self + 0x220) = 0;
        *(int*)(self + 0x21C) = 0;
        *(int*)(self + 0x214) = 0;
        *(int*)(self + 0x2E8) = 0;
        *(int*)(self + 0x2CC) = 0;
        *(int*)(self + 0x23C) = 0;
        *(t4Vector*)(self + 0x170) = D_004FF120_6C60;
        *(int*)(self + 0x244) = 0;
        *(int*)(self + 0x240) = 0;
        *(sVec4*)(self + 0x190) = dir;
        Scale_6C60(t, pos, 250.0f);
        *(int*)(self + 0x300) = 1;
        v = t;
        *(sVec4*)(self + 0x1A0) = t;
        *(float*)(self + 0x248) = 0.5f;
        *(int*)(self + 0x24C) = 0;
        *(int*)(self + 0x250) = 0;
        {
            char* cam = *(char**)(self + 0x30);
            sVEntry<void>* cvt = *(sVEntry<void>**)cam;
            ((VecFn2_6C60)cvt[4].fn)(&v, cam + cvt[4].delta);
        }
        *(int*)(self + 0x2EC) = 0;
        *(sVec4*)(self + 0x1B0) = v;
        *(int*)(self + 0x2B4) = 0;
        *(int*)(self + 0x2B8) = 0;
        *(int*)(self + 0x2F0) = 0;
        *(int*)(self + 0x2BC) = 0;
        *(int*)(self + 0x37C) = 0;
        int* q = (int*)(self + 0x340);
        for (int i = 4; i >= 0; i--) {
            q[0] = 0;
            q[5] = 0;
            q[10] = 0;
            q++;
        }
        {
            char* cam = *(char**)(self + 0x30);
            sVEntry<void>* cvt = *(sVEntry<void>**)cam;
            ((VecFn2_6C60)cvt[4].fn)(&t, cam + cvt[4].delta);
        }
        Scale_6C60(u, t, 2.0f);
        v = u;
        *(sVec4*)(self + 0x1C0) = u;
        *(sVec4*)(self + 0x100) = dir;
        *(int*)(self + 0x108) = 0;
        sVEntry<void>* svt2 = *(sVEntry<void>**)(self + 0x10);
        ((VoidFn_6C60)svt2[5].fn)(self + svt2[5].delta);
        sVEntry<void>* svt3 = *(sVEntry<void>**)(self + 0x10);
        ((IntFn_6C60)svt3[8].fn)(self + svt3[8].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00166F28);
#ifdef SKIP_ASM
struct sVE166F28a {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sVE166F28b {
    short delta;
    short index;
    void* (*fn)(void*, void*);
};

extern "C" void* func_00166F28(void* self)
{
    sVE166F28b* vt = *(sVE166F28b**)((char*)self + 0x10);
    char* o = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sVE166F28a* vt2 = *(sVE166F28a**)o;
    return vt[3].fn((char*)self + vt[3].delta, vt2[7].fn(o + vt2[7].delta));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00166F90);
#ifdef SKIP_ASM
struct sV_166F90 {
    float x, y, z, w;
    sV_166F90() {}
    sV_166F90(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    sV_166F90 Conj() const
    {
        sV_166F90 r(-x, -y, -z, w);
        return r;
    }
} __attribute__((aligned(16)));

struct sM_166F90 {
    float m[4][4];
} __attribute__((aligned(16)));


extern sM_166F90 D_004FF1A0_166F90 __asm__("D_004FF1A0");
extern sV_166F90 D_004FF150_166F90 __asm__("D_004FF150");
extern sV_166F90 D_004FF160_v166F90 __asm__("D_004FF160");
extern sV_166F90 D_004FF130_v166F90 __asm__("D_004FF130");
extern "C" void func_0031BE50(float* sout, float* cout, float x);

struct sPair_166F90 {
    sV_166F90 pos;
    sV_166F90 rot;
};

struct sCamXf_166F90 {
    char pad0[0x60];
    sPair_166F90 xf;
};

// func_0031B748 (bx/ps2main) fills *out from the matrix and returns out.
extern "C" sPair_166F90* func_0031B748_166F90(sPair_166F90* out, sM_166F90* m) __asm__("func_0031B748");

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_166F90(sM_166F90* dst, const sM_166F90* src)
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
static inline void vu0MulMtx_166F90(sM_166F90* dst, const sM_166F90* a, const sM_166F90* b)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, m = m * b).
static inline void vu0MulEqMtx_166F90(sM_166F90* m, const sM_166F90* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_166F90(sM_166F90* m, const sV_166F90* v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, 0x0(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "r"(v)
        : "memory");
}

static inline void SinCos_166F90(float* s, float* c, float angle)
{
    func_0031BE50(s, c, angle);
}

static inline void Rotate_166F90(sM_166F90* m, const sV_166F90& axis, float angle)
{
    sM_166F90 tm;
    vu0CopyMtx_166F90(&tm, m);
    sV_166F90 pos = *(sV_166F90*)tm.m[3];
    *(sV_166F90*)tm.m[3] = D_004FF130_v166F90;
    float s;
    float c;
    SinCos_166F90(&s, &c, angle);
    float one = 1.0f;
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
    m->m[0][0] = tx * x + c;
    m->m[1][0] = tx * y + sz;
    m->m[2][0] = tx * z - sy;
    m->m[0][1] = ty * x - sz;
    m->m[1][1] = ty * y + c;
    m->m[2][1] = ty * z + sx;
    m->m[0][2] = tz * x + sy;
    m->m[1][2] = tz * y - sx;
    m->m[2][2] = tz * z + c;
    m->m[3][0] = 0.0f;
    m->m[3][1] = 0.0f;
    m->m[3][2] = 0.0f;
    m->m[3][3] = one;
    m->m[0][3] = 0.0f;
    m->m[1][3] = 0.0f;
    m->m[2][3] = 0.0f;
    *(sV_166F90*)m->m[3] = pos;
    vu0MulEqMtx_166F90(m, &tm);
}

static inline sV_166F90 ConjF_166F90(float x, float y, float z, float w)
{
    sV_166F90 r;
    r.w = w;
    r.x = -x;
    r.y = -y;
    r.z = -z;
    return r;
}

extern "C" void func_00166F90_impl(char* self) __asm__("func_00166F90");
extern "C" void func_00166F90_impl(char* self)
{
    *(float*)(self + 0x4C) = 1.0f;
    sV_166F90* pos = (sV_166F90*)(self + 0x40);
    sM_166F90 m;
    vu0CopyMtx_166F90(&m, &D_004FF1A0_166F90);
    vu0TransMtx_166F90(&m, pos);
    Rotate_166F90(&m, D_004FF150_166F90, -*(float*)(self + 0x54));
    Rotate_166F90(&m, D_004FF160_v166F90, -*(float*)(self + 0x50));
    {
        sPair_166F90 xf;
        func_0031B748_166F90(&xf, &m);
        ((sCamXf_166F90*)self)->xf = xf;
    }
    *(sV_166F90*)(self + 0x70) = ConjF_166F90(*(float*)(self + 0x70), *(float*)(self + 0x74), *(float*)(self + 0x78), *(float*)(self + 0x7C));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001673A0);
#ifdef SKIP_ASM
extern "C" void* func_001673A0(void* self, void* a1)
{
    *(cQuad128*)self = *(cQuad128*)((char*)a1 + 0x60);
    *(cQuad128*)((char*)self + 0x10) = *(cQuad128*)((char*)a1 + 0x70);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001673B8);
#ifdef SKIP_ASM
extern "C" void* func_001673B8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001673F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_1673F8 {
    float r, g, b, a;
    sColor_1673F8(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sFlags_1673F8 {
    unsigned char a;
    unsigned int b : 24;
    sFlags_1673F8(int aa, int ab) : a(aa), b(ab) {}
};

struct sMatrix_1673F8 {
    float m[4][4];
    sMatrix_1673F8(const float& m00, const float& m01, const float& m02, const float& m03,
                   const float& m10, const float& m11, const float& m12, const float& m13,
                   const float& m20, const float& m21, const float& m22, const float& m23,
                   const float& m30, const float& m31, const float& m32, const float& m33)
    {
        m[0][0] = m00; m[1][0] = m10; m[2][0] = m20; m[3][0] = m30;
        m[0][1] = m01; m[1][1] = m11; m[2][1] = m21; m[3][1] = m31;
        m[0][2] = m02; m[1][2] = m12; m[2][2] = m22; m[3][2] = m32;
        m[0][3] = m03; m[1][3] = m13; m[2][3] = m23; m[3][3] = m33;
    }
};

extern sColor_1673F8 D_004C5020;
extern sColor_1673F8 D_004C5030;
extern sColor_1673F8 D_004C5040;
extern sColor_1673F8 D_004C5050;
extern sColor_1673F8 D_004C5060;
extern sColor_1673F8 D_004C5070;
extern sColor_1673F8 D_004C5080;
extern sColor_1673F8 D_004C5090;
extern sColor_1673F8 D_004C50A0;
extern sColor_1673F8 D_004C50B0;
extern sColor_1673F8 D_004C50C0;
extern sColor_1673F8 D_004C50D0;
extern sColor_1673F8 D_004C50E0;
extern sColor_1673F8 D_004C50F0;
extern sColor_1673F8 D_004C5100;
extern sColor_1673F8 D_004C5110;
extern sColor_1673F8 D_004C5120;
extern sColor_1673F8 D_004C5130;
extern sColor_1673F8 D_004C5140;
extern sColor_1673F8 D_004C5150;
extern sColor_1673F8 D_004C5160;
extern sColor_1673F8 D_004C5170;
extern sColor_1673F8 D_004C5180;
extern sColor_1673F8 D_004C5190;
extern sColor_1673F8 D_004C51A0;
extern sColor_1673F8 D_004C51B0;
extern sColor_1673F8 D_004C51C0;
extern sColor_1673F8 D_004C51D0;
extern sColor_1673F8 D_004C51E0;
extern sColor_1673F8 D_004C51F0;
extern sColor_1673F8 D_004C5200;
extern sColor_1673F8 D_004C5210;
extern sColor_1673F8 D_004C5220;
extern sColor_1673F8 D_004C5230;
extern sColor_1673F8 D_004C5240;
extern sColor_1673F8 D_004C5250;
extern sColor_1673F8 D_004C5260;
extern sColor_1673F8 D_004C5270;
extern sColor_1673F8 D_004C5280;
extern sColor_1673F8 D_004C5290;
extern sColor_1673F8 D_004C52A0;
extern sColor_1673F8 D_004C52B0;
extern sColor_1673F8 D_004C52C0;
extern sColor_1673F8 D_004C52D0;
extern sColor_1673F8 D_004C52E0;
extern sColor_1673F8 D_004C52F0;
extern sColor_1673F8 D_004C5300;
extern sColor_1673F8 D_004C5310;
extern sColor_1673F8 D_004C5320;
extern sColor_1673F8 D_004C5330;
extern sColor_1673F8 D_004C5340;
extern sColor_1673F8 D_004C5350;
extern sColor_1673F8 D_004C5360;
extern sColor_1673F8 D_004C5370;
extern sColor_1673F8 D_004C5380;
extern sColor_1673F8 D_004C5390;
extern int D_004A4D40;
extern int D_004A4D44;
extern sFlags_1673F8 D_004A4D48;
extern sFlags_1673F8 D_004A4D50;
extern sFlags_1673F8 D_004A4D58;
extern sFlags_1673F8 D_004A4D60;
extern sFlags_1673F8 D_004A4D68;
extern sFlags_1673F8 D_004A4D70;
extern sFlags_1673F8 D_004A4D78;
extern sFlags_1673F8 D_004A4D80;
extern sFlags_1673F8 D_004A4D88;
extern sFlags_1673F8 D_004A4D90;
extern sFlags_1673F8 D_004A4D98;
extern sFlags_1673F8 D_004A4DA0;
extern sFlags_1673F8 D_004A4DA8;
extern sFlags_1673F8 D_004A4DB0;
extern sMatrix_1673F8 D_004C53A0;
extern sFlags_1673F8 D_004A4DC0;
extern int D_004A4DD0[2];

extern "C" void func_0015E2A8(void);

// The unit declares func_001673F8 later as returning void*; bind this body by asm label.
extern "C" void func_001673F8_impl(int initialize, int priority) __asm__("func_001673F8");

extern "C" void func_001673F8_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004C5020) sColor_1673F8(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004C5030) sColor_1673F8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5040) sColor_1673F8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5050) sColor_1673F8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004C5060) sColor_1673F8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004C5070) sColor_1673F8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5080) sColor_1673F8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004C5090) sColor_1673F8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004C50A0) sColor_1673F8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004C50B0) sColor_1673F8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004C50C0) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C50D0) sColor_1673F8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C50E0) sColor_1673F8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004C50F0) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5100) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5110) sColor_1673F8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5120) sColor_1673F8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5130) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5140) sColor_1673F8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5150) sColor_1673F8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5160) sColor_1673F8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5170) sColor_1673F8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004C5180) sColor_1673F8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004C5190) sColor_1673F8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004C51A0) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C51B0) sColor_1673F8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004C51C0) sColor_1673F8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004C51D0) sColor_1673F8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004C51E0) sColor_1673F8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004C51F0) sColor_1673F8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004C5200) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5210) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5220) sColor_1673F8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004C5230) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5240) sColor_1673F8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004C5250) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5260) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5270) sColor_1673F8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5280) sColor_1673F8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004C5290) sColor_1673F8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004C52A0) sColor_1673F8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004C52B0) sColor_1673F8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004C52C0) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C52D0) sColor_1673F8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004C52E0) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C52F0) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5300) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5310) sColor_1673F8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004C5320) sColor_1673F8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004C5330) sColor_1673F8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004C5340) sColor_1673F8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004C5350) sColor_1673F8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004C5360) sColor_1673F8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004C5370) sColor_1673F8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004C5380) sColor_1673F8(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004C5390) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        D_004A4D44 = 0;
        D_004A4D40 = 0;
        new (&D_004A4D48) sFlags_1673F8(0, 0);
        new (&D_004A4D50) sFlags_1673F8(1, 1);
        new (&D_004A4D58) sFlags_1673F8(2, 2);
        new (&D_004A4D60) sFlags_1673F8(3, 3);
        new (&D_004A4D68) sFlags_1673F8(4, 4);
        new (&D_004A4D70) sFlags_1673F8(5, 5);
        new (&D_004A4D78) sFlags_1673F8(6, 6);
        new (&D_004A4D80) sFlags_1673F8(7, 7);
        new (&D_004A4D88) sFlags_1673F8(8, 8);
        new (&D_004A4D90) sFlags_1673F8(9, 9);
        new (&D_004A4D98) sFlags_1673F8(10, 10);
        new (&D_004A4DA0) sFlags_1673F8(11, 11);
        new (&D_004A4DA8) sFlags_1673F8(12, 12);
        new (&D_004A4DB0) sFlags_1673F8(13, 13);
        new (&D_004C53A0) sMatrix_1673F8(0.0f, 0.0f, 1.0f, 0.0f,
                                         -1.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);
        func_0015E2A8();
        new (&D_004A4DC0) sFlags_1673F8(-1, -1);
        D_004A4DD0[0] = 0x3D;
        D_004A4DD0[1] = 0x3D;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167D88);
#ifdef SKIP_ASM
extern "C" float func_00167D88(void)
{
    return 0.1f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167D98);
#ifdef SKIP_ASM
extern "C" float func_00167D98(void* self)
{
    return 0.10000000149011612f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167DA8);
#ifdef SKIP_ASM
extern "C" float func_00167DA8(void* self)
{
    return 0.10000000149011612f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167DB8);
#ifdef SKIP_ASM
struct sCamFilter {
    float field_0x00[5];
    float field_0x14[5];
    int field_0x28[5];
    int field_0x3C;
};

extern "C" void func_00167DB8(sCamFilter* self, float value)
{
    int i;
    self->field_0x3C = 0;
    for (i = 0; i < 5; i++) {
        self->field_0x00[i] = value;
        self->field_0x14[i] = value;
        self->field_0x28[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167DE8);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167DE8(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E18__FPv);
#ifdef SKIP_ASM
void func_00167E18(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E20__FPv);
#ifdef SKIP_ASM
void func_00167E20(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E28__FPv);
#ifdef SKIP_ASM
void func_00167E28(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E30);
#ifdef SKIP_ASM
extern "C" float func_00167E30(void* self)
{
    return 0.7853981852531433f;
}
#endif

extern cQuad128 D_004FF120;

//100%
INCLUDE_ASM("camera/camera", func_00167E40);
#ifdef SKIP_ASM
extern "C" void* func_00167E40(void* self)
{
    *(cQuad128*)self = D_004FF120;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E58__FPv);
#ifdef SKIP_ASM
void func_00167E58(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E60__FPv);
#ifdef SKIP_ASM
void func_00167E60(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E68);
#ifdef SKIP_ASM
class func_00167E68_cObj {
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

extern "C" void func_00167E68(func_00167E68_cObj* self)
{
    self->v05();
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E90__FPv);
#ifdef SKIP_ASM
void func_00167E90(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E98__FPv);
#ifdef SKIP_ASM
int func_00167E98(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167EA0);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167EA0(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167ED0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_00167ED0(void *arg0, long long *arg1) {
    (*(long long *)((char*)(arg0) + (0x20))) = (long long) *arg1;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167EE0);
#ifdef SKIP_ASM
extern "C" void func_00167EE0(void *arg0, float fparg0) {
    (*(float *)((char*)(arg0) + (0x30))) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167EE8);
#ifdef SKIP_ASM
extern "C" void func_00167EE8(void *arg0, float fparg0) {
    (*(float *)((char*)(arg0) + (0x34))) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167EF0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *func_00167EF0(long long *arg0, void *arg1) {
    *arg0 = (*(long long *)((char*)(arg1) + (0x20)));
    return arg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167F00);
#ifdef SKIP_ASM
extern "C" float func_00167F00(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x30)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167F08);
#ifdef SKIP_ASM
extern "C" float func_00167F08(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x34)));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167F10);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167F10(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167F40);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167F40(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167F70);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167F70(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseAlgorithm_getTargetPos);
#ifdef SKIP_ASM
extern "C" void* cChaseAlgorithm_getTargetPos(void* self, void* a1)
{
    *(cQuad128*)self = *(cQuad128*)((char*)a1 + 0x20);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_shouldSnapOnUpdate__FPv);
#ifdef SKIP_ASM
int cLookAtChaseAlgorithm_shouldSnapOnUpdate(void* self)
{
    return *(int*)((char*)self + 0x2F0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraController_update__FPv);
#ifdef SKIP_ASM
void cCameraController_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167FC0);
#ifdef SKIP_ASM
extern "C" void func_00167FC0(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0xC))) = arg1;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167FC8);
#ifdef SKIP_ASM
extern "C" int func_00167FC8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xC)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167FD0);
#ifdef SKIP_ASM
extern "C" int func_00167FD0(int *arg0) {
    return *arg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167FD8);
#ifdef SKIP_ASM
extern "C" int func_00167FD8(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167FE0);
#ifdef SKIP_ASM
extern "C" int func_00167FE0(void *arg0) {
    return (*(int *)((char*)(arg0) + (8)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167FE8);
#ifdef SKIP_ASM
extern "C" int func_00167FE8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x18)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167FF0);
#ifdef SKIP_ASM
extern "C" int func_00167FF0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167FF8);
#ifdef SKIP_ASM
extern "C" void func_00167FF8(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168000);
#ifdef SKIP_ASM
extern "C" void func_00168000(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168008);
#ifdef SKIP_ASM
extern "C" void func_00168008(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168010);
#ifdef SKIP_ASM
extern "C" void func_00168010(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168018);
#ifdef SKIP_ASM
extern "C" int func_00168018(void) {
    return 0x3D;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168020);
#ifdef SKIP_ASM
extern "C" int func_00168020(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x18)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168028);
#ifdef SKIP_ASM
extern "C" int func_00168028(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x1C)));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSimpleCamera__cSimpleCamera);
#ifdef SKIP_ASM
extern void* D_0045B8B0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cSimpleCamera__cSimpleCamera(void* self, int flags)
{
    *(void***)((char*)self + 0x90) = D_0045B8B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* func_0015D928(void* self);

//100%
INCLUDE_ASM("camera/camera", func_00168060__FPv);
#ifdef SKIP_ASM
void* func_00168060(void* self)
{
    return func_0015D928(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168080);
#ifdef SKIP_ASM
extern "C" float func_00168080(float *arg0) {
    return *arg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168088);
#ifdef SKIP_ASM
extern "C" float func_00168088(void *arg0) {
    return (*(float *)((char*)(arg0) + (4)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168090);
#ifdef SKIP_ASM
extern "C" float func_00168090(void *arg0) {
    return (*(float *)((char*)(arg0) + (8)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168098);
#ifdef SKIP_ASM
extern "C" int func_00168098(int arg0) {
    return arg0 + 0x40;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001680A0);
#ifdef SKIP_ASM
extern "C" int func_001680A0(int arg0) {
    return arg0 + 0x20;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001680A8);
#ifdef SKIP_ASM
extern "C" int func_001680A8(int arg0) {
    return arg0 + 0x20;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001680B0);
#ifdef SKIP_ASM
extern "C" int func_001680B0(int arg0) {
    return arg0 + 0x80;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001680B8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_001680B8(void *arg0, long long *arg1) {
    (*(long long *)((char*)(arg0) + (0x20))) = (long long) *arg1;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001680C8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_001680C8(void *arg0, long long *arg1) {
    (*(long long *)((char*)(arg0) + (0x30))) = (long long) *arg1;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001680D8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_001680D8(void *arg0, void *arg1) {
    (*(long long *)((char*)(arg0) + (0x20))) = (long long) (*(long long *)((char*)(arg1) + (0)));
    (*(long long *)((char*)(arg0) + (0x30))) = (long long) (*(long long *)((char*)(arg1) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001680F0);
#ifdef SKIP_ASM
extern "C" void func_001680F0(float *arg0, float fparg0) {
    *arg0 = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001680F8);
#ifdef SKIP_ASM
extern "C" void func_001680F8(void *arg0, float fparg0) {
    (*(float *)((char*)(arg0) + (0x14))) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168100);
#ifdef SKIP_ASM
extern "C" void func_00168100(void *arg0, float fparg0) {
    (*(float *)((char*)(arg0) + (0x10))) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168108);
#ifdef SKIP_ASM
extern "C" void func_00168108(void *arg0, float fparg0) {
    (*(float *)((char*)(arg0) + (0xC))) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168110);
#ifdef SKIP_ASM
extern "C" int func_00168110(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xA0)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168118);
#ifdef SKIP_ASM
extern "C" int func_00168118(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xA4)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168120);
#ifdef SKIP_ASM
extern "C" int func_00168120(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xA8)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168128);
#ifdef SKIP_ASM
extern "C" int func_00168128(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xAC)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168130);
#ifdef SKIP_ASM
extern "C" int func_00168130(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xB4)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168138);
#ifdef SKIP_ASM
extern "C" int func_00168138(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xB0)));
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168140);
#ifdef SKIP_ASM
extern "C" void func_00168140(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (4))) = arg1;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168148);
#ifdef SKIP_ASM
extern "C" int func_00168148(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168150);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF160_8150 __asm__("D_004FF160");

// PORT: the unit declares func_00168150 as returning void*; this is the real (void) definition.
extern "C" void func_00168150_def(char* self) __asm__("func_00168150");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_168150b(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub2_168150(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector cmAdd_168150(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScale_168150(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_168150(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float cmLength_168150b(const t4Vector& v)
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

// cLookAtChaseAlgorithm::verify(void): if the camera is closer to / farther from its target than the
// allowed range, snap it back behind the target.
extern "C" void func_00168150_def(char* self)
{
    t4Vector d = cmSub_168150b(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40));
    float len = cmLength_168150b(d);
    if (len < 20.0f || len > 1.0000000200408773e+20f) {
        char* o = *(char**)(self + 0x30);
        t4Vector x = *(t4Vector*)(*(char**)(o + 4) + 0x1B0);
        sVEntry<t4Vector(void*)>* e = &(*(sVEntry<t4Vector(void*)>**)o)[1];
        t4Vector p = e->fn(o + e->delta);
        int one = 1;
        float k50 = 50.0f;
        t4Vector np = cmAdd_168150(cmSub2_168150(p, cmScale_168150(x, 200.0f)), cmScaleP_168150(&D_004FF160_8150, k50));
        *(t4Vector*)(self + 0x40) = np;
        *(t4Vector*)(self + 0x20) = p;
        *(int*)(self + 0x2F0) = one;
    }
}
#endif

extern "C" void* func_001673F8(int, int);

//99.38%
INCLUDE_ASM("camera/camera", func_00168298__FPv);
#ifdef SKIP_ASM
void* func_00168298(void* self)
{
    return func_001673F8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001682B8);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C628[];

extern "C" void* func_001682B8(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0xD;
    *(void***)((char*)self + 0x10) = D_0045C628;
    return self;
}
#endif

extern void* D_0045C628[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_001682F8__FPv);
#ifdef SKIP_ASM
void* func_001682F8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C628;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168320__FPv);
#ifdef SKIP_ASM
float func_00168320(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168330__FPvT0);
#ifdef SKIP_ASM
void func_00168330(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168338);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector D_004FF160_8338 __asm__("D_004FF160");

// The per-frame camera input record (0x70 bytes) that func_00162568_8338 fills from the target for the chase algorithms.
extern "C" void func_00162568_8338(void* self, sCamInfo* out, float a, float b, float c, float d,
                              float e, float f, float g, float h) __asm__("func_00162568");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScale_001607E0_8338(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector cmAdd_001607E0_8338(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_1848(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_168150(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float cmLength_168150(const t4Vector& v)
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


// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void cmScaleEq_8338(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

extern "C" void func_00168338_v(char* self) __asm__("func_00168338");

extern "C" void func_00168338_v(char* self)
{
    sCamInfo info;
    func_00162568_8338(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    *(t4Vector*)(self + 0x20) = cmAdd_001607E0_8338(info.pos, cmScaleP_1848(&D_004FF160_8338, 2.3602666854858398f));
    t4Vector d = cmSub_168150(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40));
    float len = cmLength_168150(d);
    if (len > 0.0f) {
        cmScaleEq_8338(d, 180.19183349609375f / len);
        t4Vector e = cmSub_168150(*(t4Vector*)(self + 0x20), d);
        float t;
        if (len >= 188.07302856445312f) {
            if (len <= 775.4580078125f) {
                t = (len - 188.07302856445312f) / 587.385009765625f;
            } else {
                t = 1.0f;
            }
        } else {
            t = 0.0f;
        }
        float f = (1.0f - t) * 0.9916903972625732f + t * 0.8601375222206116f;
        *(t4Vector*)(self + 0x40) = cmAdd_001607E0_8338(cmScale_001607E0_8338(*(t4Vector*)(self + 0x40), f), cmScale_001607E0_8338(e, 1.0f - f));
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168508);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF160_8508 __asm__("D_004FF160");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScale_001607E0_8508(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector cmAdd_001607E0_8508(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector cmGet_001607E0_8508(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* e = &(*(sVEntry<t4Vector(void*)>**)o)[slot];
    return e->fn(o + e->delta);
}

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).


// PORT: PS2-only VU0 inline asm (vector subtract).


extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);
extern "C" void f_6C60_8508(void* self, long id) __asm__("func_00166C60");

extern "C" void func_00168508_v(char* self, long id) __asm__("func_00168508");

extern "C" void func_00168508_v(char* self, long id)
{
    char* obj = *(char**)(*(char**)(self + 0x30) + 0x4) + 0x6C0;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        t4Vector nv = cmSub_168150(cmAdd_001607E0_8508(cmGet_001607E0_8508(self, 1), cmScaleP_1848(&D_004FF160_8508, 100.0f)), cmScale_001607E0_8508(cmGet_001607E0_8508(self, 4), 200.0f));
        *(t4Vector*)(self + 0x40) = nv;
        f_6C60_8508(self, id);
        func_00166550(self, 100.0f, 100.0f);
        sVEntry<void(void*)>* vt2 = *(sVEntry<void(void*)>**)(self + 0x10);
        vt2[5].fn(self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168650);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C6A0[];

extern "C" void* func_00168650(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x20;
    *(void***)((char*)self + 0x10) = D_0045C6A0;
    return self;
}
#endif

extern void* D_0045C6A0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00168690__FPv);
#ifdef SKIP_ASM
void* func_00168690(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C6A0;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001686B8__FPv);
#ifdef SKIP_ASM
float func_001686B8(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001686C8__FPvT0);
#ifdef SKIP_ASM
void func_001686C8(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", func_001686D0);
#ifdef SKIP_ASM
extern "C" void func_00162C78(void* self, void* info);
extern "C" void func_00163010(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void func_00163270(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void func_00162B90(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void func_001643A8(void* self, void* info, float a, float b, float c);
extern "C" void func_00163158(void* self, void* info, float a, float b);

struct sVec_1686D0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEv_1686D0 {
    short delta;
    short index;
    sVec_1686D0 (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec_1686D0 Sub_1686D0(const sVec_1686D0& a, const sVec_1686D0& b)
{
    sVec_1686D0 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec_1686D0 Add_1686D0(const sVec_1686D0& a, const sVec_1686D0& b)
{
    sVec_1686D0 r;
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec_1686D0 Scale_1686D0(const sVec_1686D0& v, float s)
{
    sVec_1686D0 r;
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_1686D0(sVec_1686D0& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1686D0(const sVec_1686D0& v)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_1686D0(const sVec_1686D0& a, const sVec_1686D0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

static inline sVec_1686D0 Get_1686D0(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEv_1686D0* e = &(*(sVEv_1686D0**)o)[slot];
    return e->fn(o + e->delta);
}

extern "C" void func_001686D0(char* self)
{
    sCamInfo_001607E0 info;
    func_00162568(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f, 0.8513929843902588f,
                  0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f, 0.6000000238418579f);
    float zero = 0.0f;
    func_00162A20(self, (char*)&info);
    func_00162B80(self, &info);
    func_00162C78(self, &info);
    func_00163010(self, &info, 193.1998291015625f, zero, 76.68663024902344f, zero, 1.7668397426605225f,
                  0.9239780306816101f, 0.907414972782135f);
    func_00163270(self, &info, zero, 1.2432044744491577f, zero, zero, 0.9783917665481567f);
    func_001633B0(self, &info, zero, 0.9927757978439331f, 1.0f, 0.949999988079071f);
    func_00162B90(self, &info, 0.721226155757904f, 0.01082259975373745f, 0.22186216711997986f, 0.5425665378570557f,
                  0.9241908192634583f);
    func_001643A8(self, &info, 15.0f, 1.5f, 1.5f);
    func_00163158(self, &info, 0.13458502292633057f, 1.7663990259170532f);

    sVec_1686D0 d = Sub_1686D0(*(sVec_1686D0*)(self + 0x20), *(sVec_1686D0*)(self + 0x40));
    float len = Length_1686D0(d);
    ScaleEq_1686D0(d, (len + 294.9358825683594f) / len);
    sVec_1686D0 pos = Add_1686D0(*(sVec_1686D0*)(self + 0x40), d);
    sVec_1686D0 off = Sub_1686D0(Get_1686D0(self, 1), pos);
    float t = Dot_1686D0(off, d) / Dot_1686D0(d, d);
    sVec_1686D0 np = Add_1686D0(Scale_1686D0(d, t), pos);
    *(sVec_1686D0*)(self + 0x20) = np;
    *(sVec_1686D0*)(self + 0x40) = pos;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168940);
#ifdef SKIP_ASM
class cCamTargetK8940 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int v07();
};
class cCamCtrlK8940 {
public:
    char pad_0x00[0x10];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

extern "C" void func_00168940(cCamCtrlK8940* self, int id)
{
    char* rider = *(char**)(*(char**)((char*)self + 0x30) + 4);
    if (id == ((cCamTargetK8940*)(rider + 0x6C0))->v07()) {
        func_00166C60(self, id);
        func_00166550(self, 559.7440185546875f, 300.0f);
        self->v05();
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001689C8);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C538[];

extern "C" void* func_001689C8(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x9;
    *(void***)((char*)self + 0x10) = D_0045C538;
    return self;
}
#endif

extern void* D_0045C538[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00168A08__FPv);
#ifdef SKIP_ASM
void* func_00168A08(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C538;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168A30);
#ifdef SKIP_ASM
extern "C" float func_00168A30(void* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168A40);
#ifdef SKIP_ASM
extern "C" void func_00168A40(void* self, void* out)
{
    *(float*)((char*)out + 0x4) = 6.654887676239014f;
    *(float*)((char*)out + 0x0) = 16.140756607055664f;
    *(float*)((char*)out + 0xC) = 0.8425687551498413f;
    *(float*)((char*)out + 0x8) = 0.9264262914657593f;
    *(int*)((char*)out + 0x10) = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", func_00168A70);
#ifdef SKIP_ASM
extern "C" void func_00162C78(void* self, void* info);
extern "C" void func_00163010(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void func_00163270(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void func_00162B90(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void func_001635F8(void* self, void* info, float a, float b, float c, float d);
extern "C" void func_00164878(void* self, void* info, float a);
extern "C" void func_001646A0(char* self, char* p, float k, float t);
extern "C" void func_001641C0(char* self, char* p, float a, float b, float c, float d);
extern "C" void func_001643A8(void* self, void* info, float a, float b, float c);
extern "C" void func_00163158(void* self, void* info, float a, float b);
// PORT: func_00163450 is defined with 7 float parameters; this caller passes 8.
void func_00163450_8(char* self, char* trig, float a, float b, float c, float d, float e, float f, float g,
                     float h) __asm__("func_00163450");

extern "C" void func_00168A70(char* self)
{
    sCamInfo_001607E0 info;
    func_00162568(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f, 0.8513929843902588f,
                  0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f, 0.6000000238418579f);
    float zero = 0.0f;
    float k = 0.9800000190734863f;
    func_00162998(self, &info);
    func_00162A20(self, (char*)&info);
    func_00162B80(self, &info);
    func_00162C78(self, &info);
    func_00163010(self, &info, 555.0844116210938f, 11.549837112426758f, 76.68663024902344f, 307.0549621582031f,
                  1.7668397426605225f, 0.9239780306816101f, 0.907414972782135f);
    func_00163270(self, &info, 12.331472396850586f, 84.0909652709961f, 38.64322280883789f, 93.62934875488281f,
                  0.9783917665481567f);
    func_001633B0(self, &info, zero, 0.8567028641700745f, 1.0f, 0.949999988079071f);
    func_00163450_8(self, (char*)&info, 348.64111328125f, 66.33821868896484f, 162.2997283935547f, 2.00368070602417f,
                    zero, k, 0.9599999785423279f, 0.4552607834339142f);
    func_00162B90(self, &info, -8.730510711669922f, 64.15198516845703f, 58.114906311035156f, 5.931424617767334f,
                  0.9241908192634583f);
    func_001635F8(self, &info, 1.878627061843872f, 2.494720935821533f, 0.34134337306022644f, 0.8672914505004883f);
    func_00164878(self, &info, 1.5269116163253784f);
    func_001646A0(self, (char*)&info, 200.33114624023438f, 0.9706981778144836f);
    func_001641C0(self, (char*)&info, 600.0f, 300.0f, 100.0f, k);
    func_001643A8(self, &info, 15.0f, 1.5f, 1.5f);
    func_00163158(self, &info, -0.2944446802139282f, 0.6521888375282288f);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168C50);
#ifdef SKIP_ASM
extern "C" void func_00168C50(cCamCtrlK8940* self, int id)
{
    char* rider = *(char**)(*(char**)((char*)self + 0x30) + 4);
    if (id == ((cCamTargetK8940*)(rider + 0x6C0))->v07()) {
        func_00166C60(self, id);
        func_00166550(self, 559.7440185546875f, 300.0f);
        self->v05();
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168CD8);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C5B0[];

extern "C" void* func_00168CD8(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x9;
    *(void***)((char*)self + 0x10) = D_0045C5B0;
    return self;
}
#endif

extern void* D_0045C5B0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00168D18__FPv);
#ifdef SKIP_ASM
void* func_00168D18(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C5B0;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168D40);
#ifdef SKIP_ASM
extern "C" float func_00168D40(void* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168D50);
#ifdef SKIP_ASM
extern "C" void func_00168D50(void* self, void* out)
{
    *(float*)((char*)out + 0x4) = 6.654887676239014f;
    *(float*)((char*)out + 0x0) = 16.140756607055664f;
    *(float*)((char*)out + 0xC) = 0.8425687551498413f;
    *(float*)((char*)out + 0x8) = 0.9264262914657593f;
    *(int*)((char*)out + 0x10) = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", func_00168D80);
#ifdef SKIP_ASM
extern "C" void func_00162C78(void* self, void* info);
extern "C" void func_00163010(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void func_00163270(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void func_00162B90(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void func_001635F8(void* self, void* info, float a, float b, float c, float d);
extern "C" void func_00164878(void* self, void* info, float a);
extern "C" void func_001646A0(char* self, char* p, float k, float t);
extern "C" void func_001641C0(char* self, char* p, float a, float b, float c, float d);
extern "C" void func_001643A8(void* self, void* info, float a, float b, float c);
extern "C" void func_00163158(void* self, void* info, float a, float b);
// PORT: func_00163450 is defined with 7 float parameters; this caller passes 8.
void func_00163450_8(char* self, char* trig, float a, float b, float c, float d, float e, float f, float g,
                     float h) __asm__("func_00163450");

extern "C" void func_00168D80(char* self)
{
    sCamInfo_001607E0 info;
    func_00162568(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f, 0.8513929843902588f,
                  0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f, 0.6000000238418579f);
    float zero = 0.0f;
    float k = 0.9800000190734863f;
    func_00162998(self, &info);
    func_00162A20(self, (char*)&info);
    func_00162B80(self, &info);
    func_00162C78(self, &info);
    func_00163010(self, &info, 555.0844116210938f, 11.549837112426758f, 76.68663024902344f, 307.0549621582031f,
                  1.7668397426605225f, 0.9239780306816101f, 0.907414972782135f);
    func_00163270(self, &info, 12.331472396850586f, 84.0909652709961f, 38.64322280883789f, 93.62934875488281f,
                  0.9783917665481567f);
    func_001633B0(self, &info, zero, 0.8567028641700745f, 1.0f, 0.949999988079071f);
    func_00163450_8(self, (char*)&info, 348.64111328125f, 66.33821868896484f, 162.2997283935547f, 2.00368070602417f,
                    zero, k, 0.9599999785423279f, 0.4552607834339142f);
    func_00162B90(self, &info, -8.730510711669922f, 64.15198516845703f, 58.114906311035156f, 5.931424617767334f,
                  0.9241908192634583f);
    func_001635F8(self, &info, 1.878627061843872f, 2.494720935821533f, 0.34134337306022644f, 0.8672914505004883f);
    func_00164878(self, &info, 1.5269116163253784f);
    func_001646A0(self, (char*)&info, 200.33114624023438f, 0.9706981778144836f);
    func_001641C0(self, (char*)&info, 600.0f, 300.0f, 100.0f, k);
    func_001643A8(self, &info, 15.0f, 1.5f, 1.5f);
    func_00163158(self, &info, 0.2944446802139282f, 0.6521888375282288f);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168F60);
#ifdef SKIP_ASM
extern "C" void func_00168F60(cCamCtrlK8940* self, int id)
{
    char* rider = *(char**)(*(char**)((char*)self + 0x30) + 4);
    if (id == ((cCamTargetK8940*)(rider + 0x6C0))->v07()) {
        func_00166C60(self, id);
        func_00166550(self, 559.7440185546875f, 300.0f);
        self->v05();
    }
}
#endif


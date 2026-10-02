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
extern "C" void* func_001619A8(void* self, int idx, int a);
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
    *(void**)((char*)self + 0xA8) = func_001619A8(cMemMan_alloc(0x44, D_0045B760, 0, 0), idx, (int)self);
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
extern "C" void func_0015CF80(void* self, int msg);
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
    func_0015CF80(self->manual, 0x51);
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

INCLUDE_ASM("camera/camera", func_0015E050);

INCLUDE_ASM("camera/camera", func_0015E2A8);

INCLUDE_ASM("camera/camera", func_0015E360);

INCLUDE_ASM("camera/camera", func_0015E460);

INCLUDE_ASM("camera/camera", func_0015E668);

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
extern "C" void* func_0011FF48(void* dst, void* self);

extern "C" void* func_0015F568(void* self, void* a1)
{
    func_0011FF48(self, *(void**)((char*)a1 + 0x4));
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

INCLUDE_ASM("camera/camera", func_0015F780);

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
INCLUDE_ASM("camera/camera", func_0015F938);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
extern void* D_0045BD78[];
// PORT: cQuad128 (the unit's 128-bit TImode typedef) is a 16-byte register copy.
extern cQuad128 D_004FF130;
extern "C" float func_00167E30(void* self);

extern "C" void* func_0015F938(void* self)
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

INCLUDE_ASM("camera/camera", func_0015F9B8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", func_0015FD08);
#ifdef SKIP_ASM
extern "C" void* func_0015F938(void* self);
extern void* D_0045BD10[];

extern "C" void* func_0015FD08(void* self)
{
    func_0015F938(self);
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
// PORT: func_00320BF0 returns a float (input axis value, $f0).
extern "C" float func_00320BF0(int pad, int axis);
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
    float speed = func_00320BF0(pad, 0x34);
    float ry = func_00320BF0(pad, 0x31);
    float rx = func_00320BF0(pad, 0x30);
    float mf = func_00320BF0(pad, 0x32);
    float ms = func_00320BF0(pad, 0x33);
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
INCLUDE_ASM("camera/camera", func_0015FFB0);
#ifdef SKIP_ASM
extern "C" void* func_0015F938(void* self);
extern void* D_0045BCA8[];

extern "C" void* func_0015FFB0(void* self)
{
    func_0015F938(self);
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

INCLUDE_ASM("camera/camera", func_00160228);

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

INCLUDE_ASM("camera/camera", func_001604B8);

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

INCLUDE_ASM("camera/camera", func_00160BF8);

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

INCLUDE_ASM("camera/camera", func_00161060);

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

INCLUDE_ASM("camera/camera", func_001613F0);

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

INCLUDE_ASM("camera/camera", func_00161728);

INCLUDE_ASM("camera/camera", func_00161848);

//100%
INCLUDE_ASM("camera/camera", func_00161950);
#ifdef SKIP_ASM
struct func_00161950_flags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
};

extern int D_004A4DD0[2];

extern "C" void func_00161950(void* self, int i)
{
    *(int*)((char*)self + 0x1C) = 0x3D;
    *(int*)((char*)self + 0x38) = 0;
    ((func_00161950_flags*)((char*)self + 0x30))->b1 = 0; ((func_00161950_flags*)((char*)self + 0x30))->b0 = 0; ((func_00161950_flags*)((char*)self + 0x30))->b2 = 0;
    *(int*)((char*)self + 0x3C) = 0;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x2C) = 0x3D;
    *(int*)((char*)self + 0x24) = D_004A4DD0[i];
    *(int*)((char*)self + 0x28) = 0x3D;
    *(int*)((char*)self + 0x34) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", func_001619A8);
#ifdef SKIP_ASM
extern void* D_0045B908[];
extern "C" void* cCameraController_cCameraController(void* self, int a);
extern "C" void func_00161950(void* self, int idx);

extern "C" void* func_001619A8(void* self, int idx, int a)
{
    cCameraController_cCameraController(self, a);
    *(void***)((char*)self + 0x14) = D_0045B908;
    *(int*)((char*)self + 0x0) = 2;
    *(int*)((char*)self + 0x4) = 1;
    *(int*)((char*)self + 0x20) = 0;
    func_00161950(self, idx);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161A10);
#ifdef SKIP_ASM
extern void* D_0045B908[];
void operator_delete(int* ptr);
extern "C" void func_0015CC10(void* self, int flags);

extern "C" void func_00161A10(void* self, int flags)
{
    *(void***)((char*)self + 0x14) = D_0045B908;
    operator_delete(*(int**)((char*)self + 0x20));
    func_0015CC10(self, flags);
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

INCLUDE_ASM("camera/camera", func_00161BB8);

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

INCLUDE_ASM("camera/camera", func_00162318);

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

INCLUDE_ASM("camera/camera", func_00162568);

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

INCLUDE_ASM("camera/camera", func_00162B90);

INCLUDE_ASM("camera/camera", func_00162C78);

INCLUDE_ASM("camera/camera", func_00163010);

INCLUDE_ASM("camera/camera", func_00163158);

INCLUDE_ASM("camera/camera", func_00163270);

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

INCLUDE_ASM("camera/camera", func_001635F8);

INCLUDE_ASM("camera/camera", func_001641C0);

INCLUDE_ASM("camera/camera", func_001643A8);

INCLUDE_ASM("camera/camera", func_001646A0);

INCLUDE_ASM("camera/camera", func_00164878);

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

INCLUDE_ASM("camera/camera", func_001656B0);

INCLUDE_ASM("camera/camera", func_00165938);

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

INCLUDE_ASM("camera/camera", func_001662A0);

extern "C" void* func_00168150(void* self);

//100%
INCLUDE_ASM("camera/camera", func_00166530__FPv);
#ifdef SKIP_ASM
void* func_00166530(void* self)
{
    return func_00168150(self);
}
#endif

INCLUDE_ASM("camera/camera", func_00166550);

INCLUDE_ASM("camera/camera", func_00166640);

INCLUDE_ASM("camera/camera", func_001668B8);

INCLUDE_ASM("camera/camera", func_00166C60);

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

INCLUDE_ASM("camera/camera", func_00166F90);

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

INCLUDE_ASM("camera/camera", func_001673F8);

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
INCLUDE_ASM("camera/camera", func_00167FA0);
#ifdef SKIP_ASM
extern "C" void* func_00167FA0(void* self, void* a1)
{
    *(cQuad128*)self = *(cQuad128*)((char*)a1 + 0x20);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167FB0__FPv);
#ifdef SKIP_ASM
int func_00167FB0(void* self)
{
    return *(int*)((char*)self + 0x2F0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167FB8__FPv);
#ifdef SKIP_ASM
void func_00167FB8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168030);
#ifdef SKIP_ASM
extern void* D_0045B8B0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00168030(void* self, int flags)
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

INCLUDE_ASM("camera/camera", func_00168150);

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

INCLUDE_ASM("camera/camera", func_00168338);

INCLUDE_ASM("camera/camera", func_00168508);

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

INCLUDE_ASM("camera/camera", func_001686D0);

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

INCLUDE_ASM("camera/camera", func_00168A70);

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

INCLUDE_ASM("camera/camera", func_00168D80);

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


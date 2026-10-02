#include "common.h"

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_gainFocus);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);
struct cRiderSphereTree;
cRiderSphereTree* cRiderSphereTree_cRiderSphereTree(cRiderSphereTree* self);
extern char D_00458210[];

struct sVec4WG {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0DotWG(const sVec4WG& a, const sVec4WG& b)
{
    float r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf5, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %0, $vf4\n"
        : "=r"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4WG vu0ScaleMemWG(const sVec4WG& v, float s)
{
    sVec4WG r;
    int t;
    __asm__ __volatile__(
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
static inline void vu0SubWG(sVec4WG& dst, const sVec4WG& a, const sVec4WG& b)
{
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(a), "m"(b));
}

extern "C" void cWipeoutMotion_gainFocus(char* self, int a1)
{
    if (*(void**)(self + 0x44) == 0)
    {
        *(void**)(self + 0x44) = cRiderSphereTree_cRiderSphereTree((cRiderSphereTree*)cMemMan_alloc(0x2B0, D_00458210, 0, 0));
    }
    char* r = *(char**)(self + 0x40);
    sVec4WG* pos = (sVec4WG*)(r + 0x110);
    *(sVec4WG*)(self + 0x50) = *pos;
    *(sVec4WG*)(self + 0x60) = *pos;
    if (a1 == 0)
    {
        sVec4WG* n = (sVec4WG*)(r + 0x370);
        float d = vu0DotWG(*(sVec4WG*)(r + 0x1E0), *n);
        sVec4WG t = vu0ScaleMemWG(*n, d);
        sVec4WG* p = (sVec4WG*)(*(char**)(self + 0x40) + 0x1E0);
        vu0SubWG(*p, *p, t);
        *(int*)self = 0;
    }
    else
    {
        *(int*)self = 1;
        cAirPredictor_startLaunchIntoAir(*(void**)(r + 0x788), pos, r + 0x1E0, 3333.33349609375f);
    }
    *(int*)(self + 0x4) = 0;
    *(int*)(*(char**)(self + 0x40) + 0x2DC) = 0;
    *(int*)(*(char**)(self + 0x40) + 0x2E0) = 0;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136D40);
#ifdef SKIP_ASM
struct sVec4WM {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVecPairWM {
    sVec4WM a;
    sVec4WM b;
};

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0DotWM(const sVec4WM& a, const sVec4WM& b)
{
    float r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf5, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %0, $vf4\n"
        : "=r"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4WM vu0ScaleWM(const sVec4WM& v, float s)
{
    sVec4WM r;
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline void vu0SubWM(sVec4WM& dst, const sVec4WM& a, const sVec4WM& b)
{
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(a), "m"(b));
}

extern "C" void func_00136D40(char* self, sVec4WM* a, sVec4WM* b, sVecPairWM* c)
{
    *(sVecPairWM*)(*(char**)(self + 0x40) + 0x130) = *c;
    *(int*)(*(char**)(self + 0x40) + 0x150) = 1;
    *(sVec4WM*)(self + 0x20) = *b;
    *(sVec4WM*)(self + 0x10) = *a;
    if (*(int*)self == 0)
    {
        sVec4WM* n = (sVec4WM*)(*(char**)(self + 0x40) + 0x370);
        float d = vu0DotWM(*n, *a);
        sVec4WM t = vu0ScaleWM(*n, d);
        vu0SubWM(*(sVec4WM*)(self + 0x10), *(sVec4WM*)(self + 0x10), t);
    }
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136DE0);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4WM vu0ScaleMemWM_136DE0(const sVec4WM& v, float s)
{
    sVec4WM r;
    int t;
    __asm__ __volatile__(
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

extern "C" void func_00136DE0(char* self, sVec4WM* a, sVec4WM* b)
{
    *(sVec4WM*)(self + 0x30) = *b;
    *(sVec4WM*)(*(char**)(self + 0x40) + 0x1E0) = *a;
    if (*(int*)self == 0)
    {
        sVec4WM* n = (sVec4WM*)(*(char**)(self + 0x40) + 0x370);
        float d = vu0DotWM(*n, *a);
        sVec4WM t = vu0ScaleMemWM_136DE0(*n, d);
        sVec4WM* p = (sVec4WM*)(*(char**)(self + 0x40) + 0x1E0);
        vu0SubWM(*p, *p, t);
    }
    else
    {
        char* r = *(char**)(self + 0x40);
        cAirPredictor_startLaunchIntoAir(*(void**)(r + 0x788), r + 0x110, r + 0x1E0, 3333.33349609375f);
    }
    *(int*)(self + 0x4) = 0;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136E98);
#ifdef SKIP_ASM
extern "C" void func_00136F30();
extern "C" void func_00137D18(void*);
extern "C" void func_00137750(void*);

extern "C" void func_00136E98(int* self)
{
    func_00136F30();
    if (*self == 0)
    {
        func_00137D18(self);
    }
    else
    {
        func_00137750(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136EE0);
#ifdef SKIP_ASM
extern "C" void func_00137138();
extern "C" void func_00138640(void*);
extern "C" void func_00137860(void*);

extern "C" void func_00136EE0(int* self)
{
    func_00137138();
    if (*self == 0)
    {
        func_00138640(self);
    }
    else
    {
        func_00137860(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136F28__FPv);
#ifdef SKIP_ASM
void func_00136F28(void* self)
{
}
#endif

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136F30);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137138);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137550);

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137750);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void func_00113648(void* ap, void* pos, void* vel, float dt);
extern "C" void func_00121AA0(void* rider, void* a, void* b, float x, float y);

struct sVec4W7 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times-assign scalar).
static inline void vu0ScaleEqW7(sVec4W7& v, float s)
{
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(s));
}

extern "C" void func_00137750(void* p)
{
    char* self = (char*)p;
    char* r = *(char**)(self + 0x40);
    float dt = *(float*)(r + 0x300) * 0.01666666753590107f;
    func_00113648(*(void**)(r + 0x788), r + 0x110, r + 0x1E0, dt);
    int st = *(int*)(*(char**)(*(char**)(self + 0x40) + 0x788) + 0xAC);
    int ok = 0;
    if (st == 1 || st == 3) ok = 1;
    if (ok)
    {
        char* r2 = *(char**)(self + 0x40);
        char* ap = *(char**)(r2 + 0x788);
        float d = *(float*)(ap + 0x98) - *(float*)(ap + 0xA0);
        float x = *(float*)(r2 + 0x300);
        if (d >= 0.01666666753590107f)
            x /= d;
        else
            x *= 59.999996185302734f;
        char* r3 = *(char**)(self + 0x40);
        float k = 6.632251739501953f;
        func_00121AA0(r3, *(char**)(r3 + 0x788) + 0x20, *(char**)(r3 + 0x788) + 0x10, x, *(float*)(r3 + 0x300) * k);
    }
    vu0ScaleEqW7(*(sVec4W7*)(self + 0x30), 1.0f - dt * 1.5f);
    cRider_updateOrientationImplicit(*(void**)(self + 0x40));
}
#endif

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137860);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137D18);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00138640);

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00138960);
#ifdef SKIP_ASM
extern "C" void* func_0032E100(void* seg, const sVec4WG& a, const sVec4WG& b, int n, float r);
extern "C" float func_003342D0(void* world, void* seg, void* hit, int flags);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4WG vu0Scale_138960(const sVec4WG& v, float s)
{
    sVec4WG r;
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
static inline sVec4WG vu0Sub_138960(const sVec4WG& a, const sVec4WG& b)
{
    sVec4WG r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4WG vu0Add_138960(const sVec4WG& a, const sVec4WG& b)
{
    sVec4WG r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

struct sVEntry_138960 { short delta; short index; void (*fn)(void*, void*); };

extern "C" float func_00138960(void* self, char* hit, sVec4WG* dir)
{
    char* rider = *(char**)((char*)self + 0x40);
    char seg[0xB0];
    sVec4WG* pt = (sVec4WG*)(rider + 0x110);
    func_0032E100(seg, vu0Sub_138960(*pt, vu0Scale_138960(*dir, 200.0f)), vu0Add_138960(*pt, vu0Scale_138960(*dir, 200.0f)), 2, 0.574999988079071f);
    char* r2 = *(char**)((char*)self + 0x40);
    float d = func_003342D0(*(void**)(r2 + 0x860), seg, hit, *(int*)(r2 + 0x864));
    if (d >= 0.0f)
    {
        char* obj = *(char**)(hit + 0x54);
        if (obj)
        {
            *(int*)(*(char**)((char*)self + 0x40) + 0x430) = *(int*)(obj + 0x150);
            *(float*)(*(char**)((char*)self + 0x40) + 0xAAC) = *(float*)(hit + 0x6C);
            *(float*)(*(char**)((char*)self + 0x40) + 0xAB0) = *(float*)(hit + 0x70);
            *(unsigned short*)(*(char**)((char*)self + 0x40) + 0x2D4) = *(unsigned short*)(*(char**)(hit + 0x54) + 0xA);
        }
        else
        {
            *(unsigned int*)(*(char**)((char*)self + 0x40) + 0x430) = 0xFFFFFFFF;
        }
        char* o = *(char**)(hit + 0x50);
        if (o)
        {
            char* h = *(char**)(o + 0xC);
            if (h)
            {
                sVEntry_138960* vt = *(sVEntry_138960**)(h + 0xC);
                vt[42].fn(h + vt[42].delta, hit);
            }
        }
    }
    return d;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00138AD8);
#ifdef SKIP_ASM
struct sVEntry00138AD8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00138AD8(void* self, void* obj)
{
    sVEntry00138AD8* vt = *(sVEntry00138AD8**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x40);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00138B10);
#ifdef SKIP_ASM
struct sVEntry00138B10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00138B10(void* self, void* obj)
{
    sVEntry00138B10* vt = *(sVEntry00138B10**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x40);
}
#endif


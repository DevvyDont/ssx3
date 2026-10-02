#include "common.h"

INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_gainFocus);

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

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136DE0);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136E98);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136EE0);

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

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137750);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137860);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137D18);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00138640);

INCLUDE_ASM("ai/motion/wipeoutmotion", func_00138960);

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


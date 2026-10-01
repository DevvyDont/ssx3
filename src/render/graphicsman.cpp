#include "common.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

INCLUDE_ASM("render/graphicsman", cGraphicsMan_AddBlendedMatrix);

INCLUDE_ASM("render/graphicsman", func_00369A78);

INCLUDE_ASM("render/graphicsman", func_00369C28);

INCLUDE_ASM("render/graphicsman", func_00369CB0);

INCLUDE_ASM("render/graphicsman", func_00369CF8);

//100%
INCLUDE_ASM("render/graphicsman", func_00369FF0__FPv);
#ifdef SKIP_ASM
void func_00369FF0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", func_00369FF8);
#ifdef SKIP_ASM
extern "C" void func_00369FF8(void* self)
{
    char* vt = *(char**)((char*)self + 0x10D8);
    ((void (*)(void*))*(void**)(vt + 0x3CC))((char*)self + *(short*)(vt + 0x3C8));
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", func_0036A020);
#ifdef SKIP_ASM
extern "C" void func_0036A020(void* self)
{
    char* vt = *(char**)((char*)self + 0x10D8);
    ((void (*)(void*))*(void**)(vt + 0x3C4))((char*)self + *(short*)(vt + 0x3C0));
}
#endif

INCLUDE_ASM("render/graphicsman", func_0036A048);

extern "C" void* func_0036A1C0(int);

//100%
INCLUDE_ASM("render/graphicsman", func_0036A190__FPvi);
#ifdef SKIP_ASM
void* func_0036A190(void* self, int a1)
{
    return func_0036A1C0(a1);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", func_0036A1B0__FPvT0);
#ifdef SKIP_ASM
int func_0036A1B0(void* self, void* a1)
{
    int t0 = -1;
    *(int*)a1 = 0;
    *(int*)((char*)a1 + 0x4) = t0;
    return t0;
}
#endif

INCLUDE_ASM("render/graphicsman", func_0036A1C0);

INCLUDE_ASM("render/graphicsman", func_0036A1E8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/graphicsman", func_0036A290);
#ifdef SKIP_ASM
extern "C" void func_0036A1E8();

extern "C" void func_0036A290()
{
    int i;
    for (i = 0; i < 4; i++)
        func_0036A1E8();
}
#endif

INCLUDE_ASM("render/graphicsman", func_0036A2C0);

INCLUDE_ASM("render/graphicsman", func_0036A428);

INCLUDE_ASM("render/graphicsman", func_0036AA60);

INCLUDE_ASM("render/graphicsman", func_0036ABA0);

INCLUDE_ASM("render/graphicsman", func_0036AC00);

INCLUDE_ASM("render/graphicsman", func_0036AE20);

INCLUDE_ASM("render/graphicsman", func_0036B158);

INCLUDE_ASM("render/graphicsman", func_0036B9D8);

INCLUDE_ASM("render/graphicsman", func_0036C188);

INCLUDE_ASM("render/graphicsman", func_0036C398);

INCLUDE_ASM("render/graphicsman", func_0036C740);

INCLUDE_ASM("render/graphicsman", func_0036C790);

//100%
INCLUDE_ASM("render/graphicsman", func_0036CBF8);
#ifdef SKIP_ASM
struct sGfxVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern cQuad128 D_004FF120;

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline sGfxVec4 vu0DivGM(const sGfxVec4& v, float s)
{
    sGfxVec4 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" void func_0036CBF8(char* self, int w, int h, float x, float y, float s, sGfxVec4* v)
{
    *(int*)(self + 0x0) = w;
    *(int*)(self + 0x4) = h;
    *(float*)(self + 0xC) = x * s / (float)w;
    *(sGfxVec4*)(self + 0x60) = vu0DivGM(vu0DivGM(*v, s), s);
    *(float*)(self + 0x4C) = s;
    *(float*)(self + 0x30) = 1.0f / (float)*(int*)(self + 0x4);
    *(float*)(self + 0x34) = y * s;
    sGfxVec4 one;
    one.x = 1.0f;
    one.y = 1.0f;
    one.z = 0.0f;
    one.w = 0.0f;
    *(cQuad128*)(self + 0x130) = D_004FF120;
    *(sGfxVec4*)(self + 0x140) = one;
}
#endif

INCLUDE_ASM("render/graphicsman", func_0036CCB8);

//100%
INCLUDE_ASM("render/graphicsman", func_0036CE00);
#ifdef SKIP_ASM
extern "C" void func_0036CE00(void* self, float a, float b, float c)
{
    *(float*)((char*)self + 0x10) = c;
    *(float*)((char*)self + 0x20) = b - a;
    *(float*)((char*)self + 0x18) = a - c * 1.5f;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", func_0036CE28);
#ifdef SKIP_ASM
extern "C" void func_0036CE28(void* self, float a, float b)
{
    float d = b - a;
    float s = *(float*)((char*)self + 0x4c);
    a = a * s;
    d = d * s;
    *(float*)((char*)self + 0x14) = d;
    *(float*)((char*)self + 0x1c) = a - d;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", func_0036CEF8);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector add).
static inline sGfxVec4 vu0AddGM(const sGfxVec4& a, const sGfxVec4& b)
{
    sGfxVec4 r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sGfxVec4 vu0SubGM(const sGfxVec4& a, const sGfxVec4& b)
{
    sGfxVec4 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sGfxVec4 vu0ScaleGM(const sGfxVec4& v, float s)
{
    sGfxVec4 r;
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

extern "C" void func_0036CEF8(char* self, sGfxVec4* a, sGfxVec4* b, sGfxVec4* c)
{
    *(sGfxVec4*)(self + 0xB0) = vu0SubGM(*a, vu0ScaleGM(vu0AddGM(*b, *c), 3.0f));
    *(sGfxVec4*)(self + 0xC0) = vu0ScaleGM(*b, 2.0f);
    *(sGfxVec4*)(self + 0xD0) = vu0ScaleGM(*c, 2.0f);
}
#endif

extern cQuad128 D_004FF120;

//100%
INCLUDE_ASM("render/graphicsman", func_0036D008);
#ifdef SKIP_ASM
extern "C" void func_0036D008(void* self)
{
    *(cQuad128*)((char*)self + 0xe0) = D_004FF120;
}
#endif

INCLUDE_ASM("render/graphicsman", func_0036D1F0);

//100%
INCLUDE_ASM("render/graphicsman", func_0036D318);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector add).
// PORT: PS2-only VU0 inline asm (vector subtract).
// PORT: PS2-only VU0 inline asm (vector times scalar).
// PORT: PS2-only VU0 inline asm (vector divided by scalar).
extern "C" void func_0036D318(char* self, sGfxVec4* a, sGfxVec4* b, sGfxVec4* c, sGfxVec4* d, float t)
{
    *(sGfxVec4*)(self + 0xF0) = vu0SubGM(*a, vu0ScaleGM(vu0AddGM(*c, *d), 1.5f));
    *(sGfxVec4*)(self + 0x120) = vu0DivGM(vu0SubGM(*b, *a), t * *(float*)(self + 0x4C));
    *(sGfxVec4*)(self + 0x100) = *c;
    *(sGfxVec4*)(self + 0x110) = *d;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", func_0036D3D8);
#ifdef SKIP_ASM
extern "C" void func_0036D3D8(void* self, float val)
{
    *(float*)((char*)self + 0x8) = val * *(float*)((char*)self + 0x4c);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", func_0036D3E8);
#ifdef SKIP_ASM
extern "C" void func_0036D3E8(void* self, float val)
{
    *(float*)((char*)self + 0x8) += val * *(float*)((char*)self + 0x4c);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", func_0036D400);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 random-number unit (R register); the PC port needs a C PRNG.
extern "C" void func_0036D400(float* seed)
{
    float v = *seed;
    __asm__ __volatile__(
        "qmtc2.ni  %0, $vf3\n"
        "vrinit    R, $vf3x\n"
        "vrnext.x  $vf3, R\n"
        "qmfc2.ni  %0, $vf3\n"
        : "=r"(v)
        : "0"(v));
    *seed = v;
}
#endif

INCLUDE_ASM("render/graphicsman", func_0036D428);

INCLUDE_ASM("render/graphicsman", func_0036D500);

INCLUDE_ASM("render/graphicsman", func_00370018);

INCLUDE_ASM("render/graphicsman", func_00370058);

INCLUDE_ASM("render/graphicsman", func_003705E0);

//100%
INCLUDE_ASM("render/graphicsman", func_00370758);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00493160[];

extern "C" void func_00370758(int* self, int flags)
{
    *(void**)((char*)self + 0x18C) = D_00493160;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("render/graphicsman", func_00370788);

INCLUDE_ASM("render/graphicsman", func_00370888);

INCLUDE_ASM("render/graphicsman", func_003708C0);

INCLUDE_ASM("render/graphicsman", func_00370AA8);

INCLUDE_ASM("render/graphicsman", func_00370AF8);

INCLUDE_ASM("render/graphicsman", func_00370B60);

INCLUDE_ASM("render/graphicsman", func_00370C08);


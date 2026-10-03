#include "common.h"

INCLUDE_ASM("camera/cameraalgorithms", cChaseCameraController_createChaseAlgorithmBlend);

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D698);
#ifdef SKIP_ASM
struct func_0015D698_sVec4 { float x, y, z, w; } __attribute__((aligned(16)));

extern void* D_0045B8B0[];
extern func_0015D698_sVec4 D_004FF140;

extern "C" void* func_0015D698(void* self)
{
    func_0015D698_sVec4 v;
    *(void***)((char*)self + 0x90) = D_0045B8B0;
    v.x = 0; v.y = 0; v.z = 0; v.w = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x20) = v;
    v.w = 1.0f;
    v.x = 0;
    v.y = 1.0f;
    v.z = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x30) = v;
    *(func_0015D698_sVec4*)((char*)self + 0x80) = D_004FF140;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D700);
#ifdef SKIP_ASM
extern "C" void* func_0015D928(void* self);

extern "C" void func_0015D700(void* self, float x, float y, float z)
{
    func_0015D698_sVec4 v;
    *(float*)((char*)self + 0xC) = x;
    *(float*)((char*)self + 0x10) = y;
    *(float*)((char*)self + 0x14) = z;
    *(float*)((char*)self + 0x0) = x;
    *(float*)((char*)self + 0x4) = y;
    *(float*)((char*)self + 0x8) = z;
    v.x = 0; v.y = 0; v.z = 0; v.w = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x20) = v;
    v.w = 1.0f;
    v.x = 0;
    v.y = 1.0f;
    v.z = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x30) = v;
    *(func_0015D698_sVec4*)((char*)self + 0x80) = D_004FF140;
    func_0015D928(self);
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D928);
#ifdef SKIP_ASM
struct sV_D928 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_D928 {
    float m[4][4];
} __attribute__((aligned(16)));

extern char D_004C53A0[];

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_D928(void* dst, void* src)
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
static inline void vu0MulMtx_D928(void* dst, void* a, void* b)
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

// PORT: PS2-only VU0 inline asm (unit quaternion -> rotation matrix, row 3 = identity).
static inline void vu0QuatToMtx_D928(void* m, const sV_D928& q)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2       $vf4, %1\n"
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
        : "r"(m), "m"(q)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_D928(void* m, const sV_D928& v)
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

extern "C" void func_0015D928_impl(void* self) __asm__("func_0015D928");
extern "C" void func_0015D928_impl(void* self)
{
    char* view = (char*)self + 0x40;
    vu0CopyMtx_D928(view, D_004C53A0);
    sM_D928 rot;
    vu0QuatToMtx_D928(&rot, *(sV_D928*)((char*)self + 0x30));
    vu0MulMtx_D928(view, view, &rot);
    sV_D928 t;
    sV_D928 neg;
    sV_D928 pos;
    pos.x = *(float*)((char*)self + 0x20);
    pos.y = *(float*)((char*)self + 0x24);
    pos.z = *(float*)((char*)self + 0x28);
    neg.x = -pos.x;
    neg.y = -pos.y;
    neg.z = -pos.z;
    t.x = neg.x;
    t.y = neg.y;
    t.z = neg.z;
    t.w = 1.0f;
    vu0TransMtx_D928(view, t);
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015DAC0);
#ifdef SKIP_ASM
extern void* D_004A28A8;

extern "C" void func_0015DAC0(void)
{
    void* cam = *(void**)((char*)D_004A28A8 + 0x84);
    void* chase = *(void**)((char*)cam + 0x84);
    void* ctrl = *(void**)((char*)chase + 0x4);
    *(int*)((char*)ctrl + 0x4A4) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015DB58);
#ifdef SKIP_ASM
extern "C" void func_0016C968(void* self);
void cCameraTriggerMan_setInGameTriggers(void* self);
extern "C" void func_00162170(void* cam);
extern char D_004C5830[];

struct sCamVE_0015DB58 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0015DB58(void)
{
    func_0016C968(D_004C5830);
    cCameraTriggerMan_setInGameTriggers(D_004C5830);
    char* riders = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84);
    int n = *(int*)riders;
    func_00162170(*(void**)(*(char**)(riders + 4) + 0xA8));
    char* cam = *(char**)(*(char**)(riders + 4) + 0xA8);
    sCamVE_0015DB58* e = &(*(sCamVE_0015DB58**)(cam + 0x14))[4];
    e->fn(cam + e->delta);
    *(int*)(*(char**)(riders + 4) + 0x460) = 0;
    *(int*)(*(char**)(riders + 4) + 0x4A4) = 1;
    if (n >= 2)
    {
        func_00162170(*(void**)(*(char**)(riders + 8) + 0xA8));
        char* cam2 = *(char**)(*(char**)(riders + 8) + 0xA8);
        sCamVE_0015DB58* e2 = &(*(sCamVE_0015DB58**)(cam2 + 0x14))[4];
        e2->fn(cam2 + e2->delta);
        *(int*)(*(char**)(riders + 8) + 0x460) = 0;
    }
}
#endif


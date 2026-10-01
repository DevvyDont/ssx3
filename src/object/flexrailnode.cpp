#include "common.h"

INCLUDE_ASM("object/flexrailnode", cFlexRailNode_addSpaceHash);

INCLUDE_ASM("object/flexrailnode", func_00348C48);

INCLUDE_ASM("object/flexrailnode", func_00348D98);

INCLUDE_ASM("object/flexrailnode", func_00348FA0);

INCLUDE_ASM("object/flexrailnode", func_003490B0);

INCLUDE_ASM("object/flexrailnode", func_00349110);

INCLUDE_ASM("object/flexrailnode", func_00349220);

INCLUDE_ASM("object/flexrailnode", func_003492A8);

//100%
INCLUDE_ASM("object/flexrailnode", func_00349798);
#ifdef SKIP_ASM
struct sFrVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFrElem {
    sFrVec4 pos;   // 0x00
    sFrVec4 vel;   // 0x10
    char pad[0x30];
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sFrVec4 vu0ScaleFR(const sFrVec4& v, float s)
{
    sFrVec4 r;
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddFR(sFrVec4& dst, sFrVec4 b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

extern "C" void func_00349798(void* self, void* node, sFrVec4* v)
{
    if (*(int*)((char*)node + 0x64) >= 0) {
        vu0AddFR((*(sFrElem**)((char*)self + 0x70))[*(int*)((char*)node + 0x64)].pos,
                 vu0ScaleFR(*v, 1.0f - *(float*)((char*)node + 0x68)));
        vu0AddFR((*(sFrElem**)((char*)self + 0x70))[*(int*)((char*)node + 0x64) + 1].pos,
                 vu0ScaleFR(*v, *(float*)((char*)node + 0x68)));
    }
}
#endif

INCLUDE_ASM("object/flexrailnode", func_00349840);

INCLUDE_ASM("object/flexrailnode", func_003498E8);

INCLUDE_ASM("object/flexrailnode", func_00349AD0);

INCLUDE_ASM("object/flexrailnode", func_00349B48);

INCLUDE_ASM("object/flexrailnode", func_00349DB0);

INCLUDE_ASM("object/flexrailnode", func_00349EB8);

INCLUDE_ASM("object/flexrailnode", func_00349F18);

INCLUDE_ASM("object/flexrailnode", func_0034A028);

INCLUDE_ASM("object/flexrailnode", func_0034A0B0);

INCLUDE_ASM("object/flexrailnode", func_0034A568);

INCLUDE_ASM("object/flexrailnode", func_0034A838);

INCLUDE_ASM("object/flexrailnode", func_0034A8C8);

INCLUDE_ASM("object/flexrailnode", func_0034AC10);

INCLUDE_ASM("object/flexrailnode", func_0034AC88);

INCLUDE_ASM("object/flexrailnode", func_0034ADD8);

INCLUDE_ASM("object/flexrailnode", func_0034AE68);

INCLUDE_ASM("object/flexrailnode", func_0034AEA0);

extern "C" void* func_0034FE90(void* self);

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AEF8__FPv);
#ifdef SKIP_ASM
void* func_0034AEF8(void* self)
{
    return func_0034FE90(self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AF18__FPv);
#ifdef SKIP_ASM
void func_0034AF18(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AF20);
#ifdef SKIP_ASM
extern "C" void* func_0034AF20(void* self)
{
    *(int*)((char*)self + 0x5c) = 0;
    *(unsigned int*)((char*)self + 0x58) = 0xFFFFFFFFU;
    return self;
}
#endif

INCLUDE_ASM("object/flexrailnode", func_0034AF38);

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AFB8);
#ifdef SKIP_ASM
extern void* D_0048FBF8[];
void operator_delete(int*);

extern "C" void func_0034AFB8(void* self, int flags)
{
    *(void***)((char*)self + 0x184) = D_0048FBF8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("object/flexrailnode", func_0034AFE8);

INCLUDE_ASM("object/flexrailnode", func_0034B038);

INCLUDE_ASM("object/flexrailnode", func_0034B168);


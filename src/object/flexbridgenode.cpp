#include "common.h"

INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_setupGrid);

//100%
INCLUDE_ASM("object/flexbridgenode", func_00346D38);
#ifdef SKIP_ASM
struct sFlexVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
static inline sFlexVec4 flexVecSub(const sFlexVec4& a, const sFlexVec4& b)
{
    sFlexVec4 r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
static inline sFlexVec4 flexVecAdd(const sFlexVec4& a, const sFlexVec4& b)
{
    sFlexVec4 r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v * s).
static inline sFlexVec4 flexVecScale(const sFlexVec4& v, float s)
{
    sFlexVec4 r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

struct sFlexSeg {
    sFlexVec4 pos;       // 0x00
    char pad_0x10[0x40];
};

struct sFlexBridge {
    char pad_0x00[0x20];
    sFlexVec4 end;       // 0x20
    sFlexVec4 start;     // 0x30
    char pad_0x40[0x10];
    int count;           // 0x50
    char pad_0x54[0x20];
    sFlexSeg* segs;      // 0x74
    char pad_0x78[0x4];
    sFlexVec4* pts;      // 0x7C: 2 * count points
};

extern "C" void func_00346D38(sFlexBridge* self)
{
    int n = self->count;
    sFlexSeg* seg = self->segs;
    for (int i = 0; i < self->count; i++, seg++) {
        seg->pos = flexVecAdd(self->pts[i], flexVecScale(flexVecSub(self->pts[n + i], self->pts[i]), 0.5f));
    }
    seg = self->segs;
    self->start = flexVecSub(self->pts[0], seg->pos);
    self->end = flexVecSub(self->pts[n], seg->pos);
}
#endif

INCLUDE_ASM("object/flexbridgenode", func_00346E38);

INCLUDE_ASM("object/flexbridgenode", func_003470D0);

INCLUDE_ASM("object/flexbridgenode", func_00347268);

//100%
INCLUDE_ASM("object/flexbridgenode", func_003475A8);
#ifdef SKIP_ASM
extern "C" void func_00353FC0(void*);
extern "C" void func_003475D8(void*);

extern "C" void func_003475A8(void* self)
{
    func_00353FC0((char*)self + 0x50);
    func_003475D8(self);
}
#endif

INCLUDE_ASM("object/flexbridgenode", func_003475D8);

INCLUDE_ASM("object/flexbridgenode", func_00347B80);

INCLUDE_ASM("object/flexbridgenode", func_00347D38);

INCLUDE_ASM("object/flexbridgenode", func_00347D90);

INCLUDE_ASM("object/flexbridgenode", func_00347EA8);

INCLUDE_ASM("object/flexbridgenode", func_00347F90);

INCLUDE_ASM("object/flexbridgenode", func_00348008);

INCLUDE_ASM("object/flexbridgenode", func_00348058);

INCLUDE_ASM("object/flexbridgenode", func_003480C8);

INCLUDE_ASM("object/flexbridgenode", func_00348290);

//100%
INCLUDE_ASM("object/flexbridgenode", func_00348B40);
#ifdef SKIP_ASM
struct sSerVEntry_00348B40 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);

extern "C" void func_00348B40(void* self, void* stream)
{
    func_0034FE90(self, stream);
    sSerVEntry_00348B40* vt = *(sSerVEntry_00348B40**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x20, 0x30);
}
#endif


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

//100%
INCLUDE_ASM("object/flexbridgenode", func_00347D38);
#ifdef SKIP_ASM
struct sFlexBridgeVEntry7D38 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);
extern "C" void func_003545D8(void* p, void* stream);

extern "C" void func_00347D38(void* self, void* stream)
{
    func_0034FE90(self, stream);
    func_003545D8((char*)self + 0x50, stream);
    sFlexBridgeVEntry7D38* vt = *(sFlexBridgeVEntry7D38**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x20, 0x30);
}
#endif

INCLUDE_ASM("object/flexbridgenode", func_00347D90);

INCLUDE_ASM("object/flexbridgenode", func_00347EA8);

//100%
INCLUDE_ASM("object/flexbridgenode", func_00347F90);
#ifdef SKIP_ASM
struct sFlexBridgeVEntry7F90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

void cMemMan_free(void*);
extern "C" void func_0034FBF0(void* self, int flags);
extern char D_004900D0[];

extern "C" void func_00347F90(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = D_004900D0;
    void* buf = *(void**)((char*)self + 0x50);
    if (buf != 0) {
        cMemMan_free(buf);
    }
    void* obj = *(void**)((char*)self + 0x58);
    if (obj != 0) {
        sFlexBridgeVEntry7F90* vt = *(sFlexBridgeVEntry7F90**)obj;
        vt[1].fn((char*)obj + vt[1].delta, 3);
    }
    func_0034FBF0(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", func_00348008);
#ifdef SKIP_ASM
struct sFlexBridgeVEntry8008 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00348058(void* self);

extern "C" void func_00348008(void* self)
{
    sFlexBridgeVEntry8008* vt1 = *(sFlexBridgeVEntry8008**)((char*)self + 0xC);
    vt1[47].fn((char*)self + vt1[47].delta);
    sFlexBridgeVEntry8008* vt2 = *(sFlexBridgeVEntry8008**)((char*)self + 0xC);
    vt2[48].fn((char*)self + vt2[48].delta);
    func_00348058(self);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", func_00348058);
#ifdef SKIP_ASM
struct sBox00348058 {
    float min[4];
    float max[4];
} __attribute__((aligned(16)));

struct sFlexBridgeVEntry8058 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sObj00348058 {
    char pad_0x00[0xC];
    sFlexBridgeVEntry8058* vt;
    char pad_0x10[0x20];
    sBox00348058 box;
    char pad_0x50[0x4];
    int id;
};

extern "C" void* func_002D1BE0();
extern "C" void func_003291E0(void* world, int type, int id, sBox00348058* box, sBox00348058* old);

extern "C" void func_00348058(void* p)
{
    sObj00348058* self = (sObj00348058*)p;
    sBox00348058* box = &self->box;
    sBox00348058 old = self->box;
    self->vt[50].fn((char*)self + self->vt[50].delta);
    func_003291E0(func_002D1BE0(), 2, self->id, box, &old);
}
#endif

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


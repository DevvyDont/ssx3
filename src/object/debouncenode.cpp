#include "common.h"

INCLUDE_ASM("object/debouncenode", cDebounceNode_cDebounceNode);

INCLUDE_ASM("object/debouncenode", func_00342D88);

INCLUDE_ASM("object/debouncenode", func_00342DD8);

extern "C" void* func_00356B08(void* self);

//100%
INCLUDE_ASM("object/debouncenode", func_00342E78__FPv);
#ifdef SKIP_ASM
void* func_00342E78(void* self)
{
    return func_00356B08(self);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00342E98);

INCLUDE_ASM("object/debouncenode", func_00342FA8);

INCLUDE_ASM("object/debouncenode", func_00343010);

INCLUDE_ASM("object/debouncenode", func_003430D0);

INCLUDE_ASM("object/debouncenode", func_00343130);

//100%
INCLUDE_ASM("object/debouncenode", func_003434E8);
#ifdef SKIP_ASM
struct sDebounceScroll {
    char pad_0x0[0x38];
    float du; // 0x38
    float dv; // 0x3c
    float u;  // 0x40
    float v;  // 0x44
};

extern "C" void func_003434E8(sDebounceScroll* self)
{
    self->u += self->du;
    self->v += self->dv;
    if (self->u > 1.0f) {
        self->u -= 1.0f;
    } else if (self->u < -1.0f) {
        self->u += 1.0f;
    }
    if (self->v > 1.0f) {
        self->v -= 1.0f;
    } else if (self->v < -1.0f) {
        self->v += 1.0f;
    }
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343588);

//100%
INCLUDE_ASM("object/debouncenode", func_00343718);
#ifdef SKIP_ASM
struct sDebounceVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);

extern "C" void func_00343718(void* self, void* stream)
{
    func_0034FE90(self, stream);
    sDebounceVEntry* vt = *(sDebounceVEntry**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x1C, 0x2C);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343768);

INCLUDE_ASM("object/debouncenode", func_003437C0);

//100%
INCLUDE_ASM("object/debouncenode", func_00343820);
#ifdef SKIP_ASM
struct sDebounceVEntry1 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00343820(void* self)
{
    if (*(int*)((char*)self + 0x1C) == 0) {
        *(int*)((char*)self + 0x1C) = -1;
        sDebounceVEntry1* vt = *(sDebounceVEntry1**)((char*)self + 0xC);
        vt[34].fn((char*)self + vt[34].delta, 1);
    }
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343868);

//100%
INCLUDE_ASM("object/debouncenode", func_00343A18);
#ifdef SKIP_ASM
struct sDebounceVEntryA18 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);

extern "C" void func_00343A18(void* self, void* stream)
{
    func_0034FE90(self, stream);
    sDebounceVEntryA18* vt = *(sDebounceVEntryA18**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x1C, 0x8);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343A68);

INCLUDE_ASM("object/debouncenode", func_00343B28);

//100%
INCLUDE_ASM("object/debouncenode", func_00343BC0);
#ifdef SKIP_ASM
struct sDebounceState;
extern "C" void func_003442E0(sDebounceState* self);
void* func_00344348(void* self);

struct sDebounceBlock {
    char data[0x1F4];
};

extern "C" void func_00343BC0(sDebounceBlock* self)
{
    int i;
    for (i = 0; i < 5; i++) {
        func_00344348(&self[i]);
        func_003442E0((sDebounceState*)&self[i]);
    }
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343C08);

INCLUDE_ASM("object/debouncenode", func_00343C60);

INCLUDE_ASM("object/debouncenode", func_00343F38);

INCLUDE_ASM("object/debouncenode", func_003440C8);

INCLUDE_ASM("object/debouncenode", func_00344138);

INCLUDE_ASM("object/debouncenode", func_003441A8);

INCLUDE_ASM("object/debouncenode", func_00344240);

//100%
INCLUDE_ASM("object/debouncenode", func_003442E0);
#ifdef SKIP_ASM
struct sDebounceSlot {
    unsigned int id; // 0x0
    int value;       // 0x4
};

struct sDebounceState {
    int field_0x0;            // 0x000
    unsigned int field_0x4;   // 0x004
    unsigned int field_0x8;   // 0x008
    sDebounceSlot slots[32];  // 0x00c
    int field_0x10c;          // 0x10c
    int field_0x110;          // 0x110
    int field_0x114;          // 0x114
    char pad_0x118[0xc4];     // 0x118
    int field_0x1dc;          // 0x1dc
    int field_0x1e0;          // 0x1e0
    int field_0x1e4;          // 0x1e4
    int field_0x1e8;          // 0x1e8
    int field_0x1ec;          // 0x1ec
    int field_0x1f0;          // 0x1f0
};

extern "C" void func_003442E0(sDebounceState* self)
{
    int i;
    self->field_0x0 = 0;
    self->field_0x1dc = 0;
    self->field_0x8 = 0xFFFFFFFF;
    self->field_0x4 = 0xFFFFFFFF;
    self->field_0x1e8 = 0;
    self->field_0x1ec = 0;
    self->field_0x1f0 = 0;
    self->field_0x114 = 0;
    self->field_0x110 = 0;
    for (i = 0; i < 32; i++) {
        self->slots[i].id = 0xFFFFFFFF;
        self->slots[i].value = 0;
    }
}
#endif

extern "C" void* func_00344800(void* self);

//100%
INCLUDE_ASM("object/debouncenode", func_00344348__FPv);
#ifdef SKIP_ASM
void* func_00344348(void* self)
{
    return func_00344800(self);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00344368);

INCLUDE_ASM("object/debouncenode", func_003443A8);

INCLUDE_ASM("object/debouncenode", func_00344730);

INCLUDE_ASM("object/debouncenode", func_00344800);

INCLUDE_ASM("object/debouncenode", func_00344898);

INCLUDE_ASM("object/debouncenode", func_003449F0);

INCLUDE_ASM("object/debouncenode", func_00344AA0);

INCLUDE_ASM("object/debouncenode", func_00344E18);

//100%
INCLUDE_ASM("object/debouncenode", func_00344FC0);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
class cDebounceStream {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_00344FC0(void* self, cDebounceStream* s)
{
    s->v01(self, 0x10C);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00344FF8);

//100%
INCLUDE_ASM("object/debouncenode", func_00345048);
#ifdef SKIP_ASM
struct sDebVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sDebMtx {
    sDebVec4 r[4];
};

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (matrix * vector).
static inline sDebVec4 debMtxApply(sDebMtx* m, const sDebVec4& v)
{
    sDebVec4 out;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        ".set pop\n"
        : "=m"(out)
        : "m"(v), "r"(m)
        : "memory");
    return out;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v * s).
static inline sDebVec4 debVecScale(const sDebVec4& v, float s)
{
    sDebVec4 out;
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
        : "=m"(out), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return out;
}

struct sDebCurve {
    char pad_0x00[0x10];
    sDebMtx basis;   // 0x10
    float coef[4];   // 0x50: time-warp cubic
    char pad_0x60[0x24];
    float t0;        // 0x84
};

// Evaluate the curve at time t: position, velocity and acceleration.
extern "C" void func_00345048(sDebCurve* self, float t, sDebVec4* pos, sDebVec4* vel, sDebVec4* acc)
{
    t -= self->t0;
    float u = ((self->coef[0] * t + self->coef[1]) * t + self->coef[2]) * t + self->coef[3];
    float du = (self->coef[0] * (t * 3.0f) + self->coef[1] * 2.0f) * t + self->coef[2];
    sDebVec4 a;
    a.x = u * 6.0f;
    a.y = 2.0f;
    a.z = 0.0f;
    a.w = 0.0f;
    *acc = debVecScale(debMtxApply(&self->basis, a), du);
    sDebVec4 p;
    p.x = u * u * u;
    p.y = u * u;
    p.z = u;
    p.w = 1.0f;
    *pos = debMtxApply(&self->basis, p);
    sDebVec4 v;
    v.x = u * u * 3.0f;
    v.y = u * 2.0f;
    v.z = 1.0f;
    v.w = 0.0f;
    *vel = debMtxApply(&self->basis, v);
}
#endif

INCLUDE_ASM("object/debouncenode", func_003451C0);

INCLUDE_ASM("object/debouncenode", func_00345248);

INCLUDE_ASM("object/debouncenode", func_00345430);


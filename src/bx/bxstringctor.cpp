#include "common.h"

extern void* D_004A3E90;

struct cBXString2 {
    void* field_0x0;
    void* field_0x4;
    void* field_0x8;
    void* field_0xC;
    void* field_0x10;
    void* arr[4]; // 0x14-0x23
};

//85.05% - loop shape (down-counting bne vs ours) not fully reproduced
INCLUDE_ASM("bx/bxstringctor", cBXString_cBXString__FP10cBXString2);
#ifdef SKIP_ASM
cBXString2* cBXString_cBXString(cBXString2* self)
{
    self->field_0x0 = D_004A3E90;
    self->field_0x4 = D_004A3E90;
    self->field_0x8 = D_004A3E90;
    self->field_0xC = D_004A3E90;
    self->field_0x10 = D_004A3E90;
    for (int i = 0; i < 4; i++) {
        self->arr[i] = D_004A3E90;
    }
    return self;
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_00268950);

INCLUDE_ASM("bx/bxstringctor", func_002689B8);

// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target
struct sShortPad16 { short v; int pad[3]; };
extern sShortPad16 D_004A2FB0;
extern "C" void func_0025D6C0(int);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268A90);
#ifdef SKIP_ASM
extern "C" void func_00268A90(void* self, void* a1, int a2)
{
    func_0025D6C0(a2 + D_004A2FB0.v);
}
#endif

extern sShortPad16 D_004A2FB8;
extern "C" void func_002599B0(int);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268AB8);
#ifdef SKIP_ASM
extern "C" void func_00268AB8(void* self, void* a1, int a2)
{
    func_002599B0(a2 + D_004A2FB8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268AE0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A2FC0;
extern "C" void func_00259A60(int);

extern "C" void func_00268AE0(void* self, void* a1, int a2)
{
    func_00259A60(a2 + D_004A2FC0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268B08);
#ifdef SKIP_ASM
extern sShortPad16 D_004A2FF0;
void* func_002598A8(void* self);

extern "C" void func_00268B08(void* self, void* a1, char* a2)
{
    func_002598A8(a2 + D_004A2FF0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268B30);
#ifdef SKIP_ASM
extern sShortPad16 D_004A2FF8;
extern "C" void func_00259B10(int);

extern "C" void func_00268B30(void* self, void* a1, int a2)
{
    func_00259B10(a2 + D_004A2FF8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268B58);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3000;
extern "C" void func_002598C8(int);

extern "C" void func_00268B58(void* self, void* a1, int a2)
{
    func_002598C8(a2 + D_004A3000.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268B80);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3008;
extern "C" void func_0025A1D0(int);

extern "C" void func_00268B80(void* self, void* a1, int a2)
{
    func_0025A1D0(a2 + D_004A3008.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268BA8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3020;
extern "C" void func_0025B848(int);

extern "C" void func_00268BA8(void* self, void* a1, int a2)
{
    func_0025B848(a2 + D_004A3020.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268BD0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3088;
extern "C" void func_0025BD30(int);

extern "C" void func_00268BD0(void* self, void* a1, int a2)
{
    func_0025BD30(a2 + D_004A3088.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268BF8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A30A8;
extern "C" void func_0025C0C0(int);

extern "C" void func_00268BF8(void* self, void* a1, int a2)
{
    func_0025C0C0(a2 + D_004A30A8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268C20);
#ifdef SKIP_ASM
extern sShortPad16 D_004A30B0;
extern "C" void func_0025C2D8(int);

extern "C" void func_00268C20(void* self, void* a1, int a2)
{
    func_0025C2D8(a2 + D_004A30B0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268C48);
#ifdef SKIP_ASM
extern sShortPad16 D_004A30B8;
extern "C" void func_0025C4F0(int);

extern "C" void func_00268C48(void* self, void* a1, int a2)
{
    func_0025C4F0(a2 + D_004A30B8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268C70);
#ifdef SKIP_ASM
extern sShortPad16 D_004A30E8;
extern "C" void func_0025C8A8(int);

extern "C" void func_00268C70(void* self, void* a1, int a2)
{
    func_0025C8A8(a2 + D_004A30E8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268C98);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3118;
extern "C" void func_0025CEB0(int);

extern "C" void func_00268C98(void* self, void* a1, int a2)
{
    func_0025CEB0(a2 + D_004A3118.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268CC0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3120;
extern "C" void func_0025D048(int);

extern "C" void func_00268CC0(void* self, void* a1, int a2)
{
    func_0025D048(a2 + D_004A3120.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268CE8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3138;
extern "C" void func_0025D2D8(int);

extern "C" void func_00268CE8(void* self, void* a1, int a2)
{
    func_0025D2D8(a2 + D_004A3138.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268D10);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3140;
extern "C" void func_0025D538(int);

extern "C" void func_00268D10(void* self, void* a1, int a2)
{
    func_0025D538(a2 + D_004A3140.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268D38);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3148;
extern "C" void func_0025D8F8(int);

extern "C" void func_00268D38(void* self, void* a1, int a2)
{
    func_0025D8F8(a2 + D_004A3148.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268D60);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3158;
extern "C" void func_0025DB80(int);

extern "C" void func_00268D60(void* self, void* a1, int a2)
{
    func_0025DB80(a2 + D_004A3158.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268D88);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3160;
extern "C" void func_0025DDE0(int);

extern "C" void func_00268D88(void* self, void* a1, int a2)
{
    func_0025DDE0(a2 + D_004A3160.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268DB0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3170;
extern "C" void func_0025DF98(int);

extern "C" void func_00268DB0(void* self, void* a1, int a2)
{
    func_0025DF98(a2 + D_004A3170.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268DD8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3178;
extern "C" void func_0025E138(int);

extern "C" void func_00268DD8(void* self, void* a1, int a2)
{
    func_0025E138(a2 + D_004A3178.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268E00);
#ifdef SKIP_ASM
extern sShortPad16 D_004A31B0;
extern "C" void func_0025E590(int);

extern "C" void func_00268E00(void* self, void* a1, int a2)
{
    func_0025E590(a2 + D_004A31B0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268E28);
#ifdef SKIP_ASM
extern sShortPad16 D_004A31B8;
extern "C" void func_0025E7F0(int);

extern "C" void func_00268E28(void* self, void* a1, int a2)
{
    func_0025E7F0(a2 + D_004A31B8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268E78);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3210;
extern "C" void func_0025F030(int);

extern "C" void func_00268E78(void* self, void* a1, int a2)
{
    func_0025F030(a2 + D_004A3210.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268EA0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3248;
extern "C" void func_0025F8A0(int);

extern "C" void func_00268EA0(void* self, void* a1, int a2)
{
    func_0025F8A0(a2 + D_004A3248.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268EC8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3268;
void func_0025FC20(void* self);

extern "C" void func_00268EC8(void* self, void* a1, char* a2)
{
    func_0025FC20(a2 + D_004A3268.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268EF0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3318;
extern "C" void func_00260C68(int);

extern "C" void func_00268EF0(void* self, void* a1, int a2)
{
    func_00260C68(a2 + D_004A3318.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268F18);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3340;
extern "C" void func_002615C8(int);

extern "C" void func_00268F18(void* self, void* a1, int a2)
{
    func_002615C8(a2 + D_004A3340.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268F40);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3348;
extern "C" void func_00261A28(int);

extern "C" void func_00268F40(void* self, void* a1, int a2)
{
    func_00261A28(a2 + D_004A3348.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268F68);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3350;
extern "C" void func_00261770(int);

extern "C" void func_00268F68(void* self, void* a1, int a2)
{
    func_00261770(a2 + D_004A3350.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268F90);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3380;
extern "C" void func_00261970(int);

extern "C" void func_00268F90(void* self, void* a1, int a2)
{
    func_00261970(a2 + D_004A3380.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268FB8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3388;
extern "C" void func_002618C0(int);

extern "C" void func_00268FB8(void* self, void* a1, int a2)
{
    func_002618C0(a2 + D_004A3388.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268FE0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A33A0;
void func_00261A20(void* self);

extern "C" void func_00268FE0(void* self, void* a1, char* a2)
{
    func_00261A20(a2 + D_004A33A0.v);
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_00269008);

INCLUDE_ASM("bx/bxstringctor", func_002690C8);

INCLUDE_ASM("bx/bxstringctor", func_00269188);

INCLUDE_ASM("bx/bxstringctor", func_00269240);

INCLUDE_ASM("bx/bxstringctor", func_00269300);

INCLUDE_ASM("bx/bxstringctor", func_002693C0);

INCLUDE_ASM("bx/bxstringctor", func_00269478);

INCLUDE_ASM("bx/bxstringctor", func_00269538);

INCLUDE_ASM("bx/bxstringctor", func_00269608);

INCLUDE_ASM("bx/bxstringctor", func_002696F8);

INCLUDE_ASM("bx/bxstringctor", func_002697D0);

INCLUDE_ASM("bx/bxstringctor", func_00269890);

INCLUDE_ASM("bx/bxstringctor", func_00269948);

INCLUDE_ASM("bx/bxstringctor", func_00269A18);

INCLUDE_ASM("bx/bxstringctor", func_00269AE0);

INCLUDE_ASM("bx/bxstringctor", func_00269B90);

INCLUDE_ASM("bx/bxstringctor", func_00269C40);

INCLUDE_ASM("bx/bxstringctor", func_00269CF0);

extern "C" void* func_00267E18(void* self);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269DA0__FPv);
#ifdef SKIP_ASM
void* func_00269DA0(void* self)
{
    return func_00267E18(self);
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_00269DC0);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269E20__FPv);
#ifdef SKIP_ASM
void* func_00269E20(void* self)
{
    return func_00267E18(self);
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_00269E40);

extern "C" void* func_00267468(int, int);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269EA0__FPv);
#ifdef SKIP_ASM
void* func_00269EA0(void* self)
{
    return func_00267468(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269EC0__FPv);
#ifdef SKIP_ASM
int func_00269EC0(void* self)
{
    int t0 = 0x2e;
    *(int*)((char*)self + 0x38) = t0;
    *(int*)((char*)self + 0x3c) = 1;
    return t0;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269ED8);
#ifdef SKIP_ASM
extern "C" void func_0026BA68(void* p);

extern "C" void func_00269ED8(void* self, int a1, int id)
{
    if (id == 0x64) {
        func_0026BA68((char*)self + 0x38);
    } else if (id == 0x65) {
        func_0026BA68((char*)self + 0x3C);
    }
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_00269F18);

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026A068__FPv);
#ifdef SKIP_ASM
void func_0026A068(void* self)
{
    *(int*)((char*)self + 0x38) = 0;
}
#endif

extern "C" void func_0026BA88(void*);

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026A070);
#ifdef SKIP_ASM
extern "C" void func_0026A070(void* self, int a1, int a2)
{
    if (a2 == 0) {
        func_0026BA88((char*)self + 0x38);
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026A090);
#ifdef SKIP_ASM
struct sBXRange {
    int field_0x0;
    int field_0x4;
    float min; // 0x8
    float max; // 0xC
};

struct sBXRangeList {
    int count;          // 0x0
    sBXRange* items;    // 0x4
    char pad_0x08[0x30];
    float field_0x38;   // 0x38
};

extern "C" int func_0026AA80(sBXRangeList* self, sBXRange* out, int maxOut, float lo, float hi);

extern "C" int func_0026A090(sBXRangeList* self, sBXRange* out, int maxOut, float a, float b)
{
    float base = self->field_0x38;
    return func_0026AA80(self, out, maxOut, base - a, base - b);
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_0026A0B8);

INCLUDE_ASM("bx/bxstringctor", func_0026A180);

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026A338);
#ifdef SKIP_ASM
extern "C" void func_0026BA48(void* p, void* q);
// PORT: func_0026BA88 takes (dst, src); the unit declares it with one arg
void func_0026BA88_2(void* dst, void* src) __asm__("func_0026BA88");

extern "C" void func_0026A338(void* self, void* src, void* dst)
{
    func_0026BA48(dst, src);
    func_0026BA88_2((char*)dst + 0xC, (char*)src + 0xC);
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_0026A378);

INCLUDE_ASM("bx/bxstringctor", func_0026A428);

INCLUDE_ASM("bx/bxstringctor", func_0026A638);

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026A8B8);
#ifdef SKIP_ASM
struct sBXSeg16 {
    float f0;
    float f4;
    float f8;
    float fC;
};

extern "C" sBXSeg16* func_0026A8B8(sBXSeg16* out, void* self, float t)
{
    int i = 0;
    for (;;) {
        sBXSeg16* e = &(*(sBXSeg16**)((char*)self + 0x18))[i];
        if (i == *(int*)((char*)self + 0x8) - 1 || t <= e->fC) {
            out->f0 = e->f0;
            out->f4 = e->f4;
            out->f8 = e->f8;
            *(int*)&out->fC = 0;
            goto done;
        }
        t -= e->fC;
        i++;
    }
done:
    return out;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026A9B0);
#ifdef SKIP_ASM
struct sBXBox {
    char pad_0x00[0x1C];
    float min[3]; // 0x1C
    float max[3]; // 0x28
};

// Largest per-axis distance from point p to the box.
extern "C" float func_0026A9B0(sBXBox* self, float* p)
{
    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;
    if (p[0] < self->min[0]) {
        dx = self->min[0] - p[0];
    } else if (self->max[0] < p[0]) {
        dx = p[0] - self->max[0];
    }
    if (p[1] < self->min[1]) {
        dy = self->min[1] - p[1];
    } else if (self->max[1] < p[1]) {
        dy = p[1] - self->max[1];
    }
    if (p[2] < self->min[2]) {
        dz = self->min[2] - p[2];
    } else if (self->max[2] < p[2]) {
        dz = p[2] - self->max[2];
    }
    if (dy < dx) {
        if (dx < dz) {
            return dz;
        }
        return dx;
    }
    if (dy < dz) {
        return dz;
    }
    return dy;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026AA80);
#ifdef SKIP_ASM
extern "C" int func_0026AA80(sBXRangeList* self, sBXRange* out, int maxOut, float lo, float hi)
{
    int n;
    int i;
    if (hi < lo) {
        return 0;
    }
    n = 0;
    for (i = 0; i < self->count; i++) {
        if (!(hi < self->items[i].min) && !(self->items[i].max < lo)) {
            *out++ = self->items[i];
            n++;
            if (n == maxOut) {
                goto done;
            }
        }
    }
done:
    return n;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026AB20);
#ifdef SKIP_ASM
struct sBXVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBXPathSeg {
    float x, y, z;   // direction
    float len;       // 0xC
};

struct sBXPath {
    char pad_0x00[8];
    int count;          // 0x8
    float x, y, z;      // 0xC start
    sBXPathSeg* segs;   // 0x18
};

// PORT: PS2-only VU0 inline asm (direction times scalar).
static inline sBXVec4 bxVu0ScaleDir(const sBXPathSeg& seg, float s)
{
    sBXVec4 v;
    v.x = seg.x;
    v.y = seg.y;
    v.z = seg.z;
    v.w = 0.0f;
    sBXVec4 r;
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
static inline void bxVu0AddEq(sBXVec4& dst, sBXVec4 b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sBXVec4 bxVu0Add(const sBXVec4& a, sBXVec4 b)
{
    sBXVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

extern "C" sBXVec4 func_0026AB20(sBXPath* self, int* inRange, float t)
{
    sBXVec4 pos;
    pos.x = self->x;
    pos.y = self->y;
    pos.z = self->z;
    pos.w = 1.0f;
    if (inRange) {
        *inRange = 1;
    }
    for (int i = 0;; ) {
        sBXPathSeg* seg = &self->segs[i];
        if (i == self->count) {
            if (inRange) {
                *inRange = 0;
            }
            return pos;
        }
        float len = seg->len;
        if (t < len) {
            return bxVu0Add(pos, bxVu0ScaleDir(*seg, t));
        }
        t -= len;
        i++;
        bxVu0AddEq(pos, bxVu0ScaleDir(*seg, len));
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026AC48);
#ifdef SKIP_ASM
struct sBXElem16 {
    float f0;
    float f4;
    float f8;
    float fC;
};

extern "C" float func_0026AC48(void* self)
{
    int n = *(int*)((char*)self + 0x8);
    float sum = 0.0f;
    int i;
    for (i = 0; i < n; i++) {
        sum += (*(sBXElem16**)((char*)self + 0x18))[i].fC;
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026AC88);
#ifdef SKIP_ASM
extern "C" int func_0026AC88(sBXRangeList* self, float lo, float hi)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (!(hi < self->items[i].min) && !(self->items[i].max < lo)) {
            if (self->items[i].field_0x0 == 0x14 || self->items[i].field_0x0 == 0x12) {
                return 1;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026AD70);
#ifdef SKIP_ASM
extern "C" void func_0026ADF0(void* self);

extern "C" int func_0026AD70(void* self, int v)
{
    *(int*)((char*)self + 0x48) = v;
    *(int*)((char*)self + 0x4C) = 1;
    func_0026ADF0(self);
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026ADA0__FPv);
#ifdef SKIP_ASM
void func_0026ADA0(void* self)
{
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_0026ADF0);

INCLUDE_ASM("bx/bxstringctor", func_0026AF00);

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026AF70);
#ifdef SKIP_ASM
extern "C" int func_0026AF70(void* self, int a1)
{
    return *(int*)((char*)self + 0x14) + a1 * 0x3c;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026AF88);
#ifdef SKIP_ASM
extern "C" int func_0026AF88(void* self, int a1)
{
    return *(int*)((char*)self + 0xc) + (a1 << 6);
}
#endif

extern "C" void* func_0026AFB8(void*);

//100%
INCLUDE_ASM("bx/bxstringctor", func_0026AF98__FPv);
#ifdef SKIP_ASM
// PORT: the unit declares func_0026AFB8(void*) and this symbol's mangling says (void*),
// but both really take more arguments (callers pass $5-$7, and this passes a 5th arg in $8).
// Bind the real signatures to the existing symbol names with asm labels.
int func_0026AFB8_impl(void* self, void* a1, void** out, int maxOut, int flag) __asm__("func_0026AFB8");
int func_0026AF98_impl(void* self, void* a1, void** out, int maxOut) __asm__("func_0026AF98__FPv");

int func_0026AF98_impl(void* self, void* a1, void** out, int maxOut)
{
    return func_0026AFB8_impl(self, a1, out, maxOut, 1);
}
#endif

INCLUDE_ASM("bx/bxstringctor", func_0026AFB8);

INCLUDE_ASM("bx/bxstringctor", func_0026B178);


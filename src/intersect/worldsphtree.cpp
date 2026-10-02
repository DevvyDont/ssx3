#include "common.h"

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_cWorldSphTree);
#ifdef SKIP_ASM
extern void* D_0048E5F0[];
extern char D_0048E550[];
extern "C" void* func_0032C508(void* p);
extern "C" float func_0032C590(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

struct sSphV4 { float x, y, z, w; } __attribute__((aligned(16)));

extern "C" void* cWorldSphTree_cWorldSphTree(void* self, void* a1, int a2)
{
    *(void***)((char*)self + 0x50) = D_0048E5F0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x4) = 0;
    if (a2 != 0) {
        *(int*)self = 0;
    } else {
        *(int*)self = 1;
    }
    *(void**)((char*)self + 0x64) = a1;
    *(void**)((char*)self + 0x60) = a1;
    sSphV4 c = *(sSphV4*)((char*)a1 + 0x80);
    float r = func_0032C590(a1);
    sSphV4 t;
    *(sSphV4*)((char*)self + 0x40) = c;
    t.x = c.x + r;
    t.y = c.y + r;
    t.z = c.z + r;
    t.w = 1.0f;
    *(sSphV4*)((char*)self + 0x20) = t;
    t.x = c.x - r;
    t.y = c.y - r;
    t.z = c.z - r;
    t.w = 1.0f;
    *(sSphV4*)((char*)self + 0x30) = t;
    void* p = cMemMan_alloc(0x140, D_0048E550, 0, 0);
    func_0032C508(p);
    *(void**)((char*)self + 0x68) = p;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003304E8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0048E5F0[];

extern "C" void func_003304E8(void* self, int flags)
{
    *(void***)((char*)self + 0x50) = D_0048E5F0;
    operator_delete(*(int**)((char*)self + 0x68));
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00330540);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003306D8);
#ifdef SKIP_ASM
extern "C" float func_0032C590(void* self);

extern "C" float func_003306D8(void* self)
{
    float r;
    if (*(int*)self != 0) {
        r = func_0032C590(*(void**)((char*)self + 0x60)) * 2.0f;
    } else {
        r = -1.0f;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330710);
#ifdef SKIP_ASM
extern "C" void* func_00327CC8(void* self, void* src);
extern "C" void func_0032C770(void* p, void* a, float f);

extern "C" void func_00330710(void* self, void* a1, float f)
{
    func_00327CC8(*(void**)((char*)self + 0x68), *(void**)((char*)self + 0x60));
    *(void**)((char*)self + 0x60) = *(void**)((char*)self + 0x68);
    func_0032C770(*(void**)((char*)self + 0x60), a1, f);
    *(int*)((char*)self + 0x4) = 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330778__FPv);
#ifdef SKIP_ASM
int func_00330778(void* self)
{
    int t0 = *(int*)((char*)self + 0x64);
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x60) = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330788);
#ifdef SKIP_ASM
struct sWsVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330788(const sWsVec4& v)
{
    float r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %0, $vi22\n"
        : "=r"(r)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline sWsVec4 wsDiv_330788(const sWsVec4& v, float s)
{
    sWsVec4 r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" int func_0032CA78(void* tree, int a1, int a2, int a3, int a4, sWsVec4* out, int a6);

extern "C" int func_00330788(void* self, int a1, int a2, int a3, int a4, int a5, sWsVec4* outDir, float* outLen)
{
    sWsVec4 v;
    int r = func_0032CA78(*(void**)((char*)self + 0x60), a1, a2, a3, a4, &v, a5);
    if (r != 0) {
        float len = wsLength_330788(v);
        *outLen = len;
        *outDir = wsDiv_330788(v, len);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330828);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330828(const sWsVec4& v)
{
    float r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %0, $vi22\n"
        : "=r"(r)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline sWsVec4 wsDiv_330828(const sWsVec4& v, float s)
{
    sWsVec4 r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" int func_00328030(void* tree, int a1, int a2, int a3, int a4, sWsVec4* out, int a6);

extern "C" int func_00330828(void* self, int a1, int a2, int a3, int a4, int a5, sWsVec4* outDir, float* outLen)
{
    sWsVec4 v;
    int r = func_00328030(*(void**)((char*)self + 0x60), a1, a2, a3, a4, &v, a5);
    if (r != 0) {
        float len = wsLength_330828(v);
        *outLen = len;
        *outDir = wsDiv_330828(v, len);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003308C8);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_3308C8(const sWsVec4& v)
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
static inline void wsDivEq_3308C8(sWsVec4& v, float s)
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

struct sWsHit_3308C8 {
    char pad_0x00[0x10];
    sWsVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int func_0032CB58(void* self, void* other, void* a2, void* a3);

extern "C" int func_003308C8(void* self, void* a1, sWsHit_3308C8* hit)
{
    if (func_0032CB58(*(void**)((char*)self + 0x60), a1, &hit->dir, hit) != 0) {
        float len = wsLength_3308C8(hit->dir);
        hit->len = len;
        wsDivEq_3308C8(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330950);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330950(const sWsVec4& v)
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
static inline void wsDivEq_330950(sWsVec4& v, float s)
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

struct sWsHit_330950 {
    char pad_0x00[0x10];
    sWsVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int func_0032C928(void* self, void* a1, void* a2, void* a3, void* a4);

extern "C" int func_00330950(void* self, void* a1, void* a2, sWsHit_330950* hit)
{
    if (func_0032C928(*(void**)((char*)self + 0x60), a1, a2, &hit->dir, hit) != 0) {
        float len = wsLength_330950(hit->dir);
        hit->len = len;
        wsDivEq_330950(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_003309D8);

INCLUDE_ASM("intersect/worldsphtree", func_00331450);

INCLUDE_ASM("intersect/worldsphtree", func_00332DB8);

INCLUDE_ASM("intersect/worldsphtree", func_00333EF8);

INCLUDE_ASM("intersect/worldsphtree", func_003342D0);

INCLUDE_ASM("intersect/worldsphtree", func_00334458);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00334680);
#ifdef SKIP_ASM
struct sBox_00334680 {
    sWsVec4 min;
    sWsVec4 max;
};
extern "C" void func_0035C698(void* a0, sBox_00334680* box, sBox_00334680* box2, sWsVec4* pos, int mask, int* cnt, float* dist, int a7);
extern "C" void func_00348290(void* a0, sBox_00334680* box, sBox_00334680* box2, sWsVec4* pos, int mask, int* cnt, float* dist, int a7);
extern "C" void func_00335128(void* obj, sBox_00334680* box, sWsVec4* pos, int* cnt, float* dist, int a5);

// PORT: PS2-only VU0 inline asm (in-place vector subtract).
static inline void wsSubEq_334680(sWsVec4& a, const sWsVec4& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void wsAddEq_334680(sWsVec4& a, const sWsVec4& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

extern "C" int func_00334680(char* self, sWsVec4* pos, int a2, int mask, float r)
{
    sBox_00334680 box;
    sWsVec4 ext;
    int cnt;
    float dist;
    float zero = 0.0f;
    cnt = 0;
    dist = zero;
    box.min = *pos;
    box.max = *pos;
    ext.x = r;
    ext.y = r;
    ext.z = r;
    ext.w = zero;
    wsSubEq_334680(box.min, ext);
    wsAddEq_334680(box.max, ext);
    for (unsigned int i = 0; i < *(unsigned int*)(self + 0x210); i++) {
        char* o = ((char**)(self + 0x214))[i];
        int type = *(int*)(o + 8);
        if (type == 2) {
            func_0035C698(*(void**)(o + 0xC), &box, &box, pos, mask, &cnt, &dist, a2);
        } else if (type == 3) {
            func_00348290(*(void**)(o + 0xC), &box, &box, pos, mask, &cnt, &dist, a2);
        } else if (type == 1) {
            if (*(int*)(*(char**)(o + 0x68) + 0x1C) & mask) {
                func_00335128(o, &box, pos, &cnt, &dist, a2);
            }
        }
    }
    return cnt;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00334800);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* func_00327810(void* self);
extern "C" void cAIFwdDiffCache_Init(void* cache, int n);
extern char D_0048E4F8[];

extern "C" void func_00334800(void* self)
{
    void* c = func_00327810(cMemMan_alloc(0x10, D_0048E4F8, 0, 0));
    *(void**)((char*)self + 0xA4) = c;
    cAIFwdDiffCache_Init(c, 0x28);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00334850);
#ifdef SKIP_ASM
extern "C" void func_00327828(void* p, int flags);

extern "C" void func_00334850(void* self)
{
    void* p = *(void**)((char*)self + 0xA4);
    if (p != 0) {
        func_00327828(p, 3);
    }
    *(void**)((char*)self + 0xA4) = 0;
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00334888);

INCLUDE_ASM("intersect/worldsphtree", func_00335128);

INCLUDE_ASM("intersect/worldsphtree", func_00335960);

INCLUDE_ASM("intersect/worldsphtree", func_00335B90);

INCLUDE_ASM("intersect/worldsphtree", func_00335D78);

INCLUDE_ASM("intersect/worldsphtree", func_00336850);

INCLUDE_ASM("intersect/worldsphtree", func_003369D8);

INCLUDE_ASM("intersect/worldsphtree", func_00336D40);

INCLUDE_ASM("intersect/worldsphtree", func_00336F30);

INCLUDE_ASM("intersect/worldsphtree", func_003378C0);

INCLUDE_ASM("intersect/worldsphtree", func_00339598);

INCLUDE_ASM("intersect/worldsphtree", func_0033B748);

INCLUDE_ASM("intersect/worldsphtree", func_0033CCF8);

INCLUDE_ASM("intersect/worldsphtree", func_0033DBE8);

INCLUDE_ASM("intersect/worldsphtree", func_0033DFD8);

INCLUDE_ASM("intersect/worldsphtree", func_003400D8);

INCLUDE_ASM("intersect/worldsphtree", func_00340970);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340A08__FPv);
#ifdef SKIP_ASM
void func_00340A08(void* self)
{
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340A10__FPv);
#ifdef SKIP_ASM
float func_00340A10(void* self)
{
    return *(float*)((char*)self + 0xA0);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340A18);
#ifdef SKIP_ASM
extern "C" float func_00340A18(void* self)
{
    if (*(int*)self == 0) {
        return -1.0f;
    }
    return *(float*)((char*)self + 0x70) * 2.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340B18);
#ifdef SKIP_ASM
struct sOctCollect {
    void** arr;
    int* count;
};

extern "C" void func_00340B18(void* self, sOctCollect* out)
{
    void** p;
    for (p = *(void***)((char*)self + 0x28); p != 0; p = (void**)*p) {
        out->arr[(*out->count)++] = p;
    }
    if (*(void**)((char*)self + 0x0)) func_00340B18(*(void**)((char*)self + 0x0), out);
    if (*(void**)((char*)self + 0x4)) func_00340B18(*(void**)((char*)self + 0x4), out);
    if (*(void**)((char*)self + 0x8)) func_00340B18(*(void**)((char*)self + 0x8), out);
    if (*(void**)((char*)self + 0xC)) func_00340B18(*(void**)((char*)self + 0xC), out);
    if (*(void**)((char*)self + 0x10)) func_00340B18(*(void**)((char*)self + 0x10), out);
    if (*(void**)((char*)self + 0x14)) func_00340B18(*(void**)((char*)self + 0x14), out);
    if (*(void**)((char*)self + 0x18)) func_00340B18(*(void**)((char*)self + 0x18), out);
    if (*(void**)((char*)self + 0x1C)) func_00340B18(*(void**)((char*)self + 0x1C), out);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00340DC0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", func_00340FA0);
#ifdef SKIP_ASM
extern "C" void func_00335960(void* ctx, void* obj);
extern "C" void func_00335B90(void* ctx, void* obj);

extern "C" void func_00340FA0(void* self, void* ctx)
{
    void** p;
    for (p = *(void***)((char*)self + 0x24); p != 0; p = (void**)*p) {
        func_00335960(ctx, p);
    }
    for (p = *(void***)((char*)self + 0x20); p != 0; p = (void**)*p) {
        func_00335B90(ctx, p);
    }
    if (*(void**)((char*)self + 0x0)) func_00340FA0(*(void**)((char*)self + 0x0), ctx);
    if (*(void**)((char*)self + 0x4)) func_00340FA0(*(void**)((char*)self + 0x4), ctx);
    if (*(void**)((char*)self + 0x8)) func_00340FA0(*(void**)((char*)self + 0x8), ctx);
    if (*(void**)((char*)self + 0xC)) func_00340FA0(*(void**)((char*)self + 0xC), ctx);
    if (*(void**)((char*)self + 0x10)) func_00340FA0(*(void**)((char*)self + 0x10), ctx);
    if (*(void**)((char*)self + 0x14)) func_00340FA0(*(void**)((char*)self + 0x14), ctx);
    if (*(void**)((char*)self + 0x18)) func_00340FA0(*(void**)((char*)self + 0x18), ctx);
    if (*(void**)((char*)self + 0x1C)) func_00340FA0(*(void**)((char*)self + 0x1C), ctx);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", func_003410C0);
#ifdef SKIP_ASM
extern "C" void func_00336D40(void* ctx, void* obj);

extern "C" void func_003410C0(void* self, void* ctx)
{
    void** p;
    for (p = *(void***)((char*)self + 0x20); p != 0; p = (void**)*p) {
        func_00336D40(ctx, p);
    }
    if (*(void**)((char*)self + 0x0)) func_003410C0(*(void**)((char*)self + 0x0), ctx);
    if (*(void**)((char*)self + 0x4)) func_003410C0(*(void**)((char*)self + 0x4), ctx);
    if (*(void**)((char*)self + 0x8)) func_003410C0(*(void**)((char*)self + 0x8), ctx);
    if (*(void**)((char*)self + 0xC)) func_003410C0(*(void**)((char*)self + 0xC), ctx);
    if (*(void**)((char*)self + 0x10)) func_003410C0(*(void**)((char*)self + 0x10), ctx);
    if (*(void**)((char*)self + 0x14)) func_003410C0(*(void**)((char*)self + 0x14), ctx);
    if (*(void**)((char*)self + 0x18)) func_003410C0(*(void**)((char*)self + 0x18), ctx);
    if (*(void**)((char*)self + 0x1C)) func_003410C0(*(void**)((char*)self + 0x1C), ctx);
}
#endif

extern "C" void* func_003400D8(int, int);

//99.38%
INCLUDE_ASM("intersect/worldsphtree", func_00341368__FPv);
#ifdef SKIP_ASM
void* func_00341368(void* self)
{
    return func_003400D8(1, 0xffff);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00341388);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341548);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern void* D_004914E0[];
struct sVEntry_func_00341548 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_00341548(void* self, void* a1, void* stream)
{
    cInstanceNode_cInstanceNode(self, a1, stream);
    *(void***)((char*)self + 0xC) = D_004914E0;
    sVEntry_func_00341548* vt = *(sVEntry_func_00341548**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x20, 0x30);
    return self;
}
#endif

extern void* D_004914E0[];
extern "C" void* func_0034FBF0(void*);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003415A8__FPv);
#ifdef SKIP_ASM
void* func_003415A8(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_004914E0;
    return func_0034FBF0(self);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_003415D0);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341770);
#ifdef SKIP_ASM
struct sVec4_341770 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_341770 vu0Scale_341770(const sVec4_341770& v, float s)
{
    sVec4_341770 r;
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

class cMover_341770 {
public:
    virtual void v01();
    virtual sVec4_341770* getVelocity();
    virtual void v03();
    virtual void setVelocity(sVec4_341770* v);
};

extern "C" float func_002D1C70();

extern "C" void func_00341770(void* self, cMover_341770* obj)
{
    sVec4_341770 u = vu0Scale_341770(vu0Scale_341770(*obj->getVelocity(), *(float*)((char*)self + 0x34) + 1.0f), func_002D1C70());
    obj->setVelocity(&u);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00341818);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341A30);
#ifdef SKIP_ASM
extern "C" void func_00341A30(void* self, int a1)
{
    if (a1 == 1) {
        *(int*)((char*)self + 0x3c) = *(int*)((char*)self + 0x38);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341A50);
#ifdef SKIP_ASM
struct sSerVEntry_00341A50 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);

extern "C" void func_00341A50(void* self, void* stream)
{
    func_0034FE90(self, stream);
    sSerVEntry_00341A50* vt = *(sSerVEntry_00341A50**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x20, 0x30);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00341AA0);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341C80);
#ifdef SKIP_ASM
struct sVEntry00341C80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_0034DAC8(void* p, int a1, void* stream);
extern void* D_00490B10[];

extern "C" void* func_00341C80(void* self, int a1, sVEntry00341C80** stream)
{
    func_0034DAC8((char*)self + 0x1C, a1, stream);
    *(void***)((char*)self + 0x3C) = D_00490B10;
    (*stream)[2].fn((char*)stream + (*stream)[2].delta, self, 0x1C);
    *(unsigned short*)((char*)self + 0x42) |= 1;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341CF0);
#ifdef SKIP_ASM
extern "C" void func_0034DBA8(void* p, int flags);
void operator_delete(int* ptr);
extern void* D_00490B10[];

extern "C" void func_00341CF0(void* self, int flags)
{
    *(void***)((char*)self + 0x3C) = D_00490B10;
    func_0034DBA8((char*)self + 0x1C, 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341D48);
#ifdef SKIP_ASM
struct sWrapFloat_1E48;
struct sBounceFloat_1EC0;
struct sClampFloat_1F38;
extern "C" void func_00341E48(sWrapFloat_1E48* s);
extern "C" void func_00341EC0(sBounceFloat_1EC0* s);
extern "C" void func_00341F38(sClampFloat_1F38* s);
extern "C" int func_0034EBA0(void* p);

struct sVEntry_00341D48 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" int func_00341D48(void* self)
{
    *(float*)((char*)self + 0x24) = 10000000000.0f;
    *(float*)((char*)self + 0x28) = 10000000000.0f;
    if (*(int*)((char*)self + 0x8) != 0) {
        return 1;
    }
    if (*(int*)((char*)self + 0x4) != 0 && *(int*)((char*)self + 0xC) >= 0) {
        if (*(int*)((char*)self + 0xC) > 0) {
            *(int*)((char*)self + 0xC) -= 1;
        } else {
            *(float*)((char*)self + 0x24) = *(float*)((char*)self + 0x1C);
            switch (*(short*)self) {
            case 2:
                func_00341EC0((sBounceFloat_1EC0*)self);
                break;
            case 1:
                func_00341E48((sWrapFloat_1E48*)self);
                break;
            default:
                func_00341F38((sClampFloat_1F38*)self);
                break;
            }
            if (func_0034EBA0((char*)self + 0x1C) == 0) {
                return 0;
            }
        }
    }
    char* obj = (char*)self + 0x30;
    *(unsigned short*)((char*)self + 0x42) |= 1;
    *(float*)((char*)self + 0x20) = *(float*)((char*)self + 0x1C);
    if (*(int*)((char*)self + 0x8) == 0) {
        return 1;
    }
    sVEntry_00341D48* vt = *(sVEntry_00341D48**)((char*)self + 0x3C);
    vt[0x22].fn(obj + vt[0x22].delta, 1);
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341E48);
#ifdef SKIP_ASM
struct sWrapFloat_1E48 {
    char pad_0x00[0x10];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341E48(sWrapFloat_1E48* s)
{
    if (s->speed >= 0.0f) {
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->value = s->min + (s->value - s->max);
        }
    } else {
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->value = s->max - (s->min - s->value);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341EC0);
#ifdef SKIP_ASM
struct sBounceFloat_1EC0 {
    char pad_0x00[0x10];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341EC0(sBounceFloat_1EC0* s)
{
    if (s->speed >= 0.0f) {
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->speed = -s->speed;
            s->value = s->max - (s->value - s->max);
        }
    } else {
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->speed = -s->speed;
            s->value = s->min + (s->min - s->value);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341F38);
#ifdef SKIP_ASM
struct sClampFloat_1F38 {
    char pad_0x00[0x8];
    int atLimit;    // 0x08
    char pad_0x0C[0x4];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341F38(sClampFloat_1F38* s)
{
    if (s->speed >= 0.0f) {
        if (s->value == s->max) {
            s->atLimit = 1;
        }
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->value = s->max;
        }
    } else {
        if (s->value == s->min) {
            s->atLimit = 1;
        }
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->value = s->min;
        }
    }
}
#endif

void func_0034FCC0(void*);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341FC8);
#ifdef SKIP_ASM
extern "C" void func_00341FC8(void* self, int a1)
{
    if (a1 != 0) {
        func_0034FCC0((char*)self + 0x30);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341FE8);
#ifdef SKIP_ASM
extern char* D_004A5B64;
extern "C" void func_00356B08(void* p);

extern "C" void func_00341FE8(sClampFloat_1F38* self, int msg, float v)
{
    if (msg == 0x64) {
        *(int*)((char*)self + 0xC) = (int)(v * (float)*(int*)(D_004A5B64 + 0x10));
    } else if (msg == 0x65) {
        v = v * 0.03333333507180214f;
        if (self->min <= v && v <= self->max) {
            self->value = v;
        }
    } else if (msg == 0x66) {
        v = v * 0.03333333507180214f;
        self->speed = v / (float)*(int*)(D_004A5B64 + 0x10);
    } else if (msg == 0x67) {
        v = v * 0.03333333507180214f;
        float lim = *(float*)(*(char**)(*(char**)((char*)self + 0x48) + 0x80) + 0x14);
        if (lim < v) {
            v = lim;
        }
        if (v < 0.0f) {
            v = 0.0f;
        }
        self->max = v;
        if (v < self->value) {
            self->value = v;
        }
    } else if (msg == 0x68) {
        v = v * 0.03333333507180214f;
        float lim = *(float*)(*(char**)(*(char**)((char*)self + 0x48) + 0x80) + 0x14);
        if (lim < v) {
            v = lim;
        }
        if (v < 0.0f) {
            v = 0.0f;
        }
        self->min = v;
        if (self->value < v) {
            self->value = v;
        }
    } else if (msg == 0x69) {
        *(short*)self = 0;
    } else {
        func_00356B08((char*)self + 0x30);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342150);
#ifdef SKIP_ASM
extern "C" void func_0034E3D8(void* p, void* stream);
struct sVEntry_func_00342150 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00342150(void* self, void* stream)
{
    func_0034E3D8((char*)self + 0x1C, stream);
    sVEntry_func_00342150* vt = *(sVEntry_func_00342150**)stream;
    vt[1].fn((char*)stream + vt[1].delta, self, 0x1C);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003421A0);
#ifdef SKIP_ASM
extern char* D_004A5B64;
extern void* D_004908F8[];
extern "C" void func_0034D9B0(void* p, int a1, int a2, void* obj, int a4);
extern "C" float func_00351508(void* inst);

struct sSphCtl_003421A0 {
    int f00;            // 0x00
    int f04;            // 0x04
    float f08;          // 0x08
    float f0C;          // 0x0C
    float f10;          // 0x10
    float f14;          // 0x14
    float f18;          // 0x18
    float f1C;          // 0x1C
    float f20;          // 0x20
    float f24;          // 0x24
    int f28;            // 0x28
    int f2C;            // 0x2C
    char pad30[0x20];
    void** vt;          // 0x50
    char pad54[2];
    unsigned short flags; // 0x56
    char pad58[0x20];
    void* inst;         // 0x78
};

struct sFloat_003421A0 {
    float v;
};

static inline void setRate_003421A0(sFloat_003421A0* dst, float v, float dflt)
{
    float r;
    if (v >= 0.0f) {
        r = v * 0.03333333507180214f;
    } else {
        r = dflt;
    }
    dst->v = r;
}

static inline void setRateMax_003421A0(sFloat_003421A0* dst, float v, char* info)
{
    float r;
    if (v < 0.0f) {
        r = *(float*)(info + 0x14);
    } else {
        r = v * 0.03333333507180214f;
    }
    dst->v = r;
}

extern "C" void* func_003421A0(sSphCtl_003421A0* self, int a1, char* obj, char* def)
{
    func_0034D9B0((char*)self + 0x30, a1, 4, obj, 0);
    self->vt = D_004908F8;
    float t = 0.0f;
    char* info = *(char**)(obj + 0x80);
    if (self->inst != 0) {
        t = func_00351508(self->inst);
    }
    setRate_003421A0((sFloat_003421A0*)&self->f10, *(float*)(def + 0x20), t);
    setRateMax_003421A0((sFloat_003421A0*)&self->f14, *(float*)(def + 0x24), info);
    self->f0C = *(float*)(def + 0x8);
    self->f08 = *(float*)(def + 0x4) * 9.999999974752427e-07f;
    self->f18 = *(float*)(def + 0xC) * 0.03333333507180214f / (float)*(int*)(D_004A5B64 + 0x10);
    self->f2C = *(int*)(def + 0x1C);
    self->f24 = *(float*)(def + 0x14) * 0.03333333507180214f;
    self->f1C = *(float*)(def + 0x18);
    self->f20 = *(float*)(def + 0x10);
    self->flags |= 1;
    self->f00 = 0;
    self->f04 = 0;
    self->f28 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003422E8);
#ifdef SKIP_ASM
struct sVEntry003422E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_0034DAC8(void* p, int a1, void* stream);
extern void* D_004908F8[];

extern "C" void* func_003422E8(void* self, int a1, sVEntry003422E8** stream)
{
    func_0034DAC8((char*)self + 0x30, a1, stream);
    *(void***)((char*)self + 0x50) = D_004908F8;
    (*stream)[2].fn((char*)stream + (*stream)[2].delta, self, 0x30);
    *(unsigned short*)((char*)self + 0x56) |= 1;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342358);
#ifdef SKIP_ASM
extern "C" float func_002D1C70();
extern "C" int func_0034EBA0(void* p);

struct sSphCtl_00342358 {
    float force;        // 0x00
    float step;         // 0x04
    float gain;         // 0x08
    float bounce;       // 0x0C
    float min;          // 0x10
    float max;          // 0x14
    float maxStep;      // 0x18
    float damping;      // 0x1C
    float stiffness;    // 0x20
    float rest;         // 0x24
    float vel;          // 0x28
    int f2C;            // 0x2C
    float pos;          // 0x30
    float pos34;        // 0x34
    float prev;         // 0x38
    float pos3C;        // 0x3C
    char pad40[0x16];
    unsigned short flags; // 0x56
};

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_00342358(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" int func_00342358(sSphCtl_00342358* s)
{
    s->prev = s->pos;
    float dt = func_002D1C70();
    float force = s->gain * s->force;
    if (s->gain < 0.0f) {
        force = -force;
    }
    float vel = s->vel;
    float maxStep = s->maxStep;
    float step = vel * dt;
    float acc = -s->stiffness * (s->pos - s->rest);
    acc += force;
    acc += -s->damping * vel;
    s->step = step;
    if (maxStep < absf_00342358(step)) {
        if (step < 0.0f) {
            s->step = -maxStep;
        } else {
            s->step = maxStep;
        }
    }
    float pos = s->pos + s->step;
    float nv = s->vel + acc * dt;
    s->pos = pos;
    s->pos3C = pos;
    s->vel = nv;
    if (pos < s->min) {
        s->pos = s->min;
        if (absf_00342358(nv) > 0.1f) {
            s->vel = nv * s->bounce;
        } else {
            s->step = 0.0f;
            s->vel = 0.0f;
        }
    } else if (s->max < pos) {
        s->pos = s->max;
        if (absf_00342358(nv) > 0.1f) {
            s->vel = nv * s->bounce;
        } else {
            s->step = 0.0f;
            s->vel = 0.0f;
        }
    }
    s->force = 0.0f;
    s->pos34 = s->pos;
    if (func_0034EBA0(&s->pos) == 0) {
        return 0;
    }
    s->flags |= 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003424D0);
#ifdef SKIP_ASM
extern "C" float func_003424D0(void* self)
{
    if (*(float*)((char*)self + 0x30) <= *(float*)((char*)self + 0x10) && *(float*)((char*)self + 0x4) < 0.0f) {
        return 0.0f;
    }
    if (*(float*)((char*)self + 0x30) >= *(float*)((char*)self + 0x14) && *(float*)((char*)self + 0x4) > 0.0f) {
        return 0.0f;
    }
    return *(float*)((char*)self + 0x4);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00342538);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342718);
#ifdef SKIP_ASM
extern "C" void func_0034E3D8(void* p, void* stream);
struct sVEntry_func_00342718 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00342718(void* self, void* stream)
{
    func_0034E3D8((char*)self + 0x30, stream);
    sVEntry_func_00342718* vt = *(sVEntry_func_00342718**)stream;
    vt[1].fn((char*)stream + vt[1].delta, self, 0x30);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342768);
#ifdef SKIP_ASM
struct sVec4_342768 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sSphDef_342768 {
    int f00;
    short type;
    short pad06;
    float time;
    float f0C;
    float f10;
    float x, y, z;
};
struct sSph_342768 {
    int refs;
    void** vt;
    int pad08[2];
    sVec4_342768 pos;
    float f20;
    float f24;
    int t28;
    int t2C;
    int type;
    int pad34[3];
    int a40;
};
extern void* D_00490898[];
extern char* D_004A5B64;

extern "C" void* func_00342768(sSph_342768* self, sSphDef_342768* def, int a2)
{
    self->refs = 1;
    self->vt = D_00490898;
    self->a40 = a2;
    int type = def->type;
    self->type = type;
    int t = (int)(def->time * (float)*(int*)(D_004A5B64 + 0x10));
    self->t28 = t;
    sVec4_342768 v;
    v.x = def->x;
    v.y = def->y;
    v.z = def->z;
    v.w = 0.0f;
    self->pos = v;
    self->f20 = def->f10 * 100.0f;
    self->f24 = def->f0C;
    self->t2C = type == 1 ? t : 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342808);
#ifdef SKIP_ASM
struct sVEntry00342808 {
    short delta;
    short index;
    void* fn;
};

typedef void (*ReadFn00342808)(void*, void*, int);
typedef int (*ReadIntFn00342808)(void*);

extern void* D_00490898[];

extern "C" void* func_00342808(void* self, sVEntry00342808** stream)
{
    *(int*)((char*)self + 0x0) = 1;
    *(void***)((char*)self + 0x4) = D_00490898;
    ((ReadFn00342808)(*stream)[2].fn)((char*)stream + (*stream)[2].delta, (char*)self + 0x10, 0x30);
    *(int*)((char*)self + 0x40) = ((ReadIntFn00342808)(*stream)[3].fn)((char*)stream + (*stream)[3].delta);
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342880);
#ifdef SKIP_ASM
extern "C" int func_002D1AF0();
extern "C" void** func_002D1B08(int i);

struct sVEnt_00342880 { short delta; short index; void* fn; };
typedef void (*Fn0_00342880)(void*);
typedef void (*Fn1_00342880)(void*, void*);
typedef int (*Test_00342880)(void*, int);

extern "C" void func_00342880(sSph_342768* self)
{
    self->refs = 0;
    if (self->type == 0) {
        self->refs = 1;
    }
    int t = self->t2C;
    if (t > 0) {
        t--;
        self->t2C = t;
        if (t > 0) {
            self->refs = 1;
        }
    }
    sVEnt_00342880* vt = (sVEnt_00342880*)self->vt;
    ((Fn0_00342880)vt[9].fn)((char*)self + vt[9].delta);
    if (self->type == 1 || self->t2C <= 0) {
        for (int i = 0; i < func_002D1AF0(); i++) {
            void** obj = func_002D1B08(i);
            sVEnt_00342880* ovt = (sVEnt_00342880*)*obj;
            if (((Test_00342880)ovt[3].fn)((char*)obj + ovt[3].delta, self->a40) != 0) {
                self->refs = 1;
                if (self->f24 < 0.0f) {
                    sVEnt_00342880* v = (sVEnt_00342880*)self->vt;
                    ((Fn1_00342880)v[8].fn)((char*)self + v[8].delta, obj);
                } else {
                    sVEnt_00342880* v = (sVEnt_00342880*)self->vt;
                    ((Fn1_00342880)v[7].fn)((char*)self + v[7].delta, obj);
                }
            }
        }
    }
    sVEnt_00342880* v2 = (sVEnt_00342880*)self->vt;
    ((Fn0_00342880)v2[10].fn)((char*)self + v2[10].delta);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003429E0);
#ifdef SKIP_ASM
struct sVec4_3429E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_3429E0 vu0Scale_3429E0(const sVec4_3429E0& v, float s)
{
    sVec4_3429E0 r;
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

class cMover_3429E0 {
public:
    virtual void v01();
    virtual sVec4_3429E0* getVelocity();
    virtual void v03();
    virtual void setVelocity(sVec4_3429E0* v);
};

extern "C" float func_002D1C70();

extern "C" void func_003429E0(void* self, cMover_3429E0* obj)
{
    sVec4_3429E0 u = vu0Scale_3429E0(vu0Scale_3429E0(*obj->getVelocity(), *(float*)((char*)self + 0x24) + 1.0f), func_002D1C70());
    obj->setVelocity(&u);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00342A88);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342B80);
#ifdef SKIP_ASM
extern "C" void func_00342B80(void* self, int a1)
{
    if (a1 == 0x258) {
        *(int*)((char*)self + 0x2c) = *(int*)((char*)self + 0x28);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342BA0);
#ifdef SKIP_ASM
extern "C" void func_003584B8(void* self, void* stream);
struct sVEntry_func_00342BA0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};
struct sVEntry1_func_00342BA0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00342BA0(void* self, void* stream)
{
    func_003584B8(self, stream);
    sVEntry_func_00342BA0* vt = *(sVEntry_func_00342BA0**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x10, 0x30);
    sVEntry1_func_00342BA0* vt1 = *(sVEntry1_func_00342BA0**)stream;
    vt1[5].fn((char*)stream + vt1[5].delta, *(int*)((char*)self + 0x40));
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342C08);
#ifdef SKIP_ASM
extern "C" void* func_00355280(void* self, void* a1, int type, void* a3);
extern "C" void func_00355F10(void* self, void* a1);
extern void* D_004906F0[];

struct sNode_342C08 {
    int pad00[2];
    unsigned int flags;
};

struct sDef_342C08 {
    int f00;
    float time;
    int f08;
    int mode;
};

struct sSelf_342C08 {
    int pad00[3];
    void** vt;
    int pad10[2];
    sNode_342C08* node;
    int pad1C[4];
    int t2C;
    int t30;
};

extern "C" void* func_00342C08(sSelf_342C08* self, void* a1, void* a2, sDef_342C08* def, void* a4)
{
    func_00355280(self, a1, 0, a2);
    self->vt = D_004906F0;
    self->t2C = (int)(def->time * (float)*(int*)(D_004A5B64 + 0x10));
    self->t30 = def->f08;
    func_00355F10(self, a4);
    if (def->mode == 0) {
        self->node->flags &= 0xFFFFFFF0;
    } else if (def->mode == 1) {
        self->node->flags |= 1;
        self->node->flags = (self->node->flags & ~2u) | 4;
    } else {
        sNode_342C08* n = self->node;
        if ((n->flags & 3) == 3) {
            n->flags = (n->flags & ~2u) | 4;
        }
    }
    return self;
}
#endif


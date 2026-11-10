#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPSPBezierMan_cPSPBezierMan(void* self);
extern const char D_00492EA0[];

//99.23%
INCLUDE_ASM("render/bezierman", cBezierMan_construct__Fv);
#ifdef SKIP_ASM
void* cBezierMan_construct()
{
    void* mem = cMemMan_alloc(0x9C90, D_00492EA0, 0, 0);
    return cPSPBezierMan_cPSPBezierMan(mem);
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038AF30);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag_AF30(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_00372B78(void* self, int a1, int a2, int a3, int a4, int a5, int a6);
struct cBezierVirt;
extern "C" void func_0038D638(cBezierVirt* self);
extern const char D_00492EB0[];
extern const char D_00492EC0[];
extern const char D_00492ED0[];

struct sUV_AF30 {
    float u, v;
};

struct sUVTables_AF30 {
    char pad0[0x40];
    sUV_AF30 grid6[8][6];       // 0x40
    sUV_AF30 grid8[8][8];       // 0x1C0
};

extern sUVTables_AF30 D_005049C0_AF30 __asm__("D_005049C0");

extern "C" void func_0038AF30(void* self)
{
    char* s = (char*)self;
    func_00372B78(s + 0x10, 0x384, 0xDC, 0x8C, 0, 0x92, 0x5D);
    *(void**)(s + 0x440) = operator_new_tag_AF30(0x38400, D_00492EB0, 0, 0);
    *(void**)(s + 0x444) = operator_new_tag_AF30(0xDC00, D_00492EC0, 0, 0);
    *(void**)(s + 0x448) = operator_new_tag_AF30(0x8C00, D_00492ED0, 0, 0);
    *(void**)(s + 0x44C) = *(void**)(s + 0x440);
    *(void**)(s + 0x450) = *(void**)(s + 0x444);
    *(void**)(s + 0x454) = *(void**)(s + 0x448);
    *(int*)(s + 0x344) = 1;
    *(int*)(s + 0x348) = 1;
    *(int*)(s + 0x34C) = 1;
    *(int*)(s + 0x350) = 1;
    *(int*)(s + 0x354) = 0;
    *(float*)(s + 0x358) = 8.0f;
    *(float*)(s + 0x33C) = 3000.0f;
    *(float*)(s + 0x340) = 15000.0f;
    func_0038D638((cBezierVirt*)self);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            sUV_AF30 t;
            t.v = (float)i * 0.2f;
            t.u = (float)j * 0.2f;
            D_005049C0_AF30.grid6[j][i] = t;
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            sUV_AF30 t;
            t.v = (float)i * 0.1428571492433548f;
            t.u = (float)j * 0.1428571492433548f;
            D_005049C0_AF30.grid8[j][i] = t;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038B0F8);
#ifdef SKIP_ASM
void func_0038D660(void*);
void cMemMan_free(void*);

extern "C" void func_0038B0F8(void* self)
{
    func_0038D660(self);
    if (*(void**)((char*)self + 0x440) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x440));
    }
    if (*(void**)((char*)self + 0x444) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x444));
    }
    if (*(void**)((char*)self + 0x448) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x448));
    }
}
#endif

extern "C" void* func_003739D0(void*);

//100%
INCLUDE_ASM("render/bezierman", func_0038B158__FPv);
#ifdef SKIP_ASM
void* func_0038B158(void* self)
{
    return func_003739D0((char*)self + 0x10);
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038B178__FPv);
#ifdef SKIP_ASM
void func_0038B178(void* self)
{
    *(int*)((char*)self + 0x458) = 0;
    *(int*)((char*)self + 0x3c9c) = 0;
    *(int*)((char*)self + 0x4a60) = 0;
    *(int*)((char*)self + 0x4d84) = 0;
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038B190);
#ifdef SKIP_ASM
struct sBezPatch_B190 {
    void* obj;              // 0x0
    int extra;              // 0x4
    int kind;               // 0x8
    unsigned short tex;     // 0xC
    short pad;
};

struct sBezMixPatch_B190 {
    void* obj;              // 0x0
    int extra;              // 0x4
    int kind;               // 0x8
    unsigned short tex;     // 0xC
    short pad;
    unsigned char types[4]; // 0x10
};

struct sBezMan_B190 {
    char pad_0x0[0x458];
    int n4;                         // 0x458
    sBezPatch_B190 list4[900];      // 0x45C
    int n6;                         // 0x3C9C
    sBezPatch_B190 list6[220];      // 0x3CA0
    int n8;                         // 0x4A60
    sBezPatch_B190 list8[50];       // 0x4A64
    int nMix;                       // 0x4D84
    sBezMixPatch_B190 mix[75];      // 0x4D88
};

struct sBezObj_B190 {
    char pad_0x0[0x1A6];
    unsigned short tex4;    // 0x1A6
    unsigned short tex6;    // 0x1A8
    unsigned short tex8;    // 0x1AA
};

extern "C" void func_0038B190(sBezMan_B190* m, sBezObj_B190* o, unsigned char* t, int unused, int extra)
{
    if (t[0] == t[2] && t[1] == t[3] && t[0] == t[1]) {
        switch (t[0]) {
        case 4:
            if (m->n4 < 900) {
                sBezPatch_B190* e = &m->list4[m->n4++];
                e->extra = extra;
                e->obj = o;
                e->kind = 0;
                e->tex = o->tex4;
            }
            break;
        case 6:
            if (m->n6 < 220) {
                sBezPatch_B190* e = &m->list6[m->n6++];
                e->extra = extra;
                e->kind = 1;
                e->obj = o;
                e->tex = o->tex6;
            }
            break;
        case 8:
            if (m->n8 < 50) {
                sBezPatch_B190* e = &m->list8[m->n8++];
                e->extra = extra;
                e->kind = 2;
                e->obj = o;
                e->tex = o->tex8;
            }
            break;
        }
        return;
    }
    if (m->nMix < 75) {
        sBezMixPatch_B190* e = &m->mix[m->nMix++];
        e->extra = extra;
        e->obj = o;
        int* pk = &e->kind;
        *pk = (t[0] == 8 || t[1] == 8 || t[2] == 8 || t[3] == 8) ? 2 : 0;
        e->tex = o->tex8;
        for (int i = 0; i < 4; i++) {
            e->types[i] = t[i];
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038B338);
#ifdef SKIP_ASM
extern int D_004A44D4;

struct sBezVEntry_B338 {
    short delta;
    short index;
    void (*fn)(void*, void*, void*, void*, void*, int);
};

extern "C" void func_0038B338(void* self, void* a, void* b, void* c, void* d)
{
    if (D_004A44D4 == 0) {
        sBezVEntry_B338* vt = *(sBezVEntry_B338**)((char*)self + 0x4);
        vt[22].fn((char*)self + vt[22].delta, a, b, c, d, 0);
    }
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038B370);
#ifdef SKIP_ASM
// REF refs/ssx3-rust crates/ssx3-game/src/packetless/bezier.rs (end_pass): field meanings.
extern "C" void func_0031BE50(float angle, float* s, float* c);

struct sRS_B370 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex[2];
};

struct sVt_B370 {
    short delta;
    short index;
    void* fn;
};

struct sGfx_B370 {
    char pad_0x0[0x274];
    int view;                       // 0x274
    char pad_0x278[0xE84 - 0x278];
    sRS_B370* top;                  // 0xE84
    char pad_0xE88[0x10D8 - 0xE88];
    sVt_B370* vtable;               // 0x10D8
    char pad_0x10DC[0x18F0 - 0x10DC];
    char* dm;                       // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int cursor;            // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int matValid;                   // 0x6B90
};

// PORT: Sony's 128-bit quadword (u_long128); used for one aligned 16-byte copy.
typedef int cQuad_B370 __attribute__((mode(TI)));

struct sV4_B370 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy).
static inline void Copy_B370(void* dst, const void* src)
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

struct sM4_B370 {
    sV4_B370 r[4];
    sM4_B370() {}
    sM4_B370(const sM4_B370& o) { Copy_B370(this, &o); }
    sM4_B370& operator=(const sM4_B370& o)
    {
        Copy_B370(this, &o);
        return *this;
    }
};

static inline sM4_B370 operator*(const sM4_B370& a, const sM4_B370& b)
{
    sM4_B370 r;
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
        : "r"(&r), "r"(&a), "r"(&b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV4_B370 Scale_B370(const sV4_B370& v, float s)
{
    sV4_B370 r;
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

// PORT: PS2-only VU0 inline asm (matrix times vector).
static inline sV4_B370 Xform_B370(const sM4_B370& m, const sV4_B370& v)
{
    sV4_B370 r;
    __asm__(
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(r)
        : "r"(&m), "m"(v)
        : "memory");
    return r;
}

static inline sV4_B370 Row3_B370(const sM4_B370& m)
{
    return m.r[3];
}

// Orthonormal inverse in place: transpose the 3x3 part, position = -(R^T * t).
static inline void OrthoInverse_B370(sM4_B370& t)
{
    float tmp;
    tmp = t.r[0].y;
    t.r[0].y = t.r[1].x;
    t.r[1].x = tmp;
    tmp = t.r[0].z;
    t.r[0].z = t.r[2].x;
    t.r[2].x = tmp;
    tmp = t.r[1].z;
    t.r[1].z = t.r[2].y;
    t.r[2].y = tmp;
    float neg = -1.0f;
    sV4_B370 p = Scale_B370(Row3_B370(t), neg);
    p.w = 0.0f;
    p = Xform_B370(t, p);
    p.w = 1.0f;
    t.r[3] = p;
}

static inline float Frac_B370(float x)
{
    return x - (float)(int)x;
}

static inline void SetTexture_B370(sGfx_B370* gm, int idx, int tex)
{
    gm->top->tex[idx] = tex;
}

static inline int Hash8_B370(int k)
{
    return ((k << 5) + k) ^ ((((k >> 8) << 5) + k) >> 8) ^ ((((k >> 16) << 5) + k) >> 16)
        ^ ((((k >> 24) << 5) + k) >> 24);
}

// PORT: PS2-only VU0 inline asm (in-place 4x4 matrix multiply m = m * b).
static inline void MulIn_B370(void* m, const void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// Multiply m by a rotation of `angle` about `axis`, built in the local matrix r.
#define ROTATE_B370(m, r, axis, angle)          \
    {                                           \
        sM4_B370* mm = (m);                     \
        float* ax = (axis);                     \
        float s;                                \
        float c;                                \
        func_0031BE50(angle, &s, &c);           \
        float t = 1.0f - c;                     \
        float x = ax[0];                        \
        float y = ax[1];                        \
        float z = ax[2];                        \
        float tx = t * x;                       \
        float ty = t * y;                       \
        float tz = t * z;                       \
        float sx = s * x;                       \
        float sy = s * y;                       \
        float sz = s * z;                       \
        r.r[0].x = tx * x + c;                  \
        r.r[1].x = tx * y + sz;                 \
        r.r[2].x = tx * z - sy;                 \
        r.r[0].y = ty * x - sz;                 \
        r.r[1].y = ty * y + c;                  \
        r.r[2].y = ty * z + sx;                 \
        r.r[0].z = tz * x + sy;                 \
        r.r[1].z = tz * y - sx;                 \
        r.r[2].z = tz * z + c;                  \
        r.r[3].x = 0.0f;                        \
        r.r[3].y = 0.0f;                        \
        r.r[3].z = 0.0f;                        \
        r.r[0].w = 0.0f;                        \
        r.r[1].w = 0.0f;                        \
        r.r[2].w = 0.0f;                        \
        r.r[3].w = 1.0f;                        \
        MulIn_B370(mm, &r);                     \
    }

struct sPatch_B370 {
    char* obj;              // 0x0
    int extra;              // 0x4
    int kind;               // 0x8
    short slot;             // 0xC
    short pad;
};

struct sMixPatch_B370 {
    char* obj;              // 0x0
    int extra;              // 0x4
    int kind;               // 0x8
    short slot;             // 0xC
    short pad;
    unsigned char types[4]; // 0x10
};

struct sDefer_B370 {
    short tex;              // 0x0
    short tex2;             // 0x2
    int kind;               // 0x4
    unsigned int addr;      // 0x8
    sPatch_B370* patch;     // 0xC
};

struct sBezMan_B370 {
    char pad_0x0[0x10];
    char cache[0x360 - 0x10];       // 0x10
    sM4_B370 shine;                 // 0x360
    char pad_0x3A0[0x3E0 - 0x3A0];
    sM4_B370 fx;                    // 0x3E0
    sV4_B370 colour;                // 0x420
    float light[3];                 // 0x430
    char pad_0x43C[0x458 - 0x43C];
    int n4;                         // 0x458
    sPatch_B370 list4[900];         // 0x45C
    int n6;                         // 0x3C9C
    sPatch_B370 list6[220];         // 0x3CA0
    int n8;                         // 0x4A60
    sPatch_B370 list8[50];          // 0x4A64
    int nMix;                       // 0x4D84
    sMixPatch_B370 mix[75];         // 0x4D88
    int nDefer;                     // 0x5364
    sDefer_B370 defer[1];           // 0x5368
};

struct sBucket_B370 {
    int state;              // 0x0
    int key;                // 0x4
    int matrix;             // 0x8
    int fC;                 // 0xC
    int f10;                // 0x10
    int f14;                // 0x14
    int f18;                // 0x18
    sBucket_B370* next;     // 0x1C
};

struct sSortKey_B370 {
    int key;
    sBucket_B370* bucket;
};

struct sDefer2_B370 {
    char pad_0x0[0x57494];
    int nBuckets;                   // 0x57494
    sBucket_B370 buckets[0x820];    // 0x57498
    char pad_0x67898[0x678A0 - 0x67898];
    sBucket_B370* hash[257];        // 0x678A0
    int nKeys;                      // 0x67CA4
    sSortKey_B370 keys[1];          // 0x67CA8
};

extern sGfx_B370* D_004A5B80_B370 __asm__("D_004A5B80");
extern float D_004A4344;
extern float D_004A434C;
extern float D_004A4350;
extern float D_004A4354;
extern float D_004A4440;
extern float D_004FF140[4];
extern float D_004FF150[4];
extern float D_004FF160[4];
extern sM4_B370 D_004FF1A0_B370 __asm__("D_004FF1A0");
extern sM4_B370 D_005049C0;
extern char* D_0044B9D0[];
extern "C" void func_0037D968_B370(void* gm) __asm__("func_0037D968");
extern "C" void func_00374A88(void* cache, int n, void* list, int lod, int view);
extern "C" void func_00374AE0(void* cache, int n, void* list, int lod, int view);
extern "C" void func_00374B38(void* cache);
extern "C" int func_003747A0(void* cache, void* obj, int lod, int view, int normals);
extern "C" void func_0038C788(void* self, int type, int idx);
extern "C" void func_0038CA70_B370(void* self, int kind, int sub, void* patch, unsigned int* handle) __asm__("func_0038CA70");
extern "C" void func_0038CE20_B370(void* self, int sub, void* patch, void* ext, unsigned int* handle) __asm__("func_0038CE20");
extern "C" void func_0038D968_B370(void* self, unsigned int* handle) __asm__("func_0038D968");
extern "C" int func_00394ED0_B370(void* dm, void* st, int hash) __asm__("func_00394ED0");
extern "C" sBucket_B370* func_00395000(void* dm, int state, int key, int hash, int matrix);
extern "C" int func_00364240_B370(int state, int key) __asm__("func_00364240");
extern "C" void func_00362978_B370(void* bucket, int start, int end) __asm__("func_00362978");

typedef sM4_B370* (*tGetMat_B370)(void*);

// The DeferMan bucket for (state, key, matrix), created and linked into the hash and sort lists if new.
static inline sBucket_B370* GetBucket_B370(sDefer2_B370* dm, int state, int key, int matrix)
{
    int h = Hash8_B370(state ^ ((short)key | (((key >> 16) + 2) << 16))) & 0xFF;
    sBucket_B370* b = func_00395000(dm, state, key, h, matrix);
    if (b != 0) {
        return b;
    }
    sBucket_B370* nb = &dm->buckets[dm->nBuckets++];
    nb->state = state;
    nb->key = key;
    nb->matrix = matrix;
    nb->fC = 0;
    nb->f14 = 0;
    nb->f18 = 0;
    b = nb;
    int sk = func_00364240_B370(state, key);
    b->next = dm->hash[h];
    dm->hash[h] = b;
    dm->keys[dm->nKeys].key = sk;
    dm->keys[dm->nKeys++].bucket = b;
    return b;
}

// PORT: uncached (0x30000000) packet pointers held in int; VU0 inline asm.
static inline void Render_B370(sBezMan_B370* self)
{
    sGfx_B370* gm = D_004A5B80_B370;
    self->nDefer = 0;
    unsigned int* cursor = &gm->cursor;
    gm->top->f0 &= ~0xC;
    gm->top->f0 |= 0x30;
    sDefer2_B370* dm = (sDefer2_B370*)gm->dm;
    if (gm->matValid == 0) {
        func_0037D968_B370(gm);
    }
    int matrix = *(int*)((char*)dm + 0x69CC0);
    sM4_B370 view = *((tGetMat_B370)gm->vtable[35].fn)((char*)gm + gm->vtable[35].delta);
    int viewIdx = gm->view;
    sM4_B370 t = view;
    OrthoInverse_B370(t);
    sV4_B370 cam = t.r[3];

    {
    float s1, c1, s2, c2;
    float ay = cam.y * D_004A4440;
    float ax = cam.x * D_004A4440;
    ay = Frac_B370(ay * 0.15915494f) * 6.2831855f;
    ax = Frac_B370(ax * 0.15915494f) * 6.2831855f;
    func_0031BE50(ay, &s1, &c1);
    func_0031BE50(ax, &s2, &c2);
    sM4_B370 a;
    a.r[0].x = 1.0f; a.r[1].x = 0.0f; a.r[2].x = 0.0f; a.r[3].x = 0.0f;
    a.r[0].y = 0.0f; a.r[1].y = s1; a.r[2].y = -c1; a.r[3].y = 0.0f;
    a.r[0].z = 0.0f; a.r[1].z = c1; a.r[2].z = s1; a.r[3].z = 0.0f;
    a.r[0].w = 0.0f; a.r[1].w = 0.0f; a.r[2].w = 0.0f; a.r[3].w = 1.0f;
    sM4_B370 b;
    float nc2 = -c2;
    b.r[0].x = s2; b.r[1].x = 0.0f; b.r[2].x = c2; b.r[3].x = 0.0f;
    b.r[0].y = 0.0f; b.r[1].y = 1.0f; b.r[2].y = 0.0f; b.r[3].y = 0.0f;
    b.r[0].z = nc2; b.r[1].z = 0.0f; b.r[2].z = s2; b.r[3].z = 0.0f;
    b.r[0].w = 0.0f; b.r[1].w = 0.0f; b.r[2].w = 0.0f; b.r[3].w = 1.0f;
    sM4_B370 rot = a * b;
    sM4_B370 shine = D_005049C0 * rot;
    self->shine = shine;
    }

    float ry = cam.y * D_004A4344;
    float rx = cam.x * D_004A4344;
    float rz = cam.z * D_004A4344;
    ry = Frac_B370(ry * 0.15915494f) * 6.2831855f;
    rx = Frac_B370(rx * 0.15915494f) * 6.2831855f;
    rz = Frac_B370(rz * 0.15915494f) * 6.2831855f;
    float s3, c3, s4, c4, s5, c5;
    func_0031BE50(ry, &s3, &c3);
    func_0031BE50(rx, &s4, &c4);
    func_0031BE50(rz, &s5, &c5);
    self->fx = D_004FF1A0_B370;
    {
        sM4_B370 r;
        ROTATE_B370(&self->fx, r, D_004FF140, ry)
        ROTATE_B370(&self->fx, r, D_004FF150, rx)
        ROTATE_B370(&self->fx, r, D_004FF160, rz)
    }

    sV4_B370 col;
    col.x = 128.0f;
    col.y = 128.0f;
    col.z = 128.0f;
    col.w = 128.0f;
    *(cQuad_B370*)((char*)self + 0x420) = *(cQuad_B370*)&col;
    *(float*)((char*)self + 0x430) = D_004A434C;
    *(float*)((char*)self + 0x434) = D_004A4350;
    *(float*)((char*)self + 0x438) = D_004A4354;

    gm->top[1] = gm->top[0];
    sRS_B370* st = gm->top;
    gm->top = st + 1;
    st[1].f8 = (st[1].f8 & ~0x3E0) | 0x40;
    gm->top->f4 &= ~3;
    gm->top->f8 &= 0xE00003FF;
    gm->top->f4 = (gm->top->f4 & 0xFE7FFFFF) | 0x800000;
    gm->top->f4 &= 0xFFCFFFFF;
    gm->top->f4 = (gm->top->f4 & 0xFFF00FFF) | 0x14000;
    gm->top->f4 = (gm->top->f4 & ~0x7C) | 4;
    gm->top->f4 = (gm->top->f4 & ~0xF80) | 0x400;
    func_00374A88(self->cache, self->n4, self->list4, 0, viewIdx);
    func_00374A88(self->cache, self->n6, self->list6, 1, viewIdx);
    func_00374A88(self->cache, self->n8, self->list8, 2, viewIdx);
    func_00374AE0(self->cache, self->nMix, self->mix, 2, viewIdx);
    if (viewIdx == 0) {
        func_00374B38(self->cache);
    }
    gm->top->f0 = (gm->top->f0 & ~0x3C0) | 0x40;
    gm->top->fC &= 0xC0000000;
    gm->top->f8 = (gm->top->f8 & ~0x1F) | (*(int*)((char*)dm + 0x69CC4) & 0x1F);
    sRS_B370* s = gm->top;
    int base = func_00394ED0_B370(dm, s, Hash8_B370(s->f0 ^ s->f4 ^ s->f8 ^ s->fC) & 0xFF);
    for (int i = 0; i < self->n8; i++) {
        sPatch_B370* e = &self->list8[i];
        char* obj = e->obj;
        if (e->slot < 0) {
            e->slot = func_003747A0(self->cache, obj, 2, viewIdx, (*(int*)(obj + 0xC) & 0x600000) != 0);
            if (e->slot >= 0) {
                func_0038C788(self, 2, e->slot);
            }
        }
        func_0038CA70_B370(self, 2, e->slot, e, cursor);
    }
    for (int i = 0; i < self->n6; i++) {
        sPatch_B370* e = &self->list6[i];
        char* obj = e->obj;
        if (e->slot < 0) {
            e->slot = func_003747A0(self->cache, obj, 1, viewIdx, (*(int*)(obj + 0xC) & 0x600000) != 0);
            if (e->slot >= 0) {
                func_0038C788(self, 1, e->slot);
            }
        }
        func_0038CA70_B370(self, 1, e->slot, e, cursor);
    }
    for (int i = 0; i < self->n4; i++) {
        sPatch_B370* e = &self->list4[i];
        char* obj = e->obj;
        if (e->slot < 0) {
            e->slot = func_003747A0(self->cache, obj, 0, viewIdx, 0);
            if (e->slot >= 0) {
                func_0038C788(self, 0, e->slot);
            }
        }
        func_0038CA70_B370(self, 0, e->slot, e, cursor);
    }
    gm->top->f0 = (gm->top->f0 & ~0x3C0) | 0x80;
    gm->top->fC &= 0xC0000000;
    for (int i = 0; i < self->nMix; i++) {
        sMixPatch_B370* e = &self->mix[i];
        char* obj = e->obj;
        if (e->slot < 0) {
            e->slot = func_003747A0(self->cache, obj, 2, viewIdx, (*(int*)(obj + 0xC) & 0x600000) != 0);
            if (e->slot >= 0) {
                func_0038C788(self, 2, e->slot);
            }
        }
        func_0038CE20_B370(self, e->slot, e, e->types, cursor);
    }
    if (self->nDefer != 0) {
        int key = *(int*)&self->defer[0].tex;
        SetTexture_B370(gm, 0, self->defer[0].tex);
        SetTexture_B370(gm, 1, self->defer[0].tex2);
        gm->top->f0 &= ~0xC;
        gm->top->f0 |= 0x30;
        sBucket_B370* cur = GetBucket_B370(dm, base, key, matrix);
        unsigned int start = *cursor;
        unsigned long* p = (unsigned long*)(start | 0x30000000);
        int j;
        for (j = 0; j < self->nDefer; j++) {
            sDefer_B370* d = &self->defer[j];
            int k2 = *(int*)&d->tex;
            if (k2 != key) {
                unsigned int end = (unsigned int)p & 0x0FFFFFFF;
                *cursor = end;
                func_00362978_B370(cur, start, end);
                *cursor += 0x10;
                start = *cursor;
                p = (unsigned long*)(start | 0x30000000);
                SetTexture_B370(gm, 0, d->tex);
                SetTexture_B370(gm, 1, d->tex2);
                cur = GetBucket_B370(dm, base, k2, matrix);
                key = k2;
            }
            p[0] = ((unsigned long)d->addr << 32) | 0x50000000;
            p[1] = 0;
            p[2] = 0x10000001;
            p[3] = 0;
            char* tbl = D_0044B9D0[d->kind];
            int extra = d->patch->extra;
            p[4] = 0;
            p[5] = (unsigned long)(*(int*)(tbl + extra * 8 + 0x10) | 0x14000000) << 32;
            p += 6;
        }
        unsigned int end = (unsigned int)p & 0x0FFFFFFF;
        *cursor = end;
        func_00362978_B370(cur, start, end);
        *cursor += 0x10;
    }
    gm->top--;
    SetTexture_B370(gm, 0, -1);
    SetTexture_B370(gm, 1, -1);
    gm->top->f0 &= ~0x3C0;
    gm->top->fC &= 0xC0000000;
    func_0038D968_B370(self, cursor);
}

extern "C" void func_0038B370(sBezMan_B370* self)
{
    Render_B370(self);
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038C590);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV4_C590 {
    float x, y, z, w;
    sV4_C590() {}
    sV4_C590(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sV3_C590 {
    float x, y, z;
};

struct sUV_C590 {
    float u, v;
};

struct sBezVert_C590 {
    float st[4];                    // 0x00
    int rgba[4];                    // 0x10
    sV4_C590 pos;                   // 0x20
};

struct sGmVE_C590 {
    short delta;
    short index;
    void (*fn)(void*, int, sBezVert_C590*, int);
};

extern char* D_004A5B80_C590 __asm__("D_004A5B80");

static inline void setVert_C590(sBezVert_C590* v, const sV3_C590& p, const sUV_C590& uv)
{
    v->pos = sV4_C590(p.x, p.y, p.z, 1.0f);
    v->st[0] = uv.u;
    v->st[1] = uv.v;
    v->st[2] = 1.0f;
    v->rgba[0] = v->rgba[1] = v->rgba[2] = 0x80;
    v->rgba[3] = 0x80;
}

// Draws the n x n patch grid as strips: rows, then columns.
extern "C" void func_0038C590(sV3_C590* pts, sUV_C590* uvs, int n)
{
    sBezVert_C590 verts[64];
    char* gfx = D_004A5B80_C590;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            setVert_C590(&verts[j], pts[i * n + j], uvs[i * n + j]);
        }
        sGmVE_C590* vt = *(sGmVE_C590**)(gfx + 0x10D8);
        vt[73].fn(gfx + vt[73].delta, n, verts, 0);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            setVert_C590(&verts[j], pts[j * n + i], uvs[j * n + i]);
        }
        sGmVE_C590* vt = *(sGmVE_C590**)(gfx + 0x10D8);
        vt[73].fn(gfx + vt[73].delta, n, verts, 0);
    }
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038C788);
#ifdef SKIP_ASM
// PORT: builds a DMA/VIF/GIF packet in uncached (0x30000000) memory; addresses held in int and packed into 64-bit words.
extern "C" void func_0038C788(void* self, int type, int idx)
{
    char* s = (char*)self;
    int n = type * 2 + 4;
    int a;
    switch (type) {
    case 0:
        a = *(int*)(s + 0x58) + idx * 0xC0;
        break;
    case 1:
        a = *(int*)(s + 0x5C) + idx * 0x1B0;
        break;
    case 2:
        a = *(int*)(s + 0x60) + idx * 0x300;
        break;
    default:
        a = 0;
        break;
    }
    int b;
    switch (type) {
    case 0:
        b = *(int*)(s + 0x64) + idx * 0x80;
        break;
    case 1:
        b = *(int*)(s + 0x68) + idx * 0x120;
        break;
    case 2:
        b = *(int*)(s + 0x6C) + idx * 0x200;
        break;
    default:
        b = 0;
        break;
    }
    int c;
    switch (type) {
    case 0:
        c = *(int*)(s + 0x70) + idx * 0x80;
        break;
    case 1:
        c = *(int*)(s + 0x74) + idx * 0x120;
        break;
    case 2:
        c = *(int*)(s + 0x78) + idx * 0x200;
        break;
    default:
        c = 0;
        break;
    }
    int nn = n * n;
    long* p = (long*)((*(int*)(s + (type << 2) + 0x44C) + (idx << 8)) | 0x30000000);
    p[0] = 0x10000003;
    p[1] = 0;
    p[2] = 0x3000000005000000;
    p[3] = 0;
    p[4] = 0x3F8000003F800000;
    p[5] = 0x4040404020000000;
    p[6] = 0;
    p[7] = (long)((nn << 16) | 0x78008000) << 32;
    p[8] = (((nn * 3) >> 2) | 0x30000000) | ((long)a << 32);
    p[9] = 0;
    p[10] = 0x10000001;
    p[11] = 0;
    p[12] = 0x5050505020000000;
    p[13] = (long)((nn << 16) | 0x74008040) << 32;
    p[14] = ((nn >> 1) | 0x30000000) | ((long)b << 32);
    p[15] = 0;
    p += 16;
    p[0] = 0x10000001;
    p[1] = 0;
    p[2] = 0x5050505020000000;
    p[3] = (long)((nn << 16) | 0x74008080) << 32;
    p[4] = ((nn >> 1) | 0x30000000) | ((long)c << 32);
    p[5] = 0;
    p += 6;
    p[0] = 0x60000000;
    p[1] = 0;
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038CA08);
#ifdef SKIP_ASM
extern void* D_004A5B80;
extern "C" unsigned int func_0037DF88(void* self, void* a1, void* a2);

struct sBezVec4_CA08 {
    float x, y, z, w;
};

extern "C" void func_0038CA08(void* self, void* obj)
{
    char* o = (char*)obj;
    sBezVec4_CA08 a;
    sBezVec4_CA08 b;
    a.x = *(float*)(o + 0x158);
    a.y = *(float*)(o + 0x15C);
    a.z = *(float*)(o + 0x160);
    a.w = 1.0f;
    b.x = *(float*)(o + 0x164);
    b.y = *(float*)(o + 0x168);
    b.z = *(float*)(o + 0x16C);
    b.w = 1.0f;
    func_0037DF88(D_004A5B80, &a, &b);
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038CA70);
#ifdef SKIP_ASM
struct sRS_CA70 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short tex2;
};
struct sEnt_CA70 {
    sRS_CA70 rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};
struct sRing_CA70 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sEnt_CA70 ents[1];              // 0x80
};
struct sGfx_CA70 {
    char pad_0x0[0xE84];
    sRS_CA70* top;                  // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sRing_CA70* ring;               // 0x18F0
};
struct sPatch_CA70 {
    void* obj;
    int extra;
};
struct sDefer_CA70 {
    unsigned short tex;
    unsigned short tex2;
    int kind;
    unsigned int addr;
    sPatch_CA70* patch;
};
struct sBezMan_CA70 {
    char pad_0x0[0x28];
    int base28[(0x44C - 0x28) / 4]; // 0x28
    unsigned int base44C[(0x5364 - 0x44C) / 4]; // 0x44C
    int nDefer;                     // 0x5364
    sDefer_CA70 defer[1];           // 0x5368
};
extern sGfx_CA70* D_004A5B80_CA70 __asm__("D_004A5B80");
extern int D_004A44D8;
extern int D_004A44DC;
extern char* D_0044B9D0[];
extern "C" int func_0038CA08_i(void* self, void* obj) __asm__("func_0038CA08");
extern "C" void* func_0038F708(void* obj, int a1, int a2);
extern "C" void func_0038D168(void* self, int a1, void* obj, int kind, void* tbl, void* patch, unsigned int* handle);
extern "C" void func_0038D448(void* self, int kind, void* tbl, void* patch, int a4, unsigned int* handle);

// PORT: uncached (0x30000000) pointers held in int.
extern "C" void func_0038CA70(sBezMan_CA70* self, int kind, int sub, sPatch_CA70* patch, unsigned int* handle)
{
    int draw = 0;
    int a1 = self->base28[kind] + sub * 12;
    char* o = (char*)func_0038F708(patch->obj, 0x1B0, 2);
    sGfx_CA70* gfx = D_004A5B80_CA70;
    unsigned int addr = self->base44C[kind] + (sub << 8);
    unsigned int n = kind * 2 + 4;
    int doA = (*(int*)(o + 0xC) & 0x600000) && n;
    if (doA) {
        if (D_004A44D8 != 0)
            doA = 0;
        else
            draw = doA != 0;
    }
    int res = 0;
    if (kind == 2)
        res = func_0038CA08_i(self, o);
    int doB = res != 0;
    if (doB) {
        if (D_004A44DC != 0)
            doB = 0;
        else
            draw |= doB;
    }
    switch (*(int*)(o + 0xC) & 0x180000) {
    case 0x80000:
        draw = 1;
        gfx->top->f0 = (gfx->top->f0 & ~0xC) | 8;
        break;
    case 0x100000:
        draw = 1;
        gfx->top->f0 = (gfx->top->f0 & ~0xC) | 4;
        break;
    case 0x180000:
        draw = 1;
        gfx->top->f0 |= 0xC;
        break;
    default:
        gfx->top->f0 &= ~0xC;
        break;
    }
    if (draw) {
        int tex = *(short*)(o + 0x1A0);
        gfx->top->tex = tex;
        int tex2 = *(short*)(o + 0x1A2);
        gfx->top->tex2 = tex2;
        unsigned int buf = *handle;
        long* p = (long*)(buf | 0x30000000);
        p[0] = ((long)addr << 32) | 0x50000000;
        p[2] = 0x10000001;
        p[1] = 0;
        p[3] = 0;
        char* tbl = D_0044B9D0[kind];
        int extra = patch->extra;
        p[4] = 0;
        p[5] = (long)(*(int*)(tbl + extra * 8 + 0x10) | 0x14000000) << 32;
        p += 6;
        unsigned int next = (unsigned int)p & 0x0FFFFFFF;
        *handle = next;
        sRing_CA70* ring = gfx->ring;
        sRS_CA70* rs = gfx->top;
        if (ring->count < 0xA28) {
            rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
            int key = *(int*)((char*)ring + 0x69CC0);
            sEnt_CA70* e = (sEnt_CA70*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
            e->rs = *rs;
            e->buf = buf;
            e->next = next;
            e->key = key;
            e->flag = 0;
        }
        *handle += 0x10;
        if (doA)
            func_0038D168(self, a1, o, kind, tbl, patch, handle);
        if (doB)
            func_0038D448(self, kind, tbl, patch, res, handle);
    } else {
        self->defer[self->nDefer].tex = *(unsigned short*)(o + 0x1A0);
        self->defer[self->nDefer].tex2 = *(unsigned short*)(o + 0x1A2);
        self->defer[self->nDefer].kind = kind;
        self->defer[self->nDefer].addr = addr;
        self->defer[self->nDefer].patch = patch;
        self->nDefer++;
    }
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038CE20);
#ifdef SKIP_ASM
struct sRS_CE20 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short tex2;
};
struct sEnt_CE20 {
    sRS_CE20 rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};
struct sRing_CE20 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sEnt_CE20 ents[1];              // 0x80
};
struct sGfx_CE20 {
    char pad_0x0[0xE84];
    sRS_CE20* top;                  // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sRing_CE20* ring;               // 0x18F0
};
struct sPatch_CE20 {
    void* obj;
    int extra;
};
struct sBezMan_CE20 {
    char pad_0x0[0x28];
    int base28[(0x44C - 0x28) / 4]; // 0x28
    unsigned int base44C[(0x5364 - 0x44C) / 4]; // 0x44C
};
extern sGfx_CE20* D_004A5B80_CE20 __asm__("D_004A5B80");
extern int D_004A44D8;
extern int D_004A44DC;
struct sTex_CE20 {
    int addr;
    int pad;
};
extern sTex_CE20 D_00504960[];
extern "C" int func_0038CA08_i(void* self, void* obj) __asm__("func_0038CA08");
extern "C" void* func_0038F708(void* obj, int a1, int a2);
extern "C" void func_0038D168(void* self, int a1, void* obj, int kind, void* tbl, void* patch, unsigned int* handle);
extern "C" void func_0038D448(void* self, int kind, void* tbl, void* patch, int a4, unsigned int* handle);

// PORT: uncached (0x30000000) pointers held in int.
extern "C" void func_0038CE20(sBezMan_CE20* self, int sub, sPatch_CE20* patch, int* ext, unsigned int* handle)
{
    int a1 = self->base28[2] + sub * 12;
    char* o = (char*)func_0038F708(patch->obj, 0x1B0, 2);
    sGfx_CE20* gfx = D_004A5B80_CE20;
    unsigned int addr = self->base44C[2] + (sub << 8);
    int doA = (*(int*)(o + 0xC) & 0x600000) != 0;
    if (doA) {
        if (D_004A44D8 != 0)
            doA = 0;
    }
    int res = func_0038CA08_i(self, o);
    int doB = res != 0;
    if (D_004A44DC != 0)
        doB = 0;
    int tex = *(short*)(o + 0x1A0);
    gfx->top->tex = tex;
    int tex2 = *(short*)(o + 0x1A2);
    gfx->top->tex2 = tex2;
    switch (*(int*)(o + 0xC) & 0x180000) {
    case 0x80000:
        gfx->top->f0 = (gfx->top->f0 & ~0xC) | 8;
        break;
    case 0x100000:
        gfx->top->f0 = (gfx->top->f0 & ~0xC) | 4;
        break;
    case 0x180000:
        gfx->top->f0 |= 0xC;
        break;
    default:
        gfx->top->f0 &= ~0xC;
        break;
    }
    unsigned int buf = *handle;
    long* p = (long*)(buf | 0x30000000);
    if (ext) {
        p[0] = 0x10000002;
        p[1] = 0;
        p[2] = ((long)*ext << 32) | 0x6E0180D0;
        p[3] = 0;
        p[4] = 0;
        p[5] = (long)0x6C1080C0 << 32;
        p[6] = ((long)((int)patch->obj + 0x40) << 32) | 0x30000010;
        p[7] = 0;
        p += 8;
    }
    p[0] = ((long)addr << 32) | 0x50000000;
    p[2] = 0x10000001;
    p[1] = 0;
    p[3] = 0;
    sTex_CE20* tbl = D_00504960;
    int extra = patch->extra;
    p[4] = 0;
    p[5] = (long)(D_00504960[extra + 2].addr | 0x14000000) << 32;
    p += 6;
    unsigned int next = (unsigned int)p & 0x0FFFFFFF;
    *handle = next;
    sRing_CE20* ring = gfx->ring;
    sRS_CE20* rs = gfx->top;
    if (ring->count < 0xA28) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sEnt_CE20* e = (sEnt_CE20*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        e->rs = *rs;
        e->buf = buf;
        e->next = next;
        e->key = key;
        e->flag = 0;
    }
    *handle += 0x10;
    if (doA)
        func_0038D168(self, a1, o, 2, tbl, patch, handle);
    if (doB)
        func_0038D448(self, 2, tbl, patch, res, handle);
}
#endif

INCLUDE_ASM("render/bezierman", func_0038D168);

//100%
INCLUDE_ASM("render/bezierman", func_0038D448);
#ifdef SKIP_ASM
struct sRS_D448 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short tex2;
};
struct sEnt_D448 {
    sRS_D448 rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};
struct sRing_D448 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sEnt_D448 ents[1];              // 0x80
};
struct sMat_D448 {
    float m[4][4];
} __attribute__((aligned(16)));
// Projector light (PS2GraphicsMan AddLight record, 0xA0 bytes).
struct sLight_D448 {
    float color[4];                 // 0x00
    sMat_D448 mat0;                 // 0x10
    sMat_D448 mat;                  // 0x50
    float f90;                      // 0x90
    int tex;                        // 0x94
    char pad_0x98[8];
} __attribute__((aligned(16)));
struct sGfx_D448 {
    char pad_0x0[0xE84];
    sRS_D448* top;                  // 0xE84
    char pad_0xE88[0x13EC - 0xE88];
    unsigned int nLights;           // 0x13EC
    sLight_D448 lights[1];          // 0x13F0
    char pad_0x1490[0x18F0 - 0x1490];
    sRing_D448* ring;               // 0x18F0
};
struct sPatch_D448 {
    void* obj;
    int extra;
};
struct sTex_D448 {
    int addr;
    int pad;
};
extern sGfx_D448* D_004A5B80_D448 __asm__("D_004A5B80");

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMat_D448(void* d, const void* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

static inline int Tex_D448(sLight_D448* l)
{
    return l->tex;
}

static inline void QueueRecord_D448(sGfx_D448* gfx, unsigned int buf, unsigned int next, int flag)
{
    sRing_D448* ring = gfx->ring;
    sRS_D448* rs = gfx->top;
    if (ring->count < 0xA28) {
        int lyr = *(int*)((char*)ring + 0x69CC4) & 0x1F;
        rs->f8 = (rs->f8 & ~0x1F) | lyr;
        int key = *(int*)((char*)ring + 0x69CC0);
        sEnt_D448* e = (sEnt_D448*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        e->rs = *rs;
        e->buf = buf;
        e->next = next;
        e->key = key;
        e->flag = flag;
    }
}

// PORT: uncached (0x30000000) pointers held in int; PS2-only VU0 asm.
// One projector pass per light whose bit is set in mask: UNPACK the light matrix, then MSCAL the patch program.
extern "C" void func_0038D448(void* self, int kind, void* tbl, void* patch, int mask, unsigned int* handle)
{
    sGfx_D448* gfx = D_004A5B80_D448;
    sTex_D448* t = (sTex_D448*)tbl;
    int extra = ((sPatch_D448*)patch)->extra;
    unsigned int i;
    for (i = 0; i < gfx->nLights; i++) {
        sLight_D448* l = &gfx->lights[i];
        if ((mask >> i) & 1) {
            unsigned int buf = *handle;
            gfx->top->tex2 = Tex_D448(l);
            long* p = (long*)(buf | 0x30000000);
            gfx->top->f0 = (gfx->top->f0 & ~0x30) | 0x30;
            p[0] = 0x10000006;
            p[1] = 0;
            p[2] = 0x20000000;
            p[3] = (long)0x6C048080 << 32;
            CopyMat_D448(p + 4, &l->mat);
            p[12] = 0;
            p[13] = (long)(t[extra + 6].addr | 0x14000000) << 32;
            p += 14;
            unsigned int next = (unsigned int)p & 0x0FFFFFFF;
            *handle = next;
            QueueRecord_D448(gfx, buf, next, 1);
            *handle += 0x10;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038D638);
#ifdef SKIP_ASM
class cBezierVirt {
public:
    int field_0x0;
    // vptr lands at 0x4 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
};

extern "C" void func_0038D638(cBezierVirt* self)
{
    self->v17();
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038D660__FPv);
#ifdef SKIP_ASM
void func_0038D660(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038D668);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 random-number unit (R register); the PC port needs a C PRNG.
extern "C" void func_0038D668(float* seed)
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

//100%
INCLUDE_ASM("render/bezierman", func_0038D690);
#ifdef SKIP_ASM
struct sVec3_D690 { float x, y, z; };
struct sQuad_D690 { long lo; long hi; } __attribute__((aligned(16)));
struct sRS_D690 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};
struct sEnt_D690 {
    sRS_D690 rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};
struct sRing_D690 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sEnt_D690 ents[1];              // 0x80
};
struct sGfx_D690 {
    char pad_0x0[0xC4];
    float detailScale;              // 0xC4
    char pad_0xC8[0xE84 - 0xC8];
    sRS_D690* top;                  // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sRing_D690* ring;               // 0x18F0
};
extern float D_004A4348;
extern void* D_004A4474;
extern sGfx_D690* D_004A5B80_D690 __asm__("D_004A5B80");
extern char D_1460[];
extern "C" char* func_0038F460(void* self, unsigned int addr, int size, int flags);
extern "C" unsigned int func_0038F668(void* self, char* end, int arg);

static inline void CopyQuads_D690(char** pp, sQuad_D690* src, int n)
{
    sQuad_D690* d = (sQuad_D690*)*pp;
    sQuad_D690* end = d + n;
    while (d != end) {
        *d++ = *src++;
    }
    *pp = (char*)d;
}

// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int.
extern "C" void func_0038D690(void* self, void* obj, void* patch, unsigned int* handle)
{
    char* s = (char*)self;
    char* o = (char*)obj;
    if ((*(int*)(o + 0xC) & 0x800000) == 0) {
        return;
    }
    sVec3_D690 d;
    d.z = *(float*)(o + 0x160) - *(float*)(o + 0x16C);
    sGfx_D690* gfx = D_004A5B80_D690;
    d.x = *(float*)(o + 0x158) - *(float*)(o + 0x164);
    d.y = *(float*)(o + 0x15C) - *(float*)(o + 0x168);
    int n = (int)((d.x * d.x + d.y * d.y + d.z * d.z) * (D_004A4348 * 3.3333333249174757e-07f) * gfx->detailScale);
    if (*(int*)((char*)patch + 8) < 2) {
        n /= 2;
    }
    if (n <= 0) {
        return;
    }
    unsigned int buf = *handle;
    char* p = func_0038F460(D_004A4474, buf, -1, 0);
    ((long*)p)[0] = 0x10000018;
    ((long*)p)[1] = 0;
    ((long*)p)[2] = 0;
    ((long*)p)[3] = (long)0x6C168000 << 32;
    *(int*)(s + 0x43C) = n;
    p += 0x20;
    CopyQuads_D690(&p, (sQuad_D690*)(o + 0x40), 16);
    CopyQuads_D690(&p, (sQuad_D690*)(s + 0x3E0), 4);
    CopyQuads_D690(&p, (sQuad_D690*)(s + 0x420), 2);
    ((long*)p)[0] = (long)(((unsigned int)D_1460 >> 3) | 0x14000000) << 32;
    ((long*)p)[1] = (long)0x11000000 << 32;
    p += 0x10;
    unsigned int next = func_0038F668(D_004A4474, p, 4);
    *handle = next;
    sRing_D690* ring = gfx->ring;
    sRS_D690* rs = gfx->top;
    if (ring->count < 0xA28) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sEnt_D690* e = (sEnt_D690*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        e->rs = *rs;
        e->buf = buf;
        e->next = next;
        e->key = key;
        e->flag = 0;
    }
    *handle += 0x10;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/bezierman", func_0038D968);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sRS_D968 {
    int field_0x0;
    int flagsA;
    int flagsB;
    int field_0xC;
    short tex;
    short pad;
};
struct sCtx_D968 {
    char pad_0x0[0x64];
    int texSel;                                 // 0x64
    char pad_0x68[0xE84 - 0x68];
    sRS_D968* top;                              // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int texIds[(0x6B90 - 0xF50) / 4];           // 0xF50
    int field_0x6B90;                           // 0x6B90
};
struct sBezPatch_D968 {
    void* obj;
    int extra;
    int kind;
    unsigned short tex;
    short pad;
};
struct sBezMixPatch_D968 {
    void* obj;
    int extra;
    int kind;
    unsigned short tex;
    short pad;
    unsigned char types[4];
};
struct sBezMan_D968 {
    char pad_0x0[0x4A60];
    int n8;                         // 0x4A60
    sBezPatch_D968 list8[50];       // 0x4A64
    int nMix;                       // 0x4D84
    sBezMixPatch_D968 mix[75];      // 0x4D88
};
extern int D_004A44E0;
extern sCtx_D968* D_004A5B80_D968 __asm__("D_004A5B80");
extern "C" void func_0037D968(sCtx_D968* ctx);
extern "C" void* func_0038F708(void* obj, int a1, int a2);
extern "C" void func_0038D690(void* self, void* obj, void* patch, unsigned int* handle);

extern "C" void func_0038D968(sBezMan_D968* self, unsigned int* a1)
{
    if (D_004A44E0 != 0) {
        return;
    }
    sCtx_D968* ctx = D_004A5B80_D968;
    ctx->top[1] = ctx->top[0];
    sRS_D968* t = ctx->top;
    ctx->top = t + 1;
    int tex = ctx->texIds[ctx->texSel];
    t[1].tex = tex;
    ctx->top->flagsA = (ctx->top->flagsA & ~0x7C) | 0x54;
    ctx->top->flagsB = (ctx->top->flagsB & ~0x3E0) | 0x80;
    ctx->top->flagsA = (ctx->top->flagsA & 0xFE7FFFFF) | 0x800000;
    ctx->top->flagsA = (ctx->top->flagsA & 0xFFCFFFFF) | 0x300000;
    ctx->top->flagsA = (ctx->top->flagsA & 0xFFF00FFF) | 0x5C000;
    ctx->top->flagsA = (ctx->top->flagsA & 0xFFBFFFFF) | 0x400000;
    if (ctx->field_0x6B90 == 0) {
        func_0037D968(ctx);
    }
    ctx->top->field_0x0 = (ctx->top->field_0x0 & ~0x3C0) | 0x100;
    ctx->top->field_0xC = ctx->top->field_0xC & 0xC0000000;
    ctx->top->flagsA = (ctx->top->flagsA & ~3) | 2;
    ctx->top->flagsB = ctx->top->flagsB & 0xE00003FF;
    for (int i = 0; i < self->n8; i++) {
        sBezPatch_D968* e = &self->list8[i];
        func_0038D690(self, func_0038F708(e->obj, 0x1B0, 2), e, a1);
    }
    for (int i = 0; i < self->nMix; i++) {
        sBezMixPatch_D968* e = &self->mix[i];
        if (e->kind > 0) {
            func_0038D690(self, func_0038F708(e->obj, 0x1B0, 2), e, a1);
        }
    }
    ctx->top--;
}
#endif


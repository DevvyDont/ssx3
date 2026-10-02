#include "common.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

INCLUDE_ASM("ai/rider", cRider_cRider);

INCLUDE_ASM("ai/rider", func_0011B978);

//100%
INCLUDE_ASM("ai/rider", cRider_addFocusBox);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_00457960[];
extern void* D_004A28A8;
extern "C" void func_001033B0(void* self, void* box);

struct sRider_addFocusBox
{
    char pad[0x78C];
    void* mFocusBox;
};

extern "C" void cRider_addFocusBox(sRider_addFocusBox* self)
{
    void* box = cMemMan_alloc(0x10, D_00457960, 0, 0);
    self->mFocusBox = box;
    func_001033B0(*(void**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xA4), box);
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00117248(void* mem);
extern "C" void cRiderMetrix_linkToRider(void* metrix);
extern const char D_00457970[];

struct cRider {
    char pad_0x00[0x790];
    void* field_0x790;
};

//100%
INCLUDE_ASM("ai/rider", cRider_addRiderMetrix__FP6cRider);
#ifdef SKIP_ASM
// PORT: cRiderMetrix_linkToRider really takes (metrix, rider); the unit declares one arg.
extern "C" void cRiderMetrix_linkToRider_impl(void* metrix, cRider* rider) __asm__("cRiderMetrix_linkToRider");

void cRider_addRiderMetrix(cRider* self)
{
    void* mem = cMemMan_alloc(0x1CC, D_00457970, 0, 0);
    void* metrix = func_00117248(mem);
    self->field_0x790 = metrix;
    cRiderMetrix_linkToRider_impl(metrix, self);
}
#endif

INCLUDE_ASM("ai/rider", func_0011BBE8);

//100%
INCLUDE_ASM("ai/rider", func_0011BD60);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" char* strcpy(char* dst, const char* src);
void* func_0011B678(void* self);
extern "C" void func_00418EF8(void* base, int n, int size, void* (*cmp)(void*));

struct sAnimName_11BD60
{
    char name[0x100];
    int f100;
    int f104;
};

struct sAnimNameTable_11BD60
{
    sAnimName_11BD60 e[40];
    int count;      // 0x2940
};

extern "C" void func_0011BD60(void* self, void** list, int n)
{
    for (int i = 0; i < n; i++, list++)
    {
        char* name = *(char**)((char*)*list + 0x28);
        sAnimNameTable_11BD60* t = *(sAnimNameTable_11BD60**)((char*)self + 0x894);
        if (name == 0) continue;
        if (*name == 0) continue;
        int j;
        for (j = 0; j < t->count; j++)
        {
            if (func_004165A8(name, &t->e[j]) == 0) goto next;
        }
        {
            sAnimName_11BD60* e = &t->e[t->count];
            strcpy(e->name, name);
            e->f100 = -1;
            e->f104 = 0;
            t->count++;
        }
    next:;
    }
    sAnimNameTable_11BD60* t2 = *(sAnimNameTable_11BD60**)((char*)self + 0x894);
    func_00418EF8(t2, t2->count, 0x108, func_0011B678);
}
#endif

INCLUDE_ASM("ai/rider", func_0011BE88);

//100%
INCLUDE_ASM("ai/rider", func_0011C0E0);
#ifdef SKIP_ASM
struct sRiderVE_0011C0E0
{
    short delta;
    short index;
    int (*fn)(void*);
};
extern void* D_004A289C;

extern "C" int func_0011C0E0(void* self)
{
    if (*(int*)((char*)self + 0x888) != -1)
    {
        void* g = D_004A289C;
        sRiderVE_0011C0E0* vt = *(sRiderVE_0011C0E0**)((char*)g + 0x10D8);
        return *(int*)((char*)self + 0x888) < vt[114].fn((char*)g + vt[114].delta);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_0011C138);
#ifdef SKIP_ASM
void* func_00314978(void* self);
void func_00314A18(void* self, int a1);
extern "C" void func_00314988(int* self, int flags);
extern "C" void* func_00314C00(void* self, const char* name);
extern "C" int func_0011BBE8(void* self, char** list);
extern "C" void cAnimModel_addModelPartLOD(void* model, int lod, void* part, int a3);

struct sMdfArchive_11C138
{
    int a[4];
};

extern "C" void func_0011C138(void* self, int a1)
{
    sMdfArchive_11C138 arc;
    char* list[52];
    func_00314978(&arc);
    func_00314A18(&arc, a1);
    int n = func_0011BBE8(self, list);
    for (int i = 0; i < n; i++)
    {
        char* item = list[i];
        for (int j = 0; j < 4; j++)
        {
            const char* name = ((const char**)(item + 0x18))[j];
            if (name != 0)
            {
                void* part = func_00314C00(&arc, name);
                if (part != 0)
                {
                    cAnimModel_addModelPartLOD(*(void**)((char*)self + 0x780), j, part, 0);
                }
            }
        }
    }
    func_00314988((int*)&arc, 2);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_initOnce);
#ifdef SKIP_ASM
extern "C" void func_0011C298(void* self);
extern "C" void THREAD_yieldticks(int ticks);

extern "C" void cRider_initOnce(void* self, int immediate)
{
    if (immediate)
    {
        func_0011C298(self);
        return;
    }
    while (*(int*)((char*)self + 0x880) != 7)
    {
        func_0011C298(self);
        THREAD_yieldticks(1);
    }
}
#endif

INCLUDE_ASM("ai/rider", func_0011C298);

INCLUDE_ASM("ai/rider", func_0011D390);

//100%
INCLUDE_ASM("ai/rider", func_0011D640);
#ifdef SKIP_ASM
extern "C" int func_0011D640(void* self)
{
    int s = *(int*)((char*)self + 0x880);
    if (s == 7)
        return 1;
    return s == 7;
}
#endif

INCLUDE_ASM("ai/rider", func_0011D660);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_0011DD98);
#ifdef SKIP_ASM
extern "C" void* func_0026B5E0(void*, int, int);
extern char D_004D33A0[];
extern "C" float func_00115B08(void* self);

struct sVec4_0011DD98
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// The unit declares func_0011D660 later with its own vector type; bind it by asm label.
void func_0011D660_11DD98(void* self, sVec4_0011DD98* a, sVec4_0011DD98* b, int n, float t) __asm__("func_0011D660");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_0011DD98 vu0Scale_11DD98(const sVec4_0011DD98& v, float s)
{
    sVec4_0011DD98 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_11DD98(sVec4_0011DD98& dst, sVec4_0011DD98 b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

extern "C" void func_0011DD98(void* self)
{
    float* p = (float*)func_0026B5E0(D_004D33A0, 2, *(int*)((char*)self + 0x86C));
    sVec4_0011DD98 dir;
    sVec4_0011DD98 pos;
    dir.x = p[5];
    dir.y = p[6];
    dir.z = p[7];
    dir.w = 0.0f;
    pos.x = p[2];
    pos.y = p[3];
    pos.z = p[4];
    pos.w = 1.0f;
    vu0AddEq_11DD98(pos, vu0Scale_11DD98(dir, func_00115B08(self)));
    func_0011D660_11DD98(self, &pos, &dir, 0x11F, 2999.942626953125f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_0011DE60);
#ifdef SKIP_ASM
extern "C" void* func_0026B5E0(void*, int, int);
extern char D_004D33A0[];
extern "C" void func_00119368(void* metrix, int flag);
// PORT: prototype mismatch. The unit defines func_0011FEC8/func_0011FE78 with one
// param, but their bodies forward $5 to func_00111538/func_001112B8 (int arg).
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void func_0011FE78_impl(void* self, int v) __asm__("func_0011FE78__FPv");

struct sVec4_0011DE60
{
    float x, y, z, w;
};

extern "C" void func_0011D660(void* self, sVec4_0011DE60* a, sVec4_0011DE60* b, int n, float t);

extern "C" void func_0011DE60(void* self, int a1, int a2)
{
    float* p = (float*)func_0026B5E0(D_004D33A0, a2, a1);
    sVec4_0011DE60 pos;
    sVec4_0011DE60 dir;
    float px = p[2];
    float py = p[3];
    float pz = p[4];
    pos.x = px;
    pos.y = py;
    pos.z = pz;
    pos.w = 1.0f;
    float dx = p[5];
    float dy = p[6];
    float dz = p[7];
    dir.x = dx;
    dir.y = dy;
    dir.z = dz;
    dir.w = 1.0f;
    func_00119368(*(void**)((char*)self + 0x790), 1);
    func_0011FEC8_impl(self, 0);
    func_0011FE78_impl(self, 0);
    func_0011D660(self, &pos, &dir, 5, 0.0f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011DF18);
#ifdef SKIP_ASM
extern "C" char* func_00311B20(void*, int);
extern "C" void func_00111890(void* self);
extern "C" void func_003099F8(void* self);
extern void* D_004A3DD8;

struct sVec4_0011DF18
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEntryA_0011DF18 { short delta; short index; int (*fn)(void*); };
struct sVEntryB_0011DF18 { short delta; short index; void (*fn)(void*, int); };

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_0011DF18 vu0Scale_11DF18(const sVec4_0011DF18& v, float s)
{
    sVec4_0011DF18 r;
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

extern "C" void func_0011DF18(void* self, int notify)
{
    char* s = (char*)self;
    sVec4_0011DF18 v = *(sVec4_0011DF18*)(s + 0x1B0);
    *(int*)(func_00311B20(*(void**)(s + 0x784), 2) + 0x90) = 0;
    sVec4_0011DF18 t = vu0Scale_11DF18(v, 833.3333740234375f);
    *(sVec4_0011DF18*)(s + 0x1E0) = t;
    *(float*)(s + 0x1E8) = 0.0f;
    func_00111890(*(void**)(s + 0x77C));
    if (notify != 0)
    {
        char* sub = s + 0x6C0;
        sVEntryA_0011DF18* vtA = *(sVEntryA_0011DF18**)sub;
        char* obj = *(char**)(*(char**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + 0x4) + 0xA0);
        sVEntryB_0011DF18* vtB = *(sVEntryB_0011DF18**)(obj + 0x14);
        char* objB = obj + vtB[3].delta;
        vtB[3].fn(objB, vtA[7].fn(sub + vtA[7].delta));
    }
    func_003099F8(D_004A3DD8);
}
#endif

INCLUDE_ASM("ai/rider", func_0011DFE0);

//100%
INCLUDE_ASM("ai/rider", cRider_updateOrientationImplicit);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 macro-mode asm. Normalises the orientation quaternion at
// self+0x120, then builds the rider matrix at self+0x1A0 from it (rows 0-2)
// and the position at self+0x110 (row 3).
extern "C" void cRider_updateOrientationImplicit(char* self)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2       $vf3, 0x120(%0)\n"
        "vaddw.x    $vf6, $vf0, $vf0w\n"
        "vmul.xyzw  $vf4, $vf3, $vf3\n"
        "vadday.x   ACC, $vf4, $vf4y\n"
        "vmaddaz.x  ACC, $vf6, $vf4z\n"
        "vmaddw.x   $vf4, $vf6, $vf4w\n"
        "vrsqrt     Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2       $vf5, 0x120(%0)\n"
        "lqc2       $vf4, 0x120(%0)\n"
        "lqc2       $vf3, 0x110(%0)\n"
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
        "sqc2       $vf3, 0x30(%1)\n"
        "sqc2       $vf10, 0x0(%1)\n"
        "sqc2       $vf11, 0x10(%1)\n"
        "sqc2       $vf12, 0x20(%1)\n"
        ".set pop\n"
        :
        : "r"(self), "r"(self + 0x1A0)
        : "memory");
}
#endif

INCLUDE_ASM("ai/rider", func_0011E150);

//100%
INCLUDE_ASM("ai/rider", func_0011EB60);
#ifdef SKIP_ASM
extern "C" void func_003123C0(void*, int);
extern "C" void func_00312490(void*);

extern "C" void func_0011EB60(void* self, float value)
{
    func_003123C0(*(void**)((char*)self + 0x784), *(int*)((char*)self + 0xB1C));
    func_00312490(*(void**)((char*)self + 0x784));
}
#endif

INCLUDE_ASM("ai/rider", func_0011EB98);

INCLUDE_ASM("ai/rider", func_0011F3D8);

INCLUDE_ASM("ai/rider", cRider_doLeanPoseAdjust);

extern "C" void* func_001112B8(int);

//100%
INCLUDE_ASM("ai/rider", func_0011FE78__FPv);
#ifdef SKIP_ASM
void* func_0011FE78(void* self)
{
    return func_001112B8(*(int*)((char*)self + 0x77c));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011FE98__FPv);
#ifdef SKIP_ASM
int func_0011FE98(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x77c) + 0xde0);
}
#endif

extern "C" void* func_00111538(int);

//100%
INCLUDE_ASM("ai/rider", func_0011FEC8__FPv);
#ifdef SKIP_ASM
void* func_0011FEC8(void* self)
{
    return func_00111538(*(int*)((char*)self + 0x77c));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011FEE8__FPv);
#ifdef SKIP_ASM
int func_0011FEE8(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x77c) + 0xde4);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011FF48);
#ifdef SKIP_ASM
// 0x20-byte elements with the quadword at offset 0
struct sRiderXform {
    cQuad128 q;
    char pad_0x10[0x10];
};

extern "C" void* func_0011FF48(void* dst, void* self)
{
    void* q = *(void**)((char*)self + 0x780);
    int i = *(int*)((char*)self + 0x89c);
    sRiderXform* b = *(sRiderXform**)((char*)q + 0x2c);
    *(cQuad128*)dst = b[i].q;
    return dst;
}
#endif

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBECharacterInterface_getWeight(void* iface, int character);
extern "C" float cBEStatInterface_getCollisionAttrib(void* iface, int character, int stat);

//100%
INCLUDE_ASM("ai/rider", cRider_getMass);
#ifdef SKIP_ASM
extern "C" float cRider_getMass(void* self)
{
    int weight = cBECharacterInterface_getWeight(cBE_getInterface(cBE_getBE(), 2), *(int*)((char*)self + 0x86c));
    float toughness = cBEStatInterface_getCollisionAttrib(cBE_getInterface(cBE_getBE(), 3),
                                                          *(int*)((char*)self + 0x86c), *(int*)((char*)self + 0xb34));

    return (float)weight * (toughness * 1.5003352165222168f + 1.0f) * (*(float*)((char*)self + 0x2fc) * 10.0f + 1.0f);
}
#endif

extern "C" float func_00149690(void* iface, int character, int stat);

// Grab playback speed: 1 + grab stat * 0.2998.
//100%
INCLUDE_ASM("ai/rider", func_00120038);
#ifdef SKIP_ASM
extern "C" float func_00120038(void* self)
{
    void* iface = cBE_getInterface(cBE_getBE(), 3);
    return func_00149690(iface, *(int*)((char*)self + 0x86c), *(int*)((char*)self + 0xb34)) * 0.29988324642181396f + 1.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120090);
#ifdef SKIP_ASM
extern "C" void func_00120090(void* self, float x)
{
    float cur = *(float*)((char*)self + 0x300);
    if (x + 0.00083333341171965f < cur)
        x = cur - 0.00083333341171965f;
    else if (cur < x - 0.00083333341171965f)
        x = cur + 0.00083333341171965f;
    *(float*)((char*)self + 0x300) = x;
}
#endif

INCLUDE_ASM("ai/rider", func_001200D0);

INCLUDE_ASM("ai/rider", func_00120378);

//100%
INCLUDE_ASM("ai/rider", func_00120D58);
#ifdef SKIP_ASM
extern "C" void func_00120D58(void* self)
{
    *(float*)((char*)self + 0x2f8) = *(float*)((char*)self + 0xb24);
    int state = *(int*)((char*)self + 0x304);
    if (state == 3) {
        *(float*)((char*)self + 0x2f8) = 0.0f;
    } else if (state == 0) {
        *(float*)((char*)self + 0x2f8) = 1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120D90);
#ifdef SKIP_ASM
extern "C" int func_00311AE8(void*, int);

static inline float riderApproach_00120D90(float v, float t, float s)
{
    if (v > t + s)
        return v - s;
    if (v < t - s)
        return v + s;
    return t;
}

extern "C" void func_00120D90(void* self)
{
    int a = func_00311AE8(*(void**)((char*)self + 0x784), 2);
    if (a >= 0x14 && a <= 0x1A)
        *(float*)((char*)self + 0x318) = riderApproach_00120D90(*(float*)((char*)self + 0x318), 0.0f, 0.05000000447034836f);
    else
        *(float*)((char*)self + 0x318) = riderApproach_00120D90(*(float*)((char*)self + 0x318), 1.0f, 0.05000000447034836f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120E30);
#ifdef SKIP_ASM
extern "C" void func_00120E30(void* self)
{
    void* p = *(void**)((char*)self + 0x78c);
    if (p != 0) {
        *(cQuad128*)p = *(cQuad128*)((char*)self + 0x110);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120E50);
#ifdef SKIP_ASM
extern "C" void func_00332DB8(void*, void*);
extern "C" void func_00122088(void*);

extern "C" void func_00120E50(void* self)
{
    func_00332DB8(*(void**)((char*)self + 0x860), (char*)self + 0x400);
    func_00122088(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120E88);
#ifdef SKIP_ASM
extern "C" void func_002F1A08(void*);
extern "C" void func_002ECF78(void*);

extern "C" void func_00120E88(void* self)
{
    func_002F1A08(*(void**)((char*)self + 0x88C));
    func_002ECF78(*(void**)((char*)self + 0x890));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120ED8);
#ifdef SKIP_ASM
extern "C" void func_0011EB98(void* self);

extern "C" void func_00120ED8(char* self)
{
    cRider_updateOrientationImplicit(self);
    if (*(int*)(self + 0xAC4) == 0)
    {
        func_0011EB60(self, 1.0f);
        func_0011EB98(self);
    }
}
#endif

INCLUDE_ASM("ai/rider", func_00120F20);

//100%
INCLUDE_ASM("ai/rider", func_00121068);
#ifdef SKIP_ASM
struct sVEntry00121068 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void func_00111728(void* obj, void* v);

extern "C" void func_00121068(void* self)
{
    int buf[4];
    char* obj = *(char**)((char*)self + 0x77C);
    sVEntry00121068* vt = *(sVEntry00121068**)(obj + 0xDE8);
    vt[1].fn(obj + vt[1].delta, buf);
    func_00111728(*(void**)((char*)self + 0x77C), buf);
}
#endif

INCLUDE_ASM("ai/rider", func_001210B0);

INCLUDE_ASM("ai/rider", func_001211F8);

extern "C" void* func_00111408(int);

//100%
INCLUDE_ASM("ai/rider", func_001216E0__FPv);
#ifdef SKIP_ASM
void* func_001216E0(void* self)
{
    return func_00111408(*(int*)((char*)self + 0x77c));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121700);
#ifdef SKIP_ASM
extern "C" void func_0011EB60(void* self, float value);

extern "C" void func_00121700(void* self)
{
    if (*(int*)((char*)self + 0xAC4) == 0)
    {
        func_0011EB60(self, *(float*)((char*)self + 0x300));
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121728);
#ifdef SKIP_ASM
extern "C" void func_0011EB98(void* self);

extern "C" void func_00121728(void* self)
{
    if (*(int*)((char*)self + 0xAC4) == 0)
    {
        func_0011EB98(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121750);
#ifdef SKIP_ASM
extern "C" void func_001114A0(void* p);
extern "C" void func_00310530(void* model, void* v);

struct sVec4_00121750
{
    float x, y, z, w;
};
extern sVec4_00121750 D_004FF120;

static inline int VecChanged_00121750(char* self, const sVec4_00121750& b)
{
    return *(float*)(self + 0x9D0) != b.x || *(float*)(self + 0x9D4) != b.y
        || *(float*)(self + 0x9D8) != b.z || *(float*)(self + 0x9DC) != b.w;
}

extern "C" void func_00121750(char* self)
{
    func_001114A0(*(void**)(self + 0x77C));
    if (VecChanged_00121750(self, D_004FF120))
    {
        func_00310530(*(void**)(self + 0x780), self + 0x9D0);
    }
}
#endif

extern "C" void* func_003103F0(int);

//100%
INCLUDE_ASM("ai/rider", func_001217F8__FPv);
#ifdef SKIP_ASM
void* func_001217F8(void* self)
{
    return func_003103F0(*(int*)((char*)self + 0x780));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121818);
#ifdef SKIP_ASM
extern void* D_004A3DD8;
extern "C" void func_0030A060(void* mgr, void* a, void* b, void* c);
void func_00125AD0(void* self);
extern "C" void func_00112338(void* self);
extern "C" void func_001125C0(void* self);
extern "C" int func_00125228(void* self);
extern "C" void func_00117C28(void* metrix);

struct sVEntry00121818 { short delta; short index; void (*fn)(void*, void*, void*, void*); };

extern "C" void func_00121818(void* self)
{
    char* s = (char*)self;
    char* h = *(char**)(s + 0xA30);
    if (h != 0)
    {
        char* obj = *(char**)(h + 0xC);
        if (obj != 0)
        {
            sVEntry00121818* vt = *(sVEntry00121818**)(obj + 0xC);
            vt[40].fn(obj + vt[40].delta, s + 0xA60, s + 0x9E0, s + 0x6C0);
        }
        else
        {
            func_0030A060(D_004A3DD8, s + 0xA60, s + 0x9E0, s + 0x6C0);
        }
    }
    func_00125AD0(self);
    if (*(int*)(s + 0xAC4) == 0)
    {
        func_00112338(self);
    }
    func_001125C0(self);
    func_00125228(self);
    if (*(int*)(s + 0xAC4) == 0)
    {
        func_00117C28(*(void**)(s + 0x790));
    }
    func_00120E30(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001218D0);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" int func_0022E0E0(void* self, int a1);
extern "C" void func_002ED490(void* obj, float a, float b, float c);
extern "C" void func_00392D18(void* p);

struct sRiderVE_001218D0
{
    short delta;
    short index;
    void* (*fn)(void*);
};

extern "C" void func_001218D0(void* self)
{
    char* s = (char*)self;
    if (~*(int*)(s + 0x430) != 0)
        *(int*)(s + 0x434) = func_0022E0E0(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x78), (unsigned char)*(int*)(s + 0x430));
    char* obj = s + 0x6C0;
    sRiderVE_001218D0* vt = *(sRiderVE_001218D0**)obj;
    func_002ED490(vt[7].fn(obj + vt[7].delta), *(float*)(s + 0x460), *(float*)(s + 0x464), -99999.0f);
    char* p = *(char**)(s + 0x77C);
    func_00392D18(p ? p + 0xD30 : 0);
}
#endif

INCLUDE_ASM("ai/rider", func_00121950);

INCLUDE_ASM("ai/rider", func_00121AA0);

//100%
INCLUDE_ASM("ai/rider", func_00121F30);
#ifdef SKIP_ASM
extern void* D_004A289C;
extern "C" void func_0031BE50(float* s, float* c, float angle);

struct sVec4_121F30
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEntryP_121F30 { short delta; short index; void* (*fn)(void*); };
struct sVEntryF_121F30 { short delta; short index; float (*fn)(void*); };

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline sVec4_121F30 mtxMulVec_121F30(sVec4_121F30* m, sVec4_121F30* v)
{
    sVec4_121F30 r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(*v), "r"(m)
        : "memory");
    return r;
}

extern "C" void func_00121F30(char* self)
{
    void* g = D_004A289C;
    sVEntryP_121F30* gvt = *(sVEntryP_121F30**)((char*)g + 0x10D8);
    sVec4_121F30* m = (sVec4_121F30*)gvt[35].fn((char*)g + gvt[35].delta);
    char* obj = self + 0x6C0;
    sVEntryP_121F30* vt = *(sVEntryP_121F30**)obj;
    sVec4_121F30* pos = (sVec4_121F30*)vt[5].fn(obj + vt[5].delta);
    sVec4_121F30 v = mtxMulVec_121F30(m, pos);
    float z = v.z;
    void* g2 = D_004A289C;
    sVEntryF_121F30* gvt2 = *(sVEntryF_121F30**)((char*)g2 + 0x10D8);
    float fov = gvt2[29].fn((char*)g2 + gvt2[29].delta);
    float s, c;
    func_0031BE50(&s, &c, fov);
    z = z * (s / c);
    z = z * 0.009999999776482582f;
    if (z < 6.0f)
    {
        *(int*)(self + 0x898) = 0;
    }
    else
    {
        int lod = 2;
        if (z < 10.0f) lod = 1;
        *(int*)(self + 0x898) = lod;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122088);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_002F5B68(void* a, void* q, int b, void* c, int d);

extern "C" void func_00122088(void* self)
{
    cQuad128 q;
    func_0011FF48(&q, self);
    func_002F5B68(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x90), &q, *(int*)((char*)self + 0x860), (char*)self + 0x794, 8);
}
#endif

INCLUDE_ASM("ai/rider", func_001220D8);

INCLUDE_ASM("ai/rider", func_00122278);

INCLUDE_ASM("ai/rider", func_00122448);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_001225C0);
#ifdef SKIP_ASM
extern "C" void func_001220D8(void*);

extern "C" int func_001225C0(void* self)
{
    if (*(int*)((char*)self + 0xB18) != 0) {
        func_001220D8(self);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001225F0);
#ifdef SKIP_ASM
extern "C" void func_00122898(void* self);

extern "C" void func_001225F0(void* self)
{
    if (*(int*)((char*)self + 0xB18) != 0)
    {
        func_00122898(self);
    }
}
#endif

extern "C" void* func_002F2088(int);

//100%
INCLUDE_ASM("ai/rider", func_00122638__FPv);
#ifdef SKIP_ASM
void* func_00122638(void* self)
{
    return func_002F2088(*(int*)((char*)self + 0x88c));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122658);
#ifdef SKIP_ASM
// func_003E6448 looks like memset(dst, value, size).
extern "C" void* func_003E6448(void* dst, int value, int size);

extern "C" void func_00122658(void* self)
{
    func_003E6448((char*)self + 0x794, 0, 0x20);
}
#endif

INCLUDE_ASM("ai/rider", func_00122898);

//100%
INCLUDE_ASM("ai/rider", func_00122C28);
#ifdef SKIP_ASM
extern "C" void* func_0026B5E0(void*, int, int);
extern char D_004D33A0[];

extern "C" float* func_00122C28(float* out, void* rider)
{
    float* p = (float*)func_0026B5E0(D_004D33A0, 1, *(int*)((char*)rider + 0x86C));
    out[0] = p[2];
    out[1] = p[3];
    out[2] = p[4];
    out[3] = 1.0f;
    return out;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122C98);
#ifdef SKIP_ASM
extern "C" int func_00122C98(void* self)
{
    float v = *(float*)((char*)self + 0x5B4);
    float k = 0.9250819683074951f;
    return v >= k || v < -k;
}
#endif

extern "C" int func_001231A8(void* self);

//100%
INCLUDE_ASM("ai/rider", func_00122CD0__FPv);
#ifdef SKIP_ASM
void* func_00122CD0(void* self)
{
    return (void*)func_001231A8(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122CF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004A28A8;
extern "C" int func_00101A10(void* self, int character);
extern "C" int func_00122C98(void* self);
float func_0011A0C0_2(void* self, int state) __asm__("func_0011A0C0__FPv");

extern "C" void func_00122CF0(void* self)
{
    char* s = (char*)self;
    int old = *(int*)(s + 0x5B0);
    int r = func_00101A10(*(void**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xA8), *(int*)(s + 0x86C));
    *(int*)(s + 0x5B0) = r;
    if (r == 0)
    {
        if (func_00122CD0(self) != 0)
        {
            if (func_00122C98(self))
                *(int*)(s + 0x5B0) = 1;
        }
    }
    int cur = *(int*)(s + 0x5B0);
    if (old != cur)
        func_0011A0C0_2(*(void**)(s + 0x790), cur);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122D78);
#ifdef SKIP_ASM
float func_00113130(void* self);

struct sVEntry00122D78 { short delta; short index; int (*fn)(void*); };

extern "C" int func_00122D78(void* self)
{
    char* s = (char*)self;
    char* sub = s + 0x6C0;
    int n = *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xC);
    float rate = (func_00113130(self) - *(float*)(s + 0x4D0)) / (float)n;
    sVEntry00122D78* vt = *(sVEntry00122D78**)sub;
    if (rate < 30.0f - (float)vt[7].fn(sub + vt[7].delta))
    {
        sVEntry00122D78* vt2 = *(sVEntry00122D78**)sub;
        rate = 30.0f - (float)vt2[7].fn(sub + vt2[7].delta);
    }
    return n + (int)(*(float*)(s + 0x4D0) / rate);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122E50);
#ifdef SKIP_ASM
float func_00113130(void* self);

extern "C" int func_00122E50(void* self)
{
    int n = *(int*)(*(char**)((char*)self + 0x790) + 0x198) + 1;
    float t = func_00113130(self) - *(float*)((char*)self + 0x4D0);
    if (t < 1.0f)
        t = 1.0f;
    float r = (float)n / t;
    float d = *(float*)((char*)self + 0x4D0) - 1000.0f;
    if (d < 0.0f)
        d = 0.0f;
    return n + (int)(d * r);
}
#endif

INCLUDE_ASM("ai/rider", func_00122EE8);

//100%
INCLUDE_ASM("ai/rider", func_00123128);
#ifdef SKIP_ASM
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int func_00123128(void* self)
{
    return cBENewPlayerInterface_getRiderCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(int*)((char*)self + 0x86C));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123168);
#ifdef SKIP_ASM
extern "C" int func_00147410(void* iface, int riderIndex);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int func_00123168(void* self)
{
    return func_00147410(cBE_getInterface_Fv(cBE_getBE(), 1), *(int*)((char*)self + 0x86C));
}
#endif

// Grounded predicate: motion 0, or motion 2 with owner+0x30 == 0.
//100%
INCLUDE_ASM("ai/rider", func_001231A8);
#ifdef SKIP_ASM
extern "C" int func_001231A8(void* self)
{
    int grounded = 0;

    if (func_0011FE98(self) == 0 ||
        (func_0011FE98(self) == 2 && *(int*)((char*)*(void**)((char*)self + 0x77c) + 0x30) == 0)) {
        grounded = 1;
    }
    return grounded;
}
#endif

INCLUDE_ASM("ai/rider", func_00123210);

INCLUDE_ASM("ai/rider", func_001234D0);

//100%
INCLUDE_ASM("ai/rider", func_001235F8);
#ifdef SKIP_ASM
// PORT: prototype mismatch. The unit defines func_0011FEC8/func_0011FE78 with one
// param, but their bodies forward $5 to func_00111538/func_001112B8 (int arg).
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void func_0011FE78_impl(void* self, int v) __asm__("func_0011FE78__FPv");

extern "C" void func_001235F8(void* self)
{
    *(int*)(*(char**)((char*)self + 0x77C) + 0x350) = 1;
    func_0011FEC8_impl(self, 9);
    func_0011FE78_impl(self, 3);
    *(int*)(*(char**)((char*)self + 0x77C) + 0x290) = 0;
}
#endif

INCLUDE_ASM("ai/rider", func_00123640);

INCLUDE_ASM("ai/rider", func_00123B48);

//100%
INCLUDE_ASM("ai/rider", func_00123DA8);
#ifdef SKIP_ASM
extern "C" void func_0027C9B0(int, void*);
extern "C" void func_00124788(void*, void*, float, float, int);
extern "C" void func_00125448(void*, void*, float, float, int);

extern "C" void func_00123DA8(void* self, int id, float a, float b, int unused, int flags)
{
    int buf[12];
    if (*(int*)((char*)self + 0xAC4))
    {
        func_0027C9B0(id, buf);
        func_00124788(self, buf, a, b, flags);
        func_00125448(self, buf, a, b, flags);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123E30);
#ifdef SKIP_ASM
extern "C" void func_0027C9B0(int, void*);
extern "C" void* func_0028B180();
extern "C" int func_002A1B58(void* self, int msg, int a);
extern "C" int func_002A1B88(void* self, int type, int a, int b);
extern "C" void* func_00279F18(void* self, int i, int a2);
extern void* D_004A28A4;

struct sAnimEvent_123E30
{
    int id;
    signed char* arg;
};

struct sAnimEventInfo_123E30
{
    int pad[4];
    int count;                      // 0x10
    sAnimEvent_123E30* events;      // 0x14
    int pad18[6];
};

extern "C" void func_00123E30(void* self, int id, float a, float b, int unused, int flags)
{
    sAnimEventInfo_123E30 info;
    if (flags & 0x10)
    {
        func_0027C9B0(id, &info);
        for (int i = 0; i < info.count; i++)
        {
            sAnimEvent_123E30* e = &info.events[i];
            int ev = e->id;
            if (ev == 0x66) continue;
            if (ev < 0x64)
            {
                func_002A1B58(func_0028B180(), ev, (int)self);
                *(int*)((char*)self + 0xAC8) = 1;
            }
            else
            {
                ev -= 0x64;
                void* snd = func_0028B180();
                // PORT: id is a handle that is also read as a pointer here.
                int v = (int)func_00279F18(D_004A28A4, *e->arg, *(int*)(id + 8));
                func_002A1B88(snd, ev, (int)self, v);
                *(int*)((char*)self + 0xAC8) = 1;
            }
        }
    }
}
#endif

INCLUDE_ASM("ai/rider", func_00123F38);

//100%
INCLUDE_ASM("ai/rider", func_001241C0);
#ifdef SKIP_ASM
extern "C" void func_0031BE50(float* s, float* c, float angle);

struct sQuat_001241C0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void func_001241C0(void* self, sQuat_001241C0* out, float rx, float ry, float rz)
{
    sQuat_001241C0 q;
    float sz, cz, sy, cy, sx, cx;
    func_0031BE50(&sz, &cz, rz * 0.5f);
    func_0031BE50(&sy, &cy, ry * 0.5f);
    func_0031BE50(&sx, &cx, rx * 0.5f);
    float cc = cy * cx;
    float ss = sy * sx;
    q.w = cz * cc + sz * ss;
    q.x = sz * cc - cz * ss;
    q.y = cz * sy * cx + sz * cy * sx;
    q.z = cz * cy * sx - sz * sy * cx;
    *out = q;
}
#endif

INCLUDE_ASM("ai/rider", func_001242B0);

INCLUDE_ASM("ai/rider", func_00124788);

//100%
INCLUDE_ASM("ai/rider", func_00125038);
#ifdef SKIP_ASM
extern void* D_004A28A8;
void* func_00230698(void* self, int id);
extern "C" void func_002E4578(void* p);

extern "C" void func_00125038(void* self)
{
    void* w = *(void**)((char*)D_004A28A8 + 0x84);
    if (*(void**)((char*)w + 0x84) != 0 && *(int*)((char*)self + 0x870) >= 0)
    {
        if (*(int*)((char*)func_00230698(w, *(int*)((char*)self + 0x870)) + 0x78) == 1)
            func_002E4578(func_00230698(*(void**)((char*)D_004A28A8 + 0x84), *(int*)((char*)self + 0x870)));
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001250A8);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);

struct sRiderVec4_001250A8
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (a += b).
static inline void riderAddEq_001250A8(sRiderVec4_001250A8& a, const sRiderVec4_001250A8& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b));
}

extern "C" void func_001250A8(void* self, sRiderVec4_001250A8* v)
{
    sRiderVec4_001250A8* p = (sRiderVec4_001250A8*)((char*)self + 0x1E0);
    riderAddEq_001250A8(*p, *v);
    if (func_0011FE98(self) == 1)
        cAirPredictor_startLaunchIntoAir(*(void**)((char*)self + 0x788), (char*)self + 0x110, p, 3333.33349609375f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125108);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern char* D_004A2EEC;
extern "C" float func_001193E0(void* self);
extern "C" void func_00162258(void* self);
extern "C" void func_00238358(void* self, int a1, int a2);
extern "C" void func_00286EA0(void* self, void* rider);
extern "C" void* func_0028B180();

struct sVEntry125108 { short delta; short index; int (*fn)(void*); };

static inline void* getC0_125108()
{
    return *(void**)((char*)D_004A28A8 + 0xC0);
}

extern "C" void func_00125108(char* self)
{
    char* g = D_004A2EEC;
    if (g != 0 && *(int*)(self + 0x480) == 0 && *(int*)(self + 0x874) != 0)
    {
        if (*(int*)(self + 0x87C) != 0)
            *(int*)(g + 0xA0) = 1;
        else
            *(int*)(g + 0xA4) = 1;
    }
    func_001193E0(*(void**)(self + 0x790));
    *(int*)(self + 0x478) = *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xC) + *(int*)(self + 0x47C);
    if (*(float*)(self + 0x470) < 0.0f) *(float*)(self + 0x470) = 0.0f;
    char* obj = self + 0x6C0;
    sVEntry125108* vt = *(sVEntry125108**)obj;
    func_00238358(getC0_125108(), vt[7].fn(obj + vt[7].delta), *(int*)(self + 0x790) + 0xFC);
    unsigned int k = *(unsigned int*)(self + 0x870);
    if (k < 2)
    {
        char* tbl = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84);
        func_00162258(*(void**)(*(char**)(tbl + (k << 2) + 4) + 0xA8));
    }
    func_00286EA0(func_0028B180(), self);
    if (*(int*)(self + 0x480) == 1) *(int*)(self + 0x100) = 0;
}
#endif

INCLUDE_ASM("ai/rider", func_00125228);

//100%
INCLUDE_ASM("ai/rider", cRider_quitEvent);
#ifdef SKIP_ASM
struct sVEntry001253D0 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern int D_005366D0[];

static inline bool riderHasFinished001253D0(char* rider)
{
    return *(float*)(rider + 0x470) >= 0.0f;
}

extern "C" void cRider_quitEvent(char* self)
{
    bool finished = riderHasFinished001253D0(self);
    if (!finished)
    {
        *(int*)(self + 0x480) = 1;
        char* sub = self + 0x6C0;
        sVEntry001253D0* vt = *(sVEntry001253D0**)sub;
        D_005366D0[vt[7].fn(sub + vt[7].delta)] = 1;
    }
}
#endif

INCLUDE_ASM("ai/rider", func_00125448);

//100%
INCLUDE_ASM("ai/rider", func_00125958);
#ifdef SKIP_ASM
extern "C" void func_00125958(void* self, int a1)
{
    *(int*)((char*)self + 0xb2c) = a1;
    if (a1 == 0) {
        *(int*)((char*)self + 0x2f4) = 0;
        *(int*)((char*)self + 0x2f0) = 0;
    }
}
#endif

INCLUDE_ASM("ai/rider", func_00125970);

//100%
INCLUDE_ASM("ai/rider", func_00125AD0__FPv);
#ifdef SKIP_ASM
void func_00125AD0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125AD8);
#ifdef SKIP_ASM
extern "C" int func_00125AD8(void* self)
{
    union { float f; int i; } u;
    u.f = *(float*)((char*)self + 0x110) + *(float*)((char*)self + 0x114) + *(float*)((char*)self + 0x118)
        + (*(float*)((char*)self + 0x120) + *(float*)((char*)self + 0x124) + *(float*)((char*)self + 0x128) + *(float*)((char*)self + 0x12C));
    return u.i;
}
#endif

INCLUDE_ASM("ai/rider", func_00125B18);

INCLUDE_ASM("ai/rider", func_00125C70);

INCLUDE_ASM("ai/rider", func_00125EB8);

INCLUDE_ASM("ai/rider", func_001276F0);

//100%
INCLUDE_ASM("ai/rider", func_00127848);
#ifdef SKIP_ASM
int func_00320C48(void*, int);

extern "C" int func_00127848(void* self)
{
    if (func_00320C48(*(void**)((char*)self + 0xDF0), 0x1B))
        return 0;
    if (func_00320C48(*(void**)((char*)self + 0xDF0), 0x1C))
        return 1;
    if (func_00320C48(*(void**)((char*)self + 0xDF0), 0x1D))
        return 2;
    if (func_00320C48(*(void**)((char*)self + 0xDF0), 0x1E))
        return 3;
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001278C0__FPv);
#ifdef SKIP_ASM
void func_001278C0(void* self)
{
    *(float*)((char*)self + 0xE00) = 0.0f;
    *(float*)((char*)self + 0xDFC) = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001278D0);
#ifdef SKIP_ASM
extern "C" void func_001278D0(void* self, float val)
{
    *(float*)((char*)self + 0xDFC) = val >? *(float*)((char*)self + 0xDFC);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001278E0__FPvf);
#ifdef SKIP_ASM
void func_001278E0(void* self, float val)
{
    *(float*)((char*)self + 0xE00) = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_001278E8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_001474E8(void* iface, int id);
extern "C" void func_00125B18(void* p, int a, int b);

struct sVEntry001278E8 { short delta; short index; int (*fn)(void*); };

static inline bool riderInRange_1278E8(int v)
{
    return v != 0 && v < 10;
}

extern "C" void func_001278E8(void* self)
{
    char* s = (char*)self;
    if (*(int*)(s + 0xDF4) != 0)
    {
        int flag = 0;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        char* sub = *(char**)(s + 0x18) + 0x6C0;
        sVEntry001278E8* vt = *(sVEntry001278E8**)sub;
        if (func_001474E8(iface, vt[7].fn(sub + vt[7].delta)) != 0)
        {
            flag = !riderInRange_1278E8(**(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28));
        }
        func_00125B18(s + 0xDFC, *(int*)(s + 0xDF4), flag);
    }
}
#endif

INCLUDE_ASM("ai/rider", func_00127998);

extern "C" void* func_00111AC0(void* self);

//100%
INCLUDE_ASM("ai/rider", func_00128660__FPv);
#ifdef SKIP_ASM
void* func_00128660(void* self)
{
    return func_00111AC0(self);
}
#endif

extern "C" void* func_00111D98(void* self);

//100%
INCLUDE_ASM("ai/rider", func_00128680__FPv);
#ifdef SKIP_ASM
void* func_00128680(void* self)
{
    return func_00111D98(self);
}
#endif


#include "common.h"

struct cEffectLink {
    cEffectLink* next; // 0x0
    cEffectLink* prev; // 0x4
};

//100%
INCLUDE_ASM("object/effectlink", cEffectLink_add__FP11cEffectLinkT0);
#ifdef SKIP_ASM
void cEffectLink_add(cEffectLink* link, cEffectLink* other)
{
    if (link->next != 0) {
        link->next->prev = other;
    }
    other->prev = link;
    other->next = link->next;
    link->next = other;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345720);
#ifdef SKIP_ASM
extern "C" void func_00345720(cEffectLink* link, cEffectLink* other)
{
    while (link->next != 0) {
        link = link->next;
    }
    other->next = 0;
    other->prev = link;
    link->next = other;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345760);
#ifdef SKIP_ASM
extern "C" void func_00345760(cEffectLink* link)
{
    if (link->next != 0) {
        link->next->prev = link->prev;
    }
    if (link->prev != 0) {
        link->prev->next = link->next;
    }
    link->next = 0;
    link->prev = 0;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345798);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* D_004912F8[];

struct sEffectLinkObj_5798 {
    cEffectLink link;  // 0x0
    void** vt;         // 0x8
    int value;         // 0xC
};

extern "C" void* func_00345798(void* self, int a1)
{
    sEffectLinkObj_5798* o = (sEffectLinkObj_5798*)self;
    o->link.next = 0;
    o->link.prev = 0;
    o->vt = D_004912F8;
    o->value = a1;
    D_004A3FF4++;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_003457C8);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* D_004912F8[];

struct sEffectLinkObj_57C8 {
    cEffectLink link;  // 0x0
    void** vt;         // 0x8
    int value;         // 0xC
};

struct sEffVEntry57C8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_003457C8(void* p, void* obj)
{
    sEffectLinkObj_57C8* self = (sEffectLinkObj_57C8*)p;
    self->link.next = 0;
    self->link.prev = 0;
    self->vt = D_004912F8;
    sEffVEntry57C8* vt = *(sEffVEntry57C8**)obj;
    self->value = vt[3].fn((char*)obj + vt[3].delta);
    D_004A3FF4++;
    return p;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345828);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* D_004912F8[];
extern void* D_00491340[];
void operator_delete(int*);

struct sEffectLinkObj_5828 {
    cEffectLink link;  // 0x0
    void** vt;         // 0x8
    int value;         // 0xC
};

extern "C" void func_00345828(void* self, int flags)
{
    sEffectLinkObj_5828* o = (sEffectLinkObj_5828*)self;
    o->vt = D_004912F8;
    D_004A3FF4--;
    o->vt = D_00491340;
    func_00345760(&o->link);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345890);
#ifdef SKIP_ASM
struct sEffVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void func_00345890(void* self, void* obj)
{
    sEffVEntry* vt = *(sEffVEntry**)obj;
    vt[5].fn((char*)obj + vt[5].delta, *(void**)((char*)self + 0xC));
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_003458C0);
#ifdef SKIP_ASM
extern "C" void* func_00345798(void* self, int a1);
extern "C" void* func_00370018(void* self);
extern "C" void func_003705E0(void* self, void* mat, int a2);
extern int D_004A3FF8;
extern void* D_004912B0[];

struct sVEntry003458C0 {
    short delta;
    short index;
    void* (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (4x4 matrix copy); returns dst.
static inline void* vu0CopyMatrix003458C0(void* dst, void* src)
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
        : "r"(dst), "r"(src)
        : "memory");
    return dst;
}

struct sEffObj003458C0 {
    char pad0[0x1E0];
    int f1E0;
    int f1E4;
};

extern "C" void* func_003458C0(void* self, int a1, int owner)
{
    char* s = (char*)self;
    func_00345798(self, owner);
    *(void***)(s + 0x8) = D_004912B0;
    func_00370018(s + 0x50);
    *(int*)(s + 0x1E0) = a1;
    char* own = *(char**)(s + 0xC);
    void* obj = *(void**)(own + 0xC);
    void* m;
    if (obj != 0) {
        sVEntry003458C0* vt = *(sVEntry003458C0**)((char*)obj + 0xC);
        m = vu0CopyMatrix003458C0(s + 0x10, vt[24].fn((char*)obj + vt[24].delta));
    } else {
        m = vu0CopyMatrix003458C0(s + 0x10, own + 0x10);
    }
    func_003705E0(s + 0x50, m, *(int*)(s + 0x1E0));
    ((sEffObj003458C0*)self)->f1E4 = 0;
    D_004A3FF8++;
    return self;
}
#endif

INCLUDE_ASM("object/effectlink", func_003459A8);

//100%
INCLUDE_ASM("object/effectlink", func_00345AD0);
#ifdef SKIP_ASM
extern int D_004A3FF8;
extern void* D_004912B0[];
void operator_delete(int*);
extern "C" void func_00370758(int* self, int flags);
extern "C" void func_00345828(void* self, int flags);

extern "C" void func_00345AD0(void* self, int flags)
{
    char* s = (char*)self;
    *(void***)(s + 0x8) = D_004912B0;
    int* buf = *(int**)(s + 0x1E0);
    if (buf != 0) {
        operator_delete(buf);
    }
    D_004A3FF8--;
    func_00370758((int*)(s + 0x50), 2);
    func_00345828(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345B40);
#ifdef SKIP_ASM
extern "C" void func_00370788(void* self, float dt);

extern "C" void func_00345B40(void* self)
{
    char* s = (char*)self;
    if (*(int*)(s + 0x1E4) == 0) {
        void* t = s + 0x50;
        if (*(float*)(s + 0x50) < 0.0f) {
            func_00370788(t, 0.01666666753590107f);
        } else {
            func_00370788(t, 0.01666666753590107f);
            if (*(float*)(s + 0x50) <= 0.0f) {
                *(int*)(s + 0x1E4) = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345BC8);
#ifdef SKIP_ASM
extern "C" void func_003708C0(void*);

extern "C" void func_00345BC8(void* self)
{
    if (*(int*)((char*)self + 0x1E4) == 0) {
        func_003708C0((char*)self + 0x50);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", func_00345BF0);
#ifdef SKIP_ASM
struct sEffectLinkVEntry5BF0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00370AA8(void* self, void* stream);

extern "C" void func_00345BF0(void* self, void* stream)
{
    func_00345890(self, stream);
    sEffectLinkVEntry5BF0* e = &(*(sEffectLinkVEntry5BF0**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x1E4, 4);
    if (*(int*)((char*)self + 0x1E4) == 0) {
        e = &(*(sEffectLinkVEntry5BF0**)stream)[1];
        e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x1E0), 0xD8);
        e = &(*(sEffectLinkVEntry5BF0**)stream)[1];
        e->fn((char*)stream + e->delta, (char*)self + 0x10, 0x40);
        func_00370AA8((char*)self + 0x50, stream);
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345C90);
#ifdef SKIP_ASM
extern "C" void* func_00345798(void* self, int a1);
extern "C" void* func_00370B60(void* self);
extern "C" void func_00370DC8(void* self, void* target, float t);
struct sEffectLink5EF8;
extern "C" void func_00345EF8(sEffectLink5EF8* self);
extern int D_004A3FFC;
extern void* D_00491268[];
extern void* D_00491370[];

struct sVec4_00345C90 {
    float x, y, z, w;
    sVec4_00345C90(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
} __attribute__((aligned(16)));

struct sTarget00345C90 {
    char pad0[0x48];
    float x;
    float y;
    float z;
};

struct sEff00345C90 {
    void* next;
    void* prev;
    void** vt;
    void* owner;
    int bone;
    int f14;
    int f18;
    int f1C;
    sVec4_00345C90 localA;
    sVec4_00345C90 worldA;
    sVec4_00345C90 worldB;
    sVec4_00345C90 localB;
    char sub60[0x258 - 0x60];
    void** subVt;
    int f25C;
    sTarget00345C90* target;
};

extern "C" sEff00345C90* func_00345C90(sEff00345C90* self, sTarget00345C90* t, int owner, int bone, sVec4_00345C90* pos)
{
    func_00345798(self, owner);
    self->vt = D_00491268;
    func_00370B60(self->sub60);
    self->subVt = D_00491370;
    self->target = t;
    self->bone = bone;
    self->localA = *pos;
    self->localB = sVec4_00345C90(t->x, t->y, t->z, 0.0f);
    self->f14 = 0;
    t->x = 0.0f;
    t->y = 0.0f;
    t->z = 0.0f;
    func_00370DC8(self->sub60, self->target, -1.0f);
    func_00345EF8((sEffectLink5EF8*)self);
    D_004A3FFC++;
    self->f18 = 0;
    return self;
}
#endif

INCLUDE_ASM("object/effectlink", func_00345D80);

//100%
INCLUDE_ASM("object/effectlink", func_00345E88);
#ifdef SKIP_ASM
extern int D_004A3FFC;
extern void* D_00491268[];
void operator_delete(int*);
extern "C" void func_00370C08(void* self, int flags);
extern "C" void func_00345828(void* self, int flags);

extern "C" void func_00345E88(void* self, int flags)
{
    char* s = (char*)self;
    *(void***)(s + 0x8) = D_00491268;
    int* buf = *(int**)(s + 0x260);
    if (buf != 0) {
        operator_delete(buf);
    }
    D_004A3FFC--;
    func_00370C08(s + 0x60, 2);
    func_00345828(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345EF8);
#ifdef SKIP_ASM
struct sEffVec4_5EF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sEffMtx_5EF8 {
    sEffVec4_5EF8 r[4];
};

struct sEffMtx_62A0;
extern "C" void func_0034FED8(void* a, int b, sEffMtx_62A0* out);

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (matrix * vector).
static inline sEffVec4_5EF8 effMtxApply5EF8(sEffMtx_5EF8* m, sEffVec4_5EF8* v)
{
    sEffVec4_5EF8 out;
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
        : "m"(*v), "r"(m)
        : "memory");
    return out;
}

struct sEffectLink5EF8 {
    char pad_0x0[0xC];
    void* model;            // 0xC
    int bone;               // 0x10
    char pad_0x14[0xC];
    sEffVec4_5EF8 localA;   // 0x20
    sEffVec4_5EF8 worldA;   // 0x30
    sEffVec4_5EF8 worldB;   // 0x40
    sEffVec4_5EF8 localB;   // 0x50
};

extern "C" void func_00345EF8(sEffectLink5EF8* self)
{
    sEffMtx_5EF8 m;
    func_0034FED8(self->model, self->bone, (sEffMtx_62A0*)&m);
    self->worldA = effMtxApply5EF8(&m, &self->localA);
    self->worldB = effMtxApply5EF8(&m, &self->localB);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345F90);
#ifdef SKIP_ASM
struct sEffectLink5EF8;
extern "C" void func_00345EF8(sEffectLink5EF8* self);
extern "C" void func_003710D0(void* sub, void* a, void* b, int flag, float dt);

extern "C" void func_00345F90(void* self)
{
    char* s = (char*)self;
    if (*(int*)(s + 0x18) != 0) {
        return;
    }
    func_00345EF8((sEffectLink5EF8*)self);
    if (*(int*)(s + 0x14) > 0) {
        float* sub = (float*)(s + 0x60);
        if (*(int*)(s + 0x240) > 0) {
            func_003710D0(sub, s + 0x30, s + 0x40, 0, 0.01666666753590107f);
            return;
        }
    } else {
        float* sub = (float*)(s + 0x60);
        if (*(float*)(s + 0x60) < 0.0f) {
            func_003710D0(sub, s + 0x30, s + 0x40, 1, 0.01666666753590107f);
            return;
        }
        func_003710D0(sub, s + 0x30, s + 0x40, 1, 0.01666666753590107f);
        if (!(*(float*)(s + 0x60) <= 0.0f)) {
            return;
        }
    }
    *(int*)(s + 0x18) = 1;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00346060__FPv);
#ifdef SKIP_ASM
void func_00346060(void* self)
{
    *(int*)((char*)self + 0x14) = 1;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00346070);
#ifdef SKIP_ASM
extern "C" void func_00371380(void*, int);

extern "C" void func_00346070(void* self)
{
    if (*(int*)((char*)self + 0x18) == 0) {
        func_00371380((char*)self + 0x60, 7);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", func_003460A0);
#ifdef SKIP_ASM
struct sEffectLinkVEntry60A0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_003712B8(void* self, void* stream);

extern "C" void func_003460A0(void* self, void* stream)
{
    func_00345890(self, stream);
    sEffectLinkVEntry60A0* e = &(*(sEffectLinkVEntry60A0**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 0x50);
    if (*(int*)((char*)self + 0x18) == 0) {
        e = &(*(sEffectLinkVEntry60A0**)stream)[1];
        e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x260), 0xD8);
        func_003712B8((char*)self + 0x60, stream);
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00346120);
#ifdef SKIP_ASM
extern int D_004A4000;
extern void* D_00491220[];
extern "C" void* func_00345798(void* self, int a1);

struct sEffQuat_6120 {
    float x, y, z, w;
    sEffQuat_6120(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
} __attribute__((aligned(16)));

struct sEffObj_6120 {
    cEffectLink link;   // 0x0
    void** vt;          // 0x8
    int value;          // 0xC
    sEffQuat_6120 q;    // 0x10
    float f20;          // 0x20
    int f24;            // 0x24
    int f28;            // 0x28
    int f2C;            // 0x2C
    int f30;            // 0x30
};

struct sEffSrc_6120 {
    int f0;     // 0x0
    int f4;     // 0x4
    int f8;     // 0x8
    float w;    // 0xC
    float x;    // 0x10
    float y;    // 0x14
    float z;    // 0x18
    float f1C;  // 0x1C
    int f20;    // 0x20
};

extern "C" void* func_00346120(sEffObj_6120* self, sEffSrc_6120* src, int a2)
{
    func_00345798(self, a2);
    self->vt = D_00491220;
    self->f24 = src->f4;
    self->f28 = src->f8;
    self->q = sEffQuat_6120(src->x, src->y, src->z, src->w);
    self->f20 = src->f1C;
    self->f2C = src->f20;
    self->f30 = 0;
    D_004A4000++;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_003461C0);
#ifdef SKIP_ASM
extern int D_004A4000;
extern void* D_00491220[];
extern "C" void* func_003457C8(void* p, void* obj);

struct sEffVEntry61C0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_003461C0(void* self, void* stream)
{
    func_003457C8(self, stream);
    *(void***)((char*)self + 0x8) = D_00491220;
    sEffVEntry61C0* vt = *(sEffVEntry61C0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x10, 0x30);
    D_004A4000++;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00346228);
#ifdef SKIP_ASM
extern int D_004A4000;
extern void* D_00491220[];
extern "C" void func_00345828(void* self, int flags);

struct sEffectLinkObj_6228 {
    cEffectLink link;  // 0x0
    void** vt;         // 0x8
};

extern "C" void func_00346228(void* self, int flags)
{
    ((sEffectLinkObj_6228*)self)->vt = D_00491220;
    D_004A4000--;
    func_00345828(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00346258);
#ifdef SKIP_ASM
// Advance a rotation angle (degrees) by an integer step, wrapping to 0 at +/-360.
extern "C" void func_00346258(void* self)
{
    float a = *(float*)((char*)self + 0x30) + (float)*(int*)((char*)self + 0x2c);
    *(float*)((char*)self + 0x30) = a;
    if (a >= 360.0f || a <= -360.0f) {
        *(float*)((char*)self + 0x30) = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_003462A0);
#ifdef SKIP_ASM
struct sEffVec4_62A0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sEffMtx_62A0 {
    sEffVec4_62A0 r[4];
};

extern "C" void func_0034FED8(void* a, int b, sEffMtx_62A0* out);
extern "C" void func_002D1D10(sEffVec4_62A0* pos, void* p, int n, float a, float b);

extern "C" void func_003462A0(void* self)
{
    sEffMtx_62A0 m;
    void* p = (char*)self + 0x10;
    func_0034FED8(*(void**)((char*)self + 0xC), *(int*)((char*)self + 0x28), &m);
    sEffVec4_62A0 v = m.r[3];
    func_002D1D10(&v, p, *(int*)((char*)self + 0x24),
                  *(float*)((char*)self + 0x20), *(float*)((char*)self + 0x30));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", func_00346300);
#ifdef SKIP_ASM
struct sEffectLinkVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00345890(void* self, void* stream);

extern "C" void func_00346300(void* self, void* stream)
{
    func_00345890(self, stream);
    sEffectLinkVEntry* vt = *(sEffectLinkVEntry**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x10, 0x30);
}
#endif

INCLUDE_ASM("object/effectlink", func_00346350);

//100%
INCLUDE_ASM("object/effectlink", func_003464E0);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern "C" void* func_00353E80(void* self, void* stream);
extern "C" void cFlexBridgeNode_setupGrid(void* self);
extern "C" void func_003475D8(void*);
extern void* D_00490270[];

struct sEffectLinkVEntry64E0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_003464E0(void* self, void* a1, void* stream)
{
    void* p = (char*)self + 0x20;
    cInstanceNode_cInstanceNode(self, a1, stream);
    *(void***)((char*)self + 0xC) = D_00490270;
    func_00353E80((char*)self + 0x50, stream);
    cFlexBridgeNode_setupGrid(self);
    sEffectLinkVEntry64E0* e = &(*(sEffectLinkVEntry64E0**)stream)[2];
    e->fn((char*)stream + e->delta, p, 0x30);
    func_003475D8(self);
    return self;
}
#endif

INCLUDE_ASM("object/effectlink", func_00346568);


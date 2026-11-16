#include "common.h"
#include "render/ps2graphicsman.h"
#include "object/effectlink.h"

struct sEffectLinkView {  // this unit's view of cEffectLink (object/effectlink.h); typed members schedule differently
    sEffectLinkView* next; // 0x0
    sEffectLinkView* prev; // 0x4
};

//100%
INCLUDE_ASM("object/effectlink", cEffectLink_add__FP11cEffectLinkT0);
#ifdef SKIP_ASM
/**
 * Inserts an effect link into the chain directly after another.
 *
 * @param link   link to insert after
 * @param other  link to insert
 */
void cEffectLink_add(cEffectLink* link, cEffectLink* other)
{
    if (link->Next != 0) {
        link->Next->Prev = other;
    }
    other->Prev = link;
    other->Next = link->Next;
    link->Next = other;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345720);
#ifdef SKIP_ASM
extern "C" void func_00345720(cEffectLink* link, cEffectLink* other)
{
    while (link->Next != 0) {
        link = link->Next;
    }
    other->Next = 0;
    other->Prev = link;
    link->Next = other;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345760);
#ifdef SKIP_ASM
extern "C" void func_00345760(cEffectLink* link)
{
    if (link->Next != 0) {
        link->Next->Prev = link->Prev;
    }
    if (link->Prev != 0) {
        link->Prev->Next = link->Next;
    }
    link->Next = 0;
    link->Prev = 0;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345798);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* cEffectModifier_vtbl[];

struct sEffectLinkObj_5798 {
    sEffectLinkView link;  // 0x0
    void** vt;         // 0x8
    int value;         // 0xC
};

extern "C" void* func_00345798(void* self, int a1)
{
    sEffectLinkObj_5798* o = (sEffectLinkObj_5798*)self;
    o->link.next = 0;
    o->link.prev = 0;
    o->vt = cEffectModifier_vtbl;
    o->value = a1;
    D_004A3FF4++;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_003457C8);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* cEffectModifier_vtbl[];

struct sEffectLinkObj_57C8 {
    sEffectLinkView link;  // 0x0
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
    self->vt = cEffectModifier_vtbl;
    sEffVEntry57C8* vt = *(sEffVEntry57C8**)obj;
    self->value = vt[3].fn((char*)obj + vt[3].delta);
    D_004A3FF4++;
    return p;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cEffectModifier__cEffectModifier);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* cEffectModifier_vtbl[];
extern void* cEffectLink_vtbl[];
void operator_delete(int*);

struct sEffectLinkObj_5828 {
    sEffectLinkView link;  // 0x0
    void** vt;         // 0x8
    int value;         // 0xC
};

/**
 * Destroys an effect modifier: decrements the live count (D_004A3FF4), unlinks it from its effect chain, then
 * frees it when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cEffectModifier__cEffectModifier(void* self, int flags)
{
    sEffectLinkObj_5828* o = (sEffectLinkObj_5828*)self;
    o->vt = cEffectModifier_vtbl;
    D_004A3FF4--;
    o->vt = cEffectLink_vtbl;
    func_00345760((cEffectLink*)&o->link);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cEffectModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sEffVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

/**
 * Writes an effect modifier's value (offset 0xC) to a replay frame.
 *
 * Passes the value to the stream's slot-5 virtual; the stream constructor reads it back through slot 3.
 *
 * @param obj  replay stream to write to
 */
extern "C" void cEffectModifier_writeToReplayFrame(void* self, void* obj)
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
extern void* cParticleModifier_vtbl[];

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
    *(void***)(s + 0x8) = cParticleModifier_vtbl;
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

//100%
INCLUDE_ASM("object/effectlink", func_003459A8);
#ifdef SKIP_ASM
void* operator_new1(int size);
extern "C" void func_00370AF8(void* self, void* stream);
extern char t4mat_Identity[];

struct sEffectLinkVEntry_3459A8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_003459A8(void* self, void* stream)
{
    char* s = (char*)self;
    func_003457C8(self, stream);
    *(void***)(s + 0x8) = cParticleModifier_vtbl;
    func_00370018(s + 0x50);
    sEffectLinkVEntry_3459A8* e = &(*(sEffectLinkVEntry_3459A8**)stream)[2];
    e->fn((char*)stream + e->delta, s + 0x1E4, 4);
    if (*(int*)(s + 0x1E4) == 0) {
        void* buf = operator_new1(0xD8);
        *(void**)(s + 0x1E0) = buf;
        e = &(*(sEffectLinkVEntry_3459A8**)stream)[2];
        e->fn((char*)stream + e->delta, buf, 0xD8);
        e = &(*(sEffectLinkVEntry_3459A8**)stream)[2];
        e->fn((char*)stream + e->delta, s + 0x10, 0x40);
        func_003705E0(s + 0x50, s + 0x10, *(int*)(s + 0x1E0));
        func_00370AF8(s + 0x50, stream);
    } else {
        vu0CopyMatrix003458C0(s + 0x10, t4mat_Identity);
        *(int*)(s + 0x1E0) = 0;
    }
    D_004A3FF8++;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cParticleModifier__cParticleModifier);
#ifdef SKIP_ASM
extern int D_004A3FF8;
extern void* cParticleModifier_vtbl[];
void operator_delete(int*);
extern "C" void func_00370758(int* self, int flags);
extern "C" void cEffectModifier__cEffectModifier(void* self, int flags);

/**
 * Destroys a particle modifier: frees its 0xD8-byte buffer (+0x1E0), decrements the live count (D_004A3FF8),
 * destroys the embedded static emitter (+0x50), then the cEffectModifier part.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cParticleModifier__cParticleModifier(void* self, int flags)
{
    char* s = (char*)self;
    *(void***)(s + 0x8) = cParticleModifier_vtbl;
    int* buf = *(int**)(s + 0x1E0);
    if (buf != 0) {
        operator_delete(buf);
    }
    D_004A3FF8--;
    func_00370758((int*)(s + 0x50), 2);
    cEffectModifier__cEffectModifier(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cParticleModifier_update);
#ifdef SKIP_ASM
extern "C" void cStaticEmitter_update(void* self, float dt);

/**
 * Advances a particle modifier's static emitter by one 1/60 s step until the effect has finished.
 *
 * An emitter whose life (+0x50) starts negative runs forever; otherwise the effect is marked finished
 * (+0x1E4) once the life has run down to 0.
 */
extern "C" void cParticleModifier_update(void* self)
{
    char* selfBytes = (char*)self;
    if (*(int*)(selfBytes + 0x1E4) == 0) {
        void* emitter = selfBytes + 0x50;
        if (*(float*)(selfBytes + 0x50) < 0.0f) {
            cStaticEmitter_update(emitter, 0.01666666753590107f);
        } else {
            cStaticEmitter_update(emitter, 0.01666666753590107f);
            if (*(float*)(selfBytes + 0x50) <= 0.0f) {
                *(int*)(selfBytes + 0x1E4) = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cParticleModifier_render);
#ifdef SKIP_ASM
extern "C" void cStaticEmitter_render1(void*);

/**
 * Renders the particle modifier's static emitter unless the effect has finished.
 *
 * Calls cStaticEmitter_render1() on the emitter embedded at offset 0x50 while the finished flag (offset 0x1E4,
 * set by cParticleModifier_update when the emitter's time runs out) is clear.
 */
extern "C" void cParticleModifier_render(void* self)
{
    if (*(int*)((char*)self + 0x1E4) == 0) {
        cStaticEmitter_render1((char*)self + 0x50);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", cParticleModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sEffectLinkVEntry5BF0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cStaticEmitter_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a particle modifier's state to a replay frame.
 *
 * Writes the cEffectModifier part and the finished flag (+0x1E4); while not finished, also the 0xD8-byte
 * buffer (+0x1E0), the 0x40 bytes at +0x10 and the static emitter's own state.
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cParticleModifier_writeToReplayFrame(void* self, void* stream)
{
    cEffectModifier_writeToReplayFrame(self, stream);
    sEffectLinkVEntry5BF0* writeEntry = &(*(sEffectLinkVEntry5BF0**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x1E4, 4);
    if (*(int*)((char*)self + 0x1E4) == 0) {
        writeEntry = &(*(sEffectLinkVEntry5BF0**)stream)[1];
        writeEntry->fn((char*)stream + writeEntry->delta, *(void**)((char*)self + 0x1E0), 0xD8);
        writeEntry = &(*(sEffectLinkVEntry5BF0**)stream)[1];
        writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x10, 0x40);
        cStaticEmitter_writeToReplayFrame((char*)self + 0x50, stream);
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
extern void* cDynamicParticleModifier_vtbl[];
extern void* cDynamicEmitter_vtbl[];

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
    self->vt = cDynamicParticleModifier_vtbl;
    func_00370B60(self->sub60);
    self->subVt = cDynamicEmitter_vtbl;
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

//100%
INCLUDE_ASM("object/effectlink", func_00345D80);
#ifdef SKIP_ASM
void* operator_new1(int size);
struct cStream00371318;
extern "C" void func_00371318(void* self, cStream00371318* stream);
extern "C" void func_003710D0(void* sub, void* a, void* b, int flag, float dt);

struct sEffectLinkVEntry_345D80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" sEff00345C90* func_00345D80(sEff00345C90* self, void* stream)
{
    func_003457C8(self, stream);
    self->vt = cDynamicParticleModifier_vtbl;
    void* data = &self->bone;
    func_00370B60(self->sub60);
    self->subVt = cDynamicEmitter_vtbl;
    sEffectLinkVEntry_345D80* vt = *(sEffectLinkVEntry_345D80**)stream;
    vt[2].fn((char*)stream + vt[2].delta, data, 0x50);
    if (self->f18 == 0) {
        void* t = operator_new1(0xD8);
        self->target = (sTarget00345C90*)t;
        sEffectLinkVEntry_345D80* vt2 = *(sEffectLinkVEntry_345D80**)stream;
        vt2[2].fn((char*)stream + vt2[2].delta, t, 0xD8);
        func_00370DC8(self->sub60, self->target, -1.0f);
        func_00371318(self->sub60, (cStream00371318*)stream);
        func_003710D0(self->sub60, &self->worldA, &self->worldB, 1, 0.01666666753590107f);
    } else {
        self->target = 0;
    }
    D_004A3FFC++;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier__cDynamicParticleModifier);
#ifdef SKIP_ASM
extern int D_004A3FFC;
extern void* cDynamicParticleModifier_vtbl[];
void operator_delete(int*);
extern "C" void cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(void* self, int flags);
extern "C" void cEffectModifier__cEffectModifier(void* self, int flags);

/**
 * Destroys a dynamic particle modifier: frees its 0xD8-byte buffer (+0x260), decrements the live count
 * (D_004A3FFC), destroys the embedded dynamic emitter (+0x60), then the cEffectModifier part.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cDynamicParticleModifier__cDynamicParticleModifier(void* self, int flags)
{
    char* s = (char*)self;
    *(void***)(s + 0x8) = cDynamicParticleModifier_vtbl;
    int* buf = *(int**)(s + 0x260);
    if (buf != 0) {
        operator_delete(buf);
    }
    D_004A3FFC--;
    cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(s + 0x60, 2);
    cEffectModifier__cEffectModifier(self, flags);
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
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_update);
#ifdef SKIP_ASM
struct sEffectLink5EF8;
extern "C" void func_00345EF8(sEffectLink5EF8* self);
extern "C" void func_003710D0(void* sub, void* a, void* b, int flag, float dt);

/**
 * Advances a dynamic particle effect by one 1/60 s step and marks it finished when it is done.
 *
 * Follows its link first (func_00345EF8), then updates the emitter at +0x60 with the positions at
 * +0x30/+0x40. While fading (+0x14) it stops emitting and finishes once no particles are left (+0x240);
 * otherwise it keeps emitting and finishes when a non-negative life (+0x60) runs out. Does nothing once
 * finished (+0x18).
 */
extern "C" void cDynamicParticleModifier_update(void* self)
{
    char* selfBytes = (char*)self;
    if (*(int*)(selfBytes + 0x18) != 0) {
        return;
    }
    func_00345EF8((sEffectLink5EF8*)self);
    if (*(int*)(selfBytes + 0x14) > 0) {
        float* sub = (float*)(selfBytes + 0x60);
        if (*(int*)(selfBytes + 0x240) > 0) {
            func_003710D0(sub, selfBytes + 0x30, selfBytes + 0x40, 0, 0.01666666753590107f);
            return;
        }
    } else {
        float* sub = (float*)(selfBytes + 0x60);
        if (*(float*)(selfBytes + 0x60) < 0.0f) {
            func_003710D0(sub, selfBytes + 0x30, selfBytes + 0x40, 1, 0.01666666753590107f);
            return;
        }
        func_003710D0(sub, selfBytes + 0x30, selfBytes + 0x40, 1, 0.01666666753590107f);
        if (!(*(float*)(selfBytes + 0x60) <= 0.0f)) {
            return;
        }
    }
    *(int*)(selfBytes + 0x18) = 1;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_fadeEffect__FPv);
#ifdef SKIP_ASM
/**
 * Makes a dynamic particle effect fade out instead of stopping at once.
 *
 * Sets the fade flag (offset 0x14). cDynamicParticleModifier_update then stops emitting and marks the effect
 * finished once the emitter's count at offset 0x240 (probably its live particles) reaches 0.
 */
void cDynamicParticleModifier_fadeEffect(void* self)
{
    *(int*)((char*)self + 0x14) = 1;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_render);
#ifdef SKIP_ASM
extern "C" void cDynamicEmitter_render(void*, int);

/**
 * Renders the dynamic particle modifier's emitter unless the effect has finished.
 *
 * Calls cDynamicEmitter_render() with 7 on the emitter embedded at offset 0x60 while the finished flag
 * (offset 0x18) is clear.
 */
extern "C" void cDynamicParticleModifier_render(void* self)
{
    if (*(int*)((char*)self + 0x18) == 0) {
        cDynamicEmitter_render((char*)self + 0x60, 7);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sEffectLinkVEntry60A0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_003712B8(void* self, void* stream);

/**
 * Writes a dynamic particle modifier's state to a replay frame.
 *
 * Writes the cEffectModifier part and the 0x50 bytes at +0x10 (which include the fade and finished flags);
 * while not finished, also the 0xD8-byte buffer (+0x260) and the emitter's state (func_003712B8).
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cDynamicParticleModifier_writeToReplayFrame(void* self, void* stream)
{
    cEffectModifier_writeToReplayFrame(self, stream);
    sEffectLinkVEntry60A0* writeEntry = &(*(sEffectLinkVEntry60A0**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x10, 0x50);
    if (*(int*)((char*)self + 0x18) == 0) {
        writeEntry = &(*(sEffectLinkVEntry60A0**)stream)[1];
        writeEntry->fn((char*)stream + writeEntry->delta, *(void**)((char*)self + 0x260), 0xD8);
        func_003712B8((char*)self + 0x60, stream);
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00346120);
#ifdef SKIP_ASM
extern int D_004A4000;
extern void* cHaloModifier_vtbl[];
extern "C" void* func_00345798(void* self, int a1);

struct sEffQuat_6120 {
    float x, y, z, w;
    sEffQuat_6120(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
} __attribute__((aligned(16)));

struct sEffObj_6120 {
    sEffectLinkView link;   // 0x0
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
    self->vt = cHaloModifier_vtbl;
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
extern void* cHaloModifier_vtbl[];
extern "C" void* func_003457C8(void* p, void* obj);

struct sEffVEntry61C0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_003461C0(void* self, void* stream)
{
    func_003457C8(self, stream);
    *(void***)((char*)self + 0x8) = cHaloModifier_vtbl;
    sEffVEntry61C0* vt = *(sEffVEntry61C0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x10, 0x30);
    D_004A4000++;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cHaloModifier__cHaloModifier);
#ifdef SKIP_ASM
extern int D_004A4000;
extern void* cHaloModifier_vtbl[];
extern "C" void cEffectModifier__cEffectModifier(void* self, int flags);

struct sEffectLinkObj_6228 {
    sEffectLinkView link;  // 0x0
    void** vt;         // 0x8
};

/**
 * Destroys a halo modifier: decrements the live count (D_004A4000), then runs the cEffectModifier destructor.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cHaloModifier__cHaloModifier(void* self, int flags)
{
    ((sEffectLinkObj_6228*)self)->vt = cHaloModifier_vtbl;
    D_004A4000--;
    cEffectModifier__cEffectModifier(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cHaloModifier_update);
#ifdef SKIP_ASM
/**
 * Spins a halo: adds the integer step (+0x2C) to its angle in degrees (+0x30), resetting it to 0 at +/-360.
 */
// Advance a rotation angle (degrees) by an integer step, wrapping to 0 at +/-360.
extern "C" void cHaloModifier_update(void* self)
{
    float angle = *(float*)((char*)self + 0x30) + (float)*(int*)((char*)self + 0x2c);
    *(float*)((char*)self + 0x30) = angle;
    if (angle >= 360.0f || angle <= -360.0f) {
        *(float*)((char*)self + 0x30) = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cHaloModifier_render);
#ifdef SKIP_ASM
struct sEffVec4_62A0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sEffMtx_62A0 {
    sEffVec4_62A0 r[4];
};

extern "C" void func_0034FED8(void* a, int b, sEffMtx_62A0* out);
extern "C" void func_002D1D10(sEffVec4_62A0* pos, void* p, int n, float a, float b);

/**
 * Draws a halo at the position of its instance's mesh.
 *
 * Gets the world matrix of mesh +0x28 of the instance (+0xC) and draws the halo sprite there with
 * func_002D1D10(), passing the halo data (+0x10), +0x24, the size (+0x20) and the current angle (+0x30).
 */
extern "C" void cHaloModifier_render(void* self)
{
    sEffMtx_62A0 matrix;
    void* p = (char*)self + 0x10;
    func_0034FED8(*(void**)((char*)self + 0xC), *(int*)((char*)self + 0x28), &matrix);
    sEffVec4_62A0 origin = matrix.r[3];
    func_002D1D10(&origin, p, *(int*)((char*)self + 0x24),
                  *(float*)((char*)self + 0x20), *(float*)((char*)self + 0x30));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", cHaloModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sEffectLinkVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cEffectModifier_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a halo modifier's state to a replay frame.
 *
 * Writes the effect-modifier part with cEffectModifier_writeToReplayFrame(), then the 0x30 bytes at offset 0x10
 * through the stream's write virtual (slot 1).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cHaloModifier_writeToReplayFrame(void* self, void* stream)
{
    cEffectModifier_writeToReplayFrame(self, stream);
    sEffectLinkVEntry* vt = *(sEffectLinkVEntry**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x10, 0x30);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cFlexBridgeNode_cFlexBridgeNode);
#ifdef SKIP_ASM
struct sEffV3_346350 {
    float x, y, z;
    sEffV3_346350() {}
};
struct sEffV2_346350 {
    float u, v;
    sEffV2_346350() {}
};
struct sEffV4_346350 {
    float x, y, z, w;
    sEffV4_346350() {}
};

struct sEffVE_346350 {
    short delta;
    short index;
    void (*fn)(void*, void*, int, sEffV3_346350*, sEffV2_346350*, sEffV4_346350*);
};

extern "C" void* func_0034FB00(void* self, int a1, int a2, int a3);
void func_00353DA8(void* self);
extern "C" void func_00353E08(void* self, int a1, float len);
extern "C" void cFlexBridgeNode_initSpringNodes(void* self);
extern "C" void cFlexBridgeNode_setupGrid(void* self);
extern "C" void cFlexBridgeNode_updateGrid(void*);
extern void* cFlexBridgeNode_vtbl[];

// PORT: SN sqrt.s asm helper (PS2 FPU).
static inline float sqrtf_346350(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

struct sEffFlags_346350 {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
};

/**
 * Constructs a flexible bridge node for a world instance.
 *
 * Builds the spring chain (+0x50) from the bridge data and the length of the instance's first edge (the first
 * two mesh vertices, through graphics manager slot 97), then sets up the strip mesh, the spring nodes and the
 * first pose, and updates the instance's flag bits. Also sets +0x40 = 100 and +0x44 = 12 (meaning unknown).
 *
 * @param bucket  update bucket
 * @param pInst   the world instance (On Tour tInstanceRecord*)
 * @param tdata   the bridge data (On Tour tMakeFlexBridgeData*)
 * @return self
 */
extern "C" void* cFlexBridgeNode_cFlexBridgeNode(void* self, int bucket, int pInst, int tdata)
{
    func_0034FB00(self, bucket, 5, pInst);
    *(void***)((char*)self + 0xC) = cFlexBridgeNode_vtbl;
    func_00353DA8((char*)self + 0x50);
    *(int*)((char*)self + 0x44) = 12;
    *(float*)((char*)self + 0x40) = 100.0f;
    sEffV3_346350 pts[4];
    sEffV2_346350 uv[4];
    sEffV4_346350 col[4];
    sEffVE_346350* vt = (sEffVE_346350*)cGraphicsMan_TheGraphicsMan->mVtbl;
    vt[97].fn((char*)cGraphicsMan_TheGraphicsMan + vt[97].delta, *(void**)((char*)self + 0x18), 0, pts, uv, col);
    sEffV3_346350 span;
    span.x = pts[1].x - pts[0].x;
    span.y = pts[1].y - pts[0].y;
    span.z = pts[1].z - pts[0].z;
    func_00353E08((char*)self + 0x50, tdata, sqrtf_346350(span.x * span.x + span.y * span.y + span.z * span.z));
    cFlexBridgeNode_setupGrid(self);
    cFlexBridgeNode_initSpringNodes(self);
    cFlexBridgeNode_updateGrid(self);
    unsigned int* flags = (unsigned int*)(*(char**)((char*)self + 0x18) + 8);
    *flags = (*flags & 0xFFFFFFFD) | 4;
    flags = (unsigned int*)(*(char**)((char*)self + 0x18) + 8);
    *flags = (*flags & 0xFFFFFFDF) | 0x40;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cFlexBridgeNode_cFlexBridgeNode1);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern "C" void* func_00353E80(void* self, void* stream);
extern "C" void cFlexBridgeNode_setupGrid(void* self);
extern "C" void cFlexBridgeNode_updateGrid(void*);
extern void* cFlexBridgeNode_vtbl[];

struct sEffectLinkVEntry64E0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Constructs a flexible bridge node from a replay frame.
 *
 * Runs the cInstanceNode replay constructor, reads the spring chain (func_00353E80), sets up the strip mesh,
 * reads 0x30 bytes at +0x20 and poses the strip.
 *
 * @param bucket  update bucket
 * @param stream  the replay frame to read (On Tour cBxReplayFrame*)
 * @return self
 */
extern "C" void* cFlexBridgeNode_cFlexBridgeNode1(void* self, void* bucket, void* stream)
{
    void* payload = (char*)self + 0x20;
    cInstanceNode_cInstanceNode(self, bucket, stream);
    *(void***)((char*)self + 0xC) = cFlexBridgeNode_vtbl;
    func_00353E80((char*)self + 0x50, stream);
    cFlexBridgeNode_setupGrid(self);
    sEffectLinkVEntry64E0* readEntry = &(*(sEffectLinkVEntry64E0**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, payload, 0x30);
    cFlexBridgeNode_updateGrid(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cFlexBridgeNode__cFlexBridgeNode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cMemMan_free(void* ptr);
extern "C" void* cObjectInterface_getNash();
extern "C" void cSpring__cSpring(void* self, int flags);
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);

struct sVec4_346568 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBox_346568 {
    sVec4_346568 min;
    sVec4_346568 max;
};

struct sInst_346568 {
    char pad_0x00[0x60];
    float min[3];
    float max[3];
};

extern "C" void nash_Nash_movenode(void* world, int type, void* id, sBox_346568* box, sBox_346568* old);

struct sRVEntry_346568 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
struct sRCtx_346568 {
    char pad[0x10D8];
    sRVEntry_346568* vt;
};

/**
 * Destroys a flexible bridge node and puts its instance back into the space hash with its own bounding box.
 *
 * Frees the face, normal and vertex arrays, releases the strip mesh (graphics manager slot 77), moves the
 * instance in the Nash grid from the bridge's box to the instance's own box, destroys the spring chain and
 * runs the cInstanceNode destructor.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cFlexBridgeNode__cFlexBridgeNode(char* self, int flags)
{
    *(void***)(self + 0xC) = cFlexBridgeNode_vtbl;
    if (*(void**)(self + 0x84) != 0) {
        cMemMan_free(*(void**)(self + 0x84));
    }
    if (*(void**)(self + 0x80) != 0) {
        cMemMan_free(*(void**)(self + 0x80));
    }
    if (*(void**)(self + 0x7C) != 0) {
        cMemMan_free(*(void**)(self + 0x7C));
    }
    sBox_346568* box = (sBox_346568*)(self + 0x90);
    ((sRVEntry_346568*)cGraphicsMan_TheGraphicsMan->mVtbl)[77].fn((char*)cGraphicsMan_TheGraphicsMan + ((sRVEntry_346568*)cGraphicsMan_TheGraphicsMan->mVtbl)[77].delta, *(void**)(self + 0x78));
    sBox_346568 old = *(sBox_346568*)(self + 0x90);
    sInst_346568* inst = *(sInst_346568**)(self + 0x18);
    sVec4_346568 t;
    t.x = inst->min[0];
    t.y = inst->min[1];
    t.z = inst->min[2];
    t.w = 1.0f;
    *(sVec4_346568*)(self + 0x90) = t;
    t.x = inst->max[0];
    t.y = inst->max[1];
    t.z = inst->max[2];
    t.w = 1.0f;
    *(sVec4_346568*)(self + 0xA0) = t;
    nash_Nash_movenode(cObjectInterface_getNash(), 0, inst, box, &old);
    cSpring__cSpring(self + 0x50, 2);
    cInstanceNode__cInstanceNode(self, flags);
}
#endif


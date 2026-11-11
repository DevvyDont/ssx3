#include "common.h"

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_initLightCache);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char D_00487CD8[];

struct sWLCVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sWLCVec4 D_004FF120;

struct sWLCList {
    unsigned int head;
    char pad04[0x100];
};

struct sWLCBuf {
    int owner;
    int count;
    sWLCList lists[3];
};

struct sWLCNode {
    unsigned int id;
    int priority;
    char pad08[0xA8];
    sWLCVec4 v;
    int fC0;
    char padC4[0xAC];
    sWLCNode* next;
    sWLCNode* prev;
    char pad178[8];
};

// Light slot view based at self+0x40*i: id/f14 at +0x10/+0x14; slot i's cache buffer
// lives at +0x40 (func_002F53B0 views it as slots[i].buf at self+0x40).
struct sWLCLightHdr {
    char pad00[0x10];
    unsigned int id;
    int f14;
    char pad18[0x28];
};

struct sWLCMan {
    union {
        int f00;
        sWLCLightHdr hdr[8];
    };
    char pad200[0x10];
    sWLCNode nodes[32];
    sWLCNode* heads[32];
    int count;
};

extern "C" void cWorldLightMan_initLightCache(sWLCMan* self)
{
    unsigned int i;
    for (i = 0; i < 8; i++) {
        sWLCLightHdr* l = &self->hdr[i];
        l->id = 0xFFFFFFFF;
        l->f14 = 0;
        sWLCBuf* buf = (sWLCBuf*)operator new(0xA14, D_00487CD8, 0x20000000, 0);
        buf->owner = self->f00;
        buf->count = 0;
        buf->lists[0].head = 0xFFFFFFFF;
        buf->lists[1].head = 0xFFFFFFFF;
        buf->lists[2].head = 0xFFFFFFFF;
        *(sWLCBuf**)((char*)l + 0x40) = buf;
    }
    for (i = 0; i < 32; i++) {
        self->nodes[i].priority = 0;
        self->nodes[i].id = 0xFFFFFFFF;
        self->nodes[i].v = D_004FF120;
        self->nodes[i].fC0 = 0;
        self->nodes[i].next = 0;
        self->nodes[i].prev = 0;
    }
    self->count = 0;
    for (i = 0; i < 32; i++) {
        self->heads[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F53B0);
#ifdef SKIP_ASM
void operator_delete(int*);

struct func_002F53B0_sSlot {
    int* buf;
    char pad04[0x3C];
};

struct func_002F53B0_sMan {
    char pad00[0x40];
    func_002F53B0_sSlot slots[8];
};

extern "C" void func_002F53B0(func_002F53B0_sMan* self)
{
    unsigned int i;
    for (i = 0; i < 8; i++) {
        operator_delete(self->slots[i].buf);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F5400);
#ifdef SKIP_ASM
extern char D_005047F8[];
extern int D_004A43C4;
extern int D_004A3B64;
extern char* D_004A5B64;

struct sVec_5400 {
    float x, y, z, w;
    sVec_5400() {}
    sVec_5400(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

// Plain (ctor-less) quadword for matrix rows, so a local matrix needs no construction loop.
struct sQuad_5400 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_5400 {
    sQuad_5400 row[4];
};

struct sAABox_5400 {
    sVec_5400 min;
    sVec_5400 max;
    sAABox_5400() {}
    sAABox_5400(const sVec_5400& a, const sVec_5400& b) { min = a; max = b; }
};

// 0xA0-byte light set (ten quadwords).
struct sLightSet_5400 {
    sVec_5400 v[10];
};

struct sBox_5400 {
    unsigned int id;
    int priority;
    char pad08[0x8];
    sAABox_5400 bounds; // 0x10
    void* cell;         // 0x30
};

struct sNode_5400 {
    unsigned int id;          // 0x0
    int priority;             // 0x4
    char pad08[0x8];
    sLightSet_5400 cur;       // 0x10
    sVec_5400 pos;            // 0xB0
    sBox_5400* box;           // 0xC0
    char padC4[0xC];
    sLightSet_5400 target;    // 0xD0
    sNode_5400* next;         // 0x170
    sNode_5400* prev;         // 0x174
    char pad178[8];
};

struct sMan_5400 {
    char pad000[0x4];
    char* world;              // 0x4
    char pad008[0x210 - 0x8];
    sNode_5400 nodes[32];     // 0x210
    sNode_5400* heads[32];    // 0x3210
    unsigned int count;       // 0x3290
};

// Light-set reference: a name until first use, then a cached handle tagged 0x123400.
struct sLightRef_5400 {
    int handle;
    int magic;
};

struct sVEf_5400 {
    short delta;
    short index;
    int (*fn)(void*, float, float, float);
};
struct sVEr_5400 {
    short delta;
    short index;
    sLightRef_5400* (*fn)(void*);
};

// The unit defines func_002F59D0 later with its own node type.
extern "C" sNode_5400* func_002F59D0_n(sMan_5400* self, int priority) __asm__("func_002F59D0");
extern "C" sBox_5400* func_002F5A70(sMan_5400* self, int priority);
extern "C" void func_002F5AF0(void* self, void* a1, void* a2, void* a3, int count);
extern "C" void func_0034FED8(void* obj, int a1, sMat_5400* out);
extern "C" void func_00331450(void* cell, sAABox_5400* box);
extern "C" sLightSet_5400* func_002EE2F8(void);
extern "C" int func_0038AC50(void*, void*);
extern "C" sLightSet_5400* func_0038ABF8(void* self, int i);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec_5400 Sub_5400(const sVec_5400& a, const sVec_5400& b)
{
    sVec_5400 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec_5400 Add_5400(const sVec_5400& a, const sVec_5400& b)
{
    sVec_5400 r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_5400(const sVec_5400& v)
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

static inline sVec_5400 Trans_5400(const sMat_5400& m)
{
    return *(const sVec_5400*)&m.row[3];
}

static inline sVec_5400 ObjPos_5400(char* obj)
{
    return *(sVec_5400*)(obj + 0x40);
}

static inline sLightSet_5400* Resolve_5400(sLightRef_5400* ref)
{
    if (ref->magic != 0x123400) {
        ref->handle = func_0038AC50(D_005047F8, ref);
        ref->magic = 0x123400;
    }
    return func_0038ABF8(D_005047F8, ref->handle);
}

// Pushes node onto the front of hash chain heads[slot].
static inline void Link_5400(sNode_5400** heads, int slot, sNode_5400* node)
{
    sNode_5400** p = heads + slot;
    sNode_5400* old = *p;
    node->prev = 0;
    node->next = old;
    if (old != 0) {
        old->prev = node;
    }
    *p = node;
}

static inline sLightRef_5400* GetRef_5400(char* o, int slot)
{
    sVEr_5400* vt = *(sVEr_5400**)(o + 4);
    return vt[slot].fn(o + vt[slot].delta);
}

// Finds (or allocates) the cached light set for obj, refreshing it when obj has moved.
extern "C" sLightSet_5400* func_002F5400(sMan_5400* self, char* obj, int force)
{
    unsigned int key = *(unsigned int*)(obj + 0x78);
    int slot = (key >> 8) & 0x1F;
    int found = 0;
    sNode_5400* node;
    for (node = self->heads[slot]; node != 0; node = node->next) {
        if (node->id == *(unsigned int*)(obj + 0x78)) {
            found = 1;
            break;
        }
    }
    int prio = *(int*)(D_004A5B64 + 0x18);
    if (!found) {
        if (self->count < 32) {
            node = &self->nodes[self->count++];
        } else {
            node = func_002F59D0_n(self, prio);
        }
        node->id = *(unsigned int*)(obj + 0x78);
        Link_5400(self->heads, slot, node);
    }
    node->priority = prio;
    int reload = 0;
    int moved = 0;
    sVec_5400 pos;
    if (!found) {
        reload = 1;
        moved = 1;
        if (*(int*)(obj + 0xC) != 0) {
            sMat_5400 m;
            func_0034FED8(obj, 0, &m);
            pos = Trans_5400(m);
        } else {
            pos = ObjPos_5400(obj);
        }
    } else if (force) {
        if (*(int*)(obj + 0xC) != 0) {
            sMat_5400 m;
            func_0034FED8(obj, 0, &m);
            pos = Trans_5400(m);
        } else {
            pos = ObjPos_5400(obj);
        }
        float d = Length_5400(Sub_5400(pos, node->pos));
        if (10.0f < d) moved = 1;
        if (500.0f < d) reload = 1;
    }
    if (moved) {
        node->pos = pos;
        int rebuild = 0;
        if (node->box == 0 || node->box->id != node->id) {
            node->box = func_002F5A70(self, node->priority);
            node->box->id = node->id;
            rebuild = 1;
        }
        node->box->priority = node->priority;
        if (!rebuild) {
            sBox_5400* b = node->box;
            int inside = 0;
            if (b->bounds.min.x < node->pos.x && node->pos.x < b->bounds.max.x && b->bounds.min.y < node->pos.y && node->pos.y < b->bounds.max.y &&
                b->bounds.min.z < node->pos.z && node->pos.z < b->bounds.max.z) {
                inside = 1;
            }
            if (!inside) rebuild = 1;
        }
        if (rebuild) {
            float extent = 1000.0f;
            node->box->bounds = sAABox_5400(Sub_5400(node->pos, sVec_5400(extent, extent, extent, 0.0f)),
                                            Add_5400(node->pos, sVec_5400(extent, extent, extent, 0.0f)));
            func_00331450(node->box->cell, &node->box->bounds);
        }
        if (reload) {
            if (D_004A3B64 != 0) {
                node->target = *func_002EE2F8();
            } else {
                char* world = self->world;
                sVEf_5400* vt = *(sVEf_5400**)(world + 4);
                vt[2].fn(world + vt[2].delta, node->pos.x, node->pos.y, -99999.0f);
                sLightRef_5400* ref = GetRef_5400(*(char**)self->world, 40);
                if (*(signed char*)ref != 0) {
                    node->target = *Resolve_5400(ref);
                } else {
                    ref = GetRef_5400(*(char**)self->world, 37);
                    if (*(signed char*)ref != 0) {
                        node->target = *Resolve_5400(ref);
                    } else {
                        node->target = *func_0038ABF8(D_005047F8, D_004A43C4);
                    }
                }
            }
        }
        node->cur = node->target;
        func_002F5AF0(self, &node->pos, node->box->cell, &node->cur, 4);
    }
    return &node->cur;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F5998);
#ifdef SKIP_ASM
struct sWorldLight {
    char pad00[0x10];
    unsigned int id;
    char pad14[0x2C];
};

extern "C" void func_002F5998(sWorldLight* lights)
{
    unsigned int i;
    for (i = 0; i < 8; i++) {
        lights[i].id = 0xFFFFFFFF;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F59D0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sWorldLightNode {
    long key;
    char pad08[0x168];
    sWorldLightNode* next;
    sWorldLightNode* prev;
    char pad178[8];
};

struct sWorldLightMan {
    char pad000[0x210];
    sWorldLightNode nodes[32];
    sWorldLightNode* heads[32];
};

extern "C" sWorldLightNode* func_002F59D0(sWorldLightMan* self, int priority)
{
    sWorldLightNode* best = 0;
    int min = priority + 1;
    unsigned int i;
    for (i = 0; i < 32; i++) {
        int p = *(int*)((char*)&self->nodes[i] + 4);
        if (p < min) {
            min = p;
            best = &self->nodes[i];
        }
    }
    int slot = (int)(best->key >> 8) & 0x1F;
    if (best->prev == 0) {
        self->heads[slot] = best->next;
        if (best->next != 0) {
            best->next->prev = 0;
        }
    } else {
        if (best->next == 0) {
            best->prev->next = 0;
        } else {
            best->prev->next = best->next;
            best->next->prev = best->prev;
        }
    }
    return best;
}
#endif

INCLUDE_ASM("visualfx/worldlightman", func_002F5A70);

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F5AF0);
#ifdef SKIP_ASM
extern "C" void func_002F5B68(void* self, void* a1, void* a2, void** out, int count);
extern "C" void func_0038A6A8(void* a, void* b, void* c);

extern "C" void func_002F5AF0(void* self, void* a1, void* a2, void* a3, int count)
{
    void* buf[32];
    int i;
    func_002F5B68(self, a1, a2, buf, count);
    for (i = 0; i < count; i++) {
        if (buf[i] != 0) {
            func_0038A6A8(a3, a1, buf[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F5B68);
#ifdef SKIP_ASM
extern "C" float func_002F5D30(void* a, void* b, void* light);

struct sLight_5B68 {
    char pad_0x0[0x8];
    int type;           // 0x8
};

struct sLightMgr_5B68 {
    char pad_0x0[0x210];
    unsigned int count;         // 0x210
    sLight_5B68* lights[1];     // 0x214
};

// Keeps the `max` best-scoring type-6 lights in out[], sorted by descending score.
extern "C" void func_002F5B68(void* a, void* b, void* mgrp, void** outp, int max)
{
    sLightMgr_5B68* mgr = (sLightMgr_5B68*)mgrp;
    sLight_5B68** out = (sLight_5B68**)outp;
    float score[32];
    int count = 0;
    int i;
    for (i = 0; i < max; i++) {
        out[i] = 0;
        score[i] = 0.0f;
    }
    float minScore = 0.0f;
    unsigned int n = mgr->count;
    for (unsigned int j = 0; j < n; j++) {
        sLight_5B68* l = mgr->lights[j];
        if (l->type == 6) {
            float s = func_002F5D30(a, b, l);
            if (minScore < s) {
                int k;
                for (k = 0; k < count && s < score[k]; k++) {
                }
                if (k < max) {
                    count += count < max;
                    for (int m = count - 1; k < m; m--) {
                        out[m] = out[m - 1];
                        score[m] = score[m - 1];
                    }
                    out[k] = l;
                    score[k] = s;
                }
                minScore = score[max - 1];
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F5D30);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" float func_002F5D30(void* a, void* b, void* light);

struct func_002F6168_sVec3;
struct func_002F6168_sLight;
extern "C" int func_002F6168(void* self, func_002F6168_sVec3* p, func_002F6168_sLight* light,
                             float* outDist, float* outInvDist, float* outDot);

struct sLight_5D30 {
    char pad0[0x10];
    int type;               // 0x10
    float intensity;        // 0x14
    float scale;            // 0x18
    float radius;           // 0x1C
    char pad20[0x5C - 0x20];
    float inner;            // 0x5C
    float outer;            // 0x60
    signed char fallPow;    // 0x64
    signed char conePow;    // 0x65
};

static inline float Atten_5D30(sLight_5D30* l, float dist)
{
    if (l->radius < 5000.0f || dist <= 3750.0f) {
        return 1.0f;
    }
    return 1.0f - (dist - 3750.0f) * 0.0008f;
}

static inline float Fall_5D30(sLight_5D30* l, float inv)
{
    int p = l->fallPow;
    float s = inv * 100.0f;
    if (s < 1.0f) {
        switch (p) {
        case 1:
            return s;
        case 2:
            return s * s;
        case 3:
            return s * s * s;
        }
    }
    return 1.0f;
}

static inline float PowN_5D30(float x, int n)
{
    if (x == 0.0f) {
        return 0.0f;
    }
    if (n < 0) {
        x = 1.0f / x;
        n = -n;
    }
    float r = 1.0f;
    while (n) {
        if (n & 1) {
            r *= x;
        }
        n >>= 1;
        x *= x;
    }
    return r;
}

static inline float Pow_5D30(float x, int n)
{
    switch (n) {
    case 0:
        return 1.0f;
    case 1:
        return x;
    case 2:
        return x * x;
    case 3:
        return x * x * x;
    case 4:
        return x * x * x * x;
    default:
        return PowN_5D30(x, n);
    }
}

extern "C" float func_002F5D30(void* a, void* b, void* lp)
{
    sLight_5D30* l = (sLight_5D30*)lp;
    float dist;
    float inv;
    float dot;
    float att;
    float v;
    switch (l->type) {
    case 0:
        return l->scale;
    case 1:
        if (func_002F6168(a, (func_002F6168_sVec3*)b, (func_002F6168_sLight*)l, &dist, &inv, &dot)) {
            if (dot >= l->outer) {
                att = Atten_5D30(l, dist);
                v = Fall_5D30(l, inv) * l->intensity * att;
                if (l->inner <= dot) {
                    v *= Pow_5D30(dot, l->conePow);
                } else {
                    v *= Pow_5D30(l->inner, l->conePow);
                    v *= (dot - l->outer) / (l->inner - l->outer);
                }
                return l->scale * v;
            }
        }
        break;
    case 2:
        if (func_002F6168(a, (func_002F6168_sVec3*)b, (func_002F6168_sLight*)l, &dist, &inv, 0)) {
            att = Atten_5D30(l, dist);
            v = Fall_5D30(l, inv) * l->intensity * att;
            return l->scale * v;
        }
        break;
    case 3:
        return 0.0f;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6168);
#ifdef SKIP_ASM
struct func_002F6168_sVec3 {
    float x;
    float y;
    float z;
};

struct func_002F6168_sLight {
    char pad[0x1C];
    float radius;               // 0x1C
    char pad20[0x2C - 0x20];
    func_002F6168_sVec3 dir;    // 0x2C
    func_002F6168_sVec3 pos;    // 0x38
};

extern "C" int func_002F6168(void* self, func_002F6168_sVec3* p, func_002F6168_sLight* light,
                             float* outDist, float* outInvDist, float* outDot)
{
    float dx = light->pos.x - p->x;
    float dy = light->pos.y - p->y;
    float dz = light->pos.z - p->z;
    float d2 = dx * dx + dy * dy + dz * dz;
    float dist;
    float inv;
    if (light->radius * light->radius < d2) {
        return 0;
    }
    if (d2 != 0.0f) {
        // PORT: sqrt.s (sqrtf without errno check)
        __asm__("sqrt.s %0, %1" : "=f"(dist) : "f"(d2));
        inv = 1.0f / dist;
    } else {
        inv = 1.0f;
        dist = 1.0f;
    }
    if (outDot != 0) {
        *outDot = -(dx * light->dir.x + dy * light->dir.y + dz * light->dir.z) * inv;
    }
    *outDist = dist;
    *outInvDist = inv;
    return 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6238);
#ifdef SKIP_ASM
// g++ 2.95 vtable entry (no thunks): {delta, index, fn}. Vtables are double-aligned.
struct sVtEnt6238 {
    short delta;
    short index;
    void* fn;
};

struct sVt9_6238 {
    sVtEnt6238 e[9];
} __attribute__((aligned(8)));

struct sVt22_6238 {
    sVtEnt6238 e[22];
} __attribute__((aligned(8)));

extern const sVt9_6238 D_004886E0;
extern const sVt22_6238 D_00488728;
extern char D_00459B90[];

extern "C" void cRider_cRider(void* self);
void* func_002F64F0(void* self);

// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0x10): vtable copies with delta fixups when not in charge.
extern "C" void* func_002F6238(void* self, int inChrg)
{
    sVt9_6238 t1;
    sVt22_6238 t2;
    if (inChrg) {
        *(void**)self = (char*)self + 0x10;
        cRider_cRider((char*)self + 0x10);
    }
    *(void**)(*(char**)self + 0x6E8) = (void*)&D_004886E0;
    *(void**)(*(char**)self + 0x6D0) = D_00459B90;
    *(void**)(*(char**)self + 0x6C0) = (void*)&D_00488728;
    if (inChrg == 0) {
        int vc;
        t1 = D_004886E0;
        *(void**)(*(char**)self + 0x6E8) = &t1;
        {
            char* vbo = *(char**)self - 0x10;
            vc = (char*)self - vbo;
        }
        t1.e[1].delta = D_004886E0.e[1].delta + vc;
        t2 = D_00488728;
        *(void**)(*(char**)self + 0x6C0) = &t2;
        t2.e[1].delta = D_00488728.e[1].delta + vc;
    }
    func_002F64F0(self);
    return self;
}
#endif

INCLUDE_ASM("visualfx/worldlightman", func_002F6388);

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F64E8__FPv);
#ifdef SKIP_ASM
void func_002F64E8(void* self)
{
}
#endif

extern "C" void* func_00416210(void*, int, int);

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F64F0__FPv);
#ifdef SKIP_ASM
void* func_002F64F0(void* self)
{
    return func_00416210(((char*)self + 0x4), 0, 4);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6518);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: called for effect only; bound by asm label so the call has no value
void func_002F64F0_v(void* self) __asm__("func_002F64F0__FPv");
struct sWLCode6910;
extern "C" void func_002F6910(sWLCode6910* self);
extern void* D_004A28A8;
extern int D_004A3BC0;
extern int D_00534B30[];

struct sVec6518 {
    float x, y, z, w;
} __attribute__((aligned(16)));
extern sVec6518 D_004FF130;

struct sVE6518 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sRider6518 {
    char pad0[0x1E0];
    sVec6518 vel;               // 0x1E0
    char pad1F0[0xF8];          // 0x1F0
    float f2E8;                 // 0x2E8
    float f2EC;                 // 0x2EC
    float f2F0;                 // 0x2F0
    int mode;                   // 0x2F4
    float f2F8;                 // 0x2F8
    float f2FC;                 // 0x2FC
    char pad300[0x4];           // 0x300
    int f304;                   // 0x304
    int f308;                   // 0x308
    float maxSpeed;             // 0x30C
    float steer;                // 0x310
    float blend;                // 0x314
    char pad318[0x88];          // 0x318
    sVec6518 dir;               // 0x3A0
};

struct sWLCode6518 {
    sRider6518* rider;          // 0x0
    signed char state;          // 0x4
    unsigned char step;         // 0x5
    signed char count;          // 0x6
    unsigned char flags;        // 0x7
};

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float len6518(const sVec6518& v)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec6518 norm6518(const sVec6518& v)
{
    sVec6518 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sVec6518 scale6518(const sVec6518& v, float s)
{
    sVec6518 r;
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void scaleEq6518(sVec6518& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec6518 add6518(const sVec6518& a, const sVec6518& b)
{
    sVec6518 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

struct sVEsub6518 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_002F6518(sWLCode6518* self)
{
    char* sub = (char*)self->rider + 0x6C0;
    sVEsub6518* vt = *(sVEsub6518**)sub;
    if (vt[9].fn(sub + vt[9].delta) != 0) {
        return;
    }
    if (D_004A3BC0 >= 2) {
        func_002F64F0_v(self);
        D_004A3BC0--;
    }
    func_002F6910((sWLCode6910*)self);
    cBE_getInterface_Fv(*(void**)((char*)D_004A28A8 + 0x78), 7);
    if (D_00534B30[0] != 0) {
        self->state = 0;
    }
    if (self->state == 1) {
        self->rider->mode = 11;
        self->rider->f308 = (self->flags >> 1) & 1;
        self->rider->f2F8 = 1.0f;
        self->rider->f2F0 = 60.0f;
        self->rider->f2EC = 60.0f;
        self->rider->f2E8 = 60.0f;
        self->rider->f304 = 0;
    } else {
        self->rider->f308 = 1;
        if (self->rider->mode != 11) {
            return;
        }
        self->rider->mode = 0;
        self->rider->f2F8 = 0.0f;
        self->rider->f2F0 = 0.0f;
        self->rider->f2EC = 0.0f;
        self->rider->f2E8 = 0.0f;
        self->rider->f304 = 1;
    }
    sRider6518* r = self->rider;
    if (r->mode != 11) {
        return;
    }
    if (!(0.0f < r->f2FC)) {
        return;
    }
    float len = len6518(r->vel);
    if (len < r->maxSpeed && len != 0.0f) {
        r->vel = norm6518(r->vel);
        scaleEq6518(self->rider->vel, self->rider->maxSpeed * self->rider->blend + len * (1.0f - self->rider->blend));
        len = len6518(self->rider->vel);
    }
    r = self->rider;
    if (r->steer == 0.0f) {
        return;
    }
    if (!(0.0010000000474974513f < len6518(r->dir))) {
        return;
    }
    if (!(9.999999747378752e-05f < len6518(r->vel))) {
        return;
    }
    r->vel = add6518(scale6518(norm6518(r->dir), len * self->rider->steer),
                     scale6518(norm6518(self->rider->vel), len * (1.0f - self->rider->steer)));
    r = self->rider;
    if (r->vel.x < 0.0f && r->vel.x >= 0.0f) {
        r->vel = D_004FF130;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6908__FPv);
#ifdef SKIP_ASM
void func_002F6908(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6910);
#ifdef SKIP_ASM
struct sWLCode6910 {
    char pad0[4];
    signed char state;          // 0x4
    unsigned char step;         // 0x5
    signed char count;          // 0x6
    unsigned char flags;        // 0x7
};

extern signed char D_00445E30[];
extern int D_004A3BC0;
extern "C" void func_002F6B28(void* self);
extern "C" int func_002F6B38(void* self);

extern "C" void func_002F6910(sWLCode6910* self)
{
    int r = func_002F6B38(self);
    func_002F6B28(self);
    if (r == 0) {
        self->flags &= ~1;
        return;
    }
    if (self->flags & 1) {
        return;
    }
    self->flags |= 1;
    switch (self->state) {
    case -1:
        if (r & 0x40) {
            self->state = 0;
            self->count = 0;
            self->step = 0;
            self->flags = 0;
        }
        break;
    case 0:
        if (r & 0x10) {
            self->count++;
        } else if (r & 0x20) {
            if (self->count == D_00445E30[(signed char)self->step]) {
                self->count = 0;
                self->step++;
                if (self->step == 9) {
                    self->step = 0;
                    self->count = 0;
                    self->state = 1;
                }
            } else {
                self->state = -1;
            }
        }
        break;
    case 1:
        if (r == 0x60) {
            D_004A3BC0 = 8;
        } else if (r & 0x40) {
            self->flags ^= 2;
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6A58);
#ifdef SKIP_ASM
class func_002F6A58_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual int v01(void* dst, int size);
    virtual int v02(void* src, int size);
};

extern "C" void func_002F6A58(void* self, func_002F6A58_cObj* stream)
{
    stream->v01((char*)self + 4, 4);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6A90);
#ifdef SKIP_ASM
extern "C" void func_002F6A90(void* self, func_002F6A58_cObj* stream)
{
    stream->v02((char*)self + 4, 4);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6AC8);
#ifdef SKIP_ASM
extern "C" void func_002F6AC8(void* self, int type)
{
    switch (type) {
    case 0:
        *(unsigned char*)((char*)self + 7) |= 0x10;
        break;
    case 1:
        *(unsigned char*)((char*)self + 7) |= 0x20;
        break;
    case 2:
        *(unsigned char*)((char*)self + 7) |= 0x40;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6B28);
#ifdef SKIP_ASM
extern "C" void func_002F6B28(void* self)
{
    unsigned char* p = (unsigned char*)((char*)self + 0x7);
    *p &= 0x8f;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6B38);
#ifdef SKIP_ASM
extern "C" int func_002F6B38(void* self)
{
    return *(unsigned char*)((char*)self + 0x7) & 0x70;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6B48);
#ifdef SKIP_ASM
extern "C" void* func_002F6B48(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6B88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern float D_004D57B0[4];
extern float D_004D57C0[4];
extern float D_004D57D0[4];
extern float D_004D57E0[4];
extern float D_004D57F0[4];
extern float D_004D5800[4];
extern float D_004D5810[4];
extern float D_004D5820[4];
extern float D_004D5830[4];
extern float D_004D5840[4];
extern float D_004D5850[4];
extern float D_004D5860[4];
extern float D_004D5870[4];
extern float D_004D5880[4];
extern float D_004D5890[4];
extern float D_004D58A0[4];
extern float D_004D58B0[4];
extern float D_004D58C0[4];
extern float D_004D58D0[4];
extern float D_004D58E0[4];
extern float D_004D58F0[4];
extern float D_004D5900[4];
extern float D_004D5910[4];
extern float D_004D5920[4];
extern float D_004D5930[4];
extern float D_004D5940[4];
extern float D_004D5950[4];
extern float D_004D5960[4];
extern float D_004D5970[4];
extern float D_004D5980[4];
extern float D_004D5990[4];
extern float D_004D59A0[4];
extern float D_004D59B0[4];
extern float D_004D59C0[4];
extern float D_004D59D0[4];
extern float D_004D59E0[4];
extern float D_004D59F0[4];
extern float D_004D5A00[4];
extern float D_004D5A10[4];
extern float D_004D5A20[4];
extern float D_004D5A30[4];
extern float D_004D5A40[4];
extern float D_004D5A50[4];
extern float D_004D5A60[4];
extern float D_004D5A70[4];
extern float D_004D5A80[4];
extern float D_004D5A90[4];
extern float D_004D5AA0[4];
extern float D_004D5AB0[4];
extern float D_004D5AC0[4];
extern float D_004D5AD0[4];
extern float D_004D5AE0[4];
extern float D_004D5AF0[4];
extern float D_004D5B00[4];
extern float D_004D5B10[4];
extern float D_004D5B20[4];
extern float D_004D5B30[4];
extern float D_004D5B40[4];
extern float D_004D5B50[4];
extern float D_004D5B60[4];
extern float D_004FAAF0[4];
extern float D_004FAB00[4];
extern float D_004FAB10[4];
extern float D_004FAB20[4];
extern float D_004FAB30[4];
extern float D_004FAB40[4];
extern float D_004FAB50[4];
extern float D_004FAB60[4];
extern float D_004FAB70[4];
extern float D_004FAB80[4];
extern float D_004FAB90[4];
extern float D_004FABA0[4];
extern float D_004FABB0[4];
extern float D_004FABC0[4];
extern float D_004FABD0[4];
extern float D_004FABE0[4];
extern float D_004FABF0[4];
extern float D_004FAC00[4];
extern float D_004FAC10[4];
extern float D_004FAC20[4];
extern float D_004FAC30[4];
extern float D_004FAC40[4];
extern float D_004FAC50[4];
extern float D_004FAC60[4];
extern float D_004FAC70[4];
extern float D_004FAC80[4];
extern int D_004A5530;
extern int D_004A5538;
extern int D_004A553C;
extern int D_004A5540[2];
extern int D_004A5548[2];
extern int D_004A5550[2];
extern int D_004A5558[2];
extern int D_004A5560[2];
extern int D_004A5568[2];
extern int D_004A5570[2];
extern int D_004A5578[2];
extern int D_004A5580[2];
extern int D_004A5588[2];
extern int D_004A5590[2];
extern int D_004A5598[2];
extern int D_004A55A0[2];
extern int D_004A55A8[2];
extern float D_004A55AC;
extern int D_004A55B0;
extern int D_004A55B4;
extern float D_004A55B8;
extern float D_004A55BC;
struct sWlV3 {
    float x, y, z;
    sWlV3() {}
    sWlV3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};
static inline void setCol(float* c, float a, const sWlV3& v)
{
    c[0] = a;
    c[1] = v.x;
    c[2] = v.y;
    c[3] = v.z;
}
struct sWlAvElem {
    char pad[0x30];
    sWlAvElem() {}
};
extern "C" void* func_003714B8(void* self);
extern "C" void func_002D56B0(void* self, int flags);
struct sWlFx {
    char pad[0xD0];
    char emitter[0x2F0 - 0xD0];
    sWlFx() { func_003714B8(emitter); }
};
struct sWlRsm {
    char pad[0xF0];
    sWlRsm() {}
};
inline void* operator new[](unsigned int, void* p) { return p; }
extern sWlAvElem D_004D5B70[];
extern sWlAvElem D_004EDB70[];
extern sWlFx D_004EE770[];
// one past the end of D_004EE770[64] (the linker names that address D_004FA370)
extern sWlFx D_004FA370_end[] __asm__("D_004FA370");
extern sWlRsm D_004FA370[];
static inline void initE(int* p, int n)
{
    *(char*)p = n;
    *p = (*p & 0xFF) | (n << 8);
}
// Static constructors/destructors of this lump: the colour table, the avalanche particle
// arrays and fx objects (D_004D5B70, D_004EDB70, D_004EE770), the render-state
// entries (D_004FA370) and the light colour presets.
void sti_002F6B88(int init, int prio) __asm__("func_002F6B88");
void sti_002F6B88(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init) {
            D_004D57B0[0] = 1.0f;
            D_004D57B0[1] = 1.0f;
            D_004D57B0[2] = 1.0f;
            D_004D57B0[3] = 0.5f;
            D_004D57C0[0] = 0.4998680055141449f;
            D_004D57C0[1] = 1.0f;
            D_004D57C0[2] = 1.0f;
            D_004D57C0[3] = 0.0f;
            D_004D57D0[0] = 0.12951700389385223f;
            D_004D57D0[1] = 1.0f;
            D_004D57D0[2] = 1.0f;
            D_004D57D0[3] = 0.0f;
            D_004D57E0[0] = 0.22426900267601013f;
            D_004D57E0[1] = 0.9402310252189636f;
            D_004D57E0[2] = 0.5425059795379639f;
            D_004D57E0[3] = 0.0004030000127386302f;
            D_004D57F0[0] = 0.4652079939842224f;
            D_004D57F0[1] = 0.32170501351356506f;
            D_004D57F0[2] = 0.9943439960479736f;
            D_004D57F0[3] = 0.0f;
            D_004D5800[0] = 0.17792600393295288f;
            D_004D5800[1] = 1.0f;
            D_004D5800[2] = 1.0f;
            D_004D5800[3] = 0.0f;
            D_004D5810[0] = 0.4411740005016327f;
            D_004D5810[1] = 0.41388601064682007f;
            D_004D5810[2] = 0.3211260139942169f;
            D_004D5810[3] = 0.4398239850997925f;
            D_004D5820[0] = 0.500997006893158f;
            D_004D5820[1] = 0.24849599599838257f;
            D_004D5820[2] = 1.0f;
            D_004D5820[3] = 1.0f;
            D_004D5830[0] = 0.27687299251556396f;
            D_004D5830[1] = 0.6395620107650757f;
            D_004D5830[2] = 0.13423900306224823f;
            D_004D5830[3] = 0.0f;
            D_004D5840[0] = 0.8936439752578735f;
            D_004D5840[1] = 0.5263469815254211f;
            D_004D5840[2] = 0.5051450133323669f;
            D_004D5840[3] = 0.8687300086021423f;
            D_004D5850[0] = 1.0f;
            D_004D5850[1] = 1.0f;
            D_004D5850[2] = 1.0f;
            D_004D5850[3] = 1.0f;
            D_004D5860[0] = 1.0f;
            D_004D5860[1] = 0.0f;
            D_004D5860[2] = 0.0f;
            D_004D5860[3] = 0.0f;
            D_004D5870[0] = 1.0f;
            D_004D5870[1] = 0.550000011920929f;
            D_004D5870[2] = 0.5350000262260437f;
            D_004D5870[3] = 0.550000011920929f;
            D_004D5880[0] = 1.0f;
            D_004D5880[1] = 1.0f;
            D_004D5880[2] = 1.0f;
            D_004D5880[3] = 1.0f;
            D_004D5890[0] = 1.0f;
            D_004D5890[1] = 1.0f;
            D_004D5890[2] = 1.0f;
            D_004D5890[3] = 1.0f;
            D_004D58A0[0] = 1.0f;
            D_004D58A0[1] = 0.0f;
            D_004D58A0[2] = 0.0f;
            D_004D58A0[3] = 0.0f;
            D_004D58B0[0] = 1.0f;
            D_004D58B0[1] = 0.0f;
            D_004D58B0[2] = 0.0f;
            D_004D58B0[3] = 0.0f;
            D_004D58C0[0] = 1.0f;
            D_004D58C0[1] = 1.0f;
            D_004D58C0[2] = 1.0f;
            D_004D58C0[3] = 1.0f;
            D_004D58D0[0] = 1.0f;
            D_004D58D0[1] = 0.5f;
            D_004D58D0[2] = 1.0f;
            D_004D58D0[3] = 0.5f;
            D_004D58E0[0] = 1.0f;
            D_004D58E0[1] = 0.5f;
            D_004D58E0[2] = 1.0f;
            D_004D58E0[3] = 0.5f;
            D_004D58F0[0] = 1.0f;
            D_004D58F0[1] = 0.5f;
            D_004D58F0[2] = 1.0f;
            D_004D58F0[3] = 0.5f;
            D_004D5900[0] = 1.0f;
            D_004D5900[1] = 0.7828119993209839f;
            D_004D5900[2] = 0.06171900033950806f;
            D_004D5900[3] = 0.03984399884939194f;
            D_004D5910[0] = 1.0f;
            D_004D5910[1] = 1.0f;
            D_004D5910[2] = 0.7988259792327881f;
            D_004D5910[3] = 0.0f;
            D_004D5920[0] = 0.9886710047721863f;
            D_004D5920[1] = 0.9230499863624573f;
            D_004D5920[2] = 0.34311699867248535f;
            D_004D5920[3] = 0.0f;
            D_004D5930[0] = 1.0f;
            D_004D5930[1] = 1.0f;
            D_004D5930[2] = 1.0f;
            D_004D5930[3] = 1.0f;
            D_004D5940[0] = 1.0f;
            D_004D5940[1] = 1.0f;
            D_004D5940[2] = 0.7976430058479309f;
            D_004D5940[3] = 0.0f;
            D_004D5950[0] = 1.0f;
            D_004D5950[1] = 0.9222670197486877f;
            D_004D5950[2] = 0.34179699420928955f;
            D_004D5950[3] = 0.0f;
            D_004D5960[0] = 1.0f;
            D_004D5960[1] = 0.7847639918327332f;
            D_004D5960[2] = 0.06054700165987015f;
            D_004D5960[3] = 0.04062499850988388f;
            D_004D5970[0] = 1.0f;
            D_004D5970[1] = 0.800000011920929f;
            D_004D5970[2] = 0.44999998807907104f;
            D_004D5970[3] = 1.0f;
            D_004D5980[0] = 1.0f;
            D_004D5980[1] = 0.3281260132789612f;
            D_004D5980[2] = 1.0f;
            D_004D5980[3] = 0.3539060056209564f;
            D_004D5990[0] = 1.0f;
            D_004D5990[1] = 1.0f;
            D_004D5990[2] = 1.0f;
            D_004D5990[3] = 1.0f;
            D_004D59A0[0] = 1.0f;
            D_004D59A0[1] = 1.0f;
            D_004D59A0[2] = 1.0f;
            D_004D59A0[3] = 1.0f;
            D_004D59B0[0] = 1.0f;
            D_004D59B0[1] = 0.8613290190696716f;
            D_004D59B0[2] = 0.38124799728393555f;
            D_004D59B0[3] = 0.0f;
            D_004D59C0[0] = 1.0f;
            D_004D59C0[1] = 1.0f;
            D_004D59C0[2] = 1.0f;
            D_004D59C0[3] = 1.0f;
            D_004D59D0[0] = 1.0f;
            D_004D59D0[1] = 0.3244380056858063f;
            D_004D59D0[2] = 1.0f;
            D_004D59D0[3] = 0.0f;
            D_004D59E0[0] = 1.0f;
            D_004D59E0[1] = 1.0f;
            D_004D59E0[2] = 1.0f;
            D_004D59E0[3] = 1.0f;
            D_004D59F0[0] = 1.0f;
            D_004D59F0[1] = 1.0f;
            D_004D59F0[2] = 0.0f;
            D_004D59F0[3] = 0.0f;
            D_004D5A00[0] = 0.7007060050964355f;
            D_004D5A00[1] = 0.0f;
            D_004D5A00[2] = 0.0f;
            D_004D5A00[3] = 0.0f;
            D_004D5A10[0] = 1.0f;
            D_004D5A10[1] = 0.7828530073165894f;
            D_004D5A10[2] = 0.06140600144863129f;
            D_004D5A10[3] = 0.04146299883723259f;
            D_004D5A20[0] = 1.0f;
            D_004D5A20[1] = 0.32487601041793823f;
            D_004D5A20[2] = 1.0f;
            D_004D5A20[3] = 0.35280099511146545f;
            D_004D5A30[0] = 1.0f;
            D_004D5A30[1] = 1.0f;
            D_004D5A30[2] = 0.7977359890937805f;
            D_004D5A30[3] = 0.0f;
            D_004D5A40[0] = 1.0f;
            D_004D5A40[1] = 0.4937489926815033f;
            D_004D5A40[2] = 1.0f;
            D_004D5A40[3] = 0.49179700016975403f;
            D_004D5A50[0] = 1.0f;
            D_004D5A50[1] = 1.0f;
            D_004D5A50[2] = 1.0f;
            D_004D5A50[3] = 1.0f;
            D_004D5A60[0] = 1.0f;
            D_004D5A60[1] = 0.800000011920929f;
            D_004D5A60[2] = 1.0f;
            D_004D5A60[3] = 0.800000011920929f;
            D_004D5A70[0] = 1.0f;
            D_004D5A70[1] = 1.0f;
            D_004D5A70[2] = 0.0f;
            D_004D5A70[3] = 0.0f;
            D_004D5A80[0] = 1.0f;
            D_004D5A80[1] = 1.0f;
            D_004D5A80[2] = 0.0f;
            D_004D5A80[3] = 0.0f;
            D_004D5A90[0] = 1.0f;
            D_004D5A90[1] = 1.0f;
            D_004D5A90[2] = 0.0f;
            D_004D5A90[3] = 0.0f;
            D_004D5AA0[0] = 1.0f;
            D_004D5AA0[1] = 0.3273420035839081f;
            D_004D5AA0[2] = 1.0f;
            D_004D5AA0[3] = 0.35390499234199524f;
            D_004D5AB0[0] = 1.0f;
            D_004D5AB0[1] = 1.0f;
            D_004D5AB0[2] = 0.061000000685453415f;
            D_004D5AB0[3] = 0.039000000804662704f;
            D_004D5AC0[0] = 1.0f;
            D_004D5AC0[1] = 1.0f;
            D_004D5AC0[2] = 0.7999979853630066f;
            D_004D5AC0[3] = 0.0f;
            D_004D5AD0[0] = 1.0f;
            D_004D5AD0[1] = 0.9589840173721313f;
            D_004D5AD0[2] = 0.6143789887428284f;
            D_004D5AD0[3] = 0.0f;
            D_004D5AE0[0] = 1.0f;
            D_004D5AE0[1] = 0.9285150170326233f;
            D_004D5AE0[2] = 0.3399839997291565f;
            D_004D5AE0[3] = 0.0f;
            D_004D5AF0[0] = 1.0f;
            D_004D5AF0[1] = 0.8738250136375427f;
            D_004D5AF0[2] = 0.24931700527668f;
            D_004D5AF0[3] = 0.017578000202775f;
            D_004D5B00[0] = 1.0f;
            D_004D5B00[1] = 0.7843739986419678f;
            D_004D5B00[2] = 0.06132800132036209f;
            D_004D5B00[3] = 0.03945299983024597f;
            D_004D5B10[0] = 1.0f;
            D_004D5B10[1] = 0.0f;
            D_004D5B10[2] = 1.0f;
            D_004D5B10[3] = 0.0f;
            D_004D5B20[0] = 1.0f;
            D_004D5B20[1] = 1.0f;
            D_004D5B20[2] = 0.0f;
            D_004D5B20[3] = 0.0f;
            *(char*)&D_004A5530 = -1;
            D_004A5530 = (D_004A5530 & 0xFF) | 0xFFFFFF00u;
            D_004D5B30[0] = 1.0f;
            D_004D5B30[1] = 1.0f;
            D_004D5B30[2] = D_004D57C0[3];
            D_004D5B30[3] = D_004D57C0[3];
            D_004D5B40[0] = 1.0f;
            D_004D5B40[1] = 1.0f;
            D_004D5B40[2] = 0.5f;
            D_004D5B40[3] = D_004D57C0[3];
            D_004D5B50[0] = 1.0f;
            D_004D5B50[1] = 1.0f;
            D_004D5B50[2] = 1.0f;
            D_004D5B50[3] = D_004D57C0[3];
            D_004D5B60[0] = 1.0f;
            D_004D5B60[1] = 1.0f;
            D_004D5B60[2] = 1.0f;
            D_004D5B60[3] = 1.0f;
            D_004A553C = 0;
            D_004A5538 = 0;
            initE(D_004A5540, 0);
            initE(D_004A5548, 1);
            initE(D_004A5550, 2);
            initE(D_004A5558, 3);
            initE(D_004A5560, 4);
            initE(D_004A5568, 5);
            initE(D_004A5570, 6);
            initE(D_004A5578, 7);
            initE(D_004A5580, 8);
            initE(D_004A5588, 9);
            initE(D_004A5590, 10);
            initE(D_004A5598, 11);
            initE(D_004A55A0, 12);
            initE(D_004A55A8, 13);
            new (D_004D5B70) sWlAvElem[2048];
            new (D_004EDB70) sWlAvElem[64];
            new (D_004EE770) sWlFx[64];
            D_004A55AC = 6.283186f;
            new (D_004FA370) sWlRsm[8];
            D_004A55B4 = 0;
            D_004A55B0 = 0;
            D_004FAAF0[0] = 1.0f;
            D_004FAAF0[1] = 0.55f;
            D_004FAAF0[2] = 0.535f;
            D_004FAAF0[3] = 0.55f;
            __builtin_memcpy(D_004FAB00, D_004D5870, 16);
            D_004FAB10[0] = 1.0f;
            D_004FAB10[1] = 1.0f;
            D_004FAB10[2] = 1.0f;
            D_004FAB10[3] = 1.0f;
            __builtin_memcpy(D_004FAB20, D_004D5880, 16);
            D_004FAB30[0] = 1.0f;
            D_004FAB30[1] = 1.0f;
            D_004FAB30[2] = 1.0f;
            D_004FAB30[3] = 1.0f;
            __builtin_memcpy(D_004FAB40, D_004D5890, 16);
            D_004A55B8 = 320.0f;
            D_004A55BC = 240.0f;
            setCol(D_004FAB50, 1.0f, sWlV3(1.0f, 1.0f, 0.5f));
            __builtin_memcpy(D_004FAB60, D_004D57B0, 16);
            D_004FAB70[0] = 1.0f;
            D_004FAB70[1] = 1.0f;
            D_004FAB70[2] = 1.0f;
            D_004FAB70[3] = 0.0f;
            __builtin_memcpy(D_004FAB80, D_004D57C0, 16);
            D_004FAB90[0] = 1.0f;
            D_004FAB90[1] = 1.0f;
            D_004FAB90[2] = 1.0f;
            D_004FAB90[3] = 0.0f;
            __builtin_memcpy(D_004FABA0, D_004D57D0, 16);
            D_004FABB0[0] = 1.0f;
            D_004FABB0[1] = 1.0f;
            D_004FABB0[2] = 1.0f;
            D_004FABB0[3] = 0.0f;
            __builtin_memcpy(D_004FABC0, D_004D57E0, 16);
            D_004FABD0[0] = 1.0f;
            D_004FABD0[1] = 1.0f;
            D_004FABD0[2] = 1.0f;
            D_004FABD0[3] = 0.0f;
            __builtin_memcpy(D_004FABE0, D_004D57F0, 16);
            D_004FABF0[0] = 1.0f;
            D_004FABF0[1] = 1.0f;
            D_004FABF0[2] = 1.0f;
            D_004FABF0[3] = 0.0f;
            __builtin_memcpy(D_004FAC00, D_004D5800, 16);
            D_004FAC10[0] = 1.0f;
            D_004FAC10[1] = 1.0f;
            D_004FAC10[2] = 1.0f;
            D_004FAC10[3] = 0.0f;
            __builtin_memcpy(D_004FAC20, D_004D5810, 16);
            D_004FAC30[0] = 1.0f;
            D_004FAC30[1] = 1.0f;
            D_004FAC30[2] = 1.0f;
            D_004FAC30[3] = 0.0f;
            __builtin_memcpy(D_004FAC40, D_004D5820, 16);
            D_004FAC50[0] = 1.0f;
            D_004FAC50[1] = 1.0f;
            D_004FAC50[2] = 1.0f;
            D_004FAC50[3] = 0.0f;
            __builtin_memcpy(D_004FAC60, D_004D5830, 16);
            D_004FAC70[0] = 1.0f;
            D_004FAC70[1] = 1.0f;
            D_004FAC70[2] = 1.0f;
            D_004FAC70[3] = 0.0f;
            __builtin_memcpy(D_004FAC80, D_004D5840, 16);
        } else {
            if (D_004EE770 != 0) {
                sWlFx* p = D_004FA370_end;
                if (D_004EE770 != D_004FA370_end) {
                    do {
                        p--;
                        func_002D56B0(p, 0);
                    } while (p != D_004EE770);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7A50);
#ifdef SKIP_ASM
extern char D_004FF1A0[];

extern "C" void* func_002F7A50()
{
    return D_004FF1A0;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7A60);
#ifdef SKIP_ASM
extern "C" void func_002F7A60(void) {

}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7A68__FPv);
#ifdef SKIP_ASM
void* func_002F7A68(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7A70);
#ifdef SKIP_ASM
extern "C" int func_002F7A70(void *arg0) {
    return (*(int *)((char*)(arg0) + (4))) > 0;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7A80);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *func_002F7A80(long long *arg0, void *arg1) {
    *arg0 = (*(long long *)((char*)((((*(int *)((char*)(arg1) + (0xB4))) * 0x10) + (((*(int *)((char*)(arg1) + (8))) * 0x70) + (*(int *)((char*)(arg1) + (0xC)))))) + (-0x10)));
    return arg0;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7AB0);
#ifdef SKIP_ASM
extern "C" void func_002F7AB0(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7AB8);
#ifdef SKIP_ASM
extern "C" void func_002DCF28();

extern "C" void func_002F7AB8(void) {
    func_002DCF28();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7AD8);
#ifdef SKIP_ASM
extern "C" void func_002F7AD8(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7AE0);
#ifdef SKIP_ASM
extern "C" void func_002DAA78();

extern "C" void func_002F7AE0(void) {
    func_002DAA78();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7B00);
#ifdef SKIP_ASM
extern "C" void func_002F7B00(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7B08);
#ifdef SKIP_ASM
extern "C" void func_002E8560();

extern "C" void func_002F7B08(void) {
    func_002E8560();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7B28);
#ifdef SKIP_ASM
extern "C" void func_002F7B28(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7B30);
#ifdef SKIP_ASM
extern "C" void func_002D4BE0();

extern "C" void func_002F7B30(void) {
    func_002D4BE0();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7B50);
#ifdef SKIP_ASM
extern "C" void func_002F7B50(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7B58);
#ifdef SKIP_ASM
extern "C" void func_002E3930();

extern "C" void func_002F7B58(void) {
    func_002E3930();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7B78);
#ifdef SKIP_ASM
extern "C" void func_002F7B78(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7B80);
#ifdef SKIP_ASM
extern "C" void func_002EF6A0();

extern "C" void func_002F7B80(void) {
    func_002EF6A0();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BA0);
#ifdef SKIP_ASM
extern "C" void func_002F7BA0(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BA8);
#ifdef SKIP_ASM
extern "C" void func_002F1148();

extern "C" void func_002F7BA8(void) {
    func_002F1148();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BC8);
#ifdef SKIP_ASM
extern "C" int func_002F7BC8(int arg0) {
    return arg0 + 4;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BD0);
#ifdef SKIP_ASM
extern "C" void func_002F7BD0(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x6490))) = arg1;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BD8);
#ifdef SKIP_ASM
extern "C" void func_002F7BD8(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x6494))) = arg1;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BE0);
#ifdef SKIP_ASM
extern "C" int func_002F7BE0(void* self)
{
    return *(unsigned int*)self == 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BF0);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F7BF0(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7C20__FPv);
#ifdef SKIP_ASM
void func_002F7C20(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7C28__FPv);
#ifdef SKIP_ASM
void func_002F7C28(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7C30__FPv);
#ifdef SKIP_ASM
void func_002F7C30(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7C38);
#ifdef SKIP_ASM
extern void* D_00488648[];
extern "C" void* func_002D6410(void* self);

extern "C" void* func_002F7C38(void* self)
{
    *(void***)self = D_00488648;
    func_002D6410(self);
    return self;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7C70);
#ifdef SKIP_ASM
extern int D_004A3A30;

extern "C" int *func_002F7C70(void) {
    return &D_004A3A30;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7C80);
#ifdef SKIP_ASM
extern int D_004A3A34;

extern "C" int *func_002F7C80(void) {
    return &D_004A3A34;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7C90);
#ifdef SKIP_ASM
extern int D_004A3A38;

extern "C" int *func_002F7C90(void) {
    return &D_004A3A38;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7CA0);
#ifdef SKIP_ASM
extern int D_004A3A3C;

extern "C" int *func_002F7CA0(void) {
    return &D_004A3A3C;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7CB0);
#ifdef SKIP_ASM
extern int D_004A3A40;

extern "C" int *func_002F7CB0(void) {
    return &D_004A3A40;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7CC0);
#ifdef SKIP_ASM
extern int D_004A3A44;

extern "C" int *func_002F7CC0(void) {
    return &D_004A3A44;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7CD0);
#ifdef SKIP_ASM
extern int D_004A3A48;

extern "C" int *func_002F7CD0(void) {
    return &D_004A3A48;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7CE0);
#ifdef SKIP_ASM
extern int D_004A3A4C;

extern "C" int *func_002F7CE0(void) {
    return &D_004A3A4C;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7CF0);
#ifdef SKIP_ASM
extern int D_004A3A50;

extern "C" int *func_002F7CF0(void) {
    return &D_004A3A50;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D00);
#ifdef SKIP_ASM
extern int D_004A3A54;

extern "C" int *func_002F7D00(void) {
    return &D_004A3A54;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D10);
#ifdef SKIP_ASM
extern int D_004A3A58;

extern "C" int *func_002F7D10(void) {
    return &D_004A3A58;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D20);
#ifdef SKIP_ASM
extern int D_004A3A5C;

extern "C" int *func_002F7D20(void) {
    return &D_004A3A5C;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D30);
#ifdef SKIP_ASM
extern int D_004A3A60;

extern "C" int *func_002F7D30(void) {
    return &D_004A3A60;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D40);
#ifdef SKIP_ASM
extern int D_004A3A64;

extern "C" int *func_002F7D40(void) {
    return &D_004A3A64;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D50);
#ifdef SKIP_ASM
extern int D_004A3A68;

extern "C" int *func_002F7D50(void) {
    return &D_004A3A68;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D60);
#ifdef SKIP_ASM
extern int D_004A3A6C;

extern "C" int *func_002F7D60(void) {
    return &D_004A3A6C;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D70);
#ifdef SKIP_ASM
extern int D_004A3A70;

extern "C" int *func_002F7D70(void) {
    return &D_004A3A70;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D80);
#ifdef SKIP_ASM
extern int D_004A3A74;

extern "C" int *func_002F7D80(void) {
    return &D_004A3A74;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7D90);
#ifdef SKIP_ASM
extern int D_004A3A78;

extern "C" int *func_002F7D90(void) {
    return &D_004A3A78;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7DA0);
#ifdef SKIP_ASM
extern int D_004A3A7C;

extern "C" int *func_002F7DA0(void) {
    return &D_004A3A7C;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7DB0);
#ifdef SKIP_ASM
extern int D_004A3A80;

extern "C" int *func_002F7DB0(void) {
    return &D_004A3A80;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7DC0);
#ifdef SKIP_ASM
extern int D_004A3A84;

extern "C" int *func_002F7DC0(void) {
    return &D_004A3A84;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7DD0);
#ifdef SKIP_ASM
extern int D_004A3A88;

extern "C" int *func_002F7DD0(void) {
    return &D_004A3A88;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7DE0);
#ifdef SKIP_ASM
extern int D_004A3A8C;

extern "C" int *func_002F7DE0(void) {
    return &D_004A3A8C;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7DF0);
#ifdef SKIP_ASM
extern int D_004A3A90;

extern "C" int *func_002F7DF0(void) {
    return &D_004A3A90;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E00);
#ifdef SKIP_ASM
extern int D_004A3A94;

extern "C" int *func_002F7E00(void) {
    return &D_004A3A94;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E10);
#ifdef SKIP_ASM
extern int D_004A3A98;

extern "C" int *func_002F7E10(void) {
    return &D_004A3A98;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E20);
#ifdef SKIP_ASM
extern int D_004A3A9C;

extern "C" int *func_002F7E20(void) {
    return &D_004A3A9C;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E30);
#ifdef SKIP_ASM
extern int D_004A3AA0;

extern "C" int *func_002F7E30(void) {
    return &D_004A3AA0;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E40);
#ifdef SKIP_ASM
extern int D_004A3AA4;

extern "C" int *func_002F7E40(void) {
    return &D_004A3AA4;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E50);
#ifdef SKIP_ASM
extern int D_004A3AA8;

extern "C" int *func_002F7E50(void) {
    return &D_004A3AA8;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E60);
#ifdef SKIP_ASM
extern int D_004A3AAC;

extern "C" int *func_002F7E60(void) {
    return &D_004A3AAC;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E70);
#ifdef SKIP_ASM
extern "C" void func_002D9B40();

extern "C" void func_002F7E70(void) {
    func_002D9B40();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E90);
#ifdef SKIP_ASM
extern int D_004A3AB8;

extern "C" int func_002F7E90(void) {
    return D_004A3AB8;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7E98);
#ifdef SKIP_ASM
extern int D_004A3AC4;

extern "C" int func_002F7E98(void) {
    return D_004A3AC4;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7EA0);
#ifdef SKIP_ASM
extern "C" void func_002F7EA0(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7EA8);
#ifdef SKIP_ASM
extern "C" void func_002E6640();

extern "C" void func_002F7EA8(void) {
    func_002E6640();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7EC8);
#ifdef SKIP_ASM
extern "C" void func_002F7EC8(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7ED0);
#ifdef SKIP_ASM
extern "C" void func_002EADC0__FPv();

extern "C" void func_002F7ED0(void) {
    func_002EADC0__FPv();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7EF0);
#ifdef SKIP_ASM
extern "C" void func_002F7EF0(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7EF8);
#ifdef SKIP_ASM
extern "C" void func_002F7EF8(void) {

}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7F00);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F7F00(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7F30);
#ifdef SKIP_ASM
extern void* D_00488308[];
void operator_delete(int*);

extern "C" void func_002F7F30(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_00488308;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7F60);
#ifdef SKIP_ASM
extern void* D_00488308[];
void operator_delete(int*);

extern "C" void func_002F7F60(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_00488308;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7F90);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F7F90(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7FC0);
#ifdef SKIP_ASM
extern "C" int func_002F7FC0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xA0)));
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7FC8);
#ifdef SKIP_ASM
extern "C" void func_002F7FC8(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7FD0);
#ifdef SKIP_ASM
extern "C" void func_002F7FD0(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7FD8);
#ifdef SKIP_ASM
extern "C" int func_002F7FD8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x44)));
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7FE0);
#ifdef SKIP_ASM
extern "C" int func_002F7FE0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x64)));
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7FE8);
#ifdef SKIP_ASM
extern "C" int func_002F7FE8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x78)));
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7FF0__FPv);
#ifdef SKIP_ASM
int func_002F7FF0(void* self)
{
    return *(int*)((char*)self + 0x74);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7FF8);
#ifdef SKIP_ASM
extern "C" int func_002F7FF8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x60)));
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8000);
#ifdef SKIP_ASM
extern void* D_004880E0[];
extern "C" void func_002E4D30(void* p);

extern "C" void func_002F8000(void* self, int flags)
{
    *(void***)self = D_004880E0;
    if (flags & 1) {
        func_002E4D30(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8030);
#ifdef SKIP_ASM
extern void* D_004880E0[];
extern "C" void func_002E4D30(void* p);

extern "C" void func_002F8030(void* self, int flags)
{
    *(void***)self = D_004880E0;
    if (flags & 1) {
        func_002E4D30(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8060);
#ifdef SKIP_ASM
struct sLightVec_002F8060 { float x, y, z, w; };

extern void* D_004880C0[];
void func_002E4D70(void* self);

extern "C" void* func_002F8060(void* self, sLightVec_002F8060 v)
{
    func_002E4D70(self);
    *(void***)self = D_004880C0;
    *(sLightVec_002F8060*)((char*)self + 4) = v;
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F80D8);
#ifdef SKIP_ASM
extern void* D_004880E0[];
extern "C" void func_002E4D30(void* p);

extern "C" void func_002F80D8(void* self, int flags)
{
    *(void***)self = D_004880E0;
    if (flags & 1) {
        func_002E4D30(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8108);
#ifdef SKIP_ASM
extern void* D_004880E0[];
extern "C" void func_002E4D30(void* p);

extern "C" void func_002F8108(void* self, int flags)
{
    *(void***)self = D_004880E0;
    if (flags & 1) {
        func_002E4D30(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8138);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F8138(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8168__FPv);
#ifdef SKIP_ASM
void func_002F8168(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8170);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_002F8170(unsigned long* bits, int n)
{
    unsigned long mask = (unsigned long)1 << n;
    return (*bits & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8188);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F8188(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* func_003546C8(void* self);

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F81B8__FPv);
#ifdef SKIP_ASM
void* func_002F81B8(void* self)
{
    return func_003546C8(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F81D8);
#ifdef SKIP_ASM
extern "C" int func_002F81D8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F81E0);
#ifdef SKIP_ASM
extern "C" void func_002F81E0(int *arg0) {
    *arg0 = 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F81F0);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F81F0(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8220);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F8220(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8250);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F8250(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8280);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F8280(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F82B0);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F82B0(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F82E0);
#ifdef SKIP_ASM
extern void* D_00488680[];
void operator_delete(int*);

extern "C" void func_002F82E0(void* self, int flags)
{
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8310);
#ifdef SKIP_ASM
extern void* D_00487DA0[];
void operator_delete(int*);

extern "C" void func_002F8310(void* self, int flags)
{
    *(void***)((char*)self + 0x44) = D_00487DA0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8340);
#ifdef SKIP_ASM
extern void* D_00487D78[];
void operator_delete(int*);

extern "C" void func_002F8340(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_00487D78;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* func_002F6B88(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002F8370__FPv);
#ifdef SKIP_ASM
void* func_002F8370(void* self)
{
    return func_002F6B88(1, 0xffff);
}
#endif

extern "C" void* func_002F6B88(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002F8390__FPv);
#ifdef SKIP_ASM
void* func_002F8390(void* self)
{
    return func_002F6B88(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F83B0);
#ifdef SKIP_ASM
struct sLightSet_83B0 {
    int unk_0;
    int unk_4;
    float unk_8;
    float unk_C;
    float unk_10;
    float unk_14;
    float unk_18;
    float unk_1C;
    float unk_20;
    int unk_24;
    int unk_28;
    int unk_2C;
    float unk_30;
    int unk_34;
    int unk_38;
    int unk_3C;
    float unk_40;
    int unk_44;
    int unk_48;
    int unk_4C;
    int unk_50;
    float unk_54;
    int unk_58;
    int unk_5C;
    float unk_60;
    float unk_64;
    int unk_68;
    int unk_6C;
    float unk_70;
    float unk_74;
    int unk_78;
    int unk_7C;
    float unk_80;
    float unk_84;
    float unk_88;
    float unk_8C;
    float unk_90;
    float unk_94;
    float unk_98;
    float unk_9C;
    float unk_A0;
    float unk_A4;
    float unk_A8;
    int unk_AC;
    int unk_B0;
    int unk_B4;
    int unk_B8;
    int unk_BC;
    int unk_C0;
    int unk_C4;
    int unk_C8;
    float unk_CC;
    int unk_D0;
    float unk_D4;
    float unk_D8;
    int unk_DC;
    int unk_E0;
    float unk_E4;
    float unk_E8;
    float unk_EC;
    float unk_F0;
    int unk_F4;
    float unk_F8;
    float unk_FC;
    float unk_100;
    float unk_104;
    float unk_108;
    float unk_10C;
};

extern sLightSet_83B0* D_004A3BB0;

extern "C" void func_002F83B0(void)
{
    sLightSet_83B0* s = D_004A3BB0;
    s[0].unk_0 = 1;
    s[0].unk_4 = 0;
    s[0].unk_8 = -1.0f;
    s[0].unk_C = 1.0f;
    s[0].unk_10 = 40.0f;
    s[0].unk_14 = 1.0f;
    s[0].unk_18 = 0.0f;
    s[0].unk_1C = 0.0f;
    s[0].unk_20 = 0.0f;
    s[0].unk_24 = 0;
    s[0].unk_28 = 0;
    s[0].unk_2C = 0;
    s[0].unk_30 = 0.0f;
    s[0].unk_34 = 0;
    s[0].unk_38 = 0;
    s[0].unk_3C = 0;
    s[0].unk_40 = 0.0f;
    s[0].unk_44 = 0;
    s[0].unk_48 = 0;
    s[0].unk_4C = 0;
    s[0].unk_50 = 0;
    s[0].unk_54 = 150.0f;
    s[0].unk_58 = 0;
    s[0].unk_5C = 0;
    s[0].unk_60 = 0.0f;
    s[0].unk_64 = 150.0f;
    s[0].unk_68 = 0;
    s[0].unk_6C = 0;
    s[0].unk_70 = 0.0f;
    s[0].unk_74 = 150.0f;
    s[0].unk_78 = 0;
    s[0].unk_7C = 0;
    s[0].unk_80 = 0.0f;
    s[0].unk_84 = 1.0f;
    s[0].unk_88 = 1.0f;
    s[0].unk_8C = 1.0f;
    s[0].unk_90 = 1.0f;
    s[0].unk_94 = 1.0f;
    s[0].unk_98 = 1.0f;
    s[0].unk_9C = 1.0f;
    s[0].unk_A0 = 1.0f;
    s[0].unk_A4 = 0.0f;
    s[0].unk_A8 = 0.0f;
    s[0].unk_AC = 0;
    s[0].unk_B0 = 0;
    s[0].unk_B4 = 0;
    s[0].unk_B8 = 0;
    s[0].unk_BC = 0;
    s[0].unk_C0 = 0;
    s[0].unk_C8 = 1;
    s[0].unk_CC = 20.0f;
    s[0].unk_C4 = 5;
    s[0].unk_D0 = 1;
    s[0].unk_D4 = 20.0f;
    s[0].unk_D8 = 1.0f;
    s[0].unk_DC = 0;
    s[0].unk_E0 = 0;
    s[0].unk_E4 = 0.0f;
    s[0].unk_E8 = 0.0f;
    s[0].unk_EC = 0.0f;
    s[0].unk_F0 = 0.0f;
    s[0].unk_F4 = 0;
    s[0].unk_F8 = 0.0f;
    s[0].unk_FC = 0.0f;
    s[0].unk_100 = 140.0f;
    s[0].unk_104 = 1.0f;
    s[0].unk_108 = 1.0f;
    s[0].unk_10C = 1.0f;
    s[1].unk_0 = 1;
    s[1].unk_4 = 0;
    s[1].unk_8 = -1.0f;
    s[1].unk_C = 1.5f;
    s[1].unk_10 = 70.0f;
    s[1].unk_14 = 0.25f;
    s[1].unk_18 = 50.0f;
    s[1].unk_1C = 0.0f;
    s[1].unk_20 = 0.0f;
    s[1].unk_24 = 0;
    s[1].unk_28 = 0;
    s[1].unk_2C = 0;
    s[1].unk_30 = 10.0f;
    s[1].unk_34 = 0;
    s[1].unk_38 = 0;
    s[1].unk_3C = 0;
    s[1].unk_40 = 10.0f;
    s[1].unk_44 = 0;
    s[1].unk_48 = 0;
    s[1].unk_4C = 0;
    s[1].unk_50 = 0;
    s[1].unk_54 = 100.0f;
    s[1].unk_58 = 0;
    s[1].unk_5C = 0;
    s[1].unk_60 = 0.0f;
    s[1].unk_64 = 100.0f;
    s[1].unk_68 = 0;
    s[1].unk_6C = 0;
    s[1].unk_70 = 0.0f;
    s[1].unk_74 = 100.0f;
    s[1].unk_78 = 0;
    s[1].unk_7C = 0;
    s[1].unk_80 = 1690.0f;
    s[1].unk_84 = 1.0f;
    s[1].unk_88 = 1.0f;
    s[1].unk_8C = 1.0f;
    s[1].unk_90 = 1.0f;
    s[1].unk_94 = 0.0f;
    s[1].unk_98 = 1.0f;
    s[1].unk_9C = 0.0f;
    s[1].unk_A0 = 0.0f;
    s[1].unk_A4 = 0.0f;
    s[1].unk_A8 = 0.0f;
    s[1].unk_AC = 0;
    s[1].unk_B0 = 0;
    s[1].unk_B4 = 0;
    s[1].unk_B8 = 0;
    s[1].unk_BC = 0;
    s[1].unk_C0 = 0;
    s[1].unk_C8 = 0;
    s[1].unk_CC = 0.0f;
    s[1].unk_C4 = 24;
    s[1].unk_D0 = 1;
    s[1].unk_D4 = 20.0f;
    s[1].unk_D8 = 1.0f;
    s[1].unk_DC = 0;
    s[1].unk_E0 = 0;
    s[1].unk_E4 = 3.0f;
    s[1].unk_E8 = 0.0f;
    s[1].unk_EC = 3.0f;
    s[1].unk_F0 = 0.0f;
    s[1].unk_F4 = 0;
    s[1].unk_F8 = 0.0f;
    s[1].unk_FC = 0.0f;
    s[1].unk_100 = 140.0f;
    s[1].unk_104 = 1.0f;
    s[1].unk_108 = 1.0f;
    s[1].unk_10C = 1.0f;
    s[2].unk_0 = 1;
    s[2].unk_4 = 0;
    s[2].unk_8 = -1.0f;
    s[2].unk_C = 1.0f;
    s[2].unk_10 = 30.0f;
    s[2].unk_14 = 0.16501f;
    s[2].unk_18 = 0.0f;
    s[2].unk_1C = 0.0f;
    s[2].unk_20 = 0.0f;
    s[2].unk_24 = 0;
    s[2].unk_28 = 0;
    s[2].unk_2C = 0;
    s[2].unk_30 = 0.0f;
    s[2].unk_34 = 0;
    s[2].unk_38 = 0;
    s[2].unk_3C = 0;
    s[2].unk_40 = 0.0f;
    s[2].unk_44 = 0;
    s[2].unk_48 = 0;
    s[2].unk_4C = 0;
    s[2].unk_50 = 0;
    s[2].unk_54 = 50.0f;
    s[2].unk_58 = 0;
    s[2].unk_5C = 0;
    s[2].unk_60 = 0.0f;
    s[2].unk_64 = 100.0f;
    s[2].unk_68 = 0;
    s[2].unk_6C = 0;
    s[2].unk_70 = 0.0f;
    s[2].unk_74 = 100.0f;
    s[2].unk_78 = 0;
    s[2].unk_7C = 0;
    s[2].unk_80 = 655.0f;
    s[2].unk_84 = 1.0f;
    s[2].unk_88 = 1.0f;
    s[2].unk_8C = 1.0f;
    s[2].unk_90 = 1.0f;
    s[2].unk_94 = 0.0f;
    s[2].unk_98 = 1.0f;
    s[2].unk_9C = 0.0f;
    s[2].unk_A0 = 0.0f;
    s[2].unk_A4 = 0.0f;
    s[2].unk_A8 = 0.0f;
    s[2].unk_AC = 0;
    s[2].unk_B0 = 0;
    s[2].unk_B4 = 0;
    s[2].unk_B8 = 0;
    s[2].unk_BC = 0;
    s[2].unk_C0 = 0;
    s[2].unk_C8 = 0;
    s[2].unk_CC = 0.0f;
    s[2].unk_C4 = 24;
    s[2].unk_D0 = 1;
    s[2].unk_D4 = 20.0f;
    s[2].unk_D8 = 1.0f;
    s[2].unk_DC = 0;
    s[2].unk_E0 = 0;
    s[2].unk_E4 = 10.0f;
    s[2].unk_E8 = 0.0f;
    s[2].unk_EC = 0.0f;
    s[2].unk_F0 = 0.0f;
    s[2].unk_F4 = 0;
    s[2].unk_F8 = 0.0f;
    s[2].unk_FC = 0.0f;
    s[2].unk_100 = 140.0f;
    s[2].unk_104 = 1.0f;
    s[2].unk_108 = 1.0f;
    s[2].unk_10C = 1.0f;
    s[3].unk_0 = 1;
    s[3].unk_4 = 0;
    s[3].unk_8 = -1.0f;
    s[3].unk_C = 1.0f;
    s[3].unk_10 = 55.55201f;
    s[3].unk_14 = 0.1f;
    s[3].unk_18 = 0.0f;
    s[3].unk_1C = 0.0f;
    s[3].unk_20 = 0.0f;
    s[3].unk_24 = 0;
    s[3].unk_28 = 0;
    s[3].unk_2C = 0;
    s[3].unk_30 = 0.0f;
    s[3].unk_34 = 0;
    s[3].unk_38 = 0;
    s[3].unk_3C = 0;
    s[3].unk_40 = 0.0f;
    s[3].unk_44 = 0;
    s[3].unk_48 = 0;
    s[3].unk_4C = 0;
    s[3].unk_50 = 0;
    s[3].unk_54 = 50.0f;
    s[3].unk_58 = 0;
    s[3].unk_5C = 0;
    s[3].unk_60 = 0.0f;
    s[3].unk_64 = 500.0f;
    s[3].unk_68 = 0;
    s[3].unk_6C = 0;
    s[3].unk_70 = 0.0f;
    s[3].unk_74 = 500.0f;
    s[3].unk_78 = 0;
    s[3].unk_7C = 0;
    s[3].unk_80 = -200.0f;
    s[3].unk_84 = 1.0f;
    s[3].unk_88 = 1.0f;
    s[3].unk_8C = 0.2f;
    s[3].unk_90 = 0.2f;
    s[3].unk_94 = 0.0f;
    s[3].unk_98 = 1.0f;
    s[3].unk_9C = 0.2f;
    s[3].unk_A0 = 0.2f;
    s[3].unk_A4 = 0.0f;
    s[3].unk_A8 = 0.0f;
    s[3].unk_AC = 0;
    s[3].unk_B0 = 0;
    s[3].unk_B4 = 0;
    s[3].unk_B8 = 0;
    s[3].unk_BC = 0;
    s[3].unk_C0 = 0;
    s[3].unk_C8 = 0;
    s[3].unk_CC = 0.0f;
    s[3].unk_C4 = 24;
    s[3].unk_D0 = 1;
    s[3].unk_D4 = 20.0f;
    s[3].unk_D8 = 1.0f;
    s[3].unk_DC = 0;
    s[3].unk_E0 = 0;
    s[3].unk_E4 = 10.0f;
    s[3].unk_E8 = 0.0f;
    s[3].unk_EC = 0.0f;
    s[3].unk_F0 = 0.0f;
    s[3].unk_F4 = 0;
    s[3].unk_F8 = 0.0f;
    s[3].unk_FC = 0.0f;
    s[3].unk_100 = 140.0f;
    s[3].unk_104 = 1.0f;
    s[3].unk_108 = 1.0f;
    s[3].unk_10C = 1.0f;
    s[4].unk_0 = 2;
    s[4].unk_4 = 0;
    s[4].unk_8 = -1.0f;
    s[4].unk_C = 1.0f;
    s[4].unk_10 = 100.0f;
    s[4].unk_14 = 0.4f;
    s[4].unk_18 = 0.0f;
    s[4].unk_1C = 0.0f;
    s[4].unk_20 = 0.0f;
    s[4].unk_24 = 0;
    s[4].unk_28 = 0;
    s[4].unk_2C = 0;
    s[4].unk_30 = 0.0f;
    s[4].unk_34 = 0;
    s[4].unk_38 = 0;
    s[4].unk_3C = 0;
    s[4].unk_40 = 0.0f;
    s[4].unk_44 = 0;
    s[4].unk_48 = 0;
    s[4].unk_4C = 0;
    s[4].unk_50 = 0;
    s[4].unk_54 = 100.0f;
    s[4].unk_58 = 0;
    s[4].unk_5C = 0;
    s[4].unk_60 = 0.0f;
    s[4].unk_64 = 100.0f;
    s[4].unk_68 = 0;
    s[4].unk_6C = 0;
    s[4].unk_70 = 0.0f;
    s[4].unk_74 = 100.0f;
    s[4].unk_78 = 0;
    s[4].unk_7C = 0;
    s[4].unk_80 = 500.0f;
    s[4].unk_84 = 1.0f;
    s[4].unk_88 = 1.0f;
    s[4].unk_8C = 1.0f;
    s[4].unk_90 = 1.0f;
    s[4].unk_94 = 0.0f;
    s[4].unk_98 = 1.0f;
    s[4].unk_9C = 0.0f;
    s[4].unk_A0 = 0.0f;
    s[4].unk_A4 = 0.0f;
    s[4].unk_A8 = 0.0f;
    s[4].unk_AC = 0;
    s[4].unk_B0 = 0;
    s[4].unk_B4 = 0;
    s[4].unk_B8 = 0;
    s[4].unk_BC = 0;
    s[4].unk_C0 = 0;
    s[4].unk_C8 = 0;
    s[4].unk_CC = 40.0f;
    s[4].unk_C4 = 24;
    s[4].unk_D0 = 1;
    s[4].unk_D4 = 20.0f;
    s[4].unk_D8 = 1.25f;
    s[4].unk_DC = 0;
    s[4].unk_E0 = 0;
    s[4].unk_E4 = 0.0f;
    s[4].unk_E8 = 8.0f;
    s[4].unk_EC = 0.0f;
    s[4].unk_F0 = 0.0f;
    s[4].unk_F4 = 0;
    s[4].unk_F8 = 0.0f;
    s[4].unk_FC = 0.0f;
    s[4].unk_100 = 140.0f;
    s[4].unk_104 = 1.0f;
    s[4].unk_108 = 1.0f;
    s[4].unk_10C = 1.0f;
    s[5].unk_0 = 1;
    s[5].unk_4 = 0;
    s[5].unk_8 = -1.0f;
    s[5].unk_C = 1.5f;
    s[5].unk_10 = 10.0f;
    s[5].unk_14 = 0.5f;
    s[5].unk_18 = 0.0f;
    s[5].unk_1C = 0.0f;
    s[5].unk_20 = 0.0f;
    s[5].unk_24 = 0;
    s[5].unk_28 = 0;
    s[5].unk_2C = 0;
    s[5].unk_30 = 0.0f;
    s[5].unk_34 = 0;
    s[5].unk_38 = 0;
    s[5].unk_3C = 0;
    s[5].unk_40 = 0.0f;
    s[5].unk_44 = 0;
    s[5].unk_48 = 0;
    s[5].unk_4C = 0;
    s[5].unk_50 = 0;
    s[5].unk_54 = 40.0f;
    s[5].unk_58 = 0;
    s[5].unk_5C = 0;
    s[5].unk_60 = 0.0f;
    s[5].unk_64 = 40.0f;
    s[5].unk_68 = 0;
    s[5].unk_6C = 0;
    s[5].unk_70 = 0.0f;
    s[5].unk_74 = 40.0f;
    s[5].unk_78 = 0;
    s[5].unk_7C = 0;
    s[5].unk_80 = 500.0f;
    s[5].unk_84 = 0.999f;
    s[5].unk_88 = 0.3995f;
    s[5].unk_8C = 0.4f;
    s[5].unk_90 = 0.4f;
    s[5].unk_94 = 0.0f;
    s[5].unk_98 = 0.09875f;
    s[5].unk_9C = 0.115f;
    s[5].unk_A0 = 0.1075f;
    s[5].unk_A4 = 0.0f;
    s[5].unk_A8 = 0.0f;
    s[5].unk_AC = 0;
    s[5].unk_B0 = 0;
    s[5].unk_B4 = 0;
    s[5].unk_B8 = 0;
    s[5].unk_BC = 0;
    s[5].unk_C0 = 0;
    s[5].unk_C8 = 1;
    s[5].unk_CC = 120.0f;
    s[5].unk_C4 = 25;
    s[5].unk_D0 = 1;
    s[5].unk_D4 = 20.0f;
    s[5].unk_D8 = 0.8f;
    s[5].unk_DC = 0;
    s[5].unk_E0 = 0;
    s[5].unk_E4 = 0.0f;
    s[5].unk_E8 = 7.0f;
    s[5].unk_EC = 0.0f;
    s[5].unk_F0 = 0.0f;
    s[5].unk_F4 = 0;
    s[5].unk_F8 = 0.0f;
    s[5].unk_FC = 0.0f;
    s[5].unk_100 = 140.0f;
    s[5].unk_104 = 1.0f;
    s[5].unk_108 = 1.0f;
    s[5].unk_10C = 1.0f;
    s[6].unk_0 = 2;
    s[6].unk_4 = 0;
    s[6].unk_8 = -1.0f;
    s[6].unk_C = 1.0f;
    s[6].unk_10 = 24.0f;
    s[6].unk_14 = 0.1f;
    s[6].unk_18 = 6.0f;
    s[6].unk_1C = 0.1f;
    s[6].unk_20 = 0.09f;
    s[6].unk_24 = 0;
    s[6].unk_28 = 0;
    s[6].unk_2C = 0;
    s[6].unk_30 = 0.0f;
    s[6].unk_34 = 0;
    s[6].unk_38 = 0;
    s[6].unk_3C = 0;
    s[6].unk_40 = 0.0f;
    s[6].unk_44 = 0;
    s[6].unk_48 = 0;
    s[6].unk_4C = 0;
    s[6].unk_50 = 0;
    s[6].unk_54 = 0.0f;
    s[6].unk_58 = 0;
    s[6].unk_5C = 0;
    s[6].unk_60 = 0.0f;
    s[6].unk_64 = 0.0f;
    s[6].unk_68 = 0;
    s[6].unk_6C = 0;
    s[6].unk_70 = 0.0f;
    s[6].unk_74 = 0.0f;
    s[6].unk_78 = 0;
    s[6].unk_7C = 0;
    s[6].unk_80 = 0.0f;
    s[6].unk_84 = 1.0f;
    s[6].unk_88 = 0.045f;
    s[6].unk_8C = 0.00875f;
    s[6].unk_90 = 1.0f;
    s[6].unk_94 = 0.0f;
    s[6].unk_98 = 1.0f;
    s[6].unk_9C = 0.96675f;
    s[6].unk_A0 = 1.0f;
    s[6].unk_A4 = 0.5f;
    s[6].unk_A8 = 0.0f;
    s[6].unk_AC = 0;
    s[6].unk_B0 = 0;
    s[6].unk_B4 = 0;
    s[6].unk_B8 = 0;
    s[6].unk_BC = 0;
    s[6].unk_C0 = 0;
    s[6].unk_C8 = 0;
    s[6].unk_CC = 0.0f;
    s[6].unk_C4 = 5;
    s[6].unk_D0 = 1;
    s[6].unk_D4 = 20.0f;
    s[6].unk_D8 = 1.2f;
    s[6].unk_DC = 0;
    s[6].unk_E0 = 0;
    s[6].unk_E4 = -21.5f;
    s[6].unk_E8 = -13.75f;
    s[6].unk_EC = -21.5f;
    s[6].unk_F0 = -372.75f;
    s[6].unk_F4 = 0;
    s[6].unk_F8 = -3.0f;
    s[6].unk_FC = 0.0f;
    s[6].unk_100 = 140.0f;
    s[6].unk_104 = 1.0f;
    s[6].unk_108 = 1.0f;
    s[6].unk_10C = 0.0f;
    s[7].unk_0 = 2;
    s[7].unk_4 = 0;
    s[7].unk_8 = -1.0f;
    s[7].unk_C = 1.0f;
    s[7].unk_10 = 24.0f;
    s[7].unk_14 = 0.1f;
    s[7].unk_18 = 6.0f;
    s[7].unk_1C = 0.1f;
    s[7].unk_20 = 0.09f;
    s[7].unk_24 = 0;
    s[7].unk_28 = 0;
    s[7].unk_2C = 0;
    s[7].unk_30 = 0.0f;
    s[7].unk_34 = 0;
    s[7].unk_38 = 0;
    s[7].unk_3C = 0;
    s[7].unk_40 = 0.0f;
    s[7].unk_44 = 0;
    s[7].unk_48 = 0;
    s[7].unk_4C = 0;
    s[7].unk_50 = 0;
    s[7].unk_54 = 0.0f;
    s[7].unk_58 = 0;
    s[7].unk_5C = 0;
    s[7].unk_60 = 0.0f;
    s[7].unk_64 = 0.0f;
    s[7].unk_68 = 0;
    s[7].unk_6C = 0;
    s[7].unk_70 = 0.0f;
    s[7].unk_74 = 0.0f;
    s[7].unk_78 = 0;
    s[7].unk_7C = 0;
    s[7].unk_80 = 0.0f;
    s[7].unk_84 = 1.0f;
    s[7].unk_88 = 0.045f;
    s[7].unk_8C = 0.00875f;
    s[7].unk_90 = 1.0f;
    s[7].unk_94 = 0.0f;
    s[7].unk_98 = 1.0f;
    s[7].unk_9C = 0.96675f;
    s[7].unk_A0 = 1.0f;
    s[7].unk_A4 = 0.5f;
    s[7].unk_A8 = 0.0f;
    s[7].unk_AC = 0;
    s[7].unk_B0 = 0;
    s[7].unk_B4 = 0;
    s[7].unk_B8 = 0;
    s[7].unk_BC = 0;
    s[7].unk_C0 = 0;
    s[7].unk_C8 = 0;
    s[7].unk_CC = 0.0f;
    s[7].unk_C4 = 5;
    s[7].unk_D0 = 1;
    s[7].unk_D4 = 20.0f;
    s[7].unk_D8 = 1.2f;
    s[7].unk_DC = 0;
    s[7].unk_E0 = 0;
    s[7].unk_E4 = -21.5f;
    s[7].unk_E8 = 16.25f;
    s[7].unk_EC = -21.5f;
    s[7].unk_F0 = -372.75f;
    s[7].unk_F4 = 0;
    s[7].unk_F8 = -3.0f;
    s[7].unk_FC = 0.0f;
    s[7].unk_100 = 140.0f;
    s[7].unk_104 = 1.0f;
    s[7].unk_108 = 1.0f;
    s[7].unk_10C = 54.5f;
    s[8].unk_0 = 3;
    s[8].unk_4 = 0;
    s[8].unk_8 = -1.0f;
    s[8].unk_C = 1.0f;
    s[8].unk_10 = 6.97601f;
    s[8].unk_14 = 0.1f;
    s[8].unk_18 = 6.208f;
    s[8].unk_1C = 0.1f;
    s[8].unk_20 = 0.01f;
    s[8].unk_24 = 0;
    s[8].unk_28 = 0;
    s[8].unk_2C = 0;
    s[8].unk_30 = 6.0f;
    s[8].unk_34 = 0;
    s[8].unk_38 = 0;
    s[8].unk_3C = 0;
    s[8].unk_40 = 6.0f;
    s[8].unk_44 = 0;
    s[8].unk_48 = 0;
    s[8].unk_4C = 0;
    s[8].unk_50 = 0;
    s[8].unk_54 = 0.0f;
    s[8].unk_58 = 0;
    s[8].unk_5C = 0;
    s[8].unk_60 = 87.5f;
    s[8].unk_64 = 0.0f;
    s[8].unk_68 = 0;
    s[8].unk_6C = 0;
    s[8].unk_70 = 270.0f;
    s[8].unk_74 = 0.0f;
    s[8].unk_78 = 0;
    s[8].unk_7C = 0;
    s[8].unk_80 = 0.0f;
    s[8].unk_84 = 0.92425f;
    s[8].unk_88 = 1.0f;
    s[8].unk_8C = 1.0f;
    s[8].unk_90 = 0.42425f;
    s[8].unk_94 = 0.0f;
    s[8].unk_98 = 0.93575f;
    s[8].unk_9C = 0.0f;
    s[8].unk_A0 = 0.0f;
    s[8].unk_A4 = 0.0f;
    s[8].unk_A8 = 0.115f;
    s[8].unk_AC = 0;
    s[8].unk_B0 = 0;
    s[8].unk_B4 = 0;
    s[8].unk_B8 = 0;
    s[8].unk_BC = 0;
    s[8].unk_C0 = 0;
    s[8].unk_C8 = 0;
    s[8].unk_CC = 20.6f;
    s[8].unk_C4 = 5;
    s[8].unk_D0 = 1;
    s[8].unk_D4 = 20.0f;
    s[8].unk_D8 = 1.2f;
    s[8].unk_DC = 0;
    s[8].unk_E0 = 0;
    s[8].unk_E4 = -39.5f;
    s[8].unk_E8 = 0.0f;
    s[8].unk_EC = -21.5f;
    s[8].unk_F0 = -394.25f;
    s[8].unk_F4 = 0;
    s[8].unk_F8 = 0.0f;
    s[8].unk_FC = 0.0f;
    s[8].unk_100 = 110.0f;
    s[8].unk_104 = 1.0f;
    s[8].unk_108 = 1.0f;
    s[8].unk_10C = 1.0f;
    s[9].unk_0 = 8;
    s[9].unk_4 = 0;
    s[9].unk_8 = -1.0f;
    s[9].unk_C = 1.0f;
    s[9].unk_10 = 6.97601f;
    s[9].unk_14 = 0.2f;
    s[9].unk_18 = 6.208f;
    s[9].unk_1C = 0.2f;
    s[9].unk_20 = 0.01f;
    s[9].unk_24 = 0;
    s[9].unk_28 = 0;
    s[9].unk_2C = 0;
    s[9].unk_30 = 6.0f;
    s[9].unk_34 = 0;
    s[9].unk_38 = 0;
    s[9].unk_3C = 0;
    s[9].unk_40 = 6.0f;
    s[9].unk_44 = 0;
    s[9].unk_48 = 0;
    s[9].unk_4C = 0;
    s[9].unk_50 = 0;
    s[9].unk_54 = 0.0f;
    s[9].unk_58 = 0;
    s[9].unk_5C = 0;
    s[9].unk_60 = 87.5f;
    s[9].unk_64 = 0.0f;
    s[9].unk_68 = 0;
    s[9].unk_6C = 0;
    s[9].unk_70 = 270.0f;
    s[9].unk_74 = 0.0f;
    s[9].unk_78 = 0;
    s[9].unk_7C = 0;
    s[9].unk_80 = 0.0f;
    s[9].unk_84 = 0.92425f;
    s[9].unk_88 = 1.0f;
    s[9].unk_8C = 1.0f;
    s[9].unk_90 = 0.42425f;
    s[9].unk_94 = 0.0f;
    s[9].unk_98 = 0.93575f;
    s[9].unk_9C = 0.0f;
    s[9].unk_A0 = 0.0f;
    s[9].unk_A4 = 0.0f;
    s[9].unk_A8 = 0.115f;
    s[9].unk_AC = 0;
    s[9].unk_B0 = 0;
    s[9].unk_B4 = 0;
    s[9].unk_B8 = 0;
    s[9].unk_BC = 0;
    s[9].unk_C0 = 0;
    s[9].unk_C8 = 0;
    s[9].unk_CC = 20.6f;
    s[9].unk_C4 = 5;
    s[9].unk_D0 = 1;
    s[9].unk_D4 = 20.0f;
    s[9].unk_D8 = 1.2f;
    s[9].unk_DC = 0;
    s[9].unk_E0 = 0;
    s[9].unk_E4 = -39.5f;
    s[9].unk_E8 = 0.0f;
    s[9].unk_EC = -21.5f;
    s[9].unk_F0 = -394.25f;
    s[9].unk_F4 = 0;
    s[9].unk_F8 = 0.0f;
    s[9].unk_FC = 110.0f;
    s[9].unk_100 = 140.0f;
    s[9].unk_104 = 1.0f;
    s[9].unk_108 = 1.0f;
    s[9].unk_10C = 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8FF8__FPv);
#ifdef SKIP_ASM
void func_002F8FF8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F9000);
#ifdef SKIP_ASM
extern "C" void* func_002F9000(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F9040);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_2F9040 {
    float r, g, b, a;
    sColor_2F9040(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

extern sColor_2F9040 D_004FAC90;
extern sColor_2F9040 D_004FACA0;
extern sColor_2F9040 D_004FACB0;
extern sColor_2F9040 D_004FACC0;
extern sColor_2F9040 D_004FACD0;
extern sColor_2F9040 D_004FACE0;
extern sColor_2F9040 D_004FACF0;
extern sColor_2F9040 D_004FAD00;
extern sColor_2F9040 D_004FAD10;
extern sColor_2F9040 D_004FAD20;
extern sColor_2F9040 D_004FAD30;
extern sColor_2F9040 D_004FAD40;
extern sColor_2F9040 D_004FAD50;
extern sColor_2F9040 D_004FAD60;
extern sColor_2F9040 D_004FAD70;
extern sColor_2F9040 D_004FAD80;
extern sColor_2F9040 D_004FAD90;
extern sColor_2F9040 D_004FADA0;
extern sColor_2F9040 D_004FADB0;
extern sColor_2F9040 D_004FADC0;
extern sColor_2F9040 D_004FADD0;
extern sColor_2F9040 D_004FADE0;
extern sColor_2F9040 D_004FADF0;
extern sColor_2F9040 D_004FAE00;
extern sColor_2F9040 D_004FAE10;
extern sColor_2F9040 D_004FAE20;
extern sColor_2F9040 D_004FAE30;
extern sColor_2F9040 D_004FAE40;
extern sColor_2F9040 D_004FAE50;
extern sColor_2F9040 D_004FAE60;
extern sColor_2F9040 D_004FAE70;
extern sColor_2F9040 D_004FAE80;
extern sColor_2F9040 D_004FAE90;
extern sColor_2F9040 D_004FAEA0;
extern sColor_2F9040 D_004FAEB0;
extern sColor_2F9040 D_004FAEC0;
extern sColor_2F9040 D_004FAED0;
extern sColor_2F9040 D_004FAEE0;
extern sColor_2F9040 D_004FAEF0;
extern sColor_2F9040 D_004FAF00;
extern sColor_2F9040 D_004FAF10;
extern sColor_2F9040 D_004FAF20;
extern sColor_2F9040 D_004FAF30;
extern sColor_2F9040 D_004FAF40;
extern sColor_2F9040 D_004FAF50;
extern sColor_2F9040 D_004FAF60;
extern sColor_2F9040 D_004FAF70;
extern sColor_2F9040 D_004FAF80;
extern sColor_2F9040 D_004FAF90;
extern sColor_2F9040 D_004FAFA0;
extern sColor_2F9040 D_004FAFB0;
extern sColor_2F9040 D_004FAFC0;
extern sColor_2F9040 D_004FAFD0;
extern sColor_2F9040 D_004FAFE0;
extern sColor_2F9040 D_004FAFF0;
extern sColor_2F9040 D_004FB000;

// The unit declares func_002F9040 later as returning void*; bind this body by asm label.
extern "C" void func_002F9040_impl(int initialize, int priority) __asm__("func_002F9040");

extern "C" void func_002F9040_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004FAC90) sColor_2F9040(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004FACA0) sColor_2F9040(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004FACB0) sColor_2F9040(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004FACC0) sColor_2F9040(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004FACD0) sColor_2F9040(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004FACE0) sColor_2F9040(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004FACF0) sColor_2F9040(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004FAD00) sColor_2F9040(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004FAD10) sColor_2F9040(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004FAD20) sColor_2F9040(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004FAD30) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAD40) sColor_2F9040(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FAD50) sColor_2F9040(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004FAD60) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAD70) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAD80) sColor_2F9040(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FAD90) sColor_2F9040(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FADA0) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FADB0) sColor_2F9040(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FADC0) sColor_2F9040(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FADD0) sColor_2F9040(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FADE0) sColor_2F9040(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004FADF0) sColor_2F9040(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004FAE00) sColor_2F9040(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004FAE10) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAE20) sColor_2F9040(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004FAE30) sColor_2F9040(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004FAE40) sColor_2F9040(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004FAE50) sColor_2F9040(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004FAE60) sColor_2F9040(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004FAE70) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAE80) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAE90) sColor_2F9040(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004FAEA0) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAEB0) sColor_2F9040(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004FAEC0) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAED0) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FAEE0) sColor_2F9040(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004FAEF0) sColor_2F9040(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004FAF00) sColor_2F9040(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004FAF10) sColor_2F9040(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004FAF20) sColor_2F9040(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004FAF30) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAF40) sColor_2F9040(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004FAF50) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FAF60) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FAF70) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FAF80) sColor_2F9040(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004FAF90) sColor_2F9040(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004FAFA0) sColor_2F9040(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004FAFB0) sColor_2F9040(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004FAFC0) sColor_2F9040(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004FAFD0) sColor_2F9040(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004FAFE0) sColor_2F9040(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004FAFF0) sColor_2F9040(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004FB000) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

extern "C" void* func_002F9040(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002F9818__FPv);
#ifdef SKIP_ASM
void* func_002F9818(void* self)
{
    return func_002F9040(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F9838);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sLightSet_9838 {
    int unk_0;
    int unk_4;
    float unk_8;
    float unk_C;
    float unk_10;
    float unk_14;
    float unk_18;
    float unk_1C;
    float unk_20;
    int unk_24;
    int unk_28;
    int unk_2C;
    float unk_30;
    int unk_34;
    float unk_38;
    float unk_3C;
    float unk_40;
    int unk_44;
    int unk_48;
    int unk_4C;
    int unk_50;
    float unk_54;
    int unk_58;
    int unk_5C;
    int unk_60;
    float unk_64;
    int unk_68;
    int unk_6C;
    int unk_70;
    float unk_74;
    int unk_78;
    int unk_7C;
    float unk_80;
    float unk_84;
    float unk_88;
    float unk_8C;
    float unk_90;
    float unk_94;
    float unk_98;
    float unk_9C;
    float unk_A0;
    int unk_A4;
    float unk_A8;
    float unk_AC;
    float unk_B0;
    int unk_B4;
    float unk_B8;
    float unk_BC;
    float unk_C0;
    int unk_C4;
    int unk_C8;
    float unk_CC;
    int unk_D0;
    float unk_D4;
    float unk_D8;
    int unk_DC;
    int unk_E0;
    float unk_E4;
    float unk_E8;
    float unk_EC;
    float unk_F0;
    int unk_F4;
    float unk_F8;
    float unk_FC;
    float unk_100;
    float unk_104;
    float unk_108;
    float unk_10C;
};

extern sLightSet_9838* D_004A3BB0_9838 __asm__("D_004A3BB0");

extern "C" void func_002F9838(void)
{
    sLightSet_9838* s = D_004A3BB0_9838;
    s[10].unk_0 = 5;
    s[10].unk_4 = 0;
    s[10].unk_8 = -1.0f;
    s[10].unk_C = 0.8f;
    s[10].unk_10 = 5.0f;
    s[10].unk_14 = 0.25f;
    s[10].unk_18 = 0.0f;
    s[10].unk_1C = 0.1f;
    s[10].unk_20 = 0.0f;
    s[10].unk_24 = 0;
    s[10].unk_28 = 0;
    s[10].unk_2C = 0;
    s[10].unk_30 = 0.0f;
    s[10].unk_34 = 0;
    s[10].unk_38 = 0.0f;
    s[10].unk_3C = 0.0f;
    s[10].unk_40 = 0.0f;
    s[10].unk_44 = 0;
    s[10].unk_48 = 0;
    s[10].unk_4C = 0;
    s[10].unk_50 = 0;
    s[10].unk_54 = 400.0f;
    s[10].unk_58 = 0;
    s[10].unk_5C = 0;
    s[10].unk_60 = 0;
    s[10].unk_64 = 400.0f;
    s[10].unk_68 = 0;
    s[10].unk_6C = 0;
    s[10].unk_70 = 0;
    s[10].unk_74 = 400.0f;
    s[10].unk_78 = 0;
    s[10].unk_7C = 0;
    s[10].unk_80 = -185.0f;
    s[10].unk_84 = 1.0f;
    s[10].unk_88 = 1.0f;
    s[10].unk_8C = 0.391f;
    s[10].unk_90 = 0.059f;
    s[10].unk_94 = 0.0f;
    s[10].unk_98 = 1.0f;
    s[10].unk_9C = 0.97275f;
    s[10].unk_A0 = 0.0f;
    s[10].unk_A4 = 0;
    s[10].unk_A8 = 0.0f;
    s[10].unk_AC = 0.0f;
    s[10].unk_B0 = 0.0f;
    s[10].unk_B4 = 0;
    s[10].unk_B8 = 0.3515f;
    s[10].unk_BC = 0.0f;
    s[10].unk_C0 = 0.0f;
    s[10].unk_C8 = 0;
    s[10].unk_CC = 0.0f;
    s[10].unk_C4 = 41;
    s[10].unk_D0 = 1;
    s[10].unk_D4 = 20.0f;
    s[10].unk_D8 = 1.2f;
    s[10].unk_DC = 0;
    s[10].unk_E0 = 0;
    s[10].unk_E4 = 16.25f;
    s[10].unk_E8 = 3.0f;
    s[10].unk_EC = 18.0f;
    s[10].unk_F0 = 0.0f;
    s[10].unk_F4 = 0;
    s[10].unk_F8 = 0.0f;
    s[10].unk_FC = 0.0f;
    s[10].unk_100 = 140.0f;
    s[10].unk_104 = 1.0f;
    s[10].unk_108 = 0.45f;
    s[10].unk_10C = 0.7f;
    s[11].unk_0 = 10;
    s[11].unk_4 = 0;
    s[11].unk_8 = -1.0f;
    s[11].unk_C = 1.0f;
    s[11].unk_10 = 4.0f;
    s[11].unk_14 = 1.2f;
    s[11].unk_18 = 5.0f;
    s[11].unk_1C = 2.0f;
    s[11].unk_20 = 0.0f;
    s[11].unk_24 = 0;
    s[11].unk_28 = 0;
    s[11].unk_2C = 0;
    s[11].unk_30 = 17.75f;
    s[11].unk_34 = 0;
    s[11].unk_38 = 0.0f;
    s[11].unk_3C = 0.0f;
    s[11].unk_40 = 17.75f;
    s[11].unk_44 = 0;
    s[11].unk_48 = 0;
    s[11].unk_4C = 0;
    s[11].unk_50 = 0;
    s[11].unk_54 = 75.0f;
    s[11].unk_58 = 0;
    s[11].unk_5C = 0;
    s[11].unk_60 = 0;
    s[11].unk_64 = 75.0f;
    s[11].unk_68 = 0;
    s[11].unk_6C = 0;
    s[11].unk_70 = 0;
    s[11].unk_74 = 120.0f;
    s[11].unk_78 = 0;
    s[11].unk_7C = 0;
    s[11].unk_80 = 10.0f;
    s[11].unk_84 = 0.56375f;
    s[11].unk_88 = 0.76525f;
    s[11].unk_8C = 0.75725f;
    s[11].unk_90 = 0.7755f;
    s[11].unk_94 = 0.0f;
    s[11].unk_98 = 1.0f;
    s[11].unk_9C = 1.0f;
    s[11].unk_A0 = 1.0f;
    s[11].unk_A4 = 0;
    s[11].unk_A8 = 0.02975f;
    s[11].unk_AC = 0.02975f;
    s[11].unk_B0 = 0.0f;
    s[11].unk_B4 = 0;
    s[11].unk_B8 = 0.4395f;
    s[11].unk_BC = 0.43625f;
    s[11].unk_C0 = 0.4485f;
    s[11].unk_C8 = 1;
    s[11].unk_CC = 70.0f;
    s[11].unk_C4 = 5;
    s[11].unk_D0 = 1;
    s[11].unk_D4 = 20.0f;
    s[11].unk_D8 = 1.1f;
    s[11].unk_DC = 0;
    s[11].unk_E0 = 0;
    s[11].unk_E4 = 75.0f;
    s[11].unk_E8 = 0.0f;
    s[11].unk_EC = -60.0f;
    s[11].unk_F0 = 172.5f;
    s[11].unk_F4 = 0;
    s[11].unk_F8 = -223.0f;
    s[11].unk_FC = 0.0f;
    s[11].unk_100 = 55.0f;
    s[11].unk_104 = 1.0f;
    s[11].unk_108 = 0.42f;
    s[11].unk_10C = 0.2f;
    s[12].unk_0 = 8;
    s[12].unk_4 = 4;
    s[12].unk_8 = -1.0f;
    s[12].unk_C = 1.0f;
    s[12].unk_10 = 1e-05f;
    s[12].unk_14 = 0.01f;
    s[12].unk_18 = 10.112f;
    s[12].unk_1C = 0.0f;
    s[12].unk_20 = 0.001f;
    s[12].unk_24 = 0;
    s[12].unk_28 = 0;
    s[12].unk_2C = 0;
    s[12].unk_30 = 0.0f;
    s[12].unk_34 = 0;
    s[12].unk_38 = 0.0f;
    s[12].unk_3C = 0.0f;
    s[12].unk_40 = 0.0f;
    s[12].unk_44 = 0;
    s[12].unk_48 = 0;
    s[12].unk_4C = 0;
    s[12].unk_50 = 0;
    s[12].unk_54 = 0.0f;
    s[12].unk_58 = 0;
    s[12].unk_5C = 0;
    s[12].unk_60 = 0;
    s[12].unk_64 = 0.0f;
    s[12].unk_68 = 0;
    s[12].unk_6C = 0;
    s[12].unk_70 = 0;
    s[12].unk_74 = 0.0f;
    s[12].unk_78 = 0;
    s[12].unk_7C = 0;
    s[12].unk_80 = 0.0f;
    s[12].unk_84 = 1.0f;
    s[12].unk_88 = 1.0f;
    s[12].unk_8C = 0.37575f;
    s[12].unk_90 = 0.21975f;
    s[12].unk_94 = 0.0f;
    s[12].unk_98 = 1.0f;
    s[12].unk_9C = 0.97275f;
    s[12].unk_A0 = 0.0f;
    s[12].unk_A4 = 0;
    s[12].unk_A8 = 0.045f;
    s[12].unk_AC = 0.045f;
    s[12].unk_B0 = 0.0485f;
    s[12].unk_B4 = 0;
    s[12].unk_B8 = 0.2725f;
    s[12].unk_BC = 0.0f;
    s[12].unk_C0 = 0.0f;
    s[12].unk_C8 = 0;
    s[12].unk_CC = 0.0f;
    s[12].unk_C4 = 41;
    s[12].unk_D0 = 1;
    s[12].unk_D4 = 20.0f;
    s[12].unk_D8 = 1.2f;
    s[12].unk_DC = 0;
    s[12].unk_E0 = 0;
    s[12].unk_E4 = 16.25f;
    s[12].unk_E8 = 3.0f;
    s[12].unk_EC = 18.0f;
    s[12].unk_F0 = 0.0f;
    s[12].unk_F4 = 0;
    s[12].unk_F8 = 0.0f;
    s[12].unk_FC = 0.0f;
    s[12].unk_100 = 140.0f;
    s[12].unk_104 = 1.0f;
    s[12].unk_108 = 1.0f;
    s[12].unk_10C = 1.0f;
    s[13].unk_0 = 15;
    s[13].unk_4 = 0;
    s[13].unk_8 = -1.0f;
    s[13].unk_C = 1.0f;
    s[13].unk_10 = 2.0f;
    s[13].unk_14 = 1.2f;
    s[13].unk_18 = 2.5f;
    s[13].unk_1C = 2.0f;
    s[13].unk_20 = 0.0f;
    s[13].unk_24 = 0;
    s[13].unk_28 = 0;
    s[13].unk_2C = 0;
    s[13].unk_30 = 17.75f;
    s[13].unk_34 = 0;
    s[13].unk_38 = 0.0f;
    s[13].unk_3C = 0.0f;
    s[13].unk_40 = 17.75f;
    s[13].unk_44 = 0;
    s[13].unk_48 = 0;
    s[13].unk_4C = 0;
    s[13].unk_50 = 0;
    s[13].unk_54 = 87.5f;
    s[13].unk_58 = 0;
    s[13].unk_5C = 0;
    s[13].unk_60 = 0;
    s[13].unk_64 = 152.5f;
    s[13].unk_68 = 0;
    s[13].unk_6C = 0;
    s[13].unk_70 = 0;
    s[13].unk_74 = 100.0f;
    s[13].unk_78 = 0;
    s[13].unk_7C = 0;
    s[13].unk_80 = 10.0f;
    s[13].unk_84 = 0.5755f;
    s[13].unk_88 = 0.66125f;
    s[13].unk_8C = 0.6485f;
    s[13].unk_90 = 0.66825f;
    s[13].unk_94 = 0.0f;
    s[13].unk_98 = 0.2725f;
    s[13].unk_9C = 0.2725f;
    s[13].unk_A0 = 0.28475f;
    s[13].unk_A4 = 0;
    s[13].unk_A8 = 0.02975f;
    s[13].unk_AC = 0.02975f;
    s[13].unk_B0 = 0.0f;
    s[13].unk_B4 = 0;
    s[13].unk_B8 = 0.24525f;
    s[13].unk_BC = 0.24525f;
    s[13].unk_C0 = 0.26675f;
    s[13].unk_C8 = 1;
    s[13].unk_CC = 200.0f;
    s[13].unk_C4 = 5;
    s[13].unk_D0 = 1;
    s[13].unk_D4 = 20.0f;
    s[13].unk_D8 = 1.2f;
    s[13].unk_DC = 0;
    s[13].unk_E0 = 0;
    s[13].unk_E4 = 75.0f;
    s[13].unk_E8 = 0.0f;
    s[13].unk_EC = -60.0f;
    s[13].unk_F0 = 172.5f;
    s[13].unk_F4 = 0;
    s[13].unk_F8 = -223.0f;
    s[13].unk_FC = 55.0f;
    s[13].unk_100 = 105.0f;
    s[13].unk_104 = 1.0f;
    s[13].unk_108 = 0.5f;
    s[13].unk_10C = 0.3f;
    s[14].unk_0 = 10;
    s[14].unk_4 = 0;
    s[14].unk_8 = -1.0f;
    s[14].unk_C = 1.0f;
    s[14].unk_10 = 3.52001f;
    s[14].unk_14 = 1.2f;
    s[14].unk_18 = 2.24f;
    s[14].unk_1C = 2.3f;
    s[14].unk_20 = 0.0f;
    s[14].unk_24 = 0;
    s[14].unk_28 = 0;
    s[14].unk_2C = 0;
    s[14].unk_30 = 17.75f;
    s[14].unk_34 = 0;
    s[14].unk_38 = 0.0f;
    s[14].unk_3C = 0.0f;
    s[14].unk_40 = 17.75f;
    s[14].unk_44 = 0;
    s[14].unk_48 = 0;
    s[14].unk_4C = 0;
    s[14].unk_50 = 0;
    s[14].unk_54 = 87.5f;
    s[14].unk_58 = 0;
    s[14].unk_5C = 0;
    s[14].unk_60 = 0;
    s[14].unk_64 = 152.5f;
    s[14].unk_68 = 0;
    s[14].unk_6C = 0;
    s[14].unk_70 = 0;
    s[14].unk_74 = 270.0f;
    s[14].unk_78 = 0;
    s[14].unk_7C = 0;
    s[14].unk_80 = 10.0f;
    s[14].unk_84 = 0.33f;
    s[14].unk_88 = 0.36125f;
    s[14].unk_8C = 0.3485f;
    s[14].unk_90 = 0.391f;
    s[14].unk_94 = 0.0f;
    s[14].unk_98 = 0.0f;
    s[14].unk_9C = 0.0f;
    s[14].unk_A0 = 0.0f;
    s[14].unk_A4 = 0;
    s[14].unk_A8 = 0.1295f;
    s[14].unk_AC = 0.1295f;
    s[14].unk_B0 = 0.0f;
    s[14].unk_B4 = 0;
    s[14].unk_B8 = 0.44525f;
    s[14].unk_BC = 0.44525f;
    s[14].unk_C0 = 0.4665f;
    s[14].unk_C8 = 1;
    s[14].unk_CC = 300.0f;
    s[14].unk_C4 = 5;
    s[14].unk_D0 = 1;
    s[14].unk_D4 = 20.0f;
    s[14].unk_D8 = 1.1f;
    s[14].unk_DC = 0;
    s[14].unk_E0 = 0;
    s[14].unk_E4 = 75.0f;
    s[14].unk_E8 = 0.0f;
    s[14].unk_EC = -60.0f;
    s[14].unk_F0 = 200.0f;
    s[14].unk_F4 = 0;
    s[14].unk_F8 = -273.0f;
    s[14].unk_FC = 105.0f;
    s[14].unk_100 = 140.0f;
    s[14].unk_104 = 1.0f;
    s[14].unk_108 = 1.0f;
    s[14].unk_10C = 0.4f;
    s[15].unk_0 = 1;
    s[15].unk_4 = 0;
    s[15].unk_8 = -1.0f;
    s[15].unk_C = 1.0f;
    s[15].unk_10 = 4.0f;
    s[15].unk_14 = 1.0f;
    s[15].unk_18 = 0.0f;
    s[15].unk_1C = 0.0f;
    s[15].unk_20 = 0.0f;
    s[15].unk_24 = 0;
    s[15].unk_28 = 0;
    s[15].unk_2C = 0;
    s[15].unk_30 = 0.0f;
    s[15].unk_34 = 0;
    s[15].unk_38 = 0.0f;
    s[15].unk_3C = 0.0f;
    s[15].unk_40 = 0.0f;
    s[15].unk_44 = 0;
    s[15].unk_48 = 0;
    s[15].unk_4C = 0;
    s[15].unk_50 = 0;
    s[15].unk_54 = 0.0f;
    s[15].unk_58 = 0;
    s[15].unk_5C = 0;
    s[15].unk_60 = 0;
    s[15].unk_64 = 0.0f;
    s[15].unk_68 = 0;
    s[15].unk_6C = 0;
    s[15].unk_70 = 0;
    s[15].unk_74 = 0.0f;
    s[15].unk_78 = 0;
    s[15].unk_7C = 0;
    s[15].unk_80 = 0.0f;
    s[15].unk_84 = 1.0f;
    s[15].unk_88 = 1.0f;
    s[15].unk_8C = 1.0f;
    s[15].unk_90 = 1.0f;
    s[15].unk_94 = 1.0f;
    s[15].unk_98 = 1.0f;
    s[15].unk_9C = 1.0f;
    s[15].unk_A0 = 1.0f;
    s[15].unk_A4 = 0;
    s[15].unk_A8 = 0.0f;
    s[15].unk_AC = 0.0f;
    s[15].unk_B0 = 0.0f;
    s[15].unk_B4 = 0;
    s[15].unk_B8 = 0.0f;
    s[15].unk_BC = 0.0f;
    s[15].unk_C0 = 0.0f;
    s[15].unk_C8 = 1;
    s[15].unk_CC = 0.0f;
    s[15].unk_C4 = 5;
    s[15].unk_D0 = 1;
    s[15].unk_D4 = 20.0f;
    s[15].unk_D8 = 0.0f;
    s[15].unk_DC = 0;
    s[15].unk_E0 = 0;
    s[15].unk_E4 = 0.0f;
    s[15].unk_E8 = 0.0f;
    s[15].unk_EC = 0.0f;
    s[15].unk_F0 = 0.0f;
    s[15].unk_F4 = 0;
    s[15].unk_F8 = 0.0f;
    s[15].unk_FC = 0.0f;
    s[15].unk_100 = 140.0f;
    s[15].unk_104 = 1.0f;
    s[15].unk_108 = 1.0f;
    s[15].unk_10C = 1.0f;
    s[16].unk_0 = 3;
    s[16].unk_4 = 0;
    s[16].unk_8 = -1.0f;
    s[16].unk_C = 1.0f;
    s[16].unk_10 = 9.21601f;
    s[16].unk_14 = 0.09001f;
    s[16].unk_18 = 12.416f;
    s[16].unk_1C = 0.07625f;
    s[16].unk_20 = 0.0f;
    s[16].unk_24 = 0;
    s[16].unk_28 = 0;
    s[16].unk_2C = 0;
    s[16].unk_30 = 0.1f;
    s[16].unk_34 = 0;
    s[16].unk_38 = 0.0f;
    s[16].unk_3C = 0.0f;
    s[16].unk_40 = 0.1f;
    s[16].unk_44 = 0;
    s[16].unk_48 = 0;
    s[16].unk_4C = 0;
    s[16].unk_50 = 0;
    s[16].unk_54 = 20.0f;
    s[16].unk_58 = 0;
    s[16].unk_5C = 0;
    s[16].unk_60 = 0;
    s[16].unk_64 = 20.0f;
    s[16].unk_68 = 0;
    s[16].unk_6C = 0;
    s[16].unk_70 = 0;
    s[16].unk_74 = 20.0f;
    s[16].unk_78 = 0;
    s[16].unk_7C = 0;
    s[16].unk_80 = 0.0f;
    s[16].unk_84 = 0.45425f;
    s[16].unk_88 = 0.0f;
    s[16].unk_8C = 0.0f;
    s[16].unk_90 = 1.0f;
    s[16].unk_94 = 0.0f;
    s[16].unk_98 = 1.0f;
    s[16].unk_9C = 0.62725f;
    s[16].unk_A0 = 0.98175f;
    s[16].unk_A4 = 0;
    s[16].unk_A8 = 0.0f;
    s[16].unk_AC = 0.0f;
    s[16].unk_B0 = 0.0f;
    s[16].unk_B4 = 0;
    s[16].unk_B8 = 0.0f;
    s[16].unk_BC = 0.0f;
    s[16].unk_C0 = 0.0f;
    s[16].unk_C8 = 0;
    s[16].unk_CC = 0.0f;
    s[16].unk_C4 = 5;
    s[16].unk_D0 = 1;
    s[16].unk_D4 = 20.0f;
    s[16].unk_D8 = 1.2f;
    s[16].unk_DC = 0;
    s[16].unk_E0 = 0;
    s[16].unk_E4 = 9.0f;
    s[16].unk_E8 = 6.25f;
    s[16].unk_EC = 9.0f;
    s[16].unk_F0 = 0.0f;
    s[16].unk_F4 = 0;
    s[16].unk_F8 = 0.0f;
    s[16].unk_FC = 0.0f;
    s[16].unk_100 = 140.0f;
    s[16].unk_104 = 1.0f;
    s[16].unk_108 = 0.45f;
    s[16].unk_10C = 0.24f;
    s[17].unk_0 = 8;
    s[17].unk_4 = 0;
    s[17].unk_8 = -1.0f;
    s[17].unk_C = 1.0f;
    s[17].unk_10 = 19.39201f;
    s[17].unk_14 = 0.1f;
    s[17].unk_18 = 36.416f;
    s[17].unk_1C = 0.09f;
    s[17].unk_20 = 0.0f;
    s[17].unk_24 = 0;
    s[17].unk_28 = 0;
    s[17].unk_2C = 0;
    s[17].unk_30 = 3.0f;
    s[17].unk_34 = 0;
    s[17].unk_38 = 0.0f;
    s[17].unk_3C = 0.0f;
    s[17].unk_40 = 3.0f;
    s[17].unk_44 = 0;
    s[17].unk_48 = 0;
    s[17].unk_4C = 0;
    s[17].unk_50 = 0;
    s[17].unk_54 = 100.0f;
    s[17].unk_58 = 0;
    s[17].unk_5C = 0;
    s[17].unk_60 = 0;
    s[17].unk_64 = 100.0f;
    s[17].unk_68 = 0;
    s[17].unk_6C = 0;
    s[17].unk_70 = 0;
    s[17].unk_74 = 100.0f;
    s[17].unk_78 = 0;
    s[17].unk_7C = 0;
    s[17].unk_80 = 0.0f;
    s[17].unk_84 = 0.4605f;
    s[17].unk_88 = 0.0f;
    s[17].unk_8C = 0.0f;
    s[17].unk_90 = 0.0f;
    s[17].unk_94 = 0.0f;
    s[17].unk_98 = 1.0f;
    s[17].unk_9C = 1.0f;
    s[17].unk_A0 = 1.0f;
    s[17].unk_A4 = 0;
    s[17].unk_A8 = 1.0f;
    s[17].unk_AC = 0.0f;
    s[17].unk_B0 = 0.0f;
    s[17].unk_B4 = 0;
    s[17].unk_B8 = 0.0f;
    s[17].unk_BC = 1.0f;
    s[17].unk_C0 = 0.0f;
    s[17].unk_C8 = 0;
    s[17].unk_CC = 1.75f;
    s[17].unk_C4 = 38;
    s[17].unk_D0 = 1;
    s[17].unk_D4 = 20.0f;
    s[17].unk_D8 = 1.2f;
    s[17].unk_DC = 0;
    s[17].unk_E0 = 0;
    s[17].unk_E4 = 36.0f;
    s[17].unk_E8 = -15.25f;
    s[17].unk_EC = -12.0f;
    s[17].unk_F0 = 0.0f;
    s[17].unk_F4 = 0;
    s[17].unk_F8 = 0.0f;
    s[17].unk_FC = 0.0f;
    s[17].unk_100 = 140.0f;
    s[17].unk_104 = 1.0f;
    s[17].unk_108 = 0.7425f;
    s[17].unk_10C = 4.25f;
    s[18].unk_0 = 3;
    s[18].unk_4 = 0;
    s[18].unk_8 = -1.0f;
    s[18].unk_C = 1.0f;
    s[18].unk_10 = 16.25601f;
    s[18].unk_14 = 0.27251f;
    s[18].unk_18 = 11.968f;
    s[18].unk_1C = 0.13625f;
    s[18].unk_20 = 0.0f;
    s[18].unk_24 = 0;
    s[18].unk_28 = 0;
    s[18].unk_2C = 0;
    s[18].unk_30 = 0.0f;
    s[18].unk_34 = 0;
    s[18].unk_38 = 10.0f;
    s[18].unk_3C = 10.0f;
    s[18].unk_40 = 10.0f;
    s[18].unk_44 = 0;
    s[18].unk_48 = 0;
    s[18].unk_4C = 0;
    s[18].unk_50 = 0;
    s[18].unk_54 = 177.5f;
    s[18].unk_58 = 0;
    s[18].unk_5C = 0;
    s[18].unk_60 = 0;
    s[18].unk_64 = 242.5f;
    s[18].unk_68 = 0;
    s[18].unk_6C = 0;
    s[18].unk_70 = 0;
    s[18].unk_74 = 242.5f;
    s[18].unk_78 = 0;
    s[18].unk_7C = 0;
    s[18].unk_80 = -240.0f;
    s[18].unk_84 = 1.0f;
    s[18].unk_88 = 0.90275f;
    s[18].unk_8C = 0.545f;
    s[18].unk_90 = 0.0f;
    s[18].unk_94 = 0.0f;
    s[18].unk_98 = 0.16975f;
    s[18].unk_9C = 0.00875f;
    s[18].unk_A0 = 0.0f;
    s[18].unk_A4 = 0;
    s[18].unk_A8 = 0.0f;
    s[18].unk_AC = 0.0f;
    s[18].unk_B0 = 0.1665f;
    s[18].unk_B4 = 0;
    s[18].unk_B8 = 0.2425f;
    s[18].unk_BC = 0.0f;
    s[18].unk_C0 = 0.0f;
    s[18].unk_C8 = 0;
    s[18].unk_CC = 2.4f;
    s[18].unk_C4 = 41;
    s[18].unk_D0 = 1;
    s[18].unk_D4 = 20.0f;
    s[18].unk_D8 = 1.5f;
    s[18].unk_DC = 0;
    s[18].unk_E0 = 0;
    s[18].unk_E4 = 15.25f;
    s[18].unk_E8 = -9.25f;
    s[18].unk_EC = 0.0f;
    s[18].unk_F0 = 0.0f;
    s[18].unk_F4 = 0;
    s[18].unk_F8 = 0.0f;
    s[18].unk_FC = 0.0f;
    s[18].unk_100 = 140.0f;
    s[18].unk_104 = 1.0f;
    s[18].unk_108 = 1.0f;
    s[18].unk_10C = 1.0f;
    s[19].unk_0 = 3;
    s[19].unk_4 = 0;
    s[19].unk_8 = -1.0f;
    s[19].unk_C = 1.0f;
    s[19].unk_10 = 9.21601f;
    s[19].unk_14 = 0.09001f;
    s[19].unk_18 = 12.416f;
    s[19].unk_1C = 0.07625f;
    s[19].unk_20 = 0.0f;
    s[19].unk_24 = 0;
    s[19].unk_28 = 0;
    s[19].unk_2C = 0;
    s[19].unk_30 = 0.1f;
    s[19].unk_34 = 0;
    s[19].unk_38 = 0.0f;
    s[19].unk_3C = 0.0f;
    s[19].unk_40 = 0.1f;
    s[19].unk_44 = 0;
    s[19].unk_48 = 0;
    s[19].unk_4C = 0;
    s[19].unk_50 = 0;
    s[19].unk_54 = 20.0f;
    s[19].unk_58 = 0;
    s[19].unk_5C = 0;
    s[19].unk_60 = 0;
    s[19].unk_64 = 20.0f;
    s[19].unk_68 = 0;
    s[19].unk_6C = 0;
    s[19].unk_70 = 0;
    s[19].unk_74 = 20.0f;
    s[19].unk_78 = 0;
    s[19].unk_7C = 0;
    s[19].unk_80 = 0.0f;
    s[19].unk_84 = 0.45425f;
    s[19].unk_88 = 0.0f;
    s[19].unk_8C = 0.0f;
    s[19].unk_90 = 1.0f;
    s[19].unk_94 = 0.0f;
    s[19].unk_98 = 1.0f;
    s[19].unk_9C = 0.62725f;
    s[19].unk_A0 = 0.98175f;
    s[19].unk_A4 = 0;
    s[19].unk_A8 = 0.0f;
    s[19].unk_AC = 0.0f;
    s[19].unk_B0 = 0.0f;
    s[19].unk_B4 = 0;
    s[19].unk_B8 = 0.0f;
    s[19].unk_BC = 0.0f;
    s[19].unk_C0 = 0.0f;
    s[19].unk_C8 = 0;
    s[19].unk_CC = 0.0f;
    s[19].unk_C4 = 5;
    s[19].unk_D0 = 1;
    s[19].unk_D4 = 20.0f;
    s[19].unk_D8 = 1.2f;
    s[19].unk_DC = 0;
    s[19].unk_E0 = 0;
    s[19].unk_E4 = 9.0f;
    s[19].unk_E8 = -6.25f;
    s[19].unk_EC = 9.0f;
    s[19].unk_F0 = 0.0f;
    s[19].unk_F4 = 0;
    s[19].unk_F8 = 0.0f;
    s[19].unk_FC = 0.0f;
    s[19].unk_100 = 140.0f;
    s[19].unk_104 = 1.0f;
    s[19].unk_108 = 0.47275f;
    s[19].unk_10C = 0.2f;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002FA5F8__FPv);
#ifdef SKIP_ASM
void func_002FA5F8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002FA600);
#ifdef SKIP_ASM
extern "C" void* func_002FA600(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002FA640);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColor_2FA640 {
    float r, g, b, a;
    sColor_2FA640(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

extern sColor_2FA640 D_004FB010;
extern sColor_2FA640 D_004FB020;
extern sColor_2FA640 D_004FB030;
extern sColor_2FA640 D_004FB040;
extern sColor_2FA640 D_004FB050;
extern sColor_2FA640 D_004FB060;
extern sColor_2FA640 D_004FB070;
extern sColor_2FA640 D_004FB080;
extern sColor_2FA640 D_004FB090;
extern sColor_2FA640 D_004FB0A0;
extern sColor_2FA640 D_004FB0B0;
extern sColor_2FA640 D_004FB0C0;
extern sColor_2FA640 D_004FB0D0;
extern sColor_2FA640 D_004FB0E0;
extern sColor_2FA640 D_004FB0F0;
extern sColor_2FA640 D_004FB100;
extern sColor_2FA640 D_004FB110;
extern sColor_2FA640 D_004FB120;
extern sColor_2FA640 D_004FB130;
extern sColor_2FA640 D_004FB140;
extern sColor_2FA640 D_004FB150;
extern sColor_2FA640 D_004FB160;
extern sColor_2FA640 D_004FB170;
extern sColor_2FA640 D_004FB180;
extern sColor_2FA640 D_004FB190;
extern sColor_2FA640 D_004FB1A0;
extern sColor_2FA640 D_004FB1B0;
extern sColor_2FA640 D_004FB1C0;
extern sColor_2FA640 D_004FB1D0;
extern sColor_2FA640 D_004FB1E0;
extern sColor_2FA640 D_004FB1F0;
extern sColor_2FA640 D_004FB200;
extern sColor_2FA640 D_004FB210;
extern sColor_2FA640 D_004FB220;
extern sColor_2FA640 D_004FB230;
extern sColor_2FA640 D_004FB240;
extern sColor_2FA640 D_004FB250;
extern sColor_2FA640 D_004FB260;
extern sColor_2FA640 D_004FB270;
extern sColor_2FA640 D_004FB280;
extern sColor_2FA640 D_004FB290;
extern sColor_2FA640 D_004FB2A0;
extern sColor_2FA640 D_004FB2B0;
extern sColor_2FA640 D_004FB2C0;
extern sColor_2FA640 D_004FB2D0;
extern sColor_2FA640 D_004FB2E0;
extern sColor_2FA640 D_004FB2F0;
extern sColor_2FA640 D_004FB300;
extern sColor_2FA640 D_004FB310;
extern sColor_2FA640 D_004FB320;
extern sColor_2FA640 D_004FB330;
extern sColor_2FA640 D_004FB340;
extern sColor_2FA640 D_004FB350;
extern sColor_2FA640 D_004FB360;
extern sColor_2FA640 D_004FB370;
extern sColor_2FA640 D_004FB380;

// The unit declares func_002FA640 later as returning void*; bind this body by asm label.
extern "C" void func_002FA640_impl(int initialize, int priority) __asm__("func_002FA640");

extern "C" void func_002FA640_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004FB010) sColor_2FA640(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004FB020) sColor_2FA640(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004FB030) sColor_2FA640(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004FB040) sColor_2FA640(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004FB050) sColor_2FA640(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004FB060) sColor_2FA640(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004FB070) sColor_2FA640(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004FB080) sColor_2FA640(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004FB090) sColor_2FA640(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004FB0A0) sColor_2FA640(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004FB0B0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB0C0) sColor_2FA640(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FB0D0) sColor_2FA640(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004FB0E0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB0F0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB100) sColor_2FA640(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FB110) sColor_2FA640(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FB120) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB130) sColor_2FA640(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FB140) sColor_2FA640(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FB150) sColor_2FA640(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FB160) sColor_2FA640(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004FB170) sColor_2FA640(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004FB180) sColor_2FA640(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004FB190) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB1A0) sColor_2FA640(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004FB1B0) sColor_2FA640(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004FB1C0) sColor_2FA640(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004FB1D0) sColor_2FA640(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004FB1E0) sColor_2FA640(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004FB1F0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB200) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB210) sColor_2FA640(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004FB220) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB230) sColor_2FA640(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004FB240) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB250) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FB260) sColor_2FA640(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004FB270) sColor_2FA640(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004FB280) sColor_2FA640(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004FB290) sColor_2FA640(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004FB2A0) sColor_2FA640(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004FB2B0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB2C0) sColor_2FA640(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004FB2D0) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FB2E0) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FB2F0) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FB300) sColor_2FA640(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004FB310) sColor_2FA640(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004FB320) sColor_2FA640(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004FB330) sColor_2FA640(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004FB340) sColor_2FA640(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004FB350) sColor_2FA640(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004FB360) sColor_2FA640(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004FB370) sColor_2FA640(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004FB380) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

extern "C" void* func_002FA640(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002FAE18__FPv);
#ifdef SKIP_ASM
void* func_002FAE18(void* self)
{
    return func_002FA640(1, 0xffff);
}
#endif


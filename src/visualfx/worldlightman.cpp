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

INCLUDE_ASM("visualfx/worldlightman", func_002F5400);

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

INCLUDE_ASM("visualfx/worldlightman", func_002F5D30);

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

INCLUDE_ASM("visualfx/worldlightman", func_002F6518);

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

INCLUDE_ASM("visualfx/worldlightman", func_002F6B88);

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7A68__FPv);
#ifdef SKIP_ASM
void* func_002F7A68(void* self)
{
    return self;
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
INCLUDE_ASM("visualfx/worldlightman", func_002F7FF0__FPv);
#ifdef SKIP_ASM
int func_002F7FF0(void* self)
{
    return *(int*)((char*)self + 0x74);
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

INCLUDE_ASM("visualfx/worldlightman", func_002F83B0);

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8FF8__FPv);
#ifdef SKIP_ASM
void func_002F8FF8(void* self)
{
}
#endif

INCLUDE_ASM("visualfx/worldlightman", func_002F9040);

extern "C" void* func_002F9040(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002F9818__FPv);
#ifdef SKIP_ASM
void* func_002F9818(void* self)
{
    return func_002F9040(1, 0xffff);
}
#endif

INCLUDE_ASM("visualfx/worldlightman", func_002F9838);

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002FA5F8__FPv);
#ifdef SKIP_ASM
void func_002FA5F8(void* self)
{
}
#endif

INCLUDE_ASM("visualfx/worldlightman", func_002FA640);

extern "C" void* func_002FA640(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002FAE18__FPv);
#ifdef SKIP_ASM
void* func_002FAE18(void* self)
{
    return func_002FA640(1, 0xffff);
}
#endif


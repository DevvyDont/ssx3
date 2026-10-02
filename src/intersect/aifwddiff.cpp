#include "common.h"

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_Init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_00340970(void* self);
extern char D_0048E4F8[];
extern char D_0048E508[];

struct sCacheEntry_7890 {
    unsigned int key;
    sCacheEntry_7890* next;
    sCacheEntry_7890* prev;
    char pad[0x1CD0 - 0xC];
};
struct sCache_7890 {
    sCacheEntry_7890* entries;  // 0x0
    sCacheEntry_7890* lru;      // 0x4
    sCacheEntry_7890** buckets; // 0x8
    int count;                  // 0xC
};

extern "C" void cAIFwdDiffCache_Init(sCache_7890* self, int n)
{
    sCacheEntry_7890* mem = (sCacheEntry_7890*)operator_new_tag(n * sizeof(sCacheEntry_7890), D_0048E4F8, 0, 0);
    sCacheEntry_7890* p = mem;
    for (int k = n - 1; k != -1; k--, p++) {
        func_00340970(p);
    }
    self->entries = mem;
    for (int i = 0; i < n; i++) {
        self->entries[i].next = &self->entries[i] + 1;
        self->entries[i].prev = &self->entries[i] - 1;
        self->entries[i].key = 0xFFFFFFFF;
    }
    self->entries[0].prev = self->entries + n - 1;
    self->entries[n - 1].next = self->entries;
    self->count = n * 2 + 13;
    self->lru = self->entries;
    self->buckets = (sCacheEntry_7890**)operator_new_tag(self->count * 4, D_0048E508, 0x80000000, 0);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", func_003279D0);
#ifdef SKIP_ASM
struct sFwdDiffList;
struct sFwdDiffLink;
struct sFwdDiffTable;
struct sFwdDiffNode;
extern "C" void func_00327AC0(sFwdDiffList* list, sFwdDiffLink* n);
extern "C" void func_00327B30(void* self, void* node);
extern "C" void func_00327BB0(sFwdDiffTable* table, sFwdDiffNode* node);
extern "C" void func_00327C00(void* self, void* src, void* node);

struct sCacheNode_279D0 {
    unsigned int key;           // 0x0
    sCacheNode_279D0* lruNext;  // 0x4
    sCacheNode_279D0* lruPrev;  // 0x8
    sCacheNode_279D0* next;     // 0xC
};

struct sCache_279D0 {
    int pad0;
    sCacheNode_279D0* lru;      // 0x4
    sCacheNode_279D0** buckets; // 0x8
    unsigned int count;         // 0xC
};

static inline int func_003279D0_less(unsigned int a, unsigned int b) { return a < b; }

extern "C" void func_003279D0(sCache_279D0* self, void* src, sCacheNode_279D0** out)
{
    unsigned int key = *(unsigned int*)((char*)src + 0x150);
    sCacheNode_279D0* n = self->buckets[key % self->count];
    while (n != 0 && !func_003279D0_less(key, n->key)) {
        if (n->key == key) {
            func_00327AC0((sFwdDiffList*)self, (sFwdDiffLink*)n);
            *out = n;
            return;
        }
        n = n->next;
    }
    n = self->lru;
    func_00327B30(self, n);
    n->key = key;
    func_00327BB0((sFwdDiffTable*)self, (sFwdDiffNode*)n);
    self->lru = n->lruNext;
    func_00327C00(self, src, n);
    *out = n;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", func_00327AC0);
#ifdef SKIP_ASM
struct sFwdDiffLink {
    int pad0;
    sFwdDiffLink* next;   // 0x4
    sFwdDiffLink* prev;   // 0x8
};

struct sFwdDiffList {
    int pad0;
    sFwdDiffLink* head;   // 0x4
};

extern "C" void func_00327AC0(sFwdDiffList* list, sFwdDiffLink* n)
{
    if (list->head == n) {
        list->head = n->next;
        return;
    }
    if (list->head->prev == n) {
        return;
    }
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->prev = list->head->prev;
    n->next = list->head;
    list->head->prev->next = n;
    list->head->prev = n;
}
#endif

INCLUDE_ASM("intersect/aifwddiff", func_00327B30);

//100%
INCLUDE_ASM("intersect/aifwddiff", func_00327BB0);
#ifdef SKIP_ASM
struct sFwdDiffNode {
    unsigned int key;
    int pad4;
    int pad8;
    sFwdDiffNode* next;
};

struct sFwdDiffTable {
    int pad0;
    int pad4;
    sFwdDiffNode** buckets;
    unsigned int count;
};

extern "C" void func_00327BB0(sFwdDiffTable* table, sFwdDiffNode* node)
{
    unsigned int key = node->key;
    sFwdDiffNode** link = &table->buckets[key % table->count];
    for (;;) {
        sFwdDiffNode* cur = *link;
        if (cur == 0 || key < cur->key) {
            *link = node;
            node->next = cur;
            return;
        }
        link = &cur->next;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", func_00327C00);
#ifdef SKIP_ASM
extern "C" void func_00391418(void* t, void* a);
extern "C" void func_00327C68(void* self, void* a);
extern "C" void func_003914F8(void* t);
extern "C" void func_00391480(void* t, void* a);
struct sGp2930 { int a, b; };
extern sGp2930 D_004A5A20; // placeholder: target is a raw $gp+0x2930 reference

extern "C" void func_00327C00(void* self, void* a1, void* a2)
{
    sGp2930* t = &D_004A5A20;
    func_00391418(t, (char*)a1 + 0x40);
    func_00327C68(self, a2);
    func_003914F8(t);
    func_00391480(t, (char*)a2 + 0x10);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", func_00327C68);
#ifdef SKIP_ASM
extern "C" void func_00327C68(void* self, void* a)
{
    float* p = (float*)((char*)a + 0x650);
    int n = 0x50;
    do {
        p[0] = 10000000000.0f;
        p[4] = 10000000000.0f;
        p[8] = 10000000000.0f;
        p += 16;
    } while (n-- > 0);
    p = (float*)((char*)a + 0x1A90);
    n = 8;
    do {
        p[0] = 10000000000.0f;
        p[4] = 10000000000.0f;
        p[8] = 10000000000.0f;
        p += 16;
    } while (n-- > 0);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", func_00327CC8);
#ifdef SKIP_ASM
struct sQuad_27CC8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBlock_27CC8 {
    int d[32];
};

struct sFwdDiff_27CC8 {
    sQuad_27CC8 m[8];       // 0x00
    sQuad_27CC8 q;          // 0x80
    char pad_0x90[0x8];
    sBlock_27CC8* blkp;     // 0x98
    char pad_0x9C[0x24];
    sBlock_27CC8 blk;       // 0xC0
};

extern "C" sFwdDiff_27CC8* func_00327CC8(sFwdDiff_27CC8* self, sFwdDiff_27CC8* src)
{
    int i;
    for (i = 0; i < 8; i++) {
        self->m[i] = src->m[i];
    }
    self->q = src->q;
    self->blk = src->blk;
    self->blkp = &self->blk;
    return self;
}
#endif

INCLUDE_ASM("intersect/aifwddiff", func_00327DA8);

//100%
INCLUDE_ASM("intersect/aifwddiff", func_00327F18);
#ifdef SKIP_ASM
extern "C" int func_0032DF28(void* self);
extern "C" int func_0032CDB0(void* self, void* pos, void* out, int a3, int a4, int a5, int a6, float radius);

struct sSphere_27F18 {
    float pos[4];
    float radius;
    int pad[3];
};
struct sShape_27F18 {
    char pad[0x10];
    float pos[4];           // 0x10
    float radius;           // 0x20
    int pad24[2];
    int count;              // 0x2C
    sSphere_27F18 subs[1];  // 0x30
};

extern "C" int func_00327F18(void* self, sShape_27F18* shape, int a2, int a3)
{
    void* m = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)m + 8) != 0) {
        *(int*)(*(char**)((char*)self + 0x98) + 0x28) = func_0032DF28(m);
    }
    if (func_0032CDB0(self, shape->pos, (char*)self + 0x80, 0, 0, a2, a3, shape->radius) == 0) {
        return 0;
    }
    if (shape->count == 0) {
        return 1;
    }
    for (int i = 0; i < shape->count; i++) {
        if (func_0032CDB0(self, shape->subs[i].pos, (char*)self + 0x80, 0, 0, a2, a3, shape->subs[i].radius) != 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", func_00328030);
#ifdef SKIP_ASM
extern "C" float func_0032C590(void* self);
extern "C" int func_0032CA78(void* self, void* a1, void* a2, void* a3, void* v, void* a5, void* a6);

struct sVec4_28030 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_28030 vu0Sub_28030(const sVec4_28030& a, const sVec4_28030& b)
{
    sVec4_28030 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_28030(const sVec4_28030& a, const sVec4_28030& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_28030 vu0Scale_28030(const sVec4_28030& v, float s)
{
    sVec4_28030 r;
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

extern "C" int func_00328030(void* self, sVec4_28030* p, void* a2, void* a3, sVec4_28030* dir, void* a5, void* a6)
{
    float dot;
    {
        sVec4_28030 d = vu0Sub_28030(*(sVec4_28030*)((char*)self + 0x80), *p);
        dot = vu0Dot_28030(*dir, d);
    }
    if (func_0032C590(self) < __builtin_fabsf(dot)) {
        return 0;
    }
    if (dot < 0.0f) {
        sVec4_28030 nd = vu0Scale_28030(*dir, -1.0f);
        return func_0032CA78(self, p, a2, a3, &nd, a5, a6);
    }
    return func_0032CA78(self, p, a2, a3, dir, a5, a6);
}
#endif

INCLUDE_ASM("intersect/aifwddiff", func_00328360);

INCLUDE_ASM("intersect/aifwddiff", func_003284B8);

INCLUDE_ASM("intersect/aifwddiff", func_00328660);

INCLUDE_ASM("intersect/aifwddiff", func_00328808);

INCLUDE_ASM("intersect/aifwddiff", func_00328C20);

INCLUDE_ASM("intersect/aifwddiff", func_00328F28);

INCLUDE_ASM("intersect/aifwddiff", func_003291E0);

INCLUDE_ASM("intersect/aifwddiff", func_00329590);


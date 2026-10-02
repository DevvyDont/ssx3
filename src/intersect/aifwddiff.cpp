#include "common.h"

INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_Init);

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

INCLUDE_ASM("intersect/aifwddiff", func_00327F18);

INCLUDE_ASM("intersect/aifwddiff", func_00328030);

INCLUDE_ASM("intersect/aifwddiff", func_00328360);

INCLUDE_ASM("intersect/aifwddiff", func_003284B8);

INCLUDE_ASM("intersect/aifwddiff", func_00328660);

INCLUDE_ASM("intersect/aifwddiff", func_00328808);

INCLUDE_ASM("intersect/aifwddiff", func_00328C20);

INCLUDE_ASM("intersect/aifwddiff", func_00328F28);

INCLUDE_ASM("intersect/aifwddiff", func_003291E0);

INCLUDE_ASM("intersect/aifwddiff", func_00329590);


#include "common.h"

INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_Init);

INCLUDE_ASM("intersect/aifwddiff", func_003279D0);

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

INCLUDE_ASM("intersect/aifwddiff", func_00327C00);

INCLUDE_ASM("intersect/aifwddiff", func_00327C68);

INCLUDE_ASM("intersect/aifwddiff", func_00327CC8);

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


#include "common.h"

INCLUDE_ASM("ui/uistate", cUIState_hideObjSafe);

INCLUDE_ASM("ui/uistate", cUIState_showObjSafe);

INCLUDE_ASM("ui/uistate", func_0039E9D8);

INCLUDE_ASM("ui/uistate", func_0039EA90);

//100%
INCLUDE_ASM("ui/uistate", func_0039EB60);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
cListNode* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct func_0039EB60_node {
    char pad0[0x4];
    func_0039EB60_node* next; // 0x4
    char pad8[0x1C - 0x8];
    int flags; // 0x1C
};

extern "C" int func_0039EB60(cList* list, signed char id)
{
    func_0039EB60_node* n = (func_0039EB60_node*)cList_first(list);
    if (n != 0) {
        do {
            if ((((unsigned int)n->flags >> 8) & 0x3F) == id && ((n->flags >> 6) & 1) == 0) {
                return 1;
            }
            n = n->next;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    return 0;
}
#endif

INCLUDE_ASM("ui/uistate", func_0039EBE0);

//100%
INCLUDE_ASM("ui/uistate", func_0039ECA8__FPv);
#ifdef SKIP_ASM
void func_0039ECA8(void* self)
{
}
#endif

INCLUDE_ASM("ui/uistate", func_0039ECB0);

INCLUDE_ASM("ui/uistate", func_0039F100);

//100%
INCLUDE_ASM("ui/uistate", func_0039F190);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
cListNode* cList_first(cList*);

struct func_0039F190_node {
    char pad0[0x4];
    func_0039F190_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int b0 : 6;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

static inline void func_0039F190_setId(func_0039F190_node* n, int v) { n->id = v; }
static inline void func_0039F190_setB7(func_0039F190_node* n, int v) { n->b7 = v; }

extern "C" func_0039F190_node* func_0039F190(cList* list, int count)
{
    func_0039F190_node* n = (func_0039F190_node*)cList_first(list);
    int i;
    for (i = 0; i < count; i++) {
        if (n == 0) {
            goto done;
        }
        func_0039F190_setId(n, 6);
        func_0039F190_setB7(n, 1);
        n = n->next;
    }
done:
    return n;
}
#endif


#include "common.h"

INCLUDE_ASM("camera/cameraalgolist", cCameraAlgoList_insert);

INCLUDE_ASM("camera/cameraalgolist", func_0015CA50);

//100%
INCLUDE_ASM("camera/cameraalgolist", func_0015CB08);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

struct sAlgoVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sAlgoNode {
    char* algo;
    int pad_04[4];
    sAlgoNode* next;
    int owned;
};

struct sAlgoList {
    sAlgoNode* head;
    int count;
};

static inline void sAlgoNode_delete(sAlgoNode* n)
{
    if (n != 0) {
        char* algo = n->algo;
        if (algo != 0 && n->owned != 0) {
            sAlgoVEntry* vt = *(sAlgoVEntry**)(algo + 0x10);
            vt[1].fn(algo + vt[1].delta, 3);
        }
        n->algo = 0;
        n->owned = 0;
        operator_delete((int*)n);
    }
}

extern "C" void func_0015CB08(sAlgoList* list)
{
    sAlgoNode* n = list->head;
    while (n != 0) {
        sAlgoNode* next = n->next;
        sAlgoNode_delete(n);
        n = next;
    }
    list->count = 0;
    list->head = 0;
}
#endif


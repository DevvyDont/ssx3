#include "common.h"

//100%
INCLUDE_ASM("camera/cameraalgolist", cCameraAlgoList_insert);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char D_0045B1F0[];

class cCamAlgoK2 {
public:
    char pad_0x00[0x10];
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual int v04();
};

struct sAlgoNodeK2 {
    cCamAlgoK2* algo;   // 0x0
    float weight;       // 0x4
    float target;       // 0x8
    int pad_0C;
    sAlgoNodeK2* prev;  // 0x10
    sAlgoNodeK2* next;  // 0x14
    int owned;          // 0x18
};

struct sAlgoListK2 {
    sAlgoNodeK2* head;
    int count;
};

extern "C" void cCameraAlgoList_insert(sAlgoListK2* list, cCamAlgoK2* algo, int owned, float w)
{
    algo->v04();
    sAlgoNodeK2* n = (sAlgoNodeK2*)cMemMan_alloc(0x1C, D_0045B1F0, 0x20000000, 0);
    n->owned = owned;
    n->algo = algo;
    n->prev = 0;
    n->next = 0;
    if (w == 1.0f)
        n->weight = 1.0f;
    else
        n->weight = 0.0f;
    sAlgoNodeK2* h = list->head;
    n->target = w;
    if (h != 0)
        h->prev = n;
    sAlgoNodeK2* old = list->head;
    list->head = n;
    list->count++;
    n->next = old;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgolist", func_0015CA50);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

struct sAlgoVEntryK2 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sAlgoNodeK2b {
    char* algo;           // 0x0
    int pad_04[3];
    sAlgoNodeK2b* prev;   // 0x10
    sAlgoNodeK2b* next;   // 0x14
    int owned;            // 0x18
};

struct sAlgoListK2b {
    sAlgoNodeK2b* head;
    int count;
};

static inline void sAlgoNodeK2b_delete(sAlgoNodeK2b* n)
{
    if (n != 0) {
        char* algo = n->algo;
        if (algo != 0 && n->owned != 0) {
            sAlgoVEntryK2* vt = *(sAlgoVEntryK2**)(algo + 0x10);
            vt[1].fn(algo + vt[1].delta, 3);
        }
        n->algo = 0;
        n->owned = 0;
        operator_delete((int*)n);
    }
}

extern "C" sAlgoNodeK2b* func_0015CA50(sAlgoListK2b* list, sAlgoNodeK2b* n)
{
    sAlgoNodeK2b* next = n->next;
    if (n->prev == 0)
        list->head = next;
    else
        n->prev->next = next;
    if (n->next != 0)
        n->next->prev = n->prev;
    sAlgoNodeK2b_delete(n);
    list->count--;
    return next;
}
#endif

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


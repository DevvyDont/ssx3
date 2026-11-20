#include "common.h"
#include "camera/cameraalgolist.h"

//100%
INCLUDE_ASM("camera/cameraalgolist", cCameraAlgoList_insert);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char str_Algorithm_List_Node[];

class cCamAlgoK2 {
public:
    char pad_0x00[0x10];
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual int snapCurrent();
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

/**
 * Adds a camera algorithm to the front of an algorithm list with a target blend weight.
 *
 * Calls the algorithm's virtual at slot 4 first (empty in cCameraAlgorithm, cLookAtChaseAlgorithm_snapCurrent in
 * cLookAtChaseAlgorithm). It then allocates an "Algorithm List Node" whose current weight is 1 when @p w is
 * exactly 1 and 0 otherwise, with @p w as its target weight, pushes it on the front and bumps the count.
 *
 * @param list   the algorithm list
 * @param algo   algorithm to add
 * @param owned  nonzero if removing the node should also destroy the algorithm
 * @param w      target weight
 */
extern "C" void cCameraAlgoList_insert(cCameraAlgoList* list, cCamAlgoK2* algo, int owned, float w)
{
    algo->snapCurrent();
    sAlgoNodeK2* n = (sAlgoNodeK2*)cMemMan_alloc(0x1C, str_Algorithm_List_Node, 0x20000000, 0);
    n->owned = owned;
    n->algo = algo;
    n->prev = 0;
    n->next = 0;
    if (w == 1.0f)
        n->weight = 1.0f;
    else
        n->weight = 0.0f;
    sAlgoNodeK2* oldHead = (sAlgoNodeK2*)list->Head;
    n->target = w;
    if (oldHead != 0)
        oldHead->prev = n;
    sAlgoNodeK2* old = (sAlgoNodeK2*)list->Head;
    list->Head = (cCameraAlgoListNode*)n;
    list->Length++;
    n->next = old;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgolist", cCameraAlgoList_remove);
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

/**
 * Removes a node from a camera algorithm list and returns the node after it.
 *
 * Unlinks @p n (the head moves on when it has no previous node), deletes it: its algorithm too, through the algorithm's
 * virtual destructor (slot 1, flags 3), when the node owns it (+0x18), then frees the node with operator_delete, and
 * decrements the list's count.
 *
 * @param list  the list
 * @param n  the node to remove
 * @return the node that followed @p n (0 at the end)
 */
extern "C" sAlgoNodeK2b* cCameraAlgoList_remove(cCameraAlgoList* list, sAlgoNodeK2b* n)
{
    sAlgoNodeK2b* next = n->next;
    if (n->prev == 0)
        list->Head = (cCameraAlgoListNode*)next;
    else
        n->prev->next = next;
    if (n->next != 0)
        n->next->prev = n->prev;
    sAlgoNodeK2b_delete(n);
    list->Length--;
    return next;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgolist", cCameraAlgoList_removeAll);
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

/**
 * Empties an algorithm list: deletes every node and the algorithms the list owns.
 *
 * For each node (next at +0x14), deletes its algorithm (+0x0) through the destructor virtual (slot 1, flags 3)
 * when the node's owned flag (+0x18) is set, clears both fields and frees the node with operator_delete. Finally
 * zeroes the count (+0x4) and head (+0x0).
 *
 * @param list  the algorithm list (this)
 */
extern "C" void cCameraAlgoList_removeAll(cCameraAlgoList* list)
{
    sAlgoNode* n = (sAlgoNode*)list->Head;
    while (n != 0) {
        sAlgoNode* next = n->next;
        sAlgoNode_delete(n);
        n = next;
    }
    list->Length = 0;
    list->Head = 0;
}
#endif


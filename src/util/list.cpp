#include "common.h"

#include "util/list.h"

//100%
INCLUDE_ASM("util/list", cListNode_isSentinel__FP9cListNode);
#ifdef SKIP_ASM
/**
 * Tells whether a list node is an end marker, i.e. links to itself in either direction.
 *
 * @param node  node to test
 * @return 1 if node->next or node->prev points back at the node, else 0
 */
int cListNode_isSentinel(cListNode* node)
{
    return (node->mpNext == node) || (node->mpPrev == node);
}
#endif

//100%
INCLUDE_ASM("util/list", cListNode_removeFromList);
#ifdef SKIP_ASM
/**
 * Unlinks the node from its neighbours and leaves it linked only to itself.
 *
 * Neighbour links that are null or point at the node are skipped. When the node was a self-linked end on one
 * side, the neighbour on the other side becomes self-linked on that side, so it is the new end.
 */
extern "C" void cListNode_removeFromList(cListNode* self)
{
    cListNode* prevNode = self->mpPrev;
    cListNode* nextNode = self->mpNext;
    if (prevNode != 0 && prevNode != self) {
        if (nextNode == self) {
            prevNode->mpNext = prevNode;
        } else {
            prevNode->mpNext = nextNode;
        }
    }
    if (nextNode != 0 && nextNode != self) {
        if (prevNode == self) {
            nextNode->mpPrev = nextNode;
        } else {
            nextNode->mpPrev = prevNode;
        }
    }
    self->mpNext = self;
    self->mpPrev = self;
}
#endif

//100%
INCLUDE_ASM("util/list", func_00397788);
#ifdef SKIP_ASM
class func_00397788_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

extern "C" int func_00397788(void* a, func_00397788_cVirt* obj)
{
    return obj->v02(a);
}
#endif

//100%
INCLUDE_ASM("util/list", func_003977B8);
#ifdef SKIP_ASM
class func_003977B8_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

extern "C" int func_003977B8(void* a, func_003977B8_cVirt* obj)
{
    return obj->v02(a);
}
#endif

//100%
INCLUDE_ASM("util/list", func_003977E8);
#ifdef SKIP_ASM
void func_00397930(void*);
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];

extern "C" void* func_003977E8(void* self)
{
    char* s = (char*)self;
    *(void**)(s + 0x4) = s;
    *(void**)(s + 0x0) = s;
    *(void***)(s + 0x18) = cList_vtbl;
    *(void***)(s + 0x8) = cListNode_vtbl;
    *(void**)(s + 0x10) = s + 0xC;
    *(void***)(s + 0x14) = cListNode_vtbl;
    *(void**)(s + 0xC) = s + 0xC;
    func_00397930(s);
    return self;
}
#endif

//100%
INCLUDE_ASM("util/list", cList_first__FP5cList);
#ifdef SKIP_ASM
/**
 * Returns the list's first node.
 *
 * @param list  list to read
 * @return the first node (list->mStart.mpNext), or 0 when the head's next link points at itself (an empty list)
 */
cListNode* cList_first(cList* list)
{
    cListNode* head = list->mStart.mpNext;
    return (head->mpNext == head) ? 0 : head;
}
#endif

//100%
INCLUDE_ASM("util/list", func_00397858);
#ifdef SKIP_ASM
extern "C" int *func_00397858(void *arg0) {
    int *temp_2;

    temp_2 = (*(int **)((char*)(arg0) + (0xC)));
    return ((*temp_2 ^ (int) temp_2) == 0) ? 0 : temp_2;
}
#endif

//100%
INCLUDE_ASM("util/list", func_00397870);
#ifdef SKIP_ASM
extern "C" cListNode* func_00397870(cList* list, int count)
{
    cListNode* n = cList_first(list);
    for (; count > 0; count--) {
        if (n == 0) {
            return 0;
        }
        if (cListNode_isSentinel(n)) {
            break;
        }
        n = n->mpNext;
    }
    if (n != 0 && !cListNode_isSentinel(n)) {
        return n;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/list", func_003978E8);
#ifdef SKIP_ASM
extern "C" int func_003978E8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/list", cList_addToFront__FP5cListP9cListNode);
#ifdef SKIP_ASM
/**
 * Links a node in at the head of the list and makes it the first node (list->mStart.mpNext).
 *
 * Sets newNode->next to the old head, newNode->prev to the old head's prev, and the old head's prev to newNode.
 * In this list, next links run from the head towards the tail (cList_size() walks them).
 *
 * @param list     list to add to
 * @param newNode  node to insert
 */
void cList_addToFront(cList* list, cListNode* newNode)
{
    cListNode* head = list->mStart.mpNext;
    newNode->mpNext = head;
    newNode->mpPrev = head->mpPrev;
    head->mpPrev = newNode;
    list->mStart.mpNext = newNode;
}
#endif

//100%
INCLUDE_ASM("util/list", cList_addToEnd__FP5cListP9cListNode);
#ifdef SKIP_ASM
/**
 * Links a node in at the tail of the list and makes it the last node (list->mEnd.mpPrev).
 *
 * Sets newNode->prev to the old tail, newNode->next to the old tail's next, and the old tail's next to newNode.
 * In this list, prev links run from the tail towards the head.
 *
 * @param list     list to add to
 * @param newNode  node to insert
 */
void cList_addToEnd(cList* list, cListNode* newNode)
{
    cListNode* tail = list->mEnd.mpPrev;
    newNode->mpPrev = tail;
    newNode->mpNext = tail->mpNext;
    tail->mpNext = newNode;
    list->mEnd.mpPrev = newNode;
}
#endif

//100%
INCLUDE_ASM("util/list", func_00397930__FPv);
#ifdef SKIP_ASM
void func_00397930(void* self)
{
    void* t0 = (char*)self + 0xc;
    *(void**)((char*)self + 0x4) = t0;
    *(void**)((char*)self + 0xc) = self;
    *(void**)((char*)self + 0x10) = t0;
    *(void**)self = self;
}
#endif

//100%
INCLUDE_ASM("util/list", cList_destroyList);
#ifdef SKIP_ASM
class cList_destroyList_cVirt {
public:
    void* mpPrev;                  // 0x0
    cList_destroyList_cVirt* mpNext;   // 0x4
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01(int);
};

/**
 * Deletes every node in the list through its virtual destructor, then resets the list to empty.
 *
 * Walks from the head (+0x4) along the next links up to the self-linked end node, calling each node's
 * deleting destructor (slot 1, flags 3), then relinks the empty list (func_00397930). The null check on each
 * node comes after its next link has been read.
 */
extern "C" void cList_destroyList(void* self)
{
    cList_destroyList_cVirt* n = *(cList_destroyList_cVirt**)((char*)self + 0x4);
    while (n->mpNext != n) {
        cList_destroyList_cVirt* nextNode = n->mpNext;
        if (n != 0) {
            n->v01(3);
        }
        n = nextNode;
    }
    func_00397930(self);
}
#endif

//100%
INCLUDE_ASM("util/list", cList_size);
#ifdef SKIP_ASM
/**
 * Counts the nodes in the list.
 *
 * Follows next links from the head until a node whose next points at itself; that end node is not counted.
 *
 * @param list  list to count
 * @return the number of nodes
 */
extern "C" int cList_size(cList* list)
{
    cListNode* node = list->mStart.mpNext;
    int count = 0;
    while (node->mpNext != node) {
        node = node->mpNext;
        count++;
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("util/list", cList_traverseFlat);
#ifdef SKIP_ASM
class cList_traverseFlat_cVirt {
public:
    void* mpPrev;                  // 0x0
    cList_traverseFlat_cVirt* mpNext;   // 0x4
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

/**
 * Applies an operator to each node of the list, without visiting sub-lists, until one returns nonzero.
 *
 * Walks the nodes from the one at +0x4 along their link to the sentinel (a node linked to itself), reading the next
 * link first, and calls each node's slot 2 with the operator.
 *
 * @param pOperator  the operator to apply (On Tour cListOperator* pOperator)
 * @return the first nonzero node result, or 0 (On Tour returns void*)
 */
extern "C" int cList_traverseFlat(void* self, void* pOperator)
{
    cList_traverseFlat_cVirt* n = *(cList_traverseFlat_cVirt**)((char*)self + 0x4);
    while (n->mpNext != n) {
        cList_traverseFlat_cVirt* nextNode = n->mpNext;
        int result = n->v02(pOperator);
        if (result != 0) {
            return result;
        }
        n = nextNode;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/list", cList_traverseRecurse);
#ifdef SKIP_ASM
class cList_traverseRecurse_cVirt {
public:
    void* mpPrev;                  // 0x0
    cList_traverseRecurse_cVirt* mpNext;   // 0x4
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03(void*);
};

/**
 * Applies an operator to each node of the list and its sub-lists, until one returns nonzero.
 *
 * The same walk as cList_traverseFlat, calling each node's slot 3 (the recursive form) instead.
 *
 * @param pOperator  the operator to apply (On Tour cListOperator* pOperator)
 * @return the first nonzero node result, or 0 (On Tour returns void*)
 */
extern "C" int cList_traverseRecurse(void* self, void* pOperator)
{
    cList_traverseRecurse_cVirt* n = *(cList_traverseRecurse_cVirt**)((char*)self + 0x4);
    while (n->mpNext != n) {
        cList_traverseRecurse_cVirt* nextNode = n->mpNext;
        int result = n->v03(pOperator);
        if (result != 0) {
            return result;
        }
        n = nextNode;
    }
    return 0;
}
#endif


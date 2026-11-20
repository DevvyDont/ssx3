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
INCLUDE_ASM("util/list", cListNode_traverseFlat);
#ifdef SKIP_ASM
class cListNode_traverseFlat_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int onListNode(void*);
};

/**
 * Applies a list operator to this single node.
 *
 * A plain node has no children, so it just calls virtual slot 2 of @p obj (On Tour cListOperator::onListNode) with this
 * node and returns the result.
 *
 * @param obj  the operator to apply (On Tour pOperator)
 * @return the operator's result
 */
extern "C" int cListNode_traverseFlat(void* self, cListNode_traverseFlat_cVirt* obj)
{
    return obj->onListNode(self);
}
#endif

//100%
INCLUDE_ASM("util/list", cListNode_traverseRecurse);
#ifdef SKIP_ASM
class cListNode_traverseRecurse_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int onListNode(void*);
};

/**
 * Applies a list operator to this node and its children; a plain node has none.
 *
 * Calls virtual slot 2 of @p obj (On Tour cListOperator::onListNode) with this node and returns the result, exactly
 * like cListNode_traverseFlat.
 *
 * @param obj  the operator to apply (On Tour pOperator)
 * @return the operator's result
 */
extern "C" int cListNode_traverseRecurse(void* self, cListNode_traverseRecurse_cVirt* obj)
{
    return obj->onListNode(self);
}
#endif

//100%
INCLUDE_ASM("util/list", cList_cList);
#ifdef SKIP_ASM
void cList_clear(void*);
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];

/**
 * Constructs an empty intrusive list.
 *
 * Points the list's own node links at itself, sets cList_vtbl at +0x18 and cListNode_vtbl at +0x8 and +0x14, links the
 * sentinel node at +0xC to itself and calls cList_clear__FPv to empty the list.
 *
 * @return this
 */
extern "C" void* cList_cList(cList* self)
{
    self->mStart.mpNext = (cListNode*)((char*)self);
    *(void**)&self->mStart = ((char*)self);
    self->mVtbl = cList_vtbl;
    self->mStart.mVtbl = cListNode_vtbl;
    self->mEnd.mpNext = (cListNode*)(((char*)self) + 0xC);
    self->mEnd.mVtbl = cListNode_vtbl;
    *(void**)&self->mEnd = ((char*)self) + 0xC;
    cList_clear(((char*)self));
    return ((void*)self);
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
INCLUDE_ASM("util/list", cList_last);
#ifdef SKIP_ASM
/**
 * Returns the last node of a list: the end sentinel's mpPrev (mEnd.mpPrev, +0xC), or 0 when that node's mpPrev points
 * to itself, which only the start sentinel of an empty list does.
 *
 * @param list  the cList
 * @return the last node, or 0 for an empty list
 */
extern "C" int *cList_last(cList* list) {
    int *temp_2;

    temp_2 = (*(int**)&list->mEnd);
    return ((*temp_2 ^ (int) temp_2) == 0) ? 0 : temp_2;
}
#endif

//100%
INCLUDE_ASM("util/list", cList_getByIndex);
#ifdef SKIP_ASM
/**
 * Returns the list node at a given position.
 *
 * Starts at cList_first and follows the next links @p count times, stopping early at a null node or the sentinel.
 *
 * @param list   the list (this)
 * @param count  zero-based position (On Tour index)
 * @return the node, or 0 when it is null or the sentinel (the list is shorter)
 */
extern "C" cListNode* cList_getByIndex(cList* list, int count)
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
/**
 * Returns 0. Nothing in retail references it.
 *
 * @return 0
 */
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
INCLUDE_ASM("util/list", cList_clear__FPv);
#ifdef SKIP_ASM
/**
 * Resets a list to empty without destroying its nodes.
 *
 * Points the list head (+0x0) at itself and the tail (+0x4) at the sentinel node (+0xC), whose next/prev (+0xC, +0x10)
 * point back at the head and itself.
 */
void cList_clear(void* self)
{
    void* sentinel = (char*)self + 0xc;
    *(void**)((char*)self + 0x4) = sentinel;
    *(void**)((char*)self + 0xc) = self;
    *(void**)((char*)self + 0x10) = sentinel;
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
 * deleting destructor (slot 1, flags 3), then relinks the empty list (cList_clear). The null check on each
 * node comes after its next link has been read.
 */
extern "C" void cList_destroyList(cList* self)
{
    cList_destroyList_cVirt* n = (cList_destroyList_cVirt*)self->mStart.mpNext;
    while (n->mpNext != n) {
        cList_destroyList_cVirt* nextNode = n->mpNext;
        if (n != 0) {
            n->v01(3);
        }
        n = nextNode;
    }
    cList_clear(self);
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
    virtual int traverseFlat(void*);
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
extern "C" int cList_traverseFlat(cList* self, cListOperator* pOperator)
{
    cList_traverseFlat_cVirt* n = (cList_traverseFlat_cVirt*)self->mStart.mpNext;
    while (n->mpNext != n) {
        cList_traverseFlat_cVirt* nextNode = n->mpNext;
        int result = n->traverseFlat(pOperator);
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
    virtual int traverseRecurse(void*);
};

/**
 * Applies an operator to each node of the list and its sub-lists, until one returns nonzero.
 *
 * The same walk as cList_traverseFlat, calling each node's slot 3 (the recursive form) instead.
 *
 * @param pOperator  the operator to apply (On Tour cListOperator* pOperator)
 * @return the first nonzero node result, or 0 (On Tour returns void*)
 */
extern "C" int cList_traverseRecurse(cList* self, cListOperator* pOperator)
{
    cList_traverseRecurse_cVirt* n = (cList_traverseRecurse_cVirt*)self->mStart.mpNext;
    while (n->mpNext != n) {
        cList_traverseRecurse_cVirt* nextNode = n->mpNext;
        int result = n->traverseRecurse(pOperator);
        if (result != 0) {
            return result;
        }
        n = nextNode;
    }
    return 0;
}
#endif


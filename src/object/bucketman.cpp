#include "vtable.h"
#include "common.h"
#include "object/movenode.h"
#include "object/objnode.h"
#include "object/bucketman.h"


//100%
INCLUDE_ASM("object/bucketman", cBucketMan_init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char str_Buckets[];
extern char cObjNode_vtbl[];

struct sBucketList_3549E0 {
    int f0;
    sBucketList_3549E0* prev;
    sBucketList_3549E0* next;
    void* vt;
};

struct sBucket_3549E0 {
    int flags;
    sBucketList_3549E0 Head;
    sBucketList_3549E0 Tail;
    sBucketList_3549E0 AddHead;
    sBucketList_3549E0 AddTail;
    sBucket_3549E0()
    {
        Head.vt = cObjNode_vtbl;
        Head.prev = 0;
        Head.next = 0;
        Tail.vt = cObjNode_vtbl;
        Tail.prev = 0;
        Tail.next = 0;
        AddHead.vt = cObjNode_vtbl;
        AddHead.prev = 0;
        AddHead.next = 0;
        AddTail.vt = cObjNode_vtbl;
        AddTail.prev = 0;
        AddTail.next = 0;
    }
    ~sBucket_3549E0() {}
};

/**
 * Allocates the manager's buckets and gives each an empty node list and an empty pending-add list.
 *
 * Allocates @p n 0x44-byte buckets ("Buckets"), each a Flags word and four cObjNode sentinels (On Tour
 * tBucketHeader Head, Tail, AddHead, AddTail) with cObjNode's vtable. Stores @p n in NumBuckets (+0x0), links
 * each Head/Tail and AddHead/AddTail pair to each other and clears Flags.
 *
 * @param n  number of buckets (On Tour numBuckets)
 */
extern "C" void cBucketMan_init(cBucketMan* self, int n)
{
    char** slot = (char**)&self->Buckets;
    *slot = (char*)new (str_Buckets, 0, 0) sBucket_3549E0[n];
    *(int*)self = n;
    for (int i = 0; i < *(int*)self; i++) {
        ((sBucket_3549E0*)self->Buckets)[i].Head.prev = &((sBucket_3549E0*)self->Buckets)[i].Tail;
        ((sBucket_3549E0*)self->Buckets)[i].Head.next = 0;
        ((sBucket_3549E0*)self->Buckets)[i].Tail.prev = 0;
        ((sBucket_3549E0*)self->Buckets)[i].Tail.next = &((sBucket_3549E0*)self->Buckets)[i].Head;
        ((sBucket_3549E0*)self->Buckets)[i].AddHead.prev = &((sBucket_3549E0*)self->Buckets)[i].AddTail;
        ((sBucket_3549E0*)self->Buckets)[i].AddHead.next = 0;
        ((sBucket_3549E0*)self->Buckets)[i].AddTail.prev = 0;
        ((sBucket_3549E0*)self->Buckets)[i].AddTail.next = &((sBucket_3549E0*)self->Buckets)[i].AddHead;
        ((sBucket_3549E0*)self->Buckets)[i].flags = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_purge);
#ifdef SKIP_ASM
extern "C" void cBucketMan_purgeBucket(cBucketMan* self, int index);
extern "C" void cObjNode__cObjNode(void* self, int flags);
void cMemMan_free(void*);

/**
 * Empties every bucket and frees the bucket array.
 *
 * Purges buckets 0 .. count - 1 (cBucketMan_purgeBucket), then destroys the 0x44-byte buckets in reverse (their four
 * cObjNode list heads), frees the array (with its new[] header) and clears mBuckets and the count.
 */
extern "C" void cBucketMan_purge(cBucketMan* self)
{
    int i;
    for (i = 0; i < *(int*)self; i++) {
        cBucketMan_purgeBucket(self, i);
    }
    char* buckets = ((char*)self->Buckets);
    if (buckets != 0) {
        char* bucket = buckets + ((int*)buckets)[-4] * 68;
        while (((char*)self->Buckets) != bucket) {
            bucket -= 0x44;
            cObjNode__cObjNode(bucket + 0x34, 2);
            cObjNode__cObjNode(bucket + 0x24, 2);
            cObjNode__cObjNode(bucket + 0x14, 2);
            cObjNode__cObjNode(bucket + 0x4, 2);
        }
        cMemMan_free(((char*)self->Buckets) - 0x10);
    }
    *(char**)&self->Buckets = 0;
    *(int*)self = 0;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_add);
#ifdef SKIP_ASM
struct sBucketLink {
    int field_0x0;
    sBucketLink* prev; // 0x4
    sBucketLink* next; // 0x8
};


/**
 * Appends a scene node to the end of one of a bucket's two node lists.
 *
 * Each bucket is a 0x44-byte record; bit 0 of its flags picks the list headed at +0x24, otherwise the list at
 * +0x4 is used.
 *
 * @param node   the node to add
 * @param index  the bucket index
 */
extern "C" void cBucketMan_add(cBucketMan* self, sBucketLink* node, int index)
{
    if ((self->Buckets)[index].Flags & 1) {
        node->prev = (*(sBucketLink*)&(self->Buckets)[index].AddHead).prev;
        (*(sBucketLink*)&(self->Buckets)[index].AddHead).prev->next = node;
        node->next = (sBucketLink*)&self->Buckets[index].AddHead;
        (*(sBucketLink*)&(self->Buckets)[index].AddHead).prev = node;
    } else {
        node->prev = (*(sBucketLink*)&(self->Buckets)[index].Head).prev;
        (*(sBucketLink*)&(self->Buckets)[index].Head).prev->next = node;
        node->next = (sBucketLink*)&self->Buckets[index].Head;
        (*(sBucketLink*)&(self->Buckets)[index].Head).prev = node;
    }
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_mergeBucket);
#ifdef SKIP_ASM
void* cBucketMan_addfirst(cBucketMan* self, int index);
extern "C" void* cBucketMan_last(cBucketMan* self, int index);
extern "C" void* cBucketMan_prev(cBucketMan* self, cObjNode* node, int index);
extern "C" void* cBucketMan_addnext(cBucketMan* self, cObjNode* node, int index);

struct sSortVE_4C98 {
    short delta;
    short index;
    int (*fn)(void*, void*);
};





/**
 * Moves every node of a bucket's pending-add list into its main list, kept in decreasing sort-index order.
 *
 * Each pending node is unlinked and, searching from the tail with its virtual compare (slot 6), placed after
 * the last node whose sort index is at least its own, or at the front when there is none. A node with an
 * equal sort index (virtual equal, slot 7) takes the old node's place; the old one is deleted (slot 1, flags
 * 3).
 *
 * @param index  bucket index
 */
// Moves every node of the bucket's pending list into its sorted list.
extern "C" void cBucketMan_mergeBucket(cBucketMan* self, int index)
{
    cObjNode* n = (cObjNode*)cBucketMan_addfirst(self, index);
    while (n) {
        cObjNode* nextN = (cObjNode*)cBucketMan_addnext(self, (cObjNode*)n, index);
        n->Prev->Next = n->Next;
        n->Next->Prev = n->Prev;
        cObjNode* other;
        for (other = (cObjNode*)cBucketMan_last(self, index); other; other = (cObjNode*)cBucketMan_prev(self, (cObjNode*)other, index)) {
            if (((sSortVE_4C98*)VPTR_OF(cObjNode, n))[6].fn(vthis(n, ((sSortVE_4C98*)VPTR_OF(cObjNode, n))[6]), other)) {
                if (((sSortVE_4C98*)VPTR_OF(cObjNode, n))[7].fn(vthis(n, ((sSortVE_4C98*)VPTR_OF(cObjNode, n))[7]), other)) {
                    n->Next = other->Next;
                    other->Next->Prev = n;
                    n->Prev = other->Prev;
                    other->Prev->Next = n;
                    other->Next = 0;
                    other->Prev = 0;
                    if (other) {
                        sVEntry<void(void*, int)>* dv = (sVEntry<void(void*, int)>*)((sSortVE_4C98*)VPTR_OF(cObjNode, other));
                        dv[1].fn(vthis(other, dv[1]), 3);
                    }
                } else {
                    n->Next = other->Next;
                    other->Next->Prev = n;
                    n->Prev = other;
                    other->Next = n;
                }
                break;
            }
        }
        if (other == 0) {
            n->Next = ((*(&((tBucketHeader*)self->Buckets)[index].Head)).Next);
            ((*(&((tBucketHeader*)self->Buckets)[index].Head)).Next)->Prev = n;
            n->Prev = (&self->Buckets[index].Head);
            (*(cObjNode**)&(*(&((tBucketHeader*)self->Buckets)[index].Head)).Next) = n;
        }
        n = nextN;
    }
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_remove);
#ifdef SKIP_ASM
struct sBucketNode {
    int field_0x0;
    sBucketNode* prev; // 0x4
    sBucketNode* next; // 0x8
};

/**
 * Unlinks a node from its bucket list.
 *
 * Only a node with both a previous and a next neighbour is unlinked; a node at either end stays linked. The
 * node's own links are not cleared, and @p self is not used.
 *
 * @param node  node to remove
 */
extern "C" void cBucketMan_remove(cBucketMan* self, sBucketNode* node)
{
    if (node->prev != 0 && node->next != 0) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_unlink);
#ifdef SKIP_ASM
/**
 * Removes @p node from its bucket with cBucketMan_remove and then clears the node's prev and next links.
 *
 * @param node  the scene node to unlink
 */
extern "C" void cBucketMan_unlink(cBucketMan* self, sBucketNode* node)
{
    cBucketMan_remove(self, node);
    node->prev = 0;
    node->next = 0;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_first__FP10cBucketMani);
#ifdef SKIP_ASM
/**
 * Returns the first node of a bucket's main list, or 0 when the list is empty.
 *
 * Reads Head's Next link (bucket +0x8) and compares it with the Tail sentinel (bucket +0x14).
 *
 * @param index  bucket index
 * @return the first node, or 0
 */
// PORT: holds a pointer in an int (needed to match); not 64-bit safe.
void* cBucketMan_first(cBucketMan* self, int index)
{
    index *= 68;
    index += (int)((char*)self->Buckets);
    void* head = *(void**)(index + 0x8);
    return (head == (void*)(index + 0x14)) ? 0 : head;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_last);
#ifdef SKIP_ASM
/**
 * Returns the last node of one bucket's list, or 0 when the bucket is empty.
 *
 * Buckets are 0x44-byte headers (Head sentinel at +0x4, Tail at +0x14); the tail's previous link (+0x1C) pointing
 * back at Head means empty.
 *
 * @param index  the bucket (On Tour bucket)
 * @return the last node (On Tour cObjNode*), or 0
 */
// PORT: holds a pointer in an int (same shape as cBucketMan_first); not 64-bit safe.
extern "C" void* cBucketMan_last(cBucketMan* self, int index)
{
    index *= 68;
    index += (int)((char*)self->Buckets);
    void* tail = *(void**)(index + 0x1C);
    return (tail == (void*)(index + 0x4)) ? 0 : tail;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_next__FP10cBucketManPvi);
#ifdef SKIP_ASM
/**
 * Returns the node after @p node in a bucket's main list, or 0 at the end of the list.
 *
 * Follows the node's Next link (+0x4) and stops at the bucket's Tail sentinel (bucket +0x14).
 *
 * @param node   the current node
 * @param index  bucket index
 * @return the next node, or 0
 */
void* cBucketMan_next(cBucketMan* self, void* vnode, int index)
{
    cObjNode* node = (cObjNode*)vnode;
    tBucketHeader* bucket = &self->Buckets[index];
    void* next = node->Next;
    return (next == (void*)(&bucket->Tail)) ? 0 : next;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_prev);
#ifdef SKIP_ASM
/**
 * Returns the node before @p node in a bucket's list, or 0 at the start.
 *
 * @param node  the current node (On Tour cObjNode*); its previous link is at +0x8
 * @param index  the bucket (On Tour bucket)
 * @return the previous node, or 0 when it is the bucket's Head sentinel
 */
extern "C" void* cBucketMan_prev(cBucketMan* self, cObjNode* node, int index)
{
    void* next = node->Prev;
    return (next == (void*)&self->Buckets[index].Head) ? 0 : next;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_addfirst__FP10cBucketMani);
#ifdef SKIP_ASM
/**
 * Returns the first node of a bucket's pending-add list, or 0 when that list is empty.
 *
 * Reads AddHead's Next link (bucket +0x28) and compares it with the AddTail sentinel (bucket +0x34).
 *
 * @param index  bucket index
 * @return the first pending node, or 0
 */
// PORT: holds a pointer in an int (needed to match); not 64-bit safe.
void* cBucketMan_addfirst(cBucketMan* self, int index)
{
    index *= 68;
    index += (int)((char*)self->Buckets);
    void* head = *(void**)(index + 0x28);
    return (head == (void*)(index + 0x34)) ? 0 : head;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_addnext);
#ifdef SKIP_ASM
/**
 * Returns the node after @p node in a bucket's pending-add list, or 0 at its end.
 *
 * @param node  the current node (On Tour cObjNode*); its next link is at +0x4
 * @param index  the bucket (On Tour bucket)
 * @return the next node, or 0 when it is the add list's tail sentinel (bucket + 0x34)
 */
extern "C" void* cBucketMan_addnext(cBucketMan* self, cObjNode* node, int index)
{
    void* next = node->Next;
    return (next == (void*)&self->Buckets[index].AddTail) ? 0 : next;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/bucketman", cBucketMan_updateBucket);
#ifdef SKIP_ASM

/**
 * Runs the update virtual (slot 2) of every node in one bucket, reading each node's successor first so a node may
 * unlink itself.
 *
 * A final cBucketMan_first() call's result is unused.
 *
 * @param index  the bucket (On Tour bucket)
 */
extern "C" void cBucketMan_updateBucket(cBucketMan* self, int index)
{
    cObjNode* node = (cObjNode*)cBucketMan_first(self, index);
    while (((void*)node) != 0) {
        void* next = cBucketMan_next(self, node, index);
        node->update();
        node = (cObjNode*)next;
    }
    cBucketMan_first(self, index);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/bucketman", cBucketMan_applyBucket);
#ifdef SKIP_ASM

/**
 * Runs the apply virtual (slot 3) of every node in one bucket, in list order.
 *
 * @param index  the bucket (On Tour bucket)
 */
extern "C" void cBucketMan_applyBucket(cBucketMan* self, int index)
{
    cObjNode* node = (cObjNode*)cBucketMan_first(self, index);
    while (((void*)node) != 0) {
        node->apply();
        node = (cObjNode*)cBucketMan_next(self, node, index);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/bucketman", cBucketMan_renderBucket);
#ifdef SKIP_ASM

/**
 * Renders every node in one bucket: calls slot 4 (render) of each node from cBucketMan_first / cBucketMan_next.
 *
 * @param index  the bucket (On Tour const int bucket)
 */
extern "C" void cBucketMan_renderBucket(cBucketMan* self, int index)
{
    cObjNode* node = (cObjNode*)cBucketMan_first(self, index);
    while (((void*)node) != 0) {
        node->render();
        node = (cObjNode*)cBucketMan_next(self, node, index);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/bucketman", cBucketMan_purgeBucket);
#ifdef SKIP_ASM
struct sBucketVEntryPurge {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Deletes every node in a bucket through each node's virtual destructor (flags 3).
 *
 * @param index  the bucket index
 */
extern "C" void cBucketMan_purgeBucket(cBucketMan* self, int index)
{
    cObjNode* node = (cObjNode*)cBucketMan_first(self, index);
    while (((void*)node) != 0) {
        void* next = cBucketMan_next(self, node, index);
        if (((void*)node) != 0) {
            sBucketVEntryPurge* vt = (sBucketVEntryPurge*)VPTR_OF(cObjNode, node);
            vt[1].fn(vthis(node, vt[1]), 3);
        }
        node = (cObjNode*)next;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/bucketman", cBucketMan_purgeBucket1);
#ifdef SKIP_ASM


void* cBucketMan_first(cBucketMan* self, int index);
void* cBucketMan_next(cBucketMan* self, void* node, int index);

/**
 * Deletes every node in bucket @p index whose section id equals @p key.
 *
 * Walks the bucket with cBucketMan_first / cBucketMan_next (fetching the next node before acting), calls each node's
 * virtual slot 11 (getSectionId; cObjNode_vtbl word 23) and, on a match, calls virtual slot 1 (the destructor) with
 * flags 3 to destroy and free the node. The game loader purges buckets 1 and 8 this way when a world section is
 * unloaded.
 *
 * @param index  the bucket number
 * @param key  the section id whose nodes are deleted
 */
extern "C" void cBucketMan_purgeBucket1(cBucketMan* self, int index, int key)
{
    cObjNode* node = (cObjNode*)cBucketMan_first(self, index);
    while (((void*)node) != 0) {
        void* next = cBucketMan_next(self, node, index);
        sVEntry<int(void*)>* sectionIdSlot = &((sVEntry<int (void*)>*)VPTR_OF(cObjNode, node))[11];
        if (sectionIdSlot->fn(vthis(node, *sectionIdSlot)) == key) {
            if (((void*)node) != 0) {
                sVEntry<void(void*, int)>* vt = (sVEntry<void (void*, int)>*)VPTR_OF(cObjNode, node);
                vt[1].fn(vthis(node, vt[1]), 3);
            }
        }
        node = (cObjNode*)next;
    }
}
#endif


//100%
INCLUDE_ASM("object/bucketman", cBucketMan_setsortflag);
#ifdef SKIP_ASM
/**
 * Sets one bucket's sort flag: bit 0 of its header's flags.
 *
 * The bucket headers are the array at +0x4 (On Tour Buckets); cGame_load sets the flag on bucket 1 right after
 * cBucketMan_init(). On Tour asserts the index is in range; retail does not check it.
 *
 * @param bucket  the bucket index
 */
extern "C" void cBucketMan_setsortflag(cBucketMan* self, int bucket)
{
    tBucketHeader* buckets = self->Buckets;
    buckets[bucket].Flags |= 1;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cMoveNode_cMoveNode1);
#ifdef SKIP_ASM
extern void* cMoveNode_vtbl[];
extern "C" void* cInstanceNode_cInstanceNode1(void* self, void* a1, int type, void* a3);

/**
 * Constructs a cMoveNode for a placed instance.
 *
 * Runs the cInstanceNode constructor cInstanceNode_cInstanceNode1, installs cMoveNode_vtbl, sets Fade to 1.0, sets the
 * RootTransformDirty bit in Flags and zeroes ModifierBlock, Debounce and TransferHack. Subclass constructors call it as
 * their base with their node type: cFloatingNode (0xE), cParticleNode (0xD), the object node (17).
 *
 * @param bucket  the bucket the node is added to
 * @param type  the node type (tObjNodeType)
 * @param instRecord  the instance record the node belongs to
 * @return @p self
 */
extern "C" void* cMoveNode_cMoveNode1(cMoveNode* self, void* bucket, int type, void* instRecord)
{
    cInstanceNode_cInstanceNode1(self, bucket, type, instRecord);
    VPTR_OF(cObjNode, self) = cMoveNode_vtbl;
    self->Fade = 1.0f;
    self->Flags |= 1;
    self->ModifierBlock = 0;
    self->Debounce = 0;
    self->TransferHack = 0;
    return ((void*)self);
}
#endif


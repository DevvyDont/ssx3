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

// A bucket as new[] builds it: tBucketHeader's storage with the constructor retail inlines, which gives each cObjNode
// sentinel cObjNode_vtbl and clears its links. Same layout as tBucketHeader (its bytes, read through it); not derived
// from it, since the implicit cObjNode constructor would store the compiler's own vtable symbol instead.
struct tBucketHeaderNew {
    int Storage[sizeof(tBucketHeader) / sizeof(int)];
    tBucketHeaderNew()
    {
        tBucketHeader* bucket = (tBucketHeader*)Storage;
        VPTR_OF(cObjNode, &bucket->Head) = (void**)cObjNode_vtbl;
        bucket->Head.Next = 0;
        bucket->Head.Prev = 0;
        VPTR_OF(cObjNode, &bucket->Tail) = (void**)cObjNode_vtbl;
        bucket->Tail.Next = 0;
        bucket->Tail.Prev = 0;
        VPTR_OF(cObjNode, &bucket->AddHead) = (void**)cObjNode_vtbl;
        bucket->AddHead.Next = 0;
        bucket->AddHead.Prev = 0;
        VPTR_OF(cObjNode, &bucket->AddTail) = (void**)cObjNode_vtbl;
        bucket->AddTail.Next = 0;
        bucket->AddTail.Prev = 0;
    }
    ~tBucketHeaderNew() {}
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
void cBucketMan::init(int n)
{
    tBucketHeader** slot = &Buckets;
    *slot = (tBucketHeader*)new (str_Buckets, 0, 0) tBucketHeaderNew[n];
    NumBuckets = n;
    for (int i = 0; i < NumBuckets; i++) {
        Buckets[i].Head.Next = &Buckets[i].Tail;
        Buckets[i].Head.Prev = 0;
        Buckets[i].Tail.Next = 0;
        Buckets[i].Tail.Prev = &Buckets[i].Head;
        Buckets[i].AddHead.Next = &Buckets[i].AddTail;
        Buckets[i].AddHead.Prev = 0;
        Buckets[i].AddTail.Next = 0;
        Buckets[i].AddTail.Prev = &Buckets[i].AddHead;
        Buckets[i].Flags = 0;
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
void cBucketMan::purge()
{
    int i;
    for (i = 0; i < NumBuckets; i++) {
        purgeBucket(i);
    }
    // ORIGINAL-PTR: delete[] Buckets as g++ 2.95 inlines it: the element count sits in the 16-byte new[] cookie
    // before the array (a heap block header), and the block itself starts at the cookie
    tBucketHeader* buckets = Buckets;
    if (buckets != 0) {
        tBucketHeader* bucket = buckets + ((int*)buckets)[-4];
        while (Buckets != bucket) {
            bucket--;
            cObjNode__cObjNode(&bucket->AddTail, 2);
            cObjNode__cObjNode(&bucket->AddHead, 2);
            cObjNode__cObjNode(&bucket->Tail, 2);
            cObjNode__cObjNode(&bucket->Head, 2);
        }
        cMemMan_free((char*)Buckets - 0x10);
    }
    Buckets = 0;
    NumBuckets = 0;
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
void cBucketMan::mergeBucket(int index)
{
    cObjNode* n = (cObjNode*)addfirst(index);
    while (n) {
        cObjNode* nextN = (cObjNode*)addnext((cObjNode*)n, index);
        n->Prev->Next = n->Next;
        n->Next->Prev = n->Prev;
        cObjNode* other;
        for (other = (cObjNode*)last(index); other;
             other = (cObjNode*)prev((cObjNode*)other, index)) {
            if (n->compare(other)) {
                if (n->equal(other)) {
                    n->Next = other->Next;
                    other->Next->Prev = n;
                    n->Prev = other->Prev;
                    other->Prev->Next = n;
                    other->Next = 0;
                    other->Prev = 0;
                    delete other;
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
            n->Next = Buckets[index].Head.Next;
            Buckets[index].Head.Next->Prev = n;
            n->Prev = &Buckets[index].Head;
            Buckets[index].Head.Next = n;
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
void* cBucketMan::first(int index)
{
    cObjNode* head = Buckets[index].Head.Next;
    return (head == &Buckets[index].Tail) ? 0 : head;
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
void* cBucketMan::last(int index)
{
    cObjNode* tail = Buckets[index].Tail.Prev;
    return (tail == &Buckets[index].Head) ? 0 : tail;
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
 * @param vnode  the current node
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
void* cBucketMan::prev(cObjNode* node, int index)
{
    void* next = node->Prev;
    return (next == (void*)&Buckets[index].Head) ? 0 : next;
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
void* cBucketMan::addfirst(int index)
{
    cObjNode* head = Buckets[index].AddHead.Next;
    return (head == &Buckets[index].AddTail) ? 0 : head;
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
void* cBucketMan::addnext(cObjNode* node, int index)
{
    void* next = node->Next;
    return (next == (void*)&Buckets[index].AddTail) ? 0 : next;
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
void cBucketMan::updateBucket(int index)
{
    cObjNode* node = (cObjNode*)first(index);
    while (((void*)node) != 0) {
        void* next = cBucketMan_next(this, node, index);
        node->update();
        node = (cObjNode*)next;
    }
    first(index);
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
void cBucketMan::applyBucket(int index)
{
    cObjNode* node = (cObjNode*)first(index);
    while (((void*)node) != 0) {
        node->apply();
        node = (cObjNode*)cBucketMan_next(this, node, index);
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
void cBucketMan::renderBucket(int index)
{
    cObjNode* node = (cObjNode*)first(index);
    while (((void*)node) != 0) {
        node->render();
        node = (cObjNode*)cBucketMan_next(this, node, index);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/bucketman", cBucketMan_purgeBucket);
#ifdef SKIP_ASM
/**
 * Deletes every node in a bucket through each node's virtual destructor (flags 3).
 *
 * @param index  the bucket index
 */
void cBucketMan::purgeBucket(int index)
{
    cObjNode* node = (cObjNode*)first(index);
    while (((void*)node) != 0) {
        void* next = cBucketMan_next(this, node, index);
        delete node;
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
void cBucketMan::purgeBucket1(int index, int key)
{
    cObjNode* node = (cObjNode*)first(index);
    while (((void*)node) != 0) {
        void* next = cBucketMan_next(this, node, index);
        if (node->getSectionId() == key) {
            delete node;
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
void cBucketMan::setsortflag(int bucket)
{
    tBucketHeader* buckets = Buckets;
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
void* cMoveNode::cMoveNode1(void* bucket, int type, void* instRecord)
{
    cInstanceNode_cInstanceNode1(this, bucket, type, instRecord);
    VPTR_OF(cObjNode, this) = cMoveNode_vtbl;
    Fade = 1.0f;
    Flags |= 1;
    ModifierBlock = 0;
    Debounce = 0;
    TransferHack = 0;
    return ((void*)this);
}
#endif


#include "vtable.h"
#include "common.h"
#include "object/bucketman.h"
#include "object/objnode.h"
#include "replay/bxreplayframe.h"

extern "C" void cBucketMan_add(void* mgr, void* node, void* param);
extern void* cObjNode_vtbl[16];
extern char gBucketMan;


//99.94%
INCLUDE_ASM("object/objnode", cObjNode_cObjNode__FP8cObjNodePv);
#ifdef SKIP_ASM
/**
 * Constructs a scene object node and adds it to a bucket of the global bucket manager (gBucketMan).
 *
 * @param bucket  the bucket to add the node to (On Tour: const int bucket)
 * @return @p self
 */
cObjNode* cObjNode_cObjNode(cObjNode* self, void* bucket)
{
    (*(void**)&VPTR_OF(cObjNode, self)) = cObjNode_vtbl;
    cBucketMan_add(&gBucketMan, self, bucket);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/objnode", cObjNode__cObjNode);
#ifdef SKIP_ASM
extern "C" void cBucketMan_remove(void* mgr, void* node);
void operator_delete(int*);

/**
 * Destroys a scene object node: removes it from the global bucket manager, then frees it when asked to.
 *
 * Restores cObjNode's vtable at +0xC first.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cObjNode__cObjNode(cObjNode* self, int flags)
{
    VPTR_OF(cObjNode, self) = cObjNode_vtbl;
    cBucketMan_remove(&gBucketMan, self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/objnode", cTypeObjNode_cTypeObjNode1);
#ifdef SKIP_ASM
extern void* cTypeObjNode_vtbl[];
extern int cTypeObjNode_ObjNodeCount[];
extern "C" void* cObjNode_cObjNode1(void* self, void* a1);

/**
 * Constructs the typed object-node base of a node.
 *
 * Runs cObjNode_cObjNode1 with the bucket, installs cTypeObjNode_vtbl, stores type at +0x10, clears the flags (+0x12)
 * and increments the live count for that type (cTypeObjNode_ObjNodeCount[type]).
 *
 * @param bucket  update bucket
 * @param type  node type (tObjNodeType)
 * @return self
 */
void* cTypeObjNode::cTypeObjNode1(void* bucket, int type)
{
    cObjNode_cObjNode1(this, bucket);
    VPTR_OF(cObjNode, this) = cTypeObjNode_vtbl;
    ObjNodeType = type;
    *(short*)&Flags = 0;
    cTypeObjNode_ObjNodeCount[ObjNodeType]++;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/objnode", cTypeObjNode_cTypeObjNode);
#ifdef SKIP_ASM

extern void* cTypeObjNode_vtbl[];
extern int cTypeObjNode_ObjNodeCount[];
// PORT: cObjNode_cObjNode takes (self, param2); this caller also leaves the stream in $6.
cObjNode* cObjNode_cObjNode_3(cObjNode* self, void* param2, void* stream) __asm__("cObjNode_cObjNode__FP8cObjNodePv");

/**
 * Recreates a typed object node from a replay frame and counts it in the per-type live counts (cTypeObjNode_ObjNodeCount).
 *
 * @param bucket  the bucket the node belongs to (On Tour const int bucket)
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cTypeObjNode_cTypeObjNode(cTypeObjNode* self, void* bucket, cBxReplayFrame* stream)
{
    cObjNode_cObjNode_3((cObjNode*)self, bucket, stream);
    VPTR_OF(cObjNode, self) = cTypeObjNode_vtbl;
    stream->read((char*)&self->ObjNodeType, 4);
    cTypeObjNode_ObjNodeCount[self->ObjNodeType]++;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/objnode", cTypeObjNode__cTypeObjNode);
#ifdef SKIP_ASM
extern "C" void cObjNode__cObjNode(cObjNode* self, int flags);
extern void* cTypeObjNode_vtbl[];
extern int cTypeObjNode_ObjNodeCount[];

// PORT: the unit declares cTypeObjNode__cTypeObjNode(void*), but this is a deleting
// destructor taking (self, flags); the real body is bound by asm label.
void cTypeObjNode__cTypeObjNode_impl(void* self, int flags) __asm__("cTypeObjNode__cTypeObjNode");

/**
 * Destroys a typed object node: removes it from the per-type live counts, then destroys the cObjNode base.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
void cTypeObjNode__cTypeObjNode_impl(void* vself, int flags)
{
    cTypeObjNode* self = (cTypeObjNode*)vself;
    VPTR_OF(cObjNode, self) = cTypeObjNode_vtbl;
    cTypeObjNode_ObjNodeCount[self->ObjNodeType]--;
    cObjNode__cObjNode((cObjNode*)self, flags);
}
#endif

//100%
INCLUDE_ASM("object/objnode", cSortObjNode_cSortObjNode1);
#ifdef SKIP_ASM
extern int cSortObjNode_ObjNodeSortIndex;
extern int cSortObjNode_ObjNodeNoSortIndex;
extern void* cSortObjNode_vtbl[];

static inline int objNextId_4850(int* c)
{
    return (*c)++;
}

/**
 * Constructs the sorted object-node base of a node.
 *
 * Runs cTypeObjNode_cTypeObjNode1, installs cSortObjNode_vtbl and takes SortIndex (+0x14) from
 * cSortObjNode_ObjNodeNoSortIndex for ATYPE_DEAD or cSortObjNode_ObjNodeSortIndex otherwise, post-incrementing the
 * counter.
 *
 * @param bucket  update bucket
 * @param type  node type
 * @return self
 */
void* cSortObjNode::cSortObjNode1(void* bucket, int type)
{
    int id;
    this->cTypeObjNode1(bucket, type);
    VPTR_OF(cObjNode, this) = cSortObjNode_vtbl;
    if (type == ATYPE_DEAD) {
        id = objNextId_4850(&cSortObjNode_ObjNodeNoSortIndex);
    } else {
        id = objNextId_4850(&cSortObjNode_ObjNodeSortIndex);
    }
    SortIndex = (unsigned int)id;
    return ((void*)this);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/objnode", cSortObjNode_cSortObjNode);
#ifdef SKIP_ASM
struct sVEntryCSortObjNode {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern void* cSortObjNode_vtbl[];
extern "C" void* cTypeObjNode_cTypeObjNode(cTypeObjNode* self, void* a, cBxReplayFrame* stream);

/**
 * Recreates a sorted object node from a replay frame: the typed-node part and its sort index.
 *
 * @param bucket  the bucket the node belongs to (On Tour const int bucket)
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cSortObjNode_cSortObjNode(cSortObjNode* self, void* bucket, cBxReplayFrame* stream)
{
    cTypeObjNode_cTypeObjNode((cTypeObjNode*)self, bucket, stream);
    VPTR_OF(cObjNode, self) = cSortObjNode_vtbl;
    stream->read((char*)&self->SortIndex, 4);
    return ((void*)self);
}
#endif

extern void* cSortObjNode_vtbl[];
extern "C" void* cTypeObjNode__cTypeObjNode(void*);

//99.5%
INCLUDE_ASM("object/objnode", cSortObjNode__cSortObjNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a sort object node: stores cSortObjNode's vtable at +0xC, then runs the cTypeObjNode destructor.
 *
 * The g++ delete flags in the second argument register pass through untouched to cTypeObjNode__cTypeObjNode().
 *
 * @return no meaningful value (the base destructor returns nothing)
 */
void* cSortObjNode__cSortObjNode(void* vself)
{
    cSortObjNode* self = (cSortObjNode*)vself;
    VPTR_OF(cObjNode, self) = (void**)((int)(void*)cSortObjNode_vtbl);
    return cTypeObjNode__cTypeObjNode(self);
}
#endif

//100%
INCLUDE_ASM("object/objnode", cTypeObjNode_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes a typed object node's type and flags to a replay frame.
 *
 * @param obj  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cTypeObjNode_writeToReplayFrame(cTypeObjNode* self, cBxReplayFrame* obj)
{
    obj->write((const char*)&self->ObjNodeType, 4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/objnode", cSortObjNode_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes a sorted object node to a replay frame: its type and flags, then its sort index.
 *
 * @param obj  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cSortObjNode_writeToReplayFrame(cSortObjNode* self, cBxReplayFrame* obj)
{
    cTypeObjNode_writeToReplayFrame((cTypeObjNode*)self, obj);
    obj->write((const char*)&self->SortIndex, 4);
}
#endif

//100%
INCLUDE_ASM("object/objnode", cBucketMan_cBucketMan__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the bucket manager by zeroing its first two words.
 *
 * @return self
 */
void* cBucketMan_cBucketMan(void* vself)
{
    cBucketMan* self = (cBucketMan*)vself;
    int zero = 0;
    self->Buckets = (tBucketHeader*)zero;
    *(int*)self = zero;
    return ((void*)self);
}
#endif


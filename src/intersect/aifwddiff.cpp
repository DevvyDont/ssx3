#include "common.h"
#include "object/rollermodifier.h"
#include "bx/math3d.h"
#include "intersect/aifwddiff.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_Init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void tAIFwdDiff_tAIFwdDiff(void* self);
extern char str_AIFwdDiffCache[];
extern char str_AIFwdDiffHash[];

struct sCacheEntry_7890 {
    unsigned int key;
    sCacheEntry_7890* next;
    sCacheEntry_7890* prev;
    char pad[0x1CD0 - 0xC];
};

/**
 * Sets up the AI forward-difference cache: a ring of empty entries ("AIFwdDiffCache") and a hash table of
 * 2n+13 buckets.
 *
 * @param n  number of cache entries (On Tour num)
 */
extern "C" void cAIFwdDiffCache_Init(cAIFwdDiffCache* self, int n)
{
    sCacheEntry_7890* mem = (sCacheEntry_7890*)operator_new_tag(n * sizeof(sCacheEntry_7890), str_AIFwdDiffCache, 0, 0);
    sCacheEntry_7890* entry = mem;
    for (int k = n - 1; k != -1; k--, entry++) {
        tAIFwdDiff_tAIFwdDiff(entry);
    }
    *(sCacheEntry_7890**)&self->mCache = mem;
    for (int i = 0; i < n; i++) {
        ((sCacheEntry_7890*)self->mCache)[i].next = &((sCacheEntry_7890*)self->mCache)[i] + 1;
        ((sCacheEntry_7890*)self->mCache)[i].prev = &((sCacheEntry_7890*)self->mCache)[i] - 1;
        ((sCacheEntry_7890*)self->mCache)[i].key = 0xFFFFFFFF;
    }
    ((sCacheEntry_7890*)self->mCache)[0].prev = ((sCacheEntry_7890*)self->mCache) + n - 1;
    ((sCacheEntry_7890*)self->mCache)[n - 1].next = ((sCacheEntry_7890*)self->mCache);
    self->mHashSize = n * 2 + 13;
    *(sCacheEntry_7890**)&self->mAgeQueue = ((sCacheEntry_7890*)self->mCache);
    *(sCacheEntry_7890***)&self->mPatchHash = (sCacheEntry_7890**)operator_new_tag(((int)self->mHashSize) * 4, str_AIFwdDiffHash, 0x80000000, 0);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_GetFwdDiff);
#ifdef SKIP_ASM
struct sFwdDiffList;
struct sFwdDiffLink;
struct sFwdDiffTable;
struct sFwdDiffNode;
extern "C" void cAIFwdDiffCache_MoveToAgeQueueTail(sFwdDiffList* list, sFwdDiffLink* n);
extern "C" void cAIFwdDiffCache_RemoveFromHash(void* self, void* node);
extern "C" void cAIFwdDiffCache_AddToHash(sFwdDiffTable* table, sFwdDiffNode* node);
extern "C" void cAIFwdDiffCache_TesselatePatch(cAIFwdDiffCache* self, void* src, void* node);

struct sCacheNode_279D0 {
    unsigned int key;           // 0x0
    sCacheNode_279D0* lruNext;  // 0x4
    sCacheNode_279D0* lruPrev;  // 0x8
    sCacheNode_279D0* next;     // 0xC
};


static inline int cAIFwdDiffCache_GetFwdDiff_less(unsigned int a, unsigned int b) { return a < b; }

/**
 * Returns the forward-difference tessellation of a terrain patch from the AI cache, building it when it is not cached.
 *
 * Looks the patch's key (+0x150) up in its hash bucket (key modulo the bucket count +0xC; chains are sorted by key). On
 * a hit the node is moved to the tail of the age queue (cAIFwdDiffCache_MoveToAgeQueueTail). On a miss the oldest node (+0x4) is reused: it
 * is taken out of the hash (cAIFwdDiffCache_RemoveFromHash), given the key, hashed again (cAIFwdDiffCache_AddToHash), the age queue head moves on,
 * and the patch is tessellated into it (cAIFwdDiffCache_TesselatePatch). Called by cOverlapList_terrainIntersect and TerrainCallback_intersectpatch.
 *
 * @param src  the terrain patch record (On Tour const tPatchRecord&)
 * @param out  receives the cache node (On Tour ppFwdDiff)
 */
extern "C" void cAIFwdDiffCache_GetFwdDiff(cAIFwdDiffCache* self, void* src, sCacheNode_279D0** out)
{
    unsigned int key = *(unsigned int*)((char*)src + 0x150);
    sCacheNode_279D0* n = ((sCacheNode_279D0**)self->mPatchHash)[key % self->mHashSize];
    while (n != 0 && !cAIFwdDiffCache_GetFwdDiff_less(key, n->key)) {
        if (n->key == key) {
            cAIFwdDiffCache_MoveToAgeQueueTail((sFwdDiffList*)self, (sFwdDiffLink*)n);
            *out = n;
            return;
        }
        n = n->next;
    }
    n = ((sCacheNode_279D0*)self->mAgeQueue);
    cAIFwdDiffCache_RemoveFromHash(self, n);
    n->key = key;
    cAIFwdDiffCache_AddToHash((sFwdDiffTable*)self, (sFwdDiffNode*)n);
    *(sCacheNode_279D0**)&self->mAgeQueue = n->lruNext;
    cAIFwdDiffCache_TesselatePatch((cAIFwdDiffCache*)self, src, n);
    *out = n;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_MoveToAgeQueueTail);
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

/**
 * Moves an AI forward-difference cache node to the tail of the age queue, making it the most recently used.
 *
 * The queue is circular with its head (oldest) at +0x4: if the node is the head the head just moves on, if it is
 * already the tail (head->prev) nothing changes, otherwise it is unlinked and reinserted before the head.
 *
 * @param list  the cache (this)
 * @param n  the node (On Tour node)
 */
extern "C" void cAIFwdDiffCache_MoveToAgeQueueTail(sFwdDiffList* list, sFwdDiffLink* n)
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

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_RemoveFromHash);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sCacheNode_7B30 {
    unsigned int key;
    sCacheNode_7B30* lruNext;
    sCacheNode_7B30* lruPrev;
    sCacheNode_7B30* next;
};


static inline int less_7B30(unsigned int a, unsigned int b) { return a < b; }

extern "C" void cAIFwdDiffCache_RemoveFromHash_7B30(cAIFwdDiffCache* self, sCacheNode_7B30* node) __asm__("cAIFwdDiffCache_RemoveFromHash");
/**
 * Removes an AI forward-difference cache node from the patch hash table.
 *
 * Does nothing for an unused node (key 0xFFFFFFFF). Otherwise walks the node's bucket (key modulo +0xC; the chain is
 * sorted by key, so the walk stops at a larger key) and unlinks the entry with the node's key.
 *
 * @param node  the node (On Tour node)
 */
extern "C" void cAIFwdDiffCache_RemoveFromHash_7B30(cAIFwdDiffCache* self, sCacheNode_7B30* node)
{
    unsigned int key = node->key;
    if (key == 0xFFFFFFFF)
        return;
    sCacheNode_7B30** link = &((sCacheNode_7B30**)self->mPatchHash)[key % self->mHashSize];
    if (*link == 0)
        return;
    sCacheNode_7B30* n = *link;
    while (n != 0 && !less_7B30(key, n->key)) {
        if (n->key == key) {
            *link = n->next;
            return;
        }
        link = &(*link)->next;
        n = *link;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_AddToHash);
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

/**
 * Adds an AI forward-difference cache node to the patch hash table, keeping its bucket sorted by key.
 *
 * The bucket is the node's key modulo the bucket count (+0xC); the node goes before the first entry with a larger key.
 *
 * @param table  the cache (this)
 * @param node  the node (On Tour anode)
 */
extern "C" void cAIFwdDiffCache_AddToHash(sFwdDiffTable* table, sFwdDiffNode* node)
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

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_TesselatePatch);
#ifdef SKIP_ASM
extern "C" void func_00391418(void* t, void* a);
extern "C" void cAIFwdDiffCache_InitializeCache(cAIFwdDiffCache* self, void* a);
extern "C" void func_003914F8(void* t);
extern "C" void func_00391480(void* t, void* a);
struct sGp2930 { int a, b; };
extern sGp2930 D_004A5A20; // placeholder: target is a raw $gp+0x2930 reference

/**
 * Tessellates a terrain patch into an AI forward-difference cache node on VU0.
 *
 * Loads the patch's 16 control points (+0x40) into VU0 and starts its micro-program (func_00391418), resets the node's
 * cached face data (cAIFwdDiffCache_InitializeCache) while VU0 runs, waits for it (func_003914F8) and copies the 10 x 10 evaluated
 * vertices from VU0 memory into the node (+0x10, func_00391480). The object at $gp+0x2930 passed to the VU0 helpers is
 * not used by them. Called by cAIFwdDiffCache_GetFwdDiff on a cache miss.
 *
 * @param patch  the terrain patch record (On Tour const tPatchRecord&)
 * @param fwdDiff  the cache node to fill (On Tour node)
 */
extern "C" void cAIFwdDiffCache_TesselatePatch(cAIFwdDiffCache* self, void* patch, void* fwdDiff)
{
    sGp2930* patchEval = &D_004A5A20;
    func_00391418(patchEval, (char*)patch + 0x40);
    cAIFwdDiffCache_InitializeCache(self, fwdDiff);
    func_003914F8(patchEval);
    func_00391480(patchEval, (char*)fwdDiff + 0x10);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_InitializeCache);
#ifdef SKIP_ASM
/**
 * Clears the face data of an AI forward-difference cache node: marks every entry as not yet computed.
 *
 * Sets the first float of each of the three leading vectors of the 81 entries at +0x650 and the 9 entries at +0x1A90
 * (0x40 bytes each) to 1e10. Called by cAIFwdDiffCache_TesselatePatch before the patch is evaluated.
 *
 * @param patch  the cache node (On Tour node; despite the name not a patch)
 */
extern "C" void cAIFwdDiffCache_InitializeCache(cAIFwdDiffCache* self, void* patch)
{
    float* cell = (float*)((char*)patch + 0x650);
    int n = 80;
    do {
        cell[0] = 10000000000.0f;
        cell[4] = 10000000000.0f;
        cell[8] = 10000000000.0f;
        cell += 16;
    } while (n-- > 0);
    cell = (float*)((char*)patch + 0x1A90);
    n = 8;
    do {
        cell[0] = 10000000000.0f;
        cell[4] = 10000000000.0f;
        cell[8] = 10000000000.0f;
        cell += 16;
    } while (n-- > 0);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", bxSphereTree_operatorE);
#ifdef SKIP_ASM
struct sQuad_27CC8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBlock_27CC8 {
    int d[32];
};

struct sFwdDiff_27CC8 {
    sQuad_27CC8 m[8];       // 0x00
    sQuad_27CC8 q;          // 0x80
    char pad_0x90[0x8];
    sBlock_27CC8* blkp;     // 0x98
    char pad_0x9C[0x24];
    sBlock_27CC8 blk;       // 0xC0
};

/**
 * Copies a sphere tree's placement and static info into this one.
 *
 * Copies the eight axis vectors (+0x0..+0x7F), the centre (+0x80) and the 0x80-byte static info (+0xC0), then points
 * the info pointer (+0x98) at this tree's own copy. The scale (+0x90), worst case (+0x94) and sort axes (+0x9C) are not
 * copied. Called by cRollerModifier_cRollerModifier, cWorldSphTree_Transform and cRollerModifier_cRollerModifier1.
 *
 * @param src  the tree to copy (On Tour aTree)
 * @return @p self
 */
extern "C" sFwdDiff_27CC8* bxSphereTree_operatorE(sFwdDiff_27CC8* self, sFwdDiff_27CC8* src)
{
    int i;
    for (i = 0; i < 8; i++) {
        self->m[i] = src->m[i];
    }
    self->q = src->q;
    self->blk = src->blk;
    self->blkp = &self->blk;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", bxSphereTree_Load);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int n);

/**
 * Sets up a sphere tree from its packed data in a loaded collision resource.
 *
 * Returns 0 for a null @p buf. Otherwise stores the tree id, points the tree's info pointer at its own info block,
 * copies the header fields (lengths, level count, centre, extents) out of the buffer with MEM_copy, points the
 * level table (+0x20) and the packed bit tree (+0x24, or +0x28 when not compressed) into the buffer and caches
 * the centre with w = 1.
 *
 * @param buf  the packed tree data (On Tour staticBuf)
 * @param aTreeID   the tree's id (On Tour aTreeID)
 * @return the number of bytes of @p buf the tree uses
 */
extern "C" int bxSphereTree_Load(sFwdDiff_27CC8* self, char* buf, int aTreeID)
{
    if (buf == 0) {
        return 0;
    }
    self->blk.d[0] = aTreeID;
    self->blk.d[1] = 0;
    self->blkp = &self->blk;
    self->blk.d[4] = 1;
    int len0;
    int len1;
    MEM_copy(&len0, buf, 4);
    MEM_copy(&len1, buf + 4, 4);
    MEM_copy((char*)self->blkp + 0x8, buf + 0x8, 4);
    MEM_copy((char*)self->blkp + 0xC, buf + 0xC, 4);
    MEM_copy((char*)self->blkp + 0x14, buf + 0x10, 0xC);
    MEM_copy((char*)self->blkp + 0x2C, buf + 0x1C, 0xC);
    MEM_copy((char*)self->blkp + 0x38, buf + 0x28, 36);
    MEM_copy((char*)self->blkp + 0x5C, buf + 0x4C, 36);
    char* cursor = buf + 0x70;
    *(char**)((char*)self->blkp + 0x20) = cursor;
    cursor += (*(int*)((char*)self->blkp + 0xC) + 1) * 0xC;
    *(char**)((char*)self->blkp + 0x24) = cursor;
    cursor += len1;
    cursor += len0;
    char* treeInfo = (char*)self->blkp;
    if (*(int*)(treeInfo + 0x8) != 0) {
        *(char**)(treeInfo + 0x28) = 0;
    } else {
        *(char**)(treeInfo + 0x28) = *(char**)(treeInfo + 0x24);
    }
    sQuad_27CC8 centre;
    char* infoBlock = (char*)self->blkp;
    centre.x = *(float*)(infoBlock + 0x14);
    centre.y = *(float*)(infoBlock + 0x18);
    centre.z = *(float*)(infoBlock + 0x1C);
    centre.w = 1.0f;
    self->q = centre;
    return cursor - buf;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", bxSphereTree_IsRiderTreeCollision);
#ifdef SKIP_ASM
extern "C" int SphereTree_AllocBuf(void* self);
extern "C" int SphereTree_isSphereCollision(void* self, void* pos, void* out, int a3, int a4, int a5, int a6, float radius);

struct sSphere_27F18 {
    float pos[4];
    float radius;
    int pad[3];
};
struct sShape_27F18 {
    char pad[0x10];
    float pos[4];           // 0x10
    float radius;           // 0x20
    int pad24[2];
    int count;              // 0x2C
    sSphere_27F18 subs[1];  // 0x30
};

/**
 * Tests a sphere tree against a rider's spheres.
 *
 * Makes sure the tree's data buffer is loaded (SphereTree_AllocBuf when the info word +0x8 is set; the buffer goes to
 * info +0x28), then tests the rider's bounding sphere (+0x10, radius +0x20) with SphereTree_isSphereCollision from the root (centre
 * +0x80). A miss returns 0. A hit returns 1 when the rider has no sphere list (+0x2C = 0); otherwise each listed sphere
 * (from +0x30, 0x20 bytes each) is tested and the first hit returns 1. The push-out and contact outputs are filled by
 * SphereTree_isSphereCollision. Called by cWorldRider_IsSphTreeInt.
 *
 * @param shape  the rider's sphere tree (On Tour const cRiderSphereTree*)
 * @param outPush  out: how far to push apart (On Tour overlap)
 * @param outContact  out: the contact point (On Tour loc)
 * @return 1 on contact, else 0
 */
extern "C" int bxSphereTree_IsRiderTreeCollision(void* self, sShape_27F18* shape, int outPush, int outContact)
{
    void* tree = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)tree + 8) != 0) {
        *(int*)(*(char**)((char*)self + 0x98) + 0x28) = SphereTree_AllocBuf(tree);
    }
    if (SphereTree_isSphereCollision(self, shape->pos, (char*)self + 0x80, 0, 0, outPush, outContact, shape->radius) == 0) {
        return 0;
    }
    if (shape->count == 0) {
        return 1;
    }
    for (int i = 0; i < shape->count; i++) {
        if (SphereTree_isSphereCollision(self, shape->subs[i].pos, (char*)self + 0x80, 0, 0, outPush, outContact, shape->subs[i].radius) != 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", bxSphereTree_Is2SidedTriCollision);
#ifdef SKIP_ASM
extern "C" float SphereTree_GetRadius(void* self);
extern "C" int SphereTree_IsTriCollision(void* self, void* a1, void* a2, void* a3, void* v, void* a5, void* a6);


// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_28030(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_28030(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_28030(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

/**
 * Tests the sphere tree against a triangle from either side.
 *
 * Returns 0 when the tree's centre (+0x80) is farther from the triangle's plane than its radius
 * (SphereTree_GetRadius). Otherwise flips the normal to face the tree when needed and runs SphereTree_IsTriCollision (the one-sided
 * test), which fills the push-out vector and contact point.
 *
 * @param point1    first triangle corner
 * @param point2   second triangle corner
 * @param point3   third triangle corner
 * @param dir  the triangle's unit normal
 * @param overlap   out: how far to push the tree out (On Tour overlap)
 * @param loc   out: the contact point (On Tour loc)
 * @return 1 on contact
 */
extern "C" int bxSphereTree_Is2SidedTriCollision(bxSphereTree* self, t4Vector* point1, t4Vector* point2, t4Vector* point3, t4Vector* dir, t4Vector* overlap, t4Vector* loc)
{
    float dot;
    {
        t4Vector toCentre = vu0Sub_28030(*(&self->transCenter), *point1);
        dot = vu0Dot_28030(*dir, toCentre);
    }
    if (SphereTree_GetRadius(self) < __builtin_fabsf(dot)) {
        return 0;
    }
    if (dot < 0.0f) {
        t4Vector nd = vu0Scale_28030(*dir, -1.0f);
        return SphereTree_IsTriCollision(self, point1, point2, point3, &nd, overlap, loc);
    }
    return SphereTree_IsTriCollision(self, point1, point2, point3, dir, overlap, loc);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", bxSphereTree_Draw);
#ifdef SKIP_ASM
extern "C" void bxSphereTree_drawLeaves(void *, int, void *, int, int);

/**
 * Debug-draws a sphere tree's occupied leaf cells.
 *
 * Makes sure the tree's data buffer is loaded (as bxSphereTree_IsRiderTreeCollision does) and calls bxSphereTree_drawLeaves from the root: centre
 * +0x80, level 0, index 0, down to the depth stored at info +0xC. Nothing calls it.
 *
 * @param octree  the sphere tree (this)
 */
extern "C" void bxSphereTree_Draw(void *octree) {
    void *temp_4;

    temp_4 = (*(void **)((char*)(octree) + (0x98)));
    if ((*(int *)((char*)(temp_4) + (8))) != 0) {
        (*(int *)((char*)((*(void **)((char*)(octree) + (0x98)))) + (0x28))) = SphereTree_AllocBuf(temp_4);
    }
    bxSphereTree_drawLeaves(octree, (*(int *)((char*)((*(void **)((char*)(octree) + (0x98)))) + (0xC))), (char*)octree + 0x80, 0, 0);
}
#endif
//100%
INCLUDE_ASM("intersect/aifwddiff", bxSphereTree_drawLeaves);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVec4_281D0 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sLevel_281D0 {
    float scale;   // 0x0
    float child;   // 0x4
    int stride;    // 0x8
};
struct sGrid_281D0 {
    char pad0[0x8];
    int pad8;
    char padC[0x14];
    sLevel_281D0* levels; // 0x20
    char pad24[4];
    unsigned char* bits;  // 0x28
};
struct sAI_281D0 {
    sVec4_281D0 dirs[8];   // 0x0
    char pad80[0x10];
    float unit;            // 0x90
    char pad94[4];
    sGrid_281D0* grid;     // 0x98
};
struct sVE87_281D0 { short delta; short index; void (*fn)(void*, float, void*, void*, int); };

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline void vu0Scale_281D0(sVec4_281D0& r, const sVec4_281D0& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline void vu0Add_281D0(sVec4_281D0& r, const sVec4_281D0& a, const sVec4_281D0& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}


extern "C" void func_281D0(sAI_281D0* self, int end, sVec4_281D0* pos, int level, int off) __asm__("bxSphereTree_drawLeaves");
/**
 * Recursively draws the occupied leaf cells of a sphere tree as magenta boxes.
 *
 * Above the target level each set bit i of the occupancy byte at @p off (buffer at info +0x28) selects child i: its
 * centre is @p pos plus axis vector i (+0x0 + 16i) scaled by the level's offset (level table at info +0x20, 12 bytes
 * per level, float +0x10 from the entry), and its index is off + (i + 1) x the level's stride (+0x8). At the target
 * level it draws a box of size level scale x the tree's scale (+0x90) at @p pos in colour (1, 0, 1, 0) through virtual
 * slot 87 of cGraphicsMan_TheGraphicsMan. Called by bxSphereTree_Draw and itself.
 *
 * @param end  the level whose cells are drawn (On Tour desLevel)
 * @param pos  this cell's centre (On Tour center)
 * @param level  this cell's level
 * @param off  this cell's index into the occupancy bytes (On Tour index)
 */
extern "C" void func_281D0(sAI_281D0* self, int end, sVec4_281D0* pos, int level, int off)
{
    if (level < end) {
        float levelOffset = *(float*)((char*)self->grid->levels + level * 12 + 0x10);
        for (int i = 0; i < 8; i++) {
            if ((self->grid->bits[off] >> i) & 1) {
                sVec4_281D0 sum;
                sVec4_281D0 offsetVec;
                sVec4_281D0 tmpVec;
                vu0Scale_281D0(tmpVec, self->dirs[i], levelOffset);
                offsetVec = tmpVec;
                vu0Add_281D0(tmpVec, *pos, offsetVec);
                sum = tmpVec;
                int st = self->grid->levels[level].stride;
                func_281D0(self, end, &sum, level + 1, (i + 1) * st + off);
            }
        }
    } else {
        cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
        sVE87_281D0* vt = &((sVE87_281D0*)w->mVtbl)[87];
        char* thisp = (char*)w + vt->delta;
        sVec4_281D0 color;
        float boxSize = self->grid->levels[level].scale * self->unit;
        color.x = 1.0f;
        color.y = 0.0f;
        color.z = 1.0f;
        color.w = 0.0f;
        vt->fn(thisp, boxSize, pos, &color, 4);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_CellKey_proximitytest);
#ifdef SKIP_ASM
struct sCell_00328360 {
    int level;
    int x, y, z;
};
struct sBox_00328360 {
    float minx, miny, minz, pad;
    float maxx, maxy, maxz;
};

/**
 * Classifies an octree cell against a bounding box: outside (1), crossing (2) or inside (0).
 *
 * The cell spans [x - 0.2, x + 1.2] x 2^level on each axis (level, x, y, z from @p cell). It returns 1 when the box
 * misses that span on any axis. The remaining tests are likely buggy: it returns 2 when the box's maximum exceeds the
 * cell's upper bound on an axis, or when the box's minimum is below the cell's UPPER bound (x, then y twice; z is never
 * tested), which holds for practically every box that is not outside. 0 therefore only comes back when the box's
 * minimum sits exactly on the upper corner. Callers walk a 0 cell whole (for camera triggers descend) and a 2
 * cell child by child (nash_privy_QueryHelper_helpX), so in practice every overlapping cell takes the crossing path.
 *
 * Called once per root cell by cCameraTriggerMan_checkCollisions (camera triggers) and the octree queries
 * cOverlapList_getinstances, cOverlapList_gettyped, cOverlapList_getpatchesandinstances, cOverlapList_geteverything, cBXNash_terrainIntersect, cBXNash_staticObjectIntersect and cBXNash_splineIntersect.
 *
 * @param cell  the cell key: level, x, y, z (On Tour CellKey, this)
 * @param box  the query box: min at +0x0, max at +0x10
 * @return 1 outside, 2 crossing, 0 inside (On Tour ProximityResult)
 */
extern "C" int nash_CellKey_proximitytest(sCell_00328360* cell, sBox_00328360* box)
{
    union {
        int i;
        float f;
    } expBits;
    expBits.i = (cell->level + 0x7F) << 23;
    float scale = expBits.f;
    float x0 = ((float)cell->x - 0.2f) * scale;
    if (box->maxx < x0) {
        return 1;
    }
    float y0 = ((float)cell->y - 0.2f) * scale;
    if (box->maxy < y0) {
        return 1;
    }
    float z0 = ((float)cell->z - 0.2f) * scale;
    if (box->maxz < z0) {
        return 1;
    }
    float x1 = ((float)(cell->x + 1) + 0.2f) * scale;
    if (x1 < box->minx) {
        return 1;
    }
    float y1 = ((float)(cell->y + 1) + 0.2f) * scale;
    if (y1 < box->miny) {
        return 1;
    }
    float z1 = ((float)(cell->z + 1) + 0.2f) * scale;
    if (z1 < box->minz) {
        return 1;
    }
    if (x1 < box->maxx) {
        return 2;
    }
    if (y1 < box->maxy) {
        return 2;
    }
    if (z1 < box->maxz) {
        return 2;
    }
    if (box->minx < x1) {
        return 2;
    }
    if (box->miny < y1) {
        return 2;
    }
    if (box->miny < y1) {
        return 2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_Cell_remnode);
#ifdef SKIP_ASM
void operator_delete(int* p);

struct sItem_003284B8 {
    sItem_003284B8* next;   // 0x0
    sItem_003284B8* prev;   // 0x4
};
struct sList_003284B8 {
    sItem_003284B8* head;
    void remove(sItem_003284B8* it)
    {
        if (it->next) it->next->prev = it->prev;
        if (it->prev) {
            it->prev->next = it->next;
        } else {
            head = it->next;
        }
    }
};
struct sNode_003284B8 {
    sNode_003284B8* child[2][2][2];     // 0x00
    sList_003284B8 lists[3];            // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i].head) return 0;
        }
        return 1;
    }
};
struct sCell_003284B8 {
    int level;
    int x, y, z;
};

/**
 * Removes a node from the spatial hash cell that holds it, freeing cells that become empty.
 *
 * When this cell is at the key's level, unlinks @p item from the list for its type. Otherwise picks the child octant
 * from the key's coordinate bits, recurses into it and deletes the child when it no longer holds anything.
 *
 * @param node    this cell (On Tour Cell)
 * @param idx     the node type (On Tour NodeType)
 * @param item    the node to remove
 * @param target  the node's cell key
 * @param cur     this cell's key
 */
extern "C" void nash_Cell_remnode(sNode_003284B8* node, int idx, sItem_003284B8* item, const sCell_003284B8* target,
                              const sCell_003284B8* cur)
{
    int lv = cur->level;
    if (target->level == lv) {
        node->lists[idx].remove(item);
        return;
    }
    int sh = lv - target->level - 1;
    int dx = (target->x >> sh) - cur->x * 2;
    int dy = (target->y >> sh) - cur->y * 2;
    int dz = (target->z >> sh) - cur->z * 2;
    sCell_003284B8 childKey;
    childKey.x = cur->x * 2 + dx;
    childKey.level = --lv;
    childKey.y = cur->y * 2 + dy;
    childKey.z = cur->z * 2 + dz;
    sNode_003284B8** ch = &node->child[dx][dy][dz];
    nash_Cell_remnode(*ch, idx, item, target, &childKey);
    if ((*ch)->empty()) {
        operator_delete((int*)*ch);
        *ch = 0;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_Cell_addnode);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's operator new(size, tag, flags, align).
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void MEM_fill(void* p, int v, int n);
extern char str_Cell[];

struct sItem_00328660 {
    sItem_00328660* next;   // 0x0
    sItem_00328660* prev;   // 0x4
};
struct sList_00328660 {
    sItem_00328660* head;
    sList_00328660() : head(0) {}
    void push(sItem_00328660* it)
    {
        if (head) head->prev = it;
        it->next = head;
        it->prev = 0;
        head = it;
    }
};
struct sNode_00328660 {
    sNode_00328660* child[2][2][2];     // 0x00
    sList_00328660 lists[3];            // 0x20
    sNode_00328660() { MEM_fill(child, 0, sizeof(child)); }
};
struct sCell_00328660 {
    int level;
    int x, y, z;
};

/**
 * Adds a node to the spatial hash below this cell, creating the cells on the way.
 *
 * Steps from @p mykey down to the target key's level, choosing the child octant from the key's coordinate bits and
 * allocating (memory tag "Cell", children cleared) any child that does not exist, then pushes @p item on the reached
 * cell's list for its type.
 *
 * @param node    this cell (On Tour Cell): eight children, then one list per node type
 * @param idx     the node type (On Tour NodeType)
 * @param item    the node to add
 * @param target  the cell key the node belongs in
 * @param mykey   this cell's key
 */
extern "C" void nash_Cell_addnode(sNode_00328660* node, int idx, sItem_00328660* item, sCell_00328660* target, const sCell_00328660* mykey)
{
    sCell_00328660 cur = *mykey;
    while (cur.level != target->level) {
        int sh = cur.level - target->level - 1;
        cur.level--;
        int dx = (target->x >> sh) - cur.x * 2;
        int dy = (target->y >> sh) - cur.y * 2;
        int dz = (target->z >> sh) - cur.z * 2;
        cur.x = cur.x * 2 + dx;
        cur.y = cur.y * 2 + dy;
        cur.z = cur.z * 2 + dz;
        sNode_00328660** childSlot = &node->child[dx][dy][dz];
        if (*childSlot == 0) {
            *childSlot = new (str_Cell, 0x20000000, 0) sNode_00328660;
        }
        node = *childSlot;
    }
    node->lists[idx].push(item);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_Cell_movenode);
#ifdef SKIP_ASM
/**
 * Moves a node from one spatial hash cell to another, touching only the part of the octree that changes.
 *
 * When the new key is at this cell's level, removes the node from its old place (nash_Cell_remnode from this cell) and
 * links it here; when the old key is at this level, unlinks it here and adds it below (nash_Cell_addnode). Otherwise,
 * when both keys lead into the same child octant it recurses into that child; else it removes the node through the old
 * child (deleting that child when it becomes empty) and adds it through the new child, allocating the new child if
 * needed.
 *
 * @param node    this cell (On Tour Cell)
 * @param type    the node type (On Tour NodeType)
 * @param item    the node to move
 * @param oldkey  the cell key it was in
 * @param newkey  the cell key it moves to
 * @param mykey   this cell's key
 */
extern "C" void nash_Cell_movenode(sNode_003284B8* node, int type, sItem_00328660* item, sCell_00328660* oldkey,
                              sCell_00328660* newkey, sCell_00328660* mykey)
{
    if (newkey->level == mykey->level) {
        nash_Cell_remnode(node, type, (sItem_003284B8*)item, (sCell_003284B8*)oldkey, (sCell_003284B8*)mykey);
        ((sList_00328660*)&node->lists[type])->push(item);
        return;
    }
    if (oldkey->level == mykey->level) {
        node->lists[type].remove((sItem_003284B8*)item);
        nash_Cell_addnode((sNode_00328660*)node, type, item, newkey, mykey);
        return;
    }
    int olds = mykey->level - oldkey->level - 1;
    int oldcx = (oldkey->x >> olds) - mykey->x * 2;
    int oldcy = (oldkey->y >> olds) - mykey->y * 2;
    int oldcz = (oldkey->z >> olds) - mykey->z * 2;
    int news = mykey->level - newkey->level - 1;
    int newcx = (newkey->x >> news) - mykey->x * 2;
    int newcy = (newkey->y >> news) - mykey->y * 2;
    int newcz = (newkey->z >> news) - mykey->z * 2;
    if (oldcx == newcx && oldcy == newcy && oldcz == newcz) {
        sCell_00328660 childkey;
        childkey.level = mykey->level - 1;
        childkey.x = mykey->x * 2 + newcx;
        childkey.y = mykey->y * 2 + newcy;
        childkey.z = mykey->z * 2 + newcz;
        sNode_003284B8*& child = node->child[newcx][newcy][newcz];
        nash_Cell_movenode(child, type, item, oldkey, newkey, &childkey);
        return;
    }
    sCell_00328660 oldchildkey;
    oldchildkey.level = mykey->level - 1;
    oldchildkey.x = mykey->x * 2 + oldcx;
    oldchildkey.y = mykey->y * 2 + oldcy;
    oldchildkey.z = mykey->z * 2 + oldcz;
    sNode_003284B8*& oldchild = node->child[oldcx][oldcy][oldcz];
    nash_Cell_remnode(oldchild, type, (sItem_003284B8*)item, (sCell_003284B8*)oldkey, (sCell_003284B8*)&oldchildkey);
    if (oldchild->empty()) {
        operator_delete((int*)oldchild);
        oldchild = 0;
    }
    sCell_00328660 newchildkey;
    newchildkey.level = mykey->level - 1;
    newchildkey.x = mykey->x * 2 + newcx;
    newchildkey.y = mykey->y * 2 + newcy;
    newchildkey.z = mykey->z * 2 + newcz;
    sNode_003284B8*& newchild = node->child[newcx][newcy][newcz];
    if (newchild == 0) {
        newchild = (sNode_003284B8*)new (str_Cell, 0x20000000, 0) sNode_00328660;
    }
    nash_Cell_addnode((sNode_00328660*)newchild, type, item, newkey, &newchildkey);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_Octant_addnode);
#ifdef SKIP_ASM
struct sNode_8C20 {
    sNode_8C20* child[2][2][2];         // 0x00
    sList_00328660 lists[3];            // 0x20
    sNode_8C20() {}
    sNode_8C20(sNode_8C20* c, const sCell_00328660& cc, const sCell_00328660& pc)
    {
        int dx = cc.x - pc.x * 2;
        int dy = cc.y - pc.y * 2;
        int dz = cc.z - pc.z * 2;
        MEM_fill(child, 0, sizeof(child));
        child[dx][dy][dz] = c;
    }
};

struct sRoot_8C20 {
    sCell_00328660 cell;            // 0x00
    sNode_8C20* node;               // 0x10
};

static inline int Contains_8C20(const sCell_00328660& a, const sCell_00328660& b)
{
    int d = a.level - b.level;
    if (d < 0) {
        return 0;
    }
    if (d == 0) {
        return b.x == a.x && b.y == a.y && b.z == a.z;
    }
    return (b.x >> d) == a.x && (b.y >> d) == a.y && (b.z >> d) == a.z;
}

static inline sCell_00328660 Parent_8C20(const sCell_00328660& c)
{
    sCell_00328660 r;
    r.level = c.level + 1;
    r.x = c.x >> 1;
    r.y = c.y >> 1;
    r.z = c.z >> 1;
    return r;
}

/**
 * Adds a node to one octant of the spatial hash, growing the octant's root cell as needed.
 *
 * With no root cell yet, the root becomes a new cell (memory tag "Cell") at the node's key holding the node. Otherwise,
 * while the root's key does not contain the node's key, wraps the root in a new parent cell one level up (coordinates
 * halved) with the old root as its matching child; then adds the node below the root with nash_Cell_addnode.
 *
 * @param root  the octant (On Tour Octant): its root cell key and root cell
 * @param idx   the node type (On Tour NodeType)
 * @param item  the node to add
 * @param cell  the node's cell key
 */
extern "C" void nash_Octant_addnode(sRoot_8C20* root, int idx, sItem_00328660* item, sCell_00328660* cell)
{
    sNode_8C20** pn = &root->node;
    if (root->node == 0) {
        root->cell = *cell;
        sNode_8C20* n = new (str_Cell, 0x20000000, 0) sNode_8C20;
        MEM_fill(n->child, 0, sizeof(n->child));
        *pn = n;
        root->node->lists[idx].push(item);
        return;
    }
    while (!Contains_8C20(root->cell, *cell)) {
        sCell_00328660 nc = Parent_8C20(root->cell);
        sNode_8C20* n = new (str_Cell, 0x20000000, 0) sNode_8C20(root->node, root->cell, nc);
        root->cell = nc;
        root->node = n;
    }
    nash_Cell_addnode((sNode_00328660*)root->node, idx, item, cell, &root->cell);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_Nash_findkey);
#ifdef SKIP_ASM
struct sVec4_8F28 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBox_8F28 {
    sVec4_8F28 min;
    sVec4_8F28 max;
};

struct sCell_8F28 {
    int level;
    int x, y, z;
};

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_8F28 vu0Sub_8F28(const sVec4_8F28& a, const sVec4_8F28& b)
{
    sVec4_8F28 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4_8F28 vu0Add_8F28(const sVec4_8F28& a, const sVec4_8F28& b)
{
    sVec4_8F28 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_8F28 vu0Scale_8F28(const sVec4_8F28& v, float s)
{
    sVec4_8F28 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2 float->int->float round trip kept in the FPU (cvt.w.s / mfc1 / cvt.s.w).
static inline int Floor_8F28(float f)
{
    float t;
    int q;
    __asm__("cvt.w.s %0, %2\n\tmfc1 %1, %0\n\tcvt.s.w %0, %0" : "=&f"(t), "=r"(q) : "f"(f));
    if (f < t) {
        q--;
    }
    return q;
}

static inline sCell_8F28 Parent_8F28(const sCell_8F28& c)
{
    sCell_8F28 r;
    r.level = c.level + 1;
    r.x = c.x >> 1;
    r.y = c.y >> 1;
    r.z = c.z >> 1;
    return r;
}

/**
 * Works out which spatial hash cell a bounding box belongs in.
 *
 * Takes the box's size and centre, chooses the cell level from the largest of the x, y and z extents and stores
 * the level and the centre's cell coordinates at that level in @p key.
 *
 * @param key  out: the cell key
 * @param box  the bounding box
 */
extern "C" void nash_Nash_findkey(sCell_8F28* key, sBox_8F28* box)
{
    sVec4_8F28 size = vu0Sub_8F28(box->max, box->min);
    sVec4_8F28 center = vu0Add_8F28(box->min, vu0Scale_8F28(size, 0.5f));
    float maxExtent = size.x;
    if (maxExtent < size.y) {
        maxExtent = size.y;
    }
    if (maxExtent < size.z) {
        maxExtent = size.z;
    }
    union {
        float f;
        int i;
    } extentBits;
    extentBits.f = maxExtent * 0.7142857313156128f;
    key->level = ((extentBits.i >> 23) & 0xFF) - 0x7F;
    if (key->level < 11) {
        key->level = 11;
    }
    union {
        int i;
        float f;
    } scaleBits;
    scaleBits.i = (key->level + 0x7F) << 23;
    float scale = scaleBits.f;
    key->x = Floor_8F28(center.x / scale);
    key->y = Floor_8F28(center.y / scale);
    key->z = Floor_8F28(center.z / scale);
    for (;;) {
        scaleBits.i = (key->level + 0x7F) << 23;
        scale = scaleBits.f;
        int ok = 0;
        if (((float)key->x - 0.2f) * scale <= box->min.x && ((float)key->y - 0.2f) * scale <= box->min.y && ((float)key->z - 0.2f) * scale <= box->min.z
            && box->max.x <= ((float)(key->x + 1) + 0.2f) * scale && box->max.y <= ((float)(key->y + 1) + 0.2f) * scale && box->max.z <= ((float)(key->z + 1) + 0.2f) * scale) {
            ok = 1;
        }
        if (ok) {
            break;
        }
        *key = Parent_8F28(*key);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/aifwddiff", nash_Nash_movenode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void nash_Cell_movenode_91E0(sNode_003284B8* node, int idx, sItem_00328660* item, sCell_00328660* from,
                              sCell_00328660* to, sRoot_8C20* root) __asm__("nash_Cell_movenode");

static inline int Equal_91E0(const sCell_8F28& a, const sCell_8F28& b)
{
    return a.level == b.level && a.x == b.x && a.y == b.y && a.z == b.z;
}

static inline void Remove_91E0(sRoot_8C20* root, int idx, sItem_00328660* item, sCell_8F28* from)
{
    nash_Cell_remnode((sNode_003284B8*)root->node, idx, (sItem_003284B8*)item, (sCell_003284B8*)from,
                  (sCell_003284B8*)root);
    if (((sNode_003284B8*)root->node)->empty()) {
        operator_delete((int*)root->node);
        root->node = 0;
    }
}

static inline void Move_91E0(sRoot_8C20* root, int idx, sItem_00328660* item, sCell_8F28* from, sCell_8F28* to)
{
    if (!Contains_8C20(root->cell, *(sCell_00328660*)to)) {
        Remove_91E0(root, idx, item, from);
        nash_Octant_addnode(root, idx, item, (sCell_00328660*)to);
    } else {
        nash_Cell_movenode_91E0((sNode_003284B8*)root->node, idx, item, (sCell_00328660*)from, (sCell_00328660*)to, root);
    }
}

/**
 * Moves a node in the spatial hash (On Tour nash::Nash) after its bounding box has changed.
 *
 * Computes the cell keys of the old and new boxes; nothing happens when they are equal. The signs of the key's
 * x, y and z pick one of eight octant roots: when the octant changes the node is removed from the old one and
 * added to the new one, otherwise it is moved within its octant.
 *
 * @param roots   the eight octant roots, indexed by the key's sign bits
 * @param idx     the node type (On Tour nash::NodeType)
 * @param item    the node (On Tour nash::Node*)
 * @param box     the node's new bounding box
 * @param oldBox  the node's previous bounding box
 */
extern "C" void nash_Nash_movenode(sRoot_8C20 (*roots)[2][2], int idx, sItem_00328660* item, sBox_8F28* box,
                              sBox_8F28* oldBox)
{
    sCell_8F28 oc;
    sCell_8F28 nc;
    nash_Nash_findkey(&oc, oldBox);
    nash_Nash_findkey(&nc, box);
    if (Equal_91E0(nc, oc)) {
        return;
    }
    unsigned int nx = (unsigned int)nc.x >> 31;
    unsigned int ny = (unsigned int)nc.y >> 31;
    unsigned int nz = (unsigned int)nc.z >> 31;
    unsigned int ox = (unsigned int)oc.x >> 31;
    unsigned int oy = (unsigned int)oc.y >> 31;
    unsigned int oz = (unsigned int)oc.z >> 31;
    if (nx != ox || ny != oy || nz != oz) {
        Remove_91E0(&roots[ox][oy][oz], idx, item, &oc);
        nash_Octant_addnode(&roots[nx][ny][nz], idx, item, (sCell_00328660*)&nc);
    } else {
        Move_91E0(&roots[nx][ny][nz], idx, item, &oc, &nc);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", gIsBoxSphereInt);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVec4_29590 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4_29590 t4vect_XAxis;
extern sVec4_29590 t4vect_YAxis;
extern sVec4_29590 t4vect_ZAxis;

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_29590 vu0Scale_29590(const sVec4_29590& v, float s)
{
    sVec4_29590 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s));
    return r;
}

/**
 * Tests a sphere against an axis-aligned box face by face and returns the shallowest face contact.
 *
 * For each axis whose cross-section (the other two axes within [@p mn, @p mx]) contains the centre @p point, the depths
 * past the low and high faces are (point - mn) + margin and (mx - point) + margin; when both are non-negative the
 * shallower face that beats the best so far sets @p hp (the centre moved onto that face), @p nrm (the face's outward
 * axis, from t4vect_XAxis/YAxis/ZAxis) and *@p outDist. Contacts at the box's edges or corners, where the centre lies
 * outside every cross-section, are not found.
 *
 * Called by cWorldSphere_IsBoxInt, cRiderSphereTree_IsBoxCollision and SphereTree_isBoxCollision.
 *
 * @param margin  the sphere's radius (On Tour rad)
 * @param mn  the box minimum (On Tour bMin)
 * @param mx  the box maximum (On Tour bMax)
 * @param point  the sphere's centre (On Tour cen)
 * @param hp  out: the contact point (On Tour ipos)
 * @param nrm  out: the face normal (On Tour inorm)
 * @param outDist  out: the depth, 1e30 when there is none (On Tour param)
 * @return 1 when a face was found, else 0
 */
extern "C" int gIsBoxSphereInt(float margin, sVec4_29590* mn, sVec4_29590* mx, sVec4_29590* point, sVec4_29590* hp, sVec4_29590* nrm, float* outDist)
{
    *outDist = 1e30f;
    if (mn->y <= point->y && point->y <= mx->y && point->z >= mn->z && point->z <= mx->z) {
        float distMin = (point->x - mn->x) + margin;
        float distMax = (mx->x - point->x) + margin;
        if (0.0f <= distMin && 0.0f <= distMax) {
            if (distMin < *outDist) {
                *hp = *point;
                hp->x = mn->x;
                *outDist = distMin;
                sVec4_29590 n = vu0Scale_29590(t4vect_XAxis, -1.0f);
                *nrm = n;
            }
            if (distMax < *outDist) {
                *hp = *point;
                hp->x = mx->x;
                *outDist = distMax;
                *nrm = t4vect_XAxis;
            }
        }
    }
    if (point->x >= mn->x && point->x <= mx->x && point->z >= mn->z && point->z <= mx->z) {
        float distMin = (point->y - mn->y) + margin;
        float distMax = (mx->y - point->y) + margin;
        if (0.0f <= distMin && 0.0f <= distMax) {
            if (distMin < *outDist) {
                *hp = *point;
                hp->y = mn->y;
                *outDist = distMin;
                sVec4_29590 n = vu0Scale_29590(t4vect_YAxis, -1.0f);
                *nrm = n;
            }
            if (distMax < *outDist) {
                *hp = *point;
                hp->y = mx->y;
                *outDist = distMax;
                *nrm = t4vect_YAxis;
            }
        }
    }
    if (point->y >= mn->y && point->y <= mx->y && point->x >= mn->x && point->x <= mx->x) {
        float distMin = (point->z - mn->z) + margin;
        float distMax = (mx->z - point->z) + margin;
        if (0.0f <= distMin && 0.0f <= distMax) {
            if (distMin < *outDist) {
                *hp = *point;
                hp->z = mn->z;
                *outDist = distMin;
                sVec4_29590 n = vu0Scale_29590(t4vect_ZAxis, -1.0f);
                *nrm = n;
            }
            if (distMax < *outDist) {
                *hp = *point;
                hp->z = mx->z;
                *outDist = distMax;
                *nrm = t4vect_ZAxis;
            }
        }
    }
    return *outDist != 1e30f;
}
#endif


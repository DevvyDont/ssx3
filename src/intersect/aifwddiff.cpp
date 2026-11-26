#include "vtable.h"
#include "common.h"
#include "intersect/spheretree.h"
#include "intersect/riderspheretree.h"
#include "world/worldview.h"
#include "intersect/bxnash.h"
#include "object/rollermodifier.h"
#include "bx/math3d.h"
#include "intersect/aifwddiff.h"
#include "render/ps2graphicsman.h"
#include "world/patchrecord.h"

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_Init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void tAIFwdDiff_tAIFwdDiff(void* self);
extern char str_AIFwdDiffCache[];
extern char str_AIFwdDiffHash[];


/**
 * Sets up the AI forward-difference cache: a ring of empty entries ("AIFwdDiffCache") and a hash table of
 * 2n+13 buckets.
 *
 * @param n  number of cache entries (On Tour num)
 */
void cAIFwdDiffCache::Init(int n)
{
    tAIFwdDiff* mem = (tAIFwdDiff*)operator_new_tag(n * sizeof(tAIFwdDiff), str_AIFwdDiffCache, 0, 0);
    tAIFwdDiff* entry = mem;
    for (int k = n - 1; k != -1; k--, entry++) {
        tAIFwdDiff_tAIFwdDiff(entry);
    }
    *(tAIFwdDiff**)&mCache = mem;
    for (int i = 0; i < n; i++) {
        mCache[i].mNext = &mCache[i] + 1;
        mCache[i].mPrev = &mCache[i] - 1;
        mCache[i].mKey = tAIFwdDiff::kUnusedKey;
    }
    mCache[0].mPrev = (mCache) + n - 1;
    mCache[n - 1].mNext = (mCache);
    mHashSize = n * 2 + 13;
    *(tAIFwdDiff**)&mAgeQueue = (mCache);
    *(tAIFwdDiff***)&mPatchHash = (tAIFwdDiff**)operator_new_tag(((int)mHashSize) * 4, str_AIFwdDiffHash, 0x80000000,
            0);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_GetFwdDiff);
#ifdef SKIP_ASM
struct sFwdDiffList;
struct sFwdDiffLink;
struct sFwdDiffTable;
extern "C" void cAIFwdDiffCache_MoveToAgeQueueTail(cAIFwdDiffCache* list, sFwdDiffLink* n);
extern "C" void cAIFwdDiffCache_RemoveFromHash(void* self, void* node);
extern "C" void cAIFwdDiffCache_AddToHash(cAIFwdDiffCache* table, tAIFwdDiff* node);
extern "C" void cAIFwdDiffCache_TesselatePatch(cAIFwdDiffCache* self, void* src, cAIFwdDiffCache* node);



static inline int cAIFwdDiffCache_GetFwdDiff_less(unsigned int a, unsigned int b) { return a < b; }

/**
 * Returns the forward-difference tessellation of a terrain patch from the AI cache, building it when it is not cached.
 *
 * Looks the patch's key (m_RID) up in its hash bucket (key modulo the bucket count mHashSize; chains are sorted by
 * key). On a hit the node is moved to the tail of the age queue (cAIFwdDiffCache_MoveToAgeQueueTail). On a miss the
 * oldest node (mAgeQueue) is reused: it
 * is taken out of the hash (cAIFwdDiffCache_RemoveFromHash), given the key, hashed again (cAIFwdDiffCache_AddToHash), the age queue head moves on,
 * and the patch is tessellated into it (cAIFwdDiffCache_TesselatePatch). Called by cOverlapList_terrainIntersect and TerrainCallback_intersectpatch.
 *
 * @param src  the terrain patch record (On Tour const tPatchRecord&)
 * @param out  receives the cache node (On Tour ppFwdDiff)
 */
void cAIFwdDiffCache::GetFwdDiff(tPatchRecord* src, tAIFwdDiff** out)
{
    unsigned int key = src->m_RID;
    tAIFwdDiff* n = mPatchHash[key % mHashSize];
    while (n != 0 && !cAIFwdDiffCache_GetFwdDiff_less(key, n->mKey)) {
        if (n->mKey == key) {
            cAIFwdDiffCache_MoveToAgeQueueTail(this, (sFwdDiffLink*)n);
            *out = n;
            return;
        }
        n = n->mHashLink;
    }
    n = (mAgeQueue);
    cAIFwdDiffCache_RemoveFromHash(this, n);
    n->mKey = key;
    AddToHash(n);
    *(tAIFwdDiff**)&mAgeQueue = n->mNext;
    TesselatePatch(src, (cAIFwdDiffCache*)n);
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


/**
 * Moves an AI forward-difference cache node to the tail of the age queue, making it the most recently used.
 *
 * The queue is circular with its head (oldest) at +0x4: if the node is the head the head just moves on, if it is
 * already the tail (head->prev) nothing changes, otherwise it is unlinked and reinserted before the head.
 *
 * @param list  the cache (this)
 * @param n  the node (On Tour node)
 */
extern "C" void cAIFwdDiffCache_MoveToAgeQueueTail(cAIFwdDiffCache* list, sFwdDiffLink* n)
{
    if (((sFwdDiffLink*)list->mAgeQueue) == n) {
        (*(sFwdDiffLink**)&list->mAgeQueue) = n->next;
        return;
    }
    if (((sFwdDiffLink*)list->mAgeQueue)->prev == n) {
        return;
    }
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->prev = ((sFwdDiffLink*)list->mAgeQueue)->prev;
    n->next = ((sFwdDiffLink*)list->mAgeQueue);
    ((sFwdDiffLink*)list->mAgeQueue)->prev->next = n;
    ((sFwdDiffLink*)list->mAgeQueue)->prev = n;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_RemoveFromHash);
#ifdef SKIP_ASM


static inline int less_7B30(unsigned int a, unsigned int b) { return a < b; }

extern "C" void cAIFwdDiffCache_RemoveFromHash_7B30(cAIFwdDiffCache* self, tAIFwdDiff* node) __asm__("cAIFwdDiffCache_RemoveFromHash");
/**
 * Removes an AI forward-difference cache node from the patch hash table.
 *
 * Does nothing for an unused node (key tAIFwdDiff::kUnusedKey). Otherwise walks the node's bucket (key modulo
 * mHashSize; the chain is sorted by key, so the walk stops at a larger key) and unlinks the entry with the node's key.
 *
 * @param node  the node (On Tour node)
 */
extern "C" void cAIFwdDiffCache_RemoveFromHash_7B30(cAIFwdDiffCache* self, tAIFwdDiff* node)
{
    unsigned int key = node->mKey;
    if (key == tAIFwdDiff::kUnusedKey)
        return;
    tAIFwdDiff** link = &self->mPatchHash[key % self->mHashSize];
    if (*link == 0)
        return;
    tAIFwdDiff* n = *link;
    while (n != 0 && !less_7B30(key, n->mKey)) {
        if (n->mKey == key) {
            *link = n->mHashLink;
            return;
        }
        link = &(*link)->mHashLink;
        n = *link;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_AddToHash);
#ifdef SKIP_ASM
/**
 * Adds an AI forward-difference cache node to the patch hash table, keeping its bucket sorted by key.
 *
 * The bucket is the node's key modulo the bucket count (mHashSize); the node goes before the first entry with a larger key.
 *
 * @param node  the node (On Tour anode)
 */
void cAIFwdDiffCache::AddToHash(tAIFwdDiff* node)
{
    unsigned int key = node->mKey;
    tAIFwdDiff** link = &mPatchHash[key % mHashSize];
    for (;;) {
        tAIFwdDiff* cur = *link;
        if (cur == 0 || key < cur->mKey) {
            *link = node;
            node->mHashLink = cur;
            return;
        }
        link = &cur->mHashLink;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", cAIFwdDiffCache_TesselatePatch);
#ifdef SKIP_ASM
extern "C" void func_00391418(void* t, void* a);
extern "C" void cAIFwdDiffCache_InitializeCache(cAIFwdDiffCache* self, tAIFwdDiff* a);
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
void cAIFwdDiffCache::TesselatePatch(void* patch, cAIFwdDiffCache* fwdDiff)
{
    sGp2930* patchEval = &D_004A5A20;
    func_00391418(patchEval, (char*)patch + 0x40);
    InitializeCache((tAIFwdDiff*)fwdDiff);
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
void cAIFwdDiffCache::InitializeCache(tAIFwdDiff* patch)
{
    float* cell = (float*)(&patch->mInfo[0][0]);
    int n = 80;
    do {
        cell[0] = 10000000000.0f;
        cell[4] = 10000000000.0f;
        cell[8] = 10000000000.0f;
        cell += 16;
    } while (n-- > 0);
    cell = (float*)(&patch->mLowInfo[0][0]);
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
bxSphereTree* bxSphereTree::operatorE(bxSphereTree* src)
{
    int i;
    for (i = 0; i < 8; i++) {
        ((&axis[0]))[i] = ((&src->axis[0]))[i];
    }
    (*(&transCenter)) = (*(&src->transCenter));
    (*(&staticInfo)) = (*(&src->staticInfo));
    (*(tSphTreeInfo**)&treeInfo) = &(*(&staticInfo));
    return this;
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
int bxSphereTree::Load(char* buf, int aTreeID)
{
    if (buf == 0) {
        return 0;
    }
    staticInfo.treeID = aTreeID;
    staticInfo.lastBufID = 0;
    (*(tSphTreeInfo**)&this->treeInfo) = &(*(&staticInfo));
    staticInfo.refCount = 1;
    int len0;
    int len1;
    MEM_copy(&len0, buf, 4);
    MEM_copy(&len1, buf + 4, 4);
    MEM_copy(&this->treeInfo->isCompressed, buf + 0x8, 4);
    MEM_copy(&this->treeInfo->depth, buf + 0xC, 4);
    MEM_copy(&this->treeInfo->rootCenter[0], buf + 0x10, 0xC);
    MEM_copy(&this->treeInfo->centerOfMass[0], buf + 0x1C, 0xC);
    MEM_copy(&this->treeInfo->momentOfInertia[0][0], buf + 0x28, 36);
    MEM_copy(&this->treeInfo->invMomentOfInertia[0][0], buf + 0x4C, 36);
    char* cursor = buf + 0x70;
    this->treeInfo->levels = (tLevel*)cursor;
    cursor += (this->treeInfo->depth + 1) * 0xC;
    this->treeInfo->cmpTree = (unsigned char*)cursor;
    cursor += len1;
    cursor += len0;
    tSphTreeInfo* treeInfo = (tSphTreeInfo*)((char*)(this->treeInfo));
    if (treeInfo->isCompressed != 0) {
        treeInfo->bitTree = 0;
    } else {
        treeInfo->bitTree = (unsigned char*)((char*)treeInfo->cmpTree);
    }
    t4Vector centre;
    tSphTreeInfo* infoBlock = (tSphTreeInfo*)((char*)(this->treeInfo));
    centre.x = infoBlock->rootCenter[0];
    centre.y = infoBlock->rootCenter[1];
    centre.z = infoBlock->rootCenter[2];
    centre.w = 1.0f;
    (*(&transCenter)) = centre;
    return cursor - buf;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", bxSphereTree_IsRiderTreeCollision);
#ifdef SKIP_ASM
extern "C" int SphereTree_AllocBuf(void* self);
extern "C" int SphereTree_isSphereCollision(void* self, void* pos, void* out, int a3, int a4, int a5, int a6, float radius);


/**
 * Tests a sphere tree against a rider's spheres.
 *
 * Makes sure the tree's data buffer is loaded (SphereTree_AllocBuf when the info word +0x8 is set; the buffer goes to
 * info +0x28), then tests the rider's bounding sphere (+0x10, radius +0x20) with SphereTree_isSphereCollision from the
 * root (centre +0x80). A miss returns false. A hit returns true when the rider has no sphere list (+0x2C = 0);
 * otherwise each listed sphere (from +0x30, 0x20 bytes each) is tested and the first hit returns true. The push-out and
 * contact outputs are filled by SphereTree_isSphereCollision. Called by cWorldRider_IsSphTreeInt.
 *
 * @param shape  the rider's sphere tree (On Tour const cRiderSphereTree*)
 * @param outPush  out: how far to push apart (On Tour overlap)
 * @param outContact  out: the contact point (On Tour loc)
 * @return true on contact, else false
 */
bool bxSphereTree::IsRiderTreeCollision(cRiderSphereTree* shape, int outPush, int outContact)
{
    tSphTreeInfo* tree = treeInfo;
    if (tree->isCompressed != 0) {
        treeInfo->bitTree = (unsigned char*)SphereTree_AllocBuf(tree);
    }
    if (SphereTree_isSphereCollision(this, ((float*)&shape->boundCen), &transCenter, 0, 0, outPush, outContact,
            shape->boundRad) == 0) {
        return false;
    }
    if (shape->listLen == 0) {
        return true;
    }
    for (int i = 0; i < shape->listLen; i++) {
        if (SphereTree_isSphereCollision(this, ((float*)&((&shape->sphList[0]))[i].cen), &transCenter, 0, 0, outPush,
                outContact, ((&shape->sphList[0]))[i].rad) != 0) {
            return true;
        }
    }
    return false;
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
int bxSphereTree::Is2SidedTriCollision(t4Vector* point1, t4Vector* point2, t4Vector* point3, t4Vector* dir,
                                       t4Vector* overlap, t4Vector* loc)
{
    float dot;
    {
        t4Vector toCentre = vu0Sub_28030(*(&transCenter), *point1);
        dot = vu0Dot_28030(*dir, toCentre);
    }
    if (SphereTree_GetRadius(this) < __builtin_fabsf(dot)) {
        return 0;
    }
    if (dot < 0.0f) {
        t4Vector nd = vu0Scale_28030(*dir, -1.0f);
        return SphereTree_IsTriCollision(this, point1, point2, point3, &nd, overlap, loc);
    }
    return SphereTree_IsTriCollision(this, point1, point2, point3, dir, overlap, loc);
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
 */
void bxSphereTree::Draw()
{
    tSphTreeInfo* temp_4;

    temp_4 = (tSphTreeInfo*)((void*)treeInfo);
    if ((temp_4->isCompressed) != 0) {
        treeInfo->bitTree = (unsigned char*)SphereTree_AllocBuf(temp_4);
    }
    bxSphereTree_drawLeaves(this, (treeInfo->depth), &transCenter, 0, 0);
}
#endif
//100%
INCLUDE_ASM("intersect/aifwddiff", bxSphereTree_drawLeaves);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline void vu0Scale_281D0(t4Vector& r, const t4Vector& v, float s)
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
static inline void vu0Add_281D0(t4Vector& r, const t4Vector& a, const t4Vector& b)
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


extern "C" void func_281D0(bxSphereTree* self, int end, t4Vector* pos, int level, int off) __asm__("bxSphereTree_drawLeaves");
/**
 * Recursively draws the occupied leaf cells of a sphere tree as magenta boxes.
 *
 * Above the target level each set bit i of the occupancy byte at @p off (the info's bitTree) selects child i: its
 * centre is @p pos plus axis[i] scaled by the next level's cenOffset (the info's levels table), and its index is off +
 * (i + 1) x the level's indexOffset. At the target level it draws a box of size the level's radius x the tree's scale
 * at @p pos in colour (1, 0, 1, 0) through DrawSphere (virtual slot 87) of cGraphicsMan_TheGraphicsMan. Called by
 * bxSphereTree_Draw and itself.
 *
 * @param end  the level whose cells are drawn (On Tour desLevel)
 * @param pos  this cell's centre (On Tour center)
 * @param level  this cell's level
 * @param off  this cell's index into the occupancy bytes (On Tour index)
 */
extern "C" void func_281D0(bxSphereTree* self, int end, t4Vector* pos, int level, int off)
{
    if (level < end) {
        float levelOffset = (self->treeInfo)->levels[level + 1].cenOffset;
        for (int i = 0; i < 8; i++) {
            if (((self->treeInfo)->bitTree[off] >> i) & 1) {
                t4Vector sum;
                t4Vector offsetVec;
                t4Vector tmpVec;
                vu0Scale_281D0(tmpVec, self->axis[i], levelOffset);
                offsetVec = tmpVec;
                vu0Add_281D0(tmpVec, *pos, offsetVec);
                sum = tmpVec;
                int st = ((self->treeInfo)->levels)[level].indexOffset;
                func_281D0(self, end, &sum, level + 1, (i + 1) * st + off);
            }
        }
    } else {
        cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
        sVEntry<void(void*, float, void*, void*, int)>* vtable =
            (sVEntry<void(void*, float, void*, void*, int)>*)VPTR_OF(cGraphicsMan, w);
        sVEntry<void(void*, float, void*, void*, int)>* vt = &vtable[87]; // DrawSphere
        void* thisp = vthis(w, *vt);
        t4Vector color;
        float boxSize = ((self->treeInfo)->levels)[level].radius * self->scale;
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
extern "C" int nash_CellKey_proximitytest(nash_CellKey* cell, tBoundBox* box)
{
    union {
        int i;
        float f;
    } expBits;
    expBits.i = (cell->level + 0x7F) << 23;
    float scale = expBits.f;
    float x0 = ((float)cell->x - 0.2f) * scale;
    if (box->MaxPoint.x < x0) {
        return 1;
    }
    float y0 = ((float)cell->y - 0.2f) * scale;
    if (box->MaxPoint.y < y0) {
        return 1;
    }
    float z0 = ((float)cell->z - 0.2f) * scale;
    if (box->MaxPoint.z < z0) {
        return 1;
    }
    float x1 = ((float)(cell->x + 1) + 0.2f) * scale;
    if (x1 < box->MinPoint.x) {
        return 1;
    }
    float y1 = ((float)(cell->y + 1) + 0.2f) * scale;
    if (y1 < box->MinPoint.y) {
        return 1;
    }
    float z1 = ((float)(cell->z + 1) + 0.2f) * scale;
    if (z1 < box->MinPoint.z) {
        return 1;
    }
    if (x1 < box->MaxPoint.x) {
        return 2;
    }
    if (y1 < box->MaxPoint.y) {
        return 2;
    }
    if (z1 < box->MaxPoint.z) {
        return 2;
    }
    if (box->MinPoint.x < x1) {
        return 2;
    }
    if (box->MinPoint.y < y1) {
        return 2;
    }
    if (box->MinPoint.y < y1) {
        return 2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_Cell_remnode);
#ifdef SKIP_ASM
void operator_delete(int* p);

/**
 * Removes a node from the spatial hash cell that holds it, freeing cells that become empty.
 *
 * When this cell is at the key's level, unlinks @p item from the list for its type. Otherwise picks the child octant
 * from the key's coordinate bits, recurses into it and deletes the child when it no longer holds anything.
 *
 * @param idx     the node type (On Tour NodeType)
 * @param item    the node to remove
 * @param target  the node's cell key
 * @param cur     this cell's key
 */
void nash_Cell::remnode(int idx, nash_Node* item, const nash_CellKey* target, const nash_CellKey* cur)
{
    int lv = cur->level;
    if (target->level == lv) {
        nodesinside[idx].remove(item);
        return;
    }
    int sh = lv - target->level - 1;
    int dx = (target->x >> sh) - cur->x * 2;
    int dy = (target->y >> sh) - cur->y * 2;
    int dz = (target->z >> sh) - cur->z * 2;
    nash_CellKey childKey;
    childKey.x = cur->x * 2 + dx;
    childKey.level = --lv;
    childKey.y = cur->y * 2 + dy;
    childKey.z = cur->z * 2 + dz;
    nash_Cell** ch = &children[dx][dy][dz];
    (*ch)->remnode(idx, item, target, &childKey);
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

// A new Cell (On Tour Cell::Cell()): the base clears the three node lists, then the children are cleared.
struct tCellNew : nash_Cell {
    tCellNew() { MEM_fill(children, 0, sizeof(children)); }
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
extern "C" void nash_Cell_addnode(nash_Cell* node, int idx, nash_Node* item, nash_CellKey* target,
                                  const nash_CellKey* mykey)
{
    nash_CellKey cur = *mykey;
    while (cur.level != target->level) {
        int sh = cur.level - target->level - 1;
        cur.level--;
        int dx = (target->x >> sh) - cur.x * 2;
        int dy = (target->y >> sh) - cur.y * 2;
        int dz = (target->z >> sh) - cur.z * 2;
        cur.x = cur.x * 2 + dx;
        cur.y = cur.y * 2 + dy;
        cur.z = cur.z * 2 + dz;
        nash_Cell** childSlot = &node->children[dx][dy][dz];
        if (*childSlot == 0) {
            *childSlot = new (str_Cell, 0x20000000, 0) tCellNew;
        }
        node = *childSlot;
    }
    node->nodesinside[idx].prepend(item);
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
 * @param type    the node type (On Tour NodeType)
 * @param item    the node to move
 * @param oldkey  the cell key it was in
 * @param newkey  the cell key it moves to
 * @param mykey   this cell's key
 */
void nash_Cell::movenode(int type, nash_Node* item, nash_CellKey* oldkey, nash_CellKey* newkey, nash_CellKey* mykey)
{
    if (newkey->level == mykey->level) {
        remnode(type, (nash_Node*)item, oldkey, mykey);
        nodesinside[type].prepend(item);
        return;
    }
    if (oldkey->level == mykey->level) {
        nodesinside[type].remove((nash_Node*)item);
        nash_Cell_addnode(this, type, item, newkey, mykey);
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
        nash_CellKey childkey;
        childkey.level = mykey->level - 1;
        childkey.x = mykey->x * 2 + newcx;
        childkey.y = mykey->y * 2 + newcy;
        childkey.z = mykey->z * 2 + newcz;
        nash_Cell*& child = children[newcx][newcy][newcz];
        child->movenode(type, item, oldkey, newkey, &childkey);
        return;
    }
    nash_CellKey oldchildkey;
    oldchildkey.level = mykey->level - 1;
    oldchildkey.x = mykey->x * 2 + oldcx;
    oldchildkey.y = mykey->y * 2 + oldcy;
    oldchildkey.z = mykey->z * 2 + oldcz;
    nash_Cell*& oldchild = children[oldcx][oldcy][oldcz];
    oldchild->remnode(type, (nash_Node*)item, oldkey, (&oldchildkey));
    if (oldchild->empty()) {
        operator_delete((int*)oldchild);
        oldchild = 0;
    }
    nash_CellKey newchildkey;
    newchildkey.level = mykey->level - 1;
    newchildkey.x = mykey->x * 2 + newcx;
    newchildkey.y = mykey->y * 2 + newcy;
    newchildkey.z = mykey->z * 2 + newcz;
    nash_Cell*& newchild = children[newcx][newcy][newcz];
    if (newchild == 0) {
        newchild = new (str_Cell, 0x20000000, 0) tCellNew;
    }
    nash_Cell_addnode(newchild, type, item, newkey, &newchildkey);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_Octant_addnode);
#ifdef SKIP_ASM
// A new root Cell one level up (On Tour Cell::Cell(childkey, child, mykey)): the base clears the three node lists,
// then the children are cleared and the old root becomes the child at its octant.
struct tCellWrap : nash_Cell {
    tCellWrap(const nash_CellKey& childkey, nash_Cell* child, const nash_CellKey& mykey)
    {
        int dx = childkey.x - mykey.x * 2;
        int dy = childkey.y - mykey.y * 2;
        int dz = childkey.z - mykey.z * 2;
        MEM_fill(children, 0, sizeof(children));
        children[dx][dy][dz] = child;
    }
};


static inline int Contains_8C20(const nash_CellKey& a, const nash_CellKey& b)
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

static inline nash_CellKey Parent_8C20(const nash_CellKey& c)
{
    nash_CellKey r;
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
 * @param idx   the node type (On Tour NodeType)
 * @param item  the node to add
 * @param cell  the node's cell key
 */
void nash_Octant::addnode(int idx, nash_Node* item, nash_CellKey* cell)
{
    nash_Cell** pn = &rootcell;
    if (rootcell == 0) {
        rootkey = *cell;
        nash_Cell* n = new (str_Cell, 0x20000000, 0) tCellNew;
        *pn = n;
        rootcell->nodesinside[idx].prepend(item);
        return;
    }
    while (!Contains_8C20(rootkey, *cell)) {
        nash_CellKey nc = Parent_8C20(rootkey);
        nash_Cell* n = new (str_Cell, 0x20000000, 0) tCellWrap(rootkey, rootcell, nc);
        rootkey = nc;
        rootcell = n;
    }
    nash_Cell_addnode(rootcell, idx, item, cell, &rootkey);
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", nash_Nash_findkey);
#ifdef SKIP_ASM



// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_8F28(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_8F28(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector vu0Scale_8F28(const t4Vector& v, float s)
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

static inline nash_CellKey Parent_8F28(const nash_CellKey& c)
{
    nash_CellKey r;
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
extern "C" void nash_Nash_findkey(nash_CellKey* key, tBoundBox* box)
{
    t4Vector size = vu0Sub_8F28((*(&box->MaxPoint)), (*(&box->MinPoint)));
    t4Vector center = vu0Add_8F28((*(&box->MinPoint)), vu0Scale_8F28(size, 0.5f));
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
        if (((float)key->x - 0.2f) * scale <= (*(&box->MinPoint)).x && ((float)key->y - 0.2f) * scale <= (*(&box->MinPoint)).y && ((float)key->z - 0.2f) * scale <= (*(&box->MinPoint)).z
            && (*(&box->MaxPoint)).x <= ((float)(key->x + 1) + 0.2f) * scale && (*(&box->MaxPoint)).y <= ((float)(key->y + 1) + 0.2f) * scale && (*(&box->MaxPoint)).z <= ((float)(key->z + 1) + 0.2f) * scale) {
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
extern "C" void nash_Cell_movenode_91E0(nash_Cell* node, int idx, nash_Node* item, nash_CellKey* from,
                              nash_CellKey* to, nash_Octant* root) __asm__("nash_Cell_movenode");

static inline int Equal_91E0(const nash_CellKey& a, const nash_CellKey& b)
{
    return a.level == b.level && a.x == b.x && a.y == b.y && a.z == b.z;
}

static inline void Remove_91E0(nash_Octant* root, int idx, nash_Node* item, nash_CellKey* from)
{
    root->rootcell->remnode(idx, (nash_Node*)item, from, (nash_CellKey*)root);
    if (root->rootcell->empty()) {
        operator_delete((int*)root->rootcell);
        root->rootcell = 0;
    }
}

static inline void Move_91E0(nash_Octant* root, int idx, nash_Node* item, nash_CellKey* from, nash_CellKey* to)
{
    if (!Contains_8C20(root->rootkey, *(nash_CellKey*)to)) {
        Remove_91E0(root, idx, item, from);
        root->addnode(idx, item, (nash_CellKey*)to);
    } else {
        nash_Cell_movenode_91E0(root->rootcell, idx, item, (nash_CellKey*)from, (nash_CellKey*)to, root);
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
extern "C" void nash_Nash_movenode(nash_Octant (*roots)[2][2], int idx, nash_Node* item, tBoundBox* box,
                              tBoundBox* oldBox)
{
    nash_CellKey oc;
    nash_CellKey nc;
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
        roots[nx][ny][nz].addnode(idx, item, (nash_CellKey*)&nc);
    } else {
        Move_91E0(&roots[nx][ny][nz], idx, item, &oc, &nc);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/aifwddiff", gIsBoxSphereInt);
#ifdef SKIP_ASM

extern t4Vector t4vect_XAxis;
extern t4Vector t4vect_YAxis;
extern t4Vector t4vect_ZAxis;

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_29590(const t4Vector& v, float s)
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
 * @return true when a face was found, else false
 */
extern "C" bool gIsBoxSphereInt(float margin, t4Vector* mn, t4Vector* mx, t4Vector* point, t4Vector* hp, t4Vector* nrm,
        float* outDist)
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
                t4Vector n = vu0Scale_29590(t4vect_XAxis, -1.0f);
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
                t4Vector n = vu0Scale_29590(t4vect_YAxis, -1.0f);
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
                t4Vector n = vu0Scale_29590(t4vect_ZAxis, -1.0f);
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


#include "common.h"
#include "object/instancenode.h"

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_cInstanceNode);
#ifdef SKIP_ASM
struct sInstNodeCtorVEntry {
    short delta;
    short index;
    int (*fn)(void*, void*);
};

extern "C" void* cSortObjNode_cSortObjNode(void* self, void* a1, void* stream);
extern char cInstanceNode_vtbl[];

/**
 * Rebuilds an instance node from a replay frame: the cSortObjNode part, its vtable and its instance record.
 *
 * The record comes from the frame's readReplayInstance(node) (frame slot 4, given this node).
 *
 * @param bucket      bucket index, passed on to the cSortObjNode constructor (On Tour const int bucket)
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" void* cInstanceNode_cInstanceNode(cInstanceNode* self, void* bucket, void* stream)
{
    cSortObjNode_cSortObjNode(self, bucket, stream);
    self->mVtbl = (void**)cInstanceNode_vtbl;
    sInstNodeCtorVEntry* vt = *(sInstNodeCtorVEntry**)stream;
    self->Instance = (tInstanceRecord*)vt[4].fn((char*)stream + vt[4].delta, self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode__cInstanceNode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0034FC80(void* self, void* a1);
extern void* cSortObjNode__cSortObjNode_dtor(void* self, int flags) __asm__("cSortObjNode__cSortObjNode__FPv");

/**
 * Destroys an instance node: detaches it from its instance record, resets the record's flags and runs the
 * cSortObjNode destructor.
 *
 * func_0034FC80 (On Tour clearActiveNode) clears the record's node pointer (+0xC) if it points here. The
 * record's flags (+0x8) keep the upper half and bits 8-9, get the upper half ORed into the lower half and get
 * bit 1 set.
 *
 * @param flags  g++ destructor flags, passed on to the cSortObjNode destructor (bit 0 frees the memory)
 */
extern "C" void cInstanceNode__cInstanceNode(cInstanceNode* self, int flags)
{
    self->mVtbl = (void**)cInstanceNode_vtbl;
    func_0034FC80(self, self->Instance);
    *(int*)((char*)self->Instance + 8) &= 0xFFFF0300;
    {
        char* instance = (char*)self->Instance;
        int instFlags = *(int*)(instance + 8);
        *(int*)(instance + 8) = instFlags | (instFlags >> 16);
    }
    *(int*)((char*)self->Instance + 8) |= 2;
    cSortObjNode__cSortObjNode_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FC78);
#ifdef SKIP_ASM
extern "C" void func_0034FC78(int node, void* inst)
{
    *(int*)((char*)inst + 0xc) = node;
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FC80);
#ifdef SKIP_ASM
extern "C" void func_0034FC80(void* self, void* inst)
{
    if (inst != 0) {
        if (*(int*)((char*)inst + 0xc) == (int)self) {
            *(int*)((char*)inst + 0xc) = 0;
        }
    }
}
#endif

extern "C" void* func_002D19E8(int);

//100%
INCLUDE_ASM("object/instancenode", func_0034FCA0);
#ifdef SKIP_ASM
extern "C" void func_002D1A78(int);

extern "C" void func_0034FCA0(cInstanceNode* instanceNode) {
    func_002D1A78(((int)instanceNode->Instance));
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FCC0__FPv);
#ifdef SKIP_ASM
void* func_0034FCC0(void* self)
{
    return func_002D19E8(*(int*)((char*)self + 0x18));
}
#endif

extern "C" void* cObjectInterface_runOnLoseFocusScript(int);

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_runOnLoseFocusScript__FPv);
#ifdef SKIP_ASM
/**
 * Runs the lose-focus script of the node's instance through cObjectInterface_runOnLoseFocusScript().
 *
 * The instance record pointer at +0x18 is passed as the argument.
 *
 * @return cObjectInterface_runOnLoseFocusScript()'s result
 */
void* cInstanceNode_runOnLoseFocusScript(void* self)
{
    return cObjectInterface_runOnLoseFocusScript(*(int*)((char*)self + 0x18));
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_expire);
#ifdef SKIP_ASM
struct sInstNodeVEntryFD00 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Expires an instance node: unless an expire script takes over, resets the record's flags and deletes the
 * node.
 *
 * With @p runscript set, func_0034FCC0 (On Tour runExpireScript) looks for the instance's expire script; when
 * one runs, the node is left alone. Otherwise the record's flags (+0x8) are reset as in the destructor and
 * the node deletes itself through its virtual destructor (slot 1, flags 3).
 *
 * @param runscript  nonzero to try the instance's expire script first
 */
extern "C" void cInstanceNode_expire(cInstanceNode* self, int runscript)
{
    if (runscript != 0) {
        if (func_0034FCC0(self) != 0) return;
    }
    *(int*)((char*)self->Instance + 8) &= 0xFFFF0300;
    {
        char* instance = (char*)self->Instance;
        int instFlags = *(int*)(instance + 8);
        *(int*)(instance + 8) = instFlags | (instFlags >> 16);
    }
    *(int*)((char*)self->Instance + 8) |= 2;
    if (((void*)self) != 0) {
        sInstNodeVEntryFD00* vt = (sInstNodeVEntryFD00*)self->mVtbl;
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_loseFocus);
#ifdef SKIP_ASM
struct sInstNodeVEntryFD90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Drops an instance node that has lost focus: resets the record's flags and deletes the node.
 *
 * Like cInstanceNode_expire() without the script check, except that bits 8-9 of the record's flags (+0x8) are
 * cleared too (mask 0xFFFF0000) before the upper half is ORed into the lower half and bit 1 is set.
 */
extern "C" void cInstanceNode_loseFocus(cInstanceNode* self)
{
    int* flagsPtr = (int*)((char*)self->Instance + 0x8);
    *flagsPtr &= 0xFFFF0000;
    flagsPtr = (int*)((char*)self->Instance + 0x8);
    *flagsPtr |= *flagsPtr >> 16;
    flagsPtr = (int*)((char*)self->Instance + 0x8);
    *flagsPtr |= 2;
    if (((void*)self) != 0) {
        sInstNodeVEntryFD90* vt = (sInstNodeVEntryFD90*)self->mVtbl;
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

extern "C" void* cObjectInterface_runCollideScript(int, int, int);

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_runCollideScript__FPviii);
#ifdef SKIP_ASM
/**
 * Runs the world script attached to a collision with this instance.
 *
 * Forwards the collision, intersection and being to cObjectInterface_runCollideScript (On Tour cObjectInterface::runCollideScript);
 * the node itself is not used.
 *
 * @param collideInfo  the collision info (On Tour const tCollideInfo* tc)
 * @param intersectInfo  the intersection info (On Tour const tIntersectInfo* ti)
 * @param pBeing  the being that collided (On Tour cBeing* pBeing)
 * @return cObjectInterface_runCollideScript's result (On Tour's version returns nothing)
 */
void* cInstanceNode_runCollideScript(void* self, int collideInfo, int intersectInfo, int pBeing)
{
    return cObjectInterface_runCollideScript(collideInfo, intersectInfo, pBeing);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_makeThisDead);
#ifdef SKIP_ASM
struct sInstNodeDtorVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char str_DeadNode_b[];
extern "C" void* cDeadNode_cDeadNode(void* mem, int v);

/**
 * Replaces an instance node by a dead node: deletes this node and creates a cDeadNode for the same record.
 *
 * The node deletes itself through its virtual destructor (slot 1, flags 3), then a 0x1C-byte block
 * ("DeadNode") is constructed as a cDeadNode for the saved instance record by cDeadNode_cDeadNode. The record's
 * flags are not touched.
 */
extern "C" void cInstanceNode_makeThisDead(cInstanceNode* self)
{
    int instance = (int)self->Instance;
    if (((void*)self) != 0) {
        sInstNodeDtorVEntry* vt = (sInstNodeDtorVEntry*)self->mVtbl;
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
    cDeadNode_cDeadNode(cMemMan_alloc(0x1C, str_DeadNode_b, 0x20000000, 0), instance);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sInstNodeVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void cSortObjNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes an instance node to a replay frame: the cSortObjNode data, then the node through the stream's slot 6.
 *
 * The stream's virtual slot 6 takes the node itself, probably to record which instance it belongs to.
 *
 * @param stream  the replay frame stream to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cInstanceNode_writeToReplayFrame(cInstanceNode* self, void* stream)
{
    cSortObjNode_writeToReplayFrame(self, stream);
    sInstNodeVEntry* vt = *(sInstNodeVEntry**)stream;
    vt[6].fn((char*)stream + vt[6].delta, self);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_buildMeshMat);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Prototype asserts: assert(inst); assert((mesh >= -1) && (mesh < pModel->m_iNumMeshes)); assert((int(a) & 15) == 0) (bx/math3d.h).
struct sInV4_FED8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sInMat_FED8 {
    sInV4_FED8 r[4];
};

struct sInPart_FED8 {
    int parent;
    int pad4;
    int pad8;
    sInMat_FED8* mat;   // 0xC
};

struct sInModel_FED8 {
    int pad0;
    int m_iNumMeshes;       // 0x4
    sInPart_FED8* parts;    // 0x8
};

struct sInVE_FED8 {
    short delta;
    short index;
    void* fn;
};

struct sInOther_FED8 {
    char pad_0x00[0xC];
    sInVE_FED8* vt;         // 0xC
};

struct sInInst_FED8 {
    char pad_0x00[0xC];
    sInOther_FED8* other;   // 0xC
    sInMat_FED8 world;      // 0x10
    char pad_0x50[0x30];
    sInModel_FED8* pModel;  // 0x80
    float scale;            // 0x84
};

typedef int (*InBoolFn_FED8)(void*);
typedef sInMat_FED8* (*InMatFn_FED8)(void*);
typedef sInMat_FED8* (*InPartMatFn_FED8)(void*, int);

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMat_FED8(sInMat_FED8* d, const sInMat_FED8* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

static inline sInV4_FED8 row_FED8(const sInMat_FED8* m, int i)
{
    return m->r[i];
}

static inline void mulv_FED8(sInV4_FED8& r, const sInV4_FED8& a, const sInV4_FED8& b)
{
    r.x = a.x * b.x;
    r.y = a.y * b.y;
    r.z = a.z * b.z;
    r.w = a.w * b.w;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply into a VU0 temporary, then d = t).
static inline void mul_FED8(sInMat_FED8* d, const sInMat_FED8* a, const sInMat_FED8* b)
{
    sInMat_FED8 t;
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(&t), "r"(a), "r"(b)
        : "memory");
    vu0CopyMat_FED8(d, &t);
}

// m = a * m
static inline void mulLeft_FED8(sInMat_FED8* m, const sInMat_FED8* a)
{
    sInMat_FED8 t2;
    mul_FED8(&t2, a, m);
    vu0CopyMat_FED8(m, &t2);
}

// dst = (local with its translation row scaled by s) * m.
static inline void mulScaled_FED8(sInMat_FED8* dst, const sInMat_FED8* local, const sInV4_FED8& s, const sInMat_FED8* m)
{
    sInMat_FED8 l;
    l.r[0] = row_FED8(local, 0);
    l.r[1] = row_FED8(local, 1);
    l.r[2] = row_FED8(local, 2);
    sInV4_FED8 t;
    sInV4_FED8 r3 = local->r[3];
    mulv_FED8(t, r3, s);
    l.r[3] = t;
    mul_FED8(dst, &l, m);
}

/**
 * Builds the world matrix of one mesh of a placed instance, walking up the model's mesh hierarchy.
 *
 * If the instance has a node (+0xC) whose virtual slot 26 says it has animation matrices, mesh @p mesh's matrix
 * comes straight from its slot 29 (getAnimMat). Otherwise the base is the node's root transform (slot 24) or the
 * record's own matrix (+0x10). For mesh >= 0 the mesh's local matrix (model +0x80: parts +0x8, matrix +0xC) is
 * multiplied by each parent's (parent index +0x0, until -1), every translation row scaled by the instance scale
 * (+0x84), and finally by the base; mesh -1 gives the base itself.
 *
 * @param self  the instance record (static: On Tour inst)
 * @param mesh  the mesh index, or -1 for the instance's root
 * @param out   receives the matrix (On Tour mat)
 */
// Builds the world matrix of mesh `mesh` by walking its parent chain.
extern "C" void cInstanceNode_buildMeshMat(sInInst_FED8* self, int mesh, sInMat_FED8* out)
{
    sInMat_FED8* base = &self->world;
    sInModel_FED8* pModel = self->pModel;
    sInOther_FED8* instNode = self->other;
    if (instNode != 0) {
        if (mesh != -1 && ((InBoolFn_FED8)instNode->vt[26].fn)((char*)instNode + instNode->vt[26].delta)) {
            vu0CopyMat_FED8(out, ((InPartMatFn_FED8)self->other->vt[29].fn)((char*)self->other + self->other->vt[29].delta, mesh));
            return;
        }
        base = ((InMatFn_FED8)self->other->vt[24].fn)((char*)self->other + self->other->vt[24].delta);
    }
    sInPart_FED8* parts = pModel->parts;
    if (mesh >= 0) {
        float k = self->scale;
        sInV4_FED8 scaleVec;
        scaleVec.x = k;
        scaleVec.y = k;
        scaleVec.z = k;
        scaleVec.w = 1.0f;
        vu0CopyMat_FED8(out, parts[mesh].mat);
        {
            sInV4_FED8 scaledPos;
            sInV4_FED8 translation = out->r[3];
            mulv_FED8(scaledPos, translation, scaleVec);
            out->r[3] = scaledPos;
        }
        for (mesh = parts[mesh].parent; mesh != -1; mesh = parts[mesh].parent) {
            sInMat_FED8 chained;
            mulScaled_FED8(&chained, parts[mesh].mat, scaleVec, out);
            vu0CopyMat_FED8(out, &chained);
        }
        mulLeft_FED8(out, base);
    } else {
        vu0CopyMat_FED8(out, base);
    }
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_makeAnimMats);
#ifdef SKIP_ASM
struct sInMat44_0288 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sInVec4_0288 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sInBone_0288 {
    int parent;                 // 0x0
    int f4;
    int f8;
    sInMat44_0288* mat;         // 0xC
};

struct sInSkel_0288 {
    int f0;
    int count;                  // 0x4
    sInBone_0288* bones;        // 0x8
};

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_0288(sInMat44_0288* d, const sInMat44_0288* a, const sInMat44_0288* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMat_0288(sInMat44_0288* d, const sInMat44_0288* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

static inline void buildBone_0288(sInMat44_0288* d, const sInMat44_0288* parent, const sInMat44_0288* m, const sInVec4_0288& scale)
{
    sInMat44_0288 local;
    sInVec4_0288 v;
    sInVec4_0288 tr;
    sInMat44_0288 t;
    v = *(sInVec4_0288*)m->m[0];
    *(sInVec4_0288*)local.m[0] = v;
    v = *(sInVec4_0288*)m->m[1];
    *(sInVec4_0288*)local.m[1] = v;
    v = *(sInVec4_0288*)m->m[2];
    *(sInVec4_0288*)local.m[2] = v;
    tr = *(sInVec4_0288*)m->m[3];
    v.x = tr.x * scale.x;
    v.y = tr.y * scale.y;
    v.z = tr.z * scale.z;
    v.w = tr.w * scale.w;
    *(sInVec4_0288*)local.m[3] = v;
    vu0MulMat_0288(&t, parent, &local);
    vu0CopyMat_0288(d, &t);
}

/**
 * Builds the matrix of every mesh of the instance's model from its parent's matrix and the mesh's local
 * matrix.
 *
 * For each mesh (model at record +0x80: count +0x4, 0x10-byte entries +0x8) the local matrix's translation is
 * scaled by the record's scale (+0x84), then combined with out[parent], or with @p parent for a mesh whose
 * parent index is -1. A parent mesh must therefore come before its children.
 *
 * @param parent  root transform for meshes without a parent (On Tour root)
 * @param out     receives one matrix per mesh (On Tour animMats)
 */
extern "C" void cInstanceNode_makeAnimMats(cInstanceNode* self, sInMat44_0288* parent, sInMat44_0288* out)
{
    char* instance = (char*)self->Instance;
    sInSkel_0288* sk = *(sInSkel_0288**)(instance + 0x80);
    float uniformScale = *(float*)(instance + 0x84);
    sInBone_0288* bone = sk->bones;
    sInVec4_0288 scale;
    scale.x = uniformScale;
    scale.y = uniformScale;
    scale.z = uniformScale;
    scale.w = 1.0f;
    for (int i = 0; i < sk->count; i++, bone++) {
        sInMat44_0288 boneMat;
        if (bone->parent != -1) {
            buildBone_0288(&boneMat, &out[bone->parent], bone->mat, scale);
            vu0CopyMat_0288(&out[i], &boneMat);
        } else {
            buildBone_0288(&boneMat, parent, bone->mat, scale);
            vu0CopyMat_0288(&out[i], &boneMat);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_getBoundBoxInfo);
#ifdef SKIP_ASM
struct sIbVec4
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sIbVec4 IbSub(const sIbVec4& a, const sIbVec4& b)
{
    sIbVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (dot product).
static inline float IbDot(const sIbVec4& a, const sIbVec4& b)
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

// PORT: sqrt.s (sqrtf without errno check)
static inline float IbSqrt(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

/**
 * Reports the instance's bound box and the radius around the instance's position that holds all of it.
 *
 * box[0] and box[1] get the record's min (+0x60) and max (+0x6C) corners with w = 1; the radius is the
 * largest distance from the record's position (+0x40) to any of the box's eight corners.
 *
 * @param box     receives the min and max corners (On Tour tBoundBox&)
 * @param radius  receives the radius
 */
// Writes the model's bounding box (min, max) and the radius of the sphere around its centre that holds every corner.
extern "C" void cInstanceNode_getBoundBoxInfo(cInstanceNode* self, sIbVec4* box, float* radius)
{
    sIbVec4 point;
    point.x = *(float*)((char*)self->Instance + 0x60);
    point.y = *(float*)((char*)self->Instance + 0x64);
    point.z = *(float*)((char*)self->Instance + 0x68);
    point.w = 1.0f;
    box[0] = point;
    point.x = *(float*)((char*)self->Instance + 0x6C);
    point.y = *(float*)((char*)self->Instance + 0x70);
    point.z = *(float*)((char*)self->Instance + 0x74);
    box[1] = point;
    point = *(sIbVec4*)((char*)self->Instance + 0x40);
    sIbVec4* mx = &box[1];
    sIbVec4* ctr = &point;
    float best = 0.0f;
    for (int i = 0; i < 8; i++) {
        float x = (i & 1) ? box[0].x : mx->x;
        float y = (i & 2) ? box[0].y : mx->y;
        float z = (i & 4) ? box[0].z : mx->z;
        sIbVec4 corner;
        corner.x = x;
        corner.y = y;
        corner.z = z;
        corner.w = 1.0f;
        sIbVec4 delta = IbSub(corner, *ctr);
        float distSq = IbDot(delta, delta);
        if (best < distSq) best = distSq;
    }
    *radius = IbSqrt(best);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_00350698);
#ifdef SKIP_ASM
struct sInstNodeEntry {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
};

struct sInstNodeList {
    int field_0x0;
    int count;                // 0x4
    sInstNodeEntry* entries;  // 0x8
};

extern "C" int func_00350698(void* self)
{
    sInstNodeList* list = *(sInstNodeList**)((char*)self + 0x80);
    int i;
    int count = list->count;
    sInstNodeEntry* e = list->entries;
    for (i = 0; i < count; i++, e++) {
        if (e->field_0x8 != 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/instancenode", cDeadNode_cDeadNode);
#ifdef SKIP_ASM
extern "C" void* func_0034FB00(void* self, int a1, int type, int a3);
extern char cDeadNode_vtbl[];

/**
 * Constructs a dead node: the placeholder that keeps an expired or collected instance out of the world.
 *
 * Runs the cInstanceNode constructor (func_0034FB00) for bucket 8 with node type 6 and the record, stores
 * cDeadNode_vtbl, then edits the record's flags (+0x8): clears bits 5-6, clears bit 1 and sets bit 2. Allocated as
 * 0x1C bytes ("DeadNode") by cInstanceNode_makeThisDead, the debounce node and the world scripts.
 *
 * @param pInst  the instance record (On Tour pInst)
 * @return the constructed node (self)
 */
extern "C" void* cDeadNode_cDeadNode(void* self, int pInst)
{
    func_0034FB00(self, 8, 6, pInst);
    *(void**)((char*)self + 0xC) = cDeadNode_vtbl;
    unsigned int* recordFlags = (unsigned int*)(*(char**)((char*)self + 0x18) + 0x8);
    *recordFlags &= 0xFFFFFF9F;
    recordFlags = (unsigned int*)(*(char**)((char*)self + 0x18) + 0x8);
    *recordFlags = (*recordFlags & 0xFFFFFFFD) | 4;
    return self;
}
#endif


#include "vtable.h"
#include "common.h"
#include "object/effectmodifier.h"
#include "render/particle.h"
#include "object/boostnode.h"
#include "object/deadfadenode.h"
#include "world/instancerecord.h"
#include "object/modifierblock.h"
#include "world/worldrecords.h"
#include "intersect/spheretree.h"
#include "render/ps2bezierman.h"
#include "ai/rider.h"
#include "ui/uithing.h"
#include "bx/appman.h"
#include "render/bxps2deferman.h"
#include "render/bxps2tex.h"
#include "object/objectnode.h"
#include "object/restorenode.h"
#include "object/particlenode.h"
#include "object/hidenode.h"
#include "object/flexbridgenode.h"
#include "object/debouncenode.h"
#include "object/deadnode.h"
#include "object/counternode.h"
#include "object/conveyornode.h"
#include "object/animdeltanode.h"
#include "object/animframe.h"
#include "object/flagset.h"
#include "object/animteeternode.h"
#include "object/springrailnode.h"
#include "object/floatrailnode.h"
#include "object/animobject.h"
#include "object/animnode.h"
#include "object/objnode.h"
#include "object/flexrailnode.h"
#include "object/animcombonode.h"
#include "world/worldview.h"
#include "object/uvscrollmodifier.h"
#include "object/transformmodifier.h"
#include "object/texflipmodifier.h"
#include "object/splinemodifier.h"
#include "object/railmodifier.h"
#include "object/railspacenode.h"
#include "replay/bxreplayframe.h"
#include "object/raillink.h"
#include "object/positionmodifier.h"
#include "object/physicsmodifier.h"
#include "object/parentmodifier.h"
#include "object/multisplinemodifier.h"
#include "object/movenode.h"
#include "object/magnetmodifier.h"
#include "object/floatingnode.h"
#include "object/effectlink.h"
#include "object/boostmodifier.h"
#include "object/avasplinemodifier.h"
#include "object/rigidbody.h"
#include "object/rollermodifier.h"
#include "render/ps2graphicsman.h"
#include "object/instancenode.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_buildXform);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector vu0Scale(const t4Vector& v, float s)
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
        : "m"(v), "f"(s)
        : "memory");
    return r;
}


struct sVecK_C0E8 : public t4Vector {
    sVecK_C0E8(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};


// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_C0E8(t4Matrix* d, const t4Matrix* a, const t4Matrix* b)
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
static inline void vu0CopyMat_C0E8(t4Matrix* d, const t4Matrix* s)
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

// PORT: PS2-only VU0 inline asm (vector * matrix, row-vector convention).
static inline t4Vector vu0MtxMul_C0E8(const t4Matrix* m, const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}


struct sLink_C0E8 {
    int next;               // 0x0
    char pad_0x4[0x8];
    t4Matrix* mat;         // 0xC

    // next at the end of a mesh's link chain (name ours): [U] buildXform multiplies link matrices until it reads -1.
    enum { kNoNext = -1 };
};

struct sLinkTbl_C0E8 {
    char pad_0x0[0x8];
    sLink_C0E8* links;      // 0x8
};

struct sModel_C0E8 {
    char pad_0x0[0x10];
    t4Matrix mat;          // 0x10
    char pad_0x50[0x30];
    sLinkTbl_C0E8* tbl;     // 0x80
    float scale;            // 0x84
};


extern char str_RailNode[];
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);

static inline sVecK_C0E8 mulVec_C0E8(const t4Vector& a, const t4Vector& b)
{
    return sVecK_C0E8(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

static inline t4Vector row_C0E8(const t4Matrix* m, int i)
{
    return m->row[i];
}

static inline t4Matrix scaled_C0E8(const t4Matrix* src, const t4Vector& s)
{
    t4Matrix r;
    r.row[0] = row_C0E8(src, 0);
    r.row[1] = row_C0E8(src, 1);
    r.row[2] = row_C0E8(src, 2);
    r.row[3] = mulVec_C0E8(row_C0E8(src, 3), s);
    return r;
}

/**
 * Builds a rail modifier's world-to-rail transform and creates its rail node.
 *
 * The rail's frame is the instance's world matrix or, for a mesh index (Mesh) of 0 or more, that mesh's
 * chain of link matrices (translations scaled by the instance scale) times the world matrix. The inverse
 * (transposed rotation, rotated negative translation) is stored in XForm. Then allocates a 0x10-byte
 * "RailNode" (cRailSpaceNode: type 2, Rail pointing back here) as Node.
 */
void cRailModifier::buildXform()
{
    sModel_C0E8* model = ((sModel_C0E8*)Instance);
    sLink_C0E8* links = model->tbl->links;
    t4Matrix* xform;
    if (Mesh < 0) {
        t4Matrix* selfMat = &XForm;
        vu0CopyMat_C0E8(selfMat, &model->mat);
        xform = selfMat;
    } else {
        float modelScale = model->scale;
        t4Vector scl;
        scl.x = modelScale;
        scl.y = modelScale;
        scl.z = modelScale;
        scl.w = 1.0f;
        t4Matrix* selfMat = &XForm;
        // links[k].mat / .next read straight off the subscript, k in a local: retail's index-first add, formed once
        int mesh = Mesh;
        vu0CopyMat_C0E8(selfMat, links[mesh].mat);
        xform = selfMat;
        {
        t4Vector rr;
        t4Vector origin = XForm.row[3];
        rr.x = origin.x * scl.x;
        rr.y = origin.y * scl.y;
        rr.z = origin.z * scl.z;
        rr.w = origin.w * scl.w;
        XForm.row[3] = rr;
        int i = links[mesh].next;
        while (i != sLink_C0E8::kNoNext) {
            t4Matrix* mm = xform;
            t4Matrix productCopy;
            t4Matrix linkMat;
            t4Vector res;
            t4Vector r3b;
            t4Matrix* lm = links[i].mat;
            res = lm->row[0];
            linkMat.row[0] = res;
            res = lm->row[1];
            linkMat.row[1] = res;
            res = lm->row[2];
            linkMat.row[2] = res;
            r3b = lm->row[3];
            res.x = r3b.x * scl.x;
            res.y = r3b.y * scl.y;
            res.z = r3b.z * scl.z;
            res.w = r3b.w * scl.w;
            linkMat.row[3] = res;
            t4Matrix product;
            t4Matrix* pt2 = &productCopy;
            t4Matrix* pt = &product;
            vu0MulMat_C0E8(pt, &linkMat, mm);
            vu0CopyMat_C0E8(pt2, pt);
            vu0CopyMat_C0E8(mm, pt2);
            i = links[i].next;
        }
        }
        t4Matrix worldCopy;
        t4Matrix worldProduct;
        t4Matrix* pt3 = &worldProduct;
        t4Matrix* pt4 = &worldCopy;
        vu0MulMat_C0E8(pt3, &((sModel_C0E8*)Instance)->mat, xform);
        vu0CopyMat_C0E8(pt4, &worldProduct);
        vu0CopyMat_C0E8(xform, pt4);
    }
    float swapValue;
    swapValue = XForm.row[0].y;
    XForm.row[0].y = XForm.row[1].x;
    XForm.row[1].x = swapValue;
    swapValue = XForm.row[0].z;
    XForm.row[0].z = XForm.row[2].x;
    XForm.row[2].x = swapValue;
    swapValue = XForm.row[1].z;
    XForm.row[1].z = XForm.row[2].y;
    XForm.row[2].y = swapValue;
    t4Vector invTranslation = vu0Scale(row_C0E8(&XForm, 3), -1.0f);
    invTranslation.w = 0.0f;
    invTranslation = vu0MtxMul_C0E8(xform, invTranslation);
    invTranslation.w = 1.0f;
    XForm.row[3] = invTranslation;
    cRailSpaceNode* n = (cRailSpaceNode*)cMemMan_alloc(0x10, str_RailNode, 0x20000000, 0);
    n->Rail = this;
    n->type = 2;
    Node = n;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_update);
#ifdef SKIP_ASM


extern "C" void cRailModifier_updateBoundBox(void* self);
extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, void* node, tBoundBox* box, tBoundBox* old);

/**
 * Recomputes the rail's world bound box and moves its rail-space node in the Nash octree from the old box to the
 * new one.
 *
 * Calls cRailModifier_updateBoundBox, then nash_Nash_movenode(Nash, 2 (the rail space), Node, new box, old box).
 */
extern "C" void cRailModifier_update(cRailModifier* self)
{
    tBoundBox* box = (&self->BoundBox);
    tBoundBox old = *(&self->BoundBox);
    cRailModifier_updateBoundBox((self));
    nash_Nash_movenode(cObjectInterface_getNash(), 2, self->Node, box, &old);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes the rail modifier to a replay frame: the 0x30-byte cRailModifierState (+0x10: BoundBox, SplineId, Mesh)
 * with slot 1, then Instance with slot 5.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
void cRailModifier::writeToReplayFrame(cBxReplayFrame* stream)
{
    stream->write((char*)&BoundBox, 0x30);
    stream->writeReplayInstance(Instance);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_getInvMat);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMatC5A0(t4Matrix* d, const t4Matrix* a, const t4Matrix* b)
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
static inline void vu0CopyMatC5A0(t4Matrix* d, const t4Matrix* s)
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

extern "C" void cInstanceNode_buildMeshMat(void* model, int bone, t4Matrix* out);

/**
 * Builds the rail's inverse transform: the instance's mesh matrix (cInstanceNode_buildMeshMat, cInstanceNode::buildMeshMat with
 * Instance and Mesh) multiplied with XForm (+0x50), into @p mat.
 *
 * @param mat  receives the matrix (On Tour t4Matrix& mat)
 */
void cRailModifier::getInvMat(t4Matrix* mat)
{
    cInstanceNode_buildMeshMat(Instance, Mesh, mat);
    t4Matrix* src = (&XForm);
    t4Matrix copy;
    t4Matrix product;
    vu0MulMatC5A0(&product, mat, src);
    vu0CopyMatC5A0(&copy, &product);
    vu0CopyMatC5A0(mat, &copy);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_splineIntersect);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// 4x4 float matrix indexed as m[row][col] (16-byte aligned; same bytes as t4Matrix).
struct sRmMat44 {
    float m[4][4];
} __attribute__((aligned(16)));



struct sRmSet_C698 {
    char pad_0x00[0x44];
    unsigned int* refs;     // 0x44
};

struct sRmWorld_C698 {
    char pad_0x00[0x8];
    sRmSet_C698** sets;     // 0x8
};

static inline void* refToPtr_C698(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* lookup_C698(sRmSet_C698* set, unsigned int idx)
{
    unsigned int p = set->refs[idx] >> 8;
    if (p == 0) {
        return 0;
    }
    return refToPtr_C698(p);
}

struct sRmRef_C698 {
    unsigned int id;

    void* get(sRmWorld_C698* w)
    {
        sRmSet_C698* set = w->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return lookup_C698(set, id >> 8);
    }
};



// A direction (w = 0); stores w first, like sVec4W in ai/rider.cpp.
struct sRmDir_C698 : t4Vector {
    sRmDir_C698(const float& ax, const float& ay, const float& az)
    {
        w = 0.0f;
        x = ax;
        y = ay;
        z = az;
    }
};

// Power basis (u^3, u^2, u, w) of a cubic parameter u, built up from u.
struct sRmPow_C698 : t4Vector {
    sRmPow_C698(const float& param, const float& aw)
    {
        w = aw;
        z = param;
        y = param * param;
        x = param * y;
    }
};

// Five floats (the golden-section start brackets for the five sample points).
struct sRmF5_C698 {
    float v[5];
};

extern "C" sRmWorld_C698** cObjectInterface_getWorld();
extern "C" void cRailModifier_getInvMat_C698(void* self, sRmMat44* m) __asm__("cRailModifier_getInvMat");
extern t4Vector t4vect_Zero_C698 __asm__("t4vect_Zero");
extern const sRmF5_C698 D_0048E7D8;
extern const sRmF5_C698 D_0048E7F0;


// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_C698(sRmMat44* d, const sRmMat44* a, const sRmMat44* b)
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
static inline void vu0CopyMat_C698(sRmMat44* d, const sRmMat44* s)
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

// PORT: PS2-only VU0 inline asm (vector * matrix, row-vector convention).
static inline t4Vector vu0MtxMul_C698(const sRmMat44* m, const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector vu0Sub_C698(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector vu0Add_C698(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (dot product).
static inline float vu0Dot_C698(const t4Vector& a, const t4Vector& b)
{
    float r;
    int bits;
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
        : "=f"(r), "=&r"(bits)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_C698(const t4Vector& v)
{
    float r;
    int bits;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(bits)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalised copy of a 4-vector).
static inline t4Vector vu0Normal_C698(const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2       $vf3, %1\n"
        "vaddw.x    $vf6, $vf0, $vf0w\n"
        "vmul.xyzw  $vf4, $vf3, $vf3\n"
        "vadday.x   ACC, $vf4, $vf4y\n"
        "vmaddaz.x  ACC, $vf6, $vf4z\n"
        "vmaddw.x   $vf4, $vf6, $vf4w\n"
        "vrsqrt     Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2       $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// Point on a cubic segment at parameter param.
static inline t4Vector evalSeg_C698(const sRmMat44* c, float param)
{
    sRmPow_C698 v(param, 1.0f);
    return vu0MtxMul_C698(c, v);
}

// PORT: SN abs.s asm helper (PS2 FPU).
static inline float fsAbs_C698(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Tangent of a cubic segment at parameter param.
static inline t4Vector evalTan_C698(const sRmMat44* c, float param)
{
    sRmDir_C698 v(param * (3.0f * param), 2.0f * param, 1.0f);
    return vu0MtxMul_C698(c, v);
}

static inline float clamp_C698(float x, float lo, float hi)
{
    return x >= lo ? x <? hi : lo;
}

static inline void resetHit_C698(tIntersectInfo* h)
{
    h->HitType = 2;
    h->Patch = 0;
    h->Inst = 0;
    h->Spline = 0;
    h->Mesh = -1;
    h->Face = -1;
    h->Seg = -1;
    h->Stype = -1;
    h->Ivel = t4vect_Zero_C698;
    h->Iangvel = t4vect_Zero_C698;
    h->T = 0;
    h->u = 0;
    h->v = 0;
}

/**
 * Finds the point of the rail's spline nearest to a position and records it as a hit when it beats the best so far.
 *
 * Skipped when the spline (SplineId, looked up in the world) shares no bit with @p mask. Each segment's cubic is
 * taken into world space with cRailModifier_getInvMat, sampled at t = 0, 0.25 .. 1 (distance to the nearest point on
 * each chord), and the bracket round the best sample (D_0048E7D8 / D_0048E7F0) is narrowed by up to 24 golden-section
 * steps (until it is within 0.0005). When nothing was found yet or the distance is below
 * @p bestDist, sets @p found and @p bestDist and fills @p out: position, tangent direction (normalised), spline id,
 * Instance, Mesh, t, the spline record and no object. @p box and @p box2 are never read.
 *
 * @param box  the probe's minimum corner (On Tour minBpos); unused
 * @param box2  the probe's maximum corner (On Tour maxBpos); unused
 * @param point  the position to match (On Tour bpos)
 * @param mask  the spline filter mask (On Tour splineFilterMask)
 * @param found  in/out: set to 1 when a hit is recorded (On Tour bool& hit)
 * @param bestDist  in/out: the best distance so far (On Tour float& dist)
 * @param out  receives the hit (On Tour tIntersectInfo* intInfo)
 */
void cRailModifier::splineIntersect(t4Vector* box, t4Vector* box2, const t4Vector* point, unsigned int mask,
                                    int* found, float* bestDist, tIntersectInfo* out)
{
    tSplineRecord* spline = (tSplineRecord*)(*(sRmRef_C698*)&SplineId).get(*cObjectInterface_getWorld());
    if ((((unsigned int)spline->m_iFlags) & mask) == 0) {
        return;
    }
    sRmMat44 mat;
    cRailModifier_getInvMat_C698((this), &mat);
    tSplineSegmentRecord* seg = spline->m_HeaderSeg;
    for (int i = 0; i < spline->m_iNumSegments; i++, seg = seg->m_NextSeg) {
        float tLo = 0.0f;
        float tHi = 1.0f;
        t4Vector pos;
        t4Vector next;
        t4Vector dir;
        sRmMat44 segMat;
        {
            sRmMat44 product;
            vu0MulMat_C698(&product, &mat, &(*(sRmMat44*)&seg->m_fmBasisMat));
            vu0CopyMat_C698(&segMat, &product);
        }
        pos = evalSeg_C698(&segMat, tLo);
        float best = vu0Length_C698(vu0Sub_C698(*point, pos));
        int bestIdx = 0;
        int k;
        for (k = 1; k < 5; k++) {
            next = evalSeg_C698(&segMat, k * 0.25f);
            dir = vu0Sub_C698(next, pos);
            float proj = vu0Dot_C698(vu0Sub_C698(*point, pos), dir) / vu0Dot_C698(dir, dir);
            dir = vu0Add_C698(pos, vu0Scale(dir, clamp_C698(proj, 0.0f, 1.0f)));
            float pointDist = vu0Length_C698(vu0Sub_C698(*point, dir));
            if (pointDist < best) {
                best = pointDist;
                bestIdx = k;
            }
            pos = next;
        }
        sRmF5_C698 lo = D_0048E7D8;
        sRmF5_C698 hi = D_0048E7F0;
        tLo = lo.v[bestIdx];
        tHi = hi.v[bestIdx];
        float tc = tLo + (tHi - tLo) * 0.381966f;
        float td = tLo + (tHi - tLo) * 0.618034f;
        // Golden-section search on [a, b]. f(a) and f(b) are evaluated but never read (the binary keeps both
        // evaluations; their lengths drop out).
        float fa, fb, fc, fd;
        pos = evalSeg_C698(&segMat, tLo);
        fa = vu0Length_C698(vu0Sub_C698(pos, *point));
        pos = evalSeg_C698(&segMat, tc);
        fc = vu0Length_C698(vu0Sub_C698(pos, *point));
        pos = evalSeg_C698(&segMat, td);
        fd = vu0Length_C698(vu0Sub_C698(pos, *point));
        float hitT = td;
        float dist = fd;
        pos = evalSeg_C698(&segMat, tHi);
        fb = vu0Length_C698(vu0Sub_C698(pos, *point));
        for (k = 0; k < 24; k++) {
            if (fsAbs_C698(tHi - tLo) <= 0.0005f) {
                break;
            }
            if (fd < fc) {
                tLo = tc;
                tc = td;
                td = tc * 0.618034f + tHi * 0.381966f;
                pos = evalSeg_C698(&segMat, td);
                float probeDist = vu0Length_C698(vu0Sub_C698(pos, *point));
                fc = fd;
                fd = probeDist;
                hitT = td;
                dist = fd;
            } else {
                tHi = td;
                td = tc;
                tc = td * 0.618034f + tLo * 0.381966f;
                pos = evalSeg_C698(&segMat, tc);
                float probeDist = vu0Length_C698(vu0Sub_C698(pos, *point));
                fd = fc;
                fc = probeDist;
                hitT = tc;
                dist = fc;
            }
        }
        if (*found == 0 || dist < *bestDist) {
            *found = 1;
            *bestDist = dist;
            resetHit_C698(out);
            out->Ipos = pos;
            out->Inorm = vu0Normal_C698(evalTan_C698(&segMat, hitT));
            out->Stype = spline->m_eSurfaceType;
            (*(void**)&out->Inst) = (void*)Instance;
            out->Mesh = Mesh;
            out->T = hitT;
            (*(void**)&out->Spline) = spline;
            out->Patch = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_render__FPv);
#ifdef SKIP_ASM
/**
 * The rail modifier's render virtual (cRailModifier_vtbl slot 3): an empty stub; rails draw nothing themselves.
 */
void cRailModifier::render()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_cRigidBody__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a rigid body: nothing to initialise, so it only returns the object.
 *
 * Run on a roller modifier's body (+0x30) by cRollerModifier_cRollerModifier and the replay constructor
 * cRollerModifier_cRollerModifier1.
 *
 * @return the body (self)
 */
void* cRigidBody_cRigidBody(void* vself)
{
    cRigidBody* self = (cRigidBody*)vself;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_setW);
#ifdef SKIP_ASM

// t3Matrix holds its nine floats row-major: m[3 * row + column].

static inline t3Matrix scaleCFF0(const t3Matrix& a, float s)
{
    t3Matrix r;
    r.m[0] = a.m[0] * s;
    r.m[1] = a.m[1] * s;
    r.m[2] = a.m[2] * s;
    r.m[3] = a.m[3] * s;
    r.m[4] = a.m[4] * s;
    r.m[5] = a.m[5] * s;
    r.m[6] = a.m[6] * s;
    r.m[7] = a.m[7] * s;
    r.m[8] = a.m[8] * s;
    return r;
}

static inline void invertCFF0(t3Matrix& a)
{
    t3Matrix adj;
    adj.m[0] = a.m[4] * a.m[8] - a.m[5] * a.m[7];
    adj.m[1] = a.m[7] * a.m[2] - a.m[8] * a.m[1];
    adj.m[2] = a.m[1] * a.m[5] - a.m[2] * a.m[4];
    adj.m[3] = a.m[6] * a.m[5] - a.m[8] * a.m[3];
    adj.m[4] = a.m[0] * a.m[8] - a.m[2] * a.m[6];
    adj.m[5] = a.m[3] * a.m[2] - a.m[5] * a.m[0];
    adj.m[6] = a.m[3] * a.m[7] - a.m[4] * a.m[6];
    adj.m[7] = a.m[6] * a.m[1] - a.m[7] * a.m[0];
    adj.m[8] = a.m[0] * a.m[4] - a.m[1] * a.m[3];
    float inv = a.m[0] * adj.m[0] + a.m[3] * adj.m[1] + a.m[6] * adj.m[2];
    inv = 1.0f / inv;
    a = scaleCFF0(adj, inv);
}

static inline t4Vector xformCFF0(const t3Matrix& m, cRigidBody* b)
{
    t4Vector r;
    float x = b->W.x;
    float y = b->W.y;
    float z = b->W.z;
    r.x = m.m[0] * x + m.m[3] * y + m.m[6] * z;
    r.y = m.m[1] * x + m.m[4] * y + m.m[7] * z;
    r.z = m.m[2] * x + m.m[5] * y + m.m[8] * z;
    r.w = b->W.w;
    return r;
}

/**
 * Sets a rigid body's angular velocity and recomputes its angular momentum to match.
 *
 * Stores newPos as W, inverts the inverse inertia InvMOI (3x3, by adjugate over determinant) and sets the angular
 * momentum L to that inertia times W.
 *
 * @param newPos  new angular velocity W
 */
void cRigidBody::setW(const t4Vector* newPos)
{
    W = *newPos;
    t3Matrix invRot = InvMOI;
    invertCFF0(invRot);
    L = xformCFF0(invRot, this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_updateImplicit);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0ScaleRM(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}


// InvMOI * L, keeping L's w.
static inline t4Vector xformRM(cRigidBody* b)
{
    t4Vector r;
    float x = b->L.x;
    float y = b->L.y;
    float z = b->L.z;
    r.x = b->InvMOI.m[0] * x + b->InvMOI.m[3] * y + b->InvMOI.m[6] * z;
    r.y = b->InvMOI.m[1] * x + b->InvMOI.m[4] * y + b->InvMOI.m[7] * z;
    r.z = b->InvMOI.m[2] * x + b->InvMOI.m[5] * y + b->InvMOI.m[8] * z;
    r.w = b->L.w;
    return r;
}

extern "C" void cRigidBody_updateImplicit(cRigidBody* self);

/**
 * Recomputes a rigid body's velocities from its momenta.
 *
 * V (+0x80) = linear momentum (+0x20) times the inverse mass (+0x40); W (+0x90) = the world inverse inertia matrix
 * (InvMOI, 3x3 at +0x50) applied to the angular momentum (+0x30), keeping its w. Called after the inertia changes
 * (cRollerModifier_updateMOI) and after each simulation step.
 */
void cRigidBody::updateImplicit()
{
    V = vu0ScaleRM(P, InvM);
    W = xformRM(this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_stepMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
#include "replay/bxreplayframe.h"


// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline void ScaleD340(t4Vector& out, const t4Vector& v, float s)
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
        : "m"(v), "f"(s)
        : "memory");
    out = r;
}

// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddEqD340(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (quaternion derivative 0.5 * (0, w) * q).
static inline void QuatDerivD340(t4Vector& out, const t4Vector& q, const t4Vector& w)
{
    t4Vector r;
    __asm__(
        "qmtc2.ni   %3, $vf1\n"
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vsuba.w    ACC, $vf0, $vf0\n"
        "vmsubx.w   $vf2, $vf0, $vf1x\n"
        "vmul.xyz   $vf7, $vf4, $vf5\n"
        "vmulw.xyz  $vf3, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vopmsub.xyz $vf6, $vf4, $vf5\n"
        "vmulax.w   ACC, $vf2, $vf7x\n"
        "vmadday.w  ACC, $vf2, $vf7y\n"
        "vmaddz.w   $vf8, $vf2, $vf7z\n"
        "vmulax.xyz ACC, $vf3, $vf1x\n"
        "vmaddx.xyz $vf8, $vf6, $vf1x\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(w), "r"(0x3F000000)
        : "memory");
    out = r;
}

// PORT: PS2-only VU0 inline asm (normalise a 4-vector in place).
static inline void NormalizeD340(t4Vector& v)
{
    __asm__(
        "lqc2       $vf3, %0\n"
        "vaddw.x    $vf6, $vf0, $vf0w\n"
        "vmul.xyzw  $vf4, $vf3, $vf3\n"
        "vadday.x   ACC, $vf4, $vf4y\n"
        "vmaddaz.x  ACC, $vf6, $vf4z\n"
        "vmaddw.x   $vf4, $vf6, $vf4w\n"
        "vrsqrt     Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2       $vf5, %0\n"
        : "+m"(v)
        :
        : "memory");
}

/**
 * Integrates a rigid body over one time step under a force and torque.
 *
 * Position (+0x0) += V (+0x80) * dt; the orientation quaternion (+0x10) += its derivative from W (+0x90) * dt and is
 * renormalised; the linear momentum (+0x20) += force * dt and the angular momentum (+0x30) += torque * dt.
 *
 * @param force  force applied over the step
 * @param torque  torque applied over the step
 * @param dt  time step
 */
void cRigidBody::stepMotion(const t4Vector* force, const t4Vector* torque, float dt)
{
    t4Vector step;
    ScaleD340(step, V, dt);
    AddEqD340(Pose.P, step);
    {
        t4Vector quatDeriv;
        QuatDerivD340(quatDeriv, Pose.Q, W);
        ScaleD340(step, quatDeriv, dt);
    }
    Pose.Q.x += step.x;
    Pose.Q.y += step.y;
    Pose.Q.z += step.z;
    Pose.Q.w += step.w;
    NormalizeD340(Pose.Q);
    ScaleD340(step, *force, dt);
    AddEqD340(P, step);
    ScaleD340(step, *torque, dt);
    AddEqD340(L, step);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_rigidImpact);
#ifdef SKIP_ASM
extern "C" void func_0035D908(cRollerModifier* b, const t4Vector* r, const t4Vector* imp);

struct sRmRigid_D4A0 {
    t4Vector pos;        // 0x00
    char pad_0x10[0x30];
    float invMass;      // 0x40
    char pad_0x44[0xC];
    t3Matrix invI;      // 0x50
    char pad_0x74[0xC];
    t4Vector vel;        // 0x80
    t4Vector angVel;     // 0x90
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector SubD4A0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector AddD4A0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector CrossD4A0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (dot product).
static inline float DotD4A0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float LengthD4A0(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline t4Vector DivD4A0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector ScaleD4A0(const t4Vector& v, float s)
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
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline t4Vector MulID4A0(sRmRigid_D4A0* b, const t4Vector& v)
{
    t4Vector r;
    float x = v.x;
    float y = v.y;
    float z = v.z;
    r.x = b->invI.m[0] * x + b->invI.m[3] * y + b->invI.m[6] * z;
    r.y = b->invI.m[1] * x + b->invI.m[4] * y + b->invI.m[7] * z;
    r.z = b->invI.m[2] * x + b->invI.m[5] * y + b->invI.m[8] * z;
    r.w = v.w;
    return r;
}

static inline t4Vector PointVelD4A0(sRmRigid_D4A0* b, const t4Vector& p)
{
    return AddD4A0(b->vel, CrossD4A0(b->angVel, SubD4A0(p, b->pos)));
}

/**
 * Resolves a rigid body's contact with a surface by friction and bounce impulses.
 *
 * With nonzero mu and a tangential point velocity above 0.001, applies a friction impulse against the sliding direction
 * (func_0035D908) and stops if the point now separates. Then applies the normal impulse -(1 + restitution) * normal
 * speed / (inverse mass + angular term) along n (func_0035D908).
 *
 * @param body  the rigid body
 * @param point  contact point
 * @param n  contact normal
 * @param restitution  elasticity
 * @param mu  friction coefficient
 */
extern "C" void cRigidBody_rigidImpact(sRmRigid_D4A0* body, t4Vector* point, t4Vector* n, float restitution, float mu)
{
    t4Vector arm = SubD4A0(*point, body->pos);
    t4Vector vp = PointVelD4A0(body, *point);
    float normalVel = DotD4A0(vp, *n);
    if (mu != 0.0f) {
        t4Vector tangentVel = SubD4A0(vp, ScaleD4A0(*n, normalVel));
        float len = LengthD4A0(tangentVel);
        if (len > 0.0010000000474974513f) {
            t4Vector dir = DivD4A0(tangentVel, len);
            float j = -mu * len / (body->invMass + DotD4A0(dir, CrossD4A0(MulID4A0(body, CrossD4A0(arm, dir)), arm)));
            t4Vector imp = ScaleD4A0(dir, j);
            func_0035D908((cRollerModifier*)body, &arm, &imp);
            vp = PointVelD4A0(body, *point);
            normalVel = DotD4A0(vp, *n);
            if (normalVel > 0.0f) {
                return;
            }
        }
    }
    float j = -(restitution + 1.0f) * normalVel / (body->invMass + DotD4A0(*n, CrossD4A0(MulID4A0(body, CrossD4A0(arm, *n)), arm)));
    t4Vector imp = ScaleD4A0(*n, j);
    func_0035D908((cRollerModifier*)body, &arm, &imp);
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035D908);

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cRigidBody_writeToReplayFrame(cRigidBody* self, cBxReplayFrame* obj);

/**
 * Writes a rigid body's state (its first 0x50 bytes: pose, momentum, inverse mass) to a replay frame.
 *
 * @param obj  the replay frame (On Tour cBxReplayFrame*)
 */
void cRigidBody::writeToReplayFrame(cBxReplayFrame* obj)
{
    obj->write((const char*)this, 80);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Restores a rigid body's state from a replay frame.
 *
 * Reads 0x50 bytes (the momenta, pose and mass fields) straight into the body through the frame's virtual read
 * (slot 2). The counterpart of cRigidBody_writeToReplayFrame.
 *
 * @param obj  the replay frame to read from (On Tour data)
 */
void cRigidBody::readFromReplayFrame(cBxReplayFrame* obj)
{
    obj->read((char*)this, 80);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_cRollerModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
extern void* cRollerModifier_vtbl[];
void* cRigidBody_cRigidBody(void* self);
extern "C" void __10SphereTree(void* p);
extern "C" void* bxSphereTree_operatorE(void* self, void* src);
extern "C" void cRollerModifier_updateMOI(cRollerModifier* self);

extern "C" void* cRollerModifier_Collision_DA70(void* self, void* a1, void* a2) __asm__("cRollerModifier_Collision");


extern "C" tPose tPose_tPose(void* m);
extern "C" tPose cRollerModifier_getBodyPose_DA70(void* self) __asm__("cRollerModifier_getBodyPose");

struct sV4c_DA70 {
    float x, y, z, w;
    sV4c_DA70(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline t4Vector QuatRot_DA70(const t4Vector& q, const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (dst = v rotated by quaternion q).
static inline void QuatRotTo_DA70(t4Vector& dst, const t4Vector& q, const t4Vector& v)
{
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(dst)
        : "m"(q), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddTo_DA70(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (unit quaternion + position -> 4x4 matrix).
static inline void QuatPosToMtx_DA70(void* m, const t4Vector& q, const t4Vector& p)
{
    __asm__ __volatile__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf3, %2\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf3, 0x30(%0)\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "sqc2       $vf11, 0x10(%0)\n"
        "sqc2       $vf12, 0x20(%0)\n"
        :
        : "r"(m), "m"(q), "m"(p)
        : "memory");
}

struct sFlags_DA70 {
    char pad0[0x8];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
};

static inline void SetBits_DA70(unsigned int* w, unsigned int mask, unsigned int bits)
{
    *w = (*w & mask) | bits;
}

/**
 * Constructs a roller modifier: a rigid body that sends a hit object rolling.
 *
 * Takes the instance from the hit, copies its sphere tree, mass, elasticity, centre-of-mass offset and
 * inverse inertia (halved); starts at the instance's pose moved to the centre of mass; sets the contact
 * constants (elasticity 0.1, friction 0.4, stop speed 277.78, no drag), updates the instance's flag bits and
 * gives it its first kick with cRollerModifier_Collision().
 *
 * @param rdata  the roller data (On Tour tAddRollerData): mass at +0x4, elasticity at +0x8
 * @param ii     the hit (On Tour tIntersectInfo*): sphere tree at +0x48, instance at +0x50
 * @param ci     the collision (On Tour tCollideInfo*)
 * @return self
 */
extern "C" char* cRollerModifier_cRollerModifier(cRollerModifier* self, tAddRollerData* rdata, tIntersectInfo* ii, char* ci)
{
    float* timing = (float*)(&self->mKineticEnergy);
    *(void***)self = cTransformModifier_vtbl;
    cRigidBody_cRigidBody(&self->Pose);
    *(void***)self = cRollerModifier_vtbl;
    __10SphereTree(&self->SphTree);
    self->Life = 0.0f;
    self->m_pInstance = ii->Inst;
    bxSphereTree_operatorE(&self->SphTree, ii->SphTree);
    self->mMass = rdata->Mass;
    tSphTreeInfo* info = self->SphTree.treeInfo;
    self->mElast = rdata->Elast;
    self->InvM = 1.0f / timing[1];
    t4Vector offset;
    offset.x = info->centerOfMass[0];
    offset.y = info->centerOfMass[1];
    offset.z = info->centerOfMass[2];
    offset.w = 0.0f;
    self->COMOffset = offset;
    self->InvBodyMOI = info->invMomentOfInertia;
    self->InvBodyMOI.m[0] *= 0.5f;
    self->InvBodyMOI.m[1] *= 0.5f;
    self->InvBodyMOI.m[2] *= 0.5f;
    self->InvBodyMOI.m[3] *= 0.5f;
    self->InvBodyMOI.m[4] *= 0.5f;
    self->InvBodyMOI.m[5] *= 0.5f;
    self->InvBodyMOI.m[6] *= 0.5f;
    self->InvBodyMOI.m[7] *= 0.5f;
    self->InvBodyMOI.m[8] *= 0.5f;
    tPose x = tPose_tPose(&self->m_pInstance->m_fmTransform);
    {
        t4Vector rotated;
        QuatRotTo_DA70(rotated, x.Q, self->COMOffset);
        offset = rotated;
        t4Vector shift;
        t4Vector rotatedAgain;
        QuatRotTo_DA70(rotatedAgain, x.Q, offset);
        shift = rotatedAgain;
        AddTo_DA70(x.P, shift);
    }
    *(&self->Pose) = x;
    tPose y = cRollerModifier_getBodyPose_DA70(((char*)self));
    QuatPosToMtx_DA70(&self->Transform, y.Q, y.P);
    self->updateMOI();
    cRigidBody_updateImplicit((cRigidBody*)&self->Pose);
    self->ContactElastic = 0.10000000149011612f;
    self->ContactFriction = 0.4000000059604645f;
    self->StopContactSpeed = 277.77777099609375f;
    self->ViscousDrag = 0.0f;
    SetBits_DA70((unsigned int*)(&self->m_pInstance->m_iFlags), 0xFFFFFFDFu, 0x40);
    SetBits_DA70((unsigned int*)(&self->m_pInstance->m_iFlags), 0xFFFFFFFDu, 0x4);
    self->m_pInstance->m_iFlags |= 0x1;
    cRollerModifier_Collision_DA70(((char*)self), ci, ii);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_Collision);
#ifdef SKIP_ASM


// PORT: PS2-only VU0 inline asm (a + b), by value.
static inline t4Vector vu0AddR_35DDE8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s), by value.
static inline t4Vector vu0DivR_35DDE8(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}


// PORT: PS2-only VU0 inline asm (v * s), by value.
static inline t4Vector vu0ScaleR_35DDE8(const t4Vector& v, float s)
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
        : "m"(v), "f"(s)
        : "memory");
    return r;
}
// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_35DDE8(const t4Vector& a, const t4Vector& b)
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

// A t4Vector built from its four components.
struct t4VectorInit : public t4Vector {
    t4VectorInit(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

struct cRigidBody_DDE8 {
    char pad_0x00[0x20];
    t4Vector P;         // 0x20
    t4Vector L;         // 0x30
    float InvM;             // 0x40
    char pad_0x44[0x3C];
    t4Vector V;         // 0x80
    t4Vector W;         // 0x90
    float getInvM() const { return InvM; }
    void setW(const t4Vector& w) __asm__("cRigidBody_setW");
};

struct cTransformModifier_DDE8 {
    void* vptr;             // 0x00
};

struct cRollerNodeState_DDE8 {
    float mKineticEnergy;   // 0x04 in cRollerModifier
    float mMass;            // 0x08
    float mElast;           // 0x0C
    float ViscousDrag;      // 0x10
    float ContactElastic;   // 0x14
    float ContactFriction;  // 0x18
    float StopContactSpeed; // 0x1C
    float Life;             // 0x20
    float Radius;           // 0x24
    float HitFrequency;     // 0x28
};

// On Tour (debug DWARF): cRollerModifier : cTransformModifier (+0), cRollerNodeState (+4), cRigidBody (+0x30).
struct cRollerModifier_DDE8 : public cTransformModifier_DDE8, public cRollerNodeState_DDE8, public cRigidBody_DDE8 {
    // On Tour has this as cRigidBody::setV; in SSX 3 the P/V stores are this-relative while InvM goes
    // through the cRigidBody base pointer, so here it lives in the derived class.
    void setV(const t4Vector& v)
    {
        P = vu0DivR_35DDE8(v, getInvM());
        V = v;
    }
    void Collision(const tCollideInfo* tc) __asm__("cRollerModifier_Collision");
};

/**
 * Sets a roller's motion from a collision: a push along the hit direction plus an upward kick and a spin.
 *
 * Zeroes HitFrequency, sets V (and momentum P) to tc->dir * tc->len / 2 plus z = min(5000 / mMass + 100, 600),
 * so lighter bodies get a bigger kick, sets W (and L) to tc->up * 10, then stores mKineticEnergy = V.P + W.L.
 * On Tour's Collision also takes a tIntersectInfo*, which this body never reads.
 *
 * @param tc  collision info (On Tour tCollideInfo): direction at +0x10, up vector at +0x20, length at +0x30
 */
// PORT: the unit declares cRollerModifier_Collision(void*) for its caller; this is the method (On Tour: cRollerModifier::Collision).
void cRollerModifier_DDE8::Collision(const tCollideInfo* tc)
{
    float addZ = 5000.0f / mMass + 100.0f;
    HitFrequency = 0.0f;
    if (addZ > 600.0f) {
        addZ = 600.0f;
    }
    setV(vu0AddR_35DDE8(vu0DivR_35DDE8(vu0ScaleR_35DDE8(tc->Direct, tc->Vel), 2.0f),
                        t4VectorInit(0.0f, 0.0f, addZ, 0.0f)));
    setW(vu0ScaleR_35DDE8(tc->Reflect, 10.0f));
    mKineticEnergy = vu0Dot_35DDE8(V, P) + vu0Dot_35DDE8(W, L);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_cRollerModifier1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* cTransformModifier_vtbl[];
extern void* cRollerModifier_vtbl[];
void* cRigidBody_cRigidBody(void* self);
extern "C" void __10SphereTree(void* self);
extern void* cObjectInterface_getWorld_DF70() __asm__("cObjectInterface_getWorld");
void* cWorld_getPolyModelInfo(void* a, void* b);
extern "C" void* tSphTreeList_getMeshTree(void* a, int b);
extern "C" void SphereTree_TransformAbs(void* self, void* m, float scale);
extern "C" void cRollerModifier_updateMOI(cRollerModifier* self);




struct sRmSrcDF70 {
    char pad_0x00[0x2C];
    float x, y, z;          // 0x2C
    char pad_0x38[0x5C - 0x38];
    t3Matrix rot;       // 0x5C
};


/**
 * Constructs a roller modifier from a replay frame.
 *
 * Builds the bases (cTransformModifier_vtbl, then cRigidBody_cRigidBody and cRollerModifier_vtbl), constructs the
 * sphere tree, reads the rigid body (cRigidBody_readFromReplayFrame), 40 bytes of roller data and the instance through
 * the stream, attaches the instance's mesh sphere tree, restores the centre-of-mass offset and the halved body inertia
 * from the instance data, rebuilds Transform from the body pose, transforms the sphere tree and recomputes the inertia
 * (cRollerModifier_updateMOI, cRigidBody_updateImplicit).
 *
 * @param stream  replay stream to read from
 * @return self
 */
cRollerModifier* cRollerModifier::cRollerModifier1(cBxReplayFrame* stream)
{
    VPTR_AT(this, 0) = cTransformModifier_vtbl;
    cRigidBody_cRigidBody(((char*)&Pose));
    VPTR_AT(this, 0) = cRollerModifier_vtbl;
    __10SphereTree(((char*)&SphTree));
    ((cRigidBody*)(((char*)&Pose)))->readFromReplayFrame(stream);
    stream->read((char*)&mKineticEnergy, 40);
    m_pInstance = stream->readReplayInstance();
    void* meshTree = tSphTreeList_getMeshTree(cWorld_getPolyModelInfo(cObjectInterface_getWorld_DF70(),
            m_pInstance), 0);
    bxSphereTree_operatorE(((char*)&SphTree), meshTree);
    sRmSrcDF70* src = ((sRmSrcDF70*)SphTree.treeInfo);
    t4Vector off;
    off.x = src->x;
    off.y = src->y;
    off.z = src->z;
    off.w = 0.0f;
    COMOffset = off;
    InvBodyMOI = src->rot;
    InvBodyMOI.m[0] *= 0.5f;
    InvBodyMOI.m[1] *= 0.5f;
    InvBodyMOI.m[2] *= 0.5f;
    InvBodyMOI.m[3] *= 0.5f;
    InvBodyMOI.m[4] *= 0.5f;
    InvBodyMOI.m[5] *= 0.5f;
    InvBodyMOI.m[6] *= 0.5f;
    InvBodyMOI.m[7] *= 0.5f;
    InvBodyMOI.m[8] *= 0.5f;
    tPose x = cRollerModifier_getBodyPose_DA70(this);
    QuatPosToMtx_DA70(&Transform, x.Q, x.P);
    SphereTree_TransformAbs(((char*)&SphTree), &Transform, 1.0f);
    updateMOI();
    cRigidBody_updateImplicit((cRigidBody*)&Pose);
    return this;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", cRollerModifier_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes the roller modifier to a replay frame: its rigid body (+0x30), the 0x28-byte cRollerNodeState (+0x4) and
 * Instance (+0xD0).
 *
 * The same order as On Tour's writeToReplayFrame (cRigidBody first, then the state, then the instance pointer with
 * slot 5).
 *
 * @param obj  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
void cRollerModifier::writeToReplayFrame(cBxReplayFrame* obj)
{
    cRigidBody_writeToReplayFrame((cRigidBody*)(&Pose), obj);
    obj->write((const char*)&mKineticEnergy, 40);
    obj->writeReplayInstance((const tInstanceRecord*)m_pInstance);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_updateMOI);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (unit quaternion -> 4x4 rotation matrix, row 3 = (0,0,0,1)).
static inline void QuatToMtx_E248(t4Matrix& m, const t4Vector& q)
{
    __asm__ __volatile__(
        "lqc2       $vf4, %1\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf0, 0x30(%0)\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "sqc2       $vf11, 0x10(%0)\n"
        "sqc2       $vf12, 0x20(%0)\n"
        :
        : "r"(&m), "m"(q)
        : "memory");
}

// The 3x3 rotation part of a 4x4 matrix.
static inline t3Matrix Get33_E248(const t4Matrix& m)
{
    t3Matrix r;
    r.m[0] = ((float (*)[4])&m.row[0])[0][0];
    r.m[1] = ((float (*)[4])&m.row[0])[0][1];
    r.m[2] = ((float (*)[4])&m.row[0])[0][2];
    r.m[3] = ((float (*)[4])&m.row[0])[1][0];
    r.m[4] = ((float (*)[4])&m.row[0])[1][1];
    r.m[5] = ((float (*)[4])&m.row[0])[1][2];
    r.m[6] = ((float (*)[4])&m.row[0])[2][0];
    r.m[7] = ((float (*)[4])&m.row[0])[2][1];
    r.m[8] = ((float (*)[4])&m.row[0])[2][2];
    return r;
}

static inline void Transpose33_E248(t3Matrix& m)
{
    float t;
    t = m.m[1]; m.m[1] = m.m[3]; m.m[3] = t;
    t = m.m[2]; m.m[2] = m.m[6]; m.m[6] = t;
    t = m.m[5]; m.m[5] = m.m[7]; m.m[7] = t;
}

// r = b * a (a applied first).
static inline t3Matrix Mul33_E248(const t3Matrix& a, const t3Matrix& b)
{
    t3Matrix r;
    r.m[0] = a.m[0] * b.m[0] + a.m[3] * b.m[1] + a.m[6] * b.m[2];
    r.m[3] = a.m[0] * b.m[3] + a.m[3] * b.m[4] + a.m[6] * b.m[5];
    r.m[6] = a.m[0] * b.m[6] + a.m[3] * b.m[7] + a.m[6] * b.m[8];
    r.m[1] = a.m[1] * b.m[0] + a.m[4] * b.m[1] + a.m[7] * b.m[2];
    r.m[4] = a.m[1] * b.m[3] + a.m[4] * b.m[4] + a.m[7] * b.m[5];
    r.m[7] = a.m[1] * b.m[6] + a.m[4] * b.m[7] + a.m[7] * b.m[8];
    r.m[2] = a.m[2] * b.m[0] + a.m[5] * b.m[1] + a.m[8] * b.m[2];
    r.m[5] = a.m[2] * b.m[3] + a.m[5] * b.m[4] + a.m[8] * b.m[5];
    r.m[8] = a.m[2] * b.m[6] + a.m[5] * b.m[7] + a.m[8] * b.m[8];
    return r;
}


// r = I * a, with I the body inertia (passed through its owner so each element is an offset from it).
static inline t3Matrix MulI_E248(const t3Matrix& a, const cRollerModifier* body)
{
    t3Matrix r;
    r.m[0] = a.m[0] * body->InvBodyMOI.m[0] + a.m[3] * body->InvBodyMOI.m[1] + a.m[6] * body->InvBodyMOI.m[2];
    r.m[3] = a.m[0] * body->InvBodyMOI.m[3] + a.m[3] * body->InvBodyMOI.m[4] + a.m[6] * body->InvBodyMOI.m[5];
    r.m[6] = a.m[0] * body->InvBodyMOI.m[6] + a.m[3] * body->InvBodyMOI.m[7] + a.m[6] * body->InvBodyMOI.m[8];
    r.m[1] = a.m[1] * body->InvBodyMOI.m[0] + a.m[4] * body->InvBodyMOI.m[1] + a.m[7] * body->InvBodyMOI.m[2];
    r.m[4] = a.m[1] * body->InvBodyMOI.m[3] + a.m[4] * body->InvBodyMOI.m[4] + a.m[7] * body->InvBodyMOI.m[5];
    r.m[7] = a.m[1] * body->InvBodyMOI.m[6] + a.m[4] * body->InvBodyMOI.m[7] + a.m[7] * body->InvBodyMOI.m[8];
    r.m[2] = a.m[2] * body->InvBodyMOI.m[0] + a.m[5] * body->InvBodyMOI.m[1] + a.m[8] * body->InvBodyMOI.m[2];
    r.m[5] = a.m[2] * body->InvBodyMOI.m[3] + a.m[5] * body->InvBodyMOI.m[4] + a.m[8] * body->InvBodyMOI.m[5];
    r.m[8] = a.m[2] * body->InvBodyMOI.m[6] + a.m[5] * body->InvBodyMOI.m[7] + a.m[8] * body->InvBodyMOI.m[8];
    return r;
}

/**
 * Rotates the roller's body-space inverse inertia into world space for its rigid body.
 *
 * Turns the body's orientation quaternion (+0x40) into a rotation R and stores R * InvBodyMOI * R^T (InvBodyMOI at
 * +0x280) as the rigid body's InvMOI (+0x80), ready for cRigidBody_updateImplicit.
 */
void cRollerModifier::updateMOI()
{
    t4Matrix rotMat4;
    QuatToMtx_E248(rotMat4, Pose.Q);
    t3Matrix rot;
    t3Matrix rotT;
    rotT = Get33_E248(rotMat4);
    rot = rotT;
    Transpose33_E248(rotT);
    InvMOI = Mul33_E248(MulI_E248(rot, this), rotT);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_getBodyPose);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (dst = v * s).
static inline void vu0ScaleToRM(t4Vector& dst, const t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst), "=&r"(t)
        : "m"(v), "f"(s));
}

// PORT: PS2-only VU0 inline asm (dst = v rotated by quaternion q).
static inline void vu0QuatRotToRM(t4Vector& dst, const t4Vector& q, const t4Vector& v)
{
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(dst)
        : "m"(q), "m"(v));
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void vu0AddToRM(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

/**
 * Returns the pose of the roller's model: the rigid body's pose shifted back by the centre-of-mass offset.
 *
 * Copies the body's pose (+0x30: position, orientation quaternion at +0x40), rotates the centre-of-mass offset
 * (+0x2B0) by the orientation, negates it, rotates that by the pose's orientation again and adds it to the
 * position. On Tour does the same two rotations.
 *
 * @return the pose (position and orientation quaternion)
 */
extern "C" tPose cRollerModifier_getBodyPose(cRollerModifier* self)
{
    tPose pose = self->Pose;
    t4Vector negatedCom;
    t4Vector rotatedCom;
    t4Vector comOffset;
    t4Vector posDelta;
    t4Vector poseOffset;
    vu0QuatRotToRM(comOffset, self->Pose.Q, self->COMOffset);
    rotatedCom = comOffset;
    vu0ScaleToRM(comOffset, rotatedCom, -1.0f);
    negatedCom = comOffset;
    vu0QuatRotToRM(poseOffset, pose.Q, negatedCom);
    posDelta = poseOffset;
    vu0AddToRM(pose.P, posDelta);
    return pose;
}
#endif

INCLUDE_ASM("object/railmodifier", cRollerModifier_update);

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_updateRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the roller modifier's updateRootTransform has an empty body.
 */
void cRollerModifier::updateRootTransform()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_translateMe);
#ifdef SKIP_ASM
extern "C" void SphereTree_MoveOffset(void* p, const t4Vector* d);

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void vu0AddED90(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

/**
 * Moves the roller by an offset: its rigid body's position and its collision sphere tree.
 *
 * Adds @p off to the body position (+0x30) and shifts the sphere tree (+0xE0) by the same amount with
 * SphereTree_MoveOffset. cRollerModifier_collide uses it to push the roller out of what it hit.
 *
 * @param off  the offset (On Tour off)
 */
void cRollerModifier::translateMe(const t4Vector* off)
{
    vu0AddED90(Pose.P, *off);
    SphereTree_MoveOffset(&SphTree, off);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_collide);
#ifdef SKIP_ASM
extern "C" void* cWorldSphTree_cWorldSphTree(void* self, void* a1, int a2);
extern "C" void cWorldSphTree__cWorldSphTree(void* self, int flags);
extern "C" void* cObjectInterface_getNash();
extern "C" float SphereTree_GetResolution(void* self);
extern void cRigidBody_rigidImpact_EDC8(void* self, void* hit, void* normal, float a, float b) __asm__("cRigidBody_rigidImpact");

struct sHitEDC8 {
    t4Vector pos;        // 0x00
    t4Vector normal;     // 0x10
    float pad20[8];
    float dist;         // 0x40
    int pad44[3];
    char* obj;          // 0x50
    char* tri;          // 0x54
    int pad58[10];
    sHitEDC8() {}
} __attribute__((aligned(16)));

extern "C" float cBXNash_terrainIntersect1_EDC8(void* world, void* tree, sHitEDC8* out, void* a3) __asm__("cBXNash_terrainIntersect1");

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector vu0ScaleEDC8(const t4Vector& v, float s)
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
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline t4Vector vu0DivEDC8(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector vu0SubEDC8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector vu0AddEDC8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a x b, w = 0).
static inline t4Vector vu0CrossEDC8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0DotEDC8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0LengthEDC8(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}


// The momentum of a velocity: v / InvM.
static inline t4Vector toDirEDC8(cRigidBody* b, const t4Vector& v)
{
    return vu0DivEDC8(v, b->InvM);
}

static inline t4Vector xformEDC8(cRollerModifier* rollerModifier)
{
    t4Vector o;
    float x = rollerModifier->L.x;
    float y = rollerModifier->L.y;
    float z = rollerModifier->L.z;
    o.x = rollerModifier->InvMOI.m[0] * x + rollerModifier->InvMOI.m[3] * y + rollerModifier->InvMOI.m[6] * z;
    o.y = rollerModifier->InvMOI.m[1] * x + rollerModifier->InvMOI.m[4] * y + rollerModifier->InvMOI.m[7] * z;
    o.z = rollerModifier->InvMOI.m[2] * x + rollerModifier->InvMOI.m[5] * y + rollerModifier->InvMOI.m[8] * z;
    o.w = rollerModifier->L.w;
    return o;
}

static inline void setPosEDC8(cRollerModifier* rollerModifier, const t4Vector& v)
{
    rollerModifier->L = v;
    rollerModifier->W = xformEDC8(rollerModifier);
}

static inline void setOutDirEDC8(cRollerModifier* rollerModifier, const t4Vector& v)
{
    rollerModifier->P = toDirEDC8((cRigidBody*)&rollerModifier->Pose, v);
    rollerModifier->V = v;
}

/**
 * Resolves a roller's contact with the world for this frame.
 *
 * Queries the Nash grid with the roller's sphere tree. If it penetrates, it is pushed out along the normal
 * (depth + 2) and its linear and angular motion is damped to 97.5 %. If the contact point is moving into the
 * surface, a contact impulse is applied (inelastic below the stop speed), HitFrequency grows by 1/60 and a
 * Life above 5 is set to 10.
 */
void cRollerModifier::collide(const tCollideInfo*, const tIntersectInfo*)
{
    sHitEDC8 hit;
    char tree[0x70];
    cWorldSphTree_cWorldSphTree(tree, (char*)&SphTree, 1);
    float zeroF = 0.0f;
    float overlap = cBXNash_terrainIntersect1_EDC8(cObjectInterface_getNash(), tree, &hit, (char*)&RollerHint);
    overlap -= SphereTree_GetResolution((char*)&SphTree);
    if (zeroF < overlap) {
        t4Vector push = vu0ScaleEDC8(hit.normal, overlap + 2.0f);
        translateMe(&push);
        float k = 0.975f;
        setPosEDC8((this), vu0ScaleEDC8(L, k));
        setOutDirEDC8((this), vu0ScaleEDC8(V, k));
        float dp = vu0DotEDC8(vu0AddEDC8(V, vu0CrossEDC8(W, vu0SubEDC8(hit.pos, Pose.P))),
                hit.normal);
        if (dp < zeroF) {
            float lim = StopContactSpeed;
            float len = vu0LengthEDC8(V);
            float elastic = ContactElastic;
            float friction = ContactFriction;
            if (len < lim)
                elastic = zeroF;
            cRigidBody_rigidImpact_EDC8((cRollerModifier*)&Pose, &hit, &hit.normal, elastic, friction);
            HitFrequency += 0.016666668f;
            if (5.0f < Life)
                Life = 10.0f;
        }
    }
    cWorldSphTree__cWorldSphTree(tree, 2);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", __16cTexFlipModifierPvP15tInstanceRecord);
#ifdef SKIP_ASM
int BXrand();
extern "C" void cTexFlipModifier_setupTexFlip(cTexFlipModifier* self);


// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ftrunc_0035F0B8(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

/**
 * Constructs a texture-flip modifier for an instance from tex-flip make-data.
 *
 * Stores Instance, zeroes TexIndex, NumMaterials, NumTexFlipMaterials and Time, takes Type (+0x4) and Dir (+0xC) and
 * sets Speed and BaseSpeed to Speed (+0x10) / frame rate. A negative StartId (+0x8) picks a random start index (ceil of
 * 0..8), otherwise TexIndex = StartId. Ends with cTexFlipModifier_setupTexFlip.
 *
 * @param def  tex-flip make-data (tAddTexFlipData)
 * @param owner  instance record
 */
cTexFlipModifier::cTexFlipModifier(void* def, tInstanceRecord* owner)
{
    tAddTexFlipData* defData = (tAddTexFlipData*)def;
    float one = 1.0f;
    Instance = (tInstanceRecord*)owner;
    TexIndex = 0;
    NumMaterials = 0;
    NumTexFlipMaterials = 0;
    Type = defData->Type;
    float rate = defData->Speed * (one / (float)cAppMan_TheAppMan->TimerHz);
    Speed = rate;
    Dir = defData->Dir;
    BaseSpeed = rate;
    Time = 0.0f;
    int n = defData->StartId;
    if ((float)n < *(float*)(int*)&Time) {
        union { int i; float f; } randBits;
        randBits.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
        float x = (randBits.f - one) * 8.0f;
        float rounded = ftrunc_0035F0B8(x);
        if (rounded < x) {
            rounded += one;
        }
        TexIndex = (int)rounded;
    } else {
        TexIndex = n;
    }
    setupTexFlip();
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_cTexFlipModifier1);
#ifdef SKIP_ASM
extern "C" void cTexFlipModifier_setupTexFlip(cTexFlipModifier* self);

/**
 * Constructs a texture-flip modifier from a replay frame: reads the 0x18-byte state and Instance, then
 * cTexFlipModifier_setupTexFlip.
 *
 * @param obj  replay stream to read from
 * @return self
 */
void* cTexFlipModifier::cTexFlipModifier1(cBxReplayFrame* obj)
{
    obj->read((char*)this, 0x18);
    Instance = (tInstanceRecord*)(void*)obj->readReplayInstance();
    setupTexFlip();
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_setupTexFlip);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sRailItemF218 {
    char pad_0x0[0xC];
    int fC;                 // 0xC
    char* data;             // 0x10
};

struct sRailOutF218 {
    unsigned short value;   // 0x0
    short slot[3];          // 0x2
    int f8;
    int fC;
    int f10;
};

struct sRailSelF218 {
    char pad_0x0[0x14];
    int index;              // 0x14
    char* owner;            // 0x18
    int count;              // 0x1c
    int numTexFlipMaterials;    // 0x20 [O] NumTexFlipMaterials (the flip table's texture count)
    int blockNumMaterials;      // 0x24 [O] MaterialBlock (its m_iNumMaterials)
    sRailOutF218* src[8];   // 0x28
    sRailOutF218 out[1];    // 0x48
};

extern void sel_F218(sRailSelF218* self, int i) __asm__("cTexFlipModifier_setTexIndex");
extern void ms_F218(void* p, int v, int n) __asm__("MEM_fill");

/**
 * Sets a texture-flip modifier up from its instance's materials.
 *
 * NumMaterials (+0x1C) comes from the material list of the Instance (+0x18; list at +0x94); the texture count (+0x20)
 * from
 * the first material that has a flip table (+0x10). Selects the current texture with cTexFlipModifier_setTexIndex(TexIndex +0x14),
 * clears the material block (+0x24, 0xC4 bytes) and fills it: count, then per material a pointer (+0x28) to its own
 * record (+0x48, 0x14 bytes) holding the flip frame's value, or to the material itself when it does not flip.
 * Likely bug (kept for matching): the -1 fill of slots 1..3 always writes record 0, not the current one.
 *
 */
void cTexFlipModifier::setupTexFlip()
{
    sRailSelF218* self = (sRailSelF218*)this;
    int i;
    self->numTexFlipMaterials = 0;
    self->count = **(int**)(self->owner + 0x94);
    for (i = 0; i < self->count; i++) {
        sRailItemF218* item = *(sRailItemF218**)(*(char**)(self->owner + 0x94) + (i << 2) + 4);
        int* table = (int*)item->data;
        if (table != 0) {
            self->numTexFlipMaterials = *table;
            break;
        }
    }
    sel_F218(self, self->index);
    ms_F218(&self->blockNumMaterials, 0, 196);
    self->blockNumMaterials = self->count;
    for (i = 0; i < self->count; i++) {
        sRailItemF218* item = *(sRailItemF218**)(*(char**)(self->owner + 0x94) + (i << 2) + 4);
        if (item->data != 0) {
            self->src[i] = &self->out[i];
            self->src[i]->fC = item->fC;
            // writes slots 1..3 of out[0] for every entry, as tCrowdBlock3d_buildAnimation does (bug in the original)
            short* firstRecord = (short*)self->out;
            for (int j = 1; j < 4; j++) {
                firstRecord[j] = -1;
            }
            self->out[i].value = *(unsigned short*)(item->data + (self->index << 2) + 4);
        } else {
            self->src[i] = (sRailOutF218*)item;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_setTexIndex);
#ifdef SKIP_ASM
struct sRailModSelect;
extern "C" void cTexFlipModifier_setupMaterialRecords(cTexFlipModifier* self);

/**
 * Selects which texture a texture-flip modifier shows.
 *
 * Clamps @p i to the last texture (count at +0x20 minus 1; no lower bound), stores it in TexIndex (+0x14) and
 * updates every flipping material's record with cTexFlipModifier_setupMaterialRecords.
 *
 * @param i  the texture index (On Tour texindex)
 */
void cTexFlipModifier::setTexIndex(int i)
{
    int n = NumTexFlipMaterials;
    if (i >= n) {
        i = n - 1;
    }
    TexIndex = i;
    setupMaterialRecords();
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_setupMaterialRecords);
#ifdef SKIP_ASM
/**
 * Points every flipping material of a texture-flip modifier at the texture for TexIndex.
 *
 * For each of the NumMaterials materials it reads the instance's material list (m_MaterialBlock) and, where an entry
 * has a texture table (m_TextureFlip), copies the texture for TexIndex into that material's record (m_sTexture) in
 * MaterialBlock.
 */
void cTexFlipModifier::setupMaterialRecords()
{
    int i;
    for (i = 0; i < NumMaterials; i++) {
        tMaterialBlockRecord* list = Instance->m_MaterialBlock;
        tMaterialRecord* item = list->GetMaterialRecordList()[i];
        tTextureFlipRecord* table = item->m_TextureFlip.m_pTextureFlip;
        if (table != 0) {
            MaterialBlock.MaterialRecords[i].m_sTexture = table->GetTextureTable()[TexIndex];
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_update);
#ifdef SKIP_ASM
/**
 * Advances the tex-flip timer and, on each whole step, shows the next (or previous) texture of the flip sequence.
 *
 * Time grows by Speed per frame (nothing happens while Speed <= 0); when it reaches 1.0 it drops by 1.0 and Type
 * decides: kTexFlip_Sequence (0) steps TexIndex forward or, with Dir set, backward, wrapping at NumTexFlipMaterials;
 * kTexFlip_RandomSlow (1) sets Speed to 10 / the frame rate (cAppMan_TheAppMan + 0x10), switches to kTexFlip_RandomFast
 * and steps forward; kTexFlip_RandomFast (2) sets Speed to BaseSpeed times a random 0.25..1.0 (BXrand mantissa trick),
 * switches to kTexFlip_RandomSlow and steps forward. Ends with cTexFlipModifier_setTexIndex (On Tour
 * setTexIndex). The comment above the definition calls the timer a rail timer; it is the tex-flip one.
 */
void cTexFlipModifier::update()
{
    if (!(0.0f < Speed)) {
        return;
    }
    float one = 1.0f;
    float* acc = (float*)(int*)&Time;
    *acc += Speed;
    if (!(one <= *acc)) {
        return;
    }
    *acc -= one;
    if (Type == cTexFlipModifier::kTexFlip_Sequence) {
        if (Dir == 0) {
            if (++TexIndex >= NumTexFlipMaterials) {
                TexIndex = 0;
            }
        } else if (--TexIndex < 0) {
            TexIndex = NumTexFlipMaterials - 1;
        }
    } else if (Type == cTexFlipModifier::kTexFlip_RandomSlow) {
        Speed = 10.0f / (float)cAppMan_TheAppMan->TimerHz;
        Type = cTexFlipModifier::kTexFlip_RandomFast;
        if (++TexIndex >= NumTexFlipMaterials) {
            TexIndex = 0;
        }
    } else if (Type == cTexFlipModifier::kTexFlip_RandomFast) {
        union { int i; float f; } bits;
        float base = 0.25f;
        bits.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
        Type = cTexFlipModifier::kTexFlip_RandomSlow;
        Speed = BaseSpeed * ((bits.f - one) * 0.75f + base);
        if (++TexIndex >= NumTexFlipMaterials) {
            TexIndex = 0;
        }
    }
    setTexIndex(TexIndex);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_signal);
#ifdef SKIP_ASM
struct sRailModSelect;
extern "C" void cTexFlipModifier_setTexIndex(cTexFlipModifier* self, int i);

struct sRailModSel_F598 {
    int field_0x0;
    int field_0x4;
    float speed;        // 0x8
    char pad_0xc[0x8];
    int index;          // 0x14
    char pad_0x18[0x8];
    int count;          // 0x20
};

// PORT: R5900 truncate-to-integer-valued float, kept in FPU registers (no GPR round trip).
static inline float fTrunc_F598(float x)
{
    float r;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=&f"(r) : "f"(x));
    return r;
}

/**
 * Handles a script signal for the tex-flip modifier: set its speed, pick a texture or step to the next one.
 *
 * Signal kTexFlipSignal_Speed (300) sets Speed to @p value; kTexFlipSignal_SetTexture (301) with 0 <= @p value < 9 sets
 * TexIndex to @p value rounded up and applies it (cTexFlipModifier_setTexIndex, On Tour setTexIndex);
 * kTexFlipSignal_NextTexture (302) steps TexIndex forward, wrapping to 1 (not 0 as cTexFlipModifier_update does), and
 * applies it. Other ids are ignored.
 *
 * @param id  the signal id (On Tour signal)
 * @param value  the signal's argument value (On Tour arg)
 */
void cTexFlipModifier::signal(int id, float value)
{
    switch (id) {
    case kTexFlipSignal_Speed:
        Speed = value;
        break;
    case kTexFlipSignal_SetTexture:
        if (value >= 0.0f && value < 9.0f) {
            float ceilVal = fTrunc_F598(value);
            if (ceilVal < value) {
                ceilVal += 1.0f;
            }
            int n = (int)ceilVal;
            TexIndex = n;
            setTexIndex(n);
        }
        break;
    case kTexFlipSignal_NextTexture:
        TexIndex++;
        if (TexIndex >= NumTexFlipMaterials) {
            TexIndex = 1;
        }
        setTexIndex(TexIndex);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes the tex-flip modifier to a replay frame: the 0x18-byte cTexFlipModifierState (Type .. TexIndex) with slot
 * 1, then Instance with slot 5.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
void cTexFlipModifier::writeToReplayFrame(cBxReplayFrame* stream)
{
    stream->write((const char*)&Type, 0x18);
    stream->writeReplayInstance((const tInstanceRecord*)Instance);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_cUVScrollModifier);
#ifdef SKIP_ASM

struct sRmQuad_F6E8 {
    float x, y, z, w;
    sRmQuad_F6E8(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
} __attribute__((aligned(16)));

// The make-data (names: [O] On Tour tAddUVScrollData, same offsets).
struct sRmDesc_F6E8 {
    int InstId;     // 0x0
    int Type;       // 0x4
    float UStart;   // 0x8
    float VStart;   // 0xC
    float UScroll;  // 0x10
    float VScroll;  // 0x14
    float TimeOn;   // 0x18
    float TimeOff;  // 0x1C
    float AngVel;   // 0x20
    float AxisX;    // 0x24
    float AxisY;    // 0x28
    float AxisZ;    // 0x2C
};

// The modifier (names: cUVScrollModifier's, [O] cUVScrollModifierState).
struct sRmState_F6E8 {
    int Type;           // 0x0
    int Time;           // 0x4
    float TimeOn;       // 0x8
    float TimeOff;      // 0xC
    int Angle;          // 0x10
    float AngVel;       // 0x14
    char pad_0x18[0x8];
    sRmQuad_F6E8 Axis;  // 0x20
    float UV[2];        // 0x30
    float UVScroll[2];  // 0x38
    int On;             // 0x40
};

/**
 * Constructs a UV-scroll modifier from UV-scroll make-data.
 *
 * Sets On to 1, Type from +0x4, UV from UStart/VStart (+0x8, +0xC), TimeOn/TimeOff from +0x18/+0x1C, zeroes Time and
 * Angle, AngVel = AngVel (+0x20) / frame rate, Axis = (AxisX, AxisY, AxisZ, 0) and UVScroll from UScroll/VScroll
 * (+0x10, +0x14).
 *
 * @param scrollData  UV-scroll make-data (tAddUVScrollData)
 * @return self
 */
extern "C" void* cUVScrollModifier_cUVScrollModifier(sRmState_F6E8* self, sRmDesc_F6E8* scrollData)
{
    self->On = 1;
    self->Type = scrollData->Type;
    self->UV[0] = scrollData->UStart;
    self->UV[1] = scrollData->VStart;
    self->TimeOn = scrollData->TimeOn;
    self->TimeOff = scrollData->TimeOff;
    self->Time = 0;
    self->Angle = 0;
    self->AngVel = scrollData->AngVel * (1.0f / (float)cAppMan_TheAppMan->TimerHz);
    self->Axis = sRmQuad_F6E8(scrollData->AxisX, scrollData->AxisY, scrollData->AxisZ, 0.0f);
    self->UVScroll[0] = scrollData->UScroll;
    self->UVScroll[1] = scrollData->VScroll;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_cUVScrollModifier1);
#ifdef SKIP_ASM
struct sRailSerVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Constructs a UV-scroll modifier from a replay frame by reading the whole 80-byte object.
 *
 * @param stream  replay stream to read from
 * @return self
 */
void* cUVScrollModifier::cUVScrollModifier1(cBxReplayFrame* stream)
{
    stream->read((char*)this, 80);
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_update);
#ifdef SKIP_ASM

// t2Vector arithmetic as update uses it (EA's t2Vector operators; the math header declares none).
static inline t2Vector operator-(const t2Vector& v)
{
    return t2Vector(-v.x, -v.y);
}
static inline t2Vector operator*(const t2Vector& v, float s)
{
    return t2Vector(v.x * s, v.y * s);
}

/**
 * Advances the UV-scroll modifier: rotates the texture, scrolls its UV offset and switches between the on and off
 * phases.
 *
 * Does nothing unless TimeOn or TimeOff is positive. Time grows by 1 / the frame rate (cAppMan_TheAppMan + 0x10). While
 * On: Angle grows by AngVel; at TimeOn the timer restarts, On clears when TimeOff > 0 and kUVScroll_PingPong (2) and
 * kUVScroll_EasedPingPong (6) reverse UVScroll. While still on, kUVScroll_EasedPingPong eases the UV offset with a
 * triangle profile over the on phase (UVScroll * t / TimeOn, the sign of UVScroll.x choosing the half), other types
 * add UVScroll per frame; each UV component wraps into -1..1. While off, at TimeOff the timer restarts and On is set
 * again when TimeOn > 0. The 2*pi wrap tests and adjusts AngVel instead of Angle, and its second test is AngVel <= 2*pi
 * (not -2*pi), so a slow AngVel gains 2*pi every other frame and Angle grows without bound (harmless for the rotation,
 * likely an original bug).
 */
void cUVScrollModifier::update()
{
    if (TimeOn > 0.0f || TimeOff > 0.0f) {
        Time += 1.0f / (float)cAppMan_TheAppMan->TimerHz;
        if (On) {
            if (AngVel != 0.0f) {
                Angle += AngVel;
                if (AngVel >= 6.2831854820251465f) {
                    AngVel = AngVel - 6.2831854820251465f;
                } else if (AngVel <= 6.2831854820251465f) {
                    AngVel = AngVel + 6.2831854820251465f;
                }
            }
            if (Time >= TimeOn) {
                Time = 0.0f;
                if (Time < TimeOff) {
                    On = 0;
                }
                if (Type == kUVScroll_PingPong || Type == kUVScroll_EasedPingPong) {
                    UVScroll = -UVScroll;
                }
            }
            if (On) {
                if (Type == kUVScroll_EasedPingPong) {
                    if (UVScroll.x < 0.0f) {
                        float time = Time;
                        if (time < TimeOn * 0.5f) {
                            t2Vector step = UVScroll * time * (1.0f / TimeOn);
                            UV.x += step.x;
                            UV.y += step.y;
                        } else {
                            t2Vector step = UVScroll * (TimeOn - time) * (1.0f / TimeOn);
                            UV.x += step.x;
                            UV.y += step.y;
                        }
                    } else {
                        float remaining = TimeOn - Time;
                        if (remaining < TimeOn * 0.5f) {
                            t2Vector step = UVScroll * remaining * (1.0f / TimeOn);
                            UV.x += step.x;
                            UV.y += step.y;
                        } else {
                            t2Vector step = UVScroll * (TimeOn - remaining) * (1.0f / TimeOn);
                            UV.x += step.x;
                            UV.y += step.y;
                        }
                    }
                } else {
                    UV.x += UVScroll.x;
                    UV.y += UVScroll.y;
                }
                if (UV.x > 1.0f) {
                    UV.x = UV.x - 1.0f;
                } else if (UV.x < -1.0f) {
                    UV.x = UV.x + 1.0f;
                }
                if (UV.y > 1.0f) {
                    UV.y = UV.y - 1.0f;
                } else if (UV.y < -1.0f) {
                    UV.y = UV.y + 1.0f;
                }
            }
        } else if (Time >= TimeOff) {
            Time = 0.0f;
            if (TimeOn > 0.0f) {
                On = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_signal);
#ifdef SKIP_ASM
/**
 * Handles a script signal for the UV-scroll modifier: scroll speeds, on / off times or the UV offset.
 *
 * Signal kUVScrollSignal_UScroll (201) sets UVScroll.x and kUVScrollSignal_VScroll (200) UVScroll.y (range -1..1;
 * note the order is the reverse of 204/205); kUVScrollSignal_TimeOn (202) sets TimeOn and kUVScrollSignal_TimeOff
 * (203) TimeOff (0..60 seconds); kUVScrollSignal_U (204) sets UV.x and kUVScrollSignal_V (205) UV.y (-4..4). Values
 * out of range and other ids are ignored.
 *
 * @param id  the signal id (On Tour signal)
 * @param value  the signal's argument value (On Tour arg)
 */
void cUVScrollModifier::signal(int id, float value)
{
    if (id == kUVScrollSignal_VScroll || id == kUVScrollSignal_UScroll) {
        if (value >= -1.0f && value <= 1.0f) {
            if (id == kUVScrollSignal_UScroll) {
                UVScroll.x = value;
            } else {
                UVScroll.y = value;
            }
        }
    } else if (id == kUVScrollSignal_TimeOn || id == kUVScrollSignal_TimeOff) {
        if (value >= 0.0f && value <= 60.0f) {
            if (id == kUVScrollSignal_TimeOn) {
                TimeOn = value;
            } else {
                TimeOff = value;
            }
        }
    } else if (id == kUVScrollSignal_U || id == kUVScrollSignal_V) {
        if (value >= -4.0f && value <= 4.0f) {
            if (id == kUVScrollSignal_U) {
                UV.x = value;
            } else {
                UV.y = value;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_getTextureMat);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);

extern t4Matrix t4mat_Identity_35FC20[] __asm__("t4mat_Identity");

/**
 * Builds the texture matrix of a UV-scroll modifier: a rotation by Angle about Axis (identity when Angle is 0) whose
 * row 3 holds the UV offset (UV.x, UV.y).
 *
 * @param mat  receives the matrix (On Tour t4Matrix& mat)
 * @return @p mat
 */
t4Matrix* cUVScrollModifier::getTextureMat(t4Matrix* mat)
{
    float zeroF = 0.0f;
    if (Angle != zeroF) {
        float sinA;
        float cosA;
        fSinCos(&sinA, &cosA, Angle);
        float one = 1.0f;
        float oneMinusCos = one - cosA;
        float tx = oneMinusCos * Axis.x;
        float sy = sinA * Axis.y;
        float sz = sinA * Axis.z;
        float sx = sinA * Axis.x;
        float ty = oneMinusCos * Axis.y;
        float tz = oneMinusCos * Axis.z;
        mat->row[0].x = tx * Axis.x + cosA;
        mat->row[1].x = tx * Axis.y + sz;
        mat->row[2].x = tx * Axis.z - sy;
        mat->row[3].x = zeroF;
        mat->row[0].y = ty * Axis.x - sz;
        mat->row[1].y = ty * Axis.y + cosA;
        mat->row[2].y = ty * Axis.z + sx;
        mat->row[3].y = zeroF;
        mat->row[0].z = tz * Axis.x + sy;
        mat->row[1].z = tz * Axis.y - sx;
        mat->row[2].z = tz * Axis.z + cosA;
        mat->row[3].z = zeroF;
        mat->row[0].w = zeroF;
        mat->row[1].w = zeroF;
        mat->row[2].w = zeroF;
        mat->row[3].w = one;
        } else {
        // PORT: PS2-only VU0 inline asm (4x4 matrix copy, identity -> m).
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(mat), "r"(t4mat_Identity_35FC20)
            : "memory");
    }
    mat->row[3].x = UV.x;
    mat->row[3].y = UV.y;
    return mat;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes the whole UV-scroll modifier (0x50 bytes, all cUVScrollModifierState) through the replay frame's write
 * virtual (slot 1).
 *
 * @param obj  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
void cUVScrollModifier::writeToReplayFrame(cBxReplayFrame* obj)
{
    obj->write((const char*)this, 80);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035FDD0);
#ifdef SKIP_ASM
/**
 * Returns its first argument and does nothing else.
 *
 * It is variadic: the prologue only spills the remaining argument registers. Nothing references it.
 *
 * @return self
 */
extern "C" void* func_0035FDD0(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", __sti__all_in_one_object_cpp);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sV3_FE10 {
    float x, y, z;
    sV3_FE10(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

struct sV4_FE10 {
    float x, y, z, w;
    sV4_FE10(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    sV4_FE10(float ax, const sV3_FE10& v) : x(ax), y(v.x), z(v.y), w(v.z) {}
    // Placement form without throw(): g++ then adds no null check (the original was a global's ctor call).
    void* operator new(unsigned int, void* p) { return p; }
};

// CatmullRom: a Catmull-Rom basis matrix
struct sMat_FE10 {
    float m[16];
    // arguments are the rows of the basis; stored transposed (column-major)
    sMat_FE10(const float& a00, const float& a01, const float& a02, const float& a03,
              const float& a10, const float& a11, const float& a12, const float& a13,
              const float& a20, const float& a21, const float& a22, const float& a23,
              const float& a30, const float& a31, const float& a32, const float& a33)
    {
        m[0] = a00; m[4] = a01; m[8] = a02; m[12] = a03;
        m[1] = a10; m[5] = a11; m[9] = a12; m[13] = a13;
        m[2] = a20; m[6] = a21; m[10] = a22; m[14] = a23;
        m[3] = a30; m[7] = a31; m[11] = a32; m[15] = a33;
    }
    void* operator new(unsigned int, void* p) { return p; }
};

struct sList_FE10 {
    int a, b;
    void* operator new(unsigned int, void* p) { return p; }
};
void* cBucketMan_cBucketMan(void* self);

extern sV4_FE10 D_004FF740_v __asm__("D_004FF740");
extern sV4_FE10 D_004FF750_v __asm__("D_004FF750");
extern sV4_FE10 D_004FF760_v __asm__("D_004FF760");
extern sV4_FE10 D_004FF770_v __asm__("D_004FF770");
extern sV4_FE10 D_004FF780_v __asm__("D_004FF780");
extern sV4_FE10 D_004FF790_v __asm__("D_004FF790");
extern sV4_FE10 D_004FF7A0_v __asm__("D_004FF7A0");
extern sV4_FE10 D_004FF7B0_v __asm__("D_004FF7B0");
extern sV4_FE10 D_004FF7C0_v __asm__("D_004FF7C0");
extern sV4_FE10 D_004FF7D0_v __asm__("D_004FF7D0");
extern sV4_FE10 D_004FF7E0_v __asm__("D_004FF7E0");
extern sV4_FE10 D_004FF7F0_v __asm__("D_004FF7F0");
extern sV4_FE10 D_004FF800_v __asm__("D_004FF800");
extern sV4_FE10 D_004FF810_v __asm__("D_004FF810");
extern sV4_FE10 D_004FF820_v __asm__("D_004FF820");
extern sV4_FE10 D_004FF830_v __asm__("D_004FF830");
extern sV4_FE10 D_004FF840_v __asm__("D_004FF840");
extern sV4_FE10 D_004FF850_v __asm__("D_004FF850");
extern sV4_FE10 D_004FF860_v __asm__("D_004FF860");
extern sV4_FE10 D_004FF870_v __asm__("D_004FF870");
extern sV4_FE10 D_004FF880_v __asm__("D_004FF880");
extern sV4_FE10 D_004FF890_v __asm__("D_004FF890");
extern sV4_FE10 D_004FF8A0_v __asm__("D_004FF8A0");
extern sV4_FE10 D_004FF8B0_v __asm__("D_004FF8B0");
extern sV4_FE10 D_004FF8C0_v __asm__("D_004FF8C0");
extern sV4_FE10 D_004FF8D0_v __asm__("D_004FF8D0");
extern sV4_FE10 D_004FF8E0_v __asm__("D_004FF8E0");
extern sV4_FE10 D_004FF8F0_v __asm__("D_004FF8F0");
extern sV4_FE10 D_004FF900_v __asm__("D_004FF900");
extern sV4_FE10 D_004FF910_v __asm__("D_004FF910");
extern sV4_FE10 D_004FF920_v __asm__("D_004FF920");
extern sV4_FE10 D_004FF930_v __asm__("D_004FF930");
extern sV4_FE10 D_004FF940_v __asm__("D_004FF940");
extern sV4_FE10 D_004FF950_v __asm__("D_004FF950");
extern sV4_FE10 D_004FF960_v __asm__("D_004FF960");
extern sV4_FE10 D_004FF970_v __asm__("D_004FF970");
extern sV4_FE10 D_004FF980_v __asm__("D_004FF980");
extern sV4_FE10 D_004FF990_v __asm__("D_004FF990");
extern sV4_FE10 D_004FF9A0_v __asm__("D_004FF9A0");
extern sV4_FE10 D_004FF9B0_v __asm__("D_004FF9B0");
extern sV4_FE10 D_004FF9C0_v __asm__("D_004FF9C0");
extern sV4_FE10 D_004FF9D0_v __asm__("D_004FF9D0");
extern sV4_FE10 D_004FF9E0_v __asm__("D_004FF9E0");
extern sV4_FE10 D_004FF9F0_v __asm__("D_004FF9F0");
extern sV4_FE10 D_004FFA00_v __asm__("D_004FFA00");
extern sV4_FE10 D_004FFA10_v __asm__("D_004FFA10");
extern sV4_FE10 D_004FFA20_v __asm__("D_004FFA20");
extern sV4_FE10 D_004FFA30_v __asm__("D_004FFA30");
extern sV4_FE10 D_004FFA40_v __asm__("D_004FFA40");
extern sV4_FE10 D_004FFA50_v __asm__("D_004FFA50");
extern sV4_FE10 D_004FFA60_v __asm__("D_004FFA60");
extern sV4_FE10 D_004FFA70_v __asm__("D_004FFA70");
extern sV4_FE10 D_004FFA80_v __asm__("D_004FFA80");
extern sV4_FE10 D_004FFA90_v __asm__("D_004FFA90");
extern sV4_FE10 D_004FFAA0_v __asm__("D_004FFAA0");
extern sV4_FE10 D_004FFAB0_v __asm__("D_004FFAB0");
extern sMat_FE10 CatmullRom_m __asm__("CatmullRom");
extern sV4_FE10 D_004FFB00_v __asm__("D_004FFB00");
extern sV4_FE10 D_004FFB10_v __asm__("D_004FFB10");
extern char gBucketMan;

// Static initialisation of the unit's colour globals D_004FF740..D_004FFAB0, the spline basis CatmullRom,
// D_004FFB00/D_004FFB10 and the list gBucketMan (g++ __static_initialization_and_destruction_0).
// The unit declares __sti__all_in_one_object_cpp returning void*; bind this definition by asm label.
extern "C" void __sti__all_in_one_object_cpp_init(int initialize, int priority) __asm__("__sti__all_in_one_object_cpp");
/**
 * Runs the railmodifier unit's static initialisation: builds its global vectors and matrices and the bucket manager.
 *
 * Only for priority DEFAULT_INIT_PRIORITY with initialize set. Constructs the colour/parameter vectors
 * D_004FF740..D_004FFAB0, the Catmull-Rom basis CatmullRom used for rail segment matrices, D_004FFB00/D_004FFB10, and
 * gBucketMan (cBucketMan_cBucketMan).
 *
 * @param initialize  nonzero for construction
 * @param priority  init priority; only DEFAULT_INIT_PRIORITY does anything
 */
extern "C" void __sti__all_in_one_object_cpp_init(int initialize, int priority)
{
    if (priority == DEFAULT_INIT_PRIORITY && initialize != 0) {
        new (&D_004FF740_v) sV4_FE10(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004FF750_v) sV4_FE10(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF760_v) sV4_FE10(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF770_v) sV4_FE10(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004FF780_v) sV4_FE10(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004FF790_v) sV4_FE10(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF7A0_v) sV4_FE10(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004FF7B0_v) sV4_FE10(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004FF7C0_v) sV4_FE10(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004FF7D0_v) sV4_FE10(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004FF7E0_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF7F0_v) sV4_FE10(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF800_v) sV4_FE10(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004FF810_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF820_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF830_v) sV4_FE10(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF840_v) sV4_FE10(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF850_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF860_v) sV4_FE10(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF870_v) sV4_FE10(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF880_v) sV4_FE10(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF890_v) sV4_FE10(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004FF8A0_v) sV4_FE10(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004FF8B0_v) sV4_FE10(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004FF8C0_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF8D0_v) sV4_FE10(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004FF8E0_v) sV4_FE10(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004FF8F0_v) sV4_FE10(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004FF900_v) sV4_FE10(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004FF910_v) sV4_FE10(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004FF920_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF930_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF940_v) sV4_FE10(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004FF950_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF960_v) sV4_FE10(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004FF970_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF980_v) sV4_FE10(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF990_v) sV4_FE10(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF9A0_v) sV4_FE10(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004FF9B0_v) sV4_FE10(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004FF9C0_v) sV4_FE10(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004FF9D0_v) sV4_FE10(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004FF9E0_v) sV4_FE10(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF9F0_v) sV4_FE10(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004FFA00_v) sV4_FE10(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FFA10_v) sV4_FE10(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FFA20_v) sV4_FE10(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FFA30_v) sV4_FE10(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004FFA40_v) sV4_FE10(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004FFA50_v) sV4_FE10(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004FFA60_v) sV4_FE10(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004FFA70_v) sV4_FE10(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004FFA80_v) sV4_FE10(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004FFA90_v) sV4_FE10(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004FFAA0_v) sV4_FE10(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004FFAB0_v) sV4_FE10(1.0f, 1.0f, 0.0f, 0.0f);
        new (&CatmullRom_m) sMat_FE10(-0.5f, 1.5f, -1.5f, 0.5f,
                                      1.0f, -2.5f, 2.0f, -0.5f,
                                      -0.5f, 0.0f, 0.5f, 0.0f,
                                      0.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FFB00_v) sV4_FE10(1.0f, sV3_FE10(0.23000000417232513f, 0.2800000011920929f, 0.5f));
        new (&D_004FFB10_v) sV4_FE10(D_004FF7F0_v);
        cBucketMan_cBucketMan(&gBucketMan);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_cObjNode2);
#ifdef SKIP_ASM
extern void* cObjNode_vtbl[];

/**
 * Constructs a bare object node: Next (+0x4) and Prev (+0x8) = 0, vtable cObjNode_vtbl.
 *
 * @return self
 */
void* cObjNode::cObjNode2()
{
    Next = 0;
    VPTR_OF(cObjNode, this) = cObjNode_vtbl;
    Prev = 0;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cObjNode's update has an empty body.
 */
void cObjNode::update()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_apply__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cObjNode's apply has an empty body.
 */
void cObjNode::apply()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cObjNode's render has an empty body.
 */
void cObjNode::render()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_renderModel__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cObjNode's renderModel has an empty body.
 */
void cObjNode::renderModel(const t4Matrix*, int)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_compare__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a plain object node never sorts before another one.
 *
 * @return 0 (false)
 */
bool cObjNode::compare(const cObjNode*)
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_equal__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a plain object node never equals another one in sort order.
 *
 * @return 0 (false)
 */
bool cObjNode::equal(const cObjNode*)
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_getSortIndex__FPv);
#ifdef SKIP_ASM
/**
 * Returns a plain object node's sort index, which is always 0.
 *
 * @return 0
 */
unsigned int cObjNode::getSortIndex() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_hasParent__FPv);
#ifdef SKIP_ASM
/**
 * Reports that the object node has no parent.
 *
 * @return always 0
 */
bool cObjNode::hasParent() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_isType__FPv);
#ifdef SKIP_ASM
/**
 * Reports that the object node is not of the queried type.
 *
 * @return always 0
 */
bool cObjNode::isType(int)
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_getSectionId);
#ifdef SKIP_ASM
/**
 * Returns the world section of a plain object node, which has none.
 *
 * @return 0xFFFFFFFF (no section)
 */
extern "C" unsigned int cObjNode_getSectionId()
{
    return 0xFFFFFFFFU;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes nothing to the replay frame: cObjNode's writeToReplayFrame has an empty body.
 */
void cObjNode::writeToReplayFrame(cBxReplayFrame*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Reads nothing from the replay frame: cObjNode's readFromReplayFrame has an empty body.
 */
void cObjNode::readFromReplayFrame(cBxReplayFrame*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTypeObjNode's update has an empty body.
 */
void cTypeObjNode::update()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cTypeObjNode's render has an empty body.
 */
void cTypeObjNode::render()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_shouldRebound__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a typed object node should rebound.
 *
 * @return always 1
 */
int cTypeObjNode_shouldRebound(void* vself)
{
    cTypeObjNode* self = (cTypeObjNode*)vself;
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_isType);
#ifdef SKIP_ASM
/**
 * Tells whether an object node has a given type (the short at +0x10).
 *
 * @param type  node type to compare with
 * @return true when the types match
 */
bool cTypeObjNode::isType(int type)
{
    return ObjNodeType == type;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_getType);
#ifdef SKIP_ASM
/**
 * Returns a typed object node's ObjNodeType (+0x10, tObjNodeType).
 *
 * @return ObjNodeType
 */
short cTypeObjNode::getType()
{
    return (ObjNodeType);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_setRootTransformDirty);
#ifdef SKIP_ASM
/**
 * Sets the RootTransformDirty bit (bit 0 of Flags, +0x12) of a typed object node.
 *
 */
void cTypeObjNode::setRootTransformDirty()
{
    Flags = (unsigned short) ((Flags) | 1);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_isRootTransformDirty);
#ifdef SKIP_ASM
/**
 * Tests the RootTransformDirty bit (bit 0 of Flags, +0x12) of a typed object node.
 *
 * @return that bit (0 or 1)
 */
int cTypeObjNode::isRootTransformDirty()
{
    return (Flags) & 1;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_resetRootTransformDirty);
#ifdef SKIP_ASM
/**
 * Clears the RootTransformDirty bit (bit 0 of Flags, +0x12) of a typed object node.
 *
 */
void cTypeObjNode::resetRootTransformDirty()
{
    Flags = (unsigned short) (Flags & 0xFFFE);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_getObjNodeCount);
#ifdef SKIP_ASM
extern int cTypeObjNode_ObjNodeCount[];

/**
 * Returns how many object nodes of a type are alive (cTypeObjNode_ObjNodeCount[idx]).
 *
 * cTypeObjNode_ObjNodeCount is the per-type count the cTypeObjNode constructors increment and its destructor decrements
 * (src/object/objnode.cpp); On Tour has 23 types (kNumObjNodeType).
 *
 * @param idx  node type (tObjNodeType)
 * @return the live count for that type
 */
extern "C" int cTypeObjNode_getObjNodeCount(int idx)
{
    return cTypeObjNode_ObjNodeCount[idx];
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_compare);
#ifdef SKIP_ASM
/**
 * Orders two sort nodes: tells whether @p obj sorts at or after this node.
 *
 * Compares obj's virtual getSortIndex() (vtable slot 8) with this node's SortIndex (+0x14), unsigned.
 *
 * @param obj  the other node (On Tour cObjNode*)
 * @return true when obj's sort index is greater than or equal to this node's
 */
bool cSortObjNode::compare(const cObjNode* obj)
{
    return obj->getSortIndex() >= SortIndex;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_equal);
#ifdef SKIP_ASM
/**
 * Tells whether another node has the same sort index as this one.
 *
 * Compares this node's SortIndex (+0x14) with obj's virtual getSortIndex() (vtable slot 8).
 *
 * @param obj  the other node (On Tour cObjNode*)
 * @return true when the two sort indices are equal
 */
bool cSortObjNode::equal(const cObjNode* obj)
{
    return SortIndex == obj->getSortIndex();
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_getSortIndex__FPv);
#ifdef SKIP_ASM
/**
 * Returns the node's sort index, the word at +0x14 (cSortObjNodeState::SortIndex).
 *
 * @return the sort index
 */
int cSortObjNode_getSortIndex(void* vself)
{
    cSortObjNode* self = (cSortObjNode*)vself;
    return (int)self->SortIndex;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_init);
#ifdef SKIP_ASM
extern int cSortObjNode_ObjNodeSortIndex;
extern int cSortObjNode_ObjNodeNoSortIndex;

/**
 * Resets both sort-index counters (cSortObjNode_ObjNodeSortIndex, cSortObjNode_ObjNodeNoSortIndex) to 1.
 */
extern "C" void cSortObjNode_init(void)
{
    cSortObjNode_ObjNodeSortIndex = 1;
    cSortObjNode_ObjNodeNoSortIndex = 1;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_nextSortIndex);
#ifdef SKIP_ASM
extern int cSortObjNode_ObjNodeSortIndex;

/**
 * Hands out the next sort index (cSortObjNode_ObjNodeSortIndex, post-increment).
 *
 * @return the index before the increment
 */
extern "C" int cSortObjNode_nextSortIndex(void)
{
    return cSortObjNode_ObjNodeSortIndex++;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_nextNoSortIndex);
#ifdef SKIP_ASM
extern int cSortObjNode_ObjNodeNoSortIndex;

/**
 * Hands out the next no-sort index (cSortObjNode_ObjNodeNoSortIndex, post-increment).
 *
 * @return the index before the increment
 */
extern "C" int cSortObjNode_nextNoSortIndex(void)
{
    return cSortObjNode_ObjNodeNoSortIndex++;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_getStaticSortIndex);
#ifdef SKIP_ASM
extern int cSortObjNode_ObjNodeSortIndex;

/**
 * Returns the current sort-index counter, cSortObjNode_ObjNodeSortIndex.
 *
 * @return cSortObjNode_ObjNodeSortIndex
 */
extern "C" int cSortObjNode_getStaticSortIndex(void) {
    return cSortObjNode_ObjNodeSortIndex;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_getStaticNoSortIndex);
#ifdef SKIP_ASM
extern int cSortObjNode_ObjNodeNoSortIndex;

/**
 * Returns the current no-sort-index counter, cSortObjNode_ObjNodeNoSortIndex.
 *
 * @return cSortObjNode_ObjNodeNoSortIndex
 */
extern "C" int cSortObjNode_getStaticNoSortIndex(void) {
    return cSortObjNode_ObjNodeNoSortIndex;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_setStaticSortIndex);
#ifdef SKIP_ASM
extern int cSortObjNode_ObjNodeSortIndex;

/**
 * Sets the sort-index counter, cSortObjNode_ObjNodeSortIndex.
 *
 * @param sortIndex  new counter value
 */
extern "C" void cSortObjNode_setStaticSortIndex(int sortIndex) {
    cSortObjNode_ObjNodeSortIndex = sortIndex;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_setStaticNoSortIndex);
#ifdef SKIP_ASM
extern int cSortObjNode_ObjNodeNoSortIndex;

/**
 * Sets the no-sort-index counter, cSortObjNode_ObjNodeNoSortIndex.
 *
 * @param noSortIndex  new counter value
 */
extern "C" void cSortObjNode_setStaticNoSortIndex(int noSortIndex) {
    cSortObjNode_ObjNodeNoSortIndex = noSortIndex;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_update);
#ifdef SKIP_ASM
class cRailVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr lands at 0xC (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual int updateNode();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void updateOverlapSystem();
};

/**
 * Updates an instance node by calling its virtual updateNode() (vtable slot 15).
 */
void cInstanceNode::update()
{
    ((cRailVirt*)this)->updateNode();
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_updateNode__FPv);
#ifdef SKIP_ASM
/**
 * Does no per-node update and reports success.
 *
 * @return always 1
 */
int cInstanceNode_updateNode(void* vself)
{
    cInstanceNode* self = (cInstanceNode*)vself;
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_isMoveNode__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an instance node is not a move node.
 *
 * @return always 0 (cMoveNode returns 1)
 */
bool cInstanceNode::isMoveNode()
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getSectionId__FPv);
#ifdef SKIP_ASM
/**
 * Returns the world section of an instance node: the low byte of its instance's id (Instance +0x78).
 *
 * @return the section number
 */
unsigned int cInstanceNode::getSectionId()
{
    return (unsigned char)Instance->m_RID;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_isComplete__FPv);
#ifdef SKIP_ASM
/**
 * Reports an instance node as complete.
 *
 * @return always true
 */
bool cInstanceNode::isComplete()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_throwMe__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cInstanceNode's throwMe has an empty body.
 */
void cInstanceNode::throwMe(const t4Vector&)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an instance node has no node state.
 *
 * @return always 0
 */
void* cInstanceNode::getNodeState()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_hasFocusCount__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an instance node keeps no focus count.
 *
 * @return always 0
 */
bool cInstanceNode::hasFocusCount() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_incFocusCount__FPv);
#ifdef SKIP_ASM
/**
 * Does not change any focus count; an instance node keeps none.
 *
 * @return always 0
 */
int cInstanceNode::incFocusCount(tInstanceRecord*)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_decFocusCount__FPv);
#ifdef SKIP_ASM
/**
 * Does not change any focus count; an instance node keeps none.
 *
 * @return always 0
 */
int cInstanceNode::decFocusCount(tInstanceRecord*)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getFocusCount__FPv);
#ifdef SKIP_ASM
/**
 * Returns a focus count of zero; an instance node keeps none.
 *
 * @return always 0
 */
int cInstanceNode::getFocusCount() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getInstance);
#ifdef SKIP_ASM
/**
 * Returns an instance node's Instance (+0x18).
 *
 * @return Instance
 */
tInstanceRecord* cInstanceNode::getInstance()
{
    return Instance;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Returns an instance node's root transform: the matrix at Instance +0x10.
 *
 * @return pointer to the instance's matrix
 */
const t4Matrix* cInstanceNode::getRootTransform()
{
    return (const t4Matrix*)&Instance->m_fmTransform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getRootTransform1__FPv);
#ifdef SKIP_ASM
/**
 * Returns an instance node's root transform for an instance: always the matrix at its own Instance +0x10.
 *
 * The instance argument of the slot is not used.
 *
 * @return pointer to the node's instance matrix
 */
const t4Matrix* cInstanceNode::getRootTransform1(tInstanceRecord*)
{
    return (const t4Matrix*)&Instance->m_fmTransform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_hasAnimMats__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an instance node has no animation matrices.
 *
 * @return always 0
 */
bool cInstanceNode::hasAnimMats() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getAnimMats__FPv);
#ifdef SKIP_ASM
/**
 * Returns an instance node's animation matrices: none.
 *
 * @return 0
 */
const t4Matrix* cInstanceNode::getAnimMats()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getInstAnimMats__FPv);
#ifdef SKIP_ASM
/**
 * Returns an instance node's animation matrices for an instance: none.
 *
 * @return 0
 */
const t4Matrix* cInstanceNode::getInstAnimMats(tInstanceRecord*)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getAnimMat__FPv);
#ifdef SKIP_ASM
/**
 * Returns one animation matrix of an instance node: none.
 *
 * @return 0
 */
const t4Matrix* cInstanceNode::getAnimMat(int)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getAnimFrame__FPv);
#ifdef SKIP_ASM
/**
 * Returns animation frame zero; an instance node is not animated.
 *
 * @return always 0.0f
 */
float cInstanceNode::getAnimFrame() const
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getAnimDir__FPv);
#ifdef SKIP_ASM
/**
 * Returns an animation direction of zero for an instance node.
 *
 * @return always 0
 */
int cInstanceNode::getAnimDir() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_frameUpdateEvent__FPv);
#ifdef SKIP_ASM
/**
 * Handles no frame-update event and reports nothing done.
 *
 * @return always 0
 */
int cInstanceNode::frameUpdateEvent(float)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cInstanceNode's signal has an empty body.
 */
void cInstanceNode::signal(int, float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_makeActive__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cInstanceNode's makeActive has an empty body.
 */
void cInstanceNode::makeActive()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_hasIntersect__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an instance node has no intersection geometry.
 *
 * @return always 0
 */
bool cInstanceNode::hasIntersect() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_intersect__FPv);
#ifdef SKIP_ASM
/**
 * Reports no intersection for an instance node.
 *
 * @return always 0
 */
int cInstanceNode::intersect(cWorldIntersectType&, tIntersectInfo*, int, int)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_onCollide__FPv);
#ifdef SKIP_ASM
/**
 * Ignores collisions: cInstanceNode's onCollide has an empty body.
 */
void cInstanceNode::onCollide(const tCollideInfo*, const tIntersectInfo*, cBeing*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_setDebounce__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cInstanceNode's setDebounce has an empty body.
 */
void cInstanceNode::setDebounce(float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_updateIntersectInfo__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cInstanceNode's updateIntersectInfo has an empty body.
 */
void cInstanceNode::updateIntersectInfo(tIntersectInfo&)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_applyForce__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the force: cInstanceNode's applyForce has an empty body.
 */
void cInstanceNode::applyForce(const tIntersectInfo&, const t4Vector&)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_hasBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an instance node has no bounding box.
 *
 * @return always 0
 */
bool cInstanceNode::hasBoundBox()
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns an instance node's bound box: none.
 *
 * @return 0
 */
const tBoundBox* cInstanceNode::getBoundBox() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_isFlexRail__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an instance node is not a flex rail.
 *
 * @return always 0
 */
bool cInstanceNode::isFlexRail() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_9cDeadNode);
#ifdef SKIP_ASM
/**
 * Destroys a dead node; it owns nothing of its own, so its cInstanceNode part does the work and frees it when asked.
 */
cDeadNode::~cDeadNode()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDeadNode_expire__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cDeadNode's expire has an empty body.
 */
void cDeadNode::expire(int)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDeadNode_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cDeadNode's loseFocus has an empty body.
 */
void cDeadNode::loseFocus()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDeadNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cDeadNode's signal has an empty body.
 */
void cDeadNode::signal(int, float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDeadFadeNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores signals: cDeadFadeNode's signal has an empty body.
 */
void cDeadFadeNode::signal(int, float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_9cHideNode);
#ifdef SKIP_ASM
/**
 * Destroys a hide node; it owns nothing of its own, so its cInstanceNode part does the work and frees it when asked.
 */
cHideNode::~cHideNode()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHideNode_expire__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a hide node expires.
 */
void cHideNode::expire(int)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHideNode_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a hide node loses focus.
 */
void cHideNode::loseFocus()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHideNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores signals sent to a hide node.
 */
void cHideNode::signal(int, float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHideNode_onCollide__FPv);
#ifdef SKIP_ASM
/**
 * Ignores collisions with a hide node.
 */
void cHideNode::onCollide(const tCollideInfo*, const tIntersectInfo*, cBeing*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_12cRestoreNode);
#ifdef SKIP_ASM
/**
 * Destroys a restore node; it owns nothing of its own, so its cInstanceNode part does the work and frees it when asked.
 */
cRestoreNode::~cRestoreNode()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRestoreNode_expire__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a restore node expires.
 */
void cRestoreNode_expire(void* vself)
{
    cRestoreNode* self = (cRestoreNode*)vself;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRestoreNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores signals sent to a restore node.
 */
void cRestoreNode::signal(int, float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostNode_boostStart__FPv);
#ifdef SKIP_ASM
/**
 * Hook called when a boost starts: does nothing.
 */
void cBoostNode::boostStart()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostNode_boostEnd__FPv);
#ifdef SKIP_ASM
/**
 * Hook called when a boost ends: does nothing.
 */
void cBoostNode::boostEnd()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier__cTransformModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys the transform-modifier base: restores cTransformModifier_vtbl and frees the object when bit 0 of
 * @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cTransformModifier__cTransformModifier(cTransformModifier* self, int flags)
{
    *(void***)self = cTransformModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_update__FPv);
#ifdef SKIP_ASM
/**
 * Base cTransformModifier::update: does nothing.
 */
void cTransformModifier::update()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_updateRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a transform modifier's root-transform update hook.
 */
void cTransformModifier::updateRootTransform()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_postUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTransformModifier's postUpdate has an empty body.
 */
void cTransformModifier::postUpdate()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cTransformModifier's signal has an empty body.
 */
void cTransformModifier::signal(int, float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_isComplete__FPv);
#ifdef SKIP_ASM
/**
 * Reports a transform modifier as not complete.
 *
 * @return always 0
 */
bool cTransformModifier::isComplete()
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_clearComplete__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTransformModifier's clearComplete has an empty body.
 */
void cTransformModifier::clearComplete()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_shouldRebound__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a transform modifier should rebound.
 *
 * @return always true (cRollerModifier returns false)
 */
bool cTransformModifier::shouldRebound()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_runCollide__FPv);
#ifdef SKIP_ASM
/**
 * Reports that collision should run for a transform modifier.
 *
 * @return always true (cRollerModifier returns false)
 */
bool cTransformModifier::runCollide(int)
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_collide__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTransformModifier's collide has an empty body.
 */
void cTransformModifier::collide(const tCollideInfo*, const tIntersectInfo*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cTransformModifier's render has an empty body.
 */
void cTransformModifier::render()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Base cTransformModifier::getBoundBox: no bound box.
 *
 * @return 0 (On Tour returns const tBoundBox*)
 */
const tBoundBox* cTransformModifier::getBoundBox() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_setBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Base cTransformModifier::setBoundBox: does nothing (On Tour takes const tBoundBox& boundbox, not declared here).
 */
void cTransformModifier::setBoundBox(const tBoundBox&)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getRadius__FPv);
#ifdef SKIP_ASM
/**
 * Base cTransformModifier::getRadius: no radius.
 *
 * @return 0.0
 */
float cTransformModifier::getRadius() const
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_setRadius__FPv);
#ifdef SKIP_ASM
/**
 * Base cTransformModifier::setRadius: does nothing (On Tour takes float radius, not declared here).
 */
void cTransformModifier::setRadius(float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getMesh__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a transform modifier has no mesh.
 *
 * @return always 0
 */
int cTransformModifier::getMesh() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_isRootTransformDirty__FPv);
#ifdef SKIP_ASM
/**
 * Reports the transform modifier's root transform as clean.
 *
 * @return always 0
 */
bool cTransformModifier::isRootTransformDirty() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: a plain transform modifier has no root transform of its own.
 *
 * @return 0
 */
const t4Matrix* cTransformModifier::getRootTransform()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getRootTransform1__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a transform modifier has no root transform.
 *
 * @return always 0
 */
const t4Matrix* cTransformModifier::getRootTransform1(tInstanceRecord*)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_hasIntersect__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a transform modifier has no intersection geometry.
 *
 * @return always 0
 */
bool cTransformModifier::hasIntersect() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_intersect__FPv);
#ifdef SKIP_ASM
/**
 * Reports no intersection for a transform modifier.
 *
 * @return always 0
 */
int cTransformModifier::intersect(cWorldIntersectType&, tIntersectInfo*, int, int)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_updateIntersectInfo__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTransformModifier's updateIntersectInfo has an empty body.
 */
void cTransformModifier::updateIntersectInfo(tIntersectInfo&)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getAnimMats__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a transform modifier has no animation matrices.
 *
 * @return always 0
 */
const t4Matrix* cTransformModifier::getAnimMats(tInstanceRecord*)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_updateOverlapSystem__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTransformModifier's updateOverlapSystem has an empty body.
 */
void cTransformModifier::updateOverlapSystem()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getParent__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a transform modifier has no parent.
 *
 * @return always 0
 */
cMoveNode* cTransformModifier::getParent() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cTransformModifier: kTransform_None (0).
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return kTransform_None
 */
int cTransformModifier::getTransformType() const
{
    return cTransformModifier::kTransform_None;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for saving a transform modifier into a replay frame: writes nothing.
 *
 * Subclasses with state override slot 27 (cPositionModifier_writeToReplayFrame, cAvaSplineModifier_writeToReplayFrame).
 */
void cTransformModifier::writeToReplayFrame(cBxReplayFrame*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPhysicsModifier_cPhysicsModifier1);
#ifdef SKIP_ASM
extern void* cPhysicsModifier_vtbl[];

/**
 * Constructs a physics modifier: installs cPhysicsModifier_vtbl and sets Active to 1.
 *
 * @return physicsModifier
 */
void * cPhysicsModifier::cPhysicsModifier1()
{
    ((int*)VPTR_OF(cPhysicsModifier, this)) = (int*)&cPhysicsModifier_vtbl;
    Active = 1;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPhysicsModifier__cPhysicsModifier);
#ifdef SKIP_ASM
extern void* cPhysicsModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys a physics modifier: restores cPhysicsModifier's vtable at +0x4, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPhysicsModifier__cPhysicsModifier(cPhysicsModifier* self, int flags)
{
    VPTR_OF(cPhysicsModifier, self) = cPhysicsModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPhysicsModifier_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cPhysicsModifier's update has an empty body.
 */
void cPhysicsModifier::update()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPhysicsModifier_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cPhysicsModifier's signal has an empty body.
 */
void cPhysicsModifier::signal(int, float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPhysicsModifier_isActive__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the physics modifier is active: returns its first word (+0x0).
 *
 * @return the active flag (nonzero when active)
 */
int cPhysicsModifier::isActive()
{
    return Active;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPhysicsModifier_getPhysicsType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the base physics modifier's physics type id.
 *
 * @return always kPhysics_None (cBoostModifier returns kPhysics_Boost)
 */
int cPhysicsModifier::getPhysicsType()
{
    return cPhysicsModifier::kPhysics_None;
}
#endif

extern "C" void* cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(void* self);

//100%
INCLUDE_ASM("object/railmodifier", cDynamicEmitter__cDynamicEmitter__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a dynamic emitter through its base-class destructor.
 *
 * Calls cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(); only @p self is declared here, so any further
 * argument the caller passed (such as destructor flags) reaches it unchanged in its register.
 *
 * @return the base destructor's result
 */
void* cDynamicEmitter__cDynamicEmitter(void* vself)
{
    cDynamicEmitter* self = (cDynamicEmitter*)vself;
    return cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectLink__cEffectLink);
#ifdef SKIP_ASM
extern "C" void cEffectLink_unlink(void* self);
void operator_delete(int*);
extern void* cEffectLink_vtbl[];

/**
 * Destroys an effect link: restores its vtable at +0x8, unlinks it from its list, then frees it when asked to.
 *
 * cEffectLink_unlink() joins the link's neighbours and clears its next/prev pointers.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cEffectLink__cEffectLink(cEffectLink* self, int flags)
{
    VPTR_OF(cEffectLink, self) = cEffectLink_vtbl;
    cEffectLink_unlink(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectLink_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the type of a plain effect link, 1.
 *
 * tModifierBlock_remHaloModifier compares this slot's result with 3 (the halo modifier) to find halo links.
 *
 * @return 1
 */
int cEffectLink::getType()
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectLink_fadeEffect__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cEffectLink's fadeEffect has an empty body.
 */
void cEffectLink::fadeEffect()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectLink_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes nothing to the replay frame: cEffectLink's writeToReplayFrame has an empty body.
 */
void cEffectLink::writeToReplayFrame(cBxReplayFrame*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectModifier_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cEffectModifier's update has an empty body.
 */
void cEffectModifier_update(void* vself)
{
    cEffectModifier* self = (cEffectModifier*)vself;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectModifier_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cEffectModifier's render has an empty body.
 */
void cEffectModifier::render()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectModifier_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cEffectModifier's signal has an empty body.
 */
void cEffectModifier::signal(int, float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectModifier_getEffectCount);
#ifdef SKIP_ASM
extern int cEffectModifier_EffectCount;

/**
 * Returns the number of live effect modifiers, cEffectModifier_EffectCount.
 *
 * src/object/effectlink.cpp's cEffectModifier constructors increment cEffectModifier_EffectCount and its destructor decrements it.
 *
 * @return cEffectModifier_EffectCount
 */
extern "C" int cEffectModifier_getEffectCount(void) {
    return cEffectModifier_EffectCount;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParticleModifier_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the effect type id of a particle modifier.
 *
 * @return always kEffect_Particle (dynamic particle modifiers return kEffect_DynamicParticle, halo modifiers
 *         kEffect_Halo)
 */
int cParticleModifier::getType()
{
    return cEffectLink::kEffect_Particle;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDynamicParticleModifier_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the effect type id of a dynamic particle modifier.
 *
 * @return always kEffect_DynamicParticle (particle modifiers return kEffect_Particle, halo modifiers kEffect_Halo)
 */
int cDynamicParticleModifier::getType()
{
    return cEffectLink::kEffect_DynamicParticle;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDynamicParticleModifier_getDynamicParticleCount);
#ifdef SKIP_ASM
extern int cDynamicParticleModifier_DynamicParticleCount;

/**
 * Returns the number of live dynamic particle modifiers, cDynamicParticleModifier_DynamicParticleCount.
 *
 * src/object/effectlink.cpp's dynamic-particle constructors increment cDynamicParticleModifier_DynamicParticleCount and the destructor decrements it.
 *
 * @return cDynamicParticleModifier_DynamicParticleCount
 */
extern "C" int cDynamicParticleModifier_getDynamicParticleCount(void) {
    return cDynamicParticleModifier_DynamicParticleCount;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHaloModifier_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the effect type id of a halo modifier.
 *
 * @return always kEffect_Halo (particle modifiers return kEffect_Particle, dynamic particle modifiers
 *         kEffect_DynamicParticle)
 */
int cHaloModifier::getType()
{
    return cEffectLink::kEffect_Halo;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailLink__cRailLink);
#ifdef SKIP_ASM
extern "C" void cRailLink_unlink(void* self);
void operator_delete(int*);
extern void* cRailLink_vtbl[];

/**
 * Destroys a rail link: restores its vtable at +0x8, unlinks it from its list, then frees it when asked to.
 *
 * cRailLink_unlink() joins the link's neighbours and clears its next/prev pointers.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRailLink__cRailLink(cRailLink* self, int flags)
{
    VPTR_OF(cRailLink, self) = cRailLink_vtbl;
    cRailLink_unlink(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailLink_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes nothing to the replay frame: cRailLink's writeToReplayFrame has an empty body.
 */
void cRailLink::writeToReplayFrame(cBxReplayFrame*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_getSplineId);
#ifdef SKIP_ASM
/**
 * Returns a rail modifier's SplineId (+0x30).
 *
 * @return SplineId
 */
int cRailModifier::getSplineId()
{
    return ((int)SplineId);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_apply);
#ifdef SKIP_ASM
/**
 * Applies a move node by calling its virtual updateOverlapSystem() (vtable slot 50).
 */
void cMoveNode::apply()
{
    ((cRailVirt*)this)->updateOverlapSystem();
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_updateAnimState__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cMoveNode's updateAnimState has an empty body.
 */
void cMoveNode_updateAnimState(void* vself)
{
    cMoveNode* self = (cMoveNode*)vself;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_isMoveNode__FPv);
#ifdef SKIP_ASM
/**
 * Reports that this node is a move node.
 *
 * @return always true (cInstanceNode returns false)
 */
bool cMoveNode::isMoveNode()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_updateRootTransform);
#ifdef SKIP_ASM
/**
 * Marks a move node's root transform as up to date by clearing bit 0 of its flags (+0x12).
 */
void cMoveNode::updateRootTransform()
{
    unsigned short* nodeFlags = (unsigned short*)((char*)&Flags);
    *nodeFlags &= 0xFFFE;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_getParentMesh);
#ifdef SKIP_ASM
class cVirt16 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual int getMesh();
};

/**
 * Returns the mesh a move node's transform modifier follows, or 0.
 *
 * Calls the getMesh slot (vtable word 33, cTransformModifier_getMesh by default) of the transform modifier at
 * ModifierBlock +0x0; returns 0 when there is no modifier block or no transform modifier.
 *
 * @return the modifier's mesh index, or 0
 */
int cMoveNode::getParentMesh()
{
    cVirt16** holder = (cVirt16**)ModifierBlock;
    if (holder != 0) {
        cVirt16* obj = *holder;
        if (obj != 0) {
            return obj->getMesh();
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_getTransformModifier);
#ifdef SKIP_ASM
/**
 * Returns the node's transform modifier: the first word of its modifier block.
 *
 * @return the transform modifier, or 0 when the node has no modifier block
 */
int cMoveNode::getTransformModifier()
{
    int* modBlock = (int*)ModifierBlock;
    if (modBlock != 0) {
        return *modBlock;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_hasTextureMat);
#ifdef SKIP_ASM
/**
 * Tells whether a move node has a UV-scroll modifier (the source of its texture matrix).
 *
 * True when ModifierBlock exists and its UV-scroll slot (+0x4) is set; cMoveNode_getTextureMat reads the matrix from
 * that modifier.
 *
 * @return 1 if it has one, else 0
 */
int cMoveNode::hasTextureMat()
{
    int has;
    tModifierBlock* modBlock;

    modBlock = ModifierBlock;
    has = 0;
    if (modBlock != 0) {
        has = modBlock->UVScroll != 0;
    }
    return has;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_hasMaterialBlock);
#ifdef SKIP_ASM
/**
 * Tells whether a move node has a texture-flip modifier (the source of its material block).
 *
 * True when ModifierBlock exists and its texture-flip slot (+0x8) is set.
 *
 * @return 1 if it has one, else 0
 */
int cMoveNode::hasMaterialBlock()
{
    int has;
    tModifierBlock* modBlock;

    modBlock = ModifierBlock;
    has = 0;
    if (modBlock != 0) {
        has = modBlock->TexFlip != 0;
    }
    return has;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_getMaterialBlock);
#ifdef SKIP_ASM
/**
 * Returns the node's material block: the texture-flip modifier's MaterialBlock (+0x24) when it has one, else its
 * instance's (+0x94).
 *
 * @return the material block record (On Tour const tMaterialBlockRecord*)
 */
const tMaterialBlockRecord* cMoveNode::getMaterialBlock()
{
    int cond;
    tModifierBlock* modBlock = ModifierBlock;
    cond = 0;
    if (modBlock != 0) {
        cond = modBlock->TexFlip != 0;
    }
    if (cond != 0) {
        cTexFlipModifier* texFlip = modBlock->TexFlip;
        if (texFlip != 0) {
            // the modifier's block extends tMaterialBlockRecord (On Tour tTexFlipMaterialBlockRecord)
            return (const tMaterialBlockRecord*)&texFlip->MaterialBlock;
        }
        return 0;
    }
    return Instance->m_MaterialBlock;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_getTextureMat);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// the unit declares cUVScrollModifier_getTextureMat with two parameters; this caller passes one
void* cUVScrollModifier_getTextureMat_1arg(void*) __asm__("cUVScrollModifier_getTextureMat");

/**
 * Returns the node's texture matrix from its UV-scroll modifier (cUVScrollModifier_getTextureMat), when it has one.
 *
 * On Tour takes the destination (t4Matrix& mat); here it is not declared and stays in $a1 for the call.
 *
 * @return the matrix, or 0 when the node has no UV-scroll modifier
 */
void* cMoveNode::getTextureMat()
{
    tModifierBlock* modBlock = ModifierBlock;
    int cond = 0;
    if (modBlock != 0) {
        cond = modBlock->UVScroll != 0;
    }
    if (cond != 0) {
        void* uvScroll = modBlock->UVScroll;
        if (uvScroll != 0) {
            return cUVScrollModifier_getTextureMat_1arg(uvScroll);
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remTransformModifier);
#ifdef SKIP_ASM
/**
 * Removes a move node's transform modifier (tModifierBlock_remTransformModifier), if it has a modifier block.
 *
 */
void cMoveNode::remTransformModifier()
{
    tModifierBlock* modBlock = ModifierBlock;
    if (modBlock != 0) {
        modBlock->remTransformModifier();
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remUVScrollModifier);
#ifdef SKIP_ASM
/**
 * Removes a move node's UV-scroll modifier (tModifierBlock_remUVScrollModifier), if it has a modifier block.
 *
 */
void cMoveNode::remUVScrollModifier()
{
    tModifierBlock* modBlock = ModifierBlock;
    if (modBlock != 0) {
        modBlock->remUVScrollModifier();
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remTexFlipModifier);
#ifdef SKIP_ASM
/**
 * Removes a move node's texture-flip modifier (tModifierBlock_remTexFlipModifier), if it has a modifier block.
 *
 */
void cMoveNode::remTexFlipModifier()
{
    tModifierBlock* modBlock = ModifierBlock;
    if (modBlock != 0) {
        modBlock->remTexFlipModifier();
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remPhysicsModifier);
#ifdef SKIP_ASM
/**
 * Removes a move node's physics modifier (tModifierBlock_remPhysicsModifier), if it has a modifier block.
 *
 */
void cMoveNode::remPhysicsModifier()
{
    tModifierBlock* modBlock = ModifierBlock;
    if (modBlock != 0) {
        modBlock->remPhysicsModifier();
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remHaloModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remHaloModifier(tModifierBlock* block);

/**
 * Removes a move node's halo effects (tModifierBlock_remHaloModifier), if it has a modifier block.
 *
 */
void cMoveNode::remHaloModifier()
{
    tModifierBlock* modBlock = ModifierBlock;
    if (modBlock != 0) {
        tModifierBlock_remHaloModifier(modBlock);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remEffectModifier);
#ifdef SKIP_ASM
/**
 * Removes a move node's effect modifiers (tModifierBlock_remEffectModifier), if it has a modifier block.
 *
 */
void cMoveNode::remEffectModifier()
{
    tModifierBlock* modBlock = ModifierBlock;
    if (modBlock != 0) {
        modBlock->remEffectModifier();
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_fadeEffectModifier);
#ifdef SKIP_ASM
/**
 * Fades out a move node's effects (tModifierBlock_fadeEffectModifier), if it has a modifier block.
 *
 */
void cMoveNode::fadeEffectModifier()
{
    tModifierBlock* modBlock = ModifierBlock;
    if (modBlock != 0) {
        modBlock->fadeEffectModifier();
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_11cObjectNode);
#ifdef SKIP_ASM
/**
 * Destroys an object node; it owns nothing of its own, so its cMoveNode part does the work and frees it when asked.
 */
cObjectNode::~cObjectNode()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_updateNode__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an animated node's per-frame update keeps the node alive.
 *
 * @return 1
 */
int cAnimNode::updateNode()
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_getAnimFrame);
#ifdef SKIP_ASM
/**
 * Returns the animation position in frames: the float at +0x0 times 30.
 *
 * The stored value is probably a time in seconds, converted at 30 frames per second.
 *
 * @return the float at +0x0 multiplied by 30
 */
float cAnimNode::getAnimFrame()
{
    float input = CurrTime;
    return input * 30.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_getAnimSpeed__FPv);
#ifdef SKIP_ASM
/**
 * Returns an animated node's animation speed, which is 0 for the base class.
 *
 * @return 0.0f
 */
float cAnimNode::getAnimSpeed()
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_hasAnimMats__FPv);
#ifdef SKIP_ASM
/**
 * Reports that an animated node has animation matrices.
 *
 * @return always 1
 */
int cAnimNode::hasAnimMats()
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_getAnimMats);
#ifdef SKIP_ASM

/**
 * Returns the animation matrix array, first refreshing the root transform when it is dirty.
 *
 * When bit 0 (RootTransformDirty) of the embedded node's Flags (+0x26) is set, calls that node's (+0x14)
 * virtual slot 51, updateRootTransform in the cMoveNode family's vtables.
 *
 * @return the matrix array pointer stored at +0x44
 */
t4Matrix* cAnimNode::getAnimMats()
{
    if (Flags & 1) {
        void* sub = (char*)&MoveNodeBase;
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)mVtbl;
        vt[51].fn(vthis(sub, vt[51]));
    }
    return AnimMats;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_getAnimMat);
#ifdef SKIP_ASM


/**
 * Returns one animation matrix, first refreshing the root transform when it is dirty.
 *
 * The refresh is the same as cAnimNode_getAnimMats(): virtual slot 51 of the embedded node at +0x14 when bit 0
 * of its Flags (+0x26) is set.
 *
 * @param i  index of the matrix in the array at +0x44 (0x40-byte matrices)
 * @return the address of matrix @p i
 */
t4Matrix* cAnimNode::getAnimMat(int i)
{
    char* obj = (char*)&MoveNodeBase;
    if (Flags & 1) {
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)mVtbl;
        vt[51].fn(vthis(obj, vt[51]));
    }
    return &AnimMats[i];
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_clearFrameEvent);
#ifdef SKIP_ASM
/**
 * Clears an animated node's two frame-event times (+0x8, +0xC) by setting them to 1e10.
 *
 * The animated-node update functions (cAnimComboNode_updateNode, cAnimDeltaNode_updateNode) store the same 1e10 into these fields of their
 * embedded animated node.
 *
 */
void cAnimNode::clearFrameEvent()
{
    FrameStart = 1e10f;
    FrameEnd = 1e10f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimObjectNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns an animated object node's state block, which is the node itself.
 *
 * @return this node
 */
void* cAnimObjectNode::getNodeState()
{
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimObjectNode_getAnimSpeed);
#ifdef SKIP_ASM
/**
 * Returns an animated object node's speed (+0x10), or 0 once it is complete (+0x8 set).
 *
 * @return the speed, or 0.0f
 */
float cAnimObjectNode::getAnimSpeed()
{
    return AnimComplete == 0 ? AnimSpeed : 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimObjectNode_getAnimDir);
#ifdef SKIP_ASM
/**
 * Returns an animated object node's play direction: 1 when its speed (+0x10) is >= 0, else -1.
 *
 * @return 1 or -1
 */
int cAnimObjectNode::getAnimDir()
{
    int direction = -1;
    if (AnimSpeed >= 0.0f) {
        direction = 1;
    }
    return direction;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimObjectNode_isComplete__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the animated object node has finished: returns the word at +0x8 of the object it is called on.
 *
 * @return the completion flag (nonzero when complete)
 */
int cAnimObjectNode::isComplete()
{
    return AnimComplete;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimFrame_cAnimFrame1);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
extern "C" void cAnimFrame_init(void*);

/**
 * Constructs an empty animation frame: installs cAnimFrame_vtbl (+0x84) and resets it with cAnimFrame_init.
 *
 * @return self
 */
void* cAnimFrame::cAnimFrame1()
{
    mVtbl = cAnimFrame_vtbl;
    cAnimFrame_init(this);
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimFrame__cAnimFrame);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
void operator_delete(int*);

/**
 * Destroys an animation frame: restores cAnimFrame's vtable at +0x84, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAnimFrame__cAnimFrame(cAnimFrame* self, int flags)
{
    self->mVtbl = cAnimFrame_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimFrame_getAngles);
#ifdef SKIP_ASM

/**
 * Returns an animation frame's angles: the values at +0x50..+0x58 negated, with w = 0.
 *
 * @param out  receives the angles
 */
void cAnimFrame::getAngles(t4Vector* out)
{
    t4Vector negated;
    negated.x = -Vals[3];
    negated.y = -Vals[4];
    negated.z = -Vals[5];
    negated.w = 0.0f;
    *out = negated;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMeshAnimFrame_cMeshAnimFrame1);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
extern void* cMeshAnimFrame_vtbl[];
extern "C" void cAnimFrame_init(void*);
extern "C" void cMeshAnimFrame_init(cMeshAnimFrame*);

/**
 * Constructs an empty mesh animation frame.
 *
 * Builds the cAnimFrame part (cAnimFrame_vtbl, cAnimFrame_init), installs cMeshAnimFrame_vtbl and sets FrameMatrix
 * (+0x90) to identity with cMeshAnimFrame_init.
 *
 * @return self
 */
void* cMeshAnimFrame::cMeshAnimFrame1()
{
    mVtbl = cAnimFrame_vtbl;
    cAnimFrame_init(this);
    mVtbl = cMeshAnimFrame_vtbl;
    init();
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMeshAnimFrame__cMeshAnimFrame);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
void operator_delete(int*);

/**
 * Destroys a mesh animation frame: stores the base cAnimFrame vtable at +0x84, then frees the object when asked
 * to.
 *
 * The cMeshAnimFrame part has nothing to destroy; the vtable store is the inlined cAnimFrame destructor.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cMeshAnimFrame__cMeshAnimFrame(cMeshAnimFrame* self, int flags)
{
    self->mVtbl = cAnimFrame_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMeshAnimFrame_init);
#ifdef SKIP_ASM
extern t4Matrix t4mat_Identity;

/**
 * Resets a mesh animation frame's FrameMatrix (+0x90) to t4mat_Identity.
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
void cMeshAnimFrame::init()
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)&FrameMatrix), "r"(&t4mat_Identity)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMeshAnimFrame_getFrameMatrix);
#ifdef SKIP_ASM
/**
 * Returns a mesh animation frame's FrameMatrix (frame + 0x90).
 *
 * @param frame  the mesh animation frame
 * @return pointer to FrameMatrix
 */
extern "C" int cMeshAnimFrame_getFrameMatrix(int frame) {
    return frame + 144;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame_cCamAnimFrame1);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
extern void* cCamAnimFrame_vtbl[];
extern "C" void cAnimFrame_init(void*);

/**
 * Constructs an empty camera animation frame: the cAnimFrame part (cAnimFrame_vtbl, cAnimFrame_init), then cCamAnimFrame_vtbl.
 *
 * @return self
 */
void* cCamAnimFrame::cCamAnimFrame1()
{
    mVtbl = cAnimFrame_vtbl;
    cAnimFrame_init(this);
    mVtbl = cCamAnimFrame_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame__cCamAnimFrame);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
void operator_delete(int*);

/**
 * Destroys a camera animation frame: reinstalls cAnimFrame_vtbl and frees the frame when bit 0 of flags is set.
 *
 * @param flags  destructor flags
 */
extern "C" void cCamAnimFrame__cCamAnimFrame(cCamAnimFrame* self, int flags)
{
    self->mVtbl = cAnimFrame_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame_setAnimRecord);
#ifdef SKIP_ASM
/**
 * Sets a camera animation frame's animation record (+0x40).
 *
 * @param source  the animation record
 */
void cCamAnimFrame::setAnimRecord(const tAnimRecord* source)
{
    AnimRecord = source;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame_getPosition);
#ifdef SKIP_ASM

/**
 * Returns a camera animation frame's position: the values at +0x44..+0x4C with w = 1.
 *
 * @param out  receives the position
 */
void cCamAnimFrame::getPosition(t4Vector* out)
{
    t4Vector result;
    result.x = Vals[0];
    result.y = Vals[1];
    result.z = Vals[2];
    result.w = 1.0f;
    *out = result;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame_getAngles);
#ifdef SKIP_ASM

/**
 * Returns a camera animation frame's angles: the three values at +0x50..+0x58.
 *
 * @param out  receives the angles
 */
void cCamAnimFrame::getAngles(t3Vector* out)
{
    t3Vector result;
    result.x = Vals[3];
    result.y = Vals[4];
    result.z = Vals[5];
    *out = result;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame_getFocalLength);
#ifdef SKIP_ASM
/**
 * Returns a camera animation frame's focal length (+0x70) through out.
 *
 * @param out  receives the focal length
 */
void cCamAnimFrame::getFocalLength(float *out)
{
    *out = Vals[11];
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimTeeterNode__cAnimTeeterNode);
#ifdef SKIP_ASM
extern "C" void cAnimNode__cAnimNode(void* self, int flags);
void operator_delete(int*);
extern void* cAnimTeeterNode_vtbl[];

/**
 * Destroys a teeter animation node.
 *
 * Reinstalls cAnimTeeterNode_vtbl (+0x50), destroys the animated-node part at +0x30 (cAnimNode__cAnimNode, flags 0) and frees the node
 * when bit 0 of flags is set.
 *
 * @param flags  destructor flags
 */
extern "C" void cAnimTeeterNode__cAnimTeeterNode(cAnimTeeterNode* self, int flags)
{
    self->mVtbl = cAnimTeeterNode_vtbl;
    cAnimNode__cAnimNode(&self->CurrTime, 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimTeeterNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns a teeter animation node's state block, which is the node itself.
 *
 * @return this node
 */
void* cAnimTeeterNode::getNodeState()
{
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimTeeterNode_getAnimDir);
#ifdef SKIP_ASM
/**
 * Returns a teeter animation node's play direction: 1 when the value at +0x4 is >= 0, else -1.
 *
 * @return 1 or -1
 */
int cAnimTeeterNode::getAnimDir()
{
    int direction = -1;
    if (AnimSpeed >= 0.0f) {
        direction = 1;
    }
    return direction;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_14cBoostModifier);
#ifdef SKIP_ASM
void operator_delete(int*);

/**
 * Destroys a boost modifier; it owns nothing of its own, so its cPhysicsModifier part does the work and frees it when
 * asked.
 */
cBoostModifier::~cBoostModifier()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostModifier_getPhysicsType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the boost modifier's physics type id.
 *
 * @return always kPhysics_Boost (the base cPhysicsModifier returns kPhysics_None)
 */
int cBoostModifier::getPhysicsType()
{
    return cPhysicsModifier::kPhysics_Boost;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostModifier_boostStart__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a boost starts; the body is empty.
 */
void cBoostModifier::boostStart()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostModifier_boostEnd__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a boost ends; the body is empty.
 */
void cBoostModifier::boostEnd()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_13cDebounceNode);
#ifdef SKIP_ASM
/**
 * Destroys a debounce node; it owns nothing of its own, so its cMoveNode part does the work and frees it when asked.
 */
cDebounceNode::~cDebounceNode()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDebounceNode_onCollide__FPv);
#ifdef SKIP_ASM
/**
 * Ignores collisions: cDebounceNode's onCollide has an empty body.
 */
void cDebounceNode::onCollide(const tCollideInfo*, const tIntersectInfo*, cBeing*)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_13cConveyorNode);
#ifdef SKIP_ASM
/**
 * Destroys the conveyor node; it owns nothing of its own, so its cInstanceNode part does the work and frees it when
 * asked.
 */
cConveyorNode::~cConveyorNode()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cConveyorNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns a conveyor node's state block (this + 0x1C).
 *
 * @return pointer to the state
 */
void* cConveyorNode::getNodeState()
{
    return (char*)&Vel;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_12cCounterNode);
#ifdef SKIP_ASM
/**
 * Destroys the counter node; it owns nothing of its own, so its cInstanceNode part does the work and frees it when
 * asked.
 */
cCounterNode::~cCounterNode()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexBridgeNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the flex bridge node itself as its node state.
 *
 * @return this node
 */
void* cFlexBridgeNode::getNodeState()
{
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexBridgeNode_hasIntersect__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a flex bridge node has intersection geometry.
 *
 * @return always true
 */
bool cFlexBridgeNode::hasIntersect() const
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexBridgeNode_hasBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a flex bridge node has a bounding box.
 *
 * @return always true
 */
bool cFlexBridgeNode::hasBoundBox()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexBridgeNode_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns the flex bridge's bounding box, stored at +0x90.
 *
 * @return the address of the bound box (On Tour const tBoundBox*)
 */
const tBoundBox* cFlexBridgeNode::getBoundBox() const
{
    return &BoundBox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_updateNodes__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a flex rail's per-frame node update.
 */
void cFlexRailNode_updateNodes(void* vself)
{
    cFlexRailNode* self = (cFlexRailNode*)vself;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_updateMats__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a flex rail's segment-matrix update.
 */
void cFlexRailNode::updateMats()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_applyForce__FPv);
#ifdef SKIP_ASM
/**
 * Ignores forces applied to a flex rail.
 */
void cFlexRailNode::applyForce(const tIntersectInfo&, const t4Vector&)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_updateIntersectInfo__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a flex rail's intersect-info update.
 */
void cFlexRailNode::updateIntersectInfo(tIntersectInfo&)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_getMesh__FPv);
#ifdef SKIP_ASM
/**
 * Returns the mesh of a flex rail segment: always 0.
 *
 * @return 0
 */
int cFlexRailNode::getMesh(int)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_isFlexRail__FPv);
#ifdef SKIP_ASM
/**
 * Reports that the node is a flex rail.
 *
 * @return true
 */
bool cFlexRailNode::isFlexRail() const
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatRailNode_getMesh);
#ifdef SKIP_ASM
/**
 * Returns the mesh of a float rail segment: the int at +0x44 of that segment's entry in the 0x50-byte node array
 * (+0x70).
 *
 * @param segIndex  segment index
 * @return the segment mesh index
 */
int cFloatRailNode::getMesh(int segIndex)
{
    tFloatRailNode* railNodes = Nodes;
    return railNodes[segIndex].Mesh;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSpringRailNode_getMesh);
#ifdef SKIP_ASM
/**
 * Returns the mesh of a spring rail segment: the int at +0x54 of that segment's entry in the 0x60-byte node array
 * (+0x70).
 *
 * @param segIndex  segment index
 * @return the segment mesh index
 */
int cSpringRailNode::getMesh(int segIndex)
{
    tSpringRailNode* railNodes = Nodes;
    return railNodes[segIndex].Mesh;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_updateMat);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern float CatmullRom[16];



// PORT: PS2-only VU0 inline asm (vector * 4x4 basis matrix, row-vector convention).
static inline void vu0BasisMul_15D8(t4Vector& dst, const t4Vector& g, const float* b)
{
    __asm__(
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(dst)
        : "m"(g), "r"(b)
        : "memory");
}

typedef int tMmiQuad __attribute__((mode(TI)));

// PORT: hand-written MMI 4x4 transpose in place (lq/pextlw/pextuw/pcpyld/pcpyud/sq: no gcc 2.95 pattern); a port
// swaps m[i][j] and m[j][i] in plain C.
#define MMI_TRANSPOSE4(m)                                                                                              \
    {                                                                                                                  \
        tMmiQuad r0_, r1_, r2_, r3_, t0_, t1_, t2_, t3_;                                                               \
        __asm__ __volatile__(                                                                                          \
            "lq      %0, 0x0(%8)\n"                                                                                    \
            "lq      %1, 0x10(%8)\n"                                                                                   \
            "lq      %2, 0x20(%8)\n"                                                                                   \
            "lq      %3, 0x30(%8)\n"                                                                                   \
            "pextlw  %4, %1, %0\n"                                                                                     \
            "pextuw  %5, %1, %0\n"                                                                                     \
            "pextlw  %6, %3, %2\n"                                                                                     \
            "pextuw  %7, %3, %2\n"                                                                                     \
            "pcpyld  %0, %6, %4\n"                                                                                     \
            "pcpyud  %1, %4, %6\n"                                                                                     \
            "pcpyld  %2, %7, %5\n"                                                                                     \
            "pcpyud  %3, %5, %7\n"                                                                                     \
            "sq      %0, 0x0(%8)\n"                                                                                    \
            "sq      %1, 0x10(%8)\n"                                                                                   \
            "sq      %2, 0x20(%8)\n"                                                                                   \
            "sq      %3, 0x30(%8)\n"                                                                                   \
            : "=&r"(r0_), "=&r"(r1_), "=&r"(r2_), "=&r"(r3_), "=&r"(t0_), "=&r"(t1_), "=&r"(t2_), "=&r"(t3_)          \
            : "r"(m)                                                                                                   \
            : "memory");                                                                                               \
    }

/**
 * Builds a rail segment's cubic coefficient matrix from four control points.
 *
 * For each axis (x, y, z) multiplies the four control points' components by the Catmull-Rom basis CatmullRom, giving
 * the x, y and z coefficient rows 0-2 with (0, 0, 0, 1) as row 3, then transposes out in place. Nothing in retail calls
 * it.
 *
 * @param out  receives the matrix
 * @param ctrlPoint0  first control point
 * @param ctrlPoint1  second control point
 * @param ctrlPoint2  third control point
 * @param ctrlPoint3  fourth control point
 */
// PORT: hand-written MMI 4x4 transpose (MMI_TRANSPOSE4: lq/pextlw/pextuw/pcpyld/pcpyud/sq, no gcc 2.95 pattern;
// retail's registers differ per call site, so an operand asm) plus VU0 macro asm; a port replaces them with a
// plain C transpose and a C vector-matrix product.
void cFlexRailNode::updateMat(t4Matrix* out, const float* ctrlPoint0, const float* ctrlPoint1, const float* ctrlPoint2,
                              const float* ctrlPoint3)
{
    t4Matrix coeffMat;
    t4Vector zCoeffs;
    coeffMat.row[1].x = ctrlPoint0[0]; coeffMat.row[1].y = ctrlPoint1[0]; coeffMat.row[1].z = ctrlPoint2[0]; coeffMat.row[1].w = ctrlPoint3[0];
    vu0BasisMul_15D8(coeffMat.row[2], coeffMat.row[1], CatmullRom);
    coeffMat.row[0] = coeffMat.row[2];
    coeffMat.row[2].x = ctrlPoint0[1]; coeffMat.row[2].y = ctrlPoint1[1]; coeffMat.row[2].z = ctrlPoint2[1]; coeffMat.row[2].w = ctrlPoint3[1];
    vu0BasisMul_15D8(coeffMat.row[3], coeffMat.row[2], CatmullRom);
    coeffMat.row[1] = coeffMat.row[3];
    coeffMat.row[3].x = ctrlPoint0[2]; coeffMat.row[3].y = ctrlPoint1[2]; coeffMat.row[3].z = ctrlPoint2[2]; coeffMat.row[3].w = ctrlPoint3[2];
    vu0BasisMul_15D8(zCoeffs, coeffMat.row[3], CatmullRom);
    coeffMat.row[2] = zCoeffs;
    coeffMat.row[3].x = 0.0f; coeffMat.row[3].y = 0.0f; coeffMat.row[3].z = 0.0f; coeffMat.row[3].w = 1.0f;
    *out = coeffMat;
    MMI_TRANSPOSE4(out);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cFlagSet_Empty);
#ifdef SKIP_ASM
/**
 * Tells whether a flag set holds no flags: its flag count (+0x5C) is 0.
 *
 * @return true if empty
 */
bool cFlagSet::Empty()
{
    return (mCurrentNumInstances) == 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cFlagManager_GetWindFactor);
#ifdef SKIP_ASM
/**
 * Returns a flag manager's current wind factor (+0x10).
 *
 * @return the wind factor
 */
float cFlagManager::GetWindFactor()
{
    return (mCurrentWindFactor);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the floating node's state block, which starts at +0x2C.
 *
 * @return the address of the node state
 */
void* cFloatingNode::getNodeState()
{
    return &((cFloatingNode*)this)->ZSpring;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_hasBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a floating node has a bounding box.
 *
 * @return always 1
 */
bool cFloatingNode::hasBoundBox()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_getBoundBox);
#ifdef SKIP_ASM
extern "C" const tBoundBox* tModifierBlock_getBoundBox(void* p);

/**
 * Returns a floating node's bounding box: its modifier block's when it has one (+0x1C), otherwise its info
 * block (+0x78).
 *
 * @return the bounding box (On Tour const tBoundBox*), or 0 if neither exists
 */
const tBoundBox* cFloatingNode::getBoundBox() const
{
    void* modBlock = ModifierBlock;
    if (modBlock != 0) {
        return tModifierBlock_getBoundBox(modBlock);
    }
    return &Info->BoundBox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_hasAnimMats__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a floating node has animation matrices.
 *
 * @return always true
 */
bool cFloatingNode::hasAnimMats() const
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_getAnimMats);
#ifdef SKIP_ASM

/**
 * Returns the floating node's animation matrices, first refreshing the root transform when it is dirty.
 *
 * When bit 0 (RootTransformDirty) of Flags (+0x12) is set, calls the virtual updateRootTransform() (slot 51,
 * cFloatingNode_updateRootTransform).
 *
 * @return the matrix array pointer stored at +0x74
 */
const t4Matrix* cFloatingNode::getAnimMats()
{
    if (Flags & 1) {
        updateRootTransform();
    }
    return AnimMats;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_getAnimMat);
#ifdef SKIP_ASM


/**
 * Returns one of the floating node's animation matrices, first refreshing the root transform when it is dirty.
 *
 * The refresh is the same as cFloatingNode_getAnimMats(): updateRootTransform() (slot 51) when Flags bit 0 is
 * set.
 *
 * @param i  index of the matrix in the array at +0x74 (0x40-byte matrices)
 * @return the address of matrix @p i
 */
const t4Matrix* cFloatingNode::getAnimMat(int i)
{
    if (Flags & 1) {
        updateRootTransform();
    }
    return &AnimMats[i];
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_updateRootTransform);
#ifdef SKIP_ASM
/**
 * Rebuilds a floating node's part matrices and clears its dirty bit.
 *
 * Calls makeAnimMats (slot 33) with the root transform (slot 24) and the matrix array at +0x74.
 */
void cFloatingNode::updateRootTransform()
{
    // makeAnimMats (slot 33) and getRootTransform (slot 24) by hand: retail adjusts this for makeAnimMats before it
    // calls getRootTransform.
    sVEntry<void(void*, const t4Matrix*, t4Matrix*)>* vt =
        (sVEntry<void(void*, const t4Matrix*, t4Matrix*)>*)VPTR_OF(cObjNode, this);
    sVEntry<const t4Matrix*(void*)>* getVt = (sVEntry<const t4Matrix*(void*)>*)vt;
    void* obj = vthis(this, vt[33]);
    const t4Matrix* transform = getVt[24].fn(vthis(this, getVt[24]));
    vt[33].fn(obj, transform, AnimMats);
    Flags &= ~1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier__cPositionModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys a position modifier: restores the cTransformModifier base vtable (cTransformModifier_vtbl) and frees the object when
 * bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPositionModifier__cPositionModifier(cPositionModifier* self, int flags)
{
    *(void***)self = cTransformModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_update__FPv);
#ifdef SKIP_ASM
/**
 * The position modifier's update virtual: an empty stub (a fixed position needs no per-frame work).
 */
void cPositionModifier::update()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_isRootTransformDirty__FPv);
#ifdef SKIP_ASM
/**
 * Returns a position modifier's RootTransformDirty (+0x44).
 *
 * @return RootTransformDirty
 */
int cPositionModifier_isRootTransformDirty(void* vself)
{
    cPositionModifier* self = (cPositionModifier*)vself;
    return self->RootTransformDirty;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cPositionModifier: kTransform_Position (3).
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return kTransform_Position
 */
int cPositionModifier::getTransformType() const
{
    return cTransformModifier::kTransform_Position;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns a position modifier's BoundBox (this + 0x10).
 *
 * @return pointer to BoundBox
 */
const tBoundBox* cPositionModifier::getBoundBox() const
{
    return &((cPositionModifier*)this)->BoundBox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_setBoundBox);
#ifdef SKIP_ASM
/**
 * Copies a bound box (minimum and maximum points) into the position modifier's BoundBox.
 *
 * @param boundbox  the new bound box
 */
void cPositionModifier::setBoundBox(const tBoundBox& boundbox)
{
    BoundBox.MinPoint = boundbox.MinPoint;
    BoundBox.MaxPoint = boundbox.MaxPoint;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_getRadius__FPv);
#ifdef SKIP_ASM
/**
 * Returns a position modifier's Radius (+0x40).
 *
 * @return Radius
 */
float cPositionModifier::getRadius() const
{
    return Radius;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_setRadius__FPvf);
#ifdef SKIP_ASM
/**
 * Sets a position modifier's Radius (+0x40).
 *
 * @param val  new radius
 */
void cPositionModifier::setRadius(float val)
{
    Radius = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_getRootTransform);
#ifdef SKIP_ASM

/**
 * Returns the position modifier's root transform (Transform, +0x50), first rebuilding it (slot 3,
 * updateRootTransform) when RootTransformDirty is set.
 *
 * @return the transform matrix
 */
const t4Matrix* cPositionModifier::getRootTransform()
{
    if (RootTransformDirty != 0) {
        updateRootTransform();
    }
    return &Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_15cParentModifier);
#ifdef SKIP_ASM
/**
 * Destroys a parent modifier; it owns nothing of its own, so its cPositionModifier and cTransformModifier parts do the
 * work and free it when asked.
 */
cParentModifier::~cParentModifier()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_update__FPv);
#ifdef SKIP_ASM
/**
 * The parent modifier's update virtual: marks the root transform dirty (+0x44) so it follows the parent every
 * frame.
 */
void cParentModifier::update()
{
    RootTransformDirty = 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_getMesh__FPv);
#ifdef SKIP_ASM
/**
 * Returns the parent mesh index the modifier follows (Mesh, +0x90).
 *
 * @return the mesh index within the parent instance
 */
int cParentModifier::getMesh() const
{
    return Mesh;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cParentModifier: kTransform_Parent (4).
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return kTransform_Parent
 */
int cParentModifier::getTransformType() const
{
    return cTransformModifier::kTransform_Parent;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_setParent);
#ifdef SKIP_ASM
/**
 * Sets a parent modifier's ParentInst (+0xA0). Nothing in retail calls it.
 *
 * @param parentInst  instance record to follow
 */
void cParentModifier::setParent(tInstanceRecord* parentInst)
{
    ParentInst = parentInst;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_15cMagnetModifier);
#ifdef SKIP_ASM
/**
 * Destroys a magnet modifier; it owns nothing of its own, so its cTransformModifier part does the work and frees it
 * when asked.
 */
cMagnetModifier::~cMagnetModifier()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_isRootTransformDirty__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the magnet's transform changed since it was last rebuilt (RootTransformDirty, +0x50).
 *
 * @return true when dirty
 */
bool cMagnetModifier::isRootTransformDirty() const
{
    return (RootTransformDirty != 0);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_hasIntersect__FPv);
#ifdef SKIP_ASM
/**
 * The magnet modifier's hasIntersect virtual: always 1 (the magnet object takes part in rider intersection).
 *
 * @return always true
 */
bool cMagnetModifier::hasIntersect() const
{
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns the magnet modifier's bound box (BoundBox, +0x20).
 *
 * @return a pointer to the bound box (On Tour returns tBoundBox&)
 */
const tBoundBox* cMagnetModifier::getBoundBox() const
{
    return &((cMagnetModifier*)this)->BoundBox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_setBoundBox);
#ifdef SKIP_ASM
/**
 * Copies a bound box (min and max, two quadwords) into the magnet modifier's BoundBox.
 *
 * @param boundbox  the new bound box (On Tour const tBoundBox& boundbox)
 */
void cMagnetModifier::setBoundBox(const tBoundBox& boundbox)
{
    BoundBox.MinPoint = boundbox.MinPoint;
    BoundBox.MaxPoint = boundbox.MaxPoint;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_getRadius__FPv);
#ifdef SKIP_ASM
/**
 * Returns the magnet modifier's radius (Radius, +0x40).
 *
 * @return the radius
 */
float cMagnetModifier::getRadius() const
{
    return Radius;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_setRadius__FPv);
#ifdef SKIP_ASM
/**
 * The magnet modifier's setRadius virtual: an empty stub, so the radius set by the constructor never changes (On
 * Tour setRadius(float radius); the float is ignored).
 */
void cMagnetModifier::setRadius(float)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cMagnetModifier: kTransform_Magnet (6).
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return kTransform_Magnet
 */
int cMagnetModifier::getTransformType() const
{
    return cTransformModifier::kTransform_Magnet;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_getRootTransform);
#ifdef SKIP_ASM

/**
 * Returns the magnet modifier's root transform (Transform, +0x60), first rebuilding it (slot 3, updateRootTransform)
 * when RootTransformDirty is set.
 *
 * @return the transform matrix
 */
const t4Matrix* cMagnetModifier::getRootTransform()
{
    if (RootTransformDirty != 0) {
        updateRootTransform();
    }
    return &Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_18cAvaSplineModifier);
#ifdef SKIP_ASM
/**
 * Destroys an avalanche spline modifier; it owns nothing of its own, so its cTransformModifier part does the work and
 * frees it when asked.
 */
cAvaSplineModifier::~cAvaSplineModifier()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns the avalanche spline modifier's bound box (BoundBox, +0x10).
 *
 * @return a pointer to the bound box (On Tour returns tBoundBox&)
 */
const tBoundBox* cAvaSplineModifier::getBoundBox() const
{
    return &((cAvaSplineModifier*)this)->BoundBox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_setBoundBox);
#ifdef SKIP_ASM
/**
 * Copies a bound box (min and max, two quadwords) into the avalanche spline modifier's BoundBox.
 *
 * @param boundbox  the new bound box (On Tour const tBoundBox& boundbox)
 */
void cAvaSplineModifier::setBoundBox(const tBoundBox& boundbox)
{
    BoundBox.MinPoint = boundbox.MinPoint;
    BoundBox.MaxPoint = boundbox.MaxPoint;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_getRadius__FPv);
#ifdef SKIP_ASM
/**
 * Returns the avalanche spline modifier's radius (Radius, +0x30).
 *
 * @return the radius
 */
float cAvaSplineModifier::getRadius() const
{
    return Radius;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_setRadius__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the avalanche spline modifier's radius (Radius, +0x30).
 *
 * @param val  the new radius (On Tour radius)
 */
void cAvaSplineModifier::setRadius(float val)
{
    Radius = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cAvaSplineModifier: kTransform_AvaSpline (7).
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return kTransform_AvaSpline
 */
int cAvaSplineModifier::getTransformType() const
{
    return cTransformModifier::kTransform_AvaSpline;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_getRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the modifier's Transform matrix (+0x40).
 *
 * @return the root transform
 */
const t4Matrix* cAvaSplineModifier::getRootTransform()
{
    return &((cAvaSplineModifier*)this)->Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_15cSplineModifier);
#ifdef SKIP_ASM
/**
 * Destroys a spline modifier; it owns nothing of its own, so its cTransformModifier part does the work and frees it
 * when asked.
 */
cSplineModifier::~cSplineModifier()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_isComplete__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the spline modifier has completed: returns the flag at +0x50.
 *
 * @return the completion flag (nonzero when complete)
 */
int cSplineModifier_isComplete(void* vself)
{
    cSplineModifier* self = (cSplineModifier*)vself;
    return self->Complete;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_clearComplete__FPv);
#ifdef SKIP_ASM
/**
 * Clears the spline modifier's completion flag at +0x50.
 */
void cSplineModifier::clearComplete()
{
    Complete = 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns the spline modifier's bounding box, stored at +0x10.
 *
 * @return the address of the bound box (On Tour const tBoundBox*)
 */
const tBoundBox* cSplineModifier::getBoundBox() const
{
    return &((cSplineModifier*)this)->BoundBox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_setBoundBox);
#ifdef SKIP_ASM
/**
 * Sets a spline modifier's bounding box (+0x10 min, +0x20 max).
 *
 * @param boundbox  the new box (On Tour const tBoundBox&)
 */
void cSplineModifier::setBoundBox(const tBoundBox& boundbox)
{
    BoundBox.MinPoint = boundbox.MinPoint;
    BoundBox.MaxPoint = boundbox.MaxPoint;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_getRadius__FPv);
#ifdef SKIP_ASM
/**
 * Returns the spline modifier's radius, the float at +0x4C.
 *
 * @return the radius
 */
float cSplineModifier::getRadius() const
{
    return Radius;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_setRadius__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the spline modifier's radius, the float at +0x4C.
 *
 * @param val  the new radius
 */
void cSplineModifier::setRadius(float val)
{
    Radius = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_isRootTransformDirty__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the spline modifier's root transform needs rebuilding: returns the flag at +0x58.
 *
 * @return the dirty flag (nonzero when the root transform is out of date)
 */
int cSplineModifier_isRootTransformDirty(void* vself)
{
    cSplineModifier* self = (cSplineModifier*)vself;
    return self->RootTransformDirty;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_getRootTransform);
#ifdef SKIP_ASM

/**
 * Returns the spline modifier's root transform, rebuilding it first when it is dirty.
 *
 * When the dirty flag at +0x58 is set, calls the virtual updateRootTransform() (slot 3).
 *
 * @return the address of the root transform matrix at +0x60
 */
const t4Matrix* cSplineModifier::getRootTransform()
{
    if (RootTransformDirty != 0) {
        updateRootTransform();
    }
    return &Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform type id of a spline modifier.
 *
 * @return always kTransform_Spline (multi-spline modifiers return kTransform_MultiSpline, roller modifiers
 *         kTransform_Roller)
 */
int cSplineModifier::getTransformType() const
{
    return cTransformModifier::kTransform_Spline;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns the multi-spline modifier's bounding box, held 0x30 bytes into its first spline segment.
 *
 * The segment array (0x60-byte elements) is the pointer at +0x3C, allocated by
 * cMultiSplineModifier_allocNodes().
 *
 * @return the address of the bound box (On Tour const tBoundBox*)
 */
const tBoundBox* cMultiSplineModifier::getBoundBox() const
{
    return &Nodes->BoundBox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_setBoundBox);
#ifdef SKIP_ASM
/**
 * Sets a multi-spline modifier's bounding box, stored in the record at +0x3C (+0x30 min, +0x40 max).
 *
 * @param boundbox  the new box (On Tour const tBoundBox&)
 */
void cMultiSplineModifier::setBoundBox(const tBoundBox& boundbox)
{
    tMultiNode* nodes = Nodes;
    nodes->BoundBox.MinPoint = boundbox.MinPoint;
    nodes->BoundBox.MaxPoint = boundbox.MaxPoint;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_getFocusCount);
#ifdef SKIP_ASM
/**
 * Returns a multi-spline modifier's FocusCount.
 *
 * @return FocusCount
 */
int cMultiSplineModifier::getFocusCount()
{
    return (FocusCount);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_getRadius__FPv);
#ifdef SKIP_ASM
/**
 * Returns the multi-spline modifier's radius, the float at +0x18.
 *
 * @return the radius
 */
float cMultiSplineModifier::getRadius() const
{
    return Radius;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_setRadius__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the multi-spline modifier's radius, the float at +0x18.
 *
 * @param val  the new radius
 */
void cMultiSplineModifier::setRadius(float val)
{
    Radius = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_getRootTransform1);
#ifdef SKIP_ASM

/**
 * Returns the root transform of the modifier's first instance, rebuilding the transforms first when dirty.
 *
 * When the dirty flag at +0x30 is set, calls the virtual updateRootTransform() (slot 3). The result is +0x10 of
 * the first entry in the instance array at +0x44 (an instance record's root transform).
 *
 * @return the address of the first instance's root transform matrix
 */
const t4Matrix* cMultiSplineModifier::getRootTransform1(tInstanceRecord*)
{
    if (RootTransformDirty != 0) {
        updateRootTransform();
    }
    return (const t4Matrix*)&NodeInst[0]->m_fmTransform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform type id of a multi-spline modifier.
 *
 * @return always kTransform_MultiSpline (spline modifiers return kTransform_Spline, roller modifiers
 *         kTransform_Roller)
 */
int cMultiSplineModifier::getTransformType() const
{
    return cTransformModifier::kTransform_MultiSpline;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_15cRollerModifier);
#ifdef SKIP_ASM
/**
 * Destroys a roller modifier; it owns nothing of its own, so its cTransformModifier part does the work and frees it
 * when asked.
 */
cRollerModifier::~cRollerModifier()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_shouldRebound__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a roller modifier should not rebound.
 *
 * @return always 0 (the base cTransformModifier returns 1)
 */
bool cRollerModifier::shouldRebound()
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_isComplete);
#ifdef SKIP_ASM
/**
 * Tells whether the rolling object has reached the end of its life.
 *
 * Compares Life with 17.0; On Tour names this limit kMaxRollerDeath.
 *
 * @return true when Life >= 17.0
 */
bool cRollerModifier::isComplete()
{
    return Life >= 17.0f;
}
#endif

extern "C" void* cRollerModifier_Collision(void*);

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_collide1__FPv);
#ifdef SKIP_ASM
/**
 * Handles a new collision on a roller: resets its Life (+0x20) to 0 and runs cRollerModifier_Collision().
 *
 * The collision info (On Tour collide(tc, ti)) arrives in the argument registers and passes through untouched.
 *
 * @return no meaningful value (cRollerModifier_Collision returns nothing)
 */
void* cRollerModifier::collide1()
{
    Life = 0.0f;
    return cRollerModifier_Collision(this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_runCollide__FPv);
#ifdef SKIP_ASM
/**
 * Reports that collision should not run for a roller modifier.
 *
 * @return always 0 (the base cTransformModifier returns 1)
 */
bool cRollerModifier::runCollide(int)
{
    return false;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_setBoundBox);
#ifdef SKIP_ASM
/**
 * Sets a roller modifier's bounding box.
 *
 * @param bb  the new box (On Tour const tBoundBox&)
 */
void cRollerModifier::setBoundBox(const tBoundBox& bb)
{
    boundbox.MinPoint = bb.MinPoint;
    boundbox.MaxPoint = bb.MaxPoint;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns the roller's bounding box, boundbox at +0x220.
 *
 * @return the address of the bound box
 */
const tBoundBox* cRollerModifier::getBoundBox() const
{
    return &((cRollerModifier*)this)->boundbox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_getRadius__FPv);
#ifdef SKIP_ASM
/**
 * Returns the roller's radius, Radius at +0x24.
 *
 * @return the radius
 */
float cRollerModifier::getRadius() const
{
    return Radius;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_setRadius__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the roller's radius, Radius at +0x24.
 *
 * @param val  the new radius
 */
void cRollerModifier::setRadius(float val)
{
    Radius = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_getRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Returns the roller's root transform, the matrix Transform at +0x240.
 *
 * @return the address of the transform matrix
 */
const t4Matrix* cRollerModifier::getRootTransform()
{
    return &((cRollerModifier*)this)->Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_getRootTransform1__FPv);
#ifdef SKIP_ASM
/**
 * Returns the roller's root transform for an instance; every instance gets the same matrix, Transform at
 * +0x240.
 *
 * On Tour's overload takes a tInstanceRecord*, which this body ignores.
 *
 * @return the address of the transform matrix
 */
const t4Matrix* cRollerModifier::getRootTransform1(tInstanceRecord*)
{
    return &((cRollerModifier*)this)->Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform type id of a roller modifier.
 *
 * @return always kTransform_Roller (spline modifiers return kTransform_Spline, multi-spline modifiers
 *         kTransform_MultiSpline)
 */
int cRollerModifier::getTransformType() const
{
    return cTransformModifier::kTransform_Roller;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D48);
#ifdef SKIP_ASM
/**
 * Returns the int at +0x10 of its argument. Nothing in retail calls it.
 *
 * @param object  the object to read (name ours)
 * @return that int
 */
extern "C" int func_00361D48(void *object) {
    return (*(int *)((char*)(object) + 0x10));
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", _$_13cParticleNode);
#ifdef SKIP_ASM
/**
 * Destroys a particle node; it owns nothing of its own, so its cMoveNode part does the work and frees it when asked.
 */
cParticleNode::~cParticleNode()
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailManNode_getSectionId);
#ifdef SKIP_ASM
/**
 * Returns the rail manager node's section id, which is always all bits set.
 *
 * @return always 0xFFFFFFFF
 */
extern "C" unsigned int cRailManNode_getSectionId()
{
    return 0xFFFFFFFFU;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimComboNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns an anim combo node's state block, which is the node itself.
 *
 * @return this node
 */
void* cAnimComboNode::getNodeState()
{
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimComboNode_getAnimDir);
#ifdef SKIP_ASM
/**
 * Returns an anim combo node's play direction from its active speed.
 *
 * While the second range is active (+0x14) the sign of its step (+0x0) decides, otherwise the sign of the base speed
 * (+0x28): 1 when >= 0, else -1.
 *
 * @return 1 or -1
 */
int cAnimComboNode::getAnimDir()
{
    int direction;
    if (AnimActive2 != 0) {
        direction = -1;
        if (AnimSpeed2 >= 0.0f) {
            direction = 1;
        }
        return direction;
    }
    direction = -1;
    if (AnimSpeed >= 0.0f) {
        direction = 1;
    }
    return direction;
}
#endif

extern void* cAnimDeltaNode_vtbl[];
extern "C" void* cAnimObjectNode__cAnimObjectNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", cAnimDeltaNode__cAnimDeltaNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys an anim delta node: reinstalls cAnimDeltaNode_vtbl (+0x3C) and runs the animated-object destructor cAnimObjectNode__cAnimObjectNode.
 *
 * @return cAnimObjectNode__cAnimObjectNode's result
 */
void* cAnimDeltaNode__cAnimDeltaNode(void* vself)
{
    cAnimDeltaNode* self = (cAnimDeltaNode*)vself;
    self->mVtbl = cAnimDeltaNode_vtbl;
    return cAnimObjectNode__cAnimObjectNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimDeltaNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns an anim delta node's state block, which is the node itself.
 *
 * @return this node
 */
void* cAnimDeltaNode::getNodeState()
{
    return ((void*)this);
}
#endif

extern "C" void* __sti__all_in_one_object_cpp(int, int);

//99.38%
INCLUDE_ASM("object/railmodifier", func_00361E10__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static initialisation: __sti__all_in_one_object_cpp with initialize 1 and priority 0xFFFF.
 *
 * The argument is not used.
 *
 * @return __sti__all_in_one_object_cpp's result
 */
void* func_00361E10(void* self)
{
    return __sti__all_in_one_object_cpp(1, 0xffff);
}
#endif

// One of the table's three interrupt counters (names ours).
struct tIntcCounter {
    int count;                      // 0x0 [U] the VIF1 handler (intc_handleVif1) increments the first one's
    int reserved;                   // 0x4 [R] tIntcHandlerTable_clearCounters zeroes it; nothing else reads or writes it
};

// The renderer's interrupt handler table (cPS2GraphicsMan_intcHandlerTable; struct name ours, the id tables' names are the preview build's
// bxps2intc.cpp asserts): the INTC handler ids by cause and the DMAC handler ids by channel, -1 when none, and three
// interrupt counters.
struct tIntcHandlerTable {
    int mIntCID[16];                // 0x00 [P] mIntCID; [R] tIntcHandlerTable_init fills -1; [U] tIntcHandlerTable_addIntcHandler / tIntcHandlerTable_removeIntcHandler
    int mDMACID[16];                // 0x40 [P] mDMACID; [R] tIntcHandlerTable_init fills -1; [U] tIntcHandlerTable_addDmacHandler / tIntcHandlerTable_removeDmacHandler
    tIntcCounter mCounters[3];      // 0x80 [R] tIntcHandlerTable_clearCounters zeroes them; [U] intc_handleVif1 counts into the first
};

//100%
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_installRenderHandlers);
#ifdef SKIP_ASM
extern "C" int intc_handleVif1(void);
extern "C" int intc_handleVu1(void);
extern "C" void tIntcHandlerTable_addIntcHandler(tIntcHandlerTable* self, int cause, int (*handler)(void), int arg);

/**
 * Installs the renderer's two INTC handlers: intc_handleVif1 for cause 5 (VIF1) and intc_handleVu1 for cause 7 (VU1).
 *
 * Both go through tIntcHandlerTable_addIntcHandler with a 0 argument, which records each handler id in the per-cause table at +0x0.
 */
extern "C" void tIntcHandlerTable_installRenderHandlers(tIntcHandlerTable* self)
{
    tIntcHandlerTable_addIntcHandler(self, 5, intc_handleVif1, 0);
    tIntcHandlerTable_addIntcHandler(self, 7, intc_handleVu1, 0);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_removeRenderHandlers);
#ifdef SKIP_ASM
extern "C" void* tIntcHandlerTable_removeIntcHandler(tIntcHandlerTable* self, int id);

/**
 * Removes the renderer's INTC handlers for causes 5 (VIF1) and 7 (VU1) with tIntcHandlerTable_removeIntcHandler.
 *
 * @return tIntcHandlerTable_removeIntcHandler's result for cause 7
 */
extern "C" void* tIntcHandlerTable_removeRenderHandlers(tIntcHandlerTable* self)
{
    tIntcHandlerTable_removeIntcHandler(self, 5);
    return tIntcHandlerTable_removeIntcHandler(self, 7);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_init);
#ifdef SKIP_ASM
extern "C" void tIntcHandlerTable_clearCounters(tIntcHandlerTable* self);
extern "C" void tIntcHandlerTable_installRenderHandlers(tIntcHandlerTable* self);

static inline void tIntcHandlerTable_init_fill(int* p, int v)
{
    int i;
    for (i = 15; i >= 0; i--) {
        p[i] = v;
    }
}

/**
 * Sets up the renderer's interrupt handler table (cPS2GraphicsMan_intcHandlerTable in cPS2GraphicsMan_Init).
 *
 * Fills the 16 INTC handler ids (+0x0) and the 16 DMAC handler ids (+0x40) with -1 (the preview build's bxps2intc.cpp
 * asserts call these mIntCID and mDMACID), clears the counters at +0x80 (tIntcHandlerTable_clearCounters) and installs the VIF1/VU1
 * handlers (tIntcHandlerTable_installRenderHandlers).
 */
extern "C" void tIntcHandlerTable_init(tIntcHandlerTable* self)
{
    tIntcHandlerTable_init_fill(self->mIntCID, -1);
    tIntcHandlerTable_init_fill(self->mDMACID, -1);
    tIntcHandlerTable_clearCounters(self);
    tIntcHandlerTable_installRenderHandlers(self);
}
#endif

extern "C" void* tIntcHandlerTable_removeRenderHandlers(tIntcHandlerTable* self);

//99.29%
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_shutdown__FPv);
#ifdef SKIP_ASM
/**
 * Tears down the renderer's interrupt handlers by removing the VIF1/VU1 ones (tIntcHandlerTable_removeRenderHandlers).
 *
 * cPS2GraphicsMan_Purge calls it on cPS2GraphicsMan_intcHandlerTable.
 *
 * @return tIntcHandlerTable_removeRenderHandlers's result
 */
void* tIntcHandlerTable_shutdown(void* self)
{
    return tIntcHandlerTable_removeRenderHandlers((tIntcHandlerTable*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_clearCounters);
#ifdef SKIP_ASM
struct sRailPair {
    int a;
    int b;
};

/**
 * Zeroes the three counter pairs at +0x80 of the interrupt handler table (the VIF1 handler counts into the first).
 */
extern "C" void tIntcHandlerTable_clearCounters(tIntcHandlerTable* self)
{
    int i;
    for (i = 0; i < 3; i++) {
        self->mCounters[i].count = 0;
        self->mCounters[i].reserved = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361F90__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing; cPS2GraphicsMan_DrawFrame calls it on the interrupt handler table (cPS2GraphicsMan_intcHandlerTable) at the end of a frame.
 */
void func_00361F90(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_addIntcHandler);
#ifdef SKIP_ASM
extern "C" int DisableIntc(int ch);
extern "C" int AddIntcHandler(int cause, int (*handler)(void), int arg);
extern "C" int EnableIntc(int ch);

/**
 * Registers an INTC handler for an interrupt cause and records its id.
 *
 * Calls DisableIntc(cause), stores the id AddIntcHandler(cause, handler, next) returns at +cause*4, then calls
 * EnableIntc(cause).
 *
 * @param cause  INTC cause number (5 = VIF1, 7 = VU1)
 * @param handler  handler function
 * @param next  argument passed through to AddIntcHandler
 */
extern "C" void tIntcHandlerTable_addIntcHandler(tIntcHandlerTable* self, int cause, int (*handler)(void), int next)
{
    DisableIntc(cause);
    self->mIntCID[cause] = AddIntcHandler(cause, handler, next);
    EnableIntc(cause);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_removeIntcHandler);
#ifdef SKIP_ASM
extern "C" int DisableIntc(int ch);
extern "C" void* RemoveIntcHandler(int ch, int id);

/**
 * Removes the INTC handler recorded for a cause.
 *
 * Calls DisableIntc(ch), sets the cause's slot (+ch*4) to -1 and calls RemoveIntcHandler with the old id.
 *
 * @param ch  INTC cause number
 * @return RemoveIntcHandler's result
 */
extern "C" void* tIntcHandlerTable_removeIntcHandler(tIntcHandlerTable* self, int ch)
{
    DisableIntc(ch);
    int* handlerSlot = &self->mIntCID[ch];
    int id = *handlerSlot;
    *handlerSlot = -1;
    return RemoveIntcHandler(ch, id);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_addDmacHandler);
#ifdef SKIP_ASM
extern "C" int AddDmacHandler(int, int, int);
extern "C" void DisableDmac(int);
extern "C" void EnableDmac(int);

/**
 * Registers a DMAC handler for a channel and records its id.
 *
 * Calls DisableDmac(channel), stores the id AddDmacHandler(channel, handler, next) returns at +0x40 + channel*4, then
 * calls EnableDmac(channel).
 *
 * @param owner  the interrupt handler table
 * @param channel  DMA channel number
 * @param handler  handler function
 * @param next  argument passed through to AddDmacHandler
 */
extern "C" void tIntcHandlerTable_addDmacHandler(tIntcHandlerTable* owner, int channel, int handler, int next)
{
    DisableDmac(channel);
    owner->mDMACID[channel] = AddDmacHandler(channel, handler, next);
    EnableDmac(channel);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", tIntcHandlerTable_removeDmacHandler);
#ifdef SKIP_ASM
extern "C" void RemoveDmacHandler(int, int);

/**
 * Removes the DMAC handler recorded for a channel.
 *
 * Calls DisableDmac(idx), sets the channel's slot (+0x40 + idx*4) to -1 and calls RemoveDmacHandler with the old id.
 *
 * @param idx  DMA channel number
 */
extern "C" void tIntcHandlerTable_removeDmacHandler(tIntcHandlerTable* self, int idx)
{
    DisableDmac(idx);
    int* ids = self->mDMACID;
    int* slot = ids + idx;
    int old = *slot;
    *slot = -1;
    RemoveDmacHandler(idx, old);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", saveFpuRegisters);
#ifdef SKIP_ASM
/**
 * Stores all 32 FPU registers into buf as 64-bit words (the interrupt handlers' save half; restoreFpuRegisters restores).
 *
 * @param buf  0x100-byte save area
 */
// PORT: PS2-only inline asm (dump all 32 FPU registers as 64-bit words into buf).
extern "C" void saveFpuRegisters(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "por       $2, $0, $0\n"
        "mfc1      $2, $f0\n"
        "sd        $2, 0x0(%0)\n"
        "mfc1      $2, $f1\n"
        "sd        $2, 0x8(%0)\n"
        "mfc1      $2, $f2\n"
        "sd        $2, 0x10(%0)\n"
        "mfc1      $2, $f3\n"
        "sd        $2, 0x18(%0)\n"
        "mfc1      $2, $f4\n"
        "sd        $2, 0x20(%0)\n"
        "mfc1      $2, $f5\n"
        "sd        $2, 0x28(%0)\n"
        "mfc1      $2, $f6\n"
        "sd        $2, 0x30(%0)\n"
        "mfc1      $2, $f7\n"
        "sd        $2, 0x38(%0)\n"
        "mfc1      $2, $f8\n"
        "sd        $2, 0x40(%0)\n"
        "mfc1      $2, $f9\n"
        "sd        $2, 0x48(%0)\n"
        "mfc1      $2, $f10\n"
        "sd        $2, 0x50(%0)\n"
        "mfc1      $2, $f11\n"
        "sd        $2, 0x58(%0)\n"
        "mfc1      $2, $f12\n"
        "sd        $2, 0x60(%0)\n"
        "mfc1      $2, $f13\n"
        "sd        $2, 0x68(%0)\n"
        "mfc1      $2, $f14\n"
        "sd        $2, 0x70(%0)\n"
        "mfc1      $2, $f15\n"
        "sd        $2, 0x78(%0)\n"
        "mfc1      $2, $f16\n"
        "sd        $2, 0x80(%0)\n"
        "mfc1      $2, $f17\n"
        "sd        $2, 0x88(%0)\n"
        "mfc1      $2, $f18\n"
        "sd        $2, 0x90(%0)\n"
        "mfc1      $2, $f19\n"
        "sd        $2, 0x98(%0)\n"
        "mfc1      $2, $f20\n"
        "sd        $2, 0xA0(%0)\n"
        "mfc1      $2, $f21\n"
        "sd        $2, 0xA8(%0)\n"
        "mfc1      $2, $f22\n"
        "sd        $2, 0xB0(%0)\n"
        "mfc1      $2, $f23\n"
        "sd        $2, 0xB8(%0)\n"
        "mfc1      $2, $f24\n"
        "sd        $2, 0xC0(%0)\n"
        "mfc1      $2, $f25\n"
        "sd        $2, 0xC8(%0)\n"
        "mfc1      $2, $f26\n"
        "sd        $2, 0xD0(%0)\n"
        "mfc1      $2, $f27\n"
        "sd        $2, 0xD8(%0)\n"
        "mfc1      $2, $f28\n"
        "sd        $2, 0xE0(%0)\n"
        "mfc1      $2, $f29\n"
        "sd        $2, 0xE8(%0)\n"
        "mfc1      $2, $f30\n"
        "sd        $2, 0xF0(%0)\n"
        "mfc1      $2, $f31\n"
        "sd        $2, 0xF8(%0)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$2", "memory");
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", restoreFpuRegisters);
#ifdef SKIP_ASM
/**
 * Reloads all 32 FPU registers from buf, as saved by saveFpuRegisters.
 *
 * @param buf  0x100-byte save area
 */
// PORT: PS2-only inline asm (reload all 32 FPU registers from buf; deliberately
// no FPU clobbers so the compiler doesn't save/restore $f20-$f31 around it).
extern "C" void restoreFpuRegisters(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "por       $2, $0, $0\n"
        "ld        $2, 0x0(%0)\n"
        "mtc1      $2, $f0\n"
        "ld        $2, 0x8(%0)\n"
        "mtc1      $2, $f1\n"
        "ld        $2, 0x10(%0)\n"
        "mtc1      $2, $f2\n"
        "ld        $2, 0x18(%0)\n"
        "mtc1      $2, $f3\n"
        "ld        $2, 0x20(%0)\n"
        "mtc1      $2, $f4\n"
        "ld        $2, 0x28(%0)\n"
        "mtc1      $2, $f5\n"
        "ld        $2, 0x30(%0)\n"
        "mtc1      $2, $f6\n"
        "ld        $2, 0x38(%0)\n"
        "mtc1      $2, $f7\n"
        "ld        $2, 0x40(%0)\n"
        "mtc1      $2, $f8\n"
        "ld        $2, 0x48(%0)\n"
        "mtc1      $2, $f9\n"
        "ld        $2, 0x50(%0)\n"
        "mtc1      $2, $f10\n"
        "ld        $2, 0x58(%0)\n"
        "mtc1      $2, $f11\n"
        "ld        $2, 0x60(%0)\n"
        "mtc1      $2, $f12\n"
        "ld        $2, 0x68(%0)\n"
        "mtc1      $2, $f13\n"
        "ld        $2, 0x70(%0)\n"
        "mtc1      $2, $f14\n"
        "ld        $2, 0x78(%0)\n"
        "mtc1      $2, $f15\n"
        "ld        $2, 0x80(%0)\n"
        "mtc1      $2, $f16\n"
        "ld        $2, 0x88(%0)\n"
        "mtc1      $2, $f17\n"
        "ld        $2, 0x90(%0)\n"
        "mtc1      $2, $f18\n"
        "ld        $2, 0x98(%0)\n"
        "mtc1      $2, $f19\n"
        "ld        $2, 0xA0(%0)\n"
        "mtc1      $2, $f20\n"
        "ld        $2, 0xA8(%0)\n"
        "mtc1      $2, $f21\n"
        "ld        $2, 0xB0(%0)\n"
        "mtc1      $2, $f22\n"
        "ld        $2, 0xB8(%0)\n"
        "mtc1      $2, $f23\n"
        "ld        $2, 0xC0(%0)\n"
        "mtc1      $2, $f24\n"
        "ld        $2, 0xC8(%0)\n"
        "mtc1      $2, $f25\n"
        "ld        $2, 0xD0(%0)\n"
        "mtc1      $2, $f26\n"
        "ld        $2, 0xD8(%0)\n"
        "mtc1      $2, $f27\n"
        "ld        $2, 0xE0(%0)\n"
        "mtc1      $2, $f28\n"
        "ld        $2, 0xE8(%0)\n"
        "mtc1      $2, $f29\n"
        "ld        $2, 0xF0(%0)\n"
        "mtc1      $2, $f30\n"
        "ld        $2, 0xF8(%0)\n"
        "mtc1      $2, $f31\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$2", "memory");
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", intc_handleVif1);
#ifdef SKIP_ASM
extern "C" void saveFpuRegisters(void*);
extern "C" void restoreFpuRegisters(void*);

extern tIntcHandlerTable cPS2GraphicsMan_intcHandlerTable;

/**
 * Handles the VIF1 interrupt: cancels a VIF1 interrupt stall and counts it.
 *
 * Saves the FPU registers (saveFpuRegisters). When VIF1_STAT (0x10003C00) has bit 10 set (stalled on an interrupt) it
 * writes 8 (STC, stall cancel) to VIF1_FBRST (0x10003C10). It counts the interrupt at cPS2GraphicsMan_intcHandlerTable +0x80, restores the
 * FPU registers (restoreFpuRegisters) and re-enables interrupts.
 *
 * @return 1 (handled)
 */
extern "C" int intc_handleVif1(void)
{
    char buf[0x100];
    saveFpuRegisters(buf);
    tIntcHandlerTable* table = &cPS2GraphicsMan_intcHandlerTable;
    if (*(int*)0x10003C00 & 0x400) {
        *(volatile int*)0x10003C10 = 8;
    }
    table->mCounters[0].count++;
    restoreFpuRegisters(buf);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", intc_handleVu1);
#ifdef SKIP_ASM
extern "C" void saveFpuRegisters(void*);
extern "C" void restoreFpuRegisters(void*);

/**
 * Handles the VU1 interrupt: only saves and restores the FPU registers and re-enables interrupts.
 *
 * @return 1 (handled)
 */
extern "C" int intc_handleVu1(void)
{
    char buf[0x100];
    saveFpuRegisters(buf);
    restoreFpuRegisters(buf);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", gsPacket_addScissor);
#ifdef SKIP_ASM
struct sGifPacket {
    int qwc;        // 0x0
    ulong* cur;     // 0x4
};

/**
 * Appends SCISSOR_1 and SCISSOR_2 GS register writes (A+D pairs) for the same rectangle to a packet.
 *
 * Both get x .. x + w - 1 and y .. y + height - 1; the packet advances by four 64-bit words and its count by 2.
 *
 * @param pkt  packet being built
 * @param x  left edge
 * @param w  width
 * @param y  top edge
 * @param height  height
 */
// PORT: 64-bit GS register writes (ulong).
extern "C" void gsPacket_addScissor(sGifPacket* pkt, int x, int w, int y, int height)
{
    ulong scissor = (ulong)x | ((ulong)(x + (w - 1)) << 16) | ((ulong)y << 32) | ((ulong)(y + (height - 1)) << 48);
    pkt->cur[0] = scissor;
    pkt->cur[1] = 0x40;
    pkt->cur[2] = scissor;
    pkt->cur[3] = 65;
    pkt->cur += 4;
    pkt->qwc += 2;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", gsPacket_addTexFlush);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sPair64 {
    long a;
    long b;
};

/**
 * Appends a TEXFLUSH GS register write (A+D pair, register 0x3F, data 0) to a packet and counts it.
 */
extern "C" void gsPacket_addTexFlush(void* self)
{
    sPair64*& cur = *(sPair64**)((char*)self + 0x4);
    cur->a = 0;
    cur->b = 0x3F;
    cur = cur + 1;
    *(int*)((char*)self + 0x0) += 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", gsPacket_addAlpha);
#ifdef SKIP_ASM
/**
 * Appends an ALPHA blend register write (A+D pair) for a blend mode to a packet.
 *
 * Context 1 selects ALPHA_2 (0x43), anything else ALPHA_1 (0x42). Each mode has a fixed register value; modes 11-14 put
 * fix in the FIX field and mode 2 uses FIX 0x80.
 *
 * Quirk: modes 15 and 18-20 (and any value not listed) leave the data word as it was, but the entry is still added and
 * counted.
 *
 * @param pkt  packet being built
 * @param mode  blend mode
 * @param fix  fixed alpha for modes 11-14
 * @param ctx  GS context (0 or 1)
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: 64-bit GS register writes (ulong).
// Emits one GS ALPHA register write (0x42 for context 1, 0x43 for context 2) for the given blend mode.
extern "C" void gsPacket_addAlpha(sGifPacket* pkt, int mode, int fix, int ctx)
{
    switch (ctx) {
    case 0:
        pkt->cur[1] = 66;
        break;
    case 1:
        pkt->cur[1] = 67;
        break;
    default:
        pkt->cur[1] = 66;
        break;
    }
    switch (mode) {
    case 0: pkt->cur[0] = 170; break;
    case 1: pkt->cur[0] = 42; break;
    case 5: pkt->cur[0] = 68; break;
    case 6: pkt->cur[0] = 70; break;
    case 7: pkt->cur[0] = 72; break;
    case 8: pkt->cur[0] = 129; break;
    case 9: pkt->cur[0] = 66; break;
    case 4: pkt->cur[0] = 137; break;
    case 3: pkt->cur[0] = 106; break;
    case 11: pkt->cur[0] = ((ulong)fix << 32) | 0x64; break;
    case 12: pkt->cur[0] = ((ulong)fix << 32) | 0x68; break;
    case 13:
    case 14: pkt->cur[0] = ((ulong)fix << 32) | 0x88; break;
    case 2: pkt->cur[0] = ((ulong)0x80 << 32) | 0x68; break;
    case 16: pkt->cur[0] = 152; break;
    case 17: pkt->cur[0] = 88; break;
    case 10:
    case 21: pkt->cur[0] = 73; break;
    case 15:
    case 18:
    case 19:
    case 20:
        break;
    }
    pkt->cur += 2;
    pkt->qwc++;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", gsPacket_addClamp);
#ifdef SKIP_ASM
/**
 * Appends a CLAMP register write (A+D pair) for a texture wrap mode to a packet.
 *
 * Context 1 selects CLAMP_2 (9), anything else CLAMP_1 (8). Mode 3 clamps both axes (5), mode 1 clamps T only (4), mode
 * 2 clamps S only (1), anything else repeats both (0).
 *
 * @param pkt  packet being built
 * @param mode  wrap mode
 * @param ctx  GS context (0 or 1)
 */
// PORT: 64-bit GS register writes (ulong).
extern "C" void gsPacket_addClamp(sGifPacket* pkt, int mode, int ctx)
{
    ulong reg;
    switch (ctx) {
    case 0:
        reg = 8;
        break;
    case 1:
        reg = 9;
        break;
    default:
        reg = 8;
        break;
    }
    switch (mode) {
    case 3:
        pkt->cur[0] = 5;
        break;
    case 1:
        pkt->cur[0] = 4;
        break;
    case 2:
        pkt->cur[0] = 1;
        break;
    default:
        pkt->cur[0] = 0;
        break;
    }
    pkt->cur[1] = reg;
    pkt->cur += 2;
    pkt->qwc++;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", gsPacket_addZBuf);
#ifdef SKIP_ASM
/**
 * Appends a ZBUF register write (A+D pair) to a packet.
 *
 * Context 1 selects ZBUF_2 (0x4F), anything else ZBUF_1 (0x4E). The value holds zbp, the Z format (psm - 0x30) at bit
 * 24 and ZMSK (bit 32) when zmsk is 1.
 *
 * @param pkt  packet being built
 * @param zbp  Z buffer base pointer
 * @param psm  Z pixel format (0x30-0x3A)
 * @param zmsk  1 to mask Z writes
 * @param ctx  GS context (0 or 1)
 */
// PORT: 64-bit GS register writes (ulong).
extern "C" void gsPacket_addZBuf(sGifPacket* pkt, unsigned int zbp, unsigned int psm, int zmsk, int ctx)
{
    ulong reg;
    switch (ctx) {
    case 0:
        reg = 78;
        break;
    case 1:
        reg = 79;
        break;
    default:
        reg = 78;
        break;
    }
    pkt->cur[0] = (ulong)zbp | ((ulong)(psm - 0x30) << 24) | ((zmsk == 1) ? ((ulong)1 << 32) : 0);
    pkt->cur[1] = reg;
    pkt->cur += 2;
    pkt->qwc++;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", gsPacket_addTest);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sRmGsPacket {
    int count;      // 0x0
    ulong* ptr;     // 0x4
};

/**
 * Appends a TEST register write (A+D pair) for an alpha and depth test to a packet.
 *
 * Context 1 selects TEST_2 (0x48), anything else TEST_1 (0x47). Alpha test is on with ATST ALWAYS for atest 0/1 (and
 * unknown values), LESS for 2 and GREATER for 3, reference aref, AFAIL 1 (keep frame buffer only); depth test is on
 * with ZTST GEQUAL for ztest 1, GREATER for 0 and ALWAYS otherwise.
 *
 * @param pkt  packet being built
 * @param ztest  depth test mode
 * @param atest  alpha test mode
 * @param aref  alpha reference
 * @param ctx  GS context (0 or 1)
 */
extern "C" void gsPacket_addTest(sRmGsPacket* pkt, int ztest, int atest, unsigned int aref, int ctx)
{
    ulong reg;
    int atst;
    int ztst;

    switch (ctx) {
    case 0:
        reg = 71;
        break;
    case 1:
        reg = 72;
        break;
    default:
        reg = 71;
        break;
    }

    switch (atest) {
    case 0:
    case 1:
        atst = 1;
        break;
    case 2:
        atst = 2;
        break;
    case 3:
        atst = 6;
        break;
    default:
        atst = 1;
        break;
    }

    ztst = 1;
    switch (ztest) {
    case 1:
        ztst = 2;
        break;
    case 0:
        ztst = 3;
        break;
    case 2:
        break;
    }

    pkt->ptr[0] = (ulong)1 | ((ulong)atst << 1) | ((ulong)aref << 4) | ((ulong)1 << 12)
                | ((ulong)0 << 14) | ((ulong)0 << 15) | ((ulong)1 << 16) | ((ulong)ztst << 17);
    pkt->ptr[1] = reg;
    pkt->ptr += 2;
    pkt->count++;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", gsPacket_writeHeader);
#ifdef SKIP_ASM
extern ulong gsPacket_headerBuffer[];

struct sRmPkt27A8 {
    int count;      // 0x0
    ulong* cur;     // 0x4
    ulong* base;    // 0x8
};

/**
 * Writes the DMA/VIF/GIF header in front of a block of count A+D register writes; does nothing when count is 0.
 *
 * The header is six 64-bit words: a DMA tag for count + 2 quadwords, a VIF NOP, a VIF DIRECT of count + 1 quadwords
 * (mode 2: an UNPACK V4-32 to VU1 address 0x3F2 instead) and a GIF tag (NLOOP count, EOP, one A+D register). Mode 0
 * builds it at gsPacket_headerBuffer with a RET tag and appends a CALL to gsPacket_headerBuffer at *pp; modes 1 and 2 write it at the packet
 * base with a CNT tag after handing the packet's current end back through *pp.
 *
 * @param pp  the caller's DMA chain cursor
 * @param mode  0, 1 or 2 as above
 */
// PORT: 64-bit DMA/VIF/GIF tag writes (ulong) and addresses held in int.
extern "C" void gsPacket_writeHeader(sRmPkt27A8* self, ulong** pp, int mode)
{
    if (self->count == 0) {
        return;
    }
    switch (mode) {
    case 0:
        self->cur = gsPacket_headerBuffer;
        self->cur[0] = (self->count + 2) | 0x60000000;
        self->cur[1] = 0;
        self->cur[2] = 0;
        self->cur[3] = (ulong)(unsigned int)((self->count + 1) | 0x50000000) << 32;
        self->cur[4] = (ulong)(self->count | 0x8000) | ((ulong)1 << 60);
        self->cur[5] = 0xE;
        self->cur += 6;
        (*pp)[0] = ((ulong)(unsigned int)gsPacket_headerBuffer << 32) | 0x50000000;
        (*pp)[1] = 0;
        *pp += 2;
        break;
    case 1:
        *pp = self->cur;
        self->cur = self->base;
        self->cur[0] = (self->count + 2) | 0x10000000;
        self->cur[1] = 0;
        self->cur[2] = 0;
        self->cur[3] = (ulong)(unsigned int)((self->count + 1) | 0x50000000) << 32;
        self->cur[4] = (ulong)(self->count | 0x8000) | ((ulong)1 << 60);
        self->cur[5] = 0xE;
        self->cur += 6;
        break;
    case 2:
        *pp = self->cur;
        self->cur = self->base;
        self->cur[0] = (self->count + 2) | 0x10000000;
        self->cur[1] = 0;
        self->cur[2] = 0;
        self->cur[3] = (ulong)(unsigned int)(((self->count + 1) << 16) | 0x6C0003F2) << 32;
        self->cur[4] = (ulong)(self->count | 0x8000) | ((ulong)1 << 60);
        self->cur[5] = 0xE;
        self->cur += 6;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", tDeferBucket_appendPacket);
#ifdef SKIP_ASM
/**
 * Appends a DMA packet to a draw bucket's chain.
 *
 * The first packet becomes the head (+0xC); later ones are linked by turning the previous tail's tag (through the
 * uncached-accelerated mirror, | 0x30000000) into a NEXT tag to the new packet. The tail (+0x10) becomes tail.
 *
 * @param packet  start of the packet
 * @param tail  address of its closing tag
 */
// PORT: 64-bit DMA tag writes (ulong) and addresses held in int.
void tDeferBucket::appendPacket(int packet, int tail)
{
    if (mPacket != 0) {
        ulong* tag = (ulong*)(mTail | 0x30000000);
        tag[0] = ((ulong)packet << 32) | 0x20000000;
        tag[1] = 0;
        mTail = tail;
    } else {
        mPacket = packet;
        mTail = tail;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_buildPresetStates);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cPS2DeferManager_resetFrame(cPS2DeferManager* self);





extern cPS2GraphicsMan* cGraphicsMan_TheGraphicsMan_29B8 __asm__("cGraphicsMan_TheGraphicsMan");
extern sDrawState sDrawState_s_defaultDrawState_29B8 __asm__("sDrawState_s_defaultDrawState");

static inline void set0_29B8(cPS2GraphicsMan* ctx, unsigned int mask, int shift, unsigned int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[0] = (ctx->m_pCurrentDrawState->m_bitGroups[0] & ~mask) | ((v << shift) & mask);
}

static inline void set1_29B8(cPS2GraphicsMan* ctx, unsigned int mask, int shift, unsigned int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[1] = (ctx->m_pCurrentDrawState->m_bitGroups[1] & ~mask) | ((v << shift) & mask);
}

static inline void set2_29B8(cPS2GraphicsMan* ctx, unsigned int mask, int shift, unsigned int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[2] = (ctx->m_pCurrentDrawState->m_bitGroups[2] & ~mask) | ((v << shift) & mask);
}

static inline cDrawStateList* save_29B8(cPS2DeferManager* self, cPS2GraphicsMan* ctx)
{
    sDrawState* src = ctx->m_pCurrentDrawState;
    cDrawStateList* d = &((&self->mDrawStates[0]))[self->mUsedDrawStates++];
    (*(&d->mDrawState)) = *src;
    return d;
}

/**
 * Builds the defer manager's three preset draw states.
 *
 * Clears the static base (+0x57484) and resets the manager (cPS2DeferManager_resetFrame). Starting from sDrawState_s_defaultDrawState
 * in the current draw-state slot of cGraphicsMan_TheGraphicsMan, it edits the state bit fields three times and saves
 * each copy into the draw-state array (+0x51484, count +0x51480): the presets at +0x57488, +0x5748C and +0x57490
 * (mpPresetStates). The base is then set to the count so later resets keep them.
 */
void cPS2DeferManager::buildPresetStates()
{
    mNumStaticStates = 0;
    resetFrame();
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan_29B8;
    *ctx->m_pCurrentDrawState = sDrawState_s_defaultDrawState_29B8;
    set1_29B8(ctx, 0x3, 0, 0);
    set2_29B8(ctx, 0x1FFFFC00, 10, 0);
    set1_29B8(ctx, 0x1800000, 23, 1);
    set1_29B8(ctx, 0x300000, 20, 0);
    set1_29B8(ctx, 0xFF000, 12, 20);
    set0_29B8(ctx, 0x30, 4, 3);
    set0_29B8(ctx, 0x3C0, 6, 3);
    set1_29B8(ctx, 0x7C, 2, 1);
    set1_29B8(ctx, 0xF80, 7, 17);
    mpPresetStates[0] = (cDrawStateList*)save_29B8((this), ctx);
    set1_29B8(ctx, 0xF80, 7, 2);
    mpPresetStates[1] = (cDrawStateList*)save_29B8((this), ctx);
    set1_29B8(ctx, 0x7C, 2, 5);
    set1_29B8(ctx, 0xF80, 7, 3);
    set1_29B8(ctx, 0x3, 0, 1);
    set2_29B8(ctx, 0x1FFFFC00, 10, 0);
    set1_29B8(ctx, 0x1800000, 23, 1);
    set1_29B8(ctx, 0x300000, 20, 3);
    set1_29B8(ctx, 0xFF000, 12, 92);
    mpPresetStates[2] = (cDrawStateList*)save_29B8((this), ctx);
    mNumStaticStates = mUsedDrawStates;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_resetFrame);
#ifdef SKIP_ASM
extern "C" void InvalidDCache(void* begin, void* end);
extern "C" void scratchpad_storeSpareArea(void* base, int size, int mode);
extern "C" void MEM_fill(void* dst, int c, int n);
extern void* cPS2ScratchPad_freeSpace;

struct sRail20_2CC8 {
    int v[5];
};

extern sRail20_2CC8 sDrawState_s_defaultDrawState;

/**
 * Resets the defer manager for a new frame.
 *
 * Zeroes the record count (+0x0) and the bucket count (+0x57494), drops the draw states saved after the presets
 * (+0x51480 = +0x57484), clears the two 0x400-byte hash tables at +0x674A0 and +0x678A0 by DMA from the zero-filled
 * scratchpad (cPS2ScratchPad_freeSpace), zeroes the sort-item count (+0x67CA4) and the scissor count (+0x6AED0), sets the current
 * draw state (+0x69CA8) to sDrawState_s_defaultDrawState, and the current matrix (+0x69CC0) and scissor (+0x69CC4) to
 * -1 with no cached matrices (+0x69CC8).
 */
void cPS2DeferManager::resetFrame()
{
    mNumSubmitDMA = 0;
    mUsedStates = 0;
    mUsedDrawStates = mNumStaticStates;
    char* stateHashTable = (char*)&mStateHash[0];
    InvalidDCache(stateHashTable, ((char*)this) + 0x6789F);
    char* nodeHashTable = (char*)&mBucketHash[0];
    InvalidDCache(nodeHashTable, ((char*)this) + 0x67C9F);
    MEM_fill(cPS2ScratchPad_freeSpace, 0, 0x400);
    scratchpad_storeSpareArea(stateHashTable, 0x400, 2);
    scratchpad_storeSpareArea(nodeHashTable, 0x400, 2);
    mNumSortItems = 0;
    sRail20_2CC8* currentState = (sRail20_2CC8*)(&mCurrentState);
    *currentState = sDrawState_s_defaultDrawState;
    mIndexPort = 0;
    mActiveViewIndex = -1;
    mIndexView = 0;
    mActivePortIndex = -1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_bucketRecords);
#ifdef SKIP_ASM
extern void* SPRMan;
extern "C" void SyncDCache(void* begin, void* end);
extern "C" void* cPS2ScratchPad_claimAndLoadBuffer(void* ring, void* src, int size, int flags);
extern "C" void cPS2ScratchPad_advanceBuffer(void* ring);
extern "C" void dma_waitToScratchpadIdle();
extern "C" unsigned int sDrawState_makeSortKey(unsigned int* self, int key);


// A bucket: one (state, textures, matrix) combination with its DMA chain (0x20; REF structs/render.md "Bucket").
struct sRmNode_2DE8 {
    unsigned int* state;    // 0x0
    int key;                // 0x4  textures (short[2])
    int mtxIndex;           // 0x8
    int packet;             // 0xC  first packet start (CALL target)
    int tail;               // 0x10 last packet end
    int linked;             // 0x14 chained flag
    sRmNode_2DE8* link;     // 0x18 next in chain
    sRmNode_2DE8* next;     // 0x1C hash next
};

struct sRmSortItem_2DE8 {
    unsigned int key;
    sRmNode_2DE8* node;
};

struct sRmBatch_2DE8 {
    int count;                          // 0x0
    char pad_0x4[0x7C];
    tDeferSubmit entries[0xA29];       // 0x80
    char pad_0x51500[0x57494 - 0x51500];
    int nNodes;                         // 0x57494
    sRmNode_2DE8 nodes[0x800];          // 0x57498
    sRmNode_2DE8* stateHash[0x100];     // 0x67498 (unused here)
    char pad_0x67898[0x678A0 - 0x67898];
    sRmNode_2DE8* nodeHash[0x100];      // 0x678A0
    sRmNode_2DE8* last;                 // 0x67CA0
    int nItems;                         // 0x67CA4
    sRmSortItem_2DE8 items[1];          // 0x67CA8
};

// REF n2-render-layer.md: InternState, FindBucket, AppendPacket (0x362978).
extern "C" unsigned int* cPS2DeferManager_Find(sRmBatch_2DE8* self, tDeferSubmit* e, int hash);
extern "C" sRmNode_2DE8* cPS2DeferManager_Find1(sRmBatch_2DE8* self, unsigned int* state, int key, int hash, int mtxIndex);
extern "C" void tDeferBucket_appendPacket(tDeferBucket* self, int packet, int tail);

union sRmTexKey_2DE8 {
    int i;
    struct { short u; short v; } s;
    void set(short u_, short v_) { s.u = u_; s.v = v_; }
};

static inline int hashPart_2DE8(int x, int s)
{
    return (((x >> s) << 5) + x) >> s;
}

static inline int hash8_2DE8(int x)
{
    return (hashPart_2DE8(x, 0) ^ hashPart_2DE8(x, 8) ^ hashPart_2DE8(x, 16) ^ hashPart_2DE8(x, 24)) & 0xFF;
}

static inline int nodeHash_2DE8(unsigned int* state, int key)
{
    int lo = (short)key;
    int hi = key >> 16;
    return hash8_2DE8((int)state ^ (lo | ((hi + 2) << 16)));
}

static inline sRmNode_2DE8* newNode_2DE8(sRmBatch_2DE8* self, unsigned int* state, int key, int mtxIndex)
{
    sRmNode_2DE8* n = &self->nodes[self->nNodes++];
    n->state = state;
    n->key = key;
    n->mtxIndex = mtxIndex;
    n->packet = 0;
    n->linked = 0;
    n->link = 0;
    return n;
}

static inline unsigned int* findState_2DE8(sRmBatch_2DE8* self, tDeferSubmit* e)
{
    int x = e->mDrawState.m_bitGroups[0] ^ e->mDrawState.m_bitGroups[1] ^ e->mDrawState.m_bitGroups[2] ^ e->mDrawState.m_bitGroups[3];
    return cPS2DeferManager_Find(self, e, hash8_2DE8(x));
}

static inline void addEntry_2DE8(sRmBatch_2DE8* self, tDeferSubmit* e, unsigned int* state, int key)
{
    sRmNode_2DE8* node;
    if (e->mContinuation != 0)
        node = 0;
    else
        node = cPS2DeferManager_Find1(self, state, key, nodeHash_2DE8(state, key), e->mViewIndex);
    if (node == 0) {
        sRmNode_2DE8* n = newNode_2DE8(self, state, key, e->mViewIndex);
        node = n;
        if (e->mContinuation != 0 && self->last != 0) {
            node->linked = 1;
        } else {
            unsigned int sortKey = sDrawState_makeSortKey(state, key);
            int h = nodeHash_2DE8(state, key);
            n->next = self->nodeHash[h];
            self->nodeHash[h] = n;
            self->items[self->nItems].key = sortKey;
            self->items[self->nItems++].node = n;
        }
    }
    ((tDeferBucket*)node)->appendPacket(e->mPacket, e->mTail);
    if (e->mContinuation != 0) {
        sRmNode_2DE8* prev = self->last;
        if (prev != 0) {
            prev->link = node;
            prev->linked = 1;
        }
    }
    self->last = node;
}

/**
 * Files the frame's deferred draw records into buckets by draw state, textures and matrix.
 *
 * Streams the 0x80-byte records (+0x80, count +0x0) through the scratchpad ring SPRMan in chunks. For each it finds
 * the shared state (cPS2DeferManager_Find) and, unless it continues the previous record, the bucket with the same state,
 * texture pair and matrix index (cPS2DeferManager_Find1). A new bucket is added to the hash and gets a sort item with the key
 * from sDrawState_makeSortKey; continuation records are chained to the previous bucket. The record's packet is appended to the
 * bucket with tDeferBucket_appendPacket.
 *
 * @param vself  the defer manager
 */
// PORT: (int) of a pointer for hashing.
void cPS2DeferManager::bucketRecords()
{
    sRmBatch_2DE8* self = (sRmBatch_2DE8*)this;
    self->last = 0;
    SyncDCache(self->entries, (char*)&self->entries[self->count] - 1);
    void* ring = SPRMan;
    int bytes = *(int*)((char*)ring + 0x10) << 4;
    int chunk = bytes / sizeof(tDeferSubmit);
    int nChunks = self->count / chunk;
    int chunkBytes = chunk * sizeof(tDeferSubmit);
    tDeferSubmit* buf = (tDeferSubmit*)cPS2ScratchPad_claimAndLoadBuffer(ring, self->entries, chunkBytes, 4);
    for (int i = 0; i < nChunks; i++) {
        tDeferSubmit* next = (tDeferSubmit*)cPS2ScratchPad_claimAndLoadBuffer(ring, &self->entries[(i + 1) * chunk], chunkBytes, 4);
        for (int j = 0; j < chunk; j++) {
            tDeferSubmit* drawRecord = &buf[j];
            unsigned int* state = findState_2DE8(self, drawRecord);
            sRmTexKey_2DE8 key;
            key.set(drawRecord->mDrawState.m_textureState.m_texid[0], drawRecord->mDrawState.m_textureState.m_texid[1]);
            addEntry_2DE8(self, drawRecord, state, key.i);
        }
        buf = next;
        cPS2ScratchPad_advanceBuffer(ring);
        dma_waitToScratchpadIdle();
    }
    dma_waitToScratchpadIdle();
    for (int j = 0; j < self->count - chunk * nChunks; j++) {
        tDeferSubmit* drawRecord = &buf[j];
        unsigned int* state = findState_2DE8(self, drawRecord);
        sRmTexKey_2DE8 key;
        key.set(drawRecord->mDrawState.m_textureState.m_texid[0], drawRecord->mDrawState.m_textureState.m_texid[1]);
        addEntry_2DE8(self, drawRecord, state, key.i);
    }
    cPS2ScratchPad_advanceBuffer(ring);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_buildDmaChains);
#ifdef SKIP_ASM
extern void* SPRMan;
extern char* cGraphicsMan_TheGraphicsMan_railmodifier __asm__("cGraphicsMan_TheGraphicsMan");
extern const int D_004A4058[];
extern "C" void cPS2DeferManager_bucketRecords(cPS2DeferManager* self);
extern "C" void cPS2DeferManager_writeDefaultState(cPS2DeferManager* self, ulong** pp);
extern "C" void cPS2DeferManager_sortItems(cPS2DeferManager* self);
extern "C" void resetTextureUseTable();
extern "C" void cPS2ScratchPad_setLayout(void* ring, int mode);
extern "C" void cTextureMan_resetBoundTextures(void* obj);
extern "C" void vramHeap_moveLinksToScratchpad(void* self);
extern "C" void vramHeap_moveLinksFromScratchpad(void* self);
extern "C" void cPS2DeferManager_writeStateChange(void* self, unsigned int* state, int layer, ulong** pp);
extern "C" void cTextureMan_ensureContextTextures(void* obj, int u, int v, int idx, ulong** pp, int* path);
extern "C" void cPS2GraphicsMan_queueDepthFogPass(void* gs, int i, ulong** pp, int* path);
extern "C" void cPS2GraphicsMan_queueLightGlowPass(void* gs, int i, ulong** pp, int* path);
extern "C" void cPS2GraphicsMan_queueScreenTintPass(void* gs, int i, ulong** pp, int* path);


struct sRmSortItem_3490 {
    unsigned int key;
    tDeferBucket* node;
};

struct sRmBatch_3490 {
    char pad_0x0[0x67CA4];
    int nItems;                         // 0x67CA4
    sRmSortItem_3490 items[0x400];      // 0x67CA8
    sRail20_2CC8 state;                 // 0x69CA8
    char pad_0x69CBC[0x69CC4 - 0x69CBC];
    int lastTex;                        // 0x69CC4
};

struct sRmTexUse_3490 {
    int first;
    int count;
};

struct sRmPassTypes_3490 {
    int v[2];
};

extern sRmTexUse_3490 cPS2DeferManager_textureUseTable_3490[] __asm__("cPS2DeferManager_textureUseTable");

static inline void resetState_3490(sRail20_2CC8* d)
{
    *d = sDrawState_s_defaultDrawState;
}

/**
 * Builds the frame's DMA chains from the deferred draw records.
 *
 * Buckets the records (cPS2DeferManager_bucketRecords), writes the default state (cPS2DeferManager_writeDefaultState), sorts the buckets (cPS2DeferManager_sortItems),
 * resets the texture use table (resetTextureUseTable) and prepares the texture manager (cGraphicsMan_TheGraphicsMan +0x18F4:
 * cTextureMan_resetBoundTextures and vramHeap_moveLinksToScratchpad on +0x1F60). It records each texture's last use and use count in cPS2DeferManager_textureUseTable, then
 * splits the sorted list into three passes at the pass types in D_004A4058 (state word 2 bits 5-9). For every bucket it
 * writes the state change (cPS2DeferManager_writeStateChange) when the state differs, the texture setup (cTextureMan_ensureContextTextures) and a CALL to the
 * bucket's chain, whose tail becomes a RET. After a type-6 pass it runs cPS2GraphicsMan_queueDepthFogPass per entry and resets the state;
 * after a type-9 pass it ends both chains with END tags, reports their positions in out1/out2 and runs
 * cPS2GraphicsMan_queueLightGlowPass/cPS2GraphicsMan_queueScreenTintPass per entry. Finally it ends both chains, stores the main chain's end in *pktAddr and
 * closes the texture manager (vramHeap_moveLinksFromScratchpad).
 *
 * Likely bug: the copy of D_004A4058 holds two pass types, so the third pass switches on an unwritten stack word.
 *
 * @param pktAddr  main chain cursor (in: start, out: end)
 * @param out1  receives the main chain position after the type-9 pass
 * @param path  second chain cursor
 * @param out2  receives the second chain position after the type-9 pass
 */
// PORT: DMA tags written as 64-bit words through uncached (| 0x30000000 / | 0x20000000) addresses held in int.
extern "C" void cPS2DeferManager_buildDmaChains(sRmBatch_3490* self, int* pktAddr, int* out1, int* path, int* out2)
{
    sRmPassTypes_3490 split;
    int bounds[3];
    ulong* pkt;
    ((cPS2DeferManager*)self)->bucketRecords();
    *out1 = 0;
    *out2 = 0;
    pkt = (ulong*)(*pktAddr | 0x30000000);
    resetState_3490(&self->state);
    ((cPS2DeferManager*)self)->writeDefaultState(&pkt);
    ((cPS2DeferManager*)self)->sortItems();
    resetTextureUseTable();
    void* ring = SPRMan;
    int saved = *(int*)ring;
    cPS2ScratchPad_setLayout(ring, 5);
    cPS2GraphicsMan* gs = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_railmodifier;
    cTextureMan* obj = gs->mTexMan;
    cTextureMan_resetBoundTextures(obj);
    vramHeap_moveLinksToScratchpad(obj->mMainHeap);
    if (self->nItems != 0) {
        int idx = 0;
        int pass;
        for (int i = 0; i < self->nItems; i++) {
            for (tDeferBucket* n = self->items[i].node; n != 0; n = n->mpLink) {
                short primaryTex = ((short*)&n->mTexKey)[0];
                if (primaryTex >= 0) {
                    cPS2DeferManager_textureUseTable_3490[primaryTex].first = idx;
                    cPS2DeferManager_textureUseTable_3490[primaryTex].count++;
                }
                short secondaryTex = ((short*)&n->mTexKey)[1];
                if (secondaryTex >= 0) {
                    cPS2DeferManager_textureUseTable_3490[secondaryTex].first = idx;
                    cPS2DeferManager_textureUseTable_3490[secondaryTex].count++;
                }
                idx++;
            }
        }
        split = *(const sRmPassTypes_3490*)D_004A4058;
        for (int j = 0; j < 2; j++) {
            int lo = (j == 0) ? 0 : bounds[j - 1];
            int hi = self->nItems;
            bounds[j] = 0;
            while (hi - lo >= 2) {
                int mid = (lo + hi) >> 1;
                bounds[j] = mid;
                if ((int)((self->items[mid].node->mpState->mDrawState.m_bitGroups[2] & 0x3E0) >> 5) >= split.v[j])
                    hi = mid;
                else
                    lo = mid;
            }
            bounds[j] = hi;
        }
        bounds[2] = self->nItems;
        self->lastTex = -1;
        unsigned int* prev = 0;
        idx = 0;
        for (pass = 0; pass < 3; pass++) {
            int start = (pass == 0) ? 0 : bounds[pass - 1];
            int end = bounds[pass];
            for (int i = start; i < end; i++) {
                tDeferBucket* n = self->items[i].node;
                if (n != 0) {
                    do {
                        unsigned int* st = ((unsigned int*)n->mpState);
                        if (st != prev) {
                            cPS2DeferManager_writeStateChange(self, st, n->mViewIndex, &pkt);
                            prev = st;
                        }
                        int key = n->mTexKey;
                        cTextureMan_ensureContextTextures(obj, ((short*)&key)[0], ((short*)&key)[1], idx++, &pkt, path);
                        if (n->mPacket != 0) {
                            pkt[0] = ((ulong)(unsigned int)n->mPacket << 32) | 0x50000000;
                            pkt[1] = 0;
                            pkt += 2;
                            ulong* tailTag = (ulong*)(n->mTail | 0x30000000);
                            tailTag[0] = 0x60000000;
                            tailTag[1] = 0;
                        }
                        n = n->mpLink;
                    } while (n != 0);
                }
            }
            switch (split.v[pass]) {
            case 6: {
                for (int k = 0; k < gs->mNumViews; k++)
                    cPS2GraphicsMan_queueDepthFogPass(gs, k, &pkt, path);
                self->lastTex = -1;
                prev = 0;
                resetState_3490(&self->state);
                ((cPS2DeferManager*)self)->writeDefaultState(&pkt);
                int* tc = (int*)(&obj->mBoundTex[0]);
                for (int k = 0; k < 2; k++)
                    tc[k] = -1;
                obj->mCLUTTex = -1;
                break;
            }
            case 9: {
                pkt[0] = 0x70000000;
                pkt[1] = 0;
                pkt += 2;
                ulong* pathTag = (ulong*)(*path | 0x20000000);
                pathTag[0] = 0x70000000;
                pathTag[1] = 0;
                *path += 0x10;
                *out1 = (int)pkt & 0x0FFFFFFF;
                *out2 = *path;
                for (int k = 0; k < gs->mNumViews; k++)
                    cPS2GraphicsMan_queueLightGlowPass(gs, k, &pkt, path);
                for (int k = 0; k < gs->mNumViews; k++)
                    cPS2GraphicsMan_queueScreenTintPass(gs, k, &pkt, path);
                self->lastTex = -1;
                prev = 0;
                resetState_3490(&self->state);
                ((cPS2DeferManager*)self)->writeDefaultState(&pkt);
                int* tc = (int*)(&obj->mBoundTex[0]);
                for (int k = 0; k < 2; k++)
                    tc[k] = -1;
                obj->mCLUTTex = -1;
                break;
            }
            }
        }
    }
    if (*out1 == 0) {
        pkt[0] = 0x70000000;
        pkt[1] = 0;
        pkt += 2;
        *out1 = (int)pkt & 0x0FFFFFFF;
    }
    if (*out2 == 0) {
        ulong* pathTag = (ulong*)(*path | 0x20000000);
        pathTag[0] = 0x70000000;
        pathTag[1] = 0;
        *path += 0x10;
        *out2 = *path;
    }
    ulong* pathTag = (ulong*)(*path | 0x20000000);
    pathTag[0] = 0x70000000;
    pathTag[1] = 0;
    *path += 0x10;
    pkt[0] = 0x70000000;
    pkt[1] = 0;
    pkt += 2;
    *pktAddr = (int)pkt & 0x0FFFFFFF;
    // PORT: EE `sync` (orders the uncached DMA-tag writes); no C equivalent.
    __asm__ __volatile__("sync");
    vramHeap_moveLinksFromScratchpad(obj->mMainHeap);
    cPS2ScratchPad_setLayout(ring, saved);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_writeStateChange);
#ifdef SKIP_ASM
// FAKEMATCH: `self` is taken as an integer (`__SIZE_TYPE__ selfi`, cast to sRmRend3C20* in the body), so its pseudo is not
// pointer-flagged and sched1's find_base_term cannot tell `self+0x68000` from the stack: the scissor load then stays after
// the packet-header stores, as in retail (integer-typed base steering); plain form 96.29%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void gsPacket_addAlpha(sGifPacket* pkt, int a, int b, int ctx);
extern "C" void cPS2DeferManager_writeVu1ProgramSetup(cPS2DeferManager* self, int a, int b, unsigned int c, ulong** pp);
extern char* cGraphicsMan_TheGraphicsMan_railmodifier __asm__("cGraphicsMan_TheGraphicsMan");


struct sRmKey3C20 {
    unsigned short a, b, c, d;
};

struct sRmRend3C20 {
    char pad_0x0[0x69CA8];
    sDrawState state;            // 0x69CA8
    unsigned int frame;         // 0x69CBC
    int buf;                    // 0x69CC0
    int scissor;                // 0x69CC4
    char pad_0x69CC8[0x6AED4 - 0x69CC8];
    sRmKey3C20 keys[1];         // 0x6AED4
};

static inline sDrawState* State3C20(sRmRend3C20* self)
{
    return &self->state;
}

static inline unsigned int Mask3C20(unsigned int m, unsigned int w)
{
    return w & m;
}

static inline unsigned int Bits3C20(unsigned int w, unsigned int m)
{
    return w & m;
}

// PORT: 64-bit DMA/GIF tag writes (ulong); self arrives as a pointer-sized integer (__SIZE_TYPE__).
extern "C" void cPS2DeferManager_writeStateChange_impl(__SIZE_TYPE__ selfi, sDrawState* st, int a2, ulong** pp) __asm__("cPS2DeferManager_writeStateChange");
/**
 * Writes the GS register changes needed to go from the defer manager's current draw state (+0x69CA8) to st.
 *
 * Starts with a one-quadword CNT tag. When st's VU1 program type (word 0 bits 6-9) is nonzero it emits the program
 * setup (cPS2DeferManager_writeVu1ProgramSetup): mode 0 when the type changes, mode 1 when only the matrix/layer index changes, mode 2 when the
 * frame word (word 3 bits 0-29) changes. It then collects A+D writes only for fields that differ: SCISSOR from the
 * rectangle table (+0x6AED4) indexed by word 2 bits 0-4, ZBUF for both contexts on the Z-mask bit, TEST for both
 * contexts, CLAMP_1/2 (word 0 bits 2-3 / 4-5) and ALPHA_1/2 (word 1 bits 2-6 / 7-11 with FIX from word 0). It copies st
 * into the current state and closes the block with gsPacket_writeHeader mode 1.
 *
 * @param selfi  the defer manager (passed as an integer)
 * @param st  draw state to switch to
 * @param nodeLayer  the bucket's matrix/layer index
 * @param pp  DMA chain cursor
 */
extern "C" void cPS2DeferManager_writeStateChange_impl(__SIZE_TYPE__ selfi, sDrawState* st, int nodeLayer, ulong** pp)
{
    sRmRend3C20* self = (sRmRend3C20*)selfi;
    sRmPkt27A8 pkt;
    (*pp)[0] = 0x10000001;
    (*pp)[1] = 0;
    (*pp)[2] = 0;
    (*pp)[3] = (ulong)1 << 60;
    *pp += 4;
    int ctx = (st->m_bitGroups[0] & 0x3C0) >> 6;
    if (ctx != 0) {
        unsigned int frameWord = Mask3C20(0x3FFFFFFF, st->m_bitGroups[3]);
        if (((State3C20(self)->m_bitGroups[0] & 0x3C0) >> 6) != ctx) {
            self->buf = nodeLayer;
            self->frame = frameWord;
            cPS2DeferManager_writeVu1ProgramSetup((cPS2DeferManager*)self, (st->m_bitGroups[0] & 0x3C0) >> 6, 0, frameWord, pp);
        } else if (nodeLayer != self->buf) {
            self->buf = nodeLayer;
            self->frame = frameWord;
            cPS2DeferManager_writeVu1ProgramSetup((cPS2DeferManager*)self, (st->m_bitGroups[0] & 0x3C0) >> 6, 1, frameWord, pp);
        }
        if (self->frame != frameWord) {
            self->buf = nodeLayer;
            self->frame = frameWord;
            cPS2DeferManager_writeVu1ProgramSetup((cPS2DeferManager*)self, (st->m_bitGroups[0] & 0x3C0) >> 6, 2, frameWord, pp);
        }
    }
    pkt.count = 0;
    pkt.base = *pp;
    pkt.cur = *pp + 6;
    int k = st->m_bitGroups[2] & 0x1F;
    if (k != self->scissor) {
        self->scissor = k;
        if (k >= 0) {
            gsPacket_addScissor((sGifPacket*)&pkt, self->keys[k].a, self->keys[k].c, self->keys[k].b, self->keys[k].d);
        }
    }
    if (((State3C20(self)->m_bitGroups[1] & 0x400000) >> 22) != ((st->m_bitGroups[1] & 0x400000) >> 22)) {
        cPS2GraphicsMan* gs = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_railmodifier;
        gsPacket_addZBuf((sGifPacket*)&pkt, (unsigned int)gs->mCurrentZBufferPage, (unsigned int)gs->mCurrentZBFormat,
                      (st->m_bitGroups[1] & 0x400000) >> 22, 0);
        gsPacket_addZBuf((sGifPacket*)&pkt, (unsigned int)gs->mCurrentZBufferPage, (unsigned int)gs->mCurrentZBFormat,
                      (st->m_bitGroups[1] & 0x400000) >> 22, 1);
    }
    if (Bits3C20(State3C20(self)->m_bitGroups[1], 0x1800000) != Bits3C20(st->m_bitGroups[1], 0x1800000) ||
        Bits3C20(State3C20(self)->m_bitGroups[1], 0x300000) != Bits3C20(st->m_bitGroups[1], 0x300000) ||
        Bits3C20(State3C20(self)->m_bitGroups[1], 0xFF000) != Bits3C20(st->m_bitGroups[1], 0xFF000)) {
        gsPacket_addTest((sRmGsPacket*)&pkt, (st->m_bitGroups[1] & 0x1800000) >> 23, (st->m_bitGroups[1] & 0x300000) >> 20,
                      (st->m_bitGroups[1] & 0xFF000) >> 12, 0);
        gsPacket_addTest((sRmGsPacket*)&pkt, (st->m_bitGroups[1] & 0x1800000) >> 23, (st->m_bitGroups[1] & 0x300000) >> 20,
                      (st->m_bitGroups[1] & 0xFF000) >> 12, 1);
    }
    if ((State3C20(self)->m_bitGroups[0] & 0xC) != (st->m_bitGroups[0] & 0xC)) {
        gsPacket_addClamp((sGifPacket*)&pkt, (st->m_bitGroups[0] & 0xC) >> 2, 0);
    }
    if (((State3C20(self)->m_bitGroups[1] & 0x7C) >> 2) != ((st->m_bitGroups[1] & 0x7C) >> 2) || ((State3C20(self)->m_bitGroups[0] >> 12) & 0xFF) != ((st->m_bitGroups[0] >> 12) & 0xFF)) {
        gsPacket_addAlpha((sGifPacket*)&pkt, (st->m_bitGroups[1] & 0x7C) >> 2, (st->m_bitGroups[0] >> 12) & 0xFF, 0);
    }
    if ((State3C20(self)->m_bitGroups[0] & 0x30) != (st->m_bitGroups[0] & 0x30)) {
        gsPacket_addClamp((sGifPacket*)&pkt, (st->m_bitGroups[0] & 0x30) >> 4, 1);
    }
    if (((State3C20(self)->m_bitGroups[1] & 0xF80) >> 7) != ((st->m_bitGroups[1] & 0xF80) >> 7) || ((State3C20(self)->m_bitGroups[0] >> 20) & 0xFF) != ((st->m_bitGroups[0] >> 20) & 0xFF)) {
        gsPacket_addAlpha((sGifPacket*)&pkt, (st->m_bitGroups[1] & 0xF80) >> 7, (st->m_bitGroups[0] >> 20) & 0xFF, 1);
    }
    self->state = *st;
    gsPacket_writeHeader(&pkt, pp, 1);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_sortItems);
#ifdef SKIP_ASM

extern tDeferSortItem cPS2DeferManager_sortSwapBuffer[];
extern void* SPRMan;
extern "C" void* cPS2ScratchPad_claimBuffer(void* ring, unsigned int addr, int size, int flags);
extern "C" void cPS2ScratchPad_advanceBuffer(void* ring);

/**
 * Sorts the defer manager's sort items (+0x67CA8, count +0x67CA4) by key with a four-pass 8-bit radix sort.
 *
 * The 4 x 257 histogram lives in the scratchpad ring (cPS2ScratchPad_claimBuffer) and cPS2DeferManager_sortSwapBuffer is the swap buffer; after the
 * fourth pass the sorted items are back in place.
 */
void cPS2DeferManager::sortItems()
{
    void* ring = SPRMan;
    tDeferSortItem* dst = cPS2DeferManager_sortSwapBuffer;
    int n = mNumSortItems;
    tDeferSortItem* src = ((&mSortItems[0]));
    int (*hist)[257] = (int (*)[257])cPS2ScratchPad_claimBuffer(ring, 0, -1, 0);
    MEM_fill(hist, 0, 4112);
    for (int i = 0; i < n; i++) {
        unsigned int key = src[i].mKey;
        hist[0][(key & 0xFF) + 1]++;
        hist[1][((key >> 8) & 0xFF) + 1]++;
        hist[2][((key >> 16) & 0xFF) + 1]++;
        hist[3][(key >> 24) + 1]++;
    }
    for (int i = 1; i < 256; i++) {
        hist[0][i] += hist[0][i - 1];
        hist[1][i] += hist[1][i - 1];
        hist[2][i] += hist[2][i - 1];
        hist[3][i] += hist[3][i - 1];
    }
    int shift;
    for (shift = 0; shift < 32; shift += 8) {
        int* digitCounts = (int*)hist + ((shift << 5) + (shift >> 3));
        for (int i = 0; i < n; i++) {
            int idx = digitCounts[(src[i].mKey >> shift) & 0xFF]++;
            dst[idx] = src[i];
        }
        tDeferSortItem* sortedBuf = dst;
        dst = src;
        src = sortedBuf;
    }
    cPS2ScratchPad_advanceBuffer(ring);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", sDrawState_makeSortKey);
#ifdef SKIP_ASM
/**
 * Builds the sort key for a draw state and texture pair.
 *
 * Packs 31 minus the pass type (word 2 bits 5-9) into bits 26-31, a field chosen by word 1 bits 0-1 (0x3FF, 0x3FE or
 * word 2 bits 10-28) into bits 16-25, a rank of the VU1 program type (word 0 bits 6-9) into bits 13-15 and the texture
 * ids into bits 3-12, then returns the complement so the radix sort orders them high to low.
 *
 * Quirk: the word-2 field is up to 19 bits wide and is not masked, so a value above 0x3FF spills into the pass bits.
 *
 * @param key  texture pair (low and high 16 bits; negative halves are skipped)
 * @return the sort key
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned int sDrawState_makeSortKey(unsigned int* self, int key)
{
    int layerRank = 31 - ((self[2] & 0x3E0) >> 5);
    int modeRank;
    int sel = self[1] & 3;
    switch (sel) {
    case 0:
        modeRank = 0x3FF;
        break;
    case 1:
        modeRank = 0x3FE;
        break;
    case 2:
        modeRank = (self[2] & 0x1FFFFC00) >> 10;
        break;
    default:
        modeRank = 0;
        break;
    }
    int programRank;
    switch ((self[0] & 0x3C0) >> 6) {
    case 5: programRank = 6; break;
    case 2: programRank = 5; break;
    case 1: programRank = 4; break;
    case 3: programRank = 3; break;
    case 6:
    case 7: programRank = 2; break;
    case 4:
    case 8: programRank = 1; break;
    case 0:
    default: programRank = 0; break;
    }
    int lo = (short)key;
    int hi = key >> 16;
    int texBits = 0;
    if (lo >= 0) {
        texBits = lo;
    }
    if (hi >= 0) {
        texBits |= (hi & 0xF) << 6;
    }
    texBits &= 0x3FF;
    return ~((layerRank << 26) | (modeRank << 16) | (programRank << 13) | (texBits << 3));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_writeDefaultState);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void gsPacket_addAlpha(sGifPacket* pkt, int a, int b, int ctx);
extern "C" void cPS2DeferManager_writeVu1ProgramSetup(cPS2DeferManager* self, int a, int b, unsigned int c, ulong** pp);
// PORT: bound by asm label so this caller's packet type doesn't conflict with the definition.
extern "C" void gsPacket_writeHeader_4360(void* pkt, ulong** pp, int mode) __asm__("gsPacket_writeHeader");
extern char* cGraphicsMan_TheGraphicsMan_railmodifier __asm__("cGraphicsMan_TheGraphicsMan");

// A copy of sDrawState (its member names).
struct sRmGsState4360 {
    unsigned int m_bitGroups[4];
    short m_texid[2];
    sRmGsState4360()
    {
        unsigned int i;
        for (i = 0; i < 2; i++) {
            m_texid[i] = -1;
        }
        memset(this, 0, 0x10);
    }
};

struct sRmPkt4360 {
    int count;      // 0x0
    ulong* cur;     // 0x4
    ulong* base;    // 0x8
};

/**
 * Writes the complete default draw state (sDrawState_s_defaultDrawState) to the DMA chain.
 *
 * After a one-quadword CNT tag it emits the VU1 program setup (cPS2DeferManager_writeVu1ProgramSetup mode 0) and CLAMP, ALPHA, ZBUF and TEST
 * for context 1 and then context 2, closing the block with gsPacket_writeHeader mode 1.
 *
 * Likely bug: the context-2 ZBUF and TEST writes take their fields from word 1 one or two bits higher than the
 * context-1 writes and cPS2DeferManager_writeStateChange do.
 *
 * @param pp  DMA chain cursor
 */
// PORT: 64-bit DMA/GIF tag writes (ulong).
void cPS2DeferManager::writeDefaultState(ulong** pp)
{
    cPS2GraphicsMan* gs = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_railmodifier;
    sRmGsState4360 st;
    sRmPkt4360 pkt;
    st = *(sRmGsState4360*)&sDrawState_s_defaultDrawState;
    (*pp)[0] = 0x10000001;
    (*pp)[1] = 0;
    (*pp)[2] = 0;
    (*pp)[3] = (ulong)1 << 60;
    *pp += 4;
    writeVu1ProgramSetup((st.m_bitGroups[0] & 0x3C0) >> 6, 0, st.m_bitGroups[3] & 0x3FFFFFFF, pp);
    pkt.count = 0;
    pkt.base = *pp;
    pkt.cur = *pp + 6;
    gsPacket_addClamp((sGifPacket*)&pkt, (st.m_bitGroups[0] & 0xC) >> 2, 0);
    gsPacket_addAlpha((sGifPacket*)&pkt, (st.m_bitGroups[1] & 0x7C) >> 2, (st.m_bitGroups[0] >> 12) & 0xFF, 0);
    gsPacket_addZBuf((sGifPacket*)&pkt, (unsigned int)gs->mCurrentZBufferPage, (unsigned int)gs->mCurrentZBFormat,
            (st.m_bitGroups[1] >> 22) & 1, 0);
    gsPacket_addTest((sRmGsPacket*)&pkt, (st.m_bitGroups[1] >> 23) & 3, (st.m_bitGroups[1] >> 20) & 3,
            (st.m_bitGroups[1] & 0xFF000) >> 12, 0);
    gsPacket_addClamp((sGifPacket*)&pkt, (st.m_bitGroups[0] & 0x30) >> 4, 1);
    gsPacket_addAlpha((sGifPacket*)&pkt, (st.m_bitGroups[1] & 0xF80) >> 7, (st.m_bitGroups[0] >> 20) & 0xFF, 1);
    gsPacket_addZBuf((sGifPacket*)&pkt, (unsigned int)gs->mCurrentZBufferPage, (unsigned int)gs->mCurrentZBFormat,
            (st.m_bitGroups[1] >> 23) & 1, 1);
    gsPacket_addTest((sRmGsPacket*)&pkt, (st.m_bitGroups[1] >> 25) & 3, (st.m_bitGroups[1] >> 22) & 3,
            (st.m_bitGroups[1] & 0xFF000) >> 12, 1);
    gsPacket_writeHeader_4360(&pkt, pp, 1);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_findOrAddViewState);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_45B8(void* dst, const void* src)
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
        : "r"(dst), "r"(src)
        : "memory");
}

struct sM44_45B8 {
    float m[16];
    sM44_45B8& operator=(const sM44_45B8& o) { vu0CopyMtx_45B8(this, &o); return *this; }
} __attribute__((aligned(16)));

struct sV4_45B8 {
    float v[4];
} __attribute__((aligned(16)));

#define MTX_EQ_45B8(a, b) \
    ((a).m[0] == (b).m[0] && (a).m[1] == (b).m[1] && (a).m[2] == (b).m[2] && (a).m[3] == (b).m[3] && \
     (a).m[4] == (b).m[4] && (a).m[5] == (b).m[5] && (a).m[6] == (b).m[6] && (a).m[7] == (b).m[7] && \
     (a).m[8] == (b).m[8] && (a).m[9] == (b).m[9] && (a).m[10] == (b).m[10] && (a).m[11] == (b).m[11] && \
     (a).m[12] == (b).m[12] && (a).m[13] == (b).m[13] && (a).m[14] == (b).m[14] && (a).m[15] == (b).m[15])
#define VEC_EQ_45B8(a, b) \
    ((a).v[0] == (b).v[0] && (a).v[1] == (b).v[1] && (a).v[2] == (b).v[2] && (a).v[3] == (b).v[3])

// One cached transform set (0x180 bytes; tDeferViewState's member names).
struct sRailXf_45B8 {
    sM44_45B8 mWorld;           // 0x000
    sM44_45B8 mView;            // 0x040
    sM44_45B8 mWorldView;       // 0x080
    sV4_45B8 mGuardScale;       // 0x0C0
    sV4_45B8 mGuardOffset;      // 0x0D0
    sM44_45B8 mViewProj;        // 0x0E0 (key)
    sM44_45B8 mWorldClip;       // 0x120 (key)
    sV4_45B8 mScreenScale;      // 0x160 (key)
    sV4_45B8 mScreenOffset;     // 0x170 (key)

    // Each compares one key of a cached entry against this probe; entry first.
    int ViewProjEq(const sM44_45B8& e) const { return MTX_EQ_45B8(e, mViewProj); }
    int WorldClipEq(const sM44_45B8& e) const { return MTX_EQ_45B8(e, mWorldClip); }
    int ScreenScaleEq(const sV4_45B8& e) const { return VEC_EQ_45B8(e, mScreenScale); }
    int ScreenOffsetEq(const sV4_45B8& e) const { return VEC_EQ_45B8(e, mScreenOffset); }
};


/**
 * Finds or adds a 0x180-byte matrix record in the defer manager's matrix cache.
 *
 * Compares in's four keys (the matrices at +0xE0/+0x120 and vectors at +0x160/+0x170) with each cached record
 * (+0x69CD0, count +0x69CC8); on no match appends a copy. The index ends up in the current matrix (+0x69CC0). There is
 * no capacity check.
 *
 * @param in  matrix record to look up
 */
extern "C" void cPS2DeferManager_findOrAddViewState(cPS2DeferManager* self, sRailXf_45B8* in)
{
    int i;
    self->mActiveViewIndex = -1;
    for (i = 0; i < self->mIndexView; i++) {
        if (in->ViewProjEq(((sRailXf_45B8*)&self->mViewStates[0])[i].mViewProj) &&
            in->WorldClipEq(((sRailXf_45B8*)&self->mViewStates[0])[i].mWorldClip) &&
            in->ScreenScaleEq(((sRailXf_45B8*)&self->mViewStates[0])[i].mScreenScale) &&
            in->ScreenOffsetEq(((sRailXf_45B8*)&self->mViewStates[0])[i].mScreenOffset)) {
            self->mActiveViewIndex = i;
            break;
        }
    }
    if (self->mActiveViewIndex < 0) {
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mWorld = in->mWorld;
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mView = in->mView;
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mWorldView = in->mWorldView;
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mGuardScale = in->mGuardScale;
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mGuardOffset = in->mGuardOffset;
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mViewProj = in->mViewProj;
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mWorldClip = in->mWorldClip;
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mScreenScale = in->mScreenScale;
        ((sRailXf_45B8*)&self->mViewStates[0])[self->mIndexView].mScreenOffset = in->mScreenOffset;
        self->mActiveViewIndex = self->mIndexView;
        self->mIndexView++;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_findOrAddScissor);
#ifdef SKIP_ASM
struct sRailKey4 {
    unsigned short a, b, c, d;
};

struct sRailKeyTable {
    char pad_0x00[0x69CC4];
    int found;                  // 0x69CC4
    char pad_0x69CC8[0x6AED0 - 0x69CC8];
    int count;                  // 0x6AED0
    sRailKey4 keys[1];          // 0x6AED4
};

/**
 * Finds or adds a scissor rectangle (four u16 values) in the defer manager's table and makes it current.
 *
 * Searches the table at +0x6AED4 (count +0x6AED0); on no match appends k. The index is stored at +0x69CC4. There is no
 * capacity check.
 *
 * @param k  rectangle to look up
 */
extern "C" void cPS2DeferManager_findOrAddScissor(sRailKeyTable* self, sRailKey4* k)
{
    int i;
    self->found = -1;
    for (i = 0; i < self->count; i++) {
        if (self->keys[i].a == k->a && self->keys[i].b == k->b &&
            self->keys[i].c == k->c && self->keys[i].d == k->d) {
            self->found = i;
            break;
        }
    }
    if (self->found < 0) {
        self->keys[self->count].a = k->a;
        self->keys[self->count].b = k->b;
        self->keys[self->count].c = k->c;
        self->keys[self->count].d = k->d;
        self->found = self->count;
        self->count++;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_setCurrentState);
#ifdef SKIP_ASM
/**
 * Sets the defer manager's current draw state (+0x69CA8) to a copy of the 20-byte state src.
 *
 * @param src  draw state to copy
 */
void cPS2DeferManager::setCurrentState(const sDrawState* src)
{
    mCurrentState = *src;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPS2DeferManager_writeVu1ProgramSetup);
#ifdef SKIP_ASM
// VU1 microprogram chains (static .vutext DMA chains) and their constant blocks.
extern char D_0042E590[];
extern char D_00430A80[];
extern char D_00434990[];
extern char D_00435BD0[];
extern char D_00439A40[];
extern char D_0043B3F0[];
extern char D_0044B080[];
extern char D_0044B140[];
extern int D_004A5990;

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_4CD0(void* dst, const void* src)
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
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, r = a x b in Sony's sceVu0MulMatrix order).
static inline void vu0MulMtx_4CD0(void* r, const void* a, const void* b)
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
        : "r"(r), "r"(a), "r"(b)
        : "memory");
}

// 4x4 matrix whose copies go through VU0.
struct sMtx_4CD0 {
    float m[16];
    sMtx_4CD0() {}
    sMtx_4CD0(const sMtx_4CD0& o) { vu0CopyMtx_4CD0(this, &o); }
    sMtx_4CD0& operator=(const sMtx_4CD0& o) { vu0CopyMtx_4CD0(this, &o); return *this; }
    void set(float a0, float a1, float a2, float a3, float b0, float b1, float b2, float b3,
             float c0, float c1, float c2, float c3, float d0, float d1, float d2, float d3)
    {
        m[0] = a0; m[4] = a1; m[8] = a2; m[12] = a3;
        m[1] = b0; m[5] = b1; m[9] = b2; m[13] = b3;
        m[2] = c0; m[6] = c1; m[10] = c2; m[14] = c3;
        m[3] = d0; m[7] = d1; m[11] = d2; m[15] = d3;
    }
} __attribute__((aligned(16)));

static inline sMtx_4CD0 operator*(const sMtx_4CD0& a, const sMtx_4CD0& b)
{
    sMtx_4CD0 r;
    vu0MulMtx_4CD0(&r, &a, &b);
    return r;
}


// One DMA quadword.
struct sQw_4CD0 {
    int w[4];
} __attribute__((aligned(16)));

// One matrix record of the defer manager (0x180 bytes; REF structs/render.md "MatrixRecord").
struct sMtxRec_4CD0 {
    sMtx_4CD0 world;        // 0x000
    float view[16];         // 0x040
    sQw_4CD0 worldView[4];  // 0x080
    t4Vector guardScale;   // 0x0C0
    t4Vector guardOffset;  // 0x0D0
    float viewProj[16];     // 0x0E0
    sQw_4CD0 worldClip[4];  // 0x120
    t4Vector screenScale;  // 0x160
    t4Vector screenOffset; // 0x170
};


// Bias matrix for the context-2 texture projection (a guarded function-local static in the original).
extern sMtx_4CD0 D_004FFB80_4CD0 __asm__("D_004FFB80");

// The packet cursor being written.
struct sPkt_4CD0 {
    ulong* p;
    sPkt_4CD0(ulong* q) : p(q) {}
};

// Copies n quadwords to the packet and advances it.
static inline void putQw_4CD0(ulong*& p, const void* src, int n)
{
    cQuad128* d = (cQuad128*)p;
    cQuad128* e = d + n;
    const cQuad128* s = (const cQuad128*)src;
    while (d != e)
        *d++ = *s++;
    p = (ulong*)d;
}

// DMA CALL tag to a static chain.
#define DMA_CALL_4CD0(addr) (((ulong)(unsigned int)(addr) << 32) | 0x50000000)

/**
 * Writes the VU1 microprogram setup for a draw-state program type to the DMA chain.
 *
 * Uses the current matrix record (+0x69CC0 into the 0x180-byte records at +0x69CD0). Mode 0 writes the program's CALL
 * and its constants, mode 1 only the per-matrix constants, mode 2 only the per-draw CALL to w3 (types 6, 7). Types 1,
 * 2, 3 and 6 unpack record quadwords 8-13; type 1 CALLs D_0042E590, type 2 D_00430A80 (with fixed GS data, two
 * cBezierMan_TheBezierMan buffers and 1/3, 1/5, 1/7 constants), type 3 D_00435BD0; type 4 CALLs D_00439A40 with the
 * world-view set (mode 0) or the world-clip set (mode 1); type 5 CALLs D_0043B3F0 with the screen set; type 6 CALLs
 * D_00434990 and adds the world matrix times a texture bias matrix built once into D_004FFB80 (flag D_004A5990); types
 * 6 and 7 add a CALL to w3 when it is nonzero. Types 0 and 8 write nothing.
 *
 * @param vself  the defer manager
 * @param type  VU1 program type (state word 0 bits 6-9)
 * @param mode  0, 1 or 2 as above
 * @param w3  per-draw chain address (state word 3 bits 0-29)
 * @param pp  DMA chain cursor
 */
// PORT: 64-bit DMA/VIF/GIF words (ulong), 128-bit copies (cQuad128), addresses held in int.
void cPS2DeferManager::writeVu1ProgramSetup(int type, int mode, unsigned int w3, ulong** pp)
{
    cPS2DeferManager* self = this;
    sPkt_4CD0 pk(*pp);
    sMtxRec_4CD0* mtxRec = &((sMtxRec_4CD0*)&self->mViewStates[0])[self->mActiveViewIndex];
    switch (type) {
    case 0:
        break;
    case 1:
        switch (mode) {
        case 0:
            pk.p[0] = DMA_CALL_4CD0(D_0042E590);
            pk.p[1] = 0;
            pk.p[2] = ((ulong)(int)D_0044B080 << 32) | 0x3000000C;
            pk.p[3] = 0;
            pk.p += 4;
        case 1:
            pk.p[0] = 0x10000007;
            pk.p[1] = 0;
            pk.p[2] = 0;
            pk.p[3] = 0x6C06000000000000;
            ((cQuad128*)pk.p)[2] = ((cQuad128*)mtxRec)[8];
            ((cQuad128*)pk.p)[3] = ((cQuad128*)mtxRec)[9];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)mtxRec)[10];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)mtxRec)[11];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)mtxRec)[12];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)mtxRec)[13];
            pk.p += 16;
            break;
        case 2:
            break;
        }
        break;
    case 2:
        switch (mode) {
        case 0: {
            float k[4];
            int* ki = (int*)k;
            pk.p[0] = DMA_CALL_4CD0(D_00430A80);
            pk.p[1] = 0;
            pk.p[2] = 0x1000000D;
            pk.p[3] = 0;
            pk.p[4] = 0;
            pk.p[5] = 0x6C0B000000000000;
            ((cQuad128*)pk.p)[3] = ((cQuad128*)mtxRec)[8];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)mtxRec)[9];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)mtxRec)[10];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)mtxRec)[11];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)mtxRec)[12];
            ((cQuad128*)pk.p)[8] = ((cQuad128*)mtxRec)[13];
            pk.p[18] = 0x0000008000000080;
            pk.p[19] = 0x0000008000000080;
            pk.p[20] = 0x302E400000008000;
            pk.p[21] = 1042;
            pk.p[22] = 0x312E400000008000;
            pk.p[23] = 1042;
            pk.p[0x18] = 0x302EC00000008000;
            pk.p[25] = 1042;
            pk.p[26] = 0x312EC00000008000;
            pk.p[27] = 1042;
            pk.p[0x1C] = 0;
            pk.p[29] = 0x6C2A020000000000;
            cPS2BezierMan* bezierMan = (cPS2BezierMan*)cBezierMan_TheBezierMan;
            pk.p[0x1E] = ((ulong)(int)bezierMan->mPatchCache.mBasis[0] << 32) | 0x30000012;
            pk.p[0x1F] = 0;
            ulong ref2 = ((ulong)(int)bezierMan->mPatchCache.mDBasis[0] << 32) | 0x30000012;
            pk.p += 0x20;
            pk.p[0] = ref2;
            pk.p[1] = 0;
            pk.p += 2;
            pk.p[0] = 0x10000006;
            pk.p[1] = 0;
            k[0] = 0.3333333432674408f;
            k[1] = 0.20000000298023224f;
            k[2] = 0.1428571492433548f;
            k[3] = 0;
            pk.p[2] = 0x0000021200000200;
            pk.p[3] = 0;
            pk.p[4] = (ulong)ki[0] | ((ulong)ki[0] << 32);
            pk.p[5] = (ulong)ki[0] | ((ulong)ki[0] << 32);
            pk.p[6] = 0x0000021600000204;
            pk.p[7] = 0;
            pk.p[8] = (ulong)ki[1] | ((ulong)ki[1] << 32);
            pk.p[9] = (ulong)ki[1] | ((ulong)ki[1] << 32);
            pk.p[10] = 0x0000021C0000020A;
            pk.p[11] = 0;
            pk.p[12] = (ulong)ki[2] | ((ulong)ki[2] << 32);
            pk.p[13] = (ulong)ki[2] | ((ulong)ki[2] << 32);
            pk.p += 14;
            break;
        }
        case 1:
            pk.p[0] = 0x10000007;
            pk.p[1] = 0;
            pk.p[2] = 0;
            pk.p[3] = 0x6C06000000000000;
            ((cQuad128*)pk.p)[2] = ((cQuad128*)mtxRec)[8];
            ((cQuad128*)pk.p)[3] = ((cQuad128*)mtxRec)[9];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)mtxRec)[10];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)mtxRec)[11];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)mtxRec)[12];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)mtxRec)[13];
            pk.p += 16;
            break;
        case 2:
            break;
        }
        break;
    case 3:
        switch (mode) {
        case 0:
            pk.p[0] = DMA_CALL_4CD0(D_00435BD0);
            pk.p[1] = 0;
            pk.p[2] = 0x10000008;
            pk.p[3] = 0;
            pk.p[4] = 0;
            pk.p[5] = 0x6C07000000000000;
            ((cQuad128*)pk.p)[3] = ((cQuad128*)mtxRec)[8];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)mtxRec)[9];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)mtxRec)[10];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)mtxRec)[11];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)mtxRec)[12];
            ((cQuad128*)pk.p)[8] = ((cQuad128*)mtxRec)[13];
            pk.p[18] = 0x000002C20000010C;
            pk.p[19] = 0;
            pk.p += 0x14;
            pk.p[0] = ((ulong)(int)D_0044B140 << 32) | 0x30000007;
            pk.p[1] = 0;
            pk.p += 2;
            break;
        case 1:
            pk.p[0] = 0x10000007;
            pk.p[1] = 0;
            pk.p[2] = 0;
            pk.p[3] = 0x6C06000000000000;
            ((cQuad128*)pk.p)[2] = ((cQuad128*)mtxRec)[8];
            ((cQuad128*)pk.p)[3] = ((cQuad128*)mtxRec)[9];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)mtxRec)[10];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)mtxRec)[11];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)mtxRec)[12];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)mtxRec)[13];
            pk.p += 16;
            break;
        case 2:
            break;
        }
        break;
    case 4:
        switch (mode) {
        case 0: {
            pk.p[0] = DMA_CALL_4CD0(D_00439A40);
            pk.p[1] = 0;
            pk.p[2] = 0x10000008;
            pk.p[3] = 0;
            pk.p[4] = 0;
            pk.p[5] = 0x6C07000000000000;
            pk.p += 6;
            putQw_4CD0(pk.p, mtxRec->worldView, 4);
            t4Vector scaleVec = mtxRec->guardScale;
            scaleVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &scaleVec, 1);
            t4Vector offsetVec = mtxRec->guardOffset;
            offsetVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &offsetVec, 1);
            t4Vector projScale;
            projScale.x = mtxRec->view[0];
            projScale.y = mtxRec->view[5];
            projScale.z = 0;
            projScale.w = 0;
            putQw_4CD0(pk.p, &projScale, 1);
            break;
        }
        case 1: {
            pk.p[0] = 0x10000008;
            pk.p[1] = 0;
            pk.p[2] = 0;
            pk.p[3] = 0x6C07000000000000;
            pk.p += 4;
            putQw_4CD0(pk.p, mtxRec->worldClip, 4);
            t4Vector scaleVec = mtxRec->screenScale;
            scaleVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &scaleVec, 1);
            t4Vector offsetVec = mtxRec->screenOffset;
            offsetVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &offsetVec, 1);
            t4Vector projScale;
            projScale.x = mtxRec->viewProj[0];
            projScale.y = mtxRec->viewProj[5];
            projScale.z = 0;
            projScale.w = 0;
            putQw_4CD0(pk.p, &projScale, 1);
            break;
        }
        case 2:
            break;
        }
        break;
    case 5:
        switch (mode) {
        case 0: {
            pk.p[0] = DMA_CALL_4CD0(D_0043B3F0);
            pk.p[1] = 0;
            pk.p[2] = 0x10000004;
            pk.p[3] = 0;
            pk.p[4] = 0;
            pk.p[5] = 0x6C03000000000000;
            pk.p += 6;
            t4Vector scaleVec = mtxRec->screenScale;
            scaleVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &scaleVec, 1);
            t4Vector offsetVec = mtxRec->screenOffset;
            offsetVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &offsetVec, 1);
            t4Vector projScale;
            projScale.x = mtxRec->viewProj[0];
            projScale.y = mtxRec->viewProj[5];
            projScale.z = 0;
            projScale.w = 0;
            putQw_4CD0(pk.p, &projScale, 1);
            break;
        }
        case 1: {
            pk.p[0] = 0x10000004;
            pk.p[1] = 0;
            pk.p[2] = 0;
            pk.p[3] = 0x6C03000000000000;
            pk.p += 4;
            t4Vector scaleVec = mtxRec->screenScale;
            scaleVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &scaleVec, 1);
            t4Vector offsetVec = mtxRec->screenOffset;
            offsetVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &offsetVec, 1);
            t4Vector projScale;
            projScale.x = mtxRec->viewProj[0];
            projScale.y = mtxRec->viewProj[5];
            projScale.z = 0;
            projScale.w = 0;
            putQw_4CD0(pk.p, &projScale, 1);
            break;
        }
        case 2:
            break;
        }
        break;
    case 6:
        if (D_004A5990 == 0) {
            D_004FFB80_4CD0.set(0.5f, 0, 0, 0.5f, 0, -0.5f, 0, 0.5f, 0, 0, 0, 1.0f, 0, 0, 0, 1.0f);
            D_004A5990 = 1;
        }
        switch (mode) {
        case 0: {
            pk.p[0] = DMA_CALL_4CD0(D_00434990);
            pk.p[1] = 0;
            pk.p[2] = 0x10000007;
            pk.p[3] = 0;
            pk.p[4] = 0;
            pk.p[5] = 0x6C06000000000000;
            ((cQuad128*)pk.p)[3] = ((cQuad128*)mtxRec)[8];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)mtxRec)[9];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)mtxRec)[10];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)mtxRec)[11];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)mtxRec)[12];
            ((cQuad128*)pk.p)[8] = ((cQuad128*)mtxRec)[13];
            pk.p[18] = 0x10000005;
            pk.p[19] = 0;
            pk.p[20] = 0;
            pk.p[21] = 0x6C04039200000000;
            pk.p[22] = 0x312E400000008000;
            pk.p[23] = 1042;
            sMtx_4CD0 biasedWorld = mtxRec->world;
            biasedWorld = D_004FFB80_4CD0 * biasedWorld;
            ((cQuad128*)pk.p)[0xC] = *(cQuad128*)&biasedWorld.m[0];
            ((cQuad128*)pk.p)[0xD] = *(cQuad128*)&biasedWorld.m[4];
            ((cQuad128*)pk.p)[0xE] = *(cQuad128*)&biasedWorld.m[8];
            pk.p += 0x1E;
            if (w3 != 0) {
                pk.p[0] = DMA_CALL_4CD0(w3);
                pk.p[1] = 0;
                pk.p += 2;
            }
            break;
        }
        case 1: {
            pk.p[0] = 0x10000007;
            pk.p[1] = 0;
            pk.p[2] = 0;
            pk.p[3] = 0x6C06000000000000;
            ((cQuad128*)pk.p)[2] = ((cQuad128*)mtxRec)[8];
            ((cQuad128*)pk.p)[3] = ((cQuad128*)mtxRec)[9];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)mtxRec)[10];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)mtxRec)[11];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)mtxRec)[12];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)mtxRec)[13];
            pk.p[0x10] = 0x10000005;
            pk.p[17] = 0;
            pk.p[18] = 0;
            pk.p[19] = 0x6C04039200000000;
            pk.p[20] = 0x312E400000008000;
            pk.p[21] = 1042;
            sMtx_4CD0 biasedWorld = mtxRec->world;
            biasedWorld = D_004FFB80_4CD0 * biasedWorld;
            ((cQuad128*)pk.p)[0xB] = *(cQuad128*)&biasedWorld.m[0];
            ((cQuad128*)pk.p)[0xC] = *(cQuad128*)&biasedWorld.m[4];
            ((cQuad128*)pk.p)[0xD] = *(cQuad128*)&biasedWorld.m[8];
            pk.p += 0x1C;
            if (w3 != 0) {
                pk.p[0] = DMA_CALL_4CD0(w3);
                pk.p[1] = 0;
                pk.p += 2;
            }
            break;
        }
        case 2:
            if (w3 != 0) {
                pk.p[0] = DMA_CALL_4CD0(w3);
                pk.p[1] = 0;
                pk.p += 2;
            }
            break;
        }
        break;
    case 7:
        switch (mode) {
        case 0:
            pk.p[0] = DMA_CALL_4CD0(D_00434990);
            pk.p[1] = 0;
            pk.p += 2;
            if (w3 != 0) {
                pk.p[0] = DMA_CALL_4CD0(w3);
                pk.p[1] = 0;
                pk.p += 2;
            }
            break;
        case 1:
            if (w3 != 0) {
                pk.p[0] = DMA_CALL_4CD0(w3);
                pk.p[1] = 0;
                pk.p += 2;
            }
            break;
        case 2:
            if (w3 != 0) {
                pk.p[0] = DMA_CALL_4CD0(w3);
                pk.p[1] = 0;
                pk.p += 2;
            }
            break;
        }
        break;
    case 8:
        break;
    }
    *pp = pk.p;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_init);
#ifdef SKIP_ASM
extern "C" void vramHeap_fileBlock(void* self, int idx);

// A block node (sRailNode's names, below).
struct sRailLinkE40 {
    int flags;      // 0x0 the block address and its flag bits
    int size;       // 0x4
    int owner;      // 0x8
    int prev;       // 0xC
    int next;       // 0x10
    int sPrev;      // 0x14
    int sNext;      // 0x18
};

// A size-class head (a node; only its size-list links are used).
struct sRailSlotE40 {
    char pad_0x0[0x14];
    int sPrev;      // 0x14
    int sNext;      // 0x18
};

struct sRailModE40 {
    sRailLinkE40 store[0x124];  // 0x0
    sRailLinkE40* links;        // 0x1FF0
    int heapSize;               // 0x1FF4
    sRailSlotE40 slotsA[17];    // 0x1FF8
    sRailSlotE40 slotsB[17];    // 0x21D4
    // Two more head nodes (names ours, from use): the address list's (0x23B0; its next is the first block,
    // vramHeap_freeUnlockedBlocks / vramHeap_evictCheapestRun walk from it) and the spare-link stack's (0x23CC; its next is the top,
    // vramHeap_popSpareLink pops it).
    char pad_0x23B0[0xC];
    int addrHeadPrev;           // 0x23BC
    int addrHead;               // 0x23C0
    int addrHeadSPrev;          // 0x23C4
    int addrHeadSNext;          // 0x23C8
    char pad_0x23CC[0xC];
    int spareHeadPrev;          // 0x23D8
    int spareHead;              // 0x23DC
    int spareHeadSPrev;         // 0x23E0
    int spareHeadSNext;         // 0x23E4
    void* pool;                 // 0x23E8
};

/**
 * Sets up a texture-memory block allocator with one free block and an empty link pool.
 *
 * Block 0 covers the whole size from the start address (+0x1FF4 = size), links 1-291 form the spare-link stack (head
 * +0x23DC = 1), the 17 size-class heads of both lists are emptied, the address list starts at block 0 (+0x23C0) and
 * block 0 is filed into its size class (vramHeap_fileBlock). pool (+0x23E8) is the owner table freed blocks report to.
 *
 * @param list  the allocator
 * @param startAddr  start address
 * @param heapSize  size
 * @param pool  owner table
 */
extern "C" void vramHeap_init(void* list, int startAddr, int heapSize, void* pool)
{
    sRailModE40* self = (sRailModE40*)list;
    int i;
    self->heapSize = heapSize;
    self->links[0].flags = startAddr;
    self->links[0].size = heapSize;
    self->links[0].prev = -1;
    self->links[0].next = -1;
    self->links[0].owner = -1;
    for (i = 1; i < 291; i++) {
        self->links[i].flags &= ~1;
        self->links[i].next = i + 1;
    }
    self->links[291].next = -1;
    for (i = 0; i < 17; i++) {
        self->slotsA[i].sNext = -1;
        self->slotsA[i].sPrev = -1;
        self->slotsB[i].sNext = -1;
        self->slotsB[i].sPrev = -1;
    }
    self->addrHead = 0;
    self->addrHeadPrev = -1;
    self->addrHeadSPrev = -1;
    self->addrHeadSNext = -1;
    vramHeap_fileBlock(self, 0);
    self->pool = pool;
    self->spareHeadPrev = -1;
    self->spareHeadSPrev = -1;
    self->spareHeadSNext = -1;
    self->spareHead = 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_moveLinksToScratchpad);
#ifdef SKIP_ASM
extern "C" void SyncDCache(void* begin, void* end);
extern "C" int scratchpad_loadSpareArea(void* base, int size, int mode);

/**
 * Moves a block allocator's link array into the scratchpad ring for fast access.
 *
 * Writes the 0x1FF0-byte array back from the data cache (SyncDCache), copies it with scratchpad_loadSpareArea and points the
 * links pointer (+0x1FF0) at the copy. vramHeap_moveLinksFromScratchpad undoes it.
 */
extern "C" void vramHeap_moveLinksToScratchpad(void* self)
{
    SyncDCache(self, (char*)self + 0x1FEF);
    *(int*)((char*)self + 0x1FF0) = scratchpad_loadSpareArea(self, 0x1FF0, 2);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_moveLinksFromScratchpad);
#ifdef SKIP_ASM
extern "C" void InvalidDCache(void* begin, void* end);
extern "C" void scratchpad_storeSpareArea(void* base, int size, int mode);

/**
 * Moves a block allocator's link array back from the scratchpad ring (InvalidDCache, scratchpad_storeSpareArea) and points the
 * links pointer (+0x1FF0) at it again.
 */
extern "C" void vramHeap_moveLinksFromScratchpad(void* self)
{
    InvalidDCache(self, (char*)self + 0x1FEF);
    scratchpad_storeSpareArea(self, 0x1FF0, 2);
    *(void**)((char*)self + 0x1FF0) = self;
}
#endif

// 0x1c-byte elements reached through a pointer at self+0x1ff0, indexed by
// the cursor at self+0x23dc (which the function then advances)
struct sRailLink {
    char pad_0x00[0x10];
    int field_0x10;
    char pad_0x14[0x8];
};

//95.0% - logic verified correct; the target puts the mult result in v1 and
// ours lands in v0. Reordering the locals does not shift the allocation.
//100%
INCLUDE_ASM("object/railmodifier", vramHeap_popSpareLink);
#ifdef SKIP_ASM
/**
 * Pops a spare link off the allocator's stack (head +0x23DC).
 *
 * @return the link index
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Pop a link index off the free list headed at spareHead.
extern "C" int vramHeap_popSpareLink(sRailModE40* self)
{
    int i = self->spareHead;
    self->spareHead = self->links[i].next;
    return i;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_removeFromAddressList);
#ifdef SKIP_ASM
struct sRailNode {
    int mBlockAddr;
    int mBlockSize;               // 0x04
    int mOwner;                   // 0x08 (name ours) [U] the owning texture's index: vramHeap_allocBlock stores it,
                                  //      vramHeap_freeBlock clears that texture's block fields
    int mPrev;               // 0x0c
    int mNext;               // 0x10
    int mSPrev;              // 0x14
    int mSNext;              // 0x18

    // A list link (mPrev, mNext, mSPrev, mSNext; names ours): a node index, kLinkEnd at the end of a list, or a
    // size-class head, a negative word with kHeadListA or kHeadListB in kHeadListMask and the class in the low bits.
    // [U] vramHeap_fileBlock files used blocks under heads 0x90000000 | class (list A) and free ones under 0xA0000000 |
    // class (list B); vramHeap_removeFromAddressList, vramHeap_fileBlock and vramHeap_unfileBlock resolve a negative link through these bits.
    enum { kLinkEnd = -1, kHeadListMask = 0x70000000, kHeadListA = 0x10000000, kHeadListB = 0x20000000 };
    // The mOwner value vramHeap_freeBlock treats as "no owner texture" (name ours): it clears the owner's block fields
    // for any other value (vramHeap_allocFromBlock gives a split-off block -1 instead).
    enum { kNoOwner = 0x7FFFFFFF };
};

struct sRailNodeMan {
    char pad_0x0[0x1ff0];
    sRailNode* nodes;       // 0x1ff0
    int mHeapSize;          // 0x1ff4 (name ours) [U] vramHeap_init stores the heap size
    sRailNode headsA[17];   // 0x1ff8
    sRailNode headsB[18];   // 0x21d4
    char pad_0x23cc[0x10];
    int freeHead;           // 0x23dc
};

/**
 * Removes a block from the allocator's address list and returns its link to the spare stack (+0x23DC).
 *
 * @param idx  block index
 */
extern "C" void vramHeap_removeFromAddressList(sRailNodeMan* self, int idx)
{
    sRailNode* node = &self->nodes[idx];
    sRailNode* prev;
    int link = node->mPrev;
    if (link < 0) {
        if ((link & sRailNode::kHeadListMask) == sRailNode::kHeadListA) {
            prev = &self->headsA[link & 0x0FFFFFFF];
        } else {
            prev = &self->headsB[link & 0x0FFFFFFF];
        }
    } else {
        prev = &self->nodes[link];
    }
    prev->mNext = node->mNext;
    if (node->mNext >= 0) {
        self->nodes[node->mNext].mPrev = node->mPrev;
    }
    node->mNext = self->freeHead;
    self->freeHead = idx;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_fileBlock);
#ifdef SKIP_ASM
// PORT: PS2-only MMI count-leading-sign-bits (plzcw); off-PS2 use a clz builtin. Volatile so it stays before
// the size-class multiply, as in the target schedule.
static inline int railFloorLog2_60A8(int x)
{
    long r;
    __asm__ __volatile__("plzcw %0, %1\n\txori %0, %0, 0x1f\n\taddiu %0, %0, -1" : "=r"(r) : "r"(x));
    return r;
}

/**
 * Files a block into its size-class list.
 *
 * The class is floor(log2(size)) - 6. Used blocks (address word bit 0) go into list A, ordered by address; free blocks
 * into list B, ordered by size.
 *
 * Quirk: an entry is compared only with members that have a successor, so one that belongs before the last member is
 * appended after it.
 *
 * @param selfp  the allocator
 * @param idx  block index
 */
extern "C" void vramHeap_fileBlock(void* selfp, int idx)
{
    sRailNodeMan* self = (sRailNodeMan*)selfp;
    sRailNode* node = &self->nodes[idx];
    sRailNode* heads;
    int tag;
    if (node->mBlockAddr & 1) {
        heads = self->headsA;
        tag = 0x90000000;
    } else {
        heads = self->headsB;
        tag = 0xA0000000;
    }
    int sizeClass = railFloorLog2_60A8(node->mBlockSize) - 6;
    sRailNode* head = &heads[sizeClass];
    int first = head->mSNext;
    if (first == sRailNode::kLinkEnd) {
        head->mSNext = idx;
        node->mSPrev = sizeClass | tag;
        node->mSNext = first;
        return;
    }
    for (int i = first; i >= 0; i = self->nodes[i].mSNext) {
        sRailNode* n = &self->nodes[i];
        if (n->mSNext == sRailNode::kLinkEnd) {
            n->mSNext = idx;
            node->mSPrev = i;
            node->mSNext = sRailNode::kLinkEnd;
            return;
        }
        if ((heads == self->headsA && n->mBlockAddr > node->mBlockAddr) ||
            (heads == self->headsB && n->mBlockSize >= node->mBlockSize)) {
            int link = n->mSPrev;
            node->mSNext = i;
            node->mSPrev = link;
            sRailNode* prev;
            if (link < 0) {
                if ((link & sRailNode::kHeadListMask) == sRailNode::kHeadListA) {
                    prev = &self->headsA[link & 0x0FFFFFFF];
                } else {
                    prev = &self->headsB[link & 0x0FFFFFFF];
                }
            } else {
                prev = &self->nodes[link];
            }
            prev->mSNext = idx;
            self->nodes[node->mSNext].mSPrev = idx;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_unfileBlock);
#ifdef SKIP_ASM
/**
 * Removes a block from its size-class list.
 *
 * @param idx  block index
 */
extern "C" void vramHeap_unfileBlock(sRailNodeMan* self, int idx)
{
    sRailNode* node = &self->nodes[idx];
    sRailNode* prev;
    int link = node->mSPrev;
    if (link < 0) {
        if ((link & sRailNode::kHeadListMask) == sRailNode::kHeadListA) {
            prev = &self->headsA[link & 0x0FFFFFFF];
        } else {
            prev = &self->headsB[link & 0x0FFFFFFF];
        }
    } else {
        prev = &self->nodes[link];
    }
    prev->mSNext = node->mSNext;
    if (node->mSNext >= 0) {
        self->nodes[node->mSNext].mSPrev = node->mSPrev;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_allocFromBlock);
#ifdef SKIP_ASM
extern "C" void vramHeap_fileBlock(void* self, int idx);
// PORT: the unit declares vramHeap_popSpareLink as void; it returns the popped free index.
extern "C" int vramHeap_popSpareLink_pop(void* self) __asm__("vramHeap_popSpareLink");

/**
 * Allocates size units from a free block, splitting off the rest as a new free block when a spare link is left.
 *
 * The block leaves its size list; on an exact fit (or no spare link) the whole block is marked used. Otherwise a new
 * block (vramHeap_popSpareLink) takes the remainder, with no owner, after it in the address list and in its size class. The
 * block is marked used (bit 0) and refiled (vramHeap_fileBlock).
 *
 * @param idx  free block to allocate from
 * @param size  units needed
 */
extern "C" void vramHeap_allocFromBlock(sRailNodeMan* self, int idx, int size)
{
    sRailNode* node = &self->nodes[idx];
    vramHeap_unfileBlock(self, idx);
    if (size == node->mBlockSize || self->freeHead == sRailNode::kLinkEnd) {
        node->mBlockAddr |= 1;
    } else {
        int n = vramHeap_popSpareLink_pop(self);
        sRailNode* nn = &self->nodes[n];
        nn->mBlockAddr = node->mBlockAddr + size;
        nn->mBlockSize = node->mBlockSize - size;
        nn->mOwner = -1;
        nn->mPrev = idx;
        int next = node->mNext;
        nn->mNext = next;
        if (next >= 0) {
            self->nodes[next].mPrev = n;
        }
        node->mNext = n;
        vramHeap_fileBlock(self, n);
        node->mBlockSize = size;
        node->mBlockAddr |= 1;
    }
    vramHeap_fileBlock(self, idx);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_freeBlock);
#ifdef SKIP_ASM
/**
 * Frees a used block, detaches it from its owner and merges it with free neighbours.
 *
 * Unless the owner (+0x8) is kNoOwner (0x7FFFFFFF), sets the owner texture's block fields (pool +0x23E8 entry:
 * +0x28/+0x30, or +0x2C/+0x34 when flag bit 2 is set) to -1. Clears the low six flag bits, refiles the block and
 * repeatedly joins it with a free block before or after it in the address list.
 *
 * @param selfp  the allocator
 * @param idx  block index
 */
extern "C" void vramHeap_freeBlock(void* selfp, int idx)
{
    sRailNodeMan* self = (sRailNodeMan*)selfp;
    sRailNode* n = &self->nodes[idx];
    if (n->mOwner != sRailNode::kNoOwner) {
        tTexInfo* owner = (tTexInfo*)(*(char***)((char*)self + 0x23E8))[n->mOwner];
        if ((n->mBlockAddr >> 2) & 1) {
            owner->mCLUTAddr = -1;
            owner->mCLUTBlock = -1;
        } else {
            owner->mTexAddr = -1;
            owner->mTexBlock = -1;
        }
    }
    n->mBlockAddr &= ~0x3F;
    vramHeap_unfileBlock(self, idx);
    vramHeap_fileBlock(self, idx);
    for (;;) {
        int prevIdx = n->mPrev;
        if (prevIdx >= 0 && (self->nodes[prevIdx].mBlockAddr & 1) == 0) {
            idx = prevIdx;
            n = &self->nodes[prevIdx];
        } else {
            int nextIdx = n->mNext;
            if (nextIdx < 0 || (self->nodes[nextIdx].mBlockAddr & 1)) {
                return;
            }
        }
        vramHeap_unfileBlock(self, idx);
        vramHeap_unfileBlock(self, n->mNext);
        n->mBlockSize += self->nodes[n->mNext].mBlockSize;
        vramHeap_removeFromAddressList(self, n->mNext);
        vramHeap_fileBlock(self, idx);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_freeUnlockedBlocks);
#ifdef SKIP_ASM
struct sRailLink6548 {   // a block node (sRailNode's names)
    int flags;      // 0x0 the block address and its flag bits
    int size;       // 0x4
    int owner;      // 0x8
    int prev;       // 0xC
    int next;       // 0x10
    int sPrev;      // 0x14
    int sNext;      // 0x18
};

/**
 * Frees every used block of an allocator that is not locked (flag bit 1), walking the address list (+0x23C0).
 *
 * The next used block is found before each free, as freeing merges neighbours. cTextureMan_purgeAll (the texture manager's
 * purge) runs it on both of its allocators.
 */
extern "C" void vramHeap_freeUnlockedBlocks(void* self)
{
    char* heap = (char*)self;
    int i = *(int*)(heap + 0x23C0);
    while (i >= 0) {
        sRailLink6548* links = *(sRailLink6548**)(heap + 0x1FF0);
        sRailLink6548* cur = &links[i];
        int next = cur->next;
        while (next >= 0) {
            if ((*(sRailLink6548**)(heap + 0x1FF0))[next].flags & 1) {
                break;
            }
            next = (*(sRailLink6548**)(heap + 0x1FF0))[next].next;
        }
        if (cur->flags & 1) {
            if (((cur->flags >> 1) & 1) == 0) {
                vramHeap_freeBlock(self, i);
            }
        }
        i = next;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_freeAllBlocks);
#ifdef SKIP_ASM
struct sRailLink6618 {   // a block node (sRailNode's names)
    int flags;      // 0x0 the block address and its flag bits
    int size;       // 0x4
    int owner;      // 0x8
    int prev;       // 0xC
    int next;       // 0x10
    int sPrev;      // 0x14
    int sNext;      // 0x18
};

/**
 * Frees every used block of an allocator, clearing its lock bit (flag bit 1) first.
 */
extern "C" void vramHeap_freeAllBlocks(void* self)
{
    char* heap = (char*)self;
    int i = *(int*)(heap + 0x23C0);
    while (i >= 0) {
        sRailLink6618* links = *(sRailLink6618**)(heap + 0x1FF0);
        sRailLink6618* cur = &links[i];
        int next = cur->next;
        while (next >= 0) {
            if ((*(sRailLink6618**)(heap + 0x1FF0))[next].flags & 1) {
                break;
            }
            next = (*(sRailLink6618**)(heap + 0x1FF0))[next].next;
        }
        if (cur->flags & 1) {
            if ((cur->flags >> 1) & 1) {
                cur->flags &= ~2;
            }
            vramHeap_freeBlock(self, i);
        }
        i = next;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", resetTextureUseTable);
#ifdef SKIP_ASM
extern void* cPS2ScratchPad_freeSpace;
extern char cPS2DeferManager_textureUseTable[];
extern char D_0053E9FF[];

struct sRailPair66F8 {
    int a;
    int b;
};

// PORT: 128-bit fill pattern built through a union of two 64-bit halves.
static inline cQuad128 makeFill66F8(ulong lo, ulong hi)
{
    union {
        ulong l[2];
        cQuad128 q;
    } u;
    u.l[0] = lo;
    u.l[1] = hi;
    return u.q;
}

// PORT: scratchpad (0x70000000..0x70004000) free space, pointer held in int.
static inline unsigned int spadFree66F8()
{
    char* p = (char*)cPS2ScratchPad_freeSpace;
    return p ? 0x70004000 - (int)p : 0;
}

/**
 * Resets the 2000-entry texture use table cPS2DeferManager_textureUseTable to all-ones (-1).
 *
 * Invalidates the table's cache lines (InvalidDCache), fills the free scratchpad above cPS2ScratchPad_freeSpace with 0xFF bytes and
 * copies that into the table in chunks with scratchpad_storeSpareArea.
 */
extern "C" void resetTextureUseTable()
{
    cQuad128 fill = makeFill66F8(0xFFFFFFFF, 0xFFFFFFFF);
    InvalidDCache(cPS2DeferManager_textureUseTable, D_0053E9FF);
    cQuad128* spadCursor = (cQuad128*)cPS2ScratchPad_freeSpace;
    unsigned int n = spadFree66F8() >> 4;
    for (; n != 0; n--) {
        *spadCursor++ = fill;
    }
    int chunk = spadFree66F8() >> 3;
    for (int i = 0; i < 2000; i += chunk) {
        int left = 2000 - i;
        if (left < chunk) {
            chunk = left;
        }
        scratchpad_storeSpareArea(&((sRailPair66F8*)cPS2DeferManager_textureUseTable)[i], chunk * 8, 2);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_findFreeBlock);
#ifdef SKIP_ASM
// PORT: PS2-only MMI count-leading-sign-bits (plzcw); off-PS2 use a clz builtin.
static inline int railFloorLog2(int x)
{
    long r;
    __asm__("plzcw %0, %1\n\txori %0, %0, 0x1f\n\taddiu %0, %0, -1" : "=r"(r) : "0"(x));
    return r;
}

/**
 * Finds the first free block of at least size units, searching the free lists from its size class upwards.
 *
 * @param size  units needed
 * @return the block index, or -1
 */
extern "C" int vramHeap_findFreeBlock(sRailNodeMan* self, int size)
{
    int bucket;
    for (bucket = railFloorLog2(size) - 6; bucket < 17; bucket++) {
        int i = self->headsB[bucket].mSNext;
        while (i >= 0) {
            if (self->nodes[i].mBlockSize >= size) {
                return i;
            }
            i = self->nodes[i].mSNext;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_findLargestFreeBlock);
#ifdef SKIP_ASM
struct sRailLink6888 {   // a block node (sRailNode's names)
    int flags;      // 0x0 the block address and its flag bits
    int size;       // 0x4
    int owner;      // 0x8
    int prev;       // 0xC
    int next;       // 0x10
    int sPrev;      // 0x14
    int sNext;      // 0x18
};

/**
 * Returns the largest free block in the address list (ties go to the later one).
 *
 * @return the block index, or -1 when no block is free
 */
extern "C" int vramHeap_findLargestFreeBlock(void* self)
{
    char* heapBase = (char*)self;
    int i = *(int*)(heapBase + 0x23C0);
    int best = 0;
    int idx = -1;
    while (i >= 0) {
        sRailLink6888* links = *(sRailLink6888**)(heapBase + 0x1FF0);
        if (!(links[i].flags & 1)) {
            int blockSize = links[i].size;
            if (blockSize >= best) {
                best = blockSize;
                idx = i;
            }
        }
        i = links[i].next;
    }
    return idx;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_evictCheapestBlock);
#ifdef SKIP_ASM
extern "C" void vramHeap_freeBlock(void* selfp, int idx);

// PORT: PS2-only MMI count-leading-sign-bits (plzcw); off-PS2 use a clz builtin. Volatile here: this caller's
// schedule keeps the prologue and the loop constants on either side of it.
static inline int railFloorLog2v(int x)
{
    long r;
    __asm__ __volatile__("plzcw %0, %1\n\txori %0, %0, 0x1f\n\taddiu %0, %0, -1" : "=r"(r) : "0"(x));
    return r;
}

/**
 * Evicts the cheapest used block of size's size class that is large enough.
 *
 * Walks list A of that class, skipping blocks smaller than size, locked ones (flag bit 1) and those owned by skipA or
 * skipB. A block whose owner texture is used again later this frame (its last use in cPS2DeferManager_textureUseTable is after age) costs
 * size times its use count, any other costs 0; the cheapest is freed with vramHeap_freeBlock.
 *
 * @param size  units needed
 * @param skipA  owner to keep
 * @param skipB  another owner to keep
 * @param age  current position in the frame's draw order
 * @return true if a block was freed
 */
extern "C" bool vramHeap_evictCheapestBlock(sRailNodeMan* self, int size, int skipA, int skipB, int age)
{
    int i = self->headsA[railFloorLog2v(size) - 6].mSNext;
    int bestIdx = -1;
    int best = 0x7FFFFFFF;
    for (; i >= 0; i = self->nodes[i].mSNext) {
        sRailNode* n = &self->nodes[i];
        int sz = n->mBlockSize;
        if (sz < size) {
            continue;
        }
        if ((n->mBlockAddr >> 1) & 1) {
            continue;
        }
        int owner = n->mOwner;
        if (owner == skipA || owner == skipB) {
            continue;
        }
        sRailPair66F8* pr = &((sRailPair66F8*)cPS2DeferManager_textureUseTable)[owner];
        int cost = age < pr->a ? sz * pr->b : 0;
        if (cost < best) {
            best = cost;
            bestIdx = i;
        }
    }
    if (bestIdx < 0) {
        return false;
    }
    vramHeap_freeBlock(self, bestIdx);
    return true;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_evictCheapestRun);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void vramHeap_freeBlock(void* selfp, int idx);

/**
 * Frees the cheapest run of neighbouring blocks (in address order) that together hold at least @p size bytes.
 *
 * Despite the unit, this is part of the GS memory block heap that cTextureMan keeps at +0x1F60 and +0x4350. It slides a
 * window along the address-ordered block list (headed at headsB[17].next): a used block that is locked (flag bit 1) or
 * owned by @p skipA or @p skipB breaks the window. A used block whose owner's entry in the 2000-pair table cPS2DeferManager_textureUseTable
 * has a > @p level adds size * b to the window's cost; free blocks cost nothing. The search stops early at cost 0.
 * Every used block in the winning window is then freed with vramHeap_freeBlock (which also merges free neighbours).
 * vramHeap_allocBlock calls it when freeing a single block (vramHeap_evictCheapestBlock) was not enough.
 *
 * Likely bug: when no window qualifies, bestIdx stays -1 and the freeing loop still reads nodes[-1].
 *
 * @param size  the number of bytes the freed run must hold
 * @param skipA  an owner (texture index) whose blocks must be kept
 * @param skipB  a second owner whose blocks must be kept
 * @param level  the threshold compared with each owner's cPS2DeferManager_textureUseTable a value to decide whether its block costs anything
 */
// Frees the cheapest run of neighbouring blocks (in address order) that together hold at least 'size' bytes;
// locked blocks and blocks of the two given owners break a run, owners above 'level' cost size*weight.
// The two 'skip kept blocks' loops are test-first (if + do/while): the plain while form compiles to an
// unrotated loop (92.15%).
extern "C" void vramHeap_evictCheapestRun(sRailNodeMan* self, int size, int skipA, int skipB, int level)
{
    int freeList[292];
    int bestIdx = -1;
    int best = 0x7FFFFFFF;
    int fl;
    int Start = self->headsB[17].mNext;
    sRailNode* StartPtr = &self->nodes[Start];
    if (Start >= 0 && ((fl = StartPtr->mBlockAddr) & 1)) {
        do {
            if (((fl >> 1) & 1) == 0 && StartPtr->mOwner != skipA && StartPtr->mOwner != skipB) {
                break;
            }
            Start = StartPtr->mNext;
            StartPtr = &self->nodes[Start];
        } while (Start >= 0 && ((fl = StartPtr->mBlockAddr) & 1));
    }
    int End = Start;
    sRailNode* EndPtr = StartPtr;
    int sum = 0;
    int cost = 0;
    while (End >= 0 && best > 0) {
        if (sum >= size) {
            if (cost < best) {
                best = cost;
                bestIdx = Start;
            }
            sRailPair* pr = &((sRailPair*)cPS2DeferManager_textureUseTable)[StartPtr->mOwner];
            sum -= StartPtr->mBlockSize;
            if ((StartPtr->mBlockAddr & 1) && level < pr->a) {
                cost -= StartPtr->mBlockSize * pr->b;
            }
            Start = StartPtr->mNext;
            StartPtr = &self->nodes[Start];
        } else {
            if ((EndPtr->mBlockAddr & 1) &&
                (((EndPtr->mBlockAddr >> 1) & 1) || EndPtr->mOwner == skipA || EndPtr->mOwner == skipB)) {
                Start = EndPtr->mNext;
                StartPtr = &self->nodes[Start];
                if (Start >= 0 && ((fl = StartPtr->mBlockAddr) & 1)) {
                    do {
                        if (((fl >> 1) & 1) == 0 && StartPtr->mOwner != skipA && StartPtr->mOwner != skipB) {
                            break;
                        }
                        Start = StartPtr->mNext;
                        StartPtr = &self->nodes[Start];
                    } while (Start >= 0 && ((fl = StartPtr->mBlockAddr) & 1));
                }
                End = Start;
                EndPtr = StartPtr;
                sum = 0;
                cost = 0;
            } else {
                sRailPair* pr = &((sRailPair*)cPS2DeferManager_textureUseTable)[EndPtr->mOwner];
                sum += EndPtr->mBlockSize;
                if ((EndPtr->mBlockAddr & 1) && level < pr->a) {
                    cost += EndPtr->mBlockSize * pr->b;
                }
                End = EndPtr->mNext;
                EndPtr = &self->nodes[End];
            }
        }
    }
    if (sum >= size && cost < best) {
        best = cost;
        bestIdx = Start;
    }
    int n = 0;
    int i = bestIdx;
    for (int got = 0; got < size; ) {
        if (self->nodes[i].mBlockAddr & 1) {
            freeList[n++] = i;
        }
        got += self->nodes[i].mBlockSize;
        i = self->nodes[i].mNext;
    }
    for (int k = 0; k < n; k++) {
        vramHeap_freeBlock(self, freeList[k]);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", vramHeap_allocBlock);
#ifdef SKIP_ASM
extern "C" void vramHeap_allocFromBlock(sRailNodeMan* self, int idx, int size);
extern "C" bool vramHeap_evictCheapestBlock(sRailNodeMan* self, int size, int skipA, int skipB, int level);
extern "C" void vramHeap_evictCheapestRun(sRailNodeMan* self, int size, int skipA, int skipB, int level);

/**
 * Allocates a block of at least @p size bytes from a GS memory block heap for @p owner and returns its node index.
 *
 * When @p align is set the size is rounded up to a multiple of 0x2000 (one GS page); sizes below 0x100 become 0x100. It
 * loops until a block is found: vramHeap_findFreeBlock looks for a free block of that size in the size-class lists, and on
 * success vramHeap_allocFromBlock splits it and marks it used and the node's owner field (+0x8) is set to @p owner. Otherwise it
 * evicts other textures' blocks, first the cheapest single block (vramHeap_evictCheapestBlock), then, if that freed nothing, the
 * cheapest run of neighbours (vramHeap_evictCheapestRun); blocks of @p owner and @p skipB are never evicted. There is no failure
 * path: if nothing can be evicted the loop never ends.
 *
 * The callers in render/ps2graphicsman pass the cTextureMan heap at +0x1F60 (or +0x4350) and read the node's VRAM
 * address from its flags word (bits 8 up).
 *
 * @param size  the number of bytes wanted
 * @param owner  the texture index that will own the block (stored at node +0x8); its blocks are not evicted
 * @param skipB  a second texture index whose blocks are not evicted (-1 for none)
 * @param level  passed to the eviction helpers as their cost threshold
 * @param align  non-zero to round @p size up to a 0x2000-byte page
 * @return the index of the allocated node
 */
extern "C" int vramHeap_allocBlock(sRailNodeMan* self, int size, int owner, int skipB, int level, int align)
{
    int idx;
    if (align) {
        size += -size & 0x1FFF;
    }
    if (size < 0x100) {
        size = 0x100;
    }
    for (;;) {
        idx = vramHeap_findFreeBlock(self, size);
        if (idx >= 0) {
            vramHeap_allocFromBlock(self, idx, size);
            self->nodes[idx].mOwner = owner;
            return idx;
        }
        if (!vramHeap_evictCheapestBlock(self, size, owner, skipB, level)) {
            vramHeap_evictCheapestRun(self, size, owner, skipB, level);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", vramHeap_findOwnerBlock);
#ifdef SKIP_ASM
struct sRailLinkDD8 {
    int flags;      // 0x0
    int f4;
    int owner;      // 0x8 (the owning texture index, vramHeap_allocBlock stores it)
    int fC;
    int next;       // 0x10
    int f14;
    int f18;
};

/**
 * Returns the index of the first used block owned by @p key in a GS memory block heap, or -1 if there is none.
 *
 * Walks the address-ordered block list (head at +0x23C0, nodes at +0x1FF0, link at +0x10) and matches a block whose
 * flags (bit 0, used) are set and whose owner word (+0x8, where vramHeap_allocBlock stores the owning texture index)
 * equals @p key.
 *
 * @param key  the owner (texture index) to look for
 * @return the node index of that owner's first block in address order, or -1
 */
extern "C" int vramHeap_findOwnerBlock(void* self, int key)
{
    char* heap = (char*)self;
    int i = *(int*)(heap + 0x23C0);
    while (i >= 0) {
        sRailLinkDD8* links = *(sRailLinkDD8**)(heap + 0x1FF0);
        if ((links[i].flags & 1) && links[i].owner == key) {
            return i;
        }
        i = links[i].next;
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", vramHeap_freeTextureBlocks);
#ifdef SKIP_ASM
extern "C" void vramHeap_freeBlock(void* self, int id);

/**
 * Frees the GS memory blocks of texture @p i: its data block (node index at record +0x30) and its CLUT block (+0x34).
 *
 * The texture record is entry @p i of the record table whose pointer the heap keeps at +0x23E8. Each index that is not
 * negative is passed to vramHeap_freeBlock, which releases the block and resets the record's address and node fields to -1.
 * cTextureMan_dropVramCopy calls it on the cTextureMan heap at +0x1F60 or, for format 9, the one at +0x4350.
 *
 * @param i  the texture index (slot in the texture record table)
 */
extern "C" void vramHeap_freeTextureBlocks(void* self, int i)
{
    tTexInfo* entry = (tTexInfo*)(*(char***)((char*)self + 0x23E8))[i];
    if (entry->mTexBlock >= 0) {
        vramHeap_freeBlock(self, entry->mTexBlock);
    }
    if (entry->mCLUTBlock >= 0) {
        vramHeap_freeBlock(self, entry->mCLUTBlock);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTextureMan_rearrangeMipLevels);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" void tex_rearrange8BitTexture(void* self, int w, int h, void* buf, char* src);
extern char str_TempTex[];


/**
 * Converts the pixel data of every mip level of @p tex in place with tex_rearrange8BitTexture, then sets its format code to 10.
 *
 * Allocates a temporary buffer of the texture's size (+0x10) with tag "TempTex" and the manager's memory type (+0x1F58)
 * XOR 0x100. For each level 0 to mipCount (+0x24) it takes the width and height from the TEX0 register value at +0x38
 * (TW, TH), copies w * h * bpp / 8 bytes (at least 0x100) of the level to the buffer and lets tex_rearrange8BitTexture write the
 * re-laid-out level back over the source, then halves both sizes. Frees the buffer and stores 10 in the format code at
 * +0xC (the caller only converts format 9 textures).
 *
 * @param tex  the texture record to convert
 */
void cTextureMan::rearrangeMipLevels(tTexInfo* tex)
{
    int n = 0;
    char* buf = new (str_TempTex, TexMemType ^ 0x100, 0) char[tex->mSize];
    int tw = (tex->mTEX0 >> 26) & 0xF;
    int th = (tex->mTEX0 >> 30) & 0xF;
    int bpp = tex->mBPP;
    int w = 1 << tw;
    int height = 1 << th;
    char* src = ((char*)tex->mpTexels);
    if (tex->mNumMips >= 0) {
        do {
            n++;
            int bytes = w * height * bpp / 8;
            if (bytes <= 0xFF) {
                bytes = 0x100;
            }
            MEM_copy(buf, src, bytes);
            tex_rearrange8BitTexture(this, w, height, buf, src);
            src += bytes;
            w >>= 1;
            height >>= 1;
        } while (n <= tex->mNumMips);
    }
    if (buf != 0) {
        cMemMan_free(buf);
    }
    tex->mFormat = 10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTextureMan_SetTextureMemType__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the memory type (+0x1F58) the texture manager uses for its temporary allocations.
 *
 * @param val  the new memory type (cPS2GraphicsMan_InitTextures passes its memtype)
 */
void cTextureMan::SetTextureMemType(int val)
{
    TexMemType = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", cTextureMan_init);
#ifdef SKIP_ASM
extern "C" void cTextureMan_InitTexInfo(cTextureMan* self);
extern "C" void vramHeap_init(void* list, int a, int size, void* pool);

/**
 * Initialises the texture manager: clears its tables and sets up both GS memory block heaps.
 *
 * Zeroes the memory type (+0x1F58), resets the texture record table and free id list with cTextureMan_InitTexInfo, marks no
 * texture bound to either context (+0x1F4C, +0x1F50) and no CLUT loaded (+0x1F54) with -1, then vramHeap_init sets up
 * the heap at +0x1F60 over 0x400000 bytes (the 4 MB of GS memory) and the one at +0x4350 over 0x1000 bytes, both
 * pointing at the record table at +0x8. cPS2GraphicsMan_Init calls it right after allocating the manager.
 */
void cTextureMan::init()
{
    TexMemType = 0;
    InitTexInfo();
    int* bound = (int*)(&mBoundTex[0]);
    for (int i = 1; i >= 0; i--) {
        bound[i] = -1;
    }
    mCLUTTex = -1;
    vramHeap_init(mMainHeap, 0, 0x400000, &m_pTexInfo[0]);
    vramHeap_init(mTempHeap, 0, 0x1000, &m_pTexInfo[0]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", cTextureMan_freeAllHeapBlocks);
#ifdef SKIP_ASM
extern "C" void vramHeap_freeAllBlocks(void* list);

/**
 * Frees every block in both GS memory block heaps of the texture manager and forgets the bound textures.
 *
 * Calls vramHeap_freeAllBlocks (which clears each block's locked bit and frees it) on the heap at +0x4350 and then on the one at
 * +0x1F60, then sets the bound texture of both contexts (+0x1F4C, +0x1F50) and the loaded CLUT texture (+0x1F54) to -1.
 * cPS2GraphicsMan_applyDisplayMode calls it before reserving the frame and depth buffers.
 */
void cTextureMan::freeAllHeapBlocks()
{
    vramHeap_freeAllBlocks(mTempHeap);
    vramHeap_freeAllBlocks(mMainHeap);
    int* bound = (int*)(&mBoundTex[0]);
    for (int i = 1; i >= 0; i--) {
        bound[i] = -1;
    }
    mCLUTTex = -1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTextureMan_InitTexInfo);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Resets the texture manager's record table and its free list of non-bound texture ids.
 *
 * Zeroes the two words at +0x0 and +0x4 (the latter counts allocated ids) and the 2000-entry record table at +0x8 (8000
 * bytes), then points the free list head (+0x1F48) at slot 1500 (+0x1778) and makes each of slots 1500 to 1998 point at
 * the next one; slot 1999 stays 0 and ends the list. cTextureMan_NewNonBindTexID pops ids from this list.
 */
void cTextureMan::InitTexInfo()
{
    int i;
    m_iNumBindTextures = 0;
    m_iNumNonBindTextures = 0;
    MEM_fill(&m_pTexInfo[0], 0, 8000);
    m_pNonBindFreeTexInfo = (cLinkTexInfo*)((void**)(&m_pTexInfo[1500]));
    for (i = 0; i < 499; i++) {
        void** list = (void**)m_pNonBindFreeTexInfo;
        list[i] = &list[i + 1];
    }
}
#endif


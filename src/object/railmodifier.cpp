#include "common.h"
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

struct cRailModifier_C0E8 {
    char pad_0x0[0x34];
    int idx;                // 0x34
    char pad_0x38[0x8];
    sModel_C0E8* model;     // 0x40
    void* node;             // 0x44
    char pad_0x48[0x8];
    t4Matrix mat;          // 0x50
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
 * The rail's frame is the instance's world matrix or, for a mesh index (+0x34) of 0 or more, that mesh's
 * chain of link matrices (translations scaled by the instance scale) times the world matrix. The inverse
 * (transposed rotation, rotated negative translation) is stored at +0x50. Then allocates a 0x10-byte
 * "RailNode" (type 2, pointing back here) at +0x44.
 */
extern "C" void cRailModifier_buildXform(cRailModifier_C0E8* self)
{
    sModel_C0E8* model = self->model;
    sLink_C0E8* links = model->tbl->links;
    t4Matrix* xform;
    if (self->idx < 0) {
        t4Matrix* selfMat = &self->mat;
        vu0CopyMat_C0E8(selfMat, &model->mat);
        xform = selfMat;
    } else {
        float modelScale = model->scale;
        t4Vector scl;
        scl.x = modelScale;
        scl.y = modelScale;
        scl.z = modelScale;
        scl.w = 1.0f;
        t4Matrix* selfMat = &self->mat;
        sLink_C0E8* link = (sLink_C0E8*)(self->idx * 16 + (int)links);
        vu0CopyMat_C0E8(selfMat, link->mat);
        xform = selfMat;
        {
        t4Vector rr;
        t4Vector origin = self->mat.row[3];
        rr.x = origin.x * scl.x;
        rr.y = origin.y * scl.y;
        rr.z = origin.z * scl.z;
        rr.w = origin.w * scl.w;
        self->mat.row[3] = rr;
        int i = link->next;
        while (i != -1) {
            sLink_C0E8* nextLink = (sLink_C0E8*)(i * 16 + (int)links);
            t4Matrix* mm = xform;
            t4Matrix productCopy;
            t4Matrix linkMat;
            t4Vector res;
            t4Vector r3b;
            t4Matrix* lm = nextLink->mat;
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
            i = nextLink->next;
        }
        }
        t4Matrix worldCopy;
        t4Matrix worldProduct;
        t4Matrix* pt3 = &worldProduct;
        t4Matrix* pt4 = &worldCopy;
        vu0MulMat_C0E8(pt3, &self->model->mat, xform);
        vu0CopyMat_C0E8(pt4, &worldProduct);
        vu0CopyMat_C0E8(xform, pt4);
    }
    float swapValue;
    swapValue = self->mat.row[0].y;
    self->mat.row[0].y = self->mat.row[1].x;
    self->mat.row[1].x = swapValue;
    swapValue = self->mat.row[0].z;
    self->mat.row[0].z = self->mat.row[2].x;
    self->mat.row[2].x = swapValue;
    swapValue = self->mat.row[1].z;
    self->mat.row[1].z = self->mat.row[2].y;
    self->mat.row[2].y = swapValue;
    t4Vector invTranslation = vu0Scale(row_C0E8(&self->mat, 3), -1.0f);
    invTranslation.w = 0.0f;
    invTranslation = vu0MtxMul_C0E8(xform, invTranslation);
    invTranslation.w = 1.0f;
    self->mat.row[3] = invTranslation;
    void* n = cMemMan_alloc(0x10, str_RailNode, 0x20000000, 0);
    ((void**)n)[3] = self;
    ((int*)n)[2] = 2;
    self->node = n;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_update);
#ifdef SKIP_ASM
struct sBox0035C4E0 {
    float min[4];
    float max[4];
} __attribute__((aligned(16)));


extern "C" void cRailModifier_updateBoundBox(void* self);
extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, int id, sBox0035C4E0* box, sBox0035C4E0* old);

/**
 * Recomputes the rail's world bound box and moves its rail-space node in the Nash octree from the old box to the
 * new one.
 *
 * Calls cRailModifier_updateBoundBox, then nash_Nash_movenode(Nash, 2 (the rail space), Node, new box, old box).
 */
extern "C" void cRailModifier_update(cRailModifier* self)
{
    sBox0035C4E0* box = (sBox0035C4E0*)&self->BoundBox;
    sBox0035C4E0 old = *(sBox0035C4E0*)&self->BoundBox;
    cRailModifier_updateBoundBox((self));
    nash_Nash_movenode(cObjectInterface_getNash(), 2, (int)self->Node, box, &old);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream0035C540 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void write(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual void v03(void* buf, int size);
    virtual void v04(void* buf, int size);
    virtual void writeReplayInstance(void* p);
};

/**
 * Writes the rail modifier to a replay frame: the 0x30-byte cRailModifierState (+0x10: BoundBox, SplineId, Mesh)
 * with slot 1, then Instance with slot 5.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
extern "C" void cRailModifier_writeToReplayFrame(cRailModifier* self, cStream0035C540* stream)
{
    stream->write((char*)&self->BoundBox, 0x30);
    stream->writeReplayInstance(self->Instance);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_getInvMat);
#ifdef SKIP_ASM
struct sRmMat44C5A0 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMatC5A0(sRmMat44C5A0* d, const sRmMat44C5A0* a, const sRmMat44C5A0* b)
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
static inline void vu0CopyMatC5A0(sRmMat44C5A0* d, const sRmMat44C5A0* s)
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

extern "C" void cInstanceNode_buildMeshMat(void* model, int bone, sRmMat44C5A0* out);

/**
 * Builds the rail's inverse transform: the instance's mesh matrix (cInstanceNode_buildMeshMat, cInstanceNode::buildMeshMat with
 * Instance and Mesh) multiplied with XForm (+0x50), into @p mat.
 *
 * @param mat  receives the matrix (On Tour t4Matrix& mat)
 */
extern "C" void cRailModifier_getInvMat(cRailModifier* self, sRmMat44C5A0* mat)
{
    cInstanceNode_buildMeshMat(self->Instance, self->Mesh, mat);
    sRmMat44C5A0* src = (sRmMat44C5A0*)&self->XForm;
    sRmMat44C5A0 copy;
    sRmMat44C5A0 product;
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

// PORT: PS2-only VU0 inline asm (v * s).
// A spline segment: cubic coefficient matrix (rows t^3, t^2, t, 1), next segment.
struct sRmSeg_C698 {
    char pad_0x00[0x10];
    sRmMat44 coef;          // 0x10
    char pad_0x50[0x14];
    sRmSeg_C698* next;      // 0x64
};

// A rail spline resource.
struct sRmSpline_C698 {
    char pad_0x00[0x1C];
    unsigned int mask;      // 0x1C
    int numSegs;            // 0x20
    sRmSeg_C698* segs;      // 0x24
    int id;                 // 0x28
};

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


// Hit record (position, normal, distance, type, ids, spline parameter).
struct sRmHit_C698 {
    t4Vector pos;       // 0x00
    t4Vector nrm;       // 0x10
    t4Vector v20;       // 0x20
    t4Vector v30;       // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    int pad48;
    int id;             // 0x4C
    void* model;        // 0x50
    void* obj;          // 0x54
    void* spline;       // 0x58
    int bone;           // 0x5C
    int f60;            // 0x60
    int f64;            // 0x64
    float t;            // 0x68
    float u;            // 0x6C
    float v;            // 0x70
    int pad74[3];
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

// Power basis (t^3, t^2, t, w) of a cubic parameter, built up from t.
struct sRmPow_C698 : t4Vector {
    sRmPow_C698(const float& t, const float& aw)
    {
        w = aw;
        z = t;
        y = t * t;
        x = t * y;
    }
};

// Five floats (the golden-section start brackets for the five sample points).
struct sRmF5_C698 {
    float v[5];
};

extern "C" sRmWorld_C698** cObjectInterface_getWorld();
extern "C" void cRailModifier_getInvMat_C698(void* self, sRmMat44* m) __asm__("cRailModifier_getInvMat");
extern t4Vector D_004FF120_C698 __asm__("t4vect_Zero");
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
static inline float vu0Length_C698(const t4Vector& v)
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

// Point on a cubic segment at parameter t.
static inline t4Vector evalSeg_C698(const sRmMat44* c, float t)
{
    sRmPow_C698 v(t, 1.0f);
    return vu0MtxMul_C698(c, v);
}

// PORT: SN abs.s asm helper (PS2 FPU).
static inline float fsAbs_C698(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Tangent of a cubic segment at parameter t.
static inline t4Vector evalTan_C698(const sRmMat44* c, float t)
{
    sRmDir_C698 v(t * (3.0f * t), 2.0f * t, 1.0f);
    return vu0MtxMul_C698(c, v);
}

static inline float clamp_C698(float x, float lo, float hi)
{
    return x >= lo ? x <? hi : lo;
}

static inline void resetHit_C698(sRmHit_C698* h)
{
    h->type = 2;
    h->obj = 0;
    h->model = 0;
    h->spline = 0;
    h->bone = -1;
    h->f60 = -1;
    h->f64 = -1;
    h->id = -1;
    h->v20 = D_004FF120_C698;
    h->v30 = D_004FF120_C698;
    h->t = 0;
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
extern "C" void cRailModifier_splineIntersect(cRailModifier* self, t4Vector* box, t4Vector* box2, const t4Vector* point, unsigned int mask,
                              int* found, float* bestDist, sRmHit_C698* out)
{
    sRmSpline_C698* spline = (sRmSpline_C698*)(*(sRmRef_C698*)&self->SplineId).get(*cObjectInterface_getWorld());
    if ((spline->mask & mask) == 0) {
        return;
    }
    sRmMat44 mat;
    cRailModifier_getInvMat_C698((self), &mat);
    sRmSeg_C698* seg = spline->segs;
    for (int i = 0; i < spline->numSegs; i++, seg = seg->next) {
        float tLo = 0.0f;
        float tHi = 1.0f;
        t4Vector pos;
        t4Vector next;
        t4Vector dir;
        sRmMat44 segMat;
        {
            sRmMat44 t;
            vu0MulMat_C698(&t, &mat, &seg->coef);
            vu0CopyMat_C698(&segMat, &t);
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
        float t = td;
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
                t = td;
                dist = fd;
            } else {
                tHi = td;
                td = tc;
                tc = td * 0.618034f + tLo * 0.381966f;
                pos = evalSeg_C698(&segMat, tc);
                float probeDist = vu0Length_C698(vu0Sub_C698(pos, *point));
                fd = fc;
                fc = probeDist;
                t = tc;
                dist = fc;
            }
        }
        if (*found == 0 || dist < *bestDist) {
            *found = 1;
            *bestDist = dist;
            resetHit_C698(out);
            out->pos = pos;
            out->nrm = vu0Normal_C698(evalTan_C698(&segMat, t));
            out->id = spline->id;
            out->model = (void*)self->Instance;
            out->bone = self->Mesh;
            out->t = t;
            out->spline = spline;
            out->obj = 0;
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
void cRailModifier_render(void* self)
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
void* cRigidBody_cRigidBody(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_setW);
#ifdef SKIP_ASM
struct sVec4CFF0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat33CFF0 {
    float m[3][3];
};

struct sBodyCFF0 {
    char pad_0x00[0x30];
    sVec4CFF0 pos;      // 0x30
    char pad_0x40[0x10];
    sMat33CFF0 rot;     // 0x50
    char pad_0x74[0x1C];
    sVec4CFF0 outPos;   // 0x90
};

static inline sMat33CFF0 scaleCFF0(const sMat33CFF0& a, float s)
{
    sMat33CFF0 r;
    r.m[0][0] = a.m[0][0] * s;
    r.m[0][1] = a.m[0][1] * s;
    r.m[0][2] = a.m[0][2] * s;
    r.m[1][0] = a.m[1][0] * s;
    r.m[1][1] = a.m[1][1] * s;
    r.m[1][2] = a.m[1][2] * s;
    r.m[2][0] = a.m[2][0] * s;
    r.m[2][1] = a.m[2][1] * s;
    r.m[2][2] = a.m[2][2] * s;
    return r;
}

static inline void invertCFF0(sMat33CFF0& a)
{
    sMat33CFF0 adj;
    adj.m[0][0] = a.m[1][1] * a.m[2][2] - a.m[1][2] * a.m[2][1];
    adj.m[0][1] = a.m[2][1] * a.m[0][2] - a.m[2][2] * a.m[0][1];
    adj.m[0][2] = a.m[0][1] * a.m[1][2] - a.m[0][2] * a.m[1][1];
    adj.m[1][0] = a.m[2][0] * a.m[1][2] - a.m[2][2] * a.m[1][0];
    adj.m[1][1] = a.m[0][0] * a.m[2][2] - a.m[0][2] * a.m[2][0];
    adj.m[1][2] = a.m[1][0] * a.m[0][2] - a.m[1][2] * a.m[0][0];
    adj.m[2][0] = a.m[1][0] * a.m[2][1] - a.m[1][1] * a.m[2][0];
    adj.m[2][1] = a.m[2][0] * a.m[0][1] - a.m[2][1] * a.m[0][0];
    adj.m[2][2] = a.m[0][0] * a.m[1][1] - a.m[0][1] * a.m[1][0];
    float inv = a.m[0][0] * adj.m[0][0] + a.m[1][0] * adj.m[0][1] + a.m[2][0] * adj.m[0][2];
    inv = 1.0f / inv;
    a = scaleCFF0(adj, inv);
}

static inline sVec4CFF0 xformCFF0(const sMat33CFF0& m, sBodyCFF0* b)
{
    sVec4CFF0 r;
    float x = b->outPos.x;
    float y = b->outPos.y;
    float z = b->outPos.z;
    r.x = m.m[0][0] * x + m.m[1][0] * y + m.m[2][0] * z;
    r.y = m.m[0][1] * x + m.m[1][1] * y + m.m[2][1] * z;
    r.z = m.m[0][2] * x + m.m[1][2] * y + m.m[2][2] * z;
    r.w = b->outPos.w;
    return r;
}

/**
 * Sets a rigid body's angular velocity and recomputes its angular momentum to match.
 *
 * Stores newPos as W (+0x90), inverts the inverse inertia InvMOI (+0x50, 3x3, by adjugate over determinant) and sets
 * the angular momentum (+0x30) to that inertia times W.
 *
 * @param newPos  new angular velocity W
 */
extern "C" void cRigidBody_setW(sBodyCFF0* self, const sVec4CFF0* newPos)
{
    self->outPos = *newPos;
    sMat33CFF0 invRot = self->rot;
    invertCFF0(invRot);
    self->pos = xformCFF0(invRot, self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_updateImplicit);
#ifdef SKIP_ASM
struct sRmVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRmMat33 {
    float m[3][3];
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sRmVec4 vu0ScaleRM(const sRmVec4& v, float s)
{
    sRmVec4 r;
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

struct sRmBody {
    char pad_0x00[0x20];
    sRmVec4 dir;        // 0x20
    sRmVec4 pos;        // 0x30
    float scale;        // 0x40
    char pad_0x44[0xC];
    sRmMat33 rot;       // 0x50
    char pad_0x74[0xC];
    sRmVec4 outDir;     // 0x80
    sRmVec4 outPos;     // 0x90
};

static inline sRmVec4 xformRM(sRmBody* b)
{
    sRmVec4 r;
    float x = b->pos.x;
    float y = b->pos.y;
    float z = b->pos.z;
    r.x = b->rot.m[0][0] * x + b->rot.m[1][0] * y + b->rot.m[2][0] * z;
    r.y = b->rot.m[0][1] * x + b->rot.m[1][1] * y + b->rot.m[2][1] * z;
    r.z = b->rot.m[0][2] * x + b->rot.m[1][2] * y + b->rot.m[2][2] * z;
    r.w = b->pos.w;
    return r;
}

/**
 * Recomputes a rigid body's velocities from its momenta.
 *
 * V (+0x80) = linear momentum (+0x20) times the inverse mass (+0x40); W (+0x90) = the world inverse inertia matrix
 * (InvMOI, 3x3 at +0x50) applied to the angular momentum (+0x30), keeping its w. Called after the inertia changes
 * (cRollerModifier_updateMOI) and after each simulation step.
 */
extern "C" void cRigidBody_updateImplicit(sRmBody* self)
{
    self->outDir = vu0ScaleRM(self->dir, self->scale);
    self->outPos = xformRM(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_stepMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

struct sRmBody_D340 {
    t4Vector pos;       // 0x00
    t4Vector quat;      // 0x10
    t4Vector acc0;      // 0x20
    t4Vector acc1;      // 0x30
    char pad_0x40[0x40];
    t4Vector vel;       // 0x80
    t4Vector angVel;    // 0x90
};

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
 * @param body  the rigid body
 * @param force  force applied over the step
 * @param torque  torque applied over the step
 * @param dt  time step
 */
extern "C" void cRigidBody_stepMotion(sRmBody_D340* body, const t4Vector* force, const t4Vector* torque, float dt)
{
    t4Vector step;
    ScaleD340(step, body->vel, dt);
    AddEqD340(body->pos, step);
    {
        t4Vector quatDeriv;
        QuatDerivD340(quatDeriv, body->quat, body->angVel);
        ScaleD340(step, quatDeriv, dt);
    }
    body->quat.x += step.x;
    body->quat.y += step.y;
    body->quat.z += step.z;
    body->quat.w += step.w;
    NormalizeD340(body->quat);
    ScaleD340(step, *force, dt);
    AddEqD340(body->acc0, step);
    ScaleD340(step, *torque, dt);
    AddEqD340(body->acc1, step);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_rigidImpact);
#ifdef SKIP_ASM
extern "C" void func_0035D908(sRmBody* b, const sRmVec4* r, const sRmVec4* imp);

struct sRmRigid_D4A0 {
    sRmVec4 pos;        // 0x00
    char pad_0x10[0x30];
    float invMass;      // 0x40
    char pad_0x44[0xC];
    sRmMat33 invI;      // 0x50
    char pad_0x74[0xC];
    sRmVec4 vel;        // 0x80
    sRmVec4 angVel;     // 0x90
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline sRmVec4 SubD4A0(const sRmVec4& a, const sRmVec4& b)
{
    sRmVec4 r;
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
static inline sRmVec4 AddD4A0(const sRmVec4& a, const sRmVec4& b)
{
    sRmVec4 r;
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
static inline sRmVec4 CrossD4A0(const sRmVec4& a, const sRmVec4& b)
{
    sRmVec4 r;
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
static inline float DotD4A0(const sRmVec4& a, const sRmVec4& b)
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
static inline float LengthD4A0(const sRmVec4& v)
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
static inline sRmVec4 DivD4A0(const sRmVec4& v, float s)
{
    sRmVec4 r;
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
static inline sRmVec4 ScaleD4A0(const sRmVec4& v, float s)
{
    sRmVec4 r;
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

static inline sRmVec4 MulID4A0(sRmRigid_D4A0* b, const sRmVec4& v)
{
    sRmVec4 r;
    float x = v.x;
    float y = v.y;
    float z = v.z;
    r.x = b->invI.m[0][0] * x + b->invI.m[1][0] * y + b->invI.m[2][0] * z;
    r.y = b->invI.m[0][1] * x + b->invI.m[1][1] * y + b->invI.m[2][1] * z;
    r.z = b->invI.m[0][2] * x + b->invI.m[1][2] * y + b->invI.m[2][2] * z;
    r.w = v.w;
    return r;
}

static inline sRmVec4 PointVelD4A0(sRmRigid_D4A0* b, const sRmVec4& p)
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
extern "C" void cRigidBody_rigidImpact(sRmRigid_D4A0* body, sRmVec4* point, sRmVec4* n, float restitution, float mu)
{
    sRmVec4 arm = SubD4A0(*point, body->pos);
    sRmVec4 vp = PointVelD4A0(body, *point);
    float normalVel = DotD4A0(vp, *n);
    if (mu != 0.0f) {
        sRmVec4 tangentVel = SubD4A0(vp, ScaleD4A0(*n, normalVel));
        float len = LengthD4A0(tangentVel);
        if (len > 0.0010000000474974513f) {
            sRmVec4 dir = DivD4A0(tangentVel, len);
            float j = -mu * len / (body->invMass + DotD4A0(dir, CrossD4A0(MulID4A0(body, CrossD4A0(arm, dir)), arm)));
            sRmVec4 imp = ScaleD4A0(dir, j);
            func_0035D908((sRmBody*)body, &arm, &imp);
            vp = PointVelD4A0(body, *point);
            normalVel = DotD4A0(vp, *n);
            if (normalVel > 0.0f) {
                return;
            }
        }
    }
    float j = -(restitution + 1.0f) * normalVel / (body->invMass + DotD4A0(*n, CrossD4A0(MulID4A0(body, CrossD4A0(arm, *n)), arm)));
    sRmVec4 imp = ScaleD4A0(*n, j);
    func_0035D908((sRmBody*)body, &arm, &imp);
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035D908);

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_writeToReplayFrame);
#ifdef SKIP_ASM
class cRigidBody_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void write(void* buf, int size);
};

/**
 * Writes a rigid body's state (its first 0x50 bytes: pose, momentum, inverse mass) to a replay frame.
 *
 * @param obj  the replay frame (On Tour cBxReplayFrame*)
 */
extern "C" void cRigidBody_writeToReplayFrame(cRigidBody* self, cRigidBody_writeToReplayFrame_cObj* obj)
{
    obj->write(self, 80);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_readFromReplayFrame);
#ifdef SKIP_ASM
class cRigidBody_readFromReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void read(void* buf, int size);
};

/**
 * Restores a rigid body's state from a replay frame.
 *
 * Reads 0x50 bytes (the momenta, pose and mass fields) straight into the body through the frame's virtual read
 * (slot 2). The counterpart of cRigidBody_writeToReplayFrame.
 *
 * @param obj  the replay frame to read from (On Tour data)
 */
extern "C" void cRigidBody_readFromReplayFrame(void* self, cRigidBody_readFromReplayFrame_cObj* obj)
{
    obj->read(self, 80);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_cRollerModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
extern void* cRollerModifier_vtbl[];
void* cRigidBody_cRigidBody(void* self);
extern "C" void SphereTree_SphereTree(void* p);
extern "C" void* bxSphereTree_operatorE(void* self, void* src);
extern "C" void cRollerModifier_updateMOI(void* self);

extern "C" void* cRollerModifier_Collision_DA70(void* self, void* a1, void* a2) __asm__("cRollerModifier_Collision");


extern "C" tPose tPose_tPose(void* m);
extern "C" tPose cRollerModifier_getBodyPose_DA70(void* self) __asm__("cRollerModifier_getBodyPose");

struct sV4c_DA70 {
    float x, y, z, w;
    sV4c_DA70(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sRmVec4 QuatRot_DA70(const sRmVec4& q, const sRmVec4& v)
{
    sRmVec4 r;
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
static inline void QuatRotTo_DA70(sRmVec4& dst, const sRmVec4& q, const sRmVec4& v)
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
static inline void AddTo_DA70(sRmVec4& dst, const sRmVec4& b)
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
static inline void QuatPosToMtx_DA70(void* m, const sRmVec4& q, const sRmVec4& p)
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
extern "C" char* cRollerModifier_cRollerModifier(cRollerModifier* self, char* rdata, char* ii, char* ci)
{
    float* timing = (float*)(&self->mKineticEnergy);
    *(void***)self = cTransformModifier_vtbl;
    cRigidBody_cRigidBody(&self->Pose);
    *(void***)self = cRollerModifier_vtbl;
    SphereTree_SphereTree(&self->SphTree);
    *(int*)&self->Life = 0;
    self->m_pInstance = (tInstanceRecord*)(*(int*)(ii + 0x50));
    bxSphereTree_operatorE(&self->SphTree, *(void**)(ii + 0x48));
    self->mMass = *(float*)(rdata + 0x4);
    char* info = (char*)self->SphTree.treeInfo;
    self->mElast = *(float*)(rdata + 0x8);
    self->InvM = 1.0f / timing[1];
    sRmVec4 offset;
    offset.x = *(float*)(info + 0x2C);
    offset.y = *(float*)(info + 0x30);
    offset.z = *(float*)(info + 0x34);
    offset.w = 0.0f;
    *(sRmVec4*)&self->COMOffset = offset;
    *(sRmMat33*)&self->InvBodyMOI[0] = *(sRmMat33*)(info + 0x5C);
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[0][0] *= 0.5f;
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[0][1] *= 0.5f;
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[0][2] *= 0.5f;
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[1][0] *= 0.5f;
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[1][1] *= 0.5f;
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[1][2] *= 0.5f;
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[2][0] *= 0.5f;
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[2][1] *= 0.5f;
    ((sRmMat33*)(&self->InvBodyMOI[0]))->m[2][2] *= 0.5f;
    tPose x = tPose_tPose((char*)self->m_pInstance + 0x10);
    {
        sRmVec4 rotated;
        QuatRotTo_DA70(rotated, *(sRmVec4*)&x.Q, *(sRmVec4*)&self->COMOffset);
        offset = rotated;
        sRmVec4 shift;
        sRmVec4 rotatedAgain;
        QuatRotTo_DA70(rotatedAgain, *(sRmVec4*)&x.Q, offset);
        shift = rotatedAgain;
        AddTo_DA70(*(sRmVec4*)&x.P, shift);
    }
    *(&self->Pose) = x;
    tPose y = cRollerModifier_getBodyPose_DA70(((char*)self));
    QuatPosToMtx_DA70(&self->Transform, *(sRmVec4*)&y.Q, *(sRmVec4*)&y.P);
    cRollerModifier_updateMOI(((char*)self));
    cRigidBody_updateImplicit((sRmBody*)(&self->Pose));
    self->ContactElastic = 0.10000000149011612f;
    self->ContactFriction = 0.4000000059604645f;
    self->StopContactSpeed = 277.77777099609375f;
    *(int*)&self->ViscousDrag = 0;
    SetBits_DA70((unsigned int*)((char*)self->m_pInstance + 0x8), 0xFFFFFFDFu, 0x40);
    SetBits_DA70((unsigned int*)((char*)self->m_pInstance + 0x8), 0xFFFFFFFDu, 0x4);
    *(unsigned int*)((char*)self->m_pInstance + 0x8) |= 0x1;
    cRollerModifier_Collision_DA70(((char*)self), ci, ii);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_Collision);
#ifdef SKIP_ASM
struct sRmV4_35DDE8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// tCollideInfo (On Tour name)
struct sRmPath_35DDE8 {
    char pad_0x0[0x10];
    sRmV4_35DDE8 dir;   // 0x10
    sRmV4_35DDE8 up;    // 0x20
    float len;          // 0x30
};

// PORT: PS2-only VU0 inline asm (a + b), by value.
static inline sRmV4_35DDE8 vu0AddR_35DDE8(const sRmV4_35DDE8& a, const sRmV4_35DDE8& b)
{
    sRmV4_35DDE8 r;
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
static inline sRmV4_35DDE8 vu0DivR_35DDE8(const sRmV4_35DDE8& v, float s)
{
    sRmV4_35DDE8 r;
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
static inline sRmV4_35DDE8 vu0ScaleR_35DDE8(const sRmV4_35DDE8& v, float s)
{
    sRmV4_35DDE8 r;
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
static inline float vu0Dot_35DDE8(const sRmV4_35DDE8& a, const sRmV4_35DDE8& b)
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

struct sV4K_35DDE8 : public sRmV4_35DDE8 {
    sV4K_35DDE8(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

struct cRigidBody_DDE8 {
    char pad_0x00[0x20];
    sRmV4_35DDE8 P;         // 0x20
    sRmV4_35DDE8 L;         // 0x30
    float InvM;             // 0x40
    char pad_0x44[0x3C];
    sRmV4_35DDE8 V;         // 0x80
    sRmV4_35DDE8 W;         // 0x90
    float getInvM() const { return InvM; }
    void setW(const sRmV4_35DDE8& w) __asm__("cRigidBody_setW");
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
    void setV(const sRmV4_35DDE8& v)
    {
        P = vu0DivR_35DDE8(v, getInvM());
        V = v;
    }
    void Collision(const sRmPath_35DDE8* tc) __asm__("cRollerModifier_Collision");
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
void cRollerModifier_DDE8::Collision(const sRmPath_35DDE8* tc)
{
    float addZ = 5000.0f / mMass + 100.0f;
    HitFrequency = 0.0f;
    if (addZ > 600.0f) {
        addZ = 600.0f;
    }
    setV(vu0AddR_35DDE8(vu0DivR_35DDE8(vu0ScaleR_35DDE8(tc->dir, tc->len), 2.0f),
                        sV4K_35DDE8(0.0f, 0.0f, addZ, 0.0f)));
    setW(vu0ScaleR_35DDE8(tc->up, 10.0f));
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
extern "C" void SphereTree_SphereTree(void* self);
extern void* cObjectInterface_getWorld_DF70() __asm__("cObjectInterface_getWorld");
void* cWorld_getPolyModelInfo(void* a, void* b);
extern "C" void* tSphTreeList_getMeshTree(void* a, int b);
extern "C" void SphereTree_TransformAbs(void* self, void* m, float scale);
extern "C" void cRollerModifier_updateMOI(void* self);



class cStreamDF70 {
public:
    virtual void v01(void* buf, int size);
    virtual void read(void* buf, int size);
    virtual int readReplayInstance();
};

struct sRmMat33DF70 {
    float m[9];
};

struct sRmMat44DF70 {
    sRmVec4 r[4];
};

struct sRmSrcDF70 {
    char pad_0x00[0x2C];
    float x, y, z;          // 0x2C
    char pad_0x38[0x5C - 0x38];
    sRmMat33DF70 rot;       // 0x5C
};

struct sRailDF70 {
    void** vt;              // 0x00
    char data[0x28];        // 0x04
    char pad_0x2C[0x4];
    char body[0xA0];        // 0x30
    int id;                 // 0xD0
    char pad_0xD4[0xC];
    char mover[0x98];       // 0xE0
    sRmSrcDF70* src;        // 0x178
    char pad_0x17C[0x240 - 0x17C];
    sRmMat44DF70 xf;        // 0x240
    sRmMat33DF70 rot;       // 0x280
    char pad_0x2A4[0xC];
    sRmVec4 off;            // 0x2B0
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
extern "C" sRailDF70* cRollerModifier_cRollerModifier1(sRailDF70* self, cStreamDF70* stream)
{
    self->vt = cTransformModifier_vtbl;
    cRigidBody_cRigidBody(self->body);
    self->vt = cRollerModifier_vtbl;
    SphereTree_SphereTree(self->mover);
    cRigidBody_readFromReplayFrame(self->body, (cRigidBody_readFromReplayFrame_cObj*)stream);
    stream->read(self->data, 40);
    self->id = stream->readReplayInstance();
    void* meshTree = tSphTreeList_getMeshTree(cWorld_getPolyModelInfo(cObjectInterface_getWorld_DF70(), (void*)self->id), 0);
    bxSphereTree_operatorE(self->mover, meshTree);
    sRmSrcDF70* src = self->src;
    sRmVec4 off;
    off.x = src->x;
    off.y = src->y;
    off.z = src->z;
    off.w = 0.0f;
    self->off = off;
    self->rot = src->rot;
    self->rot.m[0] *= 0.5f;
    self->rot.m[1] *= 0.5f;
    self->rot.m[2] *= 0.5f;
    self->rot.m[3] *= 0.5f;
    self->rot.m[4] *= 0.5f;
    self->rot.m[5] *= 0.5f;
    self->rot.m[6] *= 0.5f;
    self->rot.m[7] *= 0.5f;
    self->rot.m[8] *= 0.5f;
    tPose x = cRollerModifier_getBodyPose_DA70(self);
    QuatPosToMtx_DA70(&self->xf, *(sRmVec4*)&x.Q, *(sRmVec4*)&x.P);
    SphereTree_TransformAbs(self->mover, &self->xf, 1.0f);
    cRollerModifier_updateMOI(self);
    cRigidBody_updateImplicit((sRmBody*)self->body);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", cRollerModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cRollerModifier_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void write(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual void v03();
    virtual void v04();
    virtual void writeReplayInstance(void* ptr);
};

/**
 * Writes the roller modifier to a replay frame: its rigid body (+0x30), the 0x28-byte cRollerNodeState (+0x4) and
 * Instance (+0xD0).
 *
 * The same order as On Tour's writeToReplayFrame (cRigidBody first, then the state, then the instance pointer with
 * slot 5).
 *
 * @param obj  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
extern "C" void cRollerModifier_writeToReplayFrame(void* self, cRollerModifier_writeToReplayFrame_cObj* obj)
{
    cRigidBody_writeToReplayFrame((cRigidBody*)((char*)self + 0x30), (cRigidBody_writeToReplayFrame_cObj*)obj);
    obj->write((char*)self + 0x4, 40);
    obj->writeReplayInstance(*(void**)((char*)self + 0xD0));
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_updateMOI);
#ifdef SKIP_ASM
struct sM44_E248 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (unit quaternion -> 4x4 rotation matrix, row 3 = (0,0,0,1)).
static inline void QuatToMtx_E248(sM44_E248& m, const sRmVec4& q)
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
static inline sRmMat33 Get33_E248(const sM44_E248& m)
{
    sRmMat33 r;
    r.m[0][0] = m.m[0][0];
    r.m[0][1] = m.m[0][1];
    r.m[0][2] = m.m[0][2];
    r.m[1][0] = m.m[1][0];
    r.m[1][1] = m.m[1][1];
    r.m[1][2] = m.m[1][2];
    r.m[2][0] = m.m[2][0];
    r.m[2][1] = m.m[2][1];
    r.m[2][2] = m.m[2][2];
    return r;
}

static inline void Transpose33_E248(sRmMat33& m)
{
    float t;
    t = m.m[0][1]; m.m[0][1] = m.m[1][0]; m.m[1][0] = t;
    t = m.m[0][2]; m.m[0][2] = m.m[2][0]; m.m[2][0] = t;
    t = m.m[1][2]; m.m[1][2] = m.m[2][1]; m.m[2][1] = t;
}

// r = b * a (a applied first).
static inline sRmMat33 Mul33_E248(const sRmMat33& a, const sRmMat33& b)
{
    sRmMat33 r;
    r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2];
    r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2];
    r.m[2][0] = a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2];
    r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2];
    r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2];
    r.m[2][1] = a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2];
    r.m[0][2] = a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2];
    r.m[1][2] = a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2];
    r.m[2][2] = a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2];
    return r;
}

struct sRmInertiaOwner_E248 {
    char pad_0x0[0x280];
    sRmMat33 inertia;   // 0x280: body-space inertia
};

// r = I * a, with I the body inertia (passed through its owner so each element is an offset from it).
static inline sRmMat33 MulI_E248(const sRmMat33& a, const sRmInertiaOwner_E248* body)
{
    sRmMat33 r;
    r.m[0][0] = a.m[0][0] * body->inertia.m[0][0] + a.m[1][0] * body->inertia.m[0][1] + a.m[2][0] * body->inertia.m[0][2];
    r.m[1][0] = a.m[0][0] * body->inertia.m[1][0] + a.m[1][0] * body->inertia.m[1][1] + a.m[2][0] * body->inertia.m[1][2];
    r.m[2][0] = a.m[0][0] * body->inertia.m[2][0] + a.m[1][0] * body->inertia.m[2][1] + a.m[2][0] * body->inertia.m[2][2];
    r.m[0][1] = a.m[0][1] * body->inertia.m[0][0] + a.m[1][1] * body->inertia.m[0][1] + a.m[2][1] * body->inertia.m[0][2];
    r.m[1][1] = a.m[0][1] * body->inertia.m[1][0] + a.m[1][1] * body->inertia.m[1][1] + a.m[2][1] * body->inertia.m[1][2];
    r.m[2][1] = a.m[0][1] * body->inertia.m[2][0] + a.m[1][1] * body->inertia.m[2][1] + a.m[2][1] * body->inertia.m[2][2];
    r.m[0][2] = a.m[0][2] * body->inertia.m[0][0] + a.m[1][2] * body->inertia.m[0][1] + a.m[2][2] * body->inertia.m[0][2];
    r.m[1][2] = a.m[0][2] * body->inertia.m[1][0] + a.m[1][2] * body->inertia.m[1][1] + a.m[2][2] * body->inertia.m[1][2];
    r.m[2][2] = a.m[0][2] * body->inertia.m[2][0] + a.m[1][2] * body->inertia.m[2][1] + a.m[2][2] * body->inertia.m[2][2];
    return r;
}

/**
 * Rotates the roller's body-space inverse inertia into world space for its rigid body.
 *
 * Turns the body's orientation quaternion (+0x40) into a rotation R and stores R * InvBodyMOI * R^T (InvBodyMOI at
 * +0x280) as the rigid body's InvMOI (+0x80), ready for cRigidBody_updateImplicit.
 */
extern "C" void cRollerModifier_updateMOI(void* self)
{
    sM44_E248 rotMat4;
    QuatToMtx_E248(rotMat4, *(sRmVec4*)((char*)self + 0x40));
    sRmMat33 rot;
    sRmMat33 rotT;
    rotT = Get33_E248(rotMat4);
    rot = rotT;
    Transpose33_E248(rotT);
    *(sRmMat33*)((char*)self + 0x80) = Mul33_E248(MulI_E248(rot, (sRmInertiaOwner_E248*)self), rotT);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_getBodyPose);
#ifdef SKIP_ASM
struct sRmXform {
    sRmVec4 pos;    // 0x00
    sRmVec4 quat;   // 0x10
};

// PORT: PS2-only VU0 inline asm (dst = v * s).
static inline void vu0ScaleToRM(sRmVec4& dst, const sRmVec4& v, float s)
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
static inline void vu0QuatRotToRM(sRmVec4& dst, const sRmVec4& q, const sRmVec4& v)
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
static inline void vu0AddToRM(sRmVec4& dst, const sRmVec4& b)
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
extern "C" sRmXform cRollerModifier_getBodyPose(void* self)
{
    sRmXform pose = *(sRmXform*)((char*)self + 0x30);
    sRmVec4 negatedCom;
    sRmVec4 rotatedCom;
    sRmVec4 comOffset;
    sRmVec4 posDelta;
    sRmVec4 poseOffset;
    vu0QuatRotToRM(comOffset, *(sRmVec4*)((char*)self + 0x40), *(sRmVec4*)((char*)self + 0x2B0));
    rotatedCom = comOffset;
    vu0ScaleToRM(comOffset, rotatedCom, -1.0f);
    negatedCom = comOffset;
    vu0QuatRotToRM(poseOffset, pose.quat, negatedCom);
    posDelta = poseOffset;
    vu0AddToRM(pose.pos, posDelta);
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
void cRollerModifier_updateRootTransform(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_translateMe);
#ifdef SKIP_ASM
extern "C" void SphereTree_MoveOffset(void* p, const sRmVec4* d);

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void vu0AddED90(sRmVec4& dst, const sRmVec4& b)
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
extern "C" void cRollerModifier_translateMe(sRmBody* self, const sRmVec4* off)
{
    vu0AddED90(self->pos, *off);
    SphereTree_MoveOffset((char*)self + 0xE0, off);
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
    sRmVec4 pos;        // 0x00
    sRmVec4 normal;     // 0x10
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
static inline sRmVec4 vu0ScaleEDC8(const sRmVec4& v, float s)
{
    sRmVec4 r;
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
static inline sRmVec4 vu0DivEDC8(const sRmVec4& v, float s)
{
    sRmVec4 r;
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
static inline sRmVec4 vu0SubEDC8(const sRmVec4& a, const sRmVec4& b)
{
    sRmVec4 r;
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
static inline sRmVec4 vu0AddEDC8(const sRmVec4& a, const sRmVec4& b)
{
    sRmVec4 r;
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
static inline sRmVec4 vu0CrossEDC8(const sRmVec4& a, const sRmVec4& b)
{
    sRmVec4 r;
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
static inline float vu0DotEDC8(const sRmVec4& a, const sRmVec4& b)
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
static inline float vu0LengthEDC8(const sRmVec4& v)
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


static inline sRmVec4 toDirEDC8(sRmBody* b, const sRmVec4& v)
{
    return vu0DivEDC8(v, b->scale);
}

static inline sRmVec4 xformEDC8(cRollerModifier* rollerModifier)
{
    sRmVec4 o;
    float x = (*(sRmBody*)&rollerModifier->Pose).pos.x;
    float y = (*(sRmBody*)&rollerModifier->Pose).pos.y;
    float z = (*(sRmBody*)&rollerModifier->Pose).pos.z;
    o.x = (*(sRmBody*)&rollerModifier->Pose).rot.m[0][0] * x + (*(sRmBody*)&rollerModifier->Pose).rot.m[1][0] * y + (*(sRmBody*)&rollerModifier->Pose).rot.m[2][0] * z;
    o.y = (*(sRmBody*)&rollerModifier->Pose).rot.m[0][1] * x + (*(sRmBody*)&rollerModifier->Pose).rot.m[1][1] * y + (*(sRmBody*)&rollerModifier->Pose).rot.m[2][1] * z;
    o.z = (*(sRmBody*)&rollerModifier->Pose).rot.m[0][2] * x + (*(sRmBody*)&rollerModifier->Pose).rot.m[1][2] * y + (*(sRmBody*)&rollerModifier->Pose).rot.m[2][2] * z;
    o.w = (*(sRmBody*)&rollerModifier->Pose).pos.w;
    return o;
}

static inline void setPosEDC8(cRollerModifier* rollerModifier, const sRmVec4& v)
{
    (*(sRmBody*)&rollerModifier->Pose).pos = v;
    (*(sRmBody*)&rollerModifier->Pose).outPos = xformEDC8(rollerModifier);
}

static inline void setOutDirEDC8(cRollerModifier* rollerModifier, const sRmVec4& v)
{
    (*(sRmBody*)&rollerModifier->Pose).dir = toDirEDC8((sRmBody*)&rollerModifier->Pose, v);
    (*(sRmBody*)&rollerModifier->Pose).outDir = v;
}

/**
 * Resolves a roller's contact with the world for this frame.
 *
 * Queries the Nash grid with the roller's sphere tree. If it penetrates, it is pushed out along the normal
 * (depth + 2) and its linear and angular motion is damped to 97.5 %. If the contact point is moving into the
 * surface, a contact impulse is applied (inelastic below the stop speed), HitFrequency grows by 1/60 and a
 * Life above 5 is set to 10.
 */
extern "C" void cRollerModifier_collide(cRollerModifier* self)
{
    sHitEDC8 hit;
    char tree[0x70];
    cWorldSphTree_cWorldSphTree(tree, (char*)&self->SphTree, 1);
    float zeroF = 0.0f;
    float overlap = cBXNash_terrainIntersect1_EDC8(cObjectInterface_getNash(), tree, &hit, (char*)&self->RollerHint);
    overlap -= SphereTree_GetResolution((char*)&self->SphTree);
    if (zeroF < overlap) {
        sRmVec4 push = vu0ScaleEDC8(hit.normal, overlap + 2.0f);
        cRollerModifier_translateMe((sRmBody*)self, &push);
        float k = 0.975f;
        setPosEDC8((self), vu0ScaleEDC8(*(sRmVec4*)&self->L, k));
        setOutDirEDC8((self), vu0ScaleEDC8(*(sRmVec4*)&self->V, k));
        float dp = vu0DotEDC8(vu0AddEDC8(*(sRmVec4*)&self->V, vu0CrossEDC8(*(sRmVec4*)&self->W, vu0SubEDC8(hit.pos, *(sRmVec4*)(sRmBody*)&self->Pose))), hit.normal);
        if (dp < zeroF) {
            float lim = self->StopContactSpeed;
            float len = vu0LengthEDC8(*(sRmVec4*)&self->V);
            float elastic = self->ContactElastic;
            float friction = self->ContactFriction;
            if (len < lim)
                elastic = zeroF;
            cRigidBody_rigidImpact_EDC8((sRmBody*)&self->Pose, &hit, &hit.normal, elastic, friction);
            self->HitFrequency += 0.016666668f;
            if (5.0f < self->Life)
                self->Life = 10.0f;
        }
    }
    cWorldSphTree__cWorldSphTree(tree, 2);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_cTexFlipModifier);
#ifdef SKIP_ASM
extern char* cAppMan_TheAppMan;
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
 * @return self
 */
extern "C" cTexFlipModifier* cTexFlipModifier_cTexFlipModifier(cTexFlipModifier* self, void* def, void* owner)
{
    char* defData = (char*)def;
    float one = 1.0f;
    self->Instance = (tInstanceRecord*)owner;
    self->TexIndex = 0;
    self->NumMaterials = 0;
    self->NumTexFlipMaterials = 0;
    self->Type = *(int*)(defData + 0x4);
    float rate = *(float*)(defData + 0x10) * (one / (float)*(int*)(cAppMan_TheAppMan + 0x10));
    self->Speed = rate;
    self->Dir = *(int*)(defData + 0xC);
    self->BaseSpeed = rate;
    *(int*)&self->Time = 0;
    int n = *(int*)(defData + 0x8);
    if ((float)n < *(float*)(int*)&self->Time) {
        union { int i; float f; } randBits;
        randBits.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
        float x = (randBits.f - one) * 8.0f;
        float rounded = ftrunc_0035F0B8(x);
        if (rounded < x) {
            rounded += one;
        }
        self->TexIndex = (int)rounded;
    } else {
        self->TexIndex = n;
    }
    cTexFlipModifier_setupTexFlip(self);
    return (self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_cTexFlipModifier1);
#ifdef SKIP_ASM
class cTexFlipModifier_cTexFlipModifier1_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void read(void* buf, int size);
    virtual void* readReplayInstance();
};

extern "C" void cTexFlipModifier_setupTexFlip(cTexFlipModifier* self);

/**
 * Constructs a texture-flip modifier from a replay frame: reads the 0x18-byte state and Instance, then
 * cTexFlipModifier_setupTexFlip.
 *
 * @param obj  replay stream to read from
 * @return self
 */
extern "C" void* cTexFlipModifier_cTexFlipModifier1(cTexFlipModifier* self, cTexFlipModifier_cTexFlipModifier1_cObj* obj)
{
    obj->read(self, 0x18);
    self->Instance = (tInstanceRecord*)obj->readReplayInstance();
    cTexFlipModifier_setupTexFlip(self);
    return ((void*)self);
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
    int f20;
    int f24;
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
 * @param selfv  the texture-flip modifier
 */
extern "C" void cTexFlipModifier_setupTexFlip(cTexFlipModifier* selfv)
{
    sRailSelF218* self = (sRailSelF218*)selfv;
    int i;
    self->f20 = 0;
    self->count = **(int**)(self->owner + 0x94);
    for (i = 0; i < self->count; i++) {
        sRailItemF218* item = *(sRailItemF218**)(*(char**)(self->owner + 0x94) + (i << 2) + 4);
        int* table = (int*)item->data;
        if (table != 0) {
            self->f20 = *table;
            break;
        }
    }
    sel_F218(self, self->index);
    ms_F218(&self->f24, 0, 196);
    self->f24 = self->count;
    for (i = 0; i < self->count; i++) {
        sRailItemF218* item = *(sRailItemF218**)(*(char**)(self->owner + 0x94) + (i << 2) + 4);
        if (item->data != 0) {
            self->src[i] = &self->out[i];
            self->src[i]->fC = item->fC;
            // writes slots 1..3 of out[0] for every entry, as func_003443A8 does (bug in the original)
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
extern "C" void cTexFlipModifier_setupMaterialRecords(sRailModSelect* self);

/**
 * Selects which texture a texture-flip modifier shows.
 *
 * Clamps @p i to the last texture (count at +0x20 minus 1; no lower bound), stores it in TexIndex (+0x14) and
 * updates every flipping material's record with cTexFlipModifier_setupMaterialRecords.
 *
 * @param i  the texture index (On Tour texindex)
 */
extern "C" void cTexFlipModifier_setTexIndex(sRailModSelect* self, int i)
{
    int n = *(int*)((char*)self + 0x20);
    if (i >= n) {
        i = n - 1;
    }
    *(int*)((char*)self + 0x14) = i;
    cTexFlipModifier_setupMaterialRecords(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_setupMaterialRecords);
#ifdef SKIP_ASM
struct sRailOut14 {
    unsigned short value;   // 0x0
    char pad_0x2[0x12];
};

struct sRailModSelect {
    char pad_0x0[0x14];
    int index;              // 0x14
    char* owner;            // 0x18
    int count;              // 0x1c
    char pad_0x20[0x28];
    sRailOut14 out[1];      // 0x48
};

/**
 * Points every flipping material of a texture-flip modifier at the texture for TexIndex.
 *
 * For each of the materials (count +0x20) it reads the instance model's material list (+0x94) and, where an entry has a
 * texture table (+0x10), copies the u16 for TexIndex (+0x14) into that material's record in MaterialBlock.
 */
extern "C" void cTexFlipModifier_setupMaterialRecords(sRailModSelect* self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        char* list = *(char**)(self->owner + 0x94);
        char* item = *(char**)(list + (i << 2) + 4);
        char* table = *(char**)(item + 0x10);
        if (table != 0) {
            self->out[i].value = *(unsigned short*)(table + (self->index << 2) + 4);
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
 * decides: 0 steps TexIndex forward or, with Dir set, backward, wrapping at NumTexFlipMaterials; 1 sets Speed to 10
 * / the frame rate (cAppMan_TheAppMan + 0x10), switches to type 2 and steps forward; 2 sets Speed to BaseSpeed times a
 * random 0.25..1.0 (BXrand mantissa trick), switches to type 1 and steps forward. Ends with cTexFlipModifier_setTexIndex (On Tour
 * setTexIndex). The comment above the definition calls the timer a rail timer; it is the tex-flip one.
 */
extern "C" void cTexFlipModifier_update(cTexFlipModifier* self)
{
    if (!(0.0f < self->Speed)) {
        return;
    }
    float one = 1.0f;
    float* acc = (float*)(int*)&self->Time;
    *acc += self->Speed;
    if (!(one <= *acc)) {
        return;
    }
    *acc -= one;
    if (self->Type == 0) {
        if (self->Dir == 0) {
            if (++self->TexIndex >= self->NumTexFlipMaterials) {
                self->TexIndex = 0;
            }
        } else if (--self->TexIndex < 0) {
            self->TexIndex = self->NumTexFlipMaterials - 1;
        }
    } else if (self->Type == 1) {
        self->Speed = 10.0f / (float)*(int*)(cAppMan_TheAppMan + 0x10);
        self->Type = 2;
        if (++self->TexIndex >= self->NumTexFlipMaterials) {
            self->TexIndex = 0;
        }
    } else if (self->Type == 2) {
        union { int i; float f; } bits;
        float base = 0.25f;
        bits.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
        self->Type = 1;
        self->Speed = self->BaseSpeed * ((bits.f - one) * 0.75f + base);
        if (++self->TexIndex >= self->NumTexFlipMaterials) {
            self->TexIndex = 0;
        }
    }
    cTexFlipModifier_setTexIndex((sRailModSelect*)self, self->TexIndex);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_signal);
#ifdef SKIP_ASM
struct sRailModSelect;
extern "C" void cTexFlipModifier_setTexIndex(sRailModSelect* self, int i);

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
 * Signal 300 sets Speed to @p value; 301 with 0 <= @p value < 9 sets TexIndex to @p value rounded up and applies it
 * (cTexFlipModifier_setTexIndex, On Tour setTexIndex); 302 steps TexIndex forward, wrapping to 1 (not 0 as cTexFlipModifier_update
 * does), and applies it. Other ids are ignored.
 *
 * @param id  the signal id (On Tour signal)
 * @param value  the signal's argument value (On Tour arg)
 */
extern "C" void cTexFlipModifier_signal(cTexFlipModifier* self, int id, float value)
{
    switch (id) {
    case 300:
        self->Speed = value;
        break;
    case 301:
        if (value >= 0.0f && value < 9.0f) {
            float ceilVal = fTrunc_F598(value);
            if (ceilVal < value) {
                ceilVal += 1.0f;
            }
            int n = (int)ceilVal;
            self->TexIndex = n;
            cTexFlipModifier_setTexIndex((sRailModSelect*)self, n);
        }
        break;
    case 302:
        self->TexIndex++;
        if (self->TexIndex >= self->NumTexFlipMaterials) {
            self->TexIndex = 1;
        }
        cTexFlipModifier_setTexIndex((sRailModSelect*)self, self->TexIndex);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream0035F688 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void write(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual void v03(void* buf, int size);
    virtual void v04(void* buf, int size);
    virtual void writeReplayInstance(void* p);
};

/**
 * Writes the tex-flip modifier to a replay frame: the 0x18-byte cTexFlipModifierState (Type .. TexIndex) with slot
 * 1, then Instance with slot 5.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
extern "C" void cTexFlipModifier_writeToReplayFrame(cTexFlipModifier* self, cStream0035F688* stream)
{
    stream->write((char*)&self->Type, 0x18);
    stream->writeReplayInstance(self->Instance);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_cUVScrollModifier);
#ifdef SKIP_ASM
extern char* cAppMan_TheAppMan;

struct sRmQuad_F6E8 {
    float x, y, z, w;
    sRmQuad_F6E8(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
} __attribute__((aligned(16)));

struct sRmDesc_F6E8 {
    int f0;         // 0x0
    int f4;         // 0x4
    float f8;       // 0x8
    float fC;       // 0xC
    float f10;      // 0x10
    float f14;      // 0x14
    float f18;      // 0x18
    float f1C;      // 0x1C
    float f20;      // 0x20
    float f24;      // 0x24
    float f28;      // 0x28
    float f2C;      // 0x2C
};

struct sRmState_F6E8 {
    int f0;             // 0x0
    int f4;             // 0x4
    float f8;           // 0x8
    float fC;           // 0xC
    int f10;            // 0x10
    float f14;          // 0x14
    char pad_0x18[0x8];
    sRmQuad_F6E8 q;     // 0x20
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    int f40;            // 0x40
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
    self->f40 = 1;
    self->f0 = scrollData->f4;
    self->f30 = scrollData->f8;
    self->f34 = scrollData->fC;
    self->f8 = scrollData->f18;
    self->fC = scrollData->f1C;
    self->f4 = 0;
    self->f10 = 0;
    self->f14 = scrollData->f20 * (1.0f / (float)*(int*)(cAppMan_TheAppMan + 0x10));
    self->q = sRmQuad_F6E8(scrollData->f24, scrollData->f28, scrollData->f2C, 0.0f);
    self->f38 = scrollData->f10;
    self->f3C = scrollData->f14;
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
extern "C" void* cUVScrollModifier_cUVScrollModifier1(cUVScrollModifier* self, void* stream)
{
    sRailSerVEntry* vt = *(sRailSerVEntry**)stream;
    vt[2].fn((char*)stream + vt[2].delta, self, 80);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_update);
#ifdef SKIP_ASM
extern char* cAppMan_TheAppMan;

struct sV2_F7D0 {
    float x, y;
    sV2_F7D0() {}
    sV2_F7D0(float ax, float ay) : x(ax), y(ay) {}
    sV2_F7D0 operator-() const { return sV2_F7D0(-x, -y); }
    sV2_F7D0 operator*(float s) const { return sV2_F7D0(x * s, y * s); }
    sV2_F7D0& operator+=(const sV2_F7D0& o)
    {
        x += o.x;
        y += o.y;
        return *this;
    }
};

struct sRmState_F7D0 {
    int f0;             // 0x0
    float t;            // 0x4
    float dur;          // 0x8
    float fC;           // 0xC
    float f10;          // 0x10
    float f14;          // 0x14
    char pad_0x18[0x18];
    sV2_F7D0 pos;       // 0x30
    sV2_F7D0 vel;       // 0x38
    int active;         // 0x40
};

/**
 * Advances the UV-scroll modifier: rotates the texture, scrolls its UV offset and switches between the on and off
 * phases.
 *
 * Does nothing unless TimeOn or TimeOff is positive. Time grows by 1 / the frame rate (cAppMan_TheAppMan + 0x10). While
 * On: Angle grows by AngVel; at TimeOn the timer restarts, On clears when TimeOff > 0 and types 2 and 6 reverse
 * UVScroll. While still on, type 6 eases the UV offset with a triangle profile over the on phase (UVScroll * t /
 * TimeOn, the sign of UVScroll[0] choosing the half), other types add UVScroll per frame; each UV component wraps
 * into -1..1. While off, at TimeOff the timer restarts and On is set again when TimeOn > 0. The 2*pi wrap tests and
 * adjusts AngVel instead of Angle, and its second test is AngVel <= 2*pi (not -2*pi), so a slow AngVel gains 2*pi
 * every other frame and Angle grows without bound (harmless for the rotation, likely an original bug).
 */
extern "C" void cUVScrollModifier_update(cUVScrollModifier* self)
{
    if (self->TimeOn > 0.0f || self->TimeOff > 0.0f) {
        self->Time += 1.0f / (float)*(int*)(cAppMan_TheAppMan + 0x10);
        if (self->On) {
            if (self->AngVel != 0.0f) {
                self->Angle += self->AngVel;
                if (self->AngVel >= 6.2831854820251465f) {
                    self->AngVel = self->AngVel - 6.2831854820251465f;
                } else if (self->AngVel <= 6.2831854820251465f) {
                    self->AngVel = self->AngVel + 6.2831854820251465f;
                }
            }
            if (self->Time >= self->TimeOn) {
                self->Time = 0.0f;
                if (self->Time < self->TimeOff) {
                    self->On = 0;
                }
                if (self->Type == 2 || self->Type == 6) {
                    *(sV2_F7D0*)&self->UVScroll[0] = -*(sV2_F7D0*)&self->UVScroll[0];
                }
            }
            if (self->On) {
                if (self->Type == 6) {
                    if (self->UVScroll[0] < 0.0f) {
                        float time = self->Time;
                        if (time < self->TimeOn * 0.5f) {
                            sV2_F7D0 step = (*(sV2_F7D0*)&self->UVScroll[0]) * time * (1.0f / self->TimeOn);
                            self->UV[0] += step.x;
                            self->UV[1] += step.y;
                        } else {
                            sV2_F7D0 step = (*(sV2_F7D0*)&self->UVScroll[0]) * (self->TimeOn - time) * (1.0f / self->TimeOn);
                            self->UV[0] += step.x;
                            self->UV[1] += step.y;
                        }
                    } else {
                        float remaining = self->TimeOn - self->Time;
                        if (remaining < self->TimeOn * 0.5f) {
                            sV2_F7D0 step = (*(sV2_F7D0*)&self->UVScroll[0]) * remaining * (1.0f / self->TimeOn);
                            self->UV[0] += step.x;
                            self->UV[1] += step.y;
                        } else {
                            sV2_F7D0 step = (*(sV2_F7D0*)&self->UVScroll[0]) * (self->TimeOn - remaining) * (1.0f / self->TimeOn);
                            self->UV[0] += step.x;
                            self->UV[1] += step.y;
                        }
                    }
                } else {
                    self->UV[0] += self->UVScroll[0];
                    self->UV[1] += self->UVScroll[1];
                }
                if (self->UV[0] > 1.0f) {
                    self->UV[0] = self->UV[0] - 1.0f;
                } else if (self->UV[0] < -1.0f) {
                    self->UV[0] = self->UV[0] + 1.0f;
                }
                if (self->UV[1] > 1.0f) {
                    self->UV[1] = self->UV[1] - 1.0f;
                } else if (self->UV[1] < -1.0f) {
                    self->UV[1] = self->UV[1] + 1.0f;
                }
            }
        } else if (self->Time >= self->TimeOff) {
            self->Time = 0.0f;
            if (self->TimeOn > 0.0f) {
                self->On = 1;
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
 * Signal 201 sets UVScroll[0] and 200 UVScroll[1] (range -1..1; note the order is the reverse of 204/205); 202 sets
 * TimeOn and 203 TimeOff (0..60 seconds); 204 sets UV[0] and 205 UV[1] (-4..4). Values out of range and other ids
 * are ignored.
 *
 * @param id  the signal id (On Tour signal)
 * @param value  the signal's argument value (On Tour arg)
 */
extern "C" void cUVScrollModifier_signal(cUVScrollModifier* self, int id, float value)
{
    if (id == 200 || id == 201) {
        if (value >= -1.0f && value <= 1.0f) {
            if (id == 201) {
                self->UVScroll[0] = value;
            } else {
                self->UVScroll[1] = value;
            }
        }
    } else if (id == 202 || id == 203) {
        if (value >= 0.0f && value <= 60.0f) {
            if (id == 202) {
                self->TimeOn = value;
            } else {
                self->TimeOff = value;
            }
        }
    } else if (id == 204 || id == 205) {
        if (value >= -4.0f && value <= 4.0f) {
            if (id == 204) {
                self->UV[0] = value;
            } else {
                self->UV[1] = value;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_getTextureMat);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);

struct sRmAxisRot_35FC20 {
    char pad_0x0[0x10];
    float angle;        // 0x10
    char pad_0x14[0xC];
    float ax, ay, az;   // 0x20
    char pad_0x2c[0x4];
    float tx, ty;       // 0x30
};

struct sRmMtx_35FC20 {
    float m[4][4];
} __attribute__((aligned(16)));

extern sRmMtx_35FC20 D_004FF1A0_35FC20[] __asm__("t4mat_Identity");

/**
 * Builds the texture matrix of a UV-scroll modifier: a rotation by Angle about Axis (identity when Angle is 0) whose
 * row 3 holds the UV offset (UV[0], UV[1]).
 *
 * @param mat  receives the matrix (On Tour t4Matrix& mat)
 * @return @p mat
 */
extern "C" sRmMtx_35FC20* cUVScrollModifier_getTextureMat(cUVScrollModifier* self, sRmMtx_35FC20* mat)
{
    float zeroF = 0.0f;
    if (self->Angle != zeroF) {
        float sinA;
        float cosA;
        fSinCos(&sinA, &cosA, self->Angle);
        float one = 1.0f;
        float oneMinusCos = one - cosA;
        float tx = oneMinusCos * self->Axis.x;
        float sy = sinA * self->Axis.y;
        float sz = sinA * self->Axis.z;
        float sx = sinA * self->Axis.x;
        float ty = oneMinusCos * self->Axis.y;
        float tz = oneMinusCos * self->Axis.z;
        mat->m[0][0] = tx * self->Axis.x + cosA;
        mat->m[1][0] = tx * self->Axis.y + sz;
        mat->m[2][0] = tx * self->Axis.z - sy;
        mat->m[3][0] = zeroF;
        mat->m[0][1] = ty * self->Axis.x - sz;
        mat->m[1][1] = ty * self->Axis.y + cosA;
        mat->m[2][1] = ty * self->Axis.z + sx;
        mat->m[3][1] = zeroF;
        mat->m[0][2] = tz * self->Axis.x + sy;
        mat->m[1][2] = tz * self->Axis.y - sx;
        mat->m[2][2] = tz * self->Axis.z + cosA;
        mat->m[3][2] = zeroF;
        mat->m[0][3] = zeroF;
        mat->m[1][3] = zeroF;
        mat->m[2][3] = zeroF;
        mat->m[3][3] = one;
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
            : "r"(mat), "r"(D_004FF1A0_35FC20)
            : "memory");
    }
    mat->m[3][0] = self->UV[0];
    mat->m[3][1] = self->UV[1];
    return mat;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cUVScrollModifier_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void write(void* buf, int size);
};

/**
 * Writes the whole UV-scroll modifier (0x50 bytes, all cUVScrollModifierState) through the replay frame's write
 * virtual (slot 1).
 *
 * @param obj  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
extern "C" void cUVScrollModifier_writeToReplayFrame(cUVScrollModifier* self, cUVScrollModifier_writeToReplayFrame_cObj* obj)
{
    obj->write(self, 80);
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
INCLUDE_ASM("object/railmodifier", func_0035FE10);
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

// D_004FFAC0: a Catmull-Rom basis matrix
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
extern sMat_FE10 D_004FFAC0_m __asm__("D_004FFAC0");
extern sV4_FE10 D_004FFB00_v __asm__("D_004FFB00");
extern sV4_FE10 D_004FFB10_v __asm__("D_004FFB10");
extern char gBucketMan;

// Static initialisation of the unit's colour globals D_004FF740..D_004FFAB0, the spline basis D_004FFAC0,
// D_004FFB00/D_004FFB10 and the list gBucketMan (g++ __static_initialization_and_destruction_0).
// The unit declares func_0035FE10 returning void*; bind this definition by asm label.
extern "C" void func_0035FE10_init(int initialize, int priority) __asm__("func_0035FE10");
/**
 * Runs the railmodifier unit's static initialisation: builds its global vectors and matrices and the bucket manager.
 *
 * Only for priority 0xFFFF with initialize set. Constructs the colour/parameter vectors D_004FF740..D_004FFAB0, the
 * Catmull-Rom basis D_004FFAC0 used for rail segment matrices, D_004FFB00/D_004FFB10, and gBucketMan (cBucketMan_cBucketMan).
 *
 * @param initialize  nonzero for construction
 * @param priority  init priority; only 0xFFFF does anything
 */
extern "C" void func_0035FE10_init(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
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
        new (&D_004FFAC0_m) sMat_FE10(-0.5f, 1.5f, -1.5f, 0.5f,
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
extern "C" void* cObjNode_cObjNode2(cObjNode* self)
{
    self->Next = 0;
    self->mVtbl = (void**)((int)(void*)cObjNode_vtbl);
    self->Prev = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cObjNode's update has an empty body.
 */
void cObjNode_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_apply__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cObjNode's apply has an empty body.
 */
void cObjNode_apply(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cObjNode's render has an empty body.
 */
void cObjNode_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_renderModel__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cObjNode's renderModel has an empty body.
 */
void cObjNode_renderModel(void* self)
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
int cObjNode_compare(void* self)
{
    return 0;
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
int cObjNode_equal(void* self)
{
    return 0;
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
int cObjNode_getSortIndex(void* self)
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
int cObjNode_hasParent(void* self)
{
    return 0;
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
int cObjNode_isType(void* self)
{
    return 0;
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
void cObjNode_writeToReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjNode_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Reads nothing from the replay frame: cObjNode's readFromReplayFrame has an empty body.
 */
void cObjNode_readFromReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTypeObjNode's update has an empty body.
 */
void cTypeObjNode_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cTypeObjNode's render has an empty body.
 */
void cTypeObjNode_render(void* self)
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
int cTypeObjNode_shouldRebound(void* self)
{
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
 * @return 1 when the types match, otherwise 0
 */
extern "C" int cTypeObjNode_isType(cTypeObjNode* self, int type)
{
    return self->ObjNodeType == type;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_getType);
#ifdef SKIP_ASM
/**
 * Returns a typed object node's ObjNodeType (+0x10, tObjNodeType).
 *
 * @param node  the typed object node
 * @return ObjNodeType
 */
extern "C" short cTypeObjNode_getType(cTypeObjNode* node) {
    return (node->ObjNodeType);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_setRootTransformDirty);
#ifdef SKIP_ASM
/**
 * Sets the RootTransformDirty bit (bit 0 of Flags, +0x12) of a typed object node.
 *
 * @param node  the typed object node
 */
extern "C" void cTypeObjNode_setRootTransformDirty(cTypeObjNode* node) {
    (node->Flags) = (unsigned short) ((node->Flags) | 1);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_isRootTransformDirty);
#ifdef SKIP_ASM
/**
 * Tests the RootTransformDirty bit (bit 0 of Flags, +0x12) of a typed object node.
 *
 * @param node  the typed object node
 * @return that bit (0 or 1)
 */
extern "C" int cTypeObjNode_isRootTransformDirty(cTypeObjNode* node) {
    return (node->Flags) & 1;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_resetRootTransformDirty);
#ifdef SKIP_ASM
/**
 * Clears the RootTransformDirty bit (bit 0 of Flags, +0x12) of a typed object node.
 *
 * @param node  the typed object node
 */
extern "C" void cTypeObjNode_resetRootTransformDirty(cTypeObjNode* node) {
    (node->Flags) = (unsigned short) ((node->Flags) & 0xFFFE);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cTypeObjNode_getObjNodeCount);
#ifdef SKIP_ASM
extern int D_0044AFF0[];

/**
 * Returns how many object nodes of a type are alive (D_0044AFF0[idx]).
 *
 * D_0044AFF0 is the per-type count the cTypeObjNode constructors increment and its destructor decrements
 * (src/object/objnode.cpp); On Tour has 23 types (kNumObjNodeType).
 *
 * @param idx  node type (tObjNodeType)
 * @return the live count for that type
 */
extern "C" int cTypeObjNode_getObjNodeCount(int idx)
{
    return D_0044AFF0[idx];
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_compare);
#ifdef SKIP_ASM
struct sRailVEntryU800 {
    short delta;
    short index;
    unsigned int (*fn)(void*);
};

/**
 * Orders two sort nodes: tells whether @p obj sorts at or after this node.
 *
 * Compares obj's virtual getSortIndex() (vtable slot 8) with this node's SortIndex (+0x14), unsigned.
 *
 * @param obj  the other node (On Tour cObjNode*)
 * @return 1 when obj's sort index is greater than or equal to this node's, otherwise 0
 */
extern "C" int cSortObjNode_compare(cSortObjNode* self, void* obj)
{
    sRailVEntryU800* vt = *(sRailVEntryU800**)((char*)obj + 0xC);
    return vt[8].fn((char*)obj + vt[8].delta) >= self->SortIndex;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSortObjNode_equal);
#ifdef SKIP_ASM
struct sRailVEntryU840 {
    short delta;
    short index;
    unsigned int (*fn)(void*);
};

/**
 * Tells whether another node has the same sort index as this one.
 *
 * Compares this node's SortIndex (+0x14) with obj's virtual getSortIndex() (vtable slot 8).
 *
 * @param obj  the other node (On Tour cObjNode*)
 * @return 1 when the two sort indices are equal, otherwise 0
 */
extern "C" int cSortObjNode_equal(cSortObjNode* self, void* obj)
{
    sRailVEntryU840* vt = *(sRailVEntryU840**)((char*)obj + 0xC);
    return self->SortIndex == vt[8].fn((char*)obj + vt[8].delta);
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
 *
 * @return updateNode()'s result (On Tour's update returns nothing)
 */
extern "C" int cInstanceNode_update(cInstanceNode* self)
{
    return ((cRailVirt*)self)->updateNode();
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
int cInstanceNode_updateNode(void* self)
{
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
int cInstanceNode_isMoveNode(void* self)
{
    return 0;
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
unsigned char cInstanceNode_getSectionId(void* vself)
{
    cInstanceNode* self = (cInstanceNode*)vself;
    return *(unsigned char*)((char*)self->Instance + 0x78);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_isComplete__FPv);
#ifdef SKIP_ASM
/**
 * Reports an instance node as complete.
 *
 * @return always 1
 */
int cInstanceNode_isComplete(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_throwMe__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cInstanceNode's throwMe has an empty body.
 */
void cInstanceNode_throwMe(void* self)
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
int cInstanceNode_getNodeState(void* self)
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
int cInstanceNode_hasFocusCount(void* self)
{
    return 0;
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
int cInstanceNode_incFocusCount(void* self)
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
int cInstanceNode_decFocusCount(void* self)
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
int cInstanceNode_getFocusCount(void* self)
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
 * @param instanceNode  the instance node
 * @return Instance
 */
extern "C" int cInstanceNode_getInstance(cInstanceNode* instanceNode) {
    return ((int)instanceNode->Instance);
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
void* cInstanceNode_getRootTransform(void* vself)
{
    cInstanceNode* self = (cInstanceNode*)vself;
    return (char*)self->Instance + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_getRootTransform1__FPv);
#ifdef SKIP_ASM
/**
 * Returns an instance node's root transform for an instance: always the matrix at its own Instance +0x10.
 *
 * The instance argument of the slot is not declared and not used.
 *
 * @return pointer to the node's instance matrix
 */
void* cInstanceNode_getRootTransform1(void* vself)
{
    cInstanceNode* self = (cInstanceNode*)vself;
    return (char*)self->Instance + 0x10;
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
int cInstanceNode_hasAnimMats(void* self)
{
    return 0;
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
int cInstanceNode_getAnimMats(void* self)
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
int cInstanceNode_getInstAnimMats(void* self)
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
int cInstanceNode_getAnimMat(void* self)
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
float cInstanceNode_getAnimFrame(void* self)
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
int cInstanceNode_getAnimDir(void* self)
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
int cInstanceNode_frameUpdateEvent(void* self)
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
void cInstanceNode_signal(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_makeActive__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cInstanceNode's makeActive has an empty body.
 */
void cInstanceNode_makeActive(void* self)
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
int cInstanceNode_hasIntersect(void* self)
{
    return 0;
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
int cInstanceNode_intersect(void* self)
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
void cInstanceNode_onCollide(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_setDebounce__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cInstanceNode's setDebounce has an empty body.
 */
void cInstanceNode_setDebounce(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_updateIntersectInfo__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cInstanceNode's updateIntersectInfo has an empty body.
 */
void cInstanceNode_updateIntersectInfo(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cInstanceNode_applyForce__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the force: cInstanceNode's applyForce has an empty body.
 */
void cInstanceNode_applyForce(void* self)
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
int cInstanceNode_hasBoundBox(void* self)
{
    return 0;
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
int cInstanceNode_getBoundBox(void* self)
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
int cInstanceNode_isFlexRail(void* self)
{
    return 0;
}
#endif

extern void* cDeadNode_vtbl[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", cDeadNode__cDeadNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a dead node: stores cDeadNode's vtable at +0xC, then runs the cInstanceNode destructor.
 *
 * The g++ delete flags arrive in the second argument register, which this body leaves untouched, so they pass
 * through to cInstanceNode__cInstanceNode(), which frees the object when bit 0 is set.
 *
 * @return no meaningful value (cInstanceNode__cInstanceNode returns nothing)
 */
void* cDeadNode__cDeadNode(void* vself)
{
    cDeadNode* self = (cDeadNode*)vself;
    self->mVtbl = (void**)((int)(void*)cDeadNode_vtbl);
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDeadNode_expire__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cDeadNode's expire has an empty body.
 */
void cDeadNode_expire(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDeadNode_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cDeadNode's loseFocus has an empty body.
 */
void cDeadNode_loseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDeadNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cDeadNode's signal has an empty body.
 */
void cDeadNode_signal(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDeadFadeNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores signals: cDeadFadeNode's signal has an empty body.
 */
void cDeadFadeNode_signal(void* self)
{
}
#endif

extern void* cHideNode_vtbl[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", cHideNode__cHideNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a hide node: reinstalls cHideNode_vtbl and runs cInstanceNode__cInstanceNode (which forwards the delete flags).
 *
 * @return cInstanceNode__cInstanceNode's result
 */
void* cHideNode__cHideNode(void* vself)
{
    cHideNode* self = (cHideNode*)vself;
    self->mVtbl = (void**)((int)(void*)cHideNode_vtbl);
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHideNode_expire__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a hide node expires.
 */
void cHideNode_expire(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHideNode_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a hide node loses focus.
 */
void cHideNode_loseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHideNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores signals sent to a hide node.
 */
void cHideNode_signal(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHideNode_onCollide__FPv);
#ifdef SKIP_ASM
/**
 * Ignores collisions with a hide node.
 */
void cHideNode_onCollide(void* self)
{
}
#endif

extern void* cRestoreNode_vtbl[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", cRestoreNode__cRestoreNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a restore node: reinstalls cRestoreNode_vtbl and runs cInstanceNode__cInstanceNode (which forwards the delete
 * flags).
 *
 * @return cInstanceNode__cInstanceNode's result
 */
void* cRestoreNode__cRestoreNode(void* vself)
{
    cRestoreNode* self = (cRestoreNode*)vself;
    self->mVtbl = (void**)((int)(void*)cRestoreNode_vtbl);
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRestoreNode_expire__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a restore node expires.
 */
void cRestoreNode_expire(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRestoreNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores signals sent to a restore node.
 */
void cRestoreNode_signal(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostNode_boostStart__FPv);
#ifdef SKIP_ASM
/**
 * Hook called when a boost starts: does nothing.
 */
void cBoostNode_boostStart(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostNode_boostEnd__FPv);
#ifdef SKIP_ASM
/**
 * Hook called when a boost ends: does nothing.
 */
void cBoostNode_boostEnd(void* self)
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
void cTransformModifier_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_updateRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a transform modifier's root-transform update hook.
 */
void cTransformModifier_updateRootTransform(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_postUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTransformModifier's postUpdate has an empty body.
 */
void cTransformModifier_postUpdate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cTransformModifier's signal has an empty body.
 */
void cTransformModifier_signal(void* self)
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
int cTransformModifier_isComplete(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_clearComplete__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTransformModifier's clearComplete has an empty body.
 */
void cTransformModifier_clearComplete(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_shouldRebound__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a transform modifier should rebound.
 *
 * @return always 1 (cRollerModifier returns 0)
 */
int cTransformModifier_shouldRebound(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_runCollide__FPv);
#ifdef SKIP_ASM
/**
 * Reports that collision should run for a transform modifier.
 *
 * @return always 1 (cRollerModifier returns 0)
 */
int cTransformModifier_runCollide(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_collide__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cTransformModifier's collide has an empty body.
 */
void cTransformModifier_collide(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cTransformModifier's render has an empty body.
 */
void cTransformModifier_render(void* self)
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
int cTransformModifier_getBoundBox(void* self)
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
void cTransformModifier_setBoundBox(void* self)
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
float cTransformModifier_getRadius(void* self)
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
void cTransformModifier_setRadius(void* self)
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
int cTransformModifier_getMesh(void* self)
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
int cTransformModifier_isRootTransformDirty(void* self)
{
    return 0;
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
int cTransformModifier_getRootTransform(void* self)
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
int cTransformModifier_getRootTransform1(void* self)
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
int cTransformModifier_hasIntersect(void* self)
{
    return 0;
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
int cTransformModifier_intersect(void* self)
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
void cTransformModifier_updateIntersectInfo(void* self)
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
int cTransformModifier_getAnimMats(void* self)
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
void cTransformModifier_updateOverlapSystem(void* self)
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
int cTransformModifier_getParent(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cTransformModifier: 0.
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return 0
 */
int cTransformModifier_getTransformType(void* self)
{
    return 0;
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
void cTransformModifier_writeToReplayFrame(void* self)
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
 * @param physicsModifier  the physics modifier
 * @return physicsModifier
 */
extern "C" void *cPhysicsModifier_cPhysicsModifier1(cPhysicsModifier* physicsModifier) {
    ((int*)physicsModifier->mVtbl) = (int*)&cPhysicsModifier_vtbl;
    (physicsModifier->Active) = 1;
    return ((void*)physicsModifier);
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
    self->mVtbl = cPhysicsModifier_vtbl;
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
void cPhysicsModifier_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPhysicsModifier_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cPhysicsModifier's signal has an empty body.
 */
void cPhysicsModifier_signal(void* self)
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
int cPhysicsModifier_isActive(void* vself)
{
    cPhysicsModifier* self = (cPhysicsModifier*)vself;
    return self->Active;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPhysicsModifier_getPhysicsType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the base physics modifier's physics type id.
 *
 * @return always 0 (cBoostModifier returns 1)
 */
int cPhysicsModifier_getPhysicsType(void* self)
{
    return 0;
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
void* cDynamicEmitter__cDynamicEmitter(void* self)
{
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
    self->mVtbl = cEffectLink_vtbl;
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
int cEffectLink_getType(void* self)
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
void cEffectLink_fadeEffect(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectLink_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes nothing to the replay frame: cEffectLink's writeToReplayFrame has an empty body.
 */
void cEffectLink_writeToReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectModifier_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cEffectModifier's update has an empty body.
 */
void cEffectModifier_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectModifier_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cEffectModifier's render has an empty body.
 */
void cEffectModifier_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectModifier_signal__FPv);
#ifdef SKIP_ASM
/**
 * Ignores the signal: cEffectModifier's signal has an empty body.
 */
void cEffectModifier_signal(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cEffectModifier_getEffectCount);
#ifdef SKIP_ASM
extern int D_004A3FF4;

/**
 * Returns the number of live effect modifiers, D_004A3FF4.
 *
 * src/object/effectlink.cpp's cEffectModifier constructors increment D_004A3FF4 and its destructor decrements it.
 *
 * @return D_004A3FF4
 */
extern "C" int cEffectModifier_getEffectCount(void) {
    return D_004A3FF4;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParticleModifier_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the effect type id of a particle modifier.
 *
 * @return always 1 (dynamic particle modifiers return 2, halo modifiers 3)
 */
int cParticleModifier_getType(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDynamicParticleModifier_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the effect type id of a dynamic particle modifier.
 *
 * @return always 2 (particle modifiers return 1, halo modifiers 3)
 */
int cDynamicParticleModifier_getType(void* self)
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDynamicParticleModifier_getDynamicParticleCount);
#ifdef SKIP_ASM
extern int D_004A3FFC;

/**
 * Returns the number of live dynamic particle modifiers, D_004A3FFC.
 *
 * src/object/effectlink.cpp's dynamic-particle constructors increment D_004A3FFC and the destructor decrements it.
 *
 * @return D_004A3FFC
 */
extern "C" int cDynamicParticleModifier_getDynamicParticleCount(void) {
    return D_004A3FFC;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cHaloModifier_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the effect type id of a halo modifier.
 *
 * @return always 3 (particle modifiers return 1, dynamic particle modifiers 2)
 */
int cHaloModifier_getType(void* self)
{
    return 0x3;
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
    self->mVtbl = cRailLink_vtbl;
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
void cRailLink_writeToReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_getSplineId);
#ifdef SKIP_ASM
/**
 * Returns a rail modifier's SplineId (+0x30).
 *
 * @param railModifier  the rail modifier
 * @return SplineId
 */
extern "C" int cRailModifier_getSplineId(void *railModifier) {
    return (*(int *)((char*)(railModifier) + (0x30)));
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_apply);
#ifdef SKIP_ASM
/**
 * Applies a move node by calling its virtual updateOverlapSystem() (vtable slot 50).
 */
extern "C" void cMoveNode_apply(cMoveNode* self)
{
    ((cRailVirt*)self)->updateOverlapSystem();
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_updateAnimState__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cMoveNode's updateAnimState has an empty body.
 */
void cMoveNode_updateAnimState(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_isMoveNode__FPv);
#ifdef SKIP_ASM
/**
 * Reports that this node is a move node.
 *
 * @return always 1 (cInstanceNode returns 0)
 */
int cMoveNode_isMoveNode(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_updateRootTransform);
#ifdef SKIP_ASM
/**
 * Marks a move node's root transform as up to date by clearing bit 0 of its flags (+0x12).
 */
extern "C" void cMoveNode_updateRootTransform(cMoveNode* self)
{
    unsigned short* nodeFlags = (unsigned short*)((char*)&self->Flags);
    *nodeFlags &= 0xFFFE;
}
#endif

extern void* cObjectNode_vtbl[];
extern "C" void* cMoveNode__cMoveNode(void*);

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
extern "C" int cMoveNode_getParentMesh(cMoveNode* self)
{
    cVirt16** holder = (cVirt16**)self->ModifierBlock;
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
extern "C" int cMoveNode_getTransformModifier(cMoveNode* self)
{
    int* modBlock = (int*)self->ModifierBlock;
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
 * @param moveNode  the move node
 * @return 1 if it has one, else 0
 */
extern "C" int cMoveNode_hasTextureMat(cMoveNode* moveNode) {
    int var_2;
    void *temp_3;

    temp_3 = ((void*)moveNode->ModifierBlock);
    var_2 = 0;
    if (temp_3 != 0) {
        var_2 = (*(int *)((char*)(temp_3) + (4))) != 0;
    }
    return var_2;
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
 * @param moveNode  the move node
 * @return 1 if it has one, else 0
 */
extern "C" int cMoveNode_hasMaterialBlock(cMoveNode* moveNode) {
    int var_2;
    void *temp_3;

    temp_3 = ((void*)moveNode->ModifierBlock);
    var_2 = 0;
    if (temp_3 != 0) {
        var_2 = (*(int *)((char*)(temp_3) + (8))) != 0;
    }
    return var_2;
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
extern "C" int cMoveNode_getMaterialBlock(cMoveNode* self)
{
    int cond;
    char* modBlock = (char*)self->ModifierBlock;
    cond = 0;
    if (modBlock != 0) {
        cond = *(int*)(modBlock + 8) != 0;
    }
    if (cond != 0) {
        int matRec = *(int*)(modBlock + 8);
        if (matRec != 0) {
            return matRec + 36;
        }
        return 0;
    }
    return *(int*)((char*)self->Instance + 0x94);
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
extern "C" void* cMoveNode_getTextureMat(cMoveNode* self)
{
    char* modBlock = (char*)self->ModifierBlock;
    int cond = 0;
    if (modBlock != 0) {
        cond = *(int*)(modBlock + 4) != 0;
    }
    if (cond != 0) {
        void* uvScroll = *(void**)(modBlock + 4);
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
extern "C" void tModifierBlock_remTransformModifier(int);

/**
 * Removes a move node's transform modifier (tModifierBlock_remTransformModifier), if it has a modifier block.
 *
 * @param moveNode  the move node
 */
extern "C" void cMoveNode_remTransformModifier(cMoveNode* moveNode) {
    int temp_4;

    temp_4 = ((int)moveNode->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remTransformModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remUVScrollModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remUVScrollModifier(int);

/**
 * Removes a move node's UV-scroll modifier (tModifierBlock_remUVScrollModifier), if it has a modifier block.
 *
 * @param moveNode  the move node
 */
extern "C" void cMoveNode_remUVScrollModifier(cMoveNode* moveNode) {
    int temp_4;

    temp_4 = ((int)moveNode->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remUVScrollModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remTexFlipModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remTexFlipModifier(int);

/**
 * Removes a move node's texture-flip modifier (tModifierBlock_remTexFlipModifier), if it has a modifier block.
 *
 * @param moveNode  the move node
 */
extern "C" void cMoveNode_remTexFlipModifier(cMoveNode* moveNode) {
    int temp_4;

    temp_4 = ((int)moveNode->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remTexFlipModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remPhysicsModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remPhysicsModifier(int);

/**
 * Removes a move node's physics modifier (tModifierBlock_remPhysicsModifier), if it has a modifier block.
 *
 * @param moveNode  the move node
 */
extern "C" void cMoveNode_remPhysicsModifier(cMoveNode* moveNode) {
    int temp_4;

    temp_4 = ((int)moveNode->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remPhysicsModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remHaloModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remHaloModifier(int);

/**
 * Removes a move node's halo effects (tModifierBlock_remHaloModifier), if it has a modifier block.
 *
 * @param moveNode  the move node
 */
extern "C" void cMoveNode_remHaloModifier(cMoveNode* moveNode) {
    int temp_4;

    temp_4 = ((int)moveNode->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remHaloModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_remEffectModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remEffectModifier(int);

/**
 * Removes a move node's effect modifiers (tModifierBlock_remEffectModifier), if it has a modifier block.
 *
 * @param moveNode  the move node
 */
extern "C" void cMoveNode_remEffectModifier(cMoveNode* moveNode) {
    int temp_4;

    temp_4 = ((int)moveNode->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remEffectModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_fadeEffectModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_fadeEffectModifier(int);

/**
 * Fades out a move node's effects (tModifierBlock_fadeEffectModifier), if it has a modifier block.
 *
 * @param moveNode  the move node
 */
extern "C" void cMoveNode_fadeEffectModifier(cMoveNode* moveNode) {
    int temp_4;

    temp_4 = ((int)moveNode->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_fadeEffectModifier(temp_4);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cObjectNode__cObjectNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys an object node: reinstalls cObjectNode_vtbl and runs cMoveNode__cMoveNode (which forwards the delete flags).
 *
 * @return cMoveNode__cMoveNode's result
 */
void* cObjectNode__cObjectNode(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)cObjectNode_vtbl;
    return cMoveNode__cMoveNode(self);
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
int cAnimNode_updateNode(void* self)
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
extern "C" float cAnimNode_getAnimFrame(cAnimNode* self)
{
    float input = self->CurrTime;
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
float cAnimNode_getAnimSpeed(void* self)
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
int cAnimNode_hasAnimMats(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_getAnimMats);
#ifdef SKIP_ASM
struct sRailVEntry1098 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Returns the animation matrix array, first refreshing the root transform when it is dirty.
 *
 * When bit 0 (RootTransformDirty) of the embedded node's Flags (+0x26) is set, calls that node's (+0x14)
 * virtual slot 51, updateRootTransform in the cMoveNode family's vtables.
 *
 * @return the matrix array pointer stored at +0x44
 */
extern "C" int cAnimNode_getAnimMats(cAnimNode* self)
{
    if (self->Flags & 1) {
        void* sub = (char*)&self->unk_014;
        sRailVEntry1098* vt = (sRailVEntry1098*)self->mVtbl;
        vt[51].fn((char*)sub + vt[51].delta);
    }
    return (int)self->AnimMats;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimNode_getAnimMat);
#ifdef SKIP_ASM
struct sVEntry003610E0 {
    short delta;
    short index;
    void (*fn)(void*);
};


/**
 * Returns one animation matrix, first refreshing the root transform when it is dirty.
 *
 * The refresh is the same as cAnimNode_getAnimMats(): virtual slot 51 of the embedded node at +0x14 when bit 0
 * of its Flags (+0x26) is set.
 *
 * @param i  index of the matrix in the array at +0x44 (0x40-byte matrices)
 * @return the address of matrix @p i
 */
extern "C" t4Matrix* cAnimNode_getAnimMat(cAnimNode* self, int i)
{
    char* obj = (char*)&self->unk_014;
    if (self->Flags & 1) {
        sVEntry003610E0* vt = (sVEntry003610E0*)self->mVtbl;
        vt[51].fn(obj + vt[51].delta);
    }
    return &(self->AnimMats)[i];
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
 * @param animNode  the animated node
 */
extern "C" void cAnimNode_clearFrameEvent(cAnimNode* animNode) {
    (animNode->FrameStart) = 1e10f;
    (animNode->FrameEnd) = 1e10f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimObjectNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns an animated object node's state block, which is the node itself.
 *
 * @return self
 */
void* cAnimObjectNode_getNodeState(void* self)
{
    return self;
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
extern "C" float cAnimObjectNode_getAnimSpeed(cAnimObjectNode* self)
{
    return self->AnimComplete == 0 ? self->AnimSpeed : 0.0f;
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
extern "C" int cAnimObjectNode_getAnimDir(cAnimObjectNode* self)
{
    int direction = -1;
    if (self->AnimSpeed >= 0.0f) {
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
int cAnimObjectNode_isComplete(void* vself)
{
    cAnimObjectNode* self = (cAnimObjectNode*)vself;
    return self->AnimComplete;
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
extern "C" void* cAnimFrame_cAnimFrame1(cAnimFrame* self)
{
    self->mVtbl = (void**)((int)(void*)cAnimFrame_vtbl);
    cAnimFrame_init(self);
    return ((void*)self);
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
struct sVec4_361208 {
    float x, y, z, w;
} __attribute__((aligned(16)));

/**
 * Returns an animation frame's angles: the values at +0x50..+0x58 negated, with w = 0.
 *
 * @param out  receives the angles
 */
extern "C" void cAnimFrame_getAngles(cAnimFrame* self, sVec4_361208* out)
{
    sVec4_361208 negated;
    negated.x = -self->Vals[3];
    negated.y = -self->Vals[4];
    negated.z = -self->Vals[5];
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
extern "C" void* cMeshAnimFrame_cMeshAnimFrame1(cMeshAnimFrame* self)
{
    self->mVtbl = (void**)((int)(void*)cAnimFrame_vtbl);
    cAnimFrame_init(self);
    self->mVtbl = (void**)((int)(void*)cMeshAnimFrame_vtbl);
    cMeshAnimFrame_init(self);
    return ((void*)self);
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
extern char t4mat_Identity[];

/**
 * Resets a mesh animation frame's FrameMatrix (+0x90) to t4mat_Identity.
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
extern "C" void cMeshAnimFrame_init(cMeshAnimFrame* self)
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
        : "r"((char*)&self->FrameMatrix), "r"(t4mat_Identity)
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
extern "C" void* cCamAnimFrame_cCamAnimFrame1(cCamAnimFrame* self)
{
    self->mVtbl = (void**)((int)(void*)cAnimFrame_vtbl);
    cAnimFrame_init(self);
    self->mVtbl = (void**)((int)(void*)cCamAnimFrame_vtbl);
    return ((void*)self);
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
 * @param frame  the camera animation frame
 * @param source  the animation record
 */
extern "C" void cCamAnimFrame_setAnimRecord(void *frame, int source) {
    (*(int *)((char*)(frame) + (0x40))) = source;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame_getPosition);
#ifdef SKIP_ASM
struct sVec4_361378 {
    float x, y, z, w;
} __attribute__((aligned(16)));

/**
 * Returns a camera animation frame's position: the values at +0x44..+0x4C with w = 1.
 *
 * @param out  receives the position
 */
extern "C" void cCamAnimFrame_getPosition(cCamAnimFrame* self, sVec4_361378* out)
{
    sVec4_361378 result;
    result.x = self->Vals[0];
    result.y = self->Vals[1];
    result.z = self->Vals[2];
    result.w = 1.0f;
    *out = result;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame_getAngles);
#ifdef SKIP_ASM
struct sVec3_3613B0 {
    float x, y, z;
};

/**
 * Returns a camera animation frame's angles: the three values at +0x50..+0x58.
 *
 * @param out  receives the angles
 */
extern "C" void cCamAnimFrame_getAngles(cCamAnimFrame* self, sVec3_3613B0* out)
{
    sVec3_3613B0 result;
    result.x = self->Vals[3];
    result.y = self->Vals[4];
    result.z = self->Vals[5];
    *out = result;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cCamAnimFrame_getFocalLength);
#ifdef SKIP_ASM
/**
 * Returns a camera animation frame's focal length (+0x70) through out.
 *
 * @param frame  the camera animation frame
 * @param out  receives the focal length
 */
extern "C" void cCamAnimFrame_getFocalLength(cCamAnimFrame* frame, float *out) {
    *out = (frame->Vals[11]);
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
 * @return self
 */
void* cAnimTeeterNode_getNodeState(void* self)
{
    return self;
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
extern "C" int cAnimTeeterNode_getAnimDir(cAnimTeeterNode* self)
{
    int direction = -1;
    if (self->AnimSpeed >= 0.0f) {
        direction = 1;
    }
    return direction;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostModifier__cBoostModifier);
#ifdef SKIP_ASM
extern void* cPhysicsModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys a boost modifier: stores the base cPhysicsModifier vtable at +0x4, then frees the object when asked
 * to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cBoostModifier__cBoostModifier(cBoostModifier* self, int flags)
{
    self->mVtbl = cPhysicsModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostModifier_getPhysicsType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the boost modifier's physics type id.
 *
 * @return always 1 (the base cPhysicsModifier returns 0)
 */
int cBoostModifier_getPhysicsType(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostModifier_boostStart__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a boost starts; the body is empty.
 */
void cBoostModifier_boostStart(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cBoostModifier_boostEnd__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a boost ends; the body is empty.
 */
void cBoostModifier_boostEnd(void* self)
{
}
#endif

extern void* cDebounceNode_vtbl[];
extern "C" void* cMoveNode__cMoveNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", cDebounceNode__cDebounceNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a debounce node: stores cDebounceNode's vtable at +0xC, then runs the cMoveNode destructor.
 *
 * The g++ delete flags in the second argument register pass through untouched to cMoveNode__cMoveNode().
 *
 * @return no meaningful value (cMoveNode__cMoveNode returns nothing)
 */
void* cDebounceNode__cDebounceNode(void* vself)
{
    cDebounceNode* self = (cDebounceNode*)vself;
    self->mVtbl = (void**)((int)(void*)cDebounceNode_vtbl);
    return cMoveNode__cMoveNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cDebounceNode_onCollide__FPv);
#ifdef SKIP_ASM
/**
 * Ignores collisions: cDebounceNode's onCollide has an empty body.
 */
void cDebounceNode_onCollide(void* self)
{
}
#endif

extern void* cConveyorNode_vtbl[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", cConveyorNode__cConveyorNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a conveyor node: reinstalls cConveyorNode_vtbl and runs cInstanceNode__cInstanceNode (which forwards the delete
 * flags).
 *
 * @return cInstanceNode__cInstanceNode's result
 */
void* cConveyorNode__cConveyorNode(void* vself)
{
    cConveyorNode* self = (cConveyorNode*)vself;
    self->mVtbl = (void**)((int)(void*)cConveyorNode_vtbl);
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cConveyorNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns a conveyor node's state block (self + 0x1C).
 *
 * @return pointer to the state
 */
void* cConveyorNode_getNodeState(void* vself)
{
    cConveyorNode* self = (cConveyorNode*)vself;
    return (char*)&self->Vel;
}
#endif

extern void* cCounterNode_vtbl[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", cCounterNode__cCounterNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a counter node: stores cCounterNode's vtable at +0xC, then runs the cInstanceNode destructor.
 *
 * The g++ delete flags in the second argument register pass through untouched to
 * cInstanceNode__cInstanceNode().
 *
 * @return no meaningful value (cInstanceNode__cInstanceNode returns nothing)
 */
void* cCounterNode__cCounterNode(void* vself)
{
    cCounterNode* self = (cCounterNode*)vself;
    self->mVtbl = (void**)((int)(void*)cCounterNode_vtbl);
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexBridgeNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the flex bridge node itself as its node state.
 *
 * @return self
 */
void* cFlexBridgeNode_getNodeState(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexBridgeNode_hasIntersect__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a flex bridge node has intersection geometry.
 *
 * @return always 1
 */
int cFlexBridgeNode_hasIntersect(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexBridgeNode_hasBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a flex bridge node has a bounding box.
 *
 * @return always 1
 */
int cFlexBridgeNode_hasBoundBox(void* self)
{
    return 0x1;
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
void* cFlexBridgeNode_getBoundBox(void* vself)
{
    cFlexBridgeNode* self = (cFlexBridgeNode*)vself;
    return (char*)&self->BoundBox;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_updateNodes__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a flex rail's per-frame node update.
 */
void cFlexRailNode_updateNodes(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_updateMats__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a flex rail's segment-matrix update.
 */
void cFlexRailNode_updateMats(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_applyForce__FPv);
#ifdef SKIP_ASM
/**
 * Ignores forces applied to a flex rail.
 */
void cFlexRailNode_applyForce(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_updateIntersectInfo__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a flex rail's intersect-info update.
 */
void cFlexRailNode_updateIntersectInfo(void* self)
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
int cFlexRailNode_getMesh(void* self)
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
 * @return 1
 */
int cFlexRailNode_isFlexRail(void* self)
{
    return 0x1;
}
#endif

// 0x50-byte elements reached through a pointer at self+0x70; indexing the
// real element type is what reproduces the target's addu operand order
struct sRailEntry50 {
    char pad_0x00[0x44];
    int field_0x44;
    char pad_0x48[8];
};

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
extern "C" int cFloatRailNode_getMesh(cFloatRailNode* self, int segIndex)
{
    sRailEntry50* railNodes = (sRailEntry50*)self->Nodes;
    return railNodes[segIndex].field_0x44;
}
#endif

struct sRailEntry60 {
    char pad_0x00[0x54];
    int field_0x54;
    char pad_0x58[8];
};

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
extern "C" int cSpringRailNode_getMesh(cSpringRailNode* self, int segIndex)
{
    sRailEntry60* railNodes = (sRailEntry60*)self->Nodes;
    return railNodes[segIndex].field_0x54;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFlexRailNode_updateMat);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern float D_004FFAC0[16];



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
 * For each axis (x, y, z) multiplies the four control points' components by the Catmull-Rom basis D_004FFAC0, giving
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
extern "C" void cFlexRailNode_updateMat(cFlexRailNode* self, t4Matrix* out, const float* ctrlPoint0, const float* ctrlPoint1, const float* ctrlPoint2, const float* ctrlPoint3)
{
    t4Matrix coeffMat;
    t4Vector zCoeffs;
    coeffMat.row[1].x = ctrlPoint0[0]; coeffMat.row[1].y = ctrlPoint1[0]; coeffMat.row[1].z = ctrlPoint2[0]; coeffMat.row[1].w = ctrlPoint3[0];
    vu0BasisMul_15D8(coeffMat.row[2], coeffMat.row[1], D_004FFAC0);
    coeffMat.row[0] = coeffMat.row[2];
    coeffMat.row[2].x = ctrlPoint0[1]; coeffMat.row[2].y = ctrlPoint1[1]; coeffMat.row[2].z = ctrlPoint2[1]; coeffMat.row[2].w = ctrlPoint3[1];
    vu0BasisMul_15D8(coeffMat.row[3], coeffMat.row[2], D_004FFAC0);
    coeffMat.row[1] = coeffMat.row[3];
    coeffMat.row[3].x = ctrlPoint0[2]; coeffMat.row[3].y = ctrlPoint1[2]; coeffMat.row[3].z = ctrlPoint2[2]; coeffMat.row[3].w = ctrlPoint3[2];
    vu0BasisMul_15D8(zCoeffs, coeffMat.row[3], D_004FFAC0);
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
 * @param flagSet  the flag set
 * @return 1 if empty, else 0
 */
extern "C" int cFlagSet_Empty(cFlagSet* flagSet) {
    return (flagSet->mCurrentNumInstances) == 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cFlagManager_GetWindFactor);
#ifdef SKIP_ASM
/**
 * Returns a flag manager's current wind factor (+0x10).
 *
 * @param flagManager  the flag manager
 * @return the wind factor
 */
extern "C" float cFlagManager_GetWindFactor(cFlagManager* flagManager) {
    return (flagManager->mCurrentWindFactor);
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
void* cFloatingNode_getNodeState(void* self)
{
    return &((cFloatingNode*)self)->ZSpring;
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
int cFloatingNode_hasBoundBox(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_getBoundBox);
#ifdef SKIP_ASM
extern "C" int tModifierBlock_getBoundBox(void* p);

/**
 * Returns a floating node's bounding box: its modifier block's when it has one (+0x1C), otherwise its info
 * block (+0x78).
 *
 * @return the bounding box (On Tour const tBoundBox*), or 0 if neither exists
 */
extern "C" int cFloatingNode_getBoundBox(cFloatingNode* self)
{
    void* modBlock = self->ModifierBlock;
    if (modBlock != 0) {
        return tModifierBlock_getBoundBox(modBlock);
    }
    return (int)self->Info;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_hasAnimMats__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a floating node has animation matrices.
 *
 * @return always 1
 */
int cFloatingNode_hasAnimMats(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_getAnimMats);
#ifdef SKIP_ASM
struct sRailVEntry17B8 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Returns the floating node's animation matrices, first refreshing the root transform when it is dirty.
 *
 * When bit 0 (RootTransformDirty) of Flags (+0x12) is set, calls the virtual updateRootTransform() (slot 51,
 * cFloatingNode_updateRootTransform).
 *
 * @return the matrix array pointer stored at +0x74
 */
extern "C" int cFloatingNode_getAnimMats(cFloatingNode* self)
{
    if (self->Flags & 1) {
        sRailVEntry17B8* vt = (sRailVEntry17B8*)self->mVtbl;
        vt[51].fn((char*)self + vt[51].delta);
    }
    return (int)self->AnimMats;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_getAnimMat);
#ifdef SKIP_ASM
struct sVEntry00361800 {
    short delta;
    short index;
    void (*fn)(void*);
};


/**
 * Returns one of the floating node's animation matrices, first refreshing the root transform when it is dirty.
 *
 * The refresh is the same as cFloatingNode_getAnimMats(): updateRootTransform() (slot 51) when Flags bit 0 is
 * set.
 *
 * @param i  index of the matrix in the array at +0x74 (0x40-byte matrices)
 * @return the address of matrix @p i
 */
extern "C" t4Matrix* cFloatingNode_getAnimMat(cFloatingNode* self, int i)
{
    if (self->Flags & 1) {
        sVEntry00361800* vt = (sVEntry00361800*)self->mVtbl;
        vt[51].fn((char*)self + vt[51].delta);
    }
    return &(self->AnimMats)[i];
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cFloatingNode_updateRootTransform);
#ifdef SKIP_ASM
struct sRmVEntryP1858 {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sRmVEntry2_1858 {
    short delta;
    short index;
    void (*fn)(void*, void*, void*);
};

struct sRmVtbl1858 {
    sRmVEntryP1858 pad[24];
    sRmVEntryP1858 get;      // slot 24 (0xC0)
    sRmVEntryP1858 pad2[8];
    sRmVEntry2_1858 set;     // slot 33 (0x108)
};

/**
 * Rebuilds a floating node's part matrices and clears its dirty bit.
 *
 * Calls makeAnimMats (slot 33) with the root transform (slot 24) and the matrix array at +0x74.
 */
extern "C" void cFloatingNode_updateRootTransform(cFloatingNode* self)
{
    sRmVtbl1858* vt = (sRmVtbl1858*)self->mVtbl;
    void* obj = (char*)self + vt->set.delta;
    void* transform = vt->get.fn((char*)self + vt->get.delta);
    vt->set.fn(obj, transform, self->AnimMats);
    self->Flags &= ~1;
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
void cPositionModifier_update(void* self)
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
 * Returns the transform-modifier type id of cPositionModifier: 3.
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return 3
 */
int cPositionModifier_getTransformType(void* self)
{
    return 0x3;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
/**
 * Returns a position modifier's BoundBox (self + 0x10).
 *
 * @return pointer to BoundBox
 */
void* cPositionModifier_getBoundBox(void* self)
{
    return &((cPositionModifier*)self)->BoundBox;
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
extern "C" void cPositionModifier_setBoundBox(cPositionModifier* self, tBoundBox* boundbox)
{
    *(cQuad128*)&self->BoundBox = *(cQuad128*)boundbox;
    *(cQuad128*)&self->BoundBox.MaxPoint = *(cQuad128*)&boundbox->MaxPoint;
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
float cPositionModifier_getRadius(void* vself)
{
    cPositionModifier* self = (cPositionModifier*)vself;
    return self->Radius;
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
void cPositionModifier_setRadius(void* vself, float val)
{
    cPositionModifier* self = (cPositionModifier*)vself;
    self->Radius = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_getRootTransform);
#ifdef SKIP_ASM
struct sRailVEntry1940 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Returns the position modifier's root transform (Transform, +0x50), first rebuilding it (slot 3,
 * updateRootTransform) when RootTransformDirty is set.
 *
 * @return the transform matrix
 */
extern "C" void* cPositionModifier_getRootTransform(cPositionModifier* self)
{
    if (self->RootTransformDirty != 0) {
        sRailVEntry1940* vt = *(sRailVEntry1940**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return (char*)&self->Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier__cParentModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys a parent modifier: restores the cTransformModifier base vtable (cTransformModifier_vtbl) and frees the object when
 * bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cParentModifier__cParentModifier(cParentModifier* self, int flags)
{
    *(void***)self = cTransformModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_update__FPv);
#ifdef SKIP_ASM
/**
 * The parent modifier's update virtual: marks the root transform dirty (+0x44) so it follows the parent every
 * frame.
 */
void cParentModifier_update(void* vself)
{
    cParentModifier* self = (cParentModifier*)vself;
    self->RootTransformDirty = 1;
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
int cParentModifier_getMesh(void* vself)
{
    cParentModifier* self = (cParentModifier*)vself;
    return self->Mesh;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cParentModifier: 4.
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return 4
 */
int cParentModifier_getTransformType(void* self)
{
    return 0x4;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_setParent);
#ifdef SKIP_ASM
/**
 * Sets a parent modifier's ParentInst (+0xA0). Nothing in retail calls it.
 *
 * @param modifier  the parent modifier
 * @param parentInst  instance record to follow
 */
extern "C" void cParentModifier_setParent(void *modifier, int parentInst) {
    (*(int *)((char*)(modifier) + (0xA0))) = parentInst;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier__cMagnetModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys a magnet modifier: restores the cTransformModifier base vtable (cTransformModifier_vtbl) and frees the object when
 * bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cMagnetModifier__cMagnetModifier(cMagnetModifier* self, int flags)
{
    *(void***)self = cTransformModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_isRootTransformDirty__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the magnet's transform changed since it was last rebuilt (RootTransformDirty, +0x50).
 *
 * @return 1 when dirty, otherwise 0
 */
int cMagnetModifier_isRootTransformDirty(void* vself)
{
    cMagnetModifier* self = (cMagnetModifier*)vself;
    return (self->RootTransformDirty != 0);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_hasIntersect__FPv);
#ifdef SKIP_ASM
/**
 * The magnet modifier's hasIntersect virtual: always 1 (the magnet object takes part in rider intersection).
 *
 * @return always 1
 */
int cMagnetModifier_hasIntersect(void* self)
{
    return 0x1;
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
void* cMagnetModifier_getBoundBox(void* self)
{
    return &((cMagnetModifier*)self)->BoundBox;
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
extern "C" void cMagnetModifier_setBoundBox(cMagnetModifier* self, tBoundBox* boundbox)
{
    *(cQuad128*)&self->BoundBox = *(cQuad128*)boundbox;
    *(cQuad128*)&self->BoundBox.MaxPoint = *(cQuad128*)&boundbox->MaxPoint;
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
float cMagnetModifier_getRadius(void* vself)
{
    cMagnetModifier* self = (cMagnetModifier*)vself;
    return self->Radius;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_setRadius__FPv);
#ifdef SKIP_ASM
/**
 * The magnet modifier's setRadius virtual: an empty stub, so the radius set by the constructor never changes (On
 * Tour setRadius(float radius); the float is ignored).
 */
void cMagnetModifier_setRadius(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cMagnetModifier: 6.
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return 6
 */
int cMagnetModifier_getTransformType(void* self)
{
    return 0x6;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_getRootTransform);
#ifdef SKIP_ASM
struct sRailVEntry1A60 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Returns the magnet modifier's root transform (Transform, +0x60), first rebuilding it (slot 3, updateRootTransform)
 * when RootTransformDirty is set.
 *
 * @return the transform matrix
 */
extern "C" void* cMagnetModifier_getRootTransform(cMagnetModifier* self)
{
    if (self->RootTransformDirty != 0) {
        sRailVEntry1A60* vt = *(sRailVEntry1A60**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return (char*)&self->Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier__cAvaSplineModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys an avalanche spline modifier: restores the cTransformModifier base vtable (cTransformModifier_vtbl) and frees the
 * object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAvaSplineModifier__cAvaSplineModifier(cAvaSplineModifier* self, int flags)
{
    *(void***)self = cTransformModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
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
void* cAvaSplineModifier_getBoundBox(void* self)
{
    return &((cAvaSplineModifier*)self)->BoundBox;
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
extern "C" void cAvaSplineModifier_setBoundBox(cAvaSplineModifier* self, tBoundBox* boundbox)
{
    *(cQuad128*)&self->BoundBox = *(cQuad128*)boundbox;
    *(cQuad128*)&self->BoundBox.MaxPoint = *(cQuad128*)&boundbox->MaxPoint;
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
float cAvaSplineModifier_getRadius(void* vself)
{
    cAvaSplineModifier* self = (cAvaSplineModifier*)vself;
    return self->Radius;
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
void cAvaSplineModifier_setRadius(void* vself, float val)
{
    cAvaSplineModifier* self = (cAvaSplineModifier*)vself;
    self->Radius = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform-modifier type id of cAvaSplineModifier: 7.
 *
 * Callers use the id to tell the modifier kinds apart (On Tour returns the same value).
 *
 * @return 7
 */
int cAvaSplineModifier_getTransformType(void* self)
{
    return 0x7;
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
void* cAvaSplineModifier_getRootTransform(void* self)
{
    return &((cAvaSplineModifier*)self)->Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier__cSplineModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys a spline modifier: stores the base cTransformModifier vtable, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cSplineModifier__cSplineModifier(cSplineModifier* self, int flags)
{
    *(void***)self = cTransformModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
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
void cSplineModifier_clearComplete(void* vself)
{
    cSplineModifier* self = (cSplineModifier*)vself;
    self->Complete = 0;
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
void* cSplineModifier_getBoundBox(void* self)
{
    return &((cSplineModifier*)self)->BoundBox;
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
extern "C" void cSplineModifier_setBoundBox(cSplineModifier* self, tBoundBox* boundbox)
{
    *(cQuad128*)&self->BoundBox = *(cQuad128*)boundbox;
    *(cQuad128*)&self->BoundBox.MaxPoint = *(cQuad128*)&boundbox->MaxPoint;
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
float cSplineModifier_getRadius(void* vself)
{
    cSplineModifier* self = (cSplineModifier*)vself;
    return self->Radius;
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
void cSplineModifier_setRadius(void* vself, float val)
{
    cSplineModifier* self = (cSplineModifier*)vself;
    self->Radius = val;
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
struct sRailVEntry1B90 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Returns the spline modifier's root transform, rebuilding it first when it is dirty.
 *
 * When the dirty flag at +0x58 is set, calls the virtual updateRootTransform() (slot 3).
 *
 * @return the address of the root transform matrix at +0x60
 */
extern "C" void* cSplineModifier_getRootTransform(cSplineModifier* self)
{
    if (self->RootTransformDirty != 0) {
        sRailVEntry1B90* vt = *(sRailVEntry1B90**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return (char*)&self->Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform type id of a spline modifier.
 *
 * @return always 1 (multi-spline modifiers return 2, roller modifiers 5)
 */
int cSplineModifier_getTransformType(void* self)
{
    return 0x1;
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
void* cMultiSplineModifier_getBoundBox(void* vself)
{
    cMultiSplineModifier* self = (cMultiSplineModifier*)vself;
    return (char*)self->Nodes + 0x30;
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
extern "C" void cMultiSplineModifier_setBoundBox(cMultiSplineModifier* self, tBoundBox* boundbox)
{
    void* nodes = self->Nodes;
    *(cQuad128*)((char*)nodes + 0x30) = *(cQuad128*)boundbox;
    *(cQuad128*)((char*)nodes + 0x40) = *(cQuad128*)&boundbox->MaxPoint;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_getFocusCount);
#ifdef SKIP_ASM
/**
 * Returns a multi-spline modifier's FocusCount.
 *
 * @param multiSplineModifier  the multi-spline modifier
 * @return FocusCount
 */
extern "C" int cMultiSplineModifier_getFocusCount(cMultiSplineModifier* multiSplineModifier) {
    return (multiSplineModifier->FocusCount);
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
float cMultiSplineModifier_getRadius(void* vself)
{
    cMultiSplineModifier* self = (cMultiSplineModifier*)vself;
    return self->Radius;
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
void cMultiSplineModifier_setRadius(void* vself, float val)
{
    cMultiSplineModifier* self = (cMultiSplineModifier*)vself;
    self->Radius = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_getRootTransform1);
#ifdef SKIP_ASM
struct sRailVEntry1C20 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Returns the root transform of the modifier's first instance, rebuilding the transforms first when dirty.
 *
 * When the dirty flag at +0x30 is set, calls the virtual updateRootTransform() (slot 3). The result is +0x10 of
 * the first entry in the instance array at +0x44 (an instance record's root transform).
 *
 * @return the address of the first instance's root transform matrix
 */
extern "C" void* cMultiSplineModifier_getRootTransform1(cMultiSplineModifier* self)
{
    if (self->RootTransformDirty != 0) {
        sRailVEntry1C20* vt = *(sRailVEntry1C20**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return *(char**)self->NodeInst + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMultiSplineModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform type id of a multi-spline modifier.
 *
 * @return always 2 (spline modifiers return 1, roller modifiers 5)
 */
int cMultiSplineModifier_getTransformType(void* self)
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier__cRollerModifier);
#ifdef SKIP_ASM
extern void* cTransformModifier_vtbl[];
void operator_delete(int*);

/**
 * Destroys a roller modifier: stores the base cTransformModifier vtable, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRollerModifier__cRollerModifier(cRollerModifier* self, int flags)
{
    *(void***)self = cTransformModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
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
int cRollerModifier_shouldRebound(void* self)
{
    return 0;
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
 * @return 1 when Life >= 17.0, otherwise 0
 */
extern "C" int cRollerModifier_isComplete(cRollerModifier* self)
{
    return self->Life >= 17.0f;
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
void* cRollerModifier_collide1(void* vself)
{
    cRollerModifier* self = (cRollerModifier*)vself;
    *(int*)&self->Life = 0;
    return cRollerModifier_Collision(self);
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
int cRollerModifier_runCollide(void* self)
{
    return 0;
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
extern "C" void cRollerModifier_setBoundBox(cRollerModifier* self, tBoundBox* bb)
{
    *(cQuad128*)&self->boundbox = *(cQuad128*)bb;
    *(cQuad128*)&self->boundbox.MaxPoint = *(cQuad128*)&bb->MaxPoint;
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
void* cRollerModifier_getBoundBox(void* self)
{
    return &((cRollerModifier*)self)->boundbox;
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
float cRollerModifier_getRadius(void* vself)
{
    cRollerModifier* self = (cRollerModifier*)vself;
    return self->Radius;
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
void cRollerModifier_setRadius(void* vself, float val)
{
    cRollerModifier* self = (cRollerModifier*)vself;
    self->Radius = val;
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
void* cRollerModifier_getRootTransform(void* self)
{
    return &((cRollerModifier*)self)->Transform;
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
void* cRollerModifier_getRootTransform1(void* self)
{
    return &((cRollerModifier*)self)->Transform;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_getTransformType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the transform type id of a roller modifier.
 *
 * @return always 5 (spline modifiers return 1, multi-spline modifiers 2)
 */
int cRollerModifier_getTransformType(void* self)
{
    return 0x5;
}
#endif

extern void* cParticleNode_vtbl[];
extern "C" void* cMoveNode__cMoveNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361D48);
#ifdef SKIP_ASM
/**
 * Returns the int at +0x10 of its argument. Nothing in retail calls it.
 *
 * @param arg0  the object to read
 * @return that int
 */
extern "C" int func_00361D48(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParticleNode__cParticleNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a particle node: stores cParticleNode's vtable at +0xC, then runs the cMoveNode destructor.
 *
 * The g++ delete flags in the second argument register pass through untouched to cMoveNode__cMoveNode().
 *
 * @return no meaningful value (cMoveNode__cMoveNode returns nothing)
 */
void* cParticleNode__cParticleNode(void* vself)
{
    cParticleNode* self = (cParticleNode*)vself;
    self->mVtbl = (void**)((int)(void*)cParticleNode_vtbl);
    return cMoveNode__cMoveNode(self);
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
 * @return self
 */
void* cAnimComboNode_getNodeState(void* self)
{
    return self;
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
extern "C" int cAnimComboNode_getAnimDir(cAnimComboNode* self)
{
    int direction;
    if (*(signed char*)&self->AnimActive2 != 0) {
        direction = -1;
        if (self->AnimSpeed2 >= 0.0f) {
            direction = 1;
        }
        return direction;
    }
    direction = -1;
    if (self->AnimSpeed >= 0.0f) {
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
    self->mVtbl = (void**)((int)(void*)cAnimDeltaNode_vtbl);
    return cAnimObjectNode__cAnimObjectNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAnimDeltaNode_getNodeState__FPv);
#ifdef SKIP_ASM
/**
 * Returns an anim delta node's state block, which is the node itself.
 *
 * @return self
 */
void* cAnimDeltaNode_getNodeState(void* self)
{
    return self;
}
#endif

extern "C" void* func_0035FE10(int, int);

//99.38%
INCLUDE_ASM("object/railmodifier", func_00361E10__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static initialisation: func_0035FE10 with initialize 1 and priority 0xFFFF.
 *
 * The argument is not used.
 *
 * @return func_0035FE10's result
 */
void* func_00361E10(void* self)
{
    return func_0035FE10(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361E30);
#ifdef SKIP_ASM
extern "C" int func_00362340(void);
extern "C" int func_003623A8(void);
extern "C" void func_00361F98(void* self, int cause, int (*handler)(void), int arg);

/**
 * Installs the renderer's two INTC handlers: func_00362340 for cause 5 (VIF1) and func_003623A8 for cause 7 (VU1).
 *
 * Both go through func_00361F98 with a 0 argument, which records each handler id in the per-cause table at +0x0.
 */
extern "C" void func_00361E30(void* self)
{
    func_00361F98(self, 5, func_00362340, 0);
    func_00361F98(self, 7, func_003623A8, 0);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361E80);
#ifdef SKIP_ASM
extern "C" void* func_00362008(void* self, int id);

/**
 * Removes the renderer's INTC handlers for causes 5 (VIF1) and 7 (VU1) with func_00362008.
 *
 * @return func_00362008's result for cause 7
 */
extern "C" void* func_00361E80(void* self)
{
    func_00362008(self, 5);
    return func_00362008(self, 7);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", func_00361EB8);
#ifdef SKIP_ASM
struct sRailPairs;
extern "C" void func_00361F60(sRailPairs* self);
extern "C" void func_00361E30(void* self);

static inline void func_00361EB8_fill(int* p, int v)
{
    int i;
    for (i = 15; i >= 0; i--) {
        p[i] = v;
    }
}

/**
 * Sets up the renderer's interrupt handler table (D_005059D8 in cPS2GraphicsMan_Init).
 *
 * Fills the 16 INTC handler ids (+0x0) and the 16 DMAC handler ids (+0x40) with -1 (the preview build's bxps2intc.cpp
 * asserts call these mIntCID and mDMACID), clears the counters at +0x80 (func_00361F60) and installs the VIF1/VU1
 * handlers (func_00361E30).
 */
extern "C" void func_00361EB8(void* self)
{
    func_00361EB8_fill((int*)self, -1);
    func_00361EB8_fill((int*)((char*)self + 0x40), -1);
    func_00361F60((sRailPairs*)self);
    func_00361E30(self);
}
#endif

extern "C" void* func_00361E80(void* self);

//99.29%
INCLUDE_ASM("object/railmodifier", func_00361F40__FPv);
#ifdef SKIP_ASM
/**
 * Tears down the renderer's interrupt handlers by removing the VIF1/VU1 ones (func_00361E80).
 *
 * cPS2GraphicsMan_Purge calls it on D_005059D8.
 *
 * @return func_00361E80's result
 */
void* func_00361F40(void* self)
{
    return func_00361E80(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361F60);
#ifdef SKIP_ASM
struct sRailPair {
    int a;
    int b;
};

struct sRailPairs {
    char pad_0x00[0x80];
    sRailPair pairs[3];
};

/**
 * Zeroes the three counter pairs at +0x80 of the interrupt handler table (the VIF1 handler counts into the first).
 */
extern "C" void func_00361F60(sRailPairs* self)
{
    int i;
    for (i = 0; i < 3; i++) {
        self->pairs[i].a = 0;
        self->pairs[i].b = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361F90__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing; cPS2GraphicsMan_DrawFrame calls it on the interrupt handler table (D_005059D8) at the end of a frame.
 */
void func_00361F90(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361F98);
#ifdef SKIP_ASM
extern "C" int DisableIntc(int ch);
extern "C" int func_00423A80(int cause, int (*handler)(void), int arg);
extern "C" int EnableIntc(int ch);

/**
 * Registers an INTC handler for an interrupt cause and records its id.
 *
 * Calls DisableIntc(cause), stores the id func_00423A80(cause, handler, next) returns at +cause*4, then calls
 * EnableIntc(cause).
 *
 * @param cause  INTC cause number (5 = VIF1, 7 = VU1)
 * @param handler  handler function
 * @param next  argument passed through to func_00423A80
 */
extern "C" void func_00361F98(void* self, int cause, int (*handler)(void), int next)
{
    DisableIntc(cause);
    *(int*)((char*)self + (cause << 2)) = func_00423A80(cause, handler, next);
    EnableIntc(cause);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362008);
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
extern "C" void* func_00362008(void* self, int ch)
{
    DisableIntc(ch);
    int* handlerSlot = (int*)((char*)self + (ch << 2));
    int id = *handlerSlot;
    *handlerSlot = -1;
    return RemoveIntcHandler(ch, id);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362058);
#ifdef SKIP_ASM
extern "C" int func_00423AB0(int, int, int);
extern "C" void DisableDmac(int);
extern "C" void EnableDmac(int);

/**
 * Registers a DMAC handler for a channel and records its id.
 *
 * Calls DisableDmac(channel), stores the id func_00423AB0(channel, handler, next) returns at +0x40 + channel*4, then
 * calls EnableDmac(channel).
 *
 * @param owner  the interrupt handler table
 * @param channel  DMA channel number
 * @param handler  handler function
 * @param next  argument passed through to func_00423AB0
 */
extern "C" void func_00362058(int owner, int channel, int handler, int next) {
    DisableDmac(channel);
    (*(int *)(((char*)(owner) + (channel << 2)) + (0x40))) = func_00423AB0(channel, handler, next);
    EnableDmac(channel);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003620C8);
#ifdef SKIP_ASM
extern "C" void RemoveDmacHandler(int, int);

/**
 * Removes the DMAC handler recorded for a channel.
 *
 * Calls DisableDmac(idx), sets the channel's slot (+0x40 + idx*4) to -1 and calls RemoveDmacHandler with the old id.
 *
 * @param idx  DMA channel number
 */
extern "C" void func_003620C8(char* self, int idx)
{
    DisableDmac(idx);
    char* base = self + 0x40;
    int* slot = (int*)(base + (idx << 2));
    int old = *slot;
    *slot = -1;
    RemoveDmacHandler(idx, old);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362120);
#ifdef SKIP_ASM
/**
 * Stores all 32 FPU registers into buf as 64-bit words (the interrupt handlers' save half; func_00362230 restores).
 *
 * @param buf  0x100-byte save area
 */
// PORT: PS2-only inline asm (dump all 32 FPU registers as 64-bit words into buf).
extern "C" void func_00362120(void* buf)
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
INCLUDE_ASM("object/railmodifier", func_00362230);
#ifdef SKIP_ASM
/**
 * Reloads all 32 FPU registers from buf, as saved by func_00362120.
 *
 * @param buf  0x100-byte save area
 */
// PORT: PS2-only inline asm (reload all 32 FPU registers from buf; deliberately
// no FPU clobbers so the compiler doesn't save/restore $f20-$f31 around it).
extern "C" void func_00362230(void* buf)
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
INCLUDE_ASM("object/railmodifier", func_00362340);
#ifdef SKIP_ASM
extern "C" void func_00362120(void*);
extern "C" void func_00362230(void*);

struct sIntcStats00362340 {
    char pad_0x00[0x80];
    int count;
};
extern sIntcStats00362340 D_005059D8;

/**
 * Handles the VIF1 interrupt: cancels a VIF1 interrupt stall and counts it.
 *
 * Saves the FPU registers (func_00362120). When VIF1_STAT (0x10003C00) has bit 10 set (stalled on an interrupt) it
 * writes 8 (STC, stall cancel) to VIF1_FBRST (0x10003C10). It counts the interrupt at D_005059D8 +0x80, restores the
 * FPU registers (func_00362230) and re-enables interrupts.
 *
 * @return 1 (handled)
 */
extern "C" int func_00362340(void)
{
    char buf[0x100];
    func_00362120(buf);
    sIntcStats00362340* stats = &D_005059D8;
    if (*(int*)0x10003C00 & 0x400) {
        *(volatile int*)0x10003C10 = 8;
    }
    stats->count++;
    func_00362230(buf);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", func_003623A8);
#ifdef SKIP_ASM
extern "C" void func_00362120(void*);
extern "C" void func_00362230(void*);

/**
 * Handles the VU1 interrupt: only saves and restores the FPU registers and re-enables interrupts.
 *
 * @return 1 (handled)
 */
extern "C" int func_003623A8(void)
{
    char buf[0x100];
    func_00362120(buf);
    func_00362230(buf);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003623D8);
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
extern "C" void func_003623D8(sGifPacket* pkt, int x, int w, int y, int height)
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
INCLUDE_ASM("object/railmodifier", func_00362448);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sPair64 {
    long a;
    long b;
};

/**
 * Appends a TEXFLUSH GS register write (A+D pair, register 0x3F, data 0) to a packet and counts it.
 */
extern "C" void func_00362448(void* self)
{
    sPair64*& cur = *(sPair64**)((char*)self + 0x4);
    cur->a = 0;
    cur->b = 0x3F;
    cur = cur + 1;
    *(int*)((char*)self + 0x0) += 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362478);
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
extern "C" void func_00362478(sGifPacket* pkt, int mode, int fix, int ctx)
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
INCLUDE_ASM("object/railmodifier", func_003625C0);
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
extern "C" void func_003625C0(sGifPacket* pkt, int mode, int ctx)
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
INCLUDE_ASM("object/railmodifier", func_00362660);
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
extern "C" void func_00362660(sGifPacket* pkt, unsigned int zbp, unsigned int psm, int zmsk, int ctx)
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
INCLUDE_ASM("object/railmodifier", func_003626D8);
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
extern "C" void func_003626D8(sRmGsPacket* pkt, int ztest, int atest, unsigned int aref, int ctx)
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
INCLUDE_ASM("object/railmodifier", func_003627A8);
#ifdef SKIP_ASM
extern ulong D_00505A80[];

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
 * builds it at D_00505A80 with a RET tag and appends a CALL to D_00505A80 at *pp; modes 1 and 2 write it at the packet
 * base with a CNT tag after handing the packet's current end back through *pp.
 *
 * @param pp  the caller's DMA chain cursor
 * @param mode  0, 1 or 2 as above
 */
// PORT: 64-bit DMA/VIF/GIF tag writes (ulong) and addresses held in int.
extern "C" void func_003627A8(sRmPkt27A8* self, ulong** pp, int mode)
{
    if (self->count == 0) {
        return;
    }
    switch (mode) {
    case 0:
        self->cur = D_00505A80;
        self->cur[0] = (self->count + 2) | 0x60000000;
        self->cur[1] = 0;
        self->cur[2] = 0;
        self->cur[3] = (ulong)(unsigned int)((self->count + 1) | 0x50000000) << 32;
        self->cur[4] = (ulong)(self->count | 0x8000) | ((ulong)1 << 60);
        self->cur[5] = 0xE;
        self->cur += 6;
        (*pp)[0] = ((ulong)(unsigned int)D_00505A80 << 32) | 0x50000000;
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
INCLUDE_ASM("object/railmodifier", func_00362978);
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
extern "C" void func_00362978(void* self, int packet, int tail)
{
    if (*(int*)((char*)self + 0xc) != 0) {
        ulong* tag = (ulong*)(*(int*)((char*)self + 0x10) | 0x30000000);
        tag[0] = ((ulong)packet << 32) | 0x20000000;
        tag[1] = 0;
        *(int*)((char*)self + 0x10) = tail;
    } else {
        *(int*)((char*)self + 0xc) = packet;
        *(int*)((char*)self + 0x10) = tail;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003629B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00362CC8(char* self);

struct sRmState_29B8 {
    unsigned int w0;
    unsigned int w1;
    unsigned int w2;
    unsigned int w3;
    short tex[2];
};

struct sRmCtx_29B8 {
    char pad0[0xE84];
    sRmState_29B8* top;     // 0xE84
};

struct sRmSaved_29B8 {
    sRmState_29B8 st;
    int pad;
};

struct sRmBatch_29B8 {
    char pad_0x0[0x51480];
    int count;                      // 0x51480
    sRmSaved_29B8 saved[1024];      // 0x51484
    int base;                       // 0x57484
    sRmSaved_29B8* a;               // 0x57488
    sRmSaved_29B8* b;               // 0x5748C
    sRmSaved_29B8* c;               // 0x57490
};

extern sRmCtx_29B8* D_004A5B80_29B8 __asm__("cGraphicsMan_TheGraphicsMan");
extern sRmState_29B8 D_00501420_29B8 __asm__("sDrawState_s_defaultDrawState");

static inline void set0_29B8(sRmCtx_29B8* ctx, unsigned int mask, int shift, unsigned int v)
{
    ctx->top->w0 = (ctx->top->w0 & ~mask) | ((v << shift) & mask);
}

static inline void set1_29B8(sRmCtx_29B8* ctx, unsigned int mask, int shift, unsigned int v)
{
    ctx->top->w1 = (ctx->top->w1 & ~mask) | ((v << shift) & mask);
}

static inline void set2_29B8(sRmCtx_29B8* ctx, unsigned int mask, int shift, unsigned int v)
{
    ctx->top->w2 = (ctx->top->w2 & ~mask) | ((v << shift) & mask);
}

static inline sRmSaved_29B8* save_29B8(sRmBatch_29B8* self, sRmCtx_29B8* ctx)
{
    sRmState_29B8* src = ctx->top;
    sRmSaved_29B8* d = &self->saved[self->count++];
    d->st = *src;
    return d;
}

/**
 * Builds the defer manager's three preset draw states.
 *
 * Clears the static base (+0x57484) and resets the manager (func_00362CC8). Starting from sDrawState_s_defaultDrawState in the current
 * draw-state slot of cGraphicsMan_TheGraphicsMan, it edits the state bit fields three times and saves each copy into the draw-state
 * array (+0x51484, count +0x51480): the presets at +0x57488, +0x5748C and +0x57490. The base is then set to the count
 * so later resets keep them.
 */
extern "C" void func_003629B8(sRmBatch_29B8* self)
{
    self->base = 0;
    func_00362CC8((char*)self);
    sRmCtx_29B8* ctx = D_004A5B80_29B8;
    *ctx->top = D_00501420_29B8;
    set1_29B8(ctx, 0x3, 0, 0);
    set2_29B8(ctx, 0x1FFFFC00, 10, 0);
    set1_29B8(ctx, 0x1800000, 23, 1);
    set1_29B8(ctx, 0x300000, 20, 0);
    set1_29B8(ctx, 0xFF000, 12, 20);
    set0_29B8(ctx, 0x30, 4, 3);
    set0_29B8(ctx, 0x3C0, 6, 3);
    set1_29B8(ctx, 0x7C, 2, 1);
    set1_29B8(ctx, 0xF80, 7, 17);
    self->a = save_29B8(self, ctx);
    set1_29B8(ctx, 0xF80, 7, 2);
    self->b = save_29B8(self, ctx);
    set1_29B8(ctx, 0x7C, 2, 5);
    set1_29B8(ctx, 0xF80, 7, 3);
    set1_29B8(ctx, 0x3, 0, 1);
    set2_29B8(ctx, 0x1FFFFC00, 10, 0);
    set1_29B8(ctx, 0x1800000, 23, 1);
    set1_29B8(ctx, 0x300000, 20, 3);
    set1_29B8(ctx, 0xFF000, 12, 92);
    self->c = save_29B8(self, ctx);
    self->base = self->count;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362CC8);
#ifdef SKIP_ASM
extern "C" void InvalidDCache(void* begin, void* end);
extern "C" void func_0038F738(void* base, int size, int mode);
extern "C" void MEM_fill(void* dst, int c, int n);
extern void* D_004A5B84;

struct sRail20_2CC8 {
    int v[5];
};

extern sRail20_2CC8 sDrawState_s_defaultDrawState;

/**
 * Resets the defer manager for a new frame.
 *
 * Zeroes the record count (+0x0) and the bucket count (+0x57494), drops the draw states saved after the presets
 * (+0x51480 = +0x57484), clears the two 0x400-byte hash tables at +0x674A0 and +0x678A0 by DMA from the zero-filled
 * scratchpad (D_004A5B84), zeroes the sort-item count (+0x67CA4) and the scissor count (+0x6AED0), sets the current
 * draw state (+0x69CA8) to sDrawState_s_defaultDrawState, and the current matrix (+0x69CC0) and scissor (+0x69CC4) to
 * -1 with no cached matrices (+0x69CC8).
 */
extern "C" void func_00362CC8(char* self)
{
    *(int*)(self + 0x0) = 0;
    *(int*)(self + 0x57494) = 0;
    *(int*)(self + 0x51480) = *(int*)(self + 0x57484);
    char* stateHashTable = self + 0x674A0;
    InvalidDCache(stateHashTable, self + 0x6789F);
    char* nodeHashTable = self + 0x678A0;
    InvalidDCache(nodeHashTable, self + 0x67C9F);
    MEM_fill(D_004A5B84, 0, 0x400);
    func_0038F738(stateHashTable, 0x400, 2);
    func_0038F738(nodeHashTable, 0x400, 2);
    *(int*)(self + 0x67CA4) = 0;
    sRail20_2CC8* currentState = (sRail20_2CC8*)(self + 0x69CA8);
    *currentState = sDrawState_s_defaultDrawState;
    *(int*)(self + 0x6AED0) = 0;
    *(int*)(self + 0x69CC0) = -1;
    *(int*)(self + 0x69CC8) = 0;
    *(int*)(self + 0x69CC4) = -1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362DE8);
#ifdef SKIP_ASM
extern void* D_004A4474;
extern "C" void SyncDCache(void* begin, void* end);
extern "C" void* func_0038F4F8(void* ring, void* src, int size, int flags);
extern "C" void func_0038F6A8(void* ring);
extern "C" void func_0038F7B0();
extern "C" unsigned int func_00364240(unsigned int* self, int key);

// One deferred draw record (0x80; REF structs/render.md / n2-render-layer.md "Record"): render state
// words, texture key, DMA packet start/end, matrix index, continuation flag.
struct sRmEntry_2DE8 {
    unsigned int w[4];      // 0x0
    short tex[2];           // 0x10
    int packet;             // 0x14
    int tail;               // 0x18
    short mtxIndex;         // 0x1C
    short continuation;     // 0x1E
    char pad_0x20[0x60];
};

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
    sRmEntry_2DE8 entries[0xA29];       // 0x80
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
extern "C" unsigned int* cPS2DeferManager_Find(sRmBatch_2DE8* self, sRmEntry_2DE8* e, int hash);
extern "C" sRmNode_2DE8* cPS2DeferManager_Find1(sRmBatch_2DE8* self, unsigned int* state, int key, int hash, int mtxIndex);
extern "C" void func_00362978(void* self, int packet, int tail);

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

static inline unsigned int* findState_2DE8(sRmBatch_2DE8* self, sRmEntry_2DE8* e)
{
    int x = e->w[0] ^ e->w[1] ^ e->w[2] ^ e->w[3];
    return cPS2DeferManager_Find(self, e, hash8_2DE8(x));
}

static inline void addEntry_2DE8(sRmBatch_2DE8* self, sRmEntry_2DE8* e, unsigned int* state, int key)
{
    sRmNode_2DE8* node;
    if (e->continuation != 0)
        node = 0;
    else
        node = cPS2DeferManager_Find1(self, state, key, nodeHash_2DE8(state, key), e->mtxIndex);
    if (node == 0) {
        sRmNode_2DE8* n = newNode_2DE8(self, state, key, e->mtxIndex);
        node = n;
        if (e->continuation != 0 && self->last != 0) {
            node->linked = 1;
        } else {
            unsigned int sortKey = func_00364240(state, key);
            int h = nodeHash_2DE8(state, key);
            n->next = self->nodeHash[h];
            self->nodeHash[h] = n;
            self->items[self->nItems].key = sortKey;
            self->items[self->nItems++].node = n;
        }
    }
    func_00362978(node, e->packet, e->tail);
    if (e->continuation != 0) {
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
 * Streams the 0x80-byte records (+0x80, count +0x0) through the scratchpad ring D_004A4474 in chunks. For each it finds
 * the shared state (cPS2DeferManager_Find) and, unless it continues the previous record, the bucket with the same state,
 * texture pair and matrix index (cPS2DeferManager_Find1). A new bucket is added to the hash and gets a sort item with the key
 * from func_00364240; continuation records are chained to the previous bucket. The record's packet is appended to the
 * bucket with func_00362978.
 *
 * @param vself  the defer manager
 */
// PORT: (int) of a pointer for hashing.
extern "C" void func_00362DE8(void* vself)
{
    sRmBatch_2DE8* self = (sRmBatch_2DE8*)vself;
    self->last = 0;
    SyncDCache(self->entries, (char*)&self->entries[self->count] - 1);
    void* ring = D_004A4474;
    int bytes = *(int*)((char*)ring + 0x10) << 4;
    int chunk = bytes / sizeof(sRmEntry_2DE8);
    int nChunks = self->count / chunk;
    int chunkBytes = chunk * sizeof(sRmEntry_2DE8);
    sRmEntry_2DE8* buf = (sRmEntry_2DE8*)func_0038F4F8(ring, self->entries, chunkBytes, 4);
    for (int i = 0; i < nChunks; i++) {
        sRmEntry_2DE8* next = (sRmEntry_2DE8*)func_0038F4F8(ring, &self->entries[(i + 1) * chunk], chunkBytes, 4);
        for (int j = 0; j < chunk; j++) {
            sRmEntry_2DE8* drawRecord = &buf[j];
            unsigned int* state = findState_2DE8(self, drawRecord);
            sRmTexKey_2DE8 key;
            key.set(drawRecord->tex[0], drawRecord->tex[1]);
            addEntry_2DE8(self, drawRecord, state, key.i);
        }
        buf = next;
        func_0038F6A8(ring);
        func_0038F7B0();
    }
    func_0038F7B0();
    for (int j = 0; j < self->count - chunk * nChunks; j++) {
        sRmEntry_2DE8* drawRecord = &buf[j];
        unsigned int* state = findState_2DE8(self, drawRecord);
        sRmTexKey_2DE8 key;
        key.set(drawRecord->tex[0], drawRecord->tex[1]);
        addEntry_2DE8(self, drawRecord, state, key.i);
    }
    func_0038F6A8(ring);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00363490);
#ifdef SKIP_ASM
extern void* D_004A4474;
extern char* cGraphicsMan_TheGraphicsMan_railmodifier __asm__("cGraphicsMan_TheGraphicsMan");
extern const int D_004A4058[];
extern "C" void func_00362DE8(void* self);
extern "C" void func_00364360(void* self, ulong** pp);
extern "C" void func_00364050(char* self);
extern "C" void func_003666F8();
extern "C" void func_0038F300(void* ring, int mode);
extern "C" void func_00396128(void* obj);
extern "C" void func_00365F68(void* self);
extern "C" void func_00365FA8(void* self);
extern "C" void func_00363C20(void* self, unsigned int* state, int layer, ulong** pp);
extern "C" void func_00368660(void* obj, int u, int v, int idx, ulong** pp, int* path);
extern "C" void func_0036AC00(void* gs, int i, ulong** pp, int* path);
extern "C" void func_0036C790(void* gs, int i, ulong** pp, int* path);
extern "C" void func_003904A0(void* gs, int i, ulong** pp, int* path);

// A batch node: one (state, texture, layer) combination with its DMA chain (0x20 bytes).
struct sRmNode_3490 {
    unsigned int* state;    // 0x0
    int key;                // 0x4
    int layer;              // 0x8
    int packet;             // 0xC
    int tail;               // 0x10
    int linked;             // 0x14
    sRmNode_3490* link;     // 0x18
    sRmNode_3490* next;     // 0x1C
};

struct sRmSortItem_3490 {
    unsigned int key;
    sRmNode_3490* node;
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

extern sRmTexUse_3490 D_0053AB80_3490[] __asm__("D_0053AB80");

static inline void resetState_3490(sRail20_2CC8* d)
{
    *d = sDrawState_s_defaultDrawState;
}

/**
 * Builds the frame's DMA chains from the deferred draw records.
 *
 * Buckets the records (func_00362DE8), writes the default state (func_00364360), sorts the buckets (func_00364050),
 * resets the texture use table (func_003666F8) and prepares the texture manager (cGraphicsMan_TheGraphicsMan +0x18F4:
 * func_00396128 and func_00365F68 on +0x1F60). It records each texture's last use and use count in D_0053AB80, then
 * splits the sorted list into three passes at the pass types in D_004A4058 (state word 2 bits 5-9). For every bucket it
 * writes the state change (func_00363C20) when the state differs, the texture setup (func_00368660) and a CALL to the
 * bucket's chain, whose tail becomes a RET. After a type-6 pass it runs func_0036AC00 per entry and resets the state;
 * after a type-9 pass it ends both chains with END tags, reports their positions in out1/out2 and runs
 * func_0036C790/func_003904A0 per entry. Finally it ends both chains, stores the main chain's end in *pktAddr and
 * closes the texture manager (func_00365FA8).
 *
 * Likely bug: the copy of D_004A4058 holds two pass types, so the third pass switches on an unwritten stack word.
 *
 * @param pktAddr  main chain cursor (in: start, out: end)
 * @param out1  receives the main chain position after the type-9 pass
 * @param path  second chain cursor
 * @param out2  receives the second chain position after the type-9 pass
 */
// PORT: DMA tags written as 64-bit words through uncached (| 0x30000000 / | 0x20000000) addresses held in int.
extern "C" void func_00363490(sRmBatch_3490* self, int* pktAddr, int* out1, int* path, int* out2)
{
    sRmPassTypes_3490 split;
    int bounds[3];
    ulong* pkt;
    func_00362DE8(self);
    *out1 = 0;
    *out2 = 0;
    pkt = (ulong*)(*pktAddr | 0x30000000);
    resetState_3490(&self->state);
    func_00364360(self, &pkt);
    func_00364050((char*)self);
    func_003666F8();
    void* ring = D_004A4474;
    int saved = *(int*)ring;
    func_0038F300(ring, 5);
    char* gs = cGraphicsMan_TheGraphicsMan_railmodifier;
    void* obj = *(void**)(gs + 0x18F4);
    func_00396128(obj);
    func_00365F68((char*)obj + 0x1F60);
    if (self->nItems != 0) {
        int idx = 0;
        int pass;
        for (int i = 0; i < self->nItems; i++) {
            for (sRmNode_3490* n = self->items[i].node; n != 0; n = n->link) {
                short primaryTex = ((short*)&n->key)[0];
                if (primaryTex >= 0) {
                    D_0053AB80_3490[primaryTex].first = idx;
                    D_0053AB80_3490[primaryTex].count++;
                }
                short secondaryTex = ((short*)&n->key)[1];
                if (secondaryTex >= 0) {
                    D_0053AB80_3490[secondaryTex].first = idx;
                    D_0053AB80_3490[secondaryTex].count++;
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
                if ((int)((self->items[mid].node->state[2] & 0x3E0) >> 5) >= split.v[j])
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
                sRmNode_3490* n = self->items[i].node;
                if (n != 0) {
                    do {
                        unsigned int* st = n->state;
                        if (st != prev) {
                            func_00363C20(self, st, n->layer, &pkt);
                            prev = st;
                        }
                        int key = n->key;
                        func_00368660(obj, ((short*)&key)[0], ((short*)&key)[1], idx++, &pkt, path);
                        if (n->packet != 0) {
                            pkt[0] = ((ulong)(unsigned int)n->packet << 32) | 0x50000000;
                            pkt[1] = 0;
                            pkt += 2;
                            ulong* tailTag = (ulong*)(n->tail | 0x30000000);
                            tailTag[0] = 0x60000000;
                            tailTag[1] = 0;
                        }
                        n = n->link;
                    } while (n != 0);
                }
            }
            switch (split.v[pass]) {
            case 6: {
                for (int k = 0; k < *(int*)(gs + 0x59C0); k++)
                    func_0036AC00(gs, k, &pkt, path);
                self->lastTex = -1;
                prev = 0;
                resetState_3490(&self->state);
                func_00364360(self, &pkt);
                int* tc = (int*)((char*)obj + 0x1F4C);
                for (int k = 0; k < 2; k++)
                    tc[k] = -1;
                *(int*)((char*)obj + 0x1F54) = -1;
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
                for (int k = 0; k < *(int*)(gs + 0x59C0); k++)
                    func_0036C790(gs, k, &pkt, path);
                for (int k = 0; k < *(int*)(gs + 0x59C0); k++)
                    func_003904A0(gs, k, &pkt, path);
                self->lastTex = -1;
                prev = 0;
                resetState_3490(&self->state);
                func_00364360(self, &pkt);
                int* tc = (int*)((char*)obj + 0x1F4C);
                for (int k = 0; k < 2; k++)
                    tc[k] = -1;
                *(int*)((char*)obj + 0x1F54) = -1;
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
    func_00365FA8((char*)obj + 0x1F60);
    func_0038F300(ring, saved);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00363C20);
#ifdef SKIP_ASM
// FAKEMATCH: `self` is taken as an integer (`__SIZE_TYPE__ selfi`, cast to sRmRend3C20* in the body), so its pseudo is not
// pointer-flagged and sched1's find_base_term cannot tell `self+0x68000` from the stack: the scissor load then stays after
// the packet-header stores, as in retail (integer-typed base steering); plain form 96.29%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void func_00362478(sGifPacket* pkt, int a, int b, int ctx);
extern "C" void func_00364CD0(void* self, int a, int b, unsigned int c, ulong** pp);
extern char* cGraphicsMan_TheGraphicsMan_railmodifier __asm__("cGraphicsMan_TheGraphicsMan");

struct sRmGs3C20 {
    unsigned int w0;
    unsigned int w1;
    unsigned int w2;
    unsigned int w3;
    short tex[2];
};

struct sRmKey3C20 {
    unsigned short a, b, c, d;
};

struct sRmRend3C20 {
    char pad_0x0[0x69CA8];
    sRmGs3C20 state;            // 0x69CA8
    unsigned int frame;         // 0x69CBC
    int buf;                    // 0x69CC0
    int scissor;                // 0x69CC4
    char pad_0x69CC8[0x6AED4 - 0x69CC8];
    sRmKey3C20 keys[1];         // 0x6AED4
};

static inline sRmGs3C20* State3C20(sRmRend3C20* self)
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
extern "C" void func_00363C20_impl(__SIZE_TYPE__ selfi, sRmGs3C20* st, int a2, ulong** pp) __asm__("func_00363C20");
/**
 * Writes the GS register changes needed to go from the defer manager's current draw state (+0x69CA8) to st.
 *
 * Starts with a one-quadword CNT tag. When st's VU1 program type (word 0 bits 6-9) is nonzero it emits the program
 * setup (func_00364CD0): mode 0 when the type changes, mode 1 when only the matrix/layer index changes, mode 2 when the
 * frame word (word 3 bits 0-29) changes. It then collects A+D writes only for fields that differ: SCISSOR from the
 * rectangle table (+0x6AED4) indexed by word 2 bits 0-4, ZBUF for both contexts on the Z-mask bit, TEST for both
 * contexts, CLAMP_1/2 (word 0 bits 2-3 / 4-5) and ALPHA_1/2 (word 1 bits 2-6 / 7-11 with FIX from word 0). It copies st
 * into the current state and closes the block with func_003627A8 mode 1.
 *
 * @param selfi  the defer manager (passed as an integer)
 * @param st  draw state to switch to
 * @param nodeLayer  the bucket's matrix/layer index
 * @param pp  DMA chain cursor
 */
extern "C" void func_00363C20_impl(__SIZE_TYPE__ selfi, sRmGs3C20* st, int nodeLayer, ulong** pp)
{
    sRmRend3C20* self = (sRmRend3C20*)selfi;
    sRmPkt27A8 pkt;
    (*pp)[0] = 0x10000001;
    (*pp)[1] = 0;
    (*pp)[2] = 0;
    (*pp)[3] = (ulong)1 << 60;
    *pp += 4;
    int ctx = (st->w0 & 0x3C0) >> 6;
    if (ctx != 0) {
        unsigned int frameWord = Mask3C20(0x3FFFFFFF, st->w3);
        if (((State3C20(self)->w0 & 0x3C0) >> 6) != ctx) {
            self->buf = nodeLayer;
            self->frame = frameWord;
            func_00364CD0(self, (st->w0 & 0x3C0) >> 6, 0, frameWord, pp);
        } else if (nodeLayer != self->buf) {
            self->buf = nodeLayer;
            self->frame = frameWord;
            func_00364CD0(self, (st->w0 & 0x3C0) >> 6, 1, frameWord, pp);
        }
        if (self->frame != frameWord) {
            self->buf = nodeLayer;
            self->frame = frameWord;
            func_00364CD0(self, (st->w0 & 0x3C0) >> 6, 2, frameWord, pp);
        }
    }
    pkt.count = 0;
    pkt.base = *pp;
    pkt.cur = *pp + 6;
    int k = st->w2 & 0x1F;
    if (k != self->scissor) {
        self->scissor = k;
        if (k >= 0) {
            func_003623D8((sGifPacket*)&pkt, self->keys[k].a, self->keys[k].c, self->keys[k].b, self->keys[k].d);
        }
    }
    if (((State3C20(self)->w1 & 0x400000) >> 22) != ((st->w1 & 0x400000) >> 22)) {
        char* gs = cGraphicsMan_TheGraphicsMan_railmodifier;
        func_00362660((sGifPacket*)&pkt, *(unsigned int*)(gs + 0x5A80), *(unsigned int*)(gs + 0x5A40),
                      (st->w1 & 0x400000) >> 22, 0);
        func_00362660((sGifPacket*)&pkt, *(unsigned int*)(gs + 0x5A80), *(unsigned int*)(gs + 0x5A40),
                      (st->w1 & 0x400000) >> 22, 1);
    }
    if (Bits3C20(State3C20(self)->w1, 0x1800000) != Bits3C20(st->w1, 0x1800000) ||
        Bits3C20(State3C20(self)->w1, 0x300000) != Bits3C20(st->w1, 0x300000) ||
        Bits3C20(State3C20(self)->w1, 0xFF000) != Bits3C20(st->w1, 0xFF000)) {
        func_003626D8((sRmGsPacket*)&pkt, (st->w1 & 0x1800000) >> 23, (st->w1 & 0x300000) >> 20,
                      (st->w1 & 0xFF000) >> 12, 0);
        func_003626D8((sRmGsPacket*)&pkt, (st->w1 & 0x1800000) >> 23, (st->w1 & 0x300000) >> 20,
                      (st->w1 & 0xFF000) >> 12, 1);
    }
    if ((State3C20(self)->w0 & 0xC) != (st->w0 & 0xC)) {
        func_003625C0((sGifPacket*)&pkt, (st->w0 & 0xC) >> 2, 0);
    }
    if (((State3C20(self)->w1 & 0x7C) >> 2) != ((st->w1 & 0x7C) >> 2) || ((State3C20(self)->w0 >> 12) & 0xFF) != ((st->w0 >> 12) & 0xFF)) {
        func_00362478((sGifPacket*)&pkt, (st->w1 & 0x7C) >> 2, (st->w0 >> 12) & 0xFF, 0);
    }
    if ((State3C20(self)->w0 & 0x30) != (st->w0 & 0x30)) {
        func_003625C0((sGifPacket*)&pkt, (st->w0 & 0x30) >> 4, 1);
    }
    if (((State3C20(self)->w1 & 0xF80) >> 7) != ((st->w1 & 0xF80) >> 7) || ((State3C20(self)->w0 >> 20) & 0xFF) != ((st->w0 >> 20) & 0xFF)) {
        func_00362478((sGifPacket*)&pkt, (st->w1 & 0xF80) >> 7, (st->w0 >> 20) & 0xFF, 1);
    }
    self->state = *st;
    func_003627A8(&pkt, pp, 1);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00364050);
#ifdef SKIP_ASM
struct sRmSortItem4050 {
    unsigned int key;
    int val;
};

extern sRmSortItem4050 D_00505B70[];
extern void* D_004A4474;
extern "C" void* func_0038F460(void* ring, unsigned int addr, int size, int flags);
extern "C" void func_0038F6A8(void* ring);

/**
 * Sorts the defer manager's sort items (+0x67CA8, count +0x67CA4) by key with a four-pass 8-bit radix sort.
 *
 * The 4 x 257 histogram lives in the scratchpad ring (func_0038F460) and D_00505B70 is the swap buffer; after the
 * fourth pass the sorted items are back in place.
 */
extern "C" void func_00364050(char* self)
{
    void* ring = D_004A4474;
    sRmSortItem4050* dst = D_00505B70;
    int n = *(int*)(self + 0x67CA4);
    sRmSortItem4050* src = (sRmSortItem4050*)(self + 0x67CA8);
    int (*hist)[257] = (int (*)[257])func_0038F460(ring, 0, -1, 0);
    MEM_fill(hist, 0, 4112);
    for (int i = 0; i < n; i++) {
        unsigned int key = src[i].key;
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
            int idx = digitCounts[(src[i].key >> shift) & 0xFF]++;
            dst[idx] = src[i];
        }
        sRmSortItem4050* sortedBuf = dst;
        dst = src;
        src = sortedBuf;
    }
    func_0038F6A8(ring);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00364240);
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
extern "C" unsigned int func_00364240(unsigned int* self, int key)
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
INCLUDE_ASM("object/railmodifier", func_00364360);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void func_00362478(sGifPacket* pkt, int a, int b, int ctx);
extern "C" void func_00364CD0(void* self, int a, int b, unsigned int c, ulong** pp);
// PORT: bound by asm label so this caller's packet type doesn't conflict with the definition.
extern "C" void func_003627A8_4360(void* pkt, ulong** pp, int mode) __asm__("func_003627A8");
extern char* cGraphicsMan_TheGraphicsMan_railmodifier __asm__("cGraphicsMan_TheGraphicsMan");

struct sRmGsState4360 {
    unsigned int w0;
    unsigned int w1;
    unsigned int w2;
    unsigned int w3;
    short tex[2];
    sRmGsState4360()
    {
        unsigned int i;
        for (i = 0; i < 2; i++) {
            tex[i] = -1;
        }
        func_00416210(this, 0, 0x10);
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
 * After a one-quadword CNT tag it emits the VU1 program setup (func_00364CD0 mode 0) and CLAMP, ALPHA, ZBUF and TEST
 * for context 1 and then context 2, closing the block with func_003627A8 mode 1.
 *
 * Likely bug: the context-2 ZBUF and TEST writes take their fields from word 1 one or two bits higher than the
 * context-1 writes and func_00363C20 do.
 *
 * @param pp  DMA chain cursor
 */
// PORT: 64-bit DMA/GIF tag writes (ulong).
extern "C" void func_00364360(void* self, ulong** pp)
{
    char* gs = cGraphicsMan_TheGraphicsMan_railmodifier;
    sRmGsState4360 st;
    sRmPkt4360 pkt;
    st = *(sRmGsState4360*)&sDrawState_s_defaultDrawState;
    (*pp)[0] = 0x10000001;
    (*pp)[1] = 0;
    (*pp)[2] = 0;
    (*pp)[3] = (ulong)1 << 60;
    *pp += 4;
    func_00364CD0(self, (st.w0 & 0x3C0) >> 6, 0, st.w3 & 0x3FFFFFFF, pp);
    pkt.count = 0;
    pkt.base = *pp;
    pkt.cur = *pp + 6;
    func_003625C0((sGifPacket*)&pkt, (st.w0 & 0xC) >> 2, 0);
    func_00362478((sGifPacket*)&pkt, (st.w1 & 0x7C) >> 2, (st.w0 >> 12) & 0xFF, 0);
    func_00362660((sGifPacket*)&pkt, *(unsigned int*)(gs + 0x5A80), *(unsigned int*)(gs + 0x5A40), (st.w1 >> 22) & 1, 0);
    func_003626D8((sRmGsPacket*)&pkt, (st.w1 >> 23) & 3, (st.w1 >> 20) & 3, (st.w1 & 0xFF000) >> 12, 0);
    func_003625C0((sGifPacket*)&pkt, (st.w0 & 0x30) >> 4, 1);
    func_00362478((sGifPacket*)&pkt, (st.w1 & 0xF80) >> 7, (st.w0 >> 20) & 0xFF, 1);
    func_00362660((sGifPacket*)&pkt, *(unsigned int*)(gs + 0x5A80), *(unsigned int*)(gs + 0x5A40), (st.w1 >> 23) & 1, 1);
    func_003626D8((sRmGsPacket*)&pkt, (st.w1 >> 25) & 3, (st.w1 >> 22) & 3, (st.w1 & 0xFF000) >> 12, 1);
    func_003627A8_4360(&pkt, pp, 1);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003645B8);
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

// One cached transform set (0x180 bytes).
struct sRailXf_45B8 {
    sM44_45B8 m0;       // 0x000
    sM44_45B8 m1;       // 0x040
    sM44_45B8 m2;       // 0x080
    sV4_45B8 v0;        // 0x0C0
    sV4_45B8 v1;        // 0x0D0
    sM44_45B8 key0;     // 0x0E0
    sM44_45B8 key1;     // 0x120
    sV4_45B8 key2;      // 0x160
    sV4_45B8 key3;      // 0x170

    // Each compares one key of a cached entry against this probe; entry first.
    int Key0Eq(const sM44_45B8& e) const { return MTX_EQ_45B8(e, key0); }
    int Key1Eq(const sM44_45B8& e) const { return MTX_EQ_45B8(e, key1); }
    int Key2Eq(const sV4_45B8& e) const { return VEC_EQ_45B8(e, key2); }
    int Key3Eq(const sV4_45B8& e) const { return VEC_EQ_45B8(e, key3); }
};

struct sRailXfTable_45B8 {
    char pad_0x00[0x69CC0];
    int found;                  // 0x69CC0
    int pad_0x69CC4;
    int count;                  // 0x69CC8
    int pad_0x69CCC;
    sRailXf_45B8 entries[1];    // 0x69CD0
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
extern "C" void func_003645B8(sRailXfTable_45B8* self, sRailXf_45B8* in)
{
    int i;
    self->found = -1;
    for (i = 0; i < self->count; i++) {
        if (in->Key0Eq(self->entries[i].key0) && in->Key1Eq(self->entries[i].key1) &&
            in->Key2Eq(self->entries[i].key2) && in->Key3Eq(self->entries[i].key3)) {
            self->found = i;
            break;
        }
    }
    if (self->found < 0) {
        self->entries[self->count].m0 = in->m0;
        self->entries[self->count].m1 = in->m1;
        self->entries[self->count].m2 = in->m2;
        self->entries[self->count].v0 = in->v0;
        self->entries[self->count].v1 = in->v1;
        self->entries[self->count].key0 = in->key0;
        self->entries[self->count].key1 = in->key1;
        self->entries[self->count].key2 = in->key2;
        self->entries[self->count].key3 = in->key3;
        self->found = self->count;
        self->count++;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00364B88);
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
extern "C" void func_00364B88(sRailKeyTable* self, sRailKey4* k)
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
INCLUDE_ASM("object/railmodifier", func_00364C90);
#ifdef SKIP_ASM
struct sData20 {
    int w[5];
};

/**
 * Sets the defer manager's current draw state (+0x69CA8) to a copy of the 20-byte state src.
 *
 * @param src  draw state to copy
 */
extern "C" void func_00364C90(void* self, sData20* src)
{
    *(sData20*)((char*)self + 0x69CA8) = *src;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00364CD0);
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
extern char* cBezierMan_TheBezierMan;
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

struct sVec_4CD0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// One DMA quadword.
struct sQw_4CD0 {
    int w[4];
} __attribute__((aligned(16)));

// One matrix record of the defer manager (0x180 bytes; REF structs/render.md "MatrixRecord").
struct sMtxRec_4CD0 {
    sMtx_4CD0 world;        // 0x000
    float view[16];         // 0x040
    sQw_4CD0 worldView[4];  // 0x080
    sVec_4CD0 guardScale;   // 0x0C0
    sVec_4CD0 guardOffset;  // 0x0D0
    float viewProj[16];     // 0x0E0
    sQw_4CD0 worldClip[4];  // 0x120
    sVec_4CD0 screenScale;  // 0x160
    sVec_4CD0 screenOffset; // 0x170
};

struct sDefer_4CD0 {
    char pad_0x0[0x69CC0];
    int curMtx;                 // 0x69CC0
    char pad_0x69CC4[0xC];
    sMtxRec_4CD0 mtx[1];        // 0x69CD0
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
extern "C" void func_00364CD0(void* vself, int type, int mode, unsigned int w3, ulong** pp)
{
    sDefer_4CD0* self = (sDefer_4CD0*)vself;
    sPkt_4CD0 pk(*pp);
    sMtxRec_4CD0* mtxRec = &self->mtx[self->curMtx];
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
            char* bezierMan = cBezierMan_TheBezierMan;
            pk.p[0x1E] = ((ulong)*(int*)(bezierMan + 0x1A0) << 32) | 0x30000012;
            pk.p[0x1F] = 0;
            ulong ref2 = ((ulong)*(int*)(bezierMan + 0x320) << 32) | 0x30000012;
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
            sVec_4CD0 scaleVec = mtxRec->guardScale;
            scaleVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &scaleVec, 1);
            sVec_4CD0 offsetVec = mtxRec->guardOffset;
            offsetVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &offsetVec, 1);
            sVec_4CD0 projScale;
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
            sVec_4CD0 scaleVec = mtxRec->screenScale;
            scaleVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &scaleVec, 1);
            sVec_4CD0 offsetVec = mtxRec->screenOffset;
            offsetVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &offsetVec, 1);
            sVec_4CD0 projScale;
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
            sVec_4CD0 scaleVec = mtxRec->screenScale;
            scaleVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &scaleVec, 1);
            sVec_4CD0 offsetVec = mtxRec->screenOffset;
            offsetVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &offsetVec, 1);
            sVec_4CD0 projScale;
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
            sVec_4CD0 scaleVec = mtxRec->screenScale;
            scaleVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &scaleVec, 1);
            sVec_4CD0 offsetVec = mtxRec->screenOffset;
            offsetVec.z *= 0.0625f;
            putQw_4CD0(pk.p, &offsetVec, 1);
            sVec_4CD0 projScale;
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
INCLUDE_ASM("object/railmodifier", func_00365E40);
#ifdef SKIP_ASM
extern "C" void func_003660A8(void* self, int idx);

struct sRailLinkE40 {
    int flags;      // 0x0
    int f4;         // 0x4
    int prev;       // 0x8
    int fC;         // 0xC
    int next;       // 0x10
    int f14;        // 0x14
    int f18;        // 0x18
};

struct sRailSlotE40 {
    char pad_0x0[0x14];
    int a;          // 0x14
    int b;          // 0x18
};

struct sRailModE40 {
    sRailLinkE40 store[0x124];  // 0x0
    sRailLinkE40* links;        // 0x1FF0
    int f1FF4;                  // 0x1FF4
    sRailSlotE40 slotsA[17];    // 0x1FF8
    sRailSlotE40 slotsB[17];    // 0x21D4
    char pad_0x23B0[0xC];
    int f23BC;                  // 0x23BC
    int f23C0;                  // 0x23C0
    int f23C4;                  // 0x23C4
    int f23C8;                  // 0x23C8
    char pad_0x23CC[0xC];
    int f23D8;                  // 0x23D8
    int f23DC;                  // 0x23DC
    int f23E0;                  // 0x23E0
    int f23E4;                  // 0x23E4
    void* pool;                 // 0x23E8
};

/**
 * Sets up a texture-memory block allocator with one free block and an empty link pool.
 *
 * Block 0 covers the whole size from the start address (+0x1FF4 = size), links 1-291 form the spare-link stack (head
 * +0x23DC = 1), the 17 size-class heads of both lists are emptied, the address list starts at block 0 (+0x23C0) and
 * block 0 is filed into its size class (func_003660A8). pool (+0x23E8) is the owner table freed blocks report to.
 *
 * @param list  the allocator
 * @param startAddr  start address
 * @param heapSize  size
 * @param pool  owner table
 */
extern "C" void func_00365E40(void* list, int startAddr, int heapSize, void* pool)
{
    sRailModE40* self = (sRailModE40*)list;
    int i;
    self->f1FF4 = heapSize;
    self->links[0].flags = startAddr;
    self->links[0].f4 = heapSize;
    self->links[0].fC = -1;
    self->links[0].next = -1;
    self->links[0].prev = -1;
    for (i = 1; i < 291; i++) {
        self->links[i].flags &= ~1;
        self->links[i].next = i + 1;
    }
    self->links[291].next = -1;
    for (i = 0; i < 17; i++) {
        self->slotsA[i].b = -1;
        self->slotsA[i].a = -1;
        self->slotsB[i].b = -1;
        self->slotsB[i].a = -1;
    }
    self->f23C0 = 0;
    self->f23BC = -1;
    self->f23C4 = -1;
    self->f23C8 = -1;
    func_003660A8(self, 0);
    self->pool = pool;
    self->f23D8 = -1;
    self->f23E0 = -1;
    self->f23E4 = -1;
    self->f23DC = 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00365F68);
#ifdef SKIP_ASM
extern "C" void SyncDCache(void* begin, void* end);
extern "C" int func_0038F708(void* base, int size, int mode);

/**
 * Moves a block allocator's link array into the scratchpad ring for fast access.
 *
 * Writes the 0x1FF0-byte array back from the data cache (SyncDCache), copies it with func_0038F708 and points the
 * links pointer (+0x1FF0) at the copy. func_00365FA8 undoes it.
 */
extern "C" void func_00365F68(void* self)
{
    SyncDCache(self, (char*)self + 0x1FEF);
    *(int*)((char*)self + 0x1FF0) = func_0038F708(self, 0x1FF0, 2);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00365FA8);
#ifdef SKIP_ASM
extern "C" void InvalidDCache(void* begin, void* end);
extern "C" void func_0038F738(void* base, int size, int mode);

/**
 * Moves a block allocator's link array back from the scratchpad ring (InvalidDCache, func_0038F738) and points the
 * links pointer (+0x1FF0) at it again.
 */
extern "C" void func_00365FA8(void* self)
{
    InvalidDCache(self, (char*)self + 0x1FEF);
    func_0038F738(self, 0x1FF0, 2);
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
INCLUDE_ASM("object/railmodifier", func_00365FE8);
#ifdef SKIP_ASM
/**
 * Pops a spare link off the allocator's stack (head +0x23DC).
 *
 * @return the link index
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Pop a link index off the free list headed at f23DC.
extern "C" int func_00365FE8(sRailModE40* self)
{
    int i = self->f23DC;
    self->f23DC = self->links[i].next;
    return i;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366008);
#ifdef SKIP_ASM
struct sRailNode {
    int field_0x0;
    int size;               // 0x04
    int field_0x8;
    int prev;               // 0x0c
    int next;               // 0x10
    int prev2;              // 0x14
    int next2;              // 0x18
};

struct sRailNodeMan {
    char pad_0x0[0x1ff0];
    sRailNode* nodes;       // 0x1ff0
    int field_0x1ff4;
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
extern "C" void func_00366008(sRailNodeMan* self, int idx)
{
    sRailNode* node = &self->nodes[idx];
    sRailNode* prev;
    int link = node->prev;
    if (link < 0) {
        if ((link & 0x70000000) == 0x10000000) {
            prev = &self->headsA[link & 0x0FFFFFFF];
        } else {
            prev = &self->headsB[link & 0x0FFFFFFF];
        }
    } else {
        prev = &self->nodes[link];
    }
    prev->next = node->next;
    if (node->next >= 0) {
        self->nodes[node->next].prev = node->prev;
    }
    node->next = self->freeHead;
    self->freeHead = idx;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003660A8);
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
extern "C" void func_003660A8(void* selfp, int idx)
{
    sRailNodeMan* self = (sRailNodeMan*)selfp;
    sRailNode* node = &self->nodes[idx];
    sRailNode* heads;
    int tag;
    if (node->field_0x0 & 1) {
        heads = self->headsA;
        tag = 0x90000000;
    } else {
        heads = self->headsB;
        tag = 0xA0000000;
    }
    int sizeClass = railFloorLog2_60A8(node->size) - 6;
    sRailNode* head = &heads[sizeClass];
    int first = head->next2;
    if (first == -1) {
        head->next2 = idx;
        node->prev2 = sizeClass | tag;
        node->next2 = first;
        return;
    }
    for (int i = first; i >= 0; i = self->nodes[i].next2) {
        sRailNode* n = &self->nodes[i];
        if (n->next2 == -1) {
            n->next2 = idx;
            node->prev2 = i;
            node->next2 = -1;
            return;
        }
        if ((heads == self->headsA && n->field_0x0 > node->field_0x0) ||
            (heads == self->headsB && n->size >= node->size)) {
            int link = n->prev2;
            node->next2 = i;
            node->prev2 = link;
            sRailNode* prev;
            if (link < 0) {
                if ((link & 0x70000000) == 0x10000000) {
                    prev = &self->headsA[link & 0x0FFFFFFF];
                } else {
                    prev = &self->headsB[link & 0x0FFFFFFF];
                }
            } else {
                prev = &self->nodes[link];
            }
            prev->next2 = idx;
            self->nodes[node->next2].prev2 = idx;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366238);
#ifdef SKIP_ASM
/**
 * Removes a block from its size-class list.
 *
 * @param idx  block index
 */
extern "C" void func_00366238(sRailNodeMan* self, int idx)
{
    sRailNode* node = &self->nodes[idx];
    sRailNode* prev;
    int link = node->prev2;
    if (link < 0) {
        if ((link & 0x70000000) == 0x10000000) {
            prev = &self->headsA[link & 0x0FFFFFFF];
        } else {
            prev = &self->headsB[link & 0x0FFFFFFF];
        }
    } else {
        prev = &self->nodes[link];
    }
    prev->next2 = node->next2;
    if (node->next2 >= 0) {
        self->nodes[node->next2].prev2 = node->prev2;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003662D0);
#ifdef SKIP_ASM
extern "C" void func_003660A8(void* self, int idx);
// PORT: the unit declares func_00365FE8 as void; it returns the popped free index.
extern "C" int func_00365FE8_pop(void* self) __asm__("func_00365FE8");

/**
 * Allocates size units from a free block, splitting off the rest as a new free block when a spare link is left.
 *
 * The block leaves its size list; on an exact fit (or no spare link) the whole block is marked used. Otherwise a new
 * block (func_00365FE8) takes the remainder, with no owner, after it in the address list and in its size class. The
 * block is marked used (bit 0) and refiled (func_003660A8).
 *
 * @param idx  free block to allocate from
 * @param size  units needed
 */
extern "C" void func_003662D0(sRailNodeMan* self, int idx, int size)
{
    sRailNode* node = &self->nodes[idx];
    func_00366238(self, idx);
    if (size == node->size || self->freeHead == -1) {
        node->field_0x0 |= 1;
    } else {
        int n = func_00365FE8_pop(self);
        sRailNode* nn = &self->nodes[n];
        nn->field_0x0 = node->field_0x0 + size;
        nn->size = node->size - size;
        nn->field_0x8 = -1;
        nn->prev = idx;
        int next = node->next;
        nn->next = next;
        if (next >= 0) {
            self->nodes[next].prev = n;
        }
        node->next = n;
        func_003660A8(self, n);
        node->size = size;
        node->field_0x0 |= 1;
    }
    func_003660A8(self, idx);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003663D8);
#ifdef SKIP_ASM
/**
 * Frees a used block, detaches it from its owner and merges it with free neighbours.
 *
 * Unless the owner (+0x8) is 0x7FFFFFFF, sets the owner texture's block fields (pool +0x23E8 entry: +0x28/+0x30, or
 * +0x2C/+0x34 when flag bit 2 is set) to -1. Clears the low six flag bits, refiles the block and repeatedly joins it
 * with a free block before or after it in the address list.
 *
 * @param selfp  the allocator
 * @param idx  block index
 */
extern "C" void func_003663D8(void* selfp, int idx)
{
    sRailNodeMan* self = (sRailNodeMan*)selfp;
    sRailNode* n = &self->nodes[idx];
    if (n->field_0x8 != 0x7FFFFFFF) {
        char* owner = (*(char***)((char*)self + 0x23E8))[n->field_0x8];
        if ((n->field_0x0 >> 2) & 1) {
            *(int*)(owner + 0x2C) = -1;
            *(int*)(owner + 0x34) = -1;
        } else {
            *(int*)(owner + 0x28) = -1;
            *(int*)(owner + 0x30) = -1;
        }
    }
    n->field_0x0 &= ~0x3F;
    func_00366238(self, idx);
    func_003660A8(self, idx);
    for (;;) {
        int prevIdx = n->prev;
        if (prevIdx >= 0 && (self->nodes[prevIdx].field_0x0 & 1) == 0) {
            idx = prevIdx;
            n = &self->nodes[prevIdx];
        } else {
            int nextIdx = n->next;
            if (nextIdx < 0 || (self->nodes[nextIdx].field_0x0 & 1)) {
                return;
            }
        }
        func_00366238(self, idx);
        func_00366238(self, n->next);
        n->size += self->nodes[n->next].size;
        func_00366008(self, n->next);
        func_003660A8(self, idx);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366548);
#ifdef SKIP_ASM
struct sRailLink6548 {
    int flags;      // 0x0
    int f4;
    int prev;       // 0x8
    int fC;
    int next;       // 0x10
    int f14;
    int f18;
};

/**
 * Frees every used block of an allocator that is not locked (flag bit 1), walking the address list (+0x23C0).
 *
 * The next used block is found before each free, as freeing merges neighbours. func_00367360 (the texture manager's
 * purge) runs it on both of its allocators.
 */
extern "C" void func_00366548(void* self)
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
                func_003663D8(self, i);
            }
        }
        i = next;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366618);
#ifdef SKIP_ASM
struct sRailLink6618 {
    int flags;      // 0x0
    int f4;
    int prev;       // 0x8
    int fC;
    int next;       // 0x10
    int f14;
    int f18;
};

/**
 * Frees every used block of an allocator, clearing its lock bit (flag bit 1) first.
 */
extern "C" void func_00366618(void* self)
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
            func_003663D8(self, i);
        }
        i = next;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003666F8);
#ifdef SKIP_ASM
extern void* D_004A5B84;
extern char D_0053AB80[];
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
    char* p = (char*)D_004A5B84;
    return p ? 0x70004000 - (int)p : 0;
}

/**
 * Resets the 2000-entry texture use table D_0053AB80 to all-ones (-1).
 *
 * Invalidates the table's cache lines (InvalidDCache), fills the free scratchpad above D_004A5B84 with 0xFF bytes and
 * copies that into the table in chunks with func_0038F738.
 */
extern "C" void func_003666F8()
{
    cQuad128 fill = makeFill66F8(0xFFFFFFFF, 0xFFFFFFFF);
    InvalidDCache(D_0053AB80, D_0053E9FF);
    cQuad128* spadCursor = (cQuad128*)D_004A5B84;
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
        func_0038F738(&((sRailPair66F8*)D_0053AB80)[i], chunk * 8, 2);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003667F8);
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
extern "C" int func_003667F8(sRailNodeMan* self, int size)
{
    int bucket;
    for (bucket = railFloorLog2(size) - 6; bucket < 17; bucket++) {
        int i = self->headsB[bucket].next2;
        while (i >= 0) {
            if (self->nodes[i].size >= size) {
                return i;
            }
            i = self->nodes[i].next2;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366888);
#ifdef SKIP_ASM
struct sRailLink6888 {
    int flags;      // 0x0
    int f4;
    int prev;       // 0x8
    int fC;
    int next;       // 0x10
    int f14;
    int f18;
};

/**
 * Returns the largest free block in the address list (ties go to the later one).
 *
 * @return the block index, or -1 when no block is free
 */
extern "C" int func_00366888(void* self)
{
    char* heapBase = (char*)self;
    int i = *(int*)(heapBase + 0x23C0);
    int best = 0;
    int idx = -1;
    while (i >= 0) {
        sRailLink6888* links = *(sRailLink6888**)(heapBase + 0x1FF0);
        if (!(links[i].flags & 1)) {
            int blockSize = links[i].f4;
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
INCLUDE_ASM("object/railmodifier", func_003668F8);
#ifdef SKIP_ASM
extern "C" void func_003663D8(void* selfp, int idx);

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
 * skipB. A block whose owner texture is used again later this frame (its last use in D_0053AB80 is after age) costs
 * size times its use count, any other costs 0; the cheapest is freed with func_003663D8.
 *
 * @param size  units needed
 * @param skipA  owner to keep
 * @param skipB  another owner to keep
 * @param age  current position in the frame's draw order
 * @return 1 if a block was freed, else 0
 */
extern "C" int func_003668F8(sRailNodeMan* self, int size, int skipA, int skipB, int age)
{
    int i = self->headsA[railFloorLog2v(size) - 6].next2;
    int bestIdx = -1;
    int best = 0x7FFFFFFF;
    for (; i >= 0; i = self->nodes[i].next2) {
        sRailNode* n = &self->nodes[i];
        int sz = n->size;
        if (sz < size) {
            continue;
        }
        if ((n->field_0x0 >> 1) & 1) {
            continue;
        }
        int owner = n->field_0x8;
        if (owner == skipA || owner == skipB) {
            continue;
        }
        sRailPair66F8* pr = &((sRailPair66F8*)D_0053AB80)[owner];
        int cost = age < pr->a ? sz * pr->b : 0;
        if (cost < best) {
            best = cost;
            bestIdx = i;
        }
    }
    if (bestIdx < 0) {
        return 0;
    }
    func_003663D8(self, bestIdx);
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003669F0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_003663D8(void* selfp, int idx);

/**
 * Frees the cheapest run of neighbouring blocks (in address order) that together hold at least @p size bytes.
 *
 * Despite the unit, this is part of the GS memory block heap that cTextureMan keeps at +0x1F60 and +0x4350. It slides a
 * window along the address-ordered block list (headed at headsB[17].next): a used block that is locked (flag bit 1) or
 * owned by @p skipA or @p skipB breaks the window. A used block whose owner's entry in the 2000-pair table D_0053AB80
 * has a > @p level adds size * b to the window's cost; free blocks cost nothing. The search stops early at cost 0.
 * Every used block in the winning window is then freed with func_003663D8 (which also merges free neighbours).
 * func_00366CE0 calls it when freeing a single block (func_003668F8) was not enough.
 *
 * Likely bug: when no window qualifies, bestIdx stays -1 and the freeing loop still reads nodes[-1].
 *
 * @param size  the number of bytes the freed run must hold
 * @param skipA  an owner (texture index) whose blocks must be kept
 * @param skipB  a second owner whose blocks must be kept
 * @param level  the threshold compared with each owner's D_0053AB80 a value to decide whether its block costs anything
 */
// Frees the cheapest run of neighbouring blocks (in address order) that together hold at least 'size' bytes;
// locked blocks and blocks of the two given owners break a run, owners above 'level' cost size*weight.
// The two 'skip kept blocks' loops are test-first (if + do/while): the plain while form compiles to an
// unrotated loop (92.15%).
extern "C" void func_003669F0(sRailNodeMan* self, int size, int skipA, int skipB, int level)
{
    int freeList[292];
    int bestIdx = -1;
    int best = 0x7FFFFFFF;
    int fl;
    int Start = self->headsB[17].next;
    sRailNode* StartPtr = &self->nodes[Start];
    if (Start >= 0 && ((fl = StartPtr->field_0x0) & 1)) {
        do {
            if (((fl >> 1) & 1) == 0 && StartPtr->field_0x8 != skipA && StartPtr->field_0x8 != skipB) {
                break;
            }
            Start = StartPtr->next;
            StartPtr = &self->nodes[Start];
        } while (Start >= 0 && ((fl = StartPtr->field_0x0) & 1));
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
            sRailPair* pr = &((sRailPair*)D_0053AB80)[StartPtr->field_0x8];
            sum -= StartPtr->size;
            if ((StartPtr->field_0x0 & 1) && level < pr->a) {
                cost -= StartPtr->size * pr->b;
            }
            Start = StartPtr->next;
            StartPtr = &self->nodes[Start];
        } else {
            if ((EndPtr->field_0x0 & 1) && (((EndPtr->field_0x0 >> 1) & 1) || EndPtr->field_0x8 == skipA || EndPtr->field_0x8 == skipB)) {
                Start = EndPtr->next;
                StartPtr = &self->nodes[Start];
                if (Start >= 0 && ((fl = StartPtr->field_0x0) & 1)) {
                    do {
                        if (((fl >> 1) & 1) == 0 && StartPtr->field_0x8 != skipA && StartPtr->field_0x8 != skipB) {
                            break;
                        }
                        Start = StartPtr->next;
                        StartPtr = &self->nodes[Start];
                    } while (Start >= 0 && ((fl = StartPtr->field_0x0) & 1));
                }
                End = Start;
                EndPtr = StartPtr;
                sum = 0;
                cost = 0;
            } else {
                sRailPair* pr = &((sRailPair*)D_0053AB80)[EndPtr->field_0x8];
                sum += EndPtr->size;
                if ((EndPtr->field_0x0 & 1) && level < pr->a) {
                    cost += EndPtr->size * pr->b;
                }
                End = EndPtr->next;
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
        if (self->nodes[i].field_0x0 & 1) {
            freeList[n++] = i;
        }
        got += self->nodes[i].size;
        i = self->nodes[i].next;
    }
    for (int k = 0; k < n; k++) {
        func_003663D8(self, freeList[k]);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", func_00366CE0);
#ifdef SKIP_ASM
extern "C" void func_003662D0(sRailNodeMan* self, int idx, int size);
extern "C" int func_003668F8(sRailNodeMan* self, int size, int skipA, int skipB, int level);
extern "C" void func_003669F0(sRailNodeMan* self, int size, int skipA, int skipB, int level);

/**
 * Allocates a block of at least @p size bytes from a GS memory block heap for @p owner and returns its node index.
 *
 * When @p align is set the size is rounded up to a multiple of 0x2000 (one GS page); sizes below 0x100 become 0x100. It
 * loops until a block is found: func_003667F8 looks for a free block of that size in the size-class lists, and on
 * success func_003662D0 splits it and marks it used and the node's owner field (+0x8) is set to @p owner. Otherwise it
 * evicts other textures' blocks, first the cheapest single block (func_003668F8), then, if that freed nothing, the
 * cheapest run of neighbours (func_003669F0); blocks of @p owner and @p skipB are never evicted. There is no failure
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
extern "C" int func_00366CE0(sRailNodeMan* self, int size, int owner, int skipB, int level, int align)
{
    int idx;
    if (align) {
        size += -size & 0x1FFF;
    }
    if (size < 0x100) {
        size = 0x100;
    }
    for (;;) {
        idx = func_003667F8(self, size);
        if (idx >= 0) {
            func_003662D0(self, idx, size);
            self->nodes[idx].field_0x8 = owner;
            return idx;
        }
        if (func_003668F8(self, size, owner, skipB, level) == 0) {
            func_003669F0(self, size, owner, skipB, level);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366DD8);
#ifdef SKIP_ASM
struct sRailLinkDD8 {
    int flags;      // 0x0
    int f4;
    int owner;      // 0x8 (the owning texture index, func_00366CE0 stores it)
    int fC;
    int next;       // 0x10
    int f14;
    int f18;
};

/**
 * Returns the index of the first used block owned by @p key in a GS memory block heap, or -1 if there is none.
 *
 * Walks the address-ordered block list (head at +0x23C0, nodes at +0x1FF0, link at +0x10) and matches a block whose
 * flags (bit 0, used) are set and whose owner word (+0x8, where func_00366CE0 stores the owning texture index)
 * equals @p key.
 *
 * @param key  the owner (texture index) to look for
 * @return the node index of that owner's first block in address order, or -1
 */
extern "C" int func_00366DD8(void* self, int key)
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
INCLUDE_ASM("object/railmodifier", func_00366E30);
#ifdef SKIP_ASM
extern "C" void func_003663D8(void* self, int id);

/**
 * Frees the GS memory blocks of texture @p i: its data block (node index at record +0x30) and its CLUT block (+0x34).
 *
 * The texture record is entry @p i of the record table whose pointer the heap keeps at +0x23E8. Each index that is not
 * negative is passed to func_003663D8, which releases the block and resets the record's address and node fields to -1.
 * func_00369098 calls it on the cTextureMan heap at +0x1F60 or, for format 9, the one at +0x4350.
 *
 * @param i  the texture index (slot in the texture record table)
 */
extern "C" void func_00366E30(void* self, int i)
{
    char* entry = (*(char***)((char*)self + 0x23E8))[i];
    if (*(int*)(entry + 0x30) >= 0) {
        func_003663D8(self, *(int*)(entry + 0x30));
    }
    if (*(int*)(entry + 0x34) >= 0) {
        func_003663D8(self, *(int*)(entry + 0x34));
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366E90);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" void func_00390198(void* self, int w, int h, void* buf, char* src);
extern char str_TempTex[];

struct sRmTex_366E90 {
    char pad_0x0[0xC];
    int state;          // 0xC
    int size;           // 0x10
    char pad_0x14[0x4];
    int bpp;            // 0x18
    char* data;         // 0x1C
    char pad_0x20[0x4];
    int mipCount;           // 0x24
    char pad_0x28[0x10];
    unsigned long tex0; // 0x38
};

/**
 * Converts the pixel data of every mip level of @p tex in place with func_00390198, then sets its format code to 10.
 *
 * Allocates a temporary buffer of the texture's size (+0x10) with tag "TempTex" and the manager's memory type (+0x1F58)
 * XOR 0x100. For each level 0 to mipCount (+0x24) it takes the width and height from the TEX0 register value at +0x38
 * (TW, TH), copies w * h * bpp / 8 bytes (at least 0x100) of the level to the buffer and lets func_00390198 write the
 * re-laid-out level back over the source, then halves both sizes. Frees the buffer and stores 10 in the format code at
 * +0xC (the caller only converts format 9 textures).
 *
 * @param tex  the texture record to convert
 */
extern "C" void func_00366E90(char* self, sRmTex_366E90* tex)
{
    int n = 0;
    char* buf = new (str_TempTex, *(int*)(self + 0x1F58) ^ 0x100, 0) char[tex->size];
    int tw = (tex->tex0 >> 26) & 0xF;
    int th = (tex->tex0 >> 30) & 0xF;
    int bpp = tex->bpp;
    int w = 1 << tw;
    int height = 1 << th;
    char* src = tex->data;
    if (tex->mipCount >= 0) {
        do {
            n++;
            int bytes = w * height * bpp / 8;
            if (bytes <= 0xFF) {
                bytes = 0x100;
            }
            MEM_copy(buf, src, bytes);
            func_00390198(self, w, height, buf, src);
            src += bytes;
            w >>= 1;
            height >>= 1;
        } while (n <= tex->mipCount);
    }
    if (buf != 0) {
        cMemMan_free(buf);
    }
    tex->state = 10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366FE0__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the memory type (+0x1F58) the texture manager uses for its temporary allocations.
 *
 * @param val  the new memory type (cPS2GraphicsMan_InitTextures passes its memtype)
 */
void func_00366FE0(void* self, int val)
{
    *(int*)((char*)self + 0x1F58) = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", func_00366FE8);
#ifdef SKIP_ASM
extern "C" void func_003670E0(void* self);
extern "C" void func_00365E40(void* list, int a, int size, void* pool);

/**
 * Initialises the texture manager: clears its tables and sets up both GS memory block heaps.
 *
 * Zeroes the memory type (+0x1F58), resets the texture record table and free id list with func_003670E0, marks no
 * texture bound to either context (+0x1F4C, +0x1F50) and no CLUT loaded (+0x1F54) with -1, then func_00365E40 sets up
 * the heap at +0x1F60 over 0x400000 bytes (the 4 MB of GS memory) and the one at +0x4350 over 0x1000 bytes, both
 * pointing at the record table at +0x8. cPS2GraphicsMan_Init calls it right after allocating the manager.
 */
extern "C" void func_00366FE8(void* self)
{
    *(int*)((char*)self + 0x1F58) = 0;
    func_003670E0(self);
    int* bound = (int*)((char*)self + 0x1F4C);
    for (int i = 1; i >= 0; i--) {
        bound[i] = -1;
    }
    *(int*)((char*)self + 0x1F54) = -1;
    func_00365E40((char*)self + 0x1F60, 0, 0x400000, (char*)self + 0x8);
    func_00365E40((char*)self + 0x4350, 0, 0x1000, (char*)self + 0x8);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", func_00367078);
#ifdef SKIP_ASM
extern "C" void func_00366618(void* list);

/**
 * Frees every block in both GS memory block heaps of the texture manager and forgets the bound textures.
 *
 * Calls func_00366618 (which clears each block's locked bit and frees it) on the heap at +0x4350 and then on the one at
 * +0x1F60, then sets the bound texture of both contexts (+0x1F4C, +0x1F50) and the loaded CLUT texture (+0x1F54) to -1.
 * func_0037BD98 calls it before reserving the frame and depth buffers.
 */
extern "C" void func_00367078(void* self)
{
    func_00366618((char*)self + 0x4350);
    func_00366618((char*)self + 0x1F60);
    int* bound = (int*)((char*)self + 0x1F4C);
    for (int i = 1; i >= 0; i--) {
        bound[i] = -1;
    }
    *(int*)((char*)self + 0x1F54) = -1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003670E0);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Resets the texture manager's record table and its free list of non-bound texture ids.
 *
 * Zeroes the two words at +0x0 and +0x4 (the latter counts allocated ids) and the 2000-entry record table at +0x8 (8000
 * bytes), then points the free list head (+0x1F48) at slot 1500 (+0x1778) and makes each of slots 1500 to 1998 point at
 * the next one; slot 1999 stays 0 and ends the list. cPSPGraphicsMan_NewNonBindTexID pops ids from this list.
 */
extern "C" void func_003670E0(void* self)
{
    int i;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    MEM_fill((char*)self + 0x8, 0, 8000);
    *(void***)((char*)self + 0x1F48) = (void**)((char*)self + 0x1778);
    for (i = 0; i < 499; i++) {
        void** list = *(void***)((char*)self + 0x1F48);
        list[i] = &list[i + 1];
    }
}
#endif


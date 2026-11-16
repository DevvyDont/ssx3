#include "common.h"
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

extern char D_0048EA60[];
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
    t4Matrix* m;
    if (self->idx < 0) {
        t4Matrix* p = &self->mat;
        vu0CopyMat_C0E8(p, &model->mat);
        m = p;
    } else {
        float s = model->scale;
        t4Vector scl;
        scl.x = s;
        scl.y = s;
        scl.z = s;
        scl.w = 1.0f;
        t4Matrix* p = &self->mat;
        sLink_C0E8* l = (sLink_C0E8*)(self->idx * 16 + (int)links);
        vu0CopyMat_C0E8(p, l->mat);
        m = p;
        {
        t4Vector rr;
        t4Vector r3 = self->mat.row[3];
        rr.x = r3.x * scl.x;
        rr.y = r3.y * scl.y;
        rr.z = r3.z * scl.z;
        rr.w = r3.w * scl.w;
        self->mat.row[3] = rr;
        int i = l->next;
        while (i != -1) {
            sLink_C0E8* l2 = (sLink_C0E8*)(i * 16 + (int)links);
            t4Matrix* mm = m;
            t4Matrix t2;
            t4Matrix a;
            t4Vector res;
            t4Vector r3b;
            t4Matrix* lm = l2->mat;
            res = lm->row[0];
            a.row[0] = res;
            res = lm->row[1];
            a.row[1] = res;
            res = lm->row[2];
            a.row[2] = res;
            r3b = lm->row[3];
            res.x = r3b.x * scl.x;
            res.y = r3b.y * scl.y;
            res.z = r3b.z * scl.z;
            res.w = r3b.w * scl.w;
            a.row[3] = res;
            t4Matrix t;
            t4Matrix* pt2 = &t2;
            t4Matrix* pt = &t;
            vu0MulMat_C0E8(pt, &a, mm);
            vu0CopyMat_C0E8(pt2, pt);
            vu0CopyMat_C0E8(mm, pt2);
            i = l2->next;
        }
        }
        t4Matrix t4;
        t4Matrix t3;
        t4Matrix* pt3 = &t3;
        t4Matrix* pt4 = &t4;
        vu0MulMat_C0E8(pt3, &self->model->mat, m);
        vu0CopyMat_C0E8(pt4, &t3);
        vu0CopyMat_C0E8(m, pt4);
    }
    float f;
    f = self->mat.row[0].y;
    self->mat.row[0].y = self->mat.row[1].x;
    self->mat.row[1].x = f;
    f = self->mat.row[0].z;
    self->mat.row[0].z = self->mat.row[2].x;
    self->mat.row[2].x = f;
    f = self->mat.row[1].z;
    self->mat.row[1].z = self->mat.row[2].y;
    self->mat.row[2].y = f;
    t4Vector v = vu0Scale(row_C0E8(&self->mat, 3), -1.0f);
    v.w = 0.0f;
    v = vu0MtxMul_C0E8(m, v);
    v.w = 1.0f;
    self->mat.row[3] = v;
    void* n = cMemMan_alloc(0x10, D_0048EA60, 0x20000000, 0);
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

struct sObj0035C4E0 {
    char pad_0x00[0x10];
    sBox0035C4E0 box;
    char pad_0x30[0x14];
    int id;
};

extern "C" void cRailModifier_updateBoundBox(void* self);
extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, int id, sBox0035C4E0* box, sBox0035C4E0* old);

extern "C" void cRailModifier_update(cRailModifier* self)
{
    sBox0035C4E0* box = &(*(sBox0035C4E0*)&self->BoundBox);
    sBox0035C4E0 old = (*(sBox0035C4E0*)&self->BoundBox);
    cRailModifier_updateBoundBox(((sObj0035C4E0*)self));
    nash_Nash_movenode(cObjectInterface_getNash(), 2, (int)self->Node, box, &old);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRailModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream0035C540 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual void v03(void* buf, int size);
    virtual void v04(void* buf, int size);
    virtual void v05(void* p);
};

extern "C" void cRailModifier_writeToReplayFrame(cRailModifier* self, cStream0035C540* stream)
{
    stream->v01((char*)self + 0x10, 0x30);
    stream->v05(self->Instance);
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

extern "C" void func_0034FED8(void* model, int bone, sRmMat44C5A0* out);

extern "C" void cRailModifier_getInvMat(cRailModifier* self, sRmMat44C5A0* m)
{
    func_0034FED8(self->Instance, self->Mesh, m);
    sRmMat44C5A0* src = (sRmMat44C5A0*)((char*)self + 0x50);
    sRmMat44C5A0 t2;
    sRmMat44C5A0 t;
    vu0MulMatC5A0(&t, m, src);
    vu0CopyMatC5A0(&t2, &t);
    vu0CopyMatC5A0(m, &t2);
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

struct sRmObj_C698 {
    char pad_0x00[0x30];
    sRmRef_C698 spline;     // 0x30
    int bone;               // 0x34
    char pad_0x38[0x8];
    void* model;            // 0x40
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

extern "C" sRmWorld_C698** func_002D1BD8();
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

extern "C" void cRailModifier_splineIntersect(cRailModifier* self, void* box, void* box2, const t4Vector* point, unsigned int mask,
                              int* found, float* bestDist, sRmHit_C698* out)
{
    sRmSpline_C698* spline = (sRmSpline_C698*)(*(sRmRef_C698*)&self->SplineId).get(*func_002D1BD8());
    if ((spline->mask & mask) == 0) {
        return;
    }
    sRmMat44 mat;
    cRailModifier_getInvMat_C698(((sRmObj_C698*)self), &mat);
    sRmSeg_C698* seg = spline->segs;
    for (int i = 0; i < spline->numSegs; i++, seg = seg->next) {
        float a = 0.0f;
        float b = 1.0f;
        t4Vector pos;
        t4Vector next;
        t4Vector dir;
        sRmMat44 c;
        {
            sRmMat44 t;
            vu0MulMat_C698(&t, &mat, &seg->coef);
            vu0CopyMat_C698(&c, &t);
        }
        pos = evalSeg_C698(&c, a);
        float best = vu0Length_C698(vu0Sub_C698(*point, pos));
        int bestIdx = 0;
        int k;
        for (k = 1; k < 5; k++) {
            next = evalSeg_C698(&c, k * 0.25f);
            dir = vu0Sub_C698(next, pos);
            float u = vu0Dot_C698(vu0Sub_C698(*point, pos), dir) / vu0Dot_C698(dir, dir);
            dir = vu0Add_C698(pos, vu0Scale(dir, clamp_C698(u, 0.0f, 1.0f)));
            float d = vu0Length_C698(vu0Sub_C698(*point, dir));
            if (d < best) {
                best = d;
                bestIdx = k;
            }
            pos = next;
        }
        sRmF5_C698 lo = D_0048E7D8;
        sRmF5_C698 hi = D_0048E7F0;
        a = lo.v[bestIdx];
        b = hi.v[bestIdx];
        float tc = a + (b - a) * 0.381966f;
        float td = a + (b - a) * 0.618034f;
        // Golden-section search on [a, b]. f(a) and f(b) are evaluated but never read (the binary keeps both
        // evaluations; their lengths drop out).
        float fa, fb, fc, fd;
        pos = evalSeg_C698(&c, a);
        fa = vu0Length_C698(vu0Sub_C698(pos, *point));
        pos = evalSeg_C698(&c, tc);
        fc = vu0Length_C698(vu0Sub_C698(pos, *point));
        pos = evalSeg_C698(&c, td);
        fd = vu0Length_C698(vu0Sub_C698(pos, *point));
        float t = td;
        float dist = fd;
        pos = evalSeg_C698(&c, b);
        fb = vu0Length_C698(vu0Sub_C698(pos, *point));
        for (k = 0; k < 24; k++) {
            if (fsAbs_C698(b - a) <= 0.0005f) {
                break;
            }
            if (fd < fc) {
                a = tc;
                tc = td;
                td = tc * 0.618034f + b * 0.381966f;
                pos = evalSeg_C698(&c, td);
                float f = vu0Length_C698(vu0Sub_C698(pos, *point));
                fc = fd;
                fd = f;
                t = td;
                dist = fd;
            } else {
                b = td;
                td = tc;
                tc = td * 0.618034f + a * 0.381966f;
                pos = evalSeg_C698(&c, tc);
                float f = vu0Length_C698(vu0Sub_C698(pos, *point));
                fd = fc;
                fc = f;
                t = tc;
                dist = fc;
            }
        }
        if (*found == 0 || dist < *bestDist) {
            *found = 1;
            *bestDist = dist;
            resetHit_C698(out);
            out->pos = pos;
            out->nrm = vu0Normal_C698(evalTan_C698(&c, t));
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
void cRailModifier_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035CFE8__FPv);
#ifdef SKIP_ASM
void* func_0035CFE8(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035CFF0);
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

extern "C" void func_0035CFF0(sBodyCFF0* self, const sVec4CFF0* v)
{
    self->outPos = *v;
    sMat33CFF0 m = self->rot;
    invertCFF0(m);
    self->pos = xformCFF0(m, self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035D288);
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

extern "C" void func_0035D288(sRmBody* self)
{
    self->outDir = vu0ScaleRM(self->dir, self->scale);
    self->outPos = xformRM(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035D340);
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

extern "C" void func_0035D340(sRmBody_D340* b, const t4Vector* a, const t4Vector* c, float dt)
{
    t4Vector tmp;
    ScaleD340(tmp, b->vel, dt);
    AddEqD340(b->pos, tmp);
    {
        t4Vector d;
        QuatDerivD340(d, b->quat, b->angVel);
        ScaleD340(tmp, d, dt);
    }
    b->quat.x += tmp.x;
    b->quat.y += tmp.y;
    b->quat.z += tmp.z;
    b->quat.w += tmp.w;
    NormalizeD340(b->quat);
    ScaleD340(tmp, *a, dt);
    AddEqD340(b->acc0, tmp);
    ScaleD340(tmp, *c, dt);
    AddEqD340(b->acc1, tmp);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035D4A0);
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

extern "C" void func_0035D4A0(sRmRigid_D4A0* b, sRmVec4* point, sRmVec4* n, float e, float mu)
{
    sRmVec4 r = SubD4A0(*point, b->pos);
    sRmVec4 vp = PointVelD4A0(b, *point);
    float d = DotD4A0(vp, *n);
    if (mu != 0.0f) {
        sRmVec4 t = SubD4A0(vp, ScaleD4A0(*n, d));
        float len = LengthD4A0(t);
        if (len > 0.0010000000474974513f) {
            sRmVec4 dir = DivD4A0(t, len);
            float j = -mu * len / (b->invMass + DotD4A0(dir, CrossD4A0(MulID4A0(b, CrossD4A0(r, dir)), r)));
            sRmVec4 imp = ScaleD4A0(dir, j);
            func_0035D908((sRmBody*)b, &r, &imp);
            vp = PointVelD4A0(b, *point);
            d = DotD4A0(vp, *n);
            if (d > 0.0f) {
                return;
            }
        }
    }
    float j = -(e + 1.0f) * d / (b->invMass + DotD4A0(*n, CrossD4A0(MulID4A0(b, CrossD4A0(r, *n)), r)));
    sRmVec4 imp = ScaleD4A0(*n, j);
    func_0035D908((sRmBody*)b, &r, &imp);
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035D908);

//100%
INCLUDE_ASM("object/railmodifier", cRigidBody_writeToReplayFrame);
#ifdef SKIP_ASM
class cRigidBody_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

/**
 * Writes a rigid body's state (its first 0x50 bytes: pose, momentum, inverse mass) to a replay frame.
 *
 * @param obj  the replay frame (On Tour cBxReplayFrame*)
 */
extern "C" void cRigidBody_writeToReplayFrame(cRigidBody* self, cRigidBody_writeToReplayFrame_cObj* obj)
{
    obj->v01(self, 0x50);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035DA38);
#ifdef SKIP_ASM
class func_0035DA38_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_0035DA38(void* self, func_0035DA38_cObj* obj)
{
    obj->v02(self, 0x50);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_cRollerModifier);
#ifdef SKIP_ASM
extern void* D_004913F8[];
extern void* cRollerModifier_vtbl[];
void* func_0035CFE8(void* self);
extern "C" void func_0032C508(void* p);
extern "C" void* func_00327CC8(void* self, void* src);
extern "C" void func_0035E248(void* self);

extern "C" void* cRollerModifier_Collision_DA70(void* self, void* a1, void* a2) __asm__("cRollerModifier_Collision");


extern "C" tPose func_0031B748(void* m);
extern "C" tPose func_0035E770_DA70(void* self) __asm__("func_0035E770");

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
    float* timing = (float*)(((char*)self) + 0x4);
    *(void***)self = D_004913F8;
    func_0035CFE8(((char*)self) + 0x30);
    *(void***)self = cRollerModifier_vtbl;
    func_0032C508(((char*)self) + 0xE0);
    *(int*)&self->Life = 0;
    self->m_pInstance = (tInstanceRecord*)(*(int*)(ii + 0x50));
    func_00327CC8(((char*)self) + 0xE0, *(void**)(ii + 0x48));
    self->mMass = *(float*)(rdata + 0x4);
    char* t = (char*)self->SphTree.treeInfo;
    self->mElast = *(float*)(rdata + 0x8);
    self->InvM = 1.0f / timing[1];
    sRmVec4 v;
    v.x = *(float*)(t + 0x2C);
    v.y = *(float*)(t + 0x30);
    v.z = *(float*)(t + 0x34);
    v.w = 0.0f;
    *(sRmVec4*)&self->COMOffset = v;
    *(sRmMat33*)&self->InvBodyMOI[0] = *(sRmMat33*)(t + 0x5C);
    ((sRmMat33*)(((char*)self) + 0x280))->m[0][0] *= 0.5f;
    ((sRmMat33*)(((char*)self) + 0x280))->m[0][1] *= 0.5f;
    ((sRmMat33*)(((char*)self) + 0x280))->m[0][2] *= 0.5f;
    ((sRmMat33*)(((char*)self) + 0x280))->m[1][0] *= 0.5f;
    ((sRmMat33*)(((char*)self) + 0x280))->m[1][1] *= 0.5f;
    ((sRmMat33*)(((char*)self) + 0x280))->m[1][2] *= 0.5f;
    ((sRmMat33*)(((char*)self) + 0x280))->m[2][0] *= 0.5f;
    ((sRmMat33*)(((char*)self) + 0x280))->m[2][1] *= 0.5f;
    ((sRmMat33*)(((char*)self) + 0x280))->m[2][2] *= 0.5f;
    tPose x = func_0031B748((char*)self->m_pInstance + 0x10);
    {
        sRmVec4 r1;
        QuatRotTo_DA70(r1, (*(sRmVec4*)&x.Q), *(sRmVec4*)&self->COMOffset);
        v = r1;
        sRmVec4 b;
        sRmVec4 r2;
        QuatRotTo_DA70(r2, (*(sRmVec4*)&x.Q), v);
        b = r2;
        AddTo_DA70((*(sRmVec4*)&x.P), b);
    }
    *(&self->Pose) = x;
    tPose y = func_0035E770_DA70(((char*)self));
    QuatPosToMtx_DA70(((char*)self) + 0x240, (*(sRmVec4*)&y.Q), (*(sRmVec4*)&y.P));
    func_0035E248(((char*)self));
    func_0035D288((sRmBody*)(((char*)self) + 0x30));
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
    void setW(const sRmV4_35DDE8& w) __asm__("func_0035CFF0");
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
INCLUDE_ASM("object/railmodifier", func_0035DF70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004913F8[];
extern void* cRollerModifier_vtbl[];
void* func_0035CFE8(void* self);
extern "C" void func_0032C508(void* self);
extern void* func_002D1BD8_DF70() __asm__("func_002D1BD8");
void* func_003A6CD8(void* a, void* b);
extern "C" void* func_003A6CE8(void* a, int b);
extern "C" void func_0032C648(void* self, void* m, float scale);
extern "C" void func_0035E248(void* self);



class cStreamDF70 {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual int v03();
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

extern "C" sRailDF70* func_0035DF70(sRailDF70* self, cStreamDF70* stream)
{
    self->vt = D_004913F8;
    func_0035CFE8(self->body);
    self->vt = cRollerModifier_vtbl;
    func_0032C508(self->mover);
    func_0035DA38(self->body, (func_0035DA38_cObj*)stream);
    stream->v02(self->data, 0x28);
    self->id = stream->v03();
    void* r = func_003A6CE8(func_003A6CD8(func_002D1BD8_DF70(), (void*)self->id), 0);
    func_00327CC8(self->mover, r);
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
    tPose x = func_0035E770_DA70(self);
    QuatPosToMtx_DA70(&self->xf, (*(sRmVec4*)&x.Q), (*(sRmVec4*)&x.P));
    func_0032C648(self->mover, &self->xf, 1.0f);
    func_0035E248(self);
    func_0035D288((sRmBody*)self->body);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", cRollerModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cRollerModifier_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual void v03();
    virtual void v04();
    virtual void v05(void* ptr);
};

extern "C" void cRollerModifier_writeToReplayFrame(void* self, cRollerModifier_writeToReplayFrame_cObj* obj)
{
    cRigidBody_writeToReplayFrame((cRigidBody*)((char*)self + 0x30), (cRigidBody_writeToReplayFrame_cObj*)obj);
    obj->v01((char*)self + 0x4, 0x28);
    obj->v05(*(void**)((char*)self + 0xD0));
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035E248);
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

// World-space inertia: rotT * inertia * rot, with rot from the body quaternion (self + 0x40).
extern "C" void func_0035E248(void* self)
{
    sM44_E248 m;
    QuatToMtx_E248(m, *(sRmVec4*)((char*)self + 0x40));
    sRmMat33 rot;
    sRmMat33 rotT;
    rotT = Get33_E248(m);
    rot = rotT;
    Transpose33_E248(rotT);
    *(sRmMat33*)((char*)self + 0x80) = Mul33_E248(MulI_E248(rot, (sRmInertiaOwner_E248*)self), rotT);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035E770);
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

extern "C" sRmXform func_0035E770(void* self)
{
    sRmXform r = *(sRmXform*)((char*)self + 0x30);
    sRmVec4 c;
    sRmVec4 b;
    sRmVec4 a;
    sRmVec4 e;
    sRmVec4 d;
    vu0QuatRotToRM(a, *(sRmVec4*)((char*)self + 0x40), *(sRmVec4*)((char*)self + 0x2B0));
    b = a;
    vu0ScaleToRM(a, b, -1.0f);
    c = a;
    vu0QuatRotToRM(d, r.quat, c);
    e = d;
    vu0AddToRM(r.pos, e);
    return r;
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
INCLUDE_ASM("object/railmodifier", func_0035ED90);
#ifdef SKIP_ASM
extern "C" void func_0032C630(void* p, const sRmVec4* d);

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

extern "C" void func_0035ED90(sRmBody* self, const sRmVec4* b)
{
    vu0AddED90(self->pos, *b);
    func_0032C630((char*)self + 0xE0, b);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cRollerModifier_collide);
#ifdef SKIP_ASM
extern "C" void* cWorldSphTree_cWorldSphTree(void* self, void* a1, int a2);
extern "C" void cWorldSphTree__cWorldSphTree(void* self, int flags);
extern "C" void* cObjectInterface_getNash();
extern "C" float func_0032C5A8(void* self);
extern void func_0035D4A0_EDC8(void* self, void* hit, void* normal, float a, float b) __asm__("func_0035D4A0");

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

extern "C" float func_00336850_EDC8(void* world, void* tree, sHitEDC8* out, void* a3) __asm__("func_00336850");

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
    (*(sRmBody*)&rollerModifier->Pose).dir = toDirEDC8(&(*(sRmBody*)&rollerModifier->Pose), v);
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
    cWorldSphTree_cWorldSphTree(tree, (char*)self + 0xE0, 1);
    float zero = 0.0f;
    float d = func_00336850_EDC8(cObjectInterface_getNash(), tree, &hit, (char*)self + 0x2C0);
    d -= func_0032C5A8((char*)self + 0xE0);
    if (zero < d) {
        sRmVec4 push = vu0ScaleEDC8(hit.normal, d + 2.0f);
        func_0035ED90((sRmBody*)self, &push);
        float k = 0.975f;
        setPosEDC8((self), vu0ScaleEDC8((*(sRmVec4*)&self->L), k));
        setOutDirEDC8((self), vu0ScaleEDC8((*(sRmVec4*)&self->V), k));
        float dp = vu0DotEDC8(vu0AddEDC8((*(sRmVec4*)&self->V), vu0CrossEDC8((*(sRmVec4*)&self->W), vu0SubEDC8(hit.pos, *(sRmVec4*)&(*(sRmBody*)&self->Pose)))), hit.normal);
        if (dp < zero) {
            float lim = self->StopContactSpeed;
            float len = vu0LengthEDC8((*(sRmVec4*)&self->V));
            float a = self->ContactElastic;
            float c = self->ContactFriction;
            if (len < lim)
                a = zero;
            func_0035D4A0_EDC8(&(*(sRmBody*)&self->Pose), &hit, &hit.normal, a, c);
            self->HitFrequency += 0.016666668f;
            if (5.0f < self->Life)
                self->Life = 10.0f;
        }
    }
    cWorldSphTree__cWorldSphTree(tree, 2);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035F0B8);
#ifdef SKIP_ASM
extern char* D_004A5B64;
int BXrand();
extern "C" void func_0035F218(cTexFlipModifier* self);

struct sRailTimer {
    int f0;
    int f4;
    float f8;
    float fC;
    int f10;
    int f14;
    void* f18;
    int f1C;
    int f20;
};

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ftrunc_0035F0B8(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

extern "C" sRailTimer* func_0035F0B8(cTexFlipModifier* self, void* def, void* owner)
{
    char* d = (char*)def;
    float one = 1.0f;
    self->Instance = (tInstanceRecord*)owner;
    self->TexIndex = 0;
    self->NumMaterials = 0;
    self->NumTexFlipMaterials = 0;
    self->Type = *(int*)(d + 0x4);
    float rate = *(float*)(d + 0x10) * (one / (float)*(int*)(D_004A5B64 + 0x10));
    self->Speed = rate;
    self->Dir = *(int*)(d + 0xC);
    self->BaseSpeed = rate;
    *(int*)&self->Time = 0;
    int n = *(int*)(d + 0x8);
    if ((float)n < *(float*)(int*)&self->Time) {
        union { int i; float f; } u;
        u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
        float x = (u.f - one) * 8.0f;
        float t = ftrunc_0035F0B8(x);
        if (t < x) {
            t += one;
        }
        self->TexIndex = (int)t;
    } else {
        self->TexIndex = n;
    }
    func_0035F218(self);
    return ((sRailTimer*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035F1A8);
#ifdef SKIP_ASM
class func_0035F1A8_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual void* v03();
};

extern "C" void func_0035F218(cTexFlipModifier* self);

extern "C" void* func_0035F1A8(cTexFlipModifier* self, func_0035F1A8_cObj* obj)
{
    obj->v02(self, 0x18);
    self->Instance = (tInstanceRecord*)obj->v03();
    func_0035F218(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035F218);
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

extern void sel_F218(sRailSelF218* self, int i) __asm__("func_0035F378");
extern void ms_F218(void* p, int v, int n) __asm__("MEM_fill");

extern "C" void func_0035F218(cTexFlipModifier* selfv)
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
    ms_F218(&self->f24, 0, 0xC4);
    self->f24 = self->count;
    for (i = 0; i < self->count; i++) {
        sRailItemF218* item = *(sRailItemF218**)(*(char**)(self->owner + 0x94) + (i << 2) + 4);
        if (item->data != 0) {
            self->src[i] = &self->out[i];
            self->src[i]->fC = item->fC;
            // writes slots 1..3 of out[0] for every entry, as func_003443A8 does (bug in the original)
            short* p = (short*)self->out;
            for (int j = 1; j < 4; j++) {
                p[j] = -1;
            }
            self->out[i].value = *(unsigned short*)(item->data + (self->index << 2) + 4);
        } else {
            self->src[i] = (sRailOutF218*)item;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035F378);
#ifdef SKIP_ASM
struct sRailModSelect;
extern "C" void func_0035F3A8(sRailModSelect* self);

extern "C" void func_0035F378(sRailModSelect* self, int i)
{
    int n = *(int*)((char*)self + 0x20);
    if (i >= n) {
        i = n - 1;
    }
    *(int*)((char*)self + 0x14) = i;
    func_0035F3A8(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035F3A8);
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

extern "C" void func_0035F3A8(sRailModSelect* self)
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
// Advances the rail timer; each whole step moves to the next/previous frame (mode 0), or alternates between a
// fixed 10-per-second step (mode 1) and a random 25..100% rate (mode 2).
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
        self->Speed = 10.0f / (float)*(int*)(D_004A5B64 + 0x10);
        self->Type = 2;
        if (++self->TexIndex >= self->NumTexFlipMaterials) {
            self->TexIndex = 0;
        }
    } else if (self->Type == 2) {
        union { int i; float f; } u;
        float base = 0.25f;
        u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
        self->Type = 1;
        self->Speed = self->BaseSpeed * ((u.f - one) * 0.75f + base);
        if (++self->TexIndex >= self->NumTexFlipMaterials) {
            self->TexIndex = 0;
        }
    }
    func_0035F378((sRailModSelect*)self, self->TexIndex);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTexFlipModifier_signal);
#ifdef SKIP_ASM
struct sRailModSelect;
extern "C" void func_0035F378(sRailModSelect* self, int i);

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

extern "C" void cTexFlipModifier_signal(cTexFlipModifier* self, int id, float v)
{
    switch (id) {
    case 300:
        self->Speed = v;
        break;
    case 301:
        if (v >= 0.0f && v < 9.0f) {
            float c = fTrunc_F598(v);
            if (c < v) {
                c += 1.0f;
            }
            int n = (int)c;
            self->TexIndex = n;
            func_0035F378((sRailModSelect*)self, n);
        }
        break;
    case 302:
        self->TexIndex++;
        if (self->TexIndex >= self->NumTexFlipMaterials) {
            self->TexIndex = 1;
        }
        func_0035F378((sRailModSelect*)self, self->TexIndex);
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
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual void v03(void* buf, int size);
    virtual void v04(void* buf, int size);
    virtual void v05(void* p);
};

extern "C" void cTexFlipModifier_writeToReplayFrame(cTexFlipModifier* self, cStream0035F688* stream)
{
    stream->v01((char*)self + 0x0, 0x18);
    stream->v05(self->Instance);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035F6E8);
#ifdef SKIP_ASM
extern char* D_004A5B64;

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

extern "C" void* func_0035F6E8(sRmState_F6E8* self, sRmDesc_F6E8* d)
{
    self->f40 = 1;
    self->f0 = d->f4;
    self->f30 = d->f8;
    self->f34 = d->fC;
    self->f8 = d->f18;
    self->fC = d->f1C;
    self->f4 = 0;
    self->f10 = 0;
    self->f14 = d->f20 * (1.0f / (float)*(int*)(D_004A5B64 + 0x10));
    self->q = sRmQuad_F6E8(d->f24, d->f28, d->f2C, 0.0f);
    self->f38 = d->f10;
    self->f3C = d->f14;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035F788);
#ifdef SKIP_ASM
struct sRailSerVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_0035F788(cUVScrollModifier* self, void* stream)
{
    sRailSerVEntry* vt = *(sRailSerVEntry**)stream;
    vt[2].fn((char*)stream + vt[2].delta, self, 0x50);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_update);
#ifdef SKIP_ASM
extern char* D_004A5B64;

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

extern "C" void cUVScrollModifier_update(cUVScrollModifier* self)
{
    if (self->TimeOn > 0.0f || self->TimeOff > 0.0f) {
        self->Time += 1.0f / (float)*(int*)(D_004A5B64 + 0x10);
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
                    (*(sV2_F7D0*)&self->UVScroll[0]) = -(*(sV2_F7D0*)&self->UVScroll[0]);
                }
            }
            if (self->On) {
                if (self->Type == 6) {
                    if (self->UVScroll[0] < 0.0f) {
                        float t = self->Time;
                        if (t < self->TimeOn * 0.5f) {
                            sV2_F7D0 m = (*(sV2_F7D0*)&self->UVScroll[0]) * t * (1.0f / self->TimeOn);
                            self->UV[0] += m.x;
                            self->UV[1] += m.y;
                        } else {
                            sV2_F7D0 m = (*(sV2_F7D0*)&self->UVScroll[0]) * (self->TimeOn - t) * (1.0f / self->TimeOn);
                            self->UV[0] += m.x;
                            self->UV[1] += m.y;
                        }
                    } else {
                        float u = self->TimeOn - self->Time;
                        if (u < self->TimeOn * 0.5f) {
                            sV2_F7D0 m = (*(sV2_F7D0*)&self->UVScroll[0]) * u * (1.0f / self->TimeOn);
                            self->UV[0] += m.x;
                            self->UV[1] += m.y;
                        } else {
                            sV2_F7D0 m = (*(sV2_F7D0*)&self->UVScroll[0]) * (self->TimeOn - u) * (1.0f / self->TimeOn);
                            self->UV[0] += m.x;
                            self->UV[1] += m.y;
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
extern "C" void cUVScrollModifier_signal(cUVScrollModifier* self, int id, float v)
{
    if (id == 200 || id == 201) {
        if (v >= -1.0f && v <= 1.0f) {
            if (id == 201) {
                self->UVScroll[0] = v;
            } else {
                self->UVScroll[1] = v;
            }
        }
    } else if (id == 202 || id == 203) {
        if (v >= 0.0f && v <= 60.0f) {
            if (id == 202) {
                self->TimeOn = v;
            } else {
                self->TimeOff = v;
            }
        }
    } else if (id == 204 || id == 205) {
        if (v >= -4.0f && v <= 4.0f) {
            if (id == 204) {
                self->UV[0] = v;
            } else {
                self->UV[1] = v;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_getTextureMat);
#ifdef SKIP_ASM
extern "C" void func_0031BE50(float* s, float* c, float angle);

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

extern "C" sRmMtx_35FC20* cUVScrollModifier_getTextureMat(cUVScrollModifier* self, sRmMtx_35FC20* m)
{
    float zero = 0.0f;
    if (self->Angle != zero) {
        float s;
        float c;
        func_0031BE50(&s, &c, self->Angle);
        float one = 1.0f;
        float t = one - c;
        float tx = t * self->Axis.x;
        float sy = s * self->Axis.y;
        float sz = s * self->Axis.z;
        float sx = s * self->Axis.x;
        float ty = t * self->Axis.y;
        float tz = t * self->Axis.z;
        m->m[0][0] = tx * self->Axis.x + c;
        m->m[1][0] = tx * self->Axis.y + sz;
        m->m[2][0] = tx * self->Axis.z - sy;
        m->m[3][0] = zero;
        m->m[0][1] = ty * self->Axis.x - sz;
        m->m[1][1] = ty * self->Axis.y + c;
        m->m[2][1] = ty * self->Axis.z + sx;
        m->m[3][1] = zero;
        m->m[0][2] = tz * self->Axis.x + sy;
        m->m[1][2] = tz * self->Axis.y - sx;
        m->m[2][2] = tz * self->Axis.z + c;
        m->m[3][2] = zero;
        m->m[0][3] = zero;
        m->m[1][3] = zero;
        m->m[2][3] = zero;
        m->m[3][3] = one;
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
            : "r"(m), "r"(D_004FF1A0_35FC20)
            : "memory");
    }
    m->m[3][0] = self->UV[0];
    m->m[3][1] = self->UV[1];
    return m;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cUVScrollModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cUVScrollModifier_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void cUVScrollModifier_writeToReplayFrame(cUVScrollModifier* self, cUVScrollModifier_writeToReplayFrame_cObj* obj)
{
    obj->v01(self, 0x50);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035FDD0);
#ifdef SKIP_ASM
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
void* func_003549D0(void* self);

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
        func_003549D0(&gBucketMan);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360700);
#ifdef SKIP_ASM
extern void* cObjNode_vtbl[];

extern "C" void* func_00360700(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0xc) = (int)(void*)cObjNode_vtbl;
    *(int*)((char*)self + 0x8) = 0;
    return self;
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
INCLUDE_ASM("object/railmodifier", func_00360740__FPv);
#ifdef SKIP_ASM
int func_00360740(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360748__FPv);
#ifdef SKIP_ASM
int func_00360748(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360750__FPv);
#ifdef SKIP_ASM
int func_00360750(void* self)
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
INCLUDE_ASM("object/railmodifier", func_00360768);
#ifdef SKIP_ASM
extern "C" unsigned int func_00360768()
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
extern "C" int cTypeObjNode_isType(void* self, int type)
{
    return *(short*)((char*)self + 0x10) == type;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003607B0);
#ifdef SKIP_ASM
extern "C" short func_003607B0(void *arg0) {
    return (*(short *)((char*)(arg0) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003607B8);
#ifdef SKIP_ASM
extern "C" void func_003607B8(void *arg0) {
    (*(unsigned short *)((char*)(arg0) + (0x12))) = (unsigned short) ((*(unsigned short *)((char*)(arg0) + (0x12))) | 1);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003607C8);
#ifdef SKIP_ASM
extern "C" int func_003607C8(void *arg0) {
    return (*(unsigned short *)((char*)(arg0) + (0x12))) & 1;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003607D8);
#ifdef SKIP_ASM
extern "C" void func_003607D8(void *arg0) {
    (*(unsigned short *)((char*)(arg0) + (0x12))) = (unsigned short) ((*(unsigned short *)((char*)(arg0) + (0x12))) & 0xFFFE);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003607E8);
#ifdef SKIP_ASM
extern int D_0044AFF0[];

extern "C" int func_003607E8(int idx)
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
extern "C" int cSortObjNode_compare(void* self, void* obj)
{
    sRailVEntryU800* vt = *(sRailVEntryU800**)((char*)obj + 0xC);
    return vt[8].fn((char*)obj + vt[8].delta) >= *(unsigned int*)((char*)self + 0x14);
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
extern "C" int cSortObjNode_equal(void* self, void* obj)
{
    sRailVEntryU840* vt = *(sRailVEntryU840**)((char*)obj + 0xC);
    return *(unsigned int*)((char*)self + 0x14) == vt[8].fn((char*)obj + vt[8].delta);
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
int cSortObjNode_getSortIndex(void* self)
{
    return *(int*)((char*)self + 0x14);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360888);
#ifdef SKIP_ASM
extern int D_004A5B78;
extern int D_004A5B7C;

extern "C" void func_00360888(void)
{
    D_004A5B78 = 1;
    D_004A5B7C = 1;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360898);
#ifdef SKIP_ASM
extern int D_004A5B78;

extern "C" int func_00360898(void)
{
    return D_004A5B78++;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003608B0);
#ifdef SKIP_ASM
extern int D_004A5B7C;

extern "C" int func_003608B0(void)
{
    return D_004A5B7C++;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003608C8);
#ifdef SKIP_ASM
extern int D_004A5B78;

extern "C" int func_003608C8(void) {
    return D_004A5B78;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003608D0);
#ifdef SKIP_ASM
extern int D_004A5B7C;

extern "C" int func_003608D0(void) {
    return D_004A5B7C;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003608D8);
#ifdef SKIP_ASM
extern int D_004A5B78;

extern "C" void func_003608D8(int arg0) {
    D_004A5B78 = arg0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003608E0);
#ifdef SKIP_ASM
extern int D_004A5B7C;

extern "C" void func_003608E0(int arg0) {
    D_004A5B7C = arg0;
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
    virtual int v15();
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
    virtual void v50();
};

/**
 * Updates an instance node by calling its virtual updateNode() (vtable slot 15).
 *
 * @return updateNode()'s result (On Tour's update returns nothing)
 */
extern "C" int cInstanceNode_update(cInstanceNode* self)
{
    return ((cRailVirt*)self)->v15();
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
INCLUDE_ASM("object/railmodifier", func_00360920__FPv);
#ifdef SKIP_ASM
unsigned char func_00360920(void* self)
{
    return *(unsigned char*)((char*)*(void**)((char*)self + 0x18) + 0x78);
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
INCLUDE_ASM("object/railmodifier", func_00360968);
#ifdef SKIP_ASM
extern "C" int func_00360968(cInstanceNode* instanceNode) {
    return ((int)instanceNode->Instance);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360970__FPv);
#ifdef SKIP_ASM
void* func_00360970(void* self)
{
    return (char*)*(void**)((char*)self + 0x18) + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360980__FPv);
#ifdef SKIP_ASM
void* func_00360980(void* self)
{
    return (char*)*(void**)((char*)self + 0x18) + 0x10;
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
INCLUDE_ASM("object/railmodifier", func_00360998__FPv);
#ifdef SKIP_ASM
int func_00360998(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609A0__FPv);
#ifdef SKIP_ASM
int func_003609A0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609A8__FPv);
#ifdef SKIP_ASM
int func_003609A8(void* self)
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
INCLUDE_ASM("object/railmodifier", func_00360A18__FPv);
#ifdef SKIP_ASM
int func_00360A18(void* self)
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
void* cDeadNode__cDeadNode(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)cDeadNode_vtbl;
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

extern void* D_00491800[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00360A70__FPv);
#ifdef SKIP_ASM
void* func_00360A70(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00491800;
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A98__FPv);
#ifdef SKIP_ASM
void func_00360A98(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AA0__FPv);
#ifdef SKIP_ASM
void func_00360AA0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AA8__FPv);
#ifdef SKIP_ASM
void func_00360AA8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AB0__FPv);
#ifdef SKIP_ASM
void func_00360AB0(void* self)
{
}
#endif

extern void* D_00491680[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00360AB8__FPv);
#ifdef SKIP_ASM
void* func_00360AB8(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00491680;
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AE0__FPv);
#ifdef SKIP_ASM
void func_00360AE0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AE8__FPv);
#ifdef SKIP_ASM
void func_00360AE8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AF0__FPv);
#ifdef SKIP_ASM
void func_00360AF0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AF8__FPv);
#ifdef SKIP_ASM
void func_00360AF8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier__cTransformModifier);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void cTransformModifier__cTransformModifier(cTransformModifier* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_update__FPv);
#ifdef SKIP_ASM
void cTransformModifier_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B38__FPv);
#ifdef SKIP_ASM
void func_00360B38(void* self)
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
int cTransformModifier_getBoundBox(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_setBoundBox__FPv);
#ifdef SKIP_ASM
void cTransformModifier_setBoundBox(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getRadius__FPv);
#ifdef SKIP_ASM
float cTransformModifier_getRadius(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_setRadius__FPv);
#ifdef SKIP_ASM
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
INCLUDE_ASM("object/railmodifier", func_00360BB8__FPv);
#ifdef SKIP_ASM
int func_00360BB8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cTransformModifier_getRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a transform modifier has no root transform.
 *
 * @return always 0
 */
int cTransformModifier_getRootTransform(void* self)
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
INCLUDE_ASM("object/railmodifier", func_00360BF8__FPv);
#ifdef SKIP_ASM
int func_00360BF8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360C00__FPv);
#ifdef SKIP_ASM
void func_00360C00(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360C08);
#ifdef SKIP_ASM
extern void* cPhysicsModifier_vtbl[];

extern "C" void *func_00360C08(cPhysicsModifier* arg0) {
    ((int*)arg0->mVtbl) = &(*(int*)&cPhysicsModifier_vtbl);
    (arg0->Active) = 1;
    return ((void*)arg0);
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
int cPhysicsModifier_isActive(void* self)
{
    return *(int*)((char*)self + 0x0);
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
extern "C" void func_00345760(void* self);
void operator_delete(int*);
extern void* cEffectLink_vtbl[];

/**
 * Destroys an effect link: restores its vtable at +0x8, unlinks it from its list, then frees it when asked to.
 *
 * func_00345760() joins the link's neighbours and clears its next/prev pointers.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cEffectLink__cEffectLink(cEffectLink* self, int flags)
{
    self->mVtbl = cEffectLink_vtbl;
    func_00345760(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360CE8__FPv);
#ifdef SKIP_ASM
int func_00360CE8(void* self)
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
INCLUDE_ASM("object/railmodifier", func_00360D18);
#ifdef SKIP_ASM
extern int D_004A3FF4;

extern "C" int func_00360D18(void) {
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
INCLUDE_ASM("object/railmodifier", func_00360D30);
#ifdef SKIP_ASM
extern int D_004A3FFC;

extern "C" int func_00360D30(void) {
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
extern "C" void func_0035B6D0(void* self);
void operator_delete(int*);
extern void* cRailLink_vtbl[];

/**
 * Destroys a rail link: restores its vtable at +0x8, unlinks it from its list, then frees it when asked to.
 *
 * func_0035B6D0() joins the link's neighbours and clears its next/prev pointers.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRailLink__cRailLink(cRailLink* self, int flags)
{
    self->mVtbl = cRailLink_vtbl;
    func_0035B6D0(self);
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
INCLUDE_ASM("object/railmodifier", func_00360D98);
#ifdef SKIP_ASM
extern "C" int func_00360D98(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x30)));
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
    ((cRailVirt*)self)->v50();
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
    unsigned short* p = (unsigned short*)((char*)self + 0x12);
    *p &= 0xFFFE;
}
#endif

extern void* D_00490E80[];
extern "C" void* cMoveNode__cMoveNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00360DE8);
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
    virtual int v16();
};

extern "C" int func_00360DE8(cMoveNode* self)
{
    cVirt16** holder = (cVirt16**)self->ModifierBlock;
    if (holder != 0) {
        cVirt16* obj = *holder;
        if (obj != 0) {
            return obj->v16();
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", cMoveNode_getTransformModifier);
#ifdef SKIP_ASM
extern "C" int cMoveNode_getTransformModifier(cMoveNode* self)
{
    int* p = (int*)self->ModifierBlock;
    if (p != 0) {
        return *p;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360E50);
#ifdef SKIP_ASM
extern "C" int func_00360E50(cMoveNode* arg0) {
    int var_2;
    void *temp_3;

    temp_3 = ((void*)arg0->ModifierBlock);
    var_2 = 0;
    if (temp_3 != 0) {
        var_2 = (*(int *)((char*)(temp_3) + (4))) != 0;
    }
    return var_2;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360E70);
#ifdef SKIP_ASM
extern "C" int func_00360E70(cMoveNode* arg0) {
    int var_2;
    void *temp_3;

    temp_3 = ((void*)arg0->ModifierBlock);
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
extern "C" int cMoveNode_getMaterialBlock(cMoveNode* self)
{
    int cond;
    char* p = (char*)self->ModifierBlock;
    cond = 0;
    if (p != 0) {
        cond = *(int*)(p + 8) != 0;
    }
    if (cond != 0) {
        int t = *(int*)(p + 8);
        if (t != 0) {
            return t + 0x24;
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

extern "C" void* cMoveNode_getTextureMat(cMoveNode* self)
{
    char* p = (char*)self->ModifierBlock;
    int cond = 0;
    if (p != 0) {
        cond = *(int*)(p + 4) != 0;
    }
    if (cond != 0) {
        void* q = *(void**)(p + 4);
        if (q != 0) {
            return cUVScrollModifier_getTextureMat_1arg(q);
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360F20);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remTransformModifier(int);

extern "C" void func_00360F20(cMoveNode* arg0) {
    int temp_4;

    temp_4 = ((int)arg0->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remTransformModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360F48);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remUVScrollModifier(int);

extern "C" void func_00360F48(cMoveNode* arg0) {
    int temp_4;

    temp_4 = ((int)arg0->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remUVScrollModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360F70);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remTexFlipModifier(int);

extern "C" void func_00360F70(cMoveNode* arg0) {
    int temp_4;

    temp_4 = ((int)arg0->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remTexFlipModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360F98);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remPhysicsModifier(int);

extern "C" void func_00360F98(cMoveNode* arg0) {
    int temp_4;

    temp_4 = ((int)arg0->ModifierBlock);
    if (temp_4 != 0) {
        tModifierBlock_remPhysicsModifier(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360FC0);
#ifdef SKIP_ASM
extern "C" void func_00353278(int);

extern "C" void func_00360FC0(cMoveNode* arg0) {
    int temp_4;

    temp_4 = ((int)arg0->ModifierBlock);
    if (temp_4 != 0) {
        func_00353278(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00360FE8);
#ifdef SKIP_ASM
extern "C" void func_003531D0(int);

extern "C" void func_00360FE8(cMoveNode* arg0) {
    int temp_4;

    temp_4 = ((int)arg0->ModifierBlock);
    if (temp_4 != 0) {
        func_003531D0(temp_4);
    }
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00361010);
#ifdef SKIP_ASM
extern "C" void func_00353228(int);

extern "C" void func_00361010(cMoveNode* arg0) {
    int temp_4;

    temp_4 = ((int)arg0->ModifierBlock);
    if (temp_4 != 0) {
        func_00353228(temp_4);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361038__FPv);
#ifdef SKIP_ASM
void* func_00361038(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00490E80;
    return cMoveNode__cMoveNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361060__FPv);
#ifdef SKIP_ASM
int func_00361060(void* self)
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
extern "C" float cAnimNode_getAnimFrame(void* self)
{
    float input = *(float*)((char*)self + 0x0);
    return input * 30.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361080__FPv);
#ifdef SKIP_ASM
float func_00361080(void* self)
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
extern "C" int cAnimNode_getAnimMats(void* self)
{
    if (*(unsigned short*)((char*)self + 0x26) & 1) {
        void* sub = (char*)self + 0x14;
        sRailVEntry1098* vt = *(sRailVEntry1098**)((char*)self + 0x20);
        vt[51].fn((char*)sub + vt[51].delta);
    }
    return *(int*)((char*)self + 0x44);
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

struct sElem003610E0 {
    char data[0x40];
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
extern "C" sElem003610E0* cAnimNode_getAnimMat(void* self, int i)
{
    char* obj = (char*)self + 0x14;
    if (*(unsigned short*)((char*)self + 0x26) & 1) {
        sVEntry003610E0* vt = *(sVEntry003610E0**)((char*)self + 0x20);
        vt[51].fn(obj + vt[51].delta);
    }
    return &(*(sElem003610E0**)((char*)self + 0x44))[i];
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361140);
#ifdef SKIP_ASM
extern "C" void func_00361140(void *arg0) {
    (*(float *)((char*)(arg0) + (8))) = 1e10f;
    (*(float *)((char*)(arg0) + (0xC))) = 1e10f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361150__FPv);
#ifdef SKIP_ASM
void* func_00361150(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361158);
#ifdef SKIP_ASM
extern "C" float func_00361158(void* self)
{
    return *(int*)((char*)self + 0x8) == 0 ? *(float*)((char*)self + 0x10) : 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361170);
#ifdef SKIP_ASM
extern "C" int func_00361170(void* self)
{
    int r = -1;
    if (*(float*)((char*)self + 0x10) >= 0.0f) {
        r = 1;
    }
    return r;
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
int cAnimObjectNode_isComplete(void* self)
{
    return *(int*)((char*)self + 0x8);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003611A0);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
extern "C" void func_003513D0(void*);

extern "C" void* func_003611A0(void* self)
{
    *(int*)((char*)self + 0x84) = (int)(void*)cAnimFrame_vtbl;
    func_003513D0(self);
    return self;
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
extern "C" void cAnimFrame__cAnimFrame(void* self, int flags)
{
    *(void***)((char*)self + 0x84) = cAnimFrame_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361208);
#ifdef SKIP_ASM
struct sVec4_361208 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void func_00361208(void* self, sVec4_361208* out)
{
    sVec4_361208 v;
    v.x = -*(float*)((char*)self + 0x50);
    v.y = -*(float*)((char*)self + 0x54);
    v.z = -*(float*)((char*)self + 0x58);
    v.w = 0.0f;
    *out = v;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00361248);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
extern void* cMeshAnimFrame_vtbl[];
extern "C" void func_003513D0(void*);
extern "C" void func_003612C0(void*);

extern "C" void* func_00361248(void* self)
{
    *(int*)((char*)self + 0x84) = (int)(void*)cAnimFrame_vtbl;
    func_003513D0(self);
    *(int*)((char*)self + 0x84) = (int)(void*)cMeshAnimFrame_vtbl;
    func_003612C0(self);
    return self;
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
extern "C" void cMeshAnimFrame__cMeshAnimFrame(void* self, int flags)
{
    *(void***)((char*)self + 0x84) = cAnimFrame_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003612C0);
#ifdef SKIP_ASM
extern char t4mat_Identity[];

// Copy the 4x4 matrix t4mat_Identity into self+0x90.
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
extern "C" void func_003612C0(void* self)
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
        : "r"((char*)self + 0x90), "r"(t4mat_Identity)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003612F8);
#ifdef SKIP_ASM
extern "C" int func_003612F8(int arg0) {
    return arg0 + 0x90;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00361300);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
extern void* D_00490AB0[];
extern "C" void func_003513D0(void*);

extern "C" void* func_00361300(void* self)
{
    *(int*)((char*)self + 0x84) = (int)(void*)cAnimFrame_vtbl;
    func_003513D0(self);
    *(int*)((char*)self + 0x84) = (int)(void*)D_00490AB0;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361340);
#ifdef SKIP_ASM
extern void* cAnimFrame_vtbl[];
void operator_delete(int*);

extern "C" void func_00361340(void* self, int flags)
{
    *(void***)((char*)self + 0x84) = cAnimFrame_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361370);
#ifdef SKIP_ASM
extern "C" void func_00361370(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x40))) = arg1;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00361378);
#ifdef SKIP_ASM
struct sVec4_361378 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void func_00361378(void* self, sVec4_361378* out)
{
    sVec4_361378 v;
    v.x = *(float*)((char*)self + 0x44);
    v.y = *(float*)((char*)self + 0x48);
    v.z = *(float*)((char*)self + 0x4C);
    v.w = 1.0f;
    *out = v;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003613B0);
#ifdef SKIP_ASM
struct sVec3_3613B0 {
    float x, y, z;
};

extern "C" void func_003613B0(void* self, sVec3_3613B0* out)
{
    sVec3_3613B0 v;
    v.x = *(float*)((char*)self + 0x50);
    v.y = *(float*)((char*)self + 0x54);
    v.z = *(float*)((char*)self + 0x58);
    *out = v;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003613F0);
#ifdef SKIP_ASM
extern "C" void func_003613F0(void *arg0, float *arg1) {
    *arg1 = (*(float *)((char*)(arg0) + (0x70)));
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361400);
#ifdef SKIP_ASM
extern "C" void func_0034DBA8(void* self, int flags);
void operator_delete(int*);
extern void* D_004908F8[];

extern "C" void func_00361400(void* self, int flags)
{
    *(void***)((char*)self + 0x50) = D_004908F8;
    func_0034DBA8((char*)self + 0x30, 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361458__FPv);
#ifdef SKIP_ASM
void* func_00361458(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361460);
#ifdef SKIP_ASM
extern "C" int func_00361460(void* self)
{
    int r = -1;
    if (*(float*)((char*)self + 0x4) >= 0.0f) {
        r = 1;
    }
    return r;
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
void* cDebounceNode__cDebounceNode(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)cDebounceNode_vtbl;
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

extern void* D_00490570[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361500__FPv);
#ifdef SKIP_ASM
void* func_00361500(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00490570;
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361528__FPv);
#ifdef SKIP_ASM
void* func_00361528(void* self)
{
    return (char*)self + 0x1C;
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
void* cCounterNode__cCounterNode(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)cCounterNode_vtbl;
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
void* cFlexBridgeNode_getBoundBox(void* self)
{
    return (char*)self + 0x90;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361578__FPv);
#ifdef SKIP_ASM
void func_00361578(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361580__FPv);
#ifdef SKIP_ASM
void func_00361580(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361588__FPv);
#ifdef SKIP_ASM
void func_00361588(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361590__FPv);
#ifdef SKIP_ASM
void func_00361590(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361598__FPv);
#ifdef SKIP_ASM
int func_00361598(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003615A0__FPv);
#ifdef SKIP_ASM
int func_003615A0(void* self)
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
INCLUDE_ASM("object/railmodifier", func_003615A8);
#ifdef SKIP_ASM
extern "C" int func_003615A8(void* self, int a1)
{
    sRailEntry50* p = *(sRailEntry50**)((char*)self + 0x70);
    return p[a1].field_0x44;
}
#endif

struct sRailEntry60 {
    char pad_0x00[0x54];
    int field_0x54;
    char pad_0x58[8];
};

//100%
INCLUDE_ASM("object/railmodifier", func_003615C0);
#ifdef SKIP_ASM
extern "C" int func_003615C0(void* self, int a1)
{
    sRailEntry60* p = *(sRailEntry60**)((char*)self + 0x70);
    return p[a1].field_0x54;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003615D8);
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

// PORT: hand-written MMI 4x4 transpose (MMI_TRANSPOSE4: lq/pextlw/pextuw/pcpyld/pcpyud/sq, no gcc 2.95 pattern;
// retail's registers differ per call site, so an operand asm) plus VU0 macro asm; a port replaces them with a
// plain C transpose and a C vector-matrix product.
extern "C" void func_003615D8(void* self, t4Matrix* out, const float* a, const float* b, const float* c, const float* d)
{
    t4Matrix L;
    t4Vector v;
    L.row[1].x = a[0]; L.row[1].y = b[0]; L.row[1].z = c[0]; L.row[1].w = d[0];
    vu0BasisMul_15D8(L.row[2], L.row[1], D_004FFAC0);
    L.row[0] = L.row[2];
    L.row[2].x = a[1]; L.row[2].y = b[1]; L.row[2].z = c[1]; L.row[2].w = d[1];
    vu0BasisMul_15D8(L.row[3], L.row[2], D_004FFAC0);
    L.row[1] = L.row[3];
    L.row[3].x = a[2]; L.row[3].y = b[2]; L.row[3].z = c[2]; L.row[3].w = d[2];
    vu0BasisMul_15D8(v, L.row[3], D_004FFAC0);
    L.row[2] = v;
    L.row[3].x = 0.0f; L.row[3].y = 0.0f; L.row[3].z = 0.0f; L.row[3].w = 1.0f;
    *out = L;
    MMI_TRANSPOSE4(out);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00361750);
#ifdef SKIP_ASM
extern "C" int func_00361750(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x5C))) == 0;
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_00361760);
#ifdef SKIP_ASM
extern "C" float func_00361760(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x10)));
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
    return (char*)self + 0x2C;
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
    void* p = self->ModifierBlock;
    if (p != 0) {
        return tModifierBlock_getBoundBox(p);
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

struct sElem00361800 {
    char data[0x40];
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
extern "C" sElem00361800* cFloatingNode_getAnimMat(cFloatingNode* self, int i)
{
    if (self->Flags & 1) {
        sVEntry00361800* vt = (sVEntry00361800*)self->mVtbl;
        vt[51].fn((char*)self + vt[51].delta);
    }
    return &((sElem00361800*)self->AnimMats)[i];
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
    void* r = vt->get.fn((char*)self + vt->get.delta);
    vt->set.fn(obj, r, self->AnimMats);
    self->Flags &= ~1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier__cPositionModifier);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void cPositionModifier__cPositionModifier(cPositionModifier* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_update__FPv);
#ifdef SKIP_ASM
void cPositionModifier_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361900__FPv);
#ifdef SKIP_ASM
int func_00361900(void* self)
{
    return *(int*)((char*)self + 0x44);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361908__FPv);
#ifdef SKIP_ASM
int func_00361908(void* self)
{
    return 0x3;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361910__FPv);
#ifdef SKIP_ASM
void* func_00361910(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cPositionModifier_setBoundBox);
#ifdef SKIP_ASM
extern "C" void cPositionModifier_setBoundBox(cPositionModifier* self, void* a1)
{
    *(cQuad128*)&self->BoundBox = *(cQuad128*)a1;
    *(cQuad128*)&self->BoundBox.MaxPoint = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361930__FPv);
#ifdef SKIP_ASM
float func_00361930(void* self)
{
    return *(float*)((char*)self + 0x40);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361938__FPvf);
#ifdef SKIP_ASM
void func_00361938(void* self, float val)
{
    *(float*)((char*)self + 0x40) = val;
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

extern "C" void* cPositionModifier_getRootTransform(cPositionModifier* self)
{
    if (self->RootTransformDirty != 0) {
        sRailVEntry1940* vt = *(sRailVEntry1940**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return (char*)self + 0x50;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier__cParentModifier);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void cParentModifier__cParentModifier(cParentModifier* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_update__FPv);
#ifdef SKIP_ASM
void cParentModifier_update(void* self)
{
    *(int*)((char*)self + 0x44) = 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cParentModifier_getMesh__FPv);
#ifdef SKIP_ASM
int cParentModifier_getMesh(void* self)
{
    return *(int*)((char*)self + 0x90);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003619D0__FPv);
#ifdef SKIP_ASM
int func_003619D0(void* self)
{
    return 0x4;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003619D8);
#ifdef SKIP_ASM
extern "C" void func_003619D8(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0xA0))) = arg1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier__cMagnetModifier);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void cMagnetModifier__cMagnetModifier(cMagnetModifier* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_isRootTransformDirty__FPv);
#ifdef SKIP_ASM
int cMagnetModifier_isRootTransformDirty(void* self)
{
    return (*(int*)((char*)self + 0x50) != 0);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_hasIntersect__FPv);
#ifdef SKIP_ASM
int cMagnetModifier_hasIntersect(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
void* cMagnetModifier_getBoundBox(void* self)
{
    return (char*)self + 0x20;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_setBoundBox);
#ifdef SKIP_ASM
extern "C" void cMagnetModifier_setBoundBox(cMagnetModifier* self, void* a1)
{
    *(cQuad128*)&self->BoundBox = *(cQuad128*)a1;
    *(cQuad128*)&self->BoundBox.MaxPoint = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_getRadius__FPv);
#ifdef SKIP_ASM
float cMagnetModifier_getRadius(void* self)
{
    return *(float*)((char*)self + 0x40);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cMagnetModifier_setRadius__FPv);
#ifdef SKIP_ASM
void cMagnetModifier_setRadius(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A58__FPv);
#ifdef SKIP_ASM
int func_00361A58(void* self)
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

extern "C" void* cMagnetModifier_getRootTransform(cMagnetModifier* self)
{
    if (self->RootTransformDirty != 0) {
        sRailVEntry1A60* vt = *(sRailVEntry1A60**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return (char*)self + 0x60;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier__cAvaSplineModifier);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void cAvaSplineModifier__cAvaSplineModifier(cAvaSplineModifier* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_getBoundBox__FPv);
#ifdef SKIP_ASM
void* cAvaSplineModifier_getBoundBox(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_setBoundBox);
#ifdef SKIP_ASM
extern "C" void cAvaSplineModifier_setBoundBox(cAvaSplineModifier* self, void* a1)
{
    *(cQuad128*)&self->BoundBox = *(cQuad128*)a1;
    *(cQuad128*)&self->BoundBox.MaxPoint = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_getRadius__FPv);
#ifdef SKIP_ASM
float cAvaSplineModifier_getRadius(void* self)
{
    return *(float*)((char*)self + 0x30);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cAvaSplineModifier_setRadius__FPvf);
#ifdef SKIP_ASM
void cAvaSplineModifier_setRadius(void* self, float val)
{
    *(float*)((char*)self + 0x30) = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B08__FPv);
#ifdef SKIP_ASM
int func_00361B08(void* self)
{
    return 0x7;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B10__FPv);
#ifdef SKIP_ASM
void* func_00361B10(void* self)
{
    return (char*)self + 0x40;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier__cSplineModifier);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

/**
 * Destroys a spline modifier: stores the base cTransformModifier vtable, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cSplineModifier__cSplineModifier(cSplineModifier* self, int flags)
{
    *(void***)self = D_004913F8;
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
int cSplineModifier_isComplete(void* self)
{
    return *(int*)((char*)self + 0x50);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", cSplineModifier_clearComplete__FPv);
#ifdef SKIP_ASM
/**
 * Clears the spline modifier's completion flag at +0x50.
 */
void cSplineModifier_clearComplete(void* self)
{
    *(int*)((char*)self + 0x50) = 0;
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
    return (char*)self + 0x10;
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
extern "C" void cSplineModifier_setBoundBox(cSplineModifier* self, void* boundbox)
{
    *(cQuad128*)&self->BoundBox = *(cQuad128*)boundbox;
    *(cQuad128*)&self->BoundBox.MaxPoint = *(cQuad128*)((char*)boundbox + 0x10);
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
float cSplineModifier_getRadius(void* self)
{
    return *(float*)((char*)self + 0x4C);
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
void cSplineModifier_setRadius(void* self, float val)
{
    *(float*)((char*)self + 0x4C) = val;
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
int cSplineModifier_isRootTransformDirty(void* self)
{
    return *(int*)((char*)self + 0x58);
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
    return (char*)self + 0x60;
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
void* cMultiSplineModifier_getBoundBox(void* self)
{
    return (char*)*(void**)((char*)self + 0x3c) + 0x30;
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
extern "C" void cMultiSplineModifier_setBoundBox(cMultiSplineModifier* self, void* boundbox)
{
    void* p = self->Nodes;
    *(cQuad128*)((char*)p + 0x30) = *(cQuad128*)boundbox;
    *(cQuad128*)((char*)p + 0x40) = *(cQuad128*)((char*)boundbox + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361C08);
#ifdef SKIP_ASM
extern "C" int func_00361C08(cMultiSplineModifier* arg0) {
    return (arg0->FocusCount);
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
float cMultiSplineModifier_getRadius(void* self)
{
    return *(float*)((char*)self + 0x18);
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
void cMultiSplineModifier_setRadius(void* self, float val)
{
    *(float*)((char*)self + 0x18) = val;
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
extern void* D_004913F8[];
void operator_delete(int*);

/**
 * Destroys a roller modifier: stores the base cTransformModifier vtable, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRollerModifier__cRollerModifier(cRollerModifier* self, int flags)
{
    *(void***)self = D_004913F8;
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
void* cRollerModifier_collide1(void* self)
{
    *(int*)((char*)self + 0x20) = 0;
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
extern "C" void cRollerModifier_setBoundBox(cRollerModifier* self, void* bb)
{
    *(cQuad128*)&self->boundbox = *(cQuad128*)bb;
    *(cQuad128*)&self->boundbox.MaxPoint = *(cQuad128*)((char*)bb + 0x10);
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
    return (char*)self + 0x220;
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
float cRollerModifier_getRadius(void* self)
{
    return *(float*)((char*)self + 0x24);
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
void cRollerModifier_setRadius(void* self, float val)
{
    *(float*)((char*)self + 0x24) = val;
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
    return (char*)self + 0x240;
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
    return (char*)self + 0x240;
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
void* cParticleNode__cParticleNode(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)cParticleNode_vtbl;
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
INCLUDE_ASM("object/railmodifier", func_00361D88__FPv);
#ifdef SKIP_ASM
void* func_00361D88(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D90);
#ifdef SKIP_ASM
extern "C" int func_00361D90(void* self)
{
    int r;
    if (*(signed char*)((char*)self + 0x14) != 0) {
        r = -1;
        if (*(float*)((char*)self + 0x0) >= 0.0f) {
            r = 1;
        }
        return r;
    }
    r = -1;
    if (*(float*)((char*)self + 0x28) >= 0.0f) {
        r = 1;
    }
    return r;
}
#endif

extern void* D_0048EA70[];
extern "C" void* func_00341CF0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361DE0__FPv);
#ifdef SKIP_ASM
void* func_00361DE0(void* self)
{
    *(int*)((char*)self + 0x3c) = (int)(void*)D_0048EA70;
    return func_00341CF0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361E08__FPv);
#ifdef SKIP_ASM
void* func_00361E08(void* self)
{
    return self;
}
#endif

extern "C" void* func_0035FE10(int, int);

//99.38%
INCLUDE_ASM("object/railmodifier", func_00361E10__FPv);
#ifdef SKIP_ASM
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
void func_00361F90(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361F98);
#ifdef SKIP_ASM
extern "C" int func_00424880(int ch);
extern "C" int func_00423A80(int cause, int (*handler)(void), int arg);
extern "C" int func_004248E8(int ch);

extern "C" void func_00361F98(void* self, int cause, int (*handler)(void), int arg)
{
    func_00424880(cause);
    *(int*)((char*)self + (cause << 2)) = func_00423A80(cause, handler, arg);
    func_004248E8(cause);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362008);
#ifdef SKIP_ASM
extern "C" int func_00424880(int ch);
extern "C" void* func_00423AA0(int ch, int id);

extern "C" void* func_00362008(void* self, int ch)
{
    func_00424880(ch);
    int* p = (int*)((char*)self + (ch << 2));
    int id = *p;
    *p = -1;
    return func_00423AA0(ch, id);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362058);
#ifdef SKIP_ASM
extern "C" int func_00423AB0(int, int, int);
extern "C" void func_00424950(int);
extern "C" void func_004249B8(int);

extern "C" void func_00362058(int arg0, int arg1, int arg2, int arg3) {
    func_00424950(arg1);
    (*(int *)(((char*)(arg0) + (arg1 << 2)) + (0x40))) = func_00423AB0(arg1, arg2, arg3);
    func_004249B8(arg1);
}
#endif
//100%
INCLUDE_ASM("object/railmodifier", func_003620C8);
#ifdef SKIP_ASM
extern "C" void func_00423AD0(int, int);

extern "C" void func_003620C8(char* self, int idx)
{
    func_00424950(idx);
    char* base = self + 0x40;
    int* slot = (int*)(base + (idx << 2));
    int old = *slot;
    *slot = -1;
    func_00423AD0(idx, old);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362120);
#ifdef SKIP_ASM
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

// PORT: 64-bit GS register writes (ulong).
extern "C" void func_003623D8(sGifPacket* pkt, int x, int w, int y, int h)
{
    ulong scissor = (ulong)x | ((ulong)(x + (w - 1)) << 16) | ((ulong)y << 32) | ((ulong)(y + (h - 1)) << 48);
    pkt->cur[0] = scissor;
    pkt->cur[1] = 0x40;
    pkt->cur[2] = scissor;
    pkt->cur[3] = 0x41;
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
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: 64-bit GS register writes (ulong).
// Emits one GS ALPHA register write (0x42 for context 1, 0x43 for context 2) for the given blend mode.
extern "C" void func_00362478(sGifPacket* pkt, int mode, int fix, int ctx)
{
    switch (ctx) {
    case 0:
        pkt->cur[1] = 0x42;
        break;
    case 1:
        pkt->cur[1] = 0x43;
        break;
    default:
        pkt->cur[1] = 0x42;
        break;
    }
    switch (mode) {
    case 0: pkt->cur[0] = 0xAA; break;
    case 1: pkt->cur[0] = 0x2A; break;
    case 5: pkt->cur[0] = 0x44; break;
    case 6: pkt->cur[0] = 0x46; break;
    case 7: pkt->cur[0] = 0x48; break;
    case 8: pkt->cur[0] = 0x81; break;
    case 9: pkt->cur[0] = 0x42; break;
    case 4: pkt->cur[0] = 0x89; break;
    case 3: pkt->cur[0] = 0x6A; break;
    case 11: pkt->cur[0] = ((ulong)fix << 32) | 0x64; break;
    case 12: pkt->cur[0] = ((ulong)fix << 32) | 0x68; break;
    case 13:
    case 14: pkt->cur[0] = ((ulong)fix << 32) | 0x88; break;
    case 2: pkt->cur[0] = ((ulong)0x80 << 32) | 0x68; break;
    case 16: pkt->cur[0] = 0x98; break;
    case 17: pkt->cur[0] = 0x58; break;
    case 10:
    case 21: pkt->cur[0] = 0x49; break;
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
// PORT: 64-bit GS register writes (ulong).
extern "C" void func_00362660(sGifPacket* pkt, unsigned int zbp, unsigned int psm, int zmsk, int ctx)
{
    ulong reg;
    switch (ctx) {
    case 0:
        reg = 0x4E;
        break;
    case 1:
        reg = 0x4F;
        break;
    default:
        reg = 0x4E;
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

// GS TEST register (TEST_1 = 0x47, TEST_2 = 0x48), as in libgraph's SCE_GS_SET_TEST.
extern "C" void func_003626D8(sRmGsPacket* pkt, int ztest, int atest, unsigned int aref, int ctx)
{
    ulong reg;
    int atst;
    int ztst;

    switch (ctx) {
    case 0:
        reg = 0x47;
        break;
    case 1:
        reg = 0x48;
        break;
    default:
        reg = 0x47;
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
// Append a DMA packet to a chain: the first one becomes the head, later ones are
// linked by patching the previous tail's tag (via the uncached-accelerated
// mirror, | 0x30000000) into a NEXT tag pointing at the new packet.
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
extern sRmState_29B8 D_00501420_29B8 __asm__("D_00501420");

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
    set1_29B8(ctx, 0xFF000, 12, 0x14);
    set0_29B8(ctx, 0x30, 4, 3);
    set0_29B8(ctx, 0x3C0, 6, 3);
    set1_29B8(ctx, 0x7C, 2, 1);
    set1_29B8(ctx, 0xF80, 7, 0x11);
    self->a = save_29B8(self, ctx);
    set1_29B8(ctx, 0xF80, 7, 2);
    self->b = save_29B8(self, ctx);
    set1_29B8(ctx, 0x7C, 2, 5);
    set1_29B8(ctx, 0xF80, 7, 3);
    set1_29B8(ctx, 0x3, 0, 1);
    set2_29B8(ctx, 0x1FFFFC00, 10, 0);
    set1_29B8(ctx, 0x1800000, 23, 1);
    set1_29B8(ctx, 0x300000, 20, 3);
    set1_29B8(ctx, 0xFF000, 12, 0x5C);
    self->c = save_29B8(self, ctx);
    self->base = self->count;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362CC8);
#ifdef SKIP_ASM
extern "C" void func_004247D8(void* begin, void* end);
extern "C" void func_0038F738(void* base, int size, int mode);
extern "C" void MEM_fill(void* dst, int c, int n);
extern void* D_004A5B84;

struct sRail20_2CC8 {
    int v[5];
};

extern sRail20_2CC8 D_00501420;

extern "C" void func_00362CC8(char* self)
{
    *(int*)(self + 0x0) = 0;
    *(int*)(self + 0x57494) = 0;
    *(int*)(self + 0x51480) = *(int*)(self + 0x57484);
    char* a = self + 0x674A0;
    func_004247D8(a, self + 0x6789F);
    char* b = self + 0x678A0;
    func_004247D8(b, self + 0x67C9F);
    MEM_fill(D_004A5B84, 0, 0x400);
    func_0038F738(a, 0x400, 2);
    func_0038F738(b, 0x400, 2);
    *(int*)(self + 0x67CA4) = 0;
    sRail20_2CC8* d = (sRail20_2CC8*)(self + 0x69CA8);
    *d = D_00501420;
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
extern "C" void func_00424698(void* begin, void* end);
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
extern "C" unsigned int* func_00394ED0(sRmBatch_2DE8* self, sRmEntry_2DE8* e, int hash);
extern "C" sRmNode_2DE8* func_00395000(sRmBatch_2DE8* self, unsigned int* state, int key, int hash, int mtxIndex);
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
    return func_00394ED0(self, e, hash8_2DE8(x));
}

static inline void addEntry_2DE8(sRmBatch_2DE8* self, sRmEntry_2DE8* e, unsigned int* state, int key)
{
    sRmNode_2DE8* node;
    if (e->continuation != 0)
        node = 0;
    else
        node = func_00395000(self, state, key, nodeHash_2DE8(state, key), e->mtxIndex);
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

// DeferMan_BuildBuckets (REF n2-render-layer.md): streams the draw records through the scratchpad in
// chunks and files each into a bucket by (state, textures, matrix); new buckets get a sort key.
// PORT: (int) of a pointer for hashing.
extern "C" void func_00362DE8(void* vself)
{
    sRmBatch_2DE8* self = (sRmBatch_2DE8*)vself;
    self->last = 0;
    func_00424698(self->entries, (char*)&self->entries[self->count] - 1);
    void* ring = D_004A4474;
    int bytes = *(int*)((char*)ring + 0x10) << 4;
    int chunk = bytes / sizeof(sRmEntry_2DE8);
    int nChunks = self->count / chunk;
    int chunkBytes = chunk * sizeof(sRmEntry_2DE8);
    sRmEntry_2DE8* buf = (sRmEntry_2DE8*)func_0038F4F8(ring, self->entries, chunkBytes, 4);
    for (int i = 0; i < nChunks; i++) {
        sRmEntry_2DE8* next = (sRmEntry_2DE8*)func_0038F4F8(ring, &self->entries[(i + 1) * chunk], chunkBytes, 4);
        for (int j = 0; j < chunk; j++) {
            sRmEntry_2DE8* e = &buf[j];
            unsigned int* state = findState_2DE8(self, e);
            sRmTexKey_2DE8 key;
            key.set(e->tex[0], e->tex[1]);
            addEntry_2DE8(self, e, state, key.i);
        }
        buf = next;
        func_0038F6A8(ring);
        func_0038F7B0();
    }
    func_0038F7B0();
    for (int j = 0; j < self->count - chunk * nChunks; j++) {
        sRmEntry_2DE8* e = &buf[j];
        unsigned int* state = findState_2DE8(self, e);
        sRmTexKey_2DE8 key;
        key.set(e->tex[0], e->tex[1]);
        addEntry_2DE8(self, e, state, key.i);
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
    *d = D_00501420;
}

// Draws the sorted batch: counts texture use, splits the sorted list into three passes by
// blend mode, then emits each node's DMA chain (closing the VIF/GIF chains between passes).
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
                short u = ((short*)&n->key)[0];
                if (u >= 0) {
                    D_0053AB80_3490[u].first = idx;
                    D_0053AB80_3490[u].count++;
                }
                short v = ((short*)&n->key)[1];
                if (v >= 0) {
                    D_0053AB80_3490[v].first = idx;
                    D_0053AB80_3490[v].count++;
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
                            ulong* t = (ulong*)(n->tail | 0x30000000);
                            t[0] = 0x60000000;
                            t[1] = 0;
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
                ulong* q = (ulong*)(*path | 0x20000000);
                q[0] = 0x70000000;
                q[1] = 0;
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
        ulong* q = (ulong*)(*path | 0x20000000);
        q[0] = 0x70000000;
        q[1] = 0;
        *path += 0x10;
        *out2 = *path;
    }
    ulong* q = (ulong*)(*path | 0x20000000);
    q[0] = 0x70000000;
    q[1] = 0;
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
extern "C" void func_00363C20_impl(__SIZE_TYPE__ selfi, sRmGs3C20* st, int a2, ulong** pp)
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
        unsigned int c = Mask3C20(0x3FFFFFFF, st->w3);
        if (((State3C20(self)->w0 & 0x3C0) >> 6) != ctx) {
            self->buf = a2;
            self->frame = c;
            func_00364CD0(self, (st->w0 & 0x3C0) >> 6, 0, c, pp);
        } else if (a2 != self->buf) {
            self->buf = a2;
            self->frame = c;
            func_00364CD0(self, (st->w0 & 0x3C0) >> 6, 1, c, pp);
        }
        if (self->frame != c) {
            self->buf = a2;
            self->frame = c;
            func_00364CD0(self, (st->w0 & 0x3C0) >> 6, 2, c, pp);
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

// 4-pass 8-bit radix sort of the item list at self+0x67CA8 (count at +0x67CA4).
extern "C" void func_00364050(char* self)
{
    void* ring = D_004A4474;
    sRmSortItem4050* dst = D_00505B70;
    int n = *(int*)(self + 0x67CA4);
    sRmSortItem4050* src = (sRmSortItem4050*)(self + 0x67CA8);
    int (*hist)[257] = (int (*)[257])func_0038F460(ring, 0, -1, 0);
    MEM_fill(hist, 0, 0x1010);
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
        int* h = (int*)hist + ((shift << 5) + (shift >> 3));
        for (int i = 0; i < n; i++) {
            int idx = h[(src[i].key >> shift) & 0xFF]++;
            dst[idx] = src[i];
        }
        sRmSortItem4050* t = dst;
        dst = src;
        src = t;
    }
    func_0038F6A8(ring);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00364240);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Packs the rail's blend/test state into a negated register word.
extern "C" unsigned int func_00364240(unsigned int* self, int key)
{
    int a = 31 - ((self[2] & 0x3E0) >> 5);
    int b;
    int sel = self[1] & 3;
    switch (sel) {
    case 0:
        b = 0x3FF;
        break;
    case 1:
        b = 0x3FE;
        break;
    case 2:
        b = (self[2] & 0x1FFFFC00) >> 10;
        break;
    default:
        b = 0;
        break;
    }
    int c;
    switch ((self[0] & 0x3C0) >> 6) {
    case 5: c = 6; break;
    case 2: c = 5; break;
    case 1: c = 4; break;
    case 3: c = 3; break;
    case 6:
    case 7: c = 2; break;
    case 4:
    case 8: c = 1; break;
    case 0:
    default: c = 0; break;
    }
    int lo = (short)key;
    int hi = key >> 16;
    int d = 0;
    if (lo >= 0) {
        d = lo;
    }
    if (hi >= 0) {
        d |= (hi & 0xF) << 6;
    }
    d &= 0x3FF;
    return ~((a << 26) | (b << 16) | (c << 13) | (d << 3));
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

// PORT: 64-bit DMA/GIF tag writes (ulong).
extern "C" void func_00364360(void* self, ulong** pp)
{
    char* gs = cGraphicsMan_TheGraphicsMan_railmodifier;
    sRmGsState4360 st;
    sRmPkt4360 pkt;
    st = *(sRmGsState4360*)&D_00501420;
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
extern char* D_004A4248;
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

// Per material type, emits the VU1 microprogram CALL and its constant header (mode 0), the matrix
// header only (mode 1), or the per-draw CALL (mode 2, types 6 and 7).
// PORT: 64-bit DMA/VIF/GIF words (ulong), 128-bit copies (cQuad128), addresses held in int.
extern "C" void func_00364CD0(void* vself, int type, int mode, unsigned int w3, ulong** pp)
{
    sDefer_4CD0* self = (sDefer_4CD0*)vself;
    sPkt_4CD0 pk(*pp);
    sMtxRec_4CD0* M = &self->mtx[self->curMtx];
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
            ((cQuad128*)pk.p)[2] = ((cQuad128*)M)[8];
            ((cQuad128*)pk.p)[3] = ((cQuad128*)M)[9];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)M)[10];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)M)[11];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)M)[12];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)M)[13];
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
            ((cQuad128*)pk.p)[3] = ((cQuad128*)M)[8];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)M)[9];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)M)[10];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)M)[11];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)M)[12];
            ((cQuad128*)pk.p)[8] = ((cQuad128*)M)[13];
            pk.p[0x12] = 0x0000008000000080;
            pk.p[0x13] = 0x0000008000000080;
            pk.p[0x14] = 0x302E400000008000;
            pk.p[0x15] = 0x412;
            pk.p[0x16] = 0x312E400000008000;
            pk.p[0x17] = 0x412;
            pk.p[0x18] = 0x302EC00000008000;
            pk.p[0x19] = 0x412;
            pk.p[0x1A] = 0x312EC00000008000;
            pk.p[0x1B] = 0x412;
            pk.p[0x1C] = 0;
            pk.p[0x1D] = 0x6C2A020000000000;
            char* g = D_004A4248;
            pk.p[0x1E] = ((ulong)*(int*)(g + 0x1A0) << 32) | 0x30000012;
            pk.p[0x1F] = 0;
            ulong ref2 = ((ulong)*(int*)(g + 0x320) << 32) | 0x30000012;
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
            ((cQuad128*)pk.p)[2] = ((cQuad128*)M)[8];
            ((cQuad128*)pk.p)[3] = ((cQuad128*)M)[9];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)M)[10];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)M)[11];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)M)[12];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)M)[13];
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
            ((cQuad128*)pk.p)[3] = ((cQuad128*)M)[8];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)M)[9];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)M)[10];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)M)[11];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)M)[12];
            ((cQuad128*)pk.p)[8] = ((cQuad128*)M)[13];
            pk.p[0x12] = 0x000002C20000010C;
            pk.p[0x13] = 0;
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
            ((cQuad128*)pk.p)[2] = ((cQuad128*)M)[8];
            ((cQuad128*)pk.p)[3] = ((cQuad128*)M)[9];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)M)[10];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)M)[11];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)M)[12];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)M)[13];
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
            putQw_4CD0(pk.p, M->worldView, 4);
            sVec_4CD0 a = M->guardScale;
            a.z *= 0.0625f;
            putQw_4CD0(pk.p, &a, 1);
            sVec_4CD0 b = M->guardOffset;
            b.z *= 0.0625f;
            putQw_4CD0(pk.p, &b, 1);
            sVec_4CD0 c;
            c.x = M->view[0];
            c.y = M->view[5];
            c.z = 0;
            c.w = 0;
            putQw_4CD0(pk.p, &c, 1);
            break;
        }
        case 1: {
            pk.p[0] = 0x10000008;
            pk.p[1] = 0;
            pk.p[2] = 0;
            pk.p[3] = 0x6C07000000000000;
            pk.p += 4;
            putQw_4CD0(pk.p, M->worldClip, 4);
            sVec_4CD0 a = M->screenScale;
            a.z *= 0.0625f;
            putQw_4CD0(pk.p, &a, 1);
            sVec_4CD0 b = M->screenOffset;
            b.z *= 0.0625f;
            putQw_4CD0(pk.p, &b, 1);
            sVec_4CD0 c;
            c.x = M->viewProj[0];
            c.y = M->viewProj[5];
            c.z = 0;
            c.w = 0;
            putQw_4CD0(pk.p, &c, 1);
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
            sVec_4CD0 a = M->screenScale;
            a.z *= 0.0625f;
            putQw_4CD0(pk.p, &a, 1);
            sVec_4CD0 b = M->screenOffset;
            b.z *= 0.0625f;
            putQw_4CD0(pk.p, &b, 1);
            sVec_4CD0 c;
            c.x = M->viewProj[0];
            c.y = M->viewProj[5];
            c.z = 0;
            c.w = 0;
            putQw_4CD0(pk.p, &c, 1);
            break;
        }
        case 1: {
            pk.p[0] = 0x10000004;
            pk.p[1] = 0;
            pk.p[2] = 0;
            pk.p[3] = 0x6C03000000000000;
            pk.p += 4;
            sVec_4CD0 a = M->screenScale;
            a.z *= 0.0625f;
            putQw_4CD0(pk.p, &a, 1);
            sVec_4CD0 b = M->screenOffset;
            b.z *= 0.0625f;
            putQw_4CD0(pk.p, &b, 1);
            sVec_4CD0 c;
            c.x = M->viewProj[0];
            c.y = M->viewProj[5];
            c.z = 0;
            c.w = 0;
            putQw_4CD0(pk.p, &c, 1);
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
            ((cQuad128*)pk.p)[3] = ((cQuad128*)M)[8];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)M)[9];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)M)[10];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)M)[11];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)M)[12];
            ((cQuad128*)pk.p)[8] = ((cQuad128*)M)[13];
            pk.p[0x12] = 0x10000005;
            pk.p[0x13] = 0;
            pk.p[0x14] = 0;
            pk.p[0x15] = 0x6C04039200000000;
            pk.p[0x16] = 0x312E400000008000;
            pk.p[0x17] = 0x412;
            sMtx_4CD0 m = M->world;
            m = D_004FFB80_4CD0 * m;
            ((cQuad128*)pk.p)[0xC] = *(cQuad128*)&m.m[0];
            ((cQuad128*)pk.p)[0xD] = *(cQuad128*)&m.m[4];
            ((cQuad128*)pk.p)[0xE] = *(cQuad128*)&m.m[8];
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
            ((cQuad128*)pk.p)[2] = ((cQuad128*)M)[8];
            ((cQuad128*)pk.p)[3] = ((cQuad128*)M)[9];
            ((cQuad128*)pk.p)[4] = ((cQuad128*)M)[10];
            ((cQuad128*)pk.p)[5] = ((cQuad128*)M)[11];
            ((cQuad128*)pk.p)[6] = ((cQuad128*)M)[12];
            ((cQuad128*)pk.p)[7] = ((cQuad128*)M)[13];
            pk.p[0x10] = 0x10000005;
            pk.p[0x11] = 0;
            pk.p[0x12] = 0;
            pk.p[0x13] = 0x6C04039200000000;
            pk.p[0x14] = 0x312E400000008000;
            pk.p[0x15] = 0x412;
            sMtx_4CD0 m = M->world;
            m = D_004FFB80_4CD0 * m;
            ((cQuad128*)pk.p)[0xB] = *(cQuad128*)&m.m[0];
            ((cQuad128*)pk.p)[0xC] = *(cQuad128*)&m.m[4];
            ((cQuad128*)pk.p)[0xD] = *(cQuad128*)&m.m[8];
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

extern "C" void func_00365E40(void* list, int a1, int a2, void* pool)
{
    sRailModE40* self = (sRailModE40*)list;
    int i;
    self->f1FF4 = a2;
    self->links[0].flags = a1;
    self->links[0].f4 = a2;
    self->links[0].fC = -1;
    self->links[0].next = -1;
    self->links[0].prev = -1;
    for (i = 1; i < 0x123; i++) {
        self->links[i].flags &= ~1;
        self->links[i].next = i + 1;
    }
    self->links[0x123].next = -1;
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
extern "C" void func_00424698(void* begin, void* end);
extern "C" int func_0038F708(void* base, int size, int mode);

extern "C" void func_00365F68(void* self)
{
    func_00424698(self, (char*)self + 0x1FEF);
    *(int*)((char*)self + 0x1FF0) = func_0038F708(self, 0x1FF0, 2);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00365FA8);
#ifdef SKIP_ASM
extern "C" void func_004247D8(void* begin, void* end);
extern "C" void func_0038F738(void* base, int size, int mode);

extern "C" void func_00365FA8(void* self)
{
    func_004247D8(self, (char*)self + 0x1FEF);
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

// Links node idx into its size-class list (list A ordered by field_0x0, list B by size).
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
    int b = railFloorLog2_60A8(node->size) - 6;
    sRailNode* head = &heads[b];
    int first = head->next2;
    if (first == -1) {
        head->next2 = idx;
        node->prev2 = b | tag;
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
        int l = n->prev;
        if (l >= 0 && (self->nodes[l].field_0x0 & 1) == 0) {
            idx = l;
            n = &self->nodes[l];
        } else {
            int r = n->next;
            if (r < 0 || (self->nodes[r].field_0x0 & 1)) {
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

// Walks the rail's link list and calls func_003663D8 on every enabled link without flag bit 1.
extern "C" void func_00366548(void* self)
{
    char* s = (char*)self;
    int i = *(int*)(s + 0x23C0);
    while (i >= 0) {
        sRailLink6548* links = *(sRailLink6548**)(s + 0x1FF0);
        sRailLink6548* cur = &links[i];
        int next = cur->next;
        while (next >= 0) {
            if ((*(sRailLink6548**)(s + 0x1FF0))[next].flags & 1) {
                break;
            }
            next = (*(sRailLink6548**)(s + 0x1FF0))[next].next;
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

// Walks the rail's link list, clearing flag bit 1 and calling func_003663D8 on every enabled link.
extern "C" void func_00366618(void* self)
{
    char* s = (char*)self;
    int i = *(int*)(s + 0x23C0);
    while (i >= 0) {
        sRailLink6618* links = *(sRailLink6618**)(s + 0x1FF0);
        sRailLink6618* cur = &links[i];
        int next = cur->next;
        while (next >= 0) {
            if ((*(sRailLink6618**)(s + 0x1FF0))[next].flags & 1) {
                break;
            }
            next = (*(sRailLink6618**)(s + 0x1FF0))[next].next;
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

extern "C" void func_003666F8()
{
    cQuad128 fill = makeFill66F8(0xFFFFFFFF, 0xFFFFFFFF);
    func_004247D8(D_0053AB80, D_0053E9FF);
    cQuad128* p = (cQuad128*)D_004A5B84;
    unsigned int n = spadFree66F8() >> 4;
    for (; n != 0; n--) {
        *p++ = fill;
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

extern "C" int func_003667F8(sRailNodeMan* self, int size)
{
    int b;
    for (b = railFloorLog2(size) - 6; b < 17; b++) {
        int i = self->headsB[b].next2;
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

// Returns the index of the disabled link with the largest f4 (at least 0), or -1.
extern "C" int func_00366888(void* self)
{
    char* s = (char*)self;
    int i = *(int*)(s + 0x23C0);
    int best = 0;
    int idx = -1;
    while (i >= 0) {
        sRailLink6888* links = *(sRailLink6888**)(s + 0x1FF0);
        if (!(links[i].flags & 1)) {
            int v = links[i].f4;
            if (v >= best) {
                best = v;
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

// Picks the cheapest free node of at least 'size' bytes from its size class (skipping locked nodes and the two
// given owners; owners whose limit is above 'age' cost size*weight) and frees it. Returns 1 if one was freed.
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
    int prev;       // 0x8
    int fC;
    int next;       // 0x10
    int f14;
    int f18;
};

// Finds the enabled link whose prev field equals `key`; returns its index or -1.
extern "C" int func_00366DD8(void* self, int key)
{
    char* s = (char*)self;
    int i = *(int*)(s + 0x23C0);
    while (i >= 0) {
        sRailLinkDD8* links = *(sRailLinkDD8**)(s + 0x1FF0);
        if ((links[i].flags & 1) && links[i].prev == key) {
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

extern "C" void func_00366E30(void* self, int i)
{
    char* e = (*(char***)((char*)self + 0x23E8))[i];
    if (*(int*)(e + 0x30) >= 0) {
        func_003663D8(self, *(int*)(e + 0x30));
    }
    if (*(int*)(e + 0x34) >= 0) {
        func_003663D8(self, *(int*)(e + 0x34));
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
extern char D_004A4060[];

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

extern "C" void func_00366E90(char* self, sRmTex_366E90* tex)
{
    int n = 0;
    char* buf = new (D_004A4060, *(int*)(self + 0x1F58) ^ 0x100, 0) char[tex->size];
    int tw = (tex->tex0 >> 26) & 0xF;
    int th = (tex->tex0 >> 30) & 0xF;
    int bpp = tex->bpp;
    int w = 1 << tw;
    int h = 1 << th;
    char* src = tex->data;
    if (tex->mipCount >= 0) {
        do {
            n++;
            int bytes = w * h * bpp / 8;
            if (bytes <= 0xFF) {
                bytes = 0x100;
            }
            MEM_copy(buf, src, bytes);
            func_00390198(self, w, h, buf, src);
            src += bytes;
            w >>= 1;
            h >>= 1;
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

extern "C" void func_00366FE8(void* self)
{
    *(int*)((char*)self + 0x1F58) = 0;
    func_003670E0(self);
    int* a = (int*)((char*)self + 0x1F4C);
    for (int i = 1; i >= 0; i--) {
        a[i] = -1;
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

extern "C" void func_00367078(void* self)
{
    func_00366618((char*)self + 0x4350);
    func_00366618((char*)self + 0x1F60);
    int* a = (int*)((char*)self + 0x1F4C);
    for (int i = 1; i >= 0; i--) {
        a[i] = -1;
    }
    *(int*)((char*)self + 0x1F54) = -1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003670E0);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

extern "C" void func_003670E0(void* self)
{
    int i;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    MEM_fill((char*)self + 0x8, 0, 0x1F40);
    *(void***)((char*)self + 0x1F48) = (void**)((char*)self + 0x1778);
    for (i = 0; i < 499; i++) {
        void** list = *(void***)((char*)self + 0x1F48);
        list[i] = &list[i + 1];
    }
}
#endif


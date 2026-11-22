#include "common.h"
#include "world/worldrecords.h"
#include "vtable.h"
#include "world/instancerecord.h"
#include "bx/math3d.h"
#include "world/worldview.h"
#include "bx/appman.h"
#include "object/floatingnode.h"
#include "object/instancenode.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cInstanceNode_getBoundBoxInfo(void* self, void* out);
extern const char str_FloatingBoundBox[];


//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_initInfo__FP13cFloatingNode);
#ifdef SKIP_ASM
// PORT: the unit declares cInstanceNode_getBoundBoxInfo with 2 args; its body uses 3 ($6 is written).
void cInstanceNode_getBoundBoxInfo_3(void* self, void* box, void* out) __asm__("cInstanceNode_getBoundBoxInfo");

/**
 * Allocates a floating node's info block (0x30 bytes, "FloatingBoundBox") and fills it with the instance's
 * bounding box (+0x0) and the value cInstanceNode_getBoundBoxInfo() writes at +0x20 (the radius).
 */
void cFloatingNode_initInfo(cFloatingNode* self)
{
    tFloatBoundBoxInfo* mem = (tFloatBoundBoxInfo*)cMemMan_alloc(0x30, str_FloatingBoundBox, 0x20000000, 0);
    (*(void**)&self->Info) = ((void*)mem);
    cInstanceNode_getBoundBoxInfo_3(self, mem, &mem->Radius);
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_purgeInfo);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Frees the floating node's info block (+0x78) with operator delete, when there is one, and clears the pointer.
 */
extern "C" void cFloatingNode_purgeInfo(cFloatingNode* self)
{
    if (((void*)self->Info) != 0) {
        operator_delete((int*)((void*)self->Info));
        (*(void**)&self->Info) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_updateOverlapSystem);
#ifdef SKIP_ASM
struct sBox_F048;
extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, void* id, tBoundBox* box, tBoundBox* old);
extern "C" void cMoveNode_updateOverlapSystem(void* self);




// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector vu0Sub_34EF40(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vu0Add_34EF40(const t4Vector& a, const t4Vector& b)
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

struct sFlVEntry_34EF40 {
    short delta;
    short index;
    char* (*fn)(void*);
};

static inline t4Vector nodePos_34EF40(char* vself)
{
    cFloatingNode* self = (cFloatingNode*)vself;
    sFlVEntry_34EF40* vt = (sFlVEntry_34EF40*)self->mVtbl;
    return *(t4Vector*)(vt[24].fn(vthis(self, vt[24])) + 0x30);
}

/**
 * Moves a floating object in the Nash space hash to a box of its radius around its current position.
 *
 * Without an info block it falls back to cMoveNode_updateOverlapSystem().
 */
extern "C" void cFloatingNode_updateOverlapSystem(cFloatingNode* self)
{
    tFloatBoundBoxInfo* info = self->Info;
    if (info != 0) {
        tBoundBox nb;
        tBoundBox old = (*(&info->BoundBox));
        float extent = info->Radius;
        t4Vector ext;
        ext.x = extent;
        ext.y = extent;
        ext.z = extent;
        ext.w = 0.0f;
        nb.MinPoint = vu0Sub_34EF40(nodePos_34EF40(((char*)self)), ext);
        nb.MaxPoint = vu0Add_34EF40(nodePos_34EF40(((char*)self)), ext);
        (*(&(self->Info)->BoundBox)) = nb;
        void* id = self->Instance;
        nash_Nash_movenode(cObjectInterface_getNash(), 0, id, (tBoundBox*)&nb, (tBoundBox*)&old);
    } else {
        cMoveNode_updateOverlapSystem(((char*)self));
    }
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_restoreOverlapSystem);
#ifdef SKIP_ASM



extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, void* id, tBoundBox* box, tBoundBox* old);
extern "C" void cMoveNode_restoreOverlapSystem(void* self);

/**
 * Puts a floating object back into the Nash space hash with its instance's own bounding box and frees its
 * info block.
 *
 * Without an info block it falls back to cMoveNode_restoreOverlapSystem().
 */
extern "C" void cFloatingNode_restoreOverlapSystem(cFloatingNode* self)
{
    tBoundBox* info = (tBoundBox*)((void*)self->Info);
    if (info != 0) {
        tBoundBox nb;
        tBoundBox old = *info;
        tInstanceRecord* inst = self->Instance;
        t4Vector corner;
        corner.x = inst->m_fvMin.m[0];
        corner.y = inst->m_fvMin.m[1];
        corner.z = inst->m_fvMin.m[2];
        corner.w = 1.0f;
        (*(&nb.MinPoint)) = corner;
        corner.x = inst->m_fvMax.m[0];
        corner.y = inst->m_fvMax.m[1];
        corner.z = inst->m_fvMax.m[2];
        (*(&nb.MaxPoint)) = corner;
        *info = nb;
        void* id = self->Instance;
        nash_Nash_movenode(cObjectInterface_getNash(), 0, id, &nb, &old);
        cFloatingNode_purgeInfo(self);
    } else {
        cMoveNode_restoreOverlapSystem(self);
    }
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_makeAnimMats);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle); // sincos



struct sVec4_F120 {
    float x, y, z, w;
    sVec4_F120() {}
    sVec4_F120(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));


struct sSkel_F120 {
    int pad0;
    int count;          // 0x4
    tMeshHeirarchyRecord* bones;  // 0x8
};


struct sFloatingNode_F120 {
    char pad0[0x12];
    unsigned short flags;   // 0x12
    char pad14[0x18 - 0x14];
    char* info;             // 0x18
    char pad1C[0x2C - 0x1C];
    float k;                // 0x2C
    char pad30[0x50 - 0x30];
    float pos1;             // 0x50
};

// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
static inline void Copy_F120(void* dst, const void* src)
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
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply); the PC port needs a plain matrix multiply.
static inline void Mul_F120(void* out, const void* a, const void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(out), "r"(a), "r"(b)
        : "memory");
}

static inline t4Vector Row_F120(const t4Matrix* m, int k)
{
    return m->row[k];
}

static inline sVec4_F120 CompMul_F120(const t4Vector& a, const sVec4_F120& b)
{
    return sVec4_F120(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

// dst = parent * (local with its translation scaled by s).
static inline void Pose_F120(t4Matrix* dst, const t4Matrix* parent, const t4Matrix* local, const sVec4_F120& s)
{
    t4Matrix l;
    l.row[0] = Row_F120(local, 0);
    l.row[1] = Row_F120(local, 1);
    l.row[2] = Row_F120(local, 2);
    sVec4_F120 v = CompMul_F120(Row_F120(local, 3), s);
    l.row[3] = *(t4Vector*)&v;
    t4Matrix t;
    Mul_F120(&t, parent, &l);
    Copy_F120(dst, &t);
}

/**
 * Builds a floating object's part matrices: the bob and tilt on top of the root transform, then each part
 * under its parent.
 *
 * The root gets a rotation from the two tilt angles and a lift of the bob height minus the root's z. Parts
 * are scaled by the instance's scale. Clears the node's dirty bit afterwards.
 *
 * @param mat  the root transform (On Tour root)
 * @param out  receives one matrix per part (On Tour animMats)
 */
// Poses a floating object's skeleton: the root bob/tilt transform, then each bone under its parent.
extern "C" void cFloatingNode_makeAnimMats(cFloatingNode* self, t4Matrix* mat, t4Matrix* out)
{
    t4Matrix tiltMat;
    t4Vector base = mat->row[3];
    float pos[3];
    float rot[3];
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = self->Z - base.z;
    cFloatingNodeState* tilt = (cFloatingNodeState*)&self->ZSpring;
    rot[0] = tilt->OX;
    rot[1] = tilt->OY;
    rot[2] = 0.0f;
    // Rotation from the tilt angles (same construction as cMeshAnimFrame_update), translated by pos.
    float sx, cx, sy, cy, sz, cz;
    fSinCos(&sx, &cx, rot[0]);
    fSinCos(&sy, &cy, rot[1]);
    fSinCos(&sz, &cz, rot[2]);
    float sxsy = sx * sy;
    float cxsy = cx * sy;
    float cxsz = cx * sz;
    tiltMat.row[0].x = cy * cz;
    tiltMat.row[0].y = -cy * sz;
    tiltMat.row[0].z = sy;
    tiltMat.row[0].w = 0.0f;
    tiltMat.row[1].x = sxsy * cz + cxsz;
    tiltMat.row[1].y = cx * cz - sxsy * sz;
    tiltMat.row[1].z = -sx * cy;
    tiltMat.row[1].w = 0.0f;
    tiltMat.row[2].x = sx * sz - cxsy * cz;
    tiltMat.row[2].y = cxsz * sy + sx * cz;
    tiltMat.row[2].z = cx * cy;
    tiltMat.row[2].w = 0.0f;
    tiltMat.row[3].x = pos[0];
    tiltMat.row[3].y = pos[1];
    tiltMat.row[3].z = pos[2];
    tiltMat.row[3].w = 1.0f;
    sSkel_F120* skel = (sSkel_F120*)self->Instance->m_Model;
    float sc4 = self->Instance->m_fScale;
    tMeshHeirarchyRecord* bone = skel->bones;
    sVec4_F120 scale(sc4, sc4, sc4, 1.0f);
    int i;
    for (i = 0; i < skel->count; i++, bone++) {
        t4Matrix boneMat;
        if (bone->m_iParentID != -1) {
            Pose_F120(&boneMat, &out[bone->m_iParentID], ((t4Matrix*)bone->m_pMeshMatrix), scale);
            Copy_F120(&out[i], &boneMat);
        } else {
            t4Matrix w;
            t4Matrix rootMat;
            Mul_F120(&rootMat, mat, &tiltMat);
            Copy_F120(&w, &rootMat);
            Pose_F120(&boneMat, &w, ((t4Matrix*)bone->m_pMeshMatrix), scale);
            Copy_F120(&out[i], &boneMat);
        }
    }
    self->Flags &= 0xFFFE;
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_applyForce);
#ifdef SKIP_ASM

extern t4Vector D_004FF140_F600[] __asm__("t4vect_XAxis");
extern t4Vector D_004FF150_F600[] __asm__("t4vect_YAxis");

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector vu0Sub_F600(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector pos_F600(t4Vector* m)
{
    return m[3];
}

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector).
static inline t4Vector vu0MtxMul_F600(const t4Vector* m, const t4Vector* v)
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
        : "o"(*v), "r"(m)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector vu0Cross_F600(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_F600(const t4Vector& a, const t4Vector& b)
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

struct sFlVE_F600 {
    short delta;
    short index;
    t4Vector* (*fn)(void*);
};

struct sFloat_F600 {
    char pad_0x00[0xC];
    sFlVE_F600* vt;     // 0x0C
    char pad_0x10[0x34];
    float f44;
    char pad_0x48[0x18];
    float f60;
    char pad_0x64[0xC];
    float f70;
};

/**
 * Adds a push at a hit point to a floating object's bob and tilt inputs for the next update.
 *
 * The bob input gets the force along the hit normal; the two tilt inputs get the torque about the object's
 * local x and y axes.
 *
 * @param intInfo  the hit (On Tour tIntersectInfo): position, then normal
 * @param force    the force
 */
extern "C" void cFloatingNode_applyForce(cFloatingNode* self, t4Vector* intInfo, t4Vector* force)
{
    sFlVE_F600* vt = (sFlVE_F600*)self->mVtbl;
    t4Vector* transform = vt[24].fn(vthis(self, vt[24]));
    t4Vector arm = vu0Sub_F600(intInfo[0], pos_F600(transform));
    t4Vector axisX = vu0MtxMul_F600(transform, &D_004FF140_F600[0]);
    t4Vector axisY = vu0MtxMul_F600(transform, &D_004FF150_F600[0]);
    self->Force += vu0Dot_F600(*force, intInfo[1]);
    t4Vector torque = vu0Cross_F600(*force, arm);
    self->TX += vu0Dot_F600(torque, axisX);
    self->TY += vu0Dot_F600(torque, axisY);
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_updateIntersectInfo);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV4_F790 {
    float x, y, z, w;
    sV4_F790() {}
    sV4_F790(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern sV4_F790 D_004FF140_F790[] __asm__("t4vect_XAxis");
extern sV4_F790 D_004FF150_F790[] __asm__("t4vect_YAxis");
extern "C" void cMoveNode_updateIntersectInfo(void* self, void* a);

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV4_F790 vu0Sub_F790(const sV4_F790& a, const sV4_F790& b)
{
    sV4_F790 r;
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
static inline sV4_F790 vu0Add_F790(const sV4_F790& a, const sV4_F790& b)
{
    sV4_F790 r;
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_F790(sV4_F790& a, const sV4_F790& b)
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

static inline sV4_F790 pos_F790(sV4_F790* m)
{
    return m[3];
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sV4_F790 vu0Scale_F790(const sV4_F790& v, float s)
{
    sV4_F790 r;
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

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector).
static inline sV4_F790 vu0MtxMul_F790(const sV4_F790* m, const sV4_F790* v)
{
    sV4_F790 r;
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
        : "o"(*v), "r"(m)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sV4_F790 vu0Cross_F790(const sV4_F790& a, const sV4_F790& b)
{
    sV4_F790 r;
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

struct sFlVE_F790 {
    short delta;
    short index;
    sV4_F790* (*fn)(void*);
};



/**
 * Adds a floating object's motion to an intersection's velocities.
 *
 * After cMoveNode_updateIntersectInfo(), adds the bob speed (along z) plus the tilt rotation's velocity at
 * the hit point to intInfo's v[2] and the tilt angular velocity to v[3].
 *
 * @param intInfo  the hit (On Tour tIntersectInfo)
 */
extern "C" void cFloatingNode_updateIntersectInfo(cFloatingNode* self, sV4_F790* intInfo)
{
    cMoveNode_updateIntersectInfo((self), intInfo);
    sFlVE_F790* vt = (sFlVE_F790*)self->mVtbl;
    sV4_F790* transform = vt[24].fn(vthis(self, vt[24]));
    cFloatingNodeState* base = (cFloatingNodeState*)&self->ZSpring;
    sV4_F790 arm = vu0Sub_F790(intInfo[0], pos_F790(transform));
    sV4_F790 axisX = vu0MtxMul_F790(transform, &D_004FF140_F790[0]);
    sV4_F790 axisY = vu0MtxMul_F790(transform, &D_004FF150_F790[0]);
    sV4_F790 angVel = vu0Add_F790(vu0Scale_F790(axisX, base->WX), vu0Scale_F790(axisY, base->WY));
    vu0AddEq_F790(intInfo[2], vu0Add_F790(sV4_F790(0.0f, 0.0f, base->Vel, 0.0f), vu0Cross_F790(arm, angVel)));
    vu0AddEq_F790(intInfo[3], angVel);
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_updateNode);
#ifdef SKIP_ASM


struct sFloatingNode_34F930 {
    char pad0[0x12];
    unsigned short flags;           // 0x12
    char pad14[0x2C - 0x14];
    float k;            // 0x2C
    float damp;         // 0x30
    float gain;         // 0x34
    float k2;           // 0x38
    float damp2;        // 0x3C
    float gain2;        // 0x40
    float in1;          // 0x44
    float vel1;         // 0x48
    float rest1;        // 0x4C
    float pos1;         // 0x50
    float pos2;         // 0x54
    float pad58;
    float vel2;         // 0x5C
    float in2;          // 0x60
    float pos3;         // 0x64
    float pad68;
    float vel3;         // 0x6C
    float in3;          // 0x70
};

/**
 * Steps a floating object's springs by one frame: the bob height and the two tilt angles.
 *
 * Each is a damped spring driven by the inputs gathered by applyForce (the bob towards its rest height); the
 * step is 1 / the app's frame rate. The tilts are clamped to +/-80 degrees, the inputs are cleared and the
 * dirty bit is set.
 *
 * @return always 1 (the node stays alive)
 */
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// Steps the float spring (bob height plus two tilt axes) by one frame.
extern "C" int cFloatingNode_updateNode(cFloatingNode* self)
{
    float dt = 1.0f / (float)cAppMan_TheAppMan->TimerHz;
    float accelZ = -self->ZSpring * (self->Z - self->Zo) + self->ForceScale * self->Force + -self->ZDamp * self->Vel;
    float accelX = -self->ASpring * self->OX + self->TorqueScale * self->TX + -self->ADamp * self->WX;
    float accelY = -self->ASpring * self->OY + self->TorqueScale * self->TY + -self->ADamp * self->WY;
    self->Z += self->Vel * dt;
    self->Vel += accelZ * dt;
    self->OX += self->WX * dt;
    self->WX += accelX * dt;
    self->OY += self->WY * dt;
    self->WY += accelY * dt;
    cFloatingNodeState* tilt = (cFloatingNodeState*)&self->ZSpring;
    float lo = -1.3962635f;    // +-80 degrees
    float hi = 1.3962635f;
    self->OX = tilt->OX >= lo ? tilt->OX <? hi : lo;
    self->OY = tilt->OY >= lo ? tilt->OY <? hi : lo;
    self->Force = 0.0f;
    self->TX = 0.0f;
    self->TY = 0.0f;
    self->Flags |= 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sFloatingVEntryFA88 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cMoveNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a floating node's state to a replay frame: the cMoveNode part, whether it has an info block, and the
 * 0x48 bytes of spring state at +0x2C.
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cFloatingNode_writeToReplayFrame(cFloatingNode* self, void* stream)
{
    int has;
    cMoveNode_writeToReplayFrame(self, stream);
    has = ((void*)self->Info) != 0;
    sFloatingVEntryFA88* vtEntry = &(*(sFloatingVEntryFA88**)stream)[1];
    vtEntry->fn(vthis(stream, *vtEntry), &has, 4);
    vtEntry = &(*(sFloatingVEntryFA88**)stream)[1];
    vtEntry->fn(vthis(stream, *vtEntry), (char*)&self->ZSpring, 72);
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", cInstanceNode_cInstanceNode1);
#ifdef SKIP_ASM
extern "C" void* cSortObjNode_cSortObjNode1(void* self, void* a1, int type);
extern "C" void cInstanceNode_setActiveNode(void* self, void* node);
extern "C" void cObjectInterface_addToFocusBox(void* node);
extern char cInstanceNode_vtbl[];

/**
 * Constructs the instance-node base of a node for a placed instance.
 *
 * Runs the sort-node constructor cSortObjNode_cSortObjNode1, stores the record in Instance, installs cInstanceNode_vtbl and makes
 * this node the record's active node (cInstanceNode_setActiveNode). Unless the record flags have bit 0x100 or the type is 6, 0x10 or
 * 22, it also passes the record to cObjectInterface_addToFocusBox.
 *
 * @param bucket  update bucket
 * @param type  node type (tObjNodeType)
 * @param node  instance record the node drives
 * @return self
 */
extern "C" void* cInstanceNode_cInstanceNode1(cInstanceNode* self, void* bucket, int type, cInstanceNode* node)
{
    cSortObjNode_cSortObjNode1(self, bucket, type);
    self->Instance = (tInstanceRecord*)node;
    self->mVtbl = (void**)cInstanceNode_vtbl;
    cInstanceNode_setActiveNode(self, node);
    if (((int)node->Prev & 0x100) == 0) {
        if (type != 6 && type != 0x10 && type != 22) {
            cObjectInterface_addToFocusBox(node);
        }
    }
    return ((void*)self);
}
#endif


#include "common.h"
#include "object/splinemodifier.h"
#include "object/raillink.h"
#include "object/multisplinemodifier.h"
#include "object/railmodifier.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("object/splinemodifier", cSplineModifier_cSplineModifier);
#ifdef SKIP_ASM
class cStream003595D8 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

struct sSmQuad_95D8 {
    float v[4];
} __attribute__((aligned(16)));

struct sSmFollow_95D8 {
    unsigned int id;        // 0xD8
    int count;              // 0xDC
    void* node;             // 0xE0
    float length;           // 0xE4
};

struct cSplineModifier_95D8 {
    void** vtable;          // 0x0
    char pad_0x4[0x54];
    int field_0x58;         // 0x58
    char pad_0x5c[0x54];
    sSmQuad_95D8 qB0;       // 0xB0
    sSmQuad_95D8 qC0;       // 0xC0
    int field_0xd0;         // 0xD0
    int field_0xd4;         // 0xD4
    sSmFollow_95D8 follow;  // 0xD8
};

extern void* cSplineModifier_vtbl[];
extern sSmQuad_95D8 t4vect_Zero;
extern "C" void cSpline_readFromReplayFrame(void* self, cStream003595D8* stream);

/**
 * Rebuilds a spline modifier from a replay frame: reads its 0x50-byte state and its spline, then resets the
 * cached path values.
 *
 * Stores cSplineModifier's vtable, clears the spline (id -1), reads cSplineModifierState into +0x10 and the
 * spline with cSpline_readFromReplayFrame() (+0xD8), zeroes Dir (+0xB0), DDir (+0xC0), X2 and Y2
 * (+0xD0/+0xD4) and sets RootTransformDirty (+0x58) so the transform is rebuilt.
 *
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" cSplineModifier_95D8* cSplineModifier_cSplineModifier(cSplineModifier* self, cStream003595D8* stream)
{
    self->mVtbl = cSplineModifier_vtbl;
    sSmFollow_95D8* follower = &(*(sSmFollow_95D8*)&self->Spline);
    self->Spline.SplineId = 0xFFFFFFFF;
    self->Spline.Seg = 0;
    self->Spline.SegIndex = 0;
    self->Spline.Length = 0;
    stream->v02((char*)self + 0x10, 0x50);
    cSpline_readFromReplayFrame(follower, stream);
    (*(sSmQuad_95D8*)&self->Dir) = t4vect_Zero;
    (*(sSmQuad_95D8*)&self->DDir) = t4vect_Zero;
    self->RootTransformDirty = 1;
    *(int*)&self->X2 = 0;
    *(int*)&self->Y2 = 0;
    return ((cSplineModifier_95D8*)self);
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cSplineModifier_setSpeed);
#ifdef SKIP_ASM
// PORT: callers declare cSplineModifier_setSpeed(void* self); the body also takes the speed in $f12.
extern "C" void cSplineModifier_setSpeed_impl(void* self, float speed) __asm__("cSplineModifier_setSpeed");
/**
 * Sets the spline modifier's speed: Velocity (+0x48) = speed * 27.7778.
 *
 * 27.7778 is 100000 / 3600, so the input is probably in km/h and Velocity in cm/s. Some callers declare the
 * function without the float, which then reaches it in $f12 (see cSplineModifier_signal()).
 *
 * @param speed  the new speed, probably in km/h
 */
extern "C" void cSplineModifier_setSpeed_impl(void* self, float speed)
{
    *(float*)((char*)self + 0x48) = speed * 27.77777862548828f;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cSplineModifier_update);
#ifdef SKIP_ASM
extern "C" float func_002D1C70();

struct cSplineModifier_9698 {
    char pad_0x0[0x30];
    int mode;               // 0x30
    char pad_0x34[0x8];
    float pos;              // 0x3C
    float accel;            // 0x40
    float accelTime;        // 0x44
    float speed;            // 0x48
    int field_0x4c;
    int stopped;            // 0x50
    int active;             // 0x54
    int dirty;              // 0x58
    char pad_0x5c[0x88];
    float length;           // 0xE4
};

/**
 * Advances a spline modifier along its spline and handles reaching either end by its sequence type.
 *
 * While ForceTime (+0x44) is positive it counts down by the tick period and Force (+0x40) accelerates
 * Velocity (+0x48). Unless stopped (Start +0x54 clear or Complete +0x50 set), OffsetDistance (+0x3C) moves by
 * Velocity * tick and RootTransformDirty (+0x58) is set.
 *
 * Past an end, SequenceType (+0x30) 0 and 4 stop there (at 0 or Length - 0.1; Complete set, Start cleared,
 * Velocity reversed), 2 reverses Velocity and any other type wraps around the spline Length (+0xE4). Type 2
 * reflects the overshoot at the start (-pos), but at the far end its pos - (pos - (Length - 0.1)) comes to
 * Length - 0.1 instead of a reflection.
 */
extern "C" void cSplineModifier_update(cSplineModifier* self)
{
    float zeroF = 0.0f;
    if (zeroF < self->ForceTime) {
        self->ForceTime -= func_002D1C70();
        self->Velocity += self->Force * func_002D1C70();
    }
    if (self->Start == 0) {
        return;
    }
    if (self->Complete != 0) {
        return;
    }
    float pos = self->OffsetDistance + self->Velocity * func_002D1C70();
    self->OffsetDistance = pos;
    if (pos < zeroF) {
        int mode = self->SequenceType;
        if (mode == 0) {
            self->Velocity = -self->Velocity;
            self->OffsetDistance = zeroF;
            self->Complete = 1;
            self->Start = 0;
        } else if (mode == 2) {
            self->Velocity = -self->Velocity;
            self->OffsetDistance = -pos;
        } else if (mode == 4) {
            self->Velocity = -self->Velocity;
            self->OffsetDistance = zeroF;
            self->Complete = 1;
            self->Start = 0;
        } else {
            self->OffsetDistance = pos + self->Spline.Length;
        }
    } else if (self->Spline.Length <= pos) {
        int mode = self->SequenceType;
        if (mode == 0) {
            self->Start = 0;
            self->Complete = 1;
            self->Velocity = -self->Velocity;
            self->OffsetDistance = self->Spline.Length - 0.10000000149011612f;
        } else if (mode == 2) {
            self->Velocity = -self->Velocity;
            self->OffsetDistance = pos - (pos - (self->Spline.Length - 0.10000000149011612f));
        } else if (mode == 4) {
            self->Start = 0;
            self->Complete = 1;
            self->Velocity = -self->Velocity;
            self->OffsetDistance = self->Spline.Length - 0.10000000149011612f;
        } else {
            self->OffsetDistance = pos - self->Spline.Length;
        }
    }
    self->RootTransformDirty = 1;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cSplineModifier_updateRootTransform);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float fsqrt(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize).
static inline t4Vector vu0Normalize(const t4Vector& v)
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

extern "C" float func_0031C228(float);
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void func_00345248(void* follow, float t, void* pos, void* vel, void* acc);


struct sSmV3_9830 {
    float x, y, z;
};

struct sSmM44_9830 {
    float m[4][4];
} __attribute__((aligned(16)));

struct cSplineModifier_9830 {
    char pad_0x0[0x34];
    int mode;               // 0x34
    float angle;            // 0x38
    float pos;              // 0x3C
    char pad_0x40[0x18];
    int dirty;              // 0x58
    char pad_0x5c[0x4];
    sSmM44_9830 mat;        // 0x60
    t4Vector origin;      // 0xA0
    t4Vector dir;         // 0xB0
    t4Vector q;           // 0xC0
    float fD0;              // 0xD0
    float fD4;              // 0xD4
    char follow[0x10];      // 0xD8
};

extern sSmV3_9830 D_004FF160;


static inline float Atan2_9830(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}


// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_9830(sSmM44_9830* d, const sSmM44_9830* a, const sSmM44_9830* b)
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
static inline void vu0CopyMat_9830(sSmM44_9830* d, const sSmM44_9830* s)
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

/**
 * Rebuilds the spline modifier's transform from the spline's position and direction at its current distance.
 *
 * Evaluates the spline at OffsetDistance (+0x3C) into Pos, Dir and DDir (+0xA0, +0xB0, +0xC0) and caches X2 =
 * Dir.x^2 and Y2 = Dir.y^2. OrientType (+0x34) 2 or 3 adds yaw and 1 or 3 adds pitch along Dir; that rotation
 * and Pos form Transform (+0x60), turned by HeadTweak (+0x38) about the up axis (D_004FF160) when it is
 * nonzero. Clears RootTransformDirty (+0x58).
 */
extern "C" void cSplineModifier_updateRootTransform(cSplineModifier_9830* self)
{
    float rx = 0.0f;
    float ry = 0.0f;
    float rz = 0.0f;
    func_00345248(self->follow, self->pos, &self->origin, &self->dir, &self->q);
    self->fD0 = self->dir.x * self->dir.x;
    self->fD4 = self->dir.y * self->dir.y;
    t4Vector n = vu0Normalize(self->dir);
    if ((unsigned)(self->mode - 2) < 2) {
        rz = ry - Atan2_9830(n.y, n.x);
    }
    if (self->mode == 1 || self->mode == 3) {
        float len = fsqrt(self->fD0 + self->fD4);
        ry += Atan2_9830(self->dir.z, len);
    }
    sSmV3_9830 rot;
    rot.x = rx;
    rot.y = ry;
    rot.z = rz;
    sSmV3_9830 pos;
    pos.x = self->origin.x;
    pos.y = self->origin.y;
    pos.z = self->origin.z;
    sSmM44_9830 rotMat;
    sSmM44_9830 copyMat;
    sSmM44_9830 product;
    sSmM44_9830* rootMat = &self->mat;
    float sc[4][2];
    func_0031BE50(&sc[0][0], &sc[0][1], rot.x);
    func_0031BE50(&sc[1][0], &sc[1][1], rot.y);
    func_0031BE50(&sc[2][0], &sc[2][1], rot.z);
    float sx = sc[0][0];
    float sy = sc[1][0];
    float cx = sc[0][1];
    float sz = sc[2][0];
    float cz = sc[2][1];
    float cy = sc[1][1];
    float sxsy = sx * sy;
    float cxsy = cx * sy;
    float cxsz = cx * sz;
    self->mat.m[0][0] = cy * cz;
    self->mat.m[0][1] = -cy * sz;
    self->mat.m[0][2] = sy;
    self->mat.m[0][3] = 0.0f;
    self->mat.m[1][0] = sxsy * cz + cxsz;
    self->mat.m[1][1] = cx * cz - sxsy * sz;
    self->mat.m[1][2] = -sx * cy;
    self->mat.m[1][3] = 0.0f;
    self->mat.m[2][0] = sx * sz - cxsy * cz;
    self->mat.m[2][1] = cxsz * sy + sx * cz;
    self->mat.m[2][2] = cx * cy;
    self->mat.m[2][3] = 0.0f;
    self->mat.m[3][0] = pos.x;
    self->mat.m[3][1] = pos.y;
    self->mat.m[3][2] = pos.z;
    self->mat.m[3][3] = 1.0f;
    char* sub = (char*)self + 0x10;
    if (self->angle != 0.0f) {
        const sSmV3_9830* axis = &D_004FF160;
        func_0031BE50(&sc[3][0], &sc[3][1], *(float*)(sub + 0x28));
        float k = 1.0f - sc[3][1];
        float kx = k * axis->x;
        float ky = k * axis->y;
        float kz = k * axis->z;
        float sX = sc[3][0] * axis->x;
        float sY = sc[3][0] * axis->y;
        float sZ = sc[3][0] * axis->z;
        rotMat.m[0][0] = kx * axis->x + sc[3][1];
        rotMat.m[1][0] = kx * axis->y + sZ;
        rotMat.m[2][0] = kx * axis->z - sY;
        rotMat.m[3][0] = 0.0f;
        rotMat.m[0][1] = ky * axis->x - sZ;
        rotMat.m[1][1] = ky * axis->y + sc[3][1];
        rotMat.m[2][1] = ky * axis->z + sX;
        rotMat.m[3][1] = 0.0f;
        rotMat.m[0][2] = kz * axis->x + sY;
        rotMat.m[1][2] = kz * axis->y - sX;
        rotMat.m[2][2] = kz * axis->z + sc[3][1];
        rotMat.m[3][2] = 0.0f;
        rotMat.m[0][3] = 0.0f;
        rotMat.m[1][3] = 0.0f;
        rotMat.m[2][3] = 0.0f;
        rotMat.m[3][3] = 1.0f;
        sSmM44_9830* tp = &copyMat;
        vu0MulMat_9830(&product, rootMat, &rotMat);
        vu0CopyMat_9830(tp, &product);
        vu0CopyMat_9830(rootMat, tp);
    }
    self->dirty = 0;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cSplineModifier_updateIntersectInfo);
#ifdef SKIP_ASM
struct sSmVec4_9CF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSmMat_9CF8 {
    sSmVec4_9CF8 r[4];
};

struct sSmBody_9CF8 {
    sSmVec4_9CF8 pos;       // 0x00
    sSmVec4_9CF8 f10;       // 0x10
    sSmVec4_9CF8 vel;       // 0x20
    sSmVec4_9CF8 rot;       // 0x30
};

struct cSplineModifier_9CF8 {
    char pad_0x0[0x48];
    float speed;            // 0x48
    int field_0x4c;
    int stopped;            // 0x50
    int active;             // 0x54
    char pad_0x58[0x8];
    sSmMat_9CF8 mat;        // 0x60
    sSmVec4_9CF8 origin;    // 0xA0
    sSmVec4_9CF8 dir;       // 0xB0
    float fC0;              // 0xC0
    float fC4;              // 0xC4
    float fC8;              // 0xC8
    float fCC;              // 0xCC
    float fD0;              // 0xD0
    float fD4;              // 0xD4
};

// PORT: PS2 abs.s asm helper; use fabsf on PC.
static inline float Abs_9CF8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_9CF8(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize).
static inline sSmVec4_9CF8 vu0Normalize_9CF8(const sSmVec4_9CF8& v)
{
    sSmVec4_9CF8 r;
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

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sSmVec4_9CF8 vu0Scale_9CF8(const sSmVec4_9CF8& v, float s)
{
    sSmVec4_9CF8 r;
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

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline sSmVec4_9CF8 vu0MulMat_9CF8(const sSmMat_9CF8* m, const sSmVec4_9CF8& v)
{
    sSmVec4_9CF8 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sSmVec4_9CF8 vu0Sub_9CF8(const sSmVec4_9CF8& a, const sSmVec4_9CF8& b)
{
    sSmVec4_9CF8 r;
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
static inline sSmVec4_9CF8 vu0Add_9CF8(const sSmVec4_9CF8& a, const sSmVec4_9CF8& b)
{
    sSmVec4_9CF8 r;
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
static inline sSmVec4_9CF8 vu0Cross_9CF8(const sSmVec4_9CF8& a, const sSmVec4_9CF8& b)
{
    sSmVec4_9CF8 r;
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddTo_9CF8(sSmVec4_9CF8& a, const sSmVec4_9CF8& b)
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

/**
 * Adds the spline modifier's motion to an intersect info: the velocity at the hit point and the angular
 * velocity.
 *
 * Does nothing unless the modifier is moving (Start set, Complete clear). The linear part is the normalised
 * Dir times Velocity; the angular rate (yaw and pitch rates from Dir, DDir, X2 and Y2, left 0 when |Dir.x| <=
 * 1e-5) is scaled by Velocity and turned into world space by Transform. Ivel gets the linear part plus (Ipos
 * - Pos) x w, and Iangvel gets w.
 *
 * @param body  the intersect info to update (On Tour tIntersectInfo&: Ipos +0x0, Ivel +0x20, Iangvel +0x30)
 */
extern "C" void cSplineModifier_updateIntersectInfo(cSplineModifier* self, sSmBody_9CF8* body)
{
    char* sub = (char*)self + 0x10;
    if (self->Start == 0) {
        return;
    }
    if (self->Complete != 0) {
        return;
    }
    float rateY = 0.0f;
    float rateZ = rateY;
    float dirX = self->Dir.x;
    if (9.999999747378752e-06f < Abs_9CF8(dirX)) {
        float ddirY = self->DDir.y;
        float ddirX = self->DDir.x;
        float selfX2 = self->X2;
        float sum = selfX2 + self->Y2;
        rateZ = -selfX2 / sum;
        rateZ *= ddirY / dirX - self->Dir.y * ddirX / selfX2;
        rateY = (1.0f / Sqrt_9CF8(sum)) * (self->DDir.z - self->Dir.z * (ddirX + ddirY) / sum);
    }
    sSmVec4_9CF8 vel = vu0Scale_9CF8(vu0Normalize_9CF8((*(sSmVec4_9CF8*)&self->Dir)), *(float*)(sub + 0x38));
    sSmVec4_9CF8 w;
    w.x = 0.0f;
    w.z = rateZ * self->Velocity;
    w.y = rateY * self->Velocity;
    w.w = 0.0f;
    w = vu0MulMat_9CF8(&(*(sSmMat_9CF8*)&self->Transform), w);
    sSmVec4_9CF8 relPos = vu0Sub_9CF8(body->pos, (*(sSmVec4_9CF8*)&self->Pos));
    vu0AddTo_9CF8(body->vel, vu0Add_9CF8(vel, vu0Cross_9CF8(relPos, w)));
    vu0AddTo_9CF8(body->rot, w);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/splinemodifier", cSplineModifier_signal);
#ifdef SKIP_ASM
extern "C" void cSplineModifier_setSpeed(void* self);

/**
 * Handles a script signal: 0x190 sets the speed and starts, 0x191 starts, 0x192 reverses and starts.
 *
 * Starting clears Complete (+0x50) and sets Start (+0x54); 0x192 negates Velocity (+0x48). For 0x190 the
 * speed is On Tour's float arg, which arrives in $f12 and passes untouched to cSplineModifier_setSpeed() (the
 * C declares no float parameter). Other signals are ignored.
 *
 * @param msg  signal id (On Tour signal)
 */
extern "C" void cSplineModifier_signal(cSplineModifier* self, int msg)
{
    if (msg == 0x190) {
        cSplineModifier_setSpeed(self);
        self->Complete = 0;
        self->Start = 1;
    } else if (msg == 0x191) {
        self->Complete = 0;
        self->Start = 1;
    } else if (msg == 0x192) {
        self->Velocity = -self->Velocity;
        self->Complete = 0;
        self->Start = 1;
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cSplineModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream00359F30 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void cSpline_writeToReplayFrame(void* self, cStream00359F30* stream);

/**
 * Writes a spline modifier to a replay frame: its 0x50-byte state (+0x10), then its spline (+0xD8).
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cSplineModifier_writeToReplayFrame(cSplineModifier* self, cStream00359F30* stream)
{
    stream->v01((char*)self + 0x10, 0x50);
    cSpline_writeToReplayFrame((char*)self + 0xD8, stream);
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_cMultiSplineModifier);
#ifdef SKIP_ASM
struct sSmQuad4_359F88 {
    float x, y, z, w;
    sSmQuad4_359F88(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

extern void* cMultiSplineModifier_vtbl[];
extern "C" void func_003451C0(void* ref, int id);
extern "C" void cMultiSplineModifier_setSpeed(cMultiSplineModifier* self, float v);
extern "C" void cMultiSplineModifier_allocInstances(cMultiSplineModifier* self);
extern "C" void cMultiSplineModifier_allocNodes(cMultiSplineModifier* self);
extern "C" void cMultiSplineModifier_setupNodes(cMultiSplineModifier* self);
extern "C" void cMultiSplineModifier_setupOverlapSystem(cMultiSplineModifier* self);
extern "C" void* cMultiSplineModifier_updateNodes(cMultiSplineModifier* self);

/**
 * Builds a multi-spline modifier from script data: NumModels copies of an instance spaced along a looping
 * spline.
 *
 * Takes NumModels, Orient, Heading (degrees, stored in radians), DrawSpline and the ARGB colour from @p desc,
 * initialises the spline from SplineId, sets the speed (cMultiSplineModifier_setSpeed, On Tour setSpeed) and starts at 0
 * (Length for a negative Speed) or at a nonzero StartDistance, reset to 0 when outside [0, Length].
 *
 * Then allocates the node arrays and instance copies, copies @p model into them, places them, adds them to
 * the overlap octree, and sets FocusCount (+0x34) to 1 when the model's flags (+0x8) have bit 0x100.
 *
 * @param desc   the multi-spline data (On Tour const tAddMultiSplineData&)
 * @param model  the instance to copy along the spline (On Tour tInstanceRecord* inst)
 * @return @p self
 */
extern "C" void* cMultiSplineModifier_cMultiSplineModifier(cMultiSplineModifier* self, char* desc, char* model)
{
    self->Instance = (tInstanceRecord*)model;
    *(void***)self = cMultiSplineModifier_vtbl;
    self->Spline.SplineId = 0xFFFFFFFF;
    self->Spline.Seg = 0;
    self->Spline.SegIndex = 0;
    *(int*)&self->Spline.Length = 0;
    *(int*)&self->Radius = 0;
    self->NumModels = *(int*)(desc + 0x2C);
    self->OrientType = *(int*)(desc + 0x8);
    self->HeadTweak = *(float*)(desc + 0x10) * 0.01745329424738884f;
    func_003451C0(((char*)self) + 0x48, *(int*)(desc + 0x4));
    cMultiSplineModifier_setSpeed(self, *(float*)(desc + 0xC));
    if (*(float*)(desc + 0xC) >= 0.0f) {
        self->OffsetDistance = 0.0f;
    } else {
        self->OffsetDistance = self->Spline.Length;
    }
    self->DrawPath = *(int*)(desc + 0x28) != 0;
    float zeroF = 0.0f;
    *(sSmQuad4_359F88*)&self->Colour = sSmQuad4_359F88(*(float*)(desc + 0x18), *(float*)(desc + 0x1C),
                                                       *(float*)(desc + 0x20), *(float*)(desc + 0x24));
    if (*(float*)(desc + 0x14) != zeroF) {
        self->OffsetDistance = *(float*)(desc + 0x14);
    }
    if (self->OffsetDistance < zeroF || self->Spline.Length < self->OffsetDistance) {
        self->OffsetDistance = zeroF;
    }
    cMultiSplineModifier_allocNodes(self);
    cMultiSplineModifier_allocInstances(self);
    cMultiSplineModifier_setupNodes(self);
    cMultiSplineModifier_updateNodes(self);
    cMultiSplineModifier_setupOverlapSystem(self);
    if (*(int*)((char*)self->Instance + 0x8) & 0x100) {
        self->FocusCount = 1;
    } else {
        self->FocusCount = 0;
    }
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_cMultiSplineModifier1);
#ifdef SKIP_ASM
class cStream0035A118 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual int v03();
};

struct sSmFollow_A118 {
    unsigned int id;        // 0x48
    int count;              // 0x4C
    void* node;             // 0x50
    float length;           // 0x54
};

struct cMultiSplineModifier_A118 {
    void** vtable;          // 0x0
    int count;              // 0x4
    char pad_0x8[0x28];
    int field_0x30;         // 0x30
    char pad_0x34[0xC];
    int field_0x40;         // 0x40
    void** insts;           // 0x44
    sSmFollow_A118 follow;  // 0x48
};

extern void* cMultiSplineModifier_vtbl[];
// PORT: same symbol as the unit's cSpline_readFromReplayFrame(void*, cStream003595D8*).
extern "C" void cSpline_readFromReplayFrame_A118(void* self, cStream0035A118* stream) __asm__("cSpline_readFromReplayFrame");
extern "C" void cMultiSplineModifier_allocNodes(cMultiSplineModifier* self);
extern "C" void cMultiSplineModifier_setupNodes(cMultiSplineModifier* self);
extern "C" void cMultiSplineModifier_setupOverlapSystem(cMultiSplineModifier* self);
void* cObjectInterface_getInstanceMan();
extern "C" void* cInstanceMan_allocInstance(void* man, unsigned int id);

/**
 * Rebuilds a multi-spline modifier from a replay frame, re-creating its instance copies by id.
 *
 * Reads the 0x34-byte state (+0x4), the spline (+0x48) and the model instance (readReplayInstance, +0x40),
 * allocates the node arrays, then reads one id per copy and allocates that record with
 * cInstanceMan_allocInstance(). Copies the model into them, re-adds them to the overlap octree and sets
 * RootTransformDirty (+0x30).
 *
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" cMultiSplineModifier_A118* cMultiSplineModifier_cMultiSplineModifier1(cMultiSplineModifier* self, cStream0035A118* stream)
{
    self->mVtbl = cMultiSplineModifier_vtbl;
    sSmFollow_A118* follower = &(*(sSmFollow_A118*)&self->Spline);
    self->Spline.SplineId = 0xFFFFFFFF;
    self->Spline.Seg = 0;
    self->Spline.SegIndex = 0;
    self->Spline.Length = 0;
    stream->v02((char*)self + 0x4, 0x34);
    cSpline_readFromReplayFrame_A118(follower, stream);
    self->Instance = (tInstanceRecord*)stream->v03();
    cMultiSplineModifier_allocNodes(self);
    unsigned int id = 0xFFFFFFFF;
    for (int i = 0; i < self->NumModels; i++) {
        stream->v02(&id, 4);
        (void**)self->NodeInst[i] = cInstanceMan_allocInstance(cObjectInterface_getInstanceMan(), id);
    }
    cMultiSplineModifier_setupNodes(self);
    cMultiSplineModifier_setupOverlapSystem(self);
    self->RootTransformDirty = 1;
    return ((cMultiSplineModifier_A118*)self);
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier__cMultiSplineModifier);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
void* cObjectInterface_getInstanceMan();
void func_00351260(void* man, void* inst);
extern "C" void func_0035A780(cMultiSplineModifier* self);
extern void* cMultiSplineModifier_vtbl[];
extern void* cTransformModifier_vtbl[];

struct cMultiSplineModifier_A250 {
    void** vtable;          // 0x0
    int count;              // 0x4
    char pad_0x8[0x30];
    void* nodes;            // 0x38
    void* data;             // 0x3C
    int field_0x40;
    void** insts;           // 0x44
};

/**
 * Destroys a multi-spline modifier: takes its copies out of the overlap octree and frees its arrays and
 * instance records.
 *
 * func_0035A780 removes copies 1 to n-1 from the Nash octree; AnimMats (+0x38) and Nodes (+0x3C) are freed,
 * each copy's record goes back on the instance manager's free list (func_00351260, probably On Tour
 * freeInstance), NodeInst (+0x44) is freed and the base cTransformModifier vtable (cTransformModifier_vtbl) is restored.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cMultiSplineModifier__cMultiSplineModifier(cMultiSplineModifier* self, int flags)
{
    self->mVtbl = cMultiSplineModifier_vtbl;
    func_0035A780(self);
    if (self->AnimMats != 0) {
        cMemMan_free(self->AnimMats);
    }
    if (self->Nodes != 0) {
        cMemMan_free(self->Nodes);
    }
    for (int i = 0; i < self->NumModels; i++) {
        func_00351260(cObjectInterface_getInstanceMan(), (void**)self->NodeInst[i]);
    }
    if ((void**)self->NodeInst != 0) {
        cMemMan_free((void**)self->NodeInst);
    }
    self->mVtbl = cTransformModifier_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_allocNodes);
#ifdef SKIP_ASM
// PORT: operator_new__FUi is the game's tagged allocator (size, tag, flags, d); bound by asm label
// as operator new[] so the array new-expressions below match.
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char str_MultiAnimMats[];
extern char str_MultiNodes[];
extern char str_NodeInsts[];

struct sSplineNode_A330 {
    char data[0x40];
    sSplineNode_A330() {}
};

struct sSplineSeg_A330 {
    char data[0x60];
    sSplineSeg_A330() {}
};

struct sMultiSpline_A330 {
    int f0;
    int count;                  // 0x4
    char pad8[0x38 - 0x8];
    sSplineNode_A330* nodes;    // 0x38
    sSplineSeg_A330* segs;      // 0x3C
    char* owner;                // 0x40
    int* indices;               // 0x44
};

/**
 * Allocates a multi-spline modifier's arrays: shared animation matrices, per-copy nodes and per-copy instance
 * pointers.
 *
 * AnimMats (+0x38) gets one 0x40-byte matrix per mesh of the model instance's model ("MultiAnimMats"), Nodes
 * (+0x3C) one 0x60-byte node per copy ("MultiNodes", flags 0xA0000000) and NodeInst (+0x44) one pointer per
 * copy ("NodeInsts").
 *
 * @param selfv  the multi-spline modifier (this)
 */
extern "C" void cMultiSplineModifier_allocNodes(cMultiSplineModifier* selfv)
{
    sMultiSpline_A330* self = (sMultiSpline_A330*)selfv;
    sSplineNode_A330** pn = &self->nodes;
    *pn = new (str_MultiAnimMats, 0x20000000, 0) sSplineNode_A330[*(int*)(*(char**)(self->owner + 0x80) + 0x4)];
    sSplineSeg_A330** ps = &self->segs;
    *ps = new (str_MultiNodes, 0xA0000000, 0) sSplineSeg_A330[self->count];
    self->indices = new (str_NodeInsts, 0x20000000, 0) int[self->count];
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_allocInstances);
#ifdef SKIP_ASM
void* cObjectInterface_getInstanceMan();
extern "C" void* func_00351170(void* man);

/**
 * Allocates one instance record from the instance manager for each node of a multi-spline modifier.
 *
 * Fills the array at +0x44 (On Tour NodeInst) with func_00351170() (On Tour cInstanceMan::allocInstance)
 * results, one for each of the +0x4 nodes.
 */
extern "C" void cMultiSplineModifier_allocInstances(cMultiSplineModifier* self)
{
    int i;
    for (i = 0; i < self->NumModels; i++) {
        ((void**)self->NodeInst)[i] = func_00351170(cObjectInterface_getInstanceMan());
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_setupNodes);
#ifdef SKIP_ASM
struct sInstanceStruct;
void* cObjectInterface_getInstanceMan();
void cInstanceMan_copyInstance(void* man, sInstanceStruct* inst, void* tmpl);
extern "C" void MEM_copy(void* dst, void* src, int n);

struct sSplineInstA458 {
    char pad_0x0[0x8];
    unsigned int flags;     // 0x8
    char pad_0xC[0x6C];
    int field_0x78;         // 0x78
};

struct sMultiSplineA458 {
    char pad_0x0[0x4];
    int count;                      // 0x4
    char pad_0x8[0x38];
    void* tmpl;                     // 0x40
    sSplineInstA458** insts;        // 0x44
};

/**
 * Fills a multi-spline modifier's instance copies with the data of its model instance.
 *
 * Copy 0 gets the model record's first 0xA0 bytes but keeps its own word at +0x78; the other copies go
 * through cInstanceMan_copyInstance() and then have flag 0x40 cleared, 0x20 set and 0x100 cleared.
 *
 * @param multiSpline  the multi-spline modifier (this)
 */
extern "C" void cMultiSplineModifier_setupNodes(cMultiSplineModifier* multiSpline)
{
    sMultiSplineA458* self = (sMultiSplineA458*)multiSpline;
    int i;
    int keep = self->insts[0]->field_0x78;
    MEM_copy(self->insts[0], self->tmpl, 0xA0);
    self->insts[0]->field_0x78 = keep;
    for (i = 1; i < self->count; i++) {
        cInstanceMan_copyInstance(cObjectInterface_getInstanceMan(), (sInstanceStruct*)self->insts[i], self->tmpl);
        self->insts[i]->flags = (self->insts[i]->flags & ~0x40) | 0x20;
        self->insts[i]->flags &= ~0x100;
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_setSpeed);
#ifdef SKIP_ASM
/**
 * Sets the multi-spline's Velocity to @p speed * 27.7777786 (1000 / 36, as from km/h).
 *
 * @param speed  the new speed
 */
extern "C" void cMultiSplineModifier_setSpeed(cMultiSplineModifier* self, float speed)
{
    self->Velocity = speed * 27.77777862548828f;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_update);
#ifdef SKIP_ASM
extern "C" float func_002D1C70();

/**
 * Advances a multi-spline modifier's distance along its looping spline and marks its transforms dirty.
 *
 * OffsetDistance (+0x10) moves by Velocity (+0x14) times the tick period and wraps into [0, Length) (+0x54);
 * RootTransformDirty (+0x30) is set.
 */
extern "C" void cMultiSplineModifier_update(cMultiSplineModifier* self)
{
    float newOffset = self->OffsetDistance + self->Velocity * func_002D1C70();
    self->OffsetDistance = newOffset;
    if (newOffset < 0.0f) {
        self->OffsetDistance = newOffset + self->Spline.Length;
    } else if (newOffset >= self->Spline.Length) {
        self->OffsetDistance = newOffset - self->Spline.Length;
    }
    self->RootTransformDirty = 1;
}
#endif

extern "C" void* cMultiSplineModifier_updateNodes(cMultiSplineModifier* self);

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_updateRootTransform__FPv);
#ifdef SKIP_ASM
/**
 * Updates the multi-spline modifier's root transform by updating its nodes.
 *
 * @return the result of cMultiSplineModifier_updateNodes()
 */
void* cMultiSplineModifier_updateRootTransform(void* self)
{
    return cMultiSplineModifier_updateNodes((cMultiSplineModifier*)self);
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_setupOverlapSystem);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// An axis-aligned bounding box (min and max corners).
struct sBoundBox { t4Vector min, max; };

// One 0x60-byte multi-spline element: its bounding box at 0x30.
struct sSmElem {
    char pad_0x00[0x30];
    sBoundBox box;              // 0x30
    char pad_0x50[0x10];
};

// An octree hash-cell key: depth level and integer cell coordinates.
struct sSmCell { int level; int x, y, z; };

extern "C" void func_00328F28(void* out, const void* box);

extern "C" void func_00328C20(void* root, int idx, void* item, const void* target);

// An overlap-octree node: 2x2x2 children and three item lists.
struct sSmNode {
    sSmNode* child[2][2][2];        // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};

// One of the overlap world's eight octant roots: its cell key and node.
struct sSmRoot {
    sSmCell cell;                   // 0x00
    sSmNode* node;                  // 0x10
};

// The octant root for a cell, chosen by the signs of its coordinates.
static inline sSmRoot* findRoot(char* w, int x, int y, int z)
{
    sSmRoot* r = (sSmRoot*)w;
    sSmRoot* rx = x >= 0 ? r : r + 4;
    sSmRoot* ry = y >= 0 ? rx : rx + 2;
    sSmRoot* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

// The same, taking the cell (gives different code from the x, y, z form; both are used).
static inline sSmRoot* findRoot(char* w, const sSmCell& c)
{
    sSmRoot* r = (sSmRoot*)w;
    sSmRoot* rx = c.x >= 0 ? r : r + 4;
    sSmRoot* ry = c.y >= 0 ? rx : rx + 2;
    sSmRoot* rz = c.z >= 0 ? ry : ry + 1;
    return rz;
}





struct sSmInst_A5F8 {
    char pad_0x00[0x40];
    t4Vector pos;       // 0x40
};

struct sSub_A5F8 {
    int count;              // 0x4
    char pad_0x8[0x10];
    float radius;           // 0x18
};

struct cMultiSpline_A5F8 {
    int pad_0x0;
    sSub_A5F8 sub;          // 0x4
    char pad_0x1c[0x20];
    sSmElem* boxes;     // 0x3C
    char** model;           // 0x40
    sSmInst_A5F8** insts;   // 0x44
};

extern "C" void cInstanceNode_getBoundBoxInfo(void* self, void* box, float* radius);
extern "C" void* cObjectInterface_getNash();

// PORT: PS2-only VU0 inline asm (a - b).
static inline void vu0Sub_A5F8(t4Vector& out, const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    out = r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline void vu0Add_A5F8(t4Vector& out, const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    out = r;
}

static inline t4Vector instPos_A5F8(cMultiSpline_A5F8* self, int i)
{
    return self->insts[i]->pos;
}


static inline void insert_A5F8(char* w, void* item, const void* box)
{
    sSmCell c;
    func_00328F28(&c, box);
    func_00328C20(findRoot(w, c.x, c.y, c.z), 0, item, &c);
}

/**
 * Puts every instance copy but the first into the Nash overlap octree, boxed by the model's radius.
 *
 * Gets the model's bound radius from its node (cInstanceNode_getBoundBoxInfo, kept in Radius +0x18; the box
 * itself is discarded), then gives each copy i >= 1 a box of its position (record +0x40) +/- Radius in
 * Nodes[i] (+0x30) and inserts the copy's record into octree list 0 under it.
 *
 * @param vself  the multi-spline modifier (this)
 */
extern "C" void cMultiSplineModifier_setupOverlapSystem(cMultiSplineModifier* vself)
{
    cMultiSpline_A5F8* self = (cMultiSpline_A5F8*)vself;
    char bound[0x20];
    cInstanceNode_getBoundBoxInfo(((void**)self->model)[3], bound, &self->sub.radius);
    sSub_A5F8* sub = &self->sub;
    for (int i = 1; i < self->sub.count; i++) {
        t4Vector ext;
        ext.x = sub->radius;
        ext.y = sub->radius;
        ext.z = sub->radius;
        ext.w = 0.0f;
        {
            t4Vector corner;
            vu0Sub_A5F8(corner, instPos_A5F8(self, i), ext);
            self->boxes[i].box.min = corner;
            vu0Add_A5F8(corner, instPos_A5F8(self, i), ext);
            self->boxes[i].box.max = corner;
        }
        void* item = self->insts[i];
        char* w = (char*)cObjectInterface_getNash();
        insert_A5F8(w, item, &self->boxes[i].box.min);
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_0035A780);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void* cObjectInterface_getNash();

struct sHashKey_A780 {
    int level;      // 0x0
    int x;          // 0x4
    int y;          // 0x8
    int z;          // 0xC
};

struct sOctCell_A780 {
    void* child[8];
    void* lists[3];
};

struct sHashCellR_A780 {
    char pad_0x0[0x10];
    sOctCell_A780* oct;   // 0x10
};

extern "C" void func_00328F28_A780(sHashKey_A780* out, void* box) __asm__("func_00328F28");
extern "C" void func_003284B8_A780(sOctCell_A780* oct, int type, void* node, sHashKey_A780* key,
                                   sHashCellR_A780* cell) __asm__("func_003284B8");

static inline int octEmpty_A780(sOctCell_A780* o)
{
    if (o->child[0] != 0 || o->child[1] != 0 || o->child[2] != 0 || o->child[3] != 0
        || o->child[4] != 0 || o->child[5] != 0 || o->child[6] != 0 || o->child[7] != 0) {
        return 0;
    }
    for (int i = 0; i < 3; i++) {
        if (o->lists[i] != 0) {
            return 0;
        }
    }
    return 1;
}

// Removes every spline segment but the first from the space hash (freeing octree cells that become empty).
extern "C" void func_0035A780(cMultiSplineModifier* vself)
{
    for (int i = 1; i < vself->NumModels; i++) {
        void* node = ((void**)vself->NodeInst)[i];
        sHashCellR_A780* cell = (sHashCellR_A780*)cObjectInterface_getNash();
        // PORT: the segment array (0x60-byte entries) is indexed with 32-bit integer arithmetic.
        char* box = (char*)(i * 0x60 + (int)vself->Nodes) + 0x30;
        (*(int*)((char*)cell + 0xA0))++;
        sHashKey_A780 key;
        func_00328F28_A780(&key, box);
        sHashCellR_A780* c1 = key.x < 0 ? cell + 4 : cell;
        sHashCellR_A780* c2 = key.y < 0 ? c1 + 2 : c1;
        sHashCellR_A780* c3 = key.z < 0 ? c2 + 1 : c2;
        sHashCellR_A780* c = c3;
        func_003284B8_A780(c->oct, 0, node, &key, c);
        if (octEmpty_A780(c->oct)) {
            operator_delete((int*)c->oct);
            c->oct = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_updateOverlapSystem);
#ifdef SKIP_ASM
struct sSmVec4_A918 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSmBox_A918 {
    sSmVec4_A918 min;       // 0x30
    sSmVec4_A918 max;       // 0x40
};

struct sSmElem_A918 {
    char pad_0x0[0x30];
    sSmBox_A918 box;        // 0x30
    char pad_0x50[0x10];
};

struct sSmVE_A918 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct cMultiSpline_A918 {
    sSmVE_A918* vtable;     // 0x0
    int count;              // 0x4
    char pad_0x8[0x10];
    float radius;           // 0x18
    char pad_0x1c[0x14];
    int dirty;              // 0x30
    char pad_0x34[0x8];
    sSmElem_A918* elems;    // 0x3C
    char* model;            // 0x40
    char** insts;           // 0x44
};

struct sBox_F048;
extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, void* id, sBox_F048* box, sBox_F048* old);

// PORT: PS2-only VU0 inline asm (a - b).
static inline sSmVec4_A918 vu0Sub_A918(const sSmVec4_A918& a, const sSmVec4_A918& b)
{
    sSmVec4_A918 r;
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
static inline sSmVec4_A918 vu0Add_A918(const sSmVec4_A918& a, const sSmVec4_A918& b)
{
    sSmVec4_A918 r;
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

static inline sSmVec4_A918 instPos_A918(cMultiSpline_A918* self, int i)
{
    return *(sSmVec4_A918*)(self->insts[i] + 0x40);
}

struct sSmInst_A918 {
    char pad_0x0[0x40];
    sSmVec4_A918 pos;       // 0x40
    char pad_0x50[0x10];
    float bmin[3];          // 0x60
    float bmax[3];          // 0x6C
};

static inline void setMin_A918(sSmInst_A918* in, const sSmVec4_A918& v)
{
    in->bmin[0] = v.x;
    in->bmin[1] = v.y;
    in->bmin[2] = v.z;
}

static inline void setMax_A918(sSmInst_A918* in, const sSmVec4_A918& v)
{
    in->bmax[0] = v.x;
    in->bmax[1] = v.y;
    in->bmax[2] = v.z;
}

/**
 * Moves every instance copy but the first to its new place in the Nash overlap octree.
 *
 * Refreshes the transforms first when RootTransformDirty (+0x30) is set (virtual updateRootTransform, slot
 * 3). Each copy's box in Nodes (+0x3C, box at +0x30) becomes its position +/- Radius (+0x18);
 * nash_Nash_movenode() moves it from the old box, and the box is also written to the copy's record (+0x60
 * min, +0x6C max).
 */
extern "C" void cMultiSplineModifier_updateOverlapSystem(cMultiSpline_A918* self)
{
    if (self->dirty != 0) {
        self->vtable[3].fn((char*)self + self->vtable[3].delta);
    }
    for (int i = 1; i < self->count; i++) {
        sSmBox_A918 old = self->elems[i].box;
        float halfSize = self->radius;
        sSmVec4_A918 ext;
        ext.x = halfSize;
        ext.y = halfSize;
        ext.z = halfSize;
        ext.w = 0.0f;
        self->elems[i].box.min = vu0Sub_A918(instPos_A918(self, i), ext);
        self->elems[i].box.max = vu0Add_A918(instPos_A918(self, i), ext);
        void* id = self->insts[i];
        nash_Nash_movenode(cObjectInterface_getNash(), 0, id, (sBox_F048*)&self->elems[i].box, (sBox_F048*)&old);
        setMin_A918((sSmInst_A918*)self->insts[i], self->elems[i].box.min);
        setMax_A918((sSmInst_A918*)self->insts[i], self->elems[i].box.max);
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_0035AAD0__FPv);
#ifdef SKIP_ASM
void* func_0035AAD0(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x34) + 0x1;
    *(int*)((char*)self + 0x34) = (int)t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_0035AAE0__FPv);
#ifdef SKIP_ASM
void* func_0035AAE0(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x34) - 0x1;
    *(int*)((char*)self + 0x34) = (int)t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_getRootTransform);
#ifdef SKIP_ASM
struct sSmVEntryV_AAF0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sSmVEntryP_AAF0 {
    short delta;
    short index;
    void* (*fn)(void*);
};

/**
 * Returns the transform matrix of an instance moved by the multi-spline modifier.
 *
 * When the dirty flag (+0x30) is set, first calls the virtual updateRootTransform() (slot 3). For the
 * modifier's own instance (+0x40, On Tour Instance), asks that instance's node (+0xC) through its virtual slot
 * 24, which by default returns its instance record + 0x10; any other record gets its own matrix at +0x10.
 *
 * @param item  the instance record (On Tour tInstanceRecord*)
 * @return the transform matrix
 */
extern "C" void* cMultiSplineModifier_getRootTransform(cMultiSplineModifier* self, void* item)
{
    if (self->RootTransformDirty != 0) {
        sSmVEntryV_AAF0* vt = *(sSmVEntryV_AAF0**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    if (item == (void*)self->Instance) {
        void* obj = *(void**)((char*)item + 0xC);
        sSmVEntryP_AAF0* vt2 = *(sSmVEntryP_AAF0**)((char*)obj + 0xC);
        return vt2[24].fn((char*)obj + vt2[24].delta);
    }
    return (char*)item + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_getAnimMats);
#ifdef SKIP_ASM
struct sSmVEntryI_AB60 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sSmVEntryV_AB60 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Returns the animation matrices to draw an instance with: the model's own, or ones built for a copy.
 *
 * For the model instance (+0x40) it asks the instance's node (+0xC): getAnimMats (slot 27) when hasAnimMats
 * (slot 26) says so, else 0. For a copy, the model's node builds them with makeAnimMats (slot 33) from the
 * copy's root transform (record +0x10) into the shared AnimMats buffer (+0x38).
 *
 * @param item  the instance record to draw (On Tour tInstanceRecord* inst)
 * @return the animation matrices, or 0 when the model instance has none
 */
extern "C" int cMultiSplineModifier_getAnimMats(cMultiSplineModifier* self, void* item)
{
    void* cur = self->Instance;
    if (item == cur) {
        void* obj = *(void**)((char*)item + 0xC);
        sSmVEntryI_AB60* vtEntry = &(*(sSmVEntryI_AB60**)((char*)obj + 0xC))[26];
        if (vtEntry->fn((char*)obj + vtEntry->delta) != 0) {
            obj = *(void**)((char*)self->Instance + 0xC);
            vtEntry = &(*(sSmVEntryI_AB60**)((char*)obj + 0xC))[27];
            return vtEntry->fn((char*)obj + vtEntry->delta);
        }
        return 0;
    }
    void* obj = *(void**)((char*)cur + 0xC);
    sSmVEntryV_AB60* vt = *(sSmVEntryV_AB60**)((char*)obj + 0xC);
    vt[33].fn((char*)obj + vt[33].delta, (char*)item + 0x10, (int)self->AnimMats);
    return (int)self->AnimMats;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_postUpdate);
#ifdef SKIP_ASM
class cSplineVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
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
    virtual int v51();
};

/**
 * Lets the model instance's node refresh its root transform after the update.
 *
 * Calls virtual slot 51 (updateRootTransform in cMoveNode's vtable) of the node of the model instance (+0x40,
 * node at +0xC).
 *
 * @return no meaningful value (the virtual returns void; On Tour's postUpdate returns nothing)
 */
extern "C" int cMultiSplineModifier_postUpdate(cMultiSplineModifier* self)
{
    cSplineVirt* splineObj = *(cSplineVirt**)((char*)self->Instance + 0xC);
    return splineObj->v51();
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_updateNodes);
#ifdef SKIP_ASM
struct sSmVec4_AC20 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSmMat_AC20 {
    sSmVec4_AC20 r[4];
    sSmMat_AC20() {}
    void setAxisRot(const sSmVec4_AC20& axis, float angle);
    void setCols(const float& a00, const float& a10, const float& a20, const float& a30,
                 const float& a01, const float& a11, const float& a21, const float& a31,
                 const float& a02, const float& a12, const float& a22, const float& a32,
                 const float& a03, const float& a13, const float& a23, const float& a33);
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sSmMat_AC20(const sSmMat_AC20& o)
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
            : "r"(this), "r"(&o)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sSmMat_AC20& operator=(const sSmMat_AC20& o)
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
            : "r"(this), "r"(&o)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

extern sSmVec4_AC20 D_004FF160_AC20 __asm__("D_004FF160");
extern void func_00345248_AC20(void* out, sSmVec4_AC20* pos, sSmVec4_AC20* dir, sSmVec4_AC20* c, float t) __asm__("func_00345248");
extern "C" float func_0031C228(float);
extern "C" void func_0031BE50(float* s, float* c, float angle);

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_AC20(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize).
static inline sSmVec4_AC20 vu0Normalize_AC20(const sSmVec4_AC20& v)
{
    sSmVec4_AC20 r;
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

// PORT: PS2-only VU0 inline asm (4x4 matrix * matrix).
static inline sSmMat_AC20 mulMat_AC20(const sSmMat_AC20& a, const sSmMat_AC20& b)
{
    sSmMat_AC20 r;
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
        : "r"(&r), "r"(&a), "r"(&b)
        : "memory");
    return r;
}

static inline float atan2_AC20(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (0.0f <= y)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f) {
        if (0.0f < y)
            r += 3.1415927410125732f;
        else
            r -= 3.1415927410125732f;
    }
    return r;
}

// Fills the matrix column by column.
inline void sSmMat_AC20::setCols(const float& a00, const float& a10, const float& a20, const float& a30,
                                 const float& a01, const float& a11, const float& a21, const float& a31,
                                 const float& a02, const float& a12, const float& a22, const float& a32,
                                 const float& a03, const float& a13, const float& a23, const float& a33)
{
    r[0].x = a00;
    r[1].x = a10;
    r[2].x = a20;
    r[3].x = a30;
    r[0].y = a01;
    r[1].y = a11;
    r[2].y = a21;
    r[3].y = a31;
    r[0].z = a02;
    r[1].z = a12;
    r[2].z = a22;
    r[3].z = a32;
    r[0].w = a03;
    r[1].w = a13;
    r[2].w = a23;
    r[3].w = a33;
}

inline void sSmMat_AC20::setAxisRot(const sSmVec4_AC20& axis, float angle)
{
    float s, c;
    func_0031BE50(&s, &c, angle);
    float t = 1.0f - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    setCols(tx * x + c, tx * y + sz, tx * z - sy, 0.0f,
            ty * x - sz, ty * y + c, ty * z + sx, 0.0f,
            tz * x + sy, tz * y - sx, tz * z + c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
}



struct sSplinePt_AC20 {
    sSmVec4_AC20 pos;       // 0x00
    sSmVec4_AC20 dir;       // 0x10
    sSmVec4_AC20 f20;       // 0x20
    char pad_0x30[0x20];
    float f50;              // 0x50
    float f54;              // 0x54
    char pad_0x58[0x8];
};

struct sSplineInst_AC20 {
    char pad_0x0[0x10];
    sSmMat_AC20 mat;        // 0x10
};

struct cMultiSplineModifier_AC20 {
    void** vtable;                  // 0x0
    int count;                      // 0x4
    int mode;                       // 0x8
    float angle;                    // 0xC
    float pos;                      // 0x10
    char pad_0x14[0x1C];
    int dirty;                      // 0x30
    char pad_0x34[0x8];
    sSplinePt_AC20* points;         // 0x3C
    void* tmpl;                     // 0x40
    sSplineInst_AC20** insts;       // 0x44
    char follow[0xC];               // 0x48
    float length;                   // 0x54
};

/**
 * Places every instance copy along the spline: evaluates it at evenly spaced distances and writes each copy's
 * root transform.
 *
 * Distances start at OffsetDistance (+0x10), step by Length / NumModels and wrap into [0, Length). Each Nodes
 * entry (+0x3C) gets the spline's position, direction and DDir plus Dir.x^2 and Dir.y^2. OrientType (+0x8) 2
 * or 3 adds yaw and 1 or 3 pitch; with the position this forms the copy's record matrix (+0x10), turned by
 * HeadTweak (+0xC) about the up axis when nonzero. Clears RootTransformDirty (+0x30).
 *
 * @param multiSpline  the multi-spline modifier (this)
 * @return no meaningful value (the C body has no return statement; On Tour's updateNodes returns void)
 */
extern "C" void* cMultiSplineModifier_updateNodes(cMultiSplineModifier* multiSpline)
{
    cMultiSplineModifier_AC20* self = (cMultiSplineModifier_AC20*)multiSpline;
    float offsets[256];
    sSmMat_AC20 rot;
    sSmVec4_AC20 ang;
    int i;
    float step = self->length / (float)self->count;
    offsets[0] = self->pos;
    for (int k = 1; k < self->count; k++) {
        offsets[k] = offsets[k - 1] + step;
    }
    char* sub = (char*)self + 0x4;
    for (int k = 0; k < self->count; k++) {
        if (offsets[k] < 0.0f) {
            offsets[k] = offsets[k] + self->length;
        } else if (offsets[k] >= self->length) {
            offsets[k] = offsets[k] - self->length;
        }
    }
    if (self->angle != 0.0f) {
        rot.setAxisRot(D_004FF160_AC20, *(float*)(sub + 0x8));
    }
    sSplinePt_AC20* pt = self->points;
    for (i = 0; i < self->count; i++, pt++) {
        float rx = 0.0f;
        float ry = rx;
        float rz = rx;
        func_00345248_AC20((char*)self + 0x48, &pt->pos, &pt->dir, &pt->f20, offsets[i]);
        pt->f50 = pt->dir.x * pt->dir.x;
        pt->f54 = pt->dir.y * pt->dir.y;
        if (self->mode == 2 || self->mode == 3) {
            rz -= atan2_AC20(pt->dir.y, pt->dir.x);
        }
        if (self->mode == 1 || self->mode == 3) {
            ry += atan2_AC20(pt->dir.z, Sqrt_AC20(pt->f50 + pt->f54));
        }
        ang.x = rx;
        ang.y = ry;
        ang.z = rz;
        sSplineInst_AC20* node = self->insts[i];
        float sx, cx, sy, cy, sz, cz;
        func_0031BE50(&sx, &cx, ang.x);
        sSmMat_AC20* nodeMat = &node->mat;
        func_0031BE50(&sy, &cy, ang.y);
        func_0031BE50(&sz, &cz, ang.z);
        float sxsy = sx * sy;
        float cxsz = cx * sz;
        node->mat.r[0].x = cy * cz;
        node->mat.r[0].y = -cy * sz;
        node->mat.r[0].z = sy;
        node->mat.r[0].w = 0.0f;
        node->mat.r[1].x = sxsy * cz + cxsz;
        node->mat.r[1].y = cx * cz - sxsy * sz;
        node->mat.r[1].z = -sx * cy;
        node->mat.r[1].w = 0.0f;
        node->mat.r[2].x = sx * sz - cx * sy * cz;
        node->mat.r[2].y = cxsz * sy + sx * cz;
        node->mat.r[2].z = cx * cy;
        node->mat.r[2].w = 0.0f;
        node->mat.r[3].x = pt->pos.x;
        node->mat.r[3].y = pt->pos.y;
        node->mat.r[3].z = pt->pos.z;
        node->mat.r[3].w = 1.0f;
        if (self->angle != 0.0f) {
            *nodeMat = mulMat_AC20(*nodeMat, rot);
        }
    }
    self->dirty = 0;
}
#endif

INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_updateIntersectInfo);

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_render);
#ifdef SKIP_ASM
struct sSmVec4_35B418 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSmBox_35B418 {
    char pad_0x0[0x30];
    sSmVec4_35B418 min;     // 0x30
    sSmVec4_35B418 max;     // 0x40
    char pad_0x50[0x10];
};

struct sSmVE_35B418 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sSmMat_35B418 {
    sSmVec4_35B418 r[4];
};

struct sSmVE43_35B418 {
    short delta;
    short index;
    sSmMat_35B418 (*fn)(void*);
};
struct sSmVE93_35B418 {
    short delta;
    short index;
    int (*fn)(void*, sSmVec4_35B418*, sSmVec4_35B418*, const sSmMat_35B418&);
};
struct sSmVEModel_35B418 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct cMultiSpline_35B418 {
    sSmVE_35B418* vtable;   // 0x0
    int count;              // 0x4
    char pad_0x8[0x14];
    int follow;             // 0x1C
    char pad_0x20[0x10];
    int dirty;              // 0x30
    char pad_0x34[0x4];
    int mode;               // 0x38
    sSmBox_35B418* boxes;   // 0x3C
    char* model;            // 0x40
    char** insts;           // 0x44
};

extern "C" void func_00345430(void* a, void* b);

/**
 * Draws every instance copy but the first that passes the view test, then the spline path when DrawPath is
 * set.
 *
 * Refreshes the transforms first when dirty (slot 3). Each copy's box (Nodes +0x3C, at +0x30) goes through
 * the graphics manager's box test (slot 93, with slot 43's matrix): 1 (culled) is skipped; otherwise the
 * model's node builds the copy's matrices (slot 33, into AnimMats +0x38) and draws them (slot 5) with flags
 * 0x420 when the box needs clipping (2) or 0x400 when inside (0). DrawPath (+0x1C) draws the spline in Colour
 * (+0x20) through func_00345430.
 */
extern "C" void cMultiSplineModifier_render(cMultiSpline_35B418* self)
{
    if (self->dirty != 0) {
        self->vtable[3].fn((char*)self + self->vtable[3].delta);
    }
    char* model = *(char**)(self->model + 0xC);
    cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
    for (int i = 1; i < self->count; i++) {
        sSmVE93_35B418* vt = (sSmVE93_35B418*)w->mVtbl;
        char* thisp = (char*)w + vt[93].delta;
        sSmBox_35B418* box = &self->boxes[i];
        sSmVE43_35B418* vt2 = (sSmVE43_35B418*)w->mVtbl;
        int hit = vt[93].fn(thisp, &box->min, &box->max, vt2[43].fn((char*)w + vt2[43].delta));
        if (hit != 1) {
            sSmVEModel_35B418* mvt = *(sSmVEModel_35B418**)(model + 0xC);
            mvt[33].fn(model + mvt[33].delta, self->insts[i] + 0x10, self->mode);
            if (hit != 0) {
                sSmVEModel_35B418* mvt2 = *(sSmVEModel_35B418**)(model + 0xC);
                mvt2[5].fn(model + mvt2[5].delta, (void*)self->mode, 0x420);
            } else {
                sSmVEModel_35B418* mvt2 = *(sSmVEModel_35B418**)(model + 0xC);
                mvt2[5].fn(model + mvt2[5].delta, (void*)self->mode, 0x400);
            }
        }
    }
    if (self->follow != 0) {
        func_00345430((char*)self + 0x48, (char*)self + 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream0035B5A8 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
    virtual void v03();
    virtual void v04();
    virtual void v05(void* obj);
};

class cStream00359F30;
extern "C" void cSpline_writeToReplayFrame(void* self, cStream00359F30* stream);

/**
 * Writes a multi-spline modifier to a replay frame: its state, spline, model instance and each copy's id.
 *
 * Writes the 0x34-byte state (+0x4), the spline (+0x48), the model instance through the frame's
 * writeReplayInstance (slot 5) and, per copy, the word at +0x78 of its record, which the replay constructor
 * reads back as the id to allocate.
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cMultiSplineModifier_writeToReplayFrame(cMultiSplineModifier* self, cStream0035B5A8* stream)
{
    stream->v01((char*)self + 0x4, 0x34);
    cSpline_writeToReplayFrame((char*)self + 0x48, (cStream00359F30*)stream);
    stream->v05(self->Instance);
    for (int i = 0; i < self->NumModels; i++) {
        stream->v01(((char**)self->NodeInst)[i] + 0x78, 4);
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cRailLink_add);
#ifdef SKIP_ASM
/**
 * Links a rail link into the list right after this one.
 *
 * Sets node->Prev to this and node->Next to this->Next (whose Prev becomes @p node), then this->Next to @p
 * node; called on a list head, it adds at the front.
 *
 * @param node  the link to insert (On Tour cRailLink* effect)
 */
extern "C" void cRailLink_add(cRailLink* self, void* node)
{
    void* head = *(void**)self;
    if (head != 0) {
        *(void**)((char*)head + 0x4) = node;
    }
    *(void**)((char*)node + 0x4) = ((void*)self);
    *(void**)node = *(void**)self;
    *(void**)self = node;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cRailLink_append);
#ifdef SKIP_ASM
struct sSplineLink {
    sSplineLink* next; // 0x0
    sSplineLink* prev; // 0x4
};

/**
 * Appends a rail link to the end of the chain another link belongs to.
 *
 * Follows next links from @p link to the last node and attaches @p other after it, clearing other->next.
 *
 * @param link   any link in the chain
 * @param other  link to append
 */
extern "C" void cRailLink_append(sSplineLink* link, sSplineLink* other)
{
    while (link->next != 0) {
        link = link->next;
    }
    other->next = 0;
    other->prev = link;
    link->next = other;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_0035B6D0);
#ifdef SKIP_ASM
extern "C" void func_0035B6D0(sSplineLink* link)
{
    if (link->next != 0) {
        link->next->prev = link->prev;
    }
    if (link->prev != 0) {
        link->prev->next = link->next;
    }
    link->next = 0;
    link->prev = 0;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cRailModifier_cRailModifier);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void* cObjectInterface_getNash();
extern "C" void* func_002D1BD8();
extern "C" void func_00328F28(void* out, const void* box);
extern "C" void func_003284B8(void* node, int idx, void* item, const void* target, const void* cur);
extern "C" void func_00328C20(void* root, int idx, void* item, const void* target);
extern char cRailModifier_vtbl[];
extern "C" void cRailModifier_buildXform(void* self);
struct sSmMod_C040;
extern "C" void cRailModifier_updateBoundBox(cRailModifier* self);

struct sSmVec4_B708 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sSmBox_B708 {
    sSmVec4_B708 min;
    sSmVec4_B708 max;
};
struct sSmCell_B708 {
    int level;
    int x, y, z;
};
struct sSmNode_B708 {
    sSmNode_B708* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sSmRoot_B708 {
    sSmCell_B708 cell;              // 0x00
    sSmNode_B708* node;             // 0x10
};
struct sSmSeg_B708 {
    char pad0[0x64];
    sSmSeg_B708* next;              // 0x64
    char pad68[4];
    float minx, miny, minz;         // 0x6C
    float maxx, maxy, maxz;         // 0x78
};
struct sSmTrack_B708 {
    char pad0[0x20];
    int count;                      // 0x20
    sSmSeg_B708* first;             // 0x24
};
struct sSmSet_B708 {
    char pad0[0x44];
    unsigned int* refs;             // 0x44
};
struct sSmWorld_B708 {
    char pad0[0x8];
    sSmSet_B708** sets;             // 0x8
};
struct sSmVEntry_B708 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sSmModelObj_B708 {
    char pad0[0xC];
    sSmVEntry_B708* vt;             // 0xC
};
struct sSmModel_B708 {
    char pad0[0xC];
    sSmModelObj_B708* obj;          // 0xC
    char pad10[0x50];
    float minx, miny, minz;         // 0x60
    float maxx, maxy, maxz;         // 0x6C
};
struct sSmDesc_B708 {
    int f0;
    unsigned int id;                // 0x4
    int f8;                         // 0x8
};

static inline sSmRoot_B708* findRoot_B708(char* w, int x, int y, int z)
{
    sSmRoot_B708* r = (sSmRoot_B708*)w;
    sSmRoot_B708* rx = x >= 0 ? r : r + 4;
    sSmRoot_B708* ry = y >= 0 ? rx : rx + 2;
    sSmRoot_B708* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void remove_B708(char* w, void* item, const sSmBox_B708* box)
{
    (*(int*)(w + 0xA0))++;
    sSmCell_B708 c;
    func_00328F28(&c, box);
    sSmRoot_B708* r = findRoot_B708(w, c.x, c.y, c.z);
    func_003284B8(r->node, 2, item, &c, &r->cell);
    if (r->node->empty()) {
        operator_delete((int*)r->node);
        r->node = 0;
    }
}

static inline void insert_B708(char* w, void* item, const sSmBox_B708* box)
{
    sSmCell_B708 c;
    func_00328F28(&c, box);
    func_00328C20(findRoot_B708(w, c.x, c.y, c.z), 2, item, &c);
}

static inline sSmTrack_B708* toTrack_B708(unsigned int p)
{
    return (sSmTrack_B708*)(p << 2);
}

static inline sSmTrack_B708* refToTrack_B708(unsigned int v)
{
    unsigned int p = v >> 8;
    if (p == 0) {
        return 0;
    }
    return toTrack_B708(p);
}

struct sSmRef_B708 {
    unsigned int id;

    sSmTrack_B708* get()
    {
        sSmSet_B708* set = (*(sSmWorld_B708**)func_002D1BD8())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return refToTrack_B708(set->refs[id >> 8]);
    }
};

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void subEq_B708(sSmVec4_B708& a, const sSmVec4_B708& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (a += b).
static inline void addEq_B708(sSmVec4_B708& a, const sSmVec4_B708& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

static inline sSmBox_B708 getBox_B708(const sSmModel_B708* m)
{
    sSmBox_B708 b;
    b.min.x = m->minx;
    b.min.y = m->miny;
    b.min.z = m->minz;
    b.min.w = 1.0f;
    b.max.x = m->maxx;
    b.max.y = m->maxy;
    b.max.z = m->maxz;
    b.max.w = 1.0f;
    return b;
}

static inline void setBox_B708(char* self, sSmBox_B708 b)
{
    *(sSmBox_B708*)(self + 0x10) = b;
}

static inline void grow_B708(sSmBox_B708& box, float d)
{
    sSmVec4_B708 pad;
    pad.x = d;
    pad.y = d;
    pad.z = d;
    pad.w = 0.0f;
    subEq_B708(box.min, pad);
    addEq_B708(box.max, pad);
}

/**
 * Builds a rail modifier for an instance's rail spline and puts it in the Nash rail-space octree in place of
 * the spline's segments.
 *
 * Takes SplineId and Mesh from @p desc. When the instance's node has no bound box (slot 44, hasBoundBox),
 * BoundBox is the record's box (+0x60 to +0x74) grown by 100. cRailModifier_buildXform() makes the transform
 * and space node, cRailModifier_updateBoundBox() refreshes the box and Node is inserted into octree list 2,
 * from which every segment of the spline is then removed.
 *
 * @param desc   the rail data (On Tour const tAddRailData&: SplineId +0x4, Mesh +0x8)
 * @param model  the instance the rail belongs to (On Tour tInstanceRecord* inst)
 * @return @p self
 */
extern "C" char* cRailModifier_cRailModifier(cRailModifier* self, sSmDesc_B708* desc, sSmModel_B708* model)
{
    sSmBox_B708* box = (sSmBox_B708*)(((char*)self) + 0x10);
    self->SplineId = 0xFFFFFFFF;
    self->mVtbl = (void**)cRailModifier_vtbl;
    self->Next = 0;
    self->Prev = 0;
    self->Instance = (tInstanceRecord*)model;
    self->SplineId = desc->id;
    self->Mesh = desc->f8;
    sSmModelObj_B708* modelObj = model->obj;
    if (modelObj->vt[44].fn((char*)modelObj + modelObj->vt[44].delta) == 0) {
        sSmModel_B708* instModel = (sSmModel_B708*)self->Instance;
        sSmBox_B708 bounds;
        bounds.min.x = instModel->minx;
        bounds.min.y = instModel->miny;
        bounds.min.z = instModel->minz;
        bounds.min.w = 1.0f;
        bounds.max.x = instModel->maxx;
        bounds.max.y = instModel->maxy;
        bounds.max.z = instModel->maxz;
        bounds.max.w = 1.0f;
        setBox_B708(((char*)self), bounds);
        sSmVec4_B708 margin;
        margin.x = 100.0f;
        margin.y = 100.0f;
        margin.z = 100.0f;
        margin.w = 0.0f;
        subEq_B708(((sSmBox_B708*)(((char*)self) + 0x10))->min, margin);
        addEq_B708(((sSmBox_B708*)(((char*)self) + 0x10))->max, margin);
    }
    cRailModifier_buildXform(((char*)self));
    cRailModifier_updateBoundBox(self);
    insert_B708((char*)cObjectInterface_getNash(), self->Node, box);
    sSmTrack_B708* track = ((sSmRef_B708*)(((char*)self) + 0x30))->get();
    sSmSeg_B708* seg = track->first;
    for (int i = 0; i < track->count; i++) {
        sSmBox_B708 bounds;
        bounds.min.x = seg->minx;
        bounds.min.y = seg->miny;
        bounds.min.z = seg->minz;
        bounds.min.w = 1.0f;
        bounds.max.x = seg->maxx;
        bounds.max.y = seg->maxy;
        bounds.max.z = seg->maxz;
        bounds.max.w = 1.0f;
        remove_B708((char*)cObjectInterface_getNash(), seg, &bounds);
        seg = seg->next;
    }
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cRailModifier_cRailModifier1);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void* cObjectInterface_getNash();

struct sSmVec4_BA88 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sSmBox_BA88 {
    sSmVec4_BA88 min;
    sSmVec4_BA88 max;
};
struct sSmCell_BA88 {
    int level;
    int x, y, z;
};
struct sSmNode_BA88 {
    sSmNode_BA88* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sSmRoot_BA88 {
    sSmCell_BA88 cell;              // 0x00
    sSmNode_BA88* node;             // 0x10
};
struct sSmSeg_BA88 {
    char pad0[0x64];
    sSmSeg_BA88* next;              // 0x64
    char pad68[4];
    float minx, miny, minz;         // 0x6C
    float maxx, maxy, maxz;         // 0x78
};
struct sSmTrack_BA88 {
    char pad0[0x20];
    int count;                      // 0x20
    sSmSeg_BA88* first;             // 0x24
};
struct sSmSet_BA88 {
    char pad0[0x44];
    unsigned int* refs;             // 0x44
};
struct sSmWorld_BA88 {
    char pad0[0x8];
    sSmSet_BA88** sets;             // 0x8
};

extern "C" void* func_002D1BD8();
extern "C" void func_00328F28(void* out, const void* box);
extern "C" void func_003284B8(void* node, int idx, void* item, const void* target, const void* cur);
extern "C" void func_00328C20(void* root, int idx, void* item, const void* target);
extern char cRailModifier_vtbl[];
extern "C" void cRailModifier_buildXform(void* self);

class cSmStream_BA88 {
public:
    virtual void v01();
    virtual void v02(void* p, int n);
    virtual int v03();
};

static inline sSmRoot_BA88* findRoot_BA88(char* w, const sSmCell_BA88& c)
{
    sSmRoot_BA88* r = (sSmRoot_BA88*)w;
    sSmRoot_BA88* rx = c.x >= 0 ? r : r + 4;
    sSmRoot_BA88* ry = c.y >= 0 ? rx : rx + 2;
    sSmRoot_BA88* rz = c.z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void remove_BA88(char* w, void* item, const sSmBox_BA88* box)
{
    (*(int*)(w + 0xA0))++;
    sSmCell_BA88 c;
    func_00328F28(&c, box);
    sSmRoot_BA88* r = findRoot_BA88(w, c);
    func_003284B8(r->node, 2, item, &c, &r->cell);
    if (r->node->empty()) {
        operator_delete((int*)r->node);
        r->node = 0;
    }
}

static inline void insert_BA88(char* w, void* item, const sSmBox_BA88* box)
{
    sSmCell_BA88 c;
    func_00328F28(&c, box);
    func_00328C20(findRoot_BA88(w, c), 2, item, &c);
}

static inline sSmTrack_BA88* toTrack_BA88(unsigned int p)
{
    return (sSmTrack_BA88*)(p << 2);
}

static inline sSmTrack_BA88* refToTrack_BA88(unsigned int v)
{
    unsigned int p = v >> 8;
    if (p == 0) {
        return 0;
    }
    return toTrack_BA88(p);
}

struct sSmRef_BA88 {
    unsigned int id;

    sSmTrack_BA88* get()
    {
        sSmSet_BA88* set = (*(sSmWorld_BA88**)func_002D1BD8())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return refToTrack_BA88(set->refs[id >> 8]);
    }
};

/**
 * Rebuilds a rail modifier from a replay frame and puts it back in the Nash rail-space octree in place of its
 * spline's segments.
 *
 * Reads the 0x30-byte state (BoundBox, SplineId, Mesh at +0x10) and the instance (readReplayInstance),
 * rebuilds the transform and space node with cRailModifier_buildXform(), inserts Node under the stored
 * BoundBox (octree list 2) and removes the spline's segments from that list. The box is not recomputed.
 *
 * @param src  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" char* cRailModifier_cRailModifier1(cRailModifier* self, cSmStream_BA88* src)
{
    self->SplineId = 0xFFFFFFFF;
    self->mVtbl = (void**)cRailModifier_vtbl;
    self->Next = 0;
    self->Prev = 0;
    src->v02(((char*)self) + 0x10, 0x30);
    self->Instance = (tInstanceRecord*)src->v03();
    cRailModifier_buildXform(((char*)self));
    insert_BA88((char*)cObjectInterface_getNash(), self->Node, (sSmBox_BA88*)(((char*)self) + 0x10));
    sSmTrack_BA88* track = ((sSmRef_BA88*)(((char*)self) + 0x30))->get();
    sSmSeg_BA88* seg = track->first;
    for (int i = 0; i < track->count; i++) {
        sSmBox_BA88 segBox;
        segBox.min.x = seg->minx;
        segBox.min.y = seg->miny;
        segBox.min.z = seg->minz;
        segBox.min.w = 1.0f;
        segBox.max.x = seg->maxx;
        segBox.max.y = seg->maxy;
        segBox.max.z = seg->maxz;
        segBox.max.w = 1.0f;
        remove_BA88((char*)cObjectInterface_getNash(), seg, &segBox);
        seg = seg->next;
    }
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cRailModifier__cRailModifier);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void* cObjectInterface_getNash();

struct sSmVec4_BD70 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sSmBox_BD70 {
    sSmVec4_BD70 min;
    sSmVec4_BD70 max;
};
struct sSmCell_BD70 {
    int level;
    int x, y, z;
};
struct sSmNode_BD70 {
    sSmNode_BD70* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sSmRoot_BD70 {
    sSmCell_BD70 cell;              // 0x00
    sSmNode_BD70* node;             // 0x10
};
struct sSmSeg_BD70 {
    char pad0[0x64];
    sSmSeg_BD70* next;              // 0x64
    char pad68[4];
    float minx, miny, minz;         // 0x6C
    float maxx, maxy, maxz;         // 0x78
};
struct sSmTrack_BD70 {
    char pad0[0x20];
    int count;                      // 0x20
    sSmSeg_BD70* first;             // 0x24
};
struct sSmSet_BD70 {
    char pad0[0x44];
    unsigned int* refs;             // 0x44
};
struct sSmWorld_BD70 {
    char pad0[0x8];
    sSmSet_BD70** sets;             // 0x8
};

extern "C" void* func_002D1BD8();
extern "C" void func_00328F28(void* out, const void* box);
extern "C" void func_003284B8(void* node, int idx, void* item, const void* target, const void* cur);
extern "C" void func_00328C20(void* root, int idx, void* item, const void* target);
extern char cRailModifier_vtbl[];
extern char cRailLink_vtbl[];

static inline sSmRoot_BD70* findRoot_BD70(char* w, const sSmCell_BD70& c)
{
    sSmRoot_BD70* r = (sSmRoot_BD70*)w;
    sSmRoot_BD70* rx = c.x >= 0 ? r : r + 4;
    sSmRoot_BD70* ry = c.y >= 0 ? rx : rx + 2;
    sSmRoot_BD70* rz = c.z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void remove_BD70(char* w, void* item, const sSmBox_BD70* box)
{
    (*(int*)(w + 0xA0))++;
    sSmCell_BD70 c;
    func_00328F28(&c, box);
    sSmRoot_BD70* r = findRoot_BD70(w, c);
    func_003284B8(r->node, 2, item, &c, &r->cell);
    if (r->node->empty()) {
        operator_delete((int*)r->node);
        r->node = 0;
    }
}

static inline void insert_BD70(char* w, void* item, const sSmBox_BD70* box)
{
    sSmCell_BD70 c;
    func_00328F28(&c, box);
    func_00328C20(findRoot_BD70(w, c), 2, item, &c);
}

static inline sSmTrack_BD70* toTrack_BD70(unsigned int p)
{
    return (sSmTrack_BD70*)(p << 2);
}

static inline sSmTrack_BD70* refToTrack_BD70(unsigned int v)
{
    unsigned int p = v >> 8;
    if (p == 0) {
        return 0;
    }
    return toTrack_BD70(p);
}

struct sSmRef_BD70 {
    unsigned int id;

    sSmTrack_BD70* get()
    {
        sSmSet_BD70* set = (*(sSmWorld_BD70**)func_002D1BD8())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return refToTrack_BD70(set->refs[id >> 8]);
    }
};

/**
 * Destroys a rail modifier: takes its rail-space node out of the Nash octree, re-inserts the spline's segments and
 * unlinks the rail.
 *
 * Removes Node from Nash with the rail's bound box (+0x10) and frees it, then inserts every segment of the rail's
 * spline (the reference at +0x30) back into Nash with its own segment box (w = 1.0). Restores the cRailLink_vtbl
 * base, unlinks the rail from its list (func_0035B6D0, On Tour cRailLink::unlink) and frees the object when bit 0
 * of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRailModifier__cRailModifier(cRailModifier* self, int flags)
{
    self->mVtbl = (void**)cRailModifier_vtbl;
    remove_BD70((char*)cObjectInterface_getNash(), self->Node, (sSmBox_BD70*)(((char*)self) + 0x10));
    operator_delete((int*)self->Node);
    sSmTrack_BD70* spline = ((sSmRef_BD70*)(((char*)self) + 0x30))->get();
    sSmSeg_BD70* seg = spline->first;
    for (int i = 0; i < spline->count; i++) {
        sSmBox_BD70 segBox;
        segBox.min.x = seg->minx;
        segBox.min.y = seg->miny;
        segBox.min.z = seg->minz;
        segBox.min.w = 1.0f;
        segBox.max.x = seg->maxx;
        segBox.max.y = seg->maxy;
        segBox.max.z = seg->maxz;
        segBox.max.w = 1.0f;
        insert_BD70((char*)cObjectInterface_getNash(), seg, &segBox);
        seg = seg->next;
    }
    self->mVtbl = (void**)cRailLink_vtbl;
    func_0035B6D0((sSplineLink*)self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", cRailModifier_updateBoundBox);
#ifdef SKIP_ASM
struct sSmVec4_C040 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSmBox_C040 {
    sSmVec4_C040 min;
    sSmVec4_C040 max;
};

struct sSmVEntryI_C040 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sSmVEntryB_C040 {
    short delta;
    short index;
    sSmBox_C040* (*fn)(void*);
};

struct sSmVtbl_C040 {
    char pad_0x0[0x160];
    sSmVEntryI_C040 v44;    // 0x160
    sSmVEntryB_C040 v45;    // 0x168
};

struct sSmObj_C040 {
    char pad_0x0[0xC];
    sSmVtbl_C040* vt;       // 0xC
};

struct sSmOwner_C040 {
    char pad_0x0[0xC];
    sSmObj_C040* obj;       // 0xC
};


// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (a -= b, 4 floats).
static inline void vecSubEq_C040(sSmVec4_C040& a, const sSmVec4_C040& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (a += b, 4 floats).
static inline void vecAddEq_C040(sSmVec4_C040& a, const sSmVec4_C040& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

/**
 * Refreshes a rail modifier's bound box from its instance node's box, grown by 100 on each side.
 *
 * Only when the instance's node (record +0xC) has a bound box (slot 44, hasBoundBox): BoundBox (+0x10)
 * becomes the node's getBoundBox (slot 45) result minus 100 at min and plus 100 at max. Otherwise the box is
 * left as it is.
 */
extern "C" void cRailModifier_updateBoundBox(cRailModifier* self)
{
    sSmObj_C040* ownerObj = ((sSmOwner_C040*)self->Instance)->obj;
    if (ownerObj->vt->v44.fn((char*)ownerObj + ownerObj->vt->v44.delta) != 0) {
        ownerObj = ((sSmOwner_C040*)self->Instance)->obj;
        sSmBox_C040* bounds = ownerObj->vt->v45.fn((char*)ownerObj + ownerObj->vt->v45.delta);
        (*(sSmBox_C040*)&self->BoundBox) = *bounds;
        sSmVec4_C040 margin;
        margin.x = 100.0f;
        margin.y = 100.0f;
        margin.z = 100.0f;
        margin.w = 0.0f;
        vecSubEq_C040((*(sSmBox_C040*)&self->BoundBox).min, margin);
        vecAddEq_C040((*(sSmBox_C040*)&self->BoundBox).max, margin);
    }
}
#endif


#include "vtable.h"
#include "common.h"
#include "intersect/worldinttypes.h"
#include "world/worldrecords.h"
#include "world/worldcache.h"
#include "render/ps2gridmesh.h"
#include "world/instancerecord.h"
#include "ai/rider.h"
#include "bx/math3d.h"
#include "world/worldview.h"
#include "object/flexbridgenode.h"
#include "object/flexrailnode.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "replay/bxreplayframe.h"
#include "object/instancenode.h"

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_setupGrid);
#ifdef SKIP_ASM
struct sBox00348058;
extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, int id, tBoundBox* box, tBoundBox* old);
void cMemMan_free(void*);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char str_BridgeVerts[];
extern char str_BridgeNorms[];
extern char str_BridgeFaces[];
extern char str_TempBridgeUVs[];
extern char str_TempBridgeRGBs[];

struct sGridV3 {
    float x, y, z;
    sGridV3() {}
    sGridV3(float a, float b, float c) : x(a), y(b), z(c) {}
};
inline sGridV3 operator-(const sGridV3& a, const sGridV3& b) { return sGridV3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline sGridV3 operator+(const sGridV3& a, const sGridV3& b) { return sGridV3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline sGridV3 operator*(const sGridV3& a, const float& s) { return sGridV3(a.x * s, a.y * s, a.z * s); }

struct sGridV2 {
    float x, y;
    sGridV2() {}
    sGridV2(float a, float b) : x(a), y(b) {}
};
inline sGridV2 operator-(const sGridV2& a, const sGridV2& b) { return sGridV2(a.x - b.x, a.y - b.y); }
inline sGridV2 operator+(const sGridV2& a, const sGridV2& b) { return sGridV2(a.x + b.x, a.y + b.y); }
inline sGridV2 operator*(const sGridV2& a, const float& s) { return sGridV2(a.x * s, a.y * s); }

struct sGridV4 {
    float x, y, z, w;
    sGridV4() {}
    sGridV4(const sGridV3& v, const float& ww) : x(v.x), y(v.y), z(v.z), w(ww) {}
    sGridV4(const float& a, const float& b, const float& c, const float& d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

struct sGridCol {
    float a, r, g, b;
    sGridCol() {}
};

struct sGridBox {
    sGridV4 min;
    sGridV4 max;
};

// PORT: PS2-only VU0 inline asm (d = a - b)
static inline void gridVecSub(sGridV4& d, const sGridV4& a, const sGridV4& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(d)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (d = a + b)
static inline void gridVecAdd(sGridV4& d, const sGridV4& a, const sGridV4& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(d)
        : "m"(a), "m"(b)
        : "memory");
}

struct sGridVEntryV {
    short delta;
    short index;
    void (*fn)(...);
};

struct sGridDef {
    char pad_0x00[0x60];
    sGridV3 min;    // 0x60
    sGridV3 max;    // 0x6C
};

struct sGridMesh {
    char pad_0x00[0x4];
    sGridVEntryV* vt;
};

struct sFlexGrid {
    char pad_0x00[0x18];
    sGridDef* def;        // 0x18
    char pad_0x1C[0x50 - 0x1C];
    int count;            // 0x50
    char pad_0x54[0x78 - 0x54];
    sGridMesh* mesh;      // 0x78
    sGridV4* pts[2];      // 0x7C: current/previous points
    unsigned char* idx;   // 0x84
    char pad_0x88[0x90 - 0x88];
    sGridBox box;         // 0x90
};


/**
 * Builds a flexible bridge's strip mesh: two rows of points along the instance's quad, with UVs, colours and
 * faces.
 *
 * Creates a dynamic mesh (graphics manager slot 76) and allocates the vertex ("BridgeVerts"), normal
 * ("BridgeNorms") and face ("BridgeFaces") arrays. The two rows interpolate the instance's four corners, UVs
 * and colours over the spring node count; the colours and UVs go to the mesh (its slot 6). The bounding box
 * is the instance's, grown by 100 on each side and moved in the Nash grid; each segment gets two triangles.
 */
extern "C" void cFlexBridgeNode_setupGrid(sFlexGrid* self)
{
    cPS2GraphicsMan* world = cGraphicsMan_TheGraphicsMan;
    int n = self->count;
    int nverts = n * 2;
    int ntris = (n - 1) * 2;
    self->mesh = (sGridMesh*)world->CreateGridMesh(n, 2, 0x20000000);
    sGridV4*& pts0 = self->pts[0];
    pts0 = new (str_BridgeVerts, 0x20000000, 0) sGridV4[nverts];
    sGridV4*& pts1 = self->pts[1];
    pts1 = new (str_BridgeNorms, 0x20000000, 0) sGridV4[nverts];
    self->idx = new (str_BridgeFaces, 0x20000000, 0) unsigned char[ntris * 3];
    sGridV3 pos[4];
    sGridV2 uvs[4];
    sGridCol cols[4];
    sGridV2* uv = new (str_TempBridgeUVs, 0x100, 0) sGridV2[nverts];
    sGridCol* col = new (str_TempBridgeRGBs, 0x100, 0) sGridCol[nverts];
    world->GetMeshQuad((const tInstanceRecord*)self->def, 0, (t3Vector*)pos, (t2Vector*)uvs, (tARGB*)cols);
    for (int i = 0; i < n; i++) {
        float floatIndex = (float)i;
        float inv = 1.0f / (float)(n - 1);
        self->pts[0][i] = sGridV4(pos[0] + (pos[1] - pos[0]) * floatIndex * inv, 1.0f);
        self->pts[0][n + i] = sGridV4(pos[2] + (pos[3] - pos[2]) * floatIndex * inv, 1.0f);
        uv[i] = uvs[0] + (uvs[1] - uvs[0]) * floatIndex * inv;
        uv[n + i] = uvs[2] + (uvs[3] - uvs[2]) * floatIndex * inv;
        col[i].a = 1.0f;
        col[i].r = cols[0].r + (cols[1].r - cols[0].r) * floatIndex * inv;
        col[i].g = cols[0].g + (cols[1].g - cols[0].g) * floatIndex * inv;
        col[i].b = cols[0].b + (cols[1].b - cols[0].b) * floatIndex * inv;
        col[n + i].a = 1.0f;
        col[n + i].r = cols[2].r + (cols[3].r - cols[2].r) * floatIndex * inv;
        col[n + i].g = cols[2].g + (cols[3].g - cols[2].g) * floatIndex * inv;
        col[n + i].b = cols[2].b + (cols[3].b - cols[2].b) * floatIndex * inv;
    }
    sGridVEntryV* meshVtEntry = &self->mesh->vt[6];
    meshVtEntry->fn(vthis(self->mesh, *meshVtEntry), pos, col, uv);
    if (uv != 0) {
        cMemMan_free(uv);
    }
    if (col != 0) {
        cMemMan_free(col);
    }
    sGridDef* def = self->def;
    self->box.min = sGridV4(def->min.x, def->min.y, def->min.z, 1.0f);
    self->box.max = sGridV4(def->max.x, def->max.y, def->max.z, 1.0f);
    sGridBox old = self->box;
    sGridV4 ext(100.0f, 100.0f, 100.0f, 0.0f);
    gridVecSub(self->box.min, self->box.min, ext);
    gridVecAdd(self->box.max, self->box.max, ext);
    // PORT: the unit declares nash_Nash_movenode's third parameter as int; the def pointer is passed through it
    int id = (int)self->def;
    nash_Nash_movenode(cObjectInterface_getNash(), 0, id, (tBoundBox*)&self->box, (tBoundBox*)&old);
    int baseVertex = 0;
    int j = 0;
    for (int k = 0; k < ntris - 1; k += 2) {
        self->idx[j + 0] = baseVertex;
        self->idx[j + 1] = baseVertex + n;
        self->idx[j + 2] = baseVertex + 1;
        self->idx[j + 3] = baseVertex + n;
        self->idx[j + 4] = baseVertex + n + 1;
        self->idx[j + 5] = baseVertex + 1;
        baseVertex++;
        j += 6;
    }
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_initSpringNodes);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
static inline t4Vector flexVecSub(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
static inline t4Vector flexVecAdd(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v * s).
static inline t4Vector flexVecScale(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

struct sFlexSeg {
    t4Vector pos;       // 0x00
    char pad_0x10[0x40];
};

struct sFlexBridge {
    char pad_0x00[0x20];
    t4Vector end;       // 0x20
    t4Vector start;     // 0x30
    char pad_0x40[0x10];
    int count;           // 0x50
    char pad_0x54[0x20];
    sFlexSeg* segs;      // 0x74
    char pad_0x78[0x4];
    t4Vector* pts;      // 0x7C: 2 * count points
};

/**
 * Places the flex bridge's spring segments and records its end offsets.
 *
 * Each of the count segments goes to the midpoint between pts[i] and pts[count + i]. Then start is set to
 * pts[0] and end to pts[count], both relative to the first segment's position.
 */
extern "C" void cFlexBridgeNode_initSpringNodes(sFlexBridge* self)
{
    int n = self->count;
    sFlexSeg* seg = self->segs;
    for (int i = 0; i < self->count; i++, seg++) {
        seg->pos = flexVecAdd(self->pts[i], flexVecScale(flexVecSub(self->pts[n + i], self->pts[i]), 0.5f));
    }
    seg = self->segs;
    self->start = flexVecSub(self->pts[0], seg->pos);
    self->end = flexVecSub(self->pts[n], seg->pos);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_applyForce);
#ifdef SKIP_ASM






// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_6E38(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_6E38(float s, const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vu0Norm_6E38(const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline t4Vector vu0MulMat_6E38(const t4Matrix* m, const t4Vector& v)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_6E38(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_6E38(t4Vector& dst, t4Vector b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

static inline t4Vector Pos_6E38(const t4Matrix* m)
{
    return m->row[3];
}

static inline t4Vector ToLocal_6E38(const t4Matrix* m, const t4Vector& v)
{
    t4Vector r;
    t4Vector f = vu0Sub_6E38(v, vu0Scale_6E38(v.w, Pos_6E38(m)));
    r.x = m->row[0].x * f.x + m->row[0].y * f.y + m->row[0].z * f.z;
    r.y = m->row[1].x * f.x + m->row[1].y * f.y + m->row[1].z * f.z;
    r.z = m->row[2].x * f.x + m->row[2].y * f.y + m->row[2].z * f.z;
    r.w = v.w;
    return r;
}

static inline float Torque_6E38(float k, const t4Vector& a, const t4Vector& b)
{
    return k * vu0Dot_6E38(a, b);
}

/**
 * Applies a force at a hit point of a flexible bridge to the segment that was hit.
 *
 * The segment is the hit triangle / 2. It gets the force in the bridge's local space times the linear factor,
 * and a torque from the force at the hit vertex times the angular factor, scaled by the hit's offset along
 * the bridge axis. Nothing happens for a hit with no triangle index.
 *
 * @param intInfo  the hit (On Tour tIntersectInfo): position and triangle index
 * @param force    the force to apply
 */
extern "C" void cFlexBridgeNode_applyForce(cFlexBridgeNode* self, tIntersectInfo* intInfo, t4Vector* force)
{
    cFlexBridgeNode* data = self;
    if (intInfo->Face < 0) {
        return;
    }
    const t4Matrix* transform = self->getRootTransform();
    t4Vector relPos = vu0Sub_6E38(intInfo->Ipos, Pos_6E38(transform));
    t4Vector y = vu0MulMat_6E38(transform, vu0Norm_6E38(*(&data->BottomRightOffset)));
    t4Vector n = vu0MulMat_6E38(transform, (data->FaceNorms)[intInfo->Face]);
    float yoffset = vu0Dot_6E38(relPos, y);
    int seg = intInfo->Face / 2;
    tSpringNode* pNode = data->Mesh.Nodes;
    vu0AddEq_6E38(pNode[seg].F, vu0Scale_6E38(data->Mesh.ForceScale, ToLocal_6E38(transform, *force)));
    pNode[seg].T += Torque_6E38(data->Mesh.TorqueScale, n, *force) * yoffset;
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_updateIntersectInfo);
#ifdef SKIP_ASM

extern t4Vector t4vect_XAxis_70D0 __asm__("t4vect_XAxis");

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector vu0Sub_70D0(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vu0Add_70D0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_70D0(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (*v * s).
static inline t4Vector vu0ScaleP_70D0(const t4Vector* v, float s)
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
        : "o"(*v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector).
static inline t4Vector vu0MtxMul_70D0(const t4Vector* m, const t4Vector& v)
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
        : "m"(v), "r"(m));
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector vu0Cross_70D0(const t4Vector& a, const t4Vector& b)
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

struct sFlVE_70D0 {
    short delta;
    short index;
    t4Vector* (*fn)(void*);
};


struct sFlBridge_70D0 {
    char pad_0x00[0xC];
    sFlVE_70D0* vt;     // 0x0C
    char pad_0x10[0x64];
    tSpringNode* ents;  // 0x74
};


/**
 * Adds the motion of the bridge segment that was hit to an intersection's velocities.
 *
 * For the segment (triangle index / 2) it adds the segment's velocity at the hit point to intInfo's v[2] and
 * its angular velocity to v[3], both turned into world space by the root transform (slot 24).
 *
 * @param intInfo  the hit (On Tour tIntersectInfo); nothing happens without a triangle index
 */
extern "C" void cFlexBridgeNode_updateIntersectInfo(cFlexBridgeNode* self, tIntersectInfo* intInfo)
{
    int idx = intInfo->Face;
    if (idx >= 0) {
        int pairIndex = idx / 2;
        tSpringNode* ents = self->Mesh.Nodes;
        t4Vector* matrix = (t4Vector*)self->getRootTransform();
        tSpringNode* entry = (tSpringNode*)(pairIndex * 0x50 + (int)ents);
        t4Vector axisVec = vu0MtxMul_70D0(matrix, vu0ScaleP_70D0(&t4vect_XAxis_70D0, entry->W));
        t4Vector arm = vu0Sub_70D0(((&intInfo->Ipos))[0], vu0MtxMul_70D0(matrix, (*(&entry->P))));
        t4Vector pointVelocity = vu0Add_70D0(vu0MtxMul_70D0(matrix, (*(&entry->V))), vu0Cross_70D0(arm, axisVec));
        vu0AddEq_70D0(((&intInfo->Ipos))[2], pointVelocity);
        vu0AddEq_70D0(((&intInfo->Ipos))[3], axisVec);
    }
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_intersect);
#ifdef SKIP_ASM
// t4Vector with the constructors the original's vector class has (the hit position's scale is built in place).
struct t4VectorCtor : public t4Vector {
    t4VectorCtor() {}
    t4VectorCtor(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

extern t4VectorCtor t4vect_Zero_v7268 __asm__("t4vect_Zero");







typedef t4Matrix* (*GetMtxFn_7268)(void*);
typedef void (*SetXformFn_7268)(void*, t4Matrix*, float);
typedef void (*VoidFn_7268)(void*);

extern "C" int cWorldIntersectType_IsMeshInt(cWorldIntersectType* t, tPolyModelMesh* mesh, int flag, tIntersectInfo* out, int cap);

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void Copy_7268(t4Matrix* d, const t4Matrix* s)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4VectorCtor Scale_7268(float s, const t4VectorCtor& v)
{
    t4VectorCtor r;
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

// PORT: PS2-only VU0 inline asm (m * v).
static inline t4VectorCtor MulVec_7268(const t4Matrix* m, const t4VectorCtor& v)
{
    t4VectorCtor r;
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

static inline t4VectorCtor Pos_7268(const t4Matrix* m)
{
    return *(const t4VectorCtor*)&m->row[3];
}

static inline t4VectorCtor Mul_7268(const t4VectorCtor& a, const t4VectorCtor& b)
{
    return t4VectorCtor(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

static inline void Reset_7268(tIntersectInfo* h)
{
    (*(int*)&h->Patch) = 0;
    (*(void**)&h->Inst) = 0;
    (*(int*)&h->Spline) = 0;
    h->Mesh = -1;
    h->Face = -1;
    h->Seg = -1;
    h->HitType = 2;
    h->Stype = -1;
    (*(t4VectorCtor*)&h->Ivel) = t4vect_Zero_v7268;
    (*(t4VectorCtor*)&h->Iangvel) = t4vect_Zero_v7268;
    (*(int*)&h->T) = 0;
    (*(int*)&h->u) = 0;
    (*(int*)&h->v) = 0;
}

// Orthonormal inverse in place: transpose the 3x3 part, position = R^T * -t.
static inline void TransposeInverse_7268(t4Matrix* t)
{
    float tmp;
    tmp = t->row[0].y; t->row[0].y = t->row[1].x; t->row[1].x = tmp;
    tmp = t->row[0].z; t->row[0].z = t->row[2].x; t->row[2].x = tmp;
    tmp = t->row[1].z; t->row[1].z = t->row[2].y; t->row[2].y = tmp;
    t4VectorCtor p = Scale_7268(-1.0f, Pos_7268(t));
    p.w = 0.0f;
    p = MulVec_7268(t, p);
    p.w = 1.0f;
    *(t4VectorCtor*)&t->row[3] = p;
}

/**
 * Tests a world query against a flexible bridge's current strip and returns the hits.
 *
 * Only runs when bit 1 of @p flags is set. Moves the tester into the bridge's local space (inverse transform
 * and 1 / scale), tests the strip's triangles (cWorldIntersectType_IsMeshInt), then turns each hit's position and normal back
 * into world space and fills in its type (1), owner and other fields.
 *
 * @param tester  the query (On Tour cWorldIntersectType)
 * @param out     receives the hits
 * @param cap     maximum number of hits
 * @param flags   query flags; bit 1 must be set
 * @return the number of hits
 */
extern "C" int cFlexBridgeNode_intersect(cFlexBridgeNode* self, cWorldIntersectType* tester, tIntersectInfo* out, int cap, int flags)
{
    if (!(flags & 2)) {
        return 0;
    }
    tPolyModelMesh mesh;
    mesh.NumFaces = (self->Mesh.Width - 1) * 2;
    mesh.NumVertices = self->Mesh.Width * 2;
    (*(void**)&mesh.Face) = ((void*)self->Faces);
    (*(t4VectorCtor**)&mesh.Vertex) = ((t4VectorCtor*)self->Verts);
    (*(t4VectorCtor**)&mesh.Normal) = ((t4VectorCtor*)self->FaceNorms);
    t4Matrix* matrix = (t4Matrix*)self->getRootTransform();
    t4Matrix inv;
    Copy_7268(&inv, matrix);
    TransposeInverse_7268(&inv);
    tester->Transform(*(const t4Matrix*)&inv, 1.0f / (self->Instance)->m_fScale);
    int n = cWorldIntersectType_IsMeshInt(tester, &mesh, 1, out, cap);
    tester->UnTransform();
    for (int i = 0; i < n; i++) {
        tIntersectInfo* hit = &out[i];
        Reset_7268(hit);
        float dataScale = self->Instance->m_fScale;
        float one = 1.0f;
        (*(t4VectorCtor*)&hit->Ipos) = MulVec_7268(matrix, Mul_7268((*(t4VectorCtor*)&hit->Ipos),
                                                                    t4VectorCtor(dataScale, dataScale, dataScale,
                                                                                 one)));
        (*(t4VectorCtor*)&hit->Inorm) = MulVec_7268(matrix, (*(t4VectorCtor*)&hit->Inorm));
        hit->HitType = 1;
        (*(void**)&hit->Inst) = self->Instance;
        int k = self->Instance->m_Model->m_iNumMeshes - 1;
        (*(int*)&hit->SphTree) = 0;
        hit->Mesh = k;
        (*(t4VectorCtor*)&hit->Ivel) = t4vect_Zero_v7268;
        (*(t4VectorCtor*)&hit->Iangvel) = t4vect_Zero_v7268;
        hit->Stype = *(short*)(((char*)(self->Instance)->m_pCollideRecord) + k * 12 + 0x1A);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_update);
#ifdef SKIP_ASM
extern "C" void cSpring_update(void*);
extern "C" void cFlexBridgeNode_updateGrid(cFlexBridgeNode*);

/**
 * Updates a flexible bridge for one frame: steps its spring mesh (Mesh, +0x50), then rebuilds its grid.
 *
 * Calls cSpring_update() and then cFlexBridgeNode_updateGrid().
 */
extern "C" void cFlexBridgeNode_update(cFlexBridgeNode* self)
{
    cSpring_update((char*)&self->Mesh);
    self->updateGrid();
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_updateGrid);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void* cObjectInterface_getNash();
// PORT: the unit declares nash_Nash_movenode with sBox00348058 (defined later); bound by asm label.
extern "C" void nash_Nash_movenode_75D8(void* world, int type, int id, void* box, void* old) __asm__("nash_Nash_movenode");

// t3Vector with an empty constructor and assignment from a t4Vector (x, y, z), as the grid's position array needs.
struct t3VectorFrom4 : public t3Vector {
    t3VectorFrom4() {}
    t3VectorFrom4& operator=(const t4Vector& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
};

extern t4Vector t4vect_ZAxis;

// A vector's components read through a call (the bound-box loop's min / max need it: a plain member read schedules
// the float moves differently).
static inline float X_75D8(const t4Vector& v) { return v.x; }
static inline float Y_75D8(const t4Vector& v) { return v.y; }
static inline float Z_75D8(const t4Vector& v) { return v.z; }

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_75D8(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_75D8(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_75D8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_75D8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_75D8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Normalize_75D8(const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline t4Vector MulMat_75D8(const t4Matrix* m, const t4Vector& v)
{
    t4Vector r;
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

/**
 * Re-poses a flexible bridge's strip from its spring nodes and updates the mesh and the space hash.
 *
 * Each node gives two points: its position plus the up and side offsets turned by the node's angle. Then the
 * per-triangle normals, the world bounding box (moved in the Nash grid) and the mesh positions (mesh slot 3).
 *
 * @param vself  the bridge node
 */
// Re-poses the bridge strip from its nodes: two rows of points (node pos +/- the rotated
// side/up offsets), per-triangle normals, the transformed bounding box and the mesh update.
void cFlexBridgeNode::updateGrid()
{
    cFlexBridgeNode* self = this;
    int n = self->Mesh.Width;
    t4Vector upRef = Scale_75D8(t4vect_ZAxis, Length_75D8(self->BottomRightOffset));
    t4Vector sideRef = Scale_75D8(t4vect_ZAxis, Length_75D8(self->BottomLeftOffset));
    int nverts = n * 2;
    int ntris = (n - 1) * 2;
    tSpringNode* node = self->Mesh.Nodes;
    int i;
    for (i = 0; i < n; i++) {
        float sinAngle, cosAngle;
        fSinCos(&sinAngle, &cosAngle, node->O);
        self->Verts[i] =
            Add_75D8(Add_75D8(node->P, Scale_75D8(self->BottomRightOffset, cosAngle)), Scale_75D8(upRef, sinAngle));
        self->Verts[n + i] =
            Sub_75D8(Add_75D8(node->P, Scale_75D8(self->BottomLeftOffset, cosAngle)), Scale_75D8(sideRef, sinAngle));
        node++;
    }
    int k;
    int j = 0;
    for (k = 0; k < ntris; k += 2) {
        self->FaceNorms[k] = Normalize_75D8(Cross_75D8(Sub_75D8(self->Verts[n + j + 1], self->Verts[j]),
                                                       Sub_75D8(self->Verts[n + j], self->Verts[j])));
        self->FaceNorms[k + 1] = Normalize_75D8(Cross_75D8(Sub_75D8(self->Verts[j + 1], self->Verts[j]),
                                                           Sub_75D8(self->Verts[n + j + 1], self->Verts[j])));
        j++;
    }
    tBoundBox* box = (&self->BoundBox);
    t3VectorFrom4 pos[32];
    float minX = 1e10f, minY = 1e10f, minZ = 1e10f;
    float maxX = -1e10f, maxY = -1e10f, maxZ = -1e10f;
    t4Matrix* matrix = (t4Matrix*)self->getRootTransform();
    for (int vertexIndex = 0; vertexIndex < nverts; vertexIndex++) {
        pos[vertexIndex] = self->Verts[vertexIndex];
        t4Vector w = MulMat_75D8(matrix, self->Verts[vertexIndex]);
        minX = X_75D8(w) < minX ? X_75D8(w) : minX;
        maxX = maxX < X_75D8(w) ? X_75D8(w) : maxX;
        minY = Y_75D8(w) < minY ? Y_75D8(w) : minY;
        maxY = maxY < Y_75D8(w) ? Y_75D8(w) : maxY;
        minZ = Z_75D8(w) < minZ ? Z_75D8(w) : minZ;
        maxZ = maxZ < Z_75D8(w) ? Z_75D8(w) : maxZ;
    }
    tBoundBox old = *(&self->BoundBox);
    t4Vector corner;
    corner.x = minX;
    corner.y = minY;
    corner.z = minZ;
    corner.w = 1.0f;
    self->BoundBox.MinPoint = corner;
    corner.x = maxX;
    corner.y = maxY;
    corner.z = maxZ;
    self->BoundBox.MaxPoint = corner;
    int id = ((int)self->Instance);
    nash_Nash_movenode_75D8(cObjectInterface_getNash(), 0, id, box, &old);
    sVEntry<void(...)>* vt = (sVEntry<void(...)>*)self->GridMesh->mVtbl;
    vt[3].fn(vthis(self->GridMesh, vt[3]), pos);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_render);
#ifdef SKIP_ASM




static inline int IsLoaded_7B80(tInstanceRecord* m)
{
    cWorldCache* wc = cWorld_m_pTheWorld->m_pWorldCache;
    if (m->m_SMap.m_uiSectionID == 0xFF)
        return 1;
    int ok = wc->m_WorldView.mp_SectionState[m->m_SMap.m_uiSectionID].state == 6;
    return ok ? wc->m_HullPages[m->m_SMap.m_iHullID].m_State == 3 : 0;
}

// The graphics manager's slots as this caller makes them (hand-made calls keep retail's order of the vtable reads):
// GetNetMatrix (slot 43), ClipBoundBox (93) and RenderModel (96) of render/graphicsman.h, and the grid mesh's draw
// (slot 7 of cGridMesh's vtable).
typedef sVEntry<t4Matrix(void*)> tGetNetMatrixSlot;
typedef sVEntry<int(void*, t4Vector*, t4Vector*, t4Matrix*)> tClipBoundBoxSlot;
typedef sVEntry<void(void*, tInstanceRecord*, int, int, int)> tRenderModelSlot;
typedef sVEntry<void(void*, const t4Matrix*, tMaterialBlockRecord*, void*)> tGridMeshRenderSlot;

/**
 * Draws a flexible bridge: its strip mesh with the bridge's transform, or the instance model when there is no
 * strip.
 *
 * Skips instances that are not loaded and boxes the graphics manager culls (slot 93). Pushes a copy of the
 * render state with field 5 set for the draw and pops it afterwards.
 */
// Draws the bridge piece's model (through its handler if it has one) unless it is culled or not streamed in.
extern "C" void cFlexBridgeNode_render(cFlexBridgeNode* self)
{
    tInstanceRecord* model = self->Instance;
    if (!IsLoaded_7B80(model)) {
        return;
    }
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    {
        tClipBoundBoxSlot* vt = (tClipBoundBoxSlot*)VPTR_OF(cGraphicsMan, ctx);
        void* thisp = vthis(ctx, vt[93]);
        t4Vector* lo = &self->BoundBox.MinPoint;
        t4Vector* hi = &self->BoundBox.MaxPoint;
        tGetNetMatrixSlot* vt43 = (tGetNetMatrixSlot*)VPTR_OF(cGraphicsMan, ctx);
        t4Matrix viewMatrix = vt43[43].fn(vthis(ctx, vt43[43]));
        if (vt[93].fn(thisp, lo, hi, &viewMatrix) == GM_CLIPCODE_OUTSIDE) {
            return;
        }
    }
    const t4Matrix* mtx = self->getRootTransform();
    ctx->m_pCurrentDrawState[1] = ctx->m_pCurrentDrawState[0];
    sDrawState* state = ctx->m_pCurrentDrawState;
    ctx->m_pCurrentDrawState = state + 1;
    state[1].m_bitGroups[1] = ((int)state[1].m_bitGroups[1] & ~0x7C) | (5 << 2);
    cGridMesh* renderer = self->GridMesh;
    if (renderer != 0) {
        tGridMeshRenderSlot* hv = (tGridMeshRenderSlot*)renderer->mVtbl;
        hv[7].fn(vthis(renderer, hv[7]), mtx, self->Instance->m_MaterialBlock, 0);
    } else {
        tRenderModelSlot* ctxVtbl = (tRenderModelSlot*)VPTR_OF(cGraphicsMan, ctx);
        ctxVtbl[96].fn(vthis(ctx, ctxVtbl[96]), self->Instance, 0, 0, 0);
    }
    ctx->m_pCurrentDrawState--;
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);
extern "C" void cSpring_writeToReplayFrame(void* p, void* stream);

/**
 * Writes a flexible bridge to a replay frame: the cInstanceNode data, its spring mesh, then 0x30 bytes from
 * +0x20.
 *
 * The spring (+0x50) is written by cSpring_writeToReplayFrame (On Tour cSpring::writeToReplayFrame); the last block goes
 * through the stream's virtual write (slot 1).
 *
 * @param stream  the replay frame stream to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cFlexBridgeNode_writeToReplayFrame(cFlexBridgeNode* self, cBxReplayFrame* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    cSpring_writeToReplayFrame((char*)&self->Mesh, stream);
    stream->write((const char*)&self->BottomLeftOffset, 0x30);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_cFlexRailNode);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode1(void* self, int type, void* a2, void* a3);
extern "C" void* cSimpleTube_cSimpleTube(void* self, int n);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char cFlexRailNode_vtbl[];
extern char str_FlexNodeMats[];
extern char str_Tube_b[];

// A segment matrix with an empty constructor: retail's new[] of the segments has one (with a plain t4Matrix array the
// constructors get a smaller frame).
struct tFlexNodeMat : public t4Matrix {
    tFlexNodeMat() {}
};


/**
 * Constructs the flex-rail base of a float or spring rail (bucket 1) for a placed rail instance.
 *
 * Runs the record constructor cInstanceNode_cInstanceNode1, installs the flex-rail vtable cFlexRailNode_vtbl, sets RailFlags (+0x24) from def
 * +0x44 and NumNodes (+0x20) to the number of the 8 instance ids (def +0x4..+0x20) that are not -1. Allocates NumNodes
 * - 1 segment matrices ("FlexNodeMats", +0x50) and a "Tube" object (+0x58) built by cSimpleTube_cSimpleTube with NumNodes * 8 -
 * 9.
 *
 * @param nodeType  node type (20 float rail, 21 spring rail)
 * @param instRecord  rail instance record
 * @param def  rail make-data
 * @return self
 */
extern "C" void* cFlexRailNode_cFlexRailNode(cFlexRailNode* self, void* nodeType, void* instRecord, tMakeFlexRailData* def)
{
    cInstanceNode_cInstanceNode1(((char*)self), 1, nodeType, instRecord);
    VPTR_OF(cObjNode, self) = (void**)cFlexRailNode_vtbl;
    int len = def->Flags;
    self->NumNodes = 0;
    self->RailFlags = len;
    for (int i = 0; i < 8; i++) {
        if ((&def->InstId1)[i] != tResourceID::kInvalidRID) {
            self->NumNodes += 1;
        }
    }
    t4Matrix** segs = &self->Mats;
    *segs = new (str_FlexNodeMats, 0x20000000, 0) tFlexNodeMat[self->NumNodes - 1];
    self->Tube = (cSimpleTube*)cSimpleTube_cSimpleTube(cMemMan_alloc(0xC, str_Tube_b, 0x20000000, 0), self->NumNodes * 8 - 9);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_cFlexRailNode1);
#ifdef SKIP_ASM

extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a, void* stream);

/**
 * Constructs the flex-rail base of a float or spring rail from a replay frame.
 *
 * Runs the replay constructor cInstanceNode_cInstanceNode, installs cFlexRailNode_vtbl, reads the 0x30-byte rail state at +0x20
 * (NumNodes, RailFlags, BoundBox) through the stream's read slot (vtable entry 2) and allocates Mats and Tube as
 * cFlexRailNode_cFlexRailNode does.
 *
 * @param bucket  update bucket
 * @param stream  replay stream to read from
 * @return self
 */
void* cFlexRailNode::cFlexRailNode1(void* bucket, cBxReplayFrame* stream)
{
    t4Matrix** segs = &Mats;
    cInstanceNode_cInstanceNode(((char*)this), bucket, stream);
    VPTR_OF(cObjNode, this) = (void**)cFlexRailNode_vtbl;
    stream->read((char*)&NumNodes, 0x30);
    *segs = new (str_FlexNodeMats, 0x20000000, 0) tFlexNodeMat[NumNodes - 1];
    Tube = (cSimpleTube*)cSimpleTube_cSimpleTube(cMemMan_alloc(0xC, str_Tube_b, 0x20000000, 0), NumNodes * 8 - 9);
    return ((char*)this);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode__cFlexRailNode);
#ifdef SKIP_ASM

void cMemMan_free(void*);
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);
extern char cFlexRailNode_vtbl[];

/**
 * Destroys the flex-rail base of a float or spring rail.
 *
 * Reinstalls cFlexRailNode_vtbl, frees the segment matrices (+0x50), destroys the Tube object (+0x58, its vtable entry 1 with
 * flags 3) and runs cInstanceNode__cInstanceNode with flags.
 *
 * @param flags  destructor flags passed on to cInstanceNode__cInstanceNode
 */
extern "C" void cFlexRailNode__cFlexRailNode(cFlexRailNode* self, int flags)
{
    VPTR_OF(cObjNode, self) = (void**)cFlexRailNode_vtbl;
    void* buf = self->Mats;
    if (buf != 0) {
        cMemMan_free(buf);
    }
    void* obj = self->Tube;
    if (obj != 0) {
        sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)obj;
        vt[1].fn(vthis(obj, vt[1]), 3);
    }
    cInstanceNode__cInstanceNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_update);
#ifdef SKIP_ASM

extern "C" void cFlexRailNode_updateSpaceHash(cFlexRailNode* self);

/**
 * Updates a flexible rail for one frame: calls its virtual slots 47 and 48, then updates its space hash.
 *
 * Slots 47 and 48 differ between the three vtables that use this update, and are empty in one of them; they are
 * probably per-type hooks that move the rail's points and matrices.
 */
extern "C" void cFlexRailNode_update(cFlexRailNode* self)
{
    self->updateNodes();
    self->updateMats();
    self->updateSpaceHash();
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_updateSpaceHash);
#ifdef SKIP_ASM



extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, int id, tBoundBox* box, tBoundBox* old);

/**
 * Recomputes a flexible rail's bounding box and moves it in the Nash space hash.
 *
 * The box comes from virtual slot 50 (cSpringRailNode_updateBoundBox: the rail elements' box, padded by 200); the node moves
 * from its old box to the new one as type 2.
 *
 */
void cFlexRailNode::updateSpaceHash()
{
    cFlexRailNode* self = this;
    tBoundBox* box = (&self->BoundBox);
    tBoundBox old = *(&self->BoundBox);
    self->updateBoundBox();
    nash_Nash_movenode(cObjectInterface_getNash(), 2, ((int)self->Node), box, &old);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMat_3480C8 {
    float m[4][4];
} __attribute__((aligned(16)));


typedef sMat_3480C8 (*tGetMat_3480C8)(void*);
typedef int (*tCull_3480C8)(void*, void*, void*, const sMat_3480C8&);

struct sCtx_3480C8 {
    char pad0[0xE84];
    sDrawState* top;        // 0xE84
    char padE88[0x10D8 - 0xE88];
    sVEntry<void>* vt;         // 0x10D8
};
extern int D_004A5980;
extern float D_004FF6C0[8][4];

struct sFlex_3480C8 {
    char pad0[0x20];
    int count;              // 0x20
    char pad24[0xC];
    char boxMin[0x10];      // 0x30
    char boxMax[0x10];      // 0x40
};

/**
 * Starts drawing a flex rail but draws nothing.
 *
 * Returns when graphics-manager slot 93 culls BoundBox (+0x30, +0x40). Otherwise fills the 8-row basis table D_004FF6C0
 * once (guard D_004A5980) with (t^3, t^2, t, 1), clears the draw state's first texture id and sets its blend bits to 4,
 * then runs empty loops over the segments.
 *
 * Quirks: the loops have no body, so nothing is drawn; and rows 4 to 7 of the table repeat t = 3/7 instead of
 * continuing to 1.
 */
extern "C" void cFlexRailNode_render(cFlexRailNode* self)
{
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    sVEntry<void>* vt = (sVEntry<void>*)VPTR_OF(cGraphicsMan, ctx);
    if (((tCull_3480C8)vt[93].fn)(vthis(ctx, vt[93]), ((char*)&self->BoundBox), ((char*)&self->BoundBox.MaxPoint),
                                  ((tGetMat_3480C8)vt[43].fn)(vthis(ctx, vt[43]))) == GM_CLIPCODE_OUTSIDE) return;
    if (D_004A5980 == 0) {
        D_004FF6C0[0][0] = 0.0f;
        D_004FF6C0[0][1] = 0.0f;
        D_004FF6C0[0][2] = 0.0f;
        D_004FF6C0[0][3] = 1.0f;
        D_004FF6C0[1][0] = 0.0029154522344470024f;
        D_004FF6C0[1][1] = 0.020408164709806442f;
        D_004FF6C0[1][2] = 0.1428571492433548f;
        D_004FF6C0[1][3] = 1.0f;
        D_004FF6C0[2][0] = 0.02332361787557602f;
        D_004FF6C0[2][1] = 0.08163265883922577f;
        D_004FF6C0[2][2] = 0.2857142984867096f;
        D_004FF6C0[2][3] = 1.0f;
        D_004FF6C0[3][0] = 0.07871722429990768f;
        D_004FF6C0[3][1] = 0.18367350101470947f;
        D_004FF6C0[3][2] = 0.4285714626312256f;
        D_004FF6C0[3][3] = 1.0f;
        D_004FF6C0[4][0] = 0.07871722429990768f;
        D_004FF6C0[4][1] = 0.18367350101470947f;
        D_004FF6C0[4][2] = 0.4285714626312256f;
        D_004FF6C0[4][3] = 1.0f;
        D_004FF6C0[5][0] = 0.07871722429990768f;
        D_004FF6C0[5][1] = 0.18367350101470947f;
        D_004FF6C0[5][2] = 0.4285714626312256f;
        D_004FF6C0[5][3] = 1.0f;
        D_004FF6C0[6][0] = 0.07871722429990768f;
        D_004FF6C0[6][1] = 0.18367350101470947f;
        D_004FF6C0[6][2] = 0.4285714626312256f;
        D_004FF6C0[6][3] = 1.0f;
        D_004FF6C0[7][0] = 0.07871722429990768f;
        D_004FF6C0[7][1] = 0.18367350101470947f;
        D_004FF6C0[7][2] = 0.4285714626312256f;
        D_004FF6C0[7][3] = 1.0f;
        D_004A5980 = 1;
    }
    ctx->m_pCurrentDrawState->m_textureState.m_texid[0] = -1;
    ctx->m_pCurrentDrawState->m_bitGroups[1] = (((int)(ctx->m_pCurrentDrawState)->m_bitGroups[1]) & ~0x7C) | 4;
    int n = self->NumNodes - 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 8; j++) {
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_splineIntersect);
#ifdef SKIP_ASM
struct sFbV_8290 {
    float x, y, z, w;
    sFbV_8290() {}
    // t4vect::x()/y()/z()/w() in On Tour (component accessors returning a reference).
    float& X() { return x; }
    float& Y() { return y; }
    float& Z() { return z; }
    float& W() { return w; }
} __attribute__((aligned(16)));


// PORT: PS2-only VU0 inline asm (out = m * v).
static inline sFbV_8290 fbApply_8290(const t4Matrix* m, const sFbV_8290& v)
{
    sFbV_8290 out;
    __asm__(
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(out)
        : "r"(m), "m"(v)
        : "memory");
    return out;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sFbV_8290 fbSub_8290(const sFbV_8290& a, const sFbV_8290& b)
{
    sFbV_8290 r;
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
static inline sFbV_8290 fbAdd_8290(const sFbV_8290& a, const sFbV_8290& b)
{
    sFbV_8290 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sFbV_8290 fbScale_8290(const sFbV_8290& v, float s)
{
    sFbV_8290 r;
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
static inline float fbDot_8290(const sFbV_8290& a, const sFbV_8290& b)
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
static inline float fbLength_8290(const sFbV_8290& v)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sFbV_8290 fbNormalize_8290(const sFbV_8290& v)
{
    sFbV_8290 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: abs.s helper (PS2 FPU).
static inline float fbAbs_8290(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// splineEval / splineSlope (On Tour names): the cubic segment and its derivative at t.
// On Tour: splineEval_8290(const t4Matrix&, float): the point at t, (t^3, t^2, t, 1) through the coefficient rows.
static inline sFbV_8290 splineEval_8290(const t4Matrix& mat, float t)
{
    sFbV_8290 v;
    v.W() = 1.0f;
    v.Z() = t;
    v.Y() = t * v.Z();
    v.X() = t * v.Y();
    return fbApply_8290(&mat, v);
}

// On Tour: splineSlope_8290(const t4Matrix&, float): the tangent at t, (3t^2, 2t, 1, 0).
static inline sFbV_8290 splineSlope_8290(const t4Matrix& mat, float t)
{
    sFbV_8290 v;
    v.W() = 0.0f;
    v.Z() = 1.0f;
    v.Y() = t * 2.0f;
    v.X() = t * (t * 3.0f);
    return fbApply_8290(&mat, v);
}

static inline float bracket_8290(float lo, float x, float hi)
{
    float r;
    if (x >= lo) {
        r = x <? hi; // PORT: g++ min operator
    } else {
        r = lo;
    }
    return r;
}



extern sFbV_8290 t4vect_Zero;

// tIntersectInfo::Init (On Tour name).
static inline void Init_8290(tIntersectInfo* h)
{
    h->HitType = 2;
    (*(int*)&h->Patch) = 0;
    (*(void**)&h->Inst) = 0;
    (*(int*)&h->Spline) = 0;
    h->Mesh = -1;
    h->Face = -1;
    h->Seg = -1;
    h->Stype = -1;
    (*(sFbV_8290*)&h->Ivel) = t4vect_Zero;
    (*(sFbV_8290*)&h->Iangvel) = t4vect_Zero;
    h->T = 0.0f;
    h->u = 0.0f;
    h->v = 0.0f;
}

// The golden-section brackets per coarse sample (rodata).
extern float D_0048E7D8[5];
extern float D_0048E7F0[5];

/**
 * Finds the point of a flex rail's spline closest to bpos and records it as a hit when it beats the best so far.
 *
 * Skipped unless RailFlags (+0x24) shares a bit with splineFilterMask. For each segment matrix (+0x50, P(t) = M * (t^3,
 * t^2, t, 1)) it samples t = 0, 0.25, ... 1, projects bpos onto each chord to pick the nearest quarter, takes the
 * search bracket for it from D_0048E7D8/D_0048E7F0 and narrows it by golden-section search (at most 24 steps, until the
 * bracket is under 0.0005). When *hit is 0 or the distance beats *dist, it sets *hit and *dist and fills intInfo: type
 * 2, position, unit tangent, Instance as the object, the mesh from vtable entry 49 (getMesh), the segment, t and the
 * surface from the instance's mesh table (+0x88, short at +0x1A per 12-byte entry; eST_Undefined becomes eST_Default).
 * minBpos and maxBpos are not used.
 *
 * Quirk: the chord projection is clamped at 0 but not at 1.
 *
 * @param minBpos  minimum corner of the query box (unused)
 * @param maxBpos  maximum corner of the query box (unused)
 * @param bpos  query point
 * @param splineFilterMask  rail flag bits to accept
 * @param hit  set to 1 when a hit is recorded
 * @param dist  best distance so far; updated on a hit
 * @param intInfo  intersect info filled on a hit
 */
extern "C" void cFlexRailNode_splineIntersect(cFlexRailNode* self, const sFbV_8290* minBpos, const sFbV_8290* maxBpos, const sFbV_8290* bpos,
                              unsigned int splineFilterMask, int* hit, float* dist, tIntersectInfo* intInfo)
{
    if (((unsigned int)self->RailFlags & splineFilterMask) == 0) {
        return;
    }
    for (int seg = 0; seg < self->NumNodes - 1; seg++) {
        int i;
        sFbV_8290 lastp;
        const float kTopSplit = 0.6180340051651001f;
        const float kBottomSplit = 1.0f - kTopSplit;
        float distLo, distMidLo, distMidHi, distHi;
        float x0 = 0.0f;
        float x3 = 1.0f;
        sFbV_8290 nextp;
        sFbV_8290 tmp;
        const t4Matrix& mat = (self->Mats)[seg];
        lastp = splineEval_8290(mat, x0);
        float cd = fbLength_8290(fbSub_8290(*bpos, lastp));
        int ci = 0;
        for (i = 1; i < 5; i++) {
            nextp = splineEval_8290(mat, i * 0.25f);
            tmp = fbSub_8290(nextp, lastp);
            float chordParam = fbDot_8290(fbSub_8290(*bpos, lastp), tmp) / fbDot_8290(tmp, tmp);
            chordParam = bracket_8290(0.0f, chordParam, 1.0f);
            tmp = fbAdd_8290(lastp, fbScale_8290(tmp, chordParam));
            float chordDist = fbLength_8290(fbSub_8290(*bpos, tmp));
            if (chordDist < cd) {
                ci = i;
                cd = chordDist;
            }
            lastp = nextp;
        }
        float minx[5];
        __builtin_memcpy(minx, D_0048E7D8, sizeof(minx));
        float maxx[5];
        __builtin_memcpy(maxx, D_0048E7F0, sizeof(maxx));
        x0 = minx[ci];
        x3 = maxx[ci];
        float x1 = x0 + (x3 - x0) * kBottomSplit;
        float x2 = x0 + (x3 - x0) * kTopSplit;
        float lastd, lastt;
        lastp = splineEval_8290(mat, x0);
        distLo = fbLength_8290(fbSub_8290(lastp, *bpos));
        lastp = splineEval_8290(mat, x1);
        distMidLo = fbLength_8290(fbSub_8290(lastp, *bpos));
        lastp = splineEval_8290(mat, x2);
        distMidHi = fbLength_8290(fbSub_8290(lastp, *bpos));
        lastp = splineEval_8290(mat, x3);
        distHi = fbLength_8290(fbSub_8290(lastp, *bpos));
        lastd = distMidHi;
        lastt = x2;
        for (i = 0; i < 24; i++) {
            if (fbAbs_8290(x3 - x0) <= 0.0005000000237487257f) {
                break;
            }
            if (distMidHi < distMidLo) {
                x0 = x1;
                x1 = x2;
                x2 = kTopSplit * x1 + kBottomSplit * x3;
                lastp = splineEval_8290(mat, x2);
                float tmp = fbLength_8290(fbSub_8290(lastp, *bpos));
                distLo = distMidLo;
                distMidLo = distMidHi;
                distMidHi = tmp;
                lastt = x2;
                lastd = tmp;
            } else {
                x3 = x2;
                x2 = x1;
                x1 = kTopSplit * x2 + kBottomSplit * x0;
                lastp = splineEval_8290(mat, x1);
                float tmp = fbLength_8290(fbSub_8290(lastp, *bpos));
                distHi = distMidHi;
                distMidHi = distMidLo;
                distMidLo = tmp;
                lastt = x1;
                lastd = tmp;
            }
        }
        if (*hit == 0 || lastd < *dist) {
            *hit = 1;
            *dist = lastd;
            Init_8290(intInfo);
            (*(sFbV_8290*)&intInfo->Ipos) = lastp;
            (*(sFbV_8290*)&intInfo->Inorm) = fbNormalize_8290(splineSlope_8290(mat, lastt));
            (*(void**)&intInfo->Inst) = (void*)self->Instance;
            intInfo->Mesh = self->getMesh(seg);
            intInfo->Seg = seg;
            (*(int*)&intInfo->Spline) = 0;
            intInfo->T = lastt;
            int mesh = intInfo->Mesh;
            if (mesh <= 0) {
                mesh = 0;
            }
            short* surf = (short*)((char*)self->Instance->m_pCollideRecord + 0x1A + mesh * 12);
            if (*surf == eST_Undefined) {
                intInfo->Stype = eST_Default;
            } else {
                intInfo->Stype = *surf;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes the flex-rail base of a float or spring rail to a replay frame.
 *
 * Writes the instance-node part (cInstanceNode_writeToReplayFrame), then the 0x30-byte rail state at +0x20 (NumNodes,
 * RailFlags, BoundBox) through the stream's write slot (vtable entry 1).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cFlexRailNode_writeToReplayFrame(cFlexRailNode* self, cBxReplayFrame* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    stream->write((const char*)&self->NumNodes, 0x30);
}
#endif


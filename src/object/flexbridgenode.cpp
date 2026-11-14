#include "common.h"

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_setupGrid);
#ifdef SKIP_ASM
struct sBox00348058;
extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, int id, sBox00348058* box, sBox00348058* old);
void cMemMan_free(void*);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char D_0048E778[];
extern char D_0048E788[];
extern char D_0048E798[];
extern char D_0048E7A8[];
extern char D_0048E7B8[];

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

struct sGridVEntry {
    short delta;
    short index;
    void* (*fn)(...);
};

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

extern char* D_004A5B80;

extern "C" void cFlexBridgeNode_setupGrid(sFlexGrid* self)
{
    char* world = D_004A5B80;
    int n = self->count;
    int nverts = n * 2;
    int ntris = (n - 1) * 2;
    sGridVEntry* e = &(*(sGridVEntry**)(world + 0x10D8))[76];
    self->mesh = (sGridMesh*)e->fn(world + e->delta, n, 2, 0x20000000);
    sGridV4*& p0 = self->pts[0];
    p0 = new (D_0048E778, 0x20000000, 0) sGridV4[nverts];
    sGridV4*& p1 = self->pts[1];
    p1 = new (D_0048E788, 0x20000000, 0) sGridV4[nverts];
    self->idx = new (D_0048E798, 0x20000000, 0) unsigned char[ntris * 3];
    sGridV3 pos[4];
    sGridV2 uvs[4];
    sGridCol cols[4];
    sGridV2* uv = new (D_0048E7A8, 0x100, 0) sGridV2[nverts];
    sGridCol* col = new (D_0048E7B8, 0x100, 0) sGridCol[nverts];
    sGridVEntryV* e2 = &(*(sGridVEntryV**)(world + 0x10D8))[97];
    e2->fn(world + e2->delta, self->def, 0, pos, uvs, cols);
    for (int i = 0; i < n; i++) {
        float t = (float)i;
        float inv = 1.0f / (float)(n - 1);
        self->pts[0][i] = sGridV4(pos[0] + (pos[1] - pos[0]) * t * inv, 1.0f);
        self->pts[0][n + i] = sGridV4(pos[2] + (pos[3] - pos[2]) * t * inv, 1.0f);
        uv[i] = uvs[0] + (uvs[1] - uvs[0]) * t * inv;
        uv[n + i] = uvs[2] + (uvs[3] - uvs[2]) * t * inv;
        col[i].a = 1.0f;
        col[i].r = cols[0].r + (cols[1].r - cols[0].r) * t * inv;
        col[i].g = cols[0].g + (cols[1].g - cols[0].g) * t * inv;
        col[i].b = cols[0].b + (cols[1].b - cols[0].b) * t * inv;
        col[n + i].a = 1.0f;
        col[n + i].r = cols[2].r + (cols[3].r - cols[2].r) * t * inv;
        col[n + i].g = cols[2].g + (cols[3].g - cols[2].g) * t * inv;
        col[n + i].b = cols[2].b + (cols[3].b - cols[2].b) * t * inv;
    }
    sGridVEntryV* e3 = &self->mesh->vt[6];
    e3->fn((char*)self->mesh + e3->delta, pos, col, uv);
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
    nash_Nash_movenode(cObjectInterface_getNash(), 0, id, (sBox00348058*)&self->box, (sBox00348058*)&old);
    int v = 0;
    int j = 0;
    for (int k = 0; k < ntris - 1; k += 2) {
        self->idx[j + 0] = v;
        self->idx[j + 1] = v + n;
        self->idx[j + 2] = v + 1;
        self->idx[j + 3] = v + n;
        self->idx[j + 4] = v + n + 1;
        self->idx[j + 5] = v + 1;
        v++;
        j += 6;
    }
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_initSpringNodes);
#ifdef SKIP_ASM
struct sFlexVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
static inline sFlexVec4 flexVecSub(const sFlexVec4& a, const sFlexVec4& b)
{
    sFlexVec4 r;
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
static inline sFlexVec4 flexVecAdd(const sFlexVec4& a, const sFlexVec4& b)
{
    sFlexVec4 r;
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
static inline sFlexVec4 flexVecScale(const sFlexVec4& v, float s)
{
    sFlexVec4 r;
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
    sFlexVec4 pos;       // 0x00
    char pad_0x10[0x40];
};

struct sFlexBridge {
    char pad_0x00[0x20];
    sFlexVec4 end;       // 0x20
    sFlexVec4 start;     // 0x30
    char pad_0x40[0x10];
    int count;           // 0x50
    char pad_0x54[0x20];
    sFlexSeg* segs;      // 0x74
    char pad_0x78[0x4];
    sFlexVec4* pts;      // 0x7C: 2 * count points
};

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
struct sV4_6E38 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_6E38 {
    sV4_6E38 r[4];
};

struct sSeg_6E38 {
    char pad_0x0[0x30];
    sV4_6E38 force;             // 0x30
    float torque;               // 0x40
    char pad_0x44[0xC];
};

struct sNode_6E38 {
    sV4_6E38 pos;               // 0x00
    char pad_0x10[0x50];
    int idx;                    // 0x60
};

class cBridge_6E38 {
public:
    char pad_0x0[0xC];
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
    virtual sMat_6E38* GetMatrix();
};

struct sBridgeData_6E38 {
    char pad_0x0[0x30];
    sV4_6E38 axis;              // 0x30
    char pad_0x40[0x24];
    float kLin;                 // 0x64
    float kAng;                 // 0x68
    char pad_0x6C[0x8];
    sSeg_6E38* segs;            // 0x74
    char pad_0x78[0x8];
    sV4_6E38* pts;              // 0x80
};

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV4_6E38 vu0Sub_6E38(const sV4_6E38& a, const sV4_6E38& b)
{
    sV4_6E38 r;
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
static inline sV4_6E38 vu0Scale_6E38(float s, const sV4_6E38& v)
{
    sV4_6E38 r;
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
static inline sV4_6E38 vu0Norm_6E38(const sV4_6E38& v)
{
    sV4_6E38 r;
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
static inline sV4_6E38 vu0MulMat_6E38(const sMat_6E38* m, const sV4_6E38& v)
{
    sV4_6E38 r;
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
static inline float vu0Dot_6E38(const sV4_6E38& a, const sV4_6E38& b)
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
static inline void vu0AddEq_6E38(sV4_6E38& dst, sV4_6E38 b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

static inline sV4_6E38 Pos_6E38(const sMat_6E38* m)
{
    return m->r[3];
}

static inline sV4_6E38 ToLocal_6E38(const sMat_6E38* m, const sV4_6E38& v)
{
    sV4_6E38 r;
    sV4_6E38 f = vu0Sub_6E38(v, vu0Scale_6E38(v.w, Pos_6E38(m)));
    r.x = m->r[0].x * f.x + m->r[0].y * f.y + m->r[0].z * f.z;
    r.y = m->r[1].x * f.x + m->r[1].y * f.y + m->r[1].z * f.z;
    r.z = m->r[2].x * f.x + m->r[2].y * f.y + m->r[2].z * f.z;
    r.w = v.w;
    return r;
}

static inline float Torque_6E38(float k, const sV4_6E38& a, const sV4_6E38& b)
{
    return k * vu0Dot_6E38(a, b);
}

extern "C" void cFlexBridgeNode_applyForce(cBridge_6E38* self, sNode_6E38* intInfo, sV4_6E38* force)
{
    sBridgeData_6E38* d = (sBridgeData_6E38*)self;
    if (intInfo->idx < 0) {
        return;
    }
    sMat_6E38* transform = self->GetMatrix();
    sV4_6E38 v = vu0Sub_6E38(intInfo->pos, Pos_6E38(transform));
    sV4_6E38 y = vu0MulMat_6E38(transform, vu0Norm_6E38(d->axis));
    sV4_6E38 n = vu0MulMat_6E38(transform, d->pts[intInfo->idx]);
    float yoffset = vu0Dot_6E38(v, y);
    int seg = intInfo->idx / 2;
    sSeg_6E38* pNode = d->segs;
    vu0AddEq_6E38(pNode[seg].force, vu0Scale_6E38(d->kLin, ToLocal_6E38(transform, *force)));
    pNode[seg].torque += Torque_6E38(d->kAng, n, *force) * yoffset;
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_updateIntersectInfo);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV4_70D0 {
    float x, y, z, w;
    sV4_70D0() {}
    sV4_70D0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern sV4_70D0 D_004FF140_70D0 __asm__("D_004FF140");

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV4_70D0 vu0Sub_70D0(const sV4_70D0& a, const sV4_70D0& b)
{
    sV4_70D0 r;
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
static inline sV4_70D0 vu0Add_70D0(const sV4_70D0& a, const sV4_70D0& b)
{
    sV4_70D0 r;
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
static inline void vu0AddEq_70D0(sV4_70D0& a, const sV4_70D0& b)
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
static inline sV4_70D0 vu0ScaleP_70D0(const sV4_70D0* v, float s)
{
    sV4_70D0 r;
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
static inline sV4_70D0 vu0MtxMul_70D0(const sV4_70D0* m, const sV4_70D0& v)
{
    sV4_70D0 r;
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
static inline sV4_70D0 vu0Cross_70D0(const sV4_70D0& a, const sV4_70D0& b)
{
    sV4_70D0 r;
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
    sV4_70D0* (*fn)(void*);
};

struct sFlEnt_70D0 {
    sV4_70D0 a;         // 0x00
    sV4_70D0 b;         // 0x10
    char pad_0x20[4];
    float f24;          // 0x24
    char pad_0x28[0x28];
};

struct sFlBridge_70D0 {
    char pad_0x00[0xC];
    sFlVE_70D0* vt;     // 0x0C
    char pad_0x10[0x64];
    sFlEnt_70D0* ents;  // 0x74
};

struct sFlState_70D0 {
    sV4_70D0 v[6];
    int idx;            // 0x60
};

extern "C" void cFlexBridgeNode_updateIntersectInfo(sFlBridge_70D0* self, sFlState_70D0* p)
{
    int idx = p->idx;
    if (idx >= 0) {
        int h = idx / 2;
        sFlEnt_70D0* ents = self->ents;
        sFlVE_70D0* vt = self->vt;
        sV4_70D0* m = vt[24].fn((char*)self + vt[24].delta);
        sFlEnt_70D0* e = (sFlEnt_70D0*)(h * 0x50 + (int)ents);
        sV4_70D0 r = vu0MtxMul_70D0(m, vu0ScaleP_70D0(&D_004FF140_70D0, e->f24));
        sV4_70D0 d = vu0Sub_70D0(p->v[0], vu0MtxMul_70D0(m, e->a));
        sV4_70D0 t = vu0Add_70D0(vu0MtxMul_70D0(m, e->b), vu0Cross_70D0(d, r));
        vu0AddEq_70D0(p->v[2], t);
        vu0AddEq_70D0(p->v[3], r);
    }
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_intersect);
#ifdef SKIP_ASM
struct sFV_7268 {
    float x, y, z, w;
    sFV_7268() {}
    sFV_7268(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sFRow_7268 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFM_7268 {
    sFRow_7268 r[4];
} __attribute__((aligned(16)));

extern sFV_7268 D_004FF120_v7268 __asm__("D_004FF120");

struct sFMesh_7268 {
    unsigned short numTris;     // 0x0
    short numVerts;             // 0x2
    void* indices;              // 0x4
    void* boxes;                // 0x8
    sFV_7268* verts;            // 0xC
    sFV_7268* normals;          // 0x10
};

struct sFHit_7268 {
    sFV_7268 pos;       // 0x00
    sFV_7268 nrm;       // 0x10
    sFV_7268 v20;       // 0x20
    sFV_7268 v30;       // 0x30
    float t;            // 0x40
    int type;           // 0x44
    int f48;            // 0x48
    int f4C;            // 0x4C
    void* owner;        // 0x50
    int f54;            // 0x54
    int f58;            // 0x58
    int f5C;            // 0x5C
    int tri;            // 0x60
    int f64;            // 0x64
    int f68;            // 0x68
    int f6C;            // 0x6C
    int f70;            // 0x70
    int pad[3];
};

struct sFVE_7268 { short delta; short index; void* fn; };

struct sFTester_7268 {
    char pad0[0x50];
    sFVE_7268* vt;      // 0x50
};

struct sFList_7268 {
    int pad0;
    int count;          // 0x4
};

struct sFData_7268 {
    char pad0[0x80];
    sFList_7268* list;  // 0x80
    float scale;        // 0x84
    char* elems;        // 0x88
};

struct sFNode_7268 {
    char pad0[0xC];
    sFVE_7268* vt;      // 0xC
    char pad10[0x8];
    sFData_7268* data;  // 0x18
    char pad1C[0x34];
    int count;          // 0x50
    char pad54[0x28];
    sFV_7268* pts;      // 0x7C
    sFV_7268* normals;  // 0x80
    void* indices;      // 0x84
};

typedef sFM_7268* (*GetMtxFn_7268)(void*);
typedef void (*SetXformFn_7268)(void*, sFM_7268*, float);
typedef void (*VoidFn_7268)(void*);

extern "C" int func_0032C0F8(sFTester_7268* t, sFMesh_7268* mesh, int flag, sFHit_7268* out, int cap);

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void Copy_7268(sFM_7268* d, const sFM_7268* s)
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
static inline sFV_7268 Scale_7268(float s, const sFV_7268& v)
{
    sFV_7268 r;
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
static inline sFV_7268 MulVec_7268(const sFM_7268* m, const sFV_7268& v)
{
    sFV_7268 r;
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

static inline sFV_7268 Pos_7268(const sFM_7268* m)
{
    return *(const sFV_7268*)&m->r[3];
}

static inline sFV_7268 Mul_7268(const sFV_7268& a, const sFV_7268& b)
{
    return sFV_7268(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

static inline void Reset_7268(sFHit_7268* h)
{
    h->f54 = 0;
    h->owner = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->tri = -1;
    h->f64 = -1;
    h->type = 2;
    h->f4C = -1;
    h->v20 = D_004FF120_v7268;
    h->v30 = D_004FF120_v7268;
    h->f68 = 0;
    h->f6C = 0;
    h->f70 = 0;
}

// Orthonormal inverse in place: transpose the 3x3 part, position = R^T * -t.
static inline void TransposeInverse_7268(sFM_7268* t)
{
    float tmp;
    tmp = t->r[0].y; t->r[0].y = t->r[1].x; t->r[1].x = tmp;
    tmp = t->r[0].z; t->r[0].z = t->r[2].x; t->r[2].x = tmp;
    tmp = t->r[1].z; t->r[1].z = t->r[2].y; t->r[2].y = tmp;
    sFV_7268 p = Scale_7268(-1.0f, Pos_7268(t));
    p.w = 0.0f;
    p = MulVec_7268(t, p);
    p.w = 1.0f;
    *(sFV_7268*)&t->r[3] = p;
}

extern "C" int cFlexBridgeNode_intersect(sFNode_7268* self, sFTester_7268* tester, sFHit_7268* out, int cap, int flags)
{
    if (!(flags & 2)) {
        return 0;
    }
    sFMesh_7268 mesh;
    mesh.numTris = (self->count - 1) * 2;
    mesh.numVerts = self->count * 2;
    mesh.indices = self->indices;
    mesh.verts = self->pts;
    mesh.normals = self->normals;
    sFM_7268* m = ((GetMtxFn_7268)self->vt[24].fn)((char*)self + self->vt[24].delta);
    sFM_7268 inv;
    Copy_7268(&inv, m);
    TransposeInverse_7268(&inv);
    ((SetXformFn_7268)tester->vt[1].fn)((char*)tester + tester->vt[1].delta, &inv, 1.0f / self->data->scale);
    int n = func_0032C0F8(tester, &mesh, 1, out, cap);
    ((VoidFn_7268)tester->vt[2].fn)((char*)tester + tester->vt[2].delta);
    for (int i = 0; i < n; i++) {
        sFHit_7268* h = &out[i];
        Reset_7268(h);
        float s = self->data->scale;
        float one = 1.0f;
        h->pos = MulVec_7268(m, Mul_7268(h->pos, sFV_7268(s, s, s, one)));
        h->nrm = MulVec_7268(m, h->nrm);
        h->type = 1;
        h->owner = self->data;
        int k = self->data->list->count - 1;
        h->f48 = 0;
        h->f5C = k;
        h->v20 = D_004FF120_v7268;
        h->v30 = D_004FF120_v7268;
        h->f4C = *(short*)(self->data->elems + k * 12 + 0x1A);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_update);
#ifdef SKIP_ASM
extern "C" void cSpring_update(void*);
extern "C" void cFlexBridgeNode_updateGrid(void*);

extern "C" void cFlexBridgeNode_update(void* self)
{
    cSpring_update((char*)self + 0x50);
    cFlexBridgeNode_updateGrid(self);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_updateGrid);
#ifdef SKIP_ASM
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void* cObjectInterface_getNash();
// PORT: the unit declares nash_Nash_movenode with sBox00348058 (defined later); bound by asm label.
extern "C" void nash_Nash_movenode_75D8(void* world, int type, int id, void* box, void* old) __asm__("nash_Nash_movenode");

struct sV4_75D8 {
    float x, y, z, w;
    float X() const { return x; }
    float Y() const { return y; }
    float Z() const { return z; }
} __attribute__((aligned(16)));

struct sV3_75D8 {
    float x, y, z;
    sV3_75D8() {}
    sV3_75D8& operator=(const sV4_75D8& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
};

struct sMat_75D8 {
    sV4_75D8 row[4];
};

struct sBox_75D8 {
    sV4_75D8 min;
    sV4_75D8 max;
};

struct sVE_75D8 {
    short delta;
    short index;
    void* (*fn)(...);
};

struct sFlexNode_75D8 {
    sV4_75D8 pos;           // 0x00
    char pad_0x10[0x10];
    float angle;            // 0x20
    char pad_0x24[0x50 - 0x24];
};

struct sVEV_75D8 {
    short delta;
    short index;
    void (*fn)(...);
};

struct sFlexMesh_75D8 {
    char pad_0x00[0x4];
    sVEV_75D8* vt;          // 0x04
};

struct sFlexBridge_75D8 {
    char pad_0x00[0xC];
    sVE_75D8* vt;           // 0x0C
    char pad_0x10[0x8];
    int def;                // 0x18
    char pad_0x1C[0x4];
    sV4_75D8 side;          // 0x20
    sV4_75D8 up;            // 0x30
    char pad_0x40[0x10];
    int count;              // 0x50
    char pad_0x54[0x74 - 0x54];
    sFlexNode_75D8* nodes;  // 0x74
    sFlexMesh_75D8* mesh;   // 0x78
    sV4_75D8* pts;          // 0x7C
    sV4_75D8* normals;      // 0x80
    char pad_0x84[0x90 - 0x84];
    sBox_75D8 box;          // 0x90
};

extern sV4_75D8 D_004FF160;

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_75D8(const sV4_75D8& v)
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
static inline sV4_75D8 Scale_75D8(const sV4_75D8& v, float s)
{
    sV4_75D8 r;
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
static inline sV4_75D8 Add_75D8(const sV4_75D8& a, const sV4_75D8& b)
{
    sV4_75D8 r;
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
static inline sV4_75D8 Sub_75D8(const sV4_75D8& a, const sV4_75D8& b)
{
    sV4_75D8 r;
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
static inline sV4_75D8 Cross_75D8(const sV4_75D8& a, const sV4_75D8& b)
{
    sV4_75D8 r;
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
static inline sV4_75D8 Normalize_75D8(const sV4_75D8& v)
{
    sV4_75D8 r;
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
static inline sV4_75D8 MulMat_75D8(const sMat_75D8* m, const sV4_75D8& v)
{
    sV4_75D8 r;
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

// Re-poses the bridge strip from its nodes: two rows of points (node pos +/- the rotated
// side/up offsets), per-triangle normals, the transformed bounding box and the mesh update.
extern "C" void cFlexBridgeNode_updateGrid(void* vself)
{
    sFlexBridge_75D8* self = (sFlexBridge_75D8*)vself;
    int n = self->count;
    sV4_75D8 a = Scale_75D8(D_004FF160, Length_75D8(self->up));
    sV4_75D8 b = Scale_75D8(D_004FF160, Length_75D8(self->side));
    int nverts = n * 2;
    int ntris = (n - 1) * 2;
    sFlexNode_75D8* node = self->nodes;
    int i;
    for (i = 0; i < n; i++) {
        float s, c;
        func_0031BE50(&s, &c, node->angle);
        self->pts[i] = Add_75D8(Add_75D8(node->pos, Scale_75D8(self->up, c)), Scale_75D8(a, s));
        self->pts[n + i] = Sub_75D8(Add_75D8(node->pos, Scale_75D8(self->side, c)), Scale_75D8(b, s));
        node++;
    }
    int k;
    int j = 0;
    for (k = 0; k < ntris; k += 2) {
        self->normals[k] = Normalize_75D8(Cross_75D8(Sub_75D8(self->pts[n + j + 1], self->pts[j]), Sub_75D8(self->pts[n + j], self->pts[j])));
        self->normals[k + 1] = Normalize_75D8(Cross_75D8(Sub_75D8(self->pts[j + 1], self->pts[j]), Sub_75D8(self->pts[n + j + 1], self->pts[j])));
        j++;
    }
    sBox_75D8* box = &self->box;
    sV3_75D8 pos[32];
    float minX = 1e10f, minY = 1e10f, minZ = 1e10f;
    float maxX = -1e10f, maxY = -1e10f, maxZ = -1e10f;
    sMat_75D8* m = (sMat_75D8*)self->vt[24].fn((char*)self + self->vt[24].delta);
    for (int v = 0; v < nverts; v++) {
        pos[v] = self->pts[v];
        sV4_75D8 w = MulMat_75D8(m, self->pts[v]);
        minX = w.X() < minX ? w.X() : minX;
        maxX = maxX < w.X() ? w.X() : maxX;
        minY = w.Y() < minY ? w.Y() : minY;
        maxY = maxY < w.Y() ? w.Y() : maxY;
        minZ = w.Z() < minZ ? w.Z() : minZ;
        maxZ = maxZ < w.Z() ? w.Z() : maxZ;
    }
    sBox_75D8 old = self->box;
    sV4_75D8 e;
    e.x = minX;
    e.y = minY;
    e.z = minZ;
    e.w = 1.0f;
    self->box.min = e;
    e.x = maxX;
    e.y = maxY;
    e.z = maxZ;
    self->box.max = e;
    int id = self->def;
    nash_Nash_movenode_75D8(cObjectInterface_getNash(), 0, id, box, &old);
    self->mesh->vt[3].fn((char*)self->mesh + self->mesh->vt[3].delta, pos);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_render);
#ifdef SKIP_ASM
extern void* D_004A47B8;
extern char* D_004A5B80;

struct sRS_7B80 {
    unsigned int f0_0 : 12;
    unsigned int f0_12 : 8;
    unsigned int f0_20 : 12;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
    int f8;
    int fC;
    int f10;
};

struct sRow_7B80 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sModel_7B80 {
    char pad_0x0[0x7D];
    unsigned char page;     // 0x7D
    short slot;             // 0x7E
    char pad_0x80[0x14];
    void* mesh;             // 0x94
};

static inline int IsLoaded_7B80(sModel_7B80* m)
{
    char* wc = *(char**)D_004A47B8;
    if (m->page == 0xFF)
        return 1;
    int ok = *(int*)(wc + (m->page << 3) + 0x24) == 6;
    return ok ? *(int*)(wc + m->slot * 0x18 + 0x3F0) == 3 : 0;
}

struct sPair_7B80 { sRow_7B80 r[4]; };
struct sVE43_7B80 { short delta; short index; sPair_7B80 (*fn)(void*); };
struct sVE93_7B80 { short delta; short index; int (*fn)(void*, void*, void*, sPair_7B80*); };
struct sVE96_7B80 { short delta; short index; void (*fn)(void*, void*, int, int, int); };
struct sVE7_7B80 { short delta; short index; void (*fn)(void*, void*, void*, void*); };
struct sVE24_7B80 { short delta; short index; void* (*fn)(void*); };

#define TOP_7B80(c) (*(sRS_7B80**)((c) + 0xE84))

// Draws the bridge piece's model (through its handler if it has one) unless it is culled or not streamed in.
extern "C" void cFlexBridgeNode_render(char* self)
{
    sModel_7B80* m = *(sModel_7B80**)(self + 0x18);
    if (!IsLoaded_7B80(m)) {
        return;
    }
    char* ctx = D_004A5B80;
    {
        sVE93_7B80* vt = *(sVE93_7B80**)(ctx + 0x10D8);
        char* thisp = ctx + vt[93].delta;
        char* lo = self + 0x90;
        char* hi = self + 0xA0;
        sVE43_7B80* vt43 = *(sVE43_7B80**)(ctx + 0x10D8);
        sPair_7B80 c = vt43[43].fn(ctx + vt43[43].delta);
        if (vt[93].fn(thisp, lo, hi, &c) == 1) {
            return;
        }
    }
    sVE24_7B80* sv = *(sVE24_7B80**)(self + 0xC);
    void* mtx = sv[24].fn(self + sv[24].delta);
    TOP_7B80(ctx)[1] = TOP_7B80(ctx)[0];
    sRS_7B80* t = TOP_7B80(ctx);
    TOP_7B80(ctx) = t + 1;
    t[1].f4_2 = 5;
    char* h = *(char**)(self + 0x78);
    if (h != 0) {
        sVE7_7B80* hv = *(sVE7_7B80**)(h + 4);
        hv[7].fn(h + hv[7].delta, mtx, (*(sModel_7B80**)(self + 0x18))->mesh, 0);
    } else {
        sVE96_7B80* v96 = *(sVE96_7B80**)(ctx + 0x10D8);
        v96[96].fn(ctx + v96[96].delta, *(void**)(self + 0x18), 0, 0, 0);
    }
    TOP_7B80(ctx)--;
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sFlexBridgeVEntry7D38 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);
extern "C" void func_003545D8(void* p, void* stream);

extern "C" void cFlexBridgeNode_writeToReplayFrame(void* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    func_003545D8((char*)self + 0x50, stream);
    sFlexBridgeVEntry7D38* vt = *(sFlexBridgeVEntry7D38**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x20, 0x30);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", func_00347D90);
#ifdef SKIP_ASM
extern "C" void* func_0034FB00(void* self, int type, void* a2, void* a3);
extern "C" void* func_00372520(void* self, int n);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char D_004900D0[];
extern char D_0048E7C8[];
extern char D_004A4008[];

struct sFlexSeg_347D90 {
    int v[16];
    sFlexSeg_347D90() {}
};

struct sFlexDef_347D90 {
    int f0;
    unsigned int links[8];
    char pad24[0x44 - 0x24];
    int f44;
};

extern "C" void* func_00347D90(char* self, void* a2, void* a3, sFlexDef_347D90* def)
{
    func_0034FB00(self, 1, a2, a3);
    *(char**)(self + 0xC) = D_004900D0;
    int len = def->f44;
    *(int*)(self + 0x20) = 0;
    *(int*)(self + 0x24) = len;
    for (int i = 0; i < 8; i++) {
        if (def->links[i] != 0xFFFFFFFF) {
            *(int*)(self + 0x20) += 1;
        }
    }
    sFlexSeg_347D90** segs = (sFlexSeg_347D90**)(self + 0x50);
    *segs = new (D_0048E7C8, 0x20000000, 0) sFlexSeg_347D90[*(int*)(self + 0x20) - 1];
    *(void**)(self + 0x58) = func_00372520(cMemMan_alloc(0xC, D_004A4008, 0x20000000, 0), *(int*)(self + 0x20) * 8 - 9);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", func_00347EA8);
#ifdef SKIP_ASM
struct sSerVEntry_00347EA8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a, void* stream);

extern "C" void* func_00347EA8(char* self, void* a, void* stream)
{
    sFlexSeg_347D90** segs = (sFlexSeg_347D90**)(self + 0x50);
    cInstanceNode_cInstanceNode(self, a, stream);
    *(char**)(self + 0xC) = D_004900D0;
    sSerVEntry_00347EA8* e = &(*(sSerVEntry_00347EA8**)stream)[2];
    e->fn((char*)stream + e->delta, self + 0x20, 0x30);
    *segs = new (D_0048E7C8, 0x20000000, 0) sFlexSeg_347D90[*(int*)(self + 0x20) - 1];
    *(void**)(self + 0x58) = func_00372520(cMemMan_alloc(0xC, D_004A4008, 0x20000000, 0), *(int*)(self + 0x20) * 8 - 9);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", func_00347F90);
#ifdef SKIP_ASM
struct sFlexBridgeVEntry7F90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

void cMemMan_free(void*);
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);
extern char D_004900D0[];

extern "C" void func_00347F90(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = D_004900D0;
    void* buf = *(void**)((char*)self + 0x50);
    if (buf != 0) {
        cMemMan_free(buf);
    }
    void* obj = *(void**)((char*)self + 0x58);
    if (obj != 0) {
        sFlexBridgeVEntry7F90* vt = *(sFlexBridgeVEntry7F90**)obj;
        vt[1].fn((char*)obj + vt[1].delta, 3);
    }
    cInstanceNode__cInstanceNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_update);
#ifdef SKIP_ASM
struct sFlexBridgeVEntry8008 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void cFlexRailNode_updateSpaceHash(void* self);

extern "C" void cFlexRailNode_update(void* self)
{
    sFlexBridgeVEntry8008* vt1 = *(sFlexBridgeVEntry8008**)((char*)self + 0xC);
    vt1[47].fn((char*)self + vt1[47].delta);
    sFlexBridgeVEntry8008* vt2 = *(sFlexBridgeVEntry8008**)((char*)self + 0xC);
    vt2[48].fn((char*)self + vt2[48].delta);
    cFlexRailNode_updateSpaceHash(self);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", cFlexRailNode_updateSpaceHash);
#ifdef SKIP_ASM
struct sBox00348058 {
    float min[4];
    float max[4];
} __attribute__((aligned(16)));

struct sFlexBridgeVEntry8058 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sObj00348058 {
    char pad_0x00[0xC];
    sFlexBridgeVEntry8058* vt;
    char pad_0x10[0x20];
    sBox00348058 box;
    char pad_0x50[0x4];
    int id;
};

extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_movenode(void* world, int type, int id, sBox00348058* box, sBox00348058* old);

extern "C" void cFlexRailNode_updateSpaceHash(void* p)
{
    sObj00348058* self = (sObj00348058*)p;
    sBox00348058* box = &self->box;
    sBox00348058 old = self->box;
    self->vt[50].fn((char*)self + self->vt[50].delta);
    nash_Nash_movenode(cObjectInterface_getNash(), 2, self->id, box, &old);
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", func_003480C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMat_3480C8 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sRS_3480C8 {
    int f0;
    int flagsA;
    int flagsB;
    int fC;
    short tex;
    short pad;
};

struct sVE_3480C8 { short delta; short index; void* fn; };
typedef sMat_3480C8 (*tGetMat_3480C8)(void*);
typedef int (*tCull_3480C8)(void*, void*, void*, const sMat_3480C8&);

struct sCtx_3480C8 {
    char pad0[0xE84];
    sRS_3480C8* top;        // 0xE84
    char padE88[0x10D8 - 0xE88];
    sVE_3480C8* vt;         // 0x10D8
};
extern sCtx_3480C8* D_004A5B80_3480C8 __asm__("D_004A5B80");
extern int D_004A5980;
extern float D_004FF6C0[8][4];

struct sFlex_3480C8 {
    char pad0[0x20];
    int count;              // 0x20
    char pad24[0xC];
    char boxMin[0x10];      // 0x30
    char boxMax[0x10];      // 0x40
};

extern "C" void func_003480C8(sFlex_3480C8* self)
{
    sCtx_3480C8* ctx = D_004A5B80_3480C8;
    sVE_3480C8* vt = ctx->vt;
    if (((tCull_3480C8)vt[93].fn)((char*)ctx + vt[93].delta, self->boxMin, self->boxMax,
                                  ((tGetMat_3480C8)vt[43].fn)((char*)ctx + vt[43].delta)) == 1) return;
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
    ctx->top->tex = -1;
    ctx->top->flagsA = (ctx->top->flagsA & ~0x7C) | 4;
    int n = self->count - 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 8; j++) {
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", func_00348290);
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

// One cubic segment: P(t) = M * (t^3, t^2, t, 1).
struct sFbM_8290 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (out = m * v).
static inline sFbV_8290 fbApply_8290(const sFbM_8290* m, const sFbV_8290& v)
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
static inline sFbV_8290 splineEval_8290(const sFbM_8290& mat, float t)
{
    sFbV_8290 v;
    v.W() = 1.0f;
    v.Z() = t;
    v.Y() = t * v.Z();
    v.X() = t * v.Y();
    return fbApply_8290(&mat, v);
}

// On Tour: splineSlope_8290(const t4Matrix&, float): the tangent at t, (3t^2, 2t, 1, 0).
static inline sFbV_8290 splineSlope_8290(const sFbM_8290& mat, float t)
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

struct sFbVE_8290 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

// Closest-point result written by func_00348290.
struct sFbHit_8290 {
    sFbV_8290 pos;      // 0x00
    sFbV_8290 dir;      // 0x10: unit tangent at the hit
    sFbV_8290 v20;      // 0x20
    sFbV_8290 v30;      // 0x30
    int pad40;
    int type;           // 0x44
    int pad48;
    int surf;           // 0x4C
    void* obj;          // 0x50
    int f54;
    int f58;
    int f5C;
    int f60;
    int seg;            // 0x64
    float t;            // 0x68
    float f6C;
    float f70;
};

extern sFbV_8290 D_004FF120;

// tIntersectInfo::Init (On Tour name).
static inline void Init_8290(sFbHit_8290* h)
{
    h->type = 2;
    h->f54 = 0;
    h->obj = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->f60 = -1;
    h->seg = -1;
    h->surf = -1;
    h->v20 = D_004FF120;
    h->v30 = D_004FF120;
    h->t = 0.0f;
    h->f6C = 0.0f;
    h->f70 = 0.0f;
}

// The golden-section brackets per coarse sample (rodata).
extern float D_0048E7D8[5];
extern float D_0048E7F0[5];

// Closest point on the bridge's cubic segments to pt: a coarse search over t = k/4, then a
// golden-section refinement; records the hit in res when it beats *bestDist.
// cFlexRailNode::splineIntersect (On Tour name): closest point on the segments' cubics to bpos: a coarse
// search over t = i/4, then a golden-section refinement; records the hit when it beats dist.
extern "C" void func_00348290(char* self, const sFbV_8290* minBpos, const sFbV_8290* maxBpos, const sFbV_8290* bpos,
                              unsigned int splineFilterMask, int* hit, float* dist, sFbHit_8290* intInfo)
{
    if ((*(unsigned int*)(self + 0x24) & splineFilterMask) == 0) {
        return;
    }
    for (int seg = 0; seg < *(int*)(self + 0x20) - 1; seg++) {
        int i;
        sFbV_8290 lastp;
        const float kTopSplit = 0.6180340051651001f;
        const float kBottomSplit = 1.0f - kTopSplit;
        float f0, f1, f2, f3;
        float x0 = 0.0f;
        float x3 = 1.0f;
        sFbV_8290 nextp;
        sFbV_8290 tmp;
        const sFbM_8290& mat = (*(sFbM_8290**)(self + 0x50))[seg];
        lastp = splineEval_8290(mat, x0);
        float cd = fbLength_8290(fbSub_8290(*bpos, lastp));
        int ci = 0;
        for (i = 1; i < 5; i++) {
            nextp = splineEval_8290(mat, i * 0.25f);
            tmp = fbSub_8290(nextp, lastp);
            float r = fbDot_8290(fbSub_8290(*bpos, lastp), tmp) / fbDot_8290(tmp, tmp);
            r = bracket_8290(0.0f, r, 1.0f);
            tmp = fbAdd_8290(lastp, fbScale_8290(tmp, r));
            float d = fbLength_8290(fbSub_8290(*bpos, tmp));
            if (d < cd) {
                ci = i;
                cd = d;
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
        f0 = fbLength_8290(fbSub_8290(lastp, *bpos));
        lastp = splineEval_8290(mat, x1);
        f1 = fbLength_8290(fbSub_8290(lastp, *bpos));
        lastp = splineEval_8290(mat, x2);
        f2 = fbLength_8290(fbSub_8290(lastp, *bpos));
        lastp = splineEval_8290(mat, x3);
        f3 = fbLength_8290(fbSub_8290(lastp, *bpos));
        lastd = f2;
        lastt = x2;
        for (i = 0; i < 24; i++) {
            if (fbAbs_8290(x3 - x0) <= 0.0005000000237487257f) {
                break;
            }
            if (f2 < f1) {
                x0 = x1;
                x1 = x2;
                x2 = kTopSplit * x1 + kBottomSplit * x3;
                lastp = splineEval_8290(mat, x2);
                float tmp = fbLength_8290(fbSub_8290(lastp, *bpos));
                f0 = f1;
                f1 = f2;
                f2 = tmp;
                lastt = x2;
                lastd = tmp;
            } else {
                x3 = x2;
                x2 = x1;
                x1 = kTopSplit * x2 + kBottomSplit * x0;
                lastp = splineEval_8290(mat, x1);
                float tmp = fbLength_8290(fbSub_8290(lastp, *bpos));
                f3 = f2;
                f2 = f1;
                f1 = tmp;
                lastt = x1;
                lastd = tmp;
            }
        }
        if (*hit == 0 || lastd < *dist) {
            *hit = 1;
            *dist = lastd;
            Init_8290(intInfo);
            intInfo->pos = lastp;
            intInfo->dir = fbNormalize_8290(splineSlope_8290(mat, lastt));
            intInfo->obj = *(void**)(self + 0x18);
            sFbVE_8290* vt = *(sFbVE_8290**)(self + 0xC);
            intInfo->f5C = vt[49].fn(self + vt[49].delta, seg);
            intInfo->seg = seg;
            intInfo->f58 = 0;
            intInfo->t = lastt;
            int mesh = intInfo->f5C;
            if (mesh <= 0) {
                mesh = 0;
            }
            short* surf = (short*)(*(char**)(*(char**)(self + 0x18) + 0x88) + 0x1A + mesh * 12);
            if (*surf == -1) {
                intInfo->surf = 0;
            } else {
                intInfo->surf = *surf;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flexbridgenode", func_00348B40);
#ifdef SKIP_ASM
struct sSerVEntry_00348B40 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

extern "C" void func_00348B40(void* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    sSerVEntry_00348B40* vt = *(sSerVEntry_00348B40**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x20, 0x30);
}
#endif


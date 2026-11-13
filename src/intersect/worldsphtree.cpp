#include "common.h"

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_cWorldSphTree);
#ifdef SKIP_ASM
extern void* D_0048E5F0[];
extern char D_0048E550[];
extern "C" void* func_0032C508(void* p);
extern "C" float func_0032C590(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

struct sSphV4 { float x, y, z, w; } __attribute__((aligned(16)));

extern "C" void* cWorldSphTree_cWorldSphTree(void* self, void* a1, int a2)
{
    *(void***)((char*)self + 0x50) = D_0048E5F0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x4) = 0;
    if (a2 != 0) {
        *(int*)self = 0;
    } else {
        *(int*)self = 1;
    }
    *(void**)((char*)self + 0x64) = a1;
    *(void**)((char*)self + 0x60) = a1;
    sSphV4 c = *(sSphV4*)((char*)a1 + 0x80);
    float r = func_0032C590(a1);
    sSphV4 t;
    *(sSphV4*)((char*)self + 0x40) = c;
    t.x = c.x + r;
    t.y = c.y + r;
    t.z = c.z + r;
    t.w = 1.0f;
    *(sSphV4*)((char*)self + 0x20) = t;
    t.x = c.x - r;
    t.y = c.y - r;
    t.z = c.z - r;
    t.w = 1.0f;
    *(sSphV4*)((char*)self + 0x30) = t;
    void* p = cMemMan_alloc(0x140, D_0048E550, 0, 0);
    func_0032C508(p);
    *(void**)((char*)self + 0x68) = p;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003304E8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0048E5F0[];

extern "C" void func_003304E8(void* self, int flags)
{
    *(void***)((char*)self + 0x50) = D_0048E5F0;
    operator_delete(*(int**)((char*)self + 0x68));
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330540);
#ifdef SKIP_ASM
struct sV4_330540 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_330540 add_330540(const sV4_330540& a, const sV4_330540& b)
{
    sV4_330540 r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV4_330540 sub_330540(const sV4_330540& a, const sV4_330540& b)
{
    sV4_330540 r;
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

extern "C" float func_0032C590(void* self);

extern "C" int func_00330540(void* self, float* lo, float* hi)
{
    sV4_330540 ext;
    float rx = func_0032C590(*(void**)((char*)self + 0x60));
    float ry = func_0032C590(*(void**)((char*)self + 0x60));
    float rz = func_0032C590(*(void**)((char*)self + 0x60));
    ext.x = rx;
    ext.y = ry;
    ext.z = rz;
    ext.w = 0.0f;
    sV4_330540 mx = add_330540(*(sV4_330540*)(*(char**)((char*)self + 0x60) + 0x80), ext);
    sV4_330540 mn = sub_330540(*(sV4_330540*)(*(char**)((char*)self + 0x60) + 0x80), ext);
    float lx = lo[0];
    float tx = mx.x;
    if (tx < lx && mn.x < lx) return 0;
    if (hi[0] < tx && hi[0] < mn.x) return 0;
    float ly = lo[1];
    float ty = mx.y;
    if (ty < ly && mn.y < ly) return 0;
    if (hi[1] < ty && hi[1] < mn.y) return 0;
    float lz = lo[2];
    float tz = mx.z;
    if (tz < lz && mn.z < lz) return 0;
    if (!(hi[2] < tz) || !(hi[2] < mn.z)) return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_GetIdealParam);
#ifdef SKIP_ASM
extern "C" float func_0032C590(void* self);

extern "C" float cWorldSphTree_GetIdealParam(void* self)
{
    float r;
    if (*(int*)self != 0) {
        r = func_0032C590(*(void**)((char*)self + 0x60)) * 2.0f;
    } else {
        r = -1.0f;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330710);
#ifdef SKIP_ASM
extern "C" void* func_00327CC8(void* self, void* src);
extern "C" void SphereTree_TransformRel(void* p, void* a, float f);

extern "C" void func_00330710(void* self, void* a1, float f)
{
    func_00327CC8(*(void**)((char*)self + 0x68), *(void**)((char*)self + 0x60));
    *(void**)((char*)self + 0x60) = *(void**)((char*)self + 0x68);
    SphereTree_TransformRel(*(void**)((char*)self + 0x60), a1, f);
    *(int*)((char*)self + 0x4) = 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330778__FPv);
#ifdef SKIP_ASM
int func_00330778(void* self)
{
    int t0 = *(int*)((char*)self + 0x64);
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x60) = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330788);
#ifdef SKIP_ASM
struct sWsVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330788(const sWsVec4& v)
{
    float r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %0, $vi22\n"
        : "=r"(r)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline sWsVec4 wsDiv_330788(const sWsVec4& v, float s)
{
    sWsVec4 r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" int func_0032CA78(void* tree, int a1, int a2, int a3, int a4, sWsVec4* out, int a6);

extern "C" int func_00330788(void* self, int a1, int a2, int a3, int a4, int a5, sWsVec4* outDir, float* outLen)
{
    sWsVec4 v;
    int r = func_0032CA78(*(void**)((char*)self + 0x60), a1, a2, a3, a4, &v, a5);
    if (r != 0) {
        float len = wsLength_330788(v);
        *outLen = len;
        *outDir = wsDiv_330788(v, len);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330828);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330828(const sWsVec4& v)
{
    float r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %0, $vi22\n"
        : "=r"(r)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline sWsVec4 wsDiv_330828(const sWsVec4& v, float s)
{
    sWsVec4 r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" int func_00328030(void* tree, int a1, int a2, int a3, int a4, sWsVec4* out, int a6);

extern "C" int func_00330828(void* self, int a1, int a2, int a3, int a4, int a5, sWsVec4* outDir, float* outLen)
{
    sWsVec4 v;
    int r = func_00328030(*(void**)((char*)self + 0x60), a1, a2, a3, a4, &v, a5);
    if (r != 0) {
        float len = wsLength_330828(v);
        *outLen = len;
        *outDir = wsDiv_330828(v, len);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003308C8);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_3308C8(const sWsVec4& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void wsDivEq_3308C8(sWsVec4& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

struct sWsHit_3308C8 {
    char pad_0x00[0x10];
    sWsVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int func_0032CB58(void* self, void* other, void* a2, void* a3);

extern "C" int func_003308C8(void* self, void* a1, sWsHit_3308C8* hit)
{
    if (func_0032CB58(*(void**)((char*)self + 0x60), a1, &hit->dir, hit) != 0) {
        float len = wsLength_3308C8(hit->dir);
        hit->len = len;
        wsDivEq_3308C8(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330950);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330950(const sWsVec4& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void wsDivEq_330950(sWsVec4& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

struct sWsHit_330950 {
    char pad_0x00[0x10];
    sWsVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int func_0032C928(void* self, void* a1, void* a2, void* a3, void* a4);

extern "C" int func_00330950(void* self, void* a1, void* a2, sWsHit_330950* hit)
{
    if (func_0032C928(*(void**)((char*)self + 0x60), a1, a2, &hit->dir, hit) != 0) {
        float len = wsLength_330950(hit->dir);
        hit->len = len;
        wsDivEq_330950(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003309D8);
#ifdef SKIP_ASM
struct sVEnt_3309D8 { short delta; short index; void* fn; };
typedef int (*Test_3309D8)(void*);

struct sBox_3309D8 {
    float min[4];
    float max[4];
};

struct sBox_3309D8;
typedef sBox_3309D8* (*GetBox_3309D8)(void*);
struct sObj_3309D8 {
    int pad00[3];
    sVEnt_3309D8* vt;
};

struct sNode_3309D8 {
    sNode_3309D8* next;
    int pad04;
    int flags;
    sObj_3309D8* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
};

struct sCtx_3309D8 {
    sNode_3309D8** list;
    int* count;
    int pad08;
    sBox_3309D8* box;
};

static inline int overlapNode_3309D8(const sBox_3309D8* b, const sNode_3309D8* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_3309D8(const sBox_3309D8* b, const sBox_3309D8* o)
{
    int r = 0;
    if (b->min[0] <= o->max[0] && b->max[0] >= o->min[0] &&
        b->min[1] <= o->max[1] && b->max[1] >= o->min[1] &&
        b->min[2] <= o->max[2] && b->max[2] >= o->min[2]) {
        r = 1;
    }
    return r;
}

extern "C" void func_003309D8(sCtx_3309D8* ctx, void* tree)
{
    for (sNode_3309D8* p = *(sNode_3309D8**)((char*)tree + 0x20); p != 0; p = p->next) {
        int flags = p->flags;
        if (flags & 0x20) {
            if (overlapNode_3309D8(ctx->box, p)) {
                int* cnt = ctx->count;
                int n = *cnt;
                ctx->list[n] = p;
                *cnt = n + 1;
            }
        } else if (flags & 0x40) {
            sVEnt_3309D8* vt = p->obj->vt;
            if (((Test_3309D8)vt[44].fn)((char*)p->obj + vt[44].delta)) {
                sVEnt_3309D8* vt2 = p->obj->vt;
                sBox_3309D8* b = ((GetBox_3309D8)vt2[45].fn)((char*)p->obj + vt2[45].delta);
                if (overlapBox_3309D8(ctx->box, b)) {
                    int* cnt = ctx->count;
                    int n = *cnt;
                    ctx->list[n] = p;
                    *cnt = n + 1;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330BA8);
#ifdef SKIP_ASM
struct sSphNode_340A40;
extern "C" void func_00340A40(sSphNode_340A40* node, sCtx_3309D8* ctx);
extern "C" int func_00328360(void* cell, void* box);
extern "C" void func_00338920(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* box, void* tree, void* cell);

struct sOctNode_330BA8 {
    sSphNode_340A40* child[8];
};

struct sCell_330BA8 {
    int level;
    int x, y, z;
    sOctNode_330BA8* tree;
};

struct sGrid_330BA8 {
    sCell_330BA8 cells[8];
    int unkA0;
};

struct sQuery_330BA8 {
    sGrid_330BA8* grid;             // 0x0
    int unk04;                      // 0x4
    int count;                      // 0x8
    sNode_3309D8* items[0x40];      // 0xC
    unsigned int ntris;             // 0x10C
    void* tris[0x40];               // 0x110
    unsigned int nobjs;             // 0x210
};

static inline void collect_330BA8(sOctNode_330BA8* node, sCtx_3309D8* ctx)
{
    func_003309D8(ctx, node);
    if (node->child[0] != 0) func_00340A40(node->child[0], ctx);
    if (node->child[1] != 0) func_00340A40(node->child[1], ctx);
    if (node->child[2] != 0) func_00340A40(node->child[2], ctx);
    if (node->child[3] != 0) func_00340A40(node->child[3], ctx);
    if (node->child[4] != 0) func_00340A40(node->child[4], ctx);
    if (node->child[5] != 0) func_00340A40(node->child[5], ctx);
    if (node->child[6] != 0) func_00340A40(node->child[6], ctx);
    if (node->child[7] != 0) func_00340A40(node->child[7], ctx);
}

// One top-level cell: test it, then either take the whole subtree or recurse into it.
static inline void queryCell_330BA8(sGrid_330BA8* grid, int i, void* box, sCtx_3309D8* ctx, char* flags)
{
    sCell_330BA8* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sOctNode_330BA8* tree = cell->tree;
        char* flag = &flags[i];
        int r = func_00328360(cell, box);
        if (r == 0) {
            collect_330BA8(tree, ctx);
        } else if (r == 2) {
            func_00338920(flag, ctx, ctx, box, tree, cell);
        }
    }
}

extern "C" void func_00330BA8(sQuery_330BA8* self, sBox_3309D8* box)
{
    sCtx_3309D8 ctx;
    char flags[8];
    sGrid_330BA8* grid = self->grid;
    sCtx_3309D8* pctx = &ctx;
    self->unk04 = grid->unkA0;
    ctx.list = self->items;
    ctx.count = &self->count;
    ctx.pad08 = 0x40;
    ctx.box = box;
    self->count = 0;
    queryCell_330BA8(grid, 0, box, pctx, flags);
    queryCell_330BA8(grid, 1, box, pctx, flags);
    queryCell_330BA8(grid, 2, box, pctx, flags);
    queryCell_330BA8(grid, 3, box, pctx, flags);
    queryCell_330BA8(grid, 4, box, pctx, flags);
    queryCell_330BA8(grid, 5, box, pctx, flags);
    queryCell_330BA8(grid, 6, box, pctx, flags);
    queryCell_330BA8(grid, 7, box, pctx, flags);
    self->ntris = 0xFFFFFFFF;
    self->nobjs = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00331450);
#ifdef SKIP_ASM
struct sOctCollect;
extern "C" void func_00340B18(void* self, sOctCollect* out);
extern "C" void func_00339598(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* box, void* tree, void* cell);

struct sTag_331450 {
};

struct sOctNode_331450 {
    void* child[8];         // 0x00
    int pad20[2];           // 0x20
    void** objs;            // 0x28
};

struct sCell_331450 {
    int level;
    int x, y, z;
    sOctNode_331450* tree;
};

struct sGrid_331450 {
    sCell_331450 cells[8];
    int unkA0;
};

struct sQuery_331450 {
    sGrid_331450* grid;             // 0x0
    int unk04;                      // 0x4
    unsigned int count;             // 0x8
    void* items[0x40];              // 0xC
    unsigned int ntris;             // 0x10C
    void* tris[0x40];               // 0x110
    unsigned int nobjs;             // 0x210
    void* objs[0x200];              // 0x214
};

static inline void collect_331450(sOctNode_331450* node, sCtx_3309D8* ctx)
{
    void** p;
    for (p = node->objs; p != 0; p = (void**)*p) {
        ctx->list[(*ctx->count)++] = (sNode_3309D8*)p;
    }
    if (node->child[0] != 0) func_00340B18(node->child[0], (sOctCollect*)ctx);
    if (node->child[1] != 0) func_00340B18(node->child[1], (sOctCollect*)ctx);
    if (node->child[2] != 0) func_00340B18(node->child[2], (sOctCollect*)ctx);
    if (node->child[3] != 0) func_00340B18(node->child[3], (sOctCollect*)ctx);
    if (node->child[4] != 0) func_00340B18(node->child[4], (sOctCollect*)ctx);
    if (node->child[5] != 0) func_00340B18(node->child[5], (sOctCollect*)ctx);
    if (node->child[6] != 0) func_00340B18(node->child[6], (sOctCollect*)ctx);
    if (node->child[7] != 0) func_00340B18(node->child[7], (sOctCollect*)ctx);
}

static inline int queryCell_331450(sGrid_331450* grid, int i, void* box, sCtx_3309D8* ctx)
{
    sCell_331450* cell = &grid->cells[i];
    sOctNode_331450* tree;
    sTag_331450 tag;
    char* flag;
    int r = 1;
    if (grid->cells[i].tree != 0) {
        tree = cell->tree;
        flag = (char*)&tag;
        r = func_00328360(cell, box);
        if (r == 0) {
            collect_331450(tree, ctx);
        } else if (r == 2) {
            func_00339598(flag, ctx, ctx, box, tree, cell);
        }
    }
    return r;
}

extern "C" void func_00331450(sQuery_331450* self, sBox_3309D8* box)
{
    sCtx_3309D8 ctx;
    sGrid_331450* grid = self->grid;
    sCtx_3309D8* pctx = &ctx;
    self->unk04 = grid->unkA0;
    ctx.list = (sNode_3309D8**)self->objs;
    ctx.count = (int*)&self->nobjs;
    ctx.pad08 = 0x200;
    ctx.box = box;
    self->nobjs = 0;
    queryCell_331450(grid, 0, box, pctx);
    queryCell_331450(grid, 1, box, pctx);
    queryCell_331450(grid, 2, box, pctx);
    queryCell_331450(grid, 3, box, pctx);
    queryCell_331450(grid, 4, box, pctx);
    queryCell_331450(grid, 5, box, pctx);
    queryCell_331450(grid, 6, box, pctx);
    queryCell_331450(grid, 7, box, pctx);
    self->ntris = 0xFFFFFFFF;
    self->count = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00331E90);
#ifdef SKIP_ASM
struct sTree_340C20;
extern "C" void func_00340C20(sTree_340C20* tree, sCtx_3309D8* ctx);
extern "C" void func_0033A3D8(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* vbox, void* vnode, void* vcell);

struct sObj_331E90 {
    sObj_331E90* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};

struct sTree_331E90 {
    sTree_331E90* child[8];
    int pad20;
    sObj_331E90* objs;          // 0x24
};

struct sCell_331E90 {
    int level;
    int x, y, z;
    sTree_331E90* tree;
};

struct sGrid_331E90 {
    sCell_331E90 cells[8];
    int unkA0;
};

struct sQuery_331E90 {
    sGrid_331E90* grid;             // 0x0
    int unk04;                      // 0x4
    int count;                      // 0x8
    sNode_3309D8* items[0x40];      // 0xC
    int ntris;                      // 0x10C
    sNode_3309D8* tris[0x40];       // 0x110
    unsigned int nobjs;             // 0x210
};

struct sCtx2_331E90 {
    sCtx_3309D8 nodes;
    sCtx_3309D8 objs;
};

static inline int overlapObj_331E90(const sBox_3309D8* b, const sObj_331E90* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void collect_331E90(sTree_331E90* tree, sCtx_3309D8* ctx)
{
    func_003309D8(ctx, tree);
    sCtx_3309D8* q = (sCtx_3309D8*)((char*)ctx + 0x10);
    for (sObj_331E90* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_331E90(q->box, p)) {
            int* cnt = q->count;
            int n = *cnt;
            q->list[n] = (sNode_3309D8*)p;
            *cnt = n + 1;
        }
    }
    if (tree->child[0] != 0) func_00340C20((sTree_340C20*)tree->child[0], ctx);
    if (tree->child[1] != 0) func_00340C20((sTree_340C20*)tree->child[1], ctx);
    if (tree->child[2] != 0) func_00340C20((sTree_340C20*)tree->child[2], ctx);
    if (tree->child[3] != 0) func_00340C20((sTree_340C20*)tree->child[3], ctx);
    if (tree->child[4] != 0) func_00340C20((sTree_340C20*)tree->child[4], ctx);
    if (tree->child[5] != 0) func_00340C20((sTree_340C20*)tree->child[5], ctx);
    if (tree->child[6] != 0) func_00340C20((sTree_340C20*)tree->child[6], ctx);
    if (tree->child[7] != 0) func_00340C20((sTree_340C20*)tree->child[7], ctx);
}

struct sTag_331E90 {
};

static inline void queryCell_331E90(sGrid_331E90* grid, int i, sBox_3309D8* box, sCtx_3309D8* ctx)
{
    sCell_331E90* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sTree_331E90* tree = cell->tree;
        sTag_331E90 tag;
        char* flag = (char*)&tag;
        int r = func_00328360(cell, box);
        if (r == 0) {
            collect_331E90(tree, ctx);
        } else if (r == 2) {
            func_0033A3D8(flag, ctx, ctx, box, tree, cell);
        }
    }
}

extern "C" void func_00331E90(sQuery_331E90* self, sBox_3309D8* box)
{
    sCtx2_331E90 ctx;
    self->unk04 = self->grid->unkA0;
    ctx.nodes.list = self->items;
    ctx.nodes.count = &self->count;
    ctx.nodes.pad08 = 0x40;
    ctx.nodes.box = box;
    ctx.objs.list = self->tris;
    ctx.objs.count = &self->ntris;
    ctx.objs.pad08 = 0x40;
    ctx.objs.box = box;
    sCtx_3309D8* objs = &ctx.objs;
    self->count = 0;
    *objs->count = 0;
    sGrid_331E90* grid = self->grid;
    sCtx_3309D8* pctx = &ctx.nodes;
    queryCell_331E90(grid, 0, box, pctx);
    queryCell_331E90(grid, 1, box, pctx);
    queryCell_331E90(grid, 2, box, pctx);
    queryCell_331E90(grid, 3, box, pctx);
    queryCell_331E90(grid, 4, box, pctx);
    queryCell_331E90(grid, 5, box, pctx);
    queryCell_331E90(grid, 6, box, pctx);
    queryCell_331E90(grid, 7, box, pctx);
    self->nobjs = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00332DB8);
#ifdef SKIP_ASM
struct sTree_340DC0;
struct sCtx_340DC0;
extern "C" void func_00340DC0(sTree_340DC0* self, sCtx_340DC0* ctx);
extern "C" void func_0033B748(char* flag, sCtx_340DC0* ctx, sCtx_340DC0* ctx2, void* vbox, void* vnode, void* vcell);

struct sObj_332DB8 {
    sObj_332DB8* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};

struct sTree_332DB8 {
    sTree_332DB8* child[8];
    void* nodes;                // 0x20
    sObj_332DB8* objs;          // 0x24
    void** all;                 // 0x28
};

struct sCell_332DB8 {
    int level;
    int x, y, z;
    sTree_332DB8* tree;
};

struct sGrid_332DB8 {
    sCell_332DB8 cells[8];
    int unkA0;
};

struct sQuery_332DB8 {
    sGrid_332DB8* grid;             // 0x0
    int unk04;                      // 0x4
    int count;                      // 0x8
    sNode_3309D8* items[0x40];      // 0xC
    int ntris;                      // 0x10C
    sNode_3309D8* tris[0x40];       // 0x110
    int nobjs;                      // 0x210
    sNode_3309D8* objs[0x200];      // 0x214
};

struct sCtx3_332DB8 {
    sCtx_3309D8 nodes;
    sCtx_3309D8 objs;
    sCtx_3309D8 all;
};

struct sTag_332DB8 {
};

static inline int overlapObj_332DB8(const sBox_3309D8* b, const sObj_332DB8* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void add_332DB8(sCtx_3309D8* s, void* p)
{
    int* cnt = s->count;
    int n = *cnt;
    s->list[n] = (sNode_3309D8*)p;
    *cnt = n + 1;
}

static inline void collect_332DB8(sTree_332DB8* tree, sCtx3_332DB8* ctx)
{
    func_003309D8(&ctx->nodes, tree);
    sCtx_3309D8* s = &ctx->objs;
    for (sObj_332DB8* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_332DB8(s->box, p)) {
            add_332DB8(s, p);
        }
    }
    s = &ctx->all;
    for (void** q = tree->all; q != 0; q = (void**)*q) {
        add_332DB8(s, q);
    }
    if (tree->child[0]) func_00340DC0((sTree_340DC0*)tree->child[0], (sCtx_340DC0*)ctx);
    if (tree->child[1]) func_00340DC0((sTree_340DC0*)tree->child[1], (sCtx_340DC0*)ctx);
    if (tree->child[2]) func_00340DC0((sTree_340DC0*)tree->child[2], (sCtx_340DC0*)ctx);
    if (tree->child[3]) func_00340DC0((sTree_340DC0*)tree->child[3], (sCtx_340DC0*)ctx);
    if (tree->child[4]) func_00340DC0((sTree_340DC0*)tree->child[4], (sCtx_340DC0*)ctx);
    if (tree->child[5]) func_00340DC0((sTree_340DC0*)tree->child[5], (sCtx_340DC0*)ctx);
    if (tree->child[6]) func_00340DC0((sTree_340DC0*)tree->child[6], (sCtx_340DC0*)ctx);
    if (tree->child[7]) func_00340DC0((sTree_340DC0*)tree->child[7], (sCtx_340DC0*)ctx);
}

static inline void queryCell_332DB8(sGrid_332DB8* grid, int i, sBox_3309D8* box, sCtx3_332DB8* ctx)
{
    sCell_332DB8* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sTree_332DB8* tree = cell->tree;
        sTag_332DB8 tag;
        char* flag = (char*)&tag;
        int r = func_00328360(cell, box);
        if (r == 0) {
            collect_332DB8(tree, ctx);
        } else if (r == 2) {
            func_0033B748(flag, (sCtx_340DC0*)ctx, (sCtx_340DC0*)ctx, box, tree, cell);
        }
    }
}

extern "C" void func_00332DB8(sQuery_332DB8* self, sBox_3309D8* box)
{
    sCtx3_332DB8 ctx;
    self->unk04 = self->grid->unkA0;
    ctx.nodes.list = self->items;
    ctx.nodes.count = &self->count;
    ctx.nodes.pad08 = 0x40;
    ctx.nodes.box = box;
    ctx.objs.list = self->tris;
    ctx.objs.count = &self->ntris;
    ctx.objs.pad08 = 0x40;
    ctx.objs.box = box;
    sCtx_3309D8* objs = &ctx.objs;
    self->count = 0;
    *objs->count = 0;
    ctx.all.list = self->objs;
    ctx.all.count = &self->nobjs;
    ctx.all.pad08 = 0x200;
    ctx.all.box = box;
    sCtx_3309D8* all = &ctx.all;
    *all->count = 0;
    sGrid_332DB8* grid = self->grid;
    sCtx3_332DB8* pctx = &ctx;
    queryCell_332DB8(grid, 0, box, pctx);
    queryCell_332DB8(grid, 1, box, pctx);
    queryCell_332DB8(grid, 2, box, pctx);
    queryCell_332DB8(grid, 3, box, pctx);
    queryCell_332DB8(grid, 4, box, pctx);
    queryCell_332DB8(grid, 5, box, pctx);
    queryCell_332DB8(grid, 6, box, pctx);
    queryCell_332DB8(grid, 7, box, pctx);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00333EF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec4_333EF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4_333EF8 D_004FF120_333EF8[] __asm__("D_004FF120");

struct sQuery_333EF8 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sTriObj_333EF8 {
    char pad00[8];
    short id;
    unsigned short flags;
    char pad0C[0x14C];
    float bmin[3];
    float bmax[3];
};

struct sHit_333EF8 {
    sVec4_333EF8 pos;   // 0x00
    sVec4_333EF8 nrm;   // 0x10
    sVec4_333EF8 v20;   // 0x20
    sVec4_333EF8 v30;   // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    int pad48;
    int id;             // 0x4C
    int f50;            // 0x50
    void* obj;          // 0x54
    int f58;            // 0x58
    int f5C;            // 0x5C
    int f60;            // 0x60
    int f64;            // 0x64
    int f68;            // 0x68
    float u;            // 0x6C
    float v;            // 0x70
    int pad74[3];
};

struct sVEnt_333EF8 { short delta; short index; void* fn; };
struct sBox_333EF8;
typedef int (*Test_333EF8)(void*);
typedef sBox_333EF8* (*GetBox_333EF8)(void*);

struct sBox_333EF8 {
    float min[4];
    float max[4];
};

struct sObj_333EF8 {
    int pad00[3];
    sVEnt_333EF8* vt;
};

struct sNode_333EF8 {
    void* next;
    int pad04;
    int flags;
    sObj_333EF8* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
    int pad78[4];
    int active;
};

struct sOwner_333EF8 {
    char pad00[0xA4];
    void* cache;
};

struct sTree_333EF8 {
    sOwner_333EF8* owner;           // 0x0
    int pad04;
    unsigned int count;             // 0x8
    sNode_333EF8* items[0x40];      // 0xC
    unsigned int ntris;             // 0x10C
    sTriObj_333EF8* tris[1];        // 0x110
};

extern "C" void func_003279D0(void* cache, void* src, void** out);
struct sVec4_335960;
struct sHit_003342D0;
extern "C" int func_0032B6E0(void* q, void* obj, void* node, sVec4_335960* pos, sVec4_335960* nrm, float* dist, sVec4_335960* uv, int arg);
extern "C" int func_00334888(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapTri_333EF8(const sQuery_333EF8* q, const sTriObj_333EF8* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapNode_333EF8(const sQuery_333EF8* q, const sNode_333EF8* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_333EF8(const sQuery_333EF8* q, const sBox_333EF8* o)
{
    int r = 0;
    if (q->max[0] > o->min[0] && q->min[0] < o->max[0] &&
        q->max[1] > o->min[1] && q->min[1] < o->max[1] &&
        q->max[2] > o->min[2] && o->max[2] > q->min[2]) {
        r = 1;
    }
    return r;
}

static inline void reset_333EF8(sHit_333EF8* h)
{
    h->type = 2;
    h->obj = 0;
    h->f50 = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->f60 = -1;
    h->f64 = -1;
    h->id = -1;
    h->v20 = *D_004FF120_333EF8;
    h->v30 = *D_004FF120_333EF8;
    h->f68 = 0;
    h->u = 0;
    h->v = 0;
}

extern "C" int func_00333EF8(void* vself, void* vq, sHit_003342D0* vhits, int cap, int arg)
{
    sTree_333EF8* self = (sTree_333EF8*)vself;
    sQuery_333EF8* q = (sQuery_333EF8*)vq;
    sHit_333EF8* hits = (sHit_333EF8*)vhits;
    int n = 0;
    for (unsigned int i = 0; i < self->ntris; i++) {
        sTriObj_333EF8* obj = self->tris[i];
        if ((obj->flags & 0x41) != 0x41) {
            continue;
        }
        if (!overlapTri_333EF8(q, obj)) {
            continue;
        }
        sVec4_333EF8 uv;
        sVec4_333EF8 pos;
        sVec4_333EF8 nrm;
        void* node;
        float dist;
        func_003279D0(self->owner->cache, obj, &node);
        if (!func_0032B6E0(q, obj, node, (sVec4_335960*)&pos, (sVec4_335960*)&nrm, &dist, (sVec4_335960*)&uv, arg)) {
            continue;
        }
        reset_333EF8(&hits[n]);
        hits[n].pos = pos;
        hits[n].nrm = nrm;
        hits[n].id = obj->id;
        hits[n].obj = obj;
        hits[n].dist = dist;
        hits[n].u = uv.x;
        hits[n].v = uv.y;
        n++;
    }
    for (unsigned int j = 0; j < self->count; j++) {
        sNode_333EF8* node = self->items[j];
        if (node->active == 0) {
            continue;
        }
        int flags = node->flags;
        if (flags & 0x20) {
            if (overlapNode_333EF8(q, node)) {
                n += func_00334888(q, node, (char*)hits + n * 0x80, cap - n, 2);
            }
        } else if (flags & 0x40) {
            if (node->obj == 0) {
                continue;
            }
            sVEnt_333EF8* vt = node->obj->vt;
            if (((Test_333EF8)vt[44].fn)((char*)node->obj + vt[44].delta)) {
                sVEnt_333EF8* vt2 = node->obj->vt;
                sBox_333EF8* b = ((GetBox_333EF8)vt2[45].fn)((char*)node->obj + vt2[45].delta);
                if (overlapBox_333EF8(q, b)) {
                    n += func_00334888(q, node, (char*)hits + n * 0x80, cap - n, 2);
                }
            }
        }
    }
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", func_003342D0);
#ifdef SKIP_ASM
struct sHit_003342D0 {
    float pad0[16];
    float dist;             // 0x40
    int pad44[3];
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[10];
    sHit_003342D0() {}
} __attribute__((aligned(16)));

struct sVEnt_003342D0 { short delta; short index; float (*fn)(void*); };
extern "C" int func_00333EF8(void* self, void* q, sHit_003342D0* hits, int max, int a4);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_003342D0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_003342D0(sHit_003342D0* a, sHit_003342D0* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

extern "C" float func_003342D0(void* self, char* q, sHit_003342D0* out, int a3)
{
    sHit_003342D0 hits[64];
    int n = func_00333EF8(self, q, hits, 0x40, a3);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    sVEnt_003342D0* vt = *(sVEnt_003342D0**)(q + 0x50);
    float ref = vt[3].fn(q + vt[3].delta);
    float bestDiff = absf_003342D0(hits[0].dist - ref);
    for (int i = 1; i < n; i++) {
        float d = absf_003342D0(hits[i].dist - ref);
        if (d < bestDiff) {
            bestDiff = d;
            best = i;
        } else if (d == bestDiff) {
            if (isBetter_003342D0(&hits[i], &hits[best])) {
                best = i;
            }
        }
    }
    *out = hits[best];
    return out->dist;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00334458);
#ifdef SKIP_ASM
struct sVEnt_334458 { short delta; short index; void* fn; };
struct sBox_334458;
typedef int (*Test_334458)(void*);
typedef sBox_334458* (*GetBox_334458)(void*);

struct sBox_334458 {
    float min[4];
    float max[4];
};

struct sQuery_334458 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sObj_334458 {
    int pad00[3];
    sVEnt_334458* vt;
};

struct sNode_334458 {
    void* next;
    int pad04;
    int flags;
    sObj_334458* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
};

struct sList_334458 {
    int pad00[2];
    unsigned int count;
    sNode_334458* items[1];
};

extern "C" int func_00334888(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_334458(const sQuery_334458* q, const sNode_334458* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_334458(const sQuery_334458* q, const sBox_334458* o)
{
    int r = 0;
    if (q->max[0] > o->min[0] && q->min[0] < o->max[0] &&
        q->max[1] > o->min[1] && q->min[1] < o->max[1] &&
        q->max[2] > o->min[2] && o->max[2] > q->min[2]) {
        r = 1;
    }
    return r;
}

extern "C" int func_00334458(sList_334458* self, sQuery_334458* q, char* buf, int cap, int full)
{
    int mode = 1;
    if (full) mode = 2;
    int n = 0;
    for (unsigned int i = 0; i < self->count; i++) {
        sNode_334458* node = self->items[i];
        int flags = node->flags;
        if (flags & 0x20) {
            if (overlapNode_334458(q, node)) {
                n += func_00334888(q, node, buf + n * 0x80, cap - n, mode);
            }
        } else if (flags & 0x40) {
            sVEnt_334458* vt = node->obj->vt;
            if (((Test_334458)vt[44].fn)((char*)node->obj + vt[44].delta)) {
                sVEnt_334458* vt2 = node->obj->vt;
                sBox_334458* b = ((GetBox_334458)vt2[45].fn)((char*)node->obj + vt2[45].delta);
                if (overlapBox_334458(q, b)) {
                    n += func_00334888(q, node, buf + n * 0x80, cap - n, mode);
                }
            }
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00334680);
#ifdef SKIP_ASM
struct sBox_00334680 {
    sWsVec4 min;
    sWsVec4 max;
};
extern "C" void func_0035C698(void* a0, sBox_00334680* box, sBox_00334680* box2, sWsVec4* pos, int mask, int* cnt, float* dist, int a7);
extern "C" void func_00348290(void* a0, sBox_00334680* box, sBox_00334680* box2, sWsVec4* pos, int mask, int* cnt, float* dist, int a7);
extern "C" void func_00335128(void* obj, sBox_00334680* box, sWsVec4* pos, int* cnt, float* dist, int a5);

// PORT: PS2-only VU0 inline asm (in-place vector subtract).
static inline void wsSubEq_334680(sWsVec4& a, const sWsVec4& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void wsAddEq_334680(sWsVec4& a, const sWsVec4& b)
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

extern "C" int func_00334680(char* self, sWsVec4* pos, int a2, int mask, float r)
{
    sBox_00334680 box;
    sWsVec4 ext;
    int cnt;
    float dist;
    float zero = 0.0f;
    cnt = 0;
    dist = zero;
    box.min = *pos;
    box.max = *pos;
    ext.x = r;
    ext.y = r;
    ext.z = r;
    ext.w = zero;
    wsSubEq_334680(box.min, ext);
    wsAddEq_334680(box.max, ext);
    for (unsigned int i = 0; i < *(unsigned int*)(self + 0x210); i++) {
        char* o = ((char**)(self + 0x214))[i];
        int type = *(int*)(o + 8);
        if (type == 2) {
            func_0035C698(*(void**)(o + 0xC), &box, &box, pos, mask, &cnt, &dist, a2);
        } else if (type == 3) {
            func_00348290(*(void**)(o + 0xC), &box, &box, pos, mask, &cnt, &dist, a2);
        } else if (type == 1) {
            if (*(int*)(*(char**)(o + 0x68) + 0x1C) & mask) {
                func_00335128(o, &box, pos, &cnt, &dist, a2);
            }
        }
    }
    return cnt;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_initTheAIFwdDiffCache);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cAIFwdDiffCache_cAIFwdDiffCache(void* self);
extern "C" void cAIFwdDiffCache_Init(void* cache, int n);
extern char D_0048E4F8[];

extern "C" void cBXNash_initTheAIFwdDiffCache(void* self)
{
    void* c = cAIFwdDiffCache_cAIFwdDiffCache(cMemMan_alloc(0x10, D_0048E4F8, 0, 0));
    *(void**)((char*)self + 0xA4) = c;
    cAIFwdDiffCache_Init(c, 0x28);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00334850);
#ifdef SKIP_ASM
extern "C" void func_00327828(void* p, int flags);

extern "C" void func_00334850(void* self)
{
    void* p = *(void**)((char*)self + 0xA4);
    if (p != 0) {
        func_00327828(p, 3);
    }
    *(void**)((char*)self + 0xA4) = 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00334888);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// ---- Shared declarations: one type per real thing, used by the functions below ----

// g++ 2.95 vtable entry with an untyped function slot (cast at the call).
struct sWsVtSlot { short delta; short index; void* fn; };

// A polymorphic world object as the list walkers see it (vtable pointer at 0xC).
struct sWsVObj {
    int pad00[3];
    sWsVtSlot* vt;
};

// Constant vector table (0x004FF120; 00341388 also reads it through its ctor vector type).
extern t4Vector D_004FF120_4888[] __asm__("D_004FF120");

struct t4Vector;

// 4x4 matrix with an empty ctor and a VU0 copy (through vf1..vf4).
struct sWsMat_4888 {
    t4Vector row[4];
    sWsMat_4888() {}
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sWsMat_4888(const sWsMat_4888& s)
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
            : "r"(this), "r"(&s)
            : "memory");
    }
};

// A bone of an instance's skeleton (16 bytes): parent index, collision mesh, local matrix.
struct sWsBone_4888 {
    int parent;         // 0x0
    char* mesh;         // 0x4
    int pad08;
    sWsMat_4888* mat;   // 0xC
};

struct sWsBoneTbl_4888 {
    int pad00;
    int count;              // 0x4
    sWsBone_4888* bones;    // 0x8
};

// Per-bone part record of an instance definition (12 bytes).
struct sWsPart_4888 {
    char pad00[0xA];
    short id;           // 0xA
};

struct sWsInstDef_4888 {
    int type;                   // 0x0
    int pad04[3];
    sWsPart_4888 parts[1];      // 0x10
};

// A world instance: collision object, world matrix, skeleton, scale, definition.
struct sWsInst_4888 {
    char pad00[0xC];
    sWsVObj* obj;               // 0xC
    sWsMat_4888 mat;            // 0x10
    char pad50[0x30];
    sWsBoneTbl_4888* tbl;       // 0x80
    float scale;                // 0x84
    sWsInstDef_4888* def;       // 0x88
};

// A triangle hit record (0x80 bytes; empty ctor).
struct sWsHit_4888 {
    t4Vector pos;       // 0x00
    t4Vector nrm;       // 0x10
    t4Vector v20;       // 0x20
    t4Vector v30;       // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    void* tri;          // 0x48
    int id;             // 0x4C
    void* inst;         // 0x50
    void* obj;          // 0x54
    int f58;            // 0x58
    int bone;           // 0x5C
    int pad60[8];
    sWsHit_4888() {}
} __attribute__((aligned(16)));

// The query object (vtable at 0x50).
struct sWsQuery_4888 {
    char pad00[0x50];
    sWsVtSlot* vt;              // 0x50
};

typedef int (*Test_4888)(void*);
typedef int (*Query_4888)(void*, void*, void*, int, int);
typedef sWsMat_4888* (*GetMat_4888)(void*, void*);
typedef void (*SetXf_4888)(void*, float, sWsMat_4888*);
typedef void (*Reset_4888)(void*);
typedef int (*Tris_4888)(void*, void*, void*, sWsHit_4888*, int);
typedef int (*Hit_4888)(void*, void*, void*);
typedef int (*Mesh_4888)(void*, void*, sWsHit_4888*, int);

extern void* D_004A47B8;
void* func_003A6CC8_4888(void* self, void* a1) __asm__("func_003A6CC8__FPvT0");
void* func_003A6CD8_4888(void* self, void* a1) __asm__("func_003A6CD8__FPvT0");
extern "C" void* func_003A6CE8(void* self, int a1);
void* func_003A6D00_4888(void* self, int a1) __asm__("func_003A6D00__FPvi");
// PORT: callers pass the hit capacity as a fifth argument the function never reads.
extern "C" int func_0032C0F8_cap(void* self, void* mesh, int flag, sWsHit_4888* out, int cap) __asm__("func_0032C0F8");

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_4888(sWsMat_4888* d, const sWsMat_4888* a, const sWsMat_4888* b)
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
static inline void vu0CopyMat_4888(sWsMat_4888* d, const sWsMat_4888* s)
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
static inline t4Vector vu0MtxMul_4888(const sWsMat_4888* m, const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (d = v * s).
static inline void vu0ScaleVector_4888(t4Vector& d, const t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(d), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
}

// d = v * s
static inline void ScaleTo_4888(t4Vector& d, const t4Vector& v, float s)
{
    vu0ScaleVector_4888(d, v, s);
}

// v * s
static inline t4Vector Scale_4888(const t4Vector& v, float s)
{
    t4Vector r;
    ScaleTo_4888(r, v, s);
    return r;
}

// A vector built from four floats.
struct sVecK_4888 : public t4Vector {
    sVecK_4888(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

// Component-wise product.
static inline sVecK_4888 MulVec_4888(const t4Vector& a, const t4Vector& b)
{
    return sVecK_4888(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

static inline t4Vector Row_4888(const sWsMat_4888& m, int i)
{
    return m.row[i];
}

// In-place inverse of a rigid transform: transpose the rotation, rotate the negated translation back.
static inline void InvertOrtho_4888(sWsMat_4888& m)
{
    float f;
    f = m.row[0].y;
    m.row[0].y = m.row[1].x;
    m.row[1].x = f;
    f = m.row[0].z;
    m.row[0].z = m.row[2].x;
    m.row[2].x = f;
    f = m.row[1].z;
    m.row[1].z = m.row[2].y;
    m.row[2].y = f;
    t4Vector v = Scale_4888(Row_4888(m, 3), -1.0f);
    v.w = 0.0f;
    v = vu0MtxMul_4888(&m, v);
    v.w = 1.0f;
    m.row[3] = v;
}

// Part record i of an instance definition.
static inline sWsPart_4888* GetPart_4888(sWsInstDef_4888* def, int i)
{
    return &def->parts[i];
}

// Collects the triangle hits of instance `node` against query q into buf (mode 1: unnamed parts only, mode 2: named
// parts only); bone matrices come from the instance's animated object or are built here from the skeleton.
extern "C" int func_4888(sWsQuery_4888* q, sWsInst_4888* node, sWsHit_4888* buf, int cap, int mode) __asm__("func_00334888");
extern "C" int func_4888(sWsQuery_4888* q, sWsInst_4888* node, sWsHit_4888* buf, int cap, int mode)
{
    if (node->obj != 0) {
        sWsVtSlot* vt = node->obj->vt;
        if (((Test_4888)vt[38].fn)((char*)node->obj + vt[38].delta)) {
            sWsVtSlot* vt2 = node->obj->vt;
            return ((Query_4888)vt2[39].fn)((char*)node->obj + vt2[39].delta, q, buf, cap, mode);
        }
    }
    int type = node->def->type;
    if (type != 3 && type != 1 && type != 2)
        return 0;
    sWsVObj* obj = node->obj;
    sWsMat_4888 mats[24];
    sWsMat_4888* pMat = &node->mat;
    int useLocal = 1;
    sWsMat_4888* boneMats = mats;
    if (obj != 0) {
        sWsVtSlot* vt = obj->vt;
        if (((Test_4888)vt[26].fn)((char*)obj + vt[26].delta)) {
            useLocal = 0;
            sWsVtSlot* vt2 = obj->vt;
            boneMats = ((GetMat_4888)vt2[28].fn)((char*)obj + vt2[28].delta, node);
        } else {
            sWsVtSlot* vt2 = obj->vt;
            pMat = ((GetMat_4888)vt2[25].fn)((char*)obj + vt2[25].delta, node);
        }
    }
    sWsBoneTbl_4888* tbl = node->tbl;
    sWsBone_4888* bone = tbl->bones;
    float s = node->scale;
    t4Vector scl;
    scl.x = s;
    scl.y = s;
    scl.z = s;
    scl.w = 1.0f;
    int n = 0;
    sWsMat_4888* m = boneMats;
    for (int i = 0, k = 0; i < tbl->count; i++, bone++, m++) {
        if (useLocal) {
            int parent = bone->parent;
            if (parent != -1) {
                sWsMat_4888 t2;
                sWsMat_4888 a;
                t4Vector res;
                t4Vector r3;
                sWsMat_4888* lm = bone->mat;
                res = lm->row[0];
                a.row[0] = res;
                res = lm->row[1];
                a.row[1] = res;
                res = lm->row[2];
                a.row[2] = res;
                r3 = lm->row[3];
                res.x = r3.x * scl.x;
                res.y = r3.y * scl.y;
                res.z = r3.z * scl.z;
                res.w = r3.w * scl.w;
                a.row[3] = res;
                sWsMat_4888 t;
                sWsMat_4888* pt2 = &t2;
                sWsMat_4888* pt = &t;
                vu0MulMat_4888(pt, &mats[parent], &a);
                vu0CopyMat_4888(pt2, pt);
                vu0CopyMat_4888(&mats[i], pt2);
            } else {
                sWsMat_4888 t2;
                sWsMat_4888 a;
                t4Vector res;
                t4Vector r3;
                sWsMat_4888* lm = bone->mat;
                res = lm->row[0];
                a.row[0] = res;
                res = lm->row[1];
                a.row[1] = res;
                res = lm->row[2];
                a.row[2] = res;
                r3 = lm->row[3];
                res.x = r3.x * scl.x;
                res.y = r3.y * scl.y;
                res.z = r3.z * scl.z;
                res.w = r3.w * scl.w;
                a.row[3] = res;
                sWsMat_4888 t;
                sWsMat_4888* pt2 = &t2;
                sWsMat_4888* pt = &t;
                vu0MulMat_4888(pt, pMat, &a);
                vu0CopyMat_4888(pt2, pt);
                vu0CopyMat_4888(&mats[i], pt2);
            }
        }
        if (bone->mesh == 0)
            continue;
        if (mode == 2) {
            if (GetPart_4888(node->def, i)->id == -1) {
                if (type != 2)
                    k++;
                continue;
            }
        } else if (mode == 1) {
            if (GetPart_4888(node->def, i)->id != -1) {
                if (type != 2)
                    k++;
                continue;
            }
        }
        sWsHit_4888 ii[64];
        sWsMat_4888 inv = *m;
        InvertOrtho_4888(inv);
        int inum = 0;
        void* tri = 0;
        ((SetXf_4888)q->vt[1].fn)((char*)q + q->vt[1].delta, 1.0f / node->scale, &inv);
        if (type == 2) {
            inum = ((Tris_4888)q->vt[7].fn)((char*)q + q->vt[7].delta, bone->mesh, bone->mesh + 0xC, ii, 64);
        } else {
            if (((Hit_4888)q->vt[9].fn)((char*)q + q->vt[9].delta, bone->mesh, bone->mesh + 0xC)) {
                if (type == 3) {
                    tri = func_003A6CE8(func_003A6CD8_4888(D_004A47B8, node), k);
                    inum = ((Mesh_4888)q->vt[6].fn)((char*)q + q->vt[6].delta, tri, ii, 64);
                } else {
                    inum = func_0032C0F8_cap(q, func_003A6D00_4888(func_003A6CC8_4888(D_004A47B8, node), k),
                                             *(int*)(bone->mesh + 0x18) & 1, ii, 64);
                }
            }
            k++;
        }
        ((Reset_4888)q->vt[2].fn)((char*)q + q->vt[2].delta);
        for (int j = 0; j < inum; j++) {
            ii[j].nrm = vu0MtxMul_4888(m, ii[j].nrm);
            buf[n] = ii[j];
            buf[n].pos = vu0MtxMul_4888(m, MulVec_4888(buf[n].pos, scl));
            buf[n].type = 1;
            buf[n].inst = node;
            buf[n].bone = i;
            buf[n].id = -1;
            buf[n].tri = tri;
            buf[n].v20 = *D_004FF120_4888;
            buf[n].v30 = *D_004FF120_4888;
            buf[n].obj = 0;
            if (GetPart_4888(node->def, i)->id != -1)
                buf[n].id = GetPart_4888(node->def, i)->id;
            n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00335128);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm throughout; needs C fallbacks off-PS2.
struct sV_335128
{
    float x, y, z, w;
    sV_335128() {}
    // t4vect::x()/y()/z()/w() in On Tour (component accessors returning a reference).
    float& X() { return x; }
    float& Y() { return y; }
    float& Z() { return z; }
    float& W() { return w; }
} __attribute__((aligned(16)));

extern sV_335128 D_004FF120_335128 __asm__("D_004FF120");

struct sF5_335128 {
    float v[5];
};
// Segment brackets for the golden-section refinement, by nearest sample.
extern const sF5_335128 D_0048E560_335128 __asm__("D_0048E560");
extern const sF5_335128 D_0048E578_335128 __asm__("D_0048E578");

// A cubic curve object: coefficient rows at 0x10 (point = M * (t^3, t^2, t, 1)) and a bounding box.
struct sCurveObj_335128 {
    char pad00[0x10];
    sV_335128 m[4];         // 0x10
    char pad50[0x18];
    char* owner;            // 0x68 (id at +0x28)
    float bmin[3];          // 0x6C
    float bmax[3];          // 0x78
};

struct sHit_335128 {
    sV_335128 pos;      // 0x00
    sV_335128 nrm;      // 0x10
    sV_335128 v20;      // 0x20
    sV_335128 v30;      // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    int pad48;
    int id;             // 0x4C
    int f50;            // 0x50
    void* obj;          // 0x54
    void* f58;          // 0x58
    int f5C;            // 0x5C
    int f60;            // 0x60
    int f64;            // 0x64
    float t;            // 0x68
    float u;            // 0x6C
    float v;            // 0x70
};

// PORT: PS2-only VU0 macro-mode asm (4x4 matrix times vector).
static inline sV_335128 MatMulV_335128(const sV_335128* m, const sV_335128& v)
{
    sV_335128 r;
    __asm__(
        "lqc2       $vf8, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "lqc2       $vf5, 0x10(%1)\n"
        "lqc2       $vf6, 0x20(%1)\n"
        "lqc2       $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2       $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV_335128 Sub_335128(const sV_335128& a, const sV_335128& b)
{
    sV_335128 r;
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
static inline sV_335128 Add_335128(const sV_335128& a, const sV_335128& b)
{
    sV_335128 r;
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
static inline sV_335128 Scale_335128(const sV_335128& v, float s)
{
    sV_335128 r;
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

// PORT: PS2-only VU0 inline asm (vector length).
static inline float Length_335128(const sV_335128& v)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_335128(const sV_335128& a, const sV_335128& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sV_335128 Normalize_335128(const sV_335128& v)
{
    sV_335128 r;
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

// PORT: PS2 abs.s asm helper; use fabsf on PC.
static inline float Abs_335128(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` (min) operator, removed in GCC 4.3.
static inline float Clamp_335128(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}

// On Tour: splineEval(const t4Matrix& emat, float w): the point at w, (w^3, w^2, w, 1) through the coefficient rows.
// The components are set through the reference accessors, w first, as On Tour does.
static inline sV_335128 splineEval_335128(const sV_335128* emat, float w)
{
    sV_335128 v;
    v.W() = 1.0f;
    v.Z() = w;
    v.Y() = w * v.Z();
    v.X() = w * v.Y();
    return MatMulV_335128(emat, v);
}

// On Tour: splineSlope(const t4Matrix& emat, float w): the tangent at w, (3w^2, 2w, 1, 0).
static inline sV_335128 splineSlope_335128(const sV_335128* emat, float w)
{
    sV_335128 v;
    v.W() = 0.0f;
    v.Z() = 1.0f;
    v.Y() = w * 2.0f;
    v.X() = w * (w * 3.0f);
    return MatMulV_335128(emat, v);
}

static inline void reset_335128(sHit_335128* h)
{
    h->type = 2;
    h->obj = 0;
    h->f50 = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->f60 = -1;
    h->f64 = -1;
    h->id = -1;
    h->v20 = D_004FF120_335128;
    h->v30 = D_004FF120_335128;
    h->t = 0;
    h->u = 0;
    h->v = 0;
}

// intersect::staticSplineSegmentIntersect (On Tour name): closest point on a spline segment to intersectpos,
// a coarse 4-segment scan, then a golden-section search.
// The unit declares func_00335128 with an int last parameter; bind the typed definition by asm label.
extern "C" void func_00335128_typed(sCurveObj_335128* pSplineSeg, sBox_00334680* box, sV_335128* intersectpos, bool* hit, float* intersectdist, sHit_335128* intersectinfo) __asm__("func_00335128");
extern "C" void func_00335128_typed(sCurveObj_335128* pSplineSeg, sBox_00334680* box, sV_335128* intersectpos, bool* hit, float* intersectdist, sHit_335128* intersectinfo)
{
    if (box->max.x < pSplineSeg->bmin[0] || box->min.x > pSplineSeg->bmax[0] ||
        box->max.y < pSplineSeg->bmin[1] || box->min.y > pSplineSeg->bmax[1] ||
        box->max.z < pSplineSeg->bmin[2] || box->min.z > pSplineSeg->bmax[2]) {
        return;
    }

    int i;
    sV_335128 lastp;
    const float kTopSplit = 0.6180340051651001f;
    const float kBottomSplit = 1.0f - kTopSplit;
    float f0, f1, f2, f3;
    float x0 = 0.0f;
    float x3 = 1.0f;
    sV_335128 nextp;
    sV_335128 tmp;
    lastp = splineEval_335128(pSplineSeg->m, x0);
    float cd = Length_335128(Sub_335128(*intersectpos, lastp));
    int ci = 0;
    for (i = 1; i < 5; i++) {
        nextp = splineEval_335128(pSplineSeg->m, i * 0.25f);
        tmp = Sub_335128(nextp, lastp);
        float r = Dot_335128(Sub_335128(*intersectpos, lastp), tmp) / Dot_335128(tmp, tmp);
        r = Clamp_335128(r, 1.0f);
        tmp = Add_335128(lastp, Scale_335128(tmp, r));
        float d = Length_335128(Sub_335128(*intersectpos, tmp));
        if (d < cd) {
            ci = i;
            cd = d;
        }
        lastp = nextp;
    }
    const sF5_335128 minx = D_0048E560_335128;
    const sF5_335128 maxx = D_0048E578_335128;
    x0 = minx.v[ci];
    x3 = maxx.v[ci];
    float x1 = x0 + (x3 - x0) * kBottomSplit;
    float x2 = x0 + (x3 - x0) * kTopSplit;
    lastp = splineEval_335128(pSplineSeg->m, x0);
    f0 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(pSplineSeg->m, x1);
    f1 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(pSplineSeg->m, x2);
    f2 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(pSplineSeg->m, x3);
    f3 = Length_335128(Sub_335128(lastp, *intersectpos));
    float lastd = f2;
    float lastt = x2;
    for (i = 0; i < 24 && !(Abs_335128(x3 - x0) <= 0.0005000000237487257f); i++) {
        if (f2 < f1) {
            x0 = x1;
            x1 = x2;
            x2 = x1 * kTopSplit + x3 * kBottomSplit;
            lastp = splineEval_335128(pSplineSeg->m, x2);
            float tmp = Length_335128(Sub_335128(lastp, *intersectpos));
            f0 = f1;
            f1 = f2;
            f2 = tmp;
            lastt = x2;
            lastd = tmp;
        } else {
            x3 = x2;
            x2 = x1;
            x1 = x2 * kTopSplit + x0 * kBottomSplit;
            lastp = splineEval_335128(pSplineSeg->m, x1);
            float tmp = Length_335128(Sub_335128(lastp, *intersectpos));
            f3 = f2;
            f2 = f1;
            f1 = tmp;
            lastt = x1;
            lastd = tmp;
        }
    }
    if (!*hit || lastd < *intersectdist) {
        *hit = true;
        *intersectdist = lastd;
        reset_335128(intersectinfo);
        intersectinfo->pos = lastp;
        intersectinfo->nrm = Normalize_335128(splineSlope_335128(pSplineSeg->m, lastt));
        intersectinfo->id = *(int*)(pSplineSeg->owner + 0x28);
        intersectinfo->f50 = 0;
        intersectinfo->f5C = -1;
        intersectinfo->f58 = pSplineSeg->owner;
        intersectinfo->t = lastt;
        intersectinfo->f64 = -1;
        intersectinfo->obj = 0;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00335960);
#ifdef SKIP_ASM
struct sVec4_335960 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4_335960 D_004FF120[];

struct sQuery_335960 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sTriObj_335960 {
    char pad00[8];
    short id;
    unsigned short flags;
    char pad0C[0x14C];
    float bmin[3];
    float bmax[3];
};

struct sHit_335960 {
    sVec4_335960 pos;   // 0x00
    sVec4_335960 nrm;   // 0x10
    sVec4_335960 v20;   // 0x20
    sVec4_335960 v30;   // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    int pad48;
    int id;             // 0x4C
    int f50;            // 0x50
    void* obj;          // 0x54
    int f58;            // 0x58
    int f5C;            // 0x5C
    int f60;            // 0x60
    int f64;            // 0x64
    int f68;            // 0x68
    float u;            // 0x6C
    float v;            // 0x70
    int pad74[3];
};

struct sCtx_335960 {
    sQuery_335960* q;
    sHit_335960* hits;
    int cap;
    int* count;
    int arg;
    void* cache;
};

extern "C" void func_003279D0(void* cache, void* src, void** out);
extern "C" int func_0032B6E0(void* q, void* obj, void* node, sVec4_335960* pos, sVec4_335960* nrm, float* dist, sVec4_335960* uv, int arg);

static inline int overlap_335960(const sQuery_335960* q, const sTriObj_335960* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline void reset_335960(sHit_335960* h)
{
    h->type = 2;
    h->obj = 0;
    h->f50 = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->f60 = -1;
    h->f64 = -1;
    h->id = -1;
    h->v20 = *D_004FF120;
    h->v30 = *D_004FF120;
    h->f68 = 0;
    h->u = 0;
    h->v = 0;
}

extern "C" void func_00335960(void* vctx, void* vobj)
{
    sCtx_335960* ctx = (sCtx_335960*)vctx;
    sTriObj_335960* obj = (sTriObj_335960*)vobj;
    sVec4_335960 pos;
    sVec4_335960 nrm;
    sVec4_335960 uv;
    void* node;
    float dist;
    if ((obj->flags & 0x41) != 0x41) return;
    if (!overlap_335960(ctx->q, obj)) return;
    func_003279D0(ctx->cache, obj, &node);
    if (!func_0032B6E0(ctx->q, obj, node, &pos, &nrm, &dist, &uv, ctx->arg)) return;
    reset_335960(&ctx->hits[*ctx->count]);
    ctx->hits[*ctx->count].pos = pos;
    ctx->hits[*ctx->count].nrm = nrm;
    ctx->hits[*ctx->count].id = obj->id;
    ctx->hits[*ctx->count].obj = obj;
    ctx->hits[*ctx->count].dist = dist;
    ctx->hits[*ctx->count].u = uv.x;
    ctx->hits[*ctx->count].v = uv.y;
    (*ctx->count)++;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00335B90);
#ifdef SKIP_ASM
struct sVEnt_335B90 { short delta; short index; void* fn; };
struct sBox_335B90;
typedef int (*Test_335B90)(void*);
typedef sBox_335B90* (*GetBox_335B90)(void*);

struct sBox_335B90 {
    float min[4];
    float max[4];
};

struct sQuery_335B90 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sObj_335B90 {
    int pad00[3];
    sVEnt_335B90* vt;
};

struct sNode_335B90 {
    void* next;
    int pad04;
    int flags;
    sObj_335B90* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
    int pad78[4];
    int active;
};

struct sCtx_335B90 {
    sQuery_335B90* q;
    char* buf;
    int cap;
    int* count;
};

extern "C" int func_00334888(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_335B90(const sQuery_335B90* q, const sNode_335B90* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_335B90(const sQuery_335B90* q, const sBox_335B90* o)
{
    int r = 0;
    if (q->max[0] > o->min[0] && q->min[0] < o->max[0] &&
        q->max[1] > o->min[1] && q->min[1] < o->max[1] &&
        q->max[2] > o->min[2] && q->min[2] < o->max[2]) {
        r = 1;
    }
    return r;
}

static inline void collect_335B90(sCtx_335B90* ctx, sNode_335B90* node)
{
    int n = *ctx->count;
    *ctx->count += func_00334888(ctx->q, node, ctx->buf + n * 0x80, ctx->cap - n, 2);
}

extern "C" void func_00335B90(void* vctx, void* vnode)
{
    sCtx_335B90* ctx = (sCtx_335B90*)vctx;
    sNode_335B90* node = (sNode_335B90*)vnode;
    if (node->active == 0) return;
    int flags = node->flags;
    if (flags & 0x20) {
        if (overlapNode_335B90(ctx->q, node)) {
            collect_335B90(ctx, node);
        }
    } else if (flags & 0x40) {
        if (node->obj == 0) return;
        sVEnt_335B90* vt = node->obj->vt;
        if (((Test_335B90)vt[44].fn)((char*)node->obj + vt[44].delta)) {
            sVEnt_335B90* vt2 = node->obj->vt;
            sBox_335B90* b = ((GetBox_335B90)vt2[45].fn)((char*)node->obj + vt2[45].delta);
            if (overlapBox_335B90(ctx->q, b)) {
                collect_335B90(ctx, node);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00335D78);
#ifdef SKIP_ASM
struct sQuad_335D78 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sQry_335D78 {
    char pad00[0x20];
    sQuad_335D78 max;   // 0x20
    sQuad_335D78 min;   // 0x30
};

struct sBox_335D78 {
    sQuad_335D78 min;
    sQuad_335D78 max;
};

struct sOct_335D78 {
    void* child[8];
    void* listB;        // 0x20
    void* listA;        // 0x24
};

struct sCell_335D78 {
    int level;
    int x, y, z;
    sOct_335D78* tree;
};

struct sGrid_335D78 {
    sCell_335D78 cells[8];
    int unkA0;
    void* cache;        // 0xA4
};

struct sTag_335D78 {
};

extern "C" void func_00340FA0(void* self, void* ctx);
extern "C" void func_0033CCF8(char* flag, sCtx_335960* ctx, sCtx_335960* ctx2, void* box, void* tree, void* cell);

static inline void collect_335D78(sOct_335D78* node, sCtx_335960* ctx)
{
    void** p;
    for (p = (void**)node->listA; p != 0; p = (void**)*p) {
        func_00335960(ctx, p);
    }
    for (p = (void**)node->listB; p != 0; p = (void**)*p) {
        func_00335B90(ctx, p);
    }
    if (node->child[0] != 0) func_00340FA0(node->child[0], ctx);
    if (node->child[1] != 0) func_00340FA0(node->child[1], ctx);
    if (node->child[2] != 0) func_00340FA0(node->child[2], ctx);
    if (node->child[3] != 0) func_00340FA0(node->child[3], ctx);
    if (node->child[4] != 0) func_00340FA0(node->child[4], ctx);
    if (node->child[5] != 0) func_00340FA0(node->child[5], ctx);
    if (node->child[6] != 0) func_00340FA0(node->child[6], ctx);
    if (node->child[7] != 0) func_00340FA0(node->child[7], ctx);
}

static inline void queryCell_335D78(sGrid_335D78* grid, int i, sBox_335D78* box, sCtx_335960* ctx)
{
    sCell_335D78* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sOct_335D78* tree = cell->tree;
        sTag_335D78 tag;
        char* flag = (char*)&tag;
        int r = func_00328360(cell, box);
        if (r == 0) {
            collect_335D78(tree, ctx);
        } else if (r == 2) {
            func_0033CCF8(flag, ctx, ctx, box, tree, cell);
        }
    }
}

struct sHit_00336850;

extern "C" int func_00335D78(void* vself, void* vq, sHit_00336850* hits, int cap, int arg)
{
    sGrid_335D78* grid = (sGrid_335D78*)vself;
    sQry_335D78* q = (sQry_335D78*)vq;
    sCtx_335960 ctx;
    sBox_335D78 box;
    int count;
    ctx.q = (sQuery_335960*)q;
    ctx.hits = (sHit_335960*)hits;
    ctx.cap = cap;
    ctx.count = &count;
    ctx.arg = arg;
    ctx.cache = grid->cache;
    box.min = q->min;
    box.max = q->max;
    sCtx_335960* pctx = &ctx;
    count = 0;
    queryCell_335D78(grid, 0, &box, pctx);
    queryCell_335D78(grid, 1, &box, pctx);
    queryCell_335D78(grid, 2, &box, pctx);
    queryCell_335D78(grid, 3, &box, pctx);
    queryCell_335D78(grid, 4, &box, pctx);
    queryCell_335D78(grid, 5, &box, pctx);
    queryCell_335D78(grid, 6, &box, pctx);
    queryCell_335D78(grid, 7, &box, pctx);
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", func_00336850);
#ifdef SKIP_ASM
struct sHit_00336850 {
    float pad0[16];
    float dist;             // 0x40
    int pad44[3];
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[10];
    sHit_00336850() {}
} __attribute__((aligned(16)));

struct sVEnt_00336850 { short delta; short index; float (*fn)(void*); };
extern "C" int func_00335D78(void* self, void* q, sHit_00336850* hits, int max, int a4);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_00336850(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_00336850(sHit_00336850* a, sHit_00336850* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

extern "C" float func_00336850(void* self, char* q, sHit_00336850* out, int a3)
{
    sHit_00336850 hits[64];
    int n = func_00335D78(self, q, hits, 0x40, a3);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    sVEnt_00336850* vt = *(sVEnt_00336850**)(q + 0x50);
    float ref = vt[3].fn(q + vt[3].delta);
    float bestDiff = absf_00336850(hits[0].dist - ref);
    for (int i = 1; i < n; i++) {
        float d = absf_00336850(hits[i].dist - ref);
        if (d < bestDiff) {
            bestDiff = d;
            best = i;
        } else if (d == bestDiff) {
            if (isBetter_00336850(&hits[i], &hits[best])) {
                best = i;
            }
        }
    }
    *out = hits[best];
    return out->dist;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003369D8);
#ifdef SKIP_ASM
struct sQuad_369D8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sHit_003369D8 {
    sQuad_369D8 p;          // 0x00
    sQuad_369D8 n;          // 0x10
    float pad20[8];         // 0x20
    float dist;             // 0x40
    int pad44[3];
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[10];
    sHit_003369D8() {}
} __attribute__((aligned(16)));

struct sQuery_369D8 {
    void* seg;              // 0x00
    sHit_003369D8* hits;    // 0x04
    int max;                // 0x08
    int* count;             // 0x0C
    int f10;                // 0x10
    void* f14;              // 0x14
};

struct sCell_369D8 {
    int f0, f4, f8, fC;
    void* node;             // 0x10
};

extern "C" void* func_0032E100(void* seg, const sQuad_369D8& a, const sQuad_369D8& b, int n, float r);
struct sCtx_369D8 {
    float* pos;             // 0x0
    char flags[8];          // 0x4
};

struct sTrav_369D8 {
    int count;              // 0x0
    sCtx_369D8 ctx;         // 0x4
};

extern "C" void func_0033DBE8(char* flag, sQuery_369D8* q, sQuery_369D8* q2, sCtx_369D8* ctx, void* node, sCell_369D8* cell);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_003369D8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` (min) operator.
static inline float clamp_003369D8(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

static inline int isBetter_003369D8(sHit_003369D8* a, sHit_003369D8* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

static inline void visit_003369D8(sCell_369D8* c, char* flag, sQuery_369D8* q, sCtx_369D8* p)
{
    func_0033DBE8(flag, q, q, p, c->node, c);
}

extern "C" int func_003369D8(sCell_369D8* self, float* pos, sQuad_369D8* outP, sQuad_369D8* outN)
{
    char seg[0xB0];
    float lo = pos[2] - 100000.0f;
    float hi = pos[2] + 100000.0f;
    float t = clamp_003369D8((pos[2] + 20.0f - lo) / (hi - lo), 0.0f, 1.0f);
    {
        sQuad_369D8 a;
        a.x = pos[0];
        a.y = pos[1];
        a.z = lo;
        a.w = 1.0f;
        sQuad_369D8 b;
        b.x = pos[0];
        b.y = pos[1];
        b.z = hi;
        b.w = 1.0f;
        func_0032E100(seg, a, b, 2, t);
    }
    sHit_003369D8 hits[64];
    sQuery_369D8 q;
    q.seg = seg;
    q.hits = hits;
    q.max = 0x40;
    sTrav_369D8 tr;
    q.count = &tr.count;
    q.f10 = 0;
    q.f14 = *(void**)((char*)self + 0xA4);
    tr.count = 0;
    tr.ctx.pos = pos;
    sCtx_369D8* ctx = &tr.ctx;
    sCell_369D8* c0 = &self[0];
    sCell_369D8* c1 = &self[1];
    sCell_369D8* c2 = &self[2];
    sCell_369D8* c3 = &self[3];
    sCell_369D8* c4 = &self[4];
    sCell_369D8* c5 = &self[5];
    sCell_369D8* c6 = &self[6];
    sCell_369D8* c7 = &self[7];
    if (self[0].node) visit_003369D8(c0, &tr.ctx.flags[0], &q, ctx);
    if (self[1].node) visit_003369D8(c1, &tr.ctx.flags[1], &q, ctx);
    if (self[2].node) visit_003369D8(c2, &tr.ctx.flags[2], &q, ctx);
    if (self[3].node) visit_003369D8(c3, &tr.ctx.flags[3], &q, ctx);
    if (self[4].node) visit_003369D8(c4, &tr.ctx.flags[4], &q, ctx);
    if (self[5].node) visit_003369D8(c5, &tr.ctx.flags[5], &q, ctx);
    if (self[6].node) visit_003369D8(c6, &tr.ctx.flags[6], &q, ctx);
    if (self[7].node) visit_003369D8(c7, &tr.ctx.flags[7], &q, ctx);
    if (tr.count == 0) {
        return 0;
    }
    float ref = pos[2];
    int best = 0;
    float bestDiff = absf_003369D8(hits[0].p.z - ref);
    for (int i = 1; i < tr.count; i++) {
        float d = absf_003369D8((hits + i)->p.z - ref);
        if (d < bestDiff) {
            bestDiff = d;
            best = i;
        } else if (d == bestDiff) {
            if (isBetter_003369D8(hits + i, hits + best)) {
                best = i;
            }
        }
    }
    *outP = (hits + best)->p;
    *outN = (hits + best)->n;
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00336D40);
#ifdef SKIP_ASM
struct sVEnt_336D40 { short delta; short index; void* fn; };
struct sBox_336D40;
typedef int (*Test_336D40)(void*);
typedef sBox_336D40* (*GetBox_336D40)(void*);

struct sBox_336D40 {
    float min[4];
    float max[4];
};

struct sQuery_336D40 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sObj_336D40 {
    int pad00[3];
    sVEnt_336D40* vt;
};

struct sNode_336D40 {
    void* next;
    int pad04;
    int flags;
    sObj_336D40* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
    int pad78[4];
    int active;
};

struct sCtx_336D40 {
    sQuery_336D40* q;
    char* buf;
    int cap;
    int* count;
    int full;
};

extern "C" int func_00334888(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_336D40(const sQuery_336D40* q, const sNode_336D40* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_336D40(const sQuery_336D40* q, const sBox_336D40* o)
{
    int r = 0;
    if (q->max[0] > o->min[0] && q->min[0] < o->max[0] &&
        q->max[1] > o->min[1] && q->min[1] < o->max[1] &&
        q->max[2] > o->min[2] && q->min[2] < o->max[2]) {
        r = 1;
    }
    return r;
}

static inline void collect_336D40(sCtx_336D40* ctx, sNode_336D40* node, int mode)
{
    int n = *ctx->count;
    *ctx->count += func_00334888(ctx->q, node, ctx->buf + n * 0x80, ctx->cap - n, mode);
}

extern "C" void func_00336D40(void* vctx, void* vnode)
{
    sCtx_336D40* ctx = (sCtx_336D40*)vctx;
    sNode_336D40* node = (sNode_336D40*)vnode;
    int mode = ctx->full ? 3 : 1;
    int flags = node->flags;
    if (flags & 0x20) {
        if (overlapNode_336D40(ctx->q, node)) {
            collect_336D40(ctx, node, mode);
        }
    } else if (flags & 0x40) {
        sVEnt_336D40* vt = node->obj->vt;
        if (((Test_336D40)vt[44].fn)((char*)node->obj + vt[44].delta)) {
            sVEnt_336D40* vt2 = node->obj->vt;
            sBox_336D40* b = ((GetBox_336D40)vt2[45].fn)((char*)node->obj + vt2[45].delta);
            if (overlapBox_336D40(ctx->q, b)) {
                collect_336D40(ctx, node, mode);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00336F30);
#ifdef SKIP_ASM
struct sQuad_336F30 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sQry_336F30 {
    char pad00[0x20];
    sQuad_336F30 max;   // 0x20
    sQuad_336F30 min;   // 0x30
};

struct sBox_336F30 {
    sQuad_336F30 min;
    sQuad_336F30 max;
};

struct sOct_336F30 {
    void* child[8];
    void* list;         // 0x20
};

struct sCell_336F30 {
    int level;
    int x, y, z;
    sOct_336F30* tree;
};

struct sGrid_336F30 {
    sCell_336F30 cells[8];
};

struct sTag_336F30 {
};

extern "C" void func_003410C0(void* self, void* ctx);
extern "C" void func_0033DFD8(char* flag, sCtx_336D40* ctx, sCtx_336D40* ctx2, void* box, void* tree, void* cell);

static inline void collect_336F30(sOct_336F30* node, sCtx_336D40* ctx)
{
    void** p;
    for (p = (void**)node->list; p != 0; p = (void**)*p) {
        func_00336D40(ctx, p);
    }
    if (node->child[0] != 0) func_003410C0(node->child[0], ctx);
    if (node->child[1] != 0) func_003410C0(node->child[1], ctx);
    if (node->child[2] != 0) func_003410C0(node->child[2], ctx);
    if (node->child[3] != 0) func_003410C0(node->child[3], ctx);
    if (node->child[4] != 0) func_003410C0(node->child[4], ctx);
    if (node->child[5] != 0) func_003410C0(node->child[5], ctx);
    if (node->child[6] != 0) func_003410C0(node->child[6], ctx);
    if (node->child[7] != 0) func_003410C0(node->child[7], ctx);
}

static inline void queryCell_336F30(sGrid_336F30* grid, int i, sBox_336F30* box, sCtx_336D40* ctx)
{
    sCell_336F30* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sOct_336F30* tree = cell->tree;
        sTag_336F30 tag;
        char* flag = (char*)&tag;
        int r = func_00328360(cell, box);
        if (r == 0) {
            collect_336F30(tree, ctx);
        } else if (r == 2) {
            func_0033DFD8(flag, ctx, ctx, box, tree, cell);
        }
    }
}

struct sHit_003378C0;

extern "C" int func_00336F30(void* vself, void* vq, sHit_003378C0* hits, int cap, int full)
{
    sGrid_336F30* grid = (sGrid_336F30*)vself;
    sQry_336F30* q = (sQry_336F30*)vq;
    sCtx_336D40 ctx;
    sBox_336F30 box;
    int count;
    ctx.q = (sQuery_336D40*)q;
    ctx.buf = (char*)hits;
    ctx.cap = cap;
    ctx.count = &count;
    ctx.full = full;
    box.min = q->min;
    box.max = q->max;
    sCtx_336D40* pctx = &ctx;
    count = 0;
    queryCell_336F30(grid, 0, &box, pctx);
    queryCell_336F30(grid, 1, &box, pctx);
    queryCell_336F30(grid, 2, &box, pctx);
    queryCell_336F30(grid, 3, &box, pctx);
    queryCell_336F30(grid, 4, &box, pctx);
    queryCell_336F30(grid, 5, &box, pctx);
    queryCell_336F30(grid, 6, &box, pctx);
    queryCell_336F30(grid, 7, &box, pctx);
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", func_003378C0);
#ifdef SKIP_ASM
struct sHit_003378C0 {
    float pad0[16];
    float dist;             // 0x40
    int pad44[3];
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[10];
    sHit_003378C0() {}
} __attribute__((aligned(16)));

struct sVEnt_003378C0 { short delta; short index; float (*fn)(void*); };
extern "C" int func_00336F30(void* self, void* q, sHit_003378C0* hits, int max, int a4);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_003378C0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_003378C0(sHit_003378C0* a, sHit_003378C0* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

extern "C" float func_003378C0(void* self, char* q, sHit_003378C0* out, int a3)
{
    sHit_003378C0 hits[128];
    int n = func_00336F30(self, q, hits, 0x80, a3);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    sVEnt_003378C0* vt = *(sVEnt_003378C0**)(q + 0x50);
    float ref = vt[3].fn(q + vt[3].delta);
    float bestDiff = absf_003378C0(hits[0].dist - ref);
    for (int i = 1; i < n; i++) {
        float d = absf_003378C0(hits[i].dist - ref);
        if (d < bestDiff) {
            bestDiff = d;
            best = i;
        } else if (d == bestDiff) {
            if (isBetter_003378C0(&hits[i], &hits[best])) {
                best = i;
            }
        }
    }
    *out = hits[best];
    return out->dist;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00337A48);
#ifdef SKIP_ASM
struct sTree_3411B8;
struct sQuery_3411B8;
extern "C" void func_003411B8(sTree_3411B8* tree, sQuery_3411B8* q);

struct sQry_337A48 {
    sWsVec4* pos;               // 0x00
    int a7;                     // 0x04
    int mask;                   // 0x08
    int* cnt;                   // 0x0C
    float* dist;                // 0x10
    sBox_00334680* box;         // 0x14
};

struct sOct_337A48 {
    sOct_337A48* child[8];
    int pad20[2];
    char* objs;                 // 0x28
};

struct sCell_337A48 {
    int level;
    int x, y, z;
    sOct_337A48* tree;
};

struct sGrid_337A48 {
    sCell_337A48 cells[8];
};

struct sTag_337A48 {
};

extern "C" void func_0033ED60(char* flag, sQry_337A48* q, sQry_337A48* q2, sBox_00334680* box, void* tree, void* cell);

static inline void collect_337A48(sOct_337A48* tree, sQry_337A48* q)
{
    for (char* o = tree->objs; o != 0; o = *(char**)o) {
        int type = *(int*)(o + 8);
        if (type == 2) {
            func_0035C698(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 3) {
            func_00348290(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 1) {
            if (*(int*)(*(char**)(o + 0x68) + 0x1C) & q->mask) {
                func_00335128(o, q->box, q->pos, q->cnt, q->dist, q->a7);
            }
        }
    }
    if (tree->child[0] != 0) func_003411B8((sTree_3411B8*)tree->child[0], (sQuery_3411B8*)q);
    if (tree->child[1] != 0) func_003411B8((sTree_3411B8*)tree->child[1], (sQuery_3411B8*)q);
    if (tree->child[2] != 0) func_003411B8((sTree_3411B8*)tree->child[2], (sQuery_3411B8*)q);
    if (tree->child[3] != 0) func_003411B8((sTree_3411B8*)tree->child[3], (sQuery_3411B8*)q);
    if (tree->child[4] != 0) func_003411B8((sTree_3411B8*)tree->child[4], (sQuery_3411B8*)q);
    if (tree->child[5] != 0) func_003411B8((sTree_3411B8*)tree->child[5], (sQuery_3411B8*)q);
    if (tree->child[6] != 0) func_003411B8((sTree_3411B8*)tree->child[6], (sQuery_3411B8*)q);
    if (tree->child[7] != 0) func_003411B8((sTree_3411B8*)tree->child[7], (sQuery_3411B8*)q);
}

static inline void queryCell_337A48(sGrid_337A48* grid, int i, sBox_00334680* box, sQry_337A48* q)
{
    sCell_337A48* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sOct_337A48* tree = cell->tree;
        sTag_337A48 tag;
        char* flag = (char*)&tag;
        int r = func_00328360(cell, box);
        if (r == 0) {
            collect_337A48(tree, q);
        } else if (r == 2) {
            func_0033ED60(flag, q, q, box, tree, cell);
        }
    }
}

extern "C" int func_00337A48(sGrid_337A48* grid, sWsVec4* pos, int a2, int mask, float r)
{
    sBox_00334680 box;
    sWsVec4 ext;
    sQry_337A48 q;
    int cnt;
    float dist;
    box.min = *pos;
    box.max = *pos;
    ext.x = r;
    ext.y = r;
    ext.z = r;
    ext.w = 0.0f;
    wsSubEq_334680(box.min, ext);
    wsAddEq_334680(box.max, ext);
    sBox_00334680* pbox = &box;
    q.pos = pos;
    q.a7 = a2;
    q.mask = mask;
    q.cnt = &cnt;
    q.dist = &dist;
    q.box = pbox;
    dist = 0.0f;
    cnt = 0;
    queryCell_337A48(grid, 0, pbox, &q);
    queryCell_337A48(grid, 1, pbox, &q);
    queryCell_337A48(grid, 2, pbox, &q);
    queryCell_337A48(grid, 3, pbox, &q);
    queryCell_337A48(grid, 4, pbox, &q);
    queryCell_337A48(grid, 5, pbox, &q);
    queryCell_337A48(grid, 6, pbox, &q);
    queryCell_337A48(grid, 7, pbox, &q);
    return cnt;
}
#endif
//100%
INCLUDE_ASM("intersect/worldsphtree", func_003388E0);
#ifdef SKIP_ASM
extern "C" void* func_003388E0(void* self, ...)
{
    return self;
}
#endif
//100%
INCLUDE_ASM("intersect/worldsphtree", func_00338920);
#ifdef SKIP_ASM
struct sCell_338920 {
    int level;
    int x, y, z;
};

extern "C" void func_00338920(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOctNode_330BA8* node = (sOctNode_330BA8*)vnode;
    sCell_338920* cell = (sCell_338920*)vcell;
    func_003309D8(ctx, node);
    if (cell->level == 11) {
        return;
    }
    sCell_338920 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float s = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * s;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * s;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * s;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * s;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * s;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * s;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * s;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * s;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * s;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * s;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * s;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * s;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sSphNode_340A40* c;
    c = node->child[0];
    if (c && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_330BA8((sOctNode_330BA8*)c, ctx2);
        else func_00338920(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = node->child[1];
    if (c && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_330BA8((sOctNode_330BA8*)c, ctx2);
        else func_00338920(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y++;
    c = node->child[3];
    if (c && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_330BA8((sOctNode_330BA8*)c, ctx2);
        else func_00338920(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = node->child[2];
    if (c && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_330BA8((sOctNode_330BA8*)c, ctx2);
        else func_00338920(flag, ctx, ctx2, box, c, &sub);
    }
    sub.x++;
    c = node->child[6];
    if (c && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_330BA8((sOctNode_330BA8*)c, ctx2);
        else func_00338920(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = node->child[7];
    if (c && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_330BA8((sOctNode_330BA8*)c, ctx2);
        else func_00338920(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y--;
    c = node->child[5];
    if (c && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_330BA8((sOctNode_330BA8*)c, ctx2);
        else func_00338920(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = node->child[4];
    if (c && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_330BA8((sOctNode_330BA8*)c, ctx2);
        else func_00338920(flag, ctx, ctx2, box, c, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00339598);
#ifdef SKIP_ASM
struct sCell_339598 {
    int level;
    int x, y, z;
};

extern "C" void func_00339598(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOctNode_331450* node = (sOctNode_331450*)vnode;
    sCell_339598* cell = (sCell_339598*)vcell;
    void** p;
    for (p = node->objs; p != 0; p = (void**)*p) {
        ctx->list[(*ctx->count)++] = (sNode_3309D8*)p;
    }
    if (cell->level == 11) {
        return;
    }
    sCell_339598 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float s = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * s;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * s;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * s;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * s;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * s;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * s;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * s;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * s;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * s;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * s;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * s;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * s;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sOctNode_331450* c;
    c = (sOctNode_331450*)node->child[0];
    if (c && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_331450(c, ctx2);
        else func_00339598(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = (sOctNode_331450*)node->child[1];
    if (c && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_331450(c, ctx2);
        else func_00339598(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y++;
    c = (sOctNode_331450*)node->child[3];
    if (c && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_331450(c, ctx2);
        else func_00339598(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = (sOctNode_331450*)node->child[2];
    if (c && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_331450(c, ctx2);
        else func_00339598(flag, ctx, ctx2, box, c, &sub);
    }
    sub.x++;
    c = (sOctNode_331450*)node->child[6];
    if (c && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_331450(c, ctx2);
        else func_00339598(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = (sOctNode_331450*)node->child[7];
    if (c && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_331450(c, ctx2);
        else func_00339598(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y--;
    c = (sOctNode_331450*)node->child[5];
    if (c && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_331450(c, ctx2);
        else func_00339598(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = (sOctNode_331450*)node->child[4];
    if (c && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_331450(c, ctx2);
        else func_00339598(flag, ctx, ctx2, box, c, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_0033A3D8);
#ifdef SKIP_ASM
struct sTree_340C20;
extern "C" void func_00340C20(sTree_340C20* tree, sCtx_3309D8* ctx);

struct sObj_33A3D8 {
    sObj_33A3D8* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};

struct sTree_33A3D8 {
    sTree_33A3D8* child[8];
    int pad20;
    sObj_33A3D8* objs;          // 0x24
};

static inline int overlapObj_33A3D8(const sBox_3309D8* b, const sObj_33A3D8* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void collectObjs_33A3D8(sTree_33A3D8* tree, sCtx_3309D8* ctx)
{
    func_003309D8(ctx, tree);
    sCtx_3309D8* q = (sCtx_3309D8*)((char*)ctx + 0x10);
    for (sObj_33A3D8* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_33A3D8(q->box, p)) {
            int* cnt = q->count;
            int n = *cnt;
            q->list[n] = (sNode_3309D8*)p;
            *cnt = n + 1;
        }
    }
}

static inline void collect_33A3D8(sTree_33A3D8* tree, sCtx_3309D8* ctx)
{
    collectObjs_33A3D8(tree, ctx);
    if (tree->child[0] != 0) func_00340C20((sTree_340C20*)tree->child[0], ctx);
    if (tree->child[1] != 0) func_00340C20((sTree_340C20*)tree->child[1], ctx);
    if (tree->child[2] != 0) func_00340C20((sTree_340C20*)tree->child[2], ctx);
    if (tree->child[3] != 0) func_00340C20((sTree_340C20*)tree->child[3], ctx);
    if (tree->child[4] != 0) func_00340C20((sTree_340C20*)tree->child[4], ctx);
    if (tree->child[5] != 0) func_00340C20((sTree_340C20*)tree->child[5], ctx);
    if (tree->child[6] != 0) func_00340C20((sTree_340C20*)tree->child[6], ctx);
    if (tree->child[7] != 0) func_00340C20((sTree_340C20*)tree->child[7], ctx);
}

struct sCell_33A3D8 {
    int level;
    int x, y, z;
};

extern "C" void func_0033A3D8(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sTree_33A3D8* node = (sTree_33A3D8*)vnode;
    sCell_33A3D8* cell = (sCell_33A3D8*)vcell;
    collectObjs_33A3D8(node, ctx);
    if (cell->level == 11) {
        return;
    }
    sCell_33A3D8 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float s = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * s;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * s;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * s;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * s;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * s;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * s;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * s;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * s;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * s;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * s;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * s;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * s;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sTree_33A3D8* c;
    c = node->child[0];
    if (c && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33A3D8(c, ctx2);
        else func_0033A3D8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = node->child[1];
    if (c && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33A3D8(c, ctx2);
        else func_0033A3D8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y++;
    c = node->child[3];
    if (c && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33A3D8(c, ctx2);
        else func_0033A3D8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = node->child[2];
    if (c && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33A3D8(c, ctx2);
        else func_0033A3D8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.x++;
    c = node->child[6];
    if (c && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33A3D8(c, ctx2);
        else func_0033A3D8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = node->child[7];
    if (c && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33A3D8(c, ctx2);
        else func_0033A3D8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y--;
    c = node->child[5];
    if (c && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33A3D8(c, ctx2);
        else func_0033A3D8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = node->child[4];
    if (c && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33A3D8(c, ctx2);
        else func_0033A3D8(flag, ctx, ctx2, box, c, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_0033B748);
#ifdef SKIP_ASM
struct sBox_33B748 {
    float min[4];
    float max[4];
};

struct sSub_33B748 {
    void** list;
    int* count;
    int pad08;
    sBox_33B748* box;
};

struct sCtx_33B748 {
    sSub_33B748 nodes;
    sSub_33B748 objs;
    sSub_33B748 all;
};

struct sObjNode_33B748 {
    sObjNode_33B748* next;
    int pad04[85];
    float bmin[3];
    float bmax[3];
};

struct sTree_33B748 {
    sTree_33B748* child[8];
    void* nodes;
    sObjNode_33B748* objs;
    void** all;
};

struct sCell_33B748 {
    int level;
    int x, y, z;
};

// PORT: alias; the unit defines func_00340DC0 later with its own struct types.
extern "C" void func_00340DC0_33B748(sTree_33B748* self, sCtx_33B748* ctx) __asm__("func_00340DC0");

static inline void add_33B748(sSub_33B748* s, void* p)
{
    int* cnt = s->count;
    int n = *cnt;
    s->list[n] = p;
    *cnt = n + 1;
}

static inline int overlap_33B748(const sBox_33B748* b, const sObjNode_33B748* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void visit_33B748(sTree_33B748* self, sCtx_33B748* ctx)
{
    func_003309D8((sCtx_3309D8*)ctx, self);
    sSub_33B748* s = &ctx->objs;
    for (sObjNode_33B748* p = self->objs; p != 0; p = p->next) {
        if (overlap_33B748(s->box, p)) {
            add_33B748(s, p);
        }
    }
    s = &ctx->all;
    for (void** q = self->all; q != 0; q = (void**)*q) {
        add_33B748(s, q);
    }
}

static inline void collect_33B748(sTree_33B748* self, sCtx_33B748* ctx)
{
    visit_33B748(self, ctx);
    if (self->child[0]) func_00340DC0_33B748(self->child[0], ctx);
    if (self->child[1]) func_00340DC0_33B748(self->child[1], ctx);
    if (self->child[2]) func_00340DC0_33B748(self->child[2], ctx);
    if (self->child[3]) func_00340DC0_33B748(self->child[3], ctx);
    if (self->child[4]) func_00340DC0_33B748(self->child[4], ctx);
    if (self->child[5]) func_00340DC0_33B748(self->child[5], ctx);
    if (self->child[6]) func_00340DC0_33B748(self->child[6], ctx);
    if (self->child[7]) func_00340DC0_33B748(self->child[7], ctx);
}

struct sCtx_340DC0;

// Declared as func_00332DB8 (its caller) declares it; sCtx_340DC0 has sCtx_33B748's layout.
extern "C" void func_0033B748(char* flag, sCtx_340DC0* vctx, sCtx_340DC0* vctx2, void* vbox, void* vnode, void* vcell)
{
    sCtx_33B748* ctx = (sCtx_33B748*)vctx;
    sCtx_33B748* ctx2 = (sCtx_33B748*)vctx2;
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sTree_33B748* node = (sTree_33B748*)vnode;
    sCell_33B748* cell = (sCell_33B748*)vcell;
    visit_33B748(node, ctx);
    if (cell->level == 11) {
        return;
    }
    sCell_33B748 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float s = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * s;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * s;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * s;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * s;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * s;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * s;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * s;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * s;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * s;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * s;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * s;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * s;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sTree_33B748* c;
    c = node->child[0];
    if (c && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33B748(c, ctx2);
        else func_0033B748(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.z++;
    c = node->child[1];
    if (c && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33B748(c, ctx2);
        else func_0033B748(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.y++;
    c = node->child[3];
    if (c && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33B748(c, ctx2);
        else func_0033B748(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.z--;
    c = node->child[2];
    if (c && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33B748(c, ctx2);
        else func_0033B748(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.x++;
    c = node->child[6];
    if (c && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33B748(c, ctx2);
        else func_0033B748(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.z++;
    c = node->child[7];
    if (c && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33B748(c, ctx2);
        else func_0033B748(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.y--;
    c = node->child[5];
    if (c && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33B748(c, ctx2);
        else func_0033B748(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.z--;
    c = node->child[4];
    if (c && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33B748(c, ctx2);
        else func_0033B748(flag, vctx, vctx2, vbox, c, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_0033CCF8);
#ifdef SKIP_ASM
struct sCell_33CCF8 {
    int level;
    int x, y, z;
};

extern "C" void func_0033CCF8(char* flag, sCtx_335960* ctx, sCtx_335960* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOct_335D78* node = (sOct_335D78*)vnode;
    sCell_33CCF8* cell = (sCell_33CCF8*)vcell;
    void** p;
    for (p = (void**)node->listA; p != 0; p = (void**)*p) {
        func_00335960(ctx, p);
    }
    for (p = (void**)node->listB; p != 0; p = (void**)*p) {
        func_00335B90(ctx, p);
    }
    if (cell->level == 11) {
        return;
    }
    sCell_33CCF8 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float s = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * s;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * s;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * s;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * s;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * s;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * s;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * s;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * s;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * s;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * s;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * s;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * s;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sOct_335D78* c;
    c = (sOct_335D78*)node->child[0];
    if (c && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_335D78(c, ctx2);
        else func_0033CCF8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = (sOct_335D78*)node->child[1];
    if (c && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_335D78(c, ctx2);
        else func_0033CCF8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y++;
    c = (sOct_335D78*)node->child[3];
    if (c && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_335D78(c, ctx2);
        else func_0033CCF8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = (sOct_335D78*)node->child[2];
    if (c && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_335D78(c, ctx2);
        else func_0033CCF8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.x++;
    c = (sOct_335D78*)node->child[6];
    if (c && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_335D78(c, ctx2);
        else func_0033CCF8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = (sOct_335D78*)node->child[7];
    if (c && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_335D78(c, ctx2);
        else func_0033CCF8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y--;
    c = (sOct_335D78*)node->child[5];
    if (c && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_335D78(c, ctx2);
        else func_0033CCF8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = (sOct_335D78*)node->child[4];
    if (c && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_335D78(c, ctx2);
        else func_0033CCF8(flag, ctx, ctx2, box, c, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_0033DBE8);
#ifdef SKIP_ASM
struct sQuery_369D8;
struct sCtx_369D8;
struct sCell_369D8;
extern "C" void func_00335960(void* ctx, void* obj);
extern "C" void func_00335B90(void* ctx, void* obj);
extern "C" void func_00340FA0(void* self, void* ctx);

struct sOct_DBE8 {
    sOct_DBE8* child[8];            // 0x00
    void** listB;                   // 0x20
    void** listA;                   // 0x24
};

struct sCell_DBE8 {
    int level;                      // 0x0
    int x;                          // 0x4
    int y;                          // 0x8
    int z;                          // 0xC
};

struct sCtx_DBE8 {
    float* pos;                     // 0x0
};

static inline int Classify_DBE8(sCtx_DBE8* ctx, sCell_DBE8* c)
{
    int bits = (c->level + 0x7F) << 23;
    float s = *(float*)&bits;
    float* p = ctx->pos;
    if (p[0] < ((float)c->x - 0.2f) * s) {
        return 1;
    }
    if (((float)(c->x + 1) + 0.2f) * s < p[0]) {
        return 1;
    }
    if (p[1] < ((float)c->y - 0.2f) * s) {
        return 1;
    }
    if (((float)(c->y + 1) + 0.2f) * s < p[1]) {
        return 1;
    }
    return 2;
}

extern "C" void func_0033DBE8(char* flag, sQuery_369D8* q, sQuery_369D8* q2, sCtx_369D8* vctx, void* vnode, sCell_369D8* vcell)
{
    sCtx_DBE8* ctx = (sCtx_DBE8*)vctx;
    sOct_DBE8* node = (sOct_DBE8*)vnode;
    sCell_DBE8* cell = (sCell_DBE8*)vcell;
    int r = Classify_DBE8(ctx, cell);
    if (r == 0) {
        void** o;
        for (o = node->listA; o != 0; o = (void**)*o) {
            func_00335960(q2, o);
        }
        for (o = node->listB; o != 0; o = (void**)*o) {
            func_00335B90(q2, o);
        }
        if (node->child[0]) {
            func_00340FA0(node->child[0], q2);
        }
        if (node->child[1]) {
            func_00340FA0(node->child[1], q2);
        }
        if (node->child[2]) {
            func_00340FA0(node->child[2], q2);
        }
        if (node->child[3]) {
            func_00340FA0(node->child[3], q2);
        }
        if (node->child[4]) {
            func_00340FA0(node->child[4], q2);
        }
        if (node->child[5]) {
            func_00340FA0(node->child[5], q2);
        }
        if (node->child[6]) {
            func_00340FA0(node->child[6], q2);
        }
        if (node->child[7]) {
            func_00340FA0(node->child[7], q2);
        }
    } else if (r == 2) {
        void** o;
        for (o = node->listA; o != 0; o = (void**)*o) {
            func_00335960(q, o);
        }
        for (o = node->listB; o != 0; o = (void**)*o) {
            func_00335B90(q, o);
        }
        if (cell->level == 11) {
            return;
        }
        sCell_DBE8 sub;
        sub.level = cell->level - 1;
        sub.x = cell->x * 2;
        sub.y = cell->y * 2;
        sub.z = cell->z * 2;
        if (node->child[0]) {
            func_0033DBE8(flag, q, q2, vctx, node->child[0], (sCell_369D8*)&sub);
        }
        sub.x++;
        if (node->child[4]) {
            func_0033DBE8(flag, q, q2, vctx, node->child[4], (sCell_369D8*)&sub);
        }
        sub.y++;
        if (node->child[6]) {
            func_0033DBE8(flag, q, q2, vctx, node->child[6], (sCell_369D8*)&sub);
        }
        sub.x--;
        if (node->child[2]) {
            func_0033DBE8(flag, q, q2, vctx, node->child[2], (sCell_369D8*)&sub);
        }
        sub.z++;
        if (node->child[3]) {
            func_0033DBE8(flag, q, q2, vctx, node->child[3], (sCell_369D8*)&sub);
        }
        sub.x++;
        if (node->child[7]) {
            func_0033DBE8(flag, q, q2, vctx, node->child[7], (sCell_369D8*)&sub);
        }
        sub.y--;
        if (node->child[5]) {
            func_0033DBE8(flag, q, q2, vctx, node->child[5], (sCell_369D8*)&sub);
        }
        sub.x--;
        if (node->child[1]) {
            func_0033DBE8(flag, q, q2, vctx, node->child[1], (sCell_369D8*)&sub);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_0033DFD8);
#ifdef SKIP_ASM
struct sCell_33DFD8 {
    int level;
    int x, y, z;
};

struct sOct_33DFD8 {
    sOct_33DFD8* child[8];
    void* list;         // 0x20
};

extern "C" void func_003410C0(void* self, void* ctx);

static inline void collect_33DFD8(sOct_33DFD8* node, sCtx_336D40* ctx)
{
    void** p;
    for (p = (void**)node->list; p != 0; p = (void**)*p) {
        func_00336D40(ctx, p);
    }
    if (node->child[0] != 0) func_003410C0(node->child[0], ctx);
    if (node->child[1] != 0) func_003410C0(node->child[1], ctx);
    if (node->child[2] != 0) func_003410C0(node->child[2], ctx);
    if (node->child[3] != 0) func_003410C0(node->child[3], ctx);
    if (node->child[4] != 0) func_003410C0(node->child[4], ctx);
    if (node->child[5] != 0) func_003410C0(node->child[5], ctx);
    if (node->child[6] != 0) func_003410C0(node->child[6], ctx);
    if (node->child[7] != 0) func_003410C0(node->child[7], ctx);
}

extern "C" void func_0033DFD8(char* flag, sCtx_336D40* ctx, sCtx_336D40* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOct_33DFD8* node = (sOct_33DFD8*)vnode;
    sCell_33DFD8* cell = (sCell_33DFD8*)vcell;
    void** p;
    for (p = (void**)node->list; p != 0; p = (void**)*p) {
        func_00336D40(ctx, p);
    }
    if (cell->level == 11) {
        return;
    }
    sCell_33DFD8 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float s = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * s;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * s;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * s;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * s;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * s;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * s;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * s;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * s;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * s;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * s;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * s;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * s;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sOct_33DFD8* c;
    c = node->child[0];
    if (c && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33DFD8(c, ctx2);
        else func_0033DFD8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = node->child[1];
    if (c && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33DFD8(c, ctx2);
        else func_0033DFD8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y++;
    c = node->child[3];
    if (c && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33DFD8(c, ctx2);
        else func_0033DFD8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = node->child[2];
    if (c && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33DFD8(c, ctx2);
        else func_0033DFD8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.x++;
    c = node->child[6];
    if (c && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33DFD8(c, ctx2);
        else func_0033DFD8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z++;
    c = node->child[7];
    if (c && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33DFD8(c, ctx2);
        else func_0033DFD8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.y--;
    c = node->child[5];
    if (c && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33DFD8(c, ctx2);
        else func_0033DFD8(flag, ctx, ctx2, box, c, &sub);
    }
    sub.z--;
    c = node->child[4];
    if (c && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33DFD8(c, ctx2);
        else func_0033DFD8(flag, ctx, ctx2, box, c, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_0033ED60);
#ifdef SKIP_ASM
struct sQuery_33ED60 {
    sWsVec4* pos;               // 0x00
    int a7;                     // 0x04
    int mask;                   // 0x08
    int* cnt;                   // 0x0C
    float* dist;                // 0x10
    sBox_00334680* box;         // 0x14
};

struct sCell_33ED60 {
    int level;
    int x, y, z;
};

struct sOct_33ED60 {
    sOct_33ED60* child[8];
    int pad20[2];
    char* objs;                 // 0x28
};

// PORT: alias; the unit defines func_003411B8 later with its own struct types.
extern "C" void func_003411B8_33ED60(sOct_33ED60* tree, sQuery_33ED60* q) __asm__("func_003411B8");

static inline void objs_33ED60(char* o, sQuery_33ED60* q)
{
    for (; o != 0; o = *(char**)o) {
        int type = *(int*)(o + 8);
        if (type == 2) {
            func_0035C698(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 3) {
            func_00348290(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 1) {
            if (*(int*)(*(char**)(o + 0x68) + 0x1C) & q->mask) {
                func_00335128(o, q->box, q->pos, q->cnt, q->dist, q->a7);
            }
        }
    }
}

static inline void collect_33ED60(sOct_33ED60* node, sQuery_33ED60* q)
{
    objs_33ED60(node->objs, q);
    if (node->child[0] != 0) func_003411B8_33ED60(node->child[0], q);
    if (node->child[1] != 0) func_003411B8_33ED60(node->child[1], q);
    if (node->child[2] != 0) func_003411B8_33ED60(node->child[2], q);
    if (node->child[3] != 0) func_003411B8_33ED60(node->child[3], q);
    if (node->child[4] != 0) func_003411B8_33ED60(node->child[4], q);
    if (node->child[5] != 0) func_003411B8_33ED60(node->child[5], q);
    if (node->child[6] != 0) func_003411B8_33ED60(node->child[6], q);
    if (node->child[7] != 0) func_003411B8_33ED60(node->child[7], q);
}

struct sQry_337A48;

// Declared as func_00337A48 (its caller) declares it; sQry_337A48 has sQuery_33ED60's layout.
extern "C" void func_0033ED60(char* flag, sQry_337A48* vctx, sQry_337A48* vctx2, sBox_00334680* vbox, void* vnode, void* vcell)
{
    sQuery_33ED60* ctx = (sQuery_33ED60*)vctx;
    sQuery_33ED60* ctx2 = (sQuery_33ED60*)vctx2;
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOct_33ED60* node = (sOct_33ED60*)vnode;
    sCell_33ED60* cell = (sCell_33ED60*)vcell;
    objs_33ED60(node->objs, ctx);
    if (cell->level == 11) {
        return;
    }
    sCell_33ED60 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float s = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * s;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * s;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * s;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * s;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * s;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * s;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * s;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * s;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * s;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * s;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * s;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * s;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sOct_33ED60* c;
    c = node->child[0];
    if (c && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33ED60(c, ctx2);
        else func_0033ED60(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.z++;
    c = node->child[1];
    if (c && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33ED60(c, ctx2);
        else func_0033ED60(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.y++;
    c = node->child[3];
    if (c && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33ED60(c, ctx2);
        else func_0033ED60(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.z--;
    c = node->child[2];
    if (c && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33ED60(c, ctx2);
        else func_0033ED60(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.x++;
    c = node->child[6];
    if (c && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33ED60(c, ctx2);
        else func_0033ED60(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.z++;
    c = node->child[7];
    if (c && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33ED60(c, ctx2);
        else func_0033ED60(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.y--;
    c = node->child[5];
    if (c && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33ED60(c, ctx2);
        else func_0033ED60(flag, vctx, vctx2, vbox, c, &sub);
    }
    sub.z--;
    c = node->child[4];
    if (c && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33ED60(c, ctx2);
        else func_0033ED60(flag, vctx, vctx2, vbox, c, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003400D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// A 16-byte vector global constructed from four floats.
struct sV4_3400D8 {
    float x, y, z, w;
    sV4_3400D8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    // Placement form without throw(): g++ then adds no null check (the original was a global's ctor call).
    void* operator new(unsigned int, void* p) { return p; }
};

extern sV4_3400D8 D_004FF2C0_v __asm__("D_004FF2C0");
extern sV4_3400D8 D_004FF2D0_v __asm__("D_004FF2D0");
extern sV4_3400D8 D_004FF2E0_v __asm__("D_004FF2E0");
extern sV4_3400D8 D_004FF2F0_v __asm__("D_004FF2F0");
extern sV4_3400D8 D_004FF300_v __asm__("D_004FF300");
extern sV4_3400D8 D_004FF310_v __asm__("D_004FF310");
extern sV4_3400D8 D_004FF320_v __asm__("D_004FF320");
extern sV4_3400D8 D_004FF330_v __asm__("D_004FF330");
extern sV4_3400D8 D_004FF340_v __asm__("D_004FF340");
extern sV4_3400D8 D_004FF350_v __asm__("D_004FF350");
extern sV4_3400D8 D_004FF360_v __asm__("D_004FF360");
extern sV4_3400D8 D_004FF370_v __asm__("D_004FF370");
extern sV4_3400D8 D_004FF380_v __asm__("D_004FF380");
extern sV4_3400D8 D_004FF390_v __asm__("D_004FF390");
extern sV4_3400D8 D_004FF3A0_v __asm__("D_004FF3A0");
extern sV4_3400D8 D_004FF3B0_v __asm__("D_004FF3B0");
extern sV4_3400D8 D_004FF3C0_v __asm__("D_004FF3C0");
extern sV4_3400D8 D_004FF3D0_v __asm__("D_004FF3D0");
extern sV4_3400D8 D_004FF3E0_v __asm__("D_004FF3E0");
extern sV4_3400D8 D_004FF3F0_v __asm__("D_004FF3F0");
extern sV4_3400D8 D_004FF400_v __asm__("D_004FF400");
extern sV4_3400D8 D_004FF410_v __asm__("D_004FF410");
extern sV4_3400D8 D_004FF420_v __asm__("D_004FF420");
extern sV4_3400D8 D_004FF430_v __asm__("D_004FF430");
extern sV4_3400D8 D_004FF440_v __asm__("D_004FF440");
extern sV4_3400D8 D_004FF450_v __asm__("D_004FF450");
extern sV4_3400D8 D_004FF460_v __asm__("D_004FF460");
extern sV4_3400D8 D_004FF470_v __asm__("D_004FF470");
extern sV4_3400D8 D_004FF480_v __asm__("D_004FF480");
extern sV4_3400D8 D_004FF490_v __asm__("D_004FF490");
extern sV4_3400D8 D_004FF4A0_v __asm__("D_004FF4A0");
extern sV4_3400D8 D_004FF4B0_v __asm__("D_004FF4B0");
extern sV4_3400D8 D_004FF4C0_v __asm__("D_004FF4C0");
extern sV4_3400D8 D_004FF4D0_v __asm__("D_004FF4D0");
extern sV4_3400D8 D_004FF4E0_v __asm__("D_004FF4E0");
extern sV4_3400D8 D_004FF4F0_v __asm__("D_004FF4F0");
extern sV4_3400D8 D_004FF500_v __asm__("D_004FF500");
extern sV4_3400D8 D_004FF510_v __asm__("D_004FF510");
extern sV4_3400D8 D_004FF520_v __asm__("D_004FF520");
extern sV4_3400D8 D_004FF530_v __asm__("D_004FF530");
extern sV4_3400D8 D_004FF540_v __asm__("D_004FF540");
extern sV4_3400D8 D_004FF550_v __asm__("D_004FF550");
extern sV4_3400D8 D_004FF560_v __asm__("D_004FF560");
extern sV4_3400D8 D_004FF570_v __asm__("D_004FF570");
extern sV4_3400D8 D_004FF580_v __asm__("D_004FF580");
extern sV4_3400D8 D_004FF590_v __asm__("D_004FF590");
extern sV4_3400D8 D_004FF5A0_v __asm__("D_004FF5A0");
extern sV4_3400D8 D_004FF5B0_v __asm__("D_004FF5B0");
extern sV4_3400D8 D_004FF5C0_v __asm__("D_004FF5C0");
extern sV4_3400D8 D_004FF5D0_v __asm__("D_004FF5D0");
extern sV4_3400D8 D_004FF5E0_v __asm__("D_004FF5E0");
extern sV4_3400D8 D_004FF5F0_v __asm__("D_004FF5F0");
extern sV4_3400D8 D_004FF600_v __asm__("D_004FF600");
extern sV4_3400D8 D_004FF610_v __asm__("D_004FF610");
extern sV4_3400D8 D_004FF620_v __asm__("D_004FF620");
extern sV4_3400D8 D_004FF630_v __asm__("D_004FF630");
// D_004FF640: the eight corners (+-1, +-1, +-1, 0), written as one 32-float block.
struct sCorners_3400D8 {
    float v[32];
    void init()
    {
        v[0] = -1.0f;
        v[1] = -1.0f;
        v[2] = -1.0f;
        v[3] = 0.0f;
        v[4] = -1.0f;
        v[5] = -1.0f;
        v[6] = 1.0f;
        v[7] = 0.0f;
        v[8] = -1.0f;
        v[9] = 1.0f;
        v[10] = -1.0f;
        v[11] = 0.0f;
        v[12] = -1.0f;
        v[13] = 1.0f;
        v[14] = 1.0f;
        v[15] = 0.0f;
        v[16] = 1.0f;
        v[17] = -1.0f;
        v[18] = -1.0f;
        v[19] = 0.0f;
        v[20] = 1.0f;
        v[21] = -1.0f;
        v[22] = 1.0f;
        v[23] = 0.0f;
        v[24] = 1.0f;
        v[25] = 1.0f;
        v[26] = -1.0f;
        v[27] = 0.0f;
        v[28] = 1.0f;
        v[29] = 1.0f;
        v[30] = 1.0f;
        v[31] = 0.0f;
    }
};
extern sCorners_3400D8 D_004FF640_c __asm__("D_004FF640");


// Static initialisation of the unit's vector globals D_004FF2C0..D_004FF630 and the corner table D_004FF640
// (g++ __static_initialization_and_destruction_0).
// The unit declares func_003400D8 returning void*; bind this definition by asm label.
extern "C" void func_003400D8_init(int initialize, int priority) __asm__("func_003400D8");
extern "C" void func_003400D8_init(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004FF2C0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004FF2D0_v) sV4_3400D8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF2E0_v) sV4_3400D8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF2F0_v) sV4_3400D8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004FF300_v) sV4_3400D8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004FF310_v) sV4_3400D8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF320_v) sV4_3400D8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004FF330_v) sV4_3400D8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004FF340_v) sV4_3400D8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004FF350_v) sV4_3400D8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004FF360_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF370_v) sV4_3400D8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF380_v) sV4_3400D8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004FF390_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF3A0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF3B0_v) sV4_3400D8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF3C0_v) sV4_3400D8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF3D0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF3E0_v) sV4_3400D8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF3F0_v) sV4_3400D8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF400_v) sV4_3400D8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF410_v) sV4_3400D8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004FF420_v) sV4_3400D8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004FF430_v) sV4_3400D8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004FF440_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF450_v) sV4_3400D8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004FF460_v) sV4_3400D8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004FF470_v) sV4_3400D8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004FF480_v) sV4_3400D8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004FF490_v) sV4_3400D8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004FF4A0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF4B0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF4C0_v) sV4_3400D8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004FF4D0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF4E0_v) sV4_3400D8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004FF4F0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF500_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF510_v) sV4_3400D8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF520_v) sV4_3400D8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004FF530_v) sV4_3400D8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004FF540_v) sV4_3400D8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004FF550_v) sV4_3400D8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004FF560_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF570_v) sV4_3400D8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004FF580_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF590_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF5A0_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF5B0_v) sV4_3400D8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004FF5C0_v) sV4_3400D8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004FF5D0_v) sV4_3400D8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004FF5E0_v) sV4_3400D8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004FF5F0_v) sV4_3400D8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004FF600_v) sV4_3400D8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004FF610_v) sV4_3400D8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004FF620_v) sV4_3400D8(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004FF630_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        D_004FF640_c.init();
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340970);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// The July 2003 prototype names this function tAIFwdDiff::tAIFwdDiff() (an out-of-line copy of the inline ctor).
struct tAIFwdDiffElem {
    float v;
    tAIFwdDiffElem() {}
};
class tAIFwdDiff {
public:
    tAIFwdDiffElem a[10][10];
    tAIFwdDiffElem b[9][9];
    tAIFwdDiffElem c[3][3];
    tAIFwdDiff();
};

tAIFwdDiff::tAIFwdDiff()
{
}
// PORT: the compiler names this ctor __10tAIFwdDiff; expose it under the address-derived name.
__asm__(".globl func_00340970\n.type func_00340970,@function\nfunc_00340970 = __10tAIFwdDiff\n.size func_00340970,0x98\n");
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340A08__FPv);
#ifdef SKIP_ASM
void func_00340A08(void* self)
{
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldLine_GetIdealParam__FPv);
#ifdef SKIP_ASM
float cWorldLine_GetIdealParam(void* self)
{
    return *(float*)((char*)self + 0xA0);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphere_GetIdealParam);
#ifdef SKIP_ASM
extern "C" float cWorldSphere_GetIdealParam(void* self)
{
    if (*(int*)self == 0) {
        return -1.0f;
    }
    return *(float*)((char*)self + 0x70) * 2.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340A40);
#ifdef SKIP_ASM
struct sSphNode_340A40 {
    sSphNode_340A40* child[8];
};

extern "C" void func_00340A40(sSphNode_340A40* node, sCtx_3309D8* ctx)
{
    func_003309D8(ctx, node);
    if (node->child[0] != 0) func_00340A40(node->child[0], ctx);
    if (node->child[1] != 0) func_00340A40(node->child[1], ctx);
    if (node->child[2] != 0) func_00340A40(node->child[2], ctx);
    if (node->child[3] != 0) func_00340A40(node->child[3], ctx);
    if (node->child[4] != 0) func_00340A40(node->child[4], ctx);
    if (node->child[5] != 0) func_00340A40(node->child[5], ctx);
    if (node->child[6] != 0) func_00340A40(node->child[6], ctx);
    if (node->child[7] != 0) func_00340A40(node->child[7], ctx);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340B18);
#ifdef SKIP_ASM
struct sOctCollect {
    void** arr;
    int* count;
};

extern "C" void func_00340B18(void* self, sOctCollect* out)
{
    void** p;
    for (p = *(void***)((char*)self + 0x28); p != 0; p = (void**)*p) {
        out->arr[(*out->count)++] = p;
    }
    if (*(void**)((char*)self + 0x0)) func_00340B18(*(void**)((char*)self + 0x0), out);
    if (*(void**)((char*)self + 0x4)) func_00340B18(*(void**)((char*)self + 0x4), out);
    if (*(void**)((char*)self + 0x8)) func_00340B18(*(void**)((char*)self + 0x8), out);
    if (*(void**)((char*)self + 0xC)) func_00340B18(*(void**)((char*)self + 0xC), out);
    if (*(void**)((char*)self + 0x10)) func_00340B18(*(void**)((char*)self + 0x10), out);
    if (*(void**)((char*)self + 0x14)) func_00340B18(*(void**)((char*)self + 0x14), out);
    if (*(void**)((char*)self + 0x18)) func_00340B18(*(void**)((char*)self + 0x18), out);
    if (*(void**)((char*)self + 0x1C)) func_00340B18(*(void**)((char*)self + 0x1C), out);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340C20);
#ifdef SKIP_ASM
struct sObj_340C20 {
    sObj_340C20* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};

struct sTree_340C20 {
    sTree_340C20* child[8];
    int pad20;
    sObj_340C20* objs;          // 0x24
};

static inline int overlapObj_340C20(const sBox_3309D8* b, const sObj_340C20* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

extern "C" void func_00340C20(sTree_340C20* tree, sCtx_3309D8* ctx)
{
    func_003309D8(ctx, tree);
    sCtx_3309D8* q = (sCtx_3309D8*)((char*)ctx + 0x10);
    for (sObj_340C20* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_340C20(q->box, p)) {
            int* cnt = q->count;
            int n = *cnt;
            q->list[n] = (sNode_3309D8*)p;
            *cnt = n + 1;
        }
    }
    if (tree->child[0] != 0) func_00340C20(tree->child[0], ctx);
    if (tree->child[1] != 0) func_00340C20(tree->child[1], ctx);
    if (tree->child[2] != 0) func_00340C20(tree->child[2], ctx);
    if (tree->child[3] != 0) func_00340C20(tree->child[3], ctx);
    if (tree->child[4] != 0) func_00340C20(tree->child[4], ctx);
    if (tree->child[5] != 0) func_00340C20(tree->child[5], ctx);
    if (tree->child[6] != 0) func_00340C20(tree->child[6], ctx);
    if (tree->child[7] != 0) func_00340C20(tree->child[7], ctx);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340DC0);
#ifdef SKIP_ASM
struct sCtx_3309D8;
extern "C" void func_003309D8(sCtx_3309D8* ctx, void* tree);

struct sBox_340DC0 {
    float min[4];
    float max[4];
};

struct sSub_340DC0 {
    void** list;
    int* count;
    int pad08;
    sBox_340DC0* box;
};

struct sCtx_340DC0 {
    sSub_340DC0 nodes;
    sSub_340DC0 objs;
    sSub_340DC0 all;
};

struct sObjNode_340DC0 {
    sObjNode_340DC0* next;
    int pad04[85];
    float bmin[3];
    float bmax[3];
};

struct sTree_340DC0 {
    sTree_340DC0* child[8];
    void* nodes;
    sObjNode_340DC0* objs;
    void** all;
};

static inline void add_340DC0(sSub_340DC0* s, void* p)
{
    int* cnt = s->count;
    int n = *cnt;
    s->list[n] = p;
    *cnt = n + 1;
}

static inline int overlap_340DC0(const sBox_340DC0* b, const sObjNode_340DC0* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

extern "C" void func_00340DC0(sTree_340DC0* self, sCtx_340DC0* ctx)
{
    func_003309D8((sCtx_3309D8*)ctx, self);
    sSub_340DC0* s = &ctx->objs;
    for (sObjNode_340DC0* p = self->objs; p != 0; p = p->next) {
        if (overlap_340DC0(s->box, p)) {
            add_340DC0(s, p);
        }
    }
    s = &ctx->all;
    for (void** q = self->all; q != 0; q = (void**)*q) {
        add_340DC0(s, q);
    }
    if (self->child[0]) func_00340DC0(self->child[0], ctx);
    if (self->child[1]) func_00340DC0(self->child[1], ctx);
    if (self->child[2]) func_00340DC0(self->child[2], ctx);
    if (self->child[3]) func_00340DC0(self->child[3], ctx);
    if (self->child[4]) func_00340DC0(self->child[4], ctx);
    if (self->child[5]) func_00340DC0(self->child[5], ctx);
    if (self->child[6]) func_00340DC0(self->child[6], ctx);
    if (self->child[7]) func_00340DC0(self->child[7], ctx);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", func_00340FA0);
#ifdef SKIP_ASM
extern "C" void func_00335960(void* ctx, void* obj);
extern "C" void func_00335B90(void* ctx, void* obj);

extern "C" void func_00340FA0(void* self, void* ctx)
{
    void** p;
    for (p = *(void***)((char*)self + 0x24); p != 0; p = (void**)*p) {
        func_00335960(ctx, p);
    }
    for (p = *(void***)((char*)self + 0x20); p != 0; p = (void**)*p) {
        func_00335B90(ctx, p);
    }
    if (*(void**)((char*)self + 0x0)) func_00340FA0(*(void**)((char*)self + 0x0), ctx);
    if (*(void**)((char*)self + 0x4)) func_00340FA0(*(void**)((char*)self + 0x4), ctx);
    if (*(void**)((char*)self + 0x8)) func_00340FA0(*(void**)((char*)self + 0x8), ctx);
    if (*(void**)((char*)self + 0xC)) func_00340FA0(*(void**)((char*)self + 0xC), ctx);
    if (*(void**)((char*)self + 0x10)) func_00340FA0(*(void**)((char*)self + 0x10), ctx);
    if (*(void**)((char*)self + 0x14)) func_00340FA0(*(void**)((char*)self + 0x14), ctx);
    if (*(void**)((char*)self + 0x18)) func_00340FA0(*(void**)((char*)self + 0x18), ctx);
    if (*(void**)((char*)self + 0x1C)) func_00340FA0(*(void**)((char*)self + 0x1C), ctx);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", func_003410C0);
#ifdef SKIP_ASM
extern "C" void func_00336D40(void* ctx, void* obj);

extern "C" void func_003410C0(void* self, void* ctx)
{
    void** p;
    for (p = *(void***)((char*)self + 0x20); p != 0; p = (void**)*p) {
        func_00336D40(ctx, p);
    }
    if (*(void**)((char*)self + 0x0)) func_003410C0(*(void**)((char*)self + 0x0), ctx);
    if (*(void**)((char*)self + 0x4)) func_003410C0(*(void**)((char*)self + 0x4), ctx);
    if (*(void**)((char*)self + 0x8)) func_003410C0(*(void**)((char*)self + 0x8), ctx);
    if (*(void**)((char*)self + 0xC)) func_003410C0(*(void**)((char*)self + 0xC), ctx);
    if (*(void**)((char*)self + 0x10)) func_003410C0(*(void**)((char*)self + 0x10), ctx);
    if (*(void**)((char*)self + 0x14)) func_003410C0(*(void**)((char*)self + 0x14), ctx);
    if (*(void**)((char*)self + 0x18)) func_003410C0(*(void**)((char*)self + 0x18), ctx);
    if (*(void**)((char*)self + 0x1C)) func_003410C0(*(void**)((char*)self + 0x1C), ctx);
}
#endif

extern "C" void* func_003400D8(int, int);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003411B8);
#ifdef SKIP_ASM
struct sQuery_3411B8 {
    sWsVec4* pos;               // 0x00
    int a7;                     // 0x04
    int mask;                   // 0x08
    int* cnt;                   // 0x0C
    float* dist;                // 0x10
    sBox_00334680* box;         // 0x14
};

struct sTree_3411B8 {
    sTree_3411B8* child[8];
    int pad20[2];
    char* objs;                 // 0x28
};

extern "C" void func_003411B8(sTree_3411B8* tree, sQuery_3411B8* q)
{
    for (char* o = tree->objs; o != 0; o = *(char**)o) {
        int type = *(int*)(o + 8);
        if (type == 2) {
            func_0035C698(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 3) {
            func_00348290(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 1) {
            if (*(int*)(*(char**)(o + 0x68) + 0x1C) & q->mask) {
                func_00335128(o, q->box, q->pos, q->cnt, q->dist, q->a7);
            }
        }
    }
    if (tree->child[0] != 0) func_003411B8(tree->child[0], q);
    if (tree->child[1] != 0) func_003411B8(tree->child[1], q);
    if (tree->child[2] != 0) func_003411B8(tree->child[2], q);
    if (tree->child[3] != 0) func_003411B8(tree->child[3], q);
    if (tree->child[4] != 0) func_003411B8(tree->child[4], q);
    if (tree->child[5] != 0) func_003411B8(tree->child[5], q);
    if (tree->child[6] != 0) func_003411B8(tree->child[6], q);
    if (tree->child[7] != 0) func_003411B8(tree->child[7], q);
}
#endif

//99.38%
INCLUDE_ASM("intersect/worldsphtree", func_00341368__FPv);
#ifdef SKIP_ASM
void* func_00341368(void* self)
{
    return func_003400D8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341388);
#ifdef SKIP_ASM
struct sVec4_341388 {
    float x, y, z, w;
    sVec4_341388() {}
    sVec4_341388(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sDef_341388 {
    int f00;
    short type;
    short pad06;
    float time;
    float f0C;
    float f10;
    float x, y, z;
};

struct sObj_341388 {
    int pad00[3];
    void** vt;
    int pad10[2];
    char* mtx;
    int pad1C;
    sVec4_341388 pos;
    float speed;
    float f34;
    int t38;
    int t3C;
    int type;
};

extern void* D_004914E0[];
extern char* D_004A5B64;
extern sVec4_341388 D_004FF120_341388[] __asm__("D_004FF120");
extern sVec4_341388 D_004FF150_341388[] __asm__("D_004FF150");
extern "C" void* func_0034FB00(void* self, void* a1, int type, void* a3);

// PORT: PS2-only VU0 inline asm (v / |v|).
static inline sVec4_341388 vu0Normalize_341388(const sVec4_341388& v)
{
    sVec4_341388 r;
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec4_341388 vu0Scale_341388(const sVec4_341388& v, float s)
{
    sVec4_341388 r;
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

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sVec4_341388 vu0Xform_341388(const sVec4_341388& v, const char* m)
{
    sVec4_341388 r;
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

extern "C" sObj_341388* func_00341388(sObj_341388* self, void* a1, int a2, void* a3, sDef_341388* def)
{
    func_0034FB00(self, a1, a2, a3);
    self->vt = D_004914E0;
    int type = def->type;
    self->type = type;
    self->t38 = (int)(def->time * (float)*(int*)(D_004A5B64 + 0x10));
    if (def->x == 0.0f && def->y == 0.0f && def->z == 0.0f) {
        if (type == 2) {
            self->pos = *D_004FF150_341388;
        } else {
            self->pos = *D_004FF120_341388;
        }
    } else {
        self->pos = vu0Normalize_341388(sVec4_341388(def->x, def->y, def->z, 0.0f));
    }
    self->speed = def->f10 * 27.77777862548828f;
    self->f34 = def->f0C;
    self->t3C = self->type ? self->t38 : 0;
    if (self->type == 2) {
        char* m = self->mtx + 0x10;
        self->pos = vu0Xform_341388(vu0Scale_341388(self->pos, -1.0f), m);
        self->speed = -self->speed;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_cBoostNode);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern void* D_004914E0[];
struct sVEntry_cBoostNode_cBoostNode {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cBoostNode_cBoostNode(void* self, void* a1, void* stream)
{
    cInstanceNode_cInstanceNode(self, a1, stream);
    *(void***)((char*)self + 0xC) = D_004914E0;
    sVEntry_cBoostNode_cBoostNode* vt = *(sVEntry_cBoostNode_cBoostNode**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x20, 0x30);
    return self;
}
#endif

extern void* D_004914E0[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003415A8__FPv);
#ifdef SKIP_ASM
void* func_003415A8(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_004914E0;
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003415D0);
#ifdef SKIP_ASM
extern "C" int func_002D1AF0();
extern "C" void** cObjectInterface_getNumBeings(int i);

struct sVEnt_003415D0 { short delta; short index; void* fn; };
typedef void (*Fn0_003415D0)(void*);
typedef void (*Fn1_003415D0)(void*, void*);
typedef void (*FnI_003415D0)(void*, int);
typedef int (*Test0_003415D0)(void*);
typedef int (*Test_003415D0)(void*, int);

struct sNode_003415D0 {
    int pad00[3];
    sVEnt_003415D0* vt;
    int pad10[2];
    int a18;
    int pad1C[6];
    float f34;
    int pad38;
    int t3C;
    int type;
};

extern "C" void func_003415D0(sNode_003415D0* self)
{
    int hit = 0;
    if (self->type != 1) {
        hit = 1;
    }
    int t = self->t3C;
    if (t > 0) {
        int n = t - 1;
        self->t3C = n;
        if (n > 0) {
            hit = 1;
        }
    }
    sVEnt_003415D0* vt = self->vt;
    ((Fn0_003415D0)vt[49].fn)((char*)self + vt[49].delta);
    if (self->type == 1 || self->t3C <= 0) {
        for (int i = 0; i < func_002D1AF0(); i++) {
            void** obj = cObjectInterface_getNumBeings(i);
            if (self->type == 2) {
                sVEnt_003415D0* ovt = (sVEnt_003415D0*)*obj;
                if (((Test0_003415D0)ovt[9].fn)((char*)obj + ovt[9].delta) != 0) continue;
            }
            sVEnt_003415D0* ovt = (sVEnt_003415D0*)*obj;
            if (((Test_003415D0)ovt[3].fn)((char*)obj + ovt[3].delta, self->a18) != 0) {
                hit = 1;
                if (self->f34 < 0.0f) {
                    sVEnt_003415D0* v = self->vt;
                    ((Fn1_003415D0)v[48].fn)((char*)self + v[48].delta, obj);
                } else {
                    sVEnt_003415D0* v = self->vt;
                    ((Fn1_003415D0)v[47].fn)((char*)self + v[47].delta, obj);
                }
            }
        }
    }
    sVEnt_003415D0* v2 = self->vt;
    ((Fn0_003415D0)v2[50].fn)((char*)self + v2[50].delta);
    if (hit == 0) {
        sVEnt_003415D0* v3 = self->vt;
        ((FnI_003415D0)v3[34].fn)((char*)self + v3[34].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341770);
#ifdef SKIP_ASM
struct sVec4_341770 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_341770 vu0Scale_341770(const sVec4_341770& v, float s)
{
    sVec4_341770 r;
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

class cMover_341770 {
public:
    virtual void v01();
    virtual sVec4_341770* getVelocity();
    virtual void v03();
    virtual void setVelocity(sVec4_341770* v);
};

extern "C" float func_002D1C70();

extern "C" void func_00341770(void* self, cMover_341770* obj)
{
    sVec4_341770 u = vu0Scale_341770(vu0Scale_341770(*obj->getVelocity(), *(float*)((char*)self + 0x34) + 1.0f), func_002D1C70());
    obj->setVelocity(&u);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341818);
#ifdef SKIP_ASM
struct sVec4_341818 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (v / |v|).
static inline sVec4_341818 vu0Normalize_341818(const sVec4_341818& v)
{
    sVec4_341818 r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_341818(const sVec4_341818& a, const sVec4_341818& b)
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
static inline sVec4_341818 vu0Scale_341818(const sVec4_341818& v, float s)
{
    sVec4_341818 r;
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

class cMover_341818 {
public:
    virtual void v01();
    virtual sVec4_341818* getVelocity();
    virtual void v03();
    virtual void setVelocity(sVec4_341818* v);
};

extern "C" float func_002D1C70();

struct sParams_341818 {
    sVec4_341818 dir;
    float speed;
    float scale;
};

struct sSph_341818 {
    char pad00[0x20];
    sParams_341818 params;
    int type;
};

static inline void push_341818(sParams_341818* p, cMover_341818* obj, const sVec4_341818& dir, float amount)
{
    sVec4_341818 u = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, amount), p->scale), func_002D1C70());
    obj->setVelocity(&u);
}

extern "C" void func_00341818(sSph_341818* self, cMover_341818* obj)
{
    sVec4_341818 vel = *obj->getVelocity();
    sVec4_341818 dir = self->params.dir;
    if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f) {
        dir = vu0Normalize_341818(vel);
    }
    float d = vu0Dot_341818(vel, dir);
    float diff = self->params.speed - d;
    sParams_341818* p = &self->params;
    if (self->type == 2) {
        if (d >= 0.0f) {
            sVec4_341818 u = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, p->speed), p->scale), func_002D1C70());
            obj->setVelocity(&u);
        }
    } else if (diff > 0.0f) {
        sVec4_341818 u = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, diff), p->scale), func_002D1C70());
        obj->setVelocity(&u);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341A30);
#ifdef SKIP_ASM
extern "C" void func_00341A30(void* self, int a1)
{
    if (a1 == 1) {
        *(int*)((char*)self + 0x3c) = *(int*)((char*)self + 0x38);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341A50);
#ifdef SKIP_ASM
struct sSerVEntry_00341A50 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

extern "C" void func_00341A50(void* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    sSerVEntry_00341A50* vt = *(sSerVEntry_00341A50**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x20, 0x30);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341AA0);
#ifdef SKIP_ASM
extern char* D_004A5B64;
extern void* D_00490B10[];
extern "C" void func_0034D9B0(void* p, int a1, int a2, void* obj, int a4);
extern "C" float func_00351508(void* inst);
float AIrandf(float lo, float hi);
float func_00317890(float a, float b);

struct sSphFloat_1AA0 {
    unsigned short id;  // 0x00
    char pad02[0x2];
    int active;         // 0x04
    int atLimit;        // 0x08
    int fC;             // 0x0C
    float speed;        // 0x10
    float min;          // 0x14
    float max;          // 0x18
    float value;        // 0x1C
    float start;        // 0x20
    char pad24[0x18];
    void** vt;          // 0x3C
    char pad40[0x2];
    unsigned short flags; // 0x42
    char pad44[0x20];
    void* inst;         // 0x64
};

struct sSphFloatDef_1AA0 {
    int f0;
    unsigned short id;  // 0x04
    char pad06[0x2];
    int dir;            // 0x08
    float min;          // 0x0C
    float max;          // 0x10
    float speed;        // 0x14
    float speedVar;     // 0x18
    float value;        // 0x1C
    int random;         // 0x20
    int f24;            // 0x24
};

struct sFloat_1AA0 {
    float v;
};

static inline void setRate_1AA0(sFloat_1AA0* dst, float v, float dflt)
{
    float r;
    if (v >= 0.0f) {
        r = v * 0.03333333507180214f;
    } else {
        r = dflt;
    }
    dst->v = r;
}

static inline void setRateMax_1AA0(sFloat_1AA0* dst, float v, char* info)
{
    float r;
    if (v < 0.0f) {
        r = *(float*)(info + 0x14);
    } else {
        r = v * 0.03333333507180214f;
    }
    dst->v = r;
}

// Animated float constructor: range and start value (frames -> seconds), optional random start and speed.
extern "C" void* func_00341AA0(sSphFloat_1AA0* self, int a1, int a2, char* obj, sSphFloatDef_1AA0* def)
{
    func_0034D9B0((char*)self + 0x1C, a1, a2, obj, def->f24 != 0);
    self->vt = D_00490B10;
    float v = 0.0f;
    char* info = *(char**)(obj + 0x80);
    if (self->inst != 0) {
        v = func_00351508(self->inst);
    }
    self->id = def->id;
    setRate_1AA0((sFloat_1AA0*)&self->min, def->min, v);
    setRateMax_1AA0((sFloat_1AA0*)&self->max, def->max, info);
    v = def->value;
    if (v < 0.0f) {
        v = self->min;
    } else {
        v *= 0.03333333507180214f;
    }
    if (v < self->min) {
        v = self->min;
    }
    if (self->max < v) {
        v = self->max;
    }
    if (def->random != 0) {
        v = AIrandf(self->min, self->max);
    }
    self->value = v;
    if (def->speedVar != 0.0f) {
        self->speed = func_00317890(def->speed * 0.03333333507180214f, def->speedVar * 0.03333333507180214f) / (float)*(int*)(D_004A5B64 + 0x10);
    } else {
        self->speed = def->speed * 0.03333333507180214f / (float)*(int*)(D_004A5B64 + 0x10);
    }
    if (def->dir == 1) {
        self->speed = -self->speed;
    }
    self->start = self->value;
    self->active = 1;
    self->flags |= 1;
    self->fC = 0;
    self->atLimit = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_cAnimObjectNode);
#ifdef SKIP_ASM
struct sVEntry00341C80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_0034DAC8(void* p, int a1, void* stream);
extern void* D_00490B10[];

extern "C" void* cAnimObjectNode_cAnimObjectNode(void* self, int a1, sVEntry00341C80** stream)
{
    func_0034DAC8((char*)self + 0x1C, a1, stream);
    *(void***)((char*)self + 0x3C) = D_00490B10;
    (*stream)[2].fn((char*)stream + (*stream)[2].delta, self, 0x1C);
    *(unsigned short*)((char*)self + 0x42) |= 1;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341CF0);
#ifdef SKIP_ASM
extern "C" void func_0034DBA8(void* p, int flags);
void operator_delete(int* ptr);
extern void* D_00490B10[];

extern "C" void func_00341CF0(void* self, int flags)
{
    *(void***)((char*)self + 0x3C) = D_00490B10;
    func_0034DBA8((char*)self + 0x1C, 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341D48);
#ifdef SKIP_ASM
struct sWrapFloat_1E48;
struct sBounceFloat_1EC0;
struct sClampFloat_1F38;
extern "C" void func_00341E48(sWrapFloat_1E48* s);
extern "C" void func_00341EC0(sBounceFloat_1EC0* s);
extern "C" void func_00341F38(sClampFloat_1F38* s);
extern "C" int func_0034EBA0(void* p);

struct sVEntry_00341D48 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" int func_00341D48(void* self)
{
    *(float*)((char*)self + 0x24) = 10000000000.0f;
    *(float*)((char*)self + 0x28) = 10000000000.0f;
    if (*(int*)((char*)self + 0x8) != 0) {
        return 1;
    }
    if (*(int*)((char*)self + 0x4) != 0 && *(int*)((char*)self + 0xC) >= 0) {
        if (*(int*)((char*)self + 0xC) > 0) {
            *(int*)((char*)self + 0xC) -= 1;
        } else {
            *(float*)((char*)self + 0x24) = *(float*)((char*)self + 0x1C);
            switch (*(short*)self) {
            case 2:
                func_00341EC0((sBounceFloat_1EC0*)self);
                break;
            case 1:
                func_00341E48((sWrapFloat_1E48*)self);
                break;
            default:
                func_00341F38((sClampFloat_1F38*)self);
                break;
            }
            if (func_0034EBA0((char*)self + 0x1C) == 0) {
                return 0;
            }
        }
    }
    char* obj = (char*)self + 0x30;
    *(unsigned short*)((char*)self + 0x42) |= 1;
    *(float*)((char*)self + 0x20) = *(float*)((char*)self + 0x1C);
    if (*(int*)((char*)self + 0x8) == 0) {
        return 1;
    }
    sVEntry_00341D48* vt = *(sVEntry_00341D48**)((char*)self + 0x3C);
    vt[0x22].fn(obj + vt[0x22].delta, 1);
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341E48);
#ifdef SKIP_ASM
struct sWrapFloat_1E48 {
    char pad_0x00[0x10];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341E48(sWrapFloat_1E48* s)
{
    if (s->speed >= 0.0f) {
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->value = s->min + (s->value - s->max);
        }
    } else {
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->value = s->max - (s->min - s->value);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341EC0);
#ifdef SKIP_ASM
struct sBounceFloat_1EC0 {
    char pad_0x00[0x10];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341EC0(sBounceFloat_1EC0* s)
{
    if (s->speed >= 0.0f) {
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->speed = -s->speed;
            s->value = s->max - (s->value - s->max);
        }
    } else {
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->speed = -s->speed;
            s->value = s->min + (s->min - s->value);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341F38);
#ifdef SKIP_ASM
struct sClampFloat_1F38 {
    char pad_0x00[0x8];
    int atLimit;    // 0x08
    char pad_0x0C[0x4];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341F38(sClampFloat_1F38* s)
{
    if (s->speed >= 0.0f) {
        if (s->value == s->max) {
            s->atLimit = 1;
        }
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->value = s->max;
        }
    } else {
        if (s->value == s->min) {
            s->atLimit = 1;
        }
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->value = s->min;
        }
    }
}
#endif

void func_0034FCC0(void*);

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_expire);
#ifdef SKIP_ASM
extern "C" void cAnimObjectNode_expire(void* self, int a1)
{
    if (a1 != 0) {
        func_0034FCC0((char*)self + 0x30);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341FE8);
#ifdef SKIP_ASM
extern char* D_004A5B64;
extern "C" void cMoveNode_signal(void* p);

extern "C" void func_00341FE8(sClampFloat_1F38* self, int msg, float v)
{
    if (msg == 0x64) {
        *(int*)((char*)self + 0xC) = (int)(v * (float)*(int*)(D_004A5B64 + 0x10));
    } else if (msg == 0x65) {
        v = v * 0.03333333507180214f;
        if (self->min <= v && v <= self->max) {
            self->value = v;
        }
    } else if (msg == 0x66) {
        v = v * 0.03333333507180214f;
        self->speed = v / (float)*(int*)(D_004A5B64 + 0x10);
    } else if (msg == 0x67) {
        v = v * 0.03333333507180214f;
        float lim = *(float*)(*(char**)(*(char**)((char*)self + 0x48) + 0x80) + 0x14);
        if (lim < v) {
            v = lim;
        }
        if (v < 0.0f) {
            v = 0.0f;
        }
        self->max = v;
        if (v < self->value) {
            self->value = v;
        }
    } else if (msg == 0x68) {
        v = v * 0.03333333507180214f;
        float lim = *(float*)(*(char**)(*(char**)((char*)self + 0x48) + 0x80) + 0x14);
        if (lim < v) {
            v = lim;
        }
        if (v < 0.0f) {
            v = 0.0f;
        }
        self->min = v;
        if (self->value < v) {
            self->value = v;
        }
    } else if (msg == 0x69) {
        *(short*)self = 0;
    } else {
        cMoveNode_signal((char*)self + 0x30);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342150);
#ifdef SKIP_ASM
extern "C" void func_0034E3D8(void* p, void* stream);
struct sVEntry_func_00342150 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00342150(void* self, void* stream)
{
    func_0034E3D8((char*)self + 0x1C, stream);
    sVEntry_func_00342150* vt = *(sVEntry_func_00342150**)stream;
    vt[1].fn((char*)stream + vt[1].delta, self, 0x1C);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003421A0);
#ifdef SKIP_ASM
extern char* D_004A5B64;
extern void* D_004908F8[];
extern "C" void func_0034D9B0(void* p, int a1, int a2, void* obj, int a4);
extern "C" float func_00351508(void* inst);

struct sSphCtl_003421A0 {
    int f00;            // 0x00
    int f04;            // 0x04
    float f08;          // 0x08
    float f0C;          // 0x0C
    float f10;          // 0x10
    float f14;          // 0x14
    float f18;          // 0x18
    float f1C;          // 0x1C
    float f20;          // 0x20
    float f24;          // 0x24
    int f28;            // 0x28
    int f2C;            // 0x2C
    char pad30[0x20];
    void** vt;          // 0x50
    char pad54[2];
    unsigned short flags; // 0x56
    char pad58[0x20];
    void* inst;         // 0x78
};

struct sFloat_003421A0 {
    float v;
};

static inline void setRate_003421A0(sFloat_003421A0* dst, float v, float dflt)
{
    float r;
    if (v >= 0.0f) {
        r = v * 0.03333333507180214f;
    } else {
        r = dflt;
    }
    dst->v = r;
}

static inline void setRateMax_003421A0(sFloat_003421A0* dst, float v, char* info)
{
    float r;
    if (v < 0.0f) {
        r = *(float*)(info + 0x14);
    } else {
        r = v * 0.03333333507180214f;
    }
    dst->v = r;
}

extern "C" void* func_003421A0(sSphCtl_003421A0* self, int a1, char* obj, char* def)
{
    func_0034D9B0((char*)self + 0x30, a1, 4, obj, 0);
    self->vt = D_004908F8;
    float t = 0.0f;
    char* info = *(char**)(obj + 0x80);
    if (self->inst != 0) {
        t = func_00351508(self->inst);
    }
    setRate_003421A0((sFloat_003421A0*)&self->f10, *(float*)(def + 0x20), t);
    setRateMax_003421A0((sFloat_003421A0*)&self->f14, *(float*)(def + 0x24), info);
    self->f0C = *(float*)(def + 0x8);
    self->f08 = *(float*)(def + 0x4) * 9.999999974752427e-07f;
    self->f18 = *(float*)(def + 0xC) * 0.03333333507180214f / (float)*(int*)(D_004A5B64 + 0x10);
    self->f2C = *(int*)(def + 0x1C);
    self->f24 = *(float*)(def + 0x14) * 0.03333333507180214f;
    self->f1C = *(float*)(def + 0x18);
    self->f20 = *(float*)(def + 0x10);
    self->flags |= 1;
    self->f00 = 0;
    self->f04 = 0;
    self->f28 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_cAnimTeeterNode);
#ifdef SKIP_ASM
struct sVEntry003422E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_0034DAC8(void* p, int a1, void* stream);
extern void* D_004908F8[];

extern "C" void* cAnimTeeterNode_cAnimTeeterNode(void* self, int a1, sVEntry003422E8** stream)
{
    func_0034DAC8((char*)self + 0x30, a1, stream);
    *(void***)((char*)self + 0x50) = D_004908F8;
    (*stream)[2].fn((char*)stream + (*stream)[2].delta, self, 0x30);
    *(unsigned short*)((char*)self + 0x56) |= 1;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342358);
#ifdef SKIP_ASM
extern "C" float func_002D1C70();
extern "C" int func_0034EBA0(void* p);

struct sSphCtl_00342358 {
    float force;        // 0x00
    float step;         // 0x04
    float gain;         // 0x08
    float bounce;       // 0x0C
    float min;          // 0x10
    float max;          // 0x14
    float maxStep;      // 0x18
    float damping;      // 0x1C
    float stiffness;    // 0x20
    float rest;         // 0x24
    float vel;          // 0x28
    int f2C;            // 0x2C
    float pos;          // 0x30
    float pos34;        // 0x34
    float prev;         // 0x38
    float pos3C;        // 0x3C
    char pad40[0x16];
    unsigned short flags; // 0x56
};

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_00342358(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" int func_00342358(sSphCtl_00342358* s)
{
    s->prev = s->pos;
    float dt = func_002D1C70();
    float force = s->gain * s->force;
    if (s->gain < 0.0f) {
        force = -force;
    }
    float vel = s->vel;
    float maxStep = s->maxStep;
    float step = vel * dt;
    float acc = -s->stiffness * (s->pos - s->rest);
    acc += force;
    acc += -s->damping * vel;
    s->step = step;
    if (maxStep < absf_00342358(step)) {
        if (step < 0.0f) {
            s->step = -maxStep;
        } else {
            s->step = maxStep;
        }
    }
    float pos = s->pos + s->step;
    float nv = s->vel + acc * dt;
    s->pos = pos;
    s->pos3C = pos;
    s->vel = nv;
    if (pos < s->min) {
        s->pos = s->min;
        if (absf_00342358(nv) > 0.1f) {
            s->vel = nv * s->bounce;
        } else {
            s->step = 0.0f;
            s->vel = 0.0f;
        }
    } else if (s->max < pos) {
        s->pos = s->max;
        if (absf_00342358(nv) > 0.1f) {
            s->vel = nv * s->bounce;
        } else {
            s->step = 0.0f;
            s->vel = 0.0f;
        }
    }
    s->force = 0.0f;
    s->pos34 = s->pos;
    if (func_0034EBA0(&s->pos) == 0) {
        return 0;
    }
    s->flags |= 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003424D0);
#ifdef SKIP_ASM
extern "C" float func_003424D0(void* self)
{
    if (*(float*)((char*)self + 0x30) <= *(float*)((char*)self + 0x10) && *(float*)((char*)self + 0x4) < 0.0f) {
        return 0.0f;
    }
    if (*(float*)((char*)self + 0x30) >= *(float*)((char*)self + 0x14) && *(float*)((char*)self + 0x4) > 0.0f) {
        return 0.0f;
    }
    return *(float*)((char*)self + 0x4);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342538);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// ---- Shared declarations: one type per real thing, used by the functions below ----

// g++ 2.95 vtable entry with an untyped function slot (cast at the call).
struct sVec4_342538 {
    float x, y, z, w;
    sVec4_342538() {}
} __attribute__((aligned(16)));

typedef char* (*GetMat_342538)(void*, int);

struct sBodyTbl_342538 { char pad00[8]; char* ents; };
struct sBodyInf_342538 { char pad00[0x80]; sBodyTbl_342538* tbl; };
struct sSub_342538 { char pad00[0x18]; sBodyInf_342538* inf; };

struct sSelf_342538 {
    float sum;                      // 0x00
    char pad04[4];
    float dir;                      // 0x08
    char pad0C[0x20];
    int mask;                       // 0x2C
    char pad30[0x2C];
    sBodyInf_342538* inf;            // 0x5C
};

struct sObj_342538 {
    sVec4_342538 pos;               // 0x00
    char pad10[0x4C];
    int idx;                        // 0x5C
};

extern sVec4_342538 D_004FF150_342538[] __asm__("D_004FF150");

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float vu0Length_342538(const sVec4_342538& v)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline void vu0Sub_342538(sVec4_342538& r, const sVec4_342538& a, const sVec4_342538& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline void vu0Scale_342538(sVec4_342538& r, const sVec4_342538& v, float s)
{
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
}

// PORT: PS2-only VU0 inline asm (matrix times vector).
static inline void vu0Xform_342538(sVec4_342538& r, const sVec4_342538& v, const char* m)
{
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
        : "r"(m), "o"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (cross product xyz).
static inline void vu0Cross_342538(sVec4_342538& r, const sVec4_342538& a, const sVec4_342538& b)
{
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
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_342538(const sVec4_342538& a, const sVec4_342538& b)
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

extern "C" void func_00342538(sSelf_342538* self, sObj_342538* o, sVec4_342538* p)
{
    int idx = o->idx;
    if (idx >= 0) {
        sSub_342538* sub = (sSub_342538*)((char*)self + 0x44);
        char* ents = sub->inf->tbl->ents;
        if (*(int*)(ents + idx * 16 + 8) != 0) {
            sWsVtSlot* vt = *(sWsVtSlot**)((char*)self + 0x50);
            char* m = ((GetMat_342538)vt[29].fn)((char*)sub + vt[29].delta, idx);
            char* m2 = m;
            if (vu0Length_342538(*p) > 100.0f) {
                sVec4_342538 d;
                sVec4_342538 a;
                sVec4_342538 t;
                sVec4_342538 u;
                a = *(sVec4_342538*)(m + 0x30);
                vu0Sub_342538(t, o->pos, a);
                d = t;
                if (self->dir < 0.0f) {
                    vu0Scale_342538(t, d, -1.0f);
                    a = t;
                    d = t;
                }
                if ((self->mask >> o->idx) & 1) {
                    vu0Scale_342538(t, d, -1.0f);
                    a = t;
                    d = t;
                }
                vu0Xform_342538(t, D_004FF150_342538[0], m2);
                a = t;
                vu0Cross_342538(u, d, *p);
                t = u;
                self->sum += vu0Dot_342538(a, t);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342718);
#ifdef SKIP_ASM
extern "C" void func_0034E3D8(void* p, void* stream);
struct sVEntry_func_00342718 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00342718(void* self, void* stream)
{
    func_0034E3D8((char*)self + 0x30, stream);
    sVEntry_func_00342718* vt = *(sVEntry_func_00342718**)stream;
    vt[1].fn((char*)stream + vt[1].delta, self, 0x30);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342768);
#ifdef SKIP_ASM
struct sVec4_342768 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sSphDef_342768 {
    int f00;
    short type;
    short pad06;
    float time;
    float f0C;
    float f10;
    float x, y, z;
};
struct sSph_342768 {
    int refs;
    void** vt;
    int pad08[2];
    sVec4_342768 pos;
    float f20;
    float f24;
    int t28;
    int t2C;
    int type;
    int pad34[3];
    int a40;
};
extern void* D_00490898[];
extern char* D_004A5B64;

extern "C" void* func_00342768(sSph_342768* self, sSphDef_342768* def, int a2)
{
    self->refs = 1;
    self->vt = D_00490898;
    self->a40 = a2;
    int type = def->type;
    self->type = type;
    int t = (int)(def->time * (float)*(int*)(D_004A5B64 + 0x10));
    self->t28 = t;
    sVec4_342768 v;
    v.x = def->x;
    v.y = def->y;
    v.z = def->z;
    v.w = 0.0f;
    self->pos = v;
    self->f20 = def->f10 * 100.0f;
    self->f24 = def->f0C;
    self->t2C = type == 1 ? t : 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342808);
#ifdef SKIP_ASM
struct sVEntry00342808 {
    short delta;
    short index;
    void* fn;
};

typedef void (*ReadFn00342808)(void*, void*, int);
typedef int (*ReadIntFn00342808)(void*);

extern void* D_00490898[];

extern "C" void* func_00342808(void* self, sVEntry00342808** stream)
{
    *(int*)((char*)self + 0x0) = 1;
    *(void***)((char*)self + 0x4) = D_00490898;
    ((ReadFn00342808)(*stream)[2].fn)((char*)stream + (*stream)[2].delta, (char*)self + 0x10, 0x30);
    *(int*)((char*)self + 0x40) = ((ReadIntFn00342808)(*stream)[3].fn)((char*)stream + (*stream)[3].delta);
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_update);
#ifdef SKIP_ASM
extern "C" int func_002D1AF0();
extern "C" void** cObjectInterface_getNumBeings(int i);

struct sVEnt_00342880 { short delta; short index; void* fn; };
typedef void (*Fn0_00342880)(void*);
typedef void (*Fn1_00342880)(void*, void*);
typedef int (*Test_00342880)(void*, int);

extern "C" void cBoostModifier_update(sSph_342768* self)
{
    self->refs = 0;
    if (self->type == 0) {
        self->refs = 1;
    }
    int t = self->t2C;
    if (t > 0) {
        t--;
        self->t2C = t;
        if (t > 0) {
            self->refs = 1;
        }
    }
    sVEnt_00342880* vt = (sVEnt_00342880*)self->vt;
    ((Fn0_00342880)vt[9].fn)((char*)self + vt[9].delta);
    if (self->type == 1 || self->t2C <= 0) {
        for (int i = 0; i < func_002D1AF0(); i++) {
            void** obj = cObjectInterface_getNumBeings(i);
            sVEnt_00342880* ovt = (sVEnt_00342880*)*obj;
            if (((Test_00342880)ovt[3].fn)((char*)obj + ovt[3].delta, self->a40) != 0) {
                self->refs = 1;
                if (self->f24 < 0.0f) {
                    sVEnt_00342880* v = (sVEnt_00342880*)self->vt;
                    ((Fn1_00342880)v[8].fn)((char*)self + v[8].delta, obj);
                } else {
                    sVEnt_00342880* v = (sVEnt_00342880*)self->vt;
                    ((Fn1_00342880)v[7].fn)((char*)self + v[7].delta, obj);
                }
            }
        }
    }
    sVEnt_00342880* v2 = (sVEnt_00342880*)self->vt;
    ((Fn0_00342880)v2[10].fn)((char*)self + v2[10].delta);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003429E0);
#ifdef SKIP_ASM
struct sVec4_3429E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_3429E0 vu0Scale_3429E0(const sVec4_3429E0& v, float s)
{
    sVec4_3429E0 r;
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

class cMover_3429E0 {
public:
    virtual void v01();
    virtual sVec4_3429E0* getVelocity();
    virtual void v03();
    virtual void setVelocity(sVec4_3429E0* v);
};

extern "C" float func_002D1C70();

extern "C" void func_003429E0(void* self, cMover_3429E0* obj)
{
    sVec4_3429E0 u = vu0Scale_3429E0(vu0Scale_3429E0(*obj->getVelocity(), *(float*)((char*)self + 0x24) + 1.0f), func_002D1C70());
    obj->setVelocity(&u);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342A88);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// Interface of a moving object (getVelocity / setVelocity at slots 2 and 4).
class cWsMover {
public:
    virtual void v01();
    virtual t4Vector* getVelocity();
    virtual void v03();
    virtual void setVelocity(t4Vector* v);
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot(const t4Vector& a, const t4Vector& b)
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

extern "C" float func_002D1C70();

struct t4Vector;



static inline void setVel_342A88(cWsMover* o, t4Vector* v)
{
    o->setVelocity(v);
}

struct sPlane_342A88 {
    t4Vector n;
    float d;
    float k;
};

struct sSelf_342A88 {
    char pad[0x10];
    sPlane_342A88 plane;
};

extern "C" void func_00342A88(sSelf_342A88* self, cWsMover* obj)
{
    float d = self->plane.d - vu0Dot(*obj->getVelocity(), self->plane.n);
    if (d > 0.0f) {
        // PORT: takes the address of a temporary (g++ 2.95 accepts it).
        sPlane_342A88* pl = &self->plane;
        setVel_342A88(obj, &vu0Scale(vu0Scale(vu0Scale(pl->n, d), pl->k), func_002D1C70()));
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_signal);
#ifdef SKIP_ASM
extern "C" void cBoostModifier_signal(void* self, int a1)
{
    if (a1 == 0x258) {
        *(int*)((char*)self + 0x2c) = *(int*)((char*)self + 0x28);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342BA0);
#ifdef SKIP_ASM
extern "C" void func_003584B8(void* self, void* stream);
struct sVEntry_func_00342BA0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};
struct sVEntry1_func_00342BA0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00342BA0(void* self, void* stream)
{
    func_003584B8(self, stream);
    sVEntry_func_00342BA0* vt = *(sVEntry_func_00342BA0**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x10, 0x30);
    sVEntry1_func_00342BA0* vt1 = *(sVEntry1_func_00342BA0**)stream;
    vt1[5].fn((char*)stream + vt1[5].delta, *(int*)((char*)self + 0x40));
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342C08);
#ifdef SKIP_ASM
extern "C" void* func_00355280(void* self, void* a1, int type, void* a3);
extern "C" void func_00355F10(void* self, void* a1);
extern void* D_004906F0[];

struct sNode_342C08 {
    int pad00[2];
    unsigned int flags;
};

struct sDef_342C08 {
    int f00;
    float time;
    int f08;
    int mode;
};

struct sSelf_342C08 {
    int pad00[3];
    void** vt;
    int pad10[2];
    sNode_342C08* node;
    int pad1C[4];
    int t2C;
    int t30;
};

extern "C" void* func_00342C08(sSelf_342C08* self, void* a1, void* a2, sDef_342C08* def, void* a4)
{
    func_00355280(self, a1, 0, a2);
    self->vt = D_004906F0;
    self->t2C = (int)(def->time * (float)*(int*)(D_004A5B64 + 0x10));
    self->t30 = def->f08;
    func_00355F10(self, a4);
    if (def->mode == 0) {
        self->node->flags &= 0xFFFFFFF0;
    } else if (def->mode == 1) {
        self->node->flags |= 1;
        self->node->flags = (self->node->flags & ~2u) | 4;
    } else {
        sNode_342C08* n = self->node;
        if ((n->flags & 3) == 3) {
            n->flags = (n->flags & ~2u) | 4;
        }
    }
    return self;
}
#endif


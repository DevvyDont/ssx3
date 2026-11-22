#include "common.h"
#include "vtable.h"
#include "visualfx/fog.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("seg/seg_1DCD10", cFXLoader_cFXLoader);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct Pre_BD10 { char pad0[0xF50]; int texmap[0x51]; char pad1[0x10B0 - 0x1094]; int dst[10]; };
class Mgr_BD10 : public Pre_BD10 {
public:
  void SetEffectTexMap(int effectID, int tex) {
    texmap[effectID] = tex;
  }
  int GetEffectTexMap(int effectID) {
    return texmap[effectID];
  }
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual void v7();
  virtual void v8();
  virtual void v9();
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
  virtual int AddTexture(long, char*, int, int, long);
};
extern Mgr_BD10 *D_BD10 __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" int FILE_load(char*, int);
extern "C" long SHAPE_locatez(int, char*);
extern "C" void MEM_free(int);
extern char str_data_textures_particle_ssh[];
extern char str_data_textures_effects_ssh[];
struct sFXShape_BD10 { char name[8]; int tex; };
extern sFXShape_BD10 gFXShape_BD10[] __asm__("D_004891B0");
/**
 * Constructs the effects loader: loads the effect texture shapes and registers each effect's texture.
 *
 * Loads "data/textures/particle.ssh" and, unless @p particlesOnly is set, "data/textures/effects.ssh" with FILE_load (0x100). For
 * each of the 0x51 effect shapes in D_004891B0 (an 8-byte name and a texture argument) it searches the loaded files
 * with SHAPE_locatez, registers the first hit through a cGraphicsMan virtual (slot 46: shape, name, texture argument,
 * 1, -1) and stores the texture ID, or -1, in cGraphicsMan_TheGraphicsMan's effect texture map (+0xF50). The files are
 * freed with MEM_free, and effect IDs 0x47..0x50 are copied to TheGraphicsMan +0x10B0.
 *
 * The object itself gets no members: everything goes into the graphics manager.
 *
 * @param loader  the object to construct (this)
 * @param particlesOnly  non-zero to load only the particle shapes (On Tour ParticlesOnly)
 * @return @p loader
 */
extern "C" int cFXLoader_cFXLoader(int loader, long particlesOnly) {
    int files[2];
    int texId;
    long shape;
    int n = 2;
    if (particlesOnly)
        n = 1;
    for (int i = 0; i < n; i++) {
        switch (i) {
        case 0:
            files[i] = FILE_load(str_data_textures_particle_ssh, 0x100);
            break;
        case 1:
            files[i] = FILE_load(str_data_textures_effects_ssh, 0x100);
            break;
        }
    }
    Mgr_BD10* grMan = D_BD10;
    for (int k = 0; k < 81; k++) {
        shape = 0;
        for (int i = 0; i < n; i++) {
            shape = SHAPE_locatez(files[i], gFXShape_BD10[k].name);
            if (shape != 0)
                break;
        }
        texId = -1;
        if (shape != 0)
            texId = D_BD10->AddTexture(shape, gFXShape_BD10[k].name, gFXShape_BD10[k].tex, 1, -1);
        grMan->SetEffectTexMap(k, texId);
    }
    for (int i = 0; i < n; i++)
        MEM_free(files[i]);
    for (int i = 0; i < 10; i++)
        grMan->dst[i] = grMan->GetEffectTexMap(71 + i);
    return loader;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1DCD10", cFXLoader__cFXLoader);
#ifdef SKIP_ASM
struct Pre_BEE0 { char pad[0x10D8]; };
class Mgr_BEE0 : Pre_BEE0 {
public:
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual void v7();
  virtual void v8();
  virtual void v9();
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
  virtual void v48b();
  virtual void RemoveTexture(int);
};
extern Mgr_BEE0 *cGraphicsMan_TheGraphicsMan_seg_1DCD10 __asm__("cGraphicsMan_TheGraphicsMan");
void operator_delete(int *);
/**
 * Destroys the effects loader: removes each effect texture it registered.
 *
 * Walks cGraphicsMan_TheGraphicsMan's 0x51-entry effect texture map (+0xF50): every ID that is not -1 is removed
 * through a cGraphicsMan virtual (slot 50; On Tour RemoveTexture) and reset to -1. Frees the object when bit 0 of @p
 * flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFXLoader__cFXLoader(int *self, int flags) {
    Mgr_BEE0 *grMan = cGraphicsMan_TheGraphicsMan_seg_1DCD10;
    int i;
    for (i = 0; i < 81; i++) {
        int *slot = (int*)((char*)grMan + 0xF50) + i;
        if (*slot != -1) {
            grMan->RemoveTexture(*slot);
            *slot = -1;
        }
    }
    if (flags & 1) operator_delete(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1DCD10", cFogParticleMan_cFogParticleMan);
#ifdef SKIP_ASM
extern char cFogParticleMan_vtbl[];
/**
 * Constructs the base fog-sprite manager: clears the sprite count miNumSphere (+0) and stores cFogParticleMan_vtbl at
 * +4.
 *
 * cGame_loadTrack calls it on the 0x3010-byte cPS2FogParticleMan block, then stores the derived vtable.
 *
 * @return the object
 */
extern "C" void *cFogParticleMan_cFogParticleMan(int *self) {
    self[0] = 0;
    *(char**)((char*)self + 4) = cFogParticleMan_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1DCD10", cFogParticleMan_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void qsort_BF98(void* base, int n, int size, int (*cmp)(void*, void*)) __asm__("qsort");
extern "C" int cFogParticleMan__FogSortFuc(void* arg0, void* arg1);
struct RS_BF98 {
    unsigned int w0_lo : 2;
    unsigned int a : 2;
    unsigned int w0_hi : 28;
    unsigned int b : 2;
    unsigned int c : 5;
    unsigned int w1_p7 : 5;
    unsigned int d : 8;
    unsigned int e : 2;
    unsigned int f : 1;
    unsigned int g : 2;
    unsigned int w1_hi : 7;
    unsigned int w2_lo : 5;
    unsigned int h : 5;
    unsigned int i : 19;
    unsigned int w2_hi : 3;
    int w3;
    int w4;
};
struct Ctx_BF98 { char pad[0xE84]; RS_BF98* top; char pad2[0xF60 - 0xE88]; int f60; };
extern Ctx_BF98* GrMan_seg_1DCD10 __asm__("GrMan");
extern char cFogParticleMan_mSortKey[];
struct VEnt_BF98 { short delta; short idx; void (*fn)(void*); };
struct Obj_BF98 { int n; char* vt; };

static inline void Push_BF98() {
    Ctx_BF98* ctx = GrMan_seg_1DCD10;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}
static inline void Pop_BF98() {
    GrMan_seg_1DCD10->top--;
}
static inline void G3_BF98(int c, int f, int g, int e, int d) {
    Ctx_BF98* ctx = GrMan_seg_1DCD10;
    ctx->top->c = c;
    ctx->top->f = f;
    ctx->top->g = g;
    ctx->top->e = e;
    ctx->top->d = d;
}

/**
 * Renders the frame's fog particles and empties the list.
 *
 * Does nothing while miNumSphere is 0. Otherwise sorts the first miNumSphere 8-byte sort keys at cFogParticleMan_mSortKey (On Tour
 * cFogParticleMan::mSortKey) with qsort and the comparator cFogParticleMan__FogSortFuc, pushes a copy of the graphics
 * manager's top render-state record (GrMan +0xE84) and sets several of its fields, calls virtual slot 3
 * (platformrender, cPS2FogParticleMan_platformrender in cPS2FogParticleMan), pops the state and resets miNumSphere to 0.
 */
extern "C" void cFogParticleMan_render(cFogParticleMan* self) {
    if (self->miNumSphere) {
        qsort_BF98(cFogParticleMan_mSortKey, self->miNumSphere, 8, cFogParticleMan__FogSortFuc);
        Push_BF98();
        {
            Ctx_BF98* ctx = GrMan_seg_1DCD10;
            int texId = ctx->f60;
            ctx->top->h = 7;
            ctx->top->b = 2;
            ctx->top->i = 0x3FF;
            *(short*)((char*)ctx->top + 0x10) = texId;
        }
        {
            Ctx_BF98* ctx = GrMan_seg_1DCD10;
            ctx->top->a = 3;
            *(short*)((char*)ctx->top + 0x12) = -1;
        }
        G3_BF98(5, 1, 0, 3, 0);
        {
            VEnt_BF98* vtEntry = (VEnt_BF98*)((char*)self->mVtbl + 0x18);
            vtEntry->fn(vthis(self, *vtEntry));
        }
        Pop_BF98();
        self->miNumSphere = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1DCD10", cFogParticleMan__FogSortFuc);
#ifdef SKIP_ASM
/**
 * Compares two fog sort keys by their view depth (miZ, +4), for the qsort in cFogParticleMan_render: the deeper key
 * sorts first.
 *
 * @param lhs  the first key
 * @param rhs  the second key
 * @return 1 when @p lhs is nearer, -1 when it is farther, 0 when equal
 */
extern "C" int cFogParticleMan__FogSortFuc(void *lhs, void *rhs) {
    int temp_3;
    int temp_4;

    temp_4 = (*(int *)((char*)(lhs) + (4)));
    temp_3 = (*(int *)((char*)(rhs) + (4)));
    if (temp_4 >= temp_3) {
        return (temp_3 >= temp_4) ? 0 : -1;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1DCD10", cPS2FogParticleMan_addFogInstance);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV3_C190 {
    float x, y, z;
    sV3_C190() {}
    sV3_C190(float a, float b, float c) : x(a), y(b), z(c) {}
};

struct sV4_C190 {
    float x, y, z, w;
    sV4_C190() {}
    sV4_C190(const sV3_C190& v, const float& vw) : x(v.x), y(v.y), z(v.z), w(vw) {}
    sV4_C190(float vx, float vy, float vz, const float& vw) : x(vx), y(vy), z(vz), w(vw) {}
    sV4_C190(float vx, float vy) : x(vx), y(vy), z(0.0f), w(0.0f) {}
} __attribute__((aligned(16)));

struct sRow_C190 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_C190;

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = a * b).
static inline void mulMat_C190(sMat_C190* d, const sMat_C190* a, const sMat_C190* b)
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
static inline void copyMat_C190(sMat_C190* d, const sMat_C190* s)
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


struct sMat_C190 {
    sRow_C190 r[4];
    sMat_C190() {}
    // Product constructor: *this = a * b.
    sMat_C190(const sMat_C190& a, const sMat_C190& b) { mulMat_C190(this, &a, &b); }
    sMat_C190& operator=(const sMat_C190& s)
    {
        copyMat_C190(this, &s);
        return *this;
    }
};

struct sCtx_C190 {
    char pad_0x0[0x6AF0];
    float sx;                       // 0x6AF0
    char pad_0x6AF4[0x10];
    float sy;                       // 0x6B04
    char pad_0x6B08[0x28];
    sMat_C190 view;                 // 0x6B30
    sV4_C190 clipMin;               // 0x6B70
    sV4_C190 clipMax;               // 0x6B80
    int ready;                      // 0x6B90
};

struct sVert_C190 {
    sV3_C190 pos;                   // 0x0
    float col[3];                   // 0xC
    float size;                     // 0x18
};

struct sPart_C190 {
    char pad_0x0[0x1C];
    int count;                      // 0x1C
    sVert_C190* verts;              // 0x20
};

struct sList_C190 {
    int f0;
    sPart_C190* part;               // 0x4
};

struct sMesh_C190 {
    int f0;
    int count;                      // 0x4
    sList_C190* list;               // 0x8
};

struct sObj_C190 {
    char pad_0x0[0x10];
    sMat_C190 mat;                  // 0x10
    char pad_0x50[0x14];
    sMesh_C190* mesh;               // 0x64
    sV3_C190 min;                   // 0x68
    sV3_C190 max;                   // 0x74
};


struct sOut_C190 {
    int count;
    int pad[3];
    tFogSprite e[1];              // 0x10
};

struct sKey_C190 {
    int idx;
    int depth;
};

extern sCtx_C190* D_C190 __asm__("cGraphicsMan_TheGraphicsMan");
extern sKey_C190 D_00537AC0_C190[] __asm__("cFogParticleMan_mSortKey");
extern "C" void func_0037D968(void* self);

static inline sV3_C190 operator*(const sV3_C190& a, float s)
{
    return sV3_C190(a.x * s, a.y * s, a.z * s);
}

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline sV4_C190 mulMatVec_C190(const sMat_C190* m, const sV4_C190& v)
{
    sV4_C190 r;
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
static inline sV4_C190 sub_C190(const sV4_C190& a, const sV4_C190& b)
{
    sV4_C190 r;
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
static inline sV4_C190 add4_C190(const sV4_C190& a, const sV4_C190& b)
{
    sV4_C190 r;
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

static inline int outcode_C190(const sCtx_C190* g, const sV4_C190& v)
{
    int code = 0;
    if (v.x < g->clipMin.x)
        code |= 2;
    if (g->clipMax.x < v.x)
        code |= 1;
    if (v.y < g->clipMin.y)
        code |= 8;
    if (g->clipMax.y < v.y)
        code |= 4;
    if (v.w < 0.0f) {
        code ^= 0x2F;
    } else {
        if (g->clipMin.z < v.z)
            code |= 0x20;
        if (v.z < g->clipMax.z)
            code |= 0x10;
    }
    return code;
}

/**
 * Adds fog sprites for a list of fog-sphere instances.
 *
 * Uses cGraphicsMan_TheGraphicsMan (refreshing its matrices with func_0037D968 when +0x6B90 is 0). For each instance it
 * projects the centre of its bounding box (+0x68, +0x74) with the view matrix; at a depth of 25000 or more the instance
 * is skipped, beyond 18000 it fades out linearly. Each point of each part of its mesh (+0x64) is projected with view *
 * instance matrix (+0x10); points at a depth of 500 or less, or behind, are skipped, and the fade ramps in up to 2500.
 * Each remaining point gets a sort key in cFogParticleMan_mSortKey (index and depth) and a sprite in mSprites: corners from the
 * point's size (+0x18) scaled by +0x6AF0 / +0x6B04, colour alpha * 128 then the point's colour * 128. With @p clip set,
 * a sprite whose two corners lie outside the same clip edge is dropped. miNumSphere counts the sprites; nothing checks
 * it against the 256 sprites of the manager. It is word 5 (slot 2) of D_004882E0.
 *
 * @param out  the fog-sprite manager
 * @param objs  the instances
 * @param n  how many
 * @param clip  nonzero to drop sprites outside the clip rectangle
 */
extern "C" void cPS2FogParticleMan_addFogInstance(cPS2FogParticleMan* out, sObj_C190** objs, int n, int clip)
{
    sCtx_C190* graphicsMan = D_C190;
    if (graphicsMan->ready == 0)
        func_0037D968(graphicsMan);
    for (int i = 0; i < n; i++) {
        sObj_C190* obj = objs[i];
        sV4_C190 viewCentre = mulMatVec_C190(&graphicsMan->view, sV4_C190(sV3_C190(obj->min.x + obj->max.x, obj->min.y + obj->max.y, obj->min.z + obj->max.z) * 0.5f, 1.0f));
        float centreDepth = (int)viewCentre.w;
        if (centreDepth >= 25000.0f)
            continue;
        float fade;
        if (centreDepth < 18000.0f)
            fade = 1.0f;
        else
            fade = (25000.0f - centreDepth) * 0.000142857141f;
        sMat_C190 modelView;
        modelView = sMat_C190(graphicsMan->view, obj->mat);
        sMesh_C190* mesh = obj->mesh;
        sList_C190* list = mesh->list;
        for (int j = 0; j < mesh->count; j++) {
            sPart_C190* part = list->part;
            if (part != 0 && part->count != 0) {
                sVert_C190* vertex = part->verts;
                for (int k = 0; k < part->count; k++, vertex++) {
                    sV4_C190 viewPos = mulMatVec_C190(&modelView, sV4_C190(vertex->pos.x, vertex->pos.y, vertex->pos.z, 1.0f));
                    int depth = (int)viewPos.w;
                    if (depth > 0) {
                        float fd = depth;
                        float nearFade;
                        if (fd <= 500.0f)
                            nearFade = 0.0f;
                        else if (fd < 2499.66748046875f)
                            nearFade = (fd - 500.0f) * 0.00050008314f;
                        else
                            nearFade = 1.0f;
                        float alpha = fade * nearFade;
                        if (alpha != 0.0f) {
                            viewPos.w = 1.0f / viewPos.w;
                            unsigned short id = out->miNumSphere;
                            D_00537AC0_C190[id].idx = id;
                            D_00537AC0_C190[id].depth = depth;
                            viewPos.x *= viewPos.w;
                            viewPos.y *= viewPos.w;
                            float spriteSize = vertex->size * viewPos.w;
                            sV4_C190 off(graphicsMan->sx * spriteSize, graphicsMan->sy * spriteSize);
                            (*(sV4_C190*)&((&out->mSprites[0]))[out->miNumSphere].mTopLeft) = sub_C190(viewPos, off);
                            (*(sV4_C190*)&((&out->mSprites[0]))[out->miNumSphere].mBottomRight) = add4_C190(viewPos, off);
                            if (clip != 0) {
                                if (outcode_C190(graphicsMan, (*(sV4_C190*)&((&out->mSprites[0]))[out->miNumSphere].mTopLeft)) & outcode_C190(graphicsMan, (*(sV4_C190*)&((&out->mSprites[0]))[out->miNumSphere].mBottomRight)))
                                    continue;
                            }
                            ((&out->mSprites[0]))[out->miNumSphere].mColour[0] = alpha * 128.0f;
                            ((&out->mSprites[0]))[out->miNumSphere].mColour[1] = vertex->col[0] * 128.0f;
                            ((&out->mSprites[0]))[out->miNumSphere].mColour[2] = vertex->col[1] * 128.0f;
                            ((&out->mSprites[0]))[out->miNumSphere].mColour[3] = vertex->col[2] * 128.0f;
                            out->miNumSphere++;
                        }
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1DCD10", cPS2FogParticleMan_platformrender);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sDma_C7B0 {
    char pad[0x10];
    int size : 28;
};
struct sRS_C7B0 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};
struct sEnt_C7B0 {
    sRS_C7B0 rs;
    unsigned int buf;
    unsigned int next;
    short key;
    short flag;
    char pad_0x20[0x60];
};
struct sRing_C7B0 {
    int count;
    char pad_0x4[0x7C];
    sEnt_C7B0 ents[1];
};
struct sGfx_C7B0 {
    char pad_0x0[0xE84];
    sRS_C7B0* top;
    char pad_0xE88[0x18F0 - 0xE88];
    sRing_C7B0* ring;
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;
};
struct sList_C7B0 {
    int count;
    int pad[3];
    tFogSprite v[1];
};
struct sSort_C7B0 { int idx; int key; };

// PORT: asm-label aliases; these globals and calls are declared with other types elsewhere.
// GS packet words are 64-bit `unsigned long`, and uncached (0x30000000) pointers are held in int.
extern sDma_C7B0* D_004A4474_C7B0 __asm__("D_004A4474");
extern sGfx_C7B0* D_004A5B80_C7B0 __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" unsigned long* func_0038F460_C7B0(sDma_C7B0* dma, unsigned int buf, int a2, int a3) __asm__("func_0038F460");
extern "C" unsigned int func_0038F668_C7B0(sDma_C7B0* dma, unsigned long* p, int a2) __asm__("func_0038F668");

static inline int FBits_C7B0(float f) { return *(int*)&f; }

/**
 * Draws the queued fog sprites as GS sprite primitives, far to near, in DMA chunks.
 *
 * Called through slot 3 of the cPS2FogParticleMan vtable (D_004882E0) by cFogParticleMan_render after it has sorted the
 * sort keys in cFogParticleMan_mSortKey. Each chunk holds at most (D_004A4474 buffer size - 4) * 2 / 5 - 1 sprites. For a chunk it
 * opens a packet at the graphics manager's current buffer address (+0x5A00) with func_0038F460, writes a DMA cnt tag, a
 * VIF DIRECT code, a GIF tag that sets PRIM to 0x56 (textured, alpha-blended sprite) and a REGLIST GIF tag of 5
 * registers (ST, RGBAQ, XYZ2, ST, XYZ2) looped once per sprite, then five words per sprite taken in mSortKey order from
 * mSprites: ST (0,0) and (1,1) scaled by the sprite's mTopLeft.w (its 1/w, also the Q of RGBAQ), the colour from
 * mColour (alpha in mColour[0]), and the two corners from mTopLeft and mBottomRight with z scaled by 1/w. An odd sprite
 * count gets one zero filler word.
 *
 * func_0038F668 closes the packet; when the graphics manager's render ring (+0x18F0) holds fewer than 2600 entries a
 * new 128-byte entry is appended (written through the uncached 0x30000000 alias) with a copy of the top render state
 * (+0xE84, its low 5 bits of word 2 replaced from ring +0x69CC4), the packet start and end and the sort key at ring
 * +0x69CC0. The buffer address then moves to the end + 0x10. Does nothing when miNumSphere is 0 or less.
 *
 * @param list  the fog particle manager whose miNumSphere sprites are drawn
 */
extern "C" void cPS2FogParticleMan_platformrender(cPS2FogParticleMan* list)
{
    int left = list->miNumSphere;
    int start = 0;
    int max = (D_004A4474_C7B0->size - 4) * 2 / 5 - 1;
    sGfx_C7B0* graphicsMan = D_004A5B80_C7B0;
    if (left <= 0) return;
    float one = 1.0f;
    do {
        int n = max;
        unsigned int buf = graphicsMan->bufAddr;
        if (left < n) n = left;
        unsigned long* packet = func_0038F460_C7B0(D_004A4474_C7B0, buf, -1, 0);
        int oddCount;
        int qwc;
        float st0[2];
        float st1[2];
        st0[0] = 0.0f;
        st0[1] = 0.0f;
        st1[0] = one;
        st1[1] = one;
        if ((n & 1) == 0) {
            oddCount = 0;
            qwc = (n * 5 >> 1) + 3;
        } else {
            oddCount = 1;
            qwc = (n * 5 >> 1) + 4;
        }
        packet[0] = (qwc + 1) | 0x10000000;
        packet[1] = 0;
        packet[2] = 0;
        packet[3] = (unsigned long)(qwc | 0x50000000) << 32;
        packet[4] = ((unsigned long)0x14000000 << 32) | 0x8001;
        packet[5] = 0;
        packet[6] = 86;
        packet[7] = 0;
        packet[8] = (n | 0x8000) | ((unsigned long)43008 << 47);
        packet[9] = 0x52512;
        int rest = left - n;
        packet += 10;
        for (int i = start; i < start + n; i++) {
            tFogSprite* fogSprite = &((&list->mSprites[0]))[((sSort_C7B0*)cFogParticleMan_mSortKey)[i].idx];
            float invW = fogSprite->mTopLeft.w;
            packet[0] = FBits_C7B0(st0[0] * invW) | ((unsigned long)FBits_C7B0(st0[1] * invW) << 32);
            packet[1] = (int)fogSprite->mColour[1] | ((unsigned long)(int)fogSprite->mColour[2] << 8) | ((unsigned long)(int)fogSprite->mColour[3] << 16) | ((unsigned long)(int)fogSprite->mColour[0] << 24) | ((unsigned long)FBits_C7B0(invW) << 32);
            packet[2] = (int)fogSprite->mTopLeft.x | ((unsigned long)(int)fogSprite->mTopLeft.y << 16) | ((unsigned long)(int)(fogSprite->mTopLeft.z * invW) << 32);
            packet[3] = FBits_C7B0(st1[0] * invW) | ((unsigned long)FBits_C7B0(st1[1] * invW) << 32);
            packet[4] = (int)fogSprite->mBottomRight.x | ((unsigned long)(int)fogSprite->mBottomRight.y << 16) | ((unsigned long)(int)(fogSprite->mBottomRight.z * invW) << 32);
            packet += 5;
        }
        if (oddCount) *packet++ = 0;
        unsigned int next = func_0038F668_C7B0(D_004A4474_C7B0, packet, 4);
        sRing_C7B0* ring = graphicsMan->ring;
        sRS_C7B0* rs = graphicsMan->top;
        if (ring->count < 2600) {
            rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
            int key = *(int*)((char*)ring + 0x69CC0);
            sEnt_C7B0* ringEntry = (sEnt_C7B0*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
            ringEntry->rs = *rs;
            ringEntry->buf = buf;
            ringEntry->next = next;
            ringEntry->key = key;
            ringEntry->flag = 0;
        }
        graphicsMan->bufAddr = next + 0x10;
        left = rest;
        start += n;
    } while (left > 0);
}
#endif

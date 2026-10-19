#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cInstanceNode_getBoundBoxInfo(void* self, void* out);
extern const char D_0048E890[];

struct cFloatingNode {
    char pad_0x00[0x78];
    void* field_0x78;
};

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_initInfo__FP13cFloatingNode);
#ifdef SKIP_ASM
// PORT: the unit declares cInstanceNode_getBoundBoxInfo with 2 args; its body uses 3 ($6 is written).
void cInstanceNode_getBoundBoxInfo_3(void* self, void* box, void* out) __asm__("cInstanceNode_getBoundBoxInfo");

void cFloatingNode_initInfo(cFloatingNode* self)
{
    void* mem = cMemMan_alloc(0x30, D_0048E890, 0x20000000, 0);
    self->field_0x78 = mem;
    cInstanceNode_getBoundBoxInfo_3(self, mem, (char*)mem + 0x20);
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", func_0034EF08);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_0034EF08(cFloatingNode* self)
{
    if (self->field_0x78 != 0) {
        operator_delete((int*)self->field_0x78);
        self->field_0x78 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", func_0034EF40);
#ifdef SKIP_ASM
struct sBox_F048;
extern "C" void* func_002D1BE0();
extern "C" void func_003291E0(void* world, int type, void* id, sBox_F048* box, sBox_F048* old);
extern "C" void func_003568B0(void* self);

struct sFlVec4_34EF40 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFlBox_34EF40 {
    sFlVec4_34EF40 min;
    sFlVec4_34EF40 max;
};

struct sFlInfo_34EF40 {
    sFlBox_34EF40 box;
    float radius;
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline sFlVec4_34EF40 vu0Sub_34EF40(const sFlVec4_34EF40& a, const sFlVec4_34EF40& b)
{
    sFlVec4_34EF40 r;
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
static inline sFlVec4_34EF40 vu0Add_34EF40(const sFlVec4_34EF40& a, const sFlVec4_34EF40& b)
{
    sFlVec4_34EF40 r;
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

static inline sFlVec4_34EF40 nodePos_34EF40(char* self)
{
    sFlVEntry_34EF40* vt = *(sFlVEntry_34EF40**)(self + 0xC);
    return *(sFlVec4_34EF40*)(vt[24].fn(self + vt[24].delta) + 0x30);
}

extern "C" void func_0034EF40(char* self)
{
    sFlInfo_34EF40* info = *(sFlInfo_34EF40**)(self + 0x78);
    if (info != 0) {
        sFlBox_34EF40 nb;
        sFlBox_34EF40 old = info->box;
        float r = info->radius;
        sFlVec4_34EF40 ext;
        ext.x = r;
        ext.y = r;
        ext.z = r;
        ext.w = 0.0f;
        nb.min = vu0Sub_34EF40(nodePos_34EF40(self), ext);
        nb.max = vu0Add_34EF40(nodePos_34EF40(self), ext);
        (*(sFlInfo_34EF40**)(self + 0x78))->box = nb;
        void* id = *(void**)(self + 0x18);
        func_003291E0(func_002D1BE0(), 0, id, (sBox_F048*)&nb, (sBox_F048*)&old);
    } else {
        func_003568B0(self);
    }
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", func_0034F048);
#ifdef SKIP_ASM
struct sVec4_F048 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBox_F048 {
    sVec4_F048 min;
    sVec4_F048 max;
};

struct sInst_F048 {
    char pad_0x00[0x60];
    float min[3];   // 0x60
    float max[3];   // 0x6C
};

extern "C" void* func_002D1BE0();
extern "C" void func_003291E0(void* world, int type, void* id, sBox_F048* box, sBox_F048* old);
extern "C" void func_003567E0(void* self);

extern "C" void func_0034F048(cFloatingNode* self)
{
    sBox_F048* info = (sBox_F048*)self->field_0x78;
    if (info != 0) {
        sBox_F048 nb;
        sBox_F048 old = *info;
        sInst_F048* inst = *(sInst_F048**)((char*)self + 0x18);
        sVec4_F048 t;
        t.x = inst->min[0];
        t.y = inst->min[1];
        t.z = inst->min[2];
        t.w = 1.0f;
        nb.min = t;
        t.x = inst->max[0];
        t.y = inst->max[1];
        t.z = inst->max[2];
        nb.max = t;
        *info = nb;
        void* id = *(void**)((char*)self + 0x18);
        func_003291E0(func_002D1BE0(), 0, id, &nb, &old);
        func_0034EF08(self);
    } else {
        func_003567E0(self);
    }
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", func_0034F120);
#ifdef SKIP_ASM
extern "C" void func_0031BE50(float* s, float* c, float angle); // sincos

struct sQuad_F120 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_F120 {
    sQuad_F120 row[4];
};

struct sVec4_F120 {
    float x, y, z, w;
    sVec4_F120() {}
    sVec4_F120(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

// Bone: parent index (-1 for the root) and its local transform.
struct sBone_F120 {
    int parent;
    int pad4[2];
    sMat_F120* local;   // 0xC
};

struct sSkel_F120 {
    int pad0;
    int count;          // 0x4
    sBone_F120* bones;  // 0x8
};

// The tilt angles are read through a view of the spring block at +0x2C (see func_0034F930).
struct sFloatTilt_F120 {
    char pad0[0x28];
    float pos2;         // 0x54
    char pad2C[0x38 - 0x2C];
    float pos3;         // 0x64
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

static inline sQuad_F120 Row_F120(const sMat_F120* m, int k)
{
    return m->row[k];
}

static inline sVec4_F120 CompMul_F120(const sQuad_F120& a, const sVec4_F120& b)
{
    return sVec4_F120(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

// dst = parent * (local with its translation scaled by s).
static inline void Pose_F120(sMat_F120* dst, const sMat_F120* parent, const sMat_F120* local, const sVec4_F120& s)
{
    sMat_F120 l;
    l.row[0] = Row_F120(local, 0);
    l.row[1] = Row_F120(local, 1);
    l.row[2] = Row_F120(local, 2);
    sVec4_F120 v = CompMul_F120(Row_F120(local, 3), s);
    l.row[3] = *(sQuad_F120*)&v;
    sMat_F120 t;
    Mul_F120(&t, parent, &l);
    Copy_F120(dst, &t);
}

// Poses a floating object's skeleton: the root bob/tilt transform, then each bone under its parent.
extern "C" void func_0034F120(sFloatingNode_F120* self, sMat_F120* mat, sMat_F120* out)
{
    sMat_F120 m;
    sQuad_F120 base = mat->row[3];
    float pos[3];
    float rot[3];
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = self->pos1 - base.z;
    sFloatTilt_F120* tilt = (sFloatTilt_F120*)&self->k;
    rot[0] = tilt->pos2;
    rot[1] = tilt->pos3;
    rot[2] = 0.0f;
    // Rotation from the tilt angles (same construction as func_00351800), translated by pos.
    float sx, cx, sy, cy, sz, cz;
    func_0031BE50(&sx, &cx, rot[0]);
    func_0031BE50(&sy, &cy, rot[1]);
    func_0031BE50(&sz, &cz, rot[2]);
    float sxsy = sx * sy;
    float cxsy = cx * sy;
    float cxsz = cx * sz;
    m.row[0].x = cy * cz;
    m.row[0].y = -cy * sz;
    m.row[0].z = sy;
    m.row[0].w = 0.0f;
    m.row[1].x = sxsy * cz + cxsz;
    m.row[1].y = cx * cz - sxsy * sz;
    m.row[1].z = -sx * cy;
    m.row[1].w = 0.0f;
    m.row[2].x = sx * sz - cxsy * cz;
    m.row[2].y = cxsz * sy + sx * cz;
    m.row[2].z = cx * cy;
    m.row[2].w = 0.0f;
    m.row[3].x = pos[0];
    m.row[3].y = pos[1];
    m.row[3].z = pos[2];
    m.row[3].w = 1.0f;
    sSkel_F120* skel = *(sSkel_F120**)(self->info + 0x80);
    float sc4 = *(float*)(self->info + 0x84);
    sBone_F120* bone = skel->bones;
    sVec4_F120 scale(sc4, sc4, sc4, 1.0f);
    int i;
    for (i = 0; i < skel->count; i++, bone++) {
        sMat_F120 r;
        if (bone->parent != -1) {
            Pose_F120(&r, &out[bone->parent], bone->local, scale);
            Copy_F120(&out[i], &r);
        } else {
            sMat_F120 w;
            sMat_F120 t;
            Mul_F120(&t, mat, &m);
            Copy_F120(&w, &t);
            Pose_F120(&r, &w, bone->local, scale);
            Copy_F120(&out[i], &r);
        }
    }
    self->flags &= 0xFFFE;
}
#endif

INCLUDE_ASM("object/floatingnode", func_0034F600);

INCLUDE_ASM("object/floatingnode", func_0034F790);

//100%
INCLUDE_ASM("object/floatingnode", func_0034F930);
#ifdef SKIP_ASM
extern char* D_004A5B64;

// The tilt clamp reads the angles through a view of the spring block at +0x2C.
struct sFloatTilt_34F930 {
    char pad0[0x28];
    float pos2;         // 0x54
    char pad2C[0x38 - 0x2C];
    float pos3;         // 0x64
};

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

// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// Steps the float spring (bob height plus two tilt axes) by one frame.
extern "C" int func_0034F930(sFloatingNode_34F930* self)
{
    float dt = 1.0f / (float)*(int*)(D_004A5B64 + 0x10);
    float a1 = -self->k * (self->pos1 - self->rest1) + self->gain * self->in1 + -self->damp * self->vel1;
    float a2 = -self->k2 * self->pos2 + self->gain2 * self->in2 + -self->damp2 * self->vel2;
    float a3 = -self->k2 * self->pos3 + self->gain2 * self->in3 + -self->damp2 * self->vel3;
    self->pos1 += self->vel1 * dt;
    self->vel1 += a1 * dt;
    self->pos2 += self->vel2 * dt;
    self->vel2 += a2 * dt;
    self->pos3 += self->vel3 * dt;
    self->vel3 += a3 * dt;
    sFloatTilt_34F930* t = (sFloatTilt_34F930*)&self->k;
    float lo = -1.3962635f;    // +-80 degrees
    float hi = 1.3962635f;
    self->pos2 = t->pos2 >= lo ? t->pos2 <? hi : lo;
    self->pos3 = t->pos3 >= lo ? t->pos3 <? hi : lo;
    self->in1 = 0.0f;
    self->in2 = 0.0f;
    self->in3 = 0.0f;
    self->flags |= 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", func_0034FA88);
#ifdef SKIP_ASM
struct sFloatingVEntryFA88 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00356B30(void* self, void* stream);

extern "C" void func_0034FA88(cFloatingNode* self, void* stream)
{
    int has;
    func_00356B30(self, stream);
    has = self->field_0x78 != 0;
    sFloatingVEntryFA88* e = &(*(sFloatingVEntryFA88**)stream)[1];
    e->fn((char*)stream + e->delta, &has, 4);
    e = &(*(sFloatingVEntryFA88**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x2C, 0x48);
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", func_0034FB00);
#ifdef SKIP_ASM
extern "C" void* func_00354850(void* self, void* a1, int type);
extern "C" void func_0034FC78(void* self, void* node);
extern "C" void func_002D1BF0(void* node);
extern char D_00491C80[];

extern "C" void* func_0034FB00(void* self, void* a1, int type, void* node)
{
    func_00354850(self, a1, type);
    *(void**)((char*)self + 0x18) = node;
    *(void**)((char*)self + 0xC) = D_00491C80;
    func_0034FC78(self, node);
    if ((*(int*)((char*)node + 0x8) & 0x100) == 0) {
        if (type != 6 && type != 0x10 && type != 0x16) {
            func_002D1BF0(node);
        }
    }
    return self;
}
#endif


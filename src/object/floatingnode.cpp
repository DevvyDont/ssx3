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

INCLUDE_ASM("object/floatingnode", func_0034F120);

INCLUDE_ASM("object/floatingnode", func_0034F600);

INCLUDE_ASM("object/floatingnode", func_0034F790);

INCLUDE_ASM("object/floatingnode", func_0034F930);

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


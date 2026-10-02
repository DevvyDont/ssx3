#include "common.h"

//100%
INCLUDE_ASM("object/flexrailnode", cFlexRailNode_addSpaceHash);
#ifdef SKIP_ASM
struct sHashKey_8B90 {
    int level;      // 0x0
    int x;          // 0x4
    int y;          // 0x8
    int z;          // 0xC
};

struct sHashCell_8B90 {
    char data[0x14];
};

struct sHashNode_8B90 {
    int field_0x0;
    int field_0x4;
    int type;       // 0x8
    void* owner;    // 0xC
};

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002D1BE0();
extern "C" void func_00328F28(sHashKey_8B90* out, void* box);
extern "C" void func_00328C20(sHashCell_8B90* cell, int type, sHashNode_8B90* node, sHashKey_8B90* key);
extern const char D_0048E808[];

extern "C" void cFlexRailNode_addSpaceHash(void* self)
{
    sHashNode_8B90* n = (sHashNode_8B90*)cMemMan_alloc(0x10, D_0048E808, 0x20000000, 0);
    n->owner = self;
    n->type = 3;
    *(sHashNode_8B90**)((char*)self + 0x54) = n;
    sHashCell_8B90* cell = (sHashCell_8B90*)func_002D1BE0();
    sHashNode_8B90* node = *(sHashNode_8B90**)((char*)self + 0x54);
    sHashKey_8B90 key;
    func_00328F28(&key, (char*)self + 0x30);
    sHashCell_8B90* c1 = key.x < 0 ? cell + 4 : cell;
    sHashCell_8B90* c2 = key.y < 0 ? c1 + 2 : c1;
    sHashCell_8B90* c3 = key.z < 0 ? c2 + 1 : c2;
    func_00328C20(c3, 2, node, &key);
}
#endif

INCLUDE_ASM("object/flexrailnode", func_00348C48);

INCLUDE_ASM("object/flexrailnode", func_00348D98);

INCLUDE_ASM("object/flexrailnode", func_00348FA0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flexrailnode", func_003490B0);
#ifdef SKIP_ASM
extern char D_0048FF30[];
void cMemMan_free(void*);
extern "C" void func_00348C48(void* self);
extern "C" void func_00347F90(void* self, int flags);

extern "C" void func_003490B0(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = D_0048FF30;
    func_00348C48(self);
    void* p = *(void**)((char*)self + 0x70);
    if (p != 0) {
        cMemMan_free(p);
    }
    func_00347F90(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_00349110);
#ifdef SKIP_ASM
struct sFrBoxVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFrBoxElem {
    char pad_0x00[0x20];
    sFrBoxVec4 pos; // 0x20
    char pad_0x30[0x20];
};

struct sFrBoxRail {
    char pad_0x00[0x20];
    int count;          // 0x20
    char pad_0x24[0xC];
    sFrBoxVec4 min;     // 0x30
    sFrBoxVec4 max;     // 0x40
    char pad_0x50[0x20];
    sFrBoxElem* elems;  // 0x70
};

// PORT: PS2-only VU0 inline asm (vector sub-assign).
static inline void vu0SubFRBox(sFrBoxVec4& dst, sFrBoxVec4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddFRBox(sFrBoxVec4& dst, sFrBoxVec4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// bounding box of the rail elements, padded by 200 on x/y/z
extern "C" void func_00349110(sFrBoxRail* self)
{
    sFrBoxElem* e = self->elems;
    int n = self->count;
    self->min = self->max = e[0].pos;
    for (int i = 1; i < n; i++) {
        sFrBoxVec4* p = &e[i].pos;
        if (p->x < self->min.x) {
            self->min.x = p->x;
        }
        if (p->y < self->min.y) {
            self->min.y = p->y;
        }
        if (p->z < self->min.z) {
            self->min.z = p->z;
        }
        if (p->x > self->max.x) {
            self->max.x = p->x;
        }
        if (p->y > self->max.y) {
            self->max.y = p->y;
        }
        if (p->z > self->max.z) {
            self->max.z = p->z;
        }
    }
    sFrBoxVec4 pad;
    pad.x = 200.0f;
    pad.y = 200.0f;
    pad.z = 200.0f;
    pad.w = 0.0f;
    vu0SubFRBox(self->min, pad);
    vu0AddFRBox(self->max, pad);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_00349220);
#ifdef SKIP_ASM
struct sFrNodeVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFrNodeMtx {
    sFrNodeVec4 r[4];
};

struct sFrModelSet9220 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C, (model >> 2) << 8 | low byte
};

struct sFrWorld9220 {
    char pad_0x0[0x8];
    sFrModelSet9220** sets; // 0x8
};

extern "C" sFrWorld9220** func_002D1BD8();
extern "C" void func_0034FED8(void* model, int bone, sFrNodeMtx* out);

static inline void* refToPtr9220(unsigned int p)
{
    return (void*)(p << 2);
}

struct sFrModelRef9220 {
    unsigned int id;

    void* get()
    {
        sFrModelSet9220* set = (*func_002D1BD8())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        unsigned int p = set->refs[id >> 8] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtr9220(p);
    }
};

struct sFrNode9220 {
    char pad_0x0[0x20];
    sFrNodeVec4 posA;       // 0x20
    sFrNodeVec4 posB;       // 0x30
    sFrModelRef9220 model;  // 0x40
    int bone;               // 0x44
};

extern "C" void func_00349220(void* self, sFrNode9220* node)
{
    sFrNodeMtx m;
    func_0034FED8(node->model.get(), node->bone, &m);
    sFrNodeVec4 v = m.r[3];
    node->posB = v;
    node->posA = v;
}
#endif

INCLUDE_ASM("object/flexrailnode", func_003492A8);

//100%
INCLUDE_ASM("object/flexrailnode", func_00349798);
#ifdef SKIP_ASM
struct sFrVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFrElem {
    sFrVec4 pos;   // 0x00
    sFrVec4 vel;   // 0x10
    char pad[0x30];
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sFrVec4 vu0ScaleFR(const sFrVec4& v, float s)
{
    sFrVec4 r;
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddFR(sFrVec4& dst, sFrVec4 b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

extern "C" void func_00349798(void* self, void* node, sFrVec4* v)
{
    if (*(int*)((char*)node + 0x64) >= 0) {
        vu0AddFR((*(sFrElem**)((char*)self + 0x70))[*(int*)((char*)node + 0x64)].pos,
                 vu0ScaleFR(*v, 1.0f - *(float*)((char*)node + 0x68)));
        vu0AddFR((*(sFrElem**)((char*)self + 0x70))[*(int*)((char*)node + 0x64) + 1].pos,
                 vu0ScaleFR(*v, *(float*)((char*)node + 0x68)));
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_00349840);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector times scalar).
// PORT: PS2-only VU0 inline asm (vector add-assign).
extern "C" void func_00349840(void* self, void* node)
{
    if (*(int*)((char*)node + 0x64) >= 0) {
        vu0AddFR(*(sFrVec4*)((char*)node + 0x20),
                 vu0ScaleFR((*(sFrElem**)((char*)self + 0x70))[*(int*)((char*)node + 0x64)].vel,
                            *(float*)((char*)node + 0x68)));
        vu0AddFR(*(sFrVec4*)((char*)node + 0x20),
                 vu0ScaleFR((*(sFrElem**)((char*)self + 0x70))[*(int*)((char*)node + 0x64) + 1].vel,
                            1.0f - *(float*)((char*)node + 0x68)));
    }
}
#endif

INCLUDE_ASM("object/flexrailnode", func_003498E8);

//100%
INCLUDE_ASM("object/flexrailnode", func_00349AD0);
#ifdef SKIP_ASM
struct sFlexRailVEntry9AD0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00348B40(void* self, void* stream);

extern "C" void func_00349AD0(void* self, void* stream)
{
    func_00348B40(self, stream);
    sFlexRailVEntry9AD0* e = &(*(sFlexRailVEntry9AD0**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x60, 0xC);
    e = &(*(sFlexRailVEntry9AD0**)stream)[1];
    e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x70), *(int*)((char*)self + 0x20) * 0x50);
}
#endif

INCLUDE_ASM("object/flexrailnode", func_00349B48);

INCLUDE_ASM("object/flexrailnode", func_00349DB0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flexrailnode", func_00349EB8);
#ifdef SKIP_ASM
extern char D_0048FD90[];
void cMemMan_free(void*);
extern "C" void func_00348C48(void* self);
extern "C" void func_00347F90(void* self, int flags);

extern "C" void func_00349EB8(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = D_0048FD90;
    func_00348C48(self);
    void* p = *(void**)((char*)self + 0x70);
    if (p != 0) {
        cMemMan_free(p);
    }
    func_00347F90(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_00349F18);
#ifdef SKIP_ASM
struct sFrBoxElem60 {
    sFrBoxVec4 pos; // 0x00
    char pad_0x10[0x50];
};

struct sFrBoxRail60 {
    char pad_0x00[0x20];
    int count;           // 0x20
    char pad_0x24[0xC];
    sFrBoxVec4 min;      // 0x30
    sFrBoxVec4 max;      // 0x40
    char pad_0x50[0x20];
    sFrBoxElem60* elems; // 0x70
};

// PORT: PS2-only VU0 inline asm (vector sub-assign).
// PORT: PS2-only VU0 inline asm (vector add-assign).
// bounding box of the rail elements, padded by 200 on x/y/z
extern "C" void func_00349F18(sFrBoxRail60* self)
{
    sFrBoxElem60* e = self->elems;
    int n = self->count;
    self->min = self->max = e[0].pos;
    for (int i = 1; i < n; i++) {
        sFrBoxVec4* p = &e[i].pos;
        if (p->x < self->min.x) {
            self->min.x = p->x;
        }
        if (p->y < self->min.y) {
            self->min.y = p->y;
        }
        if (p->z < self->min.z) {
            self->min.z = p->z;
        }
        if (p->x > self->max.x) {
            self->max.x = p->x;
        }
        if (p->y > self->max.y) {
            self->max.y = p->y;
        }
        if (p->z > self->max.z) {
            self->max.z = p->z;
        }
    }
    sFrBoxVec4 pad;
    pad.x = 200.0f;
    pad.y = 200.0f;
    pad.z = 200.0f;
    pad.w = 0.0f;
    vu0SubFRBox(self->min, pad);
    vu0AddFRBox(self->max, pad);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034A028);
#ifdef SKIP_ASM
extern "C" sFrWorld9220** func_002D1BD8();
extern "C" void func_0034FED8(void* model, int bone, sFrNodeMtx* out);

struct sFrNodeA028 {
    sFrNodeVec4 pos;        // 0x0
    char pad_0x10[0x40];
    sFrModelRef9220 model;  // 0x50
    int bone;               // 0x54
};

extern "C" void func_0034A028(void* self, sFrNodeA028* node)
{
    sFrNodeMtx m;
    func_0034FED8(node->model.get(), node->bone, &m);
    sFrNodeVec4 v = m.r[3];
    node->pos = v;
}
#endif

INCLUDE_ASM("object/flexrailnode", func_0034A0B0);

INCLUDE_ASM("object/flexrailnode", func_0034A568);

INCLUDE_ASM("object/flexrailnode", func_0034A838);

INCLUDE_ASM("object/flexrailnode", func_0034A8C8);

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AC10);
#ifdef SKIP_ASM
struct sFlexRailVEntryAC10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00348B40(void* self, void* stream);

extern "C" void func_0034AC10(void* self, void* stream)
{
    func_00348B40(self, stream);
    sFlexRailVEntryAC10* e = &(*(sFlexRailVEntryAC10**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x60, 0xC);
    e = &(*(sFlexRailVEntryAC10**)stream)[1];
    e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x70), *(int*)((char*)self + 0x20) * 0x60);
}
#endif

INCLUDE_ASM("object/flexrailnode", func_0034AC88);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flexrailnode", func_0034ADD8);
#ifdef SKIP_ASM
extern "C" void* func_0034FB00(void* self, void* a1, int type, void* a3);
extern "C" void* func_002D1CB0(void);
extern char D_0048FC10[];

struct sFrRailDesc {
    int data[0x58 / 4];
};

extern "C" sFrRailDesc* func_0034AC88(sFrRailDesc* out, void* src);
extern "C" void func_0034C548(void* mgr, void* node, sFrRailDesc* desc);

extern "C" void* func_0034ADD8(void* self, void* a1, void* node, void* src)
{
    sFrRailDesc desc;
    func_0034FB00(self, a1, 0xA, node);
    *(void**)((char*)self + 0xC) = D_0048FC10;
    *(unsigned int*)((char*)node + 0x8) = (*(unsigned int*)((char*)node + 0x8) & 0xFFFFFFFD) | 4;
    func_0034AC88(&desc, src);
    func_0034C548(func_002D1CB0(), node, &desc);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AE68);
#ifdef SKIP_ASM
extern "C" void cInstanceNode_cInstanceNode(void* self);
extern char D_0048FC10[];

extern "C" void* func_0034AE68(void* self)
{
    cInstanceNode_cInstanceNode(self);
    *(void**)((char*)self + 0xC) = D_0048FC10;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AEA0);
#ifdef SKIP_ASM
extern char D_0048FC10[];
extern "C" void* func_002D1CB0(void);
extern "C" void func_0034C600(void* set, void* arg);
extern "C" void func_0034FBF0(void* self, int flags);

extern "C" void func_0034AEA0(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = D_0048FC10;
    func_0034C600(func_002D1CB0(), *(void**)((char*)self + 0x18));
    func_0034FBF0(self, flags);
}
#endif

extern "C" void* func_0034FE90(void* self);

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AEF8__FPv);
#ifdef SKIP_ASM
void* func_0034AEF8(void* self)
{
    return func_0034FE90(self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AF18__FPv);
#ifdef SKIP_ASM
void func_0034AF18(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AF20);
#ifdef SKIP_ASM
extern "C" void* func_0034AF20(void* self)
{
    *(int*)((char*)self + 0x5c) = 0;
    *(unsigned int*)((char*)self + 0x58) = 0xFFFFFFFFU;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AF38);
#ifdef SKIP_ASM
extern int D_004A4010;
extern void* D_0048FBF8[];

struct sFlexRail_AF38 {
    char pad_0x0[0x60];
    int f60;            // 0x60
    char pad_0x64[0x10];
    int f74;            // 0x74
    char pad_0x78[0x18];
    int f90;            // 0x90
    int slots[60];      // 0x94
    void** vt;          // 0x184
};

extern "C" void* func_0034AF38(void* self)
{
    sFlexRail_AF38* s = (sFlexRail_AF38*)self;
    int i;
    func_0034AF20(self);
    s->f60 = 0;
    s->f74 = 0;
    s->vt = D_0048FBF8;
    for (i = 0; i < 60; i++) {
        s->slots[i] = 0;
    }
    int id = D_004A4010;
    s->f90 = id;
    D_004A4010 = id + 1;
    if (D_004A4010 >= 2) {
        D_004A4010 = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AFB8);
#ifdef SKIP_ASM
extern void* D_0048FBF8[];
void operator_delete(int*);

extern "C" void func_0034AFB8(void* self, int flags)
{
    *(void***)((char*)self + 0x184) = D_0048FBF8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", func_0034AFE8);
#ifdef SKIP_ASM
extern "C" int func_00415FC8(const void* a, const void* b, int n);

extern "C" int func_0034AFE8(void* self, void* data, void* ctx)
{
    if (*(int*)((char*)self + 0x5C) == 0) {
        return 0;
    }
    if (**(int**)((char*)ctx + 0x80) != *(int*)((char*)self + 0x58)) {
        return 0;
    }
    return func_00415FC8(data, self, 0x58) == 0;
}
#endif

INCLUDE_ASM("object/flexrailnode", func_0034B038);

//100%
INCLUDE_ASM("object/flexrailnode", func_0034B168);
#ifdef SKIP_ASM
struct sFrRider_B168 {
    char pad_0x0[0x78];
    int id;                 // 0x78
    char pad_0x7c[0x4];
    int* track;             // 0x80
};

struct sFrRail_B168 {
    char pad_0x0[0x58];
    int track;              // 0x58
    int count;              // 0x5C
    char pad_0x60[0x34];
    sFrRider_B168* riders[1]; // 0x94
};

extern "C" void func_0034B7B8(void* self);

extern "C" int func_0034B168(sFrRail_B168* self, sFrRider_B168* rider)
{
    if (self->count == 0) {
        return 0;
    }
    if (*rider->track != self->track) {
        return 0;
    }
    for (int i = 0; i < self->count; i++) {
        if (self->riders[i]->id == rider->id) {
            self->riders[i] = self->riders[self->count - 1];
            self->riders[self->count - 1] = 0;
            if (--self->count == 0) {
                func_0034B7B8(self);
            }
            return 1;
        }
    }
    return 0;
}
#endif


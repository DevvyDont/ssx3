#include "common.h"

INCLUDE_ASM("object/flexrailnode", cFlexRailNode_addSpaceHash);

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

INCLUDE_ASM("object/flexrailnode", func_00349220);

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

INCLUDE_ASM("object/flexrailnode", func_00349AD0);

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

INCLUDE_ASM("object/flexrailnode", func_0034A028);

INCLUDE_ASM("object/flexrailnode", func_0034A0B0);

INCLUDE_ASM("object/flexrailnode", func_0034A568);

INCLUDE_ASM("object/flexrailnode", func_0034A838);

INCLUDE_ASM("object/flexrailnode", func_0034A8C8);

INCLUDE_ASM("object/flexrailnode", func_0034AC10);

INCLUDE_ASM("object/flexrailnode", func_0034AC88);

INCLUDE_ASM("object/flexrailnode", func_0034ADD8);

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

INCLUDE_ASM("object/flexrailnode", func_0034AF38);

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

INCLUDE_ASM("object/flexrailnode", func_0034B168);


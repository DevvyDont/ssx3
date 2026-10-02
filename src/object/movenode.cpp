#include "common.h"

INCLUDE_ASM("object/movenode", cMoveNode_cMoveNode);

INCLUDE_ASM("object/movenode", func_003553C0);

//100%
INCLUDE_ASM("object/movenode", func_00355420);
#ifdef SKIP_ASM
struct sMoveNodeVEntryI {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_00355420(void* self)
{
    void* block = *(void**)((char*)self + 0x1C);
    if (block == 0) {
        return 1;
    }
    void* node = *(void**)block;
    if (node == 0) {
        return 1;
    }
    sMoveNodeVEntryI* vt = *(sMoveNodeVEntryI**)node;
    return vt[8].fn((char*)node + vt[8].delta);
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
struct tModifierBlock;
tModifierBlock* tModifierBlock_tModifierBlock(tModifierBlock* self);
extern const char D_0048E9D8[];

struct cMoveNode {
    char pad_0x00[0x1C];
    void* field_0x1C;
};

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addModifierBlock__FP9cMoveNode);
#ifdef SKIP_ASM
void cMoveNode_addModifierBlock(cMoveNode* self)
{
    void* mem = cMemMan_alloc(0x28, D_0048E9D8, 0x20000000, 0);
    self->field_0x1C = tModifierBlock_tModifierBlock((tModifierBlock*)mem);
}
#endif

INCLUDE_ASM("object/movenode", func_003554B0);

INCLUDE_ASM("object/movenode", func_00355550);

INCLUDE_ASM("object/movenode", func_003555A8);

INCLUDE_ASM("object/movenode", func_00355600);

void cEffectLink_add(void* link, void* other);

//99.74%
INCLUDE_ASM("object/movenode", cMoveNode_addEffectModifier__FP9cMoveNodePv);
#ifdef SKIP_ASM
void cMoveNode_addEffectModifier(cMoveNode* self, void* effect)
{
    if (self->field_0x1C == 0) {
        cMoveNode_addModifierBlock(self);
    }
    cEffectLink_add((char*)self->field_0x1C + 0x10, effect);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_003556A8);
#ifdef SKIP_ASM
extern "C" void func_0035B670(void* list, void* item);

extern "C" void func_003556A8(cMoveNode* self, void* item)
{
    if (self->field_0x1C == 0) {
        cMoveNode_addModifierBlock(self);
    }
    func_0035B670((char*)self->field_0x1C + 0x1C, item);
}
#endif

INCLUDE_ASM("object/movenode", func_003556F8);

//100%
INCLUDE_ASM("object/movenode", func_00355748);
#ifdef SKIP_ASM
extern "C" void func_00352D20(void*);

extern "C" void func_00355748(cMoveNode* self)
{
    if (self->field_0x1C != 0) {
        func_00352D20(self->field_0x1C);
    }
}
#endif

INCLUDE_ASM("object/movenode", func_00355770);

INCLUDE_ASM("object/movenode", func_00355858);

//100%
INCLUDE_ASM("object/movenode", func_00355878__FPvT0);
#ifdef SKIP_ASM
float func_00355878(void* self, void* a1)
{
    float t0 = *(float*)((char*)a1 + 0x4);
    *(float*)((char*)self + 0x24) = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00355888);
#ifdef SKIP_ASM
struct sMnVEntryF {
    short delta;
    short index;
    void (*fn)(void*, float);
};

extern "C" void func_00355888(void* self, void* a1)
{
    sMnVEntryF* vt = *(sMnVEntryF**)((char*)self + 0xC);
    vt[41].fn((char*)self + vt[41].delta, *(float*)((char*)a1 + 0x4));
}
#endif

INCLUDE_ASM("object/movenode", func_003558B8);

INCLUDE_ASM("object/movenode", func_00355918);

INCLUDE_ASM("object/movenode", func_00355978);

INCLUDE_ASM("object/movenode", func_003559F8);

INCLUDE_ASM("object/movenode", func_00355A78);

INCLUDE_ASM("object/movenode", func_00355AD0);

INCLUDE_ASM("object/movenode", cMoveNode_addSpline);

INCLUDE_ASM("object/movenode", func_00355B90);

INCLUDE_ASM("object/movenode", func_00355BF0);

INCLUDE_ASM("object/movenode", func_00355C50);

INCLUDE_ASM("object/movenode", cMoveNode_addParticle);

INCLUDE_ASM("object/movenode", cMoveNode_addDynamicParticle);

INCLUDE_ASM("object/movenode", func_00355DB8);

INCLUDE_ASM("object/movenode", func_00355E38);

INCLUDE_ASM("object/movenode", cMoveNode_addHalo);

INCLUDE_ASM("object/movenode", func_00355F10);

INCLUDE_ASM("object/movenode", func_00356020);

INCLUDE_ASM("object/movenode", func_00356078);

INCLUDE_ASM("object/movenode", func_003560C0);

INCLUDE_ASM("object/movenode", func_00356128);

INCLUDE_ASM("object/movenode", func_00356198);

INCLUDE_ASM("object/movenode", func_00356298);

INCLUDE_ASM("object/movenode", func_00356608);

INCLUDE_ASM("object/movenode", cMoveNode_setupOverlapSystem);

INCLUDE_ASM("object/movenode", func_003567E0);

INCLUDE_ASM("object/movenode", func_003568B0);

//100%
INCLUDE_ASM("object/movenode", func_003569D0);
#ifdef SKIP_ASM
extern "C" int func_00352B88(void*);

extern "C" int func_003569D0(cMoveNode* self)
{
    if (self->field_0x1C != 0) {
        return func_00352B88(self->field_0x1C) != 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356A00);
#ifdef SKIP_ASM
extern "C" int func_00352B88(void*);

extern "C" int func_00356A00(cMoveNode* self)
{
    if (self->field_0x1C != 0) {
        return func_00352B88(self->field_0x1C);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356A28);
#ifdef SKIP_ASM
struct sMoveNodeVEntryA28 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_00356A28(void* self)
{
    void* block = *(void**)((char*)self + 0x1C);
    if (block == 0) {
        return 0;
    }
    void* node = *(void**)block;
    if (node == 0) {
        return 0;
    }
    sMoveNodeVEntryA28* vt = *(sMoveNodeVEntryA28**)node;
    return vt[20].fn((char*)node + vt[20].delta);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356A70);
#ifdef SKIP_ASM
struct sMoveNodeVEntryA70 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int, int);
};

extern "C" int func_00356A70(void* self, int a, int b, int c, int d)
{
    void* block = *(void**)((char*)self + 0x1C);
    if (block == 0) {
        return 0;
    }
    void* node = *(void**)block;
    if (node == 0) {
        return 0;
    }
    sMoveNodeVEntryA70* vt = *(sMoveNodeVEntryA70**)node;
    return vt[21].fn((char*)node + vt[21].delta, a, b, c, d);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356AB8);
#ifdef SKIP_ASM
extern "C" void func_00353020(void*);

extern "C" void func_00356AB8(cMoveNode* self)
{
    if (self->field_0x1C != 0) {
        func_00353020(self->field_0x1C);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356AE0);
#ifdef SKIP_ASM
extern "C" void func_00353098(void*);

extern "C" void func_00356AE0(cMoveNode* self)
{
    if (self->field_0x1C != 0) {
        func_00353098(self->field_0x1C);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356B08);
#ifdef SKIP_ASM
extern "C" void func_00352F40(void*);

extern "C" void func_00356B08(cMoveNode* self)
{
    if (self->field_0x1C != 0) {
        func_00352F40(self->field_0x1C);
    }
}
#endif

INCLUDE_ASM("object/movenode", func_00356B30);

INCLUDE_ASM("object/movenode", func_00356BF0);

INCLUDE_ASM("object/movenode", func_00356C48);

INCLUDE_ASM("object/movenode", func_00356CC8);

INCLUDE_ASM("object/movenode", func_00356D48);

INCLUDE_ASM("object/movenode", func_00356DB0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00356E60);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode(void* self);
extern void* D_00490E80[];

extern "C" void* func_00356E60(void* self)
{
    cMoveNode_cMoveNode(self);
    *(void***)((char*)self + 0xC) = D_00490E80;
    return self;
}
#endif

extern "C" void* func_00356B30(void* self);

//99.29%
INCLUDE_ASM("object/movenode", func_00356E98__FPv);
#ifdef SKIP_ASM
void* func_00356E98(void* self)
{
    return func_00356B30(self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356EB8);
#ifdef SKIP_ASM
struct sMoveQuad {
    float v[4];
} __attribute__((aligned(16)));

extern void* D_0048F5F0[];
extern sMoveQuad D_004FF1A0[];

extern "C" void* func_00356EB8(void* self, sMoveQuad* pos)
{
    *(void***)self = D_0048F5F0;
    *(sMoveQuad*)((char*)self + 0x30) = *pos;
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy, D_004FF1A0 -> self+0x50).
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
        : "r"((char*)self + 0x50), "r"(D_004FF1A0)
        : "memory");
    *(int*)((char*)self + 0x44) = 1;
    *(sMoveQuad*)((char*)self + 0x80) = *(sMoveQuad*)((char*)self + 0x30);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356F10);
#ifdef SKIP_ASM
struct sMoveQuad2 {
    float v[4];
} __attribute__((aligned(16)));

extern void* D_0048F5F0[];

extern "C" void* func_00356F10(void* self, void* src)
{
    *(void***)self = D_0048F5F0;
    sMoveQuad2 v = *(sMoveQuad2*)((char*)src + 0x30);
    *(sMoveQuad2*)((char*)self + 0x30) = v;
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy, src -> self+0x50).
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
        : "r"((char*)self + 0x50), "r"(src)
        : "memory");
    *(int*)((char*)self + 0x44) = 1;
    return self;
}
#endif

INCLUDE_ASM("object/movenode", func_00356F68);

typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("object/movenode", func_00356FF0);
#ifdef SKIP_ASM
extern "C" void func_00356FF0(void* self)
{
    cQuad128 v = *(cQuad128*)((char*)self + 0x30);
    *(int*)((char*)self + 0x44) = 0;
    *(cQuad128*)((char*)self + 0x80) = v;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357000);
#ifdef SKIP_ASM
class func_00357000_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void func_00357000(void* self, func_00357000_cObj* obj)
{
    obj->v01((char*)self + 0x10, 0x40);
}
#endif

INCLUDE_ASM("object/movenode", func_00357038);

INCLUDE_ASM("object/movenode", func_00357090);

INCLUDE_ASM("object/movenode", func_00357108);

INCLUDE_ASM("object/movenode", func_00357210);

INCLUDE_ASM("object/movenode", func_00357278);

INCLUDE_ASM("object/movenode", func_003572E0);

INCLUDE_ASM("object/movenode", func_00357358);

INCLUDE_ASM("object/movenode", func_003573F8);

//100%
INCLUDE_ASM("object/movenode", func_00357528);
#ifdef SKIP_ASM
extern "C" void func_00357528(void* self)
{
    cQuad128 v = *(cQuad128*)((char*)self + 0x10);
    *(int*)((char*)self + 0x50) = 0;
    *(cQuad128*)((char*)self + 0x90) = v;
}
#endif

INCLUDE_ASM("object/movenode", func_00357538);

INCLUDE_ASM("object/movenode", func_00357660);

INCLUDE_ASM("object/movenode", func_003576D0);

//100%
INCLUDE_ASM("object/movenode", func_00357750);
#ifdef SKIP_ASM
extern void* D_0048F338[];

// PORT: PS2-only VU0 inline asm (4x4 matrix copy, src+0x10 -> self+0x40); the PC
// port needs a plain 64-byte copy.
extern "C" void* func_00357750(void* self, void* src)
{
    *(void***)self = D_0048F338;
    *(void**)((char*)self + 0x80) = src;
    *(int*)((char*)self + 0x30) = 0;
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
        : "r"((char*)self + 0x40), "r"((char*)src + 0x10)
        : "memory");
    return self;
}
#endif

INCLUDE_ASM("object/movenode", func_00357798);

//100%
INCLUDE_ASM("object/movenode", func_00357820);
#ifdef SKIP_ASM
// PORT: the project names this func_002D1CF0__FPv (one void* arg), but it is a
// pass-through wrapper and this caller passes two args (prototype mismatch).
void* func_002D1CF0_2(void*, void*) __asm__("func_002D1CF0__FPv");

extern "C" void func_00357820(void* self)
{
    func_002D1CF0_2(*(void**)((char*)self + 0x80), (char*)self + 0x40);
}
#endif

INCLUDE_ASM("object/movenode", func_00357848);

INCLUDE_ASM("object/movenode", func_003578A8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00357918);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode(void* self);
extern void* D_0048EE60[];

extern "C" void* func_00357918(void* self)
{
    cMoveNode_cMoveNode(self);
    *(void***)((char*)self + 0xC) = D_0048EE60;
    return self;
}
#endif

INCLUDE_ASM("object/movenode", func_00357950);

//99.29%
INCLUDE_ASM("object/movenode", func_003579A8__FPv);
#ifdef SKIP_ASM
void* func_003579A8(void* self)
{
    return func_00356B30(self);
}
#endif

INCLUDE_ASM("object/movenode", func_003579C8);

INCLUDE_ASM("object/movenode", func_00357A00);

INCLUDE_ASM("object/movenode", func_00357A78);

extern "C" void* func_003581B8(int, int);

//100%
INCLUDE_ASM("object/movenode", func_00357AE8__FPvii);
#ifdef SKIP_ASM
void* func_00357AE8(void* self, int a1, int a2)
{
    return func_003581B8(*(int*)((char*)self + (a1 << 2)), a2);
}
#endif

extern "C" void* func_003581F0(int, int);

//100%
INCLUDE_ASM("object/movenode", func_00357B10__FPvii);
#ifdef SKIP_ASM
void* func_00357B10(void* self, int a1, int a2)
{
    return func_003581F0(*(int*)((char*)self + (a1 << 2)), a2);
}
#endif

INCLUDE_ASM("object/movenode", func_00357B38);

INCLUDE_ASM("object/movenode", func_00357B90);

INCLUDE_ASM("object/movenode", func_00357BF8);

INCLUDE_ASM("object/movenode", func_00357C50);

INCLUDE_ASM("object/movenode", func_00357CA8);

INCLUDE_ASM("object/movenode", func_00357D28);

INCLUDE_ASM("object/movenode", func_00357DD8);

INCLUDE_ASM("object/movenode", func_00357E80);

INCLUDE_ASM("object/movenode", func_00357FA0);


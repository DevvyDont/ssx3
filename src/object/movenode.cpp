#include "common.h"

INCLUDE_ASM("object/movenode", cMoveNode_cMoveNode);

//100%
INCLUDE_ASM("object/movenode", func_003553C0);
#ifdef SKIP_ASM
extern char D_00491028[];
extern "C" void func_003567E0(void* self);
extern "C" void func_00352AE8(void* block, int flags);
extern "C" void func_0034FBF0(void* self, int flags);

extern "C" void func_003553C0(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = D_00491028;
    func_003567E0(self);
    void* block = *(void**)((char*)self + 0x1C);
    if (block != 0) {
        func_00352AE8(block, 3);
    }
    func_0034FBF0(self, flags);
}
#endif

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

//100%
INCLUDE_ASM("object/movenode", func_00355550);
#ifdef SKIP_ASM
extern "C" void func_00353118(void* block);

extern "C" void func_00355550(cMoveNode* self, void* v)
{
    if (self->field_0x1C == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        func_00353118(self->field_0x1C);
    }
    *(void**)((char*)self->field_0x1C + 0x4) = v;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_003555A8);
#ifdef SKIP_ASM
extern "C" void func_00353150(void* block);

extern "C" void func_003555A8(cMoveNode* self, void* v)
{
    if (self->field_0x1C == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        func_00353150(self->field_0x1C);
    }
    *(void**)((char*)self->field_0x1C + 0x8) = v;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00355600);
#ifdef SKIP_ASM
extern "C" void func_00353188(void* block);

extern "C" void func_00355600(cMoveNode* self, void* v)
{
    if (self->field_0x1C == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        func_00353188(self->field_0x1C);
    }
    *(void**)((char*)self->field_0x1C + 0xC) = v;
}
#endif

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

//100%
INCLUDE_ASM("object/movenode", func_003556F8);
#ifdef SKIP_ASM
extern "C" void func_00352C70(void* block);
extern "C" int func_00352E50(void* block);

extern "C" void func_003556F8(cMoveNode* self)
{
    if (self->field_0x1C != 0) {
        func_00352C70(self->field_0x1C);
        if (func_00352E50(self->field_0x1C) != 0) {
            *(unsigned short*)((char*)self + 0x12) |= 1;
        }
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_003558B8);
#ifdef SKIP_ASM
struct sMoveQuad;
extern char D_0048E8F0[];
extern "C" void* func_00356EB8(void* self, sMoveQuad* pos);
extern "C" void func_003554B0(cMoveNode* self, void* mod);

extern "C" void func_003558B8(cMoveNode* self, sMoveQuad* pos)
{
    func_003554B0(self, func_00356EB8(cMemMan_alloc(0x90, D_0048E8F0, 0x20000000, 0), pos));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00355918);
#ifdef SKIP_ASM
extern char D_0048E8F0[];
extern "C" void* func_00356F10(void* mem, void* desc);
extern "C" void func_003554B0(cMoveNode* self, void* mod);

extern "C" void func_00355918(cMoveNode* self, void* desc)
{
    func_003554B0(self, func_00356F10(cMemMan_alloc(0x90, D_0048E8F0, 0x20000000, 0), desc));
}
#endif

INCLUDE_ASM("object/movenode", func_00355978);

INCLUDE_ASM("object/movenode", func_003559F8);

INCLUDE_ASM("object/movenode", func_00355A78);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00355AD0);
#ifdef SKIP_ASM
extern char D_0048E8D0[];
extern "C" void* func_00359460(void* mem, void* desc);
extern "C" void func_003554B0(cMoveNode* self, void* mod);

extern "C" void func_00355AD0(cMoveNode* self, void* desc)
{
    func_003554B0(self, func_00359460(cMemMan_alloc(0xF0, D_0048E8D0, 0x20000000, 0), desc));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addSpline);
#ifdef SKIP_ASM
extern char D_0048E9E8[];
extern "C" void* func_00359F88(void* mem, void* desc, void* owner);
extern "C" void func_003554B0(cMoveNode* self, void* mod);

extern "C" void cMoveNode_addSpline(cMoveNode* self, void* desc)
{
    func_003554B0(self, func_00359F88(cMemMan_alloc(0x58, D_0048E9E8, 0x20000000, 0), desc,
                                      *(void**)((char*)self + 0x18)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00355B90);
#ifdef SKIP_ASM
extern char D_0048E940[];
extern "C" void* func_0035F6E8(void* mem, void* desc);
extern "C" void func_00355550(cMoveNode* self, void* mod);

extern "C" void func_00355B90(cMoveNode* self, void* desc)
{
    func_00355550(self, func_0035F6E8(cMemMan_alloc(0x50, D_0048E940, 0x20000000, 0), desc));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00355BF0);
#ifdef SKIP_ASM
extern char D_0048E958[];
extern "C" void* func_0035F0B8(void* mem, void* desc, void* owner);
extern "C" void func_003555A8(cMoveNode* self, void* mod);

extern "C" void func_00355BF0(cMoveNode* self, void* desc)
{
    func_003555A8(self, func_0035F0B8(cMemMan_alloc(0xE8, D_0048E958, 0x20000000, 0), desc, *(void**)((char*)self + 0x18)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00355C50);
#ifdef SKIP_ASM
extern char D_0048E968[];
extern "C" void* func_00342768(void* mem, void* desc, void* owner);
extern "C" void func_00355600(cMoveNode* self, void* mod);

extern "C" void func_00355C50(cMoveNode* self, void* desc)
{
    func_00355600(self, func_00342768(cMemMan_alloc(0x50, D_0048E968, 0x20000000, 0), desc, *(void**)((char*)self + 0x18)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addParticle);
#ifdef SKIP_ASM
extern char D_0048E978[];
extern "C" void* func_003458C0(void* mem, void* desc, void* owner);
void cMoveNode_addEffectModifier(cMoveNode* self, void* mod);

extern "C" void cMoveNode_addParticle(cMoveNode* self, void* desc)
{
    cMoveNode_addEffectModifier(self, func_003458C0(cMemMan_alloc(0x1F0, D_0048E978, 0x20000000, 0), desc,
                                                    *(void**)((char*)self + 0x18)));
}
#endif

INCLUDE_ASM("object/movenode", cMoveNode_addDynamicParticle);

INCLUDE_ASM("object/movenode", func_00355DB8);

INCLUDE_ASM("object/movenode", func_00355E38);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addHalo);
#ifdef SKIP_ASM
extern char D_0048E9A8[];
extern "C" void* func_00346120(void* mem, void* desc, void* owner);
void cMoveNode_addEffectModifier(cMoveNode* self, void* mod);

extern "C" void cMoveNode_addHalo(cMoveNode* self, void* desc)
{
    cMoveNode_addEffectModifier(self, func_00346120(cMemMan_alloc(0x40, D_0048E9A8, 0x20000000, 0), desc,
                                                    *(void**)((char*)self + 0x18)));
}
#endif

INCLUDE_ASM("object/movenode", func_00355F10);

//100%
INCLUDE_ASM("object/movenode", func_00356020);
#ifdef SKIP_ASM
extern "C" void func_00353398(void* block, void* other);

extern "C" void func_00356020(cMoveNode* self, cMoveNode* other)
{
    if (other->field_0x1C != 0) {
        if (self->field_0x1C == 0) {
            cMoveNode_addModifierBlock(self);
        }
        func_00353398(self->field_0x1C, (char*)other->field_0x1C + 0x10);
    }
}
#endif

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

//100%
INCLUDE_ASM("object/movenode", func_00356D48);
#ifdef SKIP_ASM
extern "C" int func_00356D48(void* self)
{
    void* block = *(void**)((char*)self + 0x1C);
    if (block == 0) {
        return 0;
    }
    void* node = *(void**)block;
    if (node == 0) {
        return 0;
    }
    sMoveNodeVEntryI* vt = *(sMoveNodeVEntryI**)node;
    if (vt[26].fn((char*)node + vt[26].delta) != 2) {
        return 0;
    }
    return *(int*)((char*)**(void***)((char*)self + 0x1C) + 0x34);
}
#endif

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

//100%
INCLUDE_ASM("object/movenode", func_00357038);
#ifdef SKIP_ASM
extern char D_0048F508[];

extern "C" void* func_00357038(void* self, void* a1, void* a2, sMoveQuad* pos)
{
    func_00356EB8(self, pos);
    *(void**)((char*)self + 0x0) = D_0048F508;
    *(void**)((char*)self + 0xA0) = a1;
    *(void**)((char*)self + 0x90) = a2;
    return self;
}
#endif

INCLUDE_ASM("object/movenode", func_00357090);

INCLUDE_ASM("object/movenode", func_00357108);

//100%
INCLUDE_ASM("object/movenode", func_00357210);
#ifdef SKIP_ASM
struct sMnVEntry7210 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void func_00357210(void* self, void* obj)
{
    void* p = *(void**)((char*)self + 0xA0);
    if (p == 0) {
        return;
    }
    void* q = *(void**)((char*)p + 0xC);
    if (q == 0) {
        return;
    }
    int saved = *(int*)((char*)obj + 0x5C);
    *(int*)((char*)obj + 0x5C) = *(int*)((char*)self + 0x90);
    sMnVEntry7210* vt = *(sMnVEntry7210**)((char*)q + 0xC);
    vt[42].fn((char*)q + vt[42].delta, obj);
    *(int*)((char*)obj + 0x5C) = saved;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00357278);
#ifdef SKIP_ASM
struct sMnVEntry7278a {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sMnVEntry7278b {
    short delta;
    short index;
    void (*fn)(void*, void*);
};


extern "C" void func_00357278(void* self, void* stream)
{
    func_00357000(self, (func_00357000_cObj*)stream);
    sMnVEntry7278a* vt1 = *(sMnVEntry7278a**)stream;
    vt1[1].fn((char*)stream + vt1[1].delta, (char*)self + 0x90, 4);
    sMnVEntry7278b* vt2 = *(sMnVEntry7278b**)stream;
    vt2[5].fn((char*)stream + vt2[5].delta, *(void**)((char*)self + 0xA0));
}
#endif

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

//100%
INCLUDE_ASM("object/movenode", func_00357848);
#ifdef SKIP_ASM
struct sMnVEntry7848a {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sMnVEntry7848b {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00357848(void* self, void* stream)
{
    sMnVEntry7848a* vt1 = *(sMnVEntry7848a**)stream;
    vt1[5].fn((char*)stream + vt1[5].delta, *(void**)((char*)self + 0x80));
    sMnVEntry7848b* vt2 = *(sMnVEntry7848b**)stream;
    vt2[1].fn((char*)stream + vt2[1].delta, (char*)self + 0x10, 0x30);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00357950);
#ifdef SKIP_ASM
struct sMnDtorVEntry7950 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00356198(void* self);

extern "C" void func_00357950(void* self)
{
    func_00356198(self);
    void* mb = *(void**)((char*)self + 0x1C);
    if (mb != 0 && *(int*)((char*)mb + 0x10) != 0) {
        return;
    }
    if (self != 0) {
        sMnDtorVEntry7950* vt = *(sMnDtorVEntry7950**)((char*)self + 0xC);
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

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

//100%
INCLUDE_ASM("object/movenode", func_00357B38);
#ifdef SKIP_ASM
extern "C" void func_00357FA0(void* p, int flags);

extern "C" void func_00357B38(void** self)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            func_00357FA0(self[i], 3);
        }
        self[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357B90);
#ifdef SKIP_ASM
extern "C" void func_00358140(void* p, void* arg);

extern "C" void func_00357B90(void** self, void* arg)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            func_00358140(self[i], arg);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357BF8);
#ifdef SKIP_ASM
extern "C" void func_00358120(void* p);

extern "C" void func_00357BF8(void** self)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            func_00358120(self[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357C50);
#ifdef SKIP_ASM
extern "C" void func_00358260(void* p);

extern "C" void func_00357C50(void** self)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            func_00358260(self[i]);
        }
    }
}
#endif

INCLUDE_ASM("object/movenode", func_00357CA8);

INCLUDE_ASM("object/movenode", func_00357D28);

INCLUDE_ASM("object/movenode", func_00357DD8);

INCLUDE_ASM("object/movenode", func_00357E80);

INCLUDE_ASM("object/movenode", func_00357FA0);


#include "common.h"

struct sBoundBoxTarget {
    char pad_0x00[0x68];
    short field_0x68;
    char pad_0x6A[2];
    void (*fnBoundBox)(void*); // 0x6C
    char pad_0x70[0x78 - 0x70];
    short field_0x78;
    char pad_0x7A[2];
    void (*fnRadius)(void*); // 0x7C
};

struct sBoundBoxNode {
    sBoundBoxTarget* target; // 0x0
};

struct tModifierBlock {
    sBoundBoxNode* node; // 0x0
    int field_0x4;
    int field_0x8;
    int field_0xC;
    int field_0x10;
    int field_0x14;
    void* field_0x18;
    int field_0x1C;
    int field_0x20;
    void* field_0x24;
};

extern void* D_00491340[16];
extern void* D_00491200[16];

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_tModifierBlock__FP14tModifierBlock);
#ifdef SKIP_ASM
tModifierBlock* tModifierBlock_tModifierBlock(tModifierBlock* self)
{
    self->field_0x18 = D_00491340;
    self->field_0x24 = D_00491200;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x1C = 0;
    self->field_0x20 = 0;
    self->node = 0;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->field_0xC = 0;
    return self;
}
#endif

INCLUDE_ASM("object/modifierblock", func_00352AE8);

//100%
INCLUDE_ASM("object/modifierblock", func_00352B88);
#ifdef SKIP_ASM
struct sMbVEntryI {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_00352B88(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryI* vt = *(sMbVEntryI**)node;
        return vt[12].fn((char*)node + vt[12].delta);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_setBoundBox__FP14tModifierBlock);
#ifdef SKIP_ASM
struct sMbVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

// PORT: the symbol's mangling says one parameter, but the body passes $5 through
// to the virtual call untouched, so the real function takes a second argument.
void tModifierBlock_setBoundBox_impl(tModifierBlock* self, void* box) __asm__("tModifierBlock_setBoundBox__FP14tModifierBlock");

void tModifierBlock_setBoundBox_impl(tModifierBlock* self, void* box)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntry* vt = *(sMbVEntry**)node;
        vt[13].fn((char*)node + vt[13].delta, box);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00352BF8);
#ifdef SKIP_ASM
struct sMbVEntryF {
    short delta;
    short index;
    float (*fn)(void*);
};

extern "C" float func_00352BF8(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryF* vt = *(sMbVEntryF**)node;
        return vt[14].fn((char*)node + vt[14].delta);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_setRadius__FP14tModifierBlock);
#ifdef SKIP_ASM
void tModifierBlock_setRadius(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sBoundBoxTarget* target = node->target;
        target->fnRadius((char*)node + target->field_0x78);
    }
}
#endif

INCLUDE_ASM("object/modifierblock", func_00352C70);

INCLUDE_ASM("object/modifierblock", func_00352D20);

INCLUDE_ASM("object/modifierblock", func_00352DD0);

INCLUDE_ASM("object/modifierblock", func_00352E50);

//100%
INCLUDE_ASM("object/modifierblock", func_00352ED0);
#ifdef SKIP_ASM
extern "C" int func_00352ED0(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryI* vt = *(sMbVEntryI**)node;
        return vt[6].fn((char*)node + vt[6].delta);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00352F08);
#ifdef SKIP_ASM
struct sMbVEntryV {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00352F08(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryV* vt = *(sMbVEntryV**)node;
        vt[7].fn((char*)node + vt[7].delta);
    }
}
#endif

INCLUDE_ASM("object/modifierblock", func_00352F40);

INCLUDE_ASM("object/modifierblock", func_00353020);

//100%
INCLUDE_ASM("object/modifierblock", func_00353098);
#ifdef SKIP_ASM
extern "C" void func_00353098(tModifierBlock* self, void* arg)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntry* vt = *(sMbVEntry**)node;
        vt[22].fn((char*)node + vt[22].delta, arg);
    }
}
#endif

INCLUDE_ASM("object/modifierblock", func_003530D0);

//100%
INCLUDE_ASM("object/modifierblock", func_00353118);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00353118(tModifierBlock* self)
{
    int** p = (int**)((char*)self + 0x4);
    if (*p != 0) {
        operator_delete(*p);
        *p = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353150);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00353150(tModifierBlock* self)
{
    int** p = (int**)((char*)self + 0x8);
    if (*p != 0) {
        operator_delete(*p);
        *p = 0;
    }
}
#endif

INCLUDE_ASM("object/modifierblock", func_00353188);

INCLUDE_ASM("object/modifierblock", func_003531D0);

INCLUDE_ASM("object/modifierblock", func_00353228);

INCLUDE_ASM("object/modifierblock", func_00353278);

INCLUDE_ASM("object/modifierblock", func_00353300);

INCLUDE_ASM("object/modifierblock", func_00353398);

//100%
INCLUDE_ASM("object/modifierblock", func_00353418);
#ifdef SKIP_ASM
struct sModifierLink {
    sModifierLink* next; // 0x0
    char pad_0x4[0x30 - 0x4];
    int id;              // 0x30
};

extern "C" int func_00353418(void* self, int id)
{
    sModifierLink* p = *(sModifierLink**)((char*)self + 0x1c);
    while (p != 0) {
        if (p->id == id) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}
#endif

INCLUDE_ASM("object/modifierblock", func_00353448);

INCLUDE_ASM("object/modifierblock", tModifierBlock_readFromReplayFrame);

extern void* D_0048F008[];

//100%
INCLUDE_ASM("object/modifierblock", func_00353AC0__FPv);
#ifdef SKIP_ASM
void* func_00353AC0(void* self)
{
    *(void***)((char*)self + 0xc) = D_0048F008;
    *(int*)((char*)self + 0x10) = -1;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x14) = 0;
    return self;
}
#endif

extern "C" void* func_003546C8(void*);

//100%
INCLUDE_ASM("object/modifierblock", func_00353AE8__FPv);
#ifdef SKIP_ASM
void* func_00353AE8(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_0048F008;
    return func_003546C8(self);
}
#endif

INCLUDE_ASM("object/modifierblock", func_00353B10);

INCLUDE_ASM("object/modifierblock", func_00353CF0);

//100%
INCLUDE_ASM("object/modifierblock", func_00353D98__FPv);
#ifdef SKIP_ASM
int func_00353D98(void* self)
{
    int t0 = -1;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x10) = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353DA8__FPv);
#ifdef SKIP_ASM
void* func_00353DA8(void* self)
{
    *(int*)((char*)self + 0x24) = 0;
    return self;
}
#endif

INCLUDE_ASM("object/modifierblock", func_00353DB8);

INCLUDE_ASM("object/modifierblock", func_00353E08);

INCLUDE_ASM("object/modifierblock", func_00353E80);


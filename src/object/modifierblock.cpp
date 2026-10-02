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

//100%
INCLUDE_ASM("object/modifierblock", func_00352E50);
#ifdef SKIP_ASM
extern "C" int func_00352E50(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryI* vt = *(sMbVEntryI**)node;
        if (vt[17].fn((char*)node + vt[17].delta) != 0) {
            return 1;
        }
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("object/modifierblock", func_00353020);
#ifdef SKIP_ASM
struct sMbVEntryP3020 {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sMbVEntryVII3020 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern "C" void func_00353020(tModifierBlock* self, int a1, int a2)
{
    sBoundBoxNode* node = self->node;
    void* obj;
    if (node != 0) {
        sMbVEntryP3020* vt = *(sMbVEntryP3020**)node;
        obj = vt[25].fn((char*)node + vt[25].delta);
    } else {
        obj = 0;
    }
    if (obj != 0) {
        sMbVEntryVII3020* vt2 = *(sMbVEntryVII3020**)((char*)obj + 0xC);
        vt2[43].fn((char*)obj + vt2[43].delta, a1, a2);
    }
}
#endif

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

//100%
INCLUDE_ASM("object/modifierblock", func_003530D0);
#ifdef SKIP_ASM
struct sMbVEntryDel {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_003530D0(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryDel* vt = *(sMbVEntryDel**)node;
        vt[1].fn((char*)node + vt[1].delta, 3);
        self->node = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("object/modifierblock", func_00353188);
#ifdef SKIP_ASM
struct sMbVEntryDel2 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00353188(tModifierBlock* self)
{
    void* obj = *(void**)((char*)self + 0xC);
    if (obj != 0) {
        sMbVEntryDel2* vt = *(sMbVEntryDel2**)((char*)obj + 0x4);
        vt[1].fn((char*)obj + vt[1].delta, 3);
        *(void**)((char*)self + 0xC) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_003531D0);
#ifdef SKIP_ASM
struct sModBlockDtorVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sModBlockLink {
    sModBlockLink* next;          // 0x0
    int field_0x4;
    sModBlockDtorVEntry* vt;      // 0x8
};

extern "C" void func_003531D0(void* self)
{
    sModBlockLink* head = *(sModBlockLink**)((char*)self + 0x10);
    if (head != 0) {
        sModBlockLink* p = head;
        do {
            sModBlockLink* next = p->next;
            if (p != 0) {
                p->vt[1].fn((char*)p + p->vt[1].delta, 3);
            }
            p = next;
        } while (p != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353228);
#ifdef SKIP_ASM
struct sModBlockVEntry3228 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sModBlockLink3228 {
    sModBlockLink3228* next;      // 0x0
    int field_0x4;
    sModBlockVEntry3228* vt;      // 0x8
};

extern "C" void func_00353228(void* self)
{
    sModBlockLink3228* head = *(sModBlockLink3228**)((char*)self + 0x10);
    if (head != 0) {
        sModBlockLink3228* p = head;
        do {
            sModBlockLink3228* next = p->next;
            p->vt[3].fn((char*)p + p->vt[3].delta);
            p = next;
        } while (p != 0);
    }
}
#endif

INCLUDE_ASM("object/modifierblock", func_00353278);

INCLUDE_ASM("object/modifierblock", func_00353300);

//100%
INCLUDE_ASM("object/modifierblock", func_00353398);
#ifdef SKIP_ASM
struct sMbVEntryV3398 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00345720(void* pool, void* item);

extern "C" void func_00353398(void* self, void* list)
{
    void* p = *(void**)list;
    while (p != 0) {
        void* next = *(void**)p;
        sMbVEntryV3398* vt = *(sMbVEntryV3398**)((char*)p + 0x8);
        vt[3].fn((char*)p + vt[3].delta);
        func_00345720((char*)self + 0x10, p);
        p = next;
    }
    *(void**)list = 0;
    *(void**)((char*)list + 0x4) = 0;
}
#endif

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

//100%
INCLUDE_ASM("object/modifierblock", func_00353DB8);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);

extern "C" void func_00353DB8(void* self, int flags)
{
    void* p = *(void**)((char*)self + 0x24);
    if (p != 0) {
        cMemMan_free(p);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("object/modifierblock", func_00353E08);

//100%
INCLUDE_ASM("object/modifierblock", func_00353E80);
#ifdef SKIP_ASM
struct sMbVEntrySer3E80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cSpring_setupNodes(void* self);

extern "C" void* func_00353E80(void* self, void* stream)
{
    sMbVEntrySer3E80* e = &(*(sMbVEntrySer3E80**)stream)[2];
    e->fn((char*)stream + e->delta, self, 0x24);
    cSpring_setupNodes(self);
    e = &(*(sMbVEntrySer3E80**)stream)[2];
    e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x24), *(int*)self * 0x50);
    return self;
}
#endif


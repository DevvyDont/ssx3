#include "common.h"

//100%
INCLUDE_ASM("object/instancenode", cInstanceNode_cInstanceNode);
#ifdef SKIP_ASM
struct sInstNodeCtorVEntry {
    short delta;
    short index;
    int (*fn)(void*, void*);
};

extern "C" void* cSortObjNode_cSortObjNode(void* self, void* a1, void* stream);
extern char D_00491C80[];

extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream)
{
    cSortObjNode_cSortObjNode(self, a1, stream);
    *(void**)((char*)self + 0xC) = D_00491C80;
    sInstNodeCtorVEntry* vt = *(sInstNodeCtorVEntry**)stream;
    *(int*)((char*)self + 0x18) = vt[4].fn((char*)stream + vt[4].delta, self);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FBF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0034FC80(void* self, void* a1);
extern void* func_00354920_dtor(void* self, int flags) __asm__("func_00354920__FPv");

extern "C" void func_0034FBF0(void* self, int flags)
{
    *(void***)((char*)self + 0xc) = (void**)D_00491C80;
    func_0034FC80(self, *(void**)((char*)self + 0x18));
    *(int*)(*(char**)((char*)self + 0x18) + 8) &= 0xFFFF0300;
    {
        char* o = *(char**)((char*)self + 0x18);
        int v = *(int*)(o + 8);
        *(int*)(o + 8) = v | (v >> 16);
    }
    *(int*)(*(char**)((char*)self + 0x18) + 8) |= 2;
    func_00354920_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FC78);
#ifdef SKIP_ASM
extern "C" void func_0034FC78(int a0, void* a1)
{
    *(int*)((char*)a1 + 0xc) = a0;
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FC80);
#ifdef SKIP_ASM
extern "C" void func_0034FC80(void* self, void* a1)
{
    if (a1 != 0) {
        if (*(int*)((char*)a1 + 0xc) == (int)self) {
            *(int*)((char*)a1 + 0xc) = 0;
        }
    }
}
#endif

extern "C" void* func_002D19E8(int);

//100%
INCLUDE_ASM("object/instancenode", func_0034FCC0__FPv);
#ifdef SKIP_ASM
void* func_0034FCC0(void* self)
{
    return func_002D19E8(*(int*)((char*)self + 0x18));
}
#endif

extern "C" void* func_002D1A30(int);

//100%
INCLUDE_ASM("object/instancenode", func_0034FCE0__FPv);
#ifdef SKIP_ASM
void* func_0034FCE0(void* self)
{
    return func_002D1A30(*(int*)((char*)self + 0x18));
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FD00);
#ifdef SKIP_ASM
struct sInstNodeVEntryFD00 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0034FD00(void* self, int a1)
{
    if (a1 != 0) {
        if (func_0034FCC0(self) != 0) return;
    }
    *(int*)(*(char**)((char*)self + 0x18) + 8) &= 0xFFFF0300;
    {
        char* o = *(char**)((char*)self + 0x18);
        int v = *(int*)(o + 8);
        *(int*)(o + 8) = v | (v >> 16);
    }
    *(int*)(*(char**)((char*)self + 0x18) + 8) |= 2;
    if (self != 0) {
        sInstNodeVEntryFD00* vt = *(sInstNodeVEntryFD00**)((char*)self + 0xc);
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FD90);
#ifdef SKIP_ASM
struct sInstNodeVEntryFD90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0034FD90(void* self)
{
    int* f = (int*)(*(char**)((char*)self + 0x18) + 0x8);
    *f &= 0xFFFF0000;
    f = (int*)(*(char**)((char*)self + 0x18) + 0x8);
    *f |= *f >> 16;
    f = (int*)(*(char**)((char*)self + 0x18) + 0x8);
    *f |= 2;
    if (self != 0) {
        sInstNodeVEntryFD90* vt = *(sInstNodeVEntryFD90**)((char*)self + 0xC);
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

extern "C" void* func_002D19B8(int, int, int);

//100%
INCLUDE_ASM("object/instancenode", func_0034FE00__FPviii);
#ifdef SKIP_ASM
void* func_0034FE00(void* self, int a1, int a2, int a3)
{
    return func_002D19B8(a1, a2, a3);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FE28);
#ifdef SKIP_ASM
struct sInstNodeDtorVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char D_0048E738[];
extern "C" void* func_003506D8(void* mem, int v);

extern "C" void func_0034FE28(void* self)
{
    int v = *(int*)((char*)self + 0x18);
    if (self != 0) {
        sInstNodeDtorVEntry* vt = *(sInstNodeDtorVEntry**)((char*)self + 0xC);
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
    func_003506D8(cMemMan_alloc(0x1C, D_0048E738, 0x20000000, 0), v);
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_0034FE90);
#ifdef SKIP_ASM
struct sInstNodeVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void func_00354980(void* self, void* stream);

extern "C" void func_0034FE90(void* self, void* stream)
{
    func_00354980(self, stream);
    sInstNodeVEntry* vt = *(sInstNodeVEntry**)stream;
    vt[6].fn((char*)stream + vt[6].delta, self);
}
#endif

INCLUDE_ASM("object/instancenode", func_0034FED8);

INCLUDE_ASM("object/instancenode", func_00350288);

INCLUDE_ASM("object/instancenode", cInstanceNode_getBoundBoxInfo);

//100%
INCLUDE_ASM("object/instancenode", func_00350698);
#ifdef SKIP_ASM
struct sInstNodeEntry {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
};

struct sInstNodeList {
    int field_0x0;
    int count;                // 0x4
    sInstNodeEntry* entries;  // 0x8
};

extern "C" int func_00350698(void* self)
{
    sInstNodeList* list = *(sInstNodeList**)((char*)self + 0x80);
    int i;
    int count = list->count;
    sInstNodeEntry* e = list->entries;
    for (i = 0; i < count; i++, e++) {
        if (e->field_0x8 != 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/instancenode", func_003506D8);
#ifdef SKIP_ASM
extern "C" void* func_0034FB00(void* self, int a1, int type, int a3);
extern char D_00491B00[];

extern "C" void* func_003506D8(void* self, int v)
{
    func_0034FB00(self, 8, 6, v);
    *(void**)((char*)self + 0xC) = D_00491B00;
    unsigned int* f = (unsigned int*)(*(char**)((char*)self + 0x18) + 0x8);
    *f &= 0xFFFFFF9F;
    f = (unsigned int*)(*(char**)((char*)self + 0x18) + 0x8);
    *f = (*f & 0xFFFFFFFD) | 4;
    return self;
}
#endif


#include "common.h"

//100%
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char D_00483CB8[];
extern char D_00483CD0[];
extern char D_00483CE0[];
extern char D_00483CF0[];
extern char D_00483D00[];
extern char D_00483D10[];
extern char D_00483D20[];
extern char D_00483D38[];
extern char D_00483D50[];
extern char D_00483D60[];
extern char D_00483D70[];
extern char D_00483D80[];
extern char D_00483D90[];
extern char D_00483DA0[];
extern "C" void* func_002BC6F8(void* mem);
extern "C" void* func_002BC728(void* mem);
extern "C" void* func_002BC750(void* mem);
extern "C" void* func_002BC778(void* mem);
extern "C" void* func_002BC7A0(void* mem);
extern "C" void* func_002BC7C8(void* mem);
extern "C" void* func_002BC830(void* mem);
extern "C" void* func_002BC890(void* mem);
extern "C" void* func_002BC8E8(void* mem);
extern "C" void* func_002BC910(void* mem);
extern "C" void* func_002BC980(void* mem);
extern "C" void* func_002BC9B0(void* mem);
extern "C" void* func_002BCAF8(void* mem);
extern "C" void* func_002BCBD0(void* mem);
extern char D_00483E00[];

struct sWPQuery_0420 {
    void* impl;
    void* vtbl;
    float f8;
    float fC;
    unsigned lo : 8;
    unsigned type : 24;
    sWPQuery_0420* next;
};

extern sWPQuery_0420* D_004A3880_0420 __asm__("D_004A3880");

extern "C" sWPQuery_0420* cWorldPainterQuery_cWorldPainterQuery(sWPQuery_0420* self, unsigned int key)
{
    self->vtbl = D_00483E00;
    self->lo = key;
    self->type = key >> 8;
    self->f8 = 4286578688.0f;
    self->fC = 4286578688.0f;
    switch (self->type) {
    case 0:
        self->impl = func_002BC6F8(cMemMan_alloc(0x18, D_00483CB8, 0, 0));
        break;
    case 1:
        self->impl = func_002BC728(cMemMan_alloc(0x10, D_00483CD0, 0, 0));
        break;
    case 2:
        self->impl = func_002BC750(cMemMan_alloc(0x10, D_00483CE0, 0, 0));
        break;
    case 3:
        self->impl = func_002BC778(cMemMan_alloc(0x10, D_00483CF0, 0, 0));
        break;
    case 4:
        self->impl = func_002BC7A0(cMemMan_alloc(0x10, D_00483D00, 0, 0));
        break;
    case 5:
        self->impl = func_002BC7C8(cMemMan_alloc(0x38, D_00483D10, 0, 0));
        break;
    case 6:
        self->impl = func_002BC830(cMemMan_alloc(0x40, D_00483D20, 0, 0));
        break;
    case 7:
        self->impl = func_002BC890(cMemMan_alloc(0x38, D_00483D38, 0, 0));
        break;
    case 8:
        self->impl = func_002BC8E8(cMemMan_alloc(0x10, D_00483D50, 0, 0));
        break;
    case 9:
        self->impl = func_002BC910(cMemMan_alloc(0x50, D_00483D60, 0, 0));
        break;
    case 10:
        self->impl = func_002BC980(cMemMan_alloc(0x10, D_00483D70, 0, 0));
        break;
    case 11:
        self->impl = func_002BC9B0(cMemMan_alloc(0x58, D_00483D80, 0, 0));
        break;
    case 12:
        self->impl = func_002BCAF8(cMemMan_alloc(0xA0, D_00483D90, 0, 0));
        break;
    case 13:
        self->impl = func_002BCBD0(cMemMan_alloc(0x18, D_00483DA0, 0, 0));
        break;
    }
    self->next = D_004A3880_0420;
    D_004A3880_0420 = self;
    return self;
}

//100%
void operator_delete(int* p);
extern char D_00483E00[];
extern char D_00485D30[];

struct sWPQuery_06C0 {
    int* buf;
    void* vtbl;
    char pad8[0xC];
    sWPQuery_06C0* next;
};

extern sWPQuery_06C0* D_004A3880;

extern "C" void func_002C06C0(sWPQuery_06C0* self, int flags)
{
    self->vtbl = D_00483E00;
    operator_delete(self->buf);
    sWPQuery_06C0* p = D_004A3880;
    if (p == self) {
        D_004A3880 = self->next;
    } else {
        while (p->next != self) {
            p = p->next;
        }
        if (p != 0) {
            p->next = self->next;
        }
    }
    self->vtbl = D_00485D30;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}

INCLUDE_ASM("worldpainter/worldpainterqueryctor", func_002C0778);

//100%
struct func_002C0A10_sItem {
    int a;
    int b;
};

struct func_002C0A10_sList {
    int field_0x0;
    int field_0x4;
    func_002C0A10_sItem* items;
};

extern "C" int func_002BAF90(func_002C0A10_sList* list);

extern "C" func_002C0A10_sItem* func_002C0A10(void* self, func_002C0A10_sList* list)
{
    int i = func_002BAF90(list);
    return (i != -1) ? &list->items[i] : 0;
}


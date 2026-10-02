#include "common.h"

INCLUDE_ASM("worldpainter/worldpainterqueryctor", cWorldPainterQuery_cWorldPainterQuery);

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", func_002C06C0);
#ifdef SKIP_ASM
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
#endif

INCLUDE_ASM("worldpainter/worldpainterqueryctor", func_002C0778);

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", func_002C0A10);
#ifdef SKIP_ASM
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
#endif


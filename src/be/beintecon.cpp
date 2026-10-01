#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0045A718[];
extern void* D_0045AC78[16];
extern void* D_004A1214;

struct cBEEconInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.58%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getThis__Fv);
#ifdef SKIP_ASM
void* cBEEconInterface_getThis()
{
    if (D_004A1214 == 0) {
        cBEEconInterface* mem = (cBEEconInterface*)cMemMan_alloc(0x10, D_0045A718, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045AC78;
        D_004A1214 = mem;
    }
    return D_004A1214;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150928);
#ifdef SKIP_ASM
extern int D_004A6CA8[];

extern "C" int func_00150928(void* self, int a, int b)
{
    if (a >= 2) {
        return 0;
    }
    int bOff = b * 0xF88;
    int aOff = a * 0x9B50;
    int off = bOff + aOff;
    char* p = (char*)D_004A6CA8 + off;
    return *(int*)(p + 0xAC4);
}
#endif

INCLUDE_ASM("be/beintecon", func_00150960);

extern int D_004A6CA8[];

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getTotalMoneyEarned__FPvii);
#ifdef SKIP_ASM
int cBEEconInterface_getTotalMoneyEarned(void* self, int a, int b)
{
    if (a >= 2) {
        return 0;
    }
    int bOff = b * 0xF88;
    int aOff = a * 0x9B50;
    int off = bOff + aOff;
    char* p = (char*)D_004A6CA8 + off;
    return *(int*)(p + 0xAC8);
}
#endif

INCLUDE_ASM("be/beintecon", func_001509F8);

//100%
INCLUDE_ASM("be/beintecon", func_00150A90);
#ifdef SKIP_ASM
extern int D_004A6CA8[];

extern "C" int func_00150A90(void* self, int a, int b, int amount)
{
    if (a >= 2) {
        return 0;
    }
    int bOff = b * 0xF88;
    int aOff = a * 0x9B50;
    int off = bOff + aOff;
    char* p = (char*)D_004A6CA8 + off;
    *(int*)(p + 0xAC4) += amount;
    *(int*)(p + 0xAC8) += amount;
    return *(int*)(p + 0xAC4);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150B48);
#ifdef SKIP_ASM
extern int D_004A6CA8[];

extern "C" int func_00150B48(void* self, int a, int b, int amount)
{
    if (a >= 2) {
        return 0;
    }
    int bOff = b * 0xF88;
    int aOff = a * 0x9B50;
    int off = bOff + aOff;
    char* p = (char*)D_004A6CA8 + off;
    return *(int*)(p + 0xAC4) -= amount;
}
#endif

INCLUDE_ASM("be/beintecon", func_00150B88);

INCLUDE_ASM("be/beintecon", func_00150C20);

extern void* D_00440550[];

//100%
INCLUDE_ASM("be/beintecon", func_00150E50__FPvi);
#ifdef SKIP_ASM
struct sEconEntry {
    int key;
    int value;
};

struct sEconTable {
    sEconEntry entries[1];
};

int func_00150E50(void* self, int a1)
{
    return ((sEconTable*)D_00440550)->entries[a1].value;
}
#endif

INCLUDE_ASM("be/beintecon", func_00151040);

//100%
INCLUDE_ASM("be/beintecon", func_00151178);
#ifdef SKIP_ASM
extern "C" int func_00151178(void* self, int level)
{
    switch (level)
    {
    case 0:
    default:
        return 500;
    case 1:
        return 1000;
    case 2:
        return 2000;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001511B0);
#ifdef SKIP_ASM
extern "C" int func_001511B0(void* self, int level)
{
    switch (level)
    {
    case 1:
    default:
        return 2000;
    case 2:
        return 4000;
    case 3:
        return 6000;
    }
}
#endif

INCLUDE_ASM("be/beintecon", func_00151368);

INCLUDE_ASM("be/beintecon", func_001513B8);

//100%
INCLUDE_ASM("be/beintecon", func_001515B8);
#ifdef SKIP_ASM
struct sEconItem_001515B8
{
    short id;
    short flags;
};

struct sEconItems_001515B8
{
    char pad[0x28C];
    int count;
    sEconItem_001515B8 items[1];
};

extern "C" void func_001515B8(sEconItems_001515B8* self)
{
    int n = self->count;
    sEconItem_001515B8* p = self->items;
    for (int i = 0; i < n; i++, p++)
    {
        if (p->flags & 4)
            p->flags |= 0x10;
        else
            p->flags &= ~0x10;
    }
}
#endif

INCLUDE_ASM("be/beintecon", func_00151600);

INCLUDE_ASM("be/beintecon", func_001519E0);

INCLUDE_ASM("be/beintecon", func_00151A88);

//100%
INCLUDE_ASM("be/beintecon", func_00151C48);
#ifdef SKIP_ASM
struct sEconItem_00151C48
{
    short id;
    short flags;
};

struct sEconItems_00151C48
{
    char pad[0x288];
    short* lookup;
    int count;
    sEconItem_00151C48 items[1];
};

extern "C" void func_00151C48(sEconItems_00151C48* self, int id, int on)
{
    int idx = self->lookup[id];
    sEconItem_00151C48* p;
    if (idx >= 0)
        p = &self->items[idx];
    else
        p = 0;
    if (on)
        p->flags |= 2;
    else
        p->flags &= ~2;
}
#endif

INCLUDE_ASM("be/beintecon", func_00151C90);

INCLUDE_ASM("be/beintecon", func_00151EF0);

INCLUDE_ASM("be/beintecon", func_001520E8);

INCLUDE_ASM("be/beintecon", func_001521F0);


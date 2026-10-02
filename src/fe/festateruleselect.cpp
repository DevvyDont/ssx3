#include "common.h"

INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_setupMenu);

INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_onCreateScreen);

extern "C" void* func_0039E4C0(void* self);

//100%
INCLUDE_ASM("fe/festateruleselect", func_00191C48__FPv);
#ifdef SKIP_ASM
void* func_00191C48(void* self)
{
    return func_0039E4C0(self);
}
#endif

INCLUDE_ASM("fe/festateruleselect", func_00191C68);

//100%
INCLUDE_ASM("fe/festateruleselect", func_00191E08);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A13A0[];

extern "C" int func_00191E08(void* self, void* msg)
{
    int id = *(int*)((char*)msg + 0x38);
    return (id == GetHashValue32(D_004A13A0)) ? 0x101 : 0x100;
}
#endif

INCLUDE_ASM("fe/festateruleselect", func_00191E48);

INCLUDE_ASM("fe/festateruleselect", func_00192088);

INCLUDE_ASM("fe/festateruleselect", func_00192240);

//100%
INCLUDE_ASM("fe/festateruleselect", func_00192380);
#ifdef SKIP_ASM
extern "C" int func_0039A738(void* self);
extern "C" void func_00192088(void* self, int idx, int a2);
extern "C" void cFEStateRuleSelect_updateMenuColor(void* self);
extern int D_004410C8[];

struct sRuleSel_2380 {
    char pad[0x51];
    unsigned char count[15];
};

extern "C" void func_00192380(sRuleSel_2380* self, void* item)
{
    int mask = D_004410C8[*(int*)((char*)item + 0x18)] << 1;
    if (func_0039A738(item) == 0) {
        for (int i = 1; i < 15; i++) {
            int bit = 1 << i;
            if ((bit & mask) == 0) {
                int v = self->count[i] - 1;
                self->count[i] = v;
                if ((signed char)v <= 0) {
                    self->count[i] = 0;
                }
            }
        }
    } else {
        for (int i = 1; i < 15; i++) {
            int bit = 1 << i;
            if ((bit & mask) == 0) {
                self->count[i] += 1;
                func_00192088(self, i, 0);
            }
        }
    }
    cFEStateRuleSelect_updateMenuColor(self);
}
#endif

INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_updateMenuColor);

//100%
INCLUDE_ASM("fe/festateruleselect", func_001926F0);
#ifdef SKIP_ASM
extern void* D_0046B078[];

extern "C" void* func_001926F0(void* self)
{
    int i;
    *(void***)((char*)self + 0x30) = D_0046B078;
    for (i = 7; i >= 0; i--) {
        ((int*)((char*)self + 0x8))[i] = 0;
    }
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x2c) = 0;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x4) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", func_00192740);
#ifdef SKIP_ASM
struct sVEntry00192740 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj00192740 {
    int pad[2];
    sVEntry00192740* vt;
};

extern "C" void func_00192740(void* self, int a1)
{
    int i;
    *(int*)((char*)self + 0x28) = a1;
    for (i = 0; i < 8; i++) {
        sObj00192740* o = ((sObj00192740**)((char*)self + 0x8))[i];
        if (o != 0) {
            o->vt[8].fn((char*)o + o->vt[8].delta, a1);
        }
    }
}
#endif

INCLUDE_ASM("fe/festateruleselect", func_001927B0);

//100%
INCLUDE_ASM("fe/festateruleselect", func_00192918);
#ifdef SKIP_ASM
extern "C" int func_00192918(void* self, int val)
{
    int i;
    for (i = 0; i < 8; i++) {
        if (((int*)((char*)self + 0x8))[i] == val) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", func_00192948);
#ifdef SKIP_ASM
extern "C" int func_00192948(void* self, int a1)
{
    int mask = (int)(0x1000000 << a1) >> 24;
    return (*(int*)((char*)self + 0x2c) & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", func_00192968__FPvi);
#ifdef SKIP_ASM
void func_00192968(void* self, int val)
{
    *(int*)((char*)self + 0x2C) = val;
}
#endif

INCLUDE_ASM("fe/festateruleselect", func_00192970);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateruleselect", func_00192D00);
#ifdef SKIP_ASM
extern void* D_0046AF68[];
extern "C" void* func_001A8500(void* self, int a1, int a2);

extern "C" void* func_00192D00(void* self, int a1)
{
    char* p;
    int i;
    func_001A8500(self, a1, 0);
    *(void***)((char*)self + 0x8) = D_0046AF68;
    p = (char*)self + 0x6D0;
    for (i = 1; i != -1; i--, p += 0x34) {
        func_001926F0(p);
    }
    *(int*)((char*)self + 0xC) = 0x14;
    *(int*)((char*)self + 0x738) = 0;
    return self;
}
#endif


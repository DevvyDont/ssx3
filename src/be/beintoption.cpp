#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_0014F418(void* self);
extern const char D_0045A6B0[];
extern void* D_0045ADC8[16];
extern void* D_004A120C;

struct cBEOptionInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//92.24% - post-call register/global-reload scheduling not fully reproduced
INCLUDE_ASM("be/beintoption", cBEOptionInterface_getThis__Fv);
#ifdef SKIP_ASM
void* cBEOptionInterface_getThis()
{
    if (D_004A120C == 0) {
        cBEOptionInterface* mem = (cBEOptionInterface*)cMemMan_alloc(0x10, D_0045A6B0, 0, 0);
        mem->vtable = D_0045ADC8;
        D_004A120C = mem;
        func_0014F418(mem);
        ((cBEOptionInterface*)D_004A120C)->field_0x8 = 0;
    }
    return D_004A120C;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F2A8);
#ifdef SKIP_ASM
struct sOptions_0014F2A8
{
    int data[0x288 / 4];
};

extern sOptions_0014F2A8 D_00535610;
extern sOptions_0014F2A8 D_00535898;

extern "C" void func_0014F2A8(void)
{
    D_00535898 = D_00535610;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F360);
#ifdef SKIP_ASM
extern sOptions_0014F2A8 D_00535610;
extern sOptions_0014F2A8 D_00535898;

extern "C" void func_0014F360(void)
{
    D_00535610 = D_00535898;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F418);
#ifdef SKIP_ASM
extern "C" void func_0014F648(void* self);
extern "C" void func_0014F600(void* self);
extern "C" void func_0014F4F8(void* self);
extern "C" void func_0014F458(void* self);

extern "C" void func_0014F418(void* self)
{
    func_0014F648(self);
    func_0014F600(self);
    func_0014F4F8(self);
    func_0014F458(self);
}
#endif

INCLUDE_ASM("be/beintoption", func_0014F458);

INCLUDE_ASM("be/beintoption", func_0014F4F8);

INCLUDE_ASM("be/beintoption", func_0014F600);

INCLUDE_ASM("be/beintoption", func_0014F648);

int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
extern const char D_0045A6C0[];

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_getDefaultQuickKeyMessageHashValue__FPvi);
#ifdef SKIP_ASM
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int value)
{
    char buf[24];
    sprintf(buf, D_0045A6C0, value);
    return GetHashValue32(buf);
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F6A8);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char* s);
extern char D_00535617[][0x40];

extern "C" char* func_0014F6A8(void* self, int i)
{
    char* s = D_00535617[i];
    if (strlen(s) == 0) {
        return 0;
    }
    return s;
}
#endif

INCLUDE_ASM("be/beintoption", func_0014F6E8);

INCLUDE_ASM("be/beintoption", func_0014F758);

//100%
INCLUDE_ASM("be/beintoption", func_0014F7A8);
#ifdef SKIP_ASM
struct sOptionTripleView_0014F7A8 {
    int a;
    int b;
    int c;
};

struct sOptionTriplesView_0014F7A8 {
    sOptionTripleView_0014F7A8 triples[2]; // 0x0
};

struct sOptionGlobal_005308B8;
extern sOptionGlobal_005308B8 D_005308B8;
extern "C" int func_0014F870();

extern "C" int func_0014F7A8(int i)
{
    int c = ((sOptionTriplesView_0014F7A8*)&D_005308B8)->triples[i].c;
    return func_0014F870() - c;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F7E8);
#ifdef SKIP_ASM
extern "C" int func_0014F870();
extern int D_005308D4[]; // D_005308B8 + 0x1C, addressed through its own symbol

extern "C" int func_0014F7E8(void* self)
{
    return func_0014F870() - D_005308D4[0];
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F810);
#ifdef SKIP_ASM
// View of D_005308B8 (the unit defines sOptionGlobal_005308B8 further down).
struct sOptionTriple_005308B8 {
    int a;
    int b;
    int c;
};

struct sOptionView_005308B8 {
    sOptionTriple_005308B8 triples[2]; // 0x0
    int field_0x18;
    int field_0x1C;
};

struct sOptionGlobal_005308B8;
extern sOptionGlobal_005308B8 D_005308B8;

extern "C" int func_0014F810(int i)
{
    sOptionView_005308B8* g = (sOptionView_005308B8*)&D_005308B8;
    if (g->triples[i].a != 0 || g->triples[i].b != 0 || g->triples[i].c != 0 || g->field_0x18 != 0 || g->field_0x1C != 0)
    {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F870);
#ifdef SKIP_ASM
struct sOptionGlobal_005308B8 {
    char pad_0x00[0x8];
    int field_0x8;
    char pad_0xC[0x8];
    int field_0x14;
    char pad_0x18[0x4];
    int field_0x1C;
};
extern sOptionGlobal_005308B8 D_005308B8;

extern "C" int func_0014F870()
{
    int m = D_005308B8.field_0x1C;
    if (m < D_005308B8.field_0x8)
        m = D_005308B8.field_0x8;
    if (m < D_005308B8.field_0x14)
        m = D_005308B8.field_0x14;
    return m;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F898);
#ifdef SKIP_ASM
struct sOptionTriple_0014F898 {
    int a;
    int b;
    int c;
};

// Swaps the two 12-byte triples at the start of D_005308B8.
extern "C" void func_0014F898()
{
    sOptionTriple_0014F898* t = (sOptionTriple_0014F898*)&D_005308B8;
    sOptionTriple_0014F898 tmp = t[0];
    t[0] = t[1];
    t[1] = tmp;
}
#endif


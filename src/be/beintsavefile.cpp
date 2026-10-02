#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0045A7E0[];
extern void* D_0045AD08[16];
extern void* D_004A121C;
extern int D_004A1228;

struct cBESaveInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.8%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_getThis__Fv);
#ifdef SKIP_ASM
void* cBESaveInterface_getThis()
{
    if (D_004A121C == 0) {
        cBESaveInterface* mem = (cBESaveInterface*)cMemMan_alloc(0x10, D_0045A7E0, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045AD08;
        D_004A121C = mem;
        D_004A1228 = 0;
    }
    return D_004A121C;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", func_00152688);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char D_0045A7F0[];
extern char* D_004A1224;

extern "C" char* func_00152688(void)
{
    D_004A1228 = 1;
    if (D_004A1224 == 0)
        D_004A1224 = (char*)operator_new_tag(0x80000, D_0045A7F0, 0x100, 0);
    return D_004A1224;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", func_001526D0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
extern "C" int func_0014E048(void* be);
extern char* D_004A1224;

extern "C" char* func_001526D0(void)
{
    return D_004A1224 + func_0014E048(cBE_getBE());
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", func_00152700);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern void* D_004A1220;

extern "C" void func_00152700(void)
{
    if (D_004A1220)
        cMemMan_free(D_004A1220);
    D_004A1220 = 0;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", func_00152728);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern char* D_004A1224;

extern "C" void func_00152728(void)
{
    if (D_004A1224)
        cMemMan_free(D_004A1224);
    D_004A1224 = 0;
    D_004A1228 = 0;
}
#endif

INCLUDE_ASM("be/beintsavefile", func_00152758);

//100%
INCLUDE_ASM("be/beintsavefile", func_00152948__FPv);
#ifdef SKIP_ASM
int func_00152948(void* self)
{
    return 0xCCC;
}
#endif

INCLUDE_ASM("be/beintsavefile", func_00152950);

//100%
INCLUDE_ASM("be/beintsavefile", func_00152BA8);
#ifdef SKIP_ASM
extern "C" int func_00152BA8()
{
    return 0x9B61;
}
#endif

INCLUDE_ASM("be/beintsavefile", func_00152BB0);

INCLUDE_ASM("be/beintsavefile", func_00152DE0);

//100%
INCLUDE_ASM("be/beintsavefile", func_00153050);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
void cMemMan_free(void*);
extern void* D_004A1220;

extern "C" void func_00153050(void)
{
    operator_delete((int*)D_004A121C);
    D_004A121C = 0;
    if (D_004A1220)
        cMemMan_free(D_004A1220);
    D_004A1220 = 0;
}
#endif


#include "common.h"

INCLUDE_ASM("fe/festateloadhint", cFELoadHintState_onCreateScreen);

INCLUDE_ASM("fe/festateloadhint", func_00245AE0);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245B50__FPv);
#ifdef SKIP_ASM
void func_00245B50(void* self)
{
}
#endif

INCLUDE_ASM("fe/festateloadhint", func_00245B58);

INCLUDE_ASM("fe/festateloadhint", cFELoadState_onCreateScreen);

INCLUDE_ASM("fe/festateloadhint", func_00245C60);

INCLUDE_ASM("fe/festateloadhint", func_00245CD0);

INCLUDE_ASM("fe/festateloadhint", func_00245D28);

INCLUDE_ASM("fe/festateloadhint", cFELoadStateInLodge_onCreateScreen);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245DB8__FPv);
#ifdef SKIP_ASM
void func_00245DB8(void* self)
{
}
#endif

INCLUDE_ASM("fe/festateloadhint", func_00245DC0);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245E30);
#ifdef SKIP_ASM
extern unsigned int D_00536640[];

extern "C" int func_00245E30(int* a, int* b)
{
    unsigned int va = D_00536640[*a];
    unsigned int vb = D_00536640[*b];
    if (vb < va) {
        return -1;
    }
    return va != vb;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245E78);
#ifdef SKIP_ASM
extern unsigned int D_00536640[];

extern "C" int func_00245E78(int* a, int* b)
{
    unsigned int va = D_00536640[*a];
    unsigned int vb = D_00536640[*b];
    if (va < vb) {
        return -1;
    }
    return va != vb;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245EC0);
#ifdef SKIP_ASM
extern int D_00536708[];

extern "C" int func_00245EC0(int* a, int* b)
{
    int va = D_00536708[*a];
    int vb = D_00536708[*b];
    if (vb < va) {
        return 1;
    }
    return -1;
}
#endif

INCLUDE_ASM("fe/festateloadhint", func_00245F00);

extern "C" void* func_0039E390(void* self);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245F30__FPv);
#ifdef SKIP_ASM
void* func_00245F30(void* self)
{
    return func_0039E390(self);
}
#endif

INCLUDE_ASM("fe/festateloadhint", func_00245F50);

INCLUDE_ASM("fe/festateloadhint", func_00246098);

INCLUDE_ASM("fe/festateloadhint", func_002461E0);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00246268);
#ifdef SKIP_ASM
extern "C" int func_00246268(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/festateloadhint", func_00246278);

INCLUDE_ASM("fe/festateloadhint", func_002464E8);

INCLUDE_ASM("fe/festateloadhint", func_00246CF8);

INCLUDE_ASM("fe/festateloadhint", cFELoadState_InitwidgetMultiP);

INCLUDE_ASM("fe/festateloadhint", cFELoadState_widgetCreateQP);

extern "C" void* func_00242EB8(int, int);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00247E20__FPv);
#ifdef SKIP_ASM
void* func_00247E20(void* self)
{
    return func_00242EB8(1, 0xffff);
}
#endif

extern "C" void* func_00242EB8(int, int);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00247E40__FPv);
#ifdef SKIP_ASM
void* func_00247E40(void* self)
{
    return func_00242EB8(0, 0xffff);
}
#endif


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

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245B58);
#ifdef SKIP_ASM
extern void* D_0047C608[];
extern "C" void* func_0039E390(void* self);

extern "C" void* func_00245B58(void* self)
{
    *(void**)((char*)self + 0x8) = D_0047C608;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadState_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFELoadState_InitwidgetMultiP(void* self, int idx);
extern signed char D_00535C11[];
extern char D_0047BE40[];
extern char D_0047BE50[];

extern "C" void cFELoadState_onCreateScreen(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 2) {
        void* engine = *(void**)((char*)self + 0x10);
        void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0047BE40), 0);
        *(void**)((char*)self + 0x40) = screen;
        if (screen != 0) {
            cUIScreen_playFrame(screen, 0, 0);
            cFELoadState_InitwidgetMultiP(self, 0);
            cFELoadState_InitwidgetMultiP(self, 1);
        }
    } else {
        void* engine = *(void**)((char*)self + 0x10);
        void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0047BE50), 0);
        *(void**)((char*)self + 0x40) = screen;
        if (screen != 0) {
            cUIScreen_playFrame(screen, 0, 0);
        }
    }
}
#endif

INCLUDE_ASM("fe/festateloadhint", func_00245C60);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245CD0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cFELoadState_widgetCreateQP(void* self, void* widget);
extern signed char D_00535C11[];

extern "C" void func_00245CD0(void* self, void* widget)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] != 2) {
        cFELoadState_widgetCreateQP(self, widget);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245D28);
#ifdef SKIP_ASM
extern void* D_0047C538[];
extern "C" void* func_0039E390(void* self);

extern "C" void* func_00245D28(void* self)
{
    *(void**)((char*)self + 0x8) = D_0047C538;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadStateInLodge_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0047BFA0[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFELoadStateInLodge_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0047BFA0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245F00);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0047DB28[];

extern "C" void func_00245F00(void* self, int flags)
{
    *(void**)self = D_0047DB28;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/festateloadhint", func_002461E0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00186A08(void* self, void* engine);
extern "C" void func_0039F400(void* list, void* item);
extern char D_004A2AC8[];
extern char D_0047B978[];

extern "C" void func_002461E0(void* self, void* item, int msg)
{
    if (item != 0) {
        if (msg == 5) {
            int hash = *(int*)((char*)item + 0x38);
            if (hash == GetHashValue32(D_004A2AC8)) {
                void* state = func_00186A08(cMemMan_alloc(0x248, D_0047B978, 0, 0), *(void**)((char*)self + 0x10));
                func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, state);
            }
        }
    }
}
#endif

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


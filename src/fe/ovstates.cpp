#include "common.h"

INCLUDE_ASM("fe/ovstates", cFEStateTitle_onCreateScreen);

INCLUDE_ASM("fe/ovstates", func_001947F8);

INCLUDE_ASM("fe/ovstates", func_001948A8);

//100%
INCLUDE_ASM("fe/ovstates", func_00194980__FPv);
#ifdef SKIP_ASM
void func_00194980(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", func_00194988);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_003A0330(void* self, int a1, int a2);
extern char D_004A17D0[];

extern "C" void func_00194988(void* self, void* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(D_004A17D0)) {
        *(int*)((char*)item + 0x18) = 0;
        func_003A0330(item, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", func_001949E0);
#ifdef SKIP_ASM
struct sVEntry_func_001949E0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void func_0039F400(void* list, void* item);

extern "C" void func_001949E0(void* self, void* msg, int type)
{
    if (type == 5) {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry_func_001949E0* vt = *(sVEntry_func_001949E0**)((char*)obj + 0x4);
        void* r = vt[4].fn((char*)obj + vt[4].delta, self, *(int*)((char*)msg + 0x18));
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", func_00194A48);
#ifdef SKIP_ASM
extern "C" int func_00194A48(void* self, int a1, int a2)
{
    return a2 != 6 ? 0x100 : 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", func_00194A60);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void*);
extern void* D_0046B9E8[];

extern "C" void* func_00194A60(void* self)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 2;
    *(void***)((char*)self + 0x8) = D_0046B9E8;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onCreateScreen);

INCLUDE_ASM("fe/ovstates", func_00194BF0);

INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onWidgetCreate);

INCLUDE_ASM("fe/ovstates", func_00194DD0);

INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onWidgetEvent);

//100%
INCLUDE_ASM("fe/ovstates", func_001952E8);
#ifdef SKIP_ASM
extern "C" int func_001952E8(void* self, int a1, int a2)
{
    switch (a2) {
    case 6:
        return *(int*)((char*)self + 0x48) == 0 ? 1 : 0x101;
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/ovstates", func_00195328);

//100%
INCLUDE_ASM("fe/ovstates", func_00195498);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void*);
extern char D_0046B918[];

extern "C" void* func_00195498(void* self)
{
    func_0039E2A0(self);
    *(void**)((char*)self + 0x8) = D_0046B918;
    *(int*)((char*)self + 0xC) = 5;
    return self;
}
#endif


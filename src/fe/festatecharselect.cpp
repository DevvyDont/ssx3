#include "common.h"

INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onCreateScreen);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181238);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern signed char D_00535C11[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_0019E538(void* self, int a1);
void* func_0039E4A0(void* self);

extern "C" void func_00181238(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    char* mgr = *(char**)((char*)D_004A28A8 + 0x7C);
    if (mgr != 0 && D_00535C11[0] != 0) {
        func_0019E538(func_001A0548(mgr + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
    }
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_001812A8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void func_00181BD0(void* self, int index, signed char charID);

extern "C" int func_001812A8(void* self, int on)
{
    if (on != 0) {
        int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
        func_00181BD0(self, *(signed char*)((char*)self + 0x44), id);
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/festatecharselect", func_00181308);

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181400__FPv);
#ifdef SKIP_ASM
void* func_00181400(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181420);
#ifdef SKIP_ASM
extern "C" void func_00181EF0(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_00181420(void* self)
{
    func_00181EF0(self);
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/festatecharselect", func_00181450);

INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onWidgetCreate);

INCLUDE_ASM("fe/festatecharselect", func_001817E8);

INCLUDE_ASM("fe/festatecharselect", func_00181BD0);

INCLUDE_ASM("fe/festatecharselect", func_00181EF0);

INCLUDE_ASM("fe/festatecharselect", func_00182220);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182420);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045D780[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void func_00182420(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045D780), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetCreate);

INCLUDE_ASM("fe/festatecharselect", func_00182690);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182808);
#ifdef SKIP_ASM
extern "C" void func_00182870(void* self);

extern "C" int func_00182808(void* self, int a1, unsigned int msg, int value)
{
    switch (msg) {
    case 7:
    case 8:
    case 9:
        return 0x100;
    case 1:
        return value < *(int*)((char*)self + 0x78);
    case 5:
        func_00182870(self);
        break;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182870);
#ifdef SKIP_ASM
extern "C" void func_00182870(void* self)
{
    int i;
    int idx = *(unsigned char*)(*(char**)((char*)self + 0x6c) + 0x98);
    for (i = 0; i < 6; i++) {
        void* it = ((void**)((char*)self + 0x54))[i];
        if (it != 0) {
            int v = ((int*)((char*)self + 0xf4))[idx + i];
            *(int*)((char*)it + 0x78) = -1;
            *(int*)((char*)it + 0x7c) = v;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_001828C0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern void* D_0046CE60[];

extern "C" void* func_001828C0(void* self, void* engine, signed char idx)
{
    func_0039E2A0(self, engine);
    *(int*)((char*)self + 0xC) = 0xC;
    *(void***)((char*)self + 0x8) = D_0046CE60;
    *(signed char*)((char*)self + 0x44) = idx;
    *(unsigned char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    *(int*)((char*)self + 0x4C) = 0;
    return self;
}
#endif


#include "common.h"

INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_costVisible);

//100%
INCLUDE_ASM("fe/festatestore", func_00184780);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern char D_004A1458[];
extern char D_004A1460[];

class cWidget_00184780 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a);
};

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" void func_00184780(void* self, int on)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    cWidget_00184780* w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004A1458));
    if (w) {
        w->v09(on);
    }
    w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004A1460));
    if (w) {
        w->v09(on);
    }
}
#endif

INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_trickVisible);

INCLUDE_ASM("fe/festatestore", func_001849B0);

//100%
INCLUDE_ASM("fe/festatestore", func_00184B70);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_001A0570(void* self, int a1, int a2);
void* func_0039E4A0(void* self);

extern "C" void func_00184B70(void* self)
{
    char* mgr = *(char**)((char*)D_004A28A8 + 0x7C);
    if (mgr != 0) {
        func_001A0570(mgr + 0xB0, *(signed char*)((char*)self + 0x44), 0);
    }
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", func_00184BB8);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_checkGate(void* self);

extern "C" int func_00184BB8(void* self, int on)
{
    if (on != 0) {
        cWScriptMan_checkGate(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", func_00184BE0);
#ifdef SKIP_ASM
struct sVEntry00184BE0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj00184BE0 {
    int pad[2];
    sVEntry00184BE0* vt;
};

extern "C" void func_00183B08(void* self);
extern "C" void func_0039E4C0(void* self, int a1);

extern "C" void func_00184BE0(void* self, int a1)
{
    sObj00184BE0* o;
    func_00183B08(self);
    o = *(sObj00184BE0**)((char*)self + 0x4C);
    if (o != 0) {
        o->vt[7].fn((char*)o + o->vt[7].delta, 1);
    }
    o = *(sObj00184BE0**)((char*)self + 0x50);
    if (o != 0) {
        o->vt[7].fn((char*)o + o->vt[7].delta, 0);
    }
    func_0039E4C0(self, a1);
}
#endif

INCLUDE_ASM("fe/festatestore", func_00184C60);

INCLUDE_ASM("fe/festatestore", func_00184F40);

INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onWidgetCreate);

INCLUDE_ASM("fe/festatestore", func_00185268);

//100%
INCLUDE_ASM("fe/festatestore", func_001859D8);
#ifdef SKIP_ASM
extern "C" int func_001859D8(void* self, int a1, unsigned int a2)
{
    if (a1 == *(int*)((char*)self + 0x50)) {
        switch (a2) {
        case 9:
            return 0x100;
        }
    } else {
        switch (a2) {
        case 8:
        case 9:
            return 0x100;
        }
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", func_00185A18);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046BDF8[];

extern "C" void* func_00185A18(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046BDF8;
    *(int*)((char*)self + 0x60) = -100;
    *(int*)((char*)self + 0x64) = 1;
    *(int*)((char*)self + 0xC) = 0x24;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

extern void* D_0046D0D0[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/festatestore", func_00185A70__FPv);
#ifdef SKIP_ASM
void* func_00185A70(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif


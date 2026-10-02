#include "common.h"

INCLUDE_ASM("main/gamestate", cGFGateState_gainFocus);

//100%
INCLUDE_ASM("main/gamestate", func_00234BE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void* iface);
extern "C" int func_0022E098(void* world, int id);
extern "C" int func_00309DD0(void* mgr, int id, int a);
extern "C" void func_00309E50(void* mgr, int id, int a);
extern void* D_004A28A8;
extern void* D_004A3DD8;

extern "C" void func_00234BE8(void* self, int a1)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* world = *(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x78);
    int id = func_0022E098(world, *func_00144BC0(iface));
    if (func_00309DD0(D_004A3DD8, id, a1) != 0) {
        func_00309E50(D_004A3DD8, id, a1);
    }
}
#endif

INCLUDE_ASM("main/gamestate", cGFGateState_update);

extern "C" void* func_00233AF0(void* self);

//100%
INCLUDE_ASM("main/gamestate", func_00234DE0__FPv);
#ifdef SKIP_ASM
void* func_00234DE0(void* self)
{
    return func_00233AF0(self);
}
#endif

extern "C" void* func_00233B28(void* self);

//100%
INCLUDE_ASM("main/gamestate", func_00234E00__FPv);
#ifdef SKIP_ASM
void* func_00234E00(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234E20);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBE_setState(int state);
extern "C" void func_0039F840(void* list);
extern char* D_004A2C68;
extern signed char D_00535C10[];

extern "C" void func_00234E20(void)
{
    cAI_setAIState(*(void**)(D_004A2C68 + 0xC), 5);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C10[0] != 4) {
        char* g = D_004A2C68;
        *(int*)(*(char**)(g + 0xC) + 0x14) = 1;
        *(int*)(*(char**)(g + 0xC) + 0x8C) = 0;
    }
    cBE_setState(1);
    char* gs = D_004A2C68;
    int n = **(int**)(gs + 0x28);
    int ok = n != 0 && n < 10;
    if (!ok) {
        func_0039F840(*(char**)(gs + 0x48) + 0x18);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234EB8__FPv);
#ifdef SKIP_ASM
void* func_00234EB8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234ED8__FPv);
#ifdef SKIP_ASM
void* func_00234ED8(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234EF8);
#ifdef SKIP_ASM
extern char D_0047D6E8[];
extern char D_0047D428[];
extern char* D_004A2C68;

extern "C" void* func_00234EF8(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(void**)((char*)self + 0xC) = D_0047D6E8;
    D_004A2C68 = 0;
    *(void**)((char*)self + 0xC) = D_0047D428;
    *(int*)((char*)self + 0x0) = 4;
    *(int*)((char*)self + 0x10) = 5;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    return self;
}
#endif

INCLUDE_ASM("main/gamestate", func_00234F40);

INCLUDE_ASM("main/gamestate", func_00235080);

//100%
INCLUDE_ASM("main/gamestate", func_00235570);
#ifdef SKIP_ASM
extern "C" void func_00278CD8(void* a);
extern "C" void func_002790A0(void* a, int b);
extern void* D_004A28A4;

extern "C" void func_00235570(void* self)
{
    func_00278CD8(D_004A28A4);
    func_002790A0(D_004A28A4, 1);
    *(int*)((char*)self + 0x10) = 5;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002355B0);
#ifdef SKIP_ASM
extern "C" int func_002355B0(void* self)
{
    return *(int*)((char*)self + 0x10) == 5;
}
#endif

INCLUDE_ASM("main/gamestate", func_002355C0);

//100%
INCLUDE_ASM("main/gamestate", func_002357F8);
#ifdef SKIP_ASM
extern "C" int func_0027AA80(void* a);
extern "C" int func_00279220(void* a, int b);
extern "C" void func_00233AA0(void* self);
extern void* D_004A28A4;
extern void* D_004A28A8;

extern "C" void func_002357F8(void* self)
{
    if (*(int*)((char*)self + 0x1C) != 0 && func_0027AA80(D_004A28A4) == 0) {
        *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x78) + 0x1A4) = 1;
        *(int*)((char*)self + 0x1C) = 0;
    }
    if (func_00279220(D_004A28A4, 1) == 0) {
        func_00233AA0(self);
    }
}
#endif

INCLUDE_ASM("main/gamestate", func_00235868);

//100%
INCLUDE_ASM("main/gamestate", func_00235990);
#ifdef SKIP_ASM
struct sVEntry_func_00235990 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233AF0 also reads $a1 (the stream); bound with its real arity.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

extern "C" void func_00235990(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_00235990* e = &(*(sVEntry_func_00235990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00235990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00235990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00235A18);
#ifdef SKIP_ASM
struct sVEntry_func_00235A18 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233B28 also reads $a1 (the stream); bound with its real arity.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

extern "C" void func_00235A18(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00235A18* e = &(*(sVEntry_func_00235A18**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00235A18**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00235A18**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

INCLUDE_ASM("main/gamestate", func_00235AA0);

INCLUDE_ASM("main/gamestate", func_00235CC8);

//100%
INCLUDE_ASM("main/gamestate", func_00235F20);
#ifdef SKIP_ASM
extern "C" void func_001F36D8(void* hud, int a);
extern void* D_004A28A8;

extern "C" void func_00235F20(void)
{
    func_001F36D8(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x94), 3);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00235F48);
#ifdef SKIP_ASM
struct sVEntry_func_00235F48 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233AF0 also reads $a1 (the stream); bound with its real arity.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

extern "C" void func_00235F48(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_00235F48* e = &(*(sVEntry_func_00235F48**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00235F48**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00235F48**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00235FD0);
#ifdef SKIP_ASM
struct sVEntry_func_00235FD0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233B28 also reads $a1 (the stream); bound with its real arity.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

extern "C" void func_00235FD0(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00235FD0* e = &(*(sVEntry_func_00235FD0**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00235FD0**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00235FD0**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

INCLUDE_ASM("main/gamestate", func_00236058);

//100%
INCLUDE_ASM("main/gamestate", func_00236198);
#ifdef SKIP_ASM
extern "C" void func_00231250(void* self, int a, int b, int refresh);
extern void* D_004A28A8;

extern "C" void func_00236198(void)
{
    func_00231250(*(void**)((char*)D_004A28A8 + 0x84), 1, 0, 0);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002361C8__FPv);
#ifdef SKIP_ASM
void* func_002361C8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002361E8__FPv);
#ifdef SKIP_ASM
void* func_002361E8(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236208);
#ifdef SKIP_ASM
extern char D_0047D6E8[];
extern char D_0047D320[];
extern char* D_004A2C68;

extern "C" void* func_00236208(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(void**)((char*)self + 0xC) = D_0047D6E8;
    D_004A2C68 = 0;
    *(void**)((char*)self + 0xC) = D_0047D320;
    *(int*)((char*)self + 0x0) = 4;
    *(int*)((char*)self + 0x18) = 0x17;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x14) = 0;
    return self;
}
#endif

INCLUDE_ASM("main/gamestate", func_00236250);

INCLUDE_ASM("main/gamestate", func_00236418);

//100%
INCLUDE_ASM("main/gamestate", func_00236728);
#ifdef SKIP_ASM
struct sVEntry_func_00236728 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233AF0 also reads $a1 (the stream); bound with its real arity.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

extern "C" void func_00236728(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_00236728* e = &(*(sVEntry_func_00236728**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00236728**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00236728**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x8, 4);
    e = &(*(sVEntry_func_00236728**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002367C8);
#ifdef SKIP_ASM
struct sVEntry_func_002367C8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233B28 also reads $a1 (the stream); bound with its real arity.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

extern "C" void func_002367C8(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_002367C8* e = &(*(sVEntry_func_002367C8**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_002367C8**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_002367C8**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x8, 4);
    e = &(*(sVEntry_func_002367C8**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236868);
#ifdef SKIP_ASM
extern char D_0047D6E8[];
extern char D_0047D2C8[];
extern char* D_004A2C68;

extern "C" void* func_00236868(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(void**)((char*)self + 0xC) = D_0047D6E8;
    D_004A2C68 = 0;
    *(void**)((char*)self + 0xC) = D_0047D2C8;
    *(int*)((char*)self + 0x0) = 0xA;
    *(int*)((char*)self + 0x4) = 0;
    return self;
}
#endif

INCLUDE_ASM("main/gamestate", func_002368A0);

INCLUDE_ASM("main/gamestate", func_00236960);

//100%
INCLUDE_ASM("main/gamestate", func_00236AF0__FPv);
#ifdef SKIP_ASM
void func_00236AF0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236AF8__FPv);
#ifdef SKIP_ASM
void* func_00236AF8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236B18__FPv);
#ifdef SKIP_ASM
void* func_00236B18(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236B38);
#ifdef SKIP_ASM
class cRace236B38 {
public:
    char pad[0xCC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};
extern char* D_004A2C68;

extern "C" void func_00236B38(void)
{
    (*(cRace236B38**)(D_004A2C68 + 0xC))->v05(3);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236B70__FPv);
#ifdef SKIP_ASM
void* func_00236B70(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236B90__FPv);
#ifdef SKIP_ASM
void* func_00236B90(void* self)
{
    return func_00233B28(self);
}
#endif

INCLUDE_ASM("main/gamestate", func_00236BB0);

//100%
INCLUDE_ASM("main/gamestate", func_00236C48);
#ifdef SKIP_ASM
extern "C" void func_00233AA0(void* self);
extern char* D_004A2EEC;

extern "C" void func_00236C48(void* self)
{
    if (D_004A2EEC == 0 || *(int*)(D_004A2EEC + 4) == 0) {
        if (*(int*)((char*)self + 8) == 1) {
            func_00233AA0(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236C88);
#ifdef SKIP_ASM
extern "C" void func_00236CD8(void* self);
extern int D_004A2A54;
extern int D_004A2A50;
extern int D_005366E8[];
extern int D_004428F0[];

extern "C" void func_00236C88(void* self)
{
    D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
    func_00236CD8(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236CD8);
#ifdef SKIP_ASM
class cRace236CD8 {
public:
    char pad[0xCC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_002790A0(void* a, int b);
extern "C" void* func_0028B180(void);
extern "C" void func_002871B0(void* mgr);
extern void* D_004A28A4;
extern char* D_004A2C68;
extern signed char D_00535C10[];

static inline int isMode4_236CD8()
{
    return D_00535C10[0] == 4;
}

extern "C" void func_00236CD8(void* self)
{
    func_002790A0(D_004A28A4, 0);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (!isMode4_236CD8()) {
        (*(cRace236CD8**)(D_004A2C68 + 0xC))->v05(3);
        *(int*)(*(char**)(D_004A2C68 + 0xC) + 0x14) = 1;
    }
    func_002871B0(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236D60__FPv);
#ifdef SKIP_ASM
void* func_00236D60(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236D80__FPv);
#ifdef SKIP_ASM
void* func_00236D80(void* self)
{
    return func_00233B28(self);
}
#endif

INCLUDE_ASM("main/gamestate", func_00236DA0);

//100%
INCLUDE_ASM("main/gamestate", func_00236E60);
#ifdef SKIP_ASM
extern "C" void func_00231250(void* self, int a, int b, int refresh);
extern void* D_004A28A8;

extern "C" void func_00236E60(void* self)
{
    if (*(int*)((char*)self + 8) == 1) {
        func_00231250(*(void**)((char*)D_004A28A8 + 0x84), 0xE, 2, 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236EA0);
#ifdef SKIP_ASM
extern "C" void func_0039F840(void* list);
extern char* D_004A2C68;

extern "C" void func_00236EA0(void)
{
    func_0039F840(*(char**)(D_004A2C68 + 0x48) + 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236EC8__FPv);
#ifdef SKIP_ASM
void* func_00236EC8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236EE8__FPv);
#ifdef SKIP_ASM
void* func_00236EE8(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236F08);
#ifdef SKIP_ASM
extern "C" void* func_0028B180(void);
extern "C" void func_0028CDF8(void* mgr);
extern void* D_004A28A8;

extern "C" void func_00236F08(void)
{
    func_0028CDF8(func_0028B180());
    *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0x8C) = 1;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236F40);
#ifdef SKIP_ASM
extern "C" int func_00279220(void* a, int b);
extern "C" void func_00233AA0(void* self);
extern void* D_004A28A4;

extern "C" void func_00236F40(void* self)
{
    int r = func_00279220(D_004A28A4, 0);
    int busy = r != 0 && r != 5;
    if (!busy) {
        func_00233AA0(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236F90);
#ifdef SKIP_ASM
extern void* D_004A28A8;

extern "C" void func_00236F90(void)
{
    *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0x8C) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236FA8__FPv);
#ifdef SKIP_ASM
void* func_00236FA8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236FC8__FPv);
#ifdef SKIP_ASM
void* func_00236FC8(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236FE8);
#ifdef SKIP_ASM
extern "C" void func_00309030(void* a);
extern "C" void func_00309F18(void* a);
extern "C" void func_001F36D8(void* hud, int a);
extern "C" void func_0020AB50(int state);
extern void* D_004A3DD8;
extern void* D_004A28A8;

extern "C" void func_00236FE8(void)
{
    func_00309030(D_004A3DD8);
    func_00309F18(D_004A3DD8);
    func_001F36D8(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x94), 0);
    func_0020AB50(0x11);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237030__FPv);
#ifdef SKIP_ASM
void func_00237030(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237038);
#ifdef SKIP_ASM
extern "C" void func_00308F38(void* a);
extern "C" void func_00308C60(void* a);
extern void* D_004A3DD8;

extern "C" void func_00237038(void)
{
    func_00308F38(D_004A3DD8);
    func_00308C60(D_004A3DD8);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237060);
#ifdef SKIP_ASM
extern "C" void* func_00237060(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x4) = a1;
    *(int*)((char*)self + 0x8) = a2;
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x0) = a1 * a2;
    *(int*)((char*)self + 0x4) = (60 / a2) * a2;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002370A0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

struct sEntry_func_002370A0 {
    void* buf;
    int pad[3];
    int* a;
    int* b;
};

struct sObj_func_002370A0 {
    char pad[0x10];
    int count;
    sEntry_func_002370A0 entries[1];
};

extern "C" void func_002370A0(sObj_func_002370A0* self, int flags)
{
    int i;
    for (i = 0; i < self->count; i++) {
        sEntry_func_002370A0* e = &self->entries[i];
        if (e->buf)
            cMemMan_free(e->buf);
        operator_delete(e->a);
        operator_delete(e->b);
    }
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

INCLUDE_ASM("main/gamestate", func_00237140);

INCLUDE_ASM("main/gamestate", func_00237280);

INCLUDE_ASM("main/gamestate", func_002373B8);

//100%
INCLUDE_ASM("main/gamestate", func_00237948__FPv);
#ifdef SKIP_ASM
void func_00237948(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237950);
#ifdef SKIP_ASM
struct sSlot237950 {
    int f0;
    int cur;
    int max;
    int pad[3];
};
struct sOwner237950 {
    char pad[0x10];
    int count;
    sSlot237950 slots[1];
};
extern char* D_004A2EEC;

extern "C" int func_00237950(sOwner237950* self)
{
    char* p = D_004A2EEC;
    if (*(int*)(p + 0x70) < 14) {
        return 0;
    }
    int busy = *(int*)(p + 0x3F8) != 0 || *(int*)(p + 0x3FC) > 0;
    if (busy) {
        return 0;
    }
    int i;
    for (i = 0; i < self->count; i++) {
        sSlot237950* s = &self->slots[i];
        if (s->cur == s->max) {
            return 0;
        }
    }
    return 1;
}
#endif

INCLUDE_ASM("main/gamestate", func_002379C8);

//100%
INCLUDE_ASM("main/gamestate", func_00237A98);
#ifdef SKIP_ASM
struct sPackSrc_7B28;
struct sPackDst_7B28;
extern "C" int func_00237B28(void* self, sPackSrc_7B28* src, sPackDst_7B28* dst);
extern "C" void func_00237BE8(void* self, sPackSrc_7B28* src, char* buf);
extern "C" void func_00257E98(void* mgr, int a, char* buf, int len);
extern char* D_004A2EEC;

extern "C" void func_00237A98(void* self, sPackSrc_7B28* src)
{
    char buf[32];
    int len = func_00237B28(self, src, (sPackDst_7B28*)buf);
    func_00237BE8(self, src, buf);
    char* p = D_004A2EEC;
    int ok = *(int*)(p + 0x40) != 0 && *(int*)(p + 4) == 0 && *(int*)(p + 0) != 0;
    if (ok) {
        func_00257E98(D_004A2EEC, 0, buf, len);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237B28);
#ifdef SKIP_ASM
struct sPackSrc_7B28 {
    int count;
    float vals[1];
};

struct sPackDst_7B28 {
    unsigned char count;
    unsigned char mask[3];
    unsigned char vals[1];
};

extern "C" int func_00237B28(void* self, sPackSrc_7B28* src, sPackDst_7B28* dst)
{
    int i;
    int j;
    int n;
    dst->count = src->count;
    for (j = 0; j < 3; j++) {
        dst->mask[j] = 0;
    }
    n = 0;
    for (i = 0; i < src->count; i++) {
        unsigned char v = (int)(src->vals[i] * 255.0f);
        if (v != 0) {
            dst->mask[i >> 3] |= 1 << (i & 7);
            dst->vals[n++] = v;
        }
    }
    return n + 4;
}
#endif

INCLUDE_ASM("main/gamestate", func_00237BE8);

//100%
INCLUDE_ASM("main/gamestate", func_00237CB0);
#ifdef SKIP_ASM
extern "C" void cBxPseudoRng_Seed(uint* state, uint seed);
extern uint D_004C9548[];

extern "C" void func_00237CB0(uint seed)
{
    cBxPseudoRng_Seed(D_004C9548, seed);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237CD8);
#ifdef SKIP_ASM
extern "C" uint cBxPseudoRng_NextInt(uint* state);
extern uint D_004C9548[];

extern "C" uint func_00237CD8(void)
{
    return cBxPseudoRng_NextInt(D_004C9548);
}
#endif


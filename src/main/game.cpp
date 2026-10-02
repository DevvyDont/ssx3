#include "common.h"

// 16-byte array elements; indexing as arr[i].field (rather than manual
// pointer arithmetic) is what makes GCC emit the target's base-first addu
struct sGameEntry {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
};
extern sGameEntry D_00442168[];

INCLUDE_ASM("main/game", cGame_renderModels);

INCLUDE_ASM("main/game", func_0022C1B0);

//100%
INCLUDE_ASM("main/game", func_0022C3B8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
struct sVEntry_func_0022C3B8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0022C3B8(void* self, int flags)
{
    void* obj = *(void**)self;
    sVEntry_func_0022C3B8* vt = *(sVEntry_func_0022C3B8**)((char*)obj + 0x4);
    vt[18].fn((char*)obj + vt[18].delta);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("main/game", func_0022C410);

INCLUDE_ASM("main/game", cGame_renderPatches);

//100%
INCLUDE_ASM("main/game", cGame_renderFogVolumes);
#ifdef SKIP_ASM
struct sVEntry_renderFog {
    short delta;
    short index;
    void (*fn)(void*, void*, int, int);
};
extern "C" void func_002DBF98(void* r);

extern "C" void cGame_renderFogVolumes(void* self, void* world)
{
    void* fog = (char*)world + 0x78B8;
    int n = *(int*)((char*)world + 0x78B4);
    if (n != 0) {
        void* r = *(void**)((char*)self + 0x18);
        sVEntry_renderFog* vt = *(sVEntry_renderFog**)((char*)r + 0x4);
        vt[2].fn((char*)r + vt[2].delta, fog, n, 0);
    }
    fog = (char*)world + 0x7AC0;
    n = *(int*)((char*)world + 0x7ABC);
    if (n != 0) {
        void* r = *(void**)((char*)self + 0x18);
        sVEntry_renderFog* vt = *(sVEntry_renderFog**)((char*)r + 0x4);
        vt[2].fn((char*)r + vt[2].delta, fog, n, 2);
    }
    func_002DBF98(*(void**)((char*)self + 0x18));
}
#endif

//100%
INCLUDE_ASM("main/game", cGame_renderLightHalos);
#ifdef SKIP_ASM
void func_002E2F98(void* self, int a1);
extern "C" void func_002E2FF8(void* self, void* list, int n, int mode, int a4);
extern "C" void func_002E30D0(void* self, int a1);

extern "C" void cGame_renderLightHalos(void* self, void* world, int a2)
{
    *(int*)((char*)*(void**)((char*)self + 0x1C) + 0x6494) = 1;
    func_002E2F98(*(void**)((char*)self + 0x1C), a2);
    void* p = (char*)world + 0x7BC4;
    int n = *(int*)((char*)world + 0x7BC0);
    if (n != 0) {
        func_002E2FF8(*(void**)((char*)self + 0x1C), p, n, 0, a2);
    }
    p = (char*)world + 0x7FC8;
    n = *(int*)((char*)world + 0x7FC4);
    if (n != 0) {
        func_002E2FF8(*(void**)((char*)self + 0x1C), p, n, 2, a2);
    }
    func_002E30D0(*(void**)((char*)self + 0x1C), a2);
    *(int*)((char*)*(void**)((char*)self + 0x1C) + 0x6494) = 0;
}
#endif

INCLUDE_ASM("main/game", func_0022C830);

//100%
INCLUDE_ASM("main/game", func_0022CCE8);
#ifdef SKIP_ASM
extern "C" void func_0022CD40(void* self);

extern "C" void* func_0022CCE8(void* self, int a1)
{
    *(int*)((char*)self + 0x1B0) = a1;
    func_0022CD40(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022CD18);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_0022CD18(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("main/game", func_0022CD40);

INCLUDE_ASM("main/game", func_0022CEA8);

INCLUDE_ASM("main/game", func_0022D088);

INCLUDE_ASM("main/game", func_0022D278);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/game", func_0022D390);
#ifdef SKIP_ASM
extern "C" int func_0022D278(void* self);

extern "C" int func_0022D390(void* self)
{
    int r = 0;
    if (func_0022D278(self) != 0) {
        r = *(int*)((char*)self + 0x1C0) != 0;
    }
    return r;
}
#endif

INCLUDE_ASM("main/game", func_0022D3D8);

INCLUDE_ASM("main/game", func_0022D478);

INCLUDE_ASM("main/game", func_0022D598);

//100%
INCLUDE_ASM("main/game", func_0022D640);
#ifdef SKIP_ASM
extern "C" void func_0022D640(void* self, int a1)
{
    if (a1 != 0 && *(int*)((char*)self + 0x1B4) == 0) {
        *(int*)((char*)self + 0x1B4) = 1;
        *(int*)((char*)self + 0x1B8) = a1;
    }
}
#endif

INCLUDE_ASM("main/game", func_0022D668);

INCLUDE_ASM("main/game", func_0022D6C8);

INCLUDE_ASM("main/game", func_0022D8D8);

//100%
INCLUDE_ASM("main/game", func_0022DE58);
#ifdef SKIP_ASM
extern "C" void func_0022D478(void* self, int v);

extern "C" void func_0022DE58(void* self, int v)
{
    if (*(int*)((char*)self + 0x1AC) != v) {
        *(int*)((char*)self + 0x1AC) = v;
        func_0022D478(self, v);
        *(int*)((char*)self + 0x1A8) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022DE98);
#ifdef SKIP_ASM
extern "C" void func_0022D598(void* self, int id);
void func_00353D98(void* self);
extern "C" void func_00353CF0(void* self, int id);

extern "C" void func_0022DE98(void* self)
{
    if (*(int*)((char*)self + 0x1A8) == 0) {
        return;
    }
    if (*(int*)((char*)self + 0x1A4) == 0) {
        return;
    }
    int i;
    for (i = 0x2C; i < 0x31; i++) {
        if (i != *(int*)((char*)self + 0x1AC)) {
            func_0022D598(self, i);
        }
    }
    int id = D_00442168[*(int*)((char*)self + 0x1AC)].field_0x4;
    void* o = *(void**)((char*)*(void**)((char*)self + 0x1B0) + 0x14);
    if (*(int*)((char*)o + 0x10) != id) {
        func_00353D98(o);
        func_00353CF0(*(void**)((char*)*(void**)((char*)self + 0x1B0) + 0x14), id);
    }
    *(int*)((char*)self + 0x1A4) = 0;
    *(int*)((char*)self + 0x1A8) = 0;
    *(int*)((char*)self + 0x1AC) = -1;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022DF50);
#ifdef SKIP_ASM
class cBEIface22DF50 {
public:
    int pad[3];
    virtual void v01();
};
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00231250(void* gm, int state, int a, int b);
void func_00231278(void* gm, int state);
extern int D_00535C08[];
extern int D_004A11B8;

extern "C" void func_0022DF50(void* self)
{
    func_00231250(*(void**)((char*)self + 0x1B0), 0xB, 0, 1);
    cBEIface22DF50* iface = (cBEIface22DF50*)cBE_getInterface_Fv(cBE_getBE(), 0);
    D_00535C08[0] = *(int*)((char*)self + 0x1BC);
    D_004A11B8 = 1;
    iface->v01();
    func_00231278(*(void**)((char*)self + 0x1B0), 0xA);
}
#endif

extern "C" void* func_002312D8(int);

//100%
INCLUDE_ASM("main/game", func_0022DFD0__FPv);
#ifdef SKIP_ASM
void* func_0022DFD0(void* self)
{
    return func_002312D8(*(int*)((char*)self + 0x1b0));
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022DFF0);
#ifdef SKIP_ASM
class cRace22DFF0 {
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
extern "C" int* func_00144BC0(void* iface);
extern "C" void cSectionMan_setSky(void* self, int sky);
extern "C" void func_00231250(void* gm, int state, int a, int b);
extern void* D_004A28A8;

extern "C" void func_0022DFF0(void* self)
{
    cSectionMan_setSky(self, *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0)));
    if (*(int*)((char*)self + 0x1CC) != 0) {
        *(int*)((char*)self + 0x1CC) = 0;
        (*(cRace22DFF0**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC))->v05(3);
    }
    func_00231250(*(void**)((char*)self + 0x1B0), 0xA, 0, 0);
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022E078);
#ifdef SKIP_ASM
extern "C" int func_0022E078(void* self, int a1)
{
    int v = D_00442168[a1].field_0x8;
    return (unsigned int)(v - 1) < 2;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022E098);
#ifdef SKIP_ASM
struct sGameEntry2 {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    int field_0x14;
    int field_0x18;
    int field_0x1c;
    int field_0x20;
    int field_0x24;
};
extern sGameEntry2 D_00442488[];

extern "C" int func_0022E098(void* self, int a1)
{
    return D_00442168[D_00442488[a1].field_0x10].field_0x4;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022E0C8);
#ifdef SKIP_ASM
extern "C" int func_0022E0C8(void* self, int a1)
{
    return D_00442168[a1].field_0x4;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022E0E0);
#ifdef SKIP_ASM
extern "C" int func_0022E0E0(void* self, int a1)
{
    int i;
    for (i = 0; i < 50; i++) {
        if (a1 == D_00442168[i].field_0x4) {
            return i;
        }
    }
    return 50;
}
#endif


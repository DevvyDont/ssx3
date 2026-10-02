#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPSPBezierMan_cPSPBezierMan(void* self);
extern const char D_00492EA0[];

//99.23%
INCLUDE_ASM("render/bezierman", cBezierMan_construct__Fv);
#ifdef SKIP_ASM
void* cBezierMan_construct()
{
    void* mem = cMemMan_alloc(0x9C90, D_00492EA0, 0, 0);
    return cPSPBezierMan_cPSPBezierMan(mem);
}
#endif

INCLUDE_ASM("render/bezierman", func_0038AF30);

//100%
INCLUDE_ASM("render/bezierman", func_0038B0F8);
#ifdef SKIP_ASM
void func_0038D660(void*);
void cMemMan_free(void*);

extern "C" void func_0038B0F8(void* self)
{
    func_0038D660(self);
    if (*(void**)((char*)self + 0x440) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x440));
    }
    if (*(void**)((char*)self + 0x444) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x444));
    }
    if (*(void**)((char*)self + 0x448) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x448));
    }
}
#endif

extern "C" void* func_003739D0(void*);

//100%
INCLUDE_ASM("render/bezierman", func_0038B158__FPv);
#ifdef SKIP_ASM
void* func_0038B158(void* self)
{
    return func_003739D0((char*)self + 0x10);
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038B178__FPv);
#ifdef SKIP_ASM
void func_0038B178(void* self)
{
    *(int*)((char*)self + 0x458) = 0;
    *(int*)((char*)self + 0x3c9c) = 0;
    *(int*)((char*)self + 0x4a60) = 0;
    *(int*)((char*)self + 0x4d84) = 0;
}
#endif

INCLUDE_ASM("render/bezierman", func_0038B190);

//100%
INCLUDE_ASM("render/bezierman", func_0038B338);
#ifdef SKIP_ASM
extern int D_004A44D4;

struct sBezVEntry_B338 {
    short delta;
    short index;
    void (*fn)(void*, void*, void*, void*, void*, int);
};

extern "C" void func_0038B338(void* self, void* a, void* b, void* c, void* d)
{
    if (D_004A44D4 == 0) {
        sBezVEntry_B338* vt = *(sBezVEntry_B338**)((char*)self + 0x4);
        vt[22].fn((char*)self + vt[22].delta, a, b, c, d, 0);
    }
}
#endif

INCLUDE_ASM("render/bezierman", func_0038B370);

INCLUDE_ASM("render/bezierman", func_0038C788);

//100%
INCLUDE_ASM("render/bezierman", func_0038CA08);
#ifdef SKIP_ASM
extern void* D_004A5B80;
extern "C" unsigned int func_0037DF88(void* self, void* a1, void* a2);

struct sBezVec4_CA08 {
    float x, y, z, w;
};

extern "C" void func_0038CA08(void* self, void* obj)
{
    char* o = (char*)obj;
    sBezVec4_CA08 a;
    sBezVec4_CA08 b;
    a.x = *(float*)(o + 0x158);
    a.y = *(float*)(o + 0x15C);
    a.z = *(float*)(o + 0x160);
    a.w = 1.0f;
    b.x = *(float*)(o + 0x164);
    b.y = *(float*)(o + 0x168);
    b.z = *(float*)(o + 0x16C);
    b.w = 1.0f;
    func_0037DF88(D_004A5B80, &a, &b);
}
#endif

INCLUDE_ASM("render/bezierman", func_0038CA70);

INCLUDE_ASM("render/bezierman", func_0038CE20);

INCLUDE_ASM("render/bezierman", func_0038D168);

INCLUDE_ASM("render/bezierman", func_0038D448);

//100%
INCLUDE_ASM("render/bezierman", func_0038D638);
#ifdef SKIP_ASM
class cBezierVirt {
public:
    int field_0x0;
    // vptr lands at 0x4 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
};

extern "C" void func_0038D638(cBezierVirt* self)
{
    self->v17();
}
#endif

//100%
INCLUDE_ASM("render/bezierman", func_0038D660__FPv);
#ifdef SKIP_ASM
void func_0038D660(void* self)
{
}
#endif

INCLUDE_ASM("render/bezierman", func_0038D690);

INCLUDE_ASM("render/bezierman", func_0038D968);


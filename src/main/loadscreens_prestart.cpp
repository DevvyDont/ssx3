#include "common.h"

INCLUDE_ASM("main/loadscreens_prestart", cPreStartScreen_update);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232488);
#ifdef SKIP_ASM
struct sVec3_232488 {
    float x, y, z;
    sVec3_232488(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
class cWorld232488 {
public:
    char pad[0x10D8];
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
    virtual void v15(const sVec3_232488&);
    virtual void v16();
    virtual int v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
};
extern "C" void func_00231FC0(void* a);
extern void* D_004A289C;
extern void* D_004A28A8;

extern "C" int func_00232488(void)
{
    if (((cWorld232488*)D_004A289C)->v17() == 0) {
        return 0;
    }
    ((cWorld232488*)D_004A289C)->v15(sVec3_232488(0.0f, 0.0f, 0.0f));
    func_00231FC0(*(void**)((char*)D_004A28A8 + 0xA4));
    ((cWorld232488*)D_004A289C)->v20();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232510);
#ifdef SKIP_ASM
extern "C" int func_00232510(void* self)
{
    if (*(float*)((char*)self + 0x4) < 0.75f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232538);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046D970[];

extern "C" void func_00232538(void* self, int flags)
{
    *(void***)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232568);
#ifdef SKIP_ASM
extern "C" void cSSXApp_initload(void* app);
extern void* D_004A28A8;

extern "C" void func_00232568(void)
{
    cSSXApp_initload(D_004A28A8);
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_00232588);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232650);
#ifdef SKIP_ASM
struct sVec3_232650 {
    float x, y, z;
    sVec3_232650(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
class cWorld232650 {
public:
    char pad[0x10D8];
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
    virtual void v15(const sVec3_232650&);
    virtual void v16();
    virtual int v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
};
extern "C" void func_00231FC0(void* a);
extern void* D_004A289C;
extern void* D_004A28A8;

extern "C" int func_00232650(void)
{
    if (((cWorld232650*)D_004A289C)->v17() == 0) {
        return 0;
    }
    ((cWorld232650*)D_004A289C)->v15(sVec3_232650(0.0f, 0.0f, 0.0f));
    func_00231FC0(*(void**)((char*)D_004A28A8 + 0xA4));
    ((cWorld232650*)D_004A289C)->v20();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_002326D8);
#ifdef SKIP_ASM
struct sLoadScreen2326D8 {
    int f0;
    int f4;
    int done;
};
extern void* D_004A28A8;

extern "C" int func_002326D8(sLoadScreen2326D8* self)
{
    self->done = 1;
    return *(float*)(*(char**)((char*)D_004A28A8 + 0xA4) + 4) == 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232710__FPv);
#ifdef SKIP_ASM
void func_00232710(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232718__FPv);
#ifdef SKIP_ASM
void func_00232718(void* self)
{
}
#endif

extern void* D_0047D8A8[];

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232720__FPv);
#ifdef SKIP_ASM
void* func_00232720(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    *(int*)self = (int)(void*)D_0047D8A8;
    return self;
}
#endif

extern void* D_0047D860[];

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232738__FPv);
#ifdef SKIP_ASM
void* func_00232738(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    *(int*)self = (int)(void*)D_0047D860;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232750);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046D970[];

extern "C" void func_00232750(void* self, int flags)
{
    *(void***)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232780);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046D970[];

extern "C" void func_00232780(void* self, int flags)
{
    *(void***)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_002327B0);
#ifdef SKIP_ASM
struct sLoadScreen2327B0 {
    int f0;
    int done;
};
extern void* D_004A28A8;

extern "C" int func_002327B0(sLoadScreen2327B0* self)
{
    self->done = 1;
    return *(float*)(*(char**)((char*)D_004A28A8 + 0xA4) + 4) == 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_002327E8);
#ifdef SKIP_ASM
struct sLoadScreen2327E8 {
    int f0;
    int done;
};
extern void* D_004A28A8;

extern "C" int func_002327E8(sLoadScreen2327E8* self)
{
    self->done = 1;
    return *(float*)(*(char**)((char*)D_004A28A8 + 0xA4) + 4) == 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232820);
#ifdef SKIP_ASM
struct sVec3_232820 {
    float x, y, z;
    sVec3_232820(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
class cWorld232820 {
public:
    char pad[0x10D8];
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
    virtual void v15(const sVec3_232820&);
    virtual void v16();
    virtual int v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
};
extern "C" void func_00231FC0(void* a);
extern void* D_004A289C;
extern void* D_004A28A8;

extern "C" int func_00232820(void)
{
    if (((cWorld232820*)D_004A289C)->v17() == 0) {
        return 0;
    }
    ((cWorld232820*)D_004A289C)->v15(sVec3_232820(0.0f, 0.0f, 0.0f));
    func_00231FC0(*(void**)((char*)D_004A28A8 + 0xA4));
    ((cWorld232820*)D_004A289C)->v20();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_002328A8);
#ifdef SKIP_ASM
struct sVec3_2328A8 {
    float x, y, z;
    sVec3_2328A8(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
class cWorld2328A8 {
public:
    char pad[0x10D8];
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
    virtual void v15(const sVec3_2328A8&);
    virtual void v16();
    virtual int v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
};
extern "C" void func_00231FC0(void* a);
extern void* D_004A289C;
extern void* D_004A28A8;

extern "C" int func_002328A8(void)
{
    if (((cWorld2328A8*)D_004A289C)->v17() == 0) {
        return 0;
    }
    ((cWorld2328A8*)D_004A289C)->v15(sVec3_2328A8(0.0f, 0.0f, 0.0f));
    func_00231FC0(*(void**)((char*)D_004A28A8 + 0xA4));
    ((cWorld2328A8*)D_004A289C)->v20();
    return 1;
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_00232930);

INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen_update);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232AE0);
#ifdef SKIP_ASM
extern "C" void func_00231CD0(void* self);
extern void* D_0047D7D0[];

extern "C" void* func_00232AE0(void* self, int arg)
{
    func_00231CD0(self);
    *(void***)self = D_0047D7D0;
    *(int*)((char*)self + 0x18) = arg;
    *(int*)((char*)self + 0xC) = 0;
    return self;
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_00232B28);

INCLUDE_ASM("main/loadscreens_prestart", func_00232C98);

extern "C" void* func_00231CF0(void* self);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232CF8__FPv);
#ifdef SKIP_ASM
void* func_00232CF8(void* self)
{
    return func_00231CF0(self);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232D18);
#ifdef SKIP_ASM
void func_00231CB0(void*);
extern "C" void func_00398038(void*);

extern "C" void func_00232D18(void* self)
{
    func_00231CB0(self);
    func_00398038(*(void**)((char*)self + 0xC));
    (*(int*)((char*)self + 0x14))++;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232D50);
#ifdef SKIP_ASM
class cWorld232D50 {
public:
    char pad[0x10D8];
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
    virtual void v18();
    virtual void v19();
};
extern "C" void func_00397B70(void* obj, int a);
extern "C" void func_0036A020(void* world);
extern void* D_004A289C;

extern "C" void func_00232D50(void* self)
{
    ((cWorld232D50*)D_004A289C)->v19();
    void* p = *(void**)((char*)self + 0xC);
    if (p != 0) {
        func_00397B70(p, 3);
    }
    *(void**)((char*)self + 0xC) = 0;
    func_0036A020(D_004A289C);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232DA8);
#ifdef SKIP_ASM
class cWorld232DA8 {
public:
    char pad[0x10D8];
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
    virtual int v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
};
extern "C" int func_00231D60(void* self);
extern "C" void func_00397DF8(void* obj);
extern void* D_004A289C;

extern "C" int func_00232DA8(void* self)
{
    if (((cWorld232DA8*)D_004A289C)->v17() == 0) {
        return 0;
    }
    if (func_00231D60(self) == 0) {
        func_00397DF8(*(void**)((char*)self + 0xC));
    }
    ((cWorld232DA8*)D_004A289C)->v20();
    return 1;
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_00232E20);

INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_loadTexture);

INCLUDE_ASM("main/loadscreens_prestart", func_00233260);

extern "C" void* func_00231C70(void* self);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_002332C8__FPv);
#ifdef SKIP_ASM
int func_002332C8(void* self)
{
    return (func_00231C70(self) != 0);
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_002332E8);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00233390);
#ifdef SKIP_ASM
class cWorld233390 {
public:
    char pad[0x10D8];
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
    virtual int v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
};
extern "C" int func_00231D60(void* self);
extern "C" void func_00397DF8(void* obj);
extern void* D_004A289C;

extern "C" int func_00233390(void* self)
{
    if (((cWorld233390*)D_004A289C)->v17() == 0) {
        return 0;
    }
    if (func_00231D60(self) == 0) {
        func_00397DF8(*(void**)((char*)self + 0xC));
    }
    ((cWorld233390*)D_004A289C)->v20();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00233408);
#ifdef SKIP_ASM
extern "C" void func_00398038(void*);
void func_00231CB0(void*);

extern "C" void func_00233408(void* self)
{
    func_00398038(*(void**)((char*)self + 0xC));
    func_00231CB0(self);
}
#endif


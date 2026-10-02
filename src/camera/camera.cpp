#include "common.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

INCLUDE_ASM("camera/camera", cCamera_resetChaseControllerSwitches);

INCLUDE_ASM("camera/camera", cCamera_cCamera);

//100%
INCLUDE_ASM("camera/camera", func_0015DD88);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0045B888[];
extern void* D_0045B8B0[];
void func_0015CD60(void* p);
void func_00176DA8_2(void* self, int flags) __asm__("func_00176DA8__FPv");

class cCamPartK2 {
public:
    char pad[0x14];
    // vptr at 0x14
    virtual ~cCamPartK2();
};

// PORT: func_00176DA8__FPv is a destructor taking (self, flags); bind the 2-arg form.
extern "C" void func_0015DD88(void* self, int flags)
{
    *(void***)((char*)self + 0x90) = D_0045B888;
    func_0015CD60(*(cCamPartK2**)((char*)self + 0xA4));
    func_0015CD60(*(cCamPartK2**)((char*)self + 0xA8));
    func_0015CD60(*(cCamPartK2**)((char*)self + 0xAC));
    delete *(cCamPartK2**)((char*)self + 0xA4);
    delete *(cCamPartK2**)((char*)self + 0xA8);
    delete *(cCamPartK2**)((char*)self + 0xAC);
    func_00176DA8_2((char*)self + 0xC0, 2);
    *(void***)((char*)self + 0x90) = D_0045B8B0;
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("camera/camera", cCamera_init);

//100%
INCLUDE_ASM("camera/camera", func_0015DF98);
#ifdef SKIP_ASM
class cCamVObj_0015DF98 {
public:
    char pad[0x14];
    // vptr at 0x14
    virtual void v01();
    virtual void v02();
};

extern "C" void* func_0015E668(void* self);

extern "C" void func_0015DF98(void* self)
{
    (*(cCamVObj_0015DF98**)((char*)self + 0xA0))->v02();
    func_0015E668(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015DFD8);
#ifdef SKIP_ASM
extern "C" void func_0015DFD8(void* self, int mode)
{
    switch (mode) {
    case 1:
        *(int*)((char*)self + 0xA0) = *(int*)((char*)self + 0xA4);
        break;
    case 2:
        *(int*)((char*)self + 0xA0) = *(int*)((char*)self + 0xA8);
        break;
    case 3:
        *(int*)((char*)self + 0xA0) = *(int*)((char*)self + 0xAC);
        break;
    }
}
#endif

extern "C" void* func_00166F28(void*);

//100%
INCLUDE_ASM("camera/camera", func_0015E030__FPv);
#ifdef SKIP_ASM
void* func_0015E030(void* self)
{
    return func_00166F28((char*)self + 0xc0);
}
#endif

INCLUDE_ASM("camera/camera", func_0015E050);

INCLUDE_ASM("camera/camera", func_0015E2A8);

INCLUDE_ASM("camera/camera", func_0015E360);

INCLUDE_ASM("camera/camera", func_0015E460);

INCLUDE_ASM("camera/camera", func_0015E668);

INCLUDE_ASM("camera/camera", func_0015EC98);

//100%
INCLUDE_ASM("camera/camera", func_0015EDC8);
#ifdef SKIP_ASM
class cCamVObj_0015EDC8 {
public:
    char pad[0x14];
    // vptr at 0x14 (g++ 2.95 places it after the class's own data)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

extern "C" void func_0015EDC8(void* self, int value)
{
    char* obj = *(char**)((char*)self + 0xA8);
    *(int*)(*(char**)(obj + 0x20) + 0x4) = value;
    (*(cCamVObj_0015EDC8**)((char*)self + 0xA8))->v04();
}
#endif

INCLUDE_ASM("camera/camera", func_0015EE00);

//100%
INCLUDE_ASM("camera/camera", func_0015F568);
#ifdef SKIP_ASM
extern "C" void* func_0011FF48(void* dst, void* self);

extern "C" void* func_0015F568(void* self, void* a1)
{
    func_0011FF48(self, *(void**)((char*)a1 + 0x4));
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F598);
#ifdef SKIP_ASM
extern "C" void* func_0015F598(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x120);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F5B0);
#ifdef SKIP_ASM
struct sQuad_0015F5B0 {
    float v[4];
} __attribute__((aligned(16)));

class cCamVObj_0015F5B0 {
public:
    // vptr at 0x0
    virtual void v01();
    virtual sQuad_0015F5B0* v02();
};

extern "C" void* func_0015F5B0(void* self, void* a1)
{
    char* p = *(char**)((char*)a1 + 0x4);
    *(sQuad_0015F5B0*)self = *((cCamVObj_0015F5B0*)(p + 0x6C0))->v02();
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F5F8);
#ifdef SKIP_ASM
extern "C" void* func_0015F5F8(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x1b0);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F610);
#ifdef SKIP_ASM
extern "C" void* func_0015F610(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x1a0);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F628);
#ifdef SKIP_ASM
extern "C" void* func_0015F628(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x1c0);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F640__FPv);
#ifdef SKIP_ASM
float func_0015F640(void* self)
{
    return *(float*)((char*)*(void**)((char*)self + 0x4) + 0x1f0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F650);
#ifdef SKIP_ASM
extern "C" void* func_0015F650(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x370);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F668);
#ifdef SKIP_ASM
extern "C" void* func_0015F668(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x4);
    *(cQuad128*)self = *(cQuad128*)((char*)p + 0x380);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F680__FPv);
#ifdef SKIP_ASM
void* func_0015F680(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x77c) + 0x130;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F690__FPv);
#ifdef SKIP_ASM
void* func_0015F690(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x77c) + 0x140;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6A0__FPv);
#ifdef SKIP_ASM
float func_0015F6A0(void* self)
{
    return *(float*)((char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x788) + 0x98);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6B0);
#ifdef SKIP_ASM
extern "C" float func_0015F6B0(void* self)
{
    void* p = *(void**)((char*)self + 0x4);
    void* p2 = *(void**)((char*)p + 0x788);
    return *(float*)((char*)p2 + 0x98) - *(float*)((char*)p2 + 0xa0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6C8);
#ifdef SKIP_ASM
extern "C" float func_0015F6C8(void* self)
{
    void* p = *(void**)((char*)self + 0x4);
    void* p2 = *(void**)((char*)p + 0x788);
    return *(float*)((char*)p2 + 0x9c) - *(float*)((char*)p2 + 0xa0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6E0__FPv);
#ifdef SKIP_ASM
void* func_0015F6E0(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x788) + 0x40;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F6F0__FPv);
#ifdef SKIP_ASM
void* func_0015F6F0(void* self)
{
    return (char*)*(void**)((char*)self + 0x4) + 0x3c0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F700__FPv);
#ifdef SKIP_ASM
float func_0015F700(void* self)
{
    return *(float*)((char*)*(void**)((char*)self + 0x4) + 0x2fc);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F710__FPv);
#ifdef SKIP_ASM
float func_0015F710(void* self)
{
    return *(float*)((char*)*(void**)((char*)self + 0x4) + 0x220);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F720);
#ifdef SKIP_ASM
extern "C" int func_0015F720(void* self)
{
    void* a = *(void**)((char*)self + 0x4);
    void* b = *(void**)((char*)a + 0x788);
    int s = *(int*)((char*)b + 0xAC);
    int r = 0;
    if (s == 1 || s == 3) {
        r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F750__FPv);
#ifdef SKIP_ASM
int func_0015F750(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x4) + 0x438);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F760__FPv);
#ifdef SKIP_ASM
float func_0015F760(void* self)
{
    return *(float*)((char*)*(void**)((char*)self + 0x4) + 0x5a4);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F770__FPv);
#ifdef SKIP_ASM
int func_0015F770(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x4) + 0x5a8);
}
#endif

INCLUDE_ASM("camera/camera", func_0015F780);

//100%
INCLUDE_ASM("camera/camera", func_0015F908__FPv);
#ifdef SKIP_ASM
int func_0015F908(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x4) + 0x5ac);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F918__FPv);
#ifdef SKIP_ASM
void* func_0015F918(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x788) + 0x20;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015F928__FPv);
#ifdef SKIP_ASM
void* func_0015F928(void* self)
{
    return (char*)*(void**)((char*)*(void**)((char*)self + 0x4) + 0x788) + 0x10;
}
#endif

INCLUDE_ASM("camera/camera", func_0015F938);

INCLUDE_ASM("camera/camera", func_0015F9B8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", func_0015FD08);
#ifdef SKIP_ASM
extern "C" void* func_0015F938(void* self);
extern void* D_0045BD10[];

extern "C" void* func_0015FD08(void* self)
{
    func_0015F938(self);
    *(int*)((char*)self + 0xC) = 0x51;
    *(void***)((char*)self + 0x10) = D_0045BD10;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015FD48__FPv);
#ifdef SKIP_ASM
void func_0015FD48(void* self)
{
}
#endif

INCLUDE_ASM("camera/camera", func_0015FD50);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", func_0015FFB0);
#ifdef SKIP_ASM
extern "C" void* func_0015F938(void* self);
extern void* D_0045BCA8[];

extern "C" void* func_0015FFB0(void* self)
{
    func_0015F938(self);
    *(int*)((char*)self + 0xC) = 0x50;
    *(void***)((char*)self + 0x10) = D_0045BCA8;
    return self;
}
#endif

INCLUDE_ASM("camera/camera", func_0015FFF0);

INCLUDE_ASM("camera/camera", func_00160028);

INCLUDE_ASM("camera/camera", func_00160130);

INCLUDE_ASM("camera/camera", func_00160228);

INCLUDE_ASM("camera/camera", func_001603F0);

//100%
INCLUDE_ASM("camera/camera", func_00160438);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045BAD0[];

extern "C" void* func_00160438(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0xB;
    *(void***)((char*)self + 0x10) = D_0045BAD0;
    return self;
}
#endif

extern void* D_0045BAD0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00160478__FPv);
#ifdef SKIP_ASM
void* func_00160478(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045BAD0;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001604A0__FPv);
#ifdef SKIP_ASM
float func_001604A0(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001604B0__FPvT0);
#ifdef SKIP_ASM
void func_001604B0(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

INCLUDE_ASM("camera/camera", func_001604B8);

//100%
INCLUDE_ASM("camera/camera", func_001606D8);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);

struct sCamVEntryIntA {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sCamVEntryVoidA {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_001606D8(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        func_00166550(self, 100.0f, 100.0f);
        sCamVEntryVoidA* vt2 = *(sCamVEntryVoidA**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160760);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045BA58[];

extern "C" void* func_00160760(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x29;
    *(void***)((char*)self + 0x10) = D_0045BA58;
    *(int*)((char*)self + 0x390) = 0;
    return self;
}
#endif

extern void* D_0045BA58[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_001607A0__FPv);
#ifdef SKIP_ASM
void* func_001607A0(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045BA58;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001607C8__FPv);
#ifdef SKIP_ASM
float func_001607C8(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001607D8__FPvT0);
#ifdef SKIP_ASM
void func_001607D8(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

INCLUDE_ASM("camera/camera", func_001607E0);

//100%
INCLUDE_ASM("camera/camera", func_00160AE8);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);

extern "C" void func_00160AE8(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        func_00166550(self, 100.0f, 100.0f);
        sCamVEntryVoidA* vt2 = *(sCamVEntryVoidA**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160B70);
#ifdef SKIP_ASM
extern void* D_0045B9E0[];
extern "C" void* func_00162318(void* self);

extern "C" void* func_00160B70(void* self, float f)
{
    func_00162318(self);
    *(float*)((char*)self + 0x390) = f;
    *(void***)((char*)self + 0x10) = D_0045B9E0;
    *(int*)((char*)self + 0x394) = 0;
    return self;
}
#endif

extern void* D_0045B9E0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00160BB8__FPv);
#ifdef SKIP_ASM
void* func_00160BB8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045B9E0;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160BE0__FPv);
#ifdef SKIP_ASM
float func_00160BE0(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00160BF0__FPvT0);
#ifdef SKIP_ASM
void func_00160BF0(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

INCLUDE_ASM("camera/camera", func_00160BF8);

//100%
INCLUDE_ASM("camera/camera", func_00160FD0);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);

extern "C" void func_00160FD0(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        func_00166550(self, 100.0f, 100.0f);
        *(int*)((char*)self + 0x394) = 0;
        sCamVEntryVoidA* vt2 = *(sCamVEntryVoidA**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

INCLUDE_ASM("camera/camera", func_00161060);

//100%
INCLUDE_ASM("camera/camera", func_00161370);
#ifdef SKIP_ASM
extern void* D_0045B968[];
extern "C" void func_0031D618(void* p, int flags);
// PORT: func_00162458 is the base deleting dtor (self, flags); the unit declares it with one arg.
void func_00162458_dtor(void* self, int flags) __asm__("func_00162458");

extern "C" void func_00161370(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045B968;
    func_0031D618((char*)self + 0x3A8, 2);
    func_0031D618((char*)self + 0x39C, 2);
    func_0031D618((char*)self + 0x390, 2);
    func_00162458_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001613D8__FPv);
#ifdef SKIP_ASM
float func_001613D8(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001613E8__FPvT0);
#ifdef SKIP_ASM
void func_001613E8(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

INCLUDE_ASM("camera/camera", func_001613F0);

//100%
INCLUDE_ASM("camera/camera", func_00161630);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);

struct sCamVEntryInt {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sCamVEntryVoid {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00161630(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryInt* vt = *(sCamVEntryInt**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        sCamVEntryVoid* vt2 = *(sCamVEntryVoid**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001616A8);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045BB48[];

extern "C" void* func_001616A8(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x7;
    *(void***)((char*)self + 0x10) = D_0045BB48;
    return self;
}
#endif

extern void* D_0045BB48[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_001616E8__FPv);
#ifdef SKIP_ASM
void* func_001616E8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045BB48;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161710__FPv);
#ifdef SKIP_ASM
float func_00161710(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161720__FPvT0);
#ifdef SKIP_ASM
void func_00161720(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

INCLUDE_ASM("camera/camera", func_00161728);

INCLUDE_ASM("camera/camera", func_00161848);

INCLUDE_ASM("camera/camera", func_00161950);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", func_001619A8);
#ifdef SKIP_ASM
extern void* D_0045B908[];
extern "C" void* cCameraController_cCameraController(void* self, int a);
extern "C" void func_00161950(void* self, int idx);

extern "C" void* func_001619A8(void* self, int idx, int a)
{
    cCameraController_cCameraController(self, a);
    *(void***)((char*)self + 0x14) = D_0045B908;
    *(int*)((char*)self + 0x0) = 2;
    *(int*)((char*)self + 0x4) = 1;
    *(int*)((char*)self + 0x20) = 0;
    func_00161950(self, idx);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161A10);
#ifdef SKIP_ASM
extern void* D_0045B908[];
void operator_delete(int* ptr);
extern "C" void func_0015CC10(void* self, int flags);

extern "C" void func_00161A10(void* self, int flags)
{
    *(void***)((char*)self + 0x14) = D_0045B908;
    operator_delete(*(int**)((char*)self + 0x20));
    func_0015CC10(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161A60);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00161A60(void* self, int* p)
{
    int* old = *(int**)((char*)self + 0x20);
    if (old != 0) {
        operator_delete(old);
    }
    *(int**)((char*)self + 0x20) = p;
}
#endif

INCLUDE_ASM("camera/camera", func_00161AB0);

INCLUDE_ASM("camera/camera", func_00161BB8);

//100%
INCLUDE_ASM("camera/camera", func_00161E58);
#ifdef SKIP_ASM
extern "C" void cCameraAlgoList_insert(void* list, void* algo, int b, float w);
extern "C" void func_0015CB08(void* list);

extern "C" void func_00161E58(void* self, void* algo, float w)
{
    *(int*)((char*)self + 0x18) = *(int*)((char*)algo + 0xC);
    void* list = *(void**)((char*)self + 0x8);
    if (*(int*)((char*)list + 0x4) == 0) {
        cCameraAlgoList_insert(list, algo, 1, 1.0f);
        return;
    }
    if (w == 1.0f) {
        func_0015CB08(*(void**)((char*)self + 0x8));
    }
    cCameraAlgoList_insert(*(void**)((char*)self + 0x8), algo, 1, w);
}
#endif

INCLUDE_ASM("camera/camera", func_00161EF0);

//100%
INCLUDE_ASM("camera/camera", func_00161F50);
#ifdef SKIP_ASM
struct sCam161F50 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int pad_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void* func_0015D050(void*, int, int);

extern "C" void func_00161F50(sCam161F50* self)
{
    int mode;
    self->b1 = 0;
    self->b0 = 0;
    mode = 0x42;
    if (!self->b2) {
        mode = self->field_0x24;
    }
    func_0015D050(self, mode, self->field_0x34);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00161FA0);
#ifdef SKIP_ASM
extern "C" void func_00161FA0(void* self, int mode)
{
    int flags = *(int*)((char*)self + 0x30) | 2;
    *(int*)((char*)self + 0x30) = flags;
    if (mode == 0x4C) {
        mode = *(int*)((char*)self + 0x24);
        func_0015D050(self, mode, 0);
    } else if (mode == 0x5D) {
        if (flags & 1)
            func_0015D050(self, *(int*)((char*)self + 0x28), *(int*)((char*)self + 0x34));
        else if (flags & 4)
            func_0015D050(self, 0x42, 0);
        else
            func_0015D050(self, *(int*)((char*)self + 0x24), 0);
    } else {
        func_0015D050(self, mode, 0);
    }
    *(int*)((char*)self + 0x2C) = mode;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162060);
#ifdef SKIP_ASM
struct sCam162060 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int field_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void* cChaseCameraController_createChaseAlgorithmBlend(void*, int, int, float);

extern "C" void func_00162060(sCam162060* self, int mode, int arg, float blend)
{
    if (mode == 0x4C) {
        self->b0 = 0;
        mode = self->field_0x24;
    } else {
        self->field_0x28 = mode;
        self->b0 = 1;
        self->field_0x34 = arg;
    }
    int ok = !self->b1 ? 1 : self->field_0x2C == 0x5D;
    if (ok) {
        cChaseCameraController_createChaseAlgorithmBlend(self, mode, arg, blend);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001620D0);
#ifdef SKIP_ASM
struct sCam1620D0 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int field_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void func_0015DAC0();
extern "C" void* cChaseCameraController_createChaseAlgorithmBlend(void*, int, int, float);

extern "C" void func_001620D0(sCam1620D0* self)
{
    int mode = self->field_0x2C;
    self->b0 = 0;
    if (mode == 0x5D)
        mode = self->field_0x24;
    func_0015DAC0();
    cChaseCameraController_createChaseAlgorithmBlend(self, mode, 0, 1.0f);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162138);
#ifdef SKIP_ASM
struct sCam162138 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int pad_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
    int pad_0x38;
    int field_0x3C;
    int field_0x40;
};


extern "C" void func_00162138(sCam162138* self)
{
    self->b1 = 0;
    self->b0 = 0;
    self->b2 = 0;
    self->field_0x28 = self->field_0x24;
    self->field_0x3C = 0;
    self->field_0x40 = 0;
    self->field_0x34 = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162170);
#ifdef SKIP_ASM
extern "C" void* func_0015D050(void*, int, int);

extern "C" void* func_00162170(sCam162138* self)
{
    func_00162138(self);
    return func_0015D050(self, self->field_0x24, 0);
}
#endif

INCLUDE_ASM("camera/camera", func_001621A8);

INCLUDE_ASM("camera/camera", func_00162218);

INCLUDE_ASM("camera/camera", func_00162258);

extern "C" void* func_0015D050(void*, int, int);

//100%
INCLUDE_ASM("camera/camera", func_00162290__FPv);
#ifdef SKIP_ASM
void* func_00162290(void* self)
{
    return func_0015D050(self, *(int*)((char*)self + 0x24), 0);
}
#endif

INCLUDE_ASM("camera/camera", func_001622B0);

//100%
INCLUDE_ASM("camera/camera", func_00162310__FPvi);
#ifdef SKIP_ASM
void func_00162310(void* self, int val)
{
    *(int*)((char*)self + 0x30) = val;
}
#endif

INCLUDE_ASM("camera/camera", func_00162318);

//100%
INCLUDE_ASM("camera/camera", func_00162458);
#ifdef SKIP_ASM
extern void* D_0045BBC0[];
extern void* D_0045BDE0[];

// PORT: func_00162458 is the base deleting dtor (self, flags); the unit declares it with one arg
// (func_00162458_dtor is declared earlier in the unit with this asm label).
void func_00162458_dtor(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BBC0;
    func_0031D618((char*)self + 0x334, 2);
    func_0031D618((char*)self + 0x328, 2);
    func_0031D618((char*)self + 0x31C, 2);
    func_0031D618((char*)self + 0x310, 2);
    func_0031D618((char*)self + 0x304, 2);
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001624E8);
#ifdef SKIP_ASM
struct sCamVE1624E8 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sCamVE1624E8i {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001624E8(void* self, int a1)
{
    if (*(int*)((char*)self + 0x2F0) != 0) {
        sCamVE1624E8* vt = *(sCamVE1624E8**)((char*)self + 0x10);
        vt[4].fn((char*)self + vt[4].delta);
    }
    sCamVE1624E8* vt = *(sCamVE1624E8**)((char*)self + 0x10);
    vt[5].fn((char*)self + vt[5].delta);
    sCamVE1624E8i* vt2 = *(sCamVE1624E8i**)((char*)self + 0x10);
    vt2[8].fn((char*)self + vt2[8].delta, a1);
}
#endif

INCLUDE_ASM("camera/camera", func_00162568);

INCLUDE_ASM("camera/camera", func_00162998);

INCLUDE_ASM("camera/camera", func_00162A20);

//100%
INCLUDE_ASM("camera/camera", func_00162B80);
#ifdef SKIP_ASM
extern "C" cQuad128 func_00162B80(void* self, void* a1)
{
    cQuad128 v = *(cQuad128*)((char*)a1 + 0x10);
    *(cQuad128*)((char*)self + 0x20) = v;
    return v;
}
#endif

INCLUDE_ASM("camera/camera", func_00162B90);

INCLUDE_ASM("camera/camera", func_00162C78);

INCLUDE_ASM("camera/camera", func_00163010);

INCLUDE_ASM("camera/camera", func_00163158);

INCLUDE_ASM("camera/camera", func_00163270);

INCLUDE_ASM("camera/camera", func_001633B0);

INCLUDE_ASM("camera/camera", func_00163450);

INCLUDE_ASM("camera/camera", func_001635F8);

INCLUDE_ASM("camera/camera", func_001641C0);

INCLUDE_ASM("camera/camera", func_001643A8);

INCLUDE_ASM("camera/camera", func_001646A0);

INCLUDE_ASM("camera/camera", func_00164878);

INCLUDE_ASM("camera/camera", func_00165540);

INCLUDE_ASM("camera/camera", func_001656B0);

INCLUDE_ASM("camera/camera", func_00165938);

//100%
INCLUDE_ASM("camera/camera", func_00166228);
#ifdef SKIP_ASM
struct sCamTarget166228 {
    float x, y, z, w;
    int valid; // 0x10
};

struct sCamVE166228 {
    short delta;
    short index;
    void (*fn)(void*, sCamTarget166228*);
};

extern "C" void func_00166640(void* self, int a1);
extern "C" void func_001662A0(void* self, float x, float y, float z, float w);
void* func_00166530(void* self);
extern "C" void func_00166F90(void* self);

extern "C" void func_00166228(void* self)
{
    sCamTarget166228 t;
    sCamVE166228* vt = *(sCamVE166228**)((char*)self + 0x10);
    vt[7].fn((char*)self + vt[7].delta, &t);
    func_00166640(self, 0);
    if (t.valid != 0)
        func_001662A0(self, t.x, t.y, t.z, t.w);
    func_00166530(self);
    func_00166F90(self);
}
#endif

INCLUDE_ASM("camera/camera", func_001662A0);

extern "C" void* func_00168150(void* self);

//100%
INCLUDE_ASM("camera/camera", func_00166530__FPv);
#ifdef SKIP_ASM
void* func_00166530(void* self)
{
    return func_00168150(self);
}
#endif

INCLUDE_ASM("camera/camera", func_00166550);

INCLUDE_ASM("camera/camera", func_00166640);

INCLUDE_ASM("camera/camera", func_001668B8);

INCLUDE_ASM("camera/camera", func_00166C60);

//100%
INCLUDE_ASM("camera/camera", func_00166F28);
#ifdef SKIP_ASM
struct sVE166F28a {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sVE166F28b {
    short delta;
    short index;
    void* (*fn)(void*, void*);
};

extern "C" void* func_00166F28(void* self)
{
    sVE166F28b* vt = *(sVE166F28b**)((char*)self + 0x10);
    char* o = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sVE166F28a* vt2 = *(sVE166F28a**)o;
    return vt[3].fn((char*)self + vt[3].delta, vt2[7].fn(o + vt2[7].delta));
}
#endif

INCLUDE_ASM("camera/camera", func_00166F90);

//100%
INCLUDE_ASM("camera/camera", func_001673A0);
#ifdef SKIP_ASM
extern "C" void* func_001673A0(void* self, void* a1)
{
    *(cQuad128*)self = *(cQuad128*)((char*)a1 + 0x60);
    *(cQuad128*)((char*)self + 0x10) = *(cQuad128*)((char*)a1 + 0x70);
    return self;
}
#endif

INCLUDE_ASM("camera/camera", func_001673F8);

INCLUDE_ASM("camera/camera", func_00167D88);

INCLUDE_ASM("camera/camera", func_00167D98);

INCLUDE_ASM("camera/camera", func_00167DA8);

//100%
INCLUDE_ASM("camera/camera", func_00167DB8);
#ifdef SKIP_ASM
struct sCamFilter {
    float field_0x00[5];
    float field_0x14[5];
    int field_0x28[5];
    int field_0x3C;
};

extern "C" void func_00167DB8(sCamFilter* self, float value)
{
    int i;
    self->field_0x3C = 0;
    for (i = 0; i < 5; i++) {
        self->field_0x00[i] = value;
        self->field_0x14[i] = value;
        self->field_0x28[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167DE8);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167DE8(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E18__FPv);
#ifdef SKIP_ASM
void func_00167E18(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E20__FPv);
#ifdef SKIP_ASM
void func_00167E20(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E28__FPv);
#ifdef SKIP_ASM
void func_00167E28(void* self)
{
}
#endif

INCLUDE_ASM("camera/camera", func_00167E30);

extern cQuad128 D_004FF120;

//100%
INCLUDE_ASM("camera/camera", func_00167E40);
#ifdef SKIP_ASM
extern "C" void* func_00167E40(void* self)
{
    *(cQuad128*)self = D_004FF120;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E58__FPv);
#ifdef SKIP_ASM
void func_00167E58(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E60__FPv);
#ifdef SKIP_ASM
void func_00167E60(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E68);
#ifdef SKIP_ASM
class func_00167E68_cObj {
public:
    char pad[0x10];
    // vptr lands at 0x10 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

extern "C" void func_00167E68(func_00167E68_cObj* self)
{
    self->v05();
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E90__FPv);
#ifdef SKIP_ASM
void func_00167E90(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167E98__FPv);
#ifdef SKIP_ASM
int func_00167E98(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167EA0);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167EA0(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167F10);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167F10(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167F40);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167F40(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167F70);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00167F70(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167FA0);
#ifdef SKIP_ASM
extern "C" void* func_00167FA0(void* self, void* a1)
{
    *(cQuad128*)self = *(cQuad128*)((char*)a1 + 0x20);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167FB0__FPv);
#ifdef SKIP_ASM
int func_00167FB0(void* self)
{
    return *(int*)((char*)self + 0x2F0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00167FB8__FPv);
#ifdef SKIP_ASM
void func_00167FB8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168030);
#ifdef SKIP_ASM
extern void* D_0045B8B0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00168030(void* self, int flags)
{
    *(void***)((char*)self + 0x90) = D_0045B8B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* func_0015D928(void* self);

//100%
INCLUDE_ASM("camera/camera", func_00168060__FPv);
#ifdef SKIP_ASM
void* func_00168060(void* self)
{
    return func_0015D928(self);
}
#endif

INCLUDE_ASM("camera/camera", func_00168150);

extern "C" void* func_001673F8(int, int);

//99.38%
INCLUDE_ASM("camera/camera", func_00168298__FPv);
#ifdef SKIP_ASM
void* func_00168298(void* self)
{
    return func_001673F8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001682B8);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C628[];

extern "C" void* func_001682B8(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0xD;
    *(void***)((char*)self + 0x10) = D_0045C628;
    return self;
}
#endif

extern void* D_0045C628[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_001682F8__FPv);
#ifdef SKIP_ASM
void* func_001682F8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C628;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168320__FPv);
#ifdef SKIP_ASM
float func_00168320(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00168330__FPvT0);
#ifdef SKIP_ASM
void func_00168330(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

INCLUDE_ASM("camera/camera", func_00168338);

INCLUDE_ASM("camera/camera", func_00168508);

//100%
INCLUDE_ASM("camera/camera", func_00168650);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C6A0[];

extern "C" void* func_00168650(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x20;
    *(void***)((char*)self + 0x10) = D_0045C6A0;
    return self;
}
#endif

extern void* D_0045C6A0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00168690__FPv);
#ifdef SKIP_ASM
void* func_00168690(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C6A0;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001686B8__FPv);
#ifdef SKIP_ASM
float func_001686B8(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001686C8__FPvT0);
#ifdef SKIP_ASM
void func_001686C8(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

INCLUDE_ASM("camera/camera", func_001686D0);

INCLUDE_ASM("camera/camera", func_00168940);

//100%
INCLUDE_ASM("camera/camera", func_001689C8);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C538[];

extern "C" void* func_001689C8(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x9;
    *(void***)((char*)self + 0x10) = D_0045C538;
    return self;
}
#endif

extern void* D_0045C538[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00168A08__FPv);
#ifdef SKIP_ASM
void* func_00168A08(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C538;
    return func_00162458(self);
}
#endif

INCLUDE_ASM("camera/camera", func_00168A30);

INCLUDE_ASM("camera/camera", func_00168A40);

INCLUDE_ASM("camera/camera", func_00168A70);

INCLUDE_ASM("camera/camera", func_00168C50);

//100%
INCLUDE_ASM("camera/camera", func_00168CD8);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C5B0[];

extern "C" void* func_00168CD8(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x9;
    *(void***)((char*)self + 0x10) = D_0045C5B0;
    return self;
}
#endif

extern void* D_0045C5B0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/camera", func_00168D18__FPv);
#ifdef SKIP_ASM
void* func_00168D18(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C5B0;
    return func_00162458(self);
}
#endif

INCLUDE_ASM("camera/camera", func_00168D40);

INCLUDE_ASM("camera/camera", func_00168D50);

INCLUDE_ASM("camera/camera", func_00168D80);

INCLUDE_ASM("camera/camera", func_00168F60);


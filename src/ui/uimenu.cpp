#include "common.h"

INCLUDE_ASM("ui/uimenu", cUIMenu_setSelected);

INCLUDE_ASM("ui/uimenu", func_0039AE98);

INCLUDE_ASM("ui/uimenu", func_0039B000);

INCLUDE_ASM("ui/uimenu", func_0039B6A0);

//100%
INCLUDE_ASM("ui/uimenu", func_0039B760);
#ifdef SKIP_ASM
extern "C" char func_003979C0(void*);

extern "C" void func_0039B760(void* self, char a1)
{
    *(char*)((char*)self + 0x95) = 0;
    *(char*)((char*)self + 0x96) = a1;
    *(int*)((char*)self + 0x90) |= 4;
    *(char*)((char*)self + 0x97) = func_003979C0((char*)self + 0x74);
    *(char*)((char*)self + 0x98) = 0;
}
#endif

INCLUDE_ASM("ui/uimenu", func_0039B7B0);

INCLUDE_ASM("ui/uimenu", cUIMenu_setSelectedByIndex);

extern "C" void* func_0039FE00(void* self);

//100%
INCLUDE_ASM("ui/uimenu", func_0039BAB0__FPv);
#ifdef SKIP_ASM
void* func_0039BAB0(void* self)
{
    return func_0039FE00(self);
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039BAD0);
#ifdef SKIP_ASM
class func_0039BAD0_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(int);
};

struct func_0039BAD0_s {
    char pad0[0x14];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    char pad18[0x78 - 0x18];
    func_0039BAD0_cVirt* a;
    func_0039BAD0_cVirt* b;
};

extern "C" void func_0039BAD0(func_0039BAD0_s* self, int on)
{
    self->b1 = (on != 0);
    self->a->v04(on);
    func_0039BAD0_cVirt* b = self->b;
    if (b != 0) {
        b->v04(on);
    }
}
#endif

INCLUDE_ASM("ui/uimenu", func_0039BB50);

INCLUDE_ASM("ui/uimenu", func_0039BBD8);

//100%
INCLUDE_ASM("ui/uimenu", func_0039BD38);
#ifdef SKIP_ASM
class func_0039BD38_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

extern "C" int func_0039BD38(void* a, func_0039BD38_cVirt* obj)
{
    return obj->v02(a);
}
#endif

INCLUDE_ASM("ui/uimenu", func_0039BD68);

//100%
INCLUDE_ASM("ui/uimenu", func_0039BE10);
#ifdef SKIP_ASM
class func_0039BE10_cVirt {
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
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
};

extern "C" void func_0039BE10(void* self)
{
    func_0039BE10_cVirt* a = *(func_0039BE10_cVirt**)((char*)self + 0x78);
    if (a != 0) {
        a->v16();
    }
    func_0039BE10_cVirt* b = *(func_0039BE10_cVirt**)((char*)self + 0x7C);
    if (b != 0) {
        b->v16();
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039BE70);
#ifdef SKIP_ASM
class func_0039BE70_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01(int);
};

extern "C" void func_0039BE70(void* self)
{
    func_0039BE70_cVirt* a = *(func_0039BE70_cVirt**)((char*)self + 0x78);
    if (a != 0) {
        a->v01(3);
        *(func_0039BE70_cVirt**)((char*)self + 0x78) = 0;
    }
    func_0039BE70_cVirt* b = *(func_0039BE70_cVirt**)((char*)self + 0x7C);
    if (b != 0) {
        b->v01(3);
        *(func_0039BE70_cVirt**)((char*)self + 0x7C) = 0;
    }
}
#endif

INCLUDE_ASM("ui/uimenu", func_0039BED8);

INCLUDE_ASM("ui/uimenu", func_0039C130);

//100%
INCLUDE_ASM("ui/uimenu", func_0039C1C8);
#ifdef SKIP_ASM
extern "C" void func_003A0000(void* self, unsigned short ev);

class func_0039C1C8_cVirt {
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
    virtual void v19(unsigned short);
};

extern "C" void func_0039C1C8(void* self, unsigned short ev)
{
    func_003A0000(self, ev);
    func_0039C1C8_cVirt* a = *(func_0039C1C8_cVirt**)((char*)self + 0x78);
    if (a != 0) {
        a->v19(ev);
    }
    func_0039C1C8_cVirt* b = *(func_0039C1C8_cVirt**)((char*)self + 0x7C);
    if (b != 0) {
        b->v19(ev);
    }
}
#endif

INCLUDE_ASM("ui/uimenu", func_0039C240);

//100%
INCLUDE_ASM("ui/uimenu", func_0039C2C8);
#ifdef SKIP_ASM
extern "C" void func_0039C2C8(void* self)
{
    func_0039FE00(self);
    *(int*)((char*)self + 0x80) = 10;
    *(int*)((char*)self + 0x84) = 100;
}
#endif

INCLUDE_ASM("ui/uimenu", func_0039C300);

INCLUDE_ASM("ui/uimenu", func_0039C398);

INCLUDE_ASM("ui/uimenu", func_0039C428);


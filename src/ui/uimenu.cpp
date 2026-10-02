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

INCLUDE_ASM("ui/uimenu", func_0039BAD0);

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

INCLUDE_ASM("ui/uimenu", func_0039BE10);

INCLUDE_ASM("ui/uimenu", func_0039BE70);

INCLUDE_ASM("ui/uimenu", func_0039BED8);

INCLUDE_ASM("ui/uimenu", func_0039C130);

INCLUDE_ASM("ui/uimenu", func_0039C1C8);

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


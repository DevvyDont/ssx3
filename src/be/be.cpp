#include "common.h"

INCLUDE_ASM("be/be", cBE_getBE);

struct sBEInterfaceVTable {
    char pad_0x00[0x8];
    short field_0x8;
    char pad_0xA[2];
    void* (*fn)(void*);
};

struct sBEInterface {
    char pad_0x00[0x4];
    sBEInterfaceVTable* vtable;
};

extern sBEInterface* D_005308A8[];
extern int D_004A11F4;

//95.88%
INCLUDE_ASM("be/be", cBE_getInterface__Fv);
#ifdef SKIP_ASM
void* cBE_getInterface()
{
    sBEInterface* iface = D_005308A8[D_004A11F4];
    return iface->vtable->fn((char*)iface + iface->vtable->field_0x8);
}
#endif

INCLUDE_ASM("be/be", func_0014DD98);

INCLUDE_ASM("be/be", func_0014DE28);

INCLUDE_ASM("be/be", cBE_setState);

INCLUDE_ASM("be/be", func_0014DF08);

INCLUDE_ASM("be/be", cBE_readFromReplayFrame);

//100%
INCLUDE_ASM("be/be", func_0014E048);
#ifdef SKIP_ASM
extern "C" int func_0014E048()
{
    return 0xBB78;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/be", func_0014E050);
#ifdef SKIP_ASM
extern "C" void func_0014A188(void* buf);
// PORT: func_0014E048 is declared () in this unit but called with the BE object here.
int func_0014E048_self(void* be) __asm__("func_0014E048");
extern "C" int func_003E62D0(void* buf, int size, int seed);
extern "C" void* memcpy(void*, const void*, unsigned int);

extern "C" void func_0014E050(void* self, char* buf)
{
    func_0014A188(buf);
    int crc = func_003E62D0(buf, func_0014E048_self(self) - 4, 0xFBEA);
    char* p = buf + (func_0014E048_self(self) - 4);
    memcpy(p, &crc, 4);
}
#endif

extern "C" void* func_0014A5E0(int);

//100%
INCLUDE_ASM("be/be", func_0014E0C0__FPvi);
#ifdef SKIP_ASM
void* func_0014E0C0(void* self, int a1)
{
    return func_0014A5E0(a1);
}
#endif


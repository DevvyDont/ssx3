#include "common.h"

INCLUDE_ASM("fe/festatecharequip", cFE_cFE);

INCLUDE_ASM("fe/festatecharequip", func_001957D0);

//100%
INCLUDE_ASM("fe/festatecharequip", func_00195880);
#ifdef SKIP_ASM
extern void* D_0046BF98[];
extern void* D_0046D848[];
extern "C" void* func_001DCC40(void* p);
extern "C" void func_00416210(void* dst, int value, int size);

extern "C" void* func_00195880(void* self)
{
    *(char*)((char*)self + 0x0) = 0;
    *(char*)((char*)self + 0x1) = 0;
    *(char*)((char*)self + 0x2) = 0;
    *(void***)((char*)self + 0x4) = D_0046D848;
    func_001DCC40((char*)self + 3);
    *(char*)((char*)self + 0x58) = 0;
    *(void***)((char*)self + 0x4) = D_0046BF98;
    *(char*)((char*)self + 0x2) = 4;
    func_00416210((char*)self + 8, 0, 0x50);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", func_001958F0);
#ifdef SKIP_ASM
extern void* D_0046BF98[];
extern void* D_0046D848[];
extern "C" void func_00195948(void* self);
void operator_delete(int* p);

extern "C" void func_001958F0(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_0046BF98;
    func_00195948(self);
    *(void***)((char*)self + 0x4) = D_0046D848;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", func_00195948);
#ifdef SKIP_ASM
extern "C" void func_00195A50(void* self, int i);

extern "C" void func_00195948(void* self)
{
    signed char i;
    for (i = 0; i < 20; i++) {
        func_00195A50(self, i);
    }
}
#endif


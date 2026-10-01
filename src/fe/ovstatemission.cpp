#include "common.h"

INCLUDE_ASM("fe/ovstatemission", cFEStateMPCircuit_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatemission", func_001A3300__FPv);
#ifdef SKIP_ASM
void func_001A3300(void* self)
{
}
#endif

INCLUDE_ASM("fe/ovstatemission", func_001A3308);

INCLUDE_ASM("fe/ovstatemission", func_001A33A0);

//100%
INCLUDE_ASM("fe/ovstatemission", func_001A34A0__FPv);
#ifdef SKIP_ASM
void func_001A34A0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemission", func_001A34A8__FPv);
#ifdef SKIP_ASM
void func_001A34A8(void* self)
{
}
#endif

INCLUDE_ASM("fe/ovstatemission", func_001A3590);

INCLUDE_ASM("fe/ovstatemission", func_001A35E8);

//100%
INCLUDE_ASM("fe/ovstatemission", func_001A3648);
#ifdef SKIP_ASM
struct sMissionSlot {
    char name[0x100];
    int f100;
    int f104;
    int f108;
    int f10C;
    int f110;
    int f114;
    int f118;
};

extern unsigned char D_004A1408[];

extern "C" void func_001A3648(sMissionSlot* self)
{
    int i;
    for (i = 0; i < 5; i++) {
        unsigned char c = D_004A1408[0];
        self[i].f100 = 0;
        self[i].name[0] = c;
        self[i].f104 = -1;
        self[i].f108 = 1;
        self[i].f10C = 0;
        self[i].f110 = -1;
        self[i].f114 = 0;
        self[i].f118 = -1;
    }
}
#endif


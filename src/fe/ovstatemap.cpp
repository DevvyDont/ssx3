#include "common.h"

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatemap", func_002087E8__FPv);
#ifdef SKIP_ASM
void func_002087E8(void* self)
{
}
#endif

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onGainTransition);

INCLUDE_ASM("fe/ovstatemap", func_00208B00);

INCLUDE_ASM("fe/ovstatemap", func_00208B78);

INCLUDE_ASM("fe/ovstatemap", func_00208BB8);

INCLUDE_ASM("fe/ovstatemap", func_00208C28);

//100%
INCLUDE_ASM("fe/ovstatemap", func_00208EF8);
#ifdef SKIP_ASM
extern "C" int func_00208EF8(void* self, int a1, int a2)
{
    return ((unsigned int)(a2 - 8) < 2) ? 0 : 0x101;
}
#endif

INCLUDE_ASM("fe/ovstatemap", func_00208F10);

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupPopup);

INCLUDE_ASM("fe/ovstatemap", func_00209300);

INCLUDE_ASM("fe/ovstatemap", func_00209370);

//100%
INCLUDE_ASM("fe/ovstatemap", func_002095E8);
#ifdef SKIP_ASM
struct sVEntry_func_002095E8 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char D_004C8B38[];
extern char D_004C8B28[];

extern "C" void func_002095E8(void* self, void* obj, int on)
{
    if (on != 0) {
        sVEntry_func_002095E8* vt = *(sVEntry_func_002095E8**)((char*)obj + 0x8);
        vt[11].fn((char*)obj + vt[11].delta, D_004C8B38);
    } else {
        sVEntry_func_002095E8* vt = *(sVEntry_func_002095E8**)((char*)obj + 0x8);
        vt[11].fn((char*)obj + vt[11].delta, D_004C8B28);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_00209648);
#ifdef SKIP_ASM
struct sVEntry_func_00209648 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char D_004C8B58[];
extern char D_004C8B48[];

extern "C" void func_00209648(void* self, void* obj, int on)
{
    if (on != 0) {
        sVEntry_func_00209648* vt = *(sVEntry_func_00209648**)((char*)obj + 0x8);
        vt[11].fn((char*)obj + vt[11].delta, D_004C8B58);
    } else {
        sVEntry_func_00209648* vt = *(sVEntry_func_00209648**)((char*)obj + 0x8);
        vt[11].fn((char*)obj + vt[11].delta, D_004C8B48);
    }
}
#endif

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupPlayerIndicator);

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupLocalSessionList);

INCLUDE_ASM("fe/ovstatemap", func_00209E78);

INCLUDE_ASM("fe/ovstatemap", func_0020A088);

INCLUDE_ASM("fe/ovstatemap", func_0020A1A8);

INCLUDE_ASM("fe/ovstatemap", func_0020A380);

INCLUDE_ASM("fe/ovstatemap", func_0020A430);

INCLUDE_ASM("fe/ovstatemap", func_0020A4E0);

INCLUDE_ASM("fe/ovstatemap", func_0020A6A8);

INCLUDE_ASM("fe/ovstatemap", func_0020A6F0);


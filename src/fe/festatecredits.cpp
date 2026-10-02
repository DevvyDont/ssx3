#include "common.h"

INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onCreateScreen);

INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onGainFocus);

INCLUDE_ASM("fe/festatecredits", func_00185F40);

//100%
INCLUDE_ASM("fe/festatecredits", func_001863B8);
#ifdef SKIP_ASM
extern "C" void* func_0039E510(void* self);

extern "C" void func_001863B8(void* self)
{
    *(int*)((char*)self + 0x60) += *(int*)((char*)self + 0x64);
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/festatecredits", func_001863E8);

//100%
INCLUDE_ASM("fe/festatecredits", func_00186478);
#ifdef SKIP_ASM
extern "C" int func_00186478(void* self, int a1, unsigned int a2)
{
    switch (a2) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", func_001864B0);
#ifdef SKIP_ASM
struct sVEntry001864B0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void func_0039F400(void* list, void* item);

extern "C" void func_001864B0(void* self, void* item, int msg)
{
    if (item != 0 && msg == 6) {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001864B0* vt = *(sVEntry001864B0**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, 0x24);
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
    }
}
#endif

INCLUDE_ASM("fe/festatecredits", func_00186518);

INCLUDE_ASM("fe/festatecredits", func_001865A8);

//100%
INCLUDE_ASM("fe/festatecredits", func_00186610);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" void func_002006B8(void* self);
extern void* D_0046BD28[];

extern "C" void* func_00186610(void* self)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 0x13;
    *(void***)((char*)self + 0x8) = D_0046BD28;
    func_002006B8((char*)self + 0x48);
    return self;
}
#endif


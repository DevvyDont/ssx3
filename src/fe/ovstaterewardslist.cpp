#include "common.h"

//100%
INCLUDE_ASM("fe/ovstaterewardslist", cOVStateRewardsList_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void func_0020A380(void* self);
extern char D_004704B8[];

extern "C" void cOVStateRewardsList_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004704B8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0020A380(self);
}
#endif

INCLUDE_ASM("fe/ovstaterewardslist", func_00200288);

INCLUDE_ASM("fe/ovstaterewardslist", func_00200388);

//100%
INCLUDE_ASM("fe/ovstaterewardslist", func_002006B8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_00144BC0(void* iface);
extern unsigned int D_004A2594;
extern int D_004A25A4;

extern "C" void func_002006B8(void)
{
    if (D_004A2594 >= 2) {
        D_004A25A4 = *(int*)((char*)func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0)) + 0x54);
    }
}
#endif


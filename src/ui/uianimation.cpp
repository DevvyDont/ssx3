#include "common.h"

INCLUDE_ASM("ui/uianimation", cUIAnimationBank_getAnimationByHashName);

INCLUDE_ASM("ui/uianimation", func_003971C8);

//100%
INCLUDE_ASM("ui/uianimation", func_00397278);
#ifdef SKIP_ASM
extern "C" unsigned short func_00397278(void* self)
{
    void* p = *(void**)self;
    if (p != 0) {
        return *(unsigned short*)((char*)p + 0x2);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", func_00397298__FPvi);
#ifdef SKIP_ASM
void func_00397298(void* self, int val)
{
    *(int*)((char*)self + 0x0) = val;
}
#endif

INCLUDE_ASM("ui/uianimation", cUIAnimation_cUIAnimation);

INCLUDE_ASM("ui/uianimation", func_003973A8);

//100%
INCLUDE_ASM("ui/uianimation", func_00397468);
#ifdef SKIP_ASM
extern "C" void func_00397468(void* self, int mode, unsigned short v)
{
    mode &= 7;
    if (mode == 0) {
        *(unsigned short*)((char*)self + 0xc) = v;
    }
    if (mode == 2) {
        *(unsigned char*)((char*)self + 0xe) = 1;
        *(unsigned short*)((char*)self + 0xa) = *(unsigned short*)((char*)*(void**)self + 0x2) - 1;
    } else {
        *(unsigned char*)((char*)self + 0xe) = 0;
        *(unsigned short*)((char*)self + 0xa) = 0;
    }
}
#endif

INCLUDE_ASM("ui/uianimation", func_003974B0);

INCLUDE_ASM("ui/uianimation", func_003975E0);


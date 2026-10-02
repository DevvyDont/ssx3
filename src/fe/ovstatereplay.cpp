#include "common.h"

INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onWidgetCreate);

INCLUDE_ASM("fe/ovstatereplay", func_0020DF10);

INCLUDE_ASM("fe/ovstatereplay", func_0020DF38);

INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onUpdate);

INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_setupCameraName);

INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_setupTicker);

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/ovstatereplay", func_0020E8E0__FPv);
#ifdef SKIP_ASM
void* func_0020E8E0(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", func_0020E900);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_00231840(void* game, int a);
extern "C" void func_0039E510(void* self);

extern "C" void func_0020E900(void* self)
{
    func_00231840(*(void**)((char*)D_004A28A8 + 0x84), 1);
    func_0039E510(self);
}
#endif


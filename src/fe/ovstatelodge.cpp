#include "common.h"

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_cFEStateMountainRoom);

INCLUDE_ASM("fe/ovstatelodge", func_001D28F8);

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D2A90);
#ifdef SKIP_ASM
extern "C" void func_001D3340(void* self);
extern "C" void func_00186518(void* self, int a1);

extern "C" void func_001D2A90(void* self, int a1)
{
    func_001D3340(self);
    func_00186518(self, a1);
}
#endif

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onWidgetCreate);

INCLUDE_ASM("fe/ovstatelodge", func_001D2EA0);

INCLUDE_ASM("fe/ovstatelodge", func_001D2F40);

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onWidgetEvent);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3120);
#ifdef SKIP_ASM
extern "C" void func_001D31C0(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001D3120(void* self)
{
    if (~*(int*)((char*)self + 0xBC) != 0) {
        func_001D31C0(self);
    }
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/ovstatelodge", func_001D3160);

INCLUDE_ASM("fe/ovstatelodge", func_001D31C0);

INCLUDE_ASM("fe/ovstatelodge", func_001D32E0);

INCLUDE_ASM("fe/ovstatelodge", func_001D3340);

INCLUDE_ASM("fe/ovstatelodge", func_001D3780);

INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onCreateScreen);

INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onWidgetCreate);

extern "C" void* func_0039E4C0(void* self);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3C60__FPv);
#ifdef SKIP_ASM
void* func_001D3C60(void* self)
{
    return func_0039E4C0(self);
}
#endif

INCLUDE_ASM("fe/ovstatelodge", func_001D3C80);

INCLUDE_ASM("fe/ovstatelodge", func_001D3E08);

INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_updateHelpText);

INCLUDE_ASM("fe/ovstatelodge", func_001D3F80);

INCLUDE_ASM("fe/ovstatelodge", func_001D4268);


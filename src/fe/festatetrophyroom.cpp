#include "common.h"

INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onCreateScreen);

extern "C" void* func_0039E4C0(void* self);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D43D0__FPv);
#ifdef SKIP_ASM
void* func_001D43D0(void* self)
{
    return func_0039E4C0(self);
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4660);
#ifdef SKIP_ASM
extern "C" int func_001D4660(void* self, int a1, unsigned int a2)
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

INCLUDE_ASM("fe/festatetrophyroom", func_001D4698);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4760);
#ifdef SKIP_ASM
extern "C" void func_001D4A20(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001D4760(void* self)
{
    if (*(int*)((char*)self + 0x5C) > 0) {
        func_001D4A20(self);
    }
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D47A0);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4890);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4918);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4A20);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4B20);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4BC8);

INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onCreateScreen);

INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onWidgetCreate);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4EA8);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4F68);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4F90);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5038);
#ifdef SKIP_ASM
extern "C" void* func_001BEE30(void* self);
extern void* D_00468050[];

extern "C" void* func_001D5038(void* self)
{
    func_001BEE30(self);
    *(void***)((char*)self + 0x8) = D_00468050;
    *(int*)((char*)self + 0xC) = 0x34;
    return self;
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D5078);

INCLUDE_ASM("fe/festatetrophyroom", func_001D50E0);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5138);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5240);
#ifdef SKIP_ASM
extern "C" int func_001A9668(void* self, void* a1, int a2);
extern "C" int func_001BF6C0(void* self, void* a1, int a2);

extern "C" int func_001D5240(void* self, void* a1, int a2)
{
    int r;
    if (*(int*)((char*)a1 + 0x18) == 0x102) {
        r = func_001A9668(self, a1, a2);
    } else {
        r = func_001BF6C0(self, a1, a2);
    }
    return r;
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D5280);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5330);

INCLUDE_ASM("fe/festatetrophyroom", func_001D53B0);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5428);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5460);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5488);

INCLUDE_ASM("fe/festatetrophyroom", func_001D58B8);

INCLUDE_ASM("fe/festatetrophyroom", func_001D59A0);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5C28);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5D78);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5DF0);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5F38__FPv);
#ifdef SKIP_ASM
void func_001D5F38(void* self)
{
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D5F40);

INCLUDE_ASM("fe/festatetrophyroom", func_001D64C0);


#include "common.h"

INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_onWidgetCreate);

INCLUDE_ASM("fe/messagecenter", func_00197500);

INCLUDE_ASM("fe/messagecenter", func_001977D0);

INCLUDE_ASM("fe/messagecenter", func_001979D8);

//100%
INCLUDE_ASM("fe/messagecenter", func_00197AD8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_97AD8 {
    char pad_0x0[0x4C];
    int list[64];   // 0x4C
    int count;      // 0x14C
    char pad_0x150[0x10];
    ulong mask;     // 0x160
};

extern "C" void func_00197AD8(void* self)
{
    sMsgCenter_97AD8* s = (sMsgCenter_97AD8*)self;
    int i;
    int j;
    int n = 0;
    for (i = 0; i < s->count; i++) {
        if ((int)((s->mask >> i) & 1)) {
            s->list[n++] = i;
        }
    }
    for (j = 0; j < s->count; j++) {
        if (!(int)((s->mask >> j) & 1)) {
            s->list[n++] = j;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00197B88);
#ifdef SKIP_ASM
extern "C" void func_00197BC8(void* self);
extern "C" void func_00197CA0(void* self);
extern "C" void func_00197DB8(void* self);
extern "C" void func_00198118(void* self);

extern "C" void func_00197B88(void* self)
{
    func_00197BC8(self);
    func_00197CA0(self);
    func_00197DB8(self);
    func_00198118(self);
}
#endif

INCLUDE_ASM("fe/messagecenter", func_00197BC8);

INCLUDE_ASM("fe/messagecenter", func_00197CA0);

INCLUDE_ASM("fe/messagecenter", func_00197DB8);

INCLUDE_ASM("fe/messagecenter", func_00197E70);

INCLUDE_ASM("fe/messagecenter", func_00198118);

INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateHelpText);

INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateButtonsText);

//100%
INCLUDE_ASM("fe/messagecenter", func_001985B0);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* func_00198AF0(void* a0);

extern "C" void func_001985B0(void* self)
{
    if (*(cUIText**)((char*)self + 0x1E0) != 0) {
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x1E0), func_00198AF0(*(void**)((char*)self + 0x168)));
    }
}
#endif

INCLUDE_ASM("fe/messagecenter", func_001985F0);

INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateHilightedSongInfo);

INCLUDE_ASM("fe/messagecenter", func_001988D8);

INCLUDE_ASM("fe/messagecenter", func_00198988);

//100%
INCLUDE_ASM("fe/messagecenter", func_00198A88);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void* func_002B40F0(void* self, int i);

extern "C" void* func_00198A88(void* self, int i)
{
    return func_002B40F0((char*)func_0028B180() + 0x118, i);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198AB8);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void* func_002B4120(void* self, int i);

extern "C" void* func_00198AB8(void* self, int i)
{
    return func_002B4120((char*)func_0028B180() + 0x118, i);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198AE8__FPv);
#ifdef SKIP_ASM
int func_00198AE8(void* self)
{
    return 0x1388;
}
#endif

INCLUDE_ASM("fe/messagecenter", func_00198AF0);

//100%
INCLUDE_ASM("fe/messagecenter", func_00198DA0);
#ifdef SKIP_ASM
extern "C" int func_00198DA0(void* self, void* a1)
{
    void* p = *(void**)self;
    void* p2 = *(void**)a1;
    return *(short*)((char*)p + 0xa) - *(short*)((char*)p2 + 0xa);
}
#endif


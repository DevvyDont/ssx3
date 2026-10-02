#include "common.h"

INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onCreateScreen);

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001D9E20);
#ifdef SKIP_ASM
struct sVEntry001D9E20 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" int func_001D9E20(void* self, int a1, int a2)
{
    if (*(int*)((char*)self + 0x50) != 0) {
        void* obj = *(void**)((char*)self + 0x20);
        if (obj != 0) {
            sVEntry001D9E20* vt = *(sVEntry001D9E20**)((char*)obj + 8);
            return vt[24].fn((char*)obj + vt[24].delta, a1, a2);
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/festatenethelppopup", func_001D9E68);

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001D9F30__FPv);
#ifdef SKIP_ASM
int func_001D9F30(void* self)
{
    return 0x1;
}
#endif

INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onWidgetCreate);

INCLUDE_ASM("fe/festatenethelppopup", func_001DA110);

INCLUDE_ASM("fe/festatenethelppopup", func_001DA238);

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA478);
#ifdef SKIP_ASM
extern "C" void func_0039F190(void*, int);

extern "C" void func_001DA478(void* self, int a1, int msg)
{
    if (msg == 6) {
        func_0039F190((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA4A8);
#ifdef SKIP_ASM
struct sNetHelpPopup_DA4A8 {
    char pad_0x0[0x48];
    int state;      // 0x48
    char pad_0x4C[0x8];
    int f54;        // 0x54
    int c[3];       // 0x58
    int f64;        // 0x64
    int a[7];       // 0x68
    int b[7];       // 0x84
};

extern "C" void func_001DA4A8(void* self)
{
    sNetHelpPopup_DA4A8* s = (sNetHelpPopup_DA4A8*)self;
    int i;
    s->state = 0;
    s->f54 = 0;
    for (i = 2; i >= 0; i--) {
        s->c[i] = 0;
    }
    s->f64 = 0;
    for (i = 0; i < 7; i++) {
        s->a[i] = 0;
        s->b[i] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA510__FPviii);
#ifdef SKIP_ASM
struct sNetHelpPopup_DA510 {
    char pad[0x68];
    int a[7];
    int b[7];
};

void func_001DA510(void* self, int i, int a, int b)
{
    sNetHelpPopup_DA510* s = (sNetHelpPopup_DA510*)self;
    s->a[i] = a;
    s->b[i] = b;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA528__FPvi);
#ifdef SKIP_ASM
void func_001DA528(void* self, int val)
{
    *(int*)((char*)self + 0x48) = val;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA530__FPvii);
#ifdef SKIP_ASM
void func_001DA530(void* self, int i, int value)
{
    *(int*)((char*)self + (i << 2) + 0x58) = value;
}
#endif

INCLUDE_ASM("fe/festatenethelppopup", func_001DA648);

INCLUDE_ASM("fe/festatenethelppopup", func_001DA7E8);

INCLUDE_ASM("fe/festatenethelppopup", func_001DA988);

INCLUDE_ASM("fe/festatenethelppopup", func_001DAAC8);

INCLUDE_ASM("fe/festatenethelppopup", func_001DABD0);

INCLUDE_ASM("fe/festatenethelppopup", func_001DACB8);

INCLUDE_ASM("fe/festatenethelppopup", func_001DAE20);

INCLUDE_ASM("fe/festatenethelppopup", func_001DAF08);

INCLUDE_ASM("fe/festatenethelppopup", func_001DB028);


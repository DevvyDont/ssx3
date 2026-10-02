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

//100%
INCLUDE_ASM("fe/messagecenter", func_00197BC8);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* func_00198A88(void* self, int i);

class cMsgWidget_97BC8 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

struct sMsgCenter_97BC8 {
    char pad_0x0[0x4C];
    int list[64];   // 0x4C
    int count;      // 0x14C
    int top;        // 0x150
    char pad_0x154[0x20];
    cMsgWidget_97BC8* texts[8]; // 0x174
};

extern "C" void func_00197BC8(void* self)
{
    sMsgCenter_97BC8* s = (sMsgCenter_97BC8*)self;
    int i;
    for (i = 0; i < 8; i++) {
        int idx = s->top + i;
        if (idx < s->count) {
            int id = s->list[idx];
            s->texts[i]->show(1);
            cUIText_setAsciiString((cUIText*)s->texts[i], (const char*)func_00198A88(self, id));
        } else {
            s->texts[i]->show(0);
        }
    }
}
#endif

INCLUDE_ASM("fe/messagecenter", func_00197CA0);

//100%
INCLUDE_ASM("fe/messagecenter", func_00197DB8);
#ifdef SKIP_ASM
class cMsgWidget_97DB8 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

extern "C" void func_00197DB8(void* self)
{
    if (*(int*)((char*)self + 0x150) == 0) {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1EC))->show(0);
    } else {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1EC))->show(1);
    }
    if (*(int*)((char*)self + 0x150) + 8 >= *(int*)((char*)self + 0x14C)) {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1F0))->show(0);
    } else {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1F0))->show(1);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/messagecenter", func_001985F0);
#ifdef SKIP_ASM
class cMsgWidget_985F0 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_985F0 {
    char pad_0x0[0x160];
    ulong mask;     // 0x160
    char pad_0x168[0x4];
    int enabled;    // 0x16C
    char pad_0x170[0x4C];
    cMsgWidget_985F0* icons[6]; // 0x1BC
};

extern "C" void func_001985F0(void* self)
{
    sMsgCenter_985F0* s = (sMsgCenter_985F0*)self;
    int n = 0;
    int i;
    int j;
    for (j = 0; j < 64; j++) {
        n += (int)((s->mask >> j) & 1);
    }
    for (i = 0; i < 6; i++) {
        if (s->icons[i] != 0) {
            if (s->enabled != 0 && n < 6 - i) {
                s->icons[i]->show(1);
            } else {
                s->icons[i]->show(0);
            }
        }
    }
}
#endif

INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateHilightedSongInfo);

//100%
INCLUDE_ASM("fe/messagecenter", func_001988D8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00158558(void* iface, int rider, int a, int idx, int full);

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_988D8 {
    char pad_0x0[0x44];
    signed char rider; // 0x44
    char pad_0x45[0x3];
    int a48;        // 0x48
    char pad_0x4C[0x10C];
    ulong seen;     // 0x158
    ulong mask;     // 0x160
    int result;     // 0x168
};

extern "C" void func_001988D8(void* self, int idx)
{
    sMsgCenter_988D8* s = (sMsgCenter_988D8*)self;
    int n = 0;
    int j;
    for (j = 0; j < 64; j++) {
        n += (int)((s->mask >> j) & 1);
    }
    int full = n >= 6;
    s->result = func_00158558(cBE_getInterface_Fv(cBE_getBE(), 0xD), s->rider, s->a48, idx, full);
    ulong bit = (ulong)1 << idx;
    s->mask |= bit;
    s->seen |= bit;
}
#endif

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


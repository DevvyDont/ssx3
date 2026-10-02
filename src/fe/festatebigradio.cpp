#include "common.h"

INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onCreateScreen);

//100%
INCLUDE_ASM("fe/festatebigradio", func_00193310__FPv);
#ifdef SKIP_ASM
int func_00193310(void* self)
{
    return 0;
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festatebigradio", func_00193318__FPv);
#ifdef SKIP_ASM
void* func_00193318(void* self)
{
    return func_0039E6B8(self);
}
#endif

INCLUDE_ASM("fe/festatebigradio", func_00193338);

//100%
INCLUDE_ASM("fe/festatebigradio", func_00193568);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char D_004A1790[];
extern char D_004A1798[];
extern int D_004A1A70;
extern "C" void func_001A87D0(void* self, int a1);
extern "C" void func_001927B0(void* p);
// func_0039E4C0 returns nothing (ends in a void vcall).
void func_0039E4C0_v(void* self) __asm__("func_0039E4C0");

static inline int isOn00193568()
{
    return D_004A1A70 == 1;
}

static inline int get6F8_3568(void* p) { return *(int*)((char*)p + 0x6F8); }

extern "C" void func_00193568(void* self, int a1)
{
    if (isOn00193568()) {
        func_001A87D0(self, a1);
        return;
    }
    func_0039E4C0_v(self);
    if (*(int*)((char*)self + 0x6F8) != 0 && *(int*)((char*)self + 0x72C) == 0) {
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_004A1790));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
        }
    } else if (get6F8_3568(self) == 0 && *(int*)((char*)self + 0x72C) != 0) {
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_004A1798));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
        }
    }
    func_001927B0((char*)self + 0x6D0);
    func_001927B0((char*)self + 0x704);
}
#endif

INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onWidgetCreate);

INCLUDE_ASM("fe/festatebigradio", func_001938F8);

//100%
INCLUDE_ASM("fe/festatebigradio", func_00193C00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_0045FDE8[];
extern char D_0045FE00[];
extern char D_0045FE20[];
extern char D_0045FE40[];
extern char D_0045FE60[];
extern char D_0045FE80[];
extern char D_0045FE98[];

extern "C" void func_00193C00(void* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_0045FDE8);
    func_001DA530(popup, 0, GetHashValue32(D_0045FE00));
    func_001DA530(popup, 1, GetHashValue32(D_0045FE20));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE40), 4);
    func_001DA510(popup, 1, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 2, GetHashValue32(D_0045FE80), 0);
    func_001DA510(popup, 3, GetHashValue32(D_0045FE98), 1);
}
#endif

INCLUDE_ASM("fe/festatebigradio", func_00193CF8);

INCLUDE_ASM("fe/festatebigradio", func_00193DE0);

INCLUDE_ASM("fe/festatebigradio", func_00193F38);

//100%
INCLUDE_ASM("fe/festatebigradio", func_00194080);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char D_004A1790[];
extern char D_004A1798[];

static inline int get6F8_4080(void* p) { return *(int*)((char*)p + 0x6F8); }
extern "C" void func_00194080(void* self)
{
    if (*(int*)((char*)self + 0x6F8) != 0 && *(int*)((char*)self + 0x72C) == 0) {
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_004A1790));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
        }
    } else if (get6F8_4080(self) == 0 && *(int*)((char*)self + 0x72C) != 0) {
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_004A1798));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", func_00194138);
#ifdef SKIP_ASM
extern int D_004A1A70;
extern "C" void* func_001A8B88(void* self, int size, int a2);

static inline int isOn00194138()
{
    return D_004A1A70 == 1;
}

extern "C" void* func_00194138(void* self, int a1, int a2)
{
    if (isOn00194138()) {
        return func_001A8B88(self, a1, a2);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", func_00194168);
#ifdef SKIP_ASM
extern int D_004A1A70;
extern "C" void func_00194440(void* self, void* a1, int a2);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

static inline int isOn00194168()
{
    return D_004A1A70 == 1;
}

extern "C" void func_00194168(void* self, void* a1, int a2)
{
    if (isOn00194168()) {
        if (*(int*)((char*)a1 + 0x18) == 0x838) {
            func_00194440(self, a1, a2);
        } else {
            func_001A97B8(self, a1, a2);
        }
    }
}
#endif

INCLUDE_ASM("fe/festatebigradio", func_001941B0);

//100%
INCLUDE_ASM("fe/festatebigradio", func_00194440);
#ifdef SKIP_ASM
extern void* D_004A33CC;
extern "C" void func_00265768(void* snd, int id);
void func_001A94D8(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_00194440(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        if (D_004A33CC != 0) {
            func_00265768(D_004A33CC, 0xF9);
        }
        func_001A94D8(self);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", func_00194498);
#ifdef SKIP_ASM
struct sColor4_4498 {
    float x, y, z, w;
};

struct sVEntry00194498a {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sVEntry00194498b {
    short delta;
    short index;
    void (*fn)(void*, sColor4_4498*);
};

extern "C" void func_00194498(void* self)
{
    sVEntry00194498a* vt = *(sVEntry00194498a**)((char*)self + 8);
    vt[8].fn((char*)self + vt[8].delta, 1);
    sColor4_4498 c = *(sColor4_4498*)((char*)self + 0x1C);
    if (c.x > 0.5f) {
        c.x = 0.5f;
        sVEntry00194498b* vt2 = *(sVEntry00194498b**)((char*)self + 8);
        vt2[11].fn((char*)self + vt2[11].delta, &c);
    }
}
#endif


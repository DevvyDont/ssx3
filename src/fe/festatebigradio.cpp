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

INCLUDE_ASM("fe/festatebigradio", func_00193568);

INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onWidgetCreate);

INCLUDE_ASM("fe/festatebigradio", func_001938F8);

INCLUDE_ASM("fe/festatebigradio", func_00193C00);

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

INCLUDE_ASM("fe/festatebigradio", func_00194138);

INCLUDE_ASM("fe/festatebigradio", func_00194168);

INCLUDE_ASM("fe/festatebigradio", func_001941B0);

INCLUDE_ASM("fe/festatebigradio", func_00194440);

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


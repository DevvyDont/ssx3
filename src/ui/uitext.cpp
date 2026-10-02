#include "common.h"

INCLUDE_ASM("ui/uitext", cUIText_render2D);

//100%
INCLUDE_ASM("ui/uitext", cUIText_deleteText);
#ifdef SKIP_ASM
void cMemMan_free(void*);

extern "C" void cUIText_deleteText(void* self)
{
    *(int*)((char*)self + 0xA8) = 0;
    if (*(void**)((char*)self + 0xAC) != 0) {
        cMemMan_free(*(void**)((char*)self + 0xAC));
        *(void**)((char*)self + 0xAC) = 0;
    }
    if (*(void**)((char*)self + 0xB4) != 0) {
        cMemMan_free(*(void**)((char*)self + 0xB4));
        *(void**)((char*)self + 0xB4) = 0;
    }
}
#endif

INCLUDE_ASM("ui/uitext", cUIText_getNumTextLines);

//100%
INCLUDE_ASM("ui/uitext", func_003A0C00);
#ifdef SKIP_ASM
extern "C" int func_003A0C00(void* self)
{
    if (((*(int*)((char*)self + 0x74) >> 3) & 1) == 0) {
        return *(int*)((char*)self + 0xb4);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A0C28);
#ifdef SKIP_ASM
extern "C" int func_003A0C28(void* self)
{
    if (((*(int*)((char*)self + 0x74) >> 3) & 1) != 0) {
        return *(int*)((char*)self + 0xb4);
    }
    return 0;
}
#endif

struct cUITextManager {
    char pad_0x00[0x28];
    short field_0x28;
    char pad_0x2A[2];
    void (*fn)(void*, int); // 0x2C
};

struct cUIText {
    char pad_0x00[0x8];
    cUITextManager* mMgr; // 0x8
    char pad_0xC[0xB0 - 0xC];
    int field_0xB0;
};

void cUIText_setAsciiStringPrivate(cUIText* self, const char* str);

//100%
INCLUDE_ASM("ui/uitext", cUIText_setAsciiString__FP7cUITextPCc);
#ifdef SKIP_ASM
void cUIText_setAsciiString(cUIText* self, const char* str)
{
    self->field_0xB0 = 0;
    cUIText_setAsciiStringPrivate(self, str);
}
#endif

INCLUDE_ASM("ui/uitext", cUIText_setAsciiStringPrivate);

INCLUDE_ASM("ui/uitext", func_003A0D00);

//100%
INCLUDE_ASM("ui/uitext", func_003A0E90);
#ifdef SKIP_ASM
extern "C" void cUIText_setUnicodeStringPrivate(void* self, const unsigned short* str);

extern "C" void func_003A0E90(void* self, const unsigned short* str)
{
    *(int*)((char*)self + 0xB0) = 0;
    cUIText_setUnicodeStringPrivate(self, str);
}
#endif

INCLUDE_ASM("ui/uitext", cUIText_setUnicodeStringPrivate);

//100%
INCLUDE_ASM("ui/uitext", cUIText_setUnicodeStringByID__FP7cUITexti);
#ifdef SKIP_ASM
void cUIText_setUnicodeStringByID(cUIText* self, int id)
{
    self->field_0xB0 = id;
    cUITextManager* mgr = self->mMgr;
    mgr->fn((char*)self + mgr->field_0x28, 0);
}
#endif

INCLUDE_ASM("ui/uitext", func_003A1030);

INCLUDE_ASM("ui/uitext", func_003A1148);

INCLUDE_ASM("ui/uitext", func_003A11B8);

//100%
INCLUDE_ASM("ui/uitext", func_003A12D0);
#ifdef SKIP_ASM
extern "C" int func_003A12D0(void* self)
{
    void* p = *(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x5c) + 0xd0) + 0x10);
    if (p != 0) {
        return *(int*)((char*)*(void**)((char*)p + 0x8) + ((*(unsigned int*)((char*)self + 0x74) >> 1) & 3) * 12 + 0xC);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1310);
#ifdef SKIP_ASM
extern "C" void func_003A1310(void* self, char a1)
{
    if (a1 == 0) {
        *(char*)((char*)self + 0x88) = 0;
        *(char*)((char*)self + 0x89) = 1;
        *(char*)((char*)self + 0x8A) = 2;
        *(char*)((char*)self + 0x8B) = 3;
    } else {
        *(char*)((char*)self + 0x88) = 4;
        *(char*)((char*)self + 0x89) = 5;
        *(char*)((char*)self + 0x8A) = 6;
        *(char*)((char*)self + 0x8B) = 7;
    }
}
#endif

INCLUDE_ASM("ui/uitext", func_003A1360);

INCLUDE_ASM("ui/uitext", func_003A13E0);

INCLUDE_ASM("ui/uitext", func_003A1588);

INCLUDE_ASM("ui/uitext", func_003A18C0);

INCLUDE_ASM("ui/uitext", func_003A1958);

INCLUDE_ASM("ui/uitext", func_003A19F8);

INCLUDE_ASM("ui/uitext", cUITextScroll_addUnicodeString);

//100%
INCLUDE_ASM("ui/uitext", func_003A1CF0);
#ifdef SKIP_ASM
extern "C" void func_003A18C0(void*, void*);
extern "C" void func_003A1D30(void*, void*);

extern "C" void func_003A1CF0(void* self, void* a1)
{
    func_003A18C0(self, a1);
    func_003A1D30(self, a1);
}
#endif

INCLUDE_ASM("ui/uitext", func_003A1D30);

INCLUDE_ASM("ui/uitext", func_003A1F18);

INCLUDE_ASM("ui/uitext", func_003A1F90);

//100%
INCLUDE_ASM("ui/uitext", func_003A2068);
#ifdef SKIP_ASM
extern "C" int func_003A2068(void* self, int i)
{
    int* arr = *(int**)((char*)self + 0xBC);
    if (arr != 0 && i >= 0 && i < *(unsigned short*)((char*)self + 0xC8)) {
        return arr[i];
    }
    return 0;
}
#endif

INCLUDE_ASM("ui/uitext", func_003A3280);

INCLUDE_ASM("ui/uitext", func_003A32F0);

//100%
INCLUDE_ASM("ui/uitext", func_003A3398);
#ifdef SKIP_ASM
struct func_003A3398_sColor {
    float r;
    float g;
    float b;
    float a;
};

extern "C" void func_003A3398(void* self, func_003A3398_sColor* c)
{
    func_003A3398_sColor tmp = *c;
    unsigned int i;
    *(func_003A3398_sColor*)((char*)self + 0x1C) = *c;
    if (*(func_003A3398_sColor**)((char*)self + 0x7C) == 0) {
        return;
    }
    tmp.r = (int)(c->r * 255.0f);
    tmp.g = (int)(c->g * 255.0f);
    tmp.b = (int)(c->b * 255.0f);
    tmp.a = (int)(c->a * 255.0f);
    for (i = 0; i < *(unsigned int*)((char*)self + 0x74) >> 17; i++) {
        (*(func_003A3398_sColor**)((char*)self + 0x7C))[i] = tmp;
    }
}
#endif


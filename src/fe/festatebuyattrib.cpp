#include "common.h"

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onCreateScreen);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F49D0);
#ifdef SKIP_ASM
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(void* self);

extern "C" int func_001F49D0(void* self, int on)
{
    if (on != 0) {
        cFEStateBuyAttrib_updateExperienceDisplay(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F49F8);
#ifdef SKIP_ASM
struct sVEntry001F49F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00186518(void* self);

extern "C" void func_001F49F8(void* self)
{
    func_00186518(self);
    void* obj = *(void**)((char*)self + 0xAC);
    sVEntry001F49F8* vt = *(sVEntry001F49F8**)((char*)obj + 8);
    vt[7].fn((char*)obj + vt[7].delta, 1);
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", func_001F4A38);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F4A60);
#ifdef SKIP_ASM
extern "C" void func_001F5300(void* self);

extern "C" void func_001F4A60(void* self, void* sender, int msg)
{
    if (msg == 0x16) {
        if (*(int*)((char*)sender + 0x6C) != 0) {
            func_001F5300(self);
        }
    }
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", func_001F4A90);

INCLUDE_ASM("fe/festatebuyattrib", func_001F4C30);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateLevels);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateCostPerLevel);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateExperienceDisplay);

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateTotalCost);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* func_00198AF0(int value);
extern char D_0046F090[];

extern "C" void cFEStateBuyAttrib_updateTotalCost(void* self)
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x48), GetHashValue32(D_0046F090));
    if (text != 0) {
        cUIText_setAsciiString(text, func_00198AF0(*(int*)((char*)self + 0xA0)));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateBank);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* func_00198AF0(int value);
extern char D_0046F0A0[];

extern "C" void cFEStateBuyAttrib_updateBank(void* self)
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x48), GetHashValue32(D_0046F0A0));
    if (text != 0) {
        cUIText_setAsciiString(text, func_00198AF0(*(int*)((char*)self + 0xA4)));
    }
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", func_001F5300);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F54B0);
#ifdef SKIP_ASM
extern void* D_00473838[];
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* func_001F54B0(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_00473838;
    *(int*)((char*)self + 0xC) = 0x2A;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), 0);
    return self;
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onCreateScreen);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F55C0);
#ifdef SKIP_ASM
extern "C" void cFEStateCareerStats_setupHighlightsList(void* self);

extern "C" int func_001F55C0(void* self, int on)
{
    if (on != 0) {
        cFEStateCareerStats_setupHighlightsList(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F55E8);
#ifdef SKIP_ASM
extern "C" void func_00186518(void* self);
extern "C" void cFEStateCareerStats_setupMenuFocus(void* self, int focus);

extern "C" void func_001F55E8(void* self)
{
    func_00186518(self);
    cFEStateCareerStats_setupMenuFocus(self, *(int*)((char*)self + 0x48));
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F5618);
#ifdef SKIP_ASM
extern "C" int func_001F5618(void* self, int a1, unsigned int a2)
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

INCLUDE_ASM("fe/festatebuyattrib", func_001F5650);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onInputBegin);

INCLUDE_ASM("fe/festatebuyattrib", func_001F5A38);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupMenuFocus);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupHighlightsList);

INCLUDE_ASM("fe/festatebuyattrib", func_001F60E0);

INCLUDE_ASM("fe/festatebuyattrib", func_001F6490);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F6840);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);

extern "C" void func_001F6840(void* self, char* name, const char* str)
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
    if (text != 0) {
        cUIText_setAsciiString(text, str);
    }
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupRidersBest);

INCLUDE_ASM("fe/festatebuyattrib", func_001F6F30);


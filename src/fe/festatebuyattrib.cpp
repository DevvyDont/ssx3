#include "common.h"

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046EFF8[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
extern "C" int func_00150928(void* iface, int a1, int charID);
extern "C" void cFEStateBuyAttrib_updateTotalCost(void* self);
extern "C" void cFEStateBuyAttrib_updateLevels(void* self);
extern "C" void cFEStateBuyAttrib_updateCostPerLevel(void* self);
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(void* self);
extern "C" void cFEStateBuyAttrib_updateBank(void* self);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);

extern "C" void cFEStateBuyAttrib_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046EFF8), 0);
    *(void**)((char*)self + 0x48) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xB);
    *(int*)((char*)self + 0xA4) = func_00150928(iface, 0, cBELibrary_getCharacterID(0));
    *(int*)((char*)self + 0xA8) = 0;
    *(int*)((char*)self + 0xA0) = 0;
    cFEStateBuyAttrib_updateTotalCost(self);
    cFEStateBuyAttrib_updateLevels(self);
    cFEStateBuyAttrib_updateCostPerLevel(self);
    cFEStateBuyAttrib_updateExperienceDisplay(self);
    cFEStateBuyAttrib_updateBank(self);
    func_0028F140(func_0028B180(), 3);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_0046F008[];
extern char D_0046F020[];
extern char D_0046F040[];
extern char D_004A2320[];
extern char D_004A2328[];

static inline int IsHash_4910(int id, char* s) { return id == GetHashValue32(s); }

extern "C" void cFEStateBuyAttrib_onWidgetCreate(void* self, void* widget)
{
    if (IsHash_4910(*(int*)((char*)widget + 0x38), D_0046F008)) {
        cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(D_0046F020));
    } else if (IsHash_4910(*(int*)((char*)widget + 0x38), D_0046F040)) {
        *(void**)((char*)self + 0xAC) = widget;
    } else if (IsHash_4910(*(int*)((char*)widget + 0x38), D_004A2320)) {
        *(int*)((char*)widget + 0x18) = 0;
    } else if (IsHash_4910(*(int*)((char*)widget + 0x38), D_004A2328)) {
        *(int*)((char*)widget + 0x18) = 1;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateCostPerLevel);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* func_00198AF0(int value);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_00150E50(void* iface, int level);
extern char D_004A2348[];
struct sVE4FC0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cFEStateBuyAttrib_updateCostPerLevel(void* self)
{
    char buf[32];
    int i;
    int* levels = (int*)((char*)self + 0x68);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xB);
    for (i = 0; i < 7; i++, levels++) {
        sprintf(buf, D_004A2348, i);
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x48), GetHashValue32(buf));
        if (text != 0) {
            cUIText_setAsciiString(text, func_00198AF0(func_00150E50(iface, *levels - 1)));
            if (*levels >= 0xB) {
                sVE4FC0* vt = *(sVE4FC0**)((char*)text + 8);
                vt[9].fn((char*)text + vt[9].delta, 0);
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046F168[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001F6F30(void* self);
extern "C" void func_001F5A38(void* self);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);

extern "C" void cFEStateCareerStats_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046F168), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x50) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0x70) = 0;
    *(int*)((char*)self + 0x6C) = 0;
    *(int*)((char*)self + 0x74) = 0;
    *(int*)((char*)self + 0x48) = 0;
    func_001F6F30(self);
    func_001F5A38(self);
    func_0028F140(func_0028B180(), 8);
}
#endif

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

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F5650);
#ifdef SKIP_ASM
struct sVEntry001F5650 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void func_0039F400(void* list, void* item);

extern "C" void func_001F5650(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5:
        break;
    case 6: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001F5650* vt = *(sVEntry001F5650**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
        break;
    }
    }
}
#endif

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


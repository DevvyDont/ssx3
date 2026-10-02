#include "common.h"

INCLUDE_ASM("fe/fepopupmisc", cFEPopupVideoCalibration_onCreateScreen);

INCLUDE_ASM("fe/fepopupmisc", func_001DEC68);

extern void* D_0046B230[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DEDE8__FPv);
#ifdef SKIP_ASM
void* func_001DEDE8(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046B230;
    return func_0039E390(self);
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001DEE10);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF028);
#ifdef SKIP_ASM
extern "C" int func_001DF028(void* self, int i)
{
    switch (i) {
    case 0:
        return *(int*)((char*)self + 0x48);
    case 1:
        return *(int*)((char*)self + 0x4c);
    }
    return *(signed char*)((char*)self + 0x50);
}
#endif

INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_onUpdate);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF378__FPv);
#ifdef SKIP_ASM
int func_001DF378(void* self)
{
    return 0x1;
}
#endif

extern void* D_0046D0D0[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF3B8__FPv);
#ifdef SKIP_ASM
void* func_001DF3B8(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF418__FPv);
#ifdef SKIP_ASM
void* func_001DF418(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF440);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046B078[];

extern "C" void func_001DF440(void* self, int flags)
{
    *(void***)((char*)self + 0x30) = D_0046B078;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001DF4E8);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF578__FPv);
#ifdef SKIP_ASM
void func_001DF578(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF5C0__FPv);
#ifdef SKIP_ASM
void func_001DF5C0(void* self)
{
}
#endif

extern void* D_0046AD48[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF608__FPv);
#ifdef SKIP_ASM
void* func_001DF608(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046AD48;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF630__FPv);
#ifdef SKIP_ASM
int func_001DF630(void* self)
{
    return 0x1;
}
#endif

extern void* D_0046AB28[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF678__FPv);
#ifdef SKIP_ASM
void* func_001DF678(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046AB28;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF6A0__FPv);
#ifdef SKIP_ASM
int func_001DF6A0(void* self)
{
    return 0x1;
}
#endif

extern void* D_0046AA18[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF6E8__FPv);
#ifdef SKIP_ASM
void* func_001DF6E8(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046AA18;
    return func_001A85D0(self);
}
#endif

extern void* D_0046A908[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF750__FPv);
#ifdef SKIP_ASM
void* func_001DF750(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046A908;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF7C8);
#ifdef SKIP_ASM
extern void* D_0046A7F8[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001DF7C8(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A7F8;
    cBXString__cBXString((char*)self + 0xB80, 2);
    cBXString__cBXString((char*)self + 0x76C, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF878);
#ifdef SKIP_ASM
extern void* D_0046A6E8[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001DF878(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A6E8;
    cBXString__cBXString((char*)self + 0x6E4, 2);
    cBXString__cBXString((char*)self + 0x6E0, 2);
    cBXString__cBXString((char*)self + 0x6DC, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF8E0__FPv);
#ifdef SKIP_ASM
int func_001DF8E0(void* self)
{
    return 0x1;
}
#endif

extern void* D_0046A618[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF960__FPv);
#ifdef SKIP_ASM
void* func_001DF960(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046A618;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF988__FPv);
#ifdef SKIP_ASM
int func_001DF988(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFA10);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFA10(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFA88__FPv);
#ifdef SKIP_ASM
int func_001DFA88(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAA0__FPv);
#ifdef SKIP_ASM
int func_001DFAA0(void* self)
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAA8__FPv);
#ifdef SKIP_ASM
int func_001DFAA8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAC0__FPv);
#ifdef SKIP_ASM
void func_001DFAC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAC8__FPv);
#ifdef SKIP_ASM
void func_001DFAC8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAD0__FPv);
#ifdef SKIP_ASM
int func_001DFAD0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAD8__FPv);
#ifdef SKIP_ASM
int func_001DFAD8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAE0__FPv);
#ifdef SKIP_ASM
int func_001DFAE0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAE8__FPv);
#ifdef SKIP_ASM
int func_001DFAE8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFB28);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFB28(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFB88__FPvi);
#ifdef SKIP_ASM
int func_001DFB88(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFB98);
#ifdef SKIP_ASM
extern "C" int func_001DFB98(void* self, void* a1)
{
    int v = *(short*)((char*)a1 + 0xc);
    if (v > 0) v = v * 10;
    return v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFBB0__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFBB0(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFBB8__FPvT0);
#ifdef SKIP_ASM
int func_001DFBB8(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFBF8);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFBF8(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC58__FPvi);
#ifdef SKIP_ASM
int func_001DFC58(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC68);
#ifdef SKIP_ASM
extern "C" int func_001DFC68(void* self, void* a1)
{
    int v = *(short*)((char*)a1 + 0xc);
    if (v > 0) v = v * 10;
    return v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC80__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFC80(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC88__FPvT0);
#ifdef SKIP_ASM
int func_001DFC88(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFCC8);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFCC8(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD28__FPvi);
#ifdef SKIP_ASM
int func_001DFD28(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD38);
#ifdef SKIP_ASM
extern "C" int func_001DFD38(void* self, void* a1)
{
    int v = *(short*)((char*)a1 + 0xc);
    if (v > 0) v = v * 10;
    return v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD50__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFD50(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD58__FPvT0);
#ifdef SKIP_ASM
int func_001DFD58(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD98);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFD98(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFDF8__FPvi);
#ifdef SKIP_ASM
int func_001DFDF8(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE08);
#ifdef SKIP_ASM
extern "C" int func_001DFE08(void* self, void* a1)
{
    int v = *(short*)((char*)a1 + 0xc);
    if (v > 0) v = v * 10;
    return v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE20__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFE20(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE28__FPvT0);
#ifdef SKIP_ASM
int func_001DFE28(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE68);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFE68(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFEC8__FPvi);
#ifdef SKIP_ASM
int func_001DFEC8(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFED8);
#ifdef SKIP_ASM
extern "C" int func_001DFED8(void* self, void* a1)
{
    int v = *(short*)((char*)a1 + 0xc);
    if (v > 0) v = v * 10;
    return v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFEF0__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFEF0(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFEF8__FPvT0);
#ifdef SKIP_ASM
int func_001DFEF8(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFF38);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFF38(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFF98__FPvi);
#ifdef SKIP_ASM
int func_001DFF98(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFFA8);
#ifdef SKIP_ASM
extern "C" int func_001DFFA8(void* self, void* a1)
{
    int v = *(short*)((char*)a1 + 0xc);
    if (v > 0) v = v * 10;
    return v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFFC0__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFFC0(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFFC8__FPvT0);
#ifdef SKIP_ASM
int func_001DFFC8(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0020__FPv);
#ifdef SKIP_ASM
int func_001E0020(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0028__FPv);
#ifdef SKIP_ASM
int func_001E0028(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E01B0__FPv);
#ifdef SKIP_ASM
void* func_001E01B0(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0210);
#ifdef SKIP_ASM
extern void* D_004696B8[];
extern void* D_0046D0D0[];
extern "C" void func_001D4B20(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001E0210(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_004696B8;
    func_001D4B20(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E02A0__FPv);
#ifdef SKIP_ASM
void* func_001E02A0(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

extern void* D_00469518[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E02D0__FPv);
#ifdef SKIP_ASM
void* func_001E02D0(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_00469518;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00460008[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFEPopupSelectMultiplayerMode_onCreateScreen(void* self)
{
    int hash = GetHashValue32(D_00460008);
    *(int*)((char*)self + 0xC) = hash;
    void* screen = cUIEngine_addScreenByHashName(*(void**)((char*)self + 0x10), self, hash, 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onWidgetCreate);

INCLUDE_ASM("fe/fepopupmisc", func_001E03F8);

INCLUDE_ASM("fe/fepopupmisc", func_001E0470);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E04F8__FPv);
#ifdef SKIP_ASM
signed char func_001E04F8(void* self)
{
    return *(signed char*)((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0500__FPv);
#ifdef SKIP_ASM
int func_001E0500(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0508__FPv);
#ifdef SKIP_ASM
int func_001E0508(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0548__FPv);
#ifdef SKIP_ASM
void* func_001E0548(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E05B0__FPv);
#ifdef SKIP_ASM
void* func_001E05B0(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0740);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DD60[];

extern "C" void func_001E0740(void* self, int flags)
{
    *(void***)self = D_0046DD60;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0770__FPv);
#ifdef SKIP_ASM
void func_001E0770(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0778__FPv);
#ifdef SKIP_ASM
void func_001E0778(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0780);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DD60[];

extern "C" void func_001E0780(void* self, int flags)
{
    *(void***)self = D_0046DD60;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E07B0__FPv);
#ifdef SKIP_ASM
void func_001E07B0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E07B8__FPv);
#ifdef SKIP_ASM
void func_001E07B8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E07F8__FPv);
#ifdef SKIP_ASM
void* func_001E07F8(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0858__FPv);
#ifdef SKIP_ASM
void* func_001E0858(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E08D0__FPv);
#ifdef SKIP_ASM
int func_001E08D0(void* self)
{
    return 0;
}
#endif

extern void* D_00468F50[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0918__FPv);
#ifdef SKIP_ASM
void* func_001E0918(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_00468F50;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0940__FPv);
#ifdef SKIP_ASM
int func_001E0940(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0948__FPv);
#ifdef SKIP_ASM
void func_001E0948(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E09A0);
#ifdef SKIP_ASM
struct sVEntry001E09A0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern void* D_00468E38[];
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001E09A0(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00468E38;
    void* obj = *(void**)((char*)self + 0x20);
    sVEntry001E09A0* vt = *(sVEntry001E09A0**)((char*)obj + 8);
    vt[32].fn((char*)obj + vt[32].delta);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0A00__FPv);
#ifdef SKIP_ASM
int func_001E0A00(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0A50);
#ifdef SKIP_ASM
extern void* D_00468D20[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");
struct sVEntry001E0A50 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_001E0A50(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00468D20;
    void* obj = *(void**)((char*)self + 0x20);
    sVEntry001E0A50* vt = *(sVEntry001E0A50**)((char*)obj + 8);
    vt[32].fn((char*)obj + vt[32].delta);
    cBXString__cBXString((char*)self + 0x6D0, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0AB8__FPv);
#ifdef SKIP_ASM
int func_001E0AB8(void* self)
{
    return 0x1;
}
#endif

extern void* D_00468C10[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0B00__FPv);
#ifdef SKIP_ASM
void* func_001E0B00(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_00468C10;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0BA0__FPv);
#ifdef SKIP_ASM
void func_001E0BA0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0BA8__FPv);
#ifdef SKIP_ASM
int func_001E0BA8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C10__FPv);
#ifdef SKIP_ASM
int func_001E0C10(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C18__FPv);
#ifdef SKIP_ASM
int func_001E0C18(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C20__FPv);
#ifdef SKIP_ASM
void func_001E0C20(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C28__FPv);
#ifdef SKIP_ASM
int func_001E0C28(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C78__FPv);
#ifdef SKIP_ASM
int func_001E0C78(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C80__FPv);
#ifdef SKIP_ASM
void func_001E0C80(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C88__FPv);
#ifdef SKIP_ASM
int func_001E0C88(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C90__FPv);
#ifdef SKIP_ASM
int func_001E0C90(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C98__FPv);
#ifdef SKIP_ASM
int func_001E0C98(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0CF8);
#ifdef SKIP_ASM
extern void* D_004686C0[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001E0CF8(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_004686C0;
    cBXString__cBXString((char*)self + 0x6DC, 2);
    cBXString__cBXString((char*)self + 0x6D8, 2);
    cBXString__cBXString((char*)self + 0x6D4, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0D60__FPv);
#ifdef SKIP_ASM
int func_001E0D60(void* self)
{
    return 0;
}
#endif

extern void* D_004685B0[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0DA8__FPv);
#ifdef SKIP_ASM
void* func_001E0DA8(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_004685B0;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0DD0__FPv);
#ifdef SKIP_ASM
int func_001E0DD0(void* self)
{
    return 0;
}
#endif

extern void* D_004684A0[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0E18__FPv);
#ifdef SKIP_ASM
void* func_001E0E18(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_004684A0;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0E40__FPv);
#ifdef SKIP_ASM
int func_001E0E40(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0E88__FPv);
#ifdef SKIP_ASM
void func_001E0E88(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0E90__FPv);
#ifdef SKIP_ASM
int func_001E0E90(void* self)
{
    return 0;
}
#endif

extern void* D_00468278[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0E98__FPv);
#ifdef SKIP_ASM
void* func_001E0E98(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_00468278;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0EC0__FPv);
#ifdef SKIP_ASM
void func_001E0EC0(void* self)
{
}
#endif

extern void* D_00468168[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0EC8__FPv);
#ifdef SKIP_ASM
void* func_001E0EC8(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_00468168;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0EF0__FPv);
#ifdef SKIP_ASM
int func_001E0EF0(void* self)
{
    return 0x1;
}
#endif

extern void* D_00468050[];
extern "C" void* func_001BEE78(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0F30__FPv);
#ifdef SKIP_ASM
void* func_001E0F30(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_00468050;
    return func_001BEE78(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0F58__FPv);
#ifdef SKIP_ASM
int func_001E0F58(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0FB8__FPv);
#ifdef SKIP_ASM
int func_001E0FB8(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0FC0);
#ifdef SKIP_ASM
class func_001E0FC0_cObj {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};

extern "C" int func_001E0FC0(func_001E0FC0_cObj* self)
{
    self->v32();
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1040__FPv);
#ifdef SKIP_ASM
int func_001E1040(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1048);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DD60[];

extern "C" void func_001E1048(void* self, int flags)
{
    *(void***)self = D_0046DD60;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1078__FPv);
#ifdef SKIP_ASM
void func_001E1078(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1080__FPv);
#ifdef SKIP_ASM
void func_001E1080(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1088__FPv);
#ifdef SKIP_ASM
void func_001E1088(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1090);
#ifdef SKIP_ASM
struct sFlags_001E1090 {
    unsigned int pad : 27;
    unsigned int flag : 1;
};

extern "C" void func_001E1090(void* self, int e)
{
    ((sFlags_001E1090*)self)->flag = (e != 0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E10B8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DD28[];

extern "C" void func_001E10B8(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_0046DD28;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E10E8__FPv);
#ifdef SKIP_ASM
void func_001E10E8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E10F0__FPv);
#ifdef SKIP_ASM
int func_001E10F0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E10F8__FPv);
#ifdef SKIP_ASM
int func_001E10F8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1100__FPv);
#ifdef SKIP_ASM
int func_001E1100(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1108);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DBA8[];

extern "C" void func_001E1108(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046DBA8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1138__FPv);
#ifdef SKIP_ASM
int func_001E1138(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1140__FPv);
#ifdef SKIP_ASM
int func_001E1140(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1148__FPv);
#ifdef SKIP_ASM
int func_001E1148(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1150__FPv);
#ifdef SKIP_ASM
int func_001E1150(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1158__FPv);
#ifdef SKIP_ASM
int func_001E1158(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1160__FPv);
#ifdef SKIP_ASM
int func_001E1160(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1168__FPv);
#ifdef SKIP_ASM
int func_001E1168(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1170__FPv);
#ifdef SKIP_ASM
int func_001E1170(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1178__FPv);
#ifdef SKIP_ASM
int func_001E1178(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1180__FPv);
#ifdef SKIP_ASM
int func_001E1180(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1188__FPv);
#ifdef SKIP_ASM
int func_001E1188(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1190__FPv);
#ifdef SKIP_ASM
int func_001E1190(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1198__FPv);
#ifdef SKIP_ASM
int func_001E1198(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11A0__FPv);
#ifdef SKIP_ASM
int func_001E11A0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11A8__FPv);
#ifdef SKIP_ASM
int func_001E11A8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11B0__FPv);
#ifdef SKIP_ASM
int func_001E11B0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11B8__FPv);
#ifdef SKIP_ASM
int func_001E11B8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11C0__FPv);
#ifdef SKIP_ASM
int func_001E11C0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11C8__FPv);
#ifdef SKIP_ASM
int func_001E11C8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11D0__FPv);
#ifdef SKIP_ASM
int func_001E11D0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11D8__FPv);
#ifdef SKIP_ASM
int func_001E11D8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11E0__FPv);
#ifdef SKIP_ASM
int func_001E11E0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11E8__FPv);
#ifdef SKIP_ASM
int func_001E11E8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11F0__FPv);
#ifdef SKIP_ASM
int func_001E11F0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E11F8__FPv);
#ifdef SKIP_ASM
int func_001E11F8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1200__FPv);
#ifdef SKIP_ASM
int func_001E1200(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1208__FPv);
#ifdef SKIP_ASM
int func_001E1208(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1210__FPv);
#ifdef SKIP_ASM
int func_001E1210(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1218__FPv);
#ifdef SKIP_ASM
int func_001E1218(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1220__FPv);
#ifdef SKIP_ASM
int func_001E1220(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1228__FPv);
#ifdef SKIP_ASM
int func_001E1228(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1230__FPv);
#ifdef SKIP_ASM
int func_001E1230(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1238__FPv);
#ifdef SKIP_ASM
int func_001E1238(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1240__FPv);
#ifdef SKIP_ASM
int func_001E1240(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1248__FPv);
#ifdef SKIP_ASM
float func_001E1248(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1258__FPv);
#ifdef SKIP_ASM
float func_001E1258(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1268__FPv);
#ifdef SKIP_ASM
float func_001E1268(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1278__FPv);
#ifdef SKIP_ASM
int func_001E1278(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1280__FPv);
#ifdef SKIP_ASM
int func_001E1280(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1288__FPv);
#ifdef SKIP_ASM
int func_001E1288(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1290__FPv);
#ifdef SKIP_ASM
int func_001E1290(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1298__FPvi);
#ifdef SKIP_ASM
void func_001E1298(void* self, int val)
{
    *(char*)((char*)self + 0x4) = (char)val;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12A0__FPv);
#ifdef SKIP_ASM
int func_001E12A0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12A8__FPv);
#ifdef SKIP_ASM
void func_001E12A8(void* self)
{
}
#endif

extern "C" void* func_001DBC98(int, int);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12B0__FPv);
#ifdef SKIP_ASM
void* func_001E12B0(void* self)
{
    return func_001DBC98(1, 0xffff);
}
#endif

extern "C" void* func_001DBC98(int, int);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12D0__FPv);
#ifdef SKIP_ASM
void* func_001E12D0(void* self)
{
    return func_001DBC98(0, 0xffff);
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E12F0);

INCLUDE_ASM("fe/fepopupmisc", func_001E1458);

INCLUDE_ASM("fe/fepopupmisc", func_001E14C0);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1548__FPv);
#ifdef SKIP_ASM
void func_001E1548(void* self)
{
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E1550);

INCLUDE_ASM("fe/fepopupmisc", func_001E1A30);

INCLUDE_ASM("fe/fepopupmisc", func_001E1AD0);

INCLUDE_ASM("fe/fepopupmisc", func_001E1C10);

INCLUDE_ASM("fe/fepopupmisc", func_001E1DD0);

INCLUDE_ASM("fe/fepopupmisc", func_001E1EB8);

INCLUDE_ASM("fe/fepopupmisc", func_001E20D0);

INCLUDE_ASM("fe/fepopupmisc", func_001E2220);

INCLUDE_ASM("fe/fepopupmisc", func_001E2370);

INCLUDE_ASM("fe/fepopupmisc", func_001E2428);

INCLUDE_ASM("fe/fepopupmisc", func_001E2518);

INCLUDE_ASM("fe/fepopupmisc", func_001E2578);

INCLUDE_ASM("fe/fepopupmisc", func_001E25D8);

INCLUDE_ASM("fe/fepopupmisc", func_001E2648);

INCLUDE_ASM("fe/fepopupmisc", func_001E2828);

INCLUDE_ASM("fe/fepopupmisc", func_001E2A08);

INCLUDE_ASM("fe/fepopupmisc", func_001E2B38);

INCLUDE_ASM("fe/fepopupmisc", func_001E2BB8);

INCLUDE_ASM("fe/fepopupmisc", func_001E2C80);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2D58);
#ifdef SKIP_ASM
struct sPopupEntry18 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18 D_004C6C08[];

struct sPopupEntry14 {
    char pad_0x0[0xC];
    int field_0xC;
    int field_0x10;
};
extern sPopupEntry14 D_00441630[];

extern "C" int func_001E2D58(int v, int i)
{
    sPopupEntry18* a = &D_004C6C08[i];
    sPopupEntry14* b = &D_00441630[a->field_0x4];
    return b->field_0xC == v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2DC0);
#ifdef SKIP_ASM
extern "C" int func_001E2DC0(int idx)
{
    sPopupEntry18* ta = &D_004C6C08[idx];
    sPopupEntry14* target = &D_00441630[ta->field_0x4];
    int r = 0;
    int i = 0;
    while (i <= idx) {
        sPopupEntry18* a = &D_004C6C08[i];
        sPopupEntry14* e = &D_00441630[a->field_0x4];
        int start = *(int*)((char*)e + 4);
        int count = *(int*)((char*)e + 8);
        if (start + count - 1 < idx) {
            if (e->field_0xC == target->field_0xC) r += count;
            i += count;
        } else {
            if (e->field_0xC == target->field_0xC) r += idx - start + 1;
            break;
        }
    }
    return r - 1;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E2EA0);

INCLUDE_ASM("fe/fepopupmisc", func_001E2FE0);

INCLUDE_ASM("fe/fepopupmisc", func_001E3100);

INCLUDE_ASM("fe/fepopupmisc", func_001E31B8);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3268);
#ifdef SKIP_ASM
struct sPopupListEntry {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList {
    sPopupListEntry entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList* func_001E39F8_list() __asm__("func_001E39F8");
extern "C" void func_001E31B8(int idx);

extern "C" void func_001E3268(int id)
{
    sPopupList* t = func_001E39F8_list();
    int i;
    for (i = 0; i < t->count; i++) {
        if (id == t->entries[i].id) {
            func_001E31B8(i);
            break;
        }
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E32C8);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3388);
#ifdef SKIP_ASM
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList* func_001E39F8_list() __asm__("func_001E39F8");

extern "C" int func_001E3388(int id)
{
    sPopupList* t = func_001E39F8_list();
    int i;
    for (i = 0; i < t->count; i++) {
        if (id == t->entries[i].id) {
            return i;
        }
    }
    return -1;
}
#endif

extern "C" void* func_001E39F8(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E33E8__FPv);
#ifdef SKIP_ASM
unsigned char func_001E33E8(void* self)
{
    return *(unsigned char*)((char*)func_001E39F8(self) + 0xcc);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3408);
#ifdef SKIP_ASM
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList* func_001E39F8_list() __asm__("func_001E39F8");

extern "C" int func_001E3408()
{
    sPopupList* t = func_001E39F8_list();
    int n = 0;
    int i;
    for (i = 0; i < t->count; i++) {
        int bit = 1 << i;
        if (!(t->mask & bit)) {
            n++;
        }
    }
    return n;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E3510);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3570);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00147580(void* iface, int a1);
extern signed char D_00535BC8[];

extern "C" int func_001E3570(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535BC8[0x49] != 0) {
        return 0;
    }
    if (D_00535BC8[0x4A] == 6 || D_00535BC8[0x4A] == 9) {
        return 0;
    }
    return func_00147580(cBE_getInterface_Fv(cBE_getBE(), 1), 0) == 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3620);
#ifdef SKIP_ASM
struct sPopupEntry8 {
    int field_0x0;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupTable8 {
    sPopupEntry8 entries[1];
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupTable8* func_001E39F8_get() __asm__("func_001E39F8");

extern "C" int func_001E3620(int idx)
{
    return func_001E39F8_get()->entries[idx].field_0x0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3650);
#ifdef SKIP_ASM
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupTable8* func_001E39F8_get() __asm__("func_001E39F8");

extern "C" unsigned char func_001E3650(int idx)
{
    return func_001E39F8_get()->entries[idx].field_0x4;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E3680);

INCLUDE_ASM("fe/fepopupmisc", func_001E3760);

INCLUDE_ASM("fe/fepopupmisc", func_001E38B8);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E39F8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_00147908(void* iface, int a1);

extern "C" void* func_001E39F8(void* self)
{
    return func_00147908(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3A30);
#ifdef SKIP_ASM
extern "C" int func_00147980(void* iface, int a1, void* a2);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int func_001E3A30(void* self)
{
    return func_00147980(cBE_getInterface_Fv(cBE_getBE(), 1), 0, self) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3A78);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00147A30(void* iface, int a1, int a2, int a3);

extern "C" void func_001E3A78(int a, int b)
{
    func_00147A30(cBE_getInterface_Fv(cBE_getBE(), 1), 0, a, b);
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E3C00);

INCLUDE_ASM("fe/fepopupmisc", func_001E3D58);

INCLUDE_ASM("fe/fepopupmisc", func_001E3E30);

INCLUDE_ASM("fe/fepopupmisc", func_001E3EC0);

INCLUDE_ASM("fe/fepopupmisc", func_001E4168);

INCLUDE_ASM("fe/fepopupmisc", func_001E4200);

INCLUDE_ASM("fe/fepopupmisc", func_001E4458);

INCLUDE_ASM("fe/fepopupmisc", func_001E4578);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4610);
#ifdef SKIP_ASM
extern "C" void func_001E48B0(void*);
extern "C" void func_001E4758(void*);
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001E4610(void* self, int a1, int a2)
{
    if (a2 == 0x14) {
        func_001E48B0(self);
        func_001E4758(self);
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E4658);

INCLUDE_ASM("fe/fepopupmisc", func_001E4758);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4840);
#ifdef SKIP_ASM
struct sVEntry_func_001E4840 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E4840(void* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001E4840* vt = *(sVEntry_func_001E4840**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4878);
#ifdef SKIP_ASM
struct sVEntry_func_001E4878 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E4878(void* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001E4878* vt = *(sVEntry_func_001E4878**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E48B0);

INCLUDE_ASM("fe/fepopupmisc", func_001E4B80);

INCLUDE_ASM("fe/fepopupmisc", func_001E4C78);

INCLUDE_ASM("fe/fepopupmisc", func_001E4F70);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E50C8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001E51D0(void* self, void* a1, void* owner, int a3, int a4);
extern "C" void func_0039F290(void* list, void* item);
extern char D_0046E240[];

extern "C" void func_001E50C8(void* self, int a1, int a2)
{
    void* item = func_001E51D0(cMemMan_alloc(0x70, D_0046E240, 0, 0), *(void**)((char*)self + 0x10), self, a1, a2);
    void* a = *(void**)((char*)self + 0x17C);
    void* b = *(void**)((char*)a + 0xD0);
    void* c = *(void**)((char*)b + 0x10);
    func_0039F290((char*)c + 0x18, item);
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E5148);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E51D0);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern void* D_00474248[];

extern "C" void* func_001E51D0(void* self, void* engine, void* owner, int a3, int a4)
{
    func_0039E318(self, engine, owner);
    *(int*)((char*)self + 0x68) = a3;
    *(int*)((char*)self + 0x6C) = a4;
    *(void***)((char*)self + 0x8) = D_00474248;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x50) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x58) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E5248);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046E258[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void func_001E5248(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046E258), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

extern "C" void* func_0039E510(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E52B0__FPv);
#ifdef SKIP_ASM
void* func_001E52B0(void* self)
{
    return func_0039E510(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E52D0);
#ifdef SKIP_ASM
extern "C" void func_001E5428(void* self);

extern "C" int func_001E52D0(void* self, int on)
{
    if (on != 0) {
        func_001E5428(self);
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E52F8);

INCLUDE_ASM("fe/fepopupmisc", func_001E5428);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E55D0);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void func_001E5638(void* self, int idx);

extern "C" void func_001E55D0(void* self, int idx)
{
    char* e = (char*)&D_004C6C08[idx];
    if (*(int*)(e + 0x8) == 1) {
        func_001E5638(self, idx);
    } else {
        cUIText* t = *(cUIText**)((char*)self + 0x54);
        if (t != 0) {
            cUIText_setUnicodeStringByID(t, *(int*)(e + 0x10));
        }
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E5638);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E5780);
#ifdef SKIP_ASM
struct sVEntry001E5780 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E5780(void* self, int idx)
{
    char* e = (char*)&D_004C6C08[idx];
    int kind = *(int*)(e + 0x8);
    if (kind < 2) {
        if (kind >= 0) {
            char* t = *(char**)((char*)self + 0x58);
            *(int*)(t + 0x14) |= 0x80;
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x58), *(int*)(e + 0x14));
            void* obj = *(void**)((char*)self + 0x58);
            sVEntry001E5780* vt = *(sVEntry001E5780**)((char*)obj + 8);
            vt[9].fn((char*)obj + vt[9].delta, 1);
        }
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E5800);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E5A18);
#ifdef SKIP_ASM
extern "C" int func_001E5A18(void* self, int a1, unsigned int a2)
{
    switch (a2) {
    case 6:
    case 7:
        return 1;
    case 9:
        return 0;
    case 8: {
        sPopupEntry18* a = &D_004C6C08[*(int*)((char*)self + 0x68)];
        sPopupEntry14* b = &D_00441630[a->field_0x4];
        return *(int*)b->pad_0x0 != 4;
    }
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E5AA0);

INCLUDE_ASM("fe/fepopupmisc", func_001E5B80);

extern "C" void* func_0020E900(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E64A8__FPv);
#ifdef SKIP_ASM
void* func_001E64A8(void* self)
{
    return func_0020E900(self);
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E64C8);

INCLUDE_ASM("fe/fepopupmisc", func_001E65B8);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E6630);
#ifdef SKIP_ASM
extern "C" int func_001E6630(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E6640);

INCLUDE_ASM("fe/fepopupmisc", func_001E6668);

INCLUDE_ASM("fe/fepopupmisc", func_001E6718);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7558);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046E510[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern signed char D_00535C11[];

extern "C" void func_001E7558(void* self, void* menu)
{
    int hash = *(int*)((char*)menu + 0x38);
    if (hash == GetHashValue32(D_0046E510)) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 1) {
            cUIMenu_setSelectedByIndex(menu, 1);
        }
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E75D0);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7920__FPv);
#ifdef SKIP_ASM
void func_001E7920(void* self)
{
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E7928);

INCLUDE_ASM("fe/fepopupmisc", func_001E7AA8);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7B78);
#ifdef SKIP_ASM
extern "C" int func_001E7B78(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E7B88);

INCLUDE_ASM("fe/fepopupmisc", func_001E7BB0);

INCLUDE_ASM("fe/fepopupmisc", func_001E8098);

INCLUDE_ASM("fe/fepopupmisc", func_001E8160);

//88.13% - logic verified correct; GCC allocates v0 to the comparison
// constant and v1 to the result, the target does the reverse. No source
// shape tried (temp local, early return, nested ternary) flips it.
INCLUDE_ASM("fe/fepopupmisc", func_001E81B0);
#ifdef SKIP_ASM
extern "C" int func_001E81B0(void* self, int a1, int a2)
{
    int r = 0;
    if (a2 == 6) {
        r = *(int*)((char*)self + 0xa0) != 0 ? 0x101 : 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E81D0);
#ifdef SKIP_ASM
extern "C" int func_001E81D0(void* self, int a1)
{
    switch (a1) {
    case 0:
        *(int*)((char*)self + 0xA0) = 0;
        break;
    case 1:
        *(int*)((char*)self + 0xA0) = 1;
        break;
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E8200);

INCLUDE_ASM("fe/fepopupmisc", func_001E87B8);

INCLUDE_ASM("fe/fepopupmisc", func_001E8880);

INCLUDE_ASM("fe/fepopupmisc", func_001E88D0);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E88F0);
#ifdef SKIP_ASM
extern "C" int func_001E88F0(void* self, int a1)
{
    switch (a1) {
    case 0:
        *(int*)((char*)self + 0xA0) = 0;
        break;
    case 1:
        *(int*)((char*)self + 0xA0) = 1;
        break;
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E8920);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8E70__FPv);
#ifdef SKIP_ASM
void* func_001E8E70(void* self)
{
    return func_0020E900(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8E90__FPv);
#ifdef SKIP_ASM
int func_001E8E90(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8E98);
#ifdef SKIP_ASM
extern "C" int func_001E8E98(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E8EA8);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8ED0);
#ifdef SKIP_ASM
extern "C" void* func_0039E4C0(void* self);
extern "C" void* func_0028B180();
extern "C" void func_00294F78(void*, int);

extern "C" void func_001E8ED0(void* self)
{
    func_0039E4C0(self);
    func_00294F78(func_0028B180(), 0xE);
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E8F00);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8FC0__FPv);
#ifdef SKIP_ASM
void func_001E8FC0(void* self)
{
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E8FC8);


#include "common.h"

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupVideoCalibration_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_004A1548[];
extern char* D_004A1540;
extern char D_0045DD90[];
extern char D_0045DDA0[];

class cUIObjK1DEB90 {
public:
    char pad_0x000[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int);
};

extern "C" void cFEPopupVideoCalibration_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A1548), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIObjK1DEB90* obj = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1540));
    if (obj != 0) {
        obj->v09(1);
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045DD90));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_0045DDA0));
    }
    *(char*)((char*)self + 0x48) = 0;
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF4E8);
#ifdef SKIP_ASM
extern void* D_0046AF68[];
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

struct sVEntry001DF4E8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001DF4E8(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046AF68;
    if ((char*)self + 0x6D0 != 0) {
        char* p = (char*)self + 0x6D0 + 0x68;
        while ((char*)self + 0x6D0 != p) {
            p -= 0x34;
            sVEntry001DF4E8* vt = *(sVEntry001DF4E8**)(p + 0x30);
            vt[1].fn(p + vt[1].delta, 0);
        }
    }
    func_001A85D0_dtor(self, flags);
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_0039B760(void* self, int a1);
extern char D_00460020[];
extern char D_004A13A0[];
extern char D_004A14F8[];

extern "C" void cFEPopupSelectMultiplayerMode_onWidgetCreate(void* self, void* item)
{
    int h1 = *(int*)((char*)item + 0x38);
    if (h1 == GetHashValue32(D_00460020)) {
        *(int*)((char*)item + 0x14) |= 1;
        func_0039B760(item, 2);
        *(int*)((char*)item + 0x14) |= 0x80;
        return;
    }
    int h2 = *(int*)((char*)item + 0x38);
    if (h2 == GetHashValue32(D_004A13A0)) {
        *(int*)((char*)item + 0x18) = 0;
        return;
    }
    int h3 = *(int*)((char*)item + 0x38);
    if (h3 == GetHashValue32(D_004A14F8)) {
        *(int*)((char*)item + 0x18) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E03F8);
#ifdef SKIP_ASM
extern int D_004A19D4;
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001E03F8(void* self, void* a1, int a2)
{
    switch (a2) {
    case 5: {
        char* p = *(char**)((char*)self + 0x10);
        *(char*)((char*)self + 0x48) = 1;
        D_004A19D4 = *(int*)((char*)a1 + 0x18) != 0;
        func_0039F190(p + 0x18, 1);
        break;
    }
    case 6: {
        char* p = *(char**)((char*)self + 0x10);
        *(char*)((char*)self + 0x48) = 0;
        func_0039F190(p + 0x18, 1);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0470);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

extern "C" int func_001E0470(void* self, void* menu, unsigned int key, int idx)
{
    switch (key) {
    default:
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        if (key < 6) {
            return 0x101;
        }
        break;
    case 2:
        cUIMenu_setSelectedByIndex(menu, idx - 1);
    case 3:
        cUIMenu_setSelectedByIndex(menu, idx + 1);
        break;
    }
    return 0x101;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E1458);
#ifdef SKIP_ASM
extern int D_004A20C0;
extern int D_004A20C8;
extern int D_004A20BC;
extern void* D_004A5B64;
extern "C" void func_001E12F0(void);
extern "C" void func_001E14C0(int type);

extern "C" void func_001E1458(void)
{
    if (D_004A20C0 == 0) {
        if (D_004A20C8 == 0) {
            D_004A20C8 = 1;
            func_001E12F0();
        }
        int t = *(int*)((char*)D_004A5B64 + 0x1C);
        if (t - D_004A20BC > 0x258) {
            func_001E14C0(1);
            D_004A20BC = t;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E14C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001E3A30(void* self);
void func_001E1548(void* self);

struct sEntry14_14C0 {
    int type;
    char pad_0x4[0x14 - 0x4];
};
extern sEntry14_14C0 D_00441630_14C0[] __asm__("D_00441630");

extern "C" void func_001E14C0(int type)
{
    int i;
    for (i = 0; i < 0x3D; i++) {
        if (func_001E3A30((void*)i) == 0) {
            int t = D_00441630_14C0[i].type;
            if (t == type || t == 5) {
                func_001E1548((void*)i);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1548__FPv);
#ifdef SKIP_ASM
void func_001E1548(void* self)
{
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E1550);

INCLUDE_ASM("fe/fepopupmisc", func_001E1A30);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E1AD0);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
unsigned int BXrand();
extern "C" int func_001E1A30(int kind, int arg);
extern "C" int func_001E3388(int id);
extern char D_0046DDF8[];

struct sPopupEntry18_1AD0 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
};
extern sPopupEntry18_1AD0 D_004C6C08_1AD0[] __asm__("D_004C6C08");

struct sRangeK1E1AD0 {
    int f0;
    int start;   // 0x4
    int count;   // 0x8
};

extern "C" int func_001E1AD0(sRangeK1E1AD0* r, int arg)
{
    int* list = (int*)operator_new_tag(r->count * 4, D_0046DDF8, 0x100, 0);
    int n = 0;
    int i;
    for (i = r->start; i < r->count + r->start; i++) {
        if (func_001E1A30(D_004C6C08_1AD0[i].fC, arg) == 0 && func_001E3388(i) == -1) {
            list[n++] = i;
        }
    }
    if (n == 0)
        return 0x100;
    int id = list[BXrand() % n];
    if (list != 0)
        cMemMan_free(list);
    return id;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E1C10);

INCLUDE_ASM("fe/fepopupmisc", func_001E1DD0);

INCLUDE_ASM("fe/fepopupmisc", func_001E1EB8);

INCLUDE_ASM("fe/fepopupmisc", func_001E20D0);

INCLUDE_ASM("fe/fepopupmisc", func_001E2220);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2370);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_001E3A78(int a, int b);

extern "C" void func_001E2370(int which)
{
    int n = **(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            func_001E3A78(0x1C, 1);
            func_001E3A78(0x1D, 1);
            break;
        case 1:
            func_001E3A78(0x1E, 1);
            func_001E3A78(0x1F, 1);
            break;
        case 2:
            func_001E3A78(0x20, 1);
            func_001E3A78(0x21, 1);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2428);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E2428(int which)
{
    int n = **(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            if (func_001E3A30((void*)0x39) == 0) {
                func_001E3A78(0x39, 1);
                func_001E2FE0(0xFC, 0);
            }
            break;
        case 1:
            if (func_001E3A30((void*)0x3A) == 0) {
                func_001E3A78(0x39, 1);
                func_001E2FE0(0xFD, 0);
            }
            break;
        case 2:
            if (func_001E3A30((void*)0x3B) == 0) {
                func_001E3A78(0x3B, 1);
                func_001E2FE0(0xFE, 0);
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2518);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E2518(void)
{
    int n = **(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
    int busy = n != 0 && n < 10;
    if (busy == 0 && func_001E3A30((void*)0x3C) == 0) {
        func_001E3A78(0x3C, 1);
        func_001E2FE0(0xFF, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2578);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E2578(void)
{
    int n = **(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
    int busy = n != 0 && n < 10;
    if (busy == 0 && func_001E3A30((void*)0x38) == 0) {
        func_001E3A78(0x38, 1);
        func_001E2FE0(0xFB, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E25D8);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_001E2648(void* a, int b);
extern "C" void func_001E2828(void* a, int b);

extern "C" void func_001E25D8(int key, void* a1, int a2)
{
    int n = **(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (key) {
        case 4:
            func_001E2648(a1, a2);
            break;
        case 5:
            func_001E2828(a1, a2);
            break;
        }
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E2648);

INCLUDE_ASM("fe/fepopupmisc", func_001E2828);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2A08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004A28A8;
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00146E98(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int func_001E2B38(int charID);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);
unsigned int BXrand();

struct sEntry14_2A08 {
    int type;
    int start;   // 0x4
    int count;   // 0x8
    char pad_0xC[0x14 - 0xC];
};
extern sEntry14_2A08 D_00441630_2A08[] __asm__("D_00441630");

extern "C" void func_001E2A08(int charID, int level)
{
    int n = **(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
    int busy = n != 0 && n < 10;
    if (busy)
        return;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (cBENewPlayerInterface_getRiderCharID(iface, func_00146E98(iface, 0)) == charID)
        return;
    int id = func_001E2B38(charID);
    sEntry14_2A08* e = &D_00441630_2A08[id];
    if (func_001E3A30((void*)id)) {
        if (level < 5)
            func_001E3A78(id, 0);
    } else if ((int)((BXrand() & 7) + 0xF) < level) {
        func_001E2FE0(e->start + BXrand() % e->count, 0);
        func_001E3A78(id, 1);
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E2B38);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2BB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001E3A30(void* self);

struct sEntry14_2BB8 {
    int type;
    int f4;
    int count;
    int fC;
    int f10;
};
extern sEntry14_2BB8 D_00441630_2BB8[] __asm__("D_00441630");

extern "C" int func_001E2BB8(int type, int all)
{
    int total = 0;
    int i;
    for (i = 0; i < 0x3D; i++) {
        sEntry14_2BB8* e = &D_00441630_2BB8[i];
        if (e->type == type) {
            if (all != 0 || (e->f10 != 0 && func_001E3A30((void*)i) != 0)) {
                total += e->count;
            } else {
                total += 1;
            }
        }
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2C80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001E3A30(void* self);

struct sEntry14_2C80 {
    int type;
    int first;
    int count;
    int fC;
    int f10;
};
extern sEntry14_2C80 D_00441630_2C80[] __asm__("D_00441630");

extern "C" int func_001E2C80(int type, int idx, int mode)
{
    int base = 0;
    int result = -1;
    int i;
    for (i = 0; i < 0x3D; i++) {
        sEntry14_2C80* e = &D_00441630_2C80[i];
        if (e->type == type) {
            int n;
            if (mode == 0 || e->f10 == 0 || func_001E3A30((void*)i) != 0) {
                n = e->count;
            } else {
                n = 1;
            }
            n += base;
            if (idx < n) {
                result = e->first + (idx - base);
                goto done;
            }
            base = n;
        }
    }
done:
    return result;
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2FE0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern void* D_004A28A8;
extern char D_004A20D0[];
extern "C" void func_001E31B8(int idx);
extern "C" unsigned char func_001E3100(int i);
extern "C" void* func_0039F9D8(void* list, int hash);

struct sPopupListEntry_2FE0 {
    int id;
    unsigned char flag;
    char pad_0x5[3];
};
struct sPopupList_2FE0 {
    sPopupListEntry_2FE0 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList_2FE0* func_001E39F8_list2FE0() __asm__("func_001E39F8");

struct sPopupEntry18_2FE0 {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    char pad_0xC[0x18 - 0xC];
};
extern sPopupEntry18_2FE0 D_004C6C08_2FE0[] __asm__("D_004C6C08");

struct sVEntryK1E2FE0 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" void func_001E2FE0(int id, unsigned char flag)
{
    sPopupList_2FE0* t = func_001E39F8_list2FE0();
    if (t->count == 0x19)
        func_001E31B8(0);
    if (D_004C6C08_2FE0[id].field_0x8 == 1)
        flag = func_001E3100(id);
    t->entries[t->count].id = id;
    t->entries[t->count].flag = flag;
    t->mask &= ~(1 << t->count);
    char* o = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x48);
    if (o != 0) {
        char* obj = (char*)func_0039F9D8(o + 0x18, GetHashValue32(D_004A20D0));
        if (obj != 0) {
            sVEntryK1E2FE0* vt = *(sVEntryK1E2FE0**)(obj + 8);
            vt[24].fn(obj + vt[24].delta, 8, 0);
        }
    }
    t->count++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E3100);
#ifdef SKIP_ASM
extern "C" int func_001E2D58(int v, int i);
unsigned int BXrand();

extern "C" unsigned char func_001E3100(int i)
{
    unsigned char r = 0xFF;
    if (func_001E2D58(3, i)) {
        r = BXrand() % 10;
    } else if (func_001E2D58(5, i)) {
        r = BXrand() % 11;
    } else if (func_001E2D58(8, i)) {
        r = BXrand() % 4;
    } else if (func_001E2D58(4, i)) {
        r = BXrand() % 10;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E31B8);
#ifdef SKIP_ASM
struct sPopupListEntry_31B8 {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList_31B8 {
    sPopupListEntry_31B8 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList_31B8* func_001E39F8_list31B8() __asm__("func_001E39F8");

extern "C" void func_001E31B8(int idx)
{
    sPopupList_31B8* t = func_001E39F8_list31B8();
    int i;
    for (i = idx; i < t->count - 1; i++) {
        t->entries[i] = t->entries[i + 1];
        int bit = 1 << (i + 1);
        if (t->mask & bit) {
            t->mask |= 1 << i;
        } else {
            t->mask &= ~(1 << i);
        }
    }
    t->count--;
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E32C8);
#ifdef SKIP_ASM
struct sPopupEntry18_32C8 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18_32C8 D_004C6C08_32C8[] __asm__("D_004C6C08");
struct sPopupListEntry_32C8 {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList_32C8 {
    sPopupListEntry_32C8 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList_32C8* func_001E39F8_list32C8() __asm__("func_001E39F8");
extern "C" void func_001E31B8(int idx);

extern "C" void func_001E32C8(int type)
{
    int n = 0;
    int j = 0;
    sPopupList_32C8* t = func_001E39F8_list32C8();
    for (; n < t->count; n++) {
        sPopupEntry18_32C8* a = &D_004C6C08_32C8[t->entries[j].id];
        if (type == a->field_0x4) {
            func_001E31B8(j);
        } else {
            j++;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3510);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001475C0(void* iface, int a1, int a2);
extern int D_004A20CC;
extern signed char D_00535C11[];

extern "C" void func_001E3510(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 0) {
        D_004A20CC = 0;
        func_001475C0(cBE_getInterface_Fv(cBE_getBE(), 1), 0, 1);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3680);
#ifdef SKIP_ASM
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern int D_004A20C4;

extern "C" unsigned char func_001E3680(int id)
{
    if (D_004A20C4 != 0) {
        return id;
    }
    int n1 = func_001E2BB8(4, 0);
    int n2 = func_001E33E8_noarg();
    int i;
    for (i = 0; i < n1 + n2; i++) {
        int k = i + 1;
        int idx = n2 - k;
        if (idx >= 0) {
            if (func_001E3620(idx) == id) {
                return i;
            }
        } else if (idx + n1 >= 0) {
            if (func_001E2C80(4, ~idx, 1) == id) {
                return i;
            }
        }
    }
    return 0xFF;
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E3760);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E38B8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00146E98(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* self, int rider, int peak);
extern "C" int func_00157BF0(void* self, int rider, int a, int b, int c);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E38B8(int player)
{
    if (player == 0) {
        void* stats = cBE_getInterface_Fv(cBE_getBE(), 0xD);
        void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
        int charID = cBENewPlayerInterface_getRiderCharID(np, func_00146E98(np, 0));
        if (cBENewPlayerInterface_isPeakLocked1(np, player, 1)) {
            if (func_00157BF0(stats, player, charID, 0, 2) && func_001E3A30((void*)0x36) == 0) {
                func_001E3A78(0x36, 1);
                func_001E2FE0(0xF9, 0);
            }
        }
        if (cBENewPlayerInterface_isPeakLocked1(np, player, 2)) {
            if (func_00157BF0(stats, player, charID, 1, 2) && func_001E3A30((void*)0x37) == 0) {
                func_001E3A78(0x37, 1);
                func_001E2FE0(0xFA, 0);
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3D58);
#ifdef SKIP_ASM
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" void func_0020A380(void* self);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void func_001E48B0(void*);
extern "C" void func_001E4758(void*);
extern "C" void func_001E4458(void*);
extern "C" void func_001E4658(void*);
extern char D_0046DF88[];

extern "C" void func_001E3D58(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046DF88), 0);
    *(void**)((char*)self + 0x17C) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0020A380(self);
    int mode = *(int*)((char*)self + 0x18);
    if (mode == 0) {
        goto add;
    }
    if (mode == 1) {
    add:
        cOVStateManager_addPDATemplate();
    }
    *(int*)((char*)self + 0x174) = 0;
    int n = func_001E33E8_noarg();
    n += func_001E2BB8(4, 1);
    func_0039B760(*(void**)((char*)self + 0x144), (unsigned char)n);
    func_001E48B0(self);
    func_001E4758(self);
    func_001E4458(self);
    func_001E4658(self);
    D_004A20C0 = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3E30);
#ifdef SKIP_ASM
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_0020D190(void);
extern "C" void func_0020A430(void* self);
extern int D_004A2A54;
extern int D_004A2A50;
extern int D_005366E8[];
extern int D_004428F0[];

extern "C" void func_001E3E30(void* self)
{
    int s = *(int*)((char*)self + 0x18);
    if (s == 1) {
        func_001E3A78(7, 0);
        func_0020D190();
        D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
    } else if (s == 0) {
        func_001E3A78(7, 1);
        func_0020D190();
        D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
    }
    func_0020A430(self);
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E3EC0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E4168);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" unsigned char func_001E3680(int id);
extern "C" void func_001E50C8(void* self, int a1, int a2);

struct sVEntry001E4168 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E4168(void* self)
{
    int state = *(int*)((char*)self + 0x18);
    if (state == 0) {
        goto reset;
    }
    if (state == 1) {
    reset:
        cUIMenu_setSelectedByIndex(*(void**)((char*)self + 0x144), func_001E3680(0x25));
        void* menu = *(void**)((char*)self + 0x144);
        sVEntry001E4168* vt = *(sVEntry001E4168**)((char*)menu + 8);
        vt[7].fn((char*)menu + vt[7].delta, 1);
        func_001E50C8(self, 0x25, -1);
    } else {
        void* menu = *(void**)((char*)self + 0x144);
        sVEntry001E4168* vt = *(sVEntry001E4168**)((char*)menu + 8);
        vt[7].fn((char*)menu + vt[7].delta, 1);
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E4200);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4458);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" int func_001E2C80(int type, int idx, int mode);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E4840(void* self, void* obj);
extern "C" void func_001E4878(void* self, void* obj);
extern char D_0046E060[];
extern char D_0046E070[];
extern char D_0046E080[];

struct sEntry18_4458 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x18 - 0x8];
};
extern sEntry18_4458 D_004C6C08_4458[] __asm__("D_004C6C08");
struct sEntry14_4458 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_4458 D_00441630_4458[] __asm__("D_00441630");

extern "C" void func_001E4458(void* self)
{
    int base = *(unsigned char*)(*(char**)((char*)self + 0x144) + 0x95);
    int diff = func_001E33E8_noarg() - base;
    if (diff - 1 >= 0) {
        func_001E4878(self, *(void**)((char*)self + 0x14C));
        func_001E4878(self, *(void**)((char*)self + 0x150));
        if (*(cUIText**)((char*)self + 0x158) != 0)
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x158), GetHashValue32(D_0046E060));
    } else {
        func_001E4840(self, *(void**)((char*)self + 0x14C));
        func_001E4840(self, *(void**)((char*)self + 0x150));
        if (*(cUIText**)((char*)self + 0x158) != 0) {
            int id = func_001E2C80(4, -diff, 1);
            sEntry18_4458* a = &D_004C6C08_4458[id];
            int t = a->field_0x4;
            sEntry14_4458* b = &D_00441630_4458[t];
            if (id == b->field_0x4) {
                if (func_001E3A30((void*)t))
                    cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x158), GetHashValue32(D_0046E070));
                else
                    cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x158), GetHashValue32(D_0046E080));
            } else {
                cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x158), GetHashValue32(D_0046E060));
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4658);
#ifdef SKIP_ASM
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" int func_001E2BB8(int type, int all);
extern int D_004A20C4;

struct sVEntryK1E4658 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E4658(void* self)
{
    if (*(int*)((char*)self + 0x174) == 0) {
        char* o = *(char**)((char*)self + 0x15C);
        sVEntryK1E4658* vt = *(sVEntryK1E4658**)(o + 8);
        vt[9].fn(o + vt[9].delta, 0);
    } else {
        char* o = *(char**)((char*)self + 0x15C);
        sVEntryK1E4658* vt = *(sVEntryK1E4658**)(o + 8);
        vt[9].fn(o + vt[9].delta, 1);
    }
    int n;
    if (D_004A20C4 != 0) {
        n = func_001E33E8_noarg();
        n += func_001E2BB8(4, 1);
    } else {
        n = func_001E33E8_noarg();
        n += func_001E2BB8(4, 0);
    }
    if (*(int*)((char*)self + 0x174) + 8 >= n) {
        char* o = *(char**)((char*)self + 0x160);
        sVEntryK1E4658* vt = *(sVEntryK1E4658**)(o + 8);
        vt[9].fn(o + vt[9].delta, 0);
    } else {
        char* o = *(char**)((char*)self + 0x160);
        sVEntryK1E4658* vt = *(sVEntryK1E4658**)(o + 8);
        vt[9].fn(o + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4758);
#ifdef SKIP_ASM
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" void func_002C26D0(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void func_003A0E90(void* text, void* p);
extern void* D_004A28A8;
extern char D_0046E0A8[];
extern char D_0046E0C0[];

struct sVEntry001E4758 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

extern "C" void func_001E4758(void* self)
{
    unsigned short buf[0x38];
    if (*(void**)((char*)self + 0x13C) != 0) {
        char* o = *(char**)((char*)D_004A28A8 + 0x8C);
        sVEntry001E4758* vt = *(sVEntry001E4758**)(o + 4);
        unsigned short* fmt = vt[4].fn(o + vt[4].delta, GetHashValue32(D_0046E0A8));
        func_002C26D0(buf, fmt, func_001E33E8_noarg());
        func_003A0E90(*(void**)((char*)self + 0x13C), buf);
    }
    if (*(void**)((char*)self + 0x140) != 0) {
        char* o = *(char**)((char*)D_004A28A8 + 0x8C);
        sVEntry001E4758* vt = *(sVEntry001E4758**)(o + 4);
        unsigned short* fmt = vt[4].fn(o + vt[4].delta, GetHashValue32(D_0046E0C0));
        func_002C26D0(buf, fmt, func_001E3408());
        func_003A0E90(*(void**)((char*)self + 0x140), buf);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4B80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A20C4;
extern "C" int func_001E2DC0(int idx);

struct sPopupEntry18_4B80 {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    char pad_0xC[0x18 - 0xC];
};
extern sPopupEntry18_4B80 D_004C6C08_4B80[] __asm__("D_004C6C08");

extern "C" unsigned char func_001E4B80(void* self, int idx)
{
    unsigned char r = 0xFF;
    if (D_004A20C4 == 0) {
        return 0xFF;
    }
    sPopupEntry18_4B80* a = &D_004C6C08_4B80[idx];
    if (a->field_0x8 == 1) {
        int v = func_001E2DC0(idx);
        int div = 1;
        sPopupEntry14* b = &D_00441630[a->field_0x4];
        switch (b->field_0xC) {
        case 3:
            div = 10;
            v = v / 2;
            break;
        case 5:
            div = 11;
            break;
        case 4:
            div = 10;
            break;
        case 8:
            div = 4;
            break;
        }
        r = v % div;
    }
    return r;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E5148);
#ifdef SKIP_ASM
extern "C" void func_001E31B8(int idx);
extern "C" int func_001E2BB8(int a0, int a1);
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" void func_001E48B0(void*);
extern "C" void func_001E4758(void*);
extern "C" void func_001E4658(void*);

extern "C" void func_001E5148(void* self, int idx)
{
    func_001E31B8(idx);
    int cur = *(unsigned char*)(*(char**)((char*)self + 0x144) + 0x95);
    int n = func_001E2BB8(4, 1);
    n += func_001E33E8_noarg();
    if (cur >= n) {
        cur--;
    }
    *(unsigned char*)(*(char**)((char*)self + 0x144) + 0x95) = cur;
    func_001E48B0(self);
    func_001E4758(self);
    func_001E4658(self);
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E52F8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A2120[];
extern char D_004A2128[];
extern char D_0046E268[];
extern char D_004A2130[];
extern char D_0046E278[];
extern char* D_004A20A8;
extern char D_0046E288[];
extern char D_0046E2A0[];
extern char D_0046E2B0[];

struct sVEntryK1E52F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E52F8(void* self, void* widget)
{
    int h = *(int*)((char*)widget + 0x38);
    sVEntryK1E52F8* vt = *(sVEntryK1E52F8**)((char*)widget + 8);
    vt[9].fn((char*)widget + vt[9].delta, 1);
    if (h == GetHashValue32(D_004A2120)) {
        *(void**)((char*)self + 0x50) = widget;
    } else if (h == GetHashValue32(D_004A2128)) {
        *(void**)((char*)self + 0x54) = widget;
    } else if (h == GetHashValue32(D_0046E268)) {
        *(void**)((char*)self + 0x48) = widget;
    } else if (h == GetHashValue32(D_004A2130)) {
        *(void**)((char*)self + 0x4C) = widget;
    } else if (h == GetHashValue32(D_0046E278)) {
        *(void**)((char*)self + 0x64) = widget;
    } else if (h == GetHashValue32(D_004A20A8)) {
        *(void**)((char*)self + 0x5C) = widget;
    } else if (h == GetHashValue32(D_0046E288)) {
        *(void**)((char*)self + 0x60) = widget;
    } else if (h == GetHashValue32(D_0046E2A0)) {
        *(void**)((char*)self + 0x58) = widget;
    } else if (h == GetHashValue32(D_0046E2B0)) {
        *(int*)((char*)widget + 0x90) |= 8;
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E5800);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPopupEntry18_5800 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18_5800 D_004C6C08_5800[] __asm__("D_004C6C08");
struct sEntry14_5800 {
    int type;
    char pad_0x4[0x14 - 0x4];
};
extern sEntry14_5800 D_00441630_5800[] __asm__("D_00441630");
extern "C" void func_001E3268(int id);
struct sVE5800 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001E5800(void* self, void* a1, unsigned int msg)
{
    switch (msg) {
    case 7: {
        int id = *(int*)((char*)self + 0x68);
        sPopupEntry18_5800* a = &D_004C6C08_5800[id];
        sEntry14_5800* b = &D_00441630_5800[a->field_0x4];
        if (b->type == 4) {
            return;
        }
        func_001E3268(id);
    }
    case 5: {
        char* o = *(char**)((char*)self + 0x20);
        sVE5800* vt = *(sVE5800**)(o + 8);
        vt[17].fn(o + vt[17].delta, self, 0x14);
        break;
    }
    case 6: {
        char* o = *(char**)((char*)self + 0x20);
        sVE5800* vt = *(sVE5800**)(o + 8);
        vt[17].fn(o + vt[17].delta, self, 0x14);
        break;
    }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E5AA0);
#ifdef SKIP_ASM
extern int D_004A20C4;

extern "C" unsigned char func_001E5AA0(void* self, int idx)
{
    if (D_004A20C4 == 0) {
        return 0xFF;
    }
    int v = func_001E2DC0(idx);
    int div = 1;
    sPopupEntry18* a = &D_004C6C08[idx];
    sPopupEntry14* b = &D_00441630[a->field_0x4];
    switch (b->field_0xC) {
    case 3:
        div = 10;
        v = v / 2;
        break;
    case 5:
        div = 11;
        break;
    case 4:
        div = 10;
        break;
    case 8:
        div = 4;
        break;
    }
    return v % div;
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E64C8);
#ifdef SKIP_ASM
// func_0039E4C0 returns nothing (ends in a void vcall); the unit's later void* declaration is a guess.
void func_0039E4C0_v(void* self) __asm__("func_0039E4C0");
extern "C" void* func_0028B180();
extern "C" void func_00294F78(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00146E98(void* iface, int a1);
extern "C" void func_0020A6F0(void* self, char* text, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern void* D_004A28A8;
extern int D_00536730[];
extern char D_0046E418[];
extern char D_0046E4E8[];
extern char D_0046E500[];

extern "C" void func_001E64C8(void* self)
{
    char buf[32];
    func_0039E4C0_v(self);
    int* p = *(int**)((char*)D_004A28A8 + 0xC0);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    int x = *p;
    int n = D_00536730[func_00146E98(iface, 0)] + 1;
    if (n >= 7) {
        n = 6;
    }
    sprintf(buf, D_0046E418, n);
    func_0020A6F0(self, buf, D_0046E4E8);
    sprintf(buf, D_0046E500, x, n);
    func_0020A6F0(self, buf, D_0046E4E8);
    func_00294F78(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E65B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046E510[];
extern void* D_004A28A8;
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void func_0026F898(void* self, int val);

extern "C" int func_001E65B8(void* self, void* a1)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046E510));
    if (obj != 0) {
        if (*(unsigned char*)((char*)obj + 0x95) == 2) {
            func_0026F898(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28), *(unsigned char*)((char*)a1 + 4));
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E6630);
#ifdef SKIP_ASM
extern "C" int func_001E6630(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E6640);
#ifdef SKIP_ASM
extern int D_004A26FC;
extern int D_004A2700;

extern "C" void func_001E6640(void* self, void* a1, int a2)
{
    if (a1 != 0 && a2 == 5) {
        int v = *(int*)((char*)a1 + 0x18);
        D_004A26FC = 1;
        D_004A2700 = v;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E6668);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046E520[];
extern char D_0046E378[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0020A380(void* self);

extern "C" void func_001E6668(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046E520), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046E378), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0020A380(self);
    *(int*)((char*)self + 0x9C) = 1;
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7AA8);
#ifdef SKIP_ASM
extern "C" void func_0020AB50(int state);
extern int D_00534B30[];
extern int* D_004A2EEC;

struct sVEntry001E7AA8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_001E7AA8(void* self, void* a1)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046E510));
    if (obj != 0) {
        if (*(unsigned char*)((char*)obj + 0x95) == 2) {
            func_0026F898(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28), *(unsigned char*)((char*)a1 + 4));
        }
    }
    cBE_getInterface_Fv(*(void**)((char*)D_004A28A8 + 0x78), 7);
    if (D_00534B30[0] != 0 && D_004A2EEC != 0 && *D_004A2EEC != 0) {
        sVEntry001E7AA8* vt = *(sVEntry001E7AA8**)((char*)a1 + 8);
        if (vt[27].fn((char*)a1 + vt[27].delta) != 0) {
            func_0020AB50(0x28);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7B78);
#ifdef SKIP_ASM
extern "C" int func_001E7B78(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7B88);
#ifdef SKIP_ASM
extern int D_004A26FC;
extern int D_004A2700;

extern "C" void func_001E7B88(void* self, void* a1, int a2)
{
    if (a1 != 0 && a2 == 5) {
        int v = *(int*)((char*)a1 + 0x18);
        D_004A26FC = 1;
        D_004A2700 = v;
    }
}
#endif

INCLUDE_ASM("fe/fepopupmisc", func_001E7BB0);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8098);
#ifdef SKIP_ASM
extern "C" int func_00146E98(void* iface, int a1);
extern "C" void func_0039F840(void* list);
extern "C" void func_00231320(void* self);

extern "C" void func_001E8098(void* self)
{
    float limit = 3.0f;
    int idx = func_00146E98(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    char* riders = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC);
    char* rider = *(char**)(riders + (idx << 2) + 0x28);
    if (*(int*)(rider + 0x480) != 0) {
        limit = 1.0f;
    }
    func_0020E900(self);
    *(float*)((char*)self + 0x9C) += 0.01666666753590107f;
    if (*(int*)((char*)self + 0xA0) != 0 && *(float*)((char*)self + 0x9C) >= limit) {
        func_0039F840(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x48) + 0x18);
        func_00231320(*(void**)((char*)D_004A28A8 + 0x84));
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8160);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_0039F840(void* list);
extern "C" void func_00231320(void* self);

extern "C" void func_001E8160(void* self, void* a1, int a2)
{
    if (a1 != 0) {
        if (a2 == 5 && *(int*)((char*)self + 0xA0) != 0) {
            char* g = *(char**)((char*)D_004A28A8 + 0x84);
            func_0039F840(*(char**)(g + 0x48) + 0x18);
            func_00231320(g);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E87B8);
#ifdef SKIP_ASM
extern "C" int func_00146E98(void* iface, int a1);
extern "C" void func_0039F840(void* list);
extern "C" void func_00231320(void* self);

extern "C" void func_001E87B8(void* self)
{
    float limit = 3.0f;
    int idx = func_00146E98(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    char* riders = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC);
    char* rider = *(char**)(riders + (idx << 2) + 0x28);
    if (*(int*)(rider + 0x480) != 0) {
        limit = 1.0f;
    }
    func_0020E900(self);
    *(float*)((char*)self + 0x9C) += 0.01666666753590107f;
    if (*(int*)((char*)self + 0xA0) != 0 && *(float*)((char*)self + 0x9C) >= limit) {
        func_0039F840(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x48) + 0x18);
        func_00231320(*(void**)((char*)D_004A28A8 + 0x84));
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8880);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_0039F840(void* list);
extern "C" void func_00231320(void* self);

extern "C" void func_001E8880(void* self, void* a1, int a2)
{
    if (a1 != 0) {
        if (a2 == 5 && *(int*)((char*)self + 0xA0) != 0) {
            char* g = *(char**)((char*)D_004A28A8 + 0x84);
            func_0039F840(*(char**)(g + 0x48) + 0x18);
            func_00231320(g);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8EA8);
#ifdef SKIP_ASM
extern int D_004A26FC;
extern int D_004A2700;

extern "C" void func_001E8EA8(void* self, void* a1, int a2)
{
    if (a1 != 0 && a2 == 5) {
        int v = *(int*)((char*)a1 + 0x18);
        D_004A26FC = 1;
        D_004A2700 = v;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8F00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046E9C0[];
extern char D_0046E378[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0020A380(void* self);
void func_001E8FC0(void* self);
extern "C" void* func_0028B180();
extern "C" void func_00294F78(void*, int);

extern "C" void func_001E8F00(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046E9C0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046E378), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001E8FC0(self);
    func_0020A380(self);
    func_00294F78(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8FC0__FPv);
#ifdef SKIP_ASM
void func_001E8FC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8FC8);
#ifdef SKIP_ASM
extern int D_004A26FC;
extern int D_004A2700;

extern "C" void func_001E8FC8(void* self, void* a1, int a2)
{
    if (a1 != 0 && a2 == 5) {
        int v = *(int*)((char*)a1 + 0x18);
        D_004A26FC = 1;
        D_004A2700 = v;
    }
}
#endif


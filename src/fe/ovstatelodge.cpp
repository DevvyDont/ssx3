#include "common.h"

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_cFEStateMountainRoom);

INCLUDE_ASM("fe/ovstatelodge", func_001D28F8);

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D2A90);
#ifdef SKIP_ASM
extern "C" void func_001D3340(void* self);
extern "C" void func_00186518(void* self, int a1);

extern "C" void func_001D2A90(void* self, int a1)
{
    func_001D3340(self);
    func_00186518(self, a1);
}
#endif

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onWidgetCreate);

INCLUDE_ASM("fe/ovstatelodge", func_001D2EA0);

INCLUDE_ASM("fe/ovstatelodge", func_001D2F40);

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onWidgetEvent);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3120);
#ifdef SKIP_ASM
extern "C" void func_001D31C0(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001D3120(void* self)
{
    if (~*(int*)((char*)self + 0xBC) != 0) {
        func_001D31C0(self);
    }
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/ovstatelodge", func_001D3160);

INCLUDE_ASM("fe/ovstatelodge", func_001D31C0);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D32E0);
#ifdef SKIP_ASM
struct sVEntry_func_001D32E0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D32E0(void* self)
{
    {
        void* obj = *(void**)((char*)self + 0x48);
        sVEntry_func_001D32E0* vt = *(sVEntry_func_001D32E0**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
    if (*(int*)((char*)self + 0xC4) >= 0) {
        void* obj = *(void**)((char*)self + 0x4C);
        sVEntry_func_001D32E0* vt = *(sVEntry_func_001D32E0**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
}
#endif

INCLUDE_ASM("fe/ovstatelodge", func_001D3340);

INCLUDE_ASM("fe/ovstatelodge", func_001D3780);

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char D_00467648[];

extern "C" void cFEStatePeakRoom_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00467648), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onWidgetCreate);

extern "C" void* func_0039E4C0(void* self);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3C60__FPv);
#ifdef SKIP_ASM
void* func_001D3C60(void* self)
{
    return func_0039E4C0(self);
}
#endif

INCLUDE_ASM("fe/ovstatelodge", func_001D3C80);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3E08);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00157BF0(void* self, int rider, int a, int b, int c);
extern "C" void* func_00397870(void* list, int id);

extern "C" int func_001D3E08(void* self, void* item, unsigned int key, int id)
{
    switch (key) {
    default:
        break;
    case 8:
    case 9:
        return 0x100;
    case 6: {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xD);
        void* e = func_00397870((char*)item + 0x74, id);
        if (func_00157BF0(iface, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xDC),
                          *(int*)((char*)self + 0xE0), *(int*)((char*)e + 0x18)) == 0) {
            return 0x10;
        }
        break;
    }
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_updateHelpText);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00157BF0(void* self, int rider, int a, int b, int c);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_00467688[];
extern char D_004676A0[];

class cPeakWidget_3EC8 {
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

extern "C" void cFEStatePeakRoom_updateHelpText(void* self, int index)
{
    if (*(void**)((char*)self + 0xD0) != 0) {
        if (func_00157BF0(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)((char*)self + 0x44),
                          *(int*)((char*)self + 0xDC), *(int*)((char*)self + 0xE0), index) != 0) {
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0xD0), GetHashValue32(D_00467688));
        } else {
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0xD0), GetHashValue32(D_004676A0));
        }
        (*(cPeakWidget_3EC8**)((char*)self + 0xD0))->show(1);
    }
}
#endif

INCLUDE_ASM("fe/ovstatelodge", func_001D3F80);

INCLUDE_ASM("fe/ovstatelodge", func_001D4268);


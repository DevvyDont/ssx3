#include "common.h"

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onCreateScreen);
#ifdef SKIP_ASM
struct sVec4_181148 { float x, y, z, w; } __attribute__((aligned(16)));
struct sCam_181148 { char pad[0x10]; float fov; };
int GetHashValue32(char* str);
extern char D_0045D600[];
extern void* D_004A28A8;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0015E050(void* cam, sVec4_181148* a, sVec4_181148* b);
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_0019E538(void* self, int a1);

extern "C" void cFEStateCharSelect_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045D600), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    sCam_181148* cam = *(sCam_181148**)((char*)D_004A28A8 + 0x7C);
    sVec4_181148 pos;
    pos.x = 0.0f;
    pos.y = 200.0f;
    pos.z = 0.0f;
    pos.w = 1.0f;
    sVec4_181148 at;
    at.x = 0.0f;
    at.y = 0.0f;
    at.z = 0.0f;
    at.w = 1.0f;
    func_0015E050((char*)cam + 0x10, &pos, &at);
    cam->fov = 0.4363323450088501f;
    func_0019E538(func_001A0548(*(char**)((char*)D_004A28A8 + 0x7C) + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
    *(int*)((char*)self + 0x80) = 1;
    *(float*)((char*)self + 0x5C) = -1.0f;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x60) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181238);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern signed char D_00535C11[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_0019E538(void* self, int a1);
void* func_0039E4A0(void* self);

extern "C" void func_00181238(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    char* mgr = *(char**)((char*)D_004A28A8 + 0x7C);
    if (mgr != 0 && D_00535C11[0] != 0) {
        func_0019E538(func_001A0548(mgr + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
    }
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_001812A8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void func_00181BD0(void* self, int index, signed char charID);

extern "C" int func_001812A8(void* self, int on)
{
    if (on != 0) {
        int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
        func_00181BD0(self, *(signed char*)((char*)self + 0x44), id);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181308);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void func_001A0508(void* self, int idx, int a2, int a3);
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_00181EF0(void* self);
extern "C" void func_0039E4C0(void* self, int a1);
extern void* D_004A28A8;
extern signed char D_00440F68[];
extern char D_004A1398[];

extern "C" void func_00181308(void* self, int a1)
{
    int i = 0;
    int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    for (; i < 10; i++) {
        if (D_00440F68[i] == id) {
            void* menu = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1398));
            if (menu != 0)
                cUIMenu_setSelectedByIndex(menu, i);
            break;
        }
    }
    func_001A0508(*(char**)((char*)D_004A28A8 + 0x7C) + 0xB0, *(signed char*)((char*)self + 0x44), D_00440F68[0], 0);
    char* r = (char*)func_001A0548(*(char**)((char*)D_004A28A8 + 0x7C) + 0xB0, *(signed char*)((char*)self + 0x44));
    *(int*)(r + 0xCCC) = 0;
    func_00181EF0(self);
    func_0039E4C0(self, a1);
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181400__FPv);
#ifdef SKIP_ASM
void* func_00181400(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181420);
#ifdef SKIP_ASM
extern "C" void func_00181EF0(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_00181420(void* self)
{
    func_00181EF0(self);
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/festatecharselect", func_00181450);

INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onWidgetCreate);

INCLUDE_ASM("fe/festatecharselect", func_001817E8);

INCLUDE_ASM("fe/festatecharselect", func_00181BD0);

INCLUDE_ASM("fe/festatecharselect", func_00181EF0);

INCLUDE_ASM("fe/festatecharselect", func_00182220);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182420);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045D780[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void func_00182420(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045D780), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182690);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001474A8(void* iface, int rider, int id);
extern "C" void func_0039F190(void*, int);
extern char D_004C66A8[];
extern char D_004C66B8[];

struct sVEK182690a {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
struct sVEK182690b {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00182690(void* self, void* menu, unsigned int msg)
{
    if (menu == 0)
        return;
    switch (msg) {
    case 2: {
        sVEK182690a* vt = *(sVEK182690a**)((char*)menu + 8);
        vt[21].fn((char*)menu + vt[21].delta, D_004C66A8);
        break;
    }
    case 1: {
        int sel = *(int*)((char*)menu + 0x18) + *(unsigned char*)(*(char**)((char*)self + 0x6C) + 0x98);
        sVEK182690a* vt = *(sVEK182690a**)((char*)menu + 8);
        vt[21].fn((char*)menu + vt[21].delta, D_004C66B8);
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x50), *(const char**)((char*)self + (sel << 2) + 0x16C));
        break;
    }
    case 6:
        *(int*)((char*)self + 0x70) = 0;
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        break;
    case 5: {
        int sel = *(int*)((char*)menu + 0x18) + *(unsigned char*)(*(char**)((char*)self + 0x6C) + 0x98);
        if (sel == 0)
            *(int*)((char*)self + 0x70) = 0;
        else
            *(int*)((char*)self + 0x70) = *(int*)((char*)self + (sel << 2) + 0x7C);
        char* pl = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        func_001474A8(pl, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0x70));
        sVEK182690b* vt = *(sVEK182690b**)(pl + 0xC);
        vt[1].fn(pl + vt[1].delta);
        *(int*)((char*)self + 0x74) = 1;
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182808);
#ifdef SKIP_ASM
extern "C" void func_00182870(void* self);

extern "C" int func_00182808(void* self, int a1, unsigned int msg, int value)
{
    switch (msg) {
    case 7:
    case 8:
    case 9:
        return 0x100;
    case 1:
        return value < *(int*)((char*)self + 0x78);
    case 5:
        func_00182870(self);
        break;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182870);
#ifdef SKIP_ASM
extern "C" void func_00182870(void* self)
{
    int i;
    int idx = *(unsigned char*)(*(char**)((char*)self + 0x6c) + 0x98);
    for (i = 0; i < 6; i++) {
        void* it = ((void**)((char*)self + 0x54))[i];
        if (it != 0) {
            int v = ((int*)((char*)self + 0xf4))[idx + i];
            *(int*)((char*)it + 0x78) = -1;
            *(int*)((char*)it + 0x7c) = v;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_001828C0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern void* D_0046CE60[];

extern "C" void* func_001828C0(void* self, void* engine, signed char idx)
{
    func_0039E2A0(self, engine);
    *(int*)((char*)self + 0xC) = 0xC;
    *(void***)((char*)self + 0x8) = D_0046CE60;
    *(signed char*)((char*)self + 0x44) = idx;
    *(unsigned char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    *(int*)((char*)self + 0x4C) = 0;
    return self;
}
#endif


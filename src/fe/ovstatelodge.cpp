#include "common.h"

INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_cFEStateMountainRoom);

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D28F8);
#ifdef SKIP_ASM
struct sVEntry001D28F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern void* D_00469858[];
extern void* D_0046D0D0[];
extern void* D_004A289C;
extern void* D_004A28A8;
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" void func_0039E390(void* self, int flags);

extern "C" void func_001D28F8(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_00469858;
    int slot = *(int*)((char*)self + 0xBC);
    if (slot >= 0) {
        func_0019DC20(*(char**)((char*)D_004A28A8 + 0x7C) + 0x1A70, *(int*)((char*)self + 0xB0), slot);
        *(int*)((char*)self + 0xBC) = -1;
    }
    int id = *(int*)((char*)self + 0xC4);
    if (id >= 0) {
        char* g = (char*)D_004A289C;
        sVEntry001D28F8* vt = *(sVEntry001D28F8**)(g + 0x10D8);
        vt[50].fn(g + vt[50].delta, id);
    }
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);
extern char D_00467228[];
extern char D_004A1F08[];
extern char D_004A1F10[];
extern char D_004A1F18[];
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];

extern "C" void cFEStateMountainRoom_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00467228), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIText* a = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1F08));
    if (a != 0) {
        cUIText_setAsciiString(a, D_004A14F8);
    }
    cUIText* b = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1F10));
    if (b != 0) {
        cUIText_setAsciiString(b, D_004A1500);
    }
    cUIText* c = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1F18));
    if (c != 0) {
        cUIText_setAsciiString(c, D_004A1508);
    }
    func_0028F140(func_0028B180(), 7);
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3160);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" int func_0019DA10(void* self, int bank, int id, int a3, int a4, int a5);

extern "C" void func_001D3160(void* self)
{
    if (*(int*)((char*)self + 0xBC) < 0) {
        void* item = *(void**)((char*)self + 0xC0);
        if (item != 0) {
            *(int*)((char*)self + 0xBC) = func_0019DA10(*(char**)((char*)D_004A28A8 + 0x7C) + 0x1A70, *(int*)((char*)self + 0xB0), *(int*)((char*)item + 4), 0xA, 1, 0);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3780);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern void* D_00469788[];

struct sLodge_3780 {
    char pad_0x0[0x48];
    int slots[4];       // 0x48
    int grid[6][5];     // 0x58
    int fD0;            // 0xD0
    char pad_0xD4[0x8];
    int charID;         // 0xDC
    int fE0;            // 0xE0
};

extern "C" void* func_001D3780(void* self, int a1, int a2, int a3)
{
    signed char idx = a2;
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 0x2C;
    *(void***)((char*)self + 0x8) = D_00469788;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    sLodge_3780* s = (sLodge_3780*)self;
    s->charID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    s->fE0 = a3;
    for (int i = 3; i >= 0; i--) {
        s->slots[i] = 0;
    }
    for (int j = 0; j < 5; j++) {
        s->grid[0][j] = 0;
        s->grid[1][j] = 0;
        s->grid[2][j] = 0;
        s->grid[3][j] = 0;
        s->grid[4][j] = 0;
        s->grid[5][j] = 0;
    }
    s->fD0 = 0;
    return self;
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3C80);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00157BF0(void* self, int rider, int a, int b, int c);
extern "C" void cFEStatePeakRoom_updateHelpText(void* self, int id);
extern "C" void func_001D3F80(void* self, int id);
extern "C" void func_0039F400(void* list, void* item);

struct sVEntA_001D3C80 { short delta; short index; void* (*fn)(void*, void*, int); };
struct sVEntB_001D3C80 { short delta; short index; void (*fn)(void*, int); };
struct sItem_001D3C80 { int pad[2]; sVEntB_001D3C80* vt; };

extern "C" void func_001D3C80(void* self, void* item, unsigned int key)
{
    if (item == 0) {
        return;
    }
    switch (key) {
    case 5:
        if (func_00157BF0(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)((char*)self + 0x44),
                          *(int*)((char*)self + 0xDC), *(int*)((char*)self + 0xE0), *(int*)((char*)item + 0x18)) != 0) {
            void* obj = **(void***)((char*)self + 0x10);
            sVEntA_001D3C80* vt = *(sVEntA_001D3C80**)((char*)obj + 4);
            void* r = vt[4].fn((char*)obj + vt[4].delta, self, (*(int*)((char*)self + 0xE0) << 16) | *(int*)((char*)item + 0x18));
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
        break;
    case 6: {
        int id = (*(int*)((char*)self + 0xE0) << 16) | *(int*)((char*)item + 0x18);
        for (int i = 0; i < 4; i++) {
            sItem_001D3C80* o = ((sItem_001D3C80**)((char*)self + 0x48))[i];
            if (o != 0) {
                o->vt[9].fn((char*)o + o->vt[9].delta, 0);
            }
        }
        void* obj = **(void***)((char*)self + 0x10);
        sVEntA_001D3C80* vt = *(sVEntA_001D3C80**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, id);
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
        break;
    }
    case 1:
        cFEStatePeakRoom_updateHelpText(self, *(int*)((char*)item + 0x18));
        func_001D3F80(self, *(int*)((char*)item + 0x18));
        break;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D4268);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void func_001D4918(void* self);
extern void* D_004696B8[];

struct sVE_4268 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sTrophy_4268 {
    char pad_0x0[0x48];
    int f48;            // 0x48
    char pad_0x4C[0x4];
    int charID;         // 0x50
    int f54;            // 0x54
    int bank;           // 0x58
    int count;          // 0x5C
    int snd[5];         // 0x60
    int tex[5];         // 0x74
    char pad_0x88[0x14];
    int f9C;            // 0x9C
};

extern "C" void* func_001D4268(void* self, int a1, int a2, int a3, int a4)
{
    signed char idx = a2;
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 0x2D;
    *(void***)((char*)self + 0x8) = D_004696B8;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    sTrophy_4268* s = (sTrophy_4268*)self;
    s->f48 = 0;
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    s->charID = cBENewPlayerInterface_getPlayerCharID(pi, *(signed char*)((char*)self + 0x44));
    sVE_4268* vt = *(sVE_4268**)((char*)pi + 0xC);
    vt[2].fn((char*)pi + vt[2].delta);
    s->f54 = a3;
    s->bank = a4;
    s->count = 0;
    for (int i = 0; i < 5; i++) {
        s->snd[i] = -1;
        s->tex[i] = -1;
    }
    s->f9C = -1;
    func_001D4918(self);
    return self;
}
#endif


#include "common.h"

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_costVisible);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_0045D9B8[];
extern char D_0045D9C8[];
extern char D_0045D9D8[];
extern char D_0045D9E8[];

class cWidget_00184668 {
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
    virtual void setVisible(int a);
};

extern "C" void cFEStateUberTrick_costVisible(void* self, int on)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    cWidget_00184668* a = (cWidget_00184668*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_0045D9B8));
    if (a) {
        a->setVisible(on);
    }
    cWidget_00184668* b = (cWidget_00184668*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_0045D9D8));
    if (b) {
        b->setVisible(on);
    }
    cWidget_00184668* c = (cWidget_00184668*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_0045D9C8));
    if (c) {
        c->setVisible(on);
    }
    cWidget_00184668* d = (cWidget_00184668*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_0045D9E8));
    if (d) {
        d->setVisible(on);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", func_00184780);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern char D_004A1458[];
extern char D_004A1460[];

class cWidget_00184780 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a);
};

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" void func_00184780(void* self, int on)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    cWidget_00184780* w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004A1458));
    if (w) {
        w->v09(on);
    }
    w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004A1460));
    if (w) {
        w->v09(on);
    }
}
#endif

INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_trickVisible);

INCLUDE_ASM("fe/festatestore", func_001849B0);

//100%
INCLUDE_ASM("fe/festatestore", func_00184B70);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_001A0570(void* self, int a1, int a2);
void* func_0039E4A0(void* self);

extern "C" void func_00184B70(void* self)
{
    char* mgr = *(char**)((char*)D_004A28A8 + 0x7C);
    if (mgr != 0) {
        func_001A0570(mgr + 0xB0, *(signed char*)((char*)self + 0x44), 0);
    }
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", func_00184BB8);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_checkGate(void* self);

extern "C" int func_00184BB8(void* self, int on)
{
    if (on != 0) {
        cWScriptMan_checkGate(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", func_00184BE0);
#ifdef SKIP_ASM
struct sVEntry00184BE0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj00184BE0 {
    int pad[2];
    sVEntry00184BE0* vt;
};

extern "C" void func_00183B08(void* self);
extern "C" void func_0039E4C0(void* self, int a1);

extern "C" void func_00184BE0(void* self, int a1)
{
    sObj00184BE0* o;
    func_00183B08(self);
    o = *(sObj00184BE0**)((char*)self + 0x4C);
    if (o != 0) {
        o->vt[7].fn((char*)o + o->vt[7].delta, 1);
    }
    o = *(sObj00184BE0**)((char*)self + 0x50);
    if (o != 0) {
        o->vt[7].fn((char*)o + o->vt[7].delta, 0);
    }
    func_0039E4C0(self, a1);
}
#endif

INCLUDE_ASM("fe/festatestore", func_00184C60);

//100%
INCLUDE_ASM("fe/festatestore", func_00184F40);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" int func_00150B48(void* self, int a, int b, int amount);
extern "C" int func_0014FE08(void* self, int rider, int idx, int bit);
extern "C" void func_00184520(void* self);
extern "C" void cWScriptMan_checkGate(void* self);
extern char D_0045D930[];
extern char D_0045DA70[];

struct sVE_4F40 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_00184F40(void* self, void* item, int msg)
{
    switch (msg) {
    case 0x15:
        break;
    case 0x16:
        if (*(int*)((char*)item + 0x6C) != 0) {
            int charID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
            func_00150B48(cBE_getInterface_Fv(cBE_getBE(), 0xB), *(signed char*)((char*)self + 0x44), charID, *(int*)((char*)item + 0x48));
            int a = *(signed char*)(*(char**)(*(char**)((char*)self + 0x4C) + 0xA0) + 0x18);
            int b = *(signed char*)(*(char**)(*(char**)((char*)self + 0x50) + 0xA0) + 0x18);
            void* pi = cBE_getInterface_Fv(cBE_getBE(), 6);
            func_0014FE08(pi, *(signed char*)((char*)self + 0x44), a, b);
            sVE_4F40* vt = *(sVE_4F40**)((char*)pi + 0xC);
            vt[1].fn((char*)pi + vt[1].delta);
            void* t = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045D930));
            if (t != 0) {
                cUIText_setUnicodeStringByID((cUIText*)t, GetHashValue32(D_0045DA70));
            }
            func_00184520(self);
        }
        cWScriptMan_checkGate(self);
        break;
    }
}
#endif

INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onWidgetCreate);

INCLUDE_ASM("fe/festatestore", func_00185268);

//100%
INCLUDE_ASM("fe/festatestore", func_001859D8);
#ifdef SKIP_ASM
extern "C" int func_001859D8(void* self, int a1, unsigned int a2)
{
    if (a1 == *(int*)((char*)self + 0x50)) {
        switch (a2) {
        case 9:
            return 0x100;
        }
    } else {
        switch (a2) {
        case 8:
        case 9:
            return 0x100;
        }
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", func_00185A18);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046BDF8[];

extern "C" void* func_00185A18(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046BDF8;
    *(int*)((char*)self + 0x60) = -100;
    *(int*)((char*)self + 0x64) = 1;
    *(int*)((char*)self + 0xC) = 0x24;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

extern void* D_0046D0D0[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/festatestore", func_00185A70__FPv);
#ifdef SKIP_ASM
void* func_00185A70(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif


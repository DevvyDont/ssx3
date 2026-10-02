#include "common.h"

INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onCreateScreen);

INCLUDE_ASM("fe/ovstatepause", func_001FA100);

INCLUDE_ASM("fe/ovstatepause", func_001FA238);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FA9E0);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* func_0020E900(void* self);
extern "C" void func_0039F190(void* self, int a1);
extern void* D_004A2EEC;
extern char D_0046E050[];

extern "C" void func_001FA9E0(void* self)
{
    func_0020E900(self);
    char* p = (char*)D_004A2EEC;
    if (p != 0) {
        int ok = *(int*)(p + 0x68) == 0 || *(int*)(p + 0x64) == 0;
        if (ok) {
            int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_0046E050));
            if (frame != 0xFFFF) {
                cUIScreen_playFrame(*(void**)((char*)self + 0x40), (unsigned short)frame, 1);
            }
            func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        }
    }
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FAA78);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAAF0);
#ifdef SKIP_ASM
extern "C" int func_001FAAF0(void* self, int a1, int a2)
{
    return a2 != 4 ? 0x101 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAB08);
#ifdef SKIP_ASM
extern "C" int func_001FAB08(void* self, int a1, int a2)
{
    return a2 != 0 ? 0x101 : 0x100;
}
#endif

INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAF08__FPv);
#ifdef SKIP_ASM
int func_001FAF08(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAF10);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* func_0020E900(void* self);
extern "C" void func_001FAFF8(void* self, bool on);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut(void* self);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingReceived(void* self);
extern void* D_004A2EEC;
extern char D_0046FB18[];

class cUIObj_1FAF10 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int v);
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

extern "C" void func_001FAF10(void* self)
{
    func_0020E900(self);
    char* p = (char*)D_004A2EEC;
    if (p != 0 && *(int*)((char*)self + 0x9C) != 0) {
        int timedOut = *(int*)(p + 0x138) <= 0 && *(int*)(p + 0x13C) == 0;
        if (timedOut) {
            *(int*)(p + 0x9C) = 0;
            cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut(self);
        } else {
            if (*(int*)(p + 0x13C) == 0) {
                return;
            }
            int received = *(int*)(p + 0x80) != 0 && *(int*)(p + 0xA0) == 0 && *(int*)(p + 0x24) == 0;
            if (received) {
                cOVState_PAUSE_ONLINE_ERROR_displayPingReceived(self);
            } else {
                ((cUIObj_1FAF10*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046FB18)))->setVisible(0);
            }
        }
        func_001FAFF8(self, 1);
        *(int*)((char*)self + 0x9C) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAFF8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_0046FA90[];
extern char D_004A2470[];
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int index);

class cUIObj_1FAFF8 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int v);
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

extern "C" void func_001FAFF8(void* self, bool on)
{
    cUIObj_1FAFF8* o = (cUIObj_1FAFF8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046FA90));
    o->setVisible(on);
    o->setEnabled(!on);
    if (on) {
        cUIObj_1FAFF8* m = (cUIObj_1FAFF8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2470));
        m->v07(1);
        cUIMenu_setSelectedByIndex(m, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FB0C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001FB2B8(void* self, void* item, int msg);
extern "C" void func_001FB458(void* self, int on);
extern "C" void func_001FB588(void* self, int on);
extern char D_0046FA90[];
extern char D_0046FB48[];

extern "C" void func_001FB0C0(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5:
        func_001FB2B8(self, item, msg);
        break;
    case 1: {
        int id = *(int*)((char*)item + 0x38);
        if (id == GetHashValue32(D_0046FA90)) {
            func_001FB588(self, 1);
            func_001FB458(self, 0);
        } else {
            int id2 = *(int*)((char*)item + 0x38);
            if (id2 == GetHashValue32(D_0046FB48)) {
                func_001FB458(self, 1);
                func_001FB588(self, 0);
            }
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut);
#ifdef SKIP_ASM
struct cUIText;
struct sVE_FB180 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_0046FB18[];
extern char D_0046FB60[];

extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut(void* self)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046FB18));
        if (t != 0 && *(int*)((char*)self + 0xA0) != 0) {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0046FB60));
        }
        sVE_FB180* vt = *(sVE_FB180**)((char*)t + 0x8);
        vt[9].fn((char*)t + vt[9].delta, *(int*)((char*)self + 0xA0));
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_displayPingReceived);
#ifdef SKIP_ASM
struct cUIText;
struct sVE_FB218 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(void* self, int visible);
extern char D_0046FB18[];
extern char D_0046FB88[];

extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingReceived(void* self)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046FB18));
        if (t != 0 && *(int*)((char*)self + 0xA0) != 0) {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0046FB88));
        }
        sVE_FB218* vt = *(sVE_FB218**)((char*)t + 0x8);
        vt[9].fn((char*)t + vt[9].delta, *(int*)((char*)self + 0xA0));
        cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(self, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FB2B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGame_exit(void* game, int a);
extern "C" void func_0012B340(void* p);
// func_0039F190 returns its last list node in $v0; the unit declares it void.
void* func_0039F190_r(void* self, int a1) __asm__("func_0039F190");
extern void* D_004A28A8;
extern int D_004A2A50;
extern int D_004A2A54;
extern int D_005366E8[];
extern int D_004428F0[];

extern "C" void func_001FB2B8(void* self, void* item, int msg)
{
    func_0039F190_r(*(char**)((char*)self + 0x10) + 0x18, 1);
    *(int*)((char*)self + 0x1C) = (*(int*)((char*)self + 0x1C) & ~0x3F00) | 0x780;
    switch (*(int*)((char*)item + 0x18)) {
    case 4:
        func_0012B340(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC));
        D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
        break;
    case 0:
    default:
        cGame_exit(*(void**)((char*)D_004A28A8 + 0x84), 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible);
#ifdef SKIP_ASM
extern char D_0046FB48[];
extern char D_0046FBB0[];
extern char D_004A2470[];
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int index);

class cUIObj_1FB378 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(void* self, int visible)
{
    cUIObj_1FB378* t = (cUIObj_1FB378*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046FB48));
    if (t != 0) {
        cUIText_setUnicodeStringByID((cUIText*)t, GetHashValue32(D_0046FBB0));
        t->setEnabled(visible ^ 1);
        t->setVisible(visible);
        *(int*)((char*)t + 0x18) = 4;
    }
    t = (cUIObj_1FB378*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2470));
    if (t != 0 && visible) {
        cUIMenu_setSelectedByIndex(t, 0);
    }
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FB458);

INCLUDE_ASM("fe/ovstatepause", func_001FB588);

INCLUDE_ASM("fe/ovstatepause", func_001FB6B8);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FBBD8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00146E98(void* iface, int idx);
extern "C" void func_0020A6F0(void* self, char* text, char* label);
extern "C" void func_0039E4C0(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004A2170[];
extern char D_0046E4E8[];
extern signed char D_00535C11[];
extern void* D_004A2EEC;

extern "C" void func_001FBBD8(void* self)
{
    char buf[32];
    func_0039E4C0(self);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    sprintf(buf, D_004A2170, func_00146E98(iface, 0) + 1);
    func_0020A6F0(self, buf, D_0046E4E8);
    if (D_00535C11[0] == 2) {
        sprintf(buf, D_004A2170, func_00146E98(iface, 1) + 1);
        func_0020A6F0(self, buf, D_0046E4E8);
    }
    char* p = (char*)D_004A2EEC;
    if (p != 0) {
        *(int*)(p + 0x64) = 0x4B0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FBCC8);
#ifdef SKIP_ASM
extern void* D_004A2EEC;

extern "C" void func_001FBCC8(void)
{
    char* p = (char*)D_004A2EEC;
    if (p != 0) {
        *(int*)(p + 0x64) = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FBCE0);
#ifdef SKIP_ASM
extern void* D_004A2EEC;
extern int D_004A26FC;
extern "C" void* func_0020E900(void* self);

extern "C" void func_001FBCE0(void* self)
{
    func_0020E900(self);
    char* p = (char*)D_004A2EEC;
    if (p != 0 && *(int*)(p + 0x64) == 0) {
        *(int*)(p + 0x64) = -1;
        D_004A26FC = 1;
    }
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FBD20);

INCLUDE_ASM("fe/ovstatepause", func_001FC768);

INCLUDE_ASM("fe/ovstatepause", func_001FC878);

INCLUDE_ASM("fe/ovstatepause", func_001FCEC0);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD0D8);
#ifdef SKIP_ASM
extern void* D_004A2EEC;
extern "C" void* func_0028B180();
extern "C" void func_00294F78(void*, int);
extern "C" void func_0039E4C0(void* self);

extern "C" void func_001FD0D8(void* self)
{
    func_0039E4C0(self);
    char* p = (char*)D_004A2EEC;
    if (p != 0) {
        *(int*)(p + 0x64) = 0x4B0;
    }
    func_00294F78(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD118);
#ifdef SKIP_ASM
extern void* D_004A2EEC;
extern "C" void* func_0028B180();
extern "C" void func_002872A8(void* self);

extern "C" void func_001FD118(void)
{
    char* p = (char*)D_004A2EEC;
    if (p != 0) {
        *(int*)(p + 0x64) = -1;
    }
    func_002872A8(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD150);
#ifdef SKIP_ASM
extern void* D_004A2EEC;
extern int D_004A26FC;
extern "C" void* func_0020E900(void* self);

extern "C" void func_001FD150(void* self)
{
    func_0020E900(self);
    char* p = (char*)D_004A2EEC;
    if (p != 0 && *(int*)(p + 0x64) == 0) {
        *(int*)(p + 0x64) = -1;
        D_004A26FC = 1;
    }
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FD190);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD268);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void func_0020A380(void* self);
extern "C" void* func_0028B180();
extern "C" void func_00294F78(void*, int);
extern char D_0046FF58[];
extern char D_0046E378[];

extern "C" void func_001FD268(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046FF58), 0);
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
    func_00294F78(func_0028B180(), 0xE);
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FD320);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDB78);
#ifdef SKIP_ASM
extern "C" int func_001FDB78(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FDB88);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDBF0);
#ifdef SKIP_ASM
extern "C" void* func_001D5330(void* self, int a1, int a2, int a3);
extern void* D_00472440[];

extern "C" void* func_001FDBF0(void* self, int a1, int a2)
{
    func_001D5330(self, a1, 1, a2);
    *(void***)((char*)self + 0x8) = D_00472440;
    *(int*)((char*)self + 0x22C) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FDC30);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDE60);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern int D_004A2A54;
extern int D_004A2A50;
extern int D_005366E8[];
extern int D_004428F0[];
void func_00270ED8(void* self);
extern "C" void* func_001D58B8(void* self);
extern "C" void func_0026F8A0(void* self, void* a1, int a2, int a3);

extern "C" void func_001FDE60(void* self)
{
    if (*(int*)((char*)self + 0x1C8) == 1) {
        func_00270ED8(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28));
        *(int*)((char*)self + 0x1C8) = 0;
    }
    D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
    func_001D58B8(self);
    func_0026F8A0(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28), 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDF00);
#ifdef SKIP_ASM
extern "C" void func_0039F718(void* self);

extern "C" void func_001FDF00(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FDF40);

INCLUDE_ASM("fe/ovstatepause", func_001FE4A8);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE5B0);
#ifdef SKIP_ASM
class cUIObj_1FE5B0 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_00470188[];
extern char D_004A20A0[];

extern "C" void func_001FE5B0(void* self, bool on)
{
    ((cUIObj_1FE5B0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00470188)))->setVisible(!on);
    ((cUIObj_1FE5B0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A20A0)))->setVisible(!on);
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE648);
#ifdef SKIP_ASM
class cUIObj_1FE648 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_004A2090[];
extern char D_004A2568[];

extern "C" void func_001FE648(void* self, int a1, bool on)
{
    cUIObj_1FE648* a = (cUIObj_1FE648*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2090));
    ((cUIObj_1FE648*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2568)))->setVisible(!on);
    *(int*)((char*)self + 0x224) = !on;
    a->setVisible(!on);
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FE6E8);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE8F0);
#ifdef SKIP_ASM
extern void* D_004A28A8;
void func_00270ED8(void* self);
extern "C" void func_001D8700(void* self, void* a1, int key);

extern "C" void func_001FE8F0(void* self, void* a1, int key)
{
    if (key == 0x16) {
        if (*(int*)((char*)self + 0x19C) == 5) {
            func_00270ED8(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28));
            *(int*)((char*)self + 0x1C8) = 0;
        }
    }
    func_001D8700(self, a1, key);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE968);
#ifdef SKIP_ASM
extern "C" void* func_001D5330(void* self, int a1, int a2, int a3);
extern void* D_004722B8[];

extern "C" void* func_001FE968(void* self, int a1, int a2)
{
    func_001D5330(self, a1, 2, a2);
    *(void***)((char*)self + 0x8) = D_004722B8;
    *(int*)((char*)self + 0x22C) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FE9A8);

extern "C" void* func_001D58B8(void* self);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FEC40__FPv);
#ifdef SKIP_ASM
void* func_001FEC40(void* self)
{
    return func_001D58B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FEC60);
#ifdef SKIP_ASM
extern "C" void func_0039F718(void* self);

extern "C" void func_001FEC60(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FECA0);

INCLUDE_ASM("fe/ovstatepause", func_001FF170);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF278);
#ifdef SKIP_ASM
class cUIObj_1FF278 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_00470188[];
extern char D_004A20A0[];

extern "C" void func_001FF278(void* self, bool on)
{
    ((cUIObj_1FF278*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00470188)))->setVisible(!on);
    ((cUIObj_1FF278*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A20A0)))->setVisible(!on);
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF310);
#ifdef SKIP_ASM
class cUIObj_1FF310 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_004A2090[];
extern char D_004A2568[];

extern "C" void func_001FF310(void* self, int a1, bool on)
{
    cUIObj_1FF310* a = (cUIObj_1FF310*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2090));
    ((cUIObj_1FF310*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2568)))->setVisible(!on);
    *(int*)((char*)self + 0x224) = !on;
    a->setVisible(!on);
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FF3B0);

INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF6D0);
#ifdef SKIP_ASM
extern "C" void func_0039E4C0(void* self);
extern "C" void func_001FFD08(void* self, int a1);

extern "C" void func_001FF6D0(void* self)
{
    func_0039E4C0(self);
    func_001FFD08(self, 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF700);
#ifdef SKIP_ASM
extern int D_004A26FC;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00158E30(void* iface);

extern "C" void func_001FF700(void* self, void* a1, int key)
{
    if (a1 != 0) {
        if (key == 5) {
            D_004A26FC = 1;
            func_00158E30(cBE_getInterface_Fv(cBE_getBE(), 0xD));
        }
    }
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FF748);

INCLUDE_ASM("fe/ovstatepause", func_001FF7B8);

INCLUDE_ASM("fe/ovstatepause", func_001FFD08);

INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onGainTransition);


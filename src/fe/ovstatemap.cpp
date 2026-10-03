#include "common.h"

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct sSelf0020A380;
extern "C" void func_0020A380(sSelf0020A380* self);
extern "C" int func_001A3BF0(void* mgr);
extern "C" void func_00208F10(void* self);
extern "C" void cOVState_MAP_setupPopup(void* self);
extern "C" void cOVState_MAP_setupPlayerIndicator(void* self);
extern "C" void cOVState_MAP_setupLocalSessionList(void* self);
extern void* D_004A28A8;
extern char D_004717F8[];
extern char D_004A25A8[];

class cUIObj_86A8 {
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

struct sMap_86A8 {
    char pad_0x0[0x10];
    void* engine;       // 0x10
    char pad_0x14[0xA8];
    int fBC;            // 0xBC
    int fC0;            // 0xC0
    int fC4;            // 0xC4
    char pad_0xC8[0x8];
    cUIObj_86A8* obj;   // 0xD0
    void* screen;       // 0xD4
    char pad_0xD8[0x4];
    int fDC;            // 0xDC
    int fE0;            // 0xE0
    int a[8];           // 0xE4
    int b[8];           // 0x104
    char pad_0x124[0x60];
    int c[8];           // 0x184
    int d[8];           // 0x1A4
    char pad_0x1C4[0x8];
    float scale;        // 0x1CC
    float speed;        // 0x1D0
};

extern "C" void cOVState_MAP_onCreateScreen(sMap_86A8* self)
{
    void* engine = self->engine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004717F8), 0);
    self->screen = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0020A380((sSelf0020A380*)self);
    self->fC4 = func_001A3BF0(*(void**)((char*)D_004A28A8 + 0x11C));
    self->fDC = 0;
    self->fBC = 0;
    self->scale = 1.0f;
    self->speed = 0.1599999964237213f;
    cUIObj_86A8* o = (cUIObj_86A8*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A25A8));
    self->obj = o;
    if (o != 0) {
        o->setVisible(0);
    }
    self->fC0 = 0;
    self->fE0 = 0;
    for (int i = 0; i < 8; i++) {
        self->a[i] = 0;
        self->b[i] = 0;
        self->c[i] = 0;
        self->d[i] = 0;
    }
    func_00208F10(self);
    cOVState_MAP_setupPopup(self);
    cOVState_MAP_setupPlayerIndicator(self);
    cOVState_MAP_setupLocalSessionList(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_002087E8__FPv);
#ifdef SKIP_ASM
void func_002087E8(void* self)
{
}
#endif

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onGainTransition);

//100%
INCLUDE_ASM("fe/ovstatemap", func_00208B00);
#ifdef SKIP_ASM
struct sVEntry00208B00 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj00208B00 {
    int pad[2];
    sVEntry00208B00* vt;
};

int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_0039E4C0(void* self, int a1);
extern char D_00470A20[];

extern "C" void func_00208B00(void* self, int a1)
{
    sObj00208B00* o;
    func_0039E4C0(self, a1);
    o = (sObj00208B00*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0xD4), GetHashValue32(D_00470A20));
    if (o != 0) {
        o->vt[7].fn((char*)o + o->vt[7].delta, 0);
        o->vt[9].fn((char*)o + o->vt[9].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_00208B78);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(void* mgr, int file);
extern "C" void func_0020A088(void* self);
extern "C" void func_0020A430(void* self);

extern "C" void func_00208B78(void* self)
{
    cFEAsyncManager_UnloadFEAsyncFile(*(void**)((char*)D_004A28A8 + 0x11C), *(int*)((char*)self + 0xC4));
    func_0020A088(self);
    func_0020A430(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_00208BB8);
#ifdef SKIP_ASM
struct sVEntry00208BB8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj00208BB8 {
    int pad[2];
    sVEntry00208BB8* vt;
};

extern "C" void func_00208BB8(void* self)
{
    sObj00208BB8* o;
    *(int*)((char*)self + 0xDC) = 1;
    *(int*)((char*)self + 0xC8) = 0;
    o = *(sObj00208BB8**)((char*)self + 0xD0);
    if (o != 0) {
        o->vt[9].fn((char*)o + o->vt[9].delta, 0);
    }
    o = *(sObj00208BB8**)((char*)self + 0xE0);
    if (o != 0) {
        o->vt[9].fn((char*)o + o->vt[9].delta, 0);
    }
}
#endif

INCLUDE_ASM("fe/ovstatemap", func_00208C28);

//100%
INCLUDE_ASM("fe/ovstatemap", func_00208EF8);
#ifdef SKIP_ASM
extern "C" int func_00208EF8(void* self, int a1, int a2)
{
    return ((unsigned int)(a2 - 8) < 2) ? 0 : 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_00208F10);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(void* mgr, int file);
extern "C" int* func_00144BC0(void*);
extern "C" void func_001A37F8(void* mgr, int a1, int file, int a3);

extern "C" void func_00208F10(void* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* mgr = *(void**)((char*)D_004A28A8 + 0x11C);
    cFEAsyncManager_UnloadFEAsyncFile(mgr, *(int*)((char*)self + 0xC4));
    func_001A37F8(mgr, *func_00144BC0(iface), *(int*)((char*)self + 0xC4), 1);
    *(int*)((char*)self + 0xC8) = 0;
}
#endif

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupPopup);

//100%
INCLUDE_ASM("fe/ovstatemap", func_00209300);
#ifdef SKIP_ASM
struct sVEntry00209300 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sObj00209300 {
    int pad[2];
    sVEntry00209300* vt;
};

extern "C" void func_00209E78(void* self);

extern "C" void func_00209300(void* self, sObj00209300* o)
{
    if (o->vt[17].fn((char*)o + o->vt[17].delta) != 0 || o->vt[18].fn((char*)o + o->vt[18].delta) != 0) {
        func_00209E78(self);
    }
}
#endif

INCLUDE_ASM("fe/ovstatemap", func_00209370);

//100%
INCLUDE_ASM("fe/ovstatemap", func_002095E8);
#ifdef SKIP_ASM
struct sVEntry_func_002095E8 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char D_004C8B38[];
extern char D_004C8B28[];

extern "C" void func_002095E8(void* self, void* obj, int on)
{
    if (on != 0) {
        sVEntry_func_002095E8* vt = *(sVEntry_func_002095E8**)((char*)obj + 0x8);
        vt[11].fn((char*)obj + vt[11].delta, D_004C8B38);
    } else {
        sVEntry_func_002095E8* vt = *(sVEntry_func_002095E8**)((char*)obj + 0x8);
        vt[11].fn((char*)obj + vt[11].delta, D_004C8B28);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_00209648);
#ifdef SKIP_ASM
struct sVEntry_func_00209648 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char D_004C8B58[];
extern char D_004C8B48[];

extern "C" void func_00209648(void* self, void* obj, int on)
{
    if (on != 0) {
        sVEntry_func_00209648* vt = *(sVEntry_func_00209648**)((char*)obj + 0x8);
        vt[11].fn((char*)obj + vt[11].delta, D_004C8B58);
    } else {
        sVEntry_func_00209648* vt = *(sVEntry_func_00209648**)((char*)obj + 0x8);
        vt[11].fn((char*)obj + vt[11].delta, D_004C8B48);
    }
}
#endif

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupPlayerIndicator);

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupLocalSessionList);

//100%
INCLUDE_ASM("fe/ovstatemap", func_00209E78);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_00471850[];

class cUIObj_9E78 {
public:
    int pad[2];
    virtual ~cUIObj_9E78();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};
struct sMap_9E78 {
    char pad_0x0[0xC0];
    int count;              // 0xC0
    char pad_0xC4[0x10];
    void* screen;           // 0xD4
    char pad_0xD8[0x8];
    cUIObj_9E78* cursor;    // 0xE0
    cUIObj_9E78* a[8];      // 0xE4
    cUIObj_9E78* b[8];      // 0x104
};

extern "C" void func_00209E78(void* p)
{
    sMap_9E78* self = (sMap_9E78*)p;
    if (self->cursor != 0) {
        self->cursor->setVisible(1);
    }
    void* list = *(void**)((char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00471850)) + 0xA0);
    int sel = 1;
    if (list != 0) {
        sel = *(int*)((char*)list + 0x18);
    }
    for (int i = 0; i < self->count; i++) {
        int cur = sel - 1;
        if (i == cur) {
            if (self->a[i] != 0) {
                self->a[i]->setVisible(0);
            }
            if (self->b[i] != 0) {
                self->b[i]->setVisible(1);
            }
        } else {
            if (self->a[i] != 0) {
                self->a[i]->setVisible(1);
            }
            if (self->b[i] != 0) {
                self->b[i]->setVisible(0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_0020A088);
#ifdef SKIP_ASM
class cUIObj_A088 {
public:
    int pad[2];
    virtual ~cUIObj_A088();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

struct sMap_A088 {
    char pad_0x0[0xE0];
    cUIObj_A088* cursor;    // 0xE0
    cUIObj_A088* a[8];      // 0xE4
    cUIObj_A088* b[8];      // 0x104
};

extern "C" void func_0020A088(void* p)
{
    sMap_A088* self = (sMap_A088*)p;
    if (self->cursor != 0) {
        self->cursor->setVisible(0);
        delete self->cursor;
        self->cursor = 0;
    }
    for (int i = 0; i < 8; i++) {
        if (self->a[i] != 0) {
            self->a[i]->setVisible(0);
            delete self->a[i];
            self->a[i] = 0;
        }
        if (self->b[i] != 0) {
            self->b[i]->setVisible(0);
            delete self->b[i];
            self->b[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_0020A1A8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: SN soft-float float->double conversion (the source passed floats to sprintf).
extern "C" double func_00413AF8(float f);
extern char D_004C6848[];
extern char D_004719F0[];
extern char D_00471A00[];

struct sGameSettings_A1A8 {
    unsigned int flags;     // 0x0, bits 22..24 = language
    int pad[0x288 / 4 - 1];
};
extern sGameSettings_A1A8 D_00535610_A1A8 __asm__("D_00535610");

// Format a time in seconds as minutes/seconds for the current language.
extern "C" char* func_0020A1A8(int secs)
{
    cBE_getInterface_Fv(cBE_getBE(), 4);
    int m = secs / 60;
    int s = secs % 60;
    sGameSettings_A1A8 set = D_00535610_A1A8;
    switch ((set.flags >> 22) & 7) {
    case 0:
    case 2:
        sprintf(D_004C6848, D_004719F0, func_00413AF8((float)m), func_00413AF8((float)s));
        break;
    case 1:
    case 3:
        sprintf(D_004C6848, D_00471A00, func_00413AF8((float)m), func_00413AF8((float)s));
        break;
    }
    return D_004C6848;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_0020A380);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A20D0[];
extern void* D_004A28A8;
extern "C" void func_002EA820(void* self);
extern "C" void* func_0039F9D8(void* list, int hash);
struct sVEntry0020A380 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

struct sSelf0020A380 {
    char pad[0x10];
    char* mgr;
    char pad14[0x3C];
    int pdaOn;
};

extern "C" void func_0020A380(sSelf0020A380* self)
{
    self->pdaOn = 0;
    char* game = *(char**)((char*)D_004A28A8 + 0x84);
    if (game != 0) {
        char* p = *(char**)(game + 0x64);
        int mode = *(int*)(p + 0x10);
        if (mode != 0) {
            func_002EA820(p);
            if (mode == 1 || mode == 3) {
                self->pdaOn = 1;
            }
        }
    }
    char* obj = (char*)func_0039F9D8(self->mgr + 0x18, GetHashValue32(D_004A20D0));
    if (obj != 0) {
        sVEntry0020A380* e = &(*(sVEntry0020A380**)(obj + 8))[24];
        e->fn(obj + e->delta, 3, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_0020A430);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A20D0[];
extern void* D_004A28A8;
extern "C" void func_002EA860(void* self);
extern "C" void* func_0039F9D8(void* list, int hash);
extern "C" void func_0020D190();
void* func_0039E4A0(void* self);
struct sVEntry0020A430 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" void func_0020A430(void* self)
{
    if (*(int*)((char*)self + 0x50) != 0) {
        char* game = *(char**)((char*)D_004A28A8 + 0x84);
        if (game != 0) {
            func_002EA860(*(void**)(game + 0x64));
            *(int*)((char*)self + 0x50) = 0;
        }
    }
    char* obj = (char*)func_0039F9D8(*(char**)((char*)self + 0x10) + 0x18, GetHashValue32(D_004A20D0));
    if (obj != 0) {
        sVEntry0020A430* e = &(*(sVEntry0020A430**)(obj + 8))[24];
        e->fn(obj + e->delta, 4, 0);
    }
    if (*(int*)((char*)self + 0x54) != 0) {
        func_0020D190();
    }
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_0020A4E0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void* cBXString_Concat(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void func_003A19F8(void* text, char* str);
extern "C" void func_001A83D8(void* list, int keep, unsigned int sel, int a3, int pos, int a5, int a6, int trim);
extern "C" int func_00258160(int a, char* name, int len);
extern void* D_004A28A8;
extern int* D_004A2EEC_A4E0 __asm__("D_004A2EEC");
extern char D_00534B30[];
extern char D_0046E660[];
extern char D_0046E7F8[];
extern char D_0046E808[];
extern char D_004A2710[];

struct sBXString_A4E0 {
    char* str;
    sBXString_A4E0() {}
    sBXString_A4E0(const sBXString_A4E0& o);
};

struct sVE_A4E0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void show_A4E0(char* o, int on)
{
    sVE_A4E0* vt = *(sVE_A4E0**)(o + 8);
    vt[9].fn(o + vt[9].delta, on);
}

// PORT: the session pointer is passed through func_00258160's int parameter.
extern "C" int func_0020A4E0(char* self)
{
    int ret = 0;
    cBE_getInterface_Fv(*(void**)((char*)D_004A28A8 + 0x78), 7);
    char* net = D_00534B30;
    if (*(int*)net != 0 && *(void**)(self + 0x40) != 0) {
        int* sess = D_004A2EEC_A4E0;
        if (sess != 0 && *sess != 0) {
            char* name = self + 0x58;
            if (func_00258160((int)sess, name, 0x41) != 0) {
                char* a = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(D_0046E660));
                if (a != 0)
                    show_A4E0(a, 0);
                char* b = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(D_0046E7F8));
                if (b != 0)
                    show_A4E0(b, 0);
                ret = 1;
                char* obj = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(D_0046E808));
                unsigned short c8 = *(unsigned short*)(obj + 0xC8);
                unsigned short c4 = *(unsigned short*)(obj + 0xC4);
                unsigned short c6 = *(unsigned short*)(obj + 0xC6);
                int none = c8 == 0;
                sBXString_A4E0 s;
                cBXString_cBXString2(&s, net + 0x498);
                cBXString_Concat(&s, D_004A2710);
                cBXString_Concat(&s, name);
                func_003A19F8(obj, s.str);
                func_001A83D8(obj, 0x28, 4, c8, c4, c6, none, 1);
                show_A4E0(obj, 1);
                cBXString__cBXString(&s, 2);
            }
        }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_0020A6A8);
#ifdef SKIP_ASM
struct sVEntry0020A6A8 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern void* D_004A28A8;

extern "C" void func_0020A6A8(void* self, int a1, int a2)
{
    char* w = *(char**)((char*)D_004A28A8 + 0x84);
    if (w != 0) {
        char* o = *(char**)(w + 0x200);
        if (o != 0) {
            sVEntry0020A6A8* vt = *(sVEntry0020A6A8**)(o + 0xC);
            vt[9].fn(o + vt[9].delta, a1, a2);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_0020A6F0);
#ifdef SKIP_ASM
extern "C" void* cUIAnimationBank_getAnimationByHashName(void* self, int hash);
extern "C" void func_0039FCC8(void* self, void* anim, int mode, int v, int a4);

extern "C" void func_0020A6F0(void* self, char* objName, char* animName)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(objName));
    if (obj != 0) {
        void* bank = (char*)*(void**)((char*)self + 0x10) + 0x50;
        void* anim = cUIAnimationBank_getAnimationByHashName(bank, GetHashValue32(animName));
        if (anim != 0) {
            func_0039FCC8(obj, anim, 3, 0, 0);
        }
    }
}
#endif


#include "common.h"

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptions_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045DD30[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFEStateOptions_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045DD30), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 1;
}
#endif

INCLUDE_ASM("fe/festateoptions", func_00188870);

INCLUDE_ASM("fe/festateoptions", func_00188980);

//100%
INCLUDE_ASM("fe/festateoptions", func_00188C58);
#ifdef SKIP_ASM
extern int D_004A3484;

extern "C" int func_00188C58(void* self, void* a1)
{
    D_004A3484 = *(unsigned char*)((char*)a1 + 4);
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_00188C68);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_00397870(void* list, int index);
extern char D_004A1530[];

extern "C" int func_00188C68(void* self, void* menu, unsigned int key, int index)
{
    if (key < 10) {
        if (key >= 8) {
            return 0x100;
        }
    }
    if (key == 6) {
        int h = *(int*)((char*)func_00397870((char*)menu + 0x74, index) + 0x38);
        if (h == GetHashValue32(D_004A1530)) {
            goto check;
        }
    }
    if (key == 7) {
    check:
        int r = 0x101;
        if (*(int*)((char*)self + 0x4C) == 0) {
            r = 1;
        }
        return r;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/festateoptions", func_00188D08);

//100%
INCLUDE_ASM("fe/festateoptions", func_00188E58);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_0039E318(void* self, void* engine, void* owner);
extern "C" void func_0039F290(void* list, void* item);
extern char D_0045DD68[];
extern void* D_0046B3D0[];
extern int D_004A14D0;
extern int D_004A14D4;
extern int D_004A14D8;
extern int D_004A14DC;
extern int D_004A14E0;

extern "C" void func_00188E58(void* self)
{
    char* p = (char*)operator new(0x4C, D_0045DD68, 0x100, 0);
    func_0039E318(p, *(void**)((char*)self + 0x10), self);
    *(void***)(p + 8) = D_0046B3D0;
    *(char*)(p + 0x48) = 0;
    func_0039F290(*(char**)((char*)self + 0x10) + 0x18, p);
    D_004A14D8 = 0;
    D_004A14E0 = 1;
    D_004A14DC = 0;
    D_004A14D4 = 0;
    D_004A14D0 = 0;
    *(int*)((char*)self + 0x4C) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_00188EE8);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046C6F0[];

extern "C" void* func_00188EE8(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046C6F0;
    *(int*)((char*)self + 0xC) = 0x1C;
    return self;
}
#endif

INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_onCreateScreen);

extern "C" void* func_0039E4C0(void* self);

//100%
INCLUDE_ASM("fe/festateoptions", func_001895F0__FPv);
#ifdef SKIP_ASM
void* func_001895F0(void* self)
{
    return func_0039E4C0(self);
}
#endif

INCLUDE_ASM("fe/festateoptions", func_00189610);

//100%
INCLUDE_ASM("fe/festateoptions", func_00189958);
#ifdef SKIP_ASM
extern "C" int func_00189958(void* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_00189970);
#ifdef SKIP_ASM
extern "C" int func_00189970(void* self, int a1, int a2)
{
    return a2 ? 0x100 : 0;
}
#endif

INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_onWidgetEvent);

INCLUDE_ASM("fe/festateoptions", func_00189D60);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018A258);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046C620[];
extern int D_004A14D4;

extern "C" void* func_0018A258(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 8) = D_0046C620;
    *(int*)((char*)self + 0xC) = 0x1D;
    D_004A14D4 = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_onCreateScreen);

INCLUDE_ASM("fe/festateoptions", func_0018A3D8);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018A870__FPv);
#ifdef SKIP_ASM
void* func_0018A870(void* self)
{
    return func_0039E4C0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018A890);
#ifdef SKIP_ASM
extern "C" int func_0018A890(void* self, int a1, int a2)
{
    switch (a2) {
    case 9:
        return 0x100;
    case 6:
        return 0;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018A8C0);
#ifdef SKIP_ASM
extern "C" int func_0018A8C0(void* self, int a1, int a2)
{
    return a2 ? 0x100 : 0;
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018A8D0);

INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_updateWidget);

INCLUDE_ASM("fe/festateoptions", func_0018BEF8);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C0E8);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046C550[];
extern int D_004A14D8;

extern "C" void* func_0018C0E8(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 8) = D_0046C550;
    *(int*)((char*)self + 0xC) = 0x1E;
    D_004A14D8 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C128);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045E038[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0018C478(void* self);

extern "C" void func_0018C128(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045E038), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0018C478(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C198__FPv);
#ifdef SKIP_ASM
void* func_0018C198(void* self)
{
    return func_0039E4C0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C1B8);
#ifdef SKIP_ASM
struct sVE_18C1B8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
int GetHashValue32(char* str);
extern char D_0045DD20[];
extern int D_004A14D8;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0014F600(void* self);
extern "C" void func_0018C478(void* self);

extern "C" void func_0018C1B8(void* self, void* widget, int msg)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (msg) {
    case 0x15:
        break;
    case 0x16: {
        int h = *(int*)((char*)widget + 0xC);
        if (h == GetHashValue32(D_0045DD20)) {
            sVE_18C1B8* e = &(*(sVE_18C1B8**)((char*)widget + 8))[23];
            if (e->fn((char*)widget + e->delta, 2) != 0) {
                func_0014F600(iface);
                func_0018C478(self);
                D_004A14D8 = 1;
            }
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C270);
#ifdef SKIP_ASM
extern "C" int func_0018C270(void* self, int a1, int a2)
{
    switch (a2) {
    case 9:
        return 0x100;
    case 6:
        return 0;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018C2A0);

INCLUDE_ASM("fe/festateoptions", func_0018C478);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C658);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046C480[];
extern int D_004A14DC;

extern "C" void* func_0018C658(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 8) = D_0046C480;
    *(int*)((char*)self + 0xC) = 0x1F;
    *(char*)((char*)self + 0x50) = 0;
    D_004A14DC = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onCreateScreen);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C8B8);
#ifdef SKIP_ASM
extern "C" void func_0018D108(void* self, int idx, float x, float y);
void* func_0039E4A0(void* self);

extern "C" void* func_0018C8B8(void* self)
{
    func_0018D108(self, 0, 0.0f, 0.0f);
    func_0018D108(self, 1, 0.0f, 0.0f);
    return func_0039E4A0(self);
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018C910);

INCLUDE_ASM("fe/festateoptions", func_0018CA10);

INCLUDE_ASM("fe/festateoptions", func_0018CCE0);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018CDB0);
#ifdef SKIP_ASM
extern "C" int func_0018CDB0(void* self, int a1, int a2)
{
    return a2 ? 0x100 : 0;
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018CDC0);

INCLUDE_ASM("fe/festateoptions", func_0018CF50);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018D058);
#ifdef SKIP_ASM
extern "C" void* func_0039E510(void* self);
extern "C" void func_0018D108(void* self, int idx, float x, float y);

extern "C" void func_0018D058(void* self)
{
    func_0039E510(self);
    if (*(unsigned char*)((char*)self + 0x50) & 1) {
        if (*(int*)((char*)self + 0x48) > 0) {
            *(int*)((char*)self + 0x48) -= 1;
        } else {
            *(int*)((char*)self + 0x48) = 0;
            *(unsigned char*)((char*)self + 0x50) &= ~1;
            func_0018D108(self, 0, 0.0f, 0.0f);
        }
    }
    if (*(unsigned char*)((char*)self + 0x50) & 2) {
        if (*(int*)((char*)self + 0x4C) > 0) {
            *(int*)((char*)self + 0x4C) -= 1;
        } else {
            *(int*)((char*)self + 0x4C) = 0;
            *(unsigned char*)((char*)self + 0x50) &= ~2;
            func_0018D108(self, 1, 0.0f, 0.0f);
        }
    }
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018D108);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018D240);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046C3B0[];

extern "C" void* func_0018D240(void* self)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 0x20;
    *(void***)((char*)self + 0x8) = D_0046C3B0;
    *(char*)((char*)self + 0x48) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045E0C0[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFEStateOptionsSaveLoad_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045E0C0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045E0D0[];
extern char D_0045E0E0[];
extern char D_0045E0F0[];
extern char D_0045E100[];
extern char D_004A1508[];
extern char D_004A1500[];

static inline int IsHash_D2E8(int id, char* s) { return id == GetHashValue32(s); }

extern "C" void cFEStateOptionsSaveLoad_onWidgetCreate(void* self, void* widget)
{
    if (IsHash_D2E8(*(int*)((char*)widget + 0x38), D_0045E0D0)) {
        *(int*)((char*)widget + 0x18) = 0;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), D_0045E0E0)) {
        *(int*)((char*)widget + 0x18) = 2;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), D_0045E0F0)) {
        *(int*)((char*)widget + 0x18) = 1;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), D_0045E100)) {
        *(int*)((char*)widget + 0x18) = 3;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), D_004A1508)) {
        *(int*)((char*)widget + 0x18) = 0x300;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), D_004A1500)) {
        *(int*)((char*)widget + 0x18) = 0x200;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018D3C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_00194498(void* obj);
extern int D_004A19D8;
extern char D_0045E0F0[];
extern char D_0045E100[];

extern "C" void func_0018D3C0(void* self)
{
    if (D_004A19D8 != 0) {
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E0F0));
        if (obj != 0) {
            func_00194498(obj);
        }
        obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E100));
        if (obj != 0) {
            func_00194498(obj);
        }
    }
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018D438);

INCLUDE_ASM("fe/festateoptions", func_0018D460);

INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onWidgetEvent);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018D7F0);
#ifdef SKIP_ASM
extern "C" void* func_001D53B0(void* self, int a1, int a2);
extern "C" void* func_00227F80(void* app);
extern void* D_0046C228[];
extern void* D_004A28A8;

extern "C" void* func_0018D7F0(void* self, int a1, int a2, int a3, int a4)
{
    func_001D53B0(self, a1, a2);
    *(int*)((char*)self + 0x230) = a4;
    *(void***)((char*)self + 8) = D_0046C228;
    *(int*)((char*)self + 0xC) = 0x21;
    *(int*)((char*)self + 0x22C) = a3;
    *(int*)((char*)self + 0x234) = 0;
    *(int*)((char*)self + 0x238) = 0;
    if (a3 == 0) {
        *(int*)((char*)func_00227F80(D_004A28A8) + 0x11C) = 0;
    }
    return self;
}
#endif

INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_onCreateScreen);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018DDD0);
#ifdef SKIP_ASM
struct cList0018DDD0 {
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
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual int v54(int a);
};
extern "C" void* func_00227F80(void* app);
extern void* D_004A28A8;

extern "C" int func_0018DDD0(void* self, void* widget, unsigned int msg)
{
    char* app = (char*)func_00227F80(D_004A28A8);
    switch (msg) {
    case 8:
        if (*(int*)((char*)self + 0x22C) != 0 && (*(cList0018DDD0**)(app + 0x434))->v54(*(int*)(app + 0x428)) != 0) {
            return 0x101;
        }
    case 6:
        if ((*(cList0018DDD0**)(app + 0x434))->v54(*(int*)(app + 0x428)) == 0) {
            return 0x100;
        }
        break;
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018DEA0);

INCLUDE_ASM("fe/festateoptions", func_0018E178);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018E2C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_004A1458[];
extern char D_004A1460[];
struct cUIObj_E2C0 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setActive(int on);
};

extern "C" void func_0018E2C0(void* self, bool on)
{
    if (*(int*)((char*)self + 0x22C) != 0) {
        int off = !on;
        ((cUIObj_E2C0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1458)))->setActive(off);
        ((cUIObj_E2C0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1460)))->setActive(off);
    }
    *(int*)((char*)self + 0x220) = !on;
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018E368);

//100%
INCLUDE_ASM("fe/festateoptions", func_0018E478);
#ifdef SKIP_ASM
extern "C" void func_0039F4C0(void* list, void* item);

struct sVEntry0018E478 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

extern "C" void func_0018E478(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        int mode = *(int*)((char*)self + 0x1BC) == 1;
        if (*(int*)((char*)self + 0x1E4) == 1) {
            mode = 0;
        }
        if (*(int*)((char*)self + 0x1B4) == 0) {
            mode = 0;
        }
        if (*(int*)((char*)self + 0x230) != 0) {
            mode = 2;
        }
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry0018E478* vt = *(sVEntry0018E478**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, mode);
        if (r != 0) {
            func_0039F4C0(*(char**)((char*)self + 0x10) + 0x18, r);
        }
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

INCLUDE_ASM("fe/festateoptions", func_0018E510);


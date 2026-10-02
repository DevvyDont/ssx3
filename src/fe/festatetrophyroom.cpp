#include "common.h"

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char D_004676C8[];

extern "C" void cFEStateTrophyRoom_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004676C8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

extern "C" void* func_0039E4C0(void* self);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D43D0__FPv);
#ifdef SKIP_ASM
void* func_001D43D0(void* self)
{
    return func_0039E4C0(self);
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4660);
#ifdef SKIP_ASM
extern "C" int func_001D4660(void* self, int a1, unsigned int a2)
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
INCLUDE_ASM("fe/festatetrophyroom", func_001D4698);
#ifdef SKIP_ASM
struct sVEntry001D4698 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void func_0039F400(void* list, void* item);
extern "C" void func_001D47A0(void* self, int index);
extern "C" void func_001D4890(void* self, int index);

class cTrophyWidget_4698 {
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

extern "C" void func_001D4698(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 6: {
        cTrophyWidget_4698* w = *(cTrophyWidget_4698**)((char*)self + 0x48);
        if (w != 0) {
            w->show(0);
        }
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001D4698* vt = *(sVEntry001D4698**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)self + 0x54));
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
        break;
    }
    case 1:
        *(int*)((char*)self + 0x9C) = *(int*)((char*)item + 0x18);
        func_001D47A0(self, *(int*)((char*)item + 0x18));
        func_001D4890(self, *(int*)((char*)item + 0x18));
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4760);
#ifdef SKIP_ASM
extern "C" void func_001D4A20(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001D4760(void* self)
{
    if (*(int*)((char*)self + 0x5C) > 0) {
        func_001D4A20(self);
    }
    func_0039E510(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D47A0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00157BF0(void* self, int rider, int a, int b, int c);
extern "C" int func_001CED90(int rider, int a, int b, int c, int index);

class cTrophyWidget_47A0 {
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

extern "C" void func_001D47A0(void* self, int index)
{
    int v;
    if (index < 0) {
        goto hide;
    }
    v = -1;
    {
        if (index == 0) {
            if (func_00157BF0(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)((char*)self + 0x44),
                              *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x54),
                              *(int*)((char*)self + 0x58)) != 0) {
                v = *(int*)((char*)self + 0x60);
            }
        } else {
            int r = func_001CED90(*(signed char*)((char*)self + 0x44),
                                  *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x54),
                                  *(int*)((char*)self + 0x58), index - 1);
            if (r != -1) {
                v = *(int*)((char*)self + ((r + 1) << 2) + 0x60);
            }
        }
    }
    if (v < 0) {
    hide:
        (*(cTrophyWidget_47A0**)((char*)self + 0x48))->show(0);
    } else {
        (*(cTrophyWidget_47A0**)((char*)self + 0x48))->show(1);
        char* w = *(char**)((char*)self + 0x48);
        *(int*)(w + 0x78) = v;
        *(int*)(w + 0x7C) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4890);
#ifdef SKIP_ASM
extern "C" void setChallengeStats(void* widget, signed char rider, int a2, int a3, int a4, int index);

// Real virtual class so g++ emits the vcall itself (vptr at +8, after 8 bytes of data).
class cTrophyWidget_4890 {
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

extern "C" void func_001D4890(void* self, int index)
{
    cTrophyWidget_4890* widget = *(cTrophyWidget_4890**)((char*)self + 0x4C);
    if (widget != 0) {
        if (index == 0) {
            widget->show(0);
        } else {
            widget->show(1);
            setChallengeStats(*(void**)((char*)self + 0x4C), *(signed char*)((char*)self + 0x44),
                              *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x54),
                              *(int*)((char*)self + 0x58), index - 1);
        }
    }
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D4918);

INCLUDE_ASM("fe/festatetrophyroom", func_001D4A20);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4B20);
#ifdef SKIP_ASM
struct cGame001D4B20 {
    char pad[0x10D8];
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
    virtual void v50(int id);
};

extern void* D_004A289C;
extern void* D_004A28A8;
extern "C" void func_0019DC20(void* self, int bank, int i);
struct sSelf001D4B20 {
    char pad[0x50];
    int bank;
    char pad54[0xC];
    int snd[5];
    int tex[5];
};

extern "C" void func_001D4B20(sSelf001D4B20* self)
{
    char* mgr = *(char**)((char*)D_004A28A8 + 0x7C) + 0x1A70;
    int i;
    for (i = 0; i < 5; i++) {
        if (self->snd[i] >= 0) {
            ((cGame001D4B20*)D_004A289C)->v50(self->snd[i]);
        }
        self->snd[i] = -1;
        if (self->tex[i] >= 0) {
            func_0019DC20(mgr, self->bank, self->tex[i]);
        }
        self->tex[i] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4BC8);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, signed char a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern void* D_004695E8[];

extern "C" void* func_001D4BC8(void* self, int a1, signed char idx, int a3)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 0x2E;
    *(void***)((char*)self + 0x8) = D_004695E8;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    *(int*)((char*)self + 0x4C) = a3;
    *(int*)((char*)self + 0x48) = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);
extern char D_00467738[];

extern "C" void cFEStateRewardsRoom_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00467738), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0028F140(func_0028B180(), 6);
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4EA8);
#ifdef SKIP_ASM
struct sVEntry001D4EA8 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void func_0039F400(void* list, void* item);

extern "C" void func_001D4EA8(void* self, void* item, unsigned int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 0:
        break;
    case 5: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001D4EA8* vt = *(sVEntry001D4EA8**)((char*)obj + 4);
        void* r = vt[4].fn((char*)obj + vt[4].delta, self, *(int*)((char*)item + 0x18));
        func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        break;
    }
    case 6: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001D4EA8* vt = *(sVEntry001D4EA8**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
        break;
    }
    }
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D4F68);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D4F90);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_00157210(void* iface, int index);
extern "C" int func_00157518(void* iface, int rider, int a, int index);
extern char D_004A2020[];

extern "C" void func_001D4F90(void* self, int index, cUIText* text)
{
    char buf[256];
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xD);
    int total = func_00157210(iface, index);
    int got = func_00157518(iface, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0x48), index);
    sprintf(buf, D_004A2020, got, total);
    cUIText_setAsciiString(text, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5038);
#ifdef SKIP_ASM
extern "C" void* func_001BEE30(void* self);
extern void* D_00468050[];

extern "C" void* func_001D5038(void* self)
{
    func_001BEE30(self);
    *(void***)((char*)self + 0x8) = D_00468050;
    *(int*)((char*)self + 0xC) = 0x34;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5078);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char D_00467778[];

extern "C" void func_001D5078(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00467778), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D50E0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_00467790[];
extern char D_00461CC0[];

extern "C" void func_001D50E0(void* self, cUIText* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(D_00467790)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(D_00461CC0));
    }
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D5138);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5240);
#ifdef SKIP_ASM
extern "C" int func_001A9668(void* self, void* a1, int a2);
extern "C" int func_001BF6C0(void* self, void* a1, int a2);

extern "C" int func_001D5240(void* self, void* a1, int a2)
{
    int r;
    if (*(int*)((char*)a1 + 0x18) == 0x102) {
        r = func_001A9668(self, a1, a2);
    } else {
        r = func_001BF6C0(self, a1, a2);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5280);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_0039F698(void* p);
extern "C" void* cUIStateStack_getCurrentState(void* self);
extern "C" void* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B3F78(void* mem, void* owner);
extern "C" void func_001A8A40(void* self, void* obj);
extern char D_00460BF0[];

extern "C" void func_001D5280(void* self, void* item, int msg)
{
    if (msg == 0xF || msg == 0x14) {
        func_001A97B8(self, item, msg);
        func_0039F698((char*)*(void**)((char*)self + 0x10) + 0x18);
        void* state = cUIStateStack_getCurrentState((char*)*(void**)((char*)self + 0x10) + 0x18);
        *(int*)((char*)state + 0x1C) |= 0x80;
        func_002636F0(func_002591B8(), 0);
        func_001A8A40(self, func_001B3F78(cMemMan_alloc(0x91C, D_00460BF0, 0, 0), *(void**)((char*)self + 0x10)));
    } else {
        func_001A97B8(self, item, msg);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5330);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" void func_001D5488(void* self, int a1);
extern void* D_00474E08[];
extern void* D_0046C980[];
extern unsigned short D_004A1390[];

extern "C" void* func_001D5330(void* self, int a1, int a2, int a3)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0x98) = a3;
    *(void***)((char*)self + 0x8) = D_00474E08;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(unsigned short*)((char*)self + 0x58) = D_004A1390[0];
    *(void***)((char*)self + 0x8) = D_0046C980;
    func_001D5488(self, a2);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D53B0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" void func_001D5488(void* self, int a1);
extern void* D_00474E08[];
extern void* D_0046C980[];
extern unsigned short D_004A1390[];

extern "C" void* func_001D53B0(void* self, int a1, int a2)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0x98) = 0;
    *(void***)((char*)self + 0x8) = D_00474E08;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(unsigned short*)((char*)self + 0x58) = D_004A1390[0];
    *(void***)((char*)self + 0x8) = D_0046C980;
    func_001D5488(self, a2);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5428);
#ifdef SKIP_ASM
extern void* D_0046C980[];
extern void* D_00474E08[];
extern void* D_004A2028;
extern "C" void func_0039E390(void* self, int flags);

extern "C" void func_001D5428(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_0046C980;
    D_004A2028 = 0;
    *(void***)((char*)self + 8) = D_00474E08;
    func_0039E390(self, flags);
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D5460);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5488);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D58B8);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cMemMan_free(void* p);
extern "C" void func_00152700(void* iface);
extern "C" void func_00152728(void* iface);
extern "C" void func_0020A430(void* self);
extern "C" void* func_00227F80(void* app);
extern "C" void func_0023C860(void* self);
extern "C" void func_0023D5E8(void* self);
extern "C" void func_002410A0(void* self);

extern "C" void func_001D58B8(void* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 5);
    if (*(int*)((char*)self + 0x1BC) == 1 && *(int*)((char*)self + 0x228) == 2 && *(int*)((char*)self + 0x1B4) == 0) {
        func_00152728(iface);
        *(int*)((char*)self + 0x1D4) = 0;
        *(int*)((char*)self + 0x1D0) = 0;
        *(int*)((char*)self + 0x228) = 0;
    }
    func_00152700(iface);
    char* app = (char*)func_00227F80(D_004A28A8);
    if (*(int*)(app + 0x434) != 0) {
        func_002410A0(app);
        func_0023D5E8(app);
        func_0023C860(app);
    }
    if (*(int*)((char*)self + 0x228) == 1) {
        *(int*)((char*)self + 0x228) = 0;
        if (*(void**)((char*)self + 0x1D4) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x1D4));
        }
        *(int*)((char*)self + 0x1D4) = 0;
        *(int*)((char*)self + 0x1D0) = 0;
    }
    func_0020A430(self);
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D59A0);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5C28);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5D78);

INCLUDE_ASM("fe/festatetrophyroom", func_001D5DF0);

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5F38__FPv);
#ifdef SKIP_ASM
void func_001D5F38(void* self)
{
}
#endif

INCLUDE_ASM("fe/festatetrophyroom", func_001D5F40);

INCLUDE_ASM("fe/festatetrophyroom", func_001D64C0);


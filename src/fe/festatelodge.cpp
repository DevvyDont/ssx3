#include "common.h"

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A2318[];
extern int D_00441B14[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_001474C8(void* iface, int player);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);

extern "C" void cFEStateLodge_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A2318), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x48) = func_001474C8(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    D_00441B14[0] = 0;
    func_0028F140(func_0028B180(), 1);
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x50) = 0x12;
    *(int*)((char*)self + 0x54) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00146D98(void* iface, int a1);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_0046EDC0[];
extern char D_0046EDD0[];
extern char D_0046EDE0[];
extern char D_0046EDF0[];
extern char D_0046EE00[];
extern char D_0046EE10[];
extern char D_0046EE20[];
extern char D_0046EE30[];
extern char D_0046EE40[];
extern char D_0046EE50[];
extern char D_0046EE60[];
extern char D_0046EE78[];
extern char D_0046EE90[];

struct sWVt_3840 { short delta; short index; void (*fn)(void*, int); };

extern "C" void cFEStateLodge_onWidgetCreate(void* self, void* w)
{
    int h = *(int*)((char*)w + 0x38);
    if (h == GetHashValue32(D_0046EDC0)) {
        *(int*)((char*)w + 0x18) = 0;
    } else if (h == GetHashValue32(D_0046EDD0)) {
        *(int*)((char*)w + 0x18) = 1;
    } else if (h == GetHashValue32(D_0046EDE0)) {
        *(int*)((char*)w + 0x18) = 2;
    } else if (h == GetHashValue32(D_0046EDF0)) {
        *(int*)((char*)w + 0x18) = 3;
    } else if (h == GetHashValue32(D_0046EE00)) {
        *(int*)((char*)w + 0x18) = 4;
    } else if (h == GetHashValue32(D_0046EE10)) {
        *(int*)((char*)w + 0x18) = 5;
    } else if (h == GetHashValue32(D_0046EE20)) {
        *(int*)((char*)w + 0x18) = 8;
    } else if (h == GetHashValue32(D_0046EE30)) {
        *(int*)((char*)w + 0x18) = 6;
    } else if (h == GetHashValue32(D_0046EE40)) {
        *(int*)((char*)w + 0x18) = 7;
        sWVt_3840* vt = *(sWVt_3840**)((char*)w + 8);
        vt[8].fn((char*)w + vt[8].delta, 1);
        vt = *(sWVt_3840**)((char*)w + 8);
        vt[9].fn((char*)w + vt[9].delta, 0);
    } else if (h == GetHashValue32(D_0046EE50)) {
        int lvl = func_00146D98(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
        if (lvl >= 17) {
            if (lvl < 19) {
                cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(D_0046EE60));
                return;
            }
            if (lvl < 21) {
                cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(D_0046EE78));
                return;
            }
        }
        cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(D_0046EE90));
    }
}
#endif

INCLUDE_ASM("fe/festatelodge", func_001F3A38);

INCLUDE_ASM("fe/festatelodge", func_001F3CC8);

INCLUDE_ASM("fe/festatelodge", func_001F3CF0);

//100%
INCLUDE_ASM("fe/festatelodge", func_001F3FF8);
#ifdef SKIP_ASM
extern void* D_004739D8[];
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* func_001F3FF8(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_004739D8;
    *(int*)((char*)self + 0xC) = 0x28;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), 0);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046EF20[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int player);
extern "C" int func_001577A0(void* self, int a1, int a2);

extern "C" void cFEStateLodgeRiderDetail_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046EF20), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x48) = 1;
    int charID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    if (func_001577A0(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)((char*)self + 0x44), charID) == 0) {
        *(int*)((char*)self + 0x48) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", func_001F4108);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00147138(void* self, int a1, const char* name);
extern "C" void func_001474A8(void* self, int a1, int a2);
extern "C" int strlen(const char* s);
extern char D_0046EF40[];

struct sVEntryK1F4108 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline int IsK1F4108(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline void commitK1F4108(void* np)
{
    sVEntryK1F4108* vt = *(sVEntryK1F4108**)((char*)np + 0xC);
    vt[1].fn((char*)np + vt[1].delta);
}

extern "C" void func_001F4108(void* self, char* popup, int event)
{
    void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (event != 0x16)
        return;
    if (IsK1F4108(*(int*)(popup + 0xC), D_0046EF40)) {
        int v = *(int*)(popup + 0x70);
        if (*(int*)(popup + 0x74) != 0) {
            void* np2 = cBE_getInterface_Fv(cBE_getBE(), 1);
            func_001474A8(np2, *(signed char*)((char*)self + 0x44), v);
            commitK1F4108(np2);
        }
    } else if (*(int*)(popup + 0xC) == 0) {
        if (*(int*)(popup + 0x5C) != 0) {
            char* name = popup + 0x74;
            if (strlen(name) != 0) {
                func_00147138(np, *(signed char*)((char*)self + 0x44), name);
                commitK1F4108(np);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
extern "C" int func_00146E98(void* iface, int a1);
extern "C" void* func_0014EEC8(void* self, int player, int index);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern char D_0046EF50[];
extern char D_0046EF60[];
extern char D_0046EF70[];
extern char D_0046EF80[];
extern char D_0046EF90[];
extern char D_0046EFA0[];
extern char D_0046EFB0[];
extern char D_0046EFC0[];

extern "C" void cFEStateLodgeRiderDetail_onWidgetCreate(void* self, void* w)
{
    int h = *(int*)((char*)w + 0x38);
    if (h == GetHashValue32(D_0046EF50)) {
        *(int*)((char*)w + 0x18) = 9;
    }     else if (h == GetHashValue32(D_0046EF60)) {
        *(int*)((char*)w + 0x18) = 10;
    }     else if (h == GetHashValue32(D_0046EF70)) {
        *(int*)((char*)w + 0x18) = 11;
    }     else if (h == GetHashValue32(D_0046EF80)) {
        *(int*)((char*)w + 0x18) = 12;
    }     else if (h == GetHashValue32(D_0046EF90)) {
        *(int*)((char*)w + 0x18) = 13;
    }     else if (h == GetHashValue32(D_0046EFA0)) {
        *(int*)((char*)w + 0x18) = 14;
    }     else if (h == GetHashValue32(D_0046EFB0)) {
        *(int*)((char*)w + 0x18) = 15;
    } else if (h == GetHashValue32(D_0046EFC0)) {
        void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
        signed char cid = cBENewPlayerInterface_getRiderCharID(np, func_00146E98(np, 0));
        cUIText_setAsciiString((cUIText*)w, (const char*)func_0014EEC8(cBE_getInterface_Fv(cBE_getBE(), 2), cid, 0));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", func_001F4380);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_00194498(void* obj);
extern char D_0046EF70[];

extern "C" void func_001F4380(void* self)
{
    if (*(int*)((char*)self + 0x48) == 0) {
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046EF70));
        if (obj != 0) {
            func_00194498(obj);
        }
    }
}
#endif

INCLUDE_ASM("fe/festatelodge", func_001F43D8);

INCLUDE_ASM("fe/festatelodge", func_001F4400);

INCLUDE_ASM("fe/festatelodge", func_001F4728);


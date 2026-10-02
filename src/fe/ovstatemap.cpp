#include "common.h"

INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onCreateScreen);

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

INCLUDE_ASM("fe/ovstatemap", func_00209E78);

INCLUDE_ASM("fe/ovstatemap", func_0020A088);

INCLUDE_ASM("fe/ovstatemap", func_0020A1A8);

INCLUDE_ASM("fe/ovstatemap", func_0020A380);

INCLUDE_ASM("fe/ovstatemap", func_0020A430);

INCLUDE_ASM("fe/ovstatemap", func_0020A4E0);

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


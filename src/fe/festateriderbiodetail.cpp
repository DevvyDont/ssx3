#include "common.h"

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_onCreateScreen);

INCLUDE_ASM("fe/festateriderbiodetail", func_00190A08);

//100%
INCLUDE_ASM("fe/festateriderbiodetail", func_00190CD8);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);

struct sNames_0CD8 {
    char* name[10];
};
extern sNames_0CD8 D_0045E470;
extern char D_004A1670[];

extern "C" void func_00190CD8(void* self)
{
    sNames_0CD8 names = D_0045E470;
    char buf[32];
    for (int i = 0; i < 10; i++) {
        sprintf(buf, D_004A1670, names.name[i]);
        if (i == *(int*)((char*)self + 0x48)) {
            cUIState_showObjSafe(self, names.name[i]);
            cUIState_showObjSafe(self, buf);
        } else {
            cUIState_hideObjSafe(self, names.name[i]);
            cUIState_hideObjSafe(self, buf);
        }
    }
}
#endif

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillDNAInfo);

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillFavesInfo);

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillQnAInfo);

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillBioInfo);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* func_0014EEC8(void* self, int player, int index);
extern "C" void func_003A18C0(void* self);
extern "C" void func_003A1F18(void* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_0045E438[];
extern char D_0045F7D0[];

extern "C" void cFEStateRiderBio_fillBioInfo(void* self)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E438));
    if (obj) {
        char buf[64];
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 2);
        sprintf(buf, D_0045F7D0, func_0014EEC8(iface, *(int*)((char*)self + 0x48), 0));
        func_003A18C0(obj);
        func_003A1F18(obj, GetHashValue32(buf));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", func_001912D0);
#ifdef SKIP_ASM
struct sVEntry001912D0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void func_0039F400(void* list, void* item);
extern "C" void func_00190A08(void* self);

extern "C" void func_001912D0(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 6: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001912D0* vt = *(sVEntry001912D0**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
        break;
    }
    case 9:
        *(int*)((char*)self + 0x4C) = *(unsigned char*)((char*)item + 0x319);
        func_00190A08(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", func_00191360);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" char func_001A1CD0(void* p, int a);
extern void* D_0046B090[];

extern "C" void* func_00191360(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046B090;
    *(int*)((char*)self + 0x48) = 0;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), 0);
    *(int*)((char*)self + 0xC) = 0x14;
    return self;
}
#endif


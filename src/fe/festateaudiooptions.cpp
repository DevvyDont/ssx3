#include "common.h"

INCLUDE_ASM("fe/festateaudiooptions", cFEStateAudioOptions_onWidgetCreate);

INCLUDE_ASM("fe/festateaudiooptions", cFEStateAudioOptions_onWidgetEvent);

//100%
INCLUDE_ASM("fe/festateaudiooptions", func_00196B08);
#ifdef SKIP_ASM
extern "C" int func_00196B08(void* self, int a1, int a2, int a3)
{
    if (a2 == 6 && (a3 == 2 || a3 == 3 || a3 == 4)) {
        if (a3 == 2 && *(int*)((char*)self + 0x7C) == 0) {
            goto ok;
        }
        if (a3 == 3 && *(int*)((char*)self + 0x7C) == 0) {
            goto ok;
        }
        if (a3 == 4 && *(int*)((char*)self + 0x74) == 0 && *(int*)((char*)self + 0x7C) == 0) {
        ok:
            return 0x10;
        }
    } else if (a2 == 8 || a2 == 9) {
        return 0;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/festateaudiooptions", func_00196B90);

INCLUDE_ASM("fe/festateaudiooptions", func_00196DB0);

//100%
INCLUDE_ASM("fe/festateaudiooptions", func_00196E80);
#ifdef SKIP_ASM
struct sVEntry00196E80 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

int GetHashValue32(char* str);
void* func_0039E4A0(void* self);
extern "C" void* func_0039F9D8(void* self, int id);
extern char D_004A18A8[];
extern void* D_004A28A8;

extern "C" void func_00196E80(void* self)
{
    if (*(int*)((char*)D_004A28A8 + 0x84) != 0) {
        void* obj = func_0039F9D8(*(char**)((char*)self + 0x10) + 0x18, GetHashValue32(D_004A18A8));
        if (obj != 0) {
            sVEntry00196E80* e = &(*(sVEntry00196E80**)((char*)obj + 8))[24];
            e->fn((char*)obj + e->delta, 4, 0);
        }
    }
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", func_00196F00);
#ifdef SKIP_ASM
extern "C" void func_00186518(void* self);
extern "C" void func_00197B88(void* self);
extern "C" void func_00197E70(void* self);
extern "C" void cFEStateRequestLine_updateButtonsText(void* self, int a1);
extern "C" void cFEStateRequestLine_updateHelpText(void* self, int a1);

extern "C" void func_00196F00(void* self)
{
    func_00186518(self);
    *(int*)((char*)self + 0x150) = 0;
    func_00197B88(self);
    func_00197E70(self);
    cFEStateRequestLine_updateButtonsText(self, 0);
    cFEStateRequestLine_updateHelpText(self, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", func_00196F50);
#ifdef SKIP_ASM
extern "C" void func_00197DB8(void* self);
extern "C" void cFEStateRequestLine_updateButtonsText(void* self, int a1);
extern "C" void cFEStateRequestLine_updateHelpText(void* self, int a1);

extern "C" int func_00196F50(void* self, int a1)
{
    if (a1 != 0) {
        func_00197DB8(self);
        cFEStateRequestLine_updateButtonsText(self, 0);
        cFEStateRequestLine_updateHelpText(self, 0);
    }
    return 0;
}
#endif


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

INCLUDE_ASM("fe/festateaudiooptions", func_00196E80);

INCLUDE_ASM("fe/festateaudiooptions", func_00196F00);

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


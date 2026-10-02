#include "common.h"

//100%
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00460168[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFEStateBonusMaterial_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00460168), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebonusmaterial", func_00195540);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];

extern "C" void func_00195540(void* self, void* menu)
{
    int h = *(int*)((char*)menu + 0x38);
    if (h == GetHashValue32(D_004A14F8)) {
        *(int*)((char*)menu + 0x18) = 0;
    }
    h = *(int*)((char*)menu + 0x38);
    if (h == GetHashValue32(D_004A1500)) {
        *(int*)((char*)menu + 0x18) = 1;
    }
    h = *(int*)((char*)menu + 0x38);
    if (h == GetHashValue32(D_004A1508)) {
        *(int*)((char*)menu + 0x18) = 2;
    }
}
#endif

INCLUDE_ASM("fe/festatebonusmaterial", func_001955B8);

extern "C" void* func_0039E4C0(void* self);

//100%
INCLUDE_ASM("fe/festatebonusmaterial", func_001955E0__FPv);
#ifdef SKIP_ASM
void* func_001955E0(void* self)
{
    return func_0039E4C0(self);
}
#endif

INCLUDE_ASM("fe/festatebonusmaterial", func_00195600);


#include "common.h"

INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onCreateScreen);

INCLUDE_ASM("fe/festateriderbio", func_001833D0);

//100%
INCLUDE_ASM("fe/festateriderbio", func_00183550);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_00194498(void* obj);
extern char D_0045D898[];

extern "C" void func_00183550(void* self)
{
    if (*(int*)((char*)self + 0x50) == 0) {
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045D898));
        if (obj != 0) {
            func_00194498(obj);
        }
    }
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festateriderbio", func_001835A8__FPv);
#ifdef SKIP_ASM
void* func_001835A8(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", func_001835C8);
#ifdef SKIP_ASM
extern "C" void func_0039E510(void*);
extern "C" void func_00182EC0(void*);

extern "C" void func_001835C8(void* self)
{
    func_0039E510(self);
    func_00182EC0(self);
}
#endif

INCLUDE_ASM("fe/festateriderbio", func_001835F8);

INCLUDE_ASM("fe/festateriderbio", func_00183710);

INCLUDE_ASM("fe/festateriderbio", func_00183A98);

INCLUDE_ASM("fe/festateriderbio", func_00183B08);

INCLUDE_ASM("fe/festateriderbio", func_00183C08);

INCLUDE_ASM("fe/festateriderbio", func_00183D08);


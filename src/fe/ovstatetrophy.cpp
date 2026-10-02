#include "common.h"

//100%
INCLUDE_ASM("fe/ovstatetrophy", cOVStateTrophy_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char D_00471CD8[];

extern "C" void cOVStateTrophy_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00471CD8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_0020E9A0);

INCLUDE_ASM("fe/ovstatetrophy", func_0020EAA0);

INCLUDE_ASM("fe/ovstatetrophy", func_0020EB08);

INCLUDE_ASM("fe/ovstatetrophy", func_0020EB50);

INCLUDE_ASM("fe/ovstatetrophy", func_0020EC18);

INCLUDE_ASM("fe/ovstatetrophy", func_0020ED20);

INCLUDE_ASM("fe/ovstatetrophy", func_0020EDA0);

INCLUDE_ASM("fe/ovstatetrophy", func_0020FB40);

struct sPad16 { char x; int pad[3]; };
extern sPad16 D_004C8BC8;

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_002105B0);
#ifdef SKIP_ASM
extern "C" void func_002105B0(int a0)
{
    char* p = (char*)&D_004C8BC8 + a0 * 0x18;
    *(int*)p = 1;
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_002105D0);

INCLUDE_ASM("fe/ovstatetrophy", func_00210608);

INCLUDE_ASM("fe/ovstatetrophy", func_00210618);

INCLUDE_ASM("fe/ovstatetrophy", func_00210820);

INCLUDE_ASM("fe/ovstatetrophy", func_002108F8);

INCLUDE_ASM("fe/ovstatetrophy", func_00210940);

INCLUDE_ASM("fe/ovstatetrophy", func_00210B58);

INCLUDE_ASM("fe/ovstatetrophy", func_00210BD8);

INCLUDE_ASM("fe/ovstatetrophy", func_00210D20);

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210DD0);
#ifdef SKIP_ASM
extern "C" void func_00210DD0(void* self, int a1, int a2)
{
    if (a2 == 0x16) {
        if (a1 == *(int*)((char*)self + 0xe0)) {
            *(int*)((char*)self + 0xe0) = 0;
        }
    }
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_00210DF0);

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210F40);
#ifdef SKIP_ASM
extern "C" void func_00210FA0(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_001CCF00(void* p, int a);
extern "C" void func_001CB418(void* p, int a);

extern "C" void func_00210F40(void* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    func_00210FA0(self, a1, a4, a5, a6);
    func_001CCF00(*(void**)((char*)self + 0xE0), a2);
    func_001CB418(*(void**)((char*)self + 0xE0), a3);
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_00210FA0);

INCLUDE_ASM("fe/ovstatetrophy", func_00211088);


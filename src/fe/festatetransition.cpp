#include "common.h"

//100%
INCLUDE_ASM("fe/festatetransition", cFEStateBackground_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char D_0045FFD8[];

extern "C" void cFEStateBackground_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045FFD8), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetransition", cFEStateTransition_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern char D_0045FFE8[];
extern char D_0045FFF8[];

extern "C" void cFEStateTransition_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* engine2;
    *(void**)((char*)self + 0x48) = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045FFE8), 0);
    engine2 = *(void**)((char*)self + 0x10);
    *(void**)((char*)self + 0x4C) = cUIEngine_addScreenByHashName(engine2, self, GetHashValue32(D_0045FFF8), 0);
}
#endif

INCLUDE_ASM("fe/festatetransition", cFEStateTransition_onScreenEvent);

INCLUDE_ASM("fe/festatetransition", func_001946A8);

//100%
INCLUDE_ASM("fe/festatetransition", func_00194738);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046BAB8[];

extern "C" void* func_00194738(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046BAB8;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0xC) = 0;
    return self;
}
#endif


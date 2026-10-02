#include "common.h"

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045E240[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFEStateProfileSelect_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045E240), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018EB40);

INCLUDE_ASM("fe/festateprofile", func_0018ECA0);

INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onCreateScreen);

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018EE98__FPv);
#ifdef SKIP_ASM
void* func_0018EE98(void* self)
{
    return func_0039E6B8(self);
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018EEB8);

INCLUDE_ASM("fe/festateprofile", func_0018EF70);

INCLUDE_ASM("fe/festateprofile", func_0018F0B8);

INCLUDE_ASM("fe/festateprofile", func_0018F168);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F278);
#ifdef SKIP_ASM
extern "C" void func_0018F2B8(void* self);
extern "C" void func_0018F9D8(void* self);

extern "C" void func_0018F278(void* self)
{
    if (*(int*)((char*)self + 0x214) == 3) {
        func_0018F2B8(self);
    } else {
        func_0018F9D8(self);
    }
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018F2B8);

INCLUDE_ASM("fe/festateprofile", func_0018F9D8);

INCLUDE_ASM("fe/festateprofile", func_0018FAF0);

extern "C" void* func_001D58B8(void*);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018FC70__FPv);
#ifdef SKIP_ASM
void func_0018FC70(void* self)
{
    func_001D58B8(self);
    *(int*)((char*)self + 0x1a4) = 0;
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018FC98);

INCLUDE_ASM("fe/festateprofile", func_0018FD90);

INCLUDE_ASM("fe/festateprofile", func_001902C0);

INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initCreateScreen);

INCLUDE_ASM("fe/festateprofile", func_00190670);

//100%
INCLUDE_ASM("fe/festateprofile", func_00190768);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_0045E318[];
extern char D_0045E348[];
extern char D_004A15D0[];
extern char D_004A1458[];
struct cUIObj_0768 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setActive(int on);
};

extern "C" void func_00190768(void* self)
{
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E318)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E348)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15D0)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1458)))->setActive(1);
    *(int*)((char*)self + 0x220) = 1;
    *(int*)((char*)self + 0x224) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_00190858);
#ifdef SKIP_ASM
extern void* D_0046B160[];
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* func_00190858(void* self, int a1, int a2)
{
    signed char idx = a2;
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046B160;
    *(int*)((char*)self + 0xC) = 0xF;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    return self;
}
#endif


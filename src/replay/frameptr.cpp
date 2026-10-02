#include "common.h"

INCLUDE_ASM("replay/frameptr", cReplayFramePtr_initBlock);

//100%
INCLUDE_ASM("replay/frameptr", func_0026E950);
#ifdef SKIP_ASM
extern "C" void func_0026E950(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x4) = a1;
    *(int*)((char*)self + 0x8) = a1 + a2;
    *(int*)((char*)self + 0xc) = a1;
    *(int*)((char*)self + 0x10) = a2;
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026E968);

struct cReplayFramePtr {
    char pad_0x00[0x4];
    int field_0x4;
    char pad_0x08[0x4];
    int field_0xC;
};

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_readRewind__FP15cReplayFramePtr);
#ifdef SKIP_ASM
void cReplayFramePtr_readRewind(cReplayFramePtr* self)
{
    self->field_0xC = self->field_0x4;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026E9C0__FPv);
#ifdef SKIP_ASM
int func_0026E9C0(void* self)
{
    int t0 = *(int*)((char*)self + 0x4);
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x8) = t0;
    return t0;
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026E9D0);

INCLUDE_ASM("replay/frameptr", func_0026EA20);

INCLUDE_ASM("replay/frameptr", func_0026EA68);

INCLUDE_ASM("replay/frameptr", func_0026EAB8);

INCLUDE_ASM("replay/frameptr", func_0026EB48);

INCLUDE_ASM("replay/frameptr", func_0026EC08);

INCLUDE_ASM("replay/frameptr", func_0026ECD8);

INCLUDE_ASM("replay/frameptr", func_0026ED28);

//100%
INCLUDE_ASM("replay/frameptr", func_0026ED88__FPv);
#ifdef SKIP_ASM
void func_0026ED88(void* self)
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_getFrameBlock__Fv);
#ifdef SKIP_ASM
void cReplayFramePtr_getFrameBlock()
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026ED98);
#ifdef SKIP_ASM
extern "C" void func_0026EDD8(void* self);

extern "C" void func_0026ED98(void* self)
{
    *(int*)((char*)self + 0x630) = (*(int*)((char*)self + 0x630) + 1) % 9;
    func_0026EDD8(self);
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026EDD8);

//100%
INCLUDE_ASM("replay/frameptr", func_0026EE68);
#ifdef SKIP_ASM
void* func_0016D1D8(void* p);
extern void* D_004C5830[];
extern "C" void func_0026EDD8(void* self);

extern "C" void func_0026EE68(void* self)
{
    func_0016D1D8(D_004C5830);
    func_0026EDD8(self);
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026EEA0);

INCLUDE_ASM("replay/frameptr", func_0026EF30);

INCLUDE_ASM("replay/frameptr", func_0026EF80);

INCLUDE_ASM("replay/frameptr", func_0026EFB8);

INCLUDE_ASM("replay/frameptr", func_0026F088);

INCLUDE_ASM("replay/frameptr", func_0026F180);

INCLUDE_ASM("replay/frameptr", func_0026F228);

INCLUDE_ASM("replay/frameptr", func_0026F428);

//100%
INCLUDE_ASM("replay/frameptr", func_0026F498__FPv);
#ifdef SKIP_ASM
void func_0026F498(void* self)
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026F4A0__FPvi);
#ifdef SKIP_ASM
void func_0026F4A0(void* self, int val)
{
    *(int*)((char*)self + 0x0) = val;
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026F4A8);

//100%
INCLUDE_ASM("replay/frameptr", func_0026F7B8);
#ifdef SKIP_ASM
void func_0026F4A0(void* self, int state);

extern "C" void func_0026F7B8(int* self)
{
    int s = *self;
    bool ok = !(s == 14 || s == 15);
    if (ok) {
        if (s == 10) {
            func_0026F4A0(self, 11);
        }
    }
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026F7F8);

INCLUDE_ASM("replay/frameptr", func_0026F850);

//100%
INCLUDE_ASM("replay/frameptr", func_0026F898__FPvi);
#ifdef SKIP_ASM
void func_0026F898(void* self, int val)
{
    *(int*)((char*)self + 0x28) = val;
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026F8A0);

INCLUDE_ASM("replay/frameptr", func_0026F980);

//100%
INCLUDE_ASM("replay/frameptr", func_0026FA50);
#ifdef SKIP_ASM
extern "C" void func_0026FA50(void* self)
{
    int s = *(int*)((char*)self + 0x0);
    bool ok = !(s == 0xE || s == 0xF);
    if (ok) {
        *(int*)((char*)self + 0x60C) = 1;
    }
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026FA78);

INCLUDE_ASM("replay/frameptr", func_0026FB88);

INCLUDE_ASM("replay/frameptr", func_0026FE50);


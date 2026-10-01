#include "common.h"

INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_cScriptCameraController);

INCLUDE_ASM("camera/script/scriptcontroller", func_001690D0);

INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_addCamera);

INCLUDE_ASM("camera/script/scriptcontroller", func_001692D8);

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_00169340);
#ifdef SKIP_ASM
extern "C" void cCameraAlgoList_insert(void*, void*, float);

extern "C" void func_00169340(void* self, void* algo)
{
    cCameraAlgoList_insert(*(void**)((char*)self + 0x8), algo, 1.0f);
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_00169368);

INCLUDE_ASM("camera/script/scriptcontroller", func_00169418);

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_001694A8__FPvi);
#ifdef SKIP_ASM
void func_001694A8(void* self, int val)
{
    *(int*)((char*)self + 0x14) = val;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_001694B0__FPv);
#ifdef SKIP_ASM
int func_001694B0(void* self)
{
    return *(int*)((char*)self + 0x14);
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_001694B8);

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_00169518);
#ifdef SKIP_ASM
class cScriptCtlVirt {
public:
    char pad[0x10];
    // vptr lands at 0x10 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

extern "C" void func_00169518(cScriptCtlVirt* self)
{
    self->v05();
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_00169540);

INCLUDE_ASM("camera/script/scriptcontroller", func_00169570);

INCLUDE_ASM("camera/script/scriptcontroller", func_00169828);

INCLUDE_ASM("camera/script/scriptcontroller", func_00169CF8);

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_00169D58);
#ifdef SKIP_ASM
extern "C" void func_00169D58(cScriptCtlVirt* self)
{
    self->v05();
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_00169D80);

INCLUDE_ASM("camera/script/scriptcontroller", func_00169DB0);

INCLUDE_ASM("camera/script/scriptcontroller", func_00169F88);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016A458);

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016A4B8);
#ifdef SKIP_ASM
extern "C" void func_0016A4B8(cScriptCtlVirt* self)
{
    self->v05();
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_0016A4E0);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016A510);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016A868);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016AD38);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016AD98);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016B150);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016B180);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016B7B8);

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BEE8__FPv);
#ifdef SKIP_ASM
void* func_0016BEE8(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016BEF8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_0016BEF8(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif


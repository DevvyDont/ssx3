#include "common.h"

INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_cleanupOnExit);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CA10__FPv);
#ifdef SKIP_ASM
void func_0016CA10(void* self)
{
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_purge);

INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_loadTriggers);

INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_streamIn);

extern void* D_004C5830[];
extern "C" void* func_0016CEF0(void*, void*, int);

//80.0%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CEC8__FPvi);
#ifdef SKIP_ASM
void* func_0016CEC8(void* self, int a1)
{
    return func_0016CEF0((void*)D_004C5830, self, a1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CEF0);
#ifdef SKIP_ASM
extern "C" void cCameraTriggerMan_streamIn(void* self, void* stream);

// PORT: the unit declares func_0016CEF0 as returning void*, but the body returns nothing.
void func_0016CEF0_impl(void* self, void* unused, void* stream) __asm__("func_0016CEF0");
void func_0016CEF0_impl(void* self, void* unused, void* stream)
{
    if (stream != 0) {
        cCameraTriggerMan_streamIn(self, stream);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CF18);
#ifdef SKIP_ASM
// PORT: func_0016CF40__FPv is mangled as (void*), but this caller passes two args.
void func_0016CF40_2(void* mgr, void* self) __asm__("func_0016CF40__FPv");

extern "C" void func_0016CF18(void* self)
{
    func_0016CF40_2((void*)D_004C5830, self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CF40__FPv);
#ifdef SKIP_ASM
void func_0016CF40(void* self)
{
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CF48);

extern "C" void* func_0016CF48(void* self, int type);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_setInGameTriggers__FPv);
#ifdef SKIP_ASM
void cCameraTriggerMan_setInGameTriggers(void* self)
{
    func_0016CF48(self, 2);
}
#endif

//99.29%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D1D8__FPv);
#ifdef SKIP_ASM
void* func_0016D1D8(void* self)
{
    return func_0016CF48(self, 1);
}
#endif

struct cCameraTriggerStack {
    int field_0x0;
    int field_0x4;
};

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerStack_init__FP19cCameraTriggerStack);
#ifdef SKIP_ASM
void cCameraTriggerStack_init(cCameraTriggerStack* self)
{
    self->field_0x0 = 0;
    self->field_0x4 = 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D210);
#ifdef SKIP_ASM
void cCameraTriggerStack_init(cCameraTriggerStack* self);

extern "C" cCameraTriggerStack* func_0016D210(cCameraTriggerStack* self)
{
    cCameraTriggerStack_init(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D238__FPvi);
#ifdef SKIP_ASM
void func_0016D238(void* self, int a1)
{
    *(int*)((char*)self + (*(int*)self << 2) + 0x8) = a1;
    *(int*)((char*)self + 0x4) = a1;
    *(int*)self = *(int*)self + 1;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D260);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D320);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016E1D8);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016E560);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016F068);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016FBB8);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_00170240);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_001707B0);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_00170D20);


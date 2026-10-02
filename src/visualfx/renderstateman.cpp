#include "common.h"

INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourR);

INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourG);

INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourB);

INCLUDE_ASM("visualfx/renderstateman", func_002EE8A0);

INCLUDE_ASM("visualfx/renderstateman", func_002EE8E8);

INCLUDE_ASM("visualfx/renderstateman", func_002EE930);

INCLUDE_ASM("visualfx/renderstateman", func_002EE978);

INCLUDE_ASM("visualfx/renderstateman", func_002EE9C0);

INCLUDE_ASM("visualfx/renderstateman", func_002EEA08);

INCLUDE_ASM("visualfx/renderstateman", func_002EEA50);

INCLUDE_ASM("visualfx/renderstateman", func_002EEA98);

INCLUDE_ASM("visualfx/renderstateman", func_002EEAE0);

INCLUDE_ASM("visualfx/renderstateman", func_002EEB28);

INCLUDE_ASM("visualfx/renderstateman", func_002EEB70);

INCLUDE_ASM("visualfx/renderstateman", func_002EEBD0);

INCLUDE_ASM("visualfx/renderstateman", func_002EEC30);

INCLUDE_ASM("visualfx/renderstateman", func_002EEC90);

INCLUDE_ASM("visualfx/renderstateman", func_002EECF0);

INCLUDE_ASM("visualfx/renderstateman", func_002EED50);

INCLUDE_ASM("visualfx/renderstateman", func_002EEDB0);

INCLUDE_ASM("visualfx/renderstateman", func_002EEDF8);

INCLUDE_ASM("visualfx/renderstateman", func_002EEE40);

INCLUDE_ASM("visualfx/renderstateman", func_002EEE88);

INCLUDE_ASM("visualfx/renderstateman", func_002EEED0);

INCLUDE_ASM("visualfx/renderstateman", func_002EEF18);

INCLUDE_ASM("visualfx/renderstateman", func_002EEF60);

INCLUDE_ASM("visualfx/renderstateman", func_002EEFA8);

INCLUDE_ASM("visualfx/renderstateman", func_002EEFF0);

INCLUDE_ASM("visualfx/renderstateman", func_002EF038);

INCLUDE_ASM("visualfx/renderstateman", func_002EF0A0);

INCLUDE_ASM("visualfx/renderstateman", func_002EF0E8);

INCLUDE_ASM("visualfx/renderstateman", func_002EF140);

INCLUDE_ASM("visualfx/renderstateman", func_002EF198);

INCLUDE_ASM("visualfx/renderstateman", func_002EF248);

INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_readFromReplayFrame);

INCLUDE_ASM("visualfx/renderstateman", func_002EF368);

INCLUDE_ASM("visualfx/renderstateman", func_002EF378);

INCLUDE_ASM("visualfx/renderstateman", func_002EF3B0);

INCLUDE_ASM("visualfx/renderstateman", func_002EF530);

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF6A0);
#ifdef SKIP_ASM
extern "C" void func_002EF6A0(void* self)
{
    int mode = (*(int*)(*(char**)self + 0x870) >= 0) ? 0x19 : 0x12;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x14) = mode;
}
#endif

INCLUDE_ASM("visualfx/renderstateman", func_002EF6D0);

INCLUDE_ASM("visualfx/renderstateman", func_002EF950);

INCLUDE_ASM("visualfx/renderstateman", func_002EFF98);

INCLUDE_ASM("visualfx/renderstateman", func_002F00A0);

INCLUDE_ASM("visualfx/renderstateman", func_002F0368);

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F0390);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002F03C8(void* self);
extern char D_00487B88[];

extern "C" void* func_002F0390(void)
{
    return func_002F03C8(cMemMan_alloc(8, D_00487B88, 0, 0));
}
#endif

INCLUDE_ASM("visualfx/renderstateman", func_002F03C8);

INCLUDE_ASM("visualfx/renderstateman", func_002F0438);

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F04B0);
#ifdef SKIP_ASM
class func_002F04B0_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
};

extern "C" void func_002F04B0(func_002F04B0_cObj** self)
{
    (*self)->v02();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F04E0);
#ifdef SKIP_ASM
extern "C" void func_002F04E0(func_002F04B0_cObj** self)
{
    (*self)->v03();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F0510);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPSPVisualEffectsMan_cPSPVisualEffectsMan(void* self);
extern char D_00487BA0[];

extern "C" void* func_002F0510(void)
{
    return cPSPVisualEffectsMan_cPSPVisualEffectsMan(cMemMan_alloc(0x48, D_00487BA0, 0, 0));
}
#endif


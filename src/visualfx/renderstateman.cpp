#include "common.h"

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourR);
#ifdef SKIP_ASM
struct sRSMVEntry {
    short delta;
    short index;
    float* (*fn)(void*);
};

struct sRSMObj {
    int field_0x0;
    sRSMVEntry* vt;
};

struct sRSMEntry {
    char pad0[0x4];
    sRSMObj** p04;
    sRSMObj** p08;
    char pad0C[0x1C - 0xC];
    sRSMObj** p1C;
    sRSMObj** p20;
    char pad24[0xF0 - 0x24];
};

extern sRSMEntry D_004FA370[];

extern "C" float cRenderStateMan_SnowFlakeColourR(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p20;
    sRSMVEntry* vt = o->vt;
    return *vt[55].fn((char*)o + vt[55].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourG);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float cRenderStateMan_SnowFlakeColourG(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p20;
    sRSMVEntry* vt = o->vt;
    return *vt[56].fn((char*)o + vt[56].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourB);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float cRenderStateMan_SnowFlakeColourB(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p20;
    sRSMVEntry* vt = o->vt;
    return *vt[57].fn((char*)o + vt[57].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EE8A0);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EE8A0(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p20;
    sRSMVEntry* vt = o->vt;
    return *vt[58].fn((char*)o + vt[58].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EE8E8);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EE8E8(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p20;
    sRSMVEntry* vt = o->vt;
    return *vt[59].fn((char*)o + vt[59].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EE930);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EE930(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p20;
    sRSMVEntry* vt = o->vt;
    return *vt[60].fn((char*)o + vt[60].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EE978);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EE978(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p20;
    sRSMVEntry* vt = o->vt;
    return *vt[61].fn((char*)o + vt[61].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EE9C0);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EE9C0(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p04;
    sRSMVEntry* vt = o->vt;
    return *vt[7].fn((char*)o + vt[7].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEA08);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEA08(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p04;
    sRSMVEntry* vt = o->vt;
    return *vt[8].fn((char*)o + vt[8].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEA50);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEA50(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p04;
    sRSMVEntry* vt = o->vt;
    return *vt[9].fn((char*)o + vt[9].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEA98);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEA98(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p04;
    sRSMVEntry* vt = o->vt;
    return *vt[10].fn((char*)o + vt[10].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEAE0);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEAE0(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p04;
    sRSMVEntry* vt = o->vt;
    return *vt[11].fn((char*)o + vt[11].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEB28);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEB28(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p04;
    sRSMVEntry* vt = o->vt;
    return *vt[12].fn((char*)o + vt[12].delta);
}
#endif

INCLUDE_ASM("visualfx/renderstateman", func_002EEB70);

INCLUDE_ASM("visualfx/renderstateman", func_002EEBD0);

INCLUDE_ASM("visualfx/renderstateman", func_002EEC30);

INCLUDE_ASM("visualfx/renderstateman", func_002EEC90);

INCLUDE_ASM("visualfx/renderstateman", func_002EECF0);

INCLUDE_ASM("visualfx/renderstateman", func_002EED50);

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEDB0);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEDB0(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p08;
    sRSMVEntry* vt = o->vt;
    return *vt[13].fn((char*)o + vt[13].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEDF8);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEDF8(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p08;
    sRSMVEntry* vt = o->vt;
    return *vt[14].fn((char*)o + vt[14].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEE40);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEE40(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p08;
    sRSMVEntry* vt = o->vt;
    return *vt[15].fn((char*)o + vt[15].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEE88);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEE88(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p08;
    sRSMVEntry* vt = o->vt;
    return *vt[16].fn((char*)o + vt[16].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEED0);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEED0(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p08;
    sRSMVEntry* vt = o->vt;
    return *vt[17].fn((char*)o + vt[17].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEF18);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEF18(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p08;
    sRSMVEntry* vt = o->vt;
    return *vt[18].fn((char*)o + vt[18].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEF60);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEF60(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p08;
    sRSMVEntry* vt = o->vt;
    return *vt[19].fn((char*)o + vt[19].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEFA8);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEFA8(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p1C;
    sRSMVEntry* vt = o->vt;
    return *vt[42].fn((char*)o + vt[42].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEFF0);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EEFF0(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p1C;
    sRSMVEntry* vt = o->vt;
    return *vt[41].fn((char*)o + vt[41].delta);
}
#endif

INCLUDE_ASM("visualfx/renderstateman", func_002EF038);

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF0A0);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EF0A0(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = *e->p20;
    sRSMVEntry* vt = o->vt;
    return *vt[47].fn((char*)o + vt[47].delta);
}
#endif

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


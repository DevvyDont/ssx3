#include "common.h"
#include "ai/riderhandler.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

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

/**
 * Returns the red component of the snowflake colour for one row of the render-state table.
 *
 * Row @p i of D_004FA370 (0xF0 bytes per row) holds at +0x20 a pointer to an object whose virtual slot 55
 * returns a pointer to the value.
 *
 * @param i  row of D_004FA370 (the avalanche effect passes 0)
 * @return the red component
 */
extern "C" float cRenderStateMan_SnowFlakeColourR(int i)
{
    sRSMEntry* entry = &D_004FA370[i];
    sRSMObj* stateObj = *entry->p20;
    sRSMVEntry* vt = stateObj->vt;
    return *vt[55].fn((char*)stateObj + vt[55].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourG);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

/**
 * Returns the green component of the snowflake colour for one row of the render-state table.
 *
 * Row @p i of D_004FA370 (0xF0 bytes per row) holds at +0x20 a pointer to an object whose virtual slot 56
 * returns a pointer to the value.
 *
 * @param i  row of D_004FA370 (the avalanche effect passes 0)
 * @return the green component
 */
extern "C" float cRenderStateMan_SnowFlakeColourG(int i)
{
    sRSMEntry* entry = &D_004FA370[i];
    sRSMObj* stateObj = *entry->p20;
    sRSMVEntry* vt = stateObj->vt;
    return *vt[56].fn((char*)stateObj + vt[56].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourB);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

/**
 * Returns the blue component of the snowflake colour for one row of the render-state table.
 *
 * Row @p i of D_004FA370 (0xF0 bytes per row) holds at +0x20 a pointer to an object whose virtual slot 57
 * returns a pointer to the value.
 *
 * @param i  row of D_004FA370 (the avalanche effect passes 0)
 * @return the blue component
 */
extern "C" float cRenderStateMan_SnowFlakeColourB(int i)
{
    sRSMEntry* entry = &D_004FA370[i];
    sRSMObj* stateObj = *entry->p20;
    sRSMVEntry* vt = stateObj->vt;
    return *vt[57].fn((char*)stateObj + vt[57].delta);
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

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEB70);
#ifdef SKIP_ASM
extern int D_004A3B60;

extern "C" float func_002EEB70(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = **(sRSMObj***)((char*)e + 0xC);
        sRSMVEntry* vt = o->vt;
        return *vt[20].fn((char*)o + vt[20].delta);
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEBD0);
#ifdef SKIP_ASM
extern int D_004A3B60;

extern "C" float func_002EEBD0(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = **(sRSMObj***)((char*)e + 0xC);
        sRSMVEntry* vt = o->vt;
        return *vt[21].fn((char*)o + vt[21].delta);
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEC30);
#ifdef SKIP_ASM
extern int D_004A3B60;

extern "C" float func_002EEC30(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = **(sRSMObj***)((char*)e + 0xC);
        sRSMVEntry* vt = o->vt;
        return *vt[22].fn((char*)o + vt[22].delta);
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EEC90);
#ifdef SKIP_ASM
extern int D_004A3B60;

extern "C" float func_002EEC90(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = **(sRSMObj***)((char*)e + 0xC);
        sRSMVEntry* vt = o->vt;
        return *vt[23].fn((char*)o + vt[23].delta);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EECF0);
#ifdef SKIP_ASM
extern int D_004A3B60;

extern "C" float func_002EECF0(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = **(sRSMObj***)((char*)e + 0xC);
        sRSMVEntry* vt = o->vt;
        return *vt[24].fn((char*)o + vt[24].delta);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EED50);
#ifdef SKIP_ASM
extern int D_004A3B60;

extern "C" float func_002EED50(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = **(sRSMObj***)((char*)e + 0xC);
        sRSMVEntry* vt = o->vt;
        return *vt[25].fn((char*)o + vt[25].delta);
    }
    return 0.0f;
}
#endif

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

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF038);
#ifdef SKIP_ASM
extern sRSMEntry D_004FA370[];

extern "C" float func_002EF038(int i)
{
    sRSMEntry* e = &D_004FA370[i];
    sRSMObj* o = **(sRSMObj***)((char*)e + 0x18);
    sRSMVEntry* vt = o->vt;
    return *vt[36].fn((char*)o + vt[36].delta) * (1.0f - *(float*)((char*)e + 0x24));
}
#endif

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

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF0E8);
#ifdef SKIP_ASM
extern int D_004A3B64;
extern int D_004A55B0;

extern "C" void* func_002EF0E8(int i)
{
    if (D_004A3B64 != 0) {
        return &D_004A55B0;
    }
    {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = *e->p1C;
        sRSMVEntry* vt = o->vt;
        return vt[37].fn((char*)o + vt[37].delta);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF140);
#ifdef SKIP_ASM
extern int D_004A3B64;
extern int D_004A55B0;

extern "C" void* func_002EF140(int i)
{
    if (D_004A3B64 != 0) {
        return &D_004A55B0;
    }
    {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = *e->p1C;
        sRSMVEntry* vt = o->vt;
        return vt[38].fn((char*)o + vt[38].delta);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF198);
#ifdef SKIP_ASM
extern int D_004A3B64;
extern int D_004A55B0;

extern "C" void* func_002EF198(int i)
{
    if (D_004A3B64 != 0) {
        return &D_004A55B0;
    }
    {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = *e->p1C;
        sRSMVEntry* vt = o->vt;
        return vt[39].fn((char*)o + vt[39].delta);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF1F0);
#ifdef SKIP_ASM
extern int D_004A3B64;
extern int D_004A55B0;

extern "C" void* func_002EF1F0(int i)
{
    if (D_004A3B64 != 0) {
        return &D_004A55B0;
    }
    {
        sRSMEntry* e = &D_004FA370[i];
        sRSMObj* o = *e->p1C;
        sRSMVEntry* vt = o->vt;
        return vt[40].fn((char*)o + vt[40].delta);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF248);
#ifdef SKIP_ASM
class cRSMObjWF {
public:
    int field_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64(void* frame);
};

extern "C" void func_002EF248(void* frame)
{
    int i;
    for (i = 0; i < 6; i++) {
        sRSMEntry* e = &D_004FA370[i];
        ((cRSMObjWF*)*e->p20)->v64(frame);
        ((cRSMObjWF*)*e->p20)->v64(frame);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_readFromReplayFrame);
#ifdef SKIP_ASM
class cRSMObjRF {
public:
    int field_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65(void* frame);
};

/**
 * Restores the render states from a replay frame.
 *
 * For each of the 6 render state entries (D_004FA370, 0xF0 bytes each), calls virtual slot 65 of the object at
 * *p20 (+0x20) twice with the frame.
 *
 * @param frame  the replay frame stream to read from
 */
extern "C" void cRenderStateMan_readFromReplayFrame(void* frame)
{
    int i;
    for (i = 0; i < 6; i++) {
        sRSMEntry* entry = &D_004FA370[i];
        ((cRSMObjRF*)*entry->p20)->v65(frame);
        ((cRSMObjRF*)*entry->p20)->v65(frame);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF368);
#ifdef SKIP_ASM
extern void* D_004A3B5C;
extern int D_004A4444;

extern "C" void func_002EF368(void* p)
{
    D_004A3B5C = p;
    D_004A4444 = p != 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EF378);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern int D_004A3B64;
extern int D_004A55B0;

extern "C" void func_002EF378(const char* name)
{
    if (name == 0) {
        D_004A3B64 = 0;
    } else {
        strcpy((char*)&D_004A55B0, name);
        D_004A3B64 = 1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_cAirStreamerFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/renderstateman_classes.h"

/**
 * Constructs an air streamer, the ribbon trail of an airborne rider; cRider is its virtual base.
 *
 * Builds the cRider base only when it is the most-derived object (a hidden second argument), sets the cRider
 * part's vtables, allocates the 25-entry ring of 0x20-byte trail segments (tag "cAirStreamerFX") at +0x4 and
 * resets the trail with readFromReplayFrame().
 */
cRsm_2EF3B0::cRsm_2EF3B0()
{
    cRsmItem_2EF3B0** slot = &items;
    *slot = new (str_cAirStreamerFX, 0, 0) cRsmItem_2EF3B0[25];
    cAirStreamerFX_readFromReplayFrame_F3B0(this);
}

// PORT: the compiler names this ctor __11cRsm_2EF3B0i; expose it under the address-derived name.
__asm__(".globl cAirStreamerFX_cAirStreamerFX\n.type cAirStreamerFX_cAirStreamerFX,@function\ncAirStreamerFX_cAirStreamerFX = __11cRsm_2EF3B0i\n.size cAirStreamerFX_cAirStreamerFX,0x180\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("__12cRiderVb_rsm = cRider_cRider\n"
        "_vt$11cRsm_2EF3B0$13cRiderVbC_rsm = D_004888D0\n"
        "_vt$11cRsm_2EF3B0$14cRiderVbBC_rsm = D_00459B90\n"
        "_vt$11cRsm_2EF3B0$12cRiderVb_rsm = D_00488918\n");
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX__cAirStreamerFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/renderstateman_classes.h"

/**
 * Destroys an air streamer: frees its segment ring, then tears down per the g++ destructor flags.
 *
 * Restores this class's vtables in the virtual cRider base and frees the ring (+0x4) if set; bit 1 of the
 * hidden flags then destroys the cRider base (cRider__cRider) and bit 0 frees the object.
 */
cVbDtor002EF530::~cVbDtor002EF530()
{
    if (m04 != 0) cMemMan_free(m04);
}

// PORT: the compiler names this dtor _$_15cVbDtor002EF530; expose it under the address-derived name.
__asm__(".globl cAirStreamerFX__cAirStreamerFX\n.type cAirStreamerFX__cAirStreamerFX,@function\ncAirStreamerFX__cAirStreamerFX = _$_15cVbDtor002EF530\n.size cAirStreamerFX__cAirStreamerFX,0x170\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002EF530$13cRiderVbC_rsm = D_004888D0\n"
        "_vt$15cVbDtor002EF530$14cRiderVbBC_rsm = D_00459B90\n"
        "_vt$15cVbDtor002EF530$12cRiderVb_rsm = D_00488918\n"
        "_$_12cRiderVb_rsm = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Resets the air streamer: empties the trail and picks its maximum length.
 *
 * Clears the head, count and texture scroll (+0x8, +0xC, +0x10) and sets the length (+0x14) to 25 segments
 * for a rider with a view (mViewIndex >= 0), otherwise 18. Despite the name it reads nothing from a replay
 * frame.
 */
extern "C" void cAirStreamerFX_readFromReplayFrame(cAirStreamerFX* self)
{
    int mode = (*(int*)(*(char**)self + 0x870) >= 0) ? 0x19 : 0x12;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x14) = mode;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_update);
#ifdef SKIP_ASM
int func_0011FE98(void*);
extern "C" int func_001298C8();

struct sRsVec4_EF6D0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRsSeg_EF6D0 {
    sRsVec4_EF6D0 p0;
    sRsVec4_EF6D0 p1;
};

struct sTrail_EF6D0 {
    char* rider;                // 0x0
    sRsSeg_EF6D0* ring;         // 0x4
    int head;                   // 0x8
    int count;                  // 0xC
    float start;                // 0x10
    int len;                    // 0x14
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sRsVec4_EF6D0 Scale_EF6D0(const sRsVec4_EF6D0& v, float s)
{
    sRsVec4_EF6D0 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sRsVec4_EF6D0 Add_EF6D0(const sRsVec4_EF6D0& a, const sRsVec4_EF6D0& b)
{
    sRsVec4_EF6D0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sRsVec4_EF6D0 Sub_EF6D0(const sRsVec4_EF6D0& a, const sRsVec4_EF6D0& b)
{
    sRsVec4_EF6D0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_EF6D0(const sRsVec4_EF6D0& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

static inline int Approach_EF6D0(int n, int step)
{
    return n > step ? n - step : (n > -step - 1 ? 0 : n + step);
}

/**
 * Grows the air streamer's trail while the rider is airborne, shrinks it otherwise, and scrolls its texture.
 *
 * In the air (MS_Air) the head segment gets two points 90 units either side of the board bone
 * (mBoneBoardChild) along its axis, minus 0.7 x the hips position; on even AI ticks the head moves on around
 * the 25-entry ring and the count grows up to the length. Otherwise the count drops by 2 per frame. The
 * texture offset moves by the rider's speed / 6000 and wraps into 0..1.
 */
extern "C" void cAirStreamerFX_update(sTrail_EF6D0* self)
{
    if (func_0011FE98(self->rider) == 1) {
        char* riderPtr = self->rider;
        sRsVec4_EF6D0 bodyOff = *(sRsVec4_EF6D0*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x30) + (*(int*)(riderPtr + 0x89C) << 6) + 0x30);
        sRsVec4_EF6D0 boardOff = *(sRsVec4_EF6D0*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x30) + (*(int*)(riderPtr + 0x8A4) << 6) + 0x30);
        sRsVec4_EF6D0 boardDir = *(sRsVec4_EF6D0*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x34) + (*(int*)(riderPtr + 0x8A4) << 6));
        sRsVec4_EF6D0 streamBase = Sub_EF6D0(boardOff, Scale_EF6D0(bodyOff, 0.699999988079071f));
        self->ring[self->head].p0 = Add_EF6D0(streamBase, Scale_EF6D0(boardDir, 90.0f));
        self->ring[self->head].p1 = Sub_EF6D0(streamBase, Scale_EF6D0(boardDir, 90.0f));
        if ((func_001298C8() & 1) == 0) {
            int old = self->head;
            self->head = old + 1;
            self->head %= 25;
            if (self->count < self->len)
                self->count++;
            self->ring[self->head] = self->ring[old];
        }
    } else {
        int n = self->count;
        self->count = Approach_EF6D0(n, 2);
    }
    self->start -= Length_EF6D0(*(sRsVec4_EF6D0*)(self->rider + 0x1E0)) * 0.00016666666488163173f;
    if (self->start < 0.0f)
        self->start += 1.0f;
    if (self->start > 1.0f)
        self->start -= 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_render);
#ifdef SKIP_ASM
struct sRsVec4_EF950 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRsColor_EF950 {
    float x, y, z, w;
    sRsColor_EF950() {}
    sRsColor_EF950(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
};

struct sRsVertUV_EF950 {
    float x, y, z, w;
    sRsVertUV_EF950() {}
} __attribute__((aligned(16)));

struct sRsVert_EF950 {
    sRsVertUV_EF950 uv;     // 0x00
    int r, g, b, a;         // 0x10
    sRsVec4_EF950 pos;      // 0x20
};

struct sRsState_EF950 {
    int f0;                 // 0x0
    int flagsA;             // 0x4
    int flagsB;             // 0x8
    int fC;                 // 0xC
    short tex;              // 0x10
    short pad;
};

struct sRsVEnt_EF950 {
    short delta;
    short index;
    void (*fn)(void*, int, sRsVert_EF950*, int);
};

struct sRsCtx_EF950 {
    char pad0[0xE84];
    sRsState_EF950* top;        // 0xE84
    char pad1[0x1044 - 0xE88];
    int tex1044;                // 0x1044
    int pad1048;
    int tex104C;                // 0x104C
    char pad2[0x10D8 - 0x1050];
    sRsVEnt_EF950* vtable;      // 0x10D8
};

struct sRsSeg_EF950 {
    sRsVec4_EF950 p0;
    sRsVec4_EF950 p1;
};

struct sTrail_EF950 {
    char* rider;                // 0x0
    sRsSeg_EF950* ring;         // 0x4
    int head;                   // 0x8
    int count;                  // 0xC
    float start;                // 0x10
    int len;                    // 0x14
};

extern sRsCtx_EF950* GrMan_renderstateman __asm__("GrMan");
extern sRsState_EF950 D_00501420[];
extern int cPowerUpFXToggles_m_TestTrickBoost;
extern float D_004A3B74;

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sRsVec4_EF950 rsScale_EF950(const sRsVec4_EF950& v, float s)
{
    sRsVec4_EF950 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sRsVec4_EF950 rsAdd_EF950(const sRsVec4_EF950& a, const sRsVec4_EF950& b)
{
    sRsVec4_EF950 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sRsVec4_EF950 rsSub_EF950(const sRsVec4_EF950& a, const sRsVec4_EF950& b)
{
    sRsVec4_EF950 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline int rsTex1044_EF950(sRsCtx_EF950* ctx)
{
    return ctx->tex1044;
}

static inline int rsTex104C_EF950(sRsCtx_EF950* ctx)
{
    return ctx->tex104C;
}

static inline void rsPush_EF950(sRsCtx_EF950* ctx)
{
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}

static inline void rsPop_EF950(sRsCtx_EF950* ctx)
{
    ctx->top--;
}

static inline void rsSetA_EF950(sRsCtx_EF950* ctx, int mask, int shift, int v)
{
    ctx->top->flagsA = (ctx->top->flagsA & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB_EF950(sRsCtx_EF950* ctx, int mask, int shift, int v)
{
    ctx->top->flagsB = (ctx->top->flagsB & ~mask) | ((v << shift) & mask);
}

static inline sRsVec4_EF950 rsRow_EF950(char* base, int idx, int off)
{
    return *(sRsVec4_EF950*)(base + (idx << 6) + off);
}

static inline void rsVert_EF950(sRsVert_EF950* v, float u, float w, const sRsColor_EF950& c, const sRsVec4_EF950& p)
{
    v->r = (int)(c.y * 255.0f);
    v->g = (int)(c.z * 255.0f);
    v->b = (int)(c.w * 255.0f);
    v->a = (int)(c.x * 128.0f);
    v->uv.z = 1.0f;
    v->pos = p;
    v->uv.x = u;
    v->uv.y = w;
}

/**
 * Draws the air streamer as two textured triangle strips that fade toward the tail.
 *
 * Needs at least two segments. Normally 15 units wide at alpha 0.3 with GrMan _EffectTexMap[63]; while a
 * trick boost runs (mPowerUpTrickTimer > 0 or the TestTrickBoost toggle) it glows: 37.5 wide, alpha 1, tinted
 * D_004A3B74, _EffectTexMap[61] scrolled by the trail offset. Segment alpha falls off as 1 - t^2/(len-1)^2,
 * and the segments are placed back at 0.7 x the current hips position.
 */
extern "C" void cAirStreamerFX_render(sTrail_EF950* self)
{
    if (self->count < 2) {
        return;
    }
    rsPush_EF950(GrMan_renderstateman);
    *GrMan_renderstateman->top = D_00501420[0];
    int glow = 0;
    float alpha = 0.3f;
    float start = 0.0f;
    if (cPowerUpFXToggles_m_TestTrickBoost != 0 || 0.0f < *(float*)(self->rider + 0x2EC)) {
        glow = 1;
        alpha = 1.0f;
        *(short*)((char*)GrMan_renderstateman->top + 0x10) = rsTex1044_EF950(GrMan_renderstateman);
        GrMan_renderstateman->top->f0 &= ~0xC;
        start = self->start;
    } else {
        sRsCtx_EF950* renderCtx = GrMan_renderstateman;
        renderCtx->top->f0 |= 0xC;
        *(short*)((char*)renderCtx->top + 0x10) = rsTex104C_EF950(renderCtx);
    }
    sRsCtx_EF950* ctx = GrMan_renderstateman;
    rsSetA_EF950(ctx, 0x400000, 22, 1);
    rsSetA_EF950(ctx, 0x1800000, 23, 0);
    rsSetA_EF950(ctx, 0x300000, 20, 0);
    rsSetA_EF950(ctx, 0xFF000, 12, 0x14);
    rsSetA_EF950(ctx, 0x3, 0, 0);
    rsSetB_EF950(ctx, 0x1FFFFC00, 10, 0);
    rsSetB_EF950(ctx, 0x3E0, 5, 8);
    rsSetA_EF950(ctx, 0x7C, 2, 7);
    sRsVert_EF950 vertsL[52];
    sRsVert_EF950 vertsR[52];
    char* rider = self->rider;
    float uvy = start;
    float invSq = 1.0f / (float)((self->len - 1) * (self->len - 1));
    sRsVec4_EF950 off = rsScale_EF950(rsRow_EF950(*(char**)(*(char**)(rider + 0x780) + 0x30), *(int*)(rider + 0x89C), 0x30), 0.7f);
    float dv = 1.0f / (float)self->count;
    float falloffT = 0.0f;
    int nverts = 0;
    sRsColor_EF950 color;
    color.x = 1.0f;
    color.y = 1.0f;
    color.z = 1.0f;
    color.w = 1.0f;
    sRsVec4_EF950 A;
    sRsVec4_EF950 B;
    rider = self->rider;
    sRsVec4_EF950 col = rsRow_EF950(*(char**)(*(char**)(rider + 0x780) + 0x34), *(int*)(rider + 0x8A4), 0x20);
    float size = 15.0f;
    if (glow) {
        float k = D_004A3B74;
        color = sRsColor_EF950(1.0f, k, k, k);
        size = 37.5f;
    }
    sRsVec4_EF950 half = rsScale_EF950(col, size * 0.5f);
    int j = self->head;
    int k = 0;
    for (int i = 0; i < self->count; i++) {
        if (j < 0) {
            j = 24;
        }
        color.x = alpha * (1.0f - falloffT * falloffT * invSq);
        if (color.x < 0.0f) {
            color.x = 0.0f;
        }
        A = rsAdd_EF950(self->ring[j].p0, off);
        B = rsAdd_EF950(self->ring[j].p1, off);
        rsVert_EF950(&vertsL[k], 0.0f, uvy, color, rsSub_EF950(A, half));
        rsVert_EF950(&vertsR[k], 0.0f, uvy, color, rsSub_EF950(B, half));
        k++;
        rsVert_EF950(&vertsL[k], 1.0f, uvy, color, rsAdd_EF950(A, half));
        rsVert_EF950(&vertsR[k], 1.0f, uvy, color, rsAdd_EF950(B, half));
        k++;
        uvy += dv;
        nverts += 2;
        j--;
        falloffT += 1.0f;
    }
    sRsCtx_EF950* o = GrMan_renderstateman;
    o->vtable[71].fn((char*)o + o->vtable[71].delta, nverts, vertsL, 0);
    o = GrMan_renderstateman;
    o->vtable[71].fn((char*)o + o->vtable[71].delta, nverts, vertsR, 0);
    rsPop_EF950(GrMan_renderstateman);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002EFF90);
#ifdef SKIP_ASM
extern void* D_004A3B78;

extern "C" int func_002EFF90(void) {
    return (*(int*)&D_004A3B78);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cVisualEffectsMan_cVisualEffectsMan);
#ifdef SKIP_ASM
struct sRSEntry_002EFF98 {
    int f0;
    int f4;
    int f8;
    int pad[5];
};
struct sRSMan_002EFF98 {
    int f[17];
    void* vt;
};
extern char cVisualEffectsMan_vtbl[];
extern sRSEntry_002EFF98* cGraphicsMan_TheGraphicsMan_renderstateman __asm__("cGraphicsMan_TheGraphicsMan");
struct sShadowCfg_002EFF98 {
    int texIndex;
};
extern sShadowCfg_002EFF98 D_004A45D4;
extern int D_004A443C;
extern void* D_004A3B78;

/**
 * Constructs the visual-effects manager base: empty effect slots, default effect textures, global pointer.
 *
 * Sets the vtable (+0x44), clears the 17 effect-object slots, stores 0x37 in D_004A45D4 and 0x30 in
 * D_004A443C and makes itself the manager D_004A3B78. On the graphics manager it sets view 6's specular,
 * patch specular, snow sparkle and rim texture ids (0x2E, 0x31, 0x44, 0x2F) and zeroes views 6 and 7 of the
 * per-view arrays m_isInNIS through m_ScreenTintOffsetB.
 *
 * @return self
 */
extern "C" void* cVisualEffectsMan_cVisualEffectsMan(sRSMan_002EFF98* self)
{
    self->vt = cVisualEffectsMan_vtbl;
    self->f[0] = 0;
    self->f[1] = 0;
    self->f[2] = 0;
    self->f[3] = 0;
    self->f[4] = 0;
    self->f[5] = 0;
    self->f[6] = 0;
    self->f[7] = 0;
    self->f[8] = 0;
    self->f[9] = 0;
    self->f[10] = 0;
    self->f[11] = 0;
    self->f[12] = 0;
    self->f[13] = 0;
    self->f[14] = 0;
    self->f[15] = 0;
    self->f[16] = 0;
    D_004A45D4.texIndex = 0x37;
    D_004A443C = 0x30;
    D_004A3B78 = self;
    sRSEntry_002EFF98* rsEntries = cGraphicsMan_TheGraphicsMan_renderstateman;
    rsEntries[1].f4 = 0x2E;
    rsEntries[2].f4 = 0x31;
    rsEntries[4].f4 = 0x2F;
    rsEntries[3].f4 = 0x44;
    rsEntries[5].f4 = 0; rsEntries[5].f8 = 0;
    rsEntries[6].f4 = 0; rsEntries[6].f8 = 0;
    rsEntries[7].f4 = 0; rsEntries[7].f8 = 0;
    rsEntries[8].f4 = 0; rsEntries[8].f8 = 0;
    rsEntries[9].f4 = 0; rsEntries[9].f8 = 0;
    rsEntries[10].f4 = 0; rsEntries[10].f8 = 0;
    rsEntries[11].f4 = 0; rsEntries[11].f8 = 0;
    rsEntries[12].f4 = 0; rsEntries[12].f8 = 0;
    rsEntries[13].f4 = 0; rsEntries[13].f8 = 0;
    rsEntries[14].f4 = 0; rsEntries[14].f8 = 0;
    rsEntries[15].f4 = 0; rsEntries[15].f8 = 0;
    rsEntries[16].f4 = 0; rsEntries[16].f8 = 0;
    rsEntries[17].f4 = 0; rsEntries[17].f8 = 0;
    rsEntries[18].f4 = 0; rsEntries[18].f8 = 0;
    rsEntries[19].f4 = 0; rsEntries[19].f8 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F00A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" float func_002EE3B8(int i);
extern "C" float func_002EE738(int i);
extern "C" float func_002EE9C0(int i);
extern "C" float func_002EEA08(int i);
extern "C" float func_002EEA50(int i);
extern "C" float func_002EEA98(int i);
extern "C" float func_002EEAE0(int i);
extern "C" float func_002EEB28(int i);
extern "C" float func_002EEB70(int i);
extern "C" float func_002EEBD0(int i);
extern "C" float func_002EEC30(int i);
extern "C" float func_002EEC90(int i);
extern "C" float func_002EECF0(int i);
extern "C" float func_002EED50(int i);
extern "C" float func_002EEDB0(int i);
extern "C" float func_002EEDF8(int i);
extern "C" float func_002EEE40(int i);
extern "C" float func_002EEE88(int i);
extern "C" float func_002EEED0(int i);
extern "C" float func_002EEF18(int i);
extern "C" float func_002EEF60(int i);
extern "C" float func_002EF038(int i);
extern "C" int func_002F7BE0(void* p);
extern void* D_004A3B5C;
extern int D_004A4328;
extern int cDepthFog_m_Exponent;
extern float cDepthFog_m_Density;
extern float cDepthFog_m_NearPlane;
extern float cDepthFog_m_FarPlane;
extern int cScreenTint_m_OverrideWP;
extern float cScreenTint_m_ColourR;
extern float cScreenTint_m_ColourG;
extern float cScreenTint_m_ColourB;
extern float cScreenTint_m_FillR;
extern float cScreenTint_m_FillG;
extern float cScreenTint_m_FillB;
extern float D_004A4614;
extern int D_004A460C;
extern int D_004A43CC;
extern float D_004A43D4;
extern float D_004A43D8;
extern float D_004A43DC;
extern float D_004A43E0;
extern float D_004A43E4;
extern float D_004A43F0;
extern float D_004A43F4;

struct sRsCol_F00A0 {
    float r, g, b, a;
    sRsCol_F00A0(const float& x, const float& y, const float& z, const float& w) : r(x), g(y), b(z), a(w) {}
};
extern sRsCol_F00A0 D_00504720;

struct sRsCtx_F00A0 {
    char pad0[0x8C];
    void* a8C[8];       // 0x8C
    float aAC[8];       // 0xAC
    float aCC[8];       // 0xCC
    float aEC[8];       // 0xEC
    float a10C[8];      // 0x10C
    float a12C[8];      // 0x12C
    float a14C[8];      // 0x14C
    float a16C[8];      // 0x16C
    float a18C[8];      // 0x18C
    float a1AC[8];      // 0x1AC
    float a1CC[8];      // 0x1CC
    float a1EC[8];      // 0x1EC
    float a20C[8];      // 0x20C
    float a22C[8];      // 0x22C
    float a24C[8];      // 0x24C
};
extern sRsCtx_F00A0* D_ctx_F00A0 __asm__("cGraphicsMan_TheGraphicsMan");

extern "C" void func_002F00A0(void* self, int i)
{
    sRsCtx_F00A0* ctx = D_ctx_F00A0;
    if (D_004A4328 == 0) {
        sRsCol_F00A0* dst = &D_00504720;
        cDepthFog_m_Density = func_002EE9C0(i);
        cDepthFog_m_Exponent = 1;
        cDepthFog_m_NearPlane = func_002EEA08(i);
        cDepthFog_m_FarPlane = func_002EEA50(i);
        float g = func_002EEA98(i);
        float b = func_002EEAE0(i);
        float a = func_002EEB28(i);
        *dst = sRsCol_F00A0(1.0f, g, b, a);
    }
    if (cScreenTint_m_OverrideWP == 0) {
        cScreenTint_m_ColourR = func_002EEB70(i);
        cScreenTint_m_ColourG = func_002EEBD0(i);
        cScreenTint_m_ColourB = func_002EEC30(i);
        cScreenTint_m_FillR = func_002EEC90(i);
        cScreenTint_m_FillG = func_002EECF0(i);
        cScreenTint_m_FillB = func_002EED50(i);
        D_004A4614 = func_002EE738(i);
        int on = 0;
        int* st = *(int**)(*(char**)((char*)SSXApp + 0x84) + 0x28);
        if (*st == 1 || func_002F7BE0(st))
            on = 1;
        D_004A460C = on;
    }
    if (D_004A43CC == 0) {
        D_004A43D4 = func_002EEDB0(i);
        D_004A43D8 = func_002EEDF8(i);
        D_004A43DC = func_002EEE40(i);
        D_004A43E0 = func_002EEE88(i);
        D_004A43E4 = func_002EEED0(i);
        D_004A43F0 = func_002EEF18(i);
        D_004A43F4 = func_002EEF60(i);
    }
    ctx->a8C[i] = D_004A3B5C;
    ctx->aAC[i] = func_002EF038(i);
    ctx->aCC[i] = func_002EE3B8(i);
    ctx->aEC[i] = func_002EE9C0(i);
    ctx->a10C[i] = func_002EEA98(i);
    ctx->a12C[i] = func_002EEAE0(i);
    ctx->a14C[i] = func_002EEB28(i);
    ctx->a16C[i] = func_002EEA08(i);
    ctx->a18C[i] = func_002EEA50(i);
    ctx->a1AC[i] = func_002EEB70(i);
    ctx->a1CC[i] = func_002EEBD0(i);
    ctx->a1EC[i] = func_002EEC30(i);
    ctx->a20C[i] = func_002EEC90(i);
    ctx->a22C[i] = func_002EECF0(i);
    ctx->a24C[i] = func_002EED50(i);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F0360);
#ifdef SKIP_ASM
extern void* D_004A3B80;

extern "C" int func_002F0360(void) {
    return (*(int*)&D_004A3B80);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F0368);
#ifdef SKIP_ASM
extern void* D_00487D78[];
extern void* D_004A3B80;

extern "C" void* func_002F0368(void* self)
{
    *(int*)self = 0;
    *(void***)((char*)self + 0x4) = D_00487D78;
    D_004A3B80 = self;
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F0388);
#ifdef SKIP_ASM
extern "C" void func_002F0388(void) {

}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F0390);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002F03C8(void* self);
extern char str_PS2AIVisualEffectsMan[];

extern "C" void* func_002F0390(void)
{
    return func_002F03C8(cMemMan_alloc(8, str_PS2AIVisualEffectsMan, 0, 0));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/renderstateman", func_002F03C8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002F0368(void* self);
extern "C" void cAvalanche_init(void* self);
extern char str_Avalanche[];
extern void* D_00487D50[];
extern void* cAvalanche_vtbl[];

extern "C" void* func_002F03C8(void* self)
{
    func_002F0368(self);
    *(void***)((char*)self + 0x4) = D_00487D50;
    void** p = (void**)cMemMan_alloc(4, str_Avalanche, 0, 0);
    *p = cAvalanche_vtbl;
    cAvalanche_init(p);
    *(void***)self = p;
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F0438);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_00487D50[];
extern void* D_00487D78[];

class func_002F0438_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(int);
};

extern "C" void func_002F0438(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_00487D50;
    func_002F0438_cVirt* o = *(func_002F0438_cVirt**)self;
    if (o != 0) {
        o->v01(3);
    }
    *(void***)((char*)self + 0x4) = D_00487D78;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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
INCLUDE_ASM("visualfx/renderstateman", cVisualEffectsMan_construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPSPVisualEffectsMan_cPSPVisualEffectsMan(void* self);
extern char str_PS2VisualEffectsMan[];

/**
 * Creates the visual-effects manager: allocates 0x48 bytes tagged "PS2VisualEffectsMan" and constructs a
 * cPSPVisualEffectsMan there.
 *
 * The base constructor records it in D_004A3B78; this function stores it nowhere itself.
 *
 * @return the new manager
 */
extern "C" void* cVisualEffectsMan_construct(void)
{
    return cPSPVisualEffectsMan_cPSPVisualEffectsMan(cMemMan_alloc(0x48, str_PS2VisualEffectsMan, 0, 0));
}
#endif


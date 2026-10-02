#include "common.h"

INCLUDE_ASM("object/splinemodifier", cSplineModifier_cSplineModifier);

INCLUDE_ASM("object/splinemodifier", func_00359688);

INCLUDE_ASM("object/splinemodifier", func_00359698);

INCLUDE_ASM("object/splinemodifier", func_00359830);

INCLUDE_ASM("object/splinemodifier", func_00359CF8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/splinemodifier", func_00359EB8);
#ifdef SKIP_ASM
extern "C" void func_00359688(void* self);

extern "C" void func_00359EB8(void* self, int msg)
{
    if (msg == 0x190) {
        func_00359688(self);
        *(int*)((char*)self + 0x50) = 0;
        *(int*)((char*)self + 0x54) = 1;
    } else if (msg == 0x191) {
        *(int*)((char*)self + 0x50) = 0;
        *(int*)((char*)self + 0x54) = 1;
    } else if (msg == 0x192) {
        *(float*)((char*)self + 0x48) = -*(float*)((char*)self + 0x48);
        *(int*)((char*)self + 0x50) = 0;
        *(int*)((char*)self + 0x54) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_00359F30);
#ifdef SKIP_ASM
class cStream00359F30 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void func_00345538(void* self, cStream00359F30* stream);

extern "C" void func_00359F30(void* self, cStream00359F30* stream)
{
    stream->v01((char*)self + 0x10, 0x50);
    func_00345538((char*)self + 0xD8, stream);
}
#endif

INCLUDE_ASM("object/splinemodifier", func_00359F88);

INCLUDE_ASM("object/splinemodifier", func_0035A118);

INCLUDE_ASM("object/splinemodifier", func_0035A250);

INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_allocNodes);

//100%
INCLUDE_ASM("object/splinemodifier", func_0035A3F0);
#ifdef SKIP_ASM
void* cObjectInterface_getInstanceMan();
extern "C" void* func_00351170(void* man);

extern "C" void func_0035A3F0(void* self)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x4); i++) {
        (*(void***)((char*)self + 0x44))[i] = func_00351170(cObjectInterface_getInstanceMan());
    }
}
#endif

INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_setupNodes);

INCLUDE_ASM("object/splinemodifier", func_0035A550);

//100%
INCLUDE_ASM("object/splinemodifier", func_0035A560);
#ifdef SKIP_ASM
extern "C" float func_002D1C70();

extern "C" void func_0035A560(void* self)
{
    float t = *(float*)((char*)self + 0x10) + *(float*)((char*)self + 0x14) * func_002D1C70();
    *(float*)((char*)self + 0x10) = t;
    if (t < 0.0f) {
        *(float*)((char*)self + 0x10) = t + *(float*)((char*)self + 0x54);
    } else if (t >= *(float*)((char*)self + 0x54)) {
        *(float*)((char*)self + 0x10) = t - *(float*)((char*)self + 0x54);
    }
    *(int*)((char*)self + 0x30) = 1;
}
#endif

extern "C" void* func_0035AC20(void* self);

//100%
INCLUDE_ASM("object/splinemodifier", func_0035A5D8__FPv);
#ifdef SKIP_ASM
void* func_0035A5D8(void* self)
{
    return func_0035AC20(self);
}
#endif

INCLUDE_ASM("object/splinemodifier", cMultiSplineModifier_setupOverlapSystem);

INCLUDE_ASM("object/splinemodifier", func_0035A780);

INCLUDE_ASM("object/splinemodifier", func_0035A918);

//100%
INCLUDE_ASM("object/splinemodifier", func_0035AAD0__FPv);
#ifdef SKIP_ASM
void* func_0035AAD0(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x34) + 0x1;
    *(int*)((char*)self + 0x34) = (int)t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_0035AAE0__FPv);
#ifdef SKIP_ASM
void* func_0035AAE0(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x34) - 0x1;
    *(int*)((char*)self + 0x34) = (int)t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_0035AAF0);
#ifdef SKIP_ASM
struct sSmVEntryV_AAF0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sSmVEntryP_AAF0 {
    short delta;
    short index;
    void* (*fn)(void*);
};

extern "C" void* func_0035AAF0(void* self, void* item)
{
    if (*(int*)((char*)self + 0x30) != 0) {
        sSmVEntryV_AAF0* vt = *(sSmVEntryV_AAF0**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    if (item == *(void**)((char*)self + 0x40)) {
        void* obj = *(void**)((char*)item + 0xC);
        sSmVEntryP_AAF0* vt2 = *(sSmVEntryP_AAF0**)((char*)obj + 0xC);
        return vt2[24].fn((char*)obj + vt2[24].delta);
    }
    return (char*)item + 0x10;
}
#endif

INCLUDE_ASM("object/splinemodifier", func_0035AB60);

//100%
INCLUDE_ASM("object/splinemodifier", func_0035ABF0);
#ifdef SKIP_ASM
class cSplineVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
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
    virtual int v51();
};

extern "C" int func_0035ABF0(void* self)
{
    cSplineVirt* o = *(cSplineVirt**)((char*)*(void**)((char*)self + 0x40) + 0xC);
    return o->v51();
}
#endif

INCLUDE_ASM("object/splinemodifier", func_0035AC20);

INCLUDE_ASM("object/splinemodifier", func_0035B200);

INCLUDE_ASM("object/splinemodifier", func_0035B418);

INCLUDE_ASM("object/splinemodifier", func_0035B5A8);

//100%
INCLUDE_ASM("object/splinemodifier", func_0035B670);
#ifdef SKIP_ASM
extern "C" void func_0035B670(void* self, void* node)
{
    void* h = *(void**)self;
    if (h != 0) {
        *(void**)((char*)h + 0x4) = node;
    }
    *(void**)((char*)node + 0x4) = self;
    *(void**)node = *(void**)self;
    *(void**)self = node;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_0035B690);
#ifdef SKIP_ASM
struct sSplineLink {
    sSplineLink* next; // 0x0
    sSplineLink* prev; // 0x4
};

extern "C" void func_0035B690(sSplineLink* link, sSplineLink* other)
{
    while (link->next != 0) {
        link = link->next;
    }
    other->next = 0;
    other->prev = link;
    link->next = other;
}
#endif

//100%
INCLUDE_ASM("object/splinemodifier", func_0035B6D0);
#ifdef SKIP_ASM
extern "C" void func_0035B6D0(sSplineLink* link)
{
    if (link->next != 0) {
        link->next->prev = link->prev;
    }
    if (link->prev != 0) {
        link->prev->next = link->next;
    }
    link->next = 0;
    link->prev = 0;
}
#endif

INCLUDE_ASM("object/splinemodifier", func_0035B708);

INCLUDE_ASM("object/splinemodifier", func_0035BA88);

INCLUDE_ASM("object/splinemodifier", func_0035BD70);

INCLUDE_ASM("object/splinemodifier", func_0035C040);


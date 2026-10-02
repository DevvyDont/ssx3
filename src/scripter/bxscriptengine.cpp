#include "common.h"

INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetupBXEngine);

INCLUDE_ASM("scripter/bxscriptengine", func_00282020);

INCLUDE_ASM("scripter/bxscriptengine", func_002820B0);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282150);
#ifdef SKIP_ASM
struct sSlot282150 { int value; int pad; };
extern "C" int func_00282150(void* self, int value, int index)
{
    sSlot282150* slots = *(sSlot282150**)((char*)self + 0x2b0);
    if (slots[index].value != 0) {
        return 0;
    }
    slots[index].value = value;
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282178);
#ifdef SKIP_ASM
extern "C" int func_00282178(void* self, int index)
{
    sSlot282150* slots = *(sSlot282150**)((char*)self + 0x2b0);
    int old = slots[index].value;
    if (old == 0) {
        return 0;
    }
    slots[index].value = 0;
    return old;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_002821A0);

INCLUDE_ASM("scripter/bxscriptengine", func_002822A0);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282338);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);
extern "C" void func_00272B58(void* cache, void* data);

extern "C" void func_00282338(void* self, int a1)
{
    char* e = (char*)func_00282CB0(self, a1);
    char* obj = *(char**)e;
    if (obj != 0 && *(int*)(obj + 0x20) != 0 && *(int*)(e + 0x20) == 1) {
        func_00272B58(*(void**)((char*)self + 0x2B8), *(void**)obj);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", func_00282390);
#ifdef SKIP_ASM
extern "C" void func_00282338(void* self, int id);

struct sScriptSlot282390 {
    int active;
    char pad_0x04[0x18];
    int id;
    int pad_0x20;
};

struct sScriptEngine282390 {
    char pad_0x000[0x2C0];
    sScriptSlot282390 slots[16];
};

extern "C" void func_00282390(sScriptEngine282390* self)
{
    for (int i = 0; i < 16; i++) {
        sScriptSlot282390* s = &self->slots[i];
        if (s->active != 0) {
            func_00282338(self, s->id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002823F0);
#ifdef SKIP_ASM
extern "C" void func_00282540(void* self, int id);

struct sScriptSlot2823F0 {
    int active;
    char pad_0x04[0x18];
    int id;
    int state;
};

struct sScriptEngine2823F0 {
    char pad_0x000[0x2C0];
    sScriptSlot2823F0 slots[16];
};

extern "C" void func_002823F0(sScriptEngine2823F0* self)
{
    for (int i = 0; i < 16; i++) {
        sScriptSlot2823F0* s = &self->slots[i];
        if (s->state != 0) {
            func_00282540(self, s->id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282450);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);
extern "C" void func_00274A30(void* p);

extern "C" int func_00282450(void* self, int a1)
{
    void* t = func_00282CB0(self, a1);
    if (t == 0) {
        return 0;
    }
    int type = *(int*)((char*)t + 0x4);
    if (type == 3 || type == 4) {
        func_00274A30(*(void**)t);
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282540);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002826D8);
#ifdef SKIP_ASM
extern "C" void func_002749E8(void* self);

extern "C" void func_002826D8(void* self, int a1)
{
    char* p = (char*)self + (a1 * 0x24 + 0x2c0);
    func_002749E8(*(void**)p);
    if (*(int*)(*(char**)p + 0x2C) > 0) {
        *(int*)(p + 0x4) = 5;
    }
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282720);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282798);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);
extern "C" void func_00282908(void* self, void* script);

extern "C" int func_00282798(void* self, int a1)
{
    char* s = (char*)func_00282CB0(self, a1);
    if (s == 0) {
        return 0;
    }
    if (*(int*)(s + 0x4) == 1) {
        func_00282908(self, s);
    }
    return *(int*)(s + 0x4);
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282838);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002828B8);
#ifdef SKIP_ASM
struct sScriptThread2C0 {
    int a;          // 0x00
    int b;          // 0x04
    int c;          // 0x08
    int d;          // 0x0C
    short e;        // 0x10
    short f;        // 0x12
    int g;          // 0x14
    int h;          // 0x18
    int id;         // 0x1C
    int k;          // 0x20
};
struct sScriptEngine2B8 {
    char pad[0x2b8];
    int count;                      // 0x2B8
    int unk2BC;                     // 0x2BC
    sScriptThread2C0 threads[16];   // 0x2C0
};
extern "C" void func_002828B8(sScriptEngine2B8* self)
{
    int i;
    self->count = 0;
    self->unk2BC = 0;
    for (i = 0; i < 16; i++) {
        self->threads[i].id = i;
        self->threads[i].k = 0;
        self->threads[i].a = 0;
        self->threads[i].b = 0;
        self->threads[i].e = 0;
        self->threads[i].f = 0;
        self->threads[i].h = 0;
        self->threads[i].g = -1;
    }
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282908);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002829D0);
#ifdef SKIP_ASM
struct sBxsVEntry10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int, int);
};

extern "C" void func_002829D0(void* self, void* a1)
{
    char* t = *(char**)((char*)a1 + 0xC);
    sBxsVEntry10* vt = *(sBxsVEntry10**)((char*)self + 0x2A8);
    vt[10].fn((char*)self + vt[10].delta, a1, *(int*)(t + 0x8), *(int*)(t + 0xC));
    *(int*)(t + 0x4) = 4;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282A18);
#ifdef SKIP_ASM
struct sVEntry282A18 {
    short delta;
    short index;
    void (*fn)(void*, void*, int, int);
};

extern "C" void func_00282540(void* self, int id);

extern "C" void func_00282A18(void* self, void* msg)
{
    char* s = *(char**)((char*)msg + 0xC);
    sVEntry282A18* vt = *(sVEntry282A18**)((char*)self + 0x2A8);
    vt[11].fn((char*)self + vt[11].delta, msg, *(int*)(s + 0x8), *(int*)(s + 0xC));
    *(int*)(s + 0x4) = 2;
    if (*(short*)(s + 0x10) != 0) {
        func_00282540(self, *(int*)(s + 0x1C));
    }
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282A80);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282B40);
#ifdef SKIP_ASM
struct sBxsThreadHdr {
    void* owner;    // 0x0
    int state;      // 0x4
    int handle;     // 0x8
};

extern "C" sBxsThreadHdr* func_00282C38(void* self, int a1);
// PORT: the setter's mangled signature is (void*, int); a pointer is passed as int.
int func_00274C10(void* self, int a1);

extern "C" void func_00282B40(void* self, void* obj)
{
    sBxsThreadHdr* t = func_00282C38(self, 1);
    t->state = 2;
    t->handle = -1;
    t->owner = obj;
    func_00274C10(obj, (int)t);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282B88);
#ifdef SKIP_ASM
void func_00282C88(void* self, void* v);

extern "C" void func_00282B88(void* self, void* ctx)
{
    void* v = *(void**)((char*)ctx + 0xC);
    *(int*)((char*)v + 0x4) = 7;
    func_00282C88(self, v);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BB0__FPvT0);
#ifdef SKIP_ASM
int func_00282BB0(void* self, void* a1)
{
    return *(int*)((char*)*(void**)((char*)a1 + 0xc) + 0xc);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BE0__FPvT0);
#ifdef SKIP_ASM
int func_00282BE0(void* self, void* a1)
{
    return *(int*)((char*)*(void**)((char*)a1 + 0xc) + 0x1c);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BF0);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);

extern "C" int func_00282BF0(void* self, int a1)
{
    int* p = (int*)func_00282CB0(self, a1);
    if (p == 0) {
        return 0;
    }
    return *p;
}
#endif

extern "C" void* func_00272CC0(int);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282C18__FPv);
#ifdef SKIP_ASM
void* func_00282C18(void* self)
{
    return func_00272CC0(*(int*)((char*)self + 0x2b8));
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282C38);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282C88__FPvT0);
#ifdef SKIP_ASM
void func_00282C88(void* self, void* a1)
{
    *(int*)((char*)a1 + 0x0) = 0;
    *(int*)((char*)a1 + 0x4) = 0;
    *(int*)((char*)a1 + 0x20) = 0;
    *(short*)((char*)a1 + 0x10) = 0;
    *(short*)((char*)a1 + 0x12) = 0;
    *(int*)((char*)a1 + 0x14) = -1;
    *(int*)((char*)a1 + 0x18) = 0;
    *(int*)((char*)a1 + 0xc) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282CB0);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1)
{
    char* p = (char*)self + (a1 * 0x24 + 0x2c0);
    return *(int*)(p + 0x20) != 0 ? p : 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282CD0);
#ifdef SKIP_ASM
void* func_00283200(void* self, int type);
extern void* D_00482060[];

extern "C" void* func_00282CD0(void* self)
{
    func_00283200(self, 2);
    *(void***)((char*)self + 0xC) = D_00482060;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0x14) = -1;
    *(int*)((char*)self + 0x18) = 9;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x28) = 0;
    return self;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282D30);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282DA8__FPvi);
#ifdef SKIP_ASM
void func_00282DA8(void* self, int val)
{
    *(int*)((char*)self + 0x10) = val;
}
#endif

extern "C" void* func_00282DD0(void*, int, int, int);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282DB0__FPvii);
#ifdef SKIP_ASM
void* func_00282DB0(void* self, int a1, int a2)
{
    return func_00282DD0(self, a1, a2, *(int*)((char*)self + 0x28));
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282DD0);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282EF0);
#ifdef SKIP_ASM
extern "C" int func_00282EF0(void* self, int a1)
{
    return *(int*)((char*)self + 0x14) == a1;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F00);
#ifdef SKIP_ASM
class cBxScriptVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    virtual void v01();
    virtual void v02();
    virtual void v03();
};

extern "C" void func_00282F00(void* self)
{
    (*(cBxScriptVirt**)((char*)self + 0x10))->v02();
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F30);
#ifdef SKIP_ASM
extern "C" void func_00282F30(void* self)
{
    (*(cBxScriptVirt**)((char*)self + 0x10))->v03();
}
#endif

extern void* D_00482418[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F60__FPv);
#ifdef SKIP_ASM
void* func_00282F60(void* self)
{
    *(void***)self = D_00482418;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F80__FPv);
#ifdef SKIP_ASM
int func_00282F80(void* self)
{
    return (*(int*)((char*)self + 0x8) != 0);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F90);
#ifdef SKIP_ASM
extern "C" void func_00274240(void* p, int a1);

extern "C" void func_00282F90(void* self)
{
    void* p = *(void**)((char*)self + 0x8);
    if (p != 0) {
        func_00274240(p, 0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282FB8);
#ifdef SKIP_ASM
extern "C" void* func_0027D2E8(void);
extern "C" void func_0027D400(void* mgr, void* self, int a2);

extern "C" void func_00282FB8(void* self, void* a1)
{
    *(void**)((char*)self + 0x8) = a1;
    func_0027D400(func_0027D2E8(), self, **(int**)((char*)a1 + 0x8));
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283000);
#ifdef SKIP_ASM
extern "C" void* func_0027D2E8(void);
extern "C" void func_0027D4A0(void* mgr, void* obj);

extern "C" void func_00283000(void* self)
{
    func_0027D4A0(func_0027D2E8(), self);
    *(int*)((char*)self + 0x8) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283038);
#ifdef SKIP_ASM
extern "C" void func_002749E8(void* self);

extern "C" void func_00283038(void* self)
{
    void* p = *(void**)((char*)self + 0x8);
    if (p != 0) {
        func_002749E8(*(void**)((char*)p + 0x8));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283060);
#ifdef SKIP_ASM
extern "C" void func_00274A08(void* self);

extern "C" void func_00283060(void* self)
{
    void* p = *(void**)((char*)self + 0x8);
    if (p != 0) {
        func_00274A08(*(void**)((char*)p + 0x8));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", func_00283088);
#ifdef SKIP_ASM
extern void* D_004823E0[];
void* func_00282F60(void* self);

extern "C" void* func_00283088(void* self)
{
    func_00282F60(self);
    *(void***)self = D_004823E0;
    *(int*)((char*)self + 0xC) = 9;
    *(int*)((char*)self + 0x10) = 0;
    return self;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_002830C8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", func_00283180);
#ifdef SKIP_ASM
extern "C" void func_00283000(void* self);

extern "C" void func_00283180(void* self)
{
    func_00283000(self);
    *(int*)((char*)self + 0xC) = 9;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_002831B0);

extern void* D_00482558[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283200__FPvi);
#ifdef SKIP_ASM
void* func_00283200(void* self, int a1)
{
    *(void***)((char*)self + 0xc) = D_00482558;
    *(int*)self = a1;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00283228);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283298);
#ifdef SKIP_ASM
struct sBxsVEntryA {
    short delta;
    short index;
    int (*fn)(void*, void*);
};

extern "C" void func_00283298(void* self)
{
    char* mgr = (char*)func_0027D2E8();
    sBxsVEntryA* vt = *(sBxsVEntryA**)(mgr + 0x2A8);
    *(int*)((char*)self + 0x4) = vt[2].fn(mgr + vt[2].delta, self);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002832D8);
#ifdef SKIP_ASM
struct sBxsVEntryB {
    short delta;
    short index;
    int (*fn)(void*, int);
};

extern "C" void func_002832D8(void* self)
{
    char* mgr = (char*)func_0027D2E8();
    sBxsVEntryB* vt = *(sBxsVEntryB**)(mgr + 0x2A8);
    vt[3].fn(mgr + vt[3].delta, *(int*)((char*)self + 0x4));
    *(int*)((char*)self + 0x4) = -1;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00283320);

INCLUDE_ASM("scripter/bxscriptengine", func_002833A8);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283430__FPv);
#ifdef SKIP_ASM
void* func_00283430(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x8) + 0x1;
    *(int*)((char*)self + 0x8) = (int)t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283440__FPv);
#ifdef SKIP_ASM
void* func_00283440(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x8) - 0x1;
    *(int*)((char*)self + 0x8) = (int)t0;
    return t0;
}
#endif


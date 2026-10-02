#include "common.h"

INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_cWScriptProcess);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307738);
#ifdef SKIP_ASM
class func_00307738_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01(int flags);
};

void cMemMan_free(void* p);
void operator_delete(int* p);
extern "C" void func_00224DF0(void* obj, int flags);
extern char D_00489B70[];

// PORT: array cookie arithmetic through int (pointer in int).
extern "C" void func_00307738(void* self, int flags)
{
    *(void**)((char*)self + 0x5C) = D_00489B70;
    char* arr = *(char**)((char*)self + 0x14);
    if (arr != 0) {
        char* p = (char*)(*(int*)(arr - 0x10) * 0x44 + (int)arr);
        while (*(char**)((char*)self + 0x14) != p) {
            p -= 0x44;
            ((func_00307738_cElem*)p)->v01(0);
        }
        cMemMan_free(*(char**)((char*)self + 0x14) - 0x10);
    }
    void* obj = *(void**)((char*)self + 0x1C);
    if (obj != 0) {
        func_00224DF0(obj, 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003077F8);
#ifdef SKIP_ASM
class func_003077F8_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01();
    virtual void v02();
};

class func_003077F8_cProc {
public:
    int state;
    int count;
    int cur;
    char pad0C[0x8];
    func_003077F8_cElem* elems;
    char pad18[0x44];
    // vptr at 0x5C
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

extern "C" void func_003077F8(func_003077F8_cProc* self)
{
    switch (self->state) {
    case 0:
        break;
    case 1:
        self->state = 2;
    case 2: {
        int st = self->state;
        if (st == 2) {
            if (self->cur < self->count) {
                self->elems[self->cur].v02();
            }
            if (self->state == st) {
                self->v05();
            }
        }
        break;
    }
    }
}
#endif

INCLUDE_ASM("wscript/wscriptprocess", func_003078C0);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003079D8);
#ifdef SKIP_ASM
struct cWScriptProcEntry {
    int a;
    unsigned int id;
    char pad08[0x3C];
};

struct cWScriptProcList {
    int pad00;
    int count;
    int cur;
    int pad0C;
    int pad10;
    cWScriptProcEntry* entries;
};

extern "C" unsigned int func_003079D8(cWScriptProcList* self)
{
    int i = self->cur;
    if (i < self->count) {
        return self->entries[i].id;
    }
    return 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307A10);
#ifdef SKIP_ASM
class func_00307A10_cObj {
public:
    char pad_0x00[0x40];
    // vptr at 0x40; sizeof == 0x44 (same layout as cWScriptProcEntry)
    virtual void v01();
    virtual void v02();
    virtual void v03(int a1); // 0x18
};

extern "C" void func_00307A10(cWScriptProcList* self, int a1)
{
    int i = self->cur;
    if (i < self->count) {
        ((func_00307A10_cObj*)self->entries)[i].v03(a1);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307A60);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307A60(void* self)
{
    void* script = *(void**)((char*)self + 0x34);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307A90);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307A90(void* self)
{
    void* script = *(void**)((char*)self + 0x38);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307AC0);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307AC0(void* self)
{
    void* script = *(void**)((char*)self + 0x3c);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307AF0);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307AF0(void* self)
{
    void* script = *(void**)((char*)self + 0x40);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307B20);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307B20(void* self)
{
    void* script = *(void**)((char*)self + 0x44);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307B50);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307B50(void* self)
{
    void* script = *(void**)((char*)self + 0x48);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307B80);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307B80(void* self)
{
    void* script = *(void**)((char*)self + 0x4c);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307BB0);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307BB0(void* self)
{
    void* script = *(void**)((char*)self + 0x50);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307BE0);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307BE0(void* self)
{
    void* script = *(void**)((char*)self + 0x54);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307C10);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307C10(void* self)
{
    void* script = *(void**)((char*)self + 0x58);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307C40);
#ifdef SKIP_ASM
class func_00307C40_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

struct func_00307C40_sProc {
    int state;
    int count;
    int cur;
    int pad0C;
    void* man;
    func_00307C40_cElem* elems;
    int pad18;
    void* luno;
    int pad20;
    int pad24;
    void* script;
};

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00224DA0(void* mem, int a1);
extern "C" void func_00224DF0(void* obj, int flags);
extern "C" void func_0030AC98(void* man, void* proc, void* script);
extern "C" void func_00309A60(void* man, void* proc);
extern char D_00489888[];

extern "C" void func_00307C40(func_00307C40_sProc* self)
{
    if (self->luno != 0) {
        func_00224DF0(self->luno, 3);
    }
    self->luno = func_00224DA0(cMemMan_alloc(4, D_00489888, 0x20000000, 0), 8);
    self->state = 1;
    self->cur = 0;
    if (self->script != 0) {
        func_0030AC98(self->man, self, self->script);
    }
    func_00309A60(self->man, self);
    if (self->cur < self->count) {
        self->elems[self->cur].v04();
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307D00);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);

extern "C" void func_00307D00(void* self)
{
    void* script = *(void**)((char*)self + 0x2c);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307D30);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(void* man, void* proc, void* script);
extern "C" void func_00307D78(void* self);

extern "C" void func_00307D30(void* self)
{
    *(int*)self = 3;
    void* script = *(void**)((char*)self + 0x30);
    if (script != 0) {
        func_0030AC98(*(void**)((char*)self + 0x10), self, script);
    }
    func_00307D78(self);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307D78);
#ifdef SKIP_ASM
extern "C" void func_00309AA0(void* man, void* proc);
extern "C" void func_00224DF0(void* obj, int flags);

extern "C" void func_00307D78(void* self)
{
    *(int*)self = 3;
    func_00309AA0(*(void**)((char*)self + 0x10), self);
    void* obj = *(void**)((char*)self + 0x1C);
    if (obj != 0) {
        func_00224DF0(obj, 3);
    }
    *(void**)((char*)self + 0x1C) = 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307DC8);
#ifdef SKIP_ASM
int func_0030B898(void* man, void* proc);

struct sVEntry00307DC8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00307DC8(void* self)
{
    if (func_0030B898(*(void**)((char*)self + 0x10), self) != 0) {
        sVEntry00307DC8* vt = *(sVEntry00307DC8**)((char*)self + 0x5C);
        vt[6].fn((char*)self + vt[6].delta);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307E10);
#ifdef SKIP_ASM
class func_00307E10_cElem {
public:
    char pad00[0x40];
    // vptr at 0x40
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
};

class func_00307E10_cProc {
public:
    int state;
    int count;
    int cur;
    char pad0C[0x8];
    func_00307E10_cElem* elems;
    char pad18[0x44];
    // vptr at 0x5C
    virtual void v01();
    virtual void v02();
};

extern "C" void func_00307E10(func_00307E10_cProc* self)
{
    if (self->state == 3) {
        return;
    }
    self->elems[self->cur].v06();
    int next = self->cur + 1;
    if (next < self->count) {
        self->cur = next;
        func_00307E10_cElem* e = &self->elems[next];
        e->v04();
    } else {
        self->v02();
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307EC0);
#ifdef SKIP_ASM
class func_00307EC0_cState {
public:
    int state;          // 0x0
    int id;             // 0x4
    char pad8[0x40 - 0x8];
    // vptr at 0x40; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

struct func_00307EC0_sProc {
    int pad0;
    int count;                          // 0x4
    int cur;                            // 0x8
    char padC[0x14 - 0xC];
    func_00307EC0_cState* states;       // 0x14
};

extern "C" void func_00307EC0(func_00307EC0_sProc* self, int id)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->states[i].id == id) {
            if (self->cur < self->count) {
                self->states[self->cur].state = 3;
            }
            self->cur = i;
            self->states[i].v04();
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00307F58);
#ifdef SKIP_ASM
class func_00307F58_cElem {
public:
    int f0;
    int id;
    char pad08[0x38];
    // vptr at 0x40
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
};

struct func_00307F58_sProc {
    int state;
    int count;
    int cur;
    int pad0C;
    void* man;
    func_00307F58_cElem* elems;
};

extern "C" void func_00307F58(func_00307F58_sProc* self, int id)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->elems[i].id == id) {
            if (self->cur < self->count) {
                self->elems[self->cur].v06();
            }
            self->cur = i;
            self->elems[i].v04();
            return;
        }
    }
}
#endif

extern "C" void* func_00309B70(int);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308018__FPv);
#ifdef SKIP_ASM
void* func_00308018(void* self)
{
    return func_00309B70(*(int*)((char*)self + 0x10));
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308038);
#ifdef SKIP_ASM
class func_00308038_cStream {
public:
    // slot N at vtable offset N*8
    virtual void v01(void* data, int size);
};

class func_00308760_cObj;
extern "C" int func_00308760(void* a, func_00308760_cObj* obj);
extern char* D_004A47B8;
extern "C" int func_003ACA38(void* map, int id);
extern "C" void func_002257E0(void* obj, void* stream, int a2);

extern "C" void func_00308038(void* self, func_00308038_cStream* stream)
{
    stream->v01(self, 0x10);
    int x = func_003ACA38(*(void**)(D_004A47B8 + 0x4), *(int*)((char*)self + 0x24));
    int has = *(void**)((char*)self + 0x1C) != 0;
    stream->v01(&has, 4);
    if (has) {
        func_002257E0(*(void**)((char*)self + 0x1C), stream, x);
    }
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x4); i++) {
        func_00308760(*(char**)((char*)self + 0x14) + i * 0x44, (func_00308760_cObj*)stream);
    }
}
#endif

INCLUDE_ASM("wscript/wscriptprocess", func_00308118);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308228);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int value, int size);
extern void* D_00489AE0[];

extern "C" void* func_00308228(void* self)
{
    *(void***)((char*)self + 0x40) = D_00489AE0;
    func_003E6448((char*)self + 8, 0, 0xC);
    *(int*)((char*)self + 0x3C) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)self = 0;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", func_00308278);
#ifdef SKIP_ASM
// PORT: func_00308018 is declared (void*) but takes (self, id) and forwards id; bound by asm label
void* func_00308018_2(void* self, int id) __asm__("func_00308018__FPv");

struct func_00308278_sDef {
    int f0;
    int a[3];
    int b[10];
};

struct func_00308278_sObj {
    int f0;
    int f4;
    void* a[3];
    void* b[10];
    void* owner;
};

extern "C" void func_00308278(func_00308278_sObj* self, void* owner, func_00308278_sDef* def)
{
    int i;
    int j;
    self->owner = owner;
    self->f4 = def->f0;
    for (i = 0; i < 3; i++) {
        self->a[i] = func_00308018_2(self->owner, def->a[i]);
    }
    for (j = 0; j < 10; j++) {
        self->b[j] = func_00308018_2(self->owner, def->b[j]);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308328);
#ifdef SKIP_ASM
class func_00308328_cProcess {
public:
    int state;                   // 0x0
    char pad_0x04[0x40 - 0x4];
    // vptr at 0x40; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

extern "C" void func_00308328(func_00308328_cProcess* self)
{
    switch (self->state) {
    case 0:
        break;
    case 1:
        self->state = 2;
        self->v05();
        break;
    case 2:
        self->v05();
        break;
    }
}
#endif

INCLUDE_ASM("wscript/wscriptprocess", func_003083A0);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003084D0);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_003084D0(void* self)
{
    void* script = *(void**)((char*)self + 0x14);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308500);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_00308500(void* self)
{
    void* script = *(void**)((char*)self + 0x18);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308530);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_00308530(void* self)
{
    void* script = *(void**)((char*)self + 0x1c);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308560);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_00308560(void* self)
{
    void* script = *(void**)((char*)self + 0x20);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308590);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_00308590(void* self)
{
    void* script = *(void**)((char*)self + 0x24);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003085C0);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_003085C0(void* self)
{
    void* script = *(void**)((char*)self + 0x28);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003085F0);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_003085F0(void* self)
{
    void* script = *(void**)((char*)self + 0x2c);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308620);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_00308620(void* self)
{
    void* script = *(void**)((char*)self + 0x30);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308650);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_00308650(void* self)
{
    void* script = *(void**)((char*)self + 0x34);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308680);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_00308680(void* self)
{
    void* script = *(void**)((char*)self + 0x38);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003086B0);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_003086B0(void* self)
{
    if (*(int*)self != 1) {
        *(int*)self = 1;
        void* script = *(void**)((char*)self + 0x8);
        if (script != 0) {
            func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3C) + 0x10), self, script);
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003086F0);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_003086F0(void* self)
{
    void* script = *(void**)((char*)self + 0xc);
    if (script != 0) {
        func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3c) + 0x10), self, script);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308720);
#ifdef SKIP_ASM
extern "C" void func_0030ADA8(void* man, void* proc, void* script);

extern "C" void func_00308720(void* self)
{
    if (*(int*)self != 3) {
        *(int*)self = 3;
        void* script = *(void**)((char*)self + 0x10);
        if (script != 0) {
            func_0030ADA8(*(void**)(*(char**)((char*)self + 0x3C) + 0x10), self, script);
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308760);
#ifdef SKIP_ASM
class func_00308760_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
};

extern "C" int func_00308760(void* a, func_00308760_cObj* obj)
{
    return obj->v01(a, 4);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308798);
#ifdef SKIP_ASM
class func_00308798_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
    virtual int v02(void* a, int b);
};

extern "C" int func_00308798(void* a, func_00308798_cObj* obj)
{
    return obj->v02(a, 4);
}
#endif

INCLUDE_ASM("wscript/wscriptprocess", func_003087D0);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_003088D8);
#ifdef SKIP_ASM
extern "C" void func_00308A48(void* self);
extern "C" void func_0030B068(void* self, int flags);
void operator_delete(int* p);
extern char D_00489C00[];
extern int D_004A3DD8;

extern "C" void func_003088D8(void* self, int flags)
{
    *(void**)((char*)self + 0x494) = D_00489C00;
    func_00308A48(self);
    D_004A3DD8 = 0;
    if ((char*)self + 0x2C0 != 0) {
        char* p = (char*)self + 0x2C0 + 0x10C;
        while ((char*)self + 0x2C0 != p) {
            p -= 0x10C;
        }
    }
    func_0030B068((char*)self + 0x2BC, 2);
    func_0030B068((char*)self + 0x2B8, 2);
    func_0030B068((char*)self + 0x2B4, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308988);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int value, int size);
extern "C" void func_0030C760(void* p);
extern "C" void func_00308AA0(void* self);

#define WSP308988_I(off) (*(int*)((char*)self + (off)))
#define WSP308988_U(off) (*(unsigned int*)((char*)self + (off)))

extern "C" void func_00308988(void* self)
{
    WSP308988_I(0x290) = 0;
    WSP308988_I(0x0) = 0;
    WSP308988_I(0x294) = 0;
    WSP308988_I(0x298) = 0;
    WSP308988_I(0x2A4) = 0;
    WSP308988_I(0x2A8) = 0;
    WSP308988_I(0x29C) = 0;
    WSP308988_I(0x2AC) = 0;
    WSP308988_I(0x4) = 0;
    WSP308988_I(0x8) = 0;
    WSP308988_I(0x10) = 0;
    WSP308988_I(0x14) = 0;
    WSP308988_I(0x18) = 0;
    WSP308988_I(0x1C) = 0;
    WSP308988_I(0x20) = 0;
    WSP308988_I(0x28) = 0;
    WSP308988_I(0x30) = 0;
    WSP308988_I(0x34) = 0;
    WSP308988_U(0x2A0) = 0xFFFFFFFF;
    WSP308988_U(0x2B0) = 0xFFFFFFFF;
    WSP308988_I(0xC) = -1;
    WSP308988_I(0x24) = -1;
    WSP308988_U(0x2C) = 0xFFFFFFFF;
    func_003E6448((char*)self + 0x38, 0, 8);
    func_0030C760((char*)self + 0x1C4);
    WSP308988_I(0x40) = 0;
    func_003E6448((char*)self + 0x44, 0, 0x180);
    func_00308AA0(self);
}

#undef WSP308988_I
#undef WSP308988_U
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308A48);
#ifdef SKIP_ASM
extern "C" void func_00224DF0(void* obj, int flags);

extern "C" void func_00308A48(void* self)
{
    void** objs = (void**)((char*)self + 0x3CC);
    int i;
    for (i = 0; i < 50; i++) {
        if (objs[i] != 0) {
            func_00224DF0(objs[i], 3);
            objs[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308AA0);
#ifdef SKIP_ASM
struct cWorldView;
int cWorldView_getNumSections(cWorldView* view);
extern "C" void func_00308A48(void* self);
// PORT: the unit declares func_00308B78 as (void* self); the body also takes the slot index
void* func_00308B78_impl(void* self, int i) __asm__("func_00308B78");

struct func_00308AA0_sSection {
    short type;
    short pad2;
    int f4;
    int f8;
};

extern "C" void func_00308AA0(void* self)
{
    int i = 0;
    func_00308A48(self);
    int n = cWorldView_getNumSections((cWorldView*)(**(char***)((char*)self + 0x28C) + 0x10));
    for (; i < n; i++) {
        func_00308AA0_sSection* tbl = *(func_00308AA0_sSection**)(*(char**)(*(char**)((char*)self + 0x28C) + 0x4) + 0x4);
        int ok = tbl[i].type >= 2 && tbl[i].f8 != 0;
        if (ok) {
            func_00308B78_impl(self, i);
        }
    }
}
#endif

extern "C" void* func_00308B78(void* self);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308B58__FPv);
#ifdef SKIP_ASM
void* func_00308B58(void* self)
{
    return func_00308B78(self);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308B78);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00224DA0(void* mem, int a1);
extern "C" void func_00224DF0(void* obj, int flags);
extern const char D_004898C8[];

// PORT: the unit declares func_00308B78 as (void* self); the body also takes the slot index
void* func_00308B78_impl(void* self, int i) __asm__("func_00308B78");
void* func_00308B78_impl(void* self, int i)
{
    char* base = (char*)self + 0x3CC;
    void** slot = (void**)(base + (i << 2));
    if (*slot != 0) {
        func_00224DF0(*slot, 3);
    }
    return *slot = func_00224DA0(cMemMan_alloc(4, D_004898C8, 0x20000000, 0), 0x40);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308BE0);
#ifdef SKIP_ASM
extern "C" int func_003A6C08(void* self, int i);
extern "C" void* func_003A6C38(void* self, int i);
extern "C" void cWScriptMan_addProcess(void* self, int section, void* def, int flags);

struct func_00308BE0_sDef {
    char pad[0x40];
};

extern "C" void func_00308BE0(void* self, int section)
{
    int n = func_003A6C08(*(void**)((char*)self + 0x28C), section);
    func_00308BE0_sDef* defs = (func_00308BE0_sDef*)func_003A6C38(*(void**)((char*)self + 0x28C), section);
    int i;
    for (i = 0; i < n; i++) {
        cWScriptMan_addProcess(self, section, &defs[i], 0);
    }
}
#endif

INCLUDE_ASM("wscript/wscriptprocess", func_00308C60);

INCLUDE_ASM("wscript/wscriptprocess", func_00308DB8);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308F38);
#ifdef SKIP_ASM
extern "C" void func_0030B1B8(void*);

extern "C" void func_00308F38(void* self)
{
    func_0030B1B8((char*)self + 0x2B8);
    func_0030B1B8((char*)self + 0x2BC);
    func_0030B1B8((char*)self + 0x2B4);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308F70);
#ifdef SKIP_ASM
extern "C" void func_0030B0E8(void* list, int section);
extern "C" void func_00224DF0(void* obj, int flags);

extern "C" void func_00308F70(void* self, int section)
{
    func_0030B0E8((char*)self + 0x2B4, section);
    func_0030B0E8((char*)self + 0x2B8, section);
    func_0030B0E8((char*)self + 0x2BC, section);
    int off = section << 2;
    char* base = (char*)self + 0x3CC;
    void** slot = (void**)(base + off);
    if (*slot != 0) {
        func_00224DF0(*slot, 3);
        *slot = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", func_00308FE0);
#ifdef SKIP_ASM
struct func_00308FE0_sEntry {
    char pad[0xC];
};

extern "C" void func_00308F70(void* self, int i);
extern "C" void func_003ACC90(void* entry);

extern "C" void func_00308FE0(void* self, int i)
{
    func_00308F70(self, i);
    func_00308FE0_sEntry* items =
        *(func_00308FE0_sEntry**)(*(char**)(*(char**)((char*)self + 0x28C) + 4) + 4);
    func_003ACC90(&items[i]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("wscript/wscriptprocess", func_00309030);
#ifdef SKIP_ASM
struct cWorldView;
int cWorldView_getNumSections(cWorldView* view);
extern "C" void func_0030B1B8(void*);
extern "C" void func_00308988(void* self);
extern "C" void func_00308BE0(void* self, int section);
extern "C" void func_00308F70(void* self, int section);

struct func_00309030_sSection {
    short type;
    short pad2;
    int f4;
    int f8;
};

extern "C" void func_00309030(void* self)
{
    int i = 0;
    func_0030B1B8((char*)self + 0x2B8);
    func_0030B1B8((char*)self + 0x2BC);
    func_0030B1B8((char*)self + 0x2B4);
    func_00308988(self);
    int n = cWorldView_getNumSections((cWorldView*)(**(char***)((char*)self + 0x28C) + 0x10));
    for (; i < n; i++) {
        func_00309030_sSection* tbl = *(func_00309030_sSection**)(*(char**)(*(char**)((char*)self + 0x28C) + 0x4) + 0x4);
        int ok = tbl[i].type >= 2 && tbl[i].f8 != 0;
        if (ok) {
            func_00308BE0(self, i);
        } else {
            func_00308F70(self, i);
        }
    }
}
#endif

INCLUDE_ASM("wscript/wscriptprocess", func_00309118);

INCLUDE_ASM("wscript/wscriptprocess", func_00309270);


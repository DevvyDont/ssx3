#include "common.h"

INCLUDE_ASM("wscript/wscriptprocess", cWScriptProcess_cWScriptProcess);

INCLUDE_ASM("wscript/wscriptprocess", func_00307738);

INCLUDE_ASM("wscript/wscriptprocess", func_003077F8);

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

INCLUDE_ASM("wscript/wscriptprocess", func_00307A10);

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

INCLUDE_ASM("wscript/wscriptprocess", func_00307C40);

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

INCLUDE_ASM("wscript/wscriptprocess", func_00307D30);

INCLUDE_ASM("wscript/wscriptprocess", func_00307D78);

INCLUDE_ASM("wscript/wscriptprocess", func_00307DC8);

INCLUDE_ASM("wscript/wscriptprocess", func_00307E10);

INCLUDE_ASM("wscript/wscriptprocess", func_00307EC0);

INCLUDE_ASM("wscript/wscriptprocess", func_00307F58);

extern "C" void* func_00309B70(int);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308018__FPv);
#ifdef SKIP_ASM
void* func_00308018(void* self)
{
    return func_00309B70(*(int*)((char*)self + 0x10));
}
#endif

INCLUDE_ASM("wscript/wscriptprocess", func_00308038);

INCLUDE_ASM("wscript/wscriptprocess", func_00308118);

INCLUDE_ASM("wscript/wscriptprocess", func_00308228);

INCLUDE_ASM("wscript/wscriptprocess", func_00308278);

INCLUDE_ASM("wscript/wscriptprocess", func_00308328);

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

INCLUDE_ASM("wscript/wscriptprocess", func_003086B0);

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

INCLUDE_ASM("wscript/wscriptprocess", func_00308720);

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

INCLUDE_ASM("wscript/wscriptprocess", func_003088D8);

INCLUDE_ASM("wscript/wscriptprocess", func_00308988);

INCLUDE_ASM("wscript/wscriptprocess", func_00308A48);

INCLUDE_ASM("wscript/wscriptprocess", func_00308AA0);

extern "C" void* func_00308B78(void* self);

//100%
INCLUDE_ASM("wscript/wscriptprocess", func_00308B58__FPv);
#ifdef SKIP_ASM
void* func_00308B58(void* self)
{
    return func_00308B78(self);
}
#endif

INCLUDE_ASM("wscript/wscriptprocess", func_00308B78);

INCLUDE_ASM("wscript/wscriptprocess", func_00308BE0);

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

INCLUDE_ASM("wscript/wscriptprocess", func_00308F70);

INCLUDE_ASM("wscript/wscriptprocess", func_00308FE0);

INCLUDE_ASM("wscript/wscriptprocess", func_00309030);

INCLUDE_ASM("wscript/wscriptprocess", func_00309118);

INCLUDE_ASM("wscript/wscriptprocess", func_00309270);


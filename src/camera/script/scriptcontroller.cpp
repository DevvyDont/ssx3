#include "common.h"

INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_cScriptCameraController);

INCLUDE_ASM("camera/script/scriptcontroller", func_001690D0);

INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_addCamera);

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_001692D8);
#ifdef SKIP_ASM
struct sScrAlgoNode {
    int key;              // 0x0
    char pad_0x04[0x10];
    sScrAlgoNode* next;   // 0x14
};

struct sScrAlgoList {
    sScrAlgoNode* head;   // 0x0
    int count;            // 0x4
};

extern "C" void func_0015CA50(sScrAlgoList* list, sScrAlgoNode* node);

extern "C" void func_001692D8(void* self, void* algo)
{
    if (*(int*)((char*)self + 0x18) != 0) {
        sScrAlgoList* list = *(sScrAlgoList**)((char*)self + 0x8);
        for (sScrAlgoNode* n = list->head; n != 0; n = n->next) {
            int k = *(int*)((char*)algo + 0x1C);
            if (n->key == k) {
                func_0015CA50(list, n);
                break;
            }
        }
        if (list->count <= 0)
            *(int*)((char*)self + 0x18) = 0;
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", func_001694B8);
#ifdef SKIP_ASM
struct func_001694B8_sVec4 { float x, y, z, w; } __attribute__((aligned(16)));

extern void* D_0045C850[];
extern func_001694B8_sVec4 D_004FF130;
extern "C" void* func_00169418(void* self);
extern "C" void* func_0015FFF0(void* self);

extern "C" void* func_001694B8(void* self)
{
    func_00169418(self);
    *(void***)((char*)self + 0x10) = D_0045C850;
    func_0015FFF0((char*)self + 0x74);
    *(func_001694B8_sVec4*)((char*)self + 0x30) = D_004FF130;
    *(int*)((char*)self + 0xC) = 0x56;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x40) = 0;
    return self;
}
#endif

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

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_00169540);
#ifdef SKIP_ASM
class cScriptCtlVirt2 {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03(int);
};

extern "C" void func_00169540(cScriptCtlVirt2* self)
{
    self->v03(0);
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_00169570);

INCLUDE_ASM("camera/script/scriptcontroller", func_00169828);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", func_00169CF8);
#ifdef SKIP_ASM
extern void* D_0045C7E8[];
extern func_001694B8_sVec4 D_004FF130;
extern "C" void* func_00169418(void* self);
extern "C" void* func_0015FFF0(void* self);

extern "C" void* func_00169CF8(void* self)
{
    func_00169418(self);
    *(void***)((char*)self + 0x10) = D_0045C7E8;
    func_0015FFF0((char*)self + 0x74);
    *(func_001694B8_sVec4*)((char*)self + 0x30) = D_004FF130;
    *(int*)((char*)self + 0xC) = 0x57;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_00169D58);
#ifdef SKIP_ASM
extern "C" void func_00169D58(cScriptCtlVirt* self)
{
    self->v05();
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_00169D80);
#ifdef SKIP_ASM
extern "C" void func_00169D80(cScriptCtlVirt2* self)
{
    self->v03(0);
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_00169DB0);

INCLUDE_ASM("camera/script/scriptcontroller", func_00169F88);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", func_0016A458);
#ifdef SKIP_ASM
extern void* D_0045C780[];
extern func_001694B8_sVec4 D_004FF130;
extern "C" void* func_00169418(void* self);
extern "C" void* func_0015FFF0(void* self);

extern "C" void* func_0016A458(void* self)
{
    func_00169418(self);
    *(void***)((char*)self + 0x10) = D_0045C780;
    func_0015FFF0((char*)self + 0x70);
    *(func_001694B8_sVec4*)((char*)self + 0x30) = D_004FF130;
    *(int*)((char*)self + 0xC) = 0x58;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016A4B8);
#ifdef SKIP_ASM
extern "C" void func_0016A4B8(cScriptCtlVirt* self)
{
    self->v05();
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016A4E0);
#ifdef SKIP_ASM
extern "C" void func_0016A4E0(cScriptCtlVirt2* self)
{
    self->v03(0);
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_0016A510);

INCLUDE_ASM("camera/script/scriptcontroller", func_0016A868);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/script/scriptcontroller", func_0016AD38);
#ifdef SKIP_ASM
extern void* D_0045C718[];
extern func_001694B8_sVec4 D_004FF130;
extern "C" void* func_00169418(void* self);
extern "C" void* func_0015FFF0(void* self);

extern "C" void* func_0016AD38(void* self)
{
    func_00169418(self);
    *(void***)((char*)self + 0x10) = D_0045C718;
    func_0015FFF0((char*)self + 0x70);
    *(func_001694B8_sVec4*)((char*)self + 0x30) = D_004FF130;
    *(int*)((char*)self + 0xC) = 0x59;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

INCLUDE_ASM("camera/script/scriptcontroller", func_0016AD98);

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_0016B150);
#ifdef SKIP_ASM
extern "C" void func_0016B150(cScriptCtlVirt2* self)
{
    self->v03(0);
}
#endif

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


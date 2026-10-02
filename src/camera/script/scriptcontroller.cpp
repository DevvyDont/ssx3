#include "common.h"

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_cScriptCameraController);
#ifdef SKIP_ASM
extern "C" void* cCameraController_cCameraController(void* self, void* arg);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0027D5E8(void* mem, void* script);
extern "C" void func_00283298(void* p);
// PORT: operator_new__FUi takes (size, tag, flags, align) here.
void* operator_new_K2(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern void* D_0045C428[];
extern char D_0045BE88[];
extern char D_0045BEA0[];

extern "C" void* cScriptCameraController_cScriptCameraController(void* self, void* script, void* arg)
{
    int i;
    cCameraController_cCameraController(self, arg);
    *(void***)((char*)self + 0x14) = D_0045C428;
    *(int*)self = 3;
    *(void***)((char*)self + 0x1C) = (void**)operator_new_K2(8, D_0045BE88, 0, 0);
    for (i = 0; i < 2; i++) {
        void** slot = (void**)((i << 2) + *(int*)((char*)self + 0x1C));
        *slot = func_0027D5E8(cMemMan_alloc(0x80, D_0045BEA0, 0, 0), script);
        func_00283298((*(void***)((char*)self + 0x1C))[i]);
    }
    *(int*)((char*)self + 0x18) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_001690D0);
#ifdef SKIP_ASM
extern "C" void func_002832D8(void* p);
extern "C" void func_0015CC10(void* self, int flags);
void cMemMan_free(void* p);
extern void* D_0045C428[];

class cScrCamScriptK2 {
public:
    char pad_0x00[0xC];
    // vptr at 0xC
    virtual ~cScrCamScriptK2();
};

extern "C" void func_001690D0(void* self, int flags)
{
    int i;
    *(void***)((char*)self + 0x14) = D_0045C428;
    for (i = 0; i < 2; i++) {
        func_002832D8((*(cScrCamScriptK2***)((char*)self + 0x1C))[i]);
        delete (*(cScrCamScriptK2***)((char*)self + 0x1C))[i];
    }
    if (*(void**)((char*)self + 0x1C) != 0)
        cMemMan_free(*(void**)((char*)self + 0x1C));
    func_0015CC10(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", cScriptCameraController_addCamera);
#ifdef SKIP_ASM
int func_002743C8(void* self);
void func_001694A8(void* self, int val);
extern "C" void* func_001694B8(void* self);
extern "C" void* func_00169CF8(void* self);
extern "C" void* func_0016A458(void* self);
extern "C" void* func_0016AD38(void* self);
// PORT: func_00169340 is defined with (self, algo); this caller passes a third int argument.
extern "C" void func_00169340_3(void* self, void* algo, int a) __asm__("func_00169340");
extern char D_0045BEB0[];
extern char D_0045BED0[];
extern char D_0045BEF0[];
extern char D_0045BF10[];

extern "C" void cScriptCameraController_addCamera(char* self, char* cam)
{
    void* ctl;
    switch (func_002743C8(*(void**)(*(char**)(cam + 0x14) + 8)))
    {
    case 0:
        ctl = func_001694B8(cMemMan_alloc(0xB0, D_0045BEB0, 0x20000000, 0));
        break;
    case 1:
        ctl = func_00169CF8(cMemMan_alloc(0xB0, D_0045BED0, 0x20000000, 0));
        break;
    case 2:
        ctl = func_0016A458(cMemMan_alloc(0xA0, D_0045BEF0, 0x20000000, 0));
        break;
    case 3:
        ctl = func_0016AD38(cMemMan_alloc(0xA0, D_0045BF10, 0x20000000, 0));
        break;
    default:
        return;
    }
    // PORT: the camera pointer is passed through an int parameter.
    func_001694A8(ctl, (int)cam);
    *(void**)(cam + 0x1C) = ctl;
    func_00169340_3(self, ctl, 1);
    *(int*)(self + 0x18) = 1;
}
#endif

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

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_00169368);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001694B0 returns a pointer (the unit declares it int); bind a pointer-returning alias.
char* func_001694B0_K2(void* self) __asm__("func_001694B0__FPv");

class cScrCamAlgoK2 {
public:
    char pad_0x00[0x10];
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

struct sScrAlgoNodeK2 {
    cScrCamAlgoK2* algo;    // 0x0
    float t;                // 0x4
    float pad_0x08;
    float weight;           // 0xC
    int pad_0x10;
    sScrAlgoNodeK2* next;   // 0x14
};

extern "C" void func_00169368(void* self)
{
    sScrAlgoNodeK2* n = **(sScrAlgoNodeK2***)((char*)self + 0x8);
    while (n != 0) {
        float t = *(float*)(func_001694B0_K2(n->algo) + 0x18);
        float t2 = t * t;
        float s = t2 * 3.0f - t2 * 2.0f * t;
        n->t = t;
        if (s > 1.0f)
            s = 1.0f;
        n->weight = s;
        n->algo->v05();
        n = n->next;
    }
}
#endif

//100%
INCLUDE_ASM("camera/script/scriptcontroller", func_00169418);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_0045BDE0[];
extern void* D_0045C8B8[];
extern void* D_004A28A8;
struct sVec4K9418 { float x, y, z, w; } __attribute__((aligned(16)));
// Same object as the unit's later `extern func_001694B8_sVec4 D_004FF130;` (that type is defined below this point).
extern sVec4K9418 D_004FF130_K9418 __asm__("D_004FF130");
extern "C" float func_00167E30(void* self);

struct sCamCtlK9418 {
    float fov;      // 0x0
    float nearZ;    // 0x4
    float farZ;     // 0x8
    int type;       // 0xC
    void** vtbl;    // 0x10
    int f14;        // 0x14
    int f18;        // 0x18
    int pad_1C;
    sVec4K9418 v20; // 0x20
};

extern "C" void* func_00169418(void* p)
{
    sCamCtlK9418* self = (sCamCtlK9418*)p;
    self->vtbl = D_0045BDE0;
    float fov = func_00167E30(self);
    self->type = 0x55;
    self->vtbl = D_0045C8B8;
    self->fov = fov;
    self->nearZ = 10.0f;
    self->farZ = 30000.0f;
    self->f14 = 0;
    void* cam = *(void**)((char*)D_004A28A8 + 0x84);
    void* chase = *(void**)((char*)cam + 0x84);
    void* ctrl = *(void**)((char*)chase + 0x4);
    self->f18 = *(int*)((char*)ctrl + 0xAC);
    self->v20 = D_004FF130_K9418;
    return self;
}
#endif

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


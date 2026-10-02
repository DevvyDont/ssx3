#include "common.h"

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onCreateScreen);

INCLUDE_ASM("fe/uitemplatemap", func_002009D0);

INCLUDE_ASM("fe/uitemplatemap", func_00200A70);

INCLUDE_ASM("fe/uitemplatemap", func_00200AC0);

INCLUDE_ASM("fe/uitemplatemap", func_00200AF0);

INCLUDE_ASM("fe/uitemplatemap", func_00200B50);

INCLUDE_ASM("fe/uitemplatemap", func_00200D00);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMenus);

struct cUITemplate_MAP {
    char pad_0x00[0x10];
    int field_0x10;
    int field_0x14;
    int field_0x18;
    int field_0x1C;
};
extern unsigned int D_004A2594;

//99.9%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupEventUnlock__FP15cUITemplate_MAP);
#ifdef SKIP_ASM
void cUITemplate_MAP_setupEventUnlock(cUITemplate_MAP* self)
{
    self->field_0x1C = 0;
    self->field_0x10 = 1;
    self->field_0x14 = 1;
    if (D_004A2594 < 2) {
        self->field_0x18 = 0;
    } else {
        self->field_0x18 = 1;
    }
}
#endif

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakUnlock);

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00201868);
#ifdef SKIP_ASM
struct sVEntry00201868 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00201868(void* self)
{
    *(int*)((char*)self + 0x2F0) = 1;
    *(int*)((char*)self + 0x2DC) = 0;
    void* obj = *(void**)((char*)self + 0x2E4);
    if (obj != 0) {
        sVEntry00201868* vt = *(sVEntry00201868**)((char*)obj + 8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

INCLUDE_ASM("fe/uitemplatemap", func_002018A8);

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00202738);
#ifdef SKIP_ASM
extern "C" int func_00202738(void* self, int a1, int a2)
{
    if (a2 == 6 || a2 == 9 || a2 == 8) {
        return 0;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00202768__FPv);
#ifdef SKIP_ASM
int func_00202768(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("fe/uitemplatemap", func_00202770);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onUpdate);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupLayout);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMenuFocus);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMapPic);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPopup);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setShowInfo);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakInfo);

INCLUDE_ASM("fe/uitemplatemap", func_00204FF0);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakGoals);

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_showEventPath);

INCLUDE_ASM("fe/uitemplatemap", func_00207430);

//100%
INCLUDE_ASM("fe/uitemplatemap", func_002083D8);
#ifdef SKIP_ASM
struct sVEntry_func_002083D8 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char D_004C8B38[];
extern char D_004C8B28[];

extern "C" void func_002083D8(void* self, void* obj, int on)
{
    if (obj != 0) {
        if (on != 0) {
            sVEntry_func_002083D8* vt = *(sVEntry_func_002083D8**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, D_004C8B38);
        } else {
            sVEntry_func_002083D8* vt = *(sVEntry_func_002083D8**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, D_004C8B28);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00208438);
#ifdef SKIP_ASM
struct sVEntry_func_00208438 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char D_004C8B58[];
extern char D_004C8B48[];

extern "C" void func_00208438(void* self, void* obj, int on)
{
    if (obj != 0) {
        if (on != 0) {
            sVEntry_func_00208438* vt = *(sVEntry_func_00208438**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, D_004C8B58);
        } else {
            sVEntry_func_00208438* vt = *(sVEntry_func_00208438**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, D_004C8B48);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208498);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void func_0020A380(void* self);
extern "C" void cUITemplate_MAP_onCreateScreen(void* tmpl, void* screen, void* owner);
extern char D_004A26E8[];

extern "C" void func_00208498(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A26E8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0020A380(self);
    cUITemplate_MAP_onCreateScreen((char*)self + 0x9C, *(void**)((char*)self + 0x40), self);
}
#endif

extern "C" void* func_002009D0(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", func_00208518__FPv);
#ifdef SKIP_ASM
void* func_00208518(void* self)
{
    return func_002009D0((char*)self + 0x9c);
}
#endif

extern "C" void* func_00200A70(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", func_00208538__FPv);
#ifdef SKIP_ASM
void* func_00208538(void* self)
{
    return func_00200A70((char*)self + 0x9c);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208558);
#ifdef SKIP_ASM
extern "C" void func_0039E4C0(void* self);
extern "C" void func_00200AC0(void* self);

extern "C" void func_00208558(void* self)
{
    func_0039E4C0(self);
    func_00200AC0((char*)self + 0x9C);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208588);
#ifdef SKIP_ASM
extern "C" void func_00200AF0(void* self);
extern "C" void func_0020A430(void* self);

extern "C" void func_00208588(void* self)
{
    func_00200AF0((char*)self + 0x9C);
    func_0020A430(self);
}
#endif

INCLUDE_ASM("fe/uitemplatemap", func_002085B8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208610);
#ifdef SKIP_ASM
extern "C" int func_00208610(void* self, int a1, int a2)
{
    if (func_00202738((char*)self + 0x9C, a1, a2) != 0) {
        return 0x101;
    }
    return 0;
}
#endif

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("fe/uitemplatemap", func_00208638);
#ifdef SKIP_ASM
extern "C" void func_00208638(void* self)
{
    func_00202768((char*)self + 0x9c);
}
#endif

extern "C" void* func_00202770(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", func_00208658__FPv);
#ifdef SKIP_ASM
void* func_00208658(void* self)
{
    return func_00202770((char*)self + 0x9c);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208678);
#ifdef SKIP_ASM
extern "C" void func_0020E900(void* self);
extern "C" void cUITemplate_MAP_onUpdate(void* self);

extern "C" void func_00208678(void* self)
{
    func_0020E900(self);
    cUITemplate_MAP_onUpdate((char*)self + 0x9C);
}
#endif


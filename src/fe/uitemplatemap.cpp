#include "common.h"

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onCreateScreen);

//100%
INCLUDE_ASM("fe/uitemplatemap", func_002009D0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_00144BC0(void* iface);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
int GetHashValue32(char* str);
extern char D_00470600[];
extern unsigned int D_004A2594;
static inline int isLabel2009D0(int id, char* s)
{
    return id == GetHashValue32(s);
}
// PORT: the unit declares func_002009D0(void*); the body also takes the menu in $5.
extern "C" void func_002009D0_r(void* self, void* menu) __asm__("func_002009D0");

extern "C" void func_002009D0_r(void* self, void* menu)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (isLabel2009D0(*(int*)((char*)menu + 0x38), D_00470600)) {
        if (D_004A2594 < 2) {
            cUIMenu_setSelectedByIndex(menu, 2);
        } else {
            cUIMenu_setSelectedByIndex(menu, 2 - *(unsigned char*)((char*)func_00144BC0(iface) + 0x54));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00200A70);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupMenuFocus(void* self, int focus);
extern "C" void cUITemplate_MAP_setupMenus(void* self);
extern "C" void func_00207430(void* self);
extern "C" void cUITemplate_MAP_setShowInfo(void* self, int show);
extern int D_004A259C;
// PORT: the unit declares func_00200A70(void*); the body also takes a flag in $5.
extern "C" int func_00200A70_r(void* self, int on) __asm__("func_00200A70");

extern "C" int func_00200A70_r(void* self, int on)
{
    if (on != 0) {
        cUITemplate_MAP_setupMenuFocus(self, D_004A259C);
        cUITemplate_MAP_setupMenus(self);
        func_00207430(self);
        cUITemplate_MAP_setShowInfo(self, *(int*)self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00200AC0);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupMenuFocus(void* self, int focus);
extern "C" void* func_0028B180(void);
extern "C" void func_0028F5B8(void* mgr);
extern int D_004A259C;

extern "C" void func_00200AC0(void* self)
{
    cUITemplate_MAP_setupMenuFocus(self, D_004A259C);
    func_0028F5B8(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00200AF0);
#ifdef SKIP_ASM
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(void* mgr, int file);
extern "C" void func_0020D190(void);
extern "C" void* func_0028B180(void);
extern "C" void func_0028F678(void* mgr, int a);
extern void* D_004A28A8;
extern unsigned int D_004A2594;

extern "C" void func_00200AF0(void* self)
{
    cFEAsyncManager_UnloadFEAsyncFile(*(void**)((char*)D_004A28A8 + 0x11C), *(int*)((char*)self + 0x2D8));
    if (D_004A2594 == 1 || D_004A2594 == 3 || D_004A2594 == 4) {
        func_0020D190();
    }
    func_0028F678(func_0028B180(), 0);
}
#endif

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

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setShowInfo);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_00470AB8[];
extern char D_00470AC8[];
extern char D_00470AD8[];

extern "C" void cUITemplate_MAP_setShowInfo(void* self, int show)
{
    *(int*)self = show;
    cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x2E8), GetHashValue32(D_00470AB8));
    if (*(int*)self != 0) {
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470AC8));
    } else {
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470AD8));
    }
}
#endif

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


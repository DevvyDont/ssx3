#include "common.h"

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void func_0020A380(void* self);
extern "C" void func_00212080(void* self);
extern char D_00471D50[];

extern "C" void cOVState_PROFILE_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00471D50), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVStateManager_addPDATemplate();
    func_00212080(self);
    func_0020A380(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211220);
#ifdef SKIP_ASM
extern "C" void func_00211380(void* self);
extern "C" void func_00211270(void* self);

extern "C" void func_00211220(void* self)
{
    if (*(int*)((char*)self + 0x1C0) != 0) {
        *(int*)((char*)self + 0x22C) = 1;
    }
    if (*(int*)((char*)self + 0x214) == 3) {
        func_00211380(self);
    } else {
        func_00211270(self);
    }
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00211270);

INCLUDE_ASM("fe/ovstateprofile", func_00211380);

INCLUDE_ASM("fe/ovstateprofile", func_00211850);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211970);
#ifdef SKIP_ASM
class cUIObj_211970 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_00471DB0[];
extern char D_004A20A0[];

extern "C" void func_00211970(void* self, bool on)
{
    ((cUIObj_211970*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00471DB0)))->setVisible(!on);
    ((cUIObj_211970*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A20A0)))->setVisible(!on);
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211A08);
#ifdef SKIP_ASM
class cUIObj_211A08 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_004A2090[];
extern char D_004A2568[];

extern "C" void func_00211A08(void* self, int a1, bool on)
{
    cUIObj_211A08* a = (cUIObj_211A08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2090));
    ((cUIObj_211A08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2568)))->setVisible(!on);
    *(int*)((char*)self + 0x224) = !on;
    a->setVisible(!on);
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00211AA8);

extern "C" void* func_001D58B8(void*);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211B10__FPv);
#ifdef SKIP_ASM
void func_00211B10(void* self)
{
    func_001D58B8(self);
    *(int*)((char*)self + 0x1a4) = 0;
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00211B38);

INCLUDE_ASM("fe/ovstateprofile", func_00211BC8);

INCLUDE_ASM("fe/ovstateprofile", func_00212080);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00212138);
#ifdef SKIP_ASM
extern "C" void* func_001D53B0(void* self, void* a1, int a2);
extern void* D_00471F80[];

extern "C" void* func_00212138(void* self, void* a1, int a2)
{
    func_001D53B0(self, a1, 0);
    *(void***)((char*)self + 0x8) = D_00471F80;
    *(int*)((char*)self + 0xC) = 0x25;
    *(int*)((char*)self + 0x208) = 1;
    *(int*)((char*)self + 0x22C) = a2;
    *(int*)((char*)self + 0x1C4) = 0;
    *(int*)((char*)self + 0x230) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void func_0020A380(void* self);
extern char D_00471E28[];

extern "C" void cOVState_AUTOSAVE_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00471E28), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0020A380(self);
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00212208);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_002122E0__FPv);
#ifdef SKIP_ASM
int func_002122E0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", func_002122E8);
#ifdef SKIP_ASM
struct sVEntry002122E8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj002122E8 {
    int pad[2];
    sVEntry002122E8* vt;
};

int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_AUTOSAVE_displayOn(void* self);
extern char D_00471E38[];

extern "C" void func_002122E8(void* self)
{
    int s = *(int*)((char*)self + 0x1C0);
    if (s != 0 && s != 6) {
        cOVState_AUTOSAVE_displayOn(self);
    } else {
        sObj002122E8* o = (sObj002122E8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00471E38));
        o->vt[9].fn((char*)o + o->vt[9].delta, 0);
    }
}
#endif

INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_displayOn);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_002124C8);
#ifdef SKIP_ASM
extern "C" void func_0039F718(void* self);

extern "C" void func_002124C8(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00212508);


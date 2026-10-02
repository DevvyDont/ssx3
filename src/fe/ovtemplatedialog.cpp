#include "common.h"

INCLUDE_ASM("fe/ovtemplatedialog", cPDATemplate_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020A8B0);
#ifdef SKIP_ASM
extern "C" int func_0020A8B0(int a0, int a1)
{
    if ((unsigned int)(a0 - 6) < 6) {
        return 0x16;
    }
    if ((unsigned int)(a0 - 4) < 2 || a0 == 0 || a1 == 3) {
        return 0xb;
    }
    return 0xa;
}
#endif

INCLUDE_ASM("fe/ovtemplatedialog", func_0020A8F8);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020AB50);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CA10);

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020CBA0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int func_0039F9D8(void* stack, int hash);
extern void* D_004A28A8;
extern char D_0046F818[];

extern "C" int func_0020CBA0(void)
{
    char* stack = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x48) + 0x18;
    return func_0039F9D8(stack, GetHashValue32(D_0046F818)) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020CBE8);
#ifdef SKIP_ASM
class cUIState20CBE8 {
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
    virtual int v25();
};
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern "C" int func_0020CBA0(void);
extern void* D_004A28A8;

extern "C" int func_0020CBE8(void)
{
    char* mgr = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x48);
    if (func_0020CBA0() == 0) {
        cUIState20CBE8* s = (cUIState20CBE8*)cUIStateStack_getCurrentState(mgr + 0x18);
        if (s != 0) {
            if (*(int*)((char*)s + 0x1C) & 1) {
                return 0;
            }
            return s->v25();
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020CC60);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0020AB50(int state);
extern char D_00535BC8[];
extern int D_004A26FC;

extern "C" void func_0020CC60(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* g = (signed char*)D_00535BC8;
    int mode = g[0x48];
    int players = g[0x49];
    if (mode == 0 || players == 2) {
        func_0020AB50(0xC);
    } else if ((mode >= 1 && mode <= 3) || mode == 6) {
        func_0020AB50(0xD);
    } else if (mode == 4) {
        D_004A26FC = 2;
    } else {
        func_0020AB50(0xC);
    }
}
#endif

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CCF8);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CFD0);

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_addPDATemplate);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0039E2A0(void* self, void* mgr);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
extern void* D_004A270C;
extern void* D_004A28A8;
extern char D_00471A10[];
extern char D_004743F0[];

extern "C" void cOVStateManager_addPDATemplate(void)
{
    if (D_004A270C == 0) {
        char* mgr = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x48);
        void* t = cMemMan_alloc(0x48, D_00471A10, 0x100, 0);
        func_0039E2A0(t, mgr);
        *(void**)((char*)t + 8) = D_004743F0;
        D_004A270C = t;
        cUIStateStack_pushExplicit(mgr + 0x18, t);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020D190);
#ifdef SKIP_ASM
class cOVTemplate20D190 {
public:
    int pad[2];
    virtual ~cOVTemplate20D190();
};
extern "C" void cListNode_removeFromList(void* node);
extern void* D_004A270C;

extern "C" void func_0020D190(void)
{
    if (D_004A270C != 0) {
        cListNode_removeFromList(D_004A270C);
        delete (cOVTemplate20D190*)D_004A270C;
    }
    D_004A270C = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020D1D8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" void func_0020A380(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_004A2860[2];
extern char* D_00441D10[];
extern char D_0046E510[];
extern char D_0046E4C0[];

class cUIObj_20D1D8 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

extern "C" void func_0020D1D8(void* self)
{
    func_0020A380(self);
    char* list = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046E510));
    *(int*)(list + 0x90) |= 8;
    for (int i = 0; i < 5; i++) {
        char buf[32];
        sprintf(buf, D_0046E4C0, i);
        cUIObj_20D1D8* o = (cUIObj_20D1D8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        if (o != 0) {
            if (i < 1) {
                int v = D_004A2860[i];
                cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(D_00441D10[v]));
                *(int*)((char*)o + 0x18) = v;
            } else {
                o->setVisible(0);
                o->setEnabled(1);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020D308);
#ifdef SKIP_ASM
extern "C" int func_0020D308(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020D318);
#ifdef SKIP_ASM
extern int D_004A26FC;

extern "C" void func_0020D318(void* self, int a1, int key)
{
    if (a1 != 0 && key == 5) {
        D_004A26FC = 1;
    }
}
#endif

INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Dialog_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020D4F0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int idx);
extern char D_0046E818[];

extern "C" void func_0020D4F0(void* self, void* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(D_0046E818)) {
        if (*(int*)((char*)self + 0x9C) == 6) {
            cUIMenu_setSelectedByIndex(item, 0);
        } else {
            cUIMenu_setSelectedByIndex(item, 1);
        }
    }
}
#endif

INCLUDE_ASM("fe/ovtemplatedialog", func_0020D568);

INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Dialog_onWidgetEvent);

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020DBD0);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0020E900(void* self);
extern "C" void func_0039F190(void* self, int a1);
extern char* D_004A2EEC;
extern char D_0046E050[];

extern "C" void func_0020DBD0(void* self)
{
    func_0020E900(self);
    char* p = D_004A2EEC;
    if (p != 0) {
        int ok = *(int*)(p + 0x68) == 0 || *(int*)(p + 0x64) == 0;
        if (ok) {
            int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_0046E050));
            if (frame != 0xFFFF) {
                cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
            }
            func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        }
    }
}
#endif

INCLUDE_ASM("fe/ovtemplatedialog", func_0020DC68);


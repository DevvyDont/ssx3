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

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211270);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_00211380(void* self);
extern "C" void* func_00227F80(void* app);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern void* D_004A28A8;
extern char D_00471D68[];

struct cSelf_11270 {
    int f0;
    int f4;
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
    virtual void setState(int s);
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual int isReady();
};
struct cCard_11270 {
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
    virtual void v51();
    virtual void v52();
    virtual int isPresent();
};
struct cUIObj_11270 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
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
    virtual void setSelected(int v);
};

extern "C" void func_00211270(void* p)
{
    cSelf_11270* self = (cSelf_11270*)p;
    if (self->isReady() != 0) {
        if ((*(cCard_11270**)((char*)func_00227F80(D_004A28A8) + 0x434))->isPresent() != 0) {
            self->setState(3);
            func_00211380(self);
            return;
        }
    }
    for (int i = 1; i < 7; i++) {
        char buf[64];
        sprintf(buf, D_00471D68, i);
        cUIObj_11270* o = (cUIObj_11270*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        o->setSelected(0);
        o->setEnabled(1);
    }
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00211380);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211850);
#ifdef SKIP_ASM
class cUIObj_211850 {
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
extern char D_00471DB0[];
extern char D_004A20A0[];

extern "C" void func_00211850(void* self, bool show)
{
    bool on = true;
    if (*(int*)((char*)self + 0x22C) != 0) {
        on = show;
    }
    if (*(int*)((char*)self + 0x230) != on) {
        int off = !on;
        cUIObj_211850* a = (cUIObj_211850*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2090));
        if (a != 0) {
            a->setVisible(off);
        }
        ((cUIObj_211850*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2568)))->setVisible(off);
        ((cUIObj_211850*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00471DB0)))->setVisible(off);
        cUIObj_211850* d = (cUIObj_211850*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A20A0));
        if (d != 0) {
            d->setVisible(off);
        }
        *(int*)((char*)self + 0x230) = on;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211AA8);
#ifdef SKIP_ASM
extern "C" void cGame_exit(void* game, int a);
extern void* D_004A28A8;
extern "C" void func_0039F718(void* self);

extern "C" void func_00211AA8(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        if (*(int*)((char*)self + 0x278) == 0x18) {
            cGame_exit(*(void**)((char*)D_004A28A8 + 0x84), 0);
        } else {
            func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
        }
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211B38);
#ifdef SKIP_ASM
class cOvl211B38 {
public:
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
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual int v54(void*);
};

extern void* D_004A28A8;
extern "C" void* func_00227F80(void* app);

extern "C" int func_00211B38(void* self, int a1, unsigned int key)
{
    char* mgr = (char*)func_00227F80(D_004A28A8);
    switch (key) {
    case 6:
    case 8:
        if ((*(cOvl211B38**)(mgr + 0x434))->v54(*(void**)(mgr + 0x428)) == 0) return 0x100;
        break;
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00211BC8);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00212080);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004A2138[];
extern char D_00471D68[];

extern "C" void func_00212080(void* self)
{
    char buf[16];
    int i;
    for (i = 1; i < 7; i++) {
        sprintf(buf, D_004A2138, i);
        cUIText_setAsciiString((cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf)), buf);
        sprintf(buf, D_00471D68, i);
        *(int*)((char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf)) + 0x18) = i - 1;
    }
}
#endif

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


#include "common.h"

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045E240[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFEStateProfileSelect_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045E240), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018EB40);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018ECA0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00147318(void* self, int a1);
extern "C" void* func_001D53B0(void* self, int a1, int a2);
extern void* D_0046BFD0[];
extern int D_004A15E8;
extern char D_00535BC8[];

extern "C" void* func_0018ECA0(void* self, int a1, int a2, unsigned char flags)
{
    func_001D53B0(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x23;
    *(void***)((char*)self + 0x8) = D_0046BFD0;
    D_004A15E8 = a2;
    *(int*)((char*)self + 0x1CC) = 0;
    *(int*)((char*)self + 0x230) = 0;
    *(int*)((char*)self + 0x234) = 0;
    *(int*)((char*)self + 0x27C) = 0;
    *(unsigned char*)((char*)self + 0x15) = flags;
    *(int*)((char*)self + 0x22C) = 0;
    for (int i = 0; i < 3; i++) {
        if (flags & 1) {
            break;
        }
        flags >>= 1;
        *(int*)((char*)self + 0x22C) += 1;
    }
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* g = (signed char*)D_00535BC8;
    int n = 1;
    if (g[0x49] == 2) {
        n = 2;
    }
    for (int j = 0; j < n; j++) {
        if (func_00147318(pi, j) == *(int*)((char*)self + 0x22C)) {
            *(int*)((char*)self + 0x1CC) = j;
            break;
        }
    }
    int idx = *(signed char*)((char*)self + 0x1CC);
    *(signed char*)((char*)self + 0x44) = idx;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045E250[];
extern int D_004A15E8;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001902C0(void* self);
extern "C" void cFEStateProfileLoad_initCreateScreen(void* self);
extern "C" void func_00190670(void* self);
extern "C" void func_00190768(void* self);

extern "C" void cFEStateProfileLoad_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045E250), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001902C0(self);
    switch (D_004A15E8) {
    case 1:
        cFEStateProfileLoad_initCreateScreen(self);
        break;
    case 2:
        func_00190670(self);
        break;
    case 3:
        func_00190768(self);
        break;
    }
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018EE98__FPv);
#ifdef SKIP_ASM
void* func_0018EE98(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018EEB8);
#ifdef SKIP_ASM
struct cList0018EEB8 {
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
    virtual int v54(int a);
};
extern "C" void* func_00227F80(void* app);
extern void* D_004A28A8;
extern int D_004A15E8;

extern "C" int func_0018EEB8(void* self, void* widget, unsigned int msg)
{
    char* app = (char*)func_00227F80(D_004A28A8);
    switch (msg) {
    case 6:
        if ((*(cList0018EEB8**)(app + 0x434))->v54(*(int*)(app + 0x428)) == 0) {
            return 0x100;
        }
        break;
    case 8:
        if (D_004A15E8 == 2 || *(int*)((char*)self + 0x220) == 0) {
            return 0x100;
        }
        break;
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018EF70);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F0B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_004A1458[];
extern char D_004A1460[];
extern int D_004A15E8;
struct cUIObj_F0B8 {
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
    virtual void setActive(int on);
};

extern "C" void func_0018F0B8(void* self, bool on)
{
    if (D_004A15E8 != 2) {
        int off = !on;
        ((cUIObj_F0B8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1458)))->setActive(off);
        ((cUIObj_F0B8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1460)))->setActive(off);
    }
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F168);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_004A15D0[];
extern char D_004A15D8[];
extern char D_004A15E0[];
extern int D_004A15E8;

struct cUIObj_F168 {
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
    virtual void setVisible(int on);
};

extern "C" void func_0018F168(void* self, bool a, bool b)
{
    cUIObj_F168* o = (cUIObj_F168*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15E0));
    if (D_004A15E8 != 2) {
        int v = !b;
        ((cUIObj_F168*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15D0)))->setVisible(v);
        *(int*)((char*)self + 0x224) = v;
        o->setVisible(v);
    } else {
        int v = !a;
        ((cUIObj_F168*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15D8)))->setVisible(v);
        *(int*)((char*)self + 0x224) = v;
        o->setVisible(v);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F278);
#ifdef SKIP_ASM
extern "C" void func_0018F2B8(void* self);
extern "C" void func_0018F9D8(void* self);

extern "C" void func_0018F278(void* self)
{
    if (*(int*)((char*)self + 0x214) == 3) {
        func_0018F2B8(self);
    } else {
        func_0018F9D8(self);
    }
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018F2B8);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F9D8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_0018F2B8(void* self);
extern "C" void* func_00227F80(void* app);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern void* D_004A28A8;
extern char D_0045E260[];

struct cSelf_F9D8 {
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
struct cCard_F9D8 {
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
struct cUIObj_F9D8 {
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

extern "C" void func_0018F9D8(void* p)
{
    cSelf_F9D8* self = (cSelf_F9D8*)p;
    void* app = func_00227F80(D_004A28A8);
    if (self->isReady() != 0) {
        if ((*(cCard_F9D8**)((char*)app + 0x434))->isPresent() != 0) {
            self->setState(3);
            func_0018F2B8(self);
            return;
        }
    }
    for (int i = 1; i < 7; i++) {
        char buf[64];
        sprintf(buf, D_0045E260, i);
        cUIObj_F9D8* o = (cUIObj_F9D8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        o->setSelected(0);
        o->setEnabled(1);
    }
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018FAF0);

extern "C" void* func_001D58B8(void*);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018FC70__FPv);
#ifdef SKIP_ASM
void func_0018FC70(void* self)
{
    func_001D58B8(self);
    *(int*)((char*)self + 0x1a4) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018FC98);
#ifdef SKIP_ASM
extern "C" void* func_00227F80(void* app);
extern "C" void func_0023CAE8(void* app, int a1, int a2);
extern "C" void func_00241E18(void*, unsigned short*, int);
void cMemMan_free(void* p);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern void* D_004A28A8;
extern char D_0045E2A0[];

struct cSelf_FC98 {
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
    virtual void v35();
    virtual void showMessage(unsigned short* text, int a2, int a3, int a4, int a5, int a6);
};

extern "C" void func_0018FC98(void* p)
{
    cSelf_FC98* self = (cSelf_FC98*)p;
    if (*(int*)((char*)self + 0x224) != 0) {
        func_0023CAE8(func_00227F80(D_004A28A8), 1, *(int*)((char*)self + 0x1CC));
        *(int*)((char*)self + 0x1C0) = 3;
        *(int*)((char*)self + 0x1A8) = 1;
        *(int*)((char*)self + 0x1DC) = 1;
        if (*(int*)((char*)self + 0x210) == 0 && *(int*)((char*)self + 0x204) == 0) {
            *(int*)((char*)self + 0x19C) = 0x16;
            unsigned short* buf = (unsigned short*)operator_new_tag(0x640, D_0045E2A0, 0x100, 0);
            func_00241E18(func_00227F80(D_004A28A8), buf, 0x320);
            self->showMessage(buf, 1, 1, 0, 0, 1);
            *(int*)((char*)self + 0x210) = 1;
            if (buf != 0) {
                cMemMan_free(buf);
            }
        }
    }
}
#endif

INCLUDE_ASM("fe/festateprofile", func_0018FD90);

INCLUDE_ASM("fe/festateprofile", func_001902C0);

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern char D_0045E318[];
extern char D_0045E348[];
extern char D_0045E368[];
extern char D_0045E378[];
extern char D_004A15D0[];
extern char D_004A1458[];

struct cUIObj_0540 {
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
    virtual void setActive(int on);
};

extern "C" void cFEStateProfileLoad_initCreateScreen(void* self)
{
    cUIObj_0540* t = (cUIObj_0540*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E318));
    t->setActive(1);
    cUIText_setUnicodeStringByID((cUIText*)t, GetHashValue32(D_0045E368));
    cUIObj_0540* u = (cUIObj_0540*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E348));
    u->setActive(1);
    cUIText_setUnicodeStringByID((cUIText*)u, GetHashValue32(D_0045E378));
    ((cUIObj_0540*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15D0)))->setActive(1);
    ((cUIObj_0540*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1458)))->setActive(1);
    *(int*)((char*)self + 0x220) = 1;
    *(int*)((char*)self + 0x224) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_00190670);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* func_00227F80(void* app);
extern void* D_004A28A8;
extern char D_0045E330[];
extern char D_0045E358[];
extern char D_004A15D8[];
extern char D_004A1460[];

struct cUIObj_0670 {
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
    virtual void setActive(int on);
};

struct sProfile_0670 {
    char pad[0x220];
    int f220;
    int f224;
};

extern "C" void func_00190670(void* self)
{
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E330)))->setActive(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E358)))->setActive(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15D8)))->setActive(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1460)))->setActive(0);
    ((sProfile_0670*)self)->f220 = 0;
    ((sProfile_0670*)self)->f224 = 1;
    *(int*)((char*)func_00227F80(D_004A28A8) + 0x11C) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_00190768);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_0045E318[];
extern char D_0045E348[];
extern char D_004A15D0[];
extern char D_004A1458[];
struct cUIObj_0768 {
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
    virtual void setActive(int on);
};

extern "C" void func_00190768(void* self)
{
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E318)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045E348)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15D0)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1458)))->setActive(1);
    *(int*)((char*)self + 0x220) = 1;
    *(int*)((char*)self + 0x224) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_00190858);
#ifdef SKIP_ASM
extern void* D_0046B160[];
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* func_00190858(void* self, int a1, int a2)
{
    signed char idx = a2;
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046B160;
    *(int*)((char*)self + 0xC) = 0xF;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    return self;
}
#endif


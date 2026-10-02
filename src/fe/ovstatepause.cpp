#include "common.h"

INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onCreateScreen);

INCLUDE_ASM("fe/ovstatepause", func_001FA100);

INCLUDE_ASM("fe/ovstatepause", func_001FA238);

INCLUDE_ASM("fe/ovstatepause", func_001FA9E0);

INCLUDE_ASM("fe/ovstatepause", func_001FAA78);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAAF0);
#ifdef SKIP_ASM
extern "C" int func_001FAAF0(void* self, int a1, int a2)
{
    return a2 != 4 ? 0x101 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAB08);
#ifdef SKIP_ASM
extern "C" int func_001FAB08(void* self, int a1, int a2)
{
    return a2 != 0 ? 0x101 : 0x100;
}
#endif

INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAF08__FPv);
#ifdef SKIP_ASM
int func_001FAF08(void* self)
{
    return 0x1;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FAF10);

INCLUDE_ASM("fe/ovstatepause", func_001FAFF8);

INCLUDE_ASM("fe/ovstatepause", func_001FB0C0);

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut);
#ifdef SKIP_ASM
struct cUIText;
struct sVE_FB180 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_0046FB18[];
extern char D_0046FB60[];

extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut(void* self)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046FB18));
        if (t != 0 && *(int*)((char*)self + 0xA0) != 0) {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0046FB60));
        }
        sVE_FB180* vt = *(sVE_FB180**)((char*)t + 0x8);
        vt[9].fn((char*)t + vt[9].delta, *(int*)((char*)self + 0xA0));
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_displayPingReceived);
#ifdef SKIP_ASM
struct cUIText;
struct sVE_FB218 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(void* self, int visible);
extern char D_0046FB18[];
extern char D_0046FB88[];

extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingReceived(void* self)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046FB18));
        if (t != 0 && *(int*)((char*)self + 0xA0) != 0) {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0046FB88));
        }
        sVE_FB218* vt = *(sVE_FB218**)((char*)t + 0x8);
        vt[9].fn((char*)t + vt[9].delta, *(int*)((char*)self + 0xA0));
        cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(self, 1);
    }
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FB2B8);

INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible);

INCLUDE_ASM("fe/ovstatepause", func_001FB458);

INCLUDE_ASM("fe/ovstatepause", func_001FB588);

INCLUDE_ASM("fe/ovstatepause", func_001FB6B8);

INCLUDE_ASM("fe/ovstatepause", func_001FBBD8);

INCLUDE_ASM("fe/ovstatepause", func_001FBCC8);

INCLUDE_ASM("fe/ovstatepause", func_001FBCE0);

INCLUDE_ASM("fe/ovstatepause", func_001FBD20);

INCLUDE_ASM("fe/ovstatepause", func_001FC768);

INCLUDE_ASM("fe/ovstatepause", func_001FC878);

INCLUDE_ASM("fe/ovstatepause", func_001FCEC0);

INCLUDE_ASM("fe/ovstatepause", func_001FD0D8);

INCLUDE_ASM("fe/ovstatepause", func_001FD118);

INCLUDE_ASM("fe/ovstatepause", func_001FD150);

INCLUDE_ASM("fe/ovstatepause", func_001FD190);

INCLUDE_ASM("fe/ovstatepause", func_001FD268);

INCLUDE_ASM("fe/ovstatepause", func_001FD320);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDB78);
#ifdef SKIP_ASM
extern "C" int func_001FDB78(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FDB88);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDBF0);
#ifdef SKIP_ASM
extern "C" void* func_001D5330(void* self, int a1, int a2, int a3);
extern void* D_00472440[];

extern "C" void* func_001FDBF0(void* self, int a1, int a2)
{
    func_001D5330(self, a1, 1, a2);
    *(void***)((char*)self + 0x8) = D_00472440;
    *(int*)((char*)self + 0x22C) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FDC30);

INCLUDE_ASM("fe/ovstatepause", func_001FDE60);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDF00);
#ifdef SKIP_ASM
extern "C" void func_0039F718(void* self);

extern "C" void func_001FDF00(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FDF40);

INCLUDE_ASM("fe/ovstatepause", func_001FE4A8);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE5B0);
#ifdef SKIP_ASM
class cUIObj_1FE5B0 {
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
extern char D_00470188[];
extern char D_004A20A0[];

extern "C" void func_001FE5B0(void* self, bool on)
{
    ((cUIObj_1FE5B0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00470188)))->setVisible(!on);
    ((cUIObj_1FE5B0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A20A0)))->setVisible(!on);
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE648);
#ifdef SKIP_ASM
class cUIObj_1FE648 {
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

extern "C" void func_001FE648(void* self, int a1, bool on)
{
    cUIObj_1FE648* a = (cUIObj_1FE648*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2090));
    ((cUIObj_1FE648*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2568)))->setVisible(!on);
    *(int*)((char*)self + 0x224) = !on;
    a->setVisible(!on);
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FE6E8);

INCLUDE_ASM("fe/ovstatepause", func_001FE8F0);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE968);
#ifdef SKIP_ASM
extern "C" void* func_001D5330(void* self, int a1, int a2, int a3);
extern void* D_004722B8[];

extern "C" void* func_001FE968(void* self, int a1, int a2)
{
    func_001D5330(self, a1, 2, a2);
    *(void***)((char*)self + 0x8) = D_004722B8;
    *(int*)((char*)self + 0x22C) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FE9A8);

extern "C" void* func_001D58B8(void* self);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FEC40__FPv);
#ifdef SKIP_ASM
void* func_001FEC40(void* self)
{
    return func_001D58B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FEC60);
#ifdef SKIP_ASM
extern "C" void func_0039F718(void* self);

extern "C" void func_001FEC60(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FECA0);

INCLUDE_ASM("fe/ovstatepause", func_001FF170);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF278);
#ifdef SKIP_ASM
class cUIObj_1FF278 {
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
extern char D_00470188[];
extern char D_004A20A0[];

extern "C" void func_001FF278(void* self, bool on)
{
    ((cUIObj_1FF278*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00470188)))->setVisible(!on);
    ((cUIObj_1FF278*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A20A0)))->setVisible(!on);
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF310);
#ifdef SKIP_ASM
class cUIObj_1FF310 {
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

extern "C" void func_001FF310(void* self, int a1, bool on)
{
    cUIObj_1FF310* a = (cUIObj_1FF310*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2090));
    ((cUIObj_1FF310*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2568)))->setVisible(!on);
    *(int*)((char*)self + 0x224) = !on;
    a->setVisible(!on);
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FF3B0);

INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF6D0);
#ifdef SKIP_ASM
extern "C" void func_0039E4C0(void* self);
extern "C" void func_001FFD08(void* self, int a1);

extern "C" void func_001FF6D0(void* self)
{
    func_0039E4C0(self);
    func_001FFD08(self, 0);
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FF700);

INCLUDE_ASM("fe/ovstatepause", func_001FF748);

INCLUDE_ASM("fe/ovstatepause", func_001FF7B8);

INCLUDE_ASM("fe/ovstatepause", func_001FFD08);

INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onGainTransition);


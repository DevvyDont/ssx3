#include "common.h"

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_0045DB80[];
extern char D_0045DB90[];
extern char D_0045DBA0[];
extern char D_0045DBB0[];

struct sVEntryK185A98 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cFEStateCredits_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045DB80), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    char* o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045DB90));
    if (o != 0)
        *(int*)(o + 0x90) |= 8;
    char* t = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045DBA0));
    *(void**)((char*)self + 0x58) = t;
    if (t != 0) {
        sVEntryK185A98* vt = *(sVEntryK185A98**)(t + 8);
        vt[9].fn(t + vt[9].delta, 0);
    }
    t = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045DBB0));
    *(void**)((char*)self + 0x58) = t;
    if (t != 0) {
        sVEntryK185A98* vt = *(sVEntryK185A98**)(t + 8);
        vt[9].fn(t + vt[9].delta, 0);
    }
}
#endif

INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onGainFocus);

INCLUDE_ASM("fe/festatecredits", func_00185F40);

//100%
INCLUDE_ASM("fe/festatecredits", func_001863B8);
#ifdef SKIP_ASM
extern "C" void* func_0039E510(void* self);

extern "C" void func_001863B8(void* self)
{
    *(int*)((char*)self + 0x60) += *(int*)((char*)self + 0x64);
    func_0039E510(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", func_001863E8);
#ifdef SKIP_ASM
struct sVEntry001863E8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_001863E8(void* self, void* pad)
{
    sVEntry001863E8* e = &(*(sVEntry001863E8**)((char*)pad + 8))[17];
    if (e->fn((char*)pad + e->delta)) {
        if (*(int*)((char*)self + 0x64) >= -7) {
            *(int*)((char*)self + 0x64) -= 1;
        }
        return 1;
    }
    e = &(*(sVEntry001863E8**)((char*)pad + 8))[18];
    if (e->fn((char*)pad + e->delta)) {
        if (*(int*)((char*)self + 0x64) < 8) {
            *(int*)((char*)self + 0x64) += 1;
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", func_00186478);
#ifdef SKIP_ASM
extern "C" int func_00186478(void* self, int a1, unsigned int a2)
{
    switch (a2) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", func_001864B0);
#ifdef SKIP_ASM
struct sVEntry001864B0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void func_0039F400(void* list, void* item);

extern "C" void func_001864B0(void* self, void* item, int msg)
{
    if (item != 0 && msg == 6) {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001864B0* vt = *(sVEntry001864B0**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, 0x24);
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", func_00186518);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern char D_004A1398[];
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" signed char func_001A06F0(void* self, int a1);
extern "C" void func_0039E4C0(void* self, void* a1);
// NOTE: placeholder for the object at $gp+0x1D90 (no symbol in the target)
struct sFlowState_865A8 {
    signed char count;
    char* states;
};
extern sFlowState_865A8 D_004A4E80;

extern "C" void func_00186518(void* self, void* a1)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    if (screen != 0) {
        void* menu = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004A1398));
        if (menu != 0) {
            cUIMenu_setSelectedByIndex(menu, func_001A06F0(&D_004A4E80, *(signed char*)((char*)self + 0xC)));
        }
    }
    func_0039E4C0(self, a1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", func_001865A8);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern char D_004A1398[];
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_001A0708(void* self, int a1, int a2);
// NOTE: placeholder for the object at $gp+0x1D90 (no symbol in the target)
extern sFlowState_865A8 D_004A4E80;

extern "C" void func_001865A8(void* self)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    if (screen != 0) {
        void* obj = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004A1398));
        if (obj != 0) {
            func_001A0708(&D_004A4E80, *(signed char*)((char*)self + 0xC), *(signed char*)((char*)obj + 0x95));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", func_00186610);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" void func_002006B8(void* self);
extern void* D_0046BD28[];

extern "C" void* func_00186610(void* self)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 0x13;
    *(void***)((char*)self + 0x8) = D_0046BD28;
    func_002006B8((char*)self + 0x48);
    return self;
}
#endif


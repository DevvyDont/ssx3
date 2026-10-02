#include "common.h"

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004679A0[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFEStateNetHelpPopup_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004679A0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001D9E20);
#ifdef SKIP_ASM
struct sVEntry001D9E20 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" int func_001D9E20(void* self, int a1, int a2)
{
    if (*(int*)((char*)self + 0x50) != 0) {
        void* obj = *(void**)((char*)self + 0x20);
        if (obj != 0) {
            sVEntry001D9E20* vt = *(sVEntry001D9E20**)((char*)obj + 8);
            return vt[24].fn((char*)obj + vt[24].delta, a1, a2);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001D9E68);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" float func_00320BF0(void* self, int id);
extern "C" void func_0039F190(void*, int);

extern "C" void func_001D9E68(void* self)
{
    int i;
    int found = 0;
    for (i = 0; i < 2; i++) {
        int** p = *(int***)((char*)D_004A28A8 + (i << 2) + 0xB0);
        int ok = *p != 0 && **p != 0;
        if (ok && func_00320BF0(p, 0x99) != 0.0f) {
            found = 1;
        }
    }
    if (found == 0) {
        func_0039F190((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001D9F30__FPv);
#ifdef SKIP_ASM
int func_001D9F30(void* self)
{
    return 0x1;
}
#endif

INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onWidgetCreate);

INCLUDE_ASM("fe/festatenethelppopup", func_001DA110);

INCLUDE_ASM("fe/festatenethelppopup", func_001DA238);

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA478);
#ifdef SKIP_ASM
extern "C" void func_0039F190(void*, int);

extern "C" void func_001DA478(void* self, int a1, int msg)
{
    if (msg == 6) {
        func_0039F190((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA4A8);
#ifdef SKIP_ASM
struct sNetHelpPopup_DA4A8 {
    char pad_0x0[0x48];
    int state;      // 0x48
    char pad_0x4C[0x8];
    int f54;        // 0x54
    int c[3];       // 0x58
    int f64;        // 0x64
    int a[7];       // 0x68
    int b[7];       // 0x84
};

extern "C" void func_001DA4A8(void* self)
{
    sNetHelpPopup_DA4A8* s = (sNetHelpPopup_DA4A8*)self;
    int i;
    s->state = 0;
    s->f54 = 0;
    for (i = 2; i >= 0; i--) {
        s->c[i] = 0;
    }
    s->f64 = 0;
    for (i = 0; i < 7; i++) {
        s->a[i] = 0;
        s->b[i] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA510__FPviii);
#ifdef SKIP_ASM
struct sNetHelpPopup_DA510 {
    char pad[0x68];
    int a[7];
    int b[7];
};

void func_001DA510(void* self, int i, int a, int b)
{
    sNetHelpPopup_DA510* s = (sNetHelpPopup_DA510*)self;
    s->a[i] = a;
    s->b[i] = b;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA528__FPvi);
#ifdef SKIP_ASM
void func_001DA528(void* self, int val)
{
    *(int*)((char*)self + 0x48) = val;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA530__FPvii);
#ifdef SKIP_ASM
void func_001DA530(void* self, int i, int value)
{
    *(int*)((char*)self + (i << 2) + 0x58) = value;
}
#endif

INCLUDE_ASM("fe/festatenethelppopup", func_001DA648);

INCLUDE_ASM("fe/festatenethelppopup", func_001DA7E8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatenethelppopup", func_001DA988);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* func_001DA648(void* self, void* a1, void* a2);
extern "C" void* func_001DA7E8(void* self, void* a1, void* a2);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D638[];
extern char D_00467A20[];
extern char D_00467A58[];
extern char D_00467A60[];

extern "C" void* func_001DA988(void* self, void* a1, void* a2)
{
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0046D638;
    func_001DA648((char*)self + 0x130, a1, D_00467A20);
    func_001DA7E8((char*)self + 0x32C, a2, D_00467A58);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x528), D_00467A60);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x544), (void*)0xA);
    cSubMenuItem_cSubMenuItem((char*)self + 0x55C, (char*)self + 0x130, D_00467A20);
    cSubMenuItem_cSubMenuItem((char*)self + 0x578, (char*)self + 0x32C, D_00467A58);
    cMenu_addItem(self, (char*)self + 0x528, -1);
    cMenu_addItem(self, (char*)self + 0x544, -1);
    cMenu_addItem(self, (char*)self + 0x55C, -1);
    cMenu_addItem(self, (char*)self + 0x578, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DAAC8);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CE418(void* self, void* a1, const char* name);
extern "C" void* func_002CCF98(void* self, int a1, void* a2, int a3);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D588[];
extern char D_00467A70[];
extern char D_00467A80[];
extern char D_004A1408[];
extern char D_00467A90[];

extern "C" void* func_001DAAC8(void* self, int* info)
{
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0046D588;
    *(int*)((char*)self + 0x130) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), D_00467A70);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)0xE);
    func_002CE418((char*)self + 0x168, D_00467A80, D_004A1408);
    func_002CCF98((char*)self + 0x194, 0, D_00467A90, 0);
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    cMenu_addItem(self, (char*)self + 0x168, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DABD0);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CCF08(void* self);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D5E0[];
extern char D_00467A98[];

extern "C" void* func_001DABD0(void* self, int* info)
{
    char* p = (char*)self + 0x168;
    int i = 3;
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0046D5E0;
    *(int*)((char*)self + 0x130) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), D_00467A98);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)0xE);
    for (; i != -1; i--) {
        func_002CCF08(p);
        p += 0x24;
    }
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    int j;
    for (j = 0; j < 4; j++) {
        cMenu_addItem(self, (char*)self + 0x168 + j * 0x24, -1);
    }
    return self;
}
#endif

INCLUDE_ASM("fe/festatenethelppopup", func_001DACB8);

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DAE20);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CCF08(void* self);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D4D8[];
extern char D_00467AB8[];

extern "C" void* func_001DAE20(void* self, int* info)
{
    char* p = (char*)self + 0x168;
    int i = 11;
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0046D4D8;
    *(int*)((char*)self + 0x130) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), D_00467AB8);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)0xE);
    for (; i != -1; i--) {
        func_002CCF08(p);
        p += 0x24;
    }
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    int j;
    for (j = 0; j < 12; j++) {
        cMenu_addItem(self, (char*)self + 0x168 + j * 0x24, -1);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DAF08);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CCF08(void* self);
extern "C" void* func_002CCF98(void* self, int a1, void* a2, int a3);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D480[];
extern char D_00467AC8[];
extern char D_00467AD8[];

extern "C" void* func_001DAF08(void* self, int* a, int* b)
{
    char* p = (char*)self + 0x16C;
    int i = 0x17;
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0046D480;
    *(int*)((char*)self + 0x130) = *a;
    *(int*)((char*)self + 0x134) = *b;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x138), D_00467AC8);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x154), (void*)0xE);
    for (; i != -1; i--) {
        func_002CCF08(p);
        p += 0x24;
    }
    func_002CCF98((char*)self + 0x4CC, -2, D_00467AD8, 0);
    cMenu_addItem(self, (char*)self + 0x138, -1);
    cMenu_addItem(self, (char*)self + 0x154, -1);
    int j;
    for (j = 0; j < 0x18; j++) {
        cMenu_addItem(self, (char*)self + 0x16C + j * 0x24, -1);
    }
    cMenu_addItem(self, (char*)self + 0x4CC, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DB028);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CCF08(void* self);
extern "C" void* func_002CE418(void* self, void* a1, const char* name);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D428[];
extern char D_00467AE0[];
extern char D_00467AF0[];
extern char D_004A1408[];
extern int D_00467D70[];

extern "C" void* func_001DB028(void* self, int* info)
{
    char* p = (char*)self + 0x194;
    int i = 0xD;
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0046D428;
    *(int*)((char*)self + 0x130) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), D_00467AE0);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)0xE);
    func_002CE418((char*)self + 0x168, D_00467AF0, D_004A1408);
    for (; i != -1; i--) {
        func_002CCF08(p);
        p += 0x24;
    }
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    int j;
    for (j = 0; j < 0xE; j++) {
        char* item = (char*)self + 0x194 + j * 0x24;
        *(int*)(item + 0x8) = j;
        *(int*)(item + 0x14) = D_00467D70[j];
        cMenu_addItem(self, item, -1);
    }
    return self;
}
#endif


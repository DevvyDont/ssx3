#include "common.h"

INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_Load3PeakPic);

INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_Load1PeakPic);

INCLUDE_ASM("fe/feasyncfile", func_001A37F8);

INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_UnloadFEAsyncFile);

struct cFEAsyncFileEntry {
    char pad_0x00[0x114];
    int mStatus; // 0x114
    char pad_0x118[0x11C - 0x114 - 4];
};

struct cFEAsyncManager {
    cFEAsyncFileEntry mFiles[1];
};

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_GetFileStatus__FP15cFEAsyncManageri);
#ifdef SKIP_ASM
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index)
{
    return self->mFiles[index].mStatus;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_SetFileStatus__FP15cFEAsyncManagerii);
#ifdef SKIP_ASM
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status)
{
    self->mFiles[index].mStatus = status;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A39F0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3BF0);
#ifdef SKIP_ASM
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index);

extern "C" int func_001A3BF0(cFEAsyncManager* self)
{
    int i;
    for (i = 0; i < 5; i++) {
        if (cFEAsyncManager_GetFileStatus(self, i) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A3C50);

INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_LoadDataFile);

INCLUDE_ASM("fe/feasyncfile", func_001A3DC8);

INCLUDE_ASM("fe/feasyncfile", func_001A3E30);

INCLUDE_ASM("fe/feasyncfile", func_001A3EA0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3F68);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A5898(void* self);
extern "C" void func_001A6978(void* self);
extern char D_00461448[];

extern "C" void func_001A3F68(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00461448), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_001A5898(self);
        func_001A6978(self);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A3FE0);

INCLUDE_ASM("fe/feasyncfile", func_001A4040);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4170);
#ifdef SKIP_ASM
struct sVE1A4170 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001A6C40(void* self, int a1);

extern "C" int func_001A4170(void* self, void* obj)
{
    int r;
    sVE1A4170* vt = *(sVE1A4170**)((char*)obj + 0x8);
    if (vt[27].fn((char*)obj + vt[27].delta) == 0) {
        r = 0;
    } else {
        func_001A6C40(self, *(int*)((char*)self + 0x6F8));
        r = 1;
    }
    return r;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A41C0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4300);
#ifdef SKIP_ASM
extern "C" void func_001A6978(void* self);

extern "C" int func_001A4300(void* self, int a1, int a2, int a3)
{
    if (a2 == 4) {
        *(int*)((char*)self + 0x6F4) = a3;
        func_001A6978(self);
        return 0;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A4330);

INCLUDE_ASM("fe/feasyncfile", func_001A4670);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4740);
#ifdef SKIP_ASM
extern "C" int func_001A4740(void* self, int a1)
{
    return *(int*)((char*)self + 0x6f4) + a1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A4750);

INCLUDE_ASM("fe/feasyncfile", func_001A4810);

INCLUDE_ASM("fe/feasyncfile", func_001A4928);

INCLUDE_ASM("fe/feasyncfile", func_001A49A0);

INCLUDE_ASM("fe/feasyncfile", func_001A4B20);

INCLUDE_ASM("fe/feasyncfile", func_001A4C58);

INCLUDE_ASM("fe/feasyncfile", func_001A4CE8);

INCLUDE_ASM("fe/feasyncfile", func_001A4D90);

INCLUDE_ASM("fe/feasyncfile", func_001A4E28);

INCLUDE_ASM("fe/feasyncfile", func_001A4FB0);

INCLUDE_ASM("fe/feasyncfile", func_001A5010);

INCLUDE_ASM("fe/feasyncfile", func_001A5128);

INCLUDE_ASM("fe/feasyncfile", func_001A52B0);

INCLUDE_ASM("fe/feasyncfile", func_001A5450);

INCLUDE_ASM("fe/feasyncfile", func_001A54D8);

INCLUDE_ASM("fe/feasyncfile", func_001A56D0);

INCLUDE_ASM("fe/feasyncfile", func_001A5800);

INCLUDE_ASM("fe/feasyncfile", func_001A5898);

INCLUDE_ASM("fe/feasyncfile", func_001A5FB8);

INCLUDE_ASM("fe/feasyncfile", func_001A6070);

INCLUDE_ASM("fe/feasyncfile", func_001A63C8);

INCLUDE_ASM("fe/feasyncfile", func_001A6618);

INCLUDE_ASM("fe/feasyncfile", func_001A66A8);

INCLUDE_ASM("fe/feasyncfile", func_001A6978);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6C40);
#ifdef SKIP_ASM
extern char D_00461590[];
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

extern "C" void func_001A6C40(void* self, int idx)
{
    if (*(void**)((char*)self + 0x40) != 0 && idx >= 0) {
        void* menu = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00461590));
        if (menu != 0) {
            cUIMenu_setSelectedByIndex(menu, idx);
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A6CA0);

INCLUDE_ASM("fe/feasyncfile", func_001A6E70);

INCLUDE_ASM("fe/feasyncfile", func_001A71B0);

INCLUDE_ASM("fe/feasyncfile", func_001A7390);

INCLUDE_ASM("fe/feasyncfile", func_001A77A0);

INCLUDE_ASM("fe/feasyncfile", func_001A7848);

INCLUDE_ASM("fe/feasyncfile", func_001A7970);

extern "C" void* func_001A8770(void* self);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7B20__FPv);
#ifdef SKIP_ASM
void* func_001A7B20(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A7B40);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7BB0__FPv);
#ifdef SKIP_ASM
void func_001A7BB0(void* self)
{
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A7BB8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7D10);
#ifdef SKIP_ASM
extern "C" void func_001A81F0(void* self);

extern "C" int func_001A7D10(void* self, int a1, int a2, int a3)
{
    if (a2 == 4) {
        *(int*)((char*)self + 0x6D4) = a3;
        func_001A81F0(self);
        return 0;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7D40);
#ifdef SKIP_ASM
extern "C" void func_001A7F78(void* self);
extern "C" void func_001A81F0(void* self);
extern "C" void* func_001A8E40(void* self);
extern "C" void* func_001A7D40(void* self, int msg)
{
    if (msg == 0x109) {
        func_001A7F78(self);
        func_001A81F0(self);
        return 0;
    }
    return func_001A8E40(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7D90);
#ifdef SKIP_ASM
class cFEVObj_001A7D90 {
public:
    int field_0x0;
    int field_0x4;
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
};

extern "C" void func_001A7F78(void* self);
extern "C" void func_001A81F0(void* self);

extern "C" int func_001A7D90(cFEVObj_001A7D90* self)
{
    self->v32();
    func_001A7F78(self);
    func_001A81F0(self);
    return 1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A7DD8);

INCLUDE_ASM("fe/feasyncfile", func_001A7ED8);

INCLUDE_ASM("fe/feasyncfile", func_001A7F78);

INCLUDE_ASM("fe/feasyncfile", func_001A81F0);

INCLUDE_ASM("fe/feasyncfile", func_001A83D8);

INCLUDE_ASM("fe/feasyncfile", func_001A8500);

INCLUDE_ASM("fe/feasyncfile", func_001A85D0);

INCLUDE_ASM("fe/feasyncfile", func_001A86E8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8768__FPv);
#ifdef SKIP_ASM
void func_001A8768(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8770);
#ifdef SKIP_ASM
class cFEAsync1A8770 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
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
};

extern "C" void func_0039E510(void*);
extern "C" void* func_001A8B88(void* self, int size, int a2);

extern "C" void* func_001A8770(void* self)
{
    ((cFEAsync1A8770*)self)->v27();
    ((cFEAsync1A8770*)self)->v28();
    func_0039E510(self);
    return func_001A8B88(self, 0x114, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A87D0);
#ifdef SKIP_ASM
struct sVE1A87D0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0039E4C0(void* self, int a1);

extern "C" void func_001A87D0(void* self, int a1)
{
    sVE1A87D0* vt = *(sVE1A87D0**)((char*)self + 0x8);
    vt[32].fn((char*)self + vt[32].delta);
    func_0039E4C0(self, a1);
    *(int*)((char*)self + 0x48) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8828__FPv);
#ifdef SKIP_ASM
int func_001A8828(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A8830);

INCLUDE_ASM("fe/feasyncfile", func_001A8918);

INCLUDE_ASM("fe/feasyncfile", func_001A89B0);

INCLUDE_ASM("fe/feasyncfile", func_001A8A40);

INCLUDE_ASM("fe/feasyncfile", func_001A8AE8);

INCLUDE_ASM("fe/feasyncfile", func_001A8B88);

INCLUDE_ASM("fe/feasyncfile", func_001A8E40);

INCLUDE_ASM("fe/feasyncfile", func_001A8F98);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9090);
#ifdef SKIP_ASM
extern "C" void func_001A91B0(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_001CCF00(void* p, int a);
extern "C" void func_001CB418(void* p, int a);

extern "C" void func_001A9090(void* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    func_001A91B0(self, a1, a4, a5, a6);
    func_001CCF00(*(void**)((char*)self + 0x69C), a2);
    func_001CB418(*(void**)((char*)self + 0x69C), a3);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9150);
#ifdef SKIP_ASM
extern "C" void func_001A91B0(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_001CD020(void* p, int a);
extern "C" void func_001CB418(void* p, int a);

extern "C" void func_001A9150(void* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    func_001A91B0(self, a1, a4, a5, a6);
    func_001CD020(*(void**)((char*)self + 0x69C), a2);
    func_001CB418(*(void**)((char*)self + 0x69C), a3);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A91B0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9268);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B0F60(void* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern const char D_00461C60[];

extern "C" void func_001A9268(void* self, int a1, int a2)
{
    if (a2 == 0xF) {
        func_001A93D0_3(self, a1, 0xF);
        func_001A8A40(self, func_001B0F60(cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), *(int*)((char*)self + 0x10)));
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A92D8);

INCLUDE_ASM("fe/feasyncfile", func_001A93D0);

INCLUDE_ASM("fe/feasyncfile", func_001A9438);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A94D8__FPv);
#ifdef SKIP_ASM
void func_001A94D8(void* self)
{
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A94E0);

INCLUDE_ASM("fe/feasyncfile", func_001A9578);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9638);
#ifdef SKIP_ASM
extern "C" void func_001A93D0(void* self, int a1, int a2, int a3);

extern "C" void func_001A9638(void* self, int a1, int a2, int a3)
{
    switch (a2) {
    case 0xF:
        break;
    case 0x14:
        break;
    default:
        func_001A93D0(self, a1, a2, a3);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A9668);

INCLUDE_ASM("fe/feasyncfile", func_001A9710);

INCLUDE_ASM("fe/feasyncfile", func_001A97B8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9890);
#ifdef SKIP_ASM
extern "C" void* cBXString_cBXString4(void* self, const char* str);

extern "C" void func_001A9890(void* self, const char* str)
{
    cBXString_cBXString4((char*)self + 0x6C0, str);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A98B0__FPv);
#ifdef SKIP_ASM
int func_001A98B0(void* self)
{
    return *(int*)((char*)self + 0x6C0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A98B8);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

struct sFEPassword1A98B8 {
    char pad[0x5C];
    char text[0x40];    // 0x5C
};

extern "C" void func_001A98B8(void* self, const char* str)
{
    sFEPassword1A98B8* s = (sFEPassword1A98B8*)self;
    int len = strlen(str);
    int i;
    for (i = 0; i < len && i < 0x3F; i++) {
        s->text[i] = '*';
    }
    if (len == 0) {
        s->text[0] = ' ';
        len = 1;
    }
    s->text[len] = 0;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A9930);

INCLUDE_ASM("fe/feasyncfile", func_001A9A18);

INCLUDE_ASM("fe/feasyncfile", func_001A9AB8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9B58);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001A3DC8(void* mem, void* engine, void* owner);
extern "C" void func_0039F290(void* list, void* item);
extern const char D_00461D40[];

extern "C" void func_001A9B58(void* self)
{
    void* p = func_001A3DC8(cMemMan_alloc(0x6FC, D_00461D40, 0x100, 0), *(void**)((char*)self + 0x10), self);
    *(int*)((char*)p + 0x18) = 0x833;
    func_0039F290((char*)*(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, p);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A9BC0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9C50);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cScreenPopup_cScreenPopup(void* mem, void* engine, void* owner);
extern "C" void func_001C5630(void* self);
extern const char D_00461D58[];

extern "C" void* func_001A9C50(void* self)
{
    void* popup;
    void* cur = *(void**)((char*)self + 0x6A0);
    if (cur == 0) {
        popup = cScreenPopup_cScreenPopup(cMemMan_alloc(0x360, D_00461D58, 0x100, 0), *(void**)((char*)self + 0x10), self);
        *(void**)((char*)self + 0x6A0) = popup;
        *(int*)((char*)popup + 0x178) = 0;
    } else {
        func_001C5630((char*)cur + 0xBC);
        *(int*)((char*)*(void**)((char*)self + 0x6A0) + 0x178) = 0;
    }
    return *(void**)((char*)self + 0x6A0);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A9CC0);

INCLUDE_ASM("fe/feasyncfile", func_001AA9B0);

INCLUDE_ASM("fe/feasyncfile", func_001AAB68);

INCLUDE_ASM("fe/feasyncfile", func_001AAD50);

INCLUDE_ASM("fe/feasyncfile", func_001AAF90);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB108);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
extern char D_00462750[];

extern "C" void func_001AB108(void* self)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0x170) = 0;
    *(int*)((char*)p + 0x150) = 0x836;
    *(int*)((char*)p + 0x14C) = 6;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462750);
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x164) = 0;
    func_001A9BC0(self, p);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AB178);

INCLUDE_ASM("fe/feasyncfile", func_001AB288);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB370);
#ifdef SKIP_ASM
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001AB478(void* self, void* popup, int a1, int a2, int a3);

extern "C" void func_001AB370(void* self, int a1, int a2)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)self + 0x6C4) = 1;
    func_001AB478(self, p, a1, a2, 0);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AB3D0);

INCLUDE_ASM("fe/feasyncfile", func_001AB478);

INCLUDE_ASM("fe/feasyncfile", func_001ABB30);

INCLUDE_ASM("fe/feasyncfile", func_001ABC50);

INCLUDE_ASM("fe/feasyncfile", func_001ABCD0);

INCLUDE_ASM("fe/feasyncfile", func_001ABE58);

INCLUDE_ASM("fe/feasyncfile", func_001ABED8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABF70);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void* self);
extern void* D_0046B6F8[];

extern "C" void* func_001ABF70(void* self)
{
    func_001A8500(self);
    *(void***)((char*)self + 0x8) = D_0046B6F8;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABFA8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A1A78[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void* func_001ABFA8(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A1A78), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        screen = cUIScreen_playFrame(screen, 0, 0);
    }
    return screen;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AC010);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC068);
#ifdef SKIP_ASM
extern void* D_00469068[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001AC068(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00469068;
    cBXString__cBXString((char*)self + 0x6DC, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AC0B8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC150__FPv);
#ifdef SKIP_ASM
void* func_001AC150(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AC170);

INCLUDE_ASM("fe/feasyncfile", func_001AC2B8);

INCLUDE_ASM("fe/feasyncfile", func_001AC360);

INCLUDE_ASM("fe/feasyncfile", func_001AC408);

INCLUDE_ASM("fe/feasyncfile", func_001AC448);

INCLUDE_ASM("fe/feasyncfile", func_001AC4F0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC5A8);
#ifdef SKIP_ASM
extern "C" int func_001AC5A8(void* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AC5C0);

INCLUDE_ASM("fe/feasyncfile", func_001AC920);

INCLUDE_ASM("fe/feasyncfile", func_001AC9C0);

INCLUDE_ASM("fe/feasyncfile", func_001ACA88);

INCLUDE_ASM("fe/feasyncfile", func_001ACB48);

INCLUDE_ASM("fe/feasyncfile", func_001ACCD0);

INCLUDE_ASM("fe/feasyncfile", func_001ACDD0);

INCLUDE_ASM("fe/feasyncfile", func_001ACEF8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD130);
#ifdef SKIP_ASM
extern void* D_00468F50[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001AD130(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x38;
    *(void***)((char*)self + 0x8) = D_00468F50;
    *(int*)((char*)self + 0x6D0) = 0;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001AD178);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern char D_00462DC8[];

extern "C" void func_001AD178(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00462DC8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001A8768(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD1E8__FPv);
#ifdef SKIP_ASM
void* func_001AD1E8(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AD208);

INCLUDE_ASM("fe/feasyncfile", func_001AD308);

INCLUDE_ASM("fe/feasyncfile", func_001AD3E8);

INCLUDE_ASM("fe/feasyncfile", func_001AD4F0);

INCLUDE_ASM("fe/feasyncfile", func_001AD5E8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD8C0);
#ifdef SKIP_ASM
class cFEVObj_001AD8C0 {
public:
    int field_0x0;
    int field_0x4;
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
    virtual void v33(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001AD930(void* self, void* a1, int a2);

extern "C" void func_001AD8C0(cFEVObj_001AD8C0* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x816:
        self->v33(a1, a2);
        break;
    case 0x819:
        func_001AD930(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AD930);

INCLUDE_ASM("fe/feasyncfile", func_001AD9C8);

INCLUDE_ASM("fe/feasyncfile", func_001ADA30);

INCLUDE_ASM("fe/feasyncfile", func_001ADC30);

INCLUDE_ASM("fe/feasyncfile", func_001ADDD0);

INCLUDE_ASM("fe/feasyncfile", func_001ADE88);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ADFB8);
#ifdef SKIP_ASM
class func_001ADFB8_cObj {
public:
    char pad[0x8];
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
};

extern "C" int func_001ADFB8(func_001ADFB8_cObj* self)
{
    self->v32();
    return 1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001ADFE8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AE0E0__FPv);
#ifdef SKIP_ASM
void* func_001AE0E0(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AE100);

INCLUDE_ASM("fe/feasyncfile", func_001AE128);

INCLUDE_ASM("fe/feasyncfile", func_001AE680);

INCLUDE_ASM("fe/feasyncfile", func_001AE7A0);

INCLUDE_ASM("fe/feasyncfile", func_001AEBF0);

INCLUDE_ASM("fe/feasyncfile", func_001AEC60);

INCLUDE_ASM("fe/feasyncfile", func_001AF098);

INCLUDE_ASM("fe/feasyncfile", func_001AF208);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF3E0);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_0039F698(void* p);

extern "C" int func_001AF3E0(void* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        return func_0039F698(*(char**)((char*)self + 0x10) + 0x18);
    }
    return func_001A97B8(self, a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF428);
#ifdef SKIP_ASM
class cFEVObj_001AF428 {
public:
    int field_0x0;
    int field_0x4;
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
    virtual void v33(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001AF428(cFEVObj_001AF428* self, void* a1, int a2)
{
    if (*(int*)((char*)a1 + 0x18) == 0x835) {
        self->v33(a1, a2);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AF470);

INCLUDE_ASM("fe/feasyncfile", func_001AF568);

INCLUDE_ASM("fe/feasyncfile", func_001AF688);

INCLUDE_ASM("fe/feasyncfile", func_001AF728);

INCLUDE_ASM("fe/feasyncfile", func_001AF930);

INCLUDE_ASM("fe/feasyncfile", func_001AFA60);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001AFD88);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void func_001AAD50(void* self, int a1, int a2);
extern char D_00463538[];
extern char D_004635C0[];

extern "C" void func_001AFD88(void* self, void* a1)
{
    cUIText* text = cUIScreen_getObjectByHashName(self, GetHashValue32(D_00463538));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004635C0));
        func_001AAD50(self, 0, *(int*)((char*)a1 + 0xC));
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AFE08);

INCLUDE_ASM("fe/feasyncfile", func_001AFE68);

INCLUDE_ASM("fe/feasyncfile", func_001B00B8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0130__FPv);
#ifdef SKIP_ASM
void* func_001B0130(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B0150);

INCLUDE_ASM("fe/feasyncfile", func_001B0538);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B05E0);
#ifdef SKIP_ASM
class cFEVObj_001B05E0 {
public:
    int field_0x0;
    int field_0x4;
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
    virtual void v33(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001B05E0(cFEVObj_001B05E0* self, void* a1, int a2)
{
    if (*(int*)((char*)a1 + 0x18) == 0x835) {
        self->v33(a1, a2);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0628);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_0039F698(void* p);

extern "C" int func_001B0628(void* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        return func_0039F698(*(char**)((char*)self + 0x10) + 0x18);
    }
    return func_001A97B8(self, a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0670);
#ifdef SKIP_ASM
class cFEVObj_001B0670 {
public:
    int field_0x0;
    int field_0x4;
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
};

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001B0850(void* self, void* iface, void* screen);

extern "C" int func_001B0670(cFEVObj_001B0670* self, int a1)
{
    self->v32();
    if (a1 != 0) {
        func_001B0850(self, cBE_getInterface_Fv(cBE_getBE(), 0), *(void**)((char*)self + 0x40));
    }
    return 1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B06E0);

INCLUDE_ASM("fe/feasyncfile", func_001B0708);

INCLUDE_ASM("fe/feasyncfile", func_001B0850);

INCLUDE_ASM("fe/feasyncfile", func_001B0C08);

INCLUDE_ASM("fe/feasyncfile", func_001B0E10);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0F28);
#ifdef SKIP_ASM
extern signed char D_00440F68[];

extern "C" int func_001B0F28(int c)
{
    int i;
    for (i = 0; i < 10; i++) {
        if (c == D_00440F68[i]) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0F60);
#ifdef SKIP_ASM
extern void* D_00468C10[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001B0F60(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(void***)((char*)self + 0x8) = D_00468C10;
    *(int*)((char*)self + 0x6DC) = 1;
    *(int*)((char*)self + 0xC) = 0x41;
    *(int*)((char*)self + 0x6D8) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B0FB0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1048__FPv);
#ifdef SKIP_ASM
void* func_001B1048(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B1068);

INCLUDE_ASM("fe/feasyncfile", func_001B10F8);

INCLUDE_ASM("fe/feasyncfile", func_001B1280);

INCLUDE_ASM("fe/feasyncfile", func_001B14D8);

INCLUDE_ASM("fe/feasyncfile", func_001B1590);

INCLUDE_ASM("fe/feasyncfile", func_001B15D8);

INCLUDE_ASM("fe/feasyncfile", func_001B1760);

INCLUDE_ASM("fe/feasyncfile", func_001B1808);

INCLUDE_ASM("fe/feasyncfile", func_001B1A08);

INCLUDE_ASM("fe/feasyncfile", func_001B1B10);

INCLUDE_ASM("fe/feasyncfile", func_001B1B98);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B1EC0);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001B1F18(void* self, void* a1, int a2);

extern "C" void func_001B1EC0(void* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x838:
        func_001B1F18(self, a1, a2);
        break;
    case 0x835:
        func_001A94D8(self);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B1F18);

INCLUDE_ASM("fe/feasyncfile", func_001B1F88);

extern void* D_0046AE58[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B2048__FPv);
#ifdef SKIP_ASM
void* func_001B2048(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046AE58;
    return func_001A85D0(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B2070);

INCLUDE_ASM("fe/feasyncfile", func_001B22A0);

INCLUDE_ASM("fe/feasyncfile", func_001B2320);

INCLUDE_ASM("fe/feasyncfile", func_001B2378);

INCLUDE_ASM("fe/feasyncfile", func_001B25E0);

INCLUDE_ASM("fe/feasyncfile", func_001B27B0);

INCLUDE_ASM("fe/feasyncfile", func_001B2CD0);

INCLUDE_ASM("fe/feasyncfile", func_001B2DA0);

INCLUDE_ASM("fe/feasyncfile", func_001B2EB8);

INCLUDE_ASM("fe/feasyncfile", func_001B31A0);

INCLUDE_ASM("fe/feasyncfile", func_001B33D8);

INCLUDE_ASM("fe/feasyncfile", func_001B3490);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B3528);
#ifdef SKIP_ASM
extern "C" void func_001B3BF0(void* self);
extern "C" void func_001A87D0(void* self, int a1);

extern "C" void func_001B3528(void* self, int a1)
{
    func_001B3BF0(self);
    func_001A87D0(self, a1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B3568);
#ifdef SKIP_ASM
extern "C" void func_001B2378(void* self, int a1);

extern "C" int func_001B3568(void* self, void* sender, int event, int value)
{
    if (event == 4) {
        if (sender == *(void**)((char*)self + 0x6FC)) {
            *(int*)((char*)self + 0x704) = value;
            func_001B2378(self, 0);
            return 0x101;
        }
        if (sender == *(void**)((char*)self + 0x700)) {
            *(int*)((char*)self + 0x708) = value;
        }
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B35B0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B3760);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B0F60(void* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_0039F600(void* p);
extern const char D_00461C60[];

extern "C" void func_001B3760(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_0039F600(*(char**)((char*)self + 0x10) + 0x18);
        func_001A8A40(self, func_001B0F60(cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), *(int*)((char*)self + 0x10)));
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B37D8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B0F60(void* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_0039F600(void* p);
extern const char D_00461C60[];

extern "C" void func_001B37D8(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_0039F600(*(char**)((char*)self + 0x10) + 0x18);
        func_001A8A40(self, func_001B0F60(cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), *(int*)((char*)self + 0x10)));
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B3850);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001B35B0(void* self, void* a1, int a2);
extern "C" void func_001B3760(void* self, void* a1, int a2);
extern "C" void func_001B37D8(void* self, void* a1, int a2);

extern "C" void func_001B3850(void* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x80B:
        func_001B35B0(self, a1, a2);
        break;
    case 0x800:
        func_001B3760(self, a1, a2);
        break;
    case 0x801:
        func_001B37D8(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B38D0);

INCLUDE_ASM("fe/feasyncfile", func_001B3978);

INCLUDE_ASM("fe/feasyncfile", func_001B3A30);

INCLUDE_ASM("fe/feasyncfile", func_001B3BF0);

INCLUDE_ASM("fe/feasyncfile", func_001B3D20);

INCLUDE_ASM("fe/feasyncfile", func_001B3F78);

INCLUDE_ASM("fe/feasyncfile", func_001B4018);

INCLUDE_ASM("fe/feasyncfile", func_001B40D0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4168);
#ifdef SKIP_ASM
void* func_0039E4A0(void* self);
extern "C" void* func_0028B180();
extern "C" void func_0028FC58(void*);

extern "C" void func_001B4168(void* self)
{
    func_0039E4A0(self);
    func_0028FC58(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4198__FPv);
#ifdef SKIP_ASM
void func_001B4198(void* self)
{
    *(int*)((char*)self + 0x708) = 6;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B41A8);

INCLUDE_ASM("fe/feasyncfile", func_001B43A8);

INCLUDE_ASM("fe/feasyncfile", func_001B4470);

INCLUDE_ASM("fe/feasyncfile", func_001B45E8);

INCLUDE_ASM("fe/feasyncfile", func_001B47C0);

INCLUDE_ASM("fe/feasyncfile", func_001B4888);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4C00);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_001B4C00(void* self, void* a1, int a2)
{
    int r;
    if (a2 == 0xF || a2 == 0x14) {
        r = func_001A97B8(self, a1, a2);
    } else {
        r = func_001A97B8(self, a1, a2);
    }
    return r;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B4C40);

INCLUDE_ASM("fe/feasyncfile", func_001B4CD8);

INCLUDE_ASM("fe/feasyncfile", func_001B4FA0);

INCLUDE_ASM("fe/feasyncfile", func_001B5068);

INCLUDE_ASM("fe/feasyncfile", func_001B5268);

INCLUDE_ASM("fe/feasyncfile", func_001B52C8);

INCLUDE_ASM("fe/feasyncfile", func_001B54B8);

INCLUDE_ASM("fe/feasyncfile", func_001B55B0);

INCLUDE_ASM("fe/feasyncfile", func_001B5740);

INCLUDE_ASM("fe/feasyncfile", func_001B5830);

INCLUDE_ASM("fe/feasyncfile", func_001B5A58);

INCLUDE_ASM("fe/feasyncfile", func_001B5BD8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5CF8);
#ifdef SKIP_ASM
class cFEVObj_001B5CF8 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int a1);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001B5CF8(void* self, void* a1, int a2)
{
    if (a2 == 0x16) {
        func_001A97B8(self, a1, a2);
        (*(cFEVObj_001B5CF8**)((char*)self + 0x6EC))->v08(0);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B5D58);

INCLUDE_ASM("fe/feasyncfile", func_001B5E28);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6078__FPv);
#ifdef SKIP_ASM
int func_001B6078(void* self)
{
    return 0x101;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B6080);

INCLUDE_ASM("fe/feasyncfile", func_001B60A8);

INCLUDE_ASM("fe/feasyncfile", func_001B6288);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6368);
#ifdef SKIP_ASM
extern void* D_004689F0[];
void cMemMan_free(void* p);
extern "C" void func_002563A8(void* p);
extern "C" void func_002561E0(void* p, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001B6368(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_004689F0;
    if (*(void**)((char*)self + 0x6D0) != 0) {
        func_002563A8(*(void**)((char*)self + 0x6D0));
        if (*(void**)((char*)self + 0x6D0) != 0) {
            func_002561E0(*(void**)((char*)self + 0x6D0), 3);
        }
    }
    if (*(void**)((char*)self + 0x6FC) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x6FC));
        *(void**)((char*)self + 0x6FC) = 0;
    }
    func_001A85D0_dtor(self, flags);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B63E8);

INCLUDE_ASM("fe/feasyncfile", func_001B65C0);

INCLUDE_ASM("fe/feasyncfile", func_001B66B8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6740);
#ifdef SKIP_ASM
class func_001B6740_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

extern "C" int func_001B6740(void* self, int a1, int a2)
{
    return (*(func_001B6740_cObj**)((char*)self + 0x20))->v24(a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6770);
#ifdef SKIP_ASM
extern "C" void func_001A8918(void* self, int a1, int msg);

extern "C" void func_001B6770(void* self, int a1, int msg)
{
    if (msg != 0xD) {
        func_001A8918(self, a1, msg);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B6798);

INCLUDE_ASM("fe/feasyncfile", func_001B6A90);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6AB8);
#ifdef SKIP_ASM
extern void* D_004688E0[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001B6AB8(void* self, int a1, int a2, int a3, int a4, int a5)
{
    func_001A8500_3(self, a1, a2);
    *(int*)((char*)self + 0x6D0) = a3;
    *(int*)((char*)self + 0x6D4) = a4;
    *(int*)((char*)self + 0x6D8) = a5;
    *(void***)((char*)self + 0x8) = D_004688E0;
    *(int*)((char*)self + 0xC) = 0x45;
    return self;
}
#endif

extern void* D_004688E0[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6B28__FPv);
#ifdef SKIP_ASM
void* func_001B6B28(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_004688E0;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6B50);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004642B0[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void* func_001B6B50(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004642B0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        screen = cUIScreen_playFrame(screen, 0, 0);
    }
    return screen;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B6BB8);

INCLUDE_ASM("fe/feasyncfile", func_001B6C98);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6E10);
#ifdef SKIP_ASM
class func_001B6E10_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

extern "C" int func_001B6E10(void* self, int a1, int a2)
{
    return (*(func_001B6E10_cObj**)((char*)self + 0x20))->v24(a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6E40);
#ifdef SKIP_ASM
class cFEVObj_001B6E40 {
public:
    int field_0x0;
    int field_0x4;
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
    virtual void v17(void* sender, int msg);
};

extern "C" void func_001A8918(void* self, int a1, int msg);

extern "C" void func_001B6E40(void* self, int a1, unsigned int msg)
{
    switch (msg) {
    case 5:
    case 6:
        (*(cFEVObj_001B6E40**)((char*)self + 0x20))->v17(self, 0xF);
        break;
    case 7:
    case 8:
        break;
    default:
        func_001A8918(self, a1, msg);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B6EA8);

INCLUDE_ASM("fe/feasyncfile", func_001B6ED0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7030);
#ifdef SKIP_ASM
extern void* D_004687D0[];
extern void* D_0046B6F8[];
extern "C" void func_002658B8();
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001B7030(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_004687D0;
    func_002658B8();
    *(void***)((char*)self + 0x8) = D_0046B6F8;
    func_001A85D0_dtor(self, flags);
}
#endif

extern "C" void* func_001ABFA8(void* self);

//99.29%
INCLUDE_ASM("fe/feasyncfile", func_001B7088__FPv);
#ifdef SKIP_ASM
void* func_001B7088(void* self)
{
    return func_001ABFA8(self);
}
#endif

extern "C" void* func_001B7778(void*);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B70A8__FPv);
#ifdef SKIP_ASM
void* func_001B70A8(void* self)
{
    int t0 = 0;
    *(signed char*)((char*)self + 0x6e0) = (signed char)t0;
    *(signed char*)((char*)self + 0x6f1) = (signed char)t0;
    return func_001B7778(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B70D0);

INCLUDE_ASM("fe/feasyncfile", func_001B7650);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7778);
#ifdef SKIP_ASM
extern "C" void func_00265F18(void*);
extern "C" void* func_00265F68(void*);

extern "C" void* func_001B7778(void* self)
{
    func_00265F18((char*)self + 0x6E0);
    return func_00265F68((char*)self + 0x6F1);
}
#endif

extern "C" void* func_001B70D0(void*, int);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B77A8__FPv);
#ifdef SKIP_ASM
void* func_001B77A8(void* self)
{
    return func_001B70D0(self, 2);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B77C8);

INCLUDE_ASM("fe/feasyncfile", func_001B7820);

INCLUDE_ASM("fe/feasyncfile", func_001B78F0);

INCLUDE_ASM("fe/feasyncfile", func_001B7938);

INCLUDE_ASM("fe/feasyncfile", func_001B7A08);

INCLUDE_ASM("fe/feasyncfile", func_001B7A50);

INCLUDE_ASM("fe/feasyncfile", func_001B7A98);

INCLUDE_ASM("fe/feasyncfile", func_001B7BC0);

INCLUDE_ASM("fe/feasyncfile", func_001B7C28);

INCLUDE_ASM("fe/feasyncfile", func_001B7C70);

INCLUDE_ASM("fe/feasyncfile", func_001B7CB8);

INCLUDE_ASM("fe/feasyncfile", func_001B7E40);

INCLUDE_ASM("fe/feasyncfile", func_001B8238);

INCLUDE_ASM("fe/feasyncfile", func_001B83C0);

INCLUDE_ASM("fe/feasyncfile", func_001B84D8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B8860);
#ifdef SKIP_ASM
class func_001B8860_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

extern "C" int func_001B8860(void* self, int a1, int a2)
{
    return (*(func_001B8860_cObj**)((char*)self + 0x20))->v24(a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B8890);
#ifdef SKIP_ASM
extern "C" int func_001B8890(void* self, int id)
{
    switch (id) {
    case 25:
        return 28;
    case 13:
        return 15;
    case 21:
        return 23;
    case 7:
        return 9;
    }
    return 28;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B88F0);

INCLUDE_ASM("fe/feasyncfile", func_001B8960);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B89E8__FPv);
#ifdef SKIP_ASM
void* func_001B89E8(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B8A08);

INCLUDE_ASM("fe/feasyncfile", func_001B8AA0);

INCLUDE_ASM("fe/feasyncfile", func_001B8C38);

INCLUDE_ASM("fe/feasyncfile", func_001B9088);

INCLUDE_ASM("fe/feasyncfile", func_001B91C8);

INCLUDE_ASM("fe/feasyncfile", func_001B92C8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B9428);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001B91C8(void* self, void* a1, int a2);
extern "C" void func_001B92C8(void* self, void* a1, int a2);

extern "C" void func_001B9428(void* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x81E:
        func_001B91C8(self, a1, a2);
        break;
    case 0x82C:
        func_001B92C8(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B9488);

INCLUDE_ASM("fe/feasyncfile", func_001B9558);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B95C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00462B88[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);

extern "C" void func_001B95C0(void* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00462B88));
    if (text != 0) {
        cUIText_setAsciiString(text, **(const char***)((char*)self + 0x6D0));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B9610);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00462BA0[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A98B8(void* self, const char* str);

extern "C" void func_001B9610(void* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00462BA0));
    if (text != 0) {
        func_001A98B8(self, (*(const char***)((char*)self + 0x6D0))[1]);
        cUIText_setAsciiString(text, (char*)self + 0x5C);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B9678);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004647F8[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A98B8(void* self, const char* str);

extern "C" void func_001B9678(void* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004647F8));
    if (text != 0) {
        func_001A98B8(self, (*(const char***)((char*)self + 0x6D0))[2]);
        cUIText_setAsciiString(text, (char*)self + 0x5C);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B96E0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00464838[];
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001B9950(void* self, cUIText* text, char* str);

extern "C" void func_001B96E0(void* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00464838));
    if (text != 0) {
        char* s = (*(char***)((char*)self + 0x6D0))[3];
        if (*(int*)(s - 8) != 0) {
            func_001B9950(self, text, s);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9740);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464780[];

extern "C" void func_001B9740(void* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00464780));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x38) != 0, &out);
        func_0039A7A8(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B97B0);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464790[];

extern "C" void func_001B97B0(void* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00464790));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x3C) != 0, &out);
        func_0039A7A8(box, out);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B9820);

INCLUDE_ASM("fe/feasyncfile", func_001B9950);

INCLUDE_ASM("fe/feasyncfile", func_001B9B18);

INCLUDE_ASM("fe/feasyncfile", func_001B9CD8);

INCLUDE_ASM("fe/feasyncfile", func_001B9D68);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9F68);
#ifdef SKIP_ASM
extern "C" int func_001B9F68(void* self, signed char val, signed char lo, signed char hi)
{
    if (val <= hi && val >= lo) {
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B9FA0);

INCLUDE_ASM("fe/feasyncfile", func_001BA110);

INCLUDE_ASM("fe/feasyncfile", func_001BA178);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA218);
#ifdef SKIP_ASM
extern "C" void* cBXString_operatorE(void* self, void* other);

extern "C" void func_001BA218(void* self)
{
    cBXString_operatorE(*(char**)((char*)self + 0x6D0) + 0x4, (char*)self + 0x6D4);
    cBXString_operatorE(*(char**)((char*)self + 0x6D0) + 0x8, (char*)self + 0x6D8);
    cBXString_operatorE(*(char**)((char*)self + 0x6D0) + 0xC, (char*)self + 0x6DC);
    *(int*)(*(char**)((char*)self + 0x6D0) + 0x38) = *(int*)((char*)self + 0x6E0);
    *(int*)(*(char**)((char*)self + 0x6D0) + 0x3C) = *(int*)((char*)self + 0x6E4);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BA280);

INCLUDE_ASM("fe/feasyncfile", func_001BA358);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA3E0);
#ifdef SKIP_ASM
extern void* D_004685B0[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001BA3E0(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x4E;
    *(void***)((char*)self + 0x8) = D_004685B0;
    *(int*)((char*)self + 0x6A8) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BA428);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA4E0__FPv);
#ifdef SKIP_ASM
void* func_001BA4E0(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BA500);

INCLUDE_ASM("fe/feasyncfile", func_001BA618);

INCLUDE_ASM("fe/feasyncfile", func_001BA8B8);

INCLUDE_ASM("fe/feasyncfile", func_001BA9F8);

INCLUDE_ASM("fe/feasyncfile", func_001BAA88);

INCLUDE_ASM("fe/feasyncfile", func_001BAB28);

INCLUDE_ASM("fe/feasyncfile", func_001BAC90);

INCLUDE_ASM("fe/feasyncfile", func_001BADE8);

INCLUDE_ASM("fe/feasyncfile", func_001BAED8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB098);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BBAC8(void* self, int state);

extern "C" void func_001BB098(void* self, void* a1, int a2)
{
    if (a2 == 0x16) {
        func_001A97B8(self, a1, a2);
        func_001BBAC8(self, 0x11);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB0E8);
#ifdef SKIP_ASM
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BA9F8(void* self, int a1);

extern "C" void func_001BB0E8(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(self, a1, 0xF);
        cBXString_cBXString4(*(char**)((char*)self + 0x6D0) + 0x10, *(const char**)((char*)a1 + 0x2B0));
        func_001BA9F8(self, 0);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BB158);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB2D0);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_004A1BD8[];

extern "C" void func_001BB2D0(void* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1BD8));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x28), &out);
        func_0039A7A8(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB340);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_004A1BE0[];

extern "C" void func_001BB340(void* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1BE0));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x2C), &out);
        func_0039A7A8(box, out);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BB3B0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB440);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464A18[];

extern "C" void func_001BB440(void* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00464A18));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x34), &out);
        func_0039A7A8(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB4B0);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464A08[];

extern "C" void func_001BB4B0(void* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00464A08));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x40) != 0, &out);
        func_0039A7A8(box, out);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BB520);

INCLUDE_ASM("fe/feasyncfile", func_001BB5D0);

INCLUDE_ASM("fe/feasyncfile", func_001BB798);

INCLUDE_ASM("fe/feasyncfile", func_001BB840);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB968);
#ifdef SKIP_ASM
extern "C" int func_001BB968(void* self, int year)
{
    int leap = 0;
    if ((year & 3) == 0) {
        leap = 1;
        if (year % 100 == 0) {
            leap = (year % 400 == 0);
        }
    }
    return leap;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BB9B0);

extern "C" void* func_001BBC40(void* self);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBAA8__FPv);
#ifdef SKIP_ASM
int func_001BBAA8(void* self)
{
    return (func_001BBC40(self) != 0);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BBAC8);

INCLUDE_ASM("fe/feasyncfile", func_001BBC40);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBD08);
#ifdef SKIP_ASM
extern void* D_004684A0[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001BBD08(void* self, int a1, int a2)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0x6D0) = a2;
    *(void***)((char*)self + 0x8) = D_004684A0;
    *(int*)((char*)self + 0xC) = 0x4F;
    *(int*)((char*)self + 0x6A8) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BBD60);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBDE0__FPv);
#ifdef SKIP_ASM
void* func_001BBDE0(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BBE00);

INCLUDE_ASM("fe/feasyncfile", func_001BBF18);

INCLUDE_ASM("fe/feasyncfile", func_001BC190);

INCLUDE_ASM("fe/feasyncfile", func_001BC290);

INCLUDE_ASM("fe/feasyncfile", func_001BC300);

INCLUDE_ASM("fe/feasyncfile", func_001BC400);

INCLUDE_ASM("fe/feasyncfile", func_001BC4F0);

INCLUDE_ASM("fe/feasyncfile", func_001BC6A0);

INCLUDE_ASM("fe/feasyncfile", func_001BC728);

INCLUDE_ASM("fe/feasyncfile", func_001BC818);

INCLUDE_ASM("fe/feasyncfile", func_001BC990);

INCLUDE_ASM("fe/feasyncfile", func_001BCB58);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BCC30);
#ifdef SKIP_ASM
class cFEVObj_001BCC30 {
public:
    int field_0x0;
    int field_0x4;
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
};

int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char D_00464DA0[];

extern "C" void func_001BCC30(cFEVObj_001BCC30* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00464DA0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        self->v28();
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BCCA8);

INCLUDE_ASM("fe/feasyncfile", func_001BCD18);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BCE18__FPv);
#ifdef SKIP_ASM
void* func_001BCE18(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BCE38);

INCLUDE_ASM("fe/feasyncfile", func_001BD1B8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD428);
#ifdef SKIP_ASM
extern "C" void func_001BD540(void* self, int a1, void* a2);

extern "C" void func_001BD428(void* self)
{
    char* p = *(char**)((char*)self + 0x69C);
    if (*(int*)(p + 0x18) == 8) {
        func_001BD540(self, *(int*)((char*)self + 0xB80), p + 0x74);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BD460);

INCLUDE_ASM("fe/feasyncfile", func_001BD540);

INCLUDE_ASM("fe/feasyncfile", func_001BD610);

INCLUDE_ASM("fe/feasyncfile", func_001BD738);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD848);
#ifdef SKIP_ASM
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void func_001BDA88(void* self, int a2);
extern "C" void func_001BD8C8(void* self, int a2);

extern "C" void func_001BD848(void* self, int a1, int a2)
{
    if (*(int*)((char*)self + 0x1C) & 6) {
        switch (*(int*)((char*)*(void**)((char*)self + 0x6A0) + 0x18)) {
        case 0x828:
            func_001BDA88(self, a2);
            break;
        case 0x829:
            func_001BD8C8(self, a2);
            break;
        default:
            func_001A8E40_3(self, a1, a2);
            break;
        }
    } else {
        func_001A8E40_3(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BD8C8);

INCLUDE_ASM("fe/feasyncfile", func_001BDA88);

INCLUDE_ASM("fe/feasyncfile", func_001BDC18);

INCLUDE_ASM("fe/feasyncfile", func_001BDCA0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001BDCF8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BD610(void* self, int a1, int a2, int a3);

extern "C" void func_001BDCF8(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(self, a1, a2);
        func_001BD610(self, *(int*)((char*)a1 + 0x6DC), *(int*)((char*)a1 + 0x6E0), *(int*)((char*)a1 + 0x6E4));
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BDD60);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001BDDE8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BDCF8(void* self, void* a1, int a2);
extern "C" void func_001BDD60(void* self, void* a1, int a2);

extern "C" void func_001BDDE8(void* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x823:
        func_001BDCF8(self, a1, a2);
        break;
    case 0x82A:
        func_001BDD60(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BDE48);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDF60);
#ifdef SKIP_ASM
class cFEVObj_001BDF60 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

struct sFESlots1BDF60 {
    char pad[0x718];
    int ids[4];                     // 0x718
    char pad728[0x730 - 0x728];
    cFEVObj_001BDF60* objs[4];      // 0x730
};

extern "C" void func_001BDFD8(void* self);

extern "C" void func_001BDF60(sFESlots1BDF60* self, int id)
{
    func_001BDFD8(self);
    for (int i = 0; i < 4; i++) {
        if (id == self->ids[i]) {
            self->objs[i]->v09(1);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDFD8);
#ifdef SKIP_ASM
class cFEVObj_001BDFD8 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

extern "C" void func_001BDFD8(void* self)
{
    int i;
    for (i = 0; i < 4; i++) {
        ((cFEVObj_001BDFD8**)((char*)self + 0x730))[i]->v09(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE038);
#ifdef SKIP_ASM
struct sListBox1BE038 {
    char pad[0x318];
    unsigned char count;    // 0x318
};

struct sFELists1BE038 {
    char pad[0x708];
    sListBox1BE038* boxes[4];       // 0x708
    char pad718[0x750 - 0x718];
    unsigned int sel[4];            // 0x750
};

extern "C" void func_0039A7A8(void* self, int a1);

extern "C" void func_001BE038(sFELists1BE038* self)
{
    for (unsigned int i = 0; i < 4; i++) {
        if (self->sel[i] >= self->boxes[i]->count) {
            self->sel[i] = self->boxes[i]->count - 1;
        }
        if (self->boxes[i]->count != 0) {
            func_0039A7A8(self->boxes[i], (unsigned char)self->sel[i]);
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BE0A8);

INCLUDE_ASM("fe/feasyncfile", func_001BE3A8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE510);
#ifdef SKIP_ASM
extern "C" void func_001BE540(void*);
extern "C" void func_001BE720(void*);

extern "C" void func_001BE510(void* self)
{
    func_001BE540(self);
    func_001BE720(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BE540);

INCLUDE_ASM("fe/feasyncfile", func_001BE720);

INCLUDE_ASM("fe/feasyncfile", func_001BE7D8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE828);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004651E0[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void* func_001BE828(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004651E0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        screen = cUIScreen_playFrame(screen, 0, 0);
    }
    return screen;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE890__FPv);
#ifdef SKIP_ASM
void* func_001BE890(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BE8B0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEA30__FPv);
#ifdef SKIP_ASM
int func_001BEA30(void* self)
{
    return 0x101;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BEA38);

INCLUDE_ASM("fe/feasyncfile", func_001BEC78);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BED28);
#ifdef SKIP_ASM
class func_001BED28_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

extern "C" int func_001BED28(void* self, int a1, int a2)
{
    return (*(func_001BED28_cObj**)((char*)self + 0x20))->v24(a1, a2);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BED58);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEE30);
#ifdef SKIP_ASM
extern void* D_00468388[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001BEE30(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x33;
    *(void***)((char*)self + 0x8) = D_00468388;
    *(int*)((char*)self + 0x6D0) = 0;
    *(int*)((char*)self + 0x6A8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEE78);
#ifdef SKIP_ASM
extern void* D_00468388[];
void cMemMan_free(void* p);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001BEE78(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00468388;
    if (*(void**)((char*)self + 0x6D0) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x6D0));
        *(void**)((char*)self + 0x6D0) = 0;
    }
    func_001A85D0_dtor(self, flags);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001BEED0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern char D_004652B0[];

extern "C" void func_001BEED0(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004652B0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001A8768(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEF40__FPv);
#ifdef SKIP_ASM
void* func_001BEF40(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BEF60);

INCLUDE_ASM("fe/feasyncfile", func_001BF228);

INCLUDE_ASM("fe/feasyncfile", func_001BF378);

INCLUDE_ASM("fe/feasyncfile", func_001BF438);

INCLUDE_ASM("fe/feasyncfile", func_001BF5A8);

INCLUDE_ASM("fe/feasyncfile", func_001BF6C0);

INCLUDE_ASM("fe/feasyncfile", func_001BF758);

INCLUDE_ASM("fe/feasyncfile", func_001BFAE0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFBB8);
#ifdef SKIP_ASM
extern void* D_0046AD48[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001BFBB8(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x3B;
    *(void***)((char*)self + 0x8) = D_0046AD48;
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BFBF8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFCA0__FPv);
#ifdef SKIP_ASM
void* func_001BFCA0(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BFCC0);

INCLUDE_ASM("fe/feasyncfile", func_001BFD98);

INCLUDE_ASM("fe/feasyncfile", func_001BFE70);

INCLUDE_ASM("fe/feasyncfile", func_001C0138);

INCLUDE_ASM("fe/feasyncfile", func_001C0240);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C02F8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C0138(void* self, void* a1, int a2);
extern "C" void func_001C0240(void* self, void* a1, int a2);

extern "C" void func_001C02F8(void* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x82D:
        func_001C0138(self, a1, a2);
        break;
    case 0x846:
        func_001C0240(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C0358);
#ifdef SKIP_ASM
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B0F60(void* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern const char D_00461C60[];

extern "C" void* func_001C0358(void* self, int a1, int a2)
{
    switch (a1) {
    default:
        return func_001A8E40_3(self, a1, a2);
    case 0xF3:
        func_001A8A40(self, func_001B0F60(cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), *(int*)((char*)self + 0x10)));
        break;
    case 0xEF:
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0418);
#ifdef SKIP_ASM
extern void* D_0046AC38[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001C0418(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x43;
    *(void***)((char*)self + 0x8) = D_0046AC38;
    *(int*)((char*)self + 0x6D0) = 0;
    *(int*)((char*)self + 0x6D8) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C04A8);

INCLUDE_ASM("fe/feasyncfile", func_001C0540);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0A28);
#ifdef SKIP_ASM
extern void* D_0046AC38[];
void cMemMan_free(void* p);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

extern "C" void func_001C0A28(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046AC38;
    if (*(void**)((char*)self + 0x6D0) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x6D0));
        *(void**)((char*)self + 0x6D0) = 0;
    }
    func_001A85D0_dtor(self, flags);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C0A80);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0B08);
#ifdef SKIP_ASM
extern "C" int func_001C0B08(void* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C0B20);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0CE0);
#ifdef SKIP_ASM
extern char D_004619B8[];
int GetHashValue32(char* str);

extern "C" void func_001C0CE0(void* self, void* item)
{
    int hash = *(int*)((char*)item + 0x38);
    if (hash == GetHashValue32(D_004619B8)) {
        *(int*)((char*)item + 0x14) |= 1;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C0D28);

extern "C" void* func_001A8E40(void* self);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0E88__FPv);
#ifdef SKIP_ASM
void* func_001C0E88(void* self)
{
    return func_001A8E40(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C0EA8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0F40);
#ifdef SKIP_ASM
extern void* D_0046AB28[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001C0F40(void* self, int a1, int a2)
{
    func_001A8500_3(self, a1, a2);
    *(void***)((char*)self + 0x8) = D_0046AB28;
    *(int*)((char*)self + 0xC) = 0x48;
    *(int*)((char*)self + 0x6EC) = 0;
    *(int*)((char*)self + 0x720) = 0;
    *(int*)((char*)self + 0x724) = 0;
    *(int*)((char*)self + 0x728) = 0;
    *(int*)((char*)self + 0x72C) = 0;
    *(int*)((char*)self + 0x730) = 0;
    *(int*)((char*)self + 0x6F8) = 0;
    *(int*)((char*)self + 0x6FC) = 0;
    *(int*)((char*)self + 0x700) = 0;
    *(int*)((char*)self + 0x704) = 0;
    *(int*)((char*)self + 0x708) = 0;
    *(int*)((char*)self + 0x70C) = 0;
    *(int*)((char*)self + 0x710) = 0;
    *(int*)((char*)self + 0x714) = 0;
    *(int*)((char*)self + 0x718) = 0;
    *(int*)((char*)self + 0x71C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0FC0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_003E6448(void* dst, int c, int n);
extern char D_004657E0[];
extern char D_005308B8[];

extern "C" void func_001C0FC0(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004657E0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_003E6448(D_005308B8, 0, 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1038);
#ifdef SKIP_ASM
extern "C" void* func_001A8770(void* self);
extern "C" void* func_001C1038(void* self)
{
    if (*(int*)((char*)self + 0x6EC) != 0) {
        int t = (*(int*)((char*)self + 0x6E8))--;
        if (t == 0) {
            *(int*)((char*)self + 0x6E8) = 30;
        }
    }
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C1080);

INCLUDE_ASM("fe/feasyncfile", func_001C13E0);

INCLUDE_ASM("fe/feasyncfile", func_001C1488);

INCLUDE_ASM("fe/feasyncfile", func_001C1560);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C17D8__FPv);
#ifdef SKIP_ASM
int func_001C17D8(void* self)
{
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C17E0);
#ifdef SKIP_ASM
extern "C" int func_001C17E0(void* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C17F8);

INCLUDE_ASM("fe/feasyncfile", func_001C19C8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1B48);
#ifdef SKIP_ASM
extern "C" int func_001C1B88(void* self, void* a1, int a2);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" int func_001C1B48(void* self, void* a1, int a2)
{
    int r;
    if (*(int*)((char*)a1 + 0x18) == 0x82F) {
        r = func_001C1B88(self, a1, a2);
    } else {
        r = func_001A97B8(self, a1, a2);
    }
    return r;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C1B88);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1C50);
#ifdef SKIP_ASM
extern void* D_0046AA18[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001C1C50(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x4A;
    *(void***)((char*)self + 0x8) = D_0046AA18;
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C1C90);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1D10__FPv);
#ifdef SKIP_ASM
void* func_001C1D10(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C1D30);

INCLUDE_ASM("fe/feasyncfile", func_001C1E08);

INCLUDE_ASM("fe/feasyncfile", func_001C1EA8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C20F8);
#ifdef SKIP_ASM
extern void* D_0046A908[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001C20F8(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x4B;
    *(void***)((char*)self + 0x8) = D_0046A908;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2138);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char D_00465B20[];

extern "C" void func_001C2138(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00465B20), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x6D0) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C21A0__FPv);
#ifdef SKIP_ASM
void* func_001C21A0(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C21C0);

INCLUDE_ASM("fe/feasyncfile", func_001C2298);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C23C0);
#ifdef SKIP_ASM
class cFEVObj_001C23C0 {
public:
    int field_0x0;
    int field_0x4;
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
};

extern "C" void func_001C2418(void* self);

extern "C" int func_001C23C0(cFEVObj_001C23C0* self, int a1)
{
    self->v32();
    if (a1 != 0) {
        func_001C2418(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2418);
#ifdef SKIP_ASM
extern "C" void func_001C2468(void* self, int i);

extern "C" void func_001C2418(void* self)
{
    int i;
    for (i = 0; i < 10; i++) {
        func_001C2468(self, i);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C2468);

INCLUDE_ASM("fe/feasyncfile", func_001C2578);

INCLUDE_ASM("fe/feasyncfile", func_001C2740);

INCLUDE_ASM("fe/feasyncfile", func_001C2A18);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2B48);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C2C50(void* self, void* a1, int a2);
extern "C" void func_001C2BA8(void* self, void* a1, int a2);

extern "C" void func_001C2B48(void* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x83A:
        func_001C2C50(self, a1, a2);
        break;
    case 0x83B:
        func_001C2BA8(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C2BA8);

INCLUDE_ASM("fe/feasyncfile", func_001C2C50);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2D18);
#ifdef SKIP_ASM
extern void* D_00468278[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

struct sFEState1C2D18 {
    char pad[0x6D0];
    int slots[4];       // 0x6D0
    int a;              // 0x6E0
    int b;              // 0x6E4
    int c;              // 0x6E8
};

extern "C" void* func_001C2D18(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(void***)((char*)self + 0x8) = D_00468278;
    *(int*)((char*)self + 0x6EC) = 1;
    *(int*)((char*)self + 0xC) = 0x42;
    sFEState1C2D18* s = (sFEState1C2D18*)self;
    s->a = 0;
    s->b = 0;
    s->c = 0;
    for (int i = 3; i >= 0; i--) {
        s->slots[i] = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2E00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001C3310(void* self);
extern char D_00465C68[];

extern "C" void func_001C2E00(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00465C68), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_001C3310(self);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C2E70);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2EE0__FPv);
#ifdef SKIP_ASM
void* func_001C2EE0(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C2F00);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C32B8);
#ifdef SKIP_ASM
class cFEVObj_001C32B8 {
public:
    int field_0x0;
    int field_0x4;
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
};

extern "C" void func_001C3EE8(void* self, int a1);

extern "C" int func_001C32B8(cFEVObj_001C32B8* self, int a1)
{
    self->v32();
    if (a1 != 0) {
        func_001C3EE8(self, 0);
    }
    return 1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C3310);

INCLUDE_ASM("fe/feasyncfile", func_001C3570);

INCLUDE_ASM("fe/feasyncfile", func_001C3668);

INCLUDE_ASM("fe/feasyncfile", func_001C3D68);

INCLUDE_ASM("fe/feasyncfile", func_001C3EE8);

INCLUDE_ASM("fe/feasyncfile", func_001C4538);

INCLUDE_ASM("fe/feasyncfile", func_001C47A8);

INCLUDE_ASM("fe/feasyncfile", func_001C49C8);

INCLUDE_ASM("fe/feasyncfile", func_001C4B78);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4B98);
#ifdef SKIP_ASM
extern "C" void func_001C3310(void* self);

extern "C" int func_001C4B98(void* self, void* a1, int a2, int a3)
{
    switch (a2) {
    case 6:
        return 0x100;
    case 4:
        *(int*)((char*)self + 0x6E8) = a3;
        func_001C3310(self);
        break;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C4BD8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4CD8);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

extern "C" void* func_001C4CD8(void* self, int msg)
{
    if (msg == 0xE1) {
        func_001C3310(self);
        cUIMenu_setSelectedByIndex(*(void**)((char*)self + 0x6DC), 0);
        return 0;
    }
    return func_001A8E40(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C4D28);

INCLUDE_ASM("fe/feasyncfile", func_001C4E40);

INCLUDE_ASM("fe/feasyncfile", func_001C4F58);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5018);
#ifdef SKIP_ASM
extern void* D_00468168[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001C5018(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x43;
    *(void***)((char*)self + 0x8) = D_00468168;
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C5098);

INCLUDE_ASM("fe/feasyncfile", func_001C5108);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5178__FPv);
#ifdef SKIP_ASM
void* func_001C5178(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C5198);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5310);
#ifdef SKIP_ASM
extern char D_00465F48[];
int GetHashValue32(char* str);

class cFEVObj_001C5310 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int a1);
};

extern "C" void func_001C5310(void* self, cFEVObj_001C5310* item)
{
    int hash = *(int*)((char*)item + 0x38);
    if (hash == GetHashValue32(D_00465F48)) {
        item->v07(1);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C5368);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5420);
#ifdef SKIP_ASM
extern "C" int func_001C5420(void* self, void* a1, int a2)
{
    return a2 == 4 ? 0 : 0x101;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C5430);

INCLUDE_ASM("fe/feasyncfile", func_001C5540);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C55F8);
#ifdef SKIP_ASM
extern "C" void func_001C5630(void* self);
extern void* D_0046CCA8[];

extern "C" void* func_001C55F8(void* self)
{
    *(void***)((char*)self + 0xC8) = D_0046CCA8;
    func_001C5630(self);
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C5630);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5750);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046CCA8[];

extern "C" void func_001C5750(void* self, int flags)
{
    *(void***)((char*)self + 0xC8) = D_0046CCA8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5780__FPvii);
#ifdef SKIP_ASM
struct cFEAsyncSlot {
    int mState; // 0x0
    int mValue; // 0x4
};

struct cFEAsyncSlots {
    char pad_0x00[0x24];
    cFEAsyncSlot mSlots[1]; // 0x24
};

void func_001C5780(void* self, int a1, int a2)
{
    cFEAsyncSlots* s = (cFEAsyncSlots*)self;
    s->mSlots[a1].mValue = a2;
    s->mSlots[a1].mState = 2;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C57A0__FPvii);
#ifdef SKIP_ASM
void func_001C57A0(void* self, int a1, int a2)
{
    cFEAsyncSlots* s = (cFEAsyncSlots*)self;
    s->mSlots[a1].mValue = a2;
    s->mSlots[a1].mState = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C57C0__FPvii);
#ifdef SKIP_ASM
void func_001C57C0(void* self, int a1, int a2)
{
    cFEAsyncSlots* s = (cFEAsyncSlots*)self;
    s->mSlots[a1].mValue = a2;
    s->mSlots[a1].mState = 3;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5800);
#ifdef SKIP_ASM
extern "C" void* func_001C5800(void* self, int a1)
{
    return (char*)self + ((a1 << 3) + 0x24);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5838);
#ifdef SKIP_ASM
struct cFEAsyncReq {
    int mState; // 0x0
    int mValue; // 0x4
    int mExtra; // 0x8
};

struct cFEAsyncReqs {
    char pad_0x00[0x54];
    cFEAsyncReq mReqs[1]; // 0x54
};

extern "C" void func_001C5838(void* self, int a1, int a2, int a3)
{
    cFEAsyncReqs* s = (cFEAsyncReqs*)self;
    s->mReqs[a1].mValue = a2;
    s->mReqs[a1].mState = 1;
    s->mReqs[a1].mExtra = a3;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5860);
#ifdef SKIP_ASM
extern "C" void func_001C5860(void* self, int a1)
{
    char* p = (char*)self + a1 * 0xc;
    *(int*)(p + 0x54) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5878);
#ifdef SKIP_ASM
extern "C" void func_001C5878(void* self, int a1, int a2, int a3)
{
    cFEAsyncReqs* s = (cFEAsyncReqs*)self;
    s->mReqs[a1].mValue = a2;
    s->mReqs[a1].mState = 3;
    s->mReqs[a1].mExtra = a3;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C58D0);
#ifdef SKIP_ASM
extern "C" void* func_001C58D0(void* self, int a1)
{
    return (char*)self + (a1 * 0xc + 0x54);
}
#endif


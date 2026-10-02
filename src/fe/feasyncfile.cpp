#include "common.h"

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_Load3PeakPic);
#ifdef SKIP_ASM
struct cFEAsyncManager;
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* D_004A19F0;
extern char D_004A19F8[];
extern char D_004A1A00[];

struct sFE3PeakPicEntry {
    char mName[0x108];
    int mParam; // 0x108
    char pad[0x11C - 0x10C];
};

struct sFE3PeakPicFiles {
    sFE3PeakPicEntry mFiles[1];
};

extern "C" void cFEAsyncManager_Load3PeakPic(cFEAsyncManager* self, int slot, int param)
{
    sFE3PeakPicFiles* files = (sFE3PeakPicFiles*)self;
    sprintf(files->mFiles[slot].mName, D_004A19F8, D_004A19F0, D_004A1A00);
    files->mFiles[slot].mParam = param;
    cFEAsyncManager_SetFileStatus(self, slot, 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_Load1PeakPic);
#ifdef SKIP_ASM
struct cFEAsyncManager;
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* D_00441258[];
extern char D_004A19F8[];
extern char D_004A1A00[];

struct sFEPeakPicEntry {
    char mName[0x108];
    int mParam; // 0x108
    char pad[0x11C - 0x10C];
};

struct sFEPeakPicFiles {
    sFEPeakPicEntry mFiles[1];
};

extern "C" void cFEAsyncManager_Load1PeakPic(cFEAsyncManager* self, int pic, int slot, int param)
{
    sFEPeakPicFiles* files = (sFEPeakPicFiles*)self;
    sprintf(files->mFiles[slot].mName, D_004A19F8, D_00441258[pic], D_004A1A00);
    files->mFiles[slot].mParam = param;
    cFEAsyncManager_SetFileStatus(self, slot, 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A37F8);
#ifdef SKIP_ASM
extern char D_004A1A08[];
extern char D_004A1A10[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_00154760(void* self, int id);

extern "C" void func_001A37F8(cFEAsyncManager* self, int id, int slot, int param)
{
    if (id >= 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 10);
        sFEPeakPicFiles* files = (sFEPeakPicFiles*)self;
        sprintf(files->mFiles[slot].mName, D_004A1A08, D_004A1A10, func_00154760(iface, id), D_004A1A00);
        files->mFiles[slot].mParam = param;
        cFEAsyncManager_SetFileStatus(self, slot, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_UnloadFEAsyncFile);
#ifdef SKIP_ASM
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index);
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status);
void cMemMan_free(void* p);
extern "C" int ASYNCFILE_release(int handle, void** data, int* size);
extern char* D_004A289C;

struct sFEUnloadFile {
    char name[0x100];
    void* data;    // 0x100
    int handle;    // 0x104
    int async;     // 0x108
    int size;      // 0x10C
    int id;        // 0x110
    int status;    // 0x114
    int f118;      // 0x118
};

struct sFEUnloadFiles {
    sFEUnloadFile files[5];
};

struct sVEUnload {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cFEAsyncManager_UnloadFEAsyncFile(cFEAsyncManager* self, int idx)
{
    sFEUnloadFiles* m = (sFEUnloadFiles*)self;
    if (cFEAsyncManager_GetFileStatus(self, idx) == 1) {
        cFEAsyncManager_SetFileStatus(self, idx, 0);
    } else if (cFEAsyncManager_GetFileStatus(self, idx) == 2) {
        ASYNCFILE_release(m->files[idx].handle, 0, 0);
        if (m->files[idx].data != 0) {
            cMemMan_free(m->files[idx].data);
        }
        m->files[idx].data = 0;
        cFEAsyncManager_SetFileStatus(self, idx, 0);
    } else if (cFEAsyncManager_GetFileStatus(self, idx) == 3) {
        char* obj = D_004A289C;
        sVEUnload* vt = *(sVEUnload**)(obj + 0x10D8);
        m->files[idx].f118 = vt[114].fn(obj + vt[114].delta) + 0xE;
        cFEAsyncManager_SetFileStatus(self, idx, 0);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3C50);
#ifdef SKIP_ASM
struct sFE3C50File {
    char name[0x100];
    void* data;    // 0x100
    int handle;    // 0x104
    int async;     // 0x108
    int size;      // 0x10C
    int id;        // 0x110
    int status;    // 0x114
    int f118;      // 0x118
};

struct sFE3C50Files {
    sFE3C50File files[5];
};

struct sVE3C50 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern char* D_004A289C;
void cMemMan_free(void* p);

extern "C" void func_001A3C50(cFEAsyncManager* self)
{
    sFE3C50Files* m = (sFE3C50Files*)self;
    int i;
    for (i = 0; i < 5; i++) {
        if (m->files[i].data != 0) {
            char* obj = D_004A289C;
            sVE3C50* vt = *(sVE3C50**)(obj + 0x10D8);
            vt[50].fn(obj + vt[50].delta, m->files[i].id);
            if (m->files[i].data != 0) {
                cMemMan_free(m->files[i].data);
            }
            m->files[i].data = 0;
            m->files[i].f118 = 0;
            m->files[i].id = -1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_LoadDataFile);
#ifdef SKIP_ASM
struct sFEAsyncFile {
    char name[0x100];
    void* data;    // 0x100
    int handle;    // 0x104
    int async;     // 0x108
    int size;      // 0x10C
    int pad110;
    int status;    // 0x114
    int pad118;
};

struct sFEAsyncFiles {
    sFEAsyncFile files[5];
};

extern char D_00461430[];
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int func_003DF748(void* file, void* data, int size);
extern "C" void func_003E1BB8(void* file, void* data, int size);

extern "C" void cFEAsyncManager_LoadDataFile(cFEAsyncManager* self, int idx)
{
    sFEAsyncFiles* m = (sFEAsyncFiles*)self;
    void** pData;
    char* base;
    m->files[idx].size = 0x109A0;
    base = (char*)self + 0x100;
    pData = (void**)(base + idx * 0x11C);
    *pData = operator_new_tag(0x109A0, D_00461430, 0x25000000, 0);
    cFEAsyncManager_SetFileStatus(self, idx, 2);
    if (m->files[idx].async != 0) {
        cFEAsyncManager_SetFileStatus(self, idx, 2);
        m->files[idx].handle = func_003DF748(&m->files[idx], *pData, m->files[idx].size);
    } else {
        func_003E1BB8(&m->files[idx], *pData, m->files[idx].size);
        cFEAsyncManager_SetFileStatus(self, idx, 3);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A3DC8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3E30);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

extern "C" void func_001A3E30()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00262768(D_004A3328, 1, 10, 0, 0, 0);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3FE0);
#ifdef SKIP_ASM
extern void* D_004A5B64;
extern "C" void func_001A6978(void* self);
extern "C" void* func_001A8770(void* self);

extern "C" void func_001A3FE0(void* self)
{
    int now = *(int*)((char*)D_004A5B64 + 0x18);
    int d = now - *(int*)((char*)self + 0x6DC);
    if (d >= 1 && d <= 59) {
        func_001A8770(self);
    } else {
        *(int*)((char*)self + 0x6DC) = now;
        func_001A6978(self);
        func_001A8770(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4040);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00461458[];
extern char D_00461470[];
extern char D_00461488[];
extern char D_004614A0[];
extern char D_0045FE60[];
extern char D_004614B8[];
extern char D_004614D0[];
extern char D_004614E8[];
extern char D_00461500[];

extern "C" void func_001A4040(void* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00461458);
    func_001DA530(menu, 0, GetHashValue32(D_00461470));
    func_001DA530(menu, 1, GetHashValue32(D_00461488));
    func_001DA530(menu, 2, GetHashValue32(D_004614A0));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_004614B8), 0);
    func_001DA510(menu, 2, GetHashValue32(D_004614D0), 2);
    func_001DA510(menu, 3, GetHashValue32(D_004614E8), 1);
    func_001DA510(menu, 4, GetHashValue32(D_00461500), 6);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A41C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_00461458[];
extern char D_00461520[];
extern char D_00461530[];
extern char D_00461540[];
extern char D_00461558[];
extern char D_00461570[];
extern char D_00461580[];
extern char D_00461590[];

class cFEVObj_001A41C0 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline int IsHash1A41C0(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001A41C0(void* self, cUIText* text)
{
    cFEVObj_001A41C0* obj = (cFEVObj_001A41C0*)text;
    if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461520)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00461458));
        obj->v09(1);
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461530)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00461540));
        obj->v09(1);
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461558)) {
        obj->v09(1);
        *(int*)((char*)text + 0x14) |= 0x80;
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461570)) {
        obj->v09(0);
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00461580));
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461590)) {
        *(int*)((char*)text + 0x14) |= 1;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4670);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00262400(void* self, int a1);
extern "C" int func_00263338(void* self, int a1);
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001A4670()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" int func_001A4670(void* self, int idx)
{
    if (func_00262400(GetSession_001A4670(), idx) != 0) {
        return 0;
    }
    if (func_00263338(GetSession_001A4670(), idx) != 0) {
        return 2;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4740);
#ifdef SKIP_ASM
extern "C" int func_001A4740(void* self, int a1)
{
    return *(int*)((char*)self + 0x6f4) + a1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4750);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_001A4810(void* self, void* a1);
extern "C" void func_00262690(void* self, int a1, void* a2);
int func_001A98B0(void* self);
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001A4750()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001A4750(void* self)
{
    char* p = *(char**)((char*)self + 0x69C);
    switch (*(int*)(p + 0x18)) {
    case 1:
        func_001A4810(self, p + 0x74);
        break;
    case 2:
        func_00262690(GetSession_001A4750(), func_001A98B0(self), *(char**)((char*)self + 0x69C) + 0x74);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4810);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025E6D0(void* self, const char* str);
extern void* D_004A3028;
extern char D_004615F8[];
extern char D_00461618[];
extern char D_00461628[];

extern "C" void func_001A4810(void* self, void* a1)
{
    const char* name = (const char*)a1;
    if (strlen(name) < 3) {
        void* p = func_001A9C50(self);
        int three = 3;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004615F8);
        *(int*)((char*)p + 0xC0) = three;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x14C) = three;
        *(int*)((char*)p + 0x150) = 0x806;
        func_001A9BC0(self, p);
    } else {
        void* p = func_001A9C50(self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00461628);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x807;
        *(int*)((char*)p + 0x14C) = 3;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0(self, p);
        cBXString_cBXString4((char*)self + 0x6D4, name);
        func_0025E6D0(D_004A3028, name);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4928);
#ifdef SKIP_ASM
extern void* D_004A3028;
extern "C" int func_0041AA88(const char* a, const char* b);
extern "C" void func_001A9890(void* self, const char* str);
int func_001A98B0(void* self);
extern "C" void func_001AB370(void* self, int a1, int a2);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001A4928(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        if (func_0041AA88(*(const char**)((char*)D_004A3028 + 0x80), *(const char**)((char*)self + 0x6D0)) != 0) {
            func_001A9890(self, *(const char**)((char*)self + 0x6D0));
            func_001AB370(self, func_001A98B0(self), 0);
        }
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A49A0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4B20);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
int func_001A98B0(void* self);
extern "C" void func_001A5898(void* self);
extern "C" void func_001A6978(void* self);
extern "C" void func_001ABB30(void* self, int a1, void* a2, int a3);
// PORT: the unit types the last parameter `int`; it is an out pointer passed through.
extern "C" void func_001A4B20_ABC50(void* self, int a1, int* out) __asm__("func_001ABC50");
extern "C" void func_001A4B20_ABE58(void* self, int a1, int* out) __asm__("func_001ABE58");
extern "C" void func_001ABED8(void* self);

extern "C" void func_001A4B20(void* self, void* a1, unsigned int kind)
{
    int r16;
    int r17;
    switch (kind) {
    case 15:
        func_001A97B8(self, a1, 15);
        func_001ABB30(self, func_001A98B0(self), a1, 15);
        break;
    case 16:
        func_001A4B20_ABE58(self, func_001A98B0(self), &r16);
        if (r16 == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, a1, 16);
        }
        func_001A5898(self);
        func_001A6978(self);
        break;
    case 17:
        func_001A4B20_ABC50(self, func_001A98B0(self), &r17);
        if (r17 == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, a1, 17);
        }
        func_001A5898(self);
        func_001A6978(self);
        break;
    default:
        func_001A97B8(self, a1, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4C58);
#ifdef SKIP_ASM
class cFEVObj_001A4C58 {
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
    virtual void v34(void* a1, int a2);
    virtual void v35(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001A4C58(cFEVObj_001A4C58* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x809:
        self->v33(a1, a2);
        break;
    case 0x80B:
        self->v34(a1, a2);
        break;
    case 0x80C:
        self->v35(a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4CE8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C5780(void* self, int a1, int a2);
extern char D_00461648[];

extern "C" void func_001A4CE8(void* self, void* src)
{
    cBXString_operatorE((char*)self + 0x6D0, (char*)src + 0x8);
    *(int*)((char*)self + 0x6D8) = *(int*)((char*)src + 0x10);
    void* p = func_001A9C50(self);
    int three = 3;
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xCC) = GetHashValue32(D_00461648);
    *(int*)((char*)p + 0xC8) = three;
    func_001C5780(sub, 0, *(int*)((char*)self + 0x6D0));
    *(int*)((char*)p + 0x14C) = three;
    *(int*)((char*)p + 0x150) = 0x809;
    func_001A9BC0(self, p);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4D90);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern char D_00461660[];
extern char D_00461618[];

extern "C" void func_001A4D90(void* self)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xCC) = GetHashValue32(D_00461660);
    *(int*)((char*)p + 0xC8) = 3;
    func_001C57C0((char*)p + 0xBC, 0, GetHashValue32(D_00461618));
    *(int*)((char*)p + 0x14C) = 3;
    *(int*)((char*)p + 0x150) = 0x808;
    func_001A9BC0(self, p);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A4E28);

INCLUDE_ASM("fe/feasyncfile", func_001A4FB0);

INCLUDE_ASM("fe/feasyncfile", func_001A5010);

INCLUDE_ASM("fe/feasyncfile", func_001A5128);

INCLUDE_ASM("fe/feasyncfile", func_001A52B0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5450);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* func_0041610C(void* dst, void* src, int n);

struct sPtrVec1A5450 {
    void** mBegin;
    void** mEnd;
};

struct sOwner1A5450 {
    char pad[0x6E8];
    sPtrVec1A5450 mVec;
};

extern "C" void func_001A5450(sOwner1A5450* self)
{
    for (;;) {
        if (self->mVec.mBegin == self->mVec.mEnd) break;
        void* p = self->mVec.mEnd[-1];
        self->mVec.mEnd--;
        if (p) {
            cBXString__cBXString(p, 3);
        }
    }
    void** first = self->mVec.mBegin;
    void** last = self->mVec.mEnd;
    void** finish = last;
    func_0041610C(first, last, (char*)finish - (char*)last);
    self->mVec.mEnd -= last - first;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A54D8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A56D0);
#ifdef SKIP_ASM
extern "C" int func_0041AA88(const char* a, const char* b);

struct sOwner1A56D0 {
    char pad[0x6E8];
    char*** mBegin;
    char*** mEnd;
};

static inline unsigned int Count1A56D0(sOwner1A56D0* s) { return s->mEnd - s->mBegin; }

extern "C" int func_001A56D0(sOwner1A56D0* self, const char* name)
{
    unsigned int i;
    for (i = 0; i < Count1A56D0(self); i++) {
        if (func_0041AA88(**(char***)((char*)self->mBegin + (i << 2)), name) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5800);
#ifdef SKIP_ASM
extern "C" int func_0041AA88(const char* a, const char* b);

struct sOwner1A5800 {
    char pad[0x6E8];
    char*** mBegin;
    char*** mEnd;
};

static inline unsigned int Count1A5800(sOwner1A5800* s) { return s->mEnd - s->mBegin; }

extern "C" int func_001A5800(sOwner1A5800* self, char** name)
{
    unsigned int i;
    for (i = 0; i < Count1A5800(self); i++) {
        if (func_0041AA88(**(char***)((char*)self->mBegin + (i << 2)), *name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A5898);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5FB8);
#ifdef SKIP_ASM
struct sVE1A5FB8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern char D_004615B0[];
extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A66A8(void* self, unsigned int index, int flag);

extern "C" void func_001A5FB8(void* self, int sel)
{
    char buf[32];
    int i;
    for (i = 0; i < 15; i++) {
        sprintf(buf, D_004615B0, i);
        void* obj = (void*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        if (obj != 0) {
            sVE1A5FB8* vt = *(sVE1A5FB8**)((char*)obj + 0x8);
            vt[9].fn((char*)obj + vt[9].delta, i == sel);
        }
        func_001A66A8(self, sel, i == sel);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A6070);

INCLUDE_ASM("fe/feasyncfile", func_001A63C8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6618);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_004615A0[];

extern "C" void func_001A6618(void* self, unsigned int index, const char* str, int a3)
{
    char buf[32];
    if (*(void**)((char*)self + 0x40) != 0 && index < 0xF) {
        sprintf(buf, D_004615A0, index);
        cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        if (text != 0) {
            cUIText_setAsciiString(text, str);
            *(int*)((char*)text + 0x18) = a3;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6E70);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_002624F8(void* self, int id);
extern "C" void* func_0041610C(void* dst, void* src, int n);
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001A6E70()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

struct sIntVec1A6E70 {
    int f0;
    int* mBegin;
    int* mEnd;
};

static inline int* Copy1A6E70(int* first, int* last, int* result)
{
    func_0041610C(result, first, (last - first) * sizeof(int));
    return result + (last - first);
}

struct sOwner1A6E70 {
    char pad[0x6E4];
    sIntVec1A6E70 mVec;
};

extern "C" void func_001A6E70(sOwner1A6E70* self)
{
    int i;
    for (i = (self->mVec.mEnd - self->mVec.mBegin) - 1; i >= 0; i--) {
        sIntVec1A6E70* v = &self->mVec;
        int id = *(int*)((char*)self->mVec.mBegin + (i << 2));
        if (func_002624F8(GetSession_001A6E70(), id) < 0) {
            // PORT: pointer arithmetic through int
            int* pos = (int*)(i * 4 + (int)self->mVec.mBegin);
            if (pos + 1 != self->mVec.mEnd) {
                Copy1A6E70(pos + 1, v->mEnd, pos);
            }
            self->mVec.mEnd--;
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A71B0);

INCLUDE_ASM("fe/feasyncfile", func_001A7390);

INCLUDE_ASM("fe/feasyncfile", func_001A77A0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7848);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* func_0041610C(void* dst, void* src, int n);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern char D_00467E18[];

struct sVE1A7848 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj1A7848 {
    char pad[0xC];
    sVE1A7848* vt;
};

struct sObjVec1A7848 {
    int f0;
    sObj1A7848** mBegin;
    sObj1A7848** mEnd;
    sObj1A7848** mCap;
};

static inline void Dealloc1A7848(sObj1A7848** p, int n)
{
    if (p != 0) {
        if (n == 1) {
            operator_delete((int*)p);
        } else {
            cMemMan_free(p);
        }
    }
}

struct sOwner1A7848 {
    char pad0[0x8];
    void* vt;
    char padC[0x6C4];
    char str[8];
    sObjVec1A7848 mVec;
};

extern "C" void func_001A7848(sOwner1A7848* self, int flags)
{
    self->vt = D_00467E18;
    for (;;) {
        if (self->mVec.mBegin == self->mVec.mEnd) break;
        sObj1A7848* p = self->mVec.mEnd[-1];
        self->mVec.mEnd--;
        if (p) {
            p->vt[1].fn((char*)p + p->vt[1].delta, 3);
        }
    }
    sObjVec1A7848* v = &self->mVec;
    sObj1A7848** first = self->mVec.mBegin;
    sObj1A7848** last = self->mVec.mEnd;
    func_0041610C(first, last, (v->mEnd - last) * sizeof(sObj1A7848*));
    self->mVec.mEnd -= last - first;
    Dealloc1A7848(v->mBegin, self->mVec.mCap - self->mVec.mBegin);
    cBXString__cBXString(self->str, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7B40);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

extern "C" void func_001A7B40()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00262768(D_004A3328, 1, 10, 0, 0, 0);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7DD8);
#ifdef SKIP_ASM
extern "C" void* func_0041610C(void* dst, void* src, int n);

struct sVE1A7DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj1A7DD8 {
    char pad[0xC];
    sVE1A7DD8* vt;
};

struct sObjVec1A7DD8 {
    int f0;
    sObj1A7DD8** mBegin;
    sObj1A7DD8** mEnd;
    sObj1A7DD8** mCap;
};

extern "C" void func_001DB918(sObjVec1A7DD8* vec, sObj1A7DD8** pos, sObj1A7DD8* const* x);

static inline sObj1A7DD8** Copy1A7DD8(sObj1A7DD8** first, sObj1A7DD8** last, sObj1A7DD8** result)
{
    func_0041610C(result, first, (last - first) * sizeof(sObj1A7DD8*));
    return result + (last - first);
}

struct sOwner1A7DD8 {
    char pad0[0x6D8];
    sObjVec1A7DD8 mVec;
};

extern "C" void func_001A7DD8(void* selfp, void* item)
{
    sOwner1A7DD8* self = (sOwner1A7DD8*)selfp;
    sObj1A7DD8* x = (sObj1A7DD8*)item;
    for (;;) {
        if ((unsigned int)(self->mVec.mEnd - self->mVec.mBegin) < 31) break;
        sObjVec1A7DD8* v = &self->mVec;
        sObj1A7DD8** pos = self->mVec.mBegin;
        sObj1A7DD8* p = *pos;
        if (p) {
            if (pos + 1 != self->mVec.mEnd) {
                Copy1A7DD8(pos + 1, v->mEnd, pos);
            }
            self->mVec.mEnd--;
            p->vt[1].fn((char*)p + p->vt[1].delta, 3);
        }
    }
    sObjVec1A7DD8* v2 = &self->mVec;
    if (self->mVec.mEnd != self->mVec.mCap) {
        sObj1A7DD8** e = v2->mEnd;
        if (e) *e = x;
        self->mVec.mEnd++;
    } else {
        func_001DB918(v2, self->mVec.mEnd, &x);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A7ED8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void func_001A7DD8(void* self, void* item);
extern char D_00461A08[];
extern void* D_0046D8C0[];
extern char D_004A1408[];
extern void* D_004A3028;

static inline void* Init_001A7ED8(char* p, const char* a, const char* b, const char* c)
{
    *(void***)(p + 0xC) = D_0046D8C0;
    cBXString_cBXString2(p, a);
    cBXString_cBXString2(p + 4, b);
    cBXString_cBXString2(p + 8, c);
    return p;
}

extern "C" void func_001A7ED8(void* self, const char* name)
{
    func_001A7DD8(self, Init_001A7ED8((char*)cMemMan_alloc(0x10, D_00461A08, 0x20000000, 0),
                                      *(const char**)((char*)D_004A3028 + 0x80), name, D_004A1408));
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A7F78);

INCLUDE_ASM("fe/feasyncfile", func_001A81F0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A83D8);
#ifdef SKIP_ASM
extern "C" void func_003A1F90(void* list, int a1);

struct sList1A83D8 {
    char pad[0xC4];
    short top;
    short sel;
    unsigned short count;
};

extern "C" void func_001A83D8(sList1A83D8* list, int keep, unsigned int sel, int a3, int pos, int a5, int a6, int trim)
{
    if (list == 0) return;
    int toEnd = 0;
    if (a3 == pos + 1 || a6 != 0) toEnd = 1;
    int n = list->count - keep;
    int atStart = pos == 0;
    int removed = 0;
    if (n > 0 && trim != 0) {
        for (removed = 0; removed < n; removed++) {
            func_003A1F90(list, 0);
        }
    }
    if (toEnd) {
        unsigned short c = list->count;
        if (sel >= c) {
            list->sel = 0;
        } else {
            list->sel = c - sel;
        }
        list->top = list->count - 1;
    } else if (atStart) {
        list->sel = 0;
        list->top = 0;
    } else {
        int d = pos - removed;
        if (d <= 0) {
            list->top = 0;
            list->sel = 0;
        } else {
            int e = a5 - removed;
            if (e <= 0) {
                list->sel = 0;
            } else {
                list->sel = e;
            }
            list->top = d;
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A8500);

INCLUDE_ASM("fe/feasyncfile", func_001A85D0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A86E8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern void* D_004A3328;
extern void* D_004A3028;
extern int D_004A1A70;

extern "C" void func_001A86E8(int on)
{
    D_004A1A70 = on;
    if (on == 0 && D_004A3328 != 0 && func_00261460(D_004A3328) != 0) {
        if (D_004A3328 == 0) {
            func_00260F80();
        }
        if (func_00261460(D_004A3328) == 0) {
            func_002613C8(D_004A3328);
        }
        if (D_004A3328 == 0) {
            func_0025B800(D_004A3028);
        }
        func_00261408(D_004A3328);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8830);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

class cFEVObj_001A8830 {
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
    virtual int v29();
};

static inline void* GetSession_001A8830()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001A8830(cFEVObj_001A8830* self)
{
    if (self->v29() != 0) {
        func_00262768(GetSession_001A8830(), 1, 0, 0, 0, 0);
    } else {
        func_00262768(GetSession_001A8830(), 0, 1, 0, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8918);
#ifdef SKIP_ASM
class cFEVObj_001A8918 {
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
    virtual void v25(void* popup);
};

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001D9D48(void* mem, void* a1, void* owner);
extern "C" void func_0039F290(void* list, void* item);
extern char D_00461A70[];

extern "C" void func_001A8918(void* self, int a1, int msg)
{
    if (a1 != 0 && msg == 7) {
        void* p = func_001D9D48(cMemMan_alloc(0x140, D_00461A70, 0x100, 0), *(void**)((char*)self + 0x10), self);
        *(int*)((char*)p + 0x50) = 1;
        ((cFEVObj_001A8918*)self)->v25(p);
        func_0039F290(*(char**)((char*)self + 0x10) + 0x18, p);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A89B0);
#ifdef SKIP_ASM
class cFEMsg_001A89B0 {
public:
    int field_0x0;
    int field_0x4;
    virtual ~cFEMsg_001A89B0();
};

extern void* D_004A3028;
extern "C" void func_001A9930(void* self);

extern "C" void* func_001A89B0(void* self, cFEMsg_001A89B0* msg)
{
    int kind = *(int*)((char*)msg + 0xC);
    if (*(int*)((char*)self + 0x6A8) != 0 && kind != 0x31 && kind != 0x33 && kind != 0x4D && kind != 0x4E
        && *(int*)D_004A3028 == 0) {
        delete msg;
        func_001A9930(self);
        return 0;
    }
    return msg;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8A40);
#ifdef SKIP_ASM
class cFEVObj_001A8A40 {
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
    virtual void* v26(void* obj);
};

int GetHashValue32(char* str);
extern char D_0045DC60[];
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0039F4C0(void* list, void* item);
// PORT: the unit declares func_001A8A40 as returning void; the body returns 0/1
extern "C" int func_001A8A40_r(void* self, void* obj) __asm__("func_001A8A40");

extern "C" int func_001A8A40_r(void* self, void* obj)
{
    void* child = *(void**)((char*)self + 0x20);
    if (child != 0) {
        return func_001A8A40_r(child, obj);
    }
    void* item = ((cFEVObj_001A8A40*)self)->v26(obj);
    if (item != 0) {
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_0045DC60));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
        }
        func_0039F4C0(*(char**)((char*)self + 0x10) + 0x18, item);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8AE8);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is probably the game's overloaded operator new(size, tag, flags, align). Calling it as
// operator new gives gcc's malloc-like (REG_NOALIAS) aliasing that the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char D_0045CDE8[];

class cFEVObj_001A8AE8 {
public:
    int field_0x0;
    virtual void* v01();
    virtual void* v02();
};

struct sListNode1A8AE8 {
    void* mNext;
    void* mPrev;
    void* mData;
};

struct sListIter1A8AE8 {
    sListNode1A8AE8* mNode;
    sListIter1A8AE8(sListNode1A8AE8* x) : mNode(x) {}
    sListIter1A8AE8(const sListIter1A8AE8& x) : mNode(x.mNode) {}
};

static inline sListIter1A8AE8 ListInsert1A8AE8(sListIter1A8AE8 pos, void* const& x)
{
    sListNode1A8AE8* tmp = (sListNode1A8AE8*)operator new(0xC, D_0045CDE8, 0x20000000, 0);
    void** data = &tmp->mData;
    if (data != 0) {
        *data = x;
    }
    tmp->mNext = pos.mNode;
    tmp->mPrev = pos.mNode->mPrev;
    ((sListNode1A8AE8*)(pos.mNode->mPrev))->mNext = tmp;
    pos.mNode->mPrev = tmp;
    return tmp;
}

extern "C" void func_001A8AE8(void* self, cFEVObj_001A8AE8* obj)
{
    if (obj != 0) {
        void* item = obj->v02();
        if (item != 0) {
            ListInsert1A8AE8(*(sListNode1A8AE8**)((char*)self + 0x6CC), item);
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A8B88);

INCLUDE_ASM("fe/feasyncfile", func_001A8E40);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8F98);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern void* D_004A3328;
extern void* D_004A3028;
extern char D_00461618[];
extern char D_00461C40[];

static inline void* GetSession_001A8F98()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001A8F98(void* self)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 1;
    func_001C57C0((char*)p + 0xBC, 0, GetHashValue32(D_00461618));
    int h = GetHashValue32(D_00461C40);
    *(int*)((char*)p + 0xC4) = h;
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x83E;
    *(int*)((char*)p + 0x14C) = 6;
    func_001A9BC0(self, p);
    func_00261408(GetSession_001A8F98());
    func_0025B800(D_004A3028);
    func_002636F0(func_002591B8(), 0);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A91B0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001CB030(void* mem, void* engine, void* owner, int a3, unsigned char a4);
extern "C" void func_0039F290(void* list, void* item);
extern "C" void func_001CD088(void* self, int v);
extern char D_0045D8D8[];

extern "C" void func_001A91B0(void* self, int a1, int a2, int a3, int a4)
{
    void* p = func_001CB030(cMemMan_alloc(0x444, D_0045D8D8, 0x100, 0), *(void**)((char*)self + 0x10), self, a2, 0xF);
    *(void**)((char*)self + 0x69C) = p;
    func_0039F290(*(char**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, p);
    *(int*)((char*)*(void**)((char*)self + 0x69C) + 0x18) = a3;
    func_001CD088(*(void**)((char*)self + 0x69C), a4);
    *(int*)((char*)self + 0x6A4) = a1;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A92D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");

struct sVE1A92D8 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE1A92D8b {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001A92D8(void* self, int a1, unsigned int kind)
{
    switch (kind) {
    case 21:
        *(int*)((char*)self + 0x6BC) = 2;
        func_001A93D0_3(self, a1, 0x15);
        break;
    case 15:
        *(int*)((char*)self + 0x6BC) = 0;
        func_001A93D0_3(self, a1, 0xF);
        break;
    case 22: {
        sVE1A92D8* vt = *(sVE1A92D8**)((char*)self + 0x8);
        vt[32].fn((char*)self + vt[32].delta);
        if (*(int*)((char*)self + 0x6BC) == 0) {
            char* obj = *(char**)((char*)self + 0x20);
            sVE1A92D8b* vt2 = *(sVE1A92D8b**)(obj + 0x8);
            vt2[17].fn(obj + vt2[17].delta, self, 0xF);
        }
        func_001A93D0_3(self, a1, 0x16);
        break;
    }
    default:
        func_001A93D0_3(self, a1, kind);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001A93D0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9438);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cFEVObj_001A9438 {
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

// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001BEE30(void* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern const char D_00461CA0[];

extern "C" void func_001A9438(void* self, int a1, int a2)
{
    if (a2 == 0x16) {
        ((cFEVObj_001A9438*)self)->v32();
        func_001A8A40(self, func_001BEE30(cMemMan_alloc(0x6D4, D_00461CA0, 0, 0), *(int*)((char*)self + 0x10)));
        func_001A93D0_3(self, a1, 0x16);
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A94D8__FPv);
#ifdef SKIP_ASM
void func_001A94D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A94E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cFEVObj_001A94E0 {
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
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");

extern "C" void func_001A94E0(void* self, int a1, int a2)
{
    switch (a2) {
    case 0x15:
        func_001A93D0_3(self, a1, 0x15);
        break;
    case 0x16:
        ((cFEVObj_001A94E0*)self)->v32();
        *(int*)((char*)self + 0x69C) = 0;
        func_001A93D0_3(self, a1, 0x16);
        break;
    default:
        func_001A93D0_3(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9578);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
struct sFEObj_001B1F88; // defined with func_001B1F88 (its ctor)
extern "C" sFEObj_001B1F88* func_001B1F88(sFEObj_001B1F88* mem, int a1);
extern "C" void* func_001BFBB8(void* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_0025DA90(void* self);
extern void* D_004A3028;
extern int D_004A4E88;
extern const char D_0045FF10[];
extern const char D_0045FF28[];

extern "C" void func_001A9578(void* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A93D0_3(self, a1, a2);
        void* obj;
        if (func_0025DA90(D_004A3028) != 0 && D_004A4E88 == 0) {
            obj = func_001B1F88((sFEObj_001B1F88*)cMemMan_alloc(0x73C, D_0045FF10, 0, 0), *(int*)((char*)self + 0x10));
        } else {
            obj = func_001BFBB8(cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), *(int*)((char*)self + 0x10));
        }
        func_001A8A40(self, obj);
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9668);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001BEE30(void* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_0039F698(void* p);
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern const char D_00461CD8[];

extern "C" void func_001A9668(void* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A93D0_3(self, a1, a2);
        func_0039F698(*(char**)((char*)self + 0x10) + 0x18);
        *(int*)((char*)self + 0x58) = 1;
        void* st = cUIStateStack_getCurrentState(*(char**)((char*)self + 0x10) + 0x18);
        *(int*)((char*)st + 0x1C) |= 0x80;
        func_001A8A40(self, func_001BEE30(cMemMan_alloc(0x6D4, D_00461CD8, 0, 0), *(int*)((char*)self + 0x10)));
        *(int*)((char*)self + 0x6AC) = 0;
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9710);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B3F78(void* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_0039F698(void* p);
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern const char D_00460BF0[];

extern "C" void func_001A9710(void* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A93D0_3(self, a1, a2);
        func_0039F698(*(char**)((char*)self + 0x10) + 0x18);
        *(int*)((char*)self + 0x58) = 1;
        void* st = cUIStateStack_getCurrentState(*(char**)((char*)self + 0x10) + 0x18);
        *(int*)((char*)st + 0x1C) |= 0x80;
        func_001A8A40(self, func_001B3F78(cMemMan_alloc(0x91C, D_00460BF0, 0, 0), *(int*)((char*)self + 0x10)));
        *(int*)((char*)self + 0x6AC) = 0;
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9930);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void* func_001A9C50(void* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" int* func_002591B8();
extern "C" int func_0039F698(void* p);
extern char D_00461618[];
extern char D_00461C40[];
extern char D_00461D28[];

extern "C" void func_001A9930(void* self)
{
    if (*(int*)((char*)self + 0x6AC) == 0) {
        int one = 1;
        *(int*)((char*)self + 0x6AC) = one;
        func_0039F698(*(char**)((char*)self + 0x10) + 0x18);
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = one;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x170) = one;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x14C) = 6;
        if (*func_002591B8() == 0) {
            *(int*)((char*)p + 0x150) = 0x83E;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00461C40);
            *(int*)((char*)p + 0xC0) = 3;
        } else {
            *(int*)((char*)p + 0x150) = 0x83D;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00461D28);
            *(int*)((char*)p + 0xC0) = 3;
        }
        func_001A9BC0(self, p);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9A18);
#ifdef SKIP_ASM
class cUIListBox_001A9A18 {
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
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_0045DCC8[];
extern char D_0045FCD8[];

extern "C" void func_001A9A18(void* self, cUIListBox_001A9A18* box, int value)
{
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_0045DCC8), 1);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_0045FCD8), 0);
    func_0039A7A8(box, value == 0);
    box->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9AB8);
#ifdef SKIP_ASM
class cUIListBox_001A9AB8 {
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
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_0045E028[];
extern char D_0045DF40[];

extern "C" void func_001A9AB8(void* self, cUIListBox_001A9AB8* box, int value)
{
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_0045E028), 1);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_0045DF40), 0);
    func_0039A7A8(box, value == 0);
    box->v09(1);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9BC0);
#ifdef SKIP_ASM
extern "C" void* func_0039FAE8(void* list);
extern "C" void func_0039F290(void* list, void* item);
extern "C" void func_001C5DD8(void* self);

extern "C" void func_001A9BC0(void* self, void* popup)
{
    void* top = func_0039FAE8(*(char**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18);
    if (*(int*)((char*)popup + 0x2B8) == 0) {
        if (popup != top) {
            func_0039F290(*(char**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, popup);
        }
    } else {
        *(int*)((char*)popup + 0x1C) = (*(int*)((char*)popup + 0x1C) & ~0x3F00) | 0x500;
        func_001C5DD8(*(void**)((char*)self + 0x6A0));
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB3D0);
#ifdef SKIP_ASM
extern "C" void func_001AB478(void* self, void* popup, int a1, int a2, int a3);
extern "C" int func_001CA150(void* popup);

extern "C" void func_001AB3D0(void* self, int a1, int a2)
{
    if ((*(int*)((char*)self + 0x1C) & 6) != 0) {
        void* popup = *(void**)((char*)self + 0x6A0);
        if (popup != 0 && *(int*)((char*)self + 0x6C4) != 0) {
            int id = *(int*)((char*)popup + 0x18);
            if (id != 0x80B) {
                if (id != 0x80C) {
                    return;
                }
            }
            if ((*(unsigned char*)((char*)popup + 0x1D) & 0x3F) < 6) {
                func_001AB478(self, *(void**)((char*)self + 0x6A0), a1, a2, func_001CA150(popup));
            }
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AB478);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001ABB30);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00262A38(void* self, int a1);
extern "C" void* func_001A77A0(void* mem, int a1, void* owner, int a3);
extern "C" void func_0039F290(void* list, void* item);
extern "C" void func_001CE3C8(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int v);
extern void* D_004A3328;
extern void* D_004A3028;
extern char D_004A1408[];
extern char D_00462AA8[];

struct sFEObj_001ABB30 {
    char pad0[0x10];
    int f10;
    char pad14[0x2C];
    char* f40;
    char pad44[0x658];
    char* f69C;
    char pad6A0[0x24];
    int f6C4;
};

// PORT: passes D_004A1408 through an int parameter.
extern "C" void func_001ABB30(void* p, int a1, void* a2, int a3)
{
    sFEObj_001ABB30* self = (sFEObj_001ABB30*)p;
    self->f6C4 = 0;
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    if (func_00262A38(D_004A3328, a1) != 0) {
        void* obj = func_001A77A0(cMemMan_alloc(0x6FC, D_00462AA8, 0x100, 0), self->f10, self, a1);
        func_0039F290(*(char**)(*(char**)(self->f40 + 0xD0) + 0x10) + 0x18, obj);
    } else {
        func_001A9090(self, 0, a1, (int)D_004A1408, 0, 2, 0x3F);
        *(int*)(self->f69C + 0x43C) = 0;
        func_001CE3C8(self->f69C, 0x4B, 1, 2);
        func_001CE468(self->f69C, 1);
        *(int*)(self->f69C + 0x438) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABC50);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261EF8(void* self, int a1, int a2, int a3);
extern void* D_004A3328;
extern void* D_004A3028;

struct sFEObj_001ABC50 {
    char pad[0x6C4];
    int f6C4;
};

extern "C" void func_001ABC50(sFEObj_001ABC50* self, int a1, int a2)
{
    self->f6C4 = 0;
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00261EF8(D_004A3328, a1, 1, a2);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001ABCD0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABE58);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261EF8(void* self, int a1, int a2, int a3);
extern void* D_004A3328;
extern void* D_004A3028;

struct sFEObj_001ABE58 {
    char pad[0x6C4];
    int f6C4;
};

extern "C" void func_001ABE58(sFEObj_001ABE58* self, int a1, int a2)
{
    self->f6C4 = 0;
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00261EF8(D_004A3328, a1, 0, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABED8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern char D_00461618[];
extern char D_00462B00[];

extern "C" void func_001ABED8(void* self)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 1;
    func_001C57C0((char*)p + 0xBC, 0, GetHashValue32(D_00461618));
    int h = GetHashValue32(D_00462B00);
    //S
    *(int*)((char*)p + 0xC4) = h;
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x845;
    *(int*)((char*)p + 0x14C) = 6;
    //E
    func_001A9BC0(self, p);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC010);
#ifdef SKIP_ASM
extern void* D_00469068[];
extern int D_004A3E90;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001AC010(void* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    int d = D_004A3E90;
    *(void***)((char*)self + 0x8) = D_00469068;
    *(int*)((char*)self + 0x6DC) = d;
    *(int*)((char*)self + 0xC) = 0x35;
    *(int*)((char*)self + 0x6D0) = 0;
    *(int*)((char*)self + 0x6E0) = 0;
    *(int*)((char*)self + 0x6A8) = 0;
    return self;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC0B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void func_001A8768(void* self);
extern char D_00462B20[];
extern char D_004A1408[];
extern void* D_004A3028;

extern "C" void func_001AC0B8(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00462B20), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cBXString_cBXString4((char*)D_004A3028 + 0x48, D_004A1408);
    cBXString_cBXString4((char*)D_004A3028 + 0x4C, D_004A1408);
    func_001A8768(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC150__FPv);
#ifdef SKIP_ASM
void* func_001AC150(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AC170);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC2B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern void* D_004A3028;
extern char D_00462BB0[];
extern char D_00461618[];

extern "C" int func_001AC2B8(void* self)
{
    if (strlen(*(const char**)((char*)D_004A3028 + 0x4C)) == 0) {
    void* p = func_001A9C50(self);
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BB0);
    *(int*)((char*)p + 0xC0) = 3;
    func_001C57C0(sub, 0, GetHashValue32(D_00461618));
    *(int*)((char*)p + 0x150) = 0x80D;
    func_001A9BC0(self, p);
    return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC360);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern void* D_004A3028;
extern char D_00462BD0[];
extern char D_00461618[];

extern "C" int func_001AC360(void* self)
{
    if (strlen(*(const char**)((char*)D_004A3028 + 0x48)) == 0) {
    void* p = func_001A9C50(self);
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BD0);
    *(int*)((char*)p + 0xC0) = 3;
    func_001C57C0(sub, 0, GetHashValue32(D_00461618));
    *(int*)((char*)p + 0x150) = 0x80D;
    func_001A9BC0(self, p);
    return 0;
    }
    return 1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AC408);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC448);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void* func_001A9C50(void* self);
void func_001C57C0(void* self, int a1, int a2);
extern char D_00462BF0[];
extern char D_00461618[];

extern "C" int func_001AC448(void* self)
{
    // PORT: string header read through a pointer held in an int
    if (*(int*)(*(int*)((char*)self + 0x6DC) - 8) == 0) {
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BF0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0(self, p);
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC4F0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00462C08[];
extern char D_00462C28[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001AC4F0(void* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00462C08);
    func_001DA530(popup, 0, GetHashValue32(D_00462C28));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 2, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC5A8);
#ifdef SKIP_ASM
extern "C" int func_001AC5A8(void* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AC5C0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC920);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001AC9C0(void* self, void* a1, int a2);
extern "C" void func_001ACA88(void* self, void* a1, int a2);
extern "C" void func_001ACB48(void* self, void* a1, int a2);
extern "C" void func_001ACCD0(void* self, void* a1, int a2);

extern "C" void func_001AC920(void* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x80E:
        func_001ACA88(self, a1, a2);
        break;
    case 0x80F:
        func_001AC9C0(self, a1, a2);
        break;
    case 0x82C:
        func_001ACB48(self, a1, a2);
        break;
    case 0x81E:
        func_001ACCD0(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC9C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" int func_001AC448(void* self);
extern "C" void func_0025B6D8(void* self, int a1);
extern void* D_004A3028;
extern char D_00462D48[];

extern "C" void func_001AC9C0(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        int three = 3;
        func_001A97B8(self, a1, 0xF);
        cBXString_cBXString4((char*)self + 0x6DC, *(const char**)((char*)a1 + 0x2B0));
        void* p = func_001A9C50(self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D48);
        *(int*)((char*)p + 0xC0) = three;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 0x810;
        func_001A9BC0(self, p);
        if (func_001AC448(self) != 0) {
            *(int*)((char*)self + 0x6D8) = three;
            func_0025B6D8(D_004A3028, 0);
        }
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ACA88);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001ACA88()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001ACA88(void* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_00261408(GetSession_001ACA88());
        func_0025B800(D_004A3028);
        func_001A97B8(self, a1, a2);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001ACB48);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ACCD0);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* func_001AD130(void* self, int a1);
extern "C" void* func_001B6ED0(void* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void func_0039F290(void* list, void* item);
extern void* D_004A3028;
extern char D_00462D10[];
extern const char D_00462D68[];

extern "C" void func_001ACCD0(void* self, void* a1, int kind)
{
    switch ((unsigned int)kind) {
    case 15: {
        func_001A97B8(self, a1, 15);
        void* p = func_001B6ED0(cMemMan_alloc(0xB10, D_00462D10, 0, 0), *(int*)((char*)self + 0x10), self, 1,
                                *(int*)((char*)D_004A3028 + 0x48), *(int*)((char*)D_004A3028 + 0x4C));
        *(int*)((char*)p + 0x18) = 0x82C;
        func_0039F290(*(char**)(*(char**)(*(char**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, p);
        break;
    }
    case 16:
    case 20:
        func_001A8A40(self, func_001AD130(cMemMan_alloc(0x6D8, D_00462D68, 0, 0), *(int*)((char*)self + 0x10)));
        break;
    default:
        func_001A97B8(self, a1, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ACDD0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void func_001A98B8(void* self, const char* str);
extern void* D_004A3028;
extern char D_00462B88[];
extern char D_00462BA0[];

struct sVE1ACDD0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void Show1ACDD0(char* o, int on)
{
    sVE1ACDD0* vt = *(sVE1ACDD0**)(o + 0x8);
    vt[8].fn(o + vt[8].delta, on);
}

extern "C" void func_001ACDD0(void* self)
{
    char* p = *(char**)((char*)self + 0x69C);
    switch (*(int*)(p + 0x18)) {
    case 6: {
        const char* name = p + 0x74;
        cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00462B88));
        if (text != 0) {
            cUIText_setAsciiString(text, name);
        }
        if (strlen(name) != 0) {
            Show1ACDD0(*(char**)((char*)self + 0x6E0), 0);
        } else {
            Show1ACDD0(*(char**)((char*)self + 0x6E0), 1);
        }
        cBXString_cBXString4((char*)D_004A3028 + 0x48, name);
        break;
    }
    case 7: {
        const char* name = p + 0x74;
        if (cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00462BA0)) != 0) {
            func_001A98B8(self, name);
            cUIText_setAsciiString(*(cUIText**)((char*)self + 0x6A4), (char*)self + 0x5C);
        }
        cBXString_cBXString4((char*)D_004A3028 + 0x4C, name);
        break;
    }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD208);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025D1B8(void* self, const char* str);
extern void* D_004A3028;
extern char D_004615F8[];
extern char D_00461618[];
extern char D_00462DE0[];

extern "C" void func_001AD208(void* self, const char* name)
{
    if (strlen(name) < 3) {
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004615F8);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x814;
        func_001A9BC0(self, p);
    } else {
        void* p = func_001A9C50(self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462DE0);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x815;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0(self, p);
        func_0025D1B8(D_004A3028, name);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD308);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void func_001AD3E8(void* self, cUIText* text);
extern void* D_004A3028;
extern char D_00462E00[];
extern char D_00462E10[];
extern char D_00462E20[];
extern char D_00462E30[];
extern char D_00462E40[];

static inline int IsHash1AD308(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001AD308(void* self, cUIText* text)
{
    void* g = D_004A3028;
    if (IsHash1AD308(*(int*)((char*)text + 0x38), D_00462E00)) {
        func_001AD3E8(self, text);
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), D_00462E10)) {
        *(int*)((char*)text + 0x18) = -1;
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), D_00462E20)) {
        cUIText_setAsciiString(text, *(const char**)((char*)g + 0x48));
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), D_00462E30)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00462E40));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD3E8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" cUIText* func_00397870(void* list, int i);
extern void* D_004A3028;
extern char D_00462E58[];

class cFEVObj_001AD3E8 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

struct sFEGlobals1AD3E8 {
    char pad[0x6C];
    int count;
    const char* names[4];
};

extern "C" void func_001AD3E8(void* self, cUIText* text)
{
    sFEGlobals1AD3E8* g = (sFEGlobals1AD3E8*)D_004A3028;
    int i = 0;
    for (; i < g->count; i++) {
        cUIText* item = func_00397870((char*)text + 0x74, i);
        *(int*)((char*)item + 0x18) = i;
        cUIText_setAsciiString(item, g->names[i]);
        ((cFEVObj_001AD3E8*)item)->v09(1);
    }
    for (; i < 4; i++) {
        cUIText* item = func_00397870((char*)text + 0x74, i);
        *(int*)((char*)item + 0x18) = i;
        cUIText_setUnicodeStringByID(item, GetHashValue32(D_00462E58));
        ((cFEVObj_001AD3E8*)item)->v09(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD4F0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00462E70[];
extern char D_00462E88[];
extern char D_00462EA0[];
extern char D_0045FE60[];
extern char D_00462EB8[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001AD4F0(void* self, void* menu)
{
    func_001DA528(menu, 4);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00462E70);
    func_001DA530(menu, 0, GetHashValue32(D_00462E88));
    func_001DA530(menu, 1, GetHashValue32(D_00462EA0));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_00462EB8), 2);
    func_001DA510(menu, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 3, GetHashValue32(D_004614E8), 1);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD930);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B6ED0(void* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void func_0039F290(void* list, void* item);
extern char D_00462D10[];

extern "C" void func_001AD930(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(self, a1, 0xF);
        void* p = func_001B6ED0(cMemMan_alloc(0xB10, D_00462D10, 0, 0), *(int*)((char*)self + 0x10), self, 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        func_0039F290(*(char**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, p);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD9C8);
#ifdef SKIP_ASM
extern void* D_004A3028;
extern "C" void func_0025D428(void* self, void* a1);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001AD9C8(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_0025D428(D_004A3028, *(void**)((char*)self + 0x6D4));
        func_001A97B8(self, a1, 0xF);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001ADA30);

INCLUDE_ASM("fe/feasyncfile", func_001ADC30);

INCLUDE_ASM("fe/feasyncfile", func_001ADDD0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ADE88);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" void func_001AF728(void* self, void* iface, void* screen);
extern "C" void func_001AF688(void* self);
extern "C" void func_001AF930(void* screen);
extern "C" void func_0025E6D0(void* self, const char* str);
extern "C" void func_001A9930(void* self);
extern void* D_004A3028;
extern int D_004A4E8C;
extern char D_004630C8[];
extern char D_004630E0[];

struct sVE1ADE88 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE1ADE88i {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001ADE88(void* self)
{
    sVE1ADE88* vt = *(sVE1ADE88**)((char*)self + 0x8);
    vt[32].fn((char*)self + vt[32].delta);
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004630C8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
        cBENewRaceInterface_setNumberAI(iface, D_004A4E8C);
        sVE1ADE88i* ivt = *(sVE1ADE88i**)(iface + 0xC);
        ivt[1].fn(iface + ivt[1].delta);
        func_001AF728(self, iface, *(void**)((char*)self + 0x40));
        func_001AF688(self);
        func_0025E6D0(D_004A3028, *(const char**)((char*)D_004A3028 + 0x98));
        func_001AF930(*(void**)((char*)self + 0x40));
        if (*(int*)((char*)self + 0x6D4) == 0) {
            cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004630E0)), 6);
        } else {
            *(int*)((char*)self + 0x6D4) = 0;
        }
    }
    if (*(int*)D_004A3028 == 0) {
        func_001A9930(self);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AE680);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" int func_0039A738(void* a);
extern void* D_004A28A8;
// D_00534B34 is a splat label inside D_00534B30
extern int D_00534B34[];
extern char D_005308B8[];

struct sVE1AE680 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE1AE680i {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sOpts1AE680 {
    char pad[0x18];
    unsigned int f0 : 6;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int b8 : 1;
};

extern "C" void func_001AE680(void* self)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        char* iface = (char*)cBE_getInterface_Fv(*(void**)((char*)D_004A28A8 + 0x78), 7);
        D_00534B34[0] = func_0039A738(*(void**)((char*)self + 0x6DC)) != 0;
        sVE1AE680* vt = *(sVE1AE680**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        int v = func_0039A738(*(void**)((char*)self + 0x6E8));
        sOpts1AE680* o = (sOpts1AE680*)D_005308B8;
        o->b6 = v;
        o->b8 = func_0039A738(*(void**)((char*)self + 0x6E4));
        o->b7 = func_0039A738(*(void**)((char*)self + 0x6EC));
        char* nr = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
        cBENewRaceInterface_setNumberAI(nr, func_0039A738(*(void**)((char*)self + 0x6E0)));
        sVE1AE680i* nvt = *(sVE1AE680i**)(nr + 0xC);
        nvt[1].fn(nr + nvt[1].delta);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AE7A0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AEBF0);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

extern "C" void func_001AEBF0()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00262768(D_004A3328, 1, 7, *(int*)((char*)D_004A3028 + 0x98), 0, 0);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF568);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void func_001AEC60(void* self, int a1);
extern "C" void func_002C26D0(void* str, int a1, char* buf);
extern "C" void func_003A0E90(cUIText* text, char* str);
extern void* D_004A28A8;
extern void* D_004A3028;
extern char D_00463468[];
extern char D_00463480[];
extern char D_004631C8[];
extern char D_004630E0[];

struct sVE_001AF568 {
    short delta;
    short index;
    int (*fn)(void* self, int hash);
};

struct sVE_001AF568b {
    short delta;
    short index;
    void (*fn)(void* self, int on);
};

extern "C" void func_001AF568(void* self)
{
    char buf[0x400];
    *(int*)((char*)self + 0x6D0) = 1;
    cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00463468));
    if (text != 0) {
        char* name = *(char**)((char*)D_004A3028 + 0x98);
        char* o = *(char**)((char*)D_004A28A8 + 0x8C);
        sVE_001AF568* vt = *(sVE_001AF568**)(o + 4);
        char* obj = o + vt[4].delta;
        func_002C26D0(buf, vt[4].fn(obj, GetHashValue32(D_00463480)), name);
        func_003A0E90(text, buf);
        char* o2 = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004631C8));
        sVE_001AF568b* vt2 = *(sVE_001AF568b**)(o2 + 8);
        vt2[8].fn(o2 + vt2[8].delta, 1);
        cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004630E0)), 7);
        func_001AEC60(self, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF688);
#ifdef SKIP_ASM
class cUIObj_001AF688 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int a1);
};

int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void func_001AEC60(void* self, int a1);
extern char D_00463468[];
extern char D_00463498[];
extern char D_004631C8[];

extern "C" void func_001AF688(void* self)
{
    *(int*)((char*)self + 0x6D0) = 0;
    cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00463468));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00463498));
        ((cUIObj_001AF688*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004631C8)))->v08(0);
        func_001AEC60(self, 0);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AF728);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF930);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_004A1390[];
extern char D_00463108[];
extern char D_004634E8[];
extern char D_004634F8[];
extern char D_00463508[];
extern char D_00463520[];
extern char D_00463538[];

static inline void Clear1AF930(void* screen, char* name)
{
    cUIText* t = cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
    if (t != 0) {
        cUIText_setAsciiString(t, D_004A1390);
    }
}

extern "C" void func_001AF930(void* screen)
{
    if (screen != 0) {
        Clear1AF930(screen, D_00463108);
        Clear1AF930(screen, D_004634E8);
        Clear1AF930(screen, D_004634F8);
        Clear1AF930(screen, D_00463508);
        Clear1AF930(screen, D_00463520);
        Clear1AF930(screen, D_00463538);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AFE08);
#ifdef SKIP_ASM
extern void* D_00468D20[];
extern int D_004A4E88;
extern "C" void* cBXString_cBXString2(void* self, const char* str);
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001AFE08(void* self, int a1, int a2, const char* name)
{
    func_001A8500_3(self, a1, a2);
    *(void***)((char*)self + 0x8) = D_00468D20;
    cBXString_cBXString2((char*)self + 0x6D0, name);
    *(int*)((char*)self + 0x6D4) = 0;
    *(int*)((char*)self + 0xC) = 0x40;
    D_004A4E88 = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001AFE68);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B00B8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

extern "C" void func_001B00B8(void* self)
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00262768(D_004A3328, 1, 8, *(int*)((char*)self + 0x6D0), 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0130__FPv);
#ifdef SKIP_ASM
void* func_001B0130(void* self)
{
    return func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B0150);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0538);
#ifdef SKIP_ASM
extern "C" void func_001A8918(void* self, int a1, int msg);
extern "C" void func_001B0C08(void* self);
extern "C" void func_001B0E10(void* self);
extern "C" void func_0025FB58(void* self, int a1, int a2);
extern void* D_004A3028;

extern "C" void func_001B0538(void* self, void* obj, int msg)
{
    if (obj == 0) {
        return;
    }
    switch (msg) {
    case 5:
        switch (*(int*)((char*)obj + 0x18)) {
        case 0xB:
            if (*(int*)((char*)self + 0x6D4) == 0) {
                func_001B0C08(self);
                func_0025FB58(D_004A3028, *(int*)((char*)D_004A3028 + 0x98), *(signed char*)((char*)self + 0x14));
            }
            break;
        case 0xC:
            func_001B0E10(self);
            break;
        }
        break;
    case 7:
        break;
    default:
        // PORT: the unit declares func_001A8918 with an int a1; obj is passed through as int
        func_001A8918(self, (int)obj, msg);
        break;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0C08);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_00463618[];
extern char D_004630E0[];
extern char D_00463468[];
extern char D_00462750[];
extern char D_004635F8[];

class cFEVObj_001B0C08 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int on);
    virtual void v08(int on);
    virtual void v09(int on);
};

extern "C" void func_001B0C08(void* self)
{
    *(int*)((char*)self + 0x6D4) = 1;
    if (*(void**)((char*)self + 0x40) != 0) {
        cFEVObj_001B0C08* o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00463618));
        if (o != 0) {
            o->v07(1);
        }
        cUIText* menu = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004630E0));
        if (menu != 0) {
            cUIMenu_setSelectedByIndex(menu, 1);
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00463468));
        if (o != 0) {
            o->v09(1);
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(D_00462750));
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004635F8));
        if (o != 0) {
            o->v08(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0E10);
#ifdef SKIP_ASM
extern "C" void func_001B0C08(void* self);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_0025F740(void* self, int a1);
extern "C" void func_00263018(void* self, void* a1);
extern "C" void* func_002591D0();
extern "C" void func_00263BA8(void* p, int a1);
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001B0E10()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

struct sVE1B0E10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001B0E10(void* self)
{
    func_001B0C08(self);
    if (*(int*)((char*)self + 0x6D4) != 0) {
        func_0025F740(D_004A3028, 0);
        func_00263018(GetSession_001B0E10(), *(void**)((char*)D_004A3028 + 0x98));
        func_00263BA8(func_002591D0(), 2);
    } else {
        func_00263018(GetSession_001B0E10(), *(void**)((char*)D_004A3028 + 0x98));
    }
    char* obj = *(char**)((char*)self + 0x20);
    sVE1B0E10* vt = *(sVE1B0E10**)(obj + 0x8);
    vt[17].fn(obj + vt[17].delta, self, 0x14);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0FB0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001B1B98(void* self, int index, int a2, int a3);
extern char D_00463800[];

extern "C" void func_001B0FB0(void* self)
{
    int i;
    *(int*)((char*)self + 0x6E0) = 0;
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00463800), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    for (i = 0; i < 30; i++) {
        func_001B1B98(self, i, 1, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1048__FPv);
#ifdef SKIP_ASM
void* func_001B1048(void* self)
{
    return func_001A8770(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1068);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_0039A738(void* a);
extern "C" void func_001B1B10(void* self, int sel, int flag);
extern "C" void func_001B1760(void* self, int index);
extern "C" void func_001B1808(void* self);
extern void* D_004A28A8;
extern char D_00534B30[];

extern "C" int func_001B1068(void* self, int a1)
{
    if (a1 != 0) {
        func_001B1B10(self, func_0039A738(*(void**)((char*)self + 0x6D0)), 0);
        cBE_getInterface_Fv(*(void**)((char*)D_004A28A8 + 0x78), 7);
        char* net = D_00534B30;
        if (*(int*)(net + 0x32C) >= 0) {
            func_001B1760(self, *(int*)(net + 0x32C));
            func_001B1B10(self, *(int*)(net + 0x32C), 1);
        }
        func_001B1808(self);
        *(int*)((char*)self + 0x6DC) = 0;
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B10F8);

INCLUDE_ASM("fe/feasyncfile", func_001B1280);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B14D8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00463848[];
extern char D_00463868[];
extern char D_00463890[];
extern char D_004638B0[];
extern char D_004614E8[];

extern "C" void func_001B14D8(void* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00463848);
    func_001DA530(popup, 0, GetHashValue32(D_00463868));
    func_001DA510(popup, 0, GetHashValue32(D_00463890), 4);
    func_001DA510(popup, 1, GetHashValue32(D_004638B0), 0);
    func_001DA510(popup, 2, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1590);
#ifdef SKIP_ASM
class cFEVObj_001B1590 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual bool v06();
};

extern void* D_004A33CC;

extern "C" bool func_001B1590(void* self, cFEVObj_001B1590* obj)
{
    if (*(int*)((char*)self + 0x6DC) != 0 || D_004A33CC == 0) {
        return !obj->v06();
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B15D8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1760);
#ifdef SKIP_ASM
class cFEVText_001B1760 {
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
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014EEC8(void* self, int player, int index);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_00463830[];

extern "C" void func_001B1760(void* self, int player)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        const char* str = (const char*)func_0014EEC8(cBE_getInterface_Fv(cBE_getBE(), 2), player, 0);
        cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00463830));
        ((cFEVText_001B1760*)text)->v09(1);
        cUIText_setAsciiString(text, str);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B1808);

INCLUDE_ASM("fe/feasyncfile", func_001B1A08);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1B10);
#ifdef SKIP_ASM
extern "C" void func_001B1760(void* self, int index);
extern "C" void func_001B1B98(void* self, int index, int a2, int a3);

extern "C" void func_001B1B10(void* self, int sel, int flag)
{
    int i;
    for (i = 0; i < 30; i++) {
        if (i == sel && flag) {
            func_001B1760(self, i);
        }
        func_001B1B98(self, i, flag, i == sel);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1F18);
#ifdef SKIP_ASM
extern void* D_004A33CC;
extern "C" void func_00265768(void* self, int a1);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001B1F18(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        if (D_004A33CC != 0) {
            func_00265768(D_004A33CC, 0xF9);
        }
        func_001A97B8(self, a1, 0xF);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1F88);
#ifdef SKIP_ASM
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern "C" char* strcpy(char* dst, const char* src);
extern void* D_0046AE58[];
extern void* D_004A3028;

struct sCol_001B1F88 {
    float r, g, b, a;
};

struct sFEObj_001B1F88 {
    char pad0[0x8];
    void** vt;                     // 0x8
    int fC;                        // 0xC
    char pad10[0x6EC - 0x10];
    int f6EC;                      // 0x6EC
    int f6F0;                      // 0x6F0
    int f6F4;                      // 0x6F4
    int f6F8;                      // 0x6F8
    int items[2];                  // 0x6FC
    int items2[2];                 // 0x704
    char name[0x20];               // 0x70C
    sCol_001B1F88 color;           // 0x72C
};

extern "C" sFEObj_001B1F88* func_001B1F88(sFEObj_001B1F88* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->vt = D_0046AE58;
    self->fC = 0x3D;
    self->f6F8 = 0;
    for (int i = 0; i < 2; i++) {
        self->items[i] = 0;
        self->items2[i] = 0;
    }
    self->f6EC = 0;
    self->f6F0 = 0;
    self->f6F4 = 0;
    sCol_001B1F88 c;
    c.r = 1.0f;
    c.g = 1.0f;
    c.b = 1.0f;
    c.a = 1.0f;
    self->color = c;
    strcpy(self->name, *(const char**)((char*)D_004A3028 + 0x80));
    return self;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B22A0);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_0025DA90(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001B22A0()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001B22A0()
{
    func_00262768(GetSession_001B22A0(), 1, 3, func_0025DA90(D_004A3028), 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B2320);
#ifdef SKIP_ASM
extern void* D_004A5B64;
extern "C" void func_001B3BF0(void* self);
extern "C" void* func_001A8770(void* self);

extern "C" void func_001B2320(void* self)
{
    if (*(int*)((char*)D_004A5B64 + 0x18) % 10000 == 0) {
        func_001B3BF0(self);
    }
    func_001A8770(self);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B2378);

INCLUDE_ASM("fe/feasyncfile", func_001B25E0);

INCLUDE_ASM("fe/feasyncfile", func_001B27B0);

INCLUDE_ASM("fe/feasyncfile", func_001B2CD0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B2DA0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00463B70[];
extern char D_00463B88[];
extern char D_00463BA8[];
extern char D_00463BC0[];
extern char D_00463BD8[];
extern char D_00461458[];
extern char D_004614E8[];
extern char D_00463BF0[];

extern "C" void func_001B2DA0(void* self, void* menu)
{
    func_001DA528(menu, 6);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00463B70);
    func_001DA530(menu, 0, GetHashValue32(D_00463B88));
    func_001DA510(menu, 0, GetHashValue32(D_00463BA8), 3);
    func_001DA510(menu, 1, GetHashValue32(D_00463BC0), 0);
    func_001DA510(menu, 2, GetHashValue32(D_00463BD8), 6);
    func_001DA510(menu, 3, GetHashValue32(D_00461458), 2);
    func_001DA510(menu, 4, GetHashValue32(D_004614E8), 1);
    func_001DA510(menu, 5, GetHashValue32(D_00463BF0), 8);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B2EB8);

INCLUDE_ASM("fe/feasyncfile", func_001B31A0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B33D8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_0025AAE0(void* self, void* a1);
extern "C" void func_00262690(void* self, int a1, void* a2);
int func_001A98B0(void* self);
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001B33D8()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001B33D8(void* self)
{
    char* p = *(char**)((char*)self + 0x69C);
    switch (*(int*)(p + 0x18)) {
    case 4:
        func_0025AAE0(D_004A3028, p + 0x74);
        break;
    case 2:
        func_00262690(GetSession_001B33D8(), func_001A98B0(self), *(char**)((char*)self + 0x69C) + 0x74);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B3490);
#ifdef SKIP_ASM
class cFEVObj_001B3490 {
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

struct sFEObj_001B3490 {
    char pad[0x6EC];
    void* f6EC;                    // 0x6EC
    int pad6F0;
    void* f6F4;                    // 0x6F4
    int pad6F8;
    cFEVObj_001B3490* items[2];    // 0x6FC
};

extern void* D_004A3028;
extern "C" void func_0039DE68(void* a, int b);
extern "C" void func_001B3978(void* self, void* a1);

extern "C" int func_001B3490(sFEObj_001B3490* self, int a1)
{
    if (a1 != 0) {
        int i;
        func_0039DE68(self->f6EC, 0);
        func_0039DE68(self->f6F4, 0);
        for (i = 0; i < 2; i++) {
            if (self->items[i] != 0) {
                self->items[i]->v07(1);
            }
        }
        func_001B3978(self, *(void**)((char*)D_004A3028 + 0x80));
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B38D0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9150(void* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void func_001CE3C8(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int v);
extern char D_00463DB8[];
extern char D_004A1408[];

extern "C" int func_001B38D0(void* self, void* obj)
{
    sVE1A4170* vt = *(sVE1A4170**)((char*)obj + 0x8);
    if (vt[27].fn((char*)obj + vt[27].delta) != 0) {
        // PORT: string pointer passed through an int parameter (func_001A9150's a3)
        func_001A9150(self, 0, GetHashValue32(D_00463DB8), (int)D_004A1408, 0, 4, 0x3F);
        *(int*)((char*)*(void**)((char*)self + 0x69C) + 0x43C) = 0;
        func_001CE468(*(void**)((char*)self + 0x69C), 1);
        func_001CE3C8(*(void**)((char*)self + 0x69C), 0x4B, 1, 2);
        *(int*)((char*)*(void**)((char*)self + 0x69C) + 0x438) = 1;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B3978);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" int func_0025F1B0(void* self);
extern "C" const char* func_0025F1E0(void* self, int idx, int a2);
extern "C" int func_004165A8(const char* a, const char* b);
extern void* D_004A3028;
extern char D_00463A38[];

extern "C" void func_001B3978(void* self, void* name)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        int sel = -1;
        int i = 0;
        int n = func_0025F1B0(D_004A3028);
        for (; i < n; i++) {
            if (func_004165A8(func_0025F1E0(D_004A3028, i, 0), (const char*)name) == 0) {
                sel = i;
                break;
            }
        }
        if (sel >= 0) {
            cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00463A38)), sel);
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B3A30);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B3BF0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_001AAD50(void* self, int a1, int a2);
extern "C" int func_0025F1B0(void* self);
extern "C" int func_0025F248(void* self, int idx);
extern "C" void func_0025F288(void* self, int idx, void* info);
extern void* D_004A3028;
extern char D_00463A60[];

class cFEVObj_001B3BF0 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

struct sInfo1B3BF0 {
    int f00;
    int f04;
    int f08;
    int f0C;
    int f10;
    int pad[3];
};

struct sOwner1B3BF0 {
    char pad0[0x40];
    void* screen;
    char pad44[0x68C];
    cFEVObj_001B3BF0* items[6];
    char pad6E8[0x1C];
    int first;
};

extern "C" void func_001B3BF0(void* p)
{
    sOwner1B3BF0* self = (sOwner1B3BF0*)p;
    char buf[32];
    sInfo1B3BF0 info;
    int n = func_0025F1B0(D_004A3028);
    int i;
    for (i = 0; i < 6; i++) {
        int idx = self->first + i;
        sprintf(buf, D_00463A60, i);
        if (idx < n) {
            int r = func_0025F248(D_004A3028, idx);
            self->items[i]->v09(1);
            if (r != 0) {
                self->items[i]->v09(0);
            } else {
                func_0025F288(D_004A3028, idx, &info);
                func_001AAD50(self->screen, i, info.f10);
            }
        } else {
            self->items[i]->v09(0);
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B3D20);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B3F78);
#ifdef SKIP_ASM
class cFEVObj_001B3F78 {
public:
    virtual void v01();
    virtual void v02();
};

extern void* D_00468B00[];
extern cFEVObj_001B3F78* D_004A1B78;
extern "C" cFEVObj_001B3F78* func_002C22C8();
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001B3F78(void* mem, int a1)
{
    func_001A8500_3(mem, a1, 0);
    *(int*)((char*)mem + 0x6D0) = 0;
    *(int*)((char*)mem + 0x6D4) = 0;
    *(int*)((char*)mem + 0x6D8) = 0;
    *(int*)((char*)mem + 0x6E4) = 0;
    *(int*)((char*)mem + 0x6E8) = 0;
    *(int*)((char*)mem + 0x6F4) = 0;
    *(int*)((char*)mem + 0x6F8) = 0;
    *(int*)((char*)mem + 0x6FC) = 0;
    *(int*)((char*)mem + 0x700) = 0;
    *(int*)((char*)mem + 0x704) = 0;
    *(int*)((char*)mem + 0x708) = 0;
    *(int*)((char*)mem + 0x710) = 0;
    *(int*)((char*)mem + 0x714) = 0;
    *(int*)((char*)mem + 0x718) = 0;
    *(int*)((char*)mem + 0x6A8) = 0;
    *(void***)((char*)mem + 0x8) = D_00468B00;
    *(int*)((char*)mem + 0xC) = 0x31;
    cFEVObj_001B3F78* o = func_002C22C8();
    D_004A1B78 = o;
    o->v02();
    *(int*)((char*)mem + 0x6E0) = -1;
    return mem;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4018);
#ifdef SKIP_ASM
extern void* D_00468B00[];
void cMemMan_free(void* p);
extern "C" void func_002C2300(void* p);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");
// D_004A1B78 is declared earlier in the unit (cFEVObj_001B3F78*, see func_001B3F78)

extern "C" void func_001B4018(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00468B00;
    if (*(void**)((char*)self + 0x710) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x710));
        *(void**)((char*)self + 0x710) = 0;
    }
    if (*(void**)((char*)self + 0x714) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x714));
        *(void**)((char*)self + 0x714) = 0;
    }
    if (*(void**)((char*)self + 0x718) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x718));
        *(void**)((char*)self + 0x718) = 0;
    }
    if (*(void**)((char*)self + 0x704) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x704));
        *(void**)((char*)self + 0x704) = 0;
    }
    if (D_004A1B78 != 0) {
        func_002C2300(D_004A1B78);
        D_004A1B78 = 0;
    }
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B40D0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_00256C50();
extern "C" void func_001B5268(void* self, int a1);
extern "C" void* func_0028B180();
void func_0028FC38(void* self);
extern char D_00463ED0[];
extern int D_004A2EEC;

extern "C" void func_001B40D0(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00463ED0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    if (D_004A2EEC != 0) {
        func_00256C50();
    }
    func_001B5268(self, 0);
    func_0028FC38(func_0028B180());
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B43A8);
#ifdef SKIP_ASM
// D_004A1B78 is declared earlier in the unit (cFEVObj_001B3F78*, see func_001B3F78)
extern "C" void func_001B4888(void* self, int a1, int a2, int a3);
extern "C" void func_001B4CD8(void* self);
extern "C" void func_001B5068(void* self, int a1);

class cFEVObj_001B43A8 {
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
    virtual int v53();
    virtual void v54();
    virtual int v55();
};

extern "C" void func_001B43A8(void* self)
{
    int a = ((cFEVObj_001B43A8*)D_004A1B78)->v53();
    if (((cFEVObj_001B43A8*)D_004A1B78)->v55() == 0) {
        *(int*)((char*)self + 0x708) = 0;
        func_001B5068(self, -1);
        func_001B4888(self, 0x840, 0x14, -1);
    } else if (a != 0) {
        func_001B4CD8(self);
    } else {
        *(int*)((char*)self + 0x708) = 0;
        func_001B5068(self, -1);
        func_001B4888(self, 0x840, 2, -1);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B4470);

INCLUDE_ASM("fe/feasyncfile", func_001B45E8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B47C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void func_002C2718(char* buf, const char* fmt, int a2);
extern "C" void func_002C26D0(void* str, int a1, char* buf);
extern void* D_004A28A8;
extern char D_004A12C0[];
extern char D_00463F58[];

struct sVE_001B47C0 {
    short delta;
    short index;
    int (*fn)(void* self, int hash);
};

extern "C" void func_001B47C0(void* self, int a1)
{
    char buf[32];
    char* str = (char*)self + 0x71C;
    func_002C2718(buf, D_004A12C0, a1);
    char* o = *(char**)((char*)D_004A28A8 + 0x8C);
    sVE_001B47C0* vt = *(sVE_001B47C0**)(o + 4);
    char* obj = o + vt[4].delta;
    func_002C26D0(str, vt[4].fn(obj, GetHashValue32(D_00463F58)), buf);
    void* p = func_001A9C50(self);
    // stores
    *(int*)((char*)p + 0xBC) = 0;
    *(char**)((char*)p + 0xC4) = str;
    *(int*)((char*)p + 0xC0) = 1;
    *(int*)((char*)p + 0x154) = 1;
    *(int*)((char*)p + 0x150) = 0x83F;
    *(int*)((char*)p + 0x164) = 0;
    *(int*)((char*)p + 0x168) = 0;
    // end
    func_001A9BC0(self, p);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4C40);
#ifdef SKIP_ASM
extern int D_004A1B80;
extern "C" void func_001B47C0(void* self, int a1);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001B4C40(void* self, void* a1, unsigned int a2)
{
    switch (a2) {
    case 15:
        D_004A1B80 = 0;
        *(int*)((char*)self + 0x70C) = 1;
        func_001B47C0(self, 1);
        *(int*)((char*)self + 0x708) = 1;
        break;
    case 16:
    case 20:
        func_001A97B8(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B4CD8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4FA0);
#ifdef SKIP_ASM
extern "C" void func_001B4888(void* self, int a1, int a2, int a3);
extern "C" void func_001B5068(void* self, int a1);
extern "C" int func_0039F698(void* p);
extern int D_004A1B80;

extern "C" void func_001B4FA0(void* self, int err)
{
    switch (err) {
    case 0:
        func_0039F698(*(char**)((char*)self + 0x10) + 0x18);
        break;
    // PORT: GNU case ranges
    case -21 ... -16:
    case -14 ... -1:
        func_001B4888(self, 0x842, 0x18, D_004A1B80);
        break;
    case -15:
        func_001B4888(self, 0x841, 6, -1);
        break;
    case -200:
        func_001B4888(self, 0x841, 0x14, 0);
        break;
    }
    func_001B5068(self, -1);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B5068);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5268);
#ifdef SKIP_ASM
extern int D_004A1B80;
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void func_0039A670(void* self);
extern "C" void func_001B47C0(void* self, int a1);

struct sFEObj_001B5268 {
    char pad[0x6E4];
    void* f6E4;    // 0x6E4
    void* f6E8;    // 0x6E8
    char pad6EC[0x6F4 - 0x6EC];
    void* f6F4;    // 0x6F4
    char pad6F8[0x708 - 0x6F8];
    int f708;      // 0x708
    int f70C;      // 0x70C
};

extern "C" void func_001B5268(void* obj, int a1)
{
    sFEObj_001B5268* self = (sFEObj_001B5268*)obj;
    cUIMenu_setSelectedByIndex(self->f6F4, 0);
    D_004A1B80 = 0;
    self->f70C = 1;
    func_0039A670(self->f6E4);
    func_0039A670(self->f6E8);
    func_001B47C0(obj, 1);
    self->f708 = 1;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B52C8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B54B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00464030[];
extern char D_00464048[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_00464068[];
extern char D_004614E8[];

extern "C" void func_001B54B8(void* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00464030);
    func_001DA530(menu, 0, GetHashValue32(D_00464048));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 3, GetHashValue32(D_00464068), 2);
    func_001DA510(menu, 4, GetHashValue32(D_004614E8), 1);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B55B0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5740);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern void* D_004A3328;
extern void* D_004A3028;

class cFEVObj_001B5740 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int a1);
};

static inline void* GetSession_001B5740()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001B5740(void* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_00261408(GetSession_001B5740());
        func_0025B800(D_004A3028);
        func_002636F0(func_002591B8(), 0);
        (*(cFEVObj_001B5740**)((char*)self + 0x6EC))->v08(0);
        func_001A97B8(self, a1, a2);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B5830);

INCLUDE_ASM("fe/feasyncfile", func_001B5A58);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5BD8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* func_0028B180();
extern "C" void func_00286200(void* p);
extern "C" void func_00256088();
extern char D_00463F98[];

struct sColor1B5BD8 {
    float r, g, b, a;
    sColor1B5BD8(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cFEVObj_001B5BD8 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int on);
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11(const sColor1B5BD8& c);
};

struct sFlags1B5BD8 {
    unsigned int f0 : 8;
    unsigned int mode : 6;
};

extern "C" void func_001B5BD8(void* self, void* a1, unsigned int kind)
{
    switch (kind) {
    case 15: {
        func_001A97B8(self, a1, 15);
        func_00286200(func_0028B180());
        func_00256088();
        cFEVObj_001B5BD8* o = (cFEVObj_001B5BD8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00463F98));
        if (o != 0) {
            o->v06(0);
            o->v11(sColor1B5BD8(1.0f, 0.5f, 0.5f, 0.5f));
        }
        ((sFlags1B5BD8*)((char*)self + 0x1C))->mode = 7;
        break;
    }
    case 16:
    case 20:
        func_001A97B8(self, a1, kind);
        break;
    default:
        func_001A97B8(self, a1, kind);
        break;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6288);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void cMemMan_free(void* p);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int USTR_length(void* s);
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern void* D_004689F0[];
extern void* D_004A28A8;
extern char D_00464208[];
extern char D_00464220[];

struct sVE_001B6288 {
    short delta;
    short index;
    void* (*fn)(void* self, int hash);
};

struct sFEObj_001B6288 {
    char pad0[0x8];
    void** vt;          // 0x8
    int fC;             // 0xC
    char pad10[0x6A8 - 0x10];
    int f6A8;           // 0x6A8
    char pad6AC[0x6D0 - 0x6AC];
    int f6D0;           // 0x6D0
    int f6D4;           // 0x6D4
    char pad6D8[0x6E4 - 0x6D8];
    int f6E4;           // 0x6E4
    char pad6E8[0x6F4 - 0x6E8];
    int f6F4;           // 0x6F4
    int f6F8;           // 0x6F8
    void* f6FC;         // 0x6FC
};

extern "C" sFEObj_001B6288* func_001B6288(sFEObj_001B6288* self, int a1, int a2)
{
    func_001A8500_3(self, a1, a2);
    self->f6F4 = 2;
    self->vt = D_004689F0;
    self->f6F8 = 0x1E;
    self->fC = 0x44;
    self->f6D0 = 0;
    self->f6D4 = 0;
    self->f6E4 = 0;
    self->f6FC = 0;
    char* o = *(char**)((char*)D_004A28A8 + 0x8C);
    sVE_001B6288* vt = *(sVE_001B6288**)(o + 4);
    int n = USTR_length(vt[4].fn(o + vt[4].delta, GetHashValue32(D_00464208)));
    if (self->f6FC != 0) {
        cMemMan_free(self->f6FC);
        self->f6FC = 0;
    }
    self->f6FC = operator_new_tag((n + 5) * 2, D_00464220, 0x100, 0);
    self->f6A8 = 0;
    return self;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B65C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_004631E8[];
extern char D_00464278[];
extern char D_00464288[];
extern char D_004A1BA0[];
extern char D_004642A0[];

class cFEVObj_001B65C0 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline int IsHash1B65C0(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001B65C0(void* self, cUIText* text)
{
    cFEVObj_001B65C0* obj = (cFEVObj_001B65C0*)text;
    if (IsHash1B65C0(*(int*)((char*)text + 0x38), D_004631E8)) {
        *(cUIText**)((char*)self + 0x6D8) = text;
        obj->v09(0);
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), D_00464278)) {
        *(cUIText**)((char*)self + 0x6D4) = text;
        cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x6D4), GetHashValue32(D_00464288));
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), D_004A1BA0)) {
        *(cUIText**)((char*)self + 0x6DC) = text;
        obj->v09(0);
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), D_004642A0)) {
        *(cUIText**)((char*)self + 0x6E0) = text;
        obj->v09(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B66B8);
#ifdef SKIP_ASM
class cFEVObj_001B66B8 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
    virtual int v06();
};

extern "C" int func_001B66B8(void* self, cFEVObj_001B66B8* obj)
{
    if (*(int*)((char*)self + 0x6E4) == 2) {
        if (obj->v05() || obj->v06()) {
            *(int*)(*(char**)((char*)self + 0x6D0) + 0x14) = 1;
            *(int*)((char*)self + 0x6E4) = 4;
        }
    }
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6BB8);
#ifdef SKIP_ASM
class cFEVText_001B6BB8 {
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
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern char D_004A1BA8[];
extern char D_004A1BB0[];
extern char D_004A1BB8[];
extern char D_00464250[];
extern char D_004642C0[];

static inline int IsHash1B6BB8(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001B6BB8(void* self, void* obj)
{
    if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_004A1BA8)) {
        *(void**)((char*)self + 0x6DC) = obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_004A1BB0)) {
        *(void**)((char*)self + 0x6E0) = obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_004A1BB8)) {
        *(void**)((char*)self + 0x6E4) = obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_00464250)) {
        ((cFEVText_001B6BB8*)obj)->v09(1);
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_004642C0)) {
        *(int*)((char*)obj + 0x90) |= 8;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7650);
#ifdef SKIP_ASM
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);

extern "C" void func_001B7650(void* self, int a1, int a2, int b0, int b1, int a5, int a6)
{
    void* p = func_001A9C50(self);
    char* sub = (char*)p + 0xBC;
    if (b0 != 0) {
        if (b1 != 0) {
            *(int*)((char*)p + 0xBC) = 2;
            func_001C57C0(sub, 0, b0);
            func_001C57C0(sub, 1, b1);
            *(int*)((char*)p + 0x168) = a2;
        } else {
            *(int*)((char*)p + 0xBC) = 1;
            func_001C57C0(sub, 0, b0);
            *(int*)((char*)p + 0x168) = 0;
        }
    } else {
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x168) = 0;
    }
    if (a5 != 0) {
        *(int*)(sub + 0x10) = a5;
        *(int*)(sub + 0xC) = 1;
    }
    if (a6 != 0) {
        *(int*)(sub + 0x8) = a6;
        *(int*)(sub + 0x4) = 1;
    }
    *(int*)(sub + 0x94) = a1;
    *(int*)(sub + 0xA8) = 0;
    *(int*)(sub + 0x98) = 1;
    func_001A9BC0(self, p);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B77C8);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B77C8(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B77C8(void* self)
{
    if (IsMode_001B77C8(4)) {
        func_001B70D0(self, 4);
    } else if (IsMode_001B77C8(5)) {
        if (*(int*)((char*)self + 0x6D4) == 1) {
            func_001B70D0(self, 4);
        } else {
            func_001B70D0(self, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7820);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);
extern "C" int func_00265CB0(void* p);

static inline bool IsMode_001B7820(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7820(void* self)
{
    if (IsMode_001B7820(4)) {
        func_001B70D0(self, 7);
    } else if (IsMode_001B7820(5)) {
        if (*(int*)((char*)self + 0x6D4) == 2 && func_00265CB0(D_004A33D0) != 0) {
            func_001B70D0(self, 0xD);
        } else if (*(int*)((char*)self + 0x6D4) == 1) {
            func_001B70D0(self, 0x10);
        } else if (*(int*)((char*)self + 0x6D4) == 5) {
            func_001B70D0(self, 0x19);
        } else if (*(int*)((char*)self + 0x6D4) == 6) {
            func_001B70D0(self, 0x19);
        } else {
            func_001B70D0(self, 9);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B78F0);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B78F0(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B78F0(void* self)
{
    if (IsMode_001B78F0(8)) {
        func_001B70D0(self, 10);
    } else if (IsMode_001B78F0(9)) {
        func_001B70D0(self, 9);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7938);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);
extern "C" int func_00265CB0(void* p);

static inline bool IsMode_001B7938(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7938(void* self)
{
    if (IsMode_001B7938(4)) {
        func_001B70D0(self, 7);
    } else if (IsMode_001B7938(5)) {
        if (*(int*)((char*)self + 0x6D4) == 2 && func_00265CB0(D_004A33D0) != 0) {
            func_001B70D0(self, 0xD);
        } else if (*(int*)((char*)self + 0x6D4) == 1) {
            func_001B70D0(self, 0x10);
        } else if (*(int*)((char*)self + 0x6D4) == 5) {
            func_001B70D0(self, 0x19);
        } else if (*(int*)((char*)self + 0x6D4) == 6) {
            func_001B70D0(self, 0x19);
        } else {
            func_001B70D0(self, 0x11);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7A08);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B7A08(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7A08(void* self)
{
    if (IsMode_001B7A08(0x15)) {
        func_001B70D0(self, 0x10);
    } else if (IsMode_001B7A08(0x16)) {
        func_001B70D0(self, 0xF);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7A50);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B7A50(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7A50(void* self)
{
    if (IsMode_001B7A50(0xC)) {
        func_001B70D0(self, 0x12);
    } else if (IsMode_001B7A50(0xD)) {
        func_001B70D0(self, 0x11);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7A98);
#ifdef SKIP_ASM
extern "C" void* func_001B70D0(void*, int);
extern "C" int func_00265CB0(void* p);
extern "C" int func_004165A8(const char* a, const char* b);
extern int* D_004A33D0;
extern char D_00536760[];

static inline bool IsMode_001B7A98(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7A98(void* self)
{
    if (IsMode_001B7A98(4)) {
        if (func_004165A8(D_00536760, (char*)self + 0x6E0) != 0 || func_004165A8(D_00536760 + 0x11, (char*)self + 0x6F1) != 0) {
            *(int*)((char*)self + 0x6D4) = 0x18;
            *(int*)((char*)self + 0xB08) = 1;
            *(int*)((char*)self + 0x6D8) = 1;
            func_001B70D0(self, 0x17);
        } else {
            func_001B70D0(self, 0x15);
        }
    } else if (IsMode_001B7A98(5)) {
        int s = *(int*)((char*)self + 0x6D4);
        if (s == 1) {
            *(int*)((char*)self + 0xB08) = 1;
            *(int*)((char*)self + 0x6D8) = 1;
            *(int*)((char*)self + 0x6D4) = 0x18;
            func_001B70D0(self, 0x17);
        } else if (s == 5) {
            func_001B70D0(self, 0x15);
        } else if (s == 6) {
            func_001B70D0(self, 0x15);
        } else if (s == 2) {
            if (func_00265CB0(D_004A33D0) != 0) {
                *(int*)((char*)self + 0xB08) = 1;
                *(int*)((char*)self + 0x6D4) = 0x18;
            }
            func_001B70D0(self, 0x17);
            goto dflt;
        } else {
        dflt:
            func_001B70D0(self, 0x17);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7BC0);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);
void* func_001B70A8(void* self);

static inline bool IsMode_001B7BC0(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7BC0(void* self)
{
    if (IsMode_001B7BC0(0x10)) {
        func_001B70A8(self);
        func_001B70D0(self, 0x18);
    } else if (IsMode_001B7BC0(0x11)) {
        func_001B70D0(self, 0x17);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7C28);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B7C28(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7C28(void* self)
{
    if (IsMode_001B7C28(0x10)) {
        func_001B70D0(self, 0x1B);
    } else if (IsMode_001B7C28(0x11)) {
        func_001B70D0(self, 0x1C);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7C70);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B7C70(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7C70(void* self)
{
    if (IsMode_001B7C70(0xC)) {
        func_001B70D0(self, 0x1D);
    } else if (IsMode_001B7C70(0xD)) {
        func_001B70D0(self, 0x1C);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B88F0);
#ifdef SKIP_ASM
extern void* D_004686C0[];
extern int D_004A3E90;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001B88F0(void* self, int a1, int a2)
{
    func_001A8500_3(self, a1, 0);
    int d = D_004A3E90;
    *(int*)((char*)self + 0x6F0) = 0;
    *(void***)((char*)self + 0x8) = D_004686C0;
    *(int*)((char*)self + 0x6D4) = d;
    *(int*)((char*)self + 0x6D8) = d;
    *(int*)((char*)self + 0x6DC) = d;
    if (a2 != 0) {
        *(int*)((char*)self + 0xC) = 0x4D;
    } else {
        *(int*)((char*)self + 0xC) = 0x50;
    }
    *(int*)((char*)self + 0x6EC) = a2;
    *(int*)((char*)self + 0x6A8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B8960);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern char D_00464740[];
extern void* D_004A3028;

extern "C" void func_001B8960(void* self)
{
    if (*(int*)((char*)self + 0x6EC) != 0) {
        *(char**)((char*)self + 0x6D0) = (char*)D_004A3028 + 0x260;
    } else {
        *(char**)((char*)self + 0x6D0) = (char*)D_004A3028 + 0x2B0;
    }
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00464740), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001A8768(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B89E8__FPv);
#ifdef SKIP_ASM
void* func_001B89E8(void* self)
{
    return func_001A8770(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B8A08);
#ifdef SKIP_ASM
struct sVE1B8A08 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" bool func_001B9558(void* self);
extern "C" void func_001B95C0(void* self);
extern "C" void func_001B9610(void* self);
extern "C" void func_001B9678(void* self);
extern "C" void func_001B96E0(void* self);
extern "C" void func_001B9740(void* self);
extern "C" void func_001B97B0(void* self);
extern "C" void func_001BA178(void* self);

extern "C" int func_001B8A08(void* self, int a1)
{
    if (a1 != 0) {
        func_001B95C0(self);
        func_001B9610(self);
        func_001B9678(self);
        func_001B96E0(self);
        func_001B97B0(self);
        func_001B9740(self);
        func_001BA178(self);
        void* obj = *(void**)((char*)self + 0x6F0);
        if (obj != 0) {
            sVE1B8A08* vt = *(sVE1B8A08**)((char*)obj + 0x8);
            void* o = (char*)obj + vt[8].delta;
            vt[8].fn(o, !func_001B9558(self));
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B8AA0);

INCLUDE_ASM("fe/feasyncfile", func_001B8C38);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9088);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_004648A8[];
extern char D_004648C8[];
extern char D_004648E0[];
extern char D_004648F8[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_004614E8[];
extern char D_00464910[];
extern char D_00464930[];

extern "C" void func_001B9088(void* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_004648A8);
    func_001DA530(menu, 0, GetHashValue32(D_004648C8));
    func_001DA530(menu, 1, GetHashValue32(D_004648E0));
    func_001DA530(menu, 2, GetHashValue32(D_004648F8));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 3, GetHashValue32(D_004614E8), 1);
    func_001DA510(menu, 4, GetHashValue32(D_00464910), 2);
    *(int*)((char*)menu + 0x64) = GetHashValue32(D_00464930);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B91C8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* func_001AD130(void* self, int a1);
extern "C" void* func_001B6ED0(void* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void func_0039F290(void* list, void* item);
extern char D_00462D10[];
extern const char D_00462D68[];

extern "C" void func_001B91C8(void* self, void* a1, int kind)
{
    switch ((unsigned int)kind) {
    case 15: {
        func_001A97B8(self, a1, 15);
        void* p = func_001B6ED0(cMemMan_alloc(0xB10, D_00462D10, 0, 0), *(int*)((char*)self + 0x10), self, 1,
                                (*(int**)((char*)self + 0x6D0))[0], (*(int**)((char*)self + 0x6D0))[1]);
        *(int*)((char*)p + 0x18) = 0x82C;
        func_0039F290(*(char**)(*(char**)(*(char**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, p);
        break;
    }
    case 16:
    case 20:
        func_001A8A40(self, func_001AD130(cMemMan_alloc(0x6D8, D_00462D68, 0, 0), *(int*)((char*)self + 0x10)));
        break;
    default:
        func_001A97B8(self, a1, kind);
        break;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9488);
#ifdef SKIP_ASM
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" int func_001A8E40_3i(void* self, int a1, int a2) __asm__("func_001A8E40");
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void* func_001A9C50(void* self);
void func_001C57C0(void* self, int a1, int a2);
extern char D_00462D80[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

extern "C" int func_001B9488(void* self, int a1, int a2)
{
    if (a1 == 0xCE) {
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 2;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D80);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
        func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
        *(int*)((char*)p + 0x168) = 0;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 0x81E;
        func_001A9BC0(self, p);
        return -1;
    }
    return func_001A8E40_3i(self, a1, a2);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9820);
#ifdef SKIP_ASM
struct cUIText;
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void func_001A98B8(void* self, const char* str);
extern "C" void func_001B9950(void* self, cUIText* text, char* str);

extern "C" void func_001B9820(void* self)
{
    char* p = *(char**)((char*)self + 0x69C);
    switch (*(unsigned int*)(p + 0x18)) {
    case 11:
        if (*(cUIText**)((char*)self + 0x6A4) != 0) {
            func_001A98B8(self, p + 0x74);
            cUIText_setAsciiString(*(cUIText**)((char*)self + 0x6A4), (char*)self + 0x5C);
        }
        cBXString_cBXString4(*(char**)((char*)self + 0x6D0) + 0x4, *(char**)((char*)self + 0x69C) + 0x74);
        break;
    case 12:
        if (*(cUIText**)((char*)self + 0x6A4) != 0) {
            func_001A98B8(self, p + 0x74);
            cUIText_setAsciiString(*(cUIText**)((char*)self + 0x6A4), (char*)self + 0x5C);
        }
        cBXString_cBXString4(*(char**)((char*)self + 0x6D0) + 0x8, *(char**)((char*)self + 0x69C) + 0x74);
        break;
    case 10:
        if (*(cUIText**)((char*)self + 0x6A4) != 0) {
            cUIText_setAsciiString(*(cUIText**)((char*)self + 0x6A4), p + 0x74);
        }
        cBXString_cBXString4(*(char**)((char*)self + 0x6D0), *(char**)((char*)self + 0x69C) + 0x74);
        break;
    case 13: {
        cBXString_cBXString4(*(char**)((char*)self + 0x6D0) + 0xC, p + 0x74);
        char* s = *(char**)(*(char**)((char*)self + 0x6D0) + 0xC);
        if (((int*)s)[-2] != 0) {
            func_001B9950(self, *(cUIText**)((char*)self + 0x6A4), s);
        }
        break;
    }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001B9950);

INCLUDE_ASM("fe/feasyncfile", func_001B9B18);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9CD8);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

extern "C" int func_001B9CD8(void* self, const char* str)
{
    int hasAt = 0;
    int hasDot = 0;
    int len = strlen(str);
    int i;
    for (i = 0; i < len; i++) {
        if (str[i] == '@') {
            hasAt = 1;
        } else if (str[i] == '.') {
            hasDot = 1;
        }
    }
    return hasAt && hasDot;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA178);
#ifdef SKIP_ASM
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void cBXString_Reset(void* self);

extern "C" void func_001BA178(void* self)
{
    if (*(int*)((char*)self + 0x6EC) == 0) {
        cBXString_operatorE((char*)self + 0x6D4, *(char**)((char*)self + 0x6D0) + 0x4);
        cBXString_operatorE((char*)self + 0x6D8, *(char**)((char*)self + 0x6D0) + 0x8);
        cBXString_operatorE((char*)self + 0x6DC, *(char**)((char*)self + 0x6D0) + 0xC);
        char* d = *(char**)((char*)self + 0x6D0);
        *(int*)((char*)self + 0x6E0) = *(int*)(d + 0x38);
        *(int*)((char*)self + 0x6E4) = *(int*)(d + 0x3C);
    } else {
        cBXString_Reset((char*)self + 0x6D4);
        cBXString_Reset((char*)self + 0x6D8);
        cBXString_Reset((char*)self + 0x6DC);
    }
    *(int*)(*(char**)((char*)self + 0x6D0) + 0x4C) = 0;
    *(int*)(*(char**)((char*)self + 0x6D0) + 0x44) = 0;
    *(int*)(*(char**)((char*)self + 0x6D0) + 0x48) = 0;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA280);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void* func_001AD130(void* self, int a1);
extern "C" void func_001BA358(void* self);
extern "C" void func_0025BF18(void* self, void* a1);
extern void* D_004A3028;
extern const char D_00462D68[];
extern char D_004649D8[];

extern "C" void func_001BA280(void* self)
{
    func_001BA358(self);
    char* p = *(char**)((char*)self + 0x6D0);
    if (*(int*)(p + 0x4C) == 0 && *(int*)(p + 0x44) == 0 && *(int*)(p + 0x48) == 0) {
        func_001A8A40(self, func_001AD130(cMemMan_alloc(0x6D8, D_00462D68, 0, 0), *(int*)((char*)self + 0x10)));
    } else {
        void* pp = func_001A9C50(self);
        *(int*)((char*)pp + 0xBC) = 0;
        *(int*)((char*)pp + 0xC4) = GetHashValue32(D_004649D8);
        *(int*)((char*)pp + 0xC0) = 3;
        *(int*)((char*)pp + 0x150) = 0x81D;
        *(int*)((char*)pp + 0x164) = 0;
        func_001A9BC0(self, pp);
        func_0025BF18(D_004A3028, *(void**)((char*)self + 0x6D0));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA358);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const char* a, const char* b);

extern "C" void func_001BA358(void* self)
{
    if (func_004165A8(*(char**)(*(char**)((char*)self + 0x6D0) + 0x4), *(char**)((char*)self + 0x6D4)) != 0) {
        *(int*)(*(char**)((char*)self + 0x6D0) + 0x44) = 1;
    }
    if (func_004165A8(*(char**)(*(char**)((char*)self + 0x6D0) + 0xC), *(char**)((char*)self + 0x6DC)) != 0) {
        *(int*)(*(char**)((char*)self + 0x6D0) + 0x4C) = 1;
    }
    char* d = *(char**)((char*)self + 0x6D0);
    if (*(int*)(d + 0x38) != *(int*)((char*)self + 0x6E0) || *(int*)(d + 0x3C) != *(int*)((char*)self + 0x6E4)) {
        *(int*)(d + 0x48) = 1;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA428);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern "C" void func_001BB2D0(void* self);
extern "C" void func_001BB340(void* self);
extern "C" void func_001BB3B0(void* self);
extern "C" void func_001BB440(void* self);
extern "C" void func_001BB4B0(void* self);
extern void* D_004A3028;
extern char D_004649F8[];

struct sFEObj_001BA428 {
    char pad0[0x10];
    void* engine;       // 0x10
    char pad14[0x40 - 0x14];
    void* screen;       // 0x40
    char pad44[0x6D0 - 0x44];
    char* f6D0;         // 0x6D0
    int f6D4;           // 0x6D4
    int pad6D8;
    int f6DC;           // 0x6DC
};

extern "C" void func_001BA428(sFEObj_001BA428* self)
{
    self->f6D4 = 0;
    self->f6D0 = (char*)D_004A3028 + 0x260;
    void* engine = self->engine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004649F8), 0);
    self->screen = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)(self->f6D0 + 0x40) = 0;
    self->f6DC = 0;
    func_001BB2D0(self);
    func_001BB340(self);
    func_001BB3B0(self);
    func_001BB440(self);
    func_001BB4B0(self);
    func_001A8768(self);
    self->f6D4 = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA4E0__FPv);
#ifdef SKIP_ASM
void* func_001BA4E0(void* self)
{
    return func_001A8770(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA500);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001BB520(void* self, void* box, int flag);
extern "C" void func_001BB5D0(void* self, void* box, int a2);
extern "C" void func_001BB798(void* self, void* box, int value);
extern "C" void func_001BB840(void* self, void* box, int a2, int a3, int a4);
extern char D_00464A08[];
extern char D_004A1BD8[];
extern char D_004A1BE0[];
extern char D_004A1BE8[];
extern char D_00464A18[];

static inline int IsHash1BA500(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001BA500(void* self, void* box)
{
    if (IsHash1BA500(*(int*)((char*)box + 0x38), D_00464A08)) {
        func_001A9A18(self, (cUIListBox_001A9A18*)box, 0);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), D_004A1BD8)) {
        func_001BB5D0(self, box, 1);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), D_004A1BE0)) {
        func_001BB840(self, box, 1, 0x7C1, 1);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), D_004A1BE8)) {
        func_001BB798(self, box, 0x7C1);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), D_00464A18)) {
        func_001BB520(self, box, 1);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BA618);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA8B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_004648A8[];
extern char D_004648C8[];
extern char D_004648E0[];
extern char D_004648F8[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_004614E8[];
extern char D_00464910[];
extern char D_00464930[];

extern "C" void func_001BA8B8(void* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_004648A8);
    func_001DA530(menu, 0, GetHashValue32(D_004648C8));
    func_001DA530(menu, 1, GetHashValue32(D_004648E0));
    func_001DA530(menu, 2, GetHashValue32(D_004648F8));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 3, GetHashValue32(D_004614E8), 1);
    func_001DA510(menu, 4, GetHashValue32(D_00464910), 2);
    *(int*)((char*)menu + 0x64) = GetHashValue32(D_00464930);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA9F8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void func_0025B6D8(void* self, int a1);
extern "C" void func_0025B9C8(void* self, void* a1);
extern char D_00464A38[];
extern void* D_004A3028;

extern "C" void func_001BA9F8(void* self, int a1)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464A38);
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x164) = 0;
    *(int*)((char*)p + 0x150) = 0x820;
    func_001A9BC0(self, p);
    if (a1 != 0) {
        func_0025B6D8(D_004A3028, 0);
    } else {
        func_0025B9C8(D_004A3028, (char*)D_004A3028 + 0x260);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BAA88);

INCLUDE_ASM("fe/feasyncfile", func_001BAB28);

INCLUDE_ASM("fe/feasyncfile", func_001BAC90);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BADE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B6ED0(void* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void func_0039F290(void* list, void* item);
extern char D_00462D10[];
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001BADE8()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001BADE8(void* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A97B8_v(self, a1, a2);
        func_00261408(GetSession_001BADE8());
        func_0025B800(D_004A3028);
        void* p = func_001B6ED0(cMemMan_alloc(0xB10, D_00462D10, 0, 0), *(int*)((char*)self + 0x10), self, 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        func_0039F290(*(char**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, p);
    } else {
        func_001A97B8_v(self, a1, a2);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB3B0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIListBox_setEntryByAsciiString(void* box, int index, const char* str, int a3);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_004A1BE8[];
extern char D_004A12C0[];

extern "C" void func_001BB3B0(void* self)
{
    char buf[16];
    cUIText* box = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1BE8));
    if (box != 0) {
        sprintf(buf, D_004A12C0, *(int*)(*(char**)((char*)self + 0x6D0) + 0x30));
        cUIListBox_setEntryByAsciiString(box, 0, buf, 0);
        func_0039A7A8(box, (unsigned char)(*(int*)(*(char**)((char*)self + 0x6D0) + 0x30) - 0x6C));
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB520);
#ifdef SKIP_ASM
class cFEVBox_001BB520 {
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
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464B10[];
extern char D_00464B20[];

extern "C" void func_001BB520(void* self, void* box, int flag)
{
    unsigned char out = 0;
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B10), 0);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B20), 1);
    func_0039A768(box, flag == 0, &out);
    func_0039A7A8(box, out);
    ((cFEVBox_001BB520*)box)->v09(1);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BB5D0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB798);
#ifdef SKIP_ASM
class cFEVBox_001BB798 {
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
    virtual void v09(int a1);
};

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIListBox_setEntryByAsciiString(void* box, int index, const char* str, int a3);
extern "C" void func_0039A670(void* self);
extern "C" void func_0039A8D8(void* self, int count);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_004A12C0[];

extern "C" void func_001BB798(void* self, void* box, int value)
{
    char buf[16];
    func_0039A670(box);
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    func_0039A8D8(box, 0x67);
    sprintf(buf, D_004A12C0, value);
    cUIListBox_setEntryByAsciiString(box, 0, buf, 0);
    func_0039A7A8(box, (unsigned char)(value - 0x6C));
    ((cFEVBox_001BB798*)box)->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB840);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_001BB9B0(void* self, int month, int year);
extern "C" void func_0039A670(void* self);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern char D_004A12C0[];

class cFEVObj_001BB840 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001BB840(void* self, void* box, int month, int year, int sel)
{
    char buf[16];
    unsigned char idx;
    int n = func_001BB9B0(self, month, year);
    func_0039A670(box);
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    int i;
    for (i = 1; i < n + 1; i++) {
        sprintf(buf, D_004A12C0, i);
        cUIListBox_addEntryByAsciiString(box, buf, i);
    }
    if (n < sel) {
        sel = n;
        *(int*)(*(char**)((char*)self + 0x6D0) + 0x2C) = sel;
    }
    idx = 0;
    func_0039A768(box, sel, &idx);
    func_0039A7A8(box, idx);
    ((cFEVObj_001BB840*)box)->v09(1);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBC40);
#ifdef SKIP_ASM
struct sObj1BBC40 {
    char data[0xD0];
};

int GetHashValue32(char* str);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void* func_001A9C50(void* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void* func_001C55F8(void* self);
extern "C" void func_001C5750(void* self, int flags);
extern char D_00464C38[];
extern char D_00461618[];
// PORT: the unit declares func_001BBC40 as returning void*; the body returns 0/1
extern "C" int func_001BBC40_r(void* self) __asm__("func_001BBC40");

extern "C" int func_001BBC40_r(void* self)
{
    sObj1BBC40 o;
    func_001C55F8(&o);
    if (*(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x40) == 0) {
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464C38);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0(self, p);
        func_001C5750(&o, 2);
        return 0;
    }
    func_001C5750(&o, 2);
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBD60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern "C" void func_001BC400(void* self);
extern char D_00464C50[];
extern void* D_004A3028;

extern "C" void func_001BBD60(void* self)
{
    *(char**)((char*)self + 0x6D8) = (char*)D_004A3028 + 0x260;
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00464C50), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001BC400(self);
    func_001A8768(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBDE0__FPv);
#ifdef SKIP_ASM
void* func_001BBDE0(void* self)
{
    return func_001A8770(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBE00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_00464C68[];
extern char D_00461520[];
extern char D_00464C78[];
extern char D_00464C90[];
extern char D_00464CA0[];
extern char D_00464CB8[];

static inline int IsHash1BBE00(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001BBE00(void* self, cUIText* text)
{
    char buf[0x40];
    if (IsHash1BBE00(*(int*)((char*)text + 0x38), D_00464C68)) {
        *(int*)((char*)text + 0x18) = 4;
    } else if (*(int*)((char*)self + 0x6D0) == 0 && IsHash1BBE00(*(int*)((char*)text + 0x38), D_00461520)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00464C78));
    } else if (*(int*)((char*)self + 0x6D0) == 0 && IsHash1BBE00(*(int*)((char*)text + 0x38), D_00464C90)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00464CA0));
    } else {
        int i;
        for (i = 0; i < 4; i++) {
            sprintf(buf, D_00464CB8, i);
            if (IsHash1BBE00(*(int*)((char*)text + 0x38), buf)) {
                cUIText_setAsciiString(text, buf);
                *(int*)((char*)text + 0x18) = i;
                break;
            }
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BBF18);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC190);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00464CE8[];
extern char D_00464D08[];
extern char D_00464D30[];
extern char D_00464D50[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001BC190(void* self, void* menu)
{
    func_001DA528(menu, 3);
    if (*(int*)((char*)self + 0x6D0) != 0) {
        *(int*)((char*)menu + 0x54) = GetHashValue32(D_00464CE8);
        func_001DA530(menu, 0, GetHashValue32(D_00464D08));
    } else {
        *(int*)((char*)menu + 0x54) = GetHashValue32(D_00464D30);
        func_001DA530(menu, 0, GetHashValue32(D_00464D50));
    }
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 2, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC290);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void func_0025B9C8(void* self, void* a1);
extern char D_00464A38[];
extern void* D_004A3028;

extern "C" void func_001BC290(void* self)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464A38);
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x820;
    *(int*)((char*)p + 0x164) = 0;
    func_001A9BC0(self, p);
    func_0025B9C8(D_004A3028, (char*)D_004A3028 + 0x260);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC300);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025D1B8(void* self, const char* str);
extern void* D_004A3028;
extern char D_004615F8[];
extern char D_00461618[];
extern char D_00462DE0[];

extern "C" void func_001BC300(void* self, const char* name)
{
    if (strlen(name) < 3) {
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004615F8);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x814;
        func_001A9BC0(self, p);
    } else {
        void* p = func_001A9C50(self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462DE0);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x815;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0(self, p);
        func_0025D1B8(D_004A3028, name);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC400);
#ifdef SKIP_ASM
class cFEVText_001BC400 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int a1);
};

struct sNames1BC400 {
    char pad[0x14];
    char* mNames[4];
    int mCount;
};

extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_00464CB8[];
extern char D_004A1408[];

extern "C" void func_001BC400(void* self)
{
    char buf[64];
    int i;
    for (i = 0; i < 4; i++) {
        sprintf(buf, D_00464CB8, i);
        cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        if (text != 0) {
            sNames1BC400* names = *(sNames1BC400**)((char*)self + 0x6D8);
            if (i < names->mCount) {
                cUIText_setAsciiString(text, ((char**)((char*)names + 0x14))[i]);
                ((cFEVText_001BC400*)text)->v08(0);
            } else {
                cUIText_setAsciiString(text, D_004A1408);
                ((cFEVText_001BC400*)text)->v08(1);
            }
        }
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BC4F0);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC6A0);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BC728(void* self, void* a1, int a2);
extern "C" void func_001BC818(void* self, void* a1, int a2);
extern "C" void func_001BC990(void* self, void* a1, int a2);

extern "C" void func_001BC6A0(void* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x81F:
        func_001BC818(self, a1, a2);
        break;
    case 0x80E:
        func_001BC728(self, a1, a2);
        break;
    case 0x82C:
        func_001BC990(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC728);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B6ED0(void* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void func_0039F290(void* list, void* item);
extern char D_00462D10[];
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001BC728()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001BC728(void* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A97B8_v(self, a1, a2);
        func_00261408(GetSession_001BC728());
        func_0025B800(D_004A3028);
        void* p = func_001B6ED0(cMemMan_alloc(0xB10, D_00462D10, 0, 0), *(int*)((char*)self + 0x10), self, 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        func_0039F290(*(char**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, p);
    } else {
        func_001A97B8_v(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BC818);

INCLUDE_ASM("fe/feasyncfile", func_001BC990);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BCB58);
#ifdef SKIP_ASM
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern void* D_0046A7F8[];
extern int D_004A3E90;

struct sFEObj_001BCB58 {
    char pad0[0x8];
    void** vt;          // 0x8
    int fC;             // 0xC
    char pad10[0x6D0 - 0x10];
    int f6D0;           // 0x6D0
    int f6D4;           // 0x6D4
    int f6D8[10];       // 0x6D8
    int f700;           // 0x700
    int f704;           // 0x704
    int f708[4];        // 0x708
    int f718[4];        // 0x718
    int f728;           // 0x728
    int f72C;           // 0x72C
    int f730[4];        // 0x730
    int f740[4];        // 0x740
    int f750[4];        // 0x750
    char pad760[0x76C - 0x760];
    int f76C;           // 0x76C
    char pad770[0x778 - 0x770];
    int f778;           // 0x778
    char pad77C[0xB80 - 0x77C];
    int fB80;           // 0xB80
};

extern "C" sFEObj_001BCB58* func_001BCB58(sFEObj_001BCB58* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->vt = D_0046A7F8;
    self->fB80 = self->f76C = D_004A3E90;
    self->fC = 0x3C;
    self->f778 = 0;
    self->f6D0 = 0;
    self->f6D4 = 0;
    for (int i = 9; i >= 0; i--) {
        self->f6D8[i] = 0;
    }
    self->f704 = 0;
    for (int j = 0; j < 4; j++) {
        self->f708[j] = 0;
        self->f718[j] = 0;
        self->f730[j] = 0;
        self->f740[j] = 0;
        self->f750[j] = 0;
    }
    self->f728 = 0;
    self->f72C = 0;
    return self;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BCCA8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

extern "C" void func_001BCCA8()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00262768(D_004A3328, 1, 2, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BCD18);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
// PORT: func_001BDF60's own definition takes an sFESlots1BDF60* (defined later); bound by asm label.
void func_001BDF60_1BDC18(void* self, int id) __asm__("func_001BDF60");
extern "C" void func_001BE0A8(void* self);
extern "C" void func_001BE3A8(void* self, int id);
extern "C" void func_001BE720(void*);
extern "C" void func_0039A7A8(void* self, int a1);

struct sVE1BCD18 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" int func_001BCD18(void* self, int reset)
{
    if (reset != 0) {
        sVE1BCD18* vt = *(sVE1BCD18**)((char*)self + 0x8);
        vt[32].fn((char*)self + vt[32].delta);
        *(int*)((char*)self + 0x760) = 0;
        func_001BE0A8(self);
        func_001BE720(self);
        func_001BE3A8(self, *(int*)((char*)self + 0x718));
    } else if (*(int*)((char*)self + 0x760) != 0) {
        func_001BE0A8(self);
        int cur = *(int*)((char*)self + 0x764);
        if (cur < 4) {
            char* list = *(char**)((char*)self + (cur << 2) + 0x708);
            int n = *(unsigned char*)(list + 0x318);
            if (*(int*)((char*)self + 0x768) >= n) {
                *(int*)((char*)self + 0x768) = n - 1;
            }
            func_0039A7A8(list, *(unsigned char*)((char*)self + 0x768));
        }
        char* ids = (char*)self + 0x718;
        cUIMenu_setSelectedByIndex(*(void**)((char*)self + 0x6D4), *(int*)((char*)self + 0x764));
        func_001BE3A8(self, *(int*)(ids + (*(int*)((char*)self + 0x764) << 2)));
        func_001BE720(self);
        func_001BDF60_1BDC18(self, *(int*)(ids + (*(int*)((char*)self + 0x764) << 2)));
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD460);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void func_0025D800(void* self, int a1, const char* a2);
extern "C" const char* func_0025F150(void* self, int a1);
extern void* D_004A3028;
extern char D_00464FA8[];

struct sFEObj_001BD460 {
    char pad0[0x770];
    int f770;           // 0x770
    char pad774[0xB80 - 0x774];
    char fB80[4];       // 0xB80 (string object)
};

extern "C" void func_001BD460(sFEObj_001BD460* self, int a1, const char* str)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FA8);
    *(int*)((char*)p + 0x164) = 0;
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x829;
    func_001A9BC0(self, p);
    if (str == 0 || strlen(str) == 0) {
        self->f770 = 0;
        func_0025D800(D_004A3028, a1, 0);
    } else {
        self->f770 = 1;
        func_0025D800(D_004A3028, a1, str);
    }
    char* s = self->fB80;
    cBXString_cBXString4(s, func_0025F150(D_004A3028, a1));
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD540);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void func_0025D860(void* self, const char* name, const char* str);
extern void* D_004A3028;
extern char D_00464FA8[];

struct sFEObj_001BD540 {
    char pad0[0x770];
    int f770;           // 0x770
    char pad774[0xB80 - 0x774];
    char fB80[4];       // 0xB80 (string object)
};

extern "C" void func_001BD540(void* self, int a1, void* a2)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FA8);
    *(int*)((char*)p + 0x164) = 0;
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x829;
    func_001A9BC0(self, p);
    if (a2 == 0 || strlen((const char*)a2) == 0) {
        ((sFEObj_001BD540*)self)->f770 = 0;
        func_0025D860(D_004A3028, (const char*)a1, 0);
    } else {
        ((sFEObj_001BD540*)self)->f770 = 1;
        func_0025D860(D_004A3028, (const char*)a1, (const char*)a2);
    }
    // PORT: the unit declares a1 as int; it is the name string
    cBXString_cBXString4(((sFEObj_001BD540*)self)->fB80, (const char*)a1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD610);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025DAC0(void* self, int a1, const char* a2, const char* a3);
extern void* D_004A3028;
extern char D_00464FC0[];
extern char D_00461618[];
extern char D_00464FD8[];

// PORT: the unit declares this (void*, int, int, int); a1/a3 are string pointers passed as int.
extern "C" void func_001BD610(void* self, int a1, int a2, int a3)
{
    const char* name = (const char*)a1;
    const char* extra = (const char*)a3;
    if (strlen(name) == 0) {
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FC0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x824;
        func_001A9BC0(self, p);
    } else {
        void* p = func_001A9C50(self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FD8);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x828;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0(self, p);
        void* g = D_004A3028;
        func_0025DAC0(g, a2, name, strlen(extra) != 0 ? extra : 0);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDC18);
#ifdef SKIP_ASM
// PORT: func_001BDF60's own definition takes an sFESlots1BDF60* (defined later); bound by asm label.
void func_001BDF60_1BDC18(void* self, int id) __asm__("func_001BDF60");
extern "C" void func_001BE3A8(void* self, int id);
// PORT: func_001BE510 is defined as (self) in this unit, but this caller passes a second arg.
void func_001BE510_2(void* self, int a1) __asm__("func_001BE510");

extern "C" int func_001BDC18(void* self, void* obj, int msg, int a3)
{
    if (obj == *(void**)((char*)self + 0x6D4)) {
        if (msg == 5) {
            func_001BDF60_1BDC18(self, *(int*)((char*)obj + 0xA0));
            func_001BE3A8(self, *(int*)((char*)obj + 0xA0));
            return 0x101;
        }
        return 0x101;
    }
    if (obj == *(void**)((char*)self + 0x6D0)) {
        if (msg == 4) {
            *(int*)((char*)self + 0x778) = a3;
            func_001BE510_2(self, *(int*)((char*)self + 0x774));
            return 0x101;
        }
    }
    return 0x101;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDD60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001A9150(void* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void func_001CE3C8(void* self, int i, int v, int mode);
extern char D_004650D0[];
extern char D_004A1408[];

extern "C" void func_001BDD60(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(self, a1, 0xF);
        // PORT: string pointer passed through an int parameter (func_001A9150's a3)
        func_001A9150(self, 0, GetHashValue32(D_004650D0), (int)D_004A1408, 0, 8, 0x3F);
        func_001CE3C8(*(void**)((char*)self + 0x69C), 0x35, 0, 2);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDE48);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465148[];
extern char D_00465160[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00465178[];
extern char D_00465198[];
extern char D_00461458[];
extern char D_004614E8[];

extern "C" void func_001BDE48(void* self, void* menu)
{
    func_001DA528(menu, 6);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00465148);
    func_001DA530(menu, 0, GetHashValue32(D_00465160));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00465178), 8);
    func_001DA510(menu, 3, GetHashValue32(D_00465198), 0);
    func_001DA510(menu, 4, GetHashValue32(D_00461458), 2);
    func_001DA510(menu, 5, GetHashValue32(D_004614E8), 1);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE720);
#ifdef SKIP_ASM
class cFEVObj_001BE720 {
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
    virtual void v09(int a1);
};

extern "C" void func_001BE720(void* self)
{
    if (*(int*)((char*)self + 0x778) == 0) {
        (*(cFEVObj_001BE720**)((char*)self + 0x728))->v09(0);
    } else {
        (*(cFEVObj_001BE720**)((char*)self + 0x728))->v09(1);
    }
    if (*(int*)((char*)self + 0x778) + 10 >= *(int*)((char*)self + 0x77C)) {
        (*(cFEVObj_001BE720**)((char*)self + 0x72C))->v09(0);
    } else {
        (*(cFEVObj_001BE720**)((char*)self + 0x72C))->v09(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE7D8);
#ifdef SKIP_ASM
extern void* D_0046A6E8[];
extern int D_004A3E90;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001BE7D8(void* self, int a1, int a2)
{
    func_001A8500_3(self, a1, a2);
    int d = D_004A3E90;
    *(void***)((char*)self + 0x8) = D_0046A6E8;
    *(int*)((char*)self + 0x6DC) = d;
    *(int*)((char*)self + 0x6E0) = d;
    *(int*)((char*)self + 0x6E4) = d;
    *(int*)((char*)self + 0xC) = 0x47;
    return self;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEC78);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(void* self, void* popup);
extern "C" void* func_001A9C50(void* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" int strlen(const char* s);
extern char D_00464FC0[];
extern char D_00461618[];

extern "C" int func_001BEC78(void* self)
{
    if (strlen(*(const char**)((char*)self + 0x6DC)) == 0) {
        int three = 3;
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FC0);
        *(int*)((char*)p + 0xC0) = three;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x14C) = three;
        *(int*)((char*)p + 0x150) = 0x847;
        func_001A9BC0(self, p);
        return 0;
    }
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BED58);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465278[];
extern char D_00465290[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001BED58(void* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465278);
    func_001DA530(popup, 0, GetHashValue32(D_00465290));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(popup, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 3, GetHashValue32(D_004614E8), 1);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BF378);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001BF378()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001BF378(void* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_00261408(GetSession_001BF378());
        func_0025B800(D_004A3028);
        func_001A97B8(self, a1, a2);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BF438);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BF5A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_001A8A40_r(void* self, void* obj) __asm__("func_001A8A40");
extern "C" void* func_001B3F78(void* mem, int a1);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern void* D_004A3328;
extern void* D_004A3028;
extern const char D_00460BF0[];

static inline void* GetSession_001BF5A8()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001BF5A8(void* self, void* a1, int kind)
{
    switch ((unsigned int)kind) {
    case 15:
        *(int*)((char*)self + 0x6B0) = 1;
        if (func_001A8A40_r(self, func_001B3F78(cMemMan_alloc(0x91C, D_00460BF0, 0, 0), *(int*)((char*)self + 0x10))) != 0) {
            func_00261408(GetSession_001BF5A8());
            func_0025B800(D_004A3028);
            func_002636F0(func_002591B8(), 0);
        }
        break;
    case 16:
    case 20:
        func_001A97B8(self, a1, kind);
        break;
    default:
        func_001A97B8(self, a1, kind);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001BF6C0);
#ifdef SKIP_ASM
class cFEVObj_001BF6C0 {
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
extern "C" void func_001BF438(void* self, void* a1, int a2);
extern "C" void func_001BF5A8(void* self, void* a1, int a2);

extern "C" void func_001BF6C0(cFEVObj_001BF6C0* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x80E:
        self->v33(a1, a2);
        break;
    case 0x82C:
        func_001BF438(self, a1, a2);
        break;
    case 0x848:
        func_001BF5A8(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BF758);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFAE0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_004653A8[];
extern char D_004653C8[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_004614E8[];
extern char D_00464910[];

extern "C" void func_001BFAE0(void* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_004653A8);
    func_001DA530(popup, 0, GetHashValue32(D_004653C8));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 2, GetHashValue32(D_004614E8), 1);
    func_001DA510(popup, 3, GetHashValue32(D_00464910), 2);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFBF8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A9930(void* self);
extern "C" void func_00256C50();
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* p, int a1);
extern int D_004A2EEC;
extern void* D_004A3028;
extern char D_004653F0[];

extern "C" void func_001BFBF8(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004653F0), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    if (*(int*)D_004A3028 == 0) {
        func_001A9930(self);
    }
    if (D_004A2EEC != 0) {
        func_00256C50();
    }
    func_0028F140(func_0028B180(), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFCA0__FPv);
#ifdef SKIP_ASM
void* func_001BFCA0(void* self)
{
    return func_001A8770(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFCC0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00465408[];
extern char D_00465420[];
extern char D_00465430[];
extern char D_00465440[];
extern char D_00465458[];
extern char D_00465470[];

static inline int IsHash1BFCC0(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001BFCC0(void* self, void* obj)
{
    if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465408)) {
        *(int*)((char*)obj + 0x18) = 1;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465420)) {
        *(int*)((char*)obj + 0x18) = 2;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465430)) {
        *(int*)((char*)obj + 0x18) = 3;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465440)) {
        *(int*)((char*)obj + 0x18) = 0;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465458)) {
        *(int*)((char*)obj + 0x18) = 4;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465470)) {
        *(int*)((char*)obj + 0x18) = 5;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFD98);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465480[];
extern char D_004654A0[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_00461458[];
extern char D_004654C0[];

extern "C" void func_001BFD98(void* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465480);
    func_001DA530(popup, 0, GetHashValue32(D_004654A0));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 2, GetHashValue32(D_00461458), 2);
    func_001DA510(popup, 3, GetHashValue32(D_004654C0), 1);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001BFE70);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0138);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
// PORT: the unit declares func_001A8A40 as returning void; the body returns 0/1
extern "C" int func_001A8A40_r(void* self, void* obj) __asm__("func_001A8A40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00194A60(void* mem, int a1);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern const char D_00460BA8[];
extern void* D_004A3328;
extern void* D_004A3028;

static inline void* GetSession_001C0138()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    return D_004A3328;
}

extern "C" void func_001C0138(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8_v(self, a1, 0xF);
        *(int*)((char*)self + 0x6B0) = 1;
        if (func_001A8A40_r(self, func_00194A60(cMemMan_alloc(0x58, D_00460BA8, 0, 0), *(int*)((char*)self + 0x10))) != 0) {
            func_00261408(GetSession_001C0138());
            func_0025B800(D_004A3028);
            func_002636F0(func_002591B8(), 0);
        }
    } else {
        func_001A97B8_v(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0240);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" void func_00261070(void* self);
void func_00261058(void* self);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001B6ED0(void* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void func_0039F290(void* list, void* item);
extern char D_00462D10[];
extern void* D_004A3328;
extern void* D_004A3028;

extern "C" void func_001C0240(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8_v(self, a1, 0xF);
        func_00261408(D_004A3328);
        func_00261070(D_004A3328);
        func_00261058(D_004A3328);
        func_0025B800(D_004A3028);
        void* p = func_001B6ED0(cMemMan_alloc(0xB10, D_00462D10, 0, 0), *(int*)((char*)self + 0x10), self, 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        func_0039F290(*(char**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, p);
    } else {
        func_001A97B8_v(self, a1, a2);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C04A8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001C0540(void* self);
extern char D_00465568[];
extern char D_00465580[];

extern "C" void func_001C04A8(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00465568), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_001C0540(self);
    }
    cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00465580));
    if (text != 0) {
        *(int*)((char*)text + 0x90) |= 8;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0A80);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* func_001A8770(void* self);
extern "C" void func_003A1F18(cUIText* text, int a1);
extern char D_00465710[];

extern "C" void func_001C0A80(void* self)
{
    if (*(unsigned int*)((char*)self + 0x6D8) < *(unsigned int*)((char*)self + 0x6D4) && *(int**)((char*)self + 0x6D0) != 0) {
        cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00465710));
        if (text != 0) {
            func_003A1F18(text, (*(int**)((char*)self + 0x6D0))[(*(unsigned int*)((char*)self + 0x6D8))++]);
        }
    }
    func_001A8770(self);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0EA8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465778[];
extern char D_00465798[];
extern char D_004657C0[];
extern char D_004614E8[];

extern "C" void func_001C0EA8(void* self, void* popup)
{
    func_001DA528(popup, 2);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465778);
    func_001DA530(popup, 0, GetHashValue32(D_00465798));
    func_001DA510(popup, 0, GetHashValue32(D_004657C0), 8);
    func_001DA510(popup, 1, GetHashValue32(D_004614E8), 1);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C13E0);
#ifdef SKIP_ASM
class cFEVText_001C13E0 {
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
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char D_00463CA0[];
extern char D_004658F0[];

extern "C" void func_001C13E0(void* self, int show)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00463CA0));
        if (text != 0) {
            ((cFEVText_001C13E0*)text)->v09(show);
        }
        text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004658F0));
        if (text != 0) {
            ((cFEVText_001C13E0*)text)->v09(show);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1488);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465900[];
extern char D_00465918[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001C1488(void* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465900);
    func_001DA530(popup, 0, GetHashValue32(D_00465918));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(popup, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 3, GetHashValue32(D_004614E8), 1);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C1B88);
#ifdef SKIP_ASM
class cFEVText_001C1B88 {
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
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C13E0(void* self, int show);
extern "C" void func_001C17F8(void* self, int a1);
extern char D_00461618[];

extern "C" int func_001C1B88(void* self, void* a1, int a2)
{
    int r;
    if (a2 == 0xF || a2 == 0x14) {
        *(int*)((char*)self + 0x6EC) = 0;
        (*(cFEVText_001C1B88**)((char*)self + 0x6E4))->v09(0);
        func_001C13E0(self, *(int*)((char*)self + 0x6EC) ^ 1);
        cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x6F4), GetHashValue32(D_00461618));
        func_001C17F8(self, 0);
        r = func_001A97B8(self, a1, a2);
    } else {
        r = func_001A97B8(self, a1, a2);
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1C90);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A9930(void* self);
extern char D_00465A38[];
extern void* D_004A3028;

extern "C" void func_001C1C90(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00465A38), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    if (*(int*)D_004A3028 == 0) {
        func_001A9930(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1D10__FPv);
#ifdef SKIP_ASM
void* func_001C1D10(void* self)
{
    return func_001A8770(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1D30);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465A48[];
extern char D_00465A60[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_00461458[];
extern char D_004614E8[];

extern "C" void func_001C1D30(void* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465A48);
    func_001DA530(popup, 0, GetHashValue32(D_00465A60));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 2, GetHashValue32(D_00461458), 2);
    func_001DA510(popup, 3, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1E08);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00465A80[];
extern char D_00465A90[];
extern char D_00465AA8[];
extern char D_00465AC0[];

static inline int IsHash1C1E08(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001C1E08(void* self, void* obj)
{
    if (IsHash1C1E08(*(int*)((char*)obj + 0x38), D_00465A80)) {
        *(int*)((char*)obj + 0x18) = 0;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), D_00465A90)) {
        *(int*)((char*)obj + 0x18) = 1;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), D_00465AA8)) {
        *(int*)((char*)obj + 0x18) = 2;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), D_00465AC0)) {
        *(int*)((char*)obj + 0x18) = 3;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C21C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465B38[];
extern char D_00465B50[];
extern char D_0045FE60[];
extern char D_00465B70[];
extern char D_004614E8[];
extern char D_00465B88[];

extern "C" void func_001C21C0(void* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465B38);
    func_001DA530(popup, 0, GetHashValue32(D_00465B50));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00465B70), 0);
    func_001DA510(popup, 2, GetHashValue32(D_004614E8), 1);
    func_001DA510(popup, 3, GetHashValue32(D_00465B88), 7);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2298);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_00465BA0[];
extern char D_004A1398[];
extern char D_00465BB0[];
extern char D_004A1390[];

class cFEVObj_001C2298 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline int IsHash1C2298(int id, char* name) { return id == GetHashValue32(name); }

struct sOwner1C2298 {
    char pad[0x6D4];
    cUIText* slots[10];
};

extern "C" void func_001C2298(sOwner1C2298* self, cUIText* text)
{
    char buf[32];
    if (IsHash1C2298(*(int*)((char*)text + 0x38), D_00465BA0)) {
        cUIText_setAsciiString(text, D_004A1390);
        ((cFEVObj_001C2298*)text)->v09(1);
    } else if (IsHash1C2298(*(int*)((char*)text + 0x38), D_004A1398)) {
        *(int*)((char*)text + 0x14) |= 1;
    } else {
        int i;
        for (i = 0; i < 10; i++) {
            sprintf(buf, D_00465BB0, i);
            if (IsHash1C2298(*(int*)((char*)text + 0x38), buf)) {
                cUIText_setAsciiString(text, D_004A1390);
                ((cFEVObj_001C2298*)text)->v09(1);
                *(int*)((char*)text + 0x18) = i;
                self->slots[i] = text;
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2468);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int i);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cBXString_Concat(void* self, const char* str);
extern "C" char* func_0014F6A8(void* self, int i);
extern "C" void func_003DCB20(char* dst, void* src);
extern "C" void func_001C2578(void* self, void* text, char* str);
extern void* D_004A28A8;
extern char D_004A1C08[];

struct sBXString1C2468 {
    char* str;
    sBXString1C2468() {}
    sBXString1C2468(const sBXString1C2468& o);
};

struct sVE_001C2468 {
    short delta;
    short index;
    void* (*fn)(void* self, int hash);
};

extern "C" void func_001C2468(void* self, int idx)
{
    char buf[16];
    sBXString1C2468 s;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
    sprintf(buf, D_004A1C08, idx + 1);
    cBXString_cBXString2(&s, buf);
    char* msg = func_0014F6A8(iface, idx);
    if (msg == 0) {
        char tmp[0x40];
        char* o = *(char**)((char*)D_004A28A8 + 0x8C);
        sVE_001C2468* vt = *(sVE_001C2468**)(o + 4);
        char* obj = o + vt[4].delta;
        void* str = vt[4].fn(obj, cBEOptionInterface_getDefaultQuickKeyMessageHashValue(iface, idx));
        func_003DCB20(tmp, str);
        cBXString_Concat(&s, tmp);
    } else {
        cBXString_Concat(&s, msg);
    }
    func_001C2578(self, *(void**)((char*)self + (idx << 2) + 0x6D4), s.str);
    cBXString__cBXString(&s, 2);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C2578);

INCLUDE_ASM("fe/feasyncfile", func_001C2740);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2A18);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0014F6E8(void* iface, int i);
extern "C" void func_001C2468(void* self, int i);
extern char D_00465C48[];
extern char D_00461618[];
extern char D_00535617[];

struct sVE1C2A18 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001C2A18(void* self)
{
    char* p = *(char**)((char*)self + 0x69C);
    unsigned int idx = *(unsigned int*)(p + 0x18);
    char* name = p + 0x74;
    if (idx < 10) {
        char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
        if (strlen(name) == 0) {
            void* pop = func_001A9C50(self);
            char* sub = (char*)pop + 0xBC;
            *(int*)((char*)pop + 0xBC) = 1;
            *(int*)((char*)pop + 0xC4) = GetHashValue32(D_00465C48);
            *(int*)((char*)pop + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)pop + 0x150) = 0x83C;
            func_001A9BC0(self, pop);
            func_0014F6E8(iface, idx);
        } else {
            char* src = *(char**)((char*)self + 0x69C) + 0x74;
            strcpy(D_00535617 + (idx << 6), src);
        }
        *(int*)((char*)self + 0x6D0) = 1;
        sVE1C2A18* vt = *(sVE1C2A18**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        func_001C2468(self, idx);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2BA8);
#ifdef SKIP_ASM
class cBEVIface_001C2BA8 {
public:
    int f0;
    int f4;
    int f8;
    virtual void v01();
};

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0014F758(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C2418(void* self);

extern "C" void func_001C2BA8(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        cBEVIface_001C2BA8* iface = (cBEVIface_001C2BA8*)cBE_getInterface_Fv(cBE_getBE(), 4);
        func_0014F758(iface);
        *(int*)((char*)self + 0x6D0) = 1;
        iface->v01();
        func_001C2418(self);
        func_001A97B8(self, a1, 0xF);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C2C50);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0018D7F0(void* mem, int engine, int a2, int a3, int a4);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* func_001C1C50(void* self, int a1);
extern const char D_00460D90[];
extern const char D_004611A8[];

extern "C" void func_001C2C50(void* self, void* a1, int a2)
{
    switch (a2) {
    case 0xF:
        func_001A97B8(self, a1, 0xF);
        func_001A8A40(self, func_0018D7F0(cMemMan_alloc(0x23C, D_00460D90, 0, 0), *(int*)((char*)self + 0x10), 2, 1, 1));
        break;
    case 0x10:
        func_001A97B8(self, a1, 0x10);
        func_001A8A40(self, func_001C1C50(cMemMan_alloc(0x6D0, D_004611A8, 0, 0), *(int*)((char*)self + 0x10)));
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2E70);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

extern "C" void func_001C2E70()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00262768(D_004A3328, 1, 5, 0, 0, 0);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C3570);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465DC0[];
extern char D_00465DD8[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00461458[];
extern char D_00465DF8[];
extern char D_004614E8[];

extern "C" void func_001C3570(void* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00465DC0);
    func_001DA530(menu, 0, GetHashValue32(D_00465DD8));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00461458), 2);
    func_001DA510(menu, 3, GetHashValue32(D_00465DF8), 8);
    func_001DA510(menu, 4, GetHashValue32(D_004614E8), 1);
}
#endif

INCLUDE_ASM("fe/feasyncfile", func_001C3668);

INCLUDE_ASM("fe/feasyncfile", func_001C3D68);

INCLUDE_ASM("fe/feasyncfile", func_001C3EE8);

INCLUDE_ASM("fe/feasyncfile", func_001C4538);

INCLUDE_ASM("fe/feasyncfile", func_001C47A8);

INCLUDE_ASM("fe/feasyncfile", func_001C49C8);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4B78);
#ifdef SKIP_ASM
extern void* D_004A3028;
extern "C" void func_0025E348(void* self, int slot);

extern "C" void func_001C4B78(void* self, int slot)
{
    *(int*)((char*)self + 0x6E8) = 0;
    func_0025E348(D_004A3028, slot);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4BD8);
#ifdef SKIP_ASM
// PORT: func_0039E508__FPv is called with (self, pad) here; bind the 2-arg form to that symbol.
void func_0039E508_2(void* self, void* pad) __asm__("func_0039E508__FPv");
extern "C" void func_001C4D28(void* self);
extern "C" void func_001C4E40(void* self);

struct sVE1C4BD8 {
    short delta;
    short index;
    int (*fn)(void*);
};

static inline int Pad1C4BD8(char* pad, int slot)
{
    sVE1C4BD8* vt = *(sVE1C4BD8**)(pad + 0x8);
    return vt[slot].fn(pad + vt[slot].delta);
}

extern "C" void func_001C4BD8(void* self, char* pad)
{
    func_0039E508_2(self, pad);
    if ((Pad1C4BD8(pad, 17) || Pad1C4BD8(pad, 18) || Pad1C4BD8(pad, 20) || Pad1C4BD8(pad, 19)) &&
        *(unsigned int*)((char*)self + 0x6E4) < 3) {
        if (Pad1C4BD8(pad, 17)) {
            func_001C4E40(self);
        } else if (Pad1C4BD8(pad, 18)) {
            func_001C4D28(self);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4D28);
#ifdef SKIP_ASM
extern char D_004A5A58;

struct sVE1C4D28 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void Show1C4D28(char* o, int on)
{
    sVE1C4D28* vt = *(sVE1C4D28**)(o + 0x8);
    vt[7].fn(o + vt[7].delta, on);
}

extern "C" void func_001C4D28(void* self)
{
    char* snd = *(char**)(*(char**)((char*)self + 0x10) + 0x14);
    if (snd == 0) snd = &D_004A5A58;
    if (snd != 0) {
        sVE1C4D28* vt = *(sVE1C4D28**)snd;
        vt[2].fn(snd + vt[2].delta, 1);
    }
    if (*(unsigned int*)((char*)self + 0x6E4) != 3) {
        Show1C4D28(*(char**)((char*)self + (*(unsigned int*)((char*)self + 0x6E4) << 2) + 0x6D0), 0);
    }
    *(unsigned int*)((char*)self + 0x6E4) += 1;
    if (*(unsigned int*)((char*)self + 0x6E4) >= 4) {
        *(unsigned int*)((char*)self + 0x6E4) = 0;
    }
    if (*(unsigned int*)((char*)self + 0x6E4) == 3) {
        Show1C4D28(*(char**)((char*)self + 0x6DC), 1);
        func_001C4D28(self);
    } else {
        char* item = *(char**)((char*)self + (*(unsigned int*)((char*)self + 0x6E4) << 2) + 0x6D0);
        if ((*(int*)(item + 0x14) >> 6) & 1) {
            Show1C4D28(item, 1);
        } else {
            func_001C4D28(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4E40);
#ifdef SKIP_ASM
extern char D_004A5A58;

struct sVE1C4E40 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void Show1C4E40(char* o, int on)
{
    sVE1C4E40* vt = *(sVE1C4E40**)(o + 0x8);
    vt[7].fn(o + vt[7].delta, on);
}

extern "C" void func_001C4E40(void* self)
{
    char* snd = *(char**)(*(char**)((char*)self + 0x10) + 0x14);
    if (snd == 0) snd = &D_004A5A58;
    if (snd != 0) {
        sVE1C4E40* vt = *(sVE1C4E40**)snd;
        vt[2].fn(snd + vt[2].delta, 1);
    }
    if (*(unsigned int*)((char*)self + 0x6E4) != 3) {
        Show1C4E40(*(char**)((char*)self + (*(unsigned int*)((char*)self + 0x6E4) << 2) + 0x6D0), 0);
    }
    *(unsigned int*)((char*)self + 0x6E4) -= 1;
    if (*(unsigned int*)((char*)self + 0x6E4) >= 4) {
        *(unsigned int*)((char*)self + 0x6E4) = 3;
    }
    if (*(unsigned int*)((char*)self + 0x6E4) == 3) {
        Show1C4E40(*(char**)((char*)self + 0x6DC), 1);
        func_001C4E40(self);
    } else {
        char* item = *(char**)((char*)self + (*(unsigned int*)((char*)self + 0x6E4) << 2) + 0x6D0);
        if ((*(int*)(item + 0x14) >> 6) & 1) {
            Show1C4E40(item, 1);
        } else {
            func_001C4E40(self);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5098);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0025E0C8(void* self, int a1);
extern char D_004A1C30[];
extern void* D_004A3028;

extern "C" void func_001C5098(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A1C30), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_0025E0C8(D_004A3028, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5108);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern void* D_004A3328;
extern void* D_004A3028;

extern "C" void func_001C5108()
{
    if (D_004A3328 == 0) {
        func_00260F80();
    }
    if (func_00261460(D_004A3328) == 0) {
        func_002613C8(D_004A3328);
    }
    if (D_004A3328 == 0) {
        func_0025B800(D_004A3028);
    }
    func_00262768(D_004A3328, 1, 4, 0, 0, 0);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C5368);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8918(void* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9B58(void* self);
extern "C" void* func_001C1C50(void* self, int a1);
extern const char D_004611A8[];

extern "C" void func_001C5368(void* self, int a1, unsigned int msg)
{
    if (a1 != 0) {
        switch (msg) {
        case 8:
            if ((*(int*)((char*)self + 0x1C) & 6) == 0) {
                func_001A9B58(self);
            }
            break;
        case 6:
            func_001A8A40(self, func_001C1C50(cMemMan_alloc(0x6D0, D_004611A8, 0, 0), *(int*)((char*)self + 0x10)));
            break;
        case 5:
            break;
        default:
            func_001A8918(self, a1, msg);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5420);
#ifdef SKIP_ASM
extern "C" int func_001C5420(void* self, void* a1, int a2)
{
    return a2 == 4 ? 0 : 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5430);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
struct cBXString;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
int cBXString_FindLastOf(cBXString* self, char ch, int len);
extern "C" void func_00318E68(cBXString* self, int idx, char ch);
extern "C" void func_003A19F8(cUIText* text, char* str);
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern char D_00465F38[];

struct sBXString1C5430 {
    char* str;
    sBXString1C5430() {}
    sBXString1C5430(const sBXString1C5430& o);
};

// PORT: a2 is a message-data pointer passed as int
extern "C" void* func_001C5430(void* self, int msg, int a2)
{
    if (msg == 0xE2) {
        cUIText* text = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00465F38));
        if (text != 0) {
            sBXString1C5430 s;
            cBXString_cBXString1(&s, (char*)a2 + 8);
            int pos;
            while ((pos = cBXString_FindLastOf((cBXString*)&s, '\n', 0)) != -1) {
                int next = pos + 1;
                if (s.str[next] == '\n') {
                    func_00318E68((cBXString*)&s, pos, '\r');
                    func_00318E68((cBXString*)&s, next, '\r');
                } else {
                    func_00318E68((cBXString*)&s, pos, ' ');
                }
            }
            func_003A19F8(text, s.str);
            cBXString__cBXString(&s, 2);
            return 0;
        }
    } else {
        return func_001A8E40_3(self, msg, a2);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5540);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465F58[];
extern char D_00465F70[];
extern char D_00465F90[];
extern char D_004614E8[];
extern char D_00461458[];

extern "C" void func_001C5540(void* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465F58);
    func_001DA530(popup, 0, GetHashValue32(D_00465F70));
    func_001DA510(popup, 0, GetHashValue32(D_00465F90), 3);
    func_001DA510(popup, 1, GetHashValue32(D_004614E8), 1);
    func_001DA510(popup, 2, GetHashValue32(D_00461458), 2);
}
#endif

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

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5630);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

struct sVec3_1C5630 {
    int x, y, z;
};

extern sVec3_1C5630 D_004FF0D8;

extern "C" void func_001C5630(void* self)
{
    char* b = (char*)self + 0x54;
    char* a = (char*)self + 0x24;
    func_00416210((char*)self + 0x4, 0, 8);
    func_00416210((char*)self + 0xC, 0, 8);
    func_00416210((char*)self + 0x14, 0, 8);
    func_00416210((char*)self + 0x1C, 0, 8);
    int i;
    for (i = 4; i >= 0; i--) {
        func_00416210(a, 0, 8);
        a += 8;
        func_00416210(b, 0, 0xC);
        b += 0xC;
    }
    func_00416210((char*)self + 0x4C, 0, 8);
    *(int*)((char*)self + 0x98) = 0;
    *(int*)((char*)self + 0xA8) = 1;
    *(int*)((char*)self + 0xBC) = 1;
    *(int*)((char*)self + 0x94) = 0;
    *(int*)((char*)self + 0xB8) = 0;
    *(int*)((char*)self + 0xB0) = 0;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x90) = 0;
    *(sVec3_1C5630*)((char*)self + 0x9C) = D_004FF0D8;
    *(int*)((char*)self + 0xAC) = -1;
    *(int*)((char*)self + 0xC0) = 0;
    *(int*)((char*)self + 0xB4) = 0;
}
#endif

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


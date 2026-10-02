#include "common.h"

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466D88[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001D0180(void* self);
extern "C" void func_001CFE60(void* self);
struct sRewardEntry_F0B0 {
    char name[8];
    int id;
};
struct sRewardGallery_F0B0 {
    char pad_0x0[0x7C];
    sRewardEntry_F0B0 entries[180];  // 0x7C
};

extern "C" void cFEStateRewardGalleryBase_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00466D88), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    int i;
    for (i = 0; i < 180; i++) {
        ((sRewardGallery_F0B0*)self)->entries[i].name[0] = 0;
        ((sRewardGallery_F0B0*)self)->entries[i].id = -1;
    }
    *(int*)((char*)self + 0x78) = 0;
    func_001D0180(self);
    if (((*(int*)((char*)self + 0x48) >> 3) & 1) == 0) {
        func_001CFE60(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CF168);
#ifdef SKIP_ASM
struct sVec3_F168 { float x, y, z; };
extern "C" void func_003A0290(void* thing, sVec3_F168* out);
struct sVE_F168a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_F168b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CF168(void* self)
{
    sVec3_F168 a;
    sVec3_F168 b;
    void* t = *(void**)((char*)self + 0x924);
    if (t != 0) {
        func_003A0290(t, &a);
        t = *(void**)((char*)self + 0x8F8);
        if (t != 0) {
            func_003A0290(t, &b);
            *(float*)((char*)self + 0x74) = b.y - a.y;
            *(float*)((char*)self + 0x70) = b.x - a.x;
        }
    }
    *(int*)((char*)self + 0x64) = 0;
    int fl = *(int*)((char*)self + 0x48);
    if (((fl >> 3) & 1) && ((fl >> 1) & 1)) {
        if (*(int*)((char*)self + 0x8FC) == 0) {
            *(int*)((char*)self + 0x8FC) = *(int*)((char*)self + 0x924);
        }
        sVE_F168a* vt = *(sVE_F168a**)((char*)self + 8);
        vt[40].fn((char*)self + vt[40].delta);
        sVE_F168b* vt2 = *(sVE_F168b**)((char*)self + 8);
        vt2[33].fn((char*)self + vt2[33].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CF238);
#ifdef SKIP_ASM
extern "C" void func_001D0238(void* self);
extern "C" void func_001CFF10(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001CF238(void* self)
{
    func_001D0238(self);
    func_001CFF10(self);
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001CF270);

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festaterewards", func_001CFD18);
#ifdef SKIP_ASM
struct sVE_FD18 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CFD18(void* self, void* widget, int msg)
{
    switch (msg) {
    case 0x15:
        if (*(int*)((char*)widget + 0xC) != 7) {
            *(int*)((char*)self + 0x1C) &= ~8;
        }
        break;
    case 0x16:
        if (((*(int*)((char*)self + 0x1C) >> 3) & 1) == 0) {
            *(int*)((char*)self + 0x1C) |= 8;
        }
        if (*(int*)((char*)widget + 0xC) == 7 && *(int*)((char*)widget + 0x6C) != 0) {
            sVE_FD18* vt = *(sVE_FD18**)((char*)self + 8);
            vt[30].fn((char*)self + vt[30].delta,
                      *(int*)((char*)self + 0x64) * *(int*)((char*)self + 0x5C) + *(int*)(*(char**)((char*)self + 0x8FC) + 0x18));
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CFDD0);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);

struct sRewardName_FDD0 {
    char name[8];
    int id;
};

struct sRewardTable_FDD0 {
    char pad_0x0[0x78];
    int count;
    sRewardName_FDD0 entries[1];
};

// PORT: the unit declares func_001CFDD0 as `void (void*, char*)`, but the body returns the id.
int func_001CFDD0_find(sRewardTable_FDD0* self, char* name) __asm__("func_001CFDD0");

int func_001CFDD0_find(sRewardTable_FDD0* self, char* name)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (func_004165A8(self->entries[i].name, name) == 0) {
            return self->entries[i].id;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001CFE60);

INCLUDE_ASM("fe/festaterewards", func_001CFF10);

INCLUDE_ASM("fe/festaterewards", func_001CFFF8);

INCLUDE_ASM("fe/festaterewards", func_001D00C0);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0180);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00156A38(void* iface);
extern "C" void func_00156A10(void* iface);
struct sVE_0180 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001D0180(void* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xD);
    if (func_00156A38(iface) == 0) {
        *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) & ~2) | 1;
        func_00156A10(iface);
    } else {
        *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) & ~1) | 2;
        sVE_0180* vt = *(sVE_0180**)((char*)self + 8);
        vt[28].fn((char*)self + vt[28].delta);
        *(int*)((char*)self + 0x60) = (*(int*)((char*)self + 0x68) + *(int*)((char*)self + 0x5C) - 1) / *(int*)((char*)self + 0x5C);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0238);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00156A38(void* iface);
struct sVE_0238a {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVE_0238b {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_0238c {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D0238(void* self)
{
    if (((*(int*)((char*)self + 0x48) >> 1) & 1) == 0) {
        if (func_00156A38(cBE_getInterface_Fv(cBE_getBE(), 0xD)) != 0) {
            *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) & ~1) | 2;
            sVE_0238a* vt = *(sVE_0238a**)((char*)self + 8);
            vt[28].fn((char*)self + vt[28].delta);
            *(int*)((char*)self + 0x60) = (*(int*)((char*)self + 0x68) + *(int*)((char*)self + 0x5C) - 1) / *(int*)((char*)self + 0x5C);
            if ((*(int*)((char*)self + 0x48) >> 3) & 1) {
                *(int*)((char*)self + 0x8FC) = *(int*)((char*)self + 0x924);
                sVE_0238b* vt2 = *(sVE_0238b**)((char*)self + 8);
                vt2[40].fn((char*)self + vt2[40].delta);
                sVE_0238c* vt3 = *(sVE_0238c**)((char*)self + 8);
                vt3[33].fn((char*)self + vt3[33].delta, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0320);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00157F20(void* iface, int player, int a2, int a3, int a4, int a5);
extern "C" int func_00150928(void* iface, int a1, int charID);
struct sVE_0320a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_0320b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D0320(void* self, int a1)
{
    func_00157F20(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)((char*)self + 0x44),
                  *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x4C), a1, 1);
    *(int*)((char*)self + 0x54) = func_00150928(cBE_getInterface_Fv(cBE_getBE(), 0xB), *(signed char*)((char*)self + 0x44),
                                                *(int*)((char*)self + 0x50));
    sVE_0320a* vt = *(sVE_0320a**)((char*)self + 8);
    vt[42].fn((char*)self + vt[42].delta);
    sVE_0320b* vt2 = *(sVE_0320b**)((char*)self + 8);
    vt2[33].fn((char*)self + vt2[33].delta, a1);
    sVE_0320a* vt3 = *(sVE_0320a**)((char*)self + 8);
    vt3[43].fn((char*)self + vt3[43].delta);
    sVE_0320a* vt4 = *(sVE_0320a**)((char*)self + 8);
    vt4[45].fn((char*)self + vt4[45].delta);
}
#endif

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_showReward);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0510);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001CABA8(void* self, void* engine, void* owner, signed char idx);
extern "C" void func_001CAA18(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_0039F290(void* stack, void* state);
extern char D_00460790[];
struct sVE_0510a {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVE_0510b {
    short delta;
    short index;
    int (*fn)(void*, int);
};

extern "C" void func_001D0510(void* self)
{
    sVE_0510a* vt = *(sVE_0510a**)((char*)self + 8);
    int sel = vt[34].fn((char*)self + vt[34].delta);
    char* popup = (char*)func_001CABA8(cMemMan_alloc(0x70, D_00460790, 0, 0), *(void**)((char*)self + 0x10), self,
                                       *(signed char*)((char*)self + 0x44));
    sVE_0510b* vt2 = *(sVE_0510b**)((char*)self + 8);
    int a = vt2[37].fn((char*)self + vt2[37].delta, sel);
    sVE_0510b* vt3 = *(sVE_0510b**)((char*)self + 8);
    int b = vt3[35].fn((char*)self + vt3[35].delta, sel);
    func_001CAA18(popup + 0x48, *(int*)((char*)self + 0x4C), a, b, *(int*)((char*)self + 0x54));
    func_0039F290((char*)*(void**)((char*)self + 0x10) + 0x18, popup);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D05F0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00157468(void* iface, int a, int b, int c, int d);

extern "C" void func_001D05F0(void* self, int arg)
{
    func_00157468(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)((char*)self + 0x44),
                  *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x4C), arg);
}
#endif

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateHelpText);

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateRow);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0B00);
#ifdef SKIP_ASM
struct sVE_0B00a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_0B00b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D0B00(void* self)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x58); i++) {
        sVE_0B00b* vt = *(sVE_0B00b**)((char*)self + 8);
        vt[39].fn((char*)self + vt[39].delta, i);
    }
    sVE_0B00a* vt1 = *(sVE_0B00a**)((char*)self + 8);
    vt1[46].fn((char*)self + vt1[46].delta);
    sVE_0B00a* vt2 = *(sVE_0B00a**)((char*)self + 8);
    vt2[45].fn((char*)self + vt2[45].delta);
    sVE_0B00a* vt3 = *(sVE_0B00a**)((char*)self + 8);
    vt3[44].fn((char*)self + vt3[44].delta);
    sVE_0B00a* vt4 = *(sVE_0B00a**)((char*)self + 8);
    vt4[41].fn((char*)self + vt4[41].delta);
    sVE_0B00a* vt5 = *(sVE_0B00a**)((char*)self + 8);
    vt5[42].fn((char*)self + vt5[42].delta);
}
#endif

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updatePageNumber);

INCLUDE_ASM("fe/festaterewards", func_001D0D30);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0E78);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* func_00198AF0(void* a0);

extern "C" void func_001D0E78(void* self)
{
    if (*(cUIText**)((char*)self + 0x910) != 0) {
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x910), func_00198AF0(*(void**)((char*)self + 0x54)));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0EB8);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004A12C0[];

extern "C" void func_001D0EB8(void* self)
{
    char buf[32];
    if (*(cUIText**)((char*)self + 0x918) != 0) {
        sprintf(buf, D_004A12C0, *(int*)((char*)self + 0x6C));
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x918), buf);
    }
    if (*(cUIText**)((char*)self + 0x914) != 0) {
        sprintf(buf, D_004A12C0, *(int*)((char*)self + 0x68));
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x914), buf);
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D0F30);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D10A8);
#ifdef SKIP_ASM
struct sVEntry001D10A8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D10A8(void* self)
{
    void* obj = *(void**)((char*)self + 0x91C);
    if (obj != 0) {
        sVEntry001D10A8* vt = *(sVEntry001D10A8**)((char*)obj + 8);
        vt[9].fn((char*)obj + vt[9].delta, *(int*)((char*)self + 0x64) > 0);
    }
    void* obj2 = *(void**)((char*)self + 0x920);
    if (obj2 != 0) {
        sVEntry001D10A8* vt = *(sVEntry001D10A8**)((char*)obj2 + 8);
        vt[9].fn((char*)obj2 + vt[9].delta,
                 *(int*)((char*)self + 0x64) + *(int*)((char*)self + 0x58) < *(int*)((char*)self + 0x60));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1128);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_0046A248[];

extern "C" void* func_001D1128(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 3);
    *(void***)((char*)self + 0x8) = D_0046A248;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 42; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_00467070[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1198__FPv);
#ifdef SKIP_ASM
void* func_001D1198(void* self)
{
    return (void*)D_00467070;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D11A8);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D11A8(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D11F8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_001572B0(void* self);
extern "C" void* func_0015A478();

struct sRewardList_11F8 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D11F8(sRewardList_11F8* self)
{
    int n = func_001572B0(cBE_getInterface_Fv(cBE_getBE(), 0xD));
    self->count = 0;
    self->base = *(int*)((char*)func_0015A478() + 0x14);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1290);
#ifdef SKIP_ASM
extern "C" void func_001D1290(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 42; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D12C8);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_0046A0C8[];

extern "C" void* func_001D12C8(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 4);
    *(void***)((char*)self + 0x8) = D_0046A0C8;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 115; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_00467098[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1330__FPv);
#ifdef SKIP_ASM
void* func_001D1330(void* self)
{
    return (void*)D_00467098;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1340);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1340(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1390);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_001572D0(void* self);
extern "C" void* func_0015A478();

struct sRewardList_1390 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D1390(sRewardList_1390* self)
{
    int n = func_001572D0(cBE_getInterface_Fv(cBE_getBE(), 0xD));
    self->count = 0;
    self->base = *(int*)((char*)func_0015A478() + 0x18);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1428);
#ifdef SKIP_ASM
extern "C" void func_001D1428(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 115; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1460);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_00469F48[];

extern "C" void* func_001D1460(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 5);
    *(void***)((char*)self + 0x8) = D_00469F48;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 99; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_004670C8[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D14C8__FPv);
#ifdef SKIP_ASM
void* func_001D14C8(void* self)
{
    return (void*)D_004670C8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D14D8);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D14D8(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1528);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_001572F0(void* self);
extern "C" void* func_0015A478();

struct sRewardList_1528 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D1528(sRewardList_1528* self)
{
    int n = func_001572F0(cBE_getInterface_Fv(cBE_getBE(), 0xD));
    self->count = 0;
    self->base = *(int*)((char*)func_0015A478() + 0x1C);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D15C0);
#ifdef SKIP_ASM
extern "C" void func_001D15C0(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 99; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D15F8);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_00469DC8[];

extern "C" void* func_001D15F8(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 6);
    *(void***)((char*)self + 0x8) = D_00469DC8;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 1; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_004670F0[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1660__FPv);
#ifdef SKIP_ASM
void* func_001D1660(void* self)
{
    return (void*)D_004670F0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1670);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1670(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D16C0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_00157330(void* self);
extern "C" void* func_0015A478();

struct sRewardList_16C0 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D16C0(sRewardList_16C0* self)
{
    int n = func_00157330(cBE_getInterface_Fv(cBE_getBE(), 0xD));
    self->count = 0;
    self->base = *(int*)((char*)func_0015A478() + 0x24);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1758);
#ifdef SKIP_ASM
extern "C" void func_001D1758(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 1; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1790);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_00469C48[];

extern "C" void* func_001D1790(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 7);
    *(void***)((char*)self + 0x8) = D_00469C48;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 27; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_00467118[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D17F8__FPv);
#ifdef SKIP_ASM
void* func_001D17F8(void* self)
{
    return (void*)D_00467118;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1808);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1808(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1858);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_00157310(void* self);
extern "C" void* func_0015A478();

struct sRewardList_1858 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D1858(sRewardList_1858* self)
{
    int n = func_00157310(cBE_getInterface_Fv(cBE_getBE(), 0xD));
    self->count = 0;
    self->base = *(int*)((char*)func_0015A478() + 0x20);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D18F0);
#ifdef SKIP_ASM
extern "C" void func_001D18F0(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 27; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1928);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_00469AC8[];

extern "C" void* func_001D1928(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 8);
    *(void***)((char*)self + 0x8) = D_00469AC8;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 27; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1990);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_00157370(void* self);
extern "C" void* func_0015A478();

struct sRewardList_1990 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D1990(sRewardList_1990* self)
{
    int n = func_00157370(cBE_getInterface_Fv(cBE_getBE(), 0xD));
    self->count = 0;
    self->base = *(int*)((char*)func_0015A478() + 0x28);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1A28);
#ifdef SKIP_ASM
extern "C" void func_001D1A28(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 27; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1A60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_00398380(void* self, int id);

extern "C" void func_001D1A60(void* self, int i)
{
    char* obj = *(char**)((char*)self + 0x10);
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    int h = GetHashValue32(*(char**)((char*)e + 4));
    func_00398380(obj + 0x58, h);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1AA0);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" unsigned char func_001A1CD0(void* self, signed char a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int player);
extern void* D_004699F8[];
struct sRewardState_1AA0 {
    char pad_0x0[0x48];
    int a[3];   // 0x48
    int b[3];   // 0x54
};

extern "C" void* func_001D1AA0(void* self, void* engine, void* owner, int player)
{
    signed char idx = player;
    int i;
    func_0039E318(self, engine, owner);
    *(int*)((char*)self + 0xC) = 0x30;
    *(void***)((char*)self + 0x8) = D_004699F8;
    *(signed char*)((char*)self + 0x44) = idx;
    *(unsigned char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    for (i = 0; i < 3; i++) {
        ((sRewardState_1AA0*)self)->a[i] = 0;
        ((sRewardState_1AA0*)self)->b[i] = 0;
    }
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x64) = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1),
                                                                         *(signed char*)((char*)self + 0x44));
    *(int*)((char*)self + 0x68) = -1;
    *(int*)((char*)self + 0x6C) = -1;
    *(int*)((char*)self + 0x70) = -1;
    *(int*)((char*)self + 0x74) = -1;
    return self;
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1B68);

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00467140[];
extern char D_004A1398[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" void cFEStatePreviewReward_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00467140), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1398));
    if (obj != 0) {
        *(int*)((char*)obj + 0x90) |= 8;
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1F78);
#ifdef SKIP_ASM
extern "C" int func_001D1F78(void* self, int a1, unsigned int a2)
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
INCLUDE_ASM("fe/festaterewards", func_001D1FB0);
#ifdef SKIP_ASM
extern "C" void func_001D2198(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001D1FB0(void* self)
{
    if (*(int*)((char*)self + 0x6C) >= 0) {
        func_001D2198(self);
    }
    func_0039E510(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1FF0);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0039F190(void* self, int a1);
extern char D_0045DC60[];
struct sVE_1FF0a {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVE_1FF0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sDefObj_1FF0 {
    sVE_1FF0b* vt;
};
// $gp-relative default object (target has raw `addiu $3,$28,0x2968`, no symbol)
extern sDefObj_1FF0 D_004A5A58;

extern "C" int func_001D1FF0(void* self, void* obj)
{
    sVE_1FF0a* vt = *(sVE_1FF0a**)((char*)obj + 8);
    if (vt[6].fn((char*)obj + vt[6].delta) != 0) {
    sDefObj_1FF0* p = *(sDefObj_1FF0**)(*(char**)((char*)self + 0x10) + 0x14);
    if (p == 0) {
        p = &D_004A5A58;
    }
    if (p != 0) {
        p->vt[2].fn((char*)p + p->vt[2].delta, 3);
    }
    int i;
    for (i = 0; i < 3; i++) {
        char* o = ((char**)((char*)self + 0x48))[i];
        if (o != 0) {
            sVE_1FF0b* vt2 = *(sVE_1FF0b**)(o + 8);
            vt2[9].fn(o + vt2[9].delta, 0);
        }
    }
    cUIScreen_playFrame(*(void**)((char*)self + 0x40), cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_0045DC60)), 1);
    func_0039F190((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
    return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D20D8);
#ifdef SKIP_ASM
extern "C" void func_001D2150(void* self, int a1);

extern "C" void func_001D20D8(void* self, int kind, void* data)
{
    *(int*)((char*)self + 0x68) = kind;
    *(void**)((char*)self + 0x60) = data;
    switch (kind) {
    case 3:
        func_001D2150(self, *(int*)((char*)data + 4));
        break;
    case 4:
        func_001D2150(self, *(int*)((char*)data + 4));
        break;
    case 5:
        func_001D2150(self, *(int*)((char*)data + 4));
        break;
    case 7:
        func_001D2150(self, *(int*)((char*)data + 4));
        break;
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D2150);

INCLUDE_ASM("fe/festaterewards", func_001D2198);

INCLUDE_ASM("fe/festaterewards", func_001D22F0);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2380);
#ifdef SKIP_ASM
extern "C" void func_00253418(void* p, int a1);
extern "C" void func_0039E390(void* self, int flags);
extern void* D_00469928[];
extern void* D_0046D0D0[];

extern "C" void func_001D2380(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_00469928;
    if (*(void**)((char*)self + 0x248) != 0) {
        func_00253418(*(void**)((char*)self + 0x248), 3);
    }
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D23E0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002533C8(void* mem);
extern "C" void func_001D25E8(void* self);
extern char D_00461320[];

extern "C" void func_001D23E0(void* self)
{
    *(void**)((char*)self + 0x248) = func_002533C8(cMemMan_alloc(0x44, D_00461320, 0, 0));
    func_001D25E8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2430);
#ifdef SKIP_ASM
extern "C" void func_00253418(void* p, int a1);

extern "C" void func_001D2430(void* self)
{
    void* p = *(void**)((char*)self + 0x248);
    if (p != 0) {
        func_00253418(p, 3);
    }
    *(void**)((char*)self + 0x248) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2468);
#ifdef SKIP_ASM
extern "C" void func_00253890(void* p, int a1);
extern "C" void* func_0039E6B8(void* self);

extern "C" void func_001D2468(void* self)
{
    void* p = *(void**)((char*)self + 0x248);
    if (p != 0) {
        *(int*)((char*)self + 0x278) = 1;
        func_00253890(p, 0);
    }
    func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D24A8);
#ifdef SKIP_ASM
extern "C" void func_00253938(void* p);
extern "C" void func_001D2638(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001D24A8(void* self)
{
    void* p = *(void**)((char*)self + 0x248);
    if (p != 0) {
        *(int*)((char*)self + 0x274) = 1;
        func_00253938(p);
        if ((*(int*)((char*)self + 0x27C) != 0 && *(int*)((char*)self + 0x278) != 0)
            || **(int**)((char*)self + 0x248) != 0) {
            func_001D2638(self);
        }
    }
    func_0039E510(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2518);
#ifdef SKIP_ASM
struct sVEntry001D2518 {
    short delta;
    short index;
    int (*fn)(void*);
};

static inline int vcall001D2518(void* obj, int slot)
{
    sVEntry001D2518* vt = *(sVEntry001D2518**)((char*)obj + 8);
    return vt[slot].fn((char*)obj + vt[slot].delta);
}

extern "C" int func_001D2518(void* self, void* obj)
{
    if (*(int*)((char*)self + 0x270) != 0) {
        if (vcall001D2518(obj, 5) != 0 || vcall001D2518(obj, 6) != 0) {
            *(int*)((char*)self + 0x27C) = 1;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2598);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_001D2598(void* self, const char* a, const char* b)
{
    strcpy((char*)self + 0x48, a);
    if (b != 0) {
        strcpy((char*)self + 0x148, b);
    } else {
        *((char*)self + 0x148) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D25E8);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002B3A70(void* p);
extern "C" int func_002534A8(void* p, void* q);
extern "C" void func_00253860(void* p);

extern "C" void func_001D25E8(void* self)
{
    func_002B3A70((char*)func_0028B180() + 0x118);
    *(int*)((char*)self + 0x254) = 1;
    func_002534A8(*(void**)((char*)self + 0x248), (char*)self + 0x24C);
    func_00253860(*(void**)((char*)self + 0x248));
    *(int*)((char*)self + 0x278) = 0;
    *(int*)((char*)self + 0x274) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2638);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002B3A98(void* self);
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001D2638(void* self)
{
    func_002B3A98((char*)func_0028B180() + 0x118);
    *(int*)((char*)self + 0x27C) = 0;
    func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
}
#endif


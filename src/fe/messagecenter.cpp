#include "common.h"

INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_onWidgetCreate);

INCLUDE_ASM("fe/messagecenter", func_00197500);

INCLUDE_ASM("fe/messagecenter", func_001977D0);

//100%
INCLUDE_ASM("fe/messagecenter", func_001979D8);
#ifdef SKIP_ASM
extern "C" void func_001988D8(void* self, int idx);
extern "C" void func_00197B88(void* self);
extern "C" void cFEStateRequestLine_updateHilightedSongInfo(void* self, int a1);
extern "C" void func_001985B0(void* self);
extern "C" void func_001985F0(void* self);
extern "C" void cFEStateRequestLine_updateButtonsText(void* self, int a1);
extern "C" void cFEStateRequestLine_updateHelpText(void* self, int a1);

struct sMsgCenter_979D8 {
    char pad_0x0[0x1C];
    int flags;        // 0x1C
    char pad_0x20[0x2C];
    int list[64];     // 0x4C
    int count;        // 0x14C
    int top;          // 0x150
    char pad_0x154[0x1C];
    char* menu;       // 0x170
};

extern "C" void func_001979D8(sMsgCenter_979D8* self, void* item, int msg)
{
    switch (msg) {
    case 0x15:
        if (*(int*)((char*)item + 0xC) != 7) {
            self->flags &= ~8;
        }
        break;
    case 0x16:
        if (((self->flags >> 3) & 1) == 0) {
            self->flags |= 8;
        }
        if (*(int*)((char*)item + 0xC) == 7 && *(int*)((char*)item + 0x6C) != 0) {
            int song = self->list[self->top + *(int*)(*(char**)(self->menu + 0xA0) + 0x18)];
            func_001988D8(self, song);
            func_00197B88(self);
            cFEStateRequestLine_updateHilightedSongInfo(self, song);
            func_001985B0(self);
            func_001985F0(self);
            cFEStateRequestLine_updateButtonsText(self, song);
            cFEStateRequestLine_updateHelpText(self, song);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00197AD8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_97AD8 {
    char pad_0x0[0x4C];
    int list[64];   // 0x4C
    int count;      // 0x14C
    char pad_0x150[0x10];
    ulong mask;     // 0x160
};

extern "C" void func_00197AD8(void* self)
{
    sMsgCenter_97AD8* s = (sMsgCenter_97AD8*)self;
    int i;
    int j;
    int n = 0;
    for (i = 0; i < s->count; i++) {
        if ((int)((s->mask >> i) & 1)) {
            s->list[n++] = i;
        }
    }
    for (j = 0; j < s->count; j++) {
        if (!(int)((s->mask >> j) & 1)) {
            s->list[n++] = j;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00197B88);
#ifdef SKIP_ASM
extern "C" void func_00197BC8(void* self);
extern "C" void func_00197CA0(void* self);
extern "C" void func_00197DB8(void* self);
extern "C" void func_00198118(void* self);

extern "C" void func_00197B88(void* self)
{
    func_00197BC8(self);
    func_00197CA0(self);
    func_00197DB8(self);
    func_00198118(self);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00197BC8);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* func_00198A88(void* self, int i);

class cMsgWidget_97BC8 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

struct sMsgCenter_97BC8 {
    char pad_0x0[0x4C];
    int list[64];   // 0x4C
    int count;      // 0x14C
    int top;        // 0x150
    char pad_0x154[0x20];
    cMsgWidget_97BC8* texts[8]; // 0x174
};

extern "C" void func_00197BC8(void* self)
{
    sMsgCenter_97BC8* s = (sMsgCenter_97BC8*)self;
    int i;
    for (i = 0; i < 8; i++) {
        int idx = s->top + i;
        if (idx < s->count) {
            int id = s->list[idx];
            s->texts[i]->show(1);
            cUIText_setAsciiString((cUIText*)s->texts[i], (const char*)func_00198A88(self, id));
        } else {
            s->texts[i]->show(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00197CA0);
#ifdef SKIP_ASM
class cMsgWidget_97CA0 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};
// PORT: 64-bit `long` masks (8 bytes on EE, 4 on Windows); use uint64_t off-PS2.
struct sMsgCenter_97CA0 {
    char pad_0x0[0x4C];
    int list[64];   // 0x4C
    int count;      // 0x14C
    int top;        // 0x150
    char pad_0x154[0x4];
    ulong newMask;  // 0x158
    ulong readMask; // 0x160
    char pad_0x168[0x2C];
    cMsgWidget_97CA0* icons[8]; // 0x194
    char pad_0x1B4[0x50];
    int colNew;     // 0x204
    int colRead;    // 0x208
    int colOther;   // 0x20C
};

static inline void setColors_97CA0(cMsgWidget_97CA0* w, int a, int b)
{
    *(int*)((char*)w + 0x78) = a;
    *(int*)((char*)w + 0x7C) = b;
}

static inline int isNew_97CA0(sMsgCenter_97CA0* s, int id)
{
    return (int)((s->newMask >> id) & 1);
}

static inline int isRead_97CA0(sMsgCenter_97CA0* s, int id)
{
    return (int)((s->readMask >> id) & 1);
}

static inline void setVisible_97CA0(cMsgWidget_97CA0* w, int v)
{
    w->show(v);
}

extern "C" void func_00197CA0(void* self)
{
    sMsgCenter_97CA0* s = (sMsgCenter_97CA0*)self;
    int i;
    for (i = 0; i < 8; i++) {
        setVisible_97CA0(s->icons[i], 0);
        int idx = s->top + i;
        if (idx < s->count) {
            int id = s->list[idx];
            setVisible_97CA0(s->icons[i], 1);
            if (isNew_97CA0(s, id)) {
                setColors_97CA0(s->icons[i], -1, s->colNew);
            } else if (isRead_97CA0(s, id)) {
                setColors_97CA0(s->icons[i], -1, s->colRead);
            } else {
                setColors_97CA0(s->icons[i], -1, s->colOther);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00197DB8);
#ifdef SKIP_ASM
class cMsgWidget_97DB8 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

extern "C" void func_00197DB8(void* self)
{
    if (*(int*)((char*)self + 0x150) == 0) {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1EC))->show(0);
    } else {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1EC))->show(1);
    }
    if (*(int*)((char*)self + 0x150) + 8 >= *(int*)((char*)self + 0x14C)) {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1F0))->show(0);
    } else {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1F0))->show(1);
    }
}
#endif

INCLUDE_ASM("fe/messagecenter", func_00197E70);

//100%
INCLUDE_ASM("fe/messagecenter", func_00198118);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int USTR_length(unsigned short* s);
void cMemMan_free(void* p);
extern "C" void func_002C26D0(unsigned short* dst, unsigned short* fmt, int n);
extern "C" void func_003A0E90(void* text, void* p);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004605D8[];
extern char D_00460688[];
extern char* D_004A28A8;

struct sVtEntry_00198118 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_00198118 {
    char pad_0x0[0x158];
    ulong mask;     // 0x158
    char pad_0x160[0x84];
    void* text;     // 0x1E4
};

extern "C" void func_00198118(void* p)
{
    sMsgCenter_00198118* self = (sMsgCenter_00198118*)p;
    char* db = *(char**)(D_004A28A8 + 0x8C);
    sVtEntry_00198118* vt = *(sVtEntry_00198118**)(db + 4);
    unsigned short* fmt = vt[4].fn(db + vt[4].delta, GetHashValue32(D_004605D8));
    unsigned short* buf = (unsigned short*)operator_new_tag((USTR_length(fmt) + 5) * 2, D_00460688, 0x100, 0);
    int n = 0;
    int j;
    for (j = 0; j < 64; j++) {
        n += (int)((self->mask >> j) & 1);
    }
    func_002C26D0(buf, fmt, n);
    func_003A0E90(self->text, buf);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateHelpText);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
int func_00198AE8(void* self);
extern char D_004606B0[];
extern char D_004606C8[];
extern char D_004606E0[];
extern char D_004606F8[];
extern char D_00460710[];
extern char D_00460720[];
extern signed char D_00535C11[];

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sReqLine_001981F8 {
    char pad_0x0[0x158];
    ulong done;     // 0x158
    ulong sel;      // 0x160
    int count;      // 0x168
    int f16C;       // 0x16C
    char pad_0x170[0x78];
    cUIText* text;  // 0x1E8
};

static inline int countSel_001981F8(sReqLine_001981F8* self)
{
    int n = 0;
    for (int i = 0; i < 64; i++) {
        n += (self->sel >> i) & 1;
    }
    return n;
}

extern "C" void cFEStateRequestLine_updateHelpText(void* p, int idx)
{
    sReqLine_001981F8* self = (sReqLine_001981F8*)p;
    if (self->text == 0) {
        return;
    }
    int on = (self->sel >> idx) & 1;
    if (on) {
        ulong bit = (ulong)1 << idx;
        int only = (self->done & ~bit) == 0;
        if (only) {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_004606B0));
        } else {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_004606C8));
        }
    } else if (self->f16C != 0) {
        if (self->count >= func_00198AE8(self) || countSel_001981F8(self) < 6) {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_004606E0));
        } else {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_004606F8));
        }
    } else {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 0) {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_00460710));
        } else {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_00460720));
        }
    }
}
#endif

INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateButtonsText);

//100%
INCLUDE_ASM("fe/messagecenter", func_001985B0);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* func_00198AF0(void* a0);

extern "C" void func_001985B0(void* self)
{
    if (*(cUIText**)((char*)self + 0x1E0) != 0) {
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x1E0), func_00198AF0(*(void**)((char*)self + 0x168)));
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_001985F0);
#ifdef SKIP_ASM
class cMsgWidget_985F0 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_985F0 {
    char pad_0x0[0x160];
    ulong mask;     // 0x160
    char pad_0x168[0x4];
    int enabled;    // 0x16C
    char pad_0x170[0x4C];
    cMsgWidget_985F0* icons[6]; // 0x1BC
};

extern "C" void func_001985F0(void* self)
{
    sMsgCenter_985F0* s = (sMsgCenter_985F0*)self;
    int n = 0;
    int i;
    int j;
    for (j = 0; j < 64; j++) {
        n += (int)((s->mask >> j) & 1);
    }
    for (i = 0; i < 6; i++) {
        if (s->icons[i] != 0) {
            if (s->enabled != 0 && n < 6 - i) {
                s->icons[i]->show(1);
            } else {
                s->icons[i]->show(0);
            }
        }
    }
}
#endif

INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateHilightedSongInfo);

//100%
INCLUDE_ASM("fe/messagecenter", func_001988D8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00158558(void* iface, int rider, int a, int idx, int full);

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_988D8 {
    char pad_0x0[0x44];
    signed char rider; // 0x44
    char pad_0x45[0x3];
    int a48;        // 0x48
    char pad_0x4C[0x10C];
    ulong seen;     // 0x158
    ulong mask;     // 0x160
    int result;     // 0x168
};

extern "C" void func_001988D8(void* self, int idx)
{
    sMsgCenter_988D8* s = (sMsgCenter_988D8*)self;
    int n = 0;
    int j;
    for (j = 0; j < 64; j++) {
        n += (int)((s->mask >> j) & 1);
    }
    int full = n >= 6;
    s->result = func_00158558(cBE_getInterface_Fv(cBE_getBE(), 0xD), s->rider, s->a48, idx, full);
    ulong bit = (ulong)1 << idx;
    s->mask |= bit;
    s->seen |= bit;
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198988);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001CABA8(void* self, void* engine, void* owner, signed char idx);
extern "C" void* func_00198A88(void* self, int i);
// PORT: func_00198AE8__FPv is called with (self, idx) here; bind the 2-arg form to that symbol.
int func_00198AE8_2(void* self, int idx) __asm__("func_00198AE8__FPv");
extern "C" void cBuyPopupInfo_initBuySong(int* self, int a1, int a2, int a3);
extern "C" void cBuyPopupInfo_initBuySongByCredit(void* self, int song, int credit);
extern "C" void func_0039F290(void* stack, void* state);
extern char D_00460790[];

// PORT: 64-bit `long` mask (8 bytes on EE, 4 on Windows); use uint64_t off-PS2.
struct sMsgCenter_98988 {
    char pad_0x0[0x10];
    char* engine;       // 0x10
    char pad_0x14[0x30];
    signed char rider;  // 0x44
    char pad_0x45[0x11B];
    ulong mask;         // 0x160
    int price;          // 0x168
};

extern "C" void func_00198988(sMsgCenter_98988* self, int idx)
{
    char* popup = (char*)func_001CABA8(cMemMan_alloc(0x70, D_00460790, 0, 0), self->engine, self, self->rider);
    int n = 0;
    for (int i = 0; i < 64; i++) {
        n += (int)((self->mask >> i) & 1);
    }
    int song = (int)func_00198A88(self, idx);
    if (n >= 6) {
        int cost = func_00198AE8_2(self, idx);
        cBuyPopupInfo_initBuySong((int*)(popup + 0x48), song, cost, self->price);
    } else {
        cBuyPopupInfo_initBuySongByCredit(popup + 0x48, song, 6 - n);
    }
    func_0039F290(self->engine + 0x18, popup);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198A88);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void* func_002B40F0(void* self, int i);

extern "C" void* func_00198A88(void* self, int i)
{
    return func_002B40F0((char*)func_0028B180() + 0x118, i);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198AB8);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void* func_002B4120(void* self, int i);

extern "C" void* func_00198AB8(void* self, int i)
{
    return func_002B4120((char*)func_0028B180() + 0x118, i);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198AE8__FPv);
#ifdef SKIP_ASM
int func_00198AE8(void* self)
{
    return 0x1388;
}
#endif

INCLUDE_ASM("fe/messagecenter", func_00198AF0);

//100%
INCLUDE_ASM("fe/messagecenter", func_00198DA0);
#ifdef SKIP_ASM
extern "C" int func_00198DA0(void* self, void* a1)
{
    void* p = *(void**)self;
    void* p2 = *(void**)a1;
    return *(short*)((char*)p + 0xa) - *(short*)((char*)p2 + 0xa);
}
#endif


#include "common.h"

//100%
struct sVE0720 {
    short delta;
    short index;
    int (*fn)(void*);
};
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0039E2A0(void* self, void* owner);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
extern char D_00460B90[];
extern void* D_0046D810[];
extern sVE0720 D_0046BB88[];

extern "C" void* cFECustom_cFECustom(void* self, void* owner)
{
    *(void**)((char*)self + 0x0) = owner;
    *(void**)((char*)self + 0x10) = 0;
    *(void***)((char*)self + 0x4) = D_0046D810;
    char* o = (char*)cMemMan_alloc(0x50, D_00460B90, 0, 0);
    func_0039E2A0(o, *(void**)((char*)self + 0x0));
    *(int*)(o + 0x48) = 0;
    *(int*)(o + 0x4C) = 0;
    *(sVE0720**)(o + 0x8) = D_0046BB88;
    *(void**)((char*)self + 0x10) = o;
    if (o != 0) {
        D_0046BB88[4].fn(o + D_0046BB88[4].delta);
        cUIStateStack_pushExplicit((char*)*(void**)((char*)self + 0x0) + 0x18, *(void**)((char*)self + 0x10));
    }
    return self;
}

//100%
int GetHashValue32(char* str);
extern char D_0045FFF8[];
struct sVEntry001A07C8 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" int func_001A07C8(void* self, int a1, int hash)
{
    if (hash == GetHashValue32(D_0045FFF8)) {
        void* obj = *(void**)((char*)self + 0x10);
        if (obj != 0) {
            sVEntry001A07C8* vt = *(sVEntry001A07C8**)((char*)obj + 8);
            vt[24].fn((char*)obj + vt[24].delta, 1, 0);
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM("fe/fereal", cFECustom_getNextState);

//100%
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_001BFBB8(void* self, void* owner);
extern "C" void* func_00181090(void* self, void* owner, int a);
extern "C" void* func_00195FF0(void* self, void* owner, int a, int b);
extern "C" void* func_001828C0(void* self, void* owner, int a);
extern "C" void* func_00186610(void* self, void* owner, int a);
extern "C" void* func_00194A60(void* self, void* owner);
extern "C" void* func_001887A0(void* self, void* owner, int a);
extern "C" void* func_0018D240(void* self, void* owner);
void* func_00232720(void* self);
class cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* func_001C1C50(void* self, void* owner);
extern "C" void* func_00194738(void* self, void* owner);
extern "C" void* func_001F3700(void* self, void* owner);
extern "C" void* func_001F3FF8(void* self, void* owner);
extern "C" void* func_00183238(void* self, void* owner, int a);
extern "C" void* cFEStateMountainRoom_cFEStateMountainRoom(void* self, void* owner, int a);
extern "C" void* func_001D3780(void* self, void* owner, int a, int b);
extern "C" void* func_001D4BC8(void* self, void* owner, int a, int b);
extern signed char D_00535C11[];
extern int D_004A19D8;
extern int D_004A1A70;
extern int D_004A2594;
extern int D_004A2598;
extern void* D_004A28A8;
extern char D_0045FF28[];
extern char D_00460BC0[];
extern char D_00460650[];
extern char D_00460C08[];
extern char D_00460C38[];
extern char D_00460BA8[];
extern char D_00461198[];
extern char D_00460D60[];
extern char D_0045CE60[];
extern char D_004611A8[];
extern char D_0045E290[];
extern char D_00461188[];
extern char D_00460DD8[];
extern char D_00460C68[];
extern char D_00460DF8[];
extern char D_00460E28[];
extern char D_00460C80[];

static inline int isOne16C0(int x) { return x == 1; }

struct sFEState16C0 {
    char pad0[0xC];
    int id;
    char pad10[0x34];
    signed char x44;
};

// Case order matters: cross-jumping keeps the later copy of identical case
// bodies, so the cases whose code the original shares come first.
extern "C" void* func_001A16C0(void** self, sFEState16C0* st, int a2)
{
    switch (st->id) {
    case 5:
    case 11:
    case 22:
        return func_00194A60(cMemMan_alloc(0x58, D_00460BA8, 0, 0), *self);
    case 42:
        return func_001F3FF8(cMemMan_alloc(0x4C, D_00460DD8, 0, 0), *self);
    case 12:
        return func_00181090(cMemMan_alloc(0x90, D_00460BC0, 0, 0), *self, st->x44);
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 36:
    case 37:
    case 38:
        return func_001887A0(cMemMan_alloc(0x54, D_00461198, 0, 0), *self, 0);
    case 9:
        if (isOne16C0(D_004A1A70))
            return func_001BFBB8(cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), *self);
        cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (D_00535C11[0]) {
        case 1:
            return func_00194A60(cMemMan_alloc(0x58, D_00460BA8, 0, 0), *self);
        case 0:
            return func_00194A60(cMemMan_alloc(0x58, D_00460BA8, 0, 0), *self);
        case 2:
            if (st->x44 == 1)
                return func_001828C0(cMemMan_alloc(0x50, D_00460C08, 0, 0), *self, 0);
            return func_00194A60(cMemMan_alloc(0x58, D_00460BA8, 0, 0), *self);
        }
        return 0;
    case 23:
        return func_00181090(cMemMan_alloc(0x90, D_00460BC0, 0, 0), *self, 1);
    case 13:
    case 14:
    case 16:
        if (D_004A19D8 != 0)
            return func_001F3700(cMemMan_alloc(0x58, D_00461188, 0, 0), *self);
        return func_001828C0(cMemMan_alloc(0x50, D_00460C08, 0, 0), *self, st->x44);
    case 15:
    case 18:
        if (D_004A19D8 != 0)
            return func_001F3FF8(cMemMan_alloc(0x4C, D_00460DD8, 0, 0), *self);
        return func_00183238(cMemMan_alloc(0x54, D_00460C68, 0, 0), *self, st->x44);
    case 17:
        return func_00195FF0(cMemMan_alloc(0x80, D_00460650, 0, 0), *self, st->x44, D_004A19D8);
    case 19:
        cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (D_00535C11[0]) {
        case 0:
        case 1:
            return func_001828C0(cMemMan_alloc(0x50, D_00460C08, 0, 0), *self, 0);
        case 2:
            return func_001828C0(cMemMan_alloc(0x50, D_00460C08, 0, 0), *self, 1);
        }
        return 0;
    case 20: {
        D_004A2598 = 1;
        D_004A2594 = 0;
        char* r = (char*)func_00186610(cMemMan_alloc(0x33C, D_00460C38, 0, 0), *self, 0);
        r[0x44] = 0;
        return r;
    }
    case 21:
        D_004A2598 = 1;
        D_004A2594 = 0;
        return func_00186610(cMemMan_alloc(0x33C, D_00460C38, 0, 0), *self, 0);
    case 8:
        cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (D_00535C11[0]) {
        case 0:
        case 1:
        case 2:
            return func_00194A60(cMemMan_alloc(0x58, D_00460BA8, 0, 0), *self);
        }
    case 35: {
        int x = ((int*)self)[3];
        if (x == 0x18)
            return func_0018D240(cMemMan_alloc(0x4C, D_00460D60, 0, 0), *self);
        if (x == 9)
            return func_00181090(cMemMan_alloc(0x90, D_00460BC0, 0, 0), *self, st->x44);
        if (x == 0x27)
            return func_001F3700(cMemMan_alloc(0x58, D_00461188, 0, 0), *self);
        break;
    }
    case 33:
        switch (a2) {
        case 0:
            if (((int*)self)[3] == 0x22)
                return func_001887A0(cMemMan_alloc(0x54, D_00461198, 0, 0), *self, 0);
            return func_0018D240(cMemMan_alloc(0x4C, D_00460D60, 0, 0), *self);
        case 1:
            cAppMan_setNextModule((cAppMan*)D_004A28A8, (unsigned int)func_00232720(cMemMan_alloc(8, D_0045CE60, 0x100, 0)));
            break;
        case 2:
            if (isOne16C0(D_004A1A70))
                return func_001C1C50(cMemMan_alloc(0x6D0, D_004611A8, 0, 0), *self);
            return 0;
        }
        break;
    case 2:
    case 39:
        return func_00194738(cMemMan_alloc(0x4C, D_0045E290, 0, 0), *self);
    case 40:
    case 41:
        return func_001F3700(cMemMan_alloc(0x58, D_00461188, 0, 0), *self);
    case 43:
        if (D_004A19D8 != 0)
            return func_001F3FF8(cMemMan_alloc(0x4C, D_00460DD8, 0, 0), *self);
        return func_001828C0(cMemMan_alloc(0x50, D_00460C08, 0, 0), *self, st->x44);
    case 46:
        if (D_004A19D8 != 0)
            return func_001F3FF8(cMemMan_alloc(0x4C, D_00460DD8, 0, 0), *self);
        return func_00183238(cMemMan_alloc(0x54, D_00460C68, 0, 0), *self, st->x44);
    case 44:
        return cFEStateMountainRoom_cFEStateMountainRoom(cMemMan_alloc(0xC8, D_00460DF8, 0, 0), *self, st->x44);
    case 45:
        return func_001D3780(cMemMan_alloc(0xE4, D_00460E28, 0, 0), *self, st->x44, a2);
    case 47:
        return func_001D4BC8(cMemMan_alloc(0x50, D_00460C80, 0, 0), *self, st->x44, a2 != 0);
    }
    return 0;
}

//100%
extern "C" void func_001A1CB8(void* self, int a1, int val)
{
    char* p = (char*)self + (signed char)a1;
    *(char*)(p + 0x8) = val;
}

//100%
extern "C" unsigned char func_001A1CD0(void* self, int a1)
{
    char* p = (char*)self + (signed char)a1;
    return *(unsigned char*)(p + 0x8);
}

//100%
extern "C" void* cFE_cFE(void* self);
void func_00263660(void* p);
void func_00263998(void* p);
extern void* D_0046D7C0[];

extern "C" void* func_001A1CE8(void* self)
{
    cFE_cFE(self);
    *(void***)self = D_0046D7C0;
    func_00263660((char*)self + 0xB5AB0);
    func_00263998((char*)self + 0xB5ABC);
    *(int*)((char*)self + 0xB5AD4) = 0;
    *(int*)((char*)self + 0xB5AD8) = 0;
    *(int*)((char*)self + 0xB5ADC) = 0;
    return self;
}

//100%
extern void* D_0046D7C0[];
extern "C" void func_002639B0(void* self, int flags);
extern "C" void func_00263678(void* self, int flags);
extern "C" void func_001957D0(void* self, int flags);

extern "C" void func_001A1D50(void* self, int flags)
{
    *(void***)self = D_0046D7C0;
    func_002639B0((char*)self + 0xB5ABC, 2);
    func_00263678((char*)self + 0xB5AB0, 2);
    func_001957D0(self, flags);
}

extern "C" void* cBE_getBE();
extern "C" void func_0014DE28(void* be, int b);
extern "C" void* func_003E6448(void* p, int c, int n);
extern "C" void cBE_setState(int state);
extern "C" void func_00369FF8(void* p);
extern "C" void func_00195E58(void* p);
extern "C" void cFEFlowState_setNumStates(void* flow, int n);
extern char D_004A4E80;
extern char D_005308B8[];
extern int D_004A19B8;
extern int D_004A4324;
extern int D_004A45D8;
extern int D_004A43C8;
extern void* D_004A289C;
extern void* D_004A28A8;

struct sVec3K1A1DC0 {
    int x, y, z;
    sVec3K1A1DC0() { x = 0; y = 0; z = 0; }
};
typedef void (*tFn_1A1DC0)(void*, const sVec3K1A1DC0&);
typedef void (*tVoid_1A1DC0)(void*);

//100%
extern "C" void func_001A1DC0(char* self)
{
    char* m;
    char* vt;
    if (D_004A19B8 != 0) {
        func_0014DE28(cBE_getBE(), 0);
        func_0014DE28(cBE_getBE(), 1);
        func_0014DE28(cBE_getBE(), 2);
        func_003E6448(D_005308B8, 0, 0x20);
        D_004A19B8 = 0;
    }
    *(int*)(self + 0xB5AD8) = 0;
    *(int*)(self + 0xB5ADC) = 0;
    cBE_setState(0);
    func_00369FF8(D_004A289C);
    func_00195E58(*(void**)((char*)D_004A28A8 + 0x8C));
    D_004A4324 = 0;
    D_004A45D8 = 0;
    D_004A43C8 = 0;
    m = (char*)D_004A289C;
    vt = *(char**)(m + 0x10D8);
    ((tFn_1A1DC0)*(void**)(vt + 0x7C))(m + *(short*)(vt + 0x78), sVec3K1A1DC0());
    cFEFlowState_setNumStates(&D_004A4E80, 0x56);
    ((tVoid_1A1DC0)*(void**)(*(char**)self + 0x44))(self + *(short*)(*(char**)self + 0x40));
}

INCLUDE_ASM("fe/fereal", cRealFE_load);

//100%
extern "C" void* func_00231CF0(void* self);
extern "C" void func_00253418(void* p, int a1);
extern "C" void func_001A06B0(void* flow);
extern int D_004A19CC;
extern char D_004A4E80;

extern "C" int func_001A2128(void* self)
{
    if (func_00231CF0(self) == 0) {
        return 0;
    }
    void* h = *(void**)((char*)self + 0xB5AD4);
    if (h != 0) {
        func_00253418(h, 3);
        *(void**)((char*)self + 0xB5AD4) = 0;
    }
    *(int*)((char*)self + 0xB5AD8) = 0;
    D_004A19CC = 0;
    *(int*)((char*)self + 0xB5ADC) = 0;
    func_001A06B0(&D_004A4E80);
    return 1;
}

//100%
struct sVEntry001A2190 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern void* D_004A289C;
extern void* D_004A28A8;
extern char D_005047F8[];
extern "C" void func_001A2BC0(void* self);
extern "C" void func_001A04C8(void* p);
extern "C" void func_0019CF40(void* p);
extern "C" void func_0038ADB0(void* p);
extern "C" void* func_0028B180();
extern "C" void func_00286200(void* p);

extern "C" void func_001A2190(void* self)
{
    char* g = (char*)D_004A289C;
    sVEntry001A2190* vt = *(sVEntry001A2190**)(g + 0x10D8);
    vt[19].fn(g + vt[19].delta);
    func_001A2BC0(self);
    func_001A04C8((char*)self + 0xB0);
    func_0019CF40((char*)self + 0x1A70);
    func_0038ADB0(D_005047F8);
    char* st = (char*)D_004A28A8;
    *(int*)(st + 0x7C) = 0;
    *(int*)(st + 0x80) = 0;
    func_00286200(func_0028B180());
}

//100%
extern "C" int func_00231D60(void* self);
extern "C" void func_00253890(void* h, int a1);
extern "C" void func_00398018(void* ui);
extern "C" void func_001A0598(void* p);
extern "C" void func_00397DF8(void* ui);
extern "C" void func_00285BF8(void* snd, int a1, float t);
extern "C" void* func_0028B180();
extern void* D_004A289C;
extern int D_004A19CC;
extern char D_004FF1A0[];

struct sFeGs {
    int w0;                         // 0x0
    unsigned int g0 : 2;            // 0x4
    unsigned int g2 : 5;
    unsigned int g7 : 5;
    unsigned int g12 : 8;
    unsigned int g20 : 2;
    unsigned int g22 : 1;
    unsigned int g23 : 2;
    unsigned int g25 : 7;
    unsigned int a0 : 5;            // 0x8
    unsigned int alpha : 5;
    unsigned int a10 : 22;
    int wC;                         // 0xC
    short tex;                      // 0x10
    short pad;

    void setG2(int v) { g2 = v; }
    void setG12(int v) { g12 = v; }
    void setG20(int v) { g20 = v; }
    void setG22(int v) { g22 = v; }
    void setG23(int v) { g23 = v; }
    void setAlpha(int v) { alpha = v; }
};

extern sFeGs D_00501420;

struct sFeVE {
    short delta;
    short index;
    void* fn;
};

struct sFeWorld {
    char pad_0x0[0xE84];
    sFeGs* top;                     // 0xE84
    char pad_0xE88[0x10D8 - 0xE88];
    sFeVE* vt;                      // 0x10D8
};

struct sFePos {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFeVert {
    float u, v, q, f0C;             // 0x00
    int r, g, b, a;                 // 0x10
    sFePos pos;                     // 0x20
    sFeVert() {}
};

static inline sFeWorld* feW() { return (sFeWorld*)D_004A289C; }

static inline void feCall(int slot)
{
    char* g = (char*)D_004A289C;
    sFeVE* vt = *(sFeVE**)(g + 0x10D8);
    ((void (*)(void*))vt[slot].fn)(g + vt[slot].delta);
}

static inline unsigned int feCallU(int slot)
{
    char* g = (char*)D_004A289C;
    sFeVE* vt = *(sFeVE**)(g + 0x10D8);
    return ((unsigned int (*)(void*))vt[slot].fn)(g + vt[slot].delta);
}

INCLUDE_ASM("fe/fereal", func_001A2208);
extern "C" int func_001A2208(void* self);

//100%
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_00231D18(void* self);
extern "C" void* func_002533C8(void* mem);
extern "C" void* func_0028B180();
extern "C" void func_002B3A70(void* p);
extern "C" void func_002B3A98(void* p);
extern "C" void func_00253390(void* self);
extern "C" int func_002534A8(void* p, void* q);
extern "C" int func_00253860(void* self);
extern "C" void func_00253938(void* self);
extern "C" void func_00253418(void* p, int a1);
int func_00289DE0(void* self);
void* func_00320C48(void* self, int a1);
extern "C" void func_00398038(void* self);
extern "C" void func_0019D250(void* p);
extern "C" void func_001A0608(void* p);
extern "C" void func_001A39F0(void* p);
extern "C" void func_00263828(void* p);
extern "C" void func_00263CF0(void* p);
extern "C" void func_0025AC50(void* p);
extern const char D_00461320[];
extern int D_004A19CC;
extern int D_004A19D0;
extern int D_004A19E0;
extern int D_004A19E4;
extern int D_00441248[];
extern int D_0044124C[];
extern int D_00441250[];
extern int D_00441254[];
extern unsigned int D_00535610[];
extern void* D_004A28A8;
extern int D_004A29EC;
extern void* D_004A3028;

struct sVidInfo_27A0 {
    int a;
    int b;
    int c;
    int pad[9];
};
struct sVE_27A0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_001A27A0(char* self)
{
    if (*(int*)(self + 0xB5ADC) != 0) return;
    func_00231D18(self);
    if (*(void**)(self + 0xB5AD4) == 0) {
        if (D_004A19CC != 0) {
            *(void**)(self + 0xB5AD4) = func_002533C8(cMemMan_alloc(0x44, D_00461320, 0x100, 0));
            func_002B3A70((char*)func_0028B180() + 0x118);
            sVidInfo_27A0 info;
            func_00253390(&info);
            int f = D_004A19CC;
            *(int*)(self + 0xB5AD8) = 0;
            if (f & 1) {
                info.a = D_00441248[0];
                D_004A19CC = f & 0xFFFFFFFE;
                D_004A19D0 = 1;
            } else if (f & 2) {
                info.a = D_0044124C[0];
                D_004A19CC = f & 0xFFFFFFFD;
                D_004A19D0 = 2;
            } else if (f & 4) {
                cBE_getInterface_Fv(cBE_getBE(), 4);
                if (((D_00535610[0] >> 22) & 7) == 0 && func_00289DE0(func_0028B180()) != 0) {
                    info.b = D_004A19E0;
                    info.a = D_00441250[0];
                } else {
                    info.b = D_004A19E4;
                    info.a = D_00441254[0];
                }
                D_004A19CC &= 0xFFFFFFFB;
                D_004A19D0 = 4;
                *(int*)(self + 0xB5AD8) = 1;
            }
            info.c = 1;
            func_002534A8(*(void**)(self + 0xB5AD4), &info);
            func_00253860(*(void**)(self + 0xB5AD4));
        }
        if (*(void**)(self + 0xB5AD4) == 0) goto noh;
    }
    {
    int done = **(int**)(self + 0xB5AD4);
    if (done == 0) {
        func_00253938(*(void**)(self + 0xB5AD4));
        if (*(int*)(self + 0xB5AD8) != 0) {
            for (int i = 0; i < 2; i++) {
                void** p = *(void***)((char*)D_004A28A8 + (i << 2) + 0xB0);
                int ok = *p != 0 && **(int**)p != 0;
                if (ok) {
                    if (func_00320C48(p, 0x70) != 0 || func_00320C48(p, 0x7A) != 0) {
                        done = 1;
                        goto out;
                    }
                }
            }
        }
    out:
        if (done == 0) return;
    }
    if (*(void**)(self + 0xB5AD4) != 0) {
        func_00253418(*(void**)(self + 0xB5AD4), 3);
    }
    int f = D_004A19CC;
    *(int*)(self + 0xB5ADC) = 1;
    *(void**)(self + 0xB5AD4) = 0;
    *(int*)(self + 0xB5AD8) = 0;
    if (f == 0) {
        func_002B3A98((char*)func_0028B180() + 0x118);
    }
    return;
    }
noh:
        if (*(int*)((char*)D_004A28A8 + 4) == 0) {
            func_00398038(*(void**)(self + 0xC));
        } else {
            *(int*)(self + 8) = 1;
        }
        sVE_27A0* vt = *(sVE_27A0**)(self + 0xA0);
        char* obj = self + 0x10;
        vt[3].fn(obj + vt[3].delta);
        func_0019D250(self + 0x1A70);
        func_001A0608(self + 0xB0);
        if (*(void**)((char*)D_004A28A8 + 0x11C) != 0) {
            func_001A39F0(*(void**)((char*)D_004A28A8 + 0x11C));
        }
        if (D_004A29EC != 0) {
            func_00263828(self + 0xB5AB0);
            func_00263CF0(self + 0xB5ABC);
            if (D_004A3028 != 0) {
                func_0025AC50(D_004A3028);
            }
        }
}

//100%
extern "C" void func_003DED50(char* name, int a1, int a2, void* out);
extern "C" void func_003DEDC0(void* handle, int arg);
extern "C" void* func_00311250(void* self, void* src);
extern "C" void func_00314F30(void* self, int kind, void* list);
// PORT: cGameAnimMap_testResolve__Fv is called with the map in $a0; bind the 1-arg form to that symbol.
void cGameAnimMap_testResolve_p(void* map) __asm__("cGameAnimMap_testResolve__Fv");
extern char D_00461330[];
extern char D_00461348[];
extern void* D_004A19C0[1];
struct sAnimTableK1A2B00 {
    void* anims[1];
};
extern sAnimTableK1A2B00* D_004A3DF8_K1A2B00 __asm__("D_004A3DF8");
extern void* D_004A3E7C;

extern "C" void cRealFE_loadCharAnimations(void)
{
    void* handle;
    int i;
    func_003DED50(D_00461330, 0, 0x64, &handle);
    for (i = 0; i < 1; i++) {
        void* anim = func_00311250(cMemMan_alloc(0x18, D_00461348, 0, 0), D_004A19C0[i]);
        D_004A3DF8_K1A2B00->anims[(unsigned char)i] = anim;
        func_00314F30(D_004A3E7C, i, anim);
    }
    cGameAnimMap_testResolve_p(D_004A3E7C);
    func_003DEDC0(handle, 0x64);
}

//100%
extern void** D_004A3DF8;
extern void* D_004A3E7C;
extern "C" void func_00314FE8(void* a, void* b);
extern "C" void func_00311110(void* a);
extern "C" void func_003112C8(void* a, int b);

extern "C" void func_001A2BC0(void* self)
{
    int i;
    for (i = 0; i < 1; i++) {
        unsigned char idx = i;
        void* p = *(void**)((char*)D_004A3DF8 + (idx << 2));
        if (p != 0) {
            func_00314FE8(D_004A3E7C, p);
            void** tbl = D_004A3DF8;
            *(void**)((char*)tbl + (idx << 2)) = 0;
            func_00311110(tbl);
            func_003112C8(p, 3);
        }
    }
}

//100%
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0039E2A0(void* self, void* owner);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
extern "C" void func_0039F400(void* stack, void* state);
extern "C" void* func_00194738(void* self, void* owner);
extern "C" void* func_001F3700(void* self, void* owner);
extern "C" void* func_001D5038(void* self, void* owner);
extern "C" void func_00255CC8(void* a, int b);
extern "C" void func_00266E88(void* a);
extern "C" void func_00266DF8(void* a);
extern "C" void func_0025B6D8(void* a, int b);
extern "C" void func_0014ECA0(void* a, int b);
extern "C" void func_0014DE28(void* be, int b);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern char D_00461358[];
extern char D_0045E290[];
extern char D_00461188[];
extern char D_00461370[];
extern void* D_0046BC58[];
extern int D_004A19C4;
extern int D_004A19D8;
extern void* D_004A28A8;
extern void* D_004A2EB8;
extern char* D_004A2EEC;
extern void* D_004A33F4;
extern void* D_004A3028;

// PORT: cMemMan_alloc bound as a placement operator new (gcc then treats the result as unaliased)
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
struct sVEnt_1A2C40 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sStartState_1A2C40 {
    int f0;
    int f4;
    sVEnt_1A2C40* vt;   // 0x8
    char pad[0x3C];
};
struct sRealFE_1A2C40 {
    char pad0[0xC];
    char* ui;   // 0xC
};

extern "C" void cRealFE_loadStartState(sRealFE_1A2C40* self)
{
    sStartState_1A2C40* s = new (D_00461358, 0, 0) sStartState_1A2C40;
    func_0039E2A0(s, self->ui);
    s->vt = (sVEnt_1A2C40*)D_0046BC58;
    s->vt[4].fn((char*)s + s->vt[4].delta);
    cUIStateStack_pushExplicit(self->ui + 0x18, s);
    switch (D_004A19C4) {
    case 0: {
        D_004A19D8 = 0;
        void* st = func_00194738(cMemMan_alloc(0x4C, D_0045E290, 0, 0), self->ui);
        func_0039F400(self->ui + 0x18, st);
        break;
    }
    case 0x27: {
        D_004A19D8 = 1;
        void* st = func_001F3700(cMemMan_alloc(0x58, D_00461188, 0, 0), self->ui);
        func_0039F400(self->ui + 0x18, st);
        D_004A19C4 = 0;
        break;
    }
    case 0x34: {
        D_004A19D8 = 0;
        void* st = func_001D5038(cMemMan_alloc(0x6D4, D_00461370, 0, 0), self->ui);
        func_0039F400(self->ui + 0x18, st);
        func_00255CC8(D_004A2EB8, *(int*)(D_004A2EEC + 0x6C));
        func_00266E88(D_004A33F4);
        func_00266DF8(D_004A33F4);
        func_0025B6D8(D_004A3028, 1);
        func_0014ECA0(cBE_getInterface_Fv(*(void**)((char*)D_004A28A8 + 0x78), 7), 0);
        func_0014DE28(cBE_getBE(), 1);
        func_0014DE28(cBE_getBE(), 2);
        D_004A19C4 = 0;
        break;
    }
    }
}

//100%
void* func_0039E288(void* self);
extern void* D_00469348[];

extern "C" void* func_001A2E20(void* self)
{
    func_0039E288(self);
    *(void***)self = D_00469348;
    *(int*)((char*)self + 0x4) = 0;
    return self;
}

//100%
extern "C" void* func_0028B180();
extern "C" void func_00294F78(void* snd, int a);

extern "C" void func_001A2E58(void* self, int which)
{
    switch (which) {
    case 1:
        func_00294F78(func_0028B180(), 2);
        break;
    case 2:
        func_00294F78(func_0028B180(), 3);
        break;
    case 3:
        func_00294F78(func_0028B180(), 1);
        break;
    case 4:
        func_00294F78(func_0028B180(), 4);
        break;
    case 5:
        func_00294F78(func_0028B180(), 0);
        break;
    case 6:
        func_00294F78(func_0028B180(), 0);
        break;
    }
}

//100%
void* func_0039E288(void* self);
extern void* D_00469318[];

extern "C" void* func_001A2F38(void* self)
{
    func_0039E288(self);
    *(void***)self = D_00469318;
    *(int*)((char*)self + 0x4) = 0;
    return self;
}

//100%
extern "C" void* func_0028B180();
extern "C" void func_00294F78(void* snd, int a);

extern "C" void func_001A2F70(void* self, int which)
{
    switch (which) {
    case 1:
        func_00294F78(func_0028B180(), 11);
        break;
    case 2:
        func_00294F78(func_0028B180(), 12);
        break;
    case 3:
        func_00294F78(func_0028B180(), 10);
        break;
    case 4:
        func_00294F78(func_0028B180(), 13);
        break;
    case 5:
        func_00294F78(func_0028B180(), 9);
        break;
    case 6:
        func_00294F78(func_0028B180(), 9);
        break;
    }
}


extern char D_00469248[];

//100%
extern "C" void* func_001A3050(void* self, void* owner)
{
    func_0039E2A0(self, owner);
    *(void**)((char*)self + 0x8) = D_00469248;
    *(int*)((char*)self + 0xC) = 0x16;
    return self;
}

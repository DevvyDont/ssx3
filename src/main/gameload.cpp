#include "common.h"

INCLUDE_ASM("main/gameload", cGame_load);

INCLUDE_ASM("main/gameload", cGame_loadTrack);

INCLUDE_ASM("main/gameload", func_0022FA98);

//100%
INCLUDE_ASM("main/gameload", func_00230050);
#ifdef SKIP_ASM
class cMgr230050 {
public:
    int pad;
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
};
extern "C" void func_00195D50(void* a);
extern "C" void func_001F30D8(void* a);
void func_00278710(void* a);
extern "C" void func_00369FF8(void* a);
extern "C" void func_003A67E0(void* a);
extern "C" void func_002B68D0(void* a);
extern "C" void func_002867E8(void* a);
extern void* D_004A28A8;
extern void* D_004A28A4;
extern void* D_004A289C;
extern void* D_004A52D4;
extern void* D_004A3500;
extern void* D_004A270C;
extern cMgr230050* D_004A4248;
extern int D_004A43C8;
extern int D_004A4324;
extern int D_004A45D8;

extern "C" void func_00230050(void* self)
{
    func_00195D50(*(void**)((char*)D_004A28A8 + 0x8C));
    void* p94 = *(void**)((char*)self + 0x94);
    D_004A270C = 0;
    func_001F30D8(p94);
    func_00278710(D_004A28A4);
    D_004A4248->v17();
    D_004A4248->v04();
    func_00369FF8(D_004A289C);
    func_003A67E0(*(void**)((char*)self + 0x10));
    func_002B68D0(D_004A52D4);
    func_002867E8(D_004A3500);
    D_004A4324 = 1;
    D_004A45D8 = 1;
    D_004A43C8 = 1;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002300F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0026CA90(void* a);
extern "C" void func_00309030(void* a);
extern "C" void func_00354C98(void* mgr, int a);
extern "C" void cBucketMan_purgeBucket(void* self, int index);
extern "C" void func_00103358(void* a);
extern "C" void func_002D9B40(void);
extern "C" void cWorld_resetMap(void* world);
extern "C" void func_00309F18(void* a);
extern "C" void func_00308C60(void* a);
extern void* D_004A3DD8;
// The unit declares D_004A5988 later as sGp2898 (8 bytes); bind a small-data alias here.
extern char D_004A5988_bm __asm__("D_004A5988");

extern "C" void func_002300F0(void* self)
{
    void* bm = &D_004A5988_bm;
    func_0026CA90(*(void**)((char*)self + 0x34));
    func_00309030(*(void**)((char*)self + 0x30));
    func_00354C98(bm, 1);
    cBucketMan_purgeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 8);
    func_00103358(*(void**)(*(char**)((char*)self + 0xC) + 0xA4));
    func_002D9B40();
    cWorld_resetMap(*(void**)((char*)self + 0x10));
    func_00309F18(D_004A3DD8);
    func_00308C60(*(void**)((char*)self + 0x30));
}
#endif

INCLUDE_ASM("main/gameload", func_00230180);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/gameload", cGame_restart);
#ifdef SKIP_ASM
extern "C" void func_00230180(void* self);
extern "C" void cGameModeMan_restartHeat(void* gm);
extern "C" void func_00231250(void* self, int a, int b, int refresh);
extern "C" void* func_0028B180(void);
extern "C" void func_002870A0(void* mgr);
extern void* D_004A28A8;

static inline int numRiders_2302A8(void* game)
{
    return *(int*)(*(char**)((char*)game + 0xC) + 0x78);
}

static inline char* rider_2302A8(void* game, int i)
{
    return *(char**)(*(char**)((char*)game + 0xC) + (i << 2) + 0x28);
}

extern "C" void cGame_restart(void* self)
{
    func_00230180(self);
    cGameModeMan_restartHeat(*(void**)((char*)D_004A28A8 + 0xC0));
    int i;
    for (i = 0; i < numRiders_2302A8(self); i++) {
        *(int*)(rider_2302A8(self, i) + 0x2F8) = 0;
    }
    func_00231250(self, 1, 2, 1);
    func_002870A0(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230338);
#ifdef SKIP_ASM
extern "C" void func_00230338(void* self)
{
    void* a = *(void**)((char*)self + 0xC);
    if (a != 0) {
        void* b = *(void**)((char*)a + 0xA4);
        if (b != 0) {
            *(int*)((char*)b + 0xD0) = -1;
        }
    }
}
#endif

INCLUDE_ASM("main/gameload", func_00230360);

INCLUDE_ASM("main/gameload", func_00230430);

INCLUDE_ASM("main/gameload", cGame_exit);

//100%
INCLUDE_ASM("main/gameload", func_002305C8);
#ifdef SKIP_ASM
extern "C" void func_0026F980(void* a);
void func_002CC0B8(void* a, int b);
extern "C" void func_002CC460(void* a);
extern int D_004A2A54;
extern int D_004A2A50;
extern int D_005366E8[];
extern int D_004428F0[];

extern "C" void func_002305C8(void* self)
{
    func_0026F980(*(void**)((char*)self + 0x28));
    func_002CC0B8(*(void**)((char*)self + 0xA4), 1);
    func_002CC460(*(void**)((char*)self + 0xA4));
    D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230640);
#ifdef SKIP_ASM
extern "C" void func_003550A0(void* mgr, int id);
struct sGp2898 { int a, b; };
extern sGp2898 D_004A5988; // target: raw $gp+0x2898 (splat doesn't symbolize it)
extern int D_004428D8[];

extern "C" void func_00230640(void)
{
    int i;
    sGp2898* mgr = &D_004A5988;
    for (i = 0; i < 6; i++) {
        func_003550A0(mgr, D_004428D8[i]);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230698__FPvi);
#ifdef SKIP_ASM
int func_00230698(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x5C);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002306A8__FPvi);
#ifdef SKIP_ASM
int func_002306A8(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x68);
}
#endif

INCLUDE_ASM("main/gameload", func_002306B8);

INCLUDE_ASM("main/gameload", func_00230E98);

INCLUDE_ASM("main/gameload", func_00230F40);

//100%
INCLUDE_ASM("main/gameload", func_00231250);
#ifdef SKIP_ASM
extern "C" void func_00230F40(void* self);

extern "C" void func_00231250(void* self, int a, int b, int refresh)
{
    *(int*)((char*)self + 0x210) = a;
    *(int*)((char*)self + 0x21C) = b;
    if (refresh != 0) {
        func_00230F40(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231278__FPvi);
#ifdef SKIP_ASM
void func_00231278(void* self, int val)
{
    *(int*)((char*)self + 0x208) = val;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002312D8);
#ifdef SKIP_ASM
struct sVEntry_002312D8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_002312D8(void* self)
{
    if (*(int*)((char*)self + 0x208) != 0) {
        return 0;
    }
    void* p = *(void**)((char*)self + 0x204);
    if (p == 0) {
        return 1;
    }
    sVEntry_002312D8* vt = *(sVEntry_002312D8**)((char*)p + 0xC);
    return vt[8].fn((char*)p + vt[8].delta);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231320);
#ifdef SKIP_ASM
extern "C" void func_00233AA0(void* p);

extern "C" void func_00231320(void* self)
{
    void* p = *(void**)((char*)self + 0x200);
    if (p != 0) {
        func_00233AA0(p);
    }
}
#endif

INCLUDE_ASM("main/gameload", func_00231348);

INCLUDE_ASM("main/gameload", func_002314D0);

INCLUDE_ASM("main/gameload", func_00231840);

//100%
INCLUDE_ASM("main/gameload", func_00231AB8);
#ifdef SKIP_ASM
extern "C" int func_00278DA0(void* a);
extern void* D_004A28A4;

extern "C" int func_00231AB8(void* self)
{
    int mode = *(int*)((char*)self + 0x210);
    if (mode != 0) {
        if (mode == 5 || mode == 14 || mode == 1 || mode == 10) {
            return 0;
        }
    }
    int r = 0;
    if (func_00278DA0(D_004A28A4) != 0) {
        int m = *(int*)((char*)self + 0x214);
        r = m != 5 && m != 14 && m != 1 && m != 10;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231C70);
#ifdef SKIP_ASM
extern "C" int func_00231CF0(void* self);
extern char* D_004A5B64;

extern "C" int func_00231C70(void* self)
{
    if (*(float*)(D_004A5B64 + 0x38) == 100.0f) {
        return func_00231CF0(self);
    }
    return 0;
}
#endif

extern "C" void* func_00231D18(void* self);

//100%
INCLUDE_ASM("main/gameload", func_00231CB0__FPv);
#ifdef SKIP_ASM
void* func_00231CB0(void* self)
{
    return func_00231D18(self);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231CD0);
#ifdef SKIP_ASM
extern char D_0047D980[];

extern "C" void* func_00231CD0(void* self)
{
    *(void**)((char*)self + 0) = D_0047D980;
    *(float*)((char*)self + 4) = 0.20000000298023224f;
    *(int*)((char*)self + 8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231CF0);
#ifdef SKIP_ASM
extern "C" int func_00231CF0(void* self)
{
    *(int*)((char*)self + 8) = 1;
    return *(float*)((char*)self + 4) >= 0.20000000298023224f;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231D18);
#ifdef SKIP_ASM
// PORT: the unit declares this void*-returning; the body returns nothing meaningful.
extern "C" void* func_00231D18(void* self)
{
    if (*(int*)((char*)self + 8) != 0) {
        *(float*)((char*)self + 4) += 0.01666666753590107f;
    } else {
        *(float*)((char*)self + 4) -= 0.01666666753590107f;
        if (*(float*)((char*)self + 4) < 0.0f) {
            *(float*)((char*)self + 4) = 0.0f;
        }
    }
}
#endif

INCLUDE_ASM("main/gameload", func_00231D60);

INCLUDE_ASM("main/gameload", func_00231F80);

INCLUDE_ASM("main/gameload", func_00231FC0);

extern void* D_0047D938[];

//100%
INCLUDE_ASM("main/gameload", func_00232328__FPv);
#ifdef SKIP_ASM
void* func_00232328(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)self = (int)(void*)D_0047D938;
    *(int*)((char*)self + 0x8) = t0;
    return self;
}
#endif


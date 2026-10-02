#include "common.h"

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045DC70[];
extern char D_0045DC80[];
extern void* D_004A28A8;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* func_00227F80(void* p);

class cUIObjK186B50 {
public:
    char pad_0x00[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int);
};

extern "C" void cFEStateMemCard_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045DC70), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_00227F80(D_004A28A8);
    ((cUIObjK186B50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045DC80)))->v09(0);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_00186BF0);
#ifdef SKIP_ASM
extern char D_004A14C0[];
extern "C" void func_001D8DE0(void* self);
extern "C" int func_0023C898(void* mp);
extern "C" void func_0039FD38(void* obj);

class cUIObjK186BF0 {
public:
    char pad_0x000[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int);
};

extern "C" void func_00186BF0(void* self)
{
    func_001D8DE0(self);
    if (func_0023C898(func_00227F80(D_004A28A8)) != 0 || *(int*)((char*)self + 0x1C0) == 6) {
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A14C0));
        if (obj != 0) {
            func_0039FD38(obj);
        }
        ((cUIObjK186BF0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0045DC80)))->v09(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_00186C98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0045DC90[];
extern "C" void func_00241DC8(void* mp, void* out, int a2);
extern "C" void func_002C26D0(unsigned short* dst, const unsigned short* fmt, ...);

class cUIObjK186C98 {
public:
    char pad_0x000[0x4];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual int v04(int hash);
};

class cFEMemCardStateK186C98 {
public:
    char pad_0x000[0x8];
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
    virtual void v36(void* buf, int a, int b, int c, int d, int e);
};
extern cFEMemCardStateK186C98* D_004A14B8_K186C98 __asm__("D_004A14B8");

struct sVtEntK186C98 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

struct sLocalK186C98 {
    int v[8];
};

extern "C" void func_00186C98(void)
{
    unsigned short buf[0x320];
    sLocalK186C98 loc;
    void* mp = func_00227F80(D_004A28A8);
    func_00241DC8(mp, &loc, *(int*)((char*)mp + 0x428));
    char* o = *(char**)((char*)D_004A28A8 + 0x8C);
    sVtEntK186C98* vt = *(sVtEntK186C98**)(o + 4);
    unsigned short* r = vt[4].fn(o + vt[4].delta, GetHashValue32(D_0045DC90));
    func_002C26D0(buf, r, &loc);
    cFEMemCardStateK186C98* st = D_004A14B8_K186C98;
    *(int*)((char*)st + 0x19C) = 4;
    st->v36(buf, 1, 1, 0, 0, 0);
}
#endif

INCLUDE_ASM("fe/fememcard", func_00186D60);

INCLUDE_ASM("fe/fememcard", func_00186F40);

INCLUDE_ASM("fe/fememcard", func_00186F90);

//100%
INCLUDE_ASM("fe/fememcard", func_00187148);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cFEMemCardStateK187148 {
public:
    char pad_0x000[0x8];
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
};
// Typed view of D_004A14B8 (other functions in this unit declare it as void*).
extern cFEMemCardStateK187148* D_004A14B8_K187148 __asm__("D_004A14B8");

extern "C" void func_00187148(void)
{
    D_004A14B8_K187148->v41();
    *(int*)((char*)D_004A14B8_K187148 + 0x1C0) = 6;
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_00187180);
#ifdef SKIP_ASM
extern void* D_004A14B8;
extern "C" void func_001D83A8(void);
extern "C" void func_00187A38(void* self);

extern "C" void func_00187180(void)
{
    func_001D83A8();
    func_00187A38(D_004A14B8);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_001871A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004A28A8;
struct sFEMemCardK1871A8 {
    char pad_0x000[0x1BC];
    int state;     // 0x1BC
    char pad_0x1C0[0x22C - 0x1C0];
    int f22C;      // 0x22C
    int pad_0x230;
    int f234;      // 0x234
};
// Typed view of D_004A14B8 (other functions in this unit declare it as void*).
extern sFEMemCardK1871A8* D_004A14B8_K1871A8 __asm__("D_004A14B8");
extern int D_004A14BC;
extern "C" void* func_00227F80(void* p);
void func_0023FB18(void* mp, int a);
extern "C" void func_0023FAE0(void* mp);
extern "C" void func_0023C8F0(void* mp, int mode);

class cMoviePlayerK1871A8 {
public:
    char pad_0x000[0x748];
    virtual void v01(int);
};

extern "C" void func_001871A8(void)
{
    cMoviePlayerK1871A8* mp = (cMoviePlayerK1871A8*)func_00227F80(D_004A28A8);
    func_0023FB18(mp, 0);
    func_0023FAE0(mp);
    mp->v01(0x36);
    D_004A14BC = 0;
    D_004A14B8_K1871A8->f22C = -1;
    D_004A14B8_K1871A8->f234 = 0;
    func_0023C8F0(mp, 2);
    D_004A14B8_K1871A8->state = 2;
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_00187230);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004A28A8;
extern int D_004A14BC;
struct sFEMemCardK187230 {
    char pad_0x000[0x22C];
    int f22C;      // 0x22C
    int pad_0x230;
    int f234;      // 0x234
};
// Typed view of D_004A14B8 (other functions in this unit declare it as void*).
extern sFEMemCardK187230* D_004A14B8_K187230 __asm__("D_004A14B8");
extern "C" void* func_00227F80(void* p);
extern "C" void func_0023C8F0(void* mp, int mode);

extern "C" void func_00187230(void)
{
    void* mp = func_00227F80(D_004A28A8);
    D_004A14BC = 0;
    D_004A14B8_K187230->f22C = -1;
    D_004A14B8_K187230->f234 = 0;
    func_0023C8F0(mp, 2);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_00187270);
#ifdef SKIP_ASM
extern "C" void func_00187318(void* self);
extern "C" void func_001877B0(void);
extern "C" void func_00187920(void);

struct sFEMemCardK187270 {
    char pad_0x000[0x234];
    int f234;      // 0x234
    int flags;     // 0x238
};

extern "C" void func_00187270(sFEMemCardK187270* self)
{
    self->f234 = 1;
    void* mp = func_00227F80(D_004A28A8);
    int flags = self->flags;
    if (!(flags & 2)) {
        func_0023FB18(mp, 0);
        func_0023FAE0(mp);
        func_00187920();
    } else if (!(flags & 1)) {
        func_0023FB18(mp, 0);
        func_0023FAE0(mp);
        func_001877B0();
    } else {
        func_00187318(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_00187318);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_00228238(void* p);

extern "C" void func_00187318(void* self)
{
    *(int*)((char*)self + 0x1C0) = 6;
    if (*(int*)((char*)self + 0x1AC) == 0)
        func_00228238(D_004A28A8);
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

INCLUDE_ASM("fe/fememcard", func_00187360);

//100%
INCLUDE_ASM("fe/fememcard", func_001874E8);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern int D_004A14BC;
extern "C" void* func_00227F80(void* p);
extern "C" void func_0023FB58(void* mp);
extern "C" void func_001877B0(void);
extern "C" void func_00187318(void* self);

static inline int moviePlaying_K1874E8(void* mp)
{
    int r = 0;
    if (*(int*)((char*)mp + 0x440) == 0)
        r = *(int*)((char*)mp + 0x43C) != 0;
    return r;
}

extern "C" void func_001874E8(void* self)
{
    if (*(int*)((char*)self + 0x1AC) != 0)
        return;
    void* mp = func_00227F80(D_004A28A8);
    if (moviePlaying_K1874E8(mp))
        return;
    if (D_004A14BC == 4 && ((*(int*)((char*)self + 0x238) ^ 1) & 1)) {
        func_0023FB58(mp);
        func_001877B0();
        return;
    }
    func_00187318(self);
}
#endif

INCLUDE_ASM("fe/fememcard", func_00187580);

INCLUDE_ASM("fe/fememcard", func_001876A8);

INCLUDE_ASM("fe/fememcard", func_001877B0);

INCLUDE_ASM("fe/fememcard", func_00187920);

INCLUDE_ASM("fe/fememcard", func_00187A38);

//100%
INCLUDE_ASM("fe/fememcard", func_00187C10__FPv);
#ifdef SKIP_ASM
int func_00187C10(void* self)
{
    return 0;
}
#endif


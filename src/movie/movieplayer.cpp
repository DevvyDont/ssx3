#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00242288(void* self);
extern const char D_0047C128[];
extern void* D_004A2C74;

//99.53%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getManager__Fv);
#ifdef SKIP_ASM
void* cMCOverlayManager_getManager()
{
    if (D_004A2C74 == 0) {
        void* mem = cMemMan_alloc(0x74C, D_0047C128, 0, 0);
        D_004A2C74 = func_00242288(mem);
    }
    return D_004A2C74;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C860);
#ifdef SKIP_ASM
struct sMovieObj_23C860 {
    char pad[0x748];
    virtual void v01();
    virtual ~sMovieObj_23C860();
};

extern "C" void func_0023C860()
{
    delete (sMovieObj_23C860*)D_004A2C74;
    D_004A2C74 = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C898);
#ifdef SKIP_ASM
extern "C" int func_0023C898(void* self)
{
    return *(int*)((char*)self + 0x130) == 1;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C8D0);
#ifdef SKIP_ASM
extern "C" int func_0023C8D0(void* self)
{
    // masks applied as separate statements; folding them into one
    // expression collapses the two `and` instructions into one
    int v = *(int*)((char*)self + 0x43c);
    v &= -5;
    v &= -481;
    return v != 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C8F0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: func_00152948__FPv ignores its argument; this caller passes none.
int func_00152948_r() __asm__("func_00152948__FPv");
extern "C" int func_00152BA8();
extern "C" void* func_0023D570(void* self, int mode);
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_00241D40_r(void* self, int mode, void* buf, int size) __asm__("func_00241D40");
extern "C" void func_00241AA0_r(void* buf, int size) __asm__("func_00241AA0");
extern "C" void func_002C26D0_v(void* dst, void* fmt, ...) __asm__("func_002C26D0");
extern char D_004A2C80[];

struct sMovieVEntry_23C8F0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023C8F0(void* self, int mode)
{
    char desc[0x200];
    char name[0x200];
    char fmt[0x200];
    char* s = (char*)self;
    *(int*)(s + 0xB4) = mode;
    func_0023D570(self, 1);
    *(int*)(s + 0xFC) = -1;
    *(int*)(s + 0xBC) = 0;
    int m = *(int*)(s + 0xB4);
    if (m == 0) {
        *(int*)(s + 0xEC) = 4;
        char* o = *(char**)(s + 0x434);
        sMovieVEntry_23C8F0* vt = *(sMovieVEntry_23C8F0**)o;
        vt[19].fn(o + vt[19].delta, 0x80000);
    } else if (m == 1) {
        *(int*)(s + 0xEC) = 6;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int v = func_00152BA8();
        char* o = *(char**)(s + 0x434);
        sMovieVEntry_23C8F0* vt = *(sMovieVEntry_23C8F0**)o;
        vt[19].fn(o + vt[19].delta, v);
    } else {
        *(int*)(s + 0xEC) = 1;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int v = func_00152948_r();
        char* o = *(char**)(s + 0x434);
        sMovieVEntry_23C8F0* vt = *(sMovieVEntry_23C8F0**)o;
        vt[19].fn(o + vt[19].delta, v);
    }
    func_002C2540(fmt, D_004A2C80);
    func_00241AA0_r(desc, 0x200);
    func_00241D40_r(self, *(int*)(s + 0xB4), name, 0x100);
    func_002C26D0_v(s + 0x1B4, fmt, desc, name);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CA28);
#ifdef SKIP_ASM
struct sMovieVEntryCA28 {
    short delta;
    short index;
    void (*fn)(void*, int, int, int);
};

extern "C" void func_0023CA28(void* self, int a, int b, int c)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntryCA28* vt = *(sMovieVEntryCA28**)obj;
    vt[29].fn((char*)obj + vt[29].delta, a, b, c);
    *(int*)((char*)self + 0xE8) = c;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CA70);
#ifdef SKIP_ASM
struct sMovieVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023CA70(void* self)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry* vt = *(sMovieVEntry**)obj;
    vt[32].fn((char*)obj + vt[32].delta);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CAA0);
#ifdef SKIP_ASM
struct sMovieVEntryCAA0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// PORT: the unit declares func_0023CAA0(void* self), but the body takes a
// second argument (callers pass it in $5); the real body is bound by asm label.
void func_0023CAA0_impl(void* self, int a) __asm__("func_0023CAA0");

void func_0023CAA0_impl(void* self, int a)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntryCAA0* vt = *(sMovieVEntryCAA0**)obj;
    vt[19].fn((char*)obj + vt[19].delta, a);
    *(int*)((char*)self + 0xF0) = a;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CAE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: func_00152948__FPv ignores its argument; this caller passes none.
int func_00152948_r() __asm__("func_00152948__FPv");
extern "C" int func_00152BA8();
extern "C" void* func_0023D570(void* self, int mode);
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_00241D40_r(void* self, int mode, void* buf, int size) __asm__("func_00241D40");
extern "C" void func_00241AA0_r(void* buf, int size) __asm__("func_00241AA0");
extern "C" void func_002C26D0_v(void* dst, void* fmt, ...) __asm__("func_002C26D0");
extern char D_004A2C80[];

struct sMovieVEntry_23CAE8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023CAE8(void* self, int mode, int a2)
{
    char desc[0x200];
    char name[0x200];
    char fmt[0x200];
    char* s = (char*)self;
    *(int*)(s + 0xB4) = mode;
    func_0023D570(self, 1);
    *(int*)(s + 0xFC) = a2;
    *(int*)(s + 0xBC) = 1;
    int m = *(int*)(s + 0xB4);
    if (m == 0) {
        *(int*)(s + 0xEC) = 4;
        char* o = *(char**)(s + 0x434);
        sMovieVEntry_23CAE8* vt = *(sMovieVEntry_23CAE8**)o;
        vt[19].fn(o + vt[19].delta, 0x80000);
    } else if (m == 1) {
        *(int*)(s + 0xEC) = 6;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int v = func_00152BA8();
        char* o = *(char**)(s + 0x434);
        sMovieVEntry_23CAE8* vt = *(sMovieVEntry_23CAE8**)o;
        vt[19].fn(o + vt[19].delta, v);
    } else {
        *(int*)(s + 0xEC) = 1;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int v = func_00152948_r();
        char* o = *(char**)(s + 0x434);
        sMovieVEntry_23CAE8* vt = *(sMovieVEntry_23CAE8**)o;
        vt[19].fn(o + vt[19].delta, v);
    }
    func_002C2540(fmt, D_004A2C80);
    func_00241AA0_r(desc, 0x200);
    func_00241D40_r(self, *(int*)(s + 0xB4), name, 0x100);
    func_002C26D0_v(s + 0x1B4, fmt, desc, name);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CC20);
#ifdef SKIP_ASM
extern "C" void func_0023CC20(void* self)
{
    if (*(int*)((char*)self + 0x118) != 0) {
        void* obj = *(void**)((char*)self + 0x434);
        sMovieVEntry* vt = *(sMovieVEntry**)obj;
        vt[43].fn((char*)obj + vt[43].delta);
    }
}
#endif

extern "C" void* func_0023CAA0(void* self);

//99.29%
INCLUDE_ASM("movie/movieplayer", func_0023CC58__FPv);
#ifdef SKIP_ASM
void* func_0023CC58(void* self)
{
    return func_0023CAA0(self);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CC78);
#ifdef SKIP_ASM
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_00241D40_r(void* self, int mode, void* buf, int size) __asm__("func_00241D40");
extern "C" void func_002C26D0_v(void* dst, void* fmt, ...) __asm__("func_002C26D0");
extern "C" void* func_002C2508(void* dst, void* src);
int GetHashValue32(char* s);
extern void* D_004A28A8;
extern char D_004A2C88[];
extern char D_0047C138[];

struct sLocVEntry_23CC78 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

extern "C" void func_0023CC78(void* self, int n, char* dst)
{
    char name[0x100];
    char fmt[0x200];
    switch (*(int*)((char*)self + 0xB4)) {
    case 0:
        func_002C2540(fmt, D_004A2C88);
        func_00241D40_r(self, *(int*)((char*)self + 0xB4), name, 0x80);
        func_002C26D0_v(dst, fmt, name, n + 1);
        break;
    case 2: {
        char* loc = *(char**)((char*)D_004A28A8 + 0x8C);
        sLocVEntry_23CC78* vt = *(sLocVEntry_23CC78**)(loc + 4);
        char* thisp = loc + vt[4].delta;
        func_002C2508(dst, vt[4].fn(thisp, GetHashValue32(D_0047C138)));
        break;
    }
    case 1:
        func_002C2540(fmt, D_004A2C88);
        func_00241D40_r(self, *(int*)((char*)self + 0xB4), name, 0x80);
        func_002C26D0_v(dst, fmt, name, n + 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_setTitleString);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_002C26D0_v(void* dst, void* fmt, ...) __asm__("func_002C26D0");
extern "C" void* func_002C2508(void* dst, void* src);
extern "C" void func_0023CC78(void* self, int n, char* dst);
extern "C" void* func_00147170(void* iface, int id);
int GetHashValue32(char* s);
extern void* D_004A28A8;
extern char D_0047C138[];
extern char D_0047C150[];
extern char D_0047C160[];

struct sLocVEntry_23CDB0 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

struct sMovieVEntry_23CDB0 {
    short delta;
    short index;
    void (*fn)(void*, char*);
};

extern "C" void cMCOverlayManager_setTitleString(void* self, int n)
{
    char out[0x200];
    char title[0x200];
    char fmt[0x200];
    char name[0x200];
    func_0023CC78(self, n, title);
    func_002C2540(fmt, D_0047C150);
    switch (*(int*)((char*)self + 0xB4)) {
    case 0: {
        char* loc = *(char**)((char*)D_004A28A8 + 0x8C);
        sLocVEntry_23CDB0* vt = *(sLocVEntry_23CDB0**)(loc + 4);
        char* thisp = loc + vt[4].delta;
        func_002C26D0_v(title, fmt, vt[4].fn(thisp, GetHashValue32(D_0047C160)));
        func_002C26D0_v(out, title, n);
        break;
    }
    case 1: {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        func_002C2540(name, func_00147170(iface, *(int*)((char*)self + 0xFC)));
        func_002C26D0_v(out, fmt, name);
        break;
    }
    default: {
        char* loc = *(char**)((char*)D_004A28A8 + 0x8C);
        sLocVEntry_23CDB0* vt = *(sLocVEntry_23CDB0**)(loc + 4);
        char* thisp = loc + vt[4].delta;
        func_002C2508(title, vt[4].fn(thisp, GetHashValue32(D_0047C138)));
        func_002C26D0_v(out, fmt, title);
        break;
    }
    }
    char* o = *(char**)((char*)self + 0x434);
    sMovieVEntry_23CDB0* mvt = *(sMovieVEntry_23CDB0**)o;
    mvt[45].fn(o + mvt[45].delta, out);
    func_002C2508((char*)self + 0x234, out);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023CF38);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D570);
#ifdef SKIP_ASM
extern "C" void* func_0023D570(void* self, int mode)
{
    char* s = (char*)self;
    *(int*)(s + 0xB8) = mode;
    switch (mode) {
    case 0:
        break;
    case 1:
        *(float*)(s + 0x110) = 1.0f;
        *(float*)(s + 0x114) = 0.016669999808073044f;
        *(int*)(s + 0x108) = 0x3C;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D5A8);
#ifdef SKIP_ASM
extern "C" void* func_002C22C8(void);

extern "C" void func_0023D5A8(void* self)
{
    char* s = (char*)self;
    void* q = func_002C22C8();
//START
    *(void**)(s + 0x434) = q;
    *(int*)(s + 0x104) = 0;
    *(float*)(s + 0x110) = 1.0f;
    *(float*)(s + 0x114) = 0.016669999808073044f;
//END
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D5E8);
#ifdef SKIP_ASM
extern "C" void func_002C2300(void*);

extern "C" void func_0023D5E8(void* self)
{
    func_002C2300(*(void**)((char*)self + 0x434));
    *(void**)((char*)self + 0x434) = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D618);
#ifdef SKIP_ASM
extern "C" void func_0023D7D8(void* self);
extern "C" void func_0023D660(void* self);

extern "C" void func_0023D618(void* self)
{
    switch (*(int*)((char*)self + 0xB8)) {
    case 0:
        func_0023D7D8(self);
        break;
    case 1:
        func_0023D660(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D660);
#ifdef SKIP_ASM
extern "C" int func_00241AC0(void* self);
extern "C" void func_0023E268(void* self);
extern "C" void func_0023E320(void* self);
extern "C" void func_0023E498(void* self);

struct sVoidVEntry_23D660 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sIntVEntry_23D660 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sArgVEntry_23D660 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline bool isState_23D660(char* s, int k)
{
    return *(int*)(s + 0x130) == k;
}

extern "C" void func_0023D660(void* self)
{
    char* s = (char*)self;
    char* o = *(char**)(s + 0x434);
    sVoidVEntry_23D660* vt = *(sVoidVEntry_23D660**)o;
    vt[3].fn(o + vt[3].delta);
    if (*(int*)(s + 0x338) != 0) {
        o = *(char**)(s + 0x434);
        sIntVEntry_23D660* ivt = *(sIntVEntry_23D660**)o;
        if (ivt[11].fn(o + ivt[11].delta) != 0) {
            return;
        }
        *(int*)(s + 0x338) = 0;
    }
    if (*(int*)(s + 0x424) == -1) {
        *(int*)(s + 0x424) = func_00241AC0(self);
    }
    o = *(char**)(s + 0x434);
    sIntVEntry_23D660* ivt = *(sIntVEntry_23D660**)o;
    if (ivt[11].fn(o + ivt[11].delta) == 0) {
        func_0023D570(self, 0);
        int ok = !(isState_23D660(s, 0x31) || isState_23D660(s, 0x32) || isState_23D660(s, 0x33) || isState_23D660(s, 0x34) || isState_23D660(s, 0x36));
        if (ok) {
            sArgVEntry_23D660* avt = *(sArgVEntry_23D660**)(s + 0x748);
            avt[1].fn(s + avt[1].delta, 3);
        }
    }
    int st = *(int*)(s + 0x130);
    if (st == 0x31) {
        func_0023E268(self);
    } else if (st == 0x33) {
        func_0023E498(self);
    } else if (st == 0x36) {
        func_0023E320(self);
    }
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023D7D8);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E268);
#ifdef SKIP_ASM
struct sMovieVEntryE268a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE268b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023E268(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryE268a* vt = *(sMovieVEntryE268a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        sMovieVEntryE268b* vt2 = *(sMovieVEntryE268b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x32);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E2C0__FPv);
#ifdef SKIP_ASM
void func_0023E2C0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E2C8);
#ifdef SKIP_ASM
struct sMovieVEntryE2C8a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE2C8b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023E2C8(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryE2C8a* vt = *(sMovieVEntryE2C8a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        sMovieVEntryE2C8b* vt2 = *(sMovieVEntryE2C8b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x33);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E320);
#ifdef SKIP_ASM
struct sMovieVEntryE320a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE320b {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sMovieVEntryE320c {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00241FD0(void* self);

extern "C" void func_0023E320(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryE320a* vt = *(sMovieVEntryE320a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        void* o2 = *(void**)((char*)self + 0x434);
        sMovieVEntryE320b* vtb = *(sMovieVEntryE320b**)o2;
        if (vtb[52].fn((char*)o2 + vtb[52].delta, *(int*)((char*)self + 0x428)) == 0) {
            sMovieVEntryE320c* vt2 = *(sMovieVEntryE320c**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 2);
            (*(void (**)())((char*)self + 0x9C))();
        } else {
            func_00241FD0(self);
            (*(void (**)())((char*)self + 0x98))();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E3C0);
#ifdef SKIP_ASM
struct sMovieVEntryE3C0a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE3C0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023E3C0(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryE3C0a* vt = *(sMovieVEntryE3C0a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            sMovieVEntryE3C0b* vt2 = *(sMovieVEntryE3C0b**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E428);
#ifdef SKIP_ASM
struct sMovieVEntryE428 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_00242050(void* self);

extern "C" void func_0023E428(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryE428* vt = *(sMovieVEntryE428**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 0);
            *(int*)((char*)self + 0xC8) = 0;
            func_00242050(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E498);
#ifdef SKIP_ASM
struct sMovieVEntryE498a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE498b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023E498(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryE498a* vt = *(sMovieVEntryE498a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        sMovieVEntryE498b* vt2 = *(sMovieVEntryE498b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x34);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E4F0__FPv);
#ifdef SKIP_ASM
void func_0023E4F0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E4F8);
#ifdef SKIP_ASM
struct sMovieVEntryE4F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023E4F8(void* self)
{
    if ((*(int*)((char*)self + 0x108))-- <= 0) {
        sMovieVEntryE4F8* vt = *(sMovieVEntryE4F8**)((char*)self + 0x748);
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023E540);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E820);
#ifdef SKIP_ASM
struct sMovieVEntryE820 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

extern "C" void func_0023E820(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryE820* vt = *(sMovieVEntryE820**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 0);
        } else if ((*(int (**)())((char*)self + 0x84))() != 0) {
            func_0023EA90(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023EA30);
#ifdef SKIP_ASM
extern int D_004A2C78;
extern "C" void func_00241180(void* self, int a1);

struct sMovieVEntryEA30 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023EA30(void* self, int v)
{
    char* s = (char*)self;
    int on = *(int*)(s + 0xBC);
    D_004A2C78 = 0;
    if (on != 0) {
        *(int*)(s + 0xBC) = 0;
        func_00241180(self, v);
    }
    *(int*)(s + 0xC0) = 0;
    *(int*)(s + 0x440) = 1;
    *(int*)(s + 0xC4) = 0;
    sMovieVEntryEA30* vt = *(sMovieVEntryEA30**)(s + 0x748);
    vt[1].fn(s + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023EA90);
#ifdef SKIP_ASM
extern "C" int func_0023C8D0(void* self);
extern "C" void func_0023FAE0(void* self);
void func_0023FB18(void* self, int val);
extern "C" void func_00241FD0(void* self);
extern "C" void func_00242050(void* self);
extern int D_004A2C78;

struct sMovieVEntryEA90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sMovieVEntryEA90b {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023EA90(void* self)
{
    char* s = (char*)self;
    if (*(int*)(s + 0xBC) != 0) {
        int playing = *(int*)(s + 0x118);
        D_004A2C78 = 0;
        if (playing != 0) {
            void* o = *(void**)(s + 0x434);
            sMovieVEntryEA90b* vt2 = *(sMovieVEntryEA90b**)o;
            vt2[44].fn((char*)o + vt2[44].delta);
        }
        sMovieVEntryEA90* vt = *(sMovieVEntryEA90**)(s + 0x748);
        vt[1].fn(s + vt[1].delta, 2);
    } else {
        *(int*)(s + 0x440) = 0;
        if (*(int*)(s + 0x12C) != 0) {
            func_0023FB18(self, 0);
            func_0023FAE0(self);
            sMovieVEntryEA90* vt = *(sMovieVEntryEA90**)(s + 0x748);
            vt[1].fn(s + vt[1].delta, 0x31);
        } else if (func_0023C8D0(self) == 0) {
            func_00241FD0(self);
        } else {
            func_00242050(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023EB50);
#ifdef SKIP_ASM
struct sMovieVEntry2 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023EB50(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 4);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023EB80);
#ifdef SKIP_ASM
struct sMovieVEntryEB80 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

extern "C" void func_0023EB80(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryEB80* vt = *(sMovieVEntryEB80**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 1);
        } else if ((*(int (**)())((char*)self + 0x84))() != 0) {
            func_0023EA90(self);
        }
    }
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023EC00);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023EF68);
#ifdef SKIP_ASM
class cMovieSub434 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual void v05(int);
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54();
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual int v64();
    virtual int v65();
    virtual int v66();
    virtual int v67();
    virtual int v68();
    virtual int v69();
    virtual int v70();
    virtual int v71();
    virtual int v72();
    virtual int v73();
    virtual int v74();
    virtual int v75();
    virtual int v76();
    virtual int v77();
    virtual int v78();
    virtual int v79();
    virtual int v80();
};
class cMoviePlayerVt {
public:
    char pad[0x748];
    virtual void v01(int);
};

extern "C" void func_0023EA30(void* self, int v);

extern "C" void func_0023EF68(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if (*(int*)((char*)self + 0x338) == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((*(cMovieSub434**)((char*)self + 0x434))->v09() != 0) {
            me->v01(0xC);
            return;
        }
        if (*(int*)((char*)self + 0xC4) != 0) {
            *(int*)((char*)self + 0xC4) = 0;
            me->v01(0xB);
            return;
        }
    }
    if ((*(int (**)())((char*)self + 0x6C))() != 0) {
        func_0023EA30(self, 0);
        return;
    }
    if ((*(int (**)())((char*)self + 0x88))() != 0) {
        *(int*)((char*)self + 0xC4) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F050);
#ifdef SKIP_ASM
extern "C" void func_0023F050(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    int t = (*(int*)((char*)self + 0x108))--;
    if (t <= 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
            if ((*(cMovieSub434**)((char*)self + 0x434))->v09() != 0) {
                me->v01(0xC);
            } else {
                me->v01(0xD);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F0F8);
#ifdef SKIP_ASM
extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023FB20(void* self, int v);

extern "C" void func_0023F0F8(void* self)
{
    if ((*(int (**)())((char*)self + 0x6C))() != 0) {
        func_0023EA30(self, 0);
        func_0023FB20(self, *(int*)((char*)self + 0x428));
        *(int*)((char*)self + 0x338) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F150);
#ifdef SKIP_ASM
struct sMovieVEntryF150 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

extern "C" void func_0023F150(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryF150* vt = *(sMovieVEntryF150**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 0);
        } else if ((*(int (**)())((char*)self + 0x84))() != 0) {
            *(int*)((char*)self + 0xBC) = 1;
            func_0023EA90(self);
            *(int*)((char*)self + 0x440) = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F1D8);
#ifdef SKIP_ASM
struct sMovieVEntryF1D8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

extern "C" void func_0023F1D8(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryF1D8* vt = *(sMovieVEntryF1D8**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 0);
        } else if ((*(int (**)())((char*)self + 0x84))() != 0) {
            func_0023EA90(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F258);
#ifdef SKIP_ASM
struct sMovieVEntryF258 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

extern "C" void func_0023F258(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryF258* vt = *(sMovieVEntryF258**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x84))() != 0) {
            func_0023EA90(self);
        } else if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F2D8);
#ifdef SKIP_ASM
extern "C" void func_0023F2D8(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F308);
#ifdef SKIP_ASM
struct sMovieVEntryF308a {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sMovieVEntryF308b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023F308(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryF308a* vt = *(sMovieVEntryF308a**)o;
    void* o2;
    sMovieVEntryF308a* vtb;
    if (vt[67].fn((char*)o + vt[67].delta, *(int*)((char*)self + 0xF8)) != 0
        || (o2 = *(void**)((char*)self + 0x434), vtb = *(sMovieVEntryF308a**)o2,
            vtb[65].fn((char*)o2 + vtb[65].delta, *(int*)((char*)self + 0xF8)) != 0)) {
        sMovieVEntryF308b* vt2 = *(sMovieVEntryF308b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0xE);
    } else {
        sMovieVEntryF308b* vt2 = *(sMovieVEntryF308b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0xF);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F3A0);
#ifdef SKIP_ASM
struct sMovieVEntryF3A0a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryF3A0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023F3A0(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryF3A0a* vt = *(sMovieVEntryF3A0a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        void* o2 = *(void**)((char*)self + 0x434);
        sMovieVEntryF3A0a* vtb = *(sMovieVEntryF3A0a**)o2;
        if (vtb[73].fn((char*)o2 + vtb[73].delta) != 0) {
            sMovieVEntryF3A0b* vt2 = *(sMovieVEntryF3A0b**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 0x14);
        } else {
            sMovieVEntryF3A0b* vt2 = *(sMovieVEntryF3A0b**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 0xF);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F438);
#ifdef SKIP_ASM
extern "C" void func_0023F438(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v73() != 0) {
            if ((*(cMovieSub434**)((char*)self + 0x434))->v74() != 0) {
                me->v01(0x37);
            } else {
                me->v01(0x14);
            }
        } else {
            me->v01(0x10);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F4F8);
#ifdef SKIP_ASM
struct sMovieVEntryF4F8a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryF4F8b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00241180(void* self, int a1);

extern "C" void func_0023F4F8(void* self)
{
    void* o;
    sMovieVEntryF4F8a* vt;
    *(float*)((char*)self + 0x10C) = (float)*(int*)((char*)self + 0xE4) / (float)*(int*)((char*)self + 0xF0);
    o = *(void**)((char*)self + 0x434);
    vt = *(sMovieVEntryF4F8a**)o;
    if (vt[73].fn((char*)o + vt[73].delta) != 0) {
        func_00241180(self, 1);
    } else {
        sMovieVEntryF4F8b* vt2 = *(sMovieVEntryF4F8b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x11);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F578);
#ifdef SKIP_ASM
extern "C" void func_0023CC20(void* self);
extern "C" void func_00241180(void* self, int a1);

class cMovieDecoder_23F578 {
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
    virtual int isPlaying();          // slot 42 (0x150)
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
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual int isFinished();         // slot 73 (0x248)
};
struct sMovieVEntry1_23F578 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023F578(char* self)
{
    *(int*)(self + 0x108) -= 1;
    *(float*)(self + 0x10C) = (float)*(int*)(self + 0xE4) / (float)*(int*)(self + 0xF0);
    if ((*(cMovieDecoder_23F578**)(self + 0x434))->isFinished() != 0) {
        func_00241180(self, 1);
        return;
    }
    if ((*(cMovieDecoder_23F578**)(self + 0x434))->isPlaying() == 1) {
        int a = *(int*)(self + 0xE4);
        int b = *(int*)(self + 0xF0);
        if (a < b) {
            if ((*(int (**)(int))(self + 4))(b - a) == 0) {
                func_00241180(self, 1);
            }
        } else {
            func_0023CC20(self);
            (*(void (**)())(self + 8))();
            sMovieVEntry1_23F578* vt1 = *(sMovieVEntry1_23F578**)(self + 0x748);
            vt1[1].fn(self + vt1[1].delta, 0x12);
        }
    } else {
        if ((*(cMovieDecoder_23F578**)(self + 0x434))->isFinished() != 0) {
            func_00241180(self, 1);
        }
    }
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023F698);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FAE0);
#ifdef SKIP_ASM
extern "C" void func_0023FB20(void* self, int v);

extern "C" void func_0023FAE0(void* self)
{
    int cur = *(int*)((char*)self + 0x428);
    if (*(int*)((char*)self + 0x444) != cur) {
        *(int*)((char*)self + 0x444) = cur;
        *(int*)((char*)self + 0x43C) = 0;
        *(int*)((char*)self + 0x440) = 0;
    }
    func_0023FB20(self, *(int*)((char*)self + 0x428));
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FB18__FPvi);
#ifdef SKIP_ASM
void func_0023FB18(void* self, int val)
{
    *(int*)((char*)self + 0x428) = val;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FB20);
#ifdef SKIP_ASM
extern "C" void func_0023FB20(void* self, int v)
{
    *(int*)((char*)self + 0x42C) = v;
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry2* vt = *(sMovieVEntry2**)obj;
    vt[5].fn((char*)obj + vt[5].delta, v);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FB58);
#ifdef SKIP_ASM
struct sMovieVEntryFB58 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void* func_0023D570(void* self, int v);

extern "C" void* func_0023FB58(void* self)
{
    *(int*)((char*)self + 0x430) = -1;
    *(int*)((char*)self + 0x42C) = 0;
    *(int*)((char*)self + 0xBC) = 0;
    *(int*)((char*)self + 0xC0) = 0;
    *(int*)((char*)self + 0xC4) = 0;
    sMovieVEntryFB58* vt = *(sMovieVEntryFB58**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 0);
    return func_0023D570(self, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FBB8);
#ifdef SKIP_ASM
extern "C" void func_0023FBB8(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    (*(int*)((char*)self + 0x108))--;
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v73() != 0) {
            me->v01(0x14);
            return;
        }
        if (*(int*)((char*)self + 0x108) <= 0) {
            (*(void (**)())((char*)self + 0xC))();
            me->v01(0x13);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FC70);
#ifdef SKIP_ASM
struct sMovieVEntryFC70a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryFC70b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023FC70(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryFC70a* vt = *(sMovieVEntryFC70a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            sMovieVEntryFC70b* vt2 = *(sMovieVEntryFC70b**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 3);
        } else if ((*(int (**)())((char*)self + 0x84))() != 0) {
            sMovieVEntryFC70b* vt2 = *(sMovieVEntryFC70b**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 6);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FD08);
#ifdef SKIP_ASM
extern "C" void func_0023EA30(void* self, int v);

extern "C" void func_0023FD08(void* self)
{
    func_0023EA30(self, 1);
    (*(void (**)())((char*)self + 0x38))();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FD40);
#ifdef SKIP_ASM
extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

class cMovieDecoder_23FD40 {
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
    virtual int isBusy();                      // slot 11 (0x58)
    virtual void v12();
    virtual void v13();
    virtual void play(int a, void* b);          // slot 14 (0x70)
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
    virtual int isReady();                     // slot 53 (0x1A8)
};

struct sMovieVEntry_23FD40 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void setState_23FD40(char* self, int s)
{
    sMovieVEntry_23FD40* vt = *(sMovieVEntry_23FD40**)(self + 0x748);
    vt[1].fn(self + vt[1].delta, s);
}

extern "C" void func_0023FD40(char* self)
{
    if ((*(cMovieDecoder_23FD40**)(self + 0x434))->isBusy() != 0) {
        return;
    }
    if (*(int*)(self + 0x338) == 0 && (*(cMovieDecoder_23FD40**)(self + 0x434))->isReady() == 0) {
        setState_23FD40(self, 4);
        return;
    }
    if ((*(int (**)())(self + 0x6C))() != 0) {
        func_0023EA30(self, 0);
        return;
    }
    if ((*(int (**)())(self + 0x84))() != 0) {
        func_0023EA90(self);
        return;
    }
    if ((*(int (**)())(self + 0x68))() != 0) {
        (*(cMovieDecoder_23FD40**)(self + 0x434))->play(*(int*)(self + 0xF8), self + 0x234);
        setState_23FD40(self, 0x19);
        return;
    }
    if ((*(int (**)())(self + 0x70))() != 0) {
        setState_23FD40(self, 0xD);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FE70);
#ifdef SKIP_ASM
struct sMovieVEntryFE70 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

extern "C" void func_0023FE70(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryFE70* vt = *(sMovieVEntryFE70**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 1);
        } else if ((*(int (**)())((char*)self + 0x84))() != 0) {
            func_0023EA90(self);
        } else {
            (*(void (**)())((char*)self + 0x68))();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FF00);
#ifdef SKIP_ASM
extern "C" void func_0023FF00(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    int n = *(int*)((char*)self + 0x108);
    if (n > 0) {
        *(int*)((char*)self + 0x108) = n - 1;
        return;
    }
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v73() != 0) {
            me->v01(0x24);
            (*(void (**)())((char*)self + 0x2C))();
        } else {
            me->v01(0x23);
            (*(void (**)())((char*)self + 0x28))();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FFC0);
#ifdef SKIP_ASM
extern "C" void func_0023FFC0(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FFF0);
#ifdef SKIP_ASM
extern "C" void func_0023FFF0(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((*(int (**)())((char*)self + 0x78))() != 0) {
            me->v01(0x1F);
            return;
        }
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            me->v01(0x1D);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002400B0);
#ifdef SKIP_ASM
struct sMovieVEntry00B0a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry00B0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_002400B0(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntry00B0a* vt = *(sMovieVEntry00B0a**)o;
    if (vt[73].fn((char*)o + vt[73].delta) != 0) {
        sMovieVEntry00B0b* vt2 = *(sMovieVEntry00B0b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x24);
        (*(void (**)())((char*)self + 0x2C))();
    } else {
        sMovieVEntry00B0b* vt2 = *(sMovieVEntry00B0b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240130);
#ifdef SKIP_ASM
extern "C" void func_0023CA70(void* self);
extern "C" void func_00241240(void* self);

class cMovieSub434_0130 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual void v40(int);
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54();
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual int v64();
    virtual int v65();
    virtual int v66();
    virtual int v67();
    virtual int v68();
    virtual int v69();
    virtual int v70();
    virtual int v71();
    virtual int v72();
    virtual int v73();
};

extern "C" void func_00240130(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    *(int*)((char*)self + 0x108) -= 1;
    int n = (*(cMovieSub434_0130**)((char*)self + 0x434))->v18();
    *(float*)((char*)self + 0x10C) = (float)n / (float)*(int*)((char*)self + 0xF0);
    if ((*(cMovieSub434_0130**)((char*)self + 0x434))->v73() != 0) {
        me->v01(0x24);
        (*(void (**)())((char*)self + 0x2C))();
        return;
    }
    if ((*(cMovieSub434_0130**)((char*)self + 0x434))->v31() == 1) {
        int a = *(int*)((char*)self + 0xE8);
        int b = *(int*)((char*)self + 0xF0);
        if (a < b) {
            if ((*(int (**)(int))((char*)self + 0x1C))(b - a) == 0)
                func_00241240(self);
        } else if ((*(int (**)())((char*)self + 0x20))() == 0) {
            (*(cMovieSub434_0130**)((char*)self + 0x434))->v40(*(int*)((char*)self + 0xF8));
            func_00241240(self);
        } else {
            func_0023CA70(self);
            me->v01(0x21);
        }
        return;
    }
    if ((*(cMovieSub434_0130**)((char*)self + 0x434))->v73() != 0) {
        (*(void (**)())((char*)self + 0x2C))();
        me->v01(0x24);
    }
}
#endif

INCLUDE_ASM("movie/movieplayer", func_002402D0);

//100%
INCLUDE_ASM("movie/movieplayer", func_002405D0);
#ifdef SKIP_ASM
extern "C" void func_002405D0(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    (*(int*)((char*)self + 0x108))--;
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v73() != 0) {
            me->v01(0x24);
            (*(void (**)())((char*)self + 0x2C))();
            return;
        }
        if (*(int*)((char*)self + 0x108) <= 0) {
            me->v01(0x22);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240688);
#ifdef SKIP_ASM
extern "C" void func_0023EA30(void* self, int v);

extern "C" void func_00240688(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((*(cMovieSub434**)((char*)self + 0x434))->v73() != 0) {
            me->v01(0x1C);
            return;
        }
        if ((*(int (**)())((char*)self + 0x68))() != 0) {
            me->v01(0x19);
            return;
        }
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240768);
#ifdef SKIP_ASM
struct sMovieVEntry0768a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry0768b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00240768(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntry0768a* vt = *(sMovieVEntry0768a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        void* o2 = *(void**)((char*)self + 0x434);
        sMovieVEntry0768a* vtb = *(sMovieVEntry0768a**)o2;
        if (vtb[73].fn((char*)o2 + vtb[73].delta) != 0) {
            sMovieVEntry0768b* vt2 = *(sMovieVEntry0768b**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 0x1C);
        } else {
            sMovieVEntry0768b* vt2 = *(sMovieVEntry0768b**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 0x1A);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240800);
#ifdef SKIP_ASM
struct sMovieVEntry0800 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);

extern "C" void func_00240800(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntry0800* vt = *(sMovieVEntry0800**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240860);
#ifdef SKIP_ASM
class cMovieDecoder_240860 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void setVolume(int v);  // slot 5 (0x28)
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual int isBusy();  // slot 11 (0x58)
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
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual int isFinished();  // slot 73 (0x248)
};

struct sMovieVEntry_240860 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void setState_240860(char* self, int s)
{
    sMovieVEntry_240860* vt = *(sMovieVEntry_240860**)(self + 0x748);
    vt[1].fn(self + vt[1].delta, s);
}

extern "C" void func_00240860(char* self)
{
    cMovieDecoder_240860* d = *(cMovieDecoder_240860**)(self + 0x434);
    if (*(int*)(self + 0x108) > 0) {
        if (d->isBusy() == 0) {
            if ((*(cMovieDecoder_240860**)(self + 0x434))->isFinished() != 0) {
                setState_240860(self, 0x1C);
                return;
            }
            (*(cMovieDecoder_240860**)(self + 0x434))->setVolume(*(int*)(self + 0x428));
        }
        *(int*)(self + 0x108) -= 1;
    } else {
        if (d->isBusy() != 0) {
            return;
        }
        if ((*(cMovieDecoder_240860**)(self + 0x434))->isFinished() != 0) {
            setState_240860(self, 0x1C);
        } else {
            setState_240860(self, 0x1B);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240960);
#ifdef SKIP_ASM
struct sMovieVEntry0960a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry0960b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00240960(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntry0960a* vt = *(sMovieVEntry0960a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x30))() != 0) {
            sMovieVEntry0960b* vt2 = *(sMovieVEntry0960b**)((char*)self + 0x748);
            vt2[1].fn((char*)self + vt2[1].delta, 6);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002409C8);
#ifdef SKIP_ASM
extern "C" void func_002409C8(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((*(cMovieSub434**)((char*)self + 0x434))->v73() != 0) {
            me->v01(0x14);
            return;
        }
        if ((*(int (**)())((char*)self + 0x70))() != 0) {
            me->v01(0xD);
            return;
        }
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            me->v01(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240AB0);
#ifdef SKIP_ASM
extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

extern "C" void func_00240AB0(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((*(int (**)())((char*)self + 0x88))() != 0) {
            me->v01(0x28);
            return;
        }
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 1);
            return;
        }
        if ((*(int (**)())((char*)self + 0x84))() != 0) {
            func_0023EA90(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240B88);
#ifdef SKIP_ASM
extern "C" void func_00240B88(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    int t = (*(int*)((char*)self + 0x108))--;
    if (t <= 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
            if ((*(cMovieSub434**)((char*)self + 0x434))->v09() != 0) {
                me->v01(0x29);
            } else {
                me->v01(0x2A);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240C30);
#ifdef SKIP_ASM
struct sMovieVEntry0C30 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023FB20(void* self, int v);

extern "C" void func_00240C30(void* self)
{
    if ((*(int (**)())((char*)self + 0x6C))() != 0) {
        sMovieVEntry0C30* vt = *(sMovieVEntry0C30**)((char*)self + 0x748);
        vt[1].fn((char*)self + vt[1].delta, 1);
        func_0023FB20(self, *(int*)((char*)self + 0x428));
        *(int*)((char*)self + 0x338) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240C90);
#ifdef SKIP_ASM
extern "C" void func_00240C90(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 6);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240CC0);
#ifdef SKIP_ASM
extern "C" void func_00240CC0(void* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if ((*(cMovieSub434**)((char*)self + 0x434))->v11() == 0) {
        if ((*(cMovieSub434**)((char*)self + 0x434))->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((*(int (**)())((char*)self + 0x68))() != 0) {
            me->v01(0x2C);
            return;
        }
        if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            me->v01(6);
            return;
        }
        (*(cMovieSub434**)((char*)self + 0x434))->v05(*(int*)((char*)self + 0x428));
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240D90);
#ifdef SKIP_ASM
class cMovieDecoder_240D90 {
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
    virtual void seek(int i);  // slot 10 (0x50)
    virtual int isBusy();  // slot 11 (0x58)
    virtual void v12();
    virtual int getCount();  // slot 13 (0x68)
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
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual int canSeek(int i);  // slot 65 (0x208)
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual int isFinished();  // slot 73 (0x248)
};

struct sMovieVEntry_240D90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void setState_240D90(char* self, int s)
{
    sMovieVEntry_240D90* vt = *(sMovieVEntry_240D90**)(self + 0x748);
    vt[1].fn(self + vt[1].delta, s);
}

extern "C" void func_00240D90(char* self)
{
    *(int*)(self + 0x108) -= 1;
    if ((*(cMovieDecoder_240D90**)(self + 0x434))->isBusy() != 0) {
        return;
    }
    if (*(int*)(self + 0x108) > 0) {
        return;
    }
    if ((*(cMovieDecoder_240D90**)(self + 0x434))->isFinished() != 0) {
        setState_240D90(self, 0x2D);
        return;
    }
    if (*(int*)(self + 0x438) < (*(cMovieDecoder_240D90**)(self + 0x434))->getCount()) {
        if ((*(cMovieDecoder_240D90**)(self + 0x434))->canSeek(*(int*)(self + 0x438)) != 0) {
            (*(cMovieDecoder_240D90**)(self + 0x434))->seek(*(int*)(self + 0x438));
        }
        *(int*)(self + 0x438) += 1;
    } else {
        setState_240D90(self, 0x2E);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240EB0);
#ifdef SKIP_ASM
extern "C" void func_00240EB0(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240EE0);
#ifdef SKIP_ASM
extern "C" void func_00240EE0(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 6);
}
#endif

extern "C" void* func_0023FB58(void* self);

//99.29%
INCLUDE_ASM("movie/movieplayer", func_00240F10__FPv);
#ifdef SKIP_ASM
void* func_00240F10(void* self)
{
    return func_0023FB58(self);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240F30);
#ifdef SKIP_ASM
struct sMovieVEntry0F30 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_0023EA30(void* self, int v);
extern "C" void func_0023EA90(void* self);

extern "C" void func_00240F30(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntry0F30* vt = *(sMovieVEntry0F30**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        if ((*(int (**)())((char*)self + 0x84))() != 0) {
            func_0023EA90(self);
        } else if ((*(int (**)())((char*)self + 0x6C))() != 0) {
            func_0023EA30(self, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241000);
#ifdef SKIP_ASM
extern int D_004A2C78;

struct sMovieVEntry1000 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

extern "C" void func_00241000(void* self, int a, int b, int c)
{
    void* o = *(void**)((char*)self + 0x434);
    D_004A2C78 = 1;
    sMovieVEntry1000* vt = *(sMovieVEntry1000**)o;
    vt[37].fn((char*)o + vt[37].delta, a, b, c);
    *(int*)((char*)self + 0xE4) = c;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002410A0);
#ifdef SKIP_ASM
extern "C" void func_002410A0(void* self)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry* vt = *(sMovieVEntry**)obj;
    vt[12].fn((char*)obj + vt[12].delta);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241138);
#ifdef SKIP_ASM
extern int D_004A2C78;

struct sMovieVEntry1138 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern "C" void func_00241138(void* self, int a, int b)
{
    void* o = *(void**)((char*)self + 0x434);
    int one = 1;
    *(int*)((char*)self + 0xE4) += b;
    D_004A2C78 = one;
    sMovieVEntry1138* vt = *(sMovieVEntry1138**)o;
    vt[41].fn((char*)o + vt[41].delta, a, b);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241180);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A2C78;
extern "C" void func_0023D570_v(void* self, int v) __asm__("func_0023D570");

struct sMovieVEntry1180a {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sMovieVEntry1180b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00241180(void* self, int a1)
{
    int on = *(int*)((char*)self + 0x118);
    D_004A2C78 = 0;
    if (on != 0) {
        void* o = *(void**)((char*)self + 0x434);
        sMovieVEntry1180a* vt = *(sMovieVEntry1180a**)o;
        vt[44].fn((char*)o + vt[44].delta);
    }
    if (a1 != 0) {
        func_0023D570_v(self, 0);
        sMovieVEntry1180b* vt2 = *(sMovieVEntry1180b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x14);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241200);
#ifdef SKIP_ASM
struct sMovieVEntry3 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern "C" void func_00241200(void* self, int a, int n)
{
    *(int*)((char*)self + 0xE8) += n;
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry3* vt = *(sMovieVEntry3**)obj;
    vt[30].fn((char*)obj + vt[30].delta, a, n);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241240);
#ifdef SKIP_ASM
struct sMovieVEntry1240a {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sMovieVEntry1240b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00241240(void* self)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntry1240a* vt = *(sMovieVEntry1240a**)o;
    vt[33].fn((char*)o + vt[33].delta);
    sMovieVEntry1240b* vt2 = *(sMovieVEntry1240b**)((char*)self + 0x748);
    vt2[1].fn((char*)self + vt2[1].delta, 0x24);
    (*(void (**)())((char*)self + 0x2C))();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002412A0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00152758(void* iface);
// PORT: func_00152948__FPv ignores its argument; this caller passes none.
int func_00152948_r() __asm__("func_00152948__FPv");
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_00241D40_r(void* self, int mode, void* buf, int size) __asm__("func_00241D40");
extern "C" void func_00241AA0_r(void* buf, int size) __asm__("func_00241AA0");
extern "C" void func_002C26D0_v(void* dst, void* fmt, ...) __asm__("func_002C26D0");
extern "C" void func_00241000_r(void* self, char* text, int a, int b) __asm__("func_00241000");
extern int D_004A2C78;
extern char D_0047C178[];

extern "C" void func_002412A0(void* self)
{
    char text[0x82];
    char name[0x82];
    char desc[0x200];
    char fmt[0x200];
    D_004A2C78 = 1;
    int a = func_00152758(cBE_getInterface_Fv(cBE_getBE(), 5));
    int b = func_00152948_r();
    func_002C2540(fmt, D_0047C178);
    func_00241D40_r(self, 2, name, 0x82);
    func_00241AA0_r(desc, 0x200);
    func_002C26D0_v(text, fmt, desc, name, *(int*)((char*)self + 0xF8) + 1);
    func_00241000_r(self, text, a, b);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241380);
#ifdef SKIP_ASM
struct sMovieVEntry1380 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00241380(void* self)
{
    int one = 1;
    void* o;
    sMovieVEntry1380* vt;
    *(int*)((char*)self + 0x340) = one;
    *(int*)((char*)self + 0xDC) = 0;
    *(int*)((char*)self + 0x42C) = 0;
    *(int*)((char*)self + 0xF8) = 0;
    *(int*)((char*)self + 0x430) = -1;
    *(int*)((char*)self + 0xFC) = -1;
    *(int*)((char*)self + 0x444) = -1;
    func_0023FB18(self, 0);
    func_0023FAE0(self);
    *(int*)((char*)self + 0x338) = one;
    *(int*)((char*)self + 0x130) = 2;
    o = *(void**)((char*)self + 0x434);
    vt = *(sMovieVEntry1380**)o;
    vt[3].fn((char*)o + vt[3].delta);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241400);
#ifdef SKIP_ASM
class cMovieDecoder_241400 {
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
    virtual int hasChapter(int i);  // slot 52 (0x1A0)
};

extern "C" void func_0023FAE0(void* self);
void func_0023FB18(void* self, int val);

static inline int nextChapter_241400(char* self, int i)
{
    if (i < *(int*)(self + 0x424) - 1) {
        return i + 1;
    }
    if (*(int*)(self + 0x100) != 0) {
        return 0;
    }
    return i;
}

extern "C" void func_00241400(char* self)
{
    int i = *(int*)(self + 0x428);
    if (*(int*)(self + 0x424) == -1) {
        return;
    }
    while (i = nextChapter_241400(self, i), (*(cMovieDecoder_241400**)(self + 0x434))->hasChapter(i) == 0) {
        if (i == *(int*)(self + 0x424) - 1 || *(int*)(self + 0x100) != 0) {
            if (i == *(int*)(self + 0x428)) {
                break;
            }
            if (*(int*)(self + 0x100) != 1) {
                break;
            }
        }
    }
    if (*(int*)(self + 0x100) == 0 && i == *(int*)(self + 0x424) - 1 && (*(cMovieDecoder_241400**)(self + 0x434))->hasChapter(i) == 0) {
        i = *(int*)(self + 0x428);
        if ((*(cMovieDecoder_241400**)(self + 0x434))->hasChapter(i) == 0) {
            i = 0;
        }
    }
    if (*(int*)(self + 0x428) != i) {
        func_0023FB18(self, i);
        func_0023FAE0(self);
    }
    *(int*)(self + 0xE0) = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241540);
#ifdef SKIP_ASM
class cMovieDecoder_241540 {
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
    virtual int hasChapter(int i);  // slot 52 (0x1A0)
};

extern "C" void func_0023FAE0(void* self);
void func_0023FB18(void* self, int val);

static inline int prevChapter_241540(char* self, int i)
{
    if (i > 0) {
        return i - 1;
    }
    if (*(int*)(self + 0x100) != 0) {
        return *(int*)(self + 0x424) - 1;
    }
    return i;
}

extern "C" void func_00241540(char* self)
{
    int i = *(int*)(self + 0x428);
    if (*(int*)(self + 0x424) == -1) {
        return;
    }
    while (i = prevChapter_241540(self, i), (*(cMovieDecoder_241540**)(self + 0x434))->hasChapter(i) == 0) {
        if (i == 0 || *(int*)(self + 0x100) != 0) {
            if (i == *(int*)(self + 0x428)) {
                break;
            }
            if (*(int*)(self + 0x100) != 1) {
                break;
            }
        }
    }
    if (*(int*)(self + 0x100) == 0 && i == 0 && (*(cMovieDecoder_241540**)(self + 0x434))->hasChapter(0) == 0) {
        i = *(int*)(self + 0x428);
        if ((*(cMovieDecoder_241540**)(self + 0x434))->hasChapter(i) == 0) {
            i = 0;
        }
    }
    if (*(int*)(self + 0x428) != i) {
        func_0023FB18(self, i);
        func_0023FAE0(self);
    }
    *(int*)(self + 0xE0) = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceDisplayString);
#ifdef SKIP_ASM
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_002C26D0_v(void* dst, void* fmt, ...) __asm__("func_002C26D0");
extern "C" void* func_002C2508(void* dst, void* src);
extern "C" int func_002C24D0(void* s);
extern "C" void func_00241DC8(void* self, void* dst, int n);
int GetHashValue32(char* s);
extern void* D_004A28A8;
extern char D_0047C360[];
extern char D_0047C378[];
extern char D_0047C390[];

class cMovieSub434_1710 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48(int);
    virtual int v49();
    virtual int v50(int);
    virtual int v51();
    virtual int v52(int);
    virtual int v53();
    virtual int v54();
    virtual int v55();
    virtual int v56(int);
    virtual int v57(int);
    virtual int v58();
    virtual int v59();
    virtual int v60(int);
};

struct sLocVEntry_241710 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

struct sMCDev_241710 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    char name[0xE0 - 0x18];
};

struct sMCOverlayMgr_241710 {
    char pad[0x344];
    sMCDev_241710 dev[1];
};

static inline void* Loc_241710(char* key)
{
    char* loc = *(char**)((char*)D_004A28A8 + 0x8C);
    sLocVEntry_241710* vt = *(sLocVEntry_241710**)(loc + 4);
    char* thisp = loc + vt[4].delta;
    return vt[4].fn(thisp, GetHashValue32(key));
}

extern "C" void cMCOverlayManager_GetDeviceDisplayString(void* self, int idx, char* dst)
{
    char buf[0x200];
    if ((*(cMovieSub434_1710**)((char*)self + 0x434))->v52(idx))
    {
        if ((*(cMovieSub434_1710**)((char*)self + 0x434))->v48(idx) || (*(cMovieSub434_1710**)((char*)self + 0x434))->v50(idx) || (*(cMovieSub434_1710**)((char*)self + 0x434))->v60(idx))
        {
            func_00241DC8(self, buf, idx);
            func_002C26D0_v(dst, Loc_241710(D_0047C360), buf);
        }
        else if (!(*(cMovieSub434_1710**)((char*)self + 0x434))->v56(idx))
        {
            func_00241DC8(self, buf, idx);
            func_002C26D0_v(dst, Loc_241710(D_0047C378), buf);
        }
        else if ((*(cMovieSub434_1710**)((char*)self + 0x434))->v57(idx))
        {
            func_00241DC8(self, buf, idx);
            char* name = ((sMCOverlayMgr_241710*)self)->dev[idx].name;
            if (func_002C24D0(name) == 0)
                func_002C26D0_v(dst, Loc_241710(D_0047C390), buf);
            else
                func_002C2508(dst, name);
        }
    }
    else
    {
        func_00241DC8(self, buf, idx);
        func_002C26D0_v(dst, Loc_241710(D_0047C360), buf);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002419D8);
#ifdef SKIP_ASM
extern "C" int func_002419D8(void* self, int a1)
{
    char* p = (char*)self + a1 * 0xe0;
    return *(int*)(p + 0x354);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceTotalString);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" unsigned short* func_002C2540_r(void*, void*) __asm__("func_002C2540");
extern "C" void func_002C26D0_v(void* dst, void* fmt, ...) __asm__("func_002C26D0");
extern char D_004A2CA0[];
extern char D_004A2C98[];

struct sMCDev {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    char pad[0xE0 - 0x18];
};
struct sMCOverlayMgr {
    char pad[0x344];
    sMCDev dev[1];
};

extern "C" void cMCOverlayManager_GetDeviceTotalString(sMCOverlayMgr* self, int idx, void* dst)
{
    unsigned short str[256];
    unsigned short fmt[256];
    if (self->dev[idx].fC == 0) return;
    if (self->dev[idx].f0 && self->dev[idx].f4 && self->dev[idx].f8) {
        func_002C2540_r(str, D_004A2CA0);
        func_002C2540_r(fmt, D_004A2C98);
        func_002C26D0_v(dst, fmt, self->dev[idx].f14, str);
    }
}
#endif

struct sPad16 { char x; int pad[3]; };
extern sPad16 D_0047C3A8;
extern "C" void func_002C2540(void*, void*);

//100%
INCLUDE_ASM("movie/movieplayer", func_00241AA0);
#ifdef SKIP_ASM
extern "C" void func_00241AA0(void* self)
{
    func_002C2540(self, &D_0047C3A8);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241AC0);
#ifdef SKIP_ASM
struct sMovieVEntry1AC0 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_00241AC0(void* self)
{
    if (*(int*)((char*)self + 0x424) < 0) {
        void* o = *(void**)((char*)self + 0x434);
        sMovieVEntry1AC0* vt = *(sMovieVEntry1AC0**)o;
        int n = vt[69].fn((char*)o + vt[69].delta);
        *(int*)((char*)self + 0x424) = n;
        if (n >= 2) {
            *(int*)((char*)self + 0x424) = 1;
        }
    }
    return *(int*)((char*)self + 0x424);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241B20);
#ifdef SKIP_ASM
class cMovieSub434_1B20 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual bool v53();
    virtual int v54();
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
};

struct sMovieFlags_1B20
{
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int d : 1;
    unsigned int e : 1;
    unsigned int f : 4;
    unsigned int rest : 23;
};

// PORT: the unit declares func_00241B20 as returning int; it really returns this 4-byte flags struct in $v0.
extern "C" sMovieFlags_1B20 func_00241B20_s(void* self) __asm__("func_00241B20");

extern "C" sMovieFlags_1B20 func_00241B20_s(void* self)
{
    if (*(int*)((char*)self + 0x42C) != *(int*)((char*)self + 0x428))
        return *(sMovieFlags_1B20*)((char*)self + 0x43C);
    sMovieFlags_1B20 s = {0, 0, 0, 0, 0, 0, 0};
    s.a = (*(cMovieSub434_1B20**)((char*)self + 0x434))->v49();
    int b = (*(cMovieSub434_1B20**)((char*)self + 0x434))->v51() || (*(cMovieSub434_1B20**)((char*)self + 0x434))->v59();
    s.b = b;
    int c = !(*(cMovieSub434_1B20**)((char*)self + 0x434))->v13() && (*(cMovieSub434_1B20**)((char*)self + 0x434))->v20() < (*(cMovieSub434_1B20**)((char*)self + 0x434))->v23();
    s.c = c;
    s.d = !(*(cMovieSub434_1B20**)((char*)self + 0x434))->v53();
    int e = !(*(cMovieSub434_1B20**)((char*)self + 0x434))->v55() || !(*(cMovieSub434_1B20**)((char*)self + 0x434))->v58();
    s.e = e;
    s.f = 0;
    return s;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("movie/movieplayer", func_00241CD8);
#ifdef SKIP_ASM
extern "C" int func_00241B20(void* self);

extern "C" int func_00241CD8(void* self)
{
    int v = func_00241B20(self);
    int changed = *(int*)((char*)self + 0x43C) != v;
    if (*(int*)((char*)self + 0x440) == 0 || changed) {
        *(int*)((char*)self + 0x43C) = v;
        if (changed) {
            *(int*)((char*)self + 0x440) = (*(int (**)(int))((char*)self + 0xB0))(v);
        }
        return *(int*)((char*)self + 0x440) ^ 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241D40);
#ifdef SKIP_ASM
extern "C" void func_002C2540(void*, void*);
extern char D_004A2CA8[];
extern char D_004A2CB0[];
extern char D_004A2CB8[];

extern "C" void func_00241D40(void* self, int mode, void* buf)
{
    switch (mode) {
    case 0:
        func_002C2540(buf, D_004A2CA8);
        break;
    case 1:
        func_002C2540(buf, D_004A2CB0);
        break;
    case 2:
        func_002C2540(buf, D_004A2CB8);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241DC8);
#ifdef SKIP_ASM
extern "C" void func_002C26D0(void* dst, void* src, int n);
extern char D_004A2C28[];

extern "C" void func_00241DC8(void* self, void* dst, int n)
{
    unsigned short buf[256];
    func_002C2540(buf, D_004A2C28);
    func_002C26D0(dst, buf, n + 1);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00241E18);

extern "C" void* func_002420C8(void*, int);

//100%
INCLUDE_ASM("movie/movieplayer", func_00241FB0__FPv);
#ifdef SKIP_ASM
void* func_00241FB0(void* self)
{
    return func_002420C8(self, *(int*)((char*)self + 0x42c));
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241FD0);
#ifdef SKIP_ASM
struct sMovieVEntry1FD0a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry1FD0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00241FD0(void* self)
{
    void* o;
    sMovieVEntry1FD0a* vt;
    (*(void (**)())((char*)self + 0xAC))();
    o = *(void**)((char*)self + 0x434);
    vt = *(sMovieVEntry1FD0a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        sMovieVEntry1FD0b* vt2 = *(sMovieVEntry1FD0b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x33);
    } else {
        sMovieVEntry1FD0b* vt2 = *(sMovieVEntry1FD0b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 0x35);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242050);
#ifdef SKIP_ASM
struct sMovieVEntry2050a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry2050b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00242050(void* self)
{
    void* o;
    sMovieVEntry2050a* vt;
    *(int*)((char*)self + 0x338) = 0;
    o = *(void**)((char*)self + 0x434);
    vt = *(sMovieVEntry2050a**)o;
    if (vt[11].fn((char*)o + vt[11].delta) == 0) {
        sMovieVEntry2050b* vt2 = *(sMovieVEntry2050b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 2);
    } else {
        sMovieVEntry2050b* vt2 = *(sMovieVEntry2050b**)((char*)self + 0x748);
        vt2[1].fn((char*)self + vt2[1].delta, 3);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002420C8);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* p, int c, int n);

class cMovieSub434_20C8 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48(int);
    virtual int v49();
    virtual int v50(int);
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54();
    virtual int v55();
    virtual int v56(int);
    virtual int v57(int);
    virtual int v58();
    virtual int v59();
    virtual int v60(int);
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual int v64();
    virtual int v65();
    virtual int v66();
    virtual int v67();
    virtual int v68();
    virtual int v69();
    virtual void v70(int, char*, int);
};

struct sSlot_20C8
{
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    char name[200];
};

struct sMP_20C8
{
    char pad0[0x344];
    sSlot_20C8 slots[1];
    int f424;
    int f428;
    int cur;
};

// PORT: the unit declares func_002420C8 as returning void*, but it returns nothing; bind the void body by asm label.
extern "C" void func_002420C8_v(void* self, int i) __asm__("func_002420C8");

extern "C" void func_002420C8_v(void* self, int i)
{
    sMP_20C8* me = (sMP_20C8*)self;
    me->slots[i].a = 1;
    char* bb = (char*)self + 0x348;
    int* pb = (int*)(bb + i * 0xE0);
    char* bc = (char*)self + 0x34C;
    int* pc = (int*)(bc + i * 0xE0);
    *pb = 1;
    *pc = 1;
    char* name = me->slots[i].name;
    me->slots[i].d = 1;
    func_003E6448(name, 0, 200);
    if ((*(cMovieSub434_20C8**)((char*)self + 0x434))->v48(i) || (*(cMovieSub434_20C8**)((char*)self + 0x434))->v50(i) || (*(cMovieSub434_20C8**)((char*)self + 0x434))->v60(i))
    {
        me->slots[i].a = 0;
        return;
    }
    if (!(*(cMovieSub434_20C8**)((char*)self + 0x434))->v56(i))
    {
        *pb = 0;
        return;
    }
    if (!(*(cMovieSub434_20C8**)((char*)self + 0x434))->v57(i))
    {
        *pc = 0;
        return;
    }
    if (i == me->cur)
        me->slots[me->cur].e = (*(cMovieSub434_20C8**)((char*)self + 0x434))->v20();
    (*(cMovieSub434_20C8**)((char*)self + 0x434))->v70(i, name, 100);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00242288);

//100%
INCLUDE_ASM("movie/movieplayer", func_002424C8);
#ifdef SKIP_ASM
extern int D_004A2C78;
extern void* D_0047CBD8[];
void operator_delete(int*);

extern "C" void func_002424C8(void* self, int flags)
{
    *(void***)((char*)self + 0x748) = D_0047CBD8;
    D_004A2C78 = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242500__FPv);
#ifdef SKIP_ASM
void* func_00242500(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)((char*)self + 0x108) = t0;
    *(int*)((char*)self + 0x10c) = t0;
    *(int*)self = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242518);
#ifdef SKIP_ASM
void operator_delete(int*);

extern "C" void func_00242518(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242540);
#ifdef SKIP_ASM
struct sMovieNode {
    char pad_0x0[0xbc];
    sMovieNode* next; // 0xbc
    sMovieNode* prev; // 0xc0
};

struct sMovieList {
    sMovieNode* head; // 0x0
    int count;        // 0x4
};

extern "C" void func_00242540(sMovieList* list, sMovieNode* node)
{
    node->next = list->head;
    node->prev = 0;
    if (list->head != 0) {
        list->head->prev = node;
    }
    list->head = node;
    list->count++;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242570);
#ifdef SKIP_ASM
extern "C" void func_00242570(sMovieList* list, sMovieNode* node)
{
    if (node->prev != 0) {
        node->prev->next = node->next;
    }
    if (node->next != 0) {
        node->next->prev = node->prev;
    }
    if (node == list->head) {
        list->head = node->next;
    }
    list->count--;
}
#endif

INCLUDE_ASM("movie/movieplayer", func_002425C0);

//100%
INCLUDE_ASM("movie/movieplayer", func_00242978);
#ifdef SKIP_ASM
// qsort-style comparator: orders by the float at +0x4, ascending.
extern "C" int func_00242978(const void* a, const void* b)
{
    float fa = *(float*)((char*)a + 0x4);
    float fb = *(float*)((char*)b + 0x4);
    if (fa < fb) {
        return -1;
    }
    if (fb < fa) {
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("movie/movieplayer", func_002429B0);


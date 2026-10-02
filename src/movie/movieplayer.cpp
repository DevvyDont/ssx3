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

INCLUDE_ASM("movie/movieplayer", func_0023C860);

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

INCLUDE_ASM("movie/movieplayer", func_0023C8F0);

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

INCLUDE_ASM("movie/movieplayer", func_0023CAE8);

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

INCLUDE_ASM("movie/movieplayer", func_0023CC78);

INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_setTitleString);

INCLUDE_ASM("movie/movieplayer", func_0023CF38);

INCLUDE_ASM("movie/movieplayer", func_0023D570);

INCLUDE_ASM("movie/movieplayer", func_0023D5A8);

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

INCLUDE_ASM("movie/movieplayer", func_0023D660);

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

INCLUDE_ASM("movie/movieplayer", func_0023EA30);

INCLUDE_ASM("movie/movieplayer", func_0023EA90);

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

INCLUDE_ASM("movie/movieplayer", func_0023F578);

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

INCLUDE_ASM("movie/movieplayer", func_0023FD40);

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

INCLUDE_ASM("movie/movieplayer", func_00240130);

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

INCLUDE_ASM("movie/movieplayer", func_00240860);

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

INCLUDE_ASM("movie/movieplayer", func_00240D90);

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

INCLUDE_ASM("movie/movieplayer", func_00241000);

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

INCLUDE_ASM("movie/movieplayer", func_00241138);

INCLUDE_ASM("movie/movieplayer", func_00241180);

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

INCLUDE_ASM("movie/movieplayer", func_002412A0);

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

INCLUDE_ASM("movie/movieplayer", func_00241400);

INCLUDE_ASM("movie/movieplayer", func_00241540);

INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceDisplayString);

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

INCLUDE_ASM("movie/movieplayer", func_00241B20);

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

INCLUDE_ASM("movie/movieplayer", func_002420C8);

INCLUDE_ASM("movie/movieplayer", func_00242288);

INCLUDE_ASM("movie/movieplayer", func_002424C8);

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


#include "common.h"

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_cSSXApp);
#ifdef SKIP_ASM
extern "C" void* cAppMan_cAppMan(void* self);
extern "C" void func_003E6448(void* dst, int c, int n);
extern int D_004A3E90;
extern void* D_0047D9C8[];

struct cSSXApp00226830 {
    char base[0x5C];
    void** vt;
    int f60;
    int f64;
    int f68;
    int f6C;
    int f70;
    int f74;
    char pad78[0xA8 - 0x78];
    char fA8[8];
    char fB0[8];
    char fB8[8];
    int fC0;
    int fC4;
    int fC8;
    int fCC;
    int fD0;
    int fD4;
    int fD8;
    int fDC;
    int fE0;
    int fE4;
    int fE8;
    int arr[11];
    int f118;
};

extern "C" cSSXApp00226830* cSSXApp_cSSXApp(cSSXApp00226830* self)
{
    cAppMan_cAppMan(self);
    self->vt = D_0047D9C8;
    self->fC4 = D_004A3E90;
    self->fC8 = D_004A3E90;
    self->fCC = D_004A3E90;
    self->fD4 = 0;
    self->fD8 = 0;
    self->fDC = 0;
    self->fE0 = 0;
    self->fE4 = 0;
    self->fE8 = 0;
    int i;
    for (i = 0; i < 11; i++) {
        self->arr[i] = 0;
    }
    self->f118 = 0;
    self->f74 = 0;
    self->f68 = 0;
    self->f6C = 0;
    self->f70 = 0;
    self->f60 = 1;
    self->f64 = 1;
    func_003E6448(self->fA8, 0, 8);
    func_003E6448(self->fB8, 0, 8);
    func_003E6448(self->fB0, 0, 8);
    return self;
}
#endif

INCLUDE_ASM("main/ssxapp", cSSXApp_init);

INCLUDE_ASM("main/ssxapp", cSSXApp_loadInputMap);

INCLUDE_ASM("main/ssxapp", cSSXApp_parseCommandLine);

extern "C" int func_00326C60(void* mgr);
extern void* D_004A28A0;

//99.9%
INCLUDE_ASM("main/ssxapp", cSSXApp_flush__Fv);
#ifdef SKIP_ASM
int cSSXApp_flush()
{
    if (D_004A28A0 != 0) {
        return func_00326C60(D_004A28A0);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_preUpdate);
#ifdef SKIP_ASM
extern "C" int func_00326B48(void* pad);
extern "C" void func_00255A20(void* p);
extern "C" void func_002668E8(void* p);
extern "C" int func_00326CA0(void* self, int i);
extern "C" void* func_00326CC8(void* self, int i);
extern "C" void func_00321298(void* target, int a, void* data);
extern void* D_004A28A0;
extern void* D_004A2EB8;
extern void* D_004A33F0;

extern "C" int cSSXApp_preUpdate(void* self)
{
    if (D_004A28A0 != 0 && func_00326B48(D_004A28A0) != 0) {
        if (D_004A2EB8 != 0) {
            func_00255A20(D_004A2EB8);
        }
        if (D_004A33F0 != 0) {
            func_002668E8(D_004A33F0);
        }
        int i;
        for (i = 0; i < 2; i++) {
            int a = func_00326CA0(D_004A28A0, i);
            void* d = func_00326CC8(D_004A28A0, i);
            func_00321298(((void**)((char*)self + 0xA8))[i], a, d);
        }
        return 1;
    }
    return 0;
}
#endif

extern "C" void func_00326B88(void* mgr);

//99.89%
INCLUDE_ASM("main/ssxapp", cSSXApp_timerCallback__Fv);
#ifdef SKIP_ASM
void cSSXApp_timerCallback()
{
    if (D_004A28A0 != 0) {
        func_00326B88(D_004A28A0);
    }
}
#endif

void* cMCOverlayManager_getManager();

//100%
INCLUDE_ASM("main/ssxapp", func_00227F80);
#ifdef SKIP_ASM
extern "C" void* func_00227F80()
{
    return cMCOverlayManager_getManager();
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_purge);
#ifdef SKIP_ASM
void operator_delete(int* p);
extern "C" void func_00398438(void* p);
extern "C" void func_00326A68(void* p);
extern "C" void func_00278308(void* p);
extern "C" void func_003DEDC0(void* handle, int arg);
// PORT: func_002B4B48 is declared (void*) but never reads it; this caller passes nothing.
extern "C" void func_002B4B48_noarg() __asm__("func_002B4B48");
extern "C" void func_00284C28();
extern "C" void func_0014DD98(void* p);
extern "C" void func_002380E8(void* p);
extern "C" void func_001A35E8(void* p, int flags);
extern void* D_004A28A0;
extern void* D_004A28A4;
extern void* D_004A28A8;
extern void* D_004A289C;

struct sVEPurge0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVEPurge {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sSSXAppPurge {
    char pad_0x00[0x78];
    void* f78;          // 0x78
    char pad_0x7C[0xC];
    int* f88;           // 0x88
    char pad_0x8C[0x10];
    char* f9C;          // 0x9C
    char pad_0xA0[0x18];
    int* fB8[2];        // 0xB8
    void* fC0;          // 0xC0
    char pad_0xC4[0x10];
    void* fD4;          // 0xD4
    void* fD8;          // 0xD8
    void* fDC;          // 0xDC
    void* fE0;          // 0xE0
    void* fE4;          // 0xE4
    void* fE8;          // 0xE8
    void* fEC[11];      // 0xEC
    void* f118;         // 0x118
    void* f11C;         // 0x11C
};

extern "C" void cSSXApp_purge(sSSXAppPurge* self)
{
    int i;
    for (i = 0; i < 2; i++) {
        if (self->fB8[i]) {
            operator_delete(self->fB8[i]);
        }
    }
    int* q = self->f88;
    if (q) {
        func_00398438(q);
        operator_delete(q);
    }
    int* mgr = (int*)D_004A28A0;
    self->f88 = 0;
    D_004A28A0 = 0;
    func_00326A68(mgr);
    operator_delete(mgr);
    if (D_004A28A4) {
        func_00278308(D_004A28A4);
        char* o = (char*)D_004A28A4;
        if (o) {
            sVEPurge* vt = *(sVEPurge**)(o + 0x2A8);
            vt[1].fn(o + vt[1].delta, 3);
        }
        D_004A28A4 = 0;
    }
    {
        char* g = (char*)D_004A289C;
        sVEPurge0* vt = *(sVEPurge0**)(g + 0x10D8);
        vt[3].fn(g + vt[3].delta);
    }
    {
        char* g = (char*)D_004A289C;
        if (g) {
            sVEPurge* vt = *(sVEPurge**)(g + 0x10D8);
            vt[1].fn(g + vt[1].delta, 3);
        }
    }
    D_004A289C = 0;
    if (self->fE4) func_003DEDC0(self->fE4, 100);
    if (self->fE8) func_003DEDC0(self->fE8, 100);
    for (i = 0; i < 11; i++) {
        if (self->fEC[i]) {
            func_003DEDC0(self->fEC[i], 100);
        }
    }
    if (self->f118) func_003DEDC0(self->f118, 100);
    func_002B4B48_noarg();
    func_00284C28();
    if (self->fD4) func_003DEDC0(self->fD4, 100);
    if (self->fD8) func_003DEDC0(self->fD8, 100);
    if (self->fDC) func_003DEDC0(self->fDC, 100);
    if (self->fE0) func_003DEDC0(self->fE0, 100);
    if (self->f78) func_0014DD98(self->f78);
    func_002380E8(self->fC0);
    if (self->f11C) func_001A35E8(self->f11C, 3);
    {
        char* m = self->f9C;
        if (m) {
            sVEPurge* vt = *(sVEPurge**)(m + 0x34);
            vt[1].fn(m + vt[1].delta, 3);
        }
    }
    D_004A28A8 = 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_startGameLoad);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0022E968(void* self);
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cGameModeMan_getGM();
extern char D_0047A808[];

extern "C" void cSSXApp_startGameLoad(void* self)
{
    void* m = func_0022E968(cMemMan_alloc(0x22C, D_0047A808, 0, 0));
    *(void**)((char*)self + 0x84) = m;
    cAppMan_setNextModule((cAppMan*)self, (unsigned int)m);
    *(void**)((char*)self + 0xC0) = cGameModeMan_getGM();
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00228238);
#ifdef SKIP_ASM
extern int D_004A203C;
extern char D_0047A818[];
extern "C" void* func_001A1CE8(void* p);
extern "C" void* func_0017F7B8(void* p);

extern "C" void func_00228238(void* self)
{
    D_004A203C = 0;
    if (*(int*)((char*)self + 0x60) != 0) {
        void* m = func_001A1CE8(cMemMan_alloc(0xB5AE0, D_0047A818, 0, 0));
        *(void**)((char*)self + 0x7C) = m;
        cAppMan_setNextModule((cAppMan*)self, (unsigned int)m);
    } else {
        void* m = func_0017F7B8(cMemMan_alloc(0x238, D_0047A818, 0, 0));
        *(void**)((char*)self + 0x80) = m;
        cAppMan_setNextModule((cAppMan*)self, (unsigned int)m);
    }
}
#endif

INCLUDE_ASM("main/ssxapp", cSSXApp_initload);

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_initLocale);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_00195880(void* self);
extern "C" void func_00195B70(void* self);
extern char D_0047A978[];

struct sOptions_228A78 {
    unsigned int pad0 : 22;
    unsigned int language : 3;
    unsigned int pad25 : 7;
    int data[0x284 / 4];
};
extern sOptions_228A78 D_00535610_228A78 __asm__("D_00535610");

struct sVEntry_228A78 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline int optLanguage_228A78(sOptions_228A78 o)
{
    return o.language;
}

extern "C" void cSSXApp_initLocale(void* self)
{
    if (*(void**)((char*)self + 0x8C) == 0) {
        *(void**)((char*)self + 0x8C) = func_00195880(cMemMan_alloc(0xC0, D_0047A978, 0, 0));
    }
    cBE_getInterface_Fv(cBE_getBE(), 4);
    char* loc = *(char**)((char*)self + 0x8C);
    sVEntry_228A78* vt = *(sVEntry_228A78**)(loc + 4);
    char* thisp = loc + vt[3].delta;
    void (**pfn)(void*, int) = &vt[3].fn;
    (*pfn)(thisp, optLanguage_228A78(D_00535610_228A78));
    func_00195B70(*(void**)((char*)self + 0x8C));
}
#endif

extern "C" void func_002B4B48(void* self);
extern "C" void func_00284C28();
extern "C" void cAppMan_loadexecpurge(void* self);

struct sExecPurgeVTable {
    char pad_0x00[0x3B8];
    short field_0x3B8;
    char pad_0x3BA[2];
    void (*fn)(void*);
};

struct sExecPurgeMgr {
    char pad_0x00[0x10D8];
    sExecPurgeVTable* vtable;
};

extern sExecPurgeMgr* D_004A5B80;

//99.95%
INCLUDE_ASM("main/ssxapp", cSSXApp_loadexecpurge__FPv);
#ifdef SKIP_ASM
void cSSXApp_loadexecpurge(void* self)
{
    func_002B4B48(self);
    func_00284C28();
    cAppMan_loadexecpurge(self);
    sExecPurgeVTable* vt = D_004A5B80->vtable;
    vt->fn((char*)D_004A5B80 + vt->field_0x3B8);
}
#endif

INCLUDE_ASM("main/ssxapp", func_00228C08);

INCLUDE_ASM("main/ssxapp", initOnline);

//100%
INCLUDE_ASM("main/ssxapp", func_00229180);
#ifdef SKIP_ASM
extern "C" void func_002292E0(void* self);
// PORT: the ctor's mangled name says int; it receives the allocated block (pointer in int)
void* cCrowdRender2D_cCrowdRender2D(int mem);
void* cCrowdRender2D_constructCrowdAnim2D(void* r);
extern char D_0047B320[];
extern int D_004A2A00;

inline void* operator new[](unsigned int, void* p) { return p; }

struct sCrowdCfg_229180 {
    short f0;
    short f2;
    short f4;
    short f6;
    int f8;
    int fC;
    int f10;
};
extern sCrowdCfg_229180 D_00536690_229180[] __asm__("D_00536690");

struct sCrowdId_229180 {
    unsigned int id;
    sCrowdId_229180() : id(0xFFFFFFFF) {}
};

struct sCrowdSlot_229180 {
    char data[0x40];
    sCrowdSlot_229180() {}
};

struct sCrowdRest_229180 {
    char data[0x20];
    sCrowdRest_229180() {}
};

static inline int AnimCount_229180(char* anim)
{
    return *(int*)(anim + 0x10);
}

struct sCrowd_229180 {
    void* render;
    char* anim;
};

extern "C" void* func_00229180(sCrowd_229180* self)
{
    new ((char*)self + 0x8) sCrowdId_229180[128];
    new ((char*)self + 0x210) sCrowdSlot_229180[128];
    new ((char*)self + 0x2220) sCrowdRest_229180[40];
    // PORT: the singleton pointer is held in an int global
    D_004A2A00 = (int)self;
    self->render = cCrowdRender2D_cCrowdRender2D((int)cMemMan_alloc(0x10, D_0047B320, 0x20000000, 0));
    char* anim = (char*)cCrowdRender2D_constructCrowdAnim2D(self->render);
    self->anim = anim;
    D_00536690_229180[0].f0 = AnimCount_229180(anim);
    D_00536690_229180[0].fC = 0x20000;
    D_00536690_229180[0].f10 = 0;
    D_00536690_229180[0].f2 = -1;
    D_00536690_229180[0].f4 = -1;
    D_00536690_229180[0].f8 = 0;
    D_00536690_229180[0].f6 = -1;
    func_002292E0(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229278);
#ifdef SKIP_ASM
extern "C" void func_00229398(void* self);
extern int D_004A2A00;
void operator_delete(int* p);
void cCrowdRender2D__cCrowdRender2D(int* self, int flags);

extern "C" void func_00229278(void* self, int flags)
{
    func_00229398(self);
    int* p = *(int**)((char*)self + 4);
    D_004A2A00 = 0;
    operator_delete(p);
    int* r = *(int**)self;
    if (r != 0) {
        cCrowdRender2D__cCrowdRender2D(r, 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_002292E0);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int c, int n);
unsigned int BXrand();

struct sCrowdSlot_2292E0 {
    char pad_0x00[0x30];
    int delay;              // 0x30
    char pad_0x34[0xC];
};

struct sCrowd_2292E0 {
    int pad0;
    int pad4;
    unsigned int ids[128];          // 0x8
    char pad_0x208[0x8];
    sCrowdSlot_2292E0 slots[128];   // 0x210
    int count;                      // 0x2210
    int count2;                     // 0x2214
    char pad_0x2218[0x8];
    char rest[0x500];               // 0x2220
};

extern "C" void func_002292E0(void* self)
{
    sCrowd_2292E0* c = (sCrowd_2292E0*)self;
    int i;
    c->count = 0;
    func_003E6448(c->slots, 0, 0x2000);
    for (i = 0; i < 128; i++) {
        c->ids[i] = 0xFFFFFFFF;
        c->slots[i].delay = BXrand() % 300 + 300;
    }
    c->count2 = 0;
    func_003E6448(c->rest, 0, 0x500);
}
#endif

INCLUDE_ASM("main/ssxapp", func_00229398);

//100%
INCLUDE_ASM("main/ssxapp", func_00229408);
#ifdef SKIP_ASM
extern "C" void func_00229B90(void* self, int a1);

extern "C" void func_00229408(void* self, int id)
{
    int i;
    for (i = 0; i < 0x80; i++) {
        unsigned* e = (unsigned*)((char*)self + 0x8) + i;
        if (*e != 0xFFFFFFFF && *(unsigned char*)e == id) {
            func_00229B90(self, i);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229498);
#ifdef SKIP_ASM
extern "C" void func_00229398(void* self);
extern "C" void func_002292E0(void* self);

extern "C" void func_00229498(void* self)
{
    func_00229398(self);
    func_002292E0(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_002294C8);
#ifdef SKIP_ASM
extern "C" void cCrowdAnim2D_update(void* p);
extern "C" void func_00229530(void* self);
extern short D_00536690[];

extern "C" void func_002294C8(void* self)
{
    int i;
    char* p;
    int t;
    cCrowdAnim2D_update(*(void**)((char*)self + 0x4));
    t = *(int*)((char*)*(void**)((char*)self + 0x4) + 0x10);
    D_00536690[0] = t;
    p = (char*)self;
    for (i = 0x27; i >= 0; i--) {
        if (*(int*)(p + 0x2230) > 0) {
            *(int*)(p + 0x2230) = *(int*)(p + 0x2230) - 1;
        }
        p += 0x20;
    }
    func_00229530(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229530);
#ifdef SKIP_ASM
unsigned int BXrand();
extern "C" int func_002A77C8(void* self, int a1);
extern void* D_004A3500;
struct sRingOwner;
struct sQuad229;
extern "C" void func_00229738(sRingOwner* self, sQuad229* q, int value);

struct sVec4_229530 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sCrowdSlot_229530 {
    sVec4_229530 pos;       // 0x00
    sVec4_229530 dx;        // 0x10
    sVec4_229530 dy;        // 0x20
    int delay;              // 0x30
    char pad_0x34[0xC];
};

struct sCrowd_229530 {
    int pad0;
    int* anim;                          // 0x4
    unsigned int ids[128];              // 0x8
    char pad_0x208[0x8];
    sCrowdSlot_229530 slots[128];       // 0x210
    int count;                          // 0x2210
};

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec4_229530 Add_229530(const sVec4_229530& a, const sVec4_229530& b)
{
    sVec4_229530 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec4_229530 Scale_229530(const sVec4_229530& v, float s)
{
    sVec4_229530 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline float randf_229530()
{
    union { int i; float f; } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

static inline float rands_229530(float lo, float hi)
{
    return randf_229530() * (hi - lo) + lo;
}

extern "C" void func_00229530(void* selfp)
{
    sCrowd_229530* self = (sCrowd_229530*)selfp;
    int n = func_002A77C8(D_004A3500, 0);
    n = n > -1 ? n : -n;
    if (n != self->count) {
        int c = n > 0 ? n : 1;
        self->count = n;
        *self->anim = c;
    }
    int dec = self->count * 10 + 4;
    for (int i = 0; i < 128; i++) {
        if (self->ids[i] != 0xFFFFFFFF) {
            self->slots[i].delay -= dec;
            if (self->slots[i].delay <= 0) {
                float r1 = rands_229530(-1.0f, 1.0f);
                float r2 = rands_229530(-1.0f, 1.0f);
                sVec4_229530 p = Add_229530(Add_229530(self->slots[i].pos, Scale_229530(self->slots[i].dx, r1)),
                                            Scale_229530(self->slots[i].dy, r2));
                func_00229738((sRingOwner*)self, (sQuad229*)&p, 3);
                self->slots[i].delay = BXrand() % 300 + 300;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229738);
#ifdef SKIP_ASM
struct sQuad229 {
    int v[4];
} __attribute__((aligned(16)));

struct sRingEntry {
    sQuad229 q;     // 0x0
    int value;      // 0x10
    int pad[3];
};

struct sRingOwner {
    char pad_0x0[0x2214];
    int index;              // 0x2214
    int pad_0x2218[2];
    sRingEntry entries[40]; // 0x2220
};

extern "C" void func_00229738(sRingOwner* self, sQuad229* q, int value)
{
    self->entries[self->index].q = *q;
    self->entries[self->index].value = value;
    self->index++;
    self->index %= 40;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229788);
#ifdef SKIP_ASM
extern "C" void func_00229910(void* self, int slot, int a1);

extern "C" void func_00229788(void* self, int a1)
{
    int i;
    for (i = 0; i < 0x40; i++) {
        if (((unsigned int*)((char*)self + 0x8))[i] == 0xFFFFFFFF) {
            func_00229910(self, i, a1);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_002297D8);
#ifdef SKIP_ASM
extern "C" void func_00229B90(void* self, int a1);

extern "C" void func_002297D8(void* self, int val)
{
    int i;
    for (i = 0; i < 0x40; i++) {
        if (((int*)self)[i + 2] == val) {
            func_00229B90(self, i);
            break;
        }
    }
}
#endif

INCLUDE_ASM("main/ssxapp", func_00229820);

INCLUDE_ASM("main/ssxapp", func_00229910);

//100%
INCLUDE_ASM("main/ssxapp", func_00229B90);
#ifdef SKIP_ASM
extern "C" void func_00229B90(void* self, int a1)
{
    ((unsigned int*)self)[a1 + 2] = 0xFFFFFFFFU;
}
#endif

INCLUDE_ASM("main/ssxapp", func_00229BA8);

//100%
INCLUDE_ASM("main/ssxapp", func_00229E20);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
class cSSXAppStream {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_00229E20(void* self, cSSXAppStream* s)
{
    s->v01((char*)self + 0x8, 0x200);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229E58);
#ifdef SKIP_ASM
extern "C" void func_00229398(void* self);
extern "C" void func_002292E0(void* self);
extern "C" void func_00229910(void* self, int slot, int a1);

// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
class cSSXAppStream_229E58 {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

struct sCrowdId_229E58 {
    unsigned int id;
    sCrowdId_229E58() : id(0xFFFFFFFF) {}
};

extern "C" void func_00229E58(void* self, cSSXAppStream_229E58* s)
{
    func_00229398(self);
    func_002292E0(self);
    sCrowdId_229E58 ids[128];
    s->v02(ids, 0x200);
    int i;
    for (i = 0; i < 128; i++) {
        if (ids[i].id != 0xFFFFFFFF) {
            func_00229910(self, i, ids[i].id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229F30);
#ifdef SKIP_ASM
// Upload two 4x4 matrices (a, b) to VU0 data memory starting at qword 4 (address 0x40).
// PORT: PS2-only VU0 inline asm (ctc2/lqc2/vsqi); the PC port needs its own matrix store.
extern "C" void func_00229F30(void* a, void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "ctc2.ni   %0, $vi1\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        "lqc2      $vf1, 0x0(%2)\n"
        "lqc2      $vf2, 0x10(%2)\n"
        "lqc2      $vf3, 0x20(%2)\n"
        "lqc2      $vf4, 0x30(%2)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        ".set reorder\n"
        :
        : "r"(0x40), "r"(a), "r"(b)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229F80__FPv);
#ifdef SKIP_ASM
void func_00229F80(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229F88);
#ifdef SKIP_ASM
// Upload a vector plus a 4x4 matrix (5 qwords) to VU0 data memory slot `index`,
// starting at qword 0x50 + index * 5.
// PORT: PS2-only VU0 inline asm (ctc2/lqc2/vsqi); the PC port needs its own store.
extern "C" void func_00229F88(int index, void* v, void* m)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "ctc2.ni   %0, $vi1\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x0(%2)\n"
        "lqc2      $vf3, 0x10(%2)\n"
        "lqc2      $vf4, 0x20(%2)\n"
        "lqc2      $vf5, 0x30(%2)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        "vsqi.xyzw $vf5, ($vi1++)\n"
        ".set reorder\n"
        :
        : "r"(index * 5 + 0x50), "r"(v), "r"(m)
        : "memory");
}
#endif

INCLUDE_ASM("main/ssxapp", func_00229FC8);

INCLUDE_ASM("main/ssxapp", func_0022A128);

INCLUDE_ASM("main/ssxapp", func_0022A270);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", func_0022A368);
#ifdef SKIP_ASM
extern "C" void func_00229FC8(void* self, void* node, int frustum);

struct sNode_A368 {
    sNode_A368* next;
};

struct sSrc_A368 {
    char pad_0x0[0x20];
    sNode_A368* head;   // 0x20
};

struct sList_A368 {
    sSrc_A368* src;
    int vuFrustum;
};

extern "C" void func_0022A368(void* self, sList_A368* list, int count)
{
    for (; count > 0; count--, list++) {
        sNode_A368* n = list->src->head;
        while (n != 0) {
            func_00229FC8(self, n, list->vuFrustum);
            n = n->next;
        }
    }
    if (*(int*)((char*)self + 0x50A4) != 0) {
        func_00229FC8(self, 0, 0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", func_0022A408);
#ifdef SKIP_ASM
extern "C" void func_0022A128(void* self, void* node, int frustum);

struct sNode_A408 {
    sNode_A408* next;
};

struct sSrc_A408 {
    char pad_0x0[0x24];
    sNode_A408* head;   // 0x24
};

struct sList_A408 {
    sSrc_A408* src;
    int vuFrustum;
};

extern "C" void func_0022A408(void* self, sList_A408* list, int count)
{
    for (; count > 0; count--, list++) {
        sNode_A408* n = list->src->head;
        while (n != 0) {
            func_0022A128(self, n, list->vuFrustum);
            n = n->next;
        }
    }
    if (*(int*)((char*)self + 0x78B0) != 0) {
        func_0022A128(self, 0, 0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", func_0022A4A8);
#ifdef SKIP_ASM
struct sNode_22A4A8 {
    sNode_22A4A8* next;
    int f4;
    int type;
};

struct sItem_22A4A8 {
    char* list;
    int arg;
};

extern "C" void func_0022A270(void* self, sNode_22A4A8* node, int arg);

struct sWorld_22A4A8 {
    char pad[0x7AB8];
    int f7AB8;
    char pad7ABC[0x7BC0 - 0x7ABC];
    int count;
    sNode_22A4A8* nodes[1];
};

extern "C" void func_0022A4A8(sWorld_22A4A8* self, sItem_22A4A8* items, int n)
{
    for (; n > 0; n--, items++) {
        for (sNode_22A4A8* p = *(sNode_22A4A8**)(items->list + 0x28); p != 0; p = p->next) {
            if (p->type == 7) {
                func_0022A270(self, p, items->arg);
            } else if (p->type == 8) {
                self->nodes[self->count++] = p;
            }
        }
    }
    if (self->f7AB8 != 0) {
        func_0022A270(self, 0, 0);
    }
}
#endif

INCLUDE_ASM("main/ssxapp", func_0022A5A0);

INCLUDE_ASM("main/ssxapp", func_0022A698);

//100%
INCLUDE_ASM("main/ssxapp", func_0022A770);
#ifdef SKIP_ASM
struct sVisNode_A770 {
    sVisNode_A770* next;    // 0x00
    int pad_0x4;
    int type;               // 0x08
    char pad_0xC[0x44];
    int sphere[4];          // 0x50
};

struct sVisSrc_A770 {
    char pad_0x0[0x28];
    sVisNode_A770* head;    // 0x28
};

struct sVisList_A770 {
    sVisSrc_A770* src;
    int vuFrustum;
};

struct sVisOwner_A770 {
    char pad_0x0[0x78B4];
    int countA;                 // 0x78B4
    sVisNode_A770* nodesA[192]; // 0x78B8
    int pad_0x7BB8[2];
    int countB;                 // 0x7BC0
    sVisNode_A770* nodesB[1];   // 0x7BC4
};

// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallms/cfc2); the PC port needs a C
// version of the microprogram at 0xEF0 (sphere vs frustum test; 2 = outside).
static inline int visSphereTest_A770(int* sphere, int frustum)
{
    int r;
    __asm__ __volatile__(
        "lqc2      $vf22, 0x0(%1)\n"
        "ctc2.ni   %0, $vi14\n"
        "vcallms   0xEF0\n"
        "cfc2.i    %0, $vi1\n"
        : "=r"(r)
        : "r"(sphere), "0"(frustum));
    return r;
}

extern "C" void func_0022A770(sVisOwner_A770* self, sVisList_A770* list, int count)
{
    for (; count > 0; count--, list++) {
        sVisNode_A770* n = list->src->head;
        while (n != 0) {
            if (n->type == 7) {
                int frustum = list->vuFrustum;
                if (frustum == 0 || visSphereTest_A770(n->sphere, frustum) != 2) {
                    self->nodesA[self->countA++] = n;
                }
            } else if (n->type == 8) {
                self->nodesB[self->countB++] = n;
            }
            n = n->next;
        }
    }
}
#endif

INCLUDE_ASM("main/ssxapp", func_0022A830);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", func_0022ADD8);
#ifdef SKIP_ASM
extern void* D_004A289C;
extern "C" void func_002425C0(void* a, void* b);
extern "C" void func_00229F30(void* a, void* b);
void func_00229F80(void* p);
extern "C" void func_0022A830(void* ctx, void* data, void* cfg);
extern "C" void func_0022A698(void* self, void* list, int n);
extern "C" void func_0022A5A0(void* self, void* list, int n);

struct sMat_22ADD8 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sVEMat_22ADD8 {
    short delta;
    short index;
    sMat_22ADD8 (*fn)(void*);
};

struct sCfg_22ADD8 {
    char pad_0x00[0x10];
    void* data;         // 0x10
};

struct sCtx_22ADD8 {
    void* owner;        // 0x0
    void* f4;           // 0x4
    void* f8;           // 0x8
    int fC;             // 0xC
};

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through vf1..vf4).
static inline void MatCopy_22ADD8(sMat_22ADD8* dst, const sMat_22ADD8* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

struct sWorldInit_22ADD8 {
    void* f0;               // 0x0
    void* f4;               // 0x4
    char pad_0x8[0x88];
    int f90;                // 0x90
    int f94;                // 0x94
    char pad_0x98[0x1098 - 0x98];
    int f1098;              // 0x1098
    char pad_0x109C[0x209C - 0x109C];
    int f209C;              // 0x209C
    char pad_0x20A0[0x40A0 - 0x20A0];
    int f40A0;              // 0x40A0
    char pad_0x40A4[0x50A4 - 0x40A4];
    int f50A4;              // 0x50A4
    int f50A8;              // 0x50A8
    char pad_0x50AC[0x70AC - 0x50AC];
    int f70AC;              // 0x70AC
    char pad_0x70B0[0x78B0 - 0x70B0];
    int f78B0;              // 0x78B0
    int f78B4;              // 0x78B4
    char pad_0x78B8[0x7AB8 - 0x78B8];
    int f7AB8;              // 0x7AB8
    int f7ABC;              // 0x7ABC
    char pad_0x7AC0[0x7BC0 - 0x7AC0];
    int f7BC0;              // 0x7BC0
    char pad_0x7BC4[0x7FC4 - 0x7BC4];
    int f7FC4;              // 0x7FC4
};

static inline void Load_22ADD8(sCtx_22ADD8* ctx, void* data, sCfg_22ADD8* c)
{
    if (data) {
        func_0022A830(ctx, data, c);
    }
}

extern "C" char* func_0022ADD8(char* self, sCfg_22ADD8* cfg, void* a2, void* a3, int a4)
{
    sWorldInit_22ADD8* w = (sWorldInit_22ADD8*)self;
    w->f0 = a2;
    w->f4 = a3;
    w->f90 = a4;
    w->f94 = 0;
    w->f1098 = 0;
    w->f209C = 0;
    w->f40A0 = 0;
    w->f50A8 = 0;
    w->f70AC = 0;
    w->f78B4 = 0;
    w->f7ABC = 0;
    w->f7BC0 = 0;
    w->f7FC4 = 0;
    w->f50A4 = 0;
    w->f78B0 = 0;
    w->f7AB8 = 0;
    {
        char* g = (char*)D_004A289C;
        sVEMat_22ADD8* vt = *(sVEMat_22ADD8**)(g + 0x10D8);
        sMat_22ADD8 m = vt[43].fn(g + vt[43].delta);
        MatCopy_22ADD8((sMat_22ADD8*)(self + 0x10), &m);
    }
    MatCopy_22ADD8((sMat_22ADD8*)(self + 0x50), (sMat_22ADD8*)((char*)D_004A289C + 0x5800));
    func_002425C0(*(void**)(self + 0x0), *(void**)(self + 0x4));
    func_00229F30(self + 0x10, self + 0x50);
    func_00229F80(*(void**)(self + 0x4));
    sCtx_22ADD8 ctx;
    char* f0 = *(char**)(self + 0x0);
    void* f8 = *(void**)(f0 + 0x110);
    ctx.owner = self;
    ctx.f4 = f0;
    ctx.f8 = f8;
    ctx.fC = 0;
    Load_22ADD8(&ctx, cfg[0].data, &cfg[0]);
    Load_22ADD8(&ctx, cfg[1].data, &cfg[1]);
    Load_22ADD8(&ctx, cfg[2].data, &cfg[2]);
    Load_22ADD8(&ctx, cfg[3].data, &cfg[3]);
    Load_22ADD8(&ctx, cfg[4].data, &cfg[4]);
    Load_22ADD8(&ctx, cfg[5].data, &cfg[5]);
    Load_22ADD8(&ctx, cfg[6].data, &cfg[6]);
    Load_22ADD8(&ctx, cfg[7].data, &cfg[7]);
    func_0022A698(self, self + 0x98, *(int*)(self + 0x94));
    func_0022A5A0(self, self + 0x98, *(int*)(self + 0x94));
    func_0022A770((sVisOwner_A770*)self, (sVisList_A770*)(self + 0x98), *(int*)(self + 0x94));
    func_0022A408(self, (sList_A408*)(self + 0x109C), *(int*)(self + 0x1098));
    func_0022A368(self, (sList_A368*)(self + 0x109C), *(int*)(self + 0x1098));
    func_0022A4A8((sWorld_22A4A8*)self, (sItem_22A4A8*)(self + 0x109C), *(int*)(self + 0x1098));
    return self;
}
#endif

INCLUDE_ASM("main/ssxapp", func_0022B008);


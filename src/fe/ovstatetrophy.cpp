#include "common.h"

//100%
INCLUDE_ASM("fe/ovstatetrophy", cOVStateTrophy_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char D_00471CD8[];

extern "C" void cOVStateTrophy_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00471CD8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_0020E9A0);

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020EAA0);
#ifdef SKIP_ASM
struct sRect20EAA0 {
    float x, y, w, h;
};
extern char D_00474230[];
extern int D_004A276C;
extern "C" void func_0020EC18(void* self, sRect20EAA0* a, sRect20EAA0* r);

extern "C" void* func_0020EAA0(void* self)
{
    sRect20EAA0 r;
    *(void**)((char*)self + 0x38) = D_00474230;
    r.x = 0.0f;
    r.y = 0.0f;
    r.w = 640.0f;
    r.h = 480.0f;
    func_0020EC18(self, &r, &r);
    D_004A276C++;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020EB08);
#ifdef SKIP_ASM
extern char D_00474230[];
extern int D_004A276C;
extern int D_004A2768;
void operator_delete(int*);

extern "C" void func_0020EB08(void* self, int flags)
{
    int n = D_004A276C - 1;
    *(void**)((char*)self + 0x38) = D_00474230;
    D_004A276C = n;
    if (n == 0) {
        D_004A2768 = 0;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020EB50);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_0020EC18(void* self, sRect20EAA0* a, sRect20EAA0* r);
extern "C" int func_00398380(void* bank, int name);
extern "C" int func_003983F0(void* bank, int hash);
extern void* D_004A28A8;
extern int D_004A2764;
extern int D_004A2768;
extern char D_004A21F0[];
struct sTex_20EB50 {
    int name;
    int f4;
    int f8;
    int handle;
};
extern sTex_20EB50 D_004C8C58[];

extern "C" void func_0020EB50(void* self, sRect20EAA0* a, sRect20EAA0* r)
{
    if (D_004A2768 == 0) {
        char* res = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x48);
        int i;
        for (i = 0; i < 11; i++) {
            D_004C8C58[i].handle = func_00398380(res + 0x58, D_004C8C58[i].name);
        }
        void* bank = *(void**)(res + 8);
        D_004A2764 = func_003983F0(bank, GetHashValue32(D_004A21F0));
        D_004A2768 = 1;
    }
    func_0020EC18(self, a, r);
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_0020EC18);

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020ED20);
#ifdef SKIP_ASM
struct sTrophySlot20ED20 {
    int active;
    int pad[5];
};
struct sPad16;
extern sPad16 D_004C8BC8;
extern char* D_004A2750;
extern int D_004A2760;
extern void* D_004A28A8;
extern "C" void func_00210618(int i);

extern "C" void func_0020ED20(void)
{
    if (D_004A2750 != 0) {
        char* race = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC);
        int i;
        for (i = 0; i < *(int*)(race + 0x78); i++) {
            func_00210618(i);
            ((sTrophySlot20ED20*)&D_004C8BC8)[i].active = 0;
        }
        D_004A2760 = 0;
    }
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_0020EDA0);

INCLUDE_ASM("fe/ovstatetrophy", func_0020FB40);

struct sPad16 { char x; int pad[3]; };
extern sPad16 D_004C8BC8;

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_002105B0);
#ifdef SKIP_ASM
extern "C" void func_002105B0(int a0)
{
    char* p = (char*)&D_004C8BC8 + a0 * 0x18;
    *(int*)p = 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_002105D0);
#ifdef SKIP_ASM
extern char* D_004A2750;
extern char* D_004A2754;
extern char* D_004A2758;
extern "C" void func_002108F8(void);

extern "C" void func_002105D0(char* data)
{
    char* a = data + *(int*)(data + 4);
    char* b = data + *(int*)(data + 0xC);
    D_004A2750 = data;
    D_004A2754 = a;
    D_004A2758 = b;
    func_002108F8();
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210608);
#ifdef SKIP_ASM
extern char* D_004A2750;
extern char* D_004A2754;
extern char* D_004A2758;

extern "C" void func_00210608(void)
{
    D_004A2750 = 0;
    D_004A2754 = 0;
    D_004A2758 = 0;
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_00210618);

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210820);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sQuad_210820 {
    float x, y, z, w;
} __attribute__((aligned(16)));
class cPosObj_210820 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual sQuad_210820* v05();
};
struct sZone_210820 {
    float sx, sy, ox, oy, f10;
};
struct sSlot_210820 {
    int active;
    int zone;
    int b, c, d, e;
};
extern sSlot_210820 D_slots210820[6] __asm__("D_004C8BC8");
extern char* D_004A2754;
extern void* D_004A28A8;

static inline float subx_210820(sZone_210820* z, sQuad_210820& p)
{
    return p.x - z->ox;
}
static inline float suby_210820(sZone_210820* z, sQuad_210820& p)
{
    return p.y - z->oy;
}

static inline float sx_210820(sZone_210820* z)
{
    return z->sx;
}
static inline float sy_210820(sZone_210820* z)
{
    return z->sy;
}

extern "C" float func_00210820(int idx)
{
    sZone_210820* zones = (sZone_210820*)D_004A2754;
    char* world = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC);
    char* rider = *(char**)(world + (idx << 2) + 0x28);
    sQuad_210820 pos = *((cPosObj_210820*)(rider + 0x6C0))->v05();
    float dy = suby_210820(&zones[D_slots210820[idx].zone], pos);
    float dx = subx_210820(&zones[D_slots210820[idx].zone], pos);
    float r = dy * sy_210820(&zones[D_slots210820[idx].zone]) + dx * sx_210820(&zones[D_slots210820[idx].zone]);
    if (D_slots210820[idx].active) {
        D_slots210820[idx].e = 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_002108F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sTrophySlot2108F8 {
    int active;
    int a, b, c, d, e;
};
extern sTrophySlot2108F8 D_slots2108F8[6] __asm__("D_004C8BC8");
extern int D_004A2760;

static inline void resetSlot2108F8(sTrophySlot2108F8* s)
{
    s->active = 1;
    s->a = 0;
    s->b = 0;
    s->c = 0;
    s->d = 0;
    s->e = 0;
}

extern "C" void func_002108F8(void)
{
    int i;
    for (i = 0; i < 6; i++) {
        resetSlot2108F8(&D_slots2108F8[i]);
    }
    D_004A2760 = 1;
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_00210940);

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210B58);
#ifdef SKIP_ASM
extern int D_004A2EEC;
extern char D_004A2560[];
extern "C" int func_00258160(int a, char* name, int len);
extern "C" void func_00210BD8(void* self, char* name, int a);
extern "C" void func_00210F40(void* self, int a1, int a2, int a3, int a4, int a5, int a6);

extern "C" int func_00210B58(void* self, int ok)
{
    if (ok != 0) {
        char* name = (char*)self + 0x9C;
        if (func_00258160(D_004A2EEC, name, 0x41) != 0) {
            func_00210BD8(self, name, 0);
        } else {
            // PORT: func_00210F40's unit definition takes these string pointers as int.
            func_00210F40(self, 0, (int)D_004A2560, (int)D_004A2560, 0, 0, 0x3D);
        }
    }
    return 1;
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_00210BD8);

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210D20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A2EEC;
extern char* D_net210D20 __asm__("D_004A2EEC");
extern "C" void* func_0020E900(void* self);
extern "C" void* func_0039F698(void* list);
extern "C" int func_00258160(int a, char* name, int len);
extern "C" void func_00210BD8(void* self, char* name, int a);

extern "C" void func_00210D20(void* self)
{
    func_0020E900(self);
    char* p = D_net210D20;
    if (p != 0) {
        int ok = *(int*)(p + 0x68) == 0 || *(int*)(p + 0x64) == 0;
        if (ok) {
            func_0039F698(*(char**)((char*)self + 0x10) + 0x18);
            *(int*)((char*)self + 0x1C) = (*(int*)((char*)self + 0x1C) & ~0x3F00) | 0x780;
        }
    }
    if (*(int*)((char*)self + 0x40) != 0) {
        char* name = (char*)self + 0x9C;
        if (func_00258160(D_004A2EEC, name, 0x41) != 0) {
            func_00210BD8(self, name, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210DD0);
#ifdef SKIP_ASM
extern "C" void func_00210DD0(void* self, int a1, int a2)
{
    if (a2 == 0x16) {
        if (a1 == *(int*)((char*)self + 0xe0)) {
            *(int*)((char*)self + 0xe0) = 0;
        }
    }
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_00210DF0);

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210F40);
#ifdef SKIP_ASM
extern "C" void func_00210FA0(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_001CCF00(void* p, int a);
extern "C" void func_001CB418(void* p, int a);

extern "C" void func_00210F40(void* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    func_00210FA0(self, a1, a4, a5, a6);
    func_001CCF00(*(void**)((char*)self + 0xE0), a2);
    func_001CB418(*(void**)((char*)self + 0xE0), a3);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210FA0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001CB030(void* mem, void* engine, void* owner, int a3, int a4);
extern "C" void func_0039F290(void* list, void* item);
extern "C" void func_001CD088(void* self, int v);
extern "C" void func_001CE3C8(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int v);
extern char D_0046EFD0[];

extern "C" void func_00210FA0(void* self, int a1, int a2, int a3, int a4)
{
    void* w = func_001CB030(cMemMan_alloc(0x444, D_0046EFD0, 0x100, 0), *(void**)((char*)self + 0x10), self, a2, 0xF);
    *(void**)((char*)self + 0xE0) = w;
    func_0039F290((char*)*(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18, w);
    *(int*)((char*)*(void**)((char*)self + 0xE0) + 0x18) = a3;
    func_001CD088(*(void**)((char*)self + 0xE0), a4);
    *(int*)((char*)*(void**)((char*)self + 0xE0) + 0x43C) = 0;
    func_001CE3C8(*(void**)((char*)self + 0xE0), 0x4B, 1, 2);
    func_001CE468(*(void**)((char*)self + 0xE0), 1);
    *(int*)((char*)*(void**)((char*)self + 0xE0) + 0x440) = 1;
    *(int*)((char*)self + 0xDC) = a1;
}
#endif

INCLUDE_ASM("fe/ovstatetrophy", func_00211088);


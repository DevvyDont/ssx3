#include "common.h"

INCLUDE_ASM("fe/debugfe", cDebugMenuScreen_draw);

//100%
INCLUDE_ASM("fe/debugfe", func_0017D008);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern void* D_004A28A8;
struct sDbgRaceSel17D008 {
    char pad_0x00[0x48];
    signed char mode;  // 0x48
    signed char slot;  // 0x49
};
extern sDbgRaceSel17D008 D_00535BC8;
class cBENewRaceIfaceK17D008 {
public:
    char pad_0x00[0xC];
    virtual int v01();
};
struct sDbgFE17D008 {
    char pad_0x0000[0x8C38];
    int modes[8]; // 0x8C38
};
extern "C" void func_00145108(void* iface, int idx);
extern "C" void cBENewRaceInterface_setGameMode(void* iface, int mode);

extern "C" void func_0017D008(void* self, int idx)
{
    cBENewRaceIfaceK17D008* iface = (cBENewRaceIfaceK17D008*)cBE_getInterface_Fv(*(void**)((char*)D_004A28A8 + 0x78), 0);
    ((sDbgFE17D008*)self)->modes[D_00535BC8.slot] = D_00535BC8.mode;
    func_00145108(iface, idx);
    cBENewRaceInterface_setGameMode(iface, ((sDbgFE17D008*)self)->modes[idx]);
    iface->v01();
}
#endif

INCLUDE_ASM("fe/debugfe", func_0017D0A8);

INCLUDE_ASM("fe/debugfe", func_0017D1B8);

//100%
INCLUDE_ASM("fe/debugfe", func_0017D200);
#ifdef SKIP_ASM
extern void* D_0046D740[];
extern char D_0045D060[];
void* func_002CAA58(void* self);
extern "C" void* func_002CCDF0(void* self, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);

extern "C" void* func_0017D200(void* self)
{
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0046D740;
    func_002CCDF0((char*)self + 0x130, D_0045D060);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D260);
#ifdef SKIP_ASM
extern void* D_004A4E90;
extern "C" void func_0017D6F0(void* self, void* ev);
extern "C" void func_00259230(void* ev);

extern "C" void func_0017D260(void* self, void* ev)
{
    if (D_004A4E90 != 0)
        func_0017D6F0(D_004A4E90, ev);
    func_00259230(ev);
}
#endif

INCLUDE_ASM("fe/debugfe", func_0017D298);

INCLUDE_ASM("fe/debugfe", func_0017D3B0);

//100%
INCLUDE_ASM("fe/debugfe", func_0017D688);
#ifdef SKIP_ASM
extern void* D_004A2EB8;
extern void* D_004A3028;
extern "C" void func_00255A20(void*);
extern "C" void func_0025AC50(void*);
extern "C" void func_002CC0C0(void* stack);
extern "C" void func_0017DB18(void* self);

struct sMenuStack17D688 {
    int count;      // 0x0
    void* items[8]; // 0x4
};
struct sDebugFE17D688 {
    int f0;
    int f4;
    sMenuStack17D688 stack; // 0x8
};

static inline void* sMenuStack17D688_top(sMenuStack17D688* s)
{
    if (s->count > 0)
        return s->items[s->count - 1];
    return 0;
}

extern "C" void func_0017D688(sDebugFE17D688* self)
{
    func_00255A20(D_004A2EB8);
    func_0025AC50(D_004A3028);
    if (sMenuStack17D688_top(&self->stack) != 0)
        func_002CC0C0(&self->stack);
    func_0017DB18(self);
}
#endif

INCLUDE_ASM("fe/debugfe", func_0017D6F0);

INCLUDE_ASM("fe/debugfe", func_0017DA30);

INCLUDE_ASM("fe/debugfe", func_0017DB18);

INCLUDE_ASM("fe/debugfe", func_0017DE70);

//100%
INCLUDE_ASM("fe/debugfe", func_0017E030);
#ifdef SKIP_ASM
extern void* D_004A3028;
extern "C" void func_002CC460(void* list);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_0025B9C8(void* mgr, void* req);
extern char D_0045D248[];

struct sDebugFEState17E030 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};

struct sDsReq17E030 {
    char pad_0x000[0x260];
    char* user;     // 0x260
    char* pass;     // 0x264
    char* pass2;    // 0x268
    char* host;     // 0x26C
    char pad_0x270[0x288 - 0x270];
    int f288;
    int f28C;
    int port;       // 0x290
    int f294;
    int f298;
    int f29C;
};

extern "C" void func_0017E030(sDebugFEState17E030* self, const char* a, const char* b)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    cBXString_cBXString4((char*)self + 0x1C04, a);
    cBXString_cBXString4((char*)self + 0x1C08, b);
    self->state = 0x4;
    sDsReq17E030* g = (sDsReq17E030*)D_004A3028;
    cBXString_cBXString4(&g->user, a);
    cBXString_cBXString4(&g->pass, b);
    cBXString_cBXString4(&g->pass2, b);
    cBXString_cBXString4(&g->host, D_0045D248);
    g->port = 0x79E;
    g->f294 = 1;
    g->f288 = 1;
    g->f29C = 0;
    g->f298 = 0;
    g->f28C = 1;
    func_0025B9C8(D_004A3028, &g->user);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E118);
#ifdef SKIP_ASM
extern void* D_004A3028;
extern "C" void func_002CC460(void* list);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_0025C610(void* mgr);

struct sDebugFEState17E118 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
    char pad_0x0C[0x1C04 - 0xC];
    char* user;  // 0x1C04
    char* pass;  // 0x1C08
};

extern "C" void func_0017E118(sDebugFEState17E118* self, const char* a, const char* b)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    cBXString_cBXString4(&self->user, a);
    cBXString_cBXString4(&self->pass, b);
    self->state = 0x5;
    cBXString_cBXString4((char*)D_004A3028 + 0x48, self->user);
    cBXString_cBXString4((char*)D_004A3028 + 0x4C, self->pass);
    func_0025C610(D_004A3028);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E1C0);
#ifdef SKIP_ASM
extern void* D_004A3028;
struct sDebugFEState17E1C0 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_0025D1B8(void* a, int b);

extern "C" void func_0017E1C0(sDebugFEState17E1C0* self, int arg)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0x8;
    func_0025D1B8(D_004A3028, arg);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E228);
#ifdef SKIP_ASM
extern void* D_004A3028;
struct sDebugFEState17E228 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_0025D428(void* a, int b);

extern "C" void func_0017E228(sDebugFEState17E228* self, int arg)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0x9;
    func_0025D428(D_004A3028, arg);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E290);
#ifdef SKIP_ASM
extern void* D_004A3028;
struct sDebugFEState17E290 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
void func_0025CD28(void* a, int b);

extern "C" void func_0017E290(sDebugFEState17E290* self, int arg)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0x7;
    func_0025CD28(D_004A3028, arg);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E2F8);
#ifdef SKIP_ASM
extern void* D_004A3028;
struct sDebugFEState17E2F8 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_0025D800(void* a, int b, int c);

extern "C" void func_0017E2F8(sDebugFEState17E2F8* self, int arg)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0xB;
    func_0025D800(D_004A3028, arg, 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E368);
#ifdef SKIP_ASM
extern "C" void func_002CC460(void* list);

extern "C" void func_0017E368(void* self)
{
    int* list = (int*)((char*)self + 0x8);
    while (*list != 0) {
        func_002CC460(list);
    }
    *(int*)((char*)self + 0x4) = 0xD;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E3B8);
#ifdef SKIP_ASM
extern "C" void func_002CC460(void* list);
extern "C" void func_002CC3B8(void* list, void* item);

extern "C" void func_0017E3B8(void* self)
{
    int* list = (int*)((char*)self + 0x8);
    while (*list != 0) {
        func_002CC460(list);
    }
    *(int*)((char*)self + 0x4) = 0xE;
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x1534);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E418);
#ifdef SKIP_ASM
extern void* D_004A3028;
struct sDebugFEState17E418 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_002CC3B8(void* list, void* item);
extern "C" void func_0025AAE0(void* a, int b);

extern "C" void func_0017E418(sDebugFEState17E418* self, int arg)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x1044);
    self->state = 0xC;
    func_0025AAE0(D_004A3028, arg);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E490);
#ifdef SKIP_ASM
extern void* D_004A3028;
struct sDebugFEState17E490 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_0025FB58(void* a, int b, int c);

extern "C" void func_0017E490(sDebugFEState17E490* self)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0xD;
    func_0025FB58(D_004A3028, *(int*)((char*)self + 0x1C0C), 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E4F0);
#ifdef SKIP_ASM
extern void* D_004A3028;
struct sDebugFEState17E4F0 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_002CC3B8(void* list, void* item);
void func_0025FC28(void* a, int b);

extern "C" void func_0017E4F0(sDebugFEState17E4F0* self)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x1044);
    self->state = 0xC;
    func_0025FC28(D_004A3028, *(int*)((char*)self + 0x1C0C));
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E558);
#ifdef SKIP_ASM
extern void* D_004A3028;
struct sDebugFEState17E558 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_002CC3B8(void* list, void* item);
extern "C" void func_0025F628(void* a, int b, int c);

extern "C" void func_0017E558(sDebugFEState17E558* self)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x1044);
    self->state = 0xC;
    func_0025F628(D_004A3028, 1, 0);
}
#endif

INCLUDE_ASM("fe/debugfe", func_0017E5C8);

INCLUDE_ASM("fe/debugfe", func_0017F2D0);

INCLUDE_ASM("fe/debugfe", func_0017F500);

INCLUDE_ASM("fe/debugfe", func_0017F7B8);

//100%
INCLUDE_ASM("fe/debugfe", func_0017F838);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0046D970[];

extern "C" void func_0017F838(void* self, int flags)
{
    *(void**)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("fe/debugfe", func_0017F868);

INCLUDE_ASM("fe/debugfe", func_0017FA60);

extern "C" void* func_00231CF0(void* self);

//100%
INCLUDE_ASM("fe/debugfe", func_0017FAB0__FPv);
#ifdef SKIP_ASM
int func_0017FAB0(void* self)
{
    return (func_00231CF0(self) != 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FAD0);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern char* D_004A289C;
extern "C" void func_003916C0(void* p, int flags);

struct sVEi_0017FAD0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sDebugFE_0017FAD0 {
    char pad_0x00[0xC];
    char* objs[2];      // 0xC
    char pad_0x14[4];
    void* font;         // 0x18
    int snd1C;          // 0x1C
    int snd20;          // 0x20
};

extern "C" void func_0017FAD0(sDebugFE_0017FAD0* self)
{
    for (int i = 0; i < 2; i++)
    {
        char* o = self->objs[i];
        if (o != 0)
        {
            sVEi_0017FAD0* vt = *(sVEi_0017FAD0**)o;
            vt[1].fn(o + vt[1].delta, 3);
        }
    }
    char* g = (char*)D_004A28A8;
    *(int*)(g + 0x7C) = 0;
    *(int*)(g + 0x80) = 0;
    if (self->snd20 >= 0)
    {
        char* m = D_004A289C;
        sVEi_0017FAD0* vt = *(sVEi_0017FAD0**)(m + 0x10D8);
        vt[50].fn(m + vt[50].delta, self->snd20);
    }
    if (self->snd1C >= 0)
    {
        char* m = D_004A289C;
        sVEi_0017FAD0* vt = *(sVEi_0017FAD0**)(m + 0x10D8);
        vt[50].fn(m + vt[50].delta, self->snd1C);
    }
    if (self->font != 0)
        func_003916C0(self->font, 3);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/debugfe", func_0017FB98);
#ifdef SKIP_ASM
extern void* D_004A28A8;
class cFEObjK17FB98 {
public:
    virtual void v01();
    virtual void v02();
};
extern "C" void func_00231D18(void* self);
extern "C" void func_0017F2D0(void* p);

extern "C" void func_0017FB98(void* self)
{
    func_00231D18(self);
    if (*(int*)((char*)D_004A28A8 + 4) == 0)
        (*(cFEObjK17FB98**)((char*)self + 0x14))->v02();
    func_0017F2D0((char*)self + 0x24);
}
#endif

INCLUDE_ASM("fe/debugfe", func_0017FBF0);

INCLUDE_ASM("fe/debugfe", func_0017FDF0);

extern "C" void* func_00320C48(int, int);

//100%
INCLUDE_ASM("fe/debugfe", func_0017FE80__FPv);
#ifdef SKIP_ASM
void* func_0017FE80(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x70);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FEA0__FPv);
#ifdef SKIP_ASM
void* func_0017FEA0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x71);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FEC0__FPv);
#ifdef SKIP_ASM
void* func_0017FEC0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x7a);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FEE0__FPv);
#ifdef SKIP_ASM
void* func_0017FEE0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x7b);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FF00__FPv);
#ifdef SKIP_ASM
void* func_0017FF00(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x7c);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FF20__FPv);
#ifdef SKIP_ASM
void* func_0017FF20(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x7d);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FF40__FPv);
#ifdef SKIP_ASM
void* func_0017FF40(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x97);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FF60__FPv);
#ifdef SKIP_ASM
void* func_0017FF60(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x98);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FF80__FPv);
#ifdef SKIP_ASM
void* func_0017FF80(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x99);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FFA0__FPv);
#ifdef SKIP_ASM
void* func_0017FFA0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x9a);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FFC0__FPv);
#ifdef SKIP_ASM
void* func_0017FFC0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x9b);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017FFE0__FPv);
#ifdef SKIP_ASM
void* func_0017FFE0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x9c);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180000__FPv);
#ifdef SKIP_ASM
void* func_00180000(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x9d);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180020__FPv);
#ifdef SKIP_ASM
void* func_00180020(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x9e);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180040__FPv);
#ifdef SKIP_ASM
void* func_00180040(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x72);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180060__FPv);
#ifdef SKIP_ASM
void* func_00180060(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x73);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180080__FPv);
#ifdef SKIP_ASM
void* func_00180080(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x74);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001800A0__FPv);
#ifdef SKIP_ASM
void* func_001800A0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x75);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001800C0__FPv);
#ifdef SKIP_ASM
void* func_001800C0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x93);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001800E0__FPv);
#ifdef SKIP_ASM
void* func_001800E0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x94);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180100__FPv);
#ifdef SKIP_ASM
void* func_00180100(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x95);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180120__FPv);
#ifdef SKIP_ASM
void* func_00180120(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x96);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180140__FPv);
#ifdef SKIP_ASM
void* func_00180140(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x7e);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180160__FPv);
#ifdef SKIP_ASM
void* func_00180160(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x7f);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180180__FPv);
#ifdef SKIP_ASM
void* func_00180180(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x80);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001801A0__FPv);
#ifdef SKIP_ASM
void* func_001801A0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x81);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001801C0__FPv);
#ifdef SKIP_ASM
void* func_001801C0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x82);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001801E0__FPv);
#ifdef SKIP_ASM
void* func_001801E0(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x83);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180200__FPv);
#ifdef SKIP_ASM
void* func_00180200(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x84);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180220__FPv);
#ifdef SKIP_ASM
void* func_00180220(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x85);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180240__FPv);
#ifdef SKIP_ASM
void* func_00180240(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x90);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180260__FPv);
#ifdef SKIP_ASM
void* func_00180260(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x92);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180280__FPv);
#ifdef SKIP_ASM
void* func_00180280(void* self)
{
    return func_00320C48(*(int*)((char*)self + 0x18), 0x91);
}
#endif

extern "C" void* func_00320BF0(int, int);

//100%
INCLUDE_ASM("fe/debugfe", func_001802A0__FPv);
#ifdef SKIP_ASM
void* func_001802A0(void* self)
{
    return func_00320BF0(*(int*)((char*)self + 0x18), 0x90);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001802C0__FPv);
#ifdef SKIP_ASM
void* func_001802C0(void* self)
{
    return func_00320BF0(*(int*)((char*)self + 0x18), 0x92);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001802E0__FPv);
#ifdef SKIP_ASM
void* func_001802E0(void* self)
{
    return func_00320BF0(*(int*)((char*)self + 0x18), 0x91);
}
#endif

INCLUDE_ASM("fe/debugfe", func_00180300);

//100%
INCLUDE_ASM("fe/debugfe", func_00180478);
#ifdef SKIP_ASM
extern void* D_004A28A8;

extern "C" void func_00180478(void* self, unsigned char idx)
{
    int v = *(int*)((char*)D_004A28A8 + (idx << 2) + 0xB0);
    *(unsigned char*)((char*)self + 0x4) = idx;
    *(int*)((char*)self + 0x18) = v;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180498);
#ifdef SKIP_ASM
extern "C" int func_00180498(void* self)
{
    void* p = *(void**)((char*)self + 0x18);
    if (p != 0) {
        return **(int**)p != 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001804B8);
#ifdef SKIP_ASM
extern void* D_004A33F0;
extern "C" int func_00266BA8(void* mgr, int a1, int* out0, int* out1);

struct sDebugFE1804B8 {
    char pad_0x00[0xC];
    int found; // 0xC
    int a;     // 0x10
    int b;     // 0x14
};

extern "C" void func_001804B8(sDebugFE1804B8* self)
{
    self->found = 0;
    if (D_004A33F0 != 0) {
        self->b = 0;
        self->a = 0;
        if (func_00266BA8(D_004A33F0, 0, &self->a, &self->b) != 0)
            self->found = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001805D8);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);

extern "C" void func_001805D8(void* self, const char* text)
{
    char* buf = (char*)self + 0xEC;
    strcpy(buf, text);
    *(int*)((char*)self + 0x16C) = strlen(buf);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180618);
#ifdef SKIP_ASM
extern "C" void func_00180618(void* self)
{
    int s = *(int*)((char*)self + 0x170);
    if (s == 0 || s == 10 || s == 20 || s == 30) {
        *(int*)((char*)self + 0x170) += 9;
    } else if (s == 39 && *(int*)((char*)self + 0xE0) == 0) {
        *(int*)((char*)self + 0x170) = 37;
    } else {
        *(int*)((char*)self + 0x170) -= 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180678);
#ifdef SKIP_ASM
extern "C" void func_00180678(void* self)
{
    int s = *(int*)((char*)self + 0x170);
    if (s == 9 || s == 19 || s == 29 || s == 39) {
        *(int*)((char*)self + 0x170) -= 9;
    } else if (s == 37 && *(int*)((char*)self + 0xE0) == 0) {
        *(int*)((char*)self + 0x170) = 39;
    } else {
        *(int*)((char*)self + 0x170) += 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001806D8);
#ifdef SKIP_ASM
extern "C" void func_001806D8(void* self)
{
    int s = *(int*)((char*)self + 0x170);
    if (s >= 0 && s < 10) {
        if (s == 8 && *(int*)((char*)self + 0xE0) == 0) {
            *(int*)((char*)self + 0x170) = 39;
        } else {
            *(int*)((char*)self + 0x170) += 30;
        }
    } else if (s >= 10 && s < 20 && *(int*)((char*)self + 0xD8) == 0) {
        if (s == 18 && *(int*)((char*)self + 0xE0) == 0) {
            *(int*)((char*)self + 0x170) = 39;
        } else {
            *(int*)((char*)self + 0x170) += 20;
        }
    } else {
        *(int*)((char*)self + 0x170) -= 10;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180768);
#ifdef SKIP_ASM
extern "C" void func_00180768(void* self)
{
    int s = *(int*)((char*)self + 0x170);
    if (s >= 30 && s < 40) {
        if (*(int*)((char*)self + 0xD8) != 0) {
            *(int*)((char*)self + 0x170) = s - 30;
        } else {
            *(int*)((char*)self + 0x170) = s - 20;
        }
    } else if (s == 28 && *(int*)((char*)self + 0xE0) == 0) {
        *(int*)((char*)self + 0x170) = 39;
    } else {
        *(int*)((char*)self + 0x170) += 10;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001807C8);
#ifdef SKIP_ASM
extern "C" int func_001807C8(void* self)
{
    int v = *(int*)((char*)self + 0xe4) ^ 1;
    *(int*)((char*)self + 0xe4) = v;
    return v;
}
#endif

INCLUDE_ASM("fe/debugfe", func_00180840);

//100%
INCLUDE_ASM("fe/debugfe", func_001809B0);
#ifdef SKIP_ASM
struct sStr10 { char c[10]; };
extern sStr10 D_0045D550;
extern char D_004A1350[];
extern char D_0045D560[];
extern char D_004A1358[];
extern char D_004A1360[];
extern "C" char* func_004162D0(char*, const char*);

extern "C" void func_001809B0(void* self, int c, char* dst)
{
    char buf[2];
    *(sStr10*)dst = D_0045D550;
    if (c < 10) {
        buf[0] = c + '0';
        buf[1] = 0;
        func_004162D0(dst, buf);
    } else if (c < 36) {
        buf[0] = c + 'A' - 10;
        buf[1] = 0;
        func_004162D0(dst, buf);
    } else if (c == 36) {
        func_004162D0(dst, D_004A1350);
    } else if (c == 37) {
        func_004162D0(dst, D_0045D560);
    } else if (c == 38) {
        func_004162D0(dst, D_004A1358);
    } else if (c == 39) {
        func_004162D0(dst, D_004A1360);
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180A90);
#ifdef SKIP_ASM
struct sStr8 { char c[8]; };
extern char D_004A1368[];
extern char D_004A1350[];
extern char D_0045D560[];
extern char D_004A1358[];
extern char D_004A1360[];
extern "C" char* func_004162D0(char*, const char*);

extern "C" void func_00180A90(void* self, int c, char* dst)
{
    char buf[2];
    *(sStr8*)dst = *(sStr8*)D_004A1368;
    if (c < 10) {
        buf[0] = c + '0';
        buf[1] = 0;
        func_004162D0(dst, buf);
    } else if (c < 36) {
        buf[0] = c + 'A' - 10;
        buf[1] = 0;
        func_004162D0(dst, buf);
    } else if (c == 36) {
        func_004162D0(dst, D_004A1350);
    } else if (c == 37) {
        func_004162D0(dst, D_0045D560);
    } else if (c == 38) {
        func_004162D0(dst, D_004A1358);
    } else if (c == 39) {
        func_004162D0(dst, D_004A1360);
    }
}
#endif


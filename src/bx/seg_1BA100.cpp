#include "common.h"
#include "sound/crowdsys.h"
#include "sound/banksys.h"
#include "worldpainter/worldpainterquery.h"
#include "worldpainter/worldpainterman.h"
#include "worldpainter/worldpainterlayer.h"
#include "sound/speechsys.h"
#include "sound/musicsys.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("bx/seg_1BA100", bxLogPrint);
#ifdef SKIP_ASM
/**
 * Discards a formatted log message: the body is empty in this build.
 *
 * @param fmt  printf-style format string (ignored, as are the variable arguments)
 */
extern "C" void bxLogPrint(const char *fmt, ...) {
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9138);
#ifdef SKIP_ASM
extern "C" int func_002B9138(int *arg0) {
    return *arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9140);
#ifdef SKIP_ASM
extern "C" int func_002B9140(void *arg0) {
    return *(volatile int *)((char*)arg0 + 0x1C);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9150);
#ifdef SKIP_ASM
extern "C" int func_002B9150(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20))) + ((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9168);
#ifdef SKIP_ASM
extern "C" int func_002B9168(void *p) {
    return *(int*)((char*)p + (*(volatile int*)((char*)p + 0x1C) << 2) + 0x24);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9180);
#ifdef SKIP_ASM
extern "C" int func_002B9180(void *p) {
    return (*(int**)((char*)p + 0x34))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9198);
#ifdef SKIP_ASM
extern "C" int func_002B9198(void *p) {
    return (*(int**)((char*)p + 0x38))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B91B0);
#ifdef SKIP_ASM
extern "C" int func_002B91B0(void *p) {
    return (*(int**)((char*)p + 0x3C))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B91C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it.
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);

// One 0x18-byte option record of cSndPlayOpts' option stack.
struct sSndOptsRec { signed char v; char pad[0x17]; };

// Per-level four-int record at cSndPlayOpts+0x70 (reset to 100, 0, 90, 50).
struct sSndOptsQuad { int a, b, c, d; };

// cSndPlayOpts (prototype class name): the current option record plus per-level arrays indexed by the stack depth idx.
struct sSndPlayOpts {
    int pad0;
    sSndOptsRec cur;                    // 0x04
    volatile int idx;                   // 0x1C
    sSndOptsRec* recs;                  // 0x20
    int arr24[4];
    int* p34;
    int* vals;                          // 0x38
    int* p3C;
    sSndCallback callbacks[4];          // 0x40
    int* p60;
    int* p64;
    int* p68;
    int* p6C;
    sSndOptsQuad* q70;
    int* p74;
    int* p78;
    int* p7C;
    int* p80;
    int* p84;
};


// cSndPlayOpts::OPTS_GetCallback (prototype name)
extern "C" sSndCallback func_002B91C8(sSndPlayOpts* s) {
    return s->callbacks[s->idx];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9208);
#ifdef SKIP_ASM
extern "C" unsigned short func_002B9208(void *arg0) {
    return (*(unsigned short *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9228);
#ifdef SKIP_ASM
extern "C" short func_002B9228(void *arg0) {
    return (*(short *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (0xA)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9248);
#ifdef SKIP_ASM
extern "C" int func_002B9248(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x70))) + ((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x10);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9260);
#ifdef SKIP_ASM
extern "C" int func_002B9260(void *p) {
    return (*(int**)((char*)p + 0x60))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9278);
#ifdef SKIP_ASM
extern "C" int func_002B9278(void *p) {
    return (*(int**)((char*)p + 0x64))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9290);
#ifdef SKIP_ASM
extern "C" float func_002B9290(void *p) {
    return (*(float**)((char*)p + 0x68))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B92A8);
#ifdef SKIP_ASM
extern "C" float func_002B92A8(void *p) {
    return (*(float**)((char*)p + 0x6C))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B92C0);
#ifdef SKIP_ASM
extern "C" int func_002B92C0(void *p) {
    return (*(int**)((char*)p + 0x74))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B92D8);
#ifdef SKIP_ASM
extern "C" int func_002B92D8(void *p) {
    return (*(int**)((char*)p + 0x78))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B92F0);
#ifdef SKIP_ASM
extern "C" int func_002B92F0(void *p) {
    return (*(int**)((char*)p + 0x7C))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9308);
#ifdef SKIP_ASM
extern "C" int func_002B9308(void *p) {
    return (*(int**)((char*)p + 0x80))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9320);
#ifdef SKIP_ASM
extern "C" int func_002B9320(void *p) {
    return (*(int**)((char*)p + 0x84))[*(volatile int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9338);
#ifdef SKIP_ASM
extern "C" void func_003BA020(int);

extern "C" void func_002B9338(void *arg0) {
    func_003BA020((*(int *)((char*)(arg0) + (0x20))) + ((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9368);
#ifdef SKIP_ASM
struct S9368 { int a[6]; };
extern "C" void func_002B9368(void *self) {
    *(S9368*)((char*)self + 4) = *(S9368*)((char*)(*(int*)((char*)self + 0x20)) + *(volatile int*)((char*)self + 0x1C) * 0x18);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B93B8);
#ifdef SKIP_ASM
struct Rec_93B8 { signed char v; char pad[0x17]; };
struct Q_93B8 { int a, b, c, d; };
struct Obj_93B8 {
    int pad0;
    Rec_93B8 cur;
    volatile int idx;
    Rec_93B8* recs;
    int arr24[4];
    int* p34;
    int* vals;
    int* p3C;
    int arr40[4][2];
    int* p60;
    int* p64;
    int* p68;
    int* p6C;
    Q_93B8* q70;
    int* p74;
    int* p78;
    int* p7C;
    int* p80;
    int* p84;
};
extern "C" void* func_00416210(void*, int, unsigned int);

extern "C" void func_002B93B8(Obj_93B8* p) {
    p->recs[p->idx] = p->cur;
    p->arr24[p->idx] = 0;
    p->p34[p->idx] = 0;
    p->vals[p->idx] = p->recs[p->idx].v;
    p->p3C[p->idx] = 0;
    func_00416210((char*)p + p->idx * 8 + 0x40, 0, 8);
    p->p60[p->idx] = 0;
    p->p64[p->idx] = 0;
    p->p68[p->idx] = 0;
    p->p6C[p->idx] = 0;
    p->q70[p->idx].b = 0;
    p->q70[p->idx].a = 100;
    p->q70[p->idx].c = 90;
    p->q70[p->idx].d = 50;
    p->p74[p->idx] = 0;
    p->p80[p->idx] = 0;
    p->p84[p->idx] = 0;
    p->p78[p->idx] = 127;
    p->p7C[p->idx] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B95B0);
#ifdef SKIP_ASM
struct Rec_95B0 { signed char v; char pad[0x17]; };
struct Q_95B0 { int a, b, c, d; };
struct Obj_95B0 {
    int pad0;
    Rec_95B0 cur;
    volatile int idx;
    Rec_95B0* recs;
    int arr24[4];
    int* p34;
    int* vals;
    int* p3C;
    int arr40[4][2];
    int* p60;
    int* p64;
    int* p68;
    int* p6C;
    Q_95B0* q70;
    int* p74;
    int* p78;
    int* p7C;
    int* p80;
    int* p84;
};
extern "C" void* func_00416210(void*, int, unsigned int);

extern "C" void func_002B95B0(Obj_95B0* p) {
    p->idx++;
    p->recs[p->idx] = p->cur;
    p->arr24[p->idx] = 0;
    p->p34[p->idx] = 0;
    p->vals[p->idx] = p->recs[p->idx].v;
    p->p3C[p->idx] = 0;
    func_00416210((char*)p + p->idx * 8 + 0x40, 0, 8);
    p->p60[p->idx] = 0;
    p->p64[p->idx] = 0;
    p->p68[p->idx] = 0;
    p->p6C[p->idx] = 0;
    p->q70[p->idx].b = 0;
    p->q70[p->idx].a = 100;
    p->q70[p->idx].c = 90;
    p->q70[p->idx].d = 50;
    p->p74[p->idx] = 0;
    p->p80[p->idx] = 0;
    p->p84[p->idx] = 0;
    p->p78[p->idx] = 127;
    p->p7C[p->idx] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B97B8);
#ifdef SKIP_ASM
extern "C" void func_002B97B8(void *arg0) {
    *(volatile int *)((char*)arg0 + 0x1C) = *(volatile int *)((char*)arg0 + 0x1C) - 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B97D0);
#ifdef SKIP_ASM
extern "C" void func_002B97D0(void *arg0, signed char arg1) {
    (*(signed char *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (2))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B97F0);
#ifdef SKIP_ASM
extern "C" void func_002B97F0(void *arg0, signed char arg1) {
    (*(signed char *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (3))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9810);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#define MIN_9810(a,b) ((a) < (b) ? (a) : (b))
#define MAX_9810(a,b) ((a) < (b) ? (b) : (a))
extern "C" void func_002B9810(sSndPlayOpts *p, int v) {
    p->vals[p->idx] = v;
    p->recs[p->idx].v = MAX_9810(MIN_9810(p->vals[p->idx], 127), 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B98A8);
#ifdef SKIP_ASM
#define MIN_98A8(a, b) ((a) < (b) ? (a) : (b))
#define MAX_98A8(a, b) ((a) >= (b) ? (a) : (b))
struct Rec_98A8 { unsigned char v; char pad[0x17]; };
struct Obj_98A8 {
    char pad0[0x1C];
    volatile int idx;
    Rec_98A8* recs;
    char pad24[0x14];
    int* vals;
};

extern "C" void func_002B98A8(Obj_98A8* p, float f) {
    p->vals[p->idx] = (int)MAX_98A8(MIN_98A8(p->vals[p->idx] * f, 127.0f), 0.0f);
    p->recs[p->idx].v = MAX_98A8(MIN_98A8(p->vals[p->idx], 127), 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B99E8);
#ifdef SKIP_ASM
#define MIN_99E8(a, b) ((a) < (b) ? (a) : (b))
#define MAX_99E8(a, b) ((a) >= (b) ? (a) : (b))
struct Rec_99E8 { unsigned char v; char pad[0x17]; };
struct Obj_99E8 {
    char pad0[0x1C];
    volatile int idx;
    Rec_99E8* recs;
    char pad24[0x14];
    int* vals;
};

extern "C" void func_002B99E8(Obj_99E8* p, int n) {
    p->vals[p->idx] = MAX_99E8(MIN_99E8(p->vals[p->idx] * n / 127, 127), 0);
    p->recs[p->idx].v = MAX_99E8(MIN_99E8(p->vals[p->idx], 127), 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9B48);
#ifdef SKIP_ASM
extern "C" void func_002B9B48(void *arg0, signed char arg1) {
    (*(signed char *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (1))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9B68);
#ifdef SKIP_ASM
extern "C" void func_002B9B68(void *arg0, short arg1) {
    (*(short *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (0xC))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9B88);
#ifdef SKIP_ASM
extern "C" void func_002B9B88(void *arg0, int arg1) {
    (*(int *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x10) + (*(int *)((char*)(arg0) + (0x70))))) + (4))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9BA0);
#ifdef SKIP_ASM
struct E_9BA0 { int v[4]; };
extern "C" void func_002B9BA0(void *p, int v) {
    (*(E_9BA0**)((char*)p + 0x70))[*(volatile int*)((char*)p + 0x1C)].v[0] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9BB8);
#ifdef SKIP_ASM
extern "C" void func_002B9BB8(void *arg0, int arg1) {
    (*(int *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x10) + (*(int *)((char*)(arg0) + (0x70))))) + (8))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9BD0);
#ifdef SKIP_ASM
extern "C" void func_002B9BD0(void *arg0, int arg1) {
    (*(int *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x10) + (*(int *)((char*)(arg0) + (0x70))))) + (0xC))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9BE8);
#ifdef SKIP_ASM
extern "C" void func_002B9BE8(void *arg0, short arg1) {
    (*(short *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (8))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C08);
#ifdef SKIP_ASM
extern "C" void func_002B9C08(void *arg0, short arg1) {
    (*(short *)((char*)((((*(volatile int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (0xA))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C28);
#ifdef SKIP_ASM
extern "C" void func_002B9C28(void *p, int v) {
    *(int*)((char*)p + (*(volatile int*)((char*)p + 0x1C) << 2) + 0x24) = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C40);
#ifdef SKIP_ASM
extern "C" void func_002B9C40(void *arg0) {
    int *base = *(int **)((char *)arg0 + 0x34);
    base[*(volatile int *)((char *)arg0 + 0x1C)] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C60);
#ifdef SKIP_ASM
extern "C" void func_002B9C60(void *p, int v) {
    (*(int**)((char*)p + 0x64))[*(volatile int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C78);
#ifdef SKIP_ASM
extern "C" void func_002B9C78(void *self, int v) {
    int t = (v <= 0xFFFF) ? v : 0xFFFF;
    *(short*)((char*)(*(volatile int*)((char*)self + 0x1C) * 0x18 + *(int*)((char*)self + 0x20)) + 0x14) = (t <= -1) ? 0 : t;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9CB0);
#ifdef SKIP_ASM
extern "C" void func_002B9CB0(void *p, int v) {
    (*(int**)((char*)p + 0x60))[*(volatile int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9CC8);
#ifdef SKIP_ASM
extern "C" void func_002B9CC8(void *p, int v) {
    (*(int**)((char*)p + 0x3C))[*(volatile int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9CE0);
#ifdef SKIP_ASM
struct V8_9CE0 { int a, b; };
extern "C" void func_002B9CE0(char *arg0, V8_9CE0 arg1) {
    V8_9CE0 *d = (V8_9CE0 *)(arg0 + *(volatile int *)(arg0 + 0x1C) * 8);
    d[8] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D10);
#ifdef SKIP_ASM
extern "C" void func_002B9D10(void *p, int v) {
    (*(int**)((char*)p + 0x74))[*(volatile int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D28);
#ifdef SKIP_ASM
extern "C" void func_002B9D28(void *p, int v) {
    (*(int**)((char*)p + 0x78))[*(volatile int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D40);
#ifdef SKIP_ASM
extern "C" void func_002B9D40(void *arg0, int arg1) {
    int *base = *(int **)((char *)arg0 + 0x7C);
    base[*(volatile int *)((char *)arg0 + 0x1C)] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D58);
#ifdef SKIP_ASM
extern "C" void func_002B9D58(void *arg0, int arg1) {
    int *base = *(int **)((char *)arg0 + 0x80);
    base[*(volatile int *)((char *)arg0 + 0x1C)] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D70);
#ifdef SKIP_ASM
extern "C" void func_002B9D70(void *arg0, int arg1) {
    int *base = *(int **)((char *)arg0 + 0x84);
    base[*(volatile int *)((char *)arg0 + 0x1C)] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D88);
#ifdef SKIP_ASM
extern "C" void func_002B9D88(void *arg0) {
    (*(int *)((char*)(arg0) + (0x94))) = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D98);
#ifdef SKIP_ASM
extern "C" void func_002B9D98(void *arg0) {
    (*(int *)((char*)(arg0) + (0x94))) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DA0);
#ifdef SKIP_ASM
extern "C" int func_002B9DA0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x94)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DA8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// volatile keeps the load out of the jr delay slot, as in the target.
extern "C" int func_002B9DA8(void *arg0) {
    return (*(volatile int *)((char*)(arg0) + (0x88)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DB8);
#ifdef SKIP_ASM
extern "C" float func_002B9DB8(void *arg0) {
    int temp_2;
    int temp_3;

    temp_2 = (*(volatile int *)((char*)(arg0) + (0x88)));
    temp_3 = (*(int *)((char*)(arg0) + (0x8C)));
    (*(int *)((char*)(arg0) + (0x8C))) = temp_2;
    return (float) (temp_2 - temp_3) * 0.01f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DE0);
#ifdef SKIP_ASM
extern "C" int func_002B9DE0(void *arg0, int arg1) {
    return (*(int *)((char*)(arg0) + (0x8EC))) + (arg1 * 0xC0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DF8);
#ifdef SKIP_ASM
extern "C" int func_002B9DF8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x8E8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E00);
#ifdef SKIP_ASM
extern "C" void func_002B9E00(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x8E0))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E08);
#ifdef SKIP_ASM
extern "C" void func_002B9E08(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x8E4))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E10);
#ifdef SKIP_ASM
extern "C" int func_002B9E10(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x8E0)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E18);
#ifdef SKIP_ASM
extern "C" int func_002B9E18(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x8E4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E20);
#ifdef SKIP_ASM
extern "C" int func_002B9E20(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E28);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct Q_9E28 { int a; int *data; int tail; int head; };
extern "C" int func_002B9E28(char *self) {
    int r;
    if (*(int *)(self + 4) != 0) {
        Q_9E28 *q = (Q_9E28 *)(self + 0x18C);
        if (*(int *)(self + 0x194) == *(int *)(self + 0x198)) r = -1;
        else r = q->data[q->head];
    } else r = -1;
    return r;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E70);
#ifdef SKIP_ASM
extern "C" void func_00289DF0(void*, int);
extern "C" void func_002B9E70(void *a) {
    func_00289DF0(a, 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E90);
#ifdef SKIP_ASM
extern "C" void func_00289DF0(void*, int);
extern "C" void func_002B9E90(void *a) {
    func_00289DF0(a, 1);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9EB0);
#ifdef SKIP_ASM
struct S9EB0 { int a; char pad[0x5C]; };
extern "C" int func_002B9EB0(void *arg0, int arg1) {
    S9EB0 *base = *(S9EB0 **)((char *)arg0 + 0xACC);
    return base[arg1].a;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9EC8);
#ifdef SKIP_ASM
struct S9EC8 { int a; char pad[0x5C]; };
extern "C" int func_002B9EC8(void *arg0, int arg1) {
    S9EC8 *base = *(S9EC8 **)((char *)arg0 + 0xACC);
    return base[arg1].a == 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9EE8);
#ifdef SKIP_ASM
struct S9EE8 { int a; char pad[0x5C]; };
extern "C" int func_002B9EE8(void *arg0, int arg1) {
    S9EE8 *base = *(S9EE8 **)((char *)arg0 + 0xACC);
    return base[arg1].a == 2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9F08);
#ifdef SKIP_ASM
extern "C" int func_002B9F88(void *, int);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B65D0(int);

extern "C" int func_002B9F08(void *self, int idx) {
    int r = 0;
    func_003B58A0();
    bool ok = *(int *)((char*)(*(int *)((char*)self + 0xACC)) + idx * 0x60) != 1;
    if (!ok) {
        r = func_003B65D0(func_002B9F88(self, idx)) == 1;
    }
    func_003B58D8();
    return r;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9F88);
#ifdef SKIP_ASM
struct S9F88 { int a; int b; char pad[0x58]; };
extern "C" int func_002B9F88(void *arg0, int arg1) {
    S9F88 *base = *(S9F88 **)((char *)arg0 + 0xACC);
    bool ne = base[arg1].a != 1;
    if (!ne) {
        return base[arg1].b;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9FB8);
#ifdef SKIP_ASM
extern "C" void func_002B9FB8(cBankSys* arg0, int arg1) {
    int *base = arg0->mBank;
    base[*(int *)(*(char **)arg0 + 0x1F4)] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9FD8);
#ifdef SKIP_ASM
extern "C" void func_002B9FD8(cBankSys* arg0, int arg1) {
    int *base = arg0->mPatch;
    base[*(int *)(*(char **)arg0 + 0x1F4)] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9FF8);
#ifdef SKIP_ASM
struct Rec_9FF8 { signed char v; char pad[0x17]; };
struct Q_9FF8 { int a, b, c, d; };
struct Obj_9FF8 {
    int pad0;
    Rec_9FF8 cur;
    volatile int idx;
    Rec_9FF8* recs;
    int arr24[4];
    int* p34;
    int* vals;
    int* p3C;
    int arr40[4][2];
    int* p60;
    int* p64;
    int* p68;
    int* p6C;
    Q_9FF8* q70;
    int* p74;
    int* p78;
    int* p7C;
    int* p80;
    int* p84;
};
extern "C" void* func_00416210(void*, int, unsigned int);


struct Outer_9FF8 { char pad[0x1D8]; Obj_9FF8 sub; };
extern "C" void func_002B9FF8(Outer_9FF8** arg) {
    Outer_9FF8* o = *arg;
    Obj_9FF8* p = &o->sub;
    o->sub.idx++;
    p->recs[o->sub.idx] = o->sub.cur;
    o->sub.arr24[o->sub.idx] = 0;
    p->p34[o->sub.idx] = 0;
    p->vals[o->sub.idx] = p->recs[o->sub.idx].v;
    p->p3C[o->sub.idx] = 0;
    func_00416210((char*)p + o->sub.idx * 8 + 0x40, 0, 8);
    p->p60[o->sub.idx] = 0;
    p->p64[o->sub.idx] = 0;
    p->p68[o->sub.idx] = 0;
    p->p6C[o->sub.idx] = 0;
    p->q70[o->sub.idx].b = 0;
    p->q70[o->sub.idx].a = 100;
    p->q70[o->sub.idx].c = 90;
    p->q70[o->sub.idx].d = 50;
    p->p74[o->sub.idx] = 0;
    p->p80[o->sub.idx] = 0;
    p->p84[o->sub.idx] = 0;
    p->p78[o->sub.idx] = 127;
    p->p7C[o->sub.idx] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BA208);
#ifdef SKIP_ASM
struct Rec_A208 { signed char v; char pad[0x17]; };
struct Q_A208 { int a, b, c, d; };
struct Obj_A208 {
    int pad0;
    Rec_A208 cur;
    volatile int idx;
    Rec_A208* recs;
    int arr24[4];
    int* p34;
    int* vals;
    int* p3C;
    int arr40[4][2];
    int* p60;
    int* p64;
    int* p68;
    int* p6C;
    Q_A208* q70;
    int* p74;
    int* p78;
    int* p7C;
    int* p80;
    int* p84;
};
extern "C" void* func_00416210(void*, int, unsigned int);


struct Outer_A208 { char pad[0x1D8]; Obj_A208 sub; };
struct Arg_A208 { Outer_A208* o; int* vals; };
extern "C" void func_002BA208(Arg_A208* arg, int v) {
    Outer_A208* o = arg->o;
    Obj_A208* p = &o->sub;
    o->sub.idx++;
    p->recs[o->sub.idx] = o->sub.cur;
    o->sub.arr24[o->sub.idx] = 0;
    p->p34[o->sub.idx] = 0;
    p->vals[o->sub.idx] = p->recs[o->sub.idx].v;
    p->p3C[o->sub.idx] = 0;
    func_00416210((char*)p + o->sub.idx * 8 + 0x40, 0, 8);
    p->p60[o->sub.idx] = 0;
    p->p64[o->sub.idx] = 0;
    p->p68[o->sub.idx] = 0;
    p->p6C[o->sub.idx] = 0;
    p->q70[o->sub.idx].b = 0;
    p->q70[o->sub.idx].a = 100;
    p->q70[o->sub.idx].c = 90;
    p->q70[o->sub.idx].d = 50;
    p->p74[o->sub.idx] = 0;
    p->p80[o->sub.idx] = 0;
    p->p84[o->sub.idx] = 0;
    p->p78[o->sub.idx] = 127;
    p->p7C[o->sub.idx] = 1;
    arg->vals[arg->o->sub.idx] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BA448);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// cSoundSys (its vtable is _vt$12cSndPlayOpts$9cSoundSys): the cSndPlayOpts part sits at 0x1D8.
struct sSoundSysView { char pad[0x1D8]; sSndPlayOpts sub; };

// An object holding the sound system and two per-level int arrays.
struct sSndOptsUser { sSoundSysView* o; int* vals; int* vals2; };

extern "C" void* func_00416210(void*, int, unsigned int);

extern "C" void func_002BA448(sSndOptsUser* arg, int v, int v2) {
    sSoundSysView* o = arg->o;
    sSndPlayOpts* p = &o->sub;
    o->sub.idx++;
    p->recs[o->sub.idx] = o->sub.cur;
    o->sub.arr24[o->sub.idx] = 0;
    p->p34[o->sub.idx] = 0;
    p->vals[o->sub.idx] = p->recs[o->sub.idx].v;
    p->p3C[o->sub.idx] = 0;
    func_00416210((char*)p + o->sub.idx * 8 + 0x40, 0, 8);
    p->p60[o->sub.idx] = 0;
    p->p64[o->sub.idx] = 0;
    p->p68[o->sub.idx] = 0;
    p->p6C[o->sub.idx] = 0;
    p->q70[o->sub.idx].b = 0;
    p->q70[o->sub.idx].a = 100;
    p->q70[o->sub.idx].c = 90;
    p->q70[o->sub.idx].d = 50;
    p->p74[o->sub.idx] = 0;
    p->p80[o->sub.idx] = 0;
    p->p84[o->sub.idx] = 0;
    p->p78[o->sub.idx] = 127;
    p->p7C[o->sub.idx] = 1;
    arg->vals[arg->o->sub.idx] = v;
    arg->vals2[arg->o->sub.idx] = v2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BA6B0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_00416210(void*, int, unsigned int);

extern "C" int func_002906B8(void*);
extern "C" void func_002BA6B0(sSndOptsUser* arg, int v, int v2) {
    sSoundSysView* o = arg->o;
    sSndPlayOpts* p = &o->sub;
    o->sub.idx++;
    p->recs[o->sub.idx] = o->sub.cur;
    o->sub.arr24[o->sub.idx] = 0;
    p->p34[o->sub.idx] = 0;
    p->vals[o->sub.idx] = p->recs[o->sub.idx].v;
    p->p3C[o->sub.idx] = 0;
    func_00416210((char*)p + o->sub.idx * 8 + 0x40, 0, 8);
    p->p60[o->sub.idx] = 0;
    p->p64[o->sub.idx] = 0;
    p->p68[o->sub.idx] = 0;
    p->p6C[o->sub.idx] = 0;
    p->q70[o->sub.idx].b = 0;
    p->q70[o->sub.idx].a = 100;
    p->q70[o->sub.idx].c = 90;
    p->q70[o->sub.idx].d = 50;
    p->p74[o->sub.idx] = 0;
    p->p80[o->sub.idx] = 0;
    p->p84[o->sub.idx] = 0;
    p->p78[o->sub.idx] = 127;
    p->p7C[o->sub.idx] = 1;
    arg->vals[arg->o->sub.idx] = v;
    arg->vals2[arg->o->sub.idx] = v2;
    func_002906B8(arg);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BA920);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_00416210(void*, int, unsigned int);

#define MIN_A920(a,b) ((a) < (b) ? (a) : (b))
#define MAX_A920(a,b) ((a) < (b) ? (b) : (a))
extern "C" int func_002906B8(void*);
extern "C" void func_002BA920(sSndOptsUser* arg, int v, int v2, int v3) {
    sSoundSysView* o = arg->o;
    sSndPlayOpts* p = &o->sub;
    o->sub.idx++;
    p->recs[o->sub.idx] = o->sub.cur;
    o->sub.arr24[o->sub.idx] = 0;
    p->p34[o->sub.idx] = 0;
    p->vals[o->sub.idx] = p->recs[o->sub.idx].v;
    p->p3C[o->sub.idx] = 0;
    func_00416210((char*)p + o->sub.idx * 8 + 0x40, 0, 8);
    p->p60[o->sub.idx] = 0;
    p->p64[o->sub.idx] = 0;
    p->p68[o->sub.idx] = 0;
    p->p6C[o->sub.idx] = 0;
    p->q70[o->sub.idx].b = 0;
    p->q70[o->sub.idx].a = 100;
    p->q70[o->sub.idx].c = 90;
    p->q70[o->sub.idx].d = 50;
    p->p74[o->sub.idx] = 0;
    p->p80[o->sub.idx] = 0;
    p->p84[o->sub.idx] = 0;
    p->p78[o->sub.idx] = 127;
    p->p7C[o->sub.idx] = 1;
    arg->vals[arg->o->sub.idx] = v;
    arg->vals2[arg->o->sub.idx] = v2;
    {
        sSoundSysView* o2 = arg->o;
        sSndPlayOpts* p2 = &o2->sub;
        p2->vals[o2->sub.idx] = v3;
        p2->recs[o2->sub.idx].v = MAX_A920(MIN_A920(p2->vals[p2->idx], 127), 0);
    }
    func_002906B8(arg);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC30);
#ifdef SKIP_ASM
extern "C" int func_002BAC30(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x408)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC38);
#ifdef SKIP_ASM
extern "C" int func_002B2488(int);

extern "C" int func_002BAC38(void *arg0) {
    int temp_4;
    int var_2;

    temp_4 = (*(int *)((char*)(arg0) + (0x408)));
    var_2 = -1;
    if (temp_4 != 0) {
        var_2 = func_002B2488(temp_4);
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC60);
#ifdef SKIP_ASM
extern "C" int func_002BAC60(cMusicSys* musicSys) {
    return (musicSys->mNumSongs);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC68);
#ifdef SKIP_ASM
extern "C" int func_002BAC68(cMusicSys* musicSys) {
    return (musicSys->mNumActiveSongs);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC70);
#ifdef SKIP_ASM
extern "C" int func_002BAC70(cMusicSys* musicSys) {
    return (musicSys->mNumFESongs);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC78);
#ifdef SKIP_ASM
struct S78 { char x[0xB]; };
extern "C" void *func_002BAC78(cMusicSys* musicSys) {
    return (S78 *)((char *)musicSys + 0x124) + musicSys->mCurrentSongNum;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC90);
#ifdef SKIP_ASM
extern "C" int func_002BAC90(cMusicSys* musicSys) {
    return (musicSys->mCurrentSongNum);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC98);
#ifdef SKIP_ASM
extern "C" void func_002B2850(int, void *);

extern "C" void func_002BAC98(void *arg0) {
    func_002B2850((*(int *)((char*)(arg0) + (0x408))), arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACB8);
#ifdef SKIP_ASM
extern "C" int func_002BACB8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x418)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACC0);
#ifdef SKIP_ASM
extern "C" void func_002BACC0(void *arg0) {
    (*(int *)((char*)(arg0) + (0x418))) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACC8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_002BACC8(void *arg0) {
    (*(long *)((char*)(arg0) + (0x400))) = (long) (*(long *)((char*)(arg0) + (0x3F8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACD8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long func_002BACD8(void *arg0) {
    return (*(long *)((char*)(arg0) + (0x400)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACE0);
#ifdef SKIP_ASM
extern "C" void func_002BACE0(cMusicSys* musicSys, int arg1) {
    (musicSys->mCurrentSongNum) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACE8);
#ifdef SKIP_ASM
extern "C" int func_002BACE8(cMusicSys* musicSys) {
    return (*(int *)((char*)(musicSys) + (0x41C)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACF0);
#ifdef SKIP_ASM
extern "C" int func_002BACF0(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACF8);
#ifdef SKIP_ASM
extern "C" int func_002BACF8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD00);
#ifdef SKIP_ASM
extern "C" char* func_002BAD00(cSpeechSys* speechSys, int i) {
    int off = i * 0x14 + 0x158;
    return ((char*)speechSys) + off;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD18);
#ifdef SKIP_ASM
extern "C" char* func_002BAD18(cCrowdSys* p, int i) {
    int off = i * 0x54 + 0x1C;
    return ((char*)p) + off;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD30);
#ifdef SKIP_ASM
extern "C" void func_002BAD30(void *p) {
    *(int*)((char*)p + 0x5790) = 0;
    *(int*)((char*)p + 0x578C) = 1;
    *(int*)((char*)p + 0x6254) = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD48);
#ifdef SKIP_ASM
extern "C" int func_002BAD48(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x578C)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD50);
#ifdef SKIP_ASM
extern "C" int func_002BAD50(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x5790)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD58);
#ifdef SKIP_ASM
extern "C" void func_002BAD58(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x598C))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD60);
#ifdef SKIP_ASM
extern "C" int func_002BAD60(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x5FB0)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD68);
#ifdef SKIP_ASM
extern "C" int func_002BAD68(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x608C)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD70);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long func_002BAD70(void *arg0) {
    return (*(long *)((char*)(arg0) + (0x6098)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD78);
#ifdef SKIP_ASM
extern "C" int func_002BAD78(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6234)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD80);
#ifdef SKIP_ASM
extern "C" char* func_002BAD80(char *p, int i) {
    int off = i * 0x14 + 0x60A4;
    return p + off;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD98);
#ifdef SKIP_ASM
extern "C" int func_002BAD98(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6238)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADA0);
#ifdef SKIP_ASM
extern "C" int func_002BADA0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6280)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADA8);
#ifdef SKIP_ASM
extern "C" int func_002BADA8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x62B0)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADB0);
#ifdef SKIP_ASM
extern "C" int func_002BADB0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x62B8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADB8);
#ifdef SKIP_ASM
extern "C" int func_002BADB8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x62B4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADC0);
#ifdef SKIP_ASM
extern "C" int func_002BADC0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6470)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADC8);
#ifdef SKIP_ASM
extern "C" int func_002BADC8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6474)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADD0);
#ifdef SKIP_ASM
struct Item_ADD0 { Item_ADD0* next; int pad; int type; };
struct List_ADD0 { int count; Item_ADD0* items[1]; };
struct Node_ADD0 { Node_ADD0* child[8]; int pad[2]; Item_ADD0* items; };

extern "C" void func_002BADD0(Node_ADD0* node, List_ADD0** out) {
    Item_ADD0* it;
    for (it = node->items; it != 0; it = it->next) {
        if (it->type == 5) {
            List_ADD0* l = *out;
            l->items[l->count++] = it;
        }
    }
    if (node->child[0]) func_002BADD0(node->child[0], out);
    if (node->child[1]) func_002BADD0(node->child[1], out);
    if (node->child[2]) func_002BADD0(node->child[2], out);
    if (node->child[3]) func_002BADD0(node->child[3], out);
    if (node->child[4]) func_002BADD0(node->child[4], out);
    if (node->child[5]) func_002BADD0(node->child[5], out);
    if (node->child[6]) func_002BADD0(node->child[6], out);
    if (node->child[7]) func_002BADD0(node->child[7], out);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAEE8);
#ifdef SKIP_ASM
extern "C" void func_002B8818(int, int);

extern "C" void func_002BAEE8(void) {
    func_002B8818(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterLayer_resolve);
#ifdef SKIP_ASM
void func_002C17C0(void *);
struct E_AF08 { int a; int off; };
extern "C" void cWorldPainterLayer_resolve(cWorldPainterLayer* self) {
    *(char**)self = ((char*)self) + *(int*)self;
    func_002C17C0(*(char**)self);
    *(char**)(((char*)self) + 8) = ((char*)self) + *(int*)(((char*)self) + 8);
    E_AF08 *e = *(E_AF08**)(((char*)self) + 8);
    int i;
    for (i = 0; i < self->m_iNumRegions; i++, e++) {
        int off = e->off;
        if (off != -1) {
            e->off = (int)(((char*)self) + off);
        } else {
            e->off = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAF90);
#ifdef SKIP_ASM
extern "C" void *cQuadTree_getLeaf(int);

extern "C" int func_002BAF90(int *arg0) {
    return (*(int *)((char*)(cQuadTree_getLeaf(*arg0)) + (4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAFB0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Placement new for constructing the key table in place.
inline void* operator new(unsigned int, void* p) { return p; }

// 8-byte key: a byte id at offset 0, overlaid by the packed word id | (index << 8).
struct sKey_AFB0 {
    union {
        unsigned char id;
        int packed;
    };
    int pad;

    sKey_AFB0(int n)
    {
        id = n;
        packed = (packed & 0xFF) | (n << 8);
    }
};

// Zeroed 8-byte header placed before the key table.
struct sPair_AFB0 {
    int a;
    int b;

    sPair_AFB0()
    {
        b = 0;
        a = 0;
    }
};

extern sPair_AFB0 D_004A52E0;
extern sKey_AFB0 D_004A52E8;
extern sKey_AFB0 D_004A52F0;
extern sKey_AFB0 D_004A52F8;
extern sKey_AFB0 D_004A5300;
extern sKey_AFB0 D_004A5308;
extern sKey_AFB0 D_004A5310;
extern sKey_AFB0 D_004A5318;
extern sKey_AFB0 D_004A5320;
extern sKey_AFB0 D_004A5328;
extern sKey_AFB0 D_004A5330;
extern sKey_AFB0 D_004A5338;
extern sKey_AFB0 D_004A5340;
extern sKey_AFB0 D_004A5348;
extern sKey_AFB0 D_004A5350;

// Static initializer (initialize, priority) for the key table.
extern "C" void func_002BAFB0(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004A52E0) sPair_AFB0();
        new (&D_004A52E8) sKey_AFB0(0);
        new (&D_004A52F0) sKey_AFB0(1);
        new (&D_004A52F8) sKey_AFB0(2);
        new (&D_004A5300) sKey_AFB0(3);
        new (&D_004A5308) sKey_AFB0(4);
        new (&D_004A5310) sKey_AFB0(5);
        new (&D_004A5318) sKey_AFB0(6);
        new (&D_004A5320) sKey_AFB0(7);
        new (&D_004A5328) sKey_AFB0(8);
        new (&D_004A5330) sKey_AFB0(9);
        new (&D_004A5338) sKey_AFB0(10);
        new (&D_004A5340) sKey_AFB0(11);
        new (&D_004A5348) sKey_AFB0(12);
        new (&D_004A5350) sKey_AFB0(13);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/seg_1BA100", func_002BB0E0);
#ifdef SKIP_ASM
extern "C" void func_002BAFB0(int, int);

extern "C" void func_002BB0E0(void) {
    func_002BAFB0(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB100);
#ifdef SKIP_ASM
extern "C" int func_002BB100(tWPNameField* arg0, void *arg1) {
    int var_6;

    var_6 = 0;
    if (((arg0->m_nameId[0]) != (*(int *)((char*)(arg1) + (0)))) || ((arg0->m_nameId[1]) != (*(int *)((char*)(arg1) + (4))))) {
        var_6 = 1;
    }
    return var_6;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_cWorldPainterMan);
#ifdef SKIP_ASM
/**
 * Constructs the world painter manager: clears the word at +0x4.
 *
 * @return @p self
 */
extern "C" void *cWorldPainterMan_cWorldPainterMan(void *self) {
    (*(int *)((char*)(self) + (4))) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan__cWorldPainterMan);
#ifdef SKIP_ASM
void cMemMan_free(void *);
void operator_delete(int *);
extern "C" void cWorldPainterSection__cWorldPainterSection(cWorldPainterSection*, int);
struct S12_B140 { int a, b, c; };
extern "C" void cWorldPainterMan__cWorldPainterMan(cWorldPainterMan* self, int flags) {
    S12_B140 *arr = (S12_B140*)self->m_pCache;
    if (arr != 0) {
        S12_B140 *p = arr + *(int*)((char*)arr - 0x10);
        if (arr != p) {
            do {
                p--;
                cWorldPainterSection__cWorldPainterSection((cWorldPainterSection*)p, 0);
            } while ((S12_B140*)self->m_pCache != p);
        }
        cMemMan_free((char*)(S12_B140*)self->m_pCache - 0x10);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_002BB370(void *arg0);
int operator_new_B1D0(unsigned int, void*, int, int) __asm__("operator_new__FUi");
extern char D_00483C68[];
struct sArr_B1D0 { int n; S12_B140 *arr; };

// Reallocates the 12-byte element array (delete[] then new[] with a count cookie).
extern "C" void cWorldPainterMan_init(cWorldPainterMan* self, int n) {
    if ((S12_B140*)self->m_pCache != 0) {
        S12_B140 *p = (S12_B140*)self->m_pCache + *(int*)((char*)(S12_B140*)self->m_pCache - 0x10);
        if ((S12_B140*)self->m_pCache != p) {
            do {
                p--;
                cWorldPainterSection__cWorldPainterSection((cWorldPainterSection*)p, 0);
            } while ((S12_B140*)self->m_pCache != p);
        }
        cMemMan_free((char*)(S12_B140*)self->m_pCache - 0x10);
    }
    self->m_uiNumSections = (unsigned int)n;
    S12_B140 **dst = (S12_B140**)&self->m_pCache;
    int *blk = (int*)operator_new_B1D0(n * 12 + 0x10, D_00483C68, 0x20000000, 0);
    blk[0] = n;
    S12_B140 *a = (S12_B140*)(blk + 4);
    S12_B140 *q = a;
    int i;
    for (i = n - 1; i != -1; i--) {
        func_002BB370(q);
        q++;
    }
    *dst = a;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_update);
#ifdef SKIP_ASM
extern "C" void func_002BB3F0(cWorldPainterSection*);
extern "C" void cWorldPainterMan_update(cWorldPainterMan* self) {
    unsigned i;
    for (i = 0; i < *(unsigned*)self; i++) {
        func_002BB3F0((cWorldPainterSection*)((char*)(self->m_pCache) + i * 12));
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB318);
#ifdef SKIP_ASM
extern "C" void func_002BB4D8(int, int);

extern "C" void func_002BB318(cWorldPainterMan* arg0, int arg1, int arg2) {
    func_002BB4D8(((int)arg0->m_pCache) + (arg1 * 0xC), arg2);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB348);
#ifdef SKIP_ASM
extern "C" void func_002BB570(void*, int, int);
extern "C" void func_002BB348(cWorldPainterMan* arg0, int arg1, int arg2) {
    func_002BB570((char*)arg0->m_pCache + arg1 * 12, arg1, arg2);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB370);
#ifdef SKIP_ASM
extern "C" void *func_002BB370(void *arg0) {
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(int *)((char*)(arg0) + (4))) = -1;
    (*(short *)((char*)(arg0) + (0))) = 0;
    (*(short *)((char*)(arg0) + (2))) = 0;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterSection__cWorldPainterSection);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void cWorldPainterSection__cWorldPainterSection(cWorldPainterSection* self, int flags) {
    void *p = self->m_pWPHeader;
    if (p != 0) {
        if (self->m_iStreamed == 0) {
            cMemMan_free(p);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB3F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void ASYNCFILE_release_B3F0(int, int *, int *) __asm__("ASYNCFILE_release");
extern "C" void MEM_free_B3F0(int) __asm__("MEM_free");
extern "C" void func_003B47F8_B3F0(int, int) __asm__("func_003B47F8");
extern "C" int func_003B4818_B3F0(int) __asm__("func_003B4818");
extern "C" int ASYNCFILE_getstatus_B3F0(int) __asm__("ASYNCFILE_getstatus");
extern "C" void MEM_copy_B3F0(int, int, int) __asm__("MEM_copy");
int operator_new_B3F0(unsigned int, void*, int, int) __asm__("operator_new__FUi");
extern char D_004A3868[];
extern "C" void func_002BB3F0(cWorldPainterSection* self_) {
    short st = *(short*)self_;
    if (st == 1 && ASYNCFILE_getstatus_B3F0(self_->m_iAsyncHandle) == st) {
        int sp0 = 0;
        int sp4 = 0;
        ASYNCFILE_release_B3F0(self_->m_iAsyncHandle, &sp0, &sp4);
        if (sp0 != 0) {
            if (sp4 > 0) {
                int t = func_003B4818_B3F0(sp0);
                if (t > 0) {
                    int r = operator_new_B3F0(t, D_004A3868, 0x20000000, 0);
                    self_->m_pWPHeader = (tWPHeader*)r;
                    func_003B47F8_B3F0(sp0, r);
                } else {
                    int r = operator_new_B3F0(sp4, D_004A3868, 0x20000000, 0);
                    self_->m_pWPHeader = (tWPHeader*)r;
                    MEM_copy_B3F0(r, sp0, sp4);
                }
                MEM_free_B3F0(sp0);
            }
        }
        *(short*)self_ = 2;
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB4D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sBXStr_B4D8 { char *p; int pad[3]; sBXStr_B4D8() {} };
extern "C" void func_003186D0_B4D8(sBXStr_B4D8*, const char*, int) __asm__("func_003186D0");
extern "C" void func_00318630_B4D8(sBXStr_B4D8*, sBXStr_B4D8*, const char*) __asm__("func_00318630");
extern "C" int func_003DF690_B4D8(const char*, int) __asm__("func_003DF690");
extern "C" void cBXString_dt_B4D8(sBXStr_B4D8*, int) __asm__("cBXString__cBXString");
extern char D_004A3870[];
extern char D_004A3878[];
extern "C" void func_002BB4D8_B4D8(void *self, int arg) __asm__("func_002BB4D8");
extern "C" void func_002BB4D8_B4D8(void *self, int arg) {
    sBXStr_B4D8 a;
    sBXStr_B4D8 b;
    *(short*)((char*)self + 2) = 0;
    func_003186D0_B4D8(&b, D_004A3870, arg);
    func_00318630_B4D8(&a, &b, D_004A3878);
    *(int*)((char*)self + 4) = func_003DF690_B4D8(a.p, 0);
    cBXString_dt_B4D8(&a, 2);
    cBXString_dt_B4D8(&b, 2);
    int n = *(int*)((char*)self + 4);
    *(short*)((char*)self + 0) = n <= 0 ? 2 : 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB570);
#ifdef SKIP_ASM
extern "C" void func_002BB5F0(cWorldPainterSection*);

extern "C" void func_002BB570(void *arg0, int arg1, int arg2) {
    (*(short *)((char*)(arg0) + (2))) = 1;
    (*(short *)((char*)(arg0) + (0))) = 2;
    (*(int *)((char*)(arg0) + (8))) = arg2;
    func_002BB5F0((cWorldPainterSection*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB5A8);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv(int);

extern "C" void func_002BB5A8(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (8)));
    if (temp_4 != 0) {
        if ((*(short *)((char*)(arg0) + (2))) == 0) {
            cMemMan_free__FPv(temp_4);
        }
        (*(int *)((char*)(arg0) + (8))) = 0;
    }
    (*(short *)((char*)(arg0) + (0))) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB5F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cWorldPainterLayer_resolve_B5F0(int p) __asm__("cWorldPainterLayer_resolve");

struct sReloc_B5F0 {
    int magic;          // 0x0 (0x10 once relocated)
    int count;          // 0x4
    int offs[1];        // 0x8
};

struct sRes_B5F0 {
    short state;        // 0x0
    short keep;         // 0x2
    int pad4;
    sReloc_B5F0* data;  // 0x8
};

// Relocates the loaded block's offset table (dropping it if it has the wrong magic) and marks it ready.
extern "C" void func_002BB5F0(cWorldPainterSection* vself)
{
    sRes_B5F0* self = (sRes_B5F0*)vself;
    if (self->data == 0) {
        self->state = 3;
        return;
    }
    {
        if (self->data->magic != 0x10) {
            if (self->keep == 0) {
                cMemMan_free(self->data);
            }
            self->data = 0;
        }
        if (self->data != 0) {
            int* p = self->data->offs;
            for (int i = 0; i < self->data->count; i++, p++) {
                if (*p == -1) {
                    *p = 0;
                } else {
                    *p = (int)self->data + *p;
                    cWorldPainterLayer_resolve_B5F0(*p);
                }
            }
        }
    }
    self->state = 3;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB6B8);
#ifdef SKIP_ASM
extern "C" void func_002BB6E0(int, void *);

extern "C" void func_002BB6B8(void) {
    void *temp_5;

    temp_5 = ((void*)SSXApp->Game->World);
    func_002BB6E0((*(int *)((char*)(temp_5) + (8))), temp_5);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB6E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cWorldPainterMan::render() (On Tour name and locals): draws the active painter layer's quad tree.
#define MIN_B6E0(a, b) ((a) < (b) ? (a) : (b))
#define MAX_B6E0(a, b) ((a) > (b) ? (a) : (b))
struct cGraphicsMan_B6E0 { char pad[0xE84]; sDrawState* m_pCurrentDrawState; };
struct tBxVertex_B6E0 {
    tBxVertex_B6E0() {}
    float x, y, z, w;
    int r, g, b, a;
    int pad[4];
};
// t4Vector with On Tour's t4vect accessors (x() ... w() return float&); the ctor stores through them.
struct t4Vector_B6E0 { float m_x, m_y, m_z, m_w;
    float& x() { return m_x; } float& y() { return m_y; } float& z() { return m_z; } float& w() { return m_w; }
    t4Vector_B6E0() {}
    t4Vector_B6E0(float ix, float iy, float iz, float iw) { x() = ix; y() = iy; z() = iz; w() = iw; } } __attribute__((aligned(16)));
struct cQuadTree_B6E0 { float scale; float minx; float miny; char pad0C[8]; unsigned short root; char pad16[10]; char* nodes; };
struct tWPLayer_B6E0 { cQuadTree_B6E0* tree; };
struct tWPHeader_B6E0 { int pad0; int numLayers; tWPLayer_B6E0* layers[1]; };
struct cWorldPainterSection_B6E0 { int pad0; int pad4; tWPHeader_B6E0* m_pWPHeader; };
struct cWorldPainterMan_B6E0 { unsigned int m_uiNumSections; cWorldPainterSection_B6E0* m_pCache; };

extern int D_004A44B4;   // cVisualEffectsTest::m_Enable
extern int D_004A3860;   // cWorldPainterMan::m_iCurrSection
extern int D_004A44B8;   // cVisualEffectsTest::m_LayerId
static inline int layerId_B6E0() { return D_004A44B8; }
extern sDrawState D_00501420;  // sDrawState::s_defaultDrawState
// cWorldPainterMan::render(cQuadTree*, const tQuadTreeNode*, tBxVertex*, t4Vector&, t4Vector&)
extern "C" void func_002BB928_B6E0(cWorldPainterMan_B6E0*, cQuadTree_B6E0*, char*, tBxVertex_B6E0*, t4Vector_B6E0*, t4Vector_B6E0*) __asm__("func_002BB928");

extern "C" void func_002BB6E0_impl(cWorldPainterMan_B6E0* self) __asm__("func_002BB6E0");
extern "C" void func_002BB6E0_impl(cWorldPainterMan_B6E0* self) {
    if (D_004A44B4) {
        cWorldPainterSection_B6E0* section = &self->m_pCache[D_004A3860];
        if (section) {
            tWPLayer_B6E0** layers = section->m_pWPHeader->layers;
            if (layers) {
                tWPLayer_B6E0* layer = layers[MIN_B6E0(MAX_B6E0(layerId_B6E0(), 0), section->m_pWPHeader->numLayers - 1)];
                if (layer) {
                    cQuadTree_B6E0* tree = layer->tree;
                    if (tree) {
                        cPS2GraphicsMan* gm = cGraphicsMan_TheGraphicsMan;
                        // PushDrawState + SetDefaultDrawState
                        (gm->m_pCurrentDrawState)[1] = (gm->m_pCurrentDrawState)[0];
                        (*(sDrawState**)&gm->m_pCurrentDrawState)++;
                        *(gm->m_pCurrentDrawState) = D_00501420;
                        tBxVertex_B6E0 vertlist[5];
                        tBxVertex_B6E0* v;
                        int i;
                        for (i = 0; i < 5; i++) {
                            v = &vertlist[i];
                            v->a = 255;
                            v->r = 0;
                            v->g = 255;
                            v->b = 0;
                            v->z = 1.0f;
                            v->w = 0;
                            v->x = 0;
                            v->y = 0;
                        }
                        t4Vector_B6E0 mincorner((float)(int)tree->minx, (float)(int)tree->miny, 0.0f, 1.0f);
                        t4Vector_B6E0 maxcorner((float)(int)(32768.0f / tree->scale + tree->minx),
                                                (float)(int)(32768.0f / tree->scale + tree->miny), 0.0f, 1.0f);
                        func_002BB928_B6E0(self, tree, tree->nodes + tree->root * 8, vertlist, &mincorner, &maxcorner);
                        (*(sDrawState**)&gm->m_pCurrentDrawState)--;  // PopDrawState
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB928);
#ifdef SKIP_ASM
// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).
struct sQtVec4_B928 {
    float x, y, z, w;
    sQtVec4_B928() {}
    sQtVec4_B928(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sQtVtx_B928 {
    char pad_0x0[0x20];
    sQtVec4_B928 pos;   // 0x20
};

struct sQtNode_B928 {
    unsigned short child[4];
};

struct sQtTree_B928 {
    char pad_0x0[0x20];
    sQtNode_B928* nodes;    // 0x20
};

struct sQtVEntry_B928 {
    short delta;
    short index;
    void (*fn)(void*, int, sQtVtx_B928*, int);
};

struct sQtCtx_B928 {
    char pad_0x0[0x10D8];
    sQtVEntry_B928* vtable;      // 0x10D8
};

class cQtCam_B928 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual sQtVec4_B928* v05();
};


// PORT: PS2-only VU0 inline asm (a + b).
static inline sQtVec4_B928 qtAdd_B928(const sQtVec4_B928& a, const sQtVec4_B928& b)
{
    sQtVec4_B928 r;
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

// PORT: PS2-only VU0 inline asm (v / s).
static inline sQtVec4_B928 qtDiv_B928(const sQtVec4_B928& v, float s)
{
    sQtVec4_B928 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// Walks a quadtree: inner nodes split the rect (lo, hi) at its midpoint into four children;
// leaves submit the rect's outline as a 5-vertex strip at the camera's height.
extern "C" void func_002BB928(cWorldPainterMan* ctx, sQtTree_B928* tree, sQtNode_B928* node, sQtVtx_B928* v,
                              sQtVec4_B928* lo, sQtVec4_B928* hi)
{
    if (node->child[0] & 1) {
        sQtVec4_B928 mid = qtDiv_B928(qtAdd_B928(*lo, *hi), 2.0f);
        sQtVec4_B928 a;
        a.x = lo->x;
        a.y = mid.y;
        a.z = 0.0f;
        a.w = 1.0f;
        sQtVec4_B928 b;
        b.x = hi->x;
        b.y = mid.y;
        b.z = 0.0f;
        b.w = 1.0f;
        sQtVec4_B928 c;
        c.x = mid.x;
        c.y = lo->y;
        c.z = 0.0f;
        c.w = 1.0f;
        sQtVec4_B928 d;
        d.x = mid.x;
        d.y = hi->y;
        d.z = 0.0f;
        d.w = 1.0f;
        func_002BB928(ctx, tree, tree->nodes + (node->child[0] >> 1), v, lo, &mid);
        func_002BB928(ctx, tree, tree->nodes + (node->child[1] >> 1), v, &a, &d);
        func_002BB928(ctx, tree, tree->nodes + (node->child[2] >> 1), v, &c, &b);
        func_002BB928(ctx, tree, tree->nodes + (node->child[3] >> 1), v, &mid, hi);
    } else {
        cPS2GraphicsMan* g = cGraphicsMan_TheGraphicsMan;
        char* rider = *(char**)(*(char**)((char*)SSXApp->Game->AI + 0x40) + 0x18);
        sQtVec4_B928 cam = *((cQtCam_B928*)(rider + 0x6C0))->v05();
        sQtVec4_B928 t;
        t.x = lo->x;
        t.y = lo->y;
        t.z = cam.z;
        t.w = 1.0f;
        v[0].pos = t;
        t.x = hi->x;
        t.y = lo->y;
        t.z = cam.z;
        t.w = 1.0f;
        v[1].pos = t;
        t.x = hi->x;
        t.y = hi->y;
        t.z = cam.z;
        t.w = 1.0f;
        v[2].pos = t;
        t.x = lo->x;
        t.y = hi->y;
        t.z = cam.z;
        t.w = 1.0f;
        v[3].pos = t;
        t.x = lo->x;
        t.y = lo->y;
        t.z = cam.z;
        t.w = 1.0f;
        v[4].pos = t;
        sQtVEntry_B928* vt = (sQtVEntry_B928*)g->mVtbl;
        vt[73].fn((char*)g + vt[73].delta, 5, v, 0);
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BBBF8);
#ifdef SKIP_ASM
extern "C" int func_002BBBF8(int a, ...) {
    return a;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BBC38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColor_2BBC38 {
    float r, g, b, a;
    sColor_2BBC38(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};


struct sFlags_2BBC38 {
    unsigned char a;
    unsigned int b : 24;
    sFlags_2BBC38(unsigned char aa, unsigned int ab) : a(aa), b(ab) {}
};

extern int D_004A5358;
extern int D_004A535C;
extern sFlags_2BBC38 D_004A5360;
extern sFlags_2BBC38 D_004A5368;
extern sFlags_2BBC38 D_004A5370;
extern sFlags_2BBC38 D_004A5378;
extern sFlags_2BBC38 D_004A5380;
extern sFlags_2BBC38 D_004A5388;
extern sFlags_2BBC38 D_004A5390;
extern sFlags_2BBC38 D_004A5398;
extern sFlags_2BBC38 D_004A53A0;
extern sFlags_2BBC38 D_004A53A8;
extern sFlags_2BBC38 D_004A53B0;
extern sFlags_2BBC38 D_004A53B8;
extern sFlags_2BBC38 D_004A53C0;
extern sFlags_2BBC38 D_004A53C8;

extern sColor_2BBC38 D_004D42B8;
extern sColor_2BBC38 D_004D42C8;
extern sColor_2BBC38 D_004D42D8;
extern sColor_2BBC38 D_004D42E8;
extern sColor_2BBC38 D_004D42F8;
extern sColor_2BBC38 D_004D4308;
extern sColor_2BBC38 D_004D4318;
extern sColor_2BBC38 D_004D4328;
extern sColor_2BBC38 D_004D4338;
extern sColor_2BBC38 D_004D4348;
extern sColor_2BBC38 D_004D4358;
extern sColor_2BBC38 D_004D4368;
extern sColor_2BBC38 D_004D4378;
extern sColor_2BBC38 D_004D4388;
extern sColor_2BBC38 D_004D4398;
extern sColor_2BBC38 D_004D43A8;
extern sColor_2BBC38 D_004D43B8;
extern sColor_2BBC38 D_004D43C8;
extern sColor_2BBC38 D_004D43D8;
extern sColor_2BBC38 D_004D43E8;
extern sColor_2BBC38 D_004D43F8;
extern sColor_2BBC38 D_004D4408;
extern sColor_2BBC38 D_004D4418;
extern sColor_2BBC38 D_004D4428;
extern sColor_2BBC38 D_004D4438;
extern sColor_2BBC38 D_004D4448;
extern sColor_2BBC38 D_004D4458;
extern sColor_2BBC38 D_004D4468;
extern sColor_2BBC38 D_004D4478;
extern sColor_2BBC38 D_004D4488;
extern sColor_2BBC38 D_004D4498;
extern sColor_2BBC38 D_004D44A8;
extern sColor_2BBC38 D_004D44B8;
extern sColor_2BBC38 D_004D44C8;
extern sColor_2BBC38 D_004D44D8;
extern sColor_2BBC38 D_004D44E8;
extern sColor_2BBC38 D_004D44F8;
extern sColor_2BBC38 D_004D4508;
extern sColor_2BBC38 D_004D4518;
extern sColor_2BBC38 D_004D4528;
extern sColor_2BBC38 D_004D4538;
extern sColor_2BBC38 D_004D4548;
extern sColor_2BBC38 D_004D4558;
extern sColor_2BBC38 D_004D4568;
extern sColor_2BBC38 D_004D4578;
extern sColor_2BBC38 D_004D4588;
extern sColor_2BBC38 D_004D4598;
extern sColor_2BBC38 D_004D45A8;
extern sColor_2BBC38 D_004D45B8;
extern sColor_2BBC38 D_004D45C8;
extern sColor_2BBC38 D_004D45D8;
extern sColor_2BBC38 D_004D45E8;
extern sColor_2BBC38 D_004D45F8;
extern sColor_2BBC38 D_004D4608;
extern sColor_2BBC38 D_004D4618;
extern sColor_2BBC38 D_004D4628;

// The unit declares func_002BBC38 later as returning void*; bind this body by asm label.
extern "C" void func_002BBC38_impl(int initialize, int priority) __asm__("func_002BBC38");

extern "C" void func_002BBC38_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        D_004A535C = 0;
        D_004A5358 = 0;
        new (&D_004A5360) sFlags_2BBC38(0, 0);
        new (&D_004A5368) sFlags_2BBC38(1, 1);
        new (&D_004A5370) sFlags_2BBC38(2, 2);
        new (&D_004A5378) sFlags_2BBC38(3, 3);
        new (&D_004A5380) sFlags_2BBC38(4, 4);
        new (&D_004A5388) sFlags_2BBC38(5, 5);
        new (&D_004A5390) sFlags_2BBC38(6, 6);
        new (&D_004A5398) sFlags_2BBC38(7, 7);
        new (&D_004A53A0) sFlags_2BBC38(8, 8);
        new (&D_004A53A8) sFlags_2BBC38(9, 9);
        new (&D_004A53B0) sFlags_2BBC38(10, 10);
        new (&D_004A53B8) sFlags_2BBC38(11, 11);
        new (&D_004A53C0) sFlags_2BBC38(12, 12);
        new (&D_004A53C8) sFlags_2BBC38(13, 13);
        new (&D_004D42B8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004D42C8) sColor_2BBC38(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004D42D8) sColor_2BBC38(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004D42E8) sColor_2BBC38(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004D42F8) sColor_2BBC38(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004D4308) sColor_2BBC38(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004D4318) sColor_2BBC38(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004D4328) sColor_2BBC38(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004D4338) sColor_2BBC38(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004D4348) sColor_2BBC38(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004D4358) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4368) sColor_2BBC38(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4378) sColor_2BBC38(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004D4388) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4398) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D43A8) sColor_2BBC38(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D43B8) sColor_2BBC38(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D43C8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D43D8) sColor_2BBC38(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D43E8) sColor_2BBC38(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D43F8) sColor_2BBC38(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D4408) sColor_2BBC38(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004D4418) sColor_2BBC38(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004D4428) sColor_2BBC38(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004D4438) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4448) sColor_2BBC38(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004D4458) sColor_2BBC38(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004D4468) sColor_2BBC38(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004D4478) sColor_2BBC38(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004D4488) sColor_2BBC38(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004D4498) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D44A8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D44B8) sColor_2BBC38(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004D44C8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D44D8) sColor_2BBC38(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004D44E8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D44F8) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4508) sColor_2BBC38(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4518) sColor_2BBC38(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004D4528) sColor_2BBC38(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004D4538) sColor_2BBC38(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004D4548) sColor_2BBC38(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004D4558) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4568) sColor_2BBC38(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004D4578) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4588) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4598) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D45A8) sColor_2BBC38(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004D45B8) sColor_2BBC38(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004D45C8) sColor_2BBC38(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004D45D8) sColor_2BBC38(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004D45E8) sColor_2BBC38(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004D45F8) sColor_2BBC38(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004D4608) sColor_2BBC38(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004D4618) sColor_2BBC38(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004D4628) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/seg_1BA100", func_002BC4E0);
#ifdef SKIP_ASM
extern "C" void func_002BBC38(int, int);

extern "C" void func_002BC4E0(void) {
    func_002BBC38(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC500);
#ifdef SKIP_ASM
extern "C" int func_002BC500(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC508);
#ifdef SKIP_ASM
extern "C" int func_002BC508(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC510);
#ifdef SKIP_ASM
extern "C" int func_002BC510(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC518);
#ifdef SKIP_ASM
extern "C" int func_002BC518(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC520);
#ifdef SKIP_ASM
extern "C" int func_002BC520(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC528);
#ifdef SKIP_ASM
extern "C" int func_002BC528(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC530);
#ifdef SKIP_ASM
extern "C" int func_002BC530(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC538);
#ifdef SKIP_ASM
extern "C" int func_002BC538(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC540);
#ifdef SKIP_ASM
extern "C" int func_002BC540(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC548);
#ifdef SKIP_ASM
extern "C" int func_002BC548(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC550);
#ifdef SKIP_ASM
extern "C" int func_002BC550(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC558);
#ifdef SKIP_ASM
extern "C" int func_002BC558(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC560);
#ifdef SKIP_ASM
extern "C" int func_002BC560(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC568);
#ifdef SKIP_ASM
extern "C" int func_002BC568(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC570);
#ifdef SKIP_ASM
extern "C" int func_002BC570(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC578);
#ifdef SKIP_ASM
extern "C" int func_002BC578(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC580);
#ifdef SKIP_ASM
extern "C" int func_002BC580(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC588);
#ifdef SKIP_ASM
extern "C" int func_002BC588(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC590);
#ifdef SKIP_ASM
extern "C" int func_002BC590(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC598);
#ifdef SKIP_ASM
extern "C" int func_002BC598(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5A0);
#ifdef SKIP_ASM
extern "C" int func_002BC5A0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5A8);
#ifdef SKIP_ASM
extern "C" int func_002BC5A8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5B0);
#ifdef SKIP_ASM
extern "C" int func_002BC5B0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5B8);
#ifdef SKIP_ASM
extern "C" int func_002BC5B8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5C0);
#ifdef SKIP_ASM
extern "C" int func_002BC5C0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5C8);
#ifdef SKIP_ASM
extern "C" int func_002BC5C8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5D0);
#ifdef SKIP_ASM
extern "C" int func_002BC5D0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5D8);
#ifdef SKIP_ASM
extern "C" int func_002BC5D8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5E0);
#ifdef SKIP_ASM
extern "C" int func_002BC5E0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5E8);
#ifdef SKIP_ASM
extern "C" int func_002BC5E8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5F0);
#ifdef SKIP_ASM
extern "C" int func_002BC5F0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5F8);
#ifdef SKIP_ASM
extern "C" int func_002BC5F8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC600);
#ifdef SKIP_ASM
extern "C" int func_002BC600(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC608);
#ifdef SKIP_ASM
extern "C" int func_002BC608(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC610);
#ifdef SKIP_ASM
extern "C" int func_002BC610(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC618);
#ifdef SKIP_ASM
extern "C" int func_002BC618(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC620);
#ifdef SKIP_ASM
extern "C" int func_002BC620(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC628);
#ifdef SKIP_ASM
extern "C" int func_002BC628(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC630);
#ifdef SKIP_ASM
extern "C" int func_002BC630(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC638);
#ifdef SKIP_ASM
extern "C" int func_002BC638(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC640);
#ifdef SKIP_ASM
extern "C" int func_002BC640(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC648);
#ifdef SKIP_ASM
extern "C" int func_002BC648(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC650);
#ifdef SKIP_ASM
extern "C" int func_002BC650(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC658);
#ifdef SKIP_ASM
extern "C" int func_002BC658(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC660);
#ifdef SKIP_ASM
extern "C" int func_002BC660(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC668);
#ifdef SKIP_ASM
extern "C" int func_002BC668(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC670);
#ifdef SKIP_ASM
extern "C" int func_002BC670(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC678);
#ifdef SKIP_ASM
extern "C" int func_002BC678(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC680);
#ifdef SKIP_ASM
extern "C" int func_002BC680(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC688);
#ifdef SKIP_ASM
extern "C" int func_002BC688(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC690);
#ifdef SKIP_ASM
extern "C" int func_002BC690(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC698);
#ifdef SKIP_ASM
extern "C" int func_002BC698(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6A0);
#ifdef SKIP_ASM
extern "C" int func_002BC6A0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6A8);
#ifdef SKIP_ASM
extern "C" int func_002BC6A8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6B0);
#ifdef SKIP_ASM
extern "C" int func_002BC6B0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6B8);
#ifdef SKIP_ASM
extern "C" int func_002BC6B8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6C0);
#ifdef SKIP_ASM
extern "C" int func_002BC6C0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6C8);
#ifdef SKIP_ASM
extern "C" int func_002BC6C8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6D0);
#ifdef SKIP_ASM
extern "C" int func_002BC6D0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6D8);
#ifdef SKIP_ASM
extern "C" int func_002BC6D8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6E0);
#ifdef SKIP_ASM
extern "C" int func_002BC6E0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6E8);
#ifdef SKIP_ASM
extern "C" int func_002BC6E8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6F0);
#ifdef SKIP_ASM
extern "C" int func_002BC6F0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6F8);
#ifdef SKIP_ASM
extern char D_00485AF8[];
extern "C" void *func_002BC6F8(tWPInGameData_MusicTrigger* arg0) {
    arg0->mVtbl = (void**)D_00485AF8;
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_lastType = -1;
    arg0->m_Type = -1;
    arg0->m_lastValue = -1;
    arg0->m_Value = -1;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC728);
#ifdef SKIP_ASM
extern char D_004858C0[];
extern "C" void *func_002BC728(tWPInGameData_Mix* arg0) {
    arg0->mVtbl = (void**)D_004858C0;
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_lastMixPreset = -1;
    arg0->m_MixPreset = -1;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC750);
#ifdef SKIP_ASM
extern char D_00485688[];
extern "C" void *func_002BC750(tWPInGameData_Ambience* arg0) {
    arg0->mVtbl = (void**)D_00485688;
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_lastBankID = -1;
    arg0->m_BankID = -1;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC778);
#ifdef SKIP_ASM
extern char D_00485450[];
extern "C" void *func_002BC778(tWPInGameData_Speech* arg0) {
    arg0->mVtbl = (void**)D_00485450;
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_lastSpeechID = -1;
    arg0->m_SpeechID = -1;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC7A0);
#ifdef SKIP_ASM
extern char D_00485218[];
extern "C" void *func_002BC7A0(tWPInGameData_Camera* arg0) {
    arg0->mVtbl = (void**)D_00485218;
    arg0->m_lastDrawDistance = 30000.0f;
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_DrawDistance = 30000.0f;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC7C8);
#ifdef SKIP_ASM
extern char D_00484FE0[];
extern "C" void *func_002BC7C8(tWPInGameData_Fog* arg0) {
    float zero = 0.0f;
    float a = 3000.0f, b = 30000.0f, c = 0.43f, d = 0.55f, e = 0.71f;
    arg0->mVtbl = (void**)((int*)D_00484FE0);
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_lastFogDensity = zero;
    arg0->m_lastFogNear = a;
    arg0->m_lastFogFar = b;
    arg0->m_lastFogColourR = c;
    arg0->m_lastFogColourG = d;
    arg0->m_lastFogColourB = e;
    arg0->m_FogDensity = zero;
    arg0->m_FogNear = a;
    arg0->m_FogFar = b;
    arg0->m_FogColourR = c;
    arg0->m_FogColourG = d;
    arg0->m_FogColourB = e;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_tWPInGameData_LightGlow);
#ifdef SKIP_ASM
extern char D_00484DA8[];
extern "C" void *tWPInGameData_LightGlow_tWPInGameData_LightGlow(tWPInGameData_LightGlow* self) {
    float one = 1.0f, zero = 0.0f;
    float *p = (float*)self;
    p[0] = -99999.0f;
    self->mVtbl = (void**)D_00484DA8;
    p[3] = one;
    p[2] = one;
    p[5] = one;
    p[4] = one;
    p[7] = one;
    p[6] = one;
    p[9] = one;
    p[8] = one;
    p[11] = one;
    p[10] = one;
    p[13] = zero;
    p[12] = zero;
    p[15] = zero;
    p[14] = zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_tWPInGameData_ScreenTint);
#ifdef SKIP_ASM
extern char D_00484B70[];
extern "C" void *tWPInGameData_ScreenTint_tWPInGameData_ScreenTint(tWPInGameData_ScreenTint* self) {
    float one = 1.0f, zero = 0.0f;
    float *p = (float*)self;
    p[0] = -99999.0f;
    self->mVtbl = (void**)D_00484B70;
    p[3] = one;
    p[2] = one;
    p[5] = one;
    p[4] = one;
    p[7] = one;
    p[6] = one;
    p[9] = zero;
    p[8] = zero;
    p[11] = zero;
    p[10] = zero;
    p[13] = zero;
    p[12] = zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC8E8);
#ifdef SKIP_ASM
extern char D_00484938[];
extern "C" void *func_002BC8E8(tWPInGameData_SkyBox* arg0) {
    float zero = 0.0f;
    arg0->mVtbl = (void**)D_00484938;
    arg0->m_lastSkyBoxRotation = zero;
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_SkyBoxRotation = zero;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_tWPInGameData_Sun);
#ifdef SKIP_ASM
extern char D_00484700[];
extern "C" void *tWPInGameData_Sun_tWPInGameData_Sun(tWPInGameData_Sun* self) {
    float one = 1.0f, zero = 0.0f;
    float *p = (float*)self;
    p[0] = -99999.0f;
    self->mVtbl = (void**)D_00484700;
    p[3] = zero;
    p[2] = zero;
    p[5] = zero;
    p[4] = zero;
    p[7] = one;
    p[6] = one;
    p[9] = one;
    p[8] = one;
    p[11] = one;
    p[10] = one;
    p[13] = one;
    p[12] = one;
    ((int*)p)[15] = 0;
    ((int*)p)[14] = 0;
    p[17] = one;
    p[16] = one;
    p[19] = one;
    p[18] = one;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC980);
#ifdef SKIP_ASM
extern char D_004844C8[];
extern "C" void *func_002BC980(tWPInGameData_Surface* arg0) {
    float one = 1.0f;
    arg0->mVtbl = (void**)D_004844C8;
    arg0->m_lastSnowSparkleDensity = one;
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_SnowSparkleDensity = one;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC9B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Query impl constructor: clears eight pairs, copies the default pair into each slot pair, sets weights.
struct sPair_002BC9B0
{
    int a;
    int b;
};
extern sPair_002BC9B0 D_004A53D0_2BC9B0 __asm__("D_004A53D0");
extern char D_00484290[];
struct sQuery_002BC9B0
{
    float minValue;         // 0x00
    char* vtable;           // 0x04
    sPair_002BC9B0 p[8];    // 0x08
    float w0;               // 0x48
    float w1;               // 0x4C
    float w2;               // 0x50
    float w3;               // 0x54
};
extern "C" void* func_002BC9B0(tWPInGameData_Lighting* o)
{
    o->mVtbl = (void**)D_00484290;
    o->m_wpTravelDistance = -99999.0f;
    ((sPair_002BC9B0*)&o->m_LightIrradianceName)[0].a = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[0].b = 0;
    ((sPair_002BC9B0*)&o->m_LightIrradianceName)[1].a = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[1].b = 0;
    ((sPair_002BC9B0*)&o->m_LightIrradianceName)[2].a = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[2].b = 0;
    ((sPair_002BC9B0*)&o->m_LightIrradianceName)[3].a = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[3].b = 0;
    ((sPair_002BC9B0*)&o->m_LightIrradianceName)[4].a = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[4].b = 0;
    ((sPair_002BC9B0*)&o->m_LightIrradianceName)[5].a = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[5].b = 0;
    ((sPair_002BC9B0*)&o->m_LightIrradianceName)[6].a = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[6].b = 0;
    ((sPair_002BC9B0*)&o->m_LightIrradianceName)[7].a = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[7].b = 0;
    { sPair_002BC9B0 t = D_004A53D0_2BC9B0; ((sPair_002BC9B0*)&o->m_LightIrradianceName)[0] = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[1] = t; }
    { sPair_002BC9B0 t = D_004A53D0_2BC9B0; ((sPair_002BC9B0*)&o->m_LightIrradianceName)[2] = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[3] = t; }
    { sPair_002BC9B0 t = D_004A53D0_2BC9B0; ((sPair_002BC9B0*)&o->m_LightIrradianceName)[4] = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[5] = t; }
    { sPair_002BC9B0 t = D_004A53D0_2BC9B0; ((sPair_002BC9B0*)&o->m_LightIrradianceName)[6] = ((sPair_002BC9B0*)&o->m_LightIrradianceName)[7] = t; }
    o->m_lastAmbientLightIntensityOffset = 1.0f;
    o->m_lastRimLightIntensityOffset = 0.75f;
    o->m_AmbientLightIntensityOffset = 1.0f;
    o->m_RimLightIntensityOffset = 0.75f;
    return ((sQuery_002BC9B0*)o);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BCAF8);
#ifdef SKIP_ASM
extern char D_00484058[];
#define F_CAF8(o) (*(float*)(self + (o)))
extern "C" void* func_002BCAF8(char *self) {
    *(char**)(self + 4) = D_00484058;
    F_CAF8(0) = -99999.0f;
    F_CAF8(0x8) = F_CAF8(0xC) = 0.0f;
    F_CAF8(0x10) = F_CAF8(0x14) = 6.0f;
    F_CAF8(0x18) = F_CAF8(0x1C) = 0.0f;
    F_CAF8(0x20) = F_CAF8(0x24) = 0.0f;
    F_CAF8(0x28) = F_CAF8(0x2C) = 0.0f;
    F_CAF8(0x30) = F_CAF8(0x34) = 10.0f;
    F_CAF8(0x38) = F_CAF8(0x3C) = 1.0f;
    F_CAF8(0x40) = F_CAF8(0x44) = 1.0f;
    F_CAF8(0x48) = F_CAF8(0x4C) = 0.0f;
    F_CAF8(0x50) = F_CAF8(0x54) = 0.0f;
    F_CAF8(0x58) = F_CAF8(0x5C) = 0.0f;
    F_CAF8(0x60) = F_CAF8(0x64) = 1.0f;
    F_CAF8(0x68) = F_CAF8(0x6C) = 1.0f;
    F_CAF8(0x70) = F_CAF8(0x74) = 1.0f;
    F_CAF8(0x78) = F_CAF8(0x7C) = 1.0f;
    F_CAF8(0x80) = F_CAF8(0x84) = 0.05999999865889549f;
    F_CAF8(0x88) = F_CAF8(0x8C) = 1.0f;
    F_CAF8(0x90) = F_CAF8(0x94) = 1.0f;
    F_CAF8(0x98) = F_CAF8(0x9C) = 1.0f;
    return self;
}
#undef F_CAF8
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BCBD0);
#ifdef SKIP_ASM
extern char D_00483E20[];
extern "C" void *func_002BCBD0(tWPInGameData_Danger* arg0) {
    float zero = 0.0f;
    arg0->mVtbl = (void**)D_00483E20;
    arg0->m_lastLevel = 0;
    arg0->m_Level = 0;
    arg0->m_wpTravelDistance = -99999.0f;
    arg0->m_lastPointRate = zero;
    arg0->m_PointRate = zero;
    return ((void*)arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_blend);
#ifdef SKIP_ASM
class cCC00 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_MusicTrigger_blend(tWPInGameData_MusicTrigger* self, void *p, float f) {
    cCC00 *o = (cCC00*)self;
    o->vt();
    f = f * f;
    self->m_Type = (int)(f * (float)*(int*)((char*)p + 4) + (1.0f - f) * (float)self->m_Type);
    self->m_lastType = *(int*)((char*)p + 4);
    self->m_Value = (int)(f * (float)*(int*)((char*)p + 8) + (1.0f - f) * (float)self->m_Value);
    self->m_lastValue = *(int*)((char*)p + 8);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Mix_blend);
#ifdef SKIP_ASM
class cCCC8 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_Mix_blend(tWPInGameData_Mix* self, void *p, float f) {
    cCCC8 *o = (cCCC8*)self;
    o->vt();
    f = f * f;
    self->m_MixPreset = (int)(f * (float)*(int*)((char*)p + 4) + (1.0f - f) * (float)self->m_MixPreset);
    self->m_lastMixPreset = *(int*)((char*)p + 4);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Ambience_blend);
#ifdef SKIP_ASM
class cCD68 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_Ambience_blend(tWPInGameData_Ambience* self, void *p, float f) {
    cCD68 *o = (cCD68*)self;
    o->vt();
    f = f * f;
    self->m_BankID = (int)(f * (float)*(int*)((char*)p + 4) + (1.0f - f) * (float)self->m_BankID);
    self->m_lastBankID = *(int*)((char*)p + 4);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Speech_blend);
#ifdef SKIP_ASM
class cCE08 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_Speech_blend(tWPInGameData_Speech* self, void *p, float f) {
    cCE08 *o = (cCE08*)self;
    o->vt();
    f = f * f;
    self->m_SpeechID = (int)(f * (float)*(int*)((char*)p + 4) + (1.0f - f) * (float)self->m_SpeechID);
    self->m_lastSpeechID = *(int*)((char*)p + 4);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Camera_blend);
#ifdef SKIP_ASM
class cCEA8 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_Camera_blend(tWPInGameData_Camera* self, void *p, float f) {
    cCEA8 *o = (cCEA8*)self;
    o->vt();
    f = f * f;
    self->m_DrawDistance = f * *(float*)((char*)p + 4) + (1.0f - f) * self->m_DrawDistance;
    self->m_lastDrawDistance = *(float*)((char*)p + 4);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_blend);
#ifdef SKIP_ASM
struct VEnt_CF38 { short delta; short idx; int (*fn)(void*); };
struct Pair_CF38 { float cur; float tgt; };
struct Obj_CF38 { int pad0; char* vt; Pair_CF38 p[6]; };
struct Src_CF38 { int pad0; float v[6]; };

extern "C" void tWPInGameData_Fog_blend(tWPInGameData_Fog* self, Src_CF38* src, float t) {
    VEnt_CF38* e;
    float inv;
    e = (VEnt_CF38*)((char*)self->mVtbl + 0x218);
    e->fn((char*)self + e->delta);
    t = t * t;
    inv = 1.0f - t;
    ((Pair_CF38*)&self->m_FogDensity)[0].cur = t * src->v[0] + inv * ((Pair_CF38*)&self->m_FogDensity)[0].cur;
    ((Pair_CF38*)&self->m_FogDensity)[0].tgt = src->v[0];
    ((Pair_CF38*)&self->m_FogDensity)[1].cur = t * src->v[1] + inv * ((Pair_CF38*)&self->m_FogDensity)[1].cur;
    ((Pair_CF38*)&self->m_FogDensity)[1].tgt = src->v[1];
    ((Pair_CF38*)&self->m_FogDensity)[2].cur = t * src->v[2] + inv * ((Pair_CF38*)&self->m_FogDensity)[2].cur;
    ((Pair_CF38*)&self->m_FogDensity)[2].tgt = src->v[2];
    ((Pair_CF38*)&self->m_FogDensity)[3].cur = t * src->v[3] + inv * ((Pair_CF38*)&self->m_FogDensity)[3].cur;
    ((Pair_CF38*)&self->m_FogDensity)[3].tgt = src->v[3];
    ((Pair_CF38*)&self->m_FogDensity)[4].cur = t * src->v[4] + inv * ((Pair_CF38*)&self->m_FogDensity)[4].cur;
    ((Pair_CF38*)&self->m_FogDensity)[4].tgt = src->v[4];
    ((Pair_CF38*)&self->m_FogDensity)[5].cur = t * src->v[5] + inv * ((Pair_CF38*)&self->m_FogDensity)[5].cur;
    ((Pair_CF38*)&self->m_FogDensity)[5].tgt = src->v[5];
    e = (VEnt_CF38*)((char*)self->mVtbl + 0x218);
    e->fn((char*)self + e->delta);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_blend);
#ifdef SKIP_ASM
struct VEnt_D068 { short delta; short idx; int (*fn)(void*); };
struct VEntV_D068 { short delta; short idx; void (*fn)(void*); };
struct Pair_D068 { float cur; float tgt; };
struct Obj_D068 { int pad0; char* vt; Pair_D068 p[7]; };
struct Src_D068 { int pad0; float v[7]; };

extern "C" void tWPInGameData_LightGlow_blend(tWPInGameData_LightGlow* self, Src_D068* src, float t) {
    VEnt_D068* e;
    float inv;
    e = (VEnt_D068*)((char*)self->mVtbl + 0x218);
    e->fn((char*)self + e->delta);
    t = t * t;
    inv = 1.0f - t;
    ((Pair_D068*)&self->m_LightGlowCutoff)[0].cur = t * src->v[0] + inv * ((Pair_D068*)&self->m_LightGlowCutoff)[0].cur;
    ((Pair_D068*)&self->m_LightGlowCutoff)[0].tgt = src->v[0];
    ((Pair_D068*)&self->m_LightGlowCutoff)[1].cur = t * src->v[1] + inv * ((Pair_D068*)&self->m_LightGlowCutoff)[1].cur;
    ((Pair_D068*)&self->m_LightGlowCutoff)[1].tgt = src->v[1];
    ((Pair_D068*)&self->m_LightGlowCutoff)[2].cur = t * src->v[2] + inv * ((Pair_D068*)&self->m_LightGlowCutoff)[2].cur;
    ((Pair_D068*)&self->m_LightGlowCutoff)[2].tgt = src->v[2];
    ((Pair_D068*)&self->m_LightGlowCutoff)[3].cur = t * src->v[3] + inv * ((Pair_D068*)&self->m_LightGlowCutoff)[3].cur;
    ((Pair_D068*)&self->m_LightGlowCutoff)[3].tgt = src->v[3];
    ((Pair_D068*)&self->m_LightGlowCutoff)[4].cur = t * src->v[4] + inv * ((Pair_D068*)&self->m_LightGlowCutoff)[4].cur;
    ((Pair_D068*)&self->m_LightGlowCutoff)[4].tgt = src->v[4];
    ((Pair_D068*)&self->m_LightGlowCutoff)[5].cur = t * src->v[5] + inv * ((Pair_D068*)&self->m_LightGlowCutoff)[5].cur;
    ((Pair_D068*)&self->m_LightGlowCutoff)[5].tgt = src->v[5];
    ((Pair_D068*)&self->m_LightGlowCutoff)[6].cur = t * src->v[6] + inv * ((Pair_D068*)&self->m_LightGlowCutoff)[6].cur;
    ((Pair_D068*)&self->m_LightGlowCutoff)[6].tgt = src->v[6];
    {
        VEntV_D068* e2 = (VEntV_D068*)((char*)self->mVtbl + 0x218);
        e2->fn((char*)self + e2->delta);
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_blend);
#ifdef SKIP_ASM
struct VEnt_D1B8 { short delta; short idx; int (*fn)(void*); };
struct Pair_D1B8 { float cur; float tgt; };
struct Obj_D1B8 { int pad0; char* vt; Pair_D1B8 p[6]; };
struct Src_D1B8 { int pad0; float v[6]; };

extern "C" void tWPInGameData_ScreenTint_blend(tWPInGameData_ScreenTint* self, Src_D1B8* src, float t) {
    VEnt_D1B8* e;
    float inv;
    e = (VEnt_D1B8*)((char*)self->mVtbl + 0x218);
    e->fn((char*)self + e->delta);
    t = t * t;
    inv = 1.0f - t;
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[0].cur = t * src->v[0] + inv * ((Pair_D1B8*)&self->m_ScreenTintScaleR)[0].cur;
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[0].tgt = src->v[0];
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[1].cur = t * src->v[1] + inv * ((Pair_D1B8*)&self->m_ScreenTintScaleR)[1].cur;
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[1].tgt = src->v[1];
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[2].cur = t * src->v[2] + inv * ((Pair_D1B8*)&self->m_ScreenTintScaleR)[2].cur;
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[2].tgt = src->v[2];
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[3].cur = t * src->v[3] + inv * ((Pair_D1B8*)&self->m_ScreenTintScaleR)[3].cur;
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[3].tgt = src->v[3];
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[4].cur = t * src->v[4] + inv * ((Pair_D1B8*)&self->m_ScreenTintScaleR)[4].cur;
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[4].tgt = src->v[4];
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[5].cur = t * src->v[5] + inv * ((Pair_D1B8*)&self->m_ScreenTintScaleR)[5].cur;
    ((Pair_D1B8*)&self->m_ScreenTintScaleR)[5].tgt = src->v[5];
    e = (VEnt_D1B8*)((char*)self->mVtbl + 0x218);
    e->fn((char*)self + e->delta);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SkyBox_blend);
#ifdef SKIP_ASM
class cD2E8 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_SkyBox_blend(tWPInGameData_SkyBox* self, void *p, float f) {
    cD2E8 *o = (cD2E8*)self;
    o->vt();
    f = f * f;
    self->m_SkyBoxRotation = f * *(float*)((char*)p + 4) + (1.0f - f) * self->m_SkyBoxRotation;
    self->m_lastSkyBoxRotation = *(float*)((char*)p + 4);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_blend);
#ifdef SKIP_ASM
struct VEnt_D378 { short delta; short idx; int (*fn)(void*); };
struct Pair_D378 { float cur; float tgt; };
struct IPair_D378 { int cur; int tgt; };
struct Obj_D378 { int pad0; char* vt; Pair_D378 p[6]; IPair_D378 i6; Pair_D378 q[2]; };
struct Src_D378 { int pad0; float v[6]; int i6; float w[2]; };

extern "C" void tWPInGameData_Sun_blend(tWPInGameData_Sun* self, Src_D378* src, float t) {
    VEnt_D378* e;
    float inv;
    e = (VEnt_D378*)((char*)self->mVtbl + 0x218);
    e->fn((char*)self + e->delta);
    t = t * t;
    inv = 1.0f - t;
    ((Pair_D378*)&self->m_SunPitchOffset)[0].cur = t * src->v[0] + inv * ((Pair_D378*)&self->m_SunPitchOffset)[0].cur;
    ((Pair_D378*)&self->m_SunPitchOffset)[0].tgt = src->v[0];
    ((Pair_D378*)&self->m_SunPitchOffset)[1].cur = t * src->v[1] + inv * ((Pair_D378*)&self->m_SunPitchOffset)[1].cur;
    ((Pair_D378*)&self->m_SunPitchOffset)[1].tgt = src->v[1];
    ((Pair_D378*)&self->m_SunPitchOffset)[2].cur = t * src->v[2] + inv * ((Pair_D378*)&self->m_SunPitchOffset)[2].cur;
    ((Pair_D378*)&self->m_SunPitchOffset)[2].tgt = src->v[2];
    ((Pair_D378*)&self->m_SunPitchOffset)[3].cur = t * src->v[3] + inv * ((Pair_D378*)&self->m_SunPitchOffset)[3].cur;
    ((Pair_D378*)&self->m_SunPitchOffset)[3].tgt = src->v[3];
    ((Pair_D378*)&self->m_SunPitchOffset)[4].cur = t * src->v[4] + inv * ((Pair_D378*)&self->m_SunPitchOffset)[4].cur;
    ((Pair_D378*)&self->m_SunPitchOffset)[4].tgt = src->v[4];
    ((Pair_D378*)&self->m_SunPitchOffset)[5].cur = t * src->v[5] + inv * ((Pair_D378*)&self->m_SunPitchOffset)[5].cur;
    ((Pair_D378*)&self->m_SunPitchOffset)[5].tgt = src->v[5];
    self->m_SunType = (int)(t * src->i6 + inv * self->m_SunType);
    self->m_lastSunType = src->i6;
    ((Pair_D378*)&self->m_LensIntensity)[0].cur = t * src->w[0] + inv * ((Pair_D378*)&self->m_LensIntensity)[0].cur;
    ((Pair_D378*)&self->m_LensIntensity)[0].tgt = src->w[0];
    ((Pair_D378*)&self->m_LensIntensity)[1].cur = t * src->w[1] + inv * ((Pair_D378*)&self->m_LensIntensity)[1].cur;
    ((Pair_D378*)&self->m_LensIntensity)[1].tgt = src->w[1];
    e = (VEnt_D378*)((char*)self->mVtbl + 0x218);
    e->fn((char*)self + e->delta);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Surface_blend);
#ifdef SKIP_ASM
class cD518 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_Surface_blend(tWPInGameData_Surface* self, void *p, float f) {
    cD518 *o = (cD518*)self;
    o->vt();
    f = f * f;
    self->m_SnowSparkleDensity = f * *(float*)((char*)p + 4) + (1.0f - f) * self->m_SnowSparkleDensity;
    self->m_lastSnowSparkleDensity = *(float*)((char*)p + 4);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_blend);
#ifdef SKIP_ASM
struct sP_D5A8 { float x, y; };
class cD5A8 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_Lighting_blend(tWPInGameData_Lighting* self, void *p, float f) {
    cD5A8 *o = (cD5A8*)self;
    o->vt();
    f = f * f;
    *(sP_D5A8*)&self->m_LightIrradianceName = *(sP_D5A8*)((char*)p + 4);
    *(sP_D5A8*)&self->m_ShadowIrradianceName = *(sP_D5A8*)((char*)p + 0xC);
    *(sP_D5A8*)&self->m_TunnelIrradianceName = *(sP_D5A8*)((char*)p + 0x14);
    *(sP_D5A8*)&self->m_ObjectIrradianceName = *(sP_D5A8*)((char*)p + 0x1C);
    self->m_AmbientLightIntensityOffset = f * *(float*)((char*)p + 0x24) + (1.0f - f) * self->m_AmbientLightIntensityOffset;
    self->m_lastAmbientLightIntensityOffset = *(float*)((char*)p + 0x24);
    self->m_RimLightIntensityOffset = f * *(float*)((char*)p + 0x28) + (1.0f - f) * self->m_RimLightIntensityOffset;
    self->m_lastRimLightIntensityOffset = *(float*)((char*)p + 0x28);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_blend);
#ifdef SKIP_ASM
// Environment property blend: w^2 blend of all 19 current values toward the
// incoming set, which is then stored as the target.
class cD698 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_Weather_blend(tWPInGameData_Weather* self, void *p, float f) {
    cD698 *o = (cD698*)self;
    o->vt();
    f = f * f;
    self->m_SnowIntensity = f * *(float*)((char*)p + 0x4) + (1.0f - f) * self->m_SnowIntensity;
    self->m_lastSnowIntensity = *(float*)((char*)p + 0x4);
    self->m_FlakeSize = f * *(float*)((char*)p + 0x8) + (1.0f - f) * self->m_FlakeSize;
    self->m_lastFlakeSize = *(float*)((char*)p + 0x8);
    self->m_WindIntensity = f * *(float*)((char*)p + 0xC) + (1.0f - f) * self->m_WindIntensity;
    self->m_lastWindIntensity = *(float*)((char*)p + 0xC);
    self->m_WindRotation = f * *(float*)((char*)p + 0x10) + (1.0f - f) * self->m_WindRotation;
    self->m_lastWindRotation = *(float*)((char*)p + 0x10);
    self->m_PlayerWindIntensity = f * *(float*)((char*)p + 0x14) + (1.0f - f) * self->m_PlayerWindIntensity;
    self->m_lastPlayerWindIntensity = *(float*)((char*)p + 0x14);
    self->m_Temperature = f * *(float*)((char*)p + 0x18) + (1.0f - f) * self->m_Temperature;
    self->m_lastTemperature = *(float*)((char*)p + 0x18);
    self->m_SnowFlurries = f * *(float*)((char*)p + 0x1C) + (1.0f - f) * self->m_SnowFlurries;
    self->m_lastSnowFlurries = *(float*)((char*)p + 0x1C);
    self->m_SnowGravityMultiplier = f * *(float*)((char*)p + 0x20) + (1.0f - f) * self->m_SnowGravityMultiplier;
    self->m_lastSnowGravityMultiplier = *(float*)((char*)p + 0x20);
    self->m_SnowFluffItensity = f * *(float*)((char*)p + 0x24) + (1.0f - f) * self->m_SnowFluffItensity;
    self->m_lastSnowFluffItensity = *(float*)((char*)p + 0x24);
    self->m_SnowFluffWindIntensity = f * *(float*)((char*)p + 0x28) + (1.0f - f) * self->m_SnowFluffWindIntensity;
    self->m_lastSnowFluffWindIntensity = *(float*)((char*)p + 0x28);
    self->m_LightningChance = f * *(float*)((char*)p + 0x2C) + (1.0f - f) * self->m_LightningChance;
    self->m_lastLightningChance = *(float*)((char*)p + 0x2C);
    self->m_SnowFlakeColourA = f * *(float*)((char*)p + 0x30) + (1.0f - f) * self->m_SnowFlakeColourA;
    self->m_lastSnowFlakeColourA = *(float*)((char*)p + 0x30);
    self->m_SnowFlakeColourR = f * *(float*)((char*)p + 0x34) + (1.0f - f) * self->m_SnowFlakeColourR;
    self->m_lastSnowFlakeColourR = *(float*)((char*)p + 0x34);
    self->m_SnowFlakeColourG = f * *(float*)((char*)p + 0x38) + (1.0f - f) * self->m_SnowFlakeColourG;
    self->m_lastSnowFlakeColourG = *(float*)((char*)p + 0x38);
    self->m_SnowFlakeColourB = f * *(float*)((char*)p + 0x3C) + (1.0f - f) * self->m_SnowFlakeColourB;
    self->m_lastSnowFlakeColourB = *(float*)((char*)p + 0x3C);
    self->m_SnowFluffColourA = f * *(float*)((char*)p + 0x40) + (1.0f - f) * self->m_SnowFluffColourA;
    self->m_lastSnowFluffColourA = *(float*)((char*)p + 0x40);
    self->m_SnowFluffColourR = f * *(float*)((char*)p + 0x44) + (1.0f - f) * self->m_SnowFluffColourR;
    self->m_lastSnowFluffColourR = *(float*)((char*)p + 0x44);
    self->m_SnowFluffColourG = f * *(float*)((char*)p + 0x48) + (1.0f - f) * self->m_SnowFluffColourG;
    self->m_lastSnowFluffColourG = *(float*)((char*)p + 0x48);
    self->m_SnowFluffColourB = f * *(float*)((char*)p + 0x4C) + (1.0f - f) * self->m_SnowFluffColourB;
    self->m_lastSnowFluffColourB = *(float*)((char*)p + 0x4C);
    o->vt();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_blend);
#ifdef SKIP_ASM
class cD968 { public: int f0; virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void v9();virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();virtual void v25();virtual void v26();virtual void v27();virtual void v28();virtual void v29();virtual void v30();virtual void v31();virtual void v32();virtual void v33();virtual void v34();virtual void v35();virtual void v36();virtual void v37();virtual void v38();virtual void v39();virtual void v40();virtual void v41();virtual void v42();virtual void v43();virtual void v44();virtual void v45();virtual void v46();virtual void v47();virtual void v48();virtual void v49();virtual void v50();virtual void v51();virtual void v52();virtual void v53();virtual void v54();virtual void v55();virtual void v56();virtual void v57();virtual void v58();virtual void v59();virtual void v60();virtual void v61();virtual void v62();virtual void v63();virtual void v64();virtual void v65(); virtual int vt(); };
extern "C" void tWPInGameData_Danger_blend(tWPInGameData_Danger* self, void *p, float f) {
    cD968 *o = (cD968*)self;
    o->vt();
    f = f * f;
    self->m_Level = (int)(f * (float)*(int*)((char*)p + 4) + (1.0f - f) * (float)self->m_Level);
    self->m_lastLevel = *(int*)((char*)p + 4);
    self->m_PointRate = f * *(float*)((char*)p + 8) + (1.0f - f) * self->m_PointRate;
    self->m_lastPointRate = *(float*)((char*)p + 8);
    o->vt();
}
#endif

extern "C" void func_002BDA28(void) {
}

extern "C" void func_002BDA30(void) {
}

extern "C" void func_002BDA38(void) {
}

extern "C" void func_002BDA40(void) {
}

extern "C" void func_002BDA48(void) {
}

extern "C" void func_002BDA50(void) {
}

extern "C" void func_002BDA58(void) {
}

extern "C" void func_002BDA60(void) {
}

extern "C" void func_002BDA68(void) {
}

extern "C" void func_002BDA70(void) {
}

extern "C" void func_002BDA78(void) {
}

extern "C" void func_002BDA80(void) {
}

extern "C" void func_002BDA88(void) {
}

extern "C" void func_002BDA90(void) {
}

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDA98);
#ifdef SKIP_ASM
extern "C" int func_002BDA98(tWPInGameData_MusicTrigger* arg0, void *arg1) {
    if ((arg0->m_Type) != (*(int *)((char*)(arg1) + (4)))) {
        return 0;
    }
    return (arg0->m_Value) == (*(int *)((char*)(arg1) + (8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDAC8);
#ifdef SKIP_ASM
extern "C" int func_002BDAC8(tWPInGameData_Mix* arg0, void *arg1) {
    return (arg0->m_MixPreset) == (*(int *)((char*)(arg1) + (4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDAE0);
#ifdef SKIP_ASM
extern "C" int func_002BDAE0(tWPInGameData_Ambience* arg0, void *arg1) {
    return (arg0->m_BankID) == (*(int *)((char*)(arg1) + (4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDAF8);
#ifdef SKIP_ASM
extern "C" int func_002BDAF8(tWPInGameData_Speech* arg0, void *arg1) {
    return (arg0->m_SpeechID) == (*(int *)((char*)(arg1) + (4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDB10);
#ifdef SKIP_ASM
extern "C" int func_002BDB10(tWPInGameData_Camera* arg0, void *arg1) {
    int var_2;

    var_2 = 0;
    if ((arg0->m_DrawDistance) == (*(float *)((char*)(arg1) + (4)))) {
        var_2 = 1;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDB38);
#ifdef SKIP_ASM
extern "C" int func_002BDB38(tWPInGameData_Fog* a, void *b) {
    if (a->m_FogDensity != *(float*)((char*)b + 4)) return 0;
    if (a->m_FogNear != *(float*)((char*)b + 8)) return 0;
    if (a->m_FogFar != *(float*)((char*)b + 0xC)) return 0;
    if (a->m_FogColourR != *(float*)((char*)b + 0x10)) return 0;
    if (a->m_FogColourG != *(float*)((char*)b + 0x14)) return 0;
    if (a->m_FogColourB != *(float*)((char*)b + 0x18)) return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDBD0);
#ifdef SKIP_ASM
extern "C" int func_002BDBD0(tWPInGameData_LightGlow* a, void *b) {
    if (a->m_LightGlowCutoff != *(float*)((char*)b + 0x4)) return 0;
    if (a->m_LightGlowCutoffScale != *(float*)((char*)b + 0x8)) return 0;
    if (a->m_LightGlowCopyIntensity != *(float*)((char*)b + 0xC)) return 0;
    if (a->m_LightGlowSourceIntensity != *(float*)((char*)b + 0x10)) return 0;
    if (a->m_LightGlowFrameIntensity != *(float*)((char*)b + 0x14)) return 0;
    if (a->m_LightGlowBlend2 != *(float*)((char*)b + 0x18)) return 0;
    if (a->m_LightGlowBlend3 != *(float*)((char*)b + 0x1C)) return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDC78);
#ifdef SKIP_ASM
extern "C" int func_002BDC78(tWPInGameData_ScreenTint* a, void *b) {
    if (a->m_ScreenTintScaleR != *(float*)((char*)b + 4)) return 0;
    if (a->m_ScreenTintScaleG != *(float*)((char*)b + 8)) return 0;
    if (a->m_ScreenTintScaleB != *(float*)((char*)b + 0xC)) return 0;
    if (a->m_ScreenTintOffsetR != *(float*)((char*)b + 0x10)) return 0;
    if (a->m_ScreenTintOffsetG != *(float*)((char*)b + 0x14)) return 0;
    if (a->m_ScreenTintOffsetB != *(float*)((char*)b + 0x18)) return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDD10);
#ifdef SKIP_ASM
extern "C" int func_002BDD10(tWPInGameData_SkyBox* arg0, void *arg1) {
    int var_2;

    var_2 = 0;
    if ((arg0->m_SkyBoxRotation) == (*(float *)((char*)(arg1) + (4)))) {
        var_2 = 1;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDD38);
#ifdef SKIP_ASM
extern "C" int func_002BDD38(tWPInGameData_Sun* a, void *b) {
    if (a->m_SunPitchOffset != *(float*)((char*)b + 0x4)) return 0;
    if (a->m_SunYawOffset != *(float*)((char*)b + 0x8)) return 0;
    if (a->m_SunColorR != *(float*)((char*)b + 0xC)) return 0;
    if (a->m_SunColorG != *(float*)((char*)b + 0x10)) return 0;
    if (a->m_SunColorB != *(float*)((char*)b + 0x14)) return 0;
    if (a->m_SunIntensity != *(float*)((char*)b + 0x18)) return 0;
    if (a->m_SunType != *(int*)((char*)b + 0x1C)) return 0;
    if (a->m_LensIntensity != *(float*)((char*)b + 0x20)) return 0;
    if (a->m_SunRadius != *(float*)((char*)b + 0x24)) return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDE08);
#ifdef SKIP_ASM
extern "C" int func_002BDE08(tWPInGameData_Surface* arg0, void *arg1) {
    int var_2;

    var_2 = 0;
    if ((arg0->m_SnowSparkleDensity) == (*(float *)((char*)(arg1) + (4)))) {
        var_2 = 1;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDE30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002BB100_DE30(char *, char *) __asm__("func_002BB100");
extern "C" int func_002BDE30(tWPInGameData_Lighting* a, char *b) {
    if (func_002BB100_DE30(((char*)a) + 8, b + 4)) return 0;
    if (func_002BB100_DE30(((char*)a) + 0x18, b + 0xC)) return 0;
    if (func_002BB100_DE30(((char*)a) + 0x28, b + 0x14)) return 0;
    if (func_002BB100_DE30(((char*)a) + 0x38, b + 0x1C)) return 0;
    if (a->m_AmbientLightIntensityOffset != *(float*)(b + 0x24)) return 0;
    if (a->m_RimLightIntensityOffset != *(float*)(b + 0x28)) return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDEE0);
#ifdef SKIP_ASM
// Environment property compare: all 19 current values against the incoming set.
extern "C" int func_002BDEE0(tWPInGameData_Weather* arg0, void *arg1) {
    if ((arg0->m_SnowIntensity) != (*(float *)((char*)(arg1) + (0x4)))) {
        return 0;
    }
    if ((arg0->m_FlakeSize) != (*(float *)((char*)(arg1) + (0x8)))) {
        return 0;
    }
    if ((arg0->m_WindIntensity) != (*(float *)((char*)(arg1) + (0xC)))) {
        return 0;
    }
    if ((arg0->m_WindRotation) != (*(float *)((char*)(arg1) + (0x10)))) {
        return 0;
    }
    if ((arg0->m_PlayerWindIntensity) != (*(float *)((char*)(arg1) + (0x14)))) {
        return 0;
    }
    if ((arg0->m_Temperature) != (*(float *)((char*)(arg1) + (0x18)))) {
        return 0;
    }
    if ((arg0->m_SnowFlurries) != (*(float *)((char*)(arg1) + (0x1C)))) {
        return 0;
    }
    if ((arg0->m_SnowGravityMultiplier) != (*(float *)((char*)(arg1) + (0x20)))) {
        return 0;
    }
    if ((arg0->m_SnowFluffItensity) != (*(float *)((char*)(arg1) + (0x24)))) {
        return 0;
    }
    if ((arg0->m_SnowFluffWindIntensity) != (*(float *)((char*)(arg1) + (0x28)))) {
        return 0;
    }
    if ((arg0->m_LightningChance) != (*(float *)((char*)(arg1) + (0x2C)))) {
        return 0;
    }
    if ((arg0->m_SnowFlakeColourA) != (*(float *)((char*)(arg1) + (0x30)))) {
        return 0;
    }
    if ((arg0->m_SnowFlakeColourR) != (*(float *)((char*)(arg1) + (0x34)))) {
        return 0;
    }
    if ((arg0->m_SnowFlakeColourG) != (*(float *)((char*)(arg1) + (0x38)))) {
        return 0;
    }
    if ((arg0->m_SnowFlakeColourB) != (*(float *)((char*)(arg1) + (0x3C)))) {
        return 0;
    }
    if ((arg0->m_SnowFluffColourA) != (*(float *)((char*)(arg1) + (0x40)))) {
        return 0;
    }
    if ((arg0->m_SnowFluffColourR) != (*(float *)((char*)(arg1) + (0x44)))) {
        return 0;
    }
    if ((arg0->m_SnowFluffColourG) != (*(float *)((char*)(arg1) + (0x48)))) {
        return 0;
    }
    if ((arg0->m_SnowFluffColourB) != (*(float *)((char*)(arg1) + (0x4C)))) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE078);
#ifdef SKIP_ASM
extern "C" int func_002BE078(tWPInGameData_Danger* arg0, void *arg1) {
    if ((arg0->m_Level) != (*(int *)((char*)(arg1) + (4)))) {
        return 0;
    }
    if ((arg0->m_PointRate) != (*(float *)((char*)(arg1) + (8)))) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0B0);
#ifdef SKIP_ASM
extern "C" void func_002BE0B0(tWPInGameData_MusicTrigger* p) {
    *(int*)&p->m_wpTravelDistance = 0;
    p->m_Type = -1;
    p->m_Value = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0C8);
#ifdef SKIP_ASM
extern "C" void func_002BE0C8(tWPInGameData_Mix* arg0) {
    (*(int*)&arg0->m_wpTravelDistance) = 0;
    (arg0->m_MixPreset) = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0D8);
#ifdef SKIP_ASM
extern "C" void func_002BE0D8(tWPInGameData_Ambience* arg0) {
    (*(int*)&arg0->m_wpTravelDistance) = 0;
    (arg0->m_BankID) = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0E8);
#ifdef SKIP_ASM
extern "C" void func_002BE0E8(tWPInGameData_Speech* arg0) {
    (*(int*)&arg0->m_wpTravelDistance) = 0;
    (arg0->m_SpeechID) = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0F8);
#ifdef SKIP_ASM
extern "C" void func_002BE0F8(tWPInGameData_Camera* arg0) {
    (*(int*)&arg0->m_wpTravelDistance) = 0;
    (arg0->m_DrawDistance) = 30000.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE108);
#ifdef SKIP_ASM
extern "C" void func_002BE108(tWPInGameData_Fog* arg0) {
    (arg0->m_FogNear) = 3000.0f;
    (arg0->m_FogFar) = 30000.0f;
    (arg0->m_FogColourR) = 0.43f;
    (arg0->m_FogColourG) = 0.55f;
    (arg0->m_FogColourB) = 0.71f;
    (*(int*)&arg0->m_FogDensity) = 0;
    (*(int*)&arg0->m_wpTravelDistance) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE140);
#ifdef SKIP_ASM
extern "C" void func_002BE140(tWPInGameData_LightGlow* arg0) {
    arg0->m_LightGlowCutoff = 1.0f;
    arg0->m_LightGlowCutoffScale = 1.0f;
    arg0->m_LightGlowCopyIntensity = 1.0f;
    arg0->m_LightGlowSourceIntensity = 1.0f;
    arg0->m_LightGlowFrameIntensity = 1.0f;
    *(int*)&arg0->m_LightGlowBlend2 = 0;
    *(int*)&arg0->m_LightGlowBlend3 = 0;
    *(int*)&arg0->m_wpTravelDistance = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE170);
#ifdef SKIP_ASM
extern "C" void func_002BE170(tWPInGameData_ScreenTint* arg0) {
    arg0->m_ScreenTintScaleR = 1.0f;
    arg0->m_ScreenTintScaleG = 1.0f;
    arg0->m_ScreenTintScaleB = 1.0f;
    *(int*)&arg0->m_ScreenTintOffsetR = 0;
    *(int*)&arg0->m_ScreenTintOffsetG = 0;
    *(int*)&arg0->m_ScreenTintOffsetB = 0;
    *(int*)&arg0->m_wpTravelDistance = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE198);
#ifdef SKIP_ASM
extern "C" void func_002BE198(tWPInGameData_SkyBox* p) {
    *(int*)&p->m_SkyBoxRotation = 0;
    *(int*)&p->m_wpTravelDistance = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE1A8);
#ifdef SKIP_ASM
extern "C" void func_002BE1A8(tWPInGameData_Sun* arg0) {
    *(int*)&arg0->m_SunPitchOffset = 0;
    *(int*)&arg0->m_SunYawOffset = 0;
    arg0->m_SunColorR = 1.0f;
    arg0->m_SunColorG = 1.0f;
    arg0->m_SunColorB = 1.0f;
    arg0->m_SunIntensity = 1.0f;
    arg0->m_SunType = 0;
    arg0->m_LensIntensity = 1.0f;
    arg0->m_SunRadius = 1.0f;
    *(int*)&arg0->m_wpTravelDistance = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE1E0);
#ifdef SKIP_ASM
extern "C" void func_002BE1E0(tWPInGameData_Surface* arg0) {
    (*(int*)&arg0->m_wpTravelDistance) = 0;
    (arg0->m_SnowSparkleDensity) = 1.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE1F8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Copies the default pair into four slots, sets 0x48 to 1.0 and 0x50 to 0.75, and clears 0x00.
struct sPair_002BE1F8
{
    float a;
    float b;
};
extern sPair_002BE1F8 D_004A53D0;
struct sPairSet_002BE1F8
{
    int flags;              // 0x00
    char pad_0x04[4];
    sPair_002BE1F8 k0;      // 0x08
    char pad_0x10[8];
    sPair_002BE1F8 k1;      // 0x18
    char pad_0x20[8];
    sPair_002BE1F8 k2;      // 0x28
    char pad_0x30[8];
    sPair_002BE1F8 k3;      // 0x38
    char pad_0x40[8];
    float field_0x48;       // 0x48
    char pad_0x4C[4];
    float field_0x50;       // 0x50
};

extern "C" void func_002BE1F8(tWPInGameData_Lighting* o)
{
    (*(sPair_002BE1F8*)&o->m_LightIrradianceName) = D_004A53D0;
    (*(sPair_002BE1F8*)&o->m_ShadowIrradianceName) = D_004A53D0;
    (*(sPair_002BE1F8*)&o->m_TunnelIrradianceName) = D_004A53D0;
    (*(sPair_002BE1F8*)&o->m_ObjectIrradianceName) = D_004A53D0;
    o->m_AmbientLightIntensityOffset = 1.0f;
    o->m_RimLightIntensityOffset = 0.75f;
    *(int*)&o->m_wpTravelDistance = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE258);
#ifdef SKIP_ASM
extern "C" void func_002BE258(tWPInGameData_Weather* arg0) {
    *(int*)&arg0->m_SnowIntensity = 0;
    arg0->m_FlakeSize = 6.0f;
    *(int*)&arg0->m_WindIntensity = 0;
    *(int*)&arg0->m_WindRotation = 0;
    *(int*)&arg0->m_PlayerWindIntensity = 0;
    arg0->m_Temperature = 10.0f;
    arg0->m_SnowFlurries = 1.0f;
    arg0->m_SnowGravityMultiplier = 1.0f;
    *(int*)&arg0->m_SnowFluffItensity = 0;
    *(int*)&arg0->m_SnowFluffWindIntensity = 0;
    *(int*)&arg0->m_LightningChance = 0;
    arg0->m_SnowFlakeColourA = 1.0f;
    arg0->m_SnowFlakeColourR = 1.0f;
    arg0->m_SnowFlakeColourG = 1.0f;
    arg0->m_SnowFlakeColourB = 1.0f;
    arg0->m_SnowFluffColourA = 0.06f;
    arg0->m_SnowFluffColourR = 1.0f;
    arg0->m_SnowFluffColourG = 1.0f;
    arg0->m_SnowFluffColourB = 1.0f;
    *(int*)&arg0->m_wpTravelDistance = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE2C8);
#ifdef SKIP_ASM
extern "C" void func_002BE2C8(tWPInGameData_Danger* p) {
    p->m_Level = 0;
    *(int*)&p->m_PointRate = 0;
    *(int*)&p->m_wpTravelDistance = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_readFromReplayFrame);
#ifdef SKIP_ASM
struct VE_E2D8 { short delta; short idx; int (*fn)(void *, void *, int); };
extern "C" void tWPInGameData_MusicTrigger_readFromReplayFrame(tWPInGameData_MusicTrigger* self, char *s) {
    VE_E2D8 *e = &(*(VE_E2D8 **)s)[2];
    e->fn(s + e->delta, ((char*)self) + 8, 4);
    e = &(*(VE_E2D8 **)s)[2];
    e->fn(s + e->delta, ((char*)self), 4);
    e = &(*(VE_E2D8 **)s)[2];
    e->fn(s + e->delta, ((char*)self) + 0x10, 4);
    e = &(*(VE_E2D8 **)s)[2];
    e->fn(s + e->delta, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE378);
#ifdef SKIP_ASM
extern "C" void func_002BE378(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE3E0);
#ifdef SKIP_ASM
extern "C" void func_002BE3E0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE448);
#ifdef SKIP_ASM
extern "C" void func_002BE448(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE4B0);
#ifdef SKIP_ASM
extern "C" void func_002BE4B0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_readFromReplayFrame);
#ifdef SKIP_ASM
struct VEnt_E518 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_E518 { char* vt; };

static inline void Wr_E518(Stream_E518* s, void* p, int n) {
    VEnt_E518* e = (VEnt_E518*)(s->vt + 0x10);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_Fog_readFromReplayFrame(tWPInGameData_Fog* self, Stream_E518* s) {
    Wr_E518(s, ((char*)self) + 0x8, 4);
    Wr_E518(s, ((char*)self), 4);
    Wr_E518(s, ((char*)self) + 0x10, 4);
    Wr_E518(s, ((char*)self), 4);
    Wr_E518(s, ((char*)self) + 0x18, 4);
    Wr_E518(s, ((char*)self), 4);
    Wr_E518(s, ((char*)self) + 0x20, 4);
    Wr_E518(s, ((char*)self), 4);
    Wr_E518(s, ((char*)self) + 0x28, 4);
    Wr_E518(s, ((char*)self), 4);
    Wr_E518(s, ((char*)self) + 0x30, 4);
    Wr_E518(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_readFromReplayFrame);
#ifdef SKIP_ASM
struct VEnt_E698 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_E698 { char* vt; };

static inline void Wr_E698(Stream_E698* s, void* p, int n) {
    VEnt_E698* e = (VEnt_E698*)(s->vt + 0x10);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_LightGlow_readFromReplayFrame(tWPInGameData_LightGlow* self, Stream_E698* s) {
    Wr_E698(s, ((char*)self) + 0x8, 4);
    Wr_E698(s, ((char*)self), 4);
    Wr_E698(s, ((char*)self) + 0x10, 4);
    Wr_E698(s, ((char*)self), 4);
    Wr_E698(s, ((char*)self) + 0x18, 4);
    Wr_E698(s, ((char*)self), 4);
    Wr_E698(s, ((char*)self) + 0x20, 4);
    Wr_E698(s, ((char*)self), 4);
    Wr_E698(s, ((char*)self) + 0x28, 4);
    Wr_E698(s, ((char*)self), 4);
    Wr_E698(s, ((char*)self) + 0x30, 4);
    Wr_E698(s, ((char*)self), 4);
    Wr_E698(s, ((char*)self) + 0x38, 4);
    Wr_E698(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_readFromReplayFrame);
#ifdef SKIP_ASM
struct VEnt_E850 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_E850 { char* vt; };

static inline void Wr_E850(Stream_E850* s, void* p, int n) {
    VEnt_E850* e = (VEnt_E850*)(s->vt + 0x10);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_ScreenTint_readFromReplayFrame(tWPInGameData_ScreenTint* self, Stream_E850* s) {
    Wr_E850(s, ((char*)self) + 0x8, 4);
    Wr_E850(s, ((char*)self), 4);
    Wr_E850(s, ((char*)self) + 0x10, 4);
    Wr_E850(s, ((char*)self), 4);
    Wr_E850(s, ((char*)self) + 0x18, 4);
    Wr_E850(s, ((char*)self), 4);
    Wr_E850(s, ((char*)self) + 0x20, 4);
    Wr_E850(s, ((char*)self), 4);
    Wr_E850(s, ((char*)self) + 0x28, 4);
    Wr_E850(s, ((char*)self), 4);
    Wr_E850(s, ((char*)self) + 0x30, 4);
    Wr_E850(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE9D0);
#ifdef SKIP_ASM
extern "C" void func_002BE9D0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_readFromReplayFrame);
#ifdef SKIP_ASM
struct VEnt_EA38 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_EA38 { char* vt; };

static inline void Wr_EA38(Stream_EA38* s, void* p, int n) {
    VEnt_EA38* e = (VEnt_EA38*)(s->vt + 0x10);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_Sun_readFromReplayFrame(tWPInGameData_Sun* self, Stream_EA38* s) {
    Wr_EA38(s, ((char*)self) + 0x8, 4);
    Wr_EA38(s, ((char*)self), 4);
    Wr_EA38(s, ((char*)self) + 0x10, 4);
    Wr_EA38(s, ((char*)self), 4);
    Wr_EA38(s, ((char*)self) + 0x18, 4);
    Wr_EA38(s, ((char*)self), 4);
    Wr_EA38(s, ((char*)self) + 0x20, 4);
    Wr_EA38(s, ((char*)self), 4);
    Wr_EA38(s, ((char*)self) + 0x28, 4);
    Wr_EA38(s, ((char*)self), 4);
    Wr_EA38(s, ((char*)self) + 0x30, 4);
    Wr_EA38(s, ((char*)self), 4);
    Wr_EA38(s, ((char*)self) + 0x38, 4);
    Wr_EA38(s, ((char*)self), 4);
    Wr_EA38(s, ((char*)self) + 0x40, 4);
    Wr_EA38(s, ((char*)self), 4);
    Wr_EA38(s, ((char*)self) + 0x48, 4);
    Wr_EA38(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BEC60);
#ifdef SKIP_ASM
extern "C" void func_002BEC60(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_readFromReplayFrame);
#ifdef SKIP_ASM
struct VEnt_ECC8 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_ECC8 { char* vt; };

static inline void Wr_ECC8(Stream_ECC8* s, void* p, int n) {
    VEnt_ECC8* e = (VEnt_ECC8*)(s->vt + 0x10);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_Lighting_readFromReplayFrame(tWPInGameData_Lighting* self, Stream_ECC8* s) {
    Wr_ECC8(s, ((char*)self) + 0x8, 8);
    Wr_ECC8(s, ((char*)self), 4);
    Wr_ECC8(s, ((char*)self) + 0x18, 8);
    Wr_ECC8(s, ((char*)self), 4);
    Wr_ECC8(s, ((char*)self) + 0x28, 8);
    Wr_ECC8(s, ((char*)self), 4);
    Wr_ECC8(s, ((char*)self) + 0x38, 8);
    Wr_ECC8(s, ((char*)self), 4);
    Wr_ECC8(s, ((char*)self) + 0x48, 4);
    Wr_ECC8(s, ((char*)self), 4);
    Wr_ECC8(s, ((char*)self) + 0x50, 4);
    Wr_ECC8(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_readFromReplayFrame);
#ifdef SKIP_ASM
class cStream_EE48 {
public:
    virtual void v01();
    virtual int write(void* p, int n);
};

// Streams 19 words of self (at +0x8 .. +0x98), each followed by the word at +0x0.
extern "C" void tWPInGameData_Weather_readFromReplayFrame(tWPInGameData_Weather* self, cStream_EE48* s)
{
    s->write(((char*)self) + 0x8, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x10, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x18, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x20, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x28, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x30, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x38, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x40, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x48, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x50, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x58, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x60, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x68, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x70, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x78, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x80, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x88, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x90, 4);
    s->write(((char*)self), 4);
    s->write(((char*)self) + 0x98, 4);
    s->write(((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_readFromReplayFrame);
#ifdef SKIP_ASM
struct VE_F2A0 { short delta; short idx; int (*fn)(void *, void *, int); };
extern "C" void tWPInGameData_Danger_readFromReplayFrame(tWPInGameData_Danger* self, char *s) {
    VE_F2A0 *e = &(*(VE_F2A0 **)s)[2];
    e->fn(s + e->delta, ((char*)self) + 8, 4);
    e = &(*(VE_F2A0 **)s)[2];
    e->fn(s + e->delta, ((char*)self), 4);
    e = &(*(VE_F2A0 **)s)[2];
    e->fn(s + e->delta, ((char*)self) + 0x10, 4);
    e = &(*(VE_F2A0 **)s)[2];
    e->fn(s + e->delta, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_writeToReplayFrame);
#ifdef SKIP_ASM
struct VE_F340 { short delta; short idx; int (*fn)(void *, void *, int); };
extern "C" void tWPInGameData_MusicTrigger_writeToReplayFrame(tWPInGameData_MusicTrigger* self, char *s) {
    VE_F340 *e = &(*(VE_F340 **)s)[1];
    e->fn(s + e->delta, ((char*)self) + 8, 4);
    e = &(*(VE_F340 **)s)[1];
    e->fn(s + e->delta, ((char*)self), 4);
    e = &(*(VE_F340 **)s)[1];
    e->fn(s + e->delta, ((char*)self) + 0x10, 4);
    e = &(*(VE_F340 **)s)[1];
    e->fn(s + e->delta, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BF3E0);
#ifdef SKIP_ASM
extern "C" void func_002BF3E0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BF448);
#ifdef SKIP_ASM
extern "C" void func_002BF448(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BF4B0);
#ifdef SKIP_ASM
extern "C" void func_002BF4B0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BF518);
#ifdef SKIP_ASM
extern "C" void func_002BF518(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_writeToReplayFrame);
#ifdef SKIP_ASM
struct VEnt_F580 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_F580 { char* vt; };

static inline void Wr_F580(Stream_F580* s, void* p, int n) {
    VEnt_F580* e = (VEnt_F580*)(s->vt + 0x8);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_Fog_writeToReplayFrame(tWPInGameData_Fog* self, Stream_F580* s) {
    Wr_F580(s, ((char*)self) + 0x8, 4);
    Wr_F580(s, ((char*)self), 4);
    Wr_F580(s, ((char*)self) + 0x10, 4);
    Wr_F580(s, ((char*)self), 4);
    Wr_F580(s, ((char*)self) + 0x18, 4);
    Wr_F580(s, ((char*)self), 4);
    Wr_F580(s, ((char*)self) + 0x20, 4);
    Wr_F580(s, ((char*)self), 4);
    Wr_F580(s, ((char*)self) + 0x28, 4);
    Wr_F580(s, ((char*)self), 4);
    Wr_F580(s, ((char*)self) + 0x30, 4);
    Wr_F580(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_writeToReplayFrame);
#ifdef SKIP_ASM
struct VEnt_F700 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_F700 { char* vt; };

static inline void Wr_F700(Stream_F700* s, void* p, int n) {
    VEnt_F700* e = (VEnt_F700*)(s->vt + 0x8);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_LightGlow_writeToReplayFrame(tWPInGameData_LightGlow* self, Stream_F700* s) {
    Wr_F700(s, ((char*)self) + 0x8, 4);
    Wr_F700(s, ((char*)self), 4);
    Wr_F700(s, ((char*)self) + 0x10, 4);
    Wr_F700(s, ((char*)self), 4);
    Wr_F700(s, ((char*)self) + 0x18, 4);
    Wr_F700(s, ((char*)self), 4);
    Wr_F700(s, ((char*)self) + 0x20, 4);
    Wr_F700(s, ((char*)self), 4);
    Wr_F700(s, ((char*)self) + 0x28, 4);
    Wr_F700(s, ((char*)self), 4);
    Wr_F700(s, ((char*)self) + 0x30, 4);
    Wr_F700(s, ((char*)self), 4);
    Wr_F700(s, ((char*)self) + 0x38, 4);
    Wr_F700(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_writeToReplayFrame);
#ifdef SKIP_ASM
struct VEnt_F8B8 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_F8B8 { char* vt; };

static inline void Wr_F8B8(Stream_F8B8* s, void* p, int n) {
    VEnt_F8B8* e = (VEnt_F8B8*)(s->vt + 0x8);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_ScreenTint_writeToReplayFrame(tWPInGameData_ScreenTint* self, Stream_F8B8* s) {
    Wr_F8B8(s, ((char*)self) + 0x8, 4);
    Wr_F8B8(s, ((char*)self), 4);
    Wr_F8B8(s, ((char*)self) + 0x10, 4);
    Wr_F8B8(s, ((char*)self), 4);
    Wr_F8B8(s, ((char*)self) + 0x18, 4);
    Wr_F8B8(s, ((char*)self), 4);
    Wr_F8B8(s, ((char*)self) + 0x20, 4);
    Wr_F8B8(s, ((char*)self), 4);
    Wr_F8B8(s, ((char*)self) + 0x28, 4);
    Wr_F8B8(s, ((char*)self), 4);
    Wr_F8B8(s, ((char*)self) + 0x30, 4);
    Wr_F8B8(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BFA38);
#ifdef SKIP_ASM
extern "C" void func_002BFA38(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_writeToReplayFrame);
#ifdef SKIP_ASM
struct VEnt_FAA0 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_FAA0 { char* vt; };

static inline void Wr_FAA0(Stream_FAA0* s, void* p, int n) {
    VEnt_FAA0* e = (VEnt_FAA0*)(s->vt + 0x8);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_Sun_writeToReplayFrame(tWPInGameData_Sun* self, Stream_FAA0* s) {
    Wr_FAA0(s, ((char*)self) + 0x8, 4);
    Wr_FAA0(s, ((char*)self), 4);
    Wr_FAA0(s, ((char*)self) + 0x10, 4);
    Wr_FAA0(s, ((char*)self), 4);
    Wr_FAA0(s, ((char*)self) + 0x18, 4);
    Wr_FAA0(s, ((char*)self), 4);
    Wr_FAA0(s, ((char*)self) + 0x20, 4);
    Wr_FAA0(s, ((char*)self), 4);
    Wr_FAA0(s, ((char*)self) + 0x28, 4);
    Wr_FAA0(s, ((char*)self), 4);
    Wr_FAA0(s, ((char*)self) + 0x30, 4);
    Wr_FAA0(s, ((char*)self), 4);
    Wr_FAA0(s, ((char*)self) + 0x38, 4);
    Wr_FAA0(s, ((char*)self), 4);
    Wr_FAA0(s, ((char*)self) + 0x40, 4);
    Wr_FAA0(s, ((char*)self), 4);
    Wr_FAA0(s, ((char*)self) + 0x48, 4);
    Wr_FAA0(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BFCC8);
#ifdef SKIP_ASM
extern "C" void func_002BFCC8(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_writeToReplayFrame);
#ifdef SKIP_ASM
struct VEnt_FD30 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_FD30 { char* vt; };

static inline void Wr_FD30(Stream_FD30* s, void* p, int n) {
    VEnt_FD30* e = (VEnt_FD30*)(s->vt + 0x8);
    e->fn((char*)s + e->delta, p, n);
}

extern "C" void tWPInGameData_Lighting_writeToReplayFrame(tWPInGameData_Lighting* self, Stream_FD30* s) {
    Wr_FD30(s, ((char*)self) + 0x8, 8);
    Wr_FD30(s, ((char*)self), 4);
    Wr_FD30(s, ((char*)self) + 0x18, 8);
    Wr_FD30(s, ((char*)self), 4);
    Wr_FD30(s, ((char*)self) + 0x28, 8);
    Wr_FD30(s, ((char*)self), 4);
    Wr_FD30(s, ((char*)self) + 0x38, 8);
    Wr_FD30(s, ((char*)self), 4);
    Wr_FD30(s, ((char*)self) + 0x48, 4);
    Wr_FD30(s, ((char*)self), 4);
    Wr_FD30(s, ((char*)self) + 0x50, 4);
    Wr_FD30(s, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream_FEB0 {
public:
    virtual int read(void* p, int n);
    virtual int write(void* p, int n);
};

// Reads 19 words into self (at +0x8 .. +0x98), each followed by one into +0x0
// (the read-side twin of tWPInGameData_Weather_readFromReplayFrame).
extern "C" void tWPInGameData_Weather_writeToReplayFrame(tWPInGameData_Weather* self, cStream_FEB0* s)
{
    s->read(((char*)self) + 0x8, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x10, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x18, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x20, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x28, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x30, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x38, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x40, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x48, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x50, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x58, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x60, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x68, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x70, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x78, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x80, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x88, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x90, 4);
    s->read(((char*)self), 4);
    s->read(((char*)self) + 0x98, 4);
    s->read(((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_writeToReplayFrame);
#ifdef SKIP_ASM
struct VE_0308 { short delta; short idx; int (*fn)(void *, void *, int); };
extern "C" void tWPInGameData_Danger_writeToReplayFrame(tWPInGameData_Danger* self, char *s) {
    VE_0308 *e = &(*(VE_0308 **)s)[1];
    e->fn(s + e->delta, ((char*)self) + 8, 4);
    e = &(*(VE_0308 **)s)[1];
    e->fn(s + e->delta, ((char*)self), 4);
    e = &(*(VE_0308 **)s)[1];
    e->fn(s + e->delta, ((char*)self) + 0x10, 4);
    e = &(*(VE_0308 **)s)[1];
    e->fn(s + e->delta, ((char*)self), 4);
}
#endif

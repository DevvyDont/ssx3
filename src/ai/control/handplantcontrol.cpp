#include "common.h"

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_gainFocus);
#ifdef SKIP_ASM
struct sVec4HP {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0SubHP(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline sVec4HP vu0DivHP(const sVec4HP& v, float s)
{
    sVec4HP r;
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

extern "C" void cHandplantMotion_gainFocus(char* self)
{
    char* rider = *(char**)(self + 0xA0);
    *(sVec4HP*)(rider + 0x1E0) = vu0DivHP(vu0SubHP(*(sVec4HP*)(self + 0x30), *(sVec4HP*)(rider + 0x110)), *(float*)(self + 0x84));
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_00138BA0);

INCLUDE_ASM("ai/control/handplantcontrol", func_00139178);

INCLUDE_ASM("ai/control/handplantcontrol", func_001391A8);

extern "C" void* func_0011E150(int, int);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00139528__FPv);
#ifdef SKIP_ASM
void* func_00139528(void* self)
{
    return func_0011E150(*(int*)((char*)self + 0xa0), 0);
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_00139548);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00139970);
#ifdef SKIP_ASM
struct sVEntry00139970 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00139970(void* self, void* obj)
{
    sVEntry00139970* vt = *(sVEntry00139970**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0xA0);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001399A8);
#ifdef SKIP_ASM
struct sVEntry001399A8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001399A8(void* self, void* obj)
{
    sVEntry001399A8* vt = *(sVEntry001399A8**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0xA0);
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_001399E0);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00139A18__FPv);
#ifdef SKIP_ASM
void func_00139A18(void* self)
{
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_00139A20);

INCLUDE_ASM("ai/control/handplantcontrol", func_00139C88);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013A7B0);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013A8F8);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013A968);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013AA48);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013ACB0);
#ifdef SKIP_ASM
struct sVEntry0013ACB0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0013ACB0(void* self, void* obj)
{
    sVEntry0013ACB0* vt = *(sVEntry0013ACB0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x4);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013ACE8);
#ifdef SKIP_ASM
struct sVEntry0013ACE8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0013ACE8(void* self, void* obj)
{
    sVEntry0013ACE8* vt = *(sVEntry0013ACE8**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x4);
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_0013AD20);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013ADC0);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013AF28);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013BD80);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013BFA8);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C140);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C5A0);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013C5E0);
#ifdef SKIP_ASM
struct sVEntry0013C5E0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0013C5E0(void* self, void* obj)
{
    sVEntry0013C5E0* vt = *(sVEntry0013C5E0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x50);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013C618);
#ifdef SKIP_ASM
struct sVEntry0013C618 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0013C618(void* self, void* obj)
{
    sVEntry0013C618* vt = *(sVEntry0013C618**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x50);
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C650);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C7A8);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C878);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C948);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013CCF0);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013D028);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013D1B8);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013D818);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013F178);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013F410);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013F488);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013F848);
#ifdef SKIP_ASM
struct sVEntry0013F848 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0013F848(void* self, void* obj)
{
    sVEntry0013F848* vt = *(sVEntry0013F848**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x18);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013F880);
#ifdef SKIP_ASM
struct sVEntry0013F880 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0013F880(void* self, void* obj)
{
    sVEntry0013F880* vt = *(sVEntry0013F880**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x18);
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_0013F8F8);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013FAD8);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013FB20);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140680);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern void* D_00459CC8[];

extern "C" void func_00140680(int* self, int flags)
{
    *(void***)self = D_00459CC8;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

extern void* D_004FF120[];

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001406B0__FPv);
#ifdef SKIP_ASM
void* func_001406B0(void* self)
{
    return (void*)D_004FF120;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001406C0__FPv);
#ifdef SKIP_ASM
int func_001406C0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001406C8__FPv);
#ifdef SKIP_ASM
void func_001406C8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001406D0__FPv);
#ifdef SKIP_ASM
void* func_001406D0(void* self)
{
    return (void*)D_004FF120;
}
#endif

extern void* D_004FF1A0[];

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001406E0__FPv);
#ifdef SKIP_ASM
void* func_001406E0(void* self)
{
    return (void*)D_004FF1A0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001406F0__FPv);
#ifdef SKIP_ASM
int func_001406F0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001406F8__FPv);
#ifdef SKIP_ASM
int func_001406F8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140700__FPv);
#ifdef SKIP_ASM
int func_00140700(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140708__FPv);
#ifdef SKIP_ASM
void func_00140708(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140710__FPv);
#ifdef SKIP_ASM
void func_00140710(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140718__FPv);
#ifdef SKIP_ASM
void func_00140718(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140720__FPv);
#ifdef SKIP_ASM
void func_00140720(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140728__FPv);
#ifdef SKIP_ASM
void func_00140728(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140730__FPv);
#ifdef SKIP_ASM
void func_00140730(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140738);
#ifdef SKIP_ASM
struct sHPC140738 {
    char pad0[0x1F0];
    int v[19][3];           // 0x1F0
    char pad2D4[0x430 - 0x2D4];
    unsigned int u430;      // 0x430
    char pad434[0x5B8 - 0x434];
    unsigned int arr[64];   // 0x5B8
};

extern "C" sHPC140738* func_00140738(sHPC140738* self)
{
    self->v[0][2] = 0;
    self->v[0][0] = 0;
    self->v[0][1] = 0;
    self->v[1][2] = 0;
    self->v[1][0] = 0;
    self->v[1][1] = 0;
    self->v[2][2] = 0;
    self->v[2][0] = 0;
    self->v[2][1] = 0;
    self->v[3][2] = 0;
    self->v[3][0] = 0;
    self->v[3][1] = 0;
    self->v[4][2] = 0;
    self->v[4][0] = 0;
    self->v[4][1] = 0;
    self->v[5][2] = 0;
    self->v[5][0] = 0;
    self->v[5][1] = 0;
    self->v[6][2] = 0;
    self->v[6][0] = 0;
    self->v[6][1] = 0;
    self->v[7][2] = 0;
    self->v[7][0] = 0;
    self->v[7][1] = 0;
    self->v[8][2] = 0;
    self->v[8][0] = 0;
    self->v[8][1] = 0;
    self->v[9][2] = 0;
    self->v[9][0] = 0;
    self->v[9][1] = 0;
    self->v[10][2] = 0;
    self->v[10][0] = 0;
    self->v[10][1] = 0;
    self->v[11][2] = 0;
    self->v[11][0] = 0;
    self->v[11][1] = 0;
    self->v[12][2] = 0;
    self->v[12][0] = 0;
    self->v[12][1] = 0;
    self->v[13][2] = 0;
    self->v[13][0] = 0;
    self->v[13][1] = 0;
    self->v[14][2] = 0;
    self->v[14][0] = 0;
    self->v[14][1] = 0;
    self->v[15][2] = 0;
    self->v[15][0] = 0;
    self->v[15][1] = 0;
    self->v[16][2] = 0;
    self->v[16][0] = 0;
    self->v[16][1] = 0;
    self->v[17][2] = 0;
    self->v[17][0] = 0;
    self->v[17][1] = 0;
    self->v[18][2] = 0;
    self->v[18][0] = 0;
    self->v[18][1] = 0;
    self->u430 = 0xFFFFFFFF;
    unsigned int* p = self->arr;
    for (int i = 63; i != -1; i--, p++) {
        *p = 0xFFFFFFFF;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001408F0__FPv);
#ifdef SKIP_ASM
void* func_001408F0(void* self)
{
    return (char*)self + 0x110;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140910__FPv);
#ifdef SKIP_ASM
void* func_00140910(void* self)
{
    return (char*)self + 0x1E0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140B80__FPv);
#ifdef SKIP_ASM
int func_00140B80(void* self)
{
    return *(int*)((char*)self + 0x86C);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140BC0__FPv);
#ifdef SKIP_ASM
int func_00140BC0(void* self)
{
    return *(int*)((char*)self + 0x874);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140BC8);
#ifdef SKIP_ASM
extern "C" int func_00140BC8(void* self)
{
    return *(int*)((char*)self + 0x874) ^ 1;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140C68__FPv);
#ifdef SKIP_ASM
void func_00140C68(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140C70__FPv);
#ifdef SKIP_ASM
void func_00140C70(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140C78__FPv);
#ifdef SKIP_ASM
void func_00140C78(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140C80__FPv);
#ifdef SKIP_ASM
void func_00140C80(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140CE0__FPv);
#ifdef SKIP_ASM
void* func_00140CE0(void* self)
{
    return self;
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_00140D80);

INCLUDE_ASM("ai/control/handplantcontrol", func_00140EE0);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141040);

INCLUDE_ASM("ai/control/handplantcontrol", func_001411C0);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141320);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141480);

INCLUDE_ASM("ai/control/handplantcontrol", func_001415C8);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141728);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141888);

INCLUDE_ASM("ai/control/handplantcontrol", func_001419E8);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141B48);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141CA8);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141E08);

INCLUDE_ASM("ai/control/handplantcontrol", func_00141F68);

INCLUDE_ASM("ai/control/handplantcontrol", func_001420C8);

INCLUDE_ASM("ai/control/handplantcontrol", func_00142228);

INCLUDE_ASM("ai/control/handplantcontrol", func_00142388);

INCLUDE_ASM("ai/control/handplantcontrol", func_001424D0);

INCLUDE_ASM("ai/control/handplantcontrol", func_00142630);

INCLUDE_ASM("ai/control/handplantcontrol", func_00142790);

INCLUDE_ASM("ai/control/handplantcontrol", func_001428F0);

INCLUDE_ASM("ai/control/handplantcontrol", func_00142A50);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00144068__FPv);
#ifdef SKIP_ASM
void func_00144068(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001441A0__FPvT0);
#ifdef SKIP_ASM
int func_001441A0(void* self, void* a1)
{
    int t0 = 2;
    *(int*)((char*)a1 + 0x8) = t0;
    *(int*)((char*)self + 0x704) = (int)((char*)*(void**)((char*)self + 0x704) - 0x1);
    return t0;
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_001441B8);

INCLUDE_ASM("ai/control/handplantcontrol", func_00144368);

extern "C" void* func_00311958(void* self);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00144610__FPv);
#ifdef SKIP_ASM
void* func_00144610(void* self)
{
    return func_00311958(self);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00144670);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here
extern "C" void func_00144670(void* self, int bit)
{
    ulong mask = (ulong)1 << bit;
    ulong a = *(ulong*)self;
    *(ulong*)self = a & ~mask;
    *(ulong*)((char*)self + 0x8) |= mask & a;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001446A0);
#ifdef SKIP_ASM
extern "C" bool func_001446A0(void* self, int bit)
{
    ulong mask = (ulong)1 << bit;
    return (*(ulong*)self & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001446B8);
#ifdef SKIP_ASM
extern "C" bool func_001446B8(void* self, int bit)
{
    ulong mask = (ulong)1 << bit;
    return (*(ulong*)((char*)self + 0x8) & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001446E8__FPv);
#ifdef SKIP_ASM
float func_001446E8(void* self)
{
    return 0.0f;
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_001446F8);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001448A8);
#ifdef SKIP_ASM
extern "C" int func_001448A8(void* self)
{
    return *(int*)((char*)self + 0x70) == 3;
}
#endif

extern "C" void* func_0013FB20(int, int);

//99.38%
INCLUDE_ASM("ai/control/handplantcontrol", func_001448B8__FPv);
#ifdef SKIP_ASM
void* func_001448B8(void* self)
{
    return func_0013FB20(1, 0xffff);
}
#endif


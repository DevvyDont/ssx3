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

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_001399E0);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);

extern "C" void func_001399E0(void* self, int state)
{
    *(int*)self = state == 5;
    char* r = *(char**)((char*)self + 0x4);
    cAirPredictor_startLaunchIntoAir(*(void**)(r + 0x788), r + 0x110, r + 0x1E0, 3333.33349609375f);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00139A18__FPv);
#ifdef SKIP_ASM
void func_00139A18(void* self)
{
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_00139A20);

INCLUDE_ASM("ai/control/handplantcontrol", func_00139C88);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013A7B0);
#ifdef SKIP_ASM
extern "C" void* func_0032E100(void* seg, const sVec4HP& a, const sVec4HP& b, int n, float r);
extern "C" float func_003342D0(void* world, void* seg, void* hit, int flags);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4HP vu0Scale_13A7B0(const sVec4HP& v, float s)
{
    sVec4HP r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4HP vu0Add_13A7B0(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0Sub_13A7B0(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

struct sVEntry_13A7B0 { short delta; short index; void (*fn)(void*, void*); };

extern "C" float func_0013A7B0(void* self, char* hit)
{
    char* rider = *(char**)((char*)self + 0x4);
    sVec4HP* pt = (sVec4HP*)(*(char**)(*(char**)(rider + 0x780) + 0x2C) + (*(int*)(rider + 0x8A0) << 5));
    sVec4HP dir = *(sVec4HP*)(rider + 0x180);
    char seg[0xB0];
    func_0032E100(seg, vu0Sub_13A7B0(*pt, vu0Scale_13A7B0(dir, 200.0f)), vu0Add_13A7B0(*pt, vu0Scale_13A7B0(dir, 200.0f)), 2, 0.574999988079071f);
    char* r2 = *(char**)((char*)self + 0x4);
    float d = func_003342D0(*(void**)(r2 + 0x860), seg, hit, *(int*)(r2 + 0x864));
    if (d >= 0.0f)
    {
        char* o = *(char**)(hit + 0x50);
        if (o)
        {
            char* h = *(char**)(o + 0xC);
            if (h)
            {
                sVEntry_13A7B0* vt = *(sVEntry_13A7B0**)(h + 0xC);
                vt[42].fn(h + vt[42].delta, hit);
            }
        }
    }
    return d;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013A8F8);
#ifdef SKIP_ASM
// PORT: cRiderAnimBase_play is declared void elsewhere; callers here match only with an int return.
extern "C" int cRiderAnimBase_play_i(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");

extern "C" void func_0013A8F8(void* self, float v)
{
    if (v < -2222.22216796875f)
    {
        cRiderAnimBase_play_i(*(void**)(*(char**)((char*)self + 0x4) + 0x784), 0x41, 0, -1.0f);
        *(int*)(*(char**)((char*)self + 0x4) + 0x2DC) = 0;
    }
    else
    {
        cRiderAnimBase_play_i(*(void**)(*(char**)((char*)self + 0x4) + 0x784), 0x40, 0, -1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013A968);
#ifdef SKIP_ASM
extern "C" void func_0013A968(void* self, float v)
{
    if (v < -2222.22216796875f)
    {
        cRiderAnimBase_play_i(*(void**)(*(char**)((char*)self + 0x4) + 0x784), 0x3F, 0, -1.0f);
        *(int*)(*(char**)((char*)self + 0x4) + 0x2DC) = 0;
    }
    else
    {
        char* r = *(char**)((char*)self + 0x4);
        float pi = 3.1415929794311523f;
        float a = *(float*)(r + 0x2DC);
        if (a < -pi)
            cRiderAnimBase_play_i(*(void**)(r + 0x784), 0x42, 0, -1.0f);
        else if (a > pi)
            cRiderAnimBase_play_i(*(void**)(r + 0x784), 0x43, 0, -1.0f);
        else if (v < -1250.0f)
            cRiderAnimBase_play_i(*(void**)(r + 0x784), 0x3E, 0, -1.0f);
        else
            cRiderAnimBase_play_i(*(void**)(r + 0x784), 0x3D, 0, -1.0f);
    }
}
#endif

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

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013AD20);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cRider_doLeanPoseAdjust(void* self, void* q);
struct sQuadHP_0013AD20 { float x, y, z, w; } __attribute__((aligned(16)));

extern "C" void func_0013AD20(void* self)
{
    char* s = (char*)self;
    char* r = *(char**)(s + 0x50);
    cRider_doLeanPoseAdjust(r, r + 0x110);
    cRider_updateOrientationImplicit(*(void**)(s + 0x50));
    char* a = *(char**)(s + 0x50);
    // G1
    *(float*)(a + 0x1F8) = 0.0f;
    *(float*)(a + 0x1F0) = 0.0f;
    *(float*)(a + 0x1F4) = 0.0f;
    // E1
    char* b = *(char**)(s + 0x50);
    *(float*)(b + 0x210) = 0.0f;
    *(float*)(b + 0x208) = 0.0f;
    *(float*)(b + 0x20C) = 0.0f;
    char* c = *(char**)(s + 0x50);
    *(float*)(c + 0x258) = 0.0f;
    *(float*)(c + 0x250) = 0.0f;
    *(float*)(c + 0x254) = 0.0f;
    *(int*)(s + 0x10) = 0;
    *(int*)(s + 0x14) = 0;
    *(int*)(s + 0x18) = 0;
    *(int*)(*(char**)(s + 0x50) + 0x2DC) = 0;
    char* d = *(char**)(s + 0x50);
    *(float*)(d + 0x260) = 0.05000000447034836f;
    *(int*)(d + 0x264) = 0;
    *(int*)(s + 0x20) = 0;
    *(int*)(s + 0x1C) = 0;
    *(sQuadHP_0013AD20*)(s + 0x30) = *(sQuadHP_0013AD20*)(*(char**)(s + 0x50) + 0x110);
    *(int*)(s + 0x40) = 0;
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_0013ADC0);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013AF28);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013BD80);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013BFA8);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C140);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013C5A0);
#ifdef SKIP_ASM
extern "C" void func_0013C5A0(void* self)
{
    char* a = *(char**)((char*)self + 0x50);
    *(float*)(a + 0x25C) = *(float*)(a + 0x264) = -0.25f;
    *(float*)(a + 0x260) = 0.0f;
    char* b = *(char**)((char*)self + 0x50);
    *(float*)(b + 0x264) = 1.0f;
    *(float*)(b + 0x260) = 0.03333333507180214f;
    *(int*)(*(char**)((char*)self + 0x50) + 0x5AC) = 0;
}
#endif

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

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013C7A8);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" int func_001298C8();

struct sHpVec4_13C7A8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sHpEntry_13C7A8 {
    char pad0[0x14];
    float m14;
    float m18;
    char pad1C[0xB0 - 0x1C];
};

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void vu0ScaleEq_13C7A8(sHpVec4_13C7A8& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

struct sHpCtl_13C7A8 {
    int m0;
    float m4;
    float m8;
    int mC;
    int m10;
    int m14;
    char* mRider;
};

extern "C" void func_0013C7A8(sHpCtl_13C7A8* self)
{
    self->m0 = 0;
    char* r = self->mRider;
    sHpEntry_13C7A8* tbl = *(sHpEntry_13C7A8**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x44);
    sHpEntry_13C7A8* e = &tbl[*(int*)(r + 0x438)];
    float k = *(float*)(*(char**)(r + 0x780) + 0x140);
    self->m4 = k * e->m14;
    self->m8 = k * e->m18;
    *(sHpVec4_13C7A8*)(r + 0x390) = *(sHpVec4_13C7A8*)(r + 0x370);
    self->mC = 0;
    int t = func_001298C8();
    self->m10 = t;
    int d = t - self->m14;
    float f;
    if (d > 0x28)
        f = (d - 0x28) * 0.010000000707805157f + 0.699999988079071f;
    else
        f = 0.699999988079071f;
    vu0ScaleEq_13C7A8(*(sHpVec4_13C7A8*)(self->mRider + 0x1E0), f <? 1.0f);
}
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C878);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013C948);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013CCF0);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013D028);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void func_00148E68(void* iface, int rider, int b);

struct sCurvePt_13D028 { float x, y; };
extern sCurvePt_13D028* D_004A1140;

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float hpAbs_13D028(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float hpLerp_13D028(const sCurvePt_13D028& a, const sCurvePt_13D028& b, float x)
{
    float y0 = a.y;
    x -= a.x;
    return y0 + (b.y - y0) * x / (b.x - a.x);
}

static inline float hpCurve_13D028(const sCurvePt_13D028* p, float x)
{
    if (p[1].x < x)
    {
        if (p[2].x < x)
        {
            if (p[3].x < x)
                return p[3].y;
            return hpLerp_13D028(p[2], p[3], x);
        }
        return hpLerp_13D028(p[1], p[2], x);
    }
    if (x < p[0].x)
        return p[0].y;
    return hpLerp_13D028(p[0], p[1], x);
}

extern "C" float func_0013D028(void* self, float* v, float a, float b, float c)
{
    if (a < 0.0f && 0.0f < c)
    {
        a = a * c;
    }
    else if (0.0f < a && c < 0.0f)
    {
        a = a * -c;
    }
    else
    {
        return 0.0f;
    }
    a = a * hpCurve_13D028(D_004A1140, hpAbs_13D028(b) * 0.035999998450279236f);
    a = a * -v[2];
    func_00148E68(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(*(char**)((char*)self + 0x18) + 0x86C), *(int*)(*(char**)((char*)self + 0x18) + 0xB34));
    float s = *(float*)(*(char**)((char*)self + 0x18) + 0x2FC);
    float one = 1.0f;
    if (0.0f < s)
        a = a * (one / (s * 4.008637428283691f + one));
    return a;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013D1B8);
#ifdef SKIP_ASM
extern "C" void* func_0032E100(void* seg, const sVec4HP& a, const sVec4HP& b, int n, float r);
extern void* D_004FF120[];

struct sHit_13D1B8 {
    sVec4HP pos;            // 0x00
    sVec4HP normal;         // 0x10
    sVec4HP v20;            // 0x20
    float pad30[4];         // 0x30
    float dist;             // 0x40
    int pad44[2];           // 0x44
    int m4C;                // 0x4C
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[5];           // 0x58
    float m6C;              // 0x6C
    float m70;              // 0x70
    int pad74[3];           // 0x74
    sHit_13D1B8() {}
} __attribute__((aligned(16)));

extern "C" int func_00333EF8(void* world, void* seg, sHit_13D1B8* hits, int max, int flags);
extern sVec4HP D_004FF160;
extern sVec4HP D_004FF120_v __asm__("D_004FF120");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4HP vu0Scale_13D1B8(const sVec4HP& v, float s)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4HP vu0Add_13D1B8(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0Sub_13D1B8(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_13D1B8(const sVec4HP& a, const sVec4HP& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product, by-value first operand).
static inline float vu0DotV_13D1B8(sVec4HP a, const sVec4HP& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4HP vu0Normalize_13D1B8(const sVec4HP& v)
{
    sVec4HP r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec4HP vu0Cross_13D1B8(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_13D1B8(const sVec4HP& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_13D1B8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}


// Non-POD by-value vector (user copy ctor => passed by invisible reference, no callee copy).
struct sVec4P_13D1B8 {
    float x, y, z, w;
    sVec4P_13D1B8(const sVec4HP& v) { *(sVec4HP*)this = v; }
    sVec4P_13D1B8(const sVec4P_13D1B8& v) { *(sVec4HP*)this = *(const sVec4HP*)&v; }
} __attribute__((aligned(16)));

static inline sVec4HP vu0SubV_13D1B8(sVec4P_13D1B8 a, const sVec4HP& b)
{
    return vu0Sub_13D1B8(*(sVec4HP*)&a, b);
}

static inline int isBetter_13D1B8(sHit_13D1B8* a, sHit_13D1B8* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

struct sVEnt_13D1B8 { short delta; short index; void (*fn)(void*, void*); };

#define RIDER (*(char**)((char*)self + 0x18))
#define RV(off) (*(sVec4HP*)(RIDER + (off)))

extern "C" int func_0013D1B8(void* self, sHit_13D1B8* out, float* dist)
{
    RV(0x380) = RV(0x370);
    sVec4HP pos = vu0Add_13D1B8(RV(0x110), vu0Scale_13D1B8(RV(0x3B0), *(float*)(*(char**)(RIDER + 0x780) + 0x140) * (*(float*)(RIDER + 0x1F0) * 45.0f)));
    sVec4HP a = vu0Add_13D1B8(pos, vu0Scale_13D1B8(RV(0x370), -100.0f));
    sVec4HP b = vu0Add_13D1B8(pos, vu0Scale_13D1B8(RV(0x370), 200.0f));
    char seg[0xB0];
    func_0032E100(seg, a, b, 2, 0.5f);
    sHit_13D1B8 hits[64];
    int n = func_00333EF8(*(void**)(RIDER + 0x860), seg, hits, 0x40, *(int*)(RIDER + 0x864));
    if (n == 0) {
        RV(0x370) = D_004FF160;
        RV(0x3D0) = D_004FF120_v;
        RV(0x3A0) = vu0Normalize_13D1B8(vu0SubV_13D1B8(RV(0x1B0), vu0Scale_13D1B8(RV(0x370), vu0DotV_13D1B8(RV(0x1B0), RV(0x370)))));
        RV(0x3B0) = vu0Cross_13D1B8(RV(0x370), RV(0x3A0));
        *(int*)(RIDER + 0x438) = 0;
        *dist = 0.0f;
        return 0;
    }
    int best = 0;
    float up0 = vu0Dot_13D1B8(hits[0].normal, RV(0x370));
    float bestDiff = absf_13D1B8(hits[0].dist - 0.5f);
    int ok = up0 >= 0.3f;
    for (int i = 1; i < n; i++) {
        float d = absf_13D1B8(hits[i].dist - 0.5f);
        float up = vu0Dot_13D1B8(hits[i].normal, RV(0x370));
        if (!ok) {
            if (up >= 0.3f) {
                bestDiff = d;
                best = i;
                ok = 1;
                continue;
            }
        } else if (up < 0.3f) {
            continue;
        }
        if (d < bestDiff) {
            bestDiff = d;
            best = i;
        } else if (d == bestDiff) {
            if (isBetter_13D1B8(&hits[i], &hits[best])) {
                best = i;
            }
        }
    }
    *out = hits[best];
    if (out->tri != 0) {
        *(int*)(RIDER + 0x430) = *(int*)(out->tri + 0x150);
        *(float*)(RIDER + 0xAAC) = out->m6C;
        *(float*)(RIDER + 0xAB0) = out->m70;
        *(short*)(RIDER + 0x2D4) = *(short*)(out->tri + 0xA);
    } else {
        *(unsigned int*)(RIDER + 0x430) = 0xFFFFFFFF;
    }
    if (out->obj != 0) {
        char* h = *(char**)(out->obj + 0xC);
        if (h) {
            sVEnt_13D1B8* vt = *(sVEnt_13D1B8**)(h + 0xC);
            vt[42].fn(h + vt[42].delta, out);
        }
    }
    *(int*)(RIDER + 0x438) = out->m4C;
    if (*(int*)(RIDER + 0x438) == -1) {
        *(int*)(RIDER + 0x438) = 0;
    }
    RV(0x370) = out->normal;
    RV(0x460) = out->pos;
    RV(0x3D0) = out->v20;
    RV(0x3A0) = vu0Normalize_13D1B8(vu0SubV_13D1B8(RV(0x1B0), vu0Scale_13D1B8(RV(0x370), vu0DotV_13D1B8(RV(0x1B0), RV(0x370)))));
    RV(0x3B0) = vu0Cross_13D1B8(RV(0x370), RV(0x3A0));
    sVec4HP delta = vu0Sub_13D1B8(pos, out->pos);
    *(float*)(RIDER + 0x454) = vu0Dot_13D1B8(delta, RV(0x370));
    *dist = vu0Length_13D1B8(vu0Sub_13D1B8(delta, vu0Scale_13D1B8(RV(0x370), *(float*)(RIDER + 0x454))));
    return 1;
}
#undef RV
#undef RIDER
#endif

INCLUDE_ASM("ai/control/handplantcontrol", func_0013D818);

INCLUDE_ASM("ai/control/handplantcontrol", func_0013F178);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013F410);
#ifdef SKIP_ASM
extern "C" int func_001298C8();

extern "C" void func_0013F410(void* self)
{
    char* a = *(char**)((char*)self + 0x18);
    *(int*)(a + 0x2C4) = 0;
    *(float*)(a + 0x2C0) = 0.05000000447034836f;
    char* b = *(char**)((char*)self + 0x18);
    *(float*)(b + 0x20C) = 0.05000000447034836f;
    *(int*)(b + 0x210) = 0;
    char* c = *(char**)((char*)self + 0x18);
    *(float*)(c + 0x2CC) = 1.6666667461395264f;
    *(int*)(c + 0x2D0) = 0;
    if (*(int*)((char*)self + 0x14) == -1)
        *(int*)((char*)self + 0x14) = func_001298C8() - 500;
    else
        *(int*)((char*)self + 0x14) = func_001298C8();
}
#endif

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

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00141480);
#ifdef SKIP_ASM
extern "C" void cRider_cRider(void* self);

struct sVEntry_00141480 { short delta; short index; void* fn; };
struct sVtblA_00141480 { sVEntry_00141480 e[9]; } __attribute__((aligned(8)));
struct sVtblB_00141480 { sVEntry_00141480 e[22]; } __attribute__((aligned(8)));

extern const sVtblA_00141480 D_004595C0_00141480 __asm__("D_004595C0");
extern char D_00459B90[];
extern const sVtblB_00141480 D_00459608_00141480 __asm__("D_00459608");

// PORT: g++ 2.95 virtual-base construction. The class derives virtually from cRider (vbase pointer at
// +0xA0, vbase at +0xB0); when not most-derived, g++ copies cRider's overridden vtables to the stack
// and fixes up the this-deltas (expand_upcast_fixups). Written out by hand here.
extern "C" void* func_00141480(void* self, int inChrg)
{
    if (inChrg)
    {
        char* vb = (char*)self + 0xB0;
        *(char**)((char*)self + 0xA0) = vb;
        cRider_cRider(vb);
    }
    *(void**)(*(char**)((char*)self + 0xA0) + 0x6E8) = (void*)&D_004595C0_00141480;
    *(void**)(*(char**)((char*)self + 0xA0) + 0x6D0) = D_00459B90;
    *(void**)(*(char**)((char*)self + 0xA0) + 0x6C0) = (void*)&D_00459608_00141480;
    if (!inChrg)
    {
        sVtblA_00141480 t1 = D_004595C0_00141480;
        *(void**)(*(char**)((char*)self + 0xA0) + 0x6E8) = &t1;
        char* base = *(char**)((char*)self + 0xA0) - 0xB0;
        int d = (char*)self - base;
        t1.e[1].delta = D_004595C0_00141480.e[1].delta + d;
        sVtblB_00141480 t2 = D_00459608_00141480;
        *(void**)(*(char**)((char*)self + 0xA0) + 0x6C0) = &t2;
        t2.e[1].delta = D_00459608_00141480.e[1].delta + d;
    }
    return self;
}
#endif

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

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00142388);
#ifdef SKIP_ASM
extern "C" void cRider_cRider(void* self);

struct sVEntry_00142388 { short delta; short index; void* fn; };
struct sVtblA_00142388 { sVEntry_00142388 e[9]; } __attribute__((aligned(8)));
struct sVtblB_00142388 { sVEntry_00142388 e[22]; } __attribute__((aligned(8)));

extern const sVtblA_00142388 D_00458C10_00142388 __asm__("D_00458C10");
extern char D_00459B90[];
extern const sVtblB_00142388 D_00458C58_00142388 __asm__("D_00458C58");

// PORT: g++ 2.95 virtual-base construction. The class derives virtually from cRider (vbase pointer at
// +0x80, vbase at +0x90); when not most-derived, g++ copies cRider's overridden vtables to the stack
// and fixes up the this-deltas (expand_upcast_fixups). Written out by hand here.
extern "C" void* func_00142388(void* self, int inChrg)
{
    if (inChrg)
    {
        char* vb = (char*)self + 0x90;
        *(char**)((char*)self + 0x80) = vb;
        cRider_cRider(vb);
    }
    *(void**)(*(char**)((char*)self + 0x80) + 0x6E8) = (void*)&D_00458C10_00142388;
    *(void**)(*(char**)((char*)self + 0x80) + 0x6D0) = D_00459B90;
    *(void**)(*(char**)((char*)self + 0x80) + 0x6C0) = (void*)&D_00458C58_00142388;
    if (!inChrg)
    {
        sVtblA_00142388 t1 = D_00458C10_00142388;
        *(void**)(*(char**)((char*)self + 0x80) + 0x6E8) = &t1;
        char* base = *(char**)((char*)self + 0x80) - 0x90;
        int d = (char*)self - base;
        t1.e[1].delta = D_00458C10_00142388.e[1].delta + d;
        sVtblB_00142388 t2 = D_00458C58_00142388;
        *(void**)(*(char**)((char*)self + 0x80) + 0x6C0) = &t2;
        t2.e[1].delta = D_00458C58_00142388.e[1].delta + d;
    }
    return self;
}
#endif

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


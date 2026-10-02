#include "common.h"

INCLUDE_ASM("ai/computer", cComputer_setIndividualRiderDifficulty);

INCLUDE_ASM("ai/computer", cComputer_updateRiderDifficulty);

//100%
INCLUDE_ASM("ai/computer", func_0010C9A8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00144C98(void* iface);
extern "C" void func_00148AA8(void* iface, int a1, int a2);

struct sVEntry10C9A8 { short delta; short index; int (*fn)(void*); };

static inline int riderV7_10C9A8(char* rider)
{
    char* obj = rider + 0x6C0;
    sVEntry10C9A8* vt = *(sVEntry10C9A8**)obj;
    return vt[7].fn(obj + vt[7].delta);
}

extern "C" void func_0010C9A8(void* self)
{
    void* s = cBE_getInterface_Fv(cBE_getBE(), 3);
    void* g = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (func_00144C98(g) == 2)
    {
        func_00148AA8(s, riderV7_10C9A8(*(char**)((char*)self + 0x18)), 7);
    }
    else if (func_00144C98(g) == 1)
    {
        func_00148AA8(s, riderV7_10C9A8(*(char**)((char*)self + 0x18)), 4);
    }
    else
    {
        func_00148AA8(s, riderV7_10C9A8(*(char**)((char*)self + 0x18)), 1);
    }
}
#endif

extern "C" void cComputer_updateRiderDifficulty(void*);

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("ai/computer", func_0010CAB8);
#ifdef SKIP_ASM
extern "C" void func_0010CAB8(void* self)
{
    cComputer_updateRiderDifficulty(self);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010CAD8);
#ifdef SKIP_ASM
extern "C" int func_0010C258(void* self);
extern "C" int func_0010C320(void* self);
extern "C" int func_0010C3B8(void* self);
extern "C" int func_0010CF68(void* self, float t);
extern "C" int func_0010CD20(void* self, float t);
extern "C" int func_0010CBE0(void* self, float t);

extern "C" int func_0010CAD8(void* self, float t)
{
    int r = -1;
    int skip = *(int*)((char*)self + 0xE20) != 0 && t < 1.0f;
    if (!skip && t > 0.75f && t != 100.0f)
    {
        if (func_0010C258(self)) r = func_0010CF68(self, t);
        if (r == -1)
        {
            if (func_0010C320(self)) r = func_0010CD20(self, t);
            if (r == -1)
            {
                if (func_0010C3B8(self)) r = func_0010CBE0(self, t);
            }
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010CBE0);
#ifdef SKIP_ASM
unsigned int AIrand();
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned short func_001500D8(void* self, int a1, int a2);
extern "C" float func_00120038(void* self);
extern "C" float func_00312820(char* self, int a1, int frame);

struct sVEntry10CBE0 { short delta; short index; int (*fn)(void*); };

static inline int riderV7_10CBE0(char* rider)
{
    char* obj = rider + 0x6C0;
    sVEntry10CBE0* vt = *(sVEntry10CBE0**)obj;
    return vt[7].fn(obj + vt[7].delta);
}

struct sComputer_0010CBE0
{
    char pad0[0x18];
    char* mRider;       // 0x18
    char pad1C[0xE24 - 0x1C];
    int fE24;           // 0xE24
    int fE28;           // 0xE28
    float fE2C;         // 0xE2C
    float fE30;         // 0xE30
    float fE34;         // 0xE34
    char padE38[0xE7C - 0xE38];
    int a[15];          // 0xE7C
};

extern "C" int func_0010CBE0(void* p, float t)
{
    sComputer_0010CBE0* self = (sComputer_0010CBE0*)p;
    self->fE24 = 0;
    self->fE28 = 0;
    self->fE30 = t;
    int idx = AIrand() % 15;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    int anim = func_001500D8(iface, riderV7_10CBE0(self->mRider), idx);
    float spd = func_00120038(self->mRider);
    float a = func_00312820(*(char**)(self->mRider + 0x784), anim, 2) / spd;
    float b = func_00312820(*(char**)(self->mRider + 0x784), anim, 1) / spd;
    if (t < a) return -1;
    self->fE34 = t - a;
    self->fE2C = a - b;
    if (idx != -1 && idx < 15)
    {
        self->a[idx]++;
    }
    return idx;
}
#endif

INCLUDE_ASM("ai/computer", func_0010CD20);

INCLUDE_ASM("ai/computer", func_0010CF68);

//100%
INCLUDE_ASM("ai/computer", func_0010D170);
#ifdef SKIP_ASM
struct sComputer_0010D170
{
    char pad[0xE7C];
    int a[15];
    int b[15];
    int c[15];
};

extern "C" void func_0010D170(sComputer_0010D170* self)
{
    for (int i = 0; i < 15; i++)
    {
        self->a[i] = 0;
        self->b[i] = 0;
        self->c[i] = 0;
    }
}
#endif

INCLUDE_ASM("ai/computer", func_0010D1A0);

INCLUDE_ASM("ai/computer", func_0010D410);

//100%
INCLUDE_ASM("ai/computer", func_0010D870);
#ifdef SKIP_ASM
extern "C" float func_0010D870(void* self, int a1, int a2, int a3)
{
    short s = *(short*)((char*)self + 0xE00);
    if (s == 0)
    {
        if (!a1)
            return 0.0f;
    }
    else if (s == 1)
    {
        if (!a2)
            return 0.0f;
    }
    else if (s == 2)
    {
        if (!a3)
            return 0.0f;
    }
    else if ((a1 == a2) != a3)
        return 0.0f;
    int n = 4;
    if (a1)
        n--;
    if (a2)
        n--;
    if (a3)
        n--;
    return n * 33.29999923706055f;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010D8F8);
#ifdef SKIP_ASM
struct sComputer_0010D8F8
{
    int a;
    int b;
    char pad[0x1C];
};

extern "C" int func_0010D8F8(void* self)
{
    sComputer_0010D8F8* p = *(sComputer_0010D8F8**)((char*)self + 0x18);
    for (int i = 0; i < 6; i++, p++)
    {
        if (p->a != 0 && p->b != 0)
            return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010D9E8);
#ifdef SKIP_ASM
extern "C" int func_0010D9E8(void* self, int a1)
{
    void* p = *(void**)((char*)self + 0x18);
    int r = 0;
    if (*(int*)((char*)p + 0xf0) != 0) {
        r = *(int*)((char*)p + 0xf8) == a1;
    }
    return r;
}
#endif

INCLUDE_ASM("ai/computer", func_0010DA10);

INCLUDE_ASM("ai/computer", func_0010DBF0);

//100%
INCLUDE_ASM("ai/computer", func_0010DEB0);
#ifdef SKIP_ASM
extern void* D_004A28A8;

extern "C" int func_0010DEB0(void* self)
{
    int i = func_0010D8F8(self);
    if (i < 0)
        return 0;
    return *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + (i << 2) + 0x28);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010DEF0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
float func_00113128(void* self);
extern signed char D_00535C11[];

extern "C" float func_0010DEF0(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    // PORT: func_0010DEB0 returns a pointer as int (unit declaration).
    int p = func_0010DEB0(self);
    if (p == 0) return 1.0f;
    char* r = *(char**)((char*)self + 0x18);
    if (*(int*)(r + 0xE4) == 0) return 1.0f;
    if (D_00535C11[0] == 2) return 1.0f;
    if (*(float*)(r + 0x4D0) < 50000.0f) return 1.0f;
    float m;
    float d = func_00113128((void*)p) - *(float*)(*(char**)((char*)self + 0x18) + 0x4D0);
    char* r2 = *(char**)((char*)self + 0x18);
    float k = *(float*)(r2 + 0xDC);
    if (d < k && (*(int*)(r2 + 0xE4) == 2 || *(int*)(r2 + 0xE4) == 3))
    {
        m = ((d - k) / k + 1.0f) * 1.100000023841858f;
    }
    else
    {
        char* r3 = *(char**)((char*)self + 0x18);
        float k2 = *(float*)(r3 + 0xE0);
        if (k2 < d && (*(int*)(r3 + 0xE4) == 1 || *(int*)(r3 + 0xE4) == 3))
        {
            m = 1.0f / ((d - k2) / k2 + 1.0f);
            m *= 0.9090908765792847f;
        }
        else
            m = 1.0f;
    }
    return m;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E028);
#ifdef SKIP_ASM
extern "C" int func_001298C8();

extern "C" void func_0010E028(void* self, int mode, float t)
{
    int v;
    if (mode == 0)
    {
        v = -1;
        *(int*)((char*)self + 0x358) = 0;
    }
    else
    {
        int frames = (int)(t * 60.0f);
        v = func_001298C8() + frames;
        *(int*)((char*)self + 0x358) = mode;
    }
    *(int*)((char*)self + 0x354) = v;
}
#endif

INCLUDE_ASM("ai/computer", func_0010E098);

//100%
INCLUDE_ASM("ai/computer", func_0010E228);
#ifdef SKIP_ASM
struct sVEntry0010E228 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0028B180();
extern "C" void func_00298488(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void func_00155BF0(void* agg, int a, int b, int c);

extern "C" void func_0010E228(void* a, void* b)
{
    func_00298488(func_0028B180(), a, b);
    func_002A0A30(func_0028B180(), b, a, 1, 0);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)b + 0x6C0;
    sVEntry0010E228* eb = &(*(sVEntry0010E228**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)a + 0x6C0;
    sVEntry0010E228* ea = &(*(sVEntry0010E228**)oa)[7];
    func_00155BF0(agg, rb, ea->fn(oa + ea->delta), 0);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E2E8);
#ifdef SKIP_ASM
struct sVEntry0010E2E8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0028B180();
extern "C" void func_00298488(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void func_00155BF0(void* agg, int a, int b, int c);

extern "C" void func_0010E2E8(void* a, void* b)
{
    func_00298488(func_0028B180(), a, b);
    func_002A0A30(func_0028B180(), b, a, 1, 1);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)b + 0x6C0;
    sVEntry0010E2E8* eb = &(*(sVEntry0010E2E8**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)a + 0x6C0;
    sVEntry0010E2E8* ea = &(*(sVEntry0010E2E8**)oa)[7];
    func_00155BF0(agg, rb, ea->fn(oa + ea->delta), 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E3A8);
#ifdef SKIP_ASM
struct sVEntry0010E3A8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0028B180();
extern "C" void func_00298138(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void func_00155BF0(void* agg, int a, int b, int c);

extern "C" void func_0010E3A8(void* a, void* b)
{
    func_00298138(func_0028B180(), b, a);
    func_002A0A30(func_0028B180(), b, a, 0, 0);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)b + 0x6C0;
    sVEntry0010E3A8* eb = &(*(sVEntry0010E3A8**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)a + 0x6C0;
    sVEntry0010E3A8* ea = &(*(sVEntry0010E3A8**)oa)[7];
    func_00155BF0(agg, rb, ea->fn(oa + ea->delta), 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010E468);
#ifdef SKIP_ASM
struct sVEntry0010E468 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0028B180();
extern "C" void func_00298138(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
extern "C" void func_00298D00(void* snd, void* obj);
void* cBEAggressionInterface_getThis();
extern "C" void func_00155BF0(void* agg, int a, int b, int c);
extern "C" float func_00119400(char* self);
extern "C" void func_0010E098(void*, int, float);

extern "C" void func_0010E468(void* a, void* b)
{
    func_00298138(func_0028B180(), b, a);
    func_002A0A30(func_0028B180(), b, a, 0, 1);
    func_00298D00(func_0028B180(), b);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)b + 0x6C0;
    sVEntry0010E468* eb = &(*(sVEntry0010E468**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)a + 0x6C0;
    sVEntry0010E468* ea = &(*(sVEntry0010E468**)oa)[7];
    func_00155BF0(agg, rb, ea->fn(oa + ea->delta), 3);
    func_0010E098(b, 2, func_00119400(*(char**)((char*)b + 0x790)));
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E558);
#ifdef SKIP_ASM
extern int D_005308D0[];
extern signed char D_00535C12[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001194C0(void*, int);

extern "C" void func_0010E558(void* self, int* arg)
{
    *(int*)((char*)self + 0xE8) = *(int*)((char*)self + 0xEC);
    if (((D_005308D0[0] >> 9) & 1) == 0)
    {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C12[0] == 1)
        {
            func_001194C0(*(void**)((char*)self + 0x790), *arg);
        }
    }
}
#endif

INCLUDE_ASM("ai/computer", func_0010E5D8);

//100%
INCLUDE_ASM("ai/computer", func_0010E770);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_0029CED8(void*, int, void*, float);
extern "C" void func_002A3B18(void*, void*, int);

extern "C" void func_0010E770(void* self, float amount)
{
    *(float*)((char*)self + 0x2E8) += amount;
    func_0029CED8(func_0028B180(), 0, self, 1.0f);
    func_002A3B18(func_0028B180(), self, 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E7D0);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_0029CED8(void*, int, void*, float);
extern "C" void func_002A3B18(void*, void*, int);

extern "C" void func_0010E7D0(void* self, float amount)
{
    *(float*)((char*)self + 0x2EC) += amount;
    func_0029CED8(func_0028B180(), 1, self, 1.0f);
    func_002A3B18(func_0028B180(), self, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010E830);
#ifdef SKIP_ASM
int func_0011FE98(void* self);
extern "C" float func_00119448(char* self, float value);
extern "C" void func_0010E098(void*, int, float);
extern "C" void* func_0028B180();
extern "C" void func_0029CED8(void*, int, void*, float);

extern "C" void func_0010E830(void* self, float value)
{
    if (func_0011FE98(self) == 1 || func_0011FE98(self) == 4)
    {
        func_0010E098(self, 4, func_00119448(*(char**)((char*)self + 0x790), value));
        func_0029CED8(func_0028B180(), 2, self, value);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010E8B8);
#ifdef SKIP_ASM
extern "C" void func_0010E098(void*, int, float);
extern "C" float func_00119608(int);
extern "C" void* func_0028B180();
extern "C" void func_0029CED8(void*, int, void*, float);

extern "C" void func_0010E8B8(void* self)
{
    func_0010E098(self, 0x10, func_00119608(*(int*)((char*)self + 0x790)));
    func_0029CED8(func_0028B180(), 3, self, 1.0f);
}
#endif

INCLUDE_ASM("ai/computer", func_0010E910);

INCLUDE_ASM("ai/computer", func_0010EB30);

INCLUDE_ASM("ai/computer", func_0010F1C0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010F280);
#ifdef SKIP_ASM
extern "C" float func_00119BB0(int);
extern "C" void func_0010E098(void*, int, float);

extern "C" void func_0010F280(void* self)
{
    func_0010E098(self, 1, func_00119BB0(*(int*)((char*)self + 0x790)));
}
#endif

extern "C" void* func_0011A0E0(int);

//100%
INCLUDE_ASM("ai/computer", func_0010F2B8__FPv);
#ifdef SKIP_ASM
void* func_0010F2B8(void* self)
{
    return func_0011A0E0(*(int*)((char*)self + 0x790));
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F2D8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0011A110(int, int);
extern "C" void func_00159B08(void*, int, int);

extern "C" void func_0010F2D8(void* self, int a, int b)
{
    func_0011A110(*(int*)((char*)self + 0x790), b);
    func_00159B08(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(int*)((char*)self + 0x86C), a);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F338);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00119EF8(int, int);
extern "C" void func_001599A0(void*, int, int);

extern "C" void func_0010F338(void* self, int a)
{
    func_00119EF8(*(int*)((char*)self + 0x790), 3);
    func_001599A0(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(int*)((char*)self + 0x86C), a);
}
#endif

extern "C" void* func_0010F3B8(void* self);

//100%
INCLUDE_ASM("ai/computer", func_0010F398__FPv);
#ifdef SKIP_ASM
void* func_0010F398(void* self)
{
    return func_0010F3B8(self);
}
#endif

INCLUDE_ASM("ai/computer", func_0010F3B8);

INCLUDE_ASM("ai/computer", func_0010F560);

//100%
INCLUDE_ASM("ai/computer", func_0010F878);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int func_00155B50(void* self, int a, int b);

struct sComputer_0010F878
{
    char pad0[0x28];
    char* mRiders[20];  // 0x28
    int mCount;         // 0x78
    int pad7C;          // 0x7C
    int f80;            // 0x80
};

extern "C" void func_0010F878(sComputer_0010F878* self)
{
    if (self->f80 <= 0) return;
    for (int i = 0; i < self->mCount; i++)
    {
        *(int*)(self->mRiders[i] + 0xF0) = 0;
        *(int*)(self->mRiders[i] + 0xF8) = -1;
        for (int j = 0; j < self->mCount; j++)
        {
            if (i == j) continue;
            int a = func_00155B50(cBEAggressionInterface_getThis(), i, j);
            if (a == 4)
            {
                *(int*)(self->mRiders[i] + 0xF0) = 1;
                *(int*)(self->mRiders[i] + 0xF8) = j;
            }
            *(int*)(self->mRiders[i] + 0xFC) = a;
        }
    }
}
#endif

INCLUDE_ASM("ai/computer", func_0010F998);

//100%
INCLUDE_ASM("ai/computer", func_0010FC30);
#ifdef SKIP_ASM
struct sVEntry0010FC30 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sAi0010FC30
{
    char pad0[0x28];
    char* mRiders[20];
    int mCount;
};

extern "C" int func_0010FC30(sAi0010FC30* self, int team)
{
    int n = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* obj = self->mRiders[i] + 0x6C0;
        sVEntry0010FC30* e = &(*(sVEntry0010FC30**)obj)[9];
        if (e->fn(obj + e->delta))
        {
            if (*(int*)(self->mRiders[i] + 0xAB8) == team)
                n++;
        }
    }
    return n;
}
#endif

INCLUDE_ASM("ai/computer", func_0010FCD8);

//100%
INCLUDE_ASM("ai/computer", func_001112B8);
#ifdef SKIP_ASM
extern "C" void func_001112F8();
extern "C" void func_00111380(void*, int);

extern "C" void func_001112B8(void* self, int v)
{
    func_001112F8();
    int old = *(int*)((char*)self + 0xDE0);
    *(int*)((char*)self + 0xDE0) = v;
    func_00111380(self, old);
}
#endif

INCLUDE_ASM("ai/computer", func_001112F8);

INCLUDE_ASM("ai/computer", func_00111380);

INCLUDE_ASM("ai/computer", func_00111408);

INCLUDE_ASM("ai/computer", func_001114A0);

//100%
INCLUDE_ASM("ai/computer", func_00111538);
#ifdef SKIP_ASM
extern "C" void func_00111578();
extern "C" void func_00111630(void*, int);

extern "C" void func_00111538(void* self, int v)
{
    func_00111578();
    int old = *(int*)((char*)self + 0xDE4);
    *(int*)((char*)self + 0xDE4) = v;
    func_00111630(self, old);
}
#endif

INCLUDE_ASM("ai/computer", func_00111578);

INCLUDE_ASM("ai/computer", func_00111630);

INCLUDE_ASM("ai/computer", func_00111728);

//100%
INCLUDE_ASM("ai/computer", func_00111890);
#ifdef SKIP_ASM
extern "C" void func_002DCF28(void*);
extern "C" void func_002DAA78(void*);
extern "C" void func_002E8560(void*);
extern "C" void func_002E6640(void*);
void func_002EADC0(void*);
extern "C" void func_002EF6A0(void*);
extern "C" void func_002D4BE0(void*);
extern "C" void func_002E3930(void*);
extern "C" void func_002DF3B0(void*);
extern "C" void func_002F1148(void*);
void func_002F64E8(void*);
// PORT: cWorldPainterMan_reset is defined with an unused self param; this caller passes none.
void cWorldPainterMan_reset_noarg() __asm__("cWorldPainterMan_reset__FPv");
void* func_002306A8(void* self, int i);
extern void* D_004A28A8;

struct sVEntry00111890 { short delta; short index; void (*fn)(void*); };

extern "C" void func_00111890(void* self)
{
    char* s = (char*)self;
    func_002DCF28(s + 0x3B0);
    func_002DAA78(s + 0x470);
    func_002E8560(s + 0x520);
    func_002E6640(s + 0x610);
    func_002EADC0(s + 0x9C0);
    func_002EF6A0(s + 0xAD0);
    func_002D4BE0(s + 0xAF0);
    func_002E3930(s + 0xB00);
    func_002DF3B0(s + 0xB40);
    func_002F1148(s + 0xC70);
    func_002F64E8(s + 0xD20);
    cWorldPainterMan_reset_noarg();
    int id = *(int*)(*(char**)(s + 0x18) + 0x870);
    if (id >= 0)
    {
        char* obj = (char*)func_002306A8(*(void**)((char*)D_004A28A8 + 0x84), id);
        if (obj != 0)
        {
            sVEntry00111890* vt = *(sVEntry00111890**)(obj + 0xC);
            vt[14].fn(obj + vt[14].delta);
        }
    }
}
#endif

extern "C" void* func_002E23E0(void*);

//100%
INCLUDE_ASM("ai/computer", func_00111AA0__FPv);
#ifdef SKIP_ASM
// PORT: prototype mismatch. The project names this func_00111AA0__FPv (one
// void* param), but every caller passes (self, vecA, vecB, int, float) and the
// body forwards them to func_002E23E0 with an extra 0 flag. The unit declares
// func_002E23E0 as (void*), so both are bound by asm label.
void func_002E23E0_impl(void* self, void* a, void* b, int c, int flag, float x) __asm__("func_002E23E0");

void func_00111AA0_impl(void* self, void* a, void* b, int c, float x) __asm__("func_00111AA0__FPv");

void func_00111AA0_impl(void* self, void* a, void* b, int c, float x)
{
    func_002E23E0_impl((char*)self + 0xB40, a, b, c, 0, x);
}
#endif

INCLUDE_ASM("ai/computer", func_00111AC0);

INCLUDE_ASM("ai/computer", func_00111D98);

INCLUDE_ASM("ai/computer", func_00112180);

INCLUDE_ASM("ai/computer", func_00112338);

//100%
INCLUDE_ASM("ai/computer", func_00112588);
#ifdef SKIP_ASM
extern "C" float func_00112588(void* self, int arg1)
{
    float v = 796.0f;
    if (arg1 == 0) {
        v = 200.0f;
    }
    return v;
}
#endif

// R5900 128-bit GPR quadword, for functions that copy/return a 16-byte
// block via a single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("ai/computer", func_001125A8);
#ifdef SKIP_ASM
extern "C" cQuad128 func_001125A8(void* self)
{
    cQuad128 v = *(cQuad128*)((char*)self + 0x4A0);
    *(cQuad128*)((char*)self + 0x4B0) = v;
    return v;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_001125B8__FPv);
#ifdef SKIP_ASM
int func_001125B8(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("ai/computer", func_001125C0);

INCLUDE_ASM("ai/computer", func_001127F0);

INCLUDE_ASM("ai/computer", func_00112A50);

INCLUDE_ASM("ai/computer", func_00112D58);

INCLUDE_ASM("ai/computer", func_00112FB0);

//100%
INCLUDE_ASM("ai/computer", func_00113128__FPv);
#ifdef SKIP_ASM
float func_00113128(void* self)
{
    return *(float*)((char*)self + 0x4D0);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113130__FPv);
#ifdef SKIP_ASM
float func_00113130(void* self)
{
    return *(float*)((char*)self + 0x4D8);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113138);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_reset(char* self);

extern "C" void* func_00113138(void* self)
{
    *(unsigned int*)((char*)self + 0x30) = 0xFFFFFFFF;
    cAirPredictor_reset((char*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113170);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00113170(void* self, int flags)
{
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif


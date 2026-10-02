#include "common.h"

INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_cWorldSphTree);

INCLUDE_ASM("intersect/worldsphtree", func_003304E8);

INCLUDE_ASM("intersect/worldsphtree", func_00330540);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003306D8);
#ifdef SKIP_ASM
extern "C" float func_0032C590(void* self);

extern "C" float func_003306D8(void* self)
{
    float r;
    if (*(int*)self != 0) {
        r = func_0032C590(*(void**)((char*)self + 0x60)) * 2.0f;
    } else {
        r = -1.0f;
    }
    return r;
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00330710);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00330778__FPv);
#ifdef SKIP_ASM
int func_00330778(void* self)
{
    int t0 = *(int*)((char*)self + 0x64);
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x60) = t0;
    return t0;
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00330788);

INCLUDE_ASM("intersect/worldsphtree", func_00330828);

INCLUDE_ASM("intersect/worldsphtree", func_003308C8);

INCLUDE_ASM("intersect/worldsphtree", func_00330950);

INCLUDE_ASM("intersect/worldsphtree", func_003309D8);

INCLUDE_ASM("intersect/worldsphtree", func_00331450);

INCLUDE_ASM("intersect/worldsphtree", func_00332DB8);

INCLUDE_ASM("intersect/worldsphtree", func_00333EF8);

INCLUDE_ASM("intersect/worldsphtree", func_003342D0);

INCLUDE_ASM("intersect/worldsphtree", func_00334458);

INCLUDE_ASM("intersect/worldsphtree", func_00334680);

INCLUDE_ASM("intersect/worldsphtree", func_00334800);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00334850);
#ifdef SKIP_ASM
extern "C" void func_00327828(void* p, int flags);

extern "C" void func_00334850(void* self)
{
    void* p = *(void**)((char*)self + 0xA4);
    if (p != 0) {
        func_00327828(p, 3);
    }
    *(void**)((char*)self + 0xA4) = 0;
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00334888);

INCLUDE_ASM("intersect/worldsphtree", func_00335128);

INCLUDE_ASM("intersect/worldsphtree", func_00335960);

INCLUDE_ASM("intersect/worldsphtree", func_00335B90);

INCLUDE_ASM("intersect/worldsphtree", func_00335D78);

INCLUDE_ASM("intersect/worldsphtree", func_00336850);

INCLUDE_ASM("intersect/worldsphtree", func_003369D8);

INCLUDE_ASM("intersect/worldsphtree", func_00336D40);

INCLUDE_ASM("intersect/worldsphtree", func_00336F30);

INCLUDE_ASM("intersect/worldsphtree", func_003378C0);

INCLUDE_ASM("intersect/worldsphtree", func_00339598);

INCLUDE_ASM("intersect/worldsphtree", func_0033B748);

INCLUDE_ASM("intersect/worldsphtree", func_0033CCF8);

INCLUDE_ASM("intersect/worldsphtree", func_0033DBE8);

INCLUDE_ASM("intersect/worldsphtree", func_0033DFD8);

INCLUDE_ASM("intersect/worldsphtree", func_003400D8);

INCLUDE_ASM("intersect/worldsphtree", func_00340970);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340A08__FPv);
#ifdef SKIP_ASM
void func_00340A08(void* self)
{
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340A10__FPv);
#ifdef SKIP_ASM
float func_00340A10(void* self)
{
    return *(float*)((char*)self + 0xA0);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00340A18);
#ifdef SKIP_ASM
extern "C" float func_00340A18(void* self)
{
    if (*(int*)self == 0) {
        return -1.0f;
    }
    return *(float*)((char*)self + 0x70) * 2.0f;
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00340B18);

INCLUDE_ASM("intersect/worldsphtree", func_00340DC0);

INCLUDE_ASM("intersect/worldsphtree", func_00340FA0);

INCLUDE_ASM("intersect/worldsphtree", func_003410C0);

extern "C" void* func_003400D8(int, int);

//99.38%
INCLUDE_ASM("intersect/worldsphtree", func_00341368__FPv);
#ifdef SKIP_ASM
void* func_00341368(void* self)
{
    return func_003400D8(1, 0xffff);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00341388);

INCLUDE_ASM("intersect/worldsphtree", func_00341548);

extern void* D_004914E0[];
extern "C" void* func_0034FBF0(void*);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003415A8__FPv);
#ifdef SKIP_ASM
void* func_003415A8(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_004914E0;
    return func_0034FBF0(self);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_003415D0);

INCLUDE_ASM("intersect/worldsphtree", func_00341770);

INCLUDE_ASM("intersect/worldsphtree", func_00341818);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341A30);
#ifdef SKIP_ASM
extern "C" void func_00341A30(void* self, int a1)
{
    if (a1 == 1) {
        *(int*)((char*)self + 0x3c) = *(int*)((char*)self + 0x38);
    }
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00341A50);

INCLUDE_ASM("intersect/worldsphtree", func_00341AA0);

INCLUDE_ASM("intersect/worldsphtree", func_00341C80);

INCLUDE_ASM("intersect/worldsphtree", func_00341CF0);

INCLUDE_ASM("intersect/worldsphtree", func_00341D48);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341E48);
#ifdef SKIP_ASM
struct sWrapFloat_1E48 {
    char pad_0x00[0x10];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341E48(sWrapFloat_1E48* s)
{
    if (s->speed >= 0.0f) {
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->value = s->min + (s->value - s->max);
        }
    } else {
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->value = s->max - (s->min - s->value);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341EC0);
#ifdef SKIP_ASM
struct sBounceFloat_1EC0 {
    char pad_0x00[0x10];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341EC0(sBounceFloat_1EC0* s)
{
    if (s->speed >= 0.0f) {
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->speed = -s->speed;
            s->value = s->max - (s->value - s->max);
        }
    } else {
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->speed = -s->speed;
            s->value = s->min + (s->min - s->value);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341F38);
#ifdef SKIP_ASM
struct sClampFloat_1F38 {
    char pad_0x00[0x8];
    int atLimit;    // 0x08
    char pad_0x0C[0x4];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

extern "C" void func_00341F38(sClampFloat_1F38* s)
{
    if (s->speed >= 0.0f) {
        if (s->value == s->max) {
            s->atLimit = 1;
        }
        s->value += s->speed;
        s->last = s->value;
        if (s->value > s->max) {
            s->value = s->max;
        }
    } else {
        if (s->value == s->min) {
            s->atLimit = 1;
        }
        s->value += s->speed;
        s->last = s->value;
        if (s->value < s->min) {
            s->value = s->min;
        }
    }
}
#endif

void func_0034FCC0(void*);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00341FC8);
#ifdef SKIP_ASM
extern "C" void func_00341FC8(void* self, int a1)
{
    if (a1 != 0) {
        func_0034FCC0((char*)self + 0x30);
    }
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00341FE8);

INCLUDE_ASM("intersect/worldsphtree", func_00342150);

INCLUDE_ASM("intersect/worldsphtree", func_003421A0);

INCLUDE_ASM("intersect/worldsphtree", func_003422E8);

INCLUDE_ASM("intersect/worldsphtree", func_00342358);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003424D0);
#ifdef SKIP_ASM
extern "C" float func_003424D0(void* self)
{
    if (*(float*)((char*)self + 0x30) <= *(float*)((char*)self + 0x10) && *(float*)((char*)self + 0x4) < 0.0f) {
        return 0.0f;
    }
    if (*(float*)((char*)self + 0x30) >= *(float*)((char*)self + 0x14) && *(float*)((char*)self + 0x4) > 0.0f) {
        return 0.0f;
    }
    return *(float*)((char*)self + 0x4);
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00342538);

INCLUDE_ASM("intersect/worldsphtree", func_00342718);

INCLUDE_ASM("intersect/worldsphtree", func_00342768);

INCLUDE_ASM("intersect/worldsphtree", func_00342808);

INCLUDE_ASM("intersect/worldsphtree", func_00342880);

INCLUDE_ASM("intersect/worldsphtree", func_003429E0);

INCLUDE_ASM("intersect/worldsphtree", func_00342A88);

//100%
INCLUDE_ASM("intersect/worldsphtree", func_00342B80);
#ifdef SKIP_ASM
extern "C" void func_00342B80(void* self, int a1)
{
    if (a1 == 0x258) {
        *(int*)((char*)self + 0x2c) = *(int*)((char*)self + 0x28);
    }
}
#endif

INCLUDE_ASM("intersect/worldsphtree", func_00342BA0);

INCLUDE_ASM("intersect/worldsphtree", func_00342C08);


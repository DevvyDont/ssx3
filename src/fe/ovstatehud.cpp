#include "common.h"

struct cOVStateHUDElem {
    int field_0x0;
    int field_0x4;
    float rangeMin; // 0x8
    float rangeMax; // 0xc
    float x; // 0x10
    float y; // 0x14
    float z; // 0x18
    float w; // 0x1c
};

INCLUDE_ASM("fe/ovstatehud", cOVStateHiScoreList_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatehud", func_001E9130);
#ifdef SKIP_ASM
struct sVEntry001E9130 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sObj001E9130 {
    int field_0x0;
    sVEntry001E9130* vt;
};

extern "C" void func_0039F400(void* list, void* item);

extern "C" void func_001E9130(void* self, void* item, int a2)
{
    if (item != 0 && a2 == 5) {
        void* r = 0;
        if (*(int*)((char*)item + 0x18) == 1) {
            sObj001E9130* o = **(sObj001E9130***)((char*)self + 0x10);
            r = o->vt[4].fn((char*)o + o->vt[4].delta, self, 1);
        }
        func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001E91A8__FPvT0);
#ifdef SKIP_ASM
void func_001E91A8(void* self, void* a1)
{
    cOVStateHUDElem* e = (cOVStateHUDElem*)self;
    float* out = (float*)a1;
    out[0] = e->x;
    out[1] = e->y;
    out[2] = e->z;
    out[3] = e->w;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001E91D0__FPvT0);
#ifdef SKIP_ASM
void func_001E91D0(void* self, void* a1)
{
    cOVStateHUDElem* e = (cOVStateHUDElem*)self;
    float* out = (float*)a1;
    out[3] = e->x;
    out[0] = e->y;
    out[1] = e->z;
    out[2] = e->w;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001E91F8);
#ifdef SKIP_ASM
extern "C" void func_001E91F8(void* self, void* a1)
{
    short* in = (short*)self;
    float* out = (float*)a1;
    out[0] = in[0];
    out[1] = in[1];
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001E9220);
#ifdef SKIP_ASM
extern "C" void func_001E9220(void* self, void* a1, void* a2)
{
    short* in = (short*)self;
    float* out = (float*)a1;
    float* def = (float*)a2;
    float x = in[2];
    if (x >= 0.0f) {
        out[0] = x;
    } else {
        out[0] = 0.0f;
        if (def != 0) {
            out[0] = def[1];
        }
    }
    float y = in[3];
    if (y >= 0.0f) {
        out[1] = y;
    } else {
        out[1] = 0.0f;
        if (def != 0) {
            out[1] = def[2];
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001E9290__FPvT0);
#ifdef SKIP_ASM
float func_001E9290(void* self, void* a1)
{
    cOVStateHUDElem* e = (cOVStateHUDElem*)self;
    float t0 = e->rangeMin;
    *(float*)a1 = t0;
    *(float*)((char*)a1 + 0x4) = e->rangeMax;
    return t0;
}
#endif

INCLUDE_ASM("fe/ovstatehud", func_001E92A8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovstatehud", func_001E94E0);
#ifdef SKIP_ASM
extern "C" unsigned short* func_002C2540(unsigned short* dst, char* src);
extern "C" void func_001E92A8(void* self);
struct sVec2f { float x, y; };

extern "C" void func_001E94E0(void* self, char* str, int a2, sVec2f* pos)
{
    func_002C2540((unsigned short*)((char*)self + 0x4C), str);
    *(int*)((char*)self + 0x8) = a2;
    *(sVec2f*)((char*)self + 0x10) = *pos;
    func_001E92A8(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovstatehud", func_001E9540);
#ifdef SKIP_ASM
extern "C" unsigned short* func_002C2508(unsigned short* dst, unsigned short* src);
extern "C" void func_001E92A8(void* self);
extern "C" void func_001E9540(void* self, unsigned short* str, int a2, sVec2f* pos)
{
    func_002C2508((unsigned short*)((char*)self + 0x4C), str);
    *(int*)((char*)self + 0x8) = a2;
    *(sVec2f*)((char*)self + 0x10) = *pos;
    func_001E92A8(self);
}
#endif

INCLUDE_ASM("fe/ovstatehud", func_001E95A0);

INCLUDE_ASM("fe/ovstatehud", func_001E9A30);

INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_onCreateScreen);

INCLUDE_ASM("fe/ovstatehud", func_001EA930);

INCLUDE_ASM("fe/ovstatehud", func_001EC1F0);

INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_onRender2D);

INCLUDE_ASM("fe/ovstatehud", func_001F10F8);

INCLUDE_ASM("fe/ovstatehud", func_001F1190);

INCLUDE_ASM("fe/ovstatehud", func_001F1338);

INCLUDE_ASM("fe/ovstatehud", func_001F14B0);

INCLUDE_ASM("fe/ovstatehud", func_001F16C0);

INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderTime);

INCLUDE_ASM("fe/ovstatehud", func_001F1B30);

INCLUDE_ASM("fe/ovstatehud", func_001F1E28);

INCLUDE_ASM("fe/ovstatehud", func_001F22E8);

INCLUDE_ASM("fe/ovstatehud", func_001F2AA0);

INCLUDE_ASM("fe/ovstatehud", func_001F2DB0);

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F30C0__FPv);
#ifdef SKIP_ASM
void* func_001F30C0(void* self)
{
    cOVStateHUDElem* e = (cOVStateHUDElem*)self;
    int t0 = 0;
    e->field_0x0 = t0;
    *(int*)&e->rangeMax = t0;
    e->field_0x4 = t0;
    *(int*)&e->rangeMin = t0;
    return self;
}
#endif

INCLUDE_ASM("fe/ovstatehud", func_001F30D8);

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F3170__FPv);
#ifdef SKIP_ASM
void func_001F3170(void* self)
{
    cOVStateHUDElem* e = (cOVStateHUDElem*)self;
    int t0 = 0;
    e->field_0x0 = t0;
    *(int*)&e->rangeMax = t0;
    e->field_0x4 = t0;
    *(int*)&e->rangeMin = t0;
}
#endif

INCLUDE_ASM("fe/ovstatehud", func_001F3188);

INCLUDE_ASM("fe/ovstatehud", func_001F31E0);

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F36C0);
#ifdef SKIP_ASM
extern "C" void func_001F36C0(void* self, int bit)
{
    *(int*)((char*)self + 0x4) |= (1 << bit);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F36D8);
#ifdef SKIP_ASM
extern "C" void func_001F36D8(void* self, int bit)
{
    *(int*)((char*)self + 0x4) &= ~(1 << bit);
    if (*(int*)((char*)self + 0x4) == 0) {
        *(int*)((char*)self + 0x8) = 0;
    }
}
#endif

INCLUDE_ASM("fe/ovstatehud", func_001F3700);


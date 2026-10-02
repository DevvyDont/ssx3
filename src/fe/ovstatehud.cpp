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

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F10F8);
#ifdef SKIP_ASM
extern "C" float func_0021E750(void* self, int align, int flags, float pos, float size, float scale);
extern "C" float func_0021E7A8(void* self, int align, float pos, float size, float scale);

extern "C" void func_001F10F8(void* self, float* out, float* pos, float* size, float* scale, int alignX, int alignY, int flags)
{
    out[0] = func_0021E750(self, alignX, flags, pos[0], size[0], scale[0]);
    out[1] = func_0021E7A8(self, alignY, pos[1], size[1], scale[1]);
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F30D8);
#ifdef SKIP_ASM
struct sVEntry001F30D8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sHudIds001F30D8 {
    int a;
    int b;
    int c;
    int d;
};

int GetHashValue32(char* str);
extern "C" int func_003983F0(void* self, int key);
extern void* D_004A28A8;
extern char D_0046ED78[];
extern char D_004A21F0[];

extern "C" void func_001F30D8(void* p)
{
    sHudIds001F30D8* self = (sHudIds001F30D8*)p;
    char* o = *(char**)((char*)D_004A28A8 + 0x8C);
    sVEntry001F30D8* vt = *(sVEntry001F30D8**)(o + 4);
    char* adj = o + vt[4].delta;
    int h = GetHashValue32(D_0046ED78);
    self->a = vt[4].fn(adj, h);
    void* tbl = *(void**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x48) + 8);
    self->d = func_003983F0(tbl, GetHashValue32(D_004A21F0));
    self->b = 0;
    self->c = 0;
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F3188);
#ifdef SKIP_ASM
extern "C" void func_001F3188(void* self)
{
    if (*(int*)((char*)self + 4) != 0) {
        float* t = (float*)((char*)self + 8);
        float two = 2.0f;
        *t += 0.01666666753590107f;
        while (*t > two) {
            *t -= two;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F3700);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00147318(void* self, int a1);
extern "C" void func_001A1CB8(void* self, int a1, int val);
extern void* D_00473AA8[];

extern "C" void* func_001F3700(void* self)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 0x27;
    *(void***)((char*)self + 0x8) = D_00473AA8;
    int idx = func_00147318(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    *(signed char*)((char*)self + 0x44) = 0;
    void* p = **(void***)((char*)self + 0x10);
    if (p != 0) {
        unsigned char mask = 1 << idx;
        func_001A1CB8(p, 0, mask);
        *(unsigned char*)((char*)self + 0x15) = mask;
    }
    return self;
}
#endif


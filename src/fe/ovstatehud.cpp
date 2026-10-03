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

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F1190);
#ifdef SKIP_ASM
struct sPos_001F1190 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sHudVert_001F1190 {
    float u, v, q, f0C;     // 0x00
    int r, g, b, a;         // 0x10
    sPos_001F1190 pos;      // 0x20
    sHudVert_001F1190() {}
};

static inline int texOf_001F1190(cOVStateHUDElem* e) { return e->field_0x0; }

struct sVEnt_001F1190 { short delta; short index; void (*fn)(void*, int, sHudVert_001F1190*, int); };
extern char* D_004A289C;

extern "C" void func_001F1190(void* self, cOVStateHUDElem* e, float* pos, float* size, float* scale, float* color)
{
    if (e != 0) {
        *(short*)(*(char**)(D_004A289C + 0xE84) + 0x10) = texOf_001F1190(e);
        sHudVert_001F1190 v[4];
        int a = (int)(color[0] * 128.0f);
        int r = (int)(color[1] * 128.0f);
        int g = (int)(color[2] * 128.0f);
        int b = (int)(color[3] * 128.0f);
        for (int i = 0; i < 4; i++) {
            v[i].a = a;
            v[i].r = r;
            v[i].g = g;
            v[i].b = b;
            v[i].q = 1.0f;
        }
        float x0 = pos[0];
        float y0 = pos[1];
        float x1 = x0 + size[0] * scale[0];
        float y1 = y0 + size[1] * scale[1];
        v[0].u = e->x;
        v[0].v = e->rangeMax;
        v[1].u = e->y;
        v[1].v = e->rangeMax;
        v[2].u = e->x;
        v[2].v = e->z;
        v[3].u = e->y;
        v[3].v = e->z;
        sPos_001F1190 p;
        p.x = x0;
        p.y = y0;
        p.z = 0.0f;
        p.w = 1.0f;
        v[0].pos = p;
        p.x = x1;
        p.y = y0;
        p.z = 0.0f;
        p.w = 1.0f;
        v[1].pos = p;
        p.x = x0;
        p.y = y1;
        p.z = 0.0f;
        p.w = 1.0f;
        v[2].pos = p;
        p.x = x1;
        p.y = y1;
        p.z = 0.0f;
        p.w = 1.0f;
        v[3].pos = p;
        sVEnt_001F1190* vt = *(sVEnt_001F1190**)(D_004A289C + 0x10D8);
        vt[71].fn(D_004A289C + vt[71].delta, 4, v, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F1338);
#ifdef SKIP_ASM
struct sPos_001F1338 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sHudVert_001F1338 {
    float u, v, q, f0C;     // 0x00
    int r, g, b, a;         // 0x10
    sPos_001F1338 pos;      // 0x20
    sHudVert_001F1338() {}
};

struct sVEnt_001F1338 { short delta; short index; void (*fn)(void*, int, sHudVert_001F1338*, int); };
extern char* D_004A289C;

extern "C" void func_001F1338(void* self, float* pos, float* size, float* scale, float* color)
{
    sHudVert_001F1338 v[4];
    int a = (int)(color[0] * 128.0f);
    int r = (int)(color[1] * 255.0f);
    int g = (int)(color[2] * 255.0f);
    int b = (int)(color[3] * 255.0f);
    for (int i = 0; i < 4; i++) {
        v[i].a = a;
        v[i].r = r;
        v[i].g = g;
        v[i].b = b;
        v[i].q = 1.0f;
    }
    float x0 = pos[0];
    float y0 = pos[1];
    float x1 = x0 + size[0] * scale[0];
    float y1 = y0 + size[1] * scale[1];
    sPos_001F1338 p;
    p.x = x0;
    p.y = y0;
    p.z = 0.0f;
    p.w = 1.0f;
    v[0].pos = p;
    p.x = x1;
    p.y = y0;
    p.z = 0.0f;
    p.w = 1.0f;
    v[1].pos = p;
    p.x = x0;
    p.y = y1;
    p.z = 0.0f;
    p.w = 1.0f;
    v[2].pos = p;
    p.x = x1;
    p.y = y1;
    p.z = 0.0f;
    p.w = 1.0f;
    v[3].pos = p;
    *(short*)(*(char**)(D_004A289C + 0xE84) + 0x10) = -1;
    sVEnt_001F1338* vt = *(sVEnt_001F1338**)(D_004A289C + 0x10D8);
    vt[71].fn(D_004A289C + vt[71].delta, 4, v, 0);
}
#endif

INCLUDE_ASM("fe/ovstatehud", func_001F14B0);

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F16C0);
#ifdef SKIP_ASM
struct sColor_001F16C0 {
    float r, g, b, a;
};
struct sHudElemDef_001F16C0 {
    char pad[0x20];
    signed char align;      // 0x20
    signed char flags;      // 0x21
    char pad22[2];
};
extern "C" void cOVStateHUD1P_renderTime(void* self, int a1, int a2, int a3, int a4, int a5, float* pos, float* scale, sColor_001F16C0* color, int align, int flags, int a11);

extern "C" void func_001F16C0(void* self, int a1, int a2, int a3, int a4, int a5, sHudElemDef_001F16C0* defs, int idx, int a8, float* scaleMul, float* posOff, sColor_001F16C0* color)
{
    float pos[4];
    float scale[4];
    sColor_001F16C0 col;
    sHudElemDef_001F16C0* e = &defs[idx];
    func_001E91F8(e, pos);
    func_001E9290(e, scale);
    if (scaleMul != 0) {
        scale[0] *= scaleMul[0];
        scale[1] *= scaleMul[1];
    }
    if (posOff != 0) {
        pos[0] += posOff[0];
        pos[1] += posOff[1];
    }
    if (color == 0) {
        func_001E91A8(e, &col);
    } else {
        col = *color;
    }
    cOVStateHUD1P_renderTime(self, a1, a2, a3, a4, a5, pos, scale, &col, defs[idx].align, defs[idx].flags, a8);
}
#endif

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

